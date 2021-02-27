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
#ifndef FIR_DECIMAF_DX
#define FIR_DECIMAF_DX
/*
    Real FIR Filter with decimation (Dx)
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"

/*
    universal decimator:
    Input/output:
    delay[M] - circular delay line
    Input:
    p        - pointer to delay line
    x[N*D]   - input signal
    h[M]     - impulse response
    D          decimation factor
    N        - number of output samples
    Output:
    z[N]     - output samples
    Restrictions:
    N>0, M>0
    D>4
    M multiple of 2
    N multiple of 8
    delay should be aligned on 8 byte boundary

    Returns:
    updated pointer to delay line
*/
float32_t * fir_decimaf_Dx(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x,  int M, int D, int N);

#endif
