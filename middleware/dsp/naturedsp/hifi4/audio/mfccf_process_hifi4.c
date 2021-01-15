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
    Compute Mel-Frequency Cepstrum Coefficients (MFCC) from speech signal
    Single precision floating-point variant
    C code optimized for HiF4 core
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common_fpu.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_audio.h"
/* MFCC extractor internal declarations. */
#include "mfcc_internal.h"

#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(size_t, mfccf_getScratchSize, (const mfcc_params_t * params));
DISCARD_FUN(void  , mfccf_process       , (mfccf_handle_t handle, void * restrict pScr, 
                                           float32_t * restrict cepstra, const float32_t * restrict speech));
#else

#define PROFILE_ENABLE  0 /* If non-zero, measure cycles and print a report to stdout. */
#define ALIGN_SIZE      (XCHAL_DATA_WIDTH)
#define ALIGN_PAD       (ALIGN_SIZE-1)
#define ALIGN_PTR(p)    (void*)(((uintptr_t)(p)+ALIGN_PAD)&~ALIGN_PAD)
#define sz_f32          sizeof(float32_t)
#define sz_f32c         sizeof(complex_float)

/* Integrated profiler. */
#include "profile.h"

PROFILE_CREATE(mfccf_stftWinUpdate);
PROFILE_CREATE(mfccf_preemph);
PROFILE_CREATE(mfccf_remdc);
PROFILE_CREATE(mfccf_prepareFftFrame);
PROFILE_CREATE(mfccf_rfft);
PROFILE_CREATE(mfccf_dct);
PROFILE_CREATE(mfccf_postProcessing);

/* When in profiling mode, it should be also explicitly enabled in logmelf_process source file. */
PROFILE_EXTERN(logmelf_vec_complex2mag_logmel);
PROFILE_EXTERN(logmelf_computeFbe);
PROFILE_EXTERN(logmelf_vec_logXf);

/*-------------------------------------------------------------------------
  Compute Mel-Frequency Cepstrum Coefficients (MFCC) from speech signal.
  MFCC features extraction procedure comprises the following steps:
  - input speech signal is optionally passed through a pre-emphasis filter;
  - filtered signal is subject to short-time Fourier transform (STFT) followed by
    magnitude spectrum computation;
  - a set of filters is applied to the magnitude spectrum, with triangular weight
    functions constructed in such a way that the prescribed frequency range 
    is divided into overlapping bands of equal mel-frequency width;
  - log-scaled filterbank energies are decorrelated via a Discrete Cosine Transform 
    Type II (DCT-II) to form cepstrum coefficients;
  - in the last step a sine lifter is optionally applied to cepstra to align 
    coefficient magnitudes.
  In general, the computation procedure follows the MFCC features extraction 
  algorithm adopted in the Hidden Markov Models Toolkit (HTK), as descibed in:
  [1] S. Young, G. Evermann, M. Gales, T. Hain, D. Kershaw, X. Liu, G. Moore,
      J. Odell, D. Ollason, D. Povey, V. Valtchev, P. Woodland,
      The HTK Book (for HTK version 3.4), 
      Cambridge University Engineering Department, 2009. 
      http://htk.eng.cam.ac.uk/docs/docs.shtml
  In addition, a number of options provide an ability to emulate the operation of
  another popular package for speech analysis:
  [2] The Auditory Toolbox for MATLAB by Malcolm Slaney, Version 2
      Interval Research Corporation
      https://engineering.purdue.edu/~malcolm/interval/1998-010/
  Precision: 
  32x32                       32-bit fixed-point input/output data
  f                           Single precision floating-point input/output data
  Input:
  objmem                      Memory block allocated for the instance object:
  params                      MFCC features extraction parameters
  callback                    User-supplied callback functions
  speech[stftHopLen]          Speech samples; Q31 for 32x32
  Temporary:
  pScr                        Scratch memory area for the processing function. To 
                              determine the scratch area size, use the respective
                              helper function: mfcc<32x32|f>_getScratchSize()
  Output:
  cepstra[cepstraNum]         Cepstral coefficients, the number of fractional bits
                              for 32x32 is defined by MFCC_CEPSTRA_FRACT_BITS.
  Restrictions:
  cepstra[],speech[]          Must not overlap, and must be aligned by 8-bytes
  Fs                          8000 <= Fs <= 48000
  fftSize                     256, 512, 1024 or 2048
  stftWinLen, stftHopLen      Must be multiples of 2, 0 < stftHopLen <= stftWinLen <= fftSize
  mfbLowFreqQ8, mfbUppFreqQ8  0 <= mfbLowFreqQ8 < mfbUppFreqQ8 <= 16000*256
  mfbBandNum, cepstraNum      0 < cepstraNum <= mfbBandNum <= 40
-------------------------------------------------------------------------*/

