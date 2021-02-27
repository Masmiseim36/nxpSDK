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
  Scalar matematics:
  scl_tan             Tangent    
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
int32_t scl_tan32x32 (int32_t x)
{
    
  int         ind,y_nsa;
  const ae_int32   * restrict ptbl1  = (const ae_int32   *)sine_table32;
  const ae_p16x2s * restrict ptbl2  = (const ae_p16x2s *)recip_table;

  uint32_t    off;
  ae_int32x2  vzw,vaw,vxw,vnw,vlw,vkw,vrw,vdel,vcos,vsin,vww,vtt;
  ae_int32x2  vdw,vcw,vsw;
  ae_f32x2    vsf,vdf,vxf,vdelf,vaf,vbf,vcf,vzf;
  ae_f32x2    vyf;
  ae_int32x2  vyw, y1,vmw,www;
  xtbool2     sgya;
  ae_int32x2  vya,vy1,vyy;
  ae_int32x2  t1,t2;
  ae_f32x2    vrf;
  int32_t     y;

  vzw = AE_MOVI(0);
  www = AE_MOVDA32X2(0xc91000, 0xc91000); //pi,Q22
  vww = AE_MOVDA32X2(0xFFFFFFFC, 0xFFFFFFFC);
  vnw = AE_MOVDA32X2(16384, 16384);
  vtt = AE_MOVDA32X2(0x400000, 0x400000); //1<<(22)
  vlw = AE_MOVDA32X2(0x200, 0x200); //1<<(9)/4
  vkw = AE_MOVDA32X2(0x7fc, 0x7fc); //1<<(9)-1
  vmw = AE_MOVDA32X2(0xE0000000, 0xE0000000); //-1<<(29)

  vxw = AE_MOVDA32X2(x,x); //x*pi/4
  vaw = AE_ADD32S(vxw,vtt);
      
  vaw = AE_SRLI32(vaw,21);
  vaw = AE_AND32(vaw, vww);

  off = AE_MOVAD32_H(vaw);
  /*Load sine */
  vsin = AE_L32_X(ptbl1, off); //sin

  vdw = AE_SLAI32(vaw,21);
  vdw = AE_SUB32(vxw, vdw);

  vaf = (vdw);
  vbf = (www);
   
  vdelf = AE_MULFP32X2RAS(vaf, vbf);//
  vdel = (vdelf);

  vaw = AE_ADD32S(vaw,vlw);
  vaw = AE_AND32(vaw, vkw);
  off = AE_MOVAD32_H(vaw);
  /*Load cosine*/
  vcos = AE_L32_X(ptbl1, off); //cos vxa
  vsf = (vsin);//sin
  vrw = AE_SLAI32(vdel, 8); //
  vdf = (vrw);//1/2*dx
  vdel = AE_SLAI32(vdel, 9); //dx,Q31

  vxf = (vcos);
  vcf = (vsin);
  AE_MULAFP32X2RAS(vcf, vxf, vdf);//sin+1/2*cos*dx
  AE_MULSFP32X2RAS(vxf, vsf, vdf);//cos - 1/2*sin*dx

  vdf = (vdel);//dx
  vzf = (vcos);//dx
  AE_MULAFP32X2RAS(vsf, vxf, vdf); //y = sin(x0)+cos(x0)dx-1/2*sin(x0)*dx^2,Q31
  AE_MULSFP32X2RAS(vzf, vcf, vdf); //y = cos(x0)-sin(x0)dx-1/2*cos(x0)*dx^2,Q31
  vsw = (vsf);//sin
  vcw = (vzf);//cos
  /*Divide*/
  sgya = AE_LT32(vcw, vzw);
  vya = AE_ABS32S(vcw);
  /*Normalize y*/
  y_nsa = AE_NSAZ32_L(vya);
  vyy = AE_SLAA32S( vya, y_nsa );//y in 0.5..1 
  y_nsa =y_nsa-15;
  /*Calculate reciprocal y for the 16 high-order bits
  recip = f(y0)+f'(y0)*dy
  recip = (1/y0)-(1/y0^2)*(y-y0)
  */
  y1 = AE_SRAI32(vyy,16);//y,Q1.14
  y1 = AE_SUB32(y1,vnw);

  vdw = AE_SRAI32(y1,5);
  vdw = AE_AND32(vdw, vww);

  ind = AE_MOVAD32_H(vdw);
  vy1 = AE_L16X2M_X(ptbl2, ind);//1
  t2 = AE_SEL32_LL(vy1,vy1);//f'
  t1 = AE_SEL32_HH(vy1,vy1);//f

  vdw = AE_SLAI32(vdw,5);
  vdw = AE_SUB32(y1,vdw);//dy
  vdw = AE_SLAI32(vdw,9);
  vdf = (vdw);
  vaf = (t1);
  vbf = (t2);
  vbf = AE_MULFP32X2RAS(vbf,vdf);//(1/y^2)*dy
  vyw = (vbf);
  vyw = AE_SLAI32(vyw, 8); //
  vy1 = AE_SUB32S(t1,vyw);

  /*refinement*/
  y1 = AE_SLAI32(vy1,7);  //Q30
  vy1 = AE_SLAI32(vy1,8);
  vyf = (y1);
  vxf = (vyy);//y,Q1.30
  vzf = (vmw);
  AE_MULAFP32X2RAS(vzf,vxf,vyf);//err=(1/y)*y-1,Q1.29
  t2 = (vzf);

  t2 = AE_SLAI32(t2,2);
  vzf = (t2);

  vyf = (vy1);
  AE_MULSFP32X2RAS(vyf,vyf,vzf);   //Q31 y0-err*y0 
  t2 = (vyf);

  vrf = (t2);//y
  vxf = (vsw);//x
  vrf = AE_MULFP32X2RAS(vrf,vxf);//(x/y)
  t2 = (vrf);
  t1 = AE_NEG32(t2);
  AE_MOVT32X2(t2, t1, sgya);
  vrw = AE_SLAA32S(t2,y_nsa);
  y = AE_MOVAD32_H(vrw);
  return y;
}
