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
#define sz_i16    (int)sizeof(int16_t)
void cmtx_det3x3_16x16(complex_fract16 * restrict d, const complex_fract16 *restrict x, int rsh, int L)
{
  int l;
  ae_int16x4    *  pz = (ae_int16x4      *)d;
  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  const ae_p16x2s    *  restrict px_;
  const ae_int16x4    *          S_rd0;
  const ae_p16x2s    *          S_rd1;
  ae_int16x4    * restrict S_wr0;
  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx, blkNum, blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ / (2*10*sz_i16);
  /* Allocate a fixed-size scratch area on the stack. */
  int16_t ALIGN(8) scr[2*10 * blkSize];

  ae_int16x4 x0, x1, x2, x3, x4;
  ae_int16x4 x5, x6, x7, x8;
  ae_int16x4 z0, z1, z2, z3, z4;
  ae_int32x2 y0, y1, y2, y3, y4, y5;
  ae_int32x2 t0, t1, t2;
  ae_int64 A0, A1, A2, A3, A4, A5;
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
    px = (const ae_int16x4      *)(x );
    xa = AE_LA64_PP(px);
    for (l = 0; l<(blkLen>>1); l++)
    {
      AE_LA16X4_IP(x0, xa, px);//01
      AE_LA16X4_IP(x1, xa, px);//23
      AE_LA16X4_IP(x2, xa, px);//45
      AE_LA16X4_IP(x3, xa, px);//67
      AE_LA16X4_IP(x4, xa, px);//80
      AE_LA16X4_IP(x5, xa, px);//12
      AE_LA16X4_IP(x6, xa, px);//34
      AE_LA16X4_IP(x7, xa, px);//56
      AE_LA16X4_IP(x8, xa, px);//78

      z0 = x2;
      z1 = AE_SEL16_5410(x1, x2);
      z2 = AE_SEL16_5432(x1, x2);
      z3 = AE_SEL16_5432(x3, x4);//78
      z4 = AE_SEL16_7632(x3, x3);//66

      AE_S16X4_IP(z0, S_wr0, 8);
      AE_S16X4_IP(z1, S_wr0, 8);
      AE_S16X4_IP(z2, S_wr0, 8);
      AE_S16X4_IP(z3, S_wr0, 8);
      AE_S16X4_IP(z4, S_wr0, 8);

      z0 = AE_SEL16_5432(x6, x7);
      z1 = AE_SEL16_7632(x6, x7);
      z2 = x6;
      z3 = x8;
      z4 = AE_SEL16_5410(x7, x7);

      AE_S16X4_IP(z0, S_wr0, 8);
      AE_S16X4_IP(z1, S_wr0, 8);
      AE_S16X4_IP(z2, S_wr0, 8);
      AE_S16X4_IP(z3, S_wr0, 8);
      AE_S16X4_IP(z4, S_wr0, 8);
    }
    if (blkLen&1)
    {
      AE_LA16X4_IP(x0, xa, px);//01
      AE_LA16X4_IP(x1, xa, px);//23
      AE_LA16X4_IP(x2, xa, px);//45
      AE_LA16X4_IP(x3, xa, px);//67
      AE_LA16X4_IP(x4, xa, px);//80

      z0 = x2;
      z1 = AE_SEL16_5410(x1, x2);
      z2 = AE_SEL16_5432(x1, x2);
      z3 = AE_SEL16_5432(x3, x4);//78
      z4 = AE_SEL16_7632(x3, x3);//66

      AE_S16X4_IP(z0, S_wr0, 8);
      AE_S16X4_IP(z1, S_wr0, 8);
      AE_S16X4_IP(z2, S_wr0, 8);
      AE_S16X4_IP(z3, S_wr0, 8);
      AE_S16X4_IP(z4, S_wr0, 8);
    }
    __Pragma("no_reorder");
    S_rd0 = (ae_int16x4*)scr;
    S_rd1 = (const ae_p16x2s*)(scr - 4);
    px_ = (const ae_p16x2s      *)(x - 7);

    AE_L16X4_IP(x0, S_rd0, 4 * 2);//45
    AE_L16X4_IP(x1, S_rd0, 4 * 2);//35
    AE_L16X4_IP(x2, S_rd0, 12 * 2);//34

    AE_L16X2M_XU(y0, S_rd1, 16 * 2); //7
    AE_L16X2M_IU(y1, S_rd1, 2 * 2); //8
    AE_L16X2M_IU(y2, S_rd1, 2 * 2); //6

    A0 = AE_MULZASD32X16_H3_L2(y1, x0);// 8 * 4 
    A1 = AE_MULZAAD32X16_H2_L3(y1, x0);//8 * 4
    for (l = 0; l<(blkLen - 1); l++)
    {
      t0 = AE_L32X2_X((ae_int32x2 *)S_rd0, -20 * 2);
      t1 = AE_L32X2_X((ae_int32x2 *)S_rd0, -16 * 2);
      t2 = AE_L32X2_X((ae_int32x2 *)S_rd0, -12 * 2);

      AE_MULSAD32X16_H1_L0(A0, y0, x0);// -7 * 5
      AE_MULSSD32X16_H1_L0(A1, y0, AE_MOVINT16X4_FROMINT32X2(t0));//-7*5

      A2 = AE_MULZASD32X16_H3_L2(y1, x1);// 8 * 3 
      A3 = AE_MULZAAD32X16_H2_L3(y1, x1);//8 * 3
      AE_MULSAD32X16_H1_L0(A2, y2, x1);// -6 * 5

      AE_MULSSD32X16_H1_L0(A3, y2, AE_MOVINT16X4_FROMINT32X2(t1));//-6*5

      A4 = AE_MULZASD32X16_H3_L2(y0, x2);// 7 * 3 
      AE_MULSAD32X16_H1_L0(A4, y2, x2);// -6 * 4

      A5 = AE_MULZAAD32X16_H2_L3(y0, x2);//7 * 3
      AE_MULSSD32X16_H1_L0(A5, y2, AE_MOVINT16X4_FROMINT32X2(t2));//-6*4

      A0 = AE_SLAA64(A0, 17 - 8 - rsh);
      A1 = AE_SLAA64(A1, 17 - 8 - rsh);
      A2 = AE_SLAA64(A2, 17 - 8 - rsh);
      A3 = AE_SLAA64(A3, 17 - 8 - rsh);
      A4 = AE_SLAA64(A4, 17 - 8 - rsh);
      A5 = AE_SLAA64(A5, 17 - 8 - rsh);
      t0 = AE_ROUND32X2F48SASYM(A0, A1);
      t1 = AE_ROUND32X2F48SASYM(A2, A3);
      t2 = AE_ROUND32X2F48SASYM(A4, A5);
      z0 = AE_ROUND16X4F32SASYM(t0, t1);
      z1 = AE_ROUND16X4F32SASYM(t2, t2);

      AE_L16X2M_IU(y3, px_, 14 * 2);
      AE_L16X2M_IU(y4, px_, 2 * 2); 
      AE_L16X2M_IU(y5, px_, 2 * 2); 
      A0 = AE_MULZASD32X16_H3_L2(y3, z0);
      AE_MULSAD32X16_H1_L0(A0, y4, z0);
      AE_MULASD32X16_H3_L2(A0, y5, z1);

      A1 = AE_MULZAAD32X16_H2_L3(y3, z0);
      z0 = AE_SHORTSWAP(z0);
      AE_MULSSD32X16_H3_L2(A1, y4, z0);
      AE_MULAAD32X16_H2_L3(A1, y5, z1);
      
      A0 = AE_SLAA64(A0, 17 - 8 );
      A1 = AE_SLAA64(A1, 17 - 8 ); 
      t0 = AE_ROUND32X2F48SASYM(A1, A0);
      z0 = AE_ROUND16X4F32SASYM(t0, t0);
      t0 = AE_MOVINT32X2_FROMINT16X4(z0);
      AE_S32_L_IP(t0, castxcc( ae_int32, pz), 4);

      AE_L16X4_IP(x0, S_rd0, 4 * 2);//45
      AE_L16X4_IP(x1, S_rd0, 4 * 2);//35
      AE_L16X4_IP(x2, S_rd0, 12 * 2);//34

      AE_L16X2M_XU(y0, S_rd1, 16 * 2); //7
      AE_L16X2M_IU(y1, S_rd1, 2 * 2); //8
      AE_L16X2M_IU(y2, S_rd1, 2 * 2); //6

      A0 = AE_MULZASD32X16_H3_L2(y1, x0);// 8 * 4 
      A1 = AE_MULZAAD32X16_H2_L3(y1, x0);//8 * 4
    }
    t0 = AE_L32X2_X((ae_int32x2 *)S_rd0, -20 * 2);
    t1 = AE_L32X2_X((ae_int32x2 *)S_rd0, -16 * 2);
    t2 = AE_L32X2_X((ae_int32x2 *)S_rd0, -12 * 2);

    AE_MULSAD32X16_H1_L0(A0, y0, x0);// -7 * 5
    AE_MULSSD32X16_H1_L0(A1, y0, AE_MOVINT16X4_FROMINT32X2(t0));//-7*5

    A2 = AE_MULZASD32X16_H3_L2(y1, x1);// 8 * 3 
    A3 = AE_MULZAAD32X16_H2_L3(y1, x1);//8 * 3
    AE_MULSAD32X16_H1_L0(A2, y2, x1);// -6 * 5

    AE_MULSSD32X16_H1_L0(A3, y2, AE_MOVINT16X4_FROMINT32X2(t1));//-6*5

    A4 = AE_MULZASD32X16_H3_L2(y0, x2);// 7 * 3 
    AE_MULSAD32X16_H1_L0(A4, y2, x2);// -6 * 4

    A5 = AE_MULZAAD32X16_H2_L3(y0, x2);//7 * 3
    AE_MULSSD32X16_H1_L0(A5, y2, AE_MOVINT16X4_FROMINT32X2(t2));//-6*4

    A0 = AE_SLAA64(A0, 17 - 8 - rsh);
    A1 = AE_SLAA64(A1, 17 - 8 - rsh);
    A2 = AE_SLAA64(A2, 17 - 8 - rsh);
    A3 = AE_SLAA64(A3, 17 - 8 - rsh);
    A4 = AE_SLAA64(A4, 17 - 8 - rsh);
    A5 = AE_SLAA64(A5, 17 - 8 - rsh);
    t0 = AE_ROUND32X2F48SASYM(A0, A1);
    t1 = AE_ROUND32X2F48SASYM(A2, A3);
    t2 = AE_ROUND32X2F48SASYM(A4, A5);
    z0 = AE_ROUND16X4F32SASYM(t0, t1);
    z1 = AE_ROUND16X4F32SASYM(t2, t2);

    AE_L16X2M_IU(y3, px_, 14 * 2);
    AE_L16X2M_IU(y4, px_, 2 * 2);
    AE_L16X2M_IU(y5, px_, 2 * 2);
    A0 = AE_MULZASD32X16_H3_L2(y3, z0);
    AE_MULSAD32X16_H1_L0(A0, y4, z0);
    AE_MULASD32X16_H3_L2(A0, y5, z1);

    A1 = AE_MULZAAD32X16_H2_L3(y3, z0);
    z0 = AE_SHORTSWAP(z0);
    AE_MULSSD32X16_H3_L2(A1, y4, z0);
    AE_MULAAD32X16_H2_L3(A1, y5, z1);

    A0 = AE_SLAA64(A0, 17 - 8);
    A1 = AE_SLAA64(A1, 17 - 8);
    t0 = AE_ROUND32X2F48SASYM(A1, A0);
    z0 = AE_ROUND16X4F32SASYM(t0, t0);
    t0 = AE_MOVINT32X2_FROMINT16X4(z0);
    AE_S32_L_IP(t0, castxcc(ae_int32, pz), 4);
    x += 9 * blkLen;
  }
} /* cmtx_det3x3_16x16() */


