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
  NatureDSP Signal Processing Library. Fitting and Interpolation Routines
    Polynomial approximation
    C code optimized for HiFi3
  IntegrIT, 2006-2018
*/
#include "NatureDSP_types.h"
/* Library API */
#include "NatureDSP_Signal_fit.h"
#include "common.h"
#include "common_fpu.h"

#if (HAVE_VFPU==0 && HAVE_FPU==0)
DISCARD_FUN(void,vec_poly8f ,(float32_t * restrict z, const float32_t * restrict x, const float32_t * restrict c, int N ))
#elif (HAVE_VFPU)

/*-------------------------------------------------------------------------
  Polynomial approximation
  Functions calculate polynomial approximation for all values from given 
  vector. Fixed point functions take polynomial coefficients in Q31 precision. 
  NOTE:
  approximation is calculated like Taylor series that is why overflow may 
  potentially occur if cumulative sum of coefficients given from the last 
  to the first coefficient is bigger that 1. To avoid this negative effect,
  all the coefficients may be scaled down and result will be shifted left 
  after all intermediate computations.

  Precision: 
  24x24  24-bit inputs, 24-bit coefficients, 24-bit output. 
  32x32  32-bit inputs, 32-bit coefficients, 32-bit output.
  f      floating point

  Input:
  x[N]    input data, Q31 or floating point
  N       length of vector
  lsh     additional left shift for result
  c[M+1]  coefficients (M=4 coefficients for vec_poly4_xxx 
          and M=8 coefficients for vec_poly8_xxx), Q31 or floating point
  Output:			
  z[N]    result, Q31 or floating point

  Restriction:
  x,c,z should not overlap
  lsh   should be in range 0...31

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  x aligned on 8-byte boundary
  N   - multiple of 2
-------------------------------------------------------------------------*/
#define sz_f32  (int)sizeof(float32_t)
void vec_poly8f (float32_t * restrict z, const float32_t * restrict x, const float32_t * restrict c, int N )
{
  int n;

  xtfloatx2 vxf;
  xtfloatx2 vc0f, vc1f, vc2f, vc3f, vc4f, vc5f, vc6f, vc7f, vc8f;
  xtfloat c0f, c1f, c2f, c3f, c4f, c5f, c6f, c7f, c8f;
  xtfloat xf, zf;
  ae_int32x2 d0,d1,d2,d3,d4,d5,d6;
  const xtfloatx2*  restrict px   = (const xtfloatx2 *)x;
  const ae_int32 *  restrict pc   = (const ae_int32  *)c;
  const xtfloatx2*  restrict px_  = (const xtfloatx2 *)x;
  xtfloatx2* restrict pz = (xtfloatx2 *)z;
  ae_valign x_align,  z_align, x_align_;
  NASSERT(x);
  NASSERT(c);
  NASSERT(z);
  if (N <= 0) return;

  x_align = AE_LA64_PP(px);
  x_align_ = AE_LA64_PP(px_);
  z_align = AE_ZALIGN64();
  
  vc8f = (xtfloatx2)c[8];
  vc7f = (xtfloatx2)c[7];
  vc6f = (xtfloatx2)c[6];
  vc5f = (xtfloatx2)c[5];
  vc4f = (xtfloatx2)c[4];
  vc3f = (xtfloatx2)c[3];
  vc2f = (xtfloatx2)c[2];
  vc1f = (xtfloatx2)c[1];
  vc0f = (xtfloatx2)c[0];

  for (n = 0; n<N - 1; n += 2)
  {
    XT_LASX2IP(vxf, x_align, px);    
    XT_MADD_SX2(vc7f, vxf, vc8f);
    XT_MADD_SX2(vc6f, vxf, vc7f);
    XT_MADD_SX2(vc5f, vxf, vc6f);
    XT_MADD_SX2(vc4f, vxf, vc5f);
    XT_LASX2IP(vxf, x_align_, px_);
    XT_MADD_SX2(vc3f, vxf, vc4f);
    XT_MADD_SX2(vc2f, vxf, vc3f);
    XT_MADD_SX2(vc1f, vxf, vc2f);
    XT_MADD_SX2(vc0f, vxf, vc1f);

    XT_SASX2IP(vc0f, z_align, pz);

    AE_L32_IP(d0, pc, sz_f32);
    AE_L32_IP(d1, pc, sz_f32);
    AE_L32_IP(d2, pc, sz_f32);
    AE_L32_IP(d3, pc, sz_f32);
    AE_L32_IP(d4, pc, sz_f32);
    AE_L32_IP(d5, pc, sz_f32);
    AE_L32_IP(d6, pc, -6 * sz_f32);

    vc0f =XT_AE_MOVXTFLOATX2_FROMINT32X2 (d0);
    vc1f =XT_AE_MOVXTFLOATX2_FROMINT32X2 (d1);
    vc2f =XT_AE_MOVXTFLOATX2_FROMINT32X2 (d2);
    vc3f =XT_AE_MOVXTFLOATX2_FROMINT32X2 (d3);
    vc4f =XT_AE_MOVXTFLOATX2_FROMINT32X2 (d4);
    vc5f =XT_AE_MOVXTFLOATX2_FROMINT32X2 (d5);
    vc6f =XT_AE_MOVXTFLOATX2_FROMINT32X2 (d6);
    vc7f = (xtfloatx2)c[7];
  }
  AE_SA64POS_FP(z_align, pz);
  
  if (N & 1)
  {
      c8f = c[8];
      c7f = c[7];
      c6f = c[6];
      c5f = c[5];
      c4f = c[4];
      c3f = c[3];
      c2f = c[2];
      c1f = c[1];
      c0f = c[0];

      xf = XT_LSI((const xtfloat *)px, 0);
      XT_MADD_S(c7f, xf, c8f);
      XT_MADD_S(c6f, xf, c7f);
      XT_MADD_S(c5f, xf, c6f);
      XT_MADD_S(c4f, xf, c5f);
      XT_MADD_S(c3f, xf, c4f);
      XT_MADD_S(c2f, xf, c3f);
      XT_MADD_S(c1f, xf, c2f);
      XT_MADD_S(c0f, xf, c1f);
      zf = c0f;

      XT_SSI(zf, (xtfloat *)pz, 0);
  }
} /* vec_poly8f() */
#else
void vec_poly8f (float32_t * restrict z, const float32_t * restrict x, const float32_t * restrict c, int N )
{
  int n;
  const xtfloat * restrict pX=(const xtfloat *)x;
        xtfloat * restrict pZ=(      xtfloat *)z;
  xtfloat c0,c1,c2,c3,c4,c5,c6,c7,c8;
  c0=XT_LSI((const xtfloat*)c,sizeof(xtfloat)*0);
  c1=XT_LSI((const xtfloat*)c,sizeof(xtfloat)*1);
  c2=XT_LSI((const xtfloat*)c,sizeof(xtfloat)*2);
  c3=XT_LSI((const xtfloat*)c,sizeof(xtfloat)*3);
  c4=XT_LSI((const xtfloat*)c,sizeof(xtfloat)*4);
  c5=XT_LSI((const xtfloat*)c,sizeof(xtfloat)*5);
  c6=XT_LSI((const xtfloat*)c,sizeof(xtfloat)*6);
  c7=XT_LSI((const xtfloat*)c,sizeof(xtfloat)*7);
  c8=XT_LSI((const xtfloat*)c,sizeof(xtfloat)*8);
  /* NOTE: both orders of summation are possible */
  for (n=0; n<N; n++)
  {
    xtfloat z0,z1,x2,xn,t;
    XT_LSIP(xn,pX,sizeof(xtfloat));
    x2=XT_MUL_S(xn,xn);
    z0=c6; XT_MADD_S(z0,x2,c8);
    z1=c5; XT_MADD_S(z1,x2,c7);
    t=c4; XT_MADD_S(t,x2,z0); z0=t;
    t=c3; XT_MADD_S(t,x2,z1); z1=t;
    t=c2; XT_MADD_S(t,x2,z0); z0=t;
    t=c1; XT_MADD_S(t,x2,z1); z1=t;
    t=c0; XT_MADD_S(t,x2,z0);z0=t;
    t=z0; XT_MADD_S(t,xn,z1);z1=t;
    XT_SSIP(z1,pZ,sizeof(xtfloat));
  }
}
#endif
