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
void vec_sub16x16 
(
  int16_t *       restrict z,
  const int16_t * restrict x,
  const int16_t * restrict y,
  int N
)
{
  int n;

  ae_int16x4  vxh, vyh, vzh;
  ae_valign      x_align, y_align, z_align;

  const ae_int16x4 * restrict px = (const ae_int16x4 *)x;
  const ae_int16x4 * restrict py = (const ae_int16x4 *)y;
        ae_int16x4 * restrict pz = (      ae_int16x4 *)z;

  NASSERT(x);
  NASSERT(y);
  NASSERT(z);
  if (N <= 0) return;

  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);
  z_align = AE_ZALIGN64();
  
  AE_LA16X4_IP(vxh, x_align, px);
  AE_LA16X4_IP(vyh, y_align, py);

  for (n=0; n<N-3; n+=4)
  {
    vzh = AE_SUB16S(vxh, vyh);
    AE_LA16X4_IP(vxh, x_align, px);
    AE_LA16X4_IP(vyh, y_align, py);
    AE_SA16X4_IP(vzh, z_align, pz);
  }
  AE_SA64POS_FP(z_align, pz);

  switch(N&3)
  {
  case 1:
    vzh = AE_SUB16S(vxh, vyh);
    vxh = AE_SEL16_6543(vzh, vzh);
    AE_S16_0_I(vxh, (ae_int16 *)pz, 0);
    break;
  case 2:
    vzh = AE_SUB16S(vxh, vyh);
    vxh = AE_SEL16_6543(vzh, vzh);
    AE_S16_0_I(vxh, (ae_int16 *)pz, 0);
    vyh = AE_SEL16_5432(vzh, vzh);
    AE_S16_0_I(vyh, (ae_int16 *)pz, sizeof(ae_int16));
    break;
  case 3:
    vzh = AE_SUB16S(vxh, vyh);
    vxh = AE_SEL16_6543(vzh, vzh);
    AE_S16_0_I(vxh, (ae_int16 *)pz, 0);
    vyh = AE_SEL16_5432(vzh, vzh);
    AE_S16_0_I(vyh, (ae_int16 *)pz, sizeof(ae_int16));
    vxh = AE_SEL16_4321(vzh, vzh);
    AE_S16_0_I(vxh, (ae_int16 *)pz, 2*sizeof(ae_int16));
    break;
  default:
    break;
  }
}
