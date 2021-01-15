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

/* Twiddles tables for fft_cplx32x16, ifft_cplx32x16, fft_cplx16x16, ifft_cplx16x16, N=240 */
#define N 240

/****************** stage 1 radix 5 ******************/
ALIGN(8) static const int16_t __fft16_tw1[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7ff5,(int16_t)0xfca6,(int16_t)0x7fd3,(int16_t)0xf94d,(int16_t)0x7f9b,(int16_t)0xf5f5,(int16_t)0x7f4c,(int16_t)0xf29f,
(int16_t)0x7fd3,(int16_t)0xf94d,(int16_t)0x7f4c,(int16_t)0xf29f,(int16_t)0x7e6d,(int16_t)0xebfa,(int16_t)0x7d34,(int16_t)0xe563,
(int16_t)0x7f9b,(int16_t)0xf5f5,(int16_t)0x7e6d,(int16_t)0xebfa,(int16_t)0x7c77,(int16_t)0xe21e,(int16_t)0x79bc,(int16_t)0xd872,
(int16_t)0x7f4c,(int16_t)0xf29f,(int16_t)0x7d34,(int16_t)0xe563,(int16_t)0x79bc,(int16_t)0xd872,(int16_t)0x74ef,(int16_t)0xcbf0,
(int16_t)0x7ee8,(int16_t)0xef4b,(int16_t)0x7ba3,(int16_t)0xdedf,(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x6eda,(int16_t)0xc000,
(int16_t)0x7e6d,(int16_t)0xebfa,(int16_t)0x79bc,(int16_t)0xd872,(int16_t)0x720d,(int16_t)0xc5e4,(int16_t)0x678e,(int16_t)0xb4c3,
(int16_t)0x7ddb,(int16_t)0xe8ad,(int16_t)0x7780,(int16_t)0xd221,(int16_t)0x6d23,(int16_t)0xbd1f,(int16_t)0x5f1f,(int16_t)0xaa5a,
(int16_t)0x7d34,(int16_t)0xe563,(int16_t)0x74ef,(int16_t)0xcbf0,(int16_t)0x678e,(int16_t)0xb4c3,(int16_t)0x55a6,(int16_t)0xa0e1,
(int16_t)0x7c77,(int16_t)0xe21e,(int16_t)0x720d,(int16_t)0xc5e4,(int16_t)0x6155,(int16_t)0xacdf,(int16_t)0x4b3d,(int16_t)0x9872,
(int16_t)0x7ba3,(int16_t)0xdedf,(int16_t)0x6eda,(int16_t)0xc000,(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x4000,(int16_t)0x9126,
(int16_t)0x7abb,(int16_t)0xdba5,(int16_t)0x6b5a,(int16_t)0xba49,(int16_t)0x5321,(int16_t)0x9eab,(int16_t)0x3410,(int16_t)0x8b11,
(int16_t)0x79bc,(int16_t)0xd872,(int16_t)0x678e,(int16_t)0xb4c3,(int16_t)0x4b3d,(int16_t)0x9872,(int16_t)0x278e,(int16_t)0x8644,
(int16_t)0x78a8,(int16_t)0xd546,(int16_t)0x637a,(int16_t)0xaf72,(int16_t)0x42e1,(int16_t)0x92dd,(int16_t)0x1a9d,(int16_t)0x82cc,
(int16_t)0x7780,(int16_t)0xd221,(int16_t)0x5f1f,(int16_t)0xaa5a,(int16_t)0x3a1c,(int16_t)0x8df3,(int16_t)0x0d61,(int16_t)0x80b4,
(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0x0000,(int16_t)0x8000,
(int16_t)0x74ef,(int16_t)0xcbf0,(int16_t)0x55a6,(int16_t)0xa0e1,(int16_t)0x278e,(int16_t)0x8644,(int16_t)0xf29f,(int16_t)0x80b4,
(int16_t)0x7388,(int16_t)0xc8e5,(int16_t)0x508e,(int16_t)0x9c86,(int16_t)0x1de2,(int16_t)0x8389,(int16_t)0xe563,(int16_t)0x82cc,
(int16_t)0x720d,(int16_t)0xc5e4,(int16_t)0x4b3d,(int16_t)0x9872,(int16_t)0x1406,(int16_t)0x8193,(int16_t)0xd872,(int16_t)0x8644,
(int16_t)0x707d,(int16_t)0xc2ec,(int16_t)0x45b7,(int16_t)0x94a6,(int16_t)0x0a0b,(int16_t)0x8065,(int16_t)0xcbf0,(int16_t)0x8b11,
(int16_t)0x6eda,(int16_t)0xc000,(int16_t)0x4000,(int16_t)0x9126,(int16_t)0x0000,(int16_t)0x8000,(int16_t)0xc000,(int16_t)0x9126,
(int16_t)0x6d23,(int16_t)0xbd1f,(int16_t)0x3a1c,(int16_t)0x8df3,(int16_t)0xf5f5,(int16_t)0x8065,(int16_t)0xb4c3,(int16_t)0x9872,
(int16_t)0x6b5a,(int16_t)0xba49,(int16_t)0x3410,(int16_t)0x8b11,(int16_t)0xebfa,(int16_t)0x8193,(int16_t)0xaa5a,(int16_t)0xa0e1,
(int16_t)0x697d,(int16_t)0xb780,(int16_t)0x2ddf,(int16_t)0x8880,(int16_t)0xe21e,(int16_t)0x8389,(int16_t)0xa0e1,(int16_t)0xaa5a,
(int16_t)0x678e,(int16_t)0xb4c3,(int16_t)0x278e,(int16_t)0x8644,(int16_t)0xd872,(int16_t)0x8644,(int16_t)0x9872,(int16_t)0xb4c3,
(int16_t)0x658d,(int16_t)0xb214,(int16_t)0x2121,(int16_t)0x845d,(int16_t)0xcf04,(int16_t)0x89be,(int16_t)0x9126,(int16_t)0xc000,
(int16_t)0x637a,(int16_t)0xaf72,(int16_t)0x1a9d,(int16_t)0x82cc,(int16_t)0xc5e4,(int16_t)0x8df3,(int16_t)0x8b11,(int16_t)0xcbf0,
(int16_t)0x6155,(int16_t)0xacdf,(int16_t)0x1406,(int16_t)0x8193,(int16_t)0xbd1f,(int16_t)0x92dd,(int16_t)0x8644,(int16_t)0xd872,
(int16_t)0x5f1f,(int16_t)0xaa5a,(int16_t)0x0d61,(int16_t)0x80b4,(int16_t)0xb4c3,(int16_t)0x9872,(int16_t)0x82cc,(int16_t)0xe563,
(int16_t)0x5cd9,(int16_t)0xa7e4,(int16_t)0x06b3,(int16_t)0x802d,(int16_t)0xacdf,(int16_t)0x9eab,(int16_t)0x80b4,(int16_t)0xf29f,
(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xa57e,(int16_t)0x8000,(int16_t)0x0000,
(int16_t)0x581c,(int16_t)0xa327,(int16_t)0xf94d,(int16_t)0x802d,(int16_t)0x9eab,(int16_t)0xacdf,(int16_t)0x80b4,(int16_t)0x0d61,
(int16_t)0x55a6,(int16_t)0xa0e1,(int16_t)0xf29f,(int16_t)0x80b4,(int16_t)0x9872,(int16_t)0xb4c3,(int16_t)0x82cc,(int16_t)0x1a9d,
(int16_t)0x5321,(int16_t)0x9eab,(int16_t)0xebfa,(int16_t)0x8193,(int16_t)0x92dd,(int16_t)0xbd1f,(int16_t)0x8644,(int16_t)0x278e,
(int16_t)0x508e,(int16_t)0x9c86,(int16_t)0xe563,(int16_t)0x82cc,(int16_t)0x8df3,(int16_t)0xc5e4,(int16_t)0x8b11,(int16_t)0x3410,
(int16_t)0x4dec,(int16_t)0x9a73,(int16_t)0xdedf,(int16_t)0x845d,(int16_t)0x89be,(int16_t)0xcf04,(int16_t)0x9126,(int16_t)0x4000,
(int16_t)0x4b3d,(int16_t)0x9872,(int16_t)0xd872,(int16_t)0x8644,(int16_t)0x8644,(int16_t)0xd872,(int16_t)0x9872,(int16_t)0x4b3d,
(int16_t)0x4880,(int16_t)0x9683,(int16_t)0xd221,(int16_t)0x8880,(int16_t)0x8389,(int16_t)0xe21e,(int16_t)0xa0e1,(int16_t)0x55a6,
(int16_t)0x45b7,(int16_t)0x94a6,(int16_t)0xcbf0,(int16_t)0x8b11,(int16_t)0x8193,(int16_t)0xebfa,(int16_t)0xaa5a,(int16_t)0x5f1f,
(int16_t)0x42e1,(int16_t)0x92dd,(int16_t)0xc5e4,(int16_t)0x8df3,(int16_t)0x8065,(int16_t)0xf5f5,(int16_t)0xb4c3,(int16_t)0x678e,
(int16_t)0x4000,(int16_t)0x9126,(int16_t)0xc000,(int16_t)0x9126,(int16_t)0x8000,(int16_t)0x0000,(int16_t)0xc000,(int16_t)0x6eda,
(int16_t)0x3d14,(int16_t)0x8f83,(int16_t)0xba49,(int16_t)0x94a6,(int16_t)0x8065,(int16_t)0x0a0b,(int16_t)0xcbf0,(int16_t)0x74ef,
(int16_t)0x3a1c,(int16_t)0x8df3,(int16_t)0xb4c3,(int16_t)0x9872,(int16_t)0x8193,(int16_t)0x1406,(int16_t)0xd872,(int16_t)0x79bc,
(int16_t)0x371b,(int16_t)0x8c78,(int16_t)0xaf72,(int16_t)0x9c86,(int16_t)0x8389,(int16_t)0x1de2,(int16_t)0xe563,(int16_t)0x7d34,
(int16_t)0x3410,(int16_t)0x8b11,(int16_t)0xaa5a,(int16_t)0xa0e1,(int16_t)0x8644,(int16_t)0x278e,(int16_t)0xf29f,(int16_t)0x7f4c,
(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0xa57e,(int16_t)0x89be,(int16_t)0x30fc,(int16_t)0x0000,(int16_t)0x7fff,
(int16_t)0x2ddf,(int16_t)0x8880,(int16_t)0xa0e1,(int16_t)0xaa5a,(int16_t)0x8df3,(int16_t)0x3a1c,(int16_t)0x0d61,(int16_t)0x7f4c,
(int16_t)0x2aba,(int16_t)0x8758,(int16_t)0x9c86,(int16_t)0xaf72,(int16_t)0x92dd,(int16_t)0x42e1,(int16_t)0x1a9d,(int16_t)0x7d34
};
/****************** stage 2 radix 2 ******************/
ALIGN(8) static const int16_t __fft16_tw2[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7ee8,(int16_t)0xef4b,
(int16_t)0x7ba3,(int16_t)0xdedf,
(int16_t)0x7642,(int16_t)0xcf04,
(int16_t)0x6eda,(int16_t)0xc000,
(int16_t)0x658d,(int16_t)0xb214,
(int16_t)0x5a82,(int16_t)0xa57e,
(int16_t)0x4dec,(int16_t)0x9a73,
(int16_t)0x4000,(int16_t)0x9126,
(int16_t)0x30fc,(int16_t)0x89be,
(int16_t)0x2121,(int16_t)0x845d,
(int16_t)0x10b5,(int16_t)0x8118,
(int16_t)0x0000,(int16_t)0x8000,
(int16_t)0xef4b,(int16_t)0x8118,
(int16_t)0xdedf,(int16_t)0x845d,
(int16_t)0xcf04,(int16_t)0x89be,
(int16_t)0xc000,(int16_t)0x9126,
(int16_t)0xb214,(int16_t)0x9a73,
(int16_t)0xa57e,(int16_t)0xa57e,
(int16_t)0x9a73,(int16_t)0xb214,
(int16_t)0x9126,(int16_t)0xc000,
(int16_t)0x89be,(int16_t)0xcf04,
(int16_t)0x845d,(int16_t)0xdedf,
(int16_t)0x8118,(int16_t)0xef4b
};
/****************** stage 3 radix 3 ******************/
ALIGN(8) static const int16_t __fft16_tw3[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7ba3,(int16_t)0xdedf,(int16_t)0x6eda,(int16_t)0xc000,
(int16_t)0x6eda,(int16_t)0xc000,(int16_t)0x4000,(int16_t)0x9126,
(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x0000,(int16_t)0x8000,
(int16_t)0x4000,(int16_t)0x9126,(int16_t)0xc000,(int16_t)0x9126,
(int16_t)0x2121,(int16_t)0x845d,(int16_t)0x9126,(int16_t)0xc000,
(int16_t)0x0000,(int16_t)0x8000,(int16_t)0x8000,(int16_t)0x0000,
(int16_t)0xdedf,(int16_t)0x845d,(int16_t)0x9126,(int16_t)0x4000
};
static const int tw_step_tab[] =
{
    1, 1, 1, 0
}; 
static const cint16ptr_t tw_tab[] = 
{
    __fft16_tw1, __fft16_tw2, __fft16_tw3, NULL
};


static const eFft_stage_type fft_cplx16x16_stg_tab[] = 
{
    fft_stage_first_DFT5,
    fft_stage_inner_DFT2,
    fft_stage_inner_DFT3,
    fft_stage_last_DFT8
};
static const eFft_stage_type ifft_cplx16x16_stg_tab[] =
{
    fft_stage_first_inv_DFT5,
    fft_stage_inner_DFT2,
    fft_stage_inner_DFT3,
    fft_stage_last_DFT8
};

const fft_cplx_x16_descr_t __cfft_x16_descr240 = 
{
    N, 
    NULL,
    fft_cplx16x16_stg_tab,
    NULL, 
    fft_cplx16x16_stg_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_cplx_x16_descr_t __cifft_x16_descr240 =
{
    N, 
    NULL, 
    ifft_cplx16x16_stg_tab, 
    NULL, 
    ifft_cplx16x16_stg_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft16_240 = (const fft_handle_t)&__cfft_x16_descr240;
const fft_handle_t cinfft16_240 = (const fft_handle_t)&__cifft_x16_descr240;
