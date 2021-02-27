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
DISCARD_FUN(void, vec_atanf, (    float32_t * restrict z, 
              const float32_t * restrict x, 
              int N ))
#else
/* Tables */
#include "pif_tbl.h"
#include "atanf_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"

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
  Vector matematics:
  vec_tan          Arctangent        
===========================================================================*/
/*-------------------------------------------------------------------------
  Arctangent 
  Functions calculate arctangent of number. Fixed point functions scale 
  output by pi which corresponds to the real phases +pi/4 and represent 
  input and output in Q15 or Q31
  NOTE:
  1.  Scalar floating point function is compatible with standard ANSI C
      routines and sets errno and exception flags accordingly

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: 2 LSB
  24x24  24-bit inputs, 24-bit output. Accuracy: 74000 (3.4e-5)
  32x32  32-bit inputs, 32-bit output. Accuracy: 42    (2.0e-8)
  f      floating point. Accuracy: 2 ULP


  Input:
  x[N]   input data, Q15, Q31 or floating point
  N      length of vectors
  Output:
  z[N]   result, Q15, Q31 or floating point

  Restriction:
  x,z should not overlap

  Scalar versions:
  ----------------
  return result, Q15, Q31 or floating point
-------------------------------------------------------------------------*/
void vec_atanf (    float32_t * restrict z, 
              const float32_t * restrict x, 
              int N )
{
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
  const xtfloat *          X;
        int32_t * restrict Z;
  const xtfloat *          S_rd;
        xtfloat * restrict S_wr;
  const xtfloat    *          POLY_TBL1;
  const xtfloat    *          POLY_TBL2;
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
      /* Input value; reducted value. */
      xtfloat x0, y0;
      /* Is greater than one; is a +/-infinity  */
      xtbool b_gt1, b_inf;

      X = (xtfloat*)((uintptr_t)x + blkIx*blkSize*sz_f32);
      S_wr = (xtfloat*)scr;
      __Pragma("loop_count min=1");
      for (n = 0; n<(blkLen ); n++)
      {
       XT_LSIP(x0, X, +4);
  
       x0 = XT_ABS_S(x0);
       b_inf = XT_OEQ_S(plusInff.f, x0);
       b_gt1 = XT_OLT_S(XT_CONST_S(1), x0);
  
       /* y <- 1.f/x */
       y0 = XT_RECIP_S(x0);
  
       /* Fast reciprocal refinement produces NaN for an infinity on input! */
       XT_MOVT_S(y0, XT_CONST_S(0), b_inf);
       /* Select reciprocal for x>1.f */
       XT_MOVF_S(y0, x0, b_gt1);
  
       XT_SSIP(y0, S_wr, sz_f32);
      }
    }
    __Pragma("no_reorder");

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
      /* Input value; reducted input value; output value. */
      xtfloat x0, y0, z0, z1;
      /* Polynomial coeffs for 0.f<=y<0.5f (#1) and 0.5f<=y<=1.f (#2). */
      xtfloat cf1_0, cf1_1, cf1_2, cf1_3, cf1_4, cf1_5, cf1_6, cf1_7;
      xtfloat cf2_0, cf2_1, cf2_2, cf2_3, cf2_4, cf2_5, cf2_6, cf2_7;
      /* Selected polynomial coeffs. */
      xtfloat cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7;
      /* Input sign; integer representation of output value. */
      int32_t sx, z0_i;
      /* Is greater than one; is less than 0.5f */
      xtbool b_gt1, b_lt05;

      X = (xtfloat*)((uintptr_t)x + blkIx*blkSize*sz_f32);
      Z = (int32_t*)((uintptr_t)z + blkIx*blkSize*sz_f32);
      S_rd = (xtfloat*)scr;

      /* Pre-load polynomial coeff set #1. */
      cf1_0 = XT_LSI(POLY_TBL1, 0 * sz_f32);
      cf1_1 = XT_LSI(POLY_TBL1, 1 * sz_f32);
      /* Pre-load first input value */
      XT_LSIP(x0, X, sz_f32);
      for (n = 0; n<blkLen ; n++)
      {
        /*Reload polynomial coeff set #2. */

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
        sx = sx&0x80000000;
        x0 = XT_ABS_S(x0);
      
        XT_LSIP(y0, S_rd, sz_f32);
      
        b_lt05 = XT_OLT_S(y0, XT_CONST_S(3));
      
        /* Reload coeff set #1 on each iteration. */

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
        b_gt1 = XT_OLT_S(XT_CONST_S(1), x0);
        z1 = XT_SUB_S(pi2f.f, z0);
        XT_MOVT_S(z0, z1, b_gt1);
      
        /* Propagate the input sign. */
        z0_i = XT_RFR(z0);
        z0_i = XT_OR(z0_i, sx);
        /* Load next input value */
        XT_LSIP(x0, X, sz_f32);
        /* Save output value */
        *Z++=z0_i;
      }
    }
  }
} /* vec_atanf() */
#endif
