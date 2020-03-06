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

#include <string.h>

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
/*  Log mel filterbank internal definitions. */
#include "logmel_internal.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_audio.h"
#include "NatureDSP_Signal_math.h"

#define ALIGN_SIZE       (XCHAL_DATA_WIDTH)
#define ALIGN_PAD        (ALIGN_SIZE-1)
#define ALIGN_PTR(p)     (void*)(((uintptr_t)(p)+ALIGN_PAD)&~ALIGN_PAD)
#define sz_i16           sizeof(int16_t)
#define sz_i32           sizeof(int32_t)
#define sz_i32c          sizeof(complex_fract32)

/* Compute the 32-bit fixed-point reciprocal for input arguments s_fract, s_exp: 
 * r_fract*2^-r_exp ~= 1/(s_fract*2^-s_exp), where r_fract is returned value and
 * r_exp is the output argument. */
static fract32 recip32x32( int16_t * r_exp, int32_t s_fract, int16_t s_exp );

/* Raise 2.0 to the power of the input argument x, -1..0 (Q31) and return the result (Q31). */
static int32_t antilog2_32x32( int32_t x );

/* Compute base-2 logarithm of each value from the input argument x and store
 * results to the output argument y. Input values should be either positive normalized
 * Q31 numbers from the range [0.5,1), or zero. Results belong to the range [-1,0), Q31,
 * and the left boundary also corresponds to zero input values. */
static fract32 scl_log2( fract32 x );

/* Linear to mel-scale frequency conversion, HTK flavour, Q16.15 format for input and output data. */
static fract32 hz2mel_htk( fract32 hz );
/* Mel-scale to linear frequency conversion, HTK flavor, Q16.15 format for input and output data. */
static fract32 mel2hz_htk( fract32 mel );
/* Linear to mel-scale frequency conversion, Auditory flavour, Q16.15 format for input and output data. */
static fract32 hz2mel_slaney( fract32 hz );
/* Mel-scale to linear frequency conversion, Auditory flavor, Q16.15 format for input and output data. */
static fract32 mel2hz_slaney( fract32 mel );

/* Compute weighting functions and (optionally) normalization scale factors. Both the HTK
 * and Auditory flavours are supported, as specified by the input argument isHtk. */
