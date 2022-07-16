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
#ifndef IMGROTATE_COMMON_H__
#define IMGROTATE_COMMON_H__
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"
// magic numbers
#define IMGROTATE_GU8_MAGIC       0x3761432a
#define IMGFASTROTATE_GU8_MAGIC   0xa761432a
#define IMGROTATE_GS8_MAGIC       0x26A960F4
#define IMGFASTROTATE_GS8_MAGIC   0x26A960F4
#define IMGROTATE_GS16_MAGIC      0x3761432b
#define IMGFASTROTATE_GS16_MAGIC  0x3761432c

#define PADDING   3 // padding rows in pixels

#define ALG 0 /* Vshear: 0 - outerloop unroll = 2, 1 - outerloop unroll = 4, 2 - scalar code */

/*--------------------------------------------------------------------------
rotation by 0,90,180,270 with no format conversion
Input:
inImg, inSz  image and its size
outSz        output image size
q            rotation (0 - 0 degrees, 1 - 90 degrees, 2 - 180 degrees, 
             3 - 270 degrees conterclockwise
Output:
outImg       image
--------------------------------------------------------------------------*/
typedef void (*fnrotateQ)  (void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q);
/*--------------------------------------------------------------------------
rotation by 0,90,180,270 with format conversion
the image is placed beginning from the row PADDING and from the column 
ALIGNMENT/2. All paddings are written with black pixels
8-bit gs  -> signed 16-bit 1 pixel per 16-bit word
16-bit gs -> signed 16-bit 1 pixel per 16-bit word
RGB       -> signed 16-bit 1 pixel per 64-bit word
Input:
inImg, inSz  image and its size
outSz        output image size
q            rotation (0 - 0 degrees, 1 - 90 degrees, 2 - 180 degrees, 
             3 - 270 degrees conterclockwise
fillColor    fill color
Output:
outImg       image
--------------------------------------------------------------------------*/
typedef void (*fnrotateQ16)(void* pScr,void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q, int fillColor);
/*--------------------------------------------------------------------------
    horizontal inplace shear functions (16-bit signed data)
    the image is placed beginning from the row PADDING and from the column 
    ALIGNMENT/2. All paddings are written with black pixels
    Input:
    beta    shear scale, Q15
    Input/output
    inoutImg, inoutSz - image and its scale
    Temp:
    pScr - allocated by fngetScratchSize()
--------------------------------------------------------------------------*/
typedef void (*fnhshear)(void* pScr, void* inoutImg, imgsize_t * restrict inoutSz,int16_t beta, int fillColor);
/*--------------------------------------------------------------------------
    vertical inplace shear functions (16-bit signed data)
    the image is placed beginning from the row PADDING and from the column 
    ALIGNMENT/2. All paddings are written with black pixels
    Input:
    alpha    shear scale, Q15
    Input/output:
    inoutImg, inoutSz - image and its scale
    Temp:
    pScr - allocated by fngetScratchSize()
--------------------------------------------------------------------------*/
typedef void (*fnvshear)(void* pScr, void* inoutImg, imgsize_t * restrict inoutSz,int16_t alpha, int fillColor);
/*--------------------------------------------------------------------------
 out-of-place cropping with conversion from signed 16-bit to destination format
    Input:
    left,up    left/up corner
    inImg,inSz input image in intermediate format
    outSz      output size
    Output:
    outImg     output image
 --------------------------------------------------------------------------*/
typedef void (*fncrop)(void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int left, int up);

/* function returns scratch size required for internal functions (not including temp image) */
typedef size_t (*fngetScratchSize) (const imgrotate_params_t* params);

typedef struct
{
    imgsize_t outSz;    /* size/stride of output image                   */
    imgsize_t tmpSz;    /* size/stride of temporary image                */
    int16_t   alpha;    /* vertical shear coefficient, Q15 <=sqrt(2)-1   */
    int16_t   beta;     /* horizontal shear coefficient, Q15 <=sqrt(0.5) */
    int16_t   left;     /* left corner of centered temp image            */
    int16_t   up;       /* upper corner of centered temp image           */
}
tImgRotateCoef;

/* conversion API */
typedef struct
{
    fnrotateQ    rotq;
    fnrotateQ16  rotq16;
    fnvshear     vshear;
    fnhshear     hshear;
    fncrop       crop;
    fngetScratchSize getScratchSize;
    uint32_t         magic;             /* magic number for identification     */
    int              bpp;               /* bytes per pixel                     */
    int              fast;              /* 1 - fast, 0 - generic               */
    int              bppTmpImage;       /* bytes per pixel for temporary image */
}
tImgRotateApi;

