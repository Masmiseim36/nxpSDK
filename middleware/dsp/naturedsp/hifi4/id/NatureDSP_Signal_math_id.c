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
 * Math Functions
 * Annotations
 */

#include "NatureDSP_types.h"
#include "NatureDSP_Signal_math.h"
#include "common.h"

ANNOTATE_FUN(vec_recip32x32, "Vector Reciprocal (32-bit data)");
ANNOTATE_FUN(vec_recip24x24, "Vector Reciprocal (24-bit data)");
ANNOTATE_FUN(vec_recip16x16, "Vector Reciprocal (16-bit data)");
ANNOTATE_FUN(vec_recip64x64, "Vector Reciprocal (64-bit data)");
ANNOTATE_FUN(scl_recip32x32, "Scalar Reciprocal (32-bit data)");
ANNOTATE_FUN(scl_recip24x24, "Scalar Reciprocal (24-bit data)");
ANNOTATE_FUN(scl_recip16x16, "Scalar Reciprocal (16-bit data)");
ANNOTATE_FUN(scl_recip64x64, "Scalar Reciprocal (64-bit data)");
ANNOTATE_FUN(vec_divide64x64,       "Vector Division (64-bit data)");
ANNOTATE_FUN(vec_divide32x32,       "Vector Division (32-bit data)");
ANNOTATE_FUN(vec_divide24x24,       "Vector Division (24-bit data)");
ANNOTATE_FUN(vec_divide16x16,       "Vector Division (16-bit data)");
ANNOTATE_FUN(vec_divide64x32i,      "Vector Division (64-bit nominator, 32-bit denominator, 32-bit output)");
ANNOTATE_FUN(vec_divide32x32_fast,  "Fast Vector Division (32-bit data)");
ANNOTATE_FUN(vec_divide24x24_fast,  "Fast Vector Division (24-bit data)");
ANNOTATE_FUN(vec_divide16x16_fast,  "Fast Vector Division (16-bit data)");
ANNOTATE_FUN(scl_divide64x64,       "Scalar Division (64-bit data)");
ANNOTATE_FUN(scl_divide32x32,       "Scalar Division (32-bit data)");
ANNOTATE_FUN(scl_divide24x24,       "Scalar Division (24-bit data)");
ANNOTATE_FUN(scl_divide16x16,       "Scalar Division (16-bit data)");
ANNOTATE_FUN(scl_divide64x32,       "Scalar Division (64-bit nominator, 32-bit denominator, 32-bit output)");
ANNOTATE_FUN(vec_log2_32x32,        "Vector Base-2  Logarithm (32-bit data)");
ANNOTATE_FUN(vec_logn_32x32,        "Vector Natural Logarithm (32-bit data)");
ANNOTATE_FUN(vec_log10_32x32,       "Vector Base-10 Logarithm (32-bit data)");
ANNOTATE_FUN(vec_log2_24x24,        "Vector Base-2  Logarithm (24-bit data)");
ANNOTATE_FUN(vec_logn_24x24,        "Vector Natural Logarithm (24-bit data)");
ANNOTATE_FUN(vec_log10_24x24,       "Vector Base-10 Logarithm (24-bit data)");
ANNOTATE_FUN(vec_log2f,             "Vector Base-2  Logarithm (floating point data)");
ANNOTATE_FUN(vec_lognf,             "Vector Natural Logarithm (floating point data)");
ANNOTATE_FUN(vec_log10f,            "Vector Base-10 Logarithm (floating point data)");
ANNOTATE_FUN(scl_log2_32x32,        "Scalar Base-2  Logarithm (32-bit data)");
ANNOTATE_FUN(scl_logn_32x32,        "Scalar Natural Logarithm (32-bit data)");
ANNOTATE_FUN(scl_log10_32x32,       "Scalar Base-10 Logarithm (32-bit data)");
ANNOTATE_FUN(scl_log2_24x24,        "Scalar Base-2  Logarithm (24-bit data)");
ANNOTATE_FUN(scl_logn_24x24,        "Scalar Natural Logarithm (24-bit data)");
ANNOTATE_FUN(scl_log10_24x24,       "Scalar Base-10 Logarithm (24-bit data)");
ANNOTATE_FUN(scl_log2f,             "Scalar Base-2  Logarithm (floating point data)");
ANNOTATE_FUN(scl_lognf,             "Scalar Natural Logarithm (floating point data)");
ANNOTATE_FUN(scl_log10f,            "Scalar Base-10 Logarithm (floating point data)");
ANNOTATE_FUN(vec_antilog2_32x32,  "Vector Base-2  Antilogarithm, (32-bit data)");
ANNOTATE_FUN(vec_antilogn_32x32,  "Vector Natural Antilogarithm, (32-bit data)");
ANNOTATE_FUN(vec_antilog10_32x32, "Vector Base-10 Antilogarithm, (32-bit data)");
ANNOTATE_FUN(vec_antilog2_24x24,  "Vector Base-2  Antilogarithm, (24-bit data)");
ANNOTATE_FUN(vec_antilogn_24x24,  "Vector Natural Antilogarithm, (24-bit data)");
ANNOTATE_FUN(vec_antilog10_24x24, "Vector Base-10 Antilogarithm, (24-bit data)");
ANNOTATE_FUN(vec_antilog2f,       "Vector Base-2  Antilogarithm, (floating point data)");
ANNOTATE_FUN(vec_antilognf,       "Vector Natural Antilogarithm, (floating point data)");
ANNOTATE_FUN(vec_antilog10f,      "Vector Base-10 Antilogarithm, (floating point data)");
ANNOTATE_FUN(vec_pow_32x32,       "Vector Power Function, (32-bit data)");
ANNOTATE_FUN(scl_antilog2_32x32,  "Scalar Base-2  Antilogarithm, (32-bit data)");
ANNOTATE_FUN(scl_antilogn_32x32,  "Scalar Natural Antilogarithm, (32-bit data)");
ANNOTATE_FUN(scl_antilog10_32x32, "Scalar Base-10 Antilogarithm, (32-bit data)");
ANNOTATE_FUN(scl_antilog2_24x24,  "Scalar Base-2  Antilogarithm, (24-bit data)");
ANNOTATE_FUN(scl_antilogn_24x24,  "Scalar Natural Antilogarithm, (24-bit data)");
ANNOTATE_FUN(scl_antilog10_24x24, "Scalar Base-10 Antilogarithm, (24-bit data)");
ANNOTATE_FUN(scl_antilog2f,       "Scalar Base-2  Antilogarithm, (floating point data)");
ANNOTATE_FUN(scl_antilognf,       "Scalar Natural Antilogarithm, (floating point data)");
ANNOTATE_FUN(scl_antilog10f,      "Scalar Base-10 Antilogarithm, (floating point data)");
ANNOTATE_FUN(vec_sqrt16x16,         "Vector Square Root (16-bit inputs, 16-bit output)");
ANNOTATE_FUN(vec_sqrt24x24,         "Vector Square Root (24-bit inputs, 24-bit output)");
ANNOTATE_FUN(vec_sqrt24x24_fast,    "Fast Vector Square Root (24-bit inputs, 24-bit output)");
ANNOTATE_FUN(vec_sqrt32x32,         "Vector Square Root (32-bit inputs, 32-bit output)");
ANNOTATE_FUN(vec_sqrt32x16,         "Vector Square Root (32-bit inputs, 16-bit output)");
ANNOTATE_FUN(vec_sqrt32x32_fast,    "Fast Vector Square Root (32-bit inputs, 32-bit output)");
ANNOTATE_FUN(vec_sqrt64x32,         "Vector Square Root (64-bit inputs, 32-bit output)");
ANNOTATE_FUN(scl_sqrt16x16,         "Scalar Square Root (16-bit inputs, 16-bit output)");
ANNOTATE_FUN(scl_sqrt24x24,         "Scalar Square Root (24-bit inputs, 24-bit output)");
ANNOTATE_FUN(scl_sqrt32x32,         "Scalar Square Root (32-bit inputs, 32-bit output)");
ANNOTATE_FUN(scl_sqrt32x16,         "Scalar Square Root (32-bit inputs, 16-bit output)");
ANNOTATE_FUN(scl_sqrt64x32,         "Scalar Square Root (64-bit inputs, 32-bit output)");
ANNOTATE_FUN(vec_rsqrt32x32,        "Vector Reciprocal Square Root (32-bit data)");
ANNOTATE_FUN(vec_rsqrt16x16,        "Vector Reciprocal Square Root (16-bit data)");
ANNOTATE_FUN(scl_rsqrt32x32,        "Scalar Reciprocal Square Root (32-bit data)");
ANNOTATE_FUN(scl_rsqrt16x16,        "Scalar Reciprocal Square Root (16-bit data)");
ANNOTATE_FUN(vec_sine24x24,         "Vector Sine (24-bit data)  ");
ANNOTATE_FUN(vec_sine24x24_fast,    "Fast Vector Sine (24-bit data)  ");
ANNOTATE_FUN(vec_cosine24x24,       "Vector Cosine (24-bit data)");
ANNOTATE_FUN(vec_cosine24x24_fast,  "Fast Vector Cosine (24-bit data)");
ANNOTATE_FUN(vec_sine32x32,         "Vector Sine (32-bit data) ");
ANNOTATE_FUN(vec_sine32x32_fast,    "Fast Vector Sine (32-bit data) ");
ANNOTATE_FUN(vec_cosine32x32,       "Vector Cosine (32-bit data)");
ANNOTATE_FUN(vec_cosine32x32_fast,  "Fast Vector Cosine (32-bit data)");
ANNOTATE_FUN(vec_sinef,             "Sine (floating point data) ");
ANNOTATE_FUN(vec_cosinef,           "Cosine (floating point data)");
ANNOTATE_FUN(scl_sine24x24,         "Scalar Sine (24-bit data) ");
ANNOTATE_FUN(scl_cosine24x24,       "Scalar Cosine (24-bit data)");
ANNOTATE_FUN(scl_sine32x32,         "Scalar Sine (32-bit data)");
ANNOTATE_FUN(scl_cosine32x32,       "Scalar Cosine (32-bit data)");
ANNOTATE_FUN(scl_sinef,             "Scalar Sine (floating point data) ");
ANNOTATE_FUN(scl_cosinef,           "Scalar Cosine (floating point data)");
ANNOTATE_FUN(vec_tan24x24,          "Vector Tangent (24-bit data)");
ANNOTATE_FUN(vec_tan32x32,          "Vector Tangent (32-bit data)");
ANNOTATE_FUN(vec_tanf,              "Vector Tangent (floating point data)");
ANNOTATE_FUN(scl_tan24x24,          "Scalar Tangent (24-bit data)");
ANNOTATE_FUN(scl_tan32x32,          "Scalar Tangent (32-bit data)");
ANNOTATE_FUN(scl_tanf,              "Scalar Tangent (floating point data)");
ANNOTATE_FUN(vec_atan24x24,         "Vector Arctangent (24-bit data) ");
ANNOTATE_FUN(vec_atan32x32,         "Vector Arctangent (32-bit data) ");
ANNOTATE_FUN(vec_atanf,             "Vector Arctangent (floating point data)");
ANNOTATE_FUN(scl_atan24x24,         "Scalar Arctangent (24-bit data) ");
ANNOTATE_FUN(scl_atan32x32,         "Scalar Arctangent (32-bit data) ");
ANNOTATE_FUN(scl_atanf,             "Scalar Arctangent (floating point data)");
ANNOTATE_FUN(vec_atan2_24x24,       "Vector Full-Quadrant Arctangent (24-bit data)");
ANNOTATE_FUN(vec_atan2f,            "Vector Full-Quadrant Arctangent (floating point data)");
ANNOTATE_FUN(scl_atan2_24x24,       "Scalar Full-Quadrant Arc Tangent (24-bit data)");
ANNOTATE_FUN(scl_atan2f,            "Scalar Full-Quadrant Arc Tangent (floating point data)");
ANNOTATE_FUN(vec_tanh32x32,         "Vector Hyperbolic Tangent (32-bit data)");
ANNOTATE_FUN(scl_tanh32x32,         "Scalar Hyperbolic Tangent (32-bit data)");
ANNOTATE_FUN(vec_tanhf,             "Vector Hyperbolic Tangent (floating point data)");
ANNOTATE_FUN(scl_tanhf,             "Scalar Hyperbolic Tangent (floating point data)");
ANNOTATE_FUN(vec_sigmoid32x32,      "Vector Sigmoid (32-bit data)");
ANNOTATE_FUN(scl_sigmoid32x32,      "Scalar Sigmoid (32-bit data)");
ANNOTATE_FUN(vec_softmax32x32,      "Vector Softmax (32-bit data)");
ANNOTATE_FUN(vec_sigmoidf,          "Vector Sigmoid (floating point data)");
ANNOTATE_FUN(scl_sigmoidf,          "Scalar Sigmoid (floating point data)");
ANNOTATE_FUN(vec_softmaxf,          "Vector Softmax (floating point data)");
ANNOTATE_FUN(vec_int2float,         "Integer to Floating Value Vector Conversion");
ANNOTATE_FUN(scl_int2float,         "Integer to Floating Value Scalar Conversion");
ANNOTATE_FUN(vec_float2int,         "Integer to Floating Value Vector Conversion");
ANNOTATE_FUN(scl_float2int,         "Integer to Floating Value Scalar Conversion");
ANNOTATE_FUN(vec_relu32x32,        "Vector Rectifier Function (32-bit data)");
ANNOTATE_FUN(vec_reluf,            "Vector Rectifier Function (floating point data)");
ANNOTATE_FUN(scl_relu32x32,        "Scalar Rectifier Function (32-bit data)");
ANNOTATE_FUN(scl_reluf,            "Scalar Rectifier Function (floating point data)");
