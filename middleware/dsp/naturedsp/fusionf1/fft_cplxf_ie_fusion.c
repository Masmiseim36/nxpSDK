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
 * C code optimized for Fusion
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(int, fft_cplxf_ie, ( complex_float * y, complex_float * x, const complex_float* twd, int twdstep, int N))
#else
/*-------------------------------------------------------------------------
  These functions make FFT on complex data with optimized memory usage.
  Scaling  : Fixed data scaling at each stage
  NOTES:
  1. Bit-reversing reordering is done here.
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call
  3. Forward/inverse FFT of size N may be supplied with constant data
     (twiddle factors) of a larger-sized FFT = N*twdstep.

  Precision: 
  24x24_ie      24-bit input/outputs, 24-bit twiddles
  32x16_ie      32-bit input/outputs, 16-bit twiddles
  f             floating point
 
  Input:
  x[N]                  complex input signal. Real and imaginary data are interleaved 
                        and real data goes first
  twd[N*twdstep*3/4]    twiddle factor table of a complex-valued FFT of size N*twdstep
  N                     FFT size
  twdstep               twiddle step 
  scalingOpt            scaling option = 3 (fixed scaling)
  Output:
  y[N]                  output spectrum. Real and imaginary data are interleaved and 
                        real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y - should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 256, 512, 1024 for fixed-point routines, power of 2 and >=8 for 
        floating point

-------------------------------------------------------------------------*/

#include "NatureDSP_Signal.h"
#define SZ_CF32 (sizeof(complex_float))
#define LOG2_SZ_CF32 3/* log2(sizeof(complex_float)) */

