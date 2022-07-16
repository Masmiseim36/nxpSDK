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
DISCARD_FUN(float32_t, scl_antilog2f, (float32_t x))
#else

/* Inline functions for floating-point exceptions and environment control. */
#include "__fenv.h"
/* Tables */
#include "expf_tbl.h"
#include "alog2f_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_ANTILOG2F_ERRH     1

/*===========================================================================
  Scalar matematics:
  scl_antilog          Antilogarithm         
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

float32_t scl_antilog2f (float32_t x)
{
  /* Input value; saturated input value; integer part of input number. */
  xtfloat x0, x1, x2;
  /* Output value; polynomial approximation */
  xtfloat y0, q;
  /* Polynomial coeffs. */
  ae_int32x2 cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7, cf8;
  /* Fractional part in Q23 and Q31; partial polynomial sums. */
  ae_int32x2 f0, f1, g0, g1;
  /* Polynomial value. */
  ae_int32 g;
  /* Exponential parts. */
  ae_int32 e0, e1;
  /* Scale factors. */
  xtfloat s0, s1;
  /* Auxiliary var. */
  ae_f32x2 t;

#if SCL_ANTILOG2F_ERRH != 0
  /* Floating-point Status and Control Register values. */
  ae_int64 fstate;

  x0 = x;

  if ( XT_UN_S( x0, x0 ) )
  {
    __Pragma( "frequency_hint never" );
    errno = EDOM;
    return (qNaNf.f);
  }
#else
  x0 = x;

  if ( XT_UN_S( x0, x0 ) )
  {
    __Pragma( "frequency_hint never" );
    return (qNaNf.f);
  }
#endif

  x1 = XT_MAX_S( alog2fminmax[0].f, x0 );
  x1 = XT_MIN_S( alog2fminmax[1].f, x1 );

#if SCL_ANTILOG2F_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_AE_MOVVFCRFSR();
#endif

  x2 = XT_FIFLOOR_S( x1 );
  e0 = XT_TRUNC_S( x2, 0 );
  x2 = XT_SUB_S( x1, x2 );
  f0 = AE_MOVINT32X2_FROMINT32( XT_TRUNC_S( x2, 23 ) );
  f1 = AE_MOVINT32X2_FROMINT32( XT_TRUNC_S( x2, 31 ) );

  /*
   * Compute 2^fract(x) in Q30, where x is in Q23/Q31 (the polynomial is splitted into
   * 2 parts).
   */

  cf0 = expftblpq[0]; cf1 = expftblpq[1];
  cf2 = expftblpq[2]; cf3 = expftblpq[3];
  cf4 = expftblpq[4]; cf5 = expftblpq[5];
  cf6 = expftblpq[6]; cf7 = expftblpq[7];
  cf8 = expftblpq[8];

  /* Compute the first part of 2^t using 24x24 multiplications. */
  t = cf0;                                                                               g0 = t;
  t = cf1; AE_MULAFP24X2RA( t, AE_MOVF24X2_FROMINT32X2(f0), AE_MOVF24X2_FROMF32X2(g0) ); g0 = t;
  t = cf2; AE_MULAFP24X2RA( t, AE_MOVF24X2_FROMINT32X2(f0), AE_MOVF24X2_FROMF32X2(g0) ); g0 = t;
  t = cf3; AE_MULAFP24X2RA( t, AE_MOVF24X2_FROMINT32X2(f0), AE_MOVF24X2_FROMF32X2(g0) ); g0 = t;
  t = cf4; AE_MULAFP24X2RA( t, AE_MOVF24X2_FROMINT32X2(f0), AE_MOVF24X2_FROMF32X2(g0) ); g0 = t;
  t = cf5; AE_MULAFP24X2RA( t, AE_MOVF24X2_FROMINT32X2(f0), AE_MOVF24X2_FROMF32X2(g0) ); g0 = t;
  t =      AE_MULFP24X2RA(     AE_MOVF24X2_FROMINT32X2(f0), AE_MOVF24X2_FROMF32X2(g0) ); g0 = t;

  /* Compute the second part of 2^t using 32x32 multiplications. */
  t = cf6;                                g1 = t;
  t = cf7; AE_MULAFP32X2RAS( t, f1, g1 ); g1 = t;
  t = cf8; AE_MULAFP32X2RAS( t, f1, g1 ); g1 = t;

  /* Combine the partial sums and convert the result to floating-point. */
  g0 = AE_SLAI32S( g0, 4 );
  g = AE_MOVINT32_FROMINT32X2( AE_ADD32S( g0, g1 ) );
  q = XT_FLOAT_S( g, 30 );

  /*
   * Calculate 2^int(x) * 2^fract(x)
   */

  e0 = AE_ADD32( e0, 254 );
  e1 = AE_SRAI32( e0, 1 );
  e0 = AE_SUB32( e0, e1 );
  e0 = AE_SLAI32( e0, 23 );
  e1 = AE_SLAI32( e1, 23 );
  s0 = XT_AE_MOVXTFLOATX2_FROMINT32X2( e0 );
  s1 = XT_AE_MOVXTFLOATX2_FROMINT32X2( e1 );

  s0 = XT_MUL_S( s0, s1 );
  y0 = XT_MUL_S( s0, q );

#if SCL_ANTILOG2F_ERRH != 0
  {
    /* Is input less than infinity; is output equal to infinity. */
    xtbool b_xfin, b_yinf;

    /* Suppress spurious exception flags and restore original status flags. */
    XT_AE_MOVFCRFSRV( fstate );

    b_xfin = XT_OLT_S( x0, plusInff.f );
    b_yinf = XT_OEQ_S( y0, plusInff.f );

    /* Check for an overflow. */
    if ( XT_ANDB( b_xfin, b_yinf ) )
    {
      __Pragma( "frequency_hint never" );
      __feraiseexcept( FE_OVERFLOW );
      errno = ERANGE;
    }
  }
#endif

  return (y0);

} /* scl_antilog2f() */
#endif
