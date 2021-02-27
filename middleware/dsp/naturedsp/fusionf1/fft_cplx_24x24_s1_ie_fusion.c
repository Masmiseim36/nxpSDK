/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
	NatureDSP Signal Processing Library. FFT part
    C code optimized for Fusion
*/
#include "NatureDSP_Signal.h"
#include "common.h"



static int fft24x24_stage_last_s1(int32_t *x,
    int32_t *y,
    int N, int exp)
{
    int32_t i, i0, i1, ai;
    ae_f24x2 t0, t1, t2, t3;

    ae_f24x2 * restrict p_y0 = (ae_f24x2 *)(y);
    ae_f24x2 * restrict p_y1 = (p_y0 + (N >> 2));
    ae_f24x2 * restrict p_y2 = (p_y1 + (N >> 2));
    ae_f24x2 * restrict p_y3 = (p_y2 + (N >> 2));
    ae_f24x2 * restrict p_x0 = (ae_f24x2 *)(x);
    int shift;


    ae_int32x2 tmp;

    tmp = AE_MOVDA32X2(N, N);
    i = AE_NSAZ32_L(tmp) + 1;
    ai = ((int32_t)0x1) << i;
    i0 = 0;


    if ((i & 1) == 0)
    {
        shift = 2 - exp + 8;    //Select scaling
        WUR_AE_SAR(shift);
        //--------------------------------------------------------------------------
        // last stage is RADIX2 !!!
        //--------------------------------------------------------------------------
        /*

        #<swps>  18 cycles per pipeline stage in steady state with unroll=1
        #<swps>   2 pipeline stages
        #<swps>  34 real ops (excluding nop)
        #<swps>
        #<swps>      min  18 cycles required by resources
        #<swps>      min   2 cycles required by recurrences
        #<swps>      min  18 cycles required by resources/recurrence
        #<swps>      min   6 cycles required for critical path
        #<swps>           32 cycles non-loop schedule length
        */
        i = N >> 3;
        do
        {
            ae_int32x2 vA1, vA2, vA3, vA0;
            ae_int32x2 vB1, vB2, vB3, vB0;
            i1 = AE_ADDBRBA32(i0, ai);

            // FFT_BUTTERFLY_R2(i0, shift);

            AE_L32X2F24_IP(t0, p_x0, 8);
            AE_L32X2F24_IP(t1, p_x0, 8);
            AE_L32X2F24_IP(t2, p_x0, 8);
            AE_L32X2F24_IP(t3, p_x0, 8);

            vA0 = t0;
            vA1 = t1;
            vA2 = t2;
            vA3 = t3;
#ifdef SHIFT_FIRST            
            vA0 = AE_SRAS32(vA0);
            vA1 = AE_SRAS32(vA1);
            vA2 = AE_SRAS32(vA2);
            vA3 = AE_SRAS32(vA3);
#endif            
            vB0 = AE_ADD32S(vA0, vA1);
            vB2 = AE_SUB32S(vA0, vA1);
            vB1 = AE_ADD32S(vA2, vA3);
            vB3 = AE_SUB32S(vA2, vA3);
#ifndef SHIFT_FIRST
            vB0 = AE_SRAS32(vB0);
            vB1 = AE_SRAS32(vB1);
            vB2 = AE_SRAS32(vB2);
            vB3 = AE_SRAS32(vB3);
#endif
            t0 = AE_MOVF24X2_FROMINT32X2(vB0);
            t1 = AE_MOVF24X2_FROMINT32X2(vB1);
            t2 = AE_MOVF24X2_FROMINT32X2(vB2);
            t3 = AE_MOVF24X2_FROMINT32X2(vB3);

            AE_S32X2F24_X(t0, p_y0, i0);
            AE_S32X2F24_X(t1, p_y1, i0);
            AE_S32X2F24_X(t2, p_y2, i0);
            AE_S32X2F24_X(t3, p_y3, i0);

            //FFT_BUTTERFLY_R2(i1, shift);

            AE_L32X2F24_IP(t0, p_x0, 8);
            AE_L32X2F24_IP(t1, p_x0, 8);
            AE_L32X2F24_IP(t2, p_x0, 8);
            AE_L32X2F24_IP(t3, p_x0, 8);

            vA0 = t0;
            vA1 = t1;
            vA2 = t2;
            vA3 = t3;
#ifdef SHIFT_FIRST
            vA0 = AE_SRAA32RS(vA0, shift);
            vA1 = AE_SRAA32RS(vA1, shift);
            vA2 = AE_SRAA32RS(vA2, shift);
            vA3 = AE_SRAA32RS(vA3, shift);
#endif
            vB0 = AE_ADD32S(vA0, vA1);
            vB2 = AE_SUB32S(vA0, vA1);
            vB1 = AE_ADD32S(vA2, vA3);
            vB3 = AE_SUB32S(vA2, vA3);

#ifndef SHIFT_FIRST
            vB0 = AE_SRAS32(vB0);
            vB1 = AE_SRAS32(vB1);
            vB2 = AE_SRAS32(vB2);
            vB3 = AE_SRAS32(vB3);
#endif

            t0 = AE_MOVF24X2_FROMINT32X2(vB0);
            t1 = AE_MOVF24X2_FROMINT32X2(vB1);
            t2 = AE_MOVF24X2_FROMINT32X2(vB2);
            t3 = AE_MOVF24X2_FROMINT32X2(vB3);

            AE_S32X2F24_X(t0, p_y0, i1);
            AE_S32X2F24_X(t1, p_y1, i1);
            AE_S32X2F24_X(t2, p_y2, i1);
            AE_S32X2F24_X(t3, p_y3, i1);

            i0 = AE_ADDBRBA32(i1, ai);
        } while (--i);

    }
    else
    {

        //--------------------------------------------------------------------------
        // last stage is RADIX4 !!!
        //--------------------------------------------------------------------------
        shift = 3 - exp + 8;    //Select scaling
        WUR_AE_SAR(shift);
        /*

        #<swps>   9 cycles per pipeline stage in steady state with unroll=1
        #<swps>   2 pipeline stages
        #<swps>  22 real ops (excluding nop)
        #<swps>
        #<swps>      min   9 cycles required by resources
        #<swps>      min   2 cycles required by recurrences
        #<swps>      min   9 cycles required by resources/recurrence
        #<swps>      min   6 cycles required for critical path
        #<swps>           18 cycles non-loop schedule length

        */
        i = N >> 2;
        do//for (i = 0; i < (N>>4); i++) 
        {
            ae_int32x2 vA1, vA2, vA3, vA0;
            ae_int32x2 vB1, vB2, vB3, vB0;


            //     FFT_BUTTERFLY_R4(i0, shift);
            AE_L32X2F24_IP(t0, p_x0, 8);
            AE_L32X2F24_IP(t1, p_x0, 8);
            AE_L32X2F24_IP(t2, p_x0, 8);
            AE_L32X2F24_IP(t3, p_x0, 8);

            vA0 = t0;
            vA1 = t1;
            vA2 = t2;
            vA3 = t3;
#ifdef SHIFT_FIRST
            vA0 = AE_SRAS32(vA0);
            vA1 = AE_SRAS32(vA1);
            vA2 = AE_SRAS32(vA2);
            vA3 = AE_SRAS32(vA3);
#endif
            vB0 = AE_ADD32S(vA0, vA2);
            vB2 = AE_SUB32S(vA0, vA2);
            vB1 = AE_ADD32S(vA1, vA3);
            vB3 = AE_SUB32S(vA1, vA3);

            vB3 = AE_SEL32_LH(vB3, vB3);

            vA0 = AE_ADD32S(vB0, vB1);
            vA2 = AE_SUB32S(vB0, vB1);
            vA1 = AE_ADDSUB32S(vB2, vB3);
            vA3 = AE_SUBADD32S(vB2, vB3);

#ifndef SHIFT_FIRST
            vA0 = AE_SRAS32(vA0);
            vA1 = AE_SRAS32(vA1);
            vA2 = AE_SRAS32(vA2);
            vA3 = AE_SRAS32(vA3);
#endif

            t0 = AE_MOVF24X2_FROMINT32X2(vA0);
            t1 = AE_MOVF24X2_FROMINT32X2(vA1);
            t2 = AE_MOVF24X2_FROMINT32X2(vA2);
            t3 = AE_MOVF24X2_FROMINT32X2(vA3);

            AE_S32X2F24_X(t0, p_y0, i0);
            AE_S32X2F24_X(t1, p_y1, i0);
            AE_S32X2F24_X(t2, p_y2, i0);
            AE_S32X2F24_X(t3, p_y3, i0);

            i0 = AE_ADDBRBA32(i0, ai);
        } while (--i);
    }
    return shift;
} //fft_stage_last_i


