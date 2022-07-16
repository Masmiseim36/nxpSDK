/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
#ifndef __NATUREDSP_SIGNAL_IMG_H__
#define __NATUREDSP_SIGNAL_IMG_H__

/* Portable data types. */
#include "NatureDSP_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
    Image processing functions:
    Image scaling  (downscaling and upscaling from input_size to output_size)
    Image rotation
    Image normalization
    Computing histograms of an input_size image

    Image formats:
    gu8    8-bit unsigned grayscale data
    gs8    8-bit signed grayscale data
    gs16   16-bit signed grayscale data (all negative values are black)

     All images are represented by width, height and stride between successive 
     rows. Pixel order is from left to right and from upside to downside. 
     For performance reasons, all functions are divided into 2 categories:
     - generic functions which process images with arbitrary parameters
     - 'fast' functions which assume that the pointer for each row is 
       aligned on 8-byte boundary. This also means that the pointer to the 
       image is aligned and stride is a multiple of 8 for 8-bit images and 
       multiple of 4 for 16-bit images.

===========================================================================*/

/* image size:
   For 'fast' versions images are representing as an array of rows with stride
   providing alignment of each row
   1. minimum width/height is 8 pixels
   2. all the images should be aligned on 8-byte boundary
   3. stride should be a multiple of 8 for 8-bit grayscale, 4 for 16-bit 
      grayscale
*/
typedef struct imgsize_tag
{
    unsigned int width;   /* width of image   */
    unsigned int height;  /* height of image  */
    unsigned int stride;  /* stride between consecutive rows, in pixels */
} imgsize_t;

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
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
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
typedef void * imgresize_method_t;
extern const imgresize_method_t imgresize_method_nearest;  //  nearest
extern const imgresize_method_t imgresize_method_bilinear; //  bilinear (multistage)
extern const imgresize_method_t imgresize_method_bicubic ; //  bicubic  (multistage)

/* Parameters of image resize operation */
typedef struct imgresize_params_tag
{
    imgsize_t          in;      /* input image sizes  */
    imgsize_t          out;     /* output image sizes */
    imgresize_method_t method;  /* method             */ 
} imgresize_params_t;

typedef void * imgresize_handle_t;

/* Returns: size of memory to be allocated for an instance object, in bytes. */
size_t imgresize_gu8_alloc     ( const imgresize_params_t * params );
size_t imgresize_gs8_alloc     ( const imgresize_params_t * params );
size_t imgresize_gs16_alloc    ( const imgresize_params_t * params );
size_t imgfastresize_gu8_alloc ( const imgresize_params_t * params );
size_t imgfastresize_gs8_alloc ( const imgresize_params_t * params );
size_t imgfastresize_gs16_alloc( const imgresize_params_t * params );

/* Returns: handle to the object, or NULL if initialization failed. */
imgresize_handle_t imgresize_gu8_init     ( void * objmem, const imgresize_params_t * params );
imgresize_handle_t imgresize_gs8_init     ( void * objmem, const imgresize_params_t * params );
imgresize_handle_t imgresize_gs16_init    ( void * objmem, const imgresize_params_t * params );
imgresize_handle_t imgfastresize_gu8_init ( void * objmem, const imgresize_params_t * params );
imgresize_handle_t imgfastresize_gs8_init ( void * objmem, const imgresize_params_t * params );
imgresize_handle_t imgfastresize_gs16_init( void * objmem, const imgresize_params_t * params );

/* resize images */ 
void imgresize_gu8_process     ( imgresize_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg);
void imgresize_gs8_process     ( imgresize_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg);
void imgresize_gs16_process    ( imgresize_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg);
void imgfastresize_gu8_process ( imgresize_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg);
void imgfastresize_gs8_process ( imgresize_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg);
void imgfastresize_gs16_process( imgresize_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg);

/* Returns: size of scratch memory area, in bytes. */
size_t imgresize_gu8_getScratchSize     ( const imgresize_params_t * params );
size_t imgresize_gs8_getScratchSize     ( const imgresize_params_t * params );
size_t imgresize_gs16_getScratchSize    ( const imgresize_params_t * params );
size_t imgfastresize_gu8_getScratchSize ( const imgresize_params_t * params );
size_t imgfastresize_gs8_getScratchSize ( const imgresize_params_t * params );
size_t imgfastresize_gs16_getScratchSize( const imgresize_params_t * params );

