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
#include "polypow2q23_tbl.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Antilogarithm
  These routines calculate antilogarithm (base2, natural and base10). 32 
  and 24-bit fixed-point functions accept inputs in Q6.25 and form outputs 
  in Q16.15 format and return 0x7FFFFFFF in case of overflow and 0 in case 
  of underflow. 16-bit fixed-point functions accept inputs in Q3.12 and 
  form outputs in Q8.7 format and return 0x7FFF in case of overflow and 
  0 in case of underflow.

  Precision:
  16x16  16-bit inputs, 16-bit outputs. Accuracy: 2 LSB
  32x32  32-bit inputs, 32-bit outputs. Accuracy: 8*e-6*y+1LSB
  24x24  24-bit inputs, 24-bit outputs. Accuracy: 8*e-6*y+1LSB
  f      floating point: Accuracy: 2 ULP
  NOTE:
  1.  Although 32 and 24 bit functions provide the similar accuracy, 32-bit
      functions have better input/output resolution (dynamic range).
  2.  Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly.

  Input:
  x[N]  input data,Q6.25 (for 32 and 24-bit functions), Q3.12 (for 16-bit 
        functions) or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q16.15 (for 32 and 24-bit functions), Q8.7 (for 16-bit 
        functions) or floating point  

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  fixed point functions return result, Q16.15 (for 32 and 24-bit functions), 
  Q8.7 (for 16-bit functions) 

-------------------------------------------------------------------------*/

int16_t scl_antilogn_16x16 (int16_t x)
{
    ae_int32x2 X,Y,E;
    ae_f32x2 f;
    X=AE_MOVDA32(x)<<8;
    X=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X),6051102);
    E=AE_ADD32(AE_SRAI32(X,19),1);
    X=AE_SUB32(AE_SRLI32(AE_SLAI32(X,13),9),0x800000);

    Y=polypow2q23[0];
    f=polypow2q23[1]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),AE_MOVF24X2_FROMINT32X2(X)); Y=f;
    f=polypow2q23[2]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),AE_MOVF24X2_FROMINT32X2(X)); Y=f;
    f=polypow2q23[3]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),AE_MOVF24X2_FROMINT32X2(X)); Y=f;
    f=polypow2q23[4]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),AE_MOVF24X2_FROMINT32X2(X)); Y=f;

    Y=AE_SLAA32S(Y,AE_MOVAD32_L(E));
    return (int16_t)(((int32_t)AE_MOVAD32_L(Y))>>16);
}
