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
ALIGN(8) static const int32_t __fft100_tw1[] = 
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

/****************** stage 2 radix 5 ******************/
ALIGN(8) static const int32_t __fft100_tw2[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7BFA8874, (int32_t)0xE02AEE06, (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x4495F8F0, (int32_t)0x93ED1002, 
    (int32_t)0x702AD13A, (int32_t)0xC255E547, (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0xC980106E, (int32_t)0x8C2EA08D, 
    (int32_t)0x5D4ED80F, (int32_t)0xA860C2AC, (int32_t)0x0809850E, (int32_t)0x8040A900, (int32_t)0xAE68E405, (int32_t)0x9D5FD273, (int32_t)0x810262AE, (int32_t)0xEFF514A2, 
    (int32_t)0x4495F8F0, (int32_t)0x93ED1002, (int32_t)0xC980106E, (int32_t)0x8C2EA08D, (int32_t)0x810262AE, (int32_t)0xEFF514A2, (int32_t)0xAE68E405, (int32_t)0x62A02D8D, 
};
#define N 100
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT5_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT5_32x32,
    fft_stageS3_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT5_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT5_32x32,
    fft_stageS3_DFT5_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 
}; 
static const cint32ptr_t tw_tab[] = 
{
    __fft100_tw1, __fft100_tw2, NULL
};
static const fft_cplx32x32_descr_t __cfft_descr100_32x32 = 
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
};     
static const fft_cplx32x32_descr_t __cifft_descr100_32x32 = 
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_100  = (const fft_handle_t)&__cfft_descr100_32x32;
const fft_handle_t cinfft32_100 = (const fft_handle_t)&__cifft_descr100_32x32;
