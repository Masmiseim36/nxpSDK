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
    Single precision floating-point variant
    C code optimized for HiF4 core
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common_fpu.h"
/* MFCC extractor internal declarations. */
#include "mfcc_internal.h"

#if HAVE_VFPU || HAVE_FPU

#define USE_REFERENCE_CODE    0
#define ALIGN_SIZE            (XCHAL_DATA_WIDTH)
#define sz_f32                sizeof(float32_t)

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

float32_t mfccf_preemph( float32_t * y, const float32_t * x, float32_t alpha, float32_t st, int N )
{
#if !USE_REFERENCE_CODE
#if HAVE_VFPU
    const xtfloatx2 * restrict X = (xtfloatx2*)&x[0];
    xtfloatx2 * restrict Y = (xtfloatx2*)&y[0];
    xtfloatx2 f, g, h = st;
    int n;
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(0==(N%2));
    /* 11 cycles per pipeline stage in steady state with unroll=8 */
    for ( n=0; n<(N>>1); n++ ) {
        XT_LSX2IP(f, X, 2*sz_f32);
        g = XT_SEL32_LH_SX2(h, f); h = f;
        XT_MSUBN_SX2(f, g, alpha);
        XT_SSX2IP(f, Y, 2*sz_f32);
    } /* n */
    return XT_LOW_S(h);
#elif HAVE_FPU
    const xtfloat * restrict X;
    xtfloat * restrict Y;
    float32_t f, g, h;
    int n;
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(0==(N%2));
    X = (xtfloat*)&x[N-1];
    Y = (xtfloat*)&y[N-1];
    XT_LSXP(f, X, -(int)sz_f32); h = f;
    /* 24 cycles per pipeline stage in steady state with unroll=8 */
    for ( n=N-1; n>0; n-- ) {
        XT_LSXP(g, X, -(int)sz_f32);
        XT_MSUB_S(f, g, alpha);
        XT_SSXP(f, Y, -(int)sz_f32); f = g;
    }
    XT_MSUB_S(f, st, alpha);
    XT_SSI(f, Y, 0);
    return h;
#endif /* HAVE_FPU */
#else /* USE_REFERENCE_CODE */
    int n;
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(0==(N%2));
    for ( n=0; n<N; n++ ) {
        /* Note that x[] and y[] may refer to the same array! */
        float32_t s = x[n];
        y[n] = s-alpha*st; st = s;
    }
    return st;
#endif
} /* mfccf_preemph() */

#endif /* HAVE_VFPU || HAVE_FPU */
