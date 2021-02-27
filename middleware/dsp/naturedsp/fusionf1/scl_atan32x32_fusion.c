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
#include "scl_atan_table.h"
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
ord=7;
Q=2^31;
tbl=round(2*atan((0:2^ord+2)/(2^ord))/pi*Q);
tbl=min(Q-1,max(-Q,tbl));

sh=(31-ord);
idx=bitshift(x,-sh);
y0=tbl(idx+1);
y1=tbl(idx+2);
y2=tbl(idx+3);
dx=x-bitshift(idx,sh);

d=(2^sh);
y=y0+(y1-y0).*dx/d;

y  = (dx  ).*(dx-d  )./(2*d^2).*y2 +...
     (dx-d).*(dx-2*d)./(2*d^2).*y0 +...
     (dx  ).*(dx-2*d)./(-d^2) .*y1;
y=y/2;
y=round(y);

*/

int32_t scl_atan32x32(int32_t x) 
{
    int off;
    int32_t y;
    ae_int32x2 vnw,vkw,vlw,vzw,vxw,vdw,vyw,vaw,vbw,vsw,vcw;
    ae_int32x2  vy1,vy2,vy0;
    ae_f32x2    vaf,vbf,vdf,vxf,vcf;
    ae_f64      vhf;
    ae_int64    vyh;
    xtbool2     x_sgn;
    
    vnw = AE_MOVDA32X2(0x1FFFFFFF, 0x1FFFFFFF); //
    vsw = AE_MOVDA32X2(0xFFFFFFFC, 0xFFFFFFFC); //
    vkw = AE_MOVDA32X2(0x20000000, 0x20000000); //
    vlw = AE_MOVDA32X2(0x40000000, 0x40000000); //
    vzw = AE_MOVI(0);
    vxw = AE_MOVDA32X2(x,x);
    x_sgn = AE_LT32(vxw, vzw);
    vyw = AE_NEG32S(vxw);
    AE_MOVT32X2(vxw, vyw, x_sgn);
    vyw = AE_SRAI32(vxw,22); //
    vyw = AE_AND32(vyw,vsw);//offset
    vdw = AE_SLAI32(vxw,5);
    
    vaw = AE_AND32(vdw,vnw);//d0
    vbw = AE_SUB32(vaw,vkw);//d1
    vcw = AE_SUB32(vaw,vlw);//d2
    
    off = AE_MOVAD32_H(vyw);
    vy0 = AE_L32_X((const ae_int32 *)atan_table, off); 
    off +=4;
    vy1 = AE_L32_X((const ae_int32 *)atan_table, off); 
    off +=4;
    vy2 = AE_L32_X((const ae_int32 *)atan_table, off); 

    vaf = (vaw);//d0
    vbf = (vbw);//d1
    vcf = (vcw);//d2

    vdf = AE_MULFP32X2RAS(vcf, vbf);//d1*d2
    vxf = (vy0);// y0

    vhf = AE_MULF32S_HH(vdf,vxf);//t*y0
    vdf = AE_MULFP32X2RAS(vaf, vbf);//d0*d1
    vxf = (vy2);// y2
    AE_MULAF32S_HH(vhf,vdf,vxf);
    vyh = (vhf);
    vyh = AE_SRAI64(vyh, 1);
    vdf = AE_MULFP32X2RAS(vaf, vcf);//d0*d2
    vhf = (vyh);
    vxf = (vy1);// y1
    AE_MULSF32S_HH(vhf,vdf,vxf);
    vyh = (vhf);
    vyh = AE_SRAI64(vyh, 29);
    vyw = AE_MOVINT32X2_FROMINT64(vyh);
    vyw = AE_SEL32_LL(vyw,vyw);
    vxw = AE_NEG32S(vyw);
    AE_MOVT32X2(vyw, vxw, x_sgn);
    y = AE_MOVAD32_H(vyw);
    return y;
}
