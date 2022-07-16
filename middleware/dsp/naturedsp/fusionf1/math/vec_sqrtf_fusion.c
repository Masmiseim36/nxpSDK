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
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, vec_sqrtf, (float32_t * restrict z, const float32_t * restrict x, int N))
#else
#include <xtensa/tie/xt_FP.h>
#include "inff_tbl.h"

/*-------------------------------------------------------------------------
  Square Root
  These routines calculate square root.
  NOTES: 
  1. Fixed point functions return 0x80000000 (for 24 and 32-bit functions), 
     0x8000 (for 16-bit functions) on negative argument
  2. For floating point function, whenever an input value is negative, 
     functions raise the "invalid" floating-point exception, assign EDOM 
     to errno and set output value to NaN. Negative zero is considered as
     a valid input, the result is also -0
  Two versions of functions available: regular version (vec_sqrt16x16, 
  vec_sqrt24x24, vec_sqrt32x32, vec_sqrtf) with arbitrary arguments and
  faster version (vec_sqrt24x24_fast, vec_sqrt32x32_fast) that apply some 
  restrictions.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: (2 LSB)
  24x24  24-bit inputs, 24-bit output. Accuracy: (2.6e-7*y+1LSB)
  32x32  32-bit inputs, 32-bit output. Accuracy: (2.6e-7*y+1LSB)
  f      floating point. Accuracy 1 ULP

  Input:
  x[N]  input data,Q31, Q15 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31, Q15 or floating point

  Restriction:
  Regular versions (vec_sqrt24x24, vec_sqrt32x32):
  x,y - should not overlap

  Faster versions (vec_sqrt24x24_fast, vec_sqrt32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result, Q31, Q15 or floating point
-------------------------------------------------------------------------*/
void vec_sqrtf(float32_t * restrict z, const float32_t * restrict x,  int N )
#if 0
{
  int n;

  NASSERT( x );
  NASSERT( z );

  for ( n=0; n<N; n++ )
  {
    z[n] = XT_SQRT_S( x[n] );
  }

} /* vec_sqrtf() */
#else
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
        /* first, compute sqrt */
        for ( n=0; n<(N>>1); n++ )
        {
            xtfloatx2 X;
            xtfloat Rl,Rh,Xl,Xh,Yl,Yh,Dl,Dh;
            XT_LASX2IP(X,ax0,px0);
            Xl=XT_LOW_S (X);
            Xh=XT_HIGH_S(X);
            Rl=XT_RSQRT_S(Xl);
            Rh=XT_RSQRT_S(Xh);
            /* last iteration for sqrt:
            y1=y0+delta
            (y0+delta)^2=x
            2*y0*delta=x-y0^2
            delta=0.5*(x-y0^2)/y0
            delta=r*0.5*(x-y0^2)
            */
            Yl=XT_MUL_S(Rl,Xl);
            Dl=Xl; XT_MSUB_S(Dl,Yl,Yl);
            XT_MADD_S(Yl,XT_MUL_S(XT_CONST_S(3),Rl),Dl);
            Yh=XT_MUL_S(Rh,Xh);
            Dh=Xh; XT_MSUB_S(Dh,Yh,Yh);
            XT_MADD_S(Yh,XT_MUL_S(XT_CONST_S(3),Rh),Dh);
            XT_SSI (Yl,pScrWr,  sizeof(float32_t));
            XT_SSIP(Yh,pScrWr,2*sizeof(float32_t));
        }
        /* process exceptional cases (<=0, Inf) */
        for ( n=0; n<(N>>1); n++ )
        {
            ae_int32x2 X,Y;
            xtbool2 binf,bzero;
            AE_LA32X2_IP(X,ax1,px1);
            bzero    =AE_EQ32(AE_AND32(X,0x7fffffff),0);
            binf     =AE_EQ32(X,0x7f800000);
            AE_L32X2_IP(Y,pScrRd,sizeof(ae_int32x2));
            AE_MOVT32X2(Y,X,binf);
            AE_MOVT32X2(Y,X,bzero);
            AE_SA32X2_IP(Y,az,pz);
        }
        M-=N;
    }
    AE_SA64POS_FP(az,pz);
    /* last off iteration */
    if(M) 
    {
        xtfloat xn,r,y,d;
        xtbool binf,bzero;
        xn=XT_LSI((const xtfloat*)px0,0);
        binf =XT_OEQ_S(xn,plusInff.f);
        bzero=XT_OEQ_S(xn,0.f);
        r = XT_RSQRT_S( xn );
        y=XT_MUL_S(r,xn);
        /* last iteration for sqrt:
        y1=y0+delta
        (y0+delta)^2=x
        2*y0*delta=x-y0^2
        delta=0.5*(x-y0^2)/y0
        delta=r*0.5*(x-y0^2)
        */
        d=xn; XT_MSUB_S(d,y,y);
        XT_MADD_S(y,XT_MUL_S(XT_CONST_S(3),r),d);
        XT_MOVT_S(y,xn,XT_ORB(binf,bzero));
        XT_SSI(y,(xtfloat*)pz,0);
  }
}
#endif
#endif
