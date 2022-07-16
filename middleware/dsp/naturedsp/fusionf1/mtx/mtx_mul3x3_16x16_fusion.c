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
#define sz_i32    (int)sizeof(int32_t)
void mtx_mul3x3_16x16(int16_t   * restrict z, const int16_t   *restrict x, const int16_t   *restrict y, int rsh, int L)
{
#if 0
  const ae_int16x4    *  restrict py = (const ae_int16x4    *)y;
  ae_int16x4    *  pz = (ae_int16x4      *)z;

  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  int l;

  ae_int16x4 vx0, vy0, vy2, vx1, vx2, vy1;
  ae_int32x2 t0, t1, y0, y1, y2, y3, y4;
  ae_int16x4 vz0;
  ae_int64 A0, A1, A2, A3, A4, A5;
  ae_valign      y_align, x_align, z_align;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(y, sizeof(*y));
  NASSERT_ALIGN(z, sizeof(*z));
  NASSERT(rsh >= 0 && rsh <= 15);
  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);
  z_align = AE_ZALIGN64();
  if (L<=0) return;
  pz = (ae_int16x4      *)z;
  z_align = AE_ZALIGN64();
  for (l = 0; l<(L-1); l++,x+=9, y+=9, z+=9)
  {
    px = (const ae_int16x4      *)x;
    x_align = AE_LA64_PP(px);
    py = (const ae_int16x4      *)y;
    y_align = AE_LA64_PP(py);
    
    AE_LA16X4_IP(vx0, x_align, px);
    AE_LA16X4_IP(vx1, x_align, px);
    AE_LA16X4_IP(vx2, x_align, px);
    AE_LA16X4_IP(vy0, y_align, py);
    AE_LA16X4_IP(vy1, y_align, py);
    AE_LA16X4_IP(vy2, y_align, py);

    y0 = AE_SEXT32X2D16_32(vy0);
    y1 = AE_SEXT32X2D16_10(vy0);
    y2 = AE_SEXT32X2D16_32(vy1);
    y3 = AE_SEXT32X2D16_10(vy1);
    y4 = AE_SEXT32X2D16_32(vy2);

    A0 = AE_MUL32X16_H3(y0, vx0);
    AE_MULA32X16_L2(A0, y1, vx0);
    AE_MULA32X16_H1(A0, y3, vx0);

    A1 = AE_MUL32X16_L3(y0, vx0);
    AE_MULA32X16_H2(A1, y2, vx0);
    AE_MULA32X16_L1(A1, y3, vx0);

    A2 = AE_MUL32X16_H3(y1, vx0);
    AE_MULA32X16_L2(A2, y2, vx0);
    AE_MULA32X16_H1(A2, y4, vx0);

    A3 = AE_MUL32X16_H0(y0, vx0);
    AE_MULA32X16_L3(A3, y1, vx1);
    AE_MULA32X16_H2(A3, y3, vx1);

    A4 = AE_MUL32X16_L0(y0, vx0);
    AE_MULA32X16_H3(A4, y2, vx1);
    AE_MULA32X16_L2(A4, y3, vx1);

    A5 = AE_MUL32X16_H0(y1, vx0);
    AE_MULA32X16_L3(A5, y2, vx1);
    AE_MULA32X16_H2(A5, y4, vx1);

    A0 = AE_SLAA64(A0, 17 - rsh);
    A1 = AE_SLAA64(A1, 17 - rsh);
    A2 = AE_SLAA64(A2, 17 - rsh);
    A3 = AE_SLAA64(A3, 17 - rsh);
    t0 = AE_ROUND32X2F48SASYM(A0, A1);
    t1 = AE_ROUND32X2F48SASYM(A2, A3);
    vz0 = AE_ROUND16X4F32SASYM(t0, t1);
    AE_SA16X4_IP(vz0, z_align, pz);

    A0 = AE_MUL32X16_H1(y0, vx1);
    AE_MULA32X16_L0(A0, y1, vx1);
    AE_MULA32X16_H3(A0, y3, vx2);

    A1 = AE_MUL32X16_L1(y0, vx1);
    AE_MULA32X16_H0(A1, y2, vx1);
    AE_MULA32X16_L3(A1, y3, vx2);

    A2 = AE_MUL32X16_H1(y1, vx1);
    AE_MULA32X16_L0(A2, y2, vx1);
    AE_MULA32X16_H3(A2, y4, vx2);

    A4 = AE_SLAA64(A4, 17 - rsh);
    A5 = AE_SLAA64(A5, 17 - rsh);
    A0 = AE_SLAA64(A0, 17 - rsh);
    A1 = AE_SLAA64(A1, 17 - rsh);
    t0 = AE_ROUND32X2F48SASYM(A4, A5);
    t1 = AE_ROUND32X2F48SASYM(A0, A1);
    vz0 = AE_ROUND16X4F32SASYM(t0, t1);
    AE_SA16X4_IP(vz0, z_align, pz);
    AE_SA64POS_FP(z_align, pz);
    t0 = AE_TRUNCA32X2F64S(A2, A2, 33 - rsh);
    vz0 = AE_ROUND16X4F32SASYM(t0, t0);
    AE_S16_0_IP(vz0, pz, 2);
  }
  px = (const ae_int16x4      *)x;
  x_align = AE_LA64_PP(px);
  py = (const ae_int16x4      *)y;
  y_align = AE_LA64_PP(py);
  pz = (ae_int16x4      *)z;
  z_align = AE_ZALIGN64();
  AE_LA16X4_IP(vx0, x_align, px);
  AE_LA16X4_IP(vx1, x_align, px);
  AE_LA16X4_IP(vx2, x_align, px);
  AE_LA16X4_IP(vy0, y_align, py);
  AE_LA16X4_IP(vy1, y_align, py);
  AE_LA16X4_IP(vy2, y_align, py);

  y0 = AE_SEXT32X2D16_32(vy0);
  y1 = AE_SEXT32X2D16_10(vy0);
  y2 = AE_SEXT32X2D16_32(vy1);
  y3 = AE_SEXT32X2D16_10(vy1);
  y4 = AE_SEXT32X2D16_32(vy2);


  A0 = AE_MUL32X16_H3(y0, vx0);
  AE_MULA32X16_L2(A0, y1, vx0);
  AE_MULA32X16_H1(A0, y3, vx0);

  A1 = AE_MUL32X16_L3(y0, vx0);
  AE_MULA32X16_H2(A1, y2, vx0);
  AE_MULA32X16_L1(A1, y3, vx0);

  A2 = AE_MUL32X16_H3(y1, vx0);
  AE_MULA32X16_L2(A2, y2, vx0);
  AE_MULA32X16_H1(A2, y4, vx0);

  A3 = AE_MUL32X16_H0(y0, vx0);
  AE_MULA32X16_L3(A3, y1, vx1);
  AE_MULA32X16_H2(A3, y3, vx1);

  A4 = AE_MUL32X16_L0(y0, vx0);
  AE_MULA32X16_H3(A4, y2, vx1);
  AE_MULA32X16_L2(A4, y3, vx1);

  A5 = AE_MUL32X16_H0(y1, vx0);
  AE_MULA32X16_L3(A5, y2, vx1);
  AE_MULA32X16_H2(A5, y4, vx1);

  A0 = AE_SLAA64(A0, 17 - rsh);
  A1 = AE_SLAA64(A1, 17 - rsh);
  A2 = AE_SLAA64(A2, 17 - rsh);
  A3 = AE_SLAA64(A3, 17 - rsh);
  t0 = AE_ROUND32X2F48SASYM(A0, A1);
  t1 = AE_ROUND32X2F48SASYM(A2, A3);
  vz0 = AE_ROUND16X4F32SASYM(t0, t1);
  AE_SA16X4_IP(vz0, z_align, pz);

  A0 = AE_MUL32X16_H1(y0, vx1);
  AE_MULA32X16_L0(A0, y1, vx1);
  AE_MULA32X16_H3(A0, y3, vx2);

  A1 = AE_MUL32X16_L1(y0, vx1);
  AE_MULA32X16_H0(A1, y2, vx1);
  AE_MULA32X16_L3(A1, y3, vx2);

  A2 = AE_MUL32X16_H1(y1, vx1);
  AE_MULA32X16_L0(A2, y2, vx1);
  AE_MULA32X16_H3(A2, y4, vx2);

  A4 = AE_SLAA64(A4, 17 - rsh);
  A5 = AE_SLAA64(A5, 17 - rsh);
  A0 = AE_SLAA64(A0, 17 - rsh);
  A1 = AE_SLAA64(A1, 17 - rsh);
  t0 = AE_ROUND32X2F48SASYM(A4, A5);
  t1 = AE_ROUND32X2F48SASYM(A0, A1);
  vz0 = AE_ROUND16X4F32SASYM(t0, t1);
  AE_SA16X4_IP(vz0, z_align, pz);
  AE_SA64POS_FP(z_align, pz);
  t0 = AE_TRUNCA32X2F64S(A2, A2, 33 - rsh);
  vz0 = AE_ROUND16X4F32SASYM(t0, t0);
  AE_S16_0_IP(vz0, pz, 2);
