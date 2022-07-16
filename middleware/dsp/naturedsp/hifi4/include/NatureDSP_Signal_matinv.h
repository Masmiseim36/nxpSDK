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
  [c]matchol		       [real/complex] Cholesky
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


/*-------------------------------------------------------------------------
Cholesky MMSE Solver
Compute the MMSE solution for a system of linear equations A*x=B, where A is
an MxN real (complex) matrix with M>=N and rank(A)==N, x is an Nx1 vector of
unknowns, and B is an MxP right hand side vector. This task is accomplished
in 3 steps:
-   Cholesky decomposition is applied to the matrix of normal equations
system, which results in an upper triangular matrix R[NxN] with real and
positive numbers on the main diagonal
-   Forward substitution step: solve R'*y=A'*B for Nx1 vector y.
-   Backward substitution step: solve the system R*x=y for the Nx1 vector of
unknowns x.
For a single MxN matrix A, these 3 steps may be done simultaneously for P
variants of Mx1 right hand side column vectors b gathered into an MxP input
matrix B. MMSE solution is computed independently for each of P columns,
with resulting column vectors forming the solution matrix x of size NxP.
Fixed point API requires explicit setting of fixed point representation of 
input/output matrices as well as for internal temporary matrices such as R 
(Cholesky decomposition) and y (decision of R'y=(A'*B))

Precision:
f     single precision floating point
32x32 32-bit inputs/outputs

Input:
sigma2      Regularization term. For fixed point, the representation 
            should be Q(2*qA-30)
qRA         qR-qA; difference between fixed point representations of R
            and A matrices (for the fixed point API only). Should be 
            equal or less than 0 (typically -2).
qYBRA       qY-qB+qR-qA, combination of fixed point representations of 
            matrices y,B,R and A (for the fixed point API only)
qXYR        combination of fixed point representation (matrices R, x and y) 
            qXYR=qX-qY+qR
A[M*N]      matrix A. . For fixed point, the representation should be Q(qA)
B[M*P]      Original right-side matrix B. For fixed point, the representation 
            should be Q(qB)
Output:
x[N*P]      Decision matrix x. For fixed point, the representation 
            is Q(qX)
Temporary:
pScr        Scratch data

N = M = 4, 6, 8, 10
P = 1

Restrictions:
All matrices should not overlap and be aligned on 8-bytes
boundary
---------------------------------------------------------------------------*/
void  cmatcholmmsesolver4x4f(void * pScr,
    complex_float * x,
    const complex_float * A,
    const complex_float * B,
    const float32_t sigma2);
void  cmatcholmmsesolver6x6f(void * pScr,
    complex_float * x,
    const complex_float * A,
    const complex_float * B,
    const float32_t sigma2);
void  cmatcholmmsesolver8x8f(void * pScr,
    complex_float * x,
    const complex_float * A,
    const complex_float * B,
    const float32_t sigma2);
void  cmatcholmmsesolver10x10f(void * pScr,
    complex_float * x,
    const complex_float * A,
    const complex_float * B,
    const float32_t sigma2);

void  matcholmmsesolver4x4f(void * pScr,
    float32_t * x,
    const float32_t * A,
    const float32_t * B,
    const float32_t  sigma2);
void  matcholmmsesolver6x6f(void * pScr,
    float32_t * x,
    const float32_t * A,
    const float32_t * B,
    const float32_t  sigma2);
void  matcholmmsesolver8x8f(void * pScr,
    float32_t * x,
    const float32_t * A,
    const float32_t * B,
    const float32_t  sigma2);
void  matcholmmsesolver10x10f(void * pScr,
    float32_t * x,
    const float32_t * A,
    const float32_t * B,
    const float32_t  sigma2);

/* scratch allocation functions */
size_t cmatcholmmsesolver4x4f_getScratchSize();
size_t cmatcholmmsesolver6x6f_getScratchSize();
size_t cmatcholmmsesolver8x8f_getScratchSize();
size_t cmatcholmmsesolver10x10f_getScratchSize();

size_t  matcholmmsesolver4x4f_getScratchSize();
size_t  matcholmmsesolver6x6f_getScratchSize();
size_t  matcholmmsesolver8x8f_getScratchSize();
size_t  matcholmmsesolver10x10f_getScratchSize();

/* 32-bit fixed point API */
void  cmatcholmmsesolver4x4_32x32(void * pScr,
          complex_fract32 * x,
    const complex_fract32 * A,
    const complex_fract32 * B,
    const int32_t sigma2, int qRA,int qYBRA,int qXYR);
