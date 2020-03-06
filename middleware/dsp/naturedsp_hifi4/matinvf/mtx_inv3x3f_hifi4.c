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
 * Real Matrix Inversion
 * Optimized code for HiFi4
  IntegrIT, 2006-2018
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Matrix functions */
#include "NatureDSP_Signal_matinv.h"
#include "common_fpu.h"

#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(void,mtx_inv3x3f,(void* pScr,float32_t* x))
size_t mtx_inv3x3f_getScratchSize        () { return 0; }

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
#define SZ_F32 (sizeof(float32_t))
#define SZ_2F32 (2*SZ_F32)

void mtx_inv3x3f(void* pScr, float32_t* x)
{
    xtfloatx2 *restrict pk;
    xtfloatx2 *restrict pmax;
    xtfloatx2 *         pA0;
    xtfloatx2 *restrict pA1;
    xtfloatx2 R, C0, C1;
    xtfloatx2 A00, A01, A02;
    xtfloatx2 A10, A11, A12;
    xtfloatx2 Amax0, Amax1, Amax2,
                Ak0,   Ak1,   Ak2;
    ae_valign vX;
    int n, k;
    float32_t *A=(float32_t *)pScr;//    float32_t ALIGN(8) A[18];

    /* Copy the matrix to buffer and
     * initialize identity matrix:
     *
     * x00 x01 x02 1.0 0.0 0.0
     * x10 x11 x12 0.0 1.0 0.0
     * x20 x21 x22 0.0 0.0 1.0
     */
    C0 = (xtfloatx2)(0.0f);
    C1 = (xtfloatx2)1.0f;
    pA0 = (xtfloatx2 *)(x);
    pA1 = (xtfloatx2 *)(A);
    {
      /* Load input matrix */
      vX = XT_LASX2PP(pA0);
      XT_LASX2IP(A00, vX, pA0);
      XT_LASX2IP(A01, vX, pA0);
      XT_LASX2IP(A02, vX, pA0);
      XT_LASX2IP(A10, vX, pA0);
      A11 = XT_LSI((xtfloat *)pA0, 0);
      /* Save input and identity matrix */
      /* 1-st row */
      XT_SSX2IP(A00, pA1, SZ_2F32);
      R = XT_SEL32_HH_SX2(A01, C1);
      XT_SSX2IP(R , pA1, SZ_2F32);
      XT_SSX2IP(C0, pA1, SZ_2F32);
      /* 2-nd row */
      R = XT_SEL32_LH_SX2(A01, A02);
      XT_SSX2IP(R , pA1, SZ_2F32);
      R = XT_SEL32_LH_SX2(A02, C0);
      XT_SSX2IP(R , pA1, SZ_2F32);
      R = XT_SEL32_LH_SX2(C1, C0);
      XT_SSX2IP(R , pA1, SZ_2F32);
      /* 3-rd row */
      XT_SSX2IP(A10, pA1, SZ_2F32);
      R = XT_SEL32_LH_SX2(A11, C0);
      XT_SSX2IP(R , pA1, SZ_2F32);
      R = XT_SEL32_LH_SX2(C0, C1);
      XT_SSX2IP(R , pA1, SZ_2F32);
    }
    /* Set bounds of the buffer */
    WUR_AE_CBEGIN0((uintptr_t)(A));
    WUR_AE_CEND0  ((uintptr_t)(A+18));

    pk  = (xtfloatx2 *)(A);
    pA0 = (xtfloatx2 *)(A+6);
    pA1 = (xtfloatx2 *)(A+12);
    /* Gauss elimination */
    for(k=0; k<3; k++)
    {
      xtfloat amax, max;
      unsigned int imax;
      /* pivoting */
      imax=k;
      amax=0.0f;
      max=0.0f;
      /* find absolute max value in the k-th column */
      for(n=k; n<3; n++)
      {
        xtbool cond;
        xtfloat a, t;

        t = A[n*6+k];
        a = XT_ABS_S(t);

        cond = xtbool2_extract_0(XT_OLT_S(amax, a));
        XT_MOVT_S(amax, a, cond);
        XT_MOVT_S(max,  t, cond);
        XT_MOVT  (imax, n, cond);
      }
      {
        R = max;
        R = XT_RECIP_SX2(R);
        
        /* swap k-th and imax-th rows */
        pmax = (xtfloatx2 *)(A+imax*6);
        
        Ak0   = XT_LSX2I(pmax, 0*SZ_2F32);
        Ak1   = XT_LSX2I(pmax, 1*SZ_2F32);
        Ak2   = XT_LSX2I(pmax, 2*SZ_2F32);
        Amax0 = XT_LSX2I(  pk, 0*SZ_2F32);
        Amax1 = XT_LSX2I(  pk, 1*SZ_2F32);
        Amax2 = XT_LSX2I(  pk, 2*SZ_2F32);
        XT_SSX2IP(Amax0, pmax, SZ_2F32);
        XT_SSX2IP(Amax1, pmax, SZ_2F32);
        XT_SSX2IP(Amax2, pmax, SZ_2F32);
        __Pragma("no_reorder")
        /* multiply k-th row by the reciprocal *
         * pivot element during swapping rows  */
        Ak0 = Ak0*R;
        Ak1 = Ak1*R;
        Ak2 = Ak2*R;
        XT_SSX2IP(Ak0, pk, SZ_2F32);
        XT_SSX2IP(Ak1, pk, SZ_2F32);
        XT_SSX2IP(Ak2, pk, SZ_2F32);

        /* elimination */
        /* join forward and back substitution */

        C0 = XT_LSX((xtfloat *)pA0, k*SZ_F32);
        C1 = XT_LSX((xtfloat *)pA1, k*SZ_F32);

        A00 = XT_LSX2I(pA0, 0*SZ_2F32);
        A01 = XT_LSX2I(pA0, 1*SZ_2F32);
        A02 = XT_LSX2I(pA0, 2*SZ_2F32);
        A10 = XT_LSX2I(pA1, 0*SZ_2F32);
        A11 = XT_LSX2I(pA1, 1*SZ_2F32);
        A12 = XT_LSX2I(pA1, 2*SZ_2F32);
        
        XT_MSUB_SX2(A00, Ak0, C0);
        XT_MSUB_SX2(A01, Ak1, C0);
        XT_MSUB_SX2(A02, Ak2, C0);
        XT_MSUB_SX2(A10, Ak0, C1);
        XT_MSUB_SX2(A11, Ak1, C1);
        XT_MSUB_SX2(A12, Ak2, C1);
        
        XT_SSX2XC(A00, pA0, SZ_2F32);
        XT_SSX2XC(A01, pA0, SZ_2F32);
        XT_SSX2XC(A02, pA0, SZ_2F32);
        XT_SSX2XC(A10, pA1, SZ_2F32);
        XT_SSX2XC(A11, pA1, SZ_2F32);
        XT_SSX2XC(A12, pA1, SZ_2F32);
      }
    }
    /* copy 4-6 columns to x */
    {
      /* extract matrix from columns */
      A00 = XT_SEL32_LH_SX2(A01, A02);
      A01 = XT_SEL32_LL_SX2(A02, A11);
      A02 = A12;
      A10 = XT_SEL32_LH_SX2(Ak1, Ak2);
      A11 = XT_LOW_S(Ak2);
      /* save inverse matrix */
      pA0 = (xtfloatx2 *)(x);
      vX = AE_ZALIGN64();
      XT_SASX2IP(A00, vX, pA0);
      XT_SASX2IP(A01, vX, pA0);
      XT_SASX2IP(A02, vX, pA0);
      XT_SASX2IP(A10, vX, pA0);
      XT_SASX2POSFP(vX, pA0);
      XT_SSI(A11, (xtfloat *)pA0, 0);
    }
}/* mtx_inv3x3f() */
size_t mtx_inv3x3f_getScratchSize        () 
{
    return 18*sizeof(float32_t);
}

