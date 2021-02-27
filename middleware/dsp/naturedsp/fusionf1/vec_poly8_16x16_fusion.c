/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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
#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Polynomial approximation
  Functions calculate polynomial approximation for all values from given 
  vector. Fixed point functions take polynomial coefficients in Q15 or 
  Q31 precision. 
  NOTE:
  approximation is calculated like Taylor series that is why overflow may 
  potentially occur if cumulative sum of coefficients given from the last 
  to the first coefficient is bigger than 1. To avoid this negative effect,
  all the coefficients may be scaled down and result will be shifted left 
  after all intermediate computations.

  Precision: 
  16x16  16-bit inputs, 16-bit coefficients, 16-bit output. 
  24x24  24-bit inputs, 24-bit coefficients, 24-bit output. 
  32x32  32-bit inputs, 32-bit coefficients, 32-bit output.
  f      floating point

  Input:
  x[N]    input data, Q15, Q31 or floating point
  N       length of vector
  lsh     additional left shift for result
  c[M+1]  coefficients (5 coefficients for vec_poly4_xxx 
          and 9 coefficients for vec_poly8_xxx), Q15, Q31 
          or floating point
  Output:
  z[N]    result, Q15, Q31 or floating point

  Restriction:
  x,c,z should not overlap
  lsh   should be in range 0...31
