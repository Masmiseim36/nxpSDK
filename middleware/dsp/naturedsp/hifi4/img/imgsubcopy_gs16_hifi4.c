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
    Functon copies one subimage to another subimage
    Input:
    in - input image
    width,height - size
    istride - stride of input image
    ostride - stride of output image
    Output:
    out - output image
------------------------------------------------*/
void imgsubcopy_gs16(void* outImg, const void* inImg, int width,int height, int istride, int ostride)
{
          int16_t * restrict out=(int16_t * )outImg;
    const int16_t * restrict in =(const int16_t * )inImg;
    ae_int16x4 * restrict pOut;
    const ae_int16x4 * restrict pIn;
    ae_valign alin, alout;
    int m,n;
    NASSERT(outImg );
    NASSERT(inImg );
    if ((width <= 0) || (height <= 0)) return;
    //everything is perfectly aligned
    if ((istride % 4 == 0) && (ostride % 4 == 0) && ((uintptr_t)in % 8 == 0) && ((uintptr_t)out % 8 == 0))
    {        
        for (n = 0; n < height; n++)
        {
            pIn = (const ae_int16x4 *)(in + n*istride);
            pOut = (ae_int16x4 *)(out + n*ostride);
            ae_int16x4 x;
            for (m = 0; m < (width >> 2); m++)
            {
                AE_L16X4_IP(x, pIn, 4 * sizeof(int16_t));
                AE_S16X4_IP(x, pOut, 4 * sizeof(int16_t));
            }
        }       
    }
    else if ((istride % 4 == 0) && ((uintptr_t)in % 8 == 0)) //only input aligned
    {
        alout = AE_ZALIGN64();
        for (n = 0; n < height; n++)
        {
            pIn = (const ae_int16x4 *)(in + n*istride);
            pOut = (ae_int16x4 *)(out + n*ostride);
            ae_int16x4 x;
            for (m = 0; m < (width >> 2); m++)
            {
                AE_L16X4_IP(x, pIn, 4 * sizeof(int16_t));
                AE_SA16X4_IP(x, alout, pOut);
            }
            AE_SA64POS_FP(alout, pOut);
        }
    }
    else if ((ostride % 4 == 0) && ((uintptr_t)out % 8 == 0)) //only output is aligned
    {
        for (n = 0; n < height; n++)
        {
            pIn = (const ae_int16x4 *)(in + n*istride);
            pOut = (ae_int16x4 *)(out + n*ostride);
            alin = AE_LA64_PP(pIn);
            ae_int16x4 x;
            for (m = 0; m < (width >> 2); m++)
            {
                AE_LA16X4_IP(x, alin, pIn);
                AE_S16X4_IP(x, pOut, 4 * sizeof(int16_t));
            }
        }
    }
    else //nothing is aligned
    {
        alout = AE_ZALIGN64();
        for (n = 0; n < height; n++)
        {
            pIn = (const ae_int16x4 *)(in + n*istride);
            pOut = (ae_int16x4 *)(out + n*ostride);
            alin = AE_LA64_PP(pIn);
            ae_int16x4 x;
            for (m = 0; m < (width >> 2); m++)
            {
                AE_LA16X4_IP(x, alin, pIn);
                AE_SA16X4_IP(x, alout, pOut);
            }
            AE_SA64POS_FP(alout, pOut);
        }
    }
    for (n = 0; n < height; n++)
    {
        for (m = width&(~3); m < width; m++)
        {
            out[n*ostride + m] = in[n*istride + m];
        }
    }
    
}
