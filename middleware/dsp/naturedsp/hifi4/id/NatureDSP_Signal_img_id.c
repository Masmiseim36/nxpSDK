/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ('Cadence    */
/* Libraries') are the copyrighted works of Cadence Design Systems Inc.     */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* NatureDSP_Signal Library                                                 */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2009-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
 * NatureDSP_Signal Library API
 * Image processing functions
 * Annotations
 */

/* Portable data types. */
#include "NatureDSP_types.h"
/* NatureDSP_Signal Library FIR Filters and Related Functions. */
#include "NatureDSP_Signal_img.h"
/* Common utility declarations. */
#include "common.h"

ANNOTATE_FUN(imginterleave,               "Image interleave (8-bit) ");
ANNOTATE_FUN(imgfastinterleave,           "Image interleave, fast (8-bit) ");
ANNOTATE_FUN(imgdeinterleave,             "Image deinterleave (8-bit) ");
ANNOTATE_FUN(imgfastdeinterleave,         "Image deinterleave, fast (8-bit) ");
ANNOTATE_FUN(imginterleave16,             "Image interleave (16-bit) ");
ANNOTATE_FUN(imgfastinterleave16,         "Image interleave, fast (16-bit) ");
ANNOTATE_FUN(imgdeinterleave16,           "Image deinterleave (16-bit) ");
ANNOTATE_FUN(imgfastdeinterleave16,       "Image deinterleave, fast (16-bit) ");
ANNOTATE_FUN(imghist_gu8,                 "Image histogram (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imgfasthist_gu8,             "Image histogram, fast (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imghist_gs8,                 "Image histogram (8-bit signed grayscale) ");
ANNOTATE_FUN(imgfasthist_gs8,             "Image histogram, fast (8-bit signed grayscale) ");
ANNOTATE_FUN(imghist_gs16,                "Image histogram (16-bit grayscale) ");
ANNOTATE_FUN(imgfasthist_gs16,            "Image histogram, fast (16-bit grayscale) ");
ANNOTATE_FUN(imgnorm_gu8,                 "Image normalization (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imgfastnorm_gu8,             "Image normalization, fast (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imgnorm_gs8,                 "Image normalization (8-bit signed grayscale) ");
ANNOTATE_FUN(imgfastnorm_gs8,             "Image normalization, fast (8-bit signed grayscale) ");
ANNOTATE_FUN(imgnorm_gs16,                "Image normalization (16-bit grayscale) ");
ANNOTATE_FUN(imgfastnorm_gs16,            "Image normalization, fast (16-bit grayscale) ");
ANNOTATE_FUN(imgnorm_gu8_nonlinear,       "Image normalization, nonlinear (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imgfastnorm_gu8_nonlinear,   "Image normalization, fast, nonlinear (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imgnorm_gs8_nonlinear,       "Image normalization, nonlinear (8-bit signed grayscale) ");
ANNOTATE_FUN(imgfastnorm_gs8_nonlinear,   "Image normalization, fast, nonlinear (8-bit signed grayscale) ");
ANNOTATE_FUN(imgnorm_gs16_nonlinear,      "Image normalization, nonlinear (16-bit grayscale) ");
ANNOTATE_FUN(imgfastnorm_gs16_nonlinear,  "Image normalization, fast, nonlinear (16-bit grayscale) ");
ANNOTATE_FUN(imgrotate_gu8_process,       "Image rotation (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imgfastrotate_gu8_process,   "Image rotation, fast (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imgrotate_gs8_process,       "Image rotation (8-bit signed grayscale) ");
ANNOTATE_FUN(imgfastrotate_gs8_process,   "Image rotation, fast (8-bit signed grayscale) ");
ANNOTATE_FUN(imgrotate_gs16_process,      "Image rotation (16-bit grayscale) ");
ANNOTATE_FUN(imgfastrotate_gs16_process,  "Image rotation, fast (16-bit grayscale) ");
ANNOTATE_FUN(imgresize_gu8_process,       "Image resize (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imgfastresize_gu8_process,   "Image resize, fast (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imgresize_gs8_process,       "Image resize (8-bit signed grayscale) ");
ANNOTATE_FUN(imgfastresize_gs8_process,   "Image resize, fast (8-bit signed grayscale) ");
ANNOTATE_FUN(imgresize_gs16_process,      "Image resize (16-bit grayscale) ");
ANNOTATE_FUN(imgfastresize_gs16_process,  "Image resize, fast (16-bit grayscale) ");
ANNOTATE_FUN(imgconvert_rgbyuv,           "Image RGB->YUV conversion (8-bit unsigned)");
ANNOTATE_FUN(imgconvert_yuvrgb,           "Image YUV->RGB conversion (8-bit unsigned)");
ANNOTATE_FUN(imgfastconvert_rgbyuv,       "Image RGB->YUV conversion, fast (8-bit unsigned)");
ANNOTATE_FUN(imgfastconvert_yuvrgb,       "Image YUV->RGB conversion, fast (8-bit unsigned)");
ANNOTATE_FUN(imgconvert_rgbyuv16,         "Image RGB->YUV conversion (16-bit signed)");
ANNOTATE_FUN(imgconvert_yuvrgb16,         "Image YUV->RGB conversion (16-bit signed)");
ANNOTATE_FUN(imgfastconvert_rgbyuv16,     "Image RGB->YUV conversion, fast (16-bit signed)");
ANNOTATE_FUN(imgfastconvert_yuvrgb16,     "Image YUV->RGB conversion, fast (16-bit signed)");
ANNOTATE_FUN(imgpad_gu8,                  "Image padding/cropping (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imgfastpad_gu8,              "Image padding/cropping, fast (8-bit unsigned grayscale) ");
ANNOTATE_FUN(imgpad_gs8,                  "Image padding/cropping (8-bit signed grayscale) ");
ANNOTATE_FUN(imgfastpad_gs8,              "Image padding/cropping, fast (8-bit signed grayscale) ");
ANNOTATE_FUN(imgpad_gs16,                 "Image padding/cropping (16-bit grayscale) ");
ANNOTATE_FUN(imgfastpad_gs16,             "Image padding/cropping, fast (16-bit grayscale) ");
ANNOTATE_FUN(imgfft_gu8,                  "Image 2D FFT (8-bit unsigned grayscale)");
ANNOTATE_FUN(imgfft_gs8,                  "Image 2D FFT (8-bit signed grayscale)");
ANNOTATE_FUN(imgfft_gs16,                 "Image 2D FFT (16-bit grayscale)");
ANNOTATE_FUN(imgifft_gu8,                 "Image 2D IFFT (8-bit unsigned grayscale)");
ANNOTATE_FUN(imgifft_gs8,                 "Image 2D IFFT (8-bit signed grayscale)");
ANNOTATE_FUN(imgifft_gs16,                "Image 2D IFFT (16-bit grayscale)");
