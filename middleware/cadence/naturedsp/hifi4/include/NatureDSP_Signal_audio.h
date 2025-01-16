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
#ifndef __NATUREDSP_SIGNAL_AUDIO_H__
#define __NATUREDSP_SIGNAL_AUDIO_H__

/* Portable data types. */
#include "NatureDSP_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
  Audio processing functions:
  logmel               Compute log mel filterbank energies
  mfcc                 Mel-frequency cepstrum coefficients extraction
===========================================================================*/

/*-------------------------------------------------------------------------
  Compute log mel filterbank energies
  In the initialization stage, split the frequency range [lowFreq..uppFreq]
  into 1/2 overlapping bands of equal mel-frequency width, and compute triangular
  weighting function for each band. 
  Data processing functions are applied to Fourier image of real signal, specified
  through the input argument spectra[fftSize/2+1]. Fourier image is converted to
  magnitude spectrum. For every mel-frequency band, magnitude samples are multiplied
  by the corresponding triangular weighting function, and summed together to form 
  filterbank energies (FBEs). Finally, log-scaled FBEs are stored to the output
  argument logFbe[mfbBandNum].
  Log mel filterbank routines follow the algorithm used in the Hidden Markov Models
  Toolkit (HTK), as descibed in:
  [1] S. Young, G. Evermann, M. Gales, T. Hain, D. Kershaw, X. Liu, G. Moore,
      J. Odell, D. Ollason, D. Povey, V. Valtchev, P. Woodland,
      The HTK Book (for HTK version 3.4), 
      Cambridge University Engineering Department, 2009. 
      http://htk.eng.cam.ac.uk/docs/docs.shtml
  In a few aspects, such as the linear to mel-scale frequency mapping, the implementation
  may optionally mimic another popular package for speech analysis:
  [2] The Auditory Toolbox for MATLAB by Malcolm Slaney, Version 2
      Interval Research Corporation
      https://engineering.purdue.edu/~malcolm/interval/1998-010/
  Precision: 
  32x32                       32-bit fixed-point input/output data
  f                           Single precision floating-point input/output data
  Input
  objmem                      Memory block allocated for the instance object:
  params                      Parameters of log mel filterbank operation
  spectra[fftSize/2+1]        Fourier image of real signal, positive frequencies only; 
                              Q31 for 32x32
  scaleExp                    Exponent value to scale the Fourier image by a factor 
                              of 2^scaleExp:
                                32x32  For full-scale Q31 real signal the scale exponent
                                       should be set to 15 plus the sum of bit shifts 
                                       applied to data throughout the real-to-complex FFT
                                       transform, as indicated by the respective FFT routine
                                f      For real signal varying in the range [-1,1] the 
                                       scale exponent should be set to 15
  Temporary:
  pScr                        Scratch memory area for the processing function. To 
                              determine the scratch area size, use the respective
                              helper function: logmel<32x32|f>_getScratchSize()
  Output:
  logFbe[mfbBandNum]          Log-scaled filterbank energies; Q6.25 for 32x32
  Restrictions:
  logFbe[],spectra[]          Must not overlap, and must be aligned by 8-bytes
  Fs                          8000 <= Fs <= 48000
  fftSize                     256, 512, 1024 or 2048
  mfbLowFreqQ8, mfbUppFreqQ8  0 <= mfbLowFreqQ8 < mfbUppFreqQ8 <= 16000*256
  mfbBandNum                  0 < mfbBandNum <= 40
-------------------------------------------------------------------------*/

#define LOGMEL_OPT_MELSCALE_HTK       (0<<0) /* Use HTK mapping between linear and mel-scale frequencies              */
#define LOGMEL_OPT_MELSCALE_AUDITORY  (1<<0) /* Use Auditory Toolbox mapping between linear and mel-scale frequencies */
#define LOGMEL_OPT_FBELOG_NATURAL     (0<<1) /* Compute base-e logarithm of filterbank energies (HTK)                 */
#define LOGMEL_OPT_FBELOG_BASE10      (1<<1) /* Compute base-10 logarithm of filterbank energies (Auditory)           */
#define LOGMEL_OPT_FBNORM_NONE        (0<<2) /* No normalization of filterbank weight functions, peak at 1.0 (HTK)    */
#define LOGMEL_OPT_FBNORM_AREA        (1<<2) /* Perform area normalization of filterbank weight functions (Auditory)  */

