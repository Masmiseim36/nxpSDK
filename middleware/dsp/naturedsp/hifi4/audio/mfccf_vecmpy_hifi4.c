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

#define ALIGN_SIZE     (XCHAL_DATA_WIDTH)

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

void mfccf_vecmpy( float32_t * z, const float32_t * x, const float32_t * y, int N )
{
    int n;
    NASSERT_ALIGN(z, ALIGN_SIZE);
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(x!=y);
    __Pragma("aligned(x, 8)");
    __Pragma("aligned(y, 8)");
    __Pragma("aligned(z, 8)");
    __Pragma("concurrent");
    /* VFPU: 6 cycles per pipeline stage in steady state with unroll=4 (w/ 2x vectorization)
     * SFPU: 16 cycles per pipeline stage in steady state with unroll=4 (w/o vectorization) */
    for ( n=0; n<N; n++ ) {
        z[n] = x[n]*y[n];
    }
} /* mfccf_vecmpy() */

#endif /* HAVE_ */
