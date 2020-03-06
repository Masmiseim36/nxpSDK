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
    DCT-IV 32x16 with scaling option 3
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

static void dct3p(uint64_t *x,uint64_t* y,int N,const tdct4_twd_fr16 *pdct4_twd);
static void ifft_cplx32x16_pair (uint64_t* y,uint64_t* x, const complex_fract16* twd, int N);
static void dct3p_N16(uint64_t *x,uint64_t* y,const tdct4_twd_fr16 *pdct4_twd);

int dct4_32x16( int32_t * y, int32_t * x, dct_handle_t h, int scalingOpt)
{  
          ae_int32x2 * restrict px0;
          ae_int32x2 * restrict px1;
          ae_int32x2 * restrict py0;
          ae_int32x2 * restrict py1;
          ae_int32x2 * restrict py2;
          ae_int32x2 * restrict py3;
    const ae_int32   * restrict ptwd0;
    const ae_int32   * restrict ptwd1;
    const tdct4_twd_fr16 *ptwd=(const tdct4_twd_fr16 *)h;
    ae_int32x2 X0, X1, Y0, Y1, Y2, Y3;
    ae_int32x2 WV0, WV1, WV2, WV3;
    ae_int16x4 t0_16, t1_16, tw0, tw1, tw2, tw3;
    ae_f64 ACC0, ACC1, ACC2, ACC3, ACC4, ACC5, ACC6, ACC7;
    ae_valign al_x0, al_x1, al_y0, al_y1;
    int N,k;

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt == 3);

    N=ptwd->N;
    WUR_AE_SAR(1);

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
        AE_ADDANDSUBRNG32(Y0, Y1, X1, X0);
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

        X0 = AE_SUBADD32(Y0, Y1);
        X1 = AE_ADDSUB32(Y1, Y0);

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
    ptwd0 = (const ae_int32 *)(ptwd->split);
    ptwd1 = (const ae_int32 *)(ptwd->split+N/4);
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

        AE_L16X4_IP(t0_16, castxcc(ae_int16x4,ptwd0), 4*sizeof(int16_t));
        AE_L16X4_IP(t1_16, castxcc(ae_int16x4,ptwd1), 4*sizeof(int16_t));
        tw0 = t0_16;
        tw1 = AE_SHORTSWAP(t0_16);
        tw2 = t1_16;
        tw3 = AE_SHORTSWAP(t1_16);

        ACC0 = AE_MULZASFD32X16_H3_L2(WV0, tw0);
        ACC1 = AE_MULZSSFD32X16_H1_L0(WV1, tw3);
        ACC2 = AE_MULZASFD32X16_H3_L2(WV1, tw2);
        ACC3 = AE_MULZSSFD32X16_H1_L0(WV0, tw1);
        ACC4 = AE_MULZAAFD32X16_H1_L0(WV2, tw0);
        ACC5 = AE_MULZSAFD32X16_H3_L2(WV3, tw3);
        ACC6 = AE_MULZAAFD32X16_H1_L0(WV3, tw2);
        ACC7 = AE_MULZSAFD32X16_H3_L2(WV2, tw1);
        Y0 = AE_ROUND32X2F48SASYM(ACC0, ACC4);
        Y1 = AE_ROUND32X2F48SASYM(ACC5, ACC1);
        Y2 = AE_ROUND32X2F48SASYM(ACC2, ACC6);
        Y3 = AE_ROUND32X2F48SASYM(ACC7, ACC3);

        AE_S32X2_IP(Y0, py0,       2*sizeof(int32_t));
        AE_S32X2_XP(Y1, py1, -2*(int)sizeof(int32_t));
        AE_S32X2_IP(Y2, py2,       2*sizeof(int32_t));
        AE_S32X2_XP(Y3, py3, -2*(int)sizeof(int32_t));
    }
    return 30-NSA(N);
} /* dct4_32x16() */

