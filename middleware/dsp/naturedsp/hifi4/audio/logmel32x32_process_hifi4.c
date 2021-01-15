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
    32-bit fixed-point variant
    C code optimized for HiFi4 core
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
/* Log mel filterbank internal definitions. */
#include "logmel_internal.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_audio.h"
#include "NatureDSP_Signal_math.h"
#include "NatureDSP_Signal_vector.h"
/* Approximation table for sqrt */
#include "scl_sqrt_table.h"

#define PROFILE_ENABLE  0 /* If non-zero, measure cycles and print a report to stdout. */
#define ALIGN_SIZE      (XCHAL_DATA_WIDTH)
#define ALIGN_PAD       (ALIGN_SIZE-1)
#define ALIGN_PTR(p)    (void*)(((uintptr_t)(p)+ALIGN_PAD)&~ALIGN_PAD)
#define sz_i16          sizeof(int16_t)
#define sz_i32          sizeof(int32_t)
#define sz_i32c         sizeof(complex_fract32)

#define MAX(a,b)        ((a)>(b) ? (a) : (b))
#define MIN(a,b)        ((a)<(b) ? (a) : (b))

/* Integrated profiler. */
#include "profile.h"

PROFILE_CREATE(logmel32x32_normalizeSpectra);
PROFILE_CREATE(logmel32x32_vec_complex2mag);
PROFILE_CREATE(logmel32x32_computeFbe);
PROFILE_CREATE(logmel32x32_logScaleFbe);

/* For each complex number in the input argument x[N], compute its magnitude and
 * store the result to respective position in the output argument y[N]. */
static void vec_complex2mag( fract32 * restrict y, const complex_fract32 * restrict x, int N );

/* Compute base-2 logarithm of each value from the input argument x[N] and store
 * results to the output argument y[N]. Input values should be either positive normalized
 * Q31 numbers from the range [0.5,1), or zero. Results belong to the range [-1,0), Q31,
 * and the left boundary also corresponds to zero input values. */
static void vec_log2( fract32 * restrict y, const fract32 * restrict x, int N );

/* Log-scale filterbank energies.
 * Logarithm base is selected via the input argument base10:
 *   base10==0: logFbe(n) = log(max(1,fbe(n)*2^-fbeExp(n)));
 *   base10!=0: logFbe(n) = log10(max(1,fbe(n)*2^-fbeExp(n)));
 * Input energy estimates must be either zero, or positive normal numbers
 * (i.e. with no redundant sign bits). */
