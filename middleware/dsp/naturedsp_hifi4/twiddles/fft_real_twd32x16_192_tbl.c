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

/* Twiddles tables for fft_cplx32x16, ifft_cplx32x16, fft_cplx16x16, ifft_cplx16x16, N=96 */
#define N 96

/****************** stage 1 radix 3 ******************/
ALIGN(8) static const int16_t __fft16_tw1[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7fba,(int16_t)0xf7a1,(int16_t)0x7ee8,(int16_t)0xef4b,
(int16_t)0x7ee8,(int16_t)0xef4b,(int16_t)0x7ba3,(int16_t)0xdedf,
(int16_t)0x7d8a,(int16_t)0xe707,(int16_t)0x7642,(int16_t)0xcf04,
(int16_t)0x7ba3,(int16_t)0xdedf,(int16_t)0x6eda,(int16_t)0xc000,
(int16_t)0x7935,(int16_t)0xd6db,(int16_t)0x658d,(int16_t)0xb214,
(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x5a82,(int16_t)0xa57e,
(int16_t)0x72cd,(int16_t)0xc763,(int16_t)0x4dec,(int16_t)0x9a73,
(int16_t)0x6eda,(int16_t)0xc000,(int16_t)0x4000,(int16_t)0x9126,
(int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x30fc,(int16_t)0x89be,
(int16_t)0x658d,(int16_t)0xb214,(int16_t)0x2121,(int16_t)0x845d,
(int16_t)0x603c,(int16_t)0xab9b,(int16_t)0x10b5,(int16_t)0x8118,
(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x0000,(int16_t)0x8000,
(int16_t)0x5465,(int16_t)0x9fc4,(int16_t)0xef4b,(int16_t)0x8118,
(int16_t)0x4dec,(int16_t)0x9a73,(int16_t)0xdedf,(int16_t)0x845d,
(int16_t)0x471d,(int16_t)0x9592,(int16_t)0xcf04,(int16_t)0x89be,
(int16_t)0x4000,(int16_t)0x9126,(int16_t)0xc000,(int16_t)0x9126,
(int16_t)0x389d,(int16_t)0x8d33,(int16_t)0xb214,(int16_t)0x9a73,
(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0xa57e,
(int16_t)0x2925,(int16_t)0x86cb,(int16_t)0x9a73,(int16_t)0xb214,
(int16_t)0x2121,(int16_t)0x845d,(int16_t)0x9126,(int16_t)0xc000,
(int16_t)0x18f9,(int16_t)0x8276,(int16_t)0x89be,(int16_t)0xcf04,
(int16_t)0x10b5,(int16_t)0x8118,(int16_t)0x845d,(int16_t)0xdedf,
(int16_t)0x085f,(int16_t)0x8046,(int16_t)0x8118,(int16_t)0xef4b,
(int16_t)0x0000,(int16_t)0x8000,(int16_t)0x8000,(int16_t)0x0000,
(int16_t)0xf7a1,(int16_t)0x8046,(int16_t)0x8118,(int16_t)0x10b5,
(int16_t)0xef4b,(int16_t)0x8118,(int16_t)0x845d,(int16_t)0x2121,
(int16_t)0xe707,(int16_t)0x8276,(int16_t)0x89be,(int16_t)0x30fc,
(int16_t)0xdedf,(int16_t)0x845d,(int16_t)0x9126,(int16_t)0x4000,
(int16_t)0xd6db,(int16_t)0x86cb,(int16_t)0x9a73,(int16_t)0x4dec,
(int16_t)0xcf04,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0x5a82,
(int16_t)0xc763,(int16_t)0x8d33,(int16_t)0xb214,(int16_t)0x658d
};
/****************** stage 2 radix 4 ******************/
ALIGN(8) static const int16_t __fft16_tw2[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7d8a,(int16_t)0xe707,(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x6a6e,(int16_t)0xb8e3,
(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x30fc,(int16_t)0x89be,
(int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xe707,(int16_t)0x8276,
(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xa57e,
(int16_t)0x471d,(int16_t)0x9592,(int16_t)0xcf04,(int16_t)0x89be,(int16_t)0x8276,(int16_t)0xe707,
(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0xa57e,(int16_t)0x89be,(int16_t)0x30fc,
(int16_t)0x18f9,(int16_t)0x8276,(int16_t)0x89be,(int16_t)0xcf04,(int16_t)0xb8e3,(int16_t)0x6a6e
};
static const int tw_step_tab[] =
{
    1, 1, 4
}; 
static const cint16ptr_t tw_tab[] = 
{
    __fft16_tw1, __fft16_tw2, NULL
};

static const eFft_stage_type fft_cplx32x16_scl2_tab[] = 
{
    fft_stage_first_DFT3,
    fft_stage_inner_DFT4,
    fft_stage_last_DFT8
};
static const eFft_stage_type fft_cplx32x16_scl3_tab[] =
{
    fft_stage_first_DFT3,
    fft_stage_inner_DFT4,
    fft_stage_last_DFT8
};
static const eFft_stage_type ifft_cplx32x16_scl2_tab[] = 
{
    fft_stage_first_inv_DFT3,
    fft_stage_inner_DFT4,
    fft_stage_last_DFT8
};
static const eFft_stage_type ifft_cplx32x16_scl3_tab[] =
{
    fft_stage_first_inv_DFT3,
    fft_stage_inner_DFT4,
    fft_stage_last_DFT8
};



static const fft_cplx_x16_descr_t __cfft_descr = 
{
    N, 
    fft_cplx32x16_scl2_tab,
    NULL,
    fft_cplx32x16_scl3_tab, 
    NULL, 
    tw_step_tab,
    tw_tab
};     
static const fft_cplx_x16_descr_t __cifft_descr =
{
    N, 
    ifft_cplx32x16_scl2_tab, 
    NULL, 
    ifft_cplx32x16_scl3_tab, 
    NULL, 
    tw_step_tab,
    tw_tab
};     

/* Twiddles tables for fft_real32x16, ifft_real32x16, fft_real16x16, ifft_real16x16, N=192 */
ALIGN(8) static const int16_t __fft_real16_tw[] =
{
(int16_t)0x0000,(int16_t)0x7fff,
(int16_t)0x0430,(int16_t)0x7fee,
(int16_t)0x085f,(int16_t)0x7fba,
(int16_t)0x0c8c,(int16_t)0x7f62,
(int16_t)0x10b5,(int16_t)0x7ee8,
(int16_t)0x14da,(int16_t)0x7e4a,
(int16_t)0x18f9,(int16_t)0x7d8a,
(int16_t)0x1d11,(int16_t)0x7ca8,
(int16_t)0x2121,(int16_t)0x7ba3,
(int16_t)0x2528,(int16_t)0x7a7d,
(int16_t)0x2925,(int16_t)0x7935,
(int16_t)0x2d17,(int16_t)0x77cc,
(int16_t)0x30fc,(int16_t)0x7642,
(int16_t)0x34d4,(int16_t)0x7497,
(int16_t)0x389d,(int16_t)0x72cd,
(int16_t)0x3c57,(int16_t)0x70e3,
(int16_t)0x4000,(int16_t)0x6eda,
(int16_t)0x4398,(int16_t)0x6cb3,
(int16_t)0x471d,(int16_t)0x6a6e,
(int16_t)0x4a8f,(int16_t)0x680b,
(int16_t)0x4dec,(int16_t)0x658d,
(int16_t)0x5134,(int16_t)0x62f2,
(int16_t)0x5465,(int16_t)0x603c,
(int16_t)0x5780,(int16_t)0x5d6c,
(int16_t)0x5a82,(int16_t)0x5a82,
(int16_t)0x5d6c,(int16_t)0x5780,
(int16_t)0x603c,(int16_t)0x5465,
(int16_t)0x62f2,(int16_t)0x5134,
(int16_t)0x658d,(int16_t)0x4dec,
(int16_t)0x680b,(int16_t)0x4a8f,
(int16_t)0x6a6e,(int16_t)0x471d,
(int16_t)0x6cb3,(int16_t)0x4398,
(int16_t)0x6eda,(int16_t)0x4000,
(int16_t)0x70e3,(int16_t)0x3c57,
(int16_t)0x72cd,(int16_t)0x389d,
(int16_t)0x7497,(int16_t)0x34d4,
(int16_t)0x7642,(int16_t)0x30fc,
(int16_t)0x77cc,(int16_t)0x2d17,
(int16_t)0x7935,(int16_t)0x2925,
(int16_t)0x7a7d,(int16_t)0x2528,
(int16_t)0x7ba3,(int16_t)0x2121,
(int16_t)0x7ca8,(int16_t)0x1d11,
(int16_t)0x7d8a,(int16_t)0x18f9,
(int16_t)0x7e4a,(int16_t)0x14da,
(int16_t)0x7ee8,(int16_t)0x10b5,
(int16_t)0x7f62,(int16_t)0x0c8c,
(int16_t)0x7fba,(int16_t)0x085f,
(int16_t)0x7fee,(int16_t)0x0430
};

static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_descr,
    __fft_real16_tw
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_descr,
    __fft_real16_tw
};
const fft_handle_t rnfft32x16_192 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32x16_192 = (const fft_handle_t)&__rifft_descr;
