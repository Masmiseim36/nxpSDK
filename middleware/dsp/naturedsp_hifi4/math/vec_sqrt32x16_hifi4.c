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
  NatureDSP Signal Processing Library. Matrix Operations
    Square root 32x16   
    Code optimized for HiFi4
  IntegrIT, 2006-2018
*/
#include "NatureDSP_Signal_math.h"
#include "NatureDSP_types.h"
#include "polyrsqrtq23_tbl.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Square Root
  These routines calculate square root.
  NOTE: functions return 0x80000000 on negative argument for 32-bit outputs
  or 0x8000 for 16-bit outputs.
  Two versions of functions available: regular version (vec_sqrt16x16, vec_sqrt32x32, 
  vec_sqrt32x16, vec_sqrt64x32) with arbitrary 
  arguments and faster version (vec_sqrt32x32_fast) that 
  apply some restrictions.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: 2LSB
  32x32  32-bit inputs, 32-bit output. Accuracy: (2.6e-7*y+1LSB)
  32x16  32-bit input, 16-bit output.  Accuracy: 2 LSB
  64x32  64-bit inputs, 32-bit output. Accuracy: 2LSB

  Input:
  x[N]  input data, Q15, Q31, Q63 
  N     length of vectors
  Output:
  y[N]  output data, Q15, Q31

  Restriction:
  Regular versions (vec_sqrt16x16, vec_sqrt32x32, vec_sqrt32x16, vec_sqrt64x32):
  x,y - should not overlap

  Faster versions (vec_sqrt32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result, Q15, Q31
-------------------------------------------------------------------------*/
void vec_sqrt32x16      (int16_t* restrict y, const int32_t* restrict x, int N)
{
    const ae_int32 *pX;
    int n;
    if (N<0) return;
    if (N&1) { *y++=scl_sqrt32x16(*x++); N--; }
    pX=(const ae_int32 *)x;
    for (n=0; n<(N>>1); n++)
    {
        ae_int32x2 X0,X1,X,Y,D,R;
        ae_f32x2 f;
        xtbool2 lezero;
        int sh0,sh1;
        /* load, take exponent */
        AE_L32_IP(X0,pX,sizeof(ae_int32));
        AE_L32_IP(X1,pX,sizeof(ae_int32));
        sh0=AE_NSAZ32_L(X0)&~1;
        sh1=AE_NSAZ32_L(X1)&~1;
        X0=AE_SLAA32(X0,sh0-8);
        X1=AE_SLAA32(X1,sh1-8);
        X=AE_SEL32_LL(X0,X1);
        lezero=AE_LT32(X,0);
        /* compute rsqrt */
        R=polyrsqrtq23[0];
        f=polyrsqrtq23[1]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(R)); R=f;
        f=polyrsqrtq23[2]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(R)); R=f;
        f=polyrsqrtq23[3]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(R)); R=f;
        f=polyrsqrtq23[4]; AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(R)); R=f;
        /* reiterate rsqrt */
        R=AE_SLAI24S(AE_MOVF24X2_FROMINT32X2(R),3);
        D=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(R),AE_MOVF24X2_FROMINT32X2(R));
        f=0x80000; AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(D),AE_MOVF24X2_FROMINT32X2(X)); D=f;
        D=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(D),AE_MOVF24X2_FROMINT32X2(R));
        D=AE_SLAI32(D,3);
        R=AE_ADD24S(R,D);
        /* compute sqrt and rescale back */
        Y=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(R));
        X0=AE_SLAA32S(Y,14-(sh0>>1)-4);
        X1=AE_SLAA32S(Y,14-(sh1>>1)-4);
        X=AE_SEL32_HL(X0,X1);
        AE_MOVT32X2(X,0x80000000,lezero);
        *y++=AE_MOVAD16_3(AE_MOVINT16X4_FROMINT32X2(X));
        *y++=AE_MOVAD16_1(AE_MOVINT16X4_FROMINT32X2(X));
    }
}
