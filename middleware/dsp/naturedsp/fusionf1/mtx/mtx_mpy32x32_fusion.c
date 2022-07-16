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
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and y. The columnar dimension of x must match the row dimension of y. 
  The resulting matrix has the same number of rows as x and the same number 
  of columns as y.

  NOTES:
  In the fixed-point routines, rows of matrices z and y may be stored in 
  non consecutive manner. Matrix x will have all the elements in contiguous 
  memory locations.

  Functions require scratch memory for storing intermediate data. This 
  scratch memory area should be aligned on 8 byte boundary and its size 
  is calculated by macros SCRATCH_MTX_MPY32X32(M,N,P), 
  SCRATCH_MTX_MPY24X24(M,N,P), SCRATCH_MTX_MPY16X16(M,N,P)

  Two versions of functions available: regular version (mtx_mpy32x32, 
  mtx_mpy24x24, mtx_mpy16x16, mtx_mpyf) with arbitrary arguments and 
  faster version (mtx_mpy32x32_fast, mtx_mpy24x24_fast, 
  mtx_mpy16x16_fast, mtx_mpyf_fast) that apply some restrictions.

  Precision: 
  32x32 32-bit inputs, 32-bit output
  24x24 24-bit inputs, 24-bit output
  16x16 16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N]      input matrix,Q31, Q15 or floating point
  y[N][P]     input matrix y. For fixed point routines, these are N 
              vectors of size P,Q31 or Q15. For floating point, this 
              is just a matrix of size NxP.
  M           number of rows in matrix x and z
  N           number of columns in matrix x and number of rows in matrix y
  P           number of columns in matrices y and z
  lsh         additional left shift
  Output:
  z[M][P]     output matrix z. For fixed point routines, these are M 
              vectors of size P Q31 or Q15. For floating point, this 
              is single matrix of size MxP
  Scratch:
  pScr        Scratch memory area with size in bytes defined by macros 
              SCRATCH_MTX_MPY32X32, SCRATCH_MTX_MPY24X24, 
              SCRATCH_MTX_MPY16X16


  Restrictions:
  For regular routines (mtx_mpy32x32,mtx_mpy24x24, mtx_mpy16x16, mtx_mpyf):
  x,y,z should not overlap

  For faster routines (mtx_mpy32x32_fast, mtx_mpy24x24_fast, 
  mtx_mpy16x16_fast, mtx_mpyf_fast):
  x,y,z should not overlap
  x - aligned on 8-byte boundary
  all rows which addresses are written to y[] - aligned on 8-byte boundary
  N is a multiple of 4,M=8,P=2  

-------------------------------------------------------------------------*/
void mtx_mpy32x32(void* pScr,
              int32_t** restrict z,
        const int32_t*  restrict x,
        const int32_t** restrict y,
        int M, int N, int P, int lsh)
{
  const ae_int32x2    *  restrict py = (const ae_int32x2    *)y;
        ae_int32      *   pz = (      ae_int32      *)z;
  const ae_int32x2    *   px = (const ae_int32x2      *)x;
  ae_valign      y_align,x_align;
  int         m, n, p;
  int         K, offa, offb;
  ae_int32x2    vpw;
  ae_int32x2    vxl, val, vbl, vcl, vdl, zl;
  ae_int32 *    Z1;
  ae_int32 *    Z2;
  ae_int32 *    T1;
  ae_int32 *    T2;
  ae_int32 *    Y1;
  if (N<0)    /* exceptional situation */
  {
    for (m=0; m<M; m++) 
      for (p=0; p<P; p++)
        z[m][p]=0;
    return ;
  }

  K = ((N+1)&(~1));
  zl = 0;

  for(p=0; p<P; p+=2)
  {
    py = (const ae_int32x2    *)y;
    pz = (      ae_int32   *)z; 
    Z1 = (ae_int32 *)pScr; 
    Z2 = Z1 + K;
    y_align = AE_LA64_PP(py);
    for(n=0; n<(N>>1); ++n)
    {
      ae_int32x2 a0,b0;
      AE_LA32X2_IP(vpw, y_align, py); 
      offa = AE_MOVAD32_H(vpw);
      offb = AE_MOVAD32_L(vpw);
      T1 = (ae_int32 *)(offa)+p;
      T2 = (ae_int32 *)(offb)+p;
      AE_L32_IP(val,T1, 4);
      AE_L32_XP(vbl,T1, 4);
      AE_L32_IP(vcl,T2, 4);
      AE_L32_XP(vdl,T2, 4);
      a0 = AE_SEL32_LL(val, vcl);
      b0 = AE_SEL32_LL(vbl, vdl);
      AE_S32X2_IP(a0, castxcc(ae_int32x2, Z1), 8);
      AE_S32X2_IP(b0, castxcc(ae_int32x2, Z2), 8);
    }
    if (N&1) 
    { 
      AE_L32_IP(vpw, castxcc(ae_int32,py), 0); 
      offa = AE_MOVAD32_H(vpw);
      T1=(ae_int32 *)(offa)+p;
      AE_L32_IP(val, T1, 4);
      AE_L32_IP(vbl, T1, 0);
      AE_S32_L_IP(val, Z1, 4);
      AE_S32_L_IP(vbl, Z2, 4);
      AE_S32_L_IP(zl, Z1, 0);
      AE_S32_L_IP(zl, Z2, 0);
    }
    for (m = 0; m<M; m++)
    {
      ae_f64 vah, vbh;
      ae_int64 vha, vhb;
      ae_int32x2  vy1, vy2;

      vah = AE_ZERO64();
      vbh = AE_ZERO64();
      Z1 = (ae_int32 *)pScr;
      Z2 = Z1 + K;

      px = (const ae_int32x2 *)x;
      px = (ae_int32x2 *)((ae_int32 *)px + m*N);
      x_align = AE_LA64_PP(px);
      for (n = 0; n<(K >> 1); ++n)
      {
        AE_LA32X2_IP(vxl, x_align, px);
        AE_L32X2_IP(val, castxcc(ae_int32x2, Z1), 8);
        AE_L32X2_IP(vbl, castxcc(ae_int32x2, Z2), 8);
        AE_MULAF32R_HH(vah, val, vxl);
        AE_MULAF32R_LL(vah, val, vxl);
        AE_MULAF32R_HH(vbh, vbl, vxl);
        AE_MULAF32R_LL(vbh, vbl, vxl);
      }

      vha = AE_SLAA64(vah, lsh);
      vhb = AE_SLAA64(vbh, lsh);
      vy1 = AE_ROUND32F48SASYM(vha); 
      vy2 = AE_ROUND32F48SASYM(vhb);
      AE_L32_IP(vpw, pz, 4);
      offb = AE_MOVAD32_H(vpw);
      Y1 = (ae_int32 *)(offb)+p;
      AE_S32_L_IP(vy1, Y1, 4);
      if (p + 1<P) AE_S32_L_IP(vy2, Y1, 4);
    }
  }
} /* mtx_mpy32x32() */
