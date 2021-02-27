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

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal.h"
#include <math.h>
#include <errno.h>
#include <fenv.h>

/* Common helper macros. */
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, vec_asinf, (float32_t * restrict z, const float32_t * restrict x, int N))
#else
/* tables and constants */
#include "pif_tbl.h"
#include "asinf_tbl.h"
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
void vec_asinf( float32_t * restrict z, const float32_t * restrict x, int N )
#if 0
{
    const xtfloat* ptbl=(const xtfloat*)asinftbl;
    int n;
    xtfloat half,one,zero;   
    half=0.5f;
    one=1.f;
    zero=0.f;
    for (n=0; n<N; n++)
    {
        xtfloat X,Y,G,R,A,D,Gsmall,Xsmall;
        xtfloat t;
        xtbool bsmall;
        int32_t sx;
        X=x[n];
        sx=XT_RFR(X);
        X=XT_ABS_S(X);
        Gsmall=XT_MUL_S(X,X);
        Xsmall=XT_NEG_S(X);
        bsmall=XT_OLT_S(X,half);
        X=XT_SUB_S(one,X);
        G=XT_MUL_S(X,half);
        t=G; XT_MOVT_S(t,one,XT_OEQ_S(G,zero));
        R=XT_RSQRT_S(t);
        Y=XT_MUL_S(G,R);
        t=G; XT_MSUB_S(t,Y,Y); D=t;
        t=Y; XT_MADD_S(t,XT_MUL_S(half,R),D); Y=t;
        X=XT_MUL_S(2.f,Y);
        A=pi2f.f;
        t=X;XT_MOVT_S(t,Xsmall,bsmall);X=t;
        t=A;XT_MOVT_S(t,zero  ,bsmall);A=t;
        t=G;XT_MOVT_S(t,Gsmall,bsmall);G=t;
        /* compute r */
        R=XT_LSI(ptbl,0*sizeof(xtfloat));
        t=XT_LSI(ptbl,1*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
        t=XT_LSI(ptbl,2*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
        t=XT_LSI(ptbl,3*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
        t=XT_LSI(ptbl,4*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
        R=XT_MUL_S(R,G);
        XT_MADD_S(X,X,R); 
        X=XT_SUB_S(A,X);
        XT_MOVLTZ_S(X,XT_NEG_S(X),sx);
        z[n]=X;
    }
}
#else
{
    float32_t ALIGN(8) scratch[MAX_ALLOCA_SZ/sizeof(float32_t)];
    const xtfloat* restrict px0;
    const xtfloat* restrict px1;
          xtfloat* restrict pz;
    const xtfloat* restrict pScrRd;
          xtfloat* restrict pScrWr;
    const xtfloat* ptbl=(const xtfloat*)asinftbl;
    int n,M;
    xtfloat X,Y,G,R,A,D,Gsmall,Xsmall;
    xtfloat t;
    xtfloat half,one,zero;   
    NASSERT(x);
    NASSERT(z);
    if(N<=0) return;
    M=N;
    half=0.5f;
    one=1.f;
    zero=0.f;
    M=N;
    px0=(const xtfloat*)x;
    px1=(const xtfloat*)x;
    pz=(      xtfloat*)z;
    while(M>1)
    {
        N=XT_MIN(M,(MAX_ALLOCA_SZ/sizeof(float32_t))/2) & ~1;
        pScrRd=(const xtfloat*)scratch;
        pScrWr=(      xtfloat*)scratch;
        /* 1..3 phases: compute 2.*sqrt((1-x)*0.5) */
        for (n=0; n<(N>>1); n++)
        {
            XT_LSIP(X,px0,sizeof(float32_t));
            X=XT_ABS_S(X);
            X=XT_SUB_S(one,X);
            G=XT_MUL_S(X,half);
            t=G; XT_MOVT_S(t,one,XT_OEQ_S(G,zero));
            XT_SSIP(G,pScrWr,sizeof(float32_t));
            XT_SSIP(t,pScrWr,sizeof(float32_t));
            XT_LSIP(X,px0,sizeof(float32_t));
            X=XT_ABS_S(X);
            X=XT_SUB_S(one,X);
            G=XT_MUL_S(X,half);
            t=G; XT_MOVT_S(t,one,XT_OEQ_S(G,zero));
            XT_SSIP(G,pScrWr,sizeof(float32_t));
            XT_SSIP(t,pScrWr,sizeof(float32_t));
        }
        __Pragma("no_reorder")
        pScrRd=(const xtfloat*)scratch;
        pScrWr=(      xtfloat*)(scratch+1);
        for (n=0; n<(N>>1); n++)
        {
            XT_LSIP(G,pScrRd,sizeof(float32_t));
            XT_LSIP(t,pScrRd,sizeof(float32_t));
            R=XT_RSQRT_S(t);
            XT_SSIP(R,pScrWr,2*sizeof(float32_t));

            XT_LSIP(G,pScrRd,sizeof(float32_t));
            XT_LSIP(t,pScrRd,sizeof(float32_t));
            R=XT_RSQRT_S(t);
            XT_SSIP(R,pScrWr,2*sizeof(float32_t));
        }
        __Pragma("no_reorder")
        pScrRd=(const xtfloat*)scratch;
        pScrWr=(      xtfloat*)(scratch+1);
        for (n=0; n<(N>>1); n++)
        {
            XT_LSIP(G,pScrRd,sizeof(float32_t));
            XT_LSIP(R,pScrRd,sizeof(float32_t));
            Y=XT_MUL_S(G,R);
            t=G; XT_MSUB_S(t,Y,Y); D=t;
            t=Y; XT_MADD_S(t,XT_MUL_S(half,R),D); Y=t;
            X=XT_MUL_S(2.f,Y);
            XT_SSIP(X,pScrWr,2*sizeof(float32_t));

            XT_LSIP(G,pScrRd,sizeof(float32_t));
            XT_LSIP(R,pScrRd,sizeof(float32_t));
            Y=XT_MUL_S(G,R);
            t=G; XT_MSUB_S(t,Y,Y); D=t;
            t=Y; XT_MADD_S(t,XT_MUL_S(half,R),D); Y=t;
            X=XT_MUL_S(2.f,Y);
            XT_SSIP(X,pScrWr,2*sizeof(float32_t));
        }
        /* finally, select G,X,A for small/big arguments, compute polynomial and apply the sign */
        __Pragma("no_reorder")
        pScrRd=(const xtfloat*)scratch;
        pScrWr=(      xtfloat*)scratch;
        for (n=0; n<N; n++)
        {
            xtbool bsmall;
            int32_t sx;
            XT_LSIP(X,px1,sizeof(float32_t));
            sx=XT_RFR(X);
            X=XT_ABS_S(X);
            Gsmall=XT_MUL_S(X,X);
            Xsmall=XT_NEG_S(XT_ABS_S(X));
            bsmall=XT_OLT_S(X,half);
            XT_LSIP(G,pScrRd,sizeof(float32_t));
            XT_LSIP(X,pScrRd,sizeof(float32_t));
            A=pi2f.f;
            t=X;XT_MOVT_S(t,Xsmall,bsmall);X=t;
            t=A;XT_MOVT_S(t,zero  ,bsmall);A=t;
            t=G;XT_MOVT_S(t,Gsmall,bsmall);G=t;
            /* compute r */
            R=XT_LSI(ptbl,0*sizeof(xtfloat));
            t=XT_LSI(ptbl,1*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
            t=XT_LSI(ptbl,2*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
            t=XT_LSI(ptbl,3*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
            t=XT_LSI(ptbl,4*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
            R=XT_MUL_S(R,G);
            XT_MADD_S(X,X,R); 
            X=XT_SUB_S(A,X);
            XT_MOVLTZ_S(X,XT_NEG_S(X),sx);
            XT_SSIP(X,pz,sizeof(float32_t));
        }
        M-=N;
    }
    /* last odd iteration */
    if(M) 
    {
        xtfloat X,Y,G,R,A,D,Gsmall,Xsmall;
        xtfloat t;
        xtbool bsmall;
        int32_t sx;
        X=XT_LSI(px0,0);
        sx=XT_RFR(X);
        X=XT_ABS_S(X);
        Gsmall=XT_MUL_S(X,X);
        Xsmall=XT_NEG_S(X);
        bsmall=XT_OLT_S(X,half);
        X=XT_SUB_S(one,X);
        G=XT_MUL_S(X,half);
        t=G; XT_MOVT_S(t,one,XT_OEQ_S(G,zero));
        R=XT_RSQRT_S(t);
        Y=XT_MUL_S(G,R);
        t=G; XT_MSUB_S(t,Y,Y); D=t;
        t=Y; XT_MADD_S(t,XT_MUL_S(half,R),D); Y=t;
        X=XT_MUL_S(2.f,Y);
        A=pi2f.f;
        t=X;XT_MOVT_S(t,Xsmall,bsmall);X=t;
        t=A;XT_MOVT_S(t,zero  ,bsmall);A=t;
        t=G;XT_MOVT_S(t,Gsmall,bsmall);G=t;
        /* compute r */
        R=XT_LSI(ptbl,0*sizeof(xtfloat));
        t=XT_LSI(ptbl,1*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
        t=XT_LSI(ptbl,2*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
        t=XT_LSI(ptbl,3*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
        t=XT_LSI(ptbl,4*sizeof(xtfloat)); XT_MADD_S(t,R,G); R=t;
        R=XT_MUL_S(R,G);
        XT_MADD_S(X,X,R); 
        X=XT_SUB_S(A,X);
        XT_MOVLTZ_S(X,XT_NEG_S(X),sx);
        XT_SSI(X,pz,0);
    }
}

#endif
#endif
