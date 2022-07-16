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
#define sz_i32    (int)sizeof(int32_t)
void cmtx_det3x3_32x32(complex_fract32 * restrict d, const complex_fract32 *restrict x, int rsh, int L)
{
#if 0
        ae_int32x2    *  pz = (ae_int32x2      *)d;
  const ae_int32x2    *  restrict px = (const ae_int32x2      *)(x);
  const ae_int32x2    *          S_rd;
        ae_int32x2    *          S_wr;
  int l;
  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx, blkNum, blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ / (9 * 2 * sz_i32);
  /* Allocate a fixed-size scratch area on the stack. */
  ae_int32x2 ALIGN(8) scr[9 * 2 * blkSize];
  ae_int32x2 x0, x1, x2, x3, x4, x5, x6, x7, x8;
  ae_int32x2 y0, y1, y2, y3;
  ae_int32x2 z0, z1, z2;
  ae_f64 A0, A1, A2, A3, A4, A5;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(d, sizeof(*d));
  NASSERT(rsh >= 0 && rsh <= 15);
  NASSERT_ALIGN8(scr);
  if (L <= 0) return;
  /*
   * Data are processed in blocks of scratch area size.Further, the algorithm
   * implementation is splitted in order to feed the optimizing compiler with a
   * few loops of managable size.
   */

  blkNum = (L + blkSize - 1) / blkSize;
  px = (const ae_int32x2      *)(x );
  for (blkIx = 0; blkIx<blkNum; blkIx++)
  {
    blkLen = XT_MIN(L - blkIx*blkSize, blkSize);
    px = (const ae_int32x2      *)(x + 3);
    S_wr = (ae_int32x2*)scr;
    for (l = 0; l<(blkLen); l++)
    {

      AE_L32X2_IP(x3, px, 8);
      AE_L32X2_IP(x4, px, 8);
      AE_L32X2_IP(x5, px, 8);
      AE_L32X2_IP(x6, px, 8);
      AE_L32X2_IP(x7, px, 8);
      AE_L32X2_XP(x8, px, 4*8);

      AE_S32X2_IP(x4, S_wr, 8);
      AE_S32X2_IP(x5, S_wr, 8);
      AE_S32X2_IP(x7, S_wr, 8);
      AE_S32X2_IP(x8, S_wr, 8);
      AE_S32X2_IP(x3, S_wr, 8);
      AE_S32X2_IP(x6, S_wr, 8);
    }
    __Pragma("no_reorder");
    S_rd = (ae_int32x2*)scr;
    S_wr = (ae_int32x2*)scr;
    for (l = 0; l<(blkLen); l++)
    {
    
      AE_L32X2_IP(x4, S_rd, 8);
      AE_L32X2_IP(x5, S_rd, 8);
      AE_L32X2_IP(x7, S_rd, 8);
      AE_L32X2_IP(x8, S_rd, 8);

      y0 = x4; y1 = x5;
      y2 = x7; y3 = x8;
      A0 = AE_MULF32R_HH(y0, y3);
      AE_MULSF32R_LL(A0, y0, y3);
      AE_MULSF32R_HH(A0, y1, y2);
      AE_MULAF32R_LL(A0, y1, y2);

      A1 = AE_MULF32R_HL(y0, y3);
      AE_MULAF32R_LH(A1, y0, y3);
      AE_MULSF32R_HL(A1, y1, y2);
      AE_MULSF32R_LH(A1, y1, y2);
      A0 = AE_SRAA64(A0, rsh);
      A1 = AE_SRAA64(A1, rsh);
      z0 = AE_ROUND32X2F48SASYM(A0, A1);

      AE_L32X2_IP(x3, S_rd, 8);
      AE_L32X2_IP(x6, S_rd, 8);

      y0 = x3; y1 = x5;
      y2 = x6; y3 = x8;

      A0 = AE_MULF32R_HH(y0, y3);
      AE_MULSF32R_LL(A0, y0, y3);
      AE_MULSF32R_HH(A0, y1, y2);
      AE_MULAF32R_LL(A0, y1, y2);

      A1 = AE_MULF32R_HL(y0, y3);
      AE_MULAF32R_LH(A1, y0, y3);
      AE_MULSF32R_HL(A1, y1, y2);
      AE_MULSF32R_LH(A1, y1, y2);
      A0 = AE_SRAA64(A0, rsh);
      A1 = AE_SRAA64(A1, rsh);
      z1 = AE_ROUND32X2F48SASYM(A0, A1);

      y0 = x3; y1 = x4;
      y2 = x6; y3 = x7;

      A0 = AE_MULF32R_HH(y0, y3);
      AE_MULSF32R_LL(A0, y0, y3);
      AE_MULSF32R_HH(A0, y1, y2);
      AE_MULAF32R_LL(A0, y1, y2);

      A1 = AE_MULF32R_HL(y0, y3);
      AE_MULAF32R_LH(A1, y0, y3);
      AE_MULSF32R_HL(A1, y1, y2);
      AE_MULSF32R_LH(A1, y1, y2);
      A0 = AE_SRAA64(A0, rsh);
      A1 = AE_SRAA64(A1, rsh);
      z2 = AE_ROUND32X2F48SASYM(A0, A1);

      AE_S32X2_IP(z0, S_wr, 8);
      AE_S32X2_IP(z1, S_wr, 8);
      AE_S32X2_IP(z2, S_wr, 8);
    }
    __Pragma("no_reorder");
    S_rd = (ae_int32x2*)scr;
    px = (const ae_int32x2      *)(x);
    for (l = 0; l<(blkLen); l++)
    {
      AE_L32X2_IP(x0, px, 8);
      AE_L32X2_IP(x1, px, 8);
      AE_L32X2_XP(x2, px, 7*8);

      AE_L32X2_IP(z0, S_rd, 8);
      AE_L32X2_IP(z1, S_rd, 8);
      AE_L32X2_IP(z2, S_rd, 8);
      A0 = AE_MULF32R_HH(x0, z0);
      AE_MULSF32R_LL(A0, x0, z0);
      AE_MULSF32R_HH(A0, x1, z1);
      AE_MULAF32R_LL(A0, x1, z1);
      AE_MULAF32R_HH(A0, x2, z2);
      AE_MULSF32R_LL(A0, x2, z2);
    
      A1 = AE_MULF32R_HL(x0, z0);
      AE_MULAF32R_LH(A1, x0, z0);
      AE_MULSF32R_HL(A1, x1, z1);
      AE_MULSF32R_LH(A1, x1, z1);
      AE_MULAF32R_HL(A1, x2, z2);
      AE_MULAF32R_LH(A1, x2, z2);

      z0 = AE_ROUND32X2F48SASYM(A0, A1);

      AE_S32X2_IP(z0, pz, 8);
    }
    x += 9 * blkLen;
  }
#else
        ae_int32x2    *  pz = (ae_int32x2      *)d;
  const ae_int32x2    *  restrict px = (const ae_int32x2      *)(x+4);

  int l;
  ae_int32x2 x0, x1, x2, x3, x4, x5, x6, x7, x8;
  ae_int32x2 y0, y1, y2, y3;
  ae_int32x2 z0, z1, z2;
  ae_f64 A0, A1;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(d, sizeof(*d));
  NASSERT(rsh >= 0 && rsh <= 15);
  if (L <= 0) return;
  AE_L32X2_IP(x4, px, 8);
  AE_L32X2_IP(x5, px, 8 * 2);
  AE_L32X2_IP(x7, px, 8);
  AE_L32X2_XP(x8, px, -2 * 8);

  AE_L32X2_XP(x6, px, -3 * 8);

  AE_L32X2_XP(x3, px, -8);
  AE_L32X2_XP(x2, px, -8);
  AE_L32X2_XP(x1, px, -8);

  AE_L32X2_XP(x0, px, 13 * 8);
  y0 = x4; y1 = x5;
  y2 = x7; y3 = x8;
  A0 = AE_MULF32R_HH(y0, y3);
  AE_MULSF32R_LL(A0, y0, y3);
  AE_MULSF32R_HH(A0, y1, y2);
  AE_MULAF32R_LL(A0, y1, y2);

  A1 = AE_MULF32R_HL(y0, y3);
  AE_MULAF32R_LH(A1, y0, y3);
  AE_MULSF32R_HL(A1, y1, y2);
  AE_MULSF32R_LH(A1, y1, y2);
  A0 = AE_SRAA64(A0, rsh);
  A1 = AE_SRAA64(A1, rsh);
  z0 = AE_ROUND32X2F48SASYM(A0, A1);

  y0 = x3; y1 = x5;
  y2 = x6; y3 = x8;

  A0 = AE_MULF32R_HH(y0, y3);
  A1 = AE_MULF32R_HL(y0, y3);
  AE_MULSF32R_LL(A0, y0, y3);
  AE_MULAF32R_LH(A1, y0, y3);
  AE_MULSF32R_HH(A0, y1, y2);
  AE_MULSF32R_HL(A1, y1, y2);
  AE_MULAF32R_LL(A0, y1, y2);
  AE_MULSF32R_LH(A1, y1, y2);

  for (l = 0; l<(L-1); l++)
  {
    A0 = AE_SRAA64(A0, rsh);
    A1 = AE_SRAA64(A1, rsh);
    z1 = AE_ROUND32X2F48SASYM(A0, A1);

    y0 = x3; y1 = x4;
    y2 = x6; y3 = x7;

    A0 = AE_MULF32R_HH(y0, y3);
    A1 = AE_MULF32R_HL(y0, y3);
    AE_MULSF32R_LL(A0, y0, y3);
    AE_MULAF32R_LH(A1, y0, y3);
    AE_MULSF32R_HH(A0, y1, y2);
    AE_MULSF32R_HL(A1, y1, y2);
    AE_MULAF32R_LL(A0, y1, y2);
    AE_MULSF32R_LH(A1, y1, y2);

    z2 = AE_TRUNCA32X2F64S(A0, A1, 16 - rsh);
    A0 = AE_MULF32R_HH(x0, z0);
    A1 = AE_MULF32R_HL(x0, z0);
    AE_MULSF32R_LL(A0, x0, z0);
    AE_MULAF32R_LH(A1, x0, z0);
    AE_MULSF32R_HH(A0, x1, z1);
    AE_MULSF32R_HL(A1, x1, z1);
    AE_MULAF32R_LL(A0, x1, z1);
    AE_MULSF32R_LH(A1, x1, z1);
    AE_MULAF32R_HH(A0, x2, z2);
    AE_MULAF32R_HL(A1, x2, z2);
    AE_MULSF32R_LL(A0, x2, z2);
    AE_MULAF32R_LH(A1, x2, z2);

    z0 = AE_ROUND32X2F48SASYM(A0, A1);
    AE_S32X2_IP(z0, pz, 8);

    AE_L32X2_IP(x4, px, 8);
    AE_L32X2_IP(x5, px, 8);
    AE_L32X2_IP(x6, px, 8);
    AE_L32X2_IP(x7, px, 8);
    AE_L32X2_XP(x8, px, -8*8);

    AE_L32X2_IP(x0, px, 8);
    AE_L32X2_IP(x1, px, 8);
    AE_L32X2_IP(x2, px, 8);

    AE_L32X2_XP(x3, px, 10*8);

    y0 = x4; y1 = x5;
    y2 = x7; y3 = x8;
    A0 = AE_MULF32R_HH(y0, y3);
    AE_MULSF32R_LL(A0, y0, y3);
    AE_MULSF32R_HH(A0, y1, y2);
    AE_MULAF32R_LL(A0, y1, y2);

    A1 = AE_MULF32R_HL(y0, y3);
    AE_MULAF32R_LH(A1, y0, y3);
    AE_MULSF32R_HL(A1, y1, y2);
    AE_MULSF32R_LH(A1, y1, y2);

    z0 = AE_TRUNCA32X2F64S(A0,A1,16-rsh);
    y0 = x3; y1 = x5;
    y2 = x6; y3 = x8;

    A0 = AE_MULF32R_HH(y0, y3);
    A1 = AE_MULF32R_HL(y0, y3);
    AE_MULSF32R_LL(A0, y0, y3);
    AE_MULAF32R_LH(A1, y0, y3);
    AE_MULSF32R_HH(A0, y1, y2);
    AE_MULSF32R_HL(A1, y1, y2);
    AE_MULAF32R_LL(A0, y1, y2);
    AE_MULSF32R_LH(A1, y1, y2);
  }
  A0 = AE_SRAA64(A0, rsh);
  A1 = AE_SRAA64(A1, rsh);
  z1 = AE_ROUND32X2F48SASYM(A0, A1);

  y0 = x3; y1 = x4;
  y2 = x6; y3 = x7;

  A0 = AE_MULF32R_HH(y0, y3);
  A1 = AE_MULF32R_HL(y0, y3);
  AE_MULSF32R_LL(A0, y0, y3);
  AE_MULAF32R_LH(A1, y0, y3);
  AE_MULSF32R_HH(A0, y1, y2);
  AE_MULSF32R_HL(A1, y1, y2);
  AE_MULAF32R_LL(A0, y1, y2);
  AE_MULSF32R_LH(A1, y1, y2);

  A0 = AE_SRAA64(A0, rsh);
  A1 = AE_SRAA64(A1, rsh);
  z2 = AE_ROUND32X2F48SASYM(A0, A1);
  A0 = AE_MULF32R_HH(x0, z0);
  A1 = AE_MULF32R_HL(x0, z0);
  AE_MULSF32R_LL(A0, x0, z0);
  AE_MULAF32R_LH(A1, x0, z0);
  AE_MULSF32R_HH(A0, x1, z1);
  AE_MULSF32R_HL(A1, x1, z1);
  AE_MULAF32R_LL(A0, x1, z1);
  AE_MULSF32R_LH(A1, x1, z1);
  AE_MULAF32R_HH(A0, x2, z2);
  AE_MULAF32R_HL(A1, x2, z2);
  AE_MULSF32R_LL(A0, x2, z2);
  AE_MULAF32R_LH(A1, x2, z2);

  z0 = AE_ROUND32X2F48SASYM(A0, A1); 
  AE_S32X2_IP(z0, pz, 8);
#endif
} /* cmtx_det3x3_32x32() */