typedef struct
{
    uint32_t              magic;  /* magic number specific for each image format */
    imgrotate_params_t    params; /* copy of parameters      */
    tImgRotateCoef        coef;   /* conversion coefficients */
    const tImgRotateApi*  pApi;   /* conversion functions    */
}
tImgRotate;


inline_ void imgrotate_params_validate(const imgrotate_params_t* params, int bpp, int fast)
{
    (void)bpp;
    (void)params;
    NASSERT(params);
    imgsize_validate(&params->in,bpp,fast);
}

/*--------------------------------------------------------------------------
  function compute coefficients according to the input image size and rotation angle
  bpp - bytes per pixel
  Input:
  params    image 
  bpp       bytes per pixel
  Output:
  coef      coefficients
--------------------------------------------------------------------------*/
void imgrotate_getCoef(tImgRotateCoef* coef, const imgrotate_params_t* params, int bpp, int fast);

/* Returns: size of memory to be allocated for an instance object, in bytes. */
inline_ size_t imgrotate_alloc  ( const imgrotate_params_t * params, const tImgRotateApi* pApi)
{
    imgrotate_params_validate(params,pApi->bpp,pApi->fast);
    return sizeof(tImgRotate)+ALIGNMENT-1;
}

/* Returns: handle to the object, or NULL if initialization failed. */
inline_ imgrotate_handle_t imgrotate_init  ( void * objmem, const imgrotate_params_t * params, const tImgRotateApi* pApi)
{
    tImgRotate *pImgRotate;
    imgrotate_params_validate(params,pApi->bpp,pApi->fast);
    NASSERT(objmem);
    pImgRotate = (tImgRotate *)((((uintptr_t)objmem)+(ALIGNMENT-1))&~(ALIGNMENT-1));
    pImgRotate->magic=pApi->magic;
    pImgRotate->params=*params;
    imgrotate_getCoef(&pImgRotate->coef,params,pApi->bpp,pApi->fast);
    pImgRotate->pApi=pApi;
    return (imgrotate_handle_t)pImgRotate;
}

/* return output image size */
inline_ void imgrotate_getOutSize  (imgsize_t * restrict outSz, const imgrotate_params_t * params, const tImgRotateApi* pApi)
{
    tImgRotateCoef coef;
    NASSERT(outSz);
    imgrotate_params_validate(params,pApi->bpp,pApi->fast);
    imgrotate_getCoef(&coef, params,pApi->bpp,pApi->fast);
    *outSz=coef.outSz;
}

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
void imgrotate_process  ( imgrotate_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgsize_t *outSz);

/* Returns: size of scratch memory area, in bytes. */
inline_ size_t imgrotate_getScratchSize  ( const imgrotate_params_t * params, const tImgRotateApi *pApi) 
{
    size_t szTmp,szScr;
    tImgRotateCoef coef;
    szScr=pApi->getScratchSize(params);
    szScr = (szScr+ALIGNMENT-1)&~(ALIGNMENT-1);
    imgrotate_getCoef(&coef,params,pApi->bpp,pApi->fast);
    szTmp=(size_t)coef.tmpSz.stride * (size_t)coef.tmpSz.height * pApi->bppTmpImage;   
    return szTmp+szScr;
}

/* 8-bit grayscale functions */
/*--------------------------------------------------------------------------
rotation by 0,90,180,270 with no format conversion
Input:
inImg, inSz  image and its size
outSz        output image size
q            rotation (0 - 0 degrees, 1 - 90 degrees, 2 - 180 degrees, 
             3 - 270 degrees conterclockwise
Output:
outImg       image
--------------------------------------------------------------------------*/
void imgfast_gu8_rotq  (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q);
void img_gu8_rotq      (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q);
void imgfast_gs8_rotq  (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q);
void img_gs8_rotq      (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q);
void imgfast_gs16_rotq (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q);
void img_gs16_rotq     (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q);

