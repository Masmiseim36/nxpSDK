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
#include <float.h>

/* DSP Library API */
#include "NatureDSP_Signal_math.h"
/* Common helper macros. */
#include "common_fpu.h"
/* Inline functions for floating-point exceptions and environment control. */
#include "__fenv.h"
/* Tables */
#include "pif_tbl.h"
#include "atanf_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(float32_t,scl_atan2f,( float32_t y, float32_t x ))
#elif HAVE_VFPU
/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_ATAN2F_ERRH     1

/*===========================================================================
  Scalar matematics:
  scl_atan2          full quadrant Arctangent        
===========================================================================*/

/*-------------------------------------------------------------------------
  Full-Quadrant Arc Tangent
  The functions compute the arc tangent of the ratios y[N]/x[N] and store the
  result to output vector z[N]. 
  Floating point functions output is in radians. Fixed point functions
  scale its output by pi.

  NOTE:
  1. Scalar floating point function is compatible with standard ANSI C routines and set 
     errno and exception flags accordingly
  2. Scalar floating point function assigns EDOM to errno whenever y==0 and x==0.

  Accuracy:
  24 bit version: 768 (3.57e-7)
  floating point: 2 ULP

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
    y[N]  vector of numerator values, Q31 or floating point
    x[N]  vector of denominator values, Q31 or floating point
    N     length of vectors
  Output:
    z[N]  results, Q31 or floating point

---------------------------------------------------------------------------*/

