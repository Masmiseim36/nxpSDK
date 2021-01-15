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
/* NatureDSP_Signal Library                                               */
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
 * FFT Routines
 * Annotations
 */

/* Portable data types. */
#include "NatureDSP_types.h"
/* NatureDSP_Signal Library FFT routines. */
#include "NatureDSP_Signal_fft.h"
/* Common utility declarations. */
#include "common.h"

ANNOTATE_FUN(fft_cplx24x24,         "FFT on Complex Data (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(fft_cplx32x16,         "FFT on Complex Data (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(fft_cplx16x16,         "FFT on Complex Data (16-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(fft_cplx32x32,         "FFT on Complex Data (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(fft_real24x24,         "FFT on Real Data (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(fft_real16x16,         "FFT on Real Data (16-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(fft_real32x16,         "FFT on Real Data (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(fft_real32x32,         "FFT on Real Data (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(ifft_cplx24x24,        "Inverse FFT on Complex Data (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(ifft_cplx32x16,        "Inverse FFT on Complex Data (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(ifft_cplx16x16,        "Inverse FFT on Complex Data (16-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(ifft_cplx32x32,        "Inverse FFT on Complex Data (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(ifft_real24x24,        "Inverse FFT on Real Data (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(ifft_real16x16,        "Inverse FFT on Real Data (16-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(ifft_real32x16,        "Inverse FFT on Real Data (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(ifft_real32x32,        "Inverse FFT on Real Data (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(fft_cplx16x16_ie,      "FFT on Complex Data with Optimized Memory Usage (16-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(fft_cplx32x16_ie,      "FFT on Complex Data with Optimized Memory Usage (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(fft_cplx32x32_ie,      "FFT on Complex Data with Optimized Memory Usage (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(fft_cplx24x24_ie,      "FFT on Complex Data with Optimized Memory Usage (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(fft_cplxf_ie,          "FFT on Complex Data with Optimized Memory Usage (floating point data)");
ANNOTATE_FUN(fft_real16x16_ie,      "FFT on Real Data with Optimized Memory Usage (16-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(fft_real32x16_ie,      "FFT on Real Data with Optimized Memory Usage (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(fft_real32x32_ie,      "FFT on Real Data with Optimized Memory Usage (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(fft_real24x24_ie,      "FFT on Real Data with Optimized Memory Usage (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(fft_real24x24_ie_24p,  "FFT on Real Data with Optimized Memory Usage (24-bit packed input/outputs, 24-bit data, 24-bit twiddles)");
ANNOTATE_FUN(fft_real32x16_ie_24p,  "FFT on Real Data with Optimized Memory Usage (24-bit packed input/outputs, 32-bit data, 16-bit twiddles)");
ANNOTATE_FUN(fft_realf_ie,          "FFT on Real Data with Optimized Memory Usage (floating point data)");
ANNOTATE_FUN(ifft_cplx16x16_ie,     "Inverse FFT on Complex Data with Optimized Memory Usage (16-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(ifft_cplx32x16_ie,     "Inverse FFT on Complex Data with Optimized Memory Usage (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(ifft_cplx32x32_ie,     "Inverse FFT on Complex Data with Optimized Memory Usage (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(ifft_cplx24x24_ie,     "Inverse FFT on Complex Data with Optimized Memory Usage (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(ifft_cplxf_ie,         "Inverse FFT on Complex Data with Optimized Memory Usage (floating point data)");
ANNOTATE_FUN(ifft_real16x16_ie,     "Inverse FFT on Real Data with Optimized Memory Usage (16-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(ifft_real32x16_ie,     "Inverse FFT on Real Data with Optimized Memory Usage (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(ifft_real32x32_ie,     "Inverse FFT on Real Data with Optimized Memory Usage (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(ifft_real24x24_ie,     "Inverse FFT on Real Data with Optimized Memory Usage (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(ifft_real32x16_ie_24p, "Inverse FFT on Real Data with Optimized Memory Usage (24-bit packed input/outputs, 32-bit data, 16-bit twiddles)");
ANNOTATE_FUN(ifft_real24x24_ie_24p, "Inverse FFT on Real Data with Optimized Memory Usage (24-bit packed input/outputs, 24-bit data, 24-bit twiddles)");
ANNOTATE_FUN(ifft_realf_ie,         "Inverse FFT on Real Data with Optimized Memory Usage (floating point data)");
ANNOTATE_FUN(dct_24x24,             "Discrete Cosine Transform, Type II (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(dct_32x16,             "Discrete Cosine Transform, Type II (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(dctf,                  "Discrete Cosine Transform, Type II (floating point data)");
ANNOTATE_FUN(dct_16x16,             "Discrete Cosine Transform, Type II (16-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(dct_32x32,             "Discrete Cosine Transform, Type II (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(dct4_24x24,            "Discrete Cosine Transform, Type IV (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(dct4_32x16,            "Discrete Cosine Transform, Type IV (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(dct4_32x32,            "Discrete Cosine Transform, Type IV (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(mdct_24x24,            "Modified Discrete Cosine Transform (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(mdct_32x16,            "Modified Discrete Cosine Transform (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(mdct_32x32,            "Modified Discrete Cosine Transform (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(imdct_24x24,           "Inverse Modified Discrete Cosine Transform (24-bit input/outputs, 24-bit twiddles)");
ANNOTATE_FUN(imdct_32x16,           "Inverse Modified Discrete Cosine Transform (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(imdct_32x32,           "Inverse Modified Discrete Cosine Transform (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(dct2d_8x16,            "2-D Discrete Cosine Transform (8-bit unsigned input, 16-bit signed output)        ");
ANNOTATE_FUN(idct2d_16x8,           "2-D Inverse Discrete Cosine Transform (16-bit signed input, 8-bit unsigned output)");
ANNOTATE_FUN(fft_spectrumf    ,     "FFT Power Spectrum ( complex floating-point data, single precision )              ");
ANNOTATE_FUN(fft_spectrum16x32,     "FFT Power Spectrum ( complex 16-bit data )                                        ");
ANNOTATE_FUN(fft_spectrum32x32,     "FFT Power Spectrum ( complex 32-bit data )                                        ");
ANNOTATE_FUN(stereo_fft_cplx16x16_ie, "FFT on Stereo Complex Data with Optimized Memory Usage (16-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(stereo_fft_cplx32x16_ie, "FFT on Stereo Complex Data with Optimized Memory Usage (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(stereo_fft_cplx32x32_ie, "FFT on Stereo Complex Data with Optimized Memory Usage (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(stereo_fft_cplxf_ie,     "FFT on Stereo Complex Data with Optimized Memory Usage (floating point data)");
ANNOTATE_FUN(stereo_ifft_cplx16x16_ie,"Inverse FFT on Stereo Complex Data with Optimized Memory Usage (16-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(stereo_ifft_cplx32x16_ie,"Inverse FFT on Stereo Complex Data with Optimized Memory Usage (32-bit input/outputs, 16-bit twiddles)");
ANNOTATE_FUN(stereo_ifft_cplx32x32_ie,"Inverse FFT on Stereo Complex Data with Optimized Memory Usage (32-bit input/outputs, 32-bit twiddles)");
ANNOTATE_FUN(stereo_ifft_cplxf_ie,    "Inverse FFT on Stereo Complex Data with Optimized Memory Usage (floating point data)");
