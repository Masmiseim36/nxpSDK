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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_Signal_fft.h"
#include "common.h"
#include "fft_x16_common.h"


/********** Twiddles table N=72 stage 1 radix 3 ******************/
ALIGN(8)  static const int16_t _fft72_tw1_[] =
{
    (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7F83, (int16_t)0xF4D8, (int16_t)0x7E0E, (int16_t)0xE9C6,
    (int16_t)0x7E0E, (int16_t)0xE9C6, (int16_t)0x7848, (int16_t)0xD439, (int16_t)0x7BA3, (int16_t)0xDEDF, (int16_t)0x6EDA, (int16_t)0xC000,
    (int16_t)0x7848, (int16_t)0xD439, (int16_t)0x620E, (int16_t)0xADB9, (int16_t)0x7402, (int16_t)0xC9E8, (int16_t)0x5247, (int16_t)0x9DF2,
    (int16_t)0x6EDA, (int16_t)0xC000, (int16_t)0x4000, (int16_t)0x9126, (int16_t)0x68DA, (int16_t)0xB695, (int16_t)0x2BC7, (int16_t)0x87B8,
    (int16_t)0x620E, (int16_t)0xADB9, (int16_t)0x163A, (int16_t)0x81F2, (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x0000, (int16_t)0x8000,
    (int16_t)0x5247, (int16_t)0x9DF2, (int16_t)0xE9C6, (int16_t)0x81F2, (int16_t)0x496B, (int16_t)0x9726, (int16_t)0xD439, (int16_t)0x87B8,
    (int16_t)0x4000, (int16_t)0x9126, (int16_t)0xC000, (int16_t)0x9126, (int16_t)0x3618, (int16_t)0x8BFE, (int16_t)0xADB9, (int16_t)0x9DF2,
    (int16_t)0x2BC7, (int16_t)0x87B8, (int16_t)0x9DF2, (int16_t)0xADB9, (int16_t)0x2121, (int16_t)0x845D, (int16_t)0x9126, (int16_t)0xC000,
    (int16_t)0x163A, (int16_t)0x81F2, (int16_t)0x87B8, (int16_t)0xD439, (int16_t)0x0B28, (int16_t)0x807D, (int16_t)0x81F2, (int16_t)0xE9C6,
    (int16_t)0x0000, (int16_t)0x8000, (int16_t)0x8000, (int16_t)0x0000, (int16_t)0xF4D8, (int16_t)0x807D, (int16_t)0x81F2, (int16_t)0x163A,
    (int16_t)0xE9C6, (int16_t)0x81F2, (int16_t)0x87B8, (int16_t)0x2BC7, (int16_t)0xDEDF, (int16_t)0x845D, (int16_t)0x9126, (int16_t)0x4000,
    (int16_t)0xD439, (int16_t)0x87B8, (int16_t)0x9DF2, (int16_t)0x5247, (int16_t)0xC9E8, (int16_t)0x8BFE, (int16_t)0xADB9, (int16_t)0x620E,
};

/********** Twiddles table N=72 stage 2 radix 2 ******************/
ALIGN(8)  static const int16_t _fft72_tw2_[] =
{
    (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7BA3, (int16_t)0xDEDF, (int16_t)0x6EDA, (int16_t)0xC000, (int16_t)0x5A82, (int16_t)0xA57E,
    (int16_t)0x4000, (int16_t)0x9126, (int16_t)0x2121, (int16_t)0x845D, (int16_t)0x0000, (int16_t)0x8000, (int16_t)0xDEDF, (int16_t)0x845D,
    (int16_t)0xC000, (int16_t)0x9126, (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0x9126, (int16_t)0xC000, (int16_t)0x845D, (int16_t)0xDEDF,
};

/********** Twiddles table N=72 stage 3 radix 3 ******************/
ALIGN(8)  static const int16_t _fft72_tw3_[] =
{
    (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x6EDA, (int16_t)0xC000, (int16_t)0x4000, (int16_t)0x9126,
    (int16_t)0x4000, (int16_t)0x9126, (int16_t)0xC000, (int16_t)0x9126, (int16_t)0x0000, (int16_t)0x8000, (int16_t)0x8000, (int16_t)0x0000,
};

/* Twiddles tables for fft_real32x16, ifft_real32x16, fft_real16x16, ifft_real16x16, N=144 */
ALIGN(8) static const int16_t __fft_real16_tw144[72] =
{
    (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0595, (int16_t)0x7FE1, (int16_t)0x0B28, (int16_t)0x7F83, (int16_t)0x10B5, (int16_t)0x7EE8,
    (int16_t)0x163A, (int16_t)0x7E0E, (int16_t)0x1BB4, (int16_t)0x7CF7, (int16_t)0x2121, (int16_t)0x7BA3, (int16_t)0x267E, (int16_t)0x7A13,
    (int16_t)0x2BC7, (int16_t)0x7848, (int16_t)0x30FC, (int16_t)0x7642, (int16_t)0x3618, (int16_t)0x7402, (int16_t)0x3B1B, (int16_t)0x718A,
    (int16_t)0x4000, (int16_t)0x6EDA, (int16_t)0x44C6, (int16_t)0x6BF4, (int16_t)0x496B, (int16_t)0x68DA, (int16_t)0x4DEC, (int16_t)0x658D,
    (int16_t)0x5247, (int16_t)0x620E, (int16_t)0x567A, (int16_t)0x5E5F, (int16_t)0x5A82, (int16_t)0x5A82, (int16_t)0x5E5F, (int16_t)0x567A,
    (int16_t)0x620E, (int16_t)0x5247, (int16_t)0x658D, (int16_t)0x4DEC, (int16_t)0x68DA, (int16_t)0x496B, (int16_t)0x6BF4, (int16_t)0x44C6,
    (int16_t)0x6EDA, (int16_t)0x4000, (int16_t)0x718A, (int16_t)0x3B1B, (int16_t)0x7402, (int16_t)0x3618, (int16_t)0x7642, (int16_t)0x30FC,
    (int16_t)0x7848, (int16_t)0x2BC7, (int16_t)0x7A13, (int16_t)0x267E, (int16_t)0x7BA3, (int16_t)0x2121, (int16_t)0x7CF7, (int16_t)0x1BB4,
    (int16_t)0x7E0E, (int16_t)0x163A, (int16_t)0x7EE8, (int16_t)0x10B5, (int16_t)0x7F83, (int16_t)0x0B28, (int16_t)0x7FE1, (int16_t)0x0595,
};

static const int tw_step_tab[] =
{
    1, 1, 1, 0
};
static const cint16ptr_t tw_tab[] =
{
    _fft72_tw1_, _fft72_tw2_, _fft72_tw3_, NULL
};


static const eFft_stage_type fft_cplx16x16_stg_tab[] =
{
    fft_stage_first_DFT3,
    fft_stage_inner_DFT2,
    fft_stage_inner_DFT3,
    fft_stage_last_DFT4
};
static const eFft_stage_type ifft_cplx16x16_stg_tab[] =
{
    fft_stage_first_inv_DFT3,
    fft_stage_inner_DFT2,
    fft_stage_inner_DFT3,
    fft_stage_last_DFT4
};

static const fft_cplx_x16_descr_t __cfft_x16_descr72 =
{
    72,
    NULL,
    fft_cplx16x16_stg_tab,
    NULL,
    fft_cplx16x16_stg_tab,
    tw_step_tab,
    tw_tab
};
static const fft_cplx_x16_descr_t __cifft_x16_descr72 =
{
    72,
    NULL,
    ifft_cplx16x16_stg_tab,
    NULL,
    ifft_cplx16x16_stg_tab,
    tw_step_tab,
    tw_tab
};


static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_x16_descr72,
    __fft_real16_tw144
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_x16_descr72,
    __fft_real16_tw144
};
const fft_handle_t rnfft16_144 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft16_144 = (const fft_handle_t)&__rifft_descr;