-------------------------------------------------------------------------*/
void vec_poly8_16x16 (int16_t * restrict z, const int16_t * restrict x, const int16_t * restrict c, int lsh, int N )
{
    ae_int16x4 X,Z;
    ae_f24x2 C0,C1,C2,C3,C4,C5,C6,C7,C8,X0,X1,S0,S1;
    ae_int32x2 ALIGN(8) C[9];
    const ae_int32x2 *pC=(const ae_int32x2 *)C;
    ae_f32x2 t0,t1;
    const ae_int16x4* restrict px;
          ae_int16x4* restrict pz;
    ae_valign ax,az;
    int n;
    NASSERT(c);
    NASSERT(x);
    NASSERT(z);
    if(N<=0) return;
    C[0]=AE_L16M_I((const ae_p16s*)c,0*sizeof(int16_t));
    C[1]=AE_L16M_I((const ae_p16s*)c,1*sizeof(int16_t));
    C[2]=AE_L16M_I((const ae_p16s*)c,2*sizeof(int16_t));
    C[3]=AE_L16M_I((const ae_p16s*)c,3*sizeof(int16_t));
    C[4]=AE_L16M_I((const ae_p16s*)c,4*sizeof(int16_t));
    C[5]=AE_L16M_I((const ae_p16s*)c,5*sizeof(int16_t));
    C[6]=AE_L16M_I((const ae_p16s*)c,6*sizeof(int16_t));
    C[7]=AE_L16M_I((const ae_p16s*)c,7*sizeof(int16_t));
    C[8]=AE_L16M_X((const ae_p16s*)c,8*sizeof(int16_t));
    C0=AE_MOVF24X2_FROMINT32X2(C[0]);
    C1=AE_MOVF24X2_FROMINT32X2(C[1]);
    C2=AE_MOVF24X2_FROMINT32X2(C[2]);
    C3=AE_MOVF24X2_FROMINT32X2(C[3]);
    C4=AE_MOVF24X2_FROMINT32X2(C[4]);
    C5=AE_MOVF24X2_FROMINT32X2(C[5]);
    C6=AE_MOVF24X2_FROMINT32X2(C[6]);
    C7=AE_MOVF24X2_FROMINT32X2(C[7]);
    C8=AE_MOVF24X2_FROMINT32X2(C[8]);

    px=(const ae_int16x4*)x;
    pz=(      ae_int16x4*)z;
    ax=AE_LA64_PP(px);
    az=AE_ZALIGN64();
        AE_LA16X4_IP(X,ax,px);
        X0=AE_MOVF24X2_FROMINT32X2(AE_SRAI32(AE_CVT32X2F16_10(X),8));
        X1=AE_MOVF24X2_FROMINT32X2(AE_SRAI32(AE_CVT32X2F16_32(X),8));
    for (n=0; n<(N&~3); n+=4)
    {
        C8=AE_MOVF24X2_FROMINT32X2(AE_L32X2_X(pC,8*sizeof(ae_int32x2)));  
        S0=C8;
        S1=C8;
        C7=AE_MOVF24X2_FROMINT32X2(AE_L32X2_I(pC,7*sizeof(ae_int32x2)));  
        t0=C7;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C7;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        C6=AE_MOVF24X2_FROMINT32X2(AE_L32X2_I(pC,6*sizeof(ae_int32x2)));  
        t0=C6;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C6;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        C5=AE_MOVF24X2_FROMINT32X2(AE_L32X2_I(pC,5*sizeof(ae_int32x2)));  
        t0=C5;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C5;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        C4=AE_MOVF24X2_FROMINT32X2(AE_L32X2_I(pC,4*sizeof(ae_int32x2)));  
        t0=C4;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C4;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        C3=AE_MOVF24X2_FROMINT32X2(AE_L32X2_I(pC,3*sizeof(ae_int32x2)));  
        t0=C3;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C3;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        C2=AE_MOVF24X2_FROMINT32X2(AE_L32X2_I(pC,2*sizeof(ae_int32x2)));  
        t0=C2;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C2;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        C1=AE_MOVF24X2_FROMINT32X2(AE_L32X2_I(pC,1*sizeof(ae_int32x2)));  
        t0=C1;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C1;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
            AE_LA16X4_IP(X,ax,px);
        //C0=AE_MOVF24X2_FROMINT32X2(AE_L32X2_I(pC,0)); 
        { ae_int32x2 t; AE_L32X2_IP(t,pC,0);C0=AE_MOVF24X2_FROMINT32X2(t); }
        t0=C0;AE_MULAFP24X2RA(t0, X0, S0); 
        t1=C0;AE_MULAFP24X2RA(t1, X1, S1); 
        t0=AE_SLAA32S(t0,lsh+8);
        t1=AE_SLAA32S(t1,lsh+8);
            X0=AE_MOVF24X2_FROMINT32X2(AE_SRAI32(AE_CVT32X2F16_10(X),8));
            X1=AE_MOVF24X2_FROMINT32X2(AE_SRAI32(AE_CVT32X2F16_32(X),8));
        Z=AE_ROUND16X4F32SASYM(t1,t0);
        AE_SA16X4_IP(Z,az,pz);
    }
    AE_SA64POS_FP(az, pz);
    N&=3;
    if(N)
    {
        ae_int16 *pz_=(ae_int16 *)pz;
        ae_int16x4 x0,x1,x2;
        S0=C8;
        S1=C8;
        t0=C7;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C7;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        t0=C6;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C6;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        t0=C5;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C5;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        t0=C4;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C4;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        t0=C3;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C3;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        t0=C2;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C2;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        t0=C1;AE_MULAFP24X2RA(t0, X0, S0); S0=AE_SAT24S(t0);
        t1=C1;AE_MULAFP24X2RA(t1, X1, S1); S1=AE_SAT24S(t1);
        t0=C0;AE_MULAFP24X2RA(t0, X0, S0); 
        t1=C0;AE_MULAFP24X2RA(t1, X1, S1); 
        t0=AE_SLAA32S(t0,lsh+8);
        t1=AE_SLAA32S(t1,lsh+8);
        X=AE_ROUND16X4F32SASYM(t1,t0);
        x0 = AE_SEL16_6543(X, X); 
        x1 = AE_SEL16_5432(X, X);
        x2 = AE_SEL16_5432(x0, x0);
        AE_S16_0_IP(x0, pz_, 2);
        if(N>1) AE_S16_0_IP(x1, pz_, 2);
        if(N>2) AE_S16_0_IP(x2, pz_, 2);
    }
}
