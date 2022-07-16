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

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common helper macros. */
#include "common.h"
/*-------------------------------------------------------------------------
  Operations with Small Matrices
  These functions implement basic operations under the sequence of small 
  square matrices. Fixed point data are interpreted as Q15 or Q31 and 
  results might be saturated.
  NOTE: 
  Determinant is computed recursively via minors of submatrices. So, in 
  the fixed-point routines, intermediate results might be saturated 
  although final result is in range. To avoid this saturation, right shift 
  might be applied at the first stage of computations. It means that final 
  result would be represented in Q(15-rsh) or Q(31-rsh) respectively. 
  Ad-hoc formula for rsh is rsh>=N-2 for real matrices and rsh>=N-1 for 
  complex matrices.

  Precision: 
  16x16  16-bit input, 16-bit output (real and complex)
  32x32  32-bit input, 32-bit output (real and complex)
  f      floating point (real and complex)

  Matrix dimensions are 2x2, 3x3, 4x4

  Input:
  x[L][N*N]      L input matrices
  y[L][N*N]      L input matrices (for addition, subtraction, multiply 
                 functions)
  rsh            right shift for fixed-point multiply and determinant 
                 function
  L              number of matrices
  Output:
  z[L][N*N]      L output matrices (for addition, subtraction, multiply, 
                 transpose functions)
  d[L]           determinants for L matrices (for determinant functions)

  Restrictions:
  rsh should be in range 0..15
  x,y,z should not overlap
-------------------------------------------------------------------------*/
/* complex matrix determinant */
void cmtx_det2x2_16x16(complex_fract16 * restrict d, const complex_fract16 *restrict x, int rsh, int L)
{
  int l;
  ae_int16x4    *  pz = (ae_int16x4      *)d;
  ae_int32    *  pz_ ;
  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  ae_int16x4 x0, x1, x2, x3;
  ae_int16x4 z0;
  ae_int32x2 y0, y1, y2, y3;
  ae_int64 A0, A1, A2;
  ae_int64 B0, B1, B2;
  ae_valign xa, za;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(d, sizeof(*d));
  NASSERT(rsh >= 0 && rsh <= 15);
  if (L <= 0) return; 
  xa = AE_LA64_PP(px);
  za = AE_ZALIGN64();
  for (l = 0; l<(L>>1); l++)
  {
    AE_LA16X4_IP(x0, xa, px);
    AE_LA16X4_IP(x1, xa, px);
    AE_LA16X4_IP(x2, xa, px);
    AE_LA16X4_IP(x3, xa, px);

    y0 = AE_SEXT32X2D16_32(x0);
    y1 = AE_SEXT32X2D16_10(x0);
    y2 = AE_SEXT32X2D16_32(x2);
    y3 = AE_SEXT32X2D16_10(x2);
    A0 = AE_MULZASD32X16_H1_L0(y0, x1);
    A1 = AE_MULZAAD32X16_H0_L1(y0, x1);
    AE_MULSAD32X16_H3_L2(A0, y1, x1);
    A2 = AE_MULZAAD32X16_H2_L3(y1, x1);
    A1 = AE_SUB64(A1,A2);

    B0 = AE_MULZASD32X16_H1_L0(y2, x3);
    B1 = AE_MULZAAD32X16_H0_L1(y2, x3);
    AE_MULSAD32X16_H3_L2(B0, y3, x3);
    B2 = AE_MULZAAD32X16_H2_L3(y3, x3);
    B1 = AE_SUB64(B1, B2);

    A0 = AE_SLAA64(A0, 17 - rsh);
    A1 = AE_SLAA64(A1, 17 - rsh);
    B0 = AE_SLAA64(B0, 17 - rsh);
    B1 = AE_SLAA64(B1, 17 - rsh);
    y0 = AE_ROUND32X2F48SASYM(A0, A1);
    y1 = AE_ROUND32X2F48SASYM(B0, B1);
    z0 = AE_ROUND16X4F32SASYM(y0, y1);
    AE_SA16X4_IP(z0, za, pz);
  }
  AE_SA64POS_FP(za, pz);
  if (L&1)
  {
    pz_ = (ae_int32 *)pz;
    AE_LA16X4_IP(x0, xa, px);
    AE_LA16X4_IP(x1, xa, px);

    y0 = AE_SEXT32X2D16_32(x0);
    y1 = AE_SEXT32X2D16_10(x0);
    A0 = AE_MULZASD32X16_H1_L0(y0, x1);
    A1 = AE_MULZAAD32X16_H0_L1(y0, x1);
    AE_MULSAD32X16_H3_L2(A0, y1, x1);
    A2 = AE_MULZAAD32X16_H2_L3(y1, x1);
    A1 = AE_SUB64(A1, A2);
    A0 = AE_SLAA64(A0, 17 - rsh);
    A1 = AE_SLAA64(A1, 17 - rsh);
    y0 = AE_ROUND32X2F48SASYM(A0, A1);
    z0 = AE_ROUND16X4F32SASYM(y0, y0);
    z0 = AE_SHORTSWAP(z0);
    y0 = AE_MOVINT32X2_FROMINT16X4(z0);
    AE_S32_L_IP(y0,pz_,0);
  }
} /* cmtx_det2x2_16x16() */


