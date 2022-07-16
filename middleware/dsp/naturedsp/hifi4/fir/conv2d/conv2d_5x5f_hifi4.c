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
  2D Convolution  
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
#include "common_fpu.h"

/*-------------------------------------------------------------------------
  2D convolution
  Functions compute the two-dimensional convolution of input matrix x[M][N]
  and y[P][Q] and store the result in matrix z[M+P-1][N+Q-1]
  Additional parameter rsh allows to control fixed point representation of 
  output data.

  Precision: 
  8x8      8-bit coefficients, 8-bit data, 8-bit output, Q7
  8x16     8-bit coefficients Q7, 16-bit data, 16-bit output, Q15
  16x16    16-bit coefficients, 16-bit data, 16-bit output, Q15
  f        floating point data

  Input:
  x[M][N]   input data Q15, Q7, floating point
  y[P][Q]   input data Q15, Q7, floating point
  M         number of rows in the matrix x
  N         number of columns in the matrix x
  P         number of rows in the matrix y
  Q         number of columns in the matrix y
  rsh       additional right shift (for fixed point API only)

  Output:
  z	[M+P-1][N+Q-1] output data, Q(7-rsh), Q(15-rsh), floating point

  Temporary:
  pScr     scratch data. Should have size at least as requested by 
           corresponding scratch allocation function

  Restrictions:
  x,y,z        should not overlap
  x,y,z,pScr   aligned on a 8-bytes boundary
  P, Q	       >0 and multiplies of 8
-------------------------------------------------------------------------*/
#if (HAVE_VFPU==0 && HAVE_FPU==0)
size_t conv2d_5x5f_getScratchSize (int P, int Q) 
{ 
    (void)P,(void)Q;
    return 0;
}
DISCARD_FUN(void,conv2d_5x5f,(void* pScr, float32_t *z, const float32_t * x, const float32_t * y, int P, int Q))
#elif HAVE_VFPU

