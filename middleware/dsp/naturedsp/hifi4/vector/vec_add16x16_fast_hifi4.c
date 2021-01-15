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
void vec_add16x16_fast ( int16_t * restrict z,
              const int16_t * restrict x,
              const int16_t * restrict y,
              int N)
{
  int n;
  ae_int16x4  x0, y0, z0;

  const ae_int16x4 * restrict px = (const ae_int16x4 *)x;
  const ae_int16x4 * restrict py = (const ae_int16x4 *)y;
        ae_int16x4 * restrict pz = (      ae_int16x4 *)z;
 
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT((N&3)==0);
  if(N<=0) return ;
  __Pragma("loop_count min=1")
  for (n=0; n<(N>>2); n++)
  {
    AE_L16X4_IP(x0, px,8);
    AE_L16X4_IP(y0, py,8);
    z0 = AE_ADD16S(x0, y0);
    AE_S16X4_IP(z0, pz,8);
  }
}
