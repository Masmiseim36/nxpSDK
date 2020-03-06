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

/****************** stage 1 radix 3 ******************/
ALIGN(8) static const int32_t __fft300_tw1[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FF8D03D, (int32_t)0xFD51C240, (int32_t)0x7FE341C4, (int32_t)0xFAA3D18E, 
    (int32_t)0x7FE341C4, (int32_t)0xFAA3D18E, (int32_t)0x7F8D13F9, (int32_t)0xF54A0B60, (int32_t)0x7FBF5700, (int32_t)0xF7F67AF2, (int32_t)0x7EFD9D52, (int32_t)0xEFF514A2, 
    (int32_t)0x7F8D13F9, (int32_t)0xF54A0B60, (int32_t)0x7E351E3F, (int32_t)0xEAA7525B, (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7D33F0CA, (int32_t)0xE5632654, 
    (int32_t)0x7EFD9D52, (int32_t)0xEFF514A2, (int32_t)0x7BFA8874, (int32_t)0xE02AEE06, (int32_t)0x7EA079CE, (int32_t)0xED4D26BD, (int32_t)0x7A8971FF, (int32_t)0xDB01018E, 
    (int32_t)0x7E351E3F, (int32_t)0xEAA7525B, (int32_t)0x78E1532D, (int32_t)0xD5E7B29E, (int32_t)0x7DBB96B1, (int32_t)0xE803E39B, (int32_t)0x7702EA78, (int32_t)0xD0E14B6F, 
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x7C9E3BC6, (int32_t)0xE2C56610, (int32_t)0x72A6AED6, (int32_t)0xC71631C7, 
    (int32_t)0x7BFA8874, (int32_t)0xE02AEE06, (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x7B48E937, (int32_t)0xDD94090B, (int32_t)0x6D7C937A, (int32_t)0xBDB14A7C, 
    (int32_t)0x7A8971FF, (int32_t)0xDB01018E, (int32_t)0x6A9D29C8, (int32_t)0xB92A7739, (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, 
    (int32_t)0x78E1532D, (int32_t)0xD5E7B29E, (int32_t)0x64501135, (int32_t)0xB07E3AC3, (int32_t)0x77F8DB32, (int32_t)0xD361FDBC, (int32_t)0x60E536C8, (int32_t)0xAC5CB6B2, 
    (int32_t)0x7702EA78, (int32_t)0xD0E14B6F, (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x75FF9C9C, (int32_t)0xCE65E3A7, (int32_t)0x598E917F, (int32_t)0xA48C28C4, 
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x73D15F73, (int32_t)0xC980106E, (int32_t)0x51971BFB, (int32_t)0x9D5FD273, 
    (int32_t)0x72A6AED6, (int32_t)0xC71631C7, (int32_t)0x4D6380F9, (int32_t)0x9A0B4EC2, (int32_t)0x716F1E6F, (int32_t)0xC4B2B72C, (int32_t)0x490D2466, (int32_t)0x96E4952D, 
    (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, 
    (int32_t)0x6D7C937A, (int32_t)0xBDB14A7C, (int32_t)0x3B4D48D4, (int32_t)0x8E90E191, (int32_t)0x6C12EFFE, (int32_t)0xBB6A0710, (int32_t)0x367FEF92, (int32_t)0x8C2EA08D, 
    (int32_t)0x6A9D29C8, (int32_t)0xB92A7739, (int32_t)0x319A1C59, (int32_t)0x8A006364, (int32_t)0x691B6AD3, (int32_t)0xB6F2DB9A, (int32_t)0x2C9E0244, (int32_t)0x880724CE, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x65F4B13E, (int32_t)0xB29C7F07, (int32_t)0x226BF6F5, (int32_t)0x84B716C9, 
    (int32_t)0x64501135, (int32_t)0xB07E3AC3, (int32_t)0x1D3A99F0, (int32_t)0x8361C43A, (int32_t)0x62A02D8D, (int32_t)0xAE68E405, (int32_t)0x17FC1C65, (int32_t)0x8244694F, 
    (int32_t)0x60E536C8, (int32_t)0xAC5CB6B2, (int32_t)0x12B2D943, (int32_t)0x815F8632, (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x0D61304E, (int32_t)0x80B381AC, 
    (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0x5B73D73C, (int32_t)0xA6716E81, (int32_t)0x02AE3DC0, (int32_t)0x80072FC3, 
    (int32_t)0x598E917F, (int32_t)0xA48C28C4, (int32_t)0xFD51C240, (int32_t)0x80072FC3, (int32_t)0x579F3D54, (int32_t)0xA2B127F1, (int32_t)0xF7F67AF2, (int32_t)0x8040A900, 
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x53A3494E, (int32_t)0x9F1AC938, (int32_t)0xED4D26BD, (int32_t)0x815F8632, 
    (int32_t)0x51971BFB, (int32_t)0x9D5FD273, (int32_t)0xE803E39B, (int32_t)0x8244694F, (int32_t)0x4F81C53D, (int32_t)0x9BAFEECB, (int32_t)0xE2C56610, (int32_t)0x8361C43A, 
    (int32_t)0x4D6380F9, (int32_t)0x9A0B4EC2, (int32_t)0xDD94090B, (int32_t)0x84B716C9, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0xD8722192, (int32_t)0x8643C7B3, 
    (int32_t)0x490D2466, (int32_t)0x96E4952D, (int32_t)0xD361FDBC, (int32_t)0x880724CE, (int32_t)0x46D588C7, (int32_t)0x9562D638, (int32_t)0xCE65E3A7, (int32_t)0x8A006364, 
    (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0xC980106E, (int32_t)0x8C2EA08D, (int32_t)0x424EB584, (int32_t)0x92836C86, (int32_t)0xC4B2B72C, (int32_t)0x8E90E191, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x3DAA1AB9, (int32_t)0x8FD52EC6, (int32_t)0xBB6A0710, (int32_t)0x93ED1002, 
    (int32_t)0x3B4D48D4, (int32_t)0x8E90E191, (int32_t)0xB6F2DB9A, (int32_t)0x96E4952D, (int32_t)0x38E9CE39, (int32_t)0x8D59512A, (int32_t)0xB29C7F07, (int32_t)0x9A0B4EC2, 
    (int32_t)0x367FEF92, (int32_t)0x8C2EA08D, (int32_t)0xAE68E405, (int32_t)0x9D5FD273, (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0xAA59EDA4, (int32_t)0xA0E0A15F, 
    (int32_t)0x319A1C59, (int32_t)0x8A006364, (int32_t)0xA6716E81, (int32_t)0xA48C28C4, (int32_t)0x2F1EB491, (int32_t)0x88FD1588, (int32_t)0xA2B127F1, (int32_t)0xA860C2AC, 
    (int32_t)0x2C9E0244, (int32_t)0x880724CE, (int32_t)0x9F1AC938, (int32_t)0xAC5CB6B2, (int32_t)0x2A184D62, (int32_t)0x871EACD3, (int32_t)0x9BAFEECB, (int32_t)0xB07E3AC3, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, (int32_t)0x24FEFE72, (int32_t)0x85768E01, (int32_t)0x9562D638, (int32_t)0xB92A7739, 
    (int32_t)0x226BF6F5, (int32_t)0x84B716C9, (int32_t)0x92836C86, (int32_t)0xBDB14A7C, (int32_t)0x1FD511FA, (int32_t)0x8405778C, (int32_t)0x8FD52EC6, (int32_t)0xC255E547, 
    (int32_t)0x1D3A99F0, (int32_t)0x8361C43A, (int32_t)0x8D59512A, (int32_t)0xC71631C7, (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x8B10F144, (int32_t)0xCBF00DBE, 
    (int32_t)0x17FC1C65, (int32_t)0x8244694F, (int32_t)0x88FD1588, (int32_t)0xD0E14B6F, (int32_t)0x1558ADA5, (int32_t)0x81CAE1C1, (int32_t)0x871EACD3, (int32_t)0xD5E7B29E, 
    (int32_t)0x12B2D943, (int32_t)0x815F8632, (int32_t)0x85768E01, (int32_t)0xDB01018E, (int32_t)0x100AEB5E, (int32_t)0x810262AE, (int32_t)0x8405778C, (int32_t)0xE02AEE06, 
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0xE5632654, (int32_t)0x0AB5F4A0, (int32_t)0x8072EC07, (int32_t)0x81CAE1C1, (int32_t)0xEAA7525B, 
    (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0x810262AE, (int32_t)0xEFF514A2, (int32_t)0x055C2E72, (int32_t)0x801CBE3C, (int32_t)0x8072EC07, (int32_t)0xF54A0B60, 
    (int32_t)0x02AE3DC0, (int32_t)0x80072FC3, (int32_t)0x801CBE3C, (int32_t)0xFAA3D18E, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x80000000, (int32_t)0x00000000, 
    (int32_t)0xFD51C240, (int32_t)0x80072FC3, (int32_t)0x801CBE3C, (int32_t)0x055C2E72, (int32_t)0xFAA3D18E, (int32_t)0x801CBE3C, (int32_t)0x8072EC07, (int32_t)0x0AB5F4A0, 
    (int32_t)0xF7F67AF2, (int32_t)0x8040A900, (int32_t)0x810262AE, (int32_t)0x100AEB5E, (int32_t)0xF54A0B60, (int32_t)0x8072EC07, (int32_t)0x81CAE1C1, (int32_t)0x1558ADA5, 
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC, (int32_t)0xEFF514A2, (int32_t)0x810262AE, (int32_t)0x8405778C, (int32_t)0x1FD511FA, 
    (int32_t)0xED4D26BD, (int32_t)0x815F8632, (int32_t)0x85768E01, (int32_t)0x24FEFE72, (int32_t)0xEAA7525B, (int32_t)0x81CAE1C1, (int32_t)0x871EACD3, (int32_t)0x2A184D62, 
    (int32_t)0xE803E39B, (int32_t)0x8244694F, (int32_t)0x88FD1588, (int32_t)0x2F1EB491, (int32_t)0xE5632654, (int32_t)0x82CC0F36, (int32_t)0x8B10F144, (int32_t)0x340FF242, 
    (int32_t)0xE2C56610, (int32_t)0x8361C43A, (int32_t)0x8D59512A, (int32_t)0x38E9CE39, (int32_t)0xE02AEE06, (int32_t)0x8405778C, (int32_t)0x8FD52EC6, (int32_t)0x3DAA1AB9, 
    (int32_t)0xDD94090B, (int32_t)0x84B716C9, (int32_t)0x92836C86, (int32_t)0x424EB584, (int32_t)0xDB01018E, (int32_t)0x85768E01, (int32_t)0x9562D638, (int32_t)0x46D588C7, 
    (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0x4B3C8C12, (int32_t)0xD5E7B29E, (int32_t)0x871EACD3, (int32_t)0x9BAFEECB, (int32_t)0x4F81C53D, 
    (int32_t)0xD361FDBC, (int32_t)0x880724CE, (int32_t)0x9F1AC938, (int32_t)0x53A3494E, (int32_t)0xD0E14B6F, (int32_t)0x88FD1588, (int32_t)0xA2B127F1, (int32_t)0x579F3D54, 
    (int32_t)0xCE65E3A7, (int32_t)0x8A006364, (int32_t)0xA6716E81, (int32_t)0x5B73D73C, (int32_t)0xCBF00DBE, (int32_t)0x8B10F144, (int32_t)0xAA59EDA4, (int32_t)0x5F1F5EA1, 
    (int32_t)0xC980106E, (int32_t)0x8C2EA08D, (int32_t)0xAE68E405, (int32_t)0x62A02D8D, (int32_t)0xC71631C7, (int32_t)0x8D59512A, (int32_t)0xB29C7F07, (int32_t)0x65F4B13E, 
    (int32_t)0xC4B2B72C, (int32_t)0x8E90E191, (int32_t)0xB6F2DB9A, (int32_t)0x691B6AD3, (int32_t)0xC255E547, (int32_t)0x8FD52EC6, (int32_t)0xBB6A0710, (int32_t)0x6C12EFFE, 
};

/****************** stage 2 radix 4 ******************/
ALIGN(8) static const int32_t __fft300_tw2[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7FBF5700, (int32_t)0xF7F67AF2, (int32_t)0x7EFD9D52, (int32_t)0xEFF514A2, (int32_t)0x7DBB96B1, (int32_t)0xE803E39B, 
    (int32_t)0x7EFD9D52, (int32_t)0xEFF514A2, (int32_t)0x7BFA8874, (int32_t)0xE02AEE06, (int32_t)0x7702EA78, (int32_t)0xD0E14B6F, 
    (int32_t)0x7DBB96B1, (int32_t)0xE803E39B, (int32_t)0x7702EA78, (int32_t)0xD0E14B6F, (int32_t)0x6C12EFFE, (int32_t)0xBB6A0710, 
    (int32_t)0x7BFA8874, (int32_t)0xE02AEE06, (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, 
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x4B3C8C12, (int32_t)0x98722192, 
    (int32_t)0x7702EA78, (int32_t)0xD0E14B6F, (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x367FEF92, (int32_t)0x8C2EA08D, 
    (int32_t)0x73D15F73, (int32_t)0xC980106E, (int32_t)0x51971BFB, (int32_t)0x9D5FD273, (int32_t)0x1FD511FA, (int32_t)0x8405778C, 
    (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0x0809850E, (int32_t)0x8040A900, 
    (int32_t)0x6C12EFFE, (int32_t)0xBB6A0710, (int32_t)0x367FEF92, (int32_t)0x8C2EA08D, (int32_t)0xEFF514A2, (int32_t)0x810262AE, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x8643C7B3, 
    (int32_t)0x62A02D8D, (int32_t)0xAE68E405, (int32_t)0x17FC1C65, (int32_t)0x8244694F, (int32_t)0xC255E547, (int32_t)0x8FD52EC6, 
    (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0xAE68E405, (int32_t)0x9D5FD273, 
    (int32_t)0x579F3D54, (int32_t)0xA2B127F1, (int32_t)0xF7F67AF2, (int32_t)0x8040A900, (int32_t)0x9D5FD273, (int32_t)0xAE68E405, 
    (int32_t)0x51971BFB, (int32_t)0x9D5FD273, (int32_t)0xE803E39B, (int32_t)0x8244694F, (int32_t)0x8FD52EC6, (int32_t)0xC255E547, 
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x8643C7B3, (int32_t)0xD8722192, 
    (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0xC980106E, (int32_t)0x8C2EA08D, (int32_t)0x810262AE, (int32_t)0xEFF514A2, 
    (int32_t)0x3DAA1AB9, (int32_t)0x8FD52EC6, (int32_t)0xBB6A0710, (int32_t)0x93ED1002, (int32_t)0x8040A900, (int32_t)0x0809850E, 
    (int32_t)0x367FEF92, (int32_t)0x8C2EA08D, (int32_t)0xAE68E405, (int32_t)0x9D5FD273, (int32_t)0x8405778C, (int32_t)0x1FD511FA, 
    (int32_t)0x2F1EB491, (int32_t)0x88FD1588, (int32_t)0xA2B127F1, (int32_t)0xA860C2AC, (int32_t)0x8C2EA08D, (int32_t)0x367FEF92, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x4B3C8C12, 
    (int32_t)0x1FD511FA, (int32_t)0x8405778C, (int32_t)0x8FD52EC6, (int32_t)0xC255E547, (int32_t)0xA860C2AC, (int32_t)0x5D4ED80F, 
    (int32_t)0x17FC1C65, (int32_t)0x8244694F, (int32_t)0x88FD1588, (int32_t)0xD0E14B6F, (int32_t)0xBB6A0710, (int32_t)0x6C12EFFE, 
    (int32_t)0x100AEB5E, (int32_t)0x810262AE, (int32_t)0x8405778C, (int32_t)0xE02AEE06, (int32_t)0xD0E14B6F, (int32_t)0x7702EA78, 
    (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0x810262AE, (int32_t)0xEFF514A2, (int32_t)0xE803E39B, (int32_t)0x7DBB96B1, 
};

/****************** stage 3 radix 5 ******************/
ALIGN(8) static const int32_t __fft300_tw3[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7BFA8874, (int32_t)0xE02AEE06, (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x4495F8F0, (int32_t)0x93ED1002, 
    (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0xC980106E, (int32_t)0x8C2EA08D, 
    (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0xAE68E405, (int32_t)0x9D5FD273, (int32_t)0x810262AE, (int32_t)0xEFF514A2, 
    (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0xC980106E, (int32_t)0x8C2EA08D, (int32_t)0x810262AE, (int32_t)0xEFF514A2, (int32_t)0xAE68E405, (int32_t)0x62A02D8D, 
};
#define N 300
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT3_first_32x32,
    fft_stageS2_DFT4_32x32,
    fft_stageS2_DFT5_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT3_first_32x32,
    fft_stageS3_DFT4_32x32,
    fft_stageS3_DFT5_32x32,
    fft_stageS3_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT3_first_32x32,
    fft_stageS2_DFT4_32x32,
    fft_stageS2_DFT5_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT3_first_32x32,
    fft_stageS3_DFT4_32x32,
    fft_stageS3_DFT5_32x32,
    fft_stageS3_DFT5_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 1, 
}; 
static const cint32ptr_t tw_tab[] = 
{
    __fft300_tw1, __fft300_tw2, __fft300_tw3, NULL
};
const fft_cplx32x32_descr_t __cfft_descr300_32x32 =
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_cplx32x32_descr_t __cifft_descr300_32x32 =
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_300 = (const fft_handle_t)&__cfft_descr300_32x32;
const fft_handle_t cinfft32_300 = (const fft_handle_t)&__cifft_descr300_32x32;
