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
/*
   Matrix multiply
 * Optimized code for Fusion
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common helper macros. */
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, mtx_vecmpyf_fast, (float32_t * z, const float32_t * x, const float32_t * y, int M, int N))
#else
/*-------------------------------------------------------------------------
  Matrix by Vector Multiply
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and vector y. 

  Two versions of functions available: regular version (mtx_vecmpy32x32, 
  mtx_vecmpy24x24, mtx_vecmpy16x16,mtx_vecmpyf) with arbitrary arguments 
  and faster version (mtx_vecmpy32x32_fast, mtx_vecmpy24x24_fast, 
  mtx_vecmpy16x16_fast, mtx_vecmpyf_fast) that apply some restrictions.

  Precision: 
  32x32 32-bit inputs, 32-bit output
  24x24 24-bit inputs, 24-bit output
  16x16 16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N] input matrix,Q31,Q15 or floating point
  y[N]   input vector,Q31,Q15 or floating point
  M      number of rows in matrix x
  N      number of columns in matrix x
  lsh    additional left shift
  Output:
  z[M]   output vector,Q31,Q15 or floating point

  Restriction:
  For regular routines (mtx_vecmpy24x24, mtx_vecmpy16x16, mtx_vecmpyf)
  x,y,z should not overlap

  For faster routines (mtx_vecmpy24x24_fast, mtx_vecmpy16x16_fast,
  mtx_vecmpyf_fast)
  x,y,z should not overlap
  x,y   aligned on 8-byte boundary
  N and M are multiples of 4
-------------------------------------------------------------------------*/
void mtx_vecmpyf_fast( float32_t * z, const float32_t * x,  const float32_t * y, int M, int N )
{
  int m, n;
  const xtfloatx2 * restrict pX0;
  const xtfloatx2 * restrict pX1;
  const xtfloatx2 * restrict pX2;
  const xtfloatx2 * restrict pX3;
  const xtfloatx2 * restrict pY;
        xtfloat * restrict pZ;
  xtfloat A0, A1, A2, A3;
  xtfloat A4, A5, A6, A7;
  xtfloatx2 X0, X1, X2, X3, Y0;
  ae_valign za;
  NASSERT_ALIGN(x, 8);
  NASSERT_ALIGN(y, 8);
  NASSERT(M % 4 == 0);
  NASSERT(N % 4 == 0);
  if (M <= 0) return;
  /* If N<4 then clear output veector and return */
  if (N < 4)
  {
    xtfloatx2 _0;

    pZ = (xtfloat *)z;
    za = AE_ZALIGN64();
    _0 = XT_CONST_S(0);
    for (n = 0; n < (M >> 1); n++)
    {
      XT_SASX2IP(_0, za, castxcc(xtfloatx2, pZ));
    }
    XT_SASX2POSFP(za, (xtfloatx2 *)pZ); 
    return;
  }
  pZ = (xtfloat *)z;
  pY = (const xtfloatx2 *)y;
  /* set circular buffer boundaries */
  WUR_AE_CBEGIN0((uintptr_t)(y + 0));
  WUR_AE_CEND0((uintptr_t)(y + N));
  for (m = 0; m<(M >> 2); m++)
  {
    pX0 = (const xtfloatx2 *)(x + 4 * m * N + 0*N);
    pX1 = (const xtfloatx2 *)(x + 4 * m * N + 1*N);
    pX2 = (const xtfloatx2 *)(x + 4 * m * N + 2*N);
    pX3 = (const xtfloatx2 *)(x + 4 * m * N + 3*N);

    A0 = A1 = A2 = A3 = XT_CONST_S(0);
    A4 = A5 = A6 = A7 = XT_CONST_S(0);

    for (n = 0; n<(N >> 1); n++)
    {
      XT_LSX2IP(X0, pX0, 2 * sizeof(float32_t));
      XT_LSX2IP(X1, pX1, 2 * sizeof(float32_t));
      XT_LSX2IP(X2, pX2, 2 * sizeof(float32_t));
      XT_LSX2IP(X3, pX3, 2 * sizeof(float32_t));
      XT_LSX2XC(Y0, pY, 2 * sizeof(float32_t));

      XT_MADD_LHH_S(A0, X0, Y0);
      XT_MADD_LLL_S(A2, X0, Y0);
      XT_MADD_LHH_S(A1, X1, Y0);
      XT_MADD_LLL_S(A3, X1, Y0);
      XT_MADD_LHH_S(A4, X2, Y0);
      XT_MADD_LLL_S(A5, X2, Y0);
      XT_MADD_LHH_S(A6, X3, Y0);
      XT_MADD_LLL_S(A7, X3, Y0);
    }
    A0 = XT_ADD_S(A0, A2);
    A1 = XT_ADD_S(A1, A3);
    A2 = XT_ADD_S(A4, A5);
    A3 = XT_ADD_S(A6, A7);
    XT_SSIP(A0, pZ, sizeof(float32_t));
    XT_SSIP(A1, pZ, sizeof(float32_t));
    XT_SSIP(A2, pZ, sizeof(float32_t));
    XT_SSIP(A3, pZ, sizeof(float32_t));
  }
} /* mtx_vecmpyf_fast() */
#endif
