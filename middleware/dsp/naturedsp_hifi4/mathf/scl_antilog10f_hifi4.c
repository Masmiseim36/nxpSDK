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

#include <errno.h>

/* DSP Library API */
#include "NatureDSP_Signal_math.h"
/* Common helper macros. */
#include "common_fpu.h"
/* Inline functions for floating-point exceptions and environment control. */
#include "__fenv.h"
/* Tables */
#include "expf_tbl.h"
#include "alog10f_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(float32_t,scl_antilog10f,( float32_t x ))
#elif HAVE_VFPU
/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_ANTILOG10F_ERRH     1

/*===========================================================================
  Scalar matematics:
  scl_antilog          Antilogarithm         
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

float32_t scl_antilog10f (float32_t x)
{
  /*
   * Reference C code for SCL_ANTILOG10F_ERRH == 0:
   *
   *   int32_t t,y;
   *   int e;
   *   int64_t a;
   *  
   *   // scale input to 1/log10(2) and convert to Q31
   *   x=frexpf(x,&e);
   *  
   *   t=(int32_t)STDLIB_MATH(ldexpf)(x,e+24);
   *   a=((int64_t)t*invlog10_2_Q29)>>21; // Q24*Q29->Q32
   *   t=((uint32_t)a)>>1;
   *   e=(int32_t)(a>>32);
   *   // compute 2^t in Q30 where t is in Q31
   *   y=                                           expftbl_Q30[0];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[1];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[2];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[3];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[4];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[5];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[6];
   *   // convert back to the floating point
   *   x=STDLIB_MATH(ldexpf)((float32_t)y,e-30);
   *   return x;
   */

  /* Input value; output value; polynomial approximation */
  xtfloatx2 x0, x1, y0, q;
  /* Fixed-point input value; polynomial value. */
  ae_int32x2 p, g;
  /* Polynomial coeffs. */
  ae_int32x2 cf0, cf1, cf2, cf3, cf4, cf5, cf6;
  /* Fractional part and its 2nd power. */
  ae_int32x2 f0, f1;
  /* Exponential parts. */
  ae_int32x2 e0, e1;
  /* Scale factors. */
  xtfloatx2 s0, s1;
  /* Auxiliary var. */
  ae_f32x2 t;
  /* Wide auxiliary vars. */
  ae_int64 w0, w1;

#if SCL_ANTILOG10F_ERRH != 0
  /* Floating-point Status and Control Register values. */
  ae_int64 fstate;
#endif

  x0 = (xtfloatx2)x;

  if ( xtbool2_extract_0( XT_UN_SX2( x0, x0 ) ) )
  {
    __Pragma( "frequency_hint never" );
#if SCL_ANTILOG10F_ERRH != 0
    errno = EDOM;
#endif
    return (qNaNf.f);
  }

  x1 = XT_MAX_SX2( alog10fminmax[0].f, x0 );
  x1 = XT_MIN_SX2( alog10fminmax[1].f, x1 );

