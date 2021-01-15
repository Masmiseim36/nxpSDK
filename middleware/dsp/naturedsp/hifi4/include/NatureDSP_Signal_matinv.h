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
#ifndef __NATUREDSP_SIGNAL_MATINV_H__
#define __NATUREDSP_SIGNAL_MATINV_H__

#include "NatureDSP_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
  Matrix Decomposition/Inversion:
  mtx_inv              Gauss-Jordan matrix inversion
  mtx_gjelim           Matrix equation solver by Gauss-Jordan elimination
===========================================================================*/

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
void mtx_inv2x2f  (void* pScr, float32_t *x);
void mtx_inv3x3f  (void* pScr, float32_t *x);
void mtx_inv4x4f  (void* pScr, float32_t *x);
void mtx_inv6x6f  (void* pScr, float32_t *x);
void mtx_inv8x8f  (void* pScr, float32_t *x);
void mtx_inv10x10f(void* pScr, float32_t *x);
int  mtx_inv2x2_32x32  (void* pScr, int32_t *x, int qX);
int  mtx_inv3x3_32x32  (void* pScr, int32_t *x, int qX);
int  mtx_inv4x4_32x32  (void* pScr, int32_t *x, int qX);
int  mtx_inv6x6_32x32  (void* pScr, int32_t *x, int qX);
int  mtx_inv8x8_32x32  (void* pScr, int32_t *x, int qX);
int  mtx_inv10x10_32x32(void* pScr, int32_t *x, int qX);
int  cmtx_inv2x2_32x32  (void* pScr, complex_fract32 *x, int qX);
int  cmtx_inv3x3_32x32  (void* pScr, complex_fract32 *x, int qX);
int  cmtx_inv4x4_32x32  (void* pScr, complex_fract32 *x, int qX);
int  cmtx_inv6x6_32x32  (void* pScr, complex_fract32 *x, int qX);
int  cmtx_inv8x8_32x32  (void* pScr, complex_fract32 *x, int qX);
int  cmtx_inv10x10_32x32(void* pScr, complex_fract32 *x, int qX);
// scratch allocation functions
size_t mtx_inv2x2f_getScratchSize        ();
size_t mtx_inv3x3f_getScratchSize        ();
size_t mtx_inv4x4f_getScratchSize        ();
size_t mtx_inv6x6f_getScratchSize        ();
size_t mtx_inv8x8f_getScratchSize        ();
size_t mtx_inv10x10f_getScratchSize      ();
size_t mtx_inv2x2_32x32_getScratchSize   ();
size_t mtx_inv3x3_32x32_getScratchSize   ();
size_t mtx_inv4x4_32x32_getScratchSize   ();
size_t mtx_inv6x6_32x32_getScratchSize   ();
size_t mtx_inv8x8_32x32_getScratchSize   ();
size_t mtx_inv10x10_32x32_getScratchSize ();
size_t cmtx_inv2x2_32x32_getScratchSize  ();
size_t cmtx_inv3x3_32x32_getScratchSize  ();
size_t cmtx_inv4x4_32x32_getScratchSize  ();
size_t cmtx_inv6x6_32x32_getScratchSize  ();
size_t cmtx_inv8x8_32x32_getScratchSize  ();
size_t cmtx_inv10x10_32x32_getScratchSize();

/*-------------------------------------------------------------------------
  These functions implement Gauss elimination elimination process with full 
  pivoting to findsolution of linear equations A*y=x
  
  Fixed point version takes representation of input matrix/vector and forms 
  representation of output vector with proper scaling.

  Precision: 
  32x32 32-bit input, 32-bit output

  Input:
  A[N*N]      input matrix, representation is defined by parameter qA
  x[N]        input rigth side of equation, representation is defined by 
              parameter qX
  qA          input matrix A representation (for fixed point API only)
  qX          input vector x representation (for fixed point API only)
  Output:
  y[N]        output vector
  Temporary:
  pScr        scratch memory. Size in bytes is defined by corresponding 
              scratch allocation function 
  N is 2,3,4,6,8,10

  Returned value: fixed point functions return fixed-point representation 
                  of resulted vector
  Restrictions:
  none
-------------------------------------------------------------------------*/
int mtx_gjelim2x2_32x32   (void* pScr, int32_t *y, const int32_t* A,const int32_t * x, int qA, int qX);
int mtx_gjelim3x3_32x32   (void* pScr, int32_t *y, const int32_t* A,const int32_t * x, int qA, int qX);
int mtx_gjelim4x4_32x32   (void* pScr, int32_t *y, const int32_t* A,const int32_t * x, int qA, int qX);
int mtx_gjelim6x6_32x32   (void* pScr, int32_t *y, const int32_t* A,const int32_t * x, int qA, int qX);
int mtx_gjelim8x8_32x32   (void* pScr, int32_t *y, const int32_t* A,const int32_t * x, int qA, int qX);
int mtx_gjelim10x10_32x32 (void* pScr, int32_t *y, const int32_t* A,const int32_t * x, int qA, int qX);
int cmtx_gjelim2x2_32x32  (void* pScr, complex_fract32 *y, const complex_fract32 *A, const complex_fract32 *x, int qA, int qX);
int cmtx_gjelim3x3_32x32  (void* pScr, complex_fract32 *y, const complex_fract32 *A, const complex_fract32 *x, int qA, int qX);
int cmtx_gjelim4x4_32x32  (void* pScr, complex_fract32 *y, const complex_fract32 *A, const complex_fract32 *x, int qA, int qX);
int cmtx_gjelim6x6_32x32  (void* pScr, complex_fract32 *y, const complex_fract32 *A, const complex_fract32 *x, int qA, int qX);
int cmtx_gjelim8x8_32x32  (void* pScr, complex_fract32 *y, const complex_fract32 *A, const complex_fract32 *x, int qA, int qX);
int cmtx_gjelim10x10_32x32(void* pScr, complex_fract32 *y, const complex_fract32 *A, const complex_fract32 *x, int qA, int qX);
// scratch allocation functions
size_t mtx_gjelim2x2_32x32_getScratchSize   ();
size_t mtx_gjelim3x3_32x32_getScratchSize   ();
size_t mtx_gjelim4x4_32x32_getScratchSize   ();
size_t mtx_gjelim6x6_32x32_getScratchSize   ();
size_t mtx_gjelim8x8_32x32_getScratchSize   ();
size_t mtx_gjelim10x10_32x32_getScratchSize ();
size_t cmtx_gjelim2x2_32x32_getScratchSize  ();
size_t cmtx_gjelim3x3_32x32_getScratchSize  ();
size_t cmtx_gjelim4x4_32x32_getScratchSize  ();
size_t cmtx_gjelim6x6_32x32_getScratchSize  ();
size_t cmtx_gjelim8x8_32x32_getScratchSize  ();
size_t cmtx_gjelim10x10_32x32_getScratchSize();

#ifdef __cplusplus
}
#endif

#endif/* __NATUREDSP_SIGNAL_MATINV_H__ */
