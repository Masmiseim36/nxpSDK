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

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common_fpu.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_audio.h"
#include "NatureDSP_Signal_math.h"
/*  Log mel filterbank internal definitions. */
#include "logmel_internal.h"
/* +/-infinity floating-point constants */
#include "inff_tbl.h"

#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(size_t, logmelf_getScratchSize, (const logmel_params_t * params));
DISCARD_FUN(void  , logmelf_process       , (logmelf_handle_t handle, void * restrict pScr, float32_t * restrict logFbe,
                                             const complex_float * restrict spectra, int scaleExp));
#else

#define PROFILE_ENABLE  0 /* If non-zero, measure cycles and print a report to stdout. */
#define ALIGN_SIZE      (XCHAL_DATA_WIDTH)
#define ALIGN_PAD       (ALIGN_SIZE-1)
#define ALIGN_PTR(p)    (void*)(((uintptr_t)(p)+ALIGN_PAD)&~ALIGN_PAD)
#define sz_i16          sizeof(int16_t)
#define sz_f32          sizeof(float32_t)

/* Integrated profiler. */
#include "profile.h"

PROFILE_CREATE(logmelf_vec_complex2mag_logmel);
PROFILE_CREATE(logmelf_computeFbe);
PROFILE_CREATE(logmelf_vec_logXf);

/*-------------------------------------------------------------------------
  Complex magnitude
  Routine computes complex magnitude

  Input:
  N            length of vector
  x[N]         input complex data
  Output:
  y[N]         output data
  Temporary:
  pScr[N]      scratch buffer

  Restriction:
  x,y,pScr     Must be 8-bytes aligned
-------------------------------------------------------------------------*/
static void vec_complex2mag_logmel(float32_t * pScr, float32_t * restrict y, const complex_float * restrict x, int N);

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

