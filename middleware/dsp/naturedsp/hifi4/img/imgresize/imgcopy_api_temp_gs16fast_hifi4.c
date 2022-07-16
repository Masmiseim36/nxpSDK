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

#define MAX(a,b)        ((a)>(b) ? (a) : (b))
#define MIN(a,b)        ((a)<(b) ? (a) : (b))

/*-------------------------------------------------------------------------
image copying (16-bit aligned temporary->8bit aligned)
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
    int m,n,w,h,
        wout=(int)out->width, win=(int)out->width,
        hout=(int)out->height,hin=(int)out->height,
        istride=in->stride,ostride=out->stride;
    NASSERT(wout <= win );
    NASSERT(hout <= hin );
    h=hout;
    w=wout;
    imgsize_validate(in,  2, 1);
    imgsize_validate(out, 2, 1);
    for (n=0; n<h; n++)
    for (m=0; m<w; m++)
    {
        int16_t p=((const int16_t*)inImg)[m+n*istride];
        p=MAX(0,p);
        ((int16_t*)outImg)[m+n*ostride]=(int16_t)p;
    }
}
#else
{
    const ae_int16x4 * restrict src;
          ae_int16x4 * restrict dst;
    int m,n,w,h,
        wout=(int)out->width, win=(int)out->width,
        hout=(int)out->height,hin=(int)out->height,
        istride=in->stride,ostride=out->stride;
    NASSERT(wout <= win );
    NASSERT(hout <= hin );
    (void)win,(void)hin;
    h=hout;
    w=wout;
    imgsize_validate(in,  2, 1);
    imgsize_validate(out, 2, 1);
    NASSERT(w<=ostride);
    if (w<=0 || h<=0) return;
    src=(const ae_int16x4 *)inImg;
    dst=(      ae_int16x4 *)outImg;
    NASSERT_ALIGN(src,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(dst,HIFI_SIMD_WIDTH);
    if ((w>>2)>0)
    {
        for (n=0; n<h; n++)
        {
            __Pragma("loop_count min=1")
            for (m=0; m<(w>>2); m++)
            {
                ae_int16x4 p;
                AE_L16X4_IP(p,src,sizeof(ae_int16x4));
                AE_MOVT16X4(p,0,AE_LT16(p,0));
                AE_S16X4_IP(p,dst,sizeof(ae_int16x4));
            }
            src = (const ae_int16x4 *)((uintptr_t)src+ (istride-(w&~3))*sizeof(int16_t));
            dst = (      ae_int16x4 *)((uintptr_t)dst+ (ostride-(w&~3))*sizeof(int16_t));
        }
    }
    for (m=(w&~3); m<w; m++)
    {
        src=(const ae_int16x4 *)((int16_t*)inImg +m) ;
        dst=(      ae_int16x4 *)((int16_t*)outImg+m) ;
        for (n=0; n<h; n++)
        {
            ae_int16x4 p;
            AE_L16_XP(p,castxcc(ae_int16,src),sizeof(int16_t)*istride);
            AE_MOVT16X4(p,0,AE_LT16(p,0));
            AE_S16_0_XP(p,castxcc(ae_int16,dst),sizeof(int16_t)*ostride);
        }
    }
}
#endif


static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in; (void)out;
    imgsize_validate(in,  2, 1);
    imgsize_validate(out, 2, 1);
    return 0;
}
const imgcopy_api imgcopy_api_temp_gs16fast={getScratchSize,copy};
