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
void imgcopyUpBtmBorders_gs16(void *up, void *btm, const void * restrict inImg, const imgpad_params_t* params)
{
    const int16_t* restrict in=(const int16_t*)inImg;
    int n,w=params->out.width,x,x0=params->x,win=params->in.width,istride=params->in.stride,hin=params->in.height;
    int16_t lu=in[0];
    int16_t ru=in[win-1];
    int16_t lb=in[istride*(hin-1)];
    int16_t rb=in[istride*(hin-1)+win-1];
    int16_t* restrict outu = (int16_t*)up;
    int16_t* restrict outb = (int16_t*)btm;
    const ae_int16x4 * restrict pInu;
    const ae_int16x4 * restrict pInb;
    ae_int16x4 * restrict pOutu = (ae_int16x4 *)outu;
    ae_int16x4 * restrict pOutb = (ae_int16x4 *)outb;
    ae_valign aliu, alib, alou, alob;
    ae_int16x4 xu, xb;
    NASSERT(up );
    NASSERT(btm);
    NASSERT_ALIGN(up,ALIGNMENT);
    NASSERT_ALIGN(btm,ALIGNMENT);
    x=XT_MAX(0,XT_MIN(x0,w));
    n = 0;
    if (params->fill==IMGPAD_EDGE)
    {
        if (x>0)
        {
            xu = AE_MOVDA16(lu);
            xb = AE_MOVDA16(lb);
            for (n = 0; n < (x >> 2); n++)
            {
                AE_S16X4_IP(xu, pOutu, 4 * sizeof(int16_t));
                AE_S16X4_IP(xb, pOutb, 4 * sizeof(int16_t));
            }
            for (n = (x&(~3)); n < x; n++)
            {
                outu[n] = lu;
                outb[n] = lb;
            }
        }
        int l, r;
        l = XT_MAX(0, x);
        r = XT_MIN(w, (x0 + win));
        if (r > l)
        {
            alob = alou = AE_ZALIGN64();
            pOutu = (ae_int16x4 *)(outu + l);
            pOutb = (ae_int16x4 *)(outb + l);
            pInu = (const ae_int16x4 *)(in + (l - x0));
            pInb = (const ae_int16x4 *)(in + (l - x0) + (hin - 1)*istride);
            aliu = AE_LA64_PP(pInu);
            alib = AE_LA64_PP(pInb);
            for (n = 0; n<((r-l)>>2); n++)
            {
                AE_LA16X4_IP(xu, aliu, pInu);
                AE_LA16X4_IP(xb, alib, pInb);
                AE_SA16X4_IP(xu, alou, pOutu);
                AE_SA16X4_IP(xb, alob, pOutb);
            }
            AE_SA64POS_FP(alob, pOutb);
            AE_SA64POS_FP(alou, pOutu);
            for (n = l + ((r - l)&(~3)); n<r; n++)
            {
                outu[n] = in[(n - x0)];
                outb[n] = in[(n - x0) + (hin - 1)*istride];
            }
        }
        l = XT_MAX(l, r);
        if (l < w)
        {
            alob = alou = AE_ZALIGN64();
            pOutu = (ae_int16x4 *)(outu + l);
            pOutb = (ae_int16x4 *)(outb + l);
            xu = AE_MOVDA16(ru);
            xb = AE_MOVDA16(rb);
            for (n = 0; n < ((w-l)>>2); n++)
            {
                AE_SA16X4_IP(xu, alou, pOutu);
                AE_SA16X4_IP(xb, alob, pOutb);
            }
            AE_SA64POS_FP(alob, pOutb);
            AE_SA64POS_FP(alou, pOutu);
            for (n = l+ ((w-l)&(~3)); n < w; n++)
            {
                outu[n] = ru;
                outb[n] = rb;
            }
        }

    }
    else
    {
        int16_t fill = (int16_t)XT_MIN(32767, XT_MAX(0, params->fill));
        xu = AE_MOVDA16(fill);
        xb = AE_MOVDA16(fill);
        for (n = 0; n < (w >> 2); n++)
        {
            AE_S16X4_IP(xu, pOutu, 4 * sizeof(int16_t));
            AE_S16X4_IP(xb, pOutb, 4 * sizeof(int16_t));
        }
        for (n = (w&(~3)); n < w; n++)
        {
            outu[n] = fill;
            outb[n] = fill;
        }
    }
}
