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
int64_t vec_dot16x16 (const int16_t * restrict x,const int16_t * restrict y,int N)
#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
{
   /* code with no Quad MAC option */
  int n;

  ae_f64      vaf;
  ae_int64    vai;
  ae_int16x4  vxh, vyh;
  ae_f24x2    vxf, vyf;
  ae_int32x2  vxw, vyw, vzw;

  ae_valign      x_align, y_align;

  const ae_int16x4 * restrict px = (const ae_int16x4 *)x;
  const ae_int16x4 * restrict py = (const ae_int16x4 *)y;

  vaf = AE_ZERO64();
  vzw = 0;

  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);

  AE_LA16X4_IP(vxh, x_align, px);
  AE_LA16X4_IP(vyh, y_align, py);

  for (n=0; n<N-3; n+=4)
  {
    vxw = AE_SEXT32X2D16_32(vxh);
    vyw = AE_SEXT32X2D16_32(vyh);
    vxf = AE_MOVF24X2_FROMINT32X2(vxw);
    vyf = AE_MOVF24X2_FROMINT32X2(vyw);
    AE_MULAAFD24_HH_LL(vaf, vxf, vyf);
    vxw = AE_SEXT32X2D16_10(vxh);
    vyw = AE_SEXT32X2D16_10(vyh);
    vxf = AE_MOVF24X2_FROMINT32X2(vxw);
    vyf = AE_MOVF24X2_FROMINT32X2(vyw);
    AE_MULAAFD24_HH_LL(vaf, vxf, vyf);
    AE_LA16X4_IP(vxh, x_align, px);
    AE_LA16X4_IP(vyh, y_align, py);
  }

  switch(N-n)
  {
  case 1:
    vxw = AE_SEXT32X2D16_32(vxh);
    vyw = AE_SEXT32X2D16_32(vyh);
    vxw = AE_SEL32_HL(vxw, vzw);
    vyw = AE_SEL32_HL(vyw, vzw);
    vxf = AE_MOVF24X2_FROMINT32X2(vxw);
    vyf = AE_MOVF24X2_FROMINT32X2(vyw);
    AE_MULAAFD24_HH_LL(vaf, vxf, vyf);
    break;
  case 2:
    vxw = AE_SEXT32X2D16_32(vxh);
    vyw = AE_SEXT32X2D16_32(vyh);
    vxf = AE_MOVF24X2_FROMINT32X2(vxw);
    vyf = AE_MOVF24X2_FROMINT32X2(vyw);
    AE_MULAAFD24_HH_LL(vaf, vxf, vyf);
    break;
  case 3:
    vxw = AE_SEXT32X2D16_32(vxh);
    vyw = AE_SEXT32X2D16_32(vyh);
    vxf = AE_MOVF24X2_FROMINT32X2(vxw);
    vyf = AE_MOVF24X2_FROMINT32X2(vyw);
    AE_MULAAFD24_HH_LL(vaf, vxf, vyf);
    vxw = AE_SEXT32X2D16_10(vxh);
    vyw = AE_SEXT32X2D16_10(vyh);
    vxw = AE_SEL32_HL(vxw, vzw);
    vyw = AE_SEL32_HL(vyw, vzw);
    vxf = AE_MOVF24X2_FROMINT32X2(vxw);
    vyf = AE_MOVF24X2_FROMINT32X2(vyw);
    AE_MULAAFD24_HH_LL(vaf, vxf, vyf);
    break;
  default:
    break;
  }

  vai = (vaf);
  return_int64(vai);
}
#else
{   /* code with Quad MAC option */
    int n;

    ae_int64    a;
    ae_int16x4  x0,y0;
    xtbool4 b;

    ae_valign      x_align, y_align;

    const ae_int16x4 * restrict px = (const ae_int16x4 *)x;
    const ae_int16x4 * restrict py = (const ae_int16x4 *)y;

    if(N<=0) return 0;

    a = AE_ZERO();
    x_align = AE_LA64_PP(px);
    y_align = AE_LA64_PP(py);

    for (n=0; n<(N&~3); n+=4)
    {
        AE_LA16X4_IP(x0, x_align, px);
        AE_LA16X4_IP(y0, y_align, py);
        AE_MULAAAAQ16(a,x0,y0);
    }
    N&=3;
    if(N)
    {
        /*
        1 -   7
        2 -   3
        3 -   1
        */
        b=(1<<(4-N))-1;
        AE_LA16X4_IP(x0, x_align, px);
        AE_LA16X4_IP(y0, y_align, py);
        AE_MOVT16X4(x0,AE_ZERO16(),b);
        AE_MOVT16X4(y0,AE_ZERO16(),b);
        AE_MULAAAAQ16(a,x0,y0);
    }
    a=AE_ADD64(a,a);
    return (int64_t)a;
}
#endif
