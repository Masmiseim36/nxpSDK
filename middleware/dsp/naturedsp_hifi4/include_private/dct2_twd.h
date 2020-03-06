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
/*          Copyright (C) 2015-2017 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
    NatureDSP Signal Processing Library. FFT part
    DCT-II twiddles
    Integrit, 2006-2018
*/
#ifndef DCT2_TWD__
#define DCT2_TWD__
#include "NatureDSP_Signal_fft.h"

#define MAGIC_DCT2_16    0x53644342
#define MAGIC_DCT2_32    0x53446432
#define MAGIC_DCT2_F     0x54323644

/* DCT-II twiddles and CFFT processing fuction */
typedef struct
{
    uint32_t magic;                 /* magic word                             */
    int      N;                     /* DCT-II size                            */
    void     *rfft_split_twd;       /* exp(i*pi/2*(0:N-1)/N)                  */
    void     *fft_twd;              /* exp(-2j*pi*[1;2;3]*(0:N/4-1)/N)        */
    void     *dct_twd;              /* exp(-i*pi/2*(0:N/2)/N)                 */
}
tdct2_twd;

#endif