static void logScaleFbe( fract32 * restrict logFbe, 
                   const fract32 * restrict fbe, 
                   const int16_t * restrict fbeExp, 
                   int bandNum, int base10 );

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
void logmel32x32_process( logmel32x32_handle_t handle, void * restrict pScr, fract32 * restrict logFbe, const complex_fract32 * restrict spectra, int scaleExp )
{
    logmel32x32_t * logmel = (logmel32x32_t*)handle;
    void * p = pScr;
    complex_fract32 *normspec; /* Normalized spectra */
    fract32 *magspec, *fbe; /* Magnitude spectrum, filterbank energies */
    int16_t *fbeExp; /* Filterbank energy exponents */
    const ae_int32x2 * restrict p_s;
    const ae_int32x2 * restrict p_w;
    const int16_t    * restrict p_seg;
          ae_int32   * restrict p_fbe;
    const ae_int32   * restrict p_fbeScl_fract;
    const int16_t    * restrict p_fbeScl_exp;
    static const int32_t one_fr32 = MAX_INT32;
    static const int16_t one_exp = 31;
    ae_valign al_s, al_w;
    ae_int32x2 vwp, vwn, vs; /* Positive-slope weight; negative-slope weight; sample */
    ae_int32x2 vfbe;
    ae_int64 ACCL, ACCR;
    int specExp, normExp=0; 
    int m, n, N, fbNormOpt, fbeSclInc, base10;
    int binNum, bandNum, nsa;
    NASSERT(logmel && logmel->magic==LOGMEL32X32_MAGIC);
    NASSERT_ALIGN(logFbe, XCHAL_DATA_WIDTH);
    NASSERT_ALIGN(spectra, XCHAL_DATA_WIDTH);
    /* Profiler scores should be explicitly reset because this module does not invoke 
     * PROFILE_REPORT macro for them. */
    PROFILE_RESET(logmel32x32_normalizeSpectra);
    PROFILE_RESET(logmel32x32_vec_complex2mag);
    PROFILE_RESET(logmel32x32_computeFbe);
    PROFILE_RESET(logmel32x32_logScaleFbe);
    binNum = logmel->binUpp-logmel->binLow;
    bandNum = logmel->params.mfbBandNum;
    /* Partition the scratch memory area. */
    normspec = (complex_fract32*)ALIGN_PTR(p); p = normspec + binNum;
    magspec  = (fract32        *)ALIGN_PTR(p); p = magspec  + binNum;
    fbe      = (fract32        *)ALIGN_PTR(p); p = fbe      + bandNum;
    fbeExp   = (int16_t        *)ALIGN_PTR(p); p = fbeExp   + bandNum; (void)p;
#ifdef _DEBUG
    /* Check that the scratch size is enough to fit all temporary arrays. 
     * This step is skipped for non-debug builds, because _getScratchSize()
     * function may involve time-consuming computations. */
    NASSERT((uint8_t*)p - (uint8_t*)pScr <= (int)logmel32x32_getScratchSize(&logmel->params));
#endif
    /* Marginal initialization for the memory debugger. */
    AE_S32X2_I(AE_ZERO32(), (ae_int32x2*)magspec, 0);
    /* FBE normalization option. */
    fbNormOpt = (NULL!=logmel->fbeScales_fract);
    p_fbeScl_fract = fbNormOpt ? (const ae_int32 *)logmel->fbeScales_fract : (const ae_int32 *)&one_fr32;
    p_fbeScl_exp   = fbNormOpt ? logmel->fbeScales_exp : &one_exp;
    fbeSclInc      = fbNormOpt ? sz_i16 : 0;
    /* Normally, the input spectrum is either normal or close to normal (block exponent is
     * snall), although frequency bins that belong to the filterbank range should be 
     * re-normalized. */
    PROFILE_START(logmel32x32_normalizeSpectra);
    nsa = vec_bexp32((int32_t*)(spectra+logmel->binLow), 2*binNum);
    vec_shift32x32((int32_t*)normspec, (int32_t*)(spectra+logmel->binLow), nsa, 2*binNum);
    PROFILE_STOP(logmel32x32_normalizeSpectra);
    /* Spectra scale exponent */
    specExp = scaleExp-nsa;
    /* Compute the magnitude spectrum; CQ31 in, Q30 out. */
    PROFILE_START(logmel32x32_vec_complex2mag);
    vec_complex2mag(magspec, normspec, binNum);
    PROFILE_STOP(logmel32x32_vec_complex2mag);
    PROFILE_START(logmel32x32_computeFbe);
    p_seg = logmel->segments;
    p_fbe = (ae_int32 *)fbe;
    /* Positive-slope side of the left-most triangle. */
    p_s = (const ae_int32x2 *)magspec;
    p_w = (const ae_int32x2 *)logmel->weights;
    ACCL = AE_ZERO64();
    N = *p_seg++;
    __Pragma("no_unroll");
    for ( n=0; n<(N>>1); n++ ) {
        AE_L32X2_IP(vwp, p_w, 2*sz_i32);
        AE_L32X2_IP(vs , p_s, 2*sz_i32);
        /* Q46 <- Q31*Q30 - 15 w/ asym. rounding */
        AE_MULAAFD32RA_HH_LL(ACCL, vwp, vs);
    } /* n */
    if (N & 1) {
        AE_L32_IP(vwp, castxcc(ae_int32,p_w), sz_i32);
        AE_L32_IP(vs , castxcc(ae_int32,p_s), sz_i32);
        /* Q46 <- Q31*Q30 - 15 w/ asym. rounding */
        AE_MULAF32RA_HH(ACCL, vwp, vs);
    }
    /* Because of 1/2 bands overlapping, each inner segment simultaneously updates
     * a pair of bands: negative-slope side for the left triangle, positive-slope 
     * side for the right triangle. */
    for ( m=0; m<bandNum-1; m++ ) {
        ACCR = AE_ZERO64();
        al_s = AE_LA64_PP(p_s);
        al_w = AE_LA64_PP(p_w);
        N = *p_seg++;
        __Pragma("no_unroll");
        for ( n=0; n<(N>>1); n++ ) {
            AE_LA32X2_IP(vwp, al_w, p_w);
            AE_LA32X2_IP(vs , al_s, p_s);
            vwn = AE_SUB32(MIN_INT32, vwp);
            /* Q46 <- Q31*Q30 - 15 w/ asym. rounding */
            AE_MULAAFD32RA_HH_LL(ACCL, vwn, vs);
            AE_MULAAFD32RA_HH_LL(ACCR, vwp, vs);
        } /* n */
        if (N & 1) {
            AE_L32_IP(vwp, castxcc(ae_int32,p_w), sz_i32);
            AE_L32_IP(vs , castxcc(ae_int32,p_s), sz_i32);
            vwn = AE_SUB32(MIN_INT32, vwp);
            /* Q46 <- Q31*Q30 - 15 w/ asym. rounding */
            AE_MULAF32RA_HH(ACCL, vwn, vs);
            AE_MULAF32RA_HH(ACCR, vwp, vs);
        }
        /* Optionally normalize the filterbank energy */
        {
            ae_int32x2 vfbeScl;
            ae_int64 ACChi, ACClo;
            ae_ep eplo;
            /* Q(46-(31-scalesExp[m])) <- Q46*Q(scalesExp[m]) - 31 */
            AE_L32_XP(vfbeScl, p_fbeScl_fract, 2*fbeSclInc);
            ACChi = AE_MULF32S_HH(AE_MOVINT32X2_FROMINT64(ACCL), vfbeScl);
            AE_MUL32USEP_LH(eplo, ACClo, AE_MOVINT32X2_FROMINT64(ACCL), vfbeScl);
            ACClo = AE_SRAI72(eplo, ACClo, 31);
            ACCL = AE_ADD64S(ACChi, ACClo);
            normExp = 31-*p_fbeScl_exp;
            p_fbeScl_exp = (const int16_t *)((uintptr_t)p_fbeScl_exp + fbeSclInc);
        }
        nsa = AE_NSA64(ACCL); 
        /* Pack the accumulated energy to a 32-bit value:
         * Q(31+(nsa-normExp-scaleExp-17)) <- Q(46-normExp)*2^specExp + nsa - specExp - 32 */
        ACCL = AE_SLAA64(ACCL, nsa);
        vfbe = AE_ROUND32X2F64SASYM(ACCL, ACCL);
        AE_S32_L_IP(vfbe, p_fbe, sz_i32);
        fbeExp[m] = nsa-normExp-specExp+(46-32)-31;
        ACCL = ACCR;
    } /* m */
    /* Negative-slope side of the right-most triangle. */
    al_s = AE_LA64_PP(p_s);
    al_w = AE_LA64_PP(p_w);
    N = *p_seg++;
    __Pragma("no_unroll");
    for ( n=0; n<(N>>1); n++ ) {
        AE_LA32X2_IP(vwp, al_w, p_w);
        AE_LA32X2_IP(vs , al_s, p_s);
        /* Q46 <- Q31*Q30 - 15 w/ asym. rounding */
        AE_MULAAFD32RA_HH_LL(ACCL, vwp, vs);
    } /* n */
    if (N & 1) {
        AE_L32_IP(vwp, castxcc(ae_int32,p_w), sz_i32);
        AE_L32_IP(vs , castxcc(ae_int32,p_s), sz_i32);
        /* Q46 <- Q31*Q30 - 15 w/ asym. rounding */
        AE_MULAF32RA_HH(ACCL, vwp, vs);
    }
    /* Optionally normalize the filterbank energy */
    {
        ae_int32x2 vfbeScl;
        ae_int64 ACChi, ACClo;
        ae_ep eplo;
        /* Q(46-(31-scalesExp[m])) <- Q46*Q(scalesExp[m]) - 31 */
        AE_L32_XP(vfbeScl, p_fbeScl_fract, 2*fbeSclInc);
        ACChi = AE_MULF32S_HH(AE_MOVINT32X2_FROMINT64(ACCL), vfbeScl);
        AE_MUL32USEP_LH(eplo, ACClo, AE_MOVINT32X2_FROMINT64(ACCL), vfbeScl);
        ACClo = AE_SRAI72(eplo, ACClo, 31);
        ACCL = AE_ADD64S(ACChi, ACClo);
        normExp = 31-*p_fbeScl_exp;
        p_fbeScl_exp = (const int16_t *)((uintptr_t)p_fbeScl_exp + fbeSclInc);
    }
    nsa = AE_NSA64(ACCL); 
    /* Q(31+(nsa-normExp-scaleExp-17)) <- Q(46-normExp)*2^specExp + nsa - specExp - 32 */
    ACCL = AE_SLAA64(ACCL, nsa);
    vfbe = AE_ROUND32X2F64SASYM(ACCL, ACCL);
    AE_S32_L_IP(vfbe, p_fbe, sz_i32);
    fbeExp[m] =  nsa-normExp-specExp+(46-32)-31;
    PROFILE_STOP(logmel32x32_computeFbe);
    /* Base-10 or natural logarithm scale for filterbank energies? */
    base10 = LOGMEL_OPT_EQ(logmel->params.opt, FBELOG, BASE10);
    /* Log-scale filterbank energies. */
    PROFILE_START(logmel32x32_logScaleFbe);
    logScaleFbe(logFbe, fbe, fbeExp, bandNum, base10);
    PROFILE_STOP(logmel32x32_logScaleFbe);
} /* logmel32x32_process() */


