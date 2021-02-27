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
/* complex matrix multiply */
void cmtx_mul4x4_16x16(complex_fract16 * restrict z, const complex_fract16 *restrict x, const complex_fract16 *restrict y, int rsh, int L)
{
#if 1
  const ae_int16x4    *  restrict py = (const ae_int16x4    *)y;
        ae_int16x4    *  pz = (ae_int16x4      *)z;
  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  int l, m, n;
  ae_int16x4 vx0, vy0, vx1, vy1;
  ae_int16x4 vy2, vy3, vy4, vy5, vy6, vy7;
  ae_int32x2 x0, x1, x2, x3, t0, t1;
  ae_int16x4 vz0;
  ae_int64 A0, A1, A2, A3;
  ae_valign      y_align, x_align, z_align;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(y, sizeof(*y));
  NASSERT_ALIGN(z, sizeof(*z));
  NASSERT(rsh >= 0 && rsh <= 15);
  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);
  z_align = AE_ZALIGN64();
  if (L<=0) return;
  for (l = 0; l<(L); l++, y+=16) 
  {
    for (m = 0; m<(4); m++)
    {
      AE_LA16X4_IP(vx0, x_align, px);
      AE_LA16X4_IP(vx1, x_align, px);
      py = (const ae_int16x4    *)(y);
      y_align = AE_LA64_PP(py);
      for (n = 0; n<(2); n++)
      {
        AE_LA16X4_IP(vy0, y_align, py);
        AE_LA16X4_IP(vy1, y_align, py);
        AE_LA16X4_IP(vy2, y_align, py);
        AE_LA16X4_IP(vy3, y_align, py);
        AE_LA16X4_IP(vy4, y_align, py);
        AE_LA16X4_IP(vy5, y_align, py);
        AE_LA16X4_IP(vy6, y_align, py);
        AE_LA16X4_IP(vy7, y_align, py);

        x0 = AE_SEXT32X2D16_32(vx0);
        x1 = AE_SEXT32X2D16_10(vx0);
        x2 = AE_SEXT32X2D16_32(vx1);
        x3 = AE_SEXT32X2D16_10(vx1);        

        A0 = AE_MULZASD32X16_H3_L2(x0, vy0);
        A1 = AE_MULZAAD32X16_H2_L3(x0, vy0);
        A2 = AE_MULZASD32X16_H1_L0(x0, vy0);
        A3 = AE_MULZAAD32X16_H0_L1(x0, vy0);

        AE_MULASD32X16_H3_L2(A0, x1, vy2);
        AE_MULAAD32X16_H2_L3(A1, x1, vy2);
        AE_MULASD32X16_H1_L0(A2, x1, vy2);
        AE_MULAAD32X16_H0_L1(A3, x1, vy2);

        AE_MULASD32X16_H3_L2(A0, x2, vy4);
        AE_MULAAD32X16_H2_L3(A1, x2, vy4);
        AE_MULASD32X16_H1_L0(A2, x2, vy4);
        AE_MULAAD32X16_H0_L1(A3, x2, vy4);

        AE_MULASD32X16_H3_L2(A0, x3, vy6);
        AE_MULAAD32X16_H2_L3(A1, x3, vy6);
        AE_MULASD32X16_H1_L0(A2, x3, vy6);
        AE_MULAAD32X16_H0_L1(A3, x3, vy6);

        py = (const ae_int16x4    *)(y + 2);
        y_align = AE_LA64_PP(py);

        t0 = AE_TRUNCA32X2F64S(A0, A1, 33 - rsh);
        t1 = AE_TRUNCA32X2F64S(A2, A3, 33 - rsh);
        vz0 = AE_ROUND16X4F32SASYM(t0, t1);
        AE_SA16X4_IP(vz0, z_align, pz);
      }
    }
  }
  AE_SA64POS_FP(z_align, pz);

