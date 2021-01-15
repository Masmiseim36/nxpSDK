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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
	NatureDSP_Signal library. FFT part
    Twiddle factor tables and descriptor structures for 32x16 & 16x16 FFTs
	IntegrIT, 2006-2018
*/

#ifndef __FFT_X16_COMMON_H__
#define __FFT_X16_COMMON_H__
#include "NatureDSP_types.h"

/* Types of FFT stages that can be used in FFT processing */
#define NUM_FFT_STAGE_TYPES 20
typedef enum {
    fft_stage_first_DFT2       = 0 ,/* first stage for forward FFT, radix-2 */
    fft_stage_first_DFT3       = 1 ,/* first stage for forward FFT, radix-3 */
    fft_stage_first_DFT4       = 2 ,/* first stage for forward FFT, radix-4 */
    fft_stage_first_DFT5       = 3 ,/* first stage for forward FFT, radix-5 */
    fft_stage_first_inv_DFT2   = 4 ,/* first stage for inverse FFT, radix-2 */
    fft_stage_first_inv_DFT3   = 5 ,/* first stage for inverse FFT, radix-3 */
    fft_stage_first_inv_DFT4   = 6 ,/* first stage for inverse FFT, radix-4 */
    fft_stage_first_inv_DFT5   = 7 ,/* first stage for inverse FFT, radix-5 */
    fft_stage_inner_DFT2       = 8 ,/* intermediate stage for forward/inverse FFT, radix-2 */
    fft_stage_inner_DFT3       = 9 ,/* intermediate stage for forward/inverse FFT, radix-3 */
    fft_stage_inner_DFT4       = 10,/* intermediate stage for forward/inverse FFT, radix-4 */
    fft_stage_inner_DFT4x2     = 11,/* intermediate stage for forward/inverse FFT, radix-4 with unrolled loop 2 times */
    fft_stage_inner_DFT4x4     = 12,/* intermediate stage for forward/inverse FFT, radix-4 with unrolled loop 4 times */
    fft_stage_inner_DFT5       = 13,/* intermediate stage for forward/inverse FFT, radix-5 */
    fft_stage_penultimate_DFT4 = 14,/* penultimate stage for forward/inverse FFT, radix-4 */
    fft_stage_last_DFT2        = 15,/* last stage for forward/inverse FFT, radix-2 */
    fft_stage_last_DFT3        = 16,/* last stage for forward/inverse FFT, radix-3 */
    fft_stage_last_DFT4        = 17,/* last stage for forward/inverse FFT, radix-4 */
    fft_stage_last_DFT5        = 18,/* last stage for forward/inverse FFT, radix-5 */
    fft_stage_last_DFT8        = 19 /* last stage for forward/inverse FFT, radix-8 */
/* Types of FFT stages that can be used in FFT processing */
} eFft_stage_type;

typedef const int16_t* cint16ptr_t;

/*
 * 32x16-bit & 16x16-bit complex-valued FFT descriptor structure.
 */
typedef struct
{
    const int N;
    /* Table of the stages for scaleOption = 2 */
    const eFft_stage_type *stages_32x16_s2; 
    const eFft_stage_type *stages_16x16_s2; 
    /* Table of the stages for scaleOption = 3 */
    const eFft_stage_type *stages_32x16_s3;
    const eFft_stage_type *stages_16x16_s3;
    /* Twiddle factors (access step and tables) */
    const int *tw_step; 
    const cint16ptr_t *twd;

} fft_cplx_x16_descr_t;

/*
 *   32x16-bit or 16x16-bit real-valued FFT descriptor structure.
 */
typedef struct
{
    /* Handle of half-size complex FFT */
    const fft_cplx_x16_descr_t *cfft_hdl; 
    const int16_t *twd;
} fft_real_x16_descr_t;

/* Descriptors that are used in real-valued FFT as half-sized complex FFT */
extern const fft_cplx_x16_descr_t __cfft_x16_descr16;
extern const fft_cplx_x16_descr_t __cfft_x16_descr32;
extern const fft_cplx_x16_descr_t __cfft_x16_descr64;
extern const fft_cplx_x16_descr_t __cfft_x16_descr128;
extern const fft_cplx_x16_descr_t __cfft_x16_descr256;
extern const fft_cplx_x16_descr_t __cfft_x16_descr512;
extern const fft_cplx_x16_descr_t __cfft_x16_descr1024;
extern const fft_cplx_x16_descr_t __cfft_x16_descr2048;
extern const fft_cplx_x16_descr_t __cfft_x16_descr4096;

extern const fft_cplx_x16_descr_t __cifft_x16_descr16;
extern const fft_cplx_x16_descr_t __cifft_x16_descr32;
extern const fft_cplx_x16_descr_t __cifft_x16_descr64;
extern const fft_cplx_x16_descr_t __cifft_x16_descr128;
extern const fft_cplx_x16_descr_t __cifft_x16_descr256;
extern const fft_cplx_x16_descr_t __cifft_x16_descr512;
extern const fft_cplx_x16_descr_t __cifft_x16_descr1024;
extern const fft_cplx_x16_descr_t __cifft_x16_descr2048;
extern const fft_cplx_x16_descr_t __cifft_x16_descr4096;

extern const fft_cplx_x16_descr_t __cfft_x16_descr160;
extern const fft_cplx_x16_descr_t __cfft_x16_descr192;
extern const fft_cplx_x16_descr_t __cfft_x16_descr240;
extern const fft_cplx_x16_descr_t __cfft_x16_descr160;
extern const fft_cplx_x16_descr_t __cfft_x16_descr192;
extern const fft_cplx_x16_descr_t __cfft_x16_descr240;
extern const fft_cplx_x16_descr_t __cfft_32x16_descr160;
extern const fft_cplx_x16_descr_t __cfft_32x16_descr192;
extern const fft_cplx_x16_descr_t __cfft_32x16_descr240;
extern const fft_cplx_x16_descr_t __cfft_32x16_descr160;
extern const fft_cplx_x16_descr_t __cfft_32x16_descr192;
extern const fft_cplx_x16_descr_t __cfft_32x16_descr240;

extern const fft_cplx_x16_descr_t __cifft_x16_descr160;
extern const fft_cplx_x16_descr_t __cifft_x16_descr192;
extern const fft_cplx_x16_descr_t __cifft_x16_descr240;
extern const fft_cplx_x16_descr_t __cifft_x16_descr160;
extern const fft_cplx_x16_descr_t __cifft_x16_descr192;
extern const fft_cplx_x16_descr_t __cifft_x16_descr240;
extern const fft_cplx_x16_descr_t __cifft_32x16_descr160;
extern const fft_cplx_x16_descr_t __cifft_32x16_descr192;
extern const fft_cplx_x16_descr_t __cifft_32x16_descr240;
extern const fft_cplx_x16_descr_t __cifft_32x16_descr160;
extern const fft_cplx_x16_descr_t __cifft_32x16_descr192;
extern const fft_cplx_x16_descr_t __cifft_32x16_descr240;

#endif // __FFT_X16_COMMON_H__