/* Returns: size of scratch memory area, in bytes. */
size_t logmel32x32_getScratchSize( const logmel_params_t * params )
{
    int binLow, binUpp;
    NASSERT(NULL!=params);
    /* Determine the number of FFT bins in the filterbank frequency range. */
    if (LOGMEL_OPT_EQ(params->opt, MELSCALE, HTK)) {
        logmel_binRange_htk(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    } else {
        logmel_binRange_auditory(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    }
    /* Compute the allocation size. */
    return ALIGN_PAD + (binUpp-binLow)   *sz_i32c + /* normspec */
           ALIGN_PAD + (binUpp-binLow)   *sz_i32  + /* magspec  */
           ALIGN_PAD + params->mfbBandNum*sz_i32  + /* fbe      */
           ALIGN_PAD + params->mfbBandNum*sz_i16;   /* fbeExp   */
} /* logmel32x32_getScratchSize() */


/*
 * For each complex number in the input argument x[N], compute its magnitude and
 * store the result to respective position in the output argument y[N].
 * Input:
 *   N          Vectors size
 *   x[N]       Input vector, CQ31
 * Output:
 *   y[N]       Output vector, Q30
 * Restrictions:
 *   x[N],y[N]  Must not overlap, and must be 8-bytes aligned
 */

void vec_complex2mag( fract32 * restrict y, const complex_fract32 * restrict x, int N )
{
#if 1
    int n, nsa0, nsa1, shft0, shft1;
    const ae_int32x2 * restrict pX = (const ae_int32x2 *)x;
          ae_int32x2 * restrict pY = (      ae_int32x2 *)y;
    ae_int64    vxw0, vxw1;
    ae_int32x2  vx0, vx1;
    ae_int32x2  vbw, vcw, vdw, vdw0, vdw1, vxx;
    ae_f32x2    vxf, vzf, vcf;

    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT_ALIGN(y, ALIGN_SIZE);
    if (N <= 0) return;

    for (n = 0; n < (N >> 1); n++) 
    {
        AE_L32X2_IP(vx0, pX, sz_i32c);
        AE_L32X2_IP(vx1, pX, sz_i32c);

        vxw0 = AE_MUL32_HH(vx0, vx0);  AE_MULA32_LL(vxw0, vx0, vx0);
        vxw1 = AE_MUL32_HH(vx1, vx1);  AE_MULA32_LL(vxw1, vx1, vx1);
        vxw0 = AE_SRAI64(vxw0, 1);
        vxw1 = AE_SRAI64(vxw1, 1);

        nsa0 = AE_NSA64(vxw0);
        nsa1 = AE_NSA64(vxw1);
        shft0 = (nsa0 & ~1);
        shft1 = (nsa1 & ~1);
        nsa0 = shft0 >> 1;
        nsa1 = shft1 >> 1;
        vxw0 = AE_SLAA64(vxw0, shft0);
        vxw1 = AE_SLAA64(vxw1, shft1);

        vxx = AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(vxw0), AE_MOVINT32X2_FROMINT64(vxw1));

        /* get table indices */
        vdw = AE_SRAI32(vxx, 24);
        vdw = AE_SLAI32(vdw, 3);
        vdw0 = AE_L32X2_X((const ae_int32x2 *)sqrt_table, AE_MOVAD32_L(vdw));
        vdw1 = AE_L32X2_X((const ae_int32x2 *)sqrt_table, AE_MOVAD32_H(vdw));
        vbw = AE_SEL32_LL(vdw1, vdw0); //f'(x0)
        vcw = AE_SEL32_HH(vdw1, vdw0); //f'(x0)

        vxf = (vxx);
        vzf = (vcw);
        AE_MULSFP32X2RAS(vxf, vzf, vzf); //dx
        vcf = (vbw);
        AE_MULAFP32X2RAS(vzf, vcf, vxf); //y 

        /*second iteration*/
        vxf = (vxx);
        AE_MULSFP32X2RAS(vxf, vzf, vzf);
        vcf = (vbw);
        AE_MULAFP32X2RAS(vzf, vcf, vxf); //y

        /*third iteration*/
        vxf = (vxx);
        AE_MULSFP32X2RAS(vxf, vzf, vzf);
        vcf = (vbw);
        AE_MULAFP32X2RAS(vzf, vcf, vxf); //y

        vdw = (vzf);
        vdw0 = AE_SRAA32(vdw, nsa0);
        vdw1 = AE_SRAA32(vdw, nsa1);
        vdw = AE_SEL32_HL(vdw0, vdw1);
        AE_S32X2_IP(vdw, pY, sizeof(*pY));
    }

    if (N & 1)
    {
        AE_L32_IP(vx0, castxcc(ae_int32,pX), sz_i32);
        AE_L32_IP(vx1, castxcc(ae_int32,pX), sz_i32);

        vxw0 = AE_MUL32_HH(vx0, vx0);  AE_MULA32_HH(vxw0, vx1, vx1);
        vxw0 = AE_SRAI64(vxw0, 1);
        nsa0 = AE_NSA64(vxw0);
        shft0 = (nsa0 & ~1);
        nsa0 = nsa0 >> 1;
        vxw0 = AE_SLAA64(vxw0, shft0);

        vxx = AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(vxw0), AE_MOVINT32X2_FROMINT64(vxw0));

        /* get table indices */
        vdw = AE_SRAI32(vxx, 24);
        vdw = AE_SLAI32(vdw, 3);
        vcw = AE_L32X2_X((const ae_int32x2 *)sqrt_table, AE_MOVAD32_H(vdw));
        vbw = AE_SEL32_LL(vcw, vcw); //f'(x0)
        vcw = AE_SEL32_HH(vcw, vcw); //f'(x0)

        vxf = (vxx);
        vzf = (vcw);
        AE_MULSFP32X2RAS(vxf, vzf, vzf); //dx
        vcf = (vbw);
        AE_MULAFP32X2RAS(vzf, vcf, vxf); //y 

        /*second iteration*/
        vxf = (vxx);
        AE_MULSFP32X2RAS(vxf, vzf, vzf);
        vcf = (vbw);
        AE_MULAFP32X2RAS(vzf, vcf, vxf); //y

        /*third iteration*/
        vxf = (vxx);
        AE_MULSFP32X2RAS(vxf, vzf, vzf);
        vcf = (vbw);
        AE_MULAFP32X2RAS(vzf, vcf, vxf); //y

        vdw = (vzf);
        vdw = AE_SRAA32(vdw, nsa0);
        AE_S32_L_I(vdw, (ae_int32 *)pY, 0);
    }
#else
    int n;
    NASSERT(N>0);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT_ALIGN(y, ALIGN_SIZE);
    for ( n=0; n<N; n++ ) {
        float64_t re, im, mag;
        re = ldexp((float64_t)x[n].s.re, -31);
        im = ldexp((float64_t)x[n].s.im, -31);
        mag = sqrt(re*re+im*im);
        y[n] = (fract32)MAX(MIN_INT32, MIN(MAX_INT32, round(ldexp(mag, 30))));
    } /* n */
#endif
} /* vec_complex2mag() */ 

