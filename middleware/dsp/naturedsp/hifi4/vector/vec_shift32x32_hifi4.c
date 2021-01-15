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

/* Library API */
#include "NatureDSP_Signal_vector.h"
#include "common.h"


/*===========================================================================
  Vector matematics:
  vec_shift,vec_scale  Vector Scaling with Saturation
===========================================================================*/

/*-------------------------------------------------------------------------
  Vector Scaling with Saturation
  These routines make shift with saturation of data values in the vector 
  by given scale factor (degree of 2).
  Functions vec_scale() make multiplication of a vector to a coefficient 
  which is not a power of 2 forming Q31, Q15 or floating-point result.
  Two versions of routines are available: regular versions (vec_shift32x32, 
  vec_shift16x16, vec_shiftf, vec_scale32x32, vec_scale16x16, vec_scalef, 
  vec_scale_sf) work with arbitrary arguments, faster versions 
  (vec_shift32x32_fast, vec_shift16x16_fast, vec_scale32x32_fast, 
  vec_scale16x16_fast) apply some restrictions

  For floating point:
  Fuction vec_shiftf() makes scaling without saturation of data values by given 
  scale factor (degree of 2). 
  Functions vec_scalef() and vec_scale_sf() make multiplication of input vector
  to coefficient which is not a power of 2.
  Two versions of routines are available: 
    without saturation - vec_scalef;
    with saturation - vec_scale_sf; 

  Precision:
  32x32 32-bit input, 32-bit output
  16x16 16-bit input, 16-bit output
  f     single precision floating point

  Input:
  x[N]    input data, Q31, Q15 or floating point
  t       shift count. If positive, it shifts left with saturation, if
          negative it shifts right
  s       scale factor, Q31, Q15 or floating point
  N       length of vector
  fmin    minimum output value (only for vec_scale_sf)
  fmax    maximum output value (only for vec_scale_sf)
  Output:
  y[N]    output data, Q31, Q15 or floating point

  Restrictions:
  For regular versions (vec_shift32x32, vec_shift16x16, vec_shiftf, 
  vec_scale32x32, vec_scale16x16, vec_scalef, vec_scale_sf):
  x,y should not overlap
  t   should be in range -31...31 for fixed-point functions and -129...146 
      for floating point
  For vec_scale_sf:
  fmin<=fmax;

  For faster versions (vec_shift32x32_fast, vec_shift16x16_fast, 
  vec_scale32x32_fast,vec_scale16x16_fast):
  x,y should not overlap
  t should be in range -31...31 
  x,y - aligned on 8-byte boundary
  N   - multiple of 4 
-------------------------------------------------------------------------*/
void vec_shift32x32
(
        int32_t * restrict y,
  const int32_t * restrict x,
  int t,
  int N
)
{
    int n;
    ae_int32x2  vxw, vyw,x0,x1;
    const ae_int32x2 * restrict px = (const ae_int32x2 *)x;
        ae_int32x2 * restrict py = (      ae_int32x2 *)y;

    ae_valign y_align;

    if(N<=0) return;
    t=XT_MAX(XT_MIN(t,31),-31);
    WUR_AE_SAR(t);
    y_align = AE_ZALIGN64();
    if ((((uintptr_t)(x))&7)==0)
    {   // px is aligned
        AE_L32X2_IP(vxw, px, 8);
        for (n=0; n<N-1; n+=2)
        {
            vyw = AE_SLAS32S(vxw);
            AE_SA32X2_IP(vyw, y_align, py);
            AE_L32X2_IP(vxw, px, 8);
        }
    }
    else
    {
        AE_L32_IP(x0,castxcc(ae_int32,px),4);
        AE_L32X2_IP(x1, px, 8);
        vxw=AE_SEL32_LH(x0,x1);
        for (n=0; n<N-1; n+=2)
        {
            vyw = AE_SLAS32S(vxw);
            AE_SA32X2_IP(vyw, y_align, py);
            x0=x1;
            AE_L32X2_IP(x1, px, 8);
            vxw=AE_SEL32_LH(x0,x1);
        }
    }
    vyw = AE_SLAS32S(vxw);
    AE_SA64POS_FP(y_align, py);
    if (N&1)  ((int32_t*)py)[0]=AE_MOVAD32_H(vyw);
}
