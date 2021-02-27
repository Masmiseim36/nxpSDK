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
#include "NatureDSP_Signal.h"
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, vec_complex2invmag, (float32_t  * restrict y, const complex_float  * restrict x, int N))
#else
#include "inff_tbl.h"

/*===========================================================================
  Vector matematics:
  vec_complex2invmag     complex magnitude (reciprocal)
===========================================================================*/
/*-------------------------------------------------------------------------
  Complex magnitude
  routines compute complex magnitude or its reciprocal

  Precision: 
  f     single precision floating point

  Output:
  y[N]  output data
  Input:
  x[N]  input complex data
  N     length of vector

  Restriction:
  none
-------------------------------------------------------------------------*/
#define sz_f32    (int)sizeof(float32_t)
void       vec_complex2invmag (float32_t  * restrict y, const complex_float  * restrict x, int N)
{
  /*
  * float32_t x_re,x_im;
  * float32_t mnt_re, mnt_im, mnt_abs;
  * int exp_re, exp_im, exp_abs;
  * const int minexp = FLT_MIN_EXP - FLT_MANT_DIG;
  * 
  * x_re = fabsf( crealf(x) );
  * x_im = fabsf( cimagf(x) );
  * 
  * exp_re = ( x_re != 0 ? (int)STDLIB_MATH(ceilf)( log2f(x_re) ) : minexp );
  * exp_im = ( x_im != 0 ? (int)STDLIB_MATH(ceilf)( log2f(x_im) ) : minexp );
  * 
  * exp_abs = ( exp_re > exp_im ? exp_re : exp_im );
  * 
  * mnt_re = STDLIB_MATH(ldexpf)( x_re, -exp_abs );
  * mnt_im = STDLIB_MATH(ldexpf)( x_im, -exp_abs );
  * 
  * mnt_abs = 1.f/sqrtf( mnt_re*mnt_re + mnt_im*mnt_im );
  * 
  * return STDLIB_MATH(ldexpf)( mnt_abs, -exp_abs );
  */

  const xtfloatx2 * restrict X = (const xtfloatx2 *)x;
  const xtfloatx2 *          S_rd;
  xtfloatx2 * restrict Y = (xtfloatx2 *)y;
  xtfloatx2 * restrict S_wr;
  int n, _N;
  xtfloat xl, xh;
  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx, blkNum, blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ / (3 * sizeof(float32_t));
  /* Allocate a fixed-size scratch area on the stack. */
  float32_t ALIGN(8) scr[3*blkSize];

  xtfloat xf, xf0, xf1;
  ae_int32x2 t0, t1, nsa;
  xtfloatx2 x0, x1, y0, xre, xim;
  ae_int32x2 e0, e1;
  ae_int32x2 u0, u1;
  ae_int32x2 nsa0;
  xtbool2 b0;
  xtbool bzero, binf, bzero1, binf1;
  if (N <= 0) return;
  NASSERT_ALIGN8(scr);
  _N = N&(~1);
  blkNum = (_N + blkSize - 1) / blkSize;
  for (blkIx = 0; blkIx<blkNum; blkIx++)
  {
    blkLen = XT_MIN(_N - blkIx*blkSize, blkSize);
    X = (xtfloatx2*)((uintptr_t)x + blkIx*blkSize * 2 * sizeof(float32_t));
    S_wr = (xtfloatx2*)scr;
    /*
    * x_re = fabsf( crealf(x) );
    * x_im = fabsf( cimagf(x) );
    * 
    * exp_re = ( x_re != 0 ? (int)STDLIB_MATH(ceilf)( log2f(x_re) ) : minexp );
    * exp_im = ( x_im != 0 ? (int)STDLIB_MATH(ceilf)( log2f(x_im) ) : minexp );
    * 
    * exp_abs = ( exp_re > exp_im ? exp_re : exp_im );
    */
    __Pragma("loop_count min=1");
    for (n = 0; n<((blkLen + 1) / 2); n++)
    {
      xtfloat v0, v1, v2, v3;
      XT_LSX2IP(x0, X, sizeof(complex_float));
      XT_LSX2IP(x1, X, sizeof(complex_float));
      x0 = XT_ABS_SX2(x0);
      x1 = XT_ABS_SX2(x1);

      xre = XT_SEL32_HH_SX2(x0, x1);
      xim = XT_SEL32_LL_SX2(x0, x1); 

      t0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(xre);
      t1 = XT_AE_MOVINT32X2_FROMXTFLOATX2(xim);

      u0 = AE_SRLI32(t0, 23);
      u1 = AE_SRLI32(t1, 23);
      u0 = AE_AND32(u0, 0xFF);
      u1 = AE_AND32(u1, 0xFF);
      e0 = AE_SUB32(u0, 127);
      e1 = AE_SUB32(u1, 127);
      nsa = AE_MAX32(e0, e1);
      b0 = AE_LT32(127, nsa);
      AE_MOVT32X2(nsa, 127, b0);

      e0 = AE_SUB32(127, nsa);
      b0 = AE_EQ32(e0, 0);
      nsa0 = AE_SLLI32(e0, 23);
      AE_MOVT32X2(nsa0, 0x00400000, b0);
      y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0);

      v0 = XT_MUL_LHH_S(xre, y0);
      v1 = XT_MUL_LLL_S(xre, y0);
      v2 = XT_MUL_LHH_S(xim, y0);
      v3 = XT_MUL_LLL_S(xim, y0);

      xf0 = XT_MUL_S(v0, v0);
      xf1 = XT_MUL_S(v1, v1);
      XT_MADD_S(xf0, v2, v2);
      XT_MADD_S(xf1, v3, v3);

      y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0);
      XT_SSX2IP(y0, S_wr, 2 * sz_f32);
      XT_SSIP(xf0, castxcc(xtfloat, S_wr), sz_f32);
      XT_SSIP(xf1, castxcc(xtfloat, S_wr), sz_f32);
    }
    __Pragma("no_reorder");
    S_rd = (xtfloatx2*)(scr + 2);
    S_wr = (xtfloatx2*)(scr + 2);
    /* mnt_abs = sqrtf( mnt_re*mnt_re + mnt_im*mnt_im ); */
    __Pragma("loop_count min=1");
    for (n = 0; n<(blkLen + 1) / 2; n++)
    {
      xtfloat yf0, yf1;

      XT_LSIP(xf0, castxcc(const xtfloat, S_rd), sizeof(float32_t));
      XT_LSIP(xf1, castxcc(const xtfloat, S_rd), 3 * sizeof(float32_t));

      yf0 = XT_RSQRT_S(xf0);
      yf1 = XT_RSQRT_S(xf1);
      
      bzero = XT_OEQ_S(xf0, XT_CONST_S(0));
      binf = XT_OEQ_S(xf0, plusInff.f);
      XT_MOVT_S(yf0, plusInff.f, bzero);
      XT_MOVT_S(yf0, XT_CONST_S(0), binf);

      bzero1 = XT_OEQ_S(xf1, XT_CONST_S(0));
      binf1 = XT_OEQ_S(xf1, plusInff.f);
      XT_MOVT_S(yf1, plusInff.f, bzero1);
      XT_MOVT_S(yf1, XT_CONST_S(0), binf1);

      XT_SSIP(yf0, castxcc(xtfloat, S_wr), sizeof(float32_t));
      XT_SSIP(yf1, castxcc(xtfloat, S_wr), 3 * sizeof(float32_t));
    }
    __Pragma("no_reorder");
    S_rd = (xtfloatx2*)(scr);
    __Pragma("loop_count min=1");
    /* STDLIB_MATH(ldexpf)( mnt_abs, exp_abs ); */
    for (n = 0; n<(blkLen + 1) / 2; n++)
    {
      XT_LSX2IP(y0, S_rd, 2 * sz_f32);
      XT_LSIP(xf0, castxcc(const xtfloat, S_rd), sz_f32);
      XT_LSIP(xf1, castxcc(const xtfloat, S_rd), sz_f32);

      xf0 = XT_MUL_S(xf0, XT_HIGH_S(y0));
      xf1 = XT_MUL_S(xf1, XT_LOW_S(y0));
      XT_SSIP(xf0, castxcc(xtfloat, Y), sz_f32);
      XT_SSIP(xf1, castxcc(xtfloat, Y), sz_f32);
    }
  }
  if (N & 1)
  {
    xtfloat yl;
    XT_LSX2IP(x0, X, sizeof(complex_float));
    x0 = XT_ABS_SX2(x0);

    t0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(x0);

    u0 = AE_SRLI32(t0, 23);
    u0 = AE_AND32(u0, 0xFF);
    e0 = AE_SUB32(u0, 127);
    e1 = AE_SEL32_LH(e0, e0);
    nsa = AE_MAX32(e0, e1);
    b0 = AE_LT32(127, nsa);
    AE_MOVT32X2(nsa, 127, b0);

    e0 = AE_SUB32(127, nsa);
    b0 = AE_EQ32(e0, 0);
    nsa0 = AE_SLLI32(e0, 23);
    AE_MOVT32X2(nsa0, 0x00400000, b0);
    y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0);
    xl = XT_MUL_LLL_S(x0, y0);
    xh = XT_MUL_LHH_S(x0, y0);
    xl = XT_MUL_S(xl, xl);
    XT_MADD_S(xl, xh, xh);
    yl = XT_RSQRT_S(xl);
    bzero = XT_OEQ_S(xl, XT_CONST_S(0));
    binf = XT_OEQ_S(xl, plusInff.f);
    XT_MOVT_S(yl, plusInff.f, bzero);
    XT_MOVT_S(yl, XT_CONST_S(0), binf);
    xl = XT_LOW_S(XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0));
    xf = XT_MUL_S(yl, xl);
    XT_SSIP(xf, castxcc(xtfloat, Y), sizeof(float32_t));
  }
} /* vec_complex2invmag() */
#endif