float32_t scl_atan2f( float32_t y, float32_t x )
{
  /*
   * Reference C code for SCL_ATAN2F_ERRH == 0:
   *
   *   const union ufloat32uint32* p;
   *   int sx,sy,big;
   *   sx=takesignf(x);
   *   sy=takesignf(y);
   *   x=fabs(x);
   *   y=fabs(y);
   *   if(x==0.f && y==0.f)
   *   {
   *     // The actual result depends on input signs.
   *     x = 1.f;
   *     y = 0.f;
   *   }
   *  
   *   big=x>y;
   *   if (big)
   *   {
   *     x=y/x;
   *   }
   *   else
   *   {
   *     // compare x==y is necessary to support (+/-Inf, +/-Inf) cases
   *     x=(x==y)? 1.0f : x/y;
   *   }
   *   p=(x<0.5f) ? atanftbl1:atanftbl2;
   *   // approximate atan(x)/x-1
   *   y=    p[0].f;
   *   y=x*y+p[1].f;
   *   y=x*y+p[2].f;
   *   y=x*y+p[3].f;
   *   y=x*y+p[4].f;
   *   y=x*y+p[5].f;
   *   y=x*y+p[6].f;
   *   y=x*y+p[7].f;
   *   // convert result to true atan(x)
   *   y=x*y+x;
   *   if(!big) y=pi2f.f-y;
   *   if(sx)   y=pif.f -y;
   *   if(sy)   y=-y;
   *   return   y;
   */

  /* Polynomial coeff table pointer. */
  const xtfloat * ptbl;
  /* Input values; output value; reducted input value and its 2nd power. */
  xtfloatx2 x0, y0, z0, z1, p0, p1;
  /* Numerator; denominator; reciprocal; quotient */
  xtfloatx2 num, den, rcp, quo;
  /* Auxiliary var; error term */
  xtfloatx2 s, eps;
  /* Temporary; input value signs */
  ae_int32x2 t, sx, sy;
  /* Selected polynomial coeffs. */
  xtfloatx2 cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7;
  /* Inf/Inf; x/Inf; 0/0; x and y are subnormal */
  xtbool2 b_num_inf, b_den_inf, b_eqz, b_subn;
  /* x less than y; x is negative; p is less than 0.5f. */
  xtbool2 b_xlty, b_sx, b_lt05;

#if SCL_ATAN2F_ERRH != 0
  /* Floating-point Status and Control Register values. */
  ae_int64 fstate;
#endif

  x0 = (xtfloatx2)x;
  y0 = (xtfloatx2)y;

  if ( xtbool2_extract_0( XT_UN_SX2( y0, x0 ) ) )
  {
    __Pragma( "frequency_hint never" );
#if SCL_ATAN2F_ERRH != 0
    errno = EDOM;
#endif
    return (qNaNf.f);
  }

  /* Keep sign of x as a boolean. */
  sx = XT_AE_MOVINT32X2_FROMXTFLOATX2( x0 );
  b_sx = AE_LT32( sx, AE_ZERO32() );

  /* Keep y sign as a binary value. */
  sy = XT_AE_MOVINT32X2_FROMXTFLOATX2( y0 );
  sy = AE_SRLI32( sy, 31 );
  sy = AE_SLLI32( sy, 31 );

  x0 = XT_ABS_SX2( x0 );
  y0 = XT_ABS_SX2( y0 );

#if SCL_ATAN2F_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_AE_MOVVFCRFSR();
#endif

  /* num <= den */
  num = XT_MIN_SX2( x0, y0 );
  den = XT_MAX_SX2( y0, x0 );

  /* Scale up numerator and denominator if BOTH are subnormal. */
  b_subn = XT_OLT_SX2( num, FLT_MIN );
  s = XT_MUL_SX2( num, 8388608.f ); XT_MOVT_SX2( num, s, b_subn );
  s = XT_MUL_SX2( den, 8388608.f ); XT_MOVT_SX2( den, s, b_subn );

  /* Classify numerator and denominator. */
  b_num_inf = XT_OEQ_SX2( num, plusInff.f ); /* Inf/Inf */
  b_den_inf = XT_OEQ_SX2( den, plusInff.f ); /* x/Inf   */
  b_eqz = XT_OEQ_SX2( den, (xtfloatx2)(0.0f) );  /* 0/0     */
  /* Initial appromimation for 1/den. */
  rcp = XT_RECIP0_SX2( den );
  /* Newton-Raphson iteration for 1/den. */
  eps = (xtfloatx2)(1.0f);
  XT_MSUB_SX2( eps, rcp, den );
  XT_MADD_SX2( rcp, rcp, eps );
  /* Approximation for the quotient num/den. */
  quo = XT_MUL_SX2( num, rcp );
  /* Refine the quotient by a modified Newton-Raphson iteration. */
  eps = num;
  XT_MSUB_SX2( eps, quo, den );
  XT_MADD_SX2( quo, rcp, eps );

  /* Force conventional results for special cases. */
  XT_MOVT_SX2(quo, (xtfloatx2)(0.0f), b_den_inf); /* x/Inf -> 0   */
  XT_MOVT_SX2(quo, (xtfloatx2)(1.0f), b_num_inf); /* Inf/Inf -> 1 */
  XT_MOVT_SX2(quo, (xtfloatx2)(0.0f), b_eqz); /* 0/0 -> 0     */

  p0 = quo;

  b_xlty = XT_OLT_SX2( x0, y0 );
  b_lt05 = XT_OLT_SX2( p0, (xtfloatx2)0.5f );

  /* Select coeffs from sets #1, #2 by reducted input value. */
  ptbl = (xtfloat*)( xtbool2_extract_0( b_lt05 ) ? atanftbl1 : atanftbl2 );

  cf0 = ptbl[0]; cf1 = ptbl[1]; cf2 = ptbl[2]; cf3 = ptbl[3];
  cf4 = ptbl[4]; cf5 = ptbl[5]; cf6 = ptbl[6]; cf7 = ptbl[7];

  /*
   * Compute the approximation to z(p) = tan(p)/p-1. We use Estrin's rule to
   * shorten the dependency path and allow for parallel computations at the cost
   * of additional multiplications.
   */

  XT_MADD_SX2( cf1, cf0, p0 ); cf0 = cf1;
  XT_MADD_SX2( cf3, cf2, p0 ); cf1 = cf3;
  XT_MADD_SX2( cf5, cf4, p0 ); cf2 = cf5;
  XT_MADD_SX2( cf7, cf6, p0 ); cf3 = cf7;

  p1 = XT_MUL_SX2( p0, p0 );

                              z0 = cf0;
  XT_MADD_SX2( cf1, z0, p1 ); z0 = cf1;
  XT_MADD_SX2( cf2, z0, p1 ); z0 = cf2;
  XT_MADD_SX2( cf3, z0, p1 ); z0 = cf3;
  /* atan(x) = z(p)*p+p */

  XT_MADD_SX2( p0, p0, z0 ); z0 = p0;

  /* if ( x0<y0 ) z0 = pi2f.f - z0; */
  z1 = XT_SUB_SX2( pi2f.f, z0 ); XT_MOVT_SX2( z0, z1, b_xlty );
  /* if ( sx ) z0 = pif.f - z0; */
  z1 = XT_SUB_SX2( pif.f, z0 ); XT_MOVT_SX2( z0, z1, b_sx );
  /* if ( sy ) z0 = -z0; */
  t = XT_AE_MOVINT32X2_FROMXTFLOATX2( z0 );
  t = AE_XOR32( t, sy );
  z0 = XT_AE_MOVXTFLOATX2_FROMINT32X2( t );

#if SCL_ATAN2F_ERRH != 0
  /* Suppress spurious exception flags and restore original status flags. */
  XT_AE_MOVFCRFSRV( fstate );
  /* Assign EDOM to errno whenever x==0 and y==0. */
  if ( xtbool2_extract_0( b_eqz ) ) { __Pragma( "frequency_hint never" ); errno = EDOM; };
#endif

  return ( XT_HIGH_S( z0 ) );

} /* scl_atan2f() */
#elif HAVE_FPU
/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_ATAN2F_ERRH    1
#define sz_f32    (int)sizeof(float32_t)

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

