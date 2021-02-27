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
    Real data linear auto-correlation, floating point, no requirements on vectors
    length and alignment.
*/

/*-------------------------------------------------------------------------
  Linear Autocorrelation 
  Functions estimate the linear auto-correlation of vector x. Returns 
  autocorrelation of length N.

  Precision: 
  16x16   16-bit data, 16-bit outputs
  32x32   32-bit data, 32-bit outputs
  f       floating point

  Input:
  s[]       scratch area of
            FIR_LACORRA16X16_SCRATCH_SIZE( N )
            FIR_LACORRA32X32_SCRATCH_SIZE( N )
            FIR_LACORRAF_SCRATCH_SIZE( N )    
            bytes
  x[N]      input data Q31, Q15 or floating point
  N         length of x
  Output:
  r[N]      output data, Q31, Q15 or floating point

  Restrictions:
  x,r,s should not overlap
  N >0
  s   - aligned on an 8-bytes boundary
-------------------------------------------------------------------------*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
#include "common.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, fir_lacorraf, (void       * restrict s,
  float32_t  * restrict r,
  const float32_t  * restrict x, int N))
#else

#if 0
#include  "baseop.h"


void fir_lacorraf(    void             * restrict s,
                      float32_t        * restrict r, 
                      const float32_t  * restrict x, int N)
{
    float32_t acc;
    int n,k;
    for (k=0; k<N; k++)
    {
        acc=0.f;
        for (n=0; n<N-k; n++)
        {
            acc+= x[n+k] * x[n] ;
        }
        r[k]=acc;
    }
}
#else

void fir_lacorraf( void       * restrict s,
                   float32_t  * restrict r,
             const float32_t  * restrict x, int N)
{
    int m, n;

    int n_iter, m_iter;

    xtfloat A0, A1, A2, A3, X0, X1, X2, X3, Y0, Y1;

    const xtfloat * restrict pX;
    const xtfloat * restrict pY;
    const xtfloat * restrict pS;
          xtfloat * restrict pR;

    NASSERT(r && x && N > 0);

    pR = (xtfloat*)r;
    /*
    * Compute r[0] entries.
    */
    {
        A0 = XT_CONST_S(0);
        pX = (const xtfloat*)(x);
        for (m = 0; m<N; m++)
        {
            XT_LSIP(X0, pX, 4);
            XT_MADD_S(A0, X0, X0);
        }
        XT_SSIP(A0, pR, 4);
    }

    /*
    * Compute r[N+1]....r[N+M-1] entries.
    */
    n_iter = N - 1;
    m_iter = n_iter - 4;
    pS = (const xtfloat*)(x + 1);
    for (n = 0; n<(n_iter&~3); n += 4, m_iter -= 4)
    {
        xtfloat Y2, Y3, Y4;

        A0 = A1 = A2 = A3 = XT_CONST_S(0);

        pX = pS + n;
        pY = (const xtfloat*)x;
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
        pY = (const xtfloat*)x;

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

#endif
