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
#ifndef __NATUREDSP_SIGNAL_FFT_H__
#define __NATUREDSP_SIGNAL_FFT_H__

#include "NatureDSP_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
  Fast Fourier Transforms:
  fft_cplx             FFT on Complex Data
  fft_real             FFT on Real Data
  ifft_cplx            Inverse FFT on Complex Data
  ifft_real            Inverse FFT on Real Data
  fft_cplx<prec>_ie    FFT on Complex Data with Optimized Memory Usage
  stereo_fft_cplx<prec>_ie   FFT on Complex Data with Optimized Memory Usage, stereo inputs/outputs
  fft_real<prec>_ie    FFT on Real Data with Optimized Memory Usage
  ifft_cplx<prec>_ie   Inverse FFT on Complex Data with Optimized Memory Usage
  stereo_ifft_cplx<prec>_ie  Inverse FFT on Complex Data with Optimized Memory Usage, stereo inputs/outputs
  ifft_real<prec>_ie   Inverse FFT on Real Data with Optimized Memory Usage
  dct/dct4             Discrete Cosine Transform, Type II/Type IV
  mdct                 Modified Discrete Cosine Transform
  imdct                Inverse Modified Discrete Cosine Transform
  dct2d                2-D Discrete Cosine Transform
  idct2d               Inverse 2-D Discrete Cosine Transform

  There are limited combinations of precision and scaling options available:
  ----------------+---------------------------------------------------------------
      FFT/IFFT    | Scaling options                        | Restrictions on the
                  |                                        | input dynamic range
  ----------------+---------------------------------------------------------------
  cplx16x16       | 2 - 16-bit dynamic scaling             |        none
                  | 3 - fixed scaling before each stage    |        none
  cplx32x16       | 2 - 32-bit dynamic scaling             |        none
                  | 3 - fixed scaling before each stage    |        none
  cplx32x32       | 2 - 32-bit dynamic scaling             |        none
                  | 3 - fixed scaling before each stage    |        none
  cplx32x16_ie    | 2 - 32-bit dynamic scaling             |        none
                  | 3 - fixed scaling before each stage    |        none
  cplx32x32_ie    | 2 - 32-bit dynamic scaling             |        none
                  | 3 - fixed scaling before each stage    |        none
------------------------------------------------------------------------------------
  cplx16x16_ie    | 2 - 16-bit dynamic scaling             |        none
------------------------------------------------------------------------------------
  real16x16       | 2 - 16-bit dynamic scaling             |        none
                  | 3 - fixed scaling before each stage    |        none
  real32x16       | 2 - 32-bit dynamic scaling             |        none
                  | 3 - fixed scaling before each stage    |        none
  real32x32       | 2 - 32-bit dynamic scaling             |        none
                  | 3 - fixed scaling before each stage    |        none
  real32x16_ie    | 2 - 32-bit dynamic scaling             |        none
                  | 3 - fixed scaling before each stage    |        none
  real32x32_ie    | 2 - 32-bit dynamic scaling             |        none
                  | 3 - fixed scaling before each stage    |        none
------------------------------------------------------------------------------------
  real16x16_ie    | 2 - 16-bit dynamic scaling             |        none
  ----------------+---------------------------------------------------------------
  DCT:            |
  ----------------+---------------------------------------------------------------
  dct_32x16       | 3 - fixed scaling before each stage    |        none
  dct_32x32       | 3 - fixed scaling before each stage    |        none
  dct_16x16       | 3 - fixed scaling before each stage    |        none
  dct4_32x16      | 3 - fixed scaling before each stage    |        none
  dct4_32x32      | 3 - fixed scaling before each stage    |        none
  mdct_32x16      | 3 - fixed scaling before each stage    |        none
  mdct_32x32      | 3 - fixed scaling before each stage    |        none
  imdct_32x16     | 3 - fixed scaling before each stage    |        none
  imdct_32x32     | 3 - fixed scaling before each stage    |        none
  ----------------+---------------------------------------------------------------
  dct2d_8x16      | 0 - no scaling                         |        none
  idct2d_16x8     | 0 - no scaling                         |        none
  ----------------+---------------------------------------------------------------
===========================================================================*/