#if SCL_ANTILOG10F_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_AE_MOVVFCRFSR();
#endif

  p = XT_TRUNC_SX2( x1, 25 );

  /* Scale input by 1/log10(2) and convert to Q31 */
  w0 = AE_MUL32_HH( p, invlog10_2_Q29 );
  e0 = AE_TRUNCA32X2F64S( w0, w0, -22 );
  w1 = AE_SLLI64( w0, 32-22 );
  f0 = AE_TRUNCI32X2F64S( w1, w1, 0 );
  f0 = AE_SRLI32( f0, 1 );

  /*
   * Compute 2^fract(x/log10(2)) in Q30, where f=x/log10(2) is in Q31. Use a
   * combination of Estrin's rule and Horner's method to evaluate the polynomial.
   */

  cf0 = expftbl_Q30[0]; cf1 = expftbl_Q30[1];
  cf2 = expftbl_Q30[2]; cf3 = expftbl_Q30[3];
  cf4 = expftbl_Q30[4]; cf5 = expftbl_Q30[5];
  cf6 = expftbl_Q30[6];

  f1 = AE_MULFP32X2RAS( f0, f0 );

  t = cf2; AE_MULAFP32X2RAS( t, f0, cf1 ); cf1 = t;
  t = cf4; AE_MULAFP32X2RAS( t, f0, cf3 ); cf2 = t;
  t = cf6; AE_MULAFP32X2RAS( t, f0, cf5 ); cf3 = t;

  t = cf0;                               g = t;
  t = cf1; AE_MULAFP32X2RAS( t, f1, g ); g = t;
  t = cf2; AE_MULAFP32X2RAS( t, f1, g ); g = t;
  t = cf3; AE_MULAFP32X2RAS( t, f1, g ); g = t;

  q = XT_FLOAT_SX2( g, 30 );

  /*
   * Calculate 2^int(x/log10(2)) * 2^fract(x/log10(2))
   */

  e0 = AE_ADD32( e0, 254 );
  e1 = AE_SRAI32( e0, 1 );
  e0 = AE_SUB32( e0, e1 );
  e0 = AE_SLAI32( e0, 23 );
  e1 = AE_SLAI32( e1, 23 );
  s0 = XT_AE_MOVXTFLOATX2_FROMINT32X2( e0 );
  s1 = XT_AE_MOVXTFLOATX2_FROMINT32X2( e1 );

  s0 = XT_MUL_SX2( s0, s1 );
  y0 = XT_MUL_SX2( s0, q );

#if SCL_ANTILOG10F_ERRH != 0
  {
    /* Is input less than infinity; is output equal to infinity. */
    xtbool2 b_xfin, b_yinf;

    /* Suppress spurious exception flags and restore original status flags. */
    XT_AE_MOVFCRFSRV( fstate );

    b_xfin = XT_OLT_SX2( x0, plusInff.f );
    b_yinf = XT_OEQ_SX2( y0, plusInff.f );

    /* Check for an overflow. */
    if ( XT_ANDB( xtbool2_extract_0(b_xfin), xtbool2_extract_0(b_yinf) ) )
    {
      __Pragma( "frequency_hint never" );
      __feraiseexcept( FE_OVERFLOW );
      errno = ERANGE;
    }
  }
#endif

  return ( XT_HIGH_S(y0) );

} /* scl_antilog10f() */

#elif HAVE_FPU
/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_ANTILOG10F_ERRH     1

/*===========================================================================
  Scalar matematics:
  scl_antilog          Antilogarithm         
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

float32_t scl_antilog10f( float32_t x )
{
  /*
   * Reference C code for SCL_ANTILOG10F_ERRH == 0:
   *
   *   int32_t t,y;
   *   int e;
   *   int64_t a;
   *  
   *   // scale input to 1/log10(2) and convert to Q31
   *   x=frexpf(x,&e);
   *  
   *   t=(int32_t)STDLIB_MATH(ldexpf)(x,e+24);
   *   a=((int64_t)t*invlog10_2_Q29)>>21; // Q24*Q29->Q32
   *   t=((uint32_t)a)>>1;
   *   e=(int32_t)(a>>32);
   *   // compute 2^t in Q30 where t is in Q31
   *   y=                                           expftbl_Q30[0];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[1];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[2];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[3];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[4];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[5];
   *   y=satQ31((((int64_t)t*y)+(1LL<<(31-1)))>>31)+expftbl_Q30[6];
   *   // convert back to the floating point
   *   x=STDLIB_MATH(ldexpf)((float32_t)y,e-30);
   *   return x;
   */

  /* Input value; output value; polynomial approximation */
  xtfloat x0, x1, y0, q;
  /* Fixed-point input value; polynomial value. */
  int32_t p, g;
  /* Fractional part and its 2nd power. */
  int32_t f0/*, f1*/;
  /* Exponential parts. */
  int32_t e0, e1;
  /* Scale factors. */
  xtfloat s0, s1;
  /* Auxiliary polynomial coeffs for MULs. */
  ae_f32x2 cf0, cf1, cf2, cf3, cf4, cf5, cf6;
  /* Auxiliary vars for MULs. */
  ae_f32x2 f1, f2, h, t;
  /* Auxiliary vars for MAX and MIN. */
  xtbool tb;
  xtfloat tf;
  /* Wide auxiliary vars. */
  ae_int64 w0, w1;

