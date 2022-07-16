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
void imgcopyLeftRightBorders_gu8(void *left, void *right, const void * restrict inImg, const imgpad_params_t* params)
{
    const uint8_t* restrict in=(const uint8_t*)inImg;
    int n,h=params->out.height,y,y0=params->y,hin=params->in.height,istride=params->in.stride,win=params->in.width;
    uint8_t lu=in[0];
    uint8_t ru=in[win-1];
    uint8_t lb=in[istride*(hin-1)];
    uint8_t rb=in[istride*(hin-1)+win-1];
    uint8_t* restrict outl = (uint8_t*)left;
    uint8_t* restrict outr = (uint8_t*)right;
    const unsigned char * restrict pin0;
    const unsigned char * restrict pin1;
    signed char * restrict pout0;
    signed char * restrict pout1;
    ae_int16x4 xl, xr;
    ae_int16x4 * restrict pOutl = (ae_int16x4 *)outl;
    ae_int16x4 * restrict pOutr = (ae_int16x4 *)outr;
    ae_valign all, alr;
    NASSERT(left );
    NASSERT(right);
    NASSERT_ALIGN(left ,ALIGNMENT);
    NASSERT_ALIGN(right,ALIGNMENT);
    y=XT_MAX(0,XT_MIN(y0,h));
    n = 0;
    if (params->fill==IMGPAD_EDGE)
    {
        if (y>0)
        {
            xl = AE_MOVDA16(lu);
            xl = AE_OR16(xl, AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(xl), 8)));
            xr = AE_MOVDA16(ru);
            xr = AE_OR16(xr, AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(xr), 8)));
            for (n = 0; n < (y >> 3); n++)
            {
                AE_S16X4_IP(xl, pOutl, 4 * sizeof(int16_t));
                AE_S16X4_IP(xr, pOutr, 4 * sizeof(int16_t));
            }
            for (n = (y&(~7)); n < y; n++)
            {
                outl[n] = lu;
                outr[n] = ru;
            }
        }
        int l, r;
        l = XT_MAX(0, y);
        r = XT_MIN(h, (y0 + hin));
        //for (n=l; n<r; n++)
        //{
        //    outl[n] = in[istride*(n - y0)];
        //    outr[n] = in[istride*(n - y0) + win - 1];
        //}
        pout0 = (signed char*)(outl+l);
        pout1 = (signed char*)(outr+l);
        pin0 = (const unsigned char*)(in + istride*(l-y0));
        pin1 = (const unsigned char*)(in + istride*(l-y0) + win - 1);
        unsigned char x0, x1;
        for (n = l; n < r; n++)
        {
            x0 = XT_L8UI(pin0, 0);
            x1 = XT_L8UI(pin1, 0);
            pin0 += istride;
            pin1 += istride;
            XT_S8I(x0, pout0, 0);
            XT_S8I(x1, pout1, 0);
            pout0 += 1;
            pout1 += 1;
        }

        l = XT_MAX(l, r);
        if (l < h)
        {
            all = alr = AE_ZALIGN64();
            pOutl = (ae_int16x4 *)(outl + l);
            pOutr = (ae_int16x4 *)(outr + l);
            xl = AE_MOVDA16(lb);
            xr = AE_MOVDA16(rb);
            int shl = ((uintptr_t)pOutl) & 1;
            int shr = ((uintptr_t)pOutr) & 1;
            if (shl)
            {
                outl[n] = lb;
                pOutl = (ae_int16x4 *)(outl + l + 1);
            }
            if (shr)
            {
                outr[n] = rb;
                pOutr = (ae_int16x4 *)(outr + l + 1);
            }
            for (n = 0; n < ((h - l-shl) >> 3); n++)
            {
                AE_SA16X4_IP(xl, all, pOutl);
            }
            for (n = 0; n < ((h - l-shr) >> 3); n++)
            {
                AE_SA16X4_IP(xr, alr, pOutr);
            }
            AE_SA64POS_FP(all, pOutl);
            AE_SA64POS_FP(alr, pOutr);
            for (n =shl+ l + ((h - l-shl)&(~7)); n < h; n++)
            {
                outl[n] = lb;
            }
            for (n = shr+l + ((h - l-shr)&(~7)); n < h; n++)
            {
                outr[n] = rb;
            }
        }
    }
    else
    {
        uint8_t fill=(uint8_t)XT_MIN(255,XT_MAX(0,params->fill));
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
