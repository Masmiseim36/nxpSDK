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
#include "common.h"


/*===========================================================================
  Scalar matematics:
  scl_recip            Reciprocal on Q31/Q15 Numbers
===========================================================================*/

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
uint32_t scl_recip16x16 (int16_t x)
#if 1
{
  int         x_nsa, ifi;
  uint32_t    r;

  ae_int64    vri, vdi, vxi;
  ae_f64      vtf;
  ae_int32x2  vxw;
  ae_int16x4  vxh, vyh, vzh;
  xtbool4     sgn, inf;

  vxw = AE_MOVI(1);
  vtf = AE_CVT64F32_H(vxw);

  vdi = (vtf);
  vdi = AE_SLAI64(vdi, 30);
  vzh = AE_MOVDA16(0);

  vxh = AE_MOVDA16(x);
  vri = vdi;

  sgn = AE_LT16(vxh, vzh);
  inf = AE_EQ16(vxh, vzh);
  ifi = (int)inf;
  vyh = AE_NEG16S(vxh);
  AE_MOVT16X4(vxh, vyh, sgn);
  vxi = AE_MOVINT64_FROMINT16X4(vxh);   // this can not be replaced with implicit type conversion because compliler may select int16_t->int64_t conversion instead !!!!

  x_nsa = AE_NSA64(vxi);

  vxh = AE_SLAA16S(vxh, x_nsa);
  ++x_nsa;
  vxw = AE_MOVINT32X2_FROMINT16X4(vxh);
  vxw = AE_CVT32X2F16_32(vxh);

  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);
  AE_DIV64D32_H(vri, vxw);

  vyh = AE_MOVINT16X4_FROMINT64(vri);
  vxh = AE_NEG16S(vyh);
  AE_MOVT16X4(vyh, vxh, sgn);

  r = (((uint32_t)x_nsa)<<16)|((uint16_t)AE_MOVAD16_0(vyh));

  if (ifi) r = 0x107fff;

  return r;
}
#else
{
    ae_int16x4 X;
    xtbool4 sx,isZero;
    ae_int16x4 Y,E,_0x4000=AE_MOVDA16(16384);
    uint32_t res;
    unsigned nsa;
    /* compute exponent and normalize inputs */
    X=AE_MOVDA16(x);
    isZero=AE_EQ16(X,AE_ZERO16());
    nsa=AE_NSAZ16_0(X);
    XT_MOVT(nsa,0x10,xtbool4_extract_0(isZero));  /* return biggest exponent for zero input */
    X=AE_SLAA16S_vector(X,nsa);
    res=((uint32_t)(nsa+1))<<16;

    sx=AE_LT16(X,AE_ZERO16());
    X=AE_ABS16S_vector(X);
    AE_MOVT16X4(X,AE_MOVDA16(32767),isZero);
    Y=AE_SUB16(AE_MOVDA16((int16_t)47852),X); /* first approximation */
    E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(X,Y));  /* 3 iterations to achieve 1 LSB accuracy in mantissa */
    E=AE_ADD16(E,E);
    Y=AE_ADD16S_vector(Y,AE_MULFP16X4S_vector(Y,E));
    E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(X,Y));
    E=AE_ADD16(E,E);
    Y=AE_ADD16S_vector(Y,AE_MULFP16X4S_vector(Y,E));
    E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(X,Y));
    E=AE_ADD16(E,E);
    Y=AE_ADD16S_vector(Y,AE_MULFP16X4S_vector(Y,E));
    X=AE_NEG16S_vector(Y); /* restore original sign */
    AE_MOVT16X4(Y,X,sx);
    res|=((uint32_t)AE_MOVAD16_0(Y))&0xffff;
    return res;
}
#endif