void  cmatcholmmsesolver6x6_32x32(void * pScr,
          complex_fract32 * x,
    const complex_fract32 * A,
    const complex_fract32 * B,
    const int32_t sigma2, int qRA,int qYBRA,int qXYR);
void  cmatcholmmsesolver8x8_32x32(void * pScr,
          complex_fract32 * x,
    const complex_fract32 * A,
    const complex_fract32 * B,
    const int32_t sigma2, int qRA,int qYBRA,int qXYR);
void  cmatcholmmsesolver10x10_32x32(void * pScr,
          complex_fract32 * x,
    const complex_fract32 * A,
    const complex_fract32 * B,
    const int32_t sigma2, int qRA,int qYBRA,int qXYR);

void  matcholmmsesolver4x4_32x32(void * pScr,
          int32_t * x,
    const int32_t * A,
    const int32_t * B,
    const int32_t  sigma2, int qRA,int qYBRA,int qXYR);
void  matcholmmsesolver6x6_32x32(void * pScr,
          int32_t * x,
    const int32_t * A,
    const int32_t * B,
    const int32_t  sigma2, int qRA,int qYBRA,int qXYR);
void  matcholmmsesolver8x8_32x32(void * pScr,
          int32_t * x,
    const int32_t * A,
    const int32_t * B,
    const int32_t  sigma2, int qRA,int qYBRA,int qXYR);
void  matcholmmsesolver10x10_32x32(void * pScr,
          int32_t * x,
    const int32_t * A,
    const int32_t * B,
    const int32_t  sigma2, int qRA,int qYBRA,int qXYR);

/* scratch allocation functions */
size_t cmatcholmmsesolver4x4_32x32_getScratchSize();
size_t cmatcholmmsesolver6x6_32x32_getScratchSize();
size_t cmatcholmmsesolver8x8_32x32_getScratchSize();
size_t cmatcholmmsesolver10x10_32x32_getScratchSize();

size_t  matcholmmsesolver4x4_32x32_getScratchSize();
size_t  matcholmmsesolver6x6_32x32_getScratchSize();
size_t  matcholmmsesolver8x8_32x32_getScratchSize();
size_t  matcholmmsesolver10x10_32x32_getScratchSize();

/*-------------------------------------------------------------------------
Cholesky decomposition for Pseudo-Inversion
Apply the Cholesky decomposition to the matrix of normal equations system
associated with a complex-valued least squares problem:
A[MxN]*X[NxP]=B[MxP], MxN.
The decomposition results in an upper triangular complex matrix R[NxN]
with real and positive numbers on the main diagonal, such that
R'*R = A'*A + sigma2*I[NxN], where ' denotes the conjugate transpose of
a matrix, and sigma2*I[NxN] is the NxN identity matrix multiplied with
the regularization term.

Precision:
f     single precision floating point
32x32 32-bit inputs/outputs

Input:
A[M*N]          matrix A. For fixed point API, the representation is Q(qA)
sigma2          regularization term. For fixed point, the representation 
                should be Q(2*qA-30)
qRA             qR-qA; difference between fixed point representations of R
                and A matrices (for the fixed point API only). Should be 
                equal or less than 0 (typically -2).
Output:
R[((N+1)*N)/2]  upper triangular matrix R
D[N]            reciprocals of the main diagonal. NOTE: for the fixed point API,
                these data are stored internally in special format with separate
                mantissa and exponent for better accuracy and dynamic range 
                control. So, even for the real data, they stored as pairs of 2
                integers and packed to the complex_fract32 format
Temporary:
pScr            Scratch memory

N = M = 4, 6, 8, 10

Restrictions:
All matrices and scratch memory should not overlap and be aligned on
8-bytes boundary
---------------------------------------------------------------------------*/
/* single-matrix API */
void  cmatcholdecomp4x4f(void* pScr,
                    complex_float *R,
                    complex_float * D,
                    const complex_float * A,
                    const float32_t  sigma2);
void  cmatcholdecomp6x6f(void* pScr,
                    complex_float *R,
                    complex_float * D,
                    const complex_float * A,
                    const float32_t  sigma2);
void  cmatcholdecomp8x8f(void* pScr,
                    complex_float *R,
                    complex_float * D,
                    const complex_float * A,
                    const float32_t  sigma2);
