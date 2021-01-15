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
    Vector multiply-accumulate for Emulated Floating Point
    optimized code for HiFi4 core
*/
/* Library API */
#include "NatureDSP_Signal_vector.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Vector Multiply-Accumulate for Emulated Floating Point
  routines multiply-accumulate vectors by scalar represented in emulated 
  floating point format

  Input:
  xmant[N]  mantissa of input data
  xexp[N]   exponent of input data
  ymant     mantissa of scalar
  yexp      exponent of scalar
  N         length of vectors
  Output:
  zmant[N]  mantissa of output data
  zexp[N]   exponent of output data

  Restriction:
  xmant,xexp,zmant,zexp should not overlap
-------------------------------------------------------------------------*/
void   vec_mac_32x16ef (      int32_t  *  restrict zmant,       int16_t  * restrict zexp, 
                        const int32_t  *  restrict xmant, const int16_t  * restrict xexp, 
                              int32_t              ymant,       int16_t             yexp, 
                        int N)
{
    ae_int16* restrict pZexpWr=(ae_int16*)zexp;
    ae_int64 a0,a1;
    ae_int64 b0,b1;
    ae_int32x2 x,xe,ae,ce,exp,cmant,nsa;
    int32_t cexp0,cexp1,xexp0,xexp1,nsa0,nsa1;
    ae_valign aXmant,aZmant,aZrd;
    ae_int16x4 nsa16;
    xtbool2 byeqz;
    const ae_int32x2 * restrict pZrd;

    int n;
    if (N<=0) return;
    byeqz=AE_EQ32(ymant,AE_ZERO32());
    pZrd=(const ae_int32x2*)zmant;
    // odd iteration
    if (N&1)
    {
        xexp0=*xexp++; 
        AE_L32_IP(x,castxcc(ae_int32,xmant),sizeof(int32_t));
        cexp0 =*zexp++;
        AE_L32_IP(cmant,castxcc(ae_int32,pZrd),sizeof(int32_t));
        /* multiply input with ymant,yexp */
        ce=AE_MOVDA32(cexp0);
        xe=AE_MOVDA32(xexp0);
        ae=AE_ADD32(xe,yexp);   
        a0=AE_MUL32_HH(x,ymant);
        AE_MOVT32X2(ae,MIN_INT16, byeqz);
        AE_MOVT32X2(ae,MIN_INT16, AE_EQ32(x    ,AE_ZERO32()));
        AE_MOVT32X2(ce,MIN_INT16, AE_EQ32(cmant,AE_ZERO32()));
        /* add third operand with renormalization */
        ce=AE_ADD32(ce,31);
        exp=AE_MAX32(ae,ce);
        ae=AE_SUB32(ae,exp);
        ce=AE_SUB32(ce,exp);
        a0=AE_SLAA64S(a0,AE_MOVAD32_H(ae));
        b0=AE_SLAA64S(AE_MUL32_HH(cmant,1),AE_MOVAD32_H(ce));
        a0=AE_ADD64S(a0,b0);
        exp=AE_SUB32(exp,31);
        /* final normalization */
        nsa0=AE_NSA64(a0);
        nsa=AE_MOVDA32(nsa0);
        x=AE_TRUNCA32X2F64S(a0,a0,nsa0);
        nsa=AE_SUB32(exp,AE_SUB32(nsa,32));
        /* underflow processing */
        AE_MOVT32X2(x,0,AE_LT32(nsa,MIN_INT16));
        /* overflow processing */
        AE_MOVT32X2(x,AE_SLLI32S(x,31),AE_LT32(MAX_INT16,nsa));
        AE_S32_L_IP(x,castxcc(ae_int32,zmant),sizeof(int32_t));
        nsa16=AE_SAT16X4(nsa,nsa);
        AE_S16_0_IP(nsa16,pZexpWr,sizeof(int16_t));
    }
    N&=~1;
    // process by 2 numbers in parallel
    pZexpWr=(ae_int16*)zexp;
    aXmant=AE_LA64_PP(castxcc(ae_int32x2,xmant));
    aZmant=AE_ZALIGN64();
    pZrd=(const ae_int32x2*)zmant;
    aZrd=AE_LA64_PP(pZrd);
    for (n=0; n<N; n+=2) 
    {
        xexp0=*xexp++; 
        xexp1=*xexp++;
        AE_LA32X2_IP(x,aXmant,castxcc(ae_int32x2,xmant));
        cexp0 =*zexp++;
        cexp1 =*zexp++;
        AE_LA32X2_IP(cmant,aZrd,pZrd);
        /* multiply input with ymant,yexp */
        ce=AE_MOVDA32X2(cexp0,cexp1);
        xe=AE_MOVDA32X2(xexp0,xexp1);
        ae=AE_ADD32(xe,yexp);   
        a0=AE_MUL32_HH(x,ymant);
        a1=AE_MUL32_LH(x,ymant);
        AE_MOVT32X2(ae,MIN_INT16, byeqz);
        AE_MOVT32X2(ae,MIN_INT16, AE_EQ32(x    ,AE_ZERO32()));
        AE_MOVT32X2(ce,MIN_INT16, AE_EQ32(cmant,AE_ZERO32()));
        /* add third operand with renormalization */
        ce=AE_ADD32(ce,31);
        exp=AE_MAX32(ae,ce);
        ae=AE_SUB32(ae,exp);
        ce=AE_SUB32(ce,exp);
        a0=AE_SLAA64S(a0,AE_MOVAD32_H(ae));
        a1=AE_SLAA64S(a1,AE_MOVAD32_L(ae));
        b0=AE_SLAA64S(AE_MUL32_HH(cmant,1),AE_MOVAD32_H(ce));
        b1=AE_SLAA64S(AE_MUL32_LH(cmant,1),AE_MOVAD32_L(ce));
        a0=AE_ADD64S(a0,b0);
        a1=AE_ADD64S(a1,b1);
        exp=AE_SUB32(exp,31);
        /* final normalization */
        nsa0=AE_NSA64(a0);
        nsa1=AE_NSA64(a1);
        nsa=AE_MOVDA32X2(nsa0,nsa1);
        x=AE_SEL32_HH(AE_TRUNCA32F64S(a0,nsa0),AE_TRUNCA32F64S(a1,nsa1));
        nsa=AE_SUB32(exp,AE_SUB32(nsa,32));
        /* underflow processing */
        AE_MOVT32X2(x,0,AE_LT32(nsa,MIN_INT16));
        /* overflow processing */
        AE_MOVT32X2(x,AE_SLLI32S(x,31),AE_LT32(MAX_INT16,nsa));
        AE_SA32X2_IP(x,aZmant,castxcc(ae_int32x2,zmant));
        nsa16=AE_SAT16X4(nsa,nsa);
        AE_S16_0_IP(AE_SHORTSWAP(nsa16),pZexpWr,sizeof(int16_t));
        AE_S16_0_IP((nsa16),pZexpWr,sizeof(int16_t));
    }
    AE_SA64POS_FP(aZmant,castxcc(ae_int32x2,zmant));
}
