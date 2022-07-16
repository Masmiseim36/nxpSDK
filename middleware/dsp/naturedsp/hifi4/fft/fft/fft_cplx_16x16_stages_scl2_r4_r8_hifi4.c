/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ("Cadence    */
/* Libraries") are the copyrighted works of Cadence Design Systems Inc.	    */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* DSP Library                                                              */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
  NatureDSP Signal Processing Library. FFT part
    Complex-valued FFT stages with butterflies radix-4, radix-8, radix-11
    with dynamic data scaling: 16-bit data, 16-bit twiddle factors
    C code optimized for HiFi4
  IntegrIT, 2006-2014
*/
#include "NatureDSP_types.h"
#include "common.h"
#include "fft_16x16_stages.h"


/* Table of 16x16 FFT stages with dynamic scaling used in processing */
const fft_cplx16x16_stage_t fft16x16_stages_scl2_tbl[NUM_FFT_STAGE_TYPES] =
{
    NULL,                                        //    fft_stage_first_DFT2       = 0 
    fft_16x16_stage_first_scl2_DFT3,             //    fft_stage_first_DFT3       = 1 
    fft_16x16_stage_first_scl2_DFT4,             //    fft_stage_first_DFT4       = 2 
    fft_16x16_stage_first_scl2_DFT5,             //    fft_stage_first_DFT5       = 3 
    NULL,                                        //    fft_stage_first_inv_DFT2   = 4 
    ifft_16x16_stage_first_scl2_DFT3,            //    fft_stage_first_inv_DFT3   = 5 
    ifft_16x16_stage_first_scl2_DFT4,            //    fft_stage_first_inv_DFT4   = 6 
    ifft_16x16_stage_first_scl2_DFT5,            //    fft_stage_first_inv_DFT5   = 7 
    fft_16x16_stage_inner_scl2_DFT2,             //    fft_stage_inner_DFT2       = 8 
    fft_16x16_stage_inner_scl2_DFT3,             //    fft_stage_inner_DFT3       = 9 
    fft_16x16_stage_inner_scl2_DFT4,              //    fft_stage_inner_DFT4       = 10
    fft_16x16_stage_inner_scl2_DFT4x2,           //    fft_stage_inner_DFT4x2     = 11
    NULL,                                        //    fft_stage_inner_DFT4x4     = 12
    NULL,                                        //    fft_stage_inner_DFT5       = 13
    fft_16x16_stage_penultimate_scl2_DFT4x4,     //    fft_stage_penultimate_DFT4 = 14
    NULL,                                        //    fft_stage_last_DFT2        = 15
    NULL,                                        //    fft_stage_last_DFT3        = 16
    fft_16x16_stage_last_scl2_DFT4,              //    fft_stage_last_DFT4        = 17
    NULL,                                        //    fft_stage_last_DFT5        = 18
    fft_16x16_stage_last_scl2_DFT8,              //    fft_stage_last_DFT8        = 19
    fft_16x16_stage_last_scl2_DFT11,             //    fft_stage_last_DFT11       = 20
    fft_16x16_stage_inner_scl2_DFT2_V2           //    fft_inner_DFT2_REF         = 21
};

/* 16-bit radix-4 butterfly with normalization */
#define DFT4XI2(x0, x1, x2, x3, shift)\
{\
    ae_int16x4 s0, s1, d0, d1;                      \
    xtbool4 mask = 0x5;                             \
    x0 = AE_SRAA16RS(x0, shift);                    \
    x1 = AE_SRAA16RS(x1, shift);                    \
    x2 = AE_SRAA16RS(x2, shift);                    \
    x3 = AE_SRAA16RS(x3, shift);                    \
    s0 = AE_ADD16S(x0, x2); d0 = AE_SUB16S(x0, x2); \
    s1 = AE_ADD16S(x1, x3); d1 = AE_SUB16S(x1, x3); \
    /*  d1 = AE_MULJ16S(d1); */                     \
    AE_MOVT16X4(d1, AE_NEG16S(d1), mask);           \
    d1 = AE_SEL16_2301(d1, d1);                     \
    x0 = AE_ADD16S(s0, s1); x2 = AE_SUB16S(s0, s1); \
    x3 = AE_ADD16S(d0, d1); x1 = AE_SUB16S(d0, d1); \
}


/*
 *  First stage of FFT 16x16, radix-4, dynamic scaling
 */
int fft_16x16_stage_first_scl2_DFT4(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  int i, _bexp;
  const ae_int16x4 * restrict px0;
  const ae_int16x4 * restrict px1;
  const ae_int16x4 * restrict px2;
  const ae_int16x4 * restrict px3;
        ae_int16x4 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  const int R = 4; // stage radix
  const int stride = (N >> 2);
  const int shift = 3 - *bexp;
  ae_int16x4 x0, x1, x2, x3;
  ae_int32x2 x00, x01, x02, x03;
  ae_int32x2 x10, x11, x12, x13;
  ae_int16x4 tw0102, tw0311, tw1213, c1, accbits;
  int32_t sumbits;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(tw_step == 1);

  px0 = (const ae_int16x4 *)x;
  px1 = (const ae_int16x4 *)((complex_fract16*)px0 + stride);
  px2 = (const ae_int16x4 *)((complex_fract16*)px1 + stride);
  px3 = (const ae_int16x4 *)((complex_fract16*)px2 + stride);
  py0 = (      ae_int16x4 *)y;
  ptwd = (const ae_int16x4 *)tw;
  c1 = 1;
  accbits = 0;
  __Pragma("loop_count min=1");
  for (i = 0; i < (stride>>1); i++)
  {
    AE_L16X4_IP(x0, px0, sizeof(ae_int16x4));
    AE_L16X4_IP(x1, px1, sizeof(ae_int16x4));
    AE_L16X4_IP(x2, px2, sizeof(ae_int16x4));
    AE_L16X4_IP(x3, px3, sizeof(ae_int16x4));

    DFT4XI2(x0, x1, x2, x3, shift);
    x00 = AE_SEXT32X2D16_32(x0);
    x10 = AE_SEXT32X2D16_10(x0);
    AE_MUL16X4(x01, x11, x1, c1);
    AE_MUL16X4(x02, x12, x2, c1);
    AE_MUL16X4(x03, x13, x3, c1);

    AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw1213, ptwd, sizeof(ae_int16x4));

    x01 = AE_MULFC32X16RAS_H(x01, tw0102);
    x02 = AE_MULFC32X16RAS_L(x02, tw0102);
    x03 = AE_MULFC32X16RAS_H(x03, tw0311);
    x11 = AE_MULFC32X16RAS_L(x11, tw0311);
    x12 = AE_MULFC32X16RAS_H(x12, tw1213);
    x13 = AE_MULFC32X16RAS_L(x13, tw1213);

    x0 = AE_SAT16X4(x00,x01);
    x1 = AE_SAT16X4(x02,x03);
    x2 = AE_SAT16X4(x10,x11);
    x3 = AE_SAT16X4(x12,x13);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x3, py0, sizeof(ae_int16x4));
    accbits = AE_OR16(accbits, AE_ABS16S(x0));
    accbits = AE_OR16(accbits, AE_ABS16S(x2));
    accbits = AE_OR16(accbits, AE_ABS16S(x1));
    accbits = AE_OR16(accbits, AE_ABS16S(x3));
  }

  accbits = AE_OR16(accbits, AE_SEL16_2301(accbits, accbits));
  accbits = AE_OR16(accbits, AE_SHORTSWAP(accbits));
  sumbits = AE_MOVAD16_0(accbits);
  _bexp = NSA(sumbits) - 16;
  XT_MOVEQZ(_bexp,sumbits,sumbits);
  *bexp = _bexp;
  *v = v[0] * R;
  return shift;
} /* fft_16x16_stage_first_scl2_DFT4() */