void  cmatcholdecomp10x10f(void* pScr,
                    complex_float *R,
                    complex_float * D,
                    const complex_float * A,
                    const float32_t  sigma2);

void  matcholdecomp4x4f(void* pScr,
                    float32_t *R,
                    float32_t * D,
                    const float32_t * A,
                    const float32_t  sigma2);
void  matcholdecomp6x6f(void* pScr,
                    float32_t *R,
                    float32_t * D,
                    const float32_t * A,
                    const float32_t  sigma2);
void  matcholdecomp8x8f(void* pScr,
                    float32_t *R,
                    float32_t * D,
                    const float32_t * A,
                    const float32_t  sigma2);
void  matcholdecomp10x10f(void* pScr,
                    float32_t *R,
                    float32_t * D,
                    const float32_t * A,
                    const float32_t  sigma2);
/* scratch allocation functions */
size_t  cmatcholdecomp4x4f_getScratchSize();
size_t  cmatcholdecomp6x6f_getScratchSize();
size_t  cmatcholdecomp8x8f_getScratchSize();
size_t  cmatcholdecomp10x10f_getScratchSize();

size_t   matcholdecomp4x4f_getScratchSize();
size_t   matcholdecomp6x6f_getScratchSize();
size_t   matcholdecomp8x8f_getScratchSize();
size_t   matcholdecomp10x10f_getScratchSize();

/* 32-bit fixed point, complex data */
void cmatcholdecomp4x4_32x32(void * restrict pScr,
                      complex_fract32 * restrict R,
                      complex_fract32 * restrict D,
                const complex_fract32 * restrict A,
                      int32_t sigma2,
                      int qRA);
void cmatcholdecomp6x6_32x32(void * restrict pScr,
                      complex_fract32 * restrict R,
                      complex_fract32 * restrict D,
                const complex_fract32 * restrict A,
                      int32_t sigma2,
                      int qRA);
void cmatcholdecomp8x8_32x32(void * restrict pScr,
                      complex_fract32 * restrict R,
                      complex_fract32 * restrict D,
                const complex_fract32 * restrict A,
                      int32_t sigma2,
                      int qRA);
void cmatcholdecomp10x10_32x32(void * restrict pScr,
                      complex_fract32 * restrict R,
                      complex_fract32 * restrict D,
                const complex_fract32 * restrict A,
                      int32_t sigma2,
                      int qRA);

/* 32-bit fixed point, real data */
void matcholdecomp4x4_32x32(void * restrict pScr,
                      int32_t * restrict R,
                      complex_fract32 * restrict D,
                const int32_t * restrict A,
                      int32_t sigma2,
                      int qRA);
void matcholdecomp6x6_32x32(void * restrict pScr,
                      int32_t * restrict R,
                      complex_fract32 * restrict D,
                const int32_t * restrict A,
                      int32_t sigma2,
                      int qRA);
void matcholdecomp8x8_32x32(void * restrict pScr,
                      int32_t * restrict R,
                      complex_fract32 * restrict D,
                const int32_t * restrict A,
                      int32_t sigma2,
                      int qRA);
void matcholdecomp10x10_32x32(void * restrict pScr,
                      int32_t * restrict R,
                      complex_fract32 * restrict D,
                const int32_t * restrict A,
                      int32_t sigma2,
                      int qRA);

/* scratch allocation functions */
size_t  cmatcholdecomp4x4_32x32_getScratchSize();
size_t  cmatcholdecomp6x6_32x32_getScratchSize();
size_t  cmatcholdecomp8x8_32x32_getScratchSize();
size_t  cmatcholdecomp10x10_32x32_getScratchSize();

size_t   matcholdecomp4x4_32x32_getScratchSize();
size_t   matcholdecomp6x6_32x32_getScratchSize();
size_t   matcholdecomp8x8_32x32_getScratchSize();
size_t   matcholdecomp10x10_32x32_getScratchSize();

