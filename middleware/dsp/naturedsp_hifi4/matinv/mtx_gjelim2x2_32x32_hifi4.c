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
 * Real Matrix Gauss-Jordan Elimination for linear equation problem, 32-bit 
   fixed point API
 * Optimized code for HiFi4
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Matrix functions */
#include "NatureDSP_Signal_matinv.h"

/*-------------------------------------------------------------------------
  These functions implement Gauss elimination elimination process with full 
  pivoting to findsolution of linear equations A*y=x
  
  Fixed point version takes representation of input matrix/vector and forms 
  representation of output vector with proper scaling.

  Precision: 
  32x32 32-bit input, 32-bit output

  Input:
  A[N*N]      input matrix, representation is defined by parameter qA
  x[N]        input rigth side of equation, representation is defined by 
              parameter qX
  qA          input matrix A representation (for fixed point API only)
  qX          input vector x representation (for fixed point API only)
  Output:
  y[N]        output vector
  Temporary:
  pScr        scratch memory. Size in bytes is defined by corresponding 
              scratch allocation function 
  N is 2,3,4,6,8,10

  Returned value: fixed point functions return fixed-point representation 
                  of resulted vector
  Restrictions:
  none
-------------------------------------------------------------------------*/
int  mtx_gjelim2x2_32x32  (void* pScr, int32_t *y, const int32_t* A,const int32_t * x, int qA, int qX) 
{
    ae_valign ax,aa,ay;
    ae_int32x2 *pA;
    const ae_int32x2 *pX;
          ae_int32x2 *pY;
    ae_int32x2 ab,cd,x01,t,r,y01,da,bc;
    ae_int64 det;
    int eX,eA,eDet;
    (void)pScr;
    pY=(ae_int32x2 *)y;
    pA=(ae_int32x2 *)A;
    aa=AE_LA64_PP(pA);
    pX=(ae_int32x2 *)x;
    ax=AE_LA64_PP(pX);
    AE_LA32X2_IP(ab,aa,pA);
    AE_LA32X2_IP(cd,aa,pA);
    AE_LA32X2_IP(x01,ax,pX);
    t=AE_ABS32S(x01);
    t=AE_MAX32(AE_SEL32_LH(t,t),t);
    eX=AE_NSAZ32_L(t)-1;
    x01=AE_SLAA32S(x01,eX);
    r=AE_MAXABS32S(ab,cd);
    r=AE_MAX32(AE_SEL32_LH(r,r),r);
    eA=AE_NSAZ32_L(r);
    det=AE_MULZASD32_HL_LH(ab,cd);
    eDet=AE_NSA64(det);
    r=AE_TRUNCA32X2F64S(det,det,eDet);
    {   // reciprocal
        ae_int32x2 x,y,e;
        xtbool2 sx;
        x=r;
        sx=AE_LT32(x,AE_ZERO32());
        x=AE_ABS32S(x);
        y=AE_SUB32(0xBAEC0000,x);
        e=0x40000000; AE_MULSFP32X2RAS(e,x,y);  AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
        e=0x40000000; AE_MULSFP32X2RAS(e,x,y);  AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
        e=0x40000000; AE_MULSFP32X2RAS(e,x,y);  AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
        e=0x40000000; AE_MULSFP32X2RAS(e,x,y);  AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
        AE_MOVT32X2(y,AE_NEG32(y),sx);
        r=y;
    }
    ab=AE_SLAA32(ab,eA);
    cd=AE_SLAA32(cd,eA);

    da=AE_SEL32_LH(cd,ab);
    bc=AE_SEL32_LH(ab,cd);

    y01=AE_MULFP32X2RAS(da,x01);
    AE_MULSFP32X2RAS(y01,bc,AE_SEL32_LH(x01,x01));
    y01=AE_MULFP32X2RAS(y01,r);
    ay=AE_ZALIGN64();
    AE_SA32X2_IP(y01,ay,pY);
    AE_SA64POS_FP(ay,pY);
    return 31+eA+qX+eX-qA-eDet;
}

size_t mtx_gjelim2x2_32x32_getScratchSize   () { return 0;  }