/*
 *  Last stage of FFT/IFFT 16x16, radix-4, dynamic scaling
 */
int fft_16x16_stage_last_scl2_DFT4(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  const ae_int16x4 * restrict px0;
  const ae_int16x4 * restrict px1;
  const ae_int16x4 * restrict px2;
  const ae_int16x4 * restrict px3;
        ae_int16x4 * restrict py0;
        ae_int16x4 * restrict py1;
        ae_int16x4 * restrict py2;
        ae_int16x4 * restrict py3;
  const int stride = (N >> 2);
  int j, shift;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  
  shift = 2 - *bexp;

  px0 = (const ae_int16x4 *)x;
  px1 = (const ae_int16x4 *)((complex_fract16*)px0 + stride);
  px2 = (const ae_int16x4 *)((complex_fract16*)px1 + stride);
  px3 = (const ae_int16x4 *)((complex_fract16*)px2 + stride);
  py0 = (      ae_int16x4 *)y;
  py1 = (      ae_int16x4 *)((complex_fract16*)py0 + stride);
  py2 = (      ae_int16x4 *)((complex_fract16*)py1 + stride);
  py3 = (      ae_int16x4 *)((complex_fract16*)py2 + stride);
  __Pragma("loop_count min=2");
  for (j = 0; j < (stride>>1); j++)
  {
    ae_int16x4 x0, x1, x2, x3;

    AE_L16X4_IP(x0, px0, 4*sizeof(int16_t));
    AE_L16X4_IP(x1, px1, 4*sizeof(int16_t));
    AE_L16X4_IP(x2, px2, 4*sizeof(int16_t));
    AE_L16X4_IP(x3, px3, 4*sizeof(int16_t));

    DFT4XI2(x0, x1, x2, x3, shift);

    AE_S16X4_IP(x0, py0, 4*sizeof(int16_t));
    AE_S16X4_IP(x1, py1, 4*sizeof(int16_t));
    AE_S16X4_IP(x2, py2, 4*sizeof(int16_t));
    AE_S16X4_IP(x3, py3, 4*sizeof(int16_t));
  }

  return shift;

} /* fft_16x16_stage_last_scl2_DFT4() */

/* radix-4 butterfly with scaling, precision 32 bits */
#define DFT4X1RNG(x0, x1, x2, x3)\
{   \
    ae_int32x2 s0, s1, d0, d1;         \
    AE_ADDANDSUBRNG32(s0, d0, x0, x2); \
    AE_ADDANDSUBRNG32(s1, d1, x1, x3); \
    d1 = AE_MUL32JS(d1);               \
    AE_ADDANDSUB32S(x0, x2, s0, s1);   \
    AE_ADDANDSUB32S(x3, x1, d0, d1);   \
}

/* 
*   Special version of the inner stages radix-4, works for any _v[0] 
*/
int fft_16x16_stage_inner_scl2_DFT4(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *_v, int tw_step, int *bexp)
{
    const int R = 4; // stage radix
    int i, j;
    const int stride = N / R;
    const int v = _v[0]; 

    ae_p16x2s * restrict px = (ae_p16x2s *)((3 * stride - 1)*sizeof(complex_fract16) + (uintptr_t)x);
    ae_p16x2s * restrict py = (ae_p16x2s *)((3 * v - 1)*sizeof(complex_fract16) + (uintptr_t)y);
    ae_int32 * restrict ptw = (ae_int32 *)tw;
    int shift;
    const int min_shift = 3;
    shift = XT_MAX(min_shift - *bexp, 0);

    ASSERT(shift>-32 && shift<32);
    ASSERT(shift >= 0);
    ASSERT(v != stride);
    ASSERT(tw_step == 1); 

    AE_CALCRNG3();
    WUR_AE_SAR(shift);
    __Pragma("loop_count min=1"); 
    for (i = 0; i < N / R / v; i++)
    {
        ae_int16x4 _t1, _t2, _t3;
        ae_int32x2 tmp1, tmp2, tmp3;

        AE_L32_XP(tmp1, ptw, sizeof(complex_fract16));
        AE_L32_XP(tmp2, ptw, sizeof(complex_fract16));
        AE_L32_XP(tmp3, ptw, sizeof(complex_fract16));

        _t1 = AE_SHORTSWAP(AE_MOVINT16X4_FROMF32X2(tmp1));
        _t2 = AE_SHORTSWAP(AE_MOVINT16X4_FROMF32X2(tmp2));
        _t3 = AE_SHORTSWAP(AE_MOVINT16X4_FROMF32X2(tmp3));

        __Pragma("loop_count min=1");
        for (j = 0; j < v; j++)
        {
            ae_int32x2 _x0, _x1, _x2, _x3;

            AE_L16X2M_XU(_x0, px, (1 - 3 * stride)*sizeof(complex_fract16));
            AE_L16X2M_XU(_x1, px, stride*sizeof(complex_fract16));
            AE_L16X2M_XU(_x2, px, stride*sizeof(complex_fract16));
            AE_L16X2M_XU(_x3, px, stride*sizeof(complex_fract16));

            DFT4X1RNG(_x0, _x1, _x2, _x3); 

            _x1 = AE_MULFC32X16RAS_H(_x1, _t1); 
            _x2 = AE_MULFC32X16RAS_H(_x2, _t2);
            _x3 = AE_MULFC32X16RAS_H(_x3, _t3);


            AE_S16X2M_XU(_x0, py, (1 - 3 * v)*sizeof(complex_fract16));
            AE_S16X2M_XU(_x1, py, v*sizeof(complex_fract16));
            AE_S16X2M_XU(_x2, py, v*sizeof(complex_fract16));
            AE_S16X2M_XU(_x3, py, v*sizeof(complex_fract16));

            _x0 = AE_SLAA32(_x0, 8);
            _x1 = AE_SLAA32(_x1, 8);
            _x2 = AE_SLAA32(_x2, 8);
            _x3 = AE_SLAA32(_x3, 8);

            AE_RNG32X2(_x0);
            AE_RNG32X2(_x1);
            AE_RNG32X2(_x2);
            AE_RNG32X2(_x3);
        }
        py += 3*v;
    }

    AE_CALCRNG3();
    *bexp = 3 - RUR_AE_SAR();

    *_v *= R;
    return shift;
}

/*
 *  Intermediate stage of FFT/IFFT 16x16, radix-4, dynamic scaling
 */
