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

/****************** stage 1 radix 4 ******************/
ALIGN(8) static const int32_t __fft480_tw1[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFD3154, (int32_t)0xFE531484, 
    (int32_t)0x7FF4C56F, (int32_t)0xFCA63BD8, (int32_t)0x7FE6BCB0, (int32_t)0xFAF988CC, (int32_t)0x7FF4C56F, (int32_t)0xFCA63BD8, (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, 
    (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67, (int32_t)0x7FE6BCB0, (int32_t)0xFAF988CC, (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67, (int32_t)0x7F1CDE01, (int32_t)0xF0F488D9, 
    (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, 
    (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67, (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, 
    (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x7F76892F, (int32_t)0xF449ACCA, (int32_t)0x7DDB4BFC, (int32_t)0xE8AC819D, (int32_t)0x7B31BBB2, (int32_t)0xDD417079, 
    (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x7F1CDE01, (int32_t)0xF0F488D9, 
    (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x7816A759, (int32_t)0xD3B26FB0, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, 
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x7EACE58A, (int32_t)0xEDA208A5, (int32_t)0x7ABA9AE6, (int32_t)0xDBA5629B, (int32_t)0x743E0918, (int32_t)0xCA695B94, 
    (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x7E26B371, (int32_t)0xEA52C166, 
    (int32_t)0x78A879F4, (int32_t)0xD545D11C, (int32_t)0x6FADF2FC, (int32_t)0xC174DBF2, (int32_t)0x7DDB4BFC, (int32_t)0xE8AC819D, (int32_t)0x777F903C, (int32_t)0xD220FFC0, 
    (int32_t)0x6D23501B, (int32_t)0xBD1EC45C, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, 
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x7CD80464, (int32_t)0xE3C02FBB, 
    (int32_t)0x7387EA23, (int32_t)0xC8E5032B, (int32_t)0x648543E4, (int32_t)0xB0C1878B, (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, 
    (int32_t)0x6154FB91, (int32_t)0xACDEE2E8, (int32_t)0x7C0FC22A, (int32_t)0xE07E0C84, (int32_t)0x707D1443, (int32_t)0xC2EC7635, (int32_t)0x5DFE47AD, (int32_t)0xA91D0EA3, 
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x7B31BBB2, (int32_t)0xDD417079, 
    (int32_t)0x6D23501B, (int32_t)0xBD1EC45C, (int32_t)0x56E2F15D, (int32_t)0xA201B853, (int32_t)0x7ABA9AE6, (int32_t)0xDBA5629B, (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, 
    (int32_t)0x53211D18, (int32_t)0x9EAB046F, (int32_t)0x7A3E17F2, (int32_t)0xDA0AECF9, (int32_t)0x697CF78A, (int32_t)0xB780001C, (int32_t)0x4F3E7875, (int32_t)0x9B7ABC1C, 
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x793501A9, (int32_t)0xD6DB1254, 
    (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x78A879F4, (int32_t)0xD545D11C, (int32_t)0x637984D4, (int32_t)0xAF726DEF, 
    (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5, (int32_t)0x7816A759, (int32_t)0xD3B26FB0, (int32_t)0x6154FB91, (int32_t)0xACDEE2E8, (int32_t)0x3E8B240E, (int32_t)0x90520D04, 
    (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0x76E33B3F, (int32_t)0xD09192EA, 
    (int32_t)0x5CD91140, (int32_t)0xA7E3FF4D, (int32_t)0x3596A46C, (int32_t)0x8BC1F6E8, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, 
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x759AF34C, (int32_t)0xCD790887, (int32_t)0x581C00B3, (int32_t)0xA326EEC0, (int32_t)0x2C4D9050, (int32_t)0x87E958A7, 
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x743E0918, (int32_t)0xCA695B94, 
    (int32_t)0x53211D18, (int32_t)0x9EAB046F, (int32_t)0x22BE8F87, (int32_t)0x84CE444E, (int32_t)0x7387EA23, (int32_t)0xC8E5032B, (int32_t)0x508D9211, (int32_t)0x9C867B2C, 
    (int32_t)0x1DE189A6, (int32_t)0x838961E8, (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, 
    (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x71474660, (int32_t)0xC466BE4F, 
    (int32_t)0x487FFFE4, (int32_t)0x96830876, (int32_t)0x0F0B7727, (int32_t)0x80E321FF, (int32_t)0x707D1443, (int32_t)0xC2EC7635, (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, 
    (int32_t)0x0A0AF299, (int32_t)0x80650347, (int32_t)0x6FADF2FC, (int32_t)0xC174DBF2, (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5, (int32_t)0x05067734, (int32_t)0x80194350, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x6E010780, (int32_t)0xBE8DF2BA, 
    (int32_t)0x3D1389CB, (int32_t)0x8F82EBBD, (int32_t)0xFAF988CC, (int32_t)0x80194350, (int32_t)0x6D23501B, (int32_t)0xBD1EC45C, (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, 
    (int32_t)0xF5F50D67, (int32_t)0x80650347, (int32_t)0x6C40CF2C, (int32_t)0xBBB28501, (int32_t)0x371AFCD5, (int32_t)0x8C7815DD, (int32_t)0xF0F488D9, (int32_t)0x80E321FF, 
    (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0xEBF9F498, (int32_t)0x81936DAF, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, 
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xE70747C4, (int32_t)0x8275A0C0, (int32_t)0x697CF78A, (int32_t)0xB780001C, (int32_t)0x2DDF0040, (int32_t)0x88806FC4, 
    (int32_t)0xE21E765A, (int32_t)0x838961E8, (int32_t)0x6887B5E2, (int32_t)0xB6201B3E, (int32_t)0x2ABA2EE4, (int32_t)0x8757860C, (int32_t)0xDD417079, (int32_t)0x84CE444E, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x668F7C25, (int32_t)0xB36A1978, 
    (int32_t)0x245A9D65, (int32_t)0x8545651A, (int32_t)0xD3B26FB0, (int32_t)0x87E958A7, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, 
    (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, (int32_t)0x648543E4, (int32_t)0xB0C1878B, (int32_t)0x1DE189A6, (int32_t)0x838961E8, (int32_t)0xCA695B94, (int32_t)0x8BC1F6E8, 
    (int32_t)0x637984D4, (int32_t)0xAF726DEF, (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0xC5E3A3A9, (int32_t)0x8DF37F8B, (int32_t)0x626968BE, (int32_t)0xAE26DCDF, 
    (int32_t)0x17537E63, (int32_t)0x8224B404, (int32_t)0xC174DBF2, (int32_t)0x90520D04, (int32_t)0x6154FB91, (int32_t)0xACDEE2E8, (int32_t)0x14060B68, (int32_t)0x81936DAF, 
    (int32_t)0xBD1EC45C, (int32_t)0x92DCAFE5, (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0xB8E31319, (int32_t)0x9592675C, 
    (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x5DFE47AD, (int32_t)0xA91D0EA3, 
    (int32_t)0x0A0AF299, (int32_t)0x80650347, (int32_t)0xB0C1878B, (int32_t)0x9B7ABC1C, (int32_t)0x5CD91140, (int32_t)0xA7E3FF4D, (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, 
    (int32_t)0xACDEE2E8, (int32_t)0x9EAB046F, (int32_t)0x5BAFC837, (int32_t)0xA6AECD5E, (int32_t)0x0359C428, (int32_t)0x800B3A91, (int32_t)0xA91D0EA3, (int32_t)0xA201B853, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x595132A2, (int32_t)0xA45037C9, 
    (int32_t)0xFCA63BD8, (int32_t)0x800B3A91, (int32_t)0xA201B853, (int32_t)0xA91D0EA3, (int32_t)0x581C00B3, (int32_t)0xA326EEC0, (int32_t)0xF94D0E2E, (int32_t)0x802CE84C, 
    (int32_t)0x9EAB046F, (int32_t)0xACDEE2E8, (int32_t)0x56E2F15D, (int32_t)0xA201B853, (int32_t)0xF5F50D67, (int32_t)0x80650347, (int32_t)0x9B7ABC1C, (int32_t)0xB0C1878B, 
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x98722192, (int32_t)0xB4C373EE, (int32_t)0x54657194, (int32_t)0x9FC3B694, 
    (int32_t)0xEF4AEAF1, (int32_t)0x811855B4, (int32_t)0x9592675C, (int32_t)0xB8E31319, (int32_t)0x53211D18, (int32_t)0x9EAB046F, (int32_t)0xEBF9F498, (int32_t)0x81936DAF, 
    (int32_t)0x92DCAFE5, (int32_t)0xBD1EC45C, (int32_t)0x51D92321, (int32_t)0x9D969742, (int32_t)0xE8AC819D, (int32_t)0x8224B404, (int32_t)0x90520D04, (int32_t)0xC174DBF2, 
    (int32_t)0x508D9211, (int32_t)0x9C867B2C, (int32_t)0xE5632654, (int32_t)0x82CC0F36, (int32_t)0x8DF37F8B, (int32_t)0xC5E3A3A9, (int32_t)0x4F3E7875, (int32_t)0x9B7ABC1C, 
    (int32_t)0xE21E765A, (int32_t)0x838961E8, (int32_t)0x8BC1F6E8, (int32_t)0xCA695B94, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, 
    (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, (int32_t)0x4C95E688, (int32_t)0x997083DB, (int32_t)0xDBA5629B, (int32_t)0x8545651A, (int32_t)0x87E958A7, (int32_t)0xD3B26FB0, 
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x49DFE4C2, (int32_t)0x97784A1E, 
    (int32_t)0xD545D11C, (int32_t)0x8757860C, (int32_t)0x84CE444E, (int32_t)0xDD417079, (int32_t)0x487FFFE4, (int32_t)0x96830876, (int32_t)0xD220FFC0, (int32_t)0x88806FC4, 
    (int32_t)0x838961E8, (int32_t)0xE21E765A, (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, (int32_t)0x8275A0C0, (int32_t)0xE70747C4, 
    (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0xCBF00DBE, (int32_t)0x8B10F144, (int32_t)0x81936DAF, (int32_t)0xEBF9F498, (int32_t)0x444D7AFF, (int32_t)0x93BF30D4, 
    (int32_t)0xC8E5032B, (int32_t)0x8C7815DD, (int32_t)0x80E321FF, (int32_t)0xF0F488D9, (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5, (int32_t)0xC5E3A3A9, (int32_t)0x8DF37F8B, 
    (int32_t)0x80650347, (int32_t)0xF5F50D67, (int32_t)0x41720D46, (int32_t)0x91FEF880, (int32_t)0xC2EC7635, (int32_t)0x8F82EBBD, (int32_t)0x80194350, (int32_t)0xFAF988CC, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, (int32_t)0x3E8B240E, (int32_t)0x90520D04, 
    (int32_t)0xBD1EC45C, (int32_t)0x92DCAFE5, (int32_t)0x80194350, (int32_t)0x05067734, (int32_t)0x3D1389CB, (int32_t)0x8F82EBBD, (int32_t)0xBA4944A2, (int32_t)0x94A6715D, 
    (int32_t)0x80650347, (int32_t)0x0A0AF299, (int32_t)0x3B9941B1, (int32_t)0x8EB8B9A0, (int32_t)0xB780001C, (int32_t)0x96830876, (int32_t)0x80E321FF, (int32_t)0x0F0B7727, 
    (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x81936DAF, (int32_t)0x14060B68, (int32_t)0x389CEA72, (int32_t)0x8D334625, 
    (int32_t)0xB2141B02, (int32_t)0x9A7365D3, (int32_t)0x8275A0C0, (int32_t)0x18F8B83C, (int32_t)0x371AFCD5, (int32_t)0x8C7815DD, (int32_t)0xAF726DEF, (int32_t)0x9C867B2C, 
    (int32_t)0x838961E8, (int32_t)0x1DE189A6, (int32_t)0x3596A46C, (int32_t)0x8BC1F6E8, (int32_t)0xACDEE2E8, (int32_t)0x9EAB046F, (int32_t)0x84CE444E, (int32_t)0x22BE8F87, 
    (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0xAA59EDA4, (int32_t)0xA0E0A15F, (int32_t)0x8643C7B3, (int32_t)0x278DDE6E, (int32_t)0x3286F779, (int32_t)0x8A650CB4, 
    (int32_t)0xA7E3FF4D, (int32_t)0xA326EEC0, (int32_t)0x87E958A7, (int32_t)0x2C4D9050, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666, 
    (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, (int32_t)0x2F6E6D16, (int32_t)0x891CC4C1, (int32_t)0xA326EEC0, (int32_t)0xA7E3FF4D, (int32_t)0x8BC1F6E8, (int32_t)0x3596A46C, 
    (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0xA0E0A15F, (int32_t)0xAA59EDA4, (int32_t)0x8DF37F8B, (int32_t)0x3A1C5C57, (int32_t)0x2C4D9050, (int32_t)0x87E958A7, 
    (int32_t)0x9EAB046F, (int32_t)0xACDEE2E8, (int32_t)0x90520D04, (int32_t)0x3E8B240E, (int32_t)0x2ABA2EE4, (int32_t)0x8757860C, (int32_t)0x9C867B2C, (int32_t)0xAF726DEF, 
    (int32_t)0x92DCAFE5, (int32_t)0x42E13BA4, (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x9A7365D3, (int32_t)0xB2141B02, (int32_t)0x9592675C, (int32_t)0x471CECE7, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x4B3C8C12, (int32_t)0x25F51307, (int32_t)0x85C1E80E, 
    (int32_t)0x96830876, (int32_t)0xB780001C, (int32_t)0x9B7ABC1C, (int32_t)0x4F3E7875, (int32_t)0x245A9D65, (int32_t)0x8545651A, (int32_t)0x94A6715D, (int32_t)0xBA4944A2, 
    (int32_t)0x9EAB046F, (int32_t)0x53211D18, (int32_t)0x22BE8F87, (int32_t)0x84CE444E, (int32_t)0x92DCAFE5, (int32_t)0xBD1EC45C, (int32_t)0xA201B853, (int32_t)0x56E2F15D, 
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0xA57D8666, (int32_t)0x5A82799A, (int32_t)0x1F81F37C, (int32_t)0x83F03DD6, 
    (int32_t)0x8F82EBBD, (int32_t)0xC2EC7635, (int32_t)0xA91D0EA3, (int32_t)0x5DFE47AD, (int32_t)0x1DE189A6, (int32_t)0x838961E8, (int32_t)0x8DF37F8B, (int32_t)0xC5E3A3A9, 
    (int32_t)0xACDEE2E8, (int32_t)0x6154FB91, (int32_t)0x1C3FD045, (int32_t)0x8327FB9C, (int32_t)0x8C7815DD, (int32_t)0xC8E5032B, (int32_t)0xB0C1878B, (int32_t)0x648543E4, 
    (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x8B10F144, (int32_t)0xCBF00DBE, (int32_t)0xB4C373EE, (int32_t)0x678DDE6E, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, 
    (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, (int32_t)0xB8E31319, (int32_t)0x6A6D98A4, (int32_t)0x17537E63, (int32_t)0x8224B404, (int32_t)0x88806FC4, (int32_t)0xD220FFC0, 
    (int32_t)0xBD1EC45C, (int32_t)0x6D23501B, (int32_t)0x15AD3E9A, (int32_t)0x81D94C8F, (int32_t)0x8757860C, (int32_t)0xD545D11C, (int32_t)0xC174DBF2, (int32_t)0x6FADF2FC, 
    (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0xC5E3A3A9, (int32_t)0x720C8075, (int32_t)0x125DF75B, (int32_t)0x81531A76, 
    (int32_t)0x8545651A, (int32_t)0xDBA5629B, (int32_t)0xCA695B94, (int32_t)0x743E0918, (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, 
    (int32_t)0xCF043AB3, (int32_t)0x7641AF3D, (int32_t)0x0F0B7727, (int32_t)0x80E321FF, (int32_t)0x838961E8, (int32_t)0xE21E765A, (int32_t)0xD3B26FB0, (int32_t)0x7816A759, 
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0xE5632654, (int32_t)0xD8722192, (int32_t)0x79BC384D, (int32_t)0x0BB65336, (int32_t)0x808976D1, 
    (int32_t)0x8224B404, (int32_t)0xE8AC819D, (int32_t)0xDD417079, (int32_t)0x7B31BBB2, (int32_t)0x0A0AF299, (int32_t)0x80650347, (int32_t)0x81936DAF, (int32_t)0xEBF9F498, 
    (int32_t)0xE21E765A, (int32_t)0x7C769E18, (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x811855B4, (int32_t)0xEF4AEAF1, (int32_t)0xE70747C4, (int32_t)0x7D8A5F40, 
    (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, (int32_t)0x80B381AC, (int32_t)0xF29ECFB2, (int32_t)0xEBF9F498, (int32_t)0x7E6C9251, (int32_t)0x05067734, (int32_t)0x80194350, 
    (int32_t)0x80650347, (int32_t)0xF5F50D67, (int32_t)0xF0F488D9, (int32_t)0x7F1CDE01, (int32_t)0x0359C428, (int32_t)0x800B3A91, (int32_t)0x802CE84C, (int32_t)0xF94D0E2E, 
    (int32_t)0xF5F50D67, (int32_t)0x7F9AFCB9, (int32_t)0x01ACEB7C, (int32_t)0x8002CEAC, (int32_t)0x800B3A91, (int32_t)0xFCA63BD8, (int32_t)0xFAF988CC, (int32_t)0x7FE6BCB0, 
};

/****************** stage 2 radix 4 ******************/
ALIGN(8) static const int32_t __fft480_tw2[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, 
    (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x79BC384D, (int32_t)0xD8722192, 
    (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, 
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, 
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x5A82799A, (int32_t)0xA57D8666, 
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x4B3C8C12, (int32_t)0x98722192, 
    (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, 
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, 
    (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x14060B68, (int32_t)0x81936DAF, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, 
    (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0xEBF9F498, (int32_t)0x81936DAF, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x8643C7B3, 
    (int32_t)0x637984D4, (int32_t)0xAF726DEF, (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0xC5E3A3A9, (int32_t)0x8DF37F8B, 
    (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0xB4C373EE, (int32_t)0x98722192, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xA57D8666, (int32_t)0xA57D8666, 
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x98722192, (int32_t)0xB4C373EE, 
    (int32_t)0x508D9211, (int32_t)0x9C867B2C, (int32_t)0xE5632654, (int32_t)0x82CC0F36, (int32_t)0x8DF37F8B, (int32_t)0xC5E3A3A9, 
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x8643C7B3, (int32_t)0xD8722192, 
    (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0xCBF00DBE, (int32_t)0x8B10F144, (int32_t)0x81936DAF, (int32_t)0xEBF9F498, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, 
    (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x81936DAF, (int32_t)0x14060B68, 
    (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0xAA59EDA4, (int32_t)0xA0E0A15F, (int32_t)0x8643C7B3, (int32_t)0x278DDE6E, 
    (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0xA0E0A15F, (int32_t)0xAA59EDA4, (int32_t)0x8DF37F8B, (int32_t)0x3A1C5C57, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x4B3C8C12, 
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0xA57D8666, (int32_t)0x5A82799A, 
    (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x8B10F144, (int32_t)0xCBF00DBE, (int32_t)0xB4C373EE, (int32_t)0x678DDE6E, 
    (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0xC5E3A3A9, (int32_t)0x720C8075, 
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0xE5632654, (int32_t)0xD8722192, (int32_t)0x79BC384D, 
    (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, (int32_t)0x80B381AC, (int32_t)0xF29ECFB2, (int32_t)0xEBF9F498, (int32_t)0x7E6C9251, 
};

/****************** stage 3 radix 2 ******************/
ALIGN(8) static const int32_t __fft480_tw3[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, 
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, 
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, 
    (int32_t)0x40000000, (int32_t)0x9126145F, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, 
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, 
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, 
    (int32_t)0xD8722192, (int32_t)0x8643C7B3, 
    (int32_t)0xC0000000, (int32_t)0x9126145F, 
    (int32_t)0xAA59EDA4, (int32_t)0xA0E0A15F, 
    (int32_t)0x98722192, (int32_t)0xB4C373EE, 
    (int32_t)0x8B10F144, (int32_t)0xCBF00DBE, 
    (int32_t)0x82CC0F36, (int32_t)0xE5632654, 
};

/****************** stage 4 radix 3 ******************/
ALIGN(8) static const int32_t __fft480_tw4[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, 
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, 
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC, 
};
#define N 480
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT2_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT2_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT2_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT2_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT5_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 1, 1, 
}; 
static const cint32ptr_t tw_tab[] = 
{
    __fft480_tw1, __fft480_tw2, __fft480_tw3, __fft480_tw4, NULL
};
 const fft_cplx32x32_descr_t __cfft_descr480_32x32 =
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
};     
 const fft_cplx32x32_descr_t __cifft_descr480_32x32 =
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_480 = (const fft_handle_t)&__cfft_descr480_32x32;
const fft_handle_t cinfft32_480 = (const fft_handle_t)&__cifft_descr480_32x32;
