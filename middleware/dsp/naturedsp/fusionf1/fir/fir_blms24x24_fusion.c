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
    Blockwise Adaptive LMS Algorithm for Real Data, 24x24-bit
    C code optimized for Fusion
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
#include "baseop.h"

void fir_blms24x24( f24 * restrict e,
                    f24 * restrict h,
              const f24 * restrict r,
              const f24 * restrict x,
              f24 norm, f24 mu,
              int N, int M )
{

        ae_f24x2   * restrict pe = (      ae_f24x2 *)e;
        ae_f24x2   * restrict ph = (      ae_f24x2 *)h;
  const ae_f24x2   * restrict px = (const ae_f24x2 *)x;
  const ae_f24x2   * restrict pr = (const ae_f24x2 *)r;
  const ae_f24x2   * restrict px1;  
  const ae_f24x2   * restrict ph1;  
  int         n, m, s_exp, x_nsa, mu_exp;
  ae_f64      A0, A1, A2, A3 ;
  ae_int64    z1;
  ae_int32x2  vxw,vrw;

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
  /*
     Pass the reference signal x[] through the adaptive filter to obtain the
     predicted signal and calculate the error, i.e. the distance to the 
     actual input signal r[].
  */
    for ( n=0; n<N; n+=4 )
    {
        ae_f24x2 X23,X34,H01,H23;
        ae_f24x2 R01,R23,E01,E23;
        ae_f24x2 x0,x1;
        ae_valign ax;

        ph = (ae_f24x2 *)(h+M-2);
        px = (const ae_f24x2 *)(x+n);
        px1= (const ae_f24x2 *)(x+n+1);
        ax = AE_LA64_PP(px1);

        A2 = A3 = AE_ZERO();
        AE_L32X2F24_RIP(H01,ph);
        AE_L32X2F24_IP (X23,px,8);
        AE_LA32X2F24_IP(X34,ax,px1);
        A0=AE_MULZAAFD24_HH_LL(X23,H01);
        A1=AE_MULZAAFD24_HH_LL(X34,H01);
        for ( m=0; m<M-2; m+=2 )
        {
            AE_L32X2F24_RIP(H23,ph);
            AE_L32X2F24_IP (X23,px,8);
            AE_LA32X2F24_IP(X34,ax,px1);
            AE_MULAAFD24_HH_LL(A0,X23,H23);
            AE_MULAAFD24_HH_LL(A1,X34,H23);
            AE_MULAAFD24_HH_LL(A2,X23,H01);
            AE_MULAAFD24_HH_LL(A3,X34,H01);
            H01=H23;
        }
        AE_L32X2F24_IP (X23,px,8);
        AE_LA32X2F24_IP(X34,ax,px1);
        AE_MULAAFD24_HH_LL(A2,X23,H01);
        AE_MULAAFD24_HH_LL(A3,X34,H01);
        x0 = AE_ROUND24X2F48SASYM(A0,A1);
        x1 = AE_ROUND24X2F48SASYM(A2,A3);
        AE_L32X2F24_IP (R01,pr,2*sizeof(int32_t));
        AE_L32X2F24_IP (R23,pr,2*sizeof(int32_t));
        E01=AE_SUB24S(R01,x0);
        E23=AE_SUB24S(R23,x1);

        AE_S32X2F24_IP (E01,pe,2*sizeof(int32_t));
        AE_S32X2F24_IP (E23,pe,2*sizeof(int32_t));
    }
    /* Compute the reciprocal for the normalization factor */
    norm&=0xffffff00;
    mu  &=0xffffff00;
    s_exp = AE_NSAZ32_L((int32_t) norm);
    vxw = (int32_t) norm;
    vxw = AE_SLAA32(vxw, s_exp);
    /* reciprocal of Q31 in Q30: 6 LSB accuracy */
    {
        ae_f64 q;
        ae_int32x2 y,e,x,_1Q30;
        x=vxw;
        _1Q30=AE_MOVDA32((int)0x40000000);
        y=AE_SUB32 (_1Q30,x);/* no saturation here!!! */
        y=AE_ADD32 (_1Q30,y);
        y=AE_INT32X2_ADD32S(_1Q30,y);

        q=AE_MULF32S_HH(x,y);          
        e=AE_SUB32S(_1Q30,AE_ROUND32X2F64SASYM(q,q));
        q=AE_MULF32S_HH(e,y);  e=AE_ROUND32X2F64SASYM(q,q);
        y=AE_ADD32 (y,e);
        y=AE_ADD32 (y,e);

        q=AE_MULF32S_HH(x,y);          
        e=AE_SUB32S(_1Q30,AE_ROUND32X2F64SASYM(q,q));
        q=AE_MULF32S_HH(e,y);  e=AE_ROUND32X2F64SASYM(q,q);
        y=AE_ADD32 (y,e);
        y=AE_ADD32 (y,e);

        q=AE_MULF32S_HH(x,y);          
        e=AE_SUB32S(_1Q30,AE_ROUND32X2F64SASYM(q,q));
        q=AE_MULF32S_HH(e,y);  e=AE_ROUND32X2F64SASYM(q,q);
        y=AE_ADD32 (y,e);
        y=AE_ADD32 (y,e);
        vrw=y;
    }

    {
        ae_int32x2  vxa;
        z1 = AE_MOVINT64_FROMINT32X2(AE_MOVDA32X2(0,1));
        vxa = AE_SEL32_HH(vrw, vrw);
        x_nsa = AE_NSAZ32_L(vxa);
        vxa = AE_SLAA32(vxa, x_nsa);
        vxa = AE_SRAI32(vxa, 8);
        /*------------------------------------*/
        mu_exp = AE_NSAZ32_L((int32_t) mu);
        vxw = (int32_t) mu;
        vxw = AE_SRAA32(vxw,8);
        // Q(23+8+mu_exp) <- Q(23+8) + mu_exp
        vxw = AE_SLAA32S(vxw, mu_exp);
        // Q(22+8-s_exp+mu_exp) <- Q(22+8-s_exp)*Q(23+8+mu_exp) - 16 + 1 - 24 + 8
        A0 = AE_MUL32_LL(vxw,vxa);
        A0 = AE_SRAI64(A0,23);
        vxw = AE_MOVINT32X2_FROMINT64(A0);
        vxw = AE_SEL32_LL(vxw,vxw); 
        s_exp -= mu_exp; // -31..30
    }
    /*
    Calculate the cross-correlation between the error signal and the 
    reference signal. Scale the result and update the estimation of the
    impulse response.
    */
    z1 = AE_SLAA64(z1,(14-s_exp)); 

    ph = (      ae_f24x2 *)h;
    ph = ph + (M/2)-1;
    px = (const ae_f24x2 *)x;
    for ( m=0; m<M; m+=4 )
    {
        int n;
        ae_valign ax;
        ae_f24x2 E01,E23,X01,X12,X23,X34;
        ae_f24x2 x0,x1,x4,x5;
        ae_f24x2    val;
        pe=(      ae_f24x2*)e;
        px=(const ae_f24x2*)(x+m);
        px1=(const ae_f24x2*)(x+m+1);
        ax=AE_LA64_PP(px1);
        AE_L32X2F24_IP(E23,pe,2*sizeof(int32_t));
        AE_L32X2F24_IP (X01,px,2*sizeof(int32_t));
        AE_LA32X2F24_IP(X12,ax,px1);
        A2=A3=AE_ZERO();
        A0=AE_MULZAAFD24_HH_LL(X01,E23);
        A1=AE_MULZAAFD24_HH_LL(X12,E23);
        for ( n=0; n<N-2; n+=2 )
        {
            E01=E23;
            AE_L32X2F24_IP(E23,pe,2*sizeof(int32_t));
            AE_L32X2F24_IP (X23,px,2*sizeof(int32_t));
            AE_LA32X2F24_IP(X34,ax,px1);
            AE_MULAAFD24_HH_LL(A0,X23,E23);
            AE_MULAAFD24_HH_LL(A1,X34,E23);
            AE_MULAAFD24_HH_LL(A2,X23,E01);
            AE_MULAAFD24_HH_LL(A3,X34,E01);
        }
        E01=E23;
        AE_L32X2F24_IP (X23,px,2*sizeof(int32_t));
        AE_LA32X2F24_IP(X34,ax,px1);
        AE_MULAAFD24_HH_LL(A2,X23,E01);
        AE_MULAAFD24_HH_LL(A3,X34,E01);
        x0 = AE_ROUND24X2F48SASYM(A0,A1);
        x1 = AE_ROUND24X2F48SASYM(A2,A3);
        val = AE_MOVF24X2_FROMINT32X2(vxw);
        A0 = AE_MULF32S_HH(val,x0);
        A1 = AE_MULF32S_LL(val,x0);
        A2 = AE_MULF32S_HH(val,x1);
        A3 = AE_MULF32S_LL(val,x1);
        ASSERT( s_exp >= -31 && s_exp < 31 );
        // Q31 <- Q(17.46-s_exp) + s_exp - 15 w/ rounding and saturation
        if ( s_exp < 15 )
        {
          A0 = AE_ADD64S(A0,z1);
          A1 = AE_ADD64S(A1,z1);
          A2 = AE_ADD64S(A2,z1);
          A3 = AE_ADD64S(A3,z1);
          A0 = AE_SRAA64(A0,(15 - s_exp));
          A1 = AE_SRAA64(A1,(15 - s_exp));
          A2 = AE_SRAA64(A2,(15 - s_exp));
          A3 = AE_SRAA64(A3,(15 - s_exp));
          x0 = AE_MOVF24X2_FROMINT32X2(AE_TRUNCA32X2F64S(A0,A1,24));
          x1 = AE_MOVF24X2_FROMINT32X2(AE_TRUNCA32X2F64S(A2,A3,24));
        }
        else
        {
          x0 = AE_MOVF24X2_FROMINT32X2(AE_SLAA32S( AE_TRUNCA32X2F64S(A0,A1,32),s_exp - 15 - 8));
          x1 = AE_MOVF24X2_FROMINT32X2(AE_SLAA32S( AE_TRUNCA32X2F64S(A2,A3,32),s_exp - 15 - 8));
        }
        ph1 = (ae_f24x2 *) ph;
        AE_L32X2F24_RIP(x4, ph1);
        AE_L32X2F24_RIP(x5, ph1);
        // Q31 <- Q31 + Q31 w/ saturation
        x0 = AE_ADDSP24S(x0,x4);
        x1 = AE_ADDSP24S(x1,x5);
        AE_S32X2F24_RIP(x0,ph); 
        AE_S32X2F24_RIP(x1,ph);
    }
} // fir_blms24x24()