/*-------------------------------------------------------------------------
  FFT on Complex Data
  These functions make FFT on complex data.
    Scaling: 
      +-------------------+----------------------------------------+
      |      Function     |           Scaling options              |
      +-------------------+----------------------------------------+
      |  fft_cplx16x16    |  2 - 16-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  fft_cplx32x32    |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  fft_cplx32x16    |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      +-------------------+----------------------------------------+
  NOTES:
  1. Bit-reversing permutation is done here. 
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call
  3. 32x32,32x16,16x16 FFTs support mixed radix transforms 
  4. N - FFT size

  Precision: 
  32x16  32-bit input/outputs, 16-bit twiddles
  32x32  32-bit input/outputs, 32-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles
 
  Input:
  x[2*N]     complex input signal. Real and imaginary data are interleaved 
             and real data goes first
  scalingOpt scaling option (see table above)
  Output:
  y[2*N]     output spectrum. Real and imaginary data are interleaved and 
             real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y        should not overlap
  x,y        aligned on a 8-bytes boundary

-------------------------------------------------------------------------*/
typedef const void* fft_handle_t;
// FFT handles for 32x16 and 16x16
extern const fft_handle_t cfft16_16;      /* N = 16   */
extern const fft_handle_t cfft16_32;      /* N = 32   */
extern const fft_handle_t cfft16_64;      /* N = 64   */
extern const fft_handle_t cfft16_128;     /* N = 128  */
extern const fft_handle_t cfft16_256;     /* N = 256  */
extern const fft_handle_t cfft16_512;     /* N = 512  */
extern const fft_handle_t cfft16_1024;    /* N = 1024 */
extern const fft_handle_t cfft16_2048;    /* N = 2048 */
extern const fft_handle_t cfft16_4096;    /* N = 4096 */
// FFT handles for 32x32
extern const fft_handle_t cfft32_16;      /* N = 16   */
extern const fft_handle_t cfft32_32;      /* N = 32   */
extern const fft_handle_t cfft32_64;      /* N = 64   */
extern const fft_handle_t cfft32_128;     /* N = 128  */
extern const fft_handle_t cfft32_256;     /* N = 256  */
extern const fft_handle_t cfft32_512;     /* N = 512  */
extern const fft_handle_t cfft32_1024;    /* N = 1024 */
extern const fft_handle_t cfft32_2048;    /* N = 2048 */
extern const fft_handle_t cfft32_4096;    /* N = 4096 */

// FFT handles for mixed radix transforms, 16x16
extern const fft_handle_t cnfft16_160;    /* N = 160 */	
extern const fft_handle_t cnfft16_192;    /* N = 192 */	
extern const fft_handle_t cnfft16_240;    /* N = 240 */	
extern const fft_handle_t cnfft16_320;    /* N = 320 */	
extern const fft_handle_t cnfft16_384;    /* N = 384 */	
extern const fft_handle_t cnfft16_480;    /* N = 480 */	
// FFT handles for mixed radix transforms, 32x16
extern const fft_handle_t cnfft32x16_160; /* N = 160 */
extern const fft_handle_t cnfft32x16_192; /* N = 192 */
extern const fft_handle_t cnfft32x16_240; /* N = 240 */
extern const fft_handle_t cnfft32x16_320; /* N = 320 */
extern const fft_handle_t cnfft32x16_384; /* N = 384 */
extern const fft_handle_t cnfft32x16_480; /* N = 480 */

// FFT handles for mixed radix transforms, 32x32
extern const fft_handle_t cnfft32_12;     /* N = 12  */
extern const fft_handle_t cnfft32_24;     /* N = 24  */
extern const fft_handle_t cnfft32_36;     /* N = 36  */
extern const fft_handle_t cnfft32_48;     /* N = 48  */
extern const fft_handle_t cnfft32_60;     /* N = 60  */
extern const fft_handle_t cnfft32_72;     /* N = 72  */
extern const fft_handle_t cnfft32_96;     /* N = 96  */
extern const fft_handle_t cnfft32_80 ;    /* N = 80  */
extern const fft_handle_t cnfft32_100;    /* N = 100 */
extern const fft_handle_t cnfft32_108;    /* N = 108 */
extern const fft_handle_t cnfft32_120;    /* N = 120 */
extern const fft_handle_t cnfft32_144;    /* N = 144 */
extern const fft_handle_t cnfft32_160;    /* N = 160 */
extern const fft_handle_t cnfft32_180;    /* N = 180 */
extern const fft_handle_t cnfft32_192;    /* N = 192 */
extern const fft_handle_t cnfft32_200;    /* N = 200 */
extern const fft_handle_t cnfft32_216;    /* N = 216 */
extern const fft_handle_t cnfft32_240;    /* N = 240 */
extern const fft_handle_t cnfft32_288;    /* N = 288 */
extern const fft_handle_t cnfft32_300;    /* N = 300 */
extern const fft_handle_t cnfft32_320;    /* N = 320 */
extern const fft_handle_t cnfft32_324;    /* N = 324 */
extern const fft_handle_t cnfft32_360;    /* N = 360 */
extern const fft_handle_t cnfft32_384;    /* N = 384 */
extern const fft_handle_t cnfft32_400;    /* N = 400 */
extern const fft_handle_t cnfft32_432;    /* N = 432 */
extern const fft_handle_t cnfft32_480;    /* N = 480 */
extern const fft_handle_t cnfft32_540;    /* N = 540 */
extern const fft_handle_t cnfft32_576;    /* N = 576 */
extern const fft_handle_t cnfft32_600;    /* N = 600 */
extern const fft_handle_t cnfft32_768;    /* N = 768 */
extern const fft_handle_t cnfft32_960;    /* N = 960 */

