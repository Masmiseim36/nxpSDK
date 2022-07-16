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
    Code optimized for Fusion
    Floating point DCT 
*/

#include "NatureDSP_Signal.h"
#include "common.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(int, dctf, (float32_t *  y, float32_t * x, int N))
#else
/*-------------------------------------------------------------------------
  Discrete Cosine Transform :
  These functions apply DCT (Type II) to input
  NOTES:
  1. DCT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call.

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles
  f      floating point

  Input:
  x[N]          input signal
  N             DCT size
  scalingOpt    scaling option
  Output:
  y[N]          transform output
  return ed value:
  total number of right shifts occurred during scaling procedure (always 5 
  for fixed point functions and 0 for floating point function)
  Restriction:
  x,y should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 32 for fixed point routines, 32 or 64 for floating point

-------------------------------------------------------------------------*/
#define SZ_CF32 (sizeof(complex_float))
#define SZ_F32  (sizeof(float32_t))

/*
exp(i*pi/2*(0:N-1)/N), N=64
*/
static const union ufloat32uint32 ALIGN(8) dct_twd64[128]=
{
{0x3f800000},{0x00000000},{0x3f7fec43},{0x3cc90ab0},{0x3f7fb10f},{0x3d48fb30},{0x3f7f4e6d},{0x3d96a905},
{0x3f7ec46d},{0x3dc8bd36},{0x3f7e1324},{0x3dfab273},{0x3f7d3aac},{0x3e164083},{0x3f7c3b28},{0x3e2f10a2},
{0x3f7b14be},{0x3e47c5c2},{0x3f79c79d},{0x3e605c13},{0x3f7853f8},{0x3e78cfcc},{0x3f76ba07},{0x3e888e93},
{0x3f74fa0b},{0x3e94a031},{0x3f731447},{0x3ea09ae5},{0x3f710908},{0x3eac7cd4},{0x3f6ed89e},{0x3eb8442a},
{0x3f6c835e},{0x3ec3ef15},{0x3f6a09a7},{0x3ecf7bca},{0x3f676bd8},{0x3edae880},{0x3f64aa59},{0x3ee63375},
{0x3f61c598},{0x3ef15aea},{0x3f5ebe05},{0x3efc5d27},{0x3f5b941a},{0x3f039c3d},{0x3f584853},{0x3f08f59b},
{0x3f54db31},{0x3f0e39da},{0x3f514d3d},{0x3f13682a},{0x3f4d9f02},{0x3f187fc0},{0x3f49d112},{0x3f1d7fd1},
{0x3f45e403},{0x3f226799},{0x3f41d870},{0x3f273656},{0x3f3daef9},{0x3f2beb4a},{0x3f396842},{0x3f3085bb},
{0x3f3504f3},{0x3f3504f3},{0x3f3085bb},{0x3f396842},{0x3f2beb4a},{0x3f3daef9},{0x3f273656},{0x3f41d870},
{0x3f226799},{0x3f45e403},{0x3f1d7fd1},{0x3f49d112},{0x3f187fc0},{0x3f4d9f02},{0x3f13682a},{0x3f514d3d},
{0x3f0e39da},{0x3f54db31},{0x3f08f59b},{0x3f584853},{0x3f039c3d},{0x3f5b941a},{0x3efc5d27},{0x3f5ebe05},
{0x3ef15aea},{0x3f61c598},{0x3ee63375},{0x3f64aa59},{0x3edae880},{0x3f676bd8},{0x3ecf7bca},{0x3f6a09a7},
{0x3ec3ef15},{0x3f6c835e},{0x3eb8442a},{0x3f6ed89e},{0x3eac7cd4},{0x3f710908},{0x3ea09ae5},{0x3f731447},
{0x3e94a031},{0x3f74fa0b},{0x3e888e93},{0x3f76ba07},{0x3e78cfcc},{0x3f7853f8},{0x3e605c13},{0x3f79c79d},
{0x3e47c5c2},{0x3f7b14be},{0x3e2f10a2},{0x3f7c3b28},{0x3e164083},{0x3f7d3aac},{0x3dfab273},{0x3f7e1324},
{0x3dc8bd36},{0x3f7ec46d},{0x3d96a905},{0x3f7f4e6d},{0x3d48fb30},{0x3f7fb10f},{0x3cc90ab0},{0x3f7fec43}
};

