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
#include "common.h"

#include "baseop.h"

/*===========================================================================
  Vector matematics:
  vec_shift,vec_scale  Vector Scaling with Saturation
===========================================================================*/

/*-------------------------------------------------------------------------
  Vector Scaling with Saturation
  These routines make shift with saturation of data values in the vector 
  by given scale factor (degree of 2). 24-bit routine works with f24 data 
  type and faster while 32-bit version keep all 32-bits and slower.
  Functions vec_scale() make multiplication of Q31 vector to Q31 coefficient 
  which is not a power of 2 forming Q31 result.
  Two versions of routines are available: regular versions (vec_shift24x24, 
  vec_shift32x32, vec_shift16x16, vec_shiftf, vec_scale32x24, vec_scale24x24, 
  vec_scale16x16, vec_scalef, vec_scale_sf) work with arbitrary arguments, faster 
  versions (vec_shift24x24_fast, vec_shift32x32_fast, vec_shift16x16_fast, 
  vec_scale32x24_fast, vec_scale24x24_fast, vec_scale16x16_fast) apply some 
  restrictions
For floating point:
  Fuction vec_shiftf() makes scaling without saturation of data values by given 
  scale factor (degree of 2). 
  Functions vec_scalef() and vec_scale_sf() make multiplication of input vector
  to coefficient which is not a power of 2.
  Two versions of routines are available: 
    without saturation - vec_scalef;
    with saturation - vec_scale_sf; 

Precision:
24x24 24-bit input, 32-bit output
32x32 32-bit input, 32-bit output
16x16 16-bit input, 16-bit output
f     single precision floating point

  Input:
  x[N]  input data , Q31,Q15 or floating point
  t     shift count. If positive, it shifts left with saturation, if
  negative it shifts right
  s     scale factor, Q31,Q15 or floating point
  N     length of vector
  fmin  minimum output value (only for vec_scale_sf)
  fmax  maximum output value (only for vec_scale_sf)

  Output:
  y[N]  output data, Q31 or Q15

  Restrictions:
  For regular versions (vec_shift24x24, vec_shift32x32, vec_shift16x16, 
  vec_shiftf, vec_scale32x24, vec_scale24x24, vec_scale16x16, vec_scalef, 
  vec_scale_sf):
  x,y should not overlap
  t   should be in range -31...31 for fixed-point functions and -129...146 
      for floating point

  For vec_scale_sf - fmin<=fmax;

  For faster versions (vec_shift24x24_fast, vec_shift32x32_fast, 
  vec_shift16x16_fast, vec_scale32x24_fast, vec_scale24x24_fast, 
  vec_scale16x16_fast):
  x,y should not overlap
  t should be in range -31...31 
  x,y - aligned on 8-byte boundary
  N   - multiple of 4 
-------------------------------------------------------------------------*/
void vec_scale16x16 (     int16_t * restrict y,
                    const int16_t * restrict x,
                    int16_t s,
                    int N)
