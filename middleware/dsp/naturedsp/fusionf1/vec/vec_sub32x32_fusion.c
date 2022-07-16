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
#include "NatureDSP_types.h"
#include "vec_sub.h"
#include "common.h"
/*-------------------------------------------------------------------------
  Vector subtraction
-------------------------------------------------------------------------*/
void vec_sub32x32 ( int32_t * restrict z,
              const int32_t * restrict x,
              const int32_t * restrict y,
              int N)
{
  int n;

  ae_int32x2  vxw, vyw, vzw;
  ae_valign      x_align, y_align, z_align;

  const ae_int32x2 * restrict px = (const ae_int32x2 *)x;
  const ae_int32x2 * restrict py = (const ae_int32x2 *)y;
        ae_int32x2 * restrict pz = (      ae_int32x2 *)z;   
  NASSERT(x);
  NASSERT(y);
  NASSERT(z);
  if (N <= 0) return;

  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);
  z_align = AE_ZALIGN64();
  
  AE_LA32X2_IP(vxw, x_align, px);
  AE_LA32X2_IP(vyw, y_align, py);

  for (n=0; n<N-1; n+=2)
  {
    vzw = AE_SUB32S(vxw, vyw);
    AE_LA32X2_IP(vxw, x_align, px);
    AE_LA32X2_IP(vyw, y_align, py);
    AE_SA32X2_IP(vzw, z_align, pz);
  }
  AE_SA64POS_FP(z_align, pz);

  if (N&1)
  {
    vzw = AE_SUB32S(vxw, vyw);
    vzw = AE_SEL32_HH(vzw, vzw);
    AE_S32_L_I(vzw, (ae_int32 *)pz, 0);
  }
}
