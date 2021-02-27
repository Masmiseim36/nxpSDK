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
int32_t vec_dot16x16_fast
  (const int16_t * restrict x, const int16_t * restrict y, int N)
#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
{
   /* code with no Quad MAC option */
  int n;

  ae_f32x2    vaf, vbf;
  ae_int32x2  vai, vbi;
  ae_f16x4    vxf, vyf;
  ae_int16x4  vxh, vyh;

  const ae_int16x4 * restrict px = (const ae_int16x4 *)x;
  const ae_int16x4 * restrict py = (const ae_int16x4 *)y;

  NASSERT_ALIGN8(px);
  NASSERT_ALIGN8(py);
  ASSERT((N&3)==0);

  vai = AE_MOVI(0);
  vbi = AE_MOVI(0);

  vaf = (vai);
  vbf = (vbi);

  for (n=0; n<N; n+=4)
  {
    AE_L16X4_IP(vxh, px, sizeof(*px));
    AE_L16X4_IP(vyh, py, sizeof(*py));

    vxf = (vxh);
    vyf = (vyh);
    AE_MULAF16X4SS(vaf, vbf, vxf, vyf);
  }

  vai = (vaf);
  vbi = (vbf);

  vai = AE_ADD32S(vai, vbi);
  vbi = AE_SEL32_LH(vai, vai);
  vai = AE_ADD32S(vai, vbi);

  return AE_MOVAD32_H(vai);
}
#else
{   /* code with Quad MAC option */
    int n;
    ae_int32x2 r;
    ae_int64    a;
    ae_int16x4  x0,y0;

    const ae_int16x4 * restrict px = (const ae_int16x4 *)x;
    const ae_int16x4 * restrict py = (const ae_int16x4 *)y;

    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT(N%4==0);
    if(N<=0) return 0;

    a = AE_ZERO();

    for (n=0; n<N; n+=4)
    {
        AE_L16X4_IP(x0, px, 4*sizeof(int16_t));
        AE_L16X4_IP(y0, py, 4*sizeof(int16_t));
        AE_MULAAAAQ16(a,x0,y0);
    }
    r=AE_TRUNCA32X2F64S(a,a,33);
    return AE_MOVAD32_L(r);
}
#endif
