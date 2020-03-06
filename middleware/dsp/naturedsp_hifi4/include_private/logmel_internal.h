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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
  NatureDSP Signal Processing Library. Audio processing part
    Log mel filterbank
    Internal declarations
  IntegrIT, 2006-2018
*/

#ifndef __LOGMEL_INTERNAL_H
#define __LOGMEL_INTERNAL_H

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_audio.h"

/* Helper to retrieve individual flags from options bitmask. */
#define LOGMEL_OPT_MELSCALE_MASK           (LOGMEL_OPT_MELSCALE_HTK|LOGMEL_OPT_MELSCALE_AUDITORY)
#define LOGMEL_OPT_FBELOG_MASK             (LOGMEL_OPT_FBELOG_NATURAL|LOGMEL_OPT_FBELOG_BASE10)
#define LOGMEL_OPT_FBNORM_MASK             (LOGMEL_OPT_FBNORM_NONE|LOGMEL_OPT_FBNORM_AREA)
#define LOGMEL_OPT_EQ(opt,feature,value)   (((opt)&(LOGMEL_OPT_##feature##_MASK))==LOGMEL_OPT_##feature##_##value)

/* Instance pointer validation number. */
#define LOGMEL32X32_MAGIC     0xac53c87b
#define LOGMELF_MAGIC         0xb32ea21d

/* Log mel filterbank instance, 32-bit fixed-point variant. */
typedef struct logmel32x32_tag {
    uint32_t        magic;           /* Instance pointer validation number                              */
    logmel_params_t params;          /* Parameters of log mel filterbank operation                      */
    int             bandNum;         /* Number of filterbank bands                                      */
    int             binLow;          /* FFT bin number for the left-most frequency edge                 */
    int             binUpp;          /* FFT bin number for the right-most frequency edge                */
    const fract32 * weights;         /* Filterbank weights, Q31, binUpp-binLow values                   */
    const int16_t * segments;        /* Segmentation of FFT bins w.r.t. filter bands, bandNum+1 values  */
    const fract32 * fbeScales_fract; /* FBE normalization factors, or NULL if normalization is disabled */
    const int16_t * fbeScales_exp;   /* Exponent of normalization factors:                              *
                                      * fbeScales[k] = fbeScales_fract[k]*2^-fbeScales_exp[k]           */
} logmel32x32_t;

/* Log mel filterbank instance, signle precision floating-point variant. */
typedef struct logmelf_tag {
    uint32_t          magic;     /* Instance pointer validation number                              */
    logmel_params_t   params;    /* Parameters of log mel filterbank operation                      */
    int               binLow;    /* FFT bin number for the left-most frequency edge                 */
    int               binUpp;    /* FFT bin number for the right-most frequency edge                */
    const float32_t * weights;   /* Filterbank weights, binUpp-binLow values                        */
    const int16_t   * segments;  /* Segmentation of FFT bins w.r.t. filter bands, bandNum+1 values  */
    const float32_t * fbeScales; /* FBE normalization factors, or NULL if normalization is disabled */
} logmelf_t;

/* Rounding options for the frequency -> FFT bin conversion */
#define LOGMEL_HZ2BIN_ROUNDOPT_DOWN      0 /* Towards zero */
#define LOGMEL_HZ2BIN_ROUNDOPT_UP        1 /* Towards +Inf */
#define LOGMEL_HZ2BIN_ROUNDOPT_NEAR      2 /* To nearest */

/* Convert a fixed-point Q14.17 frequency value from Hz to FFT bin number. Input arguments
 * invFs_fract and invFs_exp specify the reciprocal sampling rate: 1/Fs ~= invFs_fract*2^-invFs_exp.
 * Input argument rndUp selects the rounding mode: 0 - truncate; 1 - round towards +inf.  */
int logmel_hz2bin( fract32 hz_q17, fract32 invFs_fract, int16_t invFs_exp, int fftSize, int roundOpt );

/* Convert lower/upper frequency boundaries to FFT bin numbers. Output argument binLow specifies
 * the first bin that belongs to the range, binUpp specifies the next to last bin of the range. */
void logmel_binRange_htk     (int * binLow, int * binUpp, fract32 lowFreq_q8, fract32 uppFreq_q8, int Fs, int fftSize);
void logmel_binRange_auditory(int * binLow, int * binUpp, fract32 lowFreq_q8, fract32 uppFreq_q8, int Fs, int fftSize);

#endif /* __LOGMEL_INTERNAL_H */
