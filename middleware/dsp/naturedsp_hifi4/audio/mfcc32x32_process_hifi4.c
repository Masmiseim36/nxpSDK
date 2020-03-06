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
    32-bit fixed-point variant
    C code optimized for HiFi4 core
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_audio.h"
#include "NatureDSP_Signal_vector.h"
/* MFCC extractor internal declarations. */
#include "mfcc_internal.h"

#define PROFILE_ENABLE  0 /* If non-zero, measure cycles and print a report to stdout. */
#define ALIGN_SIZE      (XCHAL_DATA_WIDTH)
#define ALIGN_PAD       (ALIGN_SIZE-1)
#define ALIGN_PTR(p)    (void*)(((uintptr_t)(p)+ALIGN_PAD)&~ALIGN_PAD)
#define sz_i32          sizeof(fract32)
#define sz_i32c         sizeof(complex_fract32)

/* Integrated profiler. */
#include "profile.h"

PROFILE_CREATE(mfcc32x32_stftWinUpdate);
PROFILE_CREATE(mfcc32x32_preemph);
PROFILE_CREATE(mfcc32x32_remdc);
PROFILE_CREATE(mfcc32x32_prepareFftFrame);
PROFILE_CREATE(mfcc32x32_rfft);
PROFILE_CREATE(mfcc32x32_dct);
PROFILE_CREATE(mfcc32x32_postProcessing);

/* When in profiling mode, it should be also explicitly enabled in logmel32x32_process source file. */
PROFILE_EXTERN(logmel32x32_normalizeSpectra);
PROFILE_EXTERN(logmel32x32_vec_complex2mag);
PROFILE_EXTERN(logmel32x32_computeFbe);
PROFILE_EXTERN(logmel32x32_logScaleFbe);

/* Copy 32-bit words from the input argument src[len] to the output argument dst[len], and
 * return the minimum number of redundant sign bits (a.k.a block exponent or normalization
 * shift amount) over absolute values of copied samples. Input and output arguments src[len]
 * and dst[len] must be aligned by ALIGN_SIZE-bytes, the number of samples len must be even 
 * and positive. */