/*
N=32;  
twd = exp(-2j*pi*[1;2;3]*(0:N/4-1)/N);
twd_ri = reshape([real(twd(:).');imag(twd(:).')],1,2*numel(twd));
*/
static const union ufloat32uint32 ALIGN(8) fft_twd32[48]=
{
{0x3f800000},{0x00000000},{0x3f800000},{0x00000000},{0x3f800000},{0x00000000},
{0x3f7b14be},{0xbe47c5c2},{0x3f6c835e},{0xbec3ef15},{0x3f54db31},{0xbf0e39da},
{0x3f6c835e},{0xbec3ef15},{0x3f3504f3},{0xbf3504f3},{0x3ec3ef15},{0xbf6c835e},
{0x3f54db31},{0xbf0e39da},{0x3ec3ef15},{0xbf6c835e},{0xbe47c5c2},{0xbf7b14be},
{0x3f3504f3},{0xbf3504f3},{0x248d3132},{0xbf800000},{0xbf3504f3},{0xbf3504f3},
{0x3f0e39da},{0xbf54db31},{0xbec3ef15},{0xbf6c835e},{0xbf7b14be},{0xbe47c5c2},
{0x3ec3ef15},{0xbf6c835e},{0xbf3504f3},{0xbf3504f3},{0xbf6c835e},{0x3ec3ef15},
{0x3e47c5c2},{0xbf7b14be},{0xbf6c835e},{0xbec3ef15},{0xbf0e39da},{0x3f54db31}
};

/* 1/sqrt(2.0) */
static const union ufloat32uint32 _invsqrt2f_ = { 0x3f3504f3 };

