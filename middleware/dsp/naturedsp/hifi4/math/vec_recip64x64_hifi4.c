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
    Vector operations, Math functions, 64-bit reciprocal  
    Code optimized for HiFi4
  IntegrIT, 2006-2018
*/
#include "NatureDSP_Signal_math.h"
#include "NatureDSP_types.h"
#include "common.h"

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
void vec_recip64x64 (int64_t *  frac, int16_t *exp, const int64_t * x, int N)
{
#define SCR_SZ (MAX_ALLOCA_SZ/sizeof(int64_t))
          ae_int64* restrict pFracWr;
    const ae_int64* restrict pFracRd;
          ae_int64* restrict pScr;
          ae_int16* restrict pExp;
    const ae_int64* restrict pX;
    ae_int64 scratch[SCR_SZ];
    int n,m;

    /*    in the 1-st phase, take exponent and normalize input    */
    pFracWr=(ae_int64*)frac;
    pX     =(const ae_int64*)x;
    pExp   =(ae_int16*)exp;
    for (n=0; n<N; n++)
    {
        xtbool sx,bzero;
        int exponent;
        ae_int64 x;
        AE_L64_IP(x,pX,sizeof(int64_t));
        sx   =AE_LT64(x,AE_ZERO64());
        bzero=AE_EQ64(x,AE_ZERO64());
        x=AE_ABS64S(x);
        exponent=AE_NSA64(x);
        x=AE_SLAA64(x,exponent);// x in 0.5..1
        exponent++;
        XT_MOVNEZ(exponent,0x40,AE_MOVAB(bzero));
        XT_MOVNEZ(exponent,XT_NEG(exponent),AE_MOVAB(sx));
        AE_MOVT64(x,0x4000000000000000LL,bzero);
        AE_S64_IP(x,pFracWr,sizeof(int64_t));
        AE_S16_0_IP(AE_MOVDA16(exponent),pExp,sizeof(int16_t));
    }
    for (m=0; m<N; m+=SCR_SZ,exp+=SCR_SZ,frac+=SCR_SZ)
    {
        __Pragma("no_reorder")
        int M=XT_MIN(SCR_SZ,(N-m));
        /* first stage use 3 iterations computing in 32-bit accuracy */
        pScr   =(      ae_int64*)scratch;
        pFracRd=(const ae_int64*)frac;
        for (n=0; n<(M&~1); n+=2)
        {
            ae_int32x2 e,x,y;
            ae_int64 y0,y1;
            AE_L64_IP(y0,pFracRd,sizeof(int64_t));
            AE_L64_IP(y1,pFracRd,sizeof(int64_t));
            x=AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(y0),AE_MOVINT32X2_FROMINT64(y1));
            y=AE_SUB32((int32_t)0xBAEC0000UL,x);
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            y0=AE_MOVINT64_FROMINT32X2(AE_SEL32_HH(y,AE_ZERO32()));
            y1=AE_MOVINT64_FROMINT32X2(AE_SEL32_LH(y,AE_ZERO32()));
            AE_S64_IP(y0,pScr,sizeof(int64_t));
            AE_S64_IP(y1,pScr,sizeof(int64_t));
        }
        if (M&1)
        {
            ae_int32x2 e,x,y;
            ae_int64 y0;

            AE_L64_IP(y0,pFracRd,sizeof(int64_t));
            x=AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(y0),AE_MOVINT32X2_FROMINT64(y0));
            y=AE_SUB32((int32_t)0xBAEC0000UL,x);
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            e=0x40000000; AE_MULSFP32X2RAS(e,x,y); AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
            y0=AE_MOVINT64_FROMINT32X2(AE_SEL32_HH(y,AE_ZERO32()));
            AE_S64_IP(y0,pScr,sizeof(int64_t));
        }
        /* last 2 iterations are done in better accuracy: 
            first one use simpler computation of 1-x*y, next 
            one adds more 8 bits for precision of this matter 
            but able to use simpler last multiply
        */
        __Pragma("no_reorder")
        pScr   =(ae_int64*)scratch;
        pFracWr=(ae_int64*)frac;
        pFracRd=(const ae_int64*)frac;
        pExp   =(ae_int16*)exp;
        for (n=0; n<M; n++)
        {
            ae_int16x4 exponent;
            xtbool sx;
            ae_int64 x,y,e,z,A,B;
            ae_ep Aep;
            AE_L64_IP(x,pFracRd,sizeof(ae_int64));
            AE_L64_IP(y,pScr   ,sizeof(ae_int64));
            AE_MULZAAD32USEP_HL_LH(Aep,A,AE_MOVINT32X2_FROMINT64(x),AE_MOVINT32X2_FROMINT64(y));
            A=AE_SRAI72(Aep,A,29);
            B=AE_MUL32_HH(AE_MOVINT32X2_FROMINT64(x),AE_MOVINT32X2_FROMINT64(y));
            B=AE_SLLI64(B,3);
            e=AE_ADD64(B,A);

            AE_MULZAAD32USEP_HL_LH(Aep,A,AE_MOVINT32X2_FROMINT64(y),AE_MOVINT32X2_FROMINT64(e));
            A=AE_SRAI72(Aep,A,32);
            AE_MULA32_HH(A,AE_MOVINT32X2_FROMINT64(y),AE_MOVINT32X2_FROMINT64(e));
            y=AE_SUB64S(y,A);
            A=AE_MUL32U_LL(AE_MOVINT32X2_FROMINT64(x),AE_MOVINT32X2_FROMINT64(y));
            A=AE_SRLI64(A,32);
            AE_MULAAD32USEP_HL_LH(Aep,A,AE_MOVINT32X2_FROMINT64(x),AE_MOVINT32X2_FROMINT64(y));
            A=AE_SRAI72(Aep,A,21);
            B=AE_MUL32_HH(AE_MOVINT32X2_FROMINT64(x),AE_MOVINT32X2_FROMINT64(y));
            B=AE_SLLI64(B,11);
            e=AE_ADD64(B,A);
            z=AE_SRAI64(AE_MUL32_HL(AE_MOVINT32X2_FROMINT64(y),AE_MOVINT32X2_FROMINT64(e)),40); // just signed-by-signed 32x32 multiply
            y=AE_SUB64S(y,z);
            exponent=AE_L16_I(pExp,0);
//            sx=xtbool4_extract_0(AE_LT16(exponent,AE_ZERO16()));
            sx=AE_LT64(AE_MOVINT64_FROMINT16X4(exponent),AE_ZERO64());
            exponent=AE_ABS16S(exponent);
            AE_S16_0_IP(exponent,pExp,sizeof(int16_t));
            AE_MOVT64(y,AE_NEG64(y),sx);
            AE_S64_IP(y,pFracWr,sizeof(ae_int64));
        }
    }
}
