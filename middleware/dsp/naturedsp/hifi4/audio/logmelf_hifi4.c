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
    Compute log mel filterbank energies
    Single precision floating-point variant
    C code optimized for HiFi4 core
  IntegrIT, 2006-2018
*/
/*
 * References:
 * [1] S. Young, G. Evermann, M. Gales, T. Hain, D. Kershaw, X. Liu, G. Moore,
 *     J. Odell, D. Ollason, D. Povey, V. Valtchev, P. Woodland,
 *     The HTK Book (for HTK version 3.4), 
 *     Cambridge University Engineering Department, 2009. 
 *     http://htk.eng.cam.ac.uk/docs/docs.shtml
 * [2] Auditory Toolbox for MATLAB by Malcolm Slaney
 *     https://uk.mathworks.com/matlabcentral/linkexchange/links/38-auditory-toolbox
 * [3] Reproducing the feature outputs of common programs using Matlab and 
 *     melfcc.m, by Dan Ellis
 *     http://www.ee.columbia.edu/~dpwe/resources/matlab/rastamat/mfccs.html
 */

#include <string.h>

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common_fpu.h"
/*  Log mel filterbank internal definitions. */
#include "logmel_internal.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_audio.h"
#include "NatureDSP_Signal_math.h"
#include "NatureDSP_Signal_complex.h"

#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(size_t          , logmelf_alloc, (const logmel_params_t * params));
DISCARD_FUN(logmelf_handle_t, logmelf_init , (void * objmem, const logmel_params_t * params));
#else

#define ALIGN_SIZE     (XCHAL_DATA_WIDTH)
#define ALIGN_PAD      (ALIGN_SIZE-1)
#define ALIGN_PTR(p)   (void*)(((uintptr_t)(p)+ALIGN_PAD)&~ALIGN_PAD)
#define sz_i16         sizeof(int16_t)
#define sz_f32         sizeof(float32_t)

/* Compute the 32-bit fixed-point reciprocal for input arguments s_fract, s_exp: 
 * r_fract*2^-r_exp ~= 1/(s_fract*2^-s_exp), where r_fract is returned value and
 * r_exp is the output argument. */
static fract32 recip32( int16_t * r_exp, int32_t s_fract, int16_t s_exp );

/* Linear to mel-scale frequency conversion, HTK flavor. */
static float32_t hz2mel_htk( float32_t hz );
/* Mel-scale to linear frequency conversion, HTK flavor. */
static float32_t mel2hz_htk( float32_t mel );
/* Linear to mel-scale frequency conversion, Auditory flavor. */
static float32_t hz2mel_slaney( float32_t hz );
/* Mel-scale to linear frequency conversion, Auditory flavor. */
static float32_t mel2hz_slaney( float32_t mel );

/* Compute weighting functions and (optionally) normalization scale factors. Both the HTK
 * Auditory flavours are supported, as specified by the input argument isHtk. */
static void buildFilterbank( float32_t * restrict weights,   /* [binUpp-binLow], Out */
                             int16_t   * restrict segments,  /* [bandNum+1    ], Out */
                             float32_t * restrict fbeScales, /* [bandNum      ], Out, optional */
                             int Fs, int fftSize, fract32 lowFreq_q8, fract32 uppFreq_q8, 
                             int bandNum, int binLow, int binUpp, int isHtk );

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