/* Compute log mel filterbank energies */ 
void logmelf_process( logmelf_handle_t handle, void * restrict pScr, float32_t * restrict logFbe, const complex_float * restrict spectra, int scaleExp )
{
#if HAVE_VFPU
    logmelf_t * logmel = (logmelf_t*)handle;
    void * p = pScr;
    float32_t *cplx2mag_scr;
    float32_t *magspec, *fbe;
    const int16_t   * restrict p_segs;
    const xtfloatx2 * restrict p_s;
    const xtfloatx2 * restrict p_w;
          xtfloat   * restrict p_fbe;
    const xtfloat   * restrict p_fbeScl;
    ae_valign al_s, al_w;
    float32_t v_fbe, fbeScl, accL, accR, w, s, specScl, t;
    xtfloatx2 vaccL, vaccR, vw, vs;
    static const float32_t onef = 1.f;
    int m, n, N, binNum, p_fbeSclInc;
    NASSERT(logmel && logmel->magic==LOGMELF_MAGIC);
    NASSERT_ALIGN(logFbe, XCHAL_DATA_WIDTH);
    NASSERT_ALIGN(spectra, XCHAL_DATA_WIDTH);
    /* Profiler scores should be explicitly reset because this module does not invoke 
     * PROFILE_REPORT macro for them. */
    PROFILE_RESET(logmelf_vec_complex2mag_logmel);
    PROFILE_RESET(logmelf_computeFbe);
    PROFILE_RESET(logmelf_vec_logXf);
    binNum = logmel->binUpp-logmel->binLow;
    /* Partition the scratch memory area. */
    magspec      = (float32_t*)ALIGN_PTR(p); p = magspec + binNum;
    fbe          = (float32_t*)ALIGN_PTR(p); p = fbe + logmel->params.mfbBandNum;
    cplx2mag_scr = (float32_t*)ALIGN_PTR(p); p = cplx2mag_scr + binNum; (void)p;
#ifdef _DEBUG
    /* Check that the scratch size is enough to fit all temporary arrays. 
     * This step is skipped for non-debug builds, because _getScratchSize()
     * function may involve time-consuming computations. */
    NASSERT((uint8_t*)p - (uint8_t*)pScr <= (int)logmelf_getScratchSize(&logmel->params));
#endif
    /* Marginal initialization for the memory debugger. */
    XT_SSX2I(XT_CONST_S(0), (xtfloatx2*)magspec, 0);
    /* FBE normalization option. */
    if (NULL!=logmel->fbeScales) {
        p_fbeScl = (xtfloat*)logmel->fbeScales; p_fbeSclInc = sz_f32;
    } else {
        p_fbeScl = (xtfloat*)&onef; p_fbeSclInc = 0;
    }
    /* Spectra scale factor. */
    specScl = XT_WFR((uint32_t)(scaleExp + 127) << 23);/* specScl = 2^scaleExp */
    /* Compute the magnitude spectrum, for the frequencies of interest only. */
    PROFILE_START(logmelf_vec_complex2mag_logmel);
    vec_complex2mag_logmel(cplx2mag_scr, magspec, spectra+logmel->binLow, binNum);
    PROFILE_STOP(logmelf_vec_complex2mag_logmel);
    PROFILE_START(logmelf_computeFbe);
    p_segs = logmel->segments;
    p_w = (const xtfloatx2 *)logmel->weights;
    p_s = (const xtfloatx2 *)magspec;
    p_fbe = (xtfloat *)fbe;
    /* Positive-slope side of the left-most triangle. */
    vaccL = accL = XT_CONST_S(0);
    N = *p_segs++;
    __Pragma("no_unroll");
    for ( n=0; n<(N>>1); n++ ) {
        XT_LSX2IP(vw, p_w, 2*sz_f32);
        XT_LSX2IP(vs, p_s, 2*sz_f32);
        XT_MADDN_SX2(vaccL, vw, vs);
    } /* n */
    if (N&1) {
        XT_LSIP(w, castxcc(xtfloat,p_w), sz_f32);
        XT_LSIP(s, castxcc(xtfloat,p_s), sz_f32);
        accL = w*s;
    }
    /* Because of 1/2 bands overlapping, each inner segment simultaneously updates
     * a pair of bands: negative-slope side for the left triangle, positive-slope 
     * side for the right triangle. */
    for ( m=0; m<logmel->params.mfbBandNum-1; m++ ) {
        vaccR = accR = XT_CONST_S(0);
        al_w = AE_LA64_PP(p_w);
        al_s = AE_LA64_PP(p_s);
        N = *p_segs++;
        for ( n=0; n<(N>>1); n++ ) {
            xtfloatx2 vt;
            XT_LASX2IP(vw, al_w, p_w);
            XT_LASX2IP(vs, al_s, p_s);
            vt = XT_SUB_SX2(XT_CONST_S(1), vw);
            XT_MADDN_SX2(vaccL, vt, vs);
            XT_MADDN_SX2(vaccR, vw, vs);
        } /* n */
        if (N&1) {
            XT_LSIP(w, castxcc(xtfloat,p_w), sz_f32);
            XT_LSIP(s, castxcc(xtfloat,p_s), sz_f32);
            t = XT_SUB_S(XT_CONST_S(1), w);
            XT_MADDN_S(accL, t, s);
            XT_MADDN_S(accR, w, s);
        }
        accL = accL + (float32_t)XT_RADD_SX2(vaccL);
        XT_LSXP(fbeScl, p_fbeScl, p_fbeSclInc);
        accL = accL*fbeScl;
        /* The lower limit for FBEs mimics the HTK behavior. */
        v_fbe = XT_MAX_S(XT_CONST_S(1), specScl*accL);
        XT_SSIP(v_fbe, p_fbe, sz_f32);
        vaccL = vaccR;
        accL = accR;
    } /* m */
    /* Negative-slope side of the right-most triangle. */
    al_w = AE_LA64_PP(p_w);
    al_s = AE_LA64_PP(p_s);
    N = *p_segs++;
    __Pragma("no_unroll");
    for ( n=0; n<(N>>1); n++ ) {
        XT_LASX2IP(vw, al_w, p_w);
        XT_LASX2IP(vs, al_s, p_s);
        XT_MADDN_SX2(vaccL, vw, vs);
    } /* n */
    if (N&1) {
        XT_LSIP(w, castxcc(xtfloat,p_w), sz_f32);
        XT_LSIP(s, castxcc(xtfloat,p_s), sz_f32);
        XT_MADDN_S(accL, w, s);
    }
    accL = accL + (float32_t)XT_RADD_SX2(vaccL);
    XT_LSXP(fbeScl, p_fbeScl, p_fbeSclInc);
    accL = accL*fbeScl;
    v_fbe = XT_MAX_S(XT_CONST_S(1), specScl*accL);
    XT_SSIP(v_fbe, p_fbe, sz_f32);
    PROFILE_STOP(logmelf_computeFbe);
    /* Log-scale filterbank energies. */
    PROFILE_START(logmelf_vec_logXf);
    if (LOGMEL_OPT_EQ(logmel->params.opt, FBELOG, NATURAL)) {
        vec_lognf(logFbe, fbe, logmel->params.mfbBandNum);
    } else {
        vec_log10f(logFbe, fbe, logmel->params.mfbBandNum);
    }
    PROFILE_STOP(logmelf_vec_logXf);
#else /* HAVE_VFPU */
    logmelf_t * logmel = (logmelf_t*)handle;
    void * p = pScr;
    float32_t *cplx2mag_scr;
    float32_t *magspec, *fbe;
    xtfloat * restrict p_fbe;
    const xtfloat * restrict p_fbeScl;
    const float32_t *ps, *pw;
    float32_t accL, accR, w, s, e, specScl, fbeScl;
    int m, n, binNum, p_fbeSclInc;
    static const float32_t onef = 1.f;
    NASSERT(logmel && logmel->magic==LOGMELF_MAGIC);
    NASSERT_ALIGN(logFbe, XCHAL_DATA_WIDTH);
    NASSERT_ALIGN(spectra, XCHAL_DATA_WIDTH);
    /* Profiler scores should be explicitly reset because this module does not invoke 
     * PROFILE_REPORT macro for them. */
    PROFILE_RESET(logmelf_vec_complex2mag_logmel);
    PROFILE_RESET(logmelf_computeFbe);
    PROFILE_RESET(logmelf_vec_logXf);
    binNum = logmel->binUpp-logmel->binLow;
    /* Partition the scratch memory area. */
    magspec      = (float32_t*)ALIGN_PTR(p); p = magspec + binNum;
    cplx2mag_scr = (float32_t*)ALIGN_PTR(p); p = cplx2mag_scr + binNum;
    fbe          = (float32_t*)ALIGN_PTR(p); p = fbe + logmel->params.mfbBandNum; (void)p;
#ifdef _DEBUG
    /* Check that the scratch size is enough to fit all temporary arrays. 
     * This step is skipped for non-debug builds, because _getScratchSize()
     * function may involve time-consuming computations. */
    NASSERT((uint8_t*)p - (uint8_t*)pScr <= (int)logmelf_getScratchSize(&logmel->params));
#endif
    /* FBE normalization option. */
    if (NULL!=logmel->fbeScales) {
        p_fbeScl = (xtfloat*)logmel->fbeScales; p_fbeSclInc = sz_f32;
    } else {
        p_fbeScl = (xtfloat*)&onef; p_fbeSclInc = 0;
    }
    /* Spectra scale factor. */
    specScl = XT_WFR((uint32_t)(scaleExp + 127) << 23);
    /* Compute the magnitude spectrum, for the frequencies of interest only. */
    PROFILE_START(logmelf_vec_complex2mag_logmel);
    vec_complex2mag_logmel(cplx2mag_scr, magspec, spectra+logmel->binLow, binNum);
    PROFILE_STOP(logmelf_vec_complex2mag_logmel);
    PROFILE_START(logmelf_computeFbe);
    /* Positive-slope side of the left-most triangle. */
    ps = magspec; pw = logmel->weights; accL = XT_CONST_S(0);
    for ( n=0; n<logmel->segments[0]; n++ ) {
        XT_LSIP(w, pw, sz_f32);
        XT_LSIP(s, ps, sz_f32);
        XT_MADD_S(accL, w, s);
    } /* n */
    /* Because of 1/2 bands overlapping, each inner segment simultaneously updates
     * a pair of bands: negative-slope side for the left triangle, positive-slope 
     * side for the right triangle. */
    p_fbe = (xtfloat*)fbe;
    for ( m=0; m<logmel->params.mfbBandNum-1; m++ ) {
        accR = XT_CONST_S(0);
        for ( n=0; n<logmel->segments[m+1]; n++ ) {
            XT_LSIP(w, pw, sz_f32);
            XT_LSIP(s, ps, sz_f32);
            XT_MADD_S(accL, XT_SUB_S(XT_CONST_S(1), w), s);
            XT_MADD_S(accR, w, s);
        }
        XT_LSXP(fbeScl, p_fbeScl, p_fbeSclInc);
        accL = XT_MUL_S(accL, fbeScl);
        /* The lower limit for FBEs mimics the HTK behavior. */
        e = XT_MUL_S(specScl, accL); 
        XT_MOVT_S(e, XT_CONST_S(1), XT_OLT_S(e, XT_CONST_S(1)));
        XT_SSIP(e, p_fbe, sz_f32);
        accL = accR;
    } /* m */
    /* Negative-slope side of the right-most triangle. */
    for ( n=0; n<logmel->segments[logmel->params.mfbBandNum]; n++ ) {
        XT_LSIP(w, pw, sz_f32);
        XT_LSIP(s, ps, sz_f32);
        XT_MADD_S(accL, w, s);
    } /* n */
    XT_LSXP(fbeScl, p_fbeScl, p_fbeSclInc);
    accL = XT_MUL_S(accL, fbeScl);
    e = XT_MUL_S(specScl, accL); 
    XT_MOVT_S(e, XT_CONST_S(1), XT_OLT_S(e, XT_CONST_S(1)));
    XT_SSI(e, p_fbe, 0);
    PROFILE_STOP(logmelf_computeFbe);
    /* Log-scale filterbank energies. */
    PROFILE_START(logmelf_vec_logXf);
    if (LOGMEL_OPT_EQ(logmel->params.opt, FBELOG, NATURAL)) {
        vec_lognf(logFbe, fbe, logmel->params.mfbBandNum);
    } else {
        vec_log10f(logFbe, fbe, logmel->params.mfbBandNum);
    }
    PROFILE_STOP(logmelf_vec_logXf);
#endif /* !HAVE_VFPU */
} /* logmelf_process() */

