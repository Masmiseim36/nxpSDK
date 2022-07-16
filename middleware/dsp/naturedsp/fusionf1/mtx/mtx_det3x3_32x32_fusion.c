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
/* real matrix determinant */
#define sz_i32    (int)sizeof(int32_t)
void mtx_det3x3_32x32(int32_t   * restrict d, const int32_t   *restrict x, int rsh, int L)
{
        ae_int32x2    *  pz = (ae_int32x2      *)d;
  const ae_int32x2    *  restrict px = (const ae_int32x2      *)(x);

  int l;
  ae_int32x2 x01, x23, x45, x67, x80, x12, x34, x56, x78;
  ae_int32x2 x0, x1, x2, x3, x4;
  ae_int32x2 z0, z1, z2, z3;
  ae_f64 A0, A1, A2;
  ae_f64 B0, B1, B2;
  ae_valign xa, za;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(d, sizeof(*d));
  NASSERT(rsh >= 0 && rsh <= 15);
  if (L <= 0) return;
  xa = AE_LA64_PP(px);
  za = AE_ZALIGN64();
  for (l = 0; l<(L>>1); l++)
  {
    AE_LA32X2_IP(x01, xa, px);
    AE_LA32X2_IP(x23, xa, px);
    AE_LA32X2_IP(x45, xa, px);
    AE_LA32X2_IP(x67, xa, px);
    AE_LA32X2_IP(x80, xa, px);
    AE_LA32X2_IP(x12, xa, px);
    AE_LA32X2_IP(x34, xa, px);
    AE_LA32X2_IP(x56, xa, px);
    AE_LA32X2_IP(x78, xa, px);

    A0 = AE_MULF32R_HH(x45, x80);
    AE_MULSF32R_LL(A0, x45, x67);

    B0 = AE_MULF32R_LL(x34, x78);
    AE_MULSF32R_HH(B0, x56, x78);

    A1 = AE_MULF32R_LH(x23, x80);
    AE_MULSF32R_LH(A1, x45, x67);

    B1 = AE_MULF32R_HL(x34, x78);
    AE_MULSF32R_HL(B1, x56, x56);
    
    A0 = AE_SRAA64(A0, rsh);
    A1 = AE_SRAA64(A1, rsh);
    z0 = AE_ROUND32X2F48SASYM(A0, A1);
    B0 = AE_SRAA64(B0, rsh);
    B1 = AE_SRAA64(B1, rsh);
    z2 = AE_ROUND32X2F48SASYM(B0, B1);

    A2 = AE_MULF32R_LL(x23, x67);
    AE_MULSF32R_HH(A2, x45, x67);

    B2 = AE_MULF32R_HH(x34, x78);
    AE_MULSF32R_LL(B2, x34, x56);

    A2 = AE_SRAA64(A2, rsh);
    B2 = AE_SRAA64(B2, rsh);
    z3 = AE_ROUND32X2F48SASYM(A2, B2);

    A0 = AE_MULF32R_HH(x01, z0);
    AE_MULSF32R_LL(A0, x01, z0);
    AE_MULAF32R_HH(A0, x23, z3);

    B0 = AE_MULF32R_LH(x80, z2);
    AE_MULSF32R_HL(B0, x12, z2);
    AE_MULAF32R_LL(B0, x12, z3);
   
    z0 = AE_ROUND32X2F48SASYM(A0, B0);
    AE_SA32X2_IP(z0,za,pz);
  }
  AE_SA64POS_FP(za,pz);
  if (L&1)
  {
    AE_LA32X2_IP(x0, xa, px);
    AE_LA32X2_IP(x1, xa, px);
    AE_LA32X2_IP(x2, xa, px);
    AE_LA32X2_IP(x3, xa, px);
    AE_LA32X2_IP(x4, xa, px);

    A0 = AE_MULF32R_HH(x2, x4);
    AE_MULSF32R_LL(A0, x2, x3);

    A1 = AE_MULF32R_LH(x1, x4);
    AE_MULSF32R_LH(A1, x2, x3);
    A0 = AE_SRAA64(A0, rsh);
    A1 = AE_SRAA64(A1, rsh);
    z0 = AE_ROUND32X2F48SASYM(A0, A1);

    A2 = AE_MULF32R_LL(x1, x3);
    AE_MULSF32R_HH(A2, x2, x3);

    A2 = AE_SRAA64(A2, rsh);
    z1 = AE_ROUND32X2F48SASYM(A2, A2);

    A0 = AE_MULF32R_HH(x0, z0);
    AE_MULSF32R_LL(A0, x0, z0);
    AE_MULAF32R_HH(A0, x1, z1);

    z0 = AE_ROUND32X2F48SASYM(A0, A0);
    AE_S32_L_IP(z0, castxcc(ae_int32,pz), 4);
  }
} /* mtx_det3x3_32x32() */


