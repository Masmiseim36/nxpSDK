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
#include "scl_sine_table32.h"
#include "vec_recip_table.h"
#include "common.h"

/*===========================================================================
  Vector matematics:
  vec_tan             Tangent    
===========================================================================*/

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
void vec_tan24x24 (
              int32_t* restrict   z, 
              const f24 *restrict x,
              int N)
{
    int n;
    ae_int32x2 X,Y,E;
    ae_f32x2 f;
    xtbool2 sx;
    int32_t ALIGN(8) ss[MAX_ALLOCA_SZ/(2*sizeof(int32_t))];
    int32_t ALIGN(8) cc[MAX_ALLOCA_SZ/(2*sizeof(int32_t))];
    const ae_int32x2 * pc;
    const ae_f24x2 *   ps;
          ae_int32   * pz;

    while(N>0)
    {
        int M;
        M=MAX_ALLOCA_SZ/(2*sizeof(int32_t)); if (M>N) M=N;
        vec_sine24x24(ss,x,M);
        vec_cosine24x24(cc,x,M);
        pc=(const ae_int32x2 * )cc;
        ps=(const ae_f24x2 *   )ss;
        pz=(        ae_int32  *)z;
        for (n=0; n<(M&~1); n+=2)
        {
            ae_f24x2 t;
            ae_int32x2 Xl,Xh;
            ae_int32x2 _0x400000=AE_MOVDA32(0x400000);
            int nsal,nsah;
            AE_L32X2_IP(X,pc,8);
            sx=AE_LT32(X,AE_ZERO32());
            X=AE_ABS32S(X);
            /* normalize */
            Xl=X; Xh=AE_INTSWAP(X);
            nsal = AE_NSAZ32_L(Xl);
            nsah = AE_NSAZ32_L(Xh);
            Xl=AE_SLAA32(Xl,nsal);
            Xh=AE_SLAA32(Xh,nsah);
            X=AE_SEL32_LL(Xh,Xl);        
            /* find initial approximation of reciprocal */
            X=AE_SRAI32(X,8);
            Y=AE_SUB32(AE_MOVDA32(0x00BAEC00),X);
            /* 3 iterations to achieve 1 LSB accuracy in mantissa */
            f=AE_MOVF32X2_FROMINT32X2(_0x400000); AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(Y)); E=AE_MOVINT32X2_FROMF32X2(f);
            E=AE_SLLI32(E,1);
            f=AE_MOVF32X2_FROMINT32X2(Y); AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(E),AE_MOVF24X2_FROMINT32X2(Y)); Y=AE_MOVINT32X2_FROMF32X2(f);
            f=AE_MOVF32X2_FROMINT32X2(_0x400000); AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(Y)); E=AE_MOVINT32X2_FROMF32X2(f);
            E=AE_SLLI32(E,1);
            f=AE_MOVF32X2_FROMINT32X2(Y); AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(E),AE_MOVF24X2_FROMINT32X2(Y)); Y=AE_MOVINT32X2_FROMF32X2(f);
            f=AE_MOVF32X2_FROMINT32X2(_0x400000); AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(Y)); E=AE_MOVINT32X2_FROMF32X2(f);
            E=AE_SLLI32(E,1);
            f=AE_MOVF32X2_FROMINT32X2(Y); AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(E),AE_MOVF24X2_FROMINT32X2(Y)); Y=AE_MOVINT32X2_FROMF32X2(f);

            /* restore original sign */
            X=AE_NEG32(Y);
            AE_MOVT32X2(Y,X,sx);
            Y=AE_SAT24S(Y);
            AE_L32X2F24_IP(t,ps,8);
            f=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y),t);
            X=AE_MOVINT32X2_FROMF32X2(f);
            Xl=X;
            Xh=AE_INTSWAP(X);
            Xl=AE_SLAA32(Xl,nsal-7);
            Xh=AE_SLAA32(Xh,nsah-7);
            AE_S32_L_IP(Xh,pz,4);
            AE_S32_L_IP(Xl,pz,4);
        }
        if (M&1)
        {
            ae_f24x2 t;
            ae_int32x2 _0x400000=AE_MOVDA32(0x400000);
            int nsal;
            X=AE_L32_I((ae_int32*)pc,0);
            sx=AE_LT32(X,AE_ZERO32());
            X=AE_ABS32S(X);
            /* normalize */
            nsal = AE_NSAZ32_L(X);
            X=AE_SLAA32(X,nsal);
            /* find initial approximation of reciprocal */
            X=AE_SRAI32(X,8);
            Y=AE_SUB32(AE_MOVDA32(0x00BAEC00),X);
            /* 3 iterations to achieve 1 LSB accuracy in mantissa */
            f=AE_MOVF32X2_FROMINT32X2(_0x400000); AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(Y)); E=AE_MOVINT32X2_FROMF32X2(f);
            E=AE_SLLI32(E,1);
            f=AE_MOVF32X2_FROMINT32X2(Y); AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(E),AE_MOVF24X2_FROMINT32X2(Y)); Y=AE_MOVINT32X2_FROMF32X2(f);
            f=AE_MOVF32X2_FROMINT32X2(_0x400000); AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(Y)); E=AE_MOVINT32X2_FROMF32X2(f);
            E=AE_SLLI32(E,1);
            f=AE_MOVF32X2_FROMINT32X2(Y); AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(E),AE_MOVF24X2_FROMINT32X2(Y)); Y=AE_MOVINT32X2_FROMF32X2(f);
            f=AE_MOVF32X2_FROMINT32X2(_0x400000); AE_MULSFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(X),AE_MOVF24X2_FROMINT32X2(Y)); E=AE_MOVINT32X2_FROMF32X2(f);
            E=AE_SLLI32(E,1);
            f=AE_MOVF32X2_FROMINT32X2(Y); AE_MULAFP24X2RA(f,AE_MOVF24X2_FROMINT32X2(E),AE_MOVF24X2_FROMINT32X2(Y)); Y=AE_MOVINT32X2_FROMF32X2(f);

            /* restore original sign */
            X=AE_NEG32(Y);
            AE_MOVT32X2(Y,X,sx);
            Y=AE_SAT24S(Y);
            t=AE_L32F24_I((ae_f24*)ps,0);
            f=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y),t);
            X=AE_MOVINT32X2_FROMF32X2(f);
            X=AE_SLAA32(X,nsal-7);
            AE_S32_L_IP(X,pz,4);
        }
        /* next block */
        x+=M;
        z+=M;
        N-=M;
    }
}
