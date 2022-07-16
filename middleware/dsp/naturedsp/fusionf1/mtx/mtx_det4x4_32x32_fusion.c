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
void mtx_det4x4_32x32(int32_t   * restrict d, const int32_t   *restrict x, int rsh, int L)
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
  const int blkSize = MAX_ALLOCA_SZ / (36 *  sz_i32);
  /* Allocate a fixed-size scratch area on the stack. */
  ae_int32x2 ALIGN(8) scr[36 * blkSize];

  ae_int32x2 x01, x23, x45, x67;
  ae_int32x2 x89, xab, xcd, xef;
  ae_int32x2 y0, y1, y2, y3;
  ae_int32x2 z0, z1, z2, z3;
  ae_f64 A0, A1, A2;
  ae_valign xa, za;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(d, sizeof(*d));
  NASSERT(rsh >= 0 && rsh <= 15);
  if (L <= 0) return;
  /*
  * Data are processed in blocks of scratch area size.Further, the algorithm
  * implementation is splitted in order to feed the optimizing compiler with a
  * few loops of managable size.
  */

  blkNum = (L + blkSize - 1) / blkSize;
  px = (const ae_int32x2      *)(x);
  for (blkIx = 0; blkIx<blkNum; blkIx++)
  {
    ae_int32x2 x0, x1, x2, x3;
    ae_int32x2 y0, y1, y2, y3;
    blkLen = XT_MIN(L - blkIx*blkSize, blkSize);
    px = (const ae_int32x2      *)(x + 4);
    S_wr = (ae_int32x2*)scr;
    for (l = 0; l<(blkLen); l++)
    {
      ae_int32x2 x0, x1, x2, x3, x4, x5;
      ae_int32x2 y0, y1, y2, y3, y4, y5, y6, y7, y8;

      px = (const ae_int32x2      *)(x + l * 16 + 4);
      xa = AE_LA64_PP(px);

      AE_LA32X2_IP(x0, xa, px);
      AE_LA32X2_IP(x1, xa, px);
      AE_LA32X2_IP(x2, xa, px);
      AE_LA32X2_IP(x3, xa, px);
      AE_LA32X2_IP(x4, xa, px);
      AE_LA32X2_IP(x5, xa, px);
    

      y0 = AE_SEL32_LH(x0, x1);
      y1 = AE_SEL32_LL(x1, x2);
      y2 = x3;
      y3 = AE_SEL32_LH(x4, x5);
      y4 = AE_SEL32_LH(x5,x0);
      y5 = x1;
      y6 = AE_SEL32_HH(x2, x3);
      y7 = AE_SEL32_LH(x3, x4);
      y8 = x5;
      AE_S32X2_IP(y0, S_wr, 8);
      AE_S32X2_IP(y1, S_wr, 8);
      AE_S32X2_IP(y2, S_wr, 8);
      AE_S32X2_IP(y3, S_wr, 8);
      AE_S32X2_IP(y4, S_wr, 8);
      
      AE_S32X2_IP(y5, S_wr, 8);
      AE_S32X2_IP(y6, S_wr, 8);
      AE_S32X2_IP(y7, S_wr, 8);
      AE_S32X2_IP(y8, S_wr, 8);

      y0 = x0;
      y1 = AE_SEL32_LH(x1, x2);
      y2 = AE_SEL32_LL(x2, x3);
      y3 = x4;
      y4 = AE_SEL32_LH(x5, x0);
      y5 = AE_SEL32_LH(x0, x1);
      y6 = x2;
      y7 = AE_SEL32_HH(x3, x4);
      y8 = AE_SEL32_LH(x4, x5);

      AE_S32X2_IP(y0, S_wr, 8);
      AE_S32X2_IP(y1, S_wr, 8);
      AE_S32X2_IP(y2, S_wr, 8);
      AE_S32X2_IP(y3, S_wr, 8);
      AE_S32X2_IP(y4, S_wr, 8);

      AE_S32X2_IP(y5, S_wr, 8);
      AE_S32X2_IP(y6, S_wr, 8);
      AE_S32X2_IP(y7, S_wr, 8);
      AE_S32X2_IP(y8, S_wr, 8);
    }
    __Pragma("no_reorder");
    S_rd = (ae_int32x2*)scr;
    S_wr = (ae_int32x2*)scr;
    mtx_det3x3_32x32((fract32 *)S_wr, (fract32 *)S_rd, rsh, 4 * blkLen);
    __Pragma("no_reorder");
    
    S_rd = (ae_int32x2*)scr;
    for (l = 0; l<(blkLen); l++)
    {
      px = (const ae_int32x2      *)(x + l*16);
      xa = AE_LA64_PP(px);
      AE_LA32X2_IP(x0, xa, px);
      AE_LA32X2_IP(x1, xa, px);
      AE_L32X2_IP(y0, S_rd, 8);
      AE_L32X2_IP(y1, S_rd, 8);

      A0 = AE_MULF32R_HH(x0, y0);
      AE_MULSF32R_LL(A0, x0, y0);
      AE_MULAF32R_HH(A0, x1, y1);
      AE_MULSF32R_LL(A0, x1, y1);

      z0 = AE_ROUND32X2F48SASYM(A0, A0);
      AE_S32_L_IP(z0, pz, 4);
    }
    x += 16 * blkLen;
  }
