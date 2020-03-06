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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFD315, (int32_t)0xFF94C465, (int32_t)0x7FFF4C54, (int32_t)0xFF298915, (int32_t)0x7FFE6BBF, (int32_t)0xFEBE4E5B,
    (int32_t)0x7FFD3154, (int32_t)0xFE531484, (int32_t)0x7FFB9D15, (int32_t)0xFDE7DBD9, (int32_t)0x7FF9AF04, (int32_t)0xFD7CA4A6, (int32_t)0x7FF76721, (int32_t)0xFD116F38,
    (int32_t)0x7FF4C56F, (int32_t)0xFCA63BD8, (int32_t)0x7FF1C9EF, (int32_t)0xFC3B0AD1, (int32_t)0x7FEE74A2, (int32_t)0xFBCFDC71, (int32_t)0x7FEAC58D, (int32_t)0xFB64B100,
    (int32_t)0x7FE6BCB0, (int32_t)0xFAF988CC, (int32_t)0x7FE25A0F, (int32_t)0xFA8E641E, (int32_t)0x7FDD9DAD, (int32_t)0xFA234342, (int32_t)0x7FD8878E, (int32_t)0xF9B82684,
    (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, (int32_t)0x7FCD4E24, (int32_t)0xF8E1FA8B, (int32_t)0x7FC72AE2, (int32_t)0xF876EBE8, (int32_t)0x7FC0ADF2, (int32_t)0xF80BE28E,
    (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7FB2A71B, (int32_t)0xF735E0E5, (int32_t)0x7FAB1D3D, (int32_t)0xF6CAE92C, (int32_t)0x7FA339C5, (int32_t)0xF65FF7E9,
    (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67, (int32_t)0x7F92661D, (int32_t)0xF58A29F2, (int32_t)0x7F8975F9, (int32_t)0xF51F4DD4, (int32_t)0x7F802C52, (int32_t)0xF4B47958,
    (int32_t)0x7F76892F, (int32_t)0xF449ACCA, (int32_t)0x7F6C8C96, (int32_t)0xF3DEE874, (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7F578721, (int32_t)0xF309799E,
    (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7F411C2F, (int32_t)0xF2342F2B, (int32_t)0x7F3560B9, (int32_t)0xF1C99853, (int32_t)0x7F294BFD, (int32_t)0xF15F0B74,
    (int32_t)0x7F1CDE01, (int32_t)0xF0F488D9, (int32_t)0x7F1016CE, (int32_t)0xF08A10CD, (int32_t)0x7F02F66F, (int32_t)0xF01FA39C, (int32_t)0x7EF57CEA, (int32_t)0xEFB5418F,
    (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7ED97E9C, (int32_t)0xEEE0A00C, (int32_t)0x7ECAF9E5, (int32_t)0xEE76612D, (int32_t)0x7EBC1C31, (int32_t)0xEE0C2E9C,
    (int32_t)0x7EACE58A, (int32_t)0xEDA208A5, (int32_t)0x7E9D55FC, (int32_t)0xED37EF91, (int32_t)0x7E8D6D91, (int32_t)0xECCDE3AD, (int32_t)0x7E7D2C54, (int32_t)0xEC63E541,
    (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x7E5B9F93, (int32_t)0xEB9011FD, (int32_t)0x7E4A5426, (int32_t)0xEB263DBB, (int32_t)0x7E38B017, (int32_t)0xEABC781A,
    (int32_t)0x7E26B371, (int32_t)0xEA52C166, (int32_t)0x7E145E42, (int32_t)0xE9E919E8, (int32_t)0x7E01B096, (int32_t)0xE97F81EB, (int32_t)0x7DEEAA7A, (int32_t)0xE915F9BA,
    (int32_t)0x7DDB4BFC, (int32_t)0xE8AC819D, (int32_t)0x7DC79529, (int32_t)0xE84319DF, (int32_t)0x7DB3860F, (int32_t)0xE7D9C2CA, (int32_t)0x7D9F1EBD, (int32_t)0xE7707CA9,
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7D7547A7, (int32_t)0xE69E2465, (int32_t)0x7D5FD801, (int32_t)0xE63512D7, (int32_t)0x7D4A105D, (int32_t)0xE5CC1364,
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x7D1D7958, (int32_t)0xE4FA4BF1, (int32_t)0x7D06AA16, (int32_t)0xE4918486, (int32_t)0x7CEF8315, (int32_t)0xE428D05C,
    (int32_t)0x7CD80464, (int32_t)0xE3C02FBB, (int32_t)0x7CC02E15, (int32_t)0xE357A2EE, (int32_t)0x7CA80038, (int32_t)0xE2EF2A3E, (int32_t)0x7C8F7ADE, (int32_t)0xE286C5F5,
    (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x7C5D69F7, (int32_t)0xE1B63BB9, (int32_t)0x7C43DE8E, (int32_t)0xE14E1659, (int32_t)0x7C29FBEE, (int32_t)0xE0E60685,
    (int32_t)0x7C0FC22A, (int32_t)0xE07E0C84, (int32_t)0x7BF53153, (int32_t)0xE01628A1, (int32_t)0x7BDA497D, (int32_t)0xDFAE5B23, (int32_t)0x7BBF0ABA, (int32_t)0xDF46A454,
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7B8788BA, (int32_t)0xDE777BE6, (int32_t)0x7B6B45A5, (int32_t)0xDE100AD8, (int32_t)0x7B4EABF1, (int32_t)0xDDA8B19B,
    (int32_t)0x7B31BBB2, (int32_t)0xDD417079, (int32_t)0x7B1474FD, (int32_t)0xDCDA47B9, (int32_t)0x7AF6D7E6, (int32_t)0xDC7337A3, (int32_t)0x7AD8E482, (int32_t)0xDC0C4082,
    (int32_t)0x7ABA9AE6, (int32_t)0xDBA5629B, (int32_t)0x7A9BFB27, (int32_t)0xDB3E9E39, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x7A5DB997, (int32_t)0xDA716320,
    (int32_t)0x7A3E17F2, (int32_t)0xDA0AECF9, (int32_t)0x7A1E2082, (int32_t)0xD9A49176, (int32_t)0x79FDD35C, (int32_t)0xD93E50DE, (int32_t)0x79DD3098, (int32_t)0xD8D82B7A,
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x799AEA92, (int32_t)0xD80C336C, (int32_t)0x7979477D, (int32_t)0xD7A66150, (int32_t)0x79574F28, (int32_t)0xD740AB85,
    (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x79125F19, (int32_t)0xD6759604, (int32_t)0x78EF678F, (int32_t)0xD61036DB, (int32_t)0x78CC1B26, (int32_t)0xD5AAF521,
    (int32_t)0x78A879F4, (int32_t)0xD545D11C, (int32_t)0x78848414, (int32_t)0xD4E0CB15, (int32_t)0x7860399E, (int32_t)0xD47BE351, (int32_t)0x783B9AAD, (int32_t)0xD4171A18,
    (int32_t)0x7816A759, (int32_t)0xD3B26FB0, (int32_t)0x77F15FBC, (int32_t)0xD34DE460, (int32_t)0x77CBC3F2, (int32_t)0xD2E9786E, (int32_t)0x77A5D413, (int32_t)0xD2852C22,
    (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x7758F886, (int32_t)0xD1BCF391, (int32_t)0x77320D0D, (int32_t)0xD15907D9, (int32_t)0x770ACDEC, (int32_t)0xD0F53CE0,
    (int32_t)0x76E33B3F, (int32_t)0xD09192EA, (int32_t)0x76BB5521, (int32_t)0xD02E0A3F, (int32_t)0x76931BAE, (int32_t)0xCFCAA323, (int32_t)0x766A8F04, (int32_t)0xCF675DDD,
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x76187C77, (int32_t)0xCEA139E9, (int32_t)0x75EEF6CE, (int32_t)0xCE3E5BC5, (int32_t)0x75C51E61, (int32_t)0xCDDBA08E,
    (int32_t)0x759AF34C, (int32_t)0xCD790887, (int32_t)0x757075AC, (int32_t)0xCD1693F7, (int32_t)0x7545A5A0, (int32_t)0xCCB44322, (int32_t)0x751A8346, (int32_t)0xCC52164D,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x74C34820, (int32_t)0xCB8E29B9, (int32_t)0x74972F92, (int32_t)0xCB2C6A82, (int32_t)0x746AC52F, (int32_t)0xCACAD05F,
    (int32_t)0x743E0918, (int32_t)0xCA695B94, (int32_t)0x7410FB6B, (int32_t)0xCA080C64, (int32_t)0x73E39C49, (int32_t)0xC9A6E316, (int32_t)0x73B5EBD1, (int32_t)0xC945DFEC,
    (int32_t)0x7387EA23, (int32_t)0xC8E5032B, (int32_t)0x73599760, (int32_t)0xC8844D17, (int32_t)0x732AF3A7, (int32_t)0xC823BDF4, (int32_t)0x72FBFF1B, (int32_t)0xC7C35605,
    (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x729D2409, (int32_t)0xC702FCD3, (int32_t)0x726D3DC6, (int32_t)0xC6A30C17, (int32_t)0x723D0734, (int32_t)0xC643439D,
    (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x71DBA9AB, (int32_t)0xC5842C7E, (int32_t)0x71AA82F7, (int32_t)0xC524DE5F, (int32_t)0x71790C7E, (int32_t)0xC4C5B98E,
    (int32_t)0x71474660, (int32_t)0xC466BE4F, (int32_t)0x711530C2, (int32_t)0xC407ECE4, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x70B01790, (int32_t)0xC34AC895,
    (int32_t)0x707D1443, (int32_t)0xC2EC7635, (int32_t)0x7049C203, (int32_t)0xC28E4EB3, (int32_t)0x701620F5, (int32_t)0xC2305250, (int32_t)0x6FE2313C, (int32_t)0xC1D2814F,
    (int32_t)0x6FADF2FC, (int32_t)0xC174DBF2, (int32_t)0x6F79665B, (int32_t)0xC117627A, (int32_t)0x6F448B7E, (int32_t)0xC0BA1528, (int32_t)0x6F0F6289, (int32_t)0xC05CF43F,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6EA426ED, (int32_t)0xBFA338AC, (int32_t)0x6E6E1492, (int32_t)0xBF469E83, (int32_t)0x6E37B4B6, (int32_t)0xBEEA31C8,
    (int32_t)0x6E010780, (int32_t)0xBE8DF2BA, (int32_t)0x6DCA0D14, (int32_t)0xBE31E19B, (int32_t)0x6D92C59B, (int32_t)0xBDD5FEAC, (int32_t)0x6D5B313B, (int32_t)0xBD7A4A2C,
    (int32_t)0x6D23501B, (int32_t)0xBD1EC45C, (int32_t)0x6CEB2261, (int32_t)0xBCC36D7D, (int32_t)0x6CB2A837, (int32_t)0xBC6845CE, (int32_t)0x6C79E1C2, (int32_t)0xBC0D4D8F,
    (int32_t)0x6C40CF2C, (int32_t)0xBBB28501, (int32_t)0x6C07709B, (int32_t)0xBB57EC62, (int32_t)0x6BCDC639, (int32_t)0xBAFD83F4, (int32_t)0x6B93D02E, (int32_t)0xBAA34BF4,
    (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x6B1F01C0, (int32_t)0xB9EF6E3E, (int32_t)0x6AE429AE, (int32_t)0xB995C907, (int32_t)0x6AA90697, (int32_t)0xB93C553B,
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x6A31E000, (int32_t)0xB88A02E1, (int32_t)0x69F5DCD3, (int32_t)0xB83124CF, (int32_t)0x69B98F48, (int32_t)0xB7D87924,
    (int32_t)0x697CF78A, (int32_t)0xB780001C, (int32_t)0x694015C3, (int32_t)0xB727B9F7, (int32_t)0x6902EA1D, (int32_t)0xB6CFA6F1, (int32_t)0x68C574C4, (int32_t)0xB677C74A,
    (int32_t)0x6887B5E2, (int32_t)0xB6201B3E, (int32_t)0x6849ADA3, (int32_t)0xB5C8A30B, (int32_t)0x680B5C33, (int32_t)0xB5715EEF, (int32_t)0x67CCC1BE, (int32_t)0xB51A4F26,
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x674EB271, (int32_t)0xB46CCD84, (int32_t)0x670F3DF3, (int32_t)0xB4165C25, (int32_t)0x66CF8120, (int32_t)0xB3C0200C,
    (int32_t)0x668F7C25, (int32_t)0xB36A1978, (int32_t)0x664F2F2E, (int32_t)0xB31448A4, (int32_t)0x660E9A6A, (int32_t)0xB2BEADCC, (int32_t)0x65CDBE05, (int32_t)0xB269492D,
    (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x654B2F10, (int32_t)0xB1BF2387, (int32_t)0x65097CDB, (int32_t)0xB16A62F8, (int32_t)0x64C783BD, (int32_t)0xB115D990,
    (int32_t)0x648543E4, (int32_t)0xB0C1878B, (int32_t)0x6442BD7E, (int32_t)0xB06D6D24, (int32_t)0x63FFF0BA, (int32_t)0xB0198A96, (int32_t)0x63BCDDC7, (int32_t)0xAFC5E01B,
    (int32_t)0x637984D4, (int32_t)0xAF726DEF, (int32_t)0x6335E611, (int32_t)0xAF1F344C, (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x62ADD7D6, (int32_t)0xAE796B8A,
    (int32_t)0x626968BE, (int32_t)0xAE26DCDF, (int32_t)0x6224B495, (int32_t)0xADD487A7, (int32_t)0x61DFBB8A, (int32_t)0xAD826C1A, (int32_t)0x619A7DCE, (int32_t)0xAD308A71,
    (int32_t)0x6154FB91, (int32_t)0xACDEE2E8, (int32_t)0x610F3505, (int32_t)0xAC8D75B6, (int32_t)0x60C92A5A, (int32_t)0xAC3C4316, (int32_t)0x6082DBC1, (int32_t)0xABEB4B3F,
    (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x5FF5738D, (int32_t)0xAB4A0CD4, (int32_t)0x5FAE5A55, (int32_t)0xAAF9C6AF, (int32_t)0x5F66FDF5, (int32_t)0xAAA9BC38,
    (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x5ED77C8A, (int32_t)0xAA0A5B2E, (int32_t)0x5E8F57E2, (int32_t)0xA9BB050C, (int32_t)0x5E46F0DD, (int32_t)0xA96BEB75,
    (int32_t)0x5DFE47AD, (int32_t)0xA91D0EA3, (int32_t)0x5DB55C86, (int32_t)0xA8CE6ECB, (int32_t)0x5D6C2F99, (int32_t)0xA8800C26, (int32_t)0x5D22C11C, (int32_t)0xA831E6E9,
    (int32_t)0x5CD91140, (int32_t)0xA7E3FF4D, (int32_t)0x5C8F203B, (int32_t)0xA7965587, (int32_t)0x5C44EE40, (int32_t)0xA748E9CE, (int32_t)0x5BFA7B82, (int32_t)0xA6FBBC59,
    (int32_t)0x5BAFC837, (int32_t)0xA6AECD5E, (int32_t)0x5B64D492, (int32_t)0xA6621D12, (int32_t)0x5B19A0C8, (int32_t)0xA615ABAC, (int32_t)0x5ACE2D0F, (int32_t)0xA5C97961,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x5A36869F, (int32_t)0xA531D2F1, (int32_t)0x59EA5454, (int32_t)0xA4E65F38, (int32_t)0x599DE2EE, (int32_t)0xA49B2B6E,
    (int32_t)0x595132A2, (int32_t)0xA45037C9, (int32_t)0x590443A7, (int32_t)0xA405847E, (int32_t)0x58B71632, (int32_t)0xA3BB11C0, (int32_t)0x5869AA79, (int32_t)0xA370DFC5,
    (int32_t)0x581C00B3, (int32_t)0xA326EEC0, (int32_t)0x57CE1917, (int32_t)0xA2DD3EE4, (int32_t)0x577FF3DA, (int32_t)0xA293D067, (int32_t)0x57319135, (int32_t)0xA24AA37A,
    (int32_t)0x56E2F15D, (int32_t)0xA201B853, (int32_t)0x5694148B, (int32_t)0xA1B90F23, (int32_t)0x5644FAF4, (int32_t)0xA170A81E, (int32_t)0x55F5A4D2, (int32_t)0xA1288376,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x555643C8, (int32_t)0xA099020B, (int32_t)0x55063951, (int32_t)0xA051A5AB, (int32_t)0x54B5F32C, (int32_t)0xA00A8C73,
    (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0x5414B4C1, (int32_t)0x9F7D243F, (int32_t)0x53C3BCEA, (int32_t)0x9F36D5A6, (int32_t)0x53728A4A, (int32_t)0x9EF0CAFB,
    (int32_t)0x53211D18, (int32_t)0x9EAB046F, (int32_t)0x52CF758F, (int32_t)0x9E658232, (int32_t)0x527D93E6, (int32_t)0x9E204476, (int32_t)0x522B7859, (int32_t)0x9DDB4B6B,
    (int32_t)0x51D92321, (int32_t)0x9D969742, (int32_t)0x51869476, (int32_t)0x9D52282A, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0x50E0CBB4, (int32_t)0x9CCA19EF,
    (int32_t)0x508D9211, (int32_t)0x9C867B2C, (int32_t)0x503A1FE5, (int32_t)0x9C432239, (int32_t)0x4FE6756A, (int32_t)0x9C000F46, (int32_t)0x4F9292DC, (int32_t)0x9BBD4282,
    (int32_t)0x4F3E7875, (int32_t)0x9B7ABC1C, (int32_t)0x4EEA2670, (int32_t)0x9B387C43, (int32_t)0x4E959D08, (int32_t)0x9AF68325, (int32_t)0x4E40DC79, (int32_t)0x9AB4D0F0,
    (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x4D96B6D3, (int32_t)0x9A3241FB, (int32_t)0x4D415234, (int32_t)0x99F16596, (int32_t)0x4CEBB75C, (int32_t)0x99B0D0D2,
    (int32_t)0x4C95E688, (int32_t)0x997083DB, (int32_t)0x4C3FDFF4, (int32_t)0x99307EE0, (int32_t)0x4BE9A3DB, (int32_t)0x98F0C20D, (int32_t)0x4B93327C, (int32_t)0x98B14D8F,
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x4AE5B0DA, (int32_t)0x98333E42, (int32_t)0x4A8EA111, (int32_t)0x97F4A3CD, (int32_t)0x4A375CF5, (int32_t)0x97B6525D,
    (int32_t)0x49DFE4C2, (int32_t)0x97784A1E, (int32_t)0x498838B6, (int32_t)0x973A8B3C, (int32_t)0x4930590F, (int32_t)0x96FD15E3, (int32_t)0x48D84609, (int32_t)0x96BFEA3D,
    (int32_t)0x487FFFE4, (int32_t)0x96830876, (int32_t)0x482786DC, (int32_t)0x964670B8, (int32_t)0x47CEDB31, (int32_t)0x960A232D, (int32_t)0x4775FD1F, (int32_t)0x95CE2000,
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x46C3AAC5, (int32_t)0x9556F969, (int32_t)0x466A36F9, (int32_t)0x951BD652, (int32_t)0x461091C2, (int32_t)0x94E0FE40,
    (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0x455CB40C, (int32_t)0x946C2FD2, (int32_t)0x45027C0C, (int32_t)0x943239C7, (int32_t)0x44A8139E, (int32_t)0x93F88F65,
    (int32_t)0x444D7AFF, (int32_t)0x93BF30D4, (int32_t)0x43F2B271, (int32_t)0x93861E3E, (int32_t)0x4397BA32, (int32_t)0x934D57C9, (int32_t)0x433C9283, (int32_t)0x9314DD9F,
    (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5, (int32_t)0x4285B5D4, (int32_t)0x92A4CEC5, (int32_t)0x422A0154, (int32_t)0x926D3A65, (int32_t)0x41CE1E65, (int32_t)0x9235F2EC,
    (int32_t)0x41720D46, (int32_t)0x91FEF880, (int32_t)0x4115CE38, (int32_t)0x91C84B4A, (int32_t)0x40B9617D, (int32_t)0x9191EB6E, (int32_t)0x405CC754, (int32_t)0x915BD913,
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x3FA30BC1, (int32_t)0x90F09D77, (int32_t)0x3F45EAD8, (int32_t)0x90BB7482, (int32_t)0x3EE89D86, (int32_t)0x908699A5,
    (int32_t)0x3E8B240E, (int32_t)0x90520D04, (int32_t)0x3E2D7EB1, (int32_t)0x901DCEC4, (int32_t)0x3DCFADB0, (int32_t)0x8FE9DF0B, (int32_t)0x3D71B14D, (int32_t)0x8FB63DFD,
    (int32_t)0x3D1389CB, (int32_t)0x8F82EBBD, (int32_t)0x3CB5376B, (int32_t)0x8F4FE870, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0x3BF8131C, (int32_t)0x8EEACF3E,
    (int32_t)0x3B9941B1, (int32_t)0x8EB8B9A0, (int32_t)0x3B3A4672, (int32_t)0x8E86F382, (int32_t)0x3ADB21A1, (int32_t)0x8E557D09, (int32_t)0x3A7BD382, (int32_t)0x8E245655,
    (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0x39BCBC63, (int32_t)0x8DC2F8CC, (int32_t)0x395CF3E9, (int32_t)0x8D92C23A, (int32_t)0x38FD032D, (int32_t)0x8D62DBF7,
    (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0x383CA9FB, (int32_t)0x8D0400E5, (int32_t)0x37DC420C, (int32_t)0x8CD50C59, (int32_t)0x377BB2E9, (int32_t)0x8CA668A0,
    (int32_t)0x371AFCD5, (int32_t)0x8C7815DD, (int32_t)0x36BA2014, (int32_t)0x8C4A142F, (int32_t)0x36591CEA, (int32_t)0x8C1C63B7, (int32_t)0x35F7F39C, (int32_t)0x8BEF0495,
    (int32_t)0x3596A46C, (int32_t)0x8BC1F6E8, (int32_t)0x35352FA1, (int32_t)0x8B953AD1, (int32_t)0x34D3957E, (int32_t)0x8B68D06E, (int32_t)0x3471D647, (int32_t)0x8B3CB7E0,
    (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0x33ADE9B3, (int32_t)0x8AE57CBA, (int32_t)0x334BBCDE, (int32_t)0x8ABA5A60, (int32_t)0x32E96C09, (int32_t)0x8A8F8A54,
    (int32_t)0x3286F779, (int32_t)0x8A650CB4, (int32_t)0x32245F72, (int32_t)0x8A3AE19F, (int32_t)0x31C1A43B, (int32_t)0x8A110932, (int32_t)0x315EC617, (int32_t)0x89E78389,
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x3098A223, (int32_t)0x899570FC, (int32_t)0x30355CDD, (int32_t)0x896CE452, (int32_t)0x2FD1F5C1, (int32_t)0x8944AADF,
    (int32_t)0x2F6E6D16, (int32_t)0x891CC4C1, (int32_t)0x2F0AC320, (int32_t)0x88F53214, (int32_t)0x2EA6F827, (int32_t)0x88CDF2F3, (int32_t)0x2E430C6F, (int32_t)0x88A7077A,
    (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0x2D7AD3DE, (int32_t)0x885A2BED, (int32_t)0x2D168792, (int32_t)0x88343C0E, (int32_t)0x2CB21BA0, (int32_t)0x880EA044,
    (int32_t)0x2C4D9050, (int32_t)0x87E958A7, (int32_t)0x2BE8E5E8, (int32_t)0x87C46553, (int32_t)0x2B841CAF, (int32_t)0x879FC662, (int32_t)0x2B1F34EB, (int32_t)0x877B7BEC,
    (int32_t)0x2ABA2EE4, (int32_t)0x8757860C, (int32_t)0x2A550ADF, (int32_t)0x8733E4DA, (int32_t)0x29EFC925, (int32_t)0x87109871, (int32_t)0x298A69FC, (int32_t)0x86EDA0E7,
    (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x28BF547B, (int32_t)0x86A8B0D8, (int32_t)0x28599EB0, (int32_t)0x8686B883, (int32_t)0x27F3CC94, (int32_t)0x8665156E,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x2727D486, (int32_t)0x8622CF68, (int32_t)0x26C1AF22, (int32_t)0x86022CA4, (int32_t)0x265B6E8A, (int32_t)0x85E1DF7E,
    (int32_t)0x25F51307, (int32_t)0x85C1E80E, (int32_t)0x258E9CE0, (int32_t)0x85A24669, (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0x24C161C7, (int32_t)0x856404D9,
    (int32_t)0x245A9D65, (int32_t)0x8545651A, (int32_t)0x23F3BF7E, (int32_t)0x85271B7E, (int32_t)0x238CC85D, (int32_t)0x8509281A, (int32_t)0x2325B847, (int32_t)0x84EB8B03,
    (int32_t)0x22BE8F87, (int32_t)0x84CE444E, (int32_t)0x22574E65, (int32_t)0x84B1540F, (int32_t)0x21EFF528, (int32_t)0x8494BA5B, (int32_t)0x2188841A, (int32_t)0x84787746,
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x20B95BAC, (int32_t)0x8440F546, (int32_t)0x2051A4DD, (int32_t)0x8425B683, (int32_t)0x1FE9D75F, (int32_t)0x840ACEAD,
    (int32_t)0x1F81F37C, (int32_t)0x83F03DD6, (int32_t)0x1F19F97B, (int32_t)0x83D60412, (int32_t)0x1EB1E9A7, (int32_t)0x83BC2172, (int32_t)0x1E49C447, (int32_t)0x83A29609,
    (int32_t)0x1DE189A6, (int32_t)0x838961E8, (int32_t)0x1D793A0B, (int32_t)0x83708522, (int32_t)0x1D10D5C2, (int32_t)0x8357FFC8, (int32_t)0x1CA85D12, (int32_t)0x833FD1EB,
    (int32_t)0x1C3FD045, (int32_t)0x8327FB9C, (int32_t)0x1BD72FA4, (int32_t)0x83107CEB, (int32_t)0x1B6E7B7A, (int32_t)0x82F955EA, (int32_t)0x1B05B40F, (int32_t)0x82E286A8,
    (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x1A33EC9C, (int32_t)0x82B5EFA3, (int32_t)0x19CAED29, (int32_t)0x82A027FF, (int32_t)0x1961DB9B, (int32_t)0x828AB859,
    (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x188F8357, (int32_t)0x8260E143, (int32_t)0x18263D36, (int32_t)0x824C79F1, (int32_t)0x17BCE621, (int32_t)0x82386AD7,
    (int32_t)0x17537E63, (int32_t)0x8224B404, (int32_t)0x16EA0646, (int32_t)0x82115586, (int32_t)0x16807E15, (int32_t)0x81FE4F6A, (int32_t)0x1616E618, (int32_t)0x81EBA1BE,
    (int32_t)0x15AD3E9A, (int32_t)0x81D94C8F, (int32_t)0x154387E6, (int32_t)0x81C74FE9, (int32_t)0x14D9C245, (int32_t)0x81B5ABDA, (int32_t)0x146FEE03, (int32_t)0x81A4606D,
    (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x139C1ABF, (int32_t)0x8182D3AC, (int32_t)0x13321C53, (int32_t)0x8172926F, (int32_t)0x12C8106F, (int32_t)0x8162AA04,
    (int32_t)0x125DF75B, (int32_t)0x81531A76, (int32_t)0x11F3D164, (int32_t)0x8143E3CF, (int32_t)0x11899ED3, (int32_t)0x8135061B, (int32_t)0x111F5FF4, (int32_t)0x81268164,
    (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x104ABE71, (int32_t)0x810A8316, (int32_t)0x0FE05C64, (int32_t)0x80FD0991, (int32_t)0x0F75EF33, (int32_t)0x80EFE932,
    (int32_t)0x0F0B7727, (int32_t)0x80E321FF, (int32_t)0x0EA0F48C, (int32_t)0x80D6B403, (int32_t)0x0E3667AD, (int32_t)0x80CA9F47, (int32_t)0x0DCBD0D5, (int32_t)0x80BEE3D1,
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x0CF68662, (int32_t)0x80A878DF, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x0C21178C, (int32_t)0x8093736A,
    (int32_t)0x0BB65336, (int32_t)0x808976D1, (int32_t)0x0B4B86A8, (int32_t)0x807FD3AE, (int32_t)0x0AE0B22C, (int32_t)0x80768A07, (int32_t)0x0A75D60E, (int32_t)0x806D99E3,
    (int32_t)0x0A0AF299, (int32_t)0x80650347, (int32_t)0x09A00817, (int32_t)0x805CC63B, (int32_t)0x093516D4, (int32_t)0x8054E2C3, (int32_t)0x08CA1F1B, (int32_t)0x804D58E5,
    (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x07F41D72, (int32_t)0x803F520E, (int32_t)0x07891418, (int32_t)0x8038D51E, (int32_t)0x071E0575, (int32_t)0x8032B1DC,
    (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, (int32_t)0x0647D97C, (int32_t)0x80277872, (int32_t)0x05DCBCBE, (int32_t)0x80226253, (int32_t)0x05719BE2, (int32_t)0x801DA5F1,
    (int32_t)0x05067734, (int32_t)0x80194350, (int32_t)0x049B4F00, (int32_t)0x80153A73, (int32_t)0x0430238F, (int32_t)0x80118B5E, (int32_t)0x03C4F52F, (int32_t)0x800E3611,
    (int32_t)0x0359C428, (int32_t)0x800B3A91, (int32_t)0x02EE90C8, (int32_t)0x800898DF, (int32_t)0x02835B5A, (int32_t)0x800650FC, (int32_t)0x02182427, (int32_t)0x800462EB,
    (int32_t)0x01ACEB7C, (int32_t)0x8002CEAC, (int32_t)0x0141B1A5, (int32_t)0x80019441, (int32_t)0x00D676EB, (int32_t)0x8000B3AC, (int32_t)0x006B3B9B, (int32_t)0x80002CEB,
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr960_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr960_32x32,
    __fft_real_tw
};

const fft_handle_t rnfft32_1920 = (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_1920 = (const fft_handle_t)&__rifft_descr;
