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
#include <math.h>

/*-------------------------------------------------------------------------
  Rounding 
  Routines make floating point round to integral value. Input data are 
  rounded up/down to the nearest integral value but maintained in the 
  same floating point format.

  Precision: 
  f     single precision floating point

  Input:
  x[N]  input data, floating point
  N     length of vector
  Output:
  y[N]  rounded floating point values

  Restriction:
  x,y should not overlap
-------------------------------------------------------------------------*/
void   vec_float2ceil    (float32_t * restrict y, const float32_t * restrict x, int N)
{
    int n;
    NASSERT(x);
    NASSERT(y);
    for (n=0; n<N; n++) y[n]=ceilf(x[n]);
}
