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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFC88C1, (int32_t)0xFE236CF2, (int32_t)0x7FF22333, (int32_t)0xFC46F3B2, (int32_t)0x7FE0CFE7, (int32_t)0xFA6AAE0F,
    (int32_t)0x7FC88FCE, (int32_t)0xF88EB5D3, (int32_t)0x7FA96436, (int32_t)0xF6B324C4, (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7F5651AD, (int32_t)0xF2FD9F2C,
    (int32_t)0x7F226F3C, (int32_t)0xF123DE0D, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7EA6060C, (int32_t)0xED72DF72, (int32_t)0x7E5D8609, (int32_t)0xEB9BD522,
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7DB802D2, (int32_t)0xE7F12A05, (int32_t)0x7D5B0893, (int32_t)0xE61DBC0E, (int32_t)0x7CF7447F, (int32_t)0xE44BB4EC,
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD, (int32_t)0x7C1B74D0, (int32_t)0xE0AC4007, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7B24C362, (int32_t)0xDD139439,
    (int32_t)0x7A9F667D, (int32_t)0xDB4A081C, (int32_t)0x7A1365A5, (int32_t)0xD98278ED, (int32_t)0x7980C86F, (int32_t)0xD7BCFF58, (int32_t)0x78E796CD, (int32_t)0xD5F9B3EB,
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x77A197CB, (int32_t)0xD27A092D, (int32_t)0x76F4DC14, (int32_t)0xD0BDDA5C, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3,
    (int32_t)0x75881AFB, (int32_t)0xCD4D421B, (int32_t)0x74C8295B, (int32_t)0xCB99085B, (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x733557EA, (int32_t)0xC8392FB7,
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B, (int32_t)0x7189922C, (int32_t)0xC4E56BE4, (int32_t)0x70AA706F, (int32_t)0xC3404B8B, (int32_t)0x6FC534C8, (int32_t)0xC19E755F,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6DE8A1B9, (int32_t)0xBE6501E1, (int32_t)0x6CF16421, (int32_t)0xBCCD9142, (int32_t)0x6BF4403B, (int32_t)0xBB39C435,
    (int32_t)0x6AF143BD, (int32_t)0xB9A9B097, (int32_t)0x69E87CAE, (int32_t)0xB81D6C13, (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x67C5C886, (int32_t)0xB510A5F8,
    (int32_t)0x66ABF908, (int32_t)0xB3904EA9, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x6467BB86, (int32_t)0xB09C1F99, (int32_t)0x633D6CED, (int32_t)0xAF2870CA,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x60D8C0D2, (int32_t)0xAC4E4943, (int32_t)0x5F9E847D, (int32_t)0xAAE7F815, (int32_t)0x5E5F1A91, (int32_t)0xA9864294,
    (int32_t)0x5D1A9459, (int32_t)0xA8293BE8, (int32_t)0x5BD10369, (int32_t)0xA6D0F6F7, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x592F0909, (int32_t)0xA42EFC97,
    (int32_t)0x57D6C418, (int32_t)0xA2E56BA7, (int32_t)0x5679BD6C, (int32_t)0xA1A0E56F, (int32_t)0x551807EB, (int32_t)0xA0617B83, (int32_t)0x53B1B6BD, (int32_t)0x9F273F2E,
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0x50D78F36, (int32_t)0x9CC29313, (int32_t)0x4F63E067, (int32_t)0x9B98447A, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3,
    (int32_t)0x4C6FB157, (int32_t)0x995406F8, (int32_t)0x4AEF5A08, (int32_t)0x983A377A, (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0x47E293ED, (int32_t)0x96178352,
    (int32_t)0x46564F69, (int32_t)0x950EBC43, (int32_t)0x44C63BCB, (int32_t)0x940BBFC5, (int32_t)0x43326EBE, (int32_t)0x930E9BDF, (int32_t)0x419AFE1F, (int32_t)0x92175E47,
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x3E618AA1, (int32_t)0x903ACB38, (int32_t)0x3CBFB475, (int32_t)0x8F558F91, (int32_t)0x3B1A941C, (int32_t)0x8E766DD4,
    (int32_t)0x39724065, (int32_t)0x8D9D7216, (int32_t)0x37C6D049, (int32_t)0x8CCAA816, (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0x3466F7A5, (int32_t)0x8B37D6A5,
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x2F4225A4, (int32_t)0x890B23EC, (int32_t)0x2D85F6D3, (int32_t)0x885E6835,
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x2A064C15, (int32_t)0x87186933, (int32_t)0x284300A8, (int32_t)0x867F3791, (int32_t)0x267D8713, (int32_t)0x85EC9A5B,
    (int32_t)0x24B5F7E4, (int32_t)0x85609983, (int32_t)0x22EC6BC7, (int32_t)0x84DB3C9E, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x1F53BFF9, (int32_t)0x83E48B30,
    (int32_t)0x1D84D223, (int32_t)0x83734404, (int32_t)0x1BB44B14, (int32_t)0x8308BB81, (int32_t)0x19E243F2, (int32_t)0x82A4F76D, (int32_t)0x180ED5FB, (int32_t)0x8247FD2E,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x14642ADE, (int32_t)0x81A279F7, (int32_t)0x128D208E, (int32_t)0x8159F9F4, (int32_t)0x10B5150F, (int32_t)0x811855B4,
    (int32_t)0x0EDC21F3, (int32_t)0x80DD90C4, (int32_t)0x0D0260D4, (int32_t)0x80A9AE53, (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0x094CDB3C, (int32_t)0x80569BCA,
    (int32_t)0x07714A2D, (int32_t)0x80377032, (int32_t)0x059551F1, (int32_t)0x801F3019, (int32_t)0x03B90C4E, (int32_t)0x800DDCCD, (int32_t)0x01DC930E, (int32_t)0x8003773F,
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr216_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr216_32x32,
    __fft_real_tw
};

const fft_handle_t  rnfft32_432 = (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_432 = (const fft_handle_t)&__rifft_descr;
