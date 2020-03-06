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

#include "common.h"
#include "NatureDSP_types.h"
/* Library API */
#include "NatureDSP_Signal_vector.h"

/*-------------------------------------------------------------------------
  Vector Min/Max
  These routines find maximum/minimum value in a vector.
  Two versions of functions available: regular version (vec_min32x32, 
  vec_max32x32, vec_min16x16, vec_max16x16, vec_minf, vec_maxf) with 
  arbitrary arguments and faster version (vec_min32x32_fast, vec_max32x32_fast, 
  vec_min16x16_fast, vec_max16x16_fast) that apply some restrictions
  NOTE: functions return zero if N is less or equal to zero

  Precision: 
  32x32 32-bit data, 32-bit output
  16x16 16-bit data, 16-bit output
  f     single precision floating point
  
  Input:
  x[N]  input data
  N     length of vector
  Returned value:
  Minimum or maximum value correspondingly

  Restriction:
  For regular routines:
  none
  For faster routines:
  x aligned on 8-byte boundary
  N   - multiple of 4
-------------------------------------------------------------------------*/

int32_t     vec_min32x32_fast (const int32_t    * restrict x, int N)
{
    int         n;
    const ae_int32x2 * restrict px = (const ae_int32x2 *)x;
    int32_t     y;
    ae_int32x2  x0,x1,m0,m1;
    NASSERT_ALIGN8(x);
    NASSERT((N&3)==0);
  if (N <= 0) return 0;
    m0=m1 = AE_MOVDA32X2(0x7fffffff,0x7fffffff);
    __Pragma ("no_unroll")
    for (n=0;n<(N>>3);n++)
    {
        AE_L32X2_IP(x0, px, +8); 
        m0 = AE_MIN32(m0,x0);
        AE_L32X2_IP(x1, px, +8); 
        m1 = AE_MIN32(m1,x1);

        AE_L32X2_IP(x0, px, +8); 
        m0 = AE_MIN32(m0,x0);
        AE_L32X2_IP(x1, px, +8); 
        m1 = AE_MIN32(m1,x1);
    }
    if(N&4)
    {
        AE_L32X2_IP(x0, px, +8); 
        m0 = AE_MIN32(m0,x0);
        AE_L32X2_IP(x1, px, +8); 
        m1 = AE_MIN32(m1,x1);
    }
    m0 = AE_MIN32(m0,m1);
    y = XT_MIN(AE_MOVAD32_L(m0),AE_MOVAD32_H(m0));
    return y;
}
