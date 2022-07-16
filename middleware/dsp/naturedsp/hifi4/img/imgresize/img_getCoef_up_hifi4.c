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
#include "img_getCoef_up.h"
#include "divide_q31.h"
/* coefficients for bilinear upsampler with ratio 1..2 
   M - input length, N - output length, 1<N/M<2
*/
size_t img_getCoef_up_alloc(int M,int N)
{
    int N0=(N+3)&~3;
    size_t sz;
    NASSERT(N>M && N<M*2);
    sz =(sizeof(img_coefup_t)+ALIGNMENT-1)&(~(ALIGNMENT-1));
    sz+=(sizeof(int16_t)*2*N0 +ALIGNMENT-1)&(~(ALIGNMENT-1));
    sz+=(sizeof(int16_t)*1*N0 +ALIGNMENT-1)&(~(ALIGNMENT-1));
    return sz;
}

/* function returns w(N,2) coefficients and left(N) indices
  for upsampler
  M - input length, N - output length, 1<N/M<2
*/
img_coefup_t* img_getCoef_up_init( void* objmem,int M,int N)
{
    int16_t   *w;
    int16_t   *left;
    ae_int16x4 * restrict pWwr;
    ae_int16  * restrict pLeft;

    int N0=(N+3)&~3;
    int32_t MN_q31;
    int n;
    img_coefup_t* pObj=(img_coefup_t*)objmem;
    uintptr_t a=(uintptr_t )(pObj+1);
    NASSERT_ALIGN(pObj,ALIGNMENT);
    a=(a+ALIGNMENT-1)&(~(ALIGNMENT-1));
    w   =pObj->coef=(int16_t*)a;
    a=(uintptr_t)(w+2*N0);
    a=(a+ALIGNMENT-1)&(~(ALIGNMENT-1));
    left=pObj->left=(int16_t*)a;
    NASSERT_ALIGN(w   ,ALIGNMENT);
    NASSERT_ALIGN(left,ALIGNMENT);
    NASSERT(N>M && N<M*2);

    MN_q31= divide_q31(M,N);
#if 0
    for (n=0; n<N0; n++)
    {
        int32_t u_q15,w0_q15,w1_q15;
        int m;
        // compute left-most pixel involved in the filtering
        u_q15=L_mpy_ll( (n<<15)+(1<<14),MN_q31)+(1<<14);
        m=(u_q15>>15)-1;
        w0_q15=u_q15-(m<<15);
        w0_q15=w0_q15-(int32_t)32768;
        w1_q15=w0_q15-(int32_t)32768;
        // apply triangle window
        w0_q15=(int32_t)32768-MIN((int32_t)32768,L_abs_l(w0_q15));
        w1_q15=(int32_t)32768-MIN((int32_t)32768,L_abs_l(w1_q15));
        left[n]=m;
        w[2*n+0]=S_round_l(L_mpy_ls(L_shl_l(w0_q15,16),32767));
        w[2*n+1]=S_round_l(L_mpy_ls(L_shl_l(w1_q15,16),32767));
        w[2*n+0]=S_sature_l(w0_q15);
        w[2*n+1]=S_sature_l(w1_q15);
    }
#else
    pLeft=(ae_int16*)left;
    pWwr=(ae_int16x4 *)w;
    for (n=0; n<N0; n+=2)
    {
        ae_int32x2 u_q15,w0_q15,w1_q15,m;
        ae_int16x4 w0123;
        // compute left-most pixel involved in the filtering
        u_q15=(1<<14); AE_MULAFP32X2RAS(u_q15,MN_q31,AE_ADD32((1<<14),AE_SLAI32(AE_MOVDA32X2(n,n+1),15)));
        m=AE_SUB32(AE_SRAI32(u_q15,15),1);
        w0_q15=AE_SUB32(u_q15,AE_SLAI32(m,15));
        w0_q15=AE_SUB32(w0_q15,(int32_t)32768);
        w1_q15=AE_SUB32(w0_q15,(int32_t)32768);
        // apply triangle window
        w0_q15=AE_SUB32((int32_t)32768,AE_MIN32((int32_t)32768,AE_ABS32(w0_q15)));
        w1_q15=AE_SUB32((int32_t)32768,AE_MIN32((int32_t)32768,AE_ABS32(w1_q15)));
        w0123=AE_SAT16X4(AE_SEL32_HH(w0_q15,w1_q15),AE_SEL32_LL(w0_q15,w1_q15));
        AE_S16X4_IP(w0123,pWwr,sizeof(ae_int16x4));
        m=AE_SEL32_LH(m,m);
        AE_S32_L_IP(AE_MOVINT32X2_FROMINT16X4(AE_SAT16X4(m,m)),castxcc(ae_int32,pLeft),2*sizeof(int16_t));
    }
#endif
    // modify first N0 and last N1 indices and weights 
    __Pragma("no_reorder")
    n=0;
    while(n<N)
    {
        int m;
        m=left[n];
        if (m>=0) break;
        while(m<0)
        {
            w[2*n+0]=32767;
            w[2*n+1]=0;
            m++;
        }
        left[n++]=m;
    }
    n=N-1;
    while(n>=0)
    {
        int m;
        m=left[n];
        if (m<=M-2) break;
        while(m>M-2)
        {
            w[2*n+1]=32767;
            w[2*n+0]=0;
            m--;
        }
        left[n--]=m;
    }
    return pObj;
}
