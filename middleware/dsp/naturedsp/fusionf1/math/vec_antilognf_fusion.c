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

/* DSP Library API */
#include "NatureDSP_Signal.h"
/* Common helper macros. */
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, vec_antilognf, (float32_t * restrict y, const float32_t* restrict x, int N))
#else
/* Tables */
#include "expf_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"
#define sz_f32    (int)sizeof(float32_t)
#define _MOVT_S(x,y,cond) x=__my_MOVT_S(&x,&y,cond)
inline_ xtfloatx2 __my_MOVT_S(const xtfloatx2* px, const xtfloatx2* py, xtbool cond)
{
  xtfloatx2 X = *(const xtfloatx2*)px;
  xtfloatx2 Y = *(const xtfloatx2*)py;
  ae_int64 y = AE_MOVINT64_FROMINT32X2(XT_AE_MOVINT32X2_FROMXTFLOATX2((xtfloatx2)Y));
  ae_int64 x = AE_MOVINT64_FROMINT32X2(XT_AE_MOVINT32X2_FROMXTFLOATX2((xtfloatx2)X));
  AE_MOVT64(x, y, cond);
  return (XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT64(x)));
}
/*===========================================================================
  Vector matematics:
  vec_antilog          Antilogarithm         
===========================================================================*/

/*-------------------------------------------------------------------------
  Antilogarithm
  These routines calculate antilogarithm (base2, natural and base10). 32 
  and 24-bit fixed-point functions accept inputs in Q6.25 and form outputs 
  in Q16.15 format and return 0x7FFFFFFF in case of overflow and 0 in case 
  of underflow. 16-bit fixed-point functions accept inputs in Q3.12 and 
  form outputs in Q8.7 format and return 0x7FFF in case of overflow and 
  0 in case of underflow.

  Precision:
  16x16  16-bit inputs, 16-bit outputs. Accuracy: 2 LSB
  32x32  32-bit inputs, 32-bit outputs. Accuracy: 8*e-6*y+1LSB
  24x24  24-bit inputs, 24-bit outputs. Accuracy: 8*e-6*y+1LSB
  f      floating point: Accuracy: 2 ULP
  NOTE:
  1.  Although 32 and 24 bit functions provide the similar accuracy, 32-bit
      functions have better input/output resolution (dynamic range).
  2.  Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly.

  Input:
  x[N]  input data,Q6.25 (for 32 and 24-bit functions), Q3.12 (for 16-bit 
        functions) or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q16.15 (for 32 and 24-bit functions), Q8.7 (for 16-bit 
        functions) or floating point  

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  fixed point functions return result, Q16.15 (for 32 and 24-bit functions), 
  Q8.7 (for 16-bit functions) 

-------------------------------------------------------------------------*/

