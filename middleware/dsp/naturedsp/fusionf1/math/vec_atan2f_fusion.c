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

#include "NatureDSP_Signal.h"
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, vec_atan2f, (float32_t * z, const float32_t * y, const float32_t * x, int N))
#else
/* Tables */
#include "pif_tbl.h"
#include "atanf_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

#define sz_f32    (int)sizeof(float32_t)
#define _MOVF_S(x,y,cond) x=__my_MOVF_S(&x,&y,cond)
inline_ xtfloatx2 __my_MOVF_S(const xtfloatx2* px, const xtfloatx2* py, xtbool cond)
{
  xtfloatx2 X = *(const xtfloatx2*)px;
  xtfloatx2 Y = *(const xtfloatx2*)py;
  ae_int64 y = AE_MOVINT64_FROMINT32X2(XT_AE_MOVINT32X2_FROMXTFLOATX2((xtfloatx2)Y));
  ae_int64 x = AE_MOVINT64_FROMINT32X2(XT_AE_MOVINT32X2_FROMXTFLOATX2((xtfloatx2)X));
  AE_MOVF64(x, y, cond);
  return (XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT64(x)));
}
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
void vec_atan2f (float32_t * z, const float32_t * y, const float32_t * x,  int N )
{
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
  const xtfloat * restrict         X;
  const xtfloat * restrict         Y;
        int32_t * restrict Z;
  const xtfloat *  restrict        S_rd;
        xtfloat * restrict S_wr;
        const xtfloat    * restrict         POLY_TBL1;
        const xtfloat    *  restrict        POLY_TBL2;

  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx, blkNum, blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ / sz_f32;
  /* Allocate a fixed-size scratch area on the stack. */
  float32_t ALIGN(8) scr[blkSize];

  int n;

  if (N <= 0) return;

  NASSERT_ALIGN8(scr);

  /*
  * Data are processed in blocks of scratch area size. Further, the algorithm
  * implementation is splitted in order to feed the optimizing compiler with a
  * few loops of managable size.
  */

  blkNum = (N + blkSize - 1) / blkSize;
  POLY_TBL1 = (xtfloat*)atanftbl1;
  POLY_TBL2 = (xtfloat*)atanftbl2;
  for (blkIx = 0; blkIx<blkNum; blkIx++)
  {
    blkLen = XT_MIN(N - blkIx*blkSize, blkSize);

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
      /* Input values */
      xtfloat x0, y0, i0;
      /* Numerator; denominator; reciprocal; quotient */
      xtfloat num, den, rcp, quo;
      /* Auxiliary vars */
      xtfloat s, eps;
      /* Is NaN; Inf/Inf; x/Inf; 0/0; x and y are subnormal */
      xtbool b_nan, b_num_inf, b_den_inf, b_eqz, b_subn;
      const xtfloat   *          pT;

      X = (xtfloat*)((uintptr_t)x + blkIx*blkSize*sz_f32);
      Y = (xtfloat*)((uintptr_t)y + blkIx*blkSize*sz_f32);
      S_wr = (xtfloat*)scr;

      static const uint32_t TAB[4] = { 0x7fc00000, 0x00800000,
        0x4b000000, 0x7f800000
      };
      pT = (xtfloat  *)TAB;
      __Pragma("loop_count min=1");
      for (n = 0; n<blkLen; n++)
      {
        XT_LSIP(x0, X, sz_f32);
        XT_LSIP(y0, Y, sz_f32);

        /* Reproduce NaN in both x and y to ensure NaN propagation. */
        b_nan = XT_UN_S(x0, y0);
        i0 = pT[0];
        
        XT_MOVT_S(x0, i0, b_nan);

        x0 = XT_ABS_S(x0);
        y0 = XT_ABS_S(y0);

        /* num <= den */
        num = XT_MIN_S(y0, x0);
        den = XT_MAX_S(y0, x0);

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
        b_eqz = XT_ORB(b_eqz, b_den_inf);

        XT_MOVT_S(quo, XT_CONST_S(0), b_eqz);     /* 0/0 -> 0 or x/Inf -> 0*/
        XT_MOVT_S(quo, XT_CONST_S(1), b_num_inf); /* Inf/Inf -> 1 */

        XT_SSIP(quo, S_wr, sz_f32);
      }
    }
    __Pragma("no_reorder");

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
      const xtfloat   *          pT;
      /* Input values; output value; reducted input value*/
      xtfloat x0, y0, z0, z1, p0;
      /* Temporary; input values' sign */
      int32_t sx, sy;
      /* Polynomial coeffs for 0.f<=p<0.5f (#1) and 0.5f<=p<=1.f (#2). */
      xtfloat cf1_0, cf1_1, cf1_2, cf1_3, cf1_4, cf1_5, cf1_6, cf1_7;
      xtfloat cf2_0, cf2_1, cf2_2, cf2_3, cf2_4, cf2_5, cf2_6, cf2_7;
      /* Selected polynomial coeffs. */
      xtfloat cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7;
      /* x less than y; x is negative; num/den is less than 0.5f. */
      xtbool b_xlty, b_sx, b_lt05;

      X = (xtfloat*)((uintptr_t)x + blkIx*blkSize*sz_f32);
      Y = (xtfloat*)((uintptr_t)y + blkIx*blkSize*sz_f32);
      Z = (int32_t*)((uintptr_t)z + blkIx*blkSize*sz_f32);

      S_rd = (xtfloat*)scr;
      /* pi/2, pi */
      static const uint32_t TAB[2] = { 0x3fc90fdb, 0x40490fdb
      };
      pT = (xtfloat  *)TAB;
      cf1_0 = XT_LSI(POLY_TBL1, 0 * sz_f32);
      cf2_0 = XT_LSI(POLY_TBL2, 0 * sz_f32);
      __Pragma("loop_count min=1");
      for (n = 0; n<blkLen; n++)
      {
        xtfloat i0;
        XT_LSIP(x0, X, 0*sz_f32);
        XT_LSIP(y0, Y, 0*sz_f32);

        x0 = XT_ABS_S(x0);
        y0 = XT_ABS_S(y0);
        b_xlty = XT_OLT_S(x0, y0);

        XT_LSIP(p0, S_rd, sz_f32);

        b_lt05 = XT_OLT_S(p0, XT_CONST_S(3));

        /*Reload polynomial coeff set #2. */

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
          xtfloatx2 p0, p1;
          p0 = (xtfloatx2)cf1_0;
          p1 = (xtfloatx2)cf2_0;
          _MOVF_S(p0, p1, b_lt05); cf0 = XT_LOW_S(p0);
          p0 = (xtfloatx2)cf1_1;
          p1 = (xtfloatx2)cf2_1;
          _MOVF_S(p0, p1, b_lt05); cf1 = XT_LOW_S(p0);
          p0 = (xtfloatx2)cf1_2;
          p1 = (xtfloatx2)cf2_2;
          _MOVF_S(p0, p1, b_lt05); cf2 = XT_LOW_S(p0);
          p0 = (xtfloatx2)cf1_3;
          p1 = (xtfloatx2)cf2_3;
          _MOVF_S(p0, p1, b_lt05); cf3 = XT_LOW_S(p0);
          p0 = (xtfloatx2)cf1_4;
          p1 = (xtfloatx2)cf2_4;
          _MOVF_S(p0, p1, b_lt05); cf4 = XT_LOW_S(p0);
          p0 = (xtfloatx2)cf1_5;
          p1 = (xtfloatx2)cf2_5;
          _MOVF_S(p0, p1, b_lt05); cf5 = XT_LOW_S(p0);
          p0 = (xtfloatx2)cf1_6;
          p1 = (xtfloatx2)cf2_6;
          _MOVF_S(p0, p1, b_lt05); cf6 = XT_LOW_S(p0);
          p0 = (xtfloatx2)cf1_7;
          p1 = (xtfloatx2)cf2_7;
          _MOVF_S(p0, p1, b_lt05); cf7 = XT_LOW_S(p0);
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
        sx = (int32_t)((int *)X)[0]; X++;
        sy = (int32_t)((int *)Y)[0]; Y++;
        
        sy = sy & 0x80000000;

        b_sx = AE_INT64_LT(AE_MOVINT64_FROMINT32(sx), AE_ZERO64());

        /* if ( x0<y0 ) z0 = pi2f.f - z0; */
        XT_LSIP(i0, pT, sz_f32);
        z1 = XT_SUB_S(i0, z0); XT_MOVT_S(z0, z1, b_xlty);
        /* if ( sx ) z0 = pif.f - z0; */
        XT_LSIP(i0, pT, -sz_f32);
        z1 = XT_SUB_S(i0, z0); XT_MOVT_S(z0, z1, b_sx);
        /* if ( sy ) z0 = -z0; */
        sx = XT_RFR(z0);
        sx = XT_XOR(sx, sy);
        *Z++ = sx;
        cf1_0 = XT_LSI(POLY_TBL1, 0 * sz_f32);
        cf2_0 = XT_LSI(POLY_TBL2, 0 * sz_f32);
      }
    }
  }
} /* vec_atan2f() */
#endif
