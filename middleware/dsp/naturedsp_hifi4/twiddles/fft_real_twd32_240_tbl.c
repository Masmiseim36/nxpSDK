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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FF4C56F, (int32_t)0xFCA63BD8, (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67,
    (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x7DDB4BFC, (int32_t)0xE8AC819D,
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7ABA9AE6, (int32_t)0xDBA5629B,
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x78A879F4, (int32_t)0xD545D11C, (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x7387EA23, (int32_t)0xC8E5032B, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x707D1443, (int32_t)0xC2EC7635,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6D23501B, (int32_t)0xBD1EC45C, (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x697CF78A, (int32_t)0xB780001C,
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x637984D4, (int32_t)0xAF726DEF, (int32_t)0x6154FB91, (int32_t)0xACDEE2E8,
    (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x5CD91140, (int32_t)0xA7E3FF4D, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x581C00B3, (int32_t)0xA326EEC0,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x53211D18, (int32_t)0x9EAB046F, (int32_t)0x508D9211, (int32_t)0x9C867B2C, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3,
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x487FFFE4, (int32_t)0x96830876, (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5,
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x3D1389CB, (int32_t)0x8F82EBBD, (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0x371AFCD5, (int32_t)0x8C7815DD,
    (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0x2ABA2EE4, (int32_t)0x8757860C,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x245A9D65, (int32_t)0x8545651A, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x1DE189A6, (int32_t)0x838961E8,
    (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x17537E63, (int32_t)0x8224B404, (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x10B5150F, (int32_t)0x811855B4,
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x0A0AF299, (int32_t)0x80650347, (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, (int32_t)0x0359C428, (int32_t)0x800B3A91,
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr120_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr120_32x32,
    __fft_real_tw
};

const fft_handle_t rnfft32_240=  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_240 = (const fft_handle_t)&__rifft_descr;
