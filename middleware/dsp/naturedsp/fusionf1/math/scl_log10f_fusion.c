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

#include <errno.h>

/* DSP Library API */
#include "NatureDSP_Signal.h"
/* Common helper macros. */
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(float32_t, scl_log10f, (float32_t x))
#else
/* Inline functions for floating-point exceptions and environment control. */
#include "__fenv.h"
/* Tables */
#include "log10f_tbl.h"
#include "sqrt2f_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_LOG10F_ERRH     1

/*===========================================================================
  Scalar matematics:
  scl_log              Logarithm 
===========================================================================*/
/*-------------------------------------------------------------------------
  Logarithm:
  Different kinds of logarithm (base 2, natural, base 10). 32 and 24-bit 
  fixed point functions interpret input as Q16.15 and represent results in 
  Q25 format or return 0x80000000 on negative of zero input. 16-bit fixed-
  point functions interpret input as Q8.7 and represent result in Q3.12 or
  return 0x8000 on negative of zero input

  Precision:
  16x16  16-bit inputs, 16-bit outputs
  24x24  24-bit inputs, 24-bit outputs
  32x32  32-bit inputs, 32-bit outputs
  f      floating point

  Accuracy :
  16x16 functions                                                    2 LSB
  vec_log2_32x32,scl_log2_32x32  , vec_log2_24x24,scl_log2_24x24     730 (2.2e-5)
  vec_logn_32x32,scl_logn_32x32  , vec_logn_24x24,scl_logn_24x24     510 (1.5e-5)
  vec_log10_32x32,scl_log10_32x32, vec_log10_24x24,scl_log10_24x24   230 (6.9e-6)
  floating point                                                     2 ULP

  NOTES:
  1.  Although 32 and 24 bit functions provide the same accuracy, 32-bit 
      functions have better input/output resolution (dynamic range)
  2.  Scalar Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly.
  3.  Floating point functions limit the range of allowable input values:
      A) If x<0, the result is set to NaN. In addition, scalar floating point
         functions assign the value EDOM to errno and raise the "invalid" 
         floating-point exception.
      B) If x==0, the result is set to minus infinity. Scalar floating  point
         functions assign the value ERANGE to errno and raise the "divide-by-zero"
         floating-point exception.

  Input:
  x[N]  input data, Q16.15 (32 or 24-bit functions), Q8.7 (16-bit functions) or 
        floating point 
  N     length of vectors
  Output:
  y[N]  result, Q6.25 (32 or 24-bit functions), Q3.12 (16-bit functions) or 
        floating point 

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  return result result, Q6.25 (32 or 24-bit functions), Q3.12 (16-bit 
  functions) or floating point
-------------------------------------------------------------------------*/

