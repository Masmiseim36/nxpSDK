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
#include "tan16x16_tbl.h"
#include "common.h"
/*-------------------------------------------------------------------------
  Tangent 
  Fixed point functions calculate tan(pi*x) for number written in Q15 or 
  Q31. Floating point functions compute tan(x)
  
  Precision:
  16x16  16-bit inputs (Q15), 16-bit outputs (Q8.7). Accuracy: 1 LSB
  24x24  24-bit inputs, 32-bit outputs. Accuracy: (1.3e-4*y+1LSB) 
                                        if abs(y)<=464873(14.19 in Q15) 
                                        or abs(x)<pi*0.4776
  32x32  32-bit inputs, 32-bit outputs. Accuracy: (1.3e-4*y+1LSB)
                                        if abs(y)<=464873(14.19 in Q15) 
                                        or abs(x)<pi*0.4776
  f      floating point.                Accuracy: 2 ULP

  NOTE:
  1.  Scalar floating point function is compatible with standard ANSI C routines 
      and set errno and exception flags accordingly
  2.  Floating point functions limit the range of allowable input values: [-9099, 9099]
      Whenever the input value does not belong to this range, the result is set to NaN.

  Input:
  x[N]   input data, Q15, Q31 or floating point
  N      length of vectors
  Output:
  y[N]   result, Q8.7 (16-bit function), Q16.15 (24 or 32-bit functions) 
         or floating point

  Restriction:
  x,y - should not overlap

  Scalar versions:
  ----------------
  Return result, Q8.7 (16-bit function), Q16.15 (24 or 32-bit functions) 
  or floating point
-------------------------------------------------------------------------*/
void vec_tan16x16 (int16_t* restrict y, const int16_t *restrict x, int N)
#if 0
{
    int ey0l,ey0h,ey1l,ey1h;
    xtbool2 sx0,sx1,sr0,sr1;
    ae_int16x4 x0,x1,x2,z0;
    ae_int32x2 Z0,Z1,X0,X1,Y0,Y1,E0,E1;
    ae_f32x2   f0,f1;
    ae_f24x2   X20,X21;
    const ae_int16x4 * restrict px;
          ae_int16x4 * restrict pz;
    ae_valign ax,az;
    const ae_int32 * restrict ptan=(const ae_int32 *)polytan16x16_tbl; 
    int n;
    NASSERT(x);
    NASSERT(y);
    if(N<0) return;
    px=(const ae_int16x4 *)x;
    pz=(      ae_int16x4 *)y;
    ax = AE_LA64_PP(px);
    az = AE_ZALIGN64();
    for (n=0; n<(N&~3); n+=4)
    {
        /* load and reduction */
        AE_LA16X4_IP(x0,ax,px);
        X0=AE_CVT32X2F16_10(x0);
        X1=AE_CVT32X2F16_32(x0);
        Y0=AE_SLAI32(AE_NEG32(AE_ABS32(X0)),1);
        Y1=AE_SLAI32(AE_NEG32(AE_ABS32(X1)),1);
        Z0=AE_L32_I(ptan,20);
        sr0=AE_LT32(Z0,AE_ABS32S(Y0));
        sr1=AE_LT32(Z0,AE_ABS32S(Y1));
        Y0=AE_XOR32(X0,Y0);
        Y1=AE_XOR32(X1,Y1);
        sx0=AE_LT32(0,Y0);
        sx1=AE_LT32(0,Y1);
        X0=AE_ABS32(AE_SRAI32(AE_SLAI32(X0,2),8));
        X1=AE_ABS32(AE_SRAI32(AE_SLAI32(X1,2),8));
         /* compute polynomial */
        X20=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),AE_MOVF24X2_FROMINT32X2(X0)));
        X21=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X1),AE_MOVF24X2_FROMINT32X2(X1)));
        Y0=Y1=AE_L32_I(ptan,0);
        f0=f1=AE_L32_I(ptan,4);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptan,8);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptan,12);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptan,16);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=X0;
        f1=X1;
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=AE_SAT24S(f0);
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=AE_SAT24S(f1);

        /* compute reciprocal of Y */
        Y0=AE_MAX32(Y0,1);
        Y1=AE_MAX32(Y1,1);
        ey0l=AE_NSAZ32_L(Y0)-8; ey0h=AE_NSAZ32_L(AE_INTSWAP(Y0))-8;
        ey1l=AE_NSAZ32_L(Y1)-8; ey1h=AE_NSAZ32_L(AE_INTSWAP(Y1))-8;

        X0=AE_SEL32_HL(AE_SLAA32(Y0,ey0h),AE_SLAA32(Y0,ey0l));
        X1=AE_SEL32_HL(AE_SLAA32(Y1,ey1h),AE_SLAA32(Y1,ey1l));
        Z0=AE_SUB32(AE_L32_I(ptan,24),X0);
        Z1=AE_SUB32(AE_L32_I(ptan,24),X1);
        f0=f1=AE_L32_I(ptan,28);
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0)); E0=f0;
        AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(X1)); E1=f1;
        E0=AE_SLAI32(E0,1);
        E1=AE_SLAI32(E1,1);
        f0=Z0;
        f1=Z1;
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
        f0=f1=AE_L32_I(ptan,28);
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0)); E0=f0;
        AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(X1)); E1=f1;
        E0=AE_SLAI32(E0,1);
        E1=AE_SLAI32(E1,1);
        f0=Z0;
        f1=Z1;
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
        Z0=AE_SEL32_HL(AE_SLAA32S(Z0,ey0h+1),AE_SLAA32S(Z0,ey0l+1));
        Z1=AE_SEL32_HL(AE_SLAA32S(Z1,ey1h+1),AE_SLAA32S(Z1,ey1l+1));
        /* finally, select tan/cotan and the sign */
        AE_MOVT32X2(Y0,Z0,sr0);
        AE_MOVT32X2(Y1,Z1,sr1);
        AE_MOVT32X2(Y0,AE_NEG32(Y0),sx0);
        AE_MOVT32X2(Y1,AE_NEG32(Y1),sx1);
        z0=AE_ROUND16X4F32SASYM(Y1,Y0);
        AE_SA16X4_IP(z0,az,pz);
    }
    AE_SA64POS_FP(az, pz);
    /* tail (1...3 elements) */
    N&=3;
    if(N)
    {
        ae_int16 *pz_=(ae_int16 *)pz;
        /* load and reduction */
        AE_LA16X4_IP(x0,ax,px);
        X0=AE_CVT32X2F16_10(x0);
        X1=AE_CVT32X2F16_32(x0);
        Y0=AE_SLAI32(AE_NEG32(AE_ABS32(X0)),1);
        Y1=AE_SLAI32(AE_NEG32(AE_ABS32(X1)),1);
        Z0=AE_L32_I(ptan,20);
        sr0=AE_LT32(Z0,AE_ABS32S(Y0));
        sr1=AE_LT32(Z0,AE_ABS32S(Y1));
        Y0=AE_XOR32(X0,Y0);
        Y1=AE_XOR32(X1,Y1);
        sx0=AE_LT32(0,Y0);
        sx1=AE_LT32(0,Y1);
        X0=AE_ABS32(AE_SRAI32(AE_SLAI32(X0,2),8));
        X1=AE_ABS32(AE_SRAI32(AE_SLAI32(X1,2),8));
         /* compute polynomial */
        X20=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),AE_MOVF24X2_FROMINT32X2(X0)));
        X21=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X1),AE_MOVF24X2_FROMINT32X2(X1)));
        Y0=Y1=AE_L32_I(ptan,0);
        f0=f1=AE_L32_I(ptan,4);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptan,8);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptan,12);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptan,16);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=X0;
        f1=X1;
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=AE_SAT24S(f0);
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=AE_SAT24S(f1);

        /* compute reciprocal of Y */
        Y0=AE_MAX32(Y0,1);
        Y1=AE_MAX32(Y1,1);
        ey0l=AE_NSAZ32_L(Y0)-8; ey0h=AE_NSAZ32_L(AE_INTSWAP(Y0))-8;
        ey1l=AE_NSAZ32_L(Y1)-8; ey1h=AE_NSAZ32_L(AE_INTSWAP(Y1))-8;

        X0=AE_SEL32_HL(AE_SLAA32(Y0,ey0h),AE_SLAA32(Y0,ey0l));
        X1=AE_SEL32_HL(AE_SLAA32(Y1,ey1h),AE_SLAA32(Y1,ey1l));
        Z0=AE_SUB32(AE_L32_I(ptan,24),X0);
        Z1=AE_SUB32(AE_L32_I(ptan,24),X1);
        f0=f1=AE_L32_I(ptan,28);
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0)); E0=f0;
        AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(X1)); E1=f1;
        E0=AE_SLAI32(E0,1);
        E1=AE_SLAI32(E1,1);
        f0=Z0;
        f1=Z1;
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
        f0=f1=AE_L32_I(ptan,28);
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0)); E0=f0;
        AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(X1)); E1=f1;
        E0=AE_SLAI32(E0,1);
        E1=AE_SLAI32(E1,1);
        f0=Z0;
        f1=Z1;
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
        Z0=AE_SEL32_HL(AE_SLAA32S(Z0,ey0h+1),AE_SLAA32S(Z0,ey0l+1));
        Z1=AE_SEL32_HL(AE_SLAA32S(Z1,ey1h+1),AE_SLAA32S(Z1,ey1l+1));
        /* finally, select tan/cotan and the sign */
        AE_MOVT32X2(Y0,Z0,sr0);
        AE_MOVT32X2(Y1,Z1,sr1);
        AE_MOVT32X2(Y0,AE_NEG32(Y0),sx0);
        AE_MOVT32X2(Y1,AE_NEG32(Y1),sx1);
        z0=AE_ROUND16X4F32SASYM(Y1,Y0);
        x0 = AE_SEL16_6543(z0, z0); 
        x1 = AE_SEL16_5432(z0, z0);
        x2 = AE_SEL16_5432(x0, x0);
        AE_S16_0_IP(x0, pz_, 2);
        if(N>1) AE_S16_0_IP(x1, pz_, 2);
        if(N>2) AE_S16_0_IP(x2, pz_, 2);
    }
}
#else   /* variant with scratch storage */
{
    int32_t ALIGN(8) scratch[MAX_ALLOCA_SZ/sizeof(int32_t)];
    int M;
    int ey0l,ey0h,ey1l,ey1h;
    xtbool2 sx0,sx1,sr0,sr1;
    ae_int16x4 x0,x1,x2,z0;
    ae_int32x2 Z0,Z1,X0,X1,Y0,Y1,E0,E1;
    ae_f32x2   f0,f1;
    ae_f24x2   X20,X21;
    const ae_int16x4 * restrict px;
          ae_int16x4 * restrict pz;
    ae_valign ax,az;
    const ae_int32 * restrict ptan=(const ae_int32 *)polytan16x16_tbl; 
          ae_int32x2 * pScrWr;
    const ae_int32x2 * pScrRd;

    int n;
    NASSERT(x);
    NASSERT(y);
    if(N<0) return;
    M=N;
    while(M>3)
    {
        N=(XT_MIN(M,(MAX_ALLOCA_SZ/sizeof(int32_t)))&~3);
        pScrWr=(       ae_int32x2 *)scratch;
        pScrRd=( const ae_int32x2 *)scratch;
        px=(const ae_int16x4 *)x;
        pz=(      ae_int16x4 *)y;
        ax = AE_LA64_PP(px);
        az = AE_ZALIGN64();
        for (n=0; n<N; n+=4)
        {
            /* load and reduction */
            AE_LA16X4_IP(x0,ax,px);
            X0=AE_CVT32X2F16_10(x0);
            X1=AE_CVT32X2F16_32(x0);
            X0=AE_ABS32(AE_SRAI32(AE_SLAI32(X0,2),8));
            X1=AE_ABS32(AE_SRAI32(AE_SLAI32(X1,2),8));
             /* compute polynomial */
            X20=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),AE_MOVF24X2_FROMINT32X2(X0)));
            X21=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X1),AE_MOVF24X2_FROMINT32X2(X1)));
            Y0=Y1=AE_L32_I(ptan,0);
            f0=f1=AE_L32_I(ptan,4);
            AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
            AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
            f0=f1=AE_L32_I(ptan,8);
            AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
            AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
            f0=f1=AE_L32_I(ptan,12);
            AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
            AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
            f0=f1=AE_L32_I(ptan,16);
            AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
            AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
            f0=X0;
            f1=X1;
            AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=AE_SAT24S(f0);
            AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=AE_SAT24S(f1);
            Y0=AE_MAX32(Y0,1);
            Y1=AE_MAX32(Y1,1);
            AE_S32X2_IP(Y0,pScrWr,sizeof(Y0));
            AE_S32X2_IP(Y1,pScrWr,sizeof(Y1));
        }
        __Pragma("no_reorder");
        px=(const ae_int16x4 *)x;
        ax = AE_LA64_PP(px);
        for (n=0; n<N; n+=4)
        {
            AE_LA16X4_IP(x0,ax,px);
            X0=AE_CVT32X2F16_10(x0);
            X1=AE_CVT32X2F16_32(x0);
            Y0=AE_SLAI32(AE_NEG32(AE_ABS32(X0)),1);
            Y1=AE_SLAI32(AE_NEG32(AE_ABS32(X1)),1);
            Z0=AE_L32_I(ptan,20);
            sr0=AE_LT32(Z0,AE_ABS32S(Y0));
            sr1=AE_LT32(Z0,AE_ABS32S(Y1));
            Y0=AE_XOR32(X0,Y0);
            Y1=AE_XOR32(X1,Y1);
            sx0=AE_LT32(0,Y0);
            sx1=AE_LT32(0,Y1);

            /* compute reciprocal of Y */
            AE_L32X2_IP(Y0,pScrRd,sizeof(Y0));
            AE_L32X2_IP(Y1,pScrRd,sizeof(Y1));
            ey0l=AE_NSAZ32_L(Y0)-8; ey0h=AE_NSAZ32_L(AE_INTSWAP(Y0))-8;
            ey1l=AE_NSAZ32_L(Y1)-8; ey1h=AE_NSAZ32_L(AE_INTSWAP(Y1))-8;

            X0=AE_SEL32_HL(AE_SLAA32(Y0,ey0h),AE_SLAA32(Y0,ey0l));
            X1=AE_SEL32_HL(AE_SLAA32(Y1,ey1h),AE_SLAA32(Y1,ey1l));
            Z0=AE_SUB32(AE_L32_I(ptan,24),X0);
            Z1=AE_SUB32(AE_L32_I(ptan,24),X1);
            f0=f1=AE_L32_I(ptan,28);
            AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0)); E0=f0;
            AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(X1)); E1=f1;
            E0=AE_SLAI32(E0,1);
            E1=AE_SLAI32(E1,1);
            f0=Z0;
            f1=Z1;
            AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
            AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
            f0=f1=AE_L32_I(ptan,28);
            AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0)); E0=f0;
            AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(X1)); E1=f1;
            E0=AE_SLAI32(E0,1);
            E1=AE_SLAI32(E1,1);
            f0=Z0;
            f1=Z1;
            AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
            AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
            Z0=AE_SEL32_HL(AE_SLAA32S(Z0,ey0h+1),AE_SLAA32S(Z0,ey0l+1));
            Z1=AE_SEL32_HL(AE_SLAA32S(Z1,ey1h+1),AE_SLAA32S(Z1,ey1l+1));
            /* finally, select tan/cotan and the sign */
            AE_MOVT32X2(Y0,Z0,sr0);
            AE_MOVT32X2(Y1,Z1,sr1);
            AE_MOVT32X2(Y0,AE_NEG32(Y0),sx0);
            AE_MOVT32X2(Y1,AE_NEG32(Y1),sx1);
            z0=AE_ROUND16X4F32SASYM(Y1,Y0);
            AE_SA16X4_IP(z0,az,pz);
        }
        AE_SA64POS_FP(az, pz);
        x+=N;
        y+=N;
        M-=N;
    }
    /* tail (1...3 elements) */
    N=M&3;
    if(N)
    {
        ae_int16 *pz_;
        px=(const ae_int16x4 *)x;
        pz=(      ae_int16x4 *)y;
        pz_=(ae_int16 *)pz;
        ax = AE_LA64_PP(px);
        /* load and reduction */
        AE_LA16X4_IP(x0,ax,px);
        X0=AE_CVT32X2F16_10(x0);
        X1=AE_CVT32X2F16_32(x0);
        Y0=AE_SLAI32(AE_NEG32(AE_ABS32(X0)),1);
        Y1=AE_SLAI32(AE_NEG32(AE_ABS32(X1)),1);
        Z0=AE_L32_I(ptan,20);
        sr0=AE_LT32(Z0,AE_ABS32S(Y0));
        sr1=AE_LT32(Z0,AE_ABS32S(Y1));
        Y0=AE_XOR32(X0,Y0);
        Y1=AE_XOR32(X1,Y1);
        sx0=AE_LT32(0,Y0);
        sx1=AE_LT32(0,Y1);
        X0=AE_ABS32(AE_SRAI32(AE_SLAI32(X0,2),8));
        X1=AE_ABS32(AE_SRAI32(AE_SLAI32(X1,2),8));
         /* compute polynomial */
        X20=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),AE_MOVF24X2_FROMINT32X2(X0)));
        X21=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X1),AE_MOVF24X2_FROMINT32X2(X1)));
        Y0=Y1=AE_L32_I(ptan,0);
        f0=f1=AE_L32_I(ptan,4);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptan,8);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptan,12);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptan,16);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=X0;
        f1=X1;
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=AE_SAT24S(f0);
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=AE_SAT24S(f1);

        /* compute reciprocal of Y */
        Y0=AE_MAX32(Y0,1);
        Y1=AE_MAX32(Y1,1);
        ey0l=AE_NSAZ32_L(Y0)-8; ey0h=AE_NSAZ32_L(AE_INTSWAP(Y0))-8;
        ey1l=AE_NSAZ32_L(Y1)-8; ey1h=AE_NSAZ32_L(AE_INTSWAP(Y1))-8;

        X0=AE_SEL32_HL(AE_SLAA32(Y0,ey0h),AE_SLAA32(Y0,ey0l));
        X1=AE_SEL32_HL(AE_SLAA32(Y1,ey1h),AE_SLAA32(Y1,ey1l));
        Z0=AE_SUB32(AE_L32_I(ptan,24),X0);
        Z1=AE_SUB32(AE_L32_I(ptan,24),X1);
        f0=f1=AE_L32_I(ptan,28);
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0)); E0=f0;
        AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(X1)); E1=f1;
        E0=AE_SLAI32(E0,1);
        E1=AE_SLAI32(E1,1);
        f0=Z0;
        f1=Z1;
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
        f0=f1=AE_L32_I(ptan,28);
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0)); E0=f0;
        AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(X1)); E1=f1;
        E0=AE_SLAI32(E0,1);
        E1=AE_SLAI32(E1,1);
        f0=Z0;
        f1=Z1;
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
        Z0=AE_SEL32_HL(AE_SLAA32S(Z0,ey0h+1),AE_SLAA32S(Z0,ey0l+1));
        Z1=AE_SEL32_HL(AE_SLAA32S(Z1,ey1h+1),AE_SLAA32S(Z1,ey1l+1));
        /* finally, select tan/cotan and the sign */
        AE_MOVT32X2(Y0,Z0,sr0);
        AE_MOVT32X2(Y1,Z1,sr1);
        AE_MOVT32X2(Y0,AE_NEG32(Y0),sx0);
        AE_MOVT32X2(Y1,AE_NEG32(Y1),sx1);
        z0=AE_ROUND16X4F32SASYM(Y1,Y0);
        x0 = AE_SEL16_6543(z0, z0); 
        x1 = AE_SEL16_5432(z0, z0);
        x2 = AE_SEL16_5432(x0, x0);
        AE_S16_0_IP(x0, pz_, 2);
        if(N>1) AE_S16_0_IP(x1, pz_, 2);
        if(N>2) AE_S16_0_IP(x2, pz_, 2);
    }
}
#endif
