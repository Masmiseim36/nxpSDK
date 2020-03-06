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
  NatureDSP Signal Processing Library. FFT part
    Real-valued inverse FFT: 32-bit data, 32-bit twiddle factors
    C code optimized for HiFi4
  IntegrIT, 2006-2014
*/

/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
/* Common utility macros. */
#include "common.h"
/* Real-valued FFT descriptor structure. */
#include "fft_twiddles32x32.h"

#define _CONJ32(_x) {_x = AE_SEL32_HL(_x, AE_NEG32S(_x) ); }
#define SWAP_PTR(_x, _y) {int32_t *tmp = _x; _x = _y ; _y = tmp; } 

/*-------------------------------------------------------------------------
  Inverse FFT on Real Data
  These functions make inverse FFT on half spectral data forming real
  data samples.
      Scaling: 
      +-------------------+----------------------------------------+
      |      Function     |           Scaling options              |
      +-------------------+----------------------------------------+
      |  ifft_real16x16   |  2 - 16-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  ifft_real32x32   |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  ifft_real32x16   |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      +-------------------+----------------------------------------+

  NOTES:
  1. Bit-reversing reordering is done here. 
  2. IFFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after
     the call.
  3. Inverse FFT function for real signal transforms the input spectrum  
     and then calls ifft_cplx() with FFT size set to N/2.
  4. 32x32,32x16,16x16  FFTs support mixed radix transforms
  5. N - FFT size

  Precision:
  32x32  32-bit input/outputs, 32-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles

  Input:
  x[(N/2+1)*2]	input spectrum. Real and imaginary data are interleaved  
                and real data goes first
  scalingOpt	scaling option (see table above)

  Output:			
  y[N]	        real output signal

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y           should not overlap
  x,y           aligned on a 8-bytes boundary
-------------------------------------------------------------------------*/
int ifft_real32x32( int32_t * y, int32_t * x, fft_handle_t h, int scalingOpt )
{
    int shift, shiftSum;
    int n, N, N4, bexp;
    fft_real32x32_descr_t *hr = (fft_real32x32_descr_t *)h;
    fft_cplx32x32_descr_t *hc = (fft_cplx32x32_descr_t *)hr->cfft_hdl;
    ae_int32x2 vA0, vA1, vB0, vB1, tw;
    ae_int32x2 * restrict p_x0,
               * restrict p_x1,
               * restrict p_y0,
               * restrict p_y1;
    ae_int32x2 * restrict ptw;

    NASSERT(scalingOpt == 2 || scalingOpt == 3);
    NASSERT(x != y);
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);

    N = 2*hc->N;
    N4 = (N+2) >> 2; /* Works for all even N */

    bexp = 0;
    if (scalingOpt == 2)
    {
        int32_t i;
        ae_int32x2 acc, tmp;
        p_x0 = (ae_int32x2 *)x;
        acc = AE_ZERO32();
        __Pragma("loop_count min=4, factor=4");
        for (n = 0; n < ((N+2)>>1); n++)   
        {
            AE_L32X2_IP(tmp, p_x0, sizeof(ae_int32x2));
            tmp = AE_ABS32S(tmp); 
            acc = AE_OR32(acc, tmp);
        }
        acc = AE_OR32(acc, AE_SEL32_LH(acc, acc));
        i = AE_MOVAD32_L(acc);
        bexp = NSA(i);
        XT_MOVEQZ(bexp, 0, i);
    }

    shiftSum = 0;
    WUR_AE_SAR(0);
 /* 
    The real - to - complex spectrum conversion 
    MATLAB code:
    % N - Size of transform
    % X - input complex vector 1 x (N/2+1 )
    % x = real(N*ifft([X, conj(wrev(X(2:N/2))] ) )- output real vector 1xN
    
    twd = exp(-2*pi*1j*(0:N4-1)/N);
    a0 = X(1:N4);
    a1 = X(N/2+1:-1:N/2-N4+2); 
    b0 = a0+conj(a1);
    b1 = (a0-conj(a1))*1j.*conj(twd);
    a0 = b0+b1;
    a1 = conj(b0-b1);
    if(mod(N,4))
        x = [a0,  wrev(a1(2:N4))]; % N/2 complex samples
    else
        x = [a0,2*conj(X(N4+1)),wrev(a1(2:N4))]; % N/2 complex samples
    end

    tmp = N/2*ifft(x); 
    x = zeros(1, N);
    x(1:2:end) = real(tmp);
    x(2:2:end) = imag(tmp);
 */
    shift = (scalingOpt == 3) ? 2 : 2 - bexp;
    ASSERT(shift>-32 && shift<32);

    p_x0 = (ae_int32x2 *)x;
    p_x1 = (ae_int32x2 *)x + N/2;
    p_y0 = (ae_int32x2 *)x;
    p_y1 = (ae_int32x2 *)x + N/2 - 1;
    ptw  = (ae_int32x2 *)hr->twd + 1;

    AE_L32X2_IP(vB0, p_x0,       sizeof(complex_fract32));
    AE_L32X2_XP(vB1, p_x1, -(int)sizeof(complex_fract32));

    vB0 = AE_SRAA32(vB0, shift);
    vB1 = AE_SRAA32(vB1, shift);
    AE_ADDANDSUB32S(vA0, vA1, vB0, vB1);

    vA0 = AE_SEL32_HH(vA0, vA1);
    AE_S32X2RNG_IP(vA0, p_y0, sizeof(complex_fract32)); 

    for (n = 1; n < N4; n++)
    {
        AE_L32X2_IP(tw, ptw, sizeof(complex_fract32));

        AE_L32X2_IP(vB0, p_x0,       sizeof(complex_fract32));
        AE_L32X2_XP(vB1, p_x1, -(int)sizeof(complex_fract32));
        vB0 = AE_SRAA32(vB0, shift);
        vB1 = AE_SRAA32(vB1, shift);

        // ADD/SUBB
        AE_ADDANDSUB32S(vA0, vA1, vB0, vB1);

        vB0 = AE_SEL32_HL(vA0, vA1);
        vB1 = AE_SEL32_HL(vA1, AE_NEG32S(vA0));

        tw = AE_MUL32JS(tw);
        vB1 = AE_MULFC32RAS(vB1, tw);
        
        vA0 = AE_SUBADD32S(vB0, vB1);
        vA1 = AE_ADDSUB32S(vB1, vB0);

        AE_S32X2RNG_IP(vA0, p_y0,       sizeof(complex_fract32));
        AE_S32X2RNG_XP(vA1, p_y1, -(int)sizeof(complex_fract32));
    }
    /* When N is a multiple of 4 */
    if (0==(N&3))
    {
        /* 2*conj(x(N/4+1)) */
        vB0 = AE_L32X2_I(p_x0, 0);
        vB0 = AE_SRAA32(vB0, shift-1);
        _CONJ32(vB0); 
        AE_S32X2RNG_I(vB0, p_y0, 0);
    }
    shiftSum += shift;

    /*
     * half-sized complex-valued inverse FFT.
     */
    {
        int32_t *pdest = y; 
        const int  N = hc->N; 
        const int *tw_step = hc->tw_step;
        const cint32ptr_t *tw_tab = hc->twd;
        const fft_cplx32x32_stage_t *stg_fn = (scalingOpt == 2) ? hc->stages_s2 : hc->stages_s3;
        int v = 1;

        bexp = 0;
        if (scalingOpt==2)
        {
            AE_CALCRNG3();
            bexp = 3 - RUR_AE_SAR();
        }

        n = 0;
        while ( stg_fn[n+1] )
        {
            shiftSum += stg_fn[n](tw_tab[n], x, y, N, &v, tw_step[n], &bexp); 
            SWAP_PTR(x, y); 
            n++; 
        }
        if (y != pdest)
        {  
            /* Execute the last stage inplace */
            y = x;
        }
        /* Last stage */
        shiftSum += stg_fn[n](tw_tab[n], x, y, N, &v, tw_step[n], &bexp);
    }

    return shiftSum;
} /* ifft_real32x32() */
