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
#include "NatureDSP_Signal_vector.h"
/* Common utility and macros declarations. */
#include "common.h"

/*
The real - to - complex spectrum conversion
MATLAB code:
twd = exp(-2*pi*1j*(0:N/4-1)/N);
a0 = x(1:N/4);
a1 = wrev(x(N/4+2:N/2+1));
b0 = a0+conj(a1);
b1 = (a0-conj(a1))*1j.*conj(twd);
a0 = b0+b1;
a1 = conj(b0-b1);
x = [a0,2*conj(x(N/4+1)),wrev(a1(2:N/4))]; % N/2 complex samples
*/
static int iSpectrConv(complex_fract32 *x, int N, const complex_fract32 *twiddle_table, int twiddle_stride, int scalingOpt, int bexp)
{
    ae_int32x2 vA0, vA1, vB0, vB1, tw;
    ae_int32x2 * restrict p_x0,
               * restrict p_x1,
               * restrict ptw = (ae_int32x2*)(twiddle_table + twiddle_stride),
               * restrict p_y0,
               * restrict p_y1;
    int n;
    int shift = /*(scalingOpt == 3) ? 2 : */2 - bexp;
    ASSERT(shift>-32 && shift<32);
    WUR_AE_SAR(shift);

    p_x0 = (ae_int32x2 *)(x);
    p_x1 = (ae_int32x2 *)(x + N / 2);
    p_y0 = (ae_int32x2 *)(x);
    p_y1 = (ae_int32x2 *)(x + N / 2 - 1);

    AE_L32X2_IP(vB0, p_x0, 8);
    AE_L32X2_XP(vB1, p_x1, -8);

    vB0 = AE_SRAA32(vB0, shift);
    vB1 = AE_SRAA32(vB1, shift);
    vA0 = AE_ADD32S(vB0, vB1);
    vA1 = AE_SUB32S(vB0, vB1);

    vA0 = AE_SEL32_HH(vA0, vA1);
    AE_S32X2_IP(vA0, p_y0, sizeof(complex_fract32));

    __Pragma("loop_count min=2");
    for (n = 1; n < N / 4; n++)
    {
        AE_L32X2_IP(vB0, p_x0, sizeof(complex_fract32));
        AE_L32X2_XP(vB1, p_x1, -(int)sizeof(complex_fract32));
        AE_L32X2_XP(tw, ptw, twiddle_stride*sizeof(complex_fract32));

        vB0 = AE_SRAA32(vB0, shift);
        vB1 = AE_SRAA32(vB1, shift);
        vA0 = AE_ADDSUB32S(vB1, vB0);
        vA1 = AE_SUBADD32S(vB0, vB1);

        vB0 = vA0;
        vB1 = vA1;

        tw = AE_SEL32_LH(tw, tw);
        vB1 = AE_MULFC32RAS(vB1, tw);

        vA0 = AE_ADDSUB32S(vB1, vB0);
        vA1 = AE_SUBADD32S(vB0, vB1);

        AE_S32X2_IP(vA0, p_y0, sizeof(complex_fract32));
        AE_S32X2_XP(vA1, p_y1, -(int)sizeof(complex_fract32));
    }

    /* 2*conj(x(N/4+1)) */
    AE_L32X2_IP(vB0, p_x0, 8);
    vB0 = AE_SRAA32(vB0, shift - 1);
    vB0 = AE_SEL32_HL(vB0, AE_NEG32S(vB0));
    AE_S32X2_IP(vB0, p_y0, sizeof(complex_fract32));

    return shift;
} /* iSpectrConv */

