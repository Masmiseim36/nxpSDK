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
#include "imgresize_common.h"
#include "imgcopy_common.h"
#include "imgresizer_common.h"

#define MAX(a,b)        ((a)>(b) ? (a) : (b))
#define MIN(a,b)        ((a)<(b) ? (a) : (b))

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define IMAGE_RESIZE_MAGIC  0x536ad232

typedef struct tag_imgresizer_list_t
{
struct tag_imgresizer_t     * obj;
struct tag_imgresizer_list_t* next;
}
imgresizer_list_t;

#define LIST_ALIGNMENT ((int)sizeof(void *))


/*-------------------------------------------------------------------------
  Image resize
  Resize process is divided onto initialization and processing phases. On the 
  initialization, the function returns the size of memory requesting for 
  persistent data (i.e. coefficients) needed for dowsampling/upsampling 
  operations. In the processing phase, input image is converted to the output 
  image according to the transformation parameters. Additionally, image resize
  requires some amount of temporary (scratch) data which can not be kept between 
  successive calls of imgresize_process() function and might be reused 
  
  Image formats:
  gs     8-bit grayscale data
  gs16   16-bit grayscale data

  Input
  objmem      Memory block allocated for the instance object:
  params      Parameters of image resize operation
  inImage     input image
  Output:
  outImage    output image
  Temporary:
  pScr        Scratch memory area for the processing function. To 
              determine the scratch area size, use the respective
              helper function: imgresize_getScratchSize()
  Restrictions:
  1. see general restrictions applied for all images for fast/generic 
     functions
  2. pointer to the scratch memory should be aligned on 8-byte boundary
-------------------------------------------------------------------------*/
typedef struct
{
    uint32_t                   magic;
    imgresize_params_t         params;
    imgresizer_list_t*         list;
    void*                      tempImage;
    imgsize_t                  tempImageSz;
    const imgcopy_api*         fncopy_input;
    const imgcopy_api*         fncopy_output;
    int                        fast;
}
imgresize_t;

// returns size of temporary (intermediate) image
inline_ void imgresize_getTempImageSz(imgsize_t* sz, const imgresize_params_t * params)
{
    sz->width =MAX(params->in.width ,params->out.width );
    sz->height=MAX(params->in.height,params->out.height);
    //sz->stride=(sz->width + (ALIGNMENT/2-1))&~(ALIGNMENT/2-1);  // 16-bit aligned image
    sz->stride = (sz->width + (ALIGNMENT - 1))&~(ALIGNMENT - 1);  // 16-bit aligned image
}

// allocation routine
size_t imgresize_alloc      ( const imgresize_params_t * params )
{
    imgresize_method_api_t* method=(imgresize_method_api_t*)params->method;
    imgsize_t tmpSz,inSz,outSz;
    size_t sz;
    // allocate head structure
    sz=ALIGNED_SIZE(sizeof(imgresize_t), ALIGNMENT);
    // allocate temp image
    imgresize_getTempImageSz(&tmpSz,params);
//    sz+=ALIGNED_SIZE(tmpSz.height*tmpSz.stride*sizeof(int16_t), ALIGNMENT);
    // allocate resizers
    inSz=params->in; inSz.stride=tmpSz.stride;
    outSz=params->out; outSz.stride=tmpSz.stride;
    while(inSz.width!=outSz.width || inSz.height!=outSz.height)
    {
        method->getnext(&tmpSz,&inSz,&outSz);
        sz+=ALIGNED_SIZE(method->alloc(&inSz,&tmpSz)+sizeof(imgresizer_list_t), LIST_ALIGNMENT);
        inSz=tmpSz;
    }
    return sz;
}