#else
  ae_int32x2    *  pz = (ae_int32x2      *)d;
  const ae_int32x2    *  restrict px = (const ae_int32x2      *)(x);

  int l;
  ae_int32x2 x01, x23, x45, x67;
  ae_int32x2 x89, xab, xcd, xef;
  ae_int32x2 m0, m1;
  ae_int32x2 z0, z1, z2;
  ae_f64 A0, A1, A2;
  ae_f64 B1, B2, C2;
  ae_f64 M0, M1, M2, M3;
  ae_valign xa, za;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(d, sizeof(*d));
  NASSERT(rsh >= 0 && rsh <= 15);
  if (L <= 0) return;
  xa = AE_LA64_PP(px);
  za = AE_ZALIGN64();
  for (l = 0; l<(L ); l++)
  {
    AE_LA32X2_IP(x89, xa, px);
    AE_LA32X2_IP(xab, xa, px);
    AE_LA32X2_IP(xcd, xa, px);
    AE_LA32X2_IP(xef, xa, px);

    AE_LA32X2_IP(x01, xa, px);
    AE_LA32X2_IP(x23, xa, px);
    AE_LA32X2_IP(x45, xa, px);
    AE_LA32X2_IP(x67, xa, px);

    A0 = AE_MULF32R_HL(xab, xef);
    AE_MULSF32R_LH(A0, xab, xef);

    A1 = AE_MULF32R_LL(x89, xef);
    AE_MULSF32R_LL(A1, xcd, xab);

    A2 = AE_MULF32R_LH(x89, xef);
    AE_MULSF32R_LH(A2, xcd, xab);

    B1 = AE_MULF32R_HL(x89, xef);
    AE_MULSF32R_HL(B1, xcd, xab);

    B2 = AE_MULF32R_HH(x89, xef);
    AE_MULSF32R_HH(B2, xcd, xab);

    C2 = AE_MULF32R_HL(x89, xcd);
    AE_MULSF32R_HL(C2, xcd, x89);

    A0 = AE_SRAA64(A0, rsh);
    A1 = AE_SRAA64(A1, rsh);
    A2 = AE_SRAA64(A2, rsh);
    B1 = AE_SRAA64(B1, rsh);
    B2 = AE_SRAA64(B2, rsh);
    C2 = AE_SRAA64(C2, rsh);
    z0 = AE_ROUND32X2F48SASYM(A0, A1);
    z1 = AE_ROUND32X2F48SASYM(A2, B1);
    z2 = AE_ROUND32X2F48SASYM(B2, C2);

    M0 = AE_MULF32R_LH(x45, z0);
    AE_MULSF32R_HL(M0, x67, z0);
    AE_MULAF32R_LH(M0, x67, z1);

    M1 = AE_MULF32R_HH(x45, z0);
    AE_MULSF32R_HL(M1, x67, z1);
    AE_MULAF32R_LH(M1, x67, z2);

    M2 = AE_MULF32R_HL(x45, z0);
    AE_MULSF32R_LL(M2, x45, z1);
    AE_MULAF32R_LL(M2, x67, z2);

    M3 = AE_MULF32R_HH(x45, z1);
    AE_MULSF32R_LH(M3, x45, z2);
    AE_MULAF32R_HL(M3, x67, z2);

    m0 = AE_ROUND32X2F48SASYM(M0, M1);
    m1 = AE_ROUND32X2F48SASYM(M2, M3);
     
    A0 = AE_MULF32R_HH(x01, m0);
    AE_MULSF32R_LL(A0, x01, m0);
    AE_MULAF32R_HH(A0, x23, m1);
    AE_MULSF32R_LL(A0, x23, m1);

    z0 = AE_ROUND32X2F48SASYM(A0, A0);
    AE_S32_L_IP(z0, castxcc(ae_int32, pz), 4);
  }

#endif
} /* mtx_det4x4_32x32() */


