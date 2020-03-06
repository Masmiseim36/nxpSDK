/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

#include <math.h>
#if 0
typedef float   float32_t;
typedef double  float64_t;


#ifdef __cplusplus
extern "C" {
#endif

float32_t acosf_stdlib ( float32_t x ) { return acosf(x); }
float64_t acos_stdlib ( float64_t x ) { return acos(x); }
float32_t asinf_stdlib ( float32_t  x ) { return asinf(x); }
float64_t asin_stdlib ( float64_t  x ) { return asin(x); }
float32_t atanf_stdlib ( float32_t x ) { return atanf(x); }
float64_t atan_stdlib ( float64_t x ) { return atan(x); }
float32_t atan2f_stdlib ( float32_t y, float32_t x ) { return atan2f(y,x); }
float64_t atan2_stdlib ( float64_t y, float64_t x ) { return atan2(y,x); }
float32_t cosf_stdlib ( float32_t x ) { return cosf(x); }
float64_t cos_stdlib ( float64_t x ) { return cos(x); }
float32_t sinf_stdlib ( float32_t x ) { return sinf(x); }
float64_t sin_stdlib ( float64_t x ) { return sin(x); };
float32_t tanf_stdlib ( float32_t x ) { return tanf(x); }
float64_t tan_stdlib ( float64_t x ) { return tan(x); }
float32_t coshf_stdlib ( float32_t x ) { return coshf(x); }
float64_t cosh_stdlib ( float64_t x ) { return cosh(x); }
float32_t sinhf_stdlib ( float32_t x ) { return sinhf(x); }
float64_t sinh_stdlib ( float64_t x ) { return sinh(x); }
float32_t tanhf_stdlib ( float32_t x ) { return tanhf(x); }
float64_t tanh_stdlib ( float64_t x ) { return tanh(x); }
float32_t floorf_stdlib ( float32_t x ) { return floorf(x); }
float64_t floor_stdlib ( float64_t x ) { return floor(x); }
float32_t ceilf_stdlib ( float32_t x ) { return ceilf(x); }
float64_t ceil_stdlib ( float64_t x ) { return ceil(x); }
float32_t fmodf_stdlib ( float32_t x, float32_t y ) { return fmodf(x,y); }
float64_t fmod_stdlib ( float64_t x, float64_t y ) { return fmod(x,y); }
float32_t ldexpf_stdlib ( float32_t x, int n ) { return ldexpf(x,n); }
float64_t ldexp_stdlib ( float64_t x, int n ) { return ldexp(x,n); }
float32_t logf_stdlib ( float32_t x ) { return logf(x); }
float64_t log_stdlib ( float64_t x ) { return log(x); }
float32_t log10f_stdlib ( float32_t x ) { return log10f(x); }
float64_t log10_stdlib ( float64_t x ) { return log10(x); }
float32_t expf_stdlib ( float32_t x ) { return expf(x); }
float64_t exp_stdlib ( float64_t x ) { return exp(x); }
float32_t powf_stdlib ( float32_t x, float32_t y ) { return powf(x,y); }
float64_t pow_stdlib ( float64_t x, float64_t y ) { return pow(x,y); }
float32_t copysignf_stdlib ( float32_t x, float32_t y ) { return copysignf(x,y); }

#ifdef __cplusplus
};
#endif

#endif