/*===========================================================================

fft_cplx_24x24_s1_ie:
Forward complex FFT with autoscaling (scalingOpt=1)

N - sizeof of transform
x - input 
y - output
twd - twiddles table
twdstep - twiddles stride 
exp - input block exponent. 

Called from:

fft_real_24x24_ie_24p
ifft_real_24x24_ie_24p

===========================================================================*/


int fft_cplx_24x24_s1_ie(f24* y, f24* x, const     f24* twd, int twdstep, int N, int exp)
{
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(N == 128 || N == 256 || N == 512 || N == 1024);

    int shift = 0;
    int s;
    int stride = N / 4;
    int M = N*twdstep;

    ae_f24x2   * restrict py24;
    ae_f24x2   * restrict px24;

    ae_f24x2   * restrict ptw1 = (ae_f24x2*)twd;
    ae_f24x2   * restrict ptw2 = (ae_f24x2*)(twd + 1 * 2 * M / 4);
    ae_f24x2   * restrict ptw3 = (ae_f24x2*)(twd + 2 * 2 * M / 4);

    ae_int32x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3, vC0, vC1, vC2, vC3;


    ae_f24x2    vT1, vT2, vT3;
    int i;
    int log2n = 0;

    WUR_AE_CBEGIN0((unsigned)x);
    WUR_AE_CEND0((unsigned)(uintptr_t)(x + (N - 1) * 2));

    s = 3 - exp + 8;  //Scaling for first stage
    while (stride > 1)
    {
        ae_int32x2  maxV = 0, minV = 0;

        unsigned acc = 0;
        const unsigned acc_inc = (log2n == 0) ? 0 : (0x80000000 >> (log2n - 1));

        WUR_AE_SAR(s);
        shift += s;
        log2n += 2;
        px24 = py24 = (ae_f24x2*)x;

        ptw1 = (ae_f24x2*)twd;
        ptw2 = (ae_f24x2*)(twd + 1 * 2 * M / 4);
        ptw3 = (ae_f24x2*)(twd + 2 * 2 * M / 4);

        i = N / 4;
        //14 cycles per pipeline stage in steady state with unroll=1
        do
        {  
            ae_f24x2 t1, t2, t3, t0;
            int offset_inc = 0;
            acc += acc_inc;
            XT_MOVEQZ(offset_inc, twdstep * 8, acc);


            vA3 = AE_L32X2F24_X(px24, 3 * 8 * stride);
            vA2 = AE_L32X2F24_X(px24, 2 * 8 * stride);
            vA1 = AE_L32X2F24_X(px24, 1 * 8 * stride);
            AE_L32X2F24_XC(t0, px24, 4 * 8 * stride);
            vA0 = t0;

            vB0 = AE_ADD32S(vA0, vA2);
            vB2 = AE_SUB32S(vA0, vA2);
            vB1 = AE_ADD32S(vA1, vA3);
            vB3 = AE_SUB32S(vA1, vA3);

            vB3 = AE_SEL32_LH(vB3, vB3);

            vC0 = AE_ADD32S(vB0, vB1);
            vC2 = AE_SUB32S(vB0, vB1);
            vC3 = AE_SUBADD32S(vB2, vB3);
            vC1 = AE_ADDSUB32S(vB2, vB3);

            vC0 = AE_SRAS32(vC0);
            vC2 = AE_SRAS32(vC2);
            vC3 = AE_SRAS32(vC3);
            vC1 = AE_SRAS32(vC1);

            AE_L32X2F24_XP(vT1, ptw1, offset_inc);
            AE_L32X2F24_XP(vT2, ptw2, offset_inc);
            AE_L32X2F24_XP(vT3, ptw3, offset_inc);

            t1 = AE_MOVF24X2_FROMINT32X2(vC1);
            t2 = AE_MOVF24X2_FROMINT32X2(vC2);
            t3 = AE_MOVF24X2_FROMINT32X2(vC3);

            vC1 = AE_MULFC24RA(t1, vT1);
            vC2 = AE_MULFC24RA(t2, vT2);
            vC3 = AE_MULFC24RA(t3, vT3);

            t0 = AE_MOVF24X2_FROMINT32X2(vC0);
            t1 = AE_MOVF24X2_FROMINT32X2(vC1);
            t2 = AE_MOVF24X2_FROMINT32X2(vC2);
            t3 = AE_MOVF24X2_FROMINT32X2(vC3);

            minV = AE_MIN32(minV, vC0); maxV = AE_MAX32(maxV, vC0);
            minV = AE_MIN32(minV, vC1); maxV = AE_MAX32(maxV, vC1);
            minV = AE_MIN32(minV, vC2); maxV = AE_MAX32(maxV, vC2);
            minV = AE_MIN32(minV, vC3); maxV = AE_MAX32(maxV, vC3);

            AE_S32X2F24_X(t3, py24, 3 * 8 * stride);
            AE_S32X2F24_X(t2, py24, 1 * 8 * stride);
            AE_S32X2F24_X(t1, py24, 2 * 8 * stride);
            AE_S32X2F24_XC(t0, py24, 4 * 8 * stride);

        } while (--i);

        maxV = AE_MAX32(maxV, AE_NEG32S(minV));
        maxV = AE_MAX32(maxV, AE_SEL32_LH(maxV, maxV));
        exp = AE_NSAZ32_L(maxV);


        s = 3 - exp + 8;  //Scaling for next stages
        stride >>= 2;
        twdstep <<= 2;
    }    //while( stride > 1 )

    shift += fft24x24_stage_last_s1(x, y, N, exp);

    return shift;
}
