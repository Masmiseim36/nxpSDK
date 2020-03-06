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
  NatureDSP Signal Processing Library. Vector matematics
    Hyperbolic Tangent
    Optimized code for HiFi4
  IntegrIT, 2006-2018
*/

#include "NatureDSP_types.h"
#include "NatureDSP_Signal_math.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Hyperbolic Tangent
  The functions compute the hyperbolic tangent of input argument. 32-bit
  fixed-point functions accept inputs in Q6.25 and form outputs in Q16.15
  format.

  Precision:
  32x32  32-bit inputs, 32-bit output. Accuracy: 2 LSB.
  f      floating point input, floating point output, Accuracy: 2 ULP
  Input:
  x[N]   input data, Q6.25 or floating point  
  N      length of vectors
  Output:
  y[N]   result, Q16.15 or floating point

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  return result, Q16.15 or floating point
-------------------------------------------------------------------------*/
void vec_tanh32x32(int32_t* restrict y, const int32_t* restrict x, int N)
{
    /*
    Reference Matlab code:
        function y=tanh_32x32(x)
        % convert Q25 -> Q23 and scale by ln(2)
        x=round(pow2(double(x)*774541002*2,-31));
        s=x<0;
        x=abs(x);
        % compute 2^-x
        n=bitshift(x,-23);
        mantQ23=bitand(x,8388607);
        % polynomial for 2^-x, for x=0...1, coeffients in Q23
        polypow2=[57364 -446161 2008107 -5813551 8388608];
        y=polypow2(1);
        y=round(pow2(y.*mantQ23,-23))+polypow2(2);
        y=round(pow2(y.*mantQ23,-23))+polypow2(3);
        y=round(pow2(y.*mantQ23,-23))+polypow2(4);
        y=round(pow2(y.*mantQ23,-23))+polypow2(5);
        x=bitshift(y,-n);

        % iterations to compute 1./(1+x) in Q23
        y=8053064-bitshift(x,-1);  % first approximation 0.96-x/2
        d=8388608-y-round(pow2(y.*x,-23));
        y=y+round(pow2(y.*d,-23));
        d=8388608-y-round(pow2(y.*x,-23));
        y=y+round(pow2(y.*d,-23));
        % scale by (1-x)
        y=round(pow2(y.*(8388608-x),-23));
        % scale to Q15 with rounding
        y=bitshift(y+128,-8);

        % apply sign
        y(s)=-y(s);
    */
    static const int32_t polypow2[] = { 57364, -446161, 2008107, -5813551, 8388608 };
    int n;
    ae_int32x2 X, X0, X1, E, Y, Z, D;
    ae_f32x2 t;
    xtbool2 sign;
    const ae_int32x2 * restrict pX = (const ae_int32x2 *)x;
    const ae_int32x2 * restrict pX1 = (const ae_int32x2 *)x;
          ae_int32x2 * restrict pY = (      ae_int32x2 *)y;
    ae_valign aX, aX1, aY;

    NASSERT(x);
    NASSERT(y);
    if (N <= 0) return;

    aY = AE_ZALIGN64();
    aX = AE_LA64_PP(pX);
    aX1 = AE_LA64_PP(pX1);
    for (n = 0; n < (N >> 1); n++)
    {
        AE_LA32X2_IP(X, aX, pX);

        Z = AE_MULFP32X2RAS(X, AE_MOVDA32X2(1549082005, 1549082005));
        X = AE_ABS32S(Z);

        E = AE_SRAI32(X, 23);
        X = AE_AND32(X, AE_MOVDA32X2(0x007fffff, 0x007fffff));

        Y = AE_L32_I((const ae_int32 *)polypow2, 4 * 0);
        t = AE_L32_I((const ae_int32 *)polypow2, 4 * 1); AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X), AE_MOVF24X2_FROMINT32X2(Y)); Y = t;
        t = AE_L32_I((const ae_int32 *)polypow2, 4 * 2); AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X), AE_MOVF24X2_FROMINT32X2(Y)); Y = t;
        t = AE_L32_I((const ae_int32 *)polypow2, 4 * 3); AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X), AE_MOVF24X2_FROMINT32X2(Y)); Y = t;
        t = AE_L32_I((const ae_int32 *)polypow2, 4 * 4); AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X), AE_MOVF24X2_FROMINT32X2(Y)); Y = t;
        X0 = AE_SRAA32(Y, AE_MOVAD32_H(E));
        X1 = AE_SRAA32(Y, AE_MOVAD32_L(E));
        X = AE_SEL32_HL(X0, X1);

        Z = AE_SUB32(8053064, AE_SRAI32(X, 1));
        t = Z; AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(Z), AE_MOVF24X2_FROMINT32X2(X));
        D = AE_SUB32(8388608, t);
        t = Z; AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(Z), AE_MOVF24X2_FROMINT32X2(D)); Z = (t);
        t = Z; AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(Z), AE_MOVF24X2_FROMINT32X2(X));
        D = AE_SUB32(8388608, t);
        t = Z; AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(Z), AE_MOVF24X2_FROMINT32X2(D)); Z = (t);

        Y = AE_SUB32(8388608, X);
        Z = AE_MULFP32X2RAS(Z, Y);

        AE_LA32X2_IP(X, aX1, pX1);
        sign = AE_LT32(X, 0);
        X = AE_NEG32S(Z);
        AE_MOVT32X2(Z, X, sign);

        AE_SA32X2_IP(Z, aY, pY);
    }
    AE_SA64POS_FP(aY, pY);

    if (N & 1)
    {
        X = AE_L32_I((const ae_int32 *)pX, 0);
        sign = AE_LT32(X, 0);

        Z = AE_MULFP32X2RAS(X, AE_MOVDA32X2(1549082005, 1549082005));
        X = AE_ABS32S(Z);

        E = AE_SRAI32(X, 23);
        X = AE_AND32(X, AE_MOVDA32X2(0x007fffff, 0x007fffff));

        Y = AE_L32_I((const ae_int32 *)polypow2, 4 * 0);
        t = AE_L32_I((const ae_int32 *)polypow2, 4 * 1); AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X), AE_MOVF24X2_FROMINT32X2(Y)); Y = t;
        t = AE_L32_I((const ae_int32 *)polypow2, 4 * 2); AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X), AE_MOVF24X2_FROMINT32X2(Y)); Y = t;
        t = AE_L32_I((const ae_int32 *)polypow2, 4 * 3); AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X), AE_MOVF24X2_FROMINT32X2(Y)); Y = t;
        t = AE_L32_I((const ae_int32 *)polypow2, 4 * 4); AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(X), AE_MOVF24X2_FROMINT32X2(Y)); Y = t;
        X = AE_SRAA32(Y, AE_MOVAD32_H(E));

        Z = AE_SUB32(8053064, AE_SRAI32(X, 1));
        t = Z; AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(Z), AE_MOVF24X2_FROMINT32X2(X));
        D = AE_SUB32(8388608, t);
        t = Z; AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(Z), AE_MOVF24X2_FROMINT32X2(D)); Z = (t);
        t = Z; AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(Z), AE_MOVF24X2_FROMINT32X2(X));
        D = AE_SUB32(8388608, t);
        t = Z; AE_MULAFP24X2RA(t, AE_MOVF24X2_FROMINT32X2(Z), AE_MOVF24X2_FROMINT32X2(D)); Z = (t);

        Y = AE_SUB32(8388608, X);
        Z = AE_MULFP32X2RAS(Z, Y);

        X = AE_NEG32S(Z);
        AE_MOVT32X2(Z, X, sign);

        AE_S32_L_I(Z, (ae_int32 *)pY, 0);
    }
} /* vec_tanh32x32() */
