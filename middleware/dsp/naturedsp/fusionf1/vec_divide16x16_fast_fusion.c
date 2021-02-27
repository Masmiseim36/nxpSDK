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
void vec_divide16x16_fast 
(
  int16_t *       restrict  frac,
  int16_t *       restrict  exp,
  const int16_t * restrict  x,
  const int16_t * restrict  y,
  int M)
#if 1
{
#define SCR_SZ (MAX_ALLOCA_SZ/sizeof(int16_t))
    int16_t ALIGN(8) scr[SCR_SZ];   /* local scratch */
    xtbool4 mask5;
    int n,N;
    const ae_int16x4 * restrict px;
    const ae_int16x4 * restrict py;
          ae_int16x4 * restrict pf;
          ae_int16x4 * restrict ps;

    const ae_p16x2s * restrict pfRd;
          ae_p16x2s * restrict pfWr;
    const ae_p16x2s * restrict psRd;
/*
    const ae_int16x4 * restrict pfRd;
          ae_int16x4 * restrict pfWr;
    const ae_int16x4 * restrict psRd;
*/
    static const int16_t ALIGN(8) _0101[]={0,1,0,1};
    mask5=AE_EQ16(AE_L16X4_I((const ae_int16x4*)_0101,0),AE_ZERO16());
    while(M>0)
    {
        N=XT_MIN(SCR_SZ,M); /* size of portion */
        NASSERT_ALIGN(x,8);
        NASSERT_ALIGN(y,8);
        NASSERT_ALIGN(frac,8);
        NASSERT(N%4==0);
        /* take exponent and normalize inputs. Y is saved to the scratch */
        px=(const ae_int16x4 *)x;
        py=(const ae_int16x4 *)y;
        pf=(      ae_int16x4*)frac;
        ps=(      ae_int16x4*)scr;
        for (n=0; n<(N>>2); n++)
        {
            ae_int16x4 X0,X1,X2,X3,Y0,Y1,Y2,Y3;
            ae_int16x4 X,Y;
            int expx,expy;
            AE_L16X4_IP(X,px,sizeof(X));
            AE_L16X4_IP(Y,py,sizeof(Y));
            expx = AE_NSAZ16_0(X);
            expy = AE_NSAZ16_0(Y);
            X0=AE_SLAA16S(X,expx);
            Y0=AE_SLAA16S(Y,expy);
            exp[3]=(int16_t)(expy-expx+1);
            X=AE_SEL16_4321(X,X);
            Y=AE_SEL16_4321(Y,Y);

            expx = AE_NSAZ16_0(X);
            expy = AE_NSAZ16_0(Y);
            X1=AE_SLAA16S(X,expx);
            Y1=AE_SLAA16S(Y,expy);
            exp[2]=(int16_t)(expy-expx+1);
            X=AE_SEL16_4321(X,X);
            Y=AE_SEL16_4321(Y,Y);

            expx = AE_NSAZ16_0(X);
            expy = AE_NSAZ16_0(Y);
            X2=AE_SLAA16S(X,expx);
            Y2=AE_SLAA16S(Y,expy);
            exp[1]=(int16_t)(expy-expx+1);
            X=AE_SEL16_4321(X,X);
            Y=AE_SEL16_4321(Y,Y);

            expx = AE_NSAZ16_0(X);
            expy = AE_NSAZ16_0(Y);
            X3=AE_SLAA16S(X,expx);
            Y3=AE_SLAA16S(Y,expy);

            X =AE_SEL16_6420(X2,X0);
            X1=AE_SEL16_6420(X3,X1);
            X1=AE_SEL16_6543(X1,X1);
            AE_MOVT16X4(X ,X1,mask5);
            AE_S16X4_IP(X,pf,sizeof(X));
            Y =AE_SEL16_6420(Y2,Y0);
            Y1=AE_SEL16_6420(Y3,Y1);
            Y1=AE_SEL16_6543(Y1,Y1);
            AE_MOVT16X4(Y ,Y1,mask5);
            AE_S16X4_IP(Y,ps,sizeof(Y));

            exp[0]=(int16_t)(expy-expx+1);
            exp+=4;
        }
        __Pragma("no_reorder");

        pfRd = (const ae_p16x2s*)(frac-2);
        pfWr = (      ae_p16x2s*)(frac-2);
        psRd = (const ae_p16x2s*)(scr-2);
        __Pragma("loop_count min=1,factor=2")
        for(n=0; n<(N>>1); n++)
        {
            ae_int32x2 Y,E,X,Z;
            ae_f32x2 f;
            xtbool2 sy;
            ae_int32x2 _0x400000=AE_MOVDA32(0x400000);
            AE_L16X2M_IU(Y,psRd,4);
            sy=AE_LT32(Y,AE_ZERO32());
            X=AE_SAT24S(AE_ABS32(Y));
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
            Z=Y;
            Y=AE_NEG32(Z);
            AE_MOVT32X2(Z,Y,sy);
            Z=AE_SAT24S(Z);
            /* multiply by X */
            AE_L16X2M_IU(X,pfRd,4);
            Z=AE_SAT24S(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(Z)));
            AE_S16X2M_IU(Z,pfWr,4);
        }

        /* process next portion */
        M-=N;
        x+=N;
        y+=N;
        frac+=N;
        exp+=N;
    }
}
#else
{
    ae_int16x4 X,Y,Z,E;
    int n,N=M;
    xtbool4 mask5;
    const ae_int16x4 * restrict px;
    const ae_int16x4 * restrict py;
          ae_int16x4 * restrict pf;
    static const int16_t ALIGN(8) _0101[]={0,1,0,1};
    mask5=AE_EQ16(AE_L16X4_I((const ae_int16x4*)_0101,0),AE_ZERO16());
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT_ALIGN(frac,8);
    NASSERT(N%4==0);

    if(N<=0) return;
    /* take exponent and normalize inputs. Y is saved to the scratch */
    px=(const ae_int16x4 *)x;
    py=(const ae_int16x4 *)y;
    pf=(      ae_int16x4*)frac;
    for (n=0; n<(N>>2); n++)
    {
        ae_int16x4 X0,X1,X2,X3,Y0,Y1,Y2,Y3;
        xtbool4 sy;
        ae_int16x4 _0x4000=AE_MOVDA16(16384);
        int expx,expy;
        AE_L16X4_IP(X,px,sizeof(X));
        AE_L16X4_IP(Y,py,sizeof(Y));
        expx = AE_NSAZ16_0(X);
        expy = AE_NSAZ16_0(Y);
        X0=AE_SLAA16S(X,expx);
        Y0=AE_SLAA16S(Y,expy);
        exp[3]=(int16_t)(expy-expx+1);
        X=AE_SEL16_4321(X,X);
        Y=AE_SEL16_4321(Y,Y);

        expx = AE_NSAZ16_0(X);
        expy = AE_NSAZ16_0(Y);
        X1=AE_SLAA16S(X,expx);
        Y1=AE_SLAA16S(Y,expy);
        exp[2]=(int16_t)(expy-expx+1);
        X=AE_SEL16_4321(X,X);
        Y=AE_SEL16_4321(Y,Y);

        expx = AE_NSAZ16_0(X);
        expy = AE_NSAZ16_0(Y);
        X2=AE_SLAA16S(X,expx);
        Y2=AE_SLAA16S(Y,expy);
        exp[1]=(int16_t)(expy-expx+1);
        X=AE_SEL16_4321(X,X);
        Y=AE_SEL16_4321(Y,Y);

        expx = AE_NSAZ16_0(X);
        expy = AE_NSAZ16_0(Y);
        X3=AE_SLAA16S(X,expx);
        Y3=AE_SLAA16S(Y,expy);
        exp[0]=(int16_t)(expy-expx+1);
        exp+=4;

        X =AE_SEL16_6420(X2,X0);
        X1=AE_SEL16_6420(X3,X1);
        X1=AE_SEL16_6543(X1,X1);
        AE_MOVT16X4(X ,X1,mask5);
        Y =AE_SEL16_6420(Y2,Y0);
        Y1=AE_SEL16_6420(Y3,Y1);
        Y1=AE_SEL16_6543(Y1,Y1);
        AE_MOVT16X4(Y ,Y1,mask5);

        sy=AE_LT16(Y,AE_ZERO16());
        Y=AE_ABS16S_vector(Y);
        /* first approximation */
        Z=AE_SUB16(AE_MOVDA16((int16_t)47852),Y); 
            /* 3 iterations to achieve 1 LSB accuracy in mantissa */
        E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(Y,Z)); 
        E=AE_ADD16(E,E);
        Z=AE_ADD16S_vector(Z,AE_MULFP16X4S_vector(Z,E));
        E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(Y,Z)); 
        E=AE_ADD16(E,E);
        Z=AE_ADD16S_vector(Z,AE_MULFP16X4S_vector(Z,E));
        E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(Y,Z)); 
        E=AE_ADD16(E,E);
        Z=AE_ADD16S_vector(Z,AE_MULFP16X4S_vector(Z,E));
        /* restore original sign */
        Y=AE_NEG16S_vector(Z);
        AE_MOVT16X4(Z,Y,sy);
        /* multiply by X */
        Z=AE_MULFP16X4RAS(X,Z);
        AE_S16X4_IP(Z,pf,sizeof(Z));
    }
}
#endif
