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
#include "common.h"
#include "NatureDSP_types.h"

/*-------------------------------------------------------------------------
  Vector Min/Max
  These routines find maximum/minimum value in a vector.
  Two versions of functions available: regular version (vec_min32x32, 
  vec_max32x32, vec_min24x24, vec_max24x24,vec_max16x16, vec_min16x16, 
  vec_maxf, vec_minf) with arbitrary arguments and faster version 
  (vec_min32x32_fast, vec_max32x32_fast, vec_min24x24_fast, 
  vec_max24x24_fast,vec_min16x16_fast, vec_min16x16_fast) that apply some 
  restrictions
  NOTE: functions return zero if N is less or equal to zero

  Precision: 
  32x32 32-bit data, 32-bit output
  24x24 24-bit data, 24-bit output
  16x16 16-bit data, 16-bit output
  f     single precision floating point
  
  Input:
  x[N]  input data
  N     length of vector
  Function return minimum or maximum value correspondingly

  Restriction:
  For regular routines:
  none
  For faster routines:
  x aligned on 8-byte boundary
  N   - multiple of 4
-------------------------------------------------------------------------*/
f24     vec_min24x24_fast (const f24    * restrict x, int N)
{
  int         n;
  const ae_f24x2 * restrict px = (const ae_f24x2 *)x;
  int32_t     y;
  ae_int32x2  vxw,vmw;
  ae_f24x2    vxl;
  NASSERT_ALIGN8(px);
  ASSERT((N & 1) == 0);
  if (N <= 0) return 0;
  vmw = AE_MOVDA32X2(0x7FFFFF00,0x7FFFFF00);
  for (n=0;n<N;n+=2)
  {
    AE_L32X2F24_IP(vxl, px, +8);
    vmw = AE_MIN32(vmw,vxl);
  }
  vxw = AE_SEL32_HH(vmw,vmw);
  vmw = AE_SEL32_LL(vmw,vmw);
  vmw = AE_MIN32(vmw,vxw);
  vmw = AE_SLAA32S(vmw,8);
  y = AE_MOVAD32_H(vmw);
  return y;
}
