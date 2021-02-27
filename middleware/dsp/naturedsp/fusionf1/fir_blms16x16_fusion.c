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
    Blockwise Adaptive LMS Algorithm for Real Data, 16x32-bit  
*/

/*-------------------------------------------------------------------------
  Blockwise Adaptive LMS Algorithm for Real Data
  Blockwise LMS algorithm performs filtering of input samples x[N+M-1],
  computation of error e[N] over a block of reference data r[N] and makes
  blockwise update of IR to minimize the error output.
  Algorithm includes FIR filtering, calculation of correlation between the 
  error output e[N] and reference signal x[N+M-1] and IR taps update based
  on that correlation.
NOTES: 
  1. The algorithm must be provided with the normalization factor, which is
     the power of the input signal times N - the number of samples in a
     data block. This can be calculated using the vec_power24x24() or 
     vec_power16x16() function. In order to avoid the saturation of the 
     normalization factor, it may be biased, i.e. shifted to the right.
     If it's the case, then the adaptation coefficient must be also shifted
     to the right by the same number of bit positions.
  2. this algorithm consumes less CPU cycles per block than single 
     sample algorithm at similar convergence rate.
  3. Right selection of N depends on the change rate of impulse response:
     on static or slow varying channels convergence rate depends on
     selected mu and M, but not on N.
  4. 16x16 routine may converge slower on small errors due to roundoff 
     errors. In that cases, 16x32 rountine will give better results although 
     convergence rate on bigger errors is the same

  Precision: 
  16x16   16-bit coefficients, 16-bit data, 16-bit output
  24x24   24-bit coefficients, 24-bit data, 32-bit output
  16x32   32-bit coefficients, 16-bit data, 16-bit output
  32x32   32-bit coefficients, 32-bit data, 32-bit output
  f       floating point
  Input:
  h[M]     impulse response, Q15, Q31 or floating point
  r[N]     reference data vector (near end). First in time value is in 
           r[0], 24-bit or 16-bit, Qx or floating point
  x[N+M-1] input vector (far end). First in time value is in x[0],  
           24-bit or 16-bit, Qx or floating point
  norm     normalization factor: power of signal multiplied by N, Q15, 
           Q31 or floating point
           Fixed-point format for the 24x24-bit variant: Q(2*x-31-bias)
           Fixed-point format for the 32x16-bit variant: Q(2*x+1-bias)
  mu       adaptation coefficient (LMS step), Q(31-bias) or Q(15-bias)
  N        length of data block
  M        length of h
  Output:
  e[N]     estimated error, Q31,Q15 or floating point
  h[M]     updated impulse response, Q15, Q31 or floating point

  Restriction:
  x,r,h,e - should not overlap
  x,r,h,e - aligned on a 8-bytes boundary
  N,M     - multiples of 8 and >0
-------------------------------------------------------------------------*/
#include "NatureDSP_types.h"
#include "NatureDSP_Signal.h"
#include "common.h"

void fir_blms16x16( int16_t * restrict e,
                    int16_t * restrict h,
              const int16_t * restrict r,
              const int16_t * restrict x,
              int16_t norm, int16_t mu,
              int N, int M )
