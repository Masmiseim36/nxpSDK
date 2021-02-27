/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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

/*-------------------------------------------------------------------------
  Sine/Cosine 
  Fixed-point functions calculate sin(pi*x) or cos(pi*x) for numbers written 
  in Q31 or Q15 format. Return results in the same format. 
  Floating point functions compute sin(x) or cos(x)
  Two versions of functions available: regular version (vec_sine16x16, 
  vec_cosine16x16, vec_sine24x24, vec_cosine24x24, vec_sine32x32, 
  vec_cosine32x32, , vec_sinef, vec_cosinef) with arbitrary arguments and 
  faster version (vec_sine24x24_fast, vec_cosine24x24_fast, 
  vec_sine32x32_fast, vec_cosine32x32_fast) that apply some restrictions.
  NOTE:
  1.  Scalar floating point functions are compatible with standard ANSI C
      routines and set errno and exception flags accordingly
  2.  Floating point functions limit the range of allowable input values:
      [-102940.0, 102940.0] Whenever the input value does not belong to this
      range, the result is set to NaN.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: 2 LSB  
  24x24  24-bit inputs, 24-bit output. Accuracy: 74000(3.4e-5)
  32x32  32-bit inputs, 32-bit output. Accuracy: 1700 (7.9e-7)
  f      floating point. Accuracy 2 ULP

  Input:
  x[N]  input data, Q15, Q31 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31,Q15 or floating point

  Restriction:
  Regular versions (vec_sine24x24, vec_cosine24x24, vec_sine32x32, 
  vec_cosine32x32, vec_sinef, vec_cosinef):
  x,y - should not overlap

  Faster versions (vec_sine24x24_fast, vec_cosine24x24_fast, 
  vec_sine32x32_fast, vec_cosine32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result in Q31,Q15 or floating point
-------------------------------------------------------------------------*/
int16_t scl_sine16x16   (int16_t x)
{
    xtbool2 sx;
    ae_int32x2 X,Y;
    ae_f32x2   f;
    ae_f24x2   X2;
    static const int32_t sintblq23[]={-9347575>>8,170649474>>8,-1387072597>>8,1225767301>>8};
    /* take input, convert to Q23 and make reduction */
    X=AE_MOVDA32(((int32_t)x)<<16);
    sx=AE_LT32(X,0);
    X=AE_SRLI32(AE_SLAI32(X,1),8);
    X=AE_SUB24S(0x800000,AE_ABS32(AE_SUB32(X,0x800000)));
    /* compute polynomial */
    X2=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(X)));
    Y=sintblq23[0];
    f=sintblq23[1]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),X2); Y=f;
    f=sintblq23[2]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),X2); Y=f;
    f=sintblq23[3]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),X2); Y=f;
    f=X;            AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),AE_MOVF24X2_FROMINT32X2(X)); Y=f;
    /* apply sign and save */
    AE_MOVT32X2(Y,AE_NEG32(Y),sx);
    Y=AE_SLAI32S(Y,8);
    return AE_MOVAD16_0(AE_ROUND16X4F32SASYM(Y,Y));
}
