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
#include "NatureDSP_Signal_fit.h"

/*-------------------------------------------------------------------------
  Polynomial approximation
  Functions calculate polynomial approximation for all values from given 
  vector. Fixed point functions take polynomial coefficients in Q31 precision. 
  NOTE:
  approximation is calculated like Taylor series that is why overflow may 
  potentially occur if cumulative sum of coefficients given from the last 
  to the first coefficient is bigger that 1. To avoid this negative effect,
  all the coefficients may be scaled down and result will be shifted left 
  after all intermediate computations.

  Precision: 
  24x24  24-bit inputs, 24-bit coefficients, 24-bit output. 
  32x32  32-bit inputs, 32-bit coefficients, 32-bit output.
  f      floating point

  Input:
  x[N]    input data, Q31 or floating point
  N       length of vector
  lsh     additional left shift for result
  c[M+1]  coefficients (M=4 coefficients for vec_poly4_xxx 
          and M=8 coefficients for vec_poly8_xxx), Q31 or floating point
  Output:			
  z[N]    result, Q31 or floating point

  Restriction:
  x,c,z should not overlap
  lsh   should be in range 0...31

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  x aligned on 8-byte boundary
  N   - multiple of 2
-------------------------------------------------------------------------*/
void vec_poly8_32x32 (int32_t * restrict z, const int32_t * restrict x, const int32_t * restrict c, int lsh,int N )
{
    int         n;
        const ae_int32x2 * restrict px = (const ae_int32x2 *)x;
        const ae_int32   * restrict pc = (const ae_int32   *)c;
              ae_int32x2 * restrict pz = (      ae_int32x2 *)z;
    ae_valign      x_align, z_align;
    ae_int32x2  x0;
    ae_f32x2    t8f,a0,b0;
    if(N<=0) return;
    x_align = AE_LA64_PP(px);
    z_align = AE_ZALIGN64();

    t8f = AE_L32_X(pc, 4*8);
    for (n=0;n<N-1;n+=2)
    {
        AE_LA32X2_IP(x0, x_align, px); 
        b0=AE_L32_I(pc, 4*7); AE_MULAFP32X2RAS(b0, x0, t8f);
        a0=AE_L32_I(pc, 4*6); AE_MULAFP32X2RAS(a0, x0, b0);
        b0=AE_L32_I(pc, 4*5); AE_MULAFP32X2RAS(b0, x0, a0);
        a0=AE_L32_I(pc, 4*4); AE_MULAFP32X2RAS(a0, x0, b0);
        b0=AE_L32_I(pc, 4*3); AE_MULAFP32X2RAS(b0, x0, a0);
        a0=AE_L32_I(pc, 4*2); AE_MULAFP32X2RAS(a0, x0, b0);
        b0=AE_L32_I(pc, 4*1); AE_MULAFP32X2RAS(b0, x0, a0);
        a0=AE_L32_I(pc, 4*0); AE_MULAFP32X2RAS(a0, x0, b0);
        a0 = AE_SLAA32S(a0,lsh);
        AE_SA32X2_IP(a0, z_align, pz);//put answer
    }
    AE_SA64POS_FP(z_align, pz);
    N&=1;
    if (N>0) 
    {
        AE_LA32X2_IP(x0, x_align, px); 
        b0=AE_L32_I(pc, 4*7); AE_MULAFP32X2RAS(b0, x0, t8f);
        a0=AE_L32_I(pc, 4*6); AE_MULAFP32X2RAS(a0, x0, b0);
        b0=AE_L32_I(pc, 4*5); AE_MULAFP32X2RAS(b0, x0, a0);
        a0=AE_L32_I(pc, 4*4); AE_MULAFP32X2RAS(a0, x0, b0);
        b0=AE_L32_I(pc, 4*3); AE_MULAFP32X2RAS(b0, x0, a0);
        a0=AE_L32_I(pc, 4*2); AE_MULAFP32X2RAS(a0, x0, b0);
        b0=AE_L32_I(pc, 4*1); AE_MULAFP32X2RAS(b0, x0, a0);
        a0=AE_L32_I(pc, 4*0); AE_MULAFP32X2RAS(a0, x0, b0);
        a0 = AE_SLAA32S(a0,lsh);
        ((int32_t*)pz)[0]=AE_MOVAD32_H(a0);
    }
}