int fft_16x16_stage_inner_scl2_DFT4x2(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  const ae_int16x4 * restrict px0; 
  const ae_int16x4 * restrict px1; 
  const ae_int16x4 * restrict px2; 
  const ae_int16x4 * restrict px3; 
        ae_int16x4 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  const int shift = 3 - *bexp;
  const int R = 4; /* stage radix */
  const int stride = (N >> 2);
  int i, j, _v, _bexp;
  ae_int16x4 x0, x1, x2, x3;
  ae_int32x2 x01, x02, x03;
  ae_int32x2 x11, x12, x13;
  ae_int16x4 tw0102, tw0311, tw1213, c1, accbits;
  int32_t sumbits;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);

  c1 = 1;
  accbits = 0;
  {
    _v = v[0];
    NASSERT(_v>=2 && (_v&1)==0);
    for (j = 0; j < (_v>>1); j++)
    {
      ptwd = (const ae_int16x4 *)tw;
      px0 = (ae_int16x4 *)x + j;
      px1 = px0 + stride/2;
      px2 = px1 + stride/2;
      px3 = px2 + stride/2;
      py0 = (ae_int16x4 *)y + j;

      __Pragma( "loop_count min=1" );
      for (i = 0; i < ((stride/_v)>>1); i++)
      {
        AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
        AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
        AE_L16X4_XP(tw1213, ptwd, (3*tw_step-2)*sizeof(ae_int16x4));

        AE_L16X4_XP(x0, px0, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x1, px1, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x2, px2, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x3, px3, _v*2*sizeof(int16_t));

        DFT4XI2(x0, x1, x2, x3, shift);
        AE_MUL16X4(x01, x11, x1, c1);
        AE_MUL16X4(x02, x12, x2, c1);
        AE_MUL16X4(x03, x13, x3, c1);

        x01 = AE_MULFC32X16RAS_H(x01, tw0102);
        x11 = AE_MULFC32X16RAS_H(x11, tw0102);
        x02 = AE_MULFC32X16RAS_L(x02, tw0102);
        x12 = AE_MULFC32X16RAS_L(x12, tw0102);
        x03 = AE_MULFC32X16RAS_H(x03, tw0311);
        x13 = AE_MULFC32X16RAS_H(x13, tw0311);

        x1 = AE_SAT16X4(x01,x11);
        x2 = AE_SAT16X4(x02,x12);
        x3 = AE_SAT16X4(x03,x13);
        AE_S16X4_XP(x0, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x1, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x2, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x3, py0, _v*2*sizeof(int16_t));

        accbits = AE_OR16(accbits, AE_ABS16S(x0));
        accbits = AE_OR16(accbits, AE_ABS16S(x1));
        accbits = AE_OR16(accbits, AE_ABS16S(x2));
        accbits = AE_OR16(accbits, AE_ABS16S(x3));

        AE_L16X4_XP(x0, px0, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x1, px1, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x2, px2, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x3, px3, _v*2*sizeof(int16_t));

        DFT4XI2(x0, x1, x2, x3, shift);
        AE_MUL16X4(x01, x11, x1, c1);
        AE_MUL16X4(x02, x12, x2, c1);
        AE_MUL16X4(x03, x13, x3, c1);

        x01 = AE_MULFC32X16RAS_L(x01, tw0311);
        x11 = AE_MULFC32X16RAS_L(x11, tw0311);
        x02 = AE_MULFC32X16RAS_H(x02, tw1213);
        x12 = AE_MULFC32X16RAS_H(x12, tw1213);
        x03 = AE_MULFC32X16RAS_L(x03, tw1213);
        x13 = AE_MULFC32X16RAS_L(x13, tw1213);

        x1 = AE_SAT16X4(x01,x11);
        x2 = AE_SAT16X4(x02,x12);
        x3 = AE_SAT16X4(x03,x13);
        AE_S16X4_XP(x0, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x1, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x2, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x3, py0, _v*2*sizeof(int16_t));

        accbits = AE_OR16(accbits, AE_ABS16S(x0));
        accbits = AE_OR16(accbits, AE_ABS16S(x1));
        accbits = AE_OR16(accbits, AE_ABS16S(x2));
        accbits = AE_OR16(accbits, AE_ABS16S(x3));
      }
    }
    accbits = AE_OR16(accbits, AE_SEL16_2301(accbits, accbits));
    accbits = AE_OR16(accbits, AE_SHORTSWAP(accbits));
    sumbits = AE_MOVAD16_0(accbits);
    _bexp = NSA(sumbits) - 16;
    XT_MOVEQZ(_bexp,sumbits,sumbits);
    *bexp = _bexp;
    *v *= R;
    return shift;
  }
} /* fft_16x16_stage_inner_scl2_DFT4x2 */

/*
 *  Penultimate stage of FFT/IFFT 16x16, radix-4, dynamic scaling
 *  Restriction: last stage must be radix 4
 */
