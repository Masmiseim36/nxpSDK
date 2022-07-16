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
DISCARD_FUN(float32_t, scl_sinef, (float32_t x))
#else
/* Inline functions for floating-point exceptions and environment control. */
#include "__fenv.h"
/* Tables */
#include "inv2pif_tbl.h"
#include "sinf_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_SINEF_ERRH    1

/*===========================================================================
  Scalar matematics:
  scl_sine            sine    
===========================================================================*/

/*-------------------------------------------------------------------------
  Sine/Cosine 
  Fixed-point functions calculate sin(pi*x) or cos(pi*x) for numbers written 
  in Q31 or Q15 format. Return results in the same format. 
  Floating point functions compute sin(x) or cos(x)
  Two versions of functions available: regular version (vec_sine16x16, 
  vec_cosine16x16, vec_sine24x24, vec_cosine24x24, vec_sine32x32, 
  vec_cosine32x32, , vec_sinef, vec_cosinef) with arbitrary arguments and 
  faster version (vec_sine24x24_fast, vec_cosine24x24_fast, 
  vec_sine32x32_fast, vec_cosine32x32_fast) that apply some restrictions.
  NOTE:
  1.  Scalar floating point functions are compatible with standard ANSI C
      routines and set errno and exception flags accordingly
  2.  Floating point functions limit the range of allowable input values:
      [-102940.0, 102940.0] Whenever the input value does not belong to this
      range, the result is set to NaN.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: 2 LSB  
  24x24  24-bit inputs, 24-bit output. Accuracy: 74000(3.4e-5)
  32x32  32-bit inputs, 32-bit output. Accuracy: 1700 (7.9e-7)
  f      floating point. Accuracy 2 ULP

  Input:
  x[N]  input data, Q15, Q31 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31,Q15 or floating point

  Restriction:
  Regular versions (vec_sine24x24, vec_cosine24x24, vec_sine32x32, 
  vec_cosine32x32, vec_sinef, vec_cosinef):
  x,y - should not overlap

  Faster versions (vec_sine24x24_fast, vec_cosine24x24_fast, 
  vec_sine32x32_fast, vec_cosine32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result in Q31,Q15 or floating point
-------------------------------------------------------------------------*/

float32_t scl_sinef( float32_t x )
{
  /* Polynomial coeff table pointer. */
  const xtfloat * ptbl;
  /* Input value and its absolute; output value */
  xtfloat x0, x1, y0;
  /* Reducted input value and its 2nd and 3rd powers; correction term. */
  xtfloat p, p2, p3, dp;
  /* Polynomial value; sine and cosine approximations. */
  xtfloat g, sn, cs;
  /* Input and output signs; integer reprentation of output value */
  ae_int32 sx, sy, y0_i;
  /* Auxiliary floating-point vars. */
  xtfloat t, r;
  /* Input value segment number. */
  xtfloat jf;
  ae_int32 ji;
  /* pi/2 splitted into 7-bit chunks. */
  xtfloat c0, c1, c2, c3, c4, c5;
  /* Polynomial coeffs. */
  xtfloat cf0, cf1, cf2;
  /* Select the cosine. */
  xtbool b_cs;

#if SCL_SINEF_ERRH != 0
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
#if SCL_SINEF_ERRH != 0
    errno = EDOM;
#endif
    return (qNaNf.f);
  }

  if ( XT_OLT_S( sinf_maxval.f, XT_ABS_S(x0) ) )
  {
    __Pragma( "frequency_hint never" );
    return (qNaNf.f);
  }

  x1 = XT_ABS_S( x0 );

#if SCL_SINEF_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_AE_MOVVFCRFSR();
#endif

  /* Determine the pi/2-wide segment the input value belongs to. */
  jf = XT_MUL_S( x1, inv2pif.f );
  jf = XT_FIROUND_S( jf );
  ji = XT_TRUNC_S( jf, 0 );

  /* Determine the input sign. */
  sx = XT_AE_MOVINT32_FROMXTFLOAT( x0 );

  /* Determine the output sign. */
  sy = AE_SLLI_32( ji, 30 );
  sy = AE_XOR_32( sx, sy );
  sy = AE_SRLI_32( sy, 31 );
  sy = AE_SLLI_32( sy, 31 );
  
  /*
   * Range reduction.
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
  p2 = XT_MUL_S( p, p );
  p3 = XT_MUL_S( p2, p );

#if SCL_SINEF_ERRH != 0
  /* Suppress spurious exception flags and restore original status flags. */
  XT_AE_MOVFCRFSRV( fstate );
#endif

  /*
   * Polynomial approximation: g(p) = sin(p)/p or g(p) = cos(p).
   */

  /* Use cosine approximation for an odd-numbered segment. */
  b_cs = xtbool2_extract_0( AE_LT32( AE_SLLI_32( ji, 31 ), AE_ZERO32() ) );

  /* Select the coeff table. */
  ptbl = (xtfloat*)( b_cs ? polycosf_tbl : polysinf_tbl );

  /* Common part of polynomial evaluation. */
  cf0 = ptbl[0];                           g = cf0;
  cf1 = ptbl[1]; XT_MADDN_S( cf1, g, p2 ); g = cf1;
  cf2 = ptbl[2]; XT_MADDN_S( cf2, g, p2 ); g = cf2;

  /* Finalize evaluation for sine and cosine. */
  sn = p            ; XT_MADDN_S( sn, g, p3 );
  cs = XT_CONST_S(1); XT_MADDN_S( cs, g, p2 );

  /* Select sine or cosine approximation. */
  y0 = sn; XT_MOVT_S( y0, cs, b_cs );

  /* Adjust the output sign. */
  y0_i = XT_AE_MOVINT32_FROMXTFLOAT( y0 );
  y0_i = AE_XOR_32( sy, y0_i );
  y0 = XT_AE_MOVXTFLOAT_FROMINT32( y0_i );

  return (y0);

} /* scl_sinef() */
#endif
