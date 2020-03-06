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
 * Real Matrix Inversion 2x2, 32-bit fixed point API
 * Optimized code for HiFi4
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Matrix functions */
#include "NatureDSP_Signal_matinv.h"

/*-------------------------------------------------------------------------
  These functions implement in-place matrix inversion by Gauss elimination 
  with full pivoting
  NOTE: user may detect "invalid" or "divide-by-zero" exception in the CPU 
  flags which MAY indicate that inversion results are not accurate. Also 
  it's responsibility of the user to provide valid input matrix for 
  inversion.
  Fixed point version takes representation of input matrix and forms 
  representation of output matrix with proper scaling.

  Precision: 
  f     floating point
  32x32 32-bit input, 32-bit output

  Input:
  x[N*N]      input matrix
  qX          input matrix representation (for fixed point API only)
  Output:
  x[N*N]      result
  Temporary:
  pScr        scratch memory. Size in bytes is defined by corresponding 
              scratch allocation function 
  N is 2,3,4,6,8,10

  Returned value: floating functions return none, fixed point functions 
                  return fixed-point representation of inverted matrix
  Restrictions:
  none
-------------------------------------------------------------------------*/
int  mtx_inv2x2_32x32  (void* pScr, int32_t *x, int qX) 
{
    ae_valign ax;
    ae_int32x2 *pX;
    ae_int32x2 ab,cd,r,db,ca;
    ae_int64 det;
    int eX,eDet;
    pX=(ae_int32x2 *)x;
    ax=AE_LA64_PP(pX);
    AE_LA32X2_IP(ab,ax,pX);
    AE_LA32X2_IP(cd,ax,pX);

    r=AE_MAXABS32S(ab,cd);
    r=AE_MAX32(AE_SEL32_LH(r,r),r);
    eX=AE_NSAZ32_L(r);
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
    AE_MOVT32X2(r,AE_NEG32(r),1);

    ab=AE_SLAA32(ab,eX);
    cd=AE_SLAA32(cd,eX);

    db=AE_SEL32_LL(cd,ab);
    ca=AE_SEL32_HH(cd,ab);
    db=AE_MULFP32X2RAS(db,r);
    ca=AE_MULFP32X2RAS(ca,AE_NEG32(r));

    ax=AE_ZALIGN64();
    pX=(ae_int32x2 *)x;
    AE_SA32X2_IP(db,ax,pX);
    AE_SA32X2_IP(ca,ax,pX);
    AE_SA64POS_FP(ax,pX);
    return (62-qX)+eX-eDet;
}

//{ return mtx_inv_32x32(pScr,x,2,qX); }
size_t mtx_inv2x2_32x32_getScratchSize   () { return 0; }