int fft_16x16_stage_penultimate_scl2_DFT4x4(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  const ae_int16x4 * restrict px0;
  const ae_int16x4 * restrict px1;
  const ae_int16x4 * restrict px2;
  const ae_int16x4 * restrict px3;
        ae_int16x4 * restrict py0;
        ae_int16x4 * restrict py1;
        ae_int16x4 * restrict py2;
        ae_int16x4 * restrict py3;
  const ae_int16x4 * restrict ptwd;
  int j, _v, _bexp;
  ae_int16x4 x0, x1, x2, x3;
  ae_int32x2 x01, x02, x03;
  ae_int32x2 x11, x12, x13;
  ae_int16x4 tw0311, tw1213;
  ae_int16x4 tw2122, tw2331;
  ae_int16x4 tw3233, accbits;
  int32_t sumbits;
  const int shift = 3 - *bexp;
  const int R = 4; // stage radix
  const int stride = (N >> 2);

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(tw_step==1);

  _v = *v;
  NASSERT(stride/_v == 4);
  NASSERT((_v>1) && ((_v&1)==0));/* _v must be a multiple of 2 */
  accbits = 0;

  /* process the part of samples */
  px0 = (ae_int16x4 *)x;
  px1 = (ae_int16x4 *)((complex_fract16 *)px0 + stride);
  px2 = (ae_int16x4 *)((complex_fract16 *)px1 + stride);
  px3 = (ae_int16x4 *)((complex_fract16 *)px2 + stride);
  py0 = (ae_int16x4 *)y;
  py1 = (ae_int16x4 *)((complex_fract16 *)py0 + _v);
  py2 = (ae_int16x4 *)((complex_fract16 *)py1 + _v);
  py3 = (ae_int16x4 *)((complex_fract16 *)py2 + _v);

  /* preload twiddles */
  ptwd = (const ae_int16x4 *)tw;
  tw0311 = AE_L16X4_I(ptwd, 1*sizeof(ae_int16x4));
  tw1213 = AE_L16X4_I(ptwd, 2*sizeof(ae_int16x4));

  __Pragma("loop_count min=1");
  for (j = 0; j < (_v>>1); j++)
  {
    AE_L16X4_XP(x0, px0, _v*2*sizeof(int16_t));
    AE_L16X4_XP(x1, px1, _v*2*sizeof(int16_t));
    AE_L16X4_XP(x2, px2, _v*2*sizeof(int16_t));
    AE_L16X4_XP(x3, px3, _v*2*sizeof(int16_t));

    DFT4XI2(x0, x1, x2, x3, shift);

    AE_S16X4_XP(x0, py0, stride*2*sizeof(int16_t));
    AE_S16X4_XP(x1, py1, stride*2*sizeof(int16_t));
    AE_S16X4_XP(x2, py2, stride*2*sizeof(int16_t));
    AE_S16X4_XP(x3, py3, stride*2*sizeof(int16_t));

    accbits = AE_OR16(accbits, AE_ABS16S(x0));
    accbits = AE_OR16(accbits, AE_ABS16S(x2));
    accbits = AE_OR16(accbits, AE_ABS16S(x1));
    accbits = AE_OR16(accbits, AE_ABS16S(x3));

    AE_L16X4_XP(x0, px0, (2-_v)*2*sizeof(int16_t));
    AE_L16X4_XP(x1, px1, (2-_v)*2*sizeof(int16_t));
    AE_L16X4_XP(x2, px2, (2-_v)*2*sizeof(int16_t));
    AE_L16X4_XP(x3, px3, (2-_v)*2*sizeof(int16_t));

    DFT4XI2(x0, x1, x2, x3, shift);
    x01 = AE_SEXT32X2D16_32(x1);
    x11 = AE_SEXT32X2D16_10(x1);
    x02 = AE_SEXT32X2D16_32(x2);
    x12 = AE_SEXT32X2D16_10(x2);
    x03 = AE_SEXT32X2D16_32(x3);
    x13 = AE_SEXT32X2D16_10(x3);

    x01 = AE_MULFC32X16RAS_L(x01, tw0311);
    x11 = AE_MULFC32X16RAS_L(x11, tw0311);
    x02 = AE_MULFC32X16RAS_H(x02, tw1213);
    x12 = AE_MULFC32X16RAS_H(x12, tw1213);
    x03 = AE_MULFC32X16RAS_L(x03, tw1213);
    x13 = AE_MULFC32X16RAS_L(x13, tw1213);

    x1 = AE_SAT16X4(x01,x11);
    x2 = AE_SAT16X4(x02,x12);
    x3 = AE_SAT16X4(x03,x13);
    AE_S16X4_XP(x0, py0, (2-stride)*2*sizeof(int16_t));
    AE_S16X4_XP(x1, py1, (2-stride)*2*sizeof(int16_t));
    AE_S16X4_XP(x2, py2, (2-stride)*2*sizeof(int16_t));
    AE_S16X4_XP(x3, py3, (2-stride)*2*sizeof(int16_t));

    accbits = AE_OR16(accbits, AE_ABS16S(x0));
    accbits = AE_OR16(accbits, AE_ABS16S(x2));
    accbits = AE_OR16(accbits, AE_ABS16S(x1));
    accbits = AE_OR16(accbits, AE_ABS16S(x3));
  }

  /* process the next part of samples */
  px0 = (ae_int16x4 *)((complex_fract16 *)px0 + _v);
  px1 = (ae_int16x4 *)((complex_fract16 *)px0 + stride);
  px2 = (ae_int16x4 *)((complex_fract16 *)px1 + stride);
  px3 = (ae_int16x4 *)((complex_fract16 *)px2 + stride);
  py0 = (ae_int16x4 *)((complex_fract16 *)py3 + stride);
  py1 = (ae_int16x4 *)((complex_fract16 *)py0 + _v);
  py2 = (ae_int16x4 *)((complex_fract16 *)py1 + _v);
  py3 = (ae_int16x4 *)((complex_fract16 *)py2 + _v);
  /* preload twiddles */
  tw2122 = AE_L16X4_I(ptwd, 3*sizeof(ae_int16x4));
  tw2331 = AE_L16X4_I(ptwd, 4*sizeof(ae_int16x4));

  __Pragma("loop_count min=1");
  for (j = 0; j < (_v>>1); j++)
  {
    AE_L16X4_IP(x0, px0, 2*2*sizeof(int16_t));
    AE_L16X4_IP(x1, px1, 2*2*sizeof(int16_t));
    AE_L16X4_IP(x2, px2, 2*2*sizeof(int16_t));
    AE_L16X4_IP(x3, px3, 2*2*sizeof(int16_t));

    DFT4XI2(x0, x1, x2, x3, shift);
    x01 = AE_SEXT32X2D16_32(x1);
    x11 = AE_SEXT32X2D16_10(x1);
    x02 = AE_SEXT32X2D16_32(x2);
    x12 = AE_SEXT32X2D16_10(x2);
    x03 = AE_SEXT32X2D16_32(x3);
    x13 = AE_SEXT32X2D16_10(x3);
    x01 = AE_MULFC32X16RAS_H(x01, tw2122);
    x11 = AE_MULFC32X16RAS_H(x11, tw2122);
    x02 = AE_MULFC32X16RAS_L(x02, tw2122);
    x12 = AE_MULFC32X16RAS_L(x12, tw2122);
    x03 = AE_MULFC32X16RAS_H(x03, tw2331);
    x13 = AE_MULFC32X16RAS_H(x13, tw2331);

    x1 = AE_SAT16X4(x01,x11);
    x2 = AE_SAT16X4(x02,x12);
    x3 = AE_SAT16X4(x03,x13);
    AE_S16X4_IP(x0, py0, 2*2*sizeof(int16_t));
    AE_S16X4_IP(x1, py1, 2*2*sizeof(int16_t));
    AE_S16X4_IP(x2, py2, 2*2*sizeof(int16_t));
    AE_S16X4_IP(x3, py3, 2*2*sizeof(int16_t));

    accbits = AE_OR16(accbits, AE_ABS16S(x0));
    accbits = AE_OR16(accbits, AE_ABS16S(x2));
    accbits = AE_OR16(accbits, AE_ABS16S(x1));
    accbits = AE_OR16(accbits, AE_ABS16S(x3));
  }

  /* process the last part of samples */
  py0 = (ae_int16x4 *)((complex_fract16 *)py3);
  py1 = (ae_int16x4 *)((complex_fract16 *)py0 + _v);
  py2 = (ae_int16x4 *)((complex_fract16 *)py1 + _v);
  py3 = (ae_int16x4 *)((complex_fract16 *)py2 + _v);
  /* preload twiddles */
  tw3233 = AE_L16X4_I(ptwd, 5*sizeof(ae_int16x4));

  __Pragma("loop_count min=1");
  for (j = 0; j < (_v>>1); j++)
  {
    AE_L16X4_IP(x0, px0, 2*2*sizeof(int16_t));
    AE_L16X4_IP(x1, px1, 2*2*sizeof(int16_t));
    AE_L16X4_IP(x2, px2, 2*2*sizeof(int16_t));
    AE_L16X4_IP(x3, px3, 2*2*sizeof(int16_t));

    DFT4XI2(x0, x1, x2, x3, shift);
    x01 = AE_SEXT32X2D16_32(x1);
    x11 = AE_SEXT32X2D16_10(x1);
    x02 = AE_SEXT32X2D16_32(x2);
    x12 = AE_SEXT32X2D16_10(x2);
    x03 = AE_SEXT32X2D16_32(x3);
    x13 = AE_SEXT32X2D16_10(x3);
    x01 = AE_MULFC32X16RAS_L(x01, tw2331);
    x11 = AE_MULFC32X16RAS_L(x11, tw2331);
    x02 = AE_MULFC32X16RAS_H(x02, tw3233);
    x12 = AE_MULFC32X16RAS_H(x12, tw3233);
    x03 = AE_MULFC32X16RAS_L(x03, tw3233);
    x13 = AE_MULFC32X16RAS_L(x13, tw3233);

    x1 = AE_SAT16X4(x01,x11);
    x2 = AE_SAT16X4(x02,x12);
    x3 = AE_SAT16X4(x03,x13);
    AE_S16X4_IP(x0, py0, 2*2*sizeof(int16_t));
    AE_S16X4_IP(x1, py1, 2*2*sizeof(int16_t));
    AE_S16X4_IP(x2, py2, 2*2*sizeof(int16_t));
    AE_S16X4_IP(x3, py3, 2*2*sizeof(int16_t));

    accbits = AE_OR16(accbits, AE_ABS16S(x0));
    accbits = AE_OR16(accbits, AE_ABS16S(x2));
    accbits = AE_OR16(accbits, AE_ABS16S(x1));
    accbits = AE_OR16(accbits, AE_ABS16S(x3));
  }

  accbits = AE_OR16(accbits, AE_SEL16_2301(accbits, accbits));
  accbits = AE_OR16(accbits, AE_SHORTSWAP(accbits));
  sumbits = AE_MOVAD16_0(accbits);
  _bexp = NSA(sumbits) - 16;
  XT_MOVEQZ(_bexp,sumbits,sumbits);
  *bexp = _bexp;
  *v *= R;
  return shift;
} /* fft_16x16_stage_penultimate_scl2_DFT4x4() */

