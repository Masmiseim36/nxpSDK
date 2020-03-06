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
    Inverse FFT on Complex Data with Optimized Memory Usage
    C code optimized for HiFi4
	Integrit, 2006-2018
*/

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
#include "common_fpu.h"

/*-------------------------------------------------------------------------
  Inverse FFT on Complex Data with Optimized Memory Usage
  These functions make inverse FFT on complex data with optimized 
  memory usage.
  Scaling: 
      +-------------------+----------------------------------------+
      |      Function     |           Scaling options              |
      +-------------------+----------------------------------------+
      | ifft_cplx16x16_ie |  2 - 16-bit dynamic scaling            | 
      | ifft_cplx32x16_ie |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      | ifft_cplx32x32_ie |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      +-------------------+----------------------------------------+
  NOTES:
  1. Bit-reversing reordering is done here.
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call
  3. FFT of size N may be supplied with constant data
     (twiddle factors) of a larger-sized FFT = N*twdstep.
  4. Stereo FFTs accept inputs/form outputs in the interleaved order:
     left complex sample, right complex sample

  Precision: 
  16x16_ie      16-bit input/outputs, 16-bit twiddles
  32x16_ie      32-bit input/outputs, 16-bit twiddles
  32x32_ie      32-bit input/outputs, 32-bit twiddles
  f_ie          floating point
 
  Input:
  S                   1 for ordinary (single channel) FFT, 2 - for stereo
                      input/outputs
  x[N*S]              complex input signal. Real and imaginary data 
                      are interleaved and real data goes first

  twd[N*twdstep*3/4]  twiddle factor table of a complex-valued FFT of 
                      size N*twdstep
  N                   FFT size
  twdstep             twiddle step 
  scalingOpt          scaling option (see table above)

  Output:
  y[N*S]              output spectrum. Real and imaginary data are 
                      interleaved and real data goes first

  Returned value:     total number of right shifts occurred during 
                      scaling procedure

  Restrictions:
  x,y   should not overlap
  x,y   aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(int,stereo_ifft_cplxf_ie,(complex_float* y,complex_float* x, const complex_float* twd, int twdstep, int N))
#elif (HAVE_VFPU)
#define SZ_CF32 (sizeof(complex_float))
#define LOG2_SZ_CF32 3/* log2(sizeof(complex_float)) */

