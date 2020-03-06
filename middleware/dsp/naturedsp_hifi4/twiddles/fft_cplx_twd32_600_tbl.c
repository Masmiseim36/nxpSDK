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
ALIGN(8) static const int32_t __fft600_tw1[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFE340C, (int32_t)0xFEA8DC4F, (int32_t)0x7FF8D03D, (int32_t)0xFD51C240, 
    (int32_t)0x7FF8D03D, (int32_t)0xFD51C240, (int32_t)0x7FE341C4, (int32_t)0xFAA3D18E, (int32_t)0x7FEFD4BA, (int32_t)0xFBFABB75, (int32_t)0x7FBF5700, (int32_t)0xF7F67AF2, 
    (int32_t)0x7FE341C4, (int32_t)0xFAA3D18E, (int32_t)0x7F8D13F9, (int32_t)0xF54A0B60, (int32_t)0x7FD317B4, (int32_t)0xF94D0E2E, (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, 
    (int32_t)0x7FBF5700, (int32_t)0xF7F67AF2, (int32_t)0x7EFD9D52, (int32_t)0xEFF514A2, (int32_t)0x7FA80034, (int32_t)0xF6A02179, (int32_t)0x7EA079CE, (int32_t)0xED4D26BD, 
    (int32_t)0x7F8D13F9, (int32_t)0xF54A0B60, (int32_t)0x7E351E3F, (int32_t)0xEAA7525B, (int32_t)0x7F6E9310, (int32_t)0xF3F44240, (int32_t)0x7DBB96B1, (int32_t)0xE803E39B, 
    (int32_t)0x7F4C7E54, (int32_t)0xF29ECFB2, (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x7F26D6BA, (int32_t)0xF149BD4D, (int32_t)0x7C9E3BC6, (int32_t)0xE2C56610, 
    (int32_t)0x7EFD9D52, (int32_t)0xEFF514A2, (int32_t)0x7BFA8874, (int32_t)0xE02AEE06, (int32_t)0x7ED0D343, (int32_t)0xEEA0DF43, (int32_t)0x7B48E937, (int32_t)0xDD94090B, 
    (int32_t)0x7EA079CE, (int32_t)0xED4D26BD, (int32_t)0x7A8971FF, (int32_t)0xDB01018E, (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x79BC384D, (int32_t)0xD8722192, 
    (int32_t)0x7E351E3F, (int32_t)0xEAA7525B, (int32_t)0x78E1532D, (int32_t)0xD5E7B29E, (int32_t)0x7DFA1F27, (int32_t)0xE9554988, (int32_t)0x77F8DB32, (int32_t)0xD361FDBC, 
    (int32_t)0x7DBB96B1, (int32_t)0xE803E39B, (int32_t)0x7702EA78, (int32_t)0xD0E14B6F, (int32_t)0x7D79869E, (int32_t)0xE6B32A0E, (int32_t)0x75FF9C9C, (int32_t)0xCE65E3A7, 
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654, (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x7CEAD728, (int32_t)0xE413E1DC, (int32_t)0x73D15F73, (int32_t)0xC980106E, 
    (int32_t)0x7C9E3BC6, (int32_t)0xE2C56610, (int32_t)0x72A6AED6, (int32_t)0xC71631C7, (int32_t)0x7C4E20CA, (int32_t)0xE177BC54, (int32_t)0x716F1E6F, (int32_t)0xC4B2B72C, 
    (int32_t)0x7BFA8874, (int32_t)0xE02AEE06, (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, 
    (int32_t)0x7B48E937, (int32_t)0xDD94090B, (int32_t)0x6D7C937A, (int32_t)0xBDB14A7C, (int32_t)0x7AEAE74B, (int32_t)0xDC4A04FA, (int32_t)0x6C12EFFE, (int32_t)0xBB6A0710, 
    (int32_t)0x7A8971FF, (int32_t)0xDB01018E, (int32_t)0x6A9D29C8, (int32_t)0xB92A7739, (int32_t)0x7A248C0E, (int32_t)0xD9B90805, (int32_t)0x691B6AD3, (int32_t)0xB6F2DB9A, 
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x795079AB, (int32_t)0xD72C5763, (int32_t)0x65F4B13E, (int32_t)0xB29C7F07, 
    (int32_t)0x78E1532D, (int32_t)0xD5E7B29E, (int32_t)0x64501135, (int32_t)0xB07E3AC3, (int32_t)0x786EC7F2, (int32_t)0xD4A43C5F, (int32_t)0x62A02D8D, (int32_t)0xAE68E405, 
    (int32_t)0x77F8DB32, (int32_t)0xD361FDBC, (int32_t)0x60E536C8, (int32_t)0xAC5CB6B2, (int32_t)0x777F903C, (int32_t)0xD220FFC0, (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, 
    (int32_t)0x7702EA78, (int32_t)0xD0E14B6F, (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x7682ED65, (int32_t)0xCFA2E9C1, (int32_t)0x5B73D73C, (int32_t)0xA6716E81, 
    (int32_t)0x75FF9C9C, (int32_t)0xCE65E3A7, (int32_t)0x598E917F, (int32_t)0xA48C28C4, (int32_t)0x7578FBCC, (int32_t)0xCD2A4207, (int32_t)0x579F3D54, (int32_t)0xA2B127F1, 
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0x7461D94C, (int32_t)0xCAB74F9E, (int32_t)0x53A3494E, (int32_t)0x9F1AC938, 
    (int32_t)0x73D15F73, (int32_t)0xC980106E, (int32_t)0x51971BFB, (int32_t)0x9D5FD273, (int32_t)0x733DA540, (int32_t)0xC84A58EB, (int32_t)0x4F81C53D, (int32_t)0x9BAFEECB, 
    (int32_t)0x72A6AED6, (int32_t)0xC71631C7, (int32_t)0x4D6380F9, (int32_t)0x9A0B4EC2, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x4B3C8C12, (int32_t)0x98722192, 
    (int32_t)0x716F1E6F, (int32_t)0xC4B2B72C, (int32_t)0x490D2466, (int32_t)0x96E4952D, (int32_t)0x70CE8D30, (int32_t)0xC38374E0, (int32_t)0x46D588C7, (int32_t)0x9562D638, 
    (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0x6F83EF25, (int32_t)0xC12A10D8, (int32_t)0x424EB584, (int32_t)0x92836C86, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x6E2CCB74, (int32_t)0xBED7BB1C, (int32_t)0x3DAA1AB9, (int32_t)0x8FD52EC6, 
    (int32_t)0x6D7C937A, (int32_t)0xBDB14A7C, (int32_t)0x3B4D48D4, (int32_t)0x8E90E191, (int32_t)0x6CC948A5, (int32_t)0xBC8CB666, (int32_t)0x38E9CE39, (int32_t)0x8D59512A, 
    (int32_t)0x6C12EFFE, (int32_t)0xBB6A0710, (int32_t)0x367FEF92, (int32_t)0x8C2EA08D, (int32_t)0x6B598EA3, (int32_t)0xBA4944A2, (int32_t)0x340FF242, (int32_t)0x8B10F144, 
    (int32_t)0x6A9D29C8, (int32_t)0xB92A7739, (int32_t)0x319A1C59, (int32_t)0x8A006364, (int32_t)0x69DDC6B9, (int32_t)0xB80DA6E1, (int32_t)0x2F1EB491, (int32_t)0x88FD1588, 
    (int32_t)0x691B6AD3, (int32_t)0xB6F2DB9A, (int32_t)0x2C9E0244, (int32_t)0x880724CE, (int32_t)0x68561B8C, (int32_t)0xB5DA1D53, (int32_t)0x2A184D62, (int32_t)0x871EACD3, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x66C2B918, (int32_t)0xB3AEE73E, (int32_t)0x24FEFE72, (int32_t)0x85768E01, 
    (int32_t)0x65F4B13E, (int32_t)0xB29C7F07, (int32_t)0x226BF6F5, (int32_t)0x84B716C9, (int32_t)0x6523CCA8, (int32_t)0xB18C42FD, (int32_t)0x1FD511FA, (int32_t)0x8405778C, 
    (int32_t)0x64501135, (int32_t)0xB07E3AC3, (int32_t)0x1D3A99F0, (int32_t)0x8361C43A, (int32_t)0x637984D4, (int32_t)0xAF726DEF, (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, 
    (int32_t)0x62A02D8D, (int32_t)0xAE68E405, (int32_t)0x17FC1C65, (int32_t)0x8244694F, (int32_t)0x61C4117A, (int32_t)0xAD61A47A, (int32_t)0x1558ADA5, (int32_t)0x81CAE1C1, 
    (int32_t)0x60E536C8, (int32_t)0xAC5CB6B2, (int32_t)0x12B2D943, (int32_t)0x815F8632, (int32_t)0x6003A3B8, (int32_t)0xAB5A21FF, (int32_t)0x100AEB5E, (int32_t)0x810262AE, 
    (int32_t)0x5F1F5EA1, (int32_t)0xAA59EDA4, (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x5E386DEA, (int32_t)0xA95C20D3, (int32_t)0x0AB5F4A0, (int32_t)0x8072EC07, 
    (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0x5C62A3A0, (int32_t)0xA767DA3C, (int32_t)0x055C2E72, (int32_t)0x801CBE3C, 
    (int32_t)0x5B73D73C, (int32_t)0xA6716E81, (int32_t)0x02AE3DC0, (int32_t)0x80072FC3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, 
    (int32_t)0x598E917F, (int32_t)0xA48C28C4, (int32_t)0xFD51C240, (int32_t)0x80072FC3, (int32_t)0x589825C4, (int32_t)0xA39D5C60, (int32_t)0xFAA3D18E, (int32_t)0x801CBE3C, 
    (int32_t)0x579F3D54, (int32_t)0xA2B127F1, (int32_t)0xF7F67AF2, (int32_t)0x8040A900, (int32_t)0x56A3DF2D, (int32_t)0xA1C79216, (int32_t)0xF54A0B60, (int32_t)0x8072EC07, 
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x54A5DE01, (int32_t)0x9FFC5C48, (int32_t)0xEFF514A2, (int32_t)0x810262AE, 
    (int32_t)0x53A3494E, (int32_t)0x9F1AC938, (int32_t)0xED4D26BD, (int32_t)0x815F8632, (int32_t)0x529E5B86, (int32_t)0x9E3BEE86, (int32_t)0xEAA7525B, (int32_t)0x81CAE1C1, 
    (int32_t)0x51971BFB, (int32_t)0x9D5FD273, (int32_t)0xE803E39B, (int32_t)0x8244694F, (int32_t)0x508D9211, (int32_t)0x9C867B2C, (int32_t)0xE5632654, (int32_t)0x82CC0F36, 
    (int32_t)0x4F81C53D, (int32_t)0x9BAFEECB, (int32_t)0xE2C56610, (int32_t)0x8361C43A, (int32_t)0x4E73BD03, (int32_t)0x9ADC3358, (int32_t)0xE02AEE06, (int32_t)0x8405778C, 
    (int32_t)0x4D6380F9, (int32_t)0x9A0B4EC2, (int32_t)0xDD94090B, (int32_t)0x84B716C9, (int32_t)0x4C5118C2, (int32_t)0x993D46E8, (int32_t)0xDB01018E, (int32_t)0x85768E01, 
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x4A25E2AD, (int32_t)0x97A9E474, (int32_t)0xD5E7B29E, (int32_t)0x871EACD3, 
    (int32_t)0x490D2466, (int32_t)0x96E4952D, (int32_t)0xD361FDBC, (int32_t)0x880724CE, (int32_t)0x47F2591F, (int32_t)0x96223947, (int32_t)0xD0E14B6F, (int32_t)0x88FD1588, 
    (int32_t)0x46D588C7, (int32_t)0x9562D638, (int32_t)0xCE65E3A7, (int32_t)0x8A006364, (int32_t)0x45B6BB5E, (int32_t)0x94A6715D, (int32_t)0xCBF00DBE, (int32_t)0x8B10F144, 
    (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0xC980106E, (int32_t)0x8C2EA08D, (int32_t)0x4373499A, (int32_t)0x9336B75B, (int32_t)0xC71631C7, (int32_t)0x8D59512A, 
    (int32_t)0x424EB584, (int32_t)0x92836C86, (int32_t)0xC4B2B72C, (int32_t)0x8E90E191, (int32_t)0x412844E4, (int32_t)0x91D3348C, (int32_t)0xC255E547, (int32_t)0x8FD52EC6, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x3ED5EF28, (int32_t)0x907C10DB, (int32_t)0xBDB14A7C, (int32_t)0x92836C86, 
    (int32_t)0x3DAA1AB9, (int32_t)0x8FD52EC6, (int32_t)0xBB6A0710, (int32_t)0x93ED1002, (int32_t)0x3C7C8B20, (int32_t)0x8F3172D0, (int32_t)0xB92A7739, (int32_t)0x9562D638, 
    (int32_t)0x3B4D48D4, (int32_t)0x8E90E191, (int32_t)0xB6F2DB9A, (int32_t)0x96E4952D, (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0xB4C373EE, (int32_t)0x98722192, 
    (int32_t)0x38E9CE39, (int32_t)0x8D59512A, (int32_t)0xB29C7F07, (int32_t)0x9A0B4EC2, (int32_t)0x37B5A715, (int32_t)0x8CC25AC0, (int32_t)0xB07E3AC3, (int32_t)0x9BAFEECB, 
    (int32_t)0x367FEF92, (int32_t)0x8C2EA08D, (int32_t)0xAE68E405, (int32_t)0x9D5FD273, (int32_t)0x3548B062, (int32_t)0x8B9E26B4, (int32_t)0xAC5CB6B2, (int32_t)0x9F1AC938, 
    (int32_t)0x340FF242, (int32_t)0x8B10F144, (int32_t)0xAA59EDA4, (int32_t)0xA0E0A15F, (int32_t)0x32D5BDF9, (int32_t)0x8A870434, (int32_t)0xA860C2AC, (int32_t)0xA2B127F1, 
    (int32_t)0x319A1C59, (int32_t)0x8A006364, (int32_t)0xA6716E81, (int32_t)0xA48C28C4, (int32_t)0x305D163F, (int32_t)0x897D129B, (int32_t)0xA48C28C4, (int32_t)0xA6716E81, 
    (int32_t)0x2F1EB491, (int32_t)0x88FD1588, (int32_t)0xA2B127F1, (int32_t)0xA860C2AC, (int32_t)0x2DDF0040, (int32_t)0x88806FC4, (int32_t)0xA0E0A15F, (int32_t)0xAA59EDA4, 
    (int32_t)0x2C9E0244, (int32_t)0x880724CE, (int32_t)0x9F1AC938, (int32_t)0xAC5CB6B2, (int32_t)0x2B5BC3A1, (int32_t)0x8791380E, (int32_t)0x9D5FD273, (int32_t)0xAE68E405, 
    (int32_t)0x2A184D62, (int32_t)0x871EACD3, (int32_t)0x9BAFEECB, (int32_t)0xB07E3AC3, (int32_t)0x28D3A89D, (int32_t)0x86AF8655, (int32_t)0x9A0B4EC2, (int32_t)0xB29C7F07, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, (int32_t)0x2646F7FB, (int32_t)0x85DB73F2, (int32_t)0x96E4952D, (int32_t)0xB6F2DB9A, 
    (int32_t)0x24FEFE72, (int32_t)0x85768E01, (int32_t)0x9562D638, (int32_t)0xB92A7739, (int32_t)0x23B5FB06, (int32_t)0x851518B5, (int32_t)0x93ED1002, (int32_t)0xBB6A0710, 
    (int32_t)0x226BF6F5, (int32_t)0x84B716C9, (int32_t)0x92836C86, (int32_t)0xBDB14A7C, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000, 
    (int32_t)0x1FD511FA, (int32_t)0x8405778C, (int32_t)0x8FD52EC6, (int32_t)0xC255E547, (int32_t)0x1E8843AC, (int32_t)0x83B1DF36, (int32_t)0x8E90E191, (int32_t)0xC4B2B72C, 
    (int32_t)0x1D3A99F0, (int32_t)0x8361C43A, (int32_t)0x8D59512A, (int32_t)0xC71631C7, (int32_t)0x1BEC1E24, (int32_t)0x831528D8, (int32_t)0x8C2EA08D, (int32_t)0xC980106E, 
    (int32_t)0x1A9CD9AC, (int32_t)0x82CC0F36, (int32_t)0x8B10F144, (int32_t)0xCBF00DBE, (int32_t)0x194CD5F2, (int32_t)0x82867962, (int32_t)0x8A006364, (int32_t)0xCE65E3A7, 
    (int32_t)0x17FC1C65, (int32_t)0x8244694F, (int32_t)0x88FD1588, (int32_t)0xD0E14B6F, (int32_t)0x16AAB678, (int32_t)0x8205E0D9, (int32_t)0x880724CE, (int32_t)0xD361FDBC, 
    (int32_t)0x1558ADA5, (int32_t)0x81CAE1C1, (int32_t)0x871EACD3, (int32_t)0xD5E7B29E, (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x8643C7B3, (int32_t)0xD8722192, 
    (int32_t)0x12B2D943, (int32_t)0x815F8632, (int32_t)0x85768E01, (int32_t)0xDB01018E, (int32_t)0x115F20BD, (int32_t)0x812F2CBD, (int32_t)0x84B716C9, (int32_t)0xDD94090B, 
    (int32_t)0x100AEB5E, (int32_t)0x810262AE, (int32_t)0x8405778C, (int32_t)0xE02AEE06, (int32_t)0x0EB642B3, (int32_t)0x80D92946, (int32_t)0x8361C43A, (int32_t)0xE2C56610, 
    (int32_t)0x0D61304E, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0xE5632654, (int32_t)0x0C0BBDC0, (int32_t)0x80916CF0, (int32_t)0x8244694F, (int32_t)0xE803E39B, 
    (int32_t)0x0AB5F4A0, (int32_t)0x8072EC07, (int32_t)0x81CAE1C1, (int32_t)0xEAA7525B, (int32_t)0x095FDE87, (int32_t)0x8057FFCC, (int32_t)0x815F8632, (int32_t)0xED4D26BD, 
    (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0x810262AE, (int32_t)0xEFF514A2, (int32_t)0x06B2F1D2, (int32_t)0x802CE84C, (int32_t)0x80B381AC, (int32_t)0xF29ECFB2, 
    (int32_t)0x055C2E72, (int32_t)0x801CBE3C, (int32_t)0x8072EC07, (int32_t)0xF54A0B60, (int32_t)0x0405448B, (int32_t)0x80102B46, (int32_t)0x8040A900, (int32_t)0xF7F67AF2, 
    (int32_t)0x02AE3DC0, (int32_t)0x80072FC3, (int32_t)0x801CBE3C, (int32_t)0xFAA3D18E, (int32_t)0x015723B1, (int32_t)0x8001CBF4, (int32_t)0x80072FC3, (int32_t)0xFD51C240, 
    (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x80000000, (int32_t)0x00000000, (int32_t)0xFEA8DC4F, (int32_t)0x8001CBF4, (int32_t)0x80072FC3, (int32_t)0x02AE3DC0, 
    (int32_t)0xFD51C240, (int32_t)0x80072FC3, (int32_t)0x801CBE3C, (int32_t)0x055C2E72, (int32_t)0xFBFABB75, (int32_t)0x80102B46, (int32_t)0x8040A900, (int32_t)0x0809850E, 
    (int32_t)0xFAA3D18E, (int32_t)0x801CBE3C, (int32_t)0x8072EC07, (int32_t)0x0AB5F4A0, (int32_t)0xF94D0E2E, (int32_t)0x802CE84C, (int32_t)0x80B381AC, (int32_t)0x0D61304E, 
    (int32_t)0xF7F67AF2, (int32_t)0x8040A900, (int32_t)0x810262AE, (int32_t)0x100AEB5E, (int32_t)0xF6A02179, (int32_t)0x8057FFCC, (int32_t)0x815F8632, (int32_t)0x12B2D943, 
    (int32_t)0xF54A0B60, (int32_t)0x8072EC07, (int32_t)0x81CAE1C1, (int32_t)0x1558ADA5, (int32_t)0xF3F44240, (int32_t)0x80916CF0, (int32_t)0x8244694F, (int32_t)0x17FC1C65, 
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC, (int32_t)0xF149BD4D, (int32_t)0x80D92946, (int32_t)0x8361C43A, (int32_t)0x1D3A99F0, 
    (int32_t)0xEFF514A2, (int32_t)0x810262AE, (int32_t)0x8405778C, (int32_t)0x1FD511FA, (int32_t)0xEEA0DF43, (int32_t)0x812F2CBD, (int32_t)0x84B716C9, (int32_t)0x226BF6F5, 
    (int32_t)0xED4D26BD, (int32_t)0x815F8632, (int32_t)0x85768E01, (int32_t)0x24FEFE72, (int32_t)0xEBF9F498, (int32_t)0x81936DAF, (int32_t)0x8643C7B3, (int32_t)0x278DDE6E, 
    (int32_t)0xEAA7525B, (int32_t)0x81CAE1C1, (int32_t)0x871EACD3, (int32_t)0x2A184D62, (int32_t)0xE9554988, (int32_t)0x8205E0D9, (int32_t)0x880724CE, (int32_t)0x2C9E0244, 
    (int32_t)0xE803E39B, (int32_t)0x8244694F, (int32_t)0x88FD1588, (int32_t)0x2F1EB491, (int32_t)0xE6B32A0E, (int32_t)0x82867962, (int32_t)0x8A006364, (int32_t)0x319A1C59, 
    (int32_t)0xE5632654, (int32_t)0x82CC0F36, (int32_t)0x8B10F144, (int32_t)0x340FF242, (int32_t)0xE413E1DC, (int32_t)0x831528D8, (int32_t)0x8C2EA08D, (int32_t)0x367FEF92, 
    (int32_t)0xE2C56610, (int32_t)0x8361C43A, (int32_t)0x8D59512A, (int32_t)0x38E9CE39, (int32_t)0xE177BC54, (int32_t)0x83B1DF36, (int32_t)0x8E90E191, (int32_t)0x3B4D48D4, 
    (int32_t)0xE02AEE06, (int32_t)0x8405778C, (int32_t)0x8FD52EC6, (int32_t)0x3DAA1AB9, (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0x40000000, 
    (int32_t)0xDD94090B, (int32_t)0x84B716C9, (int32_t)0x92836C86, (int32_t)0x424EB584, (int32_t)0xDC4A04FA, (int32_t)0x851518B5, (int32_t)0x93ED1002, (int32_t)0x4495F8F0, 
    (int32_t)0xDB01018E, (int32_t)0x85768E01, (int32_t)0x9562D638, (int32_t)0x46D588C7, (int32_t)0xD9B90805, (int32_t)0x85DB73F2, (int32_t)0x96E4952D, (int32_t)0x490D2466, 
    (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0x4B3C8C12, (int32_t)0xD72C5763, (int32_t)0x86AF8655, (int32_t)0x9A0B4EC2, (int32_t)0x4D6380F9, 
    (int32_t)0xD5E7B29E, (int32_t)0x871EACD3, (int32_t)0x9BAFEECB, (int32_t)0x4F81C53D, (int32_t)0xD4A43C5F, (int32_t)0x8791380E, (int32_t)0x9D5FD273, (int32_t)0x51971BFB, 
    (int32_t)0xD361FDBC, (int32_t)0x880724CE, (int32_t)0x9F1AC938, (int32_t)0x53A3494E, (int32_t)0xD220FFC0, (int32_t)0x88806FC4, (int32_t)0xA0E0A15F, (int32_t)0x55A6125C, 
    (int32_t)0xD0E14B6F, (int32_t)0x88FD1588, (int32_t)0xA2B127F1, (int32_t)0x579F3D54, (int32_t)0xCFA2E9C1, (int32_t)0x897D129B, (int32_t)0xA48C28C4, (int32_t)0x598E917F, 
    (int32_t)0xCE65E3A7, (int32_t)0x8A006364, (int32_t)0xA6716E81, (int32_t)0x5B73D73C, (int32_t)0xCD2A4207, (int32_t)0x8A870434, (int32_t)0xA860C2AC, (int32_t)0x5D4ED80F, 
    (int32_t)0xCBF00DBE, (int32_t)0x8B10F144, (int32_t)0xAA59EDA4, (int32_t)0x5F1F5EA1, (int32_t)0xCAB74F9E, (int32_t)0x8B9E26B4, (int32_t)0xAC5CB6B2, (int32_t)0x60E536C8, 
    (int32_t)0xC980106E, (int32_t)0x8C2EA08D, (int32_t)0xAE68E405, (int32_t)0x62A02D8D, (int32_t)0xC84A58EB, (int32_t)0x8CC25AC0, (int32_t)0xB07E3AC3, (int32_t)0x64501135, 
    (int32_t)0xC71631C7, (int32_t)0x8D59512A, (int32_t)0xB29C7F07, (int32_t)0x65F4B13E, (int32_t)0xC5E3A3A9, (int32_t)0x8DF37F8B, (int32_t)0xB4C373EE, (int32_t)0x678DDE6E, 
    (int32_t)0xC4B2B72C, (int32_t)0x8E90E191, (int32_t)0xB6F2DB9A, (int32_t)0x691B6AD3, (int32_t)0xC38374E0, (int32_t)0x8F3172D0, (int32_t)0xB92A7739, (int32_t)0x6A9D29C8, 
    (int32_t)0xC255E547, (int32_t)0x8FD52EC6, (int32_t)0xBB6A0710, (int32_t)0x6C12EFFE, (int32_t)0xC12A10D8, (int32_t)0x907C10DB, (int32_t)0xBDB14A7C, (int32_t)0x6D7C937A, 
};

/****************** stage 2 radix 5 ******************/
ALIGN(8) static const int32_t __fft600_tw2[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7FEFD4BA, (int32_t)0xFBFABB75, (int32_t)0x7FBF5700, (int32_t)0xF7F67AF2, (int32_t)0x7F6E9310, (int32_t)0xF3F44240, (int32_t)0x7EFD9D52, (int32_t)0xEFF514A2, 
    (int32_t)0x7FBF5700, (int32_t)0xF7F67AF2, (int32_t)0x7EFD9D52, (int32_t)0xEFF514A2, (int32_t)0x7DBB96B1, (int32_t)0xE803E39B, (int32_t)0x7BFA8874, (int32_t)0xE02AEE06, 
    (int32_t)0x7F6E9310, (int32_t)0xF3F44240, (int32_t)0x7DBB96B1, (int32_t)0xE803E39B, (int32_t)0x7AEAE74B, (int32_t)0xDC4A04FA, (int32_t)0x7702EA78, (int32_t)0xD0E14B6F, 
    (int32_t)0x7EFD9D52, (int32_t)0xEFF514A2, (int32_t)0x7BFA8874, (int32_t)0xE02AEE06, (int32_t)0x7702EA78, (int32_t)0xD0E14B6F, (int32_t)0x702AD13A, (int32_t)0xC255E547, 
    (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, 
    (int32_t)0x7DBB96B1, (int32_t)0xE803E39B, (int32_t)0x7702EA78, (int32_t)0xD0E14B6F, (int32_t)0x6C12EFFE, (int32_t)0xBB6A0710, (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, 
    (int32_t)0x7CEAD728, (int32_t)0xE413E1DC, (int32_t)0x73D15F73, (int32_t)0xC980106E, (int32_t)0x6523CCA8, (int32_t)0xB18C42FD, (int32_t)0x51971BFB, (int32_t)0x9D5FD273, 
    (int32_t)0x7BFA8874, (int32_t)0xE02AEE06, (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x4495F8F0, (int32_t)0x93ED1002, 
    (int32_t)0x7AEAE74B, (int32_t)0xDC4A04FA, (int32_t)0x6C12EFFE, (int32_t)0xBB6A0710, (int32_t)0x54A5DE01, (int32_t)0x9FFC5C48, (int32_t)0x367FEF92, (int32_t)0x8C2EA08D, 
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, 
    (int32_t)0x786EC7F2, (int32_t)0xD4A43C5F, (int32_t)0x62A02D8D, (int32_t)0xAE68E405, (int32_t)0x412844E4, (int32_t)0x91D3348C, (int32_t)0x17FC1C65, (int32_t)0x8244694F, 
    (int32_t)0x7702EA78, (int32_t)0xD0E14B6F, (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x367FEF92, (int32_t)0x8C2EA08D, (int32_t)0x0809850E, (int32_t)0x8040A900, 
    (int32_t)0x7578FBCC, (int32_t)0xCD2A4207, (int32_t)0x579F3D54, (int32_t)0xA2B127F1, (int32_t)0x2B5BC3A1, (int32_t)0x8791380E, (int32_t)0xF7F67AF2, (int32_t)0x8040A900, 
    (int32_t)0x73D15F73, (int32_t)0xC980106E, (int32_t)0x51971BFB, (int32_t)0x9D5FD273, (int32_t)0x1FD511FA, (int32_t)0x8405778C, (int32_t)0xE803E39B, (int32_t)0x8244694F, 
    (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0xD8722192, (int32_t)0x8643C7B3, 
    (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0xC980106E, (int32_t)0x8C2EA08D, 
    (int32_t)0x6E2CCB74, (int32_t)0xBED7BB1C, (int32_t)0x3DAA1AB9, (int32_t)0x8FD52EC6, (int32_t)0xFBFABB75, (int32_t)0x80102B46, (int32_t)0xBB6A0710, (int32_t)0x93ED1002, 
    (int32_t)0x6C12EFFE, (int32_t)0xBB6A0710, (int32_t)0x367FEF92, (int32_t)0x8C2EA08D, (int32_t)0xEFF514A2, (int32_t)0x810262AE, (int32_t)0xAE68E405, (int32_t)0x9D5FD273, 
    (int32_t)0x69DDC6B9, (int32_t)0xB80DA6E1, (int32_t)0x2F1EB491, (int32_t)0x88FD1588, (int32_t)0xE413E1DC, (int32_t)0x831528D8, (int32_t)0xA2B127F1, (int32_t)0xA860C2AC, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, 
    (int32_t)0x6523CCA8, (int32_t)0xB18C42FD, (int32_t)0x1FD511FA, (int32_t)0x8405778C, (int32_t)0xCD2A4207, (int32_t)0x8A870434, (int32_t)0x8FD52EC6, (int32_t)0xC255E547, 
    (int32_t)0x62A02D8D, (int32_t)0xAE68E405, (int32_t)0x17FC1C65, (int32_t)0x8244694F, (int32_t)0xC255E547, (int32_t)0x8FD52EC6, (int32_t)0x88FD1588, (int32_t)0xD0E14B6F, 
    (int32_t)0x6003A3B8, (int32_t)0xAB5A21FF, (int32_t)0x100AEB5E, (int32_t)0x810262AE, (int32_t)0xB80DA6E1, (int32_t)0x96223947, (int32_t)0x8405778C, (int32_t)0xE02AEE06, 
    (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0xAE68E405, (int32_t)0x9D5FD273, (int32_t)0x810262AE, (int32_t)0xEFF514A2, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x80000000, (int32_t)0x00000000, 
    (int32_t)0x579F3D54, (int32_t)0xA2B127F1, (int32_t)0xF7F67AF2, (int32_t)0x8040A900, (int32_t)0x9D5FD273, (int32_t)0xAE68E405, (int32_t)0x810262AE, (int32_t)0x100AEB5E, 
    (int32_t)0x54A5DE01, (int32_t)0x9FFC5C48, (int32_t)0xEFF514A2, (int32_t)0x810262AE, (int32_t)0x96223947, (int32_t)0xB80DA6E1, (int32_t)0x8405778C, (int32_t)0x1FD511FA, 
    (int32_t)0x51971BFB, (int32_t)0x9D5FD273, (int32_t)0xE803E39B, (int32_t)0x8244694F, (int32_t)0x8FD52EC6, (int32_t)0xC255E547, (int32_t)0x88FD1588, (int32_t)0x2F1EB491, 
    (int32_t)0x4E73BD03, (int32_t)0x9ADC3358, (int32_t)0xE02AEE06, (int32_t)0x8405778C, (int32_t)0x8A870434, (int32_t)0xCD2A4207, (int32_t)0x8FD52EC6, (int32_t)0x3DAA1AB9, 
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x98722192, (int32_t)0x4B3C8C12, 
    (int32_t)0x47F2591F, (int32_t)0x96223947, (int32_t)0xD0E14B6F, (int32_t)0x88FD1588, (int32_t)0x831528D8, (int32_t)0xE413E1DC, (int32_t)0xA2B127F1, (int32_t)0x579F3D54, 
    (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0xC980106E, (int32_t)0x8C2EA08D, (int32_t)0x810262AE, (int32_t)0xEFF514A2, (int32_t)0xAE68E405, (int32_t)0x62A02D8D, 
    (int32_t)0x412844E4, (int32_t)0x91D3348C, (int32_t)0xC255E547, (int32_t)0x8FD52EC6, (int32_t)0x80102B46, (int32_t)0xFBFABB75, (int32_t)0xBB6A0710, (int32_t)0x6C12EFFE, 
    (int32_t)0x3DAA1AB9, (int32_t)0x8FD52EC6, (int32_t)0xBB6A0710, (int32_t)0x93ED1002, (int32_t)0x8040A900, (int32_t)0x0809850E, (int32_t)0xC980106E, (int32_t)0x73D15F73, 
    (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x81936DAF, (int32_t)0x14060B68, (int32_t)0xD8722192, (int32_t)0x79BC384D, 
    (int32_t)0x367FEF92, (int32_t)0x8C2EA08D, (int32_t)0xAE68E405, (int32_t)0x9D5FD273, (int32_t)0x8405778C, (int32_t)0x1FD511FA, (int32_t)0xE803E39B, (int32_t)0x7DBB96B1, 
    (int32_t)0x32D5BDF9, (int32_t)0x8A870434, (int32_t)0xA860C2AC, (int32_t)0xA2B127F1, (int32_t)0x8791380E, (int32_t)0x2B5BC3A1, (int32_t)0xF7F67AF2, (int32_t)0x7FBF5700, 
    (int32_t)0x2F1EB491, (int32_t)0x88FD1588, (int32_t)0xA2B127F1, (int32_t)0xA860C2AC, (int32_t)0x8C2EA08D, (int32_t)0x367FEF92, (int32_t)0x0809850E, (int32_t)0x7FBF5700, 
    (int32_t)0x2B5BC3A1, (int32_t)0x8791380E, (int32_t)0x9D5FD273, (int32_t)0xAE68E405, (int32_t)0x91D3348C, (int32_t)0x412844E4, (int32_t)0x17FC1C65, (int32_t)0x7DBB96B1, 
};

/****************** stage 3 radix 5 ******************/
ALIGN(8) static const int32_t __fft600_tw3[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, 
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, 
    (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0xD8722192, (int32_t)0x8643C7B3, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x80000000, (int32_t)0x00000000, 
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x98722192, (int32_t)0x4B3C8C12, 
    (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x81936DAF, (int32_t)0x14060B68, (int32_t)0xD8722192, (int32_t)0x79BC384D, 
};
#define N 600
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT3_first_32x32,
    fft_stageS2_DFT5_32x32,
    fft_stageS2_DFT5_32x32,
    fft_stageS2_DFT8_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT3_first_32x32,
    fft_stageS3_DFT5_32x32,
    fft_stageS3_DFT5_32x32,
    fft_stageS3_DFT8_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT3_first_32x32,
    fft_stageS2_DFT5_32x32,
    fft_stageS2_DFT5_32x32,
    fft_stageS2_DFT8_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT3_first_32x32,
    fft_stageS3_DFT5_32x32,
    fft_stageS3_DFT5_32x32,
    fft_stageS3_DFT8_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 1, 
}; 
static const cint32ptr_t tw_tab[] = 
{
    __fft600_tw1, __fft600_tw2, __fft600_tw3, NULL
};
static const fft_cplx32x32_descr_t __cfft_descr600_32x32 = 
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
};     
static const fft_cplx32x32_descr_t __cifft_descr600_32x32 = 
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_600  = (const fft_handle_t)&__cfft_descr600_32x32;
const fft_handle_t cinfft32_600 = (const fft_handle_t)&__cifft_descr600_32x32;