/*
 * Compute base-2 logarithm of each value from the input argument x[N] and store
 * results to the output argument y[N]. Input values should be either positive normalized
 * Q31 numbers from the range [0.5,1), or zero. Results belong to the range [-1,0), Q31,
 * and the left boundary also corresponds to zero input values.
 * 
 * Input:
 *   N          Vectors size
 *   x[N]       Input vector, Q31
 * Output:
 *   y[N]       Output vector, Q31
 * Restrictions:
 *   N          Must be positive
 *   x[N],y[N]  Must be 8-bytes aligned
 */

void vec_log2( fract32 * restrict y, const fract32 * restrict x, int N )
{
#if 1
    /*
     Matlab code for computing the polynomial:
     x=(sqrt(0.5):pow2(1,-16):sqrt(2));
     z=1-x;
     y=log(x)./z;
     p=polyfit(z,y,8);
    */
#if 1
    #define POLYLOG_ORD   9
    static const int32_t polylog_tbl[POLYLOG_ORD] = {
        -161926367,-273781379,-283444439,-304997874,-356837178,-429521585,-536898174,-715827933,-1073741641
    };
#else
    #define POLYLOG_ORD  10
    static const int32_t polylog_tbl[POLYLOG_ORD] = {
        -142862210,-248576459,-255136671,-266747201,-305401168,-357922703,-429541854,-536871698,-715827371,-1073741817
    };
#endif
    const ae_int32x2 * restrict px0;
    const ae_int32x2 * restrict px1;
          ae_int32x2 * restrict py;
    const ae_int32   * restrict ptbl;
    ae_int32x2 vx, vy, vz, vz2, vg;
    ae_int32x2 p0, p1, p2, p3, p4, p5, p6, p7, p8;
    xtbool2 bsmall;
    int n;

    NASSERT(N>0);
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);

    px0 = (const ae_int32x2 *)x;
    px1 = (const ae_int32x2 *)x;
    py  = (      ae_int32x2 *)y;
    ptbl = (const ae_int32 *)polylog_tbl;

    for ( n=0; n<(N>>1); n++ )
    {
        AE_L32X2_IP(vx, px0, 2*sz_i32);

        bsmall = AE_LT32(vx, 1518500250); /* to fit 0.707...1.414 */
        vz = vx;//vz = AE_OR32(vx, 1L<<30); /* to handle zero input */
        AE_MOVT32X2(vz, AE_SLAI32(vz, 1), bsmall);
        vz = AE_SUB32(MIN_INT32, vz);

        p0 = AE_L32_I(ptbl, 0*sz_i32);
        p1 = AE_L32_I(ptbl, 1*sz_i32);
        p2 = AE_L32_I(ptbl, 2*sz_i32);
        p3 = AE_L32_I(ptbl, 3*sz_i32);
        p4 = AE_L32_I(ptbl, 4*sz_i32);
        p5 = AE_L32_I(ptbl, 5*sz_i32);
        p6 = AE_L32_I(ptbl, 6*sz_i32);
        p7 = AE_L32_I(ptbl, 7*sz_i32);
        p8 = AE_L32_X(ptbl, 8*sz_i32);

        AE_MULAFP32X2RAS(p1, p0, vz);
        AE_MULAFP32X2RAS(p2, p1, vz);
        AE_MULAFP32X2RAS(p3, p2, vz);
        AE_MULAFP32X2RAS(p4, p3, vz);
        AE_MULAFP32X2RAS(p5, p4, vz);
        vz = AE_OR32(vz, vz);
        AE_MULAFP32X2RAS(p6, p5, vz);
        AE_MULAFP32X2RAS(p7, p6, vz);
        AE_MULAFP32X2RAS(p8, p7, vz);
        vz2 = AE_MULFP32X2RAS(vz, vz);
        vg = AE_MULFP32X2RAS(p8, vz2);

        vg = AE_SUB32(vg, vz);
        vg = AE_SLAI32S(vg, 1);
        vg = AE_MULFP32X2RAS(vg, 1549082005);

        AE_L32X2_IP(vx, px1, 2*sz_i32);
        bsmall = AE_LT32(vx, 1518500250); /* to fit 0.707...1.414 */
        vy = vg;
        AE_MOVT32X2(vy, AE_XOR32(vg, MIN_INT32), bsmall);

        AE_S32X2_IP(vy, py, 2*sz_i32);
    }
    if (N&1)
    {
        vx = AE_L32_I((ae_int32 *)px0, 0);

        bsmall = AE_LT32(vx, 1518500250); /* to fit 0.707...1.414 */
        vz = vx;// AE_OR32(vx, 1L << 30); /* to handle zero input */
        AE_MOVT32X2(vz, AE_SLAI32(vz, 1), bsmall);
        vz = AE_SUB32(MIN_INT32, vz);

        p0 = AE_L32_I(ptbl, 0*sz_i32);
        p1 = AE_L32_I(ptbl, 1*sz_i32);
        p2 = AE_L32_I(ptbl, 2*sz_i32);
        p3 = AE_L32_I(ptbl, 3*sz_i32);
        p4 = AE_L32_I(ptbl, 4*sz_i32);
        p5 = AE_L32_I(ptbl, 5*sz_i32);
        p6 = AE_L32_I(ptbl, 6*sz_i32);
        p7 = AE_L32_I(ptbl, 7*sz_i32);
        p8 = AE_L32_X(ptbl, 8*sz_i32);

        AE_MULAFP32X2RAS(p1, p0, vz);
        AE_MULAFP32X2RAS(p2, p1, vz);
        AE_MULAFP32X2RAS(p3, p2, vz);
        AE_MULAFP32X2RAS(p4, p3, vz);
        AE_MULAFP32X2RAS(p5, p4, vz);
        AE_MULAFP32X2RAS(p6, p5, vz);
        AE_MULAFP32X2RAS(p7, p6, vz);
        AE_MULAFP32X2RAS(p8, p7, vz);
        vz2 = AE_MULFP32X2RAS(vz, vz);
        vg = AE_MULFP32X2RAS(p8, vz2);

        vg = AE_SUB32(vg, vz);
        vg = AE_SLAI32S(vg, 1);
        vg = AE_MULFP32X2RAS(vg, 1549082005);
        vy = 0;
        AE_MOVT32X2(vy, MIN_INT32, bsmall);
        vy = AE_ADD32(vy, vg);

        AE_S32_L_I(vy, (ae_int32 *)py, 0);
    }
