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
    NatureDSP Signal Processing Library. FIR part
    Real FIR Filter with decimation
    C code optimized for HiFi4
    IntegrIT, 2006-2018
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
#include "fir_decimaf_Dx.h"
#include "common_fpu.h"

#if (HAVE_VFPU)
/*-------------------------------------------------------------------------
    universal decimator:
    Input/output:
    delay[M] - circular delay line
    Input:
    p        - pointer to delay line
    x[N*D]   - input signal
    h[M]     - impulse response
    D          decimation factor
    N        - number of output samples
    Output:
    z[N]     - output samples
    Restrictions:
    N>0, M>0
    D>4
    M multiple of 2
    N multiple of 8
    delay should be aligned on 8 byte boundary

    Returns:
    updated pointer to delay line
-------------------------------------------------------------------------*/
float32_t * fir_decimaf_Dx(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x,  int M, int D, int N)
{
    const xtfloat   *px;
          xtfloat   *pz;
    const xtfloatx2 *ph0;
    const xtfloatx2 *ph1;
    const xtfloatx2 *pd;
          xtfloat   *pp;
    xtfloatx2 A0, A1, A2, A3,
              X01, X0_1, X_23, X_45, X_67,
              H01, H23, H45, H67;
    xtfloat   s0;
    ae_valign ad;
    int m, n, d;

    NASSERT(N>0 && M>0);
    NASSERT(M%4==0);
    NASSERT(D>4);
    NASSERT_ALIGN(delay,8);
    /* set bounds of the delay line */
    WUR_AE_CBEGIN0( (uintptr_t)( delay + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( delay + M ) );
    /* initialize pointers */
    px=(const xtfloat*)x;
    pz=(      xtfloat*)z;
    pp=(      xtfloat*)p;
    
    /* Compute by 1 sample */
    __Pragma("loop_count min=1");
    for (n = 0; n < N; n++)
    {
        ph0 = (const xtfloatx2*)h;
        ph1 = ph0 + 1;
        A0 = A1 = A2 = A3 = (xtfloatx2)(0.0f);
        /* load input sample */
        X01 = XT_LSI(px, 0);
        /* preload samples from the delay line */
        pd = (const xtfloatx2*)pp;
        XT_LASX2NEGPC(ad, pd);
        XT_LASX2RIC(X0_1, ad, pd);
        X0_1 = XT_SEL32_HL_SX2(X01, X0_1);
        XT_LASX2RIC(X_23, ad, pd);
        
        /* process by 8 taps for the current sample */
        for (m = 0; m < (M>>3); m++)
        {
            /* load filter coefficients */
            XT_LSX2IP(H01, ph0, 4*sizeof(float32_t));
            XT_LSX2IP(H23, ph1, 4*sizeof(float32_t));
            XT_LSX2IP(H45, ph0, 4*sizeof(float32_t));
            XT_LSX2IP(H67, ph1, 4*sizeof(float32_t));
            /* load samples from the delay line */
            XT_LASX2RIC(X_45, ad, pd);
            XT_LASX2RIC(X_67, ad, pd);

            XT_MADD_SX2(A0, H01, X0_1);
            XT_MADD_SX2(A1, H23, X_23);
            XT_MADD_SX2(A2, H45, X_45);
            XT_MADD_SX2(A3, H67, X_67);
            /* load next samples */
            XT_LASX2RIC(X0_1, ad, pd);
            XT_LASX2RIC(X_23, ad, pd);
        }
        /* process last 4 taps if M is not a multiple of 8 */
        if(M & 4)
        {
            H01 = XT_LSX2I(ph0, 0);
            H23 = XT_LSX2I(ph1, 0);
            XT_MADD_SX2(A0, H01, X0_1);
            XT_MADD_SX2(A1, H23, X_23);
        }
        /* save computed sample */
        A0 = A0 + A1;
        A2 = A2 + A3;
        A0 = A0 + A2;
        s0 = XT_RADD_SX2(A0);
        XT_SSIP(s0, pz, sizeof(float32_t));

        /* update the delay line */
        __Pragma("loop_count min=4");
        for(d = 0; d < D; d++) 
        {
            XT_LSIP(s0, px, sizeof(float32_t));
            XT_SSXC(s0, pp, sizeof(float32_t));
        }
    }
    return (float32_t*)pp;
}
#elif (HAVE_FPU)
// for scalar FPU
float32_t * fir_decimaf_Dx(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x,  int M, int D, int N)
{
    const xtfloat * restrict pX=(const xtfloat *)x;
          xtfloat * restrict pZ=(      xtfloat *)z;
          xtfloat * restrict pD;
          xtfloat * restrict pP=(      xtfloat *)p;
    int k,n,m,j;

    NASSERT_ALIGN(x,8);
    NASSERT(N%8==0);
    NASSERT(x);
    NASSERT(z);
    if(N<=0) return p;
    NASSERT(N>0);
    NASSERT(M>0);
    NASSERT(D>1);
    NASSERT(M%2==0);
        /* set circular buffer boundaries */
    WUR_AE_CBEGIN0((uintptr_t)(delay + 0));
    WUR_AE_CEND0  ((uintptr_t)(delay + M));
    for (k=0,n=0; n<N*D; n+=D,k++)
    {
        xtfloat a0,a1,xn;
        a0=a1=XT_CONST_S(0);
        pD=(xtfloat *)pP;
        XT_LSIP(xn,pX,sizeof(xtfloat));
        XT_SSXC(xn,pP,sizeof(xtfloat));
        for (m=0; m<M; m+=2)
        {
            xtfloat d0,d1;
            XT_LSXC(d0,pD,-(int)sizeof(xtfloat));
            XT_LSXC(d1,pD,-(int)sizeof(xtfloat));
            XT_MADD_S(a0,h[m],d0);
            XT_MADD_S(a1,h[m+1],d1);
        }
        __Pragma("no_reorder")
        for(j=0; j<D-1; j++) 
        {
            XT_LSIP(xn,pX,sizeof(xtfloat));
            XT_SSXC(xn,pP,sizeof(xtfloat));
        }
        a0=XT_ADD_S(a0,a1);
        XT_SSIP(a0,pZ,sizeof(xtfloat));
    }
    return (float32_t*)pP;
}
#endif
