/* ------------------------------------------------------------------------ */
/* Copyright (c) 2020 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2020 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#ifndef DIVIDE_Q31_H__
#define DIVIDE_Q31_H__
#include "NatureDSP_types.h"
#include "common.h"

/* -----------------------------------------------------
accurate division of two numbers giving the result in Q31 
Input:
x,y - in Q31
Output:
returned value is division result in Q31
-----------------------------------------------------*/
int32_t divide_q31(int32_t x,int32_t y);

/* -----------------------------------------------------
reciprocal of y (y[n]=x/y[n] in Q31)
Input:
x    - input in Q31
y[N] - input in Q31
Output:
y[N] - reciprocal in Q31
Restrictions:
y -aligned
N - multiple of 4
-----------------------------------------------------*/
void recip_qx(int32_t x,int32_t* y,int N);

#endif
