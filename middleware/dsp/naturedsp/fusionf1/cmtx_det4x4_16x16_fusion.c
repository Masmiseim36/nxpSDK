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
void cmtx_det4x4_16x16(complex_fract16 * restrict d, const complex_fract16 *restrict x, int rsh, int L)
{
  int l;
  ae_int16x4    *  pz = (ae_int16x4      *)d;
  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  const ae_int16x4    *  restrict px1 = (const ae_int16x4      *)x;
  const ae_int16x4    *          S_rd0;
  ae_int16x4    * restrict S_wr0;

  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx, blkNum, blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ / (2*12*sz_i16);
  /* Allocate a fixed-size scratch area on the stack. */
  int16_t ALIGN(8) scr[2*12 * blkSize];

  ae_int16x4 x0, x1, x2, x3, x4;
  ae_int16x4 x5;
  ae_int16x4 z0, z1, z2, z3;
  ae_int32x2 y0, y1, y2, y3;
  ae_int32x2 t0, t1, t2, t3, t4, t5;
  ae_int64 A0r, A0i, A1r, A1i, A2r, A2i, B1r, B1i, B2r, B2i, C2r, C2i;
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
    
    for (l = 0; l<(blkLen); l++)
    {
      ae_int16x4 x4, x5;
      ae_int16x4 z0, z1, z2, z3, z4, z5;

      px = (const ae_int16x4      *)(x + l*16 + 8);
      xa = AE_LA64_PP(px);
      
      AE_LA16X4_IP(x4, xa, px); 
      AE_LA16X4_IP(x5, xa, px); 

      /* Save in scratch
           x10  x11
           x9  x11
           x9  x10
           x8  x11
           x8  x10
           x8  x9
      */
      z0 = x5;
      z1 = AE_SEL16_5410(x4,x5);
      z2 = AE_SEL16_5432(x4,x5);
      z3 = AE_SEL16_7610(x4,x5);
      z4 = AE_SEL16_7632(x4,x5);
      z5 = x4;

      AE_S16X4_IP(z0, S_wr0, 8);
      AE_S16X4_IP(z1, S_wr0, 8);
      AE_S16X4_IP(z2, S_wr0, 8);
      AE_S16X4_IP(z3, S_wr0, 8);
      AE_S16X4_IP(z4, S_wr0, 8);
      AE_S16X4_IP(z5, S_wr0, 8);
    }
    __Pragma("no_reorder");
    S_rd0 = (ae_int16x4*)scr;
    px1 = (const ae_int16x4      *)(x-1);
    px = (const ae_int16x4      *)(x-13);
    S_wr0 = (ae_int16x4*)scr;
    
    for (l = 0; l<(blkLen); l++)
    {
      AE_L16X4_IP(x0, S_rd0, 4 * 2);
      AE_L16X4_IP(x1, S_rd0, 4 * 2);
      AE_L16X4_IP(x2, S_rd0, 4 * 2);
      AE_L16X4_IP(x3, S_rd0, 4 * 2);
      AE_L16X4_IP(x4, S_rd0, 4 * 2);
      AE_L16X4_IP(x5, S_rd0, 4 * 2);

      AE_L16X2M_XU(y0, castxcc(const ae_p16x2s, px1), 26 * 2);
      AE_L16X2M_IU(y1, castxcc(const ae_p16x2s, px1), 2 * 2);
      AE_L16X2M_IU(y2, castxcc(const ae_p16x2s, px1), 2 * 2);
      AE_L16X2M_IU(y3, castxcc(const ae_p16x2s, px1), 2 * 2);

      A0r = AE_MULZASD32X16_H3_L2(y3, x0);
      A0i = AE_MULZAAD32X16_H2_L3(y3, x0);
      AE_MULSAD32X16_H1_L0(A0r, y2, x0); 
      x0 = AE_SHORTSWAP(x0);
      AE_MULSSD32X16_H3_L2(A0i, y2, x0);

      A1r = AE_MULZASD32X16_H3_L2(y3, x1);
      A1i = AE_MULZAAD32X16_H2_L3(y3, x1);
      AE_MULSAD32X16_H1_L0(A1r, y1, x1); 
      x1 = AE_SHORTSWAP(x1);
      AE_MULSSD32X16_H3_L2(A1i, y1, x1);

      A2r = AE_MULZASD32X16_H3_L2(y2, x2);
      A2i = AE_MULZAAD32X16_H2_L3(y2, x2);
      AE_MULSAD32X16_H1_L0(A2r, y1, x2);
      x2 = AE_SHORTSWAP(x2);
      AE_MULSSD32X16_H3_L2(A2i, y1, x2);

      B1r = AE_MULZASD32X16_H3_L2(y3, x3);
      B1i = AE_MULZAAD32X16_H2_L3(y3, x3);
      AE_MULSAD32X16_H1_L0(B1r, y0, x3);
      x3 = AE_SHORTSWAP(x3);
      AE_MULSSD32X16_H3_L2(B1i, y0, x3);

      A1r = AE_SLAA64(A1r, 17 - 8 - rsh);
      A1i = AE_SLAA64(A1i, 17 - 8 - rsh);
      B1r = AE_SLAA64(B1r, 17 - 8 - rsh);
      B1i = AE_SLAA64(B1i, 17 - 8 - rsh);

      t0 = AE_TRUNCA32X2F64S(A0r, A0i, 33 - 8 - rsh);
      t2 = AE_TRUNCA32X2F64S(A2r, A2i, 33 - 8 - rsh);

      t1 = AE_ROUND32X2F48SASYM(A1r, A1i);
      t3 = AE_ROUND32X2F48SASYM(B1r, B1i);
      z0 = AE_ROUND16X4F32SASYM(t0, t1);
      z1 = AE_ROUND16X4F32SASYM(t2, t3);

      AE_S16X4_IP(z0, S_wr0, 8);
      AE_S16X4_XP(z1, S_wr0, 5*4*2);
    }
    __Pragma("no_reorder");
    S_rd0 = (ae_int16x4*)scr;
    px1 = (const ae_int16x4      *)(x - 1);
    
    S_wr0 = (ae_int16x4*)(scr+2*4);
    for (l = 0; l<(blkLen); l++)
    {
     
      AE_L16X4_IP(x0, S_rd0, 4 * 2);
      AE_L16X4_IP(x1, S_rd0, 4 * 2);
      AE_L16X4_IP(x2, S_rd0, 4 * 2);
      AE_L16X4_IP(x3, S_rd0, 4 * 2);
      AE_L16X4_IP(x4, S_rd0, 4 * 2);
      AE_L16X4_IP(x5, S_rd0, 4 * 2);

      AE_L16X2M_XU(y0, castxcc(const ae_p16x2s, px1), 26 * 2);
      AE_L16X2M_IU(y1, castxcc(const ae_p16x2s, px1), 2 * 2);
      AE_L16X2M_IU(y2, castxcc(const ae_p16x2s, px1), 2 * 2);
      AE_L16X2M_IU(y3, castxcc(const ae_p16x2s, px1), 2 * 2);

      B2r = AE_MULZASD32X16_H3_L2(y2, x4);
      B2i = AE_MULZAAD32X16_H2_L3(y2, x4);
      AE_MULSAD32X16_H1_L0(B2r, y0, x4);
      x4 = AE_SHORTSWAP(x4);
      AE_MULSSD32X16_H3_L2(B2i, y0, x4);

      C2r = AE_MULZASD32X16_H3_L2(y1, x5);
      C2i = AE_MULZAAD32X16_H2_L3(y1, x5);
      AE_MULSAD32X16_H1_L0(C2r, y0, x5);
      x5 = AE_SHORTSWAP(x5);
      AE_MULSSD32X16_H3_L2(C2i, y0, x5);

      C2r = AE_SLAA64(C2r, 17 - 8 - rsh);
      C2i = AE_SLAA64(C2i, 17 - 8 - rsh);
      t4 = AE_TRUNCA32X2F64S(B2r, B2i, 33 - 8 - rsh);
      t5 = AE_ROUND32X2F48SASYM(C2r, C2i);
      z2 = AE_ROUND16X4F32SASYM(t4, t5);

      AE_S16X4_XP(z2, S_wr0, 6*4*2);
    }
    __Pragma("no_reorder");
    
    S_rd0 = (ae_int16x4*)(scr - 14);
    S_wr0 = (ae_int16x4*)(scr+4*3);
    px = (const ae_int16x4      *)(x - 9);
    for (l = 0; l<(blkLen); l++)
    {
      ae_int32x2 t0, t1, t2, t3, t4, t5;
      ae_int32x2 d0, d1, d2, d3;
      ae_int24x2 f0, f1, f2, f3, f4, f5, p0, p1 ,p2, p3;

      AE_L16X2M_XU(t0, castxcc(const ae_p16x2s, S_rd0), 14 * 2);
      AE_L16X2M_IU(t1, castxcc(const ae_p16x2s, S_rd0), 2 * 2);
      AE_L16X2M_IU(t2, castxcc(const ae_p16x2s, S_rd0), 2 * 2);
      AE_L16X2M_IU(t3, castxcc(const ae_p16x2s, S_rd0), 2 * 2);
      AE_L16X2M_IU(t4, castxcc(const ae_p16x2s, S_rd0), 2 * 2);
      AE_L16X2M_IU(t5, castxcc(const ae_p16x2s, S_rd0), 2 * 2);

      f0 = AE_MOVINT24X2_FROMINT32X2(t0);
      f1 = AE_MOVINT24X2_FROMINT32X2(t1);
      f2 = AE_MOVINT24X2_FROMINT32X2(t2);
      f3 = AE_MOVINT24X2_FROMINT32X2(t3);
      f4 = AE_MOVINT24X2_FROMINT32X2(t4);
      f5 = AE_MOVINT24X2_FROMINT32X2(t5);

      AE_L16X2M_XU(d0, castxcc(const ae_p16x2s, px), 26 * 2);
      AE_L16X2M_IU(d1, castxcc(const ae_p16x2s, px), 2 * 2);
      AE_L16X2M_IU(d2, castxcc(const ae_p16x2s, px), 2 * 2);
      AE_L16X2M_IU(d3, castxcc(const ae_p16x2s, px), 2 * 2);


      p0 = AE_MOVINT24X2_FROMINT32X2(d0);
      p1 = AE_MOVINT24X2_FROMINT32X2(d1);
      p2 = AE_MOVINT24X2_FROMINT32X2(d2);
      p3 = AE_MOVINT24X2_FROMINT32X2(d3);

      A0r = AE_MULZASD24_HH_LL(p1, f0);
      A0i = AE_MULZAAD24_HL_LH(p1, f0);
      A1r = AE_MULZASD24_HH_LL(p0, f0);
      A1i = AE_MULZAAD24_HL_LH(p0, f0);


      AE_MULSAD24_HH_LL(A0r, p2, f1);
      AE_MULSSD24_HL_LH(A0i, p2, f1);
      AE_MULSAD24_HH_LL(A1r, p2, f3);
      AE_MULSSD24_HL_LH(A1i, p2, f3);

      AE_MULASD24_HH_LL(A0r, p3, f2);
      AE_MULASD24_HH_LL(A1r, p3, f4);

      AE_MULAAD24_HL_LH(A0i, p3, f2);
      AE_MULAAD24_HL_LH(A1i, p3, f4);

      A1r = AE_SLAI64(A1r,1);
      A1i = AE_SLAI64(A1i,1);

      t0 = AE_TRUNCA32X2F64S(A0r, A0i, 33-16);
      t1 = AE_ROUND32X2F48SASYM(A1r, A1i);
      z0 = AE_ROUND16X4F32SASYM(t0, t1);
      AE_S16X4_IP(z0, S_wr0, 6*4 * 2);
    }
    S_rd0 = (ae_int16x4*)(scr - 14);
    S_wr0 = (ae_int16x4*)(scr+4*4);
    px = (const ae_int16x4      *)(x - 9);
    for (l = 0; l<(blkLen); l++)
    {
      ae_int32x2 t0, t1, t2, t3, t4, t5;
      ae_int32x2 d0, d1, d2, d3;
      ae_int24x2 f0, f1, f2, f3, f4, f5, p0, p1 ,p2, p3;

      AE_L16X2M_XU(t0, castxcc(const ae_p16x2s, S_rd0), 14 * 2);
      AE_L16X2M_IU(t1, castxcc(const ae_p16x2s, S_rd0), 2 * 2);
      AE_L16X2M_IU(t2, castxcc(const ae_p16x2s, S_rd0), 2 * 2);
      AE_L16X2M_IU(t3, castxcc(const ae_p16x2s, S_rd0), 2 * 2);
      AE_L16X2M_IU(t4, castxcc(const ae_p16x2s, S_rd0), 2 * 2);
      AE_L16X2M_IU(t5, castxcc(const ae_p16x2s, S_rd0), 2 * 2);

      f0 = AE_MOVINT24X2_FROMINT32X2(t0);
      f1 = AE_MOVINT24X2_FROMINT32X2(t1);
      f2 = AE_MOVINT24X2_FROMINT32X2(t2);
      f3 = AE_MOVINT24X2_FROMINT32X2(t3);
      f4 = AE_MOVINT24X2_FROMINT32X2(t4);
      f5 = AE_MOVINT24X2_FROMINT32X2(t5);

      AE_L16X2M_XU(d0, castxcc(const ae_p16x2s, px), 26 * 2);
      AE_L16X2M_IU(d1, castxcc(const ae_p16x2s, px), 2 * 2);
      AE_L16X2M_IU(d2, castxcc(const ae_p16x2s, px), 2 * 2);
      AE_L16X2M_IU(d3, castxcc(const ae_p16x2s, px), 2 * 2);


      p0 = AE_MOVINT24X2_FROMINT32X2(d0);
      p1 = AE_MOVINT24X2_FROMINT32X2(d1);
      p2 = AE_MOVINT24X2_FROMINT32X2(d2);
      p3 = AE_MOVINT24X2_FROMINT32X2(d3);

      A2r = AE_MULZASD24_HH_LL(p0, f1);
      A2i = AE_MULZAAD24_HL_LH(p0, f1);
      B1r = AE_MULZASD24_HH_LL(p0, f2);
      B1i = AE_MULZAAD24_HL_LH(p0, f2);

      AE_MULSAD24_HH_LL(A2r, p1, f3);
      AE_MULSSD24_HL_LH(A2i, p1, f3);
      AE_MULSAD24_HH_LL(B1r, p1, f4);
      AE_MULSSD24_HL_LH(B1i, p1, f4);

      AE_MULASD24_HH_LL(A2r, p3, f5);
      AE_MULASD24_HH_LL(B1r, p2, f5);
      AE_MULAAD24_HL_LH(A2i, p3, f5);
      AE_MULAAD24_HL_LH(B1i, p2, f5);

      A2r = AE_SLAI64(A2r,1);
      A2i = AE_SLAI64(A2i,1);
      B1r = AE_SLAI64(B1r,1);
      B1i = AE_SLAI64(B1i,1);


      t2 = AE_ROUND32X2F48SASYM(A2r, A2i);
      t3 = AE_ROUND32X2F48SASYM(B1r, B1i);

      z1 = AE_ROUND16X4F32SASYM(t2, t3);
      AE_S16X4_IP(z1, S_wr0, 6*4 * 2);  

    }

    __Pragma("no_reorder");
    S_rd0 = (ae_int16x4*)(scr+4*3);
    px = (const ae_int16x4      *)(x -13);
    AE_L16X4_IP(z0, S_rd0, 4 * 2);
    AE_L16X4_IP(z1, S_rd0, 5*4 * 2);
    {
      ae_int32x2 t0, t1;
      t0 = AE_L32X2_X((ae_int32x2 *)S_rd0, -6*4 * 2);
      t1 = AE_L32X2_X((ae_int32x2 *)S_rd0, -5*4 * 2);
      z2 = AE_MOVINT16X4_FROMINT32X2(t0);
      z3 = AE_MOVINT16X4_FROMINT32X2(t1);
    }
    AE_L16X2M_XU(y0, castxcc(const ae_p16x2s, px), 26 * 2);
    AE_L16X2M_IU(y1, castxcc(const ae_p16x2s, px), 2 * 2);
    AE_L16X2M_IU(y2, castxcc(const ae_p16x2s, px), 2 * 2);
    AE_L16X2M_IU(y3, castxcc(const ae_p16x2s, px), 2 * 2);
    for (l = 0; l<(blkLen - 1); l++)
    {
      A0r = AE_MULZASD32X16_H3_L2(y0, z0);
      A0i = AE_MULZAAD32X16_H2_L3(y0, z0);
      AE_MULSAD32X16_H1_L0(A0r, y1, z0);
      AE_MULSSD32X16_H1_L0(A0i, y1, z2);

      AE_MULASD32X16_H3_L2(A0r, y2, z1);
      AE_MULAAD32X16_H2_L3(A0i, y2, z1);
      AE_MULSAD32X16_H1_L0(A0r, y3, z1);
      AE_MULSSD32X16_H1_L0(A0i, y3, z3);

      t0 = AE_TRUNCA32X2F64S(A0i, A0r, 33 - 8);
      z0 = AE_ROUND16X4F32SASYM(t0, t0);
      t0 = AE_MOVINT32X2_FROMINT16X4(z0);
      AE_S32_L_IP(t0, castxcc(ae_int32,pz), 4);
      AE_L16X4_IP(z0, S_rd0, 4 * 2);
      AE_L16X4_IP(z1, S_rd0, 5*4 * 2);
      {
        ae_int32x2 t0, t1;
        t0 = AE_L32X2_X((ae_int32x2 *)S_rd0, -6 * 4 * 2);
        t1 = AE_L32X2_X((ae_int32x2 *)S_rd0, -5 * 4 * 2);
        z2 = AE_MOVINT16X4_FROMINT32X2(t0);
        z3 = AE_MOVINT16X4_FROMINT32X2(t1);
      }

      AE_L16X2M_XU(y0, castxcc(const ae_p16x2s, px), 26 * 2);
      AE_L16X2M_IU(y1, castxcc(const ae_p16x2s, px), 2 * 2);
      AE_L16X2M_IU(y2, castxcc(const ae_p16x2s, px), 2 * 2);
      AE_L16X2M_IU(y3, castxcc(const ae_p16x2s, px), 2 * 2);
    }
    A0r = AE_MULZASD32X16_H3_L2(y0, z0);
    A0i = AE_MULZAAD32X16_H2_L3(y0, z0);
    AE_MULSAD32X16_H1_L0(A0r, y1, z0);
    AE_MULSSD32X16_H1_L0(A0i, y1, z2);

    AE_MULASD32X16_H3_L2(A0r, y2, z1);
    AE_MULAAD32X16_H2_L3(A0i, y2, z1);
    AE_MULSAD32X16_H1_L0(A0r, y3, z1);
    AE_MULSSD32X16_H1_L0(A0i, y3, z3);

    t0 = AE_TRUNCA32X2F64S(A0i, A0r, 33 - 8);
    z0 = AE_ROUND16X4F32SASYM(t0, t0);
    t0 = AE_MOVINT32X2_FROMINT16X4(z0);
    AE_S32_L_IP(t0, castxcc(ae_int32, pz), 4);
    x += 16 * blkLen;
  }
} /* cmtx_det4x4_16x16() */


