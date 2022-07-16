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

/* Twiddles tables for fft_cplx32x16, ifft_cplx32x16, fft_cplx16x16, ifft_cplx16x16, N=640 */
#define N 640

/********** Twiddles table N=640 stage 1 radix 5 ******************/
ALIGN(8) static const int16_t _fft640_tw1_[] =
{
    (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000,
    (int16_t)0x7FFE, (int16_t)0xFEBE, (int16_t)0x7FFA, (int16_t)0xFD7D, (int16_t)0x7FF2, (int16_t)0xFC3B, (int16_t)0x7FE7, (int16_t)0xFAFA,
    (int16_t)0x7FFA, (int16_t)0xFD7D, (int16_t)0x7FE7, (int16_t)0xFAFA, (int16_t)0x7FC7, (int16_t)0xF877, (int16_t)0x7F9B, (int16_t)0xF5F5,
    (int16_t)0x7FF2, (int16_t)0xFC3B, (int16_t)0x7FC7, (int16_t)0xF877, (int16_t)0x7F80, (int16_t)0xF4B4, (int16_t)0x7F1D, (int16_t)0xF0F5,
    (int16_t)0x7FE7, (int16_t)0xFAFA, (int16_t)0x7F9B, (int16_t)0xF5F5, (int16_t)0x7F1D, (int16_t)0xF0F5, (int16_t)0x7E6D, (int16_t)0xEBFA,
    (int16_t)0x7FD9, (int16_t)0xF9B8, (int16_t)0x7F62, (int16_t)0xF374, (int16_t)0x7E9D, (int16_t)0xED38, (int16_t)0x7D8A, (int16_t)0xE707,
    (int16_t)0x7FC7, (int16_t)0xF877, (int16_t)0x7F1D, (int16_t)0xF0F5, (int16_t)0x7E02, (int16_t)0xE980, (int16_t)0x7C77, (int16_t)0xE21E,
    (int16_t)0x7FB3, (int16_t)0xF736, (int16_t)0x7ECB, (int16_t)0xEE76, (int16_t)0x7D4A, (int16_t)0xE5CC, (int16_t)0x7B32, (int16_t)0xDD41,
    (int16_t)0x7F9B, (int16_t)0xF5F5, (int16_t)0x7E6D, (int16_t)0xEBFA, (int16_t)0x7C77, (int16_t)0xE21E, (int16_t)0x79BC, (int16_t)0xD872,
    (int16_t)0x7F80, (int16_t)0xF4B4, (int16_t)0x7E02, (int16_t)0xE980, (int16_t)0x7B88, (int16_t)0xDE77, (int16_t)0x7817, (int16_t)0xD3B2,
    (int16_t)0x7F62, (int16_t)0xF374, (int16_t)0x7D8A, (int16_t)0xE707, (int16_t)0x7A7D, (int16_t)0xDAD8, (int16_t)0x7642, (int16_t)0xCF04,
    (int16_t)0x7F41, (int16_t)0xF234, (int16_t)0x7D07, (int16_t)0xE492, (int16_t)0x7957, (int16_t)0xD741, (int16_t)0x743E, (int16_t)0xCA69,
    (int16_t)0x7F1D, (int16_t)0xF0F5, (int16_t)0x7C77, (int16_t)0xE21E, (int16_t)0x7817, (int16_t)0xD3B2, (int16_t)0x720D, (int16_t)0xC5E4,
    (int16_t)0x7EF5, (int16_t)0xEFB5, (int16_t)0x7BDA, (int16_t)0xDFAE, (int16_t)0x76BB, (int16_t)0xD02E, (int16_t)0x6FAE, (int16_t)0xC175,
    (int16_t)0x7ECB, (int16_t)0xEE76, (int16_t)0x7B32, (int16_t)0xDD41, (int16_t)0x7546, (int16_t)0xCCB4, (int16_t)0x6D23, (int16_t)0xBD1F,
    (int16_t)0x7E9D, (int16_t)0xED38, (int16_t)0x7A7D, (int16_t)0xDAD8, (int16_t)0x73B6, (int16_t)0xC946, (int16_t)0x6A6E, (int16_t)0xB8E3,
    (int16_t)0x7E6D, (int16_t)0xEBFA, (int16_t)0x79BC, (int16_t)0xD872, (int16_t)0x720D, (int16_t)0xC5E4, (int16_t)0x678E, (int16_t)0xB4C3,
    (int16_t)0x7E39, (int16_t)0xEABC, (int16_t)0x78EF, (int16_t)0xD610, (int16_t)0x704A, (int16_t)0xC28E, (int16_t)0x6485, (int16_t)0xB0C2,
    (int16_t)0x7E02, (int16_t)0xE980, (int16_t)0x7817, (int16_t)0xD3B2, (int16_t)0x6E6E, (int16_t)0xBF47, (int16_t)0x6155, (int16_t)0xACDF,
    (int16_t)0x7DC8, (int16_t)0xE843, (int16_t)0x7732, (int16_t)0xD159, (int16_t)0x6C7A, (int16_t)0xBC0D, (int16_t)0x5DFE, (int16_t)0xA91D,
    (int16_t)0x7D8A, (int16_t)0xE707, (int16_t)0x7642, (int16_t)0xCF04, (int16_t)0x6A6E, (int16_t)0xB8E3, (int16_t)0x5A82, (int16_t)0xA57E,
    (int16_t)0x7D4A, (int16_t)0xE5CC, (int16_t)0x7546, (int16_t)0xCCB4, (int16_t)0x684A, (int16_t)0xB5C9, (int16_t)0x56E3, (int16_t)0xA202,
    (int16_t)0x7D07, (int16_t)0xE492, (int16_t)0x743E, (int16_t)0xCA69, (int16_t)0x660F, (int16_t)0xB2BF, (int16_t)0x5321, (int16_t)0x9EAB,
    (int16_t)0x7CC0, (int16_t)0xE358, (int16_t)0x732B, (int16_t)0xC824, (int16_t)0x63BD, (int16_t)0xAFC6, (int16_t)0x4F3E, (int16_t)0x9B7B,
    (int16_t)0x7C77, (int16_t)0xE21E, (int16_t)0x720D, (int16_t)0xC5E4, (int16_t)0x6155, (int16_t)0xACDF, (int16_t)0x4B3D, (int16_t)0x9872,
    (int16_t)0x7C2A, (int16_t)0xE0E6, (int16_t)0x70E3, (int16_t)0xC3A9, (int16_t)0x5ED7, (int16_t)0xAA0A, (int16_t)0x471D, (int16_t)0x9592,
    (int16_t)0x7BDA, (int16_t)0xDFAE, (int16_t)0x6FAE, (int16_t)0xC175, (int16_t)0x5C45, (int16_t)0xA749, (int16_t)0x42E1, (int16_t)0x92DD,
    (int16_t)0x7B88, (int16_t)0xDE77, (int16_t)0x6E6E, (int16_t)0xBF47, (int16_t)0x599E, (int16_t)0xA49B, (int16_t)0x3E8B, (int16_t)0x9052,
    (int16_t)0x7B32, (int16_t)0xDD41, (int16_t)0x6D23, (int16_t)0xBD1F, (int16_t)0x56E3, (int16_t)0xA202, (int16_t)0x3A1C, (int16_t)0x8DF3,
    (int16_t)0x7AD9, (int16_t)0xDC0C, (int16_t)0x6BCE, (int16_t)0xBAFE, (int16_t)0x5415, (int16_t)0x9F7D, (int16_t)0x3597, (int16_t)0x8BC2,
    (int16_t)0x7A7D, (int16_t)0xDAD8, (int16_t)0x6A6E, (int16_t)0xB8E3, (int16_t)0x5134, (int16_t)0x9D0E, (int16_t)0x30FC, (int16_t)0x89BE,
    (int16_t)0x7A1E, (int16_t)0xD9A5, (int16_t)0x6903, (int16_t)0xB6D0, (int16_t)0x4E41, (int16_t)0x9AB5, (int16_t)0x2C4E, (int16_t)0x87E9,
    (int16_t)0x79BC, (int16_t)0xD872, (int16_t)0x678E, (int16_t)0xB4C3, (int16_t)0x4B3D, (int16_t)0x9872, (int16_t)0x278E, (int16_t)0x8644,
    (int16_t)0x7957, (int16_t)0xD741, (int16_t)0x660F, (int16_t)0xB2BF, (int16_t)0x4828, (int16_t)0x9646, (int16_t)0x22BF, (int16_t)0x84CE,
    (int16_t)0x78EF, (int16_t)0xD610, (int16_t)0x6485, (int16_t)0xB0C2, (int16_t)0x4502, (int16_t)0x9432, (int16_t)0x1DE2, (int16_t)0x8389,
    (int16_t)0x7885, (int16_t)0xD4E1, (int16_t)0x62F2, (int16_t)0xAECC, (int16_t)0x41CE, (int16_t)0x9236, (int16_t)0x18F9, (int16_t)0x8276,
    (int16_t)0x7817, (int16_t)0xD3B2, (int16_t)0x6155, (int16_t)0xACDF, (int16_t)0x3E8B, (int16_t)0x9052, (int16_t)0x1406, (int16_t)0x8193,
    (int16_t)0x77A6, (int16_t)0xD285, (int16_t)0x5FAE, (int16_t)0xAAFA, (int16_t)0x3B3A, (int16_t)0x8E87, (int16_t)0x0F0B, (int16_t)0x80E3,
    (int16_t)0x7732, (int16_t)0xD159, (int16_t)0x5DFE, (int16_t)0xA91D, (int16_t)0x37DC, (int16_t)0x8CD5, (int16_t)0x0A0B, (int16_t)0x8065,
    (int16_t)0x76BB, (int16_t)0xD02E, (int16_t)0x5C45, (int16_t)0xA749, (int16_t)0x3472, (int16_t)0x8B3D, (int16_t)0x0506, (int16_t)0x8019,
    (int16_t)0x7642, (int16_t)0xCF04, (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0x0000, (int16_t)0x8000,
    (int16_t)0x75C5, (int16_t)0xCDDC, (int16_t)0x58B7, (int16_t)0xA3BB, (int16_t)0x2D7B, (int16_t)0x885A, (int16_t)0xFAFA, (int16_t)0x8019,
    (int16_t)0x7546, (int16_t)0xCCB4, (int16_t)0x56E3, (int16_t)0xA202, (int16_t)0x29F0, (int16_t)0x8711, (int16_t)0xF5F5, (int16_t)0x8065,
    (int16_t)0x74C3, (int16_t)0xCB8E, (int16_t)0x5506, (int16_t)0xA052, (int16_t)0x265B, (int16_t)0x85E2, (int16_t)0xF0F5, (int16_t)0x80E3,
    (int16_t)0x743E, (int16_t)0xCA69, (int16_t)0x5321, (int16_t)0x9EAB, (int16_t)0x22BF, (int16_t)0x84CE, (int16_t)0xEBFA, (int16_t)0x8193,
    (int16_t)0x73B6, (int16_t)0xC946, (int16_t)0x5134, (int16_t)0x9D0E, (int16_t)0x1F1A, (int16_t)0x83D6, (int16_t)0xE707, (int16_t)0x8276,
    (int16_t)0x732B, (int16_t)0xC824, (int16_t)0x4F3E, (int16_t)0x9B7B, (int16_t)0x1B6E, (int16_t)0x82F9, (int16_t)0xE21E, (int16_t)0x8389,
    (int16_t)0x729D, (int16_t)0xC703, (int16_t)0x4D41, (int16_t)0x99F1, (int16_t)0x17BD, (int16_t)0x8238, (int16_t)0xDD41, (int16_t)0x84CE,
    (int16_t)0x720D, (int16_t)0xC5E4, (int16_t)0x4B3D, (int16_t)0x9872, (int16_t)0x1406, (int16_t)0x8193, (int16_t)0xD872, (int16_t)0x8644,
    (int16_t)0x7179, (int16_t)0xC4C6, (int16_t)0x4930, (int16_t)0x96FD, (int16_t)0x104B, (int16_t)0x810B, (int16_t)0xD3B2, (int16_t)0x87E9,
    (int16_t)0x70E3, (int16_t)0xC3A9, (int16_t)0x471D, (int16_t)0x9592, (int16_t)0x0C8C, (int16_t)0x809E, (int16_t)0xCF04, (int16_t)0x89BE,
    (int16_t)0x704A, (int16_t)0xC28E, (int16_t)0x4502, (int16_t)0x9432, (int16_t)0x08CA, (int16_t)0x804D, (int16_t)0xCA69, (int16_t)0x8BC2,
    (int16_t)0x6FAE, (int16_t)0xC175, (int16_t)0x42E1, (int16_t)0x92DD, (int16_t)0x0506, (int16_t)0x8019, (int16_t)0xC5E4, (int16_t)0x8DF3,
    (int16_t)0x6F0F, (int16_t)0xC05D, (int16_t)0x40B9, (int16_t)0x9192, (int16_t)0x0142, (int16_t)0x8002, (int16_t)0xC175, (int16_t)0x9052,
    (int16_t)0x6E6E, (int16_t)0xBF47, (int16_t)0x3E8B, (int16_t)0x9052, (int16_t)0xFD7D, (int16_t)0x8006, (int16_t)0xBD1F, (int16_t)0x92DD,
    (int16_t)0x6DCA, (int16_t)0xBE32, (int16_t)0x3C57, (int16_t)0x8F1D, (int16_t)0xF9B8, (int16_t)0x8027, (int16_t)0xB8E3, (int16_t)0x9592,
    (int16_t)0x6D23, (int16_t)0xBD1F, (int16_t)0x3A1C, (int16_t)0x8DF3, (int16_t)0xF5F5, (int16_t)0x8065, (int16_t)0xB4C3, (int16_t)0x9872,
    (int16_t)0x6C7A, (int16_t)0xBC0D, (int16_t)0x37DC, (int16_t)0x8CD5, (int16_t)0xF234, (int16_t)0x80BF, (int16_t)0xB0C2, (int16_t)0x9B7B,
    (int16_t)0x6BCE, (int16_t)0xBAFE, (int16_t)0x3597, (int16_t)0x8BC2, (int16_t)0xEE76, (int16_t)0x8135, (int16_t)0xACDF, (int16_t)0x9EAB,
    (int16_t)0x6B1F, (int16_t)0xB9EF, (int16_t)0x334C, (int16_t)0x8ABA, (int16_t)0xEABC, (int16_t)0x81C7, (int16_t)0xA91D, (int16_t)0xA202,
    (int16_t)0x6A6E, (int16_t)0xB8E3, (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0xE707, (int16_t)0x8276, (int16_t)0xA57E, (int16_t)0xA57E,
    (int16_t)0x69BA, (int16_t)0xB7D8, (int16_t)0x2EA7, (int16_t)0x88CE, (int16_t)0xE358, (int16_t)0x8340, (int16_t)0xA202, (int16_t)0xA91D,
    (int16_t)0x6903, (int16_t)0xB6D0, (int16_t)0x2C4E, (int16_t)0x87E9, (int16_t)0xDFAE, (int16_t)0x8426, (int16_t)0x9EAB, (int16_t)0xACDF,
    (int16_t)0x684A, (int16_t)0xB5C9, (int16_t)0x29F0, (int16_t)0x8711, (int16_t)0xDC0C, (int16_t)0x8527, (int16_t)0x9B7B, (int16_t)0xB0C2,
    (int16_t)0x678E, (int16_t)0xB4C3, (int16_t)0x278E, (int16_t)0x8644, (int16_t)0xD872, (int16_t)0x8644, (int16_t)0x9872, (int16_t)0xB4C3,
    (int16_t)0x66D0, (int16_t)0xB3C0, (int16_t)0x2528, (int16_t)0x8583, (int16_t)0xD4E1, (int16_t)0x877B, (int16_t)0x9592, (int16_t)0xB8E3,
    (int16_t)0x660F, (int16_t)0xB2BF, (int16_t)0x22BF, (int16_t)0x84CE, (int16_t)0xD159, (int16_t)0x88CE, (int16_t)0x92DD, (int16_t)0xBD1F,
    (int16_t)0x654B, (int16_t)0xB1BF, (int16_t)0x2052, (int16_t)0x8426, (int16_t)0xCDDC, (int16_t)0x8A3B, (int16_t)0x9052, (int16_t)0xC175,
    (int16_t)0x6485, (int16_t)0xB0C2, (int16_t)0x1DE2, (int16_t)0x8389, (int16_t)0xCA69, (int16_t)0x8BC2, (int16_t)0x8DF3, (int16_t)0xC5E4,
    (int16_t)0x63BD, (int16_t)0xAFC6, (int16_t)0x1B6E, (int16_t)0x82F9, (int16_t)0xC703, (int16_t)0x8D63, (int16_t)0x8BC2, (int16_t)0xCA69,
    (int16_t)0x62F2, (int16_t)0xAECC, (int16_t)0x18F9, (int16_t)0x8276, (int16_t)0xC3A9, (int16_t)0x8F1D, (int16_t)0x89BE, (int16_t)0xCF04,
    (int16_t)0x6225, (int16_t)0xADD5, (int16_t)0x1680, (int16_t)0x81FE, (int16_t)0xC05D, (int16_t)0x90F1, (int16_t)0x87E9, (int16_t)0xD3B2,
    (int16_t)0x6155, (int16_t)0xACDF, (int16_t)0x1406, (int16_t)0x8193, (int16_t)0xBD1F, (int16_t)0x92DD, (int16_t)0x8644, (int16_t)0xD872,
    (int16_t)0x6083, (int16_t)0xABEB, (int16_t)0x118A, (int16_t)0x8135, (int16_t)0xB9EF, (int16_t)0x94E1, (int16_t)0x84CE, (int16_t)0xDD41,
    (int16_t)0x5FAE, (int16_t)0xAAFA, (int16_t)0x0F0B, (int16_t)0x80E3, (int16_t)0xB6D0, (int16_t)0x96FD, (int16_t)0x8389, (int16_t)0xE21E,
    (int16_t)0x5ED7, (int16_t)0xAA0A, (int16_t)0x0C8C, (int16_t)0x809E, (int16_t)0xB3C0, (int16_t)0x9930, (int16_t)0x8276, (int16_t)0xE707,
    (int16_t)0x5DFE, (int16_t)0xA91D, (int16_t)0x0A0B, (int16_t)0x8065, (int16_t)0xB0C2, (int16_t)0x9B7B, (int16_t)0x8193, (int16_t)0xEBFA,
    (int16_t)0x5D23, (int16_t)0xA832, (int16_t)0x0789, (int16_t)0x8039, (int16_t)0xADD5, (int16_t)0x9DDB, (int16_t)0x80E3, (int16_t)0xF0F5,
    (int16_t)0x5C45, (int16_t)0xA749, (int16_t)0x0506, (int16_t)0x8019, (int16_t)0xAAFA, (int16_t)0xA052, (int16_t)0x8065, (int16_t)0xF5F5,
    (int16_t)0x5B65, (int16_t)0xA662, (int16_t)0x0283, (int16_t)0x8006, (int16_t)0xA832, (int16_t)0xA2DD, (int16_t)0x8019, (int16_t)0xFAFA,
    (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x0000, (int16_t)0x8000, (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0x8000, (int16_t)0x0000,
    (int16_t)0x599E, (int16_t)0xA49B, (int16_t)0xFD7D, (int16_t)0x8006, (int16_t)0xA2DD, (int16_t)0xA832, (int16_t)0x8019, (int16_t)0x0506,
    (int16_t)0x58B7, (int16_t)0xA3BB, (int16_t)0xFAFA, (int16_t)0x8019, (int16_t)0xA052, (int16_t)0xAAFA, (int16_t)0x8065, (int16_t)0x0A0B,
    (int16_t)0x57CE, (int16_t)0xA2DD, (int16_t)0xF877, (int16_t)0x8039, (int16_t)0x9DDB, (int16_t)0xADD5, (int16_t)0x80E3, (int16_t)0x0F0B,
    (int16_t)0x56E3, (int16_t)0xA202, (int16_t)0xF5F5, (int16_t)0x8065, (int16_t)0x9B7B, (int16_t)0xB0C2, (int16_t)0x8193, (int16_t)0x1406,
    (int16_t)0x55F6, (int16_t)0xA129, (int16_t)0xF374, (int16_t)0x809E, (int16_t)0x9930, (int16_t)0xB3C0, (int16_t)0x8276, (int16_t)0x18F9,
    (int16_t)0x5506, (int16_t)0xA052, (int16_t)0xF0F5, (int16_t)0x80E3, (int16_t)0x96FD, (int16_t)0xB6D0, (int16_t)0x8389, (int16_t)0x1DE2,
    (int16_t)0x5415, (int16_t)0x9F7D, (int16_t)0xEE76, (int16_t)0x8135, (int16_t)0x94E1, (int16_t)0xB9EF, (int16_t)0x84CE, (int16_t)0x22BF,
    (int16_t)0x5321, (int16_t)0x9EAB, (int16_t)0xEBFA, (int16_t)0x8193, (int16_t)0x92DD, (int16_t)0xBD1F, (int16_t)0x8644, (int16_t)0x278E,
    (int16_t)0x522B, (int16_t)0x9DDB, (int16_t)0xE980, (int16_t)0x81FE, (int16_t)0x90F1, (int16_t)0xC05D, (int16_t)0x87E9, (int16_t)0x2C4E,
    (int16_t)0x5134, (int16_t)0x9D0E, (int16_t)0xE707, (int16_t)0x8276, (int16_t)0x8F1D, (int16_t)0xC3A9, (int16_t)0x89BE, (int16_t)0x30FC,
    (int16_t)0x503A, (int16_t)0x9C43, (int16_t)0xE492, (int16_t)0x82F9, (int16_t)0x8D63, (int16_t)0xC703, (int16_t)0x8BC2, (int16_t)0x3597,
    (int16_t)0x4F3E, (int16_t)0x9B7B, (int16_t)0xE21E, (int16_t)0x8389, (int16_t)0x8BC2, (int16_t)0xCA69, (int16_t)0x8DF3, (int16_t)0x3A1C,
    (int16_t)0x4E41, (int16_t)0x9AB5, (int16_t)0xDFAE, (int16_t)0x8426, (int16_t)0x8A3B, (int16_t)0xCDDC, (int16_t)0x9052, (int16_t)0x3E8B,
    (int16_t)0x4D41, (int16_t)0x99F1, (int16_t)0xDD41, (int16_t)0x84CE, (int16_t)0x88CE, (int16_t)0xD159, (int16_t)0x92DD, (int16_t)0x42E1,
    (int16_t)0x4C40, (int16_t)0x9930, (int16_t)0xDAD8, (int16_t)0x8583, (int16_t)0x877B, (int16_t)0xD4E1, (int16_t)0x9592, (int16_t)0x471D,
    (int16_t)0x4B3D, (int16_t)0x9872, (int16_t)0xD872, (int16_t)0x8644, (int16_t)0x8644, (int16_t)0xD872, (int16_t)0x9872, (int16_t)0x4B3D,
    (int16_t)0x4A37, (int16_t)0x97B6, (int16_t)0xD610, (int16_t)0x8711, (int16_t)0x8527, (int16_t)0xDC0C, (int16_t)0x9B7B, (int16_t)0x4F3E,
    (int16_t)0x4930, (int16_t)0x96FD, (int16_t)0xD3B2, (int16_t)0x87E9, (int16_t)0x8426, (int16_t)0xDFAE, (int16_t)0x9EAB, (int16_t)0x5321,
    (int16_t)0x4828, (int16_t)0x9646, (int16_t)0xD159, (int16_t)0x88CE, (int16_t)0x8340, (int16_t)0xE358, (int16_t)0xA202, (int16_t)0x56E3,
    (int16_t)0x471D, (int16_t)0x9592, (int16_t)0xCF04, (int16_t)0x89BE, (int16_t)0x8276, (int16_t)0xE707, (int16_t)0xA57E, (int16_t)0x5A82,
    (int16_t)0x4611, (int16_t)0x94E1, (int16_t)0xCCB4, (int16_t)0x8ABA, (int16_t)0x81C7, (int16_t)0xEABC, (int16_t)0xA91D, (int16_t)0x5DFE,
    (int16_t)0x4502, (int16_t)0x9432, (int16_t)0xCA69, (int16_t)0x8BC2, (int16_t)0x8135, (int16_t)0xEE76, (int16_t)0xACDF, (int16_t)0x6155,
    (int16_t)0x43F3, (int16_t)0x9386, (int16_t)0xC824, (int16_t)0x8CD5, (int16_t)0x80BF, (int16_t)0xF234, (int16_t)0xB0C2, (int16_t)0x6485,
    (int16_t)0x42E1, (int16_t)0x92DD, (int16_t)0xC5E4, (int16_t)0x8DF3, (int16_t)0x8065, (int16_t)0xF5F5, (int16_t)0xB4C3, (int16_t)0x678E,
    (int16_t)0x41CE, (int16_t)0x9236, (int16_t)0xC3A9, (int16_t)0x8F1D, (int16_t)0x8027, (int16_t)0xF9B8, (int16_t)0xB8E3, (int16_t)0x6A6E,
    (int16_t)0x40B9, (int16_t)0x9192, (int16_t)0xC175, (int16_t)0x9052, (int16_t)0x8006, (int16_t)0xFD7D, (int16_t)0xBD1F, (int16_t)0x6D23,
    (int16_t)0x3FA3, (int16_t)0x90F1, (int16_t)0xBF47, (int16_t)0x9192, (int16_t)0x8002, (int16_t)0x0142, (int16_t)0xC175, (int16_t)0x6FAE,
    (int16_t)0x3E8B, (int16_t)0x9052, (int16_t)0xBD1F, (int16_t)0x92DD, (int16_t)0x8019, (int16_t)0x0506, (int16_t)0xC5E4, (int16_t)0x720D,
    (int16_t)0x3D72, (int16_t)0x8FB6, (int16_t)0xBAFE, (int16_t)0x9432, (int16_t)0x804D, (int16_t)0x08CA, (int16_t)0xCA69, (int16_t)0x743E,
    (int16_t)0x3C57, (int16_t)0x8F1D, (int16_t)0xB8E3, (int16_t)0x9592, (int16_t)0x809E, (int16_t)0x0C8C, (int16_t)0xCF04, (int16_t)0x7642,
    (int16_t)0x3B3A, (int16_t)0x8E87, (int16_t)0xB6D0, (int16_t)0x96FD, (int16_t)0x810B, (int16_t)0x104B, (int16_t)0xD3B2, (int16_t)0x7817,
    (int16_t)0x3A1C, (int16_t)0x8DF3, (int16_t)0xB4C3, (int16_t)0x9872, (int16_t)0x8193, (int16_t)0x1406, (int16_t)0xD872, (int16_t)0x79BC,
    (int16_t)0x38FD, (int16_t)0x8D63, (int16_t)0xB2BF, (int16_t)0x99F1, (int16_t)0x8238, (int16_t)0x17BD, (int16_t)0xDD41, (int16_t)0x7B32,
    (int16_t)0x37DC, (int16_t)0x8CD5, (int16_t)0xB0C2, (int16_t)0x9B7B, (int16_t)0x82F9, (int16_t)0x1B6E, (int16_t)0xE21E, (int16_t)0x7C77,
    (int16_t)0x36BA, (int16_t)0x8C4A, (int16_t)0xAECC, (int16_t)0x9D0E, (int16_t)0x83D6, (int16_t)0x1F1A, (int16_t)0xE707, (int16_t)0x7D8A,
    (int16_t)0x3597, (int16_t)0x8BC2, (int16_t)0xACDF, (int16_t)0x9EAB, (int16_t)0x84CE, (int16_t)0x22BF, (int16_t)0xEBFA, (int16_t)0x7E6D,
    (int16_t)0x3472, (int16_t)0x8B3D, (int16_t)0xAAFA, (int16_t)0xA052, (int16_t)0x85E2, (int16_t)0x265B, (int16_t)0xF0F5, (int16_t)0x7F1D,
    (int16_t)0x334C, (int16_t)0x8ABA, (int16_t)0xA91D, (int16_t)0xA202, (int16_t)0x8711, (int16_t)0x29F0, (int16_t)0xF5F5, (int16_t)0x7F9B,
    (int16_t)0x3224, (int16_t)0x8A3B, (int16_t)0xA749, (int16_t)0xA3BB, (int16_t)0x885A, (int16_t)0x2D7B, (int16_t)0xFAFA, (int16_t)0x7FE7,
    (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0x89BE, (int16_t)0x30FC, (int16_t)0x0000, (int16_t)0x7FFF,
    (int16_t)0x2FD2, (int16_t)0x8945, (int16_t)0xA3BB, (int16_t)0xA749, (int16_t)0x8B3D, (int16_t)0x3472, (int16_t)0x0506, (int16_t)0x7FE7,
    (int16_t)0x2EA7, (int16_t)0x88CE, (int16_t)0xA202, (int16_t)0xA91D, (int16_t)0x8CD5, (int16_t)0x37DC, (int16_t)0x0A0B, (int16_t)0x7F9B,
    (int16_t)0x2D7B, (int16_t)0x885A, (int16_t)0xA052, (int16_t)0xAAFA, (int16_t)0x8E87, (int16_t)0x3B3A, (int16_t)0x0F0B, (int16_t)0x7F1D,
    (int16_t)0x2C4E, (int16_t)0x87E9, (int16_t)0x9EAB, (int16_t)0xACDF, (int16_t)0x9052, (int16_t)0x3E8B, (int16_t)0x1406, (int16_t)0x7E6D,
    (int16_t)0x2B1F, (int16_t)0x877B, (int16_t)0x9D0E, (int16_t)0xAECC, (int16_t)0x9236, (int16_t)0x41CE, (int16_t)0x18F9, (int16_t)0x7D8A,
    (int16_t)0x29F0, (int16_t)0x8711, (int16_t)0x9B7B, (int16_t)0xB0C2, (int16_t)0x9432, (int16_t)0x4502, (int16_t)0x1DE2, (int16_t)0x7C77,
    (int16_t)0x28BF, (int16_t)0x86A9, (int16_t)0x99F1, (int16_t)0xB2BF, (int16_t)0x9646, (int16_t)0x4828, (int16_t)0x22BF, (int16_t)0x7B32,
};

/********** Twiddles table N=640 stage 2 radix 4 ******************/
ALIGN(8) static const int16_t _fft640_tw2_[] =
{
    (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FD9, (int16_t)0xF9B8,
    (int16_t)0x7F62, (int16_t)0xF374, (int16_t)0x7E9D, (int16_t)0xED38, (int16_t)0x7F62, (int16_t)0xF374, (int16_t)0x7D8A, (int16_t)0xE707,
    (int16_t)0x7A7D, (int16_t)0xDAD8, (int16_t)0x7E9D, (int16_t)0xED38, (int16_t)0x7A7D, (int16_t)0xDAD8, (int16_t)0x73B6, (int16_t)0xC946,
    (int16_t)0x7D8A, (int16_t)0xE707, (int16_t)0x7642, (int16_t)0xCF04, (int16_t)0x6A6E, (int16_t)0xB8E3, (int16_t)0x7C2A, (int16_t)0xE0E6,
    (int16_t)0x70E3, (int16_t)0xC3A9, (int16_t)0x5ED7, (int16_t)0xAA0A, (int16_t)0x7A7D, (int16_t)0xDAD8, (int16_t)0x6A6E, (int16_t)0xB8E3,
    (int16_t)0x5134, (int16_t)0x9D0E, (int16_t)0x7885, (int16_t)0xD4E1, (int16_t)0x62F2, (int16_t)0xAECC, (int16_t)0x41CE, (int16_t)0x9236,
    (int16_t)0x7642, (int16_t)0xCF04, (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0x73B6, (int16_t)0xC946,
    (int16_t)0x5134, (int16_t)0x9D0E, (int16_t)0x1F1A, (int16_t)0x83D6, (int16_t)0x70E3, (int16_t)0xC3A9, (int16_t)0x471D, (int16_t)0x9592,
    (int16_t)0x0C8C, (int16_t)0x809E, (int16_t)0x6DCA, (int16_t)0xBE32, (int16_t)0x3C57, (int16_t)0x8F1D, (int16_t)0xF9B8, (int16_t)0x8027,
    (int16_t)0x6A6E, (int16_t)0xB8E3, (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0xE707, (int16_t)0x8276, (int16_t)0x66D0, (int16_t)0xB3C0,
    (int16_t)0x2528, (int16_t)0x8583, (int16_t)0xD4E1, (int16_t)0x877B, (int16_t)0x62F2, (int16_t)0xAECC, (int16_t)0x18F9, (int16_t)0x8276,
    (int16_t)0xC3A9, (int16_t)0x8F1D, (int16_t)0x5ED7, (int16_t)0xAA0A, (int16_t)0x0C8C, (int16_t)0x809E, (int16_t)0xB3C0, (int16_t)0x9930,
    (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x0000, (int16_t)0x8000, (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0x55F6, (int16_t)0xA129,
    (int16_t)0xF374, (int16_t)0x809E, (int16_t)0x9930, (int16_t)0xB3C0, (int16_t)0x5134, (int16_t)0x9D0E, (int16_t)0xE707, (int16_t)0x8276,
    (int16_t)0x8F1D, (int16_t)0xC3A9, (int16_t)0x4C40, (int16_t)0x9930, (int16_t)0xDAD8, (int16_t)0x8583, (int16_t)0x877B, (int16_t)0xD4E1,
    (int16_t)0x471D, (int16_t)0x9592, (int16_t)0xCF04, (int16_t)0x89BE, (int16_t)0x8276, (int16_t)0xE707, (int16_t)0x41CE, (int16_t)0x9236,
    (int16_t)0xC3A9, (int16_t)0x8F1D, (int16_t)0x8027, (int16_t)0xF9B8, (int16_t)0x3C57, (int16_t)0x8F1D, (int16_t)0xB8E3, (int16_t)0x9592,
    (int16_t)0x809E, (int16_t)0x0C8C, (int16_t)0x36BA, (int16_t)0x8C4A, (int16_t)0xAECC, (int16_t)0x9D0E, (int16_t)0x83D6, (int16_t)0x1F1A,
    (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0x89BE, (int16_t)0x30FC, (int16_t)0x2B1F, (int16_t)0x877B,
    (int16_t)0x9D0E, (int16_t)0xAECC, (int16_t)0x9236, (int16_t)0x41CE, (int16_t)0x2528, (int16_t)0x8583, (int16_t)0x9592, (int16_t)0xB8E3,
    (int16_t)0x9D0E, (int16_t)0x5134, (int16_t)0x1F1A, (int16_t)0x83D6, (int16_t)0x8F1D, (int16_t)0xC3A9, (int16_t)0xAA0A, (int16_t)0x5ED7,
    (int16_t)0x18F9, (int16_t)0x8276, (int16_t)0x89BE, (int16_t)0xCF04, (int16_t)0xB8E3, (int16_t)0x6A6E, (int16_t)0x12C8, (int16_t)0x8163,
    (int16_t)0x8583, (int16_t)0xDAD8, (int16_t)0xC946, (int16_t)0x73B6, (int16_t)0x0C8C, (int16_t)0x809E, (int16_t)0x8276, (int16_t)0xE707,
    (int16_t)0xDAD8, (int16_t)0x7A7D, (int16_t)0x0648, (int16_t)0x8027, (int16_t)0x809E, (int16_t)0xF374, (int16_t)0xED38, (int16_t)0x7E9D,
};

/********** Twiddles table N=640 stage 3 radix 4 ******************/
ALIGN(8) static const int16_t _fft640_tw3_[] =
{
    (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7D8A, (int16_t)0xE707,
    (int16_t)0x7642, (int16_t)0xCF04, (int16_t)0x6A6E, (int16_t)0xB8E3, (int16_t)0x7642, (int16_t)0xCF04, (int16_t)0x5A82, (int16_t)0xA57E,
    (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0x6A6E, (int16_t)0xB8E3, (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0xE707, (int16_t)0x8276,
    (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x0000, (int16_t)0x8000, (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0x471D, (int16_t)0x9592,
    (int16_t)0xCF04, (int16_t)0x89BE, (int16_t)0x8276, (int16_t)0xE707, (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0xA57E, (int16_t)0xA57E,
    (int16_t)0x89BE, (int16_t)0x30FC, (int16_t)0x18F9, (int16_t)0x8276, (int16_t)0x89BE, (int16_t)0xCF04, (int16_t)0xB8E3, (int16_t)0x6A6E,
};

static const int tw_step_tab[] =
{
    1, 1, 1, 0
}; 
static const cint16ptr_t tw_tab[] = 
{
    _fft640_tw1_, _fft640_tw2_, _fft640_tw3_, NULL
};

static const eFft_stage_type fft_stg_tab[] = 
{
    fft_stage_first_DFT5,
    fft_stage_inner_DFT4,
    fft_stage_inner_DFT4,
    fft_stage_last_DFT8
};
static const eFft_stage_type ifft_stg_tab[] =
{
    fft_stage_first_inv_DFT5,
    fft_stage_inner_DFT4,
    fft_stage_inner_DFT4,
    fft_stage_last_DFT8
};


const fft_cplx_x16_descr_t __cfft_x16_descr640 = 
{
    N, 
    fft_stg_tab, 
    fft_stg_tab, 
    fft_stg_tab, 
    fft_stg_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_cplx_x16_descr_t __cifft_x16_descr640 =
{
    N, 
    ifft_stg_tab, 
    ifft_stg_tab, 
    ifft_stg_tab, 
    ifft_stg_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft16_640  = (const fft_handle_t)&__cfft_x16_descr640;
const fft_handle_t cinfft16_640 = (const fft_handle_t)&__cifft_x16_descr640;
