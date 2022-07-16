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
#include "imgresizer_bicubic_common.h"

#define MAX(a,b)        ((a)>(b) ? (a) : (b))
#define MIN(a,b)        ((a)<(b) ? (a) : (b))

#define IMAGE_RESIZER_MAGIC 0x2364323d

typedef struct tag_imgresizer_t
{
    uint32_t  magic;
    void*     coef; /* interpolation coefficients if any */
    imgsize_t in;    /* input image sizes  */
    imgsize_t out;   /* output image sizes */
    const imgresizer_api_t* api;
}
imgresizer_bilinear_t;

static const imgresizer_api_t* imgresizer_bicubic_getApi(const imgsize_t* in,const imgsize_t* out)
{
    NASSERT(in->stride==out->stride);   // in-place only!!!
    if (in->width==out->width)
    {   // vertical
        if (in->height==2*out->height) return &imgresizer_api_dn2xv_cubic;
        if (2*in->height==out->height) return &imgresizer_api_up2xv_cubic;
        if (in->height<2*out->height && in->height>out->height) return &imgresizer_api_dnxv_cubic;
        if (out->height<2*in->height && out->height>in->height) return &imgresizer_api_upxv_cubic;
    }
    else
    {
        NASSERT(in->height==out->height);
        // horizontal
        if (in->width==2*out->width) return &imgresizer_api_dn2xh_cubic;
        if (2*in->width==out->width) return &imgresizer_api_up2xh_cubic;
        if (in->width<2*out->width && in->width>out->width) return &imgresizer_api_dnxh_cubic;
        if (out->width<2*in->width && out->width>in->width) return &imgresizer_api_upxh_cubic;
    }
    NASSERT(0); // some error in the upper level logic!
    return NULL;
}

static size_t imgresizer_bicubic_alloc(const imgsize_t* in,const imgsize_t* out)
{
    size_t sz;
    sz=ALIGNMENT-1;
    const imgresizer_api_t* api=imgresizer_bicubic_getApi(in,out);
    sz+=(sizeof(imgresizer_bilinear_t)+(ALIGNMENT-1))&~(ALIGNMENT-1);
    sz+=(api->getCoefSz(in,out)+(ALIGNMENT-1))&~(ALIGNMENT-1);
    return sz;
}
static size_t imgresizer_bicubic_getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    return imgresizer_bicubic_getApi(in,out)->getScratchSize(in,out);
}
static struct tag_imgresizer_t* imgresizer_bicubic_init(void* objmem,const imgsize_t* in,const imgsize_t* out)
{
    imgresizer_bilinear_t* pResizer;
    /* allocate objects inside */
    uintptr_t a=(uintptr_t)objmem;
    a=(a+(ALIGNMENT-1))&~(ALIGNMENT-1);
    pResizer=(imgresizer_bilinear_t* )a;
    a=(((uintptr_t)(pResizer+1))+(ALIGNMENT-1))&~(ALIGNMENT-1);
    pResizer->coef=(void*)a;
    /* initialize everything */
    pResizer->magic=IMAGE_RESIZER_MAGIC;
    pResizer->api=imgresizer_bicubic_getApi(in,out);
    pResizer->in=*in;
    pResizer->out=*out;
    pResizer->api->getCoef(pResizer->coef,in,out);
    return (struct tag_imgresizer_t*)pResizer;
}

static void imgresizer_bicubic_process(struct tag_imgresizer_t* pResizer, void *pScr, const void* imgIn, void* imgOut, int fast)
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

    For upsampling, function first try to upsample width to out->width/2^n first, 
    next height to out->height/2^n, next stepping by 2 for width and height.
    For downsampling, everything happens in reverse order: frist try to decimate height 2x, 
    next try to decimate width 2x, and when neither can be done, decimate width to 2^n*out->width
    then decimate height to 2^n*out->height.
*/
static int getnext(int in,int out)
{
    int t;
    /* for downsample: try first to decimate 2x, if not possible, decimate to 2^n*out */
    NASSERT(out!=in);
    if (out<in)
    {
        if(((in&1)==0) && (in>>1)>=out) return in>>1;
        while ((out<<1)<in) out<<=1;
        return out;
    }
    /* for upsample: another logic, first try to upsample to out/2^n, next stepping by 2 */    
    t=out;
    while ((t&1)==0 && (t>>1)>in) t>>=1;
    return MIN((in<<1),t);

}

static void imgresizer_bicubic_getnext(imgsize_t* currentOutSz,const imgsize_t* inSz,const imgsize_t* outSz)
{
    /*upsampling*/
    if (inSz->height <= outSz->height && inSz->width <= outSz->width)
    {               
        if ((inSz->width != outSz->width) && (inSz->height != outSz->height))
        {
            unsigned int h, w;
            h = getnext(inSz->height, outSz->height);
            w = getnext(inSz->width, outSz->width);
            /*Put 'bad' (not x2) resizers first*/
            if (w != 2 * inSz->width || h == 2 * inSz->height)
            {
                currentOutSz->height = inSz->height;
                currentOutSz->stride = outSz->stride;
                currentOutSz->width = w;
            }
            else
            {
                currentOutSz->width = inSz->width;
                currentOutSz->stride = inSz->stride;
                currentOutSz->height = h;
            }
        }
        else if (inSz->width != outSz->width)
        {
            currentOutSz->height = inSz->height;
            currentOutSz->stride = outSz->stride;
            currentOutSz->width = getnext(inSz->width, outSz->width);
        }
        else
        {
            currentOutSz->width = inSz->width;
            currentOutSz->stride = inSz->stride;
            currentOutSz->height = getnext(inSz->height, outSz->height);
        }

    }
    /*downsampling*/
    else if (inSz->height >= outSz->height && inSz->width >= outSz->width)
    {
        if ((inSz->width != outSz->width) && (inSz->height != outSz->height))
        {
            unsigned int h, w;
            h = getnext(inSz->height, outSz->height);
            w = getnext(inSz->width, outSz->width);
            /*Put 'bad' (not x2) resizers last*/
            if ((2 * h == inSz->height) || (2 * w != inSz->width))
            {
                currentOutSz->width = inSz->width;
                currentOutSz->stride = inSz->stride;
                currentOutSz->height = h;
            }
            else
            {
                currentOutSz->height = inSz->height;
                currentOutSz->stride = outSz->stride;
                currentOutSz->width = w;
            }
        }
        else if (inSz->height != outSz->height)
        {
            currentOutSz->width = inSz->width;
            currentOutSz->stride = inSz->stride;
            currentOutSz->height = getnext(inSz->height, outSz->height);
        }
        else
        {
            currentOutSz->height = inSz->height;
            currentOutSz->stride = outSz->stride;
            currentOutSz->width = getnext(inSz->width, outSz->width);
        }
    }
    /*Other possible cases*/
    else
    {
        if (inSz->height != outSz->height)
        {
            currentOutSz->width = inSz->width;
            currentOutSz->stride = inSz->stride;
            currentOutSz->height = getnext(inSz->height, outSz->height);
        }
        else
        {
            currentOutSz->height = inSz->height;
            currentOutSz->stride = outSz->stride;
            currentOutSz->width = getnext(inSz->width, outSz->width);
        }
    }
}

static const imgresize_method_api_t bicubic=
{
    "",
    imgresizer_bicubic_getApi,
    imgresizer_bicubic_alloc,
    imgresizer_bicubic_getScratchSize,
    imgresizer_bicubic_init,
    imgresizer_bicubic_process,
    imgresizer_bicubic_getnext,
    1
};

const imgresize_method_t imgresize_method_bicubic=(const imgresize_method_t)&bicubic;