int fft_cplx32x16( int32_t* y, int32_t* x, fft_handle_t h, int scalingOption );
int fft_cplx16x16( int16_t* y, int16_t* x, fft_handle_t h, int scalingOption );
int fft_cplx32x32( int32_t* y, int32_t* x, fft_handle_t h, int scalingOption );


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
// FFT handles for 32x16 and 16x16
extern const fft_handle_t rfft16_32;      /* N = 32   */
extern const fft_handle_t rfft16_64;      /* N = 64   */
extern const fft_handle_t rfft16_128;     /* N = 128  */
extern const fft_handle_t rfft16_256;     /* N = 256  */
extern const fft_handle_t rfft16_512;     /* N = 512  */
extern const fft_handle_t rfft16_1024;    /* N = 1024 */
extern const fft_handle_t rfft16_2048;    /* N = 2048 */
extern const fft_handle_t rfft16_4096;    /* N = 4096 */
extern const fft_handle_t rfft16_8192;    /* N = 8192 */
// FFT handles for 32x32
extern const fft_handle_t rfft32_32;      /* N = 32   */
extern const fft_handle_t rfft32_64;      /* N = 64   */
extern const fft_handle_t rfft32_128;     /* N = 128  */
extern const fft_handle_t rfft32_256;     /* N = 256  */
extern const fft_handle_t rfft32_512;     /* N = 512  */
extern const fft_handle_t rfft32_1024;    /* N = 1024 */
extern const fft_handle_t rfft32_2048;    /* N = 2048 */
extern const fft_handle_t rfft32_4096;    /* N = 4096 */
extern const fft_handle_t rfft32_8192;    /* N = 8192 */
// FFT handles for mixed radix transforms, 16x16
extern const fft_handle_t rnfft16_160;    /* N = 160 */	
extern const fft_handle_t rnfft16_192;    /* N = 192 */	
extern const fft_handle_t rnfft16_240;    /* N = 240 */	
extern const fft_handle_t rnfft16_320;    /* N = 320 */	
extern const fft_handle_t rnfft16_384;    /* N = 384 */	
extern const fft_handle_t rnfft16_480;    /* N = 480 */	
// FFT handles for mixed radix transforms, 32x16
extern const fft_handle_t rnfft32x16_160; /* N = 160 */
extern const fft_handle_t rnfft32x16_192; /* N = 192 */
extern const fft_handle_t rnfft32x16_240; /* N = 240 */
extern const fft_handle_t rnfft32x16_320; /* N = 320 */
extern const fft_handle_t rnfft32x16_384; /* N =384	 */
extern const fft_handle_t rnfft32x16_480; /* N =480	 */
// FFT handles for mixed radix transforms, 32x32
extern const fft_handle_t rnfft32_12  ;   /* N = 12   */
extern const fft_handle_t rnfft32_24  ;   /* N = 24   */
extern const fft_handle_t rnfft32_30  ;   /* N = 30   */
extern const fft_handle_t rnfft32_36  ;   /* N = 36   */
extern const fft_handle_t rnfft32_48  ;   /* N = 48   */
extern const fft_handle_t rnfft32_60  ;   /* N = 60   */
extern const fft_handle_t rnfft32_72  ;   /* N = 72   */
extern const fft_handle_t rnfft32_90  ;   /* N = 90   */
extern const fft_handle_t rnfft32_96  ;   /* N = 96   */
extern const fft_handle_t rnfft32_108 ;   /* N = 108  */
extern const fft_handle_t rnfft32_120 ;   /* N = 120  */
extern const fft_handle_t rnfft32_144 ;   /* N = 144  */
extern const fft_handle_t rnfft32_160 ;   /* N = 160  */
extern const fft_handle_t rnfft32_180 ;   /* N = 180  */
extern const fft_handle_t rnfft32_192 ;   /* N = 192  */
extern const fft_handle_t rnfft32_216 ;   /* N = 216  */
extern const fft_handle_t rnfft32_240 ;   /* N = 240  */
extern const fft_handle_t rnfft32_288 ;   /* N = 288  */
extern const fft_handle_t rnfft32_300 ;   /* N = 300  */
extern const fft_handle_t rnfft32_320 ;   /* N = 320  */
extern const fft_handle_t rnfft32_324 ;   /* N = 324  */
extern const fft_handle_t rnfft32_360 ;   /* N = 360  */
extern const fft_handle_t rnfft32_384 ;   /* N = 384  */
extern const fft_handle_t rnfft32_432 ;   /* N = 432  */
extern const fft_handle_t rnfft32_480 ;   /* N = 480  */
extern const fft_handle_t rnfft32_540 ;   /* N = 540  */
extern const fft_handle_t rnfft32_576 ;   /* N = 576  */
extern const fft_handle_t rnfft32_720 ;   /* N = 720  */
extern const fft_handle_t rnfft32_768 ;   /* N = 768  */
extern const fft_handle_t rnfft32_960 ;   /* N = 960  */
extern const fft_handle_t rnfft32_1152;   /* N = 1152 */
extern const fft_handle_t rnfft32_1440;   /* N = 1440 */
extern const fft_handle_t rnfft32_1536;   /* N = 1536 */
extern const fft_handle_t rnfft32_1920;   /* N = 1920 */

