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
/*          Copyright (C) 2015-2017 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
    NatureDSP Signal Processing Library. FFT part
    DCT-IV 32x32 with scaling option 3
    C code optimized for HiFi4
    Integrit, 2006-2017
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
/* Common utility and macros declarations. */
#include "common.h"
/* Twiddle factor tables for DCTs. */
#include "dct4_twd.h"

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

#define LOG2_SZ_CI32P 4/* log2(2*sizeof(complex_fract32)) */

static void dct3p(uint64_t *x,uint64_t* y,int N,const tdct4_twd_fr32 *pdct4_twd);
static void ifft_cplx32x32_pair (uint64_t* y,uint64_t* x, const complex_fract32* twd, int N);
static void dct3p_N16(uint64_t *x,uint64_t* y,const tdct4_twd_fr32 *pdct4_twd);

int dct4_32x32( int32_t * y, int32_t * x, dct_handle_t h, int scalingOpt)
{  
          ae_int32x2 * restrict px0;
          ae_int32x2 * restrict px1;
          ae_int32x2 * restrict py0;
          ae_int32x2 * restrict py1;
          ae_int32x2 * restrict py2;
          ae_int32x2 * restrict py3;
    const ae_int32x2 * restrict ptwd0;
    const ae_int32x2 * restrict ptwd1;
    const tdct4_twd_fr32 *ptwd=(const tdct4_twd_fr32 *)h;
    ae_int32x2 X0, X1, Y0, Y1, Y2, Y3;
    ae_int32x2 WV0, WV1, WV2, WV3;
    ae_int32x2 tw0, tw1, tw2, tw3;
    ae_f64 ACC0, ACC1, ACC2, ACC3, ACC4, ACC5, ACC6, ACC7;
    ae_valign al_x0, al_x1, al_y0, al_y1;
    int N,k;

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt == 3);

    N=ptwd->N;

    /*
      Rearrange input data. Even samples are w, odd - v
      N2 = N/2;
      w(1) = x(1);
      v(N2) = x(N);
      for k=1:(N2-1)
        w(k+1) = x(2*k+1) + x(2*k);
        v(k)   = x(2*k)   - x(2*k+1);
      end
    */
    px0 = (ae_int32x2 *)(x);
    px1 = (ae_int32x2 *)(x+N-1);
    py0 = (ae_int32x2 *)(y);
    py1 = (ae_int32x2 *)(y+N-1);
    {
        AE_L32_IP(X0, castxcc(ae_int32,px0),       sizeof(int32_t));
        AE_L32_IP(X1, castxcc(ae_int32,px1), -(int)sizeof(int32_t));
        Y0 = AE_SRAI32(X0, 1);
        Y1 = AE_SRAI32(X1, 1);
        AE_S32_L_IP(Y0, castxcc(ae_int32,py0), sizeof(int32_t));
        AE_S32_L_IP(Y1, castxcc(ae_int32,py0), sizeof(int32_t));

        AE_L32_IP(X1, castxcc(ae_int32,px0), sizeof(int32_t));
        AE_L32_IP(X0, castxcc(ae_int32,px0), sizeof(int32_t));
        X0 = AE_SRAI32(X0, 1);
        X1 = AE_SRAI32(X1, 1);
        Y0 = AE_ADD32(X0, X1);
        Y1 = AE_SUB32(X1, X0);
        AE_S32_L_IP(Y0, castxcc(ae_int32,py0),  sizeof(int32_t));
        AE_S32_L_IP(Y1, castxcc(ae_int32,py1), -(int)sizeof(int32_t));
    }
    al_x0 = AE_LA64_PP(px0);
    al_x1 = AE_LA64_PP(px1);
    al_y0 = AE_ZALIGN64();
    al_y1 = AE_ZALIGN64();
    __Pragma("loop_count min=1");
    for (k=1; k<(N>>2); k++)
    {
        AE_LA32X2_IP (X0, al_x0, px0);
        AE_LA32X2_RIP(X1, al_x1, px1);

        X0 = AE_SRAI32(X0, 1);
        X1 = AE_SRAI32(X1, 1);
        Y0 = AE_SEL32_LL(X1, X0);
        Y1 = AE_SEL32_HH(X1, X0);
        Y2 = AE_SEL32_LL(X1, X0);
        Y3 = AE_SEL32_HH(X1, X0);

        X0 = AE_SUBADD32(Y0, Y1);
        X1 = AE_ADDSUB32(Y3, Y2);

        AE_SA32X2_IP (X0, al_y0, py0);
        AE_SA32X2_RIP(X1, al_y1, py1);
    }
    AE_SA64POS_FP(al_y0, py0);
    AE_SA64NEG_FP(al_y1, py1);

    if (N==32)
    {
        dct3p_N16((uint64_t*)y,(uint64_t*)x,ptwd);
    }
    else
    {
        dct3p((uint64_t*)y,(uint64_t*)x,N>>1,ptwd);
    }

    /* split part */
    ptwd0 = (const ae_int32x2 *)(ptwd->split);
    ptwd1 = (const ae_int32x2 *)(ptwd->split+N/4);
    px0 = (ae_int32x2 *)(x);
    px1 = (ae_int32x2 *)(x+N-2);
    py0 = (ae_int32x2 *)(y      );
    py1 = (ae_int32x2 *)(y+N/2-2);
    py2 = (ae_int32x2 *)(y+N/2  );
    py3 = (ae_int32x2 *)(y+N  -2);
    __Pragma("loop_count min=1");
    for (k=0; k<(N>>3); k++)
    {
        AE_L32X2_IP(WV0, px0,  2*sizeof(int32_t));
        AE_L32X2_XP(WV1, px1, -2*(int)sizeof(int32_t));
        AE_L32X2_IP(WV2, px0,  2*sizeof(int32_t));
        AE_L32X2_XP(WV3, px1, -2*(int)sizeof(int32_t));

        AE_L32X2_IP(tw0, ptwd0, 2*sizeof(int32_t));
        AE_L32X2_IP(tw1, ptwd0, 2*sizeof(int32_t));
        AE_L32X2_IP(tw2, ptwd1, 2*sizeof(int32_t));
        AE_L32X2_IP(tw3, ptwd1, 2*sizeof(int32_t));

        ACC0 = AE_MULF32S_HH(WV0, tw0);
        ACC1 = AE_MULF32S_LH(WV1, tw2);
        ACC2 = AE_MULF32S_HH(WV1, tw2);
        ACC3 = AE_MULF32S_LH(WV0, tw0);
        ACC4 = AE_MULF32S_HH(WV2, tw1);
        ACC5 = AE_MULF32S_LH(WV3, tw3);
        ACC6 = AE_MULF32S_HH(WV3, tw3);
        ACC7 = AE_MULF32S_LH(WV2, tw1);
        AE_MULSF32S_LL(ACC0, WV0, tw0);
        AE_MULAF32S_HL(ACC1, WV1, tw2);
        AE_MULSF32S_LL(ACC2, WV1, tw2);
        AE_MULAF32S_HL(ACC3, WV0, tw0);
        AE_MULAF32S_LL(ACC4, WV2, tw1);
        AE_MULSF32S_HL(ACC5, WV3, tw3);
        AE_MULAF32S_LL(ACC6, WV3, tw3);
        AE_MULSF32S_HL(ACC7, WV2, tw1);
        ACC1 = AE_NEG64S(ACC1);
        ACC3 = AE_NEG64S(ACC3);
        Y0 = AE_ROUND32X2F64SASYM(ACC0, ACC4);
        Y1 = AE_ROUND32X2F64SASYM(ACC5, ACC1);
        Y2 = AE_ROUND32X2F64SASYM(ACC2, ACC6);
        Y3 = AE_ROUND32X2F64SASYM(ACC7, ACC3);

        AE_S32X2_IP(Y0, py0,       2*sizeof(int32_t));
        AE_S32X2_XP(Y1, py1, -2*(int)sizeof(int32_t));
        AE_S32X2_IP(Y2, py2,       2*sizeof(int32_t));
        AE_S32X2_XP(Y3, py3, -2*(int)sizeof(int32_t));
    }
    return 30-NSA(N);
} /* dct4_32x32() */