void vec_antilognf( float32_t * restrict y, const float32_t* restrict x, int N )
{
#if ANTILOG_ALG==1
  /*
  * int32_t t, y, zq,zp;
  * int e;
  * int64_t a;
  *
  * / scale input to 1/ln(2) and convert to Q31 /
  * x = frexpf(x, &e);
  *
  * t = (int32_t)STDLIB_MATH(ldexpf)(x, e + 24);
  * a = ((int64_t)t*invln2_Q30) >> 22; / Q24*Q30->Q32 /
  * t = ((uint32_t)a) >> 1;
  * e = (int32_t)(a >> 32);
  * / compute next part of 2^t using 32x32 multiplies /
  * zp = expftblpq[6];
  * zp = satQ31((((int64_t)t*zp) + (1LL << (31 - 1))) >> 31) + expftblpq[7];
  * zp = satQ31((((int64_t)t*zp) + (1LL << (31 - 1))) >> 31) + expftblpq[8];
  * / compute first part of 2^t using 24x24 multiples /
  * t = L_add_ll(t, 128) >> 8;   / t in Q23 here /
  * zq = expftblpq[0];
  * zq = mulf24ra(t, zq) + expftblpq[1];
  * zq = mulf24ra(t, zq) + expftblpq[2];
  * zq = mulf24ra(t, zq) + expftblpq[3];
  * zq = mulf24ra(t, zq) + expftblpq[4];
  * zq = mulf24ra(t, zq) + expftblpq[5];
  * zq = mulf24ra(t, zq);
  * / note : zq in q26 here /
  * / combine partial sums /
  * y = L_add_ll(zp, zq << 4);
  *
  * / convert back to the floating point /
  * x = STDLIB_MATH(ldexpf)((float32_t)y, e - 30);
  * return x;
  */
  const xtfloat *          X = (xtfloat*)x;
        xtfloat * restrict Y = (xtfloat*)y;
  const ae_int32  * restrict         TBL = (ae_int32 *)expftblpq;
  const xtfloat   * restrict         pT;

  xtfloat x0, x1, i0, i1;
  xtfloatx2 y0, y1;
  ae_int32x2 tb0, tb1, tb2, tb3, tb4, tb5, tb6, tb7, tb8;
  ae_int32x2 u0, e0, e1, n0, d0;
  ae_f32x2 acc1;
  ae_int32x2 t0, t1;
  xtbool b_nan0, b_nan1;
  ae_int64 wh, wl, wh1, wl1, wh0, wl0;
  int n;

  if (N <= 0) return;

  static const uint32_t TAB[2] = { 1549082005L, 0x7fc00000 };
  pT = (xtfloat  *)TAB;
  tb6 = AE_L32_I(TBL, +6 * sz_f32);
  tb7 = AE_L32_I(TBL, +7 * sz_f32);
  tb8 = AE_L32_X(TBL, +8 * sz_f32);

  XT_LSIP(x0, X, sz_f32);
  XT_LSIP(x1, X, sz_f32);

  b_nan0 = XT_UN_S(x0, x0);
  b_nan1 = XT_UN_S(x1, x1);

  /* scale input by 1/ln(2) and convert to Q31 */
  t0 = AE_MOVINT32X2_FROMINT32(XT_TRUNC_S(x0, 24));
  t1 = AE_MOVINT32X2_FROMINT32(XT_TRUNC_S(x1, 24));

  d0 = AE_MOVINT32X2_FROMINT32(AE_L32_I((ae_int32 *)pT, 0));
  wl = AE_MUL32_LL(t0, d0);
  wh = AE_MUL32_LL(t1, d0);
  wh0 = AE_SRAI64(wh, 22);
  wl0 = AE_SRAI64(wl, 22);
  e0 = AE_TRUNCI32X2F64S(wh0, wl0, 0);
  wh1 = AE_SLLI64(wh0, 32);
  wl1 = AE_SLLI64(wl0, 32);
  u0 = AE_TRUNCI32X2F64S(wh1, wl1, 0);
  u0 = AE_SRLI32(u0, 1);
  /* compute first part of 2^t using 32x32 multiplies */
  /* convert to Q31 */
  n0 = tb6; acc1 = AE_MOVF32X2_FROMINT32X2(tb7);
  AE_MULAFP32X2RAS(acc1, u0, n0); n0 = acc1; acc1 = tb8;
  AE_MULAFP32X2RAS(acc1, u0, n0);
  for (n = 0; n<(N >> 1); n++)
  {
    ae_f24x2 f0;
    ae_f32x2 acc0;
    ae_f24x2 tf0;
 
    tb0 = AE_L32_I(TBL, +0 * sz_f32);
    tb1 = AE_L32_I(TBL, +1 * sz_f32);
    tb2 = AE_L32_I(TBL, +2 * sz_f32);
    tb3 = AE_L32_I(TBL, +3 * sz_f32);
    tb4 = AE_L32_I(TBL, +4 * sz_f32);
    tb5 = AE_L32_I(TBL, +5 * sz_f32);
    tb6 = AE_L32_I(TBL, +6 * sz_f32);
    tb7 = AE_L32_I(TBL, +7 * sz_f32);
    tb8 = AE_L32_X(TBL, +8 * sz_f32); 

    /* compute next part of 2^t using 24x24 multiples */
    u0 = AE_SRAI32(u0,8);
    f0 = AE_MOVF24X2_FROMINT32X2(u0);
    tf0 = AE_MOVF24X2_FROMINT32X2(tb0);
    acc0 = AE_MOVF32X2_FROMINT32X2 (tb1);
    AE_MULAFP24X2RA(acc0, tf0, f0); tf0 = AE_MOVF24X2_FROMF32X2(acc0); acc0 = AE_MOVF32X2_FROMINT32X2(tb2);
    AE_MULAFP24X2RA(acc0, tf0, f0); tf0 = AE_MOVF24X2_FROMF32X2(acc0); acc0 = AE_MOVF32X2_FROMINT32X2(tb3);
    AE_MULAFP24X2RA(acc0, tf0, f0); tf0 = AE_MOVF24X2_FROMF32X2(acc0); acc0 = AE_MOVF32X2_FROMINT32X2(tb4);
    AE_MULAFP24X2RA(acc0, tf0, f0); tf0 = AE_MOVF24X2_FROMF32X2(acc0); acc0 = AE_MOVF32X2_FROMINT32X2(tb5);
    AE_MULAFP24X2RA(acc0, tf0, f0); tf0 = AE_MOVF24X2_FROMF32X2(acc0);
    acc0 = AE_MULFP24X2RA(tf0, f0);
    
    /* combine partial sums */
    n0 = AE_MOVINT32X2_FROMF32X2(acc0);
    n0 = AE_SLAI32(n0,4);
    acc1 = AE_ADD32(acc1, n0);
    n0 = acc1;
    x0 = XT_FLOAT_S(n0, 30);
    x1 = XT_FLOAT_S(AE_INTSWAP(n0), 30);
 
    i0 = XT_LSI(pT, 4);
    {
      xtfloatx2 p0, p1;
      p0 = (xtfloatx2)x0;
      p1 = (xtfloatx2)i0;
      _MOVT_S(p0, p1, b_nan0); x0 = XT_HIGH_S(p0);
      p0 = (xtfloatx2)x1;
      _MOVT_S(p0, p1, b_nan1); x1 = XT_LOW_S(p0);
    }
    /* convert back to the floating point */
    e0 = AE_ADD32(e0, 254);
    e1 = AE_SRAI32(e0, 1);
    e0 = AE_SUB32(e0, e1);
    e0 = AE_SLAI32(e0, 23);
    e1 = AE_SLAI32(e1, 23);
    y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(e0);
    y1 = XT_AE_MOVXTFLOATX2_FROMINT32X2(e1);

    i0 = XT_MUL_LLL_S(y1, y0);
    i1 = XT_MUL_LHH_S(y1, y0);
    x0 = XT_MUL_S(x0, i0);
    x1 = XT_MUL_S(x1, i1);
    XT_SSIP(x0, Y, sz_f32);
    XT_SSIP(x1, Y, sz_f32);

    /* Load next input values */
    XT_LSIP(x0, X, sz_f32);
    XT_LSIP(x1, X, sz_f32);

    b_nan0 = XT_UN_S(x0, x0);
    b_nan1 = XT_UN_S(x1, x1);

    /* scale input by 1/ln(2) and convert to Q31 */
    t0 = AE_MOVINT32X2_FROMINT32(XT_TRUNC_S(x0, 24));
    t1 = AE_MOVINT32X2_FROMINT32(XT_TRUNC_S(x1, 24));

    d0 = AE_MOVINT32X2_FROMINT32(AE_L32_I((ae_int32 *)pT, 0));
    wl = AE_MUL32_LL(t0, d0);
    wh = AE_MUL32_LL(t1, d0);
    wh0 = AE_SRAI64(wh, 22);
    wl0 = AE_SRAI64(wl, 22);
    e0 = AE_TRUNCI32X2F64S(wh0, wl0, 0);
    wh1 = AE_SLLI64(wh0, 32);
    wl1 = AE_SLLI64(wl0, 32);
    u0 = AE_TRUNCI32X2F64S(wh1, wl1, 0);
    u0 = AE_SRLI32(u0, 1);
    /* compute first part of 2^t using 32x32 multiplies */
    /* convert to Q31 */
    n0 = tb6; acc1 = AE_MOVF32X2_FROMINT32X2(tb7);
    AE_MULAFP32X2RAS(acc1, u0, n0); n0 = acc1; acc1 = tb8;
    AE_MULAFP32X2RAS(acc1, u0, n0);
  }
  if (N&1)
  {
    ae_f24x2 f0;
    ae_f32x2 acc0;
    ae_f24x2 tf0;

    tb0 = AE_L32_I(TBL, +0 * sz_f32);
    tb1 = AE_L32_I(TBL, +1 * sz_f32);
    tb2 = AE_L32_I(TBL, +2 * sz_f32);
    tb3 = AE_L32_I(TBL, +3 * sz_f32);
    tb4 = AE_L32_I(TBL, +4 * sz_f32);
    tb5 = AE_L32_I(TBL, +5 * sz_f32);
    tb6 = AE_L32_I(TBL, +6 * sz_f32);
    tb7 = AE_L32_I(TBL, +7 * sz_f32);
    tb8 = AE_L32_X(TBL, +8 * sz_f32); 
   
    /* compute next part of 2^t using 24x24 multiples */
    u0 = AE_SRAI32(u0,8);
    f0 = AE_MOVF24X2_FROMINT32X2(u0);
    tf0 = AE_MOVF24X2_FROMINT32X2(tb0);
    acc0 = AE_MOVF32X2_FROMINT32X2 (tb1);
    AE_MULAFP24X2RA(acc0, tf0, f0); tf0 = AE_MOVF24X2_FROMF32X2(acc0); acc0 = AE_MOVF32X2_FROMINT32X2(tb2);
    AE_MULAFP24X2RA(acc0, tf0, f0); tf0 = AE_MOVF24X2_FROMF32X2(acc0); acc0 = AE_MOVF32X2_FROMINT32X2(tb3);
    AE_MULAFP24X2RA(acc0, tf0, f0); tf0 = AE_MOVF24X2_FROMF32X2(acc0); acc0 = AE_MOVF32X2_FROMINT32X2(tb4);
    AE_MULAFP24X2RA(acc0, tf0, f0); tf0 = AE_MOVF24X2_FROMF32X2(acc0); acc0 = AE_MOVF32X2_FROMINT32X2(tb5);
    AE_MULAFP24X2RA(acc0, tf0, f0); tf0 = AE_MOVF24X2_FROMF32X2(acc0);
    acc0 = AE_MULFP24X2RA(tf0, f0);
    /* combine partial sums */
    n0 = AE_MOVINT32X2_FROMF32X2(acc0);
    n0 = AE_SLAI32(n0,4);
    acc1 = AE_ADD32(acc1, n0);
    n0 = acc1;
    x0 = XT_FLOAT_S(n0, 30);
    i0 = XT_LSI(pT, 4);
    XT_MOVT_S(x0, i0, b_nan0);
    /* convert back to the floating point */
    e0 = AE_ADD32(e0, 254);
    e1 = AE_SRAI32(e0, 1);
    e0 = AE_SUB32(e0, e1);
    e0 = AE_SLAI32(e0, 23);
    e1 = AE_SLAI32(e1, 23);
    y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(e0);
    y1 = XT_AE_MOVXTFLOATX2_FROMINT32X2(e1);

    i0 = XT_MUL_LLL_S(y1, y0);
    x0 = XT_MUL_S(x0, i0);
    XT_SSIP(x0, Y, sz_f32);
  }
#else
#error wrong ANTILOG_ALG
#endif
} /* vec_antilognf() */ 
#endif