#if SCL_ANTILOG10F_ERRH != 0
  /* Floating-point Status and Control Register values. */
  uint32_t fstate;
#endif

  x0 = x;

  if ( XT_UN_S( x0, x0 ) )
  {
    __Pragma( "frequency_hint never" );
#if SCL_ANTILOG10F_ERRH != 0
    errno = EDOM;
#endif
    return (qNaNf.f);
  }

  /* Check borders. */
  tf = alog10fminmax[0].f;
  x1 = tf;
  tb = XT_OLT_S(tf, x0);
  XT_MOVT_S(x1, x0, tb);
  tf = alog10fminmax[1].f;
  tb = XT_OLT_S(x1, tf);
  XT_MOVF_S(x1, tf, tb);

#if SCL_ANTILOG10F_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_RUR_FSR();
#endif

  p = XT_TRUNC_S( XT_MUL_S( x1, XT_FLOAT_S( 1<<10, 0 ) ), 15 );

  /* Scale input by 1/log10(2) and convert to Q31 */
  w0 = AE_MUL32_HH( p, invlog10_2_Q29 );
  e0 = ae_int32x2_rtor_int32(AE_TRUNCA32X2F64S(w0, w0, -22));
  w1 = AE_SLLI64( w0, 32-22 );
  f0 = ae_int32x2_rtor_int32(AE_TRUNCI32X2F64S(w1, w1, 0));
  f0 = XT_SRLI(f0, 1);

  /*
   * Compute 2^fract(x/log10(2)) in Q30, where f=x/log10(2) is in Q31. Use a
   * combination of Estrin's rule and Horner's method to evaluate the polynomial.
   */

  cf0 = expftbl_Q30[0]; cf1 = expftbl_Q30[1];
  cf2 = expftbl_Q30[2]; cf3 = expftbl_Q30[3];
  cf4 = expftbl_Q30[4]; cf5 = expftbl_Q30[5];
  cf6 = expftbl_Q30[6];

  f1 = int32_rtor_ae_f32(f0);

  f2 = AE_MULFP32X2RAS( f1, f1 );

  t = cf2; AE_MULAFP32X2RAS( t, f1, cf1 ); cf1 = t;
  t = cf4; AE_MULAFP32X2RAS( t, f1, cf3 ); cf2 = t;
  t = cf6; AE_MULAFP32X2RAS( t, f1, cf5 ); cf3 = t;

  t = cf0;                               h = t;
  t = cf1; AE_MULAFP32X2RAS( t, f2, h ); h = t;
  t = cf2; AE_MULAFP32X2RAS( t, f2, h ); h = t;
  t = cf3; AE_MULAFP32X2RAS( t, f2, h ); h = t;

  g = ae_f32_rtor_int32(h);

  q = XT_MUL_S( XT_FLOAT_S( g, 15 ), XT_FLOAT_S( 1, 15 ) );

  /*
   * Calculate 2^int(x/log10(2)) * 2^fract(x/log10(2))
   */

  e0 = XT_ADD( e0, 254 );
  e1 = XT_SRAI( e0, 1 );
  e0 = XT_SUB( e0, e1 );
  e0 = XT_SLLI( e0, 23 );
  e1 = XT_SLLI( e1, 23 );
  s0 = XT_WFR( e0 );
  s1 = XT_WFR( e1 );

  s0 = XT_MUL_S( s0, s1 );
  y0 = XT_MUL_S( s0, q );

#if SCL_ANTILOG10F_ERRH != 0
  {
    /* Is input less than infinity; is output equal to infinity. */
    xtbool b_xfin, b_yinf;

    /* Suppress spurious exception flags and restore original status flags. */
    XT_WUR_FSR(fstate);

    b_xfin = XT_OLT_S( x0, plusInff.f );
    b_yinf = XT_OEQ_S( y0, plusInff.f );

    /* Check for an overflow. */
    if ( XT_ANDB( b_xfin, b_yinf) )
    {
      __Pragma( "frequency_hint never" );
      __feraiseexcept( FE_OVERFLOW );
      errno = ERANGE;
    }
  }
#endif

  return y0;
} /* scl_antilog10f() */
#endif
