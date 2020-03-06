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
ALIGN(8) static const int32_t __fft576_tw1[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFE0CEB, (int32_t)0xFE9A904C, 
    (int32_t)0x7FF833BD, (int32_t)0xFD352B7B, (int32_t)0x7FEE74A2, (int32_t)0xFBCFDC71, (int32_t)0x7FF833BD, (int32_t)0xFD352B7B, (int32_t)0x7FE0CFE7, (int32_t)0xFA6AAE0F, 
    (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7FEE74A2, (int32_t)0xFBCFDC71, (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7F62368F, (int32_t)0xF3742CA2, 
    (int32_t)0x7FE0CFE7, (int32_t)0xFA6AAE0F, (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7FCF45F7, (int32_t)0xF905AB37, 
    (int32_t)0x7F3D3CF4, (int32_t)0xF210A676, (int32_t)0x7E4A5426, (int32_t)0xEB263DBB, (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, 
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7FA084B5, (int32_t)0xF63C53A4, (int32_t)0x7E82A146, (int32_t)0xEC87388C, (int32_t)0x7CA80038, (int32_t)0xE2EF2A3E, 
    (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7F62368F, (int32_t)0xF3742CA2, 
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x7F3D3CF4, (int32_t)0xF210A676, (int32_t)0x7CF7447F, (int32_t)0xE44BB4EC, 
    (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x7F14631D, (int32_t)0xF0AD8CF6, (int32_t)0x7C54EFDC, (int32_t)0xE1938239, (int32_t)0x77CBC3F2, (int32_t)0xD2E9786E, 
    (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x7EB713DB, (int32_t)0xEDE8CB35, 
    (int32_t)0x7AE2E9E4, (int32_t)0xDC2E9018, (int32_t)0x74972F92, (int32_t)0xCB2C6A82, (int32_t)0x7E82A146, (int32_t)0xEC87388C, (int32_t)0x7A1365A5, (int32_t)0xD98278ED, 
    (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x7E4A5426, (int32_t)0xEB263DBB, (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, 
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x7DCE313F, (int32_t)0xE8663A9E, 
    (int32_t)0x774C08AB, (int32_t)0xD19BA15F, (int32_t)0x6CB2A837, (int32_t)0xBC6845CE, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, 
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x7D42BA46, (int32_t)0xE5A917A5, (int32_t)0x7528ED32, (int32_t)0xCC72CBE5, (int32_t)0x680B5C33, (int32_t)0xB5715EEF, 
    (int32_t)0x7CF7447F, (int32_t)0xE44BB4EC, (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x7CA80038, (int32_t)0xE2EF2A3E, 
    (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x7C54EFDC, (int32_t)0xE1938239, (int32_t)0x7189922C, (int32_t)0xC4E56BE4, 
    (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x7BFE15F1, (int32_t)0xE038C773, (int32_t)0x70389514, (int32_t)0xC26EF5C4, (int32_t)0x5D6C2F99, (int32_t)0xA8800C26, 
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x7B451023, (int32_t)0xDD8643DE, 
    (int32_t)0x6D6DC08F, (int32_t)0xBD98D67F, (int32_t)0x577FF3DA, (int32_t)0xA293D067, (int32_t)0x7AE2E9E4, (int32_t)0xDC2E9018, (int32_t)0x6BF4403B, (int32_t)0xBB39C435, 
    (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, 
    (int32_t)0x7A1365A5, (int32_t)0xD98278ED, (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x79A60DF8, (int32_t)0xD82E2A5E, 
    (int32_t)0x673993A9, (int32_t)0xB44FF727, (int32_t)0x4A8EA111, (int32_t)0x97F4A3CD, (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, 
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x78C0442A, (int32_t)0xD5893B24, (int32_t)0x63D34137, (int32_t)0xAFE1BD5B, (int32_t)0x4397BA32, (int32_t)0x934D57C9, 
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x77CBC3F2, (int32_t)0xD2E9786E, 
    (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0x774C08AB, (int32_t)0xD19BA15F, (int32_t)0x5E5F1A91, (int32_t)0xA9864294, 
    (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0x76C8AB19, (int32_t)0xD04F3416, (int32_t)0x5C766C1C, (int32_t)0xA77C800C, (int32_t)0x34D3957E, (int32_t)0x8B68D06E, 
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x75B71932, (int32_t)0xCDBABF4A, 
    (int32_t)0x58837FF4, (int32_t)0xA38993E4, (int32_t)0x2D168792, (int32_t)0x88343C0E, (int32_t)0x7528ED32, (int32_t)0xCC72CBE5, (int32_t)0x5679BD6C, (int32_t)0xA1A0E56F, 
    (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x74972F92, (int32_t)0xCB2C6A82, (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0x25280C5E, (int32_t)0x8582FAA5, 
    (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x7369114C, (int32_t)0xC8A48578, 
    (int32_t)0x501E42A5, (int32_t)0x9C2CBEC9, (int32_t)0x1D10D5C2, (int32_t)0x8357FFC8, (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, 
    (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x722CE331, (int32_t)0xC6235F1D, (int32_t)0x4BB008D9, (int32_t)0x98C66C57, (int32_t)0x14D9C245, (int32_t)0x81B5ABDA, 
    (int32_t)0x7189922C, (int32_t)0xC4E56BE4, (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, 
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x70389514, (int32_t)0xC26EF5C4, (int32_t)0x44C63BCB, (int32_t)0x940BBFC5, 
    (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x6F8AF344, (int32_t)0xC1368612, (int32_t)0x42672981, (int32_t)0x92923F71, (int32_t)0x0430238F, (int32_t)0x80118B5E, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x6E258390, (int32_t)0xBECB6D02, 
    (int32_t)0x3D910A3C, (int32_t)0x8FC76AEC, (int32_t)0xFBCFDC71, (int32_t)0x80118B5E, (int32_t)0x6D6DC08F, (int32_t)0xBD98D67F, (int32_t)0x3B1A941C, (int32_t)0x8E766DD4, 
    (int32_t)0xF7A0DEC9, (int32_t)0x804628A7, (int32_t)0x6CB2A837, (int32_t)0xBC6845CE, (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0xF3742CA2, (int32_t)0x809DC971, 
    (int32_t)0x6BF4403B, (int32_t)0xBB39C435, (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0xEF4AEAF1, (int32_t)0x811855B4, (int32_t)0x6B328E68, (int32_t)0xBA0D5AEC, 
    (int32_t)0x338D341B, (int32_t)0x8AD712CE, (int32_t)0xEB263DBB, (int32_t)0x81B5ABDA, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, 
    (int32_t)0xE70747C4, (int32_t)0x8275A0C0, (int32_t)0x69A564F0, (int32_t)0xB7BAF5D3, (int32_t)0x2E645EA1, (int32_t)0x88B3F755, (int32_t)0xE2EF2A3E, (int32_t)0x8357FFC8, 
    (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x680B5C33, (int32_t)0xB5715EEF, 
    (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0xDAD7F3A2, (int32_t)0x8582FAA5, (int32_t)0x673993A9, (int32_t)0xB44FF727, (int32_t)0x267D8713, (int32_t)0x85EC9A5B, 
    (int32_t)0xD6DB1254, (int32_t)0x86CAFE57, (int32_t)0x6664A628, (int32_t)0xB330DD99, (int32_t)0x23D16FE8, (int32_t)0x851D161C, (int32_t)0xD2E9786E, (int32_t)0x88343C0E, 
    (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, (int32_t)0x64B1764E, (int32_t)0xB0F9B80F, 
    (int32_t)0x1E6C7DC7, (int32_t)0x83AB1024, (int32_t)0xCB2C6A82, (int32_t)0x8B68D06E, (int32_t)0x63D34137, (int32_t)0xAFE1BD5B, (int32_t)0x1BB44B14, (int32_t)0x8308BB81, 
    (int32_t)0xC763158E, (int32_t)0x8D334625, (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0xC3A94590, (int32_t)0x8F1D343A, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x61267EC8, (int32_t)0xACA8939D, 
    (int32_t)0x1378C774, (int32_t)0x817D5EBA, (int32_t)0xBC6845CE, (int32_t)0x934D57C9, (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x10B5150F, (int32_t)0x811855B4, 
    (int32_t)0xB8E31319, (int32_t)0x9592675C, (int32_t)0x5F4F259D, (int32_t)0xAA8F1B5C, (int32_t)0x0DEF598A, (int32_t)0x80C2C30C, (int32_t)0xB5715EEF, (int32_t)0x97F4A3CD, 
    (int32_t)0x5E5F1A91, (int32_t)0xA9864294, (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0xB2141B02, (int32_t)0x9A7365D3, (int32_t)0x5D6C2F99, (int32_t)0xA8800C26, 
    (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0xAECC336C, (int32_t)0x9D0DFE54, (int32_t)0x5C766C1C, (int32_t)0xA77C800C, (int32_t)0x059551F1, (int32_t)0x801F3019, 
    (int32_t)0xAB9A8E6C, (int32_t)0x9FC3B694, (int32_t)0x5B7DD796, (int32_t)0xA67BA631, (int32_t)0x02CAD485, (int32_t)0x8007CC43, (int32_t)0xA8800C26, (int32_t)0xA293D067, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x598459CF, (int32_t)0xA482286A, 
    (int32_t)0xFD352B7B, (int32_t)0x8007CC43, (int32_t)0xA293D067, (int32_t)0xA8800C26, (int32_t)0x58837FF4, (int32_t)0xA38993E4, (int32_t)0xFA6AAE0F, (int32_t)0x801F3019, 
    (int32_t)0x9FC3B694, (int32_t)0xAB9A8E6C, (int32_t)0x577FF3DA, (int32_t)0xA293D067, (int32_t)0xF7A0DEC9, (int32_t)0x804628A7, (int32_t)0x9D0DFE54, (int32_t)0xAECC336C, 
    (int32_t)0x5679BD6C, (int32_t)0xA1A0E56F, (int32_t)0xF4D814A4, (int32_t)0x807CB130, (int32_t)0x9A7365D3, (int32_t)0xB2141B02, (int32_t)0x5570E4A4, (int32_t)0xA0B0DA63, 
    (int32_t)0xF210A676, (int32_t)0x80C2C30C, (int32_t)0x97F4A3CD, (int32_t)0xB5715EEF, (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0xEF4AEAF1, (int32_t)0x811855B4, 
    (int32_t)0x9592675C, (int32_t)0xB8E31319, (int32_t)0x53576C63, (int32_t)0x9ED98138, (int32_t)0xEC87388C, (int32_t)0x817D5EBA, (int32_t)0x934D57C9, (int32_t)0xBC6845CE, 
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, 
    (int32_t)0xE70747C4, (int32_t)0x8275A0C0, (int32_t)0x8F1D343A, (int32_t)0xC3A94590, (int32_t)0x501E42A5, (int32_t)0x9C2CBEC9, (int32_t)0xE44BB4EC, (int32_t)0x8308BB81, 
    (int32_t)0x8D334625, (int32_t)0xC763158E, (int32_t)0x4F0647F1, (int32_t)0x9B4E89B2, (int32_t)0xE1938239, (int32_t)0x83AB1024, (int32_t)0x8B68D06E, (int32_t)0xCB2C6A82, 
    (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, (int32_t)0x4CCF2267, (int32_t)0x999B59D8, 
    (int32_t)0xDC2E9018, (int32_t)0x851D161C, (int32_t)0x88343C0E, (int32_t)0xD2E9786E, (int32_t)0x4BB008D9, (int32_t)0x98C66C57, (int32_t)0xD98278ED, (int32_t)0x85EC9A5B, 
    (int32_t)0x86CAFE57, (int32_t)0xD6DB1254, (int32_t)0x4A8EA111, (int32_t)0x97F4A3CD, (int32_t)0xD6DB1254, (int32_t)0x86CAFE57, (int32_t)0x8582FAA5, (int32_t)0xDAD7F3A2, 
    (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0xD438AF17, (int32_t)0x87B826F7, (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0x48450A2D, (int32_t)0x965A9B10, 
    (int32_t)0xD19BA15F, (int32_t)0x88B3F755, (int32_t)0x8357FFC8, (int32_t)0xE2EF2A3E, (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, 
    (int32_t)0x8275A0C0, (int32_t)0xE70747C4, (int32_t)0x45F2A514, (int32_t)0x94CD7198, (int32_t)0xCC72CBE5, (int32_t)0x8AD712CE, (int32_t)0x81B5ABDA, (int32_t)0xEB263DBB, 
    (int32_t)0x44C63BCB, (int32_t)0x940BBFC5, (int32_t)0xC9E7A512, (int32_t)0x8BFE1B3F, (int32_t)0x811855B4, (int32_t)0xEF4AEAF1, (int32_t)0x4397BA32, (int32_t)0x934D57C9, 
    (int32_t)0xC763158E, (int32_t)0x8D334625, (int32_t)0x809DC971, (int32_t)0xF3742CA2, (int32_t)0x42672981, (int32_t)0x92923F71, (int32_t)0xC4E56BE4, (int32_t)0x8E766DD4, 
    (int32_t)0x804628A7, (int32_t)0xF7A0DEC9, (int32_t)0x413492FE, (int32_t)0x91DA7C70, (int32_t)0xC26EF5C4, (int32_t)0x8FC76AEC, (int32_t)0x80118B5E, (int32_t)0xFBCFDC71, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, (int32_t)0x3EC979EE, (int32_t)0x90750CBC, 
    (int32_t)0xBD98D67F, (int32_t)0x92923F71, (int32_t)0x80118B5E, (int32_t)0x0430238F, (int32_t)0x3D910A3C, (int32_t)0x8FC76AEC, (int32_t)0xBB39C435, (int32_t)0x940BBFC5, 
    (int32_t)0x804628A7, (int32_t)0x085F2137, (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0xB8E31319, (int32_t)0x9592675C, (int32_t)0x809DC971, (int32_t)0x0C8BD35E, 
    (int32_t)0x3B1A941C, (int32_t)0x8E766DD4, (int32_t)0xB6950C1E, (int32_t)0x9726069C, (int32_t)0x811855B4, (int32_t)0x10B5150F, (int32_t)0x39DCA0E3, (int32_t)0x8DD31CCF, 
    (int32_t)0xB44FF727, (int32_t)0x98C66C57, (int32_t)0x81B5ABDA, (int32_t)0x14D9C245, (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0xB2141B02, (int32_t)0x9A7365D3, 
    (int32_t)0x8275A0C0, (int32_t)0x18F8B83C, (int32_t)0x375B7A88, (int32_t)0x8C96EEB4, (int32_t)0xAFE1BD5B, (int32_t)0x9C2CBEC9, (int32_t)0x8357FFC8, (int32_t)0x1D10D5C2, 
    (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0xADB922B7, (int32_t)0x9DF24175, (int32_t)0x845C8AE3, (int32_t)0x2120FB83, (int32_t)0x34D3957E, (int32_t)0x8B68D06E, 
    (int32_t)0xAB9A8E6C, (int32_t)0x9FC3B694, (int32_t)0x8582FAA5, (int32_t)0x25280C5E, (int32_t)0x338D341B, (int32_t)0x8AD712CE, (int32_t)0xA9864294, (int32_t)0xA1A0E56F, 
    (int32_t)0x86CAFE57, (int32_t)0x2924EDAC, (int32_t)0x324540B6, (int32_t)0x8A48E6CE, (int32_t)0xA77C800C, (int32_t)0xA38993E4, (int32_t)0x88343C0E, (int32_t)0x2D168792, 
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, (int32_t)0x2FB0CBEA, (int32_t)0x893754E7, 
    (int32_t)0xA38993E4, (int32_t)0xA77C800C, (int32_t)0x8B68D06E, (int32_t)0x34D3957E, (int32_t)0x2E645EA1, (int32_t)0x88B3F755, (int32_t)0xA1A0E56F, (int32_t)0xA9864294, 
    (int32_t)0x8D334625, (int32_t)0x389CEA72, (int32_t)0x2D168792, (int32_t)0x88343C0E, (int32_t)0x9FC3B694, (int32_t)0xAB9A8E6C, (int32_t)0x8F1D343A, (int32_t)0x3C56BA70, 
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x9DF24175, (int32_t)0xADB922B7, (int32_t)0x9126145F, (int32_t)0x40000000, (int32_t)0x2A76C4DC, (int32_t)0x873FBBD6, 
    (int32_t)0x9C2CBEC9, (int32_t)0xAFE1BD5B, (int32_t)0x934D57C9, (int32_t)0x4397BA32, (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x9A7365D3, (int32_t)0xB2141B02, 
    (int32_t)0x9592675C, (int32_t)0x471CECE7, (int32_t)0x27D1D5A2, (int32_t)0x8659F208, (int32_t)0x98C66C57, (int32_t)0xB44FF727, (int32_t)0x97F4A3CD, (int32_t)0x4A8EA111, 
    (int32_t)0x267D8713, (int32_t)0x85EC9A5B, (int32_t)0x9726069C, (int32_t)0xB6950C1E, (int32_t)0x9A7365D3, (int32_t)0x4DEBE4FE, (int32_t)0x25280C5E, (int32_t)0x8582FAA5, 
    (int32_t)0x9592675C, (int32_t)0xB8E31319, (int32_t)0x9D0DFE54, (int32_t)0x5133CC94, (int32_t)0x23D16FE8, (int32_t)0x851D161C, (int32_t)0x940BBFC5, (int32_t)0xBB39C435, 
    (int32_t)0x9FC3B694, (int32_t)0x54657194, (int32_t)0x2279BC22, (int32_t)0x84BAEFDD, (int32_t)0x92923F71, (int32_t)0xBD98D67F, (int32_t)0xA293D067, (int32_t)0x577FF3DA, 
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0xA57D8666, (int32_t)0x5A82799A, (int32_t)0x1FC7388D, (int32_t)0x8401EA0F, 
    (int32_t)0x8FC76AEC, (int32_t)0xC26EF5C4, (int32_t)0xA8800C26, (int32_t)0x5D6C2F99, (int32_t)0x1E6C7DC7, (int32_t)0x83AB1024, (int32_t)0x8E766DD4, (int32_t)0xC4E56BE4, 
    (int32_t)0xAB9A8E6C, (int32_t)0x603C496C, (int32_t)0x1D10D5C2, (int32_t)0x8357FFC8, (int32_t)0x8D334625, (int32_t)0xC763158E, (int32_t)0xAECC336C, (int32_t)0x62F201AC, 
    (int32_t)0x1BB44B14, (int32_t)0x8308BB81, (int32_t)0x8BFE1B3F, (int32_t)0xC9E7A512, (int32_t)0xB2141B02, (int32_t)0x658C9A2D, (int32_t)0x1A56E85B, (int32_t)0x82BD45BA, 
    (int32_t)0x8AD712CE, (int32_t)0xCC72CBE5, (int32_t)0xB5715EEF, (int32_t)0x680B5C33, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, 
    (int32_t)0xB8E31319, (int32_t)0x6A6D98A4, (int32_t)0x1799C562, (int32_t)0x8231CEC1, (int32_t)0x88B3F755, (int32_t)0xD19BA15F, (int32_t)0xBC6845CE, (int32_t)0x6CB2A837, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, (int32_t)0xC0000000, (int32_t)0x6ED9EBA1, (int32_t)0x14D9C245, (int32_t)0x81B5ABDA, 
    (int32_t)0x86CAFE57, (int32_t)0xD6DB1254, (int32_t)0xC3A94590, (int32_t)0x70E2CBC6, (int32_t)0x1378C774, (int32_t)0x817D5EBA, (int32_t)0x85EC9A5B, (int32_t)0xD98278ED, 
    (int32_t)0xC763158E, (int32_t)0x72CCB9DB, (int32_t)0x121734CB, (int32_t)0x8148EC25, (int32_t)0x851D161C, (int32_t)0xDC2E9018, (int32_t)0xCB2C6A82, (int32_t)0x74972F92, 
    (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0xCF043AB3, (int32_t)0x7641AF3D, (int32_t)0x0F52730A, (int32_t)0x80EB9CE3, 
    (int32_t)0x83AB1024, (int32_t)0xE1938239, (int32_t)0xD2E9786E, (int32_t)0x77CBC3F2, (int32_t)0x0DEF598A, (int32_t)0x80C2C30C, (int32_t)0x8308BB81, (int32_t)0xE44BB4EC, 
    (int32_t)0xD6DB1254, (int32_t)0x793501A9, (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x8275A0C0, (int32_t)0xE70747C4, (int32_t)0xDAD7F3A2, (int32_t)0x7A7D055B, 
    (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0x81F1D1CE, (int32_t)0xE9C5E582, (int32_t)0xDEDF047D, (int32_t)0x7BA3751D, (int32_t)0x09C3AC5C, (int32_t)0x805F7B4B, 
    (int32_t)0x817D5EBA, (int32_t)0xEC87388C, (int32_t)0xE2EF2A3E, (int32_t)0x7CA80038, (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x811855B4, (int32_t)0xEF4AEAF1, 
    (int32_t)0xE70747C4, (int32_t)0x7D8A5F40, (int32_t)0x06FA54C9, (int32_t)0x8030BA09, (int32_t)0x80C2C30C, (int32_t)0xF210A676, (int32_t)0xEB263DBB, (int32_t)0x7E4A5426, 
    (int32_t)0x059551F1, (int32_t)0x801F3019, (int32_t)0x807CB130, (int32_t)0xF4D814A4, (int32_t)0xEF4AEAF1, (int32_t)0x7EE7AA4C, (int32_t)0x0430238F, (int32_t)0x80118B5E, 
    (int32_t)0x804628A7, (int32_t)0xF7A0DEC9, (int32_t)0xF3742CA2, (int32_t)0x7F62368F, (int32_t)0x02CAD485, (int32_t)0x8007CC43, (int32_t)0x801F3019, (int32_t)0xFA6AAE0F, 
    (int32_t)0xF7A0DEC9, (int32_t)0x7FB9D759, (int32_t)0x01656FB4, (int32_t)0x8001F315, (int32_t)0x8007CC43, (int32_t)0xFD352B7B, (int32_t)0xFBCFDC71, (int32_t)0x7FEE74A2, 
};

/****************** stage 2 radix 4 ******************/
ALIGN(8) static const int32_t __fft576_tw2[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FE0CFE7, (int32_t)0xFA6AAE0F, 
    (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, 
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, 
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x7CF7447F, (int32_t)0xE44BB4EC, 
    (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x7A1365A5, (int32_t)0xD98278ED, (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, 
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x5246DD49, (int32_t)0x9DF24175, 
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x7189922C, (int32_t)0xC4E56BE4, (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0x10B5150F, (int32_t)0x811855B4, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x6BF4403B, (int32_t)0xBB39C435, 
    (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0xEF4AEAF1, (int32_t)0x811855B4, (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x2BC750E9, (int32_t)0x87B826F7, 
    (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x5E5F1A91, (int32_t)0xA9864294, 
    (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0xB2141B02, (int32_t)0x9A7365D3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, 
    (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x5679BD6C, (int32_t)0xA1A0E56F, (int32_t)0xF4D814A4, (int32_t)0x807CB130, (int32_t)0x9A7365D3, (int32_t)0xB2141B02, 
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, 
    (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0xD438AF17, (int32_t)0x87B826F7, 
    (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0x44C63BCB, (int32_t)0x940BBFC5, (int32_t)0xC9E7A512, (int32_t)0x8BFE1B3F, (int32_t)0x811855B4, (int32_t)0xEF4AEAF1, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, (int32_t)0x3B1A941C, (int32_t)0x8E766DD4, 
    (int32_t)0xB6950C1E, (int32_t)0x9726069C, (int32_t)0x811855B4, (int32_t)0x10B5150F, (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0xADB922B7, (int32_t)0x9DF24175, 
    (int32_t)0x845C8AE3, (int32_t)0x2120FB83, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, 
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x9DF24175, (int32_t)0xADB922B7, (int32_t)0x9126145F, (int32_t)0x40000000, (int32_t)0x267D8713, (int32_t)0x85EC9A5B, 
    (int32_t)0x9726069C, (int32_t)0xB6950C1E, (int32_t)0x9A7365D3, (int32_t)0x4DEBE4FE, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000, 
    (int32_t)0xA57D8666, (int32_t)0x5A82799A, (int32_t)0x1BB44B14, (int32_t)0x8308BB81, (int32_t)0x8BFE1B3F, (int32_t)0xC9E7A512, (int32_t)0xB2141B02, (int32_t)0x658C9A2D, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, (int32_t)0xC0000000, (int32_t)0x6ED9EBA1, (int32_t)0x10B5150F, (int32_t)0x811855B4, 
    (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0xCF043AB3, (int32_t)0x7641AF3D, (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0x81F1D1CE, (int32_t)0xE9C5E582, 
    (int32_t)0xDEDF047D, (int32_t)0x7BA3751D, (int32_t)0x059551F1, (int32_t)0x801F3019, (int32_t)0x807CB130, (int32_t)0xF4D814A4, (int32_t)0xEF4AEAF1, (int32_t)0x7EE7AA4C, 
};

/****************** stage 3 radix 4 ******************/
ALIGN(8) static const int32_t __fft576_tw3[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, 
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x40000000, (int32_t)0x9126145F, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0xC0000000, (int32_t)0x9126145F, 
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, (int32_t)0x9126145F, (int32_t)0xC0000000, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, 
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x9DF24175, (int32_t)0xADB922B7, (int32_t)0x9126145F, (int32_t)0x40000000, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, (int32_t)0xC0000000, (int32_t)0x6ED9EBA1, 
};

/****************** stage 4 radix 3 ******************/
ALIGN(8) static const int32_t __fft576_tw4[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, 
};
#define N 576
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 1, 1, 
}; 
static const cint32ptr_t tw_tab[] = 
{
    __fft576_tw1, __fft576_tw2, __fft576_tw3, __fft576_tw4, NULL
};
 const fft_cplx32x32_descr_t __cfft_descr576_32x32 = 
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
 };     
 const fft_cplx32x32_descr_t __cifft_descr576_32x32 =
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_576 = (const fft_handle_t)&__cfft_descr576_32x32;
const fft_handle_t cinfft32_576 = (const fft_handle_t)&__cifft_descr576_32x32;
