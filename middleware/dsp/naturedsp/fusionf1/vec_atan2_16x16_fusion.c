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
#include "polyatan16x16q23_tbl.h"
#include "common.h"

/*-------------------------------------------------------------------------
Floating-Point Full-Quadrant Arc Tangent
The functions compute the full quadrant arc tangent of the ratio y/x. 
Floating point functions output is in radians. Fixed point functions 
scale its output by pi.

NOTE:
1. Scalar function is compatible with standard ANSI C routines and set 
   errno and exception flags accordingly
2. Scalar function assigns EDOM to errno whenever y==0 and x==0.

Special cases:
     y    |   x   |  result   |  extra conditions    
  --------|-------|-----------|---------------------
   +/-0   | -0    | +/-pi     |
   +/-0   | +0    | +/-0      |
   +/-0   |  x    | +/-pi     | x<0
   +/-0   |  x    | +/-0      | x>0
   y      | +/-0  | -pi/2     | y<0
   y      | +/-0  |  pi/2     | y>0
   +/-y   | -inf  | +/-pi     | finite y>0
   +/-y   | +inf  | +/-0      | finite y>0
   +/-inf | x     | +/-pi/2   | finite x
   +/-inf | -inf  | +/-3*pi/4 | 
   +/-inf | +inf  | +/-pi/4   |

Input:
  y[N]  input data, Q15 or floating point
  x[N]  input data, Q15 or floating point
  N     length of vectors
Output:
  z[N]  result, Q15 or floating point
  
Restrictions:
x, y, z should not overlap
---------------------------------------------------------------------------*/
void vec_atan2_16x16 (int16_t   * restrict z, const int16_t   * restrict y, const int16_t   * restrict x,int N)
#if 1
{
    const ae_int32* restrict ptbl;
    const ae_int16x4 * restrict px;
    const ae_int16x4 * restrict py;
          ae_int16x4 * restrict pz;
    ae_valign ax,ay,az;
    ae_int16x4 x0,x1,x2,y0,z0;
    ae_int32x2 X0,X1,Y1,Y0,Z0,Z1,E0,E1,t;
    ae_f32x2 f0,f1;
    ae_f24x2 X20,X21;
    xtbool2 sx0,sx1,sy0,sy1,small0,small1;
    int ey0l,ey0h,ey1l,ey1h;
    int n;
    if (N<=0) return ;
    ptbl =(const ae_int32*)polyatan16x16q23;
    px=(const ae_int16x4 *)x;
    py=(const ae_int16x4 *)y;
    pz=(      ae_int16x4 *)z;
    ax = AE_LA64_PP(px);
    ay = AE_LA64_PP(py);
    az = AE_ZALIGN64();
    for (n=0; n<(N&~3); n+=4)
    {
        /* load, select quadrant */
        AE_LA16X4_IP(x0,ax,px);
        AE_LA16X4_IP(y0,ay,py);
        X0=AE_SEXT32X2D16_10(x0);
        X1=AE_SEXT32X2D16_32(x0);
        Y0=AE_SEXT32X2D16_10(y0);
        Y1=AE_SEXT32X2D16_32(y0);
        sx0=AE_LT32(X0,0);
        sx1=AE_LT32(X1,0);
        sy0=AE_LT32(Y0,0);
        sy1=AE_LT32(Y1,0);
        X0=AE_ABS32(X0); Y0=AE_ABS32(Y0); 
        X1=AE_ABS32(X1); Y1=AE_ABS32(Y1); 
        small0=AE_LT32(X0,Y0);
        small1=AE_LT32(X1,Y1);
        Z0=X0; X0=AE_MIN32(X0,Y0); Y0=AE_MAX32(Z0,Y0);
        Z1=X1; X1=AE_MIN32(X1,Y1); Y1=AE_MAX32(Z1,Y1);
        ey0l=AE_NSAZ32_L(Y0)-8; ey0h=AE_NSAZ32_L(AE_INTSWAP(Y0))-8;
        ey1l=AE_NSAZ32_L(Y1)-8; ey1h=AE_NSAZ32_L(AE_INTSWAP(Y1))-8;
        
        X0=AE_SEL32_HL(AE_SLAA32(X0,ey0h),AE_SLAA32(X0,ey0l));
        X1=AE_SEL32_HL(AE_SLAA32(X1,ey1h),AE_SLAA32(X1,ey1l));
        Y0=AE_SEL32_HL(AE_SLAA32(Y0,ey0h),AE_SLAA32(Y0,ey0l));
        Y1=AE_SEL32_HL(AE_SLAA32(Y1,ey1h),AE_SLAA32(Y1,ey1l));
        /* divide X/Y in Q23 via reciprocal */
        t=AE_L32_I(ptbl,4*sizeof(int32_t));
        Z0=AE_SUB32(t,Y0);
        Z1=AE_SUB32(t,Y1);
        t=AE_L32_I(ptbl,5*sizeof(int32_t));
        f0=f1=t;
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(Y0)); E0=f0;
        AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(Y1)); E1=f1;
        E0=AE_SLAI32(E0,1);
        E1=AE_SLAI32(E1,1);
        f0=Z0; AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        f1=Z1; AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
        f0=f1=t;
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(Y0)); E0=f0;
        AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(Y1)); E1=f1;
        E0=AE_SLAI32(E0,1);
        E1=AE_SLAI32(E1,1);
        f0=Z0; AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        f1=Z1; AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
        Z0=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0));
        Z1=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(X1));
        X0=AE_SLAI32(Z0,1);
        X1=AE_SLAI32(Z1,1);

        /* compute atan via polynomial */
        X20=AE_SAT24S(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),AE_MOVF24X2_FROMINT32X2(X0)));
        X21=AE_SAT24S(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X1),AE_MOVF24X2_FROMINT32X2(X1)));
        Y0=Y1=AE_L32_I(ptbl,0*sizeof(int32_t));
        f0=f1=AE_L32_I(ptbl,1*sizeof(int32_t));
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptbl,2*sizeof(int32_t)); 
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptbl,3*sizeof(int32_t)); 
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        Y0=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); 
        Y1=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); 

        /* move to the right octant */
        t=AE_L32_I(ptbl,5*sizeof(int32_t));
        AE_MOVT32X2(Y0,AE_SUB32(t,Y0),small0);
        AE_MOVT32X2(Y1,AE_SUB32(t,Y1),small1);
        t=AE_L32_I(ptbl,6*sizeof(int32_t));
        AE_MOVT32X2(Y0,AE_SUB32(t,Y0),sx0);
        AE_MOVT32X2(Y1,AE_SUB32(t,Y1),sx1);
        AE_MOVT32X2(Y0,AE_NEG32(Y0),sy0);
        AE_MOVT32X2(Y1,AE_NEG32(Y1),sy1);
        Y0=AE_SLAI32S(Y0,8);
        Y1=AE_SLAI32S(Y1,8);
        z0=AE_ROUND16X4F32SASYM(Y1,Y0);
        AE_SA16X4_IP(z0,az,pz);
    }
    AE_SA64POS_FP(az, pz);
    N&=3;
    if (N)
    {
        ae_int16 *pz_=(ae_int16 *)pz;
        /* load, select quadrant */
        AE_LA16X4_IP(x0,ax,px);
        AE_LA16X4_IP(y0,ay,py);
        X0=AE_SEXT32X2D16_10(x0);
        X1=AE_SEXT32X2D16_32(x0);
        Y0=AE_SEXT32X2D16_10(y0);
        Y1=AE_SEXT32X2D16_32(y0);
        sx0=AE_LT32(X0,0);
        sx1=AE_LT32(X1,0);
        sy0=AE_LT32(Y0,0);
        sy1=AE_LT32(Y1,0);
        X0=AE_ABS32(X0); Y0=AE_ABS32(Y0); 
        X1=AE_ABS32(X1); Y1=AE_ABS32(Y1); 
        small0=AE_LT32(X0,Y0);
        small1=AE_LT32(X1,Y1);
        Z0=X0; X0=AE_MIN32(X0,Y0); Y0=AE_MAX32(Z0,Y0);
        Z1=X1; X1=AE_MIN32(X1,Y1); Y1=AE_MAX32(Z1,Y1);
        ey0l=AE_NSAZ32_L(Y0)-8; ey0h=AE_NSAZ32_L(AE_INTSWAP(Y0))-8;
        ey1l=AE_NSAZ32_L(Y1)-8; ey1h=AE_NSAZ32_L(AE_INTSWAP(Y1))-8;
        
        X0=AE_SEL32_HL(AE_SLAA32(X0,ey0h),AE_SLAA32(X0,ey0l));
        X1=AE_SEL32_HL(AE_SLAA32(X1,ey1h),AE_SLAA32(X1,ey1l));
        Y0=AE_SEL32_HL(AE_SLAA32(Y0,ey0h),AE_SLAA32(Y0,ey0l));
        Y1=AE_SEL32_HL(AE_SLAA32(Y1,ey1h),AE_SLAA32(Y1,ey1l));
        /* divide X/Y in Q23 via reciprocal */
        t=AE_L32_I(ptbl,4*sizeof(int32_t));
        Z0=AE_SUB32(t,Y0);
        Z1=AE_SUB32(t,Y1);
        t=AE_L32_I(ptbl,5*sizeof(int32_t));
        f0=f1=t;
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(Y0)); E0=f0;
        AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(Y1)); E1=f1;
        E0=AE_SLAI32(E0,1);
        E1=AE_SLAI32(E1,1);
        f0=Z0; AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        f1=Z1; AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
        f0=f1=t;
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(Y0)); E0=f0;
        AE_MULSFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(Y1)); E1=f1;
        E0=AE_SLAI32(E0,1);
        E1=AE_SLAI32(E1,1);
        f0=Z0; AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        f1=Z1; AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(E1)); Z1=f1;
        Z0=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0));
        Z1=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Z1),AE_MOVF24X2_FROMINT32X2(X1));
        X0=AE_SLAI32(Z0,1);
        X1=AE_SLAI32(Z1,1);

        /* compute atan via polynomial */
        X20=AE_SAT24S(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),AE_MOVF24X2_FROMINT32X2(X0)));
        X21=AE_SAT24S(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X1),AE_MOVF24X2_FROMINT32X2(X1)));
        Y0=Y1=AE_L32_I(ptbl,0*sizeof(int32_t));
        f0=f1=AE_L32_I(ptbl,1*sizeof(int32_t));
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptbl,2*sizeof(int32_t)); 
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        f0=f1=AE_L32_I(ptbl,3*sizeof(int32_t)); 
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        AE_MULAFP24X2RA(f1,AE_MOVF24X2_FROMINT32X2(Y1),X21); Y1=f1;
        Y0=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); 
        Y1=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y1),AE_MOVF24X2_FROMINT32X2(X1)); 

        /* move to the right octant */
        t=AE_L32_I(ptbl,5*sizeof(int32_t));
        AE_MOVT32X2(Y0,AE_SUB32(t,Y0),small0);
        AE_MOVT32X2(Y1,AE_SUB32(t,Y1),small1);
        t=AE_L32_I(ptbl,6*sizeof(int32_t));
        AE_MOVT32X2(Y0,AE_SUB32(t,Y0),sx0);
        AE_MOVT32X2(Y1,AE_SUB32(t,Y1),sx1);
        AE_MOVT32X2(Y0,AE_NEG32(Y0),sy0);
        AE_MOVT32X2(Y1,AE_NEG32(Y1),sy1);
        Y0=AE_SLAI32S(Y0,8);
        Y1=AE_SLAI32S(Y1,8);
        z0=AE_ROUND16X4F32SASYM(Y1,Y0);
        x0 = AE_SEL16_6543(z0, z0); 
        x1 = AE_SEL16_5432(z0, z0);
        x2 = AE_SEL16_5432(x0, x0);
        AE_S16_0_IP(x0, pz_, 2);
        if(N>1) AE_S16_0_IP(x1, pz_, 2);
        if(N>2) AE_S16_0_IP(x2, pz_, 2);
    }
}
#else   /* shorter, but bit slower variant */
{
    const ae_int32* restrict ptbl;
    const ae_int16 * restrict px;
    const ae_int16 * restrict py;
          ae_int16 * restrict pz;
    ae_int16x4 x0,x1,y0,y1,z0;
    ae_int32x2 X0,Y0,Z0,E0,t;
    ae_f32x2 f0;
    ae_f24x2 X20;
    xtbool2 sx0,sy0,small0;
    int ey0l,ey0h;
    int n;
    if (N<=0) return ;
    ptbl =(const ae_int32*)polyatan16x16q23;
    px=(const ae_int16 *)x;
    py=(const ae_int16 *)y;
    pz=(      ae_int16 *)z;
    for (n=0; n<(N&~1); n+=2)
    {
        /* load, select quadrant */
        AE_L16_IP(x0,px,2);
        AE_L16_IP(x1,px,2);
        AE_L16_IP(y0,py,2);
        AE_L16_IP(y1,py,2);
        x0=AE_SEL16_6543(x1,x0);
        y0=AE_SEL16_6543(y1,y0);
        X0=AE_SEXT32X2D16_10(x0);
        Y0=AE_SEXT32X2D16_10(y0);
        sx0=AE_LT32(X0,0);
        sy0=AE_LT32(Y0,0);
        X0=AE_ABS32(X0); Y0=AE_ABS32(Y0); 
        small0=AE_LT32(X0,Y0);
        Z0=X0; X0=AE_MIN32(X0,Y0); Y0=AE_MAX32(Z0,Y0);
        ey0l=AE_NSAZ32_L(Y0)-8; ey0h=AE_NSAZ32_L(AE_INTSWAP(Y0))-8;
        
        X0=AE_SEL32_HL(AE_SLAA32(X0,ey0h),AE_SLAA32(X0,ey0l));
        Y0=AE_SEL32_HL(AE_SLAA32(Y0,ey0h),AE_SLAA32(Y0,ey0l));
        /* divide X/Y in Q23 via reciprocal */
        t=AE_L32_I(ptbl,4*sizeof(int32_t));
        Z0=AE_SUB32(t,Y0);
        t=AE_L32_I(ptbl,5*sizeof(int32_t));
        f0=t;
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(Y0)); E0=f0;
        E0=AE_SLAI32(E0,1);
        f0=Z0; AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        f0=t;
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(Y0)); E0=f0;
        E0=AE_SLAI32(E0,1);
        f0=Z0; AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        Z0=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0));
        X0=AE_SLAI32(Z0,1);

        /* compute atan via polynomial */
        X20=AE_SAT24S(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),AE_MOVF24X2_FROMINT32X2(X0)));
        Y0=AE_L32_I(ptbl,0*sizeof(int32_t));
        f0=AE_L32_I(ptbl,1*sizeof(int32_t));
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        f0=AE_L32_I(ptbl,2*sizeof(int32_t)); 
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        f0=AE_L32_I(ptbl,3*sizeof(int32_t)); 
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        Y0=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); 

        /* move to the right octant */
        t=AE_L32_I(ptbl,5*sizeof(int32_t));
        AE_MOVT32X2(Y0,AE_SUB32(t,Y0),small0);
        t=AE_L32_I(ptbl,6*sizeof(int32_t));
        AE_MOVT32X2(Y0,AE_SUB32(t,Y0),sx0);
        AE_MOVT32X2(Y0,AE_NEG32(Y0),sy0);
        Y0=AE_SLAI32S(Y0,8);
        z0=AE_ROUND16X4F32SASYM(Y0,Y0);
        /* save */
        AE_S16_0_I (AE_SEL16_6543(z0, z0), pz, 2);
        AE_S16_0_IP(z0, pz, 4);
    }

    if (N&1)
    {
        /* load, select quadrant */
        AE_L16_IP(x0,px,2);
        AE_L16_IP(y0,py,2);
        X0=AE_SEXT32X2D16_10(x0);
        Y0=AE_SEXT32X2D16_10(y0);
        sx0=AE_LT32(X0,0);
        sy0=AE_LT32(Y0,0);
        X0=AE_ABS32(X0); Y0=AE_ABS32(Y0); 
        small0=AE_LT32(X0,Y0);
        Z0=X0; X0=AE_MIN32(X0,Y0); Y0=AE_MAX32(Z0,Y0);
        ey0l=AE_NSAZ32_L(Y0)-8; ey0h=AE_NSAZ32_L(AE_INTSWAP(Y0))-8;
        
        X0=AE_SEL32_HL(AE_SLAA32(X0,ey0h),AE_SLAA32(X0,ey0l));
        Y0=AE_SEL32_HL(AE_SLAA32(Y0,ey0h),AE_SLAA32(Y0,ey0l));
        /* divide X/Y in Q23 via reciprocal */
        t=AE_L32_I(ptbl,4*sizeof(int32_t));
        Z0=AE_SUB32(t,Y0);
        t=AE_L32_I(ptbl,5*sizeof(int32_t));
        f0=t;
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(Y0)); E0=f0;
        E0=AE_SLAI32(E0,1);
        f0=Z0; AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        f0=t;
        AE_MULSFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(Y0)); E0=f0;
        E0=AE_SLAI32(E0,1);
        f0=Z0; AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(E0)); Z0=f0;
        Z0=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Z0),AE_MOVF24X2_FROMINT32X2(X0));
        X0=AE_SLAI32(Z0,1);

        /* compute atan via polynomial */
        X20=AE_SAT24S(AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(X0),AE_MOVF24X2_FROMINT32X2(X0)));
        Y0=AE_L32_I(ptbl,0*sizeof(int32_t));
        f0=AE_L32_I(ptbl,1*sizeof(int32_t));
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        f0=AE_L32_I(ptbl,2*sizeof(int32_t)); 
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        f0=AE_L32_I(ptbl,3*sizeof(int32_t)); 
        AE_MULAFP24X2RA(f0,AE_MOVF24X2_FROMINT32X2(Y0),X20); Y0=f0;
        Y0=AE_MULFP24X2RA(AE_MOVF24X2_FROMINT32X2(Y0),AE_MOVF24X2_FROMINT32X2(X0)); 

        /* move to the right octant */
        t=AE_L32_I(ptbl,5*sizeof(int32_t));
        AE_MOVT32X2(Y0,AE_SUB32(t,Y0),small0);
        t=AE_L32_I(ptbl,6*sizeof(int32_t));
        AE_MOVT32X2(Y0,AE_SUB32(t,Y0),sx0);
        AE_MOVT32X2(Y0,AE_NEG32(Y0),sy0);
        Y0=AE_SLAI32S(Y0,8);
        z0=AE_ROUND16X4F32SASYM(Y0,Y0);
        /* save */
        AE_S16_0_IP(z0, pz, 4);
    }
}
#endif