/*-------------------------------------------------------------------------
  Image rotate
  Image rotation function rotates input image counter clockwise about their 
  center
  Rotation following algorithms:
  - dedicated variants for 90, 180 and -90 degree rotations
  - for small angles in range -pi/4...pi/4 - 3-shear steps
  - for another angles - proper combinations of above variants. 
  Since all of these algorithms show very different computational 
  speed, overall transformation performance depends not only from
  the image size, but from the rotation angle as well.
  The size of resulted image depends on the angle and all unused portions 
  of image will be filled with fill color. Required output size will be 
  requested by getOutSize API. User should use this size as an input for 
  process function, but may select equal or bigger stride between image 
  rows
  
  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
  gs16   16-bit grayscale data

  Input
  objmem      Memory block allocated for the instance object:
  params      Parameters of image rotate operation
  inImage     input image

  Output:
  outImage    output image
  outSz       resulted size of output image 

  Temporary:
  pScr        Scratch memory area. To determine the scratch area size, 
              use the respective helper function: imgrotate_xxx_getScratchSize()
  Restrictions:
  1. see general restrictions applied for all images for fast/generic 
     functions
  2. pointer to the scratch memory should be aligned on 8-byte boundary
-------------------------------------------------------------------------*/

/* Parameters of image rotate operation */
typedef struct imgrotate_params_tag
{
    imgsize_t in;       /* input image sizes  */
    int16_t   angleQ15;    /* angle scaled by pi, counterclockwise, in Q15, 
                           (so 90 degrees counter clockwise corresponds to 
                           0x4000, 180 degrees - 0x8000, 270 degrees -
                           0xc000)
                        */
    int fill;           /* fill color for unused image parts */
} imgrotate_params_t;

typedef void * imgrotate_handle_t;

/* Returns: size of memory to be allocated for an instance object, in bytes. */
size_t imgrotate_gu8_alloc     ( const imgrotate_params_t * params );
size_t imgfastrotate_gu8_alloc ( const imgrotate_params_t * params );
size_t imgrotate_gs8_alloc     ( const imgrotate_params_t * params );
size_t imgfastrotate_gs8_alloc ( const imgrotate_params_t * params );
size_t imgrotate_gs16_alloc    ( const imgrotate_params_t * params );
size_t imgfastrotate_gs16_alloc( const imgrotate_params_t * params );

/* Returns: handle to the object, or NULL if initialization failed. */
imgrotate_handle_t imgrotate_gu8_init     ( void * objmem, const imgrotate_params_t * params );
imgrotate_handle_t imgfastrotate_gu8_init ( void * objmem, const imgrotate_params_t * params );
imgrotate_handle_t imgrotate_gs8_init     ( void * objmem, const imgrotate_params_t * params );
imgrotate_handle_t imgfastrotate_gs8_init ( void * objmem, const imgrotate_params_t * params );
imgrotate_handle_t imgrotate_gs16_init    ( void * objmem, const imgrotate_params_t * params );
imgrotate_handle_t imgfastrotate_gs16_init( void * objmem, const imgrotate_params_t * params );

/* return output image size */
void imgrotate_gu8_getOutSize     (imgsize_t * restrict outSz, const imgrotate_params_t * params);
void imgfastrotate_gu8_getOutSize (imgsize_t * restrict outSz, const imgrotate_params_t * params);
void imgrotate_gs8_getOutSize     (imgsize_t * restrict outSz, const imgrotate_params_t * params);
void imgfastrotate_gs8_getOutSize (imgsize_t * restrict outSz, const imgrotate_params_t * params);
void imgrotate_gs16_getOutSize    (imgsize_t * restrict outSz, const imgrotate_params_t * params);
void imgfastrotate_gs16_getOutSize(imgsize_t * restrict outSz, const imgrotate_params_t * params);

