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

/* DSP Library API */
#include "NatureDSP_Signal_math.h"
/* Common helper macros. */
#include "common_fpu.h"
/* Tables */
#include "alog2f_tbl.h"
#include "expf_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(void,vec_antilog2f,( float32_t * restrict y, const float32_t* restrict x, int N ))
#elif HAVE_VFPU
#define sz_i32     (int)sizeof(int32_t)

/*===========================================================================
  Vector matematics:
  vec_antilog          Antilogarithm         
===========================================================================*/

/*-------------------------------------------------------------------------
  Antilogarithm
  These routines calculate antilogarithm (base2, natural and base10). 
  Fixed-point functions accept inputs in Q25 and form outputs in Q16.15 
  format and return 0x7FFFFFFF in case of overflow and 0 in case of 
  underflow.

  Precision:
  32x32  32-bit inputs, 32-bit outputs. Accuracy: 8*e-6*y+1LSB
  f      floating point: Accuracy: 2 ULP
  NOTE:
  1.  Although 32 and 24 bit functions provide the similar accuracy, 32-bit
      functions have better input/output resolution (dynamic range).
  2.  Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly.

  Input:
  x[N]  input data,Q25 or floating point 
  N     length of vectors
  Output:
  y[N]  output data,Q16.15 or floating point  

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  fixed point functions return result in Q16.15

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  x,y - aligned on 8-byte boundary
  N   - multiple of 2
-------------------------------------------------------------------------*/

