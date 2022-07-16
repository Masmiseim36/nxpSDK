/* Copyright (c) 2019 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ('Cadence    */
/* Libraries') are the copyrighted works of Cadence Design Systems Inc.     */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/* IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                     */
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
/*          Copyright (C) 2014-2019 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#ifndef CHOLNF_COMMON_H__
#define CHOLNF_COMMON_H__

#include "NatureDSP_types.h"

typedef void(*f_cplxcholFwdrecf)(complex_float * y, const complex_float * R, const complex_float * D, const complex_float * Z, int stride);
typedef void(*f_realcholFwdrecf)(float32_t * y, const float32_t * R, const float32_t * D, const float32_t * Z, int stride);

/*--------------------------------------------------------
Forward recursion P=1
Input:
R[((N+1)*N)/2]  upper triangular matrix R
stride          width of matrix A'*B
Z[N*1]          column in matrix A'*B
D[N]            reciprocals of main diagonal
Output:
y[N*1]		Decision matrix y
N = 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
--------------------------------------------------------*/
void cplxcholFwdrec1f(complex_float * y,
	const complex_float * R,
	const complex_float * D,
	const complex_float * Z, 
    int stride);
void cplxcholFwdrec2f(complex_float * y,
	const complex_float * R,
	const complex_float * D,
	const complex_float * Z, 
    int stride);
void cplxcholFwdrec3f(complex_float * y,
	const complex_float * R,
	const complex_float * D,
	const complex_float * Z, 
    int stride);
void cplxcholFwdrec4f(complex_float * y,
	const complex_float * R,
	const complex_float * D,
	const complex_float * Z, 
    int stride);
void cplxcholFwdrec5f(complex_float * y,
	const complex_float * R,
	const complex_float * D,
	const complex_float * Z, 
    int stride);
void cplxcholFwdrec6f(complex_float * y,
	const complex_float * R,
	const complex_float * D,
	const complex_float * Z, 
    int stride);
void cplxcholFwdrec7f(complex_float * y,
	const complex_float * R,
	const complex_float * D,
	const complex_float * Z, 
    int stride);
void cplxcholFwdrec8f(complex_float * y,
	const complex_float * R,
	const complex_float * D,
	const complex_float * Z, 
    int stride);
void cplxcholFwdrec9f(complex_float * y,
	const complex_float * R,
	const complex_float * D,
	const complex_float * Z, 
    int stride);
void cplxcholFwdrec10f(complex_float * y,
	const complex_float * R,
	const complex_float * D,
	const complex_float * Z, 
    int stride);
void realcholFwdrec1f(float32_t * y,
	const float32_t * R,
	const float32_t * D,
	const float32_t * Z,
    int stride);
void realcholFwdrec2f(float32_t * y,
	const float32_t * R,
	const float32_t * D,
	const float32_t * Z,
    int stride);
void realcholFwdrec3f(float32_t * y,
	const float32_t * R,
	const float32_t * D,
	const float32_t * Z,
    int stride);
void realcholFwdrec4f(float32_t * y,
	const float32_t * R,
	const float32_t * D,
	const float32_t * Z,
    int stride);
void realcholFwdrec5f(float32_t * y,
	const float32_t * R,
	const float32_t * D,
	const float32_t * Z,
    int stride);
void realcholFwdrec6f(float32_t * y,
	const float32_t * R,
	const float32_t * D,
	const float32_t * Z,
    int stride);
void realcholFwdrec7f(float32_t * y,
	const float32_t * R,
	const float32_t * D,
	const float32_t * Z,
    int stride);
void realcholFwdrec8f(float32_t * y,
	const float32_t * R,
	const float32_t * D,
	const float32_t * Z,
    int stride);
void realcholFwdrec9f(float32_t * y,
	const float32_t * R,
	const float32_t * D,
	const float32_t * Z,
    int stride);
void realcholFwdrec10f(float32_t * y,
	const float32_t * R,
	const float32_t * D,
	const float32_t * Z,
    int stride);

/*--------------------------------------------------------
update N-th diagonal element
Input:
stride      width of matrix Z
Z[SZ]       N-th column in convolutions matrix Z
Input/output:
y           pointer to the begining of column in matrix R[L][SR] (N+1 elements is written)
Output:
D[SD]       reciprocals of main diagonal (pointer to the N-th element
--------------------------------------------------------*/
void cplxcholDiagUpdf(complex_float* y,
    complex_float* D,
    const complex_float* Z,
    int N);
void realcholDiagUpdf(float32_t* y,
    float32_t* D,
    const float32_t* Z,
    int N);

#endif
