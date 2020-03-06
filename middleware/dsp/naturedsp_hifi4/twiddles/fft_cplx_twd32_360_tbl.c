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

/****************** stage 1 radix 2 ******************/
ALIGN(8) static const int32_t __fft360_tw1[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFB0260, (int32_t)0xFDC41E9B, (int32_t)0x7FEC09E3, (int32_t)0xFB8869CE, (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, 
    (int32_t)0x7FB02DC6, (int32_t)0xF7123849, (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7F0BC097, (int32_t)0xF06695DA, 
    (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369, (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7DA5F5A5, (int32_t)0xE7939223, 
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x7CB82885, (int32_t)0xE334CDC9, (int32_t)0x7C32A67E, (int32_t)0xE108B40D, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, 
    (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46, (int32_t)0x7A6831BA, (int32_t)0xDA939061, (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x7906C0B0, (int32_t)0xD653C860, 
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x76ADF5E6, (int32_t)0xD00CE422, (int32_t)0x75D31A61, (int32_t)0xCDFC85BB, 
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x730BAEED, (int32_t)0xC7E3744B, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, 
    (int32_t)0x71046D3E, (int32_t)0xC3E85B18, (int32_t)0x6FF389DF, (int32_t)0xC1F1C224, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6DB7A87A, (int32_t)0xBE133B7C, 
    (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05, (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x6A1DE737, (int32_t)0xB86C5DF0, (int32_t)0x68D9F964, (int32_t)0xB6950C1E, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x6639B03B, (int32_t)0xB2F7B9AF, (int32_t)0x64DD8950, (int32_t)0xB1320139, (int32_t)0x637984D4, (int32_t)0xAF726DEF, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x609A52D3, (int32_t)0xAC0641FB, (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x5D9CFF83, (int32_t)0xA8B4471A, 
    (int32_t)0x5C13539B, (int32_t)0xA7156F3C, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x58EA90C4, (int32_t)0xA3ECAC65, (int32_t)0x574BB8E6, (int32_t)0xA263007D, 
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x53F9BE05, (int32_t)0x9F65AD2D, (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0x508D9211, (int32_t)0x9C867B2C, 
    (int32_t)0x4ECDFEC7, (int32_t)0x9B2276B0, (int32_t)0x4D084651, (int32_t)0x99C64FC5, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x496AF3E2, (int32_t)0x9726069C, 
    (int32_t)0x4793A210, (int32_t)0x95E218C9, (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0x43D464FB, (int32_t)0x937328F5, (int32_t)0x41ECC484, (int32_t)0x92485786, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x3E0E3DDC, (int32_t)0x900C7621, (int32_t)0x3C17A4E8, (int32_t)0x8EFB92C2, (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, 
    (int32_t)0x381C8BB5, (int32_t)0x8CF45113, (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0x32037A45, (int32_t)0x8A2CE59F, 
    (int32_t)0x2FF31BDE, (int32_t)0x89520A1A, (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x29AC37A0, (int32_t)0x86F93F50, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x256C6F9F, (int32_t)0x8597CE46, (int32_t)0x234815BA, (int32_t)0x84F56073, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, 
    (int32_t)0x1EF74BF3, (int32_t)0x83CD5982, (int32_t)0x1CCB3237, (int32_t)0x8347D77B, (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x186C6DDD, (int32_t)0x825A0A5B, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x11D06C97, (int32_t)0x813EE55B, (int32_t)0x0F996A26, (int32_t)0x80F43F69, 
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0x08EDC7B7, (int32_t)0x804FD23A, (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, 
    (int32_t)0x04779632, (int32_t)0x8013F61D, (int32_t)0x023BE165, (int32_t)0x8004FDA0, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xFDC41E9B, (int32_t)0x8004FDA0, 
    (int32_t)0xFB8869CE, (int32_t)0x8013F61D, (int32_t)0xF94D0E2E, (int32_t)0x802CE84C, (int32_t)0xF7123849, (int32_t)0x804FD23A, (int32_t)0xF4D814A4, (int32_t)0x807CB130, 
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0xF06695DA, (int32_t)0x80F43F69, (int32_t)0xEE2F9369, (int32_t)0x813EE55B, (int32_t)0xEBF9F498, (int32_t)0x81936DAF, 
    (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, (int32_t)0xE7939223, (int32_t)0x825A0A5B, (int32_t)0xE5632654, (int32_t)0x82CC0F36, (int32_t)0xE334CDC9, (int32_t)0x8347D77B, 
    (int32_t)0xE108B40D, (int32_t)0x83CD5982, (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0xDCB7EA46, (int32_t)0x84F56073, (int32_t)0xDA939061, (int32_t)0x8597CE46, 
    (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0xD653C860, (int32_t)0x86F93F50, (int32_t)0xD438AF17, (int32_t)0x87B826F7, (int32_t)0xD220FFC0, (int32_t)0x88806FC4, 
    (int32_t)0xD00CE422, (int32_t)0x89520A1A, (int32_t)0xCDFC85BB, (int32_t)0x8A2CE59F, (int32_t)0xCBF00DBE, (int32_t)0x8B10F144, (int32_t)0xC9E7A512, (int32_t)0x8BFE1B3F, 
    (int32_t)0xC7E3744B, (int32_t)0x8CF45113, (int32_t)0xC5E3A3A9, (int32_t)0x8DF37F8B, (int32_t)0xC3E85B18, (int32_t)0x8EFB92C2, (int32_t)0xC1F1C224, (int32_t)0x900C7621, 
    (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0xBE133B7C, (int32_t)0x92485786, (int32_t)0xBC2B9B05, (int32_t)0x937328F5, (int32_t)0xBA4944A2, (int32_t)0x94A6715D, 
    (int32_t)0xB86C5DF0, (int32_t)0x95E218C9, (int32_t)0xB6950C1E, (int32_t)0x9726069C, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0xB2F7B9AF, (int32_t)0x99C64FC5, 
    (int32_t)0xB1320139, (int32_t)0x9B2276B0, (int32_t)0xAF726DEF, (int32_t)0x9C867B2C, (int32_t)0xADB922B7, (int32_t)0x9DF24175, (int32_t)0xAC0641FB, (int32_t)0x9F65AD2D, 
    (int32_t)0xAA59EDA4, (int32_t)0xA0E0A15F, (int32_t)0xA8B4471A, (int32_t)0xA263007D, (int32_t)0xA7156F3C, (int32_t)0xA3ECAC65, (int32_t)0xA57D8666, (int32_t)0xA57D8666, 
    (int32_t)0xA3ECAC65, (int32_t)0xA7156F3C, (int32_t)0xA263007D, (int32_t)0xA8B4471A, (int32_t)0xA0E0A15F, (int32_t)0xAA59EDA4, (int32_t)0x9F65AD2D, (int32_t)0xAC0641FB, 
    (int32_t)0x9DF24175, (int32_t)0xADB922B7, (int32_t)0x9C867B2C, (int32_t)0xAF726DEF, (int32_t)0x9B2276B0, (int32_t)0xB1320139, (int32_t)0x99C64FC5, (int32_t)0xB2F7B9AF, 
    (int32_t)0x98722192, (int32_t)0xB4C373EE, (int32_t)0x9726069C, (int32_t)0xB6950C1E, (int32_t)0x95E218C9, (int32_t)0xB86C5DF0, (int32_t)0x94A6715D, (int32_t)0xBA4944A2, 
    (int32_t)0x937328F5, (int32_t)0xBC2B9B05, (int32_t)0x92485786, (int32_t)0xBE133B7C, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x900C7621, (int32_t)0xC1F1C224, 
    (int32_t)0x8EFB92C2, (int32_t)0xC3E85B18, (int32_t)0x8DF37F8B, (int32_t)0xC5E3A3A9, (int32_t)0x8CF45113, (int32_t)0xC7E3744B, (int32_t)0x8BFE1B3F, (int32_t)0xC9E7A512, 
    (int32_t)0x8B10F144, (int32_t)0xCBF00DBE, (int32_t)0x8A2CE59F, (int32_t)0xCDFC85BB, (int32_t)0x89520A1A, (int32_t)0xD00CE422, (int32_t)0x88806FC4, (int32_t)0xD220FFC0, 
    (int32_t)0x87B826F7, (int32_t)0xD438AF17, (int32_t)0x86F93F50, (int32_t)0xD653C860, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x8597CE46, (int32_t)0xDA939061, 
    (int32_t)0x84F56073, (int32_t)0xDCB7EA46, (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0x83CD5982, (int32_t)0xE108B40D, (int32_t)0x8347D77B, (int32_t)0xE334CDC9, 
    (int32_t)0x82CC0F36, (int32_t)0xE5632654, (int32_t)0x825A0A5B, (int32_t)0xE7939223, (int32_t)0x81F1D1CE, (int32_t)0xE9C5E582, (int32_t)0x81936DAF, (int32_t)0xEBF9F498, 
    (int32_t)0x813EE55B, (int32_t)0xEE2F9369, (int32_t)0x80F43F69, (int32_t)0xF06695DA, (int32_t)0x80B381AC, (int32_t)0xF29ECFB2, (int32_t)0x807CB130, (int32_t)0xF4D814A4, 
    (int32_t)0x804FD23A, (int32_t)0xF7123849, (int32_t)0x802CE84C, (int32_t)0xF94D0E2E, (int32_t)0x8013F61D, (int32_t)0xFB8869CE, (int32_t)0x8004FDA0, (int32_t)0xFDC41E9B, 
};

/****************** stage 2 radix 4 ******************/
ALIGN(8) static const int32_t __fft360_tw2[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7FEC09E3, (int32_t)0xFB8869CE, (int32_t)0x7FB02DC6, (int32_t)0xF7123849, (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, 
    (int32_t)0x7FB02DC6, (int32_t)0xF7123849, (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369, (int32_t)0x7D33F0CA, (int32_t)0xE5632654, 
    (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x79BC384D, (int32_t)0xD8722192, 
    (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369, (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46, (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, 
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, 
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, 
    (int32_t)0x7C32A67E, (int32_t)0xE108B40D, (int32_t)0x71046D3E, (int32_t)0xC3E85B18, (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, 
    (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46, (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, 
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x4B3C8C12, (int32_t)0x98722192, 
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x40000000, (int32_t)0x9126145F, 
    (int32_t)0x76ADF5E6, (int32_t)0xD00CE422, (int32_t)0x5C13539B, (int32_t)0xA7156F3C, (int32_t)0x340FF242, (int32_t)0x8B10F144, 
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, 
    (int32_t)0x730BAEED, (int32_t)0xC7E3744B, (int32_t)0x4ECDFEC7, (int32_t)0x9B2276B0, (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, 
    (int32_t)0x71046D3E, (int32_t)0xC3E85B18, (int32_t)0x4793A210, (int32_t)0x95E218C9, (int32_t)0x0D61304E, (int32_t)0x80B381AC, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, 
    (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05, (int32_t)0x381C8BB5, (int32_t)0x8CF45113, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, 
    (int32_t)0x6A1DE737, (int32_t)0xB86C5DF0, (int32_t)0x2FF31BDE, (int32_t)0x89520A1A, (int32_t)0xE5632654, (int32_t)0x82CC0F36, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x8643C7B3, 
    (int32_t)0x64DD8950, (int32_t)0xB1320139, (int32_t)0x1EF74BF3, (int32_t)0x83CD5982, (int32_t)0xCBF00DBE, (int32_t)0x8B10F144, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0xC0000000, (int32_t)0x9126145F, 
    (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0xB4C373EE, (int32_t)0x98722192, 
    (int32_t)0x5C13539B, (int32_t)0xA7156F3C, (int32_t)0x04779632, (int32_t)0x8013F61D, (int32_t)0xAA59EDA4, (int32_t)0xA0E0A15F, 
    (int32_t)0x58EA90C4, (int32_t)0xA3ECAC65, (int32_t)0xFB8869CE, (int32_t)0x8013F61D, (int32_t)0xA0E0A15F, (int32_t)0xAA59EDA4, 
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x98722192, (int32_t)0xB4C373EE, 
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, (int32_t)0x9126145F, (int32_t)0xC0000000, 
    (int32_t)0x4ECDFEC7, (int32_t)0x9B2276B0, (int32_t)0xE108B40D, (int32_t)0x83CD5982, (int32_t)0x8B10F144, (int32_t)0xCBF00DBE, 
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x8643C7B3, (int32_t)0xD8722192, 
    (int32_t)0x4793A210, (int32_t)0x95E218C9, (int32_t)0xD00CE422, (int32_t)0x89520A1A, (int32_t)0x82CC0F36, (int32_t)0xE5632654, 
    (int32_t)0x43D464FB, (int32_t)0x937328F5, (int32_t)0xC7E3744B, (int32_t)0x8CF45113, (int32_t)0x80B381AC, (int32_t)0xF29ECFB2, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, 
    (int32_t)0x3C17A4E8, (int32_t)0x8EFB92C2, (int32_t)0xB86C5DF0, (int32_t)0x95E218C9, (int32_t)0x80B381AC, (int32_t)0x0D61304E, 
    (int32_t)0x381C8BB5, (int32_t)0x8CF45113, (int32_t)0xB1320139, (int32_t)0x9B2276B0, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC, 
    (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0xAA59EDA4, (int32_t)0xA0E0A15F, (int32_t)0x8643C7B3, (int32_t)0x278DDE6E, 
    (int32_t)0x2FF31BDE, (int32_t)0x89520A1A, (int32_t)0xA3ECAC65, (int32_t)0xA7156F3C, (int32_t)0x8B10F144, (int32_t)0x340FF242, 
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x9DF24175, (int32_t)0xADB922B7, (int32_t)0x9126145F, (int32_t)0x40000000, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x4B3C8C12, 
    (int32_t)0x234815BA, (int32_t)0x84F56073, (int32_t)0x937328F5, (int32_t)0xBC2B9B05, (int32_t)0xA0E0A15F, (int32_t)0x55A6125C, 
    (int32_t)0x1EF74BF3, (int32_t)0x83CD5982, (int32_t)0x8EFB92C2, (int32_t)0xC3E85B18, (int32_t)0xAA59EDA4, (int32_t)0x5F1F5EA1, 
    (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x8B10F144, (int32_t)0xCBF00DBE, (int32_t)0xB4C373EE, (int32_t)0x678DDE6E, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, (int32_t)0xC0000000, (int32_t)0x6ED9EBA1, 
    (int32_t)0x11D06C97, (int32_t)0x813EE55B, (int32_t)0x84F56073, (int32_t)0xDCB7EA46, (int32_t)0xCBF00DBE, (int32_t)0x74EF0EBC, 
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0xE5632654, (int32_t)0xD8722192, (int32_t)0x79BC384D, 
    (int32_t)0x08EDC7B7, (int32_t)0x804FD23A, (int32_t)0x813EE55B, (int32_t)0xEE2F9369, (int32_t)0xE5632654, (int32_t)0x7D33F0CA, 
    (int32_t)0x04779632, (int32_t)0x8013F61D, (int32_t)0x804FD23A, (int32_t)0xF7123849, (int32_t)0xF29ECFB2, (int32_t)0x7F4C7E54, 
};

/****************** stage 3 radix 3 ******************/
ALIGN(8) static const int32_t __fft360_tw3[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369, (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46, 
    (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46, (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05, 
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, 
    (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05, (int32_t)0x381C8BB5, (int32_t)0x8CF45113, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, 
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, 
    (int32_t)0x4793A210, (int32_t)0x95E218C9, (int32_t)0xD00CE422, (int32_t)0x89520A1A, 
    (int32_t)0x381C8BB5, (int32_t)0x8CF45113, (int32_t)0xB1320139, (int32_t)0x9B2276B0, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, 
    (int32_t)0x04779632, (int32_t)0x8013F61D, (int32_t)0x804FD23A, (int32_t)0xF7123849, 
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC, 
    (int32_t)0xE108B40D, (int32_t)0x83CD5982, (int32_t)0x8EFB92C2, (int32_t)0x3C17A4E8, 
    (int32_t)0xD00CE422, (int32_t)0x89520A1A, (int32_t)0xA3ECAC65, (int32_t)0x58EA90C4, 
};

/****************** stage 4 radix 3 ******************/
ALIGN(8) static const int32_t __fft360_tw4[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, 
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, 
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC, 
};
#define N 360
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT2_first_32x32,
    fft_stageS2_DFT4x2_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT2_first_32x32,
    fft_stageS3_DFT4x2_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT2_first_32x32,
    fft_stageS2_DFT4x2_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT2_first_32x32,
    fft_stageS3_DFT4x2_32x32,
    fft_stageS3_DFT3_32x32,
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
    __fft360_tw1, __fft360_tw2, __fft360_tw3, __fft360_tw4, NULL
};
 const fft_cplx32x32_descr_t __cfft_descr360_32x32 =
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
};     
 const fft_cplx32x32_descr_t __cifft_descr360_32x32 =
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_360 = (const fft_handle_t)&__cfft_descr360_32x32;
const fft_handle_t cinfft32_360 = (const fft_handle_t)&__cifft_descr360_32x32;
