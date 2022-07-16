/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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
#include "NatureDSP_Signal.h"

/*-------------------------------------------------------------------------
  Polynomial approximation
  Functions calculate polynomial approximation for all values from given 
  vector. Fixed point functions take polynomial coefficients in Q15 or 
  Q31 precision. 
  NOTE:
  approximation is calculated like Taylor series that is why overflow may 
  potentially occur if cumulative sum of coefficients given from the last 
  to the first coefficient is bigger than 1. To avoid this negative effect,
  all the coefficients may be scaled down and result will be shifted left 
  after all intermediate computations.

  Precision: 
  16x16  16-bit inputs, 16-bit coefficients, 16-bit output. 
  24x24  24-bit inputs, 24-bit coefficients, 24-bit output. 
  32x32  32-bit inputs, 32-bit coefficients, 32-bit output.
  f      floating point

  Input:
  x[N]    input data, Q15, Q31 or floating point
  N       length of vector
  lsh     additional left shift for result
  c[M+1]  coefficients (5 coefficients for vec_poly4_xxx 
          and 9 coefficients for vec_poly8_xxx), Q15, Q31 
          or floating point
  Output:
  z[N]    result, Q15, Q31 or floating point

  Restriction:
  x,c,z should not overlap
  lsh   should be in range 0...31
-------------------------------------------------------------------------*/
void vec_poly8_24x24 (f24 * restrict z, const f24 * restrict x, const f24 * restrict c, int lsh, int N )
{
    int         n;
    const ae_f24x2 * restrict px = (const ae_f24x2 *)x;
    const ae_f24   * restrict pc = (const ae_f24   *)c;
        ae_f24x2   * restrict pz = (       ae_f24x2*)z;

    ae_valign      x_align, z_align;
    ae_f24x2    c0f, c1f, c2f, c3f, c4f, c5f, c6f, c7f, c8f, s, vxf;
    ae_f32x2 t;

    NASSERT(x);
    NASSERT(c);
    NASSERT(z);
    NASSERT(lsh>=0 && lsh<=31);

    if (N <= 0) return;
  
    x_align = AE_LA64_PP(px);
    z_align = AE_ZALIGN64();
    c8f=AE_L32F24_X (pc, 8*sizeof(*pc));
    WUR_AE_SAR(lsh);
    for (n=0; n<(N&~1); n+=2)
    { 
        AE_LA32X2F24_IP(vxf, x_align, px); 
        c7f=AE_L32F24_I (pc, 7*sizeof(*pc)); t=c7f; AE_MULAFP24X2RA(t, vxf, c8f); s=AE_SAT24S(t);
        c6f=AE_L32F24_I (pc, 6*sizeof(*pc)); t=c6f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        c5f=AE_L32F24_I (pc, 5*sizeof(*pc)); t=c5f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        c4f=AE_L32F24_I (pc, 4*sizeof(*pc)); t=c4f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        c3f=AE_L32F24_I (pc, 3*sizeof(*pc)); t=c3f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        c2f=AE_L32F24_I (pc, 2*sizeof(*pc)); t=c2f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        c1f=AE_L32F24_I (pc, 1*sizeof(*pc)); t=c1f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        AE_L32F24_XP(c0f,pc, 0*sizeof(*pc)); t=c0f; AE_MULAFP24X2RA(t, vxf, s  ); 
        t=AE_SLAS32S(t);
        s=AE_SAT24S(t);
        AE_SA32X2F24_IP(s, z_align, pz);//put answer
    }
    AE_SA64POS_FP(z_align, pz);
    if (N&1)
    { 
        AE_LA32X2F24_IP(vxf, x_align, px); 
        c7f=AE_L32F24_I (pc, 7*sizeof(*pc)); t=c7f; AE_MULAFP24X2RA(t, vxf, c8f); s=AE_SAT24S(t);
        c6f=AE_L32F24_I (pc, 6*sizeof(*pc)); t=c6f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        c5f=AE_L32F24_I (pc, 5*sizeof(*pc)); t=c5f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        c4f=AE_L32F24_I (pc, 4*sizeof(*pc)); t=c4f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        c3f=AE_L32F24_I (pc, 3*sizeof(*pc)); t=c3f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        c2f=AE_L32F24_I (pc, 2*sizeof(*pc)); t=c2f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        c1f=AE_L32F24_I (pc, 1*sizeof(*pc)); t=c1f; AE_MULAFP24X2RA(t, vxf, s  ); s=AE_SAT24S(t);
        AE_L32F24_XP(c0f,pc, 0*sizeof(*pc)); t=c0f; AE_MULAFP24X2RA(t, vxf, s  ); 
        t=AE_SLAS32S(t);
        s=AE_SAT24S(t);
        s=AE_SEL24_HH(s,s);
        AE_S32F24_L_I(s, (ae_f24 *)pz, 0);//put answer
    }
}
