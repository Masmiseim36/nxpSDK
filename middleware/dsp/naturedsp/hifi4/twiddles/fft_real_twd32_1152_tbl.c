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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFF833B, (int32_t)0xFF4D4778, (int32_t)0x7FFE0CEB, (int32_t)0xFE9A904C, (int32_t)0x7FFB9D15, (int32_t)0xFDE7DBD9,
    (int32_t)0x7FF833BD, (int32_t)0xFD352B7B, (int32_t)0x7FF3D0E9, (int32_t)0xFC82808F, (int32_t)0x7FEE74A2, (int32_t)0xFBCFDC71, (int32_t)0x7FE81EF3, (int32_t)0xFB1D407D,
    (int32_t)0x7FE0CFE7, (int32_t)0xFA6AAE0F, (int32_t)0x7FD8878E, (int32_t)0xF9B82684, (int32_t)0x7FCF45F7, (int32_t)0xF905AB37, (int32_t)0x7FC50B34, (int32_t)0xF8533D85,
    (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7FADAA7C, (int32_t)0xF6EE9060, (int32_t)0x7FA084B5, (int32_t)0xF63C53A4, (int32_t)0x7F92661D, (int32_t)0xF58A29F2,
    (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7F733EEC, (int32_t)0xF4261515, (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7F5035DB, (int32_t)0xF2C25CA4,
    (int32_t)0x7F3D3CF4, (int32_t)0xF210A676, (int32_t)0x7F294BFD, (int32_t)0xF15F0B74, (int32_t)0x7F14631D, (int32_t)0xF0AD8CF6, (int32_t)0x7EFE827F, (int32_t)0xEFFC2C57,
    (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7ECFDAB0, (int32_t)0xEE99CA1D, (int32_t)0x7EB713DB, (int32_t)0xEDE8CB35, (int32_t)0x7E9D55FC, (int32_t)0xED37EF91,
    (int32_t)0x7E82A146, (int32_t)0xEC87388C, (int32_t)0x7E66F5ED, (int32_t)0xEBD6A77C, (int32_t)0x7E4A5426, (int32_t)0xEB263DBB, (int32_t)0x7E2CBC2A, (int32_t)0xEA75FC9F,
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7DEEAA7A, (int32_t)0xE915F9BA, (int32_t)0x7DCE313F, (int32_t)0xE8663A9E, (int32_t)0x7DACC2C1, (int32_t)0xE7B6A984,
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7D670700, (int32_t)0xE65816B2, (int32_t)0x7D42BA46, (int32_t)0xE5A917A5, (int32_t)0x7D1D7958, (int32_t)0xE4FA4BF1,
    (int32_t)0x7CF7447F, (int32_t)0xE44BB4EC, (int32_t)0x7CD01C06, (int32_t)0xE39D53EA, (int32_t)0x7CA80038, (int32_t)0xE2EF2A3E, (int32_t)0x7C7EF165, (int32_t)0xE241393D,
    (int32_t)0x7C54EFDC, (int32_t)0xE1938239, (int32_t)0x7C29FBEE, (int32_t)0xE0E60685, (int32_t)0x7BFE15F1, (int32_t)0xE038C773, (int32_t)0x7BD13E39, (int32_t)0xDF8BC655,
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7B74BAF8, (int32_t)0xDE32833A, (int32_t)0x7B451023, (int32_t)0xDD8643DE, (int32_t)0x7B1474FD, (int32_t)0xDCDA47B9,
    (int32_t)0x7AE2E9E4, (int32_t)0xDC2E9018, (int32_t)0x7AB06F37, (int32_t)0xDB831E4C, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x7A48ACB3, (int32_t)0xDA2D1169,
    (int32_t)0x7A1365A5, (int32_t)0xD98278ED, (int32_t)0x79DD3098, (int32_t)0xD8D82B7A, (int32_t)0x79A60DF8, (int32_t)0xD82E2A5E, (int32_t)0x796DFE2E, (int32_t)0xD78476E3,
    (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x78FB18D7, (int32_t)0xD631FDFC, (int32_t)0x78C0442A, (int32_t)0xD5893B24, (int32_t)0x78848414, (int32_t)0xD4E0CB15,
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x780A4381, (int32_t)0xD390E872, (int32_t)0x77CBC3F2, (int32_t)0xD2E9786E, (int32_t)0x778C5AD7, (int32_t)0xD2426050,
    (int32_t)0x774C08AB, (int32_t)0xD19BA15F, (int32_t)0x770ACDEC, (int32_t)0xD0F53CE0, (int32_t)0x76C8AB19, (int32_t)0xD04F3416, (int32_t)0x7685A0B3, (int32_t)0xCFA98846,
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x75FCD73A, (int32_t)0xCE5F4C9E, (int32_t)0x75B71932, (int32_t)0xCDBABF4A, (int32_t)0x757075AC, (int32_t)0xCD1693F7,
    (int32_t)0x7528ED32, (int32_t)0xCC72CBE5, (int32_t)0x74E08050, (int32_t)0xCBCF6854, (int32_t)0x74972F92, (int32_t)0xCB2C6A82, (int32_t)0x744CFB87, (int32_t)0xCA89D3AD,
    (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x73B5EBD1, (int32_t)0xC945DFEC, (int32_t)0x7369114C, (int32_t)0xC8A48578, (int32_t)0x731B55C7, (int32_t)0xC80396F0,
    (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x727D3E20, (int32_t)0xC6C3028A, (int32_t)0x722CE331, (int32_t)0xC6235F1D, (int32_t)0x71DBA9AB, (int32_t)0xC5842C7E,
    (int32_t)0x7189922C, (int32_t)0xC4E56BE4, (int32_t)0x71369D54, (int32_t)0xC4471E83, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x708E1E24, (int32_t)0xC30BE23F,
    (int32_t)0x70389514, (int32_t)0xC26EF5C4, (int32_t)0x6FE2313C, (int32_t)0xC1D2814F, (int32_t)0x6F8AF344, (int32_t)0xC1368612, (int32_t)0x6F32DBD7, (int32_t)0xC09B053E,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6E80234F, (int32_t)0xBF657788, (int32_t)0x6E258390, (int32_t)0xBECB6D02, (int32_t)0x6DCA0D14, (int32_t)0xBE31E19B,
    (int32_t)0x6D6DC08F, (int32_t)0xBD98D67F, (int32_t)0x6D109EB3, (int32_t)0xBD004CD7, (int32_t)0x6CB2A837, (int32_t)0xBC6845CE, (int32_t)0x6C53DDD1, (int32_t)0xBBD0C28A,
    (int32_t)0x6BF4403B, (int32_t)0xBB39C435, (int32_t)0x6B93D02E, (int32_t)0xBAA34BF4, (int32_t)0x6B328E68, (int32_t)0xBA0D5AEC, (int32_t)0x6AD07BA5, (int32_t)0xB977F242,
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x6A09E627, (int32_t)0xB84EBE94, (int32_t)0x69A564F0, (int32_t)0xB7BAF5D3, (int32_t)0x694015C3, (int32_t)0xB727B9F7,
    (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x6873109C, (int32_t)0xB602ED67, (int32_t)0x680B5C33, (int32_t)0xB5715EEF, (int32_t)0x67A2DCF4, (int32_t)0xB4E061D0,
    (int32_t)0x673993A9, (int32_t)0xB44FF727, (int32_t)0x66CF8120, (int32_t)0xB3C0200C, (int32_t)0x6664A628, (int32_t)0xB330DD99, (int32_t)0x65F90391, (int32_t)0xB2A230E3,
    (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x651F6AD0, (int32_t)0xB1869D0A, (int32_t)0x64B1764E, (int32_t)0xB0F9B80F, (int32_t)0x6442BD7E, (int32_t)0xB06D6D24,
    (int32_t)0x63D34137, (int32_t)0xAFE1BD5B, (int32_t)0x63630253, (int32_t)0xAF56A9C3, (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x62804020, (int32_t)0xAE425B64,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x619A7DCE, (int32_t)0xAD308A71, (int32_t)0x61267EC8, (int32_t)0xACA8939D, (int32_t)0x60B1C25B, (int32_t)0xAC213F43,
    (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x5FC614E0, (int32_t)0xAB14821D, (int32_t)0x5F4F259D, (int32_t)0xAA8F1B5C, (int32_t)0x5ED77C8A, (int32_t)0xAA0A5B2E,
    (int32_t)0x5E5F1A91, (int32_t)0xA9864294, (int32_t)0x5DE6009D, (int32_t)0xA902D292, (int32_t)0x5D6C2F99, (int32_t)0xA8800C26, (int32_t)0x5CF1A874, (int32_t)0xA7FDF04F,
    (int32_t)0x5C766C1C, (int32_t)0xA77C800C, (int32_t)0x5BFA7B82, (int32_t)0xA6FBBC59, (int32_t)0x5B7DD796, (int32_t)0xA67BA631, (int32_t)0x5B00814D, (int32_t)0xA5FC3E8D,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x5A03C173, (int32_t)0xA4FF7EB3, (int32_t)0x598459CF, (int32_t)0xA482286A, (int32_t)0x590443A7, (int32_t)0xA405847E,
    (int32_t)0x58837FF4, (int32_t)0xA38993E4, (int32_t)0x58020FB1, (int32_t)0xA30E578C, (int32_t)0x577FF3DA, (int32_t)0xA293D067, (int32_t)0x56FD2D6E, (int32_t)0xA219FF63,
    (int32_t)0x5679BD6C, (int32_t)0xA1A0E56F, (int32_t)0x55F5A4D2, (int32_t)0xA1288376, (int32_t)0x5570E4A4, (int32_t)0xA0B0DA63, (int32_t)0x54EB7DE3, (int32_t)0xA039EB20,
    (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0x53DEC0BD, (int32_t)0x9F4E3DA5, (int32_t)0x53576C63, (int32_t)0x9ED98138, (int32_t)0x52CF758F, (int32_t)0x9E658232,
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0x51BDA49C, (int32_t)0x9D7FBFE0, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0x50A9563D, (int32_t)0x9C9CFDAD,
    (int32_t)0x501E42A5, (int32_t)0x9C2CBEC9, (int32_t)0x4F9292DC, (int32_t)0x9BBD4282, (int32_t)0x4F0647F1, (int32_t)0x9B4E89B2, (int32_t)0x4E7962F6, (int32_t)0x9AE09530,
    (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x4D5DCF1D, (int32_t)0x9A06FC6F, (int32_t)0x4CCF2267, (int32_t)0x999B59D8, (int32_t)0x4C3FDFF4, (int32_t)0x99307EE0,
    (int32_t)0x4BB008D9, (int32_t)0x98C66C57, (int32_t)0x4B1F9E30, (int32_t)0x985D230C, (int32_t)0x4A8EA111, (int32_t)0x97F4A3CD, (int32_t)0x49FD1299, (int32_t)0x978CEF64,
    (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0x48D84609, (int32_t)0x96BFEA3D, (int32_t)0x48450A2D, (int32_t)0x965A9B10, (int32_t)0x47B1416C, (int32_t)0x95F619D9,
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x46880DBE, (int32_t)0x952F845B, (int32_t)0x45F2A514, (int32_t)0x94CD7198, (int32_t)0x455CB40C, (int32_t)0x946C2FD2,
    (int32_t)0x44C63BCB, (int32_t)0x940BBFC5, (int32_t)0x442F3D76, (int32_t)0x93AC222F, (int32_t)0x4397BA32, (int32_t)0x934D57C9, (int32_t)0x42FFB329, (int32_t)0x92EF614D,
    (int32_t)0x42672981, (int32_t)0x92923F71, (int32_t)0x41CE1E65, (int32_t)0x9235F2EC, (int32_t)0x413492FE, (int32_t)0x91DA7C70, (int32_t)0x409A8878, (int32_t)0x917FDCB1,
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x3F64FAC2, (int32_t)0x90CD2429, (int32_t)0x3EC979EE, (int32_t)0x90750CBC, (int32_t)0x3E2D7EB1, (int32_t)0x901DCEC4,
    (int32_t)0x3D910A3C, (int32_t)0x8FC76AEC, (int32_t)0x3CF41DC1, (int32_t)0x8F71E1DC, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0x3BB8E17D, (int32_t)0x8EC962AC,
    (int32_t)0x3B1A941C, (int32_t)0x8E766DD4, (int32_t)0x3A7BD382, (int32_t)0x8E245655, (int32_t)0x39DCA0E3, (int32_t)0x8DD31CCF, (int32_t)0x393CFD76, (int32_t)0x8D82C1E0,
    (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0x37FC6910, (int32_t)0x8CE4AA39, (int32_t)0x375B7A88, (int32_t)0x8C96EEB4, (int32_t)0x36BA2014, (int32_t)0x8C4A142F,
    (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0x35762C53, (int32_t)0x8BB30479, (int32_t)0x34D3957E, (int32_t)0x8B68D06E, (int32_t)0x343097AC, (int32_t)0x8B1F7FB0,
    (int32_t)0x338D341B, (int32_t)0x8AD712CE, (int32_t)0x32E96C09, (int32_t)0x8A8F8A54, (int32_t)0x324540B6, (int32_t)0x8A48E6CE, (int32_t)0x31A0B362, (int32_t)0x8A0328C6,
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x305677BA, (int32_t)0x897A5F4D, (int32_t)0x2FB0CBEA, (int32_t)0x893754E7, (int32_t)0x2F0AC320, (int32_t)0x88F53214,
    (int32_t)0x2E645EA1, (int32_t)0x88B3F755, (int32_t)0x2DBD9FB0, (int32_t)0x8873A529, (int32_t)0x2D168792, (int32_t)0x88343C0E, (int32_t)0x2C6F178E, (int32_t)0x87F5BC7F,
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x2B1F34EB, (int32_t)0x877B7BEC, (int32_t)0x2A76C4DC, (int32_t)0x873FBBD6, (int32_t)0x29CE0204, (int32_t)0x8704E729,
    (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x287B891D, (int32_t)0x869201D2, (int32_t)0x27D1D5A2, (int32_t)0x8659F208, (int32_t)0x2727D486, (int32_t)0x8622CF68,
    (int32_t)0x267D8713, (int32_t)0x85EC9A5B, (int32_t)0x25D2EE97, (int32_t)0x85B7534D, (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0x247CE1B4, (int32_t)0x854F90C9,
    (int32_t)0x23D16FE8, (int32_t)0x851D161C, (int32_t)0x2325B847, (int32_t)0x84EB8B03, (int32_t)0x2279BC22, (int32_t)0x84BAEFDD, (int32_t)0x21CD7CC6, (int32_t)0x848B4508,
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x207439AB, (int32_t)0x842EC1C7, (int32_t)0x1FC7388D, (int32_t)0x8401EA0F, (int32_t)0x1F19F97B, (int32_t)0x83D60412,
    (int32_t)0x1E6C7DC7, (int32_t)0x83AB1024, (int32_t)0x1DBEC6C3, (int32_t)0x83810E9B, (int32_t)0x1D10D5C2, (int32_t)0x8357FFC8, (int32_t)0x1C62AC16, (int32_t)0x832FE3FA,
    (int32_t)0x1BB44B14, (int32_t)0x8308BB81, (int32_t)0x1B05B40F, (int32_t)0x82E286A8, (int32_t)0x1A56E85B, (int32_t)0x82BD45BA, (int32_t)0x19A7E94E, (int32_t)0x8298F900,
    (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x1849567C, (int32_t)0x82533D3F, (int32_t)0x1799C562, (int32_t)0x8231CEC1, (int32_t)0x16EA0646, (int32_t)0x82115586,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x158A0361, (int32_t)0x81D343D6, (int32_t)0x14D9C245, (int32_t)0x81B5ABDA, (int32_t)0x14295884, (int32_t)0x81990A13,
    (int32_t)0x1378C774, (int32_t)0x817D5EBA, (int32_t)0x12C8106F, (int32_t)0x8162AA04, (int32_t)0x121734CB, (int32_t)0x8148EC25, (int32_t)0x116635E3, (int32_t)0x81302550,
    (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x1003D3A9, (int32_t)0x81017D81, (int32_t)0x0F52730A, (int32_t)0x80EB9CE3, (int32_t)0x0EA0F48C, (int32_t)0x80D6B403,
    (int32_t)0x0DEF598A, (int32_t)0x80C2C30C, (int32_t)0x0D3DA35C, (int32_t)0x80AFCA25, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x0BD9EAEB, (int32_t)0x808CC114,
    (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0x0A75D60E, (int32_t)0x806D99E3, (int32_t)0x09C3AC5C, (int32_t)0x805F7B4B, (int32_t)0x09116FA0, (int32_t)0x80525584,
    (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x07ACC27B, (int32_t)0x803AF4CC, (int32_t)0x06FA54C9, (int32_t)0x8030BA09, (int32_t)0x0647D97C, (int32_t)0x80277872,
    (int32_t)0x059551F1, (int32_t)0x801F3019, (int32_t)0x04E2BF83, (int32_t)0x8017E10D, (int32_t)0x0430238F, (int32_t)0x80118B5E, (int32_t)0x037D7F71, (int32_t)0x800C2F17,
    (int32_t)0x02CAD485, (int32_t)0x8007CC43, (int32_t)0x02182427, (int32_t)0x800462EB, (int32_t)0x01656FB4, (int32_t)0x8001F315, (int32_t)0x00B2B888, (int32_t)0x80007CC5,
};


static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr576_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr576_32x32,
    __fft_real_tw
};

const fft_handle_t rnfft32_1152 = (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_1152 = (const fft_handle_t)&__rifft_descr;
