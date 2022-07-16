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
#include <math.h>
#include <errno.h>

#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(float32_t, scl_rsqrtf, (float32_t x))
#else
/* Inline functions for floating-point exceptions and environment control. */
#include "__fenv.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"

/*-------------------------------------------------------------------------
  Reciprocal Square Root
  These routines compute reciprocals of positive square root.
   Whenever an input value is negative, functions raise the "invalid" 
   floating-point exception, assign EDOM to errno and set output value to 
   NaN. For x[n]==+/-0, functions set output to +/-infinity, raise the 
   "divide by zero" floating-point exception, and assign ERANGE to errno.

  Precision: 
  f      floating point. Accuracy 2 ULP

  Input:
  x[N]  input data
  N     length of vectors
  Output:
  y[N]  output data

  Restriction:
  x,y - should not overlap

  Scalar versions:
  ----------------
  return result
-------------------------------------------------------------------------*/
float32_t scl_rsqrtf(float32_t x)
{
#if 0
    xtbool binf;
    float32_t z;

    if ( XT_UN_S(x,x) )
    {
        __Pragma("frequency_hint never");
        errno = EDOM;
        return x;
    }
    if ( x<0.f )
    {
        __Pragma("frequency_hint never");
        errno = EDOM;
        feraiseexcept(FE_INVALID);
        return qNaNf.f;
    }
    if ( x==0.f )
    {
        ae_int32 ux;
        __Pragma("frequency_hint never");
        errno = ERANGE;
        feraiseexcept(FE_DIVBYZERO);
        ux = XT_AE_MOVINT32_FROMXTFLOAT(x); /* return +/- inf */
        ux = AE_AND32(ux,0x80000000);
        ux = AE_OR32 (ux,0x7f800000);
        z  = XT_AE_MOVXTFLOAT_FROMINT32(ux);
        return z;
    }
    binf=XT_OEQ_S(x,plusInff.f);
    x=binf ? 1.f : x;
    z=XT_RSQRT_S(x);
    z=binf ? 0.f : z;
    return z;
#else
    xtfloat z;
    xtbool b_inf;

    if ( XT_UN_S(x,x) )
    {
        __Pragma("frequency_hint never");
        errno = EDOM;
        return qNaNf.f;
    }
    if ( x<0.f )
    {
        __Pragma("frequency_hint never");
        errno = EDOM;
        __feraiseexcept(FE_INVALID);
        return qNaNf.f;
    }
    if ( x==0.f )
    {
        ae_int32 ux;
        __Pragma("frequency_hint never");
        errno = ERANGE;
        __feraiseexcept(FE_DIVBYZERO);
        ux = XT_AE_MOVINT32_FROMXTFLOAT(x); /* return +/- inf */
        ux = AE_AND32(ux,0x80000000);
        ux = AE_OR32 (ux,0x7f800000);
        z  = XT_AE_MOVXTFLOAT_FROMINT32(ux);
        return z;
    }

    b_inf = XT_OEQ_S(x,plusInff.f);
    z = x;
    XT_MOVT_S( z, XT_CONST_S(1), b_inf );
    z = XT_RSQRT_S( z );
    XT_MOVT_S( z, XT_CONST_S(0), b_inf );

    return z;
#endif
}
#endif
