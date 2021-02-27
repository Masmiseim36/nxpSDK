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
/* real matrix multiply */
void mtx_mul4x4_32x32(int32_t   * restrict z, const int32_t   *restrict x, const int32_t   *restrict y, int rsh, int L)
{
  const ae_int32x2    *  restrict py = (const ae_int32x2    *)y;
  const ae_int32x2    *  restrict py_ = (const ae_int32x2    *)y;
        ae_int32x2    *  pz = (ae_int32x2      *)z;
  const ae_int32x2    *  restrict px = (const ae_int32x2      *)x;
  const ae_int32x2    *  restrict px_ = (const ae_int32x2      *)x;
  int l, m;
  ae_int32x2 vx0, vy0, vx1, vy1;
  ae_int32x2 vy2, vy3;
  ae_int32x2 vy4, vy5;
  ae_int32x2 vy6, vy7;
  ae_int32x2 vz0, vz1;
  ae_f64 A0, A1, A2, A3;
  ae_valign      y_align, x_align, z_align, y_align_, x_align_;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(y, sizeof(*y));
  NASSERT_ALIGN(z, sizeof(*z));
  NASSERT(rsh >= 0 && rsh <= 15);

  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);
  x_align_ = AE_LA64_PP(px_);
  y_align_ = AE_LA64_PP(py_);
  z_align = AE_ZALIGN64();
  for (l = 0; l<L; l++,y+=4*4)
  {
    
    for (m=0;m<4;m++)
    {
      py = (const ae_int32x2    *)y;
      y_align = AE_LA64_PP(py);
      py_ = (const ae_int32x2    *)y;
      y_align_ = AE_LA64_PP(py_);

      AE_LA32X2_IP(vx0, x_align, px);
      AE_LA32X2_IP(vx1, x_align, px);

      AE_LA32X2_IP(vy0, y_align, py);
      AE_LA32X2_IP(vy1, y_align, py);
      AE_LA32X2_IP(vy2, y_align, py);
      AE_LA32X2_IP(vy3, y_align, py);
      AE_LA32X2_IP(vy4, y_align, py);
      AE_LA32X2_IP(vy5, y_align, py);
      AE_LA32X2_IP(vy6, y_align, py);
      AE_LA32X2_IP(vy7, y_align, py);

      A0 = AE_MULF32R_HH(vx0, vy0);
      AE_MULAF32R_LH(A0, vx0, vy2);
      AE_MULAF32R_HH(A0, vx1, vy4);
      AE_MULAF32R_LH(A0, vx1, vy6);
    
      A1 = AE_MULF32R_HL(vx0, vy0);
      AE_MULAF32R_LL(A1, vx0, vy2);
      AE_MULAF32R_HL(A1, vx1, vy4);
      AE_MULAF32R_LL(A1, vx1, vy6); 

      AE_LA32X2_IP(vy0, y_align_, py_);
      AE_LA32X2_IP(vy1, y_align_, py_);
      AE_LA32X2_IP(vy2, y_align_, py_);
      AE_LA32X2_IP(vy3, y_align_, py_);

      A2 = AE_MULF32R_HH(vx0, vy1);
      AE_MULAF32R_LH(A2, vx0, vy3);
      AE_MULAF32R_HH(A2, vx1, vy5);
      AE_MULAF32R_LH(A2, vx1, vy7);

      A3 = AE_MULF32R_HL(vx0, vy1);
      AE_MULAF32R_LL(A3, vx0, vy3);
      AE_MULAF32R_HL(A3, vx1, vy5);
      AE_MULAF32R_LL(A3, vx1, vy7);
      A0 = AE_SRAA64(A0, rsh);
      A1 = AE_SRAA64(A1, rsh);
      A2 = AE_SRAA64(A2, rsh);
      A3 = AE_SRAA64(A3, rsh);
      vz0 = AE_ROUND32X2F48SASYM(A0, A1);
      vz1 = AE_ROUND32X2F48SASYM(A2, A3);
      AE_SA32X2_IP(vz0, z_align, pz);
      AE_SA32X2_IP(vz1, z_align, pz);
    }
  }
  AE_SA64POS_FP(z_align, pz);
} /* mtx_mul4x4_32x32() */


