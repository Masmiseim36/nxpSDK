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
#include "NatureDSP_Signal_math.h"
#include "common.h"
#include "recip_table.h"
#include "NatureDSP_Math.h"

/*===========================================================================
  Scalar matematics:
  scl_divide           Division of Q31/Q15 Numbers
===========================================================================*/

/*-------------------------------------------------------------------------
  Division
  These routines perform pair-wise division of vectors written in Q63, Q31 or 
  Q15 format. They return the fractional and exponential portion of the division 
  result. Since the division may generate result greater than 1, it returns 
  fractional portion frac in Q(63-exp), Q(31-exp) or Q(15-exp) format and 
  exponent exp so true division result in the Q0.31 may be found by shifting 
  fractional part left by exponent value.
  Additional routine makes integer division of 64-bit number to 32-bit 
  denominator forming 32-bit result. If result is overflown, 0x7fffffff 
  or 0x80000000 is returned depending on the signs of inputs.
  For division to 0, the result is not defined.

  Two versions of routines are available: regular versions (vec_divide64x32i,
  vec_divide64x64, vec_divide32x32, vec_divide16x16) work 
  with arbitrary arguments, faster versions (vec_divide32x32_fast, 
  vec_divide16x16_fast) apply some restrictions.

  Accuracy is measured as accuracy of fractional part (mantissa):
  vec_divide64x32i, scl_divide64x32                      :  1 LSB   
  vec_divide64x64                                        :  2 LSB 
  vec_divide32x32, vec_divide32x32_fast                  :  2 LSB (1.8e-9) 
  scl_divide32x32                                        :  2 LSB (4.8e-7) 
  vec_divide16x16, scl_divide16x16, vec_divide16x16_fast :  2 LSB (1.2e-4)

  Precision: 
  64x32i integer division, 64-bit nominator, 32-bit denominator, 32-bit output. 
  64x64  fractional division, 64-bit inputs, 64-bit output. 
  32x32  fractional division, 32-bit inputs, 32-bit output. 
  16x16  fractional division, 16-bit inputs, 16-bit output. 

  Input:
  x[N]    nominator, 64-bit integer, Q63, Q31 or Q15
  y[N]    denominator, 32-bit integer, Q63, Q31 or Q15
  N       length of vectors
  Output:
  frac[N] fractional parts of result, Q(63-exp), Q(31-exp) or Q(15-exp)
  exp[N]  exponents of result 

  Restriction:
  For regular versions (vec_divide64x32i, vec_divide64x64, vec_divide32x32,
  vec_divide16x16) :
  x,y,frac,exp should not overlap

  For faster versions (vec_divide32x32_fast, vec_divide16x16_fast) :
  x,y,frac,exp  should not overlap
  x,y,frac      to be aligned by 8-byte boundary, N - multiple of 4.

  Scalar versions:
  ----------------
  scl_divide64x32(): integer remainder
  Return packed value: 
  scl_divide64x64():
  bits 55:0 fractional part
  bits 63:56 exponent
  scl_divide32x32():
  bits 23:0 fractional part
  bits 31:24 exponent
  scl_divide16x16():
  bits 15:0 fractional part
  bits 31:16 exponent
-------------------------------------------------------------------------*/
uint32_t scl_divide32x32 (int32_t x, int32_t y)
{
    uint32_t res;
    ae_int32x2 X,Y,Z,E;
    ae_f32x2 t;
    xtbool2 sy;
    ae_int32x2 _0x40000000=AE_MOVDA32(0x40000000);
    int expx,expy;
    /* normalization */
    X=AE_MOVDA32(x);
    Y=AE_MOVDA32(y);
    expx = AE_NSAZ32_L(X);
    expy = AE_NSAZ32_L(Y);
    X=AE_SLAA32S(X,expx);
    Y=AE_SLAA32S(Y,expy);
    res=((uint32_t)(expy-expx+1))<<24;

    sy=AE_LT32(Y,AE_ZERO32());
    Y=AE_INT32X2_ABS32S(Y);
    /* first approximation */
    Z=AE_SUB32(AE_MOVDA32((int32_t)0xBAEC0000),Y); 
        /* 4 iterations to achieve 1 LSB accuracy in mantissa */
    t=_0x40000000; AE_MULSFP32X2RAS(t,Y,Z); E=t;
    E=AE_ADD32(E,E);
    t=Z; AE_MULAFP32X2RAS(t,Z,E); Z=t;
    t=_0x40000000; AE_MULSFP32X2RAS(t,Y,Z); E=t;
    E=AE_ADD32(E,E);
    t=Z; AE_MULAFP32X2RAS(t,Z,E); Z=t;
    t=_0x40000000; AE_MULSFP32X2RAS(t,Y,Z); E=t;
    E=AE_ADD32(E,E);
    t=Z; AE_MULAFP32X2RAS(t,Z,E); Z=t;
    /* restore original sign */
    Y=AE_INT32X2_NEG32S(Z);
    AE_MOVT32X2(Z,Y,sy);
    /* multiply by X */
    Z=AE_MULFP32X2RAS(X,Z);
    res|=((uint32_t)(AE_MOVAD32_L(Z)))>>8;
    return res;
}
