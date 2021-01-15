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
    Blockwise Adaptive LMS Algorithm for Complex Data, 32x32-bit
    Reference C code
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
void cxfir_blms32x32( complex_fract32* restrict e, complex_fract32* restrict h,
                const complex_fract32* restrict r,
                const complex_fract32* restrict x,
                int32_t norm,
                int32_t mu,
                int   N,
                int   M)
{  
    const ae_int32x2* restrict pX;
    const ae_int32x2* restrict pR;
          ae_int32x2* restrict pE;
          ae_int32x2* restrict pH;
    int       s_exp;
    int n, m;
    ae_int32x2 v_fract;
   
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
    pR=(const ae_int32x2*)(r);
    pE=(      ae_int32x2*)(e);
    for ( n=0; n<N; n+=2 )
    {
        ae_int64 a0re,a0im,a1re,a1im;
        ae_int32x2 x0,x1,hh,t0,t1,r0,r1;
        pX=(const ae_int32x2*)(x+n);
        pH=(      ae_int32x2*)(h+M-1);
        AE_L32X2_IP(x0,pX,sizeof(ae_int32x2));
        a0re=a0im=a1re=a1im=AE_ZERO64(); 
        __Pragma("loop_count min=8, factor=2")
        for ( m=0; m<M; m++ )
        {
            AE_L32X2_XP(hh,pH,-(int)sizeof(ae_int32x2));
            AE_L32X2_IP(x1,pX,sizeof(ae_int32x2));
                // Q16.47 
            AE_MULAAFD32RA_HH_LL(a0re,x0,hh);
            AE_MULSAFD32RA_HL_LH(a0im,x0,hh);
            AE_MULAAFD32RA_HH_LL(a1re,x1,hh);
            AE_MULSAFD32RA_HL_LH(a1im,x1,hh);
            x0=x1;
        }
        // Q(23+8) <- [ Q16.47 - 24 w/ rounding and saturation ] + 8
        t0=AE_ROUND32X2F48SASYM(a0re,a0im);
        t1=AE_ROUND32X2F48SASYM(a1re,a1im);
        AE_L32X2_IP(r0,pR,sizeof(ae_int32x2));
        AE_L32X2_IP(r1,pR,sizeof(ae_int32x2));
        r0=AE_SUB32S(r0,t0);
        r1=AE_SUB32S(r1,t1);
        AE_S32X2_IP(r0,pE,sizeof(ae_int32x2));
        AE_S32X2_IP(r1,pE,sizeof(ae_int32x2));
    }

    //
    // Compute the reciprocal for the normalization factor.
    //
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
        s_exp =XT_MIN(s_exp, 30);
        s_exp =XT_MAX(s_exp,-31);
    }

    //
    // Calculate the cross-correlation between the error signal and the 
    // reference signal. Scale the result and update the estimation of the
    // impulse response.
    //

    pH=(ae_int32x2*)(h+M-2);
    for ( m=0; m<M; m+=2)
    {
        ae_int64 a0re,a0im,a1re,a1im;
        ae_int32x2 ee,x0,x1,t0,t1,h0,h1;
        a0re=a0im=a1re=a1im=AE_ZERO64();
        pX=(const ae_int32x2*)(x+m);
        pE=(      ae_int32x2*)(e);
        AE_L32X2_IP(x0,pX,sizeof(ae_int32x2));
        __Pragma("loop_count min=8, factor=2")
        for (n=0; n<N; n++ )
        {
            // Q16.47 <- Q16.47 + ( Q(23+8)*(23+8) - 16 + 1 )
            AE_L32X2_IP(x1,pX,sizeof(ae_int32x2));
            AE_L32X2_IP(ee,pE,sizeof(ae_int32x2));
            AE_MULAAFD32RA_HH_LL(a0re,x0,ee);
            AE_MULSAFD32RA_HL_LH(a0im,x0,ee);
            AE_MULAAFD32RA_HH_LL(a1re,x1,ee);
            AE_MULSAFD32RA_HL_LH(a1im,x1,ee);
            x0=x1;
        }
        // Q(23+8) <- [ Q16.47 - 24 w/ rounding and saturation ] + 8
        t0=AE_ROUND32X2F48SASYM(a0re,a0im);
        t1=AE_ROUND32X2F48SASYM(a1re,a1im);
        // Q(17.46-s_exp) <- Q(23+8)*Q(22+8-s_exp) - 16 + 1
        a0re=AE_MULF32S_HH(t0,v_fract);
        a0im=AE_MULF32S_LL(t0,v_fract);
        a1re=AE_MULF32S_HH(t1,v_fract);
        a1im=AE_MULF32S_LL(t1,v_fract);
        ASSERT( s_exp >= -31 && s_exp < 31 );
        t0=AE_TRUNCA32X2F64S(a0re,a0im,s_exp+1);
        t1=AE_TRUNCA32X2F64S(a1re,a1im,s_exp+1);
        h0=AE_L32X2_I(pH,1*sizeof(ae_int32x2));
        h1=AE_L32X2_I(pH,0*sizeof(ae_int32x2));
        // Q31 <- Q31 + Q31 w/ saturation
        h0=AE_ADD32S(h0,t0);
        h1=AE_ADD32S(h1,t1);
        AE_S32X2_I (h0,pH,1*sizeof(ae_int32x2));
        AE_S32X2_XP(h1,pH,-2*(int)sizeof(ae_int32x2));
    }
} // cxfir_blms32x32()
