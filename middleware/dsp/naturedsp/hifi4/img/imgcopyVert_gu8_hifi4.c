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
#include "imgpad_common.h"

/*------------------------------------------------
    Functon copies vertical edges to rectange
    Input:
    edge[h] - vertical edge 
    w,h,ostride - copied region
    Output:
    outImg   - output image
------------------------------------------------*/
void imgcopyVert_gu8(void* outImg, const void* edge, int w, int h, int ostride)
{
          uint8_t * restrict out=(uint8_t * )outImg;
    const uint8_t * restrict in =(const uint8_t * )edge;
    int m,n;
    ae_int16x4 x;
    ae_int16x4 * restrict pOut;
    ae_valign alout;
    uint8_t t;
    NASSERT(edge );
    NASSERT_ALIGN(edge,ALIGNMENT);
    if (w <= 0 || h <= 0) return;
    if ((ostride % 8 == 0) && ((uintptr_t)out % 8 == 0))
    {
        for (n = 0; n < h; n++)
        {
            t = in[n];
            x = AE_MOVDA16(t);
            x = AE_OR16(x, AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x), 8)));
            pOut = (ae_int16x4 *)(out + n*ostride);
            for (m = 0; m < (w >> 3); m++)
            {
                AE_S16X4_IP(x, pOut, 4 * sizeof(int16_t));
            }
            for (m = (w&(~7)); m < w; m++)
            {
                out[n*ostride + m] = t;
            }
        }
    }
    else 
    {
        alout = AE_ZALIGN64();
        for (n = 0; n < h; n++)
        {
            t = in[n];
            x = AE_MOVDA16(t);
            x = AE_OR16(x, AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x), 8)));
            pOut = (ae_int16x4 *)(out + n*ostride);
            int sh = ((uintptr_t)pOut) & 1;
            if (sh)
            {
                out[n*ostride] = t;
                pOut = (ae_int16x4 *)(out + n*ostride+1);
            }
            for (m = 0; m < ((w-sh) >> 3); m++)
            {
                AE_SA16X4_IP(x, alout, pOut);
            }
            AE_SA64POS_FP(alout, pOut);
            for (m = sh+((w-sh)&(~7)); m < w; m++)
            {
                out[n*ostride + m] = t;
            }
        }
    }
}
