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
  NatureDSP Signal Processing Library. Audio processing part
    Compute Mel-Frequency Cepstrum Coefficients (MFCC) from speech signal
    32-bit fixed-point variant
    C code optimized for HiFi4 core
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_math.h"
/* MFCC extractor internal declarations. */
#include "mfcc_internal.h"

#define USE_REFERENCE_CODE    0
#define ALIGN_SIZE            (XCHAL_DATA_WIDTH)
#define sz_i32                sizeof(int32_t)

#if USE_REFERENCE_CODE
/* NON OPTIMIZED REFERENCE CODE: to be use it for educational purposes only */
#include "baseop.h"

/* Q(x+y-15) <- Qx*Qy - 15 w/ asym. rounding */
static int32_t mulf32x16ras(int32_t x, int16_t y)
{
    int64_t z;
    z=(int64_t)x*y+(1L<<14);
    z>>=15;
    if (z>MAX_INT32)z=MAX_INT32;
    if (z<MIN_INT32)z=MIN_INT32;
    return (int32_t)z;
} /* mulf32x16ras() */

/* Q(x+y-31) <- Qx*Qy - 31 w/ asym. rounding */
static int32_t mulf32x32ras(int32_t x, int32_t y)
{
    int64_t z;
    z=(int64_t)x*y+(1L<<30);
    z>>=31;
    if (z>MAX_INT32)z=MAX_INT32;
    if (z<MIN_INT32)z=MIN_INT32;
    return (int32_t)z;
} /* mulf32x32ras() */
#endif /* USE_REFERENCE_CODE */

/* Compute the 32-bit fixed-point reciprocal for input arguments s_fract, s_exp: 
 * r_fract*2^-r_exp ~= 1/(s_fract*2^-s_exp), where r_fract is returned value and
 * r_exp is the output argument. */
static fract32 recip32x32( int16_t * r_exp, int32_t s_fract, int16_t s_exp )
{
    fract32 r_fract;
    vec_recip32x32(&r_fract, r_exp, &s_fract, 1);
    *r_exp = 62-s_exp-*r_exp;
    return r_fract;
} /* recip32x32() */

/* Scalar Fixed-Point Cosine
 * The cosine functions returns the cosine value of x. Input value is scaled up
 * by pi, so that input domain [-1,1) corresponds to [-pi,pi). */
static fract32 cos_32b( fract32 x )
{
#if !USE_REFERENCE_CODE
    ae_f32 _x, x2, x2h, y1, y2;
    ae_f32x2 t;
    fract32 y;
    const ae_int32 * TBL;
    int s0, s1, idx;
    const int _1 = XT_MOVI(1);
    s0 = XT_EXTUI(x, 30, 1);
    s1 = XT_EXTUI(x, 31, 1);
    x = AE_MOVAD32_H(AE_ABS32S(AE_SLLI32(AE_MOVDA32(x), 1)));
    idx = XT_SUB(XT_MOVI(7), XT_EXTUI(x, 28, 3));
    x = XT_AND(x, XT_SUB(XT_SLLI(_1, 28), _1));
    x = XT_SUB(x, XT_SLLI(_1, 27));
    _x = AE_SLLI32(AE_MOVDA32(x), 1);
    x2 = AE_F32_MULF_F32(_x, AE_MOVDA32(x)); /* -> Q32 */
    x2h = AE_SRAI32(x2, 1); /* -> Q31 */
    NASSERT(sizeof(mfcc32x32_polysin_tbl)==6*8*sizeof(fract32));
    TBL = (ae_int32*)&mfcc32x32_polysin_tbl[idx];
    AE_L32_XP(t, TBL, 8*sz_i32); y1 = t;
    AE_L32_XP(t, TBL, 8*sz_i32); y2 = t;
    AE_L32_XP(t, TBL, 8*sz_i32); AE_MULAFP32X2RAS(t, x2h, y1); y1 = t; /* -> Q30 */
    AE_L32_XP(t, TBL, 8*sz_i32); AE_MULAFP32X2RAS(t, x2h, y2); y2 = t; /* -> Q30 */
    AE_L32_XP(t, TBL, 8*sz_i32); AE_MULAFP32X2RAS(t, x2h, y1); y1 = t; /* -> Q30 */
    AE_L32_XP(t, TBL, 8*sz_i32); AE_MULAFP32X2RAS(t, x2, y2); y2 = t; /* -> Q31 */
    t = y2; AE_MULSFP32X2RAS(t, _x, y1); /* -> Q31 */
    y = AE_MOVAD32_H(AE_NEG32S(t));
    XT_MOVEQZ(y, AE_MOVAD32_H(t), XT_XOR(s0, s1));
    return y;
#else /* !USE_REFERENCE_CODE */
    int32_t x2,y1,y2,y;
    int s0,s1,idx;
    s0 = (x>>30)&1;
    s1 = (x>>31)&1;
    x = L_abs_l(x<<1);
    idx=((uint32_t)x)>>28;
    idx = 7-idx;
    x &= (1L<<28)-1;
    x -= (1L<<27);
    x2 = mulf32x32ras(x<<1, x); /* -> Q32 */
    NASSERT(sizeof(mfcc32x32_polysin_tbl)==6*8*sizeof(fract32));
    y1=mfcc32x32_polysin_tbl[8*0+idx];
    y2=mfcc32x32_polysin_tbl[8*1+idx];
    y1 = mulf32x32ras(x2>>1, y1) + mfcc32x32_polysin_tbl[8*2+idx]; /* -> Q30 */
    y2 = mulf32x32ras(x2>>1, y2) + mfcc32x32_polysin_tbl[8*3+idx]; /* -> Q30 */
    y1 = mulf32x32ras(x2>>1, y1) + mfcc32x32_polysin_tbl[8*4+idx]; /* -> Q30 */
    y2 = mulf32x32ras(x2   , y2) + mfcc32x32_polysin_tbl[8*5+idx]; /* -> Q31 */
    y = L_sub_ll(y2, mulf32x32ras(x<<1, y1)); /* -> Q31 */
    return (s0^s1 ? L_neg_l(y) : y);
#endif /* USE_REFERENCE_CODE */
} /* cos_32b() */

