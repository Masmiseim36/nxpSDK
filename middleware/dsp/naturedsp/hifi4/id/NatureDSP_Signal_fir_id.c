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
 * FIR Filters and Related Functions
 * Annotations
 */

/* Portable data types. */
#include "NatureDSP_types.h"
/* NatureDSP_Signal Library FIR Filters and Related Functions. */
#include "NatureDSP_Signal_fir.h"
/* Common utility declarations. */
#include "common.h"

ANNOTATE_FUN(bkfir16x16_process,  "Fast Real FIR filter (16-bit data, 16-bit coefficients, 16-bit outputs) ");
ANNOTATE_FUN(bkfir24x24_process,  "Fast Real FIR filter (24-bit data, 24-bit coefficients, 24-bit outputs) ");
ANNOTATE_FUN(bkfir24x24p_process, "Fast Real FIR filter (24-bit data, 24-bit packed internal delay line buffer and internal coefficients storage)");
ANNOTATE_FUN(bkfir32x16_process,  "Fast Real FIR filter (32-bit data, 16-bit coefficients, 32-bit outputs) ");
ANNOTATE_FUN(bkfir32x32_process,  "Fast Real FIR filter (32-bit data, 32-bit coefficients, 32-bit outputs) ");
ANNOTATE_FUN(bkfir32x32ep_process,"Fast Real FIR filter (32-bit data, 32-bit coefficients, 32-bit outputs) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(bkfirf_process,      "Fast Real FIR filter (floating point data)                              ");
ANNOTATE_FUN(bkfira16x16_process,  "Real FIR filter (16-bit data, 16-bit coefficients, 16-bit outputs) ");
ANNOTATE_FUN(bkfira24x24_process,  "Real FIR filter (24-bit data, 24-bit coefficients, 24-bit outputs) ");
ANNOTATE_FUN(bkfira32x16_process,  "Real FIR filter (32-bit data, 16-bit coefficients, 32-bit outputs) ");
ANNOTATE_FUN(bkfira32x32_process,  "Real FIR filter (32-bit data, 32-bit coefficients, 32-bit outputs) ");
ANNOTATE_FUN(bkfira32x32ep_process,"Real FIR filter (32-bit data, 32-bit coefficients, 32-bit outputs) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(bkfiraf_process,     "Real FIR filter (floating point data)                              ");
ANNOTATE_FUN(cxfir16x16_process,  "Fast Complex Block FIR Filter (16-bit data, 16-bit coefficients, 16-bit outputs)");
ANNOTATE_FUN(cxfir24x24_process,  "Fast Complex Block FIR Filter (24-bit data, 24-bit coefficients, 24-bit outputs)");
ANNOTATE_FUN(cxfir32x16_process,  "Fast Complex Block FIR Filter (32-bit data, 16-bit coefficients, 32-bit outputs)");
ANNOTATE_FUN(cxfir32x32_process,  "Fast Complex Block FIR Filter (32-bit data, 32-bit coefficients, 32-bit outputs)");
ANNOTATE_FUN(cxfir32x32ep_process,"Fast Complex Block FIR Filter (32-bit data, 32-bit coefficients, 32-bit outputs) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(cxfirf_process,      "Fast Complex Block FIR Filter (floating point data)                             ");
ANNOTATE_FUN(firdec16x16_process,  "Decimating Block Real FIR Filter (16-bit data, 16-bit coefficients, 16-bit outputs)");
ANNOTATE_FUN(firdec24x24_process,  "Decimating Block Real FIR Filter (24-bit data, 24-bit coefficients, 24-bit outputs)");
ANNOTATE_FUN(firdec32x16_process,  "Decimating Block Real FIR Filter (32-bit data, 16-bit coefficients, 32-bit outputs)");
ANNOTATE_FUN(firdec32x32_process,  "Decimating Block Real FIR Filter (32-bit data, 32-bit coefficients, 32-bit outputs)");
ANNOTATE_FUN(firdec32x32ep_process,"Decimating Block Real FIR Filter (32-bit data, 32-bit coefficients, 32-bit outputs) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(firdecf_process,      "Decimating Block Real FIR Filter (floating point data)                             ");
ANNOTATE_FUN(firinterp16x16_process,  "Interpolating Block Real FIR Filter (16-bit data, 16-bit coefficients, 16-bit outputs)");
ANNOTATE_FUN(firinterp24x24_process,  "Interpolating Block Real FIR Filter (24-bit data, 24-bit coefficients, 24-bit outputs)");
ANNOTATE_FUN(firinterp32x16_process,  "Interpolating Block Real FIR Filter (32-bit data, 16-bit coefficients, 32-bit outputs)");
ANNOTATE_FUN(firinterp32x32_process,  "Interpolating Block Real FIR Filter (32-bit data, 32-bit coefficients, 32-bit outputs)");
ANNOTATE_FUN(firinterp32x32ep_process,"Interpolating Block Real FIR Filter (32-bit data, 32-bit coefficients, 32-bit outputs) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(firinterpf_process,      "Interpolating Block Real FIR Filter (floating point data)                             ");
ANNOTATE_FUN(fir_convol16x16,     "Fast Circular Convolution (16x16-bit data, 16-bit outputs)                               ");
ANNOTATE_FUN(fir_convol24x24,     "Fast Circular Convolution (24x24-bit data, 24-bit outputs)                               ");
ANNOTATE_FUN(fir_convol32x16,     "Fast Circular Convolution (32x16-bit data, 32-bit outputs)                               ");
ANNOTATE_FUN(fir_convol32x32,     "Fast Circular Convolution (32x32-bit data, 32-bit outputs)                               ");
ANNOTATE_FUN(fir_convol32x32ep,   "Fast Circular Convolution (32x32-bit data, 32-bit outputs) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(fir_convolf,         "Fast Circular Convolution (floating point data)                                          ");
ANNOTATE_FUN(cxfir_convol32x16,   "Fast Circular Convolution (32x16-bit complex data, 32-bit complex outputs)               ");
ANNOTATE_FUN(fir_convola16x16,    "Circular Convolution (16x16-bit data, 16-bit outputs)                                    ");
ANNOTATE_FUN(fir_convola24x24,    "Circular Convolution (24x24-bit data, 24-bit outputs)                                    ");
ANNOTATE_FUN(fir_convola32x16,    "Circular Convolution (32x16-bit data, 32-bit outputs)                                    ");
ANNOTATE_FUN(cxfir_convola32x16,  "Circular Convolution (32x16-bit complex data, 32-bit complex outputs)                    ");
ANNOTATE_FUN(fir_convola32x32,    "Circular Convolution (32x32-bit data, 32-bit outputs)                                    ");
ANNOTATE_FUN(fir_convola32x32ep,  "Circular Convolution (32x32-bit data, 32-bit outputs) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(fir_convolaf,        "Circular Convolution (floating point data)                                               ");
ANNOTATE_FUN(fir_lconvola16x16,   "Linear Convolution (16x16-bit data, 16-bit outputs)                                      ");
ANNOTATE_FUN(fir_lconvola32x32,   "Linear Convolution (32x32-bit data, 32-bit outputs)                                      ");
ANNOTATE_FUN(fir_xcorr16x16,      "Fast Circular Correlation (16x16-bit data, 16-bit outputs)                               ");
ANNOTATE_FUN(fir_xcorr24x24,      "Fast Circular Correlation (24x24-bit data, 24-bit outputs)                               ");
ANNOTATE_FUN(fir_xcorr32x16,      "Fast Circular Correlation (32x16-bit data, 32-bit outputs)                               ");
ANNOTATE_FUN(fir_xcorr32x32,      "Fast Circular Correlation (32x32-bit data, 32-bit outputs)                               ");
ANNOTATE_FUN(cxfir_xcorr32x32,    "Fast Circular Correlation (32x32-bit complex data, 32-bit complex outputs)               ");
ANNOTATE_FUN(fir_xcorr32x32ep,    "Fast Circular Correlation (32x32-bit data, 32-bit outputs) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(fir_xcorrf,          "Fast Circular Correlation (floating point data)                                          ");
ANNOTATE_FUN(cxfir_xcorrf,        "Circular Correlation (complex floating point data)                                       ");
ANNOTATE_FUN(fir_xcorra16x16,     "Circular Correlation (16x16-bit data, 16-bit outputs)                                    ");
ANNOTATE_FUN(fir_xcorra24x24,     "Circular Correlation (24x24-bit data, 24-bit outputs)                                    ");
ANNOTATE_FUN(fir_xcorra32x16,     "Circular Correlation (32x16-bit data, 32-bit outputs)                                    ");
ANNOTATE_FUN(fir_xcorra32x32,     "Circular Correlation (32x32-bit data, 32-bit outputs)                                    ");
ANNOTATE_FUN(cxfir_xcorra32x32,   "Circular Correlation (32x32-bit complex data, 32-bit complex outputs)                    ");
ANNOTATE_FUN(fir_xcorra32x32ep,   "Circular Correlation (32x32-bit data, 32-bit outputs) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(fir_xcorraf,         "Circular Correlation (floating point data)                                               ");
ANNOTATE_FUN(cxfir_xcorraf,       "Circular Correlation (complex floating point data)                                       ");
ANNOTATE_FUN(fir_lxcorra16x16,    "Linear Correlation (16x16-bit data, 16-bit outputs)                                      ");
ANNOTATE_FUN(fir_lxcorra32x32,    "Linear Correlation (32x32-bit data, 32-bit outputs)                                      ");
ANNOTATE_FUN(fir_acorr16x16,      "Fast Circular Autocorrelation (16-bit data, 16-bit outputs)                              ");
ANNOTATE_FUN(fir_acorr24x24,      "Fast Circular Autocorrelation (24-bit data, 24-bit outputs)                              ");
ANNOTATE_FUN(fir_acorr32x32,      "Fast Circular Autocorrelation (32-bit data, 32-bit outputs)                              ");
ANNOTATE_FUN(fir_acorr32x32ep,    "Fast Circular Autocorrelation (32-bit data, 32-bit outputs) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(fir_acorrf,          "Fast Circular Autocorrelation (floating point data)                                      ");
ANNOTATE_FUN(fir_acorra16x16,     "Circular Autocorrelation (16-bit data, 16-bit outputs)                                   ");
ANNOTATE_FUN(fir_acorra24x24,     "Circular Autocorrelation (24-bit data, 24-bit outputs)                                   ");
ANNOTATE_FUN(fir_acorra32x32,     "Circular Autocorrelation (32-bit data, 32-bit outputs)                                   ");
ANNOTATE_FUN(fir_acorra32x32ep,   "Circular Autocorrelation (32-bit data, 32-bit outputs) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(fir_acorraf,         "Circular Autocorrelation (floating point data)                                           ");
ANNOTATE_FUN(fir_lacorra16x16,    "Linear Autocorrelation (16-bit data, 16-bit outputs)                                     ");
ANNOTATE_FUN(fir_lacorra32x32,    "Linear Autocorrelation (32-bit data, 32-bit outputs)                                     ");
ANNOTATE_FUN(fir_blms16x16,       "Blockwise Adaptive LMS Algorithm for Real Data (16-bit coefficients, 16-bit data, 16-bit output)");
ANNOTATE_FUN(fir_blms24x24,       "Blockwise Adaptive LMS Algorithm for Real Data (24-bit coefficients, 24-bit data, 24-bit output)");
ANNOTATE_FUN(fir_blms16x32,       "Blockwise Adaptive LMS Algorithm for Real Data (32-bit coefficients, 16-bit data, 16-bit output)");
ANNOTATE_FUN(fir_blms32x32,       "Blockwise Adaptive LMS Algorithm for Real Data (32-bit coefficients, 32-bit data, 32-bit output)");
ANNOTATE_FUN(fir_blms32x32ep,     "Blockwise Adaptive LMS Algorithm for Real Data (32-bit coefficients, 32-bit data, 32-bit output) using 72-bit accumulator for intermediate computations");
ANNOTATE_FUN(cxfir_blms32x32,     "Blockwise Adaptive LMS Algorithm for Complex Data (32-bit coefficients, 32-bit data, 32-bit output)");
ANNOTATE_FUN(fir_blmsf,           "Blockwise Adaptive LMS Algorithm for Real Data (floating point data)                            ");
ANNOTATE_FUN(cxfir_blmsf,         "Blockwise Adaptive LMS Algorithm for Complex Data (floating point data)                         ");
ANNOTATE_FUN(stereo_bkfir16x16_process,  "Fast Real FIR Stereo filter (16-bit data, 16-bit coefficients, 16-bit outputs) ");
ANNOTATE_FUN(stereo_bkfir32x32_process,  "Fast Real FIR Stereo filter (32-bit data, 32-bit coefficients, 32-bit outputs) ");
ANNOTATE_FUN(stereo_bkfirf_process,      "Fast Real FIR Stereo filter (floating point data)                              ");
ANNOTATE_FUN(conv2d_3x3_16x16           ,"2D Convolution ( 3x3 16-bit kernel, 16-bit data )                              ");
ANNOTATE_FUN(conv2d_5x5_16x16           ,"2D Convolution ( 5x5 16-bit kernel, 16-bit data )                              ");
ANNOTATE_FUN(conv2d_11x7_16x16          ,"2D Convolution ( 11x7 16-bit kernel, 16-bit data )                             ");
ANNOTATE_FUN(conv2d_3x3_8x16            ,"2D Convolution ( 3x3 8-bit kernel, 16-bit data )                               ");
ANNOTATE_FUN(conv2d_5x5_8x16            ,"2D Convolution ( 5x5 8-bit kernel, 16-bit data )                               ");
ANNOTATE_FUN(conv2d_11x7_8x16           ,"2D Convolution ( 11x7 8-bit kernel, 16-bit data )                              ");
ANNOTATE_FUN(conv2d_3x3_8x8             ,"2D Convolution ( 3x3 8-bit kernel, 8-bit data )                                ");
ANNOTATE_FUN(conv2d_5x5_8x8             ,"2D Convolution ( 5x5 8-bit kernel, 8-bit data )                                ");
ANNOTATE_FUN(conv2d_11x7_8x8            ,"2D Convolution ( 11x7 8-bit kernel, 8-bit data )                               ");
