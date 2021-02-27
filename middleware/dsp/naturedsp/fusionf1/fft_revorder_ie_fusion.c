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
/*
	NatureDSP Signal Processing Library. FFT part
    C code optimized for Fusion
*/
/*===========================================================================

    Reverse order of complex 32bits samples.
    N - number of input samples.
    N is power of 2.

    Input array is 
    0,1,....N-1
    Output:
    0,N-1 ... 1

    Used in 
    ifft_cplx_24x24_ie, ifft_cplx_32x16_ie.

===========================================================================*/
#include "NatureDSP_Signal.h"
#include "common.h"


void fft_revorder_ie(int32_t *x, int N) 
{
    int i;     
    ae_int32x2 * restrict p0 = (ae_int32x2*)(x + N - 2 );
    ae_int32x2 * restrict p1 = (ae_int32x2*)(x + N + 2 );
    // Reverse order of input samples
    for(i = 0; i < N/2-1; i++)
    { 
        ae_int32x2 x0,  x1; 
        x0 = AE_L32X2_I(p0, 0); 
        x1 = AE_L32X2_I(p1, 0); 
        AE_S32X2_XP(x1, p0, -8); 
        AE_S32X2_IP(x0, p1,  8); 
    }
}
