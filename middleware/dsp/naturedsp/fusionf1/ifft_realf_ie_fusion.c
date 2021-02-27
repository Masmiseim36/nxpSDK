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

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(int, ifft_realf_ie, (float32_t* y, complex_float* x, const  complex_float* twd, int twdstep, int N))
#else

/*-------------------------------------------------------------------------
  These functions make inverse FFT on real data with optimized memory usage.
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
  f_ie          floating point

  Input:
  x - complex input signal. Real and imaginary data are interleaved
  and real data goes first:
  --------------+----------+-----------------+----------------
  Function      |   Size   |  Allocated Size |       type    |
  --------------+----------+-----------------+----------------
  24x24_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  32x16_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  24x24_ie_24p  |   3*(N+2)|      4*N+8      |       uint8_t |
  32x16_ie_24p  |   3*(N+2)|      4*N+8      |       uint8_t |
  f_ie          |   N/2+1  |      N/2+1      | complex_float |
  --------------+----------+-----------------+----------------

  twd[2*N*twdstep*3/4]  twiddle factor table of a complex-valued FFT of size N*twdstep
  N                     FFT size
  twdstep               twiddle step
  scalingOpt            24x24_ie        - 3 (Fixed scaling)
                        32x16_ie        - 3 (Fixed scaling)
                        32x16_ie_24p    - 3 (Fixed scaling)
                        24x24_ie_24p    - 1 (24-bit scaling)
  Output:
  y - output spectrum. Real and imaginary data are interleaved and
  real data goes first:
  --------------+----------+-----------------+-----------
  Function      |   Size   |  Allocated Size |  type    |
  --------------+----------+-----------------+-----------
  24x24_ie      |      N   |      N          |   f24    |
  32x16_ie      |      N   |      N          |  int32_t |
  24x24_ie_24p  |    3*N   |      4*N+8      |  uint8_t |
  32x16_ie_24p  |    3*N   |      4*N+8      |  uint8_t |
  f_ie          |      N   |      N          | float32_t|
  --------------+----------+-----------------+-----------

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

int ifft_realf_ie        (float32_t* y, complex_float* x, const  complex_float* twd, int twdstep, int N)
#if 0
{
  const xtfloatx2 *restrict p_twd;
  const xtfloatx2 *restrict p0_ld;
  const xtfloatx2 *restrict p1_ld;
        xtfloatx2 *restrict p0_st;
        xtfloatx2 *restrict p1_st;
  xtfloatx2 a0, a1, a2, a3;
  xtfloatx2 b0, b1, b2, b3;
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
   Apply the real-to-complex spectrum conversion
  
   MATLAB code:
    twd = exp(-2*pi*1j*(0:N/4-1)/N);
    a0 = x(1:N/4);
    a1 = wrev(x(N/4+2:N/2+1));
    b0 = a0+conj(a1);
    b1 = (a0-conj(a1))*1j.*conj(twd);
    a0 = b0+b1;
    a1 = conj(b0-b1);
    x = [a0,conj(x(N/4+1)),wrev(a1(2:N/4))]/2; % N/2 complex samples
  */
  {
    xtfloatx2 tw, c05;
    
    p_twd = (const xtfloatx2 *)(twd+3*twdstep);
    p0_st = (xtfloatx2 *)(x);
    p1_st = p0_st+(N>>1)-1;
    p0_ld = p0_st;
    p1_ld = p1_st;
    c05 = XT_CONST_S(3);

    XT_LSX2IP(a0, p0_ld, SZ_CF32);
    a1 = XT_LSX2I(p1_ld, SZ_CF32);
    a0 = XT_SEL32_HH_SX2(a0,  a0);
    a1 = XT_SEL32_HH_SX2(a1, XT_NEG_SX2(a1));
    a0 = XT_ADD_SX2(a0 , a1);

    /* Divide spectrum by 2 so that overall scale factor of RIFFT
     * is 1/2*2/N == N. */
    a0 = XT_MUL_SX2(a0,c05);
    XT_SSX2IP(a0, p0_st, SZ_CF32);
    
    __Pragma("loop_count min=1")
    for ( n=1; n<(N>>2); n++ )
    {
      XT_LSX2IP(a0, p0_ld,       SZ_CF32);
      XT_LSX2XP(a1, p1_ld, -(int)SZ_CF32);
      
      /* b0 = 1/2*(a0+conj(a1)) */
      /* b1 = 1/2*(a0-conj(a1)) */
      b0 = b1 = XT_MUL_SX2(a0,c05);
      XT_MADDMUX_S(b0, c05, a1, 4);
      XT_MADDMUX_S(b1, c05, a1, 6);

      /* twd <- conj(twd)*j */
      XT_LSX2XP(tw, p_twd, 3*twdstep*SZ_CF32);
      tw = XT_SEL32_LH_SX2(tw, tw);
      
      /* a0 <- b0 + b1*twd */
      /* a1 <- b0 - b1*twd */ 
      a0 = a1 = b0;
      XT_MADDMUX_S(a0, b1, tw, 0);
      XT_MADDMUX_S(a0, b1, tw, 1);
      XT_MADDMUX_S(a1, b1, tw, 2);
      XT_MADDMUX_S(a1, b1, tw, 3);
      /* a1 <- conj( a1 ) */
#if 1
      a1 = XT_SEL32_HL_SX2(a1, XT_NEG_SX2(a1));
#else
      a1 = XT_CONJC_S(a1);
#endif
      
      XT_SSX2IP(a0, p0_st,       SZ_CF32);
      XT_SSX2XP(a1, p1_st, -(int)SZ_CF32);
    }
    
    a0 = XT_LSX2I(p0_ld, 0);
    /* a0 <- conj( a0 ) */
#if 1
    a0 = XT_SEL32_HL_SX2(a0, XT_NEG_SX2(a0));
#else
    a0 = XT_CONJC_S(a0);
#endif
    XT_SSX2I (a0, p0_st, 0);
  }

  /*----------------------------------------------------------------------------
   Perform the half-sized complex-valued inverse FFT
  
   MATLAB code:
    y = ifft(x);                        % Multiplies signal by 2/N!
    y = reshape([real(y);imag(y)],1,N); % N real samples
  */
  p0_ld = (const xtfloatx2 *)(x);
  p0_st = (      xtfloatx2 *)(y);
  
  if ( N == 8 )
  {
    xtfloatx2 c1, c025;
    /* Helping constant, used for multiplication by j */
    c1 = XT_CONST_S(1);
    c025 = XT_CONST_S(3);
    c025 = XT_MUL_SX2(c025,c025);

    XT_LSX2IP(a0, p0_ld, SZ_CF32);
    XT_LSX2IP(a1, p0_ld, SZ_CF32);
    XT_LSX2IP(a2, p0_ld, SZ_CF32);
    XT_LSX2IP(a3, p0_ld, SZ_CF32);

    b0 = XT_ADD_SX2(a0 , a2);
    b1 = XT_ADD_SX2(a1 , a3);
    b2 = XT_SUB_SX2(a0 , a2);
    b3 = XT_SUB_SX2(a1 , a3);
    
    a0 = XT_ADD_SX2(b0 , b1);
    a2 = XT_SUB_SX2(b0 , b1);
    /* a1 <- b2-j*b3 */
    /* a3 <- b2+j*b3 */
    a1 = a3 = b2;
    XT_MADDMUX_S(a1, c1, b3, 1);
    XT_MADDMUX_S(a3, c1, b3, 3);
    
    a0 = XT_MUL_SX2(a0,c025);
    a1 = XT_MUL_SX2(a1,c025);
    a2 = XT_MUL_SX2(a2,c025);
    a3 = XT_MUL_SX2(a3,c025);

    XT_SSX2IP(a0, p0_st, SZ_CF32);
    XT_SSX2IP(a1, p0_st, SZ_CF32);
    XT_SSX2IP(a2, p0_st, SZ_CF32);
    XT_SSX2IP(a3, p0_st, SZ_CF32);
  }
  else
  {
    /* Divides signal by N/2 */
    ifft_cplxf_ie((complex_float*)y, x, twd, twdstep<<1, N>>1);
  }
  return 0;
} /* ifft_realf_ie() */
#else
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
   Apply the real-to-complex spectrum conversion
  
   MATLAB code:
    twd = exp(-2*pi*1j*(0:N/4-1)/N);
    a0 = x(1:N/4);
    a1 = wrev(x(N/4+2:N/2+1));
    b0 = a0+conj(a1);
    b1 = (a0-conj(a1))*1j.*conj(twd);
    a0 = b0+b1;
    a1 = conj(b0-b1);
    x = [a0,conj(x(N/4+1)),wrev(a1(2:N/4))]/2; % N/2 complex samples
  */
  {
    xtfloatx2  c05;
    xtfloat a0h,a0l,a1h,a1l;
    xtfloat b0h,b0l,b1h,b1l;
    xtfloat th,tl;
    p_twd = (const xtfloat *)(twd+3*twdstep);
    p0_st = (xtfloat *)(x);
    p1_st = (xtfloat *)(p0_st+(N)-2);
    p0_ld = (const xtfloatx2*)p0_st;
    p1_ld = (const xtfloatx2*)p1_st;
    c05 = XT_CONST_S(3);

    XT_LSX2IP(a0, p0_ld, SZ_CF32);
    a1 = XT_LSX2I(p1_ld, SZ_CF32);
    /* Divide spectrum by 2 so that overall scale factor of RIFFT
     * is 1/2*2/N == N. */
    a0h=XT_ADD_LHH_S(a0,a1);
    a0l=XT_SUB_LHH_S(a0,a1);
    a0h=XT_MUL_S(a0h,c05);
    a0l=XT_MUL_S(a0l,c05);
    XT_SSIP(a0h, p0_st, sizeof(float32_t));
    XT_SSIP(a0l, p0_st, sizeof(float32_t));
    __Pragma("loop_count min=1")
    for ( n=1; n<(N>>2); n++ )
    {
        XT_LSX2IP(a0, p0_ld,       SZ_CF32);
        XT_LSX2XP(a1, p1_ld, -(int)SZ_CF32);
        /* b0 = 1/2*(a0+conj(a1)) */
        /* b1 = 1/2*(a0-conj(a1)) */
        b0h=XT_ADD_LHH_S(a0,a1); b0l=XT_SUB_LLL_S(a0,a1);
        b1h=XT_SUB_LHH_S(a0,a1); b1l=XT_ADD_LLL_S(a0,a1); 
        b0h=XT_MUL_S(b0h,c05); b0l=XT_MUL_S(b0l,c05);
        b1h=XT_MUL_S(b1h,c05); b1l=XT_MUL_S(b1l,c05);
        /* twd <- conj(twd)*j */
        th=XT_LSI(p_twd,sizeof(float32_t));
        XT_LSXP(tl,p_twd,3*twdstep*SZ_CF32);
        /* a0 <- b0 + b1*twd */
        /* a1 <- b0 - b1*twd */ 
        /* a1 <- conj( a1 ) */
        a0h=b0h;a0l=b0l;
        a1h=b0h;a1l=XT_NEG_S(b0l);
        XT_MADD_S(a0h,b1h,th); XT_MADD_S(a0l,b1h,tl);
        XT_MSUB_S(a0h,b1l,tl); XT_MADD_S(a0l,b1l,th);
        XT_MSUB_S(a1h,b1h,th); XT_MADD_S(a1l,b1h,tl);
        XT_MADD_S(a1h,b1l,tl); XT_MADD_S(a1l,b1l,th);
        XT_SSIP(a0h, p0_st, sizeof(float32_t));
        XT_SSIP(a0l, p0_st, sizeof(float32_t));
        XT_SSI (a1l, p1_st, sizeof(float32_t));
        XT_SSIP(a1h, p1_st,-2*(int)sizeof(float32_t));
    }   
    /* a0 <- conj( a0 ) */
    a0 = XT_LSX2I(p0_ld, 0);
    XT_SSI(XT_HIGH_S(a0),p0_st,0);
    XT_SSI(XT_NEG_S(a0),p0_st,1*sizeof(float32_t));
  }

  /*----------------------------------------------------------------------------
   Perform the half-sized complex-valued inverse FFT
  
   MATLAB code:
    y = ifft(x);                        % Multiplies signal by 2/N!
    y = reshape([real(y);imag(y)],1,N); % N real samples
  */
  p0_ld = (const xtfloatx2 *)(x);
  p0_st = (      xtfloat   *)(y);
  
  if ( N == 8 )
  {
        xtfloatx2 a0, a1, a2, a3;
        xtfloat a0h,a0l,a1h,a1l,a2h,a2l,a3h,a3l;
        xtfloat b0h,b0l,b1h,b1l,b2h,b2l,b3h,b3l;
        xtfloatx2 c025;
        c025 = XT_CONST_S(3);
        c025 = XT_MUL_SX2(c025,c025);

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
        a1h=XT_SUB_S(b2h , b3l); a1l=XT_ADD_S(b2l , b3h);
        a3h=XT_ADD_S(b2h , b3l); a3l=XT_SUB_S(b2l , b3h);
        a0h = XT_MUL_S(a0h,c025); a0l = XT_MUL_S(a0l,c025);
        a1h = XT_MUL_S(a1h,c025); a1l = XT_MUL_S(a1l,c025);
        a2h = XT_MUL_S(a2h,c025); a2l = XT_MUL_S(a2l,c025);
        a3h = XT_MUL_S(a3h,c025); a3l = XT_MUL_S(a3l,c025);
        XT_SSIP(a0h, p0_st, sizeof(float32_t)); XT_SSIP(a0l, p0_st, sizeof(float32_t));
        XT_SSIP(a1h, p0_st, sizeof(float32_t)); XT_SSIP(a1l, p0_st, sizeof(float32_t));
        XT_SSIP(a2h, p0_st, sizeof(float32_t)); XT_SSIP(a2l, p0_st, sizeof(float32_t));
        XT_SSIP(a3h, p0_st, sizeof(float32_t)); XT_SSIP(a3l, p0_st, sizeof(float32_t));
  }
  else
  {
    /* Divides signal by N/2 */
    ifft_cplxf_ie((complex_float*)y, x, twd, twdstep<<1, N>>1);
  }
  return 0;
} /* ifft_realf_ie() */
#endif
#endif
