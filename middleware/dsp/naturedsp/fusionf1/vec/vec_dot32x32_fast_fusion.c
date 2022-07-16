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
#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Vector Dot product
  These routines take two vectors and calculates their dot product. Two 
  versions of routines are available: regular versions (vec_dot24x24, 
  vec_dot32x16, vec_dot32x32, vec_dot16x16, vec_dotf) work with arbitrary 
  arguments, faster versions (vec_dot24x24_fast, vec_dot32x16_fast, 
  vec_dot32x32_fast, vec_dot16x16_fast) apply some restrictions.

  Precision: 
  16x16  16x16-bit data, 64-bit output for regular version and 32-bit for 
         fast version
  24x24  24x24-bit data, 64-bit output
  32x16  32x16-bit data, 64-bit output
  32x32  32x32-bit data, 64-bit output
  f      floating point

  Input:
  x[N]  input data, Q31 or floating point
  y[N]  input data, Q31, Q15, or floating point
  N	length of vectors
  Returns: dot product of all data pairs, Q31 or floating point

  Restrictions:
  Regular versions:
  None
  Faster versions:
  x,y - aligned on 8-byte boundary
  N   - multiple of 4

  vec_dot16x16_fast utilizes 32-bit saturating accumulator, so, input data 
  should be scaled properly to avoid erroneous results
-------------------------------------------------------------------------*/
int64_t vec_dot32x32_fast (const int32_t * restrict x,const int32_t * restrict y,int N)
{
    int n;
    const ae_int32x2 * px;
    const ae_int32x2 * py;
    ae_int32x2 X,Y;
    ae_f64 Acc;
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT((N&3)==0);
    if(N<=0) return 0;
    px=(const ae_int32x2 *)x;
    py=(const ae_int32x2 *)y;
    Acc=AE_ZERO64();
    for(n=0; n<(N>>2); n++)
    {
        AE_L32X2_IP(X, px, sizeof(ae_int32x2));
        AE_L32X2_IP(Y, py, sizeof(ae_int32x2));
        AE_MULAF32R_HH(Acc,X,Y);
        AE_MULAF32R_LL(Acc,X,Y);
        AE_L32X2_IP(X, px, sizeof(ae_int32x2));
        AE_L32X2_IP(Y, py, sizeof(ae_int32x2));
        AE_MULAF32R_HH(Acc,X,Y);
        AE_MULAF32R_LL(Acc,X,Y);
    }
    Acc=AE_SRAI64(Acc,16);
    return (int64_t)Acc;
}