#if XCHAL_HAVE_FUSION_AVS
{
  int n;

  ae_int16x4  vx0h, vx1h, vy0h, vy1h, vch;
  ae_f16x4    vx0f, vx1f, vy0f, vy1f, vcf;
  const ae_int16x4 * restrict px = (const ae_int16x4 *)x;
        ae_int16x4 * restrict py = (      ae_int16x4 *)y;
  ae_valign x_align, y_align;

  NASSERT(x);
  NASSERT(y);
  if (N <= 0) return;

  x_align = AE_LA64_PP(px);
  y_align = AE_ZALIGN64();

  vch = AE_MOVDA16(s);
  vcf = (vch);

  for (n=0; n<(N>>3); n++)
  { 
    AE_LA16X4_IP(vx0h, x_align, px);
    AE_LA16X4_IP(vx1h, x_align, px);

    vx0f = (vx0h);
    vy0f = AE_MULFP16X4RAS(vx0f, vcf);
    vy0h = (vy0f);
    AE_SA16X4_IP(vy0h, y_align, py);

    vx1f = (vx1h);
    vy1f = AE_MULFP16X4RAS(vx1f, vcf);
    vy1h = (vy1f);
    AE_SA16X4_IP(vy1h, y_align, py);
  }
  if (N&4)
  {
    AE_LA16X4_IP(vx0h, x_align, px);
    vx0f = (vx0h);
    vy0f = AE_MULFP16X4RAS(vx0f, vcf);
    vy0h = (vy0f);
    AE_SA16X4_IP(vy0h, y_align, py);
  }

  AE_SA64POS_FP(y_align, py);

  switch(N&3)
  {
  case 1:
    AE_L16_IP(vx0h,castxcc(ae_int16,px),0);
    vx0f = (vx0h);
    vy0f = AE_MULFP16X4RAS(vx0f, vcf);
    vy0h = (vy0f);
    AE_S16_0_I(vy0h, (ae_int16 *)py, 0);
    break;
  case 2:
    AE_L16_IP(vx0h,castxcc(ae_int16,px),sizeof(ae_int16));
    vx0f = (vx0h);
    vy0f = AE_MULFP16X4RAS(vx0f, vcf);
    vy0h = (vy0f);
    AE_S16_0_I(vy0h, (ae_int16 *)py, 0);
    AE_L16_IP(vx0h,castxcc(ae_int16,px),0);
    vx0f = (vx0h);
    vy0f = AE_MULFP16X4RAS(vx0f, vcf);
    vy0h = (vy0f);
    AE_S16_0_I(vy0h, (ae_int16 *)py, sizeof(ae_int16));
    break;
  case 3:
    AE_L16_IP(vx0h,castxcc(ae_int16,px),sizeof(ae_int16));
    vx0f = (vx0h);
    vy0f = AE_MULFP16X4RAS(vx0f, vcf);
    vy0h = (vy0f);
    AE_S16_0_I(vy0h, (ae_int16 *)py, 0);
    AE_L16_IP(vx0h,castxcc(ae_int16,px),sizeof(ae_int16));
    vx0f = (vx0h);
    vy0f = AE_MULFP16X4RAS(vx0f, vcf);
    vy0h = (vy0f);
    AE_S16_0_I(vy0h, (ae_int16 *)py, sizeof(ae_int16));
    AE_L16_IP(vx0h,castxcc(ae_int16,px),0);
    vx0f = (vx0h);
    vy0f = AE_MULFP16X4RAS(vx0f, vcf);
    vy0h = (vy0f);
    AE_S16_0_I(vy0h, (ae_int16 *)py, 2*sizeof(ae_int16));
    break;
  default:
    break;
  }
}
#else
{
  int n;

  ae_int16x4  x0,x1,x2, y0;
  ae_f32x2    vcf,d0,d1;
  const ae_int16x4 * restrict px = (const ae_int16x4 *)x;
        ae_int16x4 * restrict py = (      ae_int16x4 *)y;
  ae_valign x_align, y_align;

  NASSERT(x);
  NASSERT(y);
  if (N <= 0) return;
  x_align = AE_LA64_PP(px);
  y_align = AE_ZALIGN64();
  vcf = AE_CVT32X2F16_32(AE_MOVDA16(s));

  for (n=0; n<(N&~3); n+=4)
  {
    AE_LA16X4_IP(x0, x_align, px);
    d0=AE_MULFP32X16X2RAS_H(vcf,x0);  
    d1=AE_MULFP32X16X2RAS_L(vcf,x0);  
    y0=AE_ROUND16X4F32SASYM(d0,d1);
    AE_SA16X4_IP(y0, y_align, py);
  }
  N&=3;
  AE_SA64POS_FP(y_align, py);
  if(N)
  {
    ae_int16 *  py_ = (ae_int16 *)py;
    AE_LA16X4_IP(x0, x_align, px);
    d0=AE_MULFP32X16X2RAS_H(vcf,x0);  
    d1=AE_MULFP32X16X2RAS_L(vcf,x0);  
    y0=AE_ROUND16X4F32SASYM(d0,d1);
    x0 = AE_SEL16_6543(y0, y0); 
    x1 = AE_SEL16_5432(y0, y0);
    x2 = AE_SEL16_5432(x0, x0);
    AE_S16_0_IP(x0, py_, 2);
    if(N>1) AE_S16_0_IP(x1, py_, 2);
    if(N>2) AE_S16_0_IP(x2, py_, 2);
  }
}
#endif
