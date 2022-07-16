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

    fft_unpack24to32_ie:
    Unpack 24 bits packed data to 32 bits unpacked.

    fft_pack32to24_ie:
    Pack 32-bits data into 24-bits packed. 

    N - number of input/output words.

    Called from: 
    fft_real_32x16_ie_24p
    fft_real_24x24_ie_24p
    ifft_real_32x16_ie_24p
    ifft_real_24x24_ie_24p

===========================================================================*/
#include "NatureDSP_Signal.h"
#include "common.h"

void fft_pack32to24_ie(uint8_t *x,  uint8_t *y, int N)
{
    ae_f24x2 d; 
    ae_valign v = AE_ZALIGN64();
    ae_f24x2 *restrict pin  = (ae_f24x2 *)x; 
    void *restrict pout = (ae_f24x2 *)y; 
    int i; 

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT((N&1)==0);

    for(i=0; i<N/2; i++)
    {
        ae_int32x2 t;
        AE_L32X2_IP(t, castxcc(ae_int32x2, pin), sizeof(*pin) );
        /* Do rounding before packing */
        t = AE_ADD32(t, 0x80);
        t = AE_SRAI32(t, 8);
        d = AE_MOVF24X2_FROMINT32X2(t);
        AE_SA24X2_IP(d, v, pout); 
    }
    AE_SA64POS_FP(v, pout); 
}

void fft_unpack24to32_ie(uint8_t *x,  uint8_t *y, int N)
{
    ae_int24x2 d; 
    ae_valign v = AE_LA64_PP(x);
    void *pin  = x; 
    ae_f24x2 *pout = (ae_f24x2 *)y; 
    int i; 

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT((N&1)==0);

    for(i=0; i<N/2; i++)
    {
        AE_LA24X2_IP(d, v, pin);
        AE_S32X2F24_IP(d, pout, sizeof(*pout)); 
    }
}
