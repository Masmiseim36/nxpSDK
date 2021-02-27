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
  Sine/Cosine 
  Fixed-point functions calculate sin(pi*x) or cos(pi*x) for numbers written 
  in Q31 or Q15 format. Return results in the same format. 
  Floating point functions compute sin(x) or cos(x)
  Two versions of functions available: regular version (vec_sine16x16, 
  vec_cosine16x16, vec_sine24x24, vec_cosine24x24, vec_sine32x32, 
  vec_cosine32x32, , vec_sinef, vec_cosinef) with arbitrary arguments and 
  faster version (vec_sine24x24_fast, vec_cosine24x24_fast, 
  vec_sine32x32_fast, vec_cosine32x32_fast) that apply some restrictions.
  NOTE:
  1.  Scalar floating point functions are compatible with standard ANSI C
      routines and set errno and exception flags accordingly
  2.  Floating point functions limit the range of allowable input values:
      [-102940.0, 102940.0] Whenever the input value does not belong to this
      range, the result is set to NaN.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: 2 LSB  
  24x24  24-bit inputs, 24-bit output. Accuracy: 74000(3.4e-5)
  32x32  32-bit inputs, 32-bit output. Accuracy: 1700 (7.9e-7)
  f      floating point. Accuracy 2 ULP

  Input:
  x[N]  input data, Q15, Q31 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31,Q15 or floating point

  Restriction:
  Regular versions (vec_sine24x24, vec_cosine24x24, vec_sine32x32, 
  vec_cosine32x32, vec_sinef, vec_cosinef):
  x,y - should not overlap

  Faster versions (vec_sine24x24_fast, vec_cosine24x24_fast, 
  vec_sine32x32_fast, vec_cosine32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result in Q31,Q15 or floating point
-------------------------------------------------------------------------*/
void vec_cosine16x16   (int16_t  * restrict y, 
                const int16_t  * restrict x, int N)
{
    xtbool2 big0,big1;
    ae_int16x4 x0,x1,x2,z0;
    ae_int32x2 X0,X1,Y0,Y1,_0x800000;
    ae_f32x2   f0,f1;
    ae_f24x2   X20,X21;
    const ae_int16x4 * restrict px;
          ae_int16x4 * restrict pz;
    ae_valign ax,az;
    static const int32_t costblq23[]={-161131,2120021,-1958992};
    const ae_int32 * restrict pcos; 
    int n;
    NASSERT(x);
    NASSERT(y);
    if (N<=0) return ;
    pcos = (const ae_int32 *)costblq23;
    _0x800000=0x800000;
    px=(const ae_int16x4 *)x;
    pz=(      ae_int16x4 *)y;
    ax = AE_LA64_PP(px);
    az = AE_ZALIGN64();
    for (n=0; n<(N&~3); n+=4)
    {
        /* take input, convert to Q23 and make reduction */
        AE_LA16X4_IP(x0,ax,px);
        X0=AE_CVT32X2F16_10(x0);
        X1=AE_CVT32X2F16_32(x0);
        X0=AE_SRAI32(X0,7);
        X1=AE_SRAI32(X1,7);
        X0=AE_ABS32(X0);
        X1=AE_ABS32(X1);
        big0=AE_LT32(_0x800000,X0);
        big1=AE_LT32(_0x800000,X1);
        X0=AE_SUB24S(_0x800000,AE_ABS32(AE_SUB32(X0,_0x800000)));
        X1=AE_SUB24S(_0x800000,AE_ABS32(AE_SUB32(X1,_0x800000)));
        /* compute polynomial */
        X20=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),AE_MOVF24X2_FROMINT32X2(X0)));
        X21=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X1),AE_MOVF24X2_FROMINT32X2(X1)));
        Y0=Y1=AE_L32_I(pcos,0);
        f0=f1=AE_L32_I(pcos,4);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(pcos,8);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=AE_SUB32(_0x800000,X20);
        f1=AE_SUB32(_0x800000,X21);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        /* apply sign */
        AE_MOVT32X2(Y0,AE_NEG32(Y0),big0);
        AE_MOVT32X2(Y1,AE_NEG32(Y1),big1);
        /* convert result to Q15 */
        Y0=AE_SLAI32S(Y0,8);
        Y1=AE_SLAI32S(Y1,8);
        z0=AE_ROUND16X4F32SASYM(Y1,Y0);
        AE_SA16X4_IP(z0,az,pz);
    }
    AE_SA64POS_FP(az, pz);
    N&=3;
    if(N)
    {
        ae_int16 *pz_=(ae_int16 *)pz;
        /* take input, convert to Q23 and make reduction */
        AE_LA16X4_IP(x0,ax,px);
        X0=AE_CVT32X2F16_10(x0);
        X1=AE_CVT32X2F16_32(x0);
        X0=AE_SRAI32(X0,7);
        X1=AE_SRAI32(X1,7);
        X0=AE_ABS32(X0);
        X1=AE_ABS32(X1);
        big0=AE_LT32(_0x800000,X0);
        big1=AE_LT32(_0x800000,X1);
        X0=AE_SUB24S(_0x800000,AE_ABS32(AE_SUB32(X0,_0x800000)));
        X1=AE_SUB24S(_0x800000,AE_ABS32(AE_SUB32(X1,_0x800000)));
        /* compute polynomial */
        X20=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),AE_MOVF24X2_FROMINT32X2(X0)));
        X21=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X1),AE_MOVF24X2_FROMINT32X2(X1)));
        Y0=Y1=AE_L32_I(pcos,0);
        f0=f1=AE_L32_I(pcos,4);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(pcos,8);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=AE_SUB32(_0x800000,X20);
        f1=AE_SUB32(_0x800000,X21);
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        /* apply sign */
        AE_MOVT32X2(Y0,AE_NEG32(Y0),big0);
        AE_MOVT32X2(Y1,AE_NEG32(Y1),big1);
        /* convert result to Q15 */
        Y0=AE_SLAI32S(Y0,8);
        Y1=AE_SLAI32S(Y1,8);
        z0=AE_ROUND16X4F32SASYM(Y1,Y0);
        x0 = AE_SEL16_6543(z0, z0); 
        x1 = AE_SEL16_5432(z0, z0);
        x2 = AE_SEL16_5432(x0, x0);
        AE_S16_0_IP(x0, pz_, 2);
        if(N>1) AE_S16_0_IP(x1, pz_, 2);
        if(N>2) AE_S16_0_IP(x2, pz_, 2);
    }
}
