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
    Common function definitions
    C code optimized for HiFi4 core
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
/*  Log mel filterbank internal definitions. */
#include "logmel_internal.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_math.h"

/* Compute the 32-bit fixed-point reciprocal for input arguments s_fract, s_exp: 
 * r_fract*2^-r_exp ~= 1/(s_fract*2^-s_exp), where r_fract is returned value and
 * r_exp is the output argument. */
static fract32 recip32( int16_t * r_exp, int32_t s_fract, int16_t s_exp )
{
    fract32 r_fract;
    vec_recip32x32(&r_fract, r_exp, &s_fract, 1);
    *r_exp = 62-s_exp-*r_exp;
    return r_fract;
} /* recip32() */

/* Convert a fixed-point Q14.17 frequency value from Hz to FFT bin number. Input arguments
 * invFs_fract and invFs_exp specify the reciprocal sampling rate: 1/Fs ~= invFs_fract*2^-invFs_exp.
 * Input argument rndUp selects the rounding mode: 0 - truncate; 1 - round towards +inf.  */
int logmel_hz2bin( fract32 hz_q17, fract32 invFs_fract, int16_t invFs_exp, int fftSize, int roundOpt )
{
    int sft, fftLog = 30-NSA(fftSize);
    int32_t rndTerm;
    ae_f32  bin, binNum;
    /* Q0 <- [Q17*Q(invFs_exp) - 31]*fftSize - (17+invFs_exp-31) */
    sft = -fftLog+17+invFs_exp-31; NASSERT(0<=sft && sft<=31);

    NASSERT(roundOpt==LOGMEL_HZ2BIN_ROUNDOPT_DOWN || 
            roundOpt==LOGMEL_HZ2BIN_ROUNDOPT_UP   || 
            roundOpt==LOGMEL_HZ2BIN_ROUNDOPT_NEAR );
    /* for LOGMEL_HZ2BIN_ROUNDOPT_DOWN */
    rndTerm = 0;
    /* for LOGMEL_HZ2BIN_ROUNDOPT_UP */
    XT_MOVEQZ(rndTerm, (1L<<sft)-1, roundOpt-LOGMEL_HZ2BIN_ROUNDOPT_UP);
    /* for LOGMEL_HZ2BIN_ROUNDOPT_NEAR */
    XT_MOVEQZ(rndTerm, (1L<<sft)>>1, roundOpt-LOGMEL_HZ2BIN_ROUNDOPT_NEAR);

    binNum = AE_F32_MULF_F32(hz_q17, invFs_fract);
    bin = AE_F32_SRA(AE_F32_ADDS_F32(binNum, rndTerm), sft);

    return (int)bin;
} /* logmel_hz2bin() */

/* Convert lower/upper frequency boundaries to FFT bin numbers. Output argument binLow specifies
 * the first bin that belongs to the range, binUpp specifies the next to last bin of the range. */
void logmel_binRange_htk(int * binLow, int * binUpp, fract32 lowFreq_q8, fract32 uppFreq_q8, int Fs, int fftSize)
{
    int low, upp;
    fract32 invFs_fract; int16_t invFs_exp; /* 1/Fs ~= invFs_fract*2^-invFs_exp */
    /* Translate lower/upper frequencies to FFT bins (zero-based!). Note that 
     * HTK rounds frequency range boundaries to nearest FFT bins, and truncates
     * the resulting range by one bin from the left and right.
     *   binLow = floor(lowFreq/Fs*fftSize+0.5)+1; % First bin
     *   binUpp = min(floor(uppFreq/Fs*fftSize+0.5),fftSize/2+1); % Next to last bin! */
    invFs_fract = recip32(&invFs_exp, Fs, 0);
    low = logmel_hz2bin(AE_F32_SLAIS(lowFreq_q8, 17-8), invFs_fract, invFs_exp, fftSize, LOGMEL_HZ2BIN_ROUNDOPT_NEAR) + 1;
    upp = logmel_hz2bin(AE_F32_SLAIS(uppFreq_q8, 17-8), invFs_fract, invFs_exp, fftSize, LOGMEL_HZ2BIN_ROUNDOPT_NEAR);
    /* Take sanity measures: 
     *   1. Filterbank must not extend beyond the Nyquist frequency. 
     *   2. For a marginally narrow frequency range, lower and upper bin numbers may swap after
     *      the range is truncated from the left and right. In this case, force the zero difference 
     *      binUpp-binLow. */
    upp = XT_MIN(upp, fftSize/2+1); low = XT_MIN(low,upp);
    *binLow = low; *binUpp = upp;
} /* logmel_binRange_htk() */

/* Convert lower/upper frequency boundaries to FFT bin numbers. Output argument binLow specifies
 * the first bin that belongs to the range, binUpp specifies the next to last bin of the range. */
void logmel_binRange_auditory(int * binLow, int * binUpp, fract32 lowFreq_q8, fract32 uppFreq_q8, int Fs, int fftSize)
{
    int low, upp;
    fract32 invFs_fract; int16_t invFs_exp; /* 1/Fs ~= invFs_fract*2^-invFs_exp */
    /* Translate frequency range boundaries to FFT bin indices: *
     *   binLow = ceil(lowFreq/Fs*fftSize); % Round towards +Inf
     *   binUpp = min(floor(uppFreq/Fs*fftSize)+1,fftSize/2+1); % Next to last bin! */
    invFs_fract = recip32(&invFs_exp, Fs, 0);
    low = logmel_hz2bin(AE_F32_SLAIS(lowFreq_q8, 17-8), invFs_fract, invFs_exp, fftSize, LOGMEL_HZ2BIN_ROUNDOPT_UP);
    upp = logmel_hz2bin(AE_F32_SLAIS(uppFreq_q8, 17-8), invFs_fract, invFs_exp, fftSize, LOGMEL_HZ2BIN_ROUNDOPT_DOWN) + 1;
    upp = XT_MIN(upp, fftSize/2+1);
    NASSERT(0<=low && low<=upp);
    *binLow = low; *binUpp = upp;
} /* logmel_binRange_auditory() */
