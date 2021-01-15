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
    Vector multiply for Emulated Floating Point
    optimized code for HiFi4 core
*/
/* Library API */
#include "NatureDSP_Signal_vector.h"
#include "NatureDSP_types.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Vector Multiply for Emulated Floating Point
  routines multiply two vectors represented in emulated floating point format

  Input:
  xmant[N]  mantissa of input data
  ymant[N]  mantissa of input data
  xexp[N]   exponent of input data
  yexp[N]   exponent of input data
  N         length of vectors
  Output:
  zmant[N]  mantissa of output data
  zexp[N]   exponent of output data

  Restriction:
  xmant,ymant,xexp,yexp,zmant,zexp should not overlap
-------------------------------------------------------------------------*/
void   vec_mul_32x16ef (      int32_t  * restrict zmant,       int16_t  * restrict zexp, 
                        const int32_t  * restrict xmant, const int16_t  * restrict xexp, 
                        const int32_t  * restrict ymant, const int16_t  * restrict yexp, 
                        int N)
{
    ae_valign aXmant,aYmant,aZmant;
    ae_int16x4 nsa16;
    xtbool2 ufl;
    ae_int64 a0,a1;
    ae_int32x2 x_exp,y_exp,exp,x_mant,y_mant,nsa,x;
    int x_exp0,y_exp0,nsa0;
    int x_exp1,y_exp1,nsa1;
    int n;
    if(N<=0) return;
    if (N&1)
    {
        x_exp0=*xexp++;
        y_exp0=*yexp++;
        x_exp=AE_MOVDA32(x_exp0);
        y_exp=AE_MOVDA32(y_exp0);

        AE_L32_IP(x_mant,castxcc(ae_int32,xmant),sizeof(int32_t));
        AE_L32_IP(y_mant,castxcc(ae_int32,ymant),sizeof(int32_t));
        /* add exponents and multiply mantissas */
        exp=AE_SUB32(AE_ADD32(x_exp,y_exp),31);
        a0=AE_MUL32_HH(x_mant,y_mant);
        /* normalization */
        nsa0=AE_NSA64(a0);
        nsa=AE_MOVDA32(nsa0);
        x=AE_TRUNCA32X2F64S(a0,a0,nsa0);
        nsa=AE_SUB32(exp,AE_SUB32(nsa,32));
        /* underflow processing */
        ufl=AE_LT32(nsa,MIN_INT16);
        AE_MOVT32X2(x,0,ufl);
        /* overflow processing */
        AE_MOVT32X2(x,AE_SLLI32S(x,31),AE_LT32(MAX_INT16,nsa));
        AE_S32_L_IP(x,castxcc(ae_int32,zmant),sizeof(int32_t));
        nsa16=AE_SAT16X4(nsa,nsa);
        AE_S16_0_IP((nsa16),castxcc(ae_int16,zexp),sizeof(int16_t));
    }
    N&=~1;
    // process by 2 numbers in parallel
    aXmant=AE_LA64_PP(castxcc(ae_int32x2,xmant));
    aYmant=AE_LA64_PP(castxcc(ae_int32x2,ymant));
    aZmant=AE_ZALIGN64();
    for (n=0; n<N; n+=2) 
    {
        x_exp0=*xexp++,y_exp0=*yexp++;
        x_exp1=*xexp++,y_exp1=*yexp++;
        x_exp=AE_MOVDA32X2(x_exp0,x_exp1);
        y_exp=AE_MOVDA32X2(y_exp0,y_exp1);

        AE_LA32X2_IP(x_mant,aXmant,castxcc(ae_int32x2,xmant));
        AE_LA32X2_IP(y_mant,aYmant,castxcc(ae_int32x2,ymant));
        /* add exponents and multiply mantissas */
        exp=AE_SUB32(AE_ADD32(x_exp,y_exp),31);
        a0=AE_MUL32_HH(x_mant,y_mant);
        a1=AE_MUL32_LL(x_mant,y_mant);
        /* normalization */
        nsa0=AE_NSA64(a0);
        nsa1=AE_NSA64(a1);
        nsa=AE_MOVDA32X2(nsa0,nsa1);
        x=AE_SEL32_HH(AE_TRUNCA32F64S(a0,nsa0),AE_TRUNCA32F64S(a1,nsa1));
        nsa=AE_SUB32(exp,AE_SUB32(nsa,32));
        /* underflow processing */
        ufl=AE_LT32(nsa,MIN_INT16);
        AE_MOVT32X2(x,0,ufl);
        /* overflow processing */
        AE_MOVT32X2(x,AE_SLLI32S(x,31),AE_LT32(MAX_INT16,nsa));
        AE_SA32X2_IP(x,aZmant,castxcc(ae_int32x2,zmant));
        nsa16=AE_SAT16X4(nsa,nsa);
        AE_S16_0_IP(AE_SHORTSWAP(nsa16),castxcc(ae_int16,zexp),sizeof(int16_t));
        AE_S16_0_IP((nsa16),castxcc(ae_int16,zexp),sizeof(int16_t));
    }
    AE_SA64POS_FP(aZmant,castxcc(ae_int32x2,zmant));
}
