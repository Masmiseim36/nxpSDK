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
#include "imgrotate_common.h"
#include "common.h"

#define SAVEDEBUGINFO 0

#if SAVEDEBUGINFO 
#include <stdio.h>
static void _dump16(const char*fname, void* img, const imgsize_t *sz)
{
    int i,j;
    FILE *f=fopen(fname,"wt");
    for (i=0; i<(int)sz->height; i++)
    {
        for (j=0; j<(int)sz->width ; j++) fprintf(f,"%d ",((int16_t*)img)[i*sz->stride+j]);
        fprintf(f,"\n");
    }
    fclose(f);
}
static void _dump8(const char*fname, void* img, const imgsize_t *sz)
{
    int i,j;
    FILE *f=fopen(fname,"wt");
    for (i=0; i<(int)sz->height; i++)
    {
        for (j=0; j<(int)sz->width ; j++) fprintf(f,"%d ",((uint8_t*)img)[i*sz->stride+j]);
        fprintf(f,"\n");
    }
    fclose(f);
}
#else
#define _dump16(fname,img,sz) {}
#define _dump8(fname,img,sz) {}
#endif

/*---------------------------------------------------------------------------
   main function for image rotation
   this function consists of following stages:
   1) if rotation angle is a multiple of 90 degress we call dedicated out of 
      place rotation function without temporary image
   2) if rotation angle is not an exact multiple of 90 degress we need to 
      rotate input image to temporary image in 16-bit representation. 
      Afterwards we make 3 shear inplace stages with this temporary image
      and crop the output with format conversion

  Input
  handle      image processing handle
  inImage     input image

  Output:
  outImage    output image

  Temporary:
  pScr        Scratch memory area
---------------------------------------------------------------------------*/
void imgrotate_process  ( imgrotate_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgsize_t * restrict outSz)
{
    int fillColor;
    void *tmpImg;
    const tImgRotateApi *pApi;
    size_t szScr;
    int bpp,fast;
    imgsize_t tmpSz;
    int q;
    tImgRotate *pRot;
    pRot=(tImgRotate*)handle;
    /* validate inputs */
    NASSERT(handle);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    pApi=pRot->pApi;
    NASSERT(pApi);
    NASSERT(pRot->magic==pApi->magic);
    NASSERT(pRot->coef.outSz.width==outSz->width);
    NASSERT(pRot->coef.outSz.height==outSz->height);
    NASSERT(pRot->coef.outSz.stride<=outSz->stride);
    bpp=pRot->pApi->bpp;
    fast=pRot->pApi->fast;
    if (fast)
    {
        NASSERT_ALIGN(inImg,ALIGNMENT);
        NASSERT_ALIGN(outImg,ALIGNMENT);
    }
    imgrotate_params_validate(&pRot->params,bpp,fast);
    /* allocate scratch for internal functions first; next data in the scratch will be used for temporary image */
    szScr=pApi->getScratchSize(&pRot->params);
    szScr = (szScr+ALIGNMENT-1)&~(ALIGNMENT-1);
    tmpImg = (void*)(((uintptr_t)pScr)+szScr);

    q=((pRot->params.angleQ15+0x2000)>>14)&3;
    /* if rotation is a multiple of 90 degrees, than just make oop rotate and return */
    if ( (pRot->params.angleQ15 & 0x3fff) == 0)
    {
        pApi->rotq(pScr,outImg,outSz,inImg,&pRot->params.in, q);
        return;
    }
    /* otherwise we do following steps:
       0.   allocate image in the scratch region
       1.   oop copy/rotate to the scratch
       2-4. inplace v/hshears
       5.   oop croping of resulted image
    */
    fillColor=pApi->bpp==1 ? pRot->params.fill<<7 : pRot->params.fill;
    tmpSz=pRot->coef.tmpSz;
    pApi->rotq16 (pScr,tmpImg,&tmpSz,inImg,&pRot->params.in,q,fillColor);
    _dump16("rotq16.txt",tmpImg,&tmpSz);
    pApi->vshear(pScr,tmpImg,&tmpSz,pRot->coef.alpha,fillColor);
    _dump16("vshear1.txt",tmpImg,&tmpSz);
    pApi->hshear(pScr,tmpImg,&tmpSz,pRot->coef.beta ,fillColor);
    _dump16("hshear.txt",tmpImg,&tmpSz);
    pApi->vshear(pScr,tmpImg,&tmpSz,pRot->coef.alpha,fillColor);
    _dump16("vshear2.txt",tmpImg,&tmpSz);
     pApi->crop(pScr,outImg,outSz,tmpImg,&tmpSz,pRot->coef.left,pRot->coef.up);
    _dump8("cropped.txt",outImg,outSz);
}