/* Returns: size of scratch memory area, in bytes. */
size_t logmelf_getScratchSize( const logmel_params_t * params )
{
    int binLow, binUpp; /* FFT bin indices */
    /* Determine the number of FFT bins in the filterbank frequency range. */
    if (LOGMEL_OPT_EQ(params->opt, MELSCALE, HTK)) {
        logmel_binRange_htk(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    } else {
        logmel_binRange_auditory(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    }
    /* Compute the allocation size. */
    return ALIGN_PAD + (binUpp-binLow)   *sz_f32 + /* magspec             */
           ALIGN_PAD + (binUpp-binLow)   *sz_f32 + /* complex2mag scratch */
           ALIGN_PAD + params->mfbBandNum*sz_f32;  /* fbe                 */
} /* logmelf_getScratchSize() */

void vec_complex2mag_logmel(float32_t * pScr, float32_t * restrict y, const complex_float * restrict x, int N)
{
#if HAVE_VFPU
    const xtfloatx2 * restrict X = (xtfloatx2*)x;
    const xtfloatx2 * restrict Y_rd;
          xtfloatx2 * restrict Y_wr;
    const xtfloatx2 * restrict S_rd;
          xtfloatx2 * restrict S_wr;
    int n;
    xtfloatx2 x0, x1, y0, z0, xre, xim, pinf = plusInff.f;
    xtfloat l;
    ae_int32x2 u0, e0, t0, t1, nsa, nsa0;
    xtbool2 b0;
    if (N <= 0) return;

    NASSERT(pScr);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN8(pScr);
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);

    S_wr = (xtfloatx2*)pScr;
    Y_wr = (xtfloatx2*)y;
    X = (xtfloatx2*)x;
    /* 16 cycles per pipeline stage in steady state with unroll=2 */
    for (n = 0; n<(N>>1); n++) {
        XT_LSX2IP(x0, X, sizeof(complex_float));
        XT_LSX2IP(x1, X, sizeof(complex_float));
        x0 = XT_ABS_SX2(x0);
        x1 = XT_ABS_SX2(x1);

        xre = XT_SEL32_HH_SX2(x0, x1);
        xim = XT_SEL32_LL_SX2(x0, x1);

        t0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(xre);
        t1 = XT_AE_MOVINT32X2_FROMXTFLOATX2(xim);

        nsa = AE_MAX32(t0, t1);
        nsa = AE_SRLI32(nsa, 23);
        nsa = AE_SUB32(nsa, 127);

        nsa = AE_MIN32(nsa, 127);

        e0 = AE_SUB32(127, nsa);
        b0 = AE_EQ32(e0, 0);
        nsa0 = AE_SLLI32(e0, 23);
        AE_MOVT32X2(nsa0, 0x00400000, b0);
        y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0);
        xre = XT_MUL_SX2(xre, y0);
        xim = XT_MUL_SX2(xim, y0);
     
        x0 = XT_MUL_SX2(xre, xre);
        x1 = XT_MUL_SX2(xim, xim);
        x0 = XT_ADD_SX2(x0, x1);
        nsa = AE_ADD32(127, nsa);
        b0 = AE_EQ32(nsa, 0);
        u0 = AE_SLLI32(nsa, 23);
        AE_MOVT32X2(u0, 0x00400000, b0);
        x1 = XT_AE_MOVXTFLOATX2_FROMINT32X2(u0);
        XT_SSX2IP(x0, S_wr, 2 * sz_f32);
        XT_SSX2IP(x1, Y_wr, 2 * sz_f32);
    }
    __Pragma("no_reorder");
    S_rd = (xtfloatx2*)pScr;
    Y_rd = (xtfloatx2*)y;
    Y_wr = (xtfloatx2*)y;
    /* 28 cycles per pipeline stage in steady state with unroll=2 */
    for (n = 0; n<(N >> 1); n++) {
        XT_LSX2IP(x0, S_rd, 2*sz_f32);
        XT_LSX2IP(y0, Y_rd, 2*sz_f32);
        z0 = XT_FSQRT_SX2(x0);
        y0 = XT_MUL_SX2(y0, z0);
        XT_MOVT_SX2(y0, pinf, XT_OEQ_SX2(x0, pinf));
        XT_SSX2IP(y0, Y_wr, 2*sz_f32);
    }

    if (N & 1) {
        XT_LSX2IP(x0, X, sizeof(complex_float));

        x0 = XT_ABS_SX2(x0);
        xre = XT_SEL32_HH_SX2(x0, x0);
        xim = XT_SEL32_LL_SX2(x0, x0);
        t0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(xre);
        t1 = XT_AE_MOVINT32X2_FROMXTFLOATX2(xim);

        nsa = AE_MAX32(t0, t1);
        nsa = AE_SRLI32(nsa, 23);
        nsa = AE_SUB32(nsa, 127);

        nsa = AE_MIN32(nsa, 127);

        e0 = AE_SUB32(127, nsa);
        b0 = AE_EQ32(e0, 0);
        nsa0 = AE_SLLI32(e0, 23);
        AE_MOVT32X2(nsa0, 0x00400000, b0);
        y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0);
        xre = XT_MUL_SX2(xre, y0);
        xim = XT_MUL_SX2(xim, y0);

        x0 = XT_MUL_SX2(xre, xre);
        x1 = XT_MUL_SX2(xim, xim);
        x0 = XT_ADD_SX2(x0, x1);

        e0 = AE_ADD32(127, nsa);
        b0 = AE_EQ32(e0, 0);
        u0 = AE_SLLI32(e0, 23);
        AE_MOVT32X2(u0, 0x00400000, b0);
        y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(u0);

        z0 = XT_FSQRT_SX2(x0);
        y0 = XT_MUL_SX2(y0, z0);
        XT_MOVT_SX2(y0, pinf, XT_OEQ_SX2(x0, pinf));

        l = XT_LOW_S(y0);
        XT_SSI(l, (xtfloat*)Y_wr, 0);
    }