void vec_antilog2f( float32_t * restrict y, const float32_t* restrict x, int N )
{
  /*
    int32_t t,y;
    int e;

    if(isnan(x)) return x;
    if (x>alog2fminmax[1].f) x= alog2fminmax[1].f;
    if (x<alog2fminmax[0].f) x= alog2fminmax[0].f;

    e=STDLIB_MATH(floorf)(x);    -149...128 
    x = x - (float32_t)e;
    t = (int32_t)STDLIB_MATH(ldexpf)(x, 31);
     compute 2^t in Q30 where t is in Q31 
    y = expftbl_Q30[0];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[1];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[2];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[3];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[4];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[5];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[6];
     convert back to the floating point 
    x = STDLIB_MATH(ldexpf)((float32_t)y, e - 30);
    return x;
  */

  const xtfloatx2 *          X   = (xtfloatx2*)x;
        xtfloatx2 * restrict Y   = (xtfloatx2*)y;
  const ae_int32  *          TBL = (ae_int32 *)expftbl_Q30;

  ae_valign X_va, Y_va;
  
  xtfloatx2 x0, y0, y1;
  ae_int32x2 tb0, tb1, tb2, tb3, tb4, tb5, tb6;
  ae_int32x2 u0, u1, e0, e1, n0;
  ae_f32x2 f0;
  xtbool2 b_nan;

  int n;

  if ( N<=0 ) return;

  X_va = AE_LA64_PP(X);
  Y_va = AE_ZALIGN64();

  for ( n=0; n<(N>>1); n++ )
  {
    XT_LASX2IP(x0, X_va, X);

    x0 = XT_MAX_SX2(alog2fminmax[0].f, x0);
    x0 = XT_MIN_SX2(alog2fminmax[1].f, x0);

    y0 = XT_FIFLOOR_SX2(x0);
    e0 = XT_TRUNC_SX2(y0, 0);
    x0 = XT_SUB_SX2(x0, y0);

    u0 = XT_TRUNC_SX2(x0, 31);

    tb0 = AE_L32_I(TBL, 0*4);
    tb1 = AE_L32_I(TBL, 1*4);
    tb2 = AE_L32_I(TBL, 2*4);
    tb3 = AE_L32_I(TBL, 3*4);
    tb4 = AE_L32_I(TBL, 4*4);
    tb5 = AE_L32_I(TBL, 5*4);
    tb6 = AE_L32_I(TBL, 6*4);

    /*                                                              
     * Use a combination of Estrin's method and Horner's scheme to evaluate
     * the fixed-point polynomial.                                     
     */

    f0 = tb2; AE_MULAFP32X2RAS(f0, tb1, u0); tb1 = f0;
    f0 = tb4; AE_MULAFP32X2RAS(f0, tb3, u0); tb2 = f0;
    f0 = tb6; AE_MULAFP32X2RAS(f0, tb5, u0); tb3 = f0;

    u1 = AE_MULFP32X2RAS(u0, u0);

    f0 = tb0;                               n0 = f0; 
    f0 = tb1; AE_MULAFP32X2RAS(f0, u1, n0); n0 = f0;
    f0 = tb2; AE_MULAFP32X2RAS(f0, u1, n0); n0 = f0;
    f0 = tb3; AE_MULAFP32X2RAS(f0, u1, n0); n0 = f0;

    x0 = XT_FLOAT_SX2(n0,30);

    b_nan = XT_UN_SX2(y0, y0);

    e0 = AE_ADD32( e0, 254 );
    e1 = AE_SRAI32(e0, 1);
    e0 = AE_SUB32(e0, e1);

    u0 = AE_SLAI32(e0, 23);
    u1 = AE_SLAI32(e1, 23);
    y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(u0);
    y1 = XT_AE_MOVXTFLOATX2_FROMINT32X2(u1);

    XT_MOVT_SX2(y0, qNaNf.f, b_nan);

    y0 = XT_MUL_SX2(y0, y1);
    y0 = XT_MUL_SX2(x0, y0);

    XT_SASX2IP(y0, Y_va, Y);
  }

  XT_SASX2POSFP(Y_va, Y);

  if ( N&1 )
  {
    x0 = XT_LSI( (xtfloat*)X, 0 );

    x0 = XT_MAX_SX2(alog2fminmax[0].f, x0);
    x0 = XT_MIN_SX2(alog2fminmax[1].f, x0);

    y0 = XT_FIFLOOR_SX2(x0);
    e0 = XT_TRUNC_SX2(y0, 0);
    x0 = XT_SUB_SX2(x0, y0);

    u0 = XT_TRUNC_SX2(x0, 31);

    tb0 = AE_L32_I(TBL, 0*4);
    tb1 = AE_L32_I(TBL, 1*4);
    tb2 = AE_L32_I(TBL, 2*4);
    tb3 = AE_L32_I(TBL, 3*4);
    tb4 = AE_L32_I(TBL, 4*4);
    tb5 = AE_L32_I(TBL, 5*4);
    tb6 = AE_L32_I(TBL, 6*4);

    f0 = tb2; AE_MULAFP32X2RAS(f0, tb1, u0); tb1 = f0;
    f0 = tb4; AE_MULAFP32X2RAS(f0, tb3, u0); tb2 = f0;
    f0 = tb6; AE_MULAFP32X2RAS(f0, tb5, u0); tb3 = f0;

    u1 = AE_MULFP32X2RAS(u0, u0);

    f0 = tb0;                               n0 = f0; 
    f0 = tb1; AE_MULAFP32X2RAS(f0, u1, n0); n0 = f0;
    f0 = tb2; AE_MULAFP32X2RAS(f0, u1, n0); n0 = f0;
    f0 = tb3; AE_MULAFP32X2RAS(f0, u1, n0); n0 = f0;

    x0 = XT_FLOAT_SX2(n0,30);

    b_nan = XT_UN_SX2(y0, y0);

    e0 = AE_ADD32( e0, 254 );
    e1 = AE_SRAI32(e0, 1);
    e0 = AE_SUB32(e0, e1);

    u0 = AE_SLAI32(e0, 23);
    u1 = AE_SLAI32(e1, 23);
    y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(u0);
    y1 = XT_AE_MOVXTFLOATX2_FROMINT32X2(u1);

    XT_MOVT_SX2(y0, qNaNf.f, b_nan);

    y0 = XT_MUL_SX2(y0, y1);
    y0 = XT_MUL_SX2(x0, y0);

    XT_SSI( y0, (xtfloat*)Y, 0 );
  }

} /* vec_antilog2f() */

#elif HAVE_FPU
#define sz_i32 (int)sizeof(int32_t)
#define sz_f32 (int)sizeof(float32_t)

/*===========================================================================
  Vector matematics:
  vec_antilog          Antilogarithm         
===========================================================================*/
/*-------------------------------------------------------------------------
  Antilogarithm
  These routines calculate antilogarithm (base2, natural and base10). 
  Fixed-point functions accept inputs in Q25 and form outputs in Q16.15 
  format and return 0x7FFFFFFF in case of overflow and 0 in case of 
  underflow.

  Precision:
  32x32  32-bit inputs, 32-bit outputs. Accuracy: 4*e-5*y+1LSB
  f      floating point: Accuracy: 2 ULP
  NOTE:
  1.  Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly

  Input:
  x[N]  input data,Q25 or floating point 
  N     length of vectors
  Output:
  y[N]  output data,Q16.15 or floating point  

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  fixed point functions return result in Q16.15

-------------------------------------------------------------------------*/

