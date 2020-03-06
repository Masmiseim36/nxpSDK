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
    Vector operations, Math functions, 64-bit reciprocal  
    Code optimized for HiFi4
  IntegrIT, 2006-2018
*/
#include "NatureDSP_Signal_math.h"
#include "NatureDSP_types.h"
#include "common.h"

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
uint64_t scl_recip64x64 (int64_t x)
{
    int64_t ALIGN(8) res=x;
    ae_int64  lx=res;
    ae_f32x2 xx,yy,ee;
    xtbool sx,bzero=AE_EQ64(lx,AE_ZERO64());
    ae_int64  y,e,z;
    int exp;
    sx=AE_LT64(lx,AE_ZERO64());
    lx=AE_ABS64S(lx);
    exp=AE_NSA64(lx);
    lx=AE_SLAA64(lx,exp);// x in 0.5..1
    exp++;
    /* first stage use 3 iterations computing in 32-bit accuracy */
    xx=AE_TRUNCI32F64S(lx,0);
    yy=AE_SUB32((int32_t)0xBAEC0000UL,xx);
    ee=0x40000000; AE_MULSFP32X2RAS(ee,xx,yy); AE_MULAFP32X2RAS(yy,yy,AE_SLLI32(ee,1));
    ee=0x40000000; AE_MULSFP32X2RAS(ee,xx,yy); AE_MULAFP32X2RAS(yy,yy,AE_SLLI32(ee,1));
    ee=0x40000000; AE_MULSFP32X2RAS(ee,xx,yy); AE_MULAFP32X2RAS(yy,yy,AE_SLLI32(ee,1));
    y=AE_MOVINT64_FROMF32X2(AE_SEL32_LL(yy,0));
    /* last 2 iterations are done in better accuracy: 
        first one use simpler computation of 1-x*y, next 
        one adds more 8 bits for precision of this matter 
        but able to use simpler last multiply
    */
    ae_ep Aep; 
    ae_int64 A,B;
    AE_MULZAAD32USEP_HL_LH(Aep,A,AE_MOVINT32X2_FROMINT64(lx),AE_MOVINT32X2_FROMINT64(y));
    A=AE_SRAI72(Aep,A,29);
    B=AE_MUL32_HH(AE_MOVINT32X2_FROMINT64(lx),AE_MOVINT32X2_FROMINT64(y));
    B=AE_SLLI64(B,3);
    e=AE_ADD64(B,A);

    AE_MULZAAD32USEP_HL_LH(Aep,A,AE_MOVINT32X2_FROMINT64(y),AE_MOVINT32X2_FROMINT64(e));
    A=AE_SRAI72(Aep,A,32);
    AE_MULA32_HH(A,AE_MOVINT32X2_FROMINT64(y),AE_MOVINT32X2_FROMINT64(e));
    y=AE_SUB64S(y,A);

    A=AE_MUL32U_LL(AE_MOVINT32X2_FROMINT64(lx),AE_MOVINT32X2_FROMINT64(y));
    A=AE_SRLI64(A,32);
    AE_MULAAD32USEP_HL_LH(Aep,A,AE_MOVINT32X2_FROMINT64(lx),AE_MOVINT32X2_FROMINT64(y));
    A=AE_SRAI72(Aep,A,21);
    B=AE_MUL32_HH(AE_MOVINT32X2_FROMINT64(lx),AE_MOVINT32X2_FROMINT64(y));
    B=AE_SLLI64(B,11);
    e=AE_ADD64(B,A);

    z=AE_SRAI64(AE_MUL32_HL(AE_MOVINT32X2_FROMINT64(y),AE_MOVINT32X2_FROMINT64(e)),40); // just signed-by-signed 32x32 multiply
    y=AE_SUB64S(y,z);

    AE_MOVT64(y,AE_NEG64(y),sx);
    y=AE_SRLI64(AE_ADD64S(y,128),8);
    y=AE_OR64(y,AE_SLAI64(exp,56));
    AE_MOVT64(y,0x407fffffffffffffUL,bzero);
    res=(int64_t)y;
    return res;
}
