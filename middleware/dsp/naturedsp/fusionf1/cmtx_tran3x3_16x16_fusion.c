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
/* complex matrix transpose */
void cmtx_tran3x3_16x16(complex_fract16* restrict z, const complex_fract16 *restrict x, int L)
{
  int l;
        ae_int16x4    *  pz = (ae_int16x4      *)z;
  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  ae_int16x4 x0, x1, x2, x3, x4, x5, x6, x7, x8;
  ae_int32x2 y0, y1, y2, y3, y4, y5, y6, y7, y8;
  ae_int32x2 t0, t1, t2, t3, t4, t5, t6, t7, t8;
  ae_int16x4 z0, z1, z2, z3, z4, z5, z6, z7, z8;
  ae_valign xa, za;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(z, sizeof(*z));
  if (L <= 0) return;
  xa = AE_LA64_PP(px);
  za = AE_ZALIGN64();
  for (l = 0; l<(L>>1);l++)
  {
    AE_LA16X4_IP(x0, xa, px);
    AE_LA16X4_IP(x1, xa, px);
    AE_LA16X4_IP(x2, xa, px);
    AE_LA16X4_IP(x3, xa, px);
    AE_LA16X4_IP(x4, xa, px);
    AE_LA16X4_IP(x5, xa, px);
    AE_LA16X4_IP(x6, xa, px);
    AE_LA16X4_IP(x7, xa, px);
    AE_LA16X4_IP(x8, xa, px);

    y0 = AE_MOVINT32X2_FROMINT16X4(x0);
    y1 = AE_MOVINT32X2_FROMINT16X4(x1);
    y2 = AE_MOVINT32X2_FROMINT16X4(x2);
    y3 = AE_MOVINT32X2_FROMINT16X4(x3);
    y4 = AE_MOVINT32X2_FROMINT16X4(x4);
    y5 = AE_MOVINT32X2_FROMINT16X4(x5);
    y6 = AE_MOVINT32X2_FROMINT16X4(x6);
    y7 = AE_MOVINT32X2_FROMINT16X4(x7);
    y8 = AE_MOVINT32X2_FROMINT16X4(x8);

    t0 = AE_SEL32_HL(y0, y1);
    t1 = AE_SEL32_HL(y3, y0);
    t2 = AE_SEL32_HL(y2, y3);
    t3 = AE_SEL32_HL(y1, y2);
    t4 = y4;
    t5 = AE_SEL32_HL(y6, y7);
    t6 = AE_SEL32_HL(y5, y6);
    t7 = AE_SEL32_HL(y8, y5);
    t8 = AE_SEL32_HL(y7, y8);
    
    z0 = AE_MOVINT16X4_FROMINT32X2(t0);
    z1 = AE_MOVINT16X4_FROMINT32X2(t1);
    z2 = AE_MOVINT16X4_FROMINT32X2(t2);
    z3 = AE_MOVINT16X4_FROMINT32X2(t3);
    z4 = AE_MOVINT16X4_FROMINT32X2(t4);
    z5 = AE_MOVINT16X4_FROMINT32X2(t5);
    z6 = AE_MOVINT16X4_FROMINT32X2(t6);
    z7 = AE_MOVINT16X4_FROMINT32X2(t7);
    z8 = AE_MOVINT16X4_FROMINT32X2(t8);
    AE_SA16X4_IP(z0, za, pz);
    AE_SA16X4_IP(z1, za, pz);
    AE_SA16X4_IP(z2, za, pz);
    AE_SA16X4_IP(z3, za, pz);
    AE_SA16X4_IP(z4, za, pz);
    AE_SA16X4_IP(z5, za, pz);
    AE_SA16X4_IP(z6, za, pz);
    AE_SA16X4_IP(z7, za, pz);
    AE_SA16X4_IP(z8, za, pz);
  }
  AE_SA64POS_FP(za, pz);
  if (L&1)
  {
    AE_LA16X4_IP(x0, xa, px);
    AE_LA16X4_IP(x1, xa, px);
    AE_LA16X4_IP(x2, xa, px);
    AE_LA16X4_IP(x3, xa, px);
    AE_LA16X4_IP(x4, xa, px);
    y0 = AE_MOVINT32X2_FROMINT16X4(x0);
    y1 = AE_MOVINT32X2_FROMINT16X4(x1);
    y2 = AE_MOVINT32X2_FROMINT16X4(x2);
    y3 = AE_MOVINT32X2_FROMINT16X4(x3);
    

    t0 = AE_SEL32_HL(y0, y1);
    t1 = AE_SEL32_HL(y3, y0);
    t2 = AE_SEL32_HL(y2, y3);
    t3 = AE_SEL32_HL(y1, y2);
    x4 = AE_SHORTSWAP(x4);
    y4 = AE_MOVINT32X2_FROMINT16X4(x4);
    z0 = AE_MOVINT16X4_FROMINT32X2(t0);
    z1 = AE_MOVINT16X4_FROMINT32X2(t1);
    z2 = AE_MOVINT16X4_FROMINT32X2(t2);
    z3 = AE_MOVINT16X4_FROMINT32X2(t3);

    AE_SA16X4_IP(z0, za, pz);
    AE_SA16X4_IP(z1, za, pz);
    AE_SA16X4_IP(z2, za, pz);
    AE_SA16X4_IP(z3, za, pz);
    AE_SA64POS_FP(za, pz);
    AE_S32_L_I(y4, (ae_int32 *)pz, 0);
  }
} /* cmtx_tran3x3_16x16() */



