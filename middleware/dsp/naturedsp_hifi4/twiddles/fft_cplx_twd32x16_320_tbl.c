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

/* Twiddles tables for fft_cplx32x16, ifft_cplx32x16, fft_cplx16x16, ifft_cplx16x16, N=320 */
#define N 320

/****************** stage 1 radix 5 ******************/
ALIGN(8) static const int16_t __fft16_tw1[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7ffa,(int16_t)0xfd7d,(int16_t)0x7fe7,(int16_t)0xfafa,(int16_t)0x7fc7,(int16_t)0xf877,(int16_t)0x7f9b,(int16_t)0xf5f5,
(int16_t)0x7fe7,(int16_t)0xfafa,(int16_t)0x7f9b,(int16_t)0xf5f5,(int16_t)0x7f1d,(int16_t)0xf0f5,(int16_t)0x7e6d,(int16_t)0xebfa,
(int16_t)0x7fc7,(int16_t)0xf877,(int16_t)0x7f1d,(int16_t)0xf0f5,(int16_t)0x7e02,(int16_t)0xe980,(int16_t)0x7c77,(int16_t)0xe21e,
(int16_t)0x7f9b,(int16_t)0xf5f5,(int16_t)0x7e6d,(int16_t)0xebfa,(int16_t)0x7c77,(int16_t)0xe21e,(int16_t)0x79bc,(int16_t)0xd872,
(int16_t)0x7f62,(int16_t)0xf374,(int16_t)0x7d8a,(int16_t)0xe707,(int16_t)0x7a7d,(int16_t)0xdad8,(int16_t)0x7642,(int16_t)0xcf04,
(int16_t)0x7f1d,(int16_t)0xf0f5,(int16_t)0x7c77,(int16_t)0xe21e,(int16_t)0x7817,(int16_t)0xd3b2,(int16_t)0x720d,(int16_t)0xc5e4,
(int16_t)0x7ecb,(int16_t)0xee76,(int16_t)0x7b32,(int16_t)0xdd41,(int16_t)0x7546,(int16_t)0xccb4,(int16_t)0x6d23,(int16_t)0xbd1f,
(int16_t)0x7e6d,(int16_t)0xebfa,(int16_t)0x79bc,(int16_t)0xd872,(int16_t)0x720d,(int16_t)0xc5e4,(int16_t)0x678e,(int16_t)0xb4c3,
(int16_t)0x7e02,(int16_t)0xe980,(int16_t)0x7817,(int16_t)0xd3b2,(int16_t)0x6e6e,(int16_t)0xbf47,(int16_t)0x6155,(int16_t)0xacdf,
(int16_t)0x7d8a,(int16_t)0xe707,(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x5a82,(int16_t)0xa57e,
(int16_t)0x7d07,(int16_t)0xe492,(int16_t)0x743e,(int16_t)0xca69,(int16_t)0x660f,(int16_t)0xb2bf,(int16_t)0x5321,(int16_t)0x9eab,
(int16_t)0x7c77,(int16_t)0xe21e,(int16_t)0x720d,(int16_t)0xc5e4,(int16_t)0x6155,(int16_t)0xacdf,(int16_t)0x4b3d,(int16_t)0x9872,
(int16_t)0x7bda,(int16_t)0xdfae,(int16_t)0x6fae,(int16_t)0xc175,(int16_t)0x5c45,(int16_t)0xa749,(int16_t)0x42e1,(int16_t)0x92dd,
(int16_t)0x7b32,(int16_t)0xdd41,(int16_t)0x6d23,(int16_t)0xbd1f,(int16_t)0x56e3,(int16_t)0xa202,(int16_t)0x3a1c,(int16_t)0x8df3,
(int16_t)0x7a7d,(int16_t)0xdad8,(int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x5134,(int16_t)0x9d0e,(int16_t)0x30fc,(int16_t)0x89be,
(int16_t)0x79bc,(int16_t)0xd872,(int16_t)0x678e,(int16_t)0xb4c3,(int16_t)0x4b3d,(int16_t)0x9872,(int16_t)0x278e,(int16_t)0x8644,
(int16_t)0x78ef,(int16_t)0xd610,(int16_t)0x6485,(int16_t)0xb0c2,(int16_t)0x4502,(int16_t)0x9432,(int16_t)0x1de2,(int16_t)0x8389,
(int16_t)0x7817,(int16_t)0xd3b2,(int16_t)0x6155,(int16_t)0xacdf,(int16_t)0x3e8b,(int16_t)0x9052,(int16_t)0x1406,(int16_t)0x8193,
(int16_t)0x7732,(int16_t)0xd159,(int16_t)0x5dfe,(int16_t)0xa91d,(int16_t)0x37dc,(int16_t)0x8cd5,(int16_t)0x0a0b,(int16_t)0x8065,
(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0x0000,(int16_t)0x8000,
(int16_t)0x7546,(int16_t)0xccb4,(int16_t)0x56e3,(int16_t)0xa202,(int16_t)0x29f0,(int16_t)0x8711,(int16_t)0xf5f5,(int16_t)0x8065,
(int16_t)0x743e,(int16_t)0xca69,(int16_t)0x5321,(int16_t)0x9eab,(int16_t)0x22bf,(int16_t)0x84ce,(int16_t)0xebfa,(int16_t)0x8193,
(int16_t)0x732b,(int16_t)0xc824,(int16_t)0x4f3e,(int16_t)0x9b7b,(int16_t)0x1b6e,(int16_t)0x82f9,(int16_t)0xe21e,(int16_t)0x8389,
(int16_t)0x720d,(int16_t)0xc5e4,(int16_t)0x4b3d,(int16_t)0x9872,(int16_t)0x1406,(int16_t)0x8193,(int16_t)0xd872,(int16_t)0x8644,
(int16_t)0x70e3,(int16_t)0xc3a9,(int16_t)0x471d,(int16_t)0x9592,(int16_t)0x0c8c,(int16_t)0x809e,(int16_t)0xcf04,(int16_t)0x89be,
(int16_t)0x6fae,(int16_t)0xc175,(int16_t)0x42e1,(int16_t)0x92dd,(int16_t)0x0506,(int16_t)0x8019,(int16_t)0xc5e4,(int16_t)0x8df3,
(int16_t)0x6e6e,(int16_t)0xbf47,(int16_t)0x3e8b,(int16_t)0x9052,(int16_t)0xfd7d,(int16_t)0x8006,(int16_t)0xbd1f,(int16_t)0x92dd,
(int16_t)0x6d23,(int16_t)0xbd1f,(int16_t)0x3a1c,(int16_t)0x8df3,(int16_t)0xf5f5,(int16_t)0x8065,(int16_t)0xb4c3,(int16_t)0x9872,
(int16_t)0x6bce,(int16_t)0xbafe,(int16_t)0x3597,(int16_t)0x8bc2,(int16_t)0xee76,(int16_t)0x8135,(int16_t)0xacdf,(int16_t)0x9eab,
(int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xe707,(int16_t)0x8276,(int16_t)0xa57e,(int16_t)0xa57e,
(int16_t)0x6903,(int16_t)0xb6d0,(int16_t)0x2c4e,(int16_t)0x87e9,(int16_t)0xdfae,(int16_t)0x8426,(int16_t)0x9eab,(int16_t)0xacdf,
(int16_t)0x678e,(int16_t)0xb4c3,(int16_t)0x278e,(int16_t)0x8644,(int16_t)0xd872,(int16_t)0x8644,(int16_t)0x9872,(int16_t)0xb4c3,
(int16_t)0x660f,(int16_t)0xb2bf,(int16_t)0x22bf,(int16_t)0x84ce,(int16_t)0xd159,(int16_t)0x88ce,(int16_t)0x92dd,(int16_t)0xbd1f,
(int16_t)0x6485,(int16_t)0xb0c2,(int16_t)0x1de2,(int16_t)0x8389,(int16_t)0xca69,(int16_t)0x8bc2,(int16_t)0x8df3,(int16_t)0xc5e4,
(int16_t)0x62f2,(int16_t)0xaecc,(int16_t)0x18f9,(int16_t)0x8276,(int16_t)0xc3a9,(int16_t)0x8f1d,(int16_t)0x89be,(int16_t)0xcf04,
(int16_t)0x6155,(int16_t)0xacdf,(int16_t)0x1406,(int16_t)0x8193,(int16_t)0xbd1f,(int16_t)0x92dd,(int16_t)0x8644,(int16_t)0xd872,
(int16_t)0x5fae,(int16_t)0xaafa,(int16_t)0x0f0b,(int16_t)0x80e3,(int16_t)0xb6d0,(int16_t)0x96fd,(int16_t)0x8389,(int16_t)0xe21e,
(int16_t)0x5dfe,(int16_t)0xa91d,(int16_t)0x0a0b,(int16_t)0x8065,(int16_t)0xb0c2,(int16_t)0x9b7b,(int16_t)0x8193,(int16_t)0xebfa,
(int16_t)0x5c45,(int16_t)0xa749,(int16_t)0x0506,(int16_t)0x8019,(int16_t)0xaafa,(int16_t)0xa052,(int16_t)0x8065,(int16_t)0xf5f5,
(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xa57e,(int16_t)0x8000,(int16_t)0x0000,
(int16_t)0x58b7,(int16_t)0xa3bb,(int16_t)0xfafa,(int16_t)0x8019,(int16_t)0xa052,(int16_t)0xaafa,(int16_t)0x8065,(int16_t)0x0a0b,
(int16_t)0x56e3,(int16_t)0xa202,(int16_t)0xf5f5,(int16_t)0x8065,(int16_t)0x9b7b,(int16_t)0xb0c2,(int16_t)0x8193,(int16_t)0x1406,
(int16_t)0x5506,(int16_t)0xa052,(int16_t)0xf0f5,(int16_t)0x80e3,(int16_t)0x96fd,(int16_t)0xb6d0,(int16_t)0x8389,(int16_t)0x1de2,
(int16_t)0x5321,(int16_t)0x9eab,(int16_t)0xebfa,(int16_t)0x8193,(int16_t)0x92dd,(int16_t)0xbd1f,(int16_t)0x8644,(int16_t)0x278e,
(int16_t)0x5134,(int16_t)0x9d0e,(int16_t)0xe707,(int16_t)0x8276,(int16_t)0x8f1d,(int16_t)0xc3a9,(int16_t)0x89be,(int16_t)0x30fc,
(int16_t)0x4f3e,(int16_t)0x9b7b,(int16_t)0xe21e,(int16_t)0x8389,(int16_t)0x8bc2,(int16_t)0xca69,(int16_t)0x8df3,(int16_t)0x3a1c,
(int16_t)0x4d41,(int16_t)0x99f1,(int16_t)0xdd41,(int16_t)0x84ce,(int16_t)0x88ce,(int16_t)0xd159,(int16_t)0x92dd,(int16_t)0x42e1,
(int16_t)0x4b3d,(int16_t)0x9872,(int16_t)0xd872,(int16_t)0x8644,(int16_t)0x8644,(int16_t)0xd872,(int16_t)0x9872,(int16_t)0x4b3d,
(int16_t)0x4930,(int16_t)0x96fd,(int16_t)0xd3b2,(int16_t)0x87e9,(int16_t)0x8426,(int16_t)0xdfae,(int16_t)0x9eab,(int16_t)0x5321,
(int16_t)0x471d,(int16_t)0x9592,(int16_t)0xcf04,(int16_t)0x89be,(int16_t)0x8276,(int16_t)0xe707,(int16_t)0xa57e,(int16_t)0x5a82,
(int16_t)0x4502,(int16_t)0x9432,(int16_t)0xca69,(int16_t)0x8bc2,(int16_t)0x8135,(int16_t)0xee76,(int16_t)0xacdf,(int16_t)0x6155,
(int16_t)0x42e1,(int16_t)0x92dd,(int16_t)0xc5e4,(int16_t)0x8df3,(int16_t)0x8065,(int16_t)0xf5f5,(int16_t)0xb4c3,(int16_t)0x678e,
(int16_t)0x40b9,(int16_t)0x9192,(int16_t)0xc175,(int16_t)0x9052,(int16_t)0x8006,(int16_t)0xfd7d,(int16_t)0xbd1f,(int16_t)0x6d23,
(int16_t)0x3e8b,(int16_t)0x9052,(int16_t)0xbd1f,(int16_t)0x92dd,(int16_t)0x8019,(int16_t)0x0506,(int16_t)0xc5e4,(int16_t)0x720d,
(int16_t)0x3c57,(int16_t)0x8f1d,(int16_t)0xb8e3,(int16_t)0x9592,(int16_t)0x809e,(int16_t)0x0c8c,(int16_t)0xcf04,(int16_t)0x7642,
(int16_t)0x3a1c,(int16_t)0x8df3,(int16_t)0xb4c3,(int16_t)0x9872,(int16_t)0x8193,(int16_t)0x1406,(int16_t)0xd872,(int16_t)0x79bc,
(int16_t)0x37dc,(int16_t)0x8cd5,(int16_t)0xb0c2,(int16_t)0x9b7b,(int16_t)0x82f9,(int16_t)0x1b6e,(int16_t)0xe21e,(int16_t)0x7c77,
(int16_t)0x3597,(int16_t)0x8bc2,(int16_t)0xacdf,(int16_t)0x9eab,(int16_t)0x84ce,(int16_t)0x22bf,(int16_t)0xebfa,(int16_t)0x7e6d,
(int16_t)0x334c,(int16_t)0x8aba,(int16_t)0xa91d,(int16_t)0xa202,(int16_t)0x8711,(int16_t)0x29f0,(int16_t)0xf5f5,(int16_t)0x7f9b,
(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0xa57e,(int16_t)0x89be,(int16_t)0x30fc,(int16_t)0x0000,(int16_t)0x7fff,
(int16_t)0x2ea7,(int16_t)0x88ce,(int16_t)0xa202,(int16_t)0xa91d,(int16_t)0x8cd5,(int16_t)0x37dc,(int16_t)0x0a0b,(int16_t)0x7f9b,
(int16_t)0x2c4e,(int16_t)0x87e9,(int16_t)0x9eab,(int16_t)0xacdf,(int16_t)0x9052,(int16_t)0x3e8b,(int16_t)0x1406,(int16_t)0x7e6d,
(int16_t)0x29f0,(int16_t)0x8711,(int16_t)0x9b7b,(int16_t)0xb0c2,(int16_t)0x9432,(int16_t)0x4502,(int16_t)0x1de2,(int16_t)0x7c77
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

static const eFft_stage_type fft_cplx32x16_scl2_tab[] = 
{
    fft_stage_first_DFT5,
    fft_stage_inner_DFT4,
    fft_stage_penultimate_DFT4,
    fft_stage_last_DFT4
};
static const eFft_stage_type fft_cplx32x16_scl3_tab[] =
{
    fft_stage_first_DFT5,
    fft_stage_inner_DFT4,
    fft_stage_penultimate_DFT4,
    fft_stage_last_DFT4
};
static const eFft_stage_type ifft_cplx32x16_scl2_tab[] = 
{
    fft_stage_first_inv_DFT5,
    fft_stage_inner_DFT4,
    fft_stage_penultimate_DFT4,
    fft_stage_last_DFT4
};
static const eFft_stage_type ifft_cplx32x16_scl3_tab[] =
{
    fft_stage_first_inv_DFT5,
    fft_stage_inner_DFT4,
    fft_stage_penultimate_DFT4,
    fft_stage_last_DFT4
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
const fft_handle_t cnfft32x16_320 = (const fft_handle_t)&__cfft_descr;
const fft_handle_t cinfft32x16_320 = (const fft_handle_t)&__cifft_descr;
