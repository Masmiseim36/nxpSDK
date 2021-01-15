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
    Real-valued inverse FFT: 32-bit data, 16-bit twiddle factors
    C code optimized for HiFi4
  IntegrIT, 2006-2014
*/

/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
/* Common utility macros. */
#include "common.h"
/* FFT descriptor structure and twiddle factor tables. */
#include "fft_x16_common.h"
#include "fft_32x16_stages.h"

#define SWAP_PTR(_x, _y) {int32_t *tmp = _x; _x = _y ; _y = tmp; } 

/*
	in-place inverse split part of FFT:
	x[N+2]  input (N+2 samples)/output(N samples)
	N       size of FFT
*/
static void isplitPart_x2(int32_t *x, const int16_t *twdSplit, int shift, int N)
{
  int shiftl, shiftr;
  int i;
  const int step_back = -8;

        ae_int32x2 * restrict p_x0;
        ae_int32x2 * restrict p_x1;
  const ae_int32x2 * restrict p_x0rd;
  const ae_int32x2 * restrict p_x1rd;
  const ae_int32   * restrict p_twd;

  ae_int32x2 vA0, vA1, vB0, vB1, vC0, vC1, scl;
  ae_int16x4 vT;
  ae_int64 T64_0, T64_1;

  NASSERT_ALIGN8(x);

  shiftl = XT_MAX(0, -shift);
  shiftr = XT_MAX(0,  shift);
  scl = 1 << shiftl;
  WUR_AE_SAR(shiftr);

  p_twd = (const ae_int32 *)twdSplit+1;
  p_x0 = (ae_int32x2 *)(x);
  p_x1 = (ae_int32x2 *)(x+N);

  // first point
  vA0 = AE_L32X2_I(p_x0, 0);
  vA1 = AE_L32X2_I(p_x1, 0);
  vA0 = AE_MULP32X2(vA0, scl);
  vA1 = AE_MULP32X2(vA1, scl);

  AE_ADDANDSUBRNG32(vB0, vB1, vA0, vA1);
  vB0 = AE_SEL32_HH(vB0, vB1);
  vB1 = AE_MOVI(0);

  AE_S32X2_IP(vB0, p_x0, 8);
  AE_S32X2_XP(vB1, p_x1, step_back);

  p_x0rd = (const ae_int32x2 *)p_x0;
  p_x1rd = (const ae_int32x2 *)p_x1;

  {
    // load next data
    AE_L32X2_IP(vA0, p_x0rd, 8);
    AE_L32X2_XP(vA1, p_x1rd, step_back);
    vA0 = AE_MULP32X2(vA0, scl);
    vA1 = AE_MULP32X2(vA1, scl);

    // load twiddle
    AE_L32_XP(vB1, p_twd, 2*sizeof(int16_t));
    vT = AE_MOVINT16X4_FROMINT32X2(vB1);

    // ADD/SUBB
    AE_ADDANDSUBRNG32(vB0, vB1, vA0, vA1);

    vA0 = AE_SEL32_HL(vB1, vB0);
    vB1 = AE_SEL32_LH(vB1, vB0);

    // do rotation
    vB0 = AE_MULFC32X16RAS_H(vA0, vT);
  }
  __Pragma( "loop_count min=2, factor=2" );
  for(i = 2; i < (N>>2); i++) 
  {
    // SUM/DIFF
    vC0 = AE_ADDSUB32S(vB1, vB0);
    vC1 = AE_SUBADD32S(vB0, vB1);

    T64_0 = AE_MOVINT64_FROMINT32X2(vC0);
    T64_1 = AE_MOVINT64_FROMINT32X2(vC1);
    AE_S64_IP(T64_0, castxcc(ae_int64,p_x0), 8);
    AE_S64_XP(T64_1, castxcc(ae_int64,p_x1), step_back);

    // load next data
    AE_L32X2_IP(vA0, p_x0rd, 8);
    AE_L32X2_XP(vA1, p_x1rd, step_back);
    vA0 = AE_MULP32X2(vA0, scl);
    vA1 = AE_MULP32X2(vA1, scl);

    // load twiddle
    AE_L32_XP(vB1, p_twd, 2*sizeof(int16_t));
    vT = AE_MOVINT16X4_FROMINT32X2(vB1);

    // ADD/SUBB
    AE_ADDANDSUBRNG32(vB0, vB1, vA0, vA1);

    vA0 = AE_SEL32_HL(vB1, vB0);
    vB1 = AE_SEL32_LH(vB1, vB0);

    // do rotation
    vB0 = AE_MULFC32X16RAS_H(vA0, vT);
  }
  {
    // SUM/DIFF
    vC0 = AE_ADDSUB32S(vB1, vB0);
    vC1 = AE_SUBADD32S(vB0, vB1);

    T64_0 = AE_MOVINT64_FROMINT32X2(vC0);
    T64_1 = AE_MOVINT64_FROMINT32X2(vC1);
    AE_S64_IP(T64_0, castxcc(ae_int64,p_x0), 8);
    AE_S64_XP(T64_1, castxcc(ae_int64,p_x1), step_back);
  }

  // middle sample
  vA0 = AE_L32X2_I(p_x0rd, 0);
  vA0 = AE_SRAA32(vA0, shift-1);
  vB0 = AE_NEG32S(vA0);
  vC0 = AE_SEL32_HL(vA0, vB0);
  AE_S32X2_I(vC0, p_x0, 0);
}

