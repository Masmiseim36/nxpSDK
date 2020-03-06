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
    complex FFT stages for 16x16 FFTs
	IntegrIT, 2006-2018
*/

#ifndef __FFT_16X16_STAGES_H__
#define __FFT_16X16_STAGES_H__
#include "NatureDSP_types.h"
#include "fft_x16_common.h"

/*
 * Pointer to functions which implement complex 16x16 FFTs stage 
 */
typedef int (*fft_cplx16x16_stage_t)(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);

/*
 *  16x16 FFT stages, scalingOption=2    
 */

int fft_16x16_stage_first_scl2_DFT3        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_first_scl2_DFT4        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_first_scl2_DFT5        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int ifft_16x16_stage_first_scl2_DFT3       (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int ifft_16x16_stage_first_scl2_DFT4       (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int ifft_16x16_stage_first_scl2_DFT5       (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl2_DFT2        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl2_DFT3        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl2_DFT4x2      (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_penultimate_scl2_DFT4x4(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_last_scl2_DFT4         (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_last_scl2_DFT8         (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);

int fft_16x16_stage_first_scl2_DFT2        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int ifft_16x16_stage_first_scl2_DFT2       (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl2_DFT4        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl2_DFT4x4      (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl2_DFT5        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_last_scl2_DFT2         (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_last_scl2_DFT3         (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_last_scl2_DFT5         (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);

/* Table of 16x16 FFT stages with dynamic scaling used in processing */
extern const fft_cplx16x16_stage_t fft16x16_stages_scl2_tbl[NUM_FFT_STAGE_TYPES];

/*
 *  16x16 FFT stages, scalingOption=3
 */

int fft_16x16_stage_first_scl3_DFT3        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_first_scl3_DFT4        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_first_scl3_DFT5        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int ifft_16x16_stage_first_scl3_DFT3       (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int ifft_16x16_stage_first_scl3_DFT4       (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int ifft_16x16_stage_first_scl3_DFT5       (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl3_DFT2        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl3_DFT3        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl3_DFT4x2      (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_penultimate_scl3_DFT4x4(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_last_scl3_DFT4         (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_last_scl3_DFT8         (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);

int fft_16x16_stage_first_scl3_DFT2        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int ifft_16x16_stage_first_scl3_DFT2       (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl3_DFT4        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl3_DFT4x4      (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_inner_scl3_DFT5        (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_last_scl3_DFT2         (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_last_scl3_DFT3         (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);
int fft_16x16_stage_last_scl3_DFT5         (const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);

/* Table of 16x16 FFT stages with static scaling used in processing */
extern const fft_cplx16x16_stage_t fft16x16_stages_scl3_tbl[NUM_FFT_STAGE_TYPES];

#endif // __FFT_16X16_STAGES_H__