/*
 *  First stage of IFFT 16x16, radix-4, dynamic scaling
 */
int ifft_16x16_stage_first_scl2_DFT4(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  int i, _bexp;
  const ae_int32   * restrict px0s;
  const ae_int16x4 * restrict px0;
        ae_int16x4 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  ae_int16x4 tw0102, tw0311, tw1213, c1, accbits;
  ae_int16x4 x0, x1, x2, x3;
  ae_int32x2 x00, x01, x02, x03;
  ae_int32x2 x10, x11, x12, x13;
  int32_t sumbits;
  const int R = 4; // stage radix
  const int stride = (N >> 2);
  const int shift = 3 - *bexp;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(tw_step == 1);
  NASSERT(0==(N&7)); /* N must be a multiple of 8 */

  py0 = (ae_int16x4 *)y;
  ptwd = (const ae_int16x4 *)tw;
  c1 = 1;
  accbits = 0;

  /*
  ifft(x) = fft( [ x(1), x(end:-1:2)] )
  */
  {
    /* First butterfly radix 4 */
    AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw1213, ptwd, sizeof(ae_int16x4));

    px0s = (ae_int32 *)x;
    x03 = AE_L32_X(px0s, (N-3*stride)*2*sizeof(int16_t));
    x02 = AE_L32_X(px0s, (N-2*stride)*2*sizeof(int16_t));
    x01 = AE_L32_X(px0s, (N-1*stride)*2*sizeof(int16_t));
    x00 = AE_L32_X(px0s, 0);
    x0 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x00));
    x1 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x01));
    x2 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x02));
    x3 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x03));
    DFT4XI2(x0, x1, x2, x3, shift);
    x00 = AE_SEXT32X2D16_10(x0);
    x01 = AE_SEXT32X2D16_10(x1);
    x02 = AE_SEXT32X2D16_10(x2);
    x03 = AE_SEXT32X2D16_10(x3);
    x01 = AE_MULFC32X16RAS_H(x01, tw0102);
    x02 = AE_MULFC32X16RAS_L(x02, tw0102);
    x03 = AE_MULFC32X16RAS_H(x03, tw0311);
    x0 = AE_SAT16X4(x00,x01);
    x1 = AE_SAT16X4(x02,x03);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));

    accbits = AE_OR16(accbits, AE_ABS16S(x0));
    accbits = AE_OR16(accbits, AE_ABS16S(x1));
  }

  px0 = (ae_int16x4 *)((complex_fract16 *)x + N - 3*stride - 2);
  __Pragma("loop_count min=1");
  for (i = 1; i < (stride>>1); i++)
  {
    AE_L16X4_XP(x3, px0, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x2, px0, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x1, px0, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x0, px0, (-3*stride-2)*2*sizeof(int16_t));

    DFT4XI2(x0, x1, x2, x3, shift);
    x10 = AE_SEXT32X2D16_32(x0);
    x00 = AE_SEXT32X2D16_10(x0);
    AE_MUL16X4(x11, x01, x1, c1);
    AE_MUL16X4(x12, x02, x2, c1);
    AE_MUL16X4(x13, x03, x3, c1);

    x01 = AE_MULFC32X16RAS_L(x01, tw0311);
    x02 = AE_MULFC32X16RAS_H(x02, tw1213);
    x03 = AE_MULFC32X16RAS_L(x03, tw1213);
    AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw1213, ptwd, sizeof(ae_int16x4));
    x11 = AE_MULFC32X16RAS_H(x11, tw0102);
    x12 = AE_MULFC32X16RAS_L(x12, tw0102);
    x13 = AE_MULFC32X16RAS_H(x13, tw0311);

    x0 = AE_SAT16X4(x00,x01);
    x1 = AE_SAT16X4(x02,x03);
    x2 = AE_SAT16X4(x10,x11);
    x3 = AE_SAT16X4(x12,x13);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x3, py0, sizeof(ae_int16x4));

    accbits = AE_OR16(accbits, AE_ABS16S(x0));
    accbits = AE_OR16(accbits, AE_ABS16S(x1));
    accbits = AE_OR16(accbits, AE_ABS16S(x2));
    accbits = AE_OR16(accbits, AE_ABS16S(x3));
  }
  {
    /* Last butterfly radix 4 */
    px0s = (ae_int32 *)x + 1;
    AE_L32_XP(x13, px0s, stride*2*sizeof(int16_t));
    AE_L32_XP(x12, px0s, stride*2*sizeof(int16_t));
    AE_L32_XP(x11, px0s, stride*2*sizeof(int16_t));
    x10 = AE_L32_X(px0s, 0);
    x0 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x10));
    x1 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x11));
    x2 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x12));
    x3 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x13));
    DFT4XI2(x0, x1, x2, x3, shift);
    x10 = AE_SEXT32X2D16_10(x0);
    x11 = AE_SEXT32X2D16_10(x1);
    x12 = AE_SEXT32X2D16_10(x2);
    x13 = AE_SEXT32X2D16_10(x3);
    x11 = AE_MULFC32X16RAS_L(x11, tw0311);
    x12 = AE_MULFC32X16RAS_H(x12, tw1213);
    x13 = AE_MULFC32X16RAS_L(x13, tw1213);
    x2 = AE_SAT16X4(x10,x11);
    x3 = AE_SAT16X4(x12,x13);
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x3, py0, sizeof(ae_int16x4));

    accbits = AE_OR16(accbits, AE_ABS16S(x2));
    accbits = AE_OR16(accbits, AE_ABS16S(x3));
  }

  accbits = AE_OR16(accbits, AE_SEL16_2301(accbits, accbits));
  accbits = AE_OR16(accbits, AE_SHORTSWAP(accbits));
  sumbits = AE_MOVAD16_0(accbits);
  _bexp = NSA(sumbits) - 16;
  XT_MOVEQZ(_bexp,sumbits,sumbits);
  *bexp = _bexp;
  *v *= R;
  return shift;
} /* ifft_16x16_stage_first_scl2_DFT4() */

/* Table of twiddle factors for radix-8 butterfly */
ALIGN(8) static const int16_t __fft8_tw1[] =
{
  (int16_t)0x4000, (int16_t)0x4000,
  (int16_t)0x2D41, (int16_t)0xD2BF,
  (int16_t)0x0000, (int16_t)0xC000,
  (int16_t)0xD2BF, (int16_t)0xD2BF,
};

/*
 *  Last stage of FFT/IFFT 16x16, radix-8, dynamic scaling
 */
