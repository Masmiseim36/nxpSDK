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
#define sz_i16    (int)sizeof(int16_t)
void mtx_det4x4_16x16(int16_t   * restrict d, const int16_t   *restrict x, int rsh, int L)
{
  int l;
  ae_int16x4    *  pz = (ae_int16x4      *)d;
  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  const ae_int16x4    *          S_rd0;
  const ae_int16x4    *          S_rd1;
  ae_int16x4    * restrict S_wr0;
  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx, blkNum, blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ / (8*4*sz_i16);
  /* Allocate a fixed-size scratch area on the stack. */
  int16_t ALIGN(8) scr[8*4 * blkSize];

  ae_int16x4 x0, x1, x2, x3;
  ae_int16x4 z0, z1, z2, z3, z4, z5, z6, z7;
  ae_int32x2 y0, y1, y2, y3, y4, y5;
  
  ae_int64 A0, A1, A2, B1, B2, C2;
  ae_valign xa;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(d, sizeof(*d));
  NASSERT(rsh >= 0 && rsh <= 15);
  if (L <= 0) return;
  xa = AE_LA64_PP(px);

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
    S_wr0 = (ae_int16x4*)scr;
    for (l = 0; l<blkLen; l++)
    {
      ae_int16x4 t0, t1, t2, t3, t4, t5;
      AE_LA16X4_IP(x0, xa, px);//0123
      AE_LA16X4_IP(x1, xa, px);//4567
      AE_LA16X4_IP(x2, xa, px);//8 9 10 11
      AE_LA16X4_IP(x3, xa, px);//12 13 14 15

      t0 = AE_SEL16_7520(x2, x2); // 8 10 9 11
      t1 = AE_SEL16_6543(x2, x2); // 9 10 11 8
      t2 = AE_SEL16_6543(x3, x3); // 13 14 15 12
      t3 = AE_SHORTSWAP(t1); //8 11 10 9
      t4 = AE_SHORTSWAP(t2); //12 15 14 13
      t5 = AE_SEL16_7520(x3, x3); // 12 14 13 15


      z0 = AE_SEL16_5410(x2, t0); // 10 11 9 11
      z1 = AE_SEL16_7632(t1, t3); // 9 10 8 11
      z2 = AE_SEL16_7632(t0, x2); // 8 10 8 9
      z3 = AE_SEL16_5410(t5, x3); // 13 15 14 15
      z3 = AE_SHORTSWAP(z3); // 15 14 15 13
      z4 = AE_SEL16_5410(t4, t2); // 14 13 15 12
      z5 = AE_SEL16_7632(x3, t5); // 12 13 12 14
      z5 = AE_SHORTSWAP(z5); // 14 12 13 12
      z6 = x1;
      z7 = x0;
      AE_S16X4_IP(z0, S_wr0, 8);
      AE_S16X4_IP(z1, S_wr0, 8);
      AE_S16X4_IP(z2, S_wr0, 8);
      AE_S16X4_IP(z3, S_wr0, 8);
      AE_S16X4_IP(z4, S_wr0, 8);
      AE_S16X4_IP(z5, S_wr0, 8);
      AE_S16X4_IP(z6, S_wr0, 8);
      AE_S16X4_IP(z7, S_wr0, 8);
    }
    __Pragma("no_reorder");
    S_rd0 = (ae_int16x4*)scr;
    S_rd1 = (ae_int16x4*)(scr-2);
    for (l = 0; l<blkLen; l++)
    {
      ae_int32x2 t0, t1, t2, t3, t4;
      AE_L16X4_IP(x0, S_rd0, 4*2);//10 11 9 11
      AE_L16X4_IP(x1, S_rd0, 4*2);//9 10 8 11
      AE_L16X4_IP(x2, S_rd0, 6*4*2);//8 10 8 9

      AE_L16X2M_IU(y0, castxcc(const ae_p16x2s,S_rd1), 14 * 2); //15 14 
      AE_L16X2M_IU(y1, castxcc(const ae_p16x2s,S_rd1), 2 * 2); // 15 13
      AE_L16X2M_IU(y2, castxcc(const ae_p16x2s,S_rd1), 2 * 2); // 14 13
      AE_L16X2M_IU(y3, castxcc(const ae_p16x2s,S_rd1), 2 * 2); // 15 12
      AE_L16X2M_IU(y4, castxcc(const ae_p16x2s,S_rd1), 2 * 2); // 14 12
      AE_L16X2M_IU(y5, castxcc(const ae_p16x2s,S_rd1), 2 * 2); // 13 12

      A0 = AE_MULZASD32X16_H3_L2(y0, x0);
      A1 = AE_MULZASD32X16_H1_L0(y1, x0);
      A2 = AE_MULZASD32X16_H3_L2(y2, x1);
      B1 = AE_MULZASD32X16_H1_L0(y3, x1);
      B2 = AE_MULZASD32X16_H3_L2(y4, x2);
      C2 = AE_MULZASD32X16_H1_L0(y5, x2);

      t0 = AE_TRUNCA32X2F64S(A1, A2, 33 - 8 - rsh);
      t1 = AE_TRUNCA32X2F64S(B1, B2, 33 - 8 - rsh);
      t2 = AE_TRUNCA32X2F64S(A1, B1, 33 - 8 - rsh);
      t3 = AE_TRUNCA32X2F64S(A2, B2, 33 - 8 - rsh);
      t4 = AE_TRUNCA32X2F64S(A0, C2, 33 - 8 - rsh);

      z0 = AE_ROUND16X4F32SASYM(t0, t1);// A1 A2 B1 B2
      z1 = AE_ROUND16X4F32SASYM(t2, t3);// A1 B2 A2 B2
      z2 = AE_ROUND16X4F32SASYM(t4, t4);// A0 C2

      AE_L16X2M_IU(y0, castxcc(const ae_p16x2s,S_rd1), 2 * 2); // 4 5
      AE_L16X2M_IU(y1, castxcc(const ae_p16x2s,S_rd1), 2 * 2); // 6 7

      A0 = AE_MULZSAD32X16_H3_L2(y1, z0);
      A1 = AE_MULZSAD32X16_H1_L0(y1, z0);
      A2 = AE_MULZASD32X16_H3_L2(y0, z1);
      B1 = AE_MULZASD32X16_H1_L0(y0, z1);

      AE_MULA32X16_L3(A0, y0, z2);
      AE_MULA32X16_H3(A1, y0, z2);
      AE_MULA32X16_L2(A2, y1, z2);
      AE_MULA32X16_H2(B1, y1, z2);
      t0 = AE_TRUNCA32X2F64S(A0, A1, 33 - 8);
      t1 = AE_TRUNCA32X2F64S(A2, B1, 33 - 8);
      z0 = AE_ROUND16X4F32SASYM(t0, t1);// M0 M1 M2 M3

      AE_L16X2M_IU(y0, castxcc(const ae_p16x2s,S_rd1), 2 * 2); // 0 1
      AE_L16X2M_IU(y1, castxcc(const ae_p16x2s,S_rd1), 2 * 2); // 2 3

      A0 = AE_MULZASD32X16_H3_L2(y0, z0);
      AE_MULASD32X16_H1_L0(A0, y1, z0);
      t0 = AE_TRUNCA32X2F64S(A0, A0, 33 - 8);

      z0 = AE_ROUND16X4F32SASYM(t0, t0);
      AE_S16_0_IP(z0, castxcc(ae_int16, pz), 2);
    }
    x += 16 * blkLen;
  }
} /* mtx_det4x4_16x16() */


