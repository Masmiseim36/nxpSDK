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
void vec_recip16x16
(
  int16_t * restrict frac, 
  int16_t * exp, 
  const int16_t * restrict x, 
  int N
)
#if 0
{
    int n;
    static const ALIGN(8) int16_t cnst0123[]={0,1,2,3};
    ae_int16x4 _0123;

    const ae_int16   * restrict px = (const ae_int16    *)x;
          ae_int16   * restrict pf = (       ae_int16   *)frac;
    const ae_int16x4 * restrict pfRd;
    const ae_int16x4 * restrict pfWr;
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
#else   /* compute using 24-bit operations */
{
    int16_t ALIGN(8) temp[MAX_ALLOCA_SZ/sizeof(int16_t)];
    int n;
    const ae_int16   * restrict px = (const ae_int16    *)x;
          ae_int16   * restrict pf = (       ae_int16   *)frac;
    const ae_p16x2s  * restrict pfRd;
    const ae_int16   * restrict pfWr;

    while (N>0)
    {
        int M;
        M=MAX_ALLOCA_SZ/sizeof(int16_t);
        if(M>N) M=N;
        /* compute exponent and normalize inputs */
        px = (const ae_int16 *)x;
        pf = (      ae_int16*)temp;
        for(n=0; n<M; n++)
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
        pfRd = (const ae_p16x2s*)(temp-2);
        pfWr = (      ae_int16 *)frac;
        for(n=0; n<(M&~1); n+=2)
        {
            xtbool2 sx,isZero;
            ae_p24x2s rd;
            ae_int32x2 X,X0,Y,E;
            ae_int32x2 _0x400000  =AE_MOVDA32X2(0x00400000,0x00400000);
            ae_int32x2 _0x00BAEC00=AE_MOVDA32X2(0x00BAEC00,0x00BAEC00);
            ae_int32x2 _0x7fff00  =AE_MOVDA32X2(0x007fff00,0x007fff00);
            ae_f32x2 f;

            AE_LP16X2F_IU(rd,pfRd,4);
            X0=AE_MOVINT32X2_FROMF24X2(rd);
            sx=AE_LT32(X0,AE_ZERO32());
            X=AE_ABS24S(X0);
            isZero=AE_EQ32(X,AE_ZERO32());
            AE_MOVT32X2(X,_0x7fff00,isZero);
            Y=AE_SUB32(_0x00BAEC00,X); /* first approximation */
            /* 3 iterations */
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
            AE_MOVT32X2(Y,X,sx);
            Y=AE_MOVINT32X2_FROMF32X2(AE_SLLI32S(AE_MOVF32X2_FROMINT32X2(Y),8));
            Y=AE_SRAI32(Y,16);
            AE_S16_0_I (AE_MOVINT16X4_FROMINT32X2(Y), castxcc(ae_int16, pfWr),2);
            Y=AE_INTSWAP(Y);
            AE_S16_0_IP(AE_MOVINT16X4_FROMINT32X2(Y), castxcc(ae_int16, pfWr), 4);
        }
        if(N&1)
        {
            xtbool2 sx,isZero;
            ae_p24x2s rd;
            ae_int32x2 X,X0,Y,E;
            ae_int32x2 _0x400000  =AE_MOVDA32X2(0x00400000,0x00400000);
            ae_int32x2 _0x00BAEC00=AE_MOVDA32X2(0x00BAEC00,0x00BAEC00);
            ae_int32x2 _0x7fff00  =AE_MOVDA32X2(0x007fff00,0x007fff00);
            ae_f32x2 f;

            AE_LP16X2F_IU(rd,pfRd,4);
            X0=AE_MOVINT32X2_FROMF24X2(rd);
            sx=AE_LT32(X0,AE_ZERO32());
            X=AE_ABS24S(X0);
            isZero=AE_EQ32(X,AE_ZERO32());
            AE_MOVT32X2(X,_0x7fff00,isZero);
            Y=AE_SUB32(_0x00BAEC00,X); /* first approximation */
            /* 3 iterations */
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
            AE_MOVT32X2(Y,X,sx);
            Y=AE_MOVINT32X2_FROMF32X2(AE_SLLI32S(AE_MOVF32X2_FROMINT32X2(Y),8));
            Y=AE_SRAI32(Y,16);
            //AE_S16_0_I (AE_MOVINT16X4_FROMINT32X2(Y),pfWr,2);
            Y=AE_INTSWAP(Y);
            AE_S16_0_IP(AE_MOVINT16X4_FROMINT32X2(Y), castxcc(ae_int16, pfWr), 4);
        }

        exp+=M;
        frac+=M;
        x+=M;
        N=N-M;
    }
} /* vec_recip16x16() */
#endif