/*-------------------------------------------------------------------------
Cholesky Forward Substitution for Pseudo-inversion
These functions make forward recursion stage of pseudo-inversion. They use
Cholesky decomposition R[NxN] of original matrices A[MxN]

Precision:
f     single precision floating point
32x32 32-bit inputs/outputs

Input:
R[((N+1)*N)/2]
            upper triangular matrix R. For fixed point, representation is Q(qR)
A[M*N]      matrix A. For fixed point, representation is Q(qA)
B[M*P]      original right-side matrix B. For fixed point, representation is Q(qB)
D[N]        reciprocals of main diagonal. NOTE: for the fixed point API,
            these data are stored internally in special format with separate
            mantissa and exponent for better accuracy and dynamic range 
            control. So, even for the real data, they stored as pairs of 2
            integers and packed to the complex_fract32 format
qYBRA       qY-qB+qR-qA, combination of fixed point representations of 
            matrices y,B,R and A (for the fixed point API only)
Output:
y[N*P]      Decision matrix y. For fixed point, representation is Q(qY)
Temporary:
pScr        Scratch memory

N = M = 4, 6, 8, 10
P = 1

Restrictions:
All matrices and scratch memory should not overlap and be aligned on
8-bytes boundary
---------------------------------------------------------------------------*/
/* single-matrix API */
void  cmatcholfwdsubst4x4f(void * pScr,
                        complex_float * y,
                        const complex_float * R,
                        const complex_float * D,
                        const complex_float * A,
                        const complex_float * B);
void  cmatcholfwdsubst6x6f(void * pScr,
                        complex_float * y,
                        const complex_float * R,
                        const complex_float * D,
                        const complex_float * A,
                        const complex_float * B);
void  cmatcholfwdsubst8x8f(void * pScr,
                        complex_float * y,
                        const complex_float * R,
                        const complex_float * D,
                        const complex_float * A,
                        const complex_float * B);
void  cmatcholfwdsubst10x10f(void * pScr,
                        complex_float * y,
                        const complex_float * R,
                        const complex_float * D,
                        const complex_float * A,
                        const complex_float * B);

void  matcholfwdsubst4x4f(void * pScr,
                        float32_t * y,
                        const float32_t * R,
                        const float32_t * D,
                        const float32_t * A,
                        const float32_t * B);
void  matcholfwdsubst6x6f(void * pScr,
                        float32_t * y,
                        const float32_t * R,
                        const float32_t * D,
                        const float32_t * A,
                        const float32_t * B);
void  matcholfwdsubst8x8f(void * pScr,
                        float32_t * y,
                        const float32_t * R,
                        const float32_t * D,
                        const float32_t * A,
                        const float32_t * B);
void  matcholfwdsubst10x10f(void * pScr,
                        float32_t * y,
                        const float32_t * R,
                        const float32_t * D,
                        const float32_t * A,
                        const float32_t * B);

/* scratch allocation functions */
size_t  cmatcholfwdsubst4x4f_getScratchSize();
size_t  cmatcholfwdsubst6x6f_getScratchSize();
size_t  cmatcholfwdsubst8x8f_getScratchSize();
size_t  cmatcholfwdsubst10x10f_getScratchSize();

size_t   matcholfwdsubst4x4f_getScratchSize();
size_t   matcholfwdsubst6x6f_getScratchSize();
size_t   matcholfwdsubst8x8f_getScratchSize();
size_t   matcholfwdsubst10x10f_getScratchSize();

/* 32-bit fixed point complex data */
void cmatcholfwdsubst4x4_32x32(void * restrict pScr,
                         complex_fract32* restrict y,
                   const complex_fract32* restrict R,
                   const complex_fract32* restrict D,
                   const complex_fract32* restrict A,
                   const complex_fract32* restrict B,
                         int qYBRA);
void cmatcholfwdsubst6x6_32x32(void * restrict pScr,
                         complex_fract32* restrict y,
                   const complex_fract32* restrict R,
                   const complex_fract32* restrict D,
                   const complex_fract32* restrict A,
                   const complex_fract32* restrict B,
                         int qYBRA);
void cmatcholfwdsubst8x8_32x32(void * restrict pScr,
                         complex_fract32* restrict y,
                   const complex_fract32* restrict R,
                   const complex_fract32* restrict D,
                   const complex_fract32* restrict A,
                   const complex_fract32* restrict B,
                         int qYBRA);
void cmatcholfwdsubst10x10_32x32(void * restrict pScr,
                         complex_fract32* restrict y,
                   const complex_fract32* restrict R,
                   const complex_fract32* restrict D,
                   const complex_fract32* restrict A,
                   const complex_fract32* restrict B,
                         int qYBRA);
/* 32-bit fixed point real data */
void matcholfwdsubst4x4_32x32(void * restrict pScr,
                         int32_t* restrict y,
                   const int32_t* restrict R,
                   const complex_fract32* restrict D,
                   const int32_t* restrict A,
                   const int32_t* restrict B,
                         int qYBRA);