// Taken from Fusion
// Using vector from Fusion
//  because scalar from Fusion applies another way of approximation
float32_t scl_atan2f( float32_t y, float32_t x )
{
  /* Input values; output value; reducted input value and its 2nd and 4th powers. */
  xtfloat x0, x1, y0, y1, z0, z1, p0, i0;
  /* Numerator; denominator; reciprocal; quotient */
  xtfloat num, den, rcp, quo;
  /* Auxiliary var; error term */
  xtfloat s, eps;
  /* Selected polynomial coeffs. */
  xtfloat cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7;
  /* Polynomial coeffs for 0.f<=p<0.5f (#1) and 0.5f<=p<=1.f (#2). */
  xtfloat cf1_0, cf1_1, cf1_2, cf1_3, cf1_4, cf1_5, cf1_6, cf1_7;
  xtfloat cf2_0, cf2_1, cf2_2, cf2_3, cf2_4, cf2_5, cf2_6, cf2_7;
  /* Is NaN; Inf/Inf; x/Inf; 0/0; x and y are subnormal */
  xtbool b_nan, b_num_inf, b_den_inf, b_eqz, b_subn;
  /* x less than y; x is negative; num/den is less than 0.5f. */
  xtbool b_xlty, b_lt05;

  const xtfloat * pT;
  xtbool b_min_y0_x0;
  xtbool b_eqz_or_den_inf;

  /*
  * const union ufloat32uint32* p;
  * int sx,sy,big;
  * sx=takesignf(x);
  * sy=takesignf(y);
  * x=fabs(x);
  * y=fabs(y);
  * if(x==0.f && y==0.f)
  * {
  * // The actual result depends on input signs.
  * x = 1.f;
  * y = 0.f;
  * }
  * 
  * big=x>y;
  * if(big)
  * {
  * x=y/x;
  * }
  * else
  * {
  * // compare x==y is necessary to support (+/-Inf, +/-Inf) cases
  * x = (x == y) ? 1.0f : x / y;
  * }
  * p = (x<0.5f) ? atanftbl1 : atanftbl2;
  * // approximate atan(x)/x-1
  * y = p[0].f;
  * y = x*y + p[1].f;
  * y = x*y + p[2].f;
  * y = x*y + p[3].f;
  * y = x*y + p[4].f;
  * y = x*y + p[5].f;
  * y = x*y + p[6].f;
  * y = x*y + p[7].f;
  * // convert result to true atan(x)
  * y = x*y + x;
  * 
  * if (!big) y = pi2f.f - y;
  * if (sx)   y = pif.f - y;
  * if (sy)   y = -y;
  * return   y;
  */
  const xtfloat * restrict POLY_TBL1;
  const xtfloat * restrict POLY_TBL2;

  POLY_TBL1 = (xtfloat*)atanftbl1;
  POLY_TBL2 = (xtfloat*)atanftbl2;

#if SCL_ATAN2F_ERRH != 0
  /* Floating-point Status and Control Register values. */
  uint32_t fstate;
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

  x1 = XT_ABS_S( x0 );
  y1 = XT_ABS_S( y0 );

#if SCL_ATAN2F_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_RUR_FSR();
#endif

  {
    /*
    * Part I, reduction to [0,pi/4]. Reference C code:
    *
    *   {
    *     float32_t x0, y0, p0;
    *
    *     for ( n=0; n<blkLen; n++ )
    *     {
    *       y0 = fabsf( y[blkIx*blkSize+n] );
    *       x0 = fabsf( x[blkIx*blkSize+n] );
    *
    *       // The actual result depends on input signs.
    *       if ( x0==0.f && y0==0.f ) { x0 = 1.f; y0 = 0.f; };
    *
    *       if ( x0>y0 ) p0 = y0/x0;
    *       // Special case of x==y is necessary to support (+/-Inf, +/-Inf) cases.
    *       else p0 = ( x0==y0 ? 1.f : x0/y0 );
    *
    *       scr[n] = p0;
    *     }
    *   }
    */

    {
      static const uint32_t TAB[4] = { 0x7fc00000, 0x00800000,
        0x4b000000, 0x7f800000
      };
      pT = (xtfloat *)TAB;
	  
	  {
        /* Reproduce NaN in both x and y to ensure NaN propagation. */
        b_nan = XT_UN_S(x0, y0);
        i0 = pT[0];
        
        XT_MOVT_S(x0, i0, b_nan);

        /* num <= den */
		b_nan = !b_nan;
		// MIN
		b_min_y0_x0 = XT_OLT_S(y1, x1);
		b_min_y0_x0 = XT_ANDB(b_min_y0_x0, b_nan);
		num = x1;
		XT_MOVT_S(num, y1, b_min_y0_x0);
		// MAX
		b_min_y0_x0 = XT_OLT_S(x1, y1);
		b_min_y0_x0 = XT_ANDB(b_min_y0_x0, b_nan);
		den = x1;
		XT_MOVT_S(den, y1, b_min_y0_x0);

        /* Classify numerator and denominator. */
        i0 = pT[1];
        b_subn = XT_OLT_S(num, i0);
        
        /* Scale up numerator and denominator if BOTH are subnormal. */
        i0 = pT[2];
        s = XT_MUL_S(num, i0); XT_MOVT_S(num, s, b_subn);
        s = XT_MUL_S(den, i0); XT_MOVT_S(den, s, b_subn);

        /* Initial appromimation for 1/den. */
        rcp = XT_RECIP0_S(den);
        /* Newton-Raphson iteration for 1/den. */
        eps = XT_CONST_S(1);
        XT_MSUB_S(eps, rcp, den);
        XT_MADD_S(rcp, rcp, eps);
        /* Approximation for the quotient num/den. */
        quo = XT_MUL_S(num, rcp);
        /* Refine the quotient by a modified Newton-Raphson iteration. */
        eps = num;
        XT_MSUB_S(eps, quo, den);
        XT_MADD_S(quo, rcp, eps);

        i0 = pT[3];
        b_num_inf = XT_OEQ_S(num, i0); /* Inf/Inf! */
        b_den_inf = XT_OEQ_S(den, i0);
        b_eqz = XT_OEQ_S(den, XT_CONST_S(0)); /* 0/0! */
        b_eqz_or_den_inf = XT_ORB(b_eqz, b_den_inf);

        XT_MOVT_S(quo, XT_CONST_S(0), b_eqz_or_den_inf);     /* 0/0 -> 0 or x/Inf -> 0*/
        XT_MOVT_S(quo, XT_CONST_S(1), b_num_inf); /* Inf/Inf -> 1 */
	  }
    }

    /*
    * Part II, polynomial approximation and full quadrant restoration.
    * Reference C code:
    *
    *   {
    *     const union ufloat32uint32 * ptbl;
    *     float32_t x0, y0, z0, p0;
    *     int sx, sy;
    *
    *     for ( n=0; n<blkLen; n++ )
    *     {
    *       y0 = y[blkIx*blkSize+n];
    *       x0 = x[blkIx*blkSize+n];
    *       p0 = scr[n];
    *
    *       sy = takesignf( y0 ); y0 = fabsf( y0 );
    *       sx = takesignf( x0 ); x0 = fabsf( x0 );
    *
    *       ptbl = ( p0<0.5f ? atanftbl1 : atanftbl2 );
    *
    *       // Approximate atan(p)/p-1
    *       z0 = ptbl[0].f;
    *       z0 = ptbl[1].f + p0*z0;
    *       z0 = ptbl[2].f + p0*z0;
    *       z0 = ptbl[3].f + p0*z0;
    *       z0 = ptbl[4].f + p0*z0;
    *       z0 = ptbl[5].f + p0*z0;
    *       z0 = ptbl[6].f + p0*z0;
    *       z0 = ptbl[7].f + p0*z0;
    *       z0 =        p0 + p0*z0;
    *
    *       if ( x0<y0 ) z0 = pi2f.f - z0;
    *       if ( sx    ) z0 = pif.f - z0;
    *       if ( sy    ) z0 = -z0;
    *
    *       z[blkIx*blkSize+n] = z0;
    *     }
    *   }
    */
    {
      /* Temporary; input values' sign */
      int32_t sx, sy;

      /* pi/2, pi */
      static const uint32_t TAB[2] = { 0x3fc90fdb, 0x40490fdb };
      pT = (xtfloat  *)TAB;
      {
        b_xlty = XT_OLT_S(x1, y1);
		p0 = quo;
		b_lt05 = XT_OLT_S(p0, XT_CONST_S(3));

        /*Reload polynomial coeff set #2. */

        cf1_0 = XT_LSI(POLY_TBL1, 0 * sz_f32);
        cf2_0 = XT_LSI(POLY_TBL2, 0 * sz_f32);
        cf1_1 = XT_LSI(POLY_TBL1, 1 * sz_f32);
        cf2_1 = XT_LSI(POLY_TBL2, 1 * sz_f32);
        cf1_2 = XT_LSI(POLY_TBL1, 2 * sz_f32);
        cf2_2 = XT_LSI(POLY_TBL2, 2 * sz_f32);
        cf1_3 = XT_LSI(POLY_TBL1, 3 * sz_f32);
        cf2_3 = XT_LSI(POLY_TBL2, 3 * sz_f32);
        cf1_4 = XT_LSI(POLY_TBL1, 4 * sz_f32);
        cf2_4 = XT_LSI(POLY_TBL2, 4 * sz_f32);
        cf1_5 = XT_LSI(POLY_TBL1, 5 * sz_f32);
        cf2_5 = XT_LSI(POLY_TBL2, 5 * sz_f32);
        cf1_6 = XT_LSI(POLY_TBL1, 6 * sz_f32);
        cf2_6 = XT_LSI(POLY_TBL2, 6 * sz_f32);
        cf1_7 = XT_LSI(POLY_TBL1, 7 * sz_f32);
        cf2_7 = XT_LSI(POLY_TBL2, 7 * sz_f32);

        /* Select coeffs from sets #1, #2 by reducted value's magnitude. */
        {
          xtfloat p0, p1;
          p0 = cf1_0;
          p1 = cf2_0;
          XT_MOVF_S(p0, p1, b_lt05); cf0 = p0;
          p0 = cf1_1;
          p1 = cf2_1;
          XT_MOVF_S(p0, p1, b_lt05); cf1 = p0;
          p0 = cf1_2;
          p1 = cf2_2;
          XT_MOVF_S(p0, p1, b_lt05); cf2 = p0;
          p0 = cf1_3;
          p1 = cf2_3;
          XT_MOVF_S(p0, p1, b_lt05); cf3 = p0;
          p0 = cf1_4;
          p1 = cf2_4;
          XT_MOVF_S(p0, p1, b_lt05); cf4 = p0;
          p0 = cf1_5;
          p1 = cf2_5;
          XT_MOVF_S(p0, p1, b_lt05); cf5 = p0;
          p0 = cf1_6;
          p1 = cf2_6;
          XT_MOVF_S(p0, p1, b_lt05); cf6 = p0;
          p0 = cf1_7;
          p1 = cf2_7;
          XT_MOVF_S(p0, p1, b_lt05); cf7 = p0;
        }

        /* Compute the approximation to z(p) = tan(p)/p-1. We use
        * Horner's method for better pipelining of a few iterations. */
        z0 = cf0;
        XT_MADD_S(cf1, p0, z0); z0 = cf1;
        XT_MADD_S(cf2, p0, z0); z0 = cf2;
        XT_MADD_S(cf3, p0, z0); z0 = cf3;
        XT_MADD_S(cf4, p0, z0); z0 = cf4;
        XT_MADD_S(cf5, p0, z0); z0 = cf5;
        XT_MADD_S(cf6, p0, z0); z0 = cf6;
        XT_MADD_S(cf7, p0, z0); z0 = cf7;

        XT_MADD_S( p0, p0, z0); z0 = p0;

        /* Keep signs of x and y. */
		sx = XT_RFR(x0);
		sy = XT_RFR(y0);
        sy = XT_AND(sy, 0x80000000);

		/* if ( x0<y0 ) z0 = pi2f.f - z0; */
        i0 = XT_LSI(pT, 0);
        z1 = XT_SUB_S(i0, z0); XT_MOVT_S(z0, z1, b_xlty);
        /* if ( sx ) z0 = pif.f - z0; */
        i0 = XT_LSI(pT, sz_f32);
        z1 = XT_SUB_S(i0, z0); XT_MOVLTZ_S(z0, z1, sx);
        /* if ( sy ) z0 = -z0; */
        XT_MOVNEZ_S(z0, XT_NEG_S(z0), sy);

#if SCL_ATAN2F_ERRH != 0
        /* Suppress spurious exception flags and restore original status flags. */
        XT_WUR_FSR(fstate);
        /* Assign EDOM to errno whenever x==0 and y==0. */
        if (b_eqz) { __Pragma("frequency_hint never"); errno = EDOM; }
#endif

		return z0;
      }
    }
  }
} /* scl_atan2f() */
#endif
