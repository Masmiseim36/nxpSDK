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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFEC096, (int32_t)0xFEE20C84, (int32_t)0x7FFB0260, (int32_t)0xFDC41E9B, (int32_t)0x7FF4C56F, (int32_t)0xFCA63BD8,
        (int32_t)0x7FEC09E3, (int32_t)0xFB8869CE, (int32_t)0x7FE0CFE7, (int32_t)0xFA6AAE0F, (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, (int32_t)0x7FC2E18E, (int32_t)0xF82F8FBC,
        (int32_t)0x7FB02DC6, (int32_t)0xF7123849, (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67, (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7F692483, (int32_t)0xF3BB538E,
        (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7F2D5CD1, (int32_t)0xF1828E9E, (int32_t)0x7F0BC097, (int32_t)0xF06695DA, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1,
        (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369, (int32_t)0x7E981262, (int32_t)0xED1494CB, (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x7E3E9B4A, (int32_t)0xEADFB855,
        (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7DDB4BFC, (int32_t)0xE8AC819D, (int32_t)0x7DA5F5A5, (int32_t)0xE7939223, (int32_t)0x7D6E2C37, (int32_t)0xE67B1C8D,
        (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x7CF7447F, (int32_t)0xE44BB4EC, (int32_t)0x7CB82885, (int32_t)0xE334CDC9, (int32_t)0x7C769E18, (int32_t)0xE21E765A,
        (int32_t)0x7C32A67E, (int32_t)0xE108B40D, (int32_t)0x7BEC430B, (int32_t)0xDFF38C4C, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7B583E21, (int32_t)0xDDCB2205,
        (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46, (int32_t)0x7ABA9AE6, (int32_t)0xDBA5629B, (int32_t)0x7A6831BA, (int32_t)0xDA939061, (int32_t)0x7A1365A5, (int32_t)0xD98278ED,
        (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x7962AB67, (int32_t)0xD7628F9F, (int32_t)0x7906C0B0, (int32_t)0xD653C860, (int32_t)0x78A879F4, (int32_t)0xD545D11C,
        (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x77E4DFD2, (int32_t)0xD32C678F, (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x7717EC41, (int32_t)0xD1167CE1,
        (int32_t)0x76ADF5E6, (int32_t)0xD00CE422, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x75D31A61, (int32_t)0xCDFC85BB, (int32_t)0x7562397A, (int32_t)0xCCF5CA5F,
        (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x74799C66, (int32_t)0xCAEB54F2, (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x7387EA23, (int32_t)0xC8E5032B,
        (int32_t)0x730BAEED, (int32_t)0xC7E3744B, (int32_t)0x728D358C, (int32_t)0xC6E2FD75, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x7189922C, (int32_t)0xC4E56BE4,
        (int32_t)0x71046D3E, (int32_t)0xC3E85B18, (int32_t)0x707D1443, (int32_t)0xC2EC7635, (int32_t)0x6FF389DF, (int32_t)0xC1F1C224, (int32_t)0x6F67D0C1, (int32_t)0xC0F843C9,
        (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6E49DD45, (int32_t)0xBF08FBA1, (int32_t)0x6DB7A87A, (int32_t)0xBE133B7C, (int32_t)0x6D23501B, (int32_t)0xBD1EC45C,
        (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05, (int32_t)0x6BF4403B, (int32_t)0xBB39C435, (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x6ABCC547, (int32_t)0xB95A20FD,
        (int32_t)0x6A1DE737, (int32_t)0xB86C5DF0, (int32_t)0x697CF78A, (int32_t)0xB780001C, (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x6834EFF3, (int32_t)0xB5AB868A,
        (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x66E4C818, (int32_t)0xB3DCD8D0, (int32_t)0x6639B03B, (int32_t)0xB2F7B9AF, (int32_t)0x658C9A2D, (int32_t)0xB2141B02,
        (int32_t)0x64DD8950, (int32_t)0xB1320139, (int32_t)0x642C810C, (int32_t)0xB05170BD, (int32_t)0x637984D4, (int32_t)0xAF726DEF, (int32_t)0x62C49827, (int32_t)0xAE94FD27,
        (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x6154FB91, (int32_t)0xACDEE2E8, (int32_t)0x609A52D3, (int32_t)0xAC0641FB, (int32_t)0x5FDDC7F3, (int32_t)0xAB2F442A,
        (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x5E5F1A91, (int32_t)0xA9864294, (int32_t)0x5D9CFF83, (int32_t)0xA8B4471A, (int32_t)0x5CD91140, (int32_t)0xA7E3FF4D,
        (int32_t)0x5C13539B, (int32_t)0xA7156F3C, (int32_t)0x5B4BCA6C, (int32_t)0xA6489AF0, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x59B76510, (int32_t)0xA4B43594,
        (int32_t)0x58EA90C4, (int32_t)0xA3ECAC65, (int32_t)0x581C00B3, (int32_t)0xA326EEC0, (int32_t)0x574BB8E6, (int32_t)0xA263007D, (int32_t)0x5679BD6C, (int32_t)0xA1A0E56F,
        (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x54D0BBD6, (int32_t)0xA022380D, (int32_t)0x53F9BE05, (int32_t)0x9F65AD2D, (int32_t)0x53211D18, (int32_t)0x9EAB046F,
        (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0x516B02D9, (int32_t)0x9D3B67D9, (int32_t)0x508D9211, (int32_t)0x9C867B2C, (int32_t)0x4FAE8F43, (int32_t)0x9BD37EF4,
        (int32_t)0x4ECDFEC7, (int32_t)0x9B2276B0, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x4D084651, (int32_t)0x99C64FC5, (int32_t)0x4C232730, (int32_t)0x991B37E8,
        (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x4A547976, (int32_t)0x97CB100D, (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0x487FFFE4, (int32_t)0x96830876,
        (int32_t)0x4793A210, (int32_t)0x95E218C9, (int32_t)0x46A5DF03, (int32_t)0x95433AB9, (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0x44C63BCB, (int32_t)0x940BBFC5,
        (int32_t)0x43D464FB, (int32_t)0x937328F5, (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5, (int32_t)0x41ECC484, (int32_t)0x92485786, (int32_t)0x40F7045F, (int32_t)0x91B622BB,
        (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x3F07BC37, (int32_t)0x90982F3F, (int32_t)0x3E0E3DDC, (int32_t)0x900C7621, (int32_t)0x3D1389CB, (int32_t)0x8F82EBBD,
        (int32_t)0x3C17A4E8, (int32_t)0x8EFB92C2, (int32_t)0x3B1A941C, (int32_t)0x8E766DD4, (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0x391D028B, (int32_t)0x8D72CA74,
        (int32_t)0x381C8BB5, (int32_t)0x8CF45113, (int32_t)0x371AFCD5, (int32_t)0x8C7815DD, (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0x3514AB0E, (int32_t)0x8B86639A,
        (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0x330A35A1, (int32_t)0x8A9DC686, (int32_t)0x32037A45, (int32_t)0x8A2CE59F, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3,
        (int32_t)0x2FF31BDE, (int32_t)0x89520A1A, (int32_t)0x2EE9831F, (int32_t)0x88E813BF, (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0x2CD39871, (int32_t)0x881B202E,
        (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x2ABA2EE4, (int32_t)0x8757860C, (int32_t)0x29AC37A0, (int32_t)0x86F93F50, (int32_t)0x289D7061, (int32_t)0x869D5499,
        (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x267D8713, (int32_t)0x85EC9A5B, (int32_t)0x256C6F9F, (int32_t)0x8597CE46, (int32_t)0x245A9D65, (int32_t)0x8545651A,
        (int32_t)0x234815BA, (int32_t)0x84F56073, (int32_t)0x2234DDFB, (int32_t)0x84A7C1DF, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x200C73B4, (int32_t)0x8413BCF5,
        (int32_t)0x1EF74BF3, (int32_t)0x83CD5982, (int32_t)0x1DE189A6, (int32_t)0x838961E8, (int32_t)0x1CCB3237, (int32_t)0x8347D77B, (int32_t)0x1BB44B14, (int32_t)0x8308BB81,
        (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x1984E373, (int32_t)0x8291D3C9, (int32_t)0x186C6DDD, (int32_t)0x825A0A5B, (int32_t)0x17537E63, (int32_t)0x8224B404,
        (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x152047AB, (int32_t)0x81C164B6, (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x12EB6B35, (int32_t)0x8167ED9E,
        (int32_t)0x11D06C97, (int32_t)0x813EE55B, (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x0F996A26, (int32_t)0x80F43F69, (int32_t)0x0E7D7162, (int32_t)0x80D2A32F,
        (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x0C44AC72, (int32_t)0x8096DB7D, (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0x0A0AF299, (int32_t)0x80650347,
        (int32_t)0x08EDC7B7, (int32_t)0x804FD23A, (int32_t)0x07D07044, (int32_t)0x803D1E72, (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, (int32_t)0x059551F1, (int32_t)0x801F3019,
        (int32_t)0x04779632, (int32_t)0x8013F61D, (int32_t)0x0359C428, (int32_t)0x800B3A91, (int32_t)0x023BE165, (int32_t)0x8004FDA0, (int32_t)0x011DF37C, (int32_t)0x80013F6A,
};


static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr360_32x32, 
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr360_32x32,
    __fft_real_tw
};

const fft_handle_t rnfft32_720 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_720 = (const fft_handle_t)&__rifft_descr;
