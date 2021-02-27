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
void vec_poly4_16x16 (int16_t * restrict z, const int16_t * restrict x, const int16_t * restrict c, int lsh, int N )
{
    ae_int16x4 X;
    ae_f24x2 C0,C1,C2,C3,C4,X0,X1,S0,S1;
    ae_f32x2 t0,t1;
    const ae_int16x4* px;
          ae_int16x4* pz;
    ae_valign ax,az;
    int n;
    NASSERT(c);
    NASSERT(x);
    NASSERT(z);
    if(N<=0) return;
    C0=AE_MOVF24X2_FROMINT32X2(AE_L16M_I((const ae_p16s*)c,0*sizeof(int16_t)));
    C1=AE_MOVF24X2_FROMINT32X2(AE_L16M_I((const ae_p16s*)c,1*sizeof(int16_t)));
    C2=AE_MOVF24X2_FROMINT32X2(AE_L16M_I((const ae_p16s*)c,2*sizeof(int16_t)));
    C3=AE_MOVF24X2_FROMINT32X2(AE_L16M_I((const ae_p16s*)c,3*sizeof(int16_t)));
    C4=AE_MOVF24X2_FROMINT32X2(AE_L16M_I((const ae_p16s*)c,4*sizeof(int16_t)));
    px=(const ae_int16x4*)x;
    pz=(      ae_int16x4*)z;
    ax=AE_LA64_PP(px);
    az=AE_ZALIGN64();
    for (n=0; n<(N&~3); n+=4)
    {
        AE_LA16X4_IP(X,ax,px);
        X0=AE_MOVF24X2_FROMINT32X2(AE_SRAI32(AE_CVT32X2F16_10(X),8));
        X1=AE_MOVF24X2_FROMINT32X2(AE_SRAI32(AE_CVT32X2F16_32(X),8));
        S0=C4;
        S1=C4;
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
        AE_SA16X4_IP(X,az,pz);
    }
    AE_SA64POS_FP(az, pz);
    N&=3;
    if(N)
    {
        ae_int16 *pz_=(ae_int16 *)pz;
        ae_int16x4 x0,x1,x2;
        AE_LA16X4_IP(X,ax,px);
        X0=AE_MOVF24X2_FROMINT32X2(AE_SRAI32(AE_CVT32X2F16_10(X),8));
        X1=AE_MOVF24X2_FROMINT32X2(AE_SRAI32(AE_CVT32X2F16_32(X),8));
        S0=C4;
        S1=C4;
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
