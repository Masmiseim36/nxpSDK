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
  vec_recip            Reciprocal on Q31/Q15 Numbers
===========================================================================*/
/*-------------------------------------------------------------------------
  Reciprocal 
  Fixed point routines return the fractional and exponential portion of the 
  reciprocal of a vector x of Q31 or Q15 numbers. Since the reciprocal is 
  always greater than 1, it returns fractional portion frac in Q(31-exp) 
  or Q(15-exp) format and exponent exp so true reciprocal value in the 
  Q0.31/Q0.15 may be found by shifting fractional part left by exponent 
  value.

  Mantissa accuracy is 1 LSB, so relative accuracy is:
  vec_recip16x16, scl_recip16x16                   6.2e-5 
  vec_recip24x24, scl_recip32x32, scl_recip24x24   2.4e-7 
  vec_recip32x32                                   9.2e-10

  Floating point routines operate with standard floating point numbers. 
  Functions return +/-infinity on zero or denormalized input and provide 
  accuracy of 1 ULP.

  Precision: 
  32x32   32-bit input, 32-bit output. 
  24x24   24-bit input, 24-bit output. 
  16x16   16-bit input, 16-bit output. 
  f       floating point


  Input:
  x[N]    input data, Q31,Q15 or floating point
  N       length of vectors

  Output:
  frac[N] fractional part of result, Q(31-exp) or Q(15-exp) (fixed point 
          functions)
  exp[N]  exponent of result (fixed point functions) 
  y[N]    result (floating point function)

  Restriction:
  x,frac,exp should not overlap

  Scalar versions:
  ----------------
  Return packed value for fixed-point functions: 
  scl_recip24x24(),scl_recip32x32():
  bits 23:0 fractional part
  bits 31:24 exponent
  scl_recip16x16():
  bits 15:0 fractional part
  bits 31:16 exponent
-------------------------------------------------------------------------*/
void vec_recip32x32
(
  int32_t * restrict        frac,
  int16_t *                 exp,
  const int32_t * restrict  x,
  int                       N
)
#if 1
{
    int n;
    static const ALIGN(8) int32_t cnst01[]={0,1};
    ae_int32x2 _01;

    const ae_int32   * restrict px = (const ae_int32    *)x;
          ae_int32   * restrict pf = (      ae_int32   *)frac;
    const ae_int32x2 * restrict pfRd;
    const ae_int32x2 * restrict pfWr;
          ae_valign    wr_align,rd_align;

    if(N<=0) return;
    /* compute exponent and normalize inputs */
    px = (const ae_int32 *)x;
    pf = (      ae_int32*)frac;
    for(n=0; n<N; n++)
    {
        xtbool isZero;
        unsigned nsa;
        ae_int32x2 X;
        ae_int64   Z;
        AE_L32_IP(X, px, sizeof(int32_t));
        Z = AE_MOVINT64_FROMINT32X2(X);
        isZero=AE_EQ64(Z,AE_ZERO64());
        nsa=AE_NSAZ32_L(X);
        XT_MOVT(nsa,0x20,(isZero));  /* return biggest exponent for zero input */
        X=AE_SLAA32S(X,nsa);
        exp[n]=(int16_t)(nsa+1);
        AE_S32_L_IP(X,pf,sizeof(int32_t));
    }
    __Pragma("no_reorder");
    pfRd = (const ae_int32x2*)frac;
    pfWr = (      ae_int32x2*)frac;
    wr_align=AE_ZALIGN64();
    rd_align=AE_LA64_PP(pfRd);
    _01 = AE_L32X2_I((const ae_int32x2*)cnst01,0);
    for(n=N; n>0; n-=2)
    {
        xtbool2 sx,wr_mask,isZero;
        ae_f32x2 t;
        ae_int32x2 X0,X,Y,E,_0x40000000;
        _0x40000000=AE_MOVDA32(0x40000000);
        AE_LA32X2_IP(X0,rd_align,pfRd);
        sx=AE_LT32(X0,AE_ZERO32());
        X=AE_INT32X2_ABS32S(X0);
        isZero=AE_EQ32(X,AE_ZERO32());
        AE_MOVT32X2(X,AE_MOVDA32(0x7fffffff),isZero);
        Y=AE_SUB32(AE_MOVDA32((int32_t)0xBAEC0000),X); /* first approximation */
        /* 4 iterations to achieve 1 LSB accuracy in mantissa */
        t=_0x40000000; AE_MULSFP32X2RAS(t,X,Y); E=t;
        E=AE_ADD32(E,E);
        t=Y; AE_MULAFP32X2RAS(t,Y,E); Y=t;
        t=_0x40000000; AE_MULSFP32X2RAS(t,X,Y); E=t;
        E=AE_ADD32(E,E);
        t=Y; AE_MULAFP32X2RAS(t,Y,E); Y=t;
        t=_0x40000000; AE_MULSFP32X2RAS(t,X,Y); E=t;
        E=AE_ADD32(E,E);
        t=Y; AE_MULAFP32X2RAS(t,Y,E); Y=t;
        t=_0x40000000; AE_MULSFP32X2RAS(t,X,Y); E=t;
        E=AE_ADD32(E,E);
        t=Y; AE_MULAFP32X2RAS(t,Y,E); Y=t;

        X=AE_INT32X2_NEG32S(Y); /* restore original sign */
        AE_MOVT32X2(Y,X,sx);
        wr_mask=AE_LT32(_01,AE_MOVDA32(n)); /* compute mask for last incomplete iteration */
        AE_MOVT32X2(X0,Y,wr_mask);
        AE_SA32X2_IP(X0, wr_align, castxcc(ae_int32x2, pfWr));
    }
    AE_SA64POS_FP(wr_align, castxcc(void, pfWr));
} /* vec_recip32x32() */
#else
{
    static const ALIGN(8) int32_t cnst01[]={0,1};
    ae_int32x2 _01;
    ae_int32x2 X,Y,Z,E;
    int n;
    const ae_int32x2  * restrict py;
    const ae_int32x2 * restrict pfRd;
          ae_int32x2 * restrict pfWr;
          ae_valign   wr_align,rd_align;
          ae_valign   y_align ;
    if(N<=0) return;
    /* take exponent and normalize inputs. Y is saved to the scratch */
    py=(const ae_int32x2*)x;
    pfRd = (const ae_int32x2*)frac;
    pfWr = (      ae_int32x2*)frac;
    wr_align=AE_ZALIGN64();
    rd_align=AE_LA64_PP(pfRd);
    y_align =AE_LA64_PP(py);
    _01 = AE_L32X2_I((const ae_int32x2*)cnst01,0);
    for(n=N; n>0; n-=2)
    {
        ae_int32x2 Yl,Yh;
        ae_f32x2 t;
        xtbool2 sy,wr_mask,isZero;
        int ee;
        ae_int32x2 _0x40000000=AE_MOVDA32(0x40000000);
        unsigned expy;
        /* normalization */
        AE_LA32X2_IP(Y,y_align,py);
        isZero=AE_EQ32(Y,AE_ZERO32());
        expy = AE_NSAZ32_L(Y);
        XT_MOVT(expy,0x20,xtbool2_extract_0(isZero));
        Yl=AE_SLAA32S(Y,expy);
        ee=exp[1];
        XT_MOVNEZ(ee,(expy+1),n-1);
        //ee= (n==1) ? ee : (int16_t)(expy-expx+1);
        exp[1]=(int16_t)ee;
        Y=AE_SEL32_LH(Y,Y);
        expy = AE_NSAZ32_L(Y);
        Yh=AE_SLAA32S(Y,expy);
        XT_MOVT(expy,0x20,xtbool2_extract_1(isZero));
        exp[0]=(int16_t)(expy+1);
        Y=AE_SEL32_LL(Yh,Yl);
        exp+=2;

        AE_MOVT32X2(Y,AE_MOVDA32(0x7fffffff),isZero);
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
        AE_LA32X2_IP(X,rd_align,pfRd);
        wr_mask=AE_LT32(_01,AE_MOVDA32(n)); /* compute mask for last incomplete iteration */
        AE_MOVT32X2(X,Z,wr_mask);
        AE_SA32X2_IP(X,wr_align,pfWr);
    }
    AE_SA64POS_FP(wr_align,pfWr);
}
#endif
