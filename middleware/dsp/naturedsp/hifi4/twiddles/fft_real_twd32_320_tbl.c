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
  (int32_t)0x7ff9af04,(int32_t)0xfd7ca4a6,
  (int32_t)0x7fe6bcb0,(int32_t)0xfaf988cc,
  (int32_t)0x7fc72ae2,(int32_t)0xf876ebe8,
  (int32_t)0x7f9afcb9,(int32_t)0xf5f50d67,
  (int32_t)0x7f62368f,(int32_t)0xf3742ca2,
  (int32_t)0x7f1cde01,(int32_t)0xf0f488d9,
  (int32_t)0x7ecaf9e5,(int32_t)0xee76612d,
  (int32_t)0x7e6c9251,(int32_t)0xebf9f498,
  (int32_t)0x7e01b096,(int32_t)0xe97f81eb,
  (int32_t)0x7d8a5f40,(int32_t)0xe70747c4,
  (int32_t)0x7d06aa16,(int32_t)0xe4918486,
  (int32_t)0x7c769e18,(int32_t)0xe21e765a,
  (int32_t)0x7bda497d,(int32_t)0xdfae5b23,
  (int32_t)0x7b31bbb2,(int32_t)0xdd417079,
  (int32_t)0x7a7d055b,(int32_t)0xdad7f3a2,
  (int32_t)0x79bc384d,(int32_t)0xd8722192,
  (int32_t)0x78ef678f,(int32_t)0xd61036db,
  (int32_t)0x7816a759,(int32_t)0xd3b26fb0,
  (int32_t)0x77320d0d,(int32_t)0xd15907d9,
  (int32_t)0x7641af3d,(int32_t)0xcf043ab3,
  (int32_t)0x7545a5a0,(int32_t)0xccb44322,
  (int32_t)0x743e0918,(int32_t)0xca695b94,
  (int32_t)0x732af3a7,(int32_t)0xc823bdf4,
  (int32_t)0x720c8075,(int32_t)0xc5e3a3a9,
  (int32_t)0x70e2cbc6,(int32_t)0xc3a94590,
  (int32_t)0x6fadf2fc,(int32_t)0xc174dbf2,
  (int32_t)0x6e6e1492,(int32_t)0xbf469e83,
  (int32_t)0x6d23501b,(int32_t)0xbd1ec45c,
  (int32_t)0x6bcdc639,(int32_t)0xbafd83f4,
  (int32_t)0x6a6d98a4,(int32_t)0xb8e31319,
  (int32_t)0x6902ea1d,(int32_t)0xb6cfa6f1,
  (int32_t)0x678dde6e,(int32_t)0xb4c373ee,
  (int32_t)0x660e9a6a,(int32_t)0xb2beadcc,
  (int32_t)0x648543e4,(int32_t)0xb0c1878b,
  (int32_t)0x62f201ac,(int32_t)0xaecc336c,
  (int32_t)0x6154fb91,(int32_t)0xacdee2e8,
  (int32_t)0x5fae5a55,(int32_t)0xaaf9c6af,
  (int32_t)0x5dfe47ad,(int32_t)0xa91d0ea3,
  (int32_t)0x5c44ee40,(int32_t)0xa748e9ce,
  (int32_t)0x5a82799a,(int32_t)0xa57d8666,
  (int32_t)0x58b71632,(int32_t)0xa3bb11c0,
  (int32_t)0x56e2f15d,(int32_t)0xa201b853,
  (int32_t)0x55063951,(int32_t)0xa051a5ab,
  (int32_t)0x53211d18,(int32_t)0x9eab046f,
  (int32_t)0x5133cc94,(int32_t)0x9d0dfe54,
  (int32_t)0x4f3e7875,(int32_t)0x9b7abc1c,
  (int32_t)0x4d415234,(int32_t)0x99f16596,
  (int32_t)0x4b3c8c12,(int32_t)0x98722192,
  (int32_t)0x4930590f,(int32_t)0x96fd15e3,
  (int32_t)0x471cece7,(int32_t)0x9592675c,
  (int32_t)0x45027c0c,(int32_t)0x943239c7,
  (int32_t)0x42e13ba4,(int32_t)0x92dcafe5,
  (int32_t)0x40b9617d,(int32_t)0x9191eb6e,
  (int32_t)0x3e8b240e,(int32_t)0x90520d04,
  (int32_t)0x3c56ba70,(int32_t)0x8f1d343a,
  (int32_t)0x3a1c5c57,(int32_t)0x8df37f8b,
  (int32_t)0x37dc420c,(int32_t)0x8cd50c59,
  (int32_t)0x3596a46c,(int32_t)0x8bc1f6e8,
  (int32_t)0x334bbcde,(int32_t)0x8aba5a60,
  (int32_t)0x30fbc54d,(int32_t)0x89be50c3,
  (int32_t)0x2ea6f827,(int32_t)0x88cdf2f3,
  (int32_t)0x2c4d9050,(int32_t)0x87e958a7,
  (int32_t)0x29efc925,(int32_t)0x87109871,
  (int32_t)0x278dde6e,(int32_t)0x8643c7b3,
  (int32_t)0x25280c5e,(int32_t)0x8582faa5,
  (int32_t)0x22be8f87,(int32_t)0x84ce444e,
  (int32_t)0x2051a4dd,(int32_t)0x8425b683,
  (int32_t)0x1de189a6,(int32_t)0x838961e8,
  (int32_t)0x1b6e7b7a,(int32_t)0x82f955ea,
  (int32_t)0x18f8b83c,(int32_t)0x8275a0c0,
  (int32_t)0x16807e15,(int32_t)0x81fe4f6a,
  (int32_t)0x14060b68,(int32_t)0x81936daf,
  (int32_t)0x11899ed3,(int32_t)0x8135061b,
  (int32_t)0x0f0b7727,(int32_t)0x80e321ff,
  (int32_t)0x0c8bd35e,(int32_t)0x809dc971,
  (int32_t)0x0a0af299,(int32_t)0x80650347,
  (int32_t)0x07891418,(int32_t)0x8038d51e,
  (int32_t)0x05067734,(int32_t)0x80194350,
  (int32_t)0x02835b5a,(int32_t)0x800650fc
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr160_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr160_32x32,
    __fft_real_tw
};

const fft_handle_t rnfft32_320=  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_320 = (const fft_handle_t)&__rifft_descr;