int fft_16x16_stage_last_scl2_DFT8(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  int j;
  const ae_int16x4 * restrict px0;
  const ae_int16x4 * restrict px1;
  const ae_int16x4 * restrict px2;
  const ae_int16x4 * restrict px3;
        ae_int16x4 * restrict py0;
        ae_int16x4 * restrict py1;
        ae_int16x4 * restrict py2;
        ae_int16x4 * restrict py3;
  const ae_int16x4 * restrict ptwd;
  ae_int16x4 tw1, tw2;
  const int R = 8; // stage radix
  const int stride = N / R;
  const int shift = 3 - *bexp;
  ae_int16x4 x0, x1, x2, x3, x4, x5, x6, x7;
  ae_int32x2 x03, x05, x07;
  ae_int32x2 x13, x15, x17;
  ae_int16x4 s0, s1, s2, s3;
  ae_int16x4 d0, d1, d2, d3;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(v[0] == stride);

  px0 = (ae_int16x4 *)x;
  px1 = px0 + stride;
  px2 = px1 + stride;
  px3 = px2 + stride;
  py0 = (ae_int16x4 *)y;
  py1 = py0 + stride;
  py2 = py1 + stride;
  py3 = py2 + stride;
  ptwd = (const ae_int16x4 *)__fft8_tw1;
  tw1 = AE_L16X4_I(ptwd, 0*sizeof(ae_int16x4));
  tw2 = AE_L16X4_I(ptwd, 1*sizeof(ae_int16x4));

  __Pragma("loop_count min=1");
  for (j = 0; j < (stride>>1); j++)
  {
    AE_L16X4_XP(x0, px0, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x1, px0, (2-stride)*2*sizeof(int16_t));
    AE_L16X4_XP(x2, px1, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x3, px1, (2-stride)*2*sizeof(int16_t));
    AE_L16X4_XP(x4, px2, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x5, px2, (2-stride)*2*sizeof(int16_t));
    AE_L16X4_XP(x6, px3, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x7, px3, (2-stride)*2*sizeof(int16_t));


    DFT4XI2(x0, x2, x4, x6, shift);
    DFT4XI2(x1, x3, x5, x7, shift);
    x03 = AE_SEXT32X2D16_32(x3);
    x13 = AE_SEXT32X2D16_10(x3);
    x05 = AE_SEXT32X2D16_32(x5);
    x15 = AE_SEXT32X2D16_10(x5);
    x07 = AE_SEXT32X2D16_32(x7);
    x17 = AE_SEXT32X2D16_10(x7);

    x03 = AE_MULFC32X16RAS_L(x03, tw1);
    x13 = AE_MULFC32X16RAS_L(x13, tw1);
    x05 = AE_MULFC32X16RAS_H(x05, tw2);
    x15 = AE_MULFC32X16RAS_H(x15, tw2);
    x07 = AE_MULFC32X16RAS_L(x07, tw2);
    x17 = AE_MULFC32X16RAS_L(x17, tw2);

    x0 = AE_SRAI16(x0,1);
    x1 = AE_SRAI16(x1,1);
    x2 = AE_SRAI16(x2,1);
    x3 = AE_SAT16X4(x03,x13);
    x4 = AE_SRAI16(x4,1);
    x5 = AE_SAT16X4(x05,x15);
    x6 = AE_SRAI16(x6,1);
    x7 = AE_SAT16X4(x07,x17);
    s0 = AE_ADD16S(x0, x1);
    d0 = AE_SUB16S(x0, x1);
    s1 = AE_ADD16S(x2, x3);
    d1 = AE_SUB16S(x2, x3);
    s2 = AE_ADD16S(x4, x5);
    d2 = AE_SUB16S(x4, x5);
    s3 = AE_ADD16S(x6, x7);
    d3 = AE_SUB16S(x6, x7);
    x0 = s0;
    x1 = s1;
    x2 = s2;
    x3 = s3;
    x4 = d0;
    x5 = d1;
    x6 = d2;
    x7 = d3;

    AE_S16X4_XP(x0, py0, stride*2*sizeof(int16_t));
    AE_S16X4_XP(x1, py0, (2-stride)*2*sizeof(int16_t));
    AE_S16X4_XP(x2, py1, stride*2*sizeof(int16_t));
    AE_S16X4_XP(x3, py1, (2-stride)*2*sizeof(int16_t));
    AE_S16X4_XP(x4, py2, stride*2*sizeof(int16_t));
    AE_S16X4_XP(x5, py2, (2-stride)*2*sizeof(int16_t));
    AE_S16X4_XP(x6, py3, stride*2*sizeof(int16_t));
    AE_S16X4_XP(x7, py3, (2-stride)*2*sizeof(int16_t));
  }

  *v *= R;
  return (shift + 1);
} /* fft_16x16_stage_last_scl2_DFT8() */


/*
DFT5 algorithm:
x - input complex vector
y - output complex vector
y = fft(x)
w1 =  exp(-1j*2*pi/5);
w2 =  exp(-1j*2*pi*2/5);

y = zeros(5,1);
s1 = (x1+x4);
s2 = (x2+x3);
d1 = (x1-x4);
d2 = (x2-x3);

y(1) = x0 + s1 + s2;
y(2) = x0 + (s1*real(w1) + s2*real(w2)) + 1j*(d1*imag(w1) + d2*imag(w2));
y(5) = x0 + (s1*real(w1) + s2*real(w2)) - 1j*(d1*imag(w1) + d2*imag(w2));
y(3) = x0 + (s1*real(w2) + s2*real(w1)) + 1j*(d1*imag(w2) - d2*imag(w1));
y(4) = x0 + (s1*real(w2) + s2*real(w1)) - 1j*(d1*imag(w2) - d2*imag(w1));

*/
ALIGN(8) static const int16_t __dft5_tw[] =
{
    (int16_t)0x278E, (int16_t)0x278E, (int16_t)0x278E, (int16_t)0x278E,
    (int16_t)0x8644, (int16_t)0x79BC, (int16_t)0x8644, (int16_t)0x79BC,
    (int16_t)0x9872, (int16_t)0x9872, (int16_t)0x9872, (int16_t)0x9872,
    (int16_t)0xB4C3, (int16_t)0x4B3D, (int16_t)0xB4C3, (int16_t)0x4B3D
};

/* twiddles should be loaded from the table above */
#define DFT5X2(x0, x1, x2, x3, x4, w1, w2, w3, w4)\
{ \
    ae_int16x4 s1, s2, d1, d2;             \
    ae_int16x4 t0, t1, t2, t3;             \
    ae_int16x4 y0, y1, y2, y3;             \
    s1 = AE_ADD16S(x1, x4);                \
    s2 = AE_ADD16S(x2, x3);                \
    d1 = AE_SUB16S(x1, x4);                \
    d2 = AE_SUB16S(x2, x3);                \
    \
    t0 = AE_MULFP16X4RAS(s1, w1);         \
    t1 = AE_MULFP16X4RAS(s2, w3);         \
    t2 = AE_MULFP16X4RAS(s1, w3);         \
    t3 = AE_MULFP16X4RAS(s2, w1);         \
    y0 = AE_ADD16S(x0, AE_ADD16S(t0, t1)); \
    y1 = AE_ADD16S(x0, AE_ADD16S(t2, t3)); \
    \
    t0 = AE_MULFP16X4RAS(d1, w2); \
    t1 = AE_MULFP16X4RAS(d2, w4); \
    t2 = AE_MULFP16X4RAS(d2, w2); \
    t3 = AE_MULFP16X4RAS(d1, w4); \
    y2 = AE_ADD16S(t0, t1);      \
    y3 = AE_SUB16S(t3, t2);      \
    y2 = AE_SEL16_2301(y2, y2);  \
    y3 = AE_SEL16_2301(y3, y3);  \
    \
    x0 = AE_ADD16S(x0, AE_ADD16S(s1, s2)); \
    x1 = AE_ADD16S(y0, y2);               \
    x2 = AE_ADD16S(y1, y3);               \
    x3 = AE_SUB16S(y1, y3);               \
    x4 = AE_SUB16S(y0, y2);               \
}

