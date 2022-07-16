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
#include "scl_atan_table16.h"
#include "common.h"

/*===========================================================================
  Scalar matematics:
  scl_atan          Arctangent        
===========================================================================*/

/*-------------------------------------------------------------------------
  Arctangent 
  Functions calculate arctangent of number. Fixed point functions scale 
  output by pi which corresponds to the real phases +pi/4 and represent 
  input and output in Q15 or Q31
  NOTE:
  1.  Scalar floating point function is compatible with standard ANSI C
      routines and sets errno and exception flags accordingly

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: 2 LSB
  24x24  24-bit inputs, 24-bit output. Accuracy: 74000 (3.4e-5)
  32x32  32-bit inputs, 32-bit output. Accuracy: 42    (2.0e-8)
  f      floating point. Accuracy: 2 ULP


  Input:
  x[N]   input data, Q15, Q31 or floating point
  N      length of vectors
  Output:
  z[N]   result, Q15, Q31 or floating point

  Restriction:
  x,z should not overlap

  Scalar versions:
  ----------------
  return result, Q15, Q31 or floating point
-------------------------------------------------------------------------*/
/*
Reference MATLAB code:
% function calculates approx atan(x)/pi, x in Q15
function y=approx_atan16(x)
Q=2^15;
ord=5;
tbl=round(4*atan((0:2^ord+2)/(2^ord))/pi*Q);
tbl=min(Q-1,max(-Q,tbl));

sh=(15-ord);
idx=bitshift(x+2^(sh-1),-sh);
idx=bitshift(x,-sh);
dx=x-bitshift(idx,sh);
y0=tbl(idx+1);
y1=tbl(idx+2);

d=(2^sh);
y=y0+(y1-y0).*dx/d;
y=y/4;

y=round(y);
*/
f24 scl_atan24x24 (f24 x)
{
  int         off;
  const ae_int32 * restrict pt = (const ae_int32 *)atan_table16;

  int32_t     y;
  ae_int32x2  vnw,vzw,vxw,vdw,vyw;
  ae_int32x2  vy1,vy2,vy0;
  ae_f32x2    vaf,vbf,vxf;
  xtbool2     x_sgn;
  
  vnw = AE_MOVDA32X2(0x03fff8, 0x03fff8); //
  vzw = AE_MOVI(0);
  vxw = AE_MOVDA32X2(x,x);
  vxw = AE_SRAI32(vxw,8); //
  x_sgn = AE_LT32(vxw, vzw);
  vyw = AE_NEG32S(vxw);
  AE_MOVT32X2(vxw, vyw, x_sgn);
  vyw = AE_SRAI32(vxw,18); //
  vdw = AE_AND32(vxw, vnw);
  vdw = AE_SLAI32(vdw,13);

  off = AE_MOVAD32_H(vyw);
  pt += off;
  
  AE_L32_IP(vy0, pt, sizeof(*pt));//y0
  AE_L32_IP(vy1, pt, sizeof(*pt));//y1 

  vaf = (vdw);//d0
  vxf = (vy0);// y0
  vy2 = AE_SUB32(vy1,vy0);
  vbf = (vy2);
 
  AE_MULAFP32X2RAS(vxf, vbf, vaf);

  vyw = (vxf);
  vxw = AE_NEG32S(vyw);
  AE_MOVT32X2(vyw, vxw, x_sgn);
  y = AE_MOVAD32_H(vyw);
  return y;
}
