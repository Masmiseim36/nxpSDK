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

/*===========================================================================
  Scalar matematics:
  scl_recip            Reciprocal on Q31/Q15 Numbers
===========================================================================*/

/*-------------------------------------------------------------------------
  Reciprocal on Q63/Q31/Q15 Numbers
  These routines return the fractional and exponential portion of the 
  reciprocal of a vector x of Q31 or Q15 numbers. Since the reciprocal is 
  always greater than 1, it returns fractional portion frac in Q(31-exp) 
  or Q(15-exp) format and exponent exp so true reciprocal value in the 
  Q0.31/Q0.15 may be found by shifting fractional part left by exponent 
  value.

  Mantissa accuracy is 1 LSB, so relative accuracy is:
  vec_recip16x16, scl_recip16x16                   6.2e-5 
  scl_recip32x32                                   2.4e-7 
  vec_recip32x32                                   9.2e-10
  vec_recip64x64                                   2.2e-19

  Precision: 
  64x64  64-bit input, 64-bit output. 
  32x32  32-bit input, 32-bit output. 
  16x16  16-bit input, 16-bit output. 

  Input:
  x[N]    input data, Q63, Q31 or Q15
  N       length of vectors

  Output:
  frac[N] fractional part of result, Q(63-exp), Q(31-exp) or Q(15-exp)
  exp[N]  exponent of result 

  Restriction:
  x,frac,exp should not overlap

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  frac,x - aligned on 8-byte boundary
  N      - multiple of 4 and >4

  Scalar versions:
  ----------------
  Return packed value: 
  scl_recip64x64():
  bits 55:0 fractional part
  bits 63:56 exponent
  scl_recip32x32():
  bits 23:0 fractional part
  bits 31:24 exponent
  scl_recip16x16():
  bits 15:0 fractional part
  bits 31:16 exponent
-------------------------------------------------------------------------*/

uint32_t scl_recip32x32 (int32_t x)
{
/*algorithm:
  f(x) = recip(x)
Rough calculation:
  f(x) = f(x0) + f'(x0)*(x1-x0)
  1/x0 = f(x0)
  1/(x0^2) = f'(x0)
  x1 = x*2^nsa, x1 in 0.5..1
  x1-x0 = dx
Refiniment:
  err = f(x)*x - 1 ,error of calculation
  f(x) = f(x) - err*f(x), refined value
*/
    int r,ev,xnsa;
    xtbool2     sgn,inf;
    ae_int32x2  X, T;
    ae_f32x2    Y,E;

    X=AE_MOVDA32X2(x,x);
    sgn = AE_LT32(X, AE_ZERO32());
    inf = AE_EQ32(X, AE_ZERO32());
    X=AE_ABS32S(X);
    xnsa = AE_NSAZ32_L(X);
    X = AE_SLAA32S( X, xnsa );//x in 0.5..1 ,Q31
    // first approximation 3-2x,Q30
    Y = AE_SUB32(AE_MOVDA32X2(0xBFFFFFFF,0xBFFFFFFF),X);

    /*
        yi=3-2*x;
        for iter=1:4
        e=1-yi.*x;
        yi=yi+e.*yi;
        end
    */
    // 1-st iteration
    E = AE_MOVDA32X2(0x40000000,0x40000000);
    AE_MULSFP32X2RAS(E,X,Y);
    E = AE_SLAI32(E,1);
    AE_MULAFP32X2RAS(Y,Y,E);
    // 2-st iteration
    E = AE_MOVDA32X2(0x40000000,0x40000000);
    AE_MULSFP32X2RAS(E,X,Y);
    E = AE_SLAI32(E,1);
    AE_MULAFP32X2RAS(Y,Y,E);
    // 3-st iteration
    E = AE_MOVDA32X2(0x40000000,0x40000000);
    AE_MULSFP32X2RAS(E,X,Y);
    E = AE_SLAI32(E,1);
    AE_MULAFP32X2RAS(Y,Y,E);
    // 4-st iteration
    E = AE_MOVDA32X2(0x40000000,0x40000000);
    AE_MULSFP32X2RAS(E,X,Y);
    E = AE_SLAI32(E,1);
    AE_MULAFP32X2RAS(Y,Y,E);
    // apply sign and move right values if input is zero
    X=AE_NEG32(Y);
    T = Y;
    AE_MOVT32X2(T, X, sgn);
    AE_MOVT32X2(T, AE_MOVDA32X2(0x7FFFFFFF,0x7FFFFFFF), inf);
    Y = T;
    xnsa=xnsa+1; if (AE_MOVAB2(inf)) xnsa=0x20; 

    r = ((uint32_t)xnsa)<<24;
    ev = AE_MOVAD32_H(Y);      
    r |= ((uint32_t)ev)>>8;
    return r;                        
}
