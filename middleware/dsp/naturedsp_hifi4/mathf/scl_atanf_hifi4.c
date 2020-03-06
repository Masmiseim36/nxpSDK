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
#include "pif_tbl.h"
#include "atanf_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(float32_t,scl_atanf,( float32_t x ))
#elif HAVE_VFPU
/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_ATANF_ERRH     1

/*===========================================================================
  Scalar matematics:
  scl_atan          Arctangent        
===========================================================================*/

/*-------------------------------------------------------------------------
  Arctangent 
  Functions calculate arctangent of number. Fixed point functions 
  scale output to pi so it is always in range -0x20000000 : 0x20000000 
  which corresponds to the real phases +pi/4 and represent input and output 
  in Q31
  NOTE:
  1.  Scalar floating point function is compatible with standard ANSI C
      routines and sets errno and exception flags accordingly

  Accuracy:
  24 bit version: 74000 (3.4e-5) 
  32 bit version: 42    (2.0e-8)
  floating point: 2 ULP

  Precision: 
  32x32  32-bit inputs, 32-bit output.
  f      floating point

  Input:
  x[N]   input data, Q31 or floating point
  N      length of vectors
  Output:
  z[N]   result, Q31 or floating point

  Restriction:
  x,z should not overlap

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  x,z - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result, Q31 or floating point
-------------------------------------------------------------------------*/

