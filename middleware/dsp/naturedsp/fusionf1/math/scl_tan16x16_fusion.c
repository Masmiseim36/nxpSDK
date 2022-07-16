/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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
#include "tan16x16_tbl.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Tangent 
  Fixed point functions calculate tan(pi*x) for number written in Q15 or 
  Q31. Floating point functions compute tan(x)
  
  Precision:
  16x16  16-bit inputs (Q15), 16-bit outputs (Q8.7). Accuracy: 1 LSB
  24x24  24-bit inputs, 32-bit outputs. Accuracy: (1.3e-4*y+1LSB) 
                                        if abs(y)<=464873(14.19 in Q15) 
                                        or abs(x)<pi*0.4776
  32x32  32-bit inputs, 32-bit outputs. Accuracy: (1.3e-4*y+1LSB)
                                        if abs(y)<=464873(14.19 in Q15) 
                                        or abs(x)<pi*0.4776
  f      floating point.                Accuracy: 2 ULP

  NOTE:
  1.  Scalar floating point function is compatible with standard ANSI C routines 
      and set errno and exception flags accordingly
  2.  Floating point functions limit the range of allowable input values: [-9099, 9099]
      Whenever the input value does not belong to this range, the result is set to NaN.

  Input:
  x[N]   input data, Q15, Q31 or floating point
  N      length of vectors
  Output:
  y[N]   result, Q8.7 (16-bit function), Q16.15 (24 or 32-bit functions) 
         or floating point

  Restriction:
  x,y - should not overlap

  Scalar versions:
  ----------------
  Return result, Q8.7 (16-bit function), Q16.15 (24 or 32-bit functions) 
  or floating point
-------------------------------------------------------------------------*/
int16_t scl_tan16x16 (int16_t x)
{
    int ey;
    xtbool2 sx,sr;
    ae_int32x2 Z,X,Y,E;
    ae_f32x2   f;
    ae_f24x2   X2;

    /* load and reduction */
    X=AE_MOVDA32(x)<<16;
    Y=AE_SLAI32(AE_NEG32(AE_ABS32(X)),1);
    Z=polytan16x16_tbl[5];
    sr=AE_LT32(Z,AE_ABS32S(Y));
    Y=AE_XOR32(X,Y);
    sx=AE_LT32(0,Y);
    X=AE_ABS32(AE_SRAI32(AE_SLAI32(X,2),8));
        /* compute polynomial */
    X2=AE_MOVF24X2_FROMINT32X2(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(X)));
    Y=polytan16x16_tbl[0];
    f=polytan16x16_tbl[1]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),X2); Y=f;
    f=polytan16x16_tbl[2]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),X2); Y=f;
    f=polytan16x16_tbl[3]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),X2); Y=f;
    f=polytan16x16_tbl[4]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),X2); Y=f;
    f=X;                   AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Y),AE_MOVF24X2_FROMINT32X2(X)); Y=AE_SAT24S(f);

    /* compute reciprocal of Y */
    Y=AE_MAX32(Y,1);
    ey=AE_NSAZ32_L(Y)-8; 

    X=AE_SLAA32(Y,ey);
    Z=AE_SUB32(polytan16x16_tbl[6],X);
    f=polytan16x16_tbl[7];
    AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Z),AE_MOVF24X2_FROMINT32X2(X)); E=f;   E=AE_SLAI32(E,1);
    f=Z;   AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Z),AE_MOVF24X2_FROMINT32X2(E)); Z=f;
    f=polytan16x16_tbl[7];
    AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Z),AE_MOVF24X2_FROMINT32X2(X)); E=f;   E=AE_SLAI32(E,1);
    f=Z;   AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(Z),AE_MOVF24X2_FROMINT32X2(E)); Z=f;
    Z=AE_SLAA32S(Z,ey+1);
    /* finally, select tan/cotan and the sign */
    AE_MOVT32X2(Y,Z,sr);
    AE_MOVT32X2(Y,AE_NEG32(Y),sx);
    return AE_MOVAD16_0(AE_ROUND16X4F32SASYM(Y,Y));
}
