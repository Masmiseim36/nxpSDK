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
    Complex-valued inverse FFT: 16-bit data, 16-bit twiddle factors
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
/* Common utility macros. */
#include "common.h"
/* Twiddle factor tables and FFT descriptor structure. */
#include "fft_x16_common.h"
#include "fft_16x16_stages.h"

#define SWAP_PTR(_x, _y) {int16_t *tmp = _x; _x = _y ; _y = tmp; } 

/*-------------------------------------------------------------------------
  Inverse FFT on Complex Data
  These functions make inverse FFT on complex data.
      Scaling: 
      +-------------------+----------------------------------------+
      |      Function     |           Scaling options              |
      +-------------------+----------------------------------------+
      |  ifft_cplx16x16   |  2 - 16-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  ifft_cplx32x32   |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  ifft_cplx32x16   |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      +-------------------+----------------------------------------+
  NOTES:
  1. Bit-reversing permutation is done here. 
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call.
  3. 32x32,32x16,16x16 FFTs support mixed radix transforms.
  4. N - FFT size.

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles

  Input:
  x[2*N]     complex input spectrum. Real and imaginary data are interleaved 
             and real data goes first
  scalingOpt scaling option (see table above)
  Output:
  y[2*N]     complex output signal. Real and imaginary data are interleaved 
             and real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y        should not overlap
  x,y        aligned on a 8-bytes boundary
-------------------------------------------------------------------------*/
int ifft_cplx16x16( int16_t* y, int16_t* x, fft_handle_t h, int scalingOption )
{
    const ae_int16x4 * restrict px;
    const fft_cplx_x16_descr_t* pdescr = (const fft_cplx_x16_descr_t*)h;
    int bexp = 0;
    int v = 1;
    int s = 0;
    int shift = 0;
    int stg_type;

    int16_t *pdest = y; 
    const int  N = pdescr->N; 
    const int *tw_step = pdescr->tw_step;
    const cint16ptr_t *tw_tab = pdescr->twd;
    const eFft_stage_type *stg_typeTbl = (scalingOption == 2) ? pdescr->stages_16x16_s2 : pdescr->stages_16x16_s3;
    const fft_cplx16x16_stage_t*stg_Tbl = (scalingOption == 2) ? fft16x16_stages_scl2_tbl : fft16x16_stages_scl3_tbl;
    fft_cplx16x16_stage_t stg_fn;

    NASSERT_ALIGN8(x); 
    NASSERT_ALIGN8(y);
    NASSERT(x!=y);
    NASSERT(scalingOption == 2 || scalingOption == 3); 

    if (scalingOption==2)
    {
      int i;
      ae_int16x4 acc, tmp;
      px = (ae_int16x4*)x;
      acc = AE_ZERO16();
      /* 1 cycles per pipeline stage in steady state with unroll=1*/
      __Pragma("loop_count min=4 factor=4");
      for (i = 0; i < (N >> 1); i++)   
      {
        AE_L16X4_IP(tmp, px, sizeof(*px));
        tmp = AE_ABS16S(tmp); 
        acc = AE_OR16(acc, tmp);
      }
      acc = AE_OR16(acc, AE_SEL16_5432(acc, acc));
      acc = AE_OR16(acc, AE_SHORTSWAP(acc));

      i = AE_MOVAD16_0(acc);
      bexp = NSA(i) - 16;
      XT_MOVEQZ(bexp, 0, i);
    }

    while ( tw_tab[s] != NULL )
    {
      stg_type = (int)stg_typeTbl[s];
      NASSERT(stg_type < NUM_FFT_STAGE_TYPES);
      stg_fn = stg_Tbl[stg_type];
      NASSERT(stg_fn != NULL);
      shift += stg_fn(tw_tab[s], x, y, N, &v, tw_step[s], &bexp); 
      SWAP_PTR(x, y); 
      s++; 
    }

    if (y != pdest)
    {  
        /* Execute the last stage inplace */
        y = x;
    }

    /* Last stage */
    stg_type = (int)stg_typeTbl[s];
    NASSERT(stg_type < NUM_FFT_STAGE_TYPES);
    stg_fn = stg_Tbl[stg_type];
    NASSERT(stg_fn != NULL);
    shift += stg_fn(tw_tab[s], x, y, N, &v, tw_step[s], &bexp);

    return shift;
} /* ifft_cplx16x16() */