#define DFT2xI2(x0, x1) \
{                               \
    ae_int16x4 _y0, _y1;        \
    _y0 = AE_ADD16S(x0, x1);    \
    _y1 = AE_SUB16S(x0, x1);    \
    x0 = _y0;                   \
    x1 = _y1;                   \
    }
/*
    DFT10
    Prime-factor FFT algorithm
*/
inline_ int DFT10xI2(int16_t *y, int16_t *x)
{
    ae_int16x4 *px = (ae_int16x4 *)x;
    ae_int16x4 *py = (ae_int16x4 *)y;
    ae_int16x4 w1, w2, w3, w4;
    ae_int16x4 X[10];
    const ae_int16x4 * restrict ptwd_dft = (const ae_int16x4 *)__dft5_tw;
    AE_L16X4_IP(w1, ptwd_dft, sizeof(ae_int16x4));
    AE_L16X4_IP(w2, ptwd_dft, sizeof(ae_int16x4));
    AE_L16X4_IP(w3, ptwd_dft, sizeof(ae_int16x4));
    AE_L16X4_IP(w4, ptwd_dft, sizeof(ae_int16x4));

    X[0] = px[0];
    X[1] = px[2];
    X[2] = px[4];
    X[3] = px[6];
    X[4] = px[8];
    X[5] = px[5];
    X[6] = px[7];
    X[7] = px[9];
    X[8] = px[1];
    X[9] = px[3];

    DFT5X2(X[0], X[1], X[2], X[3], X[4], w1, w2, w3, w4);
    DFT5X2(X[5], X[6], X[7], X[8], X[9], w1, w2, w3, w4);

    DFT2xI2(X[0], X[5]);
    DFT2xI2(X[1], X[6]);
    DFT2xI2(X[2], X[7]);
    DFT2xI2(X[3], X[8]);
    DFT2xI2(X[4], X[9]);

    py[0] = X[0];
    py[6] = X[1];
    py[2] = X[2];
    py[8] = X[3];
    py[4] = X[4];
    py[5] = X[5];
    py[1] = X[6];
    py[7] = X[7];
    py[3] = X[8];
    py[9] = X[9];
    return 0;
}

/*
    IDFT10
    Prime-factor FFT algorithm
*/
inline_ int IDFT10xI2(int16_t *y, int16_t *x)
{
    ae_int16x4 *px = (ae_int16x4 *)x;
    ae_int16x4 *py = (ae_int16x4 *)y;
    ae_int16x4 w1, w2, w3, w4;
    ae_int16x4 X[10];
    const ae_int16x4 * restrict ptwd_dft = (const ae_int16x4 *)__dft5_tw;
    AE_L16X4_IP(w1, ptwd_dft, sizeof(ae_int16x4));
    AE_L16X4_IP(w2, ptwd_dft, sizeof(ae_int16x4));
    AE_L16X4_IP(w3, ptwd_dft, sizeof(ae_int16x4));
    AE_L16X4_IP(w4, ptwd_dft, sizeof(ae_int16x4));

    X[0] = px[0];
    X[1] = px[8];
    X[2] = px[6];
    X[3] = px[4];
    X[4] = px[2];
    X[5] = px[5];
    X[6] = px[3];
    X[7] = px[1];
    X[8] = px[9];
    X[9] = px[7];

    DFT5X2(X[0], X[1], X[2], X[3], X[4], w1, w2, w3, w4);
    DFT5X2(X[5], X[6], X[7], X[8], X[9], w1, w2, w3, w4);

    DFT2xI2(X[0], X[5]);
    DFT2xI2(X[1], X[6]);
    DFT2xI2(X[2], X[7]);
    DFT2xI2(X[3], X[8]);
    DFT2xI2(X[4], X[9]);

    py[0] = X[0];
    py[6] = X[1];
    py[2] = X[2];
    py[8] = X[3];
    py[4] = X[4];
    py[5] = X[5];
    py[1] = X[6];
    py[7] = X[7];
    py[3] = X[8];
    py[9] = X[9];
    return 0;
}

inline_ void _mpy_Q16_15(ae_int16x4 *z, ae_int32x2 *x, ae_int16x4 *y)
{
    ae_int32x2 tw = x[0];

    ae_int32x2 y01 = AE_MULFC32X16RAS_H(tw, *y);
    ae_int32x2 y11 = AE_MULFC32X16RAS_L(tw, *y);
    ae_int16x4 y1 = AE_SAT16X4(y01, y11);
    *z = y1;
}

/*
    Rader's FFT algorithm
    Matlab demo:

    Inv = zeros(1,10);
    for i=1:10
        for j=1:10
            if mod(i*j, 11)==1
                Inv(i) = j; 
            end
        end
    end
    Pin = mod(2.^(1:10).', 11); 
    Pout = Inv(mod(2.^(1:10).', 11)); 
    a = exp(-1j*2*pi/11*Inv(mod(2.^(1:10), 11)).'); 

    %x = round(1000*(randn(11,1) + 1j*randn(11, 1)));
    yref = fft(x);
    y = zeros(10,1);
    temp = x(2:end);
    xp = temp( Pin );
    c = conv([xp;xp(1:end-1)], a, 'valid'); 
    A = fft(a).*exp(-1j*2*pi/10*(0:9).');
    X = fft(xp); 
    C = X.*A; 
    c10 = ifft(C);

    y(Pout) = c + x(1);
    y = [sum(x); y];
    %[yref, y]
    diff = max(abs(yref-y))

*/
/* Coefs 'A' Q16.15, see matlab code above */
ALIGN(8) static const int32_t A_Q16_15[20] =
{
    - 3277, 0,
    3130, -10407,
    8638, 6595,
    8327, 6983,
    6784, 8491,
    0, -10868,
    6784, -8491,
    -8327, 6983,
    8638, -6595,
    -3130, -10407,

};
inline_ int DFT11x2(int16_t *y, int16_t *x, int stride /*x, y stride */,  int shift)
{
    int i;
    ae_int16x4 *px = (ae_int16x4 *)x;
    ae_int16x4 *py = (ae_int16x4 *)y;
    ae_int32x2 *pa = (ae_int32x2 *)A_Q16_15;
    ae_int16x4 X[11], dft10_X[10], C[10];
    ae_int16x4 y0;

    X[0] = AE_SRAA16RS(px[stride * 0], shift);
    X[1] = AE_SRAA16RS(px[stride * 2], shift);
    X[2] = AE_SRAA16RS(px[stride * 4], shift);
    X[3] = AE_SRAA16RS(px[stride * 8], shift);
    X[4] = AE_SRAA16RS(px[stride * 5], shift);
    X[5] = AE_SRAA16RS(px[stride * 10], shift);
    X[6] = AE_SRAA16RS(px[stride * 9], shift);
    X[7] = AE_SRAA16RS(px[stride * 7], shift);
    X[8] = AE_SRAA16RS(px[stride * 3], shift);
    X[9] = AE_SRAA16RS(px[stride * 6], shift);
    X[10] = AE_SRAA16RS(px[stride * 1], shift);

    DFT10xI2((int16_t*)dft10_X, (int16_t*)&X[1]);

    y0 = X[0];
    for (i = 0; i < 10; i++)
    {
        y0 = AE_ADD16S(y0, X[i + 1]);
        _mpy_Q16_15(&C[i], &pa[i], &dft10_X[i]);
    }

    IDFT10xI2((int16_t*)C, (int16_t*)C);

    py[0] = y0;
    py[stride * 6] = AE_ADD16S(X[0], C[0]);
    py[stride * 3] = AE_ADD16S(X[0], C[1]);
    py[stride * 7] = AE_ADD16S(X[0], C[2]);
    py[stride * 9] = AE_ADD16S(X[0], C[3]);
    py[stride * 10] = AE_ADD16S(X[0], C[4]);
    py[stride * 5] = AE_ADD16S(X[0], C[5]);
    py[stride * 8] = AE_ADD16S(X[0], C[6]);
    py[stride * 4] = AE_ADD16S(X[0], C[7]);
    py[stride * 2] = AE_ADD16S(X[0], C[8]);
    py[stride * 1] = AE_ADD16S(X[0], C[9]);

    return 0;
}

