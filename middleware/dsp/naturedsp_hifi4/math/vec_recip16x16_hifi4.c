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

/* DSP Library API */
#include "NatureDSP_Signal_math.h"
#include "common.h"
/*===========================================================================
  Vector matematics:
  vec_recip            Reciprocal on Q31/Q15 Numbers
===========================================================================*/

/*-------------------------------------------------------------------------
  Reciprocal on Q63/Q31/Q15 Numbers
  These routines return the fractional and exponential portion of the 
  reciprocal of a vector x of Q31 or Q15 numbers. Since the reciprocal is 
  always greater than 1, it returns fractional portion frac in Q(31-exp) 
  or Q(15-exp) format and exponent exp so true reciprocal value in the 
  Q0.31/Q0.15 may be found by shifting fractional part left by exponent 
  value.

  Mantissa accuracy is 1 LSB, so relative accuracy is:
  vec_recip16x16, scl_recip16x16                   6.2e-5 
  scl_recip32x32                                   2.4e-7 
  vec_recip32x32                                   9.2e-10
  vec_recip64x64                                   2.2e-19

  Precision: 
  64x64  64-bit input, 64-bit output. 
  32x32  32-bit input, 32-bit output. 
  16x16  16-bit input, 16-bit output. 

  Input:
  x[N]    input data, Q63, Q31 or Q15
  N       length of vectors

  Output:
  frac[N] fractional part of result, Q(63-exp), Q(31-exp) or Q(15-exp)
  exp[N]  exponent of result 

  Restriction:
  x,frac,exp should not overlap

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  frac,x - aligned on 8-byte boundary
  N      - multiple of 4 and >4

  Scalar versions:
  ----------------
  Return packed value: 
  scl_recip64x64():
  bits 55:0 fractional part
  bits 63:56 exponent
  scl_recip32x32():
  bits 23:0 fractional part
  bits 31:24 exponent
  scl_recip16x16():
  bits 15:0 fractional part
  bits 31:16 exponent
-------------------------------------------------------------------------*/
#if 0

