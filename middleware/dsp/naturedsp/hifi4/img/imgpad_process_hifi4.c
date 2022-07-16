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

/*-------------------------------------------------------------------------
  Image padding
  Padding function makes left/right/top/bottom padding for input image
  It copies input image to the output at specific position and fills
  borders either with specified color or by duplication of pixels from
  corresponding edge
  
  Image formats:
  gs     8-bit grayscale data
  gs16   16-bit grayscale data

  Input
  params      Parameters of image padding operation
  inImage     input image
  Output:
  outImage    output image
  Temporary:
  pScr        Scratch memory area. To determine the scratch area size, 
              use the respective helper functions: xxx_getScratchSize()
  Restrictions:
  1. see general restrictions applied for all images for fast/generic 
     functions
  2. each row of output image should be always aligned 
  3. pointer to the scratch memory should be aligned on 8-byte boundary
-------------------------------------------------------------------------*/
void imgpad_process ( void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgpad_params_t* params, const imgpad_api *api )
{
    uint8_t *left,*right,*up,*btm;
    int xoff,yoff,x,y,win,wout,hin,hout,istride,ostride,fill;
    NASSERT_ALIGN(pScr, HIFI_SIMD_WIDTH);
    imgsize_validate(&params->in ,api->bpp,api->fast);
    imgsize_validate(&params->out,api->bpp,api->fast);
    NASSERT_ALIGN(inImg, (api->fast ? HIFI_SIMD_WIDTH:api->bpp));
    NASSERT_ALIGN(outImg,(api->fast ? HIFI_SIMD_WIDTH:api->bpp));
    wout=params->out.width;
    hout=params->out.height;
    ostride=params->out.stride;
    win=params->in.width;
    hin=params->in.height;
    istride=params->in.stride;
    fill=params->fill;
    x=params->x;
    y=params->y;
    (void)fill;
    /* allocate in scratch */
    
    {
        int align=HIFI_SIMD_WIDTH;
        uintptr_t a=(uintptr_t)pScr;
        left=right=up=btm=NULL;
        if (!imgpad_isCropHorz(params))
        {
            left=(uint8_t*)a;
            a+=((hout*api->bpp)+(align-1))&~(align-1);
            right=(uint8_t*)a;
            a+=((hout*api->bpp)+(align-1))&~(align-1);
        }
        if (!imgpad_isCropVert(params))
        {
            up   =(uint8_t*)a;
            a+=((wout*api->bpp)+(align-1))&~(align-1);
            btm  =(uint8_t*)a;
            a+=((wout*api->bpp)+(align-1))&~(align-1);
        }
    }
    /* copy edges */
    if (left) api->copyLeftRightBorders(left,right,inImg,params);
    if (up)   api->copyUpBtmBorders    (up,  btm,  inImg,params);
    if (x>=0)      api->copyVert  (outImg,left ,XT_MIN(wout,x),hout,ostride);
    if (x+win<wout)api->copyVert  (((uint8_t*)outImg)+api->bpp*XT_MAX(0,(x+win)),right,wout-XT_MAX(0,(x+win)),hout,ostride);
    if (y>=0)      api->copyHorz  (outImg,up ,wout,XT_MIN(hout,y),ostride);
    if (y+hin<hout)api->copyHorz  (((uint8_t*)outImg)+api->bpp*(ostride*XT_MAX(0,(y+hin))),btm,wout,hout-XT_MAX(0,(y+hin)),ostride);

    xoff=0; yoff=0;
    xoff = (x<0) ? -x: 0;
    yoff = (y<0) ? -y: 0;
    win-=xoff; x+=xoff;
    hin-=yoff; y+=yoff;
    win=XT_MIN(win,wout-x);
    hin=XT_MIN(hin,hout-y);
    api->subcopy(((uint8_t*)outImg)+api->bpp*(x+y*ostride), ((const uint8_t*)inImg)+api->bpp*(xoff+yoff*istride), win,hin,istride,ostride);
}

/* Returns: size of scratch memory area, in bytes. */
size_t imgpad_getScratchSize  ( const imgpad_params_t * params, const imgpad_api *api )
{
    size_t sz=0;
    if (!imgpad_isCropHorz(params))
    {
        sz+=((params->out.height*api->bpp+(ALIGNMENT-1))&~(ALIGNMENT-1))*2;
    }
    if (!imgpad_isCropVert(params))
    {
        sz+=((params->out.width *api->bpp+(ALIGNMENT-1))&~(ALIGNMENT-1))*2;
    }
    return sz;
}