/*
*  Last stage of FFT/IFFT 16x16, radix-11, dynamic scaling
*/
int fft_16x16_stage_last_scl2_DFT11(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
    const int stride = (N / 11);
    int j, shift;
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);

    shift = 4 - *bexp;

    __Pragma("loop_count min=1");
    for (j = 0; j < (stride >> 1); j++)
    {
        DFT11x2(y + j * 4, (int16_t*)(x + j * 4), stride/2, shift);
    }

    return shift;

} /* fft_16x16_stage_last_scl2_DFT4() */

/*
*  Intermediate stage of FFT/IFFT 16x16, radix-2, dynamic scaling, no restrictions for v[0]
*/
#if 1
int fft_16x16_stage_inner_scl2_DFT2_V2(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
    const int R = 2; // stage radix
    const int min_shift = 2;
    int i;
    const int stride = N / R;

    ae_p16x2s * restrict px = (ae_p16x2s *)x;
    ae_p16x2s * restrict py = (ae_p16x2s *)y;
    ae_int32 * restrict ptw = (ae_int32 *)tw;
    int shift;


    shift = XT_MAX(min_shift - *bexp, 0);

    ASSERT(shift>-32 && shift<32);
    ASSERT(v[0] == 4); 
    ASSERT(stride != v[0]); 
    ASSERT(tw_step == 1);

    /* Reset RRANGE register */
    AE_CALCRNG3();
    /* Set scaling */
    WUR_AE_SAR(shift);

    px = (ae_p16x2s *)(((stride - 1)) *sizeof(complex_fract16) +(uintptr_t)x);
    py = (ae_p16x2s *)( (- 1)*(int32_t)sizeof(complex_fract16)+(uintptr_t)y);

    __Pragma("loop_count min=1"); 
    for (i = 0; i < (stride>>2); i++)
    {
        ae_int32x2 _x0, _x1;
        ae_int32x2 tmp;
        ae_int16x4 _t1;

        AE_L32_IP( tmp, ptw, sizeof(complex_fract16) ); 
        _t1 = AE_SHORTSWAP( AE_MOVINT16X4_FROMF32X2(tmp));
#if 0
        int j;
        // Generic code works for any v[0]
        for (j = 0; j < v[0]; j++)
        {
            _x0 = AE_L16X2M_X((const ae_p16x2s *)x, (j + v[0] * i + 0 * stride)*sizeof(complex_fract16));
            _x1 = AE_L16X2M_X((const ae_p16x2s *)x, (j + v[0] * i + 1 * stride)*sizeof(complex_fract16));

            AE_ADDANDSUBRNG32(_x0, _x1, _x0, _x1); 
            _x1 = AE_MULFC32X16RAS_H(_x1, _t1);

            AE_S16X2M_X(_x0, (ae_p16x2s *)y, (j + R * v[0] * i + 0 * v[0])*sizeof(complex_fract16));
            AE_S16X2M_X(_x1, (ae_p16x2s *)y, (j + R * v[0] * i + 1 * v[0])*sizeof(complex_fract16));

            _x0 = AE_SLAA32(_x0, 8);
            _x1 = AE_SLAA32(_x1, 8);

            AE_RNG32X2(_x0);
            AE_RNG32X2(_x1);
        }
#else
        /* 
            Optimized code for v[0] = 4
            Inner loop unrolled 4 times
        */
        {
            AE_L16X2M_XU(_x0, px,   (1-stride)*sizeof(complex_fract16) );
            AE_L16X2M_XU(_x1, px,       stride*sizeof(complex_fract16) );

            AE_ADDANDSUBRNG32(_x0, _x1, _x0, _x1);
            _x1 = AE_MULFC32X16RAS_H(_x1, _t1);

            AE_S16X2M_XU(_x0, py, (1)*sizeof(complex_fract16));
            AE_S16X2M_XU(_x1, py, v[0]*sizeof(complex_fract16));

            _x0 = AE_SLAA32(_x0, 8);    _x1 = AE_SLAA32(_x1, 8);
            AE_RNG32X2(_x0);  AE_RNG32X2(_x1);
        }
        {
            AE_L16X2M_XU(_x0, px, (1 - stride)*sizeof(complex_fract16));
            AE_L16X2M_XU(_x1, px, stride*sizeof(complex_fract16));

            AE_ADDANDSUBRNG32(_x0, _x1, _x0, _x1);
            _x1 = AE_MULFC32X16RAS_H(_x1, _t1);

            AE_S16X2M_XU(_x0, py, (1 - v[0])*sizeof(complex_fract16));
            AE_S16X2M_XU(_x1, py, v[0] * sizeof(complex_fract16));

            _x0 = AE_SLAA32(_x0, 8);    _x1 = AE_SLAA32(_x1, 8);
            AE_RNG32X2(_x0);  AE_RNG32X2(_x1);
        }
        {
            AE_L16X2M_XU(_x0, px, (1 - stride)*sizeof(complex_fract16));
            AE_L16X2M_XU(_x1, px, stride*sizeof(complex_fract16));

            AE_ADDANDSUBRNG32(_x0, _x1, _x0, _x1);
            _x1 = AE_MULFC32X16RAS_H(_x1, _t1);

            AE_S16X2M_XU(_x0, py, (1 - v[0])*sizeof(complex_fract16));
            AE_S16X2M_XU(_x1, py, v[0] * sizeof(complex_fract16));

            _x0 = AE_SLAA32(_x0, 8);    _x1 = AE_SLAA32(_x1, 8);
            AE_RNG32X2(_x0);  AE_RNG32X2(_x1);
        }
        {
            AE_L16X2M_XU(_x0, px, (1 - stride)*sizeof(complex_fract16));
            AE_L16X2M_XU(_x1, px, stride*sizeof(complex_fract16));

            AE_ADDANDSUBRNG32(_x0, _x1, _x0, _x1);
            _x1 = AE_MULFC32X16RAS_H(_x1, _t1);

            AE_S16X2M_XU(_x0, py, (1 - v[0])*sizeof(complex_fract16));
            AE_S16X2M_XU(_x1, py, v[0] * sizeof(complex_fract16));

            _x0 = AE_SLAA32(_x0, 8);    _x1 = AE_SLAA32(_x1, 8);
            AE_RNG32X2(_x0);  AE_RNG32X2(_x1);
        }
#endif
    }

    AE_CALCRNG3();
    *bexp = 3 - RUR_AE_SAR();

    *v *= R;
    return shift;
}
#endif
