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
/*
  NatureDSP Signal Processing Library. Vector mathematics
    scalar variant of vec_powf
    code optimized for HiFi4 core
  IntegrIT, 2006-2019
*/

#include "NatureDSP_Signal_math.h"
#include "NatureDSP_types.h"
#include "common.h"
#include <errno.h>
/* Common helper macros. */
#include "common_fpu.h"
/* Inline functions for floating-point exceptions and environment control. */
#include "__fenv.h"
/* Constant tables. */
#include "pow2f_tbl.h"
/* +/-Infinity, single/double precision */
#include "inff_tbl.h"
#include "nanf_tbl.h"
#include "sqrt2f_tbl.h"
#define MIN(a,b)   ( (a)<(b) ? (a) : (b) )
#define MAX(a,b)   ( (a)>(b) ? (a) : (b) )

/*-------------------------------------------------------------------------
  Power function
  These routines calculate power function for 32-bit fixed-point numbers or 
  floating point numbers. 
  For the fixed point API, The  base is represented in Q31, the exponent 
  is represented in Q6.25. Results are represented as normalized fixed point
  number with separate mantissa in Q31 and exponent.

  Precision:
  32x32  32-bit inputs, 32-bit outputs
  f      floating point input, floating point output

  Accuracy: 
  2 ULP for fixed point API
  2 ULP under condition that |y|<=100

  Notes:
1. Scalar floating point raise  to a power functions conform to ANSI C requirements on 
   standard math library functions in respect to treatment of errno and floating-
   point exceptions. Vectorized function does not touch errno and may raise or not raise 
   floating point exceptions.
2. For floating point API, If x<0 is finite, y is finite and not an integer value, 
   then the respective result z is set to NaN
3. For fixed point API, function returns zero for all non-positive x. Fixed point 
   functions never touch errno

    Special cases:
          x   |   y    | Result |  Extra Conditions    
      --------+--------+--------+---------------------
      floating point API
      --------+--------+--------+---------------------
        +/-0  | y      | +/-inf | odd y<0
        +/-0  | y      | +inf   | even y<0
        +/-0  | y      | +/-0   | odd y>0
        +/-0  | y      | 0      | even y>0
        +/-1  | +/-inf | 1      | 
        1     | y      | 1      | any y including NaN
        x     | +/-0   | 1      | any x including NaN
        x     | y      | NaN    | finite x<0 and finite 
              |        |        | non-integer y (see 
              |        |        | note 2)
        x     | -inf   | +inf   | |x|<1
        x     | -inf   | 0      | |x|>1
        x     | +inf   | 0      | |x|<1
        x     | +inf   | +inf   | |x|>1
        -inf  | y      | -0     | y an odd integer <0
        -inf  | y      | 0      | y<0 and not an odd 
              |        |        | integer
        -inf  | y      | -inf   | y an odd integer >0
        -inf  | y      | +inf   | y>0 and not an odd 
              |        |        | integer
        +inf  | y      | 0      | y<0
        +inf  | y      | +inf   | y>0
      --------+--------+--------+---------------------
      fixed point API
      --------+--------+--------+---------------------
         x    | y      | 0      | x<=0
      --------+--------+--------+---------------------

  Input:
  x[N]  input data,Q0.31 or floating point
  y[N]  input data,Q6.25 or floating point
  N     length of vectors
  Output (fixed point API):
  m[N]  mantissa of output, Q31 
  e[N]  exponent of output  
  Output (floating point API):
  z[N]  results: floating point

  Restriction:
  z,x,y,m should not overlap
-------------------------------------------------------------------------*/
#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(float32_t, scl_powf, (float32_t x, float32_t y))
#elif HAVE_VFPU
float32_t scl_powf ( float32_t x, float32_t y )
{
  xtbool2 b_zero, b_one, b_Inf, b_NaN1, b_NaN2;
  xtbool2 bsx, b_xnan, b_xinf, b_xeqz, b_xeq1;
  xtbool2 bsy, b_ynan, b_yinf, b_yeqz, b_yint;
  xtbool2 b_zinf; 
  xtbool2 b_generic;
  ae_int32 x_i, y_i, zero_i;
  float32_t z;
  int32_t iy;
  int yodd;

  uint32_t yeqz, yinf, xeqz, xeq1, xinf, sx, sy, yint;
  uint32_t one, NaN1, NaN2, Inf, zero, xyfin, fe_inv, zinf;
  uint32_t fe_divz, snan, xnan, ynan, erange, fe_state;
  xtbool b_fe_inv, b_fe_divz, b_fe_ovfl;
  xtbool b_edom, b_erange;
  xtbool b_xyfin;
  /* Floating-point Status and Control Register values. */
  ae_int64 fstate;

  float32_t half = XT_CONST_S(3);
  zero_i = 0;
  x_i = XT_AE_MOVINT32X2_FROMXTFLOATX2( x );
  y_i = XT_AE_MOVINT32X2_FROMXTFLOATX2(y);

  fe_state = __fetestexcept(FE_INVALID | FE_OVERFLOW);
  /* Sample floating-point exception flags. */
  fstate = XT_AE_MOVVFCRFSR();

  /* Take sign of x */
  sx = XT_RFR(x);
  
  yint = ( (float32_t)XT_FITRUNC_S(y) == y );
  iy = (int)XT_TRUNC_S(y, 0);
  XT_MOVEQZ(iy, 0, iy-MAX_INT32);
  yodd = yint & (iy&1); /* check if y is odd */

  sx = sx & (yodd<<31);

  /* process special values first */
  {
    uint32_t sx;
    bsx = XT_OLT_S(x, XT_CONST_S(0));
    bsy = XT_OLT_S(y, XT_CONST_S(0));

    /* Check for integer y */
    b_yint = XT_OEQ_S(XT_FITRUNC_S(y), y);

    x = XT_ABS_S(x); /* reset sign */

    /* process special numbers */
    b_yeqz = XT_OEQ_S(XT_CONST_S(0), y);                              /*  y ==0     */
    b_yinf = XT_OEQ_S(XT_ABS_S(y), plusInff.f);              /* |y|==INF   */
    b_xeqz = XT_OEQ_S(x, XT_CONST_S(0));                              /*  x==0      */
    b_xinf = XT_OEQ_S(x, plusInff.f);                        /* |x|==INF   */
    b_xeq1 = XT_OEQ_S(x, XT_CONST_S(1));                               /* |x|==1     */

    yint = AE_MOVAB2(b_yint);
    yeqz = AE_MOVAB2(b_yeqz);
    yinf = AE_MOVAB2(b_yinf);
    xeqz = AE_MOVAB2(b_xeqz);
    xeq1 = AE_MOVAB2(b_xeq1);
    xinf = AE_MOVAB2(b_xinf);
    sx = AE_MOVAB2(bsx);
    sy = AE_MOVAB2(bsy);

    one = xeq1 & (yinf | (~sx));  /* |x|==1 && ( |y|==Inf || x>0 )                       */
    one = one | yeqz;           /* ( |x|==1 && ( |y|==Inf || x>0 ) ) || y==0 --> z=1.0 */
    NaN1 = sx&(~yint);          /* x<0 && y is not an integer --> z=NaN                */
    Inf = xinf&(~sy);          /* x==INF && y>0 --> z=INF */
    Inf = Inf | (xeqz & sy);    /* x==0   && y<0 --> z=INF */
    zero = xeqz &(~sy);         /* x==0   && y>0 --> z=0.0 */
    zero = zero | (xinf & sy);  /* x==INF && y<0 --> z=0.0 */

    b_NaN1 = AE_MOVBA2(NaN1);
    b_NaN2 = XT_UN_SX2(x, y);         /* isnan(x) || isnan(y) --> z=NaN                      */
    b_one = AE_MOVBA2(one);
    b_Inf = AE_MOVBA2(Inf);
    b_zero = AE_MOVBA2(zero);
    NaN2 = AE_MOVAB2(b_NaN2);
    /* Save special number */
    z = half;
    XT_MOVT_S(z, qNaNf.f, xtbool2_extract_0(b_NaN1));
    XT_MOVT_S(z, XT_CONST_S(0), xtbool2_extract_0(b_zero));
    XT_MOVT_S(z, plusInff.f, xtbool2_extract_0(b_Inf));
    XT_MOVT_S(z, qNaNf.f, xtbool2_extract_0(b_NaN2));
    XT_MOVT_S(z, XT_CONST_S(1), xtbool2_extract_0(b_one));
    /* Look if we have a special or generic case. */
    b_generic = XT_OEQ_S( z, half );


   {
     /* EDOM conditions:
     * A) x<0 && x is finite && y is not an integer && y is finite
     * B) x is NaN and y!=0
     * C) y is NaN and x!=1
     * We use that (A or B or C) if z==NaN. */
     b_edom = xtbool2_extract_0(XT_UN_S(z, z));
  
     /* Check if x, y are finite */
     xyfin = ~(xinf|yinf);
     xyfin = (xyfin&(~NaN2));
     b_xyfin = AE_MOVBA(xyfin);
    
     /* x<0 && x is finite && y is not an integer && y is finite --> raise "invalid" exception */
     fe_inv = (NaN1&(~xeqz));
     fe_inv = (fe_inv&xyfin);
     b_fe_inv = AE_MOVBA(fe_inv);
     /* "invalid" exception should be also raised if either input is a signalling NaN. */
     /* Check x and y for signalling NaN. */
     b_xnan = AE_LE32(zero_i, AE_SLLI32(x_i, 9));
     b_ynan = AE_LE32(zero_i, AE_SLLI32(y_i, 9));
     xnan = AE_MOVAB2(b_xnan);
     ynan = AE_MOVAB2(b_ynan);
     xnan = (AE_MOVAB2(XT_UN_S(x, x))) &xnan;
     ynan = (AE_MOVAB2(XT_UN_S(y, y))) &ynan;
     snan = xnan|ynan;
     fe_inv = fe_inv | snan;
     b_fe_inv = AE_MOVBA(fe_inv);
     /* x==0 && y<0 --> raise "divide-by-zero" exception */
     fe_divz = (xeqz&sy);

     b_fe_divz = AE_MOVBA(fe_divz);
   }

  }

  /* Computation of x^y: */
  /* x^y = 2^(y*log2(x)) */
  if (xtbool2_extract_0(b_generic))
  {
    static const int32_t c_tbl[] =
    {
      -126,
      -150,
      (int32_t)0x007FFFFF,/* max denormalized floating-point number / mantissa mask */
      (int32_t)0x4B800000,/* 2^24 */
      (int32_t)0x3F3504F3,/* sqrt(0.5) */
      (int32_t)0x3F000000,/*  0.5 */
      (int32_t)0xBF000000,/* -0.5 */
      -252,
      254
    };
    xtfloat x0,x1,xy0,dxy0;
    xtfloat c0, c1;
    ae_int32 e0, e1;

    /* compute log2(x) */
    {
      xtfloat t0, ef0, t1, t2, xx;
      xtfloat p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;
      xtfloat p10, p11, p12, p13, w0, w1;
      xtbool bdenorm, bsmall;
      ae_int32 e0, xi0, ex0;
      xx = x;
      /* process denormalized values */
      bdenorm = xtbool2_extract_0(XT_OLE_S(xx, XT_LSI((xtfloat*)c_tbl, 2 * 4)));
      t0 = XT_MUL_S(xx, XT_LSI((xtfloat*)c_tbl, 3 * 4));
      XT_MOVT_S(xx, t0, (bdenorm));
      e0 = AE_L32_I((ae_int32 *)c_tbl, 0 * 4);;
      AE_MOVT_32(e0, AE_L32_I((ae_int32 *)c_tbl, 1 * 4), (bdenorm));
      /* extract exponent */
      xi0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(xx);
      ex0 = AE_SRLI32(xi0, 23);
      e0 = AE_ADD32(e0, ex0);
      /* extract mantissa */
      ex0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(XT_LSI((xtfloat*)c_tbl, 2 * 4));/* load mantissa mask */ //!!!!!!!!!!!!!
      xi0 = AE_AND32(xi0, ex0);
      xi0 = AE_OR32(xi0, AE_L32_I((ae_int32 *)c_tbl, 5 * 4));
      xx = XT_AE_MOVXTFLOATX2_FROMINT32X2(xi0);
      /* adjust the mantissa to range [ sqrt(0.5) ; sqrt(2.0) ) */

      bsmall = xtbool2_extract_0(XT_OLT_S(xx, XT_LSI((xtfloat*)c_tbl, 4 * 4)));


      t0 = XT_ADD_S(xx, xx);
      ex0 = AE_SUB32(e0, 1);
      XT_MOVT_S(xx, t0, bsmall);
      AE_MOVT_32(e0, ex0, bsmall);
      xx = XT_SUB_S(1.0f, xx); //!!!
      ef0 = XT_FLOAT_S(e0, 0); //!!!

      /* evaluate polynomial approximation */
      /* Load table of coefficients */

      p0 = XT_LSI((const xtfloat *)log2f_coef, 0 * 4);
      p1 = XT_LSI((const xtfloat *)log2f_coef, 1 * 4);
      p2 = XT_LSI((const xtfloat *)log2f_coef, 2 * 4);
      p3 = XT_LSI((const xtfloat *)log2f_coef, 3 * 4);
      p4 = XT_LSI((const xtfloat *)log2f_coef, 4 * 4);
      p5 = XT_LSI((const xtfloat *)log2f_coef, 5 * 4);
      p6 = XT_LSI((const xtfloat *)log2f_coef, 6 * 4);
      p7 = XT_LSI((const xtfloat *)log2f_coef, 7 * 4);
      p8 = XT_LSX((const xtfloat *)log2f_coef, 8 * 4);
      p9 = XT_LSX((const xtfloat *)log2f_coef, 9 * 4);


      XT_MADD_S(p1, xx, p0);
      XT_MADD_S(p2, xx, p1);
      XT_MADD_S(p3, xx, p2);
      XT_MADD_S(p4, xx, p3);
      XT_MADD_S(p5, xx, p4);
      XT_MADD_S(p6, xx, p5);
      XT_MADD_S(p7, xx, p6);
      XT_MADD_S(p8, xx, p7);
      XT_MADD_S(p9, xx, p8);
      t2 = p9;


      p10 = XT_LSX((const xtfloat *)log2f_coef, 10 * 4);
      p11 = XT_LSX((const xtfloat *)log2f_coef, 11 * 4);
      p12 = XT_LSX((const xtfloat *)log2f_coef, 12 * 4);
      p13 = XT_LSX((const xtfloat *)log2f_coef, 13 * 4);

      /* next coefficients are computed in extended precision */
      t0 = XT_MUL_S(xx, t2); t1 = t0;
      XT_MSUB_S(t1, xx, t2);
      w0 = XT_ADD_S(t0, p10);
      w1 = XT_SUB_S(w0, p10);
      w1 = XT_SUB_S(t0, w1);
      w1 = XT_SUB_S(w1, t1);
      t0 = w0; t1 = w1;
      w0 = XT_MUL_S(xx, t0); w1 = w0;
      XT_MSUB_S(w1, xx, t0); t0 = w0;
      XT_MSUB_S(w1, xx, t1); t1 = w1;
      w0 = XT_ADD_S(t0, p11);
      w1 = XT_SUB_S(w0, p11);
      w1 = XT_SUB_S(t0, w1);
      w1 = XT_SUB_S(w1, t1);
      t0 = w0; t1 = w1;
      xx = XT_NEG_S(xx);
      w0 = XT_MUL_S(xx, t0); w1 = w0;
      XT_MSUB_S(w1, xx, t0); t0 = w0;
      XT_MSUB_S(w1, xx, t1); t1 = w1;
      /* multiply by log2(e) */
      w0 = XT_MUL_S(t0, p12); w1 = w0;
      XT_MSUB_S(w1, t0, p12);
      XT_MADD_S(w1, t1, p12);
      XT_MSUB_S(w1, t0, p13);
      t0 = w0; t1 = w1;
      /* add exponent */
      w0 = XT_ADD_S(t0, ef0);
      w1 = XT_SUB_S(w0, ef0);
      w1 = XT_SUB_S(t0, w1);
      x1 = XT_SUB_S(w1, t1);//!!!!
      x0 = w0; ///!!!!!
    }
    /* compute y*log2(x) and separate into integer and fractional parts */
    xy0 = XT_FIROUND_S(XT_MUL_S(y, x0));
    dxy0 = XT_NEG_S(xy0);
    XT_MADD_S(dxy0, y, x0);
    XT_MADD_S(dxy0, y, x1);
    dxy0 = XT_MIN_S(dxy0, 1.0f);
    dxy0 = XT_MAX_S(dxy0, -1.0f);
    /* compute 2^fract */
    {
      xtfloat p0, p1, p2, p3, p4, p5, p6;
      p0 = XT_LSI((const xtfloat *)pow2f_coef, 0 * 4);
      p1 = XT_LSI((const xtfloat *)pow2f_coef, 1 * 4);
      p2 = XT_LSI((const xtfloat *)pow2f_coef, 2 * 4);
      p3 = XT_LSI((const xtfloat *)pow2f_coef, 3 * 4);
      p4 = XT_LSI((const xtfloat *)pow2f_coef, 4 * 4);
      p5 = XT_LSI((const xtfloat *)pow2f_coef, 5 * 4);
      p6 = XT_LSI((const xtfloat *)pow2f_coef, 6 * 4);
      /* NOTE: do not change the order of computations and way of polynomial decomposition ! */ 
      XT_MADD_S(p1, dxy0, p0);
      XT_MADD_S(p2, dxy0, p1);
      XT_MADD_S(p3, dxy0, p2);
      XT_MADD_S(p4, dxy0, p3);
      XT_MADD_S(p5, dxy0, p4);
      XT_MADD_S(p6, dxy0, p5);
      z = p6;

    }
    
    /* apply integer part */
    xy0 = XT_MAX_S(xy0, -252.f);
    xy0 = XT_MIN_S(xy0, 254.f);
    e0 = XT_TRUNC_S(xy0, 0);
    e1 = AE_SRAI32(e0, 1);
    e0 = AE_SUB32(e0, e1);
    e1 = AE_SLLI32(AE_ADD32(e1, 127), 23);
    e0 = AE_SLLI32(AE_ADD32(e0, 127), 23);
    c0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(e0);
    c1 = XT_AE_MOVXTFLOATX2_FROMINT32X2(e1);
    z = XT_MUL_S(z, c1);
    z = XT_MUL_S(z, c0);
}

  /* Update errno and exceptions state */
  {
    int errno_ = 0;
    int fe_inv = 0, fe_divz = 0, fe_ovfl = 0;
    /* Suppress spurious exception flags and restore original status flags. */
    XT_AE_MOVFCRFSRV(fstate);

    /* (x==0)&&(y<0) || x is finite && y is finite && z is infinite --> set ERANGE */
    b_zinf = XT_OEQ_S(z, plusInff.f);
    zinf = AE_MOVAB2(b_zinf);
    erange = xeqz & sy;
    erange = erange | (xyfin & zinf);
    b_erange = AE_MOVBA(erange);

    errno_ = errno;
    XT_MOVT(errno_, ERANGE, b_erange);
    XT_MOVT(errno_, EDOM, b_edom);
    errno = errno_;

    /* x!=0 && x is finite && y is finite && z is infinite --> raise "overflow" exception */
    fe_ovfl = xyfin &(~xeqz);
    fe_ovfl = fe_ovfl&zinf;
    b_fe_ovfl = AE_MOVBA(fe_ovfl);


    XT_MOVT(fe_inv, FE_INVALID, b_fe_inv);
    XT_MOVT(fe_divz, FE_DIVBYZERO, b_fe_divz);
    XT_MOVT(fe_ovfl, FE_OVERFLOW, b_fe_ovfl);

    __feclearexcept(FE_INVALID | FE_OVERFLOW);
    __feraiseexcept(fe_state | fe_inv | fe_divz | fe_ovfl);
 }
  /* restore sign */
  {
    if (sx) z = -z;
  }
  return z;

}
#else
float32_t scl_powf(float32_t x, float32_t y)
{
  xtbool b_zero, b_one, b_Inf, b_NaN1, b_NaN2;
  xtbool bsx, b_xnan, b_xinf, b_xeqz, b_xeq1;
  xtbool bsy, b_ynan, b_yinf, b_yeqz, b_yint;
  xtbool b_zinf, b_generic;
  ae_int32 x_i, y_i, zero_i;
  float32_t z;
  int32_t iy;
  int yodd;

  uint32_t yeqz, yinf, xeqz, xeq1, xinf, sx, sy, yint;
  uint32_t one, NaN1, NaN2, Inf, zero, xyfin, fe_inv, zinf;
  uint32_t fe_divz, snan, xnan, ynan, erange, fe_state;
  xtbool b_fe_inv, b_fe_divz, b_fe_ovfl;
  xtbool b_edom, b_erange;
  xtbool b_xyfin;
  /* Floating-point Status and Control Register values. */
  uint32_t fstate;

  float32_t half = XT_CONST_S(3);
  zero_i = 0;
  x_i = XT_RFR( x );
  y_i = XT_RFR(y);

  fe_state = __fetestexcept(FE_INVALID | FE_OVERFLOW);
  /* Sample floating-point exception flags. */
  fstate = XT_RUR_FSR();

  /* Take sign of x */
  sx = XT_RFR(x);
  

#if defined(XT_FITRUNC_S)
  yint = ((float32_t)XT_FITRUNC_S(y) == y);
#else
  {   /* validate if y is integral - all numbers bigger than 2^23 are assumed as integral */
    xtfloat t, c;
    t = XT_ABS_S((xtfloat)y);
    c = 8388608.f;
    XT_MOVT_S(c, t, XT_ULT_S(t, 8388608.f));
    t = c;
    b_yint = XT_OEQ_S(XT_FLOAT_S(XT_TRUNC_S(t, 0), 0), t);
    yint = AE_MOVAB(b_yint);
  }
#endif
  iy = (int)XT_TRUNC_S(y, 0);
  XT_MOVEQZ(iy, 0, iy-MAX_INT32);
  yodd = yint & (iy&1); /* check if y is odd */

  sx = sx & (yodd<<31);

  /* process special values first */
  {

    uint32_t sx;
    bsx = XT_OLT_S(x, XT_CONST_S(0));
    bsy = XT_OLT_S(y, XT_CONST_S(0));

    x = XT_ABS_S(x); /* reset sign */
   
    /* process special numbers */
    b_yeqz = XT_OEQ_S(XT_CONST_S(0), y);                     /*  y ==0     */
    b_yinf = XT_OEQ_S(XT_ABS_S(y), plusInff.f);              /* |y|==INF   */
    b_xeqz = XT_OEQ_S(x, XT_CONST_S(0));                     /*  x==0      */
    b_xinf = XT_OEQ_S(x, plusInff.f);                        /* |x|==INF   */
    b_xeq1 = XT_OEQ_S(x, XT_CONST_S(1));                     /* |x|==1     */
   
//    yint = AE_MOVAB2(b_yint);
    yeqz = AE_MOVAB(b_yeqz);
    yinf = AE_MOVAB(b_yinf);
    xeqz = AE_MOVAB(b_xeqz);
    xeq1 = AE_MOVAB(b_xeq1);
    xinf = AE_MOVAB(b_xinf);
    sx = AE_MOVAB(bsx);
    sy = AE_MOVAB(bsy);
   
    one = xeq1 & (yinf | (~sx));  /* |x|==1 && ( |y|==Inf || x>0 )                       */
    one = one | yeqz;           /* ( |x|==1 && ( |y|==Inf || x>0 ) ) || y==0 --> z=1.0 */
    NaN1 = sx&(~yint);          /* x<0 && y is not an integer --> z=NaN                */
    Inf = xinf&(~sy);          /* x==INF && y>0 --> z=INF */
    Inf = Inf | (xeqz & sy);    /* x==0   && y<0 --> z=INF */
    zero = xeqz &(~sy);         /* x==0   && y>0 --> z=0.0 */
    zero = zero | (xinf & sy);  /* x==INF && y<0 --> z=0.0 */
   
    b_NaN1 = AE_MOVBA(NaN1);
    b_NaN2 = XT_UN_S(x, y);         /* isnan(x) || isnan(y) --> z=NaN                      */
    b_one = AE_MOVBA(one);
    b_Inf = AE_MOVBA(Inf);
    b_zero = AE_MOVBA(zero);
    NaN2 = AE_MOVAB(b_NaN2);
    /* Save special number */
    z = half;
    XT_MOVT_S(z, qNaNf.f, (b_NaN1));
    XT_MOVT_S(z, XT_CONST_S(0), (b_zero));
    XT_MOVT_S(z, plusInff.f, (b_Inf));
    XT_MOVT_S(z, qNaNf.f, (b_NaN2));
    XT_MOVT_S(z, XT_CONST_S(1), (b_one));
    /* Look if we have a special or generic case. */
    b_generic = XT_OEQ_S( z, half );


   {
     /* EDOM conditions:
     * A) x<0 && x is finite && y is not an integer && y is finite
     * B) x is NaN and y!=0
     * C) y is NaN and x!=1
     * We use that (A or B or C) if z==NaN. */
     b_edom = (XT_UN_S(z, z));
  
     /* Check if x, y are finite */
     xyfin = ~(xinf|yinf);
     xyfin = (xyfin&(~NaN2));
     b_xyfin = AE_MOVBA(xyfin);
    
     /* x<0 && x is finite && y is not an integer && y is finite --> raise "invalid" exception */
     fe_inv = (NaN1&(~xeqz));
     fe_inv = (fe_inv&xyfin);
     b_fe_inv = AE_MOVBA(fe_inv);
     /* "invalid" exception should be also raised if either input is a signalling NaN. */
     /* Check x and y for signalling NaN. */
     b_xnan = xtbool2_extract_0(AE_LE32(zero_i, AE_SLLI32(x_i, 9)));
     b_ynan = xtbool2_extract_0(AE_LE32(zero_i, AE_SLLI32(y_i, 9)));
     xnan = AE_MOVAB(b_xnan);
     ynan = AE_MOVAB(b_ynan);
     xnan = (AE_MOVAB(XT_UN_S(x, x))) &xnan;
     ynan = (AE_MOVAB(XT_UN_S(y, y))) &ynan;
     snan = xnan|ynan;
     fe_inv = fe_inv | snan;
     b_fe_inv = AE_MOVBA(fe_inv);
     /* x==0 && y<0 --> raise "divide-by-zero" exception */
     fe_divz = (xeqz&sy);
  
     b_fe_divz = AE_MOVBA(fe_divz);
   }

  }

  /* Computation of x^y: */
  /* x^y = 2^(y*log2(x)) */
  if ((b_generic))
  {
   
    static const int32_t c_tbl[] =
    {
      -126,
      -150,
      (int32_t)0x007FFFFF,/* max denormalized floating-point number / mantissa mask */
      (int32_t)0x4B800000,/* 2^24 */
      (int32_t)0x3F3504F3,/* sqrt(0.5) */
      (int32_t)0x3F000000,/*  0.5 */
      (int32_t)0xBF000000,/* -0.5 */

    };
    xtfloat x0,x1,xy0,dxy0;
    xtfloat c0, c1, _1, _m1;
    ae_int32 e0, e1;

    /* compute log2(x) */
    {
      xtfloat t0, ef0, t1, t2, xx;
      xtfloat p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;
      xtfloat p10, p11, p12, p13, w0, w1;
      xtbool bdenorm, bsmall;
      ae_int32 e0, xi0, ex0;
      xx = x;
      /* process denormalized values */
      bdenorm = (XT_OLE_S(xx, XT_LSI((xtfloat*)c_tbl, 2 * 4)));
      t0 = XT_MUL_S(xx, XT_LSI((xtfloat*)c_tbl, 3 * 4));
      XT_MOVT_S(xx, t0, (bdenorm));
      e0 = AE_L32_I((ae_int32 *)c_tbl, 0 * 4);;
      AE_MOVT_32(e0, AE_L32_I((ae_int32 *)c_tbl, 1 * 4), (bdenorm));
      /* extract exponent */
      xi0 = XT_RFR(xx);
      ex0 = AE_SRLI32(xi0, 23);
      e0 = AE_ADD32(e0, ex0);
      /* extract mantissa */
      ex0 = XT_RFR(XT_LSI((xtfloat*)c_tbl, 2 * 4));/* load mantissa mask */ //!!!!!!!!!!!!!
      xi0 = AE_AND32(xi0, ex0);
      xi0 = AE_OR32(xi0, AE_L32_I((ae_int32 *)c_tbl, 5 * 4));
      xx = XT_WFR(xi0);
      /* adjust the mantissa to range [ sqrt(0.5) ; sqrt(2.0) ) */

      bsmall = (XT_OLT_S(xx, XT_LSI((xtfloat*)c_tbl, 4 * 4)));


      t0 = XT_ADD_S(xx, xx);
      ex0 = AE_SUB32(e0, 1);
      XT_MOVT_S(xx, t0, bsmall);
      AE_MOVT_32(e0, ex0, bsmall);
      xx = XT_SUB_S(1.0f, xx); //!!!
      ef0 = XT_FLOAT_S(e0, 0); //!!!

      /* evaluate polynomial approximation */
      /* Load table of coefficients */

      p0 = XT_LSI((const xtfloat *)log2f_coef, 0 * 4);
      p1 = XT_LSI((const xtfloat *)log2f_coef, 1 * 4);
      p2 = XT_LSI((const xtfloat *)log2f_coef, 2 * 4);
      p3 = XT_LSI((const xtfloat *)log2f_coef, 3 * 4);
      p4 = XT_LSI((const xtfloat *)log2f_coef, 4 * 4);
      p5 = XT_LSI((const xtfloat *)log2f_coef, 5 * 4);
      p6 = XT_LSI((const xtfloat *)log2f_coef, 6 * 4);
      p7 = XT_LSI((const xtfloat *)log2f_coef, 7 * 4);
      p8 = XT_LSX((const xtfloat *)log2f_coef, 8 * 4);
      p9 = XT_LSX((const xtfloat *)log2f_coef, 9 * 4);


      XT_MADD_S(p1, xx, p0);
      XT_MADD_S(p2, xx, p1);
      XT_MADD_S(p3, xx, p2);
      XT_MADD_S(p4, xx, p3);
      XT_MADD_S(p5, xx, p4);
      XT_MADD_S(p6, xx, p5);
      XT_MADD_S(p7, xx, p6);
      XT_MADD_S(p8, xx, p7);
      XT_MADD_S(p9, xx, p8);
      t2 = p9;


      p10 = XT_LSX((const xtfloat *)log2f_coef, 10 * 4);
      p11 = XT_LSX((const xtfloat *)log2f_coef, 11 * 4);
      p12 = XT_LSX((const xtfloat *)log2f_coef, 12 * 4);
      p13 = XT_LSX((const xtfloat *)log2f_coef, 13 * 4);

      /* next coefficients are computed in extended precision */
      t0 = XT_MUL_S(xx, t2); t1 = t0;
      XT_MSUB_S(t1, xx, t2);
      w0 = XT_ADD_S(t0, p10);
      w1 = XT_SUB_S(w0, p10);
      w1 = XT_SUB_S(t0, w1);
      w1 = XT_SUB_S(w1, t1);
      t0 = w0; t1 = w1;
      w0 = XT_MUL_S(xx, t0); w1 = w0;
      XT_MSUB_S(w1, xx, t0); t0 = w0;
      XT_MSUB_S(w1, xx, t1); t1 = w1;
      w0 = XT_ADD_S(t0, p11);
      w1 = XT_SUB_S(w0, p11);
      w1 = XT_SUB_S(t0, w1);
      w1 = XT_SUB_S(w1, t1);
      t0 = w0; t1 = w1;
      xx = XT_NEG_S(xx);
      w0 = XT_MUL_S(xx, t0); w1 = w0;
      XT_MSUB_S(w1, xx, t0); t0 = w0;
      XT_MSUB_S(w1, xx, t1); t1 = w1;
      /* multiply by log2(e) */
      w0 = XT_MUL_S(t0, p12); w1 = w0;
      XT_MSUB_S(w1, t0, p12);
      XT_MADD_S(w1, t1, p12);
      XT_MSUB_S(w1, t0, p13);
      t0 = w0; t1 = w1;
      /* add exponent */
      w0 = XT_ADD_S(t0, ef0);
      w1 = XT_SUB_S(w0, ef0);
      w1 = XT_SUB_S(t0, w1);
      x1 = XT_SUB_S(w1, t1);//!!!!
      x0 = w0; ///!!!!!
    }

    /* compute y*log2(x) and separate into integer and fractional parts */
    _1 = 1.0f; _m1 = -1.0f;
    xy0 = XT_FLOAT_S(XT_ROUND_S(XT_MUL_S(y, x0), 0), 0);
    dxy0 = XT_NEG_S(xy0);
    XT_MADD_S(dxy0, y, x0);
    XT_MADD_S(dxy0, y, x1);
    dxy0 = XT_MIN_S(dxy0, _1);
    dxy0 = XT_MAX_S(dxy0, _m1);

    /* compute 2^fract */
    {
      xtfloat p0, p1, p2, p3, p4, p5, p6;
      p0 = XT_LSI((const xtfloat *)pow2f_coef, 0 * 4);
      p1 = XT_LSI((const xtfloat *)pow2f_coef, 1 * 4);
      p2 = XT_LSI((const xtfloat *)pow2f_coef, 2 * 4);
      p3 = XT_LSI((const xtfloat *)pow2f_coef, 3 * 4);
      p4 = XT_LSI((const xtfloat *)pow2f_coef, 4 * 4);
      p5 = XT_LSI((const xtfloat *)pow2f_coef, 5 * 4);
      p6 = XT_LSI((const xtfloat *)pow2f_coef, 6 * 4);
      /* NOTE: do not change the order of computations and way of polynomial decomposition ! */ 
      XT_MADD_S(p1, dxy0, p0);
      XT_MADD_S(p2, dxy0, p1);
      XT_MADD_S(p3, dxy0, p2);
      XT_MADD_S(p4, dxy0, p3);
      XT_MADD_S(p5, dxy0, p4);
      XT_MADD_S(p6, dxy0, p5);
      z = p6;

    }

    /* apply integer part */
    c0 = -252.0f; c1 = 254.0f;
    xy0 = XT_MAX_S(xy0, c0);
    xy0 = XT_MIN_S(xy0, c1);
    e0 = XT_TRUNC_S(xy0, 0);
    e1 = AE_SRAI32(e0, 1);
    e0 = AE_SUB32(e0, e1);
    e1 = AE_SLLI32(AE_ADD32(e1, 127), 23);
    e0 = AE_SLLI32(AE_ADD32(e0, 127), 23);
    c0 = XT_WFR(e0);
    c1 = XT_WFR(e1);
    z = XT_MUL_S(z, c1);
    z = XT_MUL_S(z, c0);
  }

  /* Update errno and exceptions state */
  {

    int errno_ = 0;
    int fe_inv = 0, fe_divz = 0, fe_ovfl = 0;
    /* Suppress spurious exception flags and restore original status flags. */
    XT_WUR_FSR(fstate);

    /* (x==0)&&(y<0) || x is finite && y is finite && z is infinite --> set ERANGE */
    b_zinf = XT_OEQ_S(z, plusInff.f);
    zinf = AE_MOVAB(b_zinf);
    erange = xeqz & sy;
    erange = erange | (xyfin & zinf);
    b_erange = AE_MOVBA(erange);

    errno_ = errno;
    XT_MOVT(errno_, ERANGE, b_erange);
    XT_MOVT(errno_, EDOM, b_edom);
    errno = errno_;

    /* x!=0 && x is finite && y is finite && z is infinite --> raise "overflow" exception */
    fe_ovfl = xyfin &(~xeqz);
    fe_ovfl = fe_ovfl&zinf;
    b_fe_ovfl = AE_MOVBA(fe_ovfl);


    XT_MOVT(fe_inv, FE_INVALID, b_fe_inv);
    XT_MOVT(fe_divz, FE_DIVBYZERO, b_fe_divz);
    XT_MOVT(fe_ovfl, FE_OVERFLOW, b_fe_ovfl);

    __feclearexcept(FE_INVALID | FE_OVERFLOW);
    __feraiseexcept(fe_state | fe_inv | fe_divz | fe_ovfl);
 }

  /* restore sign */
  {
    if (sx) z = -z;
  }
  return z;

} /* scl_powf() */
#endif
