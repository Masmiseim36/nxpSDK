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
#define sz_i16    (int)sizeof(int16_t)
void mtx_mul4x4_16x16(int16_t   * restrict z, const int16_t   *restrict x, const int16_t   *restrict y, int rsh, int L)
{
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
  const ae_int16x4    *  restrict py = (const ae_int16x4    *)y;
        ae_int16x4    *  restrict pz = (ae_int16x4      *)z;
  const ae_int16x4 *          S_rd;
        ae_int16x4 * restrict S_wr;
  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  int l;
  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx, blkNum, blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ / (16*sz_i16);
  /* Allocate a fixed-size scratch area on the stack. */
  ae_int16x4 ALIGN(8) scr[4*blkSize];

  ae_int16x4 vx0, vx1, vx2, vx3, vy0, vy1, vy2, vy3;
  ae_int16x4 vz0;
  ae_int16x4 y0, y1, y2, y3;
  ae_int32x2 t0, t1;
  ae_int64 A0, A1, A2, A3;
  ae_valign      y_align, x_align, z_align;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(y, sizeof(*y));
  NASSERT_ALIGN(z, sizeof(*z));
  NASSERT(rsh >= 0 && rsh <= 15);
  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);
  z_align = AE_ZALIGN64();

  NASSERT_ALIGN8(scr);
  /*
  * Data are processed in blocks of scratch area size. Further, the algorithm
  * implementation is splitted in order to feed the optimizing compiler with a
  * few loops of managable size.
  */

  blkNum = (L + blkSize - 1) / blkSize;

  for (blkIx = 0; blkIx<blkNum; blkIx++)
  {
    blkLen = XT_MIN(L - blkIx*blkSize, blkSize);
    S_wr = (ae_int16x4*)scr;
    for (l = 0; l<blkLen; l++)
    {
      AE_LA16X4_IP(vy0, y_align, py);
      AE_LA16X4_IP(vy1, y_align, py);
      AE_LA16X4_IP(vy2, y_align, py);
      AE_LA16X4_IP(vy3, y_align, py);
      y0 = AE_SEL16_7632(vy0, vy1);
      y1 = AE_SEL16_7632(vy2, vy3);   
      y2 = AE_SEL16_5410(vy0, vy1);
      y3 = AE_SEL16_5410(vy2, vy3);
      vy1 = AE_SEL16_6420(y0, y1);
      vy3 = AE_SEL16_6420(y2, y3);
      y1 = AE_SEL16_4321(y1, y1);
      y3 = AE_SEL16_4321(y3, y3);
      vy0 = AE_SEL16_7520(y0, y1);
      vy2 = AE_SEL16_7520(y2, y3);

      AE_S16X4_IP(vy0, S_wr, 8);
      AE_S16X4_IP(vy1, S_wr, 8);
      AE_S16X4_IP(vy2, S_wr, 8);
      AE_S16X4_IP(vy3, S_wr, 8);
    }
    __Pragma("no_reorder");
    S_rd = (ae_int16x4*)scr;
    for (l = 0; l<blkLen; l++)
    { 
      vy0=AE_L16X4_I(S_rd, 0*8);
      vy1=AE_L16X4_I(S_rd, 1*8);
      vy2=AE_L16X4_I(S_rd, 2*8);
      vy3=AE_L16X4_I(S_rd, 3*8);

      AE_LA16X4_IP(vx0, x_align, px);
      AE_LA16X4_IP(vx1, x_align, px);
      AE_LA16X4_IP(vx2, x_align, px);
      AE_LA16X4_IP(vx3, x_align, px);
      A0= AE_MULZAAAAQ16(vx0, vy0); 
      A1= AE_MULZAAAAQ16(vx0, vy1);
      A2= AE_MULZAAAAQ16(vx0, vy2);
      A3= AE_MULZAAAAQ16(vx0, vy3);
      t0 = AE_TRUNCA32X2F64S(A0, A1, 33-rsh);
      t1 = AE_TRUNCA32X2F64S(A2, A3, 33-rsh);
      vz0 = AE_ROUND16X4F32SASYM(t0,t1);
      AE_SA16X4_IP(vz0, z_align, pz);

      A0= AE_MULZAAAAQ16(vx1, vy0); 
      A1= AE_MULZAAAAQ16(vx1, vy1);
      A2= AE_MULZAAAAQ16(vx1, vy2);
      A3= AE_MULZAAAAQ16(vx1, vy3);
      A0 = AE_SLAA64(A0, 17-rsh);
      A1 = AE_SLAA64(A1, 17-rsh);
      A2 = AE_SLAA64(A2, 17-rsh);
      A3 = AE_SLAA64(A3, 17-rsh);
      t0 = AE_ROUND32X2F48SASYM(A0, A1);
      t1 = AE_ROUND32X2F48SASYM(A2, A3);
      vz0 = AE_ROUND16X4F32SASYM(t0,t1);
      AE_SA16X4_IP(vz0, z_align, pz);

      AE_L16X4_IP(vy0, S_rd, 8);
      AE_L16X4_IP(vy1, S_rd, 8);
      AE_L16X4_IP(vy2, S_rd, 8);
      AE_L16X4_IP(vy3, S_rd, 8);

      A0 = AE_MULZAAAAQ16(vx2, vy0);
      A1 = AE_MULZAAAAQ16(vx2, vy1);
      A2 = AE_MULZAAAAQ16(vx2, vy2);
      A3 = AE_MULZAAAAQ16(vx2, vy3);
      t0 = AE_TRUNCA32X2F64S(A0, A1, 33 - rsh);
      t1 = AE_TRUNCA32X2F64S(A2, A3, 33 - rsh);
      vz0 = AE_ROUND16X4F32SASYM(t0, t1);
      AE_SA16X4_IP(vz0, z_align, pz);

      A0 = AE_MULZAAAAQ16(vx3, vy0);
      A1 = AE_MULZAAAAQ16(vx3, vy1);
      A2 = AE_MULZAAAAQ16(vx3, vy2);
      A3 = AE_MULZAAAAQ16(vx3, vy3);
      t0 = AE_TRUNCA32X2F64S(A0, A1, 33 - rsh);
      t1 = AE_TRUNCA32X2F64S(A2, A3, 33 - rsh);
      vz0 = AE_ROUND16X4F32SASYM(t0, t1);
      AE_SA16X4_IP(vz0, z_align, pz);
    }
  }
  AE_SA64POS_FP(z_align, pz);
