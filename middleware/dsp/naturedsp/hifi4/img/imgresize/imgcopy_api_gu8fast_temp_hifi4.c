/* ------------------------------------------------------------------------ */
/* Copyright (c) 2019 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2019 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"
#include "imgcopy_common.h"

/*-------------------------------------------------------------------------
image copying (8bit aligned->16-bit aligned temporary)
Input:
in  - input size
out - output size
inImg - input imgae
Output:
outImg - output image
Temporary:
pScr
-------------------------------------------------------------------------*/
static void copy(void* pScr, void* restrict outImg,const void* restrict inImg,const imgsize_t* restrict in,const imgsize_t* restrict out)
#if 0
{
    int m,n,w=(int)in->width ,h=(int)in->height,istride=in->stride,ostride=out->stride;
    (void)pScr;
    NASSERT(out->width >= in->width);
    NASSERT(out->height >= in->height);
    imgsize_validate(in,  1, 1);
    imgsize_validate(out, 2, 1);
    for (n=0; n<h; n++)
    for (m=0; m<w; m++)
    {
        uint8_t p=((const uint8_t*)inImg)[m+n*istride];
        ((int16_t*)outImg)[m+n*ostride]=((int16_t)p)<<7;
    }
}
#else
{
    const signed char * restrict src;
          ae_int16x4 * restrict dst;
    int m,n,w=(int)in->width ,h=(int)in->height,istride=in->stride,ostride=out->stride;
    (void)pScr;
    NASSERT(out->width >= in->width);
    NASSERT(out->height >= in->height);
    imgsize_validate(in,  1, 1);
    imgsize_validate(out, 2, 1);
    w=(w+3)&~3;
    NASSERT(w<=ostride);
    if (w<=0) return;
    src=(const signed char *)inImg;
    dst=(       ae_int16x4 *)outImg;
    NASSERT_ALIGN(src,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(dst,HIFI_SIMD_WIDTH);
    for (n=0; n<h; n++)
    {
        __Pragma("loop_count min=1")
        for (m=0; m<(w>>2); m++)
        {
            ae_f16x4 p;
            AE_L8X4F_IP(p,src,sizeof(ae_int16x4)/2);
            /* tricky way to make 16-bit logical right shift: this simply works 
               because AE_L8X4F_IP loads zeroes in low 8 bits of each 16-bit 
               portion */
            p=AE_MOVINT16X4_FROMINT32X2(AE_SRLI32( AE_MOVINT32X2_FROMINT16X4(p),1));
            AE_S16X4_IP(p,dst,sizeof(ae_int16x4));
        }
        src = (const signed char *)((uintptr_t)src+ (istride-w)*sizeof(uint8_t));
        dst = (       ae_int16x4 *)((uintptr_t)dst+ (ostride-w)*sizeof(int16_t));
    }
}
#endif

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in; (void)out;
    imgsize_validate(in,  1, 1);
    imgsize_validate(out, 2, 1);
    return 0;
}
const imgcopy_api imgcopy_api_gu8fast_temp={getScratchSize,copy};
