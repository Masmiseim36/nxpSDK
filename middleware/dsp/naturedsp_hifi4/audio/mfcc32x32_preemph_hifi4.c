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

#define USE_REFERENCE_CODE    0
#define ALIGN_SIZE            (XCHAL_DATA_WIDTH)

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
#endif /* USE_REFERENCE_CODE */

/*
 * Perform pre-emphasis filtering (1st order FIR), similarly to the 
 * following MATLAB code:
 *   y = filter([1,-alpha],1,[st;x]); st = x(end);
 * Input and output arguments x[N] and y[N] may refer to the same array.
 * Input:
 *   alpha      Pre-emphasis coefficient; Q15 for 32x32
 *   st         Initial filter state
 *   N          Number of samples to be processed
 *   x[N]       Input samples
 * Output:
 *   y[N]       Output samples
 *   return     Updated filter state
 * Restrictions:
 *   N          Must be a multiple of 2
 *   x[N],y[N]  Must be 8-bytes aligned
 */

fract32 mfcc32x32_preemph( fract32 * y, const fract32 * x, fract16 alpha, fract32 st, int N )
{
#if !USE_REFERENCE_CODE
    const ae_f32x2 * restrict X = (ae_f32x2*)&x[0];
    ae_f32x2 * restrict Y = (ae_f32x2*)&y[0];
    ae_f32x2 f, g, h = st;
    int n;
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(0==(N%2));
    /* 11 cycles per pipeline stage in steady state with unroll=8 */
    for ( n=0; n<(N>>1); n++ ) {
        AE_L32X2_IP(f, X, sizeof(f));
        g = AE_SEL32_LH(h, f); h = f;
        AE_MULSFP32X16X2RAS_L(f, g, AE_MOVDA16(alpha));
        AE_S32X2_IP(f, Y, sizeof(f));
    } /* n */
    return AE_MOVAD32_L(h);
#else
    int n;
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(0==(N%2));
    for ( n=0; n<N; n++ ) {
        /* Note that x[] and y[] may refer to the same array! */
        fract32 s = x[n];
        /* Qx <- Qx*Q15 - 15 w/ rounding */
        y[n] = L_sub_ll(s, mulf32x16ras(st, alpha)); 
        st = s;
    }
    return st;
#endif /* USE_REFERENCE_CODE */
} /* mfcc32x32_preemph() */
