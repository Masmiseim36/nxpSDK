/* ------------------------------------------------------------------------ */
/* Copyright (c) 2020 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2020 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_types.h"
#include "common.h"
#include "divide_q31.h"

/* -----------------------------------------------------
accurate division of two numbers giving the result in Q31 
Input:
x,y - in Q31
Output:
returned value is division result in Q31
-----------------------------------------------------*/
int32_t divide_q31(int32_t x,int32_t y)
#if 0
{
    int sy,sx;
    int32_t z,e;
    NASSERT(y>0);
    sx=S_exp0_l(x); x<<=sx;
    sy=S_exp0_l(y); y<<=sy;
    /* first approximation */
    z=(int32_t)0xBAEC0000-y;
    /* 4 iterations to achieve 1 LSB for reciprocal */
    e=0x40000000-satQ31((((int64_t)y*z)+(1L<<30))>>31); e<<=1; z=L_add_ll(z,satQ31((((int64_t)z*e)+(1L<<30))>>31));
    e=0x40000000-satQ31((((int64_t)y*z)+(1L<<30))>>31); e<<=1; z=L_add_ll(z,satQ31((((int64_t)z*e)+(1L<<30))>>31));
    e=0x40000000-satQ31((((int64_t)y*z)+(1L<<30))>>31); e<<=1; z=L_add_ll(z,satQ31((((int64_t)z*e)+(1L<<30))>>31));
    e=0x40000000-satQ31((((int64_t)y*z)+(1L<<30))>>31); e<<=1; z=L_add_ll(z,satQ31((((int64_t)z*e)+(1L<<30))>>31));
    /* */
    z=satQ31((((int64_t)x*z)+(1L<<30))>>31); 
    z=L_shl_l(z,sy-sx+1);
    return z;
}
#else
{
    int sy,sx;
    ae_int32x2 Z,E,X,Y;
   // NASSERT(y>0);
    X=x; Y=y;
    sx=AE_NSAZ32_L(X); X=AE_SLAA32(X,sx);
    sy=AE_NSAZ32_L(Y); Y=AE_SLAA32(Y,sy);
    /* first approximation */
    Z=AE_SUB32(AE_MOVDA32(0xBAEC0000),Y);
    /* 4 iterations to achieve 1 LSB for reciprocal */
    E=0x40000000; AE_MULSFP32X2RAS(E,Y,Z); AE_MULAFP32X2RAS(Z,Z,AE_ADD32(E,E));
    E=0x40000000; AE_MULSFP32X2RAS(E,Y,Z); AE_MULAFP32X2RAS(Z,Z,AE_ADD32(E,E));
    E=0x40000000; AE_MULSFP32X2RAS(E,Y,Z); AE_MULAFP32X2RAS(Z,Z,AE_ADD32(E,E));
    E=0x40000000; AE_MULSFP32X2RAS(E,Y,Z); AE_MULAFP32X2RAS(Z,Z,AE_ADD32(E,E));
    /* */
    Z=AE_SLAA32S(AE_MULFP32X2RAS(X,Z),sy-sx+1);
    return AE_MOVAD32_H(Z);
}
#endif

/* -----------------------------------------------------
reciprocal of y (y[n]=x/y[n] in Q31)
Input:
x    - input in Q31
y[N] - input in Q31
Output:
y[N] - reciprocal in Q31
Restrictions:
y -aligned
N - multiple of 4
-----------------------------------------------------*/
void recip_qx(int32_t x,int32_t* y,int N)
#if 0
{
    int n;
    NASSERT_ALIGN(y,HIFI_SIMD_WIDTH);
    NASSERT(N%4==0);
    for (n=0; n<N; n++)
        y[n]=divide_q31(x,y[n]);
}
#else
{
    int n;
    const ae_int32x2* restrict pYrd=(const ae_int32x2* )y;
          ae_int32x2* restrict pYwr=(      ae_int32x2* )y;
    int syl,syh,sx;
    ae_int32x2 Z,E,X,Y;
   // NASSERT(y>0);
    X=x; 
    sx=AE_NSAZ32_L(X); X=AE_SLAA32(X,sx);
    NASSERT_ALIGN(y,HIFI_SIMD_WIDTH);
    NASSERT(N%4==0);
    __Pragma("loop_count factor=2")
    for (n=0; n<(N>>1); n++)
    {
        AE_L32X2_IP(Y,pYrd,sizeof(ae_int32x2));
        syl = AE_NSAZ32_L(Y);
        syh = AE_NSAZ32_L(AE_SEL32_HH(Y,Y));
        AE_MOVSARA7X2(syh, syl);
        Y = AE_SLAS32S(Y);
        /* first approximation */
        Z=AE_SUB32(AE_MOVDA32(0xBAEC0000),Y);
        /* 4 iterations to achieve 1 LSB for reciprocal */
        E=0x40000000; AE_MULSFP32X2RAS(E,Y,Z); AE_MULAFP32X2RAS(Z,Z,AE_ADD32(E,E));
        E=0x40000000; AE_MULSFP32X2RAS(E,Y,Z); AE_MULAFP32X2RAS(Z,Z,AE_ADD32(E,E));
        E=0x40000000; AE_MULSFP32X2RAS(E,Y,Z); AE_MULAFP32X2RAS(Z,Z,AE_ADD32(E,E));
        E=0x40000000; AE_MULSFP32X2RAS(E,Y,Z); AE_MULAFP32X2RAS(Z,Z,AE_ADD32(E,E));
        /* */
        Z=AE_MULFP32X2RAS(X,Z);
        AE_MOVSARA7X2(syh-sx+1, syl-sx+1);
        Z=AE_SLAS32S(Z);
        AE_S32X2_IP(Z,pYwr,sizeof(ae_int32x2));
   }
}
#endif
