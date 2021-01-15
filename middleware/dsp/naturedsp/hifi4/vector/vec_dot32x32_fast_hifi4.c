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
  vec_dot              Vector Dot Product
===========================================================================*/

/*-------------------------------------------------------------------------
  Vector Dot product
  These routines take two vectors and calculates their dot product.
  Two versions of routines are available: regular versions (vec_dot64x32,
  vec_dot64x64, vec_dot64x64i, vec_dot32x16, vec_dot32x32,vec_dot16x16, 
  vec_dotf) work with arbitrary arguments, faster versions (vec_dot64x32_fast, 
  vec_dot64x64_fast, vec_dot64x64i_fast, vec_dot32x16_fast, vec_dot32x32_fast,
  vec_dot16x16_fast) apply some restrictions.  
  NOTE:
  vec_dot16x16_fast utilizes 32-bit saturating accumulator, so input data 
  should be scaled properly to avoid erroneous results.

  Precision: 
  64x32  64x32-bit data, 64-bit output (fractional multiply Q63xQ31->Q63)
  64x64  64x64-bit data, 64-bit output (fractional multiply Q63xQ63->Q63)
  64x64i 64x64-bit data, 64-bit output (low 64 bit of integer multiply)
  32x32  32x32-bit data, 64-bit output
  32x16  32x16-bit data, 64-bit output
  16x16  16x16-bit data, 64-bit output for regular version and 32-bit for 
                        fast version
  f      single precision floating point

  Input:
  x[N]  input data, Q15, Q31, Q63 or floating point
  y[N]  input data, Q15, Q31, Q63 or floating point
  N	    length of vectors
  Returns:
  dot product of all data pairs, Q31, Q63 or floating point

  Restrictions:
  Regular versions:
    none
  Faster versions:
    x,y - aligned on 8-byte boundary
    N   - multiple of 4
-------------------------------------------------------------------------*/
int64_t vec_dot32x32_fast (const int32_t * restrict x,const int32_t * restrict y,int N)
{
  int n;
  ae_int32x2 x0,y0;
  ae_int64  Acc;
  ae_f64 t=AE_MOVINT64_FROMINT32X2(AE_MOVDA32X2(0,0x8000));
  
  const ae_int32x2 *  restrict  px = (const ae_int32x2 *)  (x);
  const ae_int32x2 *  restrict  py = (const ae_int32x2 *)  (y);
                                         
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT((N&3)==0);
  if(N<=0) return 0;
  __Pragma("loop_count min=2")
  for (n=0; n<(N>>1); n++)
  {
    AE_L32X2_IP(x0, px, 8);
    AE_L32X2_IP(y0, py, 8);
    AE_MULAAFD32RA_HH_LL(t,x0,y0);
  }
  Acc=AE_SRAI64(t,16);
  return Acc;
}