/* Returns: size of memory to be allocated for an instance object, in bytes. */
size_t logmelf_alloc( const logmel_params_t * params )
{
    int binLow, binUpp;
    size_t allocSize;
    NASSERT(NULL!=params);
    /* Check if logmel parameters meet the restrictions stated by the API. */
    if (!(8000<=params->Fs && params->Fs<=48000)                                                                   ||
        !(params->fftSize==256 || params->fftSize==512 || params->fftSize==1024 || params->fftSize==2048)          ||
        !(0<=params->mfbLowFreqQ8 && params->mfbLowFreqQ8<params->mfbUppFreqQ8 && params->mfbUppFreqQ8<=16000*256) ||
        !(0<params->mfbBandNum && params->mfbBandNum<=40)
       ) {
        NASSERT(!"logmelf_alloc(): bad params"); return (0);
    }
    /* Determine the number of FFT bins in the filterbank frequency range. */
    if (LOGMEL_OPT_EQ(params->opt, MELSCALE, HTK)) {
        logmel_binRange_htk(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    } else {
        logmel_binRange_auditory(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    }
    /* Compute the allocation size. */
    allocSize = ALIGN_PAD + sizeof(logmelf_t)              +    /* Instance structure */
                ALIGN_PAD + (binUpp-binLow)*sz_f32         +    /* weights            */
                ALIGN_PAD + (params->mfbBandNum+1)*sz_i16;      /* segments           */
    if (LOGMEL_OPT_EQ(params->opt, FBNORM, AREA)) {
        allocSize += ALIGN_PAD + (params->mfbBandNum+1)*sz_f32; /* fbeScales          */
    }
    return allocSize;
} /* logmelf_alloc() */

/* Returns: handle to the object, or NULL if initialization failed. */
logmelf_handle_t logmelf_init( void * objmem, const logmel_params_t * params )
{
    void *p = objmem;
    logmelf_t *logmel;
    float32_t *weights, *fbeScales = NULL;
    int16_t *segments;
    size_t allocSize;
    int binLow, binUpp;
    int isHtk;
    /* Validate logmel parameters and recalculate the instance object size. */
    if (0==(allocSize = logmelf_alloc(params))) return NULL;
    /* Select either the HTK or Auditory filterbank flavor. */
    isHtk = LOGMEL_OPT_EQ(params->opt, MELSCALE, HTK);
    /* Determine the number of FFT bins in the filterbank frequency range. */
    if (isHtk) {
        logmel_binRange_htk(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    } else {
        logmel_binRange_auditory(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    }
    /* Partition the object memory space. */
    logmel   = (logmelf_t*)ALIGN_PTR(p); p = logmel+1;
    weights  = (float32_t*)ALIGN_PTR(p); p = weights + (binUpp-binLow);
    segments = (int16_t  *)ALIGN_PTR(p); p = segments + (params->mfbBandNum+1);
    if (LOGMEL_OPT_EQ(params->opt, FBNORM, AREA)) {
        fbeScales = (float32_t*)ALIGN_PTR(p); p = fbeScales + params->mfbBandNum;
    }
    NASSERT((uint8_t*)p - (uint8_t*)objmem <= (int)allocSize);
    /* Build the filterbank. */
    buildFilterbank(weights, segments, fbeScales, params->Fs, params->fftSize, 
                    params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->mfbBandNum,
                    binLow, binUpp, isHtk);
    /* Initialize the instance object. */
    memset(logmel, 0, sizeof(*logmel));
    logmel->magic     = LOGMELF_MAGIC;
    logmel->params    = *params;
    logmel->binLow    = binLow;
    logmel->binUpp    = binUpp;
    logmel->weights   = weights;
    logmel->segments  = segments;
    logmel->fbeScales = fbeScales;
    return (logmelf_handle_t)logmel;
} /* logmelf_init() */

/* Compute the 32-bit fixed-point reciprocal for input arguments s_fract, s_exp: 
 * r_fract*2^-r_exp ~= 1/(s_fract*2^-s_exp), where r_fract is returned value and
 * r_exp is the output argument. */
fract32 recip32( int16_t * r_exp, int32_t s_fract, int16_t s_exp )
{
    fract32 r_fract;
    vec_recip32x32(&r_fract, r_exp, &s_fract, 1);
    *r_exp = 62-s_exp-*r_exp;
    return r_fract;
} /* recip32() */

/* Linear to mel-scale frequency conversion, HTK flavor. */
float32_t hz2mel_htk( float32_t hz )
{
    NASSERT(hz>=0.f);
    /* We use HTK's mapping from linear to mel-scale frequency, as defined 
     * in [1] p.76 Eq.(5.13). */
    return 2595.f*scl_log10f(1.f+(1.f/700.f)*hz);
} /* hz2mel_htk() */ 

/* Mel-scale to linear frequency conversion, HTK flavor. */
float32_t mel2hz_htk( float32_t mel )
{
    /* Inverse of HTK's formula, see [1] p.76 Eq.(5.13). */
    return scl_antilog10f((1/2595.f)*mel)*700.f-700.f;
} /* mel2hz_htk() */

/* Linear to mel-scale frequency conversion, Auditory flavor. */
float32_t hz2mel_slaney( float32_t hz )
{
    /* This mapping from linear to mel-scale frequency was implicitly defined in
     * mfcc.m from the Auditory toolbox [2]. Formula and constants reverse engineered
     * by Dan Ellis in [3].
     * MATLAB reference:
     *   mel = hz2mel_slaney(hz)
     *   lin = hz<1000;
     *   mel(lin) = hz(lin);
     *   mel(~lin) = 1000+1800*log(hz(~lin)/1000)/log(6.4);
     * After trivial transformations, we obtain the following formula for hz>=1000:
     *   mel(~lin) = 1800/log2(6.4)*log2(hz(~lin))+1000-5400/log10(6.4); */
    if (hz>=1000) {
        return 672.125343820555f*scl_log2f(hz)-5698.25618876999f;
    } else {
        return hz;
    }
} /* hz2mel_slaney() */

/* Mel-scale to linear frequency conversion, Auditory flavor. */
float32_t mel2hz_slaney( float32_t mel )
{
    /* This mapping from mel to linear-scale frequency was implicitly defined in
     * mfcc.m from the Auditory toolbox [2]. Formula and constants reverse engineered
     * by Dan Ellis in [3].
     * MATLAB reference:
     *   hz = mel2hz_slaney(mel)
     *   lin = mel<1000;
     *   hz(lin) = mel(lin);
     *   hz(~lin) = 1000*exp((mel(~lin)-1000)/1800*log(6.4));
     * After trivial transformations, we obtain the following formula for mel>=1000:
     *   hz(~lin) = (1000/6.4^(5/9))*2.^(mel(~lin)*(log2(6.4)/1800)); */
    if (mel>=1000) {
        return 356.551474406322f*scl_antilog2f(mel*1.48781772506258e-3f);
    } else {
        return mel;
    }
} /* mel2hz_slaney() */

/* Compute weighting functions and (optionally) normalization scale factors. Both the HTK
 * Auditory flavours are supported, as specified by the input argument isHtk. */
void buildFilterbank( float32_t * restrict weights,   /* [binUpp-binLow], Out */
                      int16_t   * restrict segments,  /* [bandNum+1    ], Out */
                      float32_t * restrict fbeScales, /* [bandNum      ], Out, optional */
                      int Fs, int fftSize, fract32 lowFreq_q8, fract32 uppFreq_q8, 
                      int bandNum, int binLow, int binUpp, int isHtk )
{
    /* Besides of dissimilar mel<->linear frequencies mappings, HTK and Auditory filterbanks
     * differ in a couple of minor aspects:
     * 1. HTK computes triangular weighting functions from mel-scale ratios, while Auditory 
     *    uses linear frequencies.
     * 2. HTK rounds lower/upper frequency boundaries to nearest FFT bins, then truncates the
     *    range by one bin from the left and right sides. Auditory uses ceil and floor when
     *    converting lower and upper frequency boundaries to FFT bins, and performs no truncation.
     * Regarding these issues, we follow the HTK when the user selects HTK's mel frequency mapping,
     * (isHtk!=0) or use the Auditory approach otherwise (isHtk==0). */
    float32_t lowFreq, uppFreq, Fsf, f, s;
    float32_t linL, linR, melL, melR; /* "lin" refers to linear frequency, "L" and "R" denote
                                       * left and right edges of the current segment. */
    float32_t melLow, melUpp, melStp; /* Mel-scale frequencies and step between edges */
    int       binL, binR; /* FFT bin indices */
    float32_t bandL=0; /* Left edge of the current band */
    fract32 invFs_fract; int16_t invFs_exp; /* 1/Fs ~= invFs_fract*2^-invFs_exp */
    int16_t fftLog;
    int m, n;
    /* Select the mel<->linear-scale mapping functions */
    float32_t (*hz2mel)(float32_t) = isHtk ? hz2mel_htk : hz2mel_slaney;
    float32_t (*mel2hz)(float32_t) = isHtk ? mel2hz_htk : mel2hz_slaney;
    /* Linear and mel-frequencies range */
    Fsf = XT_FLOAT_S(Fs, 0);
    lowFreq = XT_FLOAT_S(lowFreq_q8, 8); melLow = hz2mel(lowFreq);
    uppFreq = XT_FLOAT_S(uppFreq_q8, 8); melUpp = hz2mel(uppFreq); 
    melStp = (melUpp-melLow)/(float32_t)XT_FLOAT_S(bandNum+1, 0);
    /* s <- fftSize/Fs */
    fftLog = 30-NSA(fftSize);
    invFs_fract = recip32(&invFs_exp, Fs, 0);
    s = scl_int2float(invFs_fract, fftLog-invFs_exp);
    /* Compute positive-slope sides for bandNum triangles, 1/2 overlapping. */
    linL = lowFreq; melL = melLow; binL = binLow;
    for ( m=0; m<bandNum; m++ ) {
        /* Right-hand edge, i.e. the central frequency of m-th band. */
        melR = melLow+melStp*(float32_t)XT_FLOAT_S(m+1, 0);
        linR = mel2hz(melR);
        /* binR = max(binLow,min(binUpp,floor(mel2hz(melR)/Fs*fftSize)+1)); *
         * Two-side saturation is essentially required because of rounding used by HTK
         * when converting the frequency range to binLow, binUpp. */
        binR = XT_MAX(binLow, XT_MIN(binUpp, XT_TRUNC_S(linR*s, 0) + 1));
        /* Compute weights for FFT bins that belong to positive-slope side of
         * m-th triangle. Note that binR is NOT included, actually it belongs to
         * the positive-slope side of the next triangle. */
        for ( n=binL; n<binR; n++ ) {
            /* f = n/fftSize*Fs; */
            f = scl_int2float(n, -fftLog)*Fsf;
            /* Note that HTK computes weights from mel-scale ratios, while Auditory
             * computes ratios in linear domain. */
            if (isHtk) {
                /* weights(n-binLow+1) = (hz2mel(f)-melL)/melStp; */
                weights[n-binLow] = (hz2mel(f)-melL)/melStp;
            } else {
                /* weights(n-binLow+1) = (n/fftSize*Fs-linL)/(linR-linL); */
                weights[n-binLow] = (f-linL)/(linR-linL);
            }
        } /* n */
        segments[m] = (int16_t)(binR-binL);
        if (NULL!=fbeScales) {
            /* Compute reciprocal bandwidth of m-1st band. */
            if (m>0) fbeScales[m-1] = 2.f/(linR-bandL);
            /* Remeber the left (lower) frequency of m-th band. */
            bandL = linL;
        }
        melL = melR; linL = linR; binL = binR;
    } /* m */
    /* Compute the negative-slope side of the last triangle. */
    linR = uppFreq; melR = melUpp; binR = binUpp;
    for ( n=binL; n<binR; n++ ) {
        /* f = n/fftSize*Fs; */
        f = scl_int2float(n, -fftLog)*Fsf;
        if (isHtk) {
            /* weights(n-binLow+1) = (melR-hz2mel(f))/melStp; */
            weights[n-binLow] = (melR-hz2mel(f))/melStp;
        } else {
            /* weights(n-binLow+1) = (linR-n/fftSize*Fs)/(linR-linL); */
            weights[n-binLow] = (linR-f)/(linR-linL);
        }
    } /* n */
    segments[bandNum] = (int16_t)(binR-binL);
    if (NULL!=fbeScales) {
        /* Reciprocal bandwidth of the right-most band. */
        fbeScales[bandNum-1] = 2/(linR-bandL);
    }
} /* buildFilterbank() */

#endif /* HAVE_ */
