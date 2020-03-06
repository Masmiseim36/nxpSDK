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
void vec_divide32x32 
(
  int32_t * restrict        frac,
  int16_t * restrict        exp,
  const int32_t * restrict  x,
  const int32_t * restrict  y,
  int                       M
)
#if 0
{
    ae_int32x2 ensa,xnsa,fnsa;
    ae_int16x4 x0,x1;
    ae_valign afr,afw,ay;
    const ae_int16* restrict pexp;
    const ae_int32* restrict px;
    const ae_int32x2* restrict py;
    const ae_int32x2* restrict pfrac;
          ae_int32x2* restrict pfracWr;
    xtbool2     sgn,inf;
    ae_int32x2  X,XH,XL,F,FL,FH, T,minus_one,one;
    int xnsah,fnsah,xnsal,fnsal;

    px   =(const ae_int32*)x;
    py   =(const ae_int32x2*)y;
    pfrac=(const ae_int32x2*)frac;
    pfracWr=(    ae_int32x2*)frac;
    pexp =(const ae_int16*)exp;

    int n;
    // first, take 1/y
    vec_recip32x32(frac,exp,y,N);
    // multiply to normalized x, renormalize and process exceptional cases (y==0)
    afr = AE_LA64_PP(pfrac);
    ay  = AE_LA64_PP(py);
    afw = AE_ZALIGN64();
    for (n=0; n<(N&(~1)); n+=2)
    {
        minus_one=AE_MOVDA32X2(0x80000000,0x80000000);
              one=AE_MOVDA32X2(0x7fffffff,0x7fffffff);
        AE_LA32X2_IP(T, ay,py);
        AE_L32_IP(XH, px,    4);
        AE_L32_IP(XL, px,    4);
        AE_LA32X2_IP(F,afr,pfrac);
        FH=AE_SEL32_HH(F,F);
        FL=AE_SEL32_LL(F,F);
        X=AE_SEL32_HL(XH,XL);

        inf = AE_EQ32(T, AE_ZERO32());
        sgn = AE_LT32(X, AE_ZERO32());
        AE_MOVT32X2(one,minus_one,sgn);

        xnsah = AE_NSAZ32_L(XH);
        XH = AE_SLAA32S(XH,xnsah);
        FH = AE_MULFP32X2RAS(XH,FH);
        xnsal = AE_NSAZ32_L(XL);
        XL = AE_SLAA32S(XL,xnsal);
        FL = AE_MULFP32X2RAS(XL,FL);

        fnsah = AE_NSAZ32_L(FH);
        FH = AE_SLAA32S(FH,fnsah);
        fnsal = AE_NSAZ32_L(FL);
        FL = AE_SLAA32S(FL,fnsal);

        // combine exponents
        AE_L16_IP(x1,pexp,2);
        AE_L16_IP(x0,pexp,2);
        x0=AE_SEL16_4321(x1,x0);
        ensa=AE_SEXT32X2D16_32(x0);

        xnsa=AE_MOVDA32X2(xnsah,xnsal);
        fnsa=AE_MOVDA32X2(fnsah,fnsal);
        xnsa=AE_ADD32(xnsa,fnsa);
        xnsa=AE_SUB32(ensa,xnsa);
        AE_MOVT32X2(xnsa,AE_MOVDA32X2(0x20,0x20),inf);
        x0=AE_MOVINT16X4_FROMINT32X2(xnsa);
        x1=AE_MOVINT16X4_FROMINT32X2(AE_SEL32_LH(xnsa,xnsa));
        AE_S16_0_IP(x1,castxcc(ae_int16,exp),2);
        AE_S16_0_IP(x0,castxcc(ae_int16,exp),2);

        F=AE_SEL32_HL(FH,FL);
        AE_MOVT32X2(F,one,inf);
        AE_SA32X2_IP(F,afw,pfracWr);
    }
    AE_SA64POS_FP(afw,pfracWr);
    if(N&1)
    {
        minus_one=AE_MOVDA32X2(0x80000000,0x80000000);
              one=AE_MOVDA32X2(0x7fffffff,0x7fffffff);
        T=AE_L32_I((const ae_int32*) py,    0);
        X=AE_L32_I((const ae_int32*) px,    0);
        F=AE_L32_I((const ae_int32*) pfrac, 0);

        inf = AE_EQ32(T, AE_ZERO32());
        sgn = AE_LT32(X, AE_ZERO32());
        AE_MOVT32X2(one,minus_one,sgn);

        xnsah = AE_NSAZ32_L(X);
        X = AE_SLAA32S(X,xnsah);
        F = AE_MULFP32X2RAS(X,F);

        fnsah = AE_NSAZ32_L(F);
        F = AE_SLAA32S(F,fnsah);

        // combine exponents
        AE_L16_IP(x1,pexp,2);
        ensa=AE_SEXT32X2D16_32(x1);
        xnsa=AE_MOVDA32X2(xnsah,xnsah);
        fnsa=AE_MOVDA32X2(fnsah,fnsah);
        xnsa=AE_ADD32(xnsa,fnsa);
        xnsa=AE_SUB32(ensa,xnsa);
        AE_MOVT32X2(xnsa,AE_MOVDA32X2(0x20,0x20),inf);
        x1=AE_MOVINT16X4_FROMINT32X2(AE_SEL32_LH(xnsa,xnsa));
        AE_S16_0_IP(x1,castxcc(ae_int16,exp),2);

        AE_MOVT32X2(F,one,inf);
        AE_S32_L_I(F,(ae_int32*)pfracWr,0);
    }
}
#else
{
    ae_int32x2 X,Y,Z,E;
    int n,N=M;
    const ae_int32x2  * restrict px;
    const ae_int32x2  * restrict py;
    const ae_int32x2 * restrict pfRd;
          ae_int32x2 * restrict pfWr;
          ae_valign   wr_align,rd_align;
          ae_valign   x_align ,y_align ;
    if(N<=0) return;
    /* take exponent and normalize inputs. Y is saved to the scratch */
    px=(const ae_int32x2*)x;
    py=(const ae_int32x2*)y;
    pfRd = (const ae_int32x2*)frac;
    pfWr = (      ae_int32x2*)frac;
    wr_align=AE_ZALIGN64();
    rd_align=AE_LA64_PP(pfRd);
    x_align =AE_LA64_PP(px);
    y_align =AE_LA64_PP(py);
    for (n = 0; n<(N >> 1); n++)
    {
        ae_int32x2 Xl,Xh,Yl,Yh;
        ae_f32x2 t;
        xtbool2 sy;
        ae_int32x2 _0x40000000=AE_MOVDA32(0x40000000);
        int expx,expy;
        /* normalization */
        AE_LA32X2_IP(X,x_align,px);
        AE_LA32X2_IP(Y,y_align,py);
        expx = AE_NSAZ32_L(X);
        expy = AE_NSAZ32_L(Y);
        Xl=AE_SLAA32S(X,expx);
        Yl=AE_SLAA32S(Y,expy);

        exp[1] = (int16_t)(expy - expx + 1);
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
        AE_SA32X2_IP(Z,wr_align,pfWr);
    }
    AE_SA64POS_FP(wr_align,pfWr);
    if (N & 1)
    {
      ae_int32x2 Xl, Yl;
      ae_f32x2 t;
      xtbool2 sy;
      ae_int32x2 _0x40000000 = AE_MOVDA32(0x40000000);
      int expx, expy;
      /* normalization */
      AE_L32_IP(X, castxcc(ae_int32, px), 0);
      AE_L32_IP(Y, castxcc(ae_int32, py), 0);
      expx = AE_NSAZ32_L(X);
      expy = AE_NSAZ32_L(Y);
      Xl = AE_SLAA32S(X, expx);
      Yl = AE_SLAA32S(Y, expy);
      exp[0] = (int16_t)(expy - expx + 1);
      X = AE_SEL32_LL(Xl, Xl);
      Y = AE_SEL32_LL(Yl, Yl);

      sy = AE_LT32(Y, AE_ZERO32());
      Y = AE_INT32X2_ABS32S(Y);
      /* first approximation */
      Z = AE_SUB32(AE_MOVDA32((int32_t)0xBAEC0000), Y);
      /* 4 iterations to achieve 1 LSB accuracy in mantissa */
      t = _0x40000000; AE_MULSFP32X2RAS(t, Y, Z); E = t;
      E = AE_ADD32(E, E);
      t = Z; AE_MULAFP32X2RAS(t, Z, E); Z = t;
      t = _0x40000000; AE_MULSFP32X2RAS(t, Y, Z); E = t;
      E = AE_ADD32(E, E);
      t = Z; AE_MULAFP32X2RAS(t, Z, E); Z = t;
      t = _0x40000000; AE_MULSFP32X2RAS(t, Y, Z); E = t;
      E = AE_ADD32(E, E);
      t = Z; AE_MULAFP32X2RAS(t, Z, E); Z = t;
      t = _0x40000000; AE_MULSFP32X2RAS(t, Y, Z); E = t;
      E = AE_ADD32(E, E);
      t = Z; AE_MULAFP32X2RAS(t, Z, E); Z = t;
      /* restore original sign */
      Y = AE_INT32X2_NEG32S(Z);
      AE_MOVT32X2(Z, Y, sy);
      /* multiply by X */
      Z = AE_MULFP32X2RAS(X, Z);
      AE_S32_L_I(Z, (ae_int32 *)pfWr, 0);
    }
}
#endif