#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
{
    /* no Quad MAC version */
    ae_int32x2 s_frac;
    int     s_exp, mu_exp;
    const ae_int16x4 * restrict pX0;
    const ae_int16x4 * restrict pX1;
    const ae_int16x4 * restrict pX2;
    const ae_int16x4 * restrict pX3;
    const ae_int16x4 * restrict pY;
          ae_int16x4 * restrict pE;
    const ae_int16x4 * restrict pR;
          ae_int16x4 * restrict pH;

    int m, n;

    NASSERT(e);
    NASSERT(h);
    NASSERT(r);
    NASSERT(x);
    NASSERT_ALIGN(e,8);
    NASSERT_ALIGN(h,8);
    NASSERT_ALIGN(r,8);
    NASSERT_ALIGN(x,8);
    NASSERT(N>0 && M>0);
    NASSERT(M%8==0 && N%8==0);

    //
    // Pass the reference signal x[] through the adaptive filter to obtain the
    // predicted signal and calculate the error, i.e. the distance to the 
    // actual input signal r[].
    //
#if 0
    for ( n=0; n<N; n+=4 )
    {
        int64_t acc[4];
        acc[0] = acc[1]= acc[2]= acc[3]=0;
        for (m=0; m<M; m+=4 )
        {
            acc[0] += mpy_Q31Q15( h[M-1-m]<<16, x[n+m+0]) + mpy_Q31Q15( h[M-2-m]<<16, x[n+m+1] );
            acc[1] += mpy_Q31Q15( h[M-1-m]<<16, x[n+m+1]) + mpy_Q31Q15( h[M-2-m]<<16, x[n+m+2] );
            acc[2] += mpy_Q31Q15( h[M-1-m]<<16, x[n+m+2]) + mpy_Q31Q15( h[M-2-m]<<16, x[n+m+3] );
            acc[3] += mpy_Q31Q15( h[M-1-m]<<16, x[n+m+3]) + mpy_Q31Q15( h[M-2-m]<<16, x[n+m+4] );

            acc[0] += mpy_Q31Q15( h[M-3-m]<<16, x[n+m+2]) + mpy_Q31Q15( h[M-4-m]<<16, x[n+m+3] );
            acc[1] += mpy_Q31Q15( h[M-3-m]<<16, x[n+m+3]) + mpy_Q31Q15( h[M-4-m]<<16, x[n+m+4] );
            acc[2] += mpy_Q31Q15( h[M-3-m]<<16, x[n+m+4]) + mpy_Q31Q15( h[M-4-m]<<16, x[n+m+5] );
            acc[3] += mpy_Q31Q15( h[M-3-m]<<16, x[n+m+5]) + mpy_Q31Q15( h[M-4-m]<<16, x[n+m+6] );
        }
        e[n+0] = S_sub_ss(r[n+0], S_round_l(satQ31(acc[0]>>16)));
        e[n+1] = S_sub_ss(r[n+1], S_round_l(satQ31(acc[1]>>16)));
        e[n+2] = S_sub_ss(r[n+2], S_round_l(satQ31(acc[2]>>16)));
        e[n+3] = S_sub_ss(r[n+3], S_round_l(satQ31(acc[3]>>16)));
    }
#else
    {
        ae_f64 a0,a1,a2,a3;
        ae_int16x4 Y,X0,X1,X2,X3,R;
        ae_int32x2 Y0,Y1;
        ae_valign ax1,ax2,ax3;
        pR=(const ae_int16x4 *)r;
        pE=(      ae_int16x4 *)e;
        for ( n=0; n<N; n+=4 )
        {
            a0=a1=a2=a3=AE_ZERO64();
            pY=(const ae_int16x4 *)&h[M-4];
            pX0=(const ae_int16x4 *)(x+n+0);
            pX1=(const ae_int16x4 *)(x+n+1);
            pX2=(const ae_int16x4 *)(x+n+2);
            pX3=(const ae_int16x4 *)(x+n+3);
            ax1=AE_LA64_PP(pX1);
            ax2=AE_LA64_PP(pX2);
            ax3=AE_LA64_PP(pX3);
            __Pragma("loop_count min=2")
            for (m=0; m<(M>>2); m++)
            {
                AE_L16X4_XP(Y,pY,-(int)sizeof(ae_int16x4));
                AE_L16X4_IP(X0,pX0,sizeof(ae_int16x4));
                AE_LA16X4_IP(X1,ax1,pX1);
                AE_LA16X4_IP(X2,ax2,pX2);
                AE_LA16X4_IP(X3,ax3,pX3);
                Y0=AE_CVT32X2F16_10(Y);
                Y1=AE_CVT32X2F16_32(Y);
                AE_MULAAFD32X16_H2_L3(a0,Y0,X0);
                AE_MULAAFD32X16_H2_L3(a1,Y0,X1);
                AE_MULAAFD32X16_H2_L3(a2,Y0,X2);
                AE_MULAAFD32X16_H2_L3(a3,Y0,X3);
                AE_MULAAFD32X16_H0_L1(a0,Y1,X0);
                AE_MULAAFD32X16_H0_L1(a1,Y1,X1);
                AE_MULAAFD32X16_H0_L1(a2,Y1,X2);
                AE_MULAAFD32X16_H0_L1(a3,Y1,X3);
            }
            AE_L16X4_IP(R,pR,sizeof(ae_int16x4));
            Y0 = AE_TRUNCA32X2F64S(a0,a1,  16);
            Y1 = AE_TRUNCA32X2F64S(a2,a3,  16);
            Y  = AE_ROUND16X4F32SASYM(Y0, Y1);
            R  = AE_SUB16S(R,Y);
            AE_S16X4_IP(R,pE,sizeof(ae_int16x4));
        }
    }
#endif

    //
    // Compute the reciprocal for the normalization factor.
    //
    {
        ae_f64 q0;
        ae_int32x2  norm_v;
        ae_int16x4  mu_v;

        /* Compute the reciprocal for the normalization factor */
        s_exp = AE_NSAZ32_L(((int32_t)norm)<<16);
        norm_v = AE_MOVDA32(((int32_t)norm) << 16);
        norm_v = AE_SLAA32(norm_v, s_exp);

        {
            ae_int32x2 y, e, x, _1Q30;
            x = norm_v;
            _1Q30 = AE_MOVDA32((int)0x40000000);
            y = AE_SUB32(_1Q30, x);/* no saturation here!!! */
            y = AE_ADD32(_1Q30, y);
            y = AE_INT32X2_ADD32S(_1Q30, y);

            q0 = AE_MULF32S_HH(x, y);
            e = AE_SUB32(_1Q30, AE_ROUND32X2F64SASYM(q0, q0));
            q0 = AE_MULF32S_HH(e, y);  e = AE_ROUND32X2F64SASYM(q0, q0);
            y = AE_ADD32S(y, AE_ADD32(e, e));

            q0 = AE_MULF32S_HH(x, y);
            e = AE_SUB32(_1Q30, AE_ROUND32X2F64SASYM(q0, q0));
            q0 = AE_MULF32S_HH(e, y);  e = AE_ROUND32X2F64SASYM(q0, q0);
            y = AE_ADD32S(y, AE_ADD32(e, e));

            q0 = AE_MULF32S_HH(x, y);
            e = AE_SUB32(_1Q30, AE_ROUND32X2F64SASYM(q0, q0));
            q0 = AE_MULF32S_HH(e, y);  e = AE_ROUND32X2F64SASYM(q0, q0);
            y = AE_ADD32S(y, AE_ADD32(e, e));

            q0 = AE_MULF32S_HH(x, y);
            e = AE_SUB32(_1Q30, AE_ROUND32X2F64SASYM(q0, q0));
            q0 = AE_MULF32S_HH(e, y);  e = AE_ROUND32X2F64SASYM(q0, q0);
            y = AE_ADD32S(y, AE_ADD32(e, e));
            norm_v = y;
        }

        mu_exp = AE_NSAZ16_0(mu);
        mu_v = AE_MOVDA16(mu);
        mu_v = AE_SLAA16S(mu_v, mu_exp);

        q0 = AE_MUL32X16_H0(norm_v, mu_v);
        q0 = AE_SLAI64(q0, 17);

        s_frac = AE_TRUNCI32X2F64S(q0, q0, 0);
        s_exp -= mu_exp; // -15..30

    }
    //
    // Calculate the cross-correlation between the error signal and the 
    // reference signal. Scale the result and update the estimation of the
    // impulse response.
    //
#if 0
    for ( m=0; m<M; m+=4 )
    {
        int64_t acc[4];
        int16_t t0[4],t1[4];
        acc[0]=acc[1]=acc[2]=acc[3]=0;
        for ( n=0; n<N; n+=4 )
        {
            acc[0] += mpy_Q31Q15( e[n]<<16, x[n+m+0] ) + mpy_Q31Q15( e[n+1]<<16, x[n+m+1] );
            acc[1] += mpy_Q31Q15( e[n]<<16, x[n+m+1] ) + mpy_Q31Q15( e[n+1]<<16, x[n+m+2] );
            acc[2] += mpy_Q31Q15( e[n]<<16, x[n+m+2] ) + mpy_Q31Q15( e[n+1]<<16, x[n+m+3] );
            acc[3] += mpy_Q31Q15( e[n]<<16, x[n+m+3] ) + mpy_Q31Q15( e[n+1]<<16, x[n+m+4] );

            acc[0] += mpy_Q31Q15( e[n+2]<<16, x[n+m+2] ) + mpy_Q31Q15( e[n+3]<<16, x[n+m+3] );
            acc[1] += mpy_Q31Q15( e[n+2]<<16, x[n+m+3] ) + mpy_Q31Q15( e[n+3]<<16, x[n+m+4] );
            acc[2] += mpy_Q31Q15( e[n+2]<<16, x[n+m+4] ) + mpy_Q31Q15( e[n+3]<<16, x[n+m+5] );
            acc[3] += mpy_Q31Q15( e[n+2]<<16, x[n+m+5] ) + mpy_Q31Q15( e[n+3]<<16, x[n+m+6] );
        }
        t0[0] = S_round_l( satQ31(acc[0]>>16)); 
        t0[1] = S_round_l( satQ31(acc[1]>>16)); 
        t0[2] = S_round_l( satQ31(acc[2]>>16)); 
        t0[3] = S_round_l( satQ31(acc[3]>>16)); 
        acc[0] = mpy_Q31Q15(  s_frac,t0[0] ); 
        acc[1] = mpy_Q31Q15(  s_frac,t0[1] ); 
        acc[2] = mpy_Q31Q15(  s_frac,t0[2] ); 
        acc[3] = mpy_Q31Q15(  s_frac,t0[3] ); 
        ASSERT( s_exp >= -15 && s_exp <= 30 );
        // Q15 <- Q(48-s_exp) + s_exp - 31 w/ rounding and saturation
        t1[0] = S_sature_l(satQ31( ( acc[0] + ( 1LL << (30-s_exp) ) ) >> (31-s_exp) ));
        t1[1] = S_sature_l(satQ31( ( acc[1] + ( 1LL << (30-s_exp) ) ) >> (31-s_exp) ));
        t1[2] = S_sature_l(satQ31( ( acc[2] + ( 1LL << (30-s_exp) ) ) >> (31-s_exp) ));
        t1[3] = S_sature_l(satQ31( ( acc[3] + ( 1LL << (30-s_exp) ) ) >> (31-s_exp) ));
        // Q15 <- Q15 + Q15 w/ saturation
        h[M-1-m-0] = S_add_ss( h[M-1-m-0], t1[0] );
        h[M-1-m-1] = S_add_ss( h[M-1-m-1], t1[1] );
        h[M-1-m-2] = S_add_ss( h[M-1-m-2], t1[2] );
        h[M-1-m-3] = S_add_ss( h[M-1-m-3], t1[3] );
    }
#else
    {
        ae_f64 a0,a1,a2,a3;
        ae_int16x4 Y,X0,X1,X2,X3,H;
        ae_int32x2 Y0,Y1;
        ae_valign ax1,ax2,ax3;
        pH=(ae_int16x4 *)(h+M-4);
        for ( m=0; m<M; m+=4 )
        {
            a0=a1=a2=a3=AE_ZERO64();
            pY=(const ae_int16x4 *)e;
            pX0=(const ae_int16x4 *)(x+m+0);
            pX1=(const ae_int16x4 *)(x+m+1);
            pX2=(const ae_int16x4 *)(x+m+2);
            pX3=(const ae_int16x4 *)(x+m+3);
            ax1=AE_LA64_PP(pX1);
            ax2=AE_LA64_PP(pX2);
            ax3=AE_LA64_PP(pX3);
            __Pragma("loop_count min=2")
            for ( n=0; n<(N>>2); n++ )
            {
                AE_L16X4_IP(Y,pY,(int)sizeof(ae_int16x4));
                AE_L16X4_IP(X0,pX0,sizeof(ae_int16x4));
                AE_LA16X4_IP(X1,ax1,pX1);
                AE_LA16X4_IP(X2,ax2,pX2);
                AE_LA16X4_IP(X3,ax3,pX3);
                Y1=AE_CVT32X2F16_10(Y);
                Y0=AE_CVT32X2F16_32(Y);
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
                ASSERT( s_exp >= -15 && s_exp <= 30 );
                // Q15 <- Q(48-s_exp) + s_exp - 31 w/ rounding and saturation
            a0 = AE_MUL32X16_H0((s_frac), Y);
            a1 = AE_MUL32X16_H1((s_frac), Y);
            a2 = AE_MUL32X16_H2((s_frac), Y);
            a3 = AE_MUL32X16_H3((s_frac), Y);
            Y0 = AE_TRUNCA32X2F64S(a0, a1, 2 + s_exp);
            Y1 = AE_TRUNCA32X2F64S(a2, a3, 2 + s_exp);
            Y = AE_SAT16X4(Y0, Y1);
            H = AE_L16X4_I(pH,0);
            H=AE_ADD16S(H,Y);
                // Q15 <- Q15 + Q15 w/ saturation
            AE_S16X4_XP(H,pH,-(int)sizeof(ae_int16x4));
        }
    }
#endif

} // fir_blms16x16()

#else
{
    /* Quad MAC version */
    const ae_int16x4 * X0;
    const ae_int16x4 * X0_1;
    const ae_int16x4 * X1;
    const ae_int16x4 * X2;
    const ae_int16x4 * X3;
    const ae_int16x4 * R;
          ae_int16x4 * E;
          ae_int16x4 * restrict H;

    ae_valign va1, va2, va3;
    ae_valign h_va;

    ae_int64   q0, q1, q2, q3;
    ae_int64   q4, q5, q6, q7;
    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 h0, h1;
    ae_int16x4 r0, r1;
    ae_int16x4 e0;

    ae_int64   z;

    ae_int32x2 s_frac;
    int     s_exp, mu_exp;

    int m, n;

    NASSERT(e);
    NASSERT(h);
    NASSERT(r);
    NASSERT(x);
    NASSERT_ALIGN(e,8);
    NASSERT_ALIGN(h,8);
    NASSERT_ALIGN(r,8);
    NASSERT_ALIGN(x,8);
    NASSERT(N>0 && M>0);
    NASSERT(M%8==0 && N%8==0);

    //
    // Pass the reference signal x[] through the adaptive filter to obtain the
    // predicted signal and calculate the error, i.e. the distance to the 
    // actual input signal r[].
    //
    R = (const ae_int16x4 *)r;
    E = (ae_int16x4 *)e;

    z = AE_CVTQ56A32S(0);

    for (n = 0; n < (N >> 3); n++)
    {
        ae_f32x2 t0, t1;

        H = (ae_int16x4 *)(h + M - 4);

        X0 = (const ae_int16x4 *)(x + 8 * n);
        X0_1 = X0 + 1;
        X1 = (ae_int16x4*)((int16_t*)X0 + 1);
        X2 = (ae_int16x4*)((int16_t*)X0 + 2);
        X3 = (ae_int16x4*)((int16_t*)X0 + 3);

        va1 = AE_LA64_PP(X1);
        va2 = AE_LA64_PP(X2);
        va3 = AE_LA64_PP(X3);

        // Zero the accumulators.
        q0 = z; q1 = z; q2 = z; q3 = z;
        q4 = z; q5 = z; q6 = z; q7 = z;

        AE_LA16X4_IP(x1, va1, X1);
        AE_LA16X4_IP(x2, va2, X2);
        AE_LA16X4_IP(x3, va3, X3);

        for (m = 0; m < (M >> 2); m++)
        {
            AE_L16X4_RIP(h0, H);
            AE_L16X4_IP(x0, X0, 8);

            AE_MULAAAAQ16(q0, x0, h0);
            AE_MULAAAAQ16(q1, x1, h0);
            AE_MULAAAAQ16(q2, x2, h0);
            AE_MULAAAAQ16(q3, x3, h0);

            AE_L16X4_IP (x0, X0_1, 8);
            AE_LA16X4_IP(x1, va1, X1);
            AE_LA16X4_IP(x2, va2, X2);
            AE_LA16X4_IP(x3, va3, X3);

            AE_MULAAAAQ16(q4, x0, h0);
            AE_MULAAAAQ16(q5, x1, h0);
            AE_MULAAAAQ16(q6, x2, h0);
            AE_MULAAAAQ16(q7, x3, h0);
        }

        t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
        x0 = AE_ROUND16X4F32SASYM(t0, t1);

        t0 = AE_TRUNCA32X2F64S(q4, q5, 33);
        t1 = AE_TRUNCA32X2F64S(q6, q7, 33);
        x1 = AE_ROUND16X4F32SASYM(t0, t1);

        AE_L16X4_IP(r0, R, +8);
        AE_L16X4_IP(r1, R, +8);

        x0 = AE_SUB16S(r0, x0);
        x1 = AE_SUB16S(r1, x1);

        AE_S16X4_IP(x0, E, +8);
        AE_S16X4_IP(x1, E, +8);
    }

    //
    // Compute the reciprocal for the normalization factor.
    //
    {
        ae_int32x2  norm_v;
        ae_int16x4  mu_v;

        /* Compute the reciprocal for the normalization factor */
        s_exp = AE_NSAZ32_L(((int32_t)norm)<<16);
        norm_v = AE_MOVDA32(((int32_t)norm) << 16);
        norm_v = AE_SLAA32(norm_v, s_exp);

        {
            ae_int32x2 y, e, x, _1Q30;
            x = norm_v;
            _1Q30 = AE_MOVDA32((int)0x40000000);
            y = AE_SUB32(_1Q30, x);/* no saturation here!!! */
            y = AE_ADD32(_1Q30, y);
            y = AE_INT32X2_ADD32S(_1Q30, y);

            q0 = AE_MULF32S_HH(x, y);
            e = AE_SUB32(_1Q30, AE_ROUND32X2F64SASYM(q0, q0));
            q0 = AE_MULF32S_HH(e, y);  e = AE_ROUND32X2F64SASYM(q0, q0);
            y = AE_ADD32S(y, AE_ADD32(e, e));

            q0 = AE_MULF32S_HH(x, y);
            e = AE_SUB32(_1Q30, AE_ROUND32X2F64SASYM(q0, q0));
            q0 = AE_MULF32S_HH(e, y);  e = AE_ROUND32X2F64SASYM(q0, q0);
            y = AE_ADD32S(y, AE_ADD32(e, e));

            q0 = AE_MULF32S_HH(x, y);
            e = AE_SUB32(_1Q30, AE_ROUND32X2F64SASYM(q0, q0));
            q0 = AE_MULF32S_HH(e, y);  e = AE_ROUND32X2F64SASYM(q0, q0);
            y = AE_ADD32S(y, AE_ADD32(e, e));

            q0 = AE_MULF32S_HH(x, y);
            e = AE_SUB32(_1Q30, AE_ROUND32X2F64SASYM(q0, q0));
            q0 = AE_MULF32S_HH(e, y);  e = AE_ROUND32X2F64SASYM(q0, q0);
            y = AE_ADD32S(y, AE_ADD32(e, e));
            norm_v = y;
        }

        mu_exp = AE_NSAZ16_0(mu);
        mu_v = AE_MOVDA16(mu);
        mu_v = AE_SLAA16S(mu_v, mu_exp);

        q0 = AE_MUL32X16_H0(norm_v, mu_v);
        q0 = AE_SLAI64(q0, 17);

        s_frac = AE_TRUNCI32X2F64S(q0, q0, 0);
        s_exp -= mu_exp; // -15..30

    }

    //
    // Calculate the cross-correlation between the error signal and the 
    // reference signal. Scale the result and update the estimation of the
    // impulse response.
    //
    z = AE_CVTQ56A32S(0);
    H = (ae_int16x4 *)(h + M - 1);

    for (m = 0; m < (M >> 3); m++)
    {
        ae_f32x2 t0, t1;

        E = (ae_int16x4 *)(e);

        X0 = (const ae_int16x4 *)(x + 8 * m);
        X0_1 = X0 + 1;
        X1 = (ae_int16x4*)((int16_t*)X0 + 1);
        X2 = (ae_int16x4*)((int16_t*)X0 + 2);
        X3 = (ae_int16x4*)((int16_t*)X0 + 3);

        va1 = AE_LA64_PP(X1);
        va2 = AE_LA64_PP(X2);
        va3 = AE_LA64_PP(X3);

        // Zero the accumulators.
        q0 = z; q1 = z; q2 = z; q3 = z;
        q4 = z; q5 = z; q6 = z; q7 = z;

        AE_LA16X4_IP(x1, va1, X1);
        AE_LA16X4_IP(x2, va2, X2);
        AE_LA16X4_IP(x3, va3, X3);

        for (n = 0; n < (N >> 2); n++)
        {
            AE_L16X4_IP(e0, E,  8);
            AE_L16X4_IP(x0, X0, 8);

            AE_MULAAAAQ16(q0, x0, e0);
            AE_MULAAAAQ16(q1, x1, e0);
            AE_MULAAAAQ16(q2, x2, e0);
            AE_MULAAAAQ16(q3, x3, e0);

            AE_L16X4_IP(x0, X0_1, 8);
            AE_LA16X4_IP(x1, va1, X1);
            AE_LA16X4_IP(x2, va2, X2);
            AE_LA16X4_IP(x3, va3, X3);

            AE_MULAAAAQ16(q4, x0, e0);
            AE_MULAAAAQ16(q5, x1, e0);
            AE_MULAAAAQ16(q6, x2, e0);
            AE_MULAAAAQ16(q7, x3, e0);
        }

        t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
        x0 = AE_ROUND16X4F32SASYM(t0, t1);

        t0 = AE_TRUNCA32X2F64S(q4, q5, 33);
        t1 = AE_TRUNCA32X2F64S(q6, q7, 33);
        x1 = AE_ROUND16X4F32SASYM(t0, t1);

        q0 = AE_MUL32X16_H3(s_frac, x0);
        q1 = AE_MUL32X16_H2(s_frac, x0);
        q2 = AE_MUL32X16_H1(s_frac, x0);
        q3 = AE_MUL32X16_H0(s_frac, x0);

        q4 = AE_MUL32X16_H3(s_frac, x1);
        q5 = AE_MUL32X16_H2(s_frac, x1);
        q6 = AE_MUL32X16_H1(s_frac, x1);
        q7 = AE_MUL32X16_H0(s_frac, x1);

        ASSERT(s_exp >= -15 && s_exp <= 30);

        t0 = AE_TRUNCA32X2F64S(q0, q1, 2 + s_exp);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 2 + s_exp);
        x0 = AE_SAT16X4(t0, t1);

        t0 = AE_TRUNCA32X2F64S(q4, q5, 2 + s_exp);
        t1 = AE_TRUNCA32X2F64S(q6, q7, 2 + s_exp);
        x1 = AE_SAT16X4(t0, t1);
        
        {
            const ae_int16x4 * H_r = H;

            h_va = AE_LA64_PP(H_r);

            AE_LA16X4_RIP(h0, h_va, H_r);
            AE_LA16X4_RIP(h1, h_va, H_r);

            x0 = AE_ADD16S(x0, h0);
            x1 = AE_ADD16S(x1, h1);

            h_va = AE_ZALIGN64();

            AE_SA16X4_RIP(x0, h_va, H);
            AE_SA16X4_RIP(x1, h_va, H);
            AE_SA64POS_FP(h_va, H);
        }
    }

} // fir_blms16x16()
#endif
