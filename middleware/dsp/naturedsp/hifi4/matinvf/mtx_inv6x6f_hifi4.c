/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
 * Real Matrix Inversion, 6x6
 * Optimized code for HiFi4
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
#include "common_fpu.h"
/* Matrix functions */
#include "NatureDSP_Signal_matinv.h"
#include <math.h>

#if (HAVE_VFPU==0 && HAVE_FPU==0)
DISCARD_FUN(void,mtx_inv6x6f,(void* pScr,float32_t* x))
size_t mtx_inv6x6f_getScratchSize        () 
{
    return 0;
}
#elif (HAVE_VFPU)
/*-------------------------------------------------------------------------
  These functions implement in-place matrix inversion by Gauss elimination 
  with full pivoting
  NOTE: user may detect "invalid" or "divide-by-zero" exception in the CPU 
  flags which MAY indicate that inversion results are not accurate. Also 
  it's responsibility of the user to provide valid input matrix for 
  inversion.
  Fixed point version takes representation of input matrix and forms 
  representation of output matrix with proper scaling.

  Precision: 
  f     floating point
  32x32 32-bit input, 32-bit output

  Input:
  x[N*N]      input matrix
  qX          input matrix representation (for fixed point API only)
  Output:
  x[N*N]      result
  Temporary:
  pScr        scratch memory. Size in bytes is defined by corresponding 
              scratch allocation function 
  N is 2,3,4,6,8,10

  Returned value: floating functions return none, fixed point functions 
                  return fixed-point representation of inverted matrix
  Restrictions:
  none
-------------------------------------------------------------------------*/
void mtx_inv6x6f(void * pScr,float32_t* x)
{
    const int N=6;
    int n,k;
    float32_t* A=(float32_t* )pScr;//[N*N*2];

    xtfloatx2 * restrict pA;
    xtfloatx2 * restrict pAwr;
    xtfloatx2 * restrict pX;
    /* Set bounds of the buffer */
    WUR_AE_CBEGIN0((uintptr_t)(A));
    WUR_AE_CEND0  ((uintptr_t)(A+N*N*2));

    {
        ae_valign aX;
        pX=(xtfloatx2*)x;
        pA=(xtfloatx2*)A;
        aX=AE_LA64_PP(pX);
        for(n=0; n<N; n++)
        {
            xtfloatx2 a0,a1,a2;
            XT_LASX2IP(a0,aX,pX);
            XT_LASX2IP(a1,aX,pX);
            XT_LASX2IP(a2,aX,pX);
            XT_SSX2IP(a0,pA,sizeof(xtfloatx2));
            XT_SSX2IP(a1,pA,sizeof(xtfloatx2));
            XT_SSX2IP(a2,pA,sizeof(xtfloatx2));
            XT_SSX2IP(XT_CONST_S(0),pA,sizeof(xtfloatx2));
            XT_SSX2IP(XT_CONST_S(0),pA,sizeof(xtfloatx2));
            XT_SSX2IP(XT_CONST_S(0),pA,sizeof(xtfloatx2));
            XT_SSX(XT_CONST_S(1),(xtfloat*)pA,(-N+n)*sizeof(float32_t));
        }
    }
    /* Gauss elimination */
    for(k=0; k<N; k++)
    {
        int off;
        xtfloatx2 amax;
        xtfloatx2 R=XT_CONST_S(0);
        unsigned int imax;
        /* pivoting */
        imax=k;
        amax=XT_CONST_S(0);
        /* find absolute max value in the k-th column */
        pA=(xtfloatx2*)&A[2*k*N+k];
        for(n=k; n<N; n++)
        {
          xtbool2 cond;
          xtfloatx2 a, t;

          //XT_LSXP(t,castxcc(xtfloat,pA),2*N*sizeof(xtfloat));
          {
              ae_int32x2 tt;
              AE_L32_XP(tt,castxcc(ae_int32,pA),2*N*sizeof(xtfloat));
              t=XT_AE_MOVXTFLOATX2_FROMINT32X2(tt);
          }
          a = XT_ABS_SX2(t);

          cond = XT_OLT_SX2(amax, a);
          XT_MOVT_SX2(amax, a, cond);
          XT_MOVT_SX2(R  ,  t, cond);
          XT_MOVNEZ(imax, n, AE_MOVAB2(cond));
        }
        R = XT_RECIP_SX2(R);

        off=(int)((imax-k)*sizeof(float32_t)*2*N);
        pA=(xtfloatx2*)&A[k*N*2];
        pAwr=pA;
        for (n=0; n<N; n++) 
        {
            xtfloatx2 ak,ai;
            ak=XT_LSX2I(pA,0);
            ai=XT_LSX2X(pA,off);
            XT_SSX2X (ak,pA,off);
            XT_SSX2IP(ai,pA,sizeof(xtfloatx2));
        }
        __Pragma("no_reorder")
        /* elimination */
        {
            xtfloatx2 Ak0,Ak1,Ak2,Ak3,Ak4,Ak5;
            pA=(xtfloatx2*)&A[k*2*N];
            pAwr=pA;
            Ak1=XT_LSX2I(pA,1*sizeof(xtfloatx2));
            Ak2=XT_LSX2I(pA,2*sizeof(xtfloatx2));
            Ak3=XT_LSX2I(pA,3*sizeof(xtfloatx2));
            Ak4=XT_LSX2I(pA,4*sizeof(xtfloatx2));
            Ak5=XT_LSX2I(pA,5*sizeof(xtfloatx2));
            XT_LSX2XC(Ak0,pA,2*N*sizeof(float32_t));
            Ak0=XT_MUL_SX2(Ak0,R);
            Ak1=XT_MUL_SX2(Ak1,R);
            Ak2=XT_MUL_SX2(Ak2,R);
            Ak3=XT_MUL_SX2(Ak3,R);
            Ak4=XT_MUL_SX2(Ak4,R);
            Ak5=XT_MUL_SX2(Ak5,R);
            XT_SSX2I(Ak1,pAwr,1*sizeof(xtfloatx2));
            XT_SSX2I(Ak2,pAwr,2*sizeof(xtfloatx2));
            XT_SSX2I(Ak3,pAwr,3*sizeof(xtfloatx2));
            XT_SSX2I(Ak4,pAwr,4*sizeof(xtfloatx2));
            XT_SSX2I(Ak5,pAwr,5*sizeof(xtfloatx2));
            XT_SSX2XC(Ak0,pAwr,2*N*sizeof(float32_t));

            for (n=0; n<N-1; n++)
            {
                xtfloatx2 c,An0,An1,An2,An3,An4,An5;
                c=XT_LSX((const xtfloat*)pA,k*sizeof(float32_t));
                An1=XT_LSX2I(pA,1*sizeof(xtfloatx2));
                An2=XT_LSX2I(pA,2*sizeof(xtfloatx2));
                An3=XT_LSX2I(pA,3*sizeof(xtfloatx2));
                An4=XT_LSX2I(pA,4*sizeof(xtfloatx2));
                An5=XT_LSX2I(pA,5*sizeof(xtfloatx2));
                XT_LSX2XC(An0,pA,2*N*sizeof(float32_t));
                XT_MSUB_SX2(An0,Ak0,c);
                XT_MSUB_SX2(An1,Ak1,c);
                XT_MSUB_SX2(An2,Ak2,c);
                XT_MSUB_SX2(An3,Ak3,c);
                XT_MSUB_SX2(An4,Ak4,c);
                XT_MSUB_SX2(An5,Ak5,c);
                XT_SSX2I(An1,pAwr,1*sizeof(xtfloatx2));
                XT_SSX2I(An2,pAwr,2*sizeof(xtfloatx2));
                XT_SSX2I(An3,pAwr,3*sizeof(xtfloatx2));
                XT_SSX2I(An4,pAwr,4*sizeof(xtfloatx2));
                XT_SSX2I(An5,pAwr,5*sizeof(xtfloatx2));
                XT_SSX2XC(An0,pAwr,2*N*sizeof(float32_t));
            }
        }
        __Pragma("no_reorder")
    }
    /* copy to x */
    {
        ae_valign aX=AE_ZALIGN64();
        pX=(xtfloatx2*)x;
        pA=(xtfloatx2*)(A+N);
        for(n=0; n<N; n++)
        {
            xtfloatx2 a0,a1,a2;
            a1=XT_LSX2I(pA,1*sizeof(xtfloatx2));
            a2=XT_LSX2I(pA,2*sizeof(xtfloatx2));
            XT_LSX2XP(a0,pA,N*sizeof(xtfloatx2));
            XT_SASX2IP(a0,aX,pX);
            XT_SASX2IP(a1,aX,pX);
            XT_SASX2IP(a2,aX,pX);
        }
        XT_SASX2POSFP(aX,pX);
    }
}
// scratch allocation
size_t mtx_inv6x6f_getScratchSize()   { return (2*6*6)*sizeof(float32_t); }
#else // code for scalar FPU
void mtx_inv6x6f(void * pScr,float32_t* x)
{
    const int N=6;
    xtfloat * restrict pAn1;
    xtfloat * restrict pAn2;
    xtfloat * restrict pAn3;
    xtfloat * restrict pAn4;
    xtfloat * restrict pAn5;
    ae_int32x2* restrict pX;
    ae_int32x2* restrict pA;
    int n,m,k;
    float32_t *A = (float32_t *)pScr; //float32_t ALIGN(8) A[32];
    /* Set bounds of the buffer */
    WUR_AE_CBEGIN0((uintptr_t)(A));
    WUR_AE_CEND0  ((uintptr_t)(A+2*N*N));
    {
        ae_valign aX;
        pX=(ae_int32x2*)x;
        pA=(ae_int32x2*)A;
        aX=AE_LA64_PP(pX);
        for(n=0; n<N; n++)
        {
            ae_int32x2 a0,a1,a2;
            AE_LA32X2_IP(a0,aX,pX);
            AE_LA32X2_IP(a1,aX,pX);
            AE_LA32X2_IP(a2,aX,pX);
            AE_S32X2_IP(a0,pA,sizeof(ae_int32x2));
            AE_S32X2_IP(a1,pA,sizeof(ae_int32x2));
            AE_S32X2_IP(a2,pA,sizeof(ae_int32x2));
            AE_S32X2_IP(0,pA,sizeof(ae_int32x2));
            AE_S32X2_IP(0,pA,sizeof(ae_int32x2));
            AE_S32X2_IP(0,pA,sizeof(ae_int32x2));
            AE_S32_L_X(0x3f800000,(ae_int32*)pA,(-N+n)*sizeof(float32_t));
        }
    }
    __Pragma("no_reorder")
    for(k=0; k<N; k++)
    {
        xtfloat * pmax;
        xtfloat * pAk;
        xtfloat amax, max,R;
        unsigned int imax;
        /* pivoting */
        imax=k;
        amax=0.0f;
        /* find absolute max value in the k-th column */
        pAk  = (xtfloat *)(A+k*(2*N+1));
        for(n=k; n<N; n++)
        {
            xtbool cond;
            xtfloat a, t;
            XT_LSXP(t,pAk,2*N*sizeof(xtfloat));
            a = XT_ABS_S(t);

            cond = (XT_OLT_S(amax, a));
            XT_MOVT_S(amax, a, cond);
            XT_MOVT_S(max,  t, cond);
            XT_MOVT  (imax, n, cond);
        }

        R = max;
        R = XT_RECIP_S(R);

        /* swap k-th and imax-th rows */
        pmax = (xtfloat *)(A+imax*2*N);
        pAk  = (xtfloat *)(A+k*2*N);
        for(m=0; m<2*N; m++) 
        { 
            xtfloat t,am; 
            t=XT_LSI(pmax,0);
            am=XT_LSI(pAk,0);
            XT_SSIP(am,pmax,sizeof(xtfloat));
            XT_SSIP(XT_MUL_S(R,t),pAk,sizeof(xtfloat));
        }

        /* elimination */
        __Pragma("no_reorder")
        {
            pAk  = (xtfloat *)(A+k*2*N+k);
            xtfloat c1,c2,c3,c4,c5;
            pAn1=(xtfloat*)(pAk)    ; AE_ADDCIRC_XC(castxcc(ae_int64,pAn1),2*N*sizeof(xtfloat));
            pAn2=(xtfloat*)pAn1   ; AE_ADDCIRC_XC(castxcc(ae_int64,pAn2),2*N*sizeof(xtfloat));
            pAn3=(xtfloat*)pAn2   ; AE_ADDCIRC_XC(castxcc(ae_int64,pAn3),2*N*sizeof(xtfloat));
            pAn4=(xtfloat*)pAn3   ; AE_ADDCIRC_XC(castxcc(ae_int64,pAn4),2*N*sizeof(xtfloat));
            pAn5=(xtfloat*)pAn4   ; AE_ADDCIRC_XC(castxcc(ae_int64,pAn5),2*N*sizeof(xtfloat));
            c1=XT_LSI(pAn1,0*sizeof(xtfloat));
            c2=XT_LSI(pAn2,0*sizeof(xtfloat));
            c3=XT_LSI(pAn3,0*sizeof(xtfloat));
            c4=XT_LSI(pAn4,0*sizeof(xtfloat));
            c5=XT_LSI(pAn5,0*sizeof(xtfloat));
            for (m=k; m<2*N; m++)
            {
                xtfloat akm,an1,an2,an3,an4,an5;
                XT_LSIP(akm,pAk,sizeof(xtfloat));
                an1=XT_LSI(pAn1,0);
                an2=XT_LSI(pAn2,0);
                an3=XT_LSI(pAn3,0);
                an4=XT_LSI(pAn4,0);
                an5=XT_LSI(pAn5,0);
                XT_MSUB_S(an1,akm,c1);
                XT_MSUB_S(an2,akm,c2);
                XT_MSUB_S(an3,akm,c3);
                XT_MSUB_S(an4,akm,c4);
                XT_MSUB_S(an5,akm,c5);
                XT_SSIP(an1,pAn1,sizeof(xtfloat));
                XT_SSIP(an2,pAn2,sizeof(xtfloat));
                XT_SSIP(an3,pAn3,sizeof(xtfloat));
                XT_SSIP(an4,pAn4,sizeof(xtfloat));
                XT_SSIP(an5,pAn5,sizeof(xtfloat));
            }
        }
        __Pragma("no_reorder")
    }
    /* copy to x */
    {
        ae_valign aX=AE_ZALIGN64();
        pX=(ae_int32x2*)x;
        pA=(ae_int32x2*)(A+N);
        for(n=0; n<N; n++)
        {
            ae_int32x2 a0,a1,a2;
            a1=AE_L32X2_I(pA,1*sizeof(ae_int32x2));
            a2=AE_L32X2_I(pA,2*sizeof(ae_int32x2));
            AE_L32X2_XP(a0,pA,N*sizeof(ae_int32x2));
            AE_SA32X2_IP(a0,aX,pX);
            AE_SA32X2_IP(a1,aX,pX);
            AE_SA32X2_IP(a2,aX,pX);
        }
        AE_SA64POS_FP (aX,pX);
    }
}
// scratch allocation
size_t mtx_inv6x6f_getScratchSize()   { return (2*6*6)*sizeof(float32_t); }

#endif
