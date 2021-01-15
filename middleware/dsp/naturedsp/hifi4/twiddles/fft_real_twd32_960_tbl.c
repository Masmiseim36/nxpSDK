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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFF4C54, (int32_t)0xFF298915, (int32_t)0x7FFD3154, (int32_t)0xFE531484, (int32_t)0x7FF9AF04, (int32_t)0xFD7CA4A6,
    (int32_t)0x7FF4C56F, (int32_t)0xFCA63BD8, (int32_t)0x7FEE74A2, (int32_t)0xFBCFDC71, (int32_t)0x7FE6BCB0, (int32_t)0xFAF988CC, (int32_t)0x7FDD9DAD, (int32_t)0xFA234342,
    (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, (int32_t)0x7FC72AE2, (int32_t)0xF876EBE8, (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7FAB1D3D, (int32_t)0xF6CAE92C,
    (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67, (int32_t)0x7F8975F9, (int32_t)0xF51F4DD4, (int32_t)0x7F76892F, (int32_t)0xF449ACCA, (int32_t)0x7F62368F, (int32_t)0xF3742CA2,
    (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7F3560B9, (int32_t)0xF1C99853, (int32_t)0x7F1CDE01, (int32_t)0xF0F488D9, (int32_t)0x7F02F66F, (int32_t)0xF01FA39C,
    (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7ECAF9E5, (int32_t)0xEE76612D, (int32_t)0x7EACE58A, (int32_t)0xEDA208A5, (int32_t)0x7E8D6D91, (int32_t)0xECCDE3AD,
    (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x7E4A5426, (int32_t)0xEB263DBB, (int32_t)0x7E26B371, (int32_t)0xEA52C166, (int32_t)0x7E01B096, (int32_t)0xE97F81EB,
    (int32_t)0x7DDB4BFC, (int32_t)0xE8AC819D, (int32_t)0x7DB3860F, (int32_t)0xE7D9C2CA, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7D5FD801, (int32_t)0xE63512D7,
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x7D06AA16, (int32_t)0xE4918486, (int32_t)0x7CD80464, (int32_t)0xE3C02FBB, (int32_t)0x7CA80038, (int32_t)0xE2EF2A3E,
    (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x7C43DE8E, (int32_t)0xE14E1659, (int32_t)0x7C0FC22A, (int32_t)0xE07E0C84, (int32_t)0x7BDA497D, (int32_t)0xDFAE5B23,
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7B6B45A5, (int32_t)0xDE100AD8, (int32_t)0x7B31BBB2, (int32_t)0xDD417079, (int32_t)0x7AF6D7E6, (int32_t)0xDC7337A3,
    (int32_t)0x7ABA9AE6, (int32_t)0xDBA5629B, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x7A3E17F2, (int32_t)0xDA0AECF9, (int32_t)0x79FDD35C, (int32_t)0xD93E50DE,
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x7979477D, (int32_t)0xD7A66150, (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x78EF678F, (int32_t)0xD61036DB,
    (int32_t)0x78A879F4, (int32_t)0xD545D11C, (int32_t)0x7860399E, (int32_t)0xD47BE351, (int32_t)0x7816A759, (int32_t)0xD3B26FB0, (int32_t)0x77CBC3F2, (int32_t)0xD2E9786E,
    (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x77320D0D, (int32_t)0xD15907D9, (int32_t)0x76E33B3F, (int32_t)0xD09192EA, (int32_t)0x76931BAE, (int32_t)0xCFCAA323,
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x75EEF6CE, (int32_t)0xCE3E5BC5, (int32_t)0x759AF34C, (int32_t)0xCD790887, (int32_t)0x7545A5A0, (int32_t)0xCCB44322,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x74972F92, (int32_t)0xCB2C6A82, (int32_t)0x743E0918, (int32_t)0xCA695B94, (int32_t)0x73E39C49, (int32_t)0xC9A6E316,
    (int32_t)0x7387EA23, (int32_t)0xC8E5032B, (int32_t)0x732AF3A7, (int32_t)0xC823BDF4, (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x726D3DC6, (int32_t)0xC6A30C17,
    (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x71AA82F7, (int32_t)0xC524DE5F, (int32_t)0x71474660, (int32_t)0xC466BE4F, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590,
    (int32_t)0x707D1443, (int32_t)0xC2EC7635, (int32_t)0x701620F5, (int32_t)0xC2305250, (int32_t)0x6FADF2FC, (int32_t)0xC174DBF2, (int32_t)0x6F448B7E, (int32_t)0xC0BA1528,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6E6E1492, (int32_t)0xBF469E83, (int32_t)0x6E010780, (int32_t)0xBE8DF2BA, (int32_t)0x6D92C59B, (int32_t)0xBDD5FEAC,
    (int32_t)0x6D23501B, (int32_t)0xBD1EC45C, (int32_t)0x6CB2A837, (int32_t)0xBC6845CE, (int32_t)0x6C40CF2C, (int32_t)0xBBB28501, (int32_t)0x6BCDC639, (int32_t)0xBAFD83F4,
    (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x6AE429AE, (int32_t)0xB995C907, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x69F5DCD3, (int32_t)0xB83124CF,
    (int32_t)0x697CF78A, (int32_t)0xB780001C, (int32_t)0x6902EA1D, (int32_t)0xB6CFA6F1, (int32_t)0x6887B5E2, (int32_t)0xB6201B3E, (int32_t)0x680B5C33, (int32_t)0xB5715EEF,
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x670F3DF3, (int32_t)0xB4165C25, (int32_t)0x668F7C25, (int32_t)0xB36A1978, (int32_t)0x660E9A6A, (int32_t)0xB2BEADCC,
    (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x65097CDB, (int32_t)0xB16A62F8, (int32_t)0x648543E4, (int32_t)0xB0C1878B, (int32_t)0x63FFF0BA, (int32_t)0xB0198A96,
    (int32_t)0x637984D4, (int32_t)0xAF726DEF, (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x626968BE, (int32_t)0xAE26DCDF, (int32_t)0x61DFBB8A, (int32_t)0xAD826C1A,
    (int32_t)0x6154FB91, (int32_t)0xACDEE2E8, (int32_t)0x60C92A5A, (int32_t)0xAC3C4316, (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x5FAE5A55, (int32_t)0xAAF9C6AF,
    (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x5E8F57E2, (int32_t)0xA9BB050C, (int32_t)0x5DFE47AD, (int32_t)0xA91D0EA3, (int32_t)0x5D6C2F99, (int32_t)0xA8800C26,
    (int32_t)0x5CD91140, (int32_t)0xA7E3FF4D, (int32_t)0x5C44EE40, (int32_t)0xA748E9CE, (int32_t)0x5BAFC837, (int32_t)0xA6AECD5E, (int32_t)0x5B19A0C8, (int32_t)0xA615ABAC,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x59EA5454, (int32_t)0xA4E65F38, (int32_t)0x595132A2, (int32_t)0xA45037C9, (int32_t)0x58B71632, (int32_t)0xA3BB11C0,
    (int32_t)0x581C00B3, (int32_t)0xA326EEC0, (int32_t)0x577FF3DA, (int32_t)0xA293D067, (int32_t)0x56E2F15D, (int32_t)0xA201B853, (int32_t)0x5644FAF4, (int32_t)0xA170A81E,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x55063951, (int32_t)0xA051A5AB, (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0x53C3BCEA, (int32_t)0x9F36D5A6,
    (int32_t)0x53211D18, (int32_t)0x9EAB046F, (int32_t)0x527D93E6, (int32_t)0x9E204476, (int32_t)0x51D92321, (int32_t)0x9D969742, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54,
    (int32_t)0x508D9211, (int32_t)0x9C867B2C, (int32_t)0x4FE6756A, (int32_t)0x9C000F46, (int32_t)0x4F3E7875, (int32_t)0x9B7ABC1C, (int32_t)0x4E959D08, (int32_t)0x9AF68325,
    (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x4D415234, (int32_t)0x99F16596, (int32_t)0x4C95E688, (int32_t)0x997083DB, (int32_t)0x4BE9A3DB, (int32_t)0x98F0C20D,
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x4A8EA111, (int32_t)0x97F4A3CD, (int32_t)0x49DFE4C2, (int32_t)0x97784A1E, (int32_t)0x4930590F, (int32_t)0x96FD15E3,
    (int32_t)0x487FFFE4, (int32_t)0x96830876, (int32_t)0x47CEDB31, (int32_t)0x960A232D, (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x466A36F9, (int32_t)0x951BD652,
    (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0x45027C0C, (int32_t)0x943239C7, (int32_t)0x444D7AFF, (int32_t)0x93BF30D4, (int32_t)0x4397BA32, (int32_t)0x934D57C9,
    (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5, (int32_t)0x422A0154, (int32_t)0x926D3A65, (int32_t)0x41720D46, (int32_t)0x91FEF880, (int32_t)0x40B9617D, (int32_t)0x9191EB6E,
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x3F45EAD8, (int32_t)0x90BB7482, (int32_t)0x3E8B240E, (int32_t)0x90520D04, (int32_t)0x3DCFADB0, (int32_t)0x8FE9DF0B,
    (int32_t)0x3D1389CB, (int32_t)0x8F82EBBD, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0x3B9941B1, (int32_t)0x8EB8B9A0, (int32_t)0x3ADB21A1, (int32_t)0x8E557D09,
    (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0x395CF3E9, (int32_t)0x8D92C23A, (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0x37DC420C, (int32_t)0x8CD50C59,
    (int32_t)0x371AFCD5, (int32_t)0x8C7815DD, (int32_t)0x36591CEA, (int32_t)0x8C1C63B7, (int32_t)0x3596A46C, (int32_t)0x8BC1F6E8, (int32_t)0x34D3957E, (int32_t)0x8B68D06E,
    (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0x334BBCDE, (int32_t)0x8ABA5A60, (int32_t)0x3286F779, (int32_t)0x8A650CB4, (int32_t)0x31C1A43B, (int32_t)0x8A110932,
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x30355CDD, (int32_t)0x896CE452, (int32_t)0x2F6E6D16, (int32_t)0x891CC4C1, (int32_t)0x2EA6F827, (int32_t)0x88CDF2F3,
    (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0x2D168792, (int32_t)0x88343C0E, (int32_t)0x2C4D9050, (int32_t)0x87E958A7, (int32_t)0x2B841CAF, (int32_t)0x879FC662,
    (int32_t)0x2ABA2EE4, (int32_t)0x8757860C, (int32_t)0x29EFC925, (int32_t)0x87109871, (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x28599EB0, (int32_t)0x8686B883,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x26C1AF22, (int32_t)0x86022CA4, (int32_t)0x25F51307, (int32_t)0x85C1E80E, (int32_t)0x25280C5E, (int32_t)0x8582FAA5,
    (int32_t)0x245A9D65, (int32_t)0x8545651A, (int32_t)0x238CC85D, (int32_t)0x8509281A, (int32_t)0x22BE8F87, (int32_t)0x84CE444E, (int32_t)0x21EFF528, (int32_t)0x8494BA5B,
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x2051A4DD, (int32_t)0x8425B683, (int32_t)0x1F81F37C, (int32_t)0x83F03DD6, (int32_t)0x1EB1E9A7, (int32_t)0x83BC2172,
    (int32_t)0x1DE189A6, (int32_t)0x838961E8, (int32_t)0x1D10D5C2, (int32_t)0x8357FFC8, (int32_t)0x1C3FD045, (int32_t)0x8327FB9C, (int32_t)0x1B6E7B7A, (int32_t)0x82F955EA,
    (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x19CAED29, (int32_t)0x82A027FF, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x18263D36, (int32_t)0x824C79F1,
    (int32_t)0x17537E63, (int32_t)0x8224B404, (int32_t)0x16807E15, (int32_t)0x81FE4F6A, (int32_t)0x15AD3E9A, (int32_t)0x81D94C8F, (int32_t)0x14D9C245, (int32_t)0x81B5ABDA,
    (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x13321C53, (int32_t)0x8172926F, (int32_t)0x125DF75B, (int32_t)0x81531A76, (int32_t)0x11899ED3, (int32_t)0x8135061B,
    (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x0FE05C64, (int32_t)0x80FD0991, (int32_t)0x0F0B7727, (int32_t)0x80E321FF, (int32_t)0x0E3667AD, (int32_t)0x80CA9F47,
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x0BB65336, (int32_t)0x808976D1, (int32_t)0x0AE0B22C, (int32_t)0x80768A07,
    (int32_t)0x0A0AF299, (int32_t)0x80650347, (int32_t)0x093516D4, (int32_t)0x8054E2C3, (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x07891418, (int32_t)0x8038D51E,
    (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, (int32_t)0x05DCBCBE, (int32_t)0x80226253, (int32_t)0x05067734, (int32_t)0x80194350, (int32_t)0x0430238F, (int32_t)0x80118B5E,
    (int32_t)0x0359C428, (int32_t)0x800B3A91, (int32_t)0x02835B5A, (int32_t)0x800650FC, (int32_t)0x01ACEB7C, (int32_t)0x8002CEAC, (int32_t)0x00D676EB, (int32_t)0x8000B3AC,
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr480_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr480_32x32,
    __fft_real_tw
};

const fft_handle_t  rnfft32_960 = (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_960 = (const fft_handle_t)&__rifft_descr;
