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
    Functon copies upper/bottom edges
    Input:
    params - parameters of padding
    inImg  - input image
    Output:
    up[h] -  upper edge extended with lu and 
             ru points
    btm[h]-  bottom edge extended with lb and 
             rb points
------------------------------------------------*/

void imgcopyUpBtmBorders_gs8(void *up, void *btm, const void * restrict inImg, const imgpad_params_t* params)
{
    int n,w=params->out.width;
    uint8_t* restrict outu = (uint8_t*)up;
    uint8_t* restrict outb = (uint8_t*)btm;
    ae_int16x4 * restrict pOutu = (ae_int16x4 *)outu;
    ae_int16x4 * restrict pOutb = (ae_int16x4 *)outb;
    ae_int16x4 xu, xb;
    NASSERT(up );
    NASSERT(btm);
    NASSERT_ALIGN(up,ALIGNMENT);
    NASSERT_ALIGN(btm,ALIGNMENT);
    if (params->fill==IMGPAD_EDGE)
    {
        imgcopyUpBtmBorders_gu8(up, btm, inImg, params);
    }
    else
    {
        uint8_t fill=(uint8_t)XT_MIN(127,XT_MAX(-128,params->fill));
        xu = AE_MOVDA16(fill);
        xu = AE_OR16(xu, AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(xu), 8)));
        xb = AE_MOVDA16(fill);
        xb = AE_OR16(xb, AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(xb), 8)));
        for (n = 0; n < (w >> 3); n++)
        {
            AE_S16X4_IP(xu, pOutu, 4 * sizeof(int16_t));
            AE_S16X4_IP(xb, pOutb, 4 * sizeof(int16_t));
        }
        for (n = (w&(~7)); n < w; n++)
        {
            outu[n] = fill;
            outb[n] = fill;
        }
    }
}