int fft_real16x16( int16_t* y, int16_t* x, fft_handle_t h, int scalingOpt );
int fft_real32x16( int32_t* y, int32_t* x, fft_handle_t h, int scalingOpt );
int fft_real32x32( int32_t* y, int32_t* x, fft_handle_t h, int scalingOpt );

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
// FFT handles for 32x16 and 16x16
extern const fft_handle_t cifft16_16;     /* N = 16   */
extern const fft_handle_t cifft16_32;     /* N = 32   */
extern const fft_handle_t cifft16_64;     /* N = 64   */
extern const fft_handle_t cifft16_128;    /* N = 128  */
extern const fft_handle_t cifft16_256;    /* N = 256  */
extern const fft_handle_t cifft16_512;    /* N = 512  */
extern const fft_handle_t cifft16_1024;   /* N = 1024 */
extern const fft_handle_t cifft16_2048;   /* N = 2048 */
extern const fft_handle_t cifft16_4096;   /* N = 4096 */
// FFT handles for 32x32
extern const fft_handle_t cifft32_16;     /* N = 16   */
extern const fft_handle_t cifft32_32;     /* N = 32   */
extern const fft_handle_t cifft32_64;     /* N = 64   */
extern const fft_handle_t cifft32_128;    /* N = 128  */
extern const fft_handle_t cifft32_256;    /* N = 256  */
extern const fft_handle_t cifft32_512;    /* N = 512  */
extern const fft_handle_t cifft32_1024;   /* N = 1024 */
extern const fft_handle_t cifft32_2048;   /* N = 2048 */
extern const fft_handle_t cifft32_4096;   /* N = 4096 */
// FFT handles for mixed radix transforms, 16x16
extern const fft_handle_t cinfft16_160;    /* N = 160 */	
extern const fft_handle_t cinfft16_192; /* N =192	 */	
extern const fft_handle_t cinfft16_240; /* N =240	 */	
extern const fft_handle_t cinfft16_320; /* N =320	 */	
extern const fft_handle_t cinfft16_384; /* N =384	 */	
extern const fft_handle_t cinfft16_480; /* N =480	 */	
// FFT handles for mixed radix transforms, 32x16
extern const fft_handle_t cinfft32x16_160; /* N =160	 */
extern const fft_handle_t cinfft32x16_192; /* N =192	 */
extern const fft_handle_t cinfft32x16_240; /* N =240	 */
extern const fft_handle_t cinfft32x16_320; /* N =320	 */
extern const fft_handle_t cinfft32x16_384; /* N =384	 */
extern const fft_handle_t cinfft32x16_480; /* N =480	 */
// FFT handles for mixed radix transforms, 32x32
extern const fft_handle_t cinfft32_12;     /* N = 12  */
extern const fft_handle_t cinfft32_24;     /* N = 24  */
extern const fft_handle_t cinfft32_36;     /* N = 36  */
extern const fft_handle_t cinfft32_48;     /* N = 48  */
extern const fft_handle_t cinfft32_60;     /* N = 60  */
extern const fft_handle_t cinfft32_72;     /* N = 72  */
extern const fft_handle_t cinfft32_96;     /* N = 96  */
extern const fft_handle_t cinfft32_80 ;    /* N = 80  */
extern const fft_handle_t cinfft32_100;    /* N = 100 */
extern const fft_handle_t cinfft32_108;    /* N = 108 */
extern const fft_handle_t cinfft32_120;    /* N = 120 */
extern const fft_handle_t cinfft32_144;    /* N = 144 */
extern const fft_handle_t cinfft32_160;    /* N = 160 */
extern const fft_handle_t cinfft32_180;    /* N = 180 */
extern const fft_handle_t cinfft32_192;    /* N = 192 */
extern const fft_handle_t cinfft32_200;    /* N = 200 */
extern const fft_handle_t cinfft32_216;    /* N = 216 */
extern const fft_handle_t cinfft32_240;    /* N = 240 */
extern const fft_handle_t cinfft32_288;    /* N = 288 */
extern const fft_handle_t cinfft32_300;    /* N = 300 */
extern const fft_handle_t cinfft32_320;    /* N = 320 */
extern const fft_handle_t cinfft32_324;    /* N = 324 */
extern const fft_handle_t cinfft32_360;    /* N = 360 */
extern const fft_handle_t cinfft32_384;    /* N = 384 */
extern const fft_handle_t cinfft32_400;    /* N = 400 */
extern const fft_handle_t cinfft32_432;    /* N = 432 */
extern const fft_handle_t cinfft32_480;    /* N = 480 */
extern const fft_handle_t cinfft32_540;    /* N = 540 */
extern const fft_handle_t cinfft32_576;    /* N = 576 */
extern const fft_handle_t cinfft32_600;    /* N = 600 */
extern const fft_handle_t cinfft32_768;    /* N = 768 */
extern const fft_handle_t cinfft32_960;    /* N = 960 */