/*
    DCT-III on pairs of data
    input:
    x[N]
    Output:
    y[N]
*/
static void dct3p(uint64_t *x,uint64_t* y,int N,const tdct4_twd_fr32* pdct4_twd)
{
          ae_int32x2 * restrict px0;
          ae_int32x2 * restrict px1;
          ae_int32x2 * restrict py0;
          ae_int32x2 * restrict py1;
    const ae_int32 * restrict ptwd;
    ae_f32x2 X0, X1, T0, T1, Y0, Y1;
    ae_f32x2 a0re, a0im, b0re, b0im;
    ae_f32x2 a1re, a1im, b1re, b1im;
    ae_f32x2 c, s;
    ae_int32x2 t0;
    int k;

    px0 = (ae_int32x2 *)x;
    py0 = (ae_int32x2 *)y;

    X0 = AE_L32X2_I(px0, 0);
    X1 = AE_L32X2_X(px0, (N/2)*sizeof(ae_f32x2));
    X1 = AE_MULFP32X2RAS(X1, AE_MOVF32X2_FROMINT32X2(AE_MOVDA32(1518500250L)));
    AE_S32X2_IP(X0,py0,sizeof(ae_f32x2));
    AE_S32X2_IP(X1,py0,sizeof(ae_f32x2));

    WUR_AE_SAR(1);
    px0 = (ae_int32x2 *)(x+1);
    px1 = (ae_int32x2 *)(x+N-1);
    ptwd = (const ae_int32 *)(pdct4_twd->dct3+1);
    for (k=1; k<N/2; k++)
    {
        AE_L32_IP(t0, ptwd, sizeof(int32_t)); c = (t0);
        AE_L32_IP(t0, ptwd, sizeof(int32_t)); s = (t0);
        AE_L32X2_IP(X0, px0,       sizeof(ae_f32x2));
        AE_L32X2_XP(X1, px1, -(int)sizeof(ae_f32x2));
        T0 = AE_MULFP32X2RAS(X0, c);
        T1 = AE_MULFP32X2RAS(X1, s);
        Y0 = AE_ADDRNG32(T0, T1);
        T0 = AE_MULFP32X2RAS(X0, s);
        T1 = AE_MULFP32X2RAS(X1, c);
        Y1 = AE_SUBRNG32(T0, T1);
        AE_S32X2_IP(Y0,py0,sizeof(ae_f32x2));
        AE_S32X2_IP(Y1,py0,sizeof(ae_f32x2));
    }
    /* real fft (Nyquist sample is packed to the imaginary part of y[] */
    px0 = (ae_int32x2 *)(x);
    px1 = (ae_int32x2 *)(x+N-1);
    py0 = (ae_int32x2 *)(y);
    py1 = (ae_int32x2 *)(y+N-1);
    ptwd = (const ae_int32 *)(pdct4_twd->rfft);
    {
        AE_L32X2_IP(a0re, py0, sizeof(ae_f32x2));
        AE_L32X2_IP(a1re, py0, sizeof(ae_f32x2));
        AE_ADDANDSUBRNG32(X0, X1, a0re, a1re);
        AE_S32X2_IP(X0,px0,sizeof(ae_f32x2));
        AE_S32X2_IP(X1,px0,sizeof(ae_f32x2));
    }
    for (k=1; k<N/4; k++)
    {
        AE_L32_IP(t0, ptwd, sizeof(int32_t)); c = (t0);
        AE_L32_IP(t0, ptwd, sizeof(int32_t)); s = (t0);
        AE_L32X2_IP(a0re, py0,       sizeof(ae_f32x2));
        AE_L32X2_IP(a0im, py0,       sizeof(ae_f32x2));
        AE_L32X2_XP(a1im, py1, -(int)sizeof(ae_f32x2));
        AE_L32X2_XP(a1re, py1, -(int)sizeof(ae_f32x2));

        AE_ADDANDSUBRNG32(b0re, b1re, a0re, a1re);
        AE_ADDANDSUBRNG32(b1im, b0im, a0im, a1im);

        T0 = AE_MULFP32X2RAS(b1re, s);
        T1 = AE_MULFP32X2RAS(b1im, c);
        a0re = AE_ADD32(T0, T1);
        T0 = AE_MULFP32X2RAS(b1re, c);
        T1 = AE_MULFP32X2RAS(b1im, s);
        a0im = AE_SUB32(T0, T1);
        b1re = a0re;
        b1im = a0im;
        AE_ADDANDSUB32S(a1re, a0re, b0re, b1re);
        AE_ADDANDSUB32S(a0im, a1im, b1im, b0im);

        AE_S32X2_IP(a0re,px0,      sizeof(ae_f32x2));
        AE_S32X2_IP(a0im,px0,      sizeof(ae_f32x2));
        AE_S32X2_XP(a1im,px1,-(int)sizeof(ae_f32x2));
        AE_S32X2_XP(a1re,px1,-(int)sizeof(ae_f32x2));
    }
    {
        Y0 = AE_L32X2_I(py0, 0);
        Y1 = AE_L32X2_I(py1, 0);
        Y1 = AE_NEG32(Y1);
        AE_S32X2_I(Y0,px0, 0);
        AE_S32X2_I(Y1,px1, 0);
    }
    /* IFFT and final permutation */
    ifft_cplx32x32_pair (y,x,pdct4_twd->fft, N/2);
    px0 = (ae_int32x2 *)(x);
    px1 = (ae_int32x2 *)(x+N-1);
    py0 = (ae_int32x2 *)(y);
    py1 = (ae_int32x2 *)(y+1);
    for (k=0; k<N/2; k++)
    {
        AE_L32X2_IP (X0, px0,       sizeof(ae_f32x2));
        AE_L32X2_XP (X1, px1, -(int)sizeof(ae_f32x2));
        AE_S32X2_IP(X0, py0, 2*sizeof(ae_f32x2));
        AE_S32X2_IP(X1, py1, 2*sizeof(ae_f32x2));
    }
}