void conv2d_5x5f      (void* pScr, float32_t *z, const float32_t * x, const float32_t * y, int P, int Q)
{
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT_ALIGN(z,8);
    NASSERT_ALIGN(pScr,8);
    NASSERT(P>=0 && P%8==0);
    NASSERT(Q>=0 && Q%8==0);
    
    float32_t * pwr = (float32_t*)pScr;
    int i, j, n;
#define M 5
#define N 5
    const xtfloatx2 * restrict pY0;
    const xtfloatx2 * restrict pY1;
    const xtfloatx2 * restrict pY2;
    const xtfloatx2 * restrict pY3;
    const xtfloatx2 * restrict pY4;
    xtfloatx2 * restrict pWwr = (xtfloatx2*)pwr;
    xtfloatx2 * restrict pZ;
    xtfloatx2 * restrict pX;
    xtfloatx2 * restrict pW;
    xtfloatx2 * restrict pz0 = (xtfloatx2 *)(pwr + 10 * 5);
    xtfloatx2 * restrict pz1 = (xtfloatx2 *)(pwr + 10 * 5 + (Q&(~1)));
    ae_valign aX;
    xtfloatx2 tmp, w00;
    xtfloatx2 w0, w1, w2, w3, w4;
    xtfloatx2 z01, z23;
    xtfloatx2 y01, y23, y45, y67;
    xtfloatx2 y12, y34, y56;
    xtfloatx2 z01_0, z23_0;
    if (P <= 0 || Q <= 0) return;

    pX = (xtfloatx2 *)x;
    pW = (xtfloatx2 *)pScr + (M*N - 1);
    for (i = 0; i < ((M*N)>>1); i++)
    {
        XT_LSX2IP(w00, pX, sizeof(xtfloatx2));
        tmp = XT_SEL32_HH_SX2(w00, w00);
        XT_SSX2XP(tmp, pW, -1 * ((int)sizeof(xtfloatx2)));
        tmp = XT_SEL32_LL_SX2(w00, w00);
        XT_SSX2XP(tmp, pW, -1 * ((int)sizeof(xtfloatx2)));
    }
    pX = (xtfloatx2 *)((float32_t*)pX - 1);
    aX = XT_LASX2PP(pX);
    XT_LASX2IP(w00, aX, pX);
    tmp = XT_SEL32_LL_SX2(w00, w00);
    XT_SSX2XP(tmp, pW, -1 * ((int)sizeof(xtfloatx2)));

    /* first M-1 rows */
    for (i = 0; i < M-1; i++)
    {
        /* first  N-1 elements in a row */
        tmp = (xtfloatx2)0.0f;
        z01 = (xtfloatx2)0.0f;
        z23 = (xtfloatx2)0.0f;
        pY0 = (const xtfloatx2*)(y);
        pWwr = (xtfloatx2*)(pwr + (M - 1 - i) * 10);
        for (n = 0; n < i+1; n++)
        {        
            w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
            w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
            w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0*sizeof(xtfloatx2));
            y23 = XT_LSX2I(pY0, 1*sizeof(xtfloatx2));
            y12 = XT_SEL32_LH_SX2(y01, y23);
            y56 = XT_SEL32_LH_SX2(tmp, y01);
            XT_MADD_SX2(z01, w4, y01);
            XT_MADD_SX2(z23, w4, y23);
            XT_MADD_SX2(z01, w3, y56);
            XT_MADD_SX2(z23, w3, y12);
            XT_MADD_SX2(z23, w2, y01);
            XT_MADD_SX2(z23, w1, y56);
            pWwr += 5;
            pY0 = (xtfloatx2*)((xtfloat*)pY0 + Q);
        }
        pZ = (xtfloatx2 *)(z + i*(N + Q - 1));
        XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));

        /* middle section */
        if (i == 0)
        {
            pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + (N-1));
            pY0 = (const xtfloatx2*)(y + (0) * Q);
            pWwr = (xtfloatx2*)(pwr + 4*10);
            for (j = N - 1; j < Q; j += 4)
            {
                w0 = XT_LSX2I(pWwr, 0);
                w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
                w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
                w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
                w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
                XT_LSX2IP(y01, pY0, sizeof(xtfloatx2));
                XT_LSX2IP(y23, pY0, sizeof(xtfloatx2));
                y45 = XT_LSX2I(pY0, 0);
                y67 = XT_LSX2I(pY0, sizeof(xtfloatx2));
                y12 = XT_SEL32_LH_SX2(y01, y23);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                y56 = XT_SEL32_LH_SX2(y45, y67);
                z01 = XT_MUL_SX2(w0, y01);
                z23 = XT_MUL_SX2(w0, y23);
                XT_MADD_SX2(z01, w1, y12);
                XT_MADD_SX2(z23, w1, y34);
                XT_MADD_SX2(z01, w2, y23);
                XT_MADD_SX2(z23, w2, y45);
                XT_MADD_SX2(z01, w3, y34);
                XT_MADD_SX2(z23, w3, y56);
                XT_MADD_SX2(z01, w4, y45);
                XT_MADD_SX2(z23, w4, y67);
                XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));
            }
        }
        else
        {
            pz0 = (xtfloatx2 *)(pwr + 10 * 5);
            pY0 = (const xtfloatx2*)(y + (0) * Q);
            pWwr = (xtfloatx2*)(pwr + (M-1-i)*10);
            for (j = N - 1; j < Q; j += 4)
            {
                w0 = XT_LSX2I(pWwr, 0);
                w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
                w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
                w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
                w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
                XT_LSX2IP(y01, pY0, sizeof(xtfloatx2));
                XT_LSX2IP(y23, pY0, sizeof(xtfloatx2));
                y45 = XT_LSX2I(pY0, 0);
                y67 = XT_LSX2I(pY0, sizeof(xtfloatx2));
                y12 = XT_SEL32_LH_SX2(y01, y23);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                y56 = XT_SEL32_LH_SX2(y45, y67);
                z01 = XT_MUL_SX2(w0, y01);
                z23 = XT_MUL_SX2(w0, y23);
                XT_MADD_SX2(z01, w1, y12);
                XT_MADD_SX2(z23, w1, y34);
                XT_MADD_SX2(z01, w2, y23);
                XT_MADD_SX2(z23, w2, y45);
                XT_MADD_SX2(z01, w3, y34);
                XT_MADD_SX2(z23, w3, y56);
                XT_MADD_SX2(z01, w4, y45);
                XT_MADD_SX2(z23, w4, y67);
                XT_SSX2IP(z01, pz0, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pz0, sizeof(xtfloatx2));
            }
        }
        for (n = 0; n < i; n++)
        {
            pz0 = (xtfloatx2 *)(pwr + 5*10 + Q*(n&1));            
            pz1 = (xtfloatx2 *)(pwr + 5 * 10 + Q*(1 - (n & 1)));
            if (n == (i - 1))
            {
                pz1 = (xtfloatx2 *)(z + i*(N + Q - 1) + (N - 1));
            }
            pY1 = (const xtfloatx2*)(y + (1 + n) * Q);
            pWwr = (xtfloatx2*)(pwr + (M - i + n) * 10);
            for (j = N - 1; j < Q; j += 4)
            {
                XT_LSX2IP(z01, pz0, sizeof(xtfloatx2));
                XT_LSX2IP(z23, pz0, sizeof(xtfloatx2));
                XT_LSX2IP(y01, pY1, sizeof(xtfloatx2));                                        
                XT_LSX2XP(w0, pWwr, 0);
                z01_0 = XT_MUL_SX2(w0, y01);
                XT_LSX2IP(y23, pY1, sizeof(xtfloatx2));
                z23_0 = XT_MUL_SX2(w0, y23);
                w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
                XT_MADD_SX2(z01, w2, y23);
                y45 = XT_LSX2I(pY1, 0);
                XT_MADD_SX2(z23, w2, y45);
                w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
                XT_MADD_SX2(z01_0, w4, y45);
                y67 = XT_LSX2I(pY1, sizeof(xtfloatx2));
                XT_MADD_SX2(z23_0, w4, y67);
                w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
                y12 = XT_SEL32_LH_SX2(y01, y23);
                XT_MADD_SX2(z01, w1, y12);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                XT_MADD_SX2(z23, w1, y34); 
                w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
                XT_MADD_SX2(z01_0, w3, y34);
                y56 = XT_SEL32_LH_SX2(y45, y67);
                XT_MADD_SX2(z23_0, w3, y56); 

                XT_MADD_SX2(z23, z23_0, XT_CONST_S(1));
                XT_MADD_SX2(z01, z01_0, XT_CONST_S(1));
                XT_SSX2IP(z01, pz1, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pz1, sizeof(xtfloatx2));
            }
        }

        /* last  N-1 elements in a row */
        tmp = (xtfloatx2)0.0f;
        z01 = (xtfloatx2)0.0f;
        z23 = (xtfloatx2)0.0f;
        pY0 = (const xtfloatx2*)(y + Q - 4);
        pWwr = (xtfloatx2*)(pwr + (M - 1 - i) * 10);
        for (n = 0; n < i + 1; n++)
        {
            w0 = XT_LSX2I(pWwr, 0*sizeof(xtfloatx2));
            w1 = XT_LSX2I(pWwr, 1*sizeof(xtfloatx2));
            w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0 * sizeof(xtfloatx2));
            y23 = XT_LSX2I(pY0, 1 * sizeof(xtfloatx2));
            y12 = XT_SEL32_LH_SX2(y01, y23);
            y56 = XT_SEL32_LH_SX2(y23, tmp);
            XT_MADD_SX2(z01, w0, y01);
            XT_MADD_SX2(z01, w1, y12);
            XT_MADD_SX2(z01, w2, y23);
            XT_MADD_SX2(z01, w3, y56);
            XT_MADD_SX2(z23, w0, y23);
            XT_MADD_SX2(z23, w1, y56);
            pWwr += 5;
            pY0 = (xtfloatx2*)((xtfloat*)pY0 + Q);
        }
        pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + Q);
        XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));
    }

    /* Middle rows */
    for (i = M-1; i < P; i++)
    {
        /* first  N-1 elements in a row */
        tmp = (xtfloatx2)0.0f;
        z01 = (xtfloatx2)0.0f;
        z23 = (xtfloatx2)0.0f;
        pY0 = (const xtfloatx2*)(y + (i - (M - 1)) * Q);
        pWwr = (xtfloatx2*)(pwr);
        for (n = 0; n < M; n++)
        {        
            w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
            w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
            w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0*sizeof(xtfloatx2));
            y23 = XT_LSX2I(pY0, 1*sizeof(xtfloatx2));
            y12 = XT_SEL32_LH_SX2(y01, y23);
            y56 = XT_SEL32_LH_SX2(tmp, y01);
            XT_MADD_SX2(z01, w4, y01);
            XT_MADD_SX2(z23, w4, y23);
            XT_MADD_SX2(z01, w3, y56);
            XT_MADD_SX2(z23, w3, y12);
            XT_MADD_SX2(z23, w2, y01);
            XT_MADD_SX2(z23, w1, y56);
            pWwr += 5;
            pY0 = (xtfloatx2*)((xtfloat*)pY0 + Q);
        }
        pZ = (xtfloatx2 *)(z + i*(N + Q - 1));
        XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));

        /* Main section */
        pz0 = (xtfloatx2 *)(pwr + 5 * 10);
        pY0 = (const xtfloatx2*)(y + (i - (M - 1) + 0) * Q);
        pWwr = (xtfloatx2*)pwr;
        for (j = N - 1; j < Q; j += 4)
        {
            XT_LSX2IP(y01, pY0, sizeof(xtfloatx2));
            XT_LSX2XP(w0, pWwr, 0);
            z01_0 = XT_MUL_SX2(w0, y01);
            XT_LSX2IP(y23, pY0, sizeof(xtfloatx2));
            z23_0 = XT_MUL_SX2(w0, y23);
            w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            z01 = XT_MUL_SX2(w2, y23);
            y45 = XT_LSX2I(pY0, 0);
            z23 = XT_MUL_SX2(w2, y45);
            w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01_0, w4, y45);
            y67 = XT_LSX2I(pY0, sizeof(xtfloatx2));
            XT_MADD_SX2(z23_0, w4, y67);
            w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
            y12 = XT_SEL32_LH_SX2(y01, y23);
            XT_MADD_SX2(z01, w1, y12);
            y34 = XT_SEL32_LH_SX2(y23, y45);
            XT_MADD_SX2(z23, w1, y34);
            w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01_0, w3, y34);
            y56 = XT_SEL32_LH_SX2(y45, y67);
            XT_MADD_SX2(z23_0, w3, y56);

            XT_MADD_SX2(z23, z23_0, XT_CONST_S(1));
            XT_MADD_SX2(z01, z01_0, XT_CONST_S(1));
            XT_SSX2IP(z01, pz0, sizeof(xtfloatx2));
            XT_SSX2IP(z23, pz0, sizeof(xtfloatx2));
        }

        pz0 = (xtfloatx2 *)(pwr + 5 * 10);
        pz1 = (xtfloatx2 *)(pwr + 5 * 10 + Q);
        pY1 = (const xtfloatx2*)(y + (i - (M - 1) + 1) * Q);
        pWwr = (xtfloatx2*)(pwr + 10);
        for (j = N - 1; j < Q; j += 4)
        {
            XT_LSX2IP(z01, pz0, sizeof(xtfloatx2));
            XT_LSX2IP(z23, pz0, sizeof(xtfloatx2));
            XT_LSX2IP(y01, pY1, sizeof(xtfloatx2));
            XT_LSX2XP(w0, pWwr, 0);
            z01_0 = XT_MUL_SX2(w0, y01);
            XT_LSX2IP(y23, pY1, sizeof(xtfloatx2));
            z23_0 = XT_MUL_SX2(w0, y23);
            w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01, w2, y23);
            y45 = XT_LSX2I(pY1, 0);
            XT_MADD_SX2(z23, w2, y45);
            w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01_0, w4, y45);
            y67 = XT_LSX2I(pY1, sizeof(xtfloatx2));
            XT_MADD_SX2(z23_0, w4, y67);
            w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
            y12 = XT_SEL32_LH_SX2(y01, y23);
            XT_MADD_SX2(z01, w1, y12);
            y34 = XT_SEL32_LH_SX2(y23, y45);
            XT_MADD_SX2(z23, w1, y34);
            w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01_0, w3, y34);
            y56 = XT_SEL32_LH_SX2(y45, y67);
            XT_MADD_SX2(z23_0, w3, y56);

            XT_MADD_SX2(z23, z23_0, XT_CONST_S(1));
            XT_MADD_SX2(z01, z01_0, XT_CONST_S(1));
            XT_SSX2IP(z01, pz1, sizeof(xtfloatx2));
            XT_SSX2IP(z23, pz1, sizeof(xtfloatx2));
        }

        pz0 = (xtfloatx2 *)(pwr + 5 * 10);
        pz1 = (xtfloatx2 *)(pwr + 5 * 10 + Q);
        pY2 = (const xtfloatx2*)(y + (i - (M - 1) + 2) * Q);
        pWwr = (xtfloatx2*)(pwr + 2*10);
        for (j = N - 1; j < Q; j += 4)
        {
            XT_LSX2IP(z01, pz1, sizeof(xtfloatx2));
            XT_LSX2IP(z23, pz1, sizeof(xtfloatx2));
            XT_LSX2IP(y01, pY2, sizeof(xtfloatx2));
            XT_LSX2XP(w0, pWwr, 0);
            z01_0 = XT_MUL_SX2(w0, y01);
            XT_LSX2IP(y23, pY2, sizeof(xtfloatx2));
            z23_0 = XT_MUL_SX2(w0, y23);
            w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01, w2, y23);
            y45 = XT_LSX2I(pY2, 0);
            XT_MADD_SX2(z23, w2, y45);
            w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01_0, w4, y45);
            y67 = XT_LSX2I(pY2, sizeof(xtfloatx2));
            XT_MADD_SX2(z23_0, w4, y67);
            w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
            y12 = XT_SEL32_LH_SX2(y01, y23);
            XT_MADD_SX2(z01, w1, y12);
            y34 = XT_SEL32_LH_SX2(y23, y45);
            XT_MADD_SX2(z23, w1, y34);
            w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01_0, w3, y34);
            y56 = XT_SEL32_LH_SX2(y45, y67);
            XT_MADD_SX2(z23_0, w3, y56);

            XT_MADD_SX2(z23, z23_0, XT_CONST_S(1));
            XT_MADD_SX2(z01, z01_0, XT_CONST_S(1));
            XT_SSX2IP(z01, pz0, sizeof(xtfloatx2));
            XT_SSX2IP(z23, pz0, sizeof(xtfloatx2));
        }

        pz0 = (xtfloatx2 *)(pwr + 5 * 10);
        pz1 = (xtfloatx2 *)(pwr + 5 * 10 + Q);
        pY3 = (const xtfloatx2*)(y + (i - (M - 1) + 3) * Q);
        pWwr = (xtfloatx2*)(pwr + 3*10);
        for (j = N - 1; j < Q; j += 4)
        {
            XT_LSX2IP(z01, pz0, sizeof(xtfloatx2));
            XT_LSX2IP(z23, pz0, sizeof(xtfloatx2));
            XT_LSX2IP(y01, pY3, sizeof(xtfloatx2));
            XT_LSX2XP(w0, pWwr, 0);
            z01_0 = XT_MUL_SX2(w0, y01);
            XT_LSX2IP(y23, pY3, sizeof(xtfloatx2));
            z23_0 = XT_MUL_SX2(w0, y23);
            w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01, w2, y23);
            y45 = XT_LSX2I(pY3, 0);
            XT_MADD_SX2(z23, w2, y45);
            w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01_0, w4, y45);
            y67 = XT_LSX2I(pY3, sizeof(xtfloatx2));
            XT_MADD_SX2(z23_0, w4, y67);
            w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
            y12 = XT_SEL32_LH_SX2(y01, y23);
            XT_MADD_SX2(z01, w1, y12);
            y34 = XT_SEL32_LH_SX2(y23, y45);
            XT_MADD_SX2(z23, w1, y34);
            w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01_0, w3, y34);
            y56 = XT_SEL32_LH_SX2(y45, y67);
            XT_MADD_SX2(z23_0, w3, y56);

            XT_MADD_SX2(z23, z23_0, XT_CONST_S(1));
            XT_MADD_SX2(z01, z01_0, XT_CONST_S(1));
            XT_SSX2IP(z01, pz1, sizeof(xtfloatx2));
            XT_SSX2IP(z23, pz1, sizeof(xtfloatx2));
        }
        pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + (N-1));
        pz1 = (xtfloatx2 *)(pwr + 5 * 10 + Q);
        pY4 = (const xtfloatx2*)(y + (i - (M - 1) + 4) * Q);
        pWwr = (xtfloatx2*)(pwr + 4*10);
        for (j = N - 1; j < Q; j += 4)
        {
            XT_LSX2IP(z01, pz1, sizeof(xtfloatx2));
            XT_LSX2IP(z23, pz1, sizeof(xtfloatx2));
            XT_LSX2IP(y01, pY4, sizeof(xtfloatx2));
            XT_LSX2XP(w0, pWwr, 0);
            z01_0 = XT_MUL_SX2(w0, y01);
            XT_LSX2IP(y23, pY4, sizeof(xtfloatx2));
            z23_0 = XT_MUL_SX2(w0, y23);
            w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01, w2, y23);
            y45 = XT_LSX2I(pY4, 0);
            XT_MADD_SX2(z23, w2, y45);
            w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01_0, w4, y45);
            y67 = XT_LSX2I(pY4, sizeof(xtfloatx2));
            XT_MADD_SX2(z23_0, w4, y67);
            w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
            y12 = XT_SEL32_LH_SX2(y01, y23);
            XT_MADD_SX2(z01, w1, y12);
            y34 = XT_SEL32_LH_SX2(y23, y45);
            XT_MADD_SX2(z23, w1, y34);
            w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
            XT_MADD_SX2(z01_0, w3, y34);
            y56 = XT_SEL32_LH_SX2(y45, y67);
            XT_MADD_SX2(z23_0, w3, y56);

            XT_MADD_SX2(z23, z23_0, XT_CONST_S(1));
            XT_MADD_SX2(z01, z01_0, XT_CONST_S(1));
            XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
            XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));
        }

        /* last  N-1 elements in a row */
        tmp = (xtfloatx2)0.0f;
        z01 = (xtfloatx2)0.0f;
        z23 = (xtfloatx2)0.0f;
        pY0 = (const xtfloatx2*)(y + (i - (M - 1)) * Q + (Q - 4));
        pWwr = (xtfloatx2*)(pwr);
        for (n = 0; n < M; n++)
        {
            w0 = XT_LSX2I(pWwr, 0*sizeof(xtfloatx2));
            w1 = XT_LSX2I(pWwr, 1*sizeof(xtfloatx2));
            w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0 * sizeof(xtfloatx2));
            y23 = XT_LSX2I(pY0, 1 * sizeof(xtfloatx2));
            y12 = XT_SEL32_LH_SX2(y01, y23);
            y56 = XT_SEL32_LH_SX2(y23, tmp);
            XT_MADD_SX2(z01, w0, y01);
            XT_MADD_SX2(z01, w1, y12);
            XT_MADD_SX2(z01, w2, y23);
            XT_MADD_SX2(z01, w3, y56);
            XT_MADD_SX2(z23, w0, y23);
            XT_MADD_SX2(z23, w1, y56);
            pWwr += 5;
            pY0 = (xtfloatx2*)((xtfloat*)pY0 + Q);
        }
        pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + Q);
        XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));
    }

    /* Last M-1 rows */
    for (i = P; i < P+M-1; i++)
    {
        /* first  N-1 elements in a row */
        tmp = (xtfloatx2)0.0f;
        z01 = (xtfloatx2)0.0f;
        z23 = (xtfloatx2)0.0f;
        pY0 = (const xtfloatx2*)(y + (i-(M-1))*Q);
        pWwr = (xtfloatx2*)(pwr);
        for (n = 0; n < (M+P-1-i); n++)
        {        
            w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
            w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
            w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0*sizeof(xtfloatx2));
            y23 = XT_LSX2I(pY0, 1*sizeof(xtfloatx2));
            y12 = XT_SEL32_LH_SX2(y01, y23);
            y56 = XT_SEL32_LH_SX2(tmp, y01);
            XT_MADD_SX2(z01, w4, y01);
            XT_MADD_SX2(z23, w4, y23);
            XT_MADD_SX2(z01, w3, y56);
            XT_MADD_SX2(z23, w3, y12);
            XT_MADD_SX2(z23, w2, y01);
            XT_MADD_SX2(z23, w1, y56);
            pWwr += 5;
            pY0 = (xtfloatx2*)((xtfloat*)pY0 + Q);
        }
        pZ = (xtfloatx2 *)(z + i*(N + Q - 1));
        XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));

        /* middle section */
        if (i == (P+3))
        {
            pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + (N-1));
            pY0 = (const xtfloatx2*)(y + (i-4) * Q);
            pWwr = (xtfloatx2*)(pwr);
            for (j = N - 1; j < Q; j += 4)
            {
                w0 = XT_LSX2I(pWwr, 0);
                w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
                w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
                w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
                w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
                XT_LSX2IP(y01, pY0, sizeof(xtfloatx2));
                XT_LSX2IP(y23, pY0, sizeof(xtfloatx2));
                y45 = XT_LSX2I(pY0, 0);
                y67 = XT_LSX2I(pY0, sizeof(xtfloatx2));
                y12 = XT_SEL32_LH_SX2(y01, y23);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                y56 = XT_SEL32_LH_SX2(y45, y67);
                z01 = XT_MUL_SX2(w0, y01);
                z23 = XT_MUL_SX2(w0, y23);
                XT_MADD_SX2(z01, w1, y12);
                XT_MADD_SX2(z23, w1, y34);
                XT_MADD_SX2(z01, w2, y23);
                XT_MADD_SX2(z23, w2, y45);
                XT_MADD_SX2(z01, w3, y34);
                XT_MADD_SX2(z23, w3, y56);
                XT_MADD_SX2(z01, w4, y45);
                XT_MADD_SX2(z23, w4, y67);
                XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));
            }
        }
        else
        {
            pz0 = (xtfloatx2 *)(pwr + 5 * 10);
            pY0 = (const xtfloatx2*)(y + (i - (M - 1))*Q);
            pWwr = (xtfloatx2*)(pwr);
            for (j = N - 1; j < Q; j += 4)
            {
                w0 = XT_LSX2I(pWwr, 0);
                w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
                w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
                w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
                w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
                XT_LSX2IP(y01, pY0, sizeof(xtfloatx2));
                XT_LSX2IP(y23, pY0, sizeof(xtfloatx2));
                y45 = XT_LSX2I(pY0, 0);
                y67 = XT_LSX2I(pY0, sizeof(xtfloatx2));
                y12 = XT_SEL32_LH_SX2(y01, y23);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                y56 = XT_SEL32_LH_SX2(y45, y67);
                z01 = XT_MUL_SX2(w0, y01);
                z23 = XT_MUL_SX2(w0, y23);
                XT_MADD_SX2(z01, w1, y12);
                XT_MADD_SX2(z23, w1, y34);
                XT_MADD_SX2(z01, w2, y23);
                XT_MADD_SX2(z23, w2, y45);
                XT_MADD_SX2(z01, w3, y34);
                XT_MADD_SX2(z23, w3, y56);
                XT_MADD_SX2(z01, w4, y45);
                XT_MADD_SX2(z23, w4, y67);
                XT_SSX2IP(z01, pz0, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pz0, sizeof(xtfloatx2));
            }
        }
        for (n = 0; n < (M+P-2-i); n++)
        {
            pz0 = (xtfloatx2 *)(pwr + 5 * 10 + Q*(n & 1));
            pz1 = (xtfloatx2 *)(pwr + 5 * 10 + Q*(1 - (n & 1)));
            if (n == (M + P - 2 - i - 1))
            {
                pz1 = (xtfloatx2 *)(z + i*(N + Q - 1) + (N - 1));
            }
            pY1 = (const xtfloatx2*)(y + (i - M + 1 + 1 + n) * Q);
            pWwr = (xtfloatx2*)(pwr + (n+1) * 10);
            for (j = N - 1; j < Q; j += 4)
            {
                XT_LSX2IP(z01, pz0, sizeof(xtfloatx2));
                XT_LSX2IP(z23, pz0, sizeof(xtfloatx2));
                XT_LSX2IP(y01, pY1, sizeof(xtfloatx2));
                XT_LSX2XP(w0, pWwr, 0);
                z01_0 = XT_MUL_SX2(w0, y01);
                XT_LSX2IP(y23, pY1, sizeof(xtfloatx2));
                z23_0 = XT_MUL_SX2(w0, y23);
                w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
                XT_MADD_SX2(z01, w2, y23);
                y45 = XT_LSX2I(pY1, 0);
                XT_MADD_SX2(z23, w2, y45);
                w4 = XT_LSX2I(pWwr, 4 * sizeof(xtfloatx2));
                XT_MADD_SX2(z01_0, w4, y45);
                y67 = XT_LSX2I(pY1, sizeof(xtfloatx2));
                XT_MADD_SX2(z23_0, w4, y67);
                w1 = XT_LSX2I(pWwr, sizeof(xtfloatx2));
                y12 = XT_SEL32_LH_SX2(y01, y23);
                XT_MADD_SX2(z01, w1, y12);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                XT_MADD_SX2(z23, w1, y34);
                w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
                XT_MADD_SX2(z01_0, w3, y34);
                y56 = XT_SEL32_LH_SX2(y45, y67);
                XT_MADD_SX2(z23_0, w3, y56);

                XT_MADD_SX2(z23, z23_0, XT_CONST_S(1));
                XT_MADD_SX2(z01, z01_0, XT_CONST_S(1));
                XT_SSX2IP(z01, pz1, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pz1, sizeof(xtfloatx2));
            }
        }

        /* last  N-1 elements in a row */
        tmp = (xtfloatx2)0.0f;
        z01 = (xtfloatx2)0.0f;
        z23 = (xtfloatx2)0.0f;
        pY0 = (const xtfloatx2*)(y + (i - (M - 1))*Q + Q - 4);
        pWwr = (xtfloatx2*)(pwr);
        for (n = 0; n < (M+P-1-i); n++)
        {
            w0 = XT_LSX2I(pWwr, 0*sizeof(xtfloatx2));
            w1 = XT_LSX2I(pWwr, 1*sizeof(xtfloatx2));
            w2 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            w3 = XT_LSX2I(pWwr, 3 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0 * sizeof(xtfloatx2));
            y23 = XT_LSX2I(pY0, 1 * sizeof(xtfloatx2));
            y12 = XT_SEL32_LH_SX2(y01, y23);
            y56 = XT_SEL32_LH_SX2(y23, tmp);
            XT_MADD_SX2(z01, w0, y01);
            XT_MADD_SX2(z01, w1, y12);
            XT_MADD_SX2(z01, w2, y23);
            XT_MADD_SX2(z01, w3, y56);
            XT_MADD_SX2(z23, w0, y23);
            XT_MADD_SX2(z23, w1, y56);
            pWwr += 5;
            pY0 = (xtfloatx2*)((xtfloat*)pY0 + Q);
        }
        pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + Q);
        XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));
    }
