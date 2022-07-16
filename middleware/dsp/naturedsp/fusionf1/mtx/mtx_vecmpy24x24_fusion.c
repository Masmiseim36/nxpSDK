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
  Matrix Operations:
  mtx_mpy              Matrix Multiply
  mtx_vecmpy           Matrix by Vector Multiply
===========================================================================*/
/*-------------------------------------------------------------------------
  Matrix by Vector Multiply
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and vector y. 

  Two versions of functions available: regular version (mtx_vecmpy32x32, 
  mtx_vecmpy24x24, mtx_vecmpy16x16,mtx_vecmpyf) with arbitrary arguments 
  and faster version (mtx_vecmpy32x32_fast, mtx_vecmpy24x24_fast, 
  mtx_vecmpy16x16_fast, mtx_vecmpyf_fast) that apply some restrictions.

  Precision: 
  32x32 32-bit inputs, 32-bit output
  24x24 24-bit inputs, 24-bit output
  16x16 16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N] input matrix,Q31,Q15 or floating point
  y[N]   input vector,Q31,Q15 or floating point
  M      number of rows in matrix x
  N      number of columns in matrix x
  lsh    additional left shift
  Output:
  z[M]   output vector,Q31,Q15 or floating point

  Restriction:
  For regular routines (mtx_vecmpy24x24, mtx_vecmpy16x16, mtx_vecmpyf)
  x,y,z should not overlap

  For faster routines (mtx_vecmpy24x24_fast, mtx_vecmpy16x16_fast,
  mtx_vecmpyf_fast)
  x,y,z should not overlap
  x,y   aligned on 8-byte boundary
  N and M are multiples of 4
-------------------------------------------------------------------------*/
void mtx_vecmpy24x24 (  f24* restrict z,
               const f24* restrict x,
               const f24* restrict y,
               int M, int N, int lsh)

{
  const ae_f24x2      *  restrict px = (const ae_f24x2     *)x;
  const ae_f24x2      *  restrict py = (const ae_f24x2     *)y;
        ae_f24        *  restrict pz = (      ae_f24       *)z;
  
  ae_valign x_align,y_align;
  int m,n;
  ae_f24x2    vxl,val;
  if (N<0)    /* exceptional situation */
  {
        for (m=0; m<M; m++) z[m]=0;
        return ;
  }

  #ifdef COMPILER_XTENSA
    #pragma concurrent
  #endif
  for(m=0; m<M; m++)
  {
    ae_f64    vah;
    ae_int64  vha;
    ae_f24x2  vy1;
    
    vah = AE_ZERO();
    
    x_align = AE_LA64_PP(px);
    py = (const ae_f24x2   *)y;
    y_align = AE_LA64_PP(py);
    for(n=0; n<N-1; n+=2)
    {
      AE_LA32X2F24_IP(vxl, x_align, px); 
      AE_LA32X2F24_IP(val, y_align, py); 
      AE_MULAAFD24_HH_LL(vah,val,vxl);
    }
    if (N&1)
    {
      ae_f32x2 vxf,vaf;
      AE_L32F24_IP(vxl, castxcc(ae_f24,px), 4); 
      AE_L32F24_IP(val, castxcc(ae_f24,py), 4); 
      vxf = (vxl);
      vaf = (val);
      AE_MULAF32S_LL(vah,vxf,vaf);
    }
    vha = (vah);
    vha = AE_SLAA64(vha,lsh);
    vah = (vha);
    vy1 = AE_ROUNDSP24Q48ASYM(vah);
    AE_S32F24_L_IP(vy1,pz,4);
  }
} /* mtx_vecmpy24x24() */

