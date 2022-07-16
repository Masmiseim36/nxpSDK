/* ------------------------------------------------------------------------ */
/* Copyright (c) 2020 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2020 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"
#include "img_getCoef_dn_cubic.h"
#include "divide_q31.h"
#include "cubic_kernel.h"
/* coefficients for cubic downsampler with ratio 1..2 
   M - input length, N - output length, 1<M/N<2
*/

size_t img_getCoef_dn_cubic_alloc(int M,int N)
{
    size_t sz;
    int N0=(N+3)&~3;
    NASSERT(M>N && M<N*2);
    sz=(sizeof(img_coefdn_cubic_t)+ALIGNMENT-1)&(~(ALIGNMENT-1));
    sz+=(sizeof(int16_t)*8*N0 +ALIGNMENT-1)&(~(ALIGNMENT-1));
    sz+=(sizeof(int16_t)*1*N0 +ALIGNMENT-1)&(~(ALIGNMENT-1));
    sz+=(sizeof(int32_t)*1*N0 +ALIGNMENT-1)&(~(ALIGNMENT-1));   // temporary array size will be a multiple of 4
    return sz;
}

/* function returns w(N,4) coefficients and left(N) indices
  for downsampler
  M - input length, N - output length, 1<M/N<2
*/
img_coefdn_cubic_t* img_getCoef_dn_cubic_init(void* objmem,int M,int N)
{
    int32_t  *norm;
    int16_t  *w;
    int16_t  *left;

          ae_int16x4 * restrict pWwr;
    const ae_int16x4 * restrict pWrd;
          ae_int32x2* restrict pNorm;
          ae_int16  * restrict pLeft;

    int n;
    int N0=(N+3)&~3;
    int32_t M2N_q31,scale_q31;
    img_coefdn_cubic_t* pObj=(img_coefdn_cubic_t*)objmem;
    uintptr_t a=(uintptr_t )(pObj+1);
    NASSERT_ALIGN(pObj,ALIGNMENT);
    a=(a+ALIGNMENT-1)&(~(ALIGNMENT-1));
    w   =pObj->coef=(int16_t*)a;
    a=(uintptr_t)(w+8*N0);
    a=(a+ALIGNMENT-1)&(~(ALIGNMENT-1));
    left=pObj->left=(int16_t*)a;
    a=(uintptr_t)(left+1*N0);
    a=(a+ALIGNMENT-1)&(~(ALIGNMENT-1));
    norm=(int32_t*)a;
    NASSERT_ALIGN(w   ,ALIGNMENT);
    NASSERT_ALIGN(left,ALIGNMENT);
    NASSERT(M>N && M<N*2);
    M2N_q31=divide_q31(M,N<<1);// 0.5..1
    scale_q31=divide_q31(N,M); // 0.5..1
    /* compute left-most pixel involved in the filtering and weights for kernel */
#if 0
    for (n=0; n<N0; n++)
    {
        int m;
        int32_t u_q14,w0_q14,w1_q14,w2_q14,w3_q14,w4_q14,w5_q14,w6_q14,w7_q14;
        u_q14=(L_mpy_ll(L_deposit_s(2*n+1),M2N_q31)+(int32_t)32768)>>2;
        m= (L_mpy_ll(L_deposit_s(2*n+1-4),M2N_q31)+(int32_t)32768)>>16; 
        w0_q14=u_q14-(((int32_t)m)<<14);
        w0_q14=w0_q14-16384;
        w1_q14=w0_q14-16384;
        w2_q14=w1_q14-16384;
        w3_q14=w2_q14-16384;
        w4_q14=w3_q14-16384;
        w5_q14=w4_q14-16384;
        w6_q14=w5_q14-16384;
        w7_q14=w6_q14-16384;
        w[8*n+0]=S_sature_l(L_mpy_ll(w0_q14,scale_q31));
        w[8*n+1]=S_sature_l(L_mpy_ll(w1_q14,scale_q31));
        w[8*n+2]=S_sature_l(L_mpy_ll(w2_q14,scale_q31));
        w[8*n+3]=S_sature_l(L_mpy_ll(w3_q14,scale_q31));
        w[8*n+4]=S_sature_l(L_mpy_ll(w4_q14,scale_q31));
        w[8*n+5]=S_sature_l(L_mpy_ll(w5_q14,scale_q31));
        w[8*n+6]=S_sature_l(L_mpy_ll(w6_q14,scale_q31));
        w[8*n+7]=S_sature_l(L_mpy_ll(w7_q14,scale_q31));
        left[n]=m;
    }
#else
    pLeft=(ae_int16*)left;
    pWwr=(ae_int16x4 *)w;
    for (n=0; n<N0; n++)
    {
        ae_int32x2 w01,w23,w45,w67,tu,tm;
        ae_int16x4 w0123,w4567;
        tu=AE_MOVDA32((int32_t)32768); AE_MULAFP32X2RAS(tu,M2N_q31,(2*n+1)<<16); tu=AE_SRAI32(tu,2);
        tm=AE_MOVDA32((int32_t)32768); AE_MULAFP32X2RAS(tm,M2N_q31,(2*n-3)<<16); tm=AE_SRAI32(tm,16);
        w01=AE_SUB32(tu,AE_SLAI32(tm,14));
        AE_S16_0_IP(AE_SAT16X4(tm,tm),pLeft,sizeof(int16_t));
        w01=AE_SUB32(w01,AE_MOVDA32X2(         16384,(int32_t)32768));
        w23=AE_SUB32(w01,AE_MOVDA32((int32_t)32768));
        w45=AE_SUB32(w23,AE_MOVDA32((int32_t)32768));
        w67=AE_SUB32(w45,AE_MOVDA32((int32_t)32768));
        w0123=AE_SAT16X4(AE_MULFP32X2RAS(w01,scale_q31),AE_MULFP32X2RAS(w23,scale_q31));
        w4567=AE_SAT16X4(AE_MULFP32X2RAS(w45,scale_q31),AE_MULFP32X2RAS(w67,scale_q31));
        AE_S16X4_IP(w0123,pWwr,sizeof(ae_int16x4));
        AE_S16X4_IP(w4567,pWwr,sizeof(ae_int16x4));
    }
#endif
    /* compute cubic kernel: input in Q14, output in Q15 */
    cubic_kernel(w,N0*8);
    /* normalization */
#if 0
    for (n=0; n<N0; n++)
    {
        norm[n]=
            (int32_t)w[8*n+0]+
            (int32_t)w[8*n+1]+
            (int32_t)w[8*n+2]+
            (int32_t)w[8*n+3]+
            (int32_t)w[8*n+4]+
            (int32_t)w[8*n+5]+
            (int32_t)w[8*n+6]+
            (int32_t)w[8*n+7];
    }
#else
    __Pragma("no_reorder")
    pWrd=(const ae_int16x4 *)w;
    pNorm=(ae_int32x2*)norm;
    for (n=0; n<N0/2; n++)
    {
        ae_int16x4 w0,w1,w2,w3;
        ae_int64 q0,q1;
        AE_L16X4_IP(w0,pWrd,sizeof(ae_int16x4));
        AE_L16X4_IP(w1,pWrd,sizeof(ae_int16x4));
        AE_L16X4_IP(w2,pWrd,sizeof(ae_int16x4));
        AE_L16X4_IP(w3,pWrd,sizeof(ae_int16x4));
        q0=AE_MULZAAAAQ16(w0,AE_MOVDA16(1)); AE_MULAAAAQ16(q0,w1,AE_MOVDA16(1));
        q1=AE_MULZAAAAQ16(w2,AE_MOVDA16(1)); AE_MULAAAAQ16(q1,w3,AE_MOVDA16(1));
        AE_S32X2_IP(AE_TRUNCA32X2F64S(q0,q1,32),pNorm,sizeof(ae_int32x2));
    }
#endif
    recip_qx((int32_t)32768,norm,N0);
#if 0
    for (n=0; n<N0; n++)
    {
        w[8*n+0]=S_round_l(L_mpy_ls(norm[n],w[8*n+0]));
        w[8*n+1]=S_round_l(L_mpy_ls(norm[n],w[8*n+1]));
        w[8*n+2]=S_round_l(L_mpy_ls(norm[n],w[8*n+2]));
        w[8*n+3]=S_round_l(L_mpy_ls(norm[n],w[8*n+3]));
        w[8*n+4]=S_round_l(L_mpy_ls(norm[n],w[8*n+4]));
        w[8*n+5]=S_round_l(L_mpy_ls(norm[n],w[8*n+5]));
        w[8*n+6]=S_round_l(L_mpy_ls(norm[n],w[8*n+6]));
        w[8*n+7]=S_round_l(L_mpy_ls(norm[n],w[8*n+7]));
    }
#else
    __Pragma("no_reorder")
    pWwr=(      ae_int16x4 *)w;
    pWrd=(const ae_int16x4 *)w;
    pNorm=(ae_int32x2*)norm;
    for (n=0; n<N0; n++)
    {
        ae_int16x4 w0,w1;
        ae_int32x2 y;
        AE_L32_IP(y,castxcc(ae_int32,pNorm),sizeof(ae_int32));
        AE_L16X4_IP(w0,pWrd,sizeof(ae_int16x4));
        AE_L16X4_IP(w1,pWrd,sizeof(ae_int16x4));
        w0=AE_ROUND16X4F32SASYM(
             AE_MULFP32X16X2RAS_H(y,w0),
             AE_MULFP32X16X2RAS_L(y,w0));
        w1=AE_ROUND16X4F32SASYM(
             AE_MULFP32X16X2RAS_H(y,w1),
             AE_MULFP32X16X2RAS_L(y,w1));
        AE_S16X4_IP(w0,pWwr,sizeof(ae_int16x4));
        AE_S16X4_IP(w1,pWwr,sizeof(ae_int16x4));
    }
#endif
    // modify first and last indices and weights 
    n=0;
#if 0
    while(n<N && left[n]<0)
    {
        while(left[n]<0)
        {
            w[8*n+0]=S_add_ss(w[8*n+0],w[8*n+1]);
            w[8*n+1]=w[8*n+2];
            w[8*n+2]=w[8*n+3];
            w[8*n+3]=w[8*n+4];
            w[8*n+4]=w[8*n+5];
            w[8*n+5]=w[8*n+6];
            w[8*n+6]=w[8*n+7];
            w[8*n+7]=0;
            left[n]++;
        }
        n++;
    }
    n=N-1;
    while(n>=0)
    {
        int m;
        m=left[n];
        if (m<=M-8) break;
        while(m>M-8)
        {
            w[8*n+7]=S_add_ss(w[8*n+7],w[8*n+6]);
            w[8*n+6]=w[8*n+5];
            w[8*n+5]=w[8*n+4];
            w[8*n+4]=w[8*n+3];
            w[8*n+3]=w[8*n+2];
            w[8*n+2]=w[8*n+1];
            w[8*n+1]=w[8*n+0];
            w[8*n+0]=0;
            m--;
        }
        left[n--]=m;
    }
#else
    __Pragma("no_reorder")
    pWwr=(      ae_int16x4 *)w;
    while(n<N)
    {
        ae_int16x4 w0123,w4567,w0,w1,ws;
        int m=left[n];
        if (m>=0) break;
        w0123=AE_L16X4_I(pWwr,0*sizeof(ae_int16x4));
        w4567=AE_L16X4_I(pWwr,1*sizeof(ae_int16x4));
        while(m<0)
        {
            w0=AE_SEL16_6543(w0123,w4567);
            w1=AE_SEL16_6543(w4567,0);
            ws=AE_ADD16S(w0123,w0);
            AE_MOVT16X4(w0,ws,AE_MOVAB4(1<<3));
            w0123=w0;
            w4567=w1;
            m++;
        }
        AE_S16X4_IP(w0123,pWwr,sizeof(ae_int16x4));
        AE_S16X4_IP(w4567,pWwr,sizeof(ae_int16x4));
        left[n++]=m;
    }
    n=N-1;
    pWwr=(      ae_int16x4 *)(w+8*n);
    while(n>=0)
    {
        ae_int16x4 w0123,w4567,w0,w1,ws;
        int m;
        m=left[n];
        if (m<=M-8) break;
        w0123=AE_L16X4_I(pWwr,0*sizeof(ae_int16x4));
        w4567=AE_L16X4_I(pWwr,1*sizeof(ae_int16x4));
        while(m>M-8)
        {
            w0=AE_SEL16_4321(    0,w0123);
            w1=AE_SEL16_4321(w0123,w4567);
            ws=AE_ADD16S(w4567,w1);
            AE_MOVT16X4(w1,ws,AE_MOVAB4(1));
            w0123=w0;
            w4567=w1;
            m--;
        }
        AE_S16X4_I (w4567,pWwr, 1*sizeof(ae_int16x4));
        AE_S16X4_XP(w0123,pWwr,-2*(int)sizeof(ae_int16x4));
        left[n--]=m;
    }
#endif
    return pObj;
}
