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
#include "common.h"


/*===========================================================================
  Vector matematics:
  vec_dot              Vector Dot Product
===========================================================================*/

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
int64_t vec_dot32x16_fast (const int32_t * restrict x, const int16_t * restrict y, int N)
{
  int n;

  ae_int32x2 vxw, vzw;
  ae_int16x4 vyh;
  ae_f32x2 vxf;
  ae_f16x4 vyf;
  ae_f64 vaf;
  ae_int64 vai;
  const ae_f32x2 *    restrict  px = (const ae_f32x2 *)   x;
  const ae_int16x4 *  restrict  py = (const ae_int16x4 *) y;

  NASSERT_ALIGN8(px);
  NASSERT_ALIGN8(py);
  ASSERT((N&3)==0);

  vaf = AE_ZERO64();
  vzw = 0;

  AE_L32X2_IP(vxw, px, sizeof(*px));
  AE_L16X4_IP(vyh, py, sizeof(*py));

  for (n=0; n<N-3; n+=4)
  {
    vxf = (vxw);
    vyf = (vyh);
    AE_MULAAFD32X16_H3_L2(vaf, vxf, vyf);
    AE_L32X2_IP(vxw, px, sizeof(*px));
    vxf = (vxw);
    AE_MULAAFD32X16_H1_L0(vaf, vxf, vyf);
    AE_L32X2_IP(vxw, px, sizeof(*px));
    AE_L16X4_IP(vyh, py, sizeof(*py));
  }

  vai = (vaf);
  vai = AE_SRAI64(vai, 16);

  return_int64(vai);
}