static int copy_bexp_32b(fract32 * restrict dst, const fract32 * restrict src, int len);

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
void mfcc32x32_process( mfcc32x32_handle_t handle, void * restrict pScr, fract32 * restrict cepstra, const fract32 * restrict speech )
{
    mfcc32x32_t * mfcc = (mfcc32x32_t*)handle;
    void *p = pScr, *logmel_pScr;
    const mfcc_params_t * restrict params;
    fract32 *f, *pf, *logFbe, *features;
    complex_fract32 *spectra; 
    int winLen, hopLen, fftExp, logbn, sft, bexp=31;
    NASSERT(NULL!=mfcc && mfcc->magic==MFCC32X32_MAGIC);
    NASSERT_ALIGN(cepstra, ALIGN_SIZE);
    NASSERT_ALIGN(speech, ALIGN_SIZE);
    NASSERT(mfcc->callback.rfft);
    NASSERT(mfcc->mtxvecmpy);
    params = &mfcc->params;
    NASSERT((params->lifter>0)==(NULL!=mfcc->lifterCoefs));
    /* Partition the scratch memory. */
    f        = (fract32        *)ALIGN_PTR(p); p = f + params->fftSize;
    spectra  = (complex_fract32*)ALIGN_PTR(p); p = spectra + params->fftSize/2+1;
    features = (fract32        *)ALIGN_PTR(p); p = features + mfcc->dctRows;
    logFbe   = (fract32        *)ALIGN_PTR(p); p = logFbe + mfcc->dctCols;
    logmel_pScr = p;
#ifdef _DEBUG
    {   /* Check that the scratch size is enough to fit all temporary arrays. 
         * This step is skipped for non-debug builds, because _getScratchSize()
         * functions may involve time-consuming computations. */
        logmel_params_t logmel_params;
        LOGMEL_PARAMS_FROM_MFCC_PARAMS(&logmel_params, &mfcc->params);
        p = (uint8_t*)logmel_pScr + logmel32x32_getScratchSize(&logmel_params);
        NASSERT((uint8_t*)p - (uint8_t*)pScr <= (int)mfcc32x32_getScratchSize(params));
    }
#endif
    /* Marginal initialization for the memory debugger. */
    AE_S32X2_X(AE_ZERO32(), (ae_int32x2*)spectra, (params->fftSize/2+1)*sz_i32c);
    AE_S32X2_X(AE_ZERO32(), (ae_int32x2*)features, mfcc->dctRows*sz_i32);
    /* Persistent STFT frame */
    pf = mfcc->stftFrame;
    PROFILE_START(mfcc32x32_stftWinUpdate);
    /* Update the STFT frame: keep winLen-hopLen samples from previous call(s)
     * and append new speech samples. Input scaling by 2^15 mimics the HTK. */
    winLen = params->stftWinLen; hopLen = params->stftHopLen;
    if (winLen!=hopLen) bexp = copy_bexp_32b(pf, pf+hopLen, winLen-hopLen);
    bexp = XT_MIN(bexp, copy_bexp_32b(pf+winLen-hopLen, speech, hopLen));
    /* Determine the minimum number of redundant sign bits over the STFT frame and the pre-
     * emphasis filter state. To prevent overflow in DC removal and pre-emphasis filtering,
     * the block exponent is decreased by 1, so that the signal gets divided by 2.*/
    bexp = XT_MIN(bexp, NSA(mfcc->preemphState)) - 1;
    /* Scale data up, otherwise numeric performance of fixed-point DC removal and 
     * pre-emphasis filtering would be degraded for a weak signal. */
    vec_shift32x32(f, pf, bexp, winLen);
    PROFILE_STOP(mfcc32x32_stftWinUpdate);
    /* Perform pre-emphasis filtering, with initial filter state preserved from
     * the previous frame, as it is done in Auditory. Note that this should be
     * accomplished strictly BEFORE the DC mean removal! */
    if (0!=params->preemph && MFCC_OPT_EQ(params->opt, PREEMPH, CONTINUOUS)) {
        /* Shift amount may be negative! */
        fract32 t = AE_MOVAD32_L(AE_SLAA32S(AE_MOVDA32(mfcc->preemphState), bexp));
        PROFILE_START(mfcc32x32_preemph);
        mfcc32x32_preemph(f, f, params->preemph, t, winLen);
        PROFILE_STOP(mfcc32x32_preemph);
        mfcc->preemphState = pf[hopLen-1];
    }
    /* Optionally subtract the DC mean, estimated for the current frame (HTK). */
    if (MFCC_OPT_EQ(params->opt, DC_MEAN, REMOVE)) {
        PROFILE_START(mfcc32x32_remdc);
        mfcc32x32_remdc(f, f, winLen);
        PROFILE_STOP(mfcc32x32_remdc);
    }
    /* Pre-emphasis filtering, with filter state reset on each frame. This
     * variant of pre-processing should be done AFTER DC mean removal to keep
     * numeric results close to HTK. */
    if (0!=params->preemph && MFCC_OPT_EQ(params->opt, PREEMPH, FRAMEBYFRAME)) {
        PROFILE_START(mfcc32x32_preemph);
        mfcc32x32_preemph(f, f, params->preemph, f[0], winLen);
        PROFILE_STOP(mfcc32x32_preemph);
    }
    PROFILE_START(mfcc32x32_prepareFftFrame);
    /* Optionally apply the weighting window. */
    if (mfcc->stftWeights) {
        mfcc32x32_vecmpy(f, f, mfcc->stftWeights, winLen);
    }
    /* Pad the weighted signal with zeros to match the FFT size. */
    MFCC_ZERO_32b_ALIGNED(f+winLen, params->fftSize-winLen);
    PROFILE_STOP(mfcc32x32_prepareFftFrame);
    /* Perform the real-to-complex FFT. */
    PROFILE_START(mfcc32x32_rfft);
    fftExp = mfcc->callback.rfft(mfcc->callback.host, spectra, f, params->fftSize);
    PROFILE_STOP(mfcc32x32_rfft);
    /* Apply mel filterbank to obtain log-scaled filterbank energies. logFbe[] format is Q6.25. */
    logmel32x32_process(mfcc->logmel, logmel_pScr, logFbe, spectra, mfcc->params.scaleExp-bexp+fftExp);
    PROFILE_START(mfcc32x32_dct);
    /* The number of columns of the DCT matrix may exceed the actual DCT size, so that
     * we can use a more efficient matrix-vector multiply routine. In such a case, a
     * DCT input vector should be padded with zeros. */
    MFCC_ZERO_32b(logFbe+params->mfbBandNum, mfcc->dctCols-params->mfbBandNum);
    /* Log-scaled FBEs are passed through the DCT to form cepstral coefficients. Note
     * that the number of results (params->dctRows) may be greater than the number of
     * needed cepstral coeffs (params->cepstraNum), for the same reason on efficient
     * matrix-vector multiplication. */
    logbn = 31-NSA(params->mfbBandNum);
    mfcc->mtxvecmpy(features, mfcc->dctMtx, logFbe, mfcc->dctRows, mfcc->dctCols, -logbn);
    PROFILE_STOP(mfcc32x32_dct);
    PROFILE_START(mfcc32x32_postProcessing);
    /* Optionally lifter the cepstral features. */
    if (params->lifter>0) {
        /*  Q(25-dctExp-logbn-lifterExp) <- Q(25-dctExp-logbn)*Q(31-lifterExp) - 31 */
        mfcc32x32_vecmpy(features, features, mfcc->lifterCoefs, params->cepstraNum);
    }
    /* Scale results to form output data:
     * Q(MFCC_CEPSTRA_FRACT_BITS) <- Q(25-dctExp-logbn-lifterExp) */
    sft = MFCC_CEPSTRA_FRACT_BITS-25+mfcc->dctExp+logbn+mfcc->lifterExp;
    vec_shift32x32(cepstra, features, sft, params->cepstraNum);
    PROFILE_STOP(mfcc32x32_postProcessing);
    /* Print cycle scores. */
    PROFILE_REPORT(mfcc32x32_stftWinUpdate);
    PROFILE_REPORT(mfcc32x32_preemph);
    PROFILE_REPORT(mfcc32x32_remdc);
    PROFILE_REPORT(mfcc32x32_prepareFftFrame);
    PROFILE_REPORT(mfcc32x32_rfft);
    PROFILE_REPORT(logmel32x32_normalizeSpectra);
    PROFILE_REPORT(logmel32x32_vec_complex2mag);
    PROFILE_REPORT(logmel32x32_computeFbe);
    PROFILE_REPORT(logmel32x32_logScaleFbe);
    PROFILE_REPORT(mfcc32x32_dct);
    PROFILE_REPORT(mfcc32x32_postProcessing);
} /* mfcc32x32_process() */

