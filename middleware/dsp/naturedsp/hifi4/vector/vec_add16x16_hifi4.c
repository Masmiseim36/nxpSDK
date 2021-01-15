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
void vec_add16x16 ( int16_t * restrict z,
              const int16_t * restrict x,
              const int16_t * restrict y,
              int N)
{
  int n;
  ae_int16x4  x0,x1,vxh, vyh, vzh;
  ae_valign      x_align, y_align, z_align;

  const ae_int16x4 * restrict px = (const ae_int16x4 *)x;
  const ae_int16x4 * restrict py = (const ae_int16x4 *)y;
        ae_int16x4 * restrict pz = (      ae_int16x4 *)z;

  if(N<=0) return;
  px = (const ae_int16x4 *)x;
  py = (const ae_int16x4 *)y;
  pz = (      ae_int16x4 *)z;

  y_align = AE_LA64_PP(py);                                        
  z_align = AE_ZALIGN64();

  switch (((uintptr_t)px)&7)
  {
  case 0:
        AE_L16X4_IP (vxh, px, 8);
        AE_LA16X4_IP(vyh, y_align, py);
        vzh = AE_ADD16S(vxh, vyh);
        for (n=0; n<N-3; n+=4)
        {
            AE_SA16X4_IP(vzh, z_align, pz);
            AE_L16X4_IP (vxh, px, 8);
            AE_LA16X4_IP(vyh, y_align, py);
            vzh = AE_ADD16S(vxh, vyh);
        }
        break;
  case 2:
        x_align = AE_LA64_PP(px);
        AE_LA16X4_IP(x0, x_align, px);
        x0=AE_SEL16_4321(x0,x0);
        px=(ae_int16x4*)(x+3);
        AE_L16X4_IP(x1, px, 8);
        vxh=AE_SEL16_6543(x0,x1);
        AE_LA16X4_IP(vyh, y_align, py);
        vzh = AE_ADD16S(vxh, vyh);
        for (n=0; n<N-3; n+=4)
        {
            AE_SA16X4_IP(vzh, z_align, pz);
            x0=x1;
            AE_L16X4_IP(x1, px, 8);
            vxh=AE_SEL16_6543(x0,x1);
            AE_LA16X4_IP(vyh, y_align, py);
            vzh = AE_ADD16S(vxh, vyh);
        }
        break;
  case 4:
        x_align = AE_LA64_PP(px);
        AE_LA16X4_IP(x0, x_align, px);
        x0=AE_SEL16_5432(x0,x0);
        px=(ae_int16x4*)(x+2);
        AE_L16X4_IP(x1, px, 8);
        vxh=AE_SEL16_5432(x0,x1);
        AE_LA16X4_IP(vyh, y_align, py);
        vzh = AE_ADD16S(vxh, vyh);
        for (n=0; n<N-3; n+=4)
        {
            AE_SA16X4_IP(vzh, z_align, pz);
            x0=x1;
            AE_L16X4_IP(x1, px, 8);
            vxh=AE_SEL16_5432(x0,x1);
            AE_LA16X4_IP(vyh, y_align, py);
            vzh = AE_ADD16S(vxh, vyh);
        }
        break;
  case 6:
        x_align = AE_LA64_PP(px);
        AE_LA16X4_IP(x0, x_align, px);
        x0=AE_SEL16_6543(x0,x0);
        px=(ae_int16x4*)(x+1);
        AE_L16X4_IP(x1, px, 8);
        vxh=AE_SEL16_4321(x0,x1);
        AE_LA16X4_IP(vyh, y_align, py);
        vzh = AE_ADD16S(vxh, vyh);
        for (n=0; n<N-3; n+=4)
        {
            AE_SA16X4_IP(vzh, z_align, pz);
            x0=x1;
            AE_L16X4_IP(x1, px, 8);
            vxh=AE_SEL16_4321(x0,x1);
            AE_LA16X4_IP(vyh, y_align, py);
            vzh = AE_ADD16S(vxh, vyh);
        }
        break;
  }
  AE_SA64POS_FP(z_align, pz);
  N&=3;
  if (N>=1) ((int16_t*)pz)[0]=AE_MOVAD16_3(vzh);
  if (N>=2) ((int16_t*)pz)[1]=AE_MOVAD16_2(vzh);
  if (N>=3) ((int16_t*)pz)[2]=AE_MOVAD16_1(vzh);
}