#else
const ae_int16x4    *  restrict py = (const ae_int16x4    *)y;
ae_int16x4    *  pz = (ae_int16x4      *)z;
const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
int l, m, n;
ae_int16x4 vx0, vy0, vx1, vy1, y0, y1;
ae_int16x4 vy2, vy3, vy4, vy5, vy6, vy7;
ae_int32x2 x0, x1, x2, x3, t0, t1;
ae_int16x4 vz0;
ae_int64 A0, A1, A2, A3, A4, A5, A6, A7;
ae_valign      y_align, x_align, z_align;
NASSERT_ALIGN(x, sizeof(*x));
NASSERT_ALIGN(y, sizeof(*y));
NASSERT_ALIGN(z, sizeof(*z));
NASSERT(rsh >= 0 && rsh <= 15);
x_align = AE_LA64_PP(px);
y_align = AE_LA64_PP(py);
z_align = AE_ZALIGN64();
if (L <= 0) return;
for (l = 0; l<(L); l++, y += 16)
{
  px = (const ae_int16x4      *)(x+16*l);
  x_align = AE_LA64_PP(px);
  for (m = 0; m<(4); m++)
  {
    py = (const ae_int16x4    *)y;
    y_align = AE_LA64_PP(py);
    AE_LA16X4_IP(vx0, x_align, px);
    AE_LA16X4_IP(vx1, x_align, px);
    AE_LA16X4_IP(vy0, y_align, py);
    AE_LA16X4_IP(vy1, y_align, py);
    AE_LA16X4_IP(vy2, y_align, py);
    AE_LA16X4_IP(vy3, y_align, py);
    AE_LA16X4_IP(vy4, y_align, py);
    AE_LA16X4_IP(vy5, y_align, py);
    AE_LA16X4_IP(vy6, y_align, py);
    AE_LA16X4_IP(vy7, y_align, py);

    x0 = AE_SEXT32X2D16_32(vx0);
    x1 = AE_SEXT32X2D16_10(vx0);
    x2 = AE_SEXT32X2D16_32(vx1);
    x3 = AE_SEXT32X2D16_10(vx1);

    A0 = AE_MULZASD32X16_H3_L2(x0, vy0);
    A1 = AE_MULZAAD32X16_H2_L3(x0, vy0);
    A2 = AE_MULZASD32X16_H1_L0(x0, vy0);
    A3 = AE_MULZAAD32X16_H0_L1(x0, vy0);

    AE_MULASD32X16_H3_L2(A0, x1, vy2);
    AE_MULAAD32X16_H2_L3(A1, x1, vy2);
    AE_MULASD32X16_H1_L0(A2, x1, vy2);
    AE_MULAAD32X16_H0_L1(A3, x1, vy2);

    AE_MULASD32X16_H3_L2(A0, x2, vy4);
    AE_MULAAD32X16_H2_L3(A1, x2, vy4);
    AE_MULASD32X16_H1_L0(A2, x2, vy4);
    AE_MULAAD32X16_H0_L1(A3, x2, vy4);

    AE_MULASD32X16_H3_L2(A0, x3, vy6);
    AE_MULAAD32X16_H2_L3(A1, x3, vy6);
    AE_MULASD32X16_H1_L0(A2, x3, vy6);
    AE_MULAAD32X16_H0_L1(A3, x3, vy6);

    A0 = AE_SLAA64(A0, 17 - rsh);
    A1 = AE_SLAA64(A1, 17 - rsh);
    A2 = AE_SLAA64(A2, 17 - rsh);
    A3 = AE_SLAA64(A3, 17 - rsh);
    t0 = AE_ROUND32X2F48SASYM(A0, A1);
    t1 = AE_ROUND32X2F48SASYM(A2, A3);
    vz0 = AE_ROUND16X4F32SASYM(t0, t1);
    AE_SA16X4_IP(vz0, z_align, pz);
  }
}
for (l = 0; l<(L); l++, y += 16)
{
  px = (const ae_int16x4      *)(x + 16 * l);
  x_align = AE_LA64_PP(px);
  for (m = 0; m<(4); m++)
  {
    py = (const ae_int16x4    *)(y+16*l);
    y_align = AE_LA64_PP(py);
    AE_LA16X4_IP(vx0, x_align, px);
    AE_LA16X4_IP(vx1, x_align, px);
    AE_LA16X4_IP(vy0, y_align, py);
    AE_LA16X4_IP(vy1, y_align, py);
    AE_LA16X4_IP(vy2, y_align, py);
    AE_LA16X4_IP(vy3, y_align, py);
    AE_LA16X4_IP(vy4, y_align, py);
    AE_LA16X4_IP(vy5, y_align, py);
    AE_LA16X4_IP(vy6, y_align, py);
    AE_LA16X4_IP(vy7, y_align, py);

    x0 = AE_SEXT32X2D16_32(vx0);
    x1 = AE_SEXT32X2D16_10(vx0);
    x2 = AE_SEXT32X2D16_32(vx1);
    x3 = AE_SEXT32X2D16_10(vx1);

    A0 = AE_MULZASD32X16_H3_L2(x0, vy1);
    A1 = AE_MULZAAD32X16_H2_L3(x0, vy1);
    A2 = AE_MULZASD32X16_H1_L0(x0, vy1);
    A3 = AE_MULZAAD32X16_H0_L1(x0, vy1);

    AE_MULASD32X16_H3_L2(A0, x1, vy3);
    AE_MULAAD32X16_H2_L3(A1, x1, vy3);
    AE_MULASD32X16_H1_L0(A2, x1, vy3);
    AE_MULAAD32X16_H0_L1(A3, x1, vy3);

    AE_MULASD32X16_H3_L2(A0, x2, vy5);
    AE_MULAAD32X16_H2_L3(A1, x2, vy5);
    AE_MULASD32X16_H1_L0(A2, x2, vy5);
    AE_MULAAD32X16_H0_L1(A3, x2, vy5);

    AE_MULASD32X16_H3_L2(A0, x3, vy7);
    AE_MULAAD32X16_H2_L3(A1, x3, vy7);
    AE_MULASD32X16_H1_L0(A2, x3, vy7);
    AE_MULAAD32X16_H0_L1(A3, x3, vy7);

    A0 = AE_SLAA64(A0, 17 - rsh);
    A1 = AE_SLAA64(A1, 17 - rsh);
    A2 = AE_SLAA64(A2, 17 - rsh);
    A3 = AE_SLAA64(A3, 17 - rsh);
    t0 = AE_ROUND32X2F48SASYM(A0, A1);
    t1 = AE_ROUND32X2F48SASYM(A2, A3);
    vz0 = AE_ROUND16X4F32SASYM(t0, t1);
    AE_SA16X4_IP(vz0, z_align, pz);
  }
}
AE_SA64POS_FP(z_align, pz);
#endif
} /* cmtx_mul4x4_16x16() */



