/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
	NatureDSP Signal Processing Library. FFT part
    C code optimized for Fusion
*/

#include "NatureDSP_Signal.h"
#include "fft_real_twiddles.h"
#include "fft_cplx_twiddles.h"
#include "baseop.h"
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)

inline_ void MULCx2(ae_int16x4 *x, const ae_int16x4 *t)
{
    ae_f16x4 f;
    f = AE_MULFC16RAS_L(AE_MOVF16X4_FROMINT16X4(*x), AE_MOVF16X4_FROMINT16X4(*t));
    AE_MULFC16RAS_H(f, AE_MOVF16X4_FROMINT16X4(*x), AE_MOVF16X4_FROMINT16X4(*t));
    *x = AE_MOVINT16X4_FROMF16X4(f);
}
/*
	in-place inverse split part of FFT:
	x[N+2]  input (N+2 samples)/output(N samples)
	N       size of FFT
*/
static void isplitPart_x2(int16_t *X,int N)
{
  int i, step;
  int16_t *x = X;
  ae_p16x2s * restrict p_x0, * restrict p_x1;

  ae_int32x2  vA0, vA1, vB0, vB1, vC0, vR;


  NASSERT_ALIGN8(X);

  // setup table step
  {
    ae_q56s tmp = AE_CVTQ48A32S(N);
    step = 1<<(AE_NSAQ56S(tmp)-(38-MAX_RFFT_PWR));
  }
  step <<= 2;

  p_x0 = (ae_p16x2s *)X;
  p_x1 = (ae_p16x2s *)(X+N);

  vR = AE_MOVI(1);
  vR = AE_SLAI32(vR, 8);

  // first point
  vA0 = AE_L16X2M_I(p_x0, 0);
  vA1 = AE_L16X2M_I(p_x1, 0);
  vB0 = AE_ADD32S(vA0, vA1);
  vB1 = AE_SUB32S(vA0, vA1);
  vB0 = AE_SEL32_HH(vB0, vB1);
  vA1 = AE_MOVI(1);
  vA1 = AE_SLAI32(vA1, 8);
  vB1 = AE_MOVI(0);
  vB0 = AE_ADD32(vB0, vA1);
  vB0 = AE_SRAI32(vB0, 1);
  AE_S16X2M_I(vB0, p_x0, 0);
  AE_S16X2M_I(vB1, p_x1, 0);

  AE_L16X2M_IU(vA0, p_x0, 4);
  AE_L16X2M_IU(vA1, p_x1, -4);
#if 0
    int16_t *ptw = (int16_t*)twiddleSplit; 
  // Reference code
    for(i = 1; i < (N>>2); i++) 
    {
        int16_t x0_re = x[2 * i];
        int16_t x0_im = x[2 * i + 1];
        int16_t x1_re = x[N - 2 * i];
        int16_t x1_im = x[N - 2 * i + 1];
        int16_t s_re, s_im, d_re, d_im;
        int16_t t0_re, t0_im, t1_re, t1_im;
        int16_t c_re, c_im;

        s_re = S_add_ss(x0_re, x1_re);
        s_im = S_add_ss(x0_im, x1_im);
        d_re = S_sub_ss(x0_re, x1_re);
        d_im = S_sub_ss(x0_im, x1_im);

        t0_re = s_re; //vB1
        t0_im = d_im;
        t1_re = s_im;
        t1_im = d_re;


        // do rotation
        c_re = cmulre(t1_re, t1_im, ptw[i*step / 2], ptw[i*step / 2 + 1]);
        c_im = cmulim(t1_re, t1_im, ptw[i*step / 2], ptw[i*step / 2 + 1]);

        t1_re = S_neg_s(c_im);
        t1_im = S_neg_s(c_re);

        t0_re >>= 1;
        t0_im >>= 1;
        t1_re >>= 1;
        t1_im >>= 1;

        s_re = S_add_ss(t0_re, t1_re); //vC0
        s_im = S_add_ss(t0_im, t1_im);
        d_re = S_sub_ss(t0_re, t1_re);
        d_im = S_sub_ss(t0_im, t1_im);

        t1_re = d_re;                  //vC1     
        t1_im = S_neg_s(d_im); 

        x[2 * i] = s_re  ;
        x[2 * i + 1] = s_im  ;
        x[N - 2 * i] = t1_re  ;
        x[N - 2 * i + 1] = t1_im ;
    }
#else
  ae_int16x4 * restrict px0 = (ae_int16x4 *)&x[2];
  ae_int16x4 * restrict px1 = (ae_int16x4 *)&x[N - 2 * (1 + 1)];
  ae_int16x4 * restrict py0 = (ae_int16x4 *)&x[2];
  ae_int16x4 * restrict py1 = (ae_int16x4 *)&x[N - 2 * (1 + 1)];
  ae_valign v0 = AE_LA64_PP(px0);
  ae_valign vy0 = AE_ZALIGN64();
  xtbool4 mov_even = (int)0x5;
  ae_int32x2 * restrict pw = (ae_int32x2*)(step + (uintptr_t)twiddleSplit);

  i = (N - 2) >> 3; 
  /* 16 cycles per pipeline stage in steady state with unroll=1 */
  do//  for (i = 1; i < (N >> 2)-1; i+=2)
  {
        ae_int16x4 x0, x1, s, d, t0, t1, tw;
        ae_int32x2 tmp0, tmp1;
        AE_LA16X4_IP(x0, v0, px0);
        AE_L16X4_XP(x1, px1, -(int)sizeof(*px1));
        x1 = AE_SEL16_5432(x1, x1);
        s = AE_ADD16S(x0, x1);
        d = AE_SUB16S(x0, x1);
        
        t0 = s;
        AE_MOVT16X4(t0, d, mov_even);
        t1 = d;
        AE_MOVT16X4(t1, s, mov_even);
        /* Swap re <-> im */
        t1 = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4(AE_SHORTSWAP(t1))));

        /* do rotation*/ 
        AE_L32_XP(tmp0, castxcc(const ae_int32,pw), step);
        AE_L32_XP(tmp1, castxcc(const ae_int32,pw), step);

        tw = AE_SEL16_5432(
            AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(tmp0)), AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(tmp1)));
        MULCx2(&t1, &tw);
        t1 = AE_NEG16S(t1); 
        /* Swap re <-> im */
        t1 = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4(AE_SHORTSWAP(t1))));

        t0 = AE_SRAI16(t0, 1);
        t1 = AE_SRAI16(t1, 1);
        s = AE_ADD16S(t0, t1);
        d = AE_SUB16S(t0, t1);

        d = AE_MUL16JS(d);
        d = AE_SHORTSWAP(d);

        AE_SA16X4_IP(s, vy0, py0);
        AE_S16X4_XP(d, py1, -8);
  } while (--i); 

  AE_SA64POS_FP(vy0, py0);

  {

      ae_int16x4 x0, x1, s, d, t0, t1, tw;
      ae_int32x2 tmp0, tmp1;
      AE_LA16X4_IP(x0, v0, px0);
      AE_L16X4_XP(x1, px1, -(int)sizeof(*px1));
      x1 = AE_SEL16_5432(x1, x1);
      s = AE_ADD16S(x0, x1);
      d = AE_SUB16S(x0, x1);

      t0 = s;
      AE_MOVT16X4(t0, d, mov_even);
      t1 = d;
      AE_MOVT16X4(t1, s, mov_even);
      /* Swap re <-> im */
      t1 = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4(AE_SHORTSWAP(t1))));

      /* do rotation*/
      AE_L32_XP(tmp0, castxcc(const ae_int32,pw), step);
      AE_L32_XP(tmp1, castxcc(const ae_int32,pw), step);

      tw = AE_SEL16_5432(
          AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(tmp0)), AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(tmp1)));
      MULCx2(&t1, &tw);
      t1 = AE_NEG16S(t1);
      /* Swap re <-> im */
      t1 = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4(AE_SHORTSWAP(t1))));

      t0 = AE_SRAI16(t0, 1);
      t1 = AE_SRAI16(t1, 1);
      s = AE_ADD16S(t0, t1);
      d = AE_SUB16S(t0, t1);

      d = AE_MUL16JS(d);
      d = AE_SHORTSWAP(d);

      ((int16_t*)py0)[0]/*x[N/2-2]*/ = AE_MOVAD16_3(s);
      ((int16_t*)py0)[1]/*x[N / 2 - 1]*/ = AE_MOVAD16_2(s);

      ((int16_t*)py1)[2]/*x[N/2+2]*/ = AE_MOVAD16_1(d);
      ((int16_t*)py1)[3]/*x[N / 2 + 3]*/ = AE_MOVAD16_0(d);

  }
