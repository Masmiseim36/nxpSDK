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

/* Twiddles tables for fft_cplx32x16, ifft_cplx32x16, fft_cplx16x16, ifft_cplx16x16, N=256 */
#define N 256

/****************** stage 1 radix 4 ******************/
ALIGN(8) static const int16_t __fft16_tw1[] = 
{
    (int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
    (int16_t)0x7ff6,(int16_t)0xfcdc,(int16_t)0x7fd9,(int16_t)0xf9b8,(int16_t)0x7fa7,(int16_t)0xf695,
    (int16_t)0x7fd9,(int16_t)0xf9b8,(int16_t)0x7f62,(int16_t)0xf374,(int16_t)0x7e9d,(int16_t)0xed38,
    (int16_t)0x7fa7,(int16_t)0xf695,(int16_t)0x7e9d,(int16_t)0xed38,(int16_t)0x7ce4,(int16_t)0xe3f4,
    (int16_t)0x7f62,(int16_t)0xf374,(int16_t)0x7d8a,(int16_t)0xe707,(int16_t)0x7a7d,(int16_t)0xdad8,
    (int16_t)0x7f0a,(int16_t)0xf055,(int16_t)0x7c2a,(int16_t)0xe0e6,(int16_t)0x776c,(int16_t)0xd1ef,
    (int16_t)0x7e9d,(int16_t)0xed38,(int16_t)0x7a7d,(int16_t)0xdad8,(int16_t)0x73b6,(int16_t)0xc946,
    (int16_t)0x7e1e,(int16_t)0xea1e,(int16_t)0x7885,(int16_t)0xd4e1,(int16_t)0x6f5f,(int16_t)0xc0e9,
    (int16_t)0x7d8a,(int16_t)0xe707,(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x6a6e,(int16_t)0xb8e3,
    (int16_t)0x7ce4,(int16_t)0xe3f4,(int16_t)0x73b6,(int16_t)0xc946,(int16_t)0x64e9,(int16_t)0xb140,
    (int16_t)0x7c2a,(int16_t)0xe0e6,(int16_t)0x70e3,(int16_t)0xc3a9,(int16_t)0x5ed7,(int16_t)0xaa0a,
    (int16_t)0x7b5d,(int16_t)0xdddc,(int16_t)0x6dca,(int16_t)0xbe32,(int16_t)0x5843,(int16_t)0xa34c,
    (int16_t)0x7a7d,(int16_t)0xdad8,(int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x5134,(int16_t)0x9d0e,
    (int16_t)0x798a,(int16_t)0xd7d9,(int16_t)0x66d0,(int16_t)0xb3c0,(int16_t)0x49b4,(int16_t)0x9759,
    (int16_t)0x7885,(int16_t)0xd4e1,(int16_t)0x62f2,(int16_t)0xaecc,(int16_t)0x41ce,(int16_t)0x9236,
    (int16_t)0x776c,(int16_t)0xd1ef,(int16_t)0x5ed7,(int16_t)0xaa0a,(int16_t)0x398d,(int16_t)0x8dab,
    (int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x30fc,(int16_t)0x89be,
    (int16_t)0x7505,(int16_t)0xcc21,(int16_t)0x55f6,(int16_t)0xa129,(int16_t)0x2827,(int16_t)0x8676,
    (int16_t)0x73b6,(int16_t)0xc946,(int16_t)0x5134,(int16_t)0x9d0e,(int16_t)0x1f1a,(int16_t)0x83d6,
    (int16_t)0x7255,(int16_t)0xc673,(int16_t)0x4c40,(int16_t)0x9930,(int16_t)0x15e2,(int16_t)0x81e2,
    (int16_t)0x70e3,(int16_t)0xc3a9,(int16_t)0x471d,(int16_t)0x9592,(int16_t)0x0c8c,(int16_t)0x809e,
    (int16_t)0x6f5f,(int16_t)0xc0e9,(int16_t)0x41ce,(int16_t)0x9236,(int16_t)0x0324,(int16_t)0x800a,
    (int16_t)0x6dca,(int16_t)0xbe32,(int16_t)0x3c57,(int16_t)0x8f1d,(int16_t)0xf9b8,(int16_t)0x8027,
    (int16_t)0x6c24,(int16_t)0xbb85,(int16_t)0x36ba,(int16_t)0x8c4a,(int16_t)0xf055,(int16_t)0x80f6,
    (int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xe707,(int16_t)0x8276,
    (int16_t)0x68a7,(int16_t)0xb64c,(int16_t)0x2b1f,(int16_t)0x877b,(int16_t)0xdddc,(int16_t)0x84a3,
    (int16_t)0x66d0,(int16_t)0xb3c0,(int16_t)0x2528,(int16_t)0x8583,(int16_t)0xd4e1,(int16_t)0x877b,
    (int16_t)0x64e9,(int16_t)0xb140,(int16_t)0x1f1a,(int16_t)0x83d6,(int16_t)0xcc21,(int16_t)0x8afb,
    (int16_t)0x62f2,(int16_t)0xaecc,(int16_t)0x18f9,(int16_t)0x8276,(int16_t)0xc3a9,(int16_t)0x8f1d,
    (int16_t)0x60ec,(int16_t)0xac65,(int16_t)0x12c8,(int16_t)0x8163,(int16_t)0xbb85,(int16_t)0x93dc,
    (int16_t)0x5ed7,(int16_t)0xaa0a,(int16_t)0x0c8c,(int16_t)0x809e,(int16_t)0xb3c0,(int16_t)0x9930,
    (int16_t)0x5cb4,(int16_t)0xa7bd,(int16_t)0x0648,(int16_t)0x8027,(int16_t)0xac65,(int16_t)0x9f14,
    (int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xa57e,
    (int16_t)0x5843,(int16_t)0xa34c,(int16_t)0xf9b8,(int16_t)0x8027,(int16_t)0x9f14,(int16_t)0xac65,
    (int16_t)0x55f6,(int16_t)0xa129,(int16_t)0xf374,(int16_t)0x809e,(int16_t)0x9930,(int16_t)0xb3c0,
    (int16_t)0x539b,(int16_t)0x9f14,(int16_t)0xed38,(int16_t)0x8163,(int16_t)0x93dc,(int16_t)0xbb85,
    (int16_t)0x5134,(int16_t)0x9d0e,(int16_t)0xe707,(int16_t)0x8276,(int16_t)0x8f1d,(int16_t)0xc3a9,
    (int16_t)0x4ec0,(int16_t)0x9b17,(int16_t)0xe0e6,(int16_t)0x83d6,(int16_t)0x8afb,(int16_t)0xcc21,
    (int16_t)0x4c40,(int16_t)0x9930,(int16_t)0xdad8,(int16_t)0x8583,(int16_t)0x877b,(int16_t)0xd4e1,
    (int16_t)0x49b4,(int16_t)0x9759,(int16_t)0xd4e1,(int16_t)0x877b,(int16_t)0x84a3,(int16_t)0xdddc,
    (int16_t)0x471d,(int16_t)0x9592,(int16_t)0xcf04,(int16_t)0x89be,(int16_t)0x8276,(int16_t)0xe707,
    (int16_t)0x447b,(int16_t)0x93dc,(int16_t)0xc946,(int16_t)0x8c4a,(int16_t)0x80f6,(int16_t)0xf055,
    (int16_t)0x41ce,(int16_t)0x9236,(int16_t)0xc3a9,(int16_t)0x8f1d,(int16_t)0x8027,(int16_t)0xf9b8,
    (int16_t)0x3f17,(int16_t)0x90a1,(int16_t)0xbe32,(int16_t)0x9236,(int16_t)0x800a,(int16_t)0x0324,
    (int16_t)0x3c57,(int16_t)0x8f1d,(int16_t)0xb8e3,(int16_t)0x9592,(int16_t)0x809e,(int16_t)0x0c8c,
    (int16_t)0x398d,(int16_t)0x8dab,(int16_t)0xb3c0,(int16_t)0x9930,(int16_t)0x81e2,(int16_t)0x15e2,
    (int16_t)0x36ba,(int16_t)0x8c4a,(int16_t)0xaecc,(int16_t)0x9d0e,(int16_t)0x83d6,(int16_t)0x1f1a,
    (int16_t)0x33df,(int16_t)0x8afb,(int16_t)0xaa0a,(int16_t)0xa129,(int16_t)0x8676,(int16_t)0x2827,
    (int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0xa57e,(int16_t)0x89be,(int16_t)0x30fc,
    (int16_t)0x2e11,(int16_t)0x8894,(int16_t)0xa129,(int16_t)0xaa0a,(int16_t)0x8dab,(int16_t)0x398d,
    (int16_t)0x2b1f,(int16_t)0x877b,(int16_t)0x9d0e,(int16_t)0xaecc,(int16_t)0x9236,(int16_t)0x41ce,
    (int16_t)0x2827,(int16_t)0x8676,(int16_t)0x9930,(int16_t)0xb3c0,(int16_t)0x9759,(int16_t)0x49b4,
    (int16_t)0x2528,(int16_t)0x8583,(int16_t)0x9592,(int16_t)0xb8e3,(int16_t)0x9d0e,(int16_t)0x5134,
    (int16_t)0x2224,(int16_t)0x84a3,(int16_t)0x9236,(int16_t)0xbe32,(int16_t)0xa34c,(int16_t)0x5843,
    (int16_t)0x1f1a,(int16_t)0x83d6,(int16_t)0x8f1d,(int16_t)0xc3a9,(int16_t)0xaa0a,(int16_t)0x5ed7,
    (int16_t)0x1c0c,(int16_t)0x831c,(int16_t)0x8c4a,(int16_t)0xc946,(int16_t)0xb140,(int16_t)0x64e9,
    (int16_t)0x18f9,(int16_t)0x8276,(int16_t)0x89be,(int16_t)0xcf04,(int16_t)0xb8e3,(int16_t)0x6a6e,
    (int16_t)0x15e2,(int16_t)0x81e2,(int16_t)0x877b,(int16_t)0xd4e1,(int16_t)0xc0e9,(int16_t)0x6f5f,
    (int16_t)0x12c8,(int16_t)0x8163,(int16_t)0x8583,(int16_t)0xdad8,(int16_t)0xc946,(int16_t)0x73b6,
    (int16_t)0x0fab,(int16_t)0x80f6,(int16_t)0x83d6,(int16_t)0xe0e6,(int16_t)0xd1ef,(int16_t)0x776c,
    (int16_t)0x0c8c,(int16_t)0x809e,(int16_t)0x8276,(int16_t)0xe707,(int16_t)0xdad8,(int16_t)0x7a7d,
    (int16_t)0x096b,(int16_t)0x8059,(int16_t)0x8163,(int16_t)0xed38,(int16_t)0xe3f4,(int16_t)0x7ce4,
    (int16_t)0x0648,(int16_t)0x8027,(int16_t)0x809e,(int16_t)0xf374,(int16_t)0xed38,(int16_t)0x7e9d,
    (int16_t)0x0324,(int16_t)0x800a,(int16_t)0x8027,(int16_t)0xf9b8,(int16_t)0xf695,(int16_t)0x7fa7
};
/****************** stage 2 radix 4 ******************/
ALIGN(8) static const int16_t __fft16_tw2[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7f62,(int16_t)0xf374,(int16_t)0x7d8a,(int16_t)0xe707,(int16_t)0x7a7d,(int16_t)0xdad8,
(int16_t)0x7d8a,(int16_t)0xe707,(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x6a6e,(int16_t)0xb8e3,
(int16_t)0x7a7d,(int16_t)0xdad8,(int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x5134,(int16_t)0x9d0e,
(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x30fc,(int16_t)0x89be,
(int16_t)0x70e3,(int16_t)0xc3a9,(int16_t)0x471d,(int16_t)0x9592,(int16_t)0x0c8c,(int16_t)0x809e,
(int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xe707,(int16_t)0x8276,
(int16_t)0x62f2,(int16_t)0xaecc,(int16_t)0x18f9,(int16_t)0x8276,(int16_t)0xc3a9,(int16_t)0x8f1d,
(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xa57e,
(int16_t)0x5134,(int16_t)0x9d0e,(int16_t)0xe707,(int16_t)0x8276,(int16_t)0x8f1d,(int16_t)0xc3a9,
(int16_t)0x471d,(int16_t)0x9592,(int16_t)0xcf04,(int16_t)0x89be,(int16_t)0x8276,(int16_t)0xe707,
(int16_t)0x3c57,(int16_t)0x8f1d,(int16_t)0xb8e3,(int16_t)0x9592,(int16_t)0x809e,(int16_t)0x0c8c,
(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0xa57e,(int16_t)0x89be,(int16_t)0x30fc,
(int16_t)0x2528,(int16_t)0x8583,(int16_t)0x9592,(int16_t)0xb8e3,(int16_t)0x9d0e,(int16_t)0x5134,
(int16_t)0x18f9,(int16_t)0x8276,(int16_t)0x89be,(int16_t)0xcf04,(int16_t)0xb8e3,(int16_t)0x6a6e,
(int16_t)0x0c8c,(int16_t)0x809e,(int16_t)0x8276,(int16_t)0xe707,(int16_t)0xdad8,(int16_t)0x7a7d
};
/****************** stage 3 radix 4 ******************/
ALIGN(8) static const int16_t __fft16_tw3[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x30fc,(int16_t)0x89be,
(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xa57e,
(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0xa57e,(int16_t)0x89be,(int16_t)0x30fc
};
static const int tw_step_tab[] =
{
    1, 1, 1, 0
}; 
static const cint16ptr_t tw_tab[] = 
{
    __fft16_tw1, __fft16_tw2, __fft16_tw3, NULL
};

static const eFft_stage_type fft_stg_tab[] = 
{
    fft_stage_first_DFT4,
    fft_stage_inner_DFT4x2,
    fft_stage_penultimate_DFT4,
    fft_stage_last_DFT4
};
static const eFft_stage_type ifft_stg_tab[] =
{
    fft_stage_first_inv_DFT4,
    fft_stage_inner_DFT4x2,
    fft_stage_penultimate_DFT4,
    fft_stage_last_DFT4
};


const fft_cplx_x16_descr_t __cfft_x16_descr256 = 
{
    N, 
    fft_stg_tab, 
    fft_stg_tab, 
    fft_stg_tab, 
    fft_stg_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_cplx_x16_descr_t __cifft_x16_descr256 =
{
    N, 
    ifft_stg_tab, 
    ifft_stg_tab, 
    ifft_stg_tab, 
    ifft_stg_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cfft16_256 = (const fft_handle_t)&__cfft_x16_descr256;
const fft_handle_t cifft16_256 = (const fft_handle_t)&__cifft_x16_descr256;
