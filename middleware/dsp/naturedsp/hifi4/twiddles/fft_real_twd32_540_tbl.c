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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x7FFDC829, (int32_t)0xFE82BC51,
    (int32_t)0x7FF720B6, (int32_t)0xFD0585D9,
    (int32_t)0x7FEC09E3, (int32_t)0xFB8869CE,
    (int32_t)0x7FDC8412, (int32_t)0xFA0B7565,
    (int32_t)0x7FC88FCE, (int32_t)0xF88EB5D3,
    (int32_t)0x7FB02DC6, (int32_t)0xF7123849,
    (int32_t)0x7F935ED3, (int32_t)0xF59609F9,
    (int32_t)0x7F7223F6, (int32_t)0xF41A380E,
    (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2,
    (int32_t)0x7F226F3C, (int32_t)0xF123DE0D,
    (int32_t)0x7EF3F823, (int32_t)0xEFA97040,
    (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369,
    (int32_t)0x7E89D885, (int32_t)0xECB654A1,
    (int32_t)0x7E4E33AF, (int32_t)0xEB3DC0FA,
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582,
    (int32_t)0x7DC9CA47, (int32_t)0xE84ECF3F,
    (int32_t)0x7D810A4D, (int32_t)0xE6D88B32,
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654,
    (int32_t)0x7CE28069, (int32_t)0xE3EEAD94,
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD,
    (int32_t)0x7C32A67E, (int32_t)0xE108B40D,
    (int32_t)0x7BD4430D, (int32_t)0xDF974CFD,
    (int32_t)0x7B7194EE, (int32_t)0xDE270579,
    (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46,
    (int32_t)0x7A9F667D, (int32_t)0xDB4A081C,
    (int32_t)0x7A2FED73, (int32_t)0xD9DD6BA9,
    (int32_t)0x79BC384D, (int32_t)0xD8722192,
    (int32_t)0x79444B0E, (int32_t)0xD708366C,
    (int32_t)0x78C829DE, (int32_t)0xD59FB6C4,
    (int32_t)0x7847D909, (int32_t)0xD438AF17,
    (int32_t)0x77C35D04, (int32_t)0xD2D32BD7,
    (int32_t)0x773ABA64, (int32_t)0xD16F3969,
    (int32_t)0x76ADF5E6, (int32_t)0xD00CE422,
    (int32_t)0x761D146C, (int32_t)0xCEAC384B,
    (int32_t)0x75881AFB, (int32_t)0xCD4D421B,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE,
    (int32_t)0x7451F4FE, (int32_t)0xCA94A74D,
    (int32_t)0x73B0D331, (int32_t)0xC93B1AD4,
    (int32_t)0x730BAEED, (int32_t)0xC7E3744B,
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B,
    (int32_t)0x71B57605, (int32_t)0xC53A089E,
    (int32_t)0x71046D3E, (int32_t)0xC3E85B18,
    (int32_t)0x704F79B6, (int32_t)0xC298C2BE,
    (int32_t)0x6F96A1B5, (int32_t)0xC14B4B31,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000,
    (int32_t)0x6E195E06, (int32_t)0xBEB6ECA7,
    (int32_t)0x6D54FF8F, (int32_t)0xBD701C8D,
    (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05,
    (int32_t)0x6BC0EB6A, (int32_t)0xBAE97350,
    (int32_t)0x6AF143BD, (int32_t)0xB9A9B097,
    (int32_t)0x6A1DE737, (int32_t)0xB86C5DF0,
    (int32_t)0x6946DD2A, (int32_t)0xB7318659,
    (int32_t)0x686C2D0B, (int32_t)0xB5F934BD,
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE,
    (int32_t)0x66ABF908, (int32_t)0xB3904EA9,
    (int32_t)0x65C684AD, (int32_t)0xB25FCF93,
    (int32_t)0x64DD8950, (int32_t)0xB1320139,
    (int32_t)0x63F10F05, (int32_t)0xB006EE12,
    (int32_t)0x63011DFE, (int32_t)0xAEDEA07A,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7,
    (int32_t)0x6116F91D, (int32_t)0xAC967EF5,
    (int32_t)0x601CD640, (int32_t)0xAB76BF46,
    (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4,
    (int32_t)0x5E1E9B07, (int32_t)0xA94013EE,
    (int32_t)0x5D1A9459, (int32_t)0xA8293BE8,
    (int32_t)0x5C13539B, (int32_t)0xA7156F3C,
    (int32_t)0x5B08E1EB, (int32_t)0xA604B77A,
    (int32_t)0x59FB4886, (int32_t)0xA4F71E15,
    (int32_t)0x58EA90C4, (int32_t)0xA3ECAC65,
    (int32_t)0x57D6C418, (int32_t)0xA2E56BA7,
    (int32_t)0x56BFEC12, (int32_t)0xA1E164F9,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F,
    (int32_t)0x548940BA, (int32_t)0x9FE329C0,
    (int32_t)0x5369810B, (int32_t)0x9EE906E3,
    (int32_t)0x5246DD49, (int32_t)0x9DF24175,
    (int32_t)0x51215F86, (int32_t)0x9CFEE202,
    (int32_t)0x4FF911EE, (int32_t)0x9C0EF0FB,
    (int32_t)0x4ECDFEC7, (int32_t)0x9B2276B0,
    (int32_t)0x4DA0306D, (int32_t)0x9A397B53,
    (int32_t)0x4C6FB157, (int32_t)0x995406F8,
    (int32_t)0x4B3C8C12, (int32_t)0x98722192,
    (int32_t)0x4A06CB43, (int32_t)0x9793D2F5,
    (int32_t)0x48CE79A7, (int32_t)0x96B922D6,
    (int32_t)0x4793A210, (int32_t)0x95E218C9,
    (int32_t)0x46564F69, (int32_t)0x950EBC43,
    (int32_t)0x45168CB0, (int32_t)0x943F1496,
    (int32_t)0x43D464FB, (int32_t)0x937328F5,
    (int32_t)0x428FE373, (int32_t)0x92AB0071,
    (int32_t)0x41491359, (int32_t)0x91E6A1FA,
    (int32_t)0x40000000, (int32_t)0x9126145F,
    (int32_t)0x3EB4B4CF, (int32_t)0x90695E4B,
    (int32_t)0x3D673D42, (int32_t)0x8FB0864A,
    (int32_t)0x3C17A4E8, (int32_t)0x8EFB92C2,
    (int32_t)0x3AC5F762, (int32_t)0x8E4A89FB,
    (int32_t)0x39724065, (int32_t)0x8D9D7216,
    (int32_t)0x381C8BB5, (int32_t)0x8CF45113,
    (int32_t)0x36C4E52C, (int32_t)0x8C4F2CCF,
    (int32_t)0x356B58B3, (int32_t)0x8BAE0B02,
    (int32_t)0x340FF242, (int32_t)0x8B10F144,
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505,
    (int32_t)0x3153C7B5, (int32_t)0x89E2EB94,
    (int32_t)0x2FF31BDE, (int32_t)0x89520A1A,
    (int32_t)0x2E90C697, (int32_t)0x88C5459C,
    (int32_t)0x2D2CD429, (int32_t)0x883CA2FC,
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7,
    (int32_t)0x2A60493C, (int32_t)0x8737D622,
    (int32_t)0x28F7C994, (int32_t)0x86BBB4F2,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3,
    (int32_t)0x26229457, (int32_t)0x85D0128D,
    (int32_t)0x24B5F7E4, (int32_t)0x85609983,
    (int32_t)0x234815BA, (int32_t)0x84F56073,
    (int32_t)0x21D8FA87, (int32_t)0x848E6B12,
    (int32_t)0x2068B303, (int32_t)0x842BBCF3,
    (int32_t)0x1EF74BF3, (int32_t)0x83CD5982,
    (int32_t)0x1D84D223, (int32_t)0x83734404,
    (int32_t)0x1C11526C, (int32_t)0x831D7F97,
    (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36,
    (int32_t)0x192774CE, (int32_t)0x827EF5B3,
    (int32_t)0x17B130C1, (int32_t)0x823635B9,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x14C23F06, (int32_t)0x81B1CC51,
    (int32_t)0x1349AB5F, (int32_t)0x8176277B,
    (int32_t)0x11D06C97, (int32_t)0x813EE55B,
    (int32_t)0x10568FC0, (int32_t)0x810C07DD,
    (int32_t)0x0EDC21F3, (int32_t)0x80DD90C4,
    (int32_t)0x0D61304E, (int32_t)0x80B381AC,
    (int32_t)0x0BE5C7F2, (int32_t)0x808DDC0A,
    (int32_t)0x0A69F607, (int32_t)0x806CA12D,
    (int32_t)0x08EDC7B7, (int32_t)0x804FD23A,
    (int32_t)0x07714A2D, (int32_t)0x80377032,
    (int32_t)0x05F48A9B, (int32_t)0x80237BEE,
    (int32_t)0x04779632, (int32_t)0x8013F61D,
    (int32_t)0x02FA7A27, (int32_t)0x8008DF4A,
    (int32_t)0x017D43AF, (int32_t)0x800237D7,
};

/****************** stage 1 radix 2 ******************/
ALIGN(8) static const int32_t __fft270_tw1[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x7FF720B6, (int32_t)0xFD0585D9,
    (int32_t)0x7FDC8412, (int32_t)0xFA0B7565,
    (int32_t)0x7FB02DC6, (int32_t)0xF7123849,
    (int32_t)0x7F7223F6, (int32_t)0xF41A380E,
    (int32_t)0x7F226F3C, (int32_t)0xF123DE0D,
    (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369,
    (int32_t)0x7E4E33AF, (int32_t)0xEB3DC0FA,
    (int32_t)0x7DC9CA47, (int32_t)0xE84ECF3F,
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654,
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD,
    (int32_t)0x7BD4430D, (int32_t)0xDF974CFD,
    (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46,
    (int32_t)0x7A2FED73, (int32_t)0xD9DD6BA9,
    (int32_t)0x79444B0E, (int32_t)0xD708366C,
    (int32_t)0x7847D909, (int32_t)0xD438AF17,
    (int32_t)0x773ABA64, (int32_t)0xD16F3969,
    (int32_t)0x761D146C, (int32_t)0xCEAC384B,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE,
    (int32_t)0x73B0D331, (int32_t)0xC93B1AD4,
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B,
    (int32_t)0x71046D3E, (int32_t)0xC3E85B18,
    (int32_t)0x6F96A1B5, (int32_t)0xC14B4B31,
    (int32_t)0x6E195E06, (int32_t)0xBEB6ECA7,
    (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05,
    (int32_t)0x6AF143BD, (int32_t)0xB9A9B097,
    (int32_t)0x6946DD2A, (int32_t)0xB7318659,
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE,
    (int32_t)0x65C684AD, (int32_t)0xB25FCF93,
    (int32_t)0x63F10F05, (int32_t)0xB006EE12,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7,
    (int32_t)0x601CD640, (int32_t)0xAB76BF46,
    (int32_t)0x5E1E9B07, (int32_t)0xA94013EE,
    (int32_t)0x5C13539B, (int32_t)0xA7156F3C,
    (int32_t)0x59FB4886, (int32_t)0xA4F71E15,
    (int32_t)0x57D6C418, (int32_t)0xA2E56BA7,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F,
    (int32_t)0x5369810B, (int32_t)0x9EE906E3,
    (int32_t)0x51215F86, (int32_t)0x9CFEE202,
    (int32_t)0x4ECDFEC7, (int32_t)0x9B2276B0,
    (int32_t)0x4C6FB157, (int32_t)0x995406F8,
    (int32_t)0x4A06CB43, (int32_t)0x9793D2F5,
    (int32_t)0x4793A210, (int32_t)0x95E218C9,
    (int32_t)0x45168CB0, (int32_t)0x943F1496,
    (int32_t)0x428FE373, (int32_t)0x92AB0071,
    (int32_t)0x40000000, (int32_t)0x9126145F,
    (int32_t)0x3D673D42, (int32_t)0x8FB0864A,
    (int32_t)0x3AC5F762, (int32_t)0x8E4A89FB,
    (int32_t)0x381C8BB5, (int32_t)0x8CF45113,
    (int32_t)0x356B58B3, (int32_t)0x8BAE0B02,
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505,
    (int32_t)0x2FF31BDE, (int32_t)0x89520A1A,
    (int32_t)0x2D2CD429, (int32_t)0x883CA2FC,
    (int32_t)0x2A60493C, (int32_t)0x8737D622,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3,
    (int32_t)0x24B5F7E4, (int32_t)0x85609983,
    (int32_t)0x21D8FA87, (int32_t)0x848E6B12,
    (int32_t)0x1EF74BF3, (int32_t)0x83CD5982,
    (int32_t)0x1C11526C, (int32_t)0x831D7F97,
    (int32_t)0x192774CE, (int32_t)0x827EF5B3,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x1349AB5F, (int32_t)0x8176277B,
    (int32_t)0x10568FC0, (int32_t)0x810C07DD,
    (int32_t)0x0D61304E, (int32_t)0x80B381AC,
    (int32_t)0x0A69F607, (int32_t)0x806CA12D,
    (int32_t)0x07714A2D, (int32_t)0x80377032,
    (int32_t)0x04779632, (int32_t)0x8013F61D,
    (int32_t)0x017D43AF, (int32_t)0x800237D7,
    (int32_t)0xFE82BC51, (int32_t)0x800237D7,
    (int32_t)0xFB8869CE, (int32_t)0x8013F61D,
    (int32_t)0xF88EB5D3, (int32_t)0x80377032,
    (int32_t)0xF59609F9, (int32_t)0x806CA12D,
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC,
    (int32_t)0xEFA97040, (int32_t)0x810C07DD,
    (int32_t)0xECB654A1, (int32_t)0x8176277B,
    (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE,
    (int32_t)0xE6D88B32, (int32_t)0x827EF5B3,
    (int32_t)0xE3EEAD94, (int32_t)0x831D7F97,
    (int32_t)0xE108B40D, (int32_t)0x83CD5982,
    (int32_t)0xDE270579, (int32_t)0x848E6B12,
    (int32_t)0xDB4A081C, (int32_t)0x85609983,
    (int32_t)0xD8722192, (int32_t)0x8643C7B3,
    (int32_t)0xD59FB6C4, (int32_t)0x8737D622,
    (int32_t)0xD2D32BD7, (int32_t)0x883CA2FC,
    (int32_t)0xD00CE422, (int32_t)0x89520A1A,
    (int32_t)0xCD4D421B, (int32_t)0x8A77E505,
    (int32_t)0xCA94A74D, (int32_t)0x8BAE0B02,
    (int32_t)0xC7E3744B, (int32_t)0x8CF45113,
    (int32_t)0xC53A089E, (int32_t)0x8E4A89FB,
    (int32_t)0xC298C2BE, (int32_t)0x8FB0864A,
    (int32_t)0xC0000000, (int32_t)0x9126145F,
    (int32_t)0xBD701C8D, (int32_t)0x92AB0071,
    (int32_t)0xBAE97350, (int32_t)0x943F1496,
    (int32_t)0xB86C5DF0, (int32_t)0x95E218C9,
    (int32_t)0xB5F934BD, (int32_t)0x9793D2F5,
    (int32_t)0xB3904EA9, (int32_t)0x995406F8,
    (int32_t)0xB1320139, (int32_t)0x9B2276B0,
    (int32_t)0xAEDEA07A, (int32_t)0x9CFEE202,
    (int32_t)0xAC967EF5, (int32_t)0x9EE906E3,
    (int32_t)0xAA59EDA4, (int32_t)0xA0E0A15F,
    (int32_t)0xA8293BE8, (int32_t)0xA2E56BA7,
    (int32_t)0xA604B77A, (int32_t)0xA4F71E15,
    (int32_t)0xA3ECAC65, (int32_t)0xA7156F3C,
    (int32_t)0xA1E164F9, (int32_t)0xA94013EE,
    (int32_t)0x9FE329C0, (int32_t)0xAB76BF46,
    (int32_t)0x9DF24175, (int32_t)0xADB922B7,
    (int32_t)0x9C0EF0FB, (int32_t)0xB006EE12,
    (int32_t)0x9A397B53, (int32_t)0xB25FCF93,
    (int32_t)0x98722192, (int32_t)0xB4C373EE,
    (int32_t)0x96B922D6, (int32_t)0xB7318659,
    (int32_t)0x950EBC43, (int32_t)0xB9A9B097,
    (int32_t)0x937328F5, (int32_t)0xBC2B9B05,
    (int32_t)0x91E6A1FA, (int32_t)0xBEB6ECA7,
    (int32_t)0x90695E4B, (int32_t)0xC14B4B31,
    (int32_t)0x8EFB92C2, (int32_t)0xC3E85B18,
    (int32_t)0x8D9D7216, (int32_t)0xC68DBF9B,
    (int32_t)0x8C4F2CCF, (int32_t)0xC93B1AD4,
    (int32_t)0x8B10F144, (int32_t)0xCBF00DBE,
    (int32_t)0x89E2EB94, (int32_t)0xCEAC384B,
    (int32_t)0x88C5459C, (int32_t)0xD16F3969,
    (int32_t)0x87B826F7, (int32_t)0xD438AF17,
    (int32_t)0x86BBB4F2, (int32_t)0xD708366C,
    (int32_t)0x85D0128D, (int32_t)0xD9DD6BA9,
    (int32_t)0x84F56073, (int32_t)0xDCB7EA46,
    (int32_t)0x842BBCF3, (int32_t)0xDF974CFD,
    (int32_t)0x83734404, (int32_t)0xE27B2DDD,
    (int32_t)0x82CC0F36, (int32_t)0xE5632654,
    (int32_t)0x823635B9, (int32_t)0xE84ECF3F,
    (int32_t)0x81B1CC51, (int32_t)0xEB3DC0FA,
    (int32_t)0x813EE55B, (int32_t)0xEE2F9369,
    (int32_t)0x80DD90C4, (int32_t)0xF123DE0D,
    (int32_t)0x808DDC0A, (int32_t)0xF41A380E,
    (int32_t)0x804FD23A, (int32_t)0xF7123849,
    (int32_t)0x80237BEE, (int32_t)0xFA0B7565,
    (int32_t)0x8008DF4A, (int32_t)0xFD0585D9,
};

/****************** stage 2 radix 3 ******************/
ALIGN(8) static const int32_t __fft270_tw2[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x7FDC8412, (int32_t)0xFA0B7565, (int32_t)0x7F7223F6, (int32_t)0xF41A380E,
    (int32_t)0x7F7223F6, (int32_t)0xF41A380E, (int32_t)0x7DC9CA47, (int32_t)0xE84ECF3F,
    (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369, (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46,
    (int32_t)0x7DC9CA47, (int32_t)0xE84ECF3F, (int32_t)0x773ABA64, (int32_t)0xD16F3969,
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD, (int32_t)0x72628DEA, (int32_t)0xC68DBF9B,
    (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46, (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05,
    (int32_t)0x79444B0E, (int32_t)0xD708366C, (int32_t)0x65C684AD, (int32_t)0xB25FCF93,
    (int32_t)0x773ABA64, (int32_t)0xD16F3969, (int32_t)0x5E1E9B07, (int32_t)0xA94013EE,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F,
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B, (int32_t)0x4C6FB157, (int32_t)0x995406F8,
    (int32_t)0x6F96A1B5, (int32_t)0xC14B4B31, (int32_t)0x428FE373, (int32_t)0x92AB0071,
    (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05, (int32_t)0x381C8BB5, (int32_t)0x8CF45113,
    (int32_t)0x6946DD2A, (int32_t)0xB7318659, (int32_t)0x2D2CD429, (int32_t)0x883CA2FC,
    (int32_t)0x65C684AD, (int32_t)0xB25FCF93, (int32_t)0x21D8FA87, (int32_t)0x848E6B12,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x5E1E9B07, (int32_t)0xA94013EE, (int32_t)0x0A69F607, (int32_t)0x806CA12D,
    (int32_t)0x59FB4886, (int32_t)0xA4F71E15, (int32_t)0xFE82BC51, (int32_t)0x800237D7,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC,
    (int32_t)0x51215F86, (int32_t)0x9CFEE202, (int32_t)0xE6D88B32, (int32_t)0x827EF5B3,
    (int32_t)0x4C6FB157, (int32_t)0x995406F8, (int32_t)0xDB4A081C, (int32_t)0x85609983,
    (int32_t)0x4793A210, (int32_t)0x95E218C9, (int32_t)0xD00CE422, (int32_t)0x89520A1A,
    (int32_t)0x428FE373, (int32_t)0x92AB0071, (int32_t)0xC53A089E, (int32_t)0x8E4A89FB,
    (int32_t)0x3D673D42, (int32_t)0x8FB0864A, (int32_t)0xBAE97350, (int32_t)0x943F1496,
    (int32_t)0x381C8BB5, (int32_t)0x8CF45113, (int32_t)0xB1320139, (int32_t)0x9B2276B0,
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505, (int32_t)0xA8293BE8, (int32_t)0xA2E56BA7,
    (int32_t)0x2D2CD429, (int32_t)0x883CA2FC, (int32_t)0x9FE329C0, (int32_t)0xAB76BF46,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE,
    (int32_t)0x21D8FA87, (int32_t)0x848E6B12, (int32_t)0x91E6A1FA, (int32_t)0xBEB6ECA7,
    (int32_t)0x1C11526C, (int32_t)0x831D7F97, (int32_t)0x8C4F2CCF, (int32_t)0xC93B1AD4,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17,
    (int32_t)0x10568FC0, (int32_t)0x810C07DD, (int32_t)0x842BBCF3, (int32_t)0xDF974CFD,
    (int32_t)0x0A69F607, (int32_t)0x806CA12D, (int32_t)0x81B1CC51, (int32_t)0xEB3DC0FA,
    (int32_t)0x04779632, (int32_t)0x8013F61D, (int32_t)0x804FD23A, (int32_t)0xF7123849,
    (int32_t)0xFE82BC51, (int32_t)0x800237D7, (int32_t)0x8008DF4A, (int32_t)0x02FA7A27,
    (int32_t)0xF88EB5D3, (int32_t)0x80377032, (int32_t)0x80DD90C4, (int32_t)0x0EDC21F3,
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC,
    (int32_t)0xECB654A1, (int32_t)0x8176277B, (int32_t)0x85D0128D, (int32_t)0x26229457,
    (int32_t)0xE6D88B32, (int32_t)0x827EF5B3, (int32_t)0x89E2EB94, (int32_t)0x3153C7B5,
    (int32_t)0xE108B40D, (int32_t)0x83CD5982, (int32_t)0x8EFB92C2, (int32_t)0x3C17A4E8,
    (int32_t)0xDB4A081C, (int32_t)0x85609983, (int32_t)0x950EBC43, (int32_t)0x46564F69,
    (int32_t)0xD59FB6C4, (int32_t)0x8737D622, (int32_t)0x9C0EF0FB, (int32_t)0x4FF911EE,
    (int32_t)0xD00CE422, (int32_t)0x89520A1A, (int32_t)0xA3ECAC65, (int32_t)0x58EA90C4,
    (int32_t)0xCA94A74D, (int32_t)0x8BAE0B02, (int32_t)0xAC967EF5, (int32_t)0x6116F91D,
    (int32_t)0xC53A089E, (int32_t)0x8E4A89FB, (int32_t)0xB5F934BD, (int32_t)0x686C2D0B,
};

/****************** stage 3 radix 3 ******************/
ALIGN(8) static const int32_t __fft270_tw3[] =
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
ALIGN(8) static const int32_t __fft270_tw4[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE,
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC,
};
#define N 270

static const fft_cplx32x32_stage_t s2_tab[] =
{
    fft_stageS2_DFT2_first_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT2_first_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] =
{
    ifft_stageS2_DFT2_first_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT2_first_32x32,
    fft_stageS3_DFT3_32x32,
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
    __fft270_tw1, __fft270_tw2, __fft270_tw3, __fft270_tw4, NULL
};
static const fft_cplx32x32_descr_t __cfft_descr =
{
    N,
    s2_tab,
    s3_tab,
    tw_step_tab,
    tw_tab
};
static const fft_cplx32x32_descr_t __cifft_descr =
{
    N,
    is2_tab,
    is3_tab,
    tw_step_tab,
    tw_tab
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr,
    __fft_real_tw
};

const fft_handle_t  rnfft32_540 = (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_540 = (const fft_handle_t)&__rifft_descr;