int stereo_ifft_cplxf_ie(complex_float* y,complex_float* x, const complex_float* twd, int twdstep, int N)
{
  const xtfloatx2 *restrict p_twd;
  const xtfloatx2 *restrict p0_ld;
  const xtfloatx2 *restrict p1_ld;
  const xtfloatx2 *restrict p2_ld;
  const xtfloatx2 *restrict p3_ld;
        xtfloatx2 *restrict p0_st;
        xtfloatx2 *restrict p1_st;
        xtfloatx2 *restrict p2_st;
        xtfloatx2 *restrict p3_st;
  xtfloatx2 tw1, tw2, tw3;
  xtfloatx2 a0, a1, a2, a3;
  xtfloatx2 b0, b1, b2, b3;
  xtfloatx2 recipN;
  int N4, logN, stride, temp;
  int m, n;
  unsigned int idx, bitrevstride;

  NASSERT( x );
  NASSERT( y );
  NASSERT( twd );
  NASSERT( x != y );
  NASSERT_ALIGN( x, 8 );
  NASSERT_ALIGN( y, 8 );
  NASSERT_ALIGN( twd, 8 );
  NASSERT( twdstep >= 1 );
  NASSERT( N>=8 && 0 == (N&(N-1)) );

  N4 = N>>2;
  logN = 32 - NSA( N4 );
  /* Compute 1/N as 2^(-logN) */
  temp = (127-logN) << 23;
  recipN = XT_WFR(temp);

  /*----------------------------------------------------*/
  /* Twiddle factors are conjugated to make inverse FFT */
  /*----------------------------------------------------*/

  /* Set the pointer to the twiddle table            *
   * and set bounds of the table for circular loads. */
  p_twd = (const xtfloatx2 *)(twd);
  WUR_AE_CBEGIN0((uintptr_t)(twd));
  WUR_AE_CEND0  ((uintptr_t)(twd+3*twdstep*(N4)));

  /*-------------------------------------------------------------*
   * Perform first through the next to last stages. We use DIF,  *
   * all permutations are deferred until the last stage.         */
  for (stride = N4; stride >= 2; stride >>= 2)
  {
    p0_st = (xtfloatx2 *)(x);
    
    __Pragma("loop_count min=1")
    for ( m=0; m<N4; m+=stride )
    {
      p1_st = p0_st + stride*2;
      p2_st = p1_st + stride*2;
      p3_st = p2_st + stride*2;
      p0_ld = p0_st;
      p1_ld = p1_st;
      p2_ld = p2_st;
      p3_ld = p3_st;
      /* Radix-4 butterfly */
      __Pragma("ymemory (p_twd)")
      __Pragma("loop_count min=2")
      for ( n=0; n<stride; n++ )
      {
        XT_LSX2IP(tw1, p_twd, SZ_CF32);
        XT_LSX2IP(tw2, p_twd, SZ_CF32);
        XT_LSX2XC(tw3, p_twd, (twdstep*3-2)*SZ_CF32);

        /* Left channel */
        /* load input samples */
        XT_LSX2IP(a0, p0_ld, SZ_CF32);
        XT_LSX2IP(a1, p1_ld, SZ_CF32);
        XT_LSX2IP(a2, p2_ld, SZ_CF32);
        XT_LSX2IP(a3, p3_ld, SZ_CF32);

        /* compute butterfly */
        /* 1-st substage */
        b0 = a0 + a2;
        b1 = a1 + a3;
        b2 = a0 - a2;
        b3 = a1 - a3;
        /* 2-nd substage */
        a0 = b0 + b1;
        a2 = b0 - b1;
        /* a1 <- b2-j*b3 */
        /* a3 <- b2+j*b3 */
        b3 = XT_SEL32_LH_SX2(b3, b3);
        b3 = XT_CONJC_S(b3);
        a1 = b2 - b3;
        a3 = b2 + b3;

        /* multiply by twiddle factors */
        b0 = a0;
        b1 = XT_MULCCONJ_S(a1, tw1);
        b2 = XT_MULCCONJ_S(a2, tw2);
        b3 = XT_MULCCONJ_S(a3, tw3);

        /* Two middle quartiles are swapped on all but the last stage to use the bit reversal
         * permutation instead of the digit reverse. */
        XT_SSX2IP(b0, p0_st, SZ_CF32);
        XT_SSX2IP(b2, p1_st, SZ_CF32);
        XT_SSX2IP(b1, p2_st, SZ_CF32);
        XT_SSX2IP(b3, p3_st, SZ_CF32);

        /* Right channel */
        /* load input samples */
        XT_LSX2IP(a0, p0_ld, SZ_CF32);
        XT_LSX2IP(a1, p1_ld, SZ_CF32);
        XT_LSX2IP(a2, p2_ld, SZ_CF32);
        XT_LSX2IP(a3, p3_ld, SZ_CF32);

        /* compute butterfly */
        /* 1-st substage */
        b0 = a0 + a2;
        b1 = a1 + a3;
        b2 = a0 - a2;
        b3 = a1 - a3;
        /* 2-nd substage */
        a0 = b0 + b1;
        a2 = b0 - b1;
        /* a1 <- b2-j*b3 */
        /* a3 <- b2+j*b3 */
        b3 = XT_SEL32_LH_SX2(b3, b3);
        b3 = XT_CONJC_S(b3);
        a1 = b2 - b3;
        a3 = b2 + b3;

        /* multiply by twiddle factors */
        b0 = a0;
        b1 = XT_MULCCONJ_S(a1, tw1);
        b2 = XT_MULCCONJ_S(a2, tw2);
        b3 = XT_MULCCONJ_S(a3, tw3);

        /* Two middle quartiles are swapped on all but the last stage to use the bit reversal
         * permutation instead of the digit reverse. */
        XT_SSX2IP(b0, p0_st, SZ_CF32);
        XT_SSX2IP(b2, p1_st, SZ_CF32);
        XT_SSX2IP(b1, p2_st, SZ_CF32);
        XT_SSX2IP(b3, p3_st, SZ_CF32);
      }
      p0_st = p3_st;
    }
    twdstep <<= 2;
  }
  
  /*----------------------------------------------------------------------------
   Last stage (radix-2 or radix-4 for even powers of two) with bit reversal
   permutation.*/
  idx = 0;
  bitrevstride = 0x80000000U >> (logN-2+LOG2_SZ_CF32);
  if ( stride < 1 )
  {
    /* Radix-2 butterfly */
    const int N2 = N>>1;
    bitrevstride >>= 1;
    p0_ld = (const xtfloatx2 *)(x);
    p1_ld = p0_ld+2;
    p0_st = (xtfloatx2 *)(y);
    p1_st = p0_st+N2*2;
    p2_st = p0_st+1;
    p3_st = p1_st+1;

    __Pragma("loop_count min=2, factor=2")
    for ( n=0; n<N2; n++ )
    {
      XT_LSX2IP(a0, p0_ld, SZ_CF32);
      XT_LSX2IP(a1, p1_ld, SZ_CF32);

      b0 = a0 + a1;
      b1 = a0 - a1;
      b0 = b0*recipN;
      b1 = b1*recipN;

      XT_SSX2X(b0, p0_st, idx);
      XT_SSX2X(b1, p1_st, idx);

      XT_LSX2IP(a0, p0_ld, (2*2-1)*SZ_CF32);
      XT_LSX2IP(a1, p1_ld, (2*2-1)*SZ_CF32);

      b0 = a0 + a1;
      b1 = a0 - a1;
      b0 = b0*recipN;
      b1 = b1*recipN;

      XT_SSX2X(b0, p2_st, idx);
      XT_SSX2X(b1, p3_st, idx);

      idx = AE_ADDBRBA32(idx, bitrevstride);
    }
  }
  else
  {
    /* Radix-4 butterfly */
    p0_ld = (const xtfloatx2 *)(x);
    p1_ld = p0_ld+2;
    p2_ld = p1_ld+2;
    p3_ld = p2_ld+2;
    p0_st = (xtfloatx2 *)(y);
    p1_st = p0_st+N4*2;
    p2_st = p1_st+N4*2;
    p3_st = p2_st+N4*2;

    __Pragma("loop_count min=2, factor=2")
    for ( n=0; n<N4; n++ )
    {
      XT_LSX2IP(a0, p0_ld, SZ_CF32);
      XT_LSX2IP(a1, p1_ld, SZ_CF32);
      XT_LSX2IP(a2, p2_ld, SZ_CF32);
      XT_LSX2IP(a3, p3_ld, SZ_CF32);

      b0 = a0 + a2;
      b1 = a1 + a3;
      b2 = a0 - a2;
      b3 = a1 - a3;

      /* a0 = (b0 + b1)*(1/N) */
      /* a2 = (b0 - b1)*(1/N) */
      a0 = a2 = b0*recipN;
      XT_MADDMUX_S(a0, recipN, b1, 0);
      XT_MADDMUX_S(a2, recipN, b1, 2);
      /* a1 <- (b2 + j*b3)*(1/N) */
      /* a3 <- (b2 - j*b3)*(1/N) */
      a1 = a3 = b2*recipN;
      XT_MADDMUX_S(a1, recipN, b3, 1);
      XT_MADDMUX_S(a3, recipN, b3, 3);

      XT_SSX2X(a0, p0_st, idx);
      XT_SSX2X(a1, p1_st, idx);
      XT_SSX2X(a2, p2_st, idx);
      XT_SSX2X(a3, p3_st, idx);

      XT_LSX2IP(a0, p0_ld, (4*2-1)*SZ_CF32);
      XT_LSX2IP(a1, p1_ld, (4*2-1)*SZ_CF32);
      XT_LSX2IP(a2, p2_ld, (4*2-1)*SZ_CF32);
      XT_LSX2IP(a3, p3_ld, (4*2-1)*SZ_CF32);

      b0 = a0 + a2;
      b1 = a1 + a3;
      b2 = a0 - a2;
      b3 = a1 - a3;

      /* a0 = (b0 + b1)*(1/N) */
      /* a2 = (b0 - b1)*(1/N) */
      a0 = a2 = b0*recipN;
      XT_MADDMUX_S(a0, recipN, b1, 0);
      XT_MADDMUX_S(a2, recipN, b1, 2);
      /* a1 <- (b2 + j*b3)*(1/N) */
      /* a3 <- (b2 - j*b3)*(1/N) */
      a1 = a3 = b2*recipN;
      XT_MADDMUX_S(a1, recipN, b3, 1);
      XT_MADDMUX_S(a3, recipN, b3, 3);

      XT_SSX2X(a0, p0_st, idx+SZ_CF32);
      XT_SSX2X(a1, p1_st, idx+SZ_CF32);
      XT_SSX2X(a2, p2_st, idx+SZ_CF32);
      XT_SSX2X(a3, p3_st, idx+SZ_CF32);

      idx = AE_ADDBRBA32(idx, bitrevstride);
    }
  }
  return 0;
} /* stereo_fft_cplxf_ie() */
#else
#define SZ_F32 (sizeof(float32_t))
#define LOG2_SZ_CF32 3/* log2(sizeof(complex_float)) */

