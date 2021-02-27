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

/*===========================================================================
  Scalar matematics:
  scl_antilog          Antilogarithm         
===========================================================================*/

/*-------------------------------------------------------------------------
  Antilogarithm
  These routines calculate antilogarithm (base2, natural and base10). 32 
  and 24-bit fixed-point functions accept inputs in Q6.25 and form outputs 
  in Q16.15 format and return 0x7FFFFFFF in case of overflow and 0 in case 
  of underflow. 16-bit fixed-point functions accept inputs in Q3.12 and 
  form outputs in Q8.7 format and return 0x7FFF in case of overflow and 
  0 in case of underflow.

  Precision:
  16x16  16-bit inputs, 16-bit outputs. Accuracy: 2 LSB
  32x32  32-bit inputs, 32-bit outputs. Accuracy: 8*e-6*y+1LSB
  24x24  24-bit inputs, 24-bit outputs. Accuracy: 8*e-6*y+1LSB
  f      floating point: Accuracy: 2 ULP
  NOTE:
  1.  Although 32 and 24 bit functions provide the similar accuracy, 32-bit
      functions have better input/output resolution (dynamic range).
  2.  Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly.

  Input:
  x[N]  input data,Q6.25 (for 32 and 24-bit functions), Q3.12 (for 16-bit 
        functions) or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q16.15 (for 32 and 24-bit functions), Q8.7 (for 16-bit 
        functions) or floating point  

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  fixed point functions return result, Q16.15 (for 32 and 24-bit functions), 
  Q8.7 (for 16-bit functions) 

-------------------------------------------------------------------------*/

#include "vec_alog_table.h"
#include "common.h"
int32_t scl_antilogn_32x32 (int32_t x) 
{
    ae_int32x2 X,E,Y;
    ae_int64 a;
    ae_f32x2 t;

    X=AE_MOVDA32(x);
    a=AE_MUL32_LL(X,1549082005);
    X=AE_TRUNCI32X2F64S(a,a,2);
    E=AE_SRAI32(X,25);
    E=AE_SUB32(E,15);
    X=AE_AND32(X,AE_MOVDA32X2(0x1ffffff,0x1ffffff));
    X=AE_SUB32(X,AE_MOVDA32X2(0x2000000,0x2000000));
    X=AE_SLLI32(X,6);

    Y=AE_L32_I((const ae_int32 *)pow2poly,4*0);
    t=AE_L32_I((const ae_int32 *)pow2poly,4*1); AE_MULAFP32X2RAS(t,X,Y); Y=t;
    t=AE_L32_I((const ae_int32 *)pow2poly,4*2); AE_MULAFP32X2RAS(t,X,Y); Y=t;
    t=AE_L32_I((const ae_int32 *)pow2poly,4*3); AE_MULAFP32X2RAS(t,X,Y); Y=t;
    t=AE_L32_I((const ae_int32 *)pow2poly,4*4); AE_MULAFP32X2RAS(t,X,Y); Y=t;
    Y=AE_SLAA32S(Y,AE_MOVAD32_H(E));
    x=AE_MOVAD32_H(Y);
    return x;
}