void matcholfwdsubst6x6_32x32(void * restrict pScr,
                         int32_t* restrict y,
                   const int32_t* restrict R,
                   const complex_fract32* restrict D,
                   const int32_t* restrict A,
                   const int32_t* restrict B,
                         int qYBRA);
void matcholfwdsubst8x8_32x32(void * restrict pScr,
                         int32_t* restrict y,
                   const int32_t* restrict R,
                   const complex_fract32* restrict D,
                   const int32_t* restrict A,
                   const int32_t* restrict B,
                         int qYBRA);
void matcholfwdsubst10x10_32x32(void * restrict pScr,
                         int32_t* restrict y,
                   const int32_t* restrict R,
                   const complex_fract32* restrict D,
                   const int32_t* restrict A,
                   const int32_t* restrict B,
                         int qYBRA);
/* scratch allocation functions */
size_t  cmatcholfwdsubst4x4_32x32_getScratchSize();
size_t  cmatcholfwdsubst6x6_32x32_getScratchSize();
size_t  cmatcholfwdsubst8x8_32x32_getScratchSize();
size_t  cmatcholfwdsubst10x10_32x32_getScratchSize();

size_t   matcholfwdsubst4x4_32x32_getScratchSize();
size_t   matcholfwdsubst6x6_32x32_getScratchSize();
size_t   matcholfwdsubst8x8_32x32_getScratchSize();
size_t   matcholfwdsubst10x10_32x32_getScratchSize();

/*-------------------------------------------------------------------------
Cholesky Backward Substitution for Pseudo-inversion
These functions make backward substitution stage of pseudo-inversion. They
use Cholesky decomposition of original matrices and results of forward
substitution.

Precision:
f     single precision floating point
32x32 32-bit inputs/outputs

Input:
R[((N+1)*N)/2]
            Cholesky upper-triangle matrix R. For fixed point API, the 
            representation is Q(qR)
y[N*P]      Results of forward substitution stage. For fixed point API, 
            the representation is Q(qY)
D[N]        sequence of reciprocals of main diagonal R. NOTE: for the 
            fixed point API, these data are stored internally in special 
            format with separate mantissa and exponent for better accuracy 
            and dynamic range control. So, even for the real data, they 
            stored as pairs of 2 integers and packed to the complex_fract32 
            format
qXYR        combination of fixed point representation (matrices R, x and y) 
            qXYR=qX-qY+qR (for fixed point API only)
Output:
x[N*P]      Decision matrix x. For fixed point API, the representation is Q(qX)

N = 4, 6, 8, 10
P = 1

Restrictions:
All matrices and scratch should not overlap and be aligned on
8-bytes boundary
---------------------------------------------------------------------------*/
/* single-matrix API */
void cmatcholbkwsubst4x4f(void * pScr,
                        complex_float * x,
                        const complex_float * R,
                        const complex_float * D,
                        const complex_float * y);
void cmatcholbkwsubst6x6f(void * pScr,
                        complex_float * x,
                        const complex_float * R,
                        const complex_float * D,
                        const complex_float * y);
void cmatcholbkwsubst8x8f(void * pScr,
                        complex_float * x,
                        const complex_float * R,
                        const complex_float * D,
                        const complex_float * y);
void cmatcholbkwsubst10x10f(void * pScr,
                        complex_float * x,
                        const complex_float * R,
                        const complex_float * D,
                        const complex_float * y);

void  matcholbkwsubst4x4f(void * pScr,
                        float32_t * x,
                        const float32_t * R,
                        const float32_t * D,
                        const float32_t * y);
void  matcholbkwsubst6x6f(void * pScr,
                        float32_t * x,
                        const float32_t * R,
                        const float32_t * D,
                        const float32_t * y);
void  matcholbkwsubst8x8f(void * pScr,
                        float32_t * x,
                        const float32_t * R,
                        const float32_t * D,
                        const float32_t * y);
void  matcholbkwsubst10x10f(void * pScr,
                        float32_t * x,
                        const float32_t * R,
                        const float32_t * D,
                        const float32_t * y);

/* scratch allocation functions */
size_t cmatcholbkwsubst4x4f_getScratchSize();
size_t cmatcholbkwsubst6x6f_getScratchSize();
size_t cmatcholbkwsubst8x8f_getScratchSize();
size_t cmatcholbkwsubst10x10f_getScratchSize();