/* resize images */ 
void imgrotate_gu8_process     ( imgrotate_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgsize_t * restrict outSz);
void imgfastrotate_gu8_process ( imgrotate_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgsize_t * restrict outSz);
void imgrotate_gs8_process     ( imgrotate_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgsize_t * restrict outSz);
void imgfastrotate_gs8_process ( imgrotate_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgsize_t * restrict outSz);
void imgrotate_gs16_process    ( imgrotate_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgsize_t * restrict outSz);
void imgfastrotate_gs16_process( imgrotate_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgsize_t * restrict outSz);

/* Returns: size of scratch memory area, in bytes. */
size_t imgrotate_gu8_getScratchSize     ( const imgrotate_params_t * params );
size_t imgfastrotate_gu8_getScratchSize ( const imgrotate_params_t * params );
size_t imgrotate_gs8_getScratchSize     ( const imgrotate_params_t * params );
size_t imgfastrotate_gs8_getScratchSize ( const imgrotate_params_t * params );
size_t imgrotate_gs16_getScratchSize    ( const imgrotate_params_t * params );
size_t imgfastrotate_gs16_getScratchSize( const imgrotate_params_t * params );

/*-------------------------------------------------------------------------
  Image normalization
  Function normalize the intensity of pixels to the given range

  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
  gs16   16-bit grayscale data

  Input:
  inImg   input image
  sz      image size
  minInt  min intensity on output (for linear normalization)
  maxInt  max intensity on output (for non-linear normalization)
  tbl[64] tabulated values (for non-linear normalization)
  Input:
  outImg   input image

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imgnorm_gu8     ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, int minInt, int maxInt);
void imgfastnorm_gu8 ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, int minInt, int maxInt);
void imgnorm_gs8     ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, int minInt, int maxInt);
void imgfastnorm_gs8 ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, int minInt, int maxInt);
void imgnorm_gs16    ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, int minInt, int maxInt);
void imgfastnorm_gs16( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, int minInt, int maxInt);
void imgnorm_gu8_nonlinear     ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, const int16_t * tbl);
void imgfastnorm_gu8_nonlinear ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, const int16_t * tbl);
void imgnorm_gs8_nonlinear     ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, const int16_t * tbl);
void imgfastnorm_gs8_nonlinear ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, const int16_t * tbl);
void imgnorm_gs16_nonlinear    ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, const int16_t * tbl);
void imgfastnorm_gs16_nonlinear( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, const int16_t * tbl);

/*------------------------------------------------------------------------------
  Histogram of image 
  Functions compute the histogram of input image. First bin corresponds minimum
  intensity (0 for signed 16-bit and unsigned 8-bit data, -128 for signed 8-bit)
  and the last bin corresponds to the maximum intensity (32767 for signed 16-bit,
  255 for unsigned 8-bit data, 127  for signed 8-bit data)

  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
  gs16   16-bit grayscale data

  Input:
  inImg   input image
  sz      image size
  M       number of bins
  res->h  pointer to the histogram array
  Output:
  res[1]  histogram values, mean and variance

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
--------------------------------------------------------------------------------*/
typedef struct
{
    int32_t  mean;   /* mean,     Q15.16 */
    int32_t  var;    /* variance, Q31.0  */
    int      M;      /* number of bins   */
    int32_t* h;      /* histogram values */
}
imghist_t;

void imghist_gu8      ( imghist_t * restrict res, const void * restrict inImg, const imgsize_t* sz, int M);
void imgfasthist_gu8  ( imghist_t * restrict res, const void * restrict inImg, const imgsize_t* sz, int M);
void imghist_gs8      ( imghist_t * restrict res, const void * restrict inImg, const imgsize_t* sz, int M);
void imgfasthist_gs8  ( imghist_t * restrict res, const void * restrict inImg, const imgsize_t* sz, int M);
void imghist_gs16     ( imghist_t * restrict res, const void * restrict inImg, const imgsize_t* sz, int M);
void imgfasthist_gs16 ( imghist_t * restrict res, const void * restrict inImg, const imgsize_t* sz, int M);

