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
image copying (16-bit aligned temporary->8bit generic)
Input:
in  - input size
out - output size
inImg - input image
Output:
outImg - output image
Temporary:
pScr
-------------------------------------------------------------------------*/
static void copy(void* pScr, void* restrict outImg,const void* restrict inImg,const imgsize_t* restrict in,const imgsize_t* restrict out)
{
    const int16_t * restrict pIn;
          uint8_t * restrict pOut;
    ae_valign alIn, alOut;
    int m, n, w, h,
        wout = (int)out->width,
        win = (int)out->width,
        hout = (int)out->height,
        hin = (int)out->height,
        istride = in->stride,
        ostride = out->stride;
    (void)hin, (void)win;
    NASSERT(wout <= win);
    NASSERT(hout <= hin);
    imgsize_validate(in, 2, 1);
    imgsize_validate(out, 1, 0);
    h = hout;
    alOut = AE_ZALIGN64();

    for (n = 0; n < h; n++)
    {
        pIn  = ((const int16_t *)inImg + n*istride);
        pOut = ((      uint8_t *)outImg + n*ostride);
        w = wout;

        if ((uintptr_t)pOut & 1)
        {
            int16_t p = *pIn++;
            p >>= 7;
            p = XT_MIN(255, XT_MAX(0, p));
            *pOut++ = (uint8_t)p;
            --w;
        }

        alIn = AE_LA64_PP(pIn);
        for (m = 0; m < (w>>3); m++)
        {
            ae_int16x4 p0, p1, p2, p3;
            AE_LA16X4_IP(p0, alIn, castxcc(ae_int16x4,pIn));
            AE_LA16X4_IP(p1, alIn, castxcc(ae_int16x4,pIn));
            AE_MOVT16X4(p0, 0, AE_LT16(p0, 0));
            AE_MOVT16X4(p1, 0, AE_LT16(p1, 0));
            p0 = AE_AND16(AE_ADD16(p0, p0), (int16_t)0xff00);
            p1 = AE_AND16(AE_ADD16(p1, p1), (int16_t)0xff00);
            p2 = AE_SEL16_7531(p0, p1);
            p3 = AE_SEL16_6420(p0, p1);
            p0 = p2; p1 = p3;
            p0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(p0), 8));
            p0 = AE_OR16(p0, p1);
            AE_SA16X4_IP(p0, alOut, castxcc(ae_int16x4,pOut));
        }
        AE_SA64POS_FP(alOut, pOut);
        for (m = 0; m < (w&7); m++)
        {
            int16_t p = *pIn++;
            p >>= 7;
            p = XT_MIN(255, XT_MAX(0, p));
            *pOut++ = (uint8_t)p;
        }
    }
}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in; (void)out;
    imgsize_validate(in,  2, 1);
    imgsize_validate(out, 1, 0);
    return 0;
}
const imgcopy_api imgcopy_api_temp_gu8={getScratchSize,copy};