size_t  matcholbkwsubst4x4f_getScratchSize();
size_t  matcholbkwsubst6x6f_getScratchSize();
size_t  matcholbkwsubst8x8f_getScratchSize();
size_t  matcholbkwsubst10x10f_getScratchSize();

/* 32-bit complex data */
void cmatcholbkwsubst4x4_32x32(void *pScr,
                 complex_fract32* restrict x,
           const complex_fract32* restrict R,
           const complex_fract32* restrict D,
           const complex_fract32* restrict y,
                 int qXYR);
void cmatcholbkwsubst6x6_32x32(void *pScr,
                 complex_fract32* restrict x,
           const complex_fract32* restrict R,
           const complex_fract32* restrict D,
           const complex_fract32* restrict y,
                 int qXYR);
void cmatcholbkwsubst8x8_32x32(void *pScr,
                 complex_fract32* restrict x,
           const complex_fract32* restrict R,
           const complex_fract32* restrict D,
           const complex_fract32* restrict y,
                 int qXYR);
void cmatcholbkwsubst10x10_32x32(void *pScr,
                 complex_fract32* restrict x,
           const complex_fract32* restrict R,
           const complex_fract32* restrict D,
           const complex_fract32* restrict y,
                 int qXYR);
/* 32-bit real data */
void matcholbkwsubst4x4_32x32(void *pScr,
                 int32_t* restrict x,
           const int32_t* restrict R,
           const complex_fract32* restrict D,
           const int32_t* restrict y,
                 int qXYR);
void matcholbkwsubst6x6_32x32(void *pScr,
                 int32_t* restrict x,
           const int32_t* restrict R,
           const complex_fract32* restrict D,
           const int32_t* restrict y,
                 int qXYR);
void matcholbkwsubst8x8_32x32(void *pScr,
                 int32_t* restrict x,
           const int32_t* restrict R,
           const complex_fract32* restrict D,
           const int32_t* restrict y,
                 int qXYR);
void matcholbkwsubst10x10_32x32(void *pScr,
                 int32_t* restrict x,
           const int32_t* restrict R,
           const complex_fract32* restrict D,
           const int32_t* restrict y,
                 int qXYR);

/* scratch allocation functions */
size_t cmatcholbkwsubst4x4_32x32_getScratchSize();
size_t cmatcholbkwsubst6x6_32x32_getScratchSize();
size_t cmatcholbkwsubst8x8_32x32_getScratchSize();
size_t cmatcholbkwsubst10x10_32x32_getScratchSize();

size_t  matcholbkwsubst4x4_32x32_getScratchSize();
size_t  matcholbkwsubst6x6_32x32_getScratchSize();
size_t  matcholbkwsubst8x8_32x32_getScratchSize();
size_t  matcholbkwsubst10x10_32x32_getScratchSize();

/*-------------------------------------------------------------------------
Matrix (Pseudo) Inversion
Obtain Left Inverse of a matrix using Cholesky Decomposition
The result is matrix x = A^-1
Fixed point API requires explicit setting of fixed point representation of 
input/output matrices as well as for internal temporary matrices such as R 
(Cholesky decomposition) and y (decision of R'y=(A'*B))


Precision:
f     single precision floating point
32x32 32-bit inputs/outputs

Input:
A[M*N]      matrix A, for fixed point API, the representation is Q(qA)
sigma2      regularization term, for fixed point API, the 
            representation is Q(2*qA-30)
qRA         qR-qA; difference between fixed point representations of R
            and A matrices (for the fixed point API only). Should be 
            equal or less than 0 (typically -2).
qYBRA       qY-qB+qR-qA, combination of fixed point representations of 
            matrices y, B, R and A (for the fixed point API only). Since 
            for matrix inversion we simply use identity matrix B, we may 
            always suppose qB=31 
qXYR        combination of fixed point representation (matrices R, x and y) 
            qXYR=qX-qY+qR (for the fixed point API only)
Output:
x[N*M]      Left Inverse of the matrix A, for fixed point API, the 
            representation is Q(qX)
Temporary:
pScr            Scratch memory

N = M = 4, 6, 8, 10

Restrictions:
All matrices and scratch memory should not overlap and be aligned on
8-bytes boundary
---------------------------------------------------------------------------*/
/* floating point API */
void  cmatcholpseudoinv4x4f(void* pScr,
        complex_float *x,
        const complex_float * A,
        const float32_t  sigma2);
