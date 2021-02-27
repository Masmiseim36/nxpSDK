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

/* DSP Library API */
#include "NatureDSP_Signal.h"
/* Common helper macros. */
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, vec_lognf, (float32_t * restrict y, const float32_t * restrict x, int N))
#else
/* Tables */
#include "lognf_tbl.h"
#include "sqrt2f_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

#define sz_i32  (int)sizeof(int32_t)
#define sz_f32  (int)sizeof(float32_t)

#define _MOVT_S(x,y,cond) x=__my_MOVT_S(&x,&y,cond)

inline_ xtfloatx2 __my_MOVT_S(const xtfloatx2* px,const xtfloatx2* py,xtbool cond)
{
    xtfloatx2 X=*(const xtfloatx2*)px;
    xtfloatx2 Y=*(const xtfloatx2*)py;
    ae_int64 y=AE_MOVINT64_FROMINT32X2(XT_AE_MOVINT32X2_FROMXTFLOATX2((xtfloatx2)Y));
    ae_int64 x=AE_MOVINT64_FROMINT32X2(XT_AE_MOVINT32X2_FROMXTFLOATX2((xtfloatx2)X));
    AE_MOVT64(x,y,cond);
    return (XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT64(x)));
}


/*===========================================================================
  Vector matematics:
  vec_log              Logarithm 
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
void vec_lognf( float32_t * restrict y,const float32_t * restrict x, int N )
{
  /*
   * Reference C code for a scalar variant:
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

  const xtfloat  * restrict  X_rd;
  const xtfloat  * restrict  Y_rd;
        xtfloat  * restrict Y_wr;
  const ae_int32 *          SCR_rd;
        ae_int32 * restrict SCR_wr;
  const xtfloat  * restrict POLY_TBL;
  const xtfloat   *restrict  pT;
                                   /* 2^-126,       2^23, sqrt(0.5) */
  static const uint32_t TAB[] = { 0x00800000, 0x4b000000, 0x3f3504f3, 0x007FFFFF, 0x3F000000, 
      0x3f317218, 0x7fc00000, 0x7f800000,0xff800000 }; /* ln2, qNaN, plusInf, minusInf */

  /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx,blkNum,blkLen;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ/sz_i32;
  /* Allocate a fixed-size scratch area on the stack. */
  int32_t ALIGN(8) scr[blkSize];

  int n;

  if ( N<=0 ) return;

  NASSERT_ALIGN8( scr );

  /*
   * Data are processed in blocks of scratch area size. Further, the algorithm
   * implementation is splitted in order to feed the optimizing compiler with a
   * few loops of managable size.
   */

  POLY_TBL = (xtfloat*)lognf_tbl;

  blkNum = (N + blkSize-1)/blkSize;

  for (blkIx=0; blkIx<blkNum; blkIx++)
  {
    blkLen = XT_MIN(N-blkIx*blkSize,blkSize);

    /*
     * Part I, reference C code:
     *
     *   {
     *     float32_t fr;
     *     int ex;
     *
     *     for ( n=0; n<blkLen; n++ )
     *     {
     *       fr = frexpf( x[blkIx*blkSize+n], &ex );
     *       if ( fr < sqrt0_5f.f ) { fr *= 2.f; ex--; };
     *       y[blkIx*blkSize+n] = fr - 1.f;
     *       scr[n] = ex;
     *     }
     *   }
     */

    {
      /* Input value; fractional part */
      SCR_wr = (ae_int32*)scr;

      X_rd = (xtfloat*)((uintptr_t)x + blkIx*blkSize*sz_f32);
      Y_rd = (xtfloat*)((uintptr_t)y + blkIx*blkSize*sz_f32);
      Y_wr = (xtfloat*)((uintptr_t)y + blkIx*blkSize*sz_f32);
      pT = (xtfloat  *)TAB;

      {
          int32_t * pWr=(int32_t *)SCR_wr;
          xtfloat x0, x1, fr0, fr1, i0;
          int32_t  xn0, ex0, ex1, d0, d1;
          d0 = ((const int32_t*)pT)[3];
          d1 = ((const int32_t*)pT)[4];

          for (n = 0; n<(blkLen); n++)
          {
            /* Significand; exponential part */
            /* Is a subnormal; is less than 2^0.5  */
            xtbool b_subn, b_ltsqr;
            XT_LSIP(x0, X_rd, 4);

            /* Compare with smallest positive normal number 2^-126 */
            i0=pT[0]; b_subn = XT_OLT_S(x0, i0);
            /* Multiply subnormals by 2^23 */
            i0=pT[1]; x1 = XT_MUL_S(x0, i0);

            XT_MOVT_S(x0, x1, b_subn);
            xn0 = XT_RFR(x0);
            ex0 = xn0>>23;
            ex0 = XT_ADDI(ex0,-126);
            ex1 = XT_ADDI(ex0, -23);
            {
                unsigned int t=ex0;
                XT_MOVT(t, (unsigned) ex1, b_subn); ex0=t;
            }
            xn0 &= d0;
            xn0 |= d1;

            fr0 = XT_LOW_S(XT_AE_MOVXTFLOATX2_FROMINT32X2(int32_rtor_ae_int32x2(xn0)));
            fr1 = XT_MUL_S(fr0, XT_CONST_S(2));
            ex1 = XT_ADDI(ex0, -1);
            i0=pT[2]; b_ltsqr = XT_OLT_S(fr0,i0);
            XT_MOVT_S(fr0, fr1, b_ltsqr);
            {
                unsigned int t=ex0;
                XT_MOVT(t, ex1, b_ltsqr); ex0=t;
            }
            fr0 = XT_SUB_S(fr0, XT_CONST_S(1));
            XT_SSIP(fr0, Y_wr, sz_f32);
            *pWr++=ex0;
          }
      }
    }

    __Pragma("no_reorder");

    /*
     * Part II, reference C code:
     *
     *   {
     *     float32_t xn, yn, fr, fr2;
     *     float32_t gn, cf0, cf1, cf2, cf3;
     *   
     *     for (n=0; n<blkLen; n++)
     *     {
     *       xn = x[blkIx*blkSize+n];
     *   
     *            if ( isnan(xn)      ) yn = xn;
     *       else if ( xn<0.f         ) yn = qNaNf.f;
     *       else if ( xn==0.f        ) yn = minusInff.f;
     *       else if ( xn==plusInff.f ) yn = plusInff.f;
     *       else
     *       {
     *         fr = y[blkIx*blkSize+n];
     *   
     *         //                                                              
     *         // Use a combination of Estrin's method and Horner's scheme to  
     *         // evaluate the polynomial.                                     
     *         //                                                               
     *   
     *         cf0 = lognf_tbl[1].f - fr*lognf_tbl[0].f;
     *         cf1 = lognf_tbl[3].f - fr*lognf_tbl[2].f;
     *         cf2 = lognf_tbl[5].f - fr*lognf_tbl[4].f;
     *         cf3 = lognf_tbl[7].f - fr*lognf_tbl[6].f;
     *   
     *         fr2 = fr*fr;
     *   
     *         gn = cf0;
     *         gn = cf1 + fr2*gn;
     *         gn = cf2 + fr2*gn;
     *         gn = cf3 + fr2*gn;
     *         gn = fr  + fr2*gn;
     *         
     *         yn = gn + scr[n]*ln2.f;
     *   
     *       }
     *   
     *       y[blkIx*blkSize+n] = yn;
     *     }
     *   }
     */

    {
      /* Input value; output value; fractional part; squared fractional part */
      xtfloat x0,  fr, fr2;
      /* Exponential part */
      ae_int32 ex;
      /* Polynomial value; polynomial coefficients */
      xtfloat g, cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7;
      /* Is a NaN or is less than zero; is equal to zero; is positive infinity */
      xtbool b_ultz, b_eqz, b_inf;

      SCR_rd = (ae_int32*)scr;

      X_rd = (xtfloat*)( (uintptr_t)x + blkIx*blkSize*sz_f32 );
      Y_rd = (xtfloat*)( (uintptr_t)y + blkIx*blkSize*sz_f32 );
      Y_wr = (xtfloat*)( (uintptr_t)y + blkIx*blkSize*sz_f32 );

      pT=(const xtfloat*) (TAB+5);
      for ( n=0; n<(blkLen>>1); n++ )
      {
        xtfloatx2 y0;
        xtfloatx2 pt1,pt2,pt3;
        xtfloat t;

        XT_LSIP(fr, Y_rd, +sz_f32);

        /* Reload coefficients on each iteration. */
        //cf0 = XT_LSI(POLY_TBL, 0 * sz_f32);
        XT_LSXP(cf0, POLY_TBL, 0 * sz_f32);
        cf2 = XT_LSI(POLY_TBL, 2 * sz_f32);
        cf4 = XT_LSI(POLY_TBL, 4 * sz_f32);
        cf6 = XT_LSI(POLY_TBL, 6 * sz_f32);

        cf1 = XT_LSI(POLY_TBL, 1 * sz_f32);
        cf3 = XT_LSI(POLY_TBL, 3 * sz_f32);
        cf5 = XT_LSI(POLY_TBL, 5 * sz_f32);
        cf7 = XT_LSI(POLY_TBL, 7 * sz_f32);
        /*                                                              
         * Use a combination of Estrin's method and Horner's scheme to evaluate
         * the polynomial.                                     
         */

        XT_MSUB_S( cf1, cf0, fr );
        XT_MSUB_S( cf3, cf2, fr );
        XT_MSUB_S( cf5, cf4, fr );
        XT_MSUB_S( cf7, cf6, fr );

        fr2 = XT_MUL_S( fr, fr );

                                  g = cf1;
        XT_MADD_S( cf3, g, fr2 ); g = cf3;
        XT_MADD_S( cf5, g, fr2 ); g = cf5;
        XT_MADD_S( cf7, g, fr2 ); g = cf7;
        XT_MADD_S( fr , g, fr2 ); g = fr;

        ae_int32_loadip(ex, SCR_rd, sz_i32);

        XT_LSXP(t,pT,0);
        XT_MADD_S( g, XT_FLOAT_S( ex, 0 ), t ); y0 = (xtfloatx2)(g);

        /*
         * Reload input value and check it for special cases.
         */

        XT_LSIP(x0, X_rd, sz_f32);

        {
            pt1=XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT32(((const ae_int32*)pT)[1]));
            pt3=XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT32(((const ae_int32*)pT)[3]));
            pt2=XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT32(((const ae_int32*)pT)[2]));
            b_ultz = XT_ULT_S( x0, XT_CONST_S(0) );
            b_eqz  = XT_OEQ_S( x0, XT_CONST_S(0) );
            b_inf  = XT_OEQ_S( x0, pT[2] );
            _MOVT_S( y0, pt1, b_ultz );
            _MOVT_S( y0, pt3, b_eqz );
            _MOVT_S( y0, pt2, b_inf );
        }
        XT_SSIP(XT_LOW_S(y0), Y_wr, sz_f32);

        XT_LSIP(fr, Y_rd, +sz_f32);

        /* Reload coefficients on each iteration. */
        cf0 = XT_LSI(POLY_TBL, 0 * sz_f32);
        cf2 = XT_LSI(POLY_TBL, 2 * sz_f32);
        cf4 = XT_LSI(POLY_TBL, 4 * sz_f32);
        cf6 = XT_LSI(POLY_TBL, 6 * sz_f32);

        cf1 = XT_LSI(POLY_TBL, 1 * sz_f32);
        cf3 = XT_LSI(POLY_TBL, 3 * sz_f32);
        cf5 = XT_LSI(POLY_TBL, 5 * sz_f32);
        cf7 = XT_LSI(POLY_TBL, 7 * sz_f32);
        /*                                                              
         * Use a combination of Estrin's method and Horner's scheme to evaluate
         * the polynomial.                                     
         */
        XT_MSUB_S( cf1, cf0, fr );
        XT_MSUB_S( cf3, cf2, fr );
        XT_MSUB_S( cf5, cf4, fr );
        XT_MSUB_S( cf7, cf6, fr );

        fr2 = XT_MUL_S( fr, fr );

                                  g = cf1;
        XT_MADD_S( cf3, g, fr2 ); g = cf3;
        XT_MADD_S( cf5, g, fr2 ); g = cf5;
        XT_MADD_S( cf7, g, fr2 ); g = cf7;
        XT_MADD_S( fr , g, fr2 ); g = fr;

        ae_int32_loadip(ex, SCR_rd, sz_i32);
        t=pT[0];
        XT_MADD_S( g, XT_FLOAT_S( ex, 0 ), t ); y0 = (xtfloatx2)(g);

        /*
         * Reload input value and check it for special cases.
         */

        XT_LSIP(x0, X_rd, sz_f32);

        {
            pt1=XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT32(((const ae_int32*)pT)[1]));
            pt3=XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT32(((const ae_int32*)pT)[3]));
            pt2=XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT32(((const ae_int32*)pT)[2]));
            b_ultz = XT_ULT_S( x0, XT_CONST_S(0) );
            b_eqz  = XT_OEQ_S( x0, XT_CONST_S(0) );
            b_inf  = XT_OEQ_S( x0, pT[2] );
            _MOVT_S( y0, pt1, b_ultz );
            _MOVT_S( y0, pt3, b_eqz );
            _MOVT_S( y0, pt2, b_inf );
        }
        XT_SSIP(XT_LOW_S(y0), Y_wr, sz_f32);
      }
      if (blkLen&1)
      {
        xtfloatx2 y0;
        xtfloatx2 pt1, pt2, pt3;
        xtfloat t;

        XT_LSIP(fr, Y_rd, +sz_f32);

        /* Reload coefficients on each iteration. */
        XT_LSXP(cf0, POLY_TBL, 0 * sz_f32);
        cf2 = XT_LSI(POLY_TBL, 2 * sz_f32);
        cf4 = XT_LSI(POLY_TBL, 4 * sz_f32);
        cf6 = XT_LSI(POLY_TBL, 6 * sz_f32);

        cf1 = XT_LSI(POLY_TBL, 1 * sz_f32);
        cf3 = XT_LSI(POLY_TBL, 3 * sz_f32);
        cf5 = XT_LSI(POLY_TBL, 5 * sz_f32);
        cf7 = XT_LSI(POLY_TBL, 7 * sz_f32);
        /*
        * Use a combination of Estrin's method and Horner's scheme to evaluate
        * the polynomial.
        */

        XT_MSUB_S(cf1, cf0, fr);
        XT_MSUB_S(cf3, cf2, fr);
        XT_MSUB_S(cf5, cf4, fr);
        XT_MSUB_S(cf7, cf6, fr);

        fr2 = XT_MUL_S(fr, fr);

        g = cf1;
        XT_MADD_S(cf3, g, fr2); g = cf3;
        XT_MADD_S(cf5, g, fr2); g = cf5;
        XT_MADD_S(cf7, g, fr2); g = cf7;
        XT_MADD_S(fr, g, fr2); g = fr;

        ae_int32_loadip(ex, SCR_rd, sz_i32);

        XT_LSXP(t, pT, 0);
        XT_MADD_S(g, XT_FLOAT_S(ex, 0), t); 
        y0 = (xtfloatx2)(g);
        /*
        * Reload input value and check it for special cases.
        */
        
        XT_LSIP(x0, X_rd, sz_f32);

        {
          pt1 = XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT32(((const ae_int32*)pT)[1]));
          pt3 = XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT32(((const ae_int32*)pT)[3]));
          pt2 = XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT32(((const ae_int32*)pT)[2]));
          b_ultz = XT_ULT_S(x0, XT_CONST_S(0));
          b_eqz = XT_OEQ_S(x0, XT_CONST_S(0));
          b_inf = XT_OEQ_S(x0, pT[2]);
          _MOVT_S(y0, pt1, b_ultz);
          _MOVT_S(y0, pt3, b_eqz);
          _MOVT_S(y0, pt2, b_inf);
        }
        XT_SSIP(XT_LOW_S(y0), Y_wr, sz_f32);
      }
    }
  } 
} /* vec_lognf() */
#endif