/*--------------------------------------------------------------------------
rotation by 0,90,180,270 with no format conversion
Input:
inImg, inSz  image and its size
outSz        output image size
Output:
outImg       image
--------------------------------------------------------------------------*/
void img_gu8_rot0  (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void img_gu8_rot90 (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void img_gu8_rot180(void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void img_gu8_rot270(void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);

void img_gs16_rot0  (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void img_gs16_rot90 (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void img_gs16_rot180(void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void img_gs16_rot270(void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);

void imgfast_gu8_rot0  (void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void imgfast_gu8_rot90 (void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void imgfast_gu8_rot180(void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void imgfast_gu8_rot270(void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);

void imgfast_gs16_rot0  (void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void imgfast_gs16_rot90 (void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void imgfast_gs16_rot180(void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
void imgfast_gs16_rot270(void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz);
/*--------------------------------------------------------------------------
rotation by 0,90,180,270 with format conversion
the image is placed beginning from the row PADDING and from the column 
ALIGNMENT/2. All paddings are written with black pixels
8-bit gs  -> signed 16-bit 1 pixel per 16-bit word
16-bit gs -> signed 16-bit 1 pixel per 16-bit word
RGB       -> signed 16-bit 1 pixel per 64-bit word
Input:
inImg, inSz  image and its size
outSz        output image size
q            rotation (0 - 0 degrees, 1 - 90 degrees, 2 - 180 degrees, 
             3 - 270 degrees conterclockwise
fillColor    filling color
Output:
outImg       image
--------------------------------------------------------------------------*/
void imgfast_gu8_rotq16 (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q, int fillColor);
void img_gu8_rotq16     (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q, int fillColor);
void imgfast_gs8_rotq16 (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q, int fillColor);
void img_gs8_rotq16     (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q, int fillColor);
void imgfast_gs16_rotq16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q, int fillColor);
void img_gs16_rotq16    (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q, int fillColor);

/*--------------------------------------------------------------------------
rotation by 0,90,180,270 conterclockwise with format conversion
the image is placed beginning from the row PADDING and from the column
ALIGNMENT/2. All paddings are written with black pixels
8-bit gs  -> signed 16-bit 1 pixel per 16-bit word
16-bit gs -> signed 16-bit 1 pixel per 16-bit word
RGB       -> signed 16-bit 1 pixel per 64-bit word
Input:
inImg, inSz  image and its size
outSz        output image size
fillColor    filling color
Output:
outImg       image
--------------------------------------------------------------------------*/
void imgfast_gs16_rot0_16  (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void imgfast_gs16_rot90_16 (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void imgfast_gs16_rot180_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void imgfast_gs16_rot270_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);

void img_gs16_rot0_16  (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void img_gs16_rot90_16 (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void img_gs16_rot180_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void img_gs16_rot270_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);

void imgfast_gu8_rot0_16  (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void imgfast_gu8_rot90_16 (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void imgfast_gu8_rot180_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void imgfast_gu8_rot270_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);

void img_gu8_rot0_16  (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void img_gu8_rot90_16 (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void img_gu8_rot180_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void img_gu8_rot270_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);

void imgfast_gs8_rot0_16  (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void imgfast_gs8_rot90_16 (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void imgfast_gs8_rot180_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void imgfast_gs8_rot270_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);

void img_gs8_rot0_16  (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void img_gs8_rot90_16 (void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void img_gs8_rot180_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
void img_gs8_rot270_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor);
/*--------------------------------------------------------------------------
    horizontal inplace shear functions (16-bit signed data)
    the image is placed beginning from the row PADDING and from the column 
    ALIGNMENT/2. All paddings are written with black pixels
    Input:
    beta    shear scale, Q15
    fillColor filling color
    Input/output
    inoutImg, inoutSz - image and its scale
--------------------------------------------------------------------------*/
void img_gs_hshear(void* pScr, void* inoutImg, imgsize_t * restrict inoutSz,int16_t beta, int fillColor);

/*--------------------------------------------------------------------------
    vertical inplace shear functions (16-bit signed data)
    the image is placed beginning from the row PADDING and from the column 
    ALIGNMENT/2. All paddings are written with black pixels
    Input:
    alpha    shear scale, Q15
    Input/output:
    inoutImg, inoutSz - image and its scale
--------------------------------------------------------------------------*/
void img_gs_vshear(void* pScr, void* inoutImg, imgsize_t * restrict inoutSz,int16_t alpha, int fillColor);

/*--------------------------------------------------------------------------
 out-of-place cropping with conversion from signed 16-bit to destination format
    Input:
    left,up    left/up corner
    inImg,inSz input image in intermediate format
    outSz      output size
    Output:
    outImg     output image
 --------------------------------------------------------------------------*/
void img_gu8_crop     (void *pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int left, int up);
void imgfast_gu8_crop (void *pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int left, int up);
void img_gs8_crop     (void *pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int left, int up);
void imgfast_gs8_crop (void *pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int left, int up);
void img_gs16_crop    (void *pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int left, int up);
void imgfast_gs16_crop(void *pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int left, int up);

#endif//IMGROTATE_COMMON_H__