/*-------------------------------------------------------------------------
  RGB<->YUV conversion 
  Functions convert RGB planar images to YUV planar. 

  Y=c0*R+c1*G+c2*B
  U=c3*R+c4*G+c5*B + bias
  V=c6*R+c7*G+c8*B + bias
  where bias is 128 for 8-bit data and 16384 for 16-bit data

  Image formats:
  8-bit unsigned RGB/YUV
  16-bit signed RGB/YUV

  Input:
  inImgR,
  inImgG,
  inImgB  planes with R,G,B components
  c[13]   transform coefficients, Q29 format
  sz      image size
  Output:
  outImgY
  outImgU
  outImgV planes with Y,U,V components

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imgconvert_rgbyuv( void * restrict outImgY, 
                        void * restrict outImgU, 
                        void * restrict outImgV, 
                        const void * restrict inImgR, 
                        const void * restrict inImgG, 
                        const void * restrict inImgB, 
                        const int32_t * c,
                        const imgsize_t* sz);
void imgconvert_rgbyuv16( void * restrict outImgY, 
                          void * restrict outImgU, 
                          void * restrict outImgV, 
                          const void * restrict inImgR, 
                          const void * restrict inImgG, 
                          const void * restrict inImgB, 
                          const int32_t * c,
                          const imgsize_t* sz);
void imgfastconvert_rgbyuv
                      ( void * restrict outImgY, 
                        void * restrict outImgU, 
                        void * restrict outImgV, 
                        const void * restrict inImgR, 
                        const void * restrict inImgG, 
                        const void * restrict inImgB, 
                        const int32_t * c,
                        const imgsize_t* sz);
void imgfastconvert_rgbyuv16
                      ( void * restrict outImgY, 
                        void * restrict outImgU, 
                        void * restrict outImgV, 
                        const void * restrict inImgR, 
                        const void * restrict inImgG, 
                        const void * restrict inImgB, 
                        const int32_t * c,
                        const imgsize_t* sz);

/*-------------------------------------------------------------------------
  RGB<->YUV conversion 
  Functions convert YUV (BT.601) planar images to RGB planar 

  R=Y+c9*(V-bias)
  G=Y+c10*(U-bias)+c11*(V-bias)
  B=Y+c12*(U-bias)
  where bias is 128 for 8-bit data and 16384 for 16-bit data

  Image formats:
  8-bit unsigned RGB/YUV
  16-bit signed RGB/YUV

  Input:
  inImgY,
  inImgU,
  inImgV  planes with Y,U,V components
  c[13]   transform coefficients, Q29 format
  sz      image size
  Output:
  outImgR
  outImgG
  outImgB planes with R,G,B components

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imgconvert_yuvrgb( void * restrict outImgR, 
                        void * restrict outImgG, 
                        void * restrict outImgB, 
                        const void * restrict inImgY, 
                        const void * restrict inImgU, 
                        const void * restrict inImgV, 
                        const int32_t * c,
                        const imgsize_t* sz);
void imgconvert_yuvrgb16( void * restrict outImgR, 
                        void * restrict outImgG, 
                        void * restrict outImgB, 
                        const void * restrict inImgY, 
                        const void * restrict inImgU, 
                        const void * restrict inImgV, 
                        const int32_t * c,
                        const imgsize_t* sz);
void imgfastconvert_yuvrgb
                      ( void * restrict outImgR, 
                        void * restrict outImgG, 
                        void * restrict outImgB, 
                        const void * restrict inImgY, 
                        const void * restrict inImgU, 
                        const void * restrict inImgV, 
                        const int32_t * c,
                        const imgsize_t* sz);
void imgfastconvert_yuvrgb16
                      ( void * restrict outImgR, 
                        void * restrict outImgG, 
                        void * restrict outImgB, 
                        const void * restrict inImgY, 
                        const void * restrict inImgU, 
                        const void * restrict inImgV, 
                        const int32_t * c,
                        const imgsize_t* sz);

/*-------------------------------------------------------------------------
  Image deinterleave
  Functions convert packed images to planar format 

  Image formats:
  8-bit signed or unsigned data
  16-bit signed data

  Input:
  inImg   packed image (RGB come together)
  sz      image size
  Output:
  outImgR
  outImgG
  outImgB planes with R,G,B components

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imgdeinterleave  (    void * restrict outImgR, 
                              void * restrict outImgG, 
                              void * restrict outImgB, 
                        const void * restrict inImg, 
                        const imgsize_t* sz);
void imgdeinterleave16(    void * restrict outImgR, 
                              void * restrict outImgG, 
                              void * restrict outImgB, 
                        const void * restrict inImg, 
                        const imgsize_t* sz);
void imgfastdeinterleave
                      (       void * restrict outImgR, 
                              void * restrict outImgG, 
                              void * restrict outImgB, 
                        const void * restrict inImg, 
                        const imgsize_t* sz);
void imgfastdeinterleave16
                      (       void * restrict outImgR, 
                              void * restrict outImgG, 
                              void * restrict outImgB, 
                        const void * restrict inImg, 
                        const imgsize_t* sz);
/*-------------------------------------------------------------------------
  Image interleave
  Functions convert planar images to packed format 

  Image formats:
  8-bit signed or unsigned data
  16-bit signed data

  Input:
  inImgR
  inImgG
  inImgB  planes with R,G,B components
  sz      image size
  Output:
  outImg  packed image (RGB come together)

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imginterleave (      void * restrict outImg, 
                       const void * restrict inImgR, 
                       const void * restrict inImgG, 
                       const void * restrict inImgB, 
                       const imgsize_t* sz);
void imginterleave16 (    void * restrict outImg, 
                       const void * restrict inImgR, 
                       const void * restrict inImgG, 
                       const void * restrict inImgB, 
                       const imgsize_t* sz);
void imgfastinterleave(   void * restrict outImg, 
                       const void * restrict inImgR, 
                       const void * restrict inImgG, 
                       const void * restrict inImgB, 
                       const imgsize_t* sz);
void imgfastinterleave16( void * restrict outImg, 
                       const void * restrict inImgR, 
                       const void * restrict inImgG, 
                       const void * restrict inImgB, 
                       const imgsize_t* sz);
/*-------------------------------------------------------------------------
  Image cropping/padding
  Padding function makes left/right/top/bottom padding for input image
  It copies input image to the output at specific position and fills
  borders either with specified color or by duplication of pixels from
  corresponding edge. 
  Region to be copies is set by position of left/upper corner of input
  in the coordinates of output image. If this region is outside the 
  boundaries of output image, then function will crop required region 
  from the input with padding of unused regions
  
  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
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
/* Parameters of image padding operation */
typedef struct imgpad_params_tag
{
    imgsize_t   in;    /* input image size                            */
    imgsize_t   out;   /* output image size                           */
    int         x,y;   /* position of left upper corner of input image 
                          on the output 
                          if positions of input image corners are 
                          outside the output image, function will crop
                          required region from the input
                       */
    int         fill;  /* fill color or special constant IMGPAG_EDGE to use edge pixels */
} imgpad_params_t;

