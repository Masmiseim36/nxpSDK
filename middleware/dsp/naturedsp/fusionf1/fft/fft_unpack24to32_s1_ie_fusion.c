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
*/
#include "NatureDSP_Signal.h"
#include "common.h"


/*===========================================================================
Unpack to 32 bits  and calculate block exponent
Called from :

fft_real_24x24_ie_24p
ifft_real_24x24_ie_24p

===========================================================================*/

 
int fft_unpack24to32_s1_ie(uint8_t *x, uint8_t *y, int N)
{  
    ae_int24x2 d;
    ae_valign v = AE_LA64_PP(x);
    void *pin = x;
    ae_f24x2 *pout = (ae_f24x2 *)y;
    int i;
    ae_int32x2  maxV = 0, minV = 0;
    ae_int32x2 tmp32;
    int exp;

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT((N & 1) == 0);
    // 3 cycles per pipeline stage in steady state with unroll=1
    for (i = 0; i<(N>>1); i++)
    {
        AE_LA24X2_IP(d, v, pin);
        tmp32 = d;
        maxV = AE_MAX32(maxV, tmp32);
        minV = AE_MIN32(minV, tmp32);

        AE_S32X2F24_IP(d, pout, sizeof(*pout));
    }
    maxV = AE_MAX32(maxV, AE_NEG32S(minV));
    maxV = AE_MAX32(maxV, AE_SEL32_LH(maxV, maxV));
    exp = AE_NSAZ32_L(maxV);
    return exp;
}
