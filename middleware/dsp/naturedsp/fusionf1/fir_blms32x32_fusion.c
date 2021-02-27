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
    Blockwise Adaptive LMS Algorithm for Real Data, 32x32-bit
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

void fir_blms32x32(int32_t* restrict e, int32_t* restrict h,
             const int32_t* restrict r,
             const int32_t* restrict x,
                   int32_t norm,
                   int32_t mu,
                   int   N,
                   int   M)
{
    const ae_f32x2 * X;
    const ae_f32x2 * S;    
    const ae_f32x2 * R;
          ae_f32x2 * E;
          ae_f32x2 * restrict H;

    ae_f64   q0, q1, q2, q3;
    ae_f32x2 x0, x1, x2;
    ae_f32x2 h0, h1;
    ae_f32x2 r0, r1;
    ae_f32x2 e0;
    
    ae_int32x2 s_frac;
    int       s_exp, mu_exp;

    int n, m;

    NASSERT(e);
    NASSERT(h);
    NASSERT(r);
    NASSERT(x);
    NASSERT_ALIGN(e, 8);
    NASSERT_ALIGN(h, 8);
    NASSERT_ALIGN(r, 8);
    NASSERT_ALIGN(x, 8);
    NASSERT(N>0 && M>0);
    NASSERT(M % 8 == 0 && N % 8 == 0);

    //
    // Pass the reference signal x[] through the adaptive filter to obtain the
    // predicted signal and calculate the error, i.e. the distance to the 
    // actual input signal r[].
    //
    X = (const ae_f32x2 *)x;
    R = (const ae_f32x2 *)r;
    E = (ae_f32x2 *)e;

    for (n = 0; n < (N >> 2); n++)
    {
        ae_int32x2 t0, t1, t2;

        AE_L32X2_IP(t0, X, +8); x0 = (t0);
        AE_L32X2_IP(t1, X, +8); x1 = (t1);
        S = X;
        AE_L32X2_IP(t2, S, +8); x2 = (t2);

        H = (ae_f32x2 *)(h + M - 2);

        AE_L32X2_RIP(t0, H); h0 = (t0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q0 = AE_MULF32R_HH(x0, h0);
        AE_MULAF32R_LL(q0, x0, h0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q1 = AE_MULF32R_LH(x0, h0);
        AE_MULAF32R_LH(q1, h0, x1);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q2 = AE_MULF32R_HH(x1, h0);
        AE_MULAF32R_LL(q2, x1, h0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q3 = AE_MULF32R_LH(x1, h0);
        AE_MULAF32R_LH(q3, h0, x2);

        x0 = x1; x1 = x2;

        for (m = 0; m < (M >> 1) - 1; m++)
        {
            AE_L32X2_IP(t2, S, +8); x2 = (t2);
            AE_L32X2_RIP(t0, H); h0 = (t0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_HH(q0, x0, h0);
            AE_MULAF32R_LL(q0, x0, h0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_LH(q1, x0, h0);
            AE_MULAF32R_LH(q1, h0, x1);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_HH(q2, x1, h0);
            AE_MULAF32R_LL(q2, x1, h0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_LH(q3, x1, h0);
            AE_MULAF32R_LH(q3, h0, x2);

            x0 = x1; x1 = x2;
        }

        x0 = AE_ROUND32X2F48SASYM(q0, q1);
        x1 = AE_ROUND32X2F48SASYM(q2, q3);

        AE_L32X2_IP(t0, R, +8); r0 = (t0);
        AE_L32X2_IP(t1, R, +8); r1 = (t1);

        x0 = AE_SUB32S(r0, x0);
        x1 = AE_SUB32S(r1, x1);

        AE_S32X2_IP(x0, E, +8);
        AE_S32X2_IP(x1, E, +8);
    }


    //
    // Compute the reciprocal for the normalization factor.
    //
    {
        ae_int32x2  vxw, vrw;

        /* Compute the reciprocal for the normalization factor */
        s_exp = AE_NSAZ32_L((int32_t)norm);
        vxw = AE_MOVDA32((int32_t)norm);
        vxw = AE_SLAA32(vxw, s_exp);

        /* reciprocal of Q31 in Q30: 6 LSB accuracy */
        {            
            ae_int32x2 y, e, x, _1Q30;
            x = vxw;
            _1Q30 = AE_MOVDA32((int)0x40000000);
            y = AE_SUB32(_1Q30, x);/* no saturation here!!! */
            y = AE_ADD32(_1Q30, y);
            y = AE_INT32X2_ADD32S(_1Q30, y);

            q0 = AE_MULF32S_HH(x, y);
            e = AE_SUB32(_1Q30, AE_ROUND32X2F64SASYM(q0, q0));
            q0 = AE_MULF32S_HH(e, y);  e = AE_ROUND32X2F64SASYM(q0, q0);
            y = AE_ADD32S(y, AE_ADD32(e,e));

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
            vrw = y;
        }

        {
            ae_int32x2  vxa;
            int x_nsa;

            vxa = AE_SEL32_HH(vrw, vrw);
            x_nsa = AE_NSAZ32_L(vxa);
            vxa = AE_SLAA32(vxa, x_nsa);

            mu_exp = AE_NSAZ32_L((int32_t)mu);
            vxw = (int32_t)mu;
            vxw = AE_SLAA32S(vxw, mu_exp);

            q0 = AE_MULF32S_HH(vrw, vxw);  s_frac = AE_ROUND32X2F64SASYM(q0, q0);
            s_exp -= mu_exp; // -31..30
        }
    }

    //
    // Calculate the cross-correlation between the error signal and the 
    // reference signal. Scale the result and update the estimation of the
    // impulse response.
    //
    {
        X = (const ae_f32x2 *)x;
        H = (ae_f32x2 *)(h + M - 2);

        for (n = 0; n < (M >> 2); n++)
        {
            ae_int32x2 t0, t1, t2;

            AE_L32X2_IP(t0, X, +8); x0 = (t0);
            AE_L32X2_IP(t1, X, +8); x1 = (t1);
            S = X;
            AE_L32X2_IP(t2, S, +8); x2 = (t2);

            E = (ae_f32x2 *)(e);

            AE_L32X2_IP(t0, E, +8); e0 = (t0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            q0 = AE_MULF32R_HH(x0, e0);
            AE_MULAF32R_LL(q0, x0, e0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            q1 = AE_MULF32R_LH(x0, e0);
            AE_MULAF32R_LH(q1, e0, x1);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            q2 = AE_MULF32R_HH(x1, e0);
            AE_MULAF32R_LL(q2, x1, e0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            q3 = AE_MULF32R_LH(x1, e0);
            AE_MULAF32R_LH(q3, e0, x2);

            x0 = x1; x1 = x2;

            for (m = 0; m < (N >> 1) - 1; m++)
            {
                AE_L32X2_IP(t2, S, +8); x2 = (t2);
                AE_L32X2_IP(t0, E, +8); e0 = (t0);

                // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
                AE_MULAF32R_HH(q0, x0, e0);
                AE_MULAF32R_LL(q0, x0, e0);

                // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
                AE_MULAF32R_LH(q1, x0, e0);
                AE_MULAF32R_LH(q1, e0, x1);

                // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
                AE_MULAF32R_HH(q2, x1, e0);
                AE_MULAF32R_LL(q2, x1, e0);

                // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
                AE_MULAF32R_LH(q3, x1, e0);
                AE_MULAF32R_LH(q3, e0, x2);

                x0 = x1; x1 = x2;
            }

            x0 = AE_ROUND32X2F48SASYM(q0, q1);
            x1 = AE_ROUND32X2F48SASYM(q2, q3);

            q0 = AE_MULF32S_HH(s_frac, x0);
            q1 = AE_MULF32S_LL(s_frac, x0);
            q2 = AE_MULF32S_HH(s_frac, x1);
            q3 = AE_MULF32S_LL(s_frac, x1);

            ASSERT(s_exp >= -31 && s_exp < 31);

            x0 = AE_TRUNCA32X2F64S(q0, q1, 1 + s_exp);
            x1 = AE_TRUNCA32X2F64S(q2, q3, 1 + s_exp);
            
            {
                const ae_f32x2 * H_r = H;

                AE_L32X2_RIP(t0, H_r); h0 = t0;
                AE_L32X2_RIP(t1, H_r); h1 = t1;
                // Q31 <- Q31 + Q31 w/ saturation
                x0 = AE_ADD32S(x0, h0);
                x1 = AE_ADD32S(x1, h1);

                AE_S32X2_RIP(x0, H);
                AE_S32X2_RIP(x1, H);
            }
        }
    }

} // fir_blms32x32()
