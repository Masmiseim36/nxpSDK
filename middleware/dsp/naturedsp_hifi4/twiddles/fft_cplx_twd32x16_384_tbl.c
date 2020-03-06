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

/* Twiddles tables for fft_cplx32x16, ifft_cplx32x16, fft_cplx16x16, ifft_cplx16x16, N=384 */
#define N 384

/****************** stage 1 radix 3 ******************/
ALIGN(8) static const int16_t __fft16_tw1[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7ffc,(int16_t)0xfde8,(int16_t)0x7fee,(int16_t)0xfbd0,
(int16_t)0x7fee,(int16_t)0xfbd0,(int16_t)0x7fba,(int16_t)0xf7a1,
(int16_t)0x7fd9,(int16_t)0xf9b8,(int16_t)0x7f62,(int16_t)0xf374,
(int16_t)0x7fba,(int16_t)0xf7a1,(int16_t)0x7ee8,(int16_t)0xef4b,
(int16_t)0x7f92,(int16_t)0xf58a,(int16_t)0x7e4a,(int16_t)0xeb26,
(int16_t)0x7f62,(int16_t)0xf374,(int16_t)0x7d8a,(int16_t)0xe707,
(int16_t)0x7f29,(int16_t)0xf15f,(int16_t)0x7ca8,(int16_t)0xe2ef,
(int16_t)0x7ee8,(int16_t)0xef4b,(int16_t)0x7ba3,(int16_t)0xdedf,
(int16_t)0x7e9d,(int16_t)0xed38,(int16_t)0x7a7d,(int16_t)0xdad8,
(int16_t)0x7e4a,(int16_t)0xeb26,(int16_t)0x7935,(int16_t)0xd6db,
(int16_t)0x7def,(int16_t)0xe916,(int16_t)0x77cc,(int16_t)0xd2e9,
(int16_t)0x7d8a,(int16_t)0xe707,(int16_t)0x7642,(int16_t)0xcf04,
(int16_t)0x7d1d,(int16_t)0xe4fa,(int16_t)0x7497,(int16_t)0xcb2c,
(int16_t)0x7ca8,(int16_t)0xe2ef,(int16_t)0x72cd,(int16_t)0xc763,
(int16_t)0x7c2a,(int16_t)0xe0e6,(int16_t)0x70e3,(int16_t)0xc3a9,
(int16_t)0x7ba3,(int16_t)0xdedf,(int16_t)0x6eda,(int16_t)0xc000,
(int16_t)0x7b14,(int16_t)0xdcda,(int16_t)0x6cb3,(int16_t)0xbc68,
(int16_t)0x7a7d,(int16_t)0xdad8,(int16_t)0x6a6e,(int16_t)0xb8e3,
(int16_t)0x79dd,(int16_t)0xd8d8,(int16_t)0x680b,(int16_t)0xb571,
(int16_t)0x7935,(int16_t)0xd6db,(int16_t)0x658d,(int16_t)0xb214,
(int16_t)0x7885,(int16_t)0xd4e1,(int16_t)0x62f2,(int16_t)0xaecc,
(int16_t)0x77cc,(int16_t)0xd2e9,(int16_t)0x603c,(int16_t)0xab9b,
(int16_t)0x770b,(int16_t)0xd0f5,(int16_t)0x5d6c,(int16_t)0xa880,
(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x5a82,(int16_t)0xa57e,
(int16_t)0x7570,(int16_t)0xcd17,(int16_t)0x5780,(int16_t)0xa294,
(int16_t)0x7497,(int16_t)0xcb2c,(int16_t)0x5465,(int16_t)0x9fc4,
(int16_t)0x73b6,(int16_t)0xc946,(int16_t)0x5134,(int16_t)0x9d0e,
(int16_t)0x72cd,(int16_t)0xc763,(int16_t)0x4dec,(int16_t)0x9a73,
(int16_t)0x71dc,(int16_t)0xc584,(int16_t)0x4a8f,(int16_t)0x97f5,
(int16_t)0x70e3,(int16_t)0xc3a9,(int16_t)0x471d,(int16_t)0x9592,
(int16_t)0x6fe2,(int16_t)0xc1d3,(int16_t)0x4398,(int16_t)0x934d,
(int16_t)0x6eda,(int16_t)0xc000,(int16_t)0x4000,(int16_t)0x9126,
(int16_t)0x6dca,(int16_t)0xbe32,(int16_t)0x3c57,(int16_t)0x8f1d,
(int16_t)0x6cb3,(int16_t)0xbc68,(int16_t)0x389d,(int16_t)0x8d33,
(int16_t)0x6b94,(int16_t)0xbaa3,(int16_t)0x34d4,(int16_t)0x8b69,
(int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x30fc,(int16_t)0x89be,
(int16_t)0x6940,(int16_t)0xb728,(int16_t)0x2d17,(int16_t)0x8834,
(int16_t)0x680b,(int16_t)0xb571,(int16_t)0x2925,(int16_t)0x86cb,
(int16_t)0x66d0,(int16_t)0xb3c0,(int16_t)0x2528,(int16_t)0x8583,
(int16_t)0x658d,(int16_t)0xb214,(int16_t)0x2121,(int16_t)0x845d,
(int16_t)0x6443,(int16_t)0xb06d,(int16_t)0x1d11,(int16_t)0x8358,
(int16_t)0x62f2,(int16_t)0xaecc,(int16_t)0x18f9,(int16_t)0x8276,
(int16_t)0x619a,(int16_t)0xad31,(int16_t)0x14da,(int16_t)0x81b6,
(int16_t)0x603c,(int16_t)0xab9b,(int16_t)0x10b5,(int16_t)0x8118,
(int16_t)0x5ed7,(int16_t)0xaa0a,(int16_t)0x0c8c,(int16_t)0x809e,
(int16_t)0x5d6c,(int16_t)0xa880,(int16_t)0x085f,(int16_t)0x8046,
(int16_t)0x5bfa,(int16_t)0xa6fc,(int16_t)0x0430,(int16_t)0x8012,
(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x0000,(int16_t)0x8000,
(int16_t)0x5904,(int16_t)0xa406,(int16_t)0xfbd0,(int16_t)0x8012,
(int16_t)0x5780,(int16_t)0xa294,(int16_t)0xf7a1,(int16_t)0x8046,
(int16_t)0x55f6,(int16_t)0xa129,(int16_t)0xf374,(int16_t)0x809e,
(int16_t)0x5465,(int16_t)0x9fc4,(int16_t)0xef4b,(int16_t)0x8118,
(int16_t)0x52cf,(int16_t)0x9e66,(int16_t)0xeb26,(int16_t)0x81b6,
(int16_t)0x5134,(int16_t)0x9d0e,(int16_t)0xe707,(int16_t)0x8276,
(int16_t)0x4f93,(int16_t)0x9bbd,(int16_t)0xe2ef,(int16_t)0x8358,
(int16_t)0x4dec,(int16_t)0x9a73,(int16_t)0xdedf,(int16_t)0x845d,
(int16_t)0x4c40,(int16_t)0x9930,(int16_t)0xdad8,(int16_t)0x8583,
(int16_t)0x4a8f,(int16_t)0x97f5,(int16_t)0xd6db,(int16_t)0x86cb,
(int16_t)0x48d8,(int16_t)0x96c0,(int16_t)0xd2e9,(int16_t)0x8834,
(int16_t)0x471d,(int16_t)0x9592,(int16_t)0xcf04,(int16_t)0x89be,
(int16_t)0x455d,(int16_t)0x946c,(int16_t)0xcb2c,(int16_t)0x8b69,
(int16_t)0x4398,(int16_t)0x934d,(int16_t)0xc763,(int16_t)0x8d33,
(int16_t)0x41ce,(int16_t)0x9236,(int16_t)0xc3a9,(int16_t)0x8f1d,
(int16_t)0x4000,(int16_t)0x9126,(int16_t)0xc000,(int16_t)0x9126,
(int16_t)0x3e2d,(int16_t)0x901e,(int16_t)0xbc68,(int16_t)0x934d,
(int16_t)0x3c57,(int16_t)0x8f1d,(int16_t)0xb8e3,(int16_t)0x9592,
(int16_t)0x3a7c,(int16_t)0x8e24,(int16_t)0xb571,(int16_t)0x97f5,
(int16_t)0x389d,(int16_t)0x8d33,(int16_t)0xb214,(int16_t)0x9a73,
(int16_t)0x36ba,(int16_t)0x8c4a,(int16_t)0xaecc,(int16_t)0x9d0e,
(int16_t)0x34d4,(int16_t)0x8b69,(int16_t)0xab9b,(int16_t)0x9fc4,
(int16_t)0x32e9,(int16_t)0x8a90,(int16_t)0xa880,(int16_t)0xa294,
(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0xa57e,
(int16_t)0x2f0b,(int16_t)0x88f5,(int16_t)0xa294,(int16_t)0xa880,
(int16_t)0x2d17,(int16_t)0x8834,(int16_t)0x9fc4,(int16_t)0xab9b,
(int16_t)0x2b1f,(int16_t)0x877b,(int16_t)0x9d0e,(int16_t)0xaecc,
(int16_t)0x2925,(int16_t)0x86cb,(int16_t)0x9a73,(int16_t)0xb214,
(int16_t)0x2728,(int16_t)0x8623,(int16_t)0x97f5,(int16_t)0xb571,
(int16_t)0x2528,(int16_t)0x8583,(int16_t)0x9592,(int16_t)0xb8e3,
(int16_t)0x2326,(int16_t)0x84ec,(int16_t)0x934d,(int16_t)0xbc68,
(int16_t)0x2121,(int16_t)0x845d,(int16_t)0x9126,(int16_t)0xc000,
(int16_t)0x1f1a,(int16_t)0x83d6,(int16_t)0x8f1d,(int16_t)0xc3a9,
(int16_t)0x1d11,(int16_t)0x8358,(int16_t)0x8d33,(int16_t)0xc763,
(int16_t)0x1b06,(int16_t)0x82e3,(int16_t)0x8b69,(int16_t)0xcb2c,
(int16_t)0x18f9,(int16_t)0x8276,(int16_t)0x89be,(int16_t)0xcf04,
(int16_t)0x16ea,(int16_t)0x8211,(int16_t)0x8834,(int16_t)0xd2e9,
(int16_t)0x14da,(int16_t)0x81b6,(int16_t)0x86cb,(int16_t)0xd6db,
(int16_t)0x12c8,(int16_t)0x8163,(int16_t)0x8583,(int16_t)0xdad8,
(int16_t)0x10b5,(int16_t)0x8118,(int16_t)0x845d,(int16_t)0xdedf,
(int16_t)0x0ea1,(int16_t)0x80d7,(int16_t)0x8358,(int16_t)0xe2ef,
(int16_t)0x0c8c,(int16_t)0x809e,(int16_t)0x8276,(int16_t)0xe707,
(int16_t)0x0a76,(int16_t)0x806e,(int16_t)0x81b6,(int16_t)0xeb26,
(int16_t)0x085f,(int16_t)0x8046,(int16_t)0x8118,(int16_t)0xef4b,
(int16_t)0x0648,(int16_t)0x8027,(int16_t)0x809e,(int16_t)0xf374,
(int16_t)0x0430,(int16_t)0x8012,(int16_t)0x8046,(int16_t)0xf7a1,
(int16_t)0x0218,(int16_t)0x8004,(int16_t)0x8012,(int16_t)0xfbd0,
(int16_t)0x0000,(int16_t)0x8000,(int16_t)0x8000,(int16_t)0x0000,
(int16_t)0xfde8,(int16_t)0x8004,(int16_t)0x8012,(int16_t)0x0430,
(int16_t)0xfbd0,(int16_t)0x8012,(int16_t)0x8046,(int16_t)0x085f,
(int16_t)0xf9b8,(int16_t)0x8027,(int16_t)0x809e,(int16_t)0x0c8c,
(int16_t)0xf7a1,(int16_t)0x8046,(int16_t)0x8118,(int16_t)0x10b5,
(int16_t)0xf58a,(int16_t)0x806e,(int16_t)0x81b6,(int16_t)0x14da,
(int16_t)0xf374,(int16_t)0x809e,(int16_t)0x8276,(int16_t)0x18f9,
(int16_t)0xf15f,(int16_t)0x80d7,(int16_t)0x8358,(int16_t)0x1d11,
(int16_t)0xef4b,(int16_t)0x8118,(int16_t)0x845d,(int16_t)0x2121,
(int16_t)0xed38,(int16_t)0x8163,(int16_t)0x8583,(int16_t)0x2528,
(int16_t)0xeb26,(int16_t)0x81b6,(int16_t)0x86cb,(int16_t)0x2925,
(int16_t)0xe916,(int16_t)0x8211,(int16_t)0x8834,(int16_t)0x2d17,
(int16_t)0xe707,(int16_t)0x8276,(int16_t)0x89be,(int16_t)0x30fc,
(int16_t)0xe4fa,(int16_t)0x82e3,(int16_t)0x8b69,(int16_t)0x34d4,
(int16_t)0xe2ef,(int16_t)0x8358,(int16_t)0x8d33,(int16_t)0x389d,
(int16_t)0xe0e6,(int16_t)0x83d6,(int16_t)0x8f1d,(int16_t)0x3c57,
(int16_t)0xdedf,(int16_t)0x845d,(int16_t)0x9126,(int16_t)0x4000,
(int16_t)0xdcda,(int16_t)0x84ec,(int16_t)0x934d,(int16_t)0x4398,
(int16_t)0xdad8,(int16_t)0x8583,(int16_t)0x9592,(int16_t)0x471d,
(int16_t)0xd8d8,(int16_t)0x8623,(int16_t)0x97f5,(int16_t)0x4a8f,
(int16_t)0xd6db,(int16_t)0x86cb,(int16_t)0x9a73,(int16_t)0x4dec,
(int16_t)0xd4e1,(int16_t)0x877b,(int16_t)0x9d0e,(int16_t)0x5134,
(int16_t)0xd2e9,(int16_t)0x8834,(int16_t)0x9fc4,(int16_t)0x5465,
(int16_t)0xd0f5,(int16_t)0x88f5,(int16_t)0xa294,(int16_t)0x5780,
(int16_t)0xcf04,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0x5a82,
(int16_t)0xcd17,(int16_t)0x8a90,(int16_t)0xa880,(int16_t)0x5d6c,
(int16_t)0xcb2c,(int16_t)0x8b69,(int16_t)0xab9b,(int16_t)0x603c,
(int16_t)0xc946,(int16_t)0x8c4a,(int16_t)0xaecc,(int16_t)0x62f2,
(int16_t)0xc763,(int16_t)0x8d33,(int16_t)0xb214,(int16_t)0x658d,
(int16_t)0xc584,(int16_t)0x8e24,(int16_t)0xb571,(int16_t)0x680b,
(int16_t)0xc3a9,(int16_t)0x8f1d,(int16_t)0xb8e3,(int16_t)0x6a6e,
(int16_t)0xc1d3,(int16_t)0x901e,(int16_t)0xbc68,(int16_t)0x6cb3
};
/****************** stage 2 radix 4 ******************/
ALIGN(8) static const int16_t __fft16_tw2[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7fd9,(int16_t)0xf9b8,(int16_t)0x7f62,(int16_t)0xf374,(int16_t)0x7e9d,(int16_t)0xed38,
(int16_t)0x7f62,(int16_t)0xf374,(int16_t)0x7d8a,(int16_t)0xe707,(int16_t)0x7a7d,(int16_t)0xdad8,
(int16_t)0x7e9d,(int16_t)0xed38,(int16_t)0x7a7d,(int16_t)0xdad8,(int16_t)0x73b6,(int16_t)0xc946,
(int16_t)0x7d8a,(int16_t)0xe707,(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x6a6e,(int16_t)0xb8e3,
(int16_t)0x7c2a,(int16_t)0xe0e6,(int16_t)0x70e3,(int16_t)0xc3a9,(int16_t)0x5ed7,(int16_t)0xaa0a,
(int16_t)0x7a7d,(int16_t)0xdad8,(int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x5134,(int16_t)0x9d0e,
(int16_t)0x7885,(int16_t)0xd4e1,(int16_t)0x62f2,(int16_t)0xaecc,(int16_t)0x41ce,(int16_t)0x9236,
(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x30fc,(int16_t)0x89be,
(int16_t)0x73b6,(int16_t)0xc946,(int16_t)0x5134,(int16_t)0x9d0e,(int16_t)0x1f1a,(int16_t)0x83d6,
(int16_t)0x70e3,(int16_t)0xc3a9,(int16_t)0x471d,(int16_t)0x9592,(int16_t)0x0c8c,(int16_t)0x809e,
(int16_t)0x6dca,(int16_t)0xbe32,(int16_t)0x3c57,(int16_t)0x8f1d,(int16_t)0xf9b8,(int16_t)0x8027,
(int16_t)0x6a6e,(int16_t)0xb8e3,(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xe707,(int16_t)0x8276,
(int16_t)0x66d0,(int16_t)0xb3c0,(int16_t)0x2528,(int16_t)0x8583,(int16_t)0xd4e1,(int16_t)0x877b,
(int16_t)0x62f2,(int16_t)0xaecc,(int16_t)0x18f9,(int16_t)0x8276,(int16_t)0xc3a9,(int16_t)0x8f1d,
(int16_t)0x5ed7,(int16_t)0xaa0a,(int16_t)0x0c8c,(int16_t)0x809e,(int16_t)0xb3c0,(int16_t)0x9930,
(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xa57e,
(int16_t)0x55f6,(int16_t)0xa129,(int16_t)0xf374,(int16_t)0x809e,(int16_t)0x9930,(int16_t)0xb3c0,
(int16_t)0x5134,(int16_t)0x9d0e,(int16_t)0xe707,(int16_t)0x8276,(int16_t)0x8f1d,(int16_t)0xc3a9,
(int16_t)0x4c40,(int16_t)0x9930,(int16_t)0xdad8,(int16_t)0x8583,(int16_t)0x877b,(int16_t)0xd4e1,
(int16_t)0x471d,(int16_t)0x9592,(int16_t)0xcf04,(int16_t)0x89be,(int16_t)0x8276,(int16_t)0xe707,
(int16_t)0x41ce,(int16_t)0x9236,(int16_t)0xc3a9,(int16_t)0x8f1d,(int16_t)0x8027,(int16_t)0xf9b8,
(int16_t)0x3c57,(int16_t)0x8f1d,(int16_t)0xb8e3,(int16_t)0x9592,(int16_t)0x809e,(int16_t)0x0c8c,
(int16_t)0x36ba,(int16_t)0x8c4a,(int16_t)0xaecc,(int16_t)0x9d0e,(int16_t)0x83d6,(int16_t)0x1f1a,
(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0xa57e,(int16_t)0x89be,(int16_t)0x30fc,
(int16_t)0x2b1f,(int16_t)0x877b,(int16_t)0x9d0e,(int16_t)0xaecc,(int16_t)0x9236,(int16_t)0x41ce,
(int16_t)0x2528,(int16_t)0x8583,(int16_t)0x9592,(int16_t)0xb8e3,(int16_t)0x9d0e,(int16_t)0x5134,
(int16_t)0x1f1a,(int16_t)0x83d6,(int16_t)0x8f1d,(int16_t)0xc3a9,(int16_t)0xaa0a,(int16_t)0x5ed7,
(int16_t)0x18f9,(int16_t)0x8276,(int16_t)0x89be,(int16_t)0xcf04,(int16_t)0xb8e3,(int16_t)0x6a6e,
(int16_t)0x12c8,(int16_t)0x8163,(int16_t)0x8583,(int16_t)0xdad8,(int16_t)0xc946,(int16_t)0x73b6,
(int16_t)0x0c8c,(int16_t)0x809e,(int16_t)0x8276,(int16_t)0xe707,(int16_t)0xdad8,(int16_t)0x7a7d,
(int16_t)0x0648,(int16_t)0x8027,(int16_t)0x809e,(int16_t)0xf374,(int16_t)0xed38,(int16_t)0x7e9d
};
/****************** stage 3 radix 4 ******************/
ALIGN(8) static const int16_t __fft16_tw3[] = 
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
    1, 1, 1, 0
}; 
static const cint16ptr_t tw_tab[] = 
{
    __fft16_tw1, __fft16_tw2, __fft16_tw3, NULL
};

static const eFft_stage_type fft_cplx32x16_scl2_tab[] = 
{
    fft_stage_first_DFT3,
    fft_stage_inner_DFT4,
    fft_stage_inner_DFT4,
    fft_stage_last_DFT8
};
static const eFft_stage_type fft_cplx32x16_scl3_tab[] =
{
    fft_stage_first_DFT3,
    fft_stage_inner_DFT4,
    fft_stage_inner_DFT4,
    fft_stage_last_DFT8
};
static const eFft_stage_type ifft_cplx32x16_scl2_tab[] = 
{
    fft_stage_first_inv_DFT3,
    fft_stage_inner_DFT4,
    fft_stage_inner_DFT4,
    fft_stage_last_DFT8
};
static const eFft_stage_type ifft_cplx32x16_scl3_tab[] =
{
    fft_stage_first_inv_DFT3,
    fft_stage_inner_DFT4,
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
const fft_handle_t cnfft32x16_384 = (const fft_handle_t)&__cfft_descr;
const fft_handle_t cinfft32x16_384 = (const fft_handle_t)&__cifft_descr;
