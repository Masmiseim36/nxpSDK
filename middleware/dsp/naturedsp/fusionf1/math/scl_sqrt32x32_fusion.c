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
#include "scl_sqrt_table.h"
#include "common.h"

/*===========================================================================
  Scalar matematics:
  scl_sqrt          Square Root         
===========================================================================*/

/*-------------------------------------------------------------------------
  Square Root
  These routines calculate square root.
  NOTES: 
  1. Fixed point functions return 0x80000000 (for 24 and 32-bit functions), 
     0x8000 (for 16-bit functions) on negative argument
  2. For floating point function, whenever an input value is negative, 
     functions raise the "invalid" floating-point exception, assign EDOM 
     to errno and set output value to NaN. Negative zero is considered as
     a valid input, the result is also -0
  Two versions of functions available: regular version (vec_sqrt16x16, 
  vec_sqrt24x24, vec_sqrt32x32, vec_sqrtf) with arbitrary arguments and
  faster version (vec_sqrt24x24_fast, vec_sqrt32x32_fast) that apply some 
  restrictions.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: (2 LSB)
  24x24  24-bit inputs, 24-bit output. Accuracy: (2.6e-7*y+1LSB)
  32x32  32-bit inputs, 32-bit output. Accuracy: (2.6e-7*y+1LSB)
  f      floating point. Accuracy 1 ULP

  Input:
  x[N]  input data,Q31, Q15 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31, Q15 or floating point

  Restriction:
  Regular versions (vec_sqrt24x24, vec_sqrt32x32):
  x,y - should not overlap

  Faster versions (vec_sqrt24x24_fast, vec_sqrt32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result, Q31, Q15 or floating point
-------------------------------------------------------------------------*/

int32_t scl_sqrt32x32(int32_t x) 
{
  int         nsa, idx, shft_a;
  int32_t     y;
  ae_int32x2  vbw, vcw, vxw, vzw, vdw, viw;
  ae_f32x2    vxf, vzf, vcf;
  xtbool2     inf, zero;
  vzw = AE_MOVI(0);
  vxw = AE_MOVDA32X2(x,x);
  viw = AE_MOVDA32X2(MIN_INT32, MIN_INT32);
  zero = AE_EQ32(vxw, vzw);
  nsa = AE_NSAZ32_L(vxw);
  shft_a = ( nsa & ~1 );
  nsa   = ( nsa )/2;
  vxw = AE_SLAA32( vxw, shft_a );
  inf = AE_LT32(vxw, vzw);

  /* get table indexes */
  vdw = AE_SRAI32(vxw, 24);
  vdw = AE_SLAI32(vdw, 3);
  idx=AE_MOVAD32_H(vdw); //
  vcw = AE_L32X2_X((const ae_int32x2 *)sqrt_table, idx); 
  vbw = AE_SEL32_LL(vcw, vcw); //f'(x0)
  vzf = (vcw);
  vxf = (vxw);
  AE_MULSFP32X2RAS(vxf, vzf, vzf); //dx
  vcf = (vbw);
  AE_MULAFP32X2RAS(vzf, vcf, vxf); //y 

  /*second iteration*/
  vxf = (vxw);
  AE_MULSFP32X2RAS(vxf, vzf, vzf);
  vcf = (vbw);
  AE_MULAFP32X2RAS(vzf, vcf, vxf); //y
  vdw =  (vzf);
  vdw = AE_SRAA32(vdw, nsa);
  AE_MOVT32X2(vdw, viw, inf);
  AE_MOVT32X2(vdw, vzw, zero);
  y = AE_MOVAD32_H(vdw);
  return y;
}
