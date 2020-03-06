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
    Blockwise Adaptive LMS Algorithm for Real Data, 32x32-bit
    C code optimized for HiFi4
	Integrit, 2006-2018
*/




#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
#include "common.h"
#include "vec_recip_table.h"

/*-------------------------------------------------------------------------
  Blockwise Adaptive LMS Algorithm for Real Data
  Blockwise LMS algorithm performs filtering of reference samples x[N+M-1],
  computation of error e[N] over a block of input samples r[N] and makes
  blockwise update of IR to minimize the error output.
  Algorithm includes FIR filtering, calculation of correlation between the 
  error output e[N] and reference signal x[N+M-1] and IR taps update based
  on that correlation.
NOTES: 
  1. The algorithm must be provided with the normalization factor, which is
     the power of the reference signal times N - the number of samples in a
     data block. This can be calculated using the vec_power32x32() or 
     vec_power16x16() function. In order to avoid the saturation of the 
     normalization factor, it may be biased, i.e. shifted to the right.
     If it's the case, then the adaptation coefficient must be also shifted
     to the right by the same number of bit positions.
  2. This algorithm consumes less CPU cycles per block than single 
     sample algorithm at similar convergence rate.
  3. Right selection of N depends on the change rate of impulse response:
     on static or slow varying channels convergence rate depends on
     selected mu and M, but not on N.
  4. 16x16 routine may converge slower on small errors due to roundoff 
     errors. In that cases, 16x32 routine will give better results although
     convergence rate on bigger errors is the same.

  Precision: 
  16x16    16-bit coefficients, 16-bit data, 16-bit output
  16x32    32-bit coefficients, 16-bit data, 16-bit output
  32x32    32-bit coefficients, 32-bit data, 32-bit output, complex and real
  32x32ep  the same as above but using 72-bit accumulator for intermediate 
           computations
  f        floating point, complex and real
  Input:
  h[M]     impulse response, Q15, Q31 or floating point
  r[N]	   input data vector (near end). First in time value is in 
           r[0], Q15, Q31 or floating point
  x[N+M-1] reference data vector (far end). First in time value is in x[0],  
           Q15, Q31 or floating point
  norm     normalization factor: power of signal multiplied by N, Q15, Q31  
           or floating point
           Fixed-point format for the 32x16-bit variant: Q(2*x+1-bias)
  mu       adaptation coefficient (LMS step), Q(31-bias) or Q(15-bias)
  N        length of data block
  M        length of h
  Output:
  e[N]     estimated error, Q15, Q31 or floating point
  h[M]     updated impulse response, Q15, Q31 or floating point

  Restriction:
  x,r,h,e  should not overlap
  x,r,h,e  aligned on a 8-bytes boundary
  N,M      multiples of 8 and >0
-------------------------------------------------------------------------*/
void fir_blms32x32( int32_t* restrict e, int32_t* restrict h,
                const int32_t* restrict r,
                const int32_t* restrict x,
                int32_t norm,
                int32_t mu,
                int   N,
                int   M)
{  
  ae_int32x2 v_fract;   // scale factor (fraction part)
  int     s_exp;        // scale factor (exponent)
  int m, n;

          ae_int32x2 * restrict Hwr;
          ae_int32x2 * restrict H;
          ae_int32x2 * restrict E;
    const ae_int32x2 * restrict X;
    const ae_int32x2 * restrict R;
    ae_int32x2 h0,h1,h2,h3,e0,e1,e2,e3;
    ae_int32x2 x0,x1,x2,x3,x4,x5;
    ae_f64 q0,q1,q2,q3,q4,q5,q6,q7;

    NASSERT( e && h && r && x && mu >= 0 && norm > 0 && N >= 0 && M >= 0 );
    NASSERT(M%8==0 && N%8==0);
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(r);
    NASSERT_ALIGN8(h);
    NASSERT_ALIGN8(e);

    //
    // Pass the reference signal x[] through the adaptive filter to obtain the
    // predicted signal and calculate the error, i.e. the distance to the 
    // actual input signal r[].
    //
    WUR_AE_CBEGIN0( (uintptr_t)( h + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( h + M ) );
    R=(const ae_int32x2*)r;
    E=(      ae_int32x2*)e;
    H=( ae_int32x2 *)h;
    AE_ADDCIRC_XC(castxcc(ae_f64,H),-8);

    //__Pragma("concurrent")
    for ( n=0; n<N; n+=8 )
    {
        X=(const ae_int32x2*)&x[n];
        AE_L32X2_IP(x0,X,8);
        AE_L32X2_IP(x1,X,8);
        AE_L32X2_IP(x2,X,8);
        AE_L32X2_IP(x3,X,8);

        AE_L32X2_RIC(h0,H);
        AE_L32X2_RIC(h1,H);
        AE_L32X2_IP(x4,X,8);
        AE_L32X2_IP(x5,X,8);

        AE_MULFD32X2RA_FIR_H (q0,q1,x0,x1,h0);
        AE_MULAFD32X2RA_FIR_H(q0,q1,x1,x2,h1);
        AE_MULFD32X2RA_FIR_H (q2,q3,x1,x2,h0);
        AE_MULAFD32X2RA_FIR_H(q2,q3,x2,x3,h1);
        AE_MULFD32X2RA_FIR_H (q4,q5,x2,x3,h0);
        AE_MULAFD32X2RA_FIR_H(q4,q5,x3,x4,h1);
        AE_MULFD32X2RA_FIR_H (q6,q7,x3,x4,h0);
        AE_MULAFD32X2RA_FIR_H(q6,q7,x4,x5,h1);

        x0=x2;
        x1=x3;
        x2=x4;
        x3=x5;
        __Pragma("loop_count min=1")
        for ( m=0; m<((M>>2)-1); m++ )
        {
            AE_L32X2_RIC(h0,H);
            AE_L32X2_RIC(h1,H);
            AE_L32X2_IP(x4,X,8);
            AE_L32X2_IP(x5,X,8);
            AE_MULAFD32X2RA_FIR_H(q0,q1,x0,x1,h0);
            AE_MULAFD32X2RA_FIR_H(q0,q1,x1,x2,h1);
            AE_MULAFD32X2RA_FIR_H(q2,q3,x1,x2,h0);
            AE_MULAFD32X2RA_FIR_H(q2,q3,x2,x3,h1);
            AE_MULAFD32X2RA_FIR_H(q4,q5,x2,x3,h0);
            AE_MULAFD32X2RA_FIR_H(q4,q5,x3,x4,h1);
            AE_MULAFD32X2RA_FIR_H(q6,q7,x3,x4,h0);
            AE_MULAFD32X2RA_FIR_H(q6,q7,x4,x5,h1);
            x0=x2;
            x1=x3;
            x2=x4;
            x3=x5;
        }
        h0=AE_ROUND32X2F48SASYM(q0, q1);
        h1=AE_ROUND32X2F48SASYM(q2, q3);
        h2=AE_ROUND32X2F48SASYM(q4, q5);
        h3=AE_ROUND32X2F48SASYM(q6, q7);
        AE_L32X2_IP(e0,R, 8);
        AE_L32X2_IP(e1,R, 8);
        AE_L32X2_IP(e2,R, 8);
        AE_L32X2_IP(e3,R, 8);
        e0=AE_SUB32S(e0,h0);
        e1=AE_SUB32S(e1,h1);
        e2=AE_SUB32S(e2,h2);
        e3=AE_SUB32S(e3,h3);
        AE_S32X2_IP(e0,E,8);
        AE_S32X2_IP(e1,E,8);
        AE_S32X2_IP(e2,E,8);
        AE_S32X2_IP(e3,E,8);
    }

    // Compute the reciprocal for the normalization factor.
    {
        ae_int32x2 vxw,y1,vdw, vxa, vxb,vyw, y0, vrw;
        ae_f32x2    vaf, vyf;
        ae_int64    B0;
        int ind, x_nsa, mu_exp;
        //
        // Compute the reciprocal for the normalization factor.
        //
        vxw = AE_MOVDA32X2(norm,norm);
        s_exp = AE_NSAZ32_L(vxw);
        vxw = AE_SLAA32(vxw, s_exp);
        /*Calculate reciprocal for the 16 high-order bits
        f(x) = f(x0) + f'(x0)*(x1-x0)
        1/x0 = f(x0)
        1/(x0^2) = f'(x0)
        x1 = x*2^nsa, x1 in 0.5..1
        x1-x0 = dx
        */

        y1 = AE_SRAI32(vxw, 16);//x,Q1.14
        y1 = AE_SUB32S(y1, AE_MOVDA32X2(16384, 16384));
          
        vdw = AE_SRAI32(y1, 5);
        vdw = AE_AND32(vdw, AE_MOVDA32X2(0xFFFFFFFC,0xFFFFFFFC));

        ind = AE_MOVAD32_H(vdw);
        vxa = AE_L16X2M_X((const ae_p16x2s *)recip_table, ind);
        vxb = AE_SEL32_LL(vxa,vxa);//f'=(1/x0^2),Q.23
        vxa = AE_SEL32_HH(vxa,vxa);//f=1/x0, Q.23
        vdw = AE_SLAI32(vdw, 5);
        vdw = AE_SUB32S(y1,vdw);//dx
        vdw = AE_SLAI32(vdw, 1+8);
        vaf = vxa;
        vyw = AE_MULFP32X2RAS(vxb, vdw);/*(1/x0^2)*dx, Q.23*Q1.23->Q1.15*/
        vyw = AE_SLAI32(vyw, 16-8); 
        y0 = AE_SUB32S(vxa, vyw);/*recip(x) = 1/x0 - (1/x0^2)*dx,Q1.23*/
        /*refinement*/
        y1 = AE_SLAI32(y0, 7);  //Q30
        y0 = AE_SLAI32(y0,8);
        B0 = AE_MUL32_LL(vxw,y1);
        B0 = AE_SLAI64(B0,1);
        vxb = AE_MOVINT32X2_FROMINT64(B0);
        vrw = AE_ADD32S(AE_MOVDA32X2(0xE0000000, 0xE0000000),vxb);/*err=recip(x)*x-1,Q1.29*/
        vrw = AE_SEL32_HH(vrw,vrw);
        vrw = AE_SLAI32(vrw, 2);
        vyf = y0;
        AE_MULSFP32X2RAS(vyf, vyf, vrw);   /*recip(x)=recip(x)-err*recip(x), Q.31*/
        vrw = vyf;
        vxa = AE_SEL32_LL(vrw, vrw);
        x_nsa = AE_NSAZ32_L(vxa);
        vxa = AE_SLAA32(vxa, x_nsa);
        // Q(30-s_exp) <- Q61 / Q(31+s_exp)
        vxw = AE_MOVDA32(mu);
        mu_exp = AE_NSAZ32_L(vxw);
        vxw = AE_SLAA32S(vxw, mu_exp);
        B0 = AE_MUL32_LL(vxw,vxa);
        v_fract = AE_TRUNCA32F64S(B0,1);

        s_exp -= mu_exp; // -15..30
        s_exp++;
        s_exp =XT_MIN(s_exp, 31);
        s_exp =XT_MAX(s_exp,-16);
    }

    //
    // Calculate the cross-correlation between the error signal and the 
    // reference signal. Scale the result and update the estimation of the
    // impulse response.
    //
    WUR_AE_CBEGIN0( (uintptr_t)( h + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( h + M ) );
    WUR_AE_CBEGIN1( (uintptr_t)( e + 0 ) );
    WUR_AE_CEND1  ( (uintptr_t)( e + N ) );
    R=(const ae_int32x2*)r;
    E=(      ae_int32x2*)e;
    H=( ae_int32x2 *)h;
    AE_ADDCIRC_XC(castxcc(ae_f64,H),-8);
    Hwr=H;
    //__Pragma("loop_count min=1")
    for ( m=0; m<M; m+=8 )
    {
        X=(const ae_int32x2*)&x[m];
        AE_L32X2_IP(x0,X,8);
        AE_L32X2_IP(x1,X,8);
        AE_L32X2_IP(x2,X,8);
        AE_L32X2_IP(x3,X,8);

        AE_L32X2_XC1(e0,E,8);
        AE_L32X2_XC1(e1,E,8);
        AE_L32X2_IP(x4,X,8);
        AE_L32X2_IP(x5,X,8);
        AE_MULFD32X2RA_FIR_H (q0,q1,x0,x1,e0);
        AE_MULAFD32X2RA_FIR_H(q0,q1,x1,x2,e1);
        AE_MULFD32X2RA_FIR_H (q2,q3,x1,x2,e0);
        AE_MULAFD32X2RA_FIR_H(q2,q3,x2,x3,e1);
        AE_MULFD32X2RA_FIR_H (q4,q5,x2,x3,e0);
        AE_MULAFD32X2RA_FIR_H(q4,q5,x3,x4,e1);
        AE_MULFD32X2RA_FIR_H (q6,q7,x3,x4,e0);
        AE_MULAFD32X2RA_FIR_H(q6,q7,x4,x5,e1);
        x0=x2;
        x1=x3;
        x2=x4;
        x3=x5;
        __Pragma("loop_count min=1")
        for (n=0; n<(N>>2)-1; n++ )
        {
            AE_L32X2_XC1(e0,E,8);
            AE_L32X2_XC1(e1,E,8);
            AE_L32X2_IP(x4,X,8);
            AE_L32X2_IP(x5,X,8);
            AE_MULAFD32X2RA_FIR_H(q0,q1,x0,x1,e0);
            AE_MULAFD32X2RA_FIR_H(q0,q1,x1,x2,e1);
            AE_MULAFD32X2RA_FIR_H(q2,q3,x1,x2,e0);
            AE_MULAFD32X2RA_FIR_H(q2,q3,x2,x3,e1);
            AE_MULAFD32X2RA_FIR_H(q4,q5,x2,x3,e0);
            AE_MULAFD32X2RA_FIR_H(q4,q5,x3,x4,e1);
            AE_MULAFD32X2RA_FIR_H(q6,q7,x3,x4,e0);
            AE_MULAFD32X2RA_FIR_H(q6,q7,x4,x5,e1);
            x0=x2;
            x1=x3;
            x2=x4;
            x3=x5;
        }
        e0=AE_ROUND32X2F48SASYM(q0, q1);
        e1=AE_ROUND32X2F48SASYM(q2, q3);
        e2=AE_ROUND32X2F48SASYM(q4, q5);
        e3=AE_ROUND32X2F48SASYM(q6, q7);
        // scale error and update IR in the reverse order
        q0=AE_MULF32S_HH(e0,v_fract);
        q1=AE_MULF32S_LL(e0,v_fract);
        q2=AE_MULF32S_HH(e1,v_fract);
        q3=AE_MULF32S_LL(e1,v_fract);
        q4=AE_MULF32S_HH(e2,v_fract);
        q5=AE_MULF32S_LL(e2,v_fract);
        q6=AE_MULF32S_HH(e3,v_fract);
        q7=AE_MULF32S_LL(e3,v_fract);

        e0=AE_TRUNCA32X2F64S(q0, q1, s_exp);
        e1=AE_TRUNCA32X2F64S(q2, q3, s_exp);
        e2=AE_TRUNCA32X2F64S(q4, q5, s_exp);
        e3=AE_TRUNCA32X2F64S(q6, q7, s_exp);

        AE_L32X2_RIC(h1,H);
        AE_L32X2_RIC(h0,H);
        h1=AE_ADD32S(h1,e0);
        h0=AE_ADD32S(h0,e1);
        AE_S32X2_RIC(h1,Hwr);
        AE_S32X2_RIC(h0,Hwr);

        AE_L32X2_RIC(h1,H);
        AE_L32X2_RIC(h0,H);
        h1=AE_ADD32S(h1,e2);
        h0=AE_ADD32S(h0,e3);
        AE_S32X2_RIC(h1,Hwr);
        AE_S32X2_RIC(h0,Hwr);
    }
} // fir_blms32x32()
