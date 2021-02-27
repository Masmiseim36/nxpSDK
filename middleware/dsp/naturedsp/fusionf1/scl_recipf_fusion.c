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

#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(float32_t, scl_recipf, (float32_t x))
#else
#include "inff_tbl.h"

/*-------------------------------------------------------------------------
  Reciprocal 
  Fixed point routines return the fractional and exponential portion of the 
  reciprocal of a vector x of Q31 or Q15 numbers. Since the reciprocal is 
  always greater than 1, it returns fractional portion frac in Q(31-exp) 
  or Q(15-exp) format and exponent exp so true reciprocal value in the 
  Q0.31/Q0.15 may be found by shifting fractional part left by exponent 
  value.

  Mantissa accuracy is 1 LSB, so relative accuracy is:
  vec_recip16x16, scl_recip16x16                   6.2e-5 
  vec_recip24x24, scl_recip32x32, scl_recip24x24   2.4e-7 
  vec_recip32x32                                   9.2e-10

  Floating point routines operate with standard floating point numbers. 
  Functions return +/-infinity on zero or denormalized input and provide 
  accuracy of 1 ULP.

  Precision: 
  32x32   32-bit input, 32-bit output. 
  24x24   24-bit input, 24-bit output. 
  16x16   16-bit input, 16-bit output. 
  f       floating point


  Input:
  x[N]    input data, Q31,Q15 or floating point
  N       length of vectors

  Output:
  frac[N] fractional part of result, Q(31-exp) or Q(15-exp) (fixed point 
          functions)
  exp[N]  exponent of result (fixed point functions) 
  y[N]    result (floating point function)

  Restriction:
  x,frac,exp should not overlap

  Scalar versions:
  ----------------
  Return packed value for fixed-point functions: 
  scl_recip24x24(),scl_recip32x32():
  bits 23:0 fractional part
  bits 31:24 exponent
  scl_recip16x16():
  bits 15:0 fractional part
  bits 31:16 exponent
-------------------------------------------------------------------------*/
float32_t scl_recipf     (float32_t x)
{
    xtfloat z;
    static const union ufloat32uint32 minrecip={0x00200000};   /* 1./realmax('single') */
    ae_int32 ux,uy;
    xtbool binf,bden;
    ux=XT_AE_MOVINT32_FROMXTFLOAT(x);
    ux=AE_AND32(ux,0x80000000);
    uy=AE_OR32(ux,(int32_t)plusInff.u);
    binf=XT_OEQ_S(XT_ABS_S(x),plusInff.f);
    bden=XT_OLT_S(XT_ABS_S(x),minrecip.f);
    z=XT_RECIP_S(x);
    XT_MOVT_S(z,XT_AE_MOVXTFLOAT_FROMINT32(ux),binf);
    XT_MOVT_S(z,XT_AE_MOVXTFLOAT_FROMINT32(uy),bden);
    return (float32_t)z;
}
#endif