int stereo_ifft_cplxf_ie(complex_float* y,complex_float* x, const complex_float* twd, int twdstep, int N)
{
  const xtfloat *restrict p_twd0;
  const xtfloat *restrict p_twd1;
  const xtfloat *restrict p_swp;
  const xtfloat *restrict p0_ld;
  const xtfloat *restrict p1_ld;
  const xtfloat *restrict p2_ld;
  const xtfloat *restrict p3_ld;
        xtfloat *restrict p0_st;
        xtfloat *restrict p1_st;
        xtfloat *restrict p2_st;
        xtfloat *restrict p3_st;
  xtfloat tw1_re, tw2_re, tw3_re;
  xtfloat tw1_im, tw2_im, tw3_im;
  xtfloat a0_re, a1_re, a2_re, a3_re;
  xtfloat a0_im, a1_im, a2_im, a3_im;
  xtfloat b0_re, b1_re, b2_re, b3_re;
  xtfloat b0_im, b1_im, b2_im, b3_im;
  xtfloat recipN;
  int N4, logN, stride, temp;
  int m, n;
  unsigned int idx, bitrevstride;

  NASSERT( x );
  NASSERT( y );
  NASSERT( twd );
  NASSERT( x != y );
  NASSERT_ALIGN( x, 8 );
  NASSERT_ALIGN( y, 8 );
  NASSERT_ALIGN( twd, 8 );
  NASSERT( twdstep >= 1 );
  NASSERT( N>=8 && 0 == (N&(N-1)) );

  N4 = N>>2;
  logN = 32 - NSA( N4 );
  /* Compute 1/N as 2^(-logN) */
  temp = (127-logN) << 23;
  recipN = XT_WFR(temp);

  /*----------------------------------------------------*/
  /* Twiddle factors are conjugated to make inverse FFT */
  /*----------------------------------------------------*/

  /*------------------------------------------------------------*
   * Perform first through the next to last stages. We use DIF, *
   * all permutations are deferred until the last stage.        */
  for (stride = N4; stride >= 2; stride >>= 2)
  {
    p0_st = (xtfloat *)(x);
    
    __Pragma("loop_count min=1")
    for ( m=0; m<N4; m+=stride )
    {
      p_twd0 = (const xtfloat *)(twd);
      p_twd1 = (const xtfloat *)(twd);

      p1_st = p0_st + stride*2*2;
      p2_st = p1_st + stride*2*2;
      p3_st = p2_st + stride*2*2;
      p0_ld = p0_st;
      p1_ld = p1_st;
      p2_ld = p2_st;
      p3_ld = p3_st;
      /* Radix-4 butterfly */
      __Pragma("loop_count min=2");
      for ( n=0; n<stride*2; n++ )
      {
        XT_LSIP(tw1_re, p_twd0, SZ_F32); XT_LSIP(tw1_im, p_twd0, SZ_F32);
        XT_LSIP(tw2_re, p_twd0, SZ_F32); XT_LSIP(tw2_im, p_twd0, SZ_F32);
        XT_LSIP(tw3_re, p_twd0, SZ_F32); XT_LSXP(tw3_im, p_twd0, (twdstep*6-5)*SZ_F32);
        p_swp = p_twd0; p_twd0 = p_twd1; p_twd1 = p_swp;/* use 2 overlapping pointers for different channels */

        /* load input samples */
        XT_LSIP(a0_re, p0_ld, SZ_F32); XT_LSIP(a0_im, p0_ld, SZ_F32);
        XT_LSIP(a1_re, p1_ld, SZ_F32); XT_LSIP(a1_im, p1_ld, SZ_F32);
        XT_LSIP(a2_re, p2_ld, SZ_F32); XT_LSIP(a2_im, p2_ld, SZ_F32);
        XT_LSIP(a3_re, p3_ld, SZ_F32); XT_LSIP(a3_im, p3_ld, SZ_F32);

        /* compute butterfly */
        /* 1-st substage */
        b0_re = XT_ADD_S(a0_re, a2_re); b0_im = XT_ADD_S(a0_im, a2_im);
        b1_re = XT_ADD_S(a1_re, a3_re); b1_im = XT_ADD_S(a1_im, a3_im);
        b2_re = XT_SUB_S(a0_re, a2_re); b2_im = XT_SUB_S(a0_im, a2_im);
        b3_re = XT_SUB_S(a1_re, a3_re); b3_im = XT_SUB_S(a1_im, a3_im);
        /* 2-nd substage */
        a0_re = XT_ADD_S(b0_re, b1_re); a0_im = XT_ADD_S(b0_im, b1_im);
        a2_re = XT_SUB_S(b0_re, b1_re); a2_im = XT_SUB_S(b0_im, b1_im);
        /* a1 <- b2+j*b3 */
        /* a3 <- b2-j*b3 */
        a1_re = XT_SUB_S(b2_re, b3_im); a1_im = XT_ADD_S(b2_im, b3_re);
        a3_re = XT_ADD_S(b2_re, b3_im); a3_im = XT_SUB_S(b2_im, b3_re);

        /* multiply by twiddle factors */
        b0_re = a0_re;
        b0_im = a0_im;
        b1_re = XT_MUL_S(a1_re, tw1_re); XT_MADD_S(b1_re, a1_im, tw1_im);
        b1_im = XT_MUL_S(a1_im, tw1_re); XT_MSUB_S(b1_im, a1_re, tw1_im);
        b2_re = XT_MUL_S(a2_re, tw2_re); XT_MADD_S(b2_re, a2_im, tw2_im);
        b2_im = XT_MUL_S(a2_im, tw2_re); XT_MSUB_S(b2_im, a2_re, tw2_im);
        b3_re = XT_MUL_S(a3_re, tw3_re); XT_MADD_S(b3_re, a3_im, tw3_im);
        b3_im = XT_MUL_S(a3_im, tw3_re); XT_MSUB_S(b3_im, a3_re, tw3_im);

        /* Two middle quartiles are swapped on all but the last stage to use the bit reversal
         * permutation instead of the digit reverse. */
        XT_SSIP(b0_re, p0_st, SZ_F32); XT_SSIP(b0_im, p0_st, SZ_F32);
        XT_SSIP(b2_re, p1_st, SZ_F32); XT_SSIP(b2_im, p1_st, SZ_F32);
        XT_SSIP(b1_re, p2_st, SZ_F32); XT_SSIP(b1_im, p2_st, SZ_F32);
        XT_SSIP(b3_re, p3_st, SZ_F32); XT_SSIP(b3_im, p3_st, SZ_F32);
      }
      p0_st = p3_st;
    }
    twdstep <<= 2;
  }
  
  /*----------------------------------------------------------------------------
   Last stage (radix-2 or radix-4 for even powers of two) with bit reversal
   permutation.*/
  idx = 0;
  bitrevstride = 0x80000000U >> (logN-2+LOG2_SZ_CF32);
  if ( stride < 1 )
  {
    /* Radix-2 butterfly */
    const int N2 = N4<<1;
    bitrevstride >>= 1;
    p0_ld = (const xtfloat *)(x);
    p1_ld = p0_ld+2*2;
    p0_st = (xtfloat *)(y);
    p1_st = p0_st+N2*2*2;
    p2_st = p0_st+1*2;
    p3_st = p1_st+1*2;

    __Pragma("loop_count min=2, factor=2")
    for ( n=0; n<N2; n++ )
    {
      /* Left channel */
      XT_LSIP(a0_re, p0_ld, SZ_F32); XT_LSIP(a0_im, p0_ld, SZ_F32);
      XT_LSIP(a1_re, p1_ld, SZ_F32); XT_LSIP(a1_im, p1_ld, SZ_F32);

      b0_re = XT_ADD_S(a0_re, a1_re); b0_im = XT_ADD_S(a0_im, a1_im);
      b1_re = XT_SUB_S(a0_re, a1_re); b1_im = XT_SUB_S(a0_im, a1_im);
      b0_re = XT_MUL_S(b0_re, recipN); b0_im = XT_MUL_S(b0_im, recipN);
      b1_re = XT_MUL_S(b1_re, recipN); b1_im = XT_MUL_S(b1_im, recipN);

      XT_SSX(b0_re, p0_st, idx); XT_SSX(b0_im, p0_st, idx+SZ_F32);
      XT_SSX(b1_re, p1_st, idx); XT_SSX(b1_im, p1_st, idx+SZ_F32);

      /* Right channel */
      XT_LSIP(a0_re, p0_ld, SZ_F32); XT_LSIP(a0_im, p0_ld, (2*2*2-3)*SZ_F32);
      XT_LSIP(a1_re, p1_ld, SZ_F32); XT_LSIP(a1_im, p1_ld, (2*2*2-3)*SZ_F32);

      b0_re = XT_ADD_S(a0_re, a1_re); b0_im = XT_ADD_S(a0_im, a1_im);
      b1_re = XT_SUB_S(a0_re, a1_re); b1_im = XT_SUB_S(a0_im, a1_im);
      b0_re = XT_MUL_S(b0_re, recipN); b0_im = XT_MUL_S(b0_im, recipN);
      b1_re = XT_MUL_S(b1_re, recipN); b1_im = XT_MUL_S(b1_im, recipN);

      XT_SSX(b0_re, p2_st, idx); XT_SSX(b0_im, p2_st, idx+SZ_F32);
      XT_SSX(b1_re, p3_st, idx); XT_SSX(b1_im, p3_st, idx+SZ_F32);

      idx = AE_ADDBRBA32(idx, bitrevstride);
    }
  }
  else
  {
    /* Radix-4 butterfly */
    p0_ld = (const xtfloat *)(x);
    p1_ld = p0_ld+2*2;
    p2_ld = p1_ld+2*2;
    p3_ld = p2_ld+2*2;
    p0_st = (xtfloat *)(y);
    p1_st = p0_st+N4*2*2;
    p2_st = p1_st+N4*2*2;
    p3_st = p2_st+N4*2*2;

    __Pragma("loop_count min=2, factor=2")
    for ( n=0; n<N4; n++ )
    {
      /* Left channel */
      XT_LSIP(a0_re, p0_ld, SZ_F32); XT_LSIP(a0_im, p0_ld, SZ_F32);
      XT_LSIP(a1_re, p1_ld, SZ_F32); XT_LSIP(a1_im, p1_ld, SZ_F32);
      XT_LSIP(a2_re, p2_ld, SZ_F32); XT_LSIP(a2_im, p2_ld, SZ_F32);
      XT_LSIP(a3_re, p3_ld, SZ_F32); XT_LSIP(a3_im, p3_ld, SZ_F32);

      /* 1-st substage */
      b0_re = XT_ADD_S(a0_re, a2_re); b0_im = XT_ADD_S(a0_im, a2_im);
      b1_re = XT_ADD_S(a1_re, a3_re); b1_im = XT_ADD_S(a1_im, a3_im);
      b2_re = XT_SUB_S(a0_re, a2_re); b2_im = XT_SUB_S(a0_im, a2_im);
      b3_re = XT_SUB_S(a1_re, a3_re); b3_im = XT_SUB_S(a1_im, a3_im);
      /* 2-nd substage */
      a0_re = XT_ADD_S(b0_re, b1_re); a0_im = XT_ADD_S(b0_im, b1_im);
      a2_re = XT_SUB_S(b0_re, b1_re); a2_im = XT_SUB_S(b0_im, b1_im);
      /* a1 <- b2+j*b3 */
      /* a3 <- b2-j*b3 */
      a1_re = XT_SUB_S(b2_re, b3_im); a1_im = XT_ADD_S(b2_im, b3_re);
      a3_re = XT_ADD_S(b2_re, b3_im); a3_im = XT_SUB_S(b2_im, b3_re);

      a0_re = XT_MUL_S(a0_re, recipN); a0_im = XT_MUL_S(a0_im, recipN);
      a1_re = XT_MUL_S(a1_re, recipN); a1_im = XT_MUL_S(a1_im, recipN);
      a2_re = XT_MUL_S(a2_re, recipN); a2_im = XT_MUL_S(a2_im, recipN);
      a3_re = XT_MUL_S(a3_re, recipN); a3_im = XT_MUL_S(a3_im, recipN);
      XT_SSX(a0_re, p0_st, idx); XT_SSX(a0_im, p0_st, idx+SZ_F32);
      XT_SSX(a1_re, p1_st, idx); XT_SSX(a1_im, p1_st, idx+SZ_F32);
      XT_SSX(a2_re, p2_st, idx); XT_SSX(a2_im, p2_st, idx+SZ_F32);
      XT_SSX(a3_re, p3_st, idx); XT_SSX(a3_im, p3_st, idx+SZ_F32);

      /* Right channel */
      XT_LSIP(a0_re, p0_ld, SZ_F32); XT_LSIP(a0_im, p0_ld, (4*2*2-3)*SZ_F32);
      XT_LSIP(a1_re, p1_ld, SZ_F32); XT_LSIP(a1_im, p1_ld, (4*2*2-3)*SZ_F32);
      XT_LSIP(a2_re, p2_ld, SZ_F32); XT_LSIP(a2_im, p2_ld, (4*2*2-3)*SZ_F32);
      XT_LSIP(a3_re, p3_ld, SZ_F32); XT_LSIP(a3_im, p3_ld, (4*2*2-3)*SZ_F32);

      /* 1-st substage */
      b0_re = XT_ADD_S(a0_re, a2_re); b0_im = XT_ADD_S(a0_im, a2_im);
      b1_re = XT_ADD_S(a1_re, a3_re); b1_im = XT_ADD_S(a1_im, a3_im);
      b2_re = XT_SUB_S(a0_re, a2_re); b2_im = XT_SUB_S(a0_im, a2_im);
      b3_re = XT_SUB_S(a1_re, a3_re); b3_im = XT_SUB_S(a1_im, a3_im);
      /* 2-nd substage */
      a0_re = XT_ADD_S(b0_re, b1_re); a0_im = XT_ADD_S(b0_im, b1_im);
      a2_re = XT_SUB_S(b0_re, b1_re); a2_im = XT_SUB_S(b0_im, b1_im);
      /* a1 <- b2+j*b3 */
      /* a3 <- b2-j*b3 */
      a1_re = XT_SUB_S(b2_re, b3_im); a1_im = XT_ADD_S(b2_im, b3_re);
      a3_re = XT_ADD_S(b2_re, b3_im); a3_im = XT_SUB_S(b2_im, b3_re);

      a0_re = XT_MUL_S(a0_re, recipN); a0_im = XT_MUL_S(a0_im, recipN);
      a1_re = XT_MUL_S(a1_re, recipN); a1_im = XT_MUL_S(a1_im, recipN);
      a2_re = XT_MUL_S(a2_re, recipN); a2_im = XT_MUL_S(a2_im, recipN);
      a3_re = XT_MUL_S(a3_re, recipN); a3_im = XT_MUL_S(a3_im, recipN);
      XT_SSX(a0_re, p0_st, idx+2*SZ_F32); XT_SSX(a0_im, p0_st, idx+3*SZ_F32);
      XT_SSX(a1_re, p1_st, idx+2*SZ_F32); XT_SSX(a1_im, p1_st, idx+3*SZ_F32);
      XT_SSX(a2_re, p2_st, idx+2*SZ_F32); XT_SSX(a2_im, p2_st, idx+3*SZ_F32);
      XT_SSX(a3_re, p3_st, idx+2*SZ_F32); XT_SSX(a3_im, p3_st, idx+3*SZ_F32);

      idx = AE_ADDBRBA32(idx, bitrevstride);
    }
  }
  return 0;
} /* stereo_fft_cplxf_ie() */
#endif
