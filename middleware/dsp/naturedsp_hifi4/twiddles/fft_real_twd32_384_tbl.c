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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFB9D15, (int32_t)0xFDE7DBD9, (int32_t)0x7FEE74A2, (int32_t)0xFBCFDC71, (int32_t)0x7FD8878E, (int32_t)0xF9B82684,
    (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7F92661D, (int32_t)0xF58A29F2, (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7F294BFD, (int32_t)0xF15F0B74,
    (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7E9D55FC, (int32_t)0xED37EF91, (int32_t)0x7E4A5426, (int32_t)0xEB263DBB, (int32_t)0x7DEEAA7A, (int32_t)0xE915F9BA,
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7D1D7958, (int32_t)0xE4FA4BF1, (int32_t)0x7CA80038, (int32_t)0xE2EF2A3E, (int32_t)0x7C29FBEE, (int32_t)0xE0E60685,
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7B1474FD, (int32_t)0xDCDA47B9, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x79DD3098, (int32_t)0xD8D82B7A,
    (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x78848414, (int32_t)0xD4E0CB15, (int32_t)0x77CBC3F2, (int32_t)0xD2E9786E, (int32_t)0x770ACDEC, (int32_t)0xD0F53CE0,
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x757075AC, (int32_t)0xCD1693F7, (int32_t)0x74972F92, (int32_t)0xCB2C6A82, (int32_t)0x73B5EBD1, (int32_t)0xC945DFEC,
    (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x71DBA9AB, (int32_t)0xC5842C7E, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x6FE2313C, (int32_t)0xC1D2814F,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6DCA0D14, (int32_t)0xBE31E19B, (int32_t)0x6CB2A837, (int32_t)0xBC6845CE, (int32_t)0x6B93D02E, (int32_t)0xBAA34BF4,
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x694015C3, (int32_t)0xB727B9F7, (int32_t)0x680B5C33, (int32_t)0xB5715EEF, (int32_t)0x66CF8120, (int32_t)0xB3C0200C,
    (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x6442BD7E, (int32_t)0xB06D6D24, (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x619A7DCE, (int32_t)0xAD308A71,
    (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x5ED77C8A, (int32_t)0xAA0A5B2E, (int32_t)0x5D6C2F99, (int32_t)0xA8800C26, (int32_t)0x5BFA7B82, (int32_t)0xA6FBBC59,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x590443A7, (int32_t)0xA405847E, (int32_t)0x577FF3DA, (int32_t)0xA293D067, (int32_t)0x55F5A4D2, (int32_t)0xA1288376,
    (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0x52CF758F, (int32_t)0x9E658232, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0x4F9292DC, (int32_t)0x9BBD4282,
    (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x4C3FDFF4, (int32_t)0x99307EE0, (int32_t)0x4A8EA111, (int32_t)0x97F4A3CD, (int32_t)0x48D84609, (int32_t)0x96BFEA3D,
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x455CB40C, (int32_t)0x946C2FD2, (int32_t)0x4397BA32, (int32_t)0x934D57C9, (int32_t)0x41CE1E65, (int32_t)0x9235F2EC,
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x3E2D7EB1, (int32_t)0x901DCEC4, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0x3A7BD382, (int32_t)0x8E245655,
    (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0x36BA2014, (int32_t)0x8C4A142F, (int32_t)0x34D3957E, (int32_t)0x8B68D06E, (int32_t)0x32E96C09, (int32_t)0x8A8F8A54,
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x2F0AC320, (int32_t)0x88F53214, (int32_t)0x2D168792, (int32_t)0x88343C0E, (int32_t)0x2B1F34EB, (int32_t)0x877B7BEC,
    (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x2727D486, (int32_t)0x8622CF68, (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0x2325B847, (int32_t)0x84EB8B03,
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x1F19F97B, (int32_t)0x83D60412, (int32_t)0x1D10D5C2, (int32_t)0x8357FFC8, (int32_t)0x1B05B40F, (int32_t)0x82E286A8,
    (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x16EA0646, (int32_t)0x82115586, (int32_t)0x14D9C245, (int32_t)0x81B5ABDA, (int32_t)0x12C8106F, (int32_t)0x8162AA04,
    (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x0EA0F48C, (int32_t)0x80D6B403, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x0A75D60E, (int32_t)0x806D99E3,
    (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x0647D97C, (int32_t)0x80277872, (int32_t)0x0430238F, (int32_t)0x80118B5E, (int32_t)0x02182427, (int32_t)0x800462EB,
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr192_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr192_32x32,
    __fft_real_tw
};

const fft_handle_t rnfft32_384 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_384 = (const fft_handle_t)&__rifft_descr;