/*-------------------------------------------------------------------------
  Inverse FFT on Real Data with Optimized Memory Usage
  These functions make inverse FFT on real data from half of spectrum with
  optimized memory usage.
  Scaling: 
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      | ifft_real16x16_ie     |  2 - 16-bit dynamic scaling          |
      | ifft_real32x16_ie     |  2 - 32-bit dynamic scaling          |
      |                       |  3 - fixed scaling before each stage |
      | ifft_real32x32_ie     |  2 - 32-bit dynamic scaling          |
      |                       |  3 - fixed scaling before each stage |
      +-----------------------+--------------------------------------+
  NOTES:
  1. Bit-reversing reordering is done here.
  2. INPUT DATA MAY APPEAR DAMAGED after the call.
  3. FFT functions may use input and output buffers for temporal storage
     of intermediate 32-bit data, so FFT functions with 24-bit packed
     I/O (Nx3-byte data) require that the buffers are large enough to 
     keep Nx4-byte data.
  4. FFT of size N may be supplied with constant data (twiddle factors) 
     of a larger-sized FFT = N*twdstep.

  Precision:
  16x16_ie      16-bit input/outputs, 16-bit data, 16-bit twiddles
  32x16_ie      32-bit input/outputs, 32-bit data, 16-bit twiddles
  32x32_ie      32-bit input/outputs, 32-bit data, 32-bit twiddles
  f_ie          floating point

  Input:
  x                     input spectrum (positive side). Real and imaginary
                        data are interleaved and real data goes first:
  --------------+----------+-----------------+----------------
  Function      |   Size   |  Allocated Size |       type    |
  --------------+----------+-----------------+----------------
  16x16_ie      |   N/2+1  |      N/2+1      |complex_fract16|
  32x16_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  32x32_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  f_ie          |   N/2+1  |      N/2+1      | complex_float |
  --------------+----------+-----------------+----------------

  twd[2*N*twdstep*3/4]  twiddle factor table of a complex-valued FFT
                        of size N*twdstep
  N                     FFT size
  twdstep               twiddle step
  scalingOpt            scaling option (see table above), not applicable 
                        to the floating point function
  Output:
  y                     output spectrum. Real and imaginary data are 
                        interleaved and real data goes first:
  --------------+----------+-----------------+-----------
  Function      |   Size   |  Allocated Size |  type    |
  --------------+----------+-----------------+-----------
  16x16_ie      |     N    |      N          |  int16_t |
  32x16_ie      |     N    |      N          |  int32_t |
  32x32_ie      |     N    |      N          |  int32_t |
  f_ie          |      N   |      N          | float32_t|
  --------------+----------+-----------------+-----------

  Returned value: total number of right shifts occurred during scaling
  procedure

  Restrictions:
  x,y   should not overlap
  x,y   aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
int ifft_real32x32_ie(int32_t* y, complex_fract32* x, const complex_fract32* twd, int twdstep, int N, int scalingOpt)
{
    const ae_int32x2 * restrict px;
    int shift;
    int bexp; 

    NASSERT(scalingOpt==2 || scalingOpt==3); 
    NASSERT(x!=(complex_fract32*)y); 
    NASSERT_ALIGN8(x); 
    NASSERT_ALIGN8(y);
    NASSERT_ALIGN8(twd);


    if (scalingOpt == 2)
    {
        int n;
        int32_t t;
        ae_int32x2 acc, tmp;
        px = (const ae_int32x2 *)x;
        acc = AE_ZERO32();
        __Pragma("loop_count min=1");
        for (n = 0; n < ((N+2)>>1); n++)   
        {
            AE_L32X2_IP(tmp, px, sizeof(*px));
            tmp = AE_ABS32S(tmp); 
            acc = AE_OR32(acc, tmp);
        }
        acc = AE_OR32(acc, AE_SEL32_LH(acc, acc));
        t = AE_MOVAD32_L(acc);
        bexp = NSA(t);
        XT_MOVEQZ(bexp, 0, t);
    }
    else
    {
        bexp = 0;
    }

    shift  = iSpectrConv(x, N, twd, 3*twdstep, scalingOpt, bexp);
    shift += ifft_cplx32x32_ie((complex_fract32*)y, x, twd, twdstep*2, N/2, scalingOpt);
    
    return shift;  
} /* ifft_real32x32_ie() */
