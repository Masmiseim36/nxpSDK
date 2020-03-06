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
ALIGN(8) static const int32_t __fft768_tw1[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFEE744, (int32_t)0xFEF3EBA0, 
    (int32_t)0x7FFB9D15, (int32_t)0xFDE7DBD9, (int32_t)0x7FF62182, (int32_t)0xFCDBD541, (int32_t)0x7FFB9D15, (int32_t)0xFDE7DBD9, (int32_t)0x7FEE74A2, (int32_t)0xFBCFDC71, 
    (int32_t)0x7FD8878E, (int32_t)0xF9B82684, (int32_t)0x7FF62182, (int32_t)0xFCDBD541, (int32_t)0x7FD8878E, (int32_t)0xF9B82684, (int32_t)0x7FA736B4, (int32_t)0xF6956FB7, 
    (int32_t)0x7FEE74A2, (int32_t)0xFBCFDC71, (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7FE49698, (int32_t)0xFAC3F5FF, 
    (int32_t)0x7F92661D, (int32_t)0xF58A29F2, (int32_t)0x7F0991C4, (int32_t)0xF054D8D5, (int32_t)0x7FD8878E, (int32_t)0xF9B82684, (int32_t)0x7F62368F, (int32_t)0xF3742CA2, 
    (int32_t)0x7E9D55FC, (int32_t)0xED37EF91, (int32_t)0x7FCA47B9, (int32_t)0xF8AC7295, (int32_t)0x7F294BFD, (int32_t)0xF15F0B74, (int32_t)0x7E1D93EA, (int32_t)0xEA1DEBBB, 
    (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7FA736B4, (int32_t)0xF6956FB7, 
    (int32_t)0x7E9D55FC, (int32_t)0xED37EF91, (int32_t)0x7CE3CEB2, (int32_t)0xE3F47D96, (int32_t)0x7F92661D, (int32_t)0xF58A29F2, (int32_t)0x7E4A5426, (int32_t)0xEB263DBB, 
    (int32_t)0x7C29FBEE, (int32_t)0xE0E60685, (int32_t)0x7F7B65EF, (int32_t)0xF47F120F, (int32_t)0x7DEEAA7A, (int32_t)0xE915F9BA, (int32_t)0x7B5D039E, (int32_t)0xDDDC5B3B, 
    (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x7F46D86C, (int32_t)0xF2697E3E, 
    (int32_t)0x7D1D7958, (int32_t)0xE4FA4BF1, (int32_t)0x798A23B1, (int32_t)0xD7D946D8, (int32_t)0x7F294BFD, (int32_t)0xF15F0B74, (int32_t)0x7CA80038, (int32_t)0xE2EF2A3E, 
    (int32_t)0x78848414, (int32_t)0xD4E0CB15, (int32_t)0x7F0991C4, (int32_t)0xF054D8D5, (int32_t)0x7C29FBEE, (int32_t)0xE0E60685, (int32_t)0x776C4EDB, (int32_t)0xD1EEF59E, 
    (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x7EC3962A, (int32_t)0xEE414656, 
    (int32_t)0x7B1474FD, (int32_t)0xDCDA47B9, (int32_t)0x7504D345, (int32_t)0xCC210D79, (int32_t)0x7E9D55FC, (int32_t)0xED37EF91, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, 
    (int32_t)0x73B5EBD1, (int32_t)0xC945DFEC, (int32_t)0x7E74EA6A, (int32_t)0xEC2EEB30, (int32_t)0x79DD3098, (int32_t)0xD8D82B7A, (int32_t)0x72552C85, (int32_t)0xC67322CE, 
    (int32_t)0x7E4A5426, (int32_t)0xEB263DBB, (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x7E1D93EA, (int32_t)0xEA1DEBBB, 
    (int32_t)0x78848414, (int32_t)0xD4E0CB15, (int32_t)0x6F5F02B2, (int32_t)0xC0E8B648, (int32_t)0x7DEEAA7A, (int32_t)0xE915F9BA, (int32_t)0x77CBC3F2, (int32_t)0xD2E9786E, 
    (int32_t)0x6DCA0D14, (int32_t)0xBE31E19B, (int32_t)0x7DBD98A4, (int32_t)0xE80E6C3B, (int32_t)0x770ACDEC, (int32_t)0xD0F53CE0, (int32_t)0x6C242960, (int32_t)0xBB8532B0, 
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x7D54FF2E, (int32_t)0xE60090D6, 
    (int32_t)0x757075AC, (int32_t)0xCD1693F7, (int32_t)0x68A69E81, (int32_t)0xB64BEACD, (int32_t)0x7D1D7958, (int32_t)0xE4FA4BF1, (int32_t)0x74972F92, (int32_t)0xCB2C6A82, 
    (int32_t)0x66CF8120, (int32_t)0xB3C0200C, (int32_t)0x7CE3CEB2, (int32_t)0xE3F47D96, (int32_t)0x73B5EBD1, (int32_t)0xC945DFEC, (int32_t)0x64E88926, (int32_t)0xB140175B, 
    (int32_t)0x7CA80038, (int32_t)0xE2EF2A3E, (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x7C6A0EF2, (int32_t)0xE1EA5666, 
    (int32_t)0x71DBA9AB, (int32_t)0xC5842C7E, (int32_t)0x60EC3830, (int32_t)0xAC64D510, (int32_t)0x7C29FBEE, (int32_t)0xE0E60685, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, 
    (int32_t)0x5ED77C8A, (int32_t)0xAA0A5B2E, (int32_t)0x7BE7C847, (int32_t)0xDFE23F11, (int32_t)0x6FE2313C, (int32_t)0xC1D2814F, (int32_t)0x5CB420E0, (int32_t)0xA7BD22AC, 
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x7B5D039E, (int32_t)0xDDDC5B3B, 
    (int32_t)0x6DCA0D14, (int32_t)0xBE31E19B, (int32_t)0x5842DD54, (int32_t)0xA34BDF20, (int32_t)0x7B1474FD, (int32_t)0xDCDA47B9, (int32_t)0x6CB2A837, (int32_t)0xBC6845CE, 
    (int32_t)0x55F5A4D2, (int32_t)0xA1288376, (int32_t)0x7AC9CA7A, (int32_t)0xDBD8CE63, (int32_t)0x6B93D02E, (int32_t)0xBAA34BF4, (int32_t)0x539B2AF0, (int32_t)0x9F13C7D0, 
    (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0x7A2E26F2, (int32_t)0xD9D7BBDE, 
    (int32_t)0x694015C3, (int32_t)0xB727B9F7, (int32_t)0x4EBFE8A5, (int32_t)0x9B1776DA, (int32_t)0x79DD3098, (int32_t)0xD8D82B7A, (int32_t)0x680B5C33, (int32_t)0xB5715EEF, 
    (int32_t)0x4C3FDFF4, (int32_t)0x99307EE0, (int32_t)0x798A23B1, (int32_t)0xD7D946D8, (int32_t)0x66CF8120, (int32_t)0xB3C0200C, (int32_t)0x49B41533, (int32_t)0x9759617F, 
    (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x78DDCBF5, (int32_t)0xD5DD924B, 
    (int32_t)0x6442BD7E, (int32_t)0xB06D6D24, (int32_t)0x447ACD50, (int32_t)0x93DBD6A0, (int32_t)0x78848414, (int32_t)0xD4E0CB15, (int32_t)0x62F201AC, (int32_t)0xAECC336C, 
    (int32_t)0x41CE1E65, (int32_t)0x9235F2EC, (int32_t)0x78292B8D, (int32_t)0xD3E4C105, (int32_t)0x619A7DCE, (int32_t)0xAD308A71, (int32_t)0x3F1749B8, (int32_t)0x90A0FD4E, 
    (int32_t)0x77CBC3F2, (int32_t)0xD2E9786E, (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0x776C4EDB, (int32_t)0xD1EEF59E, 
    (int32_t)0x5ED77C8A, (int32_t)0xAA0A5B2E, (int32_t)0x398CDD32, (int32_t)0x8DAAD37B, (int32_t)0x770ACDEC, (int32_t)0xD0F53CE0, (int32_t)0x5D6C2F99, (int32_t)0xA8800C26, 
    (int32_t)0x36BA2014, (int32_t)0x8C4A142F, (int32_t)0x76A742D1, (int32_t)0xCFFC527B, (int32_t)0x5BFA7B82, (int32_t)0xA6FBBC59, (int32_t)0x33DEF287, (int32_t)0x8AFB2CBB, 
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x75DA14EF, (int32_t)0xCE0CF9C8, 
    (int32_t)0x590443A7, (int32_t)0xA405847E, (int32_t)0x2E110A62, (int32_t)0x8893B125, (int32_t)0x757075AC, (int32_t)0xCD1693F7, (int32_t)0x577FF3DA, (int32_t)0xA293D067, 
    (int32_t)0x2B1F34EB, (int32_t)0x877B7BEC, (int32_t)0x7504D345, (int32_t)0xCC210D79, (int32_t)0x55F5A4D2, (int32_t)0xA1288376, (int32_t)0x2826B928, (int32_t)0x8675DC4F, 
    (int32_t)0x74972F92, (int32_t)0xCB2C6A82, (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0x74278C72, (int32_t)0xCA38AF44, 
    (int32_t)0x52CF758F, (int32_t)0x9E658232, (int32_t)0x2223A4C5, (int32_t)0x84A2FC62, (int32_t)0x73B5EBD1, (int32_t)0xC945DFEC, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, 
    (int32_t)0x1F19F97B, (int32_t)0x83D60412, (int32_t)0x73424FA0, (int32_t)0xC85400A3, (int32_t)0x4F9292DC, (int32_t)0x9BBD4282, (int32_t)0x1C0B826A, (int32_t)0x831C314E, 
    (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x72552C85, (int32_t)0xC67322CE, 
    (int32_t)0x4C3FDFF4, (int32_t)0x99307EE0, (int32_t)0x15E21445, (int32_t)0x81E26C16, (int32_t)0x71DBA9AB, (int32_t)0xC5842C7E, (int32_t)0x4A8EA111, (int32_t)0x97F4A3CD, 
    (int32_t)0x12C8106F, (int32_t)0x8162AA04, (int32_t)0x71603361, (int32_t)0xC49636B9, (int32_t)0x48D84609, (int32_t)0x96BFEA3D, (int32_t)0x0FAB272B, (int32_t)0x80F66E3C, 
    (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x706374FF, (int32_t)0xC2BD5D14, 
    (int32_t)0x455CB40C, (int32_t)0x946C2FD2, (int32_t)0x096A9049, (int32_t)0x8058C94C, (int32_t)0x6FE2313C, (int32_t)0xC1D2814F, (int32_t)0x4397BA32, (int32_t)0x934D57C9, 
    (int32_t)0x0647D97C, (int32_t)0x80277872, (int32_t)0x6F5F02B2, (int32_t)0xC0E8B648, (int32_t)0x41CE1E65, (int32_t)0x9235F2EC, (int32_t)0x03242ABF, (int32_t)0x8009DE7E, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x6E52EE52, (int32_t)0xBF186274, 
    (int32_t)0x3E2D7EB1, (int32_t)0x901DCEC4, (int32_t)0xFCDBD541, (int32_t)0x8009DE7E, (int32_t)0x6DCA0D14, (int32_t)0xBE31E19B, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, 
    (int32_t)0xF9B82684, (int32_t)0x80277872, (int32_t)0x6D3F4A40, (int32_t)0xBD4C816A, (int32_t)0x3A7BD382, (int32_t)0x8E245655, (int32_t)0xF6956FB7, (int32_t)0x8058C94C, 
    (int32_t)0x6CB2A837, (int32_t)0xBC6845CE, (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0xF3742CA2, (int32_t)0x809DC971, (int32_t)0x6C242960, (int32_t)0xBB8532B0, 
    (int32_t)0x36BA2014, (int32_t)0x8C4A142F, (int32_t)0xF054D8D5, (int32_t)0x80F66E3C, (int32_t)0x6B93D02E, (int32_t)0xBAA34BF4, (int32_t)0x34D3957E, (int32_t)0x8B68D06E, 
    (int32_t)0xED37EF91, (int32_t)0x8162AA04, (int32_t)0x6B019F1A, (int32_t)0xB9C29579, (int32_t)0x32E96C09, (int32_t)0x8A8F8A54, (int32_t)0xEA1DEBBB, (int32_t)0x81E26C16, 
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xE70747C4, (int32_t)0x8275A0C0, (int32_t)0x69D7BF57, (int32_t)0xB804C8A9, 
    (int32_t)0x2F0AC320, (int32_t)0x88F53214, (int32_t)0xE3F47D96, (int32_t)0x831C314E, (int32_t)0x694015C3, (int32_t)0xB727B9F7, (int32_t)0x2D168792, (int32_t)0x88343C0E, 
    (int32_t)0xE0E60685, (int32_t)0x83D60412, (int32_t)0x68A69E81, (int32_t)0xB64BEACD, (int32_t)0x2B1F34EB, (int32_t)0x877B7BEC, (int32_t)0xDDDC5B3B, (int32_t)0x84A2FC62, 
    (int32_t)0x680B5C33, (int32_t)0xB5715EEF, (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0xDAD7F3A2, (int32_t)0x8582FAA5, (int32_t)0x676E5183, (int32_t)0xB4981A1C, 
    (int32_t)0x2727D486, (int32_t)0x8622CF68, (int32_t)0xD7D946D8, (int32_t)0x8675DC4F, (int32_t)0x66CF8120, (int32_t)0xB3C0200C, (int32_t)0x25280C5E, (int32_t)0x8582FAA5, 
    (int32_t)0xD4E0CB15, (int32_t)0x877B7BEC, (int32_t)0x662EEDC3, (int32_t)0xB2E97475, (int32_t)0x2325B847, (int32_t)0x84EB8B03, (int32_t)0xD1EEF59E, (int32_t)0x8893B125, 
    (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, (int32_t)0x64E88926, (int32_t)0xB140175B, 
    (int32_t)0x1F19F97B, (int32_t)0x83D60412, (int32_t)0xCC210D79, (int32_t)0x8AFB2CBB, (int32_t)0x6442BD7E, (int32_t)0xB06D6D24, (int32_t)0x1D10D5C2, (int32_t)0x8357FFC8, 
    (int32_t)0xC945DFEC, (int32_t)0x8C4A142F, (int32_t)0x639B3A0B, (int32_t)0xAF9C1FF8, (int32_t)0x1B05B40F, (int32_t)0x82E286A8, (int32_t)0xC67322CE, (int32_t)0x8DAAD37B, 
    (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0xC3A94590, (int32_t)0x8F1D343A, (int32_t)0x62471749, (int32_t)0xADFDAB11, 
    (int32_t)0x16EA0646, (int32_t)0x82115586, (int32_t)0xC0E8B648, (int32_t)0x90A0FD4E, (int32_t)0x619A7DCE, (int32_t)0xAD308A71, (int32_t)0x14D9C245, (int32_t)0x81B5ABDA, 
    (int32_t)0xBE31E19B, (int32_t)0x9235F2EC, (int32_t)0x60EC3830, (int32_t)0xAC64D510, (int32_t)0x12C8106F, (int32_t)0x8162AA04, (int32_t)0xBB8532B0, (int32_t)0x93DBD6A0, 
    (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0xB8E31319, (int32_t)0x9592675C, (int32_t)0x5F8AB487, (int32_t)0xAAD1B9FB, 
    (int32_t)0x0EA0F48C, (int32_t)0x80D6B403, (int32_t)0xB64BEACD, (int32_t)0x9759617F, (int32_t)0x5ED77C8A, (int32_t)0xAA0A5B2E, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, 
    (int32_t)0xB3C0200C, (int32_t)0x99307EE0, (int32_t)0x5E22A487, (int32_t)0xA9447570, (int32_t)0x0A75D60E, (int32_t)0x806D99E3, (int32_t)0xB140175B, (int32_t)0x9B1776DA, 
    (int32_t)0x5D6C2F99, (int32_t)0xA8800C26, (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0xAECC336C, (int32_t)0x9D0DFE54, (int32_t)0x5CB420E0, (int32_t)0xA7BD22AC, 
    (int32_t)0x0647D97C, (int32_t)0x80277872, (int32_t)0xAC64D510, (int32_t)0x9F13C7D0, (int32_t)0x5BFA7B82, (int32_t)0xA6FBBC59, (int32_t)0x0430238F, (int32_t)0x80118B5E, 
    (int32_t)0xAA0A5B2E, (int32_t)0xA1288376, (int32_t)0x5B3F42AE, (int32_t)0xA63BDC7F, (int32_t)0x02182427, (int32_t)0x800462EB, (int32_t)0xA7BD22AC, (int32_t)0xA34BDF20, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x59C42381, (int32_t)0xA4C0BD52, 
    (int32_t)0xFDE7DBD9, (int32_t)0x800462EB, (int32_t)0xA34BDF20, (int32_t)0xA7BD22AC, (int32_t)0x590443A7, (int32_t)0xA405847E, (int32_t)0xFBCFDC71, (int32_t)0x80118B5E, 
    (int32_t)0xA1288376, (int32_t)0xAA0A5B2E, (int32_t)0x5842DD54, (int32_t)0xA34BDF20, (int32_t)0xF9B82684, (int32_t)0x80277872, (int32_t)0x9F13C7D0, (int32_t)0xAC64D510, 
    (int32_t)0x577FF3DA, (int32_t)0xA293D067, (int32_t)0xF7A0DEC9, (int32_t)0x804628A7, (int32_t)0x9D0DFE54, (int32_t)0xAECC336C, (int32_t)0x56BB8A90, (int32_t)0xA1DD5B79, 
    (int32_t)0xF58A29F2, (int32_t)0x806D99E3, (int32_t)0x9B1776DA, (int32_t)0xB140175B, (int32_t)0x55F5A4D2, (int32_t)0xA1288376, (int32_t)0xF3742CA2, (int32_t)0x809DC971, 
    (int32_t)0x99307EE0, (int32_t)0xB3C0200C, (int32_t)0x552E4605, (int32_t)0xA0754B79, (int32_t)0xF15F0B74, (int32_t)0x80D6B403, (int32_t)0x9759617F, (int32_t)0xB64BEACD, 
    (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0xEF4AEAF1, (int32_t)0x811855B4, (int32_t)0x9592675C, (int32_t)0xB8E31319, (int32_t)0x539B2AF0, (int32_t)0x9F13C7D0, 
    (int32_t)0xED37EF91, (int32_t)0x8162AA04, (int32_t)0x93DBD6A0, (int32_t)0xBB8532B0, (int32_t)0x52CF758F, (int32_t)0x9E658232, (int32_t)0xEB263DBB, (int32_t)0x81B5ABDA, 
    (int32_t)0x9235F2EC, (int32_t)0xBE31E19B, (int32_t)0x520254EF, (int32_t)0x9DB8E8B7, (int32_t)0xE915F9BA, (int32_t)0x82115586, (int32_t)0x90A0FD4E, (int32_t)0xC0E8B648, 
    (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0xE70747C4, (int32_t)0x8275A0C0, (int32_t)0x8F1D343A, (int32_t)0xC3A94590, (int32_t)0x5063E008, (int32_t)0x9C64C5F5, 
    (int32_t)0xE4FA4BF1, (int32_t)0x82E286A8, (int32_t)0x8DAAD37B, (int32_t)0xC67322CE, (int32_t)0x4F9292DC, (int32_t)0x9BBD4282, (int32_t)0xE2EF2A3E, (int32_t)0x8357FFC8, 
    (int32_t)0x8C4A142F, (int32_t)0xC945DFEC, (int32_t)0x4EBFE8A5, (int32_t)0x9B1776DA, (int32_t)0xE0E60685, (int32_t)0x83D60412, (int32_t)0x8AFB2CBB, (int32_t)0xCC210D79, 
    (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, (int32_t)0x4D168B8B, (int32_t)0x99D1123D, 
    (int32_t)0xDCDA47B9, (int32_t)0x84EB8B03, (int32_t)0x8893B125, (int32_t)0xD1EEF59E, (int32_t)0x4C3FDFF4, (int32_t)0x99307EE0, (int32_t)0xDAD7F3A2, (int32_t)0x8582FAA5, 
    (int32_t)0x877B7BEC, (int32_t)0xD4E0CB15, (int32_t)0x4B67E5E4, (int32_t)0x9891AE7D, (int32_t)0xD8D82B7A, (int32_t)0x8622CF68, (int32_t)0x8675DC4F, (int32_t)0xD7D946D8, 
    (int32_t)0x4A8EA111, (int32_t)0x97F4A3CD, (int32_t)0xD6DB1254, (int32_t)0x86CAFE57, (int32_t)0x8582FAA5, (int32_t)0xDAD7F3A2, (int32_t)0x49B41533, (int32_t)0x9759617F, 
    (int32_t)0xD4E0CB15, (int32_t)0x877B7BEC, (int32_t)0x84A2FC62, (int32_t)0xDDDC5B3B, (int32_t)0x48D84609, (int32_t)0x96BFEA3D, (int32_t)0xD2E9786E, (int32_t)0x88343C0E, 
    (int32_t)0x83D60412, (int32_t)0xE0E60685, (int32_t)0x47FB3757, (int32_t)0x962840A9, (int32_t)0xD0F53CE0, (int32_t)0x88F53214, (int32_t)0x831C314E, (int32_t)0xE3F47D96, 
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, (int32_t)0x8275A0C0, (int32_t)0xE70747C4, (int32_t)0x463D6A87, (int32_t)0x94FE60E6, 
    (int32_t)0xCD1693F7, (int32_t)0x8A8F8A54, (int32_t)0x81E26C16, (int32_t)0xEA1DEBBB, (int32_t)0x455CB40C, (int32_t)0x946C2FD2, (int32_t)0xCB2C6A82, (int32_t)0x8B68D06E, 
    (int32_t)0x8162AA04, (int32_t)0xED37EF91, (int32_t)0x447ACD50, (int32_t)0x93DBD6A0, (int32_t)0xC945DFEC, (int32_t)0x8C4A142F, (int32_t)0x80F66E3C, (int32_t)0xF054D8D5, 
    (int32_t)0x4397BA32, (int32_t)0x934D57C9, (int32_t)0xC763158E, (int32_t)0x8D334625, (int32_t)0x809DC971, (int32_t)0xF3742CA2, (int32_t)0x42B37E96, (int32_t)0x92C0B5C0, 
    (int32_t)0xC5842C7E, (int32_t)0x8E245655, (int32_t)0x8058C94C, (int32_t)0xF6956FB7, (int32_t)0x41CE1E65, (int32_t)0x9235F2EC, (int32_t)0xC3A94590, (int32_t)0x8F1D343A, 
    (int32_t)0x80277872, (int32_t)0xF9B82684, (int32_t)0x40E79D8C, (int32_t)0x91AD11AE, (int32_t)0xC1D2814F, (int32_t)0x901DCEC4, (int32_t)0x8009DE7E, (int32_t)0xFCDBD541, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, (int32_t)0x3F1749B8, (int32_t)0x90A0FD4E, 
    (int32_t)0xBE31E19B, (int32_t)0x9235F2EC, (int32_t)0x8009DE7E, (int32_t)0x03242ABF, (int32_t)0x3E2D7EB1, (int32_t)0x901DCEC4, (int32_t)0xBC6845CE, (int32_t)0x934D57C9, 
    (int32_t)0x80277872, (int32_t)0x0647D97C, (int32_t)0x3D42A2EC, (int32_t)0x8F9C8B01, (int32_t)0xBAA34BF4, (int32_t)0x946C2FD2, (int32_t)0x8058C94C, (int32_t)0x096A9049, 
    (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0xB8E31319, (int32_t)0x9592675C, (int32_t)0x809DC971, (int32_t)0x0C8BD35E, (int32_t)0x3B69C947, (int32_t)0x8E9FCC9F, 
    (int32_t)0xB727B9F7, (int32_t)0x96BFEA3D, (int32_t)0x80F66E3C, (int32_t)0x0FAB272B, (int32_t)0x3A7BD382, (int32_t)0x8E245655, (int32_t)0xB5715EEF, (int32_t)0x97F4A3CD, 
    (int32_t)0x8162AA04, (int32_t)0x12C8106F, (int32_t)0x398CDD32, (int32_t)0x8DAAD37B, (int32_t)0xB3C0200C, (int32_t)0x99307EE0, (int32_t)0x81E26C16, (int32_t)0x15E21445, 
    (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0xB2141B02, (int32_t)0x9A7365D3, (int32_t)0x8275A0C0, (int32_t)0x18F8B83C, (int32_t)0x37ABFF5D, (int32_t)0x8CBDB060, 
    (int32_t)0xB06D6D24, (int32_t)0x9BBD4282, (int32_t)0x831C314E, (int32_t)0x1C0B826A, (int32_t)0x36BA2014, (int32_t)0x8C4A142F, (int32_t)0xAECC336C, (int32_t)0x9D0DFE54, 
    (int32_t)0x83D60412, (int32_t)0x1F19F97B, (int32_t)0x35C750BC, (int32_t)0x8BD8738E, (int32_t)0xAD308A71, (int32_t)0x9E658232, (int32_t)0x84A2FC62, (int32_t)0x2223A4C5, 
    (int32_t)0x34D3957E, (int32_t)0x8B68D06E, (int32_t)0xAB9A8E6C, (int32_t)0x9FC3B694, (int32_t)0x8582FAA5, (int32_t)0x25280C5E, (int32_t)0x33DEF287, (int32_t)0x8AFB2CBB, 
    (int32_t)0xAA0A5B2E, (int32_t)0xA1288376, (int32_t)0x8675DC4F, (int32_t)0x2826B928, (int32_t)0x32E96C09, (int32_t)0x8A8F8A54, (int32_t)0xA8800C26, (int32_t)0xA293D067, 
    (int32_t)0x877B7BEC, (int32_t)0x2B1F34EB, (int32_t)0x31F30638, (int32_t)0x8A25EB11, (int32_t)0xA6FBBC59, (int32_t)0xA405847E, (int32_t)0x8893B125, (int32_t)0x2E110A62, 
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, (int32_t)0x3003AD85, (int32_t)0x8958BD2F, 
    (int32_t)0xA405847E, (int32_t)0xA6FBBC59, (int32_t)0x8AFB2CBB, (int32_t)0x33DEF287, (int32_t)0x2F0AC320, (int32_t)0x88F53214, (int32_t)0xA293D067, (int32_t)0xA8800C26, 
    (int32_t)0x8C4A142F, (int32_t)0x36BA2014, (int32_t)0x2E110A62, (int32_t)0x8893B125, (int32_t)0xA1288376, (int32_t)0xAA0A5B2E, (int32_t)0x8DAAD37B, (int32_t)0x398CDD32, 
    (int32_t)0x2D168792, (int32_t)0x88343C0E, (int32_t)0x9FC3B694, (int32_t)0xAB9A8E6C, (int32_t)0x8F1D343A, (int32_t)0x3C56BA70, (int32_t)0x2C1B3EFB, (int32_t)0x87D6D473, 
    (int32_t)0x9E658232, (int32_t)0xAD308A71, (int32_t)0x90A0FD4E, (int32_t)0x3F1749B8, (int32_t)0x2B1F34EB, (int32_t)0x877B7BEC, (int32_t)0x9D0DFE54, (int32_t)0xAECC336C, 
    (int32_t)0x9235F2EC, (int32_t)0x41CE1E65, (int32_t)0x2A226DB5, (int32_t)0x8722340B, (int32_t)0x9BBD4282, (int32_t)0xB06D6D24, (int32_t)0x93DBD6A0, (int32_t)0x447ACD50, 
    (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x9A7365D3, (int32_t)0xB2141B02, (int32_t)0x9592675C, (int32_t)0x471CECE7, (int32_t)0x2826B928, (int32_t)0x8675DC4F, 
    (int32_t)0x99307EE0, (int32_t)0xB3C0200C, (int32_t)0x9759617F, (int32_t)0x49B41533, (int32_t)0x2727D486, (int32_t)0x8622CF68, (int32_t)0x97F4A3CD, (int32_t)0xB5715EEF, 
    (int32_t)0x99307EE0, (int32_t)0x4C3FDFF4, (int32_t)0x26284422, (int32_t)0x85D1D90E, (int32_t)0x96BFEA3D, (int32_t)0xB727B9F7, (int32_t)0x9B1776DA, (int32_t)0x4EBFE8A5, 
    (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0x9592675C, (int32_t)0xB8E31319, (int32_t)0x9D0DFE54, (int32_t)0x5133CC94, (int32_t)0x2427319D, (int32_t)0x85363586, 
    (int32_t)0x946C2FD2, (int32_t)0xBAA34BF4, (int32_t)0x9F13C7D0, (int32_t)0x539B2AF0, (int32_t)0x2325B847, (int32_t)0x84EB8B03, (int32_t)0x934D57C9, (int32_t)0xBC6845CE, 
    (int32_t)0xA1288376, (int32_t)0x55F5A4D2, (int32_t)0x2223A4C5, (int32_t)0x84A2FC62, (int32_t)0x9235F2EC, (int32_t)0xBE31E19B, (int32_t)0xA34BDF20, (int32_t)0x5842DD54, 
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0xA57D8666, (int32_t)0x5A82799A, (int32_t)0x201DC0EF, (int32_t)0x841837B9, 
    (int32_t)0x901DCEC4, (int32_t)0xC1D2814F, (int32_t)0xA7BD22AC, (int32_t)0x5CB420E0, (int32_t)0x1F19F97B, (int32_t)0x83D60412, (int32_t)0x8F1D343A, (int32_t)0xC3A94590, 
    (int32_t)0xAA0A5B2E, (int32_t)0x5ED77C8A, (int32_t)0x1E15A99A, (int32_t)0x8395F10E, (int32_t)0x8E245655, (int32_t)0xC5842C7E, (int32_t)0xAC64D510, (int32_t)0x60EC3830, 
    (int32_t)0x1D10D5C2, (int32_t)0x8357FFC8, (int32_t)0x8D334625, (int32_t)0xC763158E, (int32_t)0xAECC336C, (int32_t)0x62F201AC, (int32_t)0x1C0B826A, (int32_t)0x831C314E, 
    (int32_t)0x8C4A142F, (int32_t)0xC945DFEC, (int32_t)0xB140175B, (int32_t)0x64E88926, (int32_t)0x1B05B40F, (int32_t)0x82E286A8, (int32_t)0x8B68D06E, (int32_t)0xCB2C6A82, 
    (int32_t)0xB3C0200C, (int32_t)0x66CF8120, (int32_t)0x19FF6F2A, (int32_t)0x82AB00D2, (int32_t)0x8A8F8A54, (int32_t)0xCD1693F7, (int32_t)0xB64BEACD, (int32_t)0x68A69E81, 
    (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, (int32_t)0xB8E31319, (int32_t)0x6A6D98A4, (int32_t)0x17F193C5, (int32_t)0x8242675C, 
    (int32_t)0x88F53214, (int32_t)0xD0F53CE0, (int32_t)0xBB8532B0, (int32_t)0x6C242960, (int32_t)0x16EA0646, (int32_t)0x82115586, (int32_t)0x88343C0E, (int32_t)0xD2E9786E, 
    (int32_t)0xBE31E19B, (int32_t)0x6DCA0D14, (int32_t)0x15E21445, (int32_t)0x81E26C16, (int32_t)0x877B7BEC, (int32_t)0xD4E0CB15, (int32_t)0xC0E8B648, (int32_t)0x6F5F02B2, 
    (int32_t)0x14D9C245, (int32_t)0x81B5ABDA, (int32_t)0x86CAFE57, (int32_t)0xD6DB1254, (int32_t)0xC3A94590, (int32_t)0x70E2CBC6, (int32_t)0x13D114D0, (int32_t)0x818B1596, 
    (int32_t)0x8622CF68, (int32_t)0xD8D82B7A, (int32_t)0xC67322CE, (int32_t)0x72552C85, (int32_t)0x12C8106F, (int32_t)0x8162AA04, (int32_t)0x8582FAA5, (int32_t)0xDAD7F3A2, 
    (int32_t)0xC945DFEC, (int32_t)0x73B5EBD1, (int32_t)0x11BEB9AA, (int32_t)0x813C69D6, (int32_t)0x84EB8B03, (int32_t)0xDCDA47B9, (int32_t)0xCC210D79, (int32_t)0x7504D345, 
    (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0xCF043AB3, (int32_t)0x7641AF3D, (int32_t)0x0FAB272B, (int32_t)0x80F66E3C, 
    (int32_t)0x83D60412, (int32_t)0xE0E60685, (int32_t)0xD1EEF59E, (int32_t)0x776C4EDB, (int32_t)0x0EA0F48C, (int32_t)0x80D6B403, (int32_t)0x8357FFC8, (int32_t)0xE2EF2A3E, 
    (int32_t)0xD4E0CB15, (int32_t)0x78848414, (int32_t)0x0D9681C2, (int32_t)0x80B92794, (int32_t)0x82E286A8, (int32_t)0xE4FA4BF1, (int32_t)0xD7D946D8, (int32_t)0x798A23B1, 
    (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x8275A0C0, (int32_t)0xE70747C4, (int32_t)0xDAD7F3A2, (int32_t)0x7A7D055B, (int32_t)0x0B80EDF1, (int32_t)0x80849A11, 
    (int32_t)0x82115586, (int32_t)0xE915F9BA, (int32_t)0xDDDC5B3B, (int32_t)0x7B5D039E, (int32_t)0x0A75D60E, (int32_t)0x806D99E3, (int32_t)0x81B5ABDA, (int32_t)0xEB263DBB, 
    (int32_t)0xE0E60685, (int32_t)0x7C29FBEE, (int32_t)0x096A9049, (int32_t)0x8058C94C, (int32_t)0x8162AA04, (int32_t)0xED37EF91, (int32_t)0xE3F47D96, (int32_t)0x7CE3CEB2, 
    (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x811855B4, (int32_t)0xEF4AEAF1, (int32_t)0xE70747C4, (int32_t)0x7D8A5F40, (int32_t)0x07538D6B, (int32_t)0x8035B847, 
    (int32_t)0x80D6B403, (int32_t)0xF15F0B74, (int32_t)0xEA1DEBBB, (int32_t)0x7E1D93EA, (int32_t)0x0647D97C, (int32_t)0x80277872, (int32_t)0x809DC971, (int32_t)0xF3742CA2, 
    (int32_t)0xED37EF91, (int32_t)0x7E9D55FC, (int32_t)0x053C0A01, (int32_t)0x801B6968, (int32_t)0x806D99E3, (int32_t)0xF58A29F2, (int32_t)0xF054D8D5, (int32_t)0x7F0991C4, 
    (int32_t)0x0430238F, (int32_t)0x80118B5E, (int32_t)0x804628A7, (int32_t)0xF7A0DEC9, (int32_t)0xF3742CA2, (int32_t)0x7F62368F, (int32_t)0x03242ABF, (int32_t)0x8009DE7E, 
    (int32_t)0x80277872, (int32_t)0xF9B82684, (int32_t)0xF6956FB7, (int32_t)0x7FA736B4, (int32_t)0x02182427, (int32_t)0x800462EB, (int32_t)0x80118B5E, (int32_t)0xFBCFDC71, 
    (int32_t)0xF9B82684, (int32_t)0x7FD8878E, (int32_t)0x010C1460, (int32_t)0x800118BC, (int32_t)0x800462EB, (int32_t)0xFDE7DBD9, (int32_t)0xFCDBD541, (int32_t)0x7FF62182, 
};

/****************** stage 2 radix 4 ******************/
ALIGN(8) static const int32_t __fft768_tw2[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FEE74A2, (int32_t)0xFBCFDC71, 
    (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, 
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, 
    (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x7E4A5426, (int32_t)0xEB263DBB, 
    (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, 
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x7CA80038, (int32_t)0xE2EF2A3E, (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x62F201AC, (int32_t)0xAECC336C, 
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, 
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, 
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x77CBC3F2, (int32_t)0xD2E9786E, (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, 
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x74972F92, (int32_t)0xCB2C6A82, 
    (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, 
    (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x6CB2A837, (int32_t)0xBC6845CE, 
    (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0xF3742CA2, (int32_t)0x809DC971, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, 
    (int32_t)0xE70747C4, (int32_t)0x8275A0C0, (int32_t)0x680B5C33, (int32_t)0xB5715EEF, (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0xDAD7F3A2, (int32_t)0x8582FAA5, 
    (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, (int32_t)0x62F201AC, (int32_t)0xAECC336C, 
    (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0xC3A94590, (int32_t)0x8F1D343A, (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x10B5150F, (int32_t)0x811855B4, 
    (int32_t)0xB8E31319, (int32_t)0x9592675C, (int32_t)0x5D6C2F99, (int32_t)0xA8800C26, (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0xAECC336C, (int32_t)0x9D0DFE54, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x577FF3DA, (int32_t)0xA293D067, 
    (int32_t)0xF7A0DEC9, (int32_t)0x804628A7, (int32_t)0x9D0DFE54, (int32_t)0xAECC336C, (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0xEF4AEAF1, (int32_t)0x811855B4, 
    (int32_t)0x9592675C, (int32_t)0xB8E31319, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0xE70747C4, (int32_t)0x8275A0C0, (int32_t)0x8F1D343A, (int32_t)0xC3A94590, 
    (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, (int32_t)0x4A8EA111, (int32_t)0x97F4A3CD, 
    (int32_t)0xD6DB1254, (int32_t)0x86CAFE57, (int32_t)0x8582FAA5, (int32_t)0xDAD7F3A2, (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, 
    (int32_t)0x8275A0C0, (int32_t)0xE70747C4, (int32_t)0x4397BA32, (int32_t)0x934D57C9, (int32_t)0xC763158E, (int32_t)0x8D334625, (int32_t)0x809DC971, (int32_t)0xF3742CA2, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, 
    (int32_t)0xB8E31319, (int32_t)0x9592675C, (int32_t)0x809DC971, (int32_t)0x0C8BD35E, (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0xB2141B02, (int32_t)0x9A7365D3, 
    (int32_t)0x8275A0C0, (int32_t)0x18F8B83C, (int32_t)0x34D3957E, (int32_t)0x8B68D06E, (int32_t)0xAB9A8E6C, (int32_t)0x9FC3B694, (int32_t)0x8582FAA5, (int32_t)0x25280C5E, 
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, (int32_t)0x2D168792, (int32_t)0x88343C0E, 
    (int32_t)0x9FC3B694, (int32_t)0xAB9A8E6C, (int32_t)0x8F1D343A, (int32_t)0x3C56BA70, (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x9A7365D3, (int32_t)0xB2141B02, 
    (int32_t)0x9592675C, (int32_t)0x471CECE7, (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0x9592675C, (int32_t)0xB8E31319, (int32_t)0x9D0DFE54, (int32_t)0x5133CC94, 
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0xA57D8666, (int32_t)0x5A82799A, (int32_t)0x1D10D5C2, (int32_t)0x8357FFC8, 
    (int32_t)0x8D334625, (int32_t)0xC763158E, (int32_t)0xAECC336C, (int32_t)0x62F201AC, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, 
    (int32_t)0xB8E31319, (int32_t)0x6A6D98A4, (int32_t)0x14D9C245, (int32_t)0x81B5ABDA, (int32_t)0x86CAFE57, (int32_t)0xD6DB1254, (int32_t)0xC3A94590, (int32_t)0x70E2CBC6, 
    (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0xCF043AB3, (int32_t)0x7641AF3D, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, 
    (int32_t)0x8275A0C0, (int32_t)0xE70747C4, (int32_t)0xDAD7F3A2, (int32_t)0x7A7D055B, (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x811855B4, (int32_t)0xEF4AEAF1, 
    (int32_t)0xE70747C4, (int32_t)0x7D8A5F40, (int32_t)0x0430238F, (int32_t)0x80118B5E, (int32_t)0x804628A7, (int32_t)0xF7A0DEC9, (int32_t)0xF3742CA2, (int32_t)0x7F62368F, 
};

/****************** stage 3 radix 4 ******************/
ALIGN(8) static const int32_t __fft768_tw3[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, 
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, 
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, 
    (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, 
    (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000, 
    (int32_t)0xA57D8666, (int32_t)0x5A82799A, (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0xCF043AB3, (int32_t)0x7641AF3D, 
};

/****************** stage 4 radix 4 ******************/
ALIGN(8) static const int32_t __fft768_tw4[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, 
};
#define N 768
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT3_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 1, 1, 
}; 
static const cint32ptr_t tw_tab[] = 
{
    __fft768_tw1, __fft768_tw2, __fft768_tw3, __fft768_tw4, NULL
};
 const fft_cplx32x32_descr_t __cfft_descr768_32x32 = 
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
};     
 const fft_cplx32x32_descr_t __cifft_descr768_32x32 =
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_768 = (const fft_handle_t)&__cfft_descr768_32x32;
const fft_handle_t cinfft32_768 = (const fft_handle_t)&__cifft_descr768_32x32;