float32_t scl_atanf( float32_t x )
{
  /*
   * Reference C code:
   *
   *   float32_t y;
   *   int sx,big;
   *   const union ufloat32uint32* p;
   *
   *   // check for NaN on input
   *   if ( isnan(x) ) 
   *   { 
   *   #if SCL_ATANF_ERRH != 0           
   *       errno = EDOM; 
   *   #endif           
   *       return (qNaNf.f); 
   *   }
   *   // range reduction
   *   sx=x<0;
   *   x=sx?-x:x;
   *   big=x>1.0f;
   *   if (big) x=1.0f/x;
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
   *   if (big) y=pi2f.f-y;
   *   y=sx?-y:y; // apply original sign
   *   return y;
   */

  /* Polynomial coeff table pointer. */
  xtfloat * ptbl;
  /* Polynomial coeffs. */
  xtfloatx2 cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7;
  /* Input value and it 2nd power; output value */
  xtfloatx2 x0, x1, y0, y1;
  /* Integer representation of output value; input sign */
  ae_int32x2 y0_i, sx;
  /* Is >=0.5; is >1; is infinite */
  xtbool2 b_ge05, b_gt1, b_inf;

#if SCL_ATANF_ERRH != 0
  /* Floating-point Status and Control Register values. */
  ae_int64 fstate;
#endif

  x0 = (xtfloatx2)x;

  /* Check for NaN on input */
  if ( xtbool2_extract_0( XT_UN_SX2( x0, x0 ) ) ) 
  { 
    __Pragma( "frequency_hint never" );
#if SCL_ATANF_ERRH != 0
    errno = EDOM;
#endif
    return (qNaNf.f);
  }

  /* Extract the input sign. */
  sx = XT_AE_MOVINT32X2_FROMXTFLOATX2( x0 );
  sx = AE_SRLI32( sx, 31 );
  sx = AE_SLLI32( sx, 31 );

  x0 = XT_ABS_SX2( x0 );
  
  /*
   * Range reduction:
   *   a) x<=1.0: x <- x
   *   b) 1.0<x : x <- 1/x
   */

  b_gt1 = XT_OLT_SX2( (xtfloatx2)1.0f, x0 );
  b_inf = XT_OLE_SX2( plusInff.f, x0 );

#if SCL_ATANF_ERRH != 0
  /* Sample floating-point exception flags. */
  fstate = XT_AE_MOVVFCRFSR();
#endif

  if ( xtbool2_extract_0( b_gt1 ) )
  {
    x0 = XT_RECIP_SX2( x0 );
    /* Force a conventional result of zero for 1/Inf. */
    XT_MOVT_SX2( x0, (xtfloatx2)0.0f, b_inf );
  }

  /*
   * Select the polynomial and load coeffs.
   */

  b_ge05 = XT_OLE_SX2( (xtfloatx2)0.5f, x0 );

  ptbl = (xtfloat*)( xtbool2_extract_0( b_ge05 ) ? atanftbl2 : atanftbl1 );

  cf0 = ptbl[0]; cf1 = ptbl[1]; cf2 = ptbl[2]; cf3 = ptbl[3];
  cf4 = ptbl[4]; cf5 = ptbl[5]; cf6 = ptbl[6]; cf7 = ptbl[7];

  /*
   * Approximate p(x) = atan(x)/x-1. The polynomial is evaluated by Estrin's
   * rule, as this shortens the dependency path at the cost of a few additional
   * multiplications.
   */

  XT_MADD_SX2( cf1, cf0, x0 ); cf0 = cf1;
  XT_MADD_SX2( cf3, cf2, x0 ); cf1 = cf3;
  XT_MADD_SX2( cf5, cf4, x0 ); cf2 = cf5;
  XT_MADD_SX2( cf7, cf6, x0 ); cf3 = cf7;

  x1 = XT_MUL_SX2( x0, x0 ); /* x^2 */

  XT_MADD_SX2( cf1, x1, cf0 ); cf0 = cf1;
  XT_MADD_SX2( cf2, x1, cf0 ); cf0 = cf2;
  XT_MADD_SX2( cf3, x1, cf0 ); cf0 = cf3;

  /* atan(x) = p*x+x */
  XT_MADD_SX2( x0, x0, cf0 ); y0 = x0;

  /* Account for the range reduction. */
  y1 = XT_SUB_SX2( pi2f.f, y0 );
  XT_MOVT_SX2( y0, y1, b_gt1 );

#if SCL_ATANF_ERRH != 0
  /* Suppress spurious exception flags and restore original status flags. */
  XT_AE_MOVFCRFSRV( fstate );
#endif

  /* Propagate the input sign. */
  y0_i = XT_AE_MOVINT32X2_FROMXTFLOATX2( y0 );
  y0_i = AE_OR32( y0_i, sx );
  y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2( y0_i );

  return ( XT_HIGH_S( y0 ) );

} /* scl_atanf() */
#elif HAVE_FPU
/* If non-zero, set errno and raise floating-point exceptions on errors. */
#define SCL_ATANF_ERRH    1
#define sz_f32    (int)sizeof(float32_t)

/*===========================================================================
  Scalar matematics:
  scl_atan          Arctangent        
===========================================================================*/

/*-------------------------------------------------------------------------
  Arctangent 
  Functions calculate arctangent of number. Fixed point functions 
  scale output to pi so it is always in range -0x20000000 : 0x20000000 
  which corresponds to the real phases +pi/4 and represent input and output 
  in Q31
  NOTE:
  1.  Scalar floating point function is compatible with standard ANSI C
      routines and sets errno and exception flags accordingly

  Accuracy:
  32 bit version: 42    (2.0e-8)
  floating point: 2 ULP

  Precision: 
  32x32  32-bit inputs, 32-bit output.
  f      floating point
 
  Input:
  x[N]   input data, Q31 or floating point
  N      length of vectors
  Output:
  z[N]   result, Q31 or floating point

  Restriction:
  x,z should not overlap

  Scalar versions:
  ----------------
  return result, Q31 or floating point
-------------------------------------------------------------------------*/

