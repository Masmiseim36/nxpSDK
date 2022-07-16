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
#ifndef IMGPAD_COMMON_H__
#define IMGPAD_COMMON_H__ 1
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"
#include "imgpad_common.h"


typedef struct
{
    int bpp;    // bytes per pixel
    int fast;   // true for fast variant
    void (*copyLeftRightBorders)(void *left, void *right, const void * restrict inImg, const imgpad_params_t* params);
    void (*copyUpBtmBorders)    (void *left, void *right, const void * restrict inImg, const imgpad_params_t* params);
    void (*copyVert)            (void* outImg, const void* edge, int w, int h, int ostride);
    void (*copyHorz)            (void* outImg, const void* edge, int w, int h, int ostride);
    void (*subcopy)             (void* outImg, const void* inImg, int width,int height, int istride, int ostride);
}
imgpad_api;


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
void imgcopyLeftRightBorders_gu8(void *left, void *right, const void * restrict inImg, const imgpad_params_t* params);
void imgcopyLeftRightBorders_gs8(void *left, void *right, const void * restrict inImg, const imgpad_params_t* params);
void imgcopyLeftRightBorders_gs16(void *left, void *right, const void * restrict inImg, const imgpad_params_t* params);
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
void imgcopyUpBtmBorders_gu8(void *up, void *btm, const void * restrict inImg, const imgpad_params_t* params);
void imgcopyUpBtmBorders_gs8(void *up, void *btm, const void * restrict inImg, const imgpad_params_t* params);
void imgcopyUpBtmBorders_gs16(void *up, void *btm, const void * restrict inImg, const imgpad_params_t* params);
/*------------------------------------------------
    Functon copies vertical edges to rectange
    Input:
    edge[h] - vertical edge 
    w,h,ostride - copied region
    Output:
    outImg   - output image
------------------------------------------------*/
void imgcopyVert_gu8(void* outImg, const void* edge, int w, int h, int ostride);
void imgcopyVert_gs8(void* outImg, const void* edge, int w, int h, int ostride);
void imgcopyVert_gs16(void* outImg, const void* edge, int w, int h, int ostride);
/*------------------------------------------------
    Functon copies horizontal edges to rectange
    Input:
    edge[h] - vertical edge 
    w,h,ostride - copied region
    Output:
    outImg   - output image
------------------------------------------------*/
void imgcopyHorz_gu8(void* outImg, const void* edge, int w, int h, int ostride);
void imgcopyHorz_gs8(void* outImg, const void* edge, int w, int h, int ostride);
void imgcopyHorz_gs16(void* restrict outImg, const void* edge, int w, int h, int ostride);
/*------------------------------------------------
    Functon copies one subimage to another subimage
    Input:
    in - input image
    width,height - size
    istride - stride of input image
    ostride - stride of output image
    Output:
    out - output image
------------------------------------------------*/
void imgsubcopy_gu8(void* outImg, const void* inImg, int width,int height, int istride, int ostride);
void imgsubcopy_gs8(void* outImg, const void* inImg, int width, int height, int istride, int ostride);
void imgsubcopy_gs16(void* outImg, const void* inImg, int width,int height, int istride, int ostride);

/* returns if it is true cropping in horizontal/vertical directions */
inline_ int imgpad_isCropHorz(const imgpad_params_t *params)
{
    int win=params->in.width;
    int wout=params->out.width;
    int x=params->x;
    return x<0 && x+win>wout;
}
inline_ int imgpad_isCropVert(const imgpad_params_t *params)
{
    int hin=params->in.height;
    int hout=params->out.height;
    int y=params->y;
    return y<0 && y+hin>hout;
}

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
void imgpad_process ( void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgpad_params_t* params, const imgpad_api *api );
/* Returns: size of scratch memory area, in bytes. */
size_t imgpad_getScratchSize  ( const imgpad_params_t * params, const imgpad_api *api );

#endif
