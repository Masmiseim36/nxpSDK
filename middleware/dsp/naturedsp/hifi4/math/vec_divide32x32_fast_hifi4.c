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
#include "NatureDSP_Signal_math.h"
#include "common.h"

/*===========================================================================
  Vector matematics:
  vec_divide           Division of Q31/Q15 Numbers
===========================================================================*/

/*-------------------------------------------------------------------------
  Division
  These routines perform pair-wise division of vectors written in Q63, Q31 or 
  Q15 format. They return the fractional and exponential portion of the division 
  result. Since the division may generate result greater than 1, it returns 
  fractional portion frac in Q(63-exp), Q(31-exp) or Q(15-exp) format and 
  exponent exp so true division result in the Q0.31 may be found by shifting 
  fractional part left by exponent value.
  Additional routine makes integer division of 64-bit number to 32-bit 
  denominator forming 32-bit result. If result is overflown, 0x7fffffff 
  or 0x80000000 is returned depending on the signs of inputs.
  For division to 0, the result is not defined.

  Two versions of routines are available: regular versions (vec_divide64x32i,
  vec_divide64x64, vec_divide32x32, vec_divide16x16) work 
  with arbitrary arguments, faster versions (vec_divide32x32_fast, 
  vec_divide16x16_fast) apply some restrictions.

  Accuracy is measured as accuracy of fractional part (mantissa):
  vec_divide64x32i, scl_divide64x32                      :  1 LSB   
  vec_divide64x64                                        :  2 LSB 
  vec_divide32x32, vec_divide32x32_fast                  :  2 LSB (1.8e-9) 
  scl_divide32x32                                        :  2 LSB (4.8e-7) 
  vec_divide16x16, scl_divide16x16, vec_divide16x16_fast :  2 LSB (1.2e-4)

  Precision: 
  64x32i integer division, 64-bit nominator, 32-bit denominator, 32-bit output. 
  64x64  fractional division, 64-bit inputs, 64-bit output. 
  32x32  fractional division, 32-bit inputs, 32-bit output. 
  16x16  fractional division, 16-bit inputs, 16-bit output. 

  Input:
  x[N]    nominator, 64-bit integer, Q63, Q31 or Q15
  y[N]    denominator, 32-bit integer, Q63, Q31 or Q15
  N       length of vectors
  Output:
  frac[N] fractional parts of result, Q(63-exp), Q(31-exp) or Q(15-exp)
  exp[N]  exponents of result 

  Restriction:
  For regular versions (vec_divide64x32i, vec_divide64x64, vec_divide32x32,
  vec_divide16x16) :
  x,y,frac,exp should not overlap

  For faster versions (vec_divide32x32_fast, vec_divide16x16_fast) :
  x,y,frac,exp  should not overlap
  x,y,frac      to be aligned by 8-byte boundary, N - multiple of 4.

  Scalar versions:
  ----------------
  scl_divide64x32(): integer remainder
  Return packed value: 
  scl_divide64x64():
  bits 55:0 fractional part
  bits 63:56 exponent
  scl_divide32x32():
  bits 23:0 fractional part
  bits 31:24 exponent
  scl_divide16x16():
  bits 15:0 fractional part
  bits 31:16 exponent
-------------------------------------------------------------------------*/
#if 0
void vec_divide32x32_fast 
(
  int32_t * restrict        frac,
  int16_t * restrict        exp,
  const int32_t * restrict  x,
  const int32_t * restrict  y,
  int                       N
)
{
    ae_int32x2 ensa;
    const ae_int16x4* restrict pexp;
    const ae_int32x2* restrict px;
    const ae_int32x2* restrict py;
    const ae_int32x2* restrict pfrac;
          ae_int32x2* restrict pfracWr;
    xtbool2     sgn,inf;
    ae_int32x2  X,F,T,minus_one,one;
    ae_f32x2   Y,E;
    int xnsah,xnsal;
    int n;
    int __0xBFFFFFFF=0xBFFFFFFF,__0x40000000=0x40000000;

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT_ALIGN8(frac);
    NASSERT_ALIGN8(exp);
    NASSERT(N%4==0);
    if(N<=0) return;

    // first, take 1/y
    py     =(const ae_int32x2*)y;
    pfracWr=(      ae_int32x2*)frac;
    __Pragma("loop_count min=1, factor=2")
    __Pragma("loop_count min=1")
    for (n=0; n<N; n+=2)
    {
        ae_int32x2 _0,_0x40000000,_0xBFFFFFFF;
        _0=AE_ZERO32();
        //XT_MOVEQZ(__0x40000000,__0x40000000,__0x40000000);
        //XT_MOVEQZ(__0xBFFFFFFF,__0xBFFFFFFF,__0xBFFFFFFF);
        _0x40000000=AE_MOVDA32X2(__0x40000000,__0x40000000);
        _0xBFFFFFFF=AE_MOVDA32X2(__0xBFFFFFFF,__0xBFFFFFFF);
        //_0=AE_XOR32(_0x40000000,_0x40000000);

        AE_L32X2_IP(X, py, 8);
        sgn = AE_LT32(X, _0);
        inf = AE_EQ32(X, _0);
        X=AE_ABS32S(X);
        xnsal = AE_NSAZ32_L(X);
        xnsah = AE_NSAZ32_L(AE_SEL32_HH(X,X));
        AE_MOVSARA7X2(xnsah, xnsal);
        X = AE_SLAS32S(X);
        // first approximation 3-2x,Q30
        Y = AE_SUB32(_0xBFFFFFFF,X);
        /*
            yi=3-2*x;
            for iter=1:4
            e=1-yi.*x;
            yi=yi+e.*yi;
            end
        */
        // 1-st iteration
        E = _0x40000000;
        AE_MULSFP32X2RAS(E,X,Y);
        E = AE_SLAI32(E,1);
        AE_MULAFP32X2RAS(Y,Y,E);
        // 2-st iteration
        E = _0x40000000;
        AE_MULSFP32X2RAS(E,X,Y);
        E = AE_SLAI32(E,1);
        AE_MULAFP32X2RAS(Y,Y,E);
        // 3-st iteration
        E = _0x40000000;
        AE_MULSFP32X2RAS(E,X,Y);
        E = AE_SLAI32(E,1);
        AE_MULAFP32X2RAS(Y,Y,E);
        // 4-st iteration
        E = _0x40000000;
        AE_MULSFP32X2RAS(E,X,Y);
        E = AE_SLAI32(E,1);
        AE_MULAFP32X2RAS(Y,Y,E);
        // apply sign and move right values if input is zero
        X=AE_NEG32(Y);
        T = Y;

        AE_MOVT32X2(T, X, sgn);
        Y = T;
        AE_S32X2_IP(Y, pfracWr, 8);
        xnsah=xnsah+1; exp[n+0]=xnsah;
        xnsal=xnsal+1; exp[n+1]=xnsal;
    }
    __Pragma("no_reorder")

    px   =(const ae_int32x2*)x;
    py   =(const ae_int32x2*)y;
    pfrac=(const ae_int32x2*)frac;
    pfracWr=(    ae_int32x2*)frac;
    pexp =(const ae_int16x4*)exp;
    // multiply to normalized x, renormalize and process exceptional cases (y==0)
    __Pragma("loop_count min=1")
    for (n=0; n<(N>>2); n++)
    {
        xtbool2 inf0,inf1;
        int qnsah0,qnsal0;
        int qnsah1,qnsal1;
        ae_int32x2 xnsa0,xnsa1;
        ae_int64 QH,QL;
        ae_int16x4 xx;

        minus_one=AE_MOVDA32X2(0x80000000,0x80000000);
        one=AE_MOVDA32X2(0x7fffffff,0x7fffffff);
        AE_L32X2_IP(T, py,  8);
        AE_L32X2_IP(X, px,  8);
        AE_L32X2_IP(F,pfrac, 8);

        inf0= AE_EQ32(T, AE_ZERO32());
        sgn = AE_LT32(X, AE_ZERO32());
        AE_MOVT32X2(one,minus_one,sgn);

        QH=AE_MULF32S_HH(X,F);
        QL=AE_MULF32S_LL(X,F);
        qnsah0=AE_NSA64(QH);
        qnsal0=AE_NSA64(QL);
        QH=AE_SLAA64(QH,qnsah0);
        QL=AE_SLAA64(QL,qnsal0);
        T=AE_ROUND32X2F64SASYM(QH,QL);
        xnsa0=AE_MOVDA32X2(qnsah0,qnsal0);
        AE_MOVT32X2(T,one,inf0);
        AE_S32X2_IP(T,pfracWr,8);

        one=AE_MOVDA32X2(0x7fffffff,0x7fffffff);
        AE_L32X2_IP(T, py,  8);
        AE_L32X2_IP(X, px,  8);
        AE_L32X2_IP(F,pfrac, 8);

        inf1= AE_EQ32(T, AE_ZERO32());
        sgn = AE_LT32(X, AE_ZERO32());
        AE_MOVT32X2(one,minus_one,sgn);

        QH=AE_MULF32S_HH(X,F);
        QL=AE_MULF32S_LL(X,F);
        qnsah1=AE_NSA64(QH);
        qnsal1=AE_NSA64(QL);
        QH=AE_SLAA64(QH,qnsah1);
        QL=AE_SLAA64(QL,qnsal1);
        T=AE_ROUND32X2F64SASYM(QH,QL);
        xnsa1=AE_MOVDA32X2(qnsah1,qnsal1);
        AE_MOVT32X2(T,one,inf1);
        AE_S32X2_IP(T,pfracWr,8);

        // combine exponents
        AE_L16X4_IP(xx,pexp,8);
        ensa=AE_SEXT32X2D16_32(xx);
        xnsa0=AE_SUB32(ensa,xnsa0);
        ensa=AE_SEXT32X2D16_10(xx);
        xnsa1=AE_SUB32(ensa,xnsa1);
        AE_MOVT32X2(xnsa0,AE_MOVDA32X2(0x20,0x20),inf0);
        AE_MOVT32X2(xnsa1,AE_MOVDA32X2(0x20,0x20),inf1);
        xx=AE_CVT16X4(xnsa0,xnsa1);
        AE_S16X4_IP(xx,castxcc(ae_int16x4,exp),8);
    }
}
#else
void vec_divide32x32_fast 
(
  int32_t * restrict        frac,
  int16_t *                   exp,
  const int32_t * restrict  x,
  const int32_t * restrict  y,
  int                       M
)
{
    ae_int32x2 X,Y,Z,E;
    int n,N=M;
    const ae_int32x2  * restrict px;
    const ae_int32x2  * restrict py;
          ae_int32x2 * restrict pfWr;

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
        xtbool2 sy;
        ae_int32x2 _0x40000000=AE_MOVDA32(0x40000000);
        int expx,expy;
        /* normalization */
        AE_L32X2_IP(X,px,sizeof(X));
        AE_L32X2_IP(Y,py,sizeof(Y));
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
#endif