// Taken from Fusion
// Using vector from Fusion
//  because scalar from Fusion applies another way of approximation
float32_t scl_atanf(float32_t x)
{
  /* Polynomial coeffs. */
  xtfloat cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7;
  /* Input value and it 2nd and 4th powers; output value */
  xtfloat x0, x1, y0, z0, z1;
  /* Is >=0.5; is >1; is infinite */
  xtbool b_gt1, b_inf, b_lt05;

#if SCL_ATANF_ERRH != 0
  /* Floating-point Status and Control Register values. */
  uint32_t fstate;
#endif

  x0 = x;

  /* Check for NaN on input */
  if ( XT_UN_S( x0, x0 ) )
  { 
    __Pragma( "frequency_hint never" );
#if SCL_ATANF_ERRH != 0
    errno = EDOM;
#endif
    return (qNaNf.f);
  }

  x1 = XT_ABS_S(x0);

/*
  * loat32_t y;
  * nt sx,big;
  * onst union ufloat32uint32* p;
  * ange reduction
  * x = x<0;
  *  = sx ? -x : x;
  * ig = x>1.0f;
  * f (big) x = 1.0f / x;
  *  = (x<0.5f) ? atanftbl1 : atanftbl2;
  * pproximate atan(x)/x-1
  *  = p[0].f;
  *  = x*y + p[1].f;
  *  = x*y + p[2].f;
  *  = x*y + p[3].f;
  *  = x*y + p[4].f;
  *  = x*y + p[5].f;
  *  = x*y + p[6].f;
  *  = x*y + p[7].f;
  * onvert result to true atan(x)
  *  = x*y + x;
  * 
  * f (big) y = pi2f.f - y;
  *  = sx ? -y : y; apply original sign
  * eturn y;
  */
  const xtfloat *          POLY_TBL1;
  const xtfloat *          POLY_TBL2;

  /*
  * Data are processed in blocks of scratch area size. Further, the algorithm
  * implementation is splitted in order to feed the optimizing compiler with a
  * few loops of managable size.
  */

  POLY_TBL1 = (xtfloat *)atanftbl1;
  POLY_TBL2 = (xtfloat *)atanftbl2;
  {
    /*
    * Part I, range reduction. Reference C code:
    *
    *   {
    *     float32_t x0, y0;
    *
    *     for ( n=0; n<blkLen; n++ )
    *     {
    *       x0 = fabsf( x[blkIx*blkSize+n] );
    *       y0 = ( x0>1.f ? 1.f/x0 : x0 );
    *       scr[n] = y0;
    *     }
    *   }
    */

    {
      {
       b_inf = XT_OEQ_S(plusInff.f, x1);
       b_gt1 = XT_OLT_S(XT_CONST_S(1), x1);
  
#if SCL_ATANF_ERRH != 0
       /* Sample floating-point exception flags. */
       fstate = XT_RUR_FSR();
#endif

       y0 = x1;
	   XT_MOVT_S(y0, XT_RECIP_S(x1), b_gt1);
	   b_inf = XT_AND(b_gt1, b_inf);
       /* Force a conventional result of zero for 1/Inf. */
       XT_MOVT_S(y0, XT_CONST_S(0), b_inf);
      }
    }

	/*
    * Part II, polynomial approximation. Reference C code:
    *
    *   {
    *     const union ufloat32uint32 * ptbl;
    *     float32_t x0, y0, z0;
    *
    *     for ( n=0; n<blkLen; n++ )
    *     {
    *       x0 = x[blkIx*blkSize+n];
    *       y0 = scr[n];
    *
    *       ptbl = ( y0<0.5f ? atanftbl1 : atanftbl2 );
    *
    *       // Approximate atan(x)/x-1
    *       z0 = ptbl[0].f;
    *       z0 = ptbl[1].f + y0*z0;
    *       z0 = ptbl[2].f + y0*z0;
    *       z0 = ptbl[3].f + y0*z0;
    *       z0 = ptbl[4].f + y0*z0;
    *       z0 = ptbl[5].f + y0*z0;
    *       z0 = ptbl[6].f + y0*z0;
    *       z0 = ptbl[7].f + y0*z0;
    *       z0 =        y0 + y0*z0;
    *
    *       if ( fabsf(x0)>1.f ) z0 = pi2f.f - z0;
    *
    *       // Restore the input sign.
    *       z0 = setsignf( z0, takesignf(x0) );
    *
    *       z[blkIx*blkSize+n] = z0;
    *     }
    *   }
    */
    {
      /* Polynomial coeffs for 0.f<=y<0.5f (#1) and 0.5f<=y<=1.f (#2). */
      xtfloat cf1_0, cf1_1, cf1_2, cf1_3, cf1_4, cf1_5, cf1_6, cf1_7;
      xtfloat cf2_0, cf2_1, cf2_2, cf2_3, cf2_4, cf2_5, cf2_6, cf2_7;
      /* Input sign; integer representation of output value. */
      int32_t sx;

      {
        /* Load polynomial coeff set #2. */
        cf2_0 = XT_LSI(POLY_TBL2, 0 * sz_f32);
        cf2_1 = XT_LSI(POLY_TBL2, 1 * sz_f32);
        cf2_2 = XT_LSI(POLY_TBL2, 2 * sz_f32);
        cf2_3 = XT_LSI(POLY_TBL2, 3 * sz_f32);
        cf2_4 = XT_LSI(POLY_TBL2, 4 * sz_f32);
        cf2_5 = XT_LSI(POLY_TBL2, 5 * sz_f32);
        cf2_6 = XT_LSI(POLY_TBL2, 6 * sz_f32);
        cf2_7 = XT_LSI(POLY_TBL2, 7 * sz_f32);

        /* Extract the sign bit and take absolute value. */
        sx = XT_RFR(x0);
        sx = sx & 0x80000000;
        b_lt05 = XT_OLT_S(y0, XT_CONST_S(3));
      
        /* Load polynomial coeff set #1. */
        cf1_0 = XT_LSI(POLY_TBL1, 0 * sz_f32);
        cf1_1 = XT_LSI(POLY_TBL1, 1 * sz_f32);
        cf1_2 = XT_LSI(POLY_TBL1, 2 * sz_f32);
        cf1_3 = XT_LSI(POLY_TBL1, 3 * sz_f32);
        cf1_4 = XT_LSI(POLY_TBL1, 4 * sz_f32);
        cf1_5 = XT_LSI(POLY_TBL1, 5 * sz_f32);
        cf1_6 = XT_LSI(POLY_TBL1, 6 * sz_f32);
        cf1_7 = XT_LSI(POLY_TBL1, 7 * sz_f32);
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
        /* Compute the approximation to z(y) = tan(y)/y-1. We use
        * Horner's method for better pipelining of a few iterations. */
        z0 = cf0;
        XT_MADD_S(cf1, y0, z0); z0 = cf1;
        XT_MADD_S(cf2, y0, z0); z0 = cf2;
        XT_MADD_S(cf3, y0, z0); z0 = cf3;
        XT_MADD_S(cf4, y0, z0); z0 = cf4;
        XT_MADD_S(cf5, y0, z0); z0 = cf5;
        XT_MADD_S(cf6, y0, z0); z0 = cf6;
        XT_MADD_S(cf7, y0, z0); z0 = cf7;
        XT_MADD_S( y0, y0, z0); z0 = y0;
      
        /* Account for the range reduction. */
        b_gt1 = XT_OLT_S(XT_CONST_S(1), x1);
        z1 = XT_SUB_S(pi2f.f, z0);
        XT_MOVT_S(z0, z1, b_gt1);
        /* Propagate the input sign. */
        XT_MOVNEZ_S(z0, XT_NEG_S(z0), sx);
      
#if SCL_ATANF_ERRH != 0
        /* Suppress spurious exception flags and restore original status flags. */
        XT_WUR_FSR(fstate);
#endif

		return z0;
      }
    }
  }
} /* scl_atanf() */
#endif
