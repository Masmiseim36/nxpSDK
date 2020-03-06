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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFB9D1, (int32_t)0xFF79F587, (int32_t)0x7FFEE744, (int32_t)0xFEF3EBA0, (int32_t)0x7FFD885A, (int32_t)0xFE6DE2E0,
    (int32_t)0x7FFB9D15, (int32_t)0xFDE7DBD9, (int32_t)0x7FF92577, (int32_t)0xFD61D71E, (int32_t)0x7FF62182, (int32_t)0xFCDBD541, (int32_t)0x7FF2913A, (int32_t)0xFC55D6D6,
    (int32_t)0x7FEE74A2, (int32_t)0xFBCFDC71, (int32_t)0x7FE9CBC0, (int32_t)0xFB49E6A3, (int32_t)0x7FE49698, (int32_t)0xFAC3F5FF, (int32_t)0x7FDED530, (int32_t)0xFA3E0B19,
    (int32_t)0x7FD8878E, (int32_t)0xF9B82684, (int32_t)0x7FD1ADB9, (int32_t)0xF93248D1, (int32_t)0x7FCA47B9, (int32_t)0xF8AC7295, (int32_t)0x7FC25596, (int32_t)0xF826A462,
    (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7FB0CD0A, (int32_t)0xF71B2260, (int32_t)0x7FA736B4, (int32_t)0xF6956FB7, (int32_t)0x7F9D1461, (int32_t)0xF60FC761,
    (int32_t)0x7F92661D, (int32_t)0xF58A29F2, (int32_t)0x7F872BF3, (int32_t)0xF50497FB, (int32_t)0x7F7B65EF, (int32_t)0xF47F120F, (int32_t)0x7F6F141F, (int32_t)0xF3F998C0,
    (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7F54CD4F, (int32_t)0xF2EECE46, (int32_t)0x7F46D86C, (int32_t)0xF2697E3E, (int32_t)0x7F3857F6, (int32_t)0xF1E43D1C,
    (int32_t)0x7F294BFD, (int32_t)0xF15F0B74, (int32_t)0x7F19B491, (int32_t)0xF0D9E9D6, (int32_t)0x7F0991C4, (int32_t)0xF054D8D5, (int32_t)0x7EF8E3A6, (int32_t)0xEFCFD902,
    (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7ED5E5C6, (int32_t)0xEEC60F31, (int32_t)0x7EC3962A, (int32_t)0xEE414656, (int32_t)0x7EB0BB8A, (int32_t)0xEDBC90F0,
    (int32_t)0x7E9D55FC, (int32_t)0xED37EF91, (int32_t)0x7E896595, (int32_t)0xECB362CC, (int32_t)0x7E74EA6A, (int32_t)0xEC2EEB30, (int32_t)0x7E5FE493, (int32_t)0xEBAA894F,
    (int32_t)0x7E4A5426, (int32_t)0xEB263DBB, (int32_t)0x7E34393B, (int32_t)0xEAA20904, (int32_t)0x7E1D93EA, (int32_t)0xEA1DEBBB, (int32_t)0x7E06644C, (int32_t)0xE999E673,
    (int32_t)0x7DEEAA7A, (int32_t)0xE915F9BA, (int32_t)0x7DD6668F, (int32_t)0xE8922622, (int32_t)0x7DBD98A4, (int32_t)0xE80E6C3B, (int32_t)0x7DA440D6, (int32_t)0xE78ACC96,
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7D6FF3FE, (int32_t)0xE683DE53, (int32_t)0x7D54FF2E, (int32_t)0xE60090D6, (int32_t)0x7D3980EC, (int32_t)0xE57D5FDA,
    (int32_t)0x7D1D7958, (int32_t)0xE4FA4BF1, (int32_t)0x7D00E88F, (int32_t)0xE47755AB, (int32_t)0x7CE3CEB2, (int32_t)0xE3F47D96, (int32_t)0x7CC62BDF, (int32_t)0xE371C442,
    (int32_t)0x7CA80038, (int32_t)0xE2EF2A3E, (int32_t)0x7C894BDE, (int32_t)0xE26CB01B, (int32_t)0x7C6A0EF2, (int32_t)0xE1EA5666, (int32_t)0x7C4A4996, (int32_t)0xE1681DAF,
    (int32_t)0x7C29FBEE, (int32_t)0xE0E60685, (int32_t)0x7C09261D, (int32_t)0xE0641176, (int32_t)0x7BE7C847, (int32_t)0xDFE23F11, (int32_t)0x7BC5E290, (int32_t)0xDF608FE4,
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7B808015, (int32_t)0xDE5D9D6B, (int32_t)0x7B5D039E, (int32_t)0xDDDC5B3B, (int32_t)0x7B38FFDE, (int32_t)0xDD5B3E7B,
    (int32_t)0x7B1474FD, (int32_t)0xDCDA47B9, (int32_t)0x7AEF6323, (int32_t)0xDC597781, (int32_t)0x7AC9CA7A, (int32_t)0xDBD8CE63, (int32_t)0x7AA3AB29, (int32_t)0xDB584CE9,
    (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x7A55D93A, (int32_t)0xDA57C31A, (int32_t)0x7A2E26F2, (int32_t)0xD9D7BBDE, (int32_t)0x7A05EEAD, (int32_t)0xD957DE7A,
    (int32_t)0x79DD3098, (int32_t)0xD8D82B7A, (int32_t)0x79B3ECE0, (int32_t)0xD858A36B, (int32_t)0x798A23B1, (int32_t)0xD7D946D8, (int32_t)0x795FD53A, (int32_t)0xD75A164C,
    (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x7909A92D, (int32_t)0xD65C3B7B, (int32_t)0x78DDCBF5, (int32_t)0xD5DD924B, (int32_t)0x78B16A32, (int32_t)0xD55F1750,
    (int32_t)0x78848414, (int32_t)0xD4E0CB15, (int32_t)0x785719CC, (int32_t)0xD462AE23, (int32_t)0x78292B8D, (int32_t)0xD3E4C105, (int32_t)0x77FAB989, (int32_t)0xD3670446,
    (int32_t)0x77CBC3F2, (int32_t)0xD2E9786E, (int32_t)0x779C4AFC, (int32_t)0xD26C1E08, (int32_t)0x776C4EDB, (int32_t)0xD1EEF59E, (int32_t)0x773BCFC4, (int32_t)0xD171FFB8,
    (int32_t)0x770ACDEC, (int32_t)0xD0F53CE0, (int32_t)0x76D94989, (int32_t)0xD078AD9E, (int32_t)0x76A742D1, (int32_t)0xCFFC527B, (int32_t)0x7674B9FA, (int32_t)0xCF802BFF,
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x760E22D1, (int32_t)0xCE887F1E, (int32_t)0x75DA14EF, (int32_t)0xCE0CF9C8, (int32_t)0x75A585CF, (int32_t)0xCD91AB39,
    (int32_t)0x757075AC, (int32_t)0xCD1693F7, (int32_t)0x753AE4C0, (int32_t)0xCC9BB48A, (int32_t)0x7504D345, (int32_t)0xCC210D79, (int32_t)0x74CE4177, (int32_t)0xCBA69F49,
    (int32_t)0x74972F92, (int32_t)0xCB2C6A82, (int32_t)0x745F9DD1, (int32_t)0xCAB26FA9, (int32_t)0x74278C72, (int32_t)0xCA38AF44, (int32_t)0x73EEFBB3, (int32_t)0xC9BF29D9,
    (int32_t)0x73B5EBD1, (int32_t)0xC945DFEC, (int32_t)0x737C5D0B, (int32_t)0xC8CCD203, (int32_t)0x73424FA0, (int32_t)0xC85400A3, (int32_t)0x7307C3D0, (int32_t)0xC7DB6C50,
    (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x72913201, (int32_t)0xC6EAFCE1, (int32_t)0x72552C85, (int32_t)0xC67322CE, (int32_t)0x7218A9A7, (int32_t)0xC5FB87D6,
    (int32_t)0x71DBA9AB, (int32_t)0xC5842C7E, (int32_t)0x719E2CD2, (int32_t)0xC50D1149, (int32_t)0x71603361, (int32_t)0xC49636B9, (int32_t)0x7121BD9C, (int32_t)0xC41F9D50,
    (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x70A35E25, (int32_t)0xC3332FFC, (int32_t)0x706374FF, (int32_t)0xC2BD5D14, (int32_t)0x7023109A, (int32_t)0xC247CD5A,
    (int32_t)0x6FE2313C, (int32_t)0xC1D2814F, (int32_t)0x6FA0D72C, (int32_t)0xC15D7974, (int32_t)0x6F5F02B2, (int32_t)0xC0E8B648, (int32_t)0x6F1CB416, (int32_t)0xC074384C,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6E96A99D, (int32_t)0xBF8C0DE3, (int32_t)0x6E52EE52, (int32_t)0xBF186274, (int32_t)0x6E0EBA0C, (int32_t)0xBEA4FE32,
    (int32_t)0x6DCA0D14, (int32_t)0xBE31E19B, (int32_t)0x6D84E7B7, (int32_t)0xBDBF0D2F, (int32_t)0x6D3F4A40, (int32_t)0xBD4C816A, (int32_t)0x6CF934FC, (int32_t)0xBCDA3ECB,
    (int32_t)0x6CB2A837, (int32_t)0xBC6845CE, (int32_t)0x6C6BA43E, (int32_t)0xBBF696F0, (int32_t)0x6C242960, (int32_t)0xBB8532B0, (int32_t)0x6BDC37EB, (int32_t)0xBB141987,
    (int32_t)0x6B93D02E, (int32_t)0xBAA34BF4, (int32_t)0x6B4AF279, (int32_t)0xBA32CA71, (int32_t)0x6B019F1A, (int32_t)0xB9C29579, (int32_t)0x6AB7D663, (int32_t)0xB952AD88,
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x6A22E630, (int32_t)0xB873C6A6, (int32_t)0x69D7BF57, (int32_t)0xB804C8A9, (int32_t)0x698C246C, (int32_t)0xB796199B,
    (int32_t)0x694015C3, (int32_t)0xB727B9F7, (int32_t)0x68F393AE, (int32_t)0xB6B9AA34, (int32_t)0x68A69E81, (int32_t)0xB64BEACD, (int32_t)0x68593691, (int32_t)0xB5DE7C38,
    (int32_t)0x680B5C33, (int32_t)0xB5715EEF, (int32_t)0x67BD0FBD, (int32_t)0xB5049368, (int32_t)0x676E5183, (int32_t)0xB4981A1C, (int32_t)0x671F21DC, (int32_t)0xB42BF380,
    (int32_t)0x66CF8120, (int32_t)0xB3C0200C, (int32_t)0x667F6FA5, (int32_t)0xB354A037, (int32_t)0x662EEDC3, (int32_t)0xB2E97475, (int32_t)0x65DDFBD3, (int32_t)0xB27E9D3C,
    (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x653AC92B, (int32_t)0xB1A9EE3B, (int32_t)0x64E88926, (int32_t)0xB140175B, (int32_t)0x6495DA79, (int32_t)0xB0D696D8,
    (int32_t)0x6442BD7E, (int32_t)0xB06D6D24, (int32_t)0x63EF3290, (int32_t)0xB0049AB3, (int32_t)0x639B3A0B, (int32_t)0xAF9C1FF8, (int32_t)0x6346D44B, (int32_t)0xAF33FD64,
    (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x629CC28C, (int32_t)0xAE64C280, (int32_t)0x62471749, (int32_t)0xADFDAB11, (int32_t)0x61F1003F, (int32_t)0xAD96ED92,
    (int32_t)0x619A7DCE, (int32_t)0xAD308A71, (int32_t)0x61439053, (int32_t)0xACCA8221, (int32_t)0x60EC3830, (int32_t)0xAC64D510, (int32_t)0x609475C3, (int32_t)0xABFF83AF,
    (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x5FE3B38D, (int32_t)0xAB35F5B5, (int32_t)0x5F8AB487, (int32_t)0xAAD1B9FB, (int32_t)0x5F314CBA, (int32_t)0xAA6DDBA9,
    (int32_t)0x5ED77C8A, (int32_t)0xAA0A5B2E, (int32_t)0x5E7D4458, (int32_t)0xA9A738F7, (int32_t)0x5E22A487, (int32_t)0xA9447570, (int32_t)0x5DC79D7C, (int32_t)0xA8E21106,
    (int32_t)0x5D6C2F99, (int32_t)0xA8800C26, (int32_t)0x5D105B44, (int32_t)0xA81E6739, (int32_t)0x5CB420E0, (int32_t)0xA7BD22AC, (int32_t)0x5C5780D3, (int32_t)0xA75C3EE8,
    (int32_t)0x5BFA7B82, (int32_t)0xA6FBBC59, (int32_t)0x5B9D1154, (int32_t)0xA69B9B68, (int32_t)0x5B3F42AE, (int32_t)0xA63BDC7F, (int32_t)0x5AE10FF9, (int32_t)0xA5DC8006,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x5A237FFA, (int32_t)0xA51EF007, (int32_t)0x59C42381, (int32_t)0xA4C0BD52, (int32_t)0x59646498, (int32_t)0xA462EEAC,
    (int32_t)0x590443A7, (int32_t)0xA405847E, (int32_t)0x58A3C118, (int32_t)0xA3A87F2D, (int32_t)0x5842DD54, (int32_t)0xA34BDF20, (int32_t)0x57E198C7, (int32_t)0xA2EFA4BC,
    (int32_t)0x577FF3DA, (int32_t)0xA293D067, (int32_t)0x571DEEFA, (int32_t)0xA2386284, (int32_t)0x56BB8A90, (int32_t)0xA1DD5B79, (int32_t)0x5658C709, (int32_t)0xA182BBA8,
    (int32_t)0x55F5A4D2, (int32_t)0xA1288376, (int32_t)0x55922457, (int32_t)0xA0CEB346, (int32_t)0x552E4605, (int32_t)0xA0754B79, (int32_t)0x54CA0A4B, (int32_t)0xA01C4C73,
    (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0x54007C51, (int32_t)0x9F6B8A3D, (int32_t)0x539B2AF0, (int32_t)0x9F13C7D0, (int32_t)0x53357DDF, (int32_t)0x9EBC6FAD,
    (int32_t)0x52CF758F, (int32_t)0x9E658232, (int32_t)0x5269126E, (int32_t)0x9E0EFFC1, (int32_t)0x520254EF, (int32_t)0x9DB8E8B7, (int32_t)0x519B3D80, (int32_t)0x9D633D74,
    (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0x50CC029C, (int32_t)0x9CB92BB5, (int32_t)0x5063E008, (int32_t)0x9C64C5F5, (int32_t)0x4FFB654D, (int32_t)0x9C10CD70,
    (int32_t)0x4F9292DC, (int32_t)0x9BBD4282, (int32_t)0x4F296928, (int32_t)0x9B6A2587, (int32_t)0x4EBFE8A5, (int32_t)0x9B1776DA, (int32_t)0x4E5611C5, (int32_t)0x9AC536D5,
    (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x4D8162C4, (int32_t)0x9A22042D, (int32_t)0x4D168B8B, (int32_t)0x99D1123D, (int32_t)0x4CAB5FC9, (int32_t)0x9980905B,
    (int32_t)0x4C3FDFF4, (int32_t)0x99307EE0, (int32_t)0x4BD40C80, (int32_t)0x98E0DE24, (int32_t)0x4B67E5E4, (int32_t)0x9891AE7D, (int32_t)0x4AFB6C98, (int32_t)0x9842F043,
    (int32_t)0x4A8EA111, (int32_t)0x97F4A3CD, (int32_t)0x4A2183C8, (int32_t)0x97A6C96F, (int32_t)0x49B41533, (int32_t)0x9759617F, (int32_t)0x494655CC, (int32_t)0x970C6C52,
    (int32_t)0x48D84609, (int32_t)0x96BFEA3D, (int32_t)0x4869E665, (int32_t)0x9673DB94, (int32_t)0x47FB3757, (int32_t)0x962840A9, (int32_t)0x478C395A, (int32_t)0x95DD19D0,
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x46AD5278, (int32_t)0x9548299D, (int32_t)0x463D6A87, (int32_t)0x94FE60E6, (int32_t)0x45CD358F, (int32_t)0x94B50D87,
    (int32_t)0x455CB40C, (int32_t)0x946C2FD2, (int32_t)0x44EBE679, (int32_t)0x9423C815, (int32_t)0x447ACD50, (int32_t)0x93DBD6A0, (int32_t)0x44096910, (int32_t)0x93945BC2,
    (int32_t)0x4397BA32, (int32_t)0x934D57C9, (int32_t)0x4325C135, (int32_t)0x9306CB04, (int32_t)0x42B37E96, (int32_t)0x92C0B5C0, (int32_t)0x4240F2D1, (int32_t)0x927B1849,
    (int32_t)0x41CE1E65, (int32_t)0x9235F2EC, (int32_t)0x415B01CE, (int32_t)0x91F145F4, (int32_t)0x40E79D8C, (int32_t)0x91AD11AE, (int32_t)0x4073F21D, (int32_t)0x91695663,
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x3F8BC7B4, (int32_t)0x90E34BEA, (int32_t)0x3F1749B8, (int32_t)0x90A0FD4E, (int32_t)0x3EA2868C, (int32_t)0x905F28D4,
    (int32_t)0x3E2D7EB1, (int32_t)0x901DCEC4, (int32_t)0x3DB832A6, (int32_t)0x8FDCEF66, (int32_t)0x3D42A2EC, (int32_t)0x8F9C8B01, (int32_t)0x3CCCD004, (int32_t)0x8F5CA1DB,
    (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0x3BE062B0, (int32_t)0x8EDE4264, (int32_t)0x3B69C947, (int32_t)0x8E9FCC9F, (int32_t)0x3AF2EEB7, (int32_t)0x8E61D32E,
    (int32_t)0x3A7BD382, (int32_t)0x8E245655, (int32_t)0x3A04782A, (int32_t)0x8DE75659, (int32_t)0x398CDD32, (int32_t)0x8DAAD37B, (int32_t)0x3915031F, (int32_t)0x8D6ECDFF,
    (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0x382493B0, (int32_t)0x8CF83C30, (int32_t)0x37ABFF5D, (int32_t)0x8CBDB060, (int32_t)0x37332DFD, (int32_t)0x8C83A2F5,
    (int32_t)0x36BA2014, (int32_t)0x8C4A142F, (int32_t)0x3640D627, (int32_t)0x8C11044D, (int32_t)0x35C750BC, (int32_t)0x8BD8738E, (int32_t)0x354D9057, (int32_t)0x8BA0622F,
    (int32_t)0x34D3957E, (int32_t)0x8B68D06E, (int32_t)0x345960B7, (int32_t)0x8B31BE89, (int32_t)0x33DEF287, (int32_t)0x8AFB2CBB, (int32_t)0x33644B76, (int32_t)0x8AC51B40,
    (int32_t)0x32E96C09, (int32_t)0x8A8F8A54, (int32_t)0x326E54C7, (int32_t)0x8A5A7A31, (int32_t)0x31F30638, (int32_t)0x8A25EB11, (int32_t)0x317780E2, (int32_t)0x89F1DD2F,
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x307FD401, (int32_t)0x898B4606, (int32_t)0x3003AD85, (int32_t)0x8958BD2F, (int32_t)0x2F875262, (int32_t)0x8926B677,
    (int32_t)0x2F0AC320, (int32_t)0x88F53214, (int32_t)0x2E8E0048, (int32_t)0x88C4303C, (int32_t)0x2E110A62, (int32_t)0x8893B125, (int32_t)0x2D93E1F8, (int32_t)0x8863B504,
    (int32_t)0x2D168792, (int32_t)0x88343C0E, (int32_t)0x2C98FBBA, (int32_t)0x88054677, (int32_t)0x2C1B3EFB, (int32_t)0x87D6D473, (int32_t)0x2B9D51DD, (int32_t)0x87A8E634,
    (int32_t)0x2B1F34EB, (int32_t)0x877B7BEC, (int32_t)0x2AA0E8B0, (int32_t)0x874E95CE, (int32_t)0x2A226DB5, (int32_t)0x8722340B, (int32_t)0x29A3C485, (int32_t)0x86F656D3,
    (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x28A5E9B4, (int32_t)0x86A02AC6, (int32_t)0x2826B928, (int32_t)0x8675DC4F, (int32_t)0x27A75C95, (int32_t)0x864C1320,
    (int32_t)0x2727D486, (int32_t)0x8622CF68, (int32_t)0x26A82186, (int32_t)0x85FA1153, (int32_t)0x26284422, (int32_t)0x85D1D90E, (int32_t)0x25A83CE6, (int32_t)0x85AA26C6,
    (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0x24A7B317, (int32_t)0x855C54D7, (int32_t)0x2427319D, (int32_t)0x85363586, (int32_t)0x23A6887F, (int32_t)0x85109CDD,
    (int32_t)0x2325B847, (int32_t)0x84EB8B03, (int32_t)0x22A4C185, (int32_t)0x84C70022, (int32_t)0x2223A4C5, (int32_t)0x84A2FC62, (int32_t)0x21A26295, (int32_t)0x847F7FEB,
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x209F701C, (int32_t)0x843A1D70, (int32_t)0x201DC0EF, (int32_t)0x841837B9, (int32_t)0x1F9BEE8A, (int32_t)0x83F6D9E3,
    (int32_t)0x1F19F97B, (int32_t)0x83D60412, (int32_t)0x1E97E251, (int32_t)0x83B5B66A, (int32_t)0x1E15A99A, (int32_t)0x8395F10E, (int32_t)0x1D934FE5, (int32_t)0x8376B422,
    (int32_t)0x1D10D5C2, (int32_t)0x8357FFC8, (int32_t)0x1C8E3BBE, (int32_t)0x8339D421, (int32_t)0x1C0B826A, (int32_t)0x831C314E, (int32_t)0x1B88AA55, (int32_t)0x82FF1771,
    (int32_t)0x1B05B40F, (int32_t)0x82E286A8, (int32_t)0x1A82A026, (int32_t)0x82C67F14, (int32_t)0x19FF6F2A, (int32_t)0x82AB00D2, (int32_t)0x197C21AD, (int32_t)0x82900C02,
    (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x1875336A, (int32_t)0x825BBF2A, (int32_t)0x17F193C5, (int32_t)0x8242675C, (int32_t)0x176DD9DE, (int32_t)0x82299971,
    (int32_t)0x16EA0646, (int32_t)0x82115586, (int32_t)0x1666198D, (int32_t)0x81F99BB4, (int32_t)0x15E21445, (int32_t)0x81E26C16, (int32_t)0x155DF6FC, (int32_t)0x81CBC6C5,
    (int32_t)0x14D9C245, (int32_t)0x81B5ABDA, (int32_t)0x145576B1, (int32_t)0x81A01B6D, (int32_t)0x13D114D0, (int32_t)0x818B1596, (int32_t)0x134C9D34, (int32_t)0x81769A6B,
    (int32_t)0x12C8106F, (int32_t)0x8162AA04, (int32_t)0x12436F10, (int32_t)0x814F4476, (int32_t)0x11BEB9AA, (int32_t)0x813C69D6, (int32_t)0x1139F0CF, (int32_t)0x812A1A3A,
    (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x103026FE, (int32_t)0x81071C5A, (int32_t)0x0FAB272B, (int32_t)0x80F66E3C, (int32_t)0x0F26162A, (int32_t)0x80E64B6F,
    (int32_t)0x0EA0F48C, (int32_t)0x80D6B403, (int32_t)0x0E1BC2E4, (int32_t)0x80C7A80A, (int32_t)0x0D9681C2, (int32_t)0x80B92794, (int32_t)0x0D1131BA, (int32_t)0x80AB32B1,
    (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x0C066740, (int32_t)0x8090EBE1, (int32_t)0x0B80EDF1, (int32_t)0x80849A11, (int32_t)0x0AFB6805, (int32_t)0x8078D40D,
    (int32_t)0x0A75D60E, (int32_t)0x806D99E3, (int32_t)0x09F0389F, (int32_t)0x8062EB9F, (int32_t)0x096A9049, (int32_t)0x8058C94C, (int32_t)0x08E4DDA0, (int32_t)0x804F32F6,
    (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x07D95B9E, (int32_t)0x803DAA6A, (int32_t)0x07538D6B, (int32_t)0x8035B847, (int32_t)0x06CDB72F, (int32_t)0x802E5247,
    (int32_t)0x0647D97C, (int32_t)0x80277872, (int32_t)0x05C1F4E7, (int32_t)0x80212AD0, (int32_t)0x053C0A01, (int32_t)0x801B6968, (int32_t)0x04B6195D, (int32_t)0x80163440,
    (int32_t)0x0430238F, (int32_t)0x80118B5E, (int32_t)0x03AA292A, (int32_t)0x800D6EC6, (int32_t)0x03242ABF, (int32_t)0x8009DE7E, (int32_t)0x029E28E2, (int32_t)0x8006DA89,
    (int32_t)0x02182427, (int32_t)0x800462EB, (int32_t)0x01921D20, (int32_t)0x800277A6, (int32_t)0x010C1460, (int32_t)0x800118BC, (int32_t)0x00860A79, (int32_t)0x8000462F,
};


static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr768_32x32, 
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr768_32x32,
    __fft_real_tw
};

const fft_handle_t rnfft32_1536 = (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_1536 = (const fft_handle_t)&__rifft_descr;
