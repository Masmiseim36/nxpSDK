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
/* real matrix transpose */
void mtx_tran3x3_16x16(int16_t   * restrict z, const int16_t   *restrict x, int L)
{
#if 1
  int l;
  ae_int16x4    *  pz = (ae_int16x4      *)z;
  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  ae_int16x4 x0, x1, x2, x3, x4, x5, x6, x7, x8;
  ae_int16x4 z0, z1, z2, z3, z4, z5, z6, z7, z8;
  ae_int32x2 h0, h1, h2, h3, h4, h5, h6, h7, h8;
  ae_int32x2 c0, c1;
  ae_valign xa, za;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(z, sizeof(*z));
  if (L <= 0) return;
  xa = AE_LA64_PP(px);
  za = AE_ZALIGN64();
  for (l = 0; l<(L >> 2); l++)
  {
    AE_LA16X4_IP(x0, xa, px);
    AE_LA16X4_IP(x1, xa, px);
    AE_LA16X4_IP(x2, xa, px);
    AE_LA16X4_IP(x3, xa, px);
    AE_LA16X4_IP(x4, xa, px);
    AE_LA16X4_IP(x5, xa, px);
    AE_LA16X4_IP(x6, xa, px);
    AE_LA16X4_IP(x7, xa, px);
    AE_LA16X4_IP(x8, xa, px);

    h0 = AE_SEXT32X2D16_32(x0);
    h1 = AE_SEXT32X2D16_10(x0);
    h2 = AE_SEXT32X2D16_32(x1);
    h3 = AE_SEXT32X2D16_10(x1);
    h4 = AE_SEXT32X2D16_32(x2);
    h5 = AE_SEXT32X2D16_10(x2);
    h6 = AE_SEXT32X2D16_32(x3);
    h7 = AE_SEXT32X2D16_10(x3);
    h8 = AE_SEXT32X2D16_32(x4);

    c0 = AE_SEL32_HL(h0, h1);
    c1 = AE_SEL32_HL(h3, h0);
    z0 = AE_SEL16_6420(AE_MOVINT16X4_FROMINT32X2(c0), AE_MOVINT16X4_FROMINT32X2(c1));

    c0 = AE_SEL32_HL(h2, h3);
    c1 = AE_SEL32_HL(h1, h2);
    z1 = AE_SEL16_6420(AE_MOVINT16X4_FROMINT32X2(c0), AE_MOVINT16X4_FROMINT32X2(c1));

    c0 = h4;
    c1 = AE_SEL32_HL(h6, h7);
    z2 = AE_SEL16_6420(AE_MOVINT16X4_FROMINT32X2(c0), AE_MOVINT16X4_FROMINT32X2(c1));

    c0 = AE_SEL32_HL(h5, h6);
    c1 = AE_SEL32_HL(h8, h5);
    z3 = AE_SEL16_6420(AE_MOVINT16X4_FROMINT32X2(c0), AE_MOVINT16X4_FROMINT32X2(c1));

    c0 = AE_SEL32_HL(h7, h8);
    
    h0 = AE_SEXT32X2D16_10(x4);
    h1 = AE_SEXT32X2D16_32(x5);
    h2 = AE_SEXT32X2D16_10(x5);
    h3 = AE_SEXT32X2D16_32(x6);
    h4 = AE_SEXT32X2D16_10(x6);
    h5 = AE_SEXT32X2D16_32(x7);
    h6 = AE_SEXT32X2D16_10(x7);
    h7 = AE_SEXT32X2D16_32(x8);
    h8 = AE_SEXT32X2D16_10(x8);
     
    c1 = AE_SEL32_HL(h0, h1);
    z4 = AE_SEL16_6420(AE_MOVINT16X4_FROMINT32X2(c0), AE_MOVINT16X4_FROMINT32X2(c1));
    
    c0 = AE_SEL32_HL(h3, h0);
    c1 = AE_SEL32_HL(h2, h3);
    z5 = AE_SEL16_6420(AE_MOVINT16X4_FROMINT32X2(c0), AE_MOVINT16X4_FROMINT32X2(c1));
    
    c0 = AE_SEL32_HL(h1, h2);
    c1 = h4;
    z6 = AE_SEL16_6420(AE_MOVINT16X4_FROMINT32X2(c0), AE_MOVINT16X4_FROMINT32X2(c1));
    
    c0 = AE_SEL32_HL(h6, h7);
    c1 = AE_SEL32_HL(h5, h6);
    z7 = AE_SEL16_6420(AE_MOVINT16X4_FROMINT32X2(c0), AE_MOVINT16X4_FROMINT32X2(c1));
    
    c0 = AE_SEL32_HL(h8, h5);
    c1 = AE_SEL32_HL(h7, h8);
    z8 = AE_SEL16_6420(AE_MOVINT16X4_FROMINT32X2(c0), AE_MOVINT16X4_FROMINT32X2(c1));

    AE_SA16X4_IP(z0, za, pz);
    AE_SA16X4_IP(z1, za, pz);
    AE_SA16X4_IP(z2, za, pz);
    AE_SA16X4_IP(z3, za, pz);
    AE_SA16X4_IP(z4, za, pz);
    AE_SA16X4_IP(z5, za, pz);
    AE_SA16X4_IP(z6, za, pz);
    AE_SA16X4_IP(z7, za, pz);
    AE_SA16X4_IP(z8, za, pz);
  }
  AE_SA64POS_FP(za, pz);
  l = 4 * l;
  pz = (ae_int16x4      *)(z + 9 * l);
  za = AE_ZALIGN64();
  for (;l<L;l++)
  {
    px = (const ae_int16x4      *)(x + 9 * l);
    xa = AE_LA64_PP(px);

    AE_LA16X4_IP(x0, xa, px);
    AE_LA16X4_IP(x1, xa, px);
    AE_LA16X4_IP(x2, xa, px);

    h0 = AE_SEXT32X2D16_32(x0);
    h1 = AE_SEXT32X2D16_10(x0);
    h2 = AE_SEXT32X2D16_32(x1);
    h3 = AE_SEXT32X2D16_10(x1);

    c0 = AE_SEL32_HL(h0, h1);
    c1 = AE_SEL32_HL(h3, h0);
    z0 = AE_SEL16_6420(AE_MOVINT16X4_FROMINT32X2(c0), AE_MOVINT16X4_FROMINT32X2(c1));

    c0 = AE_SEL32_HL(h2, h3);
    c1 = AE_SEL32_HL(h1, h2);
    z1 = AE_SEL16_6420(AE_MOVINT16X4_FROMINT32X2(c0), AE_MOVINT16X4_FROMINT32X2(c1));

    AE_SA16X4_IP(z0, za, pz);
    AE_SA16X4_IP(z1, za, pz);
    AE_SA64POS_FP(za, pz);
    x2 = AE_SHORTSWAP(x2);
    AE_S16_0_IP(x2, castxcc(ae_int16, pz), 2);
  }
#else
  int l;
  ae_int16x4    *  pz = (ae_int16x4      *)z;
  const ae_int16x4    *  restrict px = (const ae_int16x4      *)x;
  ae_int16x4 x0, x1, x2, x3, x4, x5, x6, x7, x8, x_;
  ae_int16x4 t0, t1, t2, t3, t4, t5, t6, t7, t8;
  ae_int16x4 d0, d1, d2, d3, d4, d5, d6, d7, d8;
  ae_int16x4 z0, z1, z2, z3, z4, z5, z6, z7, z8;
  ae_valign xa, za;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(z, sizeof(*z));
  if (L <= 0) return;
  xa = AE_LA64_PP(px);
  za = AE_ZALIGN64();

  for (l = 0; l<(L>>2); l++)
  {
    AE_LA16X4_IP(x0, xa, px);
    AE_LA16X4_IP(x1, xa, px);
    AE_LA16X4_IP(x2, xa, px);
    AE_LA16X4_IP(x3, xa, px);
    AE_LA16X4_IP(x4, xa, px);
    AE_LA16X4_IP(x5, xa, px);
    AE_LA16X4_IP(x6, xa, px);
    AE_LA16X4_IP(x7, xa, px);
    AE_LA16X4_IP(x8, xa, px);


    t0 = AE_SEL16_4321(x0, x0);
    t1 = AE_SEL16_4321(x1, x1);
    d0 = AE_SEL16_4321(t1, t0);
    z0 = AE_SEL16_4321(d0, d0);
    z0 = AE_SHORTSWAP(z0);
    d1 = AE_SEL16_4321(t0, t1);
    z1 = AE_SEL16_4321(d1, d1);
    z1 = AE_SHORTSWAP(z1);

    t2 = AE_SEL16_6543(x3, x3);
    d2 = AE_SEL16_7632(x2, x3);
    z2 = AE_SEL16_4321(x3, d2);
    z2 = AE_SEL16_6543(z2, z2);

    t3 = AE_SEL16_6543(x3, x4);
    d3 = AE_SEL16_6543(t3, t3);
    z3 = AE_SEL16_5410(d3, x2);
    z3 = AE_SEL16_4321(z3, z3);
    z3 = AE_SHORTSWAP(z3);

    t4 = AE_SEL16_7610(x5, x3);
    t4 = AE_SEL16_4321(t4, t4);
    d4 = AE_SEL16_6543(x4, x4);
    z4 = AE_SEL16_7610(d4, t4);
    z4 = AE_SEL16_4321(z4, z4);

    t5 = AE_SEL16_6543(x4, x6);
    d5 = AE_SEL16_7610(x6, x5);
    d5 = AE_SEL16_4321(d5, d5);
    z5 = AE_SEL16_5410(d5, t5);
    z5 = AE_SHORTSWAP(z5);

    t6 = AE_SEL16_6543(x5, x5);
    t6 = AE_SHORTSWAP(t6);
    z6 = AE_SEL16_7610(t6, x6);

    t7 = AE_SEL16_4321(x7, x7);
    d7 = AE_SEL16_7610(x8, x7);
    z7 = AE_SEL16_4321(d7, d7);
    z7 = AE_SEL16_7610(t7, z7);
    z7 = AE_SHORTSWAP(z7);

    t8 = AE_SEL16_6543(x8, x8);
    d8 = AE_SEL16_7610(x7, x8);
    z8 = AE_SEL16_4321(d8, d8);
    z8 = AE_SEL16_5410(t8, z8);
    z8 = AE_SHORTSWAP(z8);

    AE_SA16X4_IP(z0, za, pz);
    AE_SA16X4_IP(z1, za, pz);
    AE_SA16X4_IP(z2, za, pz);
    AE_SA16X4_IP(z3, za, pz);
    AE_SA16X4_IP(z4, za, pz);
    AE_SA16X4_IP(z5, za, pz);
    AE_SA16X4_IP(z6, za, pz);
    AE_SA16X4_IP(z7, za, pz);
    AE_SA16X4_IP(z8, za, pz);
  }
  AE_SA64POS_FP(za, pz); l = 4 * l;
  pz = (ae_int16x4      *)(z + 9 * l);
  za = AE_ZALIGN64();
  for (;l<L;l++)
  {
    px = (const ae_int16x4      *)(x+9*l);
    xa = AE_LA64_PP(px);
    
    AE_LA16X4_IP(x0, xa, px);
    AE_LA16X4_IP(x1, xa, px);
    AE_LA16X4_IP(x2, xa, px);

    t0 = AE_SEL16_4321(x0, x0);
    t1 = AE_SEL16_4321(x1, x1);
    d0 = AE_SEL16_4321(t1, t0);
    z0 = AE_SEL16_4321(d0, d0);
    z0 = AE_SHORTSWAP(z0);
    d1 = AE_SEL16_4321(t0, t1);
    z1 = AE_SEL16_4321(d1, d1);
    z1 = AE_SHORTSWAP(z1);
    AE_SA16X4_IP(z0, za, pz);
    AE_SA16X4_IP(z1, za, pz);
    AE_SA64POS_FP(za, pz);
    x2 = AE_SHORTSWAP(x2);
    AE_S16_0_IP(x2,pz,2); 
  }
#endif
} /* mtx_tran3x3_16x16() */



