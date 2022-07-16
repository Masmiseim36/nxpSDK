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
#ifndef IMGRESIZER_COMMON_H__
#define IMGRESIZER_COMMON_H__
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"


typedef struct
{
    const char* name;
    /* returns size of coefficients */
    size_t (*getCoefSz)(const imgsize_t* in,const imgsize_t* out);
    /* returns coefficients */
    void (*getCoef)(void* coef, const imgsize_t* in,const imgsize_t* out);
    /* returns scratch size required for given transform */
    size_t (*getScratchSize)(const imgsize_t* in,const imgsize_t* out);
    /* in-place image resize */
    void (*process)(void *pScr, void* pCoef, const void* imgIn, void* imgOut, const imgsize_t* inSz, const imgsize_t* outSz, int fast);
}
imgresizer_api_t;

struct tag_imgresizer_t;

/* resizer method API */

typedef struct
{
    const char* name;
    /* returns specific API functions for resize from in to out */
    const imgresizer_api_t*  (*getApi)        (const imgsize_t* in,const imgsize_t* out);
    /* returns space required for allocation of given resizer */
    size_t                   (*alloc)         (const imgsize_t* in,const imgsize_t* out);
    /* returns scratch space required for allocation of given resizer */
    size_t                   (*getScratchSize)(const imgsize_t* in,const imgsize_t* out);
    /* initialize resizer with given memory space */
    struct tag_imgresizer_t* (*init)          (void* objmem,const imgsize_t* in,const imgsize_t* out);
    /* image processing by resizer */
    void(*process)(struct tag_imgresizer_t* pResizer, void *pScr, const void* imgIn, void* imgOut, int fast);
    /* 
        iterator returning image size for current transform
        Input:
        inSz - current input image size
        outSz - final image size
        Output:
        currentOutSz - output size for current intermediate transform

        Function iterates differently, depending on method. 
        For nearest method it iterates in vertical domain first.
    */
    void                     (*getnext)(imgsize_t* currentOutSz,const imgsize_t* inSz,const imgsize_t* outSz);
    /* Indicates if process for this method is in-place or not.
       Currently only nearest method supports not in-place process
    */
    int inplace;
}
imgresize_method_api_t;

#endif
