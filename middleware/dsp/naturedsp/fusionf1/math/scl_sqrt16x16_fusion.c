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
#include "polyrsqrtq23_tbl.h"
/*-------------------------------------------------------------------------
  Square Root
  These routines calculate square root.
  NOTES: 
  1. Fixed point functions return 0x80000000 (for 24 and 32-bit functions), 
     0x8000 (for 16-bit functions) on negative argument
  2. For floating point function, whenever an input value is negative, 
     functions raise the "invalid" floating-point exception, assign EDOM 
     to errno and set output value to NaN. Negative zero is considered as
     a valid input, the result is also -0
  Two versions of functions available: regular version (vec_sqrt16x16, 
  vec_sqrt24x24, vec_sqrt32x32, vec_sqrtf) with arbitrary arguments and
  faster version (vec_sqrt24x24_fast, vec_sqrt32x32_fast) that apply some 
  restrictions.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: (2 LSB)
  24x24  24-bit inputs, 24-bit output. Accuracy: (2.6e-7*y+1LSB)
  32x32  32-bit inputs, 32-bit output. Accuracy: (2.6e-7*y+1LSB)
  f      floating point. Accuracy 1 ULP

  Input:
  x[N]  input data,Q31, Q15 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31, Q15 or floating point

  Restriction:
  Regular versions (vec_sqrt24x24, vec_sqrt32x32):
  x,y - should not overlap

  Faster versions (vec_sqrt24x24_fast, vec_sqrt32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result, Q31, Q15 or floating point
-------------------------------------------------------------------------*/
int16_t scl_sqrt16x16(int16_t x )
{
    ae_int32x2 X,Y,D,R;
    ae_f32x2 f;
    xtbool2 lezero;
    int sh;
    /* load, take exponent */
    X=AE_CVTP24A16(x);
    sh=AE_NSAZ32_L(X)&~1;
    X=AE_SLAA32(X,sh-8);
    lezero=AE_LE32(X,0);
    /* compute rsqrt */
    R=polyrsqrtq23[0];
    f=polyrsqrtq23[1]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(R)); R=f;
    f=polyrsqrtq23[2]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(R)); R=f;
    f=polyrsqrtq23[3]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(R)); R=f;
    f=polyrsqrtq23[4]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(R)); R=f;
    /* reiterate rsqrt */
    R=AE_SLAI24S(AE_MOVF24X2_FROMINT32X2(R),3);
    D=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(R),AE_MOVF24X2_FROMINT32X2(R));
    f=0x80000; AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(D),AE_MOVF24X2_FROMINT32X2(X)); D=f;
    D=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(D),AE_MOVF24X2_FROMINT32X2(R));
    D=AE_SLAI32(D,3);
    R=AE_ADD24S(R,D);
    /* compute sqrt and rescale back */
    Y=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(R));
    X=AE_SLAA32S(Y,14-(sh>>1));
    AE_MOVT32X2(X,0,lezero);
    return (int16_t)(AE_MOVAD32_L(X)>>16);
}
