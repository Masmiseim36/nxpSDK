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
int stereo_fft_cplx32x16_ie_inner(complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);

/*-------------------------------------------------------------------------
  Inverse FFT on Complex Data with Optimized Memory Usage
  These functions make inverse FFT on complex data with optimized 
  memory usage.
  Scaling: 
      +-------------------+----------------------------------------+
      |      Function     |           Scaling options              |
      +-------------------+----------------------------------------+
      | ifft_cplx16x16_ie |  2 - 16-bit dynamic scaling            | 
      | ifft_cplx32x16_ie |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      | ifft_cplx32x32_ie |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      +-------------------+----------------------------------------+
  NOTES:
  1. Bit-reversing reordering is done here.
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call
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
  S                   1 for ordinary (single channel) FFT, 2 - for stereo
                      input/outputs
  x[N*S]              complex input signal. Real and imaginary data 
                      are interleaved and real data goes first

  twd[N*twdstep*3/4]  twiddle factor table of a complex-valued FFT of 
                      size N*twdstep
  N                   FFT size
  twdstep             twiddle step 
  scalingOpt          scaling option (see table above)

  Output:
  y[N*S]              output spectrum. Real and imaginary data are 
                      interleaved and real data goes first

  Returned value:     total number of right shifts occurred during 
                      scaling procedure

  Restrictions:
  x,y   should not overlap
  x,y   aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
int stereo_ifft_cplx32x16_ie(complex_fract32* y,complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt)
{
    int shift;
    int32_t i,i0,negi0,ai;
    ae_int32x2 * restrict p_y0l = (ae_int32x2 *)(y);
    ae_int32x2 * restrict p_y3l = p_y0l + (N>>1);
    ae_int32x2 * restrict p_y2l = p_y3l + (N>>1);
    ae_int32x2 * restrict p_y1l = p_y2l + (N>>1);
    ae_int32x2 * restrict p_y0r = p_y0l + 1;
    ae_int32x2 * restrict p_y3r = p_y3l + 1;
    ae_int32x2 * restrict p_y2r = p_y2l + 1;
    ae_int32x2 * restrict p_y1r = p_y1l + 1;
    ae_int32x2 * restrict p_xl = (ae_int32x2 *)(x);
    ae_int32x2 * restrict p_xr = p_xl+1;
    ae_int32x2 vA0, vA1, vA2, vA3;
    ae_int32x2 vB0, vB1, vB2, vB3;

    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt==3 || scalingOpt==2);
    NASSERT(N>=8 && (N&(N-1))==0);

    /* Perform all stages of FFT except last one */
    shift = stereo_fft_cplx32x16_ie_inner(x, twd, twdstep, N, scalingOpt);

    /*
     * Last stage:
     * bit-reversal permutation is combined with reverse permutation to get Inverse FFT
     */
    i = NSA(N);
    ai=((int32_t)0x1)<<i;
    i0=0;

    if ( (i&1)!=0 )
    {
        shift += 1;// Set scaling
        WUR_AE_SAR(1);
        //--------------------------------------------------------------------------
        // last stage is RADIX2 !!!
        //--------------------------------------------------------------------------
        /* first butterfly */
        {
            /* left channel */
            AE_L32X2_IP(vA0, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA1, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA2, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA3, p_xl, 2*sizeof(ae_int32x2));
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA1);
            AE_ADDANDSUBRNG32(vB1, vB3, vA2, vA3);
            AE_S32X2_I(vB0, p_y0l, 0);
            AE_S32X2_I(vB3, p_y3l, 0);
            AE_S32X2_I(vB2, p_y2l, 0);
            AE_S32X2_I(vB1, p_y1l, 0);

            /* right channel */
            AE_L32X2_IP(vA0, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA1, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA2, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA3, p_xr, 2*sizeof(ae_int32x2));
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA1);
            AE_ADDANDSUBRNG32(vB1, vB3, vA2, vA3);
            AE_S32X2_I(vB0, p_y0r, 0);
            AE_S32X2_I(vB3, p_y3r, 0);
            AE_S32X2_I(vB2, p_y2r, 0);
            AE_S32X2_I(vB1, p_y1r, 0);
        }
        p_y0l = p_y1l + (N>>1);
        p_y0r = p_y0l + 1;
        i = (N>>2)-1;
        do
        {
            i0 = AE_ADDBRBA32(i0, ai);
            negi0 = -i0;

            /* left channel */
            AE_L32X2_IP(vA0, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA1, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA2, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA3, p_xl, 2*sizeof(ae_int32x2));
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA1);
            AE_ADDANDSUBRNG32(vB1, vB3, vA2, vA3);
            AE_S32X2_X(vB3, p_y3l, negi0);
            AE_S32X2_X(vB2, p_y2l, negi0);
            AE_S32X2_X(vB1, p_y1l, negi0);
            AE_S32X2_X(vB0, p_y0l, negi0);

            /* right channel */
            AE_L32X2_IP(vA0, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA1, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA2, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA3, p_xr, 2*sizeof(ae_int32x2));
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA1);
            AE_ADDANDSUBRNG32(vB1, vB3, vA2, vA3);
            AE_S32X2_X(vB3, p_y3r, negi0);
            AE_S32X2_X(vB2, p_y2r, negi0);
            AE_S32X2_X(vB1, p_y1r, negi0);
            AE_S32X2_X(vB0, p_y0r, negi0);

        }while(--i);
    }
    else
    {
        shift += 2;// Set scaling
        WUR_AE_SAR(2);
        //--------------------------------------------------------------------------
        // last stage is RADIX4 !!!
        //--------------------------------------------------------------------------
        /* first butterfly */
        {
            /* left channel */
            AE_L32X2_IP(vA0, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA1, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA2, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA3, p_xl, 2*sizeof(ae_int32x2));
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
            AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
            vB3 = AE_MUL32JS(vB3);
            AE_ADDANDSUB32S(vA0, vA2, vB0, vB1);
            AE_ADDANDSUB32S(vA3, vA1, vB2, vB3);
            AE_S32X2_I(vA3, p_y3l, 0);
            AE_S32X2_I(vA2, p_y2l, 0);
            AE_S32X2_I(vA1, p_y1l, 0);
            AE_S32X2_I(vA0, p_y0l, 0);

            /* right channel */
            AE_L32X2_IP(vA0, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA1, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA2, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA3, p_xr, 2*sizeof(ae_int32x2));
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
            AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
            vB3 = AE_MUL32JS(vB3);
            AE_ADDANDSUB32S(vA0, vA2, vB0, vB1);
            AE_ADDANDSUB32S(vA3, vA1, vB2, vB3);
            AE_S32X2_I(vA3, p_y3r, 0);
            AE_S32X2_I(vA2, p_y2r, 0);
            AE_S32X2_I(vA1, p_y1r, 0);
            AE_S32X2_I(vA0, p_y0r, 0);

        }
        p_y0l = p_y1l + (N>>1);
        p_y0r = p_y0l + 1;
        i = (N>>2)-1;
        do
        {
            i0 = AE_ADDBRBA32(i0, ai);
            negi0 = -i0;

            /* left channel */
            AE_L32X2_IP(vA0, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA1, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA2, p_xl, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA3, p_xl, 2*sizeof(ae_int32x2));
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
            AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
            vB3 = AE_MUL32JS(vB3);
            AE_ADDANDSUB32S(vA0, vA2, vB0, vB1);
            AE_ADDANDSUB32S(vA3, vA1, vB2, vB3);
            AE_S32X2_X(vA3, p_y3l, negi0);
            AE_S32X2_X(vA2, p_y2l, negi0);
            AE_S32X2_X(vA1, p_y1l, negi0);
            AE_S32X2_X(vA0, p_y0l, negi0);

            /* right channel */
            AE_L32X2_IP(vA0, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA1, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA2, p_xr, 2*sizeof(ae_int32x2));
            AE_L32X2_IP(vA3, p_xr, 2*sizeof(ae_int32x2));
            AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
            AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
            vB3 = AE_MUL32JS(vB3);
            AE_ADDANDSUB32S(vA0, vA2, vB0, vB1);
            AE_ADDANDSUB32S(vA3, vA1, vB2, vB3);
            AE_S32X2_X(vA3, p_y3r, negi0);
            AE_S32X2_X(vA2, p_y2r, negi0);
            AE_S32X2_X(vA1, p_y1r, negi0);
            AE_S32X2_X(vA0, p_y0r, negi0);
        }while(--i);
    }

    return shift;
} /* stereo_ifft_cplx32x16_ie() */