void  cmatcholpseudoinv6x6f(void* pScr,
        complex_float *x,
        const complex_float * A,
        const float32_t  sigma2);
void  cmatcholpseudoinv8x8f(void* pScr,
        complex_float *x,
        const complex_float * A,
        const float32_t  sigma2);
void  cmatcholpseudoinv10x10f(void* pScr,
        complex_float *x,
        const complex_float * A,
        const float32_t  sigma2);

void  matcholpseudoinv4x4f(void* pScr,
    float32_t *x,
    const float32_t * A,
    const float32_t  sigma2);
void  matcholpseudoinv6x6f(void* pScr,
    float32_t *x,
    const float32_t * A,
    const float32_t  sigma2);
void  matcholpseudoinv8x8f(void* pScr,
    float32_t *x,
    const float32_t * A,
    const float32_t  sigma2);
void  matcholpseudoinv10x10f(void* pScr,
    float32_t *x,
    const float32_t * A,
    const float32_t  sigma2);

/* scratch allocation functions */
size_t  cmatcholpseudoinv4x4f_getScratchSize();
size_t  cmatcholpseudoinv6x6f_getScratchSize();
size_t  cmatcholpseudoinv8x8f_getScratchSize();
size_t  cmatcholpseudoinv10x10f_getScratchSize();

size_t   matcholpseudoinv4x4f_getScratchSize();
size_t   matcholpseudoinv6x6f_getScratchSize();
size_t   matcholpseudoinv8x8f_getScratchSize();
size_t   matcholpseudoinv10x10f_getScratchSize();

/* 32-bit fixed point API */
void  cmatcholpseudoinv4x4_32x32(void* pScr,
        complex_fract32 *x,
        const complex_fract32 * A,
        const int32_t  sigma2, 
        int qRA, int qYBRA, int qXYR);
void  cmatcholpseudoinv6x6_32x32(void* pScr,
        complex_fract32 *x,
        const complex_fract32 * A,
        const int32_t  sigma2, 
        int qRA, int qYBRA, int qXYR);
void  cmatcholpseudoinv8x8_32x32(void* pScr,
        complex_fract32 *x,
        const complex_fract32 * A,
        const int32_t  sigma2, 
        int qRA, int qYBRA, int qXYR);
void  cmatcholpseudoinv10x10_32x32(void* pScr,
        complex_fract32 *x,
        const complex_fract32 * A,
        const int32_t  sigma2, 
        int qRA, int qYBRA, int qXYR);

void  matcholpseudoinv4x4_32x32(void* pScr,
          int32_t *x,
    const int32_t * A,
    const int32_t  sigma2,
    int qRA, int qYBRA, int qXYR);
void  matcholpseudoinv6x6_32x32(void* pScr,
          int32_t *x,
    const int32_t * A,
    const int32_t  sigma2,
    int qRA, int qYBRA, int qXYR);
void  matcholpseudoinv8x8_32x32(void* pScr,
          int32_t *x,
    const int32_t * A,
    const int32_t  sigma2,
    int qRA, int qYBRA, int qXYR);
void  matcholpseudoinv10x10_32x32(void* pScr,
          int32_t *x,
    const int32_t * A,
    const int32_t  sigma2,
    int qRA, int qYBRA, int qXYR);

/* scratch allocation functions */
size_t  cmatcholpseudoinv4x4_32x32_getScratchSize();
size_t  cmatcholpseudoinv6x6_32x32_getScratchSize();
size_t  cmatcholpseudoinv8x8_32x32_getScratchSize();
size_t  cmatcholpseudoinv10x10_32x32_getScratchSize();

size_t   matcholpseudoinv4x4_32x32_getScratchSize();
size_t   matcholpseudoinv6x6_32x32_getScratchSize();
size_t   matcholpseudoinv8x8_32x32_getScratchSize();
size_t   matcholpseudoinv10x10_32x32_getScratchSize();