#else
    int n;
    NASSERT(N>0);
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    for ( n=0; n<N; n++ ) {
        if (x[n]>0) {
            float64_t f = ldexp((float64_t)x[n], -31);
            NASSERT(S_exp0_l(x[n])==0);
            y[n] = (fract32)MAX(MIN_INT32, round(ldexp(log2(f), 31)));
        } else {
            y[n] = MIN_INT32;
        }
    } /* n */
#endif
} /* vec_log2() */

/*
 * Log-scale filterbank energies.
 * Logarithm base is selected via the input argument base10:
 *   base10==0: logFbe(n) = log(max(1,fbe(n)*2^-fbeExp(n)));
 *   base10!=0: logFbe(n) = log10(max(1,fbe(n)*2^-fbeExp(n)));
 * Input energy estimates must be either zero, or positive normal numbers
 * (i.e. with no redundant sign bits). 
 * Input:
 *   bandNum          Number of filterbank bands
 *   base10           If non-zero, use base-10 logarithm, otherwise use the natural logarithm
 *   fbe[bandNum]     Filterbank energies, Q(31+fbeExp[bandNum])
 *   fbeExp[bandNum]  Enwergy exponents 
 * Output:
 *   logFbe[bandNum]  Log-scaled filterbank energies, Q6.25
 * Restrictions:
 *   bandNum                  Must be positive
 *   logFbe[],fbe[],fbeExp[]  Must be 8-bytes aligned, must not overlap
 *   fbe[]                    Either positive normalized, or zero
 */
