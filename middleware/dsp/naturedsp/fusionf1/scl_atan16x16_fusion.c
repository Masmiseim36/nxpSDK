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
#include "polyatan16x16q23_tbl.h"
#include "common.h"
/*-------------------------------------------------------------------------
  Arctangent 
  Functions calculate arctangent of number. Fixed point functions scale 
  output by pi which corresponds to the real phases +pi/4 and represent 
  input and output in Q15 or Q31
  NOTE:
  1.  Scalar floating point function is compatible with standard ANSI C
      routines and sets errno and exception flags accordingly

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: 2 LSB
  24x24  24-bit inputs, 24-bit output. Accuracy: 74000 (3.4e-5)
  32x32  32-bit inputs, 32-bit output. Accuracy: 42    (2.0e-8)
  f      floating point. Accuracy: 2 ULP


  Input:
  x[N]   input data, Q15, Q31 or floating point
  N      length of vectors
  Output:
  z[N]   result, Q15, Q31 or floating point

  Restriction:
  x,z should not overlap

  Scalar versions:
  ----------------
  return result, Q15, Q31 or floating point
-------------------------------------------------------------------------*/
int16_t scl_atan16x16 (int16_t x)
{
    ae_int16x4 x0;
    ae_f32x2 X0,Y0,f0;
    ae_f24x2 X20;

    X0=AE_CVTP24A16X2_LL(x,x);
    /* compute polynomial */
    X20=AE_SAT24S(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),AE_MOVF24X2_FROMINT32X2(X0)));
    Y0=polyatan16x16q23[0];
    f0=polyatan16x16q23[1]; 
    AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
    f0=polyatan16x16q23[2]; 
    AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
    f0=polyatan16x16q23[3]; 
    AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
    Y0=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); 
    /* convert result to Q15 */
    Y0=AE_SLAI32S(Y0,8);
    x0=AE_ROUND16X4F32SASYM(Y0,Y0);
    return (int16_t)AE_MOVAD16_0(x0);
}
