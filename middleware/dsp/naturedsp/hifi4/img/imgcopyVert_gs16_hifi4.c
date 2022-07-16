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
void imgcopyVert_gs16(void* outImg, const void* edge, int w, int h, int ostride)
{
          int16_t * restrict out=(int16_t * )outImg;
    const ae_int16 * restrict in =(const ae_int16 * )edge;
    ae_int16x4 * restrict pOut;
    ae_int16 * restrict out_;
    ae_int16x4 x;
    ae_valign alout;
    int m,n;
    NASSERT(edge );
    NASSERT_ALIGN(edge,ALIGNMENT);
    //aligned version
    if ((ostride % 4 == 0) && ((uintptr_t)out % 8 == 0))
    {
        for (n = 0; n < h; n++)
        {
            AE_L16_IP(x, in, sizeof(int16_t));
            pOut = (ae_int16x4 *)(out + n*ostride);
            for (m = 0; m < (w >> 2); m++)
            {
                AE_S16X4_IP(x, pOut, 4 * sizeof(int16_t));
            }
            out_ = (ae_int16 *)(pOut);
            for (m = (w&(~3)); m < w; m++)
            {
                AE_S16_0_IP(x, out_, sizeof(int16_t));
            }
        }
    }
    else //not aligned
    {
        alout = AE_ZALIGN64();
        for (n = 0; n < h; n++)
        {
            AE_L16_IP(x, in, sizeof(int16_t));
            pOut = (ae_int16x4 *)(out + n*ostride);
            for (m = 0; m < (w >> 2); m++)
            {
                AE_SA16X4_IP(x, alout, pOut);
            }
            AE_SA64POS_FP(alout, pOut);
            out_ = (ae_int16 *)(out + n*ostride + (w&(~3)));
            for (m = (w&(~3)); m < w; m++)
            {
                AE_S16_0_IP(x, out_, sizeof(int16_t));
            }
        }
    }
}
