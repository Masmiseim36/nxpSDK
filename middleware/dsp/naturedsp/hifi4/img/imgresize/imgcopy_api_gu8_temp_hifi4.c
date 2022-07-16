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
image copying (8bit non generic->16-bit aligned temporary)
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
{
    const uint8_t * restrict pIn;
          int16_t * restrict pOut;
    int m, n, witer,
        w = (int)in->width,
        h = (int)in->height,
        istride = in->stride,
        ostride = out->stride;
    ae_valign alOut;

    (void)pScr;
    NASSERT(out->width >= in->width);
    NASSERT(out->height >= in->height);
    imgsize_validate(in, 1, 0);
    imgsize_validate(out, 2, 1);

    alOut = AE_ZALIGN64();
    for (n = 0; n < h; n++)
    {
        pIn  = ((const uint8_t *)inImg + n*istride);
        pOut = ((      int16_t *)outImg + n*ostride);
        witer = w;
        /* First read input by 1 element until pointer will be aligned by 4 bytes */
        while ( (0 != ((uintptr_t)pIn & 3)) && (witer > 0) )
        {
            uint8_t p = *pIn++;
            *pOut++ = ((int16_t)p) << 7;
            --witer;
        }
        /* Copy other elements */
        for (m = 0; m < (witer & ~3); m+=4)
        {
            ae_int16x4 p0;
            AE_L8X4F_IP(p0, castxcc(const signed char,pIn), 4*sizeof(uint8_t));
            /* tricky way to make 16-bit logical right shift: this simply works 
               because AE_L8X4F_IP loads zeroes in low 8 bits of each 16-bit 
               portion */
            p0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(p0), 1));
            AE_SA16X4_IP(p0, alOut, castxcc(ae_int16x4,pOut));
        }
        AE_SA64POS_FP(alOut, pOut);
        __Pragma("no_unroll");
        for (; m < witer; m++)
        {
            uint8_t p = *pIn++;
            *pOut++ = ((int16_t)p) << 7;
        }
    }
}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in; (void)out;
    imgsize_validate(in,  1, 0);
    imgsize_validate(out, 2, 1);
    return 0;
}
const imgcopy_api imgcopy_api_gu8_temp={getScratchSize,copy};