float32_t scl_log10f( float32_t x )
{
#if LOGF10_TBL_ORDER == 9
  /* Input value; output value; fractional part and its 2nd and 4th powers; auxiliary var. */
  xtfloat x0, x1, y0, fr0, fr1, fr2, t;
  /* Fixed-point representation of fractional part; exponential part; exponent bias */
  ae_int32 fr0_i, ex0, ex1, bias;
  /* Polynomial value; polynomial coefficients */
  xtfloat g, cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7, cf8;
  /* Floating-point zero constant. */
  xtfloat zero_f = XT_CONST_S(0);
  /* Is a subnormal; is less than 2^0.5; is positive infinity */
  xtbool b_subn, b_ltsqr, b_inf;

#if SCL_LOG10F_ERRH == 0
  /* Is a NaN or is less than zero; is equal to zero. */
  xtbool b_ultz, b_eqz;
#else
  /* Floating-point Status and Control Register values. */
  ae_int64 fstate;
#endif

  x0 = x;

#if SCL_LOG10F_ERRH != 0
  if ( XT_UN_S( x0, x0 ) )
  {
    __Pragma( "frequency_hint never" );
    errno = EDOM;
    return (qNaNf.f);
  }
  else if ( XT_OLT_S( x0, zero_f ) )
  {
    __Pragma( "frequency_hint never" );
    errno = EDOM;
    __feraiseexcept( FE_INVALID );
    return (qNaNf.f);
  }
  else if ( XT_OEQ_S( x0, zero_f ) )
  {
    __Pragma( "frequency_hint never" );
    errno = ERANGE;
    __feraiseexcept( FE_DIVBYZERO );
    return (minusInff.f);
  }
#endif

  /*
   * frexpf(x)
   */

  /* Compare with smallest positive normal number 2^-126 */
  b_subn = XT_OLT_S( x0,  XT_AE_MOVXTFLOAT_FROMINT32(0x00800000) );

#if SCL_LOG10F_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_AE_MOVVFCRFSR();
#endif

  t = XT_CONST_S(1);
  /* Multiply subnormals by 2^23 */
  XT_MOVT_S( t, XT_AE_MOVXTFLOAT_FROMINT32(0x4b000000), b_subn );
  x1 = XT_MUL_S( x0, t );
  fr0_i = XT_AE_MOVINT32_FROMXTFLOAT( x1 );

  bias = 127-1;
  AE_MOVT_32( bias, 127-1+23, b_subn );
  ex0 = AE_SRLI_32( fr0_i, 23 );
  ex0 = AE_SUB_32( ex0, bias );

  fr0_i = AE_AND_32( fr0_i, (1<<23)-1 );
  fr0_i = AE_OR_32( fr0_i, 126<<23 );

  fr0 = XT_AE_MOVXTFLOAT_FROMINT32( fr0_i );

  fr1 = XT_MUL_S( fr0, XT_CONST_S(2) );
  ex1 = AE_SUB_32( ex0, AE_MOVI(1) );

  b_ltsqr = XT_OLT_S( fr0, sqrt0_5f.f );
  XT_MOVT_S( fr0, fr1, b_ltsqr );
  AE_MOVT_32( ex0, ex1, b_ltsqr );
  y0 = XT_FLOAT_S( ex0, 2 );

  fr0 = XT_SUB_S( XT_CONST_S(1), fr0 );
  fr2 = XT_MUL_S(fr0, fr0);

#if SCL_LOG10F_ERRH != 0
  /* Suppress spurious exception flags and restore original status flags. */
  XT_AE_MOVFCRFSRV( fstate );
#endif

  /*                                                              
   * Use a combination of Estrin's method and Horner's scheme to evaluate
   * the polynomial.                                     
   */

  cf0 = log10f_tbl[0].f; cf1 = log10f_tbl[1].f;
  cf2 = log10f_tbl[2].f; cf3 = log10f_tbl[3].f;
  cf4 = log10f_tbl[4].f; cf5 = log10f_tbl[5].f;
  cf6 = log10f_tbl[6].f; cf7 = log10f_tbl[7].f;
  cf8 = log10f_tbl[8].f; 

  XT_MADDN_S(cf2, cf1, fr0);
  XT_MADDN_S(cf4, cf3, fr0);
  XT_MADDN_S(cf6, cf5, fr0);
  XT_MADDN_S(cf8, cf7, fr0);

  g = cf0;
  XT_MADDN_S(cf2, g, fr2); g = cf2;
  XT_MADDN_S(cf4, g, fr2); g = cf4;
  XT_MADDN_S(cf6, g, fr2); g = cf6;
  XT_MADDN_S(cf8, g, fr2); g = cf8;

  /* 
   * Compute y = fr*g + ex*log10(2)
   */

  XT_MADDN_S( y0, y0, _4log10_2minus1.f );
  XT_MADDN_S( y0, g, fr0 );

  /*
   * Process the special cases.
   */

#if SCL_LOG10F_ERRH != 0
  b_inf = XT_OEQ_S( x0, plusInff.f );
  XT_MOVT_S( y0, plusInff.f, b_inf ); /* x==Inf -> y=Inf */
#else
  b_inf = XT_OEQ_S( x0, plusInff.f );
  b_ultz = XT_ULT_S( x0, zero_f );
  b_eqz = XT_OEQ_S( x0, zero_f );

  XT_MOVT_S( y0, plusInff.f , b_inf  ); /* x==Inf -> y=Inf */
  XT_MOVT_S( y0, minusInff.f, b_eqz  ); /* x==0.f -> y=-Inf */
  XT_MOVT_S( y0, qNaNf.f    , b_ultz ); /* x<0 || isnan(x) -> y=qNaN */
#endif

  return (y0);
#else
#error wrong LOGF10_TBL_ORDER
#endif

} /* scl_log10f() */
#endif