/* 
 * Calculate the DCT matrix, as defined in The HTK Book, see [1] p.77 Eq.(5.14).
 * MATLAB reference code:
 *   dctMtx = (2/N)^0.5*cos(pi/N*(0:M-1)'*((1:N)-0.5));
 *   if orthNorm, dctMtx(1,:) = dctMtx(1,:)/2^0.5; end;
 * For efficiency reasons, the DCT matrix may be stored as a submatrix of a larger
 * matrix, as specified by input arguemnts M,N (DCT dimensions) and rows,cols (matrix
 * dimensions): 0<M<=rows, 0<N<=cols. Padding elements of the larger matrix are zeroed.
 * Input:
 * M                  Vertical dimension of the DCT transform
 * N                  Horizontal dimension of the DCT transform
 * rows               Vertical dimension of the storage matrix
 * cols               Horizontal dimension of the storage matrix
 * orthNorm           If non-zero, then in addition to regular normalization of the whole matrix
 *                    by a factor of (2/N)^0.5, its first row is further multiplied by 1/2^0.5, 
 *                    so that the resulting DCT matrix becomes orthogonally normalized.
 * Output:
 * dctMtx[rows*cols]  DCT matrix of size MxN, padded with zeros to form a storage matrix
 *                    of size rows x cols; Q(31-dctExp) for 32x32
 * dctExp             Returned value (32x32 only), base-2 exponent of the DCT matrix
 * Restrictions:
 * dctMtx[]           Aligned by 8-bytes
 * 0<M<=rows
 * 0<N<=cols 
 */