#endif
  // middle sample
  vA0 = AE_L16X2M_I((ae_p16x2s *)(x + N / 2), 0); 
  vB0 = AE_NEG32S(vA0);
  vC0 = AE_SEL32_HL(vA0, vB0);
  AE_S16X2M_I(vC0, (ae_p16x2s *)(x + N / 2), 0);
}
#else
/*
	in-place inverse split part of FFT:
	x[N+2]  input (N+2 samples)/output(N samples)
	N       size of FFT
*/
static void isplitPart_x2(int16_t *X,int N)
{
  int i, step;

  ae_p16x2s * restrict p_x0, * restrict p_x1;
  const ae_p16x2s * restrict p_twd;

  ae_int32x2  vA0, vA1, vB0, vB1, vC0, vC1, vR;
  ae_int16x4  vT;
  ae_f32x2    vF0, vF1;
  ae_f16x4    vF2;

  NASSERT_ALIGN8(X);

  // setup table step
  {
    ae_q56s tmp = AE_CVTQ48A32S(N);
    step = 1<<(AE_NSAQ56S(tmp)-(38-MAX_RFFT_PWR));
  }
  step <<= 2;

  p_twd = (const ae_p16x2s *)twiddleSplit;
  p_x0 = (ae_p16x2s *)X;
  p_x1 = (ae_p16x2s *)(X+N);

  vR = AE_MOVI(1);
  vR = AE_SLAI32(vR, 8);

  // first point
  vA0 = AE_L16X2M_I(p_x0, 0);
  vA1 = AE_L16X2M_I(p_x1, 0);
  vB0 = AE_ADD32S(vA0, vA1);
  vB1 = AE_SUB32S(vA0, vA1);
  vB0 = AE_SEL32_HH(vB0, vB1);
  vA1 = AE_MOVI(1);
  vA1 = AE_SLAI32(vA1, 8);
  vB1 = AE_MOVI(0);
  vB0 = AE_ADD32(vB0, vA1);
  vB0 = AE_SRAI32(vB0, 1);
  AE_S16X2M_I(vB0, p_x0, 0);
  AE_S16X2M_I(vB1, p_x1, 0);

  AE_L16X2M_IU(vA0, p_x0, 4);
  AE_L16X2M_IU(vA1, p_x1, -4);

  for(i = 1; i < (N>>2); i++) 
  {
    // load twiddle
    AE_L16X2M_XU(vB1, p_twd, step);
    vB1 = AE_SRAI32(vB1, 8);
    vT = AE_CVT16X4(vB1, vB1);

    // ADD/SUBB
    vB0 = AE_ADD32S(vA0, vA1);
    vB1 = AE_SUB32S(vA0, vA1);

    vA0 = AE_SEL32_LH(vB0, vB1);
    vB1 = AE_SEL32_HL(vB0, vB1);

    // do rotation
    vF1 = (vA0);
    vF2 = (vT);
    vF0 = AE_MULFC32X16RAS_H(vF1, vF2);
    vB0 = (vF0);
    vA1 = AE_NEG32S(vB0);
    vB0 = AE_SEL32_LH(vA1, vA1);

    // load next data
    AE_L16X2M_IU(vA0, p_x0, 4);
    AE_L16X2M_IU(vA1, p_x1, -4);

    // SUM/DIFF
    vC0 = AE_ADD32S(vB1, vB0);
    vC1 = AE_SUB32S(vB1, vB0);
    vB1 = AE_NEG32S(vC1);
    vC1 = AE_SEL32_HL(vC1, vB1);

    vC0 = AE_ADD32S(vC0, vR);
    vC1 = AE_ADD32S(vC1, vR);
    vC0 = AE_SRAI32(vC0, 1);
    vC1 = AE_SRAI32(vC1, 1);

    AE_S16X2M_I(vC0, p_x0, -4);
    AE_S16X2M_I(vC1, p_x1, 4);
  }

  // middle sample
  vB0 = AE_NEG32S(vA0);
  vC0 = AE_SEL32_HL(vA0, vB0);
  AE_S16X2M_I(vC0, p_x0, 0);
}
#endif
/*-------------------------------------------------------------------------
  Inverse FFT forming real data

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles
  
  NOTES:
  1. Bit-reversing reordering is done here. 
  2. IFFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after call.
  3. Inverse FFT function for real signal transforms the input spectrum and 
     then calls ifft_cplx() with FFT size set to N/2.

  Input:
  x[(N/2+1)*2]	input spectrum. Real and imaginary data are interleaved and 
                real data goes first
   N            FFT size
   scalingOpt   scaling option:
                0 - no scaling
                1 - 24-bit scaling
                2 - 32-bit scaling on the first stage and 24-bit scaling later
                3 - fixed scaling
   Output:
   y[N]         real output signal

  Restrictions:
  Arrays should not overlap
  x,y - aligned on a 8-bytes boundary
  N   - 2^m: 32...8192

-------------------------------------------------------------------------*/
int ifft_real16x16( int16_t* y,int16_t* x,fft_handle_t h,int scalingOpt)
{
  int scale,N;

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(scalingOpt==3);
  N=(((const tFftDescr*)h)->N)<<1;

  isplitPart_x2(x, N);

  scale =  ifft_cplx16x16(y,x,h,3);

  return scale;
}
