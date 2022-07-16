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
#include <float.h>

/* DSP Library API */
#include "NatureDSP_Signal.h"
/* Common helper macros. */
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(float32_t, scl_atan2f, (float32_t y, float32_t x))
#else
/* Inline functions for floating-point exceptions and environment control. */
#include "__fenv.h"
/* Tables */
#include "pif_tbl.h"
#include "atanf_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_ATAN2F_ERRH     1

/*===========================================================================
  Scalar matematics:
  scl_atan2          full quadrant Arctangent        
===========================================================================*/

/*-------------------------------------------------------------------------
Floating-Point Full-Quadrant Arc Tangent
The functions compute the full quadrant arc tangent of the ratio y/x. 
Floating point functions output is in radians. Fixed point functions 
scale its output by pi.

NOTE:
1. Scalar function is compatible with standard ANSI C routines and set 
   errno and exception flags accordingly
2. Scalar function assigns EDOM to errno whenever y==0 and x==0.

Special cases:
     y    |   x   |  result   |  extra conditions    
  --------|-------|-----------|---------------------
   +/-0   | -0    | +/-pi     |
   +/-0   | +0    | +/-0      |
   +/-0   |  x    | +/-pi     | x<0
   +/-0   |  x    | +/-0      | x>0
   y      | +/-0  | -pi/2     | y<0
   y      | +/-0  |  pi/2     | y>0
   +/-y   | -inf  | +/-pi     | finite y>0
   +/-y   | +inf  | +/-0      | finite y>0
   +/-inf | x     | +/-pi/2   | finite x
   +/-inf | -inf  | +/-3*pi/4 | 
   +/-inf | +inf  | +/-pi/4   |

Input:
  y[N]  input data, Q15 or floating point
  x[N]  input data, Q15 or floating point
  N     length of vectors
Output:
  z[N]  result, Q15 or floating point
  
Restrictions:
x, y, z should not overlap
---------------------------------------------------------------------------*/