void logScaleFbe( fract32 * restrict logFbe, 
            const fract32 * restrict fbe, 
            const int16_t * restrict fbeExp, 
            int bandNum, int base10 )
{
    const ae_int32x2 * restrict p_logFbe_rd;
          ae_int32x2 * restrict p_logFbe_wr;
    const ae_int16x4 * restrict p_fbeExp;
    ae_int32x2 vlogFbe0, vlogFbe1, vfbeExp0, vfbeExp1, vg0, vg1, vlog;
    ae_int16x4 vfbeExp_fr16;
    fract32 f;
    int n;

    NASSERT(bandNum>0);
    NASSERT_ALIGN(logFbe, ALIGN_SIZE);
    NASSERT_ALIGN(fbe, ALIGN_SIZE);
    NASSERT_ALIGN(fbeExp, ALIGN_SIZE);

    /* logFbe[n] <- log2(fbe[n]); Q31 in, Q31 out. */
    vec_log2(logFbe, fbe, bandNum);

    f = 646456993; /* log10(2), Q31 */
    XT_MOVEQZ(f, 1488522236, base10); /* log(2), Q31 */
    vlog = f;

    p_logFbe_rd = (const ae_int32x2 *)logFbe;
    p_logFbe_wr = (      ae_int32x2 *)logFbe;
    p_fbeExp    = (const ae_int16x4 *)fbeExp;

    for ( n=0; n<(bandNum>>2); n++ )
    {
        /* g <- log2(fbe(n)) - fbeExp; Q6.25 <- Q31 - 6 */
        AE_L32X2_IP(vlogFbe0, p_logFbe_rd, 2*sz_i32);
        AE_L32X2_IP(vlogFbe1, p_logFbe_rd, 2*sz_i32);
        AE_L16X4_IP(vfbeExp_fr16, p_fbeExp, 4*sz_i16);

        AE_MUL16X4(vfbeExp0, vfbeExp1, vfbeExp_fr16, 1);
        vfbeExp0 = AE_SLAI32S(vfbeExp0, 25);
        vfbeExp1 = AE_SLAI32S(vfbeExp1, 25);
        vlogFbe0 = AE_SRAI32R(vlogFbe0, 6);
        vlogFbe1 = AE_SRAI32R(vlogFbe1, 6);

        vg0 = AE_SUB32S(vlogFbe0, vfbeExp0);
        vg1 = AE_SUB32S(vlogFbe1, vfbeExp1);

        /* Q25 <- Q31*Q25 - 31 w/ rounding */
        vg0 = AE_MAX32(vg0, 0);
        vg1 = AE_MAX32(vg1, 0);
        vg0 = AE_MULFP32X2RAS(vlog, vg0);
        vg1 = AE_MULFP32X2RAS(vlog, vg1);
        AE_S32X2_IP(vg0, p_logFbe_wr, 2*sz_i32);
        AE_S32X2_IP(vg1, p_logFbe_wr, 2*sz_i32);
    }
    if (bandNum & 2)
    {
        ae_int32x2 vt;
        /* g <- log2(fbe(n)) - fbeExp; Q6.25 <- Q31 - 6 */
        AE_L32X2_IP(vlogFbe0, p_logFbe_rd, 2*sz_i32);
        AE_L32_IP(vt, castxcc(ae_int32,p_fbeExp), 2*sz_i16);
        vfbeExp_fr16 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(vt));

        vfbeExp0 = AE_CVT32X2F16_32(vfbeExp_fr16);
        vfbeExp0 = AE_SLAI32S(vfbeExp0, 9);
        vlogFbe0 = AE_SRAI32R(vlogFbe0, 6);

        vg0 = AE_SUB32S(vlogFbe0, vfbeExp0);

        /* Q25 <- Q31*Q25 - 31 w/ rounding */
        vg0 = AE_MAX32(vg0, 0);
        vg0 = AE_MULFP32X2RAS(vlog, vg0);
        AE_S32X2_IP(vg0, p_logFbe_wr, 2*sz_i32);
    }
    if (bandNum & 1)
    {
        /* g <- log2(fbe(n)) - fbeExp; Q6.25 <- Q31 - 6 */
        AE_L32_IP(vlogFbe0, castxcc(ae_int32,p_logFbe_rd), sz_i32);
        AE_L16_IP(vfbeExp_fr16, castxcc(ae_int16,p_fbeExp), sz_i16);

        vfbeExp0 = AE_CVT32X2F16_32(vfbeExp_fr16);
        vfbeExp0 = AE_SLAI32S(vfbeExp0, 9);
        vlogFbe0 = AE_SRAI32R(vlogFbe0, 6);

        vg0 = AE_SUB32S(vlogFbe0, vfbeExp0);

        /* Q25 <- Q31*Q25 - 31 w/ rounding */
        vg0 = AE_MAX32(vg0, 0);
        vg0 = AE_MULFP32X2RAS(vlog, vg0);
        AE_S32_L_IP(vg0, castxcc(ae_int32,p_logFbe_wr), sz_i32);
    }
} /* logScaleFbe() */