int ifft_cplx32x16( int32_t* y, int32_t* x, fft_handle_t h, int scalingOption );
int ifft_cplx16x16( int16_t* y, int16_t* x, fft_handle_t h, int scalingOption );
int ifft_cplx32x32( int32_t* y, int32_t* x, fft_handle_t h, int scalingOption );

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
// FFT handles for 32x16 and 16x16
extern const fft_handle_t rifft16_32;     /* N = 32   */
extern const fft_handle_t rifft16_64;     /* N = 64   */
extern const fft_handle_t rifft16_128;    /* N = 128  */
extern const fft_handle_t rifft16_256;    /* N = 256  */
extern const fft_handle_t rifft16_512;    /* N = 512  */
extern const fft_handle_t rifft16_1024;   /* N = 1024 */
extern const fft_handle_t rifft16_2048;   /* N = 2048 */
extern const fft_handle_t rifft16_4096;   /* N = 4096 */
extern const fft_handle_t rifft16_8192;   /* N = 8192 */
// FFT handles for 32x32
extern const fft_handle_t rifft32_32;     /* N = 32   */
extern const fft_handle_t rifft32_64;     /* N = 64   */
extern const fft_handle_t rifft32_128;    /* N = 128  */
extern const fft_handle_t rifft32_256;    /* N = 256  */
extern const fft_handle_t rifft32_512;    /* N = 512  */
extern const fft_handle_t rifft32_1024;   /* N = 1024 */
extern const fft_handle_t rifft32_2048;   /* N = 2048 */
extern const fft_handle_t rifft32_4096;   /* N = 4096 */
extern const fft_handle_t rifft32_8192;   /* N = 8192 */
// FFT handles for mixed radix transforms, 16x16
extern const fft_handle_t rinfft16_160;    /* N = 160 */	
extern const fft_handle_t rinfft16_192; /* N =192	 */	
extern const fft_handle_t rinfft16_240; /* N =240	 */	
extern const fft_handle_t rinfft16_320; /* N =320	 */	
extern const fft_handle_t rinfft16_384; /* N =384	 */	
extern const fft_handle_t rinfft16_480; /* N =480	 */	
// FFT handles for mixed radix transforms, 16x16
extern const fft_handle_t rinfft32x16_160; /* N =160	 */
extern const fft_handle_t rinfft32x16_192; /* N =192	 */
extern const fft_handle_t rinfft32x16_240; /* N =240	 */
extern const fft_handle_t rinfft32x16_320; /* N =320	 */
extern const fft_handle_t rinfft32x16_384; /* N =384	 */
extern const fft_handle_t rinfft32x16_480; /* N =480	 */
// FFT handles for mixed radix transforms, 32x32
extern const fft_handle_t rinfft32_12  ;   /* N = 12   */
extern const fft_handle_t rinfft32_24  ;   /* N = 24   */
extern const fft_handle_t rinfft32_30  ;   /* N = 30   */
extern const fft_handle_t rinfft32_36  ;   /* N = 36   */
extern const fft_handle_t rinfft32_48  ;   /* N = 48   */
extern const fft_handle_t rinfft32_60  ;   /* N = 60   */
extern const fft_handle_t rinfft32_72  ;   /* N = 72   */
extern const fft_handle_t rinfft32_90  ;   /* N = 90   */
extern const fft_handle_t rinfft32_96  ;   /* N = 96   */
extern const fft_handle_t rinfft32_108 ;   /* N = 108  */
extern const fft_handle_t rinfft32_120 ;   /* N = 120  */
extern const fft_handle_t rinfft32_144 ;   /* N = 144  */
extern const fft_handle_t rinfft32_160 ;   /* N = 160  */
extern const fft_handle_t rinfft32_180 ;   /* N = 180  */
extern const fft_handle_t rinfft32_192 ;   /* N = 192  */
extern const fft_handle_t rinfft32_216 ;   /* N = 216  */
extern const fft_handle_t rinfft32_240 ;   /* N = 240  */
extern const fft_handle_t rinfft32_288 ;   /* N = 288  */
extern const fft_handle_t rinfft32_300 ;   /* N = 300  */
extern const fft_handle_t rinfft32_320 ;   /* N = 320  */
extern const fft_handle_t rinfft32_324 ;   /* N = 324  */
extern const fft_handle_t rinfft32_360 ;   /* N = 360  */
extern const fft_handle_t rinfft32_384 ;   /* N = 384  */
extern const fft_handle_t rinfft32_432 ;   /* N = 432  */
extern const fft_handle_t rinfft32_480 ;   /* N = 480  */
extern const fft_handle_t rinfft32_540 ;   /* N = 540  */
extern const fft_handle_t rinfft32_576 ;   /* N = 576  */
extern const fft_handle_t rinfft32_720 ;   /* N = 720  */
extern const fft_handle_t rinfft32_768 ;   /* N = 768  */
extern const fft_handle_t rinfft32_960 ;   /* N = 960  */
extern const fft_handle_t rinfft32_1152;   /* N = 1152 */
extern const fft_handle_t rinfft32_1440;   /* N = 1440 */
extern const fft_handle_t rinfft32_1536;   /* N = 1536 */
extern const fft_handle_t rinfft32_1920;   /* N = 1920 */

