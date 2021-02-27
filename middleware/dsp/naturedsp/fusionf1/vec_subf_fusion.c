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
#if (XCHAL_HAVE_FUSION_FP)

/*-------------------------------------------------------------------------
  Vector subtraction
-------------------------------------------------------------------------*/
void vec_subf ( float32_t * restrict z,
              const float32_t * restrict x,
              const float32_t * restrict y,
              int N)
{
  int n;

  xtfloat xf0, yf0;
  xtfloat zf0;
  pair_xtfloat vxf0, vyf0, vzf0;
  const xtfloat * restrict px = (const xtfloat *)x;
  const xtfloat * restrict py = (const xtfloat *)y;
  pair_xtfloat * restrict pz = (pair_xtfloat *)z;
  ae_valign x_align, y_align, z_align;
  NASSERT(x);
  NASSERT(y);
  if (N <= 0) return; 
  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);

  z_align = AE_ZALIGN64();
  for (n = 0; n<(N>>1); n++)
  { 
    XT_pair_xtfloat_aligning_load_post_update_positive(vxf0, x_align, castxcc(const pair_xtfloat, px));
    XT_pair_xtfloat_aligning_load_post_update_positive(vyf0, y_align, castxcc(const pair_xtfloat, py));

    vzf0 = XT_SUBX2_S(vxf0, vyf0);
    XT_pair_xtfloat_aligning_store_post_update_positive(vzf0, z_align, pz);
  }
  XT_pair_xtfloat_aligning_store_flush_positive(z_align, pz);
  if (N&1)
  {
    XT_LSIP(xf0, px, 4);
    XT_LSIP(yf0, py, 4);
    zf0 = XT_SUB_S(xf0, yf0);
    XT_SSIP(zf0, castxcc(xtfloat,pz), 4);
  }
} /* vec_subf() */
#endif
