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
      http:                                   
  In addition, a number of options provide an ability to emulate the operation of
  another popular package for speech analysis:
  [2] The Auditory Toolbox for MATLAB by Malcolm Slaney, Version 2
      Interval Research Corporation
      https:                                                    
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
    Common function definitions
    C code optimized for HiFi4 core
  IntegrIT, 2006-2018
*/

#include <string.h>

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_audio.h"

/* Fill the parameters structure with default values. */
void mfcc_getDefaultParams( mfcc_params_t * params )
{
    memset(params, 0, sizeof(*params));
    params->Fs           = 16000   ; /* Sampling rate, Hz                                                              */
    params->scaleExp     = 15      ; /* Specifies the scaling factor applied to speech signal: 2^scaleExp              */
    params->preemph      = 31785   ; /* Pre-emphasis filter coeff, Q15; set to 0 to disable the filter                 */
    params->fftSize      = 512     ; /* FFT size: 512, 1024 or 2048 samples                                            */
    params->stftWinLen   = 400     ; /* Short-time Fourier transform window length, must not exceed fftSize            */
    params->stftHopLen   = 160     ; /* Number of audio smaples between successive windows, must not exceed stftWinLen */
    params->mfbLowFreqQ8 = 0*256   ; /* Lowest band's left frequency edge, Hz (Q8)                                     */
    params->mfbUppFreqQ8 = 4000*256; /* Uppermost band's right frequency edge, Hz (Q8)                                 */
    params->mfbBandNum   = 20      ; /* Number of mel filterbank spectral bands                                        */
    params->cepstraNum   = 13      ; /* Number of cepstral coeffs to compute, including the 0th coeff                  */
    params->lifter       = 22      ; /* Cepstral lifter parameter; zet to zero to disable the lifter                   */
    params->opt          = 0       ; /* Options to control various aspects of MFCC features extraction                 */
} /* mfcc_getDefaultParams() */
