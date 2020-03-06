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

/* Scalar Fixed-Point Sine
 * The sine functions returns the cosine value of x. Input value is scaled up
 * by pi, so that input domain [-1,1) corresponds to [-pi,pi). */
static fract32 sin_32b( fract32 x )
{
#if !USE_REFERENCE_CODE
    ae_f32 _x, x2, x2h, y1, y2;
    ae_f32x2 t;
    fract32 y;
    const ae_int32 * TBL;
    int sx, idx;
    const int _1 = XT_MOVI(1);
    sx = XT_EXTUI(x, 31, 1);
    x = AE_MOVAD32_H(AE_ABS32S(AE_SLLI32(AE_MOVDA32(x), 1)));
    idx = XT_EXTUI(x, 28, 3);
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
    t = y2; AE_MULAFP32X2RAS(t, _x, y1); /* -> Q31 */
    y = AE_MOVAD32_H(AE_NEG32S(t));
    XT_MOVEQZ(y, AE_MOVAD32_H(t), sx);
    return y;
#else /* !USE_REFERENCE_CODE */
    int32_t x2,y1,y2,y;
    int sx,idx;
    sx = x<0;
    x = L_abs_l(x<<1);
    idx = ((uint32_t)x)>>28;
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
    y = L_add_ll(y2, mulf32x32ras(x<<1, y1)); /* -> Q31 */
    return (sx ? L_neg_l(y) : y);
#endif /* USE_REFERENCE_CODE */
} /* sin_32b() */

/*
 * Compute the lifter coefficients, as defined in The HTK Book, see [1] p.75 Eq.(5.12).
 * MATLAB reference code for the liftering operation:
 *   lifter = @(c,L)((1+L/2*sin(pi/L*(0:length(c)-1)')).*c);
 * Input:
 * L               Lifter parameter
 * N               Number of coeffs to compute (equals the number of cepstral features)
 * Output:
 * lifterCoefs[N]  Lifter coeffs, Q(31-lifterExp) for 32x32
 * lifterExp       Returned value (32x32 only), base-2 block exponent of lifter coeffs.
 * Restrictions:
 * lifterCoefs[]   Aligned by 8-bytes
 * N>0
 */

int  mfcc32x32_compLifterCoefs( fract32   * restrict lifterCoefs, int L, int N )
{
#if !USE_REFERENCE_CODE
    ae_f64 psi, cf;
    ae_f32 r_fract, cf_r;
    fract32 theta, sn;
    int16_t r_exp;
    int n, lifterExp;
    NASSERT_ALIGN(lifterCoefs, ALIGN_SIZE);
    NASSERT(N>0); NASSERT(L>0);
    lifterExp = 30-NSA(2+L);
    /* r <- 1/L; Q(r_exp) */
    r_fract = AE_MOVDA32(recip32x32(&r_exp, L, 0)); NASSERT(r_exp>=31);
    for ( n=0; n<N; n++ ) {
        /* psi <- n/L == n*r; Q0*Q(r_exp) */
        psi = AE_MUL32_LL(AE_MOVDA32(n), r_fract);
        /* theta <- mod(1+psi,2)-1; Q31 <- Q(r_exp) + 31-r_exp */
        theta = AE_MOVAD32_L(AE_MOVINT32X2_FROMINT64(AE_SLAA64(psi, 31-r_exp)));
        /* sn <- sin(pi*theta); Q31 */
        sn = sin_32b(theta);
        /* cf <- 1+L/2*sn; Q32 <- (Q0/2+1)*Q31 */
        cf = AE_CVT64A32(XT_MOVI(1)); AE_MULA32_HH(cf, AE_MOVDA32(L), AE_MOVDA32(sn));
        /* Q(31-lifterExp) <- [Q32 + 31-lifterExp] - 32 w/ rounding */
        cf_r = AE_ROUND32F64SASYM(AE_SLAA64S(cf, 31-lifterExp));
        AE_S32_L_X(cf_r, (ae_int32*)lifterCoefs, n*sz_i32);
    }
    return lifterExp;
#else /* !USE_REFERENCE_CODE */
    int64_t psi, cf;
    fract32 r_fract, theta, sn;
    int16_t r_exp;
    int n, lifterExp;
    NASSERT_ALIGN(lifterCoefs, ALIGN_SIZE);
    NASSERT(N>0); NASSERT(L>0);
    lifterExp = 30-S_exp0_l(2+L);
    /* r <- 1/L; Q(r_exp) */
    r_fract = recip32x32(&r_exp, L, 0); NASSERT(r_exp>=31);
    for ( n=0; n<N; n++ ) {
        /* psi <- n/L == n*r; Q0*Q(r_exp) */
        psi = (int64_t)n*r_fract;
        /* theta <- mod(1+psi,2)-1; Q31 <- Q(r_exp) + 31-r_exp */
        theta = (fract32)(psi>>(r_exp-31));
        /* sn <- sin(pi*theta); Q31 */
        sn = sin_32b(theta);
        /* cf <- 1+L/2*sn; Q32 <- (Q0/2+1)*Q31 */
        cf = (1LL<<32)+(int64_t)L*sn;
        /* Q(31-lifterExp) <- Q32 - (lifterExp+1) w/ rounding */
        lifterCoefs[n] = satQ31((cf+(1LL<<lifterExp))>>(lifterExp+1));
    }
    return lifterExp;
#endif /* USE_REFERENCE_CODE */
} /* mfcc32x32_compLifterCoefs() */
