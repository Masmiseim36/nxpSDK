/* ------------------------------------------------------------------------ */
/* Copyright (c) 2019 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2019 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"
#include "img_getCoef_up_cubic.h"
#include "divide_q31.h"
#include "cubic_kernel.h"

/* coefficients for bilinear upsampler with ratio 1..2 
   M - input length, N - output length, 1<N/M<2
*/
size_t img_getCoef_up_cubic_alloc(int M,int N)
{
    int N0=(N+3)&~3;
    size_t sz;
    NASSERT(N>M && N<M*2);
    sz =(sizeof(img_coefup_cubic_t)+ALIGNMENT-1)&(~(ALIGNMENT-1));
    sz+=(sizeof(int16_t)*4*N0 +ALIGNMENT-1)&(~(ALIGNMENT-1));
    sz+=(sizeof(int16_t)*1*N0 +ALIGNMENT-1)&(~(ALIGNMENT-1));
    sz+=(sizeof(int32_t)*1*N0 +ALIGNMENT-1)&(~(ALIGNMENT-1));
    return sz;
}

/* function returns w(N,2) coefficients and left(N) indices
  for upsampler
  M - input length, N - output length, 1<N/M<2
*/
img_coefup_cubic_t* img_getCoef_up_cubic_init( void* objmem,int M,int N)
{
    int16_t   *w;
    int16_t   *left;

          ae_int16x4 * restrict pWwr;
    const ae_int16x4 * restrict pWrd;
          ae_int32x2* restrict pNorm;
          ae_int16  * restrict pLeft;

    int N0=(N+3)&~3;
    int32_t* norm;
    int32_t MN_q31;
    int n;
    img_coefup_cubic_t* pObj=(img_coefup_cubic_t*)objmem;
    uintptr_t a=(uintptr_t )(pObj+1);
    NASSERT_ALIGN(pObj,ALIGNMENT);
    a=(a+ALIGNMENT-1)&(~(ALIGNMENT-1));
    w   =pObj->coef=(int16_t*)a;
    a=(uintptr_t)(w+4*N0);
    a=(a+ALIGNMENT-1)&(~(ALIGNMENT-1));
    left=pObj->left=(int16_t*)a;
    a=(uintptr_t)(left+1*N0);
    a=(a+ALIGNMENT-1)&(~(ALIGNMENT-1));
    norm=(int32_t*)a;
    NASSERT_ALIGN(w   ,ALIGNMENT);
    NASSERT_ALIGN(left,ALIGNMENT);
    NASSERT(N>M && N<M*2);
    MN_q31= divide_q31(M,N);    // 0.5...1
    /* compute left-most pixel involved in the filtering and weights */
#if 0
    for (n=0; n<N0; n++)
    {
        int m;
        int32_t u_q14,w0_q14,w1_q14,w2_q14,w3_q14;
        u_q14=L_mpy_ll( (n<<14)+(1<<13),MN_q31)+(1<<13);
        m=(u_q14>>14)-2;
        w0_q14=u_q14-(m<<14);
        w0_q14=w0_q14-16384;
        w1_q14=w0_q14-16384;
        w2_q14=w1_q14-16384;
        w3_q14=w2_q14-16384;
        w[4*n+0]=S_sature_l(w0_q14);
        w[4*n+1]=S_sature_l(w1_q14);
        w[4*n+2]=S_sature_l(w2_q14);
        w[4*n+3]=S_sature_l(w3_q14);
        left[n]=m;
    }
#else
    pLeft=(ae_int16*)left;
    pWwr=(ae_int16x4 *)w;
    for (n=0; n<N0; n++)
    {
        ae_int32x2 w01,w23,tu,tm;
        tu=(1<<13); AE_MULAFP32X2RAS(tu,MN_q31,(n<<14)+(1<<13));
        tm=AE_SUB32(AE_SRAI32(tu,14),2);
        w01=AE_SUB32(tu,AE_SLAI32(tm,14));
        w01=AE_SUB32(w01,AE_MOVDA32X2((int32_t)16384,(int32_t)32768));
        w23=AE_SUB32(w01,AE_MOVDA32((int32_t)32768));
        AE_S16X4_IP(AE_SAT16X4(w01,w23),pWwr,sizeof(ae_int16x4));
        AE_S16_0_IP(AE_SAT16X4(tm,tm),pLeft,sizeof(int16_t));
    }
#endif
    /* cubic kernel, input Q14, output Q15 */
    cubic_kernel(w,N0*4);
    /* normalization */
#if 0
    for (n=0; n<N0; n++)
    {
        norm[n]=(int32_t)w[4*n+0]+
            (int32_t)w[4*n+1]+
            (int32_t)w[4*n+2]+
            (int32_t)w[4*n+3];
    }
#else
    __Pragma("no_reorder")
    pWrd=(const ae_int16x4 *)w;
    pNorm=(ae_int32x2*)norm;
    for (n=0; n<(N0>>2); n++)
    {
        ae_int16x4 w0,w1,w2,w3;
        ae_int64 q0,q1,q2,q3;
        AE_L16X4_IP(w0,pWrd,sizeof(ae_int16x4));
        AE_L16X4_IP(w1,pWrd,sizeof(ae_int16x4));
        AE_L16X4_IP(w2,pWrd,sizeof(ae_int16x4));
        AE_L16X4_IP(w3,pWrd,sizeof(ae_int16x4));
        q0=AE_MULZAAAAQ16(w0,AE_MOVDA16(1)); 
        q1=AE_MULZAAAAQ16(w1,AE_MOVDA16(1));
        q2=AE_MULZAAAAQ16(w2,AE_MOVDA16(1)); 
        q3=AE_MULZAAAAQ16(w3,AE_MOVDA16(1));
        AE_S32X2_IP(AE_TRUNCA32X2F64S(q0,q1,32),pNorm,sizeof(ae_int32x2));
        AE_S32X2_IP(AE_TRUNCA32X2F64S(q2,q3,32),pNorm,sizeof(ae_int32x2));
    }
#endif
    recip_qx((int32_t)16384,norm,N0);
#if 0
    for (n=0; n<N0; n++)
    {
        w[4*n+0]=S_round_l(L_mpy_ls(norm[n],w[4*n+0]));
        w[4*n+1]=S_round_l(L_mpy_ls(norm[n],w[4*n+1]));
        w[4*n+2]=S_round_l(L_mpy_ls(norm[n],w[4*n+2]));
        w[4*n+3]=S_round_l(L_mpy_ls(norm[n],w[4*n+3]));
    }
#else
    __Pragma("no_reorder")
    pWwr=(      ae_int16x4 *)w;
    pWrd=(const ae_int16x4 *)w;
    pNorm=(ae_int32x2*)norm;
    for (n=0; n<N0; n++)
    {
        ae_int16x4 w0;
        ae_int32x2 y;
        AE_L32_IP(y,castxcc(ae_int32,pNorm),sizeof(ae_int32));
        AE_L16X4_IP(w0,pWrd,sizeof(ae_int16x4));
        w0=AE_ROUND16X4F32SASYM(
             AE_MULFP32X16X2RAS_H(y,w0),
             AE_MULFP32X16X2RAS_L(y,w0));
        AE_S16X4_IP(w0,pWwr,sizeof(ae_int16x4));
    }
#endif
    /* modify first and last indices and weights */
#if 0
    n=0;
    while(n<N && left[n]<0)
    {
        while(left[n]<0)
        {
            w[4*n+0]=S_add_ss(w[4*n+0],w[4*n+1]);
            w[4*n+1]=w[4*n+2];
            w[4*n+2]=w[4*n+3];
            w[4*n+3]=0;
            left[n]++;
        }
        n++;
    }
    n=N-1;
    while(n>=0 && left[n]>M-4)
    {
        while(left[n]>M-4)
        {
            w[4*n+3]=S_add_ss(w[4*n+3],w[4*n+2]);
            w[4*n+2]=w[4*n+1];
            w[4*n+1]=w[4*n+0];
            w[4*n+0]=0;
            left[n]--;
        }
        n--;
    }
#else
    __Pragma("no_reorder")
    n=0;
    pWwr=(      ae_int16x4 *)w;
    while(n<N )
    {
        ae_int16x4 w0123,w0,ws;
        int m;
        m=left[n];
        if (m>=0) break;
        w0123=AE_L16X4_I(pWwr,0*sizeof(ae_int16x4));
        while(m<0)
        {
            w0=AE_SEL16_6543(w0123,0);
            ws=AE_ADD16S(w0123,w0);
            AE_MOVT16X4(w0,ws,AE_MOVAB4(1<<3));
            w0123=w0;
            m++;
        }
        AE_S16X4_IP(w0123,pWwr,sizeof(ae_int16x4));
        left[n++]=m;
    }
    n=N-1;
    pWwr=(      ae_int16x4 *)(w+4*n);
    while(n>=0 )
    {
        ae_int16x4 w0123,w0,ws;
        int m;
        m=left[n];
        if (m<=M-4) break;
        w0123=AE_L16X4_I(pWwr,0*sizeof(ae_int16x4));
        while(m>M-4)
        {
            w0=AE_SEL16_4321(0,w0123);
            ws=AE_ADD16S(w0123,w0);
            AE_MOVT16X4(w0,ws,AE_MOVAB4(1));
            w0123=w0;
            m--;
        }
        AE_S16X4_XP(w0123,pWwr,-1*(int)sizeof(ae_int16x4));
        left[n--]=m;
    }
#endif
    return pObj;
}