int ifft_real16x16( int16_t* y, int16_t* x, fft_handle_t h, int scalingOpt );
int ifft_real32x16( int32_t* y, int32_t* x, fft_handle_t h, int scalingOpt );
int ifft_real32x32( int32_t* y, int32_t* x, fft_handle_t h, int scalingOpt );

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
/* Ordinary variant */
int fft_cplx16x16_ie(complex_fract16* y, complex_fract16* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int fft_cplx32x16_ie(complex_fract32* y, complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int fft_cplx32x32_ie(complex_fract32* y, complex_fract32* x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int fft_cplxf_ie    (complex_float  * y, complex_float  * x, const complex_float  * twd, int twdstep, int N);
/* Stereo variant */
int stereo_fft_cplx16x16_ie(complex_fract16* y, complex_fract16* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int stereo_fft_cplx32x16_ie(complex_fract32* y, complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int stereo_fft_cplx32x32_ie(complex_fract32* y, complex_fract32* x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int stereo_fft_cplxf_ie    (complex_float  * y, complex_float  * x, const complex_float  * twd, int twdstep, int N);

/*-------------------------------------------------------------------------
  FFT on Real Data with Optimized Memory Usage
  These functions make FFT on real data forming half of spectrum with
  optimized memory usage.
  Scaling: 
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      |  fft_real16x16_ie     |  2 - 16-bit dynamic scaling          |
      |  fft_real32x16_ie     |  2 - 32-bit dynamic scaling          |
      |                       |  3 - fixed scaling before each stage |
      |  fft_real32x32_ie     |  2 - 32-bit dynamic scaling          |
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
  x                     input signal: 
  --------------+----------+-----------------+----------+
  Function      |   Size   |  Allocated Size |  type    |
  --------------+----------+-----------------+-----------
  16x16_ie      |     N    |      N          |  int16_t |
  32x16_ie      |     N    |      N          |  int32_t |
  32x32_ie      |     N    |      N          |  int32_t |
  --------------+----------+-----------------+----------+

  twd[N*twdstep*3/4]    twiddle factor table of a complex-valued 
                        FFT of size N*twdstep
  N                     FFT size
  twdstep               twiddle step
  scalingOpt            scaling option (see table above), not applicable 
                        to the floating point function

  Output:
  y                     output spectrum. Real and imaginary data 
                        are interleaved and real data goes first:
  --------------+----------+-----------------+---------------+
  Function      |   Size   |  Allocated Size |  type         |
  --------------+----------+-----------------+----------------
  16x16_ie      |   N/2+1  |      N/2+1      |complex_fract16|
  32x16_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  32x32_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  f_ie          |   N/2+1  |      N/2+1      |complex_float  |
  --------------+----------+-----------------+---------------+

  Returned value: total number of right shifts occurred during scaling
  procedure

  Restrictions:
  x,y     should not overlap
  x,y     aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
int fft_real16x16_ie    (complex_fract16* y, int16_t  * x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int fft_real32x16_ie    (complex_fract32* y, int32_t  * x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int fft_real32x32_ie    (complex_fract32* y, int32_t  * x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int fft_realf_ie        (complex_float  * y,float32_t * x, const complex_float  * twd, int twdstep, int N);

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
/* Ordinary variant */
int ifft_cplx16x16_ie(complex_fract16* y,complex_fract16* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int ifft_cplx32x16_ie(complex_fract32* y,complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int ifft_cplx32x32_ie(complex_fract32* y,complex_fract32* x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int ifft_cplxf_ie    (complex_float  * y,complex_float  * x, const complex_float  * twd, int twdstep, int N);
/* Stereo variant */
int stereo_ifft_cplx16x16_ie(complex_fract16* y, complex_fract16* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int stereo_ifft_cplx32x16_ie(complex_fract32* y, complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int stereo_ifft_cplx32x32_ie(complex_fract32* y, complex_fract32* x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int stereo_ifft_cplxf_ie    (complex_float  * y, complex_float  * x, const complex_float  * twd, int twdstep, int N);

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
int ifft_real16x16_ie    (  int16_t* y, complex_fract16* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int ifft_real32x16_ie    (  int32_t* y, complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int ifft_real32x32_ie    (  int32_t* y, complex_fract32* x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int ifft_realf_ie        (float32_t* y,   complex_float* x, const   complex_float* twd, int twdstep, int N);

/*-------------------------------------------------------------------------
  Power spectrum
  These functions compute a normalized power spectrum from the output signal 
  generated by an FFT function. The N argument specifies the size of the FFT 
  and must be a power of 2. The  mode argument is used to specify the type 
  of FFT function used to generate the x array. If the x array has been 
  generated from a frequency-domain complex input signal (output of complex 
  FFT function), the mode argument must be set to 0. Otherwise the mode 
  argument must be set to 1 to signify that the x array has been generated 
  from a frequency-domain real input signal (output of real FFT function).
  The block_exponent argument is used to control the normalization of the 
  power spectrum. It will usually be set to the block_exponent that is 
  returned by corresponding FFT functions.  If the input array was 
  generated by some other means, then the value specified for the 
  block_exponent argument will depend upon how the FFT was calculated. 
  If the function used to calculate the FFT did not scale the intermediate 
  results at any of the stages of the computation, then set block_exponent 
  to zero; if the FFT function scaled the intermediate results at each 
  stage of the computation, then set block_exponent to -1; otherwise set 
  block_exponent to the sum of negated base-2 logarithm of all scaling 
  factors applied to data at intermediate FFT stages. This value will be 
  in the range 0 to log2(N).
  fft_spectrum functions write the power spectrum to the output array y. 
  If mode is set to 0, then the length of the power spectrum will be N. If 
  mode is set to 1, then the length of the power spectrum will be (N/2+1)

  Precision:
  16x32   16-bit inputs, 32-bit outputs
  32x32   32-bit inputs/outputs
  f       floating point inputs/outputs. Requires VFPU/SFPU core option


  Input:
  for mode==0
  x[N]           input spectrum . Real and imaginary
                 data are interleaved and real data goes first:
  for mode==1
  x[N/2+1]       input spectrum (positive side). Real and imaginary
                 data are interleaved and real data goes first:
  block_exponent power spectrum normalization control
  N              FFT size
  mode           power spectrum mode:
                 0 � complex signal
                 1 � real signal

  twdstep               twiddle step
  scalingOpt            scaling option (see table above), not applicable 
                        to the floating point function
  Output:
  for mode==0
  y[N]           output power spectrum
  for mode==1
  y[N/2+1]       output power spectrum
  Returned value:  none

  Restrictions:
  x,y   should not overlap
  x,y   aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
void fft_spectrumf     ( float32_t* y, const complex_float   * x, int N,                     int mode );
void fft_spectrum16x32 ( int32_t*   y, const complex_fract16 * x, int N, int block_exponent, int mode );
void fft_spectrum32x32 ( int32_t*   y, const complex_fract32 * x, int N, int block_exponent, int mode );

/*-------------------------------------------------------------------------
  Discrete Cosine Transform.
  These functions apply DCT (Type II, Type IV) to input.
  Scaling:
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      |       dct_16x16       |  3 - fixed scaling before each stage |
      |       dct_32x16       |  3 - fixed scaling before each stage |
      |       dct_32x32       |  3 - fixed scaling before each stage |
      |       dct4_32x16      |  3 - fixed scaling before each stage |
      |       dct4_32x32      |  3 - fixed scaling before each stage |
      +-----------------------+--------------------------------------+
  NOTES:
     1. DCT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call.
     2. N - DCT size (depends on selected DCT handle)

  Precision: 
  16x16  16-bit input/outputs, 16-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  32x32  32-bit input/outputs, 32-bit twiddles
  f      floating point

  Input:
  x[N]        input signal
  h           DCT handle
  scalingOpt  scaling option (see table above) 
              not applicable to the floating point function
  Output:
  y[N]        transform output
  
  Returned value:
              total number of right shifts occurred during scaling 
              procedure 
  Restriction:
  x,y         should not overlap
  x,y         aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
typedef const void * dct_handle_t;
// DCT-II handles
extern const dct_handle_t dct2_16_32;    /* N=32, dct_16x16, dct_32x16 */
extern const dct_handle_t dct2_16_64;    /* N=64, dct_16x16, dct_32x16     */
extern const dct_handle_t dct2_32_32;    /* N=32, dct_32x32     */
extern const dct_handle_t dct2_32_64;    /* N=64, dct_32x32     */
extern const dct_handle_t dct2_f_32;     /* N=32, dctf                     */
extern const dct_handle_t dct2_f_64;     /* N=64, dctf                     */
// DCT-IV handles
extern const dct_handle_t dct4_16_32 ;   /* N=32 , dct4_32x16 */
extern const dct_handle_t dct4_16_64 ;   /* N=64 , dct4_32x16              */
extern const dct_handle_t dct4_16_128;   /* N=128, dct4_32x16              */
extern const dct_handle_t dct4_16_256;   /* N=256, dct4_32x16              */
extern const dct_handle_t dct4_16_512;   /* N=512, dct4_32x16              */
extern const dct_handle_t dct4_32_32 ;   /* N=32 , dct4_32x32 */
extern const dct_handle_t dct4_32_64 ;   /* N=64 , dct4_32x32 */
extern const dct_handle_t dct4_32_128;   /* N=128, dct4_32x32 */
extern const dct_handle_t dct4_32_256;   /* N=256, dct4_32x32 */
extern const dct_handle_t dct4_32_512;   /* N=512, dct4_32x32 */
// Type II
int dct_16x16 ( int16_t   * y,int16_t   * x,dct_handle_t h,int scalingOpt);
int dct_32x16 ( int32_t   * y,int32_t   * x,dct_handle_t h,int scalingOpt);
int dct_32x32 ( int32_t   * y,int32_t   * x,dct_handle_t h,int scalingOpt);
int dctf      ( float32_t * y,float32_t * x,dct_handle_t h               );
// Type IV
int dct4_32x16( int32_t   * y,int32_t   * x,dct_handle_t h,int scalingOpt);
int dct4_32x32( int32_t   * y,int32_t   * x,dct_handle_t h,int scalingOpt);

/*-------------------------------------------------------------------------
  Modified Discrete Cosine Transform.
  These functions apply Modified DCT to input (convert 2N real data to N 
  spectral components) and make inverse conversion forming 2N numbers from 
  N inputs. Normally, combination of MDCT and DCT is invertible if applied 
  to subsequent data blocks with overlapping.
  Scaling:
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      |       mdct_32x16      |  3 - fixed scaling before each stage |
      |       mdct_32x32      |  3 - fixed scaling before each stage |
      |      imdct_32x16      |  3 - fixed scaling before each stage |
      |      imdct_32x32      |  3 - fixed scaling before each stage |
      +-----------------------+--------------------------------------+
  NOTES:
     1. MDCT/IMDCT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED 
     after the call.
     2. N - MDCT size (depends on selected MDCT handle)

  Precision: 
  32x16  32-bit input/outputs, 16-bit twiddles
  32x32  32-bit input/outputs, 32-bit twiddles

  -------------------------------------------------------------------------
  For MDCT:
  Input:
  x[2*N]      input signal
  h           MDCT handle
  scalingOpt  scaling option (see table above)
  Output:
  y[N]        output of transform 
  -------------------------------------------------------------------------
  For IMDCT:
  Input:
  x[N]        input signal
  h           IMDCT handle
  scalingOpt  scaling option (see table above)
  Output:
  y[2*N]      output of transform
  -------------------------------------------------------------------------
  Returned value:
              total number of right shifts occurred during scaling 
              procedure 
  Restriction:
  x,y         should not overlap
  x,y         aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
// MDCT/IMDCT handles                                                                                
extern const dct_handle_t mdct_16_32 ;   /* N=32 , mdct_32x16, imdct_32x16 */
extern const dct_handle_t mdct_16_64 ;   /* N=64 , mdct_32x16, imdct_32x16                           */
extern const dct_handle_t mdct_16_128;   /* N=128, mdct_32x16, imdct_32x16                           */
extern const dct_handle_t mdct_16_256;   /* N=256, mdct_32x16, imdct_32x16                           */
extern const dct_handle_t mdct_16_512;   /* N=512, mdct_32x16, imdct_32x16                           */
extern const dct_handle_t mdct_32_32 ;   /* N=32 , mdct_32x32  */
extern const dct_handle_t mdct_32_64 ;   /* N=64 , mdct_32x32  */
extern const dct_handle_t mdct_32_128;   /* N=128, mdct_32x32  */
extern const dct_handle_t mdct_32_256;   /* N=256, mdct_32x32  */
extern const dct_handle_t mdct_32_512;   /* N=512, mdct_32x32  */

int  mdct_32x16(int32_t* y, int32_t* x, dct_handle_t h, int scalingOpt);
int  mdct_32x32(int32_t* y, int32_t* x, dct_handle_t h, int scalingOpt);
int imdct_32x16(int32_t* y, int32_t* x, dct_handle_t h, int scalingOpt);
int imdct_32x32(int32_t* y, int32_t* x, dct_handle_t h, int scalingOpt);

/*-------------------------------------------------------------------------
  2-D Discrete Cosine Transform.
  These functions apply DCT (Type II) to the series of L input blocks 
  of NxN pixels. Algorithm uses ITU-T T.81 (JPEG compression) DCT-II 
  definition with bias 128 and left-to-right, top-to-bottom orientation.

  Scaling:
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      |       dct2d_8x16      |           0 - no scaling             |
      +-----------------------+--------------------------------------+
  Notes:
  N - DCT size (depends on selected DCT handle)

  Precision: 
  8x16  8-bit unsigned input, 16-bit signed output

  Input:
  x[N*N*L]    input pixels: L NxN blocks
  h           DCT handle
  L           number of input blocks
  scalingOpt  scaling option (see table above), should be 0

  Output:
  y[N*N*L]    output of transform: L NxN blocks
  
  Returned value: 0
  Restriction:
  x,y         should not overlap
  x,y         aligned on 8-bytes boundary

-------------------------------------------------------------------------*/
int dct2d_8x16(int16_t * y, uint8_t * x, dct_handle_t h, int L, int scalingOpt);
// 2D-DCT handles
extern const dct_handle_t dct2d_16_8 ;   // N=8 , dct2d_8x16

/*-------------------------------------------------------------------------
  2-D Inverse Discrete Cosine Transform.
  These functions apply inverse DCT (Type II) to the series of L input 
  blocks of NxN pixels. Algorithm uses ITU-T T.81 (JPEG compression) DCT-II 
  definition with bias 128 and left-to-right, top-to-bottom orientation.

  Scaling:
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      |       idct2d_16x8     |           0 - no scaling             |
      +-----------------------+--------------------------------------+
  Notes:
  N - IDCT size (depends on selected IDCT handle)

  Precision: 
  16x8  16-bit signed input, 8-bit unsigned output

  Input:
  x[N*N*L]    input data: L NxN blocks
  h           DCT handle
  L           number of input blocks
  scalingOpt  scaling option (see table above), should be 0

  Output:
  y[N*N*L]    output pixels: L NxN blocks

  Returned value: 0
  Restriction:
  x,y         should not overlap
  x,y         aligned on 8-bytes boundary

-------------------------------------------------------------------------*/
int idct2d_16x8(uint8_t * y, int16_t * x, dct_handle_t h, int L, int scalingOpt);
// 2D-IDCT handles
extern const dct_handle_t idct2d_16_8 ;   // N=8 , idct2d_16x8


#ifdef __cplusplus
}
#endif

#endif/* __NATUREDSP_SIGNAL_FFT_H__ */