/*
    paired fft: operates with pairs of data packed in 64-bit words
    input/output:
    x[N]    - data
    Input:
    twd[3/4*N]  - twiddles
    N           - FFT size
    Temporary:
    y[N]
*/
static void ifft_cplx32x32_pair (uint64_t* y,uint64_t* x, const complex_fract32* twd,  int N)
{
          ae_int32x2 * restrict px0;
          ae_int32x2 * restrict px1;
          ae_int32x2 * restrict px2;
          ae_int32x2 * restrict px3;
          ae_int32x2 * restrict py0;
          ae_int32x2 * restrict py1;
          ae_int32x2 * restrict py2;
          ae_int32x2 * restrict py3;
    const ae_int32x2 * restrict ptwd;
    int logN, idx, bitrevstride, stride;
    int m, n;
    int twdstep;

    ae_f32x2 a0_re, a1_re, a2_re, a3_re, b0_re, b1_re, b2_re, b3_re;
    ae_f32x2 a0_im, a1_im, a2_im, a3_im, b0_im, b1_im, b2_im, b3_im;
    ae_f32x2 tw1, tw2, tw3;
    ae_f32x2 tw1re, tw2re, tw3re;
    ae_f32x2 tw1im, tw2im, tw3im;
    ae_int32x2 t0;

    NASSERT( x );
    NASSERT( y );
    NASSERT( twd );
    NASSERT( x != y );
    NASSERT_ALIGN( x, 8 );
    NASSERT_ALIGN( y, 8 );
    NASSERT( N>=8 && 0 == (N&(N-1)) );

    twdstep = 1;
    logN = 30 - NSA( N );

    /*----------------------------------------------------------------------------*
     * Perform the first stage. We use DIF, all permutations are deferred until   *
     * the last stage.                                                            */
    stride = N/4;
    ptwd = (const ae_int32x2 *)(twd);
    px0 = (ae_int32x2 *)(x+0*2*stride);
    px1 = (ae_int32x2 *)(x+1*2*stride);
    px2 = (ae_int32x2 *)(x+2*2*stride);
    px3 = (ae_int32x2 *)(x+3*2*stride);
    py0 = (ae_int32x2 *)(y+0*2*stride);
    py1 = (ae_int32x2 *)(y+1*2*stride);
    py2 = (ae_int32x2 *)(y+2*2*stride);
    py3 = (ae_int32x2 *)(y+3*2*stride);

    for ( n=0; n<stride; n++ )
    {
        AE_L32X2_IP(t0, ptwd, 2*sizeof(int32_t)); tw1 = (t0);
        AE_L32X2_IP(t0, ptwd, 2*sizeof(int32_t)); tw2 = (t0);
        AE_L32X2_IP(t0, ptwd, 2*sizeof(int32_t)); tw3 = (t0);
        tw1re = AE_SEL32_HH(tw1, tw1);
        tw2re = AE_SEL32_HH(tw2, tw2);
        tw3re = AE_SEL32_HH(tw3, tw3);
        tw1im = AE_SEL32_LL(tw1, tw1);
        tw2im = AE_SEL32_LL(tw2, tw2);
        tw3im = AE_SEL32_LL(tw3, tw3);

        /* Real and imaginary parts are swapped on the first and last stages to
         * inverse the FFT:
         * conj(x) == -j*swap(x) =>
         * ifft(x) == conj(fft(conj(x)) == swap(fft(swap(x)))
         * Just in case, divide data by the FFT size. */
        AE_L32X2_IP(a0_im, px0, sizeof(ae_f32x2));
        AE_L32X2_IP(a0_re, px0, sizeof(ae_f32x2));
        AE_L32X2_IP(a1_im, px1, sizeof(ae_f32x2));
        AE_L32X2_IP(a1_re, px1, sizeof(ae_f32x2));
        AE_L32X2_IP(a2_im, px2, sizeof(ae_f32x2));
        AE_L32X2_IP(a2_re, px2, sizeof(ae_f32x2));
        AE_L32X2_IP(a3_im, px3, sizeof(ae_f32x2));
        AE_L32X2_IP(a3_re, px3, sizeof(ae_f32x2));

        AE_ADDANDSUBRNG32(b0_re, b2_re, a0_re, a2_re);
        AE_ADDANDSUBRNG32(b1_re, b3_re, a1_re, a3_re);
        AE_ADDANDSUBRNG32(b0_im, b2_im, a0_im, a2_im);
        AE_ADDANDSUBRNG32(b1_im, b3_im, a1_im, a3_im);

        AE_ADDANDSUBRNG32(a0_re, a2_re, b0_re, b1_re);
        AE_ADDANDSUBRNG32(a0_im, a2_im, b0_im, b1_im);
        AE_ADDANDSUBRNG32(a1_re, a3_re, b2_re, b3_im);
        AE_ADDANDSUBRNG32(a3_im, a1_im, b2_im, b3_re);

        b0_re = a0_re;
        b0_im = a0_im;
        b1_re = AE_MULFP32X2RAS(a1_re, tw1re);  AE_MULSFP32X2RAS(b1_re, a1_im, tw1im);
        b1_im = AE_MULFP32X2RAS(a1_im, tw1re);  AE_MULAFP32X2RAS(b1_im, a1_re, tw1im);
        b2_re = AE_MULFP32X2RAS(a2_re, tw2re);  AE_MULSFP32X2RAS(b2_re, a2_im, tw2im);
        b2_im = AE_MULFP32X2RAS(a2_im, tw2re);  AE_MULAFP32X2RAS(b2_im, a2_re, tw2im);
        b3_re = AE_MULFP32X2RAS(a3_re, tw3re);  AE_MULSFP32X2RAS(b3_re, a3_im, tw3im);
        b3_im = AE_MULFP32X2RAS(a3_im, tw3re);  AE_MULAFP32X2RAS(b3_im, a3_re, tw3im);

        /* Two middle quartiles are swapped on all but the last stage to use the bit reversal
         * permutation instead of the digit reverse. */
        AE_S32X2_IP(b0_re, py0, sizeof(ae_f32x2));
        AE_S32X2_IP(b0_im, py0, sizeof(ae_f32x2));
        AE_S32X2_IP(b2_re, py1, sizeof(ae_f32x2));
        AE_S32X2_IP(b2_im, py1, sizeof(ae_f32x2));
        AE_S32X2_IP(b1_re, py2, sizeof(ae_f32x2));
        AE_S32X2_IP(b1_im, py2, sizeof(ae_f32x2));
        AE_S32X2_IP(b3_re, py3, sizeof(ae_f32x2));
        AE_S32X2_IP(b3_im, py3, sizeof(ae_f32x2));
    }

    /*----------------------------------------------
      Perform second through the next to last stages.*/
    for ( stride/=4; stride>1; stride/=4 )
    {
        twdstep *= 4;

        for ( m=0; m*(4*stride)<N; m++ )
        {
            ptwd = (const ae_int32x2 *)(twd);

            px0 = (ae_int32x2 *)(y+4*m*2*stride+0*2*stride);
            px1 = (ae_int32x2 *)(y+4*m*2*stride+1*2*stride);
            px2 = (ae_int32x2 *)(y+4*m*2*stride+2*2*stride);
            px3 = (ae_int32x2 *)(y+4*m*2*stride+3*2*stride);

            py0 = (ae_int32x2 *)(y+4*m*2*stride+0*2*stride);
            py1 = (ae_int32x2 *)(y+4*m*2*stride+1*2*stride);
            py2 = (ae_int32x2 *)(y+4*m*2*stride+2*2*stride);
            py3 = (ae_int32x2 *)(y+4*m*2*stride+3*2*stride);

            for ( n=0; n<stride; n++ )
            {
                AE_L32X2_IP(t0, ptwd, 2*sizeof(int32_t)); tw1 = (t0);
                AE_L32X2_IP(t0, ptwd, 2*sizeof(int32_t)); tw2 = (t0);
                AE_L32X2_IP(t0, ptwd, 2*sizeof(int32_t)); tw3 = (t0);
                ptwd += twdstep*3-3;
                tw1re = AE_SEL32_HH(tw1, tw1);
                tw2re = AE_SEL32_HH(tw2, tw2);
                tw3re = AE_SEL32_HH(tw3, tw3);
                tw1im = AE_SEL32_LL(tw1, tw1);
                tw2im = AE_SEL32_LL(tw2, tw2);
                tw3im = AE_SEL32_LL(tw3, tw3);


                AE_L32X2_IP(a0_re, px0, sizeof(ae_f32x2));
                AE_L32X2_IP(a0_im, px0, sizeof(ae_f32x2));
                AE_L32X2_IP(a1_re, px1, sizeof(ae_f32x2));
                AE_L32X2_IP(a1_im, px1, sizeof(ae_f32x2));
                AE_L32X2_IP(a2_re, px2, sizeof(ae_f32x2));
                AE_L32X2_IP(a2_im, px2, sizeof(ae_f32x2));
                AE_L32X2_IP(a3_re, px3, sizeof(ae_f32x2));
                AE_L32X2_IP(a3_im, px3, sizeof(ae_f32x2));

                AE_ADDANDSUBRNG32(b0_re, b2_re, a0_re, a2_re);
                AE_ADDANDSUBRNG32(b1_re, b3_re, a1_re, a3_re);
                AE_ADDANDSUBRNG32(b0_im, b2_im, a0_im, a2_im);
                AE_ADDANDSUBRNG32(b1_im, b3_im, a1_im, a3_im);

                AE_ADDANDSUBRNG32(a0_re, a2_re, b0_re, b1_re);
                AE_ADDANDSUBRNG32(a0_im, a2_im, b0_im, b1_im);
                AE_ADDANDSUBRNG32(a1_re, a3_re, b2_re, b3_im);
                AE_ADDANDSUBRNG32(a3_im, a1_im, b2_im, b3_re);

                b0_re = a0_re;
                b0_im = a0_im;
                b1_re = AE_MULFP32X2RAS(a1_re, tw1re);  AE_MULSFP32X2RAS(b1_re, a1_im, tw1im);
                b1_im = AE_MULFP32X2RAS(a1_im, tw1re);  AE_MULAFP32X2RAS(b1_im, a1_re, tw1im);
                b2_re = AE_MULFP32X2RAS(a2_re, tw2re);  AE_MULSFP32X2RAS(b2_re, a2_im, tw2im);
                b2_im = AE_MULFP32X2RAS(a2_im, tw2re);  AE_MULAFP32X2RAS(b2_im, a2_re, tw2im);
                b3_re = AE_MULFP32X2RAS(a3_re, tw3re);  AE_MULSFP32X2RAS(b3_re, a3_im, tw3im);
                b3_im = AE_MULFP32X2RAS(a3_im, tw3re);  AE_MULAFP32X2RAS(b3_im, a3_re, tw3im);

                /* Two middle quartiles are swapped on all but the last stage to use the bit reversal
                 * permutation instead of the digit reverse. */
                AE_S32X2_IP(b0_re, py0, sizeof(ae_f32x2));
                AE_S32X2_IP(b0_im, py0, sizeof(ae_f32x2));
                AE_S32X2_IP(b2_re, py1, sizeof(ae_f32x2));
                AE_S32X2_IP(b2_im, py1, sizeof(ae_f32x2));
                AE_S32X2_IP(b1_re, py2, sizeof(ae_f32x2));
                AE_S32X2_IP(b1_im, py2, sizeof(ae_f32x2));
                AE_S32X2_IP(b3_re, py3, sizeof(ae_f32x2));
                AE_S32X2_IP(b3_im, py3, sizeof(ae_f32x2));
            }
        }
    }

    /*----------------------------------------------------------------------------
    Last stage (radix-4 or radix-2 for odd powers of two) with bit reversal
    permutation. */
    idx = 0;
    bitrevstride = 0x80000000U >> (logN-3+LOG2_SZ_CI32P);

    if ( stride == 1 )
    {
        px0 = (ae_int32x2 *)(y+2*0);
        px1 = (ae_int32x2 *)(y+2*1);
        px2 = (ae_int32x2 *)(y+2*2);
        px3 = (ae_int32x2 *)(y+2*3);
        py0 = (ae_int32x2 *)(x+2*0*N/4);
        py1 = (ae_int32x2 *)(x+2*1*N/4);
        py2 = (ae_int32x2 *)(x+2*2*N/4);
        py3 = (ae_int32x2 *)(x+2*3*N/4);
        for ( n=0; n<N/4; n++ )
        {
            AE_L32X2_IP(a0_re, px0,   sizeof(ae_f32x2));
            AE_L32X2_IP(a0_im, px0, 7*sizeof(ae_f32x2));
            AE_L32X2_IP(a1_re, px1,   sizeof(ae_f32x2));
            AE_L32X2_IP(a1_im, px1, 7*sizeof(ae_f32x2));
            AE_L32X2_IP(a2_re, px2,   sizeof(ae_f32x2));
            AE_L32X2_IP(a2_im, px2, 7*sizeof(ae_f32x2));
            AE_L32X2_IP(a3_re, px3,   sizeof(ae_f32x2));
            AE_L32X2_IP(a3_im, px3, 7*sizeof(ae_f32x2));

            AE_ADDANDSUBRNG32(b0_re, b2_re, a0_re, a2_re);
            AE_ADDANDSUBRNG32(b1_re, b3_re, a1_re, a3_re);
            AE_ADDANDSUBRNG32(b0_im, b2_im, a0_im, a2_im);
            AE_ADDANDSUBRNG32(b1_im, b3_im, a1_im, a3_im);

            AE_ADDANDSUBRNG32(a0_re, a2_re, b0_re, b1_re);
            AE_ADDANDSUBRNG32(a0_im, a2_im, b0_im, b1_im);
            AE_ADDANDSUBRNG32(a1_re, a3_re, b2_re, b3_im);
            AE_ADDANDSUBRNG32(a3_im, a1_im, b2_im, b3_re);

            /* Real and imaginary parts are swapped on the first and last stages to
             * inverse the FFT:
             * conj(x) == -j*swap(x) =>
             * ifft(x) == conj(fft(conj(x)) == swap(fft(swap(x))) */
            AE_S32X2_X(a0_im, py0, idx+0*sizeof(ae_f32x2));
            AE_S32X2_X(a0_re, py0, idx+1*sizeof(ae_f32x2));
            AE_S32X2_X(a1_im, py1, idx+0*sizeof(ae_f32x2));
            AE_S32X2_X(a1_re, py1, idx+1*sizeof(ae_f32x2));
            AE_S32X2_X(a2_im, py2, idx+0*sizeof(ae_f32x2));
            AE_S32X2_X(a2_re, py2, idx+1*sizeof(ae_f32x2));
            AE_S32X2_X(a3_im, py3, idx+0*sizeof(ae_f32x2));
            AE_S32X2_X(a3_re, py3, idx+1*sizeof(ae_f32x2));

            idx = AE_ADDBRBA32(idx, bitrevstride);
        }
    }
    else
    {
        bitrevstride >>= 1;

        px0 = (ae_int32x2 *)(y+0);
        px1 = (ae_int32x2 *)(y+1);
        px2 = (ae_int32x2 *)(y+2);
        px3 = (ae_int32x2 *)(y+3);
        py0 = (ae_int32x2 *)(x+0);
        py1 = (ae_int32x2 *)(x+1);
        py2 = (ae_int32x2 *)(x+0+N);
        py3 = (ae_int32x2 *)(x+1+N);
        for ( n=0; n<N/2; n++ )
        {
            AE_L32X2_IP(a0_re, px0, 4*sizeof(ae_f32x2));
            AE_L32X2_IP(a0_im, px1, 4*sizeof(ae_f32x2));
            AE_L32X2_IP(a1_re, px2, 4*sizeof(ae_f32x2));
            AE_L32X2_IP(a1_im, px3, 4*sizeof(ae_f32x2));

            AE_ADDANDSUBRNG32(b0_re, b1_re, a0_re, a1_re);
            AE_ADDANDSUBRNG32(b0_im, b1_im, a0_im, a1_im);

            /* Real and imaginary parts are swapped on the first and last stages to
             * inverse the FFT:
             * conj(x) == -j*swap(x) =>
             * ifft(x) == conj(fft(conj(x)) == swap(fft(swap(x))) */
            AE_S32X2_X(b0_im, py0, idx);
            AE_S32X2_X(b0_re, py1, idx);
            AE_S32X2_X(b1_im, py2, idx);
            AE_S32X2_X(b1_re, py3, idx);

            idx = AE_ADDBRBA32(idx, bitrevstride);
        }
    }
} /* ifft_cplx32x32_pair() */

