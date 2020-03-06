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
void vec_rsqrt32x32(int32_t * frac,
                    int16_t * exp,
                    const int32_t * x,
                    int N)
{
  int n;
  ae_int32x2 X, Y, E, Z;
  ae_int32x2 U;
  ae_int32x2 _0x80000000, _0x20000000;
  ae_valign fw_align, fr_align;
  const ae_int32x2   * restrict pX  = (const ae_int32x2   *)x;
  const ae_int32x2   * restrict pFr = (const ae_int32x2   *)frac;
        ae_int32x2   * restrict pFw = (      ae_int32x2   *)frac;

  if (N <= 0) return;
  
  /* compute exponent and normalize inputs */
  for (n = 0; n<N; n++)
  {
    xtbool isneg;
    unsigned nsa;
    ae_int64   X64;
    AE_L32_IP(X, castxcc(ae_int32, pX), sizeof(int32_t));
    nsa = AE_NSAZ32_L(X);
    X64 = AE_MOVINT64_FROMINT32X2(X);
    isneg = AE_LE64(X64, AE_ZERO64());
    X = AE_SLAA32S(X, (nsa&~1));
    nsa = ((nsa >> 1) + 1);
    XT_MOVT(nsa, 0x1f, (isneg));
    exp[n] = (int16_t)(nsa);
    AE_S32_L_IP(X, castxcc(ae_int32, pFw), sizeof(int32_t));
  }
  __Pragma("no_reorder");
  pFw = (ae_int32x2   *)frac;
  fr_align = AE_LA64_PP(pFr);
  fw_align = AE_ZALIGN64();
  for (n = 0; n<(N>>1); n++)
  {
    xtbool2 sx, iszero;
    ae_f32x2 t;
    ae_int64 U64_l, U64_h;
    AE_LA32X2_IP(X, fr_align, pFr);/* Q31 */
    sx = AE_LT32(X, AE_ZERO32()); 
    iszero = AE_EQ32(X, AE_ZERO32());
    _0x80000000 = AE_MOVDA32(0x80000000);
    _0x20000000 = AE_MOVDA32(0x20000000);
    Y = AE_SUB32(_0x80000000, AE_SRAI32(X, 1)); /* Q30 */
    AE_MOVT32X2(Y, AE_MOVDA32(0x7fffffff), iszero);

    /*1-st iteration*/
    Z = AE_MULFP32X2RAS(Y, Y);
    t = _0x20000000; AE_MULSFP32X2RAS(t, X, Z);E = t;
    Z = AE_MULFP32X2RAS(Y, E);
#if 0
    Z = AE_ADD32(Z, Z);
    Y = AE_ADD32(Y, Z);
#else
    AE_MULAP32X2(Y, Z, AE_MOVDA32(2));
#endif
    /*2-nd iteration*/
    Z = AE_MULFP32X2RAS(Y, Y);
    t = _0x20000000; AE_MULSFP32X2RAS(t, X, Z); E = t;
    Z = AE_MULFP32X2RAS(Y, E);
#if 0
    Z = AE_ADD32(Z, Z);
    Y = AE_ADD32(Y, Z);
#else
    AE_MULAP32X2(Y, Z, AE_MOVDA32(2));
#endif
    /*3-rd iteration*/
    Z = AE_MULFP32X2RAS(Y, Y);
    t = _0x20000000; AE_MULSFP32X2RAS(t, X, Z); E = t;
    Z = AE_MULFP32X2RAS(Y, E);
#if 0
    Z = AE_ADD32(Z, Z);
    Y = AE_ADD32(Y, Z);
#else
    AE_MULAP32X2(Y, Z, AE_MOVDA32(2));
#endif

    /*last iteration will be done in bit higher precision*/
    U64_l = AE_MUL32_LL(Y, Y);
    U64_h = AE_MUL32_HH(Y, Y);
    U64_l = AE_ADD64(U64_l, 0x20000000);
    U64_h = AE_ADD64(U64_h, 0x20000000);
    U64_l = AE_SLLI64(U64_l, 2);
    U64_h = AE_SLLI64(U64_h, 2);
    U = AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(U64_h), AE_MOVINT32X2_FROMINT64(U64_l));
    Z = AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(U64_l), AE_MOVINT32X2_FROMINT64(U64_h));

    U64_l = AE_MUL32U_LL(X, U);
    X = AE_SEL32_HH(X, X);
    U64_h = AE_MUL32U_LL(X, Z);
    U64_l = AE_SRLI64(U64_l, 29);
    U64_h = AE_SRLI64(U64_h, 29);
    E = AE_SEL32_LL(AE_MOVINT32X2_FROMINT64(U64_h), AE_MOVINT32X2_FROMINT64(U64_l));
    Z = AE_MULFP32X2RAS(Y, E);
    Z = AE_ADD32(Z, AE_MOVDA32(2));
    Y = AE_SUB32S(Y, AE_SRAI32(Z, 2));
    AE_MOVT32X2(Y, AE_MOVDA32(0x80000000), sx);

    AE_SA32X2_IP(Y, fw_align, pFw);
  } 
  AE_SA64POS_FP(fw_align, pFw);
  
  if (N&1)
  {
    xtbool2 sx, iszero;
    ae_f32x2 t;
    ae_int64 U64_l;
    _0x80000000 = AE_MOVDA32(0x80000000);
    AE_L32_IP(X, castxcc(ae_int32, pFr), sizeof(int32_t));/* Q31 */
    sx = AE_LT32(X, AE_ZERO32());
    iszero = AE_EQ32(X, AE_ZERO32());

    _0x20000000 = AE_MOVDA32(0x20000000);
    Y = AE_SUB32(_0x80000000, AE_SRAI32(X, 1)); /* Q30 */
    AE_MOVT32X2(Y, AE_MOVDA32(0x7fffffff), iszero);
    /*1-st iteration*/
    Z = AE_MULFP32X2RAS(Y, Y);
    t = _0x20000000; AE_MULSFP32X2RAS(t, X, Z); E = t;
    Z = AE_MULFP32X2RAS(Y, E);
#if 0
    Z = AE_ADD32(Z, Z);
    Y = AE_ADD32(Y, Z);
#else
    AE_MULAP32X2(Y, Z, AE_MOVDA32(2));
#endif
    /*2-nd iteration*/
    Z = AE_MULFP32X2RAS(Y, Y);
    t = _0x20000000; AE_MULSFP32X2RAS(t, X, Z); E = t;
    Z = AE_MULFP32X2RAS(Y, E);
#if 0
    Z = AE_ADD32(Z, Z);
    Y = AE_ADD32(Y, Z);
#else
    AE_MULAP32X2(Y, Z, AE_MOVDA32(2));
#endif
    /*3-rd iteration*/
    Z = AE_MULFP32X2RAS(Y, Y);
    t = _0x20000000; AE_MULSFP32X2RAS(t, X, Z); E = t;
    Z = AE_MULFP32X2RAS(Y, E);
#if 0
    Z = AE_ADD32(Z, Z);
    Y = AE_ADD32(Y, Z);
#else
    AE_MULAP32X2(Y, Z, AE_MOVDA32(2));
#endif
    /*last iteration will be done in bit higher precision*/
    U64_l = AE_MUL32_LL(Y, Y);
    U64_l = AE_ADD64(U64_l, 0x20000000);
    U64_l = AE_SLLI64(U64_l, 2);
    U = AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(U64_l), AE_MOVINT32X2_FROMINT64(U64_l));

    U64_l = AE_MUL32U_LL(X, U);
    U64_l = AE_SRLI64(U64_l, 29);
    E = AE_SEL32_LL(AE_MOVINT32X2_FROMINT64(U64_l), AE_MOVINT32X2_FROMINT64(U64_l));
    Z = AE_MULFP32X2RAS(Y, E);
    Z = AE_ADD32(Z, AE_MOVDA32(2));
    Y = AE_SUB32S(Y, AE_SRAI32(Z, 2));
    AE_MOVT32X2(Y, AE_MOVDA32(0x80000000), sx);
    
    AE_S32_L_IP(Y, castxcc(ae_int32, pFw), sizeof(int32_t));
  }
} /* vec_rsqrt32x32() */
