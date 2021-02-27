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
    twiddle factors for 24x16 real fwd/rev FFT transforms
*/
#ifndef FFT_REAL_TWIDDLES_H___
#define FFT_REAL_TWIDDLES_H___
#include "common.h"

/*
    GENERAL NOTE:
    MAX_RFFT_PWR controls the maximum avaliable size of real FFT transforms
    It may be reconfigured to save total amount of memory needed for storing the tables

    MAX_RFFT_PWR    Maximum real FFT size
    13              8192
    12              4096
    11              2048
    10              1024
     9               512
     8               256
     7               128
*/

#define MAX_RFFT_PWR 13
#define MAX_RFFT_LEN (1<<MAX_RFFT_PWR)

extern const int16_t twiddleSplit[MAX_RFFT_LEN/2];
extern const int32_t twiddleSplit32[MAX_RFFT_LEN / 2];

#endif