// general imitialization
imgresize_handle_t imgresize_init ( void * objmem, const imgresize_params_t * params, 
                                    const imgcopy_api* fncopy_input,const imgcopy_api* fncopy_output, int fast)
{
    imgresize_method_api_t* method=(imgresize_method_api_t*)params->method;
    imgsize_t tmpSz,inSz,outSz;
    imgresizer_list_t * prev;
    imgresize_t* pResize;
    /* allocate subobjects */
    void *a=objmem;
    size_t sz;
    NASSERT(objmem);

    /* allocate the head structure */
    pResize=(imgresize_t*)ALIGNED_ADDR(a, ALIGNMENT);
    a = pResize + 1;
    /* allocate the temporary image */
    #if 0
    pResize->tempImage=(void*)ALIGNED_ADDR(a, ALIGNMENT);
    imgresize_getTempImageSz(&pResize->tempImageSz,params);
    a = (void *)((uintptr_t)(pResize->tempImage) + pResize->tempImageSz.height*pResize->tempImageSz.stride*sizeof(int16_t));
    #endif
    imgresize_getTempImageSz(&pResize->tempImageSz, params);
    /* compute, allocate and initialize needed resizers */
    tmpSz=pResize->tempImageSz;
    inSz=params->in; inSz.stride=tmpSz.stride;
    outSz=params->out; outSz.stride=tmpSz.stride;
    prev=pResize->list=NULL;
    int count = 0;
    /* calculate list's length */
    while (inSz.width != outSz.width || inSz.height != outSz.height)
    {
        /* get next resizer */
        method->getnext(&tmpSz, &inSz, &outSz);
        inSz = tmpSz;
        count++;
    }
    tmpSz = pResize->tempImageSz;
    inSz = params->in; inSz.stride = tmpSz.stride;
    outSz = params->out; outSz.stride = tmpSz.stride;
    pResize->fast = fast;
    if ((method->inplace == 0) && count == 2)
    {
        inSz = params->in;
        outSz = params->out;
    }
    if ((method->inplace == 0) && count == 1)
    {
        inSz = params->in;
        outSz = params->out;
        tmpSz = outSz;
    }
    while(inSz.width!=outSz.width || inSz.height!=outSz.height)
    {
        struct tag_imgresizer_t *resizer;
        imgresizer_list_t *list;
        list=(imgresizer_list_t *)ALIGNED_ADDR(a, LIST_ALIGNMENT);
        a = list + 1;

        /* get next resizer */
        method->getnext(&tmpSz,&inSz,&outSz);
        sz=method->alloc(&inSz,&tmpSz);
        resizer=method->init((void*)a,&inSz,&tmpSz);
        /* insert the resizer into the list */
        list->obj=resizer;
        list->next=NULL;
        if (prev) prev->next=list;
        else      pResize->list=list;
        prev=list;
        inSz=tmpSz;

        a = (void *)((uintptr_t)a + sz);
    }
    pResize->magic=IMAGE_RESIZE_MAGIC;
    pResize->params=*params;
    pResize->fncopy_input=fncopy_input;
    pResize->fncopy_output=fncopy_output;
    return (imgresize_handle_t)pResize;
}

// general processing function 
void imgresize_process ( imgresize_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg)
{
    void* tempImg;
    void *a;
    imgresize_t* pResize=(imgresize_t*)handle;
    imgresizer_list_t *list=pResize->list;
    imgresize_method_api_t* method=(imgresize_method_api_t*)pResize->params.method;
    NASSERT(pResize);
    NASSERT(pResize->magic==IMAGE_RESIZE_MAGIC);
    a = pScr;
    pResize->tempImage = (void*)ALIGNED_ADDR(a, ALIGNMENT);
    imgresize_getTempImageSz(&pResize->tempImageSz, &pResize->params);
    a = (void *)((uintptr_t)(pResize->tempImage) + pResize->tempImageSz.height*pResize->tempImageSz.stride*sizeof(int16_t));
    pScr = a;
    tempImg=pResize->tempImage;
    int sz = 0;
    while (list)
    {
        sz++;
        list = list->next;
    }
    list = pResize->list;
    if ((method->inplace == 1) || (sz == 0))
    {
        pResize->fncopy_input->copy(pScr, tempImg, inImg, &pResize->params.in, &pResize->tempImageSz);

        while (list)
        {
            method->process(list->obj, pScr, tempImg, tempImg, pResize->fast);
            list = list->next;
        }
        pResize->fncopy_output->copy(pScr, outImg, tempImg, &pResize->tempImageSz, &pResize->params.out);
    }
    else if (sz==2)
    {
        method->process(list->obj, pScr, inImg, tempImg, pResize->fast);
        list = list->next;
        method->process(list->obj, pScr, tempImg, outImg, pResize->fast);
    }
    else
    {
        method->process(list->obj, pScr, inImg, outImg, pResize->fast);
    }
}

// general scratch size 
size_t imgresize_getScratchSize( const imgresize_params_t * params,const imgcopy_api* fncopy_input,const imgcopy_api* fncopy_output)
{
    imgresize_method_api_t* method=(imgresize_method_api_t*)params->method;
    size_t sz, sz1, sz2, sz_tmp;
    imgsize_t inSz,outSz,tmpSz;
    imgresize_getTempImageSz(&tmpSz, params);
    sz_tmp = ALIGNED_SIZE(tmpSz.height*tmpSz.stride*sizeof(int16_t), ALIGNMENT);
    sz1=fncopy_input->getScratchSize(&params->in,&tmpSz);
    sz2=fncopy_output->getScratchSize(&tmpSz,&params->out);
    sz=MAX(sz1,sz2);
    inSz=params->in; inSz.stride=tmpSz.stride;
    outSz=params->out; outSz.stride=tmpSz.stride;
    while(inSz.width!=outSz.width || inSz.height!=outSz.height)
    {
        method->getnext(&tmpSz,&inSz,&outSz);
        sz1=method->getScratchSize(&inSz,&tmpSz);
        inSz=tmpSz;
        sz=MAX(sz1,sz);
    }
    return sz + sz_tmp;
}
