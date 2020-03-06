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
  NatureDSP Signal Processing Library. Mathematics
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
void vec_rsqrt16x16 (
                  int16_t * frac, 
                  int16_t * exp,
            const int16_t *  x, 
            int N)
{
  int n;
  ae_int16x4 X, Y;
  ae_int32x2 Z0, Z1, E0, E1;
  ae_int32x2 _0x800000, _0x200000;
  ae_valign fw_align, fr_align;
  const ae_int16x4   * restrict pX  = (const ae_int16x4   *)x;
  const ae_int16x4   * restrict pFr = (const ae_int16x4   *)frac;
        ae_int16x4   * restrict pFw = (      ae_int16x4   *)frac;

  if (N <= 0) return;

  /* compute exponent and normalize inputs */
  for (n = 0; n<N; n++)
  {
    xtbool isneg;
    ae_int64 X64;
    unsigned nsa;
    AE_L16_IP(X, castxcc(ae_int16, pX), sizeof(int16_t));
    nsa = AE_NSAZ16_0(X); 
    X64 = AE_MOVINT64_FROMINT16X4(X);
    isneg = AE_LE64(X64, AE_ZERO64());
    X = AE_SLAA16S_vector(X, (nsa&~1));
    nsa = ((nsa >> 1) + 1);
    XT_MOVT(nsa, 0xf, (isneg));
    exp[n] = (int16_t)(nsa);
    AE_S16_0_IP(X, castxcc(ae_int16, pFw), sizeof(int16_t));
  }
  __Pragma("no_reorder");
  pFw = (ae_int16x4   *)frac;
  fr_align = AE_LA64_PP(pFr);
  fw_align = AE_ZALIGN64();
  for (n = 0; n<(N >> 2); n++)
  {
    xtbool4 sx, iszero;
    ae_int32x2 X0, X1;
    ae_int32x2 Y0, Y1;
    ae_f32x2 t;
    
    AE_LA16X4_IP(X, fr_align, pFr);/* Q31 */
    sx = AE_LT16(X, AE_ZERO16());
    iszero = AE_EQ16(X, AE_ZERO16());
    _0x800000 = AE_MOVDA32(0x800000);
    _0x200000 = AE_MOVDA32(0x200000);
    X0 = AE_CVT32X2F16_10(X);
    X1 = AE_CVT32X2F16_32(X);
    X0 = AE_SRAI32(X0, 8); /* Q23 */
    X1 = AE_SRAI32(X1, 8); /* Q23 */
    Y0 = AE_SUB32(_0x800000, AE_SRAI32(X0, 1)); /* Q22 */
    Y1 = AE_SUB32(_0x800000, AE_SRAI32(X1, 1)); /* Q22 */

    /*1-st iteration*/
    E0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y0), AE_MOVF24X2_FROMINT32X2(Y0));
    E1 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y1), AE_MOVF24X2_FROMINT32X2(Y1));
    t = _0x200000; AE_MULSFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X0), AE_MOVF24X2_FROMINT32X2(E0)); E0 = t;
    t = _0x200000; AE_MULSFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X1), AE_MOVF24X2_FROMINT32X2(E1)); E1 = t;
    Z0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(E0), AE_MOVF24X2_FROMINT32X2(Y0));
    Z1 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(E1), AE_MOVF24X2_FROMINT32X2(Y1));
    Z0 = AE_ADD32(Z0, Z0);
    Z1 = AE_ADD32(Z1, Z1);
    Y0 = AE_ADD32(Y0, Z0);
    Y1 = AE_ADD32(Y1, Z1);

    /*2-st iteration*/
    E0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y0), AE_MOVF24X2_FROMINT32X2(Y0));
    E1 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y1), AE_MOVF24X2_FROMINT32X2(Y1));
    t = _0x200000; AE_MULSFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X0), AE_MOVF24X2_FROMINT32X2(E0)); E0 = t;
    t = _0x200000; AE_MULSFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X1), AE_MOVF24X2_FROMINT32X2(E1)); E1 = t;
    Z0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(E0), AE_MOVF24X2_FROMINT32X2(Y0));
    Z1 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(E1), AE_MOVF24X2_FROMINT32X2(Y1));
    Z0 = AE_ADD32(Z0, Z0);
    Z1 = AE_ADD32(Z1, Z1);
    Y0 = AE_ADD32(Y0, Z0);
    Y1 = AE_ADD32(Y1, Z1);

    /*3-st iteration*/
    E0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y0), AE_MOVF24X2_FROMINT32X2(Y0));
    E1 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y1), AE_MOVF24X2_FROMINT32X2(Y1));
    t = _0x200000; AE_MULSFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X0), AE_MOVF24X2_FROMINT32X2(E0)); E0 = t;
    t = _0x200000; AE_MULSFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X1), AE_MOVF24X2_FROMINT32X2(E1)); E1 = t;
    Z0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(E0), AE_MOVF24X2_FROMINT32X2(Y0));
    Z1 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(E1), AE_MOVF24X2_FROMINT32X2(Y1));
    Z0 = AE_ADD32(Z0, Z0);
    Z1 = AE_ADD32(Z1, Z1);
    Y0 = AE_ADD32(Y0, Z0);
    Y1 = AE_ADD32(Y1, Z1);

    Y0 = AE_SLAI32(Y0, 8);
    Y1 = AE_SLAI32(Y1, 8);
    
    Y=AE_TRUNC16X4F32((Y1), (Y0));

    AE_MOVT16X4(Y, AE_MOVDA16(0x7fff), iszero);
    AE_MOVT16X4(Y, AE_MOVDA16(0x8000), sx);
    AE_SA16X4_IP(Y, fw_align,pFw);
  }
  AE_SA64POS_FP(fw_align, pFw);
  for (n = (N&3); n>0; n--)
  {
    xtbool4 sx, iszero;
    ae_int32x2 X0;
    ae_int32x2 Y0;
    ae_f32x2 t;
    AE_L16_IP(X, castxcc(ae_int16, pFr), sizeof(int16_t));
    sx = AE_LT16(X, AE_ZERO16());
    iszero = AE_EQ16(X, AE_ZERO16());
    _0x800000 = AE_MOVDA32(0x800000);
    _0x200000 = AE_MOVDA32(0x200000);
    X0 = AE_CVT32X2F16_10(X);

    X0 = AE_SRAI32(X0, 8); /* Q23 */
    Y0 = AE_SUB32(_0x800000, AE_SRAI32(X0, 1)); /* Q22 */

    /*1-st iteration*/
    E0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y0), AE_MOVF24X2_FROMINT32X2(Y0));
    t = _0x200000; AE_MULSFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X0), AE_MOVF24X2_FROMINT32X2(E0)); E0 = t;
    Z0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(E0), AE_MOVF24X2_FROMINT32X2(Y0));
    Z0 = AE_ADD32(Z0, Z0);
    Y0 = AE_ADD32(Y0, Z0);

    /*2-st iteration*/
    E0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y0), AE_MOVF24X2_FROMINT32X2(Y0));
    t = _0x200000; AE_MULSFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X0), AE_MOVF24X2_FROMINT32X2(E0)); E0 = t;
    Z0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(E0), AE_MOVF24X2_FROMINT32X2(Y0));
    Z0 = AE_ADD32(Z0, Z0);
    Y0 = AE_ADD32(Y0, Z0);

    /*3-st iteration*/
    E0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y0), AE_MOVF24X2_FROMINT32X2(Y0));
    t = _0x200000; AE_MULSFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X0), AE_MOVF24X2_FROMINT32X2(E0)); E0 = t;
    Z0 = AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(E0), AE_MOVF24X2_FROMINT32X2(Y0));
    Z0 = AE_ADD32(Z0, Z0);
    Y0 = AE_ADD32(Y0, Z0);

    Y0 = AE_SLAI32(Y0, 8);
    Y = AE_TRUNC16X4F32((Y0), (Y0));

    AE_MOVT16X4(Y, AE_MOVDA16(0x7fff), iszero);
    AE_MOVT16X4(Y, AE_MOVDA16(0x8000), sx);

    AE_S16_0_IP(Y, castxcc(ae_int16, pFw), sizeof(int16_t));
  }
} /* vec_rsqrt16x16() */