int dctf     ( float32_t * restrict y,float32_t * restrict x,int N)
/*
    Reference Matlab code:
    function y=dctf(x)
    N=numel(x);
    y(1:N/2)     =x(1:2:N);
    y(N:-1:N/2+1)=x(2:2:N);
    % take fft of N/2
    y=fft(y(1:2:N)+j*y(2:2:N));
    w=exp(i*pi/2*(0:N-1)/N);
    % DCT split algorithm
    Y0=y(1);
    T0=real(Y0)+imag(Y0);
    T1=real(Y0)-imag(Y0);
    z(1      )= real(T0);%*sqrt(2)/2;
    z(N/2+1  )= real(T1)*sqrt(2)/2;
    for k=2:N/4
        Y0=y(k);
        Y1=y(N/2+2-k);
        COSI=(w(4*(k-1)+1));
        W1=w(k);
        W2=w(N/2+2-k);
        S=Y0+Y1;
        D=Y0-Y1;
        T0=i*real(D)+imag(S);
        T1=i*imag(D)+real(S);
        Y0=  ( imag(T0)*imag(COSI)-real(T0)*real(COSI)) + ...
           i*( real(T0)*imag(COSI)+imag(T0)*real(COSI));
        T0=0.5*(T1-Y0);
        T1=0.5*(T1+Y0);
        z(k      )= real(T0)*real(W1)+imag(T0)*imag(W1);
        z(N+2-k  )= real(T0)*imag(W1)-imag(T0)*real(W1);
        z(N/2+2-k)= real(T1)*real(W2)-imag(T1)*imag(W2);
        z(N/2+k  )= real(T1)*imag(W2)+imag(T1)*real(W2);
    end
    W1=w(N/4+1);
    T0=y(N/4+1);
    z(N/4+1  )= real(T0)*real(W1)-imag(T0)*imag(W1);
    z(N+1-N/4)= real(T0)*imag(W1)+imag(T0)*real(W1);
    y=z;
*/
#if 0
{
    const xtfloatx2 *restrict p0_twd;
    const xtfloatx2 *restrict p1_twd;
    const xtfloatx2 *restrict p2_twd;
    const xtfloatx2 *restrict p0_ld;
    const xtfloatx2 *restrict p1_ld;
          xtfloatx2 *restrict p0_stx2;
          xtfloatx2 *restrict p1_stx2;
          xtfloat   *restrict p0_st;
          xtfloat   *restrict p1_st;
          xtfloat   *restrict p2_st;
          xtfloat   *restrict p3_st;
    xtfloatx2 t0, t1, y0, y1,
              w1, w2, s, d, cosi, c05;
    xtfloat b0, b1, re, im, invsqrt2f;
    ae_int32x2 t32x2;
    int k, n, twd_stride;
    int N2, N4;

    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT(x!=y);
    ASSERT(N==32 || N==64);

    N2 = N>>1;
    N4 = N2>>1;

    /* permute inputs */
    p0_ld  = (const xtfloatx2 *)x;
    p0_stx2 = (xtfloatx2 *)y;
    p1_stx2 = (xtfloatx2 *)(y+N-2);
    __Pragma("loop_count min=1")
    for (n=0; n<N4; n++)
    {
      /* y[n]    =x[2*n+0] */
      /* y[N-1-n]=x[2*n+1] */
      XT_LSX2IP(t0, p0_ld, SZ_CF32);
      XT_LSX2IP(t1, p0_ld, SZ_CF32);
      y0 = XT_SEL32_HH_SX2(t0, t1);
      y1 = XT_SEL32_LL_SX2(t1, t0);
      XT_SSX2IP(y0, p0_stx2,       SZ_CF32);
      XT_SSX2XP(y1, p1_stx2, -(int)SZ_CF32);
    }

    /* compute fft(N/2) */
    /* set twiddle stride to 2 for N=32 or 1 for N=64 */
    twd_stride=2;
    XT_MOVEQZ(twd_stride, 1, N-64);

    fft_cplxf_ie((complex_float*)x,(complex_float*)y,(complex_float*)fft_twd32,twd_stride,N2);

    /* make final DCT transformation of FFT outputs */
    p0_ld  = (const xtfloatx2 *)x;
    p1_ld  = (const xtfloatx2 *)x+N2-1;
    p0_twd = (const xtfloatx2 *)dct_twd64+4*twd_stride;
    p1_twd = (const xtfloatx2 *)dct_twd64+twd_stride;
    p2_twd = (const xtfloatx2 *)dct_twd64+(N2-1)*twd_stride;
    p0_st = (xtfloat *)y;
    p1_st = p0_st+N2;
    p2_st = p1_st-1;
    p3_st = p2_st+N2;

    /* Load constants */
    c05 = XT_CONST_S(3);/* 0.5 */
    invsqrt2f = XT_LSI((xtfloat *)&_invsqrt2f_, 0);/* 1/sqrt(2) */

    XT_LSX2IP(y0, p0_ld, SZ_CF32);
    /* b0 = y0.re + y0.im */
    /* b1 = y0.re - y0.im */
    re = XT_HIGH_S(y0);
    im = XT_LOW_S (y0);
    b0=XT_ADD_S(re, im);
    b1=XT_SUB_S(re, im);
    XT_SSIP(b0, p0_st, SZ_F32);
    b1 = XT_MUL_S(b1, invsqrt2f);
    XT_SSIP(b1, p1_st, SZ_F32);

    __Pragma("loop_count min=2")
    for (k=1; k<N4; k++)
    {
      XT_LSX2IP(y0, p0_ld,       SZ_CF32);
      XT_LSX2XP(y1, p1_ld, -(int)SZ_CF32);
      XT_LSX2XP(cosi, p0_twd, 4*twd_stride*SZ_CF32);
      XT_LSX2XP(w1  , p1_twd,   twd_stride*SZ_CF32);
      XT_LSX2XP(w2  , p2_twd,  -twd_stride*SZ_CF32);
      
      s  = XT_ADD_SX2(y0 , y1);
      d  = XT_SUB_SX2(y0 , y1);
      /* t0.re = s.im; t0.im = d.re */
      t0 = XT_SEL32_LH_SX2(s, d);
      /* t0.re = s.re; t0.im = d.im */
      t1 = XT_SEL32_HL_SX2(s, d);

      y0 = XT_MULC_S(t0, cosi);
      /* t0 = 0.5*(t1+conj(y0)) */
      /* t1 = 0.5*(t1-conj(y0)) */
      t0 = t1 = XT_MUL_SX2(t1,c05);
      XT_MADDMUX_S(t0, c05, y0, 4);
      XT_MADDMUX_S(t1, c05, y0, 6);
      t0 = XT_MULCCONJ_S(w1, t0);
      t1 = XT_MULC_S    (w2, t1);
      
      /* y[k    ]= t0.re */
      /* y[N-k  ]= t0.im */
      re = XT_HIGH_S(t0);
      t32x2 = XT_AE_MOVINT32X2_FROMXTFLOATX2(t0);
      XT_SSIP(re, p0_st, SZ_F32);/* save real part */
      AE_S32_L_IP(t32x2, p3_st, -(int)SZ_F32);/* save imag part */
      /* y[N/2-k]= t1.re */
      /* y[N/2+k]= t1.im */
      re = XT_HIGH_S(t1);
      t32x2 = XT_AE_MOVINT32X2_FROMXTFLOATX2(t1);
      XT_SSIP(re, p2_st, -(int)SZ_F32);/* save real part */
      AE_S32_L_IP(t32x2, p1_st, SZ_F32);/* save imag part*/
    }
    t0 = XT_LSX2I(p0_ld, 0);
    w1 = XT_LSX2I(p1_twd, 0);
    t0 = XT_MULC_S(t0, w1);

    re = XT_HIGH_S(t0);
    im = XT_LOW_S (t0);
    XT_SSI(re, p0_st, 0);
    XT_SSI(im, p3_st, 0);

    return 0;
} /* dctf() */
#elif 0
{
    const xtfloatx2 *restrict p0_twd;
    const xtfloatx2 *restrict p1_twd;
    const xtfloatx2 *restrict p2_twd;
    const xtfloatx2 *restrict p0_ld;
    const xtfloatx2 *restrict p1_ld;
          xtfloatx2 *restrict p0_stx2;
          xtfloatx2 *restrict p1_stx2;
          xtfloat   *restrict p0_st;
          xtfloat   *restrict p1_st;
          xtfloat   *restrict p2_st;
          xtfloat   *restrict p3_st;
    xtfloatx2 t0, t1, y0, y1,
              w1, w2, cosi;
    xtfloat b0, b1, invsqrt2f, c05;
    xtfloat sl,sh,dl,dh,y0l,y0h,th,tl,zre,zim;
    ae_int32x2 t32x2;
    int k, n, twd_stride;
    int N2, N4;

    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT(x!=y);
    ASSERT(N==32 || N==64);

    N2 = N>>1;
    N4 = N2>>1;

    /* permute inputs */
    p0_ld  = (const xtfloatx2 *)x;
    p0_stx2 = (xtfloatx2 *)y;
    p1_stx2 = (xtfloatx2 *)(y+N-2);
    __Pragma("loop_count min=1")
    for (n=0; n<N4; n++)
    {
        xtfloatx2 t0,t1,y0,y1;
        /* y[n]    =x[2*n+0] */
        /* y[N-1-n]=x[2*n+1] */
        XT_LSX2IP(t0, p0_ld, SZ_CF32);
        XT_LSX2IP(t1, p0_ld, SZ_CF32);
        y0 = XT_SEL32_HH_SX2(t0, t1);
        y1 = XT_SEL32_LL_SX2(t1, t0);
        XT_SSX2IP(y0, p0_stx2,       SZ_CF32);
        XT_SSX2XP(y1, p1_stx2, -(int)SZ_CF32);
    }

    /* compute fft(N/2) */
    /* set twiddle stride to 2 for N=32 or 1 for N=64 */
    twd_stride=2;
    XT_MOVEQZ(twd_stride, 1, N-64);

    fft_cplxf_ie((complex_float*)x,(complex_float*)y,(complex_float*)fft_twd32,twd_stride,N2);

    /* make final DCT transformation of FFT outputs */
    p0_ld  = (const xtfloatx2 *)x;
    p1_ld  = (const xtfloatx2 *)x+N2-1;
    p0_twd = (const xtfloatx2 *)dct_twd64+4*twd_stride;
    p1_twd = (const xtfloatx2 *)dct_twd64+twd_stride;
    p2_twd = (const xtfloatx2 *)dct_twd64+(N2-1)*twd_stride;
    p0_st = (xtfloat *)y;
    p1_st = p0_st+N2;
    p2_st = p1_st-1;
    p3_st = p2_st+N2;

    /* Load constants */
    c05 = XT_CONST_S(3);/* 0.5 */
    invsqrt2f = XT_LSI((xtfloat *)&_invsqrt2f_, 0);/* 1/sqrt(2) */
    invsqrt2f = XT_NEG_S(invsqrt2f);
    XT_LSX2IP(y0, p0_ld, SZ_CF32);
    /* b0 = y0.re + y0.im */
    /* b1 = y0.re - y0.im */
    b0=XT_ADD_LLH_S(y0,y0);
    b1=XT_SUB_LLH_S(y0,y0); 
    XT_SSIP(b0, p0_st, SZ_F32);
    b1 = XT_MUL_S(b1, invsqrt2f);
    XT_SSIP(b1, p1_st, SZ_F32);

    __Pragma("loop_count min=2")
    for (k=1; k<N4; k++)
    {
        XT_LSX2IP(y0, p0_ld,       SZ_CF32);
        XT_LSX2XP(y1, p1_ld, -(int)SZ_CF32);

        sl=XT_ADD_S    (y0,y1);
        sh=XT_ADD_LHH_S(y0,y1);
        dl=XT_SUB_S    (y0,y1);
        dh=XT_SUB_LHH_S(y0,y1);
        /* t0 = 0.5*(t1+conj(y0)) */
        /* t1 = 0.5*(t1-conj(y0)) */

        XT_LSX2XP(cosi, p0_twd, 4*twd_stride*SZ_CF32);
        y0h=XT_MUL_LLH_S((xtfloatx2)sl,cosi);
        XT_MSUB_S(y0h,dh,cosi);
        y0l=XT_MUL_S(cosi,sl);
        XT_MADD_LLH_S(y0l,(xtfloatx2)dh,cosi);
        sh=XT_MUL_S(sh,c05);
        dl=XT_MUL_S(dl,c05);
        th= sh; tl=dl;
        XT_MADD_S(sh,c05,y0h);
        XT_MSUB_S(dl,y0l,c05); 
        XT_MSUB_S(th,c05,y0h); 
        XT_MADD_S(tl,y0l,c05); 

        XT_LSX2XP(w1  , p1_twd,   twd_stride*SZ_CF32);
        zre=XT_MUL_LLH_S((xtfloatx2)sh,w1);
        zim=XT_MUL_S(w1,sh);
        XT_MADD_S(zre,w1,dl);
        XT_MSUB_LLH_S(zim,(xtfloatx2)dl,w1);
        XT_SSIP(zre, p0_st, SZ_F32);      /* save real part */
        XT_SSIP(zim, p3_st, -(int)SZ_F32);/* save imag part */

        XT_LSX2XP(w2  , p2_twd,  -twd_stride*SZ_CF32);
        zre=XT_MUL_LLH_S((xtfloatx2)th,w2);
        zim=XT_MUL_LLH_S((xtfloatx2)tl,w2);
        XT_MSUB_S(zre,w2,tl);
        XT_MADD_S(zim,w2,th);
        /* y[N/2-k]= t1.re */
        /* y[N/2+k]= t1.im */
        XT_SSIP(zre, p2_st, -(int)SZ_F32);/* save real part */
        XT_SSIP(zim, p1_st, SZ_F32);      /* save imag part*/
    }
    th = XT_LSI((const xtfloat*)p0_ld, 0);
    tl = XT_LSI((const xtfloat*)p0_ld, sizeof(float32_t));
    w1 = XT_LSX2I(p1_twd, 0);
    zre=XT_MUL_LLH_S((xtfloatx2)th,w1);
    zim=XT_MUL_S(w1,th);
    XT_MSUB_S(zre,tl,w1);
    XT_MADD_LLH_S(zim,(xtfloatx2)tl,w1);
    XT_SSI(zre, p0_st, 0);
    XT_SSI(zim, p3_st, 0);
    return 0;
} /* dctf() */
#else
{
    const xtfloat *restrict p0_twd;
    const xtfloat *restrict p1_twd;
    const xtfloat *restrict p2_twd;
    const xtfloatx2 *restrict p0_ld;
    const xtfloatx2 *restrict p1_ld;
          xtfloatx2 *restrict p0_stx2;
          xtfloatx2 *restrict p1_stx2;
          xtfloat   *restrict p0_st;
          xtfloat   *restrict p1_st;
          xtfloat   *restrict p2_st;
          xtfloat   *restrict p3_st;
    xtfloatx2 y0, y1;
    xtfloat b0, b1, invsqrt2f, c05;
    xtfloat sl,sh,dl,dh,y0l,y0h,th,tl,zre,zim,cosh,cosl,w1h,w1l,w2h,w2l;
    int k, n, twd_stride;
    int N2, N4;

    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT(x!=y);
    ASSERT(N==32 || N==64);

    N2 = N>>1;
    N4 = N2>>1;

    /* permute inputs */
    p0_ld  = (const xtfloatx2 *)x;
    p0_stx2 = (xtfloatx2 *)y;
    p1_stx2 = (xtfloatx2 *)(y+N-2);
    __Pragma("loop_count min=1")
    for (n=0; n<N4; n++)
    {
        xtfloatx2 t0,t1,y0,y1;
        /* y[n]    =x[2*n+0] */
        /* y[N-1-n]=x[2*n+1] */
        XT_LSX2IP(t0, p0_ld, SZ_CF32);
        XT_LSX2IP(t1, p0_ld, SZ_CF32);
        y0 = XT_SEL32_HH_SX2(t0, t1);
        y1 = XT_SEL32_LL_SX2(t1, t0);
        XT_SSX2IP(y0, p0_stx2,       SZ_CF32);
        XT_SSX2XP(y1, p1_stx2, -(int)SZ_CF32);
    }

    /* compute fft(N/2) */
    /* set twiddle stride to 2 for N=32 or 1 for N=64 */
    twd_stride=2;
    XT_MOVEQZ(twd_stride, 1, N-64);

    fft_cplxf_ie((complex_float*)x,(complex_float*)y,(complex_float*)fft_twd32,twd_stride,N2);

    /* make final DCT transformation of FFT outputs */
    p0_ld  = (const xtfloatx2 *)x;
    p1_ld  = (const xtfloatx2 *)x+N2-1;
    p0_twd = (const xtfloat *)(dct_twd64+8*twd_stride);
    p1_twd = (const xtfloat *)(dct_twd64+2*twd_stride);
    p2_twd = (const xtfloat *)(dct_twd64+2*(N2-1)*twd_stride);
    p0_st = (xtfloat *)y;
    p1_st = p0_st+N2;
    p2_st = p1_st-1;
    p3_st = p2_st+N2;

    /* Load constants */
    c05 = XT_CONST_S(3);/* 0.5 */
    invsqrt2f = XT_LSI((xtfloat *)&_invsqrt2f_, 0);/* 1/sqrt(2) */
    invsqrt2f = XT_NEG_S(invsqrt2f);
    XT_LSX2IP(y0, p0_ld, SZ_CF32);
    /* b0 = y0.re + y0.im */
    /* b1 = y0.re - y0.im */
    b0=XT_ADD_LLH_S(y0,y0);
    b1=XT_SUB_LLH_S(y0,y0); 
    XT_SSIP(b0, p0_st, SZ_F32);
    b1 = XT_MUL_S(b1, invsqrt2f);
    XT_SSIP(b1, p1_st, SZ_F32);

    __Pragma("loop_count min=2")
    for (k=1; k<N4; k++)
    {
        XT_LSX2IP(y0, p0_ld,       SZ_CF32);
        XT_LSX2XP(y1, p1_ld, -(int)SZ_CF32);

        sl=XT_ADD_S    (y0,y1);
        sh=XT_ADD_LHH_S(y0,y1);
        dl=XT_SUB_S    (y0,y1);
        dh=XT_SUB_LHH_S(y0,y1);
        /* t0 = 0.5*(t1+conj(y0)) */
        /* t1 = 0.5*(t1-conj(y0)) */

        cosl=XT_LSI ( p0_twd, sizeof(float32_t));
        XT_LSXP(cosh, p0_twd, 4*twd_stride*SZ_CF32);
        y0h=XT_MUL_S(sl,cosh);
        XT_MSUB_S(y0h,dh,cosl);
        y0l=XT_MUL_S(cosl,sl);
        XT_MADD_S(y0l,dh,cosh);
        sh=XT_MUL_S(sh,c05);
        dl=XT_MUL_S(dl,c05);
        th= sh; tl=dl;
        XT_MADD_S(sh,c05,y0h);
        XT_MSUB_S(dl,y0l,c05); 
        XT_MSUB_S(th,c05,y0h); 
        XT_MADD_S(tl,y0l,c05); 

        w1l=XT_LSI( p1_twd, sizeof(float32_t));
        XT_LSXP(w1h, p1_twd,   twd_stride*SZ_CF32);
        zre=XT_MUL_S(sh,w1h);
        zim=XT_MUL_S(w1l,sh);
        XT_MADD_S(zre,w1l,dl);
        XT_MSUB_S(zim,dl,w1h);
        XT_SSIP(zre, p0_st,       sizeof(float32_t));/* save real part */
        XT_SSIP(zim, p3_st, -(int)sizeof(float32_t));/* save imag part */

        w2l=XT_LSI ( p2_twd, sizeof(float32_t));
        XT_LSXP(w2h, p2_twd, -twd_stride*SZ_CF32);
        zre=XT_MUL_S(th,w2h);
        zim=XT_MUL_S(tl,w2h);
        XT_MSUB_S(zre,w2l,tl);
        XT_MADD_S(zim,w2l,th);
        /* y[N/2-k]= t1.re */
        /* y[N/2+k]= t1.im */
        XT_SSIP(zre, p2_st, -(int)SZ_F32);/* save real part */
        XT_SSIP(zim, p1_st, SZ_F32);      /* save imag part*/
    }
    th = XT_LSI((const xtfloat*)p0_ld, 0);
    tl = XT_LSI((const xtfloat*)p0_ld, sizeof(float32_t));
    w1l=XT_LSI( p1_twd, sizeof(float32_t));
    w1h=XT_LSI( p1_twd, 0);
    zre=XT_MUL_S(th,w1h);
    zim=XT_MUL_S(w1l,th);
    XT_MSUB_S(zre,tl,w1l);
    XT_MADD_S(zim,tl,w1h);
    XT_SSI(zre, p0_st, 0);
    XT_SSI(zim, p3_st, 0);
    return 0;
} /* dctf() */
#endif

#endif
