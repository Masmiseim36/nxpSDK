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
inImg - input imgae
Output:
outImg - output image
Temporary:
pScr
-------------------------------------------------------------------------*/
static void copy(void* pScr, void* restrict outImg,const void* restrict inImg,const imgsize_t* restrict in,const imgsize_t* restrict out)
{
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    ae_valign alOut;
    int m, n, w, h,
        wout = (int)out->width,
        win = (int)in->width,
        hout = (int)out->height,
        hin = (int)in->height,
        istride = in->stride,
        ostride = out->stride;
    (void)win,(void)hin;
    NASSERT(wout <= win);
    NASSERT(hout <= hin);
    imgsize_validate(in, 2, 1);
    imgsize_validate(out, 2, 0);

    h = hout;
    w = wout;
    alOut = AE_ZALIGN64();

    for (n = 0; n < h; n++)
    {
        pIn  = (const ae_int16x4 *)((int16_t *)inImg + n*istride);
        pOut = (      ae_int16x4 *)((int16_t *)outImg + n*ostride);
        for (m = 0; m < (w>>2); m++)
        {
            ae_int16x4 p0;
            AE_L16X4_IP(p0, pIn, sizeof(ae_int16x4));
            AE_MOVT16X4(p0, 0, AE_LT16(p0, 0));
            AE_SA16X4_IP(p0, alOut, pOut);
        }
        AE_SA64POS_FP(alOut, pOut);
        __Pragma("no_unroll");
        for (m = 0; m < (w&3); m++)
        {
            ae_int16x4 p0;
            AE_L16_IP(p0, castxcc(ae_int16,pIn), sizeof(int16_t));
            AE_MOVT16X4(p0, 0, AE_LT16(p0, 0));
            AE_S16_0_IP(p0, castxcc(ae_int16,pOut), sizeof(int16_t));
        }
    }
}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in; (void)out;
    imgsize_validate(in,  2, 1);
    imgsize_validate(out, 2, 0);
    return 0;
}
const imgcopy_api imgcopy_api_temp_gs16={getScratchSize,copy};
