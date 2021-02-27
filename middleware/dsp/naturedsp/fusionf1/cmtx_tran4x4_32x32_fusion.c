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
void cmtx_tran4x4_32x32(complex_fract32* restrict z, const complex_fract32 *restrict x, int L)
{
  int l;
  ae_int32x2    *  pz = (ae_int32x2      *)z;
  const ae_int32x2    *  restrict px = (const ae_int32x2      *)x;
  ae_int32x2 x0, x1, x2, x3, x4, x5, x6, x7;
  ae_int32x2 x8, x9, x10, x11, x12, x13, x14, x15;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(z, sizeof(*z));
  if (L <= 0) return;
  for (l = 0; l<(L); l++)
  {
    AE_L32X2_IP(x0, px, 8);
    AE_L32X2_IP(x1, px, 8);
    AE_L32X2_IP(x2, px, 8);
    AE_L32X2_IP(x3, px, 8);
    AE_L32X2_IP(x4, px, 8);
    AE_L32X2_IP(x5, px, 8);
    AE_L32X2_IP(x6, px, 8);
    AE_L32X2_IP(x7, px, 8);
    AE_L32X2_IP(x8, px, 8);
    AE_L32X2_IP(x9, px, 8);
    AE_L32X2_IP(x10, px, 8);
    AE_L32X2_IP(x11, px, 8);
    AE_L32X2_IP(x12, px, 8);
    AE_L32X2_IP(x13, px, 8);
    AE_L32X2_IP(x14, px, 8);
    AE_L32X2_IP(x15, px, 8);

    AE_S32X2_IP(x0, pz, 8);
    AE_S32X2_IP(x4, pz, 8);
    AE_S32X2_IP(x8, pz, 8);
    AE_S32X2_IP(x12, pz, 8);
    AE_S32X2_IP(x1, pz, 8);
    AE_S32X2_IP(x5, pz, 8);
    AE_S32X2_IP(x9, pz, 8);
    AE_S32X2_IP(x13, pz, 8);
    AE_S32X2_IP(x2, pz, 8);
    AE_S32X2_IP(x6, pz, 8);
    AE_S32X2_IP(x10, pz, 8);
    AE_S32X2_IP(x14, pz, 8);
    AE_S32X2_IP(x3, pz, 8);
    AE_S32X2_IP(x7, pz, 8);
    AE_S32X2_IP(x11, pz, 8);
    AE_S32X2_IP(x15, pz, 8);
  }
} /* cmtx_tran4x4_32x32() */



