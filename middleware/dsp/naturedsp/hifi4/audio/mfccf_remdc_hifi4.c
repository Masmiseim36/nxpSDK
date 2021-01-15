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
/* MFCC extractor internal declarations. */
#include "mfcc_internal.h"

#if HAVE_VFPU || HAVE_FPU

#define ALIGN_SIZE            (XCHAL_DATA_WIDTH)
#define USE_REFERENCE_CODE    0

/*
 * Compute the mean value over the input argument x[N], then subtract it from 
 * each element of x[N] and store the result to the output argument y[N]. 
 * Input and output arguments may refer to the same array.
 * Note:
 *   For the fixed-point variant, input data should be properly scaled, 
 *   otherwise results may saturate. It is sufficient to ensure that the
 *   minimum number of redunsant sign bits over input data is non-zero to
 *   prevent overflow.
 * Input:
 *   N          Vectors size
 *   x[N]       Input vector
 * Output:
 *   y[N]       Output vector
 * Restrictions:
 *   N          Must be a multiple of 2
 *   x[N],y[N]  Must be 8-bytes aligned
 */

void mfccf_remdc( float32_t * y, const float32_t * x, int N )
{
#if !USE_REFERENCE_CODE
    /*
     * Reference code:
     *   float32_t s, t, acch, accl, mean;
     *   int n;
     *   NASSERT_ALIGN(y, ALIGN_SIZE);
     *   NASSERT_ALIGN(x, ALIGN_SIZE);
     *   NASSERT(0==(N%2));
     *   acch = accl = 0;
     *   for ( n=0; n<N; n++ ) {
     *       s = x[n];
     *       t = s+accl+acch;
     *       // It is essential to accumulate the sum in extended precision.
     *       accl = -(t-acch-accl-s);
     *       acch = t;
     *   }
     *   mean = acch/N;
     *   for ( n=0; n<N; n++ ) {
     *       y[n] = x[n]-mean;
     *   }
     */
#if HAVE_VFPU
    const xtfloatx2 * restrict X0;
    const xtfloatx2 * restrict X1;
    const xtfloatx2 * restrict X2;
    const xtfloatx2 * restrict X3;
    xtfloatx2 acch0, acch1, acch2, acch3;
    xtfloatx2 accl0, accl1, accl2, accl3;
    xtfloatx2 s, t;
    float32_t mean;
    int n;
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(0==(N%2));
    X0 = (xtfloatx2*)&x[0*2];
    X1 = (xtfloatx2*)&x[1*2];
    X2 = (xtfloatx2*)&x[2*2];
    X3 = (xtfloatx2*)&x[3*2];
    acch0 = acch1 = acch2 = acch3 = XT_CONST_S(0);
    accl0 = accl1 = accl2 = accl3 = XT_CONST_S(0);
    /* 20 cycles per pipeline stage in steady state with unroll=1 */
    for ( n=0; n<(N>>3); n++ ) {
        XT_LSX2IP(s, X0, 4*sizeof(s));
        /* t <- s+accl+acch */
        t = XT_ADD_SX2(XT_ADD_SX2(s, accl0), acch0);
        /* accl <- -(t-acch-accl-s); acch <- t */
        accl0 = XT_ADD_SX2(XT_ADD_SX2(XT_SUB_SX2(acch0, t), accl0), s);
        acch0 = t;
        XT_LSX2IP(s, X1, 4*sizeof(s));
        t = XT_ADD_SX2(XT_ADD_SX2(s, accl1), acch1);
        accl1 = XT_ADD_SX2(XT_ADD_SX2(XT_SUB_SX2(acch1, t), accl1), s);
        acch1 = t;
        XT_LSX2IP(s, X2, 4*sizeof(s));
        t = XT_ADD_SX2(XT_ADD_SX2(s, accl2), acch2);
        accl2 = XT_ADD_SX2(XT_ADD_SX2(XT_SUB_SX2(acch2, t), accl2), s);
        acch2 = t;
        XT_LSX2IP(s, X3, 4*sizeof(s));
        t = XT_ADD_SX2(XT_ADD_SX2(s, accl3), acch3);
        accl3 = XT_ADD_SX2(XT_ADD_SX2(XT_SUB_SX2(acch3, t), accl3), s);
        acch3 = t;
    } /* n */
    if ((N&6)==6) {
        s = XT_LSX2I(X0, 0);
        t = XT_ADD_SX2(XT_ADD_SX2(s, accl0), acch0);
        accl0 = XT_ADD_SX2(XT_ADD_SX2(XT_SUB_SX2(acch0, t), accl0), s);
        acch0 = t;
        s = XT_LSX2I(X1, 0);
        t = XT_ADD_SX2(XT_ADD_SX2(s, accl1), acch1);
        accl1 = XT_ADD_SX2(XT_ADD_SX2(XT_SUB_SX2(acch1, t), accl1), s);
        acch1 = t;
        s = XT_LSX2I(X2, 0);
        t = XT_ADD_SX2(XT_ADD_SX2(s, accl2), acch2);
        accl2 = XT_ADD_SX2(XT_ADD_SX2(XT_SUB_SX2(acch2, t), accl2), s);
        acch2 = t;
    } else if ((N&6)==4) {
        s = XT_LSX2I(X0, 0);
        t = XT_ADD_SX2(XT_ADD_SX2(s, accl0), acch0);
        accl0 = XT_ADD_SX2(XT_ADD_SX2(XT_SUB_SX2(acch0, t), accl0), s);
        acch0 = t;
        s = XT_LSX2I(X1, 0);
        t = XT_ADD_SX2(XT_ADD_SX2(s, accl1), acch1);
        accl1 = XT_ADD_SX2(XT_ADD_SX2(XT_SUB_SX2(acch1, t), accl1), s);
        acch1 = t;
    } else if ((N&6)==2) {
        s = XT_LSX2I(X0, 0);
        t = XT_ADD_SX2(XT_ADD_SX2(s, accl0), acch0);
        accl0 = XT_ADD_SX2(XT_ADD_SX2(XT_SUB_SX2(acch0, t), accl0), s);
        acch0 = t;
    }
    t = XT_ADD_SX2(XT_ADD_SX2(XT_ADD_SX2(accl0, accl1), accl2), accl3);
    t = XT_ADD_SX2(XT_ADD_SX2(XT_ADD_SX2(XT_ADD_SX2(t, acch0), acch1), acch2), acch3);
    mean = XT_DIV_S(XT_RADD_SX2(t), XT_FLOAT_S(N, 0));
    __Pragma("aligned(x,8)");
    __Pragma("aligned(y,8)");
    __Pragma("loop_count factor=2");
    __Pragma("concurrent");
    /* 4 cycles per pipeline stage in steady state with unroll=4 */
    for ( n=0; n<N; n++ ) {
        y[n] = x[n]-mean;
    }
#elif HAVE_FPU
    const xtfloat * restrict X0;
    const xtfloat * restrict X1;
    const xtfloat * restrict X2;
    const xtfloat * restrict X3;
    xtfloat acch0, acch1, acch2, acch3;
    xtfloat accl0, accl1, accl2, accl3;
    xtfloat s, t;
    float32_t mean;
    int n;
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(0==(N%2));
    X0 = (xtfloat*)&x[0];
    X1 = (xtfloat*)&x[1];
    X2 = (xtfloat*)&x[2];
    X3 = (xtfloat*)&x[3];
    acch0 = acch1 = acch2 = acch3 = XT_CONST_S(0);
    accl0 = accl1 = accl2 = accl3 = XT_CONST_S(0);
    /* 20 cycles per pipeline stage in steady state with unroll=1 */
    for ( n=0; n<(N>>2); n++ ) {
        XT_LSIP(s, X0, 4*sizeof(s));
        /* t <- s+accl+acch */
        t = XT_ADD_S(XT_ADD_S(s, accl0), acch0);
        /* accl <- -(t-acch-accl-s); acch <- t */
        accl0 = XT_ADD_S(XT_ADD_S(XT_SUB_S(acch0, t), accl0), s);
        acch0 = t;
        XT_LSIP(s, X1, 4*sizeof(s));
        t = XT_ADD_S(XT_ADD_S(s, accl1), acch1);
        accl1 = XT_ADD_S(XT_ADD_S(XT_SUB_S(acch1, t), accl1), s);
        acch1 = t;
        XT_LSIP(s, X2, 4*sizeof(s));
        t = XT_ADD_S(XT_ADD_S(s, accl2), acch2);
        accl2 = XT_ADD_S(XT_ADD_S(XT_SUB_S(acch2, t), accl2), s);
        acch2 = t;
        XT_LSIP(s, X3, 4*sizeof(s));
        t = XT_ADD_S(XT_ADD_S(s, accl3), acch3);
        accl3 = XT_ADD_S(XT_ADD_S(XT_SUB_S(acch3, t), accl3), s);
        acch3 = t;
    } /* n */
    if ((N&3)==3) {
        s = XT_LSI(X0, 0);
        t = XT_ADD_S(XT_ADD_S(s, accl0), acch0);
        accl0 = XT_ADD_S(XT_ADD_S(XT_SUB_S(acch0, t), accl0), s);
        acch0 = t;
        s = XT_LSI(X1, 0);
        t = XT_ADD_S(XT_ADD_S(s, accl1), acch1);
        accl1 = XT_ADD_S(XT_ADD_S(XT_SUB_S(acch1, t), accl1), s);
        acch1 = t;
        s = XT_LSI(X2, 0);
        t = XT_ADD_S(XT_ADD_S(s, accl2), acch2);
        accl2 = XT_ADD_S(XT_ADD_S(XT_SUB_S(acch2, t), accl2), s);
        acch2 = t;
    } else if ((N&3)==2) {
        s = XT_LSI(X0, 0);
        t = XT_ADD_S(XT_ADD_S(s, accl0), acch0);
        accl0 = XT_ADD_S(XT_ADD_S(XT_SUB_S(acch0, t), accl0), s);
        acch0 = t;
        s = XT_LSI(X1, 0);
        t = XT_ADD_S(XT_ADD_S(s, accl1), acch1);
        accl1 = XT_ADD_S(XT_ADD_S(XT_SUB_S(acch1, t), accl1), s);
        acch1 = t;
    } else if ((N&3)==1) {
        s = XT_LSI(X0, 0);
        t = XT_ADD_S(XT_ADD_S(s, accl0), acch0);
        accl0 = XT_ADD_S(XT_ADD_S(XT_SUB_S(acch0, t), accl0), s);
        acch0 = t;
    }
    t = XT_ADD_S(XT_ADD_S(XT_ADD_S(accl0, accl1), accl2), accl3);
    t = XT_ADD_S(XT_ADD_S(XT_ADD_S(XT_ADD_S(t, acch0), acch1), acch2), acch3);
    mean = XT_DIV_S(t, XT_FLOAT_S(N, 0));
    __Pragma("concurrent");
    /* 12 cycles per pipeline stage in steady state with unroll=4 */
    for ( n=0; n<N; n++ ) {
        y[n] = x[n]-mean;
    }
#endif /* HAVE_FPU */
#else /* USE_REFERENCE_CODE */
    float32_t s, t, acch, accl, mean;
    int n;
    NASSERT_ALIGN(y, ALIGN_SIZE);
    NASSERT_ALIGN(x, ALIGN_SIZE);
    NASSERT(0==(N%2));
    acch = accl = 0;
    for ( n=0; n<N; n++ ) {
        s = x[n];
        t = s+accl+acch;
        /* It is essential to accumulate the sum in extended precision. */
        accl = -(t-acch-accl-s);
        acch = t;
    }
    mean = acch/N;
    for ( n=0; n<N; n++ ) {
        y[n] = x[n]-mean;
    }
#endif
} /* mfccf_remdc() */

#endif /* HAVE_VFPU || HAVE_FPU */
