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
/* Common helper macros. */
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, vec_rsqrtf, (float32_t* restrict z, const float32_t* restrict x, int N))
#else
/* sNaN/qNaN, single precision. */
#include "nanf_tbl.h"
/* +/-Infinity, single precision */
#include "inff_tbl.h"

/*-------------------------------------------------------------------------
  Reciprocal Square Root
  These routines compute reciprocals of positive square root.
   Whenever an input value is negative, functions raise the "invalid" 
   floating-point exception, assign EDOM to errno and set output value to 
   NaN. For x[n]==+/-0, functions set output to +/-infinity, raise the 
   "divide by zero" floating-point exception, and assign ERANGE to errno.

  Precision: 
  f      floating point. Accuracy 2 ULP

  Input:
  x[N]  input data
  N     length of vectors
  Output:
  y[N]  output data

  Restriction:
  x,y - should not overlap

  Scalar versions:
  ----------------
  return result
-------------------------------------------------------------------------*/
void vec_rsqrtf         (float32_t* restrict z, const float32_t* restrict x, int N)
{
    int32_t ALIGN(8) scratch[MAX_ALLOCA_SZ/sizeof(int32_t)];
    const xtfloatx2 * restrict px0;
    const ae_int32x2 * restrict px1;
             xtfloat * pScrWr;
    const ae_int32x2 * pScrRd;
          ae_int32x2 * restrict pz;
    ae_valign ax0,ax1,az;
    int n,M;

    NASSERT( x );
    NASSERT( z );
    if(N<=0) return;
    M=N;
    px0=(const xtfloatx2 *)x;
    px1=(const ae_int32x2 *)x;
    pz=(      ae_int32x2 *)z;
    ax0 = XT_LASX2PP(px0);
    ax1 = AE_LA64_PP(px1);
    az = AE_ZALIGN64();
    while(M>1)
    {
        N=XT_MIN(M,MAX_ALLOCA_SZ/sizeof(int32_t)) & ~1;
        pScrWr=(xtfloat * )scratch;
        pScrRd=(const ae_int32x2 *)scratch;
        /* first, compute rsqrt */
        for ( n=0; n<(N>>1); n++ )
        {
            xtfloatx2 X;
            xtfloat Yl,Yh;
            XT_LASX2IP(X,ax0,px0);
            Yl=XT_RSQRT_S(XT_LOW_S (X));
            Yh=XT_RSQRT_S(XT_HIGH_S(X));
            XT_SSI (Yl,pScrWr,  sizeof(float32_t));
            XT_SSIP(Yh,pScrWr,2*sizeof(float32_t));
        }
        /* process exceptional cases (<=0, Inf) */
        for ( n=0; n<(N>>1); n++ )
        {
            ae_int32x2 X,Y,Y0;
            xtbool2 blesszero,binf,bzero;
            AE_LA32X2_IP(X,ax1,px1);
            bzero    =AE_EQ32(AE_AND32(X,0x7fffffff),0);
            blesszero=AE_LT32(X,0);
            binf     =AE_EQ32(X,0x7f800000);
            Y0=0x7f800000; AE_MOVT32X2(Y0,0xff800000,blesszero);/* +/- inf */
            AE_L32X2_IP(Y,pScrRd,sizeof(ae_int32x2));

            AE_MOVT32X2(Y,0,binf);
            AE_MOVT32X2(Y,(int32_t)qNaNf.u,blesszero);
            AE_MOVT32X2(Y,Y0,bzero);
            AE_SA32X2_IP(Y,az,pz);
        }
        x+=N;
        z+=N;
        M-=N;
    }
    AE_SA64POS_FP(az,pz);
    /* last odd iteration */
    if(M&1)
    {
        xtfloat y,y0,xn;
        xtfloat *py0;
        ae_int32 *pux;
        ae_int32 ux;
        xtbool blesszero,binf,bzero;
        xn=XT_LSI((const xtfloat*)px0,0);
        bzero    =XT_OEQ_S(xn,0.f);
        blesszero=XT_OLT_S(xn,0.f);
        binf     =XT_OEQ_S(xn,plusInff.f);

        pux = (ae_int32 *)(&xn);
        ux = *pux; /* return +/- inf */
        ux = AE_AND32(ux,0x80000000);
        ux = AE_OR32 (ux,0x7f800000);
        py0 = (xtfloat*)(&ux);
        y0 = *py0;

        y=XT_RSQRT_S(xn);
        XT_MOVT_S(y,0.f,binf);
        XT_MOVT_S(y,qNaNf.f,blesszero);
        XT_MOVT_S(y,y0,bzero);
        XT_SSI(y,(xtfloat*)pz,0);
    }
}
#endif
