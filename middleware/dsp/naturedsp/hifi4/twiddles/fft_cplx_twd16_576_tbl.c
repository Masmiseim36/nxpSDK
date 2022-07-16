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

/* Twiddles tables for fft_cplx32x16, ifft_cplx32x16, fft_cplx16x16, ifft_cplx16x16, N=576 */
#define N 576

/********** Twiddles table N=576 stage 1 radix 4 ******************/
ALIGN(8) static const int16_t _fft576_tw1_[] =
{
    (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFE, (int16_t)0xFE9B,
    (int16_t)0x7FF8, (int16_t)0xFD35, (int16_t)0x7FEE, (int16_t)0xFBD0, (int16_t)0x7FF8, (int16_t)0xFD35, (int16_t)0x7FE1, (int16_t)0xFA6B,
    (int16_t)0x7FBA, (int16_t)0xF7A1, (int16_t)0x7FEE, (int16_t)0xFBD0, (int16_t)0x7FBA, (int16_t)0xF7A1, (int16_t)0x7F62, (int16_t)0xF374,
    (int16_t)0x7FE1, (int16_t)0xFA6B, (int16_t)0x7F83, (int16_t)0xF4D8, (int16_t)0x7EE8, (int16_t)0xEF4B, (int16_t)0x7FCF, (int16_t)0xF906,
    (int16_t)0x7F3D, (int16_t)0xF211, (int16_t)0x7E4A, (int16_t)0xEB26, (int16_t)0x7FBA, (int16_t)0xF7A1, (int16_t)0x7EE8, (int16_t)0xEF4B,
    (int16_t)0x7D8A, (int16_t)0xE707, (int16_t)0x7FA1, (int16_t)0xF63C, (int16_t)0x7E83, (int16_t)0xEC87, (int16_t)0x7CA8, (int16_t)0xE2EF,
    (int16_t)0x7F83, (int16_t)0xF4D8, (int16_t)0x7E0E, (int16_t)0xE9C6, (int16_t)0x7BA3, (int16_t)0xDEDF, (int16_t)0x7F62, (int16_t)0xF374,
    (int16_t)0x7D8A, (int16_t)0xE707, (int16_t)0x7A7D, (int16_t)0xDAD8, (int16_t)0x7F3D, (int16_t)0xF211, (int16_t)0x7CF7, (int16_t)0xE44C,
    (int16_t)0x7935, (int16_t)0xD6DB, (int16_t)0x7F14, (int16_t)0xF0AE, (int16_t)0x7C55, (int16_t)0xE194, (int16_t)0x77CC, (int16_t)0xD2E9,
    (int16_t)0x7EE8, (int16_t)0xEF4B, (int16_t)0x7BA3, (int16_t)0xDEDF, (int16_t)0x7642, (int16_t)0xCF04, (int16_t)0x7EB7, (int16_t)0xEDE9,
    (int16_t)0x7AE3, (int16_t)0xDC2F, (int16_t)0x7497, (int16_t)0xCB2C, (int16_t)0x7E83, (int16_t)0xEC87, (int16_t)0x7A13, (int16_t)0xD982,
    (int16_t)0x72CD, (int16_t)0xC763, (int16_t)0x7E4A, (int16_t)0xEB26, (int16_t)0x7935, (int16_t)0xD6DB, (int16_t)0x70E3, (int16_t)0xC3A9,
    (int16_t)0x7E0E, (int16_t)0xE9C6, (int16_t)0x7848, (int16_t)0xD439, (int16_t)0x6EDA, (int16_t)0xC000, (int16_t)0x7DCE, (int16_t)0xE866,
    (int16_t)0x774C, (int16_t)0xD19C, (int16_t)0x6CB3, (int16_t)0xBC68, (int16_t)0x7D8A, (int16_t)0xE707, (int16_t)0x7642, (int16_t)0xCF04,
    (int16_t)0x6A6E, (int16_t)0xB8E3, (int16_t)0x7D43, (int16_t)0xE5A9, (int16_t)0x7529, (int16_t)0xCC73, (int16_t)0x680B, (int16_t)0xB571,
    (int16_t)0x7CF7, (int16_t)0xE44C, (int16_t)0x7402, (int16_t)0xC9E8, (int16_t)0x658D, (int16_t)0xB214, (int16_t)0x7CA8, (int16_t)0xE2EF,
    (int16_t)0x72CD, (int16_t)0xC763, (int16_t)0x62F2, (int16_t)0xAECC, (int16_t)0x7C55, (int16_t)0xE194, (int16_t)0x718A, (int16_t)0xC4E5,
    (int16_t)0x603C, (int16_t)0xAB9B, (int16_t)0x7BFE, (int16_t)0xE039, (int16_t)0x7039, (int16_t)0xC26F, (int16_t)0x5D6C, (int16_t)0xA880,
    (int16_t)0x7BA3, (int16_t)0xDEDF, (int16_t)0x6EDA, (int16_t)0xC000, (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x7B45, (int16_t)0xDD86,
    (int16_t)0x6D6E, (int16_t)0xBD99, (int16_t)0x5780, (int16_t)0xA294, (int16_t)0x7AE3, (int16_t)0xDC2F, (int16_t)0x6BF4, (int16_t)0xBB3A,
    (int16_t)0x5465, (int16_t)0x9FC4, (int16_t)0x7A7D, (int16_t)0xDAD8, (int16_t)0x6A6E, (int16_t)0xB8E3, (int16_t)0x5134, (int16_t)0x9D0E,
    (int16_t)0x7A13, (int16_t)0xD982, (int16_t)0x68DA, (int16_t)0xB695, (int16_t)0x4DEC, (int16_t)0x9A73, (int16_t)0x79A6, (int16_t)0xD82E,
    (int16_t)0x673A, (int16_t)0xB450, (int16_t)0x4A8F, (int16_t)0x97F5, (int16_t)0x7935, (int16_t)0xD6DB, (int16_t)0x658D, (int16_t)0xB214,
    (int16_t)0x471D, (int16_t)0x9592, (int16_t)0x78C0, (int16_t)0xD589, (int16_t)0x63D3, (int16_t)0xAFE2, (int16_t)0x4398, (int16_t)0x934D,
    (int16_t)0x7848, (int16_t)0xD439, (int16_t)0x620E, (int16_t)0xADB9, (int16_t)0x4000, (int16_t)0x9126, (int16_t)0x77CC, (int16_t)0xD2E9,
    (int16_t)0x603C, (int16_t)0xAB9B, (int16_t)0x3C57, (int16_t)0x8F1D, (int16_t)0x774C, (int16_t)0xD19C, (int16_t)0x5E5F, (int16_t)0xA986,
    (int16_t)0x389D, (int16_t)0x8D33, (int16_t)0x76C9, (int16_t)0xD04F, (int16_t)0x5C76, (int16_t)0xA77D, (int16_t)0x34D4, (int16_t)0x8B69,
    (int16_t)0x7642, (int16_t)0xCF04, (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0x75B7, (int16_t)0xCDBB,
    (int16_t)0x5883, (int16_t)0xA38A, (int16_t)0x2D17, (int16_t)0x8834, (int16_t)0x7529, (int16_t)0xCC73, (int16_t)0x567A, (int16_t)0xA1A1,
    (int16_t)0x2925, (int16_t)0x86CB, (int16_t)0x7497, (int16_t)0xCB2C, (int16_t)0x5465, (int16_t)0x9FC4, (int16_t)0x2528, (int16_t)0x8583,
    (int16_t)0x7402, (int16_t)0xC9E8, (int16_t)0x5247, (int16_t)0x9DF2, (int16_t)0x2121, (int16_t)0x845D, (int16_t)0x7369, (int16_t)0xC8A5,
    (int16_t)0x501E, (int16_t)0x9C2D, (int16_t)0x1D11, (int16_t)0x8358, (int16_t)0x72CD, (int16_t)0xC763, (int16_t)0x4DEC, (int16_t)0x9A73,
    (int16_t)0x18F9, (int16_t)0x8276, (int16_t)0x722D, (int16_t)0xC623, (int16_t)0x4BB0, (int16_t)0x98C6, (int16_t)0x14DA, (int16_t)0x81B6,
    (int16_t)0x718A, (int16_t)0xC4E5, (int16_t)0x496B, (int16_t)0x9726, (int16_t)0x10B5, (int16_t)0x8118, (int16_t)0x70E3, (int16_t)0xC3A9,
    (int16_t)0x471D, (int16_t)0x9592, (int16_t)0x0C8C, (int16_t)0x809E, (int16_t)0x7039, (int16_t)0xC26F, (int16_t)0x44C6, (int16_t)0x940C,
    (int16_t)0x085F, (int16_t)0x8046, (int16_t)0x6F8B, (int16_t)0xC137, (int16_t)0x4267, (int16_t)0x9292, (int16_t)0x0430, (int16_t)0x8012,
    (int16_t)0x6EDA, (int16_t)0xC000, (int16_t)0x4000, (int16_t)0x9126, (int16_t)0x0000, (int16_t)0x8000, (int16_t)0x6E26, (int16_t)0xBECB,
    (int16_t)0x3D91, (int16_t)0x8FC7, (int16_t)0xFBD0, (int16_t)0x8012, (int16_t)0x6D6E, (int16_t)0xBD99, (int16_t)0x3B1B, (int16_t)0x8E76,
    (int16_t)0xF7A1, (int16_t)0x8046, (int16_t)0x6CB3, (int16_t)0xBC68, (int16_t)0x389D, (int16_t)0x8D33, (int16_t)0xF374, (int16_t)0x809E,
    (int16_t)0x6BF4, (int16_t)0xBB3A, (int16_t)0x3618, (int16_t)0x8BFE, (int16_t)0xEF4B, (int16_t)0x8118, (int16_t)0x6B33, (int16_t)0xBA0D,
    (int16_t)0x338D, (int16_t)0x8AD7, (int16_t)0xEB26, (int16_t)0x81B6, (int16_t)0x6A6E, (int16_t)0xB8E3, (int16_t)0x30FC, (int16_t)0x89BE,
    (int16_t)0xE707, (int16_t)0x8276, (int16_t)0x69A5, (int16_t)0xB7BB, (int16_t)0x2E64, (int16_t)0x88B4, (int16_t)0xE2EF, (int16_t)0x8358,
    (int16_t)0x68DA, (int16_t)0xB695, (int16_t)0x2BC7, (int16_t)0x87B8, (int16_t)0xDEDF, (int16_t)0x845D, (int16_t)0x680B, (int16_t)0xB571,
    (int16_t)0x2925, (int16_t)0x86CB, (int16_t)0xDAD8, (int16_t)0x8583, (int16_t)0x673A, (int16_t)0xB450, (int16_t)0x267E, (int16_t)0x85ED,
    (int16_t)0xD6DB, (int16_t)0x86CB, (int16_t)0x6665, (int16_t)0xB331, (int16_t)0x23D1, (int16_t)0x851D, (int16_t)0xD2E9, (int16_t)0x8834,
    (int16_t)0x658D, (int16_t)0xB214, (int16_t)0x2121, (int16_t)0x845D, (int16_t)0xCF04, (int16_t)0x89BE, (int16_t)0x64B1, (int16_t)0xB0FA,
    (int16_t)0x1E6C, (int16_t)0x83AB, (int16_t)0xCB2C, (int16_t)0x8B69, (int16_t)0x63D3, (int16_t)0xAFE2, (int16_t)0x1BB4, (int16_t)0x8309,
    (int16_t)0xC763, (int16_t)0x8D33, (int16_t)0x62F2, (int16_t)0xAECC, (int16_t)0x18F9, (int16_t)0x8276, (int16_t)0xC3A9, (int16_t)0x8F1D,
    (int16_t)0x620E, (int16_t)0xADB9, (int16_t)0x163A, (int16_t)0x81F2, (int16_t)0xC000, (int16_t)0x9126, (int16_t)0x6126, (int16_t)0xACA9,
    (int16_t)0x1379, (int16_t)0x817D, (int16_t)0xBC68, (int16_t)0x934D, (int16_t)0x603C, (int16_t)0xAB9B, (int16_t)0x10B5, (int16_t)0x8118,
    (int16_t)0xB8E3, (int16_t)0x9592, (int16_t)0x5F4F, (int16_t)0xAA8F, (int16_t)0x0DEF, (int16_t)0x80C3, (int16_t)0xB571, (int16_t)0x97F5,
    (int16_t)0x5E5F, (int16_t)0xA986, (int16_t)0x0B28, (int16_t)0x807D, (int16_t)0xB214, (int16_t)0x9A73, (int16_t)0x5D6C, (int16_t)0xA880,
    (int16_t)0x085F, (int16_t)0x8046, (int16_t)0xAECC, (int16_t)0x9D0E, (int16_t)0x5C76, (int16_t)0xA77D, (int16_t)0x0595, (int16_t)0x801F,
    (int16_t)0xAB9B, (int16_t)0x9FC4, (int16_t)0x5B7E, (int16_t)0xA67C, (int16_t)0x02CB, (int16_t)0x8008, (int16_t)0xA880, (int16_t)0xA294,
    (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x0000, (int16_t)0x8000, (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0x5984, (int16_t)0xA482,
    (int16_t)0xFD35, (int16_t)0x8008, (int16_t)0xA294, (int16_t)0xA880, (int16_t)0x5883, (int16_t)0xA38A, (int16_t)0xFA6B, (int16_t)0x801F,
    (int16_t)0x9FC4, (int16_t)0xAB9B, (int16_t)0x5780, (int16_t)0xA294, (int16_t)0xF7A1, (int16_t)0x8046, (int16_t)0x9D0E, (int16_t)0xAECC,
    (int16_t)0x567A, (int16_t)0xA1A1, (int16_t)0xF4D8, (int16_t)0x807D, (int16_t)0x9A73, (int16_t)0xB214, (int16_t)0x5571, (int16_t)0xA0B1,
    (int16_t)0xF211, (int16_t)0x80C3, (int16_t)0x97F5, (int16_t)0xB571, (int16_t)0x5465, (int16_t)0x9FC4, (int16_t)0xEF4B, (int16_t)0x8118,
    (int16_t)0x9592, (int16_t)0xB8E3, (int16_t)0x5357, (int16_t)0x9EDA, (int16_t)0xEC87, (int16_t)0x817D, (int16_t)0x934D, (int16_t)0xBC68,
    (int16_t)0x5247, (int16_t)0x9DF2, (int16_t)0xE9C6, (int16_t)0x81F2, (int16_t)0x9126, (int16_t)0xC000, (int16_t)0x5134, (int16_t)0x9D0E,
    (int16_t)0xE707, (int16_t)0x8276, (int16_t)0x8F1D, (int16_t)0xC3A9, (int16_t)0x501E, (int16_t)0x9C2D, (int16_t)0xE44C, (int16_t)0x8309,
    (int16_t)0x8D33, (int16_t)0xC763, (int16_t)0x4F06, (int16_t)0x9B4F, (int16_t)0xE194, (int16_t)0x83AB, (int16_t)0x8B69, (int16_t)0xCB2C,
    (int16_t)0x4DEC, (int16_t)0x9A73, (int16_t)0xDEDF, (int16_t)0x845D, (int16_t)0x89BE, (int16_t)0xCF04, (int16_t)0x4CCF, (int16_t)0x999B,
    (int16_t)0xDC2F, (int16_t)0x851D, (int16_t)0x8834, (int16_t)0xD2E9, (int16_t)0x4BB0, (int16_t)0x98C6, (int16_t)0xD982, (int16_t)0x85ED,
    (int16_t)0x86CB, (int16_t)0xD6DB, (int16_t)0x4A8F, (int16_t)0x97F5, (int16_t)0xD6DB, (int16_t)0x86CB, (int16_t)0x8583, (int16_t)0xDAD8,
    (int16_t)0x496B, (int16_t)0x9726, (int16_t)0xD439, (int16_t)0x87B8, (int16_t)0x845D, (int16_t)0xDEDF, (int16_t)0x4845, (int16_t)0x965B,
    (int16_t)0xD19C, (int16_t)0x88B4, (int16_t)0x8358, (int16_t)0xE2EF, (int16_t)0x471D, (int16_t)0x9592, (int16_t)0xCF04, (int16_t)0x89BE,
    (int16_t)0x8276, (int16_t)0xE707, (int16_t)0x45F3, (int16_t)0x94CD, (int16_t)0xCC73, (int16_t)0x8AD7, (int16_t)0x81B6, (int16_t)0xEB26,
    (int16_t)0x44C6, (int16_t)0x940C, (int16_t)0xC9E8, (int16_t)0x8BFE, (int16_t)0x8118, (int16_t)0xEF4B, (int16_t)0x4398, (int16_t)0x934D,
    (int16_t)0xC763, (int16_t)0x8D33, (int16_t)0x809E, (int16_t)0xF374, (int16_t)0x4267, (int16_t)0x9292, (int16_t)0xC4E5, (int16_t)0x8E76,
    (int16_t)0x8046, (int16_t)0xF7A1, (int16_t)0x4135, (int16_t)0x91DA, (int16_t)0xC26F, (int16_t)0x8FC7, (int16_t)0x8012, (int16_t)0xFBD0,
    (int16_t)0x4000, (int16_t)0x9126, (int16_t)0xC000, (int16_t)0x9126, (int16_t)0x8000, (int16_t)0x0000, (int16_t)0x3EC9, (int16_t)0x9075,
    (int16_t)0xBD99, (int16_t)0x9292, (int16_t)0x8012, (int16_t)0x0430, (int16_t)0x3D91, (int16_t)0x8FC7, (int16_t)0xBB3A, (int16_t)0x940C,
    (int16_t)0x8046, (int16_t)0x085F, (int16_t)0x3C57, (int16_t)0x8F1D, (int16_t)0xB8E3, (int16_t)0x9592, (int16_t)0x809E, (int16_t)0x0C8C,
    (int16_t)0x3B1B, (int16_t)0x8E76, (int16_t)0xB695, (int16_t)0x9726, (int16_t)0x8118, (int16_t)0x10B5, (int16_t)0x39DD, (int16_t)0x8DD3,
    (int16_t)0xB450, (int16_t)0x98C6, (int16_t)0x81B6, (int16_t)0x14DA, (int16_t)0x389D, (int16_t)0x8D33, (int16_t)0xB214, (int16_t)0x9A73,
    (int16_t)0x8276, (int16_t)0x18F9, (int16_t)0x375B, (int16_t)0x8C97, (int16_t)0xAFE2, (int16_t)0x9C2D, (int16_t)0x8358, (int16_t)0x1D11,
    (int16_t)0x3618, (int16_t)0x8BFE, (int16_t)0xADB9, (int16_t)0x9DF2, (int16_t)0x845D, (int16_t)0x2121, (int16_t)0x34D4, (int16_t)0x8B69,
    (int16_t)0xAB9B, (int16_t)0x9FC4, (int16_t)0x8583, (int16_t)0x2528, (int16_t)0x338D, (int16_t)0x8AD7, (int16_t)0xA986, (int16_t)0xA1A1,
    (int16_t)0x86CB, (int16_t)0x2925, (int16_t)0x3245, (int16_t)0x8A49, (int16_t)0xA77D, (int16_t)0xA38A, (int16_t)0x8834, (int16_t)0x2D17,
    (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0x89BE, (int16_t)0x30FC, (int16_t)0x2FB1, (int16_t)0x8937,
    (int16_t)0xA38A, (int16_t)0xA77D, (int16_t)0x8B69, (int16_t)0x34D4, (int16_t)0x2E64, (int16_t)0x88B4, (int16_t)0xA1A1, (int16_t)0xA986,
    (int16_t)0x8D33, (int16_t)0x389D, (int16_t)0x2D17, (int16_t)0x8834, (int16_t)0x9FC4, (int16_t)0xAB9B, (int16_t)0x8F1D, (int16_t)0x3C57,
    (int16_t)0x2BC7, (int16_t)0x87B8, (int16_t)0x9DF2, (int16_t)0xADB9, (int16_t)0x9126, (int16_t)0x4000, (int16_t)0x2A77, (int16_t)0x8740,
    (int16_t)0x9C2D, (int16_t)0xAFE2, (int16_t)0x934D, (int16_t)0x4398, (int16_t)0x2925, (int16_t)0x86CB, (int16_t)0x9A73, (int16_t)0xB214,
    (int16_t)0x9592, (int16_t)0x471D, (int16_t)0x27D2, (int16_t)0x865A, (int16_t)0x98C6, (int16_t)0xB450, (int16_t)0x97F5, (int16_t)0x4A8F,
    (int16_t)0x267E, (int16_t)0x85ED, (int16_t)0x9726, (int16_t)0xB695, (int16_t)0x9A73, (int16_t)0x4DEC, (int16_t)0x2528, (int16_t)0x8583,
    (int16_t)0x9592, (int16_t)0xB8E3, (int16_t)0x9D0E, (int16_t)0x5134, (int16_t)0x23D1, (int16_t)0x851D, (int16_t)0x940C, (int16_t)0xBB3A,
    (int16_t)0x9FC4, (int16_t)0x5465, (int16_t)0x227A, (int16_t)0x84BB, (int16_t)0x9292, (int16_t)0xBD99, (int16_t)0xA294, (int16_t)0x5780,
    (int16_t)0x2121, (int16_t)0x845D, (int16_t)0x9126, (int16_t)0xC000, (int16_t)0xA57E, (int16_t)0x5A82, (int16_t)0x1FC7, (int16_t)0x8402,
    (int16_t)0x8FC7, (int16_t)0xC26F, (int16_t)0xA880, (int16_t)0x5D6C, (int16_t)0x1E6C, (int16_t)0x83AB, (int16_t)0x8E76, (int16_t)0xC4E5,
    (int16_t)0xAB9B, (int16_t)0x603C, (int16_t)0x1D11, (int16_t)0x8358, (int16_t)0x8D33, (int16_t)0xC763, (int16_t)0xAECC, (int16_t)0x62F2,
    (int16_t)0x1BB4, (int16_t)0x8309, (int16_t)0x8BFE, (int16_t)0xC9E8, (int16_t)0xB214, (int16_t)0x658D, (int16_t)0x1A57, (int16_t)0x82BD,
    (int16_t)0x8AD7, (int16_t)0xCC73, (int16_t)0xB571, (int16_t)0x680B, (int16_t)0x18F9, (int16_t)0x8276, (int16_t)0x89BE, (int16_t)0xCF04,
    (int16_t)0xB8E3, (int16_t)0x6A6E, (int16_t)0x179A, (int16_t)0x8232, (int16_t)0x88B4, (int16_t)0xD19C, (int16_t)0xBC68, (int16_t)0x6CB3,
    (int16_t)0x163A, (int16_t)0x81F2, (int16_t)0x87B8, (int16_t)0xD439, (int16_t)0xC000, (int16_t)0x6EDA, (int16_t)0x14DA, (int16_t)0x81B6,
    (int16_t)0x86CB, (int16_t)0xD6DB, (int16_t)0xC3A9, (int16_t)0x70E3, (int16_t)0x1379, (int16_t)0x817D, (int16_t)0x85ED, (int16_t)0xD982,
    (int16_t)0xC763, (int16_t)0x72CD, (int16_t)0x1217, (int16_t)0x8149, (int16_t)0x851D, (int16_t)0xDC2F, (int16_t)0xCB2C, (int16_t)0x7497,
    (int16_t)0x10B5, (int16_t)0x8118, (int16_t)0x845D, (int16_t)0xDEDF, (int16_t)0xCF04, (int16_t)0x7642, (int16_t)0x0F52, (int16_t)0x80EC,
    (int16_t)0x83AB, (int16_t)0xE194, (int16_t)0xD2E9, (int16_t)0x77CC, (int16_t)0x0DEF, (int16_t)0x80C3, (int16_t)0x8309, (int16_t)0xE44C,
    (int16_t)0xD6DB, (int16_t)0x7935, (int16_t)0x0C8C, (int16_t)0x809E, (int16_t)0x8276, (int16_t)0xE707, (int16_t)0xDAD8, (int16_t)0x7A7D,
    (int16_t)0x0B28, (int16_t)0x807D, (int16_t)0x81F2, (int16_t)0xE9C6, (int16_t)0xDEDF, (int16_t)0x7BA3, (int16_t)0x09C4, (int16_t)0x805F,
    (int16_t)0x817D, (int16_t)0xEC87, (int16_t)0xE2EF, (int16_t)0x7CA8, (int16_t)0x085F, (int16_t)0x8046, (int16_t)0x8118, (int16_t)0xEF4B,
    (int16_t)0xE707, (int16_t)0x7D8A, (int16_t)0x06FA, (int16_t)0x8031, (int16_t)0x80C3, (int16_t)0xF211, (int16_t)0xEB26, (int16_t)0x7E4A,
    (int16_t)0x0595, (int16_t)0x801F, (int16_t)0x807D, (int16_t)0xF4D8, (int16_t)0xEF4B, (int16_t)0x7EE8, (int16_t)0x0430, (int16_t)0x8012,
    (int16_t)0x8046, (int16_t)0xF7A1, (int16_t)0xF374, (int16_t)0x7F62, (int16_t)0x02CB, (int16_t)0x8008, (int16_t)0x801F, (int16_t)0xFA6B,
    (int16_t)0xF7A1, (int16_t)0x7FBA, (int16_t)0x0165, (int16_t)0x8002, (int16_t)0x8008, (int16_t)0xFD35, (int16_t)0xFBD0, (int16_t)0x7FEE,
};

/********** Twiddles table N=576 stage 2 radix 4 ******************/
ALIGN(8) static const int16_t _fft576_tw2_[] =
{
    (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FE1, (int16_t)0xFA6B,
    (int16_t)0x7F83, (int16_t)0xF4D8, (int16_t)0x7EE8, (int16_t)0xEF4B, (int16_t)0x7F83, (int16_t)0xF4D8, (int16_t)0x7E0E, (int16_t)0xE9C6,
    (int16_t)0x7BA3, (int16_t)0xDEDF, (int16_t)0x7EE8, (int16_t)0xEF4B, (int16_t)0x7BA3, (int16_t)0xDEDF, (int16_t)0x7642, (int16_t)0xCF04,
    (int16_t)0x7E0E, (int16_t)0xE9C6, (int16_t)0x7848, (int16_t)0xD439, (int16_t)0x6EDA, (int16_t)0xC000, (int16_t)0x7CF7, (int16_t)0xE44C,
    (int16_t)0x7402, (int16_t)0xC9E8, (int16_t)0x658D, (int16_t)0xB214, (int16_t)0x7BA3, (int16_t)0xDEDF, (int16_t)0x6EDA, (int16_t)0xC000,
    (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x7A13, (int16_t)0xD982, (int16_t)0x68DA, (int16_t)0xB695, (int16_t)0x4DEC, (int16_t)0x9A73,
    (int16_t)0x7848, (int16_t)0xD439, (int16_t)0x620E, (int16_t)0xADB9, (int16_t)0x4000, (int16_t)0x9126, (int16_t)0x7642, (int16_t)0xCF04,
    (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0x7402, (int16_t)0xC9E8, (int16_t)0x5247, (int16_t)0x9DF2,
    (int16_t)0x2121, (int16_t)0x845D, (int16_t)0x718A, (int16_t)0xC4E5, (int16_t)0x496B, (int16_t)0x9726, (int16_t)0x10B5, (int16_t)0x8118,
    (int16_t)0x6EDA, (int16_t)0xC000, (int16_t)0x4000, (int16_t)0x9126, (int16_t)0x0000, (int16_t)0x8000, (int16_t)0x6BF4, (int16_t)0xBB3A,
    (int16_t)0x3618, (int16_t)0x8BFE, (int16_t)0xEF4B, (int16_t)0x8118, (int16_t)0x68DA, (int16_t)0xB695, (int16_t)0x2BC7, (int16_t)0x87B8,
    (int16_t)0xDEDF, (int16_t)0x845D, (int16_t)0x658D, (int16_t)0xB214, (int16_t)0x2121, (int16_t)0x845D, (int16_t)0xCF04, (int16_t)0x89BE,
    (int16_t)0x620E, (int16_t)0xADB9, (int16_t)0x163A, (int16_t)0x81F2, (int16_t)0xC000, (int16_t)0x9126, (int16_t)0x5E5F, (int16_t)0xA986,
    (int16_t)0x0B28, (int16_t)0x807D, (int16_t)0xB214, (int16_t)0x9A73, (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x0000, (int16_t)0x8000,
    (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0x567A, (int16_t)0xA1A1, (int16_t)0xF4D8, (int16_t)0x807D, (int16_t)0x9A73, (int16_t)0xB214,
    (int16_t)0x5247, (int16_t)0x9DF2, (int16_t)0xE9C6, (int16_t)0x81F2, (int16_t)0x9126, (int16_t)0xC000, (int16_t)0x4DEC, (int16_t)0x9A73,
    (int16_t)0xDEDF, (int16_t)0x845D, (int16_t)0x89BE, (int16_t)0xCF04, (int16_t)0x496B, (int16_t)0x9726, (int16_t)0xD439, (int16_t)0x87B8,
    (int16_t)0x845D, (int16_t)0xDEDF, (int16_t)0x44C6, (int16_t)0x940C, (int16_t)0xC9E8, (int16_t)0x8BFE, (int16_t)0x8118, (int16_t)0xEF4B,
    (int16_t)0x4000, (int16_t)0x9126, (int16_t)0xC000, (int16_t)0x9126, (int16_t)0x8000, (int16_t)0x0000, (int16_t)0x3B1B, (int16_t)0x8E76,
    (int16_t)0xB695, (int16_t)0x9726, (int16_t)0x8118, (int16_t)0x10B5, (int16_t)0x3618, (int16_t)0x8BFE, (int16_t)0xADB9, (int16_t)0x9DF2,
    (int16_t)0x845D, (int16_t)0x2121, (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0x89BE, (int16_t)0x30FC,
    (int16_t)0x2BC7, (int16_t)0x87B8, (int16_t)0x9DF2, (int16_t)0xADB9, (int16_t)0x9126, (int16_t)0x4000, (int16_t)0x267E, (int16_t)0x85ED,
    (int16_t)0x9726, (int16_t)0xB695, (int16_t)0x9A73, (int16_t)0x4DEC, (int16_t)0x2121, (int16_t)0x845D, (int16_t)0x9126, (int16_t)0xC000,
    (int16_t)0xA57E, (int16_t)0x5A82, (int16_t)0x1BB4, (int16_t)0x8309, (int16_t)0x8BFE, (int16_t)0xC9E8, (int16_t)0xB214, (int16_t)0x658D,
    (int16_t)0x163A, (int16_t)0x81F2, (int16_t)0x87B8, (int16_t)0xD439, (int16_t)0xC000, (int16_t)0x6EDA, (int16_t)0x10B5, (int16_t)0x8118,
    (int16_t)0x845D, (int16_t)0xDEDF, (int16_t)0xCF04, (int16_t)0x7642, (int16_t)0x0B28, (int16_t)0x807D, (int16_t)0x81F2, (int16_t)0xE9C6,
    (int16_t)0xDEDF, (int16_t)0x7BA3, (int16_t)0x0595, (int16_t)0x801F, (int16_t)0x807D, (int16_t)0xF4D8, (int16_t)0xEF4B, (int16_t)0x7EE8,
};

/********** Twiddles table N=576 stage 3 radix 3 ******************/
ALIGN(8) static const int16_t _fft576_tw3_[] =
{
    (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7E0E, (int16_t)0xE9C6, (int16_t)0x7848, (int16_t)0xD439,
    (int16_t)0x7848, (int16_t)0xD439, (int16_t)0x620E, (int16_t)0xADB9, (int16_t)0x6EDA, (int16_t)0xC000, (int16_t)0x4000, (int16_t)0x9126,
    (int16_t)0x620E, (int16_t)0xADB9, (int16_t)0x163A, (int16_t)0x81F2, (int16_t)0x5247, (int16_t)0x9DF2, (int16_t)0xE9C6, (int16_t)0x81F2,
    (int16_t)0x4000, (int16_t)0x9126, (int16_t)0xC000, (int16_t)0x9126, (int16_t)0x2BC7, (int16_t)0x87B8, (int16_t)0x9DF2, (int16_t)0xADB9,
    (int16_t)0x163A, (int16_t)0x81F2, (int16_t)0x87B8, (int16_t)0xD439, (int16_t)0x0000, (int16_t)0x8000, (int16_t)0x8000, (int16_t)0x0000,
    (int16_t)0xE9C6, (int16_t)0x81F2, (int16_t)0x87B8, (int16_t)0x2BC7, (int16_t)0xD439, (int16_t)0x87B8, (int16_t)0x9DF2, (int16_t)0x5247,
};

/********** Twiddles table N=576 stage 4 radix 3 ******************/
ALIGN(8) static const int16_t _fft576_tw4_[] =
{
    (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x6EDA, (int16_t)0xC000, (int16_t)0x4000, (int16_t)0x9126,
    (int16_t)0x4000, (int16_t)0x9126, (int16_t)0xC000, (int16_t)0x9126, (int16_t)0x0000, (int16_t)0x8000, (int16_t)0x8000, (int16_t)0x0000,
};

static const int tw_step_tab[] =
{
    1, 1, 1, 1, 0
}; 
static const cint16ptr_t tw_tab[] = 
{
    _fft576_tw1_, _fft576_tw2_, _fft576_tw3_, _fft576_tw4_, NULL
};


static const eFft_stage_type fft_cplx16x16_stg_tab[] = 
{
    fft_stage_first_DFT4,
    fft_stage_inner_DFT4,
    fft_stage_inner_DFT3,
    fft_stage_inner_DFT3,
    fft_stage_last_DFT4
};
static const eFft_stage_type ifft_cplx16x16_stg_tab[] =
{
    fft_stage_first_inv_DFT4,
    fft_stage_inner_DFT4,
    fft_stage_inner_DFT3,
    fft_stage_inner_DFT3,
    fft_stage_last_DFT4
};

const fft_cplx_x16_descr_t __cfft_x16_descr576 =
{
    N, 
    NULL, 
    fft_cplx16x16_stg_tab, 
    NULL, 
    fft_cplx16x16_stg_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_cplx_x16_descr_t __cifft_x16_descr576 =
{
    N, 
    NULL, 
    ifft_cplx16x16_stg_tab, 
    NULL, 
    ifft_cplx16x16_stg_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft16_576 = (const fft_handle_t)&__cfft_x16_descr576;
const fft_handle_t cinfft16_576 = (const fft_handle_t)&__cifft_x16_descr576;
