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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFB026, (int32_t)0xFF7105E9, (int32_t)0x7FFEC096, (int32_t)0xFEE20C84, (int32_t)0x7FFD3154, (int32_t)0xFE531484,
    (int32_t)0x7FFB0260, (int32_t)0xFDC41E9B, (int32_t)0x7FF833BD, (int32_t)0xFD352B7B, (int32_t)0x7FF4C56F, (int32_t)0xFCA63BD8, (int32_t)0x7FF0B77A, (int32_t)0xFC175062,
    (int32_t)0x7FEC09E3, (int32_t)0xFB8869CE, (int32_t)0x7FE6BCB0, (int32_t)0xFAF988CC, (int32_t)0x7FE0CFE7, (int32_t)0xFA6AAE0F, (int32_t)0x7FDA4391, (int32_t)0xF9DBDA4A,
    (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, (int32_t)0x7FCB4C5B, (int32_t)0xF8BE4A6E, (int32_t)0x7FC2E18E, (int32_t)0xF82F8FBC, (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9,
    (int32_t)0x7FB02DC6, (int32_t)0xF7123849, (int32_t)0x7FA5E4E1, (int32_t)0xF6839CED, (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67, (int32_t)0x7F8F7559, (int32_t)0xF5668A68,
    (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7F76892F, (int32_t)0xF449ACCA, (int32_t)0x7F692483, (int32_t)0xF3BB538E, (int32_t)0x7F5B20DF, (int32_t)0xF32D09A0,
    (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7F3D3CF4, (int32_t)0xF210A676, (int32_t)0x7F2D5CD1, (int32_t)0xF1828E9E, (int32_t)0x7F1CDE01, (int32_t)0xF0F488D9,
    (int32_t)0x7F0BC097, (int32_t)0xF06695DA, (int32_t)0x7EFA04A8, (int32_t)0xEFD8B651, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7ED4B198, (int32_t)0xEEBD3468,
    (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369, (int32_t)0x7EACE58A, (int32_t)0xEDA208A5, (int32_t)0x7E981262, (int32_t)0xED1494CB, (int32_t)0x7E82A146, (int32_t)0xEC87388C,
    (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x7E55E59E, (int32_t)0xEB6CC9A1, (int32_t)0x7E3E9B4A, (int32_t)0xEADFB855, (int32_t)0x7E26B371, (int32_t)0xEA52C166,
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7DF50BAB, (int32_t)0xE939255A, (int32_t)0x7DDB4BFC, (int32_t)0xE8AC819D, (int32_t)0x7DC0EF44, (int32_t)0xE81FFAFB,
    (int32_t)0x7DA5F5A5, (int32_t)0xE7939223, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7D6E2C37, (int32_t)0xE67B1C8D, (int32_t)0x7D515CAF, (int32_t)0xE5EF112D,
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x7D15E8AD, (int32_t)0xE4D75CAF, (int32_t)0x7CF7447F, (int32_t)0xE44BB4EC, (int32_t)0x7CD80464, (int32_t)0xE3C02FBB,
    (int32_t)0x7CB82885, (int32_t)0xE334CDC9, (int32_t)0x7C97B109, (int32_t)0xE2A98FC4, (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x7C54EFDC, (int32_t)0xE1938239,
    (int32_t)0x7C32A67E, (int32_t)0xE108B40D, (int32_t)0x7C0FC22A, (int32_t)0xE07E0C84, (int32_t)0x7BEC430B, (int32_t)0xDFF38C4C, (int32_t)0x7BC8294D, (int32_t)0xDF69340F,
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7B7E26AA, (int32_t)0xDE54FE40, (int32_t)0x7B583E21, (int32_t)0xDDCB2205, (int32_t)0x7B31BBB2, (int32_t)0xDD417079,
    (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46, (int32_t)0x7AE2E9E4, (int32_t)0xDC2E9018, (int32_t)0x7ABA9AE6, (int32_t)0xDBA5629B, (int32_t)0x7A91B2C7, (int32_t)0xDB1C627B,
    (int32_t)0x7A6831BA, (int32_t)0xDA939061, (int32_t)0x7A3E17F2, (int32_t)0xDA0AECF9, (int32_t)0x7A1365A5, (int32_t)0xD98278ED, (int32_t)0x79E81B06, (int32_t)0xD8FA34E7,
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x798FBDB0, (int32_t)0xD7EA3F96, (int32_t)0x7962AB67, (int32_t)0xD7628F9F, (int32_t)0x793501A9, (int32_t)0xD6DB1254,
    (int32_t)0x7906C0B0, (int32_t)0xD653C860, (int32_t)0x78D7E8B6, (int32_t)0xD5CCB26A, (int32_t)0x78A879F4, (int32_t)0xD545D11C, (int32_t)0x787874A7, (int32_t)0xD4BF251E,
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x7816A759, (int32_t)0xD3B26FB0, (int32_t)0x77E4DFD2, (int32_t)0xD32C678F, (int32_t)0x77B282B3, (int32_t)0xD2A6975D,
    (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x774C08AB, (int32_t)0xD19BA15F, (int32_t)0x7717EC41, (int32_t)0xD1167CE1, (int32_t)0x76E33B3F, (int32_t)0xD09192EA,
    (int32_t)0x76ADF5E6, (int32_t)0xD00CE422, (int32_t)0x76781C7A, (int32_t)0xCF88712E, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x760AAE73, (int32_t)0xCE804155,
    (int32_t)0x75D31A61, (int32_t)0xCDFC85BB, (int32_t)0x759AF34C, (int32_t)0xCD790887, (int32_t)0x7562397A, (int32_t)0xCCF5CA5F, (int32_t)0x7528ED32, (int32_t)0xCC72CBE5,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x74B49E5F, (int32_t)0xCB6D908C, (int32_t)0x74799C66, (int32_t)0xCAEB54F2, (int32_t)0x743E0918, (int32_t)0xCA695B94,
    (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x73C52FAB, (int32_t)0xC966320E, (int32_t)0x7387EA23, (int32_t)0xC8E5032B, (int32_t)0x734A1475, (int32_t)0xC864190A,
    (int32_t)0x730BAEED, (int32_t)0xC7E3744B, (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x728D358C, (int32_t)0xC6E2FD75, (int32_t)0x724D224F, (int32_t)0xC6632C9E,
    (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x71CB504E, (int32_t)0xC5646336, (int32_t)0x7189922C, (int32_t)0xC4E56BE4, (int32_t)0x71474660, (int32_t)0xC466BE4F,
    (int32_t)0x71046D3E, (int32_t)0xC3E85B18, (int32_t)0x70C10718, (int32_t)0xC36A42DA, (int32_t)0x707D1443, (int32_t)0xC2EC7635, (int32_t)0x70389514, (int32_t)0xC26EF5C4,
    (int32_t)0x6FF389DF, (int32_t)0xC1F1C224, (int32_t)0x6FADF2FC, (int32_t)0xC174DBF2, (int32_t)0x6F67D0C1, (int32_t)0xC0F843C9, (int32_t)0x6F212385, (int32_t)0xC07BFA44,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6E92296E, (int32_t)0xBF845596, (int32_t)0x6E49DD45, (int32_t)0xBF08FBA1, (int32_t)0x6E010780, (int32_t)0xBE8DF2BA,
    (int32_t)0x6DB7A87A, (int32_t)0xBE133B7C, (int32_t)0x6D6DC08F, (int32_t)0xBD98D67F, (int32_t)0x6D23501B, (int32_t)0xBD1EC45C, (int32_t)0x6CD8577A, (int32_t)0xBCA505AC,
    (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05, (int32_t)0x6C40CF2C, (int32_t)0xBBB28501, (int32_t)0x6BF4403B, (int32_t)0xBB39C435, (int32_t)0x6BA72A98, (int32_t)0xBAC15939,
    (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x6B0B6CBD, (int32_t)0xB9D18707, (int32_t)0x6ABCC547, (int32_t)0xB95A20FD, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319,
    (int32_t)0x6A1DE737, (int32_t)0xB86C5DF0, (int32_t)0x69CDB162, (int32_t)0xB7F60215, (int32_t)0x697CF78A, (int32_t)0xB780001C, (int32_t)0x692BBA14, (int32_t)0xB70A5899,
    (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x6887B5E2, (int32_t)0xB6201B3E, (int32_t)0x6834EFF3, (int32_t)0xB5AB868A, (int32_t)0x67E1A7FF, (int32_t)0xB5374E95,
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x673993A9, (int32_t)0xB44FF727, (int32_t)0x66E4C818, (int32_t)0xB3DCD8D0, (int32_t)0x668F7C25, (int32_t)0xB36A1978,
    (int32_t)0x6639B03B, (int32_t)0xB2F7B9AF, (int32_t)0x65E364C4, (int32_t)0xB285BA02, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x653550E2, (int32_t)0xB1A2DD3A,
    (int32_t)0x64DD8950, (int32_t)0xB1320139, (int32_t)0x648543E4, (int32_t)0xB0C1878B, (int32_t)0x642C810C, (int32_t)0xB05170BD, (int32_t)0x63D34137, (int32_t)0xAFE1BD5B,
    (int32_t)0x637984D4, (int32_t)0xAF726DEF, (int32_t)0x631F4C54, (int32_t)0xAF038305, (int32_t)0x62C49827, (int32_t)0xAE94FD27, (int32_t)0x626968BE, (int32_t)0xAE26DCDF,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x61B19A00, (int32_t)0xAD4BCF37, (int32_t)0x6154FB91, (int32_t)0xACDEE2E8, (int32_t)0x60F7E3B0, (int32_t)0xAC725E52,
    (int32_t)0x609A52D3, (int32_t)0xAC0641FB, (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x5FDDC7F3, (int32_t)0xAB2F442A, (int32_t)0x5F7ECEDD, (int32_t)0xAAC463BB,
    (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x5EBF77B5, (int32_t)0xA9EFE26C, (int32_t)0x5E5F1A91, (int32_t)0xA9864294, (int32_t)0x5DFE47AD, (int32_t)0xA91D0EA3,
    (int32_t)0x5D9CFF83, (int32_t)0xA8B4471A, (int32_t)0x5D3B428C, (int32_t)0xA84BEC7C, (int32_t)0x5CD91140, (int32_t)0xA7E3FF4D, (int32_t)0x5C766C1C, (int32_t)0xA77C800C,
    (int32_t)0x5C13539B, (int32_t)0xA7156F3C, (int32_t)0x5BAFC837, (int32_t)0xA6AECD5E, (int32_t)0x5B4BCA6C, (int32_t)0xA6489AF0, (int32_t)0x5AE75AB9, (int32_t)0xA5E2D873,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x5A1D278D, (int32_t)0xA518A547, (int32_t)0x59B76510, (int32_t)0xA4B43594, (int32_t)0x595132A2, (int32_t)0xA45037C9,
    (int32_t)0x58EA90C4, (int32_t)0xA3ECAC65, (int32_t)0x58837FF4, (int32_t)0xA38993E4, (int32_t)0x581C00B3, (int32_t)0xA326EEC0, (int32_t)0x57B41384, (int32_t)0xA2C4BD74,
    (int32_t)0x574BB8E6, (int32_t)0xA263007D, (int32_t)0x56E2F15D, (int32_t)0xA201B853, (int32_t)0x5679BD6C, (int32_t)0xA1A0E56F, (int32_t)0x56101D94, (int32_t)0xA140884B,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x553B9C45, (int32_t)0xA0813123, (int32_t)0x54D0BBD6, (int32_t)0xA022380D, (int32_t)0x54657194, (int32_t)0x9FC3B694,
    (int32_t)0x53F9BE05, (int32_t)0x9F65AD2D, (int32_t)0x538DA1AE, (int32_t)0x9F081C50, (int32_t)0x53211D18, (int32_t)0x9EAB046F, (int32_t)0x52B430C9, (int32_t)0x9E4E6600,
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0x51D92321, (int32_t)0x9D969742, (int32_t)0x516B02D9, (int32_t)0x9D3B67D9, (int32_t)0x50FC7CFB, (int32_t)0x9CE0B3AC,
    (int32_t)0x508D9211, (int32_t)0x9C867B2C, (int32_t)0x501E42A5, (int32_t)0x9C2CBEC9, (int32_t)0x4FAE8F43, (int32_t)0x9BD37EF4, (int32_t)0x4F3E7875, (int32_t)0x9B7ABC1C,
    (int32_t)0x4ECDFEC7, (int32_t)0x9B2276B0, (int32_t)0x4E5D22C6, (int32_t)0x9ACAAF1E, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x4D7A45FE, (int32_t)0x9A1C9B3C,
    (int32_t)0x4D084651, (int32_t)0x99C64FC5, (int32_t)0x4C95E688, (int32_t)0x997083DB, (int32_t)0x4C232730, (int32_t)0x991B37E8, (int32_t)0x4BB008D9, (int32_t)0x98C66C57,
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x4AC8B16B, (int32_t)0x981E5801, (int32_t)0x4A547976, (int32_t)0x97CB100D, (int32_t)0x49DFE4C2, (int32_t)0x97784A1E,
    (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0x48F5A767, (int32_t)0x96D445EC, (int32_t)0x487FFFE4, (int32_t)0x96830876, (int32_t)0x4809FDEB, (int32_t)0x96324E9E,
    (int32_t)0x4793A210, (int32_t)0x95E218C9, (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x46A5DF03, (int32_t)0x95433AB9, (int32_t)0x462E78F9, (int32_t)0x94F49343,
    (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0x453EA6C7, (int32_t)0x9458D568, (int32_t)0x44C63BCB, (int32_t)0x940BBFC5, (int32_t)0x444D7AFF, (int32_t)0x93BF30D4,
    (int32_t)0x43D464FB, (int32_t)0x937328F5, (int32_t)0x435AFA54, (int32_t)0x9327A886, (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5, (int32_t)0x42672981, (int32_t)0x92923F71,
    (int32_t)0x41ECC484, (int32_t)0x92485786, (int32_t)0x41720D46, (int32_t)0x91FEF880, (int32_t)0x40F7045F, (int32_t)0x91B622BB, (int32_t)0x407BAA6A, (int32_t)0x916DD692,
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x3F8405BC, (int32_t)0x90DEDC7B, (int32_t)0x3F07BC37, (int32_t)0x90982F3F, (int32_t)0x3E8B240E, (int32_t)0x90520D04,
    (int32_t)0x3E0E3DDC, (int32_t)0x900C7621, (int32_t)0x3D910A3C, (int32_t)0x8FC76AEC, (int32_t)0x3D1389CB, (int32_t)0x8F82EBBD, (int32_t)0x3C95BD26, (int32_t)0x8F3EF8E8,
    (int32_t)0x3C17A4E8, (int32_t)0x8EFB92C2, (int32_t)0x3B9941B1, (int32_t)0x8EB8B9A0, (int32_t)0x3B1A941C, (int32_t)0x8E766DD4, (int32_t)0x3A9B9CCA, (int32_t)0x8E34AFB2,
    (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0x399CD362, (int32_t)0x8DB2DDB1, (int32_t)0x391D028B, (int32_t)0x8D72CA74, (int32_t)0x389CEA72, (int32_t)0x8D334625,
    (int32_t)0x381C8BB5, (int32_t)0x8CF45113, (int32_t)0x379BE6F6, (int32_t)0x8CB5EB8B, (int32_t)0x371AFCD5, (int32_t)0x8C7815DD, (int32_t)0x3699CDF2, (int32_t)0x8C3AD055,
    (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0x3596A46C, (int32_t)0x8BC1F6E8, (int32_t)0x3514AB0E, (int32_t)0x8B86639A, (int32_t)0x34926F74, (int32_t)0x8B4B61A1,
    (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0x338D341B, (int32_t)0x8AD712CE, (int32_t)0x330A35A1, (int32_t)0x8A9DC686, (int32_t)0x3286F779, (int32_t)0x8A650CB4,
    (int32_t)0x32037A45, (int32_t)0x8A2CE59F, (int32_t)0x317FBEAB, (int32_t)0x89F5518D, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x30778ED2, (int32_t)0x8987E386,
    (int32_t)0x2FF31BDE, (int32_t)0x89520A1A, (int32_t)0x2F6E6D16, (int32_t)0x891CC4C1, (int32_t)0x2EE9831F, (int32_t)0x88E813BF, (int32_t)0x2E645EA1, (int32_t)0x88B3F755,
    (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0x2D5968A3, (int32_t)0x884D7D4D, (int32_t)0x2CD39871, (int32_t)0x881B202E, (int32_t)0x2C4D9050, (int32_t)0x87E958A7,
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x2B40DAE2, (int32_t)0x87878B59, (int32_t)0x2ABA2EE4, (int32_t)0x8757860C, (int32_t)0x2A334D96, (int32_t)0x8728174A,
    (int32_t)0x29AC37A0, (int32_t)0x86F93F50, (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x289D7061, (int32_t)0x869D5499, (int32_t)0x2815C06A, (int32_t)0x86704250,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x2705CB19, (int32_t)0x8617E4FA, (int32_t)0x267D8713, (int32_t)0x85EC9A5B, (int32_t)0x25F51307, (int32_t)0x85C1E80E,
    (int32_t)0x256C6F9F, (int32_t)0x8597CE46, (int32_t)0x24E39D85, (int32_t)0x856E4D39, (int32_t)0x245A9D65, (int32_t)0x8545651A, (int32_t)0x23D16FE8, (int32_t)0x851D161C,
    (int32_t)0x234815BA, (int32_t)0x84F56073, (int32_t)0x22BE8F87, (int32_t)0x84CE444E, (int32_t)0x2234DDFB, (int32_t)0x84A7C1DF, (int32_t)0x21AB01C0, (int32_t)0x8481D956,
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x2096CBF1, (int32_t)0x8437D6B3, (int32_t)0x200C73B4, (int32_t)0x8413BCF5, (int32_t)0x1F81F37C, (int32_t)0x83F03DD6,
    (int32_t)0x1EF74BF3, (int32_t)0x83CD5982, (int32_t)0x1E6C7DC7, (int32_t)0x83AB1024, (int32_t)0x1DE189A6, (int32_t)0x838961E8, (int32_t)0x1D56703C, (int32_t)0x83684EF7,
    (int32_t)0x1CCB3237, (int32_t)0x8347D77B, (int32_t)0x1C3FD045, (int32_t)0x8327FB9C, (int32_t)0x1BB44B14, (int32_t)0x8308BB81, (int32_t)0x1B28A351, (int32_t)0x82EA1753,
    (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x1A10EED3, (int32_t)0x82AEA351, (int32_t)0x1984E373, (int32_t)0x8291D3C9, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0,
    (int32_t)0x186C6DDD, (int32_t)0x825A0A5B, (int32_t)0x17E00505, (int32_t)0x823F10BC, (int32_t)0x17537E63, (int32_t)0x8224B404, (int32_t)0x16C6DAA6, (int32_t)0x820AF455,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x15AD3E9A, (int32_t)0x81D94C8F, (int32_t)0x152047AB, (int32_t)0x81C164B6, (int32_t)0x1493365F, (int32_t)0x81AA1A62,
    (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x1378C774, (int32_t)0x817D5EBA, (int32_t)0x12EB6B35, (int32_t)0x8167ED9E, (int32_t)0x125DF75B, (int32_t)0x81531A76,
    (int32_t)0x11D06C97, (int32_t)0x813EE55B, (int32_t)0x1142CB98, (int32_t)0x812B4E68, (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x102749AF, (int32_t)0x8105FB58,
    (int32_t)0x0F996A26, (int32_t)0x80F43F69, (int32_t)0x0F0B7727, (int32_t)0x80E321FF, (int32_t)0x0E7D7162, (int32_t)0x80D2A32F, (int32_t)0x0DEF598A, (int32_t)0x80C2C30C,
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x0CD2F660, (int32_t)0x80A4DF21, (int32_t)0x0C44AC72, (int32_t)0x8096DB7D, (int32_t)0x0BB65336, (int32_t)0x808976D1,
    (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0x0A997598, (int32_t)0x80708AA7, (int32_t)0x0A0AF299, (int32_t)0x80650347, (int32_t)0x097C6313, (int32_t)0x805A1B1F,
    (int32_t)0x08EDC7B7, (int32_t)0x804FD23A, (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x07D07044, (int32_t)0x803D1E72, (int32_t)0x0741B592, (int32_t)0x8034B3A5,
    (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, (int32_t)0x062425B6, (int32_t)0x8025BC6F, (int32_t)0x059551F1, (int32_t)0x801F3019, (int32_t)0x05067734, (int32_t)0x80194350,
    (int32_t)0x04779632, (int32_t)0x8013F61D, (int32_t)0x03E8AF9E, (int32_t)0x800F4886, (int32_t)0x0359C428, (int32_t)0x800B3A91, (int32_t)0x02CAD485, (int32_t)0x8007CC43,
    (int32_t)0x023BE165, (int32_t)0x8004FDA0, (int32_t)0x01ACEB7C, (int32_t)0x8002CEAC, (int32_t)0x011DF37C, (int32_t)0x80013F6A, (int32_t)0x008EFA17, (int32_t)0x80004FDA,
};

/****************** stage 1 radix 4 ******************/
ALIGN(8) static const int32_t __fft720_tw1[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFEC096, (int32_t)0xFEE20C84,
    (int32_t)0x7FFB0260, (int32_t)0xFDC41E9B, (int32_t)0x7FF4C56F, (int32_t)0xFCA63BD8, (int32_t)0x7FFB0260, (int32_t)0xFDC41E9B, (int32_t)0x7FEC09E3, (int32_t)0xFB8869CE,
    (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, (int32_t)0x7FF4C56F, (int32_t)0xFCA63BD8, (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67,
    (int32_t)0x7FEC09E3, (int32_t)0xFB8869CE, (int32_t)0x7FB02DC6, (int32_t)0xF7123849, (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7FE0CFE7, (int32_t)0xFA6AAE0F,
    (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2,
    (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x7FC2E18E, (int32_t)0xF82F8FBC, (int32_t)0x7F0BC097, (int32_t)0xF06695DA, (int32_t)0x7DDB4BFC, (int32_t)0xE8AC819D,
    (int32_t)0x7FB02DC6, (int32_t)0xF7123849, (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369, (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67,
    (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582,
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7F692483, (int32_t)0xF3BB538E, (int32_t)0x7DA5F5A5, (int32_t)0xE7939223, (int32_t)0x7ABA9AE6, (int32_t)0xDBA5629B,
    (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x7F2D5CD1, (int32_t)0xF1828E9E,
    (int32_t)0x7CB82885, (int32_t)0xE334CDC9, (int32_t)0x78A879F4, (int32_t)0xD545D11C, (int32_t)0x7F0BC097, (int32_t)0xF06695DA, (int32_t)0x7C32A67E, (int32_t)0xE108B40D,
    (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3,
    (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369, (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46, (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x7E981262, (int32_t)0xED1494CB,
    (int32_t)0x7A6831BA, (int32_t)0xDA939061, (int32_t)0x7387EA23, (int32_t)0xC8E5032B, (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x79BC384D, (int32_t)0xD8722192,
    (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x7E3E9B4A, (int32_t)0xEADFB855, (int32_t)0x7906C0B0, (int32_t)0xD653C860, (int32_t)0x707D1443, (int32_t)0xC2EC7635,
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x7DDB4BFC, (int32_t)0xE8AC819D,
    (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x6D23501B, (int32_t)0xBD1EC45C, (int32_t)0x7DA5F5A5, (int32_t)0xE7939223, (int32_t)0x76ADF5E6, (int32_t)0xD00CE422,
    (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x7D6E2C37, (int32_t)0xE67B1C8D, (int32_t)0x75D31A61, (int32_t)0xCDFC85BB, (int32_t)0x697CF78A, (int32_t)0xB780001C,
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x7CF7447F, (int32_t)0xE44BB4EC,
    (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x7CB82885, (int32_t)0xE334CDC9, (int32_t)0x730BAEED, (int32_t)0xC7E3744B,
    (int32_t)0x637984D4, (int32_t)0xAF726DEF, (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x6154FB91, (int32_t)0xACDEE2E8,
    (int32_t)0x7C32A67E, (int32_t)0xE108B40D, (int32_t)0x71046D3E, (int32_t)0xC3E85B18, (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x7BEC430B, (int32_t)0xDFF38C4C,
    (int32_t)0x6FF389DF, (int32_t)0xC1F1C224, (int32_t)0x5CD91140, (int32_t)0xA7E3FF4D, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x7B583E21, (int32_t)0xDDCB2205, (int32_t)0x6DB7A87A, (int32_t)0xBE133B7C, (int32_t)0x581C00B3, (int32_t)0xA326EEC0,
    (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46, (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x7ABA9AE6, (int32_t)0xDBA5629B,
    (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x53211D18, (int32_t)0x9EAB046F, (int32_t)0x7A6831BA, (int32_t)0xDA939061, (int32_t)0x6A1DE737, (int32_t)0xB86C5DF0,
    (int32_t)0x508D9211, (int32_t)0x9C867B2C, (int32_t)0x7A1365A5, (int32_t)0xD98278ED, (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3,
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x7962AB67, (int32_t)0xD7628F9F,
    (int32_t)0x6639B03B, (int32_t)0xB2F7B9AF, (int32_t)0x487FFFE4, (int32_t)0x96830876, (int32_t)0x7906C0B0, (int32_t)0xD653C860, (int32_t)0x64DD8950, (int32_t)0xB1320139,
    (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0x78A879F4, (int32_t)0xD545D11C, (int32_t)0x637984D4, (int32_t)0xAF726DEF, (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5,
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x77E4DFD2, (int32_t)0xD32C678F,
    (int32_t)0x609A52D3, (int32_t)0xAC0641FB, (int32_t)0x3D1389CB, (int32_t)0x8F82EBBD, (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4,
    (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0x7717EC41, (int32_t)0xD1167CE1, (int32_t)0x5D9CFF83, (int32_t)0xA8B4471A, (int32_t)0x371AFCD5, (int32_t)0x8C7815DD,
    (int32_t)0x76ADF5E6, (int32_t)0xD00CE422, (int32_t)0x5C13539B, (int32_t)0xA7156F3C, (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x75D31A61, (int32_t)0xCDFC85BB, (int32_t)0x58EA90C4, (int32_t)0xA3ECAC65,
    (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0x7562397A, (int32_t)0xCCF5CA5F, (int32_t)0x574BB8E6, (int32_t)0xA263007D, (int32_t)0x2ABA2EE4, (int32_t)0x8757860C,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x74799C66, (int32_t)0xCAEB54F2,
    (int32_t)0x53F9BE05, (int32_t)0x9F65AD2D, (int32_t)0x245A9D65, (int32_t)0x8545651A, (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x5246DD49, (int32_t)0x9DF24175,
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x7387EA23, (int32_t)0xC8E5032B, (int32_t)0x508D9211, (int32_t)0x9C867B2C, (int32_t)0x1DE189A6, (int32_t)0x838961E8,
    (int32_t)0x730BAEED, (int32_t)0xC7E3744B, (int32_t)0x4ECDFEC7, (int32_t)0x9B2276B0, (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x728D358C, (int32_t)0xC6E2FD75,
    (int32_t)0x4D084651, (int32_t)0x99C64FC5, (int32_t)0x17537E63, (int32_t)0x8224B404, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x4B3C8C12, (int32_t)0x98722192,
    (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x7189922C, (int32_t)0xC4E56BE4, (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0x10B5150F, (int32_t)0x811855B4,
    (int32_t)0x71046D3E, (int32_t)0xC3E85B18, (int32_t)0x4793A210, (int32_t)0x95E218C9, (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x707D1443, (int32_t)0xC2EC7635,
    (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0x0A0AF299, (int32_t)0x80650347, (int32_t)0x6FF389DF, (int32_t)0xC1F1C224, (int32_t)0x43D464FB, (int32_t)0x937328F5,
    (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, (int32_t)0x6F67D0C1, (int32_t)0xC0F843C9, (int32_t)0x41ECC484, (int32_t)0x92485786, (int32_t)0x0359C428, (int32_t)0x800B3A91,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x6E49DD45, (int32_t)0xBF08FBA1,
    (int32_t)0x3E0E3DDC, (int32_t)0x900C7621, (int32_t)0xFCA63BD8, (int32_t)0x800B3A91, (int32_t)0x6DB7A87A, (int32_t)0xBE133B7C, (int32_t)0x3C17A4E8, (int32_t)0x8EFB92C2,
    (int32_t)0xF94D0E2E, (int32_t)0x802CE84C, (int32_t)0x6D23501B, (int32_t)0xBD1EC45C, (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0xF5F50D67, (int32_t)0x80650347,
    (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05, (int32_t)0x381C8BB5, (int32_t)0x8CF45113, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x6BF4403B, (int32_t)0xBB39C435,
    (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0xEF4AEAF1, (int32_t)0x811855B4, (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x340FF242, (int32_t)0x8B10F144,
    (int32_t)0xEBF9F498, (int32_t)0x81936DAF, (int32_t)0x6ABCC547, (int32_t)0xB95A20FD, (int32_t)0x32037A45, (int32_t)0x8A2CE59F, (int32_t)0xE8AC819D, (int32_t)0x8224B404,
    (int32_t)0x6A1DE737, (int32_t)0xB86C5DF0, (int32_t)0x2FF31BDE, (int32_t)0x89520A1A, (int32_t)0xE5632654, (int32_t)0x82CC0F36, (int32_t)0x697CF78A, (int32_t)0xB780001C,
    (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0xE21E765A, (int32_t)0x838961E8, (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x2BC750E9, (int32_t)0x87B826F7,
    (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x6834EFF3, (int32_t)0xB5AB868A, (int32_t)0x29AC37A0, (int32_t)0x86F93F50, (int32_t)0xDBA5629B, (int32_t)0x8545651A,
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x66E4C818, (int32_t)0xB3DCD8D0,
    (int32_t)0x256C6F9F, (int32_t)0x8597CE46, (int32_t)0xD545D11C, (int32_t)0x8757860C, (int32_t)0x6639B03B, (int32_t)0xB2F7B9AF, (int32_t)0x234815BA, (int32_t)0x84F56073,
    (int32_t)0xD220FFC0, (int32_t)0x88806FC4, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3,
    (int32_t)0x64DD8950, (int32_t)0xB1320139, (int32_t)0x1EF74BF3, (int32_t)0x83CD5982, (int32_t)0xCBF00DBE, (int32_t)0x8B10F144, (int32_t)0x642C810C, (int32_t)0xB05170BD,
    (int32_t)0x1CCB3237, (int32_t)0x8347D77B, (int32_t)0xC8E5032B, (int32_t)0x8C7815DD, (int32_t)0x637984D4, (int32_t)0xAF726DEF, (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36,
    (int32_t)0xC5E3A3A9, (int32_t)0x8DF37F8B, (int32_t)0x62C49827, (int32_t)0xAE94FD27, (int32_t)0x186C6DDD, (int32_t)0x825A0A5B, (int32_t)0xC2EC7635, (int32_t)0x8F82EBBD,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x6154FB91, (int32_t)0xACDEE2E8,
    (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0xBD1EC45C, (int32_t)0x92DCAFE5, (int32_t)0x609A52D3, (int32_t)0xAC0641FB, (int32_t)0x11D06C97, (int32_t)0x813EE55B,
    (int32_t)0xBA4944A2, (int32_t)0x94A6715D, (int32_t)0x5FDDC7F3, (int32_t)0xAB2F442A, (int32_t)0x0F996A26, (int32_t)0x80F43F69, (int32_t)0xB780001C, (int32_t)0x96830876,
    (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x5E5F1A91, (int32_t)0xA9864294,
    (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0xB2141B02, (int32_t)0x9A7365D3, (int32_t)0x5D9CFF83, (int32_t)0xA8B4471A, (int32_t)0x08EDC7B7, (int32_t)0x804FD23A,
    (int32_t)0xAF726DEF, (int32_t)0x9C867B2C, (int32_t)0x5CD91140, (int32_t)0xA7E3FF4D, (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, (int32_t)0xACDEE2E8, (int32_t)0x9EAB046F,
    (int32_t)0x5C13539B, (int32_t)0xA7156F3C, (int32_t)0x04779632, (int32_t)0x8013F61D, (int32_t)0xAA59EDA4, (int32_t)0xA0E0A15F, (int32_t)0x5B4BCA6C, (int32_t)0xA6489AF0,
    (int32_t)0x023BE165, (int32_t)0x8004FDA0, (int32_t)0xA7E3FF4D, (int32_t)0xA326EEC0, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000,
    (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x59B76510, (int32_t)0xA4B43594, (int32_t)0xFDC41E9B, (int32_t)0x8004FDA0, (int32_t)0xA326EEC0, (int32_t)0xA7E3FF4D,
    (int32_t)0x58EA90C4, (int32_t)0xA3ECAC65, (int32_t)0xFB8869CE, (int32_t)0x8013F61D, (int32_t)0xA0E0A15F, (int32_t)0xAA59EDA4, (int32_t)0x581C00B3, (int32_t)0xA326EEC0,
    (int32_t)0xF94D0E2E, (int32_t)0x802CE84C, (int32_t)0x9EAB046F, (int32_t)0xACDEE2E8, (int32_t)0x574BB8E6, (int32_t)0xA263007D, (int32_t)0xF7123849, (int32_t)0x804FD23A,
    (int32_t)0x9C867B2C, (int32_t)0xAF726DEF, (int32_t)0x5679BD6C, (int32_t)0xA1A0E56F, (int32_t)0xF4D814A4, (int32_t)0x807CB130, (int32_t)0x9A7365D3, (int32_t)0xB2141B02,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x98722192, (int32_t)0xB4C373EE, (int32_t)0x54D0BBD6, (int32_t)0xA022380D,
    (int32_t)0xF06695DA, (int32_t)0x80F43F69, (int32_t)0x96830876, (int32_t)0xB780001C, (int32_t)0x53F9BE05, (int32_t)0x9F65AD2D, (int32_t)0xEE2F9369, (int32_t)0x813EE55B,
    (int32_t)0x94A6715D, (int32_t)0xBA4944A2, (int32_t)0x53211D18, (int32_t)0x9EAB046F, (int32_t)0xEBF9F498, (int32_t)0x81936DAF, (int32_t)0x92DCAFE5, (int32_t)0xBD1EC45C,
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x516B02D9, (int32_t)0x9D3B67D9,
    (int32_t)0xE7939223, (int32_t)0x825A0A5B, (int32_t)0x8F82EBBD, (int32_t)0xC2EC7635, (int32_t)0x508D9211, (int32_t)0x9C867B2C, (int32_t)0xE5632654, (int32_t)0x82CC0F36,
    (int32_t)0x8DF37F8B, (int32_t)0xC5E3A3A9, (int32_t)0x4FAE8F43, (int32_t)0x9BD37EF4, (int32_t)0xE334CDC9, (int32_t)0x8347D77B, (int32_t)0x8C7815DD, (int32_t)0xC8E5032B,
    (int32_t)0x4ECDFEC7, (int32_t)0x9B2276B0, (int32_t)0xE108B40D, (int32_t)0x83CD5982, (int32_t)0x8B10F144, (int32_t)0xCBF00DBE, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3,
    (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, (int32_t)0x4D084651, (int32_t)0x99C64FC5, (int32_t)0xDCB7EA46, (int32_t)0x84F56073,
    (int32_t)0x88806FC4, (int32_t)0xD220FFC0, (int32_t)0x4C232730, (int32_t)0x991B37E8, (int32_t)0xDA939061, (int32_t)0x8597CE46, (int32_t)0x8757860C, (int32_t)0xD545D11C,
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x4A547976, (int32_t)0x97CB100D,
    (int32_t)0xD653C860, (int32_t)0x86F93F50, (int32_t)0x8545651A, (int32_t)0xDBA5629B, (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0xD438AF17, (int32_t)0x87B826F7,
    (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0x487FFFE4, (int32_t)0x96830876, (int32_t)0xD220FFC0, (int32_t)0x88806FC4, (int32_t)0x838961E8, (int32_t)0xE21E765A,
    (int32_t)0x4793A210, (int32_t)0x95E218C9, (int32_t)0xD00CE422, (int32_t)0x89520A1A, (int32_t)0x82CC0F36, (int32_t)0xE5632654, (int32_t)0x46A5DF03, (int32_t)0x95433AB9,
    (int32_t)0xCDFC85BB, (int32_t)0x8A2CE59F, (int32_t)0x8224B404, (int32_t)0xE8AC819D, (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0xCBF00DBE, (int32_t)0x8B10F144,
    (int32_t)0x81936DAF, (int32_t)0xEBF9F498, (int32_t)0x44C63BCB, (int32_t)0x940BBFC5, (int32_t)0xC9E7A512, (int32_t)0x8BFE1B3F, (int32_t)0x811855B4, (int32_t)0xEF4AEAF1,
    (int32_t)0x43D464FB, (int32_t)0x937328F5, (int32_t)0xC7E3744B, (int32_t)0x8CF45113, (int32_t)0x80B381AC, (int32_t)0xF29ECFB2, (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5,
    (int32_t)0xC5E3A3A9, (int32_t)0x8DF37F8B, (int32_t)0x80650347, (int32_t)0xF5F50D67, (int32_t)0x41ECC484, (int32_t)0x92485786, (int32_t)0xC3E85B18, (int32_t)0x8EFB92C2,
    (int32_t)0x802CE84C, (int32_t)0xF94D0E2E, (int32_t)0x40F7045F, (int32_t)0x91B622BB, (int32_t)0xC1F1C224, (int32_t)0x900C7621, (int32_t)0x800B3A91, (int32_t)0xFCA63BD8,
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, (int32_t)0x3F07BC37, (int32_t)0x90982F3F,
    (int32_t)0xBE133B7C, (int32_t)0x92485786, (int32_t)0x800B3A91, (int32_t)0x0359C428, (int32_t)0x3E0E3DDC, (int32_t)0x900C7621, (int32_t)0xBC2B9B05, (int32_t)0x937328F5,
    (int32_t)0x802CE84C, (int32_t)0x06B2F1D2, (int32_t)0x3D1389CB, (int32_t)0x8F82EBBD, (int32_t)0xBA4944A2, (int32_t)0x94A6715D, (int32_t)0x80650347, (int32_t)0x0A0AF299,
    (int32_t)0x3C17A4E8, (int32_t)0x8EFB92C2, (int32_t)0xB86C5DF0, (int32_t)0x95E218C9, (int32_t)0x80B381AC, (int32_t)0x0D61304E, (int32_t)0x3B1A941C, (int32_t)0x8E766DD4,
    (int32_t)0xB6950C1E, (int32_t)0x9726069C, (int32_t)0x811855B4, (int32_t)0x10B5150F, (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0xB4C373EE, (int32_t)0x98722192,
    (int32_t)0x81936DAF, (int32_t)0x14060B68, (int32_t)0x391D028B, (int32_t)0x8D72CA74, (int32_t)0xB2F7B9AF, (int32_t)0x99C64FC5, (int32_t)0x8224B404, (int32_t)0x17537E63,
    (int32_t)0x381C8BB5, (int32_t)0x8CF45113, (int32_t)0xB1320139, (int32_t)0x9B2276B0, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC, (int32_t)0x371AFCD5, (int32_t)0x8C7815DD,
    (int32_t)0xAF726DEF, (int32_t)0x9C867B2C, (int32_t)0x838961E8, (int32_t)0x1DE189A6, (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0xADB922B7, (int32_t)0x9DF24175,
    (int32_t)0x845C8AE3, (int32_t)0x2120FB83, (int32_t)0x3514AB0E, (int32_t)0x8B86639A, (int32_t)0xAC0641FB, (int32_t)0x9F65AD2D, (int32_t)0x8545651A, (int32_t)0x245A9D65,
    (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0xAA59EDA4, (int32_t)0xA0E0A15F, (int32_t)0x8643C7B3, (int32_t)0x278DDE6E, (int32_t)0x330A35A1, (int32_t)0x8A9DC686,
    (int32_t)0xA8B4471A, (int32_t)0xA263007D, (int32_t)0x8757860C, (int32_t)0x2ABA2EE4, (int32_t)0x32037A45, (int32_t)0x8A2CE59F, (int32_t)0xA7156F3C, (int32_t)0xA3ECAC65,
    (int32_t)0x88806FC4, (int32_t)0x2DDF0040, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x89BE50C3, (int32_t)0x30FBC54D,
    (int32_t)0x2FF31BDE, (int32_t)0x89520A1A, (int32_t)0xA3ECAC65, (int32_t)0xA7156F3C, (int32_t)0x8B10F144, (int32_t)0x340FF242, (int32_t)0x2EE9831F, (int32_t)0x88E813BF,
    (int32_t)0xA263007D, (int32_t)0xA8B4471A, (int32_t)0x8C7815DD, (int32_t)0x371AFCD5, (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0xA0E0A15F, (int32_t)0xAA59EDA4,
    (int32_t)0x8DF37F8B, (int32_t)0x3A1C5C57, (int32_t)0x2CD39871, (int32_t)0x881B202E, (int32_t)0x9F65AD2D, (int32_t)0xAC0641FB, (int32_t)0x8F82EBBD, (int32_t)0x3D1389CB,
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x9DF24175, (int32_t)0xADB922B7, (int32_t)0x9126145F, (int32_t)0x40000000, (int32_t)0x2ABA2EE4, (int32_t)0x8757860C,
    (int32_t)0x9C867B2C, (int32_t)0xAF726DEF, (int32_t)0x92DCAFE5, (int32_t)0x42E13BA4, (int32_t)0x29AC37A0, (int32_t)0x86F93F50, (int32_t)0x9B2276B0, (int32_t)0xB1320139,
    (int32_t)0x94A6715D, (int32_t)0x45B6BB5E, (int32_t)0x289D7061, (int32_t)0x869D5499, (int32_t)0x99C64FC5, (int32_t)0xB2F7B9AF, (int32_t)0x96830876, (int32_t)0x487FFFE4,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x4B3C8C12, (int32_t)0x267D8713, (int32_t)0x85EC9A5B,
    (int32_t)0x9726069C, (int32_t)0xB6950C1E, (int32_t)0x9A7365D3, (int32_t)0x4DEBE4FE, (int32_t)0x256C6F9F, (int32_t)0x8597CE46, (int32_t)0x95E218C9, (int32_t)0xB86C5DF0,
    (int32_t)0x9C867B2C, (int32_t)0x508D9211, (int32_t)0x245A9D65, (int32_t)0x8545651A, (int32_t)0x94A6715D, (int32_t)0xBA4944A2, (int32_t)0x9EAB046F, (int32_t)0x53211D18,
    (int32_t)0x234815BA, (int32_t)0x84F56073, (int32_t)0x937328F5, (int32_t)0xBC2B9B05, (int32_t)0xA0E0A15F, (int32_t)0x55A6125C, (int32_t)0x2234DDFB, (int32_t)0x84A7C1DF,
    (int32_t)0x92485786, (int32_t)0xBE133B7C, (int32_t)0xA326EEC0, (int32_t)0x581C00B3, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000,
    (int32_t)0xA57D8666, (int32_t)0x5A82799A, (int32_t)0x200C73B4, (int32_t)0x8413BCF5, (int32_t)0x900C7621, (int32_t)0xC1F1C224, (int32_t)0xA7E3FF4D, (int32_t)0x5CD91140,
    (int32_t)0x1EF74BF3, (int32_t)0x83CD5982, (int32_t)0x8EFB92C2, (int32_t)0xC3E85B18, (int32_t)0xAA59EDA4, (int32_t)0x5F1F5EA1, (int32_t)0x1DE189A6, (int32_t)0x838961E8,
    (int32_t)0x8DF37F8B, (int32_t)0xC5E3A3A9, (int32_t)0xACDEE2E8, (int32_t)0x6154FB91, (int32_t)0x1CCB3237, (int32_t)0x8347D77B, (int32_t)0x8CF45113, (int32_t)0xC7E3744B,
    (int32_t)0xAF726DEF, (int32_t)0x637984D4, (int32_t)0x1BB44B14, (int32_t)0x8308BB81, (int32_t)0x8BFE1B3F, (int32_t)0xC9E7A512, (int32_t)0xB2141B02, (int32_t)0x658C9A2D,
    (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x8B10F144, (int32_t)0xCBF00DBE, (int32_t)0xB4C373EE, (int32_t)0x678DDE6E, (int32_t)0x1984E373, (int32_t)0x8291D3C9,
    (int32_t)0x8A2CE59F, (int32_t)0xCDFC85BB, (int32_t)0xB780001C, (int32_t)0x697CF78A, (int32_t)0x186C6DDD, (int32_t)0x825A0A5B, (int32_t)0x89520A1A, (int32_t)0xD00CE422,
    (int32_t)0xBA4944A2, (int32_t)0x6B598EA3, (int32_t)0x17537E63, (int32_t)0x8224B404, (int32_t)0x88806FC4, (int32_t)0xD220FFC0, (int32_t)0xBD1EC45C, (int32_t)0x6D23501B,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, (int32_t)0xC0000000, (int32_t)0x6ED9EBA1, (int32_t)0x152047AB, (int32_t)0x81C164B6,
    (int32_t)0x86F93F50, (int32_t)0xD653C860, (int32_t)0xC2EC7635, (int32_t)0x707D1443, (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x8643C7B3, (int32_t)0xD8722192,
    (int32_t)0xC5E3A3A9, (int32_t)0x720C8075, (int32_t)0x12EB6B35, (int32_t)0x8167ED9E, (int32_t)0x8597CE46, (int32_t)0xDA939061, (int32_t)0xC8E5032B, (int32_t)0x7387EA23,
    (int32_t)0x11D06C97, (int32_t)0x813EE55B, (int32_t)0x84F56073, (int32_t)0xDCB7EA46, (int32_t)0xCBF00DBE, (int32_t)0x74EF0EBC, (int32_t)0x10B5150F, (int32_t)0x811855B4,
    (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0xCF043AB3, (int32_t)0x7641AF3D, (int32_t)0x0F996A26, (int32_t)0x80F43F69, (int32_t)0x83CD5982, (int32_t)0xE108B40D,
    (int32_t)0xD220FFC0, (int32_t)0x777F903C, (int32_t)0x0E7D7162, (int32_t)0x80D2A32F, (int32_t)0x8347D77B, (int32_t)0xE334CDC9, (int32_t)0xD545D11C, (int32_t)0x78A879F4,
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0xE5632654, (int32_t)0xD8722192, (int32_t)0x79BC384D, (int32_t)0x0C44AC72, (int32_t)0x8096DB7D,
    (int32_t)0x825A0A5B, (int32_t)0xE7939223, (int32_t)0xDBA5629B, (int32_t)0x7ABA9AE6, (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0x81F1D1CE, (int32_t)0xE9C5E582,
    (int32_t)0xDEDF047D, (int32_t)0x7BA3751D, (int32_t)0x0A0AF299, (int32_t)0x80650347, (int32_t)0x81936DAF, (int32_t)0xEBF9F498, (int32_t)0xE21E765A, (int32_t)0x7C769E18,
    (int32_t)0x08EDC7B7, (int32_t)0x804FD23A, (int32_t)0x813EE55B, (int32_t)0xEE2F9369, (int32_t)0xE5632654, (int32_t)0x7D33F0CA, (int32_t)0x07D07044, (int32_t)0x803D1E72,
    (int32_t)0x80F43F69, (int32_t)0xF06695DA, (int32_t)0xE8AC819D, (int32_t)0x7DDB4BFC, (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, (int32_t)0x80B381AC, (int32_t)0xF29ECFB2,
    (int32_t)0xEBF9F498, (int32_t)0x7E6C9251, (int32_t)0x059551F1, (int32_t)0x801F3019, (int32_t)0x807CB130, (int32_t)0xF4D814A4, (int32_t)0xEF4AEAF1, (int32_t)0x7EE7AA4C,
    (int32_t)0x04779632, (int32_t)0x8013F61D, (int32_t)0x804FD23A, (int32_t)0xF7123849, (int32_t)0xF29ECFB2, (int32_t)0x7F4C7E54, (int32_t)0x0359C428, (int32_t)0x800B3A91,
    (int32_t)0x802CE84C, (int32_t)0xF94D0E2E, (int32_t)0xF5F50D67, (int32_t)0x7F9AFCB9, (int32_t)0x023BE165, (int32_t)0x8004FDA0, (int32_t)0x8013F61D, (int32_t)0xFB8869CE,
    (int32_t)0xF94D0E2E, (int32_t)0x7FD317B4, (int32_t)0x011DF37C, (int32_t)0x80013F6A, (int32_t)0x8004FDA0, (int32_t)0xFDC41E9B, (int32_t)0xFCA63BD8, (int32_t)0x7FF4C56F,
};

/****************** stage 2 radix 4 ******************/
ALIGN(8) static const int32_t __fft720_tw2[] =
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
ALIGN(8) static const int32_t __fft720_tw3[] =
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
ALIGN(8) static const int32_t __fft720_tw4[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE,
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC,
};
#define N 720
static const fft_cplx32x32_stage_t s2_tab[] =
{
    fft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] =
{
    ifft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT4x4_32x32,
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
    __fft720_tw1, __fft720_tw2, __fft720_tw3, __fft720_tw4, NULL
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

const fft_handle_t rnfft32_1440 = (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_1440 = (const fft_handle_t)&__rifft_descr;
