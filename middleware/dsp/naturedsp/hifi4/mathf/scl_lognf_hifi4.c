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
#include "lognf_tbl.h"
#include "sqrt2f_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(float32_t,scl_lognf,( float32_t x ))
#elif HAVE_VFPU
/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_LOGNF_ERRH     1

/*===========================================================================
  Scalar matematics:
  scl_log              Logarithm 
===========================================================================*/

/*-------------------------------------------------------------------------
  Logarithm:
  Different kinds of logarithm (base 2, natural, base 10). Fixed point 
  functions represent results in Q25 format or return 0x80000000 on negative 
  of zero input.

  Precision:
  32x32  32-bit inputs, 32-bit outputs
  f      floating point

  Accuracy :
  vec_log2_32x32,scl_log2_32x32              730 (2.2e-5)
  vec_logn_32x32,scl_logn_32x32              510 (1.5e-5)
  vec_log10_32x32,scl_log10_32x32            230 (6.9e-6)
  floating point                             2 ULP

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
  x[N]  input data, Q16.15 or floating point 
  N     length of vectors
  Output:
  y[N]  result, Q25 or floating point 

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  return result in Q25 or floating point
-------------------------------------------------------------------------*/

float32_t scl_lognf( float32_t x )
{
  /*
   * Reference C code for SCL_LOGNF_ERRH == 0:
   *
   *   float32_t y;
   *   int e;
   *   
   *   if ( x<0           ) return ( qNaNf.f     );
   *   if ( x==0          ) return ( minusInff.f );
   *   if ( x==plusInff.f ) return ( x           );
   *   
   *   x = frexpf(x, &e);
   *   if (x<sqrt0_5f.f) { x = x * 2; e--; }
   *   
   *   x = x - 1.0f;
   *   y = lognf_tbl[0].f;
   *   y = lognf_tbl[1].f - x*y;
   *   y = lognf_tbl[2].f - x*y;
   *   y = lognf_tbl[3].f - x*y;
   *   y = lognf_tbl[4].f - x*y;
   *   y = lognf_tbl[5].f - x*y;
   *   y = lognf_tbl[6].f - x*y;
   *   y = lognf_tbl[7].f - x*y;
   *   y = x*y + 1.0f;
   *   y = x*y;
   *   
   *   y = y + e*ln2.f;
   *   return y;
   */

  /* Input value; output value; fractional part and its 2nd power; auxiliary var. */
  xtfloatx2 x0, x1, y0, fr0, fr1, t;
  /* Fixed-point representation of fractional part; exponential part; exponent bias */
  ae_int32x2 fr0_i, ex0, ex1, bias;
  /* Polynomial value; polynomial coefficients */
  xtfloatx2 g, cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7;
  /* Floating-point zero constant. */
  xtfloatx2 zero_f = (xtfloatx2)(0.0f);
  /* Is a subnormal; is less than 2^0.5; is positive infinity */
  xtbool2 b_subn, b_ltsqr, b_inf;

#if SCL_LOGNF_ERRH == 0
  /* Is a NaN or is less than zero; is equal to zero. */
  xtbool2 b_ultz, b_eqz;
#else
  /* Floating-point Status and Control Register values. */
  ae_int64 fstate;
#endif

  x0 = (xtfloatx2)x;

#if SCL_LOGNF_ERRH != 0
  if ( xtbool2_extract_0( XT_UN_SX2( x0, x0 ) ) )
  {
    __Pragma( "frequency_hint never" );
    errno = EDOM;
    return (qNaNf.f);
  }
  else if ( xtbool2_extract_0( XT_OLT_SX2( x0, zero_f ) ) )
  {
    __Pragma( "frequency_hint never" );
    errno = EDOM;
    __feraiseexcept( FE_INVALID );
    return (qNaNf.f);
  }
  else if ( xtbool2_extract_0( XT_OEQ_SX2( x0, zero_f ) ) )
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
  b_subn = XT_OLT_SX2( x0, XT_AE_MOVXTFLOATX2_FROMINT32X2(0x00800000) );

#if SCL_LOGNF_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_AE_MOVVFCRFSR();
#endif
  t = (xtfloatx2)(1.0f);
  /* Multiply subnormals by 2^23 */
  XT_MOVT_SX2( t, XT_AE_MOVXTFLOATX2_FROMINT32X2(0x4b000000), b_subn );
  x1 = XT_MUL_SX2( x0, t );
  fr0_i = XT_AE_MOVINT32X2_FROMXTFLOATX2( x1 );

  bias = 127-1;
  AE_MOVT32X2( bias, 127-1+23, b_subn );
  ex0 = AE_SRLI32( fr0_i, 23 );
  ex0 = AE_SUB32( ex0, bias );

  fr0_i = AE_AND32( fr0_i, (1<<23)-1 );
  fr0_i = AE_OR32( fr0_i, 126<<23 );

  fr0 = XT_AE_MOVXTFLOATX2_FROMINT32X2( fr0_i );

  fr1 = XT_MUL_SX2( fr0, (xtfloatx2)2.0f );
  ex1 = AE_SUB32( ex0, AE_MOVI(1) );

  b_ltsqr = XT_OLT_SX2( fr0, sqrt0_5f.f );
  XT_MOVT_SX2( fr0, fr1, b_ltsqr );
  AE_MOVT32X2( ex0, ex1, b_ltsqr );
  y0 = XT_MUL_SX2( XT_FLOAT_SX2( ex0, 0 ), ln2.f );

  fr0 = XT_SUB_SX2( (xtfloatx2)1.0f, fr0 );
  fr1 = XT_MUL_SX2( fr0, fr0 );

#if SCL_LOGNF_ERRH != 0
  /* Suppress spurious exception flags and restore original status flags. */
  XT_AE_MOVFCRFSRV( fstate );
#endif

  /*                                                              
   * Evaluate the polynomial by Estrin's rule.
   */

  cf0 = (xtfloatx2)lognf_tbl[0].f; cf1 = (xtfloatx2)lognf_tbl[1].f;
  cf2 = (xtfloatx2)lognf_tbl[2].f; cf3 = (xtfloatx2)lognf_tbl[3].f;
  cf4 = (xtfloatx2)lognf_tbl[4].f; cf5 = (xtfloatx2)lognf_tbl[5].f;
  cf6 = (xtfloatx2)lognf_tbl[6].f; cf7 = (xtfloatx2)lognf_tbl[7].f;

  XT_MADDN_SX2( cf1, cf0, fr0 );
  XT_MADDN_SX2( cf3, cf2, fr0 );
  XT_MADDN_SX2( cf5, cf4, fr0 );
  XT_MADDN_SX2( cf7, cf6, fr0 );

                              g = cf1;
  XT_MADDN_SX2( cf3, g, fr1 ); g = cf3;
  XT_MADDN_SX2( cf5, g, fr1 ); g = cf5;
  XT_MADDN_SX2( cf7, g, fr1 ); g = cf7;

  /* 
   * Compute y = fr*g + ex*ln(2)
   */

  t = (xtfloatx2)1.0f; XT_MSUBN_SX2( t, g, fr0 ); g = t;
  XT_MSUBN_SX2( y0, g, fr0 );

  /*
   * Process the special cases.
   */

#if SCL_LOGNF_ERRH != 0
  b_inf = XT_OEQ_SX2( x0, plusInff.f );
  XT_MOVT_SX2( y0, plusInff.f, b_inf ); /* x==Inf -> y=Inf */
#else
  b_inf = XT_OEQ_SX2( x0, plusInff.f );
  b_ultz = XT_ULT_SX2( x0, zero_f );
  b_eqz = XT_OEQ_SX2( x0, zero_f );

  XT_MOVT_SX2( y0, plusInff.f , b_inf  ); /* x==Inf -> y=Inf */
  XT_MOVT_SX2( y0, minusInff.f, b_eqz  ); /* x==0.f -> y=-Inf */
  XT_MOVT_SX2( y0, qNaNf.f    , b_ultz ); /* x<0 || isnan(x) -> y=qNaN */
#endif

  return ( XT_HIGH_S( y0 ) );

} /* scl_lognf() */
#elif HAVE_FPU
/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_LOGNF_ERRH     1

