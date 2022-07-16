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
#include "common.h"

/*===========================================================================
  Vector matematics:
  vec_divide           Division of Q31/Q15 Numbers
===========================================================================*/
/*-------------------------------------------------------------------------
  Division 
Fixed point routines perform pair-wise division of vectors written in Q31 or 
Q15 format. They return the fractional and exponential portion of the division 
result. Since the division may generate result greater than 1, it returns 
fractional portion frac in Q(31-exp) or or Q(15-exp) format and exponent 
exp so true division result in the Q0.31 may be found by shifting 
fractional part left by exponent value.
For division to 0, the result is not defined 

For fixed point finctions, mantissa accuracy is 2 LSB, so relative accuracy is:
vec_divide16x16, scl_divide16x16                   1.2e-4 
vec_divide24x24, scl_divide32x32, scl_divide24x24  4.8e-7 
vec_divide32x32                                    1.8e-9

Floating point routines operate with standard floating point numbers. Functions 
return +/-infinity in case of overflow and provide accuracy of 2 ULP.

Two versions of routines are available: regular versions (vec_divide32x32, 
vec_divide24x24, vec_divide16x16) work with arbitrary arguments, faster 
versions (vec_divide32x3_fast, vec_divide24x24_fast, vec_divide16x16_fast) 
apply some restrictions.

  Precision: 
  32x32  32-bit inputs, 32-bit output. 
  24x24  24-bit inputs, 24-bit output. 
  16x16  16-bit inputs, 16-bit output. 
  f      floating point

  Input:
  x[N]    nominator,Q31, Q15, floating point
  y[N]    denominator,Q31, Q15, floating point
  N       length of vectors
  Output:
  frac[N] fractional parts of result, Q(31-exp) or Q(15-exp) (for fixed 
          point functions)
  exp[N]  exponents of result (for fixed point functions) 
  z[N]    result (for floating point function)

  Restriction:
  For regular versions (vec_divide32x32, vec_divide24x24, 
  vec_divide16x16, vec_dividef) :
  x,y,frac,exp should not overlap

  For faster versions (vec_divide32x3_fast, vec_divide24x24_fast, 
  vec_divide16x16_fast) :
  x,y,frac,exp should not overlap
  x, y, frac to be aligned by 8-byte boundary, N - multiple of 4.

  Scalar versions:
  ----------------
  Return packed value (for fixed point functions): 
  scl_divide24x24(),scl_divide32x32():
  bits 23:0 fractional part
  bits 31:24 exponent
  scl_divide16x16():
  bits 15:0 fractional part
  bits 31:16 exponent
-------------------------------------------------------------------------*/
void vec_divide24x24_fast 
(
  int32_t * restrict        frac,
  int16_t *                   exp,
  const int32_t * restrict  x,
  const int32_t * restrict  y,
  int                       M
)
#if 0
{
    ae_int32x2 X,Y,Z,E;
    int n,N=M;
    const ae_int32x2  * restrict px;
    const ae_int32x2  * restrict py;
    const ae_int32x2 * restrict pfWr;

    if(N<=0) return;
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT_ALIGN8(frac);
    NASSERT(N%4==0);
    /* take exponent and normalize inputs. Y is saved to the scratch */
    px=(const ae_int32x2*)x;
    py=(const ae_int32x2*)y;
    pfWr = (      ae_int32x2*)frac;
    __Pragma("loop_count min=1,factor=2")
    for (n=0; n<(N>>1); n++)
    {
        ae_int32x2 Xl,Xh,Yl,Yh;
        ae_f32x2 t;
        ae_f24x2 tmp;
        xtbool2 sy;
        ae_int32x2 _0x40000000=AE_MOVDA32(0x40000000);
        int expx,expy;
        /* normalization */
        AE_L32X2F24_IP(tmp,px,sizeof(X)); X=tmp;
        AE_L32X2F24_IP(tmp,py,sizeof(Y)); Y=tmp;

        expx = AE_NSAZ32_L(X);
        expy = AE_NSAZ32_L(Y);
        Xl=AE_SLAA32S(X,expx);
        Yl=AE_SLAA32S(Y,expy);
        exp[1]=(int16_t)(expy-expx+1);
        X=AE_SEL32_LH(X,X);
        Y=AE_SEL32_LH(Y,Y);
        expx = AE_NSAZ32_L(X);
        expy = AE_NSAZ32_L(Y);
        Xh=AE_SLAA32S(X,expx);
        Yh=AE_SLAA32S(Y,expy);
        exp[0]=(int16_t)(expy-expx+1);
        X=AE_SEL32_LL(Xh,Xl);
        Y=AE_SEL32_LL(Yh,Yl);
        exp+=2;

        sy=AE_LT32(Y,AE_ZERO32());
        Y=AE_INT32X2_ABS32S(Y);
        /* first approximation */
        Z=AE_SUB32(AE_MOVDA32((int32_t)0xBAEC0000),Y); 
            /* 4 iterations to achieve 1 LSB accuracy in mantissa */
        t=_0x40000000; AE_MULSFP32X2RAS(t,Y,Z); E=t;
        E=AE_ADD32(E,E);
        t=Z; AE_MULAFP32X2RAS(t,Z,E); Z=t;
        t=_0x40000000; AE_MULSFP32X2RAS(t,Y,Z); E=t;
        E=AE_ADD32(E,E);
        t=Z; AE_MULAFP32X2RAS(t,Z,E); Z=t;
        t=_0x40000000; AE_MULSFP32X2RAS(t,Y,Z); E=t;
        E=AE_ADD32(E,E);
        t=Z; AE_MULAFP32X2RAS(t,Z,E); Z=t;
        /* restore original sign */
        Y=AE_INT32X2_NEG32S(Z);
        AE_MOVT32X2(Z,Y,sy);
        /* multiply by X */
        Z=AE_MULFP32X2RAS(X,Z);
        AE_S32X2_IP(Z,pfWr,sizeof(X));
    }
}
#else
{
    ae_int32x2 X,Y,E;
    int n,N=M;
    const ae_int32x2  * restrict px;
    const ae_int32x2  * restrict py;
    //const ae_int32x2 * restrict pfWr; __MNSD__
    ae_int32x2 * restrict pfWr;

    if(N<=0) return;
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT_ALIGN8(frac);
    NASSERT(N%4==0);
    /* take exponent and normalize inputs. Y is saved to the scratch */
    px=(const ae_int32x2*)x;
    py=(const ae_int32x2*)y;
    pfWr = (ae_int32x2*)frac;
    __Pragma("loop_count min=1,factor=2")
    for (n=0; n<(N>>1); n++)
    {
        ae_int32x2 Xl,Xh,Yl,Yh,XX,YY;
        ae_int32x2 _0x400000=AE_MOVDA32(0x400000);
        ae_f24x2 tmp;
        ae_f32x2 f;
        xtbool2 sy;
        int expx,expy;
        /* normalization */
        AE_L32X2F24_IP(tmp, castxcc(ae_f24x2, px),sizeof(XX)); XX=tmp;
        AE_L32X2F24_IP(tmp, castxcc(ae_f24x2, py), sizeof(YY)); YY = tmp;

        expx = AE_NSAZ32_L(XX);
        expy = AE_NSAZ32_L(YY);
        Xl=AE_SLAA32S(XX,expx);
        Yl=AE_SLAA32S(YY,expy);
        exp[1]=(int16_t)(expy-expx+1);
        XX=AE_SEL32_LH(XX,XX);
        YY=AE_SEL32_LH(YY,YY);
        expx = AE_NSAZ32_L(XX);
        expy = AE_NSAZ32_L(YY);
        Xh=AE_SLAA32S(XX,expx);
        Yh=AE_SLAA32S(YY,expy);
        exp[0]=(int16_t)(expy-expx+1);
        XX=AE_SEL32_LL(Xh,Xl);
        YY=AE_SEL32_LL(Yh,Yl);
        exp+=2;

        sy=AE_LT32(YY,AE_ZERO32());
        YY=AE_INT32X2_ABS32S(YY);
        X=AE_SRAI32(YY,8);
        /* first approximation */
        Y=AE_SUB32(AE_MOVDA32((int32_t)0x00BAEC00),X); /* first approximation */
            /* 3 iterations to achieve 1 LSB accuracy in mantissa */
        f=AE_MOVF32X2_FROMINT32X2(_0x400000); AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(Y)); E=AE_MOVINT32X2_FROMF32X2(f);
        E=AE_SLLI32(E,1);
        f=AE_MOVF32X2_FROMINT32X2(Y); AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(E),AE_MOVF24X2_FROMINT32X2(Y)); Y=AE_MOVINT32X2_FROMF32X2(f);
        f=AE_MOVF32X2_FROMINT32X2(_0x400000); AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(Y)); E=AE_MOVINT32X2_FROMF32X2(f);
        E=AE_SLLI32(E,1);
        f=AE_MOVF32X2_FROMINT32X2(Y); AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(E),AE_MOVF24X2_FROMINT32X2(Y)); Y=AE_MOVINT32X2_FROMF32X2(f);
        f=AE_MOVF32X2_FROMINT32X2(_0x400000); AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(Y)); E=AE_MOVINT32X2_FROMF32X2(f);
        E=AE_SLLI32(E,1);
        f=AE_MOVF32X2_FROMINT32X2(Y); AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(E),AE_MOVF24X2_FROMINT32X2(Y)); Y=AE_MOVINT32X2_FROMF32X2(f);
        /* restore original sign */
        X=AE_NEG32(Y);
        AE_MOVT32X2(Y,X,sy);
        Y=AE_SLLI32S(Y,8);
        /* multiply by X */
        AE_S32X2RA64S_IP(AE_MULF32R_HH(XX, Y),AE_MULF32R_LL(XX, Y),pfWr);
    }
} /* vec_divide24x24_fast() */
#endif
