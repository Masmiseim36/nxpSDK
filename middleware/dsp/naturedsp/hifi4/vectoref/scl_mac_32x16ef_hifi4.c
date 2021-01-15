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
    Multiply-accumulate for Emulated Floating Point
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
void   scl_mac_32x16ef (      int32_t  * zmant, int16_t *  zexp, 
                              int32_t    xmant, int16_t    xexp, 
                              int32_t    ymant, int16_t    yexp)
{
    ae_int64 a,b;
    ae_int32x2 x;
    int32_t aexp,exp,cmant,cexp,nsa,cond;
    xtbool2 ufl;
    /* first, multiply and compute exponent for product */
    aexp=(int32_t)xexp+(int32_t)yexp;
    a=AE_MUL32_HH(xmant,ymant);
    XT_MOVNEZ(aexp,MIN_INT16,AE_MOVBA(AE_EQ64(a,0)));
    /* next, add with z with normalization of both inputs */
    cmant=zmant[0];
    cexp =zexp [0];
    XT_MOVEQZ(cexp,MIN_INT16,cmant);
    cexp=cexp+31;
    exp=XT_MAX(aexp,cexp);
    a=AE_SLAA64S(a,aexp-exp);
    b=AE_SLAA64S(AE_MUL32_HH(cmant,1),cexp-exp);
    a=AE_ADD64S(a,b);
    exp-=31;
    /* overall normalization */
    nsa=AE_NSA64(a);
    x=AE_TRUNCA32F64S(a,nsa);
    nsa=exp-(nsa-32);
    /* underflow processing */
    cond=0; XT_MOVLTZ(cond,1,nsa-MIN_INT16);
    ufl=AE_MOVBA1X2(cond,cond);
    AE_MOVT32X2(x,0,ufl);
    /* overflow processing */
    exp=0;
    XT_MOVLTZ(exp,31,MAX_INT16-nsa);
    x  =AE_SLAA32S(x,exp); /* saturate mantissa in case of overflow */ 
    AE_S32_L_I(x,(ae_int32*)zmant,0);
    zexp[0]=XT_MIN(MAX_INT16,nsa);
}