/*===========================================================================
  Scalar matematics:
  scl_log              Logarithm 
===========================================================================*/
/*-------------------------------------------------------------------------
  Logarithm:
  Different kinds of logarithm (base 2, natural, base 10). Fixed point 
  functions represent results in Q25 format or return 0x80000000 on negative 
  of zero input.

  Precision:
  32x32  32-bit inputs, 32-bit outputs
  f      floating point

  Accuracy :
  vec_log2_32x32,scl_log2_32x32       730 (2.2e-5)
  vec_logn_32x32,scl_logn_32x32       510 (1.5e-5)
  vec_log10_32x32,scl_log10_32x32     230 (6.9e-6)
  floating point                      2 ULP

  NOTES:
  1.  Scalar Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly.
  2.  Floating point functions limit the range of allowable input values:
      A) If x<0, the result is set to NaN. In addition, scalar floating point
         functions assign the value EDOM to errno and raise the "invalid" 
         floating-point exception.
      B) If x==0, the result is set to minus infinity. Scalar floating  point
         functions assign the value ERANGE to errno and raise the "divide-by-zero"
         floating-point exception.

  Input:
  x[N]  input data, Q16.15 or floating point 
  N     length of vectors
  Output:
  y[N]  result, Q25 or floating point 

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  return result in Q25 or floating point
-------------------------------------------------------------------------*/