#elif 0
  const ae_int16x4    *  restrict py = (const ae_int16x4    *)y;
  ae_int16x4    *  pz = (ae_int16x4      *)z;

  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  int l;

  ae_int16x4 vx0, vy0, vy2, vx1, vx2, vy1;
  ae_int32x2 t0, t1, y0, y1, y2, y3, y4;
  ae_int16x4 vz0;
  ae_int64 A0, A1, A2, A3, A4, A5;
  ae_valign      y_align, x_align, z_align;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(y, sizeof(*y));
  NASSERT_ALIGN(z, sizeof(*z));
  NASSERT(rsh >= 0 && rsh <= 15);
  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);
  z_align = AE_ZALIGN64();
  if (L<=0) return;

  for (l = 0; l<(L); l++,x+=9, y+=9, z+=9)
  {
    px = (const ae_int16x4      *)x;
    x_align = AE_LA64_PP(px);
    py = (const ae_int16x4      *)y;
    y_align = AE_LA64_PP(py);
    pz = (ae_int16x4      *)z;
    z_align = AE_ZALIGN64();
    AE_LA16X4_IP(vx0, x_align, px);
    AE_LA16X4_IP(vx1, x_align, px);
    AE_LA16X4_IP(vy0, y_align, py);
    AE_LA16X4_IP(vy1, y_align, py);
    AE_LA16X4_IP(vy2, y_align, py);

    y0 = AE_SEXT32X2D16_32(vx0);
    y1 = AE_SEXT32X2D16_10(vy0);
    y2 = AE_SEXT32X2D16_32(vy1);
    y3 = AE_SEXT32X2D16_10(vy1);
    y4 = AE_SEXT32X2D16_32(vy2);

    A0 = AE_MUL32X16_H3(y0, vx0);
    AE_MULA32X16_L2(A0, y1, vx0);
    AE_MULA32X16_H1(A0, y3, vx0);

    A1 = AE_MUL32X16_L3(y0, vx0);
    AE_MULA32X16_H2(A1, y2, vx0);
    AE_MULA32X16_L1(A1, y3, vx0);

    A2 = AE_MUL32X16_H3(y1, vx0);
    AE_MULA32X16_L2(A2, y2, vx0);
    AE_MULA32X16_H1(A2, y4, vx0);

    A3 = AE_MUL32X16_H0(y0, vx0);
    AE_MULA32X16_L3(A3, y1, vx1);
    AE_MULA32X16_H2(A3, y3, vx1);

    A4 = AE_MUL32X16_L0(y0, vx0);
    AE_MULA32X16_H3(A4, y2, vx1);
    AE_MULA32X16_L2(A4, y3, vx1);

    A5 = AE_MUL32X16_H0(y1, vx0);
    AE_MULA32X16_L3(A5, y2, vx1);
    AE_MULA32X16_H2(A5, y4, vx1);

    A0 = AE_SLAA64(A0, 17 - rsh);
    A1 = AE_SLAA64(A1, 17 - rsh);
    A2 = AE_SLAA64(A2, 17 - rsh);
    A3 = AE_SLAA64(A3, 17 - rsh);
    t0 = AE_ROUND32X2F48SASYM(A0, A1);
    t1 = AE_ROUND32X2F48SASYM(A2, A3);
    vz0 = AE_ROUND16X4F32SASYM(t0, t1);
    AE_SA16X4_IP(vz0, z_align, pz);
  }
  AE_SA64POS_FP(z_align, pz);
  for (l = 0; l<(L); l++, x += 9, y += 9, z += 9)
  {
    px = (const ae_int16x4      *)x;
    x_align = AE_LA64_PP(px);
    py = (const ae_int16x4      *)y;
    y_align = AE_LA64_PP(py);
    pz = (ae_int16x4      *)z;
    z_align = AE_ZALIGN64();

    AE_LA16X4_IP(vx1, x_align, px);
    AE_LA16X4_IP(vx2, x_align, px);
    AE_LA16X4_IP(vy0, y_align, py);
    AE_LA16X4_IP(vy1, y_align, py);
    AE_LA16X4_IP(vy2, y_align, py);

    y0 = AE_SEXT32X2D16_32(vy0);
    y1 = AE_SEXT32X2D16_10(vy0);
    y2 = AE_SEXT32X2D16_32(vy1);
    y3 = AE_SEXT32X2D16_10(vy1);
    y4 = AE_SEXT32X2D16_32(vy2);

    A0 = AE_MUL32X16_H1(y0, vx1);
    AE_MULA32X16_L0(A0, y1, vx1);
    AE_MULA32X16_H3(A0, y3, vx2);

    A1 = AE_MUL32X16_L1(y0, vx1);
    AE_MULA32X16_H0(A1, y2, vx1);
    AE_MULA32X16_L3(A1, y3, vx2);

    A2 = AE_MUL32X16_H1(y1, vx1);
    AE_MULA32X16_L0(A2, y2, vx1);
    AE_MULA32X16_H3(A2, y4, vx2);

    A4 = AE_SLAA64(A4, 17 - rsh);
    A5 = AE_SLAA64(A5, 17 - rsh);
    A0 = AE_SLAA64(A0, 17 - rsh);
    A1 = AE_SLAA64(A1, 17 - rsh);
    t0 = AE_ROUND32X2F48SASYM(A4, A5);
    t1 = AE_ROUND32X2F48SASYM(A0, A1);
    vz0 = AE_ROUND16X4F32SASYM(t0, t1);
    AE_SA16X4_IP(vz0, z_align, pz);
    AE_SA64POS_FP(z_align, pz);
    t0 = AE_TRUNCA32X2F64S(A2, A2, 33 - rsh);
    vz0 = AE_ROUND16X4F32SASYM(t0, t0);
    AE_S16_0_IP(vz0, pz, 2);
  }
  #else
  const ae_int16x4    *  restrict py = (const ae_int16x4    *)y;
        ae_int16x4    *  pz = (ae_int16x4      *)z;
  const ae_p16x2s     *          S_rd;
        ae_int16x4    * restrict S_wr;
  const ae_int16x4    *  restrict px0 = (const ae_int16x4      *)x;
  const ae_int16x4    *  restrict px1 = (const ae_int16x4      *)(x+3);
  const ae_int16x4    *  restrict px2 = (const ae_int16x4      *)(x+6);
  int l;
  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx, blkNum, blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ / (3*4*sz_i16);
  /* Allocate a fixed-size scratch area on the stack. */
  ae_int16x4 ALIGN(8) scr[3*blkSize];

  ae_int16x4 vy0, vy2, vy1, x0, x1, x2;
  ae_int32x2 t0, t1, t2, t3, t4, t5, y0, y1, y2, y3, y4;
  ae_int16x4 vz0;
  ae_int64 A0, A1, A2, A3, A4, A5;
  ae_valign      y_align, z_align;
  ae_valign xa0, xa1, xa2;
  ae_int24x2 p0, p1, p2, p3, p4;
  ae_int24x2 f0, f1, f2, f3, f4, f5;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(y, sizeof(*y));
  NASSERT_ALIGN(z, sizeof(*z));
  NASSERT(rsh >= 0 && rsh <= 15);
  y_align = AE_LA64_PP(py);
  z_align = AE_ZALIGN64();
  if (L<=0) return;

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
      ae_int16x4 z0, z1,t0, t1;
      py = (const ae_int16x4      *)(y+9*l);
      y_align = AE_LA64_PP(py);
      AE_LA16X4_IP(vy0, y_align, py);
      AE_LA16X4_IP(vy1, y_align, py);
      AE_LA16X4_IP(vy2, y_align, py);

      t0 = AE_SEL16_4321(vy0, vy0);
      t1 = AE_SEL16_5432(t0, t0);
      z0 = AE_SEL16_7362(t1, vy1);
      z1 = AE_SEL16_7610(t0, vy1);
      AE_S16X4_IP(z0, S_wr, 8);
      AE_S16X4_IP(z1, S_wr, 8);
      AE_S16X4_IP(vy2, S_wr, 8);
    }
    y = y + (blkLen * 9);
    __Pragma("no_reorder");
    S_rd = (ae_p16x2s*)(scr -1);
    pz = (ae_int16x4      *)(z );
    z_align = AE_ZALIGN64();
    AE_L16X2M_IU(y0, S_rd, 4 * 2);
    AE_L16X2M_IU(y1, S_rd, 2 * 2);
    AE_L16X2M_IU(y2, S_rd, 2 * 2);
    AE_L16X2M_IU(y3, S_rd, 2 * 2);
    AE_L16X2M_IU(y4, S_rd, 2 * 2);
    for (l = 0; l<(blkLen -1); l++)
    {
      
      px0 = (const ae_int16x4      *)(x + 9 * l);
      px1 = (const ae_int16x4      *)(x + 3+ 9 * l);
      px2 = (const ae_int16x4      *)(x + 6+ 9 * l);
      xa0 = AE_LA64_PP(px0);
      xa1 = AE_LA64_PP(px1);
      xa2 = AE_LA64_PP(px2);

      AE_LA16X4_IP(x0, xa0, px0);
      AE_LA16X4_IP(x1, xa1, px1);
      AE_LA16X4_IP(x2, xa2, px2);

      t0 = AE_SEXT32X2D16_32(x0);
      t1 = AE_SEXT32X2D16_10(x0);
      t2 = AE_SEXT32X2D16_32(x1);
      t3 = AE_SEXT32X2D16_10(x1);
      t4 = AE_SEXT32X2D16_32(x2); 
      t5 = AE_SEXT32X2D16_10(x2);

      p0 = AE_MOVINT24X2_FROMINT32X2(y0);
      p1 = AE_MOVINT24X2_FROMINT32X2(y1);
      p2 = AE_MOVINT24X2_FROMINT32X2(y2);
      p3 = AE_MOVINT24X2_FROMINT32X2(y3);
      p4 = AE_MOVINT24X2_FROMINT32X2(y4);

      f0 = AE_MOVINT24X2_FROMINT32X2(t0);
      f1 = AE_MOVINT24X2_FROMINT32X2(t1);
      f2 = AE_MOVINT24X2_FROMINT32X2(t2);
      f3 = AE_MOVINT24X2_FROMINT32X2(t3);
      f4 = AE_MOVINT24X2_FROMINT32X2(t4);
      f5 = AE_MOVINT24X2_FROMINT32X2(t5);

      A0 = AE_MULZAAD24_HL_LH(p2, f0);
      AE_MULA24_HH(A0, p3, f1);
   
      A1 = AE_MULZAAD24_HH_LL(p0, f0);
      AE_MULA24_LH(A1, p3, f1);
      A2 = AE_MULZAAD24_HH_LL(p1, f0);
      AE_MULA24_HH(A2, p4, f1);
   
      A3 = AE_MULZAAD24_HL_LH(p2, f2);
      AE_MULA24_HH(A3, p3, f3);
   
      A4 = AE_MULZAAD24_HH_LL(p0, f2);
      AE_MULA24_LH(A4, p3, f3);
   
      A5 = AE_MULZAAD24_HH_LL(p1, f2);
      AE_MULA24_HH(A5, p4, f3);

      A0 = AE_SLAA64(A0, 17 - 8 - rsh);
      A1 = AE_SLAA64(A1, 17 - 8 - rsh);
      A2 = AE_SLAA64(A2, 17 - 8 - rsh);
      A3 = AE_SLAA64(A3, 17 - 8 - rsh);
      t0 = AE_ROUND32X2F48SASYM(A0, A1);
      t1 = AE_ROUND32X2F48SASYM(A2, A3);
      vz0 = AE_ROUND16X4F32SASYM(t0, t1);
      AE_SA16X4_IP(vz0, z_align, pz);

      A0 = AE_MULZAAD24_HL_LH(p2, f4);
      AE_MULA24_HH(A0, p3, f5);

      A1 = AE_MULZAAD24_HH_LL(p0, f4);
      AE_MULA24_LH(A1, p3, f5);

      A2 = AE_MULZAAD24_HH_LL(p1, f4);
      AE_MULA24_HH(A2, p4, f5);
      A4 = AE_SLAA64(A4, 17 - 8 - rsh);
      A5 = AE_SLAA64(A5, 17 - 8 - rsh);
      A0 = AE_SLAA64(A0, 17 - 8 - rsh);
      A1 = AE_SLAA64(A1, 17 - 8 - rsh);
      t0 = AE_ROUND32X2F48SASYM(A4, A5);
      t1 = AE_ROUND32X2F48SASYM(A0, A1);

      vz0 = AE_ROUND16X4F32SASYM(t0, t1);
      AE_SA16X4_IP(vz0, z_align, pz);
      AE_SA64POS_FP(z_align, pz);
      t0 = AE_TRUNCA32X2F64S(A2, A2, 33 - 8 - rsh);
      vz0 = AE_ROUND16X4F32SASYM(t0, t0);
      AE_S16_0_IP(vz0, castxcc(ae_int16, pz),2);
      AE_L16X2M_IU(y0, S_rd, 4 * 2);
      AE_L16X2M_IU(y1, S_rd, 2 * 2);
      AE_L16X2M_IU(y2, S_rd, 2 * 2);
      AE_L16X2M_IU(y3, S_rd, 2 * 2);
      AE_L16X2M_IU(y4, S_rd, 2 * 2);
    }
    px0 = (const ae_int16x4      *)(x + 9 * l);
    px1 = (const ae_int16x4      *)(x + 3 + 9 * l);
    px2 = (const ae_int16x4      *)(x + 6 + 9 * l);
    xa0 = AE_LA64_PP(px0);
    xa1 = AE_LA64_PP(px1);
    xa2 = AE_LA64_PP(px2);

    AE_LA16X4_IP(x0, xa0, px0);
    AE_LA16X4_IP(x1, xa1, px1);
    AE_LA16X4_IP(x2, xa2, px2);

    t0 = AE_SEXT32X2D16_32(x0);
    t1 = AE_SEXT32X2D16_10(x0);
    t2 = AE_SEXT32X2D16_32(x1);
    t3 = AE_SEXT32X2D16_10(x1);
    t4 = AE_SEXT32X2D16_32(x2);
    t5 = AE_SEXT32X2D16_10(x2);

    p0 = AE_MOVINT24X2_FROMINT32X2(y0);
    p1 = AE_MOVINT24X2_FROMINT32X2(y1);
    p2 = AE_MOVINT24X2_FROMINT32X2(y2);
    p3 = AE_MOVINT24X2_FROMINT32X2(y3);
    p4 = AE_MOVINT24X2_FROMINT32X2(y4);

    f0 = AE_MOVINT24X2_FROMINT32X2(t0);
    f1 = AE_MOVINT24X2_FROMINT32X2(t1);
    f2 = AE_MOVINT24X2_FROMINT32X2(t2);
    f3 = AE_MOVINT24X2_FROMINT32X2(t3);
    f4 = AE_MOVINT24X2_FROMINT32X2(t4);
    f5 = AE_MOVINT24X2_FROMINT32X2(t5);

    A0 = AE_MULZAAD24_HL_LH(p2, f0);
    AE_MULA24_HH(A0, p3, f1);

    A1 = AE_MULZAAD24_HH_LL(p0, f0);
    AE_MULA24_LH(A1, p3, f1);
    A2 = AE_MULZAAD24_HH_LL(p1, f0);
    AE_MULA24_HH(A2, p4, f1);

    A3 = AE_MULZAAD24_HL_LH(p2, f2);
    AE_MULA24_HH(A3, p3, f3);

    A4 = AE_MULZAAD24_HH_LL(p0, f2);
    AE_MULA24_LH(A4, p3, f3);

    A5 = AE_MULZAAD24_HH_LL(p1, f2);
    AE_MULA24_HH(A5, p4, f3);

    A0 = AE_SLAA64(A0, 17 - 8 - rsh);
    A1 = AE_SLAA64(A1, 17 - 8 - rsh);
    A2 = AE_SLAA64(A2, 17 - 8 - rsh);
    A3 = AE_SLAA64(A3, 17 - 8 - rsh);
    t0 = AE_ROUND32X2F48SASYM(A0, A1);
    t1 = AE_ROUND32X2F48SASYM(A2, A3);
    vz0 = AE_ROUND16X4F32SASYM(t0, t1);
    AE_SA16X4_IP(vz0, z_align, pz);

    A0 = AE_MULZAAD24_HL_LH(p2, f4);
    AE_MULA24_HH(A0, p3, f5);

    A1 = AE_MULZAAD24_HH_LL(p0, f4);
    AE_MULA24_LH(A1, p3, f5);

    A2 = AE_MULZAAD24_HH_LL(p1, f4);
    AE_MULA24_HH(A2, p4, f5);
    A4 = AE_SLAA64(A4, 17 - 8 - rsh);
    A5 = AE_SLAA64(A5, 17 - 8 - rsh);
    A0 = AE_SLAA64(A0, 17 - 8 - rsh);
    A1 = AE_SLAA64(A1, 17 - 8 - rsh);
    t0 = AE_ROUND32X2F48SASYM(A4, A5);
    t1 = AE_ROUND32X2F48SASYM(A0, A1);

    vz0 = AE_ROUND16X4F32SASYM(t0, t1);
    AE_SA16X4_IP(vz0, z_align, pz);
    AE_SA64POS_FP(z_align, pz);
    t0 = AE_TRUNCA32X2F64S(A2, A2, 33 - 8 - rsh);
    vz0 = AE_ROUND16X4F32SASYM(t0, t0);
    AE_S16_0_IP(vz0, castxcc(ae_int16, pz), 2);
    x = x + (blkLen * 9);
    z = z + (blkLen * 9);
  }

#endif
} /* mtx_mul3x3_16x16() */


