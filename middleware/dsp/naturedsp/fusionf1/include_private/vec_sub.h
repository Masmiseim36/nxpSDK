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
#ifndef VEC_SUB__
#define VEC_SUB__
/*===========================================================================
  Matrix part.
  Operations with Small Matrices, helper file
  Code optimized for Fusion
  IntegrIT, 2006-2015
===========================================================================*/

/* Portable data types. */
#include "NatureDSP_types.h"

/* vector subtraction */
void vec_sub16x16 (     int16_t * restrict z,
                  const int16_t * restrict x,
                  const int16_t * restrict y,
                  int N
                  );
void vec_sub32x32(int32_t * restrict z,
            const int32_t * restrict x,
            const int32_t * restrict y,
            int N);
void vec_subf    (float32_t * restrict z,
            const float32_t * restrict x,
            const float32_t * restrict y,
            int N);

#endif
