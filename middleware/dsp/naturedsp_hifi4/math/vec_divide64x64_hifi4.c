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
    Divide 64x64 
    Code optimized for HiFi4
  IntegrIT, 2006-2018
*/

#include "NatureDSP_Signal_math.h"
#include "NatureDSP_types.h"
#include "common.h"

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
void vec_divide64x64 
                (int64_t * restrict frac, 
                 int16_t *exp,
                 const int64_t * restrict x, 
                 const int64_t * restrict y, int N)
{
#define SCR_SZ (MAX_ALLOCA_SZ/(sizeof(int64_t)*2))
    const ae_int64* restrict pX;
    const ae_int64* restrict pY;
    const ae_int64* restrict pScrRd;
          ae_int64* restrict pScrWr;
    const ae_int64* restrict pFractRd;
          ae_int64* restrict pFractWr;
          ae_int16* restrict pExp;
    int64_t scratch[SCR_SZ*2];
    int n,m,M;

    for (m=0; m<N; m+=SCR_SZ,x+=SCR_SZ,y+=SCR_SZ,frac+=SCR_SZ,exp+=SCR_SZ)
    {
        M=XT_MIN(N-m,SCR_SZ);
        /*
            normalization phase. Here we compute output exponent, 
            save the sign of numerator in the LSB of exp[], and normalized 
            numerator/denominator in the scratch memory
        */
        pExp    =(ae_int16*) exp;
        pFractWr=(ae_int64*)frac;
        pScrWr  =(ae_int64*)scratch;
        pX      =(const ae_int64*)x;
        pY      =(const ae_int64*)y;
        for (n=0; n<M; n++)
        {
            ae_int64 x,y;
            int expx,expy,exponent;
            xtbool sy, bzero;
            AE_L64_IP(x,pX,sizeof(int64_t));
            AE_L64_IP(y,pY,sizeof(int64_t));
            sy=AE_LT64(y,AE_ZERO64());
            bzero=AE_EQ64(y,AE_ZERO64());
            y=AE_ABS64S(y);
            expx=AE_NSA64(x);
            expy=AE_NSA64(y);
            x=AE_SLAA64(x,expx);
            y=AE_SLAA64(y,expy);
            exponent=expy-expx+1;
            XT_MOVNEZ(exponent,0x40,AE_MOVAB(bzero));
            // set numerator MAX_INT64/MIN_INT64 according to the 
            // sign if denominator is zero
            AE_MOVT64(x,AE_SUB64(MAX_INT64,AE_SRAI64(x,63)),bzero);
            AE_MOVT64(y,0x4000000000000000LL,bzero);
            exp[n]=(exponent<<1)|AE_MOVAB(sy);
            AE_S64_IP(x,pFractWr,sizeof(int64_t));
            AE_S64_IP(y,pScrWr,2*sizeof(int64_t));
        }
        /* at the next phase, we compute approximate reciprocal 
           in 32-bit accuracy using SIMD operations
        */
        __Pragma("no_reorder")
        pScrRd  =(const ae_int64*)scratch;
        pScrWr  =(      ae_int64*)(scratch+1);
        for (n=0; n<(M&~1); n+=2)
        {
            ae_int32x2 e,x,y;
            ae_int64 y0,y1;
            AE_L64_IP(y0,pScrRd,2*sizeof(int64_t));
            AE_L64_IP(y1,pScrRd,2*sizeof(int64_t));
            x=AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(y0),AE_MOVINT32X2_FROMINT64(y1));
            y=AE_SUB32((int32_t)0xBAEC0000UL,x);
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            y0=AE_MOVINT64_FROMINT32X2(AE_SEL32_HH(y,AE_ZERO32()));
            y1=AE_MOVINT64_FROMINT32X2(AE_SEL32_LH(y,AE_ZERO32()));
            AE_S64_IP(y0,pScrWr,2*sizeof(int64_t));
            AE_S64_IP(y1,pScrWr,2*sizeof(int64_t));
        }
        if (M&1)
        {
            ae_int32x2 e,x,y;
            ae_int64 y0;
            AE_L64_IP(y0,pScrRd,0);
            x=AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(y0),AE_MOVINT32X2_FROMINT64(y0));
            y=AE_SUB32((int32_t)0xBAEC0000UL,x);
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            y0=AE_MOVINT64_FROMINT32X2(AE_SEL32_HH(y,AE_ZERO32()));
            AE_S64_IP(y0,pScrWr,0);
        }
        /* last 2 iterations are done in better accuracy: 
            first one use simpler computation of 1-x*y, next 
            one adds more 8 bits for precision of this matter 
            but able to use simpler last multiply
        */
        __Pragma("no_reorder")
        pScrRd  =(const ae_int64*)scratch;
        pScrWr  =(      ae_int64*)scratch+1;
        for (n=0; n<M; n++)
        {
            ae_int64   x,y,z,e;
            ae_ep      Aep;
            AE_L64_IP(x,pScrRd,sizeof(int64_t));
            AE_L64_IP(y,pScrRd,sizeof(int64_t));

            AE_MULZAAD32USEP_HL_LH(Aep,e,AE_MOVINT32X2_FROMINT64(x),AE_MOVINT32X2_FROMINT64(y));
            e=AE_SRAI72(Aep,e,32);
            AE_MULA32_HH(e,AE_MOVINT32X2_FROMINT64(x),AE_MOVINT32X2_FROMINT64(y));
            e=AE_SLAI64(e,3);

            AE_MULZAAD32USEP_HL_LH(Aep,z,AE_MOVINT32X2_FROMINT64(y),AE_MOVINT32X2_FROMINT64(e));
            z=AE_SRAI72(Aep,z,32);
            AE_MULA32_HH(z,AE_MOVINT32X2_FROMINT64(y),AE_MOVINT32X2_FROMINT64(e));
            y=AE_SUB64S(y,z);

            AE_S64_IP(y,pScrWr,2*sizeof(int64_t));
        }
        __Pragma("no_reorder")
        pScrRd  =(const ae_int64*)scratch;
        pScrWr  =(      ae_int64*)scratch;
        for (n=0; n<M; n++)
        {
            ae_int64   x,y,z,e;
            ae_ep      Aep;
            ae_int32x2 t;
            AE_L64_IP(x,pScrRd,sizeof(int64_t));
            AE_L64_IP(y,pScrRd,sizeof(int64_t));

            z=AE_SRLI64(AE_MUL32U_LL(AE_MOVINT32X2_FROMINT64(x),AE_MOVINT32X2_FROMINT64(y)),32);
            Aep=AE_MOVEA(0);  AE_MULAAD32USEP_HL_LH(Aep,z,AE_MOVINT32X2_FROMINT64(x),AE_MOVINT32X2_FROMINT64(y));
            z=AE_SLLI64(z,10);
            e=AE_SLLI64(AE_MUL32_HH(AE_MOVINT32X2_FROMINT64(x),AE_MOVINT32X2_FROMINT64(y)),42);
            e=AE_ADD64(z,e);

            t=AE_MULFP32X2RAS(AE_MOVINT32X2_FROMINT64(y),AE_MOVINT32X2_FROMINT64(e));
            AE_S32X2_IP(t,castxcc(ae_int32x2,pScrWr),sizeof(int64_t));
            AE_S64_IP(y,pScrWr,sizeof(int64_t));
        }
        /* last stage: very accurate multiply by numerator with right sign */
        __Pragma("no_reorder")
        pScrRd  =(const ae_int64*)scratch;
        pScrWr  =(      ae_int64*)scratch;
        pFractRd=(const ae_int64*)frac;
        pFractWr=(      ae_int64*)frac;
        pExp    =(      ae_int16*)exp ;
        for (n=0; n<M; n++)
        {
            ae_ep Aep;
            ae_int64   x,r,z;
            ae_int32x2 dr;
            ae_int16x4 e;
            xtbool sy;
            r=AE_L64_I(pScrRd,1*sizeof(int64_t));
            AE_L32X2_IP(dr,castxcc(ae_int32x2,pScrRd),2*sizeof(int64_t));
            AE_L64_IP(x,pFractRd,sizeof(int64_t));
            Aep=AE_MOVEA(0); z=AE_SRLI64(AE_MUL32U_LL(AE_MOVINT32X2_FROMINT64(r),AE_MOVINT32X2_FROMINT64(x)),32);
            AE_MULAAD32USEP_HL_LH(Aep,z,AE_MOVINT32X2_FROMINT64(r),AE_MOVINT32X2_FROMINT64(x));
            z=AE_SRAI72(Aep,z,31);
            AE_MULAF32S_HH(z,AE_MOVINT32X2_FROMINT64(r),AE_MOVINT32X2_FROMINT64(x));
            z=AE_SUB64S(z,AE_SRAI64(AE_MUL32_HH(dr,AE_MOVINT32X2_FROMINT64(x)),39));
            e=AE_L16_I(pExp,0);
            sy=AE_MOVBA(AE_MOVAD16_0(e)&1);
            AE_S16_0_IP(AE_SRAI16(e,1),pExp,sizeof(int16_t));
            AE_MOVT64(z,AE_NEG64S(z),sy);
            AE_S64_IP(z,pFractWr,sizeof(int64_t));
        }
    }
}
