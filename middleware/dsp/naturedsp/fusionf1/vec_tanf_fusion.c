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
/* DSP Library API */
#include "NatureDSP_Signal.h"
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, vec_tanf,  ( 
                    float32_t *restrict z, 
              const float32_t *restrict x,
              int N))
#else
/* Value of 2/pi, 4/pi, etc. */
#include "inv2pif_tbl.h"
/* Angular argument reduction constants. */
#include "sinf_tbl.h"
/* tan/cotan approximation polynomial coeffs. */
#include "tanf_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

#define sz_f32    (int)sizeof(float32_t)
/*===========================================================================
  Vector matematics:
  vec_tan             Tangent    
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

void vec_tanf ( 
                    float32_t *restrict z, 
              const float32_t *restrict x,
              int N)
{
  /*
  * float32_t x2,y,yt,yc;
  * int sx,n,j,st;
  * sx=takesignf(x);
  * x=sx?-x:x;
  * if(x>tanf_maxval) return qNaN.f;
  * argument reduction
  * process reduces x by integral multiple of pi/4.
  * The output is deduced to the sum of two single precision
  * floating point values x+dx.
  * 
  * n = (int)STDLIB_MATH(ceilf)(x*inv4pif.f);
  * j = n&~1;
  * 
  * {
  * float32_t dx, t, y = x, jj = (float32_t)j;
  * const union ufloat32uint32 c[6] = {
  * { 0x3f4a0000 },
  * { 0xbb700000 },
  * { 0xb6160000 },
  * { 0x32080000 },
  * { 0x2e060000 },
  * { 0xa9b9ee5a } };
  * dx = 0.f;
  * y -= c[0].f*jj;
  * y -= c[1].f*jj;
  * y -= c[2].f*jj;
  * t = y; y -= c[3].f*jj; t = (t - y); t -= c[3].f*jj; dx = t;
  * t = y; y -= c[4].f*jj; t = (t - y); t -= c[4].f*jj; dx = (dx + t);
  * t = y; y -= c[5].f*jj; t = (t - y); t -= c[5].f*jj; dx = (dx + t);
  * y = (y + dx);
  * x = y;
  * }
  * 
  * compute tan via minmax polynomial
  * x2 = x*x;
  * yt = polytanf_tbl[0].f;
  * yt = yt*x2 + polytanf_tbl[1].f;
  * yt = yt*x2 + polytanf_tbl[2].f;
  * yt = yt*x2 + polytanf_tbl[3].f;
  * yt = yt*x2 + polytanf_tbl[4].f;
  * yt = yt*x2 + polytanf_tbl[5].f;
  * yt = yt*x2 + polytanf_tbl[6].f;
  * yt = yt*x2;
  * 
  * dx is small enough (<3e-8) and wiil be used to modify
  * tangent value computed by polynomial using derivation
  * tg(x+dx)=tg(x)+dx/cos(x)^2
  * for 2 ULP operation it is enough to suppose 1/cos(x)^2 ~ 1
  * for 1 ULP operation, it should be computed accurately
  * 
  * resulting value is decomposed as follows
  * tag(x+dx)~(P*x+dx)+x
  * The order of summation is crucial!
  * 
  * yt = (yt*x) + x;
  * yc = 1.f / yt;
  * 
  * adjust sign
  * n = (n >> 1) & 1;
  * st = sx ^ n;
  * select tan/cotan
  * y = n ? yc : yt;
  * apply the sign
  * y = changesignf(y, st);
  * return y;
  */

  const xtfloat * restrict  X;
        int32_t * restrict  Z;
  const xtfloat * restrict  S_rd;
        xtfloat * restrict  S_wr;
  const xtfloat * restrict  T;
  const int     * restrict  pT;

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

  for (blkIx = 0; blkIx<blkNum; blkIx++)
  {
    blkLen = XT_MIN(N - blkIx*blkSize, blkSize);
    /*
    * Part I, range reduction. Reference C code:
    *
    *   {
    *     float32_t xn, p, dp, t;
    *     int ji;
    *     float32_t jf;
    *
    *     static const union ufloat32uint32 c[6] = {
    *       { 0x3f4a0000 }, { 0xbb700000 },
    *       { 0xb6160000 }, { 0x32080000 },
    *       { 0x2e060000 }, { 0xa9b9ee5a }
    *     };
    *
    *     for ( n=0; n<blkLen; n++ )
    *     {
    *       xn = fabsf( x[blkIx*blkSize+n] );
    *
    *       // Determine the pi/2-wide segment the input value belongs to.
    *       ji = (int)ceilf( xn*inv4pif.f );
    *       jf = (float32_t)( ji & ~1 );
    *
    *       // Calculate the difference between the segment midpoint and input value.
    *       p = xn;
    *       p -= c[0].f*jf;
    *       p -= c[1].f*jf;
    *       p -= c[2].f*jf;
    *       t = p; p -= c[3].f*jf; t = t - p; t -= c[3].f*jf; dp = t;
    *       t = p; p -= c[4].f*jf; t = t - p; t -= c[4].f*jf; dp += t;
    *       t = p; p -= c[5].f*jf; t = t - p; t -= c[5].f*jf; dp += t;
    *       p += dp;
    *
    *       scr[n] = p;
    *     }
    *   }
    */
    {
      /* Input value; reducted input value; correction term. */
      xtfloat xn, p, dp;
      /* Auxiliary floating-point vars. */
      xtfloat t, r;
      /* Input value segment number. */
      ae_int32 ji, i0;
      xtfloat jf;
      /* pi/4 splitted into 7-bit chunks. */
      xtfloat c0, c1, c2, c3, c4, c5;

      static const uint32_t c[6] = {
        0x3f4a0000 ,  0xbb700000 ,
        0xb6160000 ,  0x32080000 ,
        0x2e060000 ,  0xa9b9ee5a 
      };
      /* 4/pi, ~1 */
      const uint32_t TAB[2] = { 0x3fa2f983, 0xFFFFFFFE };
      X = (xtfloat*)((uintptr_t)x + blkIx*blkSize*sz_f32);
      S_wr = (xtfloat*)scr;
      T = (xtfloat  *)c;

      pT = (int  *)TAB;
      for (n = 0; n<(blkLen); n++)
      {
        XT_LSIP(xn, X, sz_f32);
        /*
        * Determine the pi/2-wide segment the input value belongs to.
        */
        xn = XT_ABS_S(xn);
        XT_LSIP(c0, castxcc(xtfloat, pT), 0*sz_f32);
        t = XT_MUL_S(xn, c0);
        t = XT_FICEIL_S(t);
        ji = XT_TRUNC_S(t, 0);
        i0 = XT_L32I(pT, 1*sz_f32);
        ji = XT_AND(ji, i0);
        jf = XT_FLOAT_S(ji, 0);

        /*
        * Calculate the difference between the segment midpoint and input value.
        */

        XT_LSIP(c0, T, +3 * sz_f32);
        c1 = XT_LSI(T, -2 * sz_f32);
        c2 = XT_LSI(T, -1 * sz_f32);
        c4 = XT_LSI(T, +1 * sz_f32);
        c5 = XT_LSI(T, +2 * sz_f32);
        XT_LSXP(c3, T, -3 * sz_f32);


        p = xn;
        XT_MSUB_S(p, jf, c0);
        XT_MSUB_S(p, jf, c1);
        XT_MSUB_S(p, jf, c2);

        r = XT_MUL_S(jf, c3); t = p; p = XT_SUB_S(p, r); t = XT_SUB_S(t, p); t = XT_SUB_S(t, r); dp = t;
        r = XT_MUL_S(jf, c4); t = p; p = XT_SUB_S(p, r); t = XT_SUB_S(t, p); t = XT_SUB_S(t, r); dp = XT_ADD_S(t, dp);
        r = XT_MUL_S(jf, c5); t = p; p = XT_SUB_S(p, r); t = XT_SUB_S(t, p); t = XT_SUB_S(t, r); dp = XT_ADD_S(t, dp);

        p = XT_ADD_S(p, dp);

        
        XT_SSIP(p, S_wr, sz_f32);
      }
    }
    __Pragma("no_reorder");

    /*
    * Part II, tangent approximation via minmax polynomial. Reference C code:
    *
    *   {
    *     float32_t yt, p, p2;
    *
    *     for ( n=0; n<blkLen; n++ )
    *     {
    *       p = scr[n];
    *       p2 = p*p;
    *
    *       yt = polytanf_tbl[0].f;
    *       yt = polytanf_tbl[1].f + yt*p2;
    *       yt = polytanf_tbl[2].f + yt*p2;
    *       yt = polytanf_tbl[3].f + yt*p2;
    *       yt = polytanf_tbl[4].f + yt*p2;
    *       yt = polytanf_tbl[5].f + yt*p2;
    *       yt = polytanf_tbl[6].f + yt*p2;
    *       yt =                     yt*p2;
    *
    *       scr[n] = yt*p + p;
    *     }
    *   }
    */
    {
      /* Reducted input value and its 2nd power; tangent approximation. */
      xtfloat p, p2, yt;
      /* Polynomial coeffs. */
      xtfloat cf0, cf1, cf2, cf3, cf4, cf5, cf6;

      S_rd = (xtfloat*)scr;
      S_wr = (xtfloat*)scr;
      T = (xtfloat  *)polytanf_tbl;
      /* Pre-load polynomial coeffs. */
      cf0 = XT_LSI(T, 0 * sz_f32);
      cf1 = XT_LSI(T, 1 * sz_f32);
      __Pragma("loop_count min=1");
      for (n = 0; n<(blkLen); n++)
      {
        XT_LSIP(p, S_rd, sz_f32);
        /* Reload polynomial coeffs. */
        cf0 = XT_LSI(T, 0 * sz_f32);
        cf1 = XT_LSI(T, 1 * sz_f32);
        cf2 = XT_LSI(T, 2 * sz_f32);
        cf3 = XT_LSI(T, 3 * sz_f32);
        cf4 = XT_LSI(T, 4 * sz_f32);
        cf5 = XT_LSI(T, 5 * sz_f32);
        cf6 = XT_LSI(T, 6 * sz_f32);

        p2 = XT_MUL_S(p, p);

        yt = cf0;
        XT_MADD_S(cf1, p2, yt); yt = cf1;
        XT_MADD_S(cf2, p2, yt); yt = cf2;
        XT_MADD_S(cf3, p2, yt); yt = cf3;
        XT_MADD_S(cf4, p2, yt); yt = cf4;
        XT_MADD_S(cf5, p2, yt); yt = cf5;
        XT_MADD_S(cf6, p2, yt); yt = cf6;
        yt = XT_MUL_S(p2, yt);
        p = XT_LSI(S_rd, -sz_f32);
        XT_MADD_S(p, p, yt); yt = p;
       
        XT_SSIP(yt, S_wr, sz_f32);
        
      }
    }
    __Pragma("no_reorder");

    /*
    * Part III, estimation of cotangent and finalization of results. Reference C code:
    *
    *   {
    *     float32_t xn, xm, yt, yc, zn;
    *     int ji, sx, sz;
    *
    *     for ( n=0; n<blkLen; n++ )
    *     {
    *       xn = x[blkIx*blkSize+n];
    *       yt = scr[n];
    *
    *       xm = fabsf(xn);
    *       ji = (int)ceilf( xm*inv4pif.f );
    *       ji = (ji>>1) & 1;
    *       sx = takesignf( xn );
    *       sz = sx ^ ji;
    *       yc = 1.f/yt;
    *       zn = ( ji ? yc : yt );
    *       zn = changesignf( zn, sz );
    *
    *       z[blkIx*blkSize+n] = ( xm<=tanf_maxval ? zn : qNaNf.f );
    *     }
    *   }
    */
    {
      /* Input value and its magnitude; resulting value. */
      xtfloat xn, xm;
      /* Tangent and cotangent */
      xtfloat yt, yc;
      /* Input value segment number. */
      int32_t ji;
      /* Auxiliary floating-point var. */
      xtfloat t;
      /* Input sign; result sign; auxiliary var.  */
      int32_t sx, sz, s;
      /* Cotangent/tangent selection flag; input data validation flag. */
      xtbool b_cot, b_dom;

      X = (xtfloat*)((uintptr_t)x + blkIx*blkSize*sz_f32);
      Z = (int32_t*)((uintptr_t)z + blkIx*blkSize*sz_f32);

      S_rd = (xtfloat*)scr;
      __Pragma("loop_count min=1");
      for (n = 0; n<blkLen; n++)
      {
        XT_LSIP(xn, X, sz_f32);
        /* Load tangent approximation from the scratch. */
        XT_LSIP(yt, S_rd, sz_f32);
        /* Re-calculate the pi/2-wide segment number. */
        xm = XT_ABS_S(xn);
        b_dom = XT_OLE_S(xm, tanf_maxval);
        t = XT_MUL_S(xm, inv4pif.f);
        t = XT_FICEIL_S(t);
        ji = (int)XT_TRUNC_S(t, 0);
        ji = ji << 30;
        /* Compute the sign adjustme nt term. */
        sx = XT_RFR(xn);
        sz = sx ^ ji;
        sz = (sz & 0x80000000);
        /* Compute the cotangent for odd-numbered segments. */
        yc = XT_RECIP_S(yt);
        b_cot = AE_INT64_LT(AE_MOVINT64_FROMINT32(ji), AE_ZERO64());
 
        XT_MOVT_S(yt, yc, (b_cot));
        /* Adjust the sign. */
        s = XT_RFR(yt);
        s = XT_XOR(s, sz);

        /* Set result to quiet NaN for an out-of-domain input value. */
        {
          unsigned int _t = s;
          XT_MOVF(_t, qNaNf.u, b_dom); s = _t;
        }
        *Z++=s;
      }
    }
  }
} /* vec_tanf() */
#endif
