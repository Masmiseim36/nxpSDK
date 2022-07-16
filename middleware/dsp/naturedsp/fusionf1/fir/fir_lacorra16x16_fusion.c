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
    Real data linear auto-correlation, 16x16, no requirements on vectors
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

/* Common utility and macros declarations. */
#include "common.h"
#include "baseop.h"
/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)

void fir_lacorra16x16( void     * restrict s,
                       int16_t  * restrict r,
                 const int16_t  * restrict x, int N)
{
    void     * s_ptr;
    int16_t  * x_buf;

    ae_int16x4       * restrict D;
    ae_int16         * restrict D_s;
    ae_valign S_va;
    int n,k;
    const ae_int16x4 *          S;
    ae_int16x4* restrict pR;
    const ae_int16x4* pX0;
    const ae_int16x4* pX1;
    const ae_int16x4* pX2;
    const ae_int16x4* pX3;
    const ae_int16x4* pY;

    NASSERT(s);
    NASSERT(r);
    NASSERT(x);
    NASSERT_ALIGN(s, 8);
    NASSERT(N>0);

    //----------------------------------------------------------------------------
    // Partition the scratch memory area.
    s_ptr = s;
    x_buf = (int16_t*)ALIGNED_ADDR(s_ptr, 8);
    s_ptr = x_buf + N + 6;

    ASSERT((int8_t *)s_ptr - (int8_t *)s <=(int)FIR_LACORRA16X16_SCRATCH_SIZE(N));
    //----------------------------------------------------------------------------
    // Copy x[N].
    S = (const ae_int16x4 *)x;
    D = (ae_int16x4 *)x_buf;
    S_va = AE_LA64_PP(S);
    for (n = 0; n<((N + 3) >> 2); n++)
    {
        ae_int16x4 x0;
        AE_LA16X4_IP(x0, S_va, S);
        AE_S16X4_IP(x0, D, +8);
    }
    /* append zero padding */
    D_s = (ae_int16 *)((int16_t *)x_buf + N);
    AE_S16_0_IP(AE_ZERO16(), D_s, +2);
    AE_S16_0_IP(AE_ZERO16(), D_s, +2);
    AE_S16_0_IP(AE_ZERO16(), D_s, +2);
    AE_S16_0_IP(AE_ZERO16(), D_s, +2);
    AE_S16_0_IP(AE_ZERO16(), D_s, +2);
    AE_S16_0_IP(AE_ZERO16(), D_s, +2);

    x=x_buf;
    __Pragma("no_reorder")
#if 0
    {
        int64_t acc[4];
        for (k=0; k<(N&~3); k+=4)
        {
            acc[0]=acc[1]=acc[2]=acc[3]=0;
            for (n=0; n<((N-k+3)&~3); n++)
            {
                acc[0]+=mpy_Q31Q15(x[n]<<16, x[n+k+0]);
                acc[1]+=mpy_Q31Q15(x[n]<<16, x[n+k+1]);
                acc[2]+=mpy_Q31Q15(x[n]<<16, x[n+k+2]);
                acc[3]+=mpy_Q31Q15(x[n]<<16, x[n+k+3]);
            }
            r[k+0]=S_round_l(satQ31(acc[0]>>16));
            r[k+1]=S_round_l(satQ31(acc[1]>>16));
            r[k+2]=S_round_l(satQ31(acc[2]>>16));
            r[k+3]=S_round_l(satQ31(acc[3]>>16));
        }
        if (k<N)
        {
            acc[0]=acc[1]=acc[2]=acc[3]=0;
            for (n=0; n<((N-k+3)&~3); n++)
            {
                acc[0]+=mpy_Q31Q15(x[n]<<16, x[n+k+0]);
                acc[1]+=mpy_Q31Q15(x[n]<<16, x[n+k+1]);
                acc[2]+=mpy_Q31Q15(x[n]<<16, x[n+k+2]);
                acc[3]+=mpy_Q31Q15(x[n]<<16, x[n+k+3]);
            }
            for(n=0; k<N; k++,n++)
            r[k]=S_round_l(satQ31(acc[n]>>16));
        }
    }
#else
    {
        int K;
        ae_f64 a0,a1,a2,a3;
        ae_int16x4 Y,X0,X1,X2,X3;
        ae_int32x2 Y0,Y1;
        ae_valign ar,ax1,ax2,ax3;
        
        ar=AE_ZALIGN64();
        pR=(ae_int16x4*)r;
        for (k=0,K=((N+3)>>2); k<(N&~3); k+=4,K--)
        {
            pY=(const ae_int16x4*)x;
            pX0=(const ae_int16x4*)(x+k+0);
            pX1=(const ae_int16x4*)(x+k+1);
            pX2=(const ae_int16x4*)(x+k+2);
            pX3=(const ae_int16x4*)(x+k+3);
            ax1=AE_LA64_PP(pX1);
            ax2=AE_LA64_PP(pX2);
            ax3=AE_LA64_PP(pX3);
            a0=a1=a2=a3=AE_ZERO64();
            NASSERT(K>=1);
            __Pragma("loop_count min=1")
            for (n=0; n<K; n++)
            {
                AE_L16X4_IP(Y,pY,sizeof(ae_int16x4));
                AE_L16X4_IP (X0,pX0,sizeof(ae_int16x4));
                AE_LA16X4_IP(X1,ax1,pX1);
                AE_LA16X4_IP(X2,ax2,pX2);
                AE_LA16X4_IP(X3,ax3,pX3);
                Y0=AE_CVT32X2F16_32(Y);
                Y1=AE_CVT32X2F16_10(Y);
                AE_MULAAFD32X16_H3_L2(a0,Y0,X0);
                AE_MULAAFD32X16_H3_L2(a1,Y0,X1);
                AE_MULAAFD32X16_H3_L2(a2,Y0,X2);
                AE_MULAAFD32X16_H3_L2(a3,Y0,X3);
                AE_MULAAFD32X16_H1_L0(a0,Y1,X0);
                AE_MULAAFD32X16_H1_L0(a1,Y1,X1);
                AE_MULAAFD32X16_H1_L0(a2,Y1,X2);
                AE_MULAAFD32X16_H1_L0(a3,Y1,X3);
            }
            Y0 = AE_TRUNCA32X2F64S(a0,a1,  16);
            Y1 = AE_TRUNCA32X2F64S(a2,a3,  16);
            Y  = AE_ROUND16X4F32SASYM(Y0, Y1);
            AE_SA16X4_IP(Y,ar,pR);
        }
        AE_SA64POS_FP(ar, pR);
        if (k<N)
        {
            pY=(const ae_int16x4*)x;
            pX0=(const ae_int16x4*)(x+k+0);
            pX1=(const ae_int16x4*)(x+k+1);
            pX2=(const ae_int16x4*)(x+k+2);
            pX3=(const ae_int16x4*)(x+k+3);
            ax1=AE_LA64_PP(pX1);
            ax2=AE_LA64_PP(pX2);
            ax3=AE_LA64_PP(pX3);
            a0=a1=a2=a3=AE_ZERO64();
            AE_L16X4_IP(Y,pY,sizeof(ae_int16x4));
            AE_L16X4_IP (X0,pX0,sizeof(ae_int16x4));
            AE_LA16X4_IP(X1,ax1,pX1);
            AE_LA16X4_IP(X2,ax2,pX2);
            AE_LA16X4_IP(X3,ax3,pX3);
            Y0=AE_CVT32X2F16_32(Y);
            Y1=AE_CVT32X2F16_10(Y);
            AE_MULAAFD32X16_H3_L2(a0,Y0,X0);
            AE_MULAAFD32X16_H3_L2(a1,Y0,X1);
            AE_MULAAFD32X16_H3_L2(a2,Y0,X2);
            AE_MULAAFD32X16_H3_L2(a3,Y0,X3);
            AE_MULAAFD32X16_H1_L0(a0,Y1,X0);
            AE_MULAAFD32X16_H1_L0(a1,Y1,X1);
            AE_MULAAFD32X16_H1_L0(a2,Y1,X2);
            AE_MULAAFD32X16_H1_L0(a3,Y1,X3);
            Y0 = AE_TRUNCA32X2F64S(a3,a2,  16);
            Y1 = AE_TRUNCA32X2F64S(a1,a0,  16);
            Y  = AE_ROUND16X4F32SASYM(Y0, Y1);
            n=N-k;
            AE_S16_0_IP(Y, castxcc(ae_int16, pR), +2);
            if (n > 1) { Y = AE_SEL16_4321(Y,Y); AE_S16_0_IP(Y, castxcc(ae_int16, pR), +2); }
            if (n > 2) { Y = AE_SEL16_4321(Y,Y); AE_S16_0_IP(Y, castxcc(ae_int16, pR), +2); }
        }
    }
#endif

}
#else
void fir_lacorra16x16( void     * restrict s,
                       int16_t  * restrict r,
                 const int16_t  * restrict x, int N)
{
    void     * s_ptr;
    int16_t  * x_buf;

    ae_int16x4       * restrict D;
    ae_int16         * restrict D_s;
    ae_valign S_va;
    ae_int16x4 z_f16x4;

    int n, m;

    const ae_int16x4 *          X0;
    const ae_int16x4 *          X1;
    const ae_int16x4 *          X2;
    const ae_int16x4 *          X3;
    const ae_int16x4 *          S;
    const ae_int16x4 *          S1;
    const ae_int16x4 *          Y;
          ae_int16x4 * restrict R;

    ae_int64   q0, q1, q2, q3;
    ae_int64   q4, q5, q6, q7;
    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 y0;
    ae_f32x2   t0, t1;

    ae_valign  ar;

    NASSERT(s);
    NASSERT(r);
    NASSERT(x);
    NASSERT_ALIGN(s, 8);
    NASSERT(N>0);

    //----------------------------------------------------------------------------
    // Partition the scratch memory area.
    s_ptr = s;
    x_buf = (int16_t*)ALIGNED_ADDR(s_ptr, 8);
    s_ptr = x_buf + N + 6;

    ASSERT((int8_t *)s_ptr - (int8_t *)s <=
        (int)FIR_LACORRA16X16_SCRATCH_SIZE(N));
    //----------------------------------------------------------------------------
    // Copy x[N].
    S = (const ae_int16x4 *)x;
    D = (ae_int16x4 *)x_buf;
    S_va = AE_LA64_PP(S);
    for (n = 0; n<((N + 3) >> 2); n++)
    {
        AE_LA16X4_IP(x0, S_va, S);
        AE_S16X4_IP(x0, D, +8);
    }

    D_s = (ae_int16 *)((int16_t *)x_buf + N);
    z_f16x4 = AE_ZERO16();
    for (m = 0; m<6; m++)
    {
        AE_S16_0_IP(z_f16x4, D_s, +2);
    }
    
    /*
    * Compute r[0]...r[3] entries.
    */
    {
        ae_valign  ax1, ax2, ax3;

        int n_iter = XT_MIN(N, 4);
        int m_iter = ((N + 3)&~3);

        /* compute N&~3 correlation results */
        ar = AE_ZALIGN64();

        X0 = (const ae_int16x4 *)x_buf;
        R = (ae_int16x4 *)(r);

        for (n = 0; n < (n_iter&~3); n += 4){
            X1 = (const ae_int16x4 *)((int16_t *)X0 + 1);
            X2 = (const ae_int16x4 *)((int16_t *)X0 + 2);
            X3 = (const ae_int16x4 *)((int16_t *)X0 + 3);

            ax1 = AE_LA64_PP(X1);
            ax2 = AE_LA64_PP(X2);
            ax3 = AE_LA64_PP(X3);

            AE_L16X4_IP(x0, X0, +8);
            AE_LA16X4_IP(x1, ax1, X1);
            AE_LA16X4_IP(x2, ax2, X2);
            AE_LA16X4_IP(x3, ax3, X3);

            S = X0;

            Y = (const ae_int16x4 *)x_buf;
            AE_L16X4_IP(y0, Y, 8);

            q0 = AE_MULZAAAAQ16(x0, y0);
            q1 = AE_MULZAAAAQ16(x1, y0);
            q2 = AE_MULZAAAAQ16(x2, y0);
            q3 = AE_MULZAAAAQ16(x3, y0);

            for (m = 0; m < (m_iter >> 2) - 1; m++){
                AE_L16X4_IP(x0, S, +8);
                AE_LA16X4_IP(x1, ax1, X1);
                AE_LA16X4_IP(x2, ax2, X2);
                AE_LA16X4_IP(x3, ax3, X3);

                AE_L16X4_IP(y0, Y, 8);
                AE_MULAAAAQ16(q0, x0, y0);
                AE_MULAAAAQ16(q1, x1, y0);
                AE_MULAAAAQ16(q2, x2, y0);
                AE_MULAAAAQ16(q3, x3, y0);
            }

            t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
            AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);
        }
        AE_SA64POS_FP(ar, R);
        /* process last 1...3 samples */
        n_iter &= 3;
        if (n_iter)
        {
            X1 = (const ae_int16x4 *)((int16_t *)X0 + 1);
            X2 = (const ae_int16x4 *)((int16_t *)X0 + 2);

            ax1 = AE_LA64_PP(X1);
            ax2 = AE_LA64_PP(X2);

            AE_L16X4_IP(x0, X0, +8);
            AE_LA16X4_IP(x1, ax1, X1);
            AE_LA16X4_IP(x2, ax2, X2);

            Y = (const ae_int16x4 *)x_buf;
            AE_L16X4_IP(y0, Y, 8);

            q0 = AE_MULZAAAAQ16(x0, y0);
            q1 = AE_MULZAAAAQ16(x1, y0);
            q2 = AE_MULZAAAAQ16(x2, y0);

            for (m = 0; m < (m_iter >> 2) - 1; m++){
                AE_L16X4_IP(x0, X0, +8);
                AE_LA16X4_IP(x1, ax1, X1);
                AE_LA16X4_IP(x2, ax2, X2);

                AE_L16X4_IP(y0, Y, 8);
                AE_MULAAAAQ16(q0, x0, y0);
                AE_MULAAAAQ16(q1, x1, y0);
                AE_MULAAAAQ16(q2, x2, y0);
            }

            t0 = AE_TRUNCA32X2F64S(q1, q0, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q2, 33);
            x0 = AE_ROUND16X4F32SASYM(t1, t0);

            AE_S16_0_IP(x0, castxcc(ae_int16, R), +2);
            if (n_iter > 1) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, R), +2); }
            if (n_iter > 2) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, R), +2); }
        }
    }

    /*
    * Compute r[4]....r[N] entries.
    */
    {
        ae_valign  ax1, ax2, ax3;

        int n_iter = XT_MAX(0, (N - 4));
        int m_iter = ((N + 2 - 3) >> 2) - 1;

        X0 = (const ae_int16x4 *)(x_buf + 4);
        R = (ae_int16x4 *)(r + 4);

        ar = AE_ZALIGN64();

        for (n = 0; n < (n_iter&~7); n += 8, m_iter -= 2){
            X1 = (const ae_int16x4 *)((int16_t *)X0 + 1);
            X2 = (const ae_int16x4 *)((int16_t *)X0 + 2);
            X3 = (const ae_int16x4 *)((int16_t *)X0 + 3);

            ax1 = AE_LA64_PP(X1);
            ax2 = AE_LA64_PP(X2);
            ax3 = AE_LA64_PP(X3);

            AE_L16X4_IP(x0, X0, +8);
            AE_LA16X4_IP(x1, ax1, X1);
            AE_LA16X4_IP(x2, ax2, X2);
            AE_LA16X4_IP(x3, ax3, X3);

            S = X0;

            Y = (const ae_int16x4 *)x_buf;
            AE_L16X4_IP(y0, Y, 8);

            q0 = AE_MULZAAAAQ16(x0, y0);
            q1 = AE_MULZAAAAQ16(x1, y0);
            q2 = AE_MULZAAAAQ16(x2, y0);
            q3 = AE_MULZAAAAQ16(x3, y0);

            AE_L16X4_IP(x0, X0, +8);
            S1 = X0;
            AE_LA16X4_IP(x1, ax1, X1);
            AE_LA16X4_IP(x2, ax2, X2);
            AE_LA16X4_IP(x3, ax3, X3);

            q4 = AE_MULZAAAAQ16(x0, y0);
            q5 = AE_MULZAAAAQ16(x1, y0);
            q6 = AE_MULZAAAAQ16(x2, y0);
            q7 = AE_MULZAAAAQ16(x3, y0);

            for (m = 0; m < m_iter - 1; m++){
                AE_L16X4_IP(y0, Y, 8);
                AE_L16X4_IP(x0, S, +8);

                AE_MULAAAAQ16(q0, x0, y0);
                AE_MULAAAAQ16(q1, x1, y0);
                AE_MULAAAAQ16(q2, x2, y0);
                AE_MULAAAAQ16(q3, x3, y0);

                AE_L16X4_IP(x0, S1, +8);
                AE_LA16X4_IP(x1, ax1, X1);
                AE_LA16X4_IP(x2, ax2, X2);
                AE_LA16X4_IP(x3, ax3, X3);

                AE_MULAAAAQ16(q4, x0, y0);
                AE_MULAAAAQ16(q5, x1, y0);
                AE_MULAAAAQ16(q6, x2, y0);
                AE_MULAAAAQ16(q7, x3, y0);
            }

            AE_L16X4_IP(y0, Y, 8);
            AE_L16X4_IP(x0, S, +8);

            AE_MULAAAAQ16(q0, x0, y0);
            AE_MULAAAAQ16(q1, x1, y0);
            AE_MULAAAAQ16(q2, x2, y0);
            AE_MULAAAAQ16(q3, x3, y0);

            t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
            AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);

            t0 = AE_TRUNCA32X2F64S(q4, q5, 33);
            t1 = AE_TRUNCA32X2F64S(q6, q7, 33);
            AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);
        }
        n_iter &= 7;
        if (n_iter & 4){
            X1 = (const ae_int16x4 *)((int16_t *)X0 + 1);
            X2 = (const ae_int16x4 *)((int16_t *)X0 + 2);
            X3 = (const ae_int16x4 *)((int16_t *)X0 + 3);

            ax1 = AE_LA64_PP(X1);
            ax2 = AE_LA64_PP(X2);
            ax3 = AE_LA64_PP(X3);

            AE_L16X4_IP(x0, X0, +8);
            AE_LA16X4_IP(x1, ax1, X1);
            AE_LA16X4_IP(x2, ax2, X2);
            AE_LA16X4_IP(x3, ax3, X3);

            S = X0;

            Y = (const ae_int16x4 *)x_buf;
            AE_L16X4_IP(y0, Y, 8);

            q0 = AE_MULZAAAAQ16(x0, y0);
            q1 = AE_MULZAAAAQ16(x1, y0);
            q2 = AE_MULZAAAAQ16(x2, y0);
            q3 = AE_MULZAAAAQ16(x3, y0);

            for (m = 0; m < m_iter; m++){
                AE_L16X4_IP(x0, S, +8);
                AE_LA16X4_IP(x1, ax1, X1);
                AE_LA16X4_IP(x2, ax2, X2);
                AE_LA16X4_IP(x3, ax3, X3);

                AE_L16X4_IP(y0, Y, 8);
                AE_MULAAAAQ16(q0, x0, y0);
                AE_MULAAAAQ16(q1, x1, y0);
                AE_MULAAAAQ16(q2, x2, y0);
                AE_MULAAAAQ16(q3, x3, y0);
            }

            t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
            AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, R);

            m_iter -= 1;
        }

        AE_SA64POS_FP(ar, R);
        /* process last 1...3 samples */
        n_iter &= 3;
        if (n_iter)
        {
            X1 = (const ae_int16x4 *)((int16_t *)X0 + 1);
            X2 = (const ae_int16x4 *)((int16_t *)X0 + 2);

            ax1 = AE_LA64_PP(X1);
            ax2 = AE_LA64_PP(X2);

            AE_L16X4_IP(x0, X0, +8);
            AE_LA16X4_IP(x1, ax1, X1);
            AE_LA16X4_IP(x2, ax2, X2);

            Y = (const ae_int16x4 *)x_buf;
            AE_L16X4_IP(y0, Y, 8);

            q0 = AE_MULZAAAAQ16(x0, y0);
            q1 = AE_MULZAAAAQ16(x1, y0);
            q2 = AE_MULZAAAAQ16(x2, y0);

            t0 = AE_TRUNCA32X2F64S(q1, q0, 33);
            t1 = AE_TRUNCA32X2F64S(q2, q2, 33);
            x0 = AE_ROUND16X4F32SASYM(t1, t0);

            AE_S16_0_IP(x0, castxcc(ae_int16, R), +2);
            if (n_iter > 1) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, R), +2); }
            if (n_iter > 2) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, R), +2); }
        }
    }

} /* fir_lacorra16x16() */
#endif

