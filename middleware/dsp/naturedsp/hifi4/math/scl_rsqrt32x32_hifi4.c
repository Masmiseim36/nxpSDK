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
  NatureDSP Signal Processing Library. Vector matematics
    Vector operations
    Optimized code for HiFi4
  IntegrIT, 2006-2018
*/

#include "NatureDSP_types.h"
#include "NatureDSP_Signal_math.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Reciprocal Square Root
  These routines return the fractional and exponential portion of the 
  reciprocal square root of a vector x of Q31 or Q15 numbers. Since the 
  reciprocal square root is always greater than 1, they return fractional 
  portion frac in Q(31-exp) or Q(15-exp) format and exponent exp so true 
  reciprocal value in the Q0.31/Q0.15 may be found by shifting fractional 
  part left by exponent value.

  Mantissa accuracy is 1 LSB, so relative accuracy is:
  vec_rsqrt16x16, scl_rsqrt16x16	6.2e-5
  scl_rsqrt32x32	                2.4e-7
  vec_rsqrt32x32	                9.2e-10

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: 2LSB
  32x32  32-bit inputs, 32-bit output. Accuracy: (2.6e-7*y+1LSB)

  Input:
  x[N]     input data, Q15, Q31 
  N        length of vectors
  Output:
  frac[N]  fractional part of result, Q(31-exp) or Q(15-exp)
  exp[N]   exponent of result 

  Restriction:
  x, fract, exp - should not overlap

  Scalar versions:
  ----------------
  Returned packed value: 
  scl_rsqrt32x32():
  bits 23…0 fractional part
  bits 31…24 exponent
  scl_rsqrt16x16():
  bits 15…0 fractional part
  bits 31…16 exponent

-------------------------------------------------------------------------*/
uint32_t scl_rsqrt32x32(int32_t x)
{
  unsigned x_nsa;
  uint32_t    r;
  ae_int32x2  X, Y, E;
  xtbool2     sgn, inf;
  ae_int32x2 _0x20000000 = AE_MOVDA32(0x20000000);

  X = AE_MOVDA32(x);
  x_nsa = AE_NSAZ32_L(X);
  x_nsa = XT_AND(x_nsa, ~1);


  sgn = AE_LT32(X, 0);
  inf = AE_EQ32(X, 0);

  X = AE_SLAA32(X, x_nsa);
  Y = AE_SUB32S(2147483647, AE_SRAI32(X, 1));
  

  XT_MOVT(x_nsa, 0x3c, XT_ORB(xtbool2_extract_0(inf), xtbool2_extract_0(sgn)));  /* return biggest exponent for <=0 input */
  AE_MOVT32X2(Y, AE_MOVDA32(0x80000000), sgn);

  E = AE_MULFP32X2RS(Y, Y);
  E = AE_MULFP32X2RS(X, E);
  E = AE_MULFP32X2RS(AE_SUB32S(_0x20000000, E), Y);
  E = AE_SLAI32S(E, 1);
  Y = AE_ADD32S(Y, E);

  E = AE_MULFP32X2RS(Y, Y);
  E = AE_MULFP32X2RS(X, E);
  E = AE_MULFP32X2RS(AE_SUB32S(_0x20000000, E), Y);
  E = AE_SLAI32S(E, 1);
  Y = AE_ADD32S(Y, E);

  E = AE_MULFP32X2RS(Y, Y);
  E = AE_MULFP32X2RS(X, E);
  E = AE_MULFP32X2RS(AE_SUB32S(_0x20000000, E), Y);
  E = AE_SLAI32S(E, 1);
  Y = AE_ADD32S(Y, E);

  E = AE_MULFP32X2RS(Y, Y);
  E = AE_MULFP32X2RS(X, E);
  E = AE_MULFP32X2RS(AE_SUB32S(_0x20000000, E), Y);
  E = AE_SLAI32S(E, 1);
  Y = AE_ADD32S(Y, E);

  r = AE_MOVAD32_H(Y);

  r = (((uint32_t)(x_nsa >> 1) + 1) << 24) | ((uint32_t)r >> 8);

  return r;
} /* scl_rsqrt32x32() */
