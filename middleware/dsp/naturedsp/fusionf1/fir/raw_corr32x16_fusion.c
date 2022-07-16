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
#include "raw_corr32x16.h"

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
void raw_corr32x16(int32_t* r, const int32_t* restrict x, const int16_t* restrict y,int N, int M)
{
    const ae_int32x2 * restrict pX0;
    const ae_int32x2 * restrict pX1;
    const int32_t * restrict pY;
            ae_f24x2 * restrict pR;
    ae_int32x2 X01,X12;
    ae_f16x4 Y01;
    ae_f64 a0,a1,a2,a3;
    ae_valign ax0;
    int n, m;
    NASSERT(M>0);
    NASSERT(M%2==0);
      /* compute N&~3 correlation results */
    pR=(ae_f24x2*)r;
    for ( n=0; n<(N&~3); n+=4 )
    {
        a0=a1=a2=a3=AE_ZERO();
        pY=(const int32_t*)y;
        pX0=(const ae_int32x2*)(x+n);
        pX1=(const ae_int32x2*)(x+n+1);
        ax0=AE_LA64_PP(pX1);
        AE_L32X2_IP (X01,pX0,sizeof(int32_t)*2);
        AE_LA32X2_IP(X12,ax0,pX1);
        __Pragma("loop_count min=1")
        for ( m=0; m<M; m+=2 )
        {
            ae_int32x2 t;
            AE_L32_IP(t, castxcc(const ae_int32, pY), sizeof(int32_t)); Y01 = AE_MOVF16X4_FROMINT32X2(t);
            AE_MULAAFD32X16_H2_L3(a0,X01,Y01);
            AE_MULAAFD32X16_H2_L3(a1,X12,Y01);
            AE_L32X2_IP (X01,pX0,sizeof(int32_t)*2);
            AE_LA32X2_IP(X12,ax0,pX1);
            AE_MULAAFD32X16_H2_L3(a2,X01,Y01);
            AE_MULAAFD32X16_H2_L3(a3,X12,Y01);
        }
        AE_S32RA64S_IP(a0, castxcc(ae_f32, pR), sizeof(int32_t));
        AE_S32RA64S_IP(a1, castxcc(ae_f32, pR), sizeof(int32_t));
        AE_S32RA64S_IP(a2, castxcc(ae_f32, pR), sizeof(int32_t));
        AE_S32RA64S_IP(a3, castxcc(ae_f32, pR), sizeof(int32_t));
    }
    /* process last 1...3 samples */
    N&=3;
    if(N&2)
    {
      a0 = a1 = AE_ZERO();
      pY = (const int32_t*)y;
      pX0=(const ae_int32x2*)(x+n);
      pX1=(const ae_int32x2*)(x+n+1);
      ax0=AE_LA64_PP(pX1);
      
      n=n+2;
      __Pragma("loop_count min=1")
      for ( m=0; m<M; m+=2 )
      {
        ae_int32x2 t;
        AE_L32X2_IP(X01, pX0, sizeof(int32_t) * 2);
        AE_LA32X2_IP(X12, ax0, pX1);
        AE_L32_IP(t, castxcc(ae_int32, pY), sizeof(int32_t)); Y01 = AE_MOVF16X4_FROMINT32X2(t);
        AE_MULAAFD32X16_H2_L3(a0,X01,Y01);
        AE_MULAAFD32X16_H2_L3(a1,X12,Y01);
          
      }
      AE_S32RA64S_IP(a0, castxcc(ae_f32, pR), sizeof(int32_t));
      AE_S32RA64S_IP(a1, castxcc(ae_f32, pR), sizeof(int32_t));
    }
    if(N&1)
    {
      a0 = AE_ZERO();
      pY = (const int32_t*)y;
      pX0=(const ae_int32x2*)(x+n);
      __Pragma("loop_count min=1")
        for ( m=0; m<M; m+=2 )
        {
          ae_int32x2 t;
          AE_L32_IP(t, castxcc(ae_int32, pY), sizeof(int32_t)); Y01 = AE_MOVF16X4_FROMINT32X2(t);
          AE_L32X2_IP(X01, pX0, sizeof(int32_t) * 2);
          AE_MULAAFD32X16_H2_L3(a0, X01, Y01);
        }
      AE_S32RA64S_IP(a0, castxcc(ae_f32, pR), sizeof(int32_t));
    }
}
