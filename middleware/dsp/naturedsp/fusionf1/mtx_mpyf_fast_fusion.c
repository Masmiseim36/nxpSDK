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
DISCARD_FUN(void, mtx_mpyf_fast, (float32_t * z, const float32_t * x, const float32_t * y, int M, int N, int P))
#else
/*-------------------------------------------------------------------------
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and y. The columnar dimension of x must match the row dimension of y. 
  The resulting matrix has the same number of rows as x and the same number 
  of columns as y.

  NOTES:
  In the fixed-point routines, rows of matrices z and y may be stored in 
  non consecutive manner. Matrix x will have all the elements in contiguous 
  memory locations.

  Functions require scratch memory for storing intermediate data. This 
  scratch memory area should be aligned on 8 byte boundary and its size 
  is calculated by macros SCRATCH_MTX_MPY32X32(M,N,P), 
  SCRATCH_MTX_MPY24X24(M,N,P), SCRATCH_MTX_MPY16X16(M,N,P)

  Two versions of functions available: regular version (mtx_mpy32x32, 
  mtx_mpy24x24, mtx_mpy16x16, mtx_mpyf) with arbitrary arguments and 
  faster version (mtx_mpy32x32_fast, mtx_mpy24x24_fast, 
  mtx_mpy16x16_fast, mtx_mpyf_fast) that apply some restrictions.

  Precision: 
  32x32 32-bit inputs, 32-bit output
  24x24 24-bit inputs, 24-bit output
  16x16 16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N]      input matrix,Q31, Q15 or floating point
  y[N][P]     input matrix y. For fixed point routines, these are N 
              vectors of size P,Q31 or Q15. For floating point, this 
              is just a matrix of size NxP.
  M           number of rows in matrix x and z
  N           number of columns in matrix x and number of rows in matrix y
  P           number of columns in matrices y and z
  lsh         additional left shift
  Output:
  z[M][P]     output matrix z. For fixed point routines, these are M 
              vectors of size P Q31 or Q15. For floating point, this 
              is single matrix of size MxP
  Scratch:
  pScr        Scratch memory area with size in bytes defined by macros 
              SCRATCH_MTX_MPY32X32, SCRATCH_MTX_MPY24X24, 
              SCRATCH_MTX_MPY16X16


  Restrictions:
  For regular routines (mtx_mpy32x32,mtx_mpy24x24, mtx_mpy16x16, mtx_mpyf):
  x,y,z should not overlap

  For faster routines (mtx_mpy32x32_fast, mtx_mpy24x24_fast, 
  mtx_mpy16x16_fast, mtx_mpyf_fast):
  x,y,z should not overlap
  x - aligned on 8-byte boundary
  all rows which addresses are written to y[] - aligned on 8-byte boundary
  N is a multiple of 4,M=8,P=2  

-------------------------------------------------------------------------*/
void mtx_mpyf_fast( float32_t * z, const float32_t * x,  const float32_t * y, int M, int N, int P )
{
  int m, n;
  const xtfloat * restrict pX0;
  const xtfloat * restrict pX1;
  const xtfloat * restrict pX2;
  const xtfloat * restrict pX3;
  const xtfloat * restrict pY;
        xtfloat * restrict pZ;
  xtfloat A0, A1, A2, A3;
  xtfloat A4, A5, A6, A7;
  xtfloat X0, X1, X2, X3, Y0, Y1;
  ae_valign za;
  NASSERT(x);
  NASSERT(y);
  NASSERT(z);
  NASSERT((z != x) && (z != y));
  NASSERT(N % 4 == 0 && M == 8 && P == 2);
  NASSERT_ALIGN(x, 8);
  NASSERT_ALIGN(y, 8);
  if (M <= 0 || P <= 0) return;
  /* If N<4 then clear output matrix and return */
  if (N < 4)
  {
    xtfloatx2 _0;
    pZ = (xtfloat *)z;
    za = AE_ZALIGN64();
    _0 = XT_CONST_S(0);
    for (n = 0; n < (8); n++)
    {
      XT_SASX2IP(_0, za, castxcc(xtfloatx2, pZ));
    }
    XT_SASX2POSFP(za, (xtfloatx2 *)pZ);
    return;
  }
  /* set circular buffer boundaries */
  WUR_AE_CBEGIN0((uintptr_t)(y + 0));
  WUR_AE_CEND0((uintptr_t)(y + 2*N));
  pZ = (xtfloat *)z;
  pY = (const xtfloat *)(y);
  for (m = 0; m<(2); m++)
  {
    pX0 = (const xtfloat *)(x + 4 * m * N + 0 * N);
    pX1 = (const xtfloat *)(x + 4 * m * N + 1 * N);
    pX2 = (const xtfloat *)(x + 4 * m * N + 2 * N);
    pX3 = (const xtfloat *)(x + 4 * m * N + 3 * N);
    
    A0 = A1 = A2 = A3 = XT_CONST_S(0);
    A4 = A5 = A6 = A7 = XT_CONST_S(0);
    for (n = 0; n<N; n++)
    {
      XT_LSIP(X0, pX0, sizeof(float32_t));
      XT_LSIP(X1, pX1, sizeof(float32_t));
      XT_LSIP(X2, pX2, sizeof(float32_t));
      XT_LSIP(X3, pX3, sizeof(float32_t));
      XT_LSXC(Y0, pY, sizeof(float32_t));
      XT_LSXC(Y1, pY, sizeof(float32_t));

      XT_MADD_S(A0, X0, Y0);
      XT_MADD_S(A1, X0, Y1);
      XT_MADD_S(A2, X1, Y0);
      XT_MADD_S(A3, X1, Y1);
      XT_MADD_S(A4, X2, Y0);
      XT_MADD_S(A5, X2, Y1);
      XT_MADD_S(A6, X3, Y0);
      XT_MADD_S(A7, X3, Y1);
    }
    XT_SSIP(A0, pZ, sizeof(float32_t));
    XT_SSIP(A1, pZ, sizeof(float32_t));
    XT_SSIP(A2, pZ, sizeof(float32_t));
    XT_SSIP(A3, pZ, sizeof(float32_t));
    XT_SSIP(A4, pZ, sizeof(float32_t));
    XT_SSIP(A5, pZ, sizeof(float32_t));
    XT_SSIP(A6, pZ, sizeof(float32_t));
    XT_SSIP(A7, pZ, sizeof(float32_t));
  }
} /* mtx_mpyf_fast() */
#endif
