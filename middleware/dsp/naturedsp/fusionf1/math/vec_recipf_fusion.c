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
#include "NatureDSP_types.h"
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, vec_recipf, (float32_t* restrict y,
                  const float32_t* restrict x, int N))
#else
#include "inff_tbl.h"

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
void vec_recipf  (float32_t* restrict y,
                  const float32_t* restrict x, int N)
{
    const ae_int32x2 *restrict px;
          ae_int32x2 *restrict pz;
    ae_valign ax,az;
    xtbool2 binf,bden;
    ae_int32x2 X,absX,Zinf,Zden;
    xtfloatx2 F,Z;
    xtfloat Zh,Zl;
    int n;
    NASSERT(x);
    NASSERT(y);
    if(N<=0) return;
    px=(const ae_int32x2 *restrict)x;
    pz=(      ae_int32x2 *restrict)y;
    ax=AE_LA64_PP(px);
    az=AE_ZALIGN64();
    for (n=0; n<(N&~1); n+=2)
    {
        AE_LA32X2_IP(X,ax,px);
        Zinf=AE_AND32(X  , 0x80000000); /* generate zero with right sign */
        Zden=AE_OR32 (Zinf,0x7f800000); /* generate infinity with right sign */
        absX=AE_AND32(X   ,0x7fffffff); /* absolute value */
        binf=AE_EQ32(absX ,0x7f800000); 
        bden=AE_LT32(absX ,0x00200000); /* 1/realmax('single')) */
        F=XT_AE_MOVXTFLOATX2_FROMINT32X2(X);
        Zh=XT_RECIP_S(XT_HIGH_S(F));
        Zl=XT_RECIP_S(XT_LOW_S (F));
        Z=XT_SEL32_LL_SX2((xtfloatx2)Zh,(xtfloatx2)Zl);
        X=XT_AE_MOVINT32X2_FROMXTFLOATX2(Z);
        AE_MOVT32X2(X,Zinf,binf);
        AE_MOVT32X2(X,Zden,bden);
        AE_SA32X2_IP(X,az,pz);
    }
    AE_SA64POS_FP(az, pz);
    if(N&1) 
    {
        X=AE_L32_I((const ae_int32*)px,0);
        Zinf=AE_AND32(X  , 0x80000000);
        Zden=AE_OR32 (Zinf,0x7f800000);
        absX=AE_AND32(X   ,0x7fffffff);
        binf=AE_EQ32(absX ,0x7f800000);
        bden=AE_LT32(absX ,0x00200000);
        F=XT_AE_MOVXTFLOATX2_FROMINT32X2(X);
        Zl=XT_RECIP_S(XT_LOW_S (F));
        Z=(xtfloatx2)Zl;
        X=XT_AE_MOVINT32X2_FROMXTFLOATX2(Z);
        AE_MOVT32X2(X,Zinf,binf);
        AE_MOVT32X2(X,Zden,bden);
        AE_S32_L_I(X,(ae_int32*)pz,0);
    }
}
#endif
