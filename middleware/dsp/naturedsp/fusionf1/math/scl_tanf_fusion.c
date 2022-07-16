/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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
DISCARD_FUN(float32_t, scl_tanf, (float32_t x))
#else
/* Inline functions for floating-point exceptions and environment control. */
#include "__fenv.h"
/* Value of 2/pi, 4/pi, etc. */
#include "inv2pif_tbl.h"
/* Angular argument reduction constants. */
#include "sinf_tbl.h"
/* tan/cotan approximation polynomial coeffs. */
#include "tanf_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_TANF_ERRH     1

/*===========================================================================
  Scalar matematics:
  scl_tan             Tangent    
===========================================================================*/

/*-------------------------------------------------------------------------
  Tangent 
  Fixed point functions calculate tan(pi*x) for number written in Q15 or 
  Q31. Floating point functions compute tan(x)
  
  Precision:
  16x16  16-bit inputs (Q15), 16-bit outputs (Q8.7). Accuracy: 1 LSB
  24x24  24-bit inputs, 32-bit outputs. Accuracy: (1.3e-4*y+1LSB) 
                                        if abs(y)<=464873(14.19 in Q15) 
                                        or abs(x)<pi*0.4776
  32x32  32-bit inputs, 32-bit outputs. Accuracy: (1.3e-4*y+1LSB)
                                        if abs(y)<=464873(14.19 in Q15) 
                                        or abs(x)<pi*0.4776
  f      floating point.                Accuracy: 2 ULP

  NOTE:
  1.  Scalar floating point function is compatible with standard ANSI C routines 
      and set errno and exception flags accordingly
  2.  Floating point functions limit the range of allowable input values: [-9099, 9099]
      Whenever the input value does not belong to this range, the result is set to NaN.

  Input:
  x[N]   input data, Q15, Q31 or floating point
  N      length of vectors
  Output:
  y[N]   result, Q8.7 (16-bit function), Q16.15 (24 or 32-bit functions) 
         or floating point

  Restriction:
  x,y - should not overlap

  Scalar versions:
  ----------------
  Return result, Q8.7 (16-bit function), Q16.15 (24 or 32-bit functions) 
  or floating point
-------------------------------------------------------------------------*/

float32_t scl_tanf (float32_t x)
{
  /* Input value and its absolute; output value. */
  xtfloat x0, x1, z0;
  /* Input sign; result sign; auxiliary var.  */
  ae_int32 sx, sz, s;
  /* Reducted input value and its powers; correction term. */
  xtfloat p, p2, p3, p4, p8, dp;
  /* Auxiliary floating-point vars. */
  xtfloat t, r;
  /* Input value segment number. */
  xtfloat jf;
  ae_int32 ji;
  /* pi/2 splitted into 7-bit chunks. */
  xtfloat c0, c1, c2, c3, c4, c5;
  /* Polynomial coeffs. */
  xtfloat cf0, cf1, cf2, cf3, cf4, cf5, cf6;

#if SCL_TANF_ERRH != 0
  /* Floating-point Status and Control Register values. */
  ae_int64 fstate;
#endif

  /* pi/2 splitted into 7-bit chunks. */
  static const union ufloat32uint32 c[6] = {
    { 0x3fca0000 }, { 0xbbf00000 },
    { 0xb6960000 }, { 0x32880000 },
    { 0x2e860000 }, { 0xaa39ee5a }
  };

  x0 = x;

  if ( XT_UN_S( x0, x0 ) )
  {
    __Pragma( "frequency_hint never" );
#if SCL_TANF_ERRH != 0
    errno = EDOM;
#endif
    return (qNaNf.f);
  }

  if ( XT_OLT_S( tanf_maxval, XT_ABS_S(x0) ) )
  {
    __Pragma( "frequency_hint never" );
    return (qNaNf.f);
  }

  x1 = XT_ABS_S( x0 );

#if SCL_TANF_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_AE_MOVVFCRFSR();
#endif

  /* Determine the pi/2-wide segment the input value belongs to. */
  jf = XT_MUL_S( x1, inv2pif.f );
  jf = XT_FIROUND_S( jf );
  ji = XT_TRUNC_S( jf, 0 );
  ji = AE_SLLI_32( ji, 31 );

  /*
   * Calculate the difference between the segment midpoint and input value.
   */

  c0 = c[0].f; c1 = c[1].f;
  c2 = c[2].f; c3 = c[3].f;
  c4 = c[4].f; c5 = c[5].f;

  p = x1;

  XT_MSUB_S( p, jf, c0 );
  XT_MSUB_S( p, jf, c1 );
  XT_MSUB_S( p, jf, c2 );

  r = XT_MUL_S(jf,c3); t = p; p = XT_SUB_S(p,r); t = XT_SUB_S(t,p); t = XT_SUB_S(t,r); dp = t;
  r = XT_MUL_S(jf,c4); t = p; p = XT_SUB_S(p,r); t = XT_SUB_S(t,p); t = XT_SUB_S(t,r); dp = XT_ADD_S(t,dp);
  r = XT_MUL_S(jf,c5); t = p; p = XT_SUB_S(p,r); t = XT_SUB_S(t,p); t = XT_SUB_S(t,r); dp = XT_ADD_S(t,dp);

  p = XT_ADD_S( p, dp );

  /*
   * Evaluate a polynomial approximation to y(p) = tan(p)/p-1. We use Estrin's rule to
   * shorten the dependency path and allow for parallel computations at the cost
   * of additional multiplications.
   */

  cf0 = polytanf_tbl[0].f; cf1 = polytanf_tbl[1].f;
  cf2 = polytanf_tbl[2].f; cf3 = polytanf_tbl[3].f;
  cf4 = polytanf_tbl[4].f; cf5 = polytanf_tbl[5].f;
  cf6 = polytanf_tbl[6].f;

  p2 = XT_MUL_S( p, p );
  p3 = XT_MUL_S( p2, p );
  p4 = XT_MUL_S( p2, p2 );
  p8 = XT_MUL_S( p4, p4 );

  XT_MADD_S( cf2, cf1, p2 ); cf1 = cf2;
  XT_MADD_S( cf4, cf3, p2 ); cf2 = cf4;
  XT_MADD_S( cf6, cf5, p2 ); cf3 = cf6;

  XT_MADD_S( cf1, cf0, p4 ); cf0 = cf1;
  XT_MADD_S( cf3, cf2, p4 ); cf1 = cf3;

  XT_MADD_S( cf1, cf0, p8 ); cf0 = cf1;

  XT_MADD_S( p, p3, cf0 ); z0 = p;

  /* Compute the cotangent for odd-numbered segments. */
  if ( AE_MOVAD32_H( ji ) < 0 )
  {
    __Pragma( "frequency_hint frequent" );
    z0 = XT_RECIP_S( z0 );
  }

#if SCL_TANF_ERRH != 0
  /* Suppress spurious exception flags and restore original status flags. */
  XT_AE_MOVFCRFSRV( fstate );
#endif

  /* Compute the sign adjustment term. */
  sx = XT_AE_MOVINT32_FROMXTFLOAT( x0 );
  sz = AE_XOR_32( sx, ji );
  sz = AE_SRLI_32( sz, 31 );
  sz = AE_SLLI_32( sz, 31 );

  /* Adjust the sign. */
  s = XT_AE_MOVINT32_FROMXTFLOAT( z0 );
  s = AE_XOR_32( s, sz );
  z0 = XT_AE_MOVXTFLOAT_FROMINT32( s );

  return (z0);

} /* scl_tanf() */
#endif