void vec_recip16x16
(
/*algorithm:
  y = recip(x)
  y0 = 3-2*x
  3 iterations:
  e=1-yi.*x;
  yi=yi+e.*yi;
*/
  int16_t * restrict frac, 
  int16_t * exp, 
  const int16_t * restrict x, 
  int N
)
{
    ae_valign ax,af,ae; 
    const ae_int16x4* restrict px;
          ae_int16x4* restrict pfrac;
          ae_int16x4* restrict pexp;
    int xnsa0,xnsa1,xnsa2,xnsa3;
    int n;
    volatile int __0x40000000=0x40000000;
    volatile int __0x10001   =0x10001;
    volatile int __0x7fff7fff=0x7fff7fff;
    ae_int32x2 _0x40000000,_0xBFFFFFFF;
    ae_int16x4 _0x10001,_0x100010,_0x7fff7fff;

    ae_int16x4 X,X1,X2,X3,T,vexp;
    ae_f32x2   YH,YL,EH,EL;
    xtbool4 sinf,sneg;
    if(N<=0) return;
    px=(const ae_int16x4*)x;
    pfrac=(ae_int16x4*)frac;
    pexp =(ae_int16x4*)exp;
    ax = AE_LA64_PP(px);
    af = AE_ZALIGN64();
    ae = AE_ZALIGN64();
    for (n=0; n<(N&(~3)); n+=4)        
    {
        // prevent caching
       // XT_MOVEQZ(__0x40000000,__0x40000000,__0x40000000);
       // XT_MOVEQZ(__0x10001   ,__0x10001   ,__0x10001);
        //XT_MOVEQZ(__0x7fff7fff,__0x7fff7fff,__0x7fff7fff);
        _0x40000000 = AE_MOVDA32X2(__0x40000000,__0x40000000);
        _0x10001    = AE_MOVDA16X2(__0x10001,   __0x10001);
        _0x7fff7fff = AE_MOVDA16X2(__0x7fff7fff,__0x7fff7fff);
        _0x100010   = AE_SLAI16S(_0x10001,4);
        _0xBFFFFFFF=AE_MOVDA32X2(0xBFFFFFFF,0xBFFFFFFF);

        // extract exponent and normalize input to 0.5...1 range
        AE_LA16X4_IP(X,ax,px);
        sneg=AE_LT16(X,AE_ZERO16());
        sinf=AE_EQ16(X,AE_ZERO16());
        X = AE_ABS16S(X);                             
        xnsa0=AE_NSAZ16_0(X);
        xnsa1=AE_NSAZ16_0(X1=AE_SEL16_4321(X,X));
        xnsa2=AE_NSAZ16_0(X2=AE_SEL16_5432(X,X));
        xnsa3=AE_NSAZ16_0(X3=AE_SEL16_6543(X,X));
        X = AE_SLAA16S(X ,xnsa0);
        X1= AE_SLAA16S(X1,xnsa1);
        X2= AE_SLAA16S(X2,xnsa2);
        X3= AE_SLAA16S(X3,xnsa3);
        X =AE_SEL16_6420(X1,X );
        X2=AE_SEL16_6420(X3,X2);
        X =AE_SEL16_6420(X2,X );
        // combine nsa into the vector
        X2 = AE_MOVDA16X2(xnsa1,xnsa0);
        X3 = AE_MOVDA16X2(xnsa3,xnsa2);
        vexp =AE_SEL16_5410(X3,X2);
        /*
            yi=3-2*x;
            for iter=1:3
            e=1-yi.*x;
            yi=yi+e.*yi;
            end
        */
        YL= AE_CVT32X2F16_10(X);
        YH= AE_CVT32X2F16_32(X);
        YL= AE_SUB32(_0xBFFFFFFF,YL);
        YH= AE_SUB32(_0xBFFFFFFF,YH);
        EL= _0x40000000; AE_MULSFP32X16X2RAS_L(EL,YL,X);
        EH= _0x40000000; AE_MULSFP32X16X2RAS_H(EH,YH,X);
        EL = AE_SLAI32S(EL,1);
        EH = AE_SLAI32S(EH,1);
        AE_MULAFP32X2RAS(YL,YL,EL);
        AE_MULAFP32X2RAS(YH,YH,EH);
        EL= _0x40000000; AE_MULSFP32X16X2RAS_L(EL,YL,X);
        EH= _0x40000000; AE_MULSFP32X16X2RAS_H(EH,YH,X);
        EL = AE_SLAI32S(EL,1);
        EH = AE_SLAI32S(EH,1);
        AE_MULAFP32X2RAS(YL,YL,EL);
        AE_MULAFP32X2RAS(YH,YH,EH);
        EL= _0x40000000; AE_MULSFP32X16X2RAS_L(EL,YL,X);
        EH= _0x40000000; AE_MULSFP32X16X2RAS_H(EH,YH,X);
        EL = AE_SLAI32S(EL,1);
        EH = AE_SLAI32S(EH,1);
        AE_MULAFP32X2RAS(YL,YL,EL);
        AE_MULAFP32X2RAS(YH,YH,EH);
        // apply sign and control infinity
        X = AE_ROUND16X4F32SASYM(YH,YL); 
        T = AE_NEG16S(X);
        AE_MOVT16X4(X,T,sneg);
        AE_MOVT16X4(X,_0x7fff7fff,sinf);
        vexp=AE_ADD16(vexp,_0x10001);
        AE_MOVT16X4(vexp,_0x100010,sinf);

        AE_SA16X4_IP(X,af,pfrac);
        AE_SA16X4_IP(vexp,ae,pexp);
    }
    AE_SA64POS_FP(af,pfrac);
    AE_SA64POS_FP(ae,pexp);
    // last 1..3 iterations
    N&=3;
    if (N)
    {
        // prevent caching
 //       XT_MOVEQZ(__0x40000000,__0x40000000,__0x40000000);
//        XT_MOVEQZ(__0x10001   ,__0x10001   ,__0x10001);
        //XT_MOVEQZ(__0x7fff7fff,__0x7fff7fff,__0x7fff7fff);
        _0x40000000 = AE_MOVDA32X2(__0x40000000,__0x40000000);
        _0x10001    = AE_MOVDA16X2(__0x10001,   __0x10001);
        _0x7fff7fff = AE_MOVDA16X2(__0x7fff7fff,__0x7fff7fff);
        _0x100010   = AE_SLAI16S(_0x10001,4);
        _0xBFFFFFFF=AE_MOVDA32X2(0xBFFFFFFF,0xBFFFFFFF);

        // extract exponent and normalize input to 0.5...1 range
        AE_LA16X4_IP(X,ax,px);
        sneg=AE_LT16(X,AE_ZERO16());
        sinf=AE_EQ16(X,AE_ZERO16());
        X = AE_ABS16S(X);
        xnsa0=AE_NSAZ16_0(X);
        xnsa1=AE_NSAZ16_0(X1=AE_SEL16_4321(X,X));
        xnsa2=AE_NSAZ16_0(X2=AE_SEL16_5432(X,X));
        xnsa3=AE_NSAZ16_0(X3=AE_SEL16_6543(X,X));
        X = AE_SLAA16S(X ,xnsa0);
        X1= AE_SLAA16S(X1,xnsa1);
        X2= AE_SLAA16S(X2,xnsa2);
        X3= AE_SLAA16S(X3,xnsa3);
        X =AE_SEL16_6420(X1,X );
        X2=AE_SEL16_6420(X3,X2);
        X =AE_SEL16_6420(X2,X );
        // combine nsa into the vector
        X2 = AE_MOVDA16X2(xnsa1,xnsa0);
        X3 = AE_MOVDA16X2(xnsa3,xnsa2);
        vexp =AE_SEL16_5410(X3,X2);
        /*
            yi=3-2*x;
            for iter=1:4
            e=1-yi.*x;
            yi=yi+e.*yi;
            end
        */
        YL= AE_CVT32X2F16_10(X);
        YH= AE_CVT32X2F16_32(X);
        YL= AE_SUB32(_0xBFFFFFFF,YL);
        YH= AE_SUB32(_0xBFFFFFFF,YH);
        EL= _0x40000000; AE_MULSFP32X16X2RAS_L(EL,YL,X);
        EH= _0x40000000; AE_MULSFP32X16X2RAS_H(EH,YH,X);
        EL = AE_SLAI32S(EL,1);
        EH = AE_SLAI32S(EH,1);
        AE_MULAFP32X2RAS(YL,YL,EL);
        AE_MULAFP32X2RAS(YH,YH,EH);
        EL= _0x40000000; AE_MULSFP32X16X2RAS_L(EL,YL,X);
        EH= _0x40000000; AE_MULSFP32X16X2RAS_H(EH,YH,X);
        EL = AE_SLAI32S(EL,1);
        EH = AE_SLAI32S(EH,1);
        AE_MULAFP32X2RAS(YL,YL,EL);
        AE_MULAFP32X2RAS(YH,YH,EH);
        EL= _0x40000000; AE_MULSFP32X16X2RAS_L(EL,YL,X);
        EH= _0x40000000; AE_MULSFP32X16X2RAS_H(EH,YH,X);
        EL = AE_SLAI32S(EL,1);
        EH = AE_SLAI32S(EH,1);
        AE_MULAFP32X2RAS(YL,YL,EL);
        AE_MULAFP32X2RAS(YH,YH,EH);
        // apply sign and control infinity
        X = AE_ROUND16X4F32SASYM(YH,YL); 
        T = AE_NEG16S(X);
        AE_MOVT16X4(X,T,sneg);
        AE_MOVT16X4(X,_0x7fff7fff,sinf);
        vexp=AE_ADD16(vexp,_0x10001);
        AE_MOVT16X4(vexp,_0x100010,sinf);

        // save
        X3=AE_SEL16_4321(X,X);
        X2=AE_SEL16_5432(X,X);
        X1=AE_SEL16_6543(X,X);
        AE_S16_0_IP(X1,castxcc(ae_int16,pfrac),2);
        if (N>1) AE_S16_0_IP(X2,castxcc(ae_int16,pfrac),2);
        if (N>2) AE_S16_0_IP(X3,castxcc(ae_int16,pfrac),2);

        X3=AE_SEL16_4321(vexp,vexp);
        X2=AE_SEL16_5432(vexp,vexp);
        X1=AE_SEL16_6543(vexp,vexp);
        AE_S16_0_IP(X1,castxcc(ae_int16,pexp),2);
        if (N>1) AE_S16_0_IP(X2,castxcc(ae_int16,pexp),2);
        if (N>2) AE_S16_0_IP(X3,castxcc(ae_int16,pexp),2);
    }
}
#else
void vec_recip16x16
(
  int16_t * restrict frac, 
  int16_t * exp, 
  const int16_t * restrict x, 
  int N
)
{
    int n;
    static const ALIGN(8) int16_t cnst0123[]={0,1,2,3};
    ae_int16x4 _0123;

    const ae_int16   * restrict px = (const ae_int16    *)x;
          ae_int16   * restrict pf = (       ae_int16   *)frac;
    const ae_int16x4 * restrict pfRd;
          ae_int16x4 * restrict pfWr;
          ae_valign   wr_align,rd_align;

    if(N<=0) return;
    /* compute exponent and normalize inputs */
    px = (const ae_int16 *)x;
    pf = (      ae_int16*)frac;
    for(n=0; n<N; n++)
    {
        xtbool isZero;
        unsigned nsa;
        ae_int16x4 X;
        ae_int64   Z;
        AE_L16_IP(X, px, sizeof(int16_t));
        Z = AE_MOVINT64_FROMINT16X4(X);
        isZero=AE_EQ64(Z,AE_ZERO64());
        nsa=AE_NSAZ16_0(X);
        XT_MOVT(nsa,0x10,(isZero));  /* return biggest exponent for zero input */
        X=AE_SLAA16S_vector(X,nsa);
        exp[n]=(int16_t)(nsa+1);
        AE_S16_0_IP(X,pf,sizeof(int16_t));
    }
    __Pragma("no_reorder");
    pfRd = (const ae_int16x4*)frac;
    pfWr = (      ae_int16x4*)frac;
    wr_align=AE_ZALIGN64();
    rd_align=AE_LA64_PP(pfRd);
    _0123 = AE_L16X4_I((const ae_int16x4*)cnst0123,0);
    for(n=N; n>0; n-=4)
    {
        xtbool4 sx,wr_mask,isZero;
        ae_int16x4 X0,X,Y,E,_0x4000=AE_MOVDA16(16384);
        AE_LA16X4_IP(X0,rd_align,pfRd);
        sx=AE_LT16(X0,AE_ZERO16());
        X=AE_ABS16S_vector(X0);
        isZero=AE_EQ16(X,AE_ZERO16());
        AE_MOVT16X4(X,AE_MOVDA16(32767),isZero);
        Y=AE_SUB16(AE_MOVDA16((int16_t)47852),X); /* first approximation */
        E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(X,Y));  /* 3 iterations to achieve 1 LSB accuracy in mantissa */
        E=AE_ADD16(E,E);
        Y=AE_ADD16S_vector(Y,AE_MULFP16X4S_vector(Y,E));
        E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(X,Y));
        E=AE_ADD16(E,E);
        Y=AE_ADD16S_vector(Y,AE_MULFP16X4S_vector(Y,E));
        E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(X,Y));
        E=AE_ADD16(E,E);
        Y=AE_ADD16S_vector(Y,AE_MULFP16X4S_vector(Y,E));
        X=AE_NEG16S_vector(Y); /* restore original sign */
        AE_MOVT16X4(Y,X,sx);
        wr_mask=AE_LT16(_0123,AE_MOVDA16(n)); /* compute mask for last incomplete iteration */
        AE_MOVT16X4(X0,Y,wr_mask);
        AE_SA16X4_IP(X0,wr_align,pfWr);
    }
    AE_SA64POS_FP(wr_align,pfWr);
}
#endif