static void buildFilterbank( fract32  * restrict weights,         /* [binUpp-binLow], Out */
                             int16_t  * restrict segments,        /* [bandNum+1    ], Out */
                             fract32  * restrict fbeScales_fract, /* [bandNum      ], Out, optional */
                             int16_t  * restrict fbeScales_exp,   /* [bandNum      ], Out, optional */
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
size_t logmel32x32_alloc( const logmel_params_t * params )
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
        NASSERT(!"logmel32x32_alloc(): bad params"); return (0);
    }
    /* Determine the number of FFT bins in the filterbank frequency range. */
    if (LOGMEL_OPT_EQ(params->opt, MELSCALE, HTK)) {
        logmel_binRange_htk(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    } else {
        logmel_binRange_auditory(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    }
    /* Compute the allocation size. */
    allocSize = ALIGN_PAD + sizeof(logmel32x32_t)  +         /* Instance structure */
                ALIGN_PAD + (binUpp-binLow)*sz_i32 +         /* weights            */
                ALIGN_PAD + (params->mfbBandNum+1)*sz_i16;   /* segments           */
    if (LOGMEL_OPT_EQ(params->opt, FBNORM, AREA)) {
        allocSize += ALIGN_PAD + params->mfbBandNum*sz_i32 + /* fbeScales_fract    */
                     ALIGN_PAD + params->mfbBandNum*sz_i16;  /* fbeScales_exp      */
    }
    return allocSize;
} /* logmel32x32_alloc() */

/* Returns: handle to the object, or NULL if initialization failed. */
logmel32x32_handle_t logmel32x32_init( void * objmem, const logmel_params_t * params )
{
    void *p = objmem;
    logmel32x32_t *logmel;
    fract32 *weights, *fbeScales_fract = NULL;
    int16_t *segments, *fbeScales_exp = NULL;
    size_t allocSize;
    int binLow, binUpp;
    int isHtk;
    /* Validate logmel parameters and recalculate the instance object size. */
    if (0==(allocSize = logmel32x32_alloc(params))) return NULL;
    /* Select either the HTK or Auditory filterbank flavor. */
    isHtk = LOGMEL_OPT_EQ(params->opt, MELSCALE, HTK);
    /* Determine the number of FFT bins in the filterbank frequency range. */
    if (isHtk) {
        logmel_binRange_htk(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    } else {
        logmel_binRange_auditory(&binLow, &binUpp, params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->Fs, params->fftSize);
    }
    /* Partition the object memory space. */
    logmel   = (logmel32x32_t*)ALIGN_PTR(p); p = logmel+1;
    weights  = (fract32      *)ALIGN_PTR(p); p = weights + (binUpp-binLow);
    segments = (int16_t      *)ALIGN_PTR(p); p = segments + (params->mfbBandNum+1);
    if (LOGMEL_OPT_EQ(params->opt, FBNORM, AREA)) {
        fbeScales_fract = (fract32*)ALIGN_PTR(p); p = fbeScales_fract + params->mfbBandNum;
        fbeScales_exp   = (int16_t*)ALIGN_PTR(p); p = fbeScales_exp + params->mfbBandNum;
    }
    NASSERT((uint8_t*)p - (uint8_t*)objmem <= (int)allocSize);
    /* Build the filterbank. */
    buildFilterbank(weights, segments, fbeScales_fract, fbeScales_exp, params->Fs, params->fftSize, 
                    params->mfbLowFreqQ8, params->mfbUppFreqQ8, params->mfbBandNum, binLow, binUpp, isHtk);
    /* Initialize the instance object. */
    memset(logmel, 0, sizeof(*logmel));
    logmel->magic           = LOGMEL32X32_MAGIC;
    logmel->params          = *params;
    logmel->binLow          = binLow;
    logmel->binUpp          = binUpp;
    logmel->weights         = weights;
    logmel->segments        = segments;
    logmel->fbeScales_fract = fbeScales_fract;
    logmel->fbeScales_exp   = fbeScales_exp;
    return (logmel32x32_handle_t)logmel;
} /* logmel32x32_init() */

/* Compute the 32-bit fixed-point reciprocal for input arguments s_fract, s_exp: 
 * r_fract*2^-r_exp ~= 1/(s_fract*2^-s_exp), where r_fract is returned value and
 * r_exp is the output argument. */
fract32 recip32x32( int16_t * r_exp, int32_t s_fract, int16_t s_exp )
{
#if 1
    fract32 r_fract;
    vec_recip32x32(&r_fract, r_exp, &s_fract, 1);
    *r_exp = 62-s_exp-*r_exp;
    return r_fract;
#else
    uint32_t r;
    fract16 e;
    fract32 r_fract;
    int s_nsa;
    NASSERT(0!=s_fract);
    s_nsa = NSA(s_fract); 
    /* Q(53-s_nsa) <- Q61/Q(s_nsa) - 8 */
    r = scl_recip32x32(s_fract<<s_nsa);
    /* Q(61-s_nsa) <- Q(53-s_nsa) + 8 */
    r_fract = (fract32)(r<<8); NASSERT(1==((int32_t)r>>24));
    /* Perform a Newton-Raphson iteration to refine 8 LSBs. */
    /* Q31 <- 2*[Q(61-s_nsa)*Q(s_nsa) - 31] + 1 */
    e = S_sature_l((1L<<31) - 2*L_mpy_ll(r_fract, s_fract<<s_nsa));
    /* Q(61-s_nsa) <- [Q(61-s_nsa)*Q31 - 15] - 16 */
    r_fract = L_add_ll(r_fract, S_round_l(L_mpy_ls(r_fract, e)));
    *r_exp = 61-s_nsa-s_exp; 
    return r_fract;
#endif
} /* recip32x32() */

/* Raise 2.0 to the power of the input argument x, -1..0 (Q31) and return the result (Q31). */
int32_t antilog2_32x32( int32_t x )
{
    /*
        polynomial coefficients for 2.^x (x in range -1...0) in Q31 
        x=(-1:pow2(1,-16):0); y=2.^x; p=polyfit(x,y,8);
        first 7 coefficients are in Q33, the last is in Q31
    */
    static const int32_t pow2poly[] = {
          8371, 126415, 1319316, 11451625, 82618622, 476776598, 2063529978,/* Q33 */
          1488522236 /* Q31 */
    };
    const ae_int32 * ppoly = (const ae_int32 *)&pow2poly;
    ae_int32x2 y, z, x_;
    ae_int64 W;
    NASSERT(x<=0);

    x_ = AE_MOVDA32(x);
    AE_L32_IP(y, ppoly, sz_i32);                             z = y;
    AE_L32_IP(y, ppoly, sz_i32); AE_MULAFP32X2RAS(y, x_, z); z = y;
    AE_L32_IP(y, ppoly, sz_i32); AE_MULAFP32X2RAS(y, x_, z); z = y;
    AE_L32_IP(y, ppoly, sz_i32); AE_MULAFP32X2RAS(y, x_, z); z = y;
    AE_L32_IP(y, ppoly, sz_i32); AE_MULAFP32X2RAS(y, x_, z); z = y;
    AE_L32_IP(y, ppoly, sz_i32); AE_MULAFP32X2RAS(y, x_, z); z = y;
    AE_L32_IP(y, ppoly, sz_i32); AE_MULAFP32X2RAS(y, x_, z); z = y;
    z = AE_SRAI32R(z, 2);
    AE_L32_IP(y, ppoly, sz_i32); AE_MULAFP32X2RAS(y, x_, z); z = y;
    W = AE_ADD64(AE_MUL32_LL(x_, z), (1LL<<62));
    AE_PKSRF32(z, W, 1);
    return AE_MOVAD32_L(z);
} /* antilog2_32x32() */

/*
 * Compute base-2 logarithm of each value from the input argument x and store
 * results to the output argument y. Input values should be either positive normalized
 * Q31 numbers from the range [0.5,1), or zero. Results belong to the range [-1,0), Q31,
 * and the left boundary also corresponds to zero input values.
 * 
 * Input:
 *   x       Input value, Q31
 * Return:
 *   y       Output value, Q31
 */

fract32 scl_log2( fract32 x )
{
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
    const ae_int32   * restrict ptbl;
    ae_int32x2 vx, vy, vz, vz2, vg;
    ae_int32x2 p0, p1, p2, p3, p4, p5, p6, p7, p8;
    xtbool2 bsmall;

    ptbl = (const ae_int32 *)polylog_tbl;

    vx = AE_MOVDA32(x);
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

    vy = vg;
    AE_MOVT32X2(vy, AE_XOR32(vg, MIN_INT32), bsmall);

    return AE_MOVAD32_L(vy);
} /* scl_log2() */

/* Linear to mel-scale frequency conversion, HTK flavour, Q14.17 format for input and output data. */
fract32 hz2mel_htk( fract32 hz )
{
    /* We use HTK's mapping from linear to mel-scale frequency, as defined 
     * in [1] p.76 Eq.(5.13): mel = 2595*log10(1+hz/700) */
    fract32 g,h;
    ae_int32x2 F, G, S;
    int nsa;
    NASSERT(hz>=0);
    /* f <- hz/700; Q26 <- Q17*Q40 - 31 */
    /* g <- 1+f; Q26 */
    F = AE_MOVDA32(hz);
    G = AE_MOVDA32(1L<<26);
    AE_MULAFP32X2RAS(G, F, AE_MOVDA32(1570730897L));
    /* h <- log2(g/2^(31-26)+nsa); Q31 */
    nsa = AE_NSAZ32_L(G); G = AE_SLAA32(G, nsa);
    g = AE_MOVAD32_L(G);
    h = scl_log2(g);
    /* s <- h+(31-26-nsa); Q27 <- Q31 - 4 w/ rounding */
    S = AE_ADD32S(AE_SRAI32R(h, 4), AE_MOVDA32((fract32)(31-26-nsa)<<27));
    /* s*(2595/log2(10)); Q16.17 <- Q21*Q27 - 31 w/ rounding */
    return AE_MOVAD32_L(AE_MULFP32X2RAS(AE_MOVDA32(1638238181L), S));
} /* hz2mel_htk() */ 

/* Mel-scale to linear frequency conversion, HTK flavor, Q14.17 format for input and output data. */
fract32 mel2hz_htk( fract32 mel )
{
    /* Inverse of HTK's formula, see [1] p.76 Eq.(5.13): hz = 700*(10^(mel/2595)-1) */
    fract32 e, f, g, h;
    ae_int32x2 F, H;
    ae_int64 hz;
    /* f <- mel*(log2(10)/2595); Q4.27 <- (Q17+1)*Q40 - 31 */
    F = AE_MOVDA32(mel);
    F = AE_MULFP32X2RAS(AE_SLAI32S(F, 1), AE_MOVDA32(1407513899L));
    /* Decompose f into the sum e+g, where e is integer (Q0), and g is fractional, -1<=g<0 (Q27) */
    f = AE_MOVAD32_L(F);
    e = (f>>27)+1;
    g = (f&((1L<<27)-1))-(1L<<27);
    /* h <- 2^g; Q31 */
    h = antilog2_32x32(g<<4);
    /* hz <- 700*h*2^e-700; Q(31-e) <- Q0*Q31*2^e - e */
    hz = AE_MUL32_LL(700, h);
    hz = AE_SUB64(hz, 700LL<<(31-e));
    /* Q14.17 <- Q(31-e) - (14-e) w/ rounding */
    hz = AE_ADD64S(hz, (1LL<<(13-e)) );
    H = AE_TRUNCA32X2F64S(hz, hz, e-14+32);
    return AE_MOVAD32_L(H);
} /* mel2hz_htk() */

/* Linear to mel-scale frequency conversion, Auditory flavour, Q14.17 format for input and output data. */
fract32 hz2mel_slaney( fract32 hz )
{
    fract32 f, g, h;
    int nsa;
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
    if (hz>=(1000L<<17)) {
        /* g <- log2(hz/2^(31-17)+nsa); Q31 */
        nsa = NSA(hz); f = hz<<nsa;
        g = scl_log2(f);
        /* h <- g+(31-17-nsa); Q27 <- Q31 - 4 w/ rounding */
        h = ((g+(1<<3))>>4) + ((fract32)(31-17-nsa)<<27);
        /* (1800/log2(6.4))*g + (1000-5400/log10(6.4)); Q17 <- Q21*Q27 - 31 */
        return (int32_t)AE_F32_MULF_F32(AE_MOVDA32(1409549009L), h)-746881835L;
    } else {
        return hz;
    }
} /* hz2mel_slaney() */

/* Mel-scale to linear frequency conversion, Auditory flavor, Q14.17 format for input and output data. */
fract32 mel2hz_slaney( fract32 mel )
{
    fract32 e, f, g, h;
    ae_int32x2 F, hz;
    ae_int64 H;
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
    if (mel>=1000L<<17) {
        /* f <- mel*(log2(6.4)/1800); Q4.27 <- (Q17+1)*Q40 - 31 */
        F = AE_MOVDA32(mel);
        F = AE_MULFP32X2RAS(AE_SLAI32S(F, 1), AE_MOVDA32(1635872889L));
        /* Decompose f into the sum e+g, where e is integer (Q0), and g is fractional, -1<=g<0 (Q27) */
        f = AE_MOVAD32_L(F);
        e = (f>>27)+1;
        g = (f&((1L<<27)-1))-(1L<<27);
        /* h <- 2^g; Q31 */
        h = antilog2_32x32(g<<4);
        /* hz <- (1000/6.4^(5/9))*h*2^e; Q14.17 <- Q22*Q31*2^e - 36 w/ rounding */
        H = 1LL<<(35-e);
        AE_MULA32_LL(H, 1495485275, h);
        hz = AE_TRUNCA32X2F64S(H, H, e-36+32);
        return AE_MOVAD32_L(hz);
    } else {
        return mel;
    }
} /* mel2hz_slaney() */

/* Compute weighting functions and (optionally) normalization scale factors. Both the HTK
 * and Auditory flavours are supported, as specified by the input argument isHtk. */
void buildFilterbank( fract32  * restrict weights,         /* [binUpp-binLow], Out */
                      int16_t  * restrict segments,        /* [bandNum+1    ], Out */
                      fract32  * restrict fbeScales_fract, /* [bandNum      ], Out, optional */
                      int16_t  * restrict fbeScales_exp,   /* [bandNum      ], Out, optional */
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
    fract32 linLow, linUpp, linL, linR; /* Q14.17; "lin" refers to linear frequency, "L" and "R" denote left and right edges */
    fract32 melLow, melUpp, melL, melR; /* Q14.17; mel-scale frequencies */
    int     binL, binR; /* FFT bin indices */
    fract32 bandL=0; /* Left edge of the current band */
    fract32 invFs_fract, melStp_fract, invStp_fract=0; /* 1/Fs ~= invFs_fract*2^-invFs_exp */
    int16_t invFs_exp, melStp_exp, invStp_exp=0;
    int16_t fftLog;
    fract32 f, r, t, rnd;
    int16_t s;
    ae_int64 R;
    int m, n, nsa, sft;
    /* Select the mel<->linear-scale mapping functions */
    fract32 (*hz2mel)(fract32) = isHtk ? hz2mel_htk : hz2mel_slaney;
    fract32 (*mel2hz)(fract32) = isHtk ? mel2hz_htk : mel2hz_slaney;
    /* Convert frequency boundaries to a fixed-point format that better suits precise calculations. */
    NASSERT(lowFreq_q8<=16000*256 && uppFreq_q8<=16000*256);
    linLow = AE_F32_SLAIS(lowFreq_q8, 17-8);
    linUpp = AE_F32_SLAIS(uppFreq_q8, 17-8);
    /* Linear and mel-frequencies range */
    melLow = hz2mel(linLow);
    melUpp = hz2mel(linUpp);
    /* Compute the mel-frequency step between band edges:
     * melStp <- (melUpp-melLow)/(bandNum+1) */
    r = recip32x32(&s, bandNum+1, 0);
    t = melUpp-melLow; nsa = NSA(t);
    /* Q(17+nsa+s-31) <- Q(s)(Q17+nsa) - 31 */
    melStp_fract = AE_F32_MULF_F32(r, t<<nsa);
    melStp_exp = (int16_t)(nsa+s-31);
    if (isHtk) {
        /* Compute the reciprocal mel-frequency step:
         * invStp <- (bandNum+1)/(melUpp-melLow) */
        r = recip32x32(&s, melUpp-melLow, 17);
        nsa = NSA(bandNum+1)-16;
        /* Q(s+nsa-15) <- Q(s)*Q(nsa) - 15 */
        invStp_fract = AE_F32_MUL_F16(r, (bandNum+1)<<nsa);
        invStp_exp = (int16_t)(s+nsa-15);
    }
    invFs_fract = recip32x32(&invFs_exp, Fs, 0);
    fftLog = 30-NSA(fftSize);
    /* Compute positive-slope sides for bandNum triangles, 1/2 overlapping. */
    melL = melLow; linL = linLow; binL = binLow;
    for ( m=0; m<bandNum; m++ ) {
        /* Right-hand edge, i.e. the central frequency of m-th band:
         * melR <- melLow+m*melStp; Q17 <- [Q0 + 31-melStp_exp]*Q(17+melStp_exp) - 31 */
        melR = melLow + (fract32)AE_F32_MULF_F32((m+1)<<(31-melStp_exp), melStp_fract);
        linR = mel2hz(melR);
        /* binR = max(binLow,min(binUpp,floor(linR/Fs*fftSize)+1));
         * Two-side saturation is essentially required because of rounding used by HTK
         * when converting the frequency range to binLow, binUpp. */
        binR = logmel_hz2bin(linR, invFs_fract, invFs_exp, fftSize, LOGMEL_HZ2BIN_ROUNDOPT_DOWN) + 1;
        binR = XT_MAX(binLow, XT_MIN(binUpp, binR));
        /* Note that HTK computes weights from mel-scale ratios, while Auditory
         * computes ratios in linear domain. */
        if (!isHtk) invStp_fract = recip32x32(&invStp_exp, linR-linL, 17);
        /* Compute weights for FFT bins that belong to positive-slope side of
         * m-th triangle. Note that binR is NOT included, actually it belongs to
         * the positive-slope side of the next triangle. */
        for ( n=binL; n<binR; n++ ) {
            /* Note that HTK computes weights from mel-scale ratios, while Auditory
             * computes ratios in linear domain:
             * HTK:
             *   weights(n-binLow+1) = (hz2mel(n/fftSize*Fs)-melL)/melStp;
             * Auditory:
             *   weights(n-binLow+1) = (n/fftSize*Fs-linL)/(linR-linL); */
            /* Q17 <- Q0*Q0/fftSize + 17 */
            f = (Fs*n)<<(17-fftLog);
            if (isHtk) {
                /* weights(n-binLow+1) = (hz2mel(n/fftSize*Fs)-melL)/melStp; */
                r = hz2mel(f)-melL;
            } else {
                /* weights(n-binLow+1) = (n/fftSize*Fs-linL)/(linR-linL); */
                r = f-linL;
            }
            /* Q31 <- Q17*Q(invStp_exp) + 14-invStp_exp */
            sft = -(14-invStp_exp); rnd = 1L<<(sft-1);
            R = (ae_int64)rnd;
            AE_MULA32_LL(R, r, invStp_fract);
            weights[n-binLow] = AE_MOVAD32_L(AE_TRUNCA32F64S(R, 32-sft));
        } /* n */
        segments[m] = (int16_t)(binR-binL);
        if (NULL!=fbeScales_fract) {
            if (m>0) {
                /* Compute reciprocal bandwidth of m-1st band: 2/(linR-bandL) */
                fbeScales_fract[m-1] = recip32x32(&fbeScales_exp[m-1], linR-bandL, 17+1);
            }
            /* Remeber the left (lower) frequency of m-th band. */
            bandL = linL;
        }
        melL = melR; linL = linR; binL = binR;
    } /* n */
    melR = melUpp; linR = linUpp; binR = binUpp;
    if (!isHtk) invStp_fract = recip32x32(&invStp_exp, linR-linL, 17);
    /* Compute the negative-slope side of the last triangle. */
    for ( n=binL; n<binR; n++ ) {
        /* Q17 <- Q0*Q0/fftSize + 17 */
        f = (Fs*n)<<(17-fftLog);
        if (isHtk) {
            r = melR-hz2mel(f);
        } else {
            r = linR-f;
        }
        /* Q31 <- Q17*Q(invStp_exp) + 14-invStp_exp */
        sft = -(14-invStp_exp); rnd = 1L<<(sft-1);
        R = (ae_int64)rnd;
        AE_MULA32_LL(R, r, invStp_fract);
        weights[n-binLow] = AE_MOVAD32_L(AE_TRUNCA32F64S(R, 32-sft));
    } /* n */
    segments[bandNum] = (int16_t)(binR-binL);
    if (NULL!=fbeScales_fract) {
        /* Reciprocal bandwidth of the right-most band: 2/(linR-bandL) */
        fbeScales_fract[bandNum-1] = recip32x32(&fbeScales_exp[bandNum-1], linR-bandL, 17+1);
    }
} /* buildFilterbank() */