/*-------------------------------------------------------------------------
  Inverse FFT on Real Data
  These functions make inverse FFT on half spectral data forming real
  data samples.
      Scaling: 
      +-------------------+----------------------------------------+
      |      Function     |           Scaling options              |
      +-------------------+----------------------------------------+
      |  ifft_real16x16   |  2 - 16-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  ifft_real32x32   |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  ifft_real32x16   |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      +-------------------+----------------------------------------+

  NOTES:
  1. Bit-reversing reordering is done here. 
  2. IFFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after
     the call.
  3. Inverse FFT function for real signal transforms the input spectrum  
     and then calls ifft_cplx() with FFT size set to N/2.
  4. 32x32,32x16,16x16  FFTs support mixed radix transforms
  5. N - FFT size

  Precision:
  32x32  32-bit input/outputs, 32-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles

  Input:
  x[(N/2+1)*2]	input spectrum. Real and imaginary data are interleaved  
                and real data goes first
  scalingOpt	scaling option (see table above)

  Output:			
  y[N]	        real output signal

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y           should not overlap
  x,y           aligned on a 8-bytes boundary
-------------------------------------------------------------------------*/
int ifft_real32x16( int32_t * y, int32_t * x, fft_handle_t h, int scalingOpt )
{
    int bexp, shift;
    int N, n;
    int s = 0;
    int v = 1;
    int stg_type;
    const int isplitScale = 2; // Scaling of isplitPart_x2
    int32_t *pdest = y; 
    const int16_t * twdSplit;
    const fft_real_x16_descr_t * h_real;
    const fft_cplx_x16_descr_t * h_cplx;
    const ae_int32x2 * restrict p_x0;
    const cint16ptr_t *tw_tab;
    const int *tw_step;
    const eFft_stage_type *stg_typeTbl;
    const fft_cplx32x16_stage_t*stg_Tbl;
    fft_cplx32x16_stage_t stg_fn;
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt==3 || scalingOpt==2);

    h_real = (const fft_real_x16_descr_t*)h;
    h_cplx = h_real->cfft_hdl;
    twdSplit = h_real->twd;
    tw_tab = h_cplx->twd;
    tw_step = h_cplx->tw_step;
    stg_typeTbl = (scalingOpt == 2) ? h_cplx->stages_32x16_s2 : h_cplx->stages_32x16_s3;
    stg_Tbl = (scalingOpt == 2) ? fft32x16_stages_scl2_tbl : fft32x16_stages_scl3_tbl;
    N=(h_cplx->N) << 1;

    bexp = 0;
    if (scalingOpt == 2)
    {
        int32_t i;
        ae_int32x2 acc, tmp;
        p_x0 = (ae_int32x2 *)x;
        acc = AE_ZERO32();
        __Pragma("loop_count min=4, factor=4");
        for (n = 0; n < ((N+2)>>1); n++)   
        {
            AE_L32X2_IP(tmp, p_x0, sizeof(ae_int32x2));
            tmp = AE_ABS32S(tmp); 
            acc = AE_OR32(acc, tmp);
        }
        acc = AE_OR32(acc, AE_SEL32_LH(acc, acc));
        i = AE_MOVAD32_L(acc);
        bexp = NSA(i);
        XT_MOVEQZ(bexp, 0, i);
    }
    shift = isplitScale - bexp;
    isplitPart_x2(x, twdSplit, shift, N);

    /* half-sized complex IFFT */
    N >>= 1;
    bexp = 0;
    while ( tw_tab[s] != NULL )
    {
        stg_type = (int)stg_typeTbl[s];
        NASSERT(stg_type < NUM_FFT_STAGE_TYPES);
        stg_fn = stg_Tbl[stg_type];
        NASSERT(stg_fn != NULL);
        shift += stg_fn(tw_tab[s], x, y, N, &v, tw_step[s], &bexp); 
        SWAP_PTR(x, y); 
        s++; 
    }

    if (y != pdest)
    {  
        /* Execute the last stage inplace */
        y = x;
    }

    /* Last stage */
    stg_type = (int)stg_typeTbl[s];
    NASSERT(stg_type < NUM_FFT_STAGE_TYPES);
    stg_fn = stg_Tbl[stg_type];
    NASSERT(stg_fn != NULL);
    shift += stg_fn(tw_tab[s], x, y, N, &v, tw_step[s], &bexp);

    return shift;
} /* ifft_real32x16() */
