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
void vec_divide16x16 
(
  int16_t *       restrict  frac,
  int16_t *       restrict  exp,
  const int16_t * restrict  x,
  const int16_t * restrict  y,
  int M)
#if 0
{
#define SCR_SZ (MAX_ALLOCA_SZ/sizeof(int16_t))
    int16_t ALIGN(8) scr[SCR_SZ];   /* local scratch */
    static const ALIGN(8) int16_t cnst0123[]={0,1,2,3};
    ae_int16x4 _0123;
    ae_int16x4 X,Y,Z,E;
    int n,N;
    const ae_int16   * restrict px;
    const ae_int16   * restrict py;
          ae_int16   * restrict pf;
          ae_int16   * restrict ps;
    const ae_int16x4 * restrict pfRd;
    const ae_int16x4 * restrict pfWr;
    const ae_int16x4 * restrict psRd;
          ae_valign   wr_align,rd_align;

    while(M>0)
    {
        N=XT_MIN(SCR_SZ,M); /* size of portion */
        /* take exponent and normalize inputs. Y is saved to the scratch */
        px=(const ae_int16 *)x;
        py=(const ae_int16 *)y;
        pf=(      ae_int16 *)frac;
        ps=(      ae_int16 *)scr;
        for (n=0; n<N; n++)
        {
            int expx,expy;
            AE_L16_IP(X,px,sizeof(int16_t));
            AE_L16_IP(Y,py,sizeof(int16_t));
            expx = AE_NSAZ16_0(X);
            expy = AE_NSAZ16_0(Y);
            X=AE_SLAA16S(X,expx);
            Y=AE_SLAA16S(Y,expy);
            AE_S16_0_IP(X,pf,sizeof(int16_t));
            AE_S16_0_IP(Y,ps,sizeof(int16_t));
            exp[n]=(int16_t)(expy-expx+1);
        }
        __Pragma("no_reorder");
        pfRd = (const ae_int16x4*)frac;
        pfWr = (      ae_int16x4*)frac;
        psRd = (const ae_int16x4*)scr;
        wr_align=AE_ZALIGN64();
        rd_align=AE_LA64_PP(pfRd);
        _0123 = AE_L16X4_I((const ae_int16x4*)cnst0123,0);
        for(n=N; n>0; n-=4)
        {
            xtbool4 sy,wr_mask;
            ae_int16x4 _0x4000=AE_MOVDA16(16384);
            AE_L16X4_IP(Y,psRd,sizeof(Y));
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
            AE_LA16X4_IP(X,rd_align,pfRd);
            Z=AE_MULFP16X4RAS(X,Z);
            wr_mask=AE_LT16(_0123,AE_MOVDA16(n)); /* compute mask for last incomplete iteration */
            AE_MOVT16X4(X,Z,wr_mask);
            AE_SA16X4_IP(X,wr_align,pfWr);
        }
        AE_SA64POS_FP(wr_align,pfWr);
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
#define SCR_SZ (MAX_ALLOCA_SZ/sizeof(int16_t))
    int16_t ALIGN(8) scr[SCR_SZ];   /* local scratch */
    int n,N;
    const ae_int16   * restrict px;
    const ae_int16   * restrict py;
          ae_int16   * restrict pf;
          ae_int16   * restrict ps;
    const ae_p16s    * restrict pfRd;
    const ae_p16x2s  * restrict pfWr;
    const ae_p16x2s  * restrict psRd;
          ae_valign   rd_align;

    while(M>0)
    {
        N=XT_MIN(SCR_SZ,M); /* size of portion */
        /* take exponent and normalize inputs. Y is saved to the scratch */
        px=(const ae_int16 *)x;
        py=(const ae_int16 *)y;
        pf=(      ae_int16 *)frac;
        ps=(      ae_int16 *)scr;
        for (n=0; n<N; n++)
        {
            ae_int16x4 X,Y;
            int expx,expy;
            AE_L16_IP(X,px,sizeof(int16_t));
            AE_L16_IP(Y,py,sizeof(int16_t));
            expx = AE_NSAZ16_0(X);
            expy = AE_NSAZ16_0(Y);
            X=AE_SLAA16S(X,expx);
            Y=AE_SLAA16S(Y,expy);
            AE_S16_0_IP(X,pf,sizeof(int16_t));
            AE_S16_0_IP(Y,ps,sizeof(int16_t));
            exp[n]=(int16_t)(expy-expx+1);
        }
        __Pragma("no_reorder");
        pfRd = (const ae_p16s  *)(frac-1);
        pfWr = (      ae_p16x2s*)(frac-1);
        psRd = (const ae_p16x2s*)(scr-2);
        rd_align=AE_LA64_PP(pfRd);
        for(n=0; n<(N&~1); n+=2)
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
            Y=AE_SAT24S(Z);
            AE_L16M_IU(X,pfRd,2);
            Z=AE_SAT24S(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(Y))));
            AE_S16M_L_IU(Z,castxcc(ae_p16s,pfWr),2);
            AE_L16M_IU(X,pfRd,2);
            Z=AE_SAT24S(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2((Y))));
            AE_S16M_L_IU(Z, castxcc(ae_p16s, pfWr), 2);
        }
        if(N&1)
        {
            ae_int32x2 Y,E,X,Z;
            ae_f32x2 f;
            xtbool2 sy;
            ae_int32x2 _0x400000=AE_MOVDA32(0x400000);
            Y=AE_L16M_I((const ae_p16s*)psRd,4);
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
            AE_L16M_IU(X,pfRd,2);
            Z=AE_SAT24S(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(Z))));
            AE_S16M_L_IU(Z, castxcc(ae_p16s, pfWr), 2);
        }
        /* process next portion */
        M-=N;
        x+=N;
        y+=N;
        frac+=N;
        exp+=N;
    }
} /* vec_divide16x16() */
#endif
