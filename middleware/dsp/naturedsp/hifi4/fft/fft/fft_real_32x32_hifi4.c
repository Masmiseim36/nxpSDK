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
    Real-valued forward FFT: 32-bit data, 32-bit twiddle factors
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
/* Common utility macros. */
#include "common.h"
/* Complex-valued FFT descriptor structure and twiddle factor tables. */
#include "fft_twiddles32x32.h"

#define _CONJ32(_x) {_x = AE_SEL32_HL(_x, AE_NEG32S(_x) ); }
#define SWAP_PTR(_x, _y) {int32_t *tmp = _x; _x = _y ; _y = tmp; } 

/*-------------------------------------------------------------------------
  FFT on Real Data
  These functions make FFT on real data forming half of spectrum
      Scaling: 
      +-------------------+----------------------------------------+
      |      Function     |           Scaling options              |
      +-------------------+----------------------------------------+
      |  fft_real16x16    |  2 - 16-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  fft_real32x32    |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  fft_real32x16    |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      +-------------------+----------------------------------------+
  NOTES:
  1. Bit-reversal reordering is done here. 
  2. FFT runs in-place so INPUT DATA WILL APPEAR DAMAGED after the call.
  3. Real data FFT function calls fft_cplx() to apply complex FFT of size
     N/2 to input data and then transforms the resulting spectrum.
  4. 32x32,32x16,16x16 FFTs support mixed radix transforms 
  5. N - FFT size

  Precision:
  32x32  32-bit input/outputs, 32-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles

  Input:
  x[N]          input signal
  scalingOpt    scaling option (see table above)
  Output:
  y[(N/2+1)*2]  output spectrum (positive side)

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y           should not overlap
  x,y           aligned on a 8-bytes boundary
-------------------------------------------------------------------------*/
int fft_real32x32( int32_t * y,
                   int32_t * x,
                   fft_handle_t h, int scalingOpt )
{
    int n;
    ae_int32x2  vA0, vA1, vB0, vB1, tw;
    ae_int64   * restrict ptw;
    ae_int32x2 * restrict p_x0, 
               * restrict p_x1, 
               * restrict p_y0, 
               * restrict p_y1;
    int N4;
    int shift, shiftSum; 
    int N, bexp; 
    fft_real32x32_descr_t *hr = (fft_real32x32_descr_t *)h; 
    fft_cplx32x32_descr_t *hc = (fft_cplx32x32_descr_t *)hr->cfft_hdl;
    N = 2 * hc->N;
    N4 = (N + 2) >> 2; /* Works for all even N */

    NASSERT(scalingOpt==2 || scalingOpt==3); 
    NASSERT(x!=y); 
    NASSERT_ALIGN8(x); 
    NASSERT_ALIGN8(y);

    /*
     * half-sized complex-valued forward FFT.
     */
    {
      int32_t *pdest = y; 
      const int N = hc->N; 
      const int *tw_step = hc->tw_step;
      const cint32ptr_t *tw_tab = hc->twd; 
      const fft_cplx32x32_stage_t *stg_fn = (scalingOpt == 2) ? hc->stages_s2 : hc->stages_s3;
      int v = 1;
      bexp = 0;

      if (scalingOpt==2)
      {
        int32_t i;
        ae_int32x2 acc, tmp;
        p_x0 = (ae_int32x2 *)x;
        acc = AE_ZERO32();
        __Pragma("loop_count min=2, factor=2");
        for (n = 0; n < N; n++)   
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

      n = 0;
      shiftSum = 0;
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

/*----------------------------------------------------------------------------
Apply the in-place real-to-complex spectrum conversion

    MATLAB code:
    % x is input real sequence 1xN
    % y is output complex sequence 1xN
    y = fft(x(1:2:end) + 1j*x(2:2:end)); 
    N = length(x); 
    N4 = floor((N/2+1)/2); % N must be a multiple of 2
    twd = exp(-2*pi*1j*(0:N4-1)/N);
    a0 = y(1:N4);

    a1 = [y(1), y(N/2:-1:N/2-N4+2)];
    b0 = 1/2*(a0+conj(a1));
    b1 = 1/2*(a0-conj(a1))*-1j.*twd;
    a0 = b0+b1;
    a1 = b0-b1;
    if(mod(N,4))    
        y(1:N) = [a0, wrev(conj(a1(2:N4))), ...
        a1,    wrev(conj(a0(2:N4)))];    
    else
        y(1:N) = [a0,conj(y(N4+1)),wrev(conj(a1(2:N4))), ...
        a1,     y(N4+1) ,wrev(conj(a0(2:N4)))];
    end
*/
    shift = 0;
    if (scalingOpt == 2)
    {
        AE_CALCRNG1();
        bexp = 1 - RUR_AE_SAR();
        shift = XT_MAX(1-bexp, 0);
    }
    shiftSum += shift;

    p_x0 = (ae_int32x2 *)y;
    p_x1 = (ae_int32x2 *)y + N/2 - 1;
    p_y0 = (ae_int32x2 *)y;
    p_y1 = (ae_int32x2 *)y + N/2;
    ptw  = (ae_int64   *)hr->twd + 1;

    /*
    b0 = y[0];
    b0.s.re >>= shift;
    b0.s.im >>= shift;

    a0.s.re = L_add_ll(b0.s.re, b0.s.im);
    a0.s.im = 0;
    a1.s.re = L_sub_ll(b0.s.re, b0.s.im);
    a1.s.im = 0;

    a1 = conj_fr32c(a1); 
    y[0] = a0;
    y[N / 2] = a1;
    */
    WUR_AE_SAR(shift+1);

    AE_L32X2_IP(vB0, p_x0, sizeof(complex_fract32));
    vB0 = AE_SRAA32(vB0, shift);

    vB1 = AE_MUL32JS(vB0);  
    vB0 = AE_ADD32S(vB0, vB1);

    vA1 = AE_SEL32_HH(vB0, AE_MOVI(0));
    vA0 = AE_SEL32_LL(vB0, AE_MOVI(0));
    AE_S32X2_IP(vA0, p_y0,       sizeof(complex_fract32)); 
    AE_S32X2_XP(vA1, p_y1, -(int)sizeof(complex_fract32));

    __Pragma("loop_count min=2");
    for (n = 1; n < N4; n++)
    {
        ae_int64 t64;
        AE_L64_IP(t64, ptw, sizeof(complex_fract32));
        tw = AE_MOVINT32X2_FROMINT64(t64);

        AE_L32X2_IP(vB0, p_x0,       sizeof(complex_fract32));
        AE_L32X2_XP(vB1, p_x1, -(int)sizeof(complex_fract32));

        // ADD/SUBB
        AE_ADDANDSUBRNG32(vA0, vA1, vB0, vB1);

        vB1 = AE_SEL32_HL(AE_NEG32S(vA1), vA0);
        vB0 = AE_SEL32_HL(vA0, vA1);

        vB1 = AE_MULFC32RAS(vB1, tw);

        vA0 = AE_SUBADD32S(vB0, vB1);
        vA1 = AE_ADDSUB32S(vB1, vB0);

        AE_S32X2_IP(vA0, p_y0,       sizeof(complex_fract32)); 
        AE_S32X2_XP(vA1, p_y1, -(int)sizeof(complex_fract32));
    }
    /* When N is not multiple of 4 */
    if (N & 3)
    {
        return shiftSum; 
    }

    vA0 = AE_L32X2_I(p_x0, 0);
    vA0 = AE_SRAA32(vA0, shift);
    _CONJ32(vA0);  /* a0 = conj(a0) */
    AE_S32X2_I(vA0, p_y0, 0);

    return shiftSum;  
} /* fft_real32x32() */