/*-------------------------------------------------------------------------
Preprocessing for Least Square Solutions
The result is matrix Z[NxN], such that
Z = A'*A + sigma2*I[NxN], where ' denotes the conjugate transpose of
a matrix, and sigma2*I[NxN] is the NxN identity matrix multiplied with
the regularization term.

Precision:
f     single precision floating point
32x32 32-bit inputs/outputs

Input:
A[M*N]          matrix A. For the fixed point, the representation is Q(qA)
sigma2          regularization term. For fixed point, the representation 
                should be Q(2*qA-30)
qRA             qR-qA; difference between fixed point representations of 
                decomposition matrix R and original matrix A (for the fixed 
                point API only). Should be equal or less than 0 (typically 
                -2).
Output:
Z[N*N]          matrix Z. For the fixed point, the representation is Q(2*qR-4)
Temporary:
pScr            Scratch memory

N = M = 4, 6, 8, 10

Restrictions:
All matrices and scratch memory should not overlap and be aligned on
8-bytes boundary
---------------------------------------------------------------------------*/
/* floating point API */
void  cmatcholpreprocess4x4f(void* pScr,
            complex_float *Z,
    const   complex_float * A,
    const   float32_t  sigma2);
void  cmatcholpreprocess6x6f(void* pScr,
            complex_float *Z,
    const   complex_float * A,
    const   float32_t  sigma2);
void  cmatcholpreprocess8x8f(void* pScr,
            complex_float *Z,
    const   complex_float * A,
    const   float32_t  sigma2);
void  cmatcholpreprocess10x10f(void* pScr,
            complex_float *Z,
    const   complex_float * A,
    const   float32_t  sigma2);

void  matcholpreprocess4x4f(void* pScr,
            float32_t *Z,
    const   float32_t * A,
    const   float32_t  sigma2);
void  matcholpreprocess6x6f(void* pScr,
            float32_t *Z,
    const   float32_t * A,
    const   float32_t  sigma2);
void  matcholpreprocess8x8f(void* pScr,
            float32_t *Z,
    const   float32_t * A,
    const   float32_t  sigma2);
void  matcholpreprocess10x10f(void* pScr,
            float32_t *Z,
    const   float32_t * A,
    const   float32_t  sigma2);

/* scratch allocation functions */
size_t  cmatcholpreprocess4x4f_getScratchSize();
size_t  cmatcholpreprocess6x6f_getScratchSize();
size_t  cmatcholpreprocess8x8f_getScratchSize();
size_t  cmatcholpreprocess10x10f_getScratchSize();

size_t   matcholpreprocess4x4f_getScratchSize();
size_t   matcholpreprocess6x6f_getScratchSize();
size_t   matcholpreprocess8x8f_getScratchSize();
size_t   matcholpreprocess10x10f_getScratchSize();

/* fixed point point API */
void  cmatcholpreprocess4x4_32x32(void* pScr,
            complex_fract64 *Z,
    const   complex_fract32 * A,
            int32_t  sigma2, int qRA);
void  cmatcholpreprocess6x6_32x32(void* pScr,
            complex_fract64 *Z,
    const   complex_fract32 * A,
            int32_t  sigma2, int qRA);
void  cmatcholpreprocess8x8_32x32(void* pScr,
            complex_fract64 *Z,
    const   complex_fract32 * A,
            int32_t  sigma2, int qRA);
void  cmatcholpreprocess10x10_32x32(void* pScr,
            complex_fract64 *Z,
    const   complex_fract32 * A,
            int32_t  sigma2, int qRA);

void  matcholpreprocess4x4_32x32(void* pScr,
            int64_t *Z,
    const   int32_t * A,
            int32_t  sigma2, int qRA);
void  matcholpreprocess6x6_32x32(void* pScr,
            int64_t *Z,
    const   int32_t * A,
            int32_t  sigma2, int qRA);
void  matcholpreprocess8x8_32x32(void* pScr,
            int64_t *Z,
    const   int32_t * A,
            int32_t  sigma2, int qRA);
void  matcholpreprocess10x10_32x32(void* pScr,
            int64_t *Z,
    const   int32_t * A,
            int32_t  sigma2, int qRA);

/* scratch allocation functions */
size_t  cmatcholpreprocess4x4_32x32_getScratchSize();
size_t  cmatcholpreprocess6x6_32x32_getScratchSize();
size_t  cmatcholpreprocess8x8_32x32_getScratchSize();
size_t  cmatcholpreprocess10x10_32x32_getScratchSize();

size_t   matcholpreprocess4x4_32x32_getScratchSize();
size_t   matcholpreprocess6x6_32x32_getScratchSize();
size_t   matcholpreprocess8x8_32x32_getScratchSize();
size_t   matcholpreprocess10x10_32x32_getScratchSize();

#ifdef __cplusplus
}
#endif

#endif/* __NATUREDSP_SIGNAL_MATINV_H__ */