float32_t scl_atan2f( float32_t y, float32_t x )
{
  /* Polynomial coeff table pointer. */
  const xtfloat * ptbl;
  /* Input values; output value; reducted input value and its 2nd and 4th powers. */
  xtfloat x0, y0, z0, z1, p0, p1, p2;
  /* Numerator; denominator; reciprocal; quotient */
  xtfloat num, den, rcp, quo;
  /* Auxiliary var; error term */
  xtfloat s, eps;
  /* Temporary; input value signs */
  ae_int32 t, sx, sy;
  /* Selected polynomial coeffs. */
  xtfloat cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7;
  /* Inf/Inf; x/Inf; 0/0; x and y are subnormal */
  xtbool b_num_inf, b_den_inf, b_eqz, b_subn;
  /* x less than y; x is negative; p is less than 0.5f. */
  xtbool b_xlty, b_sx, b_lt05;

#if SCL_ATAN2F_ERRH != 0
  /* Floating-point Status and Control Register values. */
  ae_int64 fstate;
#endif

  x0 = x;
  y0 = y;

  if ( XT_UN_S( y0, x0 ) )
  {
    __Pragma( "frequency_hint never" );
#if SCL_ATAN2F_ERRH != 0
    errno = EDOM;
#endif
    return (qNaNf.f);
  }

  /* Keep sign of x as a boolean. */
  sx = XT_AE_MOVINT32_FROMXTFLOAT( x0 );
  b_sx = xtbool2_extract_0( AE_LT32( sx, AE_ZERO32() ) );

  /* Keep y sign as a binary value. */
  sy = XT_AE_MOVINT32_FROMXTFLOAT( y0 );
  sy = AE_SRLI_32( sy, 31 );
  sy = AE_SLLI_32( sy, 31 );

  x0 = XT_ABS_S( x0 );
  y0 = XT_ABS_S( y0 );

#if SCL_ATAN2F_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_AE_MOVVFCRFSR();
#endif

  /* num <= den */
  num = XT_MIN_S( x0, y0 );
  den = XT_MAX_S( y0, x0 );

  /* Scale up numerator and denominator if BOTH are subnormal. */
  b_subn = XT_OLT_S( num, FLT_MIN );
  s = XT_MUL_S( num, 8388608.f ); XT_MOVT_S( num, s, b_subn );
  s = XT_MUL_S( den, 8388608.f ); XT_MOVT_S( den, s, b_subn );

  /* Classify numerator and denominator. */
  b_num_inf = XT_OEQ_S( num, plusInff.f ); /* Inf/Inf */
  b_den_inf = XT_OEQ_S( den, plusInff.f ); /* x/Inf   */
  b_eqz = XT_OEQ_S( den, XT_CONST_S(0) );  /* 0/0     */
  /* Initial appromimation for 1/den. */
  rcp = XT_RECIP0_S( den );
  /* Newton-Raphson iteration for 1/den. */
  eps = XT_CONST_S(1);
  XT_MSUB_S( eps, rcp, den );
  XT_MADD_S( rcp, rcp, eps );
  /* Approximation for the quotient num/den. */
  quo = XT_MUL_S( num, rcp );
  /* Refine the quotient by a modified Newton-Raphson iteration. */
  eps = num;
  XT_MSUB_S( eps, quo, den );
  XT_MADD_S( quo, rcp, eps );
  /* Force conventional results for special cases. */
  XT_MOVT_S( quo, XT_CONST_S(0), b_den_inf ); /* x/Inf -> 0   */
  XT_MOVT_S( quo, XT_CONST_S(1), b_num_inf ); /* Inf/Inf -> 1 */
  XT_MOVT_S( quo, XT_CONST_S(0), b_eqz     ); /* 0/0 -> 0     */

  p0 = quo;

  b_xlty = XT_OLT_S( x0, y0 );
  b_lt05 = XT_OLT_S( p0, XT_CONST_S(3) );

  /* Select coeffs from sets #1, #2 by reducted input value. */
  ptbl = (xtfloat*)( b_lt05 ? atanftbl1 : atanftbl2 );

  cf0 = ptbl[0]; cf1 = ptbl[1]; cf2 = ptbl[2]; cf3 = ptbl[3];
  cf4 = ptbl[4]; cf5 = ptbl[5]; cf6 = ptbl[6]; cf7 = ptbl[7];

  /*
   * Compute the approximation to z(p) = tan(p)/p-1. We use Estrin's rule to
   * shorten the dependency path and allow for parallel computations at the cost
   * of additional multiplications.
   */

  p1 = XT_MUL_S( p0, p0 ); /* p^2 */
  p2 = XT_MUL_S( p1, p1 ); /* p^4 */

  XT_MADD_S( cf1, cf0, p0 ); cf0 = cf1;
  XT_MADD_S( cf3, cf2, p0 ); cf1 = cf3;
  XT_MADD_S( cf5, cf4, p0 ); cf2 = cf5;
  XT_MADD_S( cf7, cf6, p0 ); cf3 = cf7;

  XT_MADD_S( cf1, cf0, p1 ); cf0 = cf1;
  XT_MADD_S( cf3, cf2, p1 ); cf1 = cf3;

  XT_MADD_S( cf1, cf0, p2 ); cf0 = cf1;

  /* atan(x) = z(p)*p+p */
  XT_MADD_S( p0, cf0, p0 ); z0 = p0;

  /* if ( x0<y0 ) z0 = pi2f.f - z0; */
  z1 = XT_SUB_S( pi2f.f, z0 ); XT_MOVT_S( z0, z1, b_xlty );
  /* if ( sx ) z0 = pif.f - z0; */
  z1 = XT_SUB_S( pif.f, z0 ); XT_MOVT_S( z0, z1, b_sx );
  /* if ( sy ) z0 = -z0; */
  t = XT_AE_MOVINT32_FROMXTFLOAT( z0 );
  t = AE_XOR_32( t, sy );
  z0 = XT_AE_MOVXTFLOAT_FROMINT32( t );

#if SCL_ATAN2F_ERRH != 0
  /* Suppress spurious exception flags and restore original status flags. */
  XT_AE_MOVFCRFSRV( fstate );
  /* Assign EDOM to errno whenever x==0 and y==0. */
  if ( b_eqz ) { __Pragma( "frequency_hint never" ); errno = EDOM; };
#endif

  return (z0);

} /* scl_atan2f() */
#endif
