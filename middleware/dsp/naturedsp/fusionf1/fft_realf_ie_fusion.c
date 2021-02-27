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
DISCARD_FUN(int, fft_realf_ie, (complex_float* y, float32_t* x, const complex_float* twd, int twdstep, int N))
#else

/*-------------------------------------------------------------------------
  These functions make FFT on real data with optimized memory usage.
  Scaling  : 
      24x24_ie        - Fixed data scaling at each stage
      32x16_ie        - Fixed data scaling at each stage
      32x16_ie_24p    - Fixed data scaling at each stage
      24x24_ie_24p    - 24-bit scaling
    
  NOTES:
  1. Bit-reversing reordering is done here.
  2. INPUT DATA MAY APPEAR DAMAGED after the call
  3. FFT functions may use input and output buffers for temporal
  storage of intermediate 32-bit data, so FFT functions
  with 24-bit packed I/O (Nx3-byte data) require that the
  buffers are large enough to keep Nx4-byte data
  4. Forward/inverse FFT of size N may be supplied with constant data
  (twiddle factors) of a larger-sized FFT = N*twdstep.

  Precision:
  24x24_ie      24-bit input/outputs, 24-bit data, 24-bit twiddles
  32x16_ie      32-bit input/outputs, 32-bit data, 16-bit twiddles
  24x24_ie_24p  24-bit packed input/outputs, 24-bit data, 24-bit twiddles
  32x16_ie_24p  24-bit packed input/outputs, 32-bit data, 16-bit twiddles
  f             floating point

  Input:
  x - real input signal. Real and imaginary data are interleaved
  and real data goes first:
  --------------+----------+-----------------+-----------
  Function      |   Size   |  Allocated Size |  type    |
  --------------+----------+-----------------+-----------
  24x24_ie      |     N    |      N          |   f24    |
  32x16_ie      |     N    |      N          |  int32_t |
  24x24_ie_24p  |     3*N  |      4*N+8      |  uint8_t |
  32x16_ie_24p  |     3*N  |      4*N+8      |  uint8_t |
  --------------+----------+-----------------+-----------

  twd[N*twdstep*3/4]    twiddle factor table of a complex-valued 
                        FFT of size N*twdstep
  N                     FFT size
  twdstep               twiddle step
  scalingOpt            24x24_ie        - 3 (Fixed scaling)
                        32x16_ie        - 3 (Fixed scaling)
                        32x16_ie_24p    - 3 (Fixed scaling)
                        24x24_ie_24p    - 1 (24-bit scaling)

  Output:
  y - output spectrum. Real and imaginary data are interleaved and
  real data goes first:
  --------------+----------+-----------------+----------------
  Function      |   Size   |  Allocated Size |  type         |
  --------------+----------+-----------------+----------------
  24x24_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  32x16_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  24x24_ie_24p  |  3*(N+2) |      4*N+8      |  uint8_t      |
  32x16_ie_24p  |  3*(N+2) |      4*N+8      |  uint8_t      |
  f_ie          |   N/2+1  |      N/2+1      |complex_float  |
  --------------+----------+-----------------+----------------

  Returned value: total number of right shifts occurred during scaling
  procedure

  Restrictions:
  x,y - should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 256, 512, 1024 for fixed point functions, power of two and >=8 for 
        floating point

-------------------------------------------------------------------------*/

#include "NatureDSP_Signal.h"
#define SZ_CF32 (sizeof(complex_float))

