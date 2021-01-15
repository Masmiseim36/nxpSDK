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
#include "log_table.h"
#include "common.h"
#include "NatureDSP_Signal_math.h"

/*===========================================================================
  Vector matematics:
  vec_log              Logarithm 
===========================================================================*/

/*-------------------------------------------------------------------------
  Logarithm:
  Different kinds of logarithm (base 2, natural, base 10). Fixed point 
  functions represent results in Q25 format or return 0x80000000 on negative 
  of zero input.

  Precision:
  32x32  32-bit inputs, 32-bit outputs
  f      floating point

  Accuracy :
  vec_log2_32x32,scl_log2_32x32              730 (2.2e-5)
  vec_logn_32x32,scl_logn_32x32              510 (1.5e-5)
  vec_log10_32x32,scl_log10_32x32            230 (6.9e-6)
  floating point                             2 ULP

  NOTES:
  1.  Although 32 and 24 bit functions provide the same accuracy, 32-bit 
      functions have better input/output resolution (dynamic range)
  2.  Scalar Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly.
  3.  Floating point functions limit the range of allowable input values:
      A) If x<0, the result is set to NaN. In addition, scalar floating point
         functions assign the value EDOM to errno and raise the "invalid" 
         floating-point exception.
      B) If x==0, the result is set to minus infinity. Scalar floating  point
         functions assign the value ERANGE to errno and raise the "divide-by-zero"
         floating-point exception.

  Input:
  x[N]  input data, Q16.15 or floating point 
  N     length of vectors
  Output:
  y[N]  result, Q25 or floating point 

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  return result in Q25 or floating point
-------------------------------------------------------------------------*/
void vec_log2_32x32 (int32_t * restrict y, const int32_t * restrict x, int N)
{
/*algorithm
  f(x) = log2(x)   
  f(x) = (f(x0) + f'(x0)*(x1-x0))+nsa
  log2(x0) = f(x0)
  (1/x0)*log2(e) = f'(x0)
  x = (2^nsa)*x1
  x1-x0 = dx
  x0 in 0.5..1
*/
  int n, nsal,nsah, offh,offl;

        ae_int32x2 * restrict py = (      ae_int32x2 *)y;
  const ae_int32x2 * restrict px = (const ae_int32x2 *)x;

  ae_int32x2  vxw, vyw, vnw, vdw, vmw, vsw, vhw, vzw, viw;
  ae_f64      vyh,vyl;
  xtbool2     inf;
  ae_valign x_align, y_align;
  if(N<=0) return;
  vmw = AE_MOVDA32X2(0x7fffff, 0x7fffff);
  vsw = AE_MOVDA32X2(0x400000, 0x400000);
  vhw = AE_MOVDA32X2(0x40000000, 0x40000000);
  vzw = AE_MOVI(0);
  viw = AE_MOVDA32X2(MIN_INT32, MIN_INT32);
  x_align = AE_LA64_PP(px);
  y_align = AE_ZALIGN64();

  for (n=0; n<(N>>1); n++)
  {
    AE_LA32X2_IP(vxw, x_align, px);/*x,Q16.15*/
    inf = AE_LE32(vxw, vzw);
    /* Normalize x*/
    nsal = AE_NSAZ32_L(vxw);
    nsah = AE_NSAZ32_L(AE_SEL32_HH(vxw,vxw));
    vnw = AE_MOVDA32X2(nsah, nsal);/*Q.0*/
    AE_MOVSARA7X2(nsah,nsal);
    vxw = AE_SLAS32S(vxw);/*x, Q.31*/
    vxw = AE_SUB32(vxw, vhw);
    vdw = AE_AND32(vxw, vmw);
    vdw = AE_SUB32(vdw, vsw);
    vdw = AE_SLAI32(vdw, 2);       /*x1-x0, Q.33 */
    vxw = AE_SRAI32(vxw, 23);
    vxw = AE_SLAI32(vxw, 3);
    offh= AE_MOVAD32_H(vxw);
    offl= AE_MOVAD32_L(vxw);
    vxw = AE_L32X2_X((const ae_int32x2 *)log2_table, offh);
    vyh = AE_CVT64F32_H(vxw);     /* Q.63 */
    AE_MULAF32S_LH(vyh, vxw, vdw);   /* log2(x0)+(1/x0)*log2(e)*(x1-x0), Q.29 * Q.33 -> Q.63 */
    vxw = AE_L32X2_X((const ae_int32x2 *)log2_table, offl);
    vyl = AE_CVT64F32_H(vxw);     /* Q.63 */
    AE_MULAF32S_LL(vyl, vxw, vdw);   /* log2(x0)+(1/x0)*log2(e)*(x1-x0), Q.29 * Q.33 -> Q.63 */
    vyw=AE_TRUNCA32X2F64S(vyh,vyl,-6);
    vnw = AE_SUB32(16, vnw);
    vnw = AE_SLAI32(vnw, 25);/*Q.25*/
    vyw = AE_ADD32(vyw, vnw);/*(log2(x0)+(1/x0)*log2(e)*(x1-x0))+nsa, Q.25*/
    AE_MOVT32X2(vyw, viw, inf);
    AE_SA32X2_IP(vyw, y_align, py);
  }
  AE_SA64POS_FP(y_align, py);

  if(N&1)
  {
    vxw=AE_L32_I((const ae_int32*) px, 0);/*x,Q16.15*/
    inf = AE_LE32(vxw, vzw);
    /* Normalize x*/
    nsal = AE_NSAZ32_L(vxw);
    vnw = AE_MOVDA32X2(nsal, nsal);/*Q.0*/
    AE_MOVSARA7X2(nsal,nsal);
    vxw = AE_SLAS32S(vxw);/*x, Q.31*/
    vxw = AE_SUB32(vxw, vhw);
    vdw = AE_AND32(vxw, vmw);
    vdw = AE_SUB32(vdw, vsw);
    vdw = AE_SLAI32(vdw, 2);       /*x1-x0, Q.33 */
    vxw = AE_SRAI32(vxw, 23);
    vxw = AE_SLAI32(vxw, 3);
    offl= AE_MOVAD32_L(vxw);
    vxw = AE_L32X2_X((const ae_int32x2 *)log2_table, offl);
    vyl = AE_CVT64F32_H(vxw);     /* Q.63 */
    AE_MULAF32S_LL(vyl, vxw, vdw);   /* log2(x0)+(1/x0)*log2(e)*(x1-x0), Q.29 * Q.33 -> Q.63 */
    vyw=AE_TRUNCA32X2F64S(vyl,vyl,-6);
    vnw = AE_SUB32(16, vnw);
    vnw = AE_SLAI32(vnw, 25);/*Q.25*/
    vyw = AE_ADD32(vyw, vnw);/*(log2(x0)+(1/x0)*log2(e)*(x1-x0))+nsa, Q.25*/
    AE_MOVT32X2(vyw, viw, inf);
    AE_S32_L_I(vyw, (ae_int32*)py, 0);
  }
}
