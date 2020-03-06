/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ("Cadence    */
/* Libraries") are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
  NatureDSP Signal Processing Library. Matrix Operations
   Matrix multiply
 * Optimized code for HiFi4
  IntegrIT, 2006-2018
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_matop.h"
/* Common helper macros. */
#include "common.h"
#include "common_fpu.h"

#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(void,mtx_mpyf_fast,(void* pScr, float32_t * z, const float32_t * x,  const float32_t * y, int M, int N, int P ))
#elif (HAVE_VFPU)
/*-------------------------------------------------------------------------
  Matrix Multiply
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and y. The columnar dimension of x must match the row dimension of y. 
  The resulting matrix has the same number of rows as x and the same number 
  of columns as y.
  Transposing API allows to interpret input yt as transposed matrix y.

  NOTE: lsh factor is not relevant for floating point routines.

  Functions require scratch memory for storing intermediate data. This 
  scratch memory area should be aligned on 8 byte boundary and its size is 
  calculated by dedicated scratch allocation functions.

  Two versions of functions available: regular version (mtx_mpy[t]32x32, 
  mtx_mpy[t]16x16, mtx_mpy[t]8x16, mtx_mpy[t]8x8, mtx[t]_mpyf) with 
  arbitrary arguments and faster version (mtx_mpy[t]32x32_fast, 
  mtx_mpy[t]16x16_fast, mtx_mpy[t]8x16_fast, mtx_mpy[t]8x8_fast, 
  mtx_mpy[t]f_fast) that apply some restrictions

  Precision:
  32x32 32-bit inputs, 32-bit output
  16x16 16-bit inputs, 16-bit output
  8x8   8-bit inputs, 8-bit output
  8x16  8/16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N]      input matrix x, Q7, Q15, Q31 or floating point
  y[N*P]      input matrix y, Q7, Q15, Q31 or floating point
  yt[P*N]     transposed input matrix y. Q31,Q15, Q7 floating point. (for 
              transposing API only)
  M           number of rows in matrix x and z
  N           number of columns in matrix x and number of rows in matrix y
  P           number of columns in matrices y and z
  lsh         left shift applied to the result (applied to the fixed-
              point functions only) 
  Output:
  z[M*P]      output matrix z, Q7, Q15, Q31 or floating point 
  Scratch:
  pScr        size in bytes defined by corresponding scratch allocation 
              functions

  Restrictions:
  For regular routines mpy[t]32x32, mtx_mpy[t]16x16, mtx_mpy[t]8x8, 
  mtx_mpy[t]8x16, mtx_mpy[t]f):
  pScr    aligned on 8-byte boundary
  x,y,z   should not overlap

  For faster routines (mtx_mpy[t]32x32_fast, mtx_mpy[t]16x16_fast, 
  mtx_mpy[t]8x8_fast, mtx_mpy[t]8x16_fast, 
  mtx_mpy[t]f_fast):
  x,y,z        should not overlap
  x,y,z,pScr   aligned on 8-byte boundary
  M,N,P        multiplies of 4
  lsh         should be in range:
              -31...31 for mtx_mpy32x32, mtx_mpy32x32_fast
              -15...15 for mtx_mpy16x16, mtx_mpy16x16_fast, mtx_mpy[t]8x8, 
                       mtx_mpy[t]8x8_fast, mtx_mpy[t]8x16, 
                       mtx_mpy[t]8x16_fast 

-------------------------------------------------------------------------*/
#define SZ_F32 (sizeof(float32_t))

void mtx_mpyf_fast(void* pScr, float32_t * z, const float32_t * x,  const float32_t * y, int M, int N, int P )
{
    const xtfloatx2 *restrict pX0;
    const xtfloatx2 *restrict pX1;
    const xtfloatx2 *restrict pX2;
    const xtfloatx2 *restrict pX3;
    const xtfloatx2 *restrict pY;
          xtfloatx2 *restrict pZ;
    xtfloatx2 y0, y1, t;
    xtfloatx2 x0, x1, x2, x3;
    xtfloatx2 acc00, acc01, acc10, acc11, acc20, acc21, acc30, acc31;
    xtfloatx2 z00, z10, z20, z30, z01, z11, z21, z31;
    int m, n, p;

    (void)pScr;
    NASSERT(x);
    NASSERT(y);
    NASSERT(z);
    NASSERT((z != x) && (z != y));
    NASSERT(N % 4 == 0);
    NASSERT(M % 4 == 0);
    NASSERT(P % 4 == 0);
    NASSERT_ALIGN(x, 8);
    NASSERT_ALIGN(y, 8);
    NASSERT_ALIGN(z, 8);

    if ((M <= 0) || (P <= 0)) return;
    if (N <= 0)
    {
        z00 = (xtfloatx2)0.0f;
        pZ = (xtfloatx2 *)(z);
        for (m = 0; m < ((M * P) >> 1); m++)
        {
            XT_SSX2IP(z00, pZ, SZ_F32 * 2);
        }
        return;
    }

    for (p = 0; p < P; p += 2)
    {
        pZ = (xtfloatx2 *)(z);
        pX3 = (const xtfloatx2 *)(x);

        for (m = 0; m < M; m += 4)
        {
            pX0 = (const xtfloatx2 *)pX3;
            pX1 = (const xtfloatx2 *)((float32_t *)pX0 + N);
            pX2 = (const xtfloatx2 *)((float32_t *)pX1 + N);
            pX3 = (const xtfloatx2 *)((float32_t *)pX2 + N);
            pY = (const xtfloatx2 *)(y);

            acc00 = acc01 = acc10 = acc11 =
                acc20 = acc21 = acc30 = acc31 = (xtfloatx2)0.0f;

            /* Innermost loop: compute 2 values for 4 rows */
            //__Pragma("loop_count factor=2");
            __Pragma("loop_count min=2");
            for (n = 0; n < (N >> 1); n++)
            {
                XT_LSX2IP(x0, pX0, SZ_F32 * 2);
                XT_LSX2IP(x1, pX1, SZ_F32 * 2);
                XT_LSX2IP(x2, pX2, SZ_F32 * 2);
                XT_LSX2IP(x3, pX3, SZ_F32 * 2);

                XT_LSX2XP(y0, pY, SZ_F32*P);
                XT_LSX2XP(y1, pY, SZ_F32*P);

                t = XT_SEL32_LL_SX2(y0, y1);
                y0 = XT_SEL32_HH_SX2(y0, y1);
                y1 = t;

                XT_MADD_SX2(acc00, x0, y0);
                XT_MADD_SX2(acc01, x0, y1);
                XT_MADD_SX2(acc10, x1, y0);
                XT_MADD_SX2(acc11, x1, y1);
                XT_MADD_SX2(acc20, x2, y0);
                XT_MADD_SX2(acc21, x2, y1);
                XT_MADD_SX2(acc30, x3, y0);
                XT_MADD_SX2(acc31, x3, y1);
            }
            z00 = XT_SEL32_HL_SX2(acc00, acc01);
            z01 = XT_SEL32_LH_SX2(acc00, acc01);
            z00 = z00 + z01;
            z10 = XT_SEL32_HL_SX2(acc10, acc11);
            z11 = XT_SEL32_LH_SX2(acc10, acc11);
            z10 = z10 + z11;
            z20 = XT_SEL32_HL_SX2(acc20, acc21);
            z21 = XT_SEL32_LH_SX2(acc20, acc21);
            z20 = z20 + z21;
            z30 = XT_SEL32_HL_SX2(acc30, acc31);
            z31 = XT_SEL32_LH_SX2(acc30, acc31);
            z30 = z30 + z31;

            XT_SSX2XP(z00, pZ, SZ_F32 * P);
            XT_SSX2XP(z10, pZ, SZ_F32 * P);
            XT_SSX2XP(z20, pZ, SZ_F32 * P);
            XT_SSX2XP(z30, pZ, SZ_F32 * P);
        }
        y += 2;
        z += 2;
    }
} /* mtx_mpyf_fast() */
#elif (HAVE_FPU)
void mtx_mpyf_fast(void* pScr, float32_t * z, const float32_t * x,  const float32_t * y, int M, int N, int P )
{
          xtfloat * restrict pZ;
    const xtfloat * restrict pX;
    const xtfloat * restrict pY;
    int m,n,p;
    (void)pScr;
    NASSERT(x);
    NASSERT(y);
    NASSERT(z);
    NASSERT((z != x) && (z != y));
    NASSERT(N % 4 == 0);
    NASSERT(M % 4 == 0);
    NASSERT(P % 4 == 0);
    NASSERT_ALIGN(x, 8);
    NASSERT_ALIGN(y, 8);
    NASSERT_ALIGN(z, 8);

    if ((M <= 0) || (P <= 0)) return;
    if (N <= 0)
    {
        pZ = (xtfloat *)(z);
        for (m = 0; m < (M * P); m++)
        {
            XT_SSIP(XT_CONST_S(0), pZ, sizeof(xtfloat));
        }
        return;
    }
    pZ = (xtfloat  *)(z);
    for (m = 0; m<M; m++)
    {
      for (p = 0; p<P; p+=4)
      {
        xtfloat A0,A1,A2,A3,x0,y0,y1,y2,y3;
        pX = (const xtfloat  *)(x+m*N);
        pY = (const xtfloat  *)(y+p);
        A0=A1=A2=A3=XT_CONST_S(0);
        for ( n=0; n<N; n++ )
        {
          XT_LSIP(x0, pX, sizeof(xtfloat));
          y1=XT_LSI(pY, 1*sizeof(xtfloat));
          y2=XT_LSI(pY, 2*sizeof(xtfloat));
          y3=XT_LSI(pY, 3*sizeof(xtfloat));
          XT_LSXP(y0, pY, P*sizeof(xtfloat));
          XT_MADD_S(A0, x0, y0);
          XT_MADD_S(A1, x0, y1);
          XT_MADD_S(A2, x0, y2);
          XT_MADD_S(A3, x0, y3);
        }
        XT_SSIP(A0, pZ, sizeof(xtfloat));
        XT_SSIP(A1, pZ, sizeof(xtfloat));
        XT_SSIP(A2, pZ, sizeof(xtfloat));
        XT_SSIP(A3, pZ, sizeof(xtfloat));
      }
    }
}

#endif


size_t mtx_mpyf_fast_getScratchSize( int M, int N, int P )
{
    (void)M;(void)N;(void)P;
    return 0;
}