#define IMGPAD_EDGE -777777

/* pad images */ 
void imgpad_gu8     ( void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgpad_params_t* params );
void imgpad_gs8     ( void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgpad_params_t* params );
void imgpad_gs16    ( void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgpad_params_t* params );
void imgfastpad_gu8 ( void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgpad_params_t* params );
void imgfastpad_gs8 ( void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgpad_params_t* params );
void imgfastpad_gs16( void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgpad_params_t* params );

/* Returns: size of scratch memory area, in bytes. */
size_t imgpad_gu8_getScratchSize          ( const imgpad_params_t * params );
size_t imgpad_gs8_getScratchSize          ( const imgpad_params_t * params );
size_t imgpad_gs16_getScratchSize         ( const imgpad_params_t * params );
size_t imgfastpad_gu8_getScratchSize      ( const imgpad_params_t * params );
size_t imgfastpad_gs8_getScratchSize      ( const imgpad_params_t * params );
size_t imgfastpad_gs16_getScratchSize     ( const imgpad_params_t * params );

/*-------------------------------------------------------------------------
  Image 2D-FFT makes special kind of Fourier transform over image data. 
  It supports a number of resolutions. For processing of arbitrary sizes, user 
  might pad original data to the closest available dimension.

  Important notes:
  1.  Accuracy of the fixed point 2D FFT suffers from the presence of unknown 
      overall image brightness so for the optimal results, mean pixel value 
      should be precomputed (i.e. by histogram function) and passed on the 
      input of FFT. 
  2.  FFT processes the data with autoscaling similarly as it is done in 
      regular FFT routines so it returns the scaled transform and number of 
      right shifts done during scaling procedure. Inverse FFT uses this scale 
      factor to recover original data range and saturates results to 8-bit 
      unsigned/signed or 16-bit signed data.
  3.  FFT accepts input in desired format and form output in form of 2D 
      array of complex 16-bit numbers
  4.  Supported dimensions: 64, 96, 128, 144, 176, 240, 256, 288, 320, 352, 
      384, 480, 512, 576, 640

  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
  gs16   16-bit grayscale data

  Input:
  img      input image
  sz[1]    size, see list of supported dimensions
  mean     mean value (average intensity) over the image, Q15.16
  Output:
  y[2*w*h] output spectrum. Real and imaginary data are interleaved and real 
           data goes first. w and h - width and height of original image

  Restrictions:
  img,y        should be aligned on a 8-bytes boundary
  image        dimensions should be selected from the list of supported 
               dimensions
  image stride should be a multiple of 8 for 8-bit images and 4 for 16-bit 
               images
-------------------------------------------------------------------------*/
/* forward transform */
int imgfft_gu8 (void *pScr, 
               complex_fract16* y, const void* img, 
               int32_t mean, const imgsize_t* sz);
