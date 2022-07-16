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
#include "NatureDSP_types.h"

#include <math.h>
#include <errno.h>
#include <__fenv.h>

/* Common helper macros. */
#include "common.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(float32_t, scl_acosf, (float32_t x))
#else
/* tables and constants */
#include "pif_tbl.h"
#include "asinf_tbl.h"
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"

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
float32_t scl_acosf( float32_t x )
{
    float32_t zero,one,half;
    xtfloat g,r,s,t;
    xtfloat a, b, c, y;
    int sx;

    /* Floating-point Status and Control Register values. */
    ae_int64 fstate;

    xtbool xnan,xinv;
    xtbool gzero;
    xtbool  xone,xlesshalf;

    one  = (float32_t)XT_CONST_S(1);
    half = (float32_t)XT_CONST_S(3);
    sx=XT_RFR(x);
    x=XT_ABS_S(x);
    xinv=XT_ULT_S( one, x) ;
    xone=XT_UEQ_S( one, x) ;
    xlesshalf=XT_ULT_S(x,half);
    xnan =XT_UN_S( x, x );
    if ((xnan) )
    {
        __Pragma("frequency_hint never");
        errno=EDOM;
        return qNaNf.f;
    }

    if (xinv)
    {
        __Pragma("frequency_hint never");
        errno=EDOM;
        __feraiseexcept( FE_INVALID);
        return qNaNf.f;
    }

    /* Sample floating-point exception flags. */
    fstate = XT_AE_MOVVFCRFSR();

    if (xlesshalf)
    {
        y=x;
        g=XT_MUL_S(y,y);
        /* compute r */
        r=asinftbl[0].f;
        t=asinftbl[1].f; XT_MADD_S(t,r,g); r=t;
        t=asinftbl[2].f; XT_MADD_S(t,r,g); r=t;
        t=asinftbl[3].f; XT_MADD_S(t,r,g); r=t;
        t=asinftbl[4].f; XT_MADD_S(t,r,g); r=t;
        r=XT_MUL_S(r,g);
        XT_MADD_S(y,y,r);
        XT_MOVLTZ_S(y,XT_NEG_S(y),sx);
        y = XT_SUB_S(pi2f.f , y);

        /* Suppress spurious exception flags and restore original status flags. */
        XT_AE_MOVFCRFSRV( fstate );

        return y;
    }
    
    x=(one-x);
    g=x*half;
    zero = (float32_t)XT_CONST_S(0);
    /* compute r */
    r=asinftbl[0].f;
    t=asinftbl[1].f; XT_MADD_S(t,r,g); r=t;
    t=asinftbl[2].f; XT_MADD_S(t,r,g); r=t;
    t=asinftbl[3].f; XT_MADD_S(t,r,g); r=t;
    t=asinftbl[4].f; XT_MADD_S(t,r,g); r=t;

    r=XT_MUL_S(r,g);
    /* Initial approximation */
    gzero=XT_OEQ_S(g,zero);
    XT_MOVT_S(g,one,gzero);  // to prevent from DIVZ exceptions if x==+/-1
    t = XT_RSQRT0_S( g );
    XT_MOVT_S(t,zero,gzero);

    /* Newton-Raphson iteration */
    a = XT_MUL_S(g,t);
    b = XT_MUL_S(half,t);
    c=one; XT_MSUB_S(c,a,t);
    XT_MADD_S(t,b,c);
    /* Newton-Raphson iteration */
    a = XT_MUL_S(g,t);
    b = XT_MUL_S(half,t);
    c=one; XT_MSUB_S(c,a,t);
    XT_MADD_S(t,b,c);
    y=XT_MUL_S(x,t);
    s=zero; XT_MOVLTZ_S(s,pif.f,sx);
    XT_MADD_S(y,y,r);
    XT_MOVLTZ_S(y,XT_NEG_S(y),sx);
    y = XT_ADD_S(s , y);

    /* Suppress spurious exception flags and restore original status flags. */
    XT_AE_MOVFCRFSRV( fstate );

    return y;

} /* acosf() */
#endif
