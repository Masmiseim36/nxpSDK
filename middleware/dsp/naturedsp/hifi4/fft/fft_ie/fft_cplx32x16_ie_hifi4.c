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
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
/* Common utility and macros declarations. */
#include "common.h"

static int fft_stage_last_ie( int32_t *x, int32_t *y, int N );

/*-------------------------------------------------------------------------
  FFT on Complex Data with Optimized Memory Usage
  These functions make FFT on complex data with optimized memory usage.
  Scaling: 
      +-------------------+----------------------------------------+
      |      Function     |           Scaling options              |
      +-------------------+----------------------------------------+
      |  fft_cplx16x16_ie |  2 - 16-bit dynamic scaling            | 
      |  fft_cplx32x16_ie |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  fft_cplx32x32_ie |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      +-------------------+----------------------------------------+
  NOTES:
  1. Bit-reversing reordering is done here.
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call.
  3. FFT of size N may be supplied with constant data
     (twiddle factors) of a larger-sized FFT = N*twdstep.
  4. Stereo FFTs accept inputs/form outputs in the interleaved order:
     left complex sample, right complex sample

  Precision: 
  16x16_ie      16-bit input/outputs, 16-bit twiddles
  32x16_ie      32-bit input/outputs, 16-bit twiddles
  32x32_ie      32-bit input/outputs, 32-bit twiddles
  f_ie          floating point
 
  Input:
  S                     1 for ordinary (single channel) FFT, 2 - for stereo
                        input/outputs
  x[N*S]                complex input signal. Real and imaginary data 
                        are interleaved and real data goes first
  twd[N*twdstep*3/4]    twiddle factor table of a complex-valued FFT of 
                        size N*twdstep
  N                     FFT size
  twdstep               twiddle step 
  scalingOpt            scaling option (see table above), not applicable
                        to the floating point function 
  Output:
  y[N*S]                output spectrum. Real and imaginary data are 
                        interleaved and real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure. Floating point function always return 0.

  Restrictions:
  x,y   should not overlap
  x,y   aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
int fft_cplx32x16_ie(complex_fract32* y,complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt)
{
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt==3 || scalingOpt==2);
    NASSERT(N>=16 && (N&(N-1))==0);

    int offset_inc, s;
    int shift = 0;
    int tw_step = twdstep;
    int stride = N>>2;

    ae_int32x2 * restrict px;
    ae_int32x2 * restrict py;
    ae_int32x2 * restrict px1;
    ae_int32x2 * restrict py1;

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

    /*
     * autoscaling case
     */
    if (scalingOpt == 2)
    {
        const int min_shift = 3;
        int32_t i;
        ae_int32x2 acc, tmp;
        px = (ae_int32x2 *)x;
        acc = AE_ZERO32();
        __Pragma("loop_count min=4 factor=4");
        for (i = 0; i < N; i++)   
        {
            AE_L32X2_IP(tmp, px, sizeof(*px));
            tmp = AE_ABS32S(tmp); 
            acc = AE_OR32(acc, tmp);
        }
        acc = AE_OR32(acc, AE_SEL32_LH(acc, acc));
        i = AE_MOVAD32_L(acc);
        bexp = NSA(i);
        XT_MOVEQZ(bexp, 0, i);

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

            px = (ae_int32x2*)x;
            py = (ae_int32x2*)x;

            i = N>>2;
            do
            {
                vA3 = AE_L32X2_X(px,  3*stride*sizeof(ae_int32x2));
                vA2 = AE_L32X2_X(px,  2*stride*sizeof(ae_int32x2));
                vA1 = AE_L32X2_X(px,  1*stride*sizeof(ae_int32x2));
                AE_L32X2_IP(vA0, px,  sizeof(ae_int32x2));

                vA0 = AE_MULP32X2(vA0, scl);
                vA1 = AE_MULP32X2(vA1, scl);
                vA2 = AE_MULP32X2(vA2, scl);
                vA3 = AE_MULP32X2(vA3, scl);
                AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                vB3 = AE_MUL32JS(vB3);
                AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                AE_L32_XP( t1, p16tw1, offset_inc );
                AE_L32_XP( t2, p16tw2, offset_inc );
                AE_L32_XP( t3, p16tw3, offset_inc );

                t1_f16x4 = AE_MOVF16X4_FROMINT32X2(t1);
                t2_f16x4 = AE_MOVF16X4_FROMINT32X2(t2);
                t3_f16x4 = AE_MOVF16X4_FROMINT32X2(t3);
                vC1 = AE_MULFC32X16RAS_L(vC1, t1_f16x4);
                vC2 = AE_MULFC32X16RAS_L(vC2, t2_f16x4);
                vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                AE_S32X2RNG_X (vC3, py,  3*stride*sizeof(ae_int32x2));
                AE_S32X2RNG_X (vC2, py,  1*stride*sizeof(ae_int32x2));
                AE_S32X2RNG_X (vC1, py,  2*stride*sizeof(ae_int32x2));
                AE_S32X2RNG_IP(vC0, py,  sizeof(ae_int32x2));
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

                px = (ae_int32x2*)x+j*2;
                py = (ae_int32x2*)x+j*2;
                px1 = px+1;
                py1 = py+1;

                __Pragma("loop_count min=2");
                for (i = 0; i < (1<<log2n); i++)
                {
                    /* butterfly 0 */
                    AE_L32X2_XP(vA0, px, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA1, px, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA2, px, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA3, px, stride*sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_H(vC1, t1_f16x4);
                    vC2 = AE_MULFC32X16RAS_L(vC2, t1_f16x4);
                    vC3 = AE_MULFC32X16RAS_H(vC3, t2_f16x4);

                    AE_S32X2RNG_XP(vC0, py, stride*sizeof(ae_int32x2));
                    AE_S32X2RNG_XP(vC2, py, stride*sizeof(ae_int32x2));
                    AE_S32X2RNG_XP(vC1, py, stride*sizeof(ae_int32x2));
                    AE_S32X2RNG_XP(vC3, py, stride*sizeof(ae_int32x2));

                    /* butterfly 1 */
                    AE_L32X2_XP(vA0, px1, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA1, px1, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA2, px1, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA3, px1, stride*sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_L(vC1, t2_f16x4);
                    vC2 = AE_MULFC32X16RAS_H(vC2, t3_f16x4);
                    vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                    AE_S32X2RNG_XP(vC0, py1, stride*sizeof(ae_int32x2));
                    AE_S32X2RNG_XP(vC2, py1, stride*sizeof(ae_int32x2));
                    AE_S32X2RNG_XP(vC1, py1, stride*sizeof(ae_int32x2));
                    AE_S32X2RNG_XP(vC3, py1, stride*sizeof(ae_int32x2));
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

            px = (ae_int32x2*)x;
            py = (ae_int32x2*)x;

            i = N>>2;
            do
            {
                vA3 = AE_L32X2_X(px, 3*stride*sizeof(ae_int32x2));
                vA2 = AE_L32X2_X(px, 2*stride*sizeof(ae_int32x2));
                vA1 = AE_L32X2_X(px, 1*stride*sizeof(ae_int32x2));
                AE_L32X2_IP(vA0, px, sizeof(ae_int32x2));

                AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                vB3 = AE_MUL32JS(vB3);
                AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                AE_L32_XP( t1, p16tw1, offset_inc );
                AE_L32_XP( t2, p16tw2, offset_inc );
                AE_L32_XP( t3, p16tw3, offset_inc );
                t1_f16x4 = AE_MOVF16X4_FROMINT32X2(t1);
                t2_f16x4 = AE_MOVF16X4_FROMINT32X2(t2);
                t3_f16x4 = AE_MOVF16X4_FROMINT32X2(t3);

                vC1 = AE_MULFC32X16RAS_L(vC1, t1_f16x4);
                vC2 = AE_MULFC32X16RAS_L(vC2, t2_f16x4);
                vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                AE_S32X2_X (vC3, py, 3*stride*sizeof(ae_int32x2));
                AE_S32X2_X (vC2, py, 1*stride*sizeof(ae_int32x2));
                AE_S32X2_X (vC1, py, 2*stride*sizeof(ae_int32x2));
                AE_S32X2_IP(vC0, py, sizeof(ae_int32x2));

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

                px = (ae_int32x2*)x+j*2;
                py = (ae_int32x2*)x+j*2;
                px1 = px+1;
                py1 = py+1;

                __Pragma("loop_count min=2");
                for (i = 0; i < (1<<log2n); i++)
                {
                    /* butterfly 0 */
                    AE_L32X2_XP(vA0, px, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA1, px, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA2, px, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA3, px, stride*sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_H(vC1, t1_f16x4);
                    vC2 = AE_MULFC32X16RAS_L(vC2, t1_f16x4);
                    vC3 = AE_MULFC32X16RAS_H(vC3, t2_f16x4);

                    AE_S32X2_XP(vC0, py, stride*sizeof(ae_int32x2));
                    AE_S32X2_XP(vC2, py, stride*sizeof(ae_int32x2));
                    AE_S32X2_XP(vC1, py, stride*sizeof(ae_int32x2));
                    AE_S32X2_XP(vC3, py, stride*sizeof(ae_int32x2));

                    /* butterfly 1 */
                    AE_L32X2_XP(vA0, px1, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA1, px1, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA2, px1, stride*sizeof(ae_int32x2));
                    AE_L32X2_XP(vA3, px1, stride*sizeof(ae_int32x2));

                    AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
                    AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
                    vB3 = AE_MUL32JS(vB3);
                    AE_ADDANDSUB32S(vC0, vC2, vB0, vB1);
                    AE_ADDANDSUB32S(vC3, vC1, vB2, vB3);

                    vC1 = AE_MULFC32X16RAS_L(vC1, t2_f16x4);
                    vC2 = AE_MULFC32X16RAS_H(vC2, t3_f16x4);
                    vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4);

                    AE_S32X2_XP(vC0, py1, stride*sizeof(ae_int32x2));
                    AE_S32X2_XP(vC2, py1, stride*sizeof(ae_int32x2));
                    AE_S32X2_XP(vC1, py1, stride*sizeof(ae_int32x2));
                    AE_S32X2_XP(vC3, py1, stride*sizeof(ae_int32x2));
                }
            }

            stride>>=2;
            tw_step<<=2;
        }
    }

    shift += fft_stage_last_ie((int32_t*)x, (int32_t*)y, N);

    return shift;
} /* fft_cplx32x16_ie() */

/* Last stage of computing FFT */
int fft_stage_last_ie( int32_t *x,
                       int32_t *y,
                       int N)
{
    int32_t i,i0,i1,ai;
    ae_int32x2 * restrict p_y0 = (ae_int32x2 *)(y);
    ae_int32x2 * restrict p_y1 = (p_y0 + (N >> 2));
    ae_int32x2 * restrict p_y2 = (p_y1 + (N >> 2));
    ae_int32x2 * restrict p_y3 = (p_y2 + (N >> 2));
    ae_int32x2 * restrict p_x0 = (ae_int32x2 *)(x);
    int shift;

    i = NSA(N)+1;
    ai=((int32_t)0x1)<<i;
    i0=0;


    if ( (i&1)==0 )
    {
        shift = 1;    //Select scaling
        WUR_AE_SAR(shift);
        //--------------------------------------------------------------------------
        // last stage is RADIX2 !!!
        //--------------------------------------------------------------------------
        i = N>>3;
        do
        {
            ae_int32x2 vA1, vA2, vA3, vA0;
            ae_int32x2 vB1, vB2, vB3, vB0;
            i1 = AE_ADDBRBA32(i0, ai);

            // FFT_BUTTERFLY_R2(i0, shift);
            AE_L32X2_IP(vA0,  p_x0, 8);
            AE_L32X2_IP(vA1,  p_x0, 8);
            AE_L32X2_IP(vA2,  p_x0, 8);
            AE_L32X2_IP(vA3,  p_x0, 8);
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA1);
            AE_ADDANDSUBRNG32(vB1, vB3, vA2, vA3);
            AE_S32X2_X(vB0, p_y0, i0);
            AE_S32X2_X(vB1, p_y1, i0);
            AE_S32X2_X(vB2, p_y2, i0);
            AE_S32X2_X(vB3, p_y3, i0);

            //FFT_BUTTERFLY_R2(i1, shift);
            AE_L32X2_IP(vA0,  p_x0, 8);
            AE_L32X2_IP(vA1,  p_x0, 8);
            AE_L32X2_IP(vA2,  p_x0, 8);
            AE_L32X2_IP(vA3,  p_x0, 8);
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA1);
            AE_ADDANDSUBRNG32(vB1, vB3, vA2, vA3);
            AE_S32X2_X(vB0, p_y0, i1);
            AE_S32X2_X(vB1, p_y1, i1);
            AE_S32X2_X(vB2, p_y2, i1);
            AE_S32X2_X(vB3, p_y3, i1);

            i0 = AE_ADDBRBA32(i1, ai);
        }while(--i);
    }
    else
    {
        shift = 2;    //Select scaling
        WUR_AE_SAR(shift);
        //--------------------------------------------------------------------------
        // last stage is RADIX4 !!!
        //--------------------------------------------------------------------------
        i = N>>3;
        do
        {
            ae_int32x2 vA1, vA2, vA3, vA0;
            ae_int32x2 vB1, vB2, vB3, vB0;

            //     FFT_BUTTERFLY_R4(i0, shift);
            AE_L32X2_IP(vA0,  p_x0, 8);
            AE_L32X2_IP(vA1,  p_x0, 8);
            AE_L32X2_IP(vA2,  p_x0, 8);
            AE_L32X2_IP(vA3,  p_x0, 8);
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
            AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
            vB3 = AE_MUL32JS(vB3);
            AE_ADDANDSUB32S(vA0, vA2, vB0, vB1);
            AE_ADDANDSUB32S(vA3, vA1, vB2, vB3);
            AE_S32X2_X(vA0, p_y0, i0);
            AE_S32X2_X(vA1, p_y1, i0);
            AE_S32X2_X(vA2, p_y2, i0);
            AE_S32X2_X(vA3, p_y3, i0);
            i0 = AE_ADDBRBA32(i0, ai);

            //     FFT_BUTTERFLY_R4(i0, shift);
            AE_L32X2_IP(vA0,  p_x0, 8);
            AE_L32X2_IP(vA1,  p_x0, 8);
            AE_L32X2_IP(vA2,  p_x0, 8);
            AE_L32X2_IP(vA3,  p_x0, 8);
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
            AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
            vB3 = AE_MUL32JS(vB3);
            AE_ADDANDSUB32S(vA0, vA2, vB0, vB1);
            AE_ADDANDSUB32S(vA3, vA1, vB2, vB3);
            AE_S32X2_X(vA0, p_y0, i0);
            AE_S32X2_X(vA1, p_y1, i0);
            AE_S32X2_X(vA2, p_y2, i0);
            AE_S32X2_X(vA3, p_y3, i0);
            i0 = AE_ADDBRBA32(i0, ai);
        }while(--i);
    }
    return shift;
} /* fft_stage_last_ie */