/*
    DCT-III on pairs of data
    input:
    x[N]
    Output:
    y[N]
*/
static void dct3p(uint64_t *x,uint64_t* y,int N,const tdct4_twd_fr16* pdct4_twd)
{
          ae_int32x2 * restrict px0;
          ae_int32x2 * restrict px1;
          ae_int32x2 * restrict py0;
          ae_int32x2 * restrict py1;
    const ae_int32 * restrict ptwd;
    ae_f32x2 X0, X1, T0, T1, Y0, Y1;
    ae_f32x2 a0re, a0im, b0re, b0im;
    ae_f32x2 a1re, a1im, b1re, b1im;
    ae_f16x4 cs;
    ae_int32x2 t32;
    ae_int16x4 t0;
    int k;

    px0 = (ae_int32x2 *)x;
    py0 = (ae_int32x2 *)y;
    WUR_AE_SAR(1);

    X0 = AE_L32X2_I(px0, 0);
    X1 = AE_L32X2_X(px0, (N/2)*sizeof(ae_f32x2));
    X1 = AE_MULFP32X16X2RAS_L(X1, AE_MOVDA16(23170));
    Y0 = AE_SEL32_HH(X0, X1);
    Y1 = AE_SEL32_LL(X0, X1);
    AE_S32X2_IP(Y0,py0,sizeof(ae_f32x2));
    AE_S32X2_IP(Y1,py0,sizeof(ae_f32x2));

    px0 = (ae_int32x2 *)(x+1);
    px1 = (ae_int32x2 *)(x+N-1);
    ptwd = (const ae_int32 *)(pdct4_twd->dct3+1);
    __Pragma("loop_count min=1");
    for (k=1; k<(N>>1); k++)
    {
        AE_L32_IP(t32, ptwd, 2*sizeof(int16_t)); t0 = AE_MOVINT16X4_FROMINT32X2(t32);
        cs = t0;
        AE_L32X2_IP(X0, px0,       sizeof(ae_f32x2));
        AE_L32X2_XP(X1, px1, -(int)sizeof(ae_f32x2));

        T0 = AE_F32X2_SRAI(X0, 1);
        T1 = AE_F32X2_SRAI(X1, 1);
        X0 = AE_SEL32_HH(T0, T1);
        X1 = AE_SEL32_LL(T0, T1);
        Y0 = AE_MULFC32X16RAS_L(X0, cs);
        Y1 = AE_MULFC32X16RAS_L(X1, cs);

        AE_S32X2_IP(Y0,py0,sizeof(ae_f32x2));
        AE_S32X2_IP(Y1,py0,sizeof(ae_f32x2));
    }
    /* real fft (Nyquist sample is packed to the imaginary part of y[]) */
    px0 = (ae_int32x2 *)(x);
    px1 = (ae_int32x2 *)(x+N-1);
    py0 = (ae_int32x2 *)(y);
    py1 = (ae_int32x2 *)(y+N-1);
    ptwd = (const ae_int32 *)(pdct4_twd->rfft);
    {
        AE_L32X2_IP(a0re, py0, sizeof(ae_f32x2));
        AE_L32X2_IP(a1re, py0, sizeof(ae_f32x2));
        a0re = AE_F32X2_SRAI(a0re, 1);
        a1re = AE_F32X2_SRAI(a1re, 1);
        X0 = AE_SEL32_HH(a0re, a0re);
        Y0 = AE_SEL32_LL(a0re, a0re);
        X1 = AE_SEL32_HH(a1re, a1re);
        Y1 = AE_SEL32_LL(a1re, a1re);
        X0 = AE_SUBADD32(X0, Y0);
        X1 = AE_SUBADD32(X1, Y1);
        AE_S32X2_IP(X0,px0,sizeof(ae_f32x2));
        AE_S32X2_IP(X1,px0,sizeof(ae_f32x2));
    }
    __Pragma("loop_count min=1");
    for (k=1; k<(N>>2); k++)
    {
        AE_L32_IP(t32, ptwd, 2*sizeof(int16_t)); t0 = AE_MOVINT16X4_FROMINT32X2(t32);
        cs = AE_SHORTSWAP(t0);
        AE_L32X2_IP(a0re, py0,       sizeof(ae_f32x2));
        AE_L32X2_IP(a0im, py0,       sizeof(ae_f32x2));
        AE_L32X2_XP(a1im, py1, -(int)sizeof(ae_f32x2));
        AE_L32X2_XP(a1re, py1, -(int)sizeof(ae_f32x2));

        AE_ADDANDSUBRNG32(Y0, Y1, a0re, a1re);
        AE_ADDANDSUBRNG32(X0, X1, a0im, a1im);
        b0re = AE_SEL32_HL(Y1, Y0);
        b1re = AE_SEL32_LH(Y1, Y0);
        b0im = AE_SEL32_HL(X1, X0);
        b1im = AE_SEL32_LH(X1, X0);

        a0re = AE_MULFC32X16RAS_L(b1re, cs);
        a0im = AE_MULFC32X16RAS_L(b1im, cs);

        X0 = b0re;
        X1 = b0im;
        Y0 = a0re;
        Y1 = a0im;

        a0re = AE_ADDSUB32(X0, Y0);
        a0im = AE_ADDSUB32(X1, Y1);
        a1re = AE_SUBADD32(Y0, X0);
        a1im = AE_SUBADD32(Y1, X1);

        AE_S32X2_IP(a0re,px0,      sizeof(ae_f32x2));
        AE_S32X2_IP(a0im,px0,      sizeof(ae_f32x2));
        AE_S32X2_XP(a1im,px1,-(int)sizeof(ae_f32x2));
        AE_S32X2_XP(a1re,px1,-(int)sizeof(ae_f32x2));
    }
    {
        X0 = AE_L32X2_I(py0, 0);
        X1 = AE_L32X2_I(py1, 0);
        T0 = AE_SEL32_HH(X0, X1);
        T0 = AE_NEG32(T0);
        Y0 = AE_SEL32_HL(T0, X0);
        Y1 = AE_SEL32_LL(T0, X1);
        AE_S32X2_I(Y0,px0, 0);
        AE_S32X2_I(Y1,px1, 0);
    }
    /* IFFT and final permutation */
    ifft_cplx32x16_pair (y,x,pdct4_twd->fft, N/2);
    px0 = (ae_int32x2 *)(x);
    px1 = (ae_int32x2 *)(x+N-1);
    py0 = (ae_int32x2 *)(y);
    py1 = (ae_int32x2 *)(y+1);
    __Pragma("loop_count min=1");
    for (k=0; k<(N>>2); k++)
    {
        AE_L32X2_IP (X0, px0, sizeof(ae_f32x2));
        AE_L32X2_IP (X1, px0, sizeof(ae_f32x2));
        Y0 = AE_SEL32_LL(X0, X1);
        Y1 = AE_SEL32_HH(X0, X1);
        AE_S32X2_IP(Y0, py0, 2*sizeof(ae_f32x2));
        AE_S32X2_IP(Y1, py0, 2*sizeof(ae_f32x2));

        AE_L32X2_XP (X0, px1, -(int)sizeof(ae_f32x2));
        AE_L32X2_XP (X1, px1, -(int)sizeof(ae_f32x2));
        Y0 = AE_SEL32_HH(X1, X0);
        Y1 = AE_SEL32_LL(X1, X0);
        AE_S32X2_IP(Y0, py1, 2*sizeof(ae_f32x2));
        AE_S32X2_IP(Y1, py1, 2*sizeof(ae_f32x2));
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
static void ifft_cplx32x16_pair (uint64_t* y,uint64_t* x, const complex_fract16* twd,  int N)
{
          ae_int32x2 * restrict px0;
          ae_int32x2 * restrict px1;
          ae_int32x2 * restrict px2;
          ae_int32x2 * restrict px3;
          ae_int32x2 * restrict py0;
          ae_int32x2 * restrict py1;
          ae_int32x2 * restrict py2;
          ae_int32x2 * restrict py3;
    const ae_int32 * restrict ptwd;
    int logN, idx, bitrevstride, stride;
    int m, n;
    int twdstep;

    ae_f32x2 a00, a01, a10, a11, a20, a21, a30, a31;
    ae_f32x2 b00, b01, b10, b11, b20, b21, b30, b31;
    ae_f16x4 tw1, tw2, tw3;
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
    WUR_AE_SAR(1);

    /*----------------------------------------------------------------------------*
     * Perform the first stage. We use DIF, all permutations are deferred until   *
     * the last stage.                                                            */
    stride = N/4;
    ptwd = (const ae_int32 *)(twd);
    px0 = (ae_int32x2 *)(x+0*2*stride);
    px1 = (ae_int32x2 *)(x+1*2*stride);
    px2 = (ae_int32x2 *)(x+2*2*stride);
    px3 = (ae_int32x2 *)(x+3*2*stride);
    py0 = (ae_int32x2 *)(y+0*2*stride);
    py1 = (ae_int32x2 *)(y+1*2*stride);
    py2 = (ae_int32x2 *)(y+2*2*stride);
    py3 = (ae_int32x2 *)(y+3*2*stride);

    __Pragma("loop_count min=1");
    for ( n=0; n<stride; n++ )
    {
        AE_L32_IP(t0, ptwd, 2*sizeof(int16_t)); tw1 = AE_MOVF16X4_FROMINT32X2(t0);
        AE_L32_IP(t0, ptwd, 2*sizeof(int16_t)); tw2 = AE_MOVF16X4_FROMINT32X2(t0);
        AE_L32_IP(t0, ptwd, 2*sizeof(int16_t)); tw3 = AE_MOVF16X4_FROMINT32X2(t0);
        tw1 = AE_SHORTSWAP(tw1);
        tw2 = AE_SHORTSWAP(tw2);
        tw3 = AE_SHORTSWAP(tw3);

        /* Real and imaginary parts are swapped on the first and last stages to
         * inverse the FFT:
         * conj(x) == -j*swap(x) =>
         * ifft(x) == conj(fft(conj(x)) == swap(fft(swap(x)))
         * Just in case, divide data by the FFT size. */
        AE_L32X2_IP(a01, px0, sizeof(ae_f32x2));
        AE_L32X2_IP(a00, px0, sizeof(ae_f32x2));
        AE_L32X2_IP(a11, px1, sizeof(ae_f32x2));
        AE_L32X2_IP(a10, px1, sizeof(ae_f32x2));
        AE_L32X2_IP(a21, px2, sizeof(ae_f32x2));
        AE_L32X2_IP(a20, px2, sizeof(ae_f32x2));
        AE_L32X2_IP(a31, px3, sizeof(ae_f32x2));
        AE_L32X2_IP(a30, px3, sizeof(ae_f32x2));

        AE_ADDANDSUBRNG32(b00, b20, a00, a20);
        AE_ADDANDSUBRNG32(b01, b21, a01, a21);
        AE_ADDANDSUBRNG32(b10, b30, a10, a30);
        AE_ADDANDSUBRNG32(b11, b31, a11, a31);

        b30 = AE_MUL32JS(b30);
        b31 = AE_MUL32JS(b31);

        AE_ADDANDSUBRNG32(a00, a20, b00, b10);
        AE_ADDANDSUBRNG32(a01, a21, b01, b11);
        AE_ADDANDSUBRNG32(a30, a10, b20, b30);
        AE_ADDANDSUBRNG32(a31, a11, b21, b31);

        b00 = a00;
        b01 = a01;
        b10 = AE_MULFC32X16RAS_L(a10, tw1);
        b11 = AE_MULFC32X16RAS_L(a11, tw1);
        b20 = AE_MULFC32X16RAS_L(a20, tw2);
        b21 = AE_MULFC32X16RAS_L(a21, tw2);
        b30 = AE_MULFC32X16RAS_L(a30, tw3);
        b31 = AE_MULFC32X16RAS_L(a31, tw3);

        /* Two middle quartiles are swapped on all but the last stage to use the bit reversal
         * permutation instead of the digit reverse. */
        AE_S32X2_IP(b00, py0, sizeof(ae_f32x2));
        AE_S32X2_IP(b01, py0, sizeof(ae_f32x2));
        AE_S32X2_IP(b20, py1, sizeof(ae_f32x2));
        AE_S32X2_IP(b21, py1, sizeof(ae_f32x2));
        AE_S32X2_IP(b10, py2, sizeof(ae_f32x2));
        AE_S32X2_IP(b11, py2, sizeof(ae_f32x2));
        AE_S32X2_IP(b30, py3, sizeof(ae_f32x2));
        AE_S32X2_IP(b31, py3, sizeof(ae_f32x2));
    }

    /*----------------------------------------------
      Perform second through the next to last stages.*/
    for ( stride/=4; stride>1; stride/=4 )
    {
        twdstep *= 4;

        for ( m=0; m*(4*stride)<N; m++ )
        {
            ptwd = (const ae_int32 *)(twd);

            px0 = (ae_int32x2 *)(y+4*m*2*stride+0*2*stride);
            px1 = (ae_int32x2 *)(y+4*m*2*stride+1*2*stride);
            px2 = (ae_int32x2 *)(y+4*m*2*stride+2*2*stride);
            px3 = (ae_int32x2 *)(y+4*m*2*stride+3*2*stride);

            py0 = (ae_int32x2 *)(y+4*m*2*stride+0*2*stride);
            py1 = (ae_int32x2 *)(y+4*m*2*stride+1*2*stride);
            py2 = (ae_int32x2 *)(y+4*m*2*stride+2*2*stride);
            py3 = (ae_int32x2 *)(y+4*m*2*stride+3*2*stride);

            __Pragma("loop_count min=1");
            for ( n=0; n<stride; n++ )
            {
                AE_L32_IP(t0, ptwd, 2*sizeof(int16_t)); tw1 = AE_MOVF16X4_FROMINT32X2(t0);
                AE_L32_IP(t0, ptwd, 2*sizeof(int16_t)); tw2 = AE_MOVF16X4_FROMINT32X2(t0);
                AE_L32_IP(t0, ptwd, 2*sizeof(int16_t)); tw3 = AE_MOVF16X4_FROMINT32X2(t0);
                ptwd += twdstep*3-3;
                tw1 = AE_SHORTSWAP(tw1);
                tw2 = AE_SHORTSWAP(tw2);
                tw3 = AE_SHORTSWAP(tw3);

                AE_L32X2_IP(a00, px0, sizeof(ae_f32x2));
                AE_L32X2_IP(a01, px0, sizeof(ae_f32x2));
                AE_L32X2_IP(a10, px1, sizeof(ae_f32x2));
                AE_L32X2_IP(a11, px1, sizeof(ae_f32x2));
                AE_L32X2_IP(a20, px2, sizeof(ae_f32x2));
                AE_L32X2_IP(a21, px2, sizeof(ae_f32x2));
                AE_L32X2_IP(a30, px3, sizeof(ae_f32x2));
                AE_L32X2_IP(a31, px3, sizeof(ae_f32x2));

                AE_ADDANDSUBRNG32(b00, b20, a00, a20);
                AE_ADDANDSUBRNG32(b01, b21, a01, a21);
                AE_ADDANDSUBRNG32(b10, b30, a10, a30);
                AE_ADDANDSUBRNG32(b11, b31, a11, a31);

                b30 = AE_MUL32JS(b30);
                b31 = AE_MUL32JS(b31);

                AE_ADDANDSUBRNG32(a00, a20, b00, b10);
                AE_ADDANDSUBRNG32(a01, a21, b01, b11);
                AE_ADDANDSUBRNG32(a30, a10, b20, b30);
                AE_ADDANDSUBRNG32(a31, a11, b21, b31);

                b00 = a00;
                b01 = a01;
                b10 = AE_MULFC32X16RAS_L(a10, tw1);
                b11 = AE_MULFC32X16RAS_L(a11, tw1);
                b20 = AE_MULFC32X16RAS_L(a20, tw2);
                b21 = AE_MULFC32X16RAS_L(a21, tw2);
                b30 = AE_MULFC32X16RAS_L(a30, tw3);
                b31 = AE_MULFC32X16RAS_L(a31, tw3);

                /* Two middle quartiles are swapped on all but the last stage to use the bit reversal
                 * permutation instead of the digit reverse. */
                AE_S32X2_IP(b00, py0, sizeof(ae_f32x2));
                AE_S32X2_IP(b01, py0, sizeof(ae_f32x2));
                AE_S32X2_IP(b20, py1, sizeof(ae_f32x2));
                AE_S32X2_IP(b21, py1, sizeof(ae_f32x2));
                AE_S32X2_IP(b10, py2, sizeof(ae_f32x2));
                AE_S32X2_IP(b11, py2, sizeof(ae_f32x2));
                AE_S32X2_IP(b30, py3, sizeof(ae_f32x2));
                AE_S32X2_IP(b31, py3, sizeof(ae_f32x2));
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
        __Pragma("loop_count min=1");
        for ( n=0; n<(N>>2); n++ )
        {
            AE_L32X2_IP(a00, px0,   sizeof(ae_f32x2));
            AE_L32X2_IP(a01, px0, 7*sizeof(ae_f32x2));
            AE_L32X2_IP(a10, px1,   sizeof(ae_f32x2));
            AE_L32X2_IP(a11, px1, 7*sizeof(ae_f32x2));
            AE_L32X2_IP(a20, px2,   sizeof(ae_f32x2));
            AE_L32X2_IP(a21, px2, 7*sizeof(ae_f32x2));
            AE_L32X2_IP(a30, px3,   sizeof(ae_f32x2));
            AE_L32X2_IP(a31, px3, 7*sizeof(ae_f32x2));

            AE_ADDANDSUBRNG32(b00, b20, a00, a20);
            AE_ADDANDSUBRNG32(b01, b21, a01, a21);
            AE_ADDANDSUBRNG32(b10, b30, a10, a30);
            AE_ADDANDSUBRNG32(b11, b31, a11, a31);

            b30 = AE_MUL32JS(b30);
            b31 = AE_MUL32JS(b31);

            AE_ADDANDSUBRNG32(a00, a20, b00, b10);
            AE_ADDANDSUBRNG32(a01, a21, b01, b11);
            AE_ADDANDSUBRNG32(a30, a10, b20, b30);
            AE_ADDANDSUBRNG32(a31, a11, b21, b31);

            /* Real and imaginary parts are swapped on the first and last stages to
             * inverse the FFT:
             * conj(x) == -j*swap(x) =>
             * ifft(x) == conj(fft(conj(x)) == swap(fft(swap(x))) */
            AE_S32X2_X(a01, py0, idx+0*sizeof(ae_f32x2));
            AE_S32X2_X(a00, py0, idx+1*sizeof(ae_f32x2));
            AE_S32X2_X(a11, py1, idx+0*sizeof(ae_f32x2));
            AE_S32X2_X(a10, py1, idx+1*sizeof(ae_f32x2));
            AE_S32X2_X(a21, py2, idx+0*sizeof(ae_f32x2));
            AE_S32X2_X(a20, py2, idx+1*sizeof(ae_f32x2));
            AE_S32X2_X(a31, py3, idx+0*sizeof(ae_f32x2));
            AE_S32X2_X(a30, py3, idx+1*sizeof(ae_f32x2));

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
        __Pragma("loop_count min=2, factor=2");
        for ( n=0; n<(N>>1); n++ )
        {
            AE_L32X2_IP(a00, px0, 4*sizeof(ae_f32x2));
            AE_L32X2_IP(a01, px1, 4*sizeof(ae_f32x2));
            AE_L32X2_IP(a10, px2, 4*sizeof(ae_f32x2));
            AE_L32X2_IP(a11, px3, 4*sizeof(ae_f32x2));

            AE_ADDANDSUBRNG32(b00, b10, a00, a10);
            AE_ADDANDSUBRNG32(b01, b11, a01, a11);

            /* Real and imaginary parts are swapped on the first and last stages to
             * inverse the FFT:
             * conj(x) == -j*swap(x) =>
             * ifft(x) == conj(fft(conj(x)) == swap(fft(swap(x))) */
            AE_S32X2_X(b01, py0, idx);
            AE_S32X2_X(b00, py1, idx);
            AE_S32X2_X(b11, py2, idx);
            AE_S32X2_X(b10, py3, idx);

            idx = AE_ADDBRBA32(idx, bitrevstride);
        }
    }
} /* ifft_cplx32x16_pair() */

/*
    DCT-III on pairs of data, N==16
    input:
    x[16]
    Output:
    y[16]
*/
static void dct3p_N16(uint64_t *x,uint64_t* y,const tdct4_twd_fr16* pdct4_twd)
{
    const int N = 16;
          ae_int32x2 * restrict px0;
          ae_int32x2 * restrict px1;
          ae_int32x2 * restrict px2;
          ae_int32x2 * restrict px3;
          ae_int32x2 * restrict py0;
          ae_int32x2 * restrict py1;
          ae_int32x2 * restrict py2;
          ae_int32x2 * restrict py3;
    const ae_int32 * restrict ptwd0;
    const ae_int32 * restrict ptwd1;
    const ae_int32 * restrict ptwd2;
    ae_f32x2 X0, X1, T0, T1, Y0, Y1;
    ae_f32x2 a0re, a0im, b0re, b0im;
    ae_f32x2 a1re, a1im, b1re, b1im;
    ae_f16x4 cs;
    ae_int32x2 t32;
    ae_int16x4 t0;
    int k;

    px0 = (ae_int32x2 *)(x);
    px1 = (ae_int32x2 *)(x+N-1);
    px2 = (ae_int32x2 *)(x+N/2-1);
    px3 = (ae_int32x2 *)(x+N/2+1);
    py0 = (ae_int32x2 *)(y);
    py1 = (ae_int32x2 *)(y+N-1);
    ptwd0 = (const ae_int32 *)(pdct4_twd->dct3+1);
    ptwd1 = (const ae_int32 *)(pdct4_twd->dct3+7);
    ptwd2 = (const ae_int32 *)(pdct4_twd->rfft);

    WUR_AE_SAR(1);
    {
        X1 = AE_L32X2_X(px0, (N/2)*sizeof(ae_f32x2));
        AE_L32X2_IP(X0, px0, sizeof(ae_f32x2));
        X1 = AE_MULFP32X16X2RAS_L(X1, AE_MOVDA16(23170));

        AE_ADDANDSUBRNG32(X0, X1, X0, X1);

        Y0 = AE_SEL32_HH(X1, X0);
        Y1 = AE_SEL32_LL(X1, X0);

        AE_S32X2_IP(Y0,py0,sizeof(ae_f32x2));
        AE_S32X2_IP(Y1,py0,sizeof(ae_f32x2));
    }
    for (k=1; k<N/4; k++)
    {
        AE_L32_IP(t32, ptwd0, 2*sizeof(int16_t)); cs = AE_MOVINT16X4_FROMINT32X2(t32);
        AE_L32X2_IP(X0, px0,       sizeof(ae_f32x2));
        AE_L32X2_XP(X1, px1, -(int)sizeof(ae_f32x2));

        T0 = AE_F32X2_SRAI(X0, 1);
        T1 = AE_F32X2_SRAI(X1, 1);
        X0 = AE_SEL32_HH(T0, T1);
        X1 = AE_SEL32_LL(T0, T1);
        a0re = AE_MULFC32X16RAS_L(X0, cs);
        a0im = AE_MULFC32X16RAS_L(X1, cs);

        AE_L32_IP(t32, ptwd1, -2*(int)sizeof(int16_t)); cs = AE_MOVINT16X4_FROMINT32X2(t32);
        AE_L32X2_XP(X0, px2, -(int)sizeof(ae_f32x2));
        AE_L32X2_IP(X1, px3,       sizeof(ae_f32x2));

        T0 = AE_F32X2_SRAI(X0, 1);
        T1 = AE_F32X2_SRAI(X1, 1);
        X0 = AE_SEL32_HH(T0, T1);
        X1 = AE_SEL32_LL(T0, T1);
        a1re = AE_MULFC32X16RAS_L(X0, cs);
        a1im = AE_MULFC32X16RAS_L(X1, cs);

        AE_L32_IP(t32, ptwd2, 2*sizeof(int16_t)); t0 = AE_MOVINT16X4_FROMINT32X2(t32);
        cs = AE_SHORTSWAP(t0);

        AE_ADDANDSUBRNG32(Y0, Y1, a0re, a1re);
        AE_ADDANDSUBRNG32(X0, X1, a0im, a1im);
        b0re = AE_SEL32_HL(Y1, Y0);
        b1re = AE_SEL32_LH(Y1, Y0);
        b0im = AE_SEL32_HL(X1, X0);
        b1im = AE_SEL32_LH(X1, X0);
        a0re = AE_MULFC32X16RAS_L(b1re, cs);
        a0im = AE_MULFC32X16RAS_L(b1im, cs);

        X0 = b0re;
        X1 = b0im;
        Y0 = a0re;
        Y1 = a0im;
        a0re = AE_ADDSUB32(X0, Y0);
        a1re = AE_SUBADD32(Y0, X0);
        a0im = AE_ADDSUB32(X1, Y1);
        a1im = AE_SUBADD32(Y1, X1);

        AE_S32X2_IP(a0re,py0,      sizeof(ae_f32x2));
        AE_S32X2_IP(a0im,py0,      sizeof(ae_f32x2));
        AE_S32X2_XP(a1im,py1,-(int)sizeof(ae_f32x2));
        AE_S32X2_XP(a1re,py1,-(int)sizeof(ae_f32x2));
    }
    {
        AE_L32_IP(t32, ptwd1, -2*(int)sizeof(int16_t)); cs = AE_MOVINT16X4_FROMINT32X2(t32);
        cs = AE_SHORTSWAP(cs);
        AE_L32X2_XP(X0, px2, -(int)sizeof(ae_f32x2));
        AE_L32X2_IP(X1, px3,       sizeof(ae_f32x2));
        T0 = AE_F32X2_SRAI(X0, 1);
        T1 = AE_F32X2_SRAI(X1, 1);
        X0 = AE_SEL32_HH(T1, T0);
        X1 = AE_SEL32_LL(T1, T0);
        Y0 = AE_MULFC32X16RAS_L(X0, cs);
        Y1 = AE_MULFC32X16RAS_L(X1, cs);

        AE_S32X2_XP(Y1,py1,-(int)sizeof(ae_f32x2));
        AE_S32X2_XP(Y0,py1,-(int)sizeof(ae_f32x2));
    }
    __Pragma("no_reorder");
    /* IFFT (on pairs of data, N=8) and final permutation */
    WUR_AE_SAR(1);
    {
        int stride;
        int n;

        ae_f32x2 a00, a01, a10, a11, a20, a21, a30, a31;
        ae_f32x2 b00, b01, b10, b11, b20, b21, b30, b31;
        ae_f16x4 tw1, tw2, tw3;
        ae_int32x2 t0;

        /*----------------------------------------------------------------------------*
         * Perform the first stage. We use DIF, all permutations are deferred until   *
         * the last stage.                                                            */
        stride = 8/4;
        ptwd0 = (const ae_int32 *)(pdct4_twd->fft);
        px0 = (ae_int32x2 *)(y+0*2*stride);
        px1 = (ae_int32x2 *)(y+1*2*stride);
        px2 = (ae_int32x2 *)(y+2*2*stride);
        px3 = (ae_int32x2 *)(y+3*2*stride);
        py0 = (ae_int32x2 *)(x+0*2*stride);
        py1 = (ae_int32x2 *)(x+1*2*stride);
        py2 = (ae_int32x2 *)(x+2*2*stride);
        py3 = (ae_int32x2 *)(x+3*2*stride);

        for ( n=0; n<stride; n++ )
        {
            AE_L32_IP(t0, ptwd0, 2*sizeof(int16_t)); tw1 = AE_MOVF16X4_FROMINT32X2(t0);
            AE_L32_IP(t0, ptwd0, 2*sizeof(int16_t)); tw2 = AE_MOVF16X4_FROMINT32X2(t0);
            AE_L32_IP(t0, ptwd0, 2*sizeof(int16_t)); tw3 = AE_MOVF16X4_FROMINT32X2(t0);
            tw1 = AE_SHORTSWAP(tw1);
            tw2 = AE_SHORTSWAP(tw2);
            tw3 = AE_SHORTSWAP(tw3);

            /* Real and imaginary parts are swapped on the first and last stages to
             * inverse the FFT:
             * conj(x) == -j*swap(x) =>
             * ifft(x) == conj(fft(conj(x)) == swap(fft(swap(x)))
             * Just in case, divide data by the FFT size. */
            AE_L32X2_IP(a01, px0, sizeof(ae_f32x2));
            AE_L32X2_IP(a00, px0, sizeof(ae_f32x2));
            AE_L32X2_IP(a11, px1, sizeof(ae_f32x2));
            AE_L32X2_IP(a10, px1, sizeof(ae_f32x2));
            AE_L32X2_IP(a21, px2, sizeof(ae_f32x2));
            AE_L32X2_IP(a20, px2, sizeof(ae_f32x2));
            AE_L32X2_IP(a31, px3, sizeof(ae_f32x2));
            AE_L32X2_IP(a30, px3, sizeof(ae_f32x2));

            AE_ADDANDSUBRNG32(b00, b20, a00, a20);
            AE_ADDANDSUBRNG32(b01, b21, a01, a21);
            AE_ADDANDSUBRNG32(b10, b30, a10, a30);
            AE_ADDANDSUBRNG32(b11, b31, a11, a31);

            b30 = AE_MUL32JS(b30);
            b31 = AE_MUL32JS(b31);

            AE_ADDANDSUBRNG32(a00, a20, b00, b10);
            AE_ADDANDSUBRNG32(a01, a21, b01, b11);
            AE_ADDANDSUBRNG32(a30, a10, b20, b30);
            AE_ADDANDSUBRNG32(a31, a11, b21, b31);

            b00 = a00;
            b01 = a01;
            b10 = AE_MULFC32X16RAS_L(a10, tw1);
            b11 = AE_MULFC32X16RAS_L(a11, tw1);
            b20 = AE_MULFC32X16RAS_L(a20, tw2);
            b21 = AE_MULFC32X16RAS_L(a21, tw2);
            b30 = AE_MULFC32X16RAS_L(a30, tw3);
            b31 = AE_MULFC32X16RAS_L(a31, tw3);

            /* Two middle quartiles are swapped on all but the last stage to use the bit reversal
             * permutation instead of the digit reverse. */
            AE_S32X2_IP(b00, py0, sizeof(ae_f32x2));
            AE_S32X2_IP(b01, py0, sizeof(ae_f32x2));
            AE_S32X2_IP(b20, py1, sizeof(ae_f32x2));
            AE_S32X2_IP(b21, py1, sizeof(ae_f32x2));
            AE_S32X2_IP(b10, py2, sizeof(ae_f32x2));
            AE_S32X2_IP(b11, py2, sizeof(ae_f32x2));
            AE_S32X2_IP(b30, py3, sizeof(ae_f32x2));
            AE_S32X2_IP(b31, py3, sizeof(ae_f32x2));
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
                AE_L32X2_IP(a00, px0, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a01, px1, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a10, px2, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a11, px3, 4*sizeof(ae_f32x2));

                AE_ADDANDSUBRNG32(b00, b10, a00, a10);
                AE_ADDANDSUBRNG32(b01, b11, a01, a11);

                a00 = AE_SEL32_HH(b01, b00);
                a01 = AE_SEL32_LL(b01, b00);
                a10 = AE_SEL32_HH(b11, b10);
                a11 = AE_SEL32_LL(b11, b10);
                AE_S32X2_I(a01, py0, 0*2*sizeof(ae_f32x2));
                AE_S32X2_I(a00, py0, 1*2*sizeof(ae_f32x2));
                AE_S32X2_X(a11, py1, 7*2*sizeof(ae_f32x2));
                AE_S32X2_X(a10, py1, 6*2*sizeof(ae_f32x2));
            }
            {
                AE_L32X2_IP(a00, px0, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a01, px1, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a10, px2, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a11, px3, 4*sizeof(ae_f32x2));

                AE_ADDANDSUBRNG32(b00, b10, a00, a10);
                AE_ADDANDSUBRNG32(b01, b11, a01, a11);

                a00 = AE_SEL32_HH(b01, b00);
                a01 = AE_SEL32_LL(b01, b00);
                a10 = AE_SEL32_HH(b11, b10);
                a11 = AE_SEL32_LL(b11, b10);
                AE_S32X2_X(a01, py0, 4*2*sizeof(ae_f32x2));
                AE_S32X2_X(a00, py0, 5*2*sizeof(ae_f32x2));
                AE_S32X2_I(a11, py1, 3*2*sizeof(ae_f32x2));
                AE_S32X2_I(a10, py1, 2*2*sizeof(ae_f32x2));
            }
            {
                AE_L32X2_IP(a00, px0, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a01, px1, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a10, px2, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a11, px3, 4*sizeof(ae_f32x2));

                AE_ADDANDSUBRNG32(b00, b10, a00, a10);
                AE_ADDANDSUBRNG32(b01, b11, a01, a11);

                a00 = AE_SEL32_HH(b01, b00);
                a01 = AE_SEL32_LL(b01, b00);
                a10 = AE_SEL32_HH(b11, b10);
                a11 = AE_SEL32_LL(b11, b10);
                AE_S32X2_I(a01, py0, 2*2*sizeof(ae_f32x2));
                AE_S32X2_I(a00, py0, 3*2*sizeof(ae_f32x2));
                AE_S32X2_X(a11, py1, 5*2*sizeof(ae_f32x2));
                AE_S32X2_X(a10, py1, 4*2*sizeof(ae_f32x2));
            }
            {
                AE_L32X2_IP(a00, px0, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a01, px1, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a10, px2, 4*sizeof(ae_f32x2));
                AE_L32X2_IP(a11, px3, 4*sizeof(ae_f32x2));

                AE_ADDANDSUBRNG32(b00, b10, a00, a10);
                AE_ADDANDSUBRNG32(b01, b11, a01, a11);

                a00 = AE_SEL32_HH(b01, b00);
                a01 = AE_SEL32_LL(b01, b00);
                a10 = AE_SEL32_HH(b11, b10);
                a11 = AE_SEL32_LL(b11, b10);
                AE_S32X2_X(a01, py0, 6*2*sizeof(ae_f32x2));
                AE_S32X2_X(a00, py0, 7*2*sizeof(ae_f32x2));
                AE_S32X2_I(a11, py1, 1*2*sizeof(ae_f32x2));
                AE_S32X2_I(a10, py1, 0*2*sizeof(ae_f32x2));
            }
        }
    } /* IFFT with permutation */
}
