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
  Vector matematics:
  vec_atan          Arctangent        
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

#include "common.h"
#include "baseop.h"
void vec_atan24x24 (f24 * restrict z, 
              const f24 * restrict x, 
              int N )
{
  int         off,n,ofq;
        ae_int32x2 * restrict pz = (      ae_int32x2 *)z;
  const ae_f24x2 * restrict px = (const ae_f24x2 *)x;
  const ae_int32 *  pt = (const ae_int32 *)atan_table16;
  const ae_int32 *  pq = (const ae_int32 *)atan_table16;


  ae_valign x_align, z_align;

  ae_int32x2  vnw,vzw,vxw,vdw,vyw,vrw,vtw;
  ae_int32x2  vy1,vy0,vq0,vq1;
  ae_f32x2    vaf,vbf,vxf;
  ae_f24x2    vxl;
  xtbool2     x_sgn;
  if (N <= 0) return;
  x_align = AE_LA64_PP(px);
  z_align = AE_ZALIGN64();
  vnw = AE_MOVDA32X2(0x0003fff8, 0x0003fff8); //
  vzw = AE_MOVI(0);
  
  #ifdef COMPILER_XTENSA
    #pragma concurrent
  #endif
  for (n=0;n<N-1;n+=2) 
  {
    AE_LA32X2F24_IP(vxl, x_align, px);  
    vxw = (vxl); //
    x_sgn = AE_LT32(vxw, vzw);
    vyw = AE_NEG32S(vxw);
    AE_MOVT32X2(vxw, vyw, x_sgn);
    vyw = AE_SRAI32(vxw,18); //
    vdw = AE_AND32(vxw, vnw);
    vdw = AE_SLAI32(vdw,13);

    off = AE_MOVAD32_H(vyw);
    ofq = AE_MOVAD32_L(vyw);
    pt += off;
    pq += ofq;
    off = 0;
    
    AE_L32_IP(vy0, pt, +4);//y0
    AE_L32_IP(vy1, pt, -4);//y1 
    AE_L32_IP(vq0, pq, +4);//y0
    AE_L32_IP(vq1, pq, -4);//y1 

    vy0 = AE_SEL32_HH(vy0,vq0);//y0 Q29
    vy1 = AE_SEL32_LL(vy1,vq1);//y1 Q29

    vaf = (vdw);//d0
    vxf = (vy0);// y0
    vy1 = AE_SUB32(vy1,vy0);

    vbf = (vy1);//y1-y0
    AE_MULAFP32X2RAS(vxf, vbf, vaf);//

    vrw = (vxf);

    vtw = AE_NEG32S(vrw);
    AE_MOVT32X2(vrw, vtw, x_sgn);
    AE_SA32X2_IP(vrw, z_align, pz);//put answer

    pt = (const ae_int32 *)atan_table16;
    pq = (const ae_int32 *)atan_table16;

  }
  AE_SA64POS_FP(z_align, pz);
  
  if (N&1)
  {
    AE_L32F24_IP(vxl, castxcc(ae_f24,px), 0);
    vxw = (vxl);
    x_sgn = AE_LT32(vxw, vzw);
    vyw = AE_NEG32S(vxw);
    AE_MOVT32X2(vxw, vyw, x_sgn);
    vyw = AE_SRAI32(vxw,18); //
    vdw = AE_AND32(vxw, vnw);
    vdw = AE_SLAI32(vdw,13);

    off = AE_MOVAD32_H(vyw);
    pt += off;
    AE_L32_IP(vy0, pt, +4);//y0
    AE_L32_IP(vy1, pt, -4);//y1 
    pt -= off;

    vaf = (vdw);//d0
    vxf = (vy0);// y0
    vy1 = AE_SUB32(vy1,vy0);
    vbf = (vy1);//y1-y0
    
    AE_MULAFP32X2RAS(vxf, vbf, vaf);

    vyw = (vxf);
    vxw = AE_NEG32S(vyw);
    AE_MOVT32X2(vyw, vxw, x_sgn);
    AE_S32_L_IP(vyw, castxcc(ae_int32,pz), +4);
  }
}
