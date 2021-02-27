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
/* real matrix transpose */
void mtx_tran4x4_16x16(int16_t   * restrict z, const int16_t   *restrict x, int L)
{
  int l;
  ae_int16x4    *  pz = (ae_int16x4      *)z;
  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  ae_int16x4 x0, x1, x2, x3;
  ae_int16x4 t0, t1, t2, t3, tmp;
  ae_int16x4 z0, z1, z2, z3;
  ae_valign xa, za;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(z, sizeof(*z));
  if (L <= 0) return;
  xa = AE_LA64_PP(px);
  za = AE_ZALIGN64();
  for (l = 0; l<(L); l++)
  {
    AE_LA16X4_IP(x0, xa, px);
    AE_LA16X4_IP(x1, xa, px);
    AE_LA16X4_IP(x2, xa, px);
    AE_LA16X4_IP(x3, xa, px);

    t0 = AE_SEL16_7362(x0, x1);
    t1 = AE_SEL16_7362(x2, x3);
    t2 = AE_SEL16_5410(x0, x1);
    t3 = AE_SEL16_5410(x2, x3);
    tmp = AE_SEL16_4321(t3, t3);
    
    z0 = AE_SEL16_7632(t0, t1);
    z1 = AE_SEL16_5410(t0, t1);
    z2 = AE_SEL16_7520(t2, tmp);
    z3 = AE_SEL16_6420(t2, t3);
    AE_SA16X4_IP(z0, za, pz);
    AE_SA16X4_IP(z1, za, pz);
    AE_SA16X4_IP(z2, za, pz);
    AE_SA16X4_IP(z3, za, pz);
  }
  AE_SA64POS_FP(za, pz);
} /* mtx_tran4x4_16x16() */



