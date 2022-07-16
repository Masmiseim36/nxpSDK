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
#include "logq23_tbl.h"

/*-------------------------------------------------------------------------
  Logarithm:
  Different kinds of logarithm (base 2, natural, base 10). 32 and 24-bit 
  fixed point functions interpret input as Q16.15 and represent results in 
  Q25 format or return 0x80000000 on negative of zero input. 16-bit fixed-
  point functions interpret input as Q8.7 and represent result in Q3.12 or
  return 0x8000 on negative of zero input

  Precision:
  16x16  16-bit inputs, 16-bit outputs
  24x24  24-bit inputs, 24-bit outputs
  32x32  32-bit inputs, 32-bit outputs
  f      floating point

  Accuracy :
  16x16 functions                                                    2 LSB
  vec_log2_32x32,scl_log2_32x32  , vec_log2_24x24,scl_log2_24x24     730 (2.2e-5)
  vec_logn_32x32,scl_logn_32x32  , vec_logn_24x24,scl_logn_24x24     510 (1.5e-5)
  vec_log10_32x32,scl_log10_32x32, vec_log10_24x24,scl_log10_24x24   230 (6.9e-6)
  floating point                                                     2 ULP

  NOTES:
  1.  Although 32 and 24 bit functions provide the same accuracy, 32-bit 
      functions have better input/output resolution (dynamic range)
  2.  Scalar Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly.
  3.  Floating point functions limit the range of allowable input values:
      A) If x<0, the result is set to NaN. In addition, scalar floating point
         functions assign the value EDOM to errno and raise the "invalid" 
         floating-point exception.
      B) If x==0, the result is set to minus infinity. Scalar floating  point
         functions assign the value ERANGE to errno and raise the "divide-by-zero"
         floating-point exception.

  Input:
  x[N]  input data, Q16.15 (32 or 24-bit functions), Q8.7 (16-bit functions) or 
        floating point 
  N     length of vectors
  Output:
  y[N]  result, Q6.25 (32 or 24-bit functions), Q3.12 (16-bit functions) or 
        floating point 

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  return result result, Q6.25 (32 or 24-bit functions), Q3.12 (16-bit 
  functions) or floating point
-------------------------------------------------------------------------*/
void vec_log10_16x16(int16_t * restrict y, const int16_t * restrict x, int N)
{
    const ae_p16s* restrict px;
          ae_p16s* restrict py;
    const ae_int32* restrict ptbl=(const ae_int32*)logq23_tbl;

    xtbool2 small,lesszero;
    ae_int32x2 X0,X1,X,E,Z,Y,Z2;
    ae_f32x2 f;
    int e0,e1;
    int n;
    NASSERT(x);
    NASSERT(y);
    if (N<0) return;
    px=(const ae_p16s*)(x-1);
    py=(      ae_p16s*)(y-1);
    for(n=0; n<(N>>1);n++) 
    {
        /* take inputs and rescale */
        X0=AE_L16M_I (px,  sizeof(int16_t));
        AE_L16M_IU(X1,px,2*sizeof(int16_t));
        e0=AE_NSAZ32_L(X0);
        e1=AE_NSAZ32_L(X1);
        X0=AE_SLAA32(X0,e0-8);
        X1=AE_SLAA32(X1,e1-8);
        X =AE_SEL32_HH(X0,X1);
        E =AE_MOVDA32X2(16-e0,16-e1);
        lesszero = AE_LE32(X,0);
        small    = AE_LE32(X,AE_L32_I(ptbl,12));
        AE_MOVT32X2(X,AE_SLAI32(X,1),small);
        AE_MOVT32X2(E,AE_SUB32(E,1),small);
        /* compute polynomial */
        Z=AE_SUB32(X,0x800000);
        Y=AE_L32_I(ptbl,0);
        Z2=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Z),AE_MOVF24X2_FROMINT32X2(Z));
        f=AE_L32_I(ptbl,4);  AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Z ),AE_MOVF24X2_FROMINT32X2(Y)); Y=f;
        f=AE_L32_I(ptbl,8);  AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Z ),AE_MOVF24X2_FROMINT32X2(Y)); Y=f;
        f=Z;                 AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Z2),AE_MOVF24X2_FROMINT32X2(Y)); Y=f;
        /* apply original exponent and scale */
        Z=AE_L32_I(ptbl,24);
        Y=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y),AE_MOVF24X2_FROMINT32X2(Z));
        Z=AE_L32_I(ptbl,28);
        f=Y;AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(AE_SLAI32(E,16)),AE_MOVF24X2_FROMINT32X2(Z)); Y=f;
        Y=AE_SLAI24S(AE_MOVF24X2_FROMINT32X2(Y),4);  
        /* save results */
        AE_MOVT32X2(Y,0x800000,lesszero);
        AE_S16M_L_I (AE_INTSWAP(Y),py,  sizeof(int16_t));
        AE_S16M_L_IU(           Y ,py,2*sizeof(int16_t));
        //Y=Y; __MNSD__
    }
    if(N&1)
    {
        /* take inputs and rescale */
        X0=AE_L16M_I (px,  sizeof(int16_t));
        e0=AE_NSAZ32_L(X0);
        X0=AE_SLAA32(X0,e0-8);
        X =X0;
        E =AE_MOVDA32X2(16-e0,16-e0);
        lesszero = AE_LE32(X,0);
        small    = AE_LE32(X,AE_L32_I(ptbl,12));
        AE_MOVT32X2(X,AE_SLAI32(X,1),small);
        AE_MOVT32X2(E,AE_SUB32(E,1),small);
        /* compute polynomial */
        Z=AE_SUB32(X,0x800000);
        Y=AE_L32_I(ptbl,0);
        Z2=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Z),AE_MOVF24X2_FROMINT32X2(Z));
        f=AE_L32_I(ptbl,4);  AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Z ),AE_MOVF24X2_FROMINT32X2(Y)); Y=f;
        f=AE_L32_I(ptbl,8);  AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Z ),AE_MOVF24X2_FROMINT32X2(Y)); Y=f;
        f=Z;                 AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Z2),AE_MOVF24X2_FROMINT32X2(Y)); Y=f;
        /* apply original exponent and scale */
        Z=AE_L32_I(ptbl,24);
        Y=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y),AE_MOVF24X2_FROMINT32X2(Z));
        Z=AE_L32_I(ptbl,28);
        f=Y;AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(AE_SLAI32(E,16)),AE_MOVF24X2_FROMINT32X2(Z)); Y=f;
        Y=AE_SLAI24S(AE_MOVF24X2_FROMINT32X2(Y),4);  
        /* save results */
        AE_MOVT32X2(Y,0x800000,lesszero);
        AE_S16M_L_I (Y,py,  sizeof(int16_t));
    }
}