/* Parameters of log mel filterbank operation. */
typedef struct logmel_params_tag
{
    int     Fs;           /* Sampling rate, Hz                                                                        */
    int     fftSize;      /* FFT size: 256, 512, 1024 or 2048 samples                                                 */
    fract32 mfbLowFreqQ8; /* Lowest band's left frequency edge, Hz (Q8)                                               */
    fract32 mfbUppFreqQ8; /* Uppermost band's right frequency edge, Hz (Q8)                                           */
    int     mfbBandNum;   /* Number of mel filterbank spectral bands                                                  */
    int     opt;          /* Options to control various aspects of filterbank, ORed combination of LOGMEL_OPT_x flags */
} logmel_params_t;

typedef void * logmel32x32_handle_t;
typedef void * logmelf_handle_t;

/* Returns: size of memory to be allocated for an instance object, in bytes. */
size_t logmel32x32_alloc( const logmel_params_t * params );
size_t logmelf_alloc    ( const logmel_params_t * params );

/* Returns: handle to the object, or NULL if initialization failed. */
logmel32x32_handle_t logmel32x32_init( void * objmem, const logmel_params_t * params );
logmelf_handle_t     logmelf_init    ( void * objmem, const logmel_params_t * params );

/* Compute log mel filterbank energies */ 
void logmel32x32_process( logmel32x32_handle_t handle, void * restrict pScr, fract32   * restrict logFbe, const complex_fract32 * restrict spectra, int scaleExp );
void logmelf_process    ( logmelf_handle_t     handle, void * restrict pScr, float32_t * restrict logFbe, const complex_float   * restrict spectra, int scaleExp );

/* Returns: size of scratch memory area, in bytes. */
size_t logmel32x32_getScratchSize( const logmel_params_t * params );
size_t logmelf_getScratchSize    ( const logmel_params_t * params );

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

#define MFCC_OPT_DC_MEAN_REMOVE        (0<<3) /* For every STFT frame, evaluate and subtract the DC mean (HTK)                */
#define MFCC_OPT_DC_MEAN_DONT_REMOVE   (1<<3) /* Do not remove DC mean (Auditory)                                             */
#define MFCC_OPT_PREEMPH_FRAMEBYFRAME  (0<<4) /* Pre-emphasis filter state is reset between STFT frames (HTK)                 */
#define MFCC_OPT_PREEMPH_CONTINUOUS    (1<<4) /* Pre-emphasis filter state is reset once, during initialization (Auditory)    */
#define MFCC_OPT_DCT_NORMALIZED        (0<<5) /* Multiply the DCT-II matrix by (2/N)^0.5, where N is the transform size (HTK) */
#define MFCC_OPT_DCT_ORTHOGONAL        (1<<5) /* ... and multiply the first row by 1/2^0.5 (Auditory)                         */

#define MFCC_CEPSTRA_FRACT_BITS        15     /* Fixed point position for 32x32 cepstral coefficient results. */

/* MFCC extraction parameters. */
typedef struct mfcc_params_tag
{                          /*   Default                                 Description                                           */
    int      Fs;           /* [16000     ] Sampling rate, Hz                                                                  */
    int      scaleExp;     /* [15        ] Specifies the scaling factor applied to speech signal: 2^scaleExp                  */
    fract16  preemph;      /* [0.97*32768] Pre-emphasis filter coeff, Q15; set to 0 to disable the filter                     */
    int      fftSize;      /* [512       ] FFT size: 256, 512, 1024 or 2048 samples                                           */
    int      stftWinLen;   /* [400       ] Short-time Fourier transform window length, must not exceed fftSize                */
    int      stftHopLen;   /* [160       ] Number of audio samples between successive windows, must not exceed stftWinLen     */
    fract32  mfbLowFreqQ8; /* [0*256     ] Lowest band's left frequency edge, Hz (Q8)                                         */
    fract32  mfbUppFreqQ8; /* [4000*256  ] Uppermost band's right frequency edge, Hz (Q8)                                     */
    int      mfbBandNum;   /* [20        ] Number of mel filterbank spectral bands                                            */
    int      cepstraNum;   /* [13        ] Number of cepstral coeffs to compute, including the 0th coeff.                     */
    int      lifter;       /* [22        ] Cepstral lifter parameter; zet to zero to disable the lifter                       */
    int      opt;          /* [0         ] Options to control various aspects of MFCC features extraction, ORed combination   *
                            *              of MFCC_OPT_x and LOGMEL_OPT_x flags.                                              */
} mfcc_params_t;

