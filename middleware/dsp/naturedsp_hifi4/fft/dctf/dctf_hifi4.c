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
    Discrete Cosine Transform, Type II 
    C code optimized for HiFi4
   Integrit, 2006-2018
*/
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
#include "common.h"
#include "dct2_twd.h"

#include "common_fpu.h"

#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(int,dctf,( float32_t *  y,float32_t * x,dct_handle_t h))
#elif HAVE_VFPU

/*-------------------------------------------------------------------------
  Discrete Cosine Transform.
  These functions apply DCT (Type II, Type IV) to input.
  Scaling:
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      |       dct_16x16       |  3 - fixed scaling before each stage |
      |       dct_32x16       |  3 - fixed scaling before each stage |
      |       dct_32x32       |  3 - fixed scaling before each stage |
      |       dct4_32x16      |  3 - fixed scaling before each stage |
      |       dct4_32x32      |  3 - fixed scaling before each stage |
      +-----------------------+--------------------------------------+
  NOTES:
     1. DCT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call.
     2. N - DCT size (depends on selected DCT handle)

  Precision: 
  16x16  16-bit input/outputs, 16-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  32x32  32-bit input/outputs, 32-bit twiddles
  f      floating point

  Input:
  x[N]        input signal
  h           DCT handle
  scalingOpt  scaling option (see table above) 
              not applicable to the floating point function
  Output:
  y[N]        transform output
  
  Returned value:
              total number of right shifts occurred during scaling 
              procedure 
  Restriction:
  x,y         should not overlap
  x,y         aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
#define SZ_CF32 (sizeof(complex_float))
#define SZ_F32  (sizeof(float32_t))
/* 1/sqrt(2.0) */
static const union ufloat32uint32 _invsqrt2f_ = { 0x3f3504f3 };
int dctf     ( float32_t * restrict y,float32_t * restrict x,dct_handle_t h)
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
{
    const union ufloat32uint32 *rfft_split_twd; 
    const tdct2_twd *descr=(const tdct2_twd *)h;
    int N;
    const xtfloatx2 *restrict p0_twd;
    const xtfloatx2 *restrict p1_twd;
    const xtfloatx2 *restrict p2_twd;
    const xtfloatx2 *restrict p0_ld;
    const xtfloatx2 *restrict p1_ld;
    const xtfloatx2 *restrict p2_ld;
    const xtfloatx2 *restrict p3_ld;
          xtfloatx2 *restrict p0_stx2;
          xtfloatx2 *restrict p1_stx2;
          xtfloatx2 *restrict p2_stx2;
          xtfloatx2 *restrict p3_stx2;
          xtfloat   *restrict p0_st;
          xtfloat   *restrict p1_st;
          xtfloat   *restrict p2_st;
          xtfloat   *restrict p3_st;
    xtfloatx2 t0, t1, y0, y1,
              w1, w2, s, d, cosi, c05;
    xtfloat b0, b1, re, im, invsqrt2f;
    ae_int32x2 t32x2;
    int k, n;
    int N2, N4;

    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT(x!=y);
    NASSERT(descr->magic==MAGIC_DCT2_F);
    NASSERT(descr->N==32 || descr->N==64);
    N=descr->N;
    rfft_split_twd=(const union ufloat32uint32 *)descr->rfft_split_twd;
    N2 = N>>1;
    N4 = N2>>1;

    /* permute inputs */
    p0_ld  = (const xtfloatx2 *)x;
    p0_stx2 = (xtfloatx2 *)y;
    p1_stx2 = (xtfloatx2 *)(y+N-2);
    __Pragma("loop_count min=2")
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
    if(N == 32)
    {
      xtfloatx2 a0, a1, a2, a3;
      xtfloatx2 b0, b1, b2, b3;
      xtfloatx2 tw1, tw2, tw3;
      int idx, bitrevstride;
      p0_twd = (const xtfloatx2 *)descr->fft_twd;
      p0_stx2 = (xtfloatx2 *)(y);
      p1_stx2 = p0_stx2 + 4;
      p2_stx2 = p1_stx2 + 4;
      p3_stx2 = p2_stx2 + 4;
      p0_ld = p0_stx2;
      p1_ld = p1_stx2;
      p2_ld = p2_stx2;
      p3_ld = p3_stx2;
      /* Radix-4 butterfly */
      for ( n=0; n<4; n++ )
      {
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
        a1 = b2 + b3;
        a3 = b2 - b3;
        /* multiply by twiddle factors */
        XT_LSX2IP(tw1, p0_twd, SZ_CF32);
        XT_LSX2IP(tw2, p0_twd, SZ_CF32);
        XT_LSX2XC(tw3, p0_twd, (3-2)*SZ_CF32);
        b0 = a0;
        b1 = XT_MULC_S(a1, tw1);
        b2 = XT_MULC_S(a2, tw2);
        b3 = XT_MULC_S(a3, tw3);
        /* Two middle quartiles are swapped on all but the last stage to use the bit reversal
         * permutation instead of the digit reverse. */
        XT_SSX2IP(b0, p0_stx2, SZ_CF32);
        XT_SSX2IP(b2, p1_stx2, SZ_CF32);
        XT_SSX2IP(b1, p2_stx2, SZ_CF32);
        XT_SSX2IP(b3, p3_stx2, SZ_CF32);
      }
      __Pragma("no_reorder")
      /*-------------------------------------
       Last stage (radix-4) with bit reversal
       permutation.*/
      idx = 0;
      bitrevstride = 0x80000000U >> 4;

      p0_ld = (const xtfloatx2 *)(y);
      p1_ld = p0_ld+1;
      p2_ld = p1_ld+1;
      p3_ld = p2_ld+1;
      p0_stx2 = (xtfloatx2 *)(x);
      p1_stx2 = p0_stx2+4;
      p2_stx2 = p1_stx2+4;
      p3_stx2 = p2_stx2+4;
    
      for ( n=0; n<4; n++ )
      {
        XT_LSX2IP(a0, p0_ld, 4*SZ_CF32);
        XT_LSX2IP(a1, p1_ld, 4*SZ_CF32);
        XT_LSX2IP(a2, p2_ld, 4*SZ_CF32);
        XT_LSX2IP(a3, p3_ld, 4*SZ_CF32);

        b0 = a0 + a2;
        b1 = a1 + a3;
        b2 = a0 - a2;
        b3 = a1 - a3;
      
        a0 = b0 + b1;
        a2 = b0 - b1;
        /* a1 <- b2-j*b3 */
        /* a3 <- b2+j*b3 */
        b3 = XT_CONJC_S(b3);
        b3 = XT_SEL32_LH_SX2(b3, b3);
        a1 = b2 - b3;
        a3 = b2 + b3;
      
        XT_SSX2X(a0, p0_stx2, idx);
        XT_SSX2X(a1, p1_stx2, idx);
        XT_SSX2X(a2, p2_stx2, idx);
        XT_SSX2X(a3, p3_stx2, idx);

        idx = AE_ADDBRBA32(idx, bitrevstride);
      }
    }
    else
    {
        fft_cplxf_ie((complex_float*)x,(complex_float*)y,(complex_float*)descr->fft_twd,1,N2);
    }

    /* make final DCT transformation of FFT outputs */
    p0_ld  = (const xtfloatx2 *)x;
    p1_ld  = (const xtfloatx2 *)x+N2-1;
    p0_twd = (const xtfloatx2 *)rfft_split_twd+4;
    p1_twd = (const xtfloatx2 *)rfft_split_twd+1;
    p2_twd = (const xtfloatx2 *)rfft_split_twd+(N2-1);
    p0_st = (xtfloat *)y;
    p1_st = p0_st+N2;
    p2_st = p1_st-1;
    p3_st = p2_st+N2;

    /* Load constants */
    c05 = (xtfloatx2)0.5f;/* 0.5 */
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
      XT_LSX2IP(cosi, p0_twd, 4*SZ_CF32);
      XT_LSX2IP(w1  , p1_twd,   SZ_CF32);
      XT_LSX2XP(w2  , p2_twd, -(int)SZ_CF32);
      
      s  = y0 + y1;
      d  = y0 - y1;
      /* t0.re = s.im; t0.im = d.re */
      t0 = XT_SEL32_LH_SX2(s, d);
      /* t0.re = s.re; t0.im = d.im */
      t1 = XT_SEL32_HL_SX2(s, d);

      /* y0 = t0*cosi           */
      /* t0 = 0.5*(t1+conj(y0)) */
      /* t1 = 0.5*(t1-conj(y0)) */
      cosi = cosi*c05;
      t1 = t1*c05;
      y0 = XT_MULMUX_S(t0, cosi, 1);
      XT_MADDMUX_S(y0, t0, cosi, 7);
      t0 = t1 + y0;
      t1 = t1 - y0;
      t0 = XT_MULCCONJ_S(w1, t0);
      t1 = XT_MULC_S    (w2, t1);
      
      /* y[k    ]= t0.re */
      /* y[N-k  ]= t0.im */
      re = XT_HIGH_S(t0);
      t32x2 = XT_AE_MOVINT32X2_FROMXTFLOATX2(t0);
      XT_SSIP(re, p0_st, SZ_F32);/* save real part */
      AE_S32_L_IP(t32x2, castxcc(ae_int32,p3_st), -(int)SZ_F32);/* save imag part */
      /* y[N/2-k]= t1.re */
      /* y[N/2+k]= t1.im */
      re = XT_HIGH_S(t1);
      t32x2 = XT_AE_MOVINT32X2_FROMXTFLOATX2(t1);
      XT_SSIP(re, p2_st, -(int)SZ_F32);/* save real part */
      AE_S32_L_IP(t32x2, castxcc(ae_int32,p1_st), SZ_F32);/* save imag part*/
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
#else

int dctf     ( float32_t * restrict y,float32_t * restrict x,dct_handle_t h)
{
    const tdct2_twd *descr=(const tdct2_twd *)h;
    const xtfloat * restrict pX0;
    const xtfloat * restrict pX1;
    const xtfloat * restrict pCosi;
    const xtfloat * restrict pTwd1;
    const xtfloat * restrict pTwd2;
    xtfloat y0_re,y1_re,t0_re,t1_re,cosi_re,w1_re,w2_re,s_re,d_re;
    xtfloat y0_im,y1_im,t0_im,t1_im,cosi_im,w1_im,w2_im,s_im,d_im;
    const ae_int32x2 * restrict p0_ld;
          ae_int32x2 * restrict p0_stx2;
          ae_int32x2 * restrict p1_stx2;

    int N, k, n, twd_stride;
    const complex_float* dct_twd;
    const complex_float *fft_twd;

    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT(x!=y);
    NASSERT(descr->magic==MAGIC_DCT2_F);
    N = descr->N;
    ASSERT(N==32 || N==64);

    /* permute inputs */
    p0_ld  = (const ae_int32x2 *)x;
    p0_stx2 = (ae_int32x2 *)y;
    p1_stx2 = (ae_int32x2 *)(y+N-2);
    __Pragma("loop_count min=1")
    for (n=0; n<(N>>2); n++)
    {
        ae_int32x2 t0,t1,y0,y1;
      /* y[n]    =x[2*n+0] */
      /* y[N-1-n]=x[2*n+1] */
      AE_L32X2_IP(t0, p0_ld, sizeof(ae_int32x2));
      AE_L32X2_IP(t1, p0_ld, sizeof(ae_int32x2));
      y0 = AE_SEL32_HH(t0, t1);
      y1 = AE_SEL32_LL(t1, t0);
      AE_S32X2_IP(y0, p0_stx2,       sizeof(ae_int32x2));
      AE_S32X2_XP(y1, p1_stx2, -(int)sizeof(ae_int32x2));
    }
    /* compute fft(N/2) */
    dct_twd=(const complex_float*)descr->rfft_split_twd;
    fft_twd=(const complex_float *)descr->fft_twd;
    twd_stride=1;
    fft_cplxf_ie((complex_float*)x,(complex_float*)y,fft_twd,twd_stride,N/2);
    /* make final DCT transformation of FFT outputs */
    pX0=(const xtfloat *)(x);
    pX1=(const xtfloat *)(x+N-2);
    pCosi=(const xtfloat *)(dct_twd+4*twd_stride);
    pTwd1=(const xtfloat *)(dct_twd+  twd_stride);
    pTwd2=(const xtfloat *)(dct_twd+(N/2-1)*twd_stride);
    y0_im=XT_LSI(pX0,sizeof(xtfloat)); XT_LSIP(y0_re,pX0, 2*sizeof(xtfloat));
    s_re=XT_ADD_S(y0_re,y0_im);
    s_im=XT_SUB_S(y0_re,y0_im);
    y[0  ]= s_re;
    y[N/2]= XT_MUL_S(s_im,0.707106781f);
    for (k=1; k<(N>>2); k++)
    {
        y0_im=XT_LSI(pX0,sizeof(xtfloat)); XT_LSIP(y0_re,pX0, 2*sizeof(xtfloat));
        y1_im=XT_LSI(pX1,sizeof(xtfloat)); XT_LSXP(y1_re,pX1,-2*(int)sizeof(xtfloat));

        w1_im=XT_LSI(pTwd1,sizeof(xtfloat)); XT_LSXP(w1_re,pTwd1, twd_stride*2*sizeof(xtfloat));
        w2_im=XT_LSI(pTwd2,sizeof(xtfloat)); XT_LSXP(w2_re,pTwd2,-twd_stride*2*sizeof(xtfloat));
        cosi_im=XT_LSI(pCosi,sizeof(xtfloat)); XT_LSXP(cosi_re,pCosi,4*twd_stride*2*sizeof(xtfloat));

        s_re=XT_ADD_S(y0_re,y1_re);
        s_im=XT_ADD_S(y0_im,y1_im);
        d_re=XT_SUB_S(y0_re,y1_re);
        d_im=XT_SUB_S(y0_im,y1_im);

        t0_re=s_im; t0_im=d_re;
        t1_re=s_re; t1_im=d_im;
        y0_re=XT_MUL_S(t0_im,cosi_im);XT_MSUB_S(y0_re,t0_re,cosi_re);
        y0_im=XT_MUL_S(t0_re,cosi_im);XT_MADD_S(y0_im,t0_im,cosi_re);
        t0_re=XT_MUL_S(0.5f,XT_SUB_S(t1_re,y0_re));
        t0_im=XT_MUL_S(0.5f,XT_SUB_S(t1_im,y0_im));
        t1_re=XT_MUL_S(0.5f,XT_ADD_S(t1_re,y0_re));
        t1_im=XT_MUL_S(0.5f,XT_ADD_S(t1_im,y0_im));
        s_re=XT_MUL_S(t0_re,w1_re); XT_MADD_S(s_re,t0_im,w1_im);
        s_im=XT_MUL_S(t0_re,w1_im); XT_MSUB_S(s_im,t0_im,w1_re);
        d_re=XT_MUL_S(t1_re,w2_re); XT_MSUB_S(d_re,t1_im,w2_im);
        d_im=XT_MUL_S(t1_re,w2_im); XT_MADD_S(d_im,t1_im,w2_re);
        y[k         ]= s_re;
        y[(N>>1)+k  ]= d_im;
        y[N-k       ]= s_im;
        y[N-k-(N>>1)]= d_re;
    }
    t0_im=XT_LSI(pX0,sizeof(xtfloat)); XT_LSIP(t0_re,pX0, 2*sizeof(xtfloat));
    w1_im=XT_LSI(pTwd1,sizeof(xtfloat)); XT_LSXP(w1_re,pTwd1, twd_stride*2*sizeof(xtfloat));
    d_re=XT_MUL_S(t0_re,w1_re); XT_MSUB_S(d_re,t0_im,w1_im);
    d_im=XT_MUL_S(t0_re,w1_im); XT_MADD_S(d_im,t0_im,w1_re);
    y[k]  = d_re;
    y[N-k]= d_im;

    return 0;
}
#endif