/* Perform the sliding window STFT analysis and extract the MFCC features */
void mfccf_process( mfccf_handle_t handle, void * restrict pScr, float32_t * restrict cepstra, const float32_t * restrict speech )
{
    mfccf_t * mfcc = (mfccf_t*)handle;
    void *p = pScr, *logmel_pScr;
    const mfcc_params_t * restrict params;
    float32_t *f, *pf, *logFbe, *features;
    complex_float *spectra; 
    int winLen, hopLen;
    NASSERT(NULL!=mfcc && mfcc->magic==MFCCF_MAGIC);
    NASSERT_ALIGN(cepstra, ALIGN_SIZE);
    NASSERT_ALIGN(speech, ALIGN_SIZE);
    NASSERT(mfcc->callback.rfft);
    NASSERT(mfcc->mtxvecmpy);
    params = &mfcc->params;
    NASSERT((params->lifter>0)==(NULL!=mfcc->lifterCoefs));
    /* Partition the scratch memory. */
    f        = (float32_t    *)ALIGN_PTR(p); p = f + params->fftSize;
    spectra  = (complex_float*)ALIGN_PTR(p); p = spectra + params->fftSize/2+1;
    logFbe   = (float32_t    *)ALIGN_PTR(p); p = logFbe + mfcc->dctCols;
    features = (float32_t    *)ALIGN_PTR(p); p = features + mfcc->dctRows;
    logmel_pScr = p; 
#ifdef _DEBUG
    {   /* Check that the scratch size is enough to fit all temporary arrays. 
         * This step is skipped for non-debug builds, because _getScratchSize()
         * functions may involve time-consuming computations. */
        logmel_params_t logmel_params;
        LOGMEL_PARAMS_FROM_MFCC_PARAMS(&logmel_params, params);
        p = (uint8_t*)logmel_pScr + logmelf_getScratchSize(&logmel_params);
        NASSERT((uint8_t*)p - (uint8_t*)pScr <= (int)mfccf_getScratchSize(params));
    }
#endif
    /* Persistent STFT frame */
    pf = mfcc->stftFrame;
    /* Update the STFT frame: keep winLen-hopLen samples from previous call(s)
     * and append new speech samples. */
    PROFILE_START(mfccf_stftWinUpdate);
    winLen = params->stftWinLen; hopLen = params->stftHopLen;
    if (winLen!=hopLen) MFCC_COPY_32b_ALIGNED_EVEN(pf, pf+hopLen, winLen-hopLen);
    MFCC_COPY_32b_ALIGNED_EVEN(pf+winLen-hopLen, speech, hopLen);
    PROFILE_STOP(mfccf_stftWinUpdate);
    /* Perform pre-emphasis filtering, with initial filter state preserved from
     * the previous frame, as it is done in Auditory. Note that this should be
     * accomplished strictly BEFORE the DC mean removal! */
    if (0!=params->preemph && MFCC_OPT_EQ(params->opt, PREEMPH, CONTINUOUS)) {
        float32_t t = pf[hopLen-1], alpha = XT_FLOAT_S(params->preemph, 15);
        PROFILE_START(mfccf_preemph);
        mfccf_preemph(f, pf, alpha, mfcc->preemphState, winLen); 
        PROFILE_STOP(mfccf_preemph);
        mfcc->preemphState = t; pf = f;
    }
    /* Optionally subtract the DC mean, estimated for the current frame (HTK). */
    if (MFCC_OPT_EQ(params->opt, DC_MEAN, REMOVE)) {
        PROFILE_START(mfccf_remdc);
        mfccf_remdc(f, pf, winLen); pf = f;
        PROFILE_STOP(mfccf_remdc);
    }
    /* Pre-emphasis filtering, with filter state reset on each frame. This
     * variant of pre-processing should be done AFTER DC mean removal to keep
     * numeric results close to HTK. */
    if (0!=params->preemph && MFCC_OPT_EQ(params->opt, PREEMPH, FRAMEBYFRAME)) {
        float32_t alpha = XT_FLOAT_S(params->preemph, 15);
        PROFILE_START(mfccf_preemph);
        mfccf_preemph(f, pf, alpha, pf[0], winLen); pf = f;
        PROFILE_STOP(mfccf_preemph);
    }
    PROFILE_START(mfccf_prepareFftFrame);
    /* Optionally apply the weighting window. */
    if (mfcc->stftWeights) {
        mfccf_vecmpy(f, pf, mfcc->stftWeights, winLen); pf = f;
    }
    /* If no preprocessing options have been selected, then speech samples still
     * reside in the persistent STFT frame array. FFT will distort its input data, so
     * we have to make a copy in the scratch memory. */
    if (f!=pf) MFCC_COPY_32b_ALIGNED_EVEN(f, pf, winLen);
    /* Pad the weighted signal with zeros to match the FFT size. */
    MFCC_ZERO_32b_ALIGNED(f+winLen, params->fftSize-winLen);
    PROFILE_STOP(mfccf_prepareFftFrame);
    /* Perform the real-to-complex FFT. */
    PROFILE_START(mfccf_rfft);
    mfcc->callback.rfft(mfcc->callback.host, spectra, f, params->fftSize);
    PROFILE_STOP(mfccf_rfft);
    /* Apply mel filterbank to obtain log-scaled filterbank energies. */
    logmelf_process(mfcc->logmel, logmel_pScr, logFbe, spectra, params->scaleExp);
    PROFILE_START(mfccf_dct);
    /* The number of columns of the DCT matrix may exceed the actual DCT size, so that
     * we can use a more efficient matrix-vector multiply routine. In such a case, a
     * DCT input vector should be padded with zeros. */
    MFCC_ZERO_32b(logFbe+params->mfbBandNum, mfcc->dctCols-params->mfbBandNum);
    /* Log-scaled FBEs are passed through the DCT to form cepstral coefficients. Note
     * that the number of results (params->dctRows) may be greater than the number of
     * needed cepstral coeffs (params->cepstraNum), for the same reason on efficient
     * matrix-vector multiplication. */
    mfcc->mtxvecmpy(features, mfcc->dctMtx, logFbe, mfcc->dctRows, mfcc->dctCols);
    PROFILE_STOP(mfccf_dct);
    /* Optionally lifter the cepstral features, otherwise simply copy results to the output array. */
    PROFILE_START(mfccf_postProcessing);
    if (params->lifter>0) {
        mfccf_vecmpy(cepstra, features, mfcc->lifterCoefs, params->cepstraNum);
    } else {
        MFCC_COPY_32b_ALIGNED(cepstra, features, params->cepstraNum);
    }
    PROFILE_STOP(mfccf_postProcessing);
    /* Print cycle scores. */
    PROFILE_REPORT(mfccf_stftWinUpdate);
    PROFILE_REPORT(mfccf_preemph);
    PROFILE_REPORT(mfccf_remdc);
    PROFILE_REPORT(mfccf_prepareFftFrame);
    PROFILE_REPORT(mfccf_rfft);
    PROFILE_REPORT(logmelf_vec_complex2mag_logmel);
    PROFILE_REPORT(logmelf_computeFbe);
    PROFILE_REPORT(logmelf_vec_logXf);
    PROFILE_REPORT(mfccf_dct);
    PROFILE_REPORT(mfccf_postProcessing);
} /* mfccf_process() */