/* STFT weighting window generator callback function. The number of window samples is specified by the input 
 * argument len. Generated window samples (Q31 for 32x32) must be stored to the output argument window[len],
 * aligned by 8-bytes. */
typedef void mfcc32x32_genWindow_cbfxn_t( void * host, fract32   * window, int len );
typedef void mfccf_genWindow_cbfxn_t    ( void * host, float32_t * window, int len );

/* Real-to-complex FFT callback function. Transform size is specified by the input argument fftSize. Time-domain real
 * input signal is specified through the input argument x[fftSize]. The function must store the positive-frequency
 * spectrum samples to the output argument y[fftSize/2+1].  Fixed-point function variant must also return the block
 * exponent of the Fourier image, which is the sum of bit shifts applied to data throughout the transform. Block 
 * exponent is a signed quantity, with positive values rendering right shifts. Input and output arguments x[fftSize]
 * and y[fftSize/2+1] do not overlap, and are aligned by 8-bytes. The FFT function is allowed to re-use the input
 * argument x[fftSize] for temporal storage of intermediate data. */
typedef int  mfcc32x32_rfft_cbfxn_t( void * host, complex_fract32 * restrict y, fract32   * restrict x, int fftSize );
typedef void mfccf_rfft_cbfxn_t    ( void * host, complex_float   * restrict y, float32_t * restrict x, int fftSize );

/* Callbacks for the fixed-point MFCC extractor (32x32). */
typedef struct mfcc32x32_callback_tag
{
    /* User-supplied host handle to be passed as the first input argument of a callback function. */
    void * host;
    /* Optional STFT weighting window generator function. If NULL, then MFCC assumes the rectangular
     * window, otherwise it invokes the user-supplied function during initialization. */
    mfcc32x32_genWindow_cbfxn_t * genWindow;
    /* Real-to-complex FFT function, is called once per STFT sliding window update. */
    mfcc32x32_rfft_cbfxn_t * rfft;
} mfcc32x32_callback_t;

/* Callbacks for the floating-point MFCC extractor (single precision). */
typedef struct mfccf_callback_tag
{
    /* User-supplied host handle to be passed as the first input argument of a callback function. */
    void * host;
    /* Optional STFT weighting window generator function. If NULL, then MFCC assumes the rectangular
     * window, otherwise it invokes the user-supplied function during initialization. */
    mfccf_genWindow_cbfxn_t * genWindow;
    /* Real-to-complex FFT function, is called once per STFT sliding window update. */
    mfccf_rfft_cbfxn_t * rfft;
} mfccf_callback_t;

/* Instance object handle. */
typedef void * mfcc32x32_handle_t;
typedef void * mfccf_handle_t;

/* Fill the parameters structure with default values. */
void mfcc_getDefaultParams( mfcc_params_t * params );

/* Returns: size of memory to be allocated for an instance object, in bytes. */
size_t mfcc32x32_alloc( const mfcc_params_t * params );
size_t mfccf_alloc    ( const mfcc_params_t * params );

/* Returns: handle to the object, or NULL if initialization failed. */
mfcc32x32_handle_t mfcc32x32_init( void * objmem, const mfcc_params_t * params, const mfcc32x32_callback_t * callback );
mfccf_handle_t     mfccf_init    ( void * objmem, const mfcc_params_t * params, const mfccf_callback_t     * callback );

/* Perform the sliding window STFT analysis and extract the MFCC features */
void mfcc32x32_process( mfcc32x32_handle_t handle, void * restrict pScr, fract32   * restrict cepstra, const fract32   * restrict speech );
void mfccf_process    ( mfccf_handle_t     handle, void * restrict pScr, float32_t * restrict cepstra, const float32_t * restrict speech );

/* Returns: size of scratch memory area, in bytes. */
size_t mfcc32x32_getScratchSize( const mfcc_params_t * params );
size_t mfccf_getScratchSize    ( const mfcc_params_t * params );

#ifdef __cplusplus
};
#endif

#endif /* __NATUREDSP_SIGNAL_AUDIO_H__ */
