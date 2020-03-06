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
#include "fft_twiddles32x32.h"
#include "common.h"

ALIGN(8) static const int32_t __fft_real_tw[] =
{
  (int32_t)0x7fffffff,(int32_t)0x00000000,
  (int32_t)0x7fe6bcb0,(int32_t)0xfaf988cc,
  (int32_t)0x7f9afcb9,(int32_t)0xf5f50d67,
  (int32_t)0x7f1cde01,(int32_t)0xf0f488d9,
  (int32_t)0x7e6c9251,(int32_t)0xebf9f498,
  (int32_t)0x7d8a5f40,(int32_t)0xe70747c4,
  (int32_t)0x7c769e18,(int32_t)0xe21e765a,
  (int32_t)0x7b31bbb2,(int32_t)0xdd417079,
  (int32_t)0x79bc384d,(int32_t)0xd8722192,
  (int32_t)0x7816a759,(int32_t)0xd3b26fb0,
  (int32_t)0x7641af3d,(int32_t)0xcf043ab3,
  (int32_t)0x743e0918,(int32_t)0xca695b94,
  (int32_t)0x720c8075,(int32_t)0xc5e3a3a9,
  (int32_t)0x6fadf2fc,(int32_t)0xc174dbf2,
  (int32_t)0x6d23501b,(int32_t)0xbd1ec45c,
  (int32_t)0x6a6d98a4,(int32_t)0xb8e31319,
  (int32_t)0x678dde6e,(int32_t)0xb4c373ee,
  (int32_t)0x648543e4,(int32_t)0xb0c1878b,
  (int32_t)0x6154fb91,(int32_t)0xacdee2e8,
  (int32_t)0x5dfe47ad,(int32_t)0xa91d0ea3,
  (int32_t)0x5a82799a,(int32_t)0xa57d8666,
  (int32_t)0x56e2f15d,(int32_t)0xa201b853,
  (int32_t)0x53211d18,(int32_t)0x9eab046f,
  (int32_t)0x4f3e7875,(int32_t)0x9b7abc1c,
  (int32_t)0x4b3c8c12,(int32_t)0x98722192,
  (int32_t)0x471cece7,(int32_t)0x9592675c,
  (int32_t)0x42e13ba4,(int32_t)0x92dcafe5,
  (int32_t)0x3e8b240e,(int32_t)0x90520d04,
  (int32_t)0x3a1c5c57,(int32_t)0x8df37f8b,
  (int32_t)0x3596a46c,(int32_t)0x8bc1f6e8,
  (int32_t)0x30fbc54d,(int32_t)0x89be50c3,
  (int32_t)0x2c4d9050,(int32_t)0x87e958a7,
  (int32_t)0x278dde6e,(int32_t)0x8643c7b3,
  (int32_t)0x22be8f87,(int32_t)0x84ce444e,
  (int32_t)0x1de189a6,(int32_t)0x838961e8,
  (int32_t)0x18f8b83c,(int32_t)0x8275a0c0,
  (int32_t)0x14060b68,(int32_t)0x81936daf,
  (int32_t)0x0f0b7727,(int32_t)0x80e321ff,
  (int32_t)0x0a0af299,(int32_t)0x80650347,
  (int32_t)0x05067734,(int32_t)0x80194350
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr80_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr80_32x32,
    __fft_real_tw
};

const fft_handle_t rnfft32_160=  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_160 = (const fft_handle_t)&__rifft_descr;