#else
// for scalar FPU
#define SZ_F32 (sizeof(float32_t))
#define SZ_2F32 (2*SZ_F32)

void mtx_inv3x3f(void *pScr, float32_t* x)
{
    xtfloat * restrict pAn1;
    xtfloat * restrict pAn2;
    ae_int32x2  *restrict pA0;
    ae_int32x2  *restrict pA1;
    xtfloat  R;
    ae_valign vX;
    int m, n, k;
    float32_t *A=(float32_t *)pScr;//    float32_t ALIGN(8) A[18];

    /* Copy the matrix to buffer and
     * initialize identity matrix:
     *
     * x00 x01 x02 1.0 0.0 0.0
     * x10 x11 x12 0.0 1.0 0.0
     * x20 x21 x22 0.0 0.0 1.0
     */
    {
        ae_int32x2 C0, C1, A00, A01, A02, R;
        ae_int32x2 A10, A11;
        C0 = AE_ZERO32();
        C1 = 0x3f800000;
        pA0 = (ae_int32x2 *)(x);
        pA1 = (ae_int32x2 *)(A);
        /* Load input matrix */
        vX = AE_LA64_PP(pA0);
        AE_LA32X2_IP(A00, vX, pA0);
        AE_LA32X2_IP(A01, vX, pA0);
        AE_LA32X2_IP(A02, vX, pA0);
        AE_LA32X2_IP(A10, vX, pA0);
        A11 = AE_L32_I((const ae_int32*)pA0, 0);
        /* Save input and identity matrix */
        /* 1-st row */
        AE_S32X2_IP(A00, pA1, SZ_2F32);
        R = AE_SEL32_HH(A01, C1);
        AE_S32X2_IP(R , pA1, SZ_2F32);
        AE_S32X2_IP(C0, pA1, SZ_2F32);
        /* 2-nd row */
        R = AE_SEL32_LH(A01, A02);
        AE_S32X2_IP(R , pA1, SZ_2F32);
        R = AE_SEL32_LH(A02, C0);
        AE_S32X2_IP(R , pA1, SZ_2F32);
        R = AE_SEL32_LH(C1, C0);
        AE_S32X2_IP(R , pA1, SZ_2F32);
        /* 3-rd row */
        AE_S32X2_IP(A10, pA1, SZ_2F32);
        R = AE_SEL32_LH(A11, C0);
        AE_S32X2_IP(R , pA1, SZ_2F32);
        R = AE_SEL32_LH(C0, C1);
        AE_S32X2_IP(R , pA1, SZ_2F32);
    }
    __Pragma("no_reorder")
    /* Set bounds of the buffer */
    WUR_AE_CBEGIN0((uintptr_t)(A));
    WUR_AE_CEND0  ((uintptr_t)(A+18));
    /* Gauss elimination */
    for(k=0; k<3; k++)
    {
        xtfloat * pmax;
        xtfloat * pAk;
        xtfloat amax,max;
        unsigned int imax;
        /* pivoting */
        imax=k;
        amax=XT_CONST_S(0);
        for(n=k; n<3; n++)
        {
            xtbool cond;
            xtfloat a, t;

            t = A[n*6+k];
            a = XT_ABS_S(t);

            cond = XT_OLT_S(amax, a);
            XT_MOVT_S(amax, a, cond);
            XT_MOVT_S(max,  t, cond);
            XT_MOVT  (imax, n, cond);
        }
        R = max;
        R = XT_RECIP_S(R);

        /* swap k-th and imax-th rows */
        pmax = (xtfloat *)(A+imax*6);
        pAk  = (xtfloat *)(A+k*6);
        for(m=0; m<6; m++) 
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
            pAk  = (xtfloat *)(A+k*6);
            xtfloat c1,c2;
            pAn1=(xtfloat*)pAk    ; AE_ADDCIRC_XC(castxcc(ae_int64,pAn1),6*sizeof(xtfloat));
            pAn2=(xtfloat*)pAn1   ; AE_ADDCIRC_XC(castxcc(ae_int64,pAn2),6*sizeof(xtfloat));
            c1=XT_LSX(pAn1,k*sizeof(xtfloat));
            c2=XT_LSX(pAn2,k*sizeof(xtfloat));
            for (m=0; m<6; m++)
            {
                xtfloat akm,an1,an2;
                XT_LSIP(akm,pAk,sizeof(xtfloat));
                an1=XT_LSI(pAn1,0);
                an2=XT_LSI(pAn2,0);
                XT_MSUB_S(an1,akm,c1);
                XT_MSUB_S(an2,akm,c2);
                XT_SSIP(an1,pAn1,sizeof(xtfloat));
                XT_SSIP(an2,pAn2,sizeof(xtfloat));
            }
        }
    }
    /* copy 4-6 columns to x */
    __Pragma("no_reorder")
    {
        ae_int32x2 A1,A2,A4,A5,A7,A8,x0,x1,x2,x3,x4;
        pA0 = (ae_int32x2 *)(x);
        pA1 = (ae_int32x2 *)(A+2);
        A1=AE_L32X2_I(pA1,0*sizeof(ae_int32x2));
        A2=AE_L32X2_I(pA1,1*sizeof(ae_int32x2));
        A4=AE_L32X2_I(pA1,3*sizeof(ae_int32x2));
        A5=AE_L32X2_I(pA1,4*sizeof(ae_int32x2));
        A7=AE_L32X2_I(pA1,6*sizeof(ae_int32x2));
        A8=AE_L32X2_I(pA1,7*sizeof(ae_int32x2));
        /* extract matrix from columns */
        x0 = AE_SEL32_LH(A1, A2);
        x1 = AE_SEL32_LL(A2, A4);
        x2 = A5;
        x3 = AE_SEL32_LH(A7, A8);
        x4 = A8;
        /* save inverse matrix */
        pA0 = (ae_int32x2 *)(x);
        vX = AE_ZALIGN64();
        AE_SA32X2_IP(x0, vX, pA0);
        AE_SA32X2_IP(x1, vX, pA0);
        AE_SA32X2_IP(x2, vX, pA0);
        AE_SA32X2_IP(x3, vX, pA0);
        AE_SA64POS_FP(vX, pA0);
        AE_S32_L_I(x4, (ae_int32 *)pA0, 0);
    }
}
size_t mtx_inv3x3f_getScratchSize        () 
{
    return 18*sizeof(float32_t);
}

#endif