#else
  const ae_int16x4    *  restrict py = (const ae_int16x4    *)y;
  ae_int16x4    *  restrict pz = (ae_int16x4      *)z;
  const ae_int16x4 *          S_rd;
  ae_int16x4 * restrict S_wr;
  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  int l;
  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx, blkNum, blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ / (16 * sz_i16);
  /* Allocate a fixed-size scratch area on the stack. */
  ae_int16x4 ALIGN(8) scr[4 * blkSize];

  ae_int16x4 vx0, vx1, vx2, vx3, vy0, vy1, vy2, vy3;
  ae_int16x4 vz0;
  ae_int16x4 y0, y1, y2, y3;
  ae_int32x2 t0, t1;
  ae_int64 A0, A1, A2, A3;
  ae_valign      y_align, x_align, z_align;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(y, sizeof(*y));
  NASSERT_ALIGN(z, sizeof(*z));
  NASSERT(rsh >= 0 && rsh <= 15);
  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);
  z_align = AE_ZALIGN64();

  NASSERT_ALIGN8(scr);
  /*
  * Data are processed in blocks of scratch area size. Further, the algorithm
  * implementation is splitted in order to feed the optimizing compiler with a
  * few loops of managable size.
  */

  blkNum = (L + blkSize - 1) / blkSize;

  for (blkIx = 0; blkIx<blkNum; blkIx++)
  {
    blkLen = XT_MIN(L - blkIx*blkSize, blkSize);
    S_wr = (ae_int16x4*)scr;
    for (l = 0; l<blkLen; l++)
    {
      AE_LA16X4_IP(vy0, y_align, py);
      AE_LA16X4_IP(vy1, y_align, py);
      AE_LA16X4_IP(vy2, y_align, py);
      AE_LA16X4_IP(vy3, y_align, py);
      y0 = AE_SEL16_7632(vy0, vy1);
      y1 = AE_SEL16_7632(vy2, vy3);
      y2 = AE_SEL16_5410(vy0, vy1);
      y3 = AE_SEL16_5410(vy2, vy3);
      vy1 = AE_SEL16_6420(y0, y1);
      vy3 = AE_SEL16_6420(y2, y3);
      y1 = AE_SEL16_4321(y1, y1);
      y3 = AE_SEL16_4321(y3, y3);
      vy0 = AE_SEL16_7520(y0, y1);
      vy2 = AE_SEL16_7520(y2, y3);

      AE_S16X4_IP(vy0, S_wr, 8);
      AE_S16X4_IP(vy1, S_wr, 8);
      AE_S16X4_IP(vy2, S_wr, 8);
      AE_S16X4_IP(vy3, S_wr, 8);
    }
    __Pragma("no_reorder");
    S_rd = (ae_int16x4*)scr;
    for (l = 0; l<blkLen; l++)
    {
      vy0 = AE_L16X4_I(S_rd, 0 * 8);
      vy1 = AE_L16X4_I(S_rd, 1 * 8);
      vy2 = AE_L16X4_I(S_rd, 2 * 8);
      vy3 = AE_L16X4_I(S_rd, 3 * 8);

      AE_LA16X4_IP(vx0, x_align, px);
      AE_LA16X4_IP(vx1, x_align, px);
      AE_LA16X4_IP(vx2, x_align, px);
      AE_LA16X4_IP(vx3, x_align, px);


      t0 = AE_SEXT32X2D16_32(vx0);
      t1 = AE_SEXT32X2D16_10(vx0);
      A0 = AE_MULZAAD32X16_H3_L2(t0, vy0);
      AE_MULAAD32X16_H1_L0(A0, t1, vy0);
      A1 = AE_MULZAAD32X16_H3_L2(t0, vy1);
      AE_MULAAD32X16_H1_L0(A1, t1, vy1);
      A2 = AE_MULZAAD32X16_H3_L2(t0, vy2);
      AE_MULAAD32X16_H1_L0(A2, t1, vy2);
      A3 = AE_MULZAAD32X16_H3_L2(t0, vy3);
      AE_MULAAD32X16_H1_L0(A3, t1, vy3);

      t0 = AE_TRUNCA32X2F64S(A0, A1, 33 - rsh);
      t1 = AE_TRUNCA32X2F64S(A2, A3, 33 - rsh);
      vz0 = AE_ROUND16X4F32SASYM(t0, t1);
      AE_SA16X4_IP(vz0, z_align, pz);

      vy0 = AE_L16X4_I(S_rd, 0 * 8);
      vy1 = AE_L16X4_I(S_rd, 1 * 8);
      vy2 = AE_L16X4_I(S_rd, 2 * 8);
      vy3 = AE_L16X4_I(S_rd, 3 * 8);

      t0 = AE_SEXT32X2D16_32(vx1);
      t1 = AE_SEXT32X2D16_10(vx1);
      A0 = AE_MULZAAD32X16_H3_L2(t0, vy0);
      AE_MULAAD32X16_H1_L0(A0, t1, vy0);
      A1 = AE_MULZAAD32X16_H3_L2(t0, vy1);
      AE_MULAAD32X16_H1_L0(A1, t1, vy1);
      A2 = AE_MULZAAD32X16_H3_L2(t0, vy2);
      AE_MULAAD32X16_H1_L0(A2, t1, vy2);
      A3 = AE_MULZAAD32X16_H3_L2(t0, vy3);
      AE_MULAAD32X16_H1_L0(A3, t1, vy3);

      A0 = AE_SLAA64(A0, 17 - rsh);
      A1 = AE_SLAA64(A1, 17 - rsh);
      A2 = AE_SLAA64(A2, 17 - rsh);
      A3 = AE_SLAA64(A3, 17 - rsh);
      t0 = AE_ROUND32X2F48SASYM(A0, A1);
      t1 = AE_ROUND32X2F48SASYM(A2, A3);
      vz0 = AE_ROUND16X4F32SASYM(t0, t1);
      AE_SA16X4_IP(vz0, z_align, pz);

      AE_L16X4_IP(vy0, S_rd, 8);
      AE_L16X4_IP(vy1, S_rd, 8);
      AE_L16X4_IP(vy2, S_rd, 8);
      AE_L16X4_IP(vy3, S_rd, 8);


      t0 = AE_SEXT32X2D16_32(vx2);
      t1 = AE_SEXT32X2D16_10(vx2);
      A0 = AE_MULZAAD32X16_H3_L2(t0, vy0);
      AE_MULAAD32X16_H1_L0(A0, t1, vy0);
      A1 = AE_MULZAAD32X16_H3_L2(t0, vy1);
      AE_MULAAD32X16_H1_L0(A1, t1, vy1);
      A2 = AE_MULZAAD32X16_H3_L2(t0, vy2);
      AE_MULAAD32X16_H1_L0(A2, t1, vy2);
      A3 = AE_MULZAAD32X16_H3_L2(t0, vy3);
      AE_MULAAD32X16_H1_L0(A3, t1, vy3);

      t0 = AE_TRUNCA32X2F64S(A0, A1, 33 - rsh);
      t1 = AE_TRUNCA32X2F64S(A2, A3, 33 - rsh);
      vz0 = AE_ROUND16X4F32SASYM(t0, t1);
      AE_SA16X4_IP(vz0, z_align, pz);

      t0 = AE_SEXT32X2D16_32(vx3);
      t1 = AE_SEXT32X2D16_10(vx3);
      A0 = AE_MULZAAD32X16_H3_L2(t0, vy0);
      AE_MULAAD32X16_H1_L0(A0, t1, vy0);
      A1 = AE_MULZAAD32X16_H3_L2(t0, vy1);
      AE_MULAAD32X16_H1_L0(A1, t1, vy1);
      A2 = AE_MULZAAD32X16_H3_L2(t0, vy2);
      AE_MULAAD32X16_H1_L0(A2, t1, vy2);
      A3 = AE_MULZAAD32X16_H3_L2(t0, vy3);
      AE_MULAAD32X16_H1_L0(A3, t1, vy3);

      t0 = AE_TRUNCA32X2F64S(A0, A1, 33 - rsh);
      t1 = AE_TRUNCA32X2F64S(A2, A3, 33 - rsh);
      vz0 = AE_ROUND16X4F32SASYM(t0, t1);
      AE_SA16X4_IP(vz0, z_align, pz);
    }
  }
  AE_SA64POS_FP(z_align, pz);
#endif
} /* mtx_mul4x4_16x16() */



