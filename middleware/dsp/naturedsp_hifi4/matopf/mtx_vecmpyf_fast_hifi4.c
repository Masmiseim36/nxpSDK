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
#include "common_fpu.h"

#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(void ,mtx_vecmpyf_fast,( float32_t * z, const float32_t * x,  const float32_t * y, int M, int N ))
#elif (HAVE_VFPU)
/*-------------------------------------------------------------------------
  Matrix by Vector Multiply
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and vector y. 
  NOTE: lsh factor is not relevant for floating point routines.

  Two versions of functions available: regular version (mtx_vecmpy32x32,  
  mtx_vecmpy16x16, mtx_vecmpy8x8, mtx_vecmpy8x16, mtx_vecmpyf) with arbitrary 
  arguments and faster version (mtx_vecmpy32x32_fast, mtx_vecmpy16x16_fast, 
  mtx_vecmpy8x8_fast, mtx_vecmpy8x16_fast,  mtx_vecmpyf_fast) that apply 
  some restrictions

  Precision: 
  32x32 32-bit input, 32-bit output
  16x16 16-bit input, 16-bit output
  8x8   8-bit inputs, 8-bit output
  8x16  8/16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N] input matrix,Q31,Q15 or floating point
  y[N]   input vector,Q31,Q15 or floating point
  M      number of rows in matrix x
  N      number of columns in matrix x
  lsh    additional left shift(applied to the fixed-
         point functions only) 
  Output:
  z[M]   output vector,Q31,Q15 or floating point

  Restriction:
  For regular routines (mtx_vecmpy32x32, mtx_vecmpy16x16, mtx_vecmpy8x8,
  mtx_vecmpy8x16,  mtx_vecmpyf)
  x,y,z should not overlap

  For faster routines  (mtx_vecmpy32x32_fast, mtx_vecmpy16x16_fast, 
  mtx_vecmpy8x8_fast, mtx_vecmpy8x16_fast, mtx_vecmpyf_fast)
  x,y,z   should not overlap
  x,y     aligned on 8-byte boundary
  N, M    multiples of 4
  lsh     should be in range:
          -31...31 for mtx_vecmpy32x32, mtx_vecmpy32x32_fast
          -15...15 for mtx_vecmpy16x16, mtx_vecmpy16x16_fast, 
                   mtx_vecmpy8x8_fast, mtx_vecmpy8x16_fast   
-------------------------------------------------------------------------*/
#define SZ_F32 (sizeof(float32_t))
void mtx_vecmpyf_fast( float32_t * z, const float32_t * x,  const float32_t * y, int M, int N )
{
  const xtfloatx2 *restrict px0;
  const xtfloatx2 *restrict px1;
  const xtfloatx2 *restrict px2;
  const xtfloatx2 *restrict px3;
  const xtfloatx2 *restrict py;
        xtfloatx2 *restrict pz;
  xtfloatx2 y0, y1;
  xtfloatx2 z0, z1;
  xtfloatx2 x00, x01, x10, x11,
            x20, x21, x30, x31;
  xtfloatx2 acc00, acc01, acc10, acc11,
            acc20, acc21, acc30, acc31;
  ae_valign vz;
  int m, n;

  NASSERT(x);
  NASSERT(y);
  NASSERT(z);
  NASSERT((z != x) && (z != y));
  NASSERT_ALIGN(x,8);
  NASSERT_ALIGN(y,8);
  NASSERT(M%4==0);
  NASSERT(N%4==0);

  if (M < 4)    return;

  pz = (xtfloatx2 *)z;
  vz = AE_ZALIGN64();
  /* If N<4 then clear output veector and return */
  if (N < 4)
  {
    z0 = (xtfloatx2)0.0f;
    __Pragma("loop_count min=1")
    for (m = 0; m < (M>>1); m++)
    {
      XT_SASX2IP(z0, vz, pz);
    }
    XT_SASX2POSFP(vz, pz);
    return;
  }
  px0 = (const xtfloatx2 *)(x);
  
  /* Compute by 4 values */
  __Pragma("loop_count min=1")
  for (m = 0; m < (M>>2); m++)
  {
    px1 = (const xtfloatx2 *)((float32_t *)px0+N);
    px2 = (const xtfloatx2 *)((float32_t *)px1+N);
    px3 = (const xtfloatx2 *)((float32_t *)px2+N);
    py  = (const xtfloatx2 *)(y);

    acc00 = acc01 = acc10 = acc11 =
    acc20 = acc21 = acc30 = acc31 =  (xtfloatx2)0.0f;

    __Pragma("loop_count min=1")
    for (n = 0; n < (N>>2); n++)
    {
      XT_LSX2IP(x00, px0, SZ_F32*2);
      XT_LSX2IP(x01, px0, SZ_F32*2);
      XT_LSX2IP(x10, px1, SZ_F32*2);
      XT_LSX2IP(x11, px1, SZ_F32*2);
      XT_LSX2IP(x20, px2, SZ_F32*2);
      XT_LSX2IP(x21, px2, SZ_F32*2);
      XT_LSX2IP(x30, px3, SZ_F32*2);
      XT_LSX2IP(x31, px3, SZ_F32*2);

      XT_LSX2IP( y0,  py, SZ_F32*2);
      XT_LSX2IP( y1,  py, SZ_F32*2);

      XT_MADD_SX2(acc00, x00, y0);
      XT_MADD_SX2(acc01, x01, y1);
      XT_MADD_SX2(acc10, x10, y0);
      XT_MADD_SX2(acc11, x11, y1);
      XT_MADD_SX2(acc20, x20, y0);
      XT_MADD_SX2(acc21, x21, y1);
      XT_MADD_SX2(acc30, x30, y0);
      XT_MADD_SX2(acc31, x31, y1);
    }
    acc00 = acc00 + acc01;
    acc10 = acc10 + acc11;
    y0 = XT_SEL32_HL_SX2(acc00, acc10);
    y1 = XT_SEL32_LH_SX2(acc00, acc10);
    z0 = y0 + y1;

    acc20 = acc20 + acc21;
    acc30 = acc30 + acc31;
    y0 = XT_SEL32_HL_SX2(acc20, acc30);
    y1 = XT_SEL32_LH_SX2(acc20, acc30);
    z1 = y0 + y1;

    XT_SASX2IP(z0, vz, pz);
    XT_SASX2IP(z1, vz, pz);
    /* Go to the next 4 rows of the input matrix */
    px0 = px3;
  }
  XT_SASX2POSFP(vz, pz);
} /* mtx_vecmpyf() */
#elif (HAVE_FPU)
void mtx_vecmpyf_fast( float32_t * z, const float32_t * x,  const float32_t * y, int M, int N )
{
    int m, n;
          xtfloat * restrict pZ;
    const xtfloat * restrict pX;
    const xtfloat * restrict pY;

    NASSERT(x);
    NASSERT(y);
    NASSERT(z);
    NASSERT((z != x) && (z != y));
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT(M%4==0);
    NASSERT(N%4==0);
    pZ=(xtfloat*)z;
    if (N<0)
    {
        for ( m=0; m<M; m++) XT_SSIP(XT_CONST_S(0),pZ,sizeof(xtfloat));
        return;
    }
    for ( m=0; m<M; m+=4)
    {
        xtfloat a0,a1,a2,a3,x0,x1,x2,x3,y0;
        a0=a1=a2=a3=XT_CONST_S(0);
        pX=(const xtfloat*)(x+m*N);
        pY=(const xtfloat*)(y);
        for ( n=0; n<N; n++ )
        {
            XT_LSIP(y0,pY,sizeof(xtfloat));
            x1=XT_LSX(pX,1*N*sizeof(xtfloat));
            x2=XT_LSX(pX,2*N*sizeof(xtfloat));
            x3=XT_LSX(pX,3*N*sizeof(xtfloat));
            XT_LSIP(x0,pX,sizeof(xtfloat));
            XT_MADD_S(a0,x0,y0);
            XT_MADD_S(a1,x1,y0);
            XT_MADD_S(a2,x2,y0);
            XT_MADD_S(a3,x3,y0);
        }
        XT_SSIP(a0,pZ,sizeof(xtfloat));
        XT_SSIP(a1,pZ,sizeof(xtfloat));
        XT_SSIP(a2,pZ,sizeof(xtfloat));
        XT_SSIP(a3,pZ,sizeof(xtfloat));
    }
}

#endif
