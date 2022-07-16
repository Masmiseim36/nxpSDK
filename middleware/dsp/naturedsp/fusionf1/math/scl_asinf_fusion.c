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
#include <math.h>
#include <errno.h>
#include <__fenv.h>

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal.h"
/* Common helper macros. */
#include "common.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(float32_t, scl_asinf, (float32_t x))
#else
/* tables and constants */
#include "pif_tbl.h"
#include "asinf_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

#if ASINF_ALG!=0 /* 2 ULP algorithm */
#error wrong ASINF_ALG
#endif

/*-------------------------------------------------------------------------
  Arc Sine/Cosine 
  The arc sine/cosine functions return the arc sine/cosine of x. Output is 
  in radians. For floating-point routines, input value should belong to 
  [-1,1], otherwise the functions raise the  "invalid" floating-point 
  exception, assign EDOM to errno and return NaN.

  Precision: 
  f    floating point. Accuracy: 2 ULP

  Input:
  x[N]   input data, floating point
  N      length of vectors
  Output:
  z[N]   result, floating point

  Restriction:
  x,z should not overlap

  Scalar versions:
  ----------------
  return result, floating point
-------------------------------------------------------------------------*/
float32_t scl_asinf( float32_t  x )
{
    xtfloat g,f,z,one=1.f,half=0.5f,zero=0.f;
    xtbool xbig;
    float32_t r;
    int32_t sx;    

    /* Floating-point Status and Control Register values. */
    ae_int64 fstate;

    if ( XT_UN_S(x,x) )
    {
      __Pragma("frequency_hint never");
      errno = EDOM;
      return qNaNf.f;
    }
    z=XT_ABS_S(x);
    if ( XT_OLT_S(one,z) )
    {
      __Pragma("frequency_hint never");
      errno = EDOM;
      __feraiseexcept(FE_INVALID);
      return qNaNf.f;
    }
    /*
        Modyfied Cody Algorithm from Software Manual for Elementary Functions
        Uses polynomial approximation of r=asin(x)/x-1 instead of rational
        approximation (which involves division)
    */
    sx=XT_RFR(x);
    xbig=XT_OLE_S(half,z);

    /* Sample floating-point exception flags. */
    fstate = XT_AE_MOVVFCRFSR();

    if(xbig)
    {
        xtfloat t,a,b,c,y,dy,w,gg;
        xtbool gzero;
        z=XT_SUB_S(one,z);
        g=XT_MUL_S(z,half);
        /* compute r */
        gg=g;
        r=XT_LSI((const xtfloat*)asinftbl,0*sizeof(float32_t));
        /* Initial approximation */
        gzero=XT_OEQ_S(g,zero);
        XT_MOVT_S(g,one,gzero);  // to prevent from DIVZ exceptions if x==+/-1
        t = XT_RSQRT0_S( g );
        XT_MOVT_S(t,zero,gzero);
        /* Newton-Raphson iteration */
        a = XT_MUL_S(g,t);
        b = XT_MUL_S(half,t);
        c=one; XT_MSUB_S(c,a,t);
        w=XT_LSI((const xtfloat*)asinftbl,1*sizeof(float32_t)); XT_MADD_S(w,r,gg); r=w;
        XT_MADD_S(t,b,c);
        /* Newton-Raphson iteration */
        a = XT_MUL_S(g,t);
        w=XT_LSI((const xtfloat*)asinftbl,2*sizeof(float32_t)); XT_MADD_S(w,r,gg); r=w;
        b = XT_MUL_S(half,t);
        c=one; XT_MSUB_S(c,a,t);
        w=XT_LSI((const xtfloat*)asinftbl,3*sizeof(float32_t)); XT_MADD_S(w,r,gg); r=w;
        XT_MADD_S(t,b,c);
        y=XT_MUL_S(z,t);
        w=XT_LSI((const xtfloat*)asinftbl,4*sizeof(float32_t)); XT_MADD_S(w,r,gg); r=w;
        dy=XT_MUL_S(y,y);
        r=XT_MUL_S(r,gg);
        XT_MSUB_S(z,half,dy); dy=z;
        XT_MADD_S(dy,y,r);
        y=XT_ADD_S(dy,y);
        z=XT_SUB_S(pi2f.f,y);

        /* Suppress spurious exception flags and restore original status flags. */
        XT_AE_MOVFCRFSRV( fstate );

        XT_MOVLTZ_S(z,XT_NEG_S(z),sx);
        return z;
    }
    g=XT_MUL_S(z,z);
    r=asinftbl[0].f;
    f=asinftbl[1].f; XT_MADD_S(f,r,g); r=f;
    f=asinftbl[2].f; XT_MADD_S(f,r,g); r=f;
    f=asinftbl[3].f; XT_MADD_S(f,r,g); r=f;
    f=asinftbl[4].f; XT_MADD_S(f,r,g); r=f;
    r=XT_MUL_S(r,g);

    /* Suppress spurious exception flags and restore original status flags. */
    XT_AE_MOVFCRFSRV( fstate );

    XT_MADDN_S(z,z,r);
    XT_MOVLTZ_S(z,XT_NEG_S(z),sx);
    return z;
} /* asinf() */
#endif
