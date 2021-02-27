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

/*
  NatureDSP Signal Processing Library. FIR part
    helper for correlation/convolution
    C code optimized for Fusion
*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "raw_corr24x24.h"

/*-----------------------------------------------------
    raw correlation:
    Input:
    x[N+M-1] padded with extra 3 zeroes
    y[M]
    Output:
    r[N]
    restriction:
    M should be a multiple of 2 and >0
-----------------------------------------------------*/
void raw_corr24x24(f24* r, const f24* restrict x, const f24* restrict y,int N, int M)
{
    const ae_f24x2 * restrict pX0;
    const ae_f24x2 * restrict pX1;
    const ae_f24x2 * restrict pY;
            ae_f24x2 * restrict pR;
    ae_f24x2 X01,X12,Y01,d0;
    ae_f64 a0,a1,a2,a3;
    ae_valign ax0,ar;
    int n, m;
    NASSERT(M>0);
    NASSERT(M%2==0);
      /* compute N&~3 correlation results */
    ar=AE_ZALIGN64();
    pR=(ae_f24x2*)r;
    for ( n=0; n<(N&~3); n+=4 )
    {
        a0=a1=a2=a3=AE_ZERO();
        pY=(const ae_f24x2*)y;
        pX0=(const ae_f24x2*)(x+n);
        pX1=(const ae_f24x2*)(x+n+1);
        ax0=AE_LA64_PP(pX1);
        AE_L32X2F24_IP (X01,pX0,sizeof(int32_t)*2);
        AE_LA32X2F24_IP(X12,ax0,pX1);
        __Pragma("loop_count min=1")
        for ( m=0; m<M; m+=2 )
        {
            AE_L32X2F24_IP(Y01,pY,sizeof(int32_t)*2);
            AE_MULAAFD24_HH_LL(a0,Y01,X01);
            AE_MULAAFD24_HH_LL(a1,Y01,X12);
            AE_L32X2F24_IP (X01,pX0,sizeof(int32_t)*2);
            AE_LA32X2F24_IP(X12,ax0,pX1);
            AE_MULAAFD24_HH_LL(a2,Y01,X01);
            AE_MULAAFD24_HH_LL(a3,Y01,X12);
        }
        d0 = AE_ROUND24X2F48SASYM( a0, a1 ); AE_SA32X2F24_IP( d0, ar,pR);
        d0 = AE_ROUND24X2F48SASYM( a2, a3 ); AE_SA32X2F24_IP( d0, ar,pR);
    }
    AE_SA64POS_FP(ar,pR);
    /* process last 1...3 samples */
    N&=3;
    if(N&2)
    {
      a0 = a1 = AE_ZERO();
      pY = (const ae_f24x2*)y;
      pX0 = (const ae_f24x2*)(x + n);
      pX1 = (const ae_f24x2*)(x + n + 1);
      ax0 = AE_LA64_PP(pX1);
      n=n+2;
      __Pragma("loop_count min=1")
        for ( m=0; m<M; m+=2 )
        {
          AE_L32X2F24_IP(Y01, pY, sizeof(int32_t) * 2);
          AE_L32X2F24_IP(X01, pX0, sizeof(int32_t) * 2);
          AE_LA32X2F24_IP(X12, ax0, pX1);
          AE_MULAAFD24_HH_LL(a0, Y01, X01);
          AE_MULAAFD24_HH_LL(a1, Y01, X12);
        }
      d0 = AE_ROUND24X2F48SASYM(a0, a1); AE_SA32X2F24_IP(d0, ar, pR);
    }
    AE_SA64POS_FP(ar, pR);
    if(N&1)
    {
      a0 = AE_ZERO();
      pY = (const ae_f24x2*)y;
      pX0 = (const ae_f24x2*)(x + n);
      __Pragma("loop_count min=1")
       for (m = 0; m<M; m += 2)
        {
          AE_L32X2F24_IP(Y01, pY, sizeof(int32_t) * 2);
          AE_L32X2F24_IP(X01, pX0, sizeof(int32_t) * 2);
          AE_MULAAFD24_HH_LL(a0, Y01, X01);
        }
      d0 = AE_ROUND24X2F48SASYM(a0, a0);
      AE_S32F24_L_IP(AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(d0)), castxcc(ae_f24, pR), 4);
    }
} /* raw_corr24x24() */
