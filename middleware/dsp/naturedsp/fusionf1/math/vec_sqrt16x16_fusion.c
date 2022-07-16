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
#include "polyrsqrtq23_tbl.h"

/*-------------------------------------------------------------------------
  Square Root
  These routines calculate square root.
  NOTES: 
  1. Fixed point functions return 0x80000000 (for 24 and 32-bit functions), 
     0x8000 (for 16-bit functions) on negative argument
  2. For floating point function, whenever an input value is negative, 
     functions raise the "invalid" floating-point exception, assign EDOM 
     to errno and set output value to NaN. Negative zero is considered as
     a valid input, the result is also -0
  Two versions of functions available: regular version (vec_sqrt16x16, 
  vec_sqrt24x24, vec_sqrt32x32, vec_sqrtf) with arbitrary arguments and
  faster version (vec_sqrt24x24_fast, vec_sqrt32x32_fast) that apply some 
  restrictions.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: (2 LSB)
  24x24  24-bit inputs, 24-bit output. Accuracy: (2.6e-7*y+1LSB)
  32x32  32-bit inputs, 32-bit output. Accuracy: (2.6e-7*y+1LSB)
  f      floating point. Accuracy 1 ULP

  Input:
  x[N]  input data,Q31, Q15 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31, Q15 or floating point

  Restriction:
  Regular versions (vec_sqrt24x24, vec_sqrt32x32):
  x,y - should not overlap

  Faster versions (vec_sqrt24x24_fast, vec_sqrt32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result, Q31, Q15 or floating point
-------------------------------------------------------------------------*/
void vec_sqrt16x16(int16_t * z, const int16_t * x,  int N )
{
    const ae_p16s * restrict px;
          ae_p16s * restrict pz;
    ae_int32x2 X0,X1,X,R,D,Y;
    ae_f32x2 f;
    xtbool2 lezero;
    int sh0,sh1;
    int n;
    NASSERT(x);
    NASSERT(z);
    if(N<=0) return;
    px=(const ae_p16s *)(x-1);
    pz=(      ae_p16s *)(z-1);
    for(n=0; n<(N&~1); n+=2)
    {
        /* load, take exponent */
        X0=AE_L16M_I (px,  sizeof(int16_t));
        AE_L16M_IU(X1,px,2*sizeof(int16_t));
        sh0=AE_NSAZ32_L(X0)&~1;
        sh1=AE_NSAZ32_L(X1)&~1;
        X0=AE_SLAA32(X0,sh0-8);
        X1=AE_SLAA32(X1,sh1-8);
        X =AE_SEL32_HH(X0,X1);
        lezero=AE_LE32(X,0);
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
        X0=AE_SLAA32S(Y,14-(sh0>>1));
        X1=AE_SLAA32S(Y,14-(sh1>>1));
        X=AE_SEL32_HL(X0,X1);
        AE_MOVT32X2(X,0,lezero);
        X=AE_SRAI32(X,8);
        AE_S16M_L_I (AE_INTSWAP(X),pz,  sizeof(int16_t));
        AE_S16M_L_IU(           X ,pz,2*sizeof(int16_t));
        //X0=X0; __MNSD__
    }
    if(N&1)
    {
        /* load, take exponent */
        X0=AE_L16M_I (px, sizeof(int16_t));
        sh0=AE_NSAZ32_L(X0)&~1;
        X0=AE_SLAA32(X0,sh0-8);
        X =X0;
        lezero=AE_LE32(X,0);
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
        X0=AE_SLAA32S(Y,14-(sh0>>1));
        X=X0;
        AE_MOVT32X2(X,0,lezero);
        X=AE_SRAI32(X,8);
        AE_S16M_L_I(X ,pz,sizeof(int16_t));
    }
} /* vecsqrt_16b() */
