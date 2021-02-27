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
#ifndef FIR_INTERPF_4X
#define FIR_INTERPF_4X
/*
    Real FIR Filter with interpolation (4x)
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"

/*
    4x interpolator:
    Input/output:
    delay[M*4] - circular delay line
    Input:
    p        - pointer to delay line
    x[N]     - input signal
    h[M*4]   - impulse response
    N        - number of output samples
    Output:
    z[N*4]     - output samples
    Restrictions:
    N>0, M>0
    N   - multiple of 8
    M   - multiple of 4
    delay should be aligned on 8 byte boundary

    Returns:
    updated pointer to delay line
*/

float32_t * fir_interpf_4x(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x, int M, int N);

#endif
