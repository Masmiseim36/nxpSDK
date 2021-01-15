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
  vec_add              Vector Sum
===========================================================================*/

/*-------------------------------------------------------------------------
  Vector Sum
  This routine makes pair wise saturated summation of vectors.
  Two versions of routines are available: regular versions (vec_add32x32, 
  vec_add16x16, vec_addf) work with arbitrary arguments, faster versions 
  (vec_add32x32_fast, vec_add16x16_fast) apply some restrictions.

  Precision: 
  32x32 32-bit inputs, 32-bit output
  16x16 16-bit inputs, 16-bit output
  f     single precision floating point

  Input:
  x[N]   input data
  y[N]   input data
  N      length of vectors
  Output:
  z[N]   output data

  Restriction:
  Regular versions (vec_add32x32, vec_add16x16, vec_addf):
  x,y,z - should not be overlapped
  Faster versions (vec_add32x32_fast, vec_add16x16_fast):
  z,x,y - aligned on 8-byte boundary
  N     - multiple of 4
-------------------------------------------------------------------------*/
void vec_add32x32 ( int32_t * restrict z,
              const int32_t * restrict x,
              const int32_t * restrict y,
              int N)
{
  int n;

  ae_int32x2  vxh, vyh, vzh;
  ae_valign      y_align, z_align;

  const ae_int32x2 * restrict px = (const ae_int32x2 *)x;
  const ae_int32x2 * restrict py = (const ae_int32x2 *)y;
        ae_int32x2 * restrict pz = (      ae_int32x2 *)z;
  if(N<=0) return;

  if ((((uintptr_t)(x))&7)==0)
  {   // px is aligned
      y_align = AE_LA64_PP(py);                                        
      z_align = AE_ZALIGN64();

      AE_L32X2_IP(vxh, px, 8);
      AE_LA32X2_IP(vyh, y_align, py);
      for (n=0; n<N-1; n+=2)
      {
        vzh = AE_ADD32S(vxh, vyh);
        AE_L32X2_IP(vxh, px, 8);
        AE_LA32X2_IP(vyh, y_align, py);
        AE_SA32X2_IP(vzh, z_align, pz);
      }
  }
  else  // px is unaligned
  {
      ae_int32x2 x0,x1;

      AE_L32_IP(x0,castxcc(ae_int32,px),4);
      y_align = AE_LA64_PP(py);
      z_align = AE_ZALIGN64();

      AE_L32X2_IP(x1, px, 8);
      vxh=AE_SEL32_LH(x0,x1);
      AE_LA32X2_IP(vyh, y_align, py);
      for (n=0; n<N-1; n+=2)
      {
        vzh = AE_ADD32S(vxh, vyh);
        x0=x1;
        AE_L32X2_IP(x1, px, 8);
        vxh=AE_SEL32_LH(x0,x1);
        AE_LA32X2_IP(vyh, y_align, py);
        AE_SA32X2_IP(vzh, z_align, pz);
      }
    }
    AE_SA64POS_FP(z_align, pz);
    vzh = AE_ADD32S(vxh, vyh);
    if (N&1)  ((int32_t*)pz)[0]=AE_MOVAD32_H(vzh);
}
