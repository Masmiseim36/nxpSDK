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
/*===========================================================================
Fast Fourier Transforms:
fft_cplx             FFT on Complex Data
fft_real             FFT on Real Data
ifft_cplx            IFFT on Complex Data
ifft_real            Inverse FFT Forming Real Data
fft_cplx<prec>_ie    FFT on Complex Data with optimized memory usage
fft_real<prec>_ie    FFT on Real Data with optimized memory usage
ifft_cplx<prec>_ie   IFFT on Complex Data with optimized memory usage
ifft_real<prec>_ie   Inverse FFT Forming Real Data with optimized memory usage
dct                  Discrete Cosine Transform

There are limited combinations of precision and scaling options available:
----------------+---------------------------------------------------------------
FFT/IFFT        | Scaling options                        | Restrictions on the
                |                                        | input dynamic range
----------------+---------------------------------------------------------------
cplx24x24       | 0 ÿ no scaling                         | input signal < 2^23/(2*N),
                |                                        | N-fft-size
real24x24       | 1 ÿ 24-bit scaling                     |        none
                | 2 ÿ 32-bit scaling on the first stage  |        none
                | and 24-bit scaling later               |        none
                | 3 ÿ fixed scaling before each stage    |        none
------------------------------------------------------------------------------------
cplx32x16       | 3 ÿ fixed scaling before each stage    |        none
cplx16x16       | 3 ÿ fixed scaling before each stage    |        none
cplx32x16_ie    | 3 ÿ fixed scaling before each stage    |        none
cplx24x24_ie    | 3 ÿ fixed scaling before each stage    |        none
real32x16       | 3 ÿ fixed scaling before each stage    |        none
real16x16       | 3 ÿ fixed scaling before each stage    |        none
real32x16_ie    | 3 ÿ fixed scaling before each stage    |        none
real24x24_ie    | 3 ÿ fixed scaling before each stage    |        none
real32x16_ie_24p| 3 ÿ fixed scaling before each stage    |        none
----------------+---------------------------------------------------------------
real24x24_ie_24p| 1 ÿ 24-bit scaling                     |        none
----------------+---------------------------------------------------------------
DCT:            |
----------------+---------------------------------------------------------------
24x24	          | 0 ÿ no scaling                         |        none
32x16	          | 0 ÿ no scaling                         |        none
----------------+---------------------------------------------------------------

===========================================================================*/
#include "NatureDSP_Signal.h"
#include "common.h"

extern void fft_revorder_ie(int32_t *x, int N) ; 



/*-------------------------------------------------------------------------
  These functions make inverse FFT on complex data with optimized memory usage.
  Scaling  : Fixed data scaling at each stage
  NOTES:
  1. Bit-reversing reordering is done here.
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call
  3. Forward/inverse FFT of size N may be supplied with constant data
     (twiddle factors) of a larger-sized FFT = N*twdstep.

  Precision: 
  24x24_ie              24-bit input/outputs, 24-bit twiddles
  32x16_ie              32-bit input/outputs, 16-bit twiddles
  f_ie                  floating point
 
  Input:
  x[N]                complex input signal. Real and imaginary data are interleaved 
                      and real data goes first

  twd[N*twdstep*3/4]  twiddle factor table of a complex-valued FFT of size N*twdstep
  N                   FFT size
  twdstep             twiddle step 
  scalingOpt          24x24_ie        - 3 (Fixed scaling)
                      32x16_ie        - 3 (Fixed scaling)


  Output:
  y[N]                output spectrum. Real and imaginary data are interleaved and 
                      real data goes first

  Returned value:     total number of right shifts occurred during scaling 
                      procedure

  Restrictions:
  x,y - should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 256, 512, 1024 for fixed-point routines, power of 2 and >=8 for 
        floating point

-------------------------------------------------------------------------*/

int ifft_cplx32x16_ie(complex_fract32* y,complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt)
{

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt==3);
    NASSERT(N==128||N==256||N==512||N==1024);

    fft_revorder_ie((int32_t*)x, N); 
    return fft_cplx32x16_ie(y, x, twd, twdstep, N, scalingOpt);
}
