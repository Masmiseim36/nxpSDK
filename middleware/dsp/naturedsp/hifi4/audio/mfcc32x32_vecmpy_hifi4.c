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
/* MFCC extractor internal declarations. */
#include "mfcc_internal.h"

#define USE_REFERENCE_CODE  0
#define ALIGN_SIZE          (XCHAL_DATA_WIDTH)

#if USE_REFERENCE_CODE
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
#endif

/*
 * Pairwise multiplication of input vector arguments x[n] and y[n], with 
 * resulting values stored to the output argument z[N]. Input arguments 
 * x[N] nad y[N] must be distinct, but the output argument z[N] may refer
 * to any of input arguments.
 * Input:
 *   N          Vectors size
 *   x[N]       First multiplicand; Qx for 32x32
 *   y[N]       Second multiplicand; Qy for 32x32
 * Output:
 *   z[N]       Pairwise multiplication results; Q(x+y-31) for 32x32
 * Restrictions:
 *   x[N],y[N]  Must not overlap, and must be aligned by 8-bytes
 *   z[N]       Must be aligned by 8-bytes
 */

void mfcc32x32_vecmpy( fract32 * z, const fract32 * x, const fract32 * y, int N )
{
#if !USE_REFERENCE_CODE
    const ae_f32x2 * restrict X;
    const ae_f32x2 * restrict Y;
    ae_f32x2 * restrict Z;
    ae_f32x2 a, b, c;
    int n;
    NASSERT_ALIGN(z, ALIGN_SIZE);
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(x!=y);
    X = (ae_f32x2*)x;
    Y = (ae_f32x2*)y;
    Z = (ae_f32x2*)z;
    /* 3 cycles per pipeline stage in steady state with unroll=2 */
    for ( n=0; n<(N>>1); n++ ) {
        AE_L32X2_IP(a, X, sizeof(*X));
        AE_L32X2_IP(b, Y, sizeof(*Y));
        /* Q(x+y-31) <- Qx*Qy - 31 w/ rounding */
        c = AE_MULFP32X2RAS(a, b);
        AE_S32X2_IP(c, Z, sizeof(*Z));
    }
    if (0!=(N&1)) {
        a = AE_L32_I((ae_int32*)X, 0);
        b = AE_L32_I((ae_int32*)Y, 0);
        c = AE_MULFP32X2RAS(a, b);
        AE_S32_L_I(c, (ae_int32*)Z, 0);
    }
#else
    int n;
    NASSERT_ALIGN(z, ALIGN_SIZE);
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(x!=y);
    for ( n=0; n<N; n++ ) {
        /* Q(x+y-31) <- Qx*Qy - 31 w/ rounding */
        z[n] = mulf32x32ras(x[n], y[n]);
    }
#endif /* USE_REFERENCE_CODE */
} /* mfcc32x32_vecmpy() */
