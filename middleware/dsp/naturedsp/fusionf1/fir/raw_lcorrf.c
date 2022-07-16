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
#if (XCHAL_HAVE_FUSION_FP)
#include "raw_lcorrf.h"

/*-----------------------------------------------------
    raw linear correlation:
    Input:
    x[N] 
    y[M]
    Output:
    r[N+M-1]
    restriction:
    M should be >0
-----------------------------------------------------*/
void raw_lcorrf(float32_t  * restrict r,
          const float32_t  * restrict x,
          const float32_t  * restrict y,
                int N, int M)
{
    int m, n;

    int n_iter, m_iter;

    xtfloat A0, A1, A2, A3, X0, X1, X2, X3, X4, Y0, Y1;

    const xtfloat * restrict pX;
    const xtfloat * restrict pY;
    const xtfloat * restrict pS;
          xtfloat * restrict pR;

    pR = (xtfloat*)r;

    /*
    * Compute first M-1 entries.
    */
    pS = (const xtfloat*)(y + M - 4);
    n_iter = (M - 1);
    for (n = 0; n<(n_iter&~3); n += 4, pS -= 4)
    {
        A0 = A1 = A2 = A3 = XT_CONST_S(0);
        pX = (const xtfloat*)(x);
        pY = (pS);
        XT_LSIP(X0, pX, 4);
        XT_LSIP(X1, pX, 4);
        XT_LSIP(X2, pX, 4);
        XT_LSIP(X3, pX, 4);

        XT_LSIP(Y0, pY, 4);
        XT_LSIP(Y1, pY, 4);

        XT_MADD_S(A3, X0, Y0);
        XT_MADD_S(A3, X1, Y1);

        XT_MADD_S(A2, X0, Y1);

        XT_LSIP(Y0, pY, 4);
        XT_LSIP(Y1, pY, 4);

        XT_MADD_S(A3, X2, Y0);
        XT_MADD_S(A3, X3, Y1);

        XT_MADD_S(A2, X1, Y0);
        XT_MADD_S(A2, X2, Y1);

        XT_MADD_S(A1, X0, Y0);
        XT_MADD_S(A1, X1, Y1);

        XT_MADD_S(A0, X0, Y1);

        X0 = X1;
        X1 = X2;
        X2 = X3;

        for (m = 0; m < (n >> 1); m++)
        {
            XT_LSIP(X3, pX, 4);
            XT_LSIP(X4, pX, 4);

            XT_LSIP(Y0, pY, 4);
            XT_LSIP(Y1, pY, 4);
            XT_MADD_S(A0, X0, Y0);
            XT_MADD_S(A1, X1, Y0);
            XT_MADD_S(A2, X2, Y0);
            XT_MADD_S(A3, X3, Y0);
            XT_MADD_S(A0, X1, Y1);
            XT_MADD_S(A1, X2, Y1);
            XT_MADD_S(A2, X3, Y1);
            XT_MADD_S(A3, X4, Y1);
            X0 = X2;
            X1 = X3;
            X2 = X4;
        }
        if (n & 1)
        {
            XT_LSIP(X3, pX, 4);
            XT_LSIP(X4, pX, 4);

            XT_LSIP(Y0, pY, 4);
            XT_MADD_S(A0, X0, Y0);
            XT_MADD_S(A1, X1, Y0);
            XT_MADD_S(A2, X2, Y0);
            XT_MADD_S(A3, X3, Y0);
        }
        XT_SSIP(A0, pR, 4);
        XT_SSIP(A1, pR, 4);
        XT_SSIP(A2, pR, 4);
        XT_SSIP(A3, pR, 4);
    }
    /* last 1...3 iterations */
    n_iter &= 3;
    if (n_iter)
    {
        A0 = A1 = A2 = XT_CONST_S(0);
        pX = (const xtfloat*)(x);
        pY = (pS);
        XT_LSIP(X0, pX, 4);
        XT_LSIP(X1, pX, 4);
        XT_LSIP(X2, pX, 4);

        XT_LSIP(Y0, pY, 4);
        XT_LSIP(Y1, pY, 4);

        XT_MADD_S(A2, X0, Y1);

        XT_LSIP(Y0, pY, 4);
        XT_LSIP(Y1, pY, 4);

        XT_MADD_S(A2, X1, Y0);
        XT_MADD_S(A2, X2, Y1);

        XT_MADD_S(A1, X0, Y0);
        XT_MADD_S(A1, X1, Y1);

        XT_MADD_S(A0, X0, Y1);

        X0 = X1;
        X1 = X2;

        for (m = 0; m < (n >> 1); m++)
        {
            XT_LSIP(X2, pX, 4);
            XT_LSIP(X3, pX, 4);

            XT_LSIP(Y0, pY, 4);
            XT_LSIP(Y1, pY, 4);
            XT_MADD_S(A0, X0, Y0);
            XT_MADD_S(A1, X1, Y0);
            XT_MADD_S(A2, X2, Y0);
            XT_MADD_S(A0, X1, Y1);
            XT_MADD_S(A1, X2, Y1);
            XT_MADD_S(A2, X3, Y1);
            X0 = X2;
            X1 = X3;
        }
        if (n & 1)
        {
            XT_LSIP(Y0, pY, 4);
            XT_MADD_S(A0, X0, Y0);
            XT_MADD_S(A1, X1, Y0);
            XT_MADD_S(A2, X2, Y0);
        }
        XT_SSIP(A0, pR, 4);
        if (n_iter>1) XT_SSIP(A1, pR, 4);
        if (n_iter>2) XT_SSIP(A2, pR, 4);
    }

    /*
    * Compute r[M]...r[N] entries.
    */
    n_iter = (N - M + 1);
    for (n = 0; n<(n_iter&~3); n += 4)
    {
        A0 = A1 = A2 = A3 = XT_CONST_S(0);

        pX = (const xtfloat*)(x + n);
        pY = (const xtfloat*)y;
        XT_LSIP(X0, pX, 4);
        XT_LSIP(X1, pX, 4);
        XT_LSIP(X2, pX, 4);
        for (m = 0; m<(M >> 1); m++)
        {
            XT_LSIP(X3, pX, 4);
            XT_LSIP(X4, pX, 4);

            XT_LSIP(Y0, pY, 4);
            XT_LSIP(Y1, pY, 4);
            XT_MADD_S(A0, X0, Y0);
            XT_MADD_S(A1, X1, Y0);
            XT_MADD_S(A2, X2, Y0);
            XT_MADD_S(A3, X3, Y0);
            XT_MADD_S(A0, X1, Y1);
            XT_MADD_S(A1, X2, Y1);
            XT_MADD_S(A2, X3, Y1);
            XT_MADD_S(A3, X4, Y1);
            X0 = X2;
            X1 = X3;
            X2 = X4;
        }
        if (M & 1)
        {
            XT_LSIP(X3, pX, 4);
            XT_LSIP(X4, pX, 4);

            XT_LSIP(Y0, pY, 4);
            XT_MADD_S(A0, X0, Y0);
            XT_MADD_S(A1, X1, Y0);
            XT_MADD_S(A2, X2, Y0);
            XT_MADD_S(A3, X3, Y0);
        }
        XT_SSIP(A0, pR, 4);
        XT_SSIP(A1, pR, 4);
        XT_SSIP(A2, pR, 4);
        XT_SSIP(A3, pR, 4);
    }
    /* last 1...3 iterations */
    n_iter &= 3;
    if (n_iter)
    {
        A0 = A1 = A2 = XT_CONST_S(0);
        pX = (const xtfloat*)(x + ((N - M + 1)&~3));
        pY = (const xtfloat*)y;
        XT_LSIP(X0, pX, 4);
        XT_LSIP(X1, pX, 4);
        for (m = 0; m<(M >> 1); m++)
        {
            XT_LSIP(X2, pX, 4);
            XT_LSIP(X3, pX, 4);

            XT_LSIP(Y0, pY, 4);
            XT_LSIP(Y1, pY, 4);
            XT_MADD_S(A0, X0, Y0);
            XT_MADD_S(A1, X1, Y0);
            XT_MADD_S(A2, X2, Y0);
            XT_MADD_S(A0, X1, Y1);
            XT_MADD_S(A1, X2, Y1);
            XT_MADD_S(A2, X3, Y1);
            X0 = X2;
            X1 = X3;
        }
        if (M & 1)
        {
            XT_LSIP(X2, pX, 4);
            XT_LSIP(Y0, pY, 4);
            XT_MADD_S(A0, X0, Y0);
            XT_MADD_S(A1, X1, Y0);
            XT_MADD_S(A2, X2, Y0);
        }
        XT_SSIP(A0, pR, 4);
        if (n_iter>1) XT_SSIP(A1, pR, 4);
        if (n_iter>2) XT_SSIP(A2, pR, 4);
    }

    /*
    * Compute r[N+1]....r[N+M-1] entries.
    */
    n_iter = M - 1;
    m_iter = n_iter - 4;    
    pS = (const xtfloat*)(x + (N - M) + 1);
    for (n = 0; n<(n_iter&~3); n += 4, m_iter -= 4)
    {
        xtfloat Y2, Y3, Y4;

        A0 = A1 = A2 = A3 = XT_CONST_S(0);

        pX = pS + n;
        pY = (const xtfloat*)y;
        XT_LSIP(X0, pX, 4);
        XT_LSIP(X1, pX, 4);
        XT_LSIP(X2, pX, 4);
        XT_LSIP(X3, pX, 4);

        XT_LSIP(Y0, pY, 4);
        XT_LSIP(Y1, pY, 4);

        XT_MADD_S(A0, X0, Y0);
        XT_MADD_S(A0, X1, Y1);

        XT_MADD_S(A1, X1, Y0);
        XT_MADD_S(A1, X2, Y1);

        XT_MADD_S(A2, X2, Y0);
        XT_MADD_S(A2, X3, Y1);

        XT_MADD_S(A3, X3, Y0);

        XT_LSIP(Y2, pY, 4);
        XT_LSIP(Y3, pY, 4);

        XT_MADD_S(A0, X2, Y2);
        XT_MADD_S(A0, X3, Y3);

        XT_MADD_S(A1, X3, Y2);

        Y0 = Y1; Y1 = Y2; Y2 = Y3;

        for (m = 0; m<(m_iter >> 1); m++)
        {
            XT_LSIP(X0, pX, 4);
            XT_LSIP(X1, pX, 4);

            XT_LSIP(Y3, pY, 4);
            XT_LSIP(Y4, pY, 4);

            XT_MADD_S(A0, X0, Y3);
            XT_MADD_S(A1, X0, Y2);
            XT_MADD_S(A2, X0, Y1);
            XT_MADD_S(A3, X0, Y0);

            XT_MADD_S(A0, X1, Y4);
            XT_MADD_S(A1, X1, Y3);
            XT_MADD_S(A2, X1, Y2);
            XT_MADD_S(A3, X1, Y1);
            Y0 = Y2;
            Y1 = Y3;
            Y2 = Y4;
        }

        if (m_iter & 1)
        {
            XT_LSIP(X0, pX, 4);

            XT_LSIP(Y3, pY, 4);

            XT_MADD_S(A0, X0, Y3);
            XT_MADD_S(A1, X0, Y2);
            XT_MADD_S(A2, X0, Y1);
            XT_MADD_S(A3, X0, Y0);
        }

        XT_SSIP(A0, pR, 4);
        XT_SSIP(A1, pR, 4);
        XT_SSIP(A2, pR, 4);
        XT_SSIP(A3, pR, 4);
    }
    /* last 1...3 iterations */
    n_iter &= 3;
    if (n_iter)
    {
        A0 = A1 = A2 = XT_CONST_S(0);

        pX = (const xtfloat*)(x + N - 3);
        pY = (const xtfloat*)y;

        XT_LSIP(X0, pX, 4);
        XT_LSIP(X1, pX, 4);
        XT_LSIP(X2, pX, 4);

        XT_LSIP(Y0, pY, 4);
        XT_LSIP(Y1, pY, 4);

        XT_MADD_S(A0, X0, Y0);
        XT_MADD_S(A0, X1, Y1);

        XT_MADD_S(A1, X1, Y0);
        XT_MADD_S(A1, X2, Y1);

        XT_MADD_S(A2, X2, Y0);

        XT_LSIP(Y0, pY, 4);
        XT_MADD_S(A0, X2, Y0);

        if (n_iter>2) XT_SSIP(A0, pR, 4);
        if (n_iter>1) XT_SSIP(A1, pR, 4);
        XT_SSIP(A2, pR, 4);
    }
}
#endif

