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
#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#include "common.h"
#include "polypow2q23_tbl.h"

/*-------------------------------------------------------------------------
  Antilogarithm
  These routines calculate antilogarithm (base2, natural and base10). 32 
  and 24-bit fixed-point functions accept inputs in Q6.25 and form outputs 
  in Q16.15 format and return 0x7FFFFFFF in case of overflow and 0 in case 
  of underflow. 16-bit fixed-point functions accept inputs in Q3.12 and 
  form outputs in Q8.7 format and return 0x7FFF in case of overflow and 
  0 in case of underflow.

  Precision:
  16x16  16-bit inputs, 16-bit outputs. Accuracy: 2 LSB
  32x32  32-bit inputs, 32-bit outputs. Accuracy: 8*e-6*y+1LSB
  24x24  24-bit inputs, 24-bit outputs. Accuracy: 8*e-6*y+1LSB
  f      floating point: Accuracy: 2 ULP
  NOTE:
  1.  Although 32 and 24 bit functions provide the similar accuracy, 32-bit
      functions have better input/output resolution (dynamic range).
  2.  Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly.

  Input:
  x[N]  input data,Q6.25 (for 32 and 24-bit functions), Q3.12 (for 16-bit 
        functions) or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q16.15 (for 32 and 24-bit functions), Q8.7 (for 16-bit 
        functions) or floating point  

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  fixed point functions return result, Q16.15 (for 32 and 24-bit functions), 
  Q8.7 (for 16-bit functions) 

-------------------------------------------------------------------------*/

void vec_antilogn_16x16 (int16_t * restrict y, const int16_t * restrict x, int N)
{
    ae_int16x4 x0,x1,x2,z0;
    ae_int32x2 X0,X1,E0,E1,Y0,Y1;
    ae_f32x2 f0,f1;
    const ae_int16x4* restrict px;
          ae_int16x4* restrict pz;
    const ae_int32* restrict ptbl=(const ae_int32*)polypow2q23;
    ae_valign ax,az;
    int n;
    NASSERT(x);
    NASSERT(y);
    if (N<=0) return;
    px=(const ae_int16x4*)(x);
    pz=(      ae_int16x4*)(y);
    ax = AE_LA64_PP(px);
    az = AE_ZALIGN64();

    for (n=0; n<(N&~3); n+=4)
    {
        /* load, multiply by log2(e) and convert to Q19, take exponent */
        AE_LA16X4_IP(x0,ax,px);
        X0=AE_CVT32X2F16_10(x0);
        X1=AE_CVT32X2F16_32(x0);

        X0=AE_SRAI32(X0,8);
        X1=AE_SRAI32(X1,8);
        X0=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),6051102);
        X1=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X1),6051102);
        E0=AE_ADD32(AE_SRAI32(X0,19),1);
        E1=AE_ADD32(AE_SRAI32(X1,19),1);
        X0=AE_SUB32(AE_SRLI32(AE_SLAI32(X0,13),9),0x800000);
        X1=AE_SUB32(AE_SRLI32(AE_SLAI32(X1,13),9),0x800000);

        /* compute polyniomial from mantissa */
        Y0=Y1=AE_L32_I(ptbl,0);
        f0=f1=AE_L32_I(ptbl,4);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=f1;
        f0=f1=AE_L32_I(ptbl,8);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=f1;
        f0=f1=AE_L32_I(ptbl,12);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=f1;
        f0=f1=AE_L32_I(ptbl,16);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=f1;
        /* apply exponent part */
        Y0=AE_SEL32_HL(AE_SLAA32S(Y0,AE_MOVAD32_H(E0)),AE_SLAA32S(Y0,AE_MOVAD32_L(E0)));
        Y1=AE_SEL32_HL(AE_SLAA32S(Y1,AE_MOVAD32_H(E1)),AE_SLAA32S(Y1,AE_MOVAD32_L(E1)));
        z0=AE_TRUNC16X4F32(Y1,Y0);
        AE_SA16X4_IP(z0,az,pz);
    }
    AE_SA64POS_FP(az, pz);
    N&=3;
    if(N)
    {
        ae_int16 *pz_=(ae_int16 *)pz;
        AE_LA16X4_IP(x0,ax,px);
        X0=AE_CVT32X2F16_10(x0);
        X1=AE_CVT32X2F16_32(x0);
        X0=AE_SRAI32(X0,8);
        X1=AE_SRAI32(X1,8);
        X0=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),6051102);
        X1=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X1),6051102);
        E0=AE_ADD32(AE_SRAI32(X0,19),1);
        E1=AE_ADD32(AE_SRAI32(X1,19),1);
        X0=AE_SUB32(AE_SRLI32(AE_SLAI32(X0,13),9),0x800000);
        X1=AE_SUB32(AE_SRLI32(AE_SLAI32(X1,13),9),0x800000);

        Y0=Y1=AE_L32_I(ptbl,0);
        f0=f1=AE_L32_I(ptbl,4);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=f1;
        f0=f1=AE_L32_I(ptbl,8);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=f1;
        f0=f1=AE_L32_I(ptbl,12);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=f1;
        f0=f1=AE_L32_I(ptbl,16);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); Y1=f1;

        Y0=AE_SEL32_HL(AE_SLAA32S(Y0,AE_MOVAD32_H(E0)),AE_SLAA32S(Y0,AE_MOVAD32_L(E0)));
        Y1=AE_SEL32_HL(AE_SLAA32S(Y1,AE_MOVAD32_H(E1)),AE_SLAA32S(Y1,AE_MOVAD32_L(E1)));
        z0=AE_TRUNC16X4F32(Y1,Y0);
        x0 = AE_SEL16_6543(z0, z0); 
        x1 = AE_SEL16_5432(z0, z0);
        x2 = AE_SEL16_5432(x0, x0);
        AE_S16_0_IP(x0, pz_, 2);
        if(N>1) AE_S16_0_IP(x1, pz_, 2);
        if(N>2) AE_S16_0_IP(x2, pz_, 2);
    }
}
