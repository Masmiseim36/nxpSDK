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
/*          Copyright (C) 2015-2017 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
    NatureDSP Signal Processing Library. FFT part
    C code optimized for HiFi4
    Integrit, 2006-2017
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"

/*-------------------------------------------------------------------------
  Internal stages of functions stereo_fft_cplx32x16_ie(),
  stereo_ifft_cplx32x16_ie()

  Performs all stages of FFT except last one. It is assumed that
  the last stage is radix-2/radix-4 and implemented with
  bit-reversal permutation.
 
  Input/Output:
  x[N]                complex input signal. Real and imaginary data 
                      are interleaved and real data goes first

  Input:
  twd[N*twdstep*3/4]  twiddle factor table of a complex-valued FFT of 
                      size N*twdstep
  N                   FFT size
  twdstep             twiddle step 
  scalingOpt          scaling option

  Returned value:     total number of right shifts occurred during 
                      scaling procedure

  Restrictions:
  x - should not overlap and must be aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
int stereo_fft_cplx32x16_ie_inner(complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt)
{
    int offset_inc, s;
    int shift = 0;
    int tw_step = twdstep;
    int stride = N>>2;

    ae_int32x2 * restrict px0;
    ae_int32x2 * restrict px1;
    ae_int32x2 * restrict px2;
    ae_int32x2 * restrict px3;
    ae_int32x2 * restrict py0;
    ae_int32x2 * restrict py1;
    ae_int32x2 * restrict py2;
    ae_int32x2 * restrict py3;

    ae_int32   * restrict p16tw1;
    ae_int32   * restrict p16tw2;
    ae_int32   * restrict p16tw3;

    ae_int32x2  vA0, vA1, vA2, vA3,
                vB0, vB1, vB2, vB3,
                vC0, vC1, vC2, vC3;
    ae_int32x2 t1, t2, t3;
    ae_f16x4 t1_f16x4;
    ae_f16x4 t2_f16x4;
    ae_f16x4 t3_f16x4;
    int i, j, bexp;
    int log2n = 0;

    NASSERT_ALIGN8(x);
    NASSERT(scalingOpt==3 || scalingOpt==2);
    NASSERT(N>=16 && (N&(N-1))==0);

    /*
     * autoscaling case
     */
    if (scalingOpt == 2)
    {
        const int min_shift = 3;
        int32_t sumbits;
        ae_int32x2 acc, tmp;

        /* Calculate the exponent to prescale the input data */
        px0 = (ae_int32x2 *)x;
        acc = AE_ZERO32();
        __Pragma("loop_count min=4 factor=4");
        for (i = 0; i < N*2; i++)   
        {
            AE_L32X2_IP(tmp, px0, sizeof(ae_int16x4));
            tmp = AE_ABS32S(tmp); 
            acc = AE_OR32(acc, tmp);
        }
        acc = AE_OR32(acc, AE_SEL32_LH(acc, acc));
        sumbits = AE_MOVAD32_L(acc);
        bexp = NSA(sumbits);
        XT_MOVEQZ(bexp, sumbits, sumbits);

        /* First stage */
        {
            ae_int32x2 scl;
            int shiftl, shiftr;

            shift = min_shift - bexp;
            shiftl = XT_MAX(0, -shift);
            shiftr = XT_MAX(0,  shift);

            scl = 1 << shiftl;
            WUR_AE_SAR(shiftr);
            p16tw1 = (ae_int32*) (twd);
            p16tw2 = (ae_int32*) (twd+1*twdstep*(N>>2));
            p16tw3 = (ae_int32*) (twd+2*twdstep*(N>>2));

            offset_inc = tw_step*sizeof(complex_fract16);

            px0 = (ae_int32x2*)x;
            py0 = (ae_int32x2*)x;

            i = N>>2;
            do
            {
                AE_L32_XP( t1, p16tw1, offset_inc );
                AE_L32_XP( t2, p16tw2, offset_inc );
                AE_L32_XP( t3, p16tw3, offset_inc );
                t1_f16x4 = AE_MOVF16X4_FROMINT32X2(t1);
                t2_f16x4 = AE_MOVF16X4_FROMINT32X2(t2);
                t3_f16x4 = AE_MOVF16X4_FROMINT32X2(t3);

                /* left channel */
                vA3 = AE_L32X2_X(px0, 3*stride*2*sizeof(ae_int32x2));
                vA2 = AE_L32X2_X(px0, 2*stride*2*sizeof(ae_int32x2));
                vA1 = AE_L32X2_X(px0, 1*stride*2*sizeof(ae_int32x2));
                AE_L32X2_IP(vA0, px0, sizeof(ae_int32x2));

                vA0 = AE_MULP32X2(vA0, scl);
                vA1 = AE_MULP32X2(vA1, scl);
                vA2 = AE_MULP32X2(vA2, scl);
                vA3 = AE_MULP32X2(vA3, scl);
                AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                vB3 = AE_MUL32JS(vB3);
                AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                vC1 = AE_MULFC32X16RAS_L(vC1, t1_f16x4);
                vC2 = AE_MULFC32X16RAS_L(vC2, t2_f16x4);
                vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                AE_S32X2RNG_X (vC3, py0, 3*stride*2*sizeof(ae_int32x2));
                AE_S32X2RNG_X (vC2, py0, 1*stride*2*sizeof(ae_int32x2));
                AE_S32X2RNG_X (vC1, py0, 2*stride*2*sizeof(ae_int32x2));
                AE_S32X2RNG_IP(vC0, py0, sizeof(ae_int32x2));

                /* right channel */
                vA3 = AE_L32X2_X(px0, 3*stride*2*sizeof(ae_int32x2));
                vA2 = AE_L32X2_X(px0, 2*stride*2*sizeof(ae_int32x2));
                vA1 = AE_L32X2_X(px0, 1*stride*2*sizeof(ae_int32x2));
                AE_L32X2_IP(vA0, px0, sizeof(ae_int32x2));

                vA0 = AE_MULP32X2(vA0, scl);
                vA1 = AE_MULP32X2(vA1, scl);
                vA2 = AE_MULP32X2(vA2, scl);
                vA3 = AE_MULP32X2(vA3, scl);
                AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                vB3 = AE_MUL32JS(vB3);
                AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                vC1 = AE_MULFC32X16RAS_L(vC1, t1_f16x4);
                vC2 = AE_MULFC32X16RAS_L(vC2, t2_f16x4);
                vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                AE_S32X2RNG_X (vC3, py0, 3*stride*2*sizeof(ae_int32x2));
                AE_S32X2RNG_X (vC2, py0, 1*stride*2*sizeof(ae_int32x2));
                AE_S32X2RNG_X (vC1, py0, 2*stride*2*sizeof(ae_int32x2));
                AE_S32X2RNG_IP(vC0, py0, sizeof(ae_int32x2));
            }while(--i);

            stride>>=2;
            tw_step<<=2;
        }
        /* Intermediate stage */
        while( stride > 1 )
        {
            /* set pointers and access stride for twiddle factors */
            p16tw1 = (ae_int32*) (twd);
            p16tw2 = (ae_int32*) (twd+1*twdstep*(N>>2));
            p16tw3 = (ae_int32*) (twd+2*twdstep*(N>>2));
            offset_inc = tw_step*sizeof(complex_fract16);

            AE_CALCRNG3();
            bexp = 3 - RUR_AE_SAR();
            s = XT_MAX(0, min_shift - bexp);
            WUR_AE_SAR(s);
            shift += s;

            log2n += 2;

            for (j = 0; j < (stride>>1); j++)
            {
                /* load twiddle factors */
                AE_L32_XP( t1, p16tw1, offset_inc );
                AE_L32_XP( t2, p16tw2, offset_inc );
                t1 = AE_SEL32_LL(t1, t2);
                t1_f16x4 = AE_MOVF16X4_FROMINT32X2(t1);
                AE_L32_XP( t1, p16tw3, offset_inc );
                AE_L32_XP( t2, p16tw1, offset_inc );
                t1 = AE_SEL32_LL(t1, t2);
                t2_f16x4 = AE_MOVF16X4_FROMINT32X2(t1);
                AE_L32_XP( t1, p16tw2, offset_inc );
                AE_L32_XP( t2, p16tw3, offset_inc );
                t1 = AE_SEL32_LL(t1, t2);
                t3_f16x4 = AE_MOVF16X4_FROMINT32X2(t1);

                px0 = (ae_int32x2*)x+j*2*2;
                py0 = (ae_int32x2*)x+j*2*2;
                px1 = px0 + 2*stride;
                py1 = py0 + 2*stride;
                px2 = px1 + 2*stride;
                py2 = py1 + 2*stride;
                px3 = px2 + 2*stride;
                py3 = py2 + 2*stride;

                __Pragma("loop_count min=2");
                for (i = 0; i < (1<<log2n); i++)
                {
                    /* butterfly 0, left channel */
                    AE_L32X2_IP(vA0, px0, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA1, px1, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA2, px2, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA3, px3, sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_H(vC1, t1_f16x4);
                    vC2 = AE_MULFC32X16RAS_L(vC2, t1_f16x4);
                    vC3 = AE_MULFC32X16RAS_H(vC3, t2_f16x4);

                    AE_S32X2RNG_IP(vC0, py0, sizeof(ae_int32x2));
                    AE_S32X2RNG_IP(vC2, py1, sizeof(ae_int32x2));
                    AE_S32X2RNG_IP(vC1, py2, sizeof(ae_int32x2));
                    AE_S32X2RNG_IP(vC3, py3, sizeof(ae_int32x2));

                    /* butterfly 0, right channel */
                    AE_L32X2_IP(vA0, px0, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA1, px1, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA2, px2, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA3, px3, sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_H(vC1, t1_f16x4);
                    vC2 = AE_MULFC32X16RAS_L(vC2, t1_f16x4);
                    vC3 = AE_MULFC32X16RAS_H(vC3, t2_f16x4);

                    AE_S32X2RNG_IP(vC0, py0, sizeof(ae_int32x2));
                    AE_S32X2RNG_IP(vC2, py1, sizeof(ae_int32x2));
                    AE_S32X2RNG_IP(vC1, py2, sizeof(ae_int32x2));
                    AE_S32X2RNG_IP(vC3, py3, sizeof(ae_int32x2));

                    /* butterfly 1, left channel */
                    AE_L32X2_IP(vA0, px0, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA1, px1, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA2, px2, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA3, px3, sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_L(vC1, t2_f16x4);
                    vC2 = AE_MULFC32X16RAS_H(vC2, t3_f16x4);
                    vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                    AE_S32X2RNG_IP(vC0, py0, sizeof(ae_int32x2));
                    AE_S32X2RNG_IP(vC2, py1, sizeof(ae_int32x2));
                    AE_S32X2RNG_IP(vC1, py2, sizeof(ae_int32x2));
                    AE_S32X2RNG_IP(vC3, py3, sizeof(ae_int32x2));

                    /* butterfly 1, right channel */
                    AE_L32X2_XP(vA0, px0, (4*stride*2-3)*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA1, px1, (4*stride*2-3)*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA2, px2, (4*stride*2-3)*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA3, px3, (4*stride*2-3)*sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_L(vC1, t2_f16x4);
                    vC2 = AE_MULFC32X16RAS_H(vC2, t3_f16x4);
                    vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                    AE_S32X2RNG_XP(vC0, py0, (4*stride*2-3)*sizeof(ae_int32x2));
                    AE_S32X2RNG_XP(vC2, py1, (4*stride*2-3)*sizeof(ae_int32x2));
                    AE_S32X2RNG_XP(vC1, py2, (4*stride*2-3)*sizeof(ae_int32x2));
                    AE_S32X2RNG_XP(vC3, py3, (4*stride*2-3)*sizeof(ae_int32x2));
                }
            }

            stride>>=2;
            tw_step<<=2;
        }
    }
    /*
     * static scaling case
     */
    else
    {
        /* First stage */
        {
            p16tw1 = (ae_int32*) (twd);
            p16tw2 = (ae_int32*) (twd+1*twdstep*(N>>2));
            p16tw3 = (ae_int32*) (twd+2*twdstep*(N>>2));
            offset_inc = tw_step*sizeof(complex_fract16);

            WUR_AE_SAR(3);
            shift += 3;

            px0 = (ae_int32x2*)x;
            py0 = (ae_int32x2*)x;

            i = N>>2;
            do
            {
                AE_L32_XP( t1, p16tw1, offset_inc );
                AE_L32_XP( t2, p16tw2, offset_inc );
                AE_L32_XP( t3, p16tw3, offset_inc );
                t1_f16x4 = AE_MOVF16X4_FROMINT32X2(t1);
                t2_f16x4 = AE_MOVF16X4_FROMINT32X2(t2);
                t3_f16x4 = AE_MOVF16X4_FROMINT32X2(t3);

                /* Left channel */
                vA3 = AE_L32X2_X(px0, 3*stride*2*sizeof(ae_int32x2));
                vA2 = AE_L32X2_X(px0, 2*stride*2*sizeof(ae_int32x2));
                vA1 = AE_L32X2_X(px0, 1*stride*2*sizeof(ae_int32x2));
                AE_L32X2_IP(vA0, px0, sizeof(ae_int32x2));

                AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                vB3 = AE_MUL32JS(vB3);
                AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                vC1 = AE_MULFC32X16RAS_L(vC1, t1_f16x4);
                vC2 = AE_MULFC32X16RAS_L(vC2, t2_f16x4);
                vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                AE_S32X2_X (vC3, py0, 3*stride*2*sizeof(ae_int32x2));
                AE_S32X2_X (vC2, py0, 1*stride*2*sizeof(ae_int32x2));
                AE_S32X2_X (vC1, py0, 2*stride*2*sizeof(ae_int32x2));
                AE_S32X2_IP(vC0, py0, sizeof(ae_int32x2));

                /* Right channel */
                vA3 = AE_L32X2_X(px0, 3*stride*2*sizeof(ae_int32x2));
                vA2 = AE_L32X2_X(px0, 2*stride*2*sizeof(ae_int32x2));
                vA1 = AE_L32X2_X(px0, 1*stride*2*sizeof(ae_int32x2));
                AE_L32X2_IP(vA0, px0, sizeof(ae_int32x2));

                AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                vB3 = AE_MUL32JS(vB3);
                AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                vC1 = AE_MULFC32X16RAS_L(vC1, t1_f16x4);
                vC2 = AE_MULFC32X16RAS_L(vC2, t2_f16x4);
                vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                AE_S32X2_X (vC3, py0, 3*stride*2*sizeof(ae_int32x2));
                AE_S32X2_X (vC2, py0, 1*stride*2*sizeof(ae_int32x2));
                AE_S32X2_X (vC1, py0, 2*stride*2*sizeof(ae_int32x2));
                AE_S32X2_IP(vC0, py0, sizeof(ae_int32x2));

            }while(--i);

            stride>>=2;
            tw_step<<=2;
        }

        /* Intermediate stage */
        while( stride > 1 )
        {
            /* set pointers and access stride for twiddle factors */
            p16tw1 = (ae_int32*) (twd);
            p16tw2 = (ae_int32*) (twd+1*twdstep*(N>>2));
            p16tw3 = (ae_int32*) (twd+2*twdstep*(N>>2));
            offset_inc = tw_step*sizeof(complex_fract16);

            WUR_AE_SAR(2);
            shift += 2;
            log2n += 2;

            for (j = 0; j < (stride>>1); j++)
            {
                /* load twiddle factors */
                AE_L32_XP( t1, p16tw1, offset_inc );
                AE_L32_XP( t2, p16tw2, offset_inc );
                t1 = AE_SEL32_LL(t1, t2);
                t1_f16x4 = AE_MOVF16X4_FROMINT32X2(t1);
                AE_L32_XP( t1, p16tw3, offset_inc );
                AE_L32_XP( t2, p16tw1, offset_inc );
                t1 = AE_SEL32_LL(t1, t2);
                t2_f16x4 = AE_MOVF16X4_FROMINT32X2(t1);
                AE_L32_XP( t1, p16tw2, offset_inc );
                AE_L32_XP( t2, p16tw3, offset_inc );
                t1 = AE_SEL32_LL(t1, t2);
                t3_f16x4 = AE_MOVF16X4_FROMINT32X2(t1);

                px0 = (ae_int32x2*)x+j*2*2;
                py0 = (ae_int32x2*)x+j*2*2;
                px1 = px0 + 2*stride;
                py1 = py0 + 2*stride;
                px2 = px1 + 2*stride;
                py2 = py1 + 2*stride;
                px3 = px2 + 2*stride;
                py3 = py2 + 2*stride;

                __Pragma("loop_count min=2");
                for (i = 0; i < (1<<log2n); i++)
                {
                    /* butterfly 0, left channel */
                    AE_L32X2_IP(vA0, px0, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA1, px1, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA2, px2, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA3, px3, sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_H(vC1, t1_f16x4);
                    vC2 = AE_MULFC32X16RAS_L(vC2, t1_f16x4);
                    vC3 = AE_MULFC32X16RAS_H(vC3, t2_f16x4);

                    AE_S32X2_IP(vC0, py0, sizeof(ae_int32x2));
                    AE_S32X2_IP(vC2, py1, sizeof(ae_int32x2));
                    AE_S32X2_IP(vC1, py2, sizeof(ae_int32x2));
                    AE_S32X2_IP(vC3, py3, sizeof(ae_int32x2));

                    /* butterfly 0, right channel */
                    AE_L32X2_IP(vA0, px0, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA1, px1, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA2, px2, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA3, px3, sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_H(vC1, t1_f16x4);
                    vC2 = AE_MULFC32X16RAS_L(vC2, t1_f16x4);
                    vC3 = AE_MULFC32X16RAS_H(vC3, t2_f16x4);

                    AE_S32X2_IP(vC0, py0, sizeof(ae_int32x2));
                    AE_S32X2_IP(vC2, py1, sizeof(ae_int32x2));
                    AE_S32X2_IP(vC1, py2, sizeof(ae_int32x2));
                    AE_S32X2_IP(vC3, py3, sizeof(ae_int32x2));

                    /* butterfly 1, left channel */
                    AE_L32X2_IP(vA0, px0, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA1, px1, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA2, px2, sizeof(ae_int32x2));
                    AE_L32X2_IP(vA3, px3, sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_L(vC1, t2_f16x4);
                    vC2 = AE_MULFC32X16RAS_H(vC2, t3_f16x4);
                    vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                    AE_S32X2_IP(vC0, py0, sizeof(ae_int32x2));
                    AE_S32X2_IP(vC2, py1, sizeof(ae_int32x2));
                    AE_S32X2_IP(vC1, py2, sizeof(ae_int32x2));
                    AE_S32X2_IP(vC3, py3, sizeof(ae_int32x2));

                    /* butterfly 1, right channel */
                    AE_L32X2_XP(vA0, px0, (4*2*stride-3)*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA1, px1, (4*2*stride-3)*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA2, px2, (4*2*stride-3)*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA3, px3, (4*2*stride-3)*sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_L(vC1, t2_f16x4);
                    vC2 = AE_MULFC32X16RAS_H(vC2, t3_f16x4);
                    vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                    AE_S32X2_XP(vC0, py0, (4*2*stride-3)*sizeof(ae_int32x2));
                    AE_S32X2_XP(vC2, py1, (4*2*stride-3)*sizeof(ae_int32x2));
                    AE_S32X2_XP(vC1, py2, (4*2*stride-3)*sizeof(ae_int32x2));
                    AE_S32X2_XP(vC3, py3, (4*2*stride-3)*sizeof(ae_int32x2));
                }
            }

            stride>>=2;
            tw_step<<=2;
        }
    }

    return shift;
} /* stereo_fft_cplx32x16_ie_inner() */