#else /* HAVE_VFPU */
    int n;
    const xtfloat* restrict pX = (const xtfloat*)x;
    const xtfloat* restrict pZrd;
          xtfloat* restrict pZwr;
    const xtfloat* restrict pYrd = (const xtfloat*)y;
          xtfloat* restrict pYwr  =(      xtfloat*)y;
    NASSERT(pScr);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN8(pScr);
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    pZwr = (xtfloat*)pScr;
    pYwr = (xtfloat*)y;
    /* 41 cycles per pipeline stage in steady state with unroll=2 */
    for (n=0; n<N; n++) {
        xtfloat x0, x1, y0;
        xtfloat xre, xim;
        int t0, t1, nsa;
        int e0;
        int nsa0;

        XT_LSIP(xre,pX,1*sizeof(xtfloat));
        XT_LSIP(xim,pX,1*sizeof(xtfloat));
        xre = XT_ABS_S(xre);
        xim = XT_ABS_S(xim);
        t0 = XT_RFR(xre);
        t1 = XT_RFR(xim);
        nsa = XT_MAX(t0, t1);
        nsa = ((uint32_t)nsa)>> 23;
        nsa = (nsa-127);
        nsa = XT_MIN(nsa, 127);
        e0 = (127-nsa);
        nsa0 = (e0<<23);
        XT_MOVEQZ(nsa0,0x00400000,e0);
        y0 = XT_WFR(nsa0);

        xre = XT_MUL_S(xre, y0);
        xim = XT_MUL_S(xim, y0);

        x0 = XT_MUL_S(xre, xre);
        x1 = XT_MUL_S(xim, xim);

        x0 = XT_ADD_S(x0, x1);
        XT_SSIP(x0,pYwr,sizeof(xtfloat));

        e0 = (127+nsa);
        nsa0 = (e0<<23);
        XT_MOVEQZ(nsa0, 0x00400000, e0);
        x0 = XT_WFR(nsa0);
        XT_SSIP(x0,pZwr,sizeof(xtfloat));
    }
    __Pragma("no_reorder")
    pZrd = (xtfloat*)pScr;
    pYrd = (xtfloat*)y;
    pYwr = (xtfloat*)y;
    /* 43 cycles per pipeline stage in steady state with unroll=2 */
    for (n=0; n<N; n++) {
        xtfloat z0,y0,x0;
        XT_LSIP(y0,pYrd,sizeof(xtfloat));
        z0 = XT_FSQRT_S(y0);
        XT_MOVT_S(z0, plusInff.f, XT_OEQ_S(y0, plusInff.f));
        XT_LSIP(x0,pZrd,sizeof(xtfloat));
        z0 = XT_MUL_S(z0, x0);
        XT_SSIP(z0,pYwr,sizeof(xtfloat));
    }
#endif /* !HAVE_VFPU */
} /* vec_complex2mag_logmel() */

#endif /* HAVE_ */
