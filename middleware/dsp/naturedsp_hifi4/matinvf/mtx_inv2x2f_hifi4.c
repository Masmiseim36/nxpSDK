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
/*
 * Real Matrix Inversion
 * Optimized code for HiFi4
  IntegrIT, 2006-2018
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Matrix functions */
#include "NatureDSP_Signal_matinv.h"
#include "common_fpu.h"

#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(void,mtx_inv2x2f,(void* pScr,float32_t* x))
#elif (HAVE_VFPU)
/*-------------------------------------------------------------------------
  These functions implement in-place matrix inversion by Gauss elimination 
  with full pivoting
  NOTE: user may detect "invalid" or "divide-by-zero" exception in the CPU 
  flags which MAY indicate that inversion results are not accurate. Also 
  it's responsibility of the user to provide valid input matrix for 
  inversion.
  Fixed point version takes representation of input matrix and forms 
  representation of output matrix with proper scaling.

  Precision: 
  f     floating point
  32x32 32-bit input, 32-bit output

  Input:
  x[N*N]      input matrix
  qX          input matrix representation (for fixed point API only)
  Output:
  x[N*N]      result
  Temporary:
  pScr        scratch memory. Size in bytes is defined by corresponding 
              scratch allocation function 
  N is 2,3,4,6,8,10

  Returned value: floating functions return none, fixed point functions 
                  return fixed-point representation of inverted matrix
  Restrictions:
  none
-------------------------------------------------------------------------*/
void mtx_inv2x2f(void *pScr, float32_t* x)
{
  xtfloatx2 *px;
  xtfloatx2 db, ca;
  xtfloatx2 a, b, c, d, r;
  ae_valign al_px;
  (void)pScr;

  /* Load matrix */
  px = (xtfloatx2 *)x;
  a = XT_LSI((xtfloat *)px, 0*sizeof(float32_t));
  b = XT_LSI((xtfloat *)px, 1*sizeof(float32_t));
  c = XT_LSI((xtfloat *)px, 2*sizeof(float32_t));
  d = XT_LSI((xtfloat *)px, 3*sizeof(float32_t));

  /* Find the determinant and its reciprocal */
  r = a*d;
  XT_MSUB_SX2(r, b, c);
  r=XT_RECIP_SX2(r);
  /* Calculate matrix inversion */
  db = XT_SEL32_LL_SX2( d, -b);
  ca = XT_SEL32_HH_SX2(-c,  a);
  db = db*r;
  ca = ca*r;

  /* Save inverse matrix */
  px = (xtfloatx2 *)x;
  al_px = AE_ZALIGN64();
  XT_SASX2IP(db, al_px, px);
  XT_SASX2IP(ca, al_px, px);
  XT_SASX2POSFP(al_px, px);
}/* mtx_inv2x2f() */
#else
void mtx_inv2x2f(void *pScr, float32_t* x)
{
  xtfloat *pX = (xtfloat *) x;
  xtfloat a, b, c, d, r, rn;
  (void)pScr;
  a = XT_LSI(pX, 0*sizeof(float32_t));
  b = XT_LSI(pX, 1*sizeof(float32_t));
  c = XT_LSI(pX, 2*sizeof(float32_t));
  d = XT_LSI(pX, 3*sizeof(float32_t));
  /* Find the determinant and its reciprocal */
  r = XT_MUL_S(a, d);
  XT_MSUB_S(r, b, c);
  r = XT_RECIP_S(r); rn = XT_NEG_S(r);
  /* Calculate matrix inversion */
  a = XT_MUL_S(a, r);
  b = XT_MUL_S(b, rn);
  c = XT_MUL_S(c, rn);
  d = XT_MUL_S(d, r);
  XT_SSI(d, pX, 0 * sizeof(float32_t));
  XT_SSI(b, pX, 1 * sizeof(float32_t));
  XT_SSI(c, pX, 2 * sizeof(float32_t));
  XT_SSI(a, pX, 3 * sizeof(float32_t));
} /* mtx_inv2x2f() */

#endif

size_t mtx_inv2x2f_getScratchSize        () { return 0; }