/* Returns: size of scratch memory area, in bytes. */
size_t mfccf_getScratchSize( const mfcc_params_t * params )
{
    logmel_params_t logmel_params;
    int dctRows, dctCols;
    NASSERT(NULL!=params);
    /* DCT matrix dimensions. */
    dctRows = MFCCF_ALWAYS_USE_MTXVECMPY_FAST ? (params->cepstraNum+3)&~3 : params->cepstraNum;
    dctCols = MFCCF_ALWAYS_USE_MTXVECMPY_FAST ? (params->mfbBandNum+3)&~3 : params->mfbBandNum;
    /* Parameters of log mel filterbank operation. */
    LOGMEL_PARAMS_FROM_MFCC_PARAMS(&logmel_params, params);
    return ALIGN_PAD + sz_f32*params->fftSize        + /* f, time domain signal, zero-padded to match the FFT size */
           ALIGN_PAD + sz_f32c*(params->fftSize/2+1) + /* S, Fourier image                                         */
           ALIGN_PAD + sz_f32*dctCols                + /* logFbe, zero-padded to match the DCT matrix size         */
           ALIGN_PAD + sz_f32*dctRows                + /* features, zero-padded to match the DCT matrix size       */
           logmelf_getScratchSize(&logmel_params);
} /* mfccf_getScratchSize() */

#endif /* HAVE_ */
