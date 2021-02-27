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
void mtx_det3x3_16x16(int16_t   * restrict d, const int16_t   *restrict x, int rsh, int L)
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
  const int blkSize = MAX_ALLOCA_SZ / (16*sz_i16);
  /* Allocate a fixed-size scratch area on the stack. */
  int16_t ALIGN(8) scr[16 * blkSize];

  ae_int16x4 x0, x1, x2;
  ae_int16x4 z0, z1, z2;
  ae_int32x2 y0, y1, y2, y3, y4;
  ae_int32x2 t0, t1;
  ae_int64 A0, A1, A2;
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
      ae_int16x4 t0, t1, t2;
      px = (const ae_int16x4      *)(x+9*l);
      xa = AE_LA64_PP(px);
      AE_LA16X4_IP(x0, xa, px);//0123
      AE_LA16X4_IP(x1, xa, px);//4567
      AE_LA16X4_IP(x2, xa, px);//8012

      t0 = AE_SEL16_4321(x0, x1);//3456
      t1 = AE_SEL16_4321(x1, x2);//7801

      z0 = AE_SEL16_7632(x1, t0); // 4 5 3 4

      t2 = AE_SEL16_7610(t1, t0);//7856

      z1 = AE_SEL16_7520(t0, t2);//3586 
      z2 = AE_SEL16_7610(t1, x1); // 7867
      z2 = AE_SHORTSWAP(z2);//7687
      AE_S16X4_IP(z0, S_wr0, 8);
      AE_S16X4_IP(z1, S_wr0, 8);
      AE_S16X4_IP(z2, S_wr0, 8);
      AE_S16X4_IP(x0, S_wr0, 8);
    }
    __Pragma("no_reorder");
    S_rd0 = (ae_int16x4*)scr;
    S_rd1 = (ae_int16x4*)(scr-2);
    for (l = 0; l<blkLen; l++)
    {
      AE_L16X4_IP(x0, S_rd0, 4 * 2);//4534
      AE_L16X4_IP(x1, S_rd0, 12 * 2);//3568

      AE_L16X2M_IU(y0, castxcc(const ae_p16x2s,S_rd1), 8 * 2); //86
      AE_L16X2M_IU(y1, castxcc(const ae_p16x2s,S_rd1), 2 * 2); //76
      AE_L16X2M_IU(y2, castxcc(const ae_p16x2s,S_rd1), 2 * 2); //87
      AE_L16X2M_IU(y3, castxcc(const ae_p16x2s,S_rd1), 2 * 2); //01
      AE_L16X2M_IU(y4, castxcc(const ae_p16x2s,S_rd1), 2 * 2); //23

     
      
      A0 = AE_MULZASD32X16_H3_L2(y2,x0);//8*4-7*5
      A1 = AE_MULZASD32X16_H3_L2(y0,x1);//3*8-5*6
      A2 = AE_MULZASD32X16_H1_L0(y1,x0);//7*3-6*4


      t0 = AE_TRUNCA32X2F64S(A0, A1, 33 - 8 - rsh);
      t1 = AE_TRUNCA32X2F64S(A2, A2, 33 - 8 - rsh);

      z0 = AE_ROUND16X4F32SASYM(t0, t1);
      A0 = AE_MULZASD32X16_H3_L2(y3, z0);
      AE_MULA32X16_H1(A0,y4,z0);
      t0 = AE_TRUNCA32X2F64S(A0, A0, 33 - 8);

      z0 = AE_ROUND16X4F32SASYM(t0, t0);
      AE_S16_0_IP(z0, castxcc(ae_int16, pz), 2);

      
    }
    x += 9 * blkLen;
  }
} /* mtx_det3x3_16x16() */