int fft_cplxf_ie    (complex_float * y, complex_float * x, const complex_float* twd, int twdstep, int N )
{
  const xtfloat *restrict p_twd;
  const xtfloatx2 *restrict p0_ld;
  const xtfloatx2 *restrict p1_ld;
  const xtfloatx2 *restrict p2_ld;
  const xtfloatx2 *restrict p3_ld;
        xtfloat *restrict p0_st;
        xtfloat *restrict p1_st;
        xtfloat *restrict p2_st;
        xtfloat *restrict p3_st;
  int N4, logN, stride;
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
  logN = 32 - XT_NSA( N4 );

  /* Set the pointer to the twiddle table            *
   * and set bounds of the table for circular loads. */
  p_twd = (const xtfloat *)(twd);
  WUR_AE_CBEGIN0((uintptr_t)(twd));
  WUR_AE_CEND0  ((uintptr_t)(twd+3*twdstep*(N4)));
  /*----------------------------------------------------------------------------*
   * Perform the first stage. We use DIF, all permutations are deferred until   *
   * the last stage.                                                            */
  {
    stride = N4;
    
    p0_st = (xtfloat *)(x);
    p1_st = (xtfloat *)XT_ADDX8(stride, (uintptr_t)p0_st);
    p2_st = (xtfloat *)XT_ADDX8(stride, (uintptr_t)p1_st);
    p3_st = (xtfloat *)XT_ADDX8(stride, (uintptr_t)p2_st);
    p0_ld = (const xtfloatx2 *)p0_st;
    p1_ld = (const xtfloatx2 *)p1_st;
    p2_ld = (const xtfloatx2 *)p2_st;
    p3_ld = (const xtfloatx2 *)p3_st;
    __Pragma("loop_count min=2")
    __Pragma("concurrent")
    for ( n=0; n<stride; n++ )
    {
      xtfloatx2 a0, a1, a2, a3;
      xtfloat a0h,a0l,a1h,a1l,a2h,a2l,a3h,a3l;
      xtfloat b0h,b0l,b1h,b1l,b2h,b2l,b3h,b3l;
      xtfloat t0h, t0l, t1h, t1l, t2h, t2l;

      XT_LSX2IP(a0, p0_ld, SZ_CF32);
      XT_LSX2IP(a1, p1_ld, SZ_CF32);
      XT_LSX2IP(a2, p2_ld, SZ_CF32);
      XT_LSX2IP(a3, p3_ld, SZ_CF32);

      b0h=XT_ADD_LHH_S(a0 , a2); b0l=XT_ADD_S(a0 , a2);
      b1h=XT_ADD_LHH_S(a1 , a3); b1l=XT_ADD_S(a1 , a3);
      b2h=XT_SUB_LHH_S(a0 , a2); b2l=XT_SUB_S(a0 , a2);
      b3h=XT_SUB_LHH_S(a1 , a3); b3l=XT_SUB_S(a1 , a3);


      a0h=XT_ADD_S(b0h , b1h); a0l=XT_ADD_S(b0l , b1l);
      a2h=XT_SUB_S(b0h , b1h); a2l=XT_SUB_S(b0l , b1l);
      /* a1 <- b2-j*b3 */
      /* a3 <- b2+j*b3 */
      a1h=XT_ADD_S(b2h , b3l); a1l=XT_SUB_S(b2l , b3h);
      a3h=XT_SUB_S(b2h , b3l); a3l=XT_ADD_S(b2l , b3h);
      XT_SSIP(a0h, p0_st, sizeof(float32_t));
      XT_SSIP(a0l, p0_st, sizeof(float32_t));

      t0h = XT_LSI(p_twd, SZ_CF32);
      t0l = XT_LSI(p_twd, SZ_CF32 + sizeof(float32_t));
      t1l = XT_LSI(p_twd, 1 * 4);
      t2h = XT_LSI(p_twd, 2 * SZ_CF32);
      t2l = XT_LSI(p_twd, 2 * SZ_CF32+4);
      XT_LSXC(t1h, p_twd, (twdstep * 3)*SZ_CF32);
     

      b2h = XT_MUL_S(a2h, t0h);
      b2l = XT_MUL_S(a2h, t0l);
      b1h = XT_MUL_S(a1h, t1h);
      b1l = XT_MUL_S(a1h, t1l);
      b3h = XT_MUL_S(a3h, t2h);
      b3l = XT_MUL_S(a3h, t2l);

      XT_MSUB_S(b2h, a2l, t0l);
      XT_MADD_S(b2l, a2l, t0h);
      XT_MSUB_S(b1h, a1l, t1l);
      XT_MADD_S(b1l, a1l, t1h);
      XT_MSUB_S(b3h, a3l, t2l);
      XT_MADD_S(b3l, a3l, t2h);
      XT_SSI(b2l, p1_st, sizeof(float32_t));
      XT_SSIP(b2h, p1_st, 2*sizeof(float32_t));
      XT_SSI(b1l, p2_st, sizeof(float32_t));
      XT_SSIP(b1h, p2_st, 2*sizeof(float32_t));
      XT_SSI(b3l, p3_st, sizeof(float32_t));
      XT_SSIP(b3h, p3_st, 2*sizeof(float32_t));
    }
  }
  /*----------------------------------------------------------------------------
   Perform second through the next to last stages.*/

  for ( stride>>=2; stride>1; stride>>=2 )
  {
    twdstep <<= 2;

    p0_st = (xtfloat *)(x);

    for ( m=0; m<N4; m+=stride )
    {
      p1_st = (xtfloat *)XT_ADDX8(stride, (uintptr_t)p0_st);
      p2_st = (xtfloat *)XT_ADDX8(stride, (uintptr_t)p1_st);
      p3_st = (xtfloat *)XT_ADDX8(stride, (uintptr_t)p2_st);
      p0_ld = (const xtfloatx2 *)p0_st;
      p1_ld = (const xtfloatx2 *)p1_st;
      p2_ld = (const xtfloatx2 *)p2_st;
      p3_ld = (const xtfloatx2 *)p3_st;
      
      __Pragma("loop_count min=2")
      __Pragma("concurrent")
      for ( n=0; n<stride; n++ )
      {
        xtfloatx2 a0, a1, a2, a3;
        xtfloat a0h,a0l,a1h,a1l,a2h,a2l,a3h,a3l;
        xtfloat b0h,b0l,b1h,b1l,b2h,b2l,b3h,b3l;
        xtfloat t0h, t0l, t1h, t1l, t2h, t2l;

        XT_LSX2IP(a0, p0_ld, SZ_CF32);
        XT_LSX2IP(a1, p1_ld, SZ_CF32);
        XT_LSX2IP(a2, p2_ld, SZ_CF32);
        XT_LSX2IP(a3, p3_ld, SZ_CF32);

        b0h=XT_ADD_LHH_S(a0 , a2); b0l=XT_ADD_S(a0 , a2);
        b1h=XT_ADD_LHH_S(a1 , a3); b1l=XT_ADD_S(a1 , a3);
        b2h=XT_SUB_LHH_S(a0 , a2); b2l=XT_SUB_S(a0 , a2);
        b3h=XT_SUB_LHH_S(a1 , a3); b3l=XT_SUB_S(a1 , a3);


        a0h=XT_ADD_S(b0h , b1h); a0l=XT_ADD_S(b0l , b1l);
        a2h=XT_SUB_S(b0h , b1h); a2l=XT_SUB_S(b0l , b1l);
        /* a1 <- b2-j*b3 */
        /* a3 <- b2+j*b3 */
        a1h=XT_ADD_S(b2h , b3l); a1l=XT_SUB_S(b2l , b3h);
        a3h=XT_SUB_S(b2h , b3l); a3l=XT_ADD_S(b2l , b3h);
        XT_SSIP(a0h, p0_st, sizeof(float32_t));
        XT_SSIP(a0l, p0_st, sizeof(float32_t));

        t0h = XT_LSI(p_twd, SZ_CF32);
        t0l = XT_LSI(p_twd, SZ_CF32 + sizeof(float32_t));
        t1l = XT_LSI(p_twd, 1 * 4);
        t2h = XT_LSI(p_twd, 2 * SZ_CF32);
        t2l = XT_LSI(p_twd, 2 * SZ_CF32+4);
        XT_LSXC(t1h, p_twd, (twdstep * 3)*SZ_CF32);


        b2h = XT_MUL_S(a2h, t0h);
        b2l = XT_MUL_S(a2h, t0l);
        b1h = XT_MUL_S(a1h, t1h);
        b1l = XT_MUL_S(a1h, t1l);
        b3h = XT_MUL_S(a3h, t2h);
        b3l = XT_MUL_S(a3h, t2l);

        XT_MSUB_S(b2h, a2l, t0l);
        XT_MADD_S(b2l, a2l, t0h);
        XT_MSUB_S(b1h, a1l, t1l);
        XT_MADD_S(b1l, a1l, t1h);
        XT_MSUB_S(b3h, a3l, t2l);
        XT_MADD_S(b3l, a3l, t2h);
        XT_SSI(b2l, p1_st, sizeof(float32_t));
        XT_SSIP(b2h, p1_st, 2*sizeof(float32_t));
        XT_SSI(b1l, p2_st, sizeof(float32_t));
        XT_SSIP(b1h, p2_st, 2*sizeof(float32_t));
        XT_SSI(b3l, p3_st, sizeof(float32_t));
        XT_SSIP(b3h, p3_st, 2*sizeof(float32_t));
      }
      p0_st = p3_st;
    }
  }

  /*----------------------------------------------------------------------------
   Last stage (radix-4 or radix-2 for odd powers of two) with bit reversal
   permutation.*/
  idx = 0;
  bitrevstride = 0x80000000U >> (logN-3+LOG2_SZ_CF32);
  if ( stride != 0 )
  { 
    p0_ld = (const xtfloatx2 *)(x);
    p1_ld = p0_ld+1;
    p2_ld = p1_ld+1;
    p3_ld = p2_ld+1;
    p0_st = (xtfloat *)(y);
    p1_st = p0_st+2*N4;
    p2_st = p1_st+2*N4;
    p3_st = p2_st+2*N4;
    
    __Pragma("loop_count min=2")
    for ( n=0; n<N4; n++ )
    {
        xtfloatx2 a0, a1, a2, a3;
        xtfloat a0h,a0l,a1h,a1l,a2h,a2l,a3h,a3l;
        xtfloat b0h,b0l,b1h,b1l,b2h,b2l,b3h,b3l;
        XT_LSX2IP(a0, p0_ld, 4*SZ_CF32);
        XT_LSX2IP(a1, p1_ld, 4*SZ_CF32);
        XT_LSX2IP(a2, p2_ld, 4*SZ_CF32);
        XT_LSX2IP(a3, p3_ld, 4*SZ_CF32);

        b0h=XT_ADD_LHH_S(a0 , a2); b0l=XT_ADD_S(a0 , a2);
        b1h=XT_ADD_LHH_S(a1 , a3); b1l=XT_ADD_S(a1 , a3);
        b2h=XT_SUB_LHH_S(a0 , a2); b2l=XT_SUB_S(a0 , a2);
        b3h=XT_SUB_LHH_S(a1 , a3); b3l=XT_SUB_S(a1 , a3);

        a0h=XT_ADD_S(b0h , b1h); a0l=XT_ADD_S(b0l , b1l);
        XT_SSX(a0l,p0_st,sizeof(float32_t)+idx);
        XT_SSX(a0h,p0_st,                  idx);
        a2h=XT_SUB_S(b0h , b1h); a2l=XT_SUB_S(b0l , b1l);
        XT_SSX(a2l,p2_st,sizeof(float32_t)+idx);
        XT_SSX(a2h,p2_st,                  idx);
        /* a1 <- b2-j*b3 */
        /* a3 <- b2+j*b3 */
        a1h=XT_ADD_S(b2h , b3l); a1l=XT_SUB_S(b2l , b3h);
        XT_SSX(a1l,p1_st,sizeof(float32_t)+idx);
        XT_SSX(a1h,p1_st,                  idx);
        a3h=XT_SUB_S(b2h , b3l); a3l=XT_ADD_S(b2l , b3h);
        XT_SSX(a3l,p3_st,sizeof(float32_t)+idx);
        XT_SSX(a3h,p3_st,                  idx);

        idx = AE_ADDBRBA32(idx, bitrevstride);
    }
  }
  else
  {
    bitrevstride >>= 1;

    p0_ld = (const xtfloatx2 *)(x);
    p1_ld = p0_ld+1;
    p0_st = (xtfloat *)(y);
    p1_st = (xtfloat *)XT_ADDX8(N4<<1, (uintptr_t)p0_st);
    
    __Pragma("loop_count min=1")
    for ( n=0; n<(N4<<1); n++ )
    {
        xtfloatx2 a0, a1;
        xtfloat b0h,b0l,b1h,b1l;
        XT_LSX2IP(a0, p0_ld, 2*SZ_CF32);
        XT_LSX2IP(a1, p1_ld, 2*SZ_CF32);
        b0h=XT_ADD_LHH_S(a0 , a1); b0l=XT_ADD_S(a0 , a1);
        XT_SSX(b0l,p0_st,sizeof(float32_t)+idx);
        XT_SSX(b0h,p0_st,                  idx);
        b1h=XT_SUB_LHH_S(a0 , a1); b1l=XT_SUB_S(a0 , a1);
        XT_SSX(b1l,p1_st,sizeof(float32_t)+idx);
        XT_SSX(b1h,p1_st,                  idx);
        idx = AE_ADDBRBA32(idx, bitrevstride);
    }
  }
  return 0;
} /* fft_cplxf_ie() */

#endif