int mfcc32x32_compDctMatrix( fract32 * restrict dctMtx, int M, int N, int rows, int cols, int orthNorm )
{
#if !USE_REFERENCE_CODE
    int16_t r_exp;
    int nsa, s_exp, dctExp, m, n;
    fract32 r_fract, theta, cs_m0, cs;
    ae_f32 alpha, s_fract;
    ae_f64 psi;
    NASSERT_ALIGN(dctMtx, ALIGN_SIZE);
    NASSERT(0<M && M<=rows);
    NASSERT(0<N && N<=cols);
    /* r <- 1/N; Q(r_exp) */
    r_fract = recip32x32(&r_exp, N, 0);
    /* alpha <- (2*r)^0.5; Q(31-dctExp) */
    if (r_exp&1) {
        /* sqrt: Q((x-1)/2) <- Qx;
         * Q((r_exp-1+31-1)/2) <- (2*Q(r_exp)-1)+31 */
        alpha = AE_MOVDA32(scl_sqrt64x32((int64_t)r_fract<<31));
        /* (r_exp-1+31-1)/2 == 31-dctExp */
        dctExp = 31-((r_exp-1+31-1)>>1);
    } else {
        /* Q((r_exp-1+32-1)/2) <- (2*Q(r_exp)-1)+32 */
        alpha = AE_MOVDA32(scl_sqrt64x32((int64_t)r_fract<<32));
        dctExp = 31-((r_exp-1+32-1)>>1);
    }
    /* Value for the first row, either 1/2^0.5 or 1.0 */
    cs_m0 = 1518500250L;
    XT_MOVEQZ(cs_m0, MAX_INT32, orthNorm);
    for ( n=0; n<N; n++ ) {
        /* s <- (n+0.5)/N; Q(r_exp+nsa-31) <- Q(r_exp)*Q(nsa) - 15 w/ rounding */
        nsa = NSA(n)-16;
        s_fract = AE_F32_MUL_F16(AE_MOVDA32(r_fract), AE_MOVDA16((n<<nsa)+(1<<(nsa-1))));
        s_exp = r_exp+nsa-15;
        AE_S32_L_X(AE_F32_MULF_F32(alpha, AE_MOVDA32(cs_m0)), (ae_int32*)dctMtx, n*sz_i32);
        for ( m=1; m<M; m++ ) {
            /* psi <- m*(n+0.5)/N == m*s; Q0*Q(s_exp) */
            psi = AE_MUL32_HH(AE_MOVDA32(m), s_fract);
            /* theta <- mod(1+psi,2)-1; Q31 <- Q(s_exp) + 31-s_exp */
            theta = AE_MOVAD32_L(AE_MOVINT32X2_FROMINT64(AE_SLAA64(psi, 31-s_exp)));
            /* cs <- cos(pi*theta); Q31 */
            cs = cos_32b(theta);
            /* Q(31-dctExp) <- Q(31-dctExp)*Q31 - 31 w/ rounding */
            AE_S32_L_X(AE_F32_MULF_F32(alpha, AE_MOVDA32(cs)), (ae_int32*)dctMtx, (m*cols+n)*sz_i32);
        } /* m */
        for ( m=M; m<rows; m++ ) {
            AE_S32_L_X(AE_MOVI(0), (ae_int32*)dctMtx, (m*cols+n)*sz_i32);
        }
    } /* n */
    for ( n=N; n<cols; n++ ) {
        for ( m=0; m<rows; m++ ) {
            AE_S32_L_X(AE_MOVI(0), (ae_int32*)dctMtx, (m*cols+n)*sz_i32);
        } /* m */
    } /* n */
    return dctExp;
#else /* !USE_REFERENCE_CODE */
    fract32 r_fract, s_fract, alpha, theta, cs;
    int16_t r_exp;
    int nsa, s_exp, dctExp, m, n;
    int64_t psi;
    NASSERT_ALIGN(dctMtx, ALIGN_SIZE);
    NASSERT(0<M && M<=rows);
    NASSERT(0<N && N<=cols);
    /* r <- 1/N; Q(r_exp) */
    r_fract = recip32x32(&r_exp, N, 0);
    /* alpha <- (2*r)^0.5; Q(31-dctExp) */
    if (r_exp&1) {
        /* sqrt: Q((x-1)/2) <- Qx;
         * Q((r_exp-1+31-1)/2) <- (2*Q(r_exp)-1)+31 */
        alpha = scl_sqrt64x32((int64_t)r_fract<<31);
        /* (r_exp-1+31-1)/2 == 31-dctExp */
        dctExp = 31-((r_exp-1+31-1)>>1);
    } else {
        /* Q((r_exp-1+32-1)/2) <- (2*Q(r_exp)-1)+32 */
        alpha = scl_sqrt64x32((int64_t)r_fract<<32);
        dctExp = 31-((r_exp-1+32-1)>>1);
    }
    for ( n=0; n<cols; n++ ) {
        /* s <- (n+0.5)/N; Q(r_exp+nsa-31) <- Q(r_exp)*Q(nsa) - 15 w/ rounding */
        nsa = S_exp0_s(n);
        s_fract = mulf32x16ras(r_fract, (n<<nsa)+(1L<<(nsa-1)));
        s_exp = r_exp+nsa-15;
        for ( m=0; m<rows; m++ ) {
            if (m<M && n<N) {
                if (m>0) {
                    /* psi <- m*(n+0.5)/N == m*s; Q0*Q(s_exp) */
                    psi = (int64_t)m*s_fract;
                    /* theta <- mod(1+psi,2)-1; Q31 <- Q(s_exp) + 31-s_exp */
                    theta = (fract32)(31>=s_exp ? psi<<(31-s_exp) : psi>>-(31-s_exp));
                    /* cs <- cos(pi*theta); Q31 */
                    cs = cos_32b(theta);
                } else {
                    /* First row, either 1/2^0.5 or 1.0 */
                    cs = (orthNorm ? 1518500250L : MAX_INT32);
                }
            } else {
                cs = 0;
            }
            /* Q(31-dctExp) <- Q(31-dctExp)*Q31 - 31 w/ rounding */
            dctMtx[m*cols+n] = mulf32x32ras(alpha, cs);
        } /* m */
    } /* n */
    return dctExp;
#endif /* USE_REFERENCE_CODE */
} /* mfcc32x32_compDctMatrix() */
