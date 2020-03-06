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

void mfccf_compDctMatrix( float32_t * restrict dctMtx, int M, int N, int rows, int cols, int orthNorm )
{
    float32_t alpha, beta, phi;
    int m, n;
    NASSERT_ALIGN(dctMtx, ALIGN_SIZE);
    NASSERT(0<M && M<=rows);
    NASSERT(0<N && N<=cols);
    alpha = XT_RSQRT_S(XT_FLOAT_S(N, 1));
    /* beta = (orthNorm ? sqrtf(1.f/N) : alpha); */
    beta = XT_RSQRT_S(XT_FLOAT_S(N, 0)); XT_MOVEQZ_S(beta, alpha, orthNorm);
    phi = XT_DIV_S(pif.f, XT_FLOAT_S(N, 0));
    /* First row. */
    for ( n=0; n<cols; n++ ) {
        dctMtx[n] = (n<N ? beta : 0);
    }
    /* Second to last rows. */
    for ( m=1; m<rows; m++ ) {
        for ( n=0; n<cols; n++ ) {
            if (m<M && n<N) {
                dctMtx[m*cols+n] = alpha*scl_cosinef(phi*m*(n+0.5f));
            } else {
                dctMtx[m*cols+n] = 0;
            }
        } /* n */
    } /* m */
} /* mfccf_compDctMatrix() */

#endif /* HAVE_ */
