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
    Functon copies left/right edges
    Input:
    params - parameters of padding
    inImg  - input image
    Output:
    left[h] - left edge extended with lu and 
              lb points
    right[h]- right edge extended with ru and 
              rb points
------------------------------------------------*/

void imgcopyLeftRightBorders_gs8(void *left, void *right, const void * restrict inImg, const imgpad_params_t* params)
{
    int n,h=params->out.height;
    uint8_t* restrict outl = (uint8_t*)left;
    uint8_t* restrict outr = (uint8_t*)right;
    ae_int16x4 xl, xr;
    ae_int16x4 * restrict pOutl = (ae_int16x4 *)outl;
    ae_int16x4 * restrict pOutr = (ae_int16x4 *)outr;
    NASSERT(left );
    NASSERT(right);
    NASSERT_ALIGN(left ,ALIGNMENT);
    NASSERT_ALIGN(right,ALIGNMENT);
    if (params->fill==IMGPAD_EDGE)
    {
        imgcopyLeftRightBorders_gu8(left, right, inImg, params);
    }
    else
    {
        uint8_t fill=(uint8_t)XT_MIN(127,XT_MAX(-128,params->fill));
        xl = AE_MOVDA16(fill);
        xl = AE_OR16(xl, AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(xl), 8)));
        xr = AE_MOVDA16(fill);
        xr = AE_OR16(xr, AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(xr), 8)));
        for (n = 0; n < (h >> 3); n++)
        {
            AE_S16X4_IP(xl, pOutl, 4 * sizeof(int16_t));
            AE_S16X4_IP(xr, pOutr, 4 * sizeof(int16_t));
        }
        for (n = (h&(~7)); n < h; n++)
        {
            outl[n] = fill;
            outr[n] = fill;
        }
    }
}