int imgfft_gs8(void *pScr, 
               complex_fract16* y, const void* img, 
               int32_t mean, const imgsize_t* sz);
int imgfft_gs16(void *pScr, 
               complex_fract16* y, const void* img, 
               int32_t mean, const imgsize_t* sz);

/* Request the scratch size:*/ 
size_t imgfft_gu8_getScratchSize   (const imgsize_t* sz);
size_t imgfft_gs8_getScratchSize   (const imgsize_t* sz);
size_t imgfft_gs16_getScratchSize  (const imgsize_t* sz);

/*-------------------------------------------------------------------------
  Inverse image 2D-FFT makes special kind of Fourier transform over image data. 
  It supports a number of resolutions. For processing of arbitrary sizes, user 
  might pad original data to the closest available dimension.

  Important notes:
  1.  IFFT ignores zero-th tap of transform. Instead, it uses dedicated 
      parameter to set mean intensity of the output image
  2.  IFFT accepts input in form of 16-bit complex data and form output in 
      desired format 
  3.  IFFT makes in-place transform so input spectrum will be damaged after 
      the call
  4.  Supported dimensions: 64, 96, 128, 144, 176, 240, 256, 288, 320, 352, 
      384, 480, 512, 576, 640

  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
  gs16   16-bit grayscale data

  Input:
  y[2*w*h]    input spectrum. w and h - width and height of output image
  mean        desired mean value (average intensity) over the output image, Q15.16
  fftscale    scale factor returned by FFT
  sz[1]       output image size
  Output:
  img         output image

  Restrictions:
  img,y        should be aligned on a 8-bytes boundary
  image        dimensions should be selected from the list of supported 
               dimensions
  image stride should be a multiple of 8 for 8-bit images and 4 for 16-bit 
               images
-------------------------------------------------------------------------*/
void imgifft_gu8 (void *pScr, 
               void* img, complex_fract16* y,
               int32_t mean, const imgsize_t* sz,
               int fftscale);
void imgifft_gs8 (void *pScr, 
               void* img, complex_fract16* y,
               int32_t mean, const imgsize_t* sz,
               int fftscale);
void imgifft_gs16(void *pScr, 
               void* img, complex_fract16* y,
               int32_t mean, const imgsize_t* sz,
               int fftscale);

/* Request the scratch size:*/ 
size_t imgifft_gu8_getScratchSize  (const imgsize_t* sz);
size_t imgifft_gs8_getScratchSize  (const imgsize_t* sz);
size_t imgifft_gs16_getScratchSize (const imgsize_t* sz);
	

#ifdef __cplusplus
};
#endif

#endif /* __NATUREDSP_SIGNAL_IMG_H__ */