void vec_antilog2f( float32_t * restrict y, const float32_t* restrict x, int N )
{
  /*
    int32_t t,y;
    int e;

    if(isnan(x)) return x;
    if (x>alog2fminmax[1].f) x= alog2fminmax[1].f;
    if (x<alog2fminmax[0].f) x= alog2fminmax[0].f;

    e=STDLIB_MATH(floorf)(x);    -149...128 
    x = x - (float32_t)e;
    t = (int32_t)STDLIB_MATH(ldexpf)(x, 31);
     compute 2^t in Q30 where t is in Q31 
    y = expftbl_Q30[0];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[1];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[2];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[3];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[4];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[5];
    y = satQ31((((int64_t)t*y) + (1LL << (31 - 1))) >> 31) + expftbl_Q30[6];
     convert back to the floating point 
    x = STDLIB_MATH(ldexpf)((float32_t)y, e - 30);
    return x;
  */

  const xtfloat  *          X0  = (xtfloat*)x;
  const xtfloat  *          X1  = (xtfloat*)x;
  const ae_int32 *          TBL = (ae_int32*)expftbl_Q30;
        xtfloat  * restrict Y   = (xtfloat*)y;

  xtfloat    x0, x1, x0_, x1_, y0, y1, z0, z1;
  ae_int32x2 tb0, tb1, tb2, tb3, tb4, tb5, tb6;
  int32_t    e0, e1, n0, n1, u0, u1;
  xtbool     b_nan0, b_nan1;

  int n;

  if ( N<=0 ) return;

  for ( n=0; n<(N>>1); n++ )
  {
    ae_f32x2   v01, s01, f01, m01;
    ae_int32x2 e01, g01, u01, t01;

	XT_LSIP(x0, X0, sz_f32);
	XT_LSIP(x1, X0, sz_f32);

    /* Check borders. */
	x0 = XT_MAX_S(alog2fminmax[0].f, x0);
	x0 = XT_MIN_S(alog2fminmax[1].f, x0);
	x1 = XT_MAX_S(alog2fminmax[0].f, x1);
	x1 = XT_MIN_S(alog2fminmax[1].f, x1);

    y0 = XT_FLOOR_S(x0, 0);
    y1 = XT_FLOOR_S(x1, 0);
    e0 = XT_TRUNC_S(y0, 0);
    e1 = XT_TRUNC_S(y1, 0);
    x0 = XT_SUB_S(x0, y0);
    x1 = XT_SUB_S(x1, y1);
    u0 = XT_TRUNC_S( XT_MUL_S( x0, XT_FLOAT_S( 1<<16, 0 ) ), 15 );
    u1 = XT_TRUNC_S( XT_MUL_S( x1, XT_FLOAT_S( 1<<16, 0 ) ), 15 );

    tb0 = AE_L32_I(TBL, 0*sz_f32);
    tb1 = AE_L32_I(TBL, 1*sz_f32);
    tb2 = AE_L32_I(TBL, 2*sz_f32);
    tb3 = AE_L32_I(TBL, 3*sz_f32);
    tb4 = AE_L32_I(TBL, 4*sz_f32);
    tb5 = AE_L32_I(TBL, 5*sz_f32);
    tb6 = AE_L32_I(TBL, 6*sz_f32);

    /*                                                              
     * Use a combination of Estrin's method and Horner's scheme to evaluate
     * the fixed-point polynomial.                                     
     */

    v01 = AE_MOVDA32X2(u1, u0);

	f01 = tb2; AE_MULAFP32X2RAS(f01, tb1, v01); tb1 = f01;
	f01 = tb4; AE_MULAFP32X2RAS(f01, tb3, v01); tb2 = f01;
	f01 = tb6; AE_MULAFP32X2RAS(f01, tb5, v01); tb3 = f01;

	s01 = AE_MULFP32X2RAS(v01, v01);

	f01 = tb0;                                  m01 = f01;
	f01 = tb1; AE_MULAFP32X2RAS(f01, s01, m01); m01 = f01;
	f01 = tb2; AE_MULAFP32X2RAS(f01, s01, m01); m01 = f01;
	f01 = tb3; AE_MULAFP32X2RAS(f01, s01, m01); m01 = f01;

    n0 = AE_MOVAD32_L(m01);
    n1 = AE_MOVAD32_H(m01);

    x0 = XT_MUL_S( XT_FLOAT_S( n0, 15 ), XT_FLOAT_S( 1, 15 ) );
    x1 = XT_MUL_S( XT_FLOAT_S( n1, 15 ), XT_FLOAT_S( 1, 15 ) );

    e01 = AE_MOVDA32X2(e1, e0);
    e01 = AE_ADD32(e01, 254);
    g01 = AE_SRAI32(e01, 1);
    e01 = AE_SUB32(e01, g01);
    u01 = AE_SLLI32(e01, 23);
    t01 = AE_SLLI32(g01, 23);
    y0 = XT_WFR(AE_MOVAD32_L(u01));
    y1 = XT_WFR(AE_MOVAD32_H(u01));
    z0 = XT_WFR(AE_MOVAD32_L(t01));
    z1 = XT_WFR(AE_MOVAD32_H(t01));

	XT_LSIP(x0_, X1, sz_f32);
	XT_LSIP(x1_, X1, sz_f32);
	b_nan0 = XT_UN_S(x0_, x0_);
	b_nan1 = XT_UN_S(x1_, x1_);
    XT_MOVT_S(y0, qNaNf.f, b_nan0);
    XT_MOVT_S(y1, qNaNf.f, b_nan1);

    y0 = XT_MUL_S(y0, z0);
    y1 = XT_MUL_S(y1, z1);
    y0 = XT_MUL_S(x0, y0);
    y1 = XT_MUL_S(x1, y1);

	XT_SSIP(y0, Y, sz_f32);
	XT_SSIP(y1, Y, sz_f32);
  }
  if (N & 1)
  {
    ae_f32x2 f0;
    ae_f32   v0, s0, m0;
    ae_int32 g0, t0;

    XT_LSIP(x0, X0, sz_f32);

	b_nan0 = XT_UN_S(x0, x0);

    /* Check borders. */
	x0 = XT_MAX_S(alog2fminmax[0].f, x0);
	x0 = XT_MIN_S(alog2fminmax[1].f, x0);

    y0 = XT_FLOOR_S(x0, 0);
    e0 = XT_TRUNC_S(y0, 0);
    x0 = XT_SUB_S(x0, y0);
    u0 = XT_TRUNC_S( XT_MUL_S( x0, XT_FLOAT_S( 1<<16, 0 ) ), 15 );

    tb0 = AE_L32_I(TBL, 0*sz_f32);
    tb1 = AE_L32_I(TBL, 1*sz_f32);
    tb2 = AE_L32_I(TBL, 2*sz_f32);
    tb3 = AE_L32_I(TBL, 3*sz_f32);
    tb4 = AE_L32_I(TBL, 4*sz_f32);
    tb5 = AE_L32_I(TBL, 5*sz_f32);
    tb6 = AE_L32_I(TBL, 6*sz_f32);

    /*                                                              
     * Use a combination of Estrin's method and Horner's scheme to evaluate
     * the fixed-point polynomial.                                     
     */

    v0 = u0;

    f0 = tb2; AE_MULAFP32X2RAS(f0, tb1, v0); tb1 = f0;
    f0 = tb4; AE_MULAFP32X2RAS(f0, tb3, v0); tb2 = f0;
    f0 = tb6; AE_MULAFP32X2RAS(f0, tb5, v0); tb3 = f0;

    s0 = AE_MULFP32X2RAS(v0, v0);

    f0 = tb0;                               m0 = f0;
    f0 = tb1; AE_MULAFP32X2RAS(f0, s0, m0); m0 = f0;
    f0 = tb2; AE_MULAFP32X2RAS(f0, s0, m0); m0 = f0;
    f0 = tb3; AE_MULAFP32X2RAS(f0, s0, m0); m0 = f0;

    n0 = ae_f32_rtor_int32(m0);

    x0 = XT_MUL_S( XT_FLOAT_S( n0, 15 ), XT_FLOAT_S( 1, 15 ) );

    e0 = XT_ADD(e0, 254);
    g0 = XT_SRAI(e0, 1);
    e0 = XT_SUB(e0, g0);
    u0 = XT_SLLI(e0, 23);
    t0 = XT_SLLI(g0, 23);
    y0 = XT_WFR(u0);
    z0 = XT_WFR(t0);

	XT_LSIP(x0_, X1, sz_f32);
	b_nan0 = XT_UN_S(x0_, x0_);
    XT_MOVT_S(y0, qNaNf.f, b_nan0);

    y0 = XT_MUL_S(y0, z0);
    y0 = XT_MUL_S(x0, y0);

	XT_SSIP(y0, Y, sz_f32);
  }
} /* vec_antilog2f() */
#endif
