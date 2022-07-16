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
#include "imgresizer_common.h"
#include "imgresizer_nearest_common.h"

#define MAX(a,b)        ((a)>(b) ? (a) : (b))
#define MIN(a,b)        ((a)<(b) ? (a) : (b))

#define IMAGE_RESIZER_MAGIC 0x37623243

typedef struct tag_imgresizer_t
{
    uint32_t  magic;
    void*     coef; /* interpolation coefficients if any */
    imgsize_t in;    /* input image sizes  */
    imgsize_t out;   /* output image sizes */
    const imgresizer_api_t* api;
}
imgresizer_nearest_t;

static const imgresizer_api_t* imgresizer_nearest_getApi(const imgsize_t* in,const imgsize_t* out)
{
    //NASSERT(in->stride==out->stride);   // in-place only!!!
    if (in->width==out->width)
    {   // vertical
       return &imgresizer_api_nv;
    }
    else
    {
        NASSERT(in->height==out->height);
        // horizontal
       return &imgresizer_api_nh;
    }
    NASSERT(0); // some error in the upper level logic!
    return NULL;
} 

static size_t imgresizer_nearest_alloc(const imgsize_t* in,const imgsize_t* out)
{
    size_t sz;
    sz=ALIGNMENT - 1;
    const imgresizer_api_t* api=imgresizer_nearest_getApi(in,out);
    sz+=(sizeof(imgresizer_nearest_t)+(ALIGNMENT-1))&~(ALIGNMENT-1);
    sz+=(api->getCoefSz(in,out)+(ALIGNMENT-1))&~(ALIGNMENT-1);
    return sz;
}
static size_t imgresizer_nearest_getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    return imgresizer_nearest_getApi(in,out)->getScratchSize(in,out);
}
static struct tag_imgresizer_t* imgresizer_nearest_init(void* objmem,const imgsize_t* in,const imgsize_t* out)
{
    imgresizer_nearest_t* pResizer;
    /* allocate objects inside */
    uintptr_t a=(uintptr_t)objmem;
    a=(a+(ALIGNMENT-1))&~(ALIGNMENT-1);
    pResizer=(imgresizer_nearest_t* )a;
    a=(((uintptr_t)(pResizer+1))+(ALIGNMENT-1))&~(ALIGNMENT-1);
    pResizer->coef=(void*)a;
    /* initialize everything */
    pResizer->magic=IMAGE_RESIZER_MAGIC;
    pResizer->api=imgresizer_nearest_getApi(in,out);
    pResizer->in=*in;
    pResizer->out=*out;
    pResizer->api->getCoef(pResizer->coef,in,out);
    return (struct tag_imgresizer_t*)pResizer;
}

static void imgresizer_nearest_process(struct tag_imgresizer_t* pResizer, void *pScr, const void* imgIn, void* imgOut, int fast)
{
    NASSERT(pResizer);
    NASSERT(pResizer->magic==IMAGE_RESIZER_MAGIC);
    pResizer->api->process(pScr,pResizer->coef,imgIn,imgOut,&pResizer->in,&pResizer->out, fast);
}

/* 
    iterator returning image size for current transform
    Input:
    inSz - current input image size
    outSz - final image size
    Output:
    currentOutSz - output size for current intermediate transform

    Function iterates in vertical domain first!
*/
static void imgresizer_nearest_getnext(imgsize_t* currentOutSz,const imgsize_t* inSz,const imgsize_t* outSz)
{
    if (inSz->height != outSz->height)
    {
        currentOutSz->width = inSz->width;
       // currentOutSz->stride = inSz->stride; //!!!!!!!!!!
        currentOutSz->height = outSz->height;

    }
    else
    {
        currentOutSz->height = inSz->height;
        currentOutSz->stride = outSz->stride;
        currentOutSz->width = outSz->width;
    }
}

static const imgresize_method_api_t nearest=
{
    "",
    imgresizer_nearest_getApi,
    imgresizer_nearest_alloc,
    imgresizer_nearest_getScratchSize,
    imgresizer_nearest_init,
    imgresizer_nearest_process,
    imgresizer_nearest_getnext,
    0
};

const imgresize_method_t imgresize_method_nearest=(const imgresize_method_t)&nearest;
