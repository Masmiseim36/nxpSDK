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
#include "fft_twiddles32x32.h"
#include "common.h"

/* Twiddles tables for fft_cplx32x32, ifft_cplx32x32 */

/****************** stage 1 radix 3 ******************/
ALIGN(8) static const int32_t __fft192_tw1[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FEE74A2, (int32_t)0xFBCFDC71, (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, 
    (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, 
    (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7E4A5426, (int32_t)0xEB263DBB, (int32_t)0x793501A9, (int32_t)0xD6DB1254, 
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x7CA80038, (int32_t)0xE2EF2A3E, (int32_t)0x72CCB9DB, (int32_t)0xC763158E, 
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, 
    (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x77CBC3F2, (int32_t)0xD2E9786E, (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, 
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x74972F92, (int32_t)0xCB2C6A82, (int32_t)0x54657194, (int32_t)0x9FC3B694, 
    (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x471CECE7, (int32_t)0x9592675C, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x6CB2A837, (int32_t)0xBC6845CE, (int32_t)0x389CEA72, (int32_t)0x8D334625, 
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x680B5C33, (int32_t)0xB5715EEF, (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, 
    (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, 
    (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x5D6C2F99, (int32_t)0xA8800C26, (int32_t)0x085F2137, (int32_t)0x804628A7, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x577FF3DA, (int32_t)0xA293D067, (int32_t)0xF7A0DEC9, (int32_t)0x804628A7, 
    (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0xEF4AEAF1, (int32_t)0x811855B4, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0xE70747C4, (int32_t)0x8275A0C0, 
    (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x4A8EA111, (int32_t)0x97F4A3CD, (int32_t)0xD6DB1254, (int32_t)0x86CAFE57, 
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, (int32_t)0x4397BA32, (int32_t)0x934D57C9, (int32_t)0xC763158E, (int32_t)0x8D334625, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0xB8E31319, (int32_t)0x9592675C, 
    (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0xB2141B02, (int32_t)0x9A7365D3, (int32_t)0x34D3957E, (int32_t)0x8B68D06E, (int32_t)0xAB9A8E6C, (int32_t)0x9FC3B694, 
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x2D168792, (int32_t)0x88343C0E, (int32_t)0x9FC3B694, (int32_t)0xAB9A8E6C, 
    (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x9A7365D3, (int32_t)0xB2141B02, (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0x9592675C, (int32_t)0xB8E31319, 
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x1D10D5C2, (int32_t)0x8357FFC8, (int32_t)0x8D334625, (int32_t)0xC763158E, 
    (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, (int32_t)0x14D9C245, (int32_t)0x81B5ABDA, (int32_t)0x86CAFE57, (int32_t)0xD6DB1254, 
    (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x8275A0C0, (int32_t)0xE70747C4, 
    (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x811855B4, (int32_t)0xEF4AEAF1, (int32_t)0x0430238F, (int32_t)0x80118B5E, (int32_t)0x804628A7, (int32_t)0xF7A0DEC9, 
    (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x80000000, (int32_t)0x00000000, (int32_t)0xFBCFDC71, (int32_t)0x80118B5E, (int32_t)0x804628A7, (int32_t)0x085F2137, 
    (int32_t)0xF7A0DEC9, (int32_t)0x804628A7, (int32_t)0x811855B4, (int32_t)0x10B5150F, (int32_t)0xF3742CA2, (int32_t)0x809DC971, (int32_t)0x8275A0C0, (int32_t)0x18F8B83C, 
    (int32_t)0xEF4AEAF1, (int32_t)0x811855B4, (int32_t)0x845C8AE3, (int32_t)0x2120FB83, (int32_t)0xEB263DBB, (int32_t)0x81B5ABDA, (int32_t)0x86CAFE57, (int32_t)0x2924EDAC, 
    (int32_t)0xE70747C4, (int32_t)0x8275A0C0, (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, (int32_t)0xE2EF2A3E, (int32_t)0x8357FFC8, (int32_t)0x8D334625, (int32_t)0x389CEA72, 
    (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0x40000000, (int32_t)0xDAD7F3A2, (int32_t)0x8582FAA5, (int32_t)0x9592675C, (int32_t)0x471CECE7, 
    (int32_t)0xD6DB1254, (int32_t)0x86CAFE57, (int32_t)0x9A7365D3, (int32_t)0x4DEBE4FE, (int32_t)0xD2E9786E, (int32_t)0x88343C0E, (int32_t)0x9FC3B694, (int32_t)0x54657194, 
    (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0x5A82799A, (int32_t)0xCB2C6A82, (int32_t)0x8B68D06E, (int32_t)0xAB9A8E6C, (int32_t)0x603C496C, 
    (int32_t)0xC763158E, (int32_t)0x8D334625, (int32_t)0xB2141B02, (int32_t)0x658C9A2D, (int32_t)0xC3A94590, (int32_t)0x8F1D343A, (int32_t)0xB8E31319, (int32_t)0x6A6D98A4, 
};

/****************** stage 2 radix 4 ******************/
ALIGN(8) static const int32_t __fft192_tw2[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7F62368F, (int32_t)0xF3742CA2, 
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, 
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, 
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, 
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, 
    (int32_t)0xE70747C4, (int32_t)0x8275A0C0, (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0xC3A94590, (int32_t)0x8F1D343A, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, 
    (int32_t)0xE70747C4, (int32_t)0x8275A0C0, (int32_t)0x8F1D343A, (int32_t)0xC3A94590, (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, 
    (int32_t)0x8275A0C0, (int32_t)0xE70747C4, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0xB8E31319, (int32_t)0x9592675C, (int32_t)0x809DC971, (int32_t)0x0C8BD35E, 
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, (int32_t)0x25280C5E, (int32_t)0x8582FAA5, 
    (int32_t)0x9592675C, (int32_t)0xB8E31319, (int32_t)0x9D0DFE54, (int32_t)0x5133CC94, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, 
    (int32_t)0xB8E31319, (int32_t)0x6A6D98A4, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x8275A0C0, (int32_t)0xE70747C4, (int32_t)0xDAD7F3A2, (int32_t)0x7A7D055B, 
};

/****************** stage 3 radix 4 ******************/
ALIGN(8) static const int32_t __fft192_tw3[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, 
    (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, 
};
#define N 192
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT3_first_32x32,
    fft_stageS2_DFT4_32x32,
    fft_stageS2_DFT4x2_32x32,
    fft_stageS2_DFT4_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT3_first_32x32,
    fft_stageS3_DFT4_32x32,
    fft_stageS3_DFT4x2_32x32,
    fft_stageS3_DFT4_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT3_first_32x32,
    fft_stageS2_DFT4_32x32,
    fft_stageS2_DFT4x2_32x32,
    fft_stageS2_DFT4_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT3_first_32x32,
    fft_stageS3_DFT4_32x32,
    fft_stageS3_DFT4x2_32x32,
    fft_stageS3_DFT4_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 1, 
}; 
static const cint32ptr_t tw_tab[] = 
{
    __fft192_tw1, __fft192_tw2, __fft192_tw3, NULL
};
const fft_cplx32x32_descr_t __cfft_descr192_32x32 =
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_cplx32x32_descr_t __cifft_descr192_32x32 =
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_192 = (const fft_handle_t)&__cfft_descr192_32x32;
const fft_handle_t cinfft32_192 = (const fft_handle_t)&__cifft_descr192_32x32;