/*
    DCT-III on pairs of data, N==16
    input:
    x[16]
    Output:
    y[16]
*/
static void dct3p_N16(uint64_t *x,uint64_t* y,const tdct4_twd_fr32* pdct4_twd)
{
    const int N = 16;
          ae_int32x2   * restrict px0;
          ae_int32x2   * restrict px1;
          ae_int32x2   * restrict px2;
          ae_int32x2   * restrict px3;
          ae_int32x2   * restrict py0;
          ae_int32x2   * restrict py1;
          ae_int32x2   * restrict py2;
          ae_int32x2   * restrict py3;
    const ae_int32x2 * restrict ptwd;
    ae_f32x2 X0, X1, T0, T1, Y0, Y1;
    ae_f32x2 a0re, a0im, b0re, b0im;
    ae_f32x2 a1re, a1im, b1re, b1im;
    ae_f32x2 a2re, a2im, b2re, b2im;
    ae_f32x2 a3re, a3im, b3re, b3im;
    ae_f32x2 c, s;
    ae_int32x2 t0;
    int k;

    WUR_AE_SAR(1);
    px0 = (ae_int32x2 *)(x);
    px1 = (ae_int32x2 *)(x+N-1);
    py0 = (ae_int32x2 *)(y);
    ptwd = (const ae_int32x2 *)(pdct4_twd->dct3+1);

    {
        X1 = AE_L32X2_X(px0, (N/2)*sizeof(ae_f32x2));
        AE_L32X2_IP(X0, px0, sizeof(ae_f32x2));
        X1 = AE_MULFP32X2RAS(X1, AE_MOVF32X2_FROMINT32(AE_MOVDA32(1518500250L)));
        AE_S32X2_IP(X0,py0,sizeof(ae_f32x2));
        AE_S32X2_IP(X1,py0,sizeof(ae_f32x2));
    }
    for (k=1; k<N/2; k++)
    {
        AE_L32X2_IP(t0, ptwd, 2*sizeof(int32_t));
        c = AE_SEL32_HH(t0, t0);
        s = AE_SEL32_LL(t0, t0);
        AE_L32X2_IP(X0, px0,       sizeof(ae_f32x2));
        AE_L32X2_XP(X1, px1, -(int)sizeof(ae_f32x2));
        T0 = AE_MULFP32X2RAS(X0, c);
        T1 = AE_MULFP32X2RAS(X1, s);
        Y0 = AE_ADDRNG32(T0, T1);
        T0 = AE_MULFP32X2RAS(X0, s);
        T1 = AE_MULFP32X2RAS(X1, c);
        Y1 = AE_SUBRNG32(T0, T1);
        AE_S32X2_IP(Y0,py0,sizeof(ae_f32x2));
        AE_S32X2_IP(Y1,py0,sizeof(ae_f32x2));
    }
    __Pragma("no_reorder");
    /* real fft (Nyquist sample is packed to the imaginary part of y[] */
    px0 = (ae_int32x2 *)(x);
    px1 = (ae_int32x2 *)(x+N-1);
    py0 = (ae_int32x2 *)(y);
    py1 = (ae_int32x2 *)(y+N-1);
    ptwd = (const ae_int32x2 *)(pdct4_twd->rfft);
    {
        AE_L32X2_IP(a0re, py0, sizeof(ae_f32x2));
        AE_L32X2_IP(a1re, py0, sizeof(ae_f32x2));
        AE_ADDANDSUBRNG32(X0, X1, a0re, a1re);
        AE_S32X2_IP(X0,px0,sizeof(ae_f32x2));
        AE_S32X2_IP(X1,px0,sizeof(ae_f32x2));
    }
    for (k=1; k<N/4; k++)
    {
        AE_L32X2_IP(t0, ptwd, 2*sizeof(int32_t));
        c = AE_SEL32_HH(t0, t0);
        s = AE_SEL32_LL(t0, t0);
        AE_L32X2_IP(a0re, py0,       sizeof(ae_f32x2));
        AE_L32X2_IP(a0im, py0,       sizeof(ae_f32x2));
        AE_L32X2_XP(a1im, py1, -(int)sizeof(ae_f32x2));
        AE_L32X2_XP(a1re, py1, -(int)sizeof(ae_f32x2));

        AE_ADDANDSUBRNG32(b0re, b1re, a0re, a1re);
        AE_ADDANDSUBRNG32(b1im, b0im, a0im, a1im);

        a0re = AE_MULFP32X2RAS(b1re, s);
        AE_MULAFP32X2RAS(a0re, b1im, c);
        a0im = AE_MULFP32X2RAS(b1re, c);
        AE_MULSFP32X2RAS(a0im, b1im, s);
        b1re = a0re;
        b1im = a0im;
        AE_ADDANDSUB32S(a1re, a0re, b0re, b1re);
        AE_ADDANDSUB32S(a0im, a1im, b1im, b0im);

        AE_S32X2_IP(a0re,px0,      sizeof(ae_f32x2));
        AE_S32X2_IP(a0im,px0,      sizeof(ae_f32x2));
        AE_S32X2_XP(a1im,px1,-(int)sizeof(ae_f32x2));
        AE_S32X2_XP(a1re,px1,-(int)sizeof(ae_f32x2));
    }
    {
        Y0 = AE_L32X2_I(py0, 0);
        Y1 = AE_L32X2_I(py1, 0);
        Y1 = AE_NEG32(Y1);
        AE_S32X2_I(Y0,px0, 0);
        AE_S32X2_I(Y1,px1, 0);
    }
    /* IFFT (on pairs of data, N=8) and final permutation */
    __Pragma("no_reorder");
    {
        int stride;
        int n;

        ae_f32x2 tw1, tw2, tw3;
        ae_f32x2 tw1re, tw2re, tw3re;
        ae_f32x2 tw1im, tw2im, tw3im;

        /*----------------------------------------------------------------------------*
         * Perform the first stage. We use DIF, all permutations are deferred until   *
         * the last stage.                                                            */
        stride = 8/4;
        ptwd = (const ae_int32x2 *)(pdct4_twd->fft);
        px0 = (ae_int32x2 *)(x+0*2*stride);
        px1 = (ae_int32x2 *)(x+1*2*stride);
        px2 = (ae_int32x2 *)(x+2*2*stride);
        px3 = (ae_int32x2 *)(x+3*2*stride);
        py0 = (ae_int32x2 *)(x+0*2*stride);
        py1 = (ae_int32x2 *)(x+1*2*stride);
        py2 = (ae_int32x2 *)(x+2*2*stride);
        py3 = (ae_int32x2 *)(x+3*2*stride);

        for ( n=0; n<stride; n++ )
        {
            AE_L32X2_IP(t0, ptwd, 2*sizeof(int32_t)); tw1 = (t0);
            AE_L32X2_IP(t0, ptwd, 2*sizeof(int32_t)); tw2 = (t0);
            AE_L32X2_IP(t0, ptwd, 2*sizeof(int32_t)); tw3 = (t0);
            tw1re = AE_SEL32_HH(tw1, tw1);
            tw2re = AE_SEL32_HH(tw2, tw2);
            tw3re = AE_SEL32_HH(tw3, tw3);
            tw1im = AE_SEL32_LL(tw1, tw1);
            tw2im = AE_SEL32_LL(tw2, tw2);
            tw3im = AE_SEL32_LL(tw3, tw3);

            /* Real and imaginary parts are swapped on the first and last stages to
             * inverse the FFT:
             * conj(x) == -j*swap(x) =>
             * ifft(x) == conj(fft(conj(x)) == swap(fft(swap(x)))
             * Just in case, divide data by the FFT size. */
            AE_L32X2_IP(a0im, px0, sizeof(ae_f32x2));
            AE_L32X2_IP(a0re, px0, sizeof(ae_f32x2));
            AE_L32X2_IP(a1im, px1, sizeof(ae_f32x2));
            AE_L32X2_IP(a1re, px1, sizeof(ae_f32x2));
            AE_L32X2_IP(a2im, px2, sizeof(ae_f32x2));
            AE_L32X2_IP(a2re, px2, sizeof(ae_f32x2));
            AE_L32X2_IP(a3im, px3, sizeof(ae_f32x2));
            AE_L32X2_IP(a3re, px3, sizeof(ae_f32x2));

            AE_ADDANDSUBRNG32(b0re, b2re, a0re, a2re);
            AE_ADDANDSUBRNG32(b1re, b3re, a1re, a3re);
            AE_ADDANDSUBRNG32(b0im, b2im, a0im, a2im);
            AE_ADDANDSUBRNG32(b1im, b3im, a1im, a3im);
                                                              
            AE_ADDANDSUBRNG32(a0re, a2re, b0re, b1re);
            AE_ADDANDSUBRNG32(a0im, a2im, b0im, b1im);
            AE_ADDANDSUBRNG32(a1re, a3re, b2re, b3im);
            AE_ADDANDSUBRNG32(a3im, a1im, b2im, b3re);

            b0re = a0re;
            b0im = a0im;
            b1re = AE_MULFP32X2RAS(a1re, tw1re);  AE_MULSFP32X2RAS(b1re, a1im, tw1im);
            b1im = AE_MULFP32X2RAS(a1im, tw1re);  AE_MULAFP32X2RAS(b1im, a1re, tw1im);
            b2re = AE_MULFP32X2RAS(a2re, tw2re);  AE_MULSFP32X2RAS(b2re, a2im, tw2im);
            b2im = AE_MULFP32X2RAS(a2im, tw2re);  AE_MULAFP32X2RAS(b2im, a2re, tw2im);
            b3re = AE_MULFP32X2RAS(a3re, tw3re);  AE_MULSFP32X2RAS(b3re, a3im, tw3im);
            b3im = AE_MULFP32X2RAS(a3im, tw3re);  AE_MULAFP32X2RAS(b3im, a3re, tw3im);

            /* Two middle quartiles are swapped on all but the last stage to use the bit reversal
             * permutation instead of the digit reverse. */
            AE_S32X2_IP(b0re, py0, sizeof(ae_f32x2));
            AE_S32X2_IP(b0im, py0, sizeof(ae_f32x2));
            AE_S32X2_IP(b2re, py1, sizeof(ae_f32x2));
            AE_S32X2_IP(b2im, py1, sizeof(ae_f32x2));
            AE_S32X2_IP(b1re, py2, sizeof(ae_f32x2));
            AE_S32X2_IP(b1im, py2, sizeof(ae_f32x2));
            AE_S32X2_IP(b3re, py3, sizeof(ae_f32x2));
            AE_S32X2_IP(b3im, py3, sizeof(ae_f32x2));
        }

        __Pragma("no_reorder");
        /*-------------------------------------------------------------------
          Last stage (radix-2) with bit reversal permutation.
          Final DCT-3 algorithm permutation is combined with FFT permutation.
        -------------------------------------------------------------------*/
        {
            px0 = (ae_int32x2 *)(x+0);
            px1 = (ae_int32x2 *)(x+1);
            px2 = (ae_int32x2 *)(x+2);
            px3 = (ae_int32x2 *)(x+3);
            py0 = (ae_int32x2 *)(y+0);
            py1 = (ae_int32x2 *)(y+1);

            /* Real and imaginary parts are swapped on the first and last stages to
             * inverse the FFT:
             * conj(x) == -j*swap(x) =>
             * ifft(x) == conj(fft(conj(x)) == swap(fft(swap(x))) */
            {
                AE_L32X2_IP(a0re, px0, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a0im, px1, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a1re, px2, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a1im, px3, 4*sizeof(ae_f32x2));

                AE_ADDANDSUBRNG32(b0re, b1re, a0re, a1re);
                AE_ADDANDSUBRNG32(b0im, b1im, a0im, a1im);

                AE_S32X2_I(b0im, py0, 0*2*sizeof(ae_f32x2));
                AE_S32X2_I(b0re, py0, 1*2*sizeof(ae_f32x2));
                AE_S32X2_X(b1im, py1, 7*2*sizeof(ae_f32x2));
                AE_S32X2_X(b1re, py1, 6*2*sizeof(ae_f32x2));
            }
            {
                AE_L32X2_IP(a0re, px0, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a0im, px1, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a1re, px2, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a1im, px3, 4*sizeof(ae_f32x2));

                AE_ADDANDSUBRNG32(b0re, b1re, a0re, a1re);
                AE_ADDANDSUBRNG32(b0im, b1im, a0im, a1im);

                AE_S32X2_X(b0im, py0, 4*2*sizeof(ae_f32x2));
                AE_S32X2_X(b0re, py0, 5*2*sizeof(ae_f32x2));
                AE_S32X2_I(b1im, py1, 3*2*sizeof(ae_f32x2));
                AE_S32X2_I(b1re, py1, 2*2*sizeof(ae_f32x2));
            }
            {
                AE_L32X2_IP(a0re, px0, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a0im, px1, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a1re, px2, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a1im, px3, 4*sizeof(ae_f32x2));

                AE_ADDANDSUBRNG32(b0re, b1re, a0re, a1re);
                AE_ADDANDSUBRNG32(b0im, b1im, a0im, a1im);

                AE_S32X2_I(b0im, py0, 2*2*sizeof(ae_f32x2));
                AE_S32X2_I(b0re, py0, 3*2*sizeof(ae_f32x2));
                AE_S32X2_X(b1im, py1, 5*2*sizeof(ae_f32x2));
                AE_S32X2_X(b1re, py1, 4*2*sizeof(ae_f32x2));
            }
            {
                AE_L32X2_IP(a0re, px0, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a0im, px1, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a1re, px2, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a1im, px3, 4*sizeof(ae_f32x2));

                AE_ADDANDSUBRNG32(b0re, b1re, a0re, a1re);
                AE_ADDANDSUBRNG32(b0im, b1im, a0im, a1im);

                AE_S32X2_X(b0im, py0, 6*2*sizeof(ae_f32x2));
                AE_S32X2_X(b0re, py0, 7*2*sizeof(ae_f32x2));
                AE_S32X2_I(b1im, py1, 1*2*sizeof(ae_f32x2));
                AE_S32X2_I(b1re, py1, 0*2*sizeof(ae_f32x2));
            }
        }
    } /* ifft_cplx32x32_pair() */
}
