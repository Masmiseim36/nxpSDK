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

#if USE_REFERENCE_CODE
/* NON OPTIMIZED REFERENCE CODE: to be use it for educational purposes only */
#include "baseop.h"

/* simplified fractional multiply Q63*Q31->Q63 */
static int64_t mulf64x32(int64_t x,int32_t y)
{
    int64_t z;
    uint32_t xlo;
    int32_t xhi;
    xlo=(uint32_t)x;
    xhi=(int32_t)(x>>32);
    z=((int64_t)xlo*y)>>32;
    z+=(int64_t)xhi*y;
    z<<=1;
    return z;
} /* mulf64x32() */

/* Compute the 32-bit fixed-point reciprocal for input arguments s_fract, s_exp: 
 * r_fract*2^-r_exp ~= 1/(s_fract*2^-s_exp), where r_fract is returned value and
 * r_exp is the output argument. */
static fract32 recip32x32( int16_t * r_exp, int32_t s_fract, int16_t s_exp )
{
#if 1
    fract32 r_fract;
    vec_recip32x32(&r_fract, r_exp, &s_fract, 1);
    *r_exp = 62-s_exp-*r_exp;
    return r_fract;
#else
    uint32_t r;
    fract16 e;
    fract32 r_fract;
    int s_nsa;
    NASSERT(0!=s_fract);
    s_nsa = S_exp0_l(s_fract); 
    /* Q(53-s_nsa) <- Q61/Q(s_nsa) - 8 */
    r = scl_recip32x32(s_fract<<s_nsa);
    /* Q(61-s_nsa) <- Q(53-s_nsa) + 8 */
    r_fract = (fract32)(r<<8); NASSERT(1==((int32_t)r>>24));
    /* Perform a Newton-Raphson iteration to refine 8 LSBs. */
    /* Q31 <- 2*[Q(61-s_nsa)*Q(s_nsa) - 31] + 1 */
    e = S_sature_l((1L<<31) - 2*L_mpy_ll(r_fract, s_fract<<s_nsa));
    /* Q(61-s_nsa) <- [Q(61-s_nsa)*Q31 - 15] - 16 */
    r_fract = L_add_ll(r_fract, S_round_l(L_mpy_ls(r_fract, e)));
    *r_exp = 61-s_nsa-s_exp; 
    return r_fract;
#endif
} /* recip32x32() */
#endif /* USE_REFERENCE_CODE */

/*
 * Compute the mean value over the input argument x[N], then subtract it from 
 * each element of x[N] and store the result to the output argument y[N]. 
 * Input and output arguments may refer to the same array.
 * Note:
 *   For the fixed-point variant, input data should be properly scaled, 
 *   otherwise results may saturate. It is sufficient to ensure that the
 *   minimum number of redunsant sign bits over input data is non-zero to
 *   prevent overflow.
 * Input:
 *   N          Vectors size
 *   x[N]       Input vector
 * Output:
 *   y[N]       Output vector
 * Restrictions:
 *   N          Must be a multiple of 2
 *   x[N],y[N]  Must be 8-bytes aligned
 */

void mfcc32x32_remdc( fract32 * y, const fract32 * x, int N )
{
#if !USE_REFERENCE_CODE
    const ae_int32x2 * restrict X;
    ae_int32x2 * restrict Y;
    ae_int32x2 f, s, t, mean;
    ae_int64 acc, g;
    ae_ep ep;
    int16_t r_exp;
    int32_t r_fract, _N = N;
    int n;
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(0==(N%2));
    X = (ae_int32x2*)&x[0];
    acc = AE_ZERO64();
    for ( n=0; n<(N>>1); n++ ) {
        AE_L32X2_IP(f, X, sizeof(f));
        AE_MULAAD32_HH_LL(acc, f, AE_MOVI_32(1));
    }
    /* r <- 1/N; Q(62-r_exp) */
    vec_recip32x32(&r_fract, &r_exp, &_N, 1);
    /* g <- acc*1/N; Q(31+x-r_exp) <- Qx*Q(62-r_exp) - 31 */
    s = AE_MOVINT32X2_FROMINT64(acc); t = AE_MOVINT32X2_FROMINT32(AE_MOVDA32(r_fract));
    AE_MUL32USEP_LL(ep, g, s, t); g = AE_SRAI64(g, 31); AE_MULAF32S_HL(g, s, t);
    /* mean <- round(g); Qx <- [Q(31+x-r_exp) + 1 + r_exp] - 32 w/ rounding */
    mean = AE_ROUND32F64SASYM(AE_SLAA64S(g, 1+r_exp));
    X = (ae_int32x2*)&x[0];
    Y = (ae_int32x2*)&y[0];
    for ( n=0; n<(N>>1); n++ ) {
        AE_L32X2_IP(f, X, sizeof(f));
        AE_S32X2_IP(AE_SUB32S(f, mean), Y, sizeof(f));
    }
#else
    int64_t acc, f;
    fract32 mean, r_fract;
    int16_t r_exp;
    int n;
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(0==(N%2));
    for (acc=0, n=0; n<N; n++) {
        acc += x[n]; /* Qx */
    }
    /* r <- 1/N; Q(r_exp) */
    r_fract = recip32x32(&r_exp, N, 0);
    /* f <- acc*1/N; Q(x+r_exp-31) <- Qx*Q(r_exp) - 31 */
    f = mulf64x32(acc, r_fract);
    /* mean <- round(f); Qx <- [Q(x+r_exp-31) + 63-r_exp] - 32 w/ rounding */
    mean = roundQ63_Q31(f<<(63-r_exp));
    for ( n=0; n<N; n++ ) {
        y[n] = L_sub_ll(x[n], mean);
    }
#endif
} /* mfcc32x32_remdc() */
