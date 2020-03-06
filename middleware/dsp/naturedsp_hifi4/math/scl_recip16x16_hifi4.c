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

/* DSP Library API */
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
uint32_t scl_recip16x16 (int16_t x)
{
  int         x_nsa, ifi;
  uint32_t    r;

  ae_int64    vri, vdi, vxi;
  ae_f64      vtf;
  ae_int32x2  vxw;
  ae_int16x4  vxh, vyh, vzh;
  xtbool4     sgn;

  vxw = AE_MOVI(1);
  vtf = AE_CVT64F32_H(vxw);

  vdi = (vtf);
  vdi = AE_SLAI64(vdi, 30);
  vzh = AE_MOVDA16(0);

  vxh = AE_MOVDA16(x);
  vri = vdi;

  sgn = AE_LT16(vxh, vzh);
  ifi = (x==0);
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
