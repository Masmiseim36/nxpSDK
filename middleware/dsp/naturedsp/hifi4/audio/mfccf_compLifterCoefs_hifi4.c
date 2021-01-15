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
/* Signal Processing Library API. */
#include "NatureDSP_Signal_math.h"
/* Constants and tabulated data. */
#include "pif_tbl.h"
/* MFCC extractor internal declarations. */
#include "mfcc_internal.h"

#if HAVE_VFPU || HAVE_FPU

#define ALIGN_SIZE     (XCHAL_DATA_WIDTH)

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

void mfccf_compLifterCoefs( float32_t * restrict lifterCoefs, int L, int N )
{
    float32_t L2f, phi, sn, cf;
    int n;
    NASSERT_ALIGN(lifterCoefs, ALIGN_SIZE);
    NASSERT(N>0); NASSERT(L>0);
    L2f = XT_FLOAT_S(L, 1); /* L/2 */
    phi = XT_DIV_S(pif.f, XT_FLOAT_S(L, 0)); /* pi/L */
    for ( n=0; n<N; n++ ) {
        /* 1+L/2*sin(pi/L*n) */
        sn = scl_sinef(XT_MUL_S(phi, XT_FLOAT_S(n, 0)));
        cf = XT_CONST_S(1); XT_MADD_S(cf, L2f, sn);
        lifterCoefs[n] = cf;
    }
} /* mfccf_compLifterCoefs() */

#endif /* HAVE_ */
