/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

/*
  NatureDSP Signal Processing Library. FIR part
    Real data circular auto-correlation, floating point
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Circular Autocorrelation 
  Estimates the auto-correlation of vector x. Returns autocorrelation of 
  length N.

  Precision: 
  16x16   16-bit data, 16-bit outputs
  24x24   24-bit data, 24-bit outputs
  32x32   32-bit data, 32-bit outputs
  f       floating point

  Input:
  x[N]      input data, Q31, Q15 or floating point
  N         length of x
  Output:
  r[N]      output data, Q31, Q15 or floating point

  Restrictions:
  x,r should not overlap
  x,r - aligned on an 8-bytes boundary
  N   - multiple of 4 and >0
-------------------------------------------------------------------------*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, fir_acorrf, (float32_t * restrict r,
                          const float32_t * restrict x,
                          int N))
#else

void fir_acorrf(     float32_t * restrict r,
               const float32_t * restrict x,
               int N )
{
    fir_xcorrf(r,x,x,N,N);
} /* fir_acorrf() */
#endif