float32_t scl_lognf( float32_t x )
{
  /*
   * Reference C code for SCL_LOGNF_ERRH == 0:
   *
   *   float32_t y;
   *   int e;
   *   
   *   if ( x<0           ) return ( qNaNf.f     );
   *   if ( x==0          ) return ( minusInff.f );
   *   if ( x==plusInff.f ) return ( x           );
   *   
   *   x = frexpf(x, &e);
   *   if (x<sqrt0_5f.f) { x = x * 2; e--; }
   *   
   *   x = x - 1.0f;
   *   y = lognf_tbl[0].f;
   *   y = lognf_tbl[1].f - x*y;
   *   y = lognf_tbl[2].f - x*y;
   *   y = lognf_tbl[3].f - x*y;
   *   y = lognf_tbl[4].f - x*y;
   *   y = lognf_tbl[5].f - x*y;
   *   y = lognf_tbl[6].f - x*y;
   *   y = lognf_tbl[7].f - x*y;
   *   y = x*y + 1.0f;
   *   y = x*y;
   *   
   *   y = y + e*ln2.f;
   *   return y;
   */

  /* Input value; output value; fractional part and its 2nd power; auxiliary var. */
  xtfloat x0, x1, y0, fr0, fr1, t;
  /* Fixed-point representation of fractional part; exponential part; exponent bias */
  int32_t fr0_i, ex0, ex1, bias;
  /* Polynomial value; polynomial coefficients */
  xtfloat g, cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7;
  /* Floating-point zero constant. */
  xtfloat zero_f = (xtfloat)(0.0f);
  /* Is a subnormal; is less than 2^0.5; is positive infinity */
  xtbool b_subn, b_ltsqr, b_inf;

#if SCL_LOGNF_ERRH == 0
  /* Is a NaN or is less than zero; is equal to zero. */
  xtbool b_ultz, b_eqz;
#else
  /* Floating-point Status and Control Register values. */
  uint32_t fstate;
#endif

  x0 = x;

#if SCL_LOGNF_ERRH != 0
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
  b_subn = XT_OLT_S( x0, XT_WFR(0x00800000) );

#if SCL_LOGNF_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_RUR_FSR();
#endif

  t = (xtfloat)(1.0f);
  /* Multiply subnormals by 2^23 */
  XT_MOVT_S( t, XT_WFR(0x4b000000), b_subn );
  x1 = XT_MUL_S( x0, t );
  fr0_i = XT_RFR( x1 );

  bias = 127-1;
  XT_MOVT( bias, 127-1+23, b_subn );
  ex0 = (uint32_t)fr0_i >> 23;
  ex0 = XT_SUB( ex0, bias );

  fr0_i = XT_AND( fr0_i, (1<<23)-1 );
  fr0_i = XT_OR( fr0_i, 126<<23 );

  fr0 = XT_WFR( fr0_i );

  fr1 = XT_MUL_S( fr0, (xtfloat)2.0f );
  ex1 = XT_SUB( ex0, XT_MOVI(1) );

  b_ltsqr = XT_OLT_S( fr0, sqrt0_5f.f );
  XT_MOVT_S( fr0, fr1, b_ltsqr );
  XT_MOVT( ex0, ex1, b_ltsqr );
  y0 = XT_MUL_S( XT_FLOAT_S( ex0, 0 ), ln2.f );

  fr0 = XT_SUB_S( fr0, 1.0f );
  fr1 = XT_MUL_S( fr0, fr0 );

  /*                                                              
   * Evaluate the polynomial by Estrin's rule.
   */

  cf0 = lognf_tbl[0].f; cf1 = lognf_tbl[1].f;
  cf2 = lognf_tbl[2].f; cf3 = lognf_tbl[3].f;
  cf4 = lognf_tbl[4].f; cf5 = lognf_tbl[5].f;
  cf6 = lognf_tbl[6].f; cf7 = lognf_tbl[7].f;

  XT_MSUB_S( cf1, cf0, fr0 );
  XT_MSUB_S( cf3, cf2, fr0 );
  XT_MSUB_S( cf5, cf4, fr0 );
  XT_MSUB_S( cf7, cf6, fr0 );

                             g = cf1;
  XT_MADDN_S( cf3, g, fr1 ); g = cf3;
  XT_MADDN_S( cf5, g, fr1 ); g = cf5;
  XT_MADDN_S( cf7, g, fr1 ); g = cf7;

  /* 
   * Compute y = fr*g + ex*ln(2)
   */

  t = 1.0f;
  XT_MADD_S( t, g, fr0 );
  g = t;
  XT_MADD_S( y0, g, fr0 );

  /*
   * Process the special cases.
   */

#if SCL_LOGNF_ERRH != 0
  b_inf = XT_OEQ_S( x0, plusInff.f );
  XT_MOVT_S( y0, plusInff.f, b_inf ); /* x==Inf -> y=Inf */
  /* Suppress spurious exception flags and restore original status flags. */
  XT_WUR_FSR(fstate);
#else
  b_inf = XT_OEQ_S( x0, plusInff.f );
  b_ultz = XT_ULT_S( x0, zero_f );
  b_eqz = XT_OEQ_S( x0, zero_f );

  XT_MOVT_S( y0, plusInff.f , b_inf  ); /* x==Inf -> y=Inf */
  XT_MOVT_S( y0, minusInff.f, b_eqz  ); /* x==0.f -> y=-Inf */
  XT_MOVT_S( y0, qNaNf.f    , b_ultz ); /* x<0 || isnan(x) -> y=qNaN */
#endif

  return y0;
} /* scl_lognf() */
#endif