/* Returns: size of scratch memory area, in bytes. */
size_t mfcc32x32_getScratchSize( const mfcc_params_t * params )
{
    logmel_params_t logmel_params;
    int dctRows, dctCols;
    NASSERT(NULL!=params);
    /* DCT matrix dimensions. */
    dctRows = MFCC32X32_ALWAYS_USE_MTXVECMPY_FAST ? (params->cepstraNum+3)&~3 : params->cepstraNum;
    dctCols = MFCC32X32_ALWAYS_USE_MTXVECMPY_FAST ? (params->mfbBandNum+3)&~3 : params->mfbBandNum;
    /* Parameters of log mel filterbank operation. */
    LOGMEL_PARAMS_FROM_MFCC_PARAMS(&logmel_params, params);
    return ALIGN_PAD + sz_i32*params->fftSize        + /* f, time domain signal, zero-padded to match the FFT size */
           ALIGN_PAD + sz_i32c*(params->fftSize/2+1) + /* S, Fourier image                                         */
           ALIGN_PAD + sz_i32*dctCols                + /* logFbe, zero-padded to match the DCT matrix size         */
           ALIGN_PAD + sz_i32*dctRows                + /* features, zero-padded to match the DCT matrix size       */
           logmel32x32_getScratchSize(&logmel_params);
} /* mfcc32x32_getScratchSize() */

/* Copy 32-bit words from the input argument src[len] to the output argument dst[len], and
 * return the minimum number of redundant sign bits (a.k.a block exponent or normalization
 * shift amount) over absolute values of copied samples. Input and output arguments src[len]
 * and dst[len] must be aligned by ALIGN_SIZE-bytes, the number of samples len must be even 
 * and positive. */
int copy_bexp_32b(fract32 * restrict dst, const fract32 * restrict src, int len)
{
    const ae_int32x2 * restrict _src = (ae_int32x2*)src;
    ae_int32x2 * restrict _dst = (ae_int32x2*)dst;
    ae_int32x2 acc;
    ae_int32x2 f;
    int n;
    NASSERT_ALIGN(dst, ALIGN_SIZE);
    NASSERT_ALIGN(src, ALIGN_SIZE);
    NASSERT((len>0)&&(0==(len%2)));
    acc = AE_MOVI(0);
    __Pragma("loop_count min=1");
    /* 1 cycles per pipeline stage in steady state with unroll=1 */
    for ( n=0; n<(len>>1); n++ ) {
        AE_L32X2_IP(f, _src, 2*sz_i32);
        acc = AE_MAXABS32S(acc, f);
        AE_S32X2_IP(f, _dst, 2*sz_i32);
    }
    acc = AE_MAXABS32S(acc, AE_SEL32_LH(acc, acc));
    return NSA(AE_MOVAD32_L(acc));
} /* copy_bexp_32b() */