int fft_realf_ie        (complex_float* y,float32_t* x, const complex_float* twd, int twdstep, int N)
{
  const xtfloat   *restrict p_twd;
  const xtfloatx2 *restrict p0_ld;
  const xtfloatx2 *restrict p1_ld;
        xtfloat   *restrict p0_st;
        xtfloat   *restrict p1_st;
  xtfloatx2 a0, a1;

  int n;
  
  NASSERT( x );
  NASSERT( y );
  NASSERT( twd );

  NASSERT( (void*)x != y );

  NASSERT_ALIGN( x, 8 );
  NASSERT_ALIGN( y, 8 );
  NASSERT_ALIGN( twd, 8 );

  NASSERT( twdstep >= 1 );
  NASSERT( N>=8 && 0 == (N&(N-1)) );
  
  /*----------------------------------------------------------------------------
   Perform the half-sized complex-valued forward FFT
  
   MATLAB code:
    y(1:N/2) = fft(x(1:2:N)+1j*x(2:2:N));
  */
  p0_ld = (const xtfloatx2 *)(x);
  p0_st = (      xtfloat   *)(y);

  if ( N == 8 )
  {
        xtfloatx2 a0, a1, a2, a3;
        xtfloat a0h,a0l,a1h,a1l,a2h,a2l,a3h,a3l;
        xtfloat b0h,b0l,b1h,b1l,b2h,b2l,b3h,b3l;

        XT_LSX2IP(a0, p0_ld, SZ_CF32);
        XT_LSX2IP(a1, p0_ld, SZ_CF32);
        XT_LSX2IP(a2, p0_ld, SZ_CF32);
        XT_LSX2IP(a3, p0_ld, SZ_CF32);
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
        XT_SSIP(a1h, p0_st, sizeof(float32_t));
        XT_SSIP(a1l, p0_st, sizeof(float32_t));
        XT_SSIP(a2h, p0_st, sizeof(float32_t));
        XT_SSIP(a2l, p0_st, sizeof(float32_t));
        XT_SSIP(a3h, p0_st, sizeof(float32_t));
        XT_SSIP(a3l, p0_st, sizeof(float32_t));
  }
  else
  {
    fft_cplxf_ie( y, (complex_float*)x, twd, twdstep<<1, N>>1 );
  }

  /*----------------------------------------------------------------------------
   Apply the in-place real-to-complex spectrum conversion
  
   MATLAB code:
    twd = exp(-2*pi*1j*(0:N/4-1)/N);
    a0 = y(1:N/4);
    a1 = [y(1),wrev(y(N/4+2:N/2))];
    b0 = 1/2*(a0+conj(a1));
    b1 = 1/2*(a0-conj(a1))*-1j.*twd;
    a0 = b0+b1;
    a1 = b0-b1;
    y(1:N) = [a0,conj(y(N/4+1)),wrev(conj(a1))];
  */
  {
    xtfloat a0h,a0l,a1h,a1l;
    xtfloat b0h,b0l,b1h,b1l;
    xtfloat th,tl;
    xtfloatx2 c05;

    p_twd = (const xtfloat  *)(twd+3*twdstep);
    p0_st = (xtfloat *)(y);
    p1_st = p0_st+(N);
    p0_ld = (const xtfloatx2*)(p0_st);
    p1_ld = (const xtfloatx2*)(p1_st-2);

    c05 = XT_CONST_S(3);
    
    /* a0.re = y[0].re+y[0].im; a0.im = 0 */
    /* a1.re = y[0].re-y[0].im; a1.im = 0 */
    XT_LSX2IP(a0, p0_ld, SZ_CF32);
    b0h=XT_ADD_LHL_S(a0,a0); 
    b1h=XT_NEG_S(XT_SUB_LLH_S(a0,a0)); 
    b0l=b1l=XT_CONST_S(0);
    XT_SSIP(b0h, p0_st, sizeof(float32_t));
    XT_SSIP(b0l, p0_st, sizeof(float32_t));
    XT_SSI (b1l, p1_st, sizeof(float32_t));
    XT_SSIP(b1h, p1_st,-2*(int)sizeof(float32_t));
    __Pragma("loop_count min=1")
    for ( n=1; n<(N>>2); n++ )
    {
        XT_LSX2IP(a0, p0_ld,       SZ_CF32);
        XT_LSX2XP(a1, p1_ld, -(int)SZ_CF32);

        /* b0 <- 1/2*(a0+conj(a1)) */
        /* b1 <- 1/2*(a0-conj(a1)) */
        b0h=XT_ADD_LHH_S(a0,a1); b0l=XT_SUB_LLL_S(a0,a1);
        b1h=XT_SUB_LHH_S(a0,a1); b1l=XT_ADD_LLL_S(a0,a1); 
        b0h=XT_MUL_S(b0h,c05); b0l=XT_MUL_S(b0l,c05);
        b1h=XT_MUL_S(b1h,c05); b1l=XT_MUL_S(b1l,c05);
      
        /* a0 <- b0-j*b1*twd */
        /* a1 <- b0+j*b1*twd */
        th=XT_LSI(p_twd,sizeof(float32_t));
        XT_LSXP(tl,p_twd,3*twdstep*SZ_CF32);

        a0h=b0h;a0l=b0l;
        a1h=b0h;a1l=XT_NEG_S(b0l);
        XT_MADD_S(a0h,b1h,th); XT_MSUB_S(a0l,b1h,tl); 
        XT_MADD_S(a0h,b1l,tl); XT_MADD_S(a0l,b1l,th); 
        XT_MSUB_S(a1h,b1h,th); XT_MSUB_S(a1l,b1h,tl);
        XT_MSUB_S(a1h,b1l,tl); XT_MADD_S(a1l,b1l,th);

        XT_SSIP(a0h, p0_st, sizeof(float32_t));
        XT_SSIP(a0l, p0_st, sizeof(float32_t));
        XT_SSI (a1l, p1_st, sizeof(float32_t));
        XT_SSIP(a1h, p1_st,-2*(int)sizeof(float32_t));
    }
    a0 = XT_LSX2I(p0_ld, 0);
    XT_SSI(XT_HIGH_S(a0),p0_st,0);
    XT_SSI(XT_NEG_S(a0),p0_st,1*sizeof(float32_t));
  }
  return 0;
} /* fft_realf_ie() */
#endif