#undef M
#undef N
}

size_t conv2d_5x5f_getScratchSize (int P, int Q)
{
    (void)P,(void)Q;
    return 5*5*sizeof(xtfloatx2) +(2*Q)*sizeof(xtfloat); // Enough to store 5x(2*5) matrix and 2*Q temp elements
} // MxN=5x5



#else //SFPU

void conv2d_5x5f      (void* pScr, float32_t *z, const float32_t * x, const float32_t * y, int P, int Q)
{
    NASSERT_ALIGN(x, 8);
    NASSERT_ALIGN(y, 8);
    NASSERT_ALIGN(z, 8);
    NASSERT_ALIGN(pScr, 8);
    NASSERT(P >= 0 && P % 8 == 0);
    NASSERT(Q >= 0 && Q % 8 == 0);
    int i, j, m, n, n0, n1, m0, m1;
    float32_t * pwr = (float32_t*)pScr;
    xtfloat * restrict pWwr = (xtfloat*)pwr;
    const xtfloat * restrict pY;
    xtfloat * restrict pZ;
    xtfloat * restrict pX;
    xtfloat * restrict pW;
    xtfloat w0, w1, w2, w3, w4;
    xtfloat y0, y1, y2, y3, y4, y5, y6, y7;
    xtfloat s0, s1, s2, s3;
    if (P <= 0 || Q <= 0) return;

#define M 5
#define N 5
    pX = (xtfloat*)x;
    pW = (xtfloat*)pScr + M*N-1;
    for (i = 0; i < M*N; i++)
    {
        XT_LSIP(w0, pX, sizeof(xtfloat));
        XT_SSXP(w0, pW, -1 * ((int)sizeof(xtfloat)));
    }

    /* first M-1 rows */
    for (i = 0; i < M - 1; i++)
    {
        for (j = 0; j < N - 1; j++)
        {
            float32_t s;
            m0 = XT_MAX(i - P + 1, 0);
            m1 = XT_MIN(i + 1, M);
            n0 = XT_MAX(j - Q + 1, 0);
            n1 = XT_MIN(j + 1, N);
            s = 0;
            for (n = n0; n < n1; n++)
            for (m = m0; m < m1; m++)
            {
                s += x[m*N + n] * y[(i - m)*Q + (j - n)];
            }
            z[i*(N + Q - 1) + j] = s;
        }
        pZ = (xtfloat *)(z + i*(N + Q - 1) + (N-1));

        for (j = N - 1; j < Q; j++)
        {
            xtfloat s = XT_MOV_S(0.0f);
            pWwr = (xtfloat*)(pwr + ((M-1) - i)*N);
            n0 = (j - (N - 1));
            for (m = 0; m < i+1; m++)
            {
                pY = (const xtfloat *)(y + (m)*Q + n0);
                for (n = 0; n < N; n++)
                {
                    XT_LSIP(w0, pWwr, sizeof(xtfloat));
                    XT_LSIP(y0, pY, sizeof(xtfloat));
                    XT_MADD_S(s, w0, y0);
                }
            }
            XT_SSIP(s, pZ, sizeof(xtfloat));
        }

        for (j = Q; j < N + Q - 1; j++)
        {
            float32_t s;
            m0 = XT_MAX(i - P + 1, 0);
            m1 = XT_MIN(i + 1, M);
            n0 = XT_MAX(j - Q + 1, 0);
            n1 = XT_MIN(j + 1, N);
            s = 0;
            for (n = n0; n < n1; n++)
            for (m = m0; m < m1; m++)
            {
                s += x[m*N + n] * y[(i - m)*Q + (j - n)];
            }
            z[i*(N + Q - 1) + j] = s;
        }

    }

    for (i = M-1; i < P; i++)
    {
        pZ = (xtfloat *)(z + i*(N + Q - 1));
        for (j = 0; j < N - 1; j++)
        {
            m0 = i - (M - 1);
            n0 = (N - 1) - j;
            xtfloat s = XT_MOV_S(0.0f);
            for (m = 0; m < M; m++)
            { 
                pY = (const xtfloat *)(y + (m0 + m)*Q);
                pWwr = (xtfloat *)(pwr + m*N + n0);
                for (n = 0; n < j+1; n++)
                {
                    XT_LSIP(w0, pWwr, sizeof(xtfloat));
                    XT_LSIP(y0, pY, sizeof(xtfloat));
                    XT_MADD_S(s, w0, y0);
                }
            }
            XT_SSIP(s, pZ, sizeof(xtfloat));
        }

        /* Main section */
        for (j = N-1; j < Q; j+=4)
        {
            s0 = XT_MOV_S(0.0f);
            s1 = XT_MOV_S(0.0f);
            s2 = XT_MOV_S(0.0f);
            s3 = XT_MOV_S(0.0f);
            pWwr = (xtfloat*)pwr;
            m0 = i - (M - 1);
            n0 = (j - (N - 1));
            for (m = 0; m < M; m++)
            {  
                pY = (const xtfloat *)(y + (m0 + m)*Q + n0);
                XT_LSIP(w0, pWwr, sizeof(xtfloat));
                XT_LSIP(w1, pWwr, sizeof(xtfloat));
                XT_LSIP(w2, pWwr, sizeof(xtfloat));
                XT_LSIP(w3, pWwr, sizeof(xtfloat));
                XT_LSIP(w4, pWwr, sizeof(xtfloat));
                XT_LSIP(y0, pY, sizeof(xtfloat));
                XT_LSIP(y1, pY, sizeof(xtfloat));
                XT_LSIP(y2, pY, sizeof(xtfloat));
                XT_LSIP(y3, pY, sizeof(xtfloat));
                XT_LSIP(y4, pY, sizeof(xtfloat));
                XT_LSIP(y5, pY, sizeof(xtfloat));
                XT_LSIP(y6, pY, sizeof(xtfloat));
                XT_LSIP(y7, pY, sizeof(xtfloat));

                XT_MADD_S(s0, w0, y0);
                XT_MADD_S(s0, w1, y1);
                XT_MADD_S(s0, w2, y2);
                XT_MADD_S(s0, w3, y3);
                XT_MADD_S(s0, w4, y4);

                XT_MADD_S(s1, w0, y1);
                XT_MADD_S(s1, w1, y2);
                XT_MADD_S(s1, w2, y3);
                XT_MADD_S(s1, w3, y4);
                XT_MADD_S(s1, w4, y5);

                XT_MADD_S(s2, w0, y2);
                XT_MADD_S(s2, w1, y3);
                XT_MADD_S(s2, w2, y4);
                XT_MADD_S(s2, w3, y5);
                XT_MADD_S(s2, w4, y6);

                XT_MADD_S(s3, w0, y3);
                XT_MADD_S(s3, w1, y4);
                XT_MADD_S(s3, w2, y5);
                XT_MADD_S(s3, w3, y6);
                XT_MADD_S(s3, w4, y7);
            }
            XT_SSIP(s0, pZ, sizeof(xtfloat));
            XT_SSIP(s1, pZ, sizeof(xtfloat));
            XT_SSIP(s2, pZ, sizeof(xtfloat));
            XT_SSIP(s3, pZ, sizeof(xtfloat));
        }

        for (j = Q; j < N + Q - 1; j++)
        {
            xtfloat s = XT_MOV_S(0.0f);
            n1 = N + Q - j - 1;
            m0 = i - (M - 1);
            n0 = j - (N - 1);
            for (m = 0; m < M; m++)
            {
                pWwr = (xtfloat *)(pwr + m*N);
                pY = (const xtfloat *)(y + (m0 + m)*Q + n0);
                for (n = 0; n < n1; n++)
                {
                    XT_LSIP(w0, pWwr, sizeof(xtfloat));
                    XT_LSIP(y0, pY, sizeof(xtfloat));
                    XT_MADD_S(s, w0, y0);
                }
            }
            XT_SSIP(s, pZ, sizeof(xtfloat));
        }
    }

    /* Last M-1 rows */
    for (i = P; i < M + P - 1; i++)
    {
        for (j = 0; j < N - 1; j++)
        {
            float32_t s;
            m0 = XT_MAX(i - P + 1, 0);
            m1 = XT_MIN(i + 1, M);
            n0 = XT_MAX(j - Q + 1, 0);
            n1 = XT_MIN(j + 1, N);
            s = 0;
            for (n = n0; n < n1; n++)
            for (m = m0; m < m1; m++)
            {
                s += x[m*N + n] * y[(i - m)*Q + (j - n)];
            }
            z[i*(N + Q - 1) + j] = s;
        }

        pZ = (xtfloat *)(z + i*(N + Q - 1) + (N - 1));

        for (j = N - 1; j < Q; j++)
        {
            xtfloat s = XT_MOV_S(0.0f);
            pWwr = (xtfloat*)pwr;
            n0 = (j - (N - 1));
            m1 = (M + P - i - 1);
            m0 = i - M + 1;
            for (m = 0; m < m1; m++)
            {
                pY = (const xtfloat *)(y + (m0+m)*Q + n0);
                for (n = 0; n < N; n++)
                {
                    XT_LSIP(w0, pWwr, sizeof(xtfloat));
                    XT_LSIP(y0, pY, sizeof(xtfloat));
                    XT_MADD_S(s, w0, y0);
                }
            }
            XT_SSIP(s, pZ, sizeof(xtfloat));
        }

        for (j = Q; j < N + Q - 1; j++)
        {
            float32_t s;
            m0 = XT_MAX(i - P + 1, 0);
            m1 = XT_MIN(i + 1, M);
            n0 = XT_MAX(j - Q + 1, 0);
            n1 = XT_MIN(j + 1, N);
            s = 0;
            for (n = n0; n < n1; n++)
            for (m = m0; m < m1; m++)
            {
                s += x[m*N + n] * y[(i - m)*Q + (j - n)];
            }
            z[i*(N + Q - 1) + j] = s;
        }
    }
#undef M
#undef N
}

size_t conv2d_5x5f_getScratchSize(int P, int Q)
{
    if (P <= 0 || Q <= 0)  return 0;
    return 5*5*sizeof(xtfloat); // for 5x5 matrix
} // MxN=5x5


#endif
