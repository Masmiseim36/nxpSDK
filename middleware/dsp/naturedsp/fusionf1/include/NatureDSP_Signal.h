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

#ifndef __NATUREDSP_SIGNAL_H__
#define __NATUREDSP_SIGNAL_H__

#include "NatureDSP_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
  FIR filters and related functions:

  bkfir,bkfira                          Block real FIR filter 
  cxfir                                 Complex block FIR filter
  firdec                                Decimating block real FIR filter
  firinterp,cxfirinterp                 Interpolating block real FIR filter
  fir_convol,cxfir_convol,fir_lconvola  linear/circular convolution
  fir_xcorr,cxfir_xcorr,fir_lxcorra     linear/circular correlation
  fir_acorr,fir_lacorra                 linear/circular autocorrelation 
  
  fir_blms                              Blockwise Adaptive LMS algorithm for 
                                        real data
===========================================================================*/

/*-------------------------------------------------------------------------
  Real FIR filter.
  Computes a real FIR filter (direct-form) using IR stored in vector h. The 
  real data input is stored in vector x. The filter output result is stored 
  in vector y. The filter calculates N output samples using M coefficients 
  and requires last M-1 samples in the delay line.
  NOTE: 
   user application is not responsible for management of delay lines


  Precision: 
  16x16  16-bit data, 16-bit coefficients, 16-bit outputs
  24x24  24-bit data, 24-bit coefficients, 24-bit outputs
  24x24p use 24-bit data packing for internal delay line buffer
         and internal coefficients storage
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  32x32    32-bit data, 32-bit coefficients, 32-bit outputs
  f        floating point

  Input:
  x[N]      - input samples, Q31, Q15, floating point
  h[M]      - filter coefficients in normal order, Q31, Q15, floating point
  N         - length of sample block, should be a multiple of 4
  M         - length of filter, should be a multiple of 4
  Output:
  y[N]      - input samples, Q31, Q15, floating point

  Restrictions:
  x,y should not be overlapping
  x,h - aligned on a 8-bytes boundary
  N,M - multiples of 4 
-------------------------------------------------------------------------*/
typedef void * bkfir16x16_handle_t;
typedef void * bkfir24x24_handle_t;
typedef void * bkfir24x24p_handle_t;
typedef void * bkfir32x16_handle_t;
typedef void * bkfir32x32_handle_t;
typedef void * bkfirf_handle_t;

// Update the delay line and compute filter output
void bkfir16x16_process ( bkfir16x16_handle_t  handle, int16_t   * restrict y, const int16_t   * restrict x, int N);
void bkfir24x24_process ( bkfir24x24_handle_t  handle, f24       * restrict y, const f24       * restrict x, int N);
void bkfir24x24p_process( bkfir24x24p_handle_t handle, f24       * restrict y, const f24       * restrict x, int N);
void bkfir32x16_process ( bkfir32x16_handle_t  handle, int32_t   * restrict y, const int32_t   * restrict x, int N);
void bkfir32x32_process ( bkfir32x32_handle_t  handle, int32_t   * restrict y, const int32_t   * restrict x, int N);
void bkfirf_process     ( bkfirf_handle_t      handle, float32_t * restrict y, const float32_t * restrict x, int N);

/* Returns: size of memory in bytes to be allocated */
size_t bkfir16x16_alloc (int M);
size_t bkfir24x24_alloc (int M); 
size_t bkfir24x24p_alloc(int M);
size_t bkfir32x16_alloc (int M);
size_t bkfir32x32_alloc (int M);
size_t bkfirf_alloc     (int M); 

/* Returns: handle to the object */
bkfir16x16_handle_t bkfir16x16_init  (void * objmem, int M, const int16_t  * restrict h);
bkfir24x24_handle_t bkfir24x24_init  (void * objmem, int M, const f24      * restrict h);
bkfir24x24p_handle_t bkfir24x24p_init(void * objmem, int M, const f24      * restrict h);
bkfir32x16_handle_t bkfir32x16_init  (void * objmem, int M, const int16_t  * restrict h);
bkfir32x32_handle_t bkfir32x32_init  (void * objmem, int M, const int32_t  * restrict h);
bkfirf_handle_t     bkfirf_init      (void * objmem, int M, const float32_t* restrict h);

/*-------------------------------------------------------------------------
  Real FIR filter.
  Computes a real FIR filter (direct-form) using IR stored in vector h. The 
  real data input is stored in vector x. The filter output result is stored 
  in vector y. The filter calculates N output samples using M coefficients 
  and requires last M-1 samples in the delay line.
  These functions implement FIR filter described in previous chapter with no 
  limitation on size of data block, alignment and length of impulse response 
  for the cost of performance.
  NOTE: 
  User application is not responsible for management of delay lines

  Precision: 
  16x16    16-bit data, 16-bit coefficients, 16-bit outputs
  24x24    24-bit data, 24-bit coefficients, 24-bit outputs
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  f        floating point
  Input:
  x[N]      - input samples, Q31, floating point
  h[M]      - filter coefficients in normal order, Q31, Q15, floating point
  N         - length of sample block
  M         - length of filter
  Output:
  y[N]      - input samples, Q31, floating point 

  Restrictions:
  x,y should not be overlapping
-------------------------------------------------------------------------*/
typedef void * bkfira16x16_handle_t;
typedef void * bkfira24x24_handle_t;
typedef void * bkfira32x16_handle_t;
typedef void * bkfira32x32_handle_t;
typedef void * bkfiraf_handle_t;

// Returns: size of memory in bytes to be allocated
size_t bkfira16x16_alloc(int M);
size_t bkfira24x24_alloc(int M);
size_t bkfira32x16_alloc(int M);
size_t bkfira32x32_alloc(int M);
size_t bkfiraf_alloc(int M);

// Returns: handle to the object
bkfira16x16_handle_t bkfira16x16_init (void * objmem, int M, const int16_t  * restrict h);
bkfira24x24_handle_t bkfira24x24_init (void * objmem, int M, const f24      * restrict h);
bkfira32x16_handle_t bkfira32x16_init (void * objmem, int M, const int16_t  * restrict h);
bkfira32x32_handle_t bkfira32x32_init (void * objmem, int M, const int32_t  * restrict h);
bkfira32x16_handle_t bkfiraf_init     (void * objmem, int M, const float32_t* restrict h);

// Update the delay line and compute filter output
void bkfira16x16_process ( bkfira16x16_handle_t  handle, int16_t   * restrict y, const int16_t   * restrict x,  int N);
void bkfira24x24_process ( bkfira24x24_handle_t  handle, f24       * restrict y, const f24       * restrict x , int N);
void bkfira32x16_process ( bkfira32x16_handle_t  handle, int32_t   * restrict y, const int32_t   * restrict x , int N);
void bkfira32x32_process ( bkfira32x32_handle_t  handle, int32_t   * restrict y, const int32_t   * restrict x,  int N);
void bkfiraf_process     ( bkfiraf_handle_t      handle, float32_t * restrict y, const float32_t * restrict x , int N);

/*-------------------------------------------------------------------------
  Complex Block FIR Filter
  Computes a complex FIR filter (direct-form) using complex IR stored in 
  vector h. The complex data input is stored in vector x. The filter output
  result is stored in vector r. The filter calculates N output samples using 
  M coefficients and requires last M-1 samples in the delay line. Real and
  imaginary parts are interleaved and real parts go first (at even indexes).
  NOTE: 
  User application is not responsible for management of delay lines

  Precision: 
  16x16     16-bit data, 16-bit coefficients, 16-bit outputs
  24x24     24-bit data, 24-bit coefficients, 24-bit outputs
  32x16     32-bit data, 16-bit coefficients, 32-bit outputs
  32x32     32-bit data, 32-bit coefficients, 32-bit outputs
  f         floating point
  Input:
  h[M] complex filter coefficients; h[0] is to be multiplied with the newest 
       sample , Q31, Q15, floating point
  x[N] input samples , Q15, Q31 or floating point
  N    length of sample block (in complex samples) 
  M    length of filter 
  Output:
  y[N] output samples , Q15, Q31 or floating point

  Restriction:
  x,y - should not overlap
  x,h - aligned on a 8-bytes boundary
  N,M - multiples of 4
-------------------------------------------------------------------------*/
typedef void* cxfir16x16_handle_t;
typedef void* cxfir24x24_handle_t;
typedef void* cxfir32x16_handle_t;
typedef void* cxfir32x32_handle_t;
typedef void* cxfirf_handle_t;

// Returns: size of memory in bytes to be allocated
size_t cxfir16x16_alloc(int M);
size_t cxfir24x24_alloc(int M);
size_t cxfir32x16_alloc(int M);
size_t cxfir32x32_alloc(int M);
size_t cxfirf_alloc(int M);

// Returns: handle to the object
cxfir16x16_handle_t cxfir16x16_init(void * objmem, int M, const complex_fract16* restrict h);
cxfir24x24_handle_t cxfir24x24_init(void * objmem, int M, const complex_fract32* restrict h);
cxfir32x16_handle_t cxfir32x16_init(void * objmem, int M, const complex_fract16* restrict h);
cxfir32x32_handle_t cxfir32x32_init(void * objmem, int M, const complex_fract32* restrict h);
cxfirf_handle_t     cxfirf_init    (void * objmem, int M, const complex_float  * restrict h);

// Update the delay line and compute filter output
void cxfir16x16_process(cxfir16x16_handle_t handle, complex_fract16 * restrict y, const complex_fract16*  restrict x,  int N);
void cxfir24x24_process(cxfir24x24_handle_t handle, complex_fract32 * restrict y, const complex_fract32 * restrict x , int N);
void cxfir32x16_process(cxfir32x16_handle_t handle, complex_fract32 * restrict y, const complex_fract32 * restrict x , int N);
void cxfir32x32_process(cxfir32x32_handle_t handle, complex_fract32 * restrict y, const complex_fract32 * restrict x,  int N);
void cxfirf_process    (cxfirf_handle_t     handle, complex_float   * restrict y, const complex_float   * restrict x , int N);

/*-------------------------------------------------------------------------
  Decimating Block Real FIR Filter
  Computes a real FIR filter (direct-form) with decimation using IR stored 
  in vector h. The real data input is stored in vector x. The filter output 
  result is stored in vector r. The filter calculates N output samples using
  M coefficients and requires last D*N+M-1 samples on the delay line.
  NOTE:
  - To avoid aliasing IR should be synthesized in such a way to be narrower 
    than input sample rate divided to 2D.
  - user application is not responsible for management of delay lines

  Precision: 
  24x24     24-bit data, 24-bit coefficients, 24-bit outputs
  32x16     32-bit data, 16-bit coefficients, 32-bit outputs
  f         floating point

  Input:
  h[M]          filter coefficients; h[0] is to be multiplied with the newest 
                sample, Q31, Q15, floating point
  D             decimation factor 
  N             length of output sample block
  M             length of filter
  x[D*N]        input samples , Q15, Q31 or floating point
  Output:
  y[N]          output samples, Q15, Q31 or floating point

  Restriction:
  x,h,r should not overlap
  x, h - aligned on an 8-bytes boundary
  N - multiple of 8
  D should exceed 1

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  D - 2, 3 or 4
-------------------------------------------------------------------------*/
typedef void* firdec16x16_handle_t;
typedef void* firdec24x24_handle_t;
typedef void* firdec32x16_handle_t;
typedef void* firdec32x32_handle_t;
typedef void* firdecf_handle_t;

// Returns: size of memory in bytes to be allocated
size_t firdec16x16_alloc(int D, int M);
size_t firdec24x24_alloc(int D, int M);
size_t firdec32x16_alloc(int D, int M);
size_t firdec32x32_alloc(int D, int M);
size_t firdecf_alloc    (int D, int M);

// Returns: handle to the object
firdec16x16_handle_t firdec16x16_init(void * objmem, int D, int M, const int16_t   * restrict h);
firdec24x24_handle_t firdec24x24_init(void * objmem, int D, int M, const f24       * restrict h);
firdec32x16_handle_t firdec32x16_init(void * objmem, int D, int M, const int16_t   * restrict h);
firdec32x32_handle_t firdec32x32_init(void * objmem, int D, int M, const int32_t   * restrict h);
firdecf_handle_t     firdecf_init    (void * objmem, int D, int M, const float32_t * restrict h);

// Update the delay line and compute decimator output
void firdec16x16_process(firdec16x16_handle_t handle, int16_t   * restrict y, const int16_t   * x, int N);
void firdec24x24_process(firdec24x24_handle_t handle, f24       * restrict y, const f24       * x, int N);
void firdec32x16_process(firdec32x16_handle_t handle, int32_t   * restrict y, const int32_t   * x, int N);
void firdec32x32_process(firdec32x32_handle_t handle, int32_t   * restrict y, const int32_t   * x, int N);
void firdecf_process    (firdecf_handle_t     handle, float32_t * restrict y, const float32_t * x, int N);

/*-------------------------------------------------------------------------
  Interpolating Block Real/Complex FIR Filter
  Computes a real FIR filter (direct-form) with interpolation using IR stored 
  in vector h. The real data input is stored in vector x. The filter output 
  result is stored in vector y. The filter calculates N*D output samples 
  using M*D coefficients and requires last N+M*D-1 samples on the delay line.
  NOTE:
  user application is not responsible for management of delay lines

  Precision: 
  16x16     16-bit real data, 16-bit coefficients, 16-bit real outputs 
  16x16     16-bit complex data, 16-bit coefficients, 16-bit complex outputs
  24x24     24-bit real data, 24-bit coefficients, 24-bit real outputs 
  32x16     32-bit real data, 16-bit coefficients, 32-bit real outputs 
  32x32     32-bit real data, 32-bit coefficients, 32-bit real outputs 
  f         floating point

  Input:
  h[M*D]        filter coefficients; h[0] is to be multiplied with the 
                newest sample,Q31, Q15, floating point
  D             interpolation ratio
  N             length of input sample block
  M             length of subfilter. Total length of filter is M*D
  x[N]          input samples,Q15, Q31 or floating point
  Output:
  y[N*D]        output samples, Q15, Q31 or floating point

  Restrictions:
  x,h,y should not overlap
  x,h - aligned on an 8-bytes boundary
  N   - multiple of 8
  M   - multiple of 4
  D should be >1

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  D   - 2, 3 or 4

-------------------------------------------------------------------------*/
typedef void* firinterp16x16_handle_t;
typedef void* cxfirinterp16x16_handle_t;
typedef void* firinterp24x24_handle_t;
typedef void* firinterp32x16_handle_t;
typedef void* firinterp32x32_handle_t;
typedef void* firinterpf_handle_t;

// Returns: size of memory in bytes to be allocated
size_t firinterp16x16_alloc  (int D, int M);
size_t cxfirinterp16x16_alloc(int D, int M);
size_t firinterp24x24_alloc  (int D, int M);
size_t firinterp32x16_alloc  (int D, int M);
size_t firinterp32x32_alloc  (int D, int M);
size_t firinterpf_alloc      (int D, int M);

// Returns: handle to the object
firinterp16x16_handle_t firinterp16x16_init    (void * objmem, int D, int M, const int16_t   * restrict h);
cxfirinterp16x16_handle_t cxfirinterp16x16_init(void * objmem, int D, int M, const int16_t   * restrict h);
firinterp24x24_handle_t firinterp24x24_init    (void * objmem, int D, int M, const f24       * restrict h);
firinterp32x16_handle_t firinterp32x16_init    (void * objmem, int D, int M, const int16_t   * restrict h);
firinterp32x32_handle_t firinterp32x32_init    (void * objmem, int D, int M, const int32_t   * restrict h);
firinterpf_handle_t     firinterpf_init        (void * objmem, int D, int M, const float32_t * restrict h);

// Update the delay line and compute interpolator output
void firinterp16x16_process  (firinterp16x16_handle_t   handle, int16_t         * restrict y, const int16_t        * restrict x, int N);
void cxfirinterp16x16_process(cxfirinterp16x16_handle_t handle, complex_fract16 * restrict y, const complex_fract16* restrict x, int N);
void firinterp24x24_process  (firinterp24x24_handle_t   handle, f24             * restrict y, const f24            * restrict x, int N);
void firinterp32x16_process  (firinterp32x16_handle_t   handle, int32_t         * restrict y, const int32_t        * restrict x, int N);
void firinterp32x32_process  (firinterp32x32_handle_t   handle, int32_t         * restrict y, const int32_t        * restrict x, int N);
void firinterpf_process      (firinterpf_handle_t       handle, float32_t       * restrict y, const float32_t      * restrict x, int N);

/*-------------------------------------------------------------------------
  Circular Convolution
  Performs circular convolution between vectors x (of length N) and y (of 
  length M)  resulting in vector r of length N.

  Precision: 
  16x16  16x16-bit data, 16-bit outputs
  24x24  24x24-bit data, 24-bit outputs
  32x16  32x16-bit data, 32-bit outputs (both real and complex)
  32x32  32x32-bit data, 32-bit outputs 
  f      floating point 

  Input:
  x[N]          input data (Q31,Q15 or floating point)
  y[M]          input data (Q31,Q15 or floating point)
  N             length of x
  M             length of y
  Output:
  r[N]          output data,Q31,Q15 or floating point

  Restriction:
  x,y,r should not overlap
  x,y,r - aligned on an 8-bytes boundary
  N,M   - multiples of 4 and >0
-------------------------------------------------------------------------*/
void fir_convol16x16 (int16_t* restrict r,
            const int16_t * restrict x, 
            const int16_t * restrict y, 
            int N, int M);
void fir_convol24x24 (f24 * restrict r,
            const f24 * restrict x,
            const f24 * restrict y,
            int N,
            int M);
void fir_convol32x16 ( int32_t * restrict r,
            const int32_t * restrict x,
            const int16_t * restrict y,
            int N,
            int M);
void fir_convol32x32 (int32_t* restrict r,
            const int32_t * restrict x, 
            const int32_t * restrict y, int N, int M);
void fir_convolf ( float32_t * restrict r,
             const float32_t * restrict x,
             const float32_t * restrict y,
             int N,
             int M);
void cxfir_convol32x16(complex_fract32 * restrict r,
  const complex_fract32 * restrict x,
  const complex_fract16 * restrict y,
  int N,
  int M);

/*-------------------------------------------------------------------------
  Circular Convolution
  Performs circular convolution between vectors x (of length N) and y (of 
  length M) resulting in vector r of length N.
  These functions implement the circular convolution algorithm described in
  the previous chapter with no limitations on x and y vectors length and
  alignment at the cost of increased processing complexity. In addition, this
  implementation variant requires scratch memory area.

  Precision: 
  16x16  16x16-bit data, 16-bit outputs
  24x24  24x24-bit data, 24-bit outputs
  32x16  32x16-bit data, 32-bit outputs (both real and complex)
  32x32  32x32-bit data, 32-bit outputs 
  f      floating point 

  Input:
  s[]           Scratch memory,
                FIR_CONVOLA16X16_SCRATCH_SIZE( N, M )  
                FIR_CONVOLA24X24_SCRATCH_SIZE( N, M )   
                FIR_CONVOLA32X16_SCRATCH_SIZE( N, M )  
                CXFIR_CONVOLA32X16_SCRATCH_SIZE( N, M )
                FIR_CONVOLA32X32_SCRATCH_SIZE( N, M )  
                FIR_CONVOLAF_SCRATCH_SIZE( N, M )      
                bytes
  x[N]          input data (Q31,Q15 or floating point)
  y[M]          input data (Q31,Q15 or floating point)
  N             length of x
  M             length of y
  Output:
  r[N]          output data,Q31,Q15 or floating point

  Restriction:
  x,y,r should not overlap
  s        - must be aligned on an 8-bytes boundary
  N,M      - must be >0
  N >= M-1 - minimum allowed length of vector x is the length of y minus one
-------------------------------------------------------------------------*/
#define FIR_CONVOLA16X16_SCRATCH_SIZE( N, M )    ( 2*( 2*(M) + (N) + 6 ) + 6)
#define FIR_CONVOLA24X24_SCRATCH_SIZE( N, M )    ( 4*( 2*(M) + (N) + 6 ) )
#define FIR_CONVOLA32X16_SCRATCH_SIZE( N, M )    ( 4*( (M) + (N) + 2 ) +  2*( (M) + 6 ) )
#define CXFIR_CONVOLA32X16_SCRATCH_SIZE( N, M )  (2*( 4*( (M) + (N) + 2 ) + 2 * ((M)+6)))
#define FIR_CONVOLA32X32_SCRATCH_SIZE( N, M )    ( 4*( 2*(M) + (N) + 6 ) )
#define FIR_CONVOLAF_SCRATCH_SIZE( N, M )        ( 4*( 2*(M) + (N) + 6 ) )

void fir_convola16x16 (void    * restrict s,
                       int16_t * restrict r,
                 const int16_t * restrict x, 
                 const int16_t * restrict y, int N, int M);
void fir_convola24x24 (void * restrict s,
                       f24  * restrict r,
                 const f24  * restrict x,
                 const f24  * restrict y,
                 int N,
                 int M);
void fir_convola32x16 (void    * restrict s,
                       int32_t * restrict r,
                 const int32_t * restrict x,
                 const int16_t * restrict y,
                 int N,
                 int M);
void fir_convola32x32 (void    * restrict s,
                       int32_t * restrict r,
                 const int32_t * restrict x, 
                 const int32_t * restrict y, int N, int M);
void fir_convolaf     (void       * restrict s,
                       float32_t  * restrict r,
                 const float32_t  * restrict x,
                 const float32_t  * restrict y,
                 int N,
                 int M);
void cxfir_convola32x16(void           * restrict s,
                       complex_fract32 * restrict r,
                 const complex_fract32 * restrict x,
                 const complex_fract16 * restrict y,
                 int N,
                 int M);
/*-------------------------------------------------------------------------
  Linear Convolution
  Functions perform linear convolution between vectors x (of length N) and
  y (of length M) resulting in vector r of length N+M-1.

  Precision: 
  16x16   16x16-bit data, 16-bit outputs
  32x16   32x16-bit data, 32-bit outputs 
  32x32   32x32-bit data, 32-bit outputs 
  f       floating point 

  Input:
  s[]           Scratch memory,
                FIR_LCONVOLA16X16_SCRATCH_SIZE( N, M )
                FIR_LCONVOLA32X16_SCRATCH_SIZE( N, M )
                FIR_LCONVOLA32X32_SCRATCH_SIZE( N, M )
                FIR_LCONVOLAF_SCRATCH_SIZE( N, M )    
                bytes
  x[N]          input data (Q31,Q15 or floating point)
  y[M]          input data (Q31,Q15 or floating point)
  N             length of x
  M             length of y
  Output:
  r[N+M-1]      output data,Q31,Q15 or floating point

  Restriction:
  x,y,r should not overlap
  s        - must be aligned on an 8-bytes boundary
  N,M      - must be >0
  N >= M-1 - minimum allowed length of vector x is the length of y minus one
-------------------------------------------------------------------------*/
#define FIR_LCONVOLA16X16_SCRATCH_SIZE( N, M )    ( 2*(N + M + 16) )
#define FIR_LCONVOLA32X16_SCRATCH_SIZE( N, M )    ( 4*(N + 4) + 2*(M + 4) )
#define FIR_LCONVOLA32X32_SCRATCH_SIZE( N, M )    ( 4*(N + M + 9) )
#define FIR_LCONVOLAF_SCRATCH_SIZE( N, M )        ( 4*(M + 7) )

void fir_lconvola16x16 (void    * restrict s,
                       int16_t  * restrict r,
                 const int16_t  * restrict x, 
                 const int16_t  * restrict y, int N, int M);
void fir_lconvola32x16 (void    * restrict s,
                       int32_t  * restrict r,
                 const int32_t  * restrict x, 
                 const int16_t  * restrict y, int N, int M);
void fir_lconvola32x32 (void    * restrict s,
                       int32_t  * restrict r,
                 const int32_t  * restrict x, 
                 const int32_t  * restrict y, int N, int M);
void fir_lconvolaf     (void    * restrict s,
                       float32_t* restrict r,
                 const float32_t* restrict x, 
                 const float32_t* restrict y, int N, int M);


/*-------------------------------------------------------------------------
  Circular Correlation
  Estimates the circular cross-correlation between vectors x (of length N) 
  and y (of length M)  resulting in vector r of length N. It is a similar 
  to correlation but x is read in opposite direction.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  24x24     24x24-bit data, 24-bit outputs
  32x16     32x16-bit data, 32-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  f         floating point (both real and complex data)

  Input:
  x[N]          input data Q31,Q15 or floating point
  y[M]          input data Q31,Q15 or floating point
  N             length of x
  M             length of y
  Output:
  r[N]          output data,Q31,Q15 or floating point

  Restriction:
  x,y,r should not overlap
  x,y,r - aligned on an 8-bytes boundary
  N,M   - multiples of 4 and >0
-------------------------------------------------------------------------*/
void fir_xcorr16x16 (  int16_t  * restrict r,
                 const int16_t  * restrict x, 
                 const int16_t  * restrict y, int N, int M);
void fir_xcorr24x24 (  f24   * restrict r,
                 const f24   * restrict x,
                 const f24   * restrict y,int N,int M);
void fir_xcorr32x16 (int32_t * restrict r,
            const int32_t    * restrict x,
            const int16_t    * restrict y,int N,int M);
void fir_xcorr32x32 (  int32_t  * restrict r,
                 const int32_t  * restrict x, 
                 const int32_t  * restrict y, int N, int M);
void fir_xcorrf (  float32_t * restrict r,
             const float32_t * restrict x,
             const float32_t * restrict y,int N,int M);
void cxfir_xcorrf ( complex_float * restrict r,
             const  complex_float * restrict x,
             const  complex_float * restrict y,int N,int M);
/*-------------------------------------------------------------------------
  Circular Correlation
  Estimates the circular cross-correlation between vectors x (of length N) 
  and y (of length M)  resulting in vector r of length N. It is a similar 
  to correlation but x is read in opposite direction.
  These functions implement the circular correlation algorithm described in
  the previous chapter with no limitations on x and y vectors length and
  alignment at the cost of increased processing complexity. In addition, this
  implementation variant requires scratch memory area.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  24x24     24x24-bit data, 24-bit outputs
  32x16     32x16-bit data, 32-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  f         floating point (both real and complex data)

  Input:
  s[]           Scratch memory, 
                FIR_XCORRA16X16_SCRATCH_SIZE( N, M ) 
                FIR_XCORRA24X24_SCRATCH_SIZE( N, M ) 
                FIR_XCORRA32X16_SCRATCH_SIZE( N, M ) 
                FIR_XCORRA32X32_SCRATCH_SIZE( N, M ) 
                FIR_XCORRAF_SCRATCH_SIZE( N, M )     
                CXFIR_XCORRAF_SCRATCH_SIZE( N, M )   
                bytes
  x[N]          input data Q31, Q15  or floating point
  y[M]          input data Q31, Q15 or floating point
  N             length of x
  M             length of y
  Output:
  r[N]          output data, Q31, Q15  or floating point

  Restrictions:
  x,y,r should not overlap
  s        - must be aligned on an 8-bytes boundary
  N,M      - must be >0
  N >= M-1 - minimum allowed length of vector x is the length of y minus one
-------------------------------------------------------------------------*/
#define FIR_XCORRA16X16_SCRATCH_SIZE( N, M )      ( 2*( 2*(M) + (N) + 6 ) + 6)
#define FIR_XCORRA24X24_SCRATCH_SIZE( N, M )      ( 4*( 2*(M) + (N) + 6 ) )
#define FIR_XCORRA32X16_SCRATCH_SIZE( N, M )      ( 4*( (M) + (N) + 2 ) + 2*( (M) + 6 ) )
#define FIR_XCORRA32X32_SCRATCH_SIZE( N, M )      ( 4*( 2*(M) + (N) + 6 ) )
#define FIR_XCORRAF_SCRATCH_SIZE( N, M )          ( 4*(M+N-1) )
#define CXFIR_XCORRAF_SCRATCH_SIZE( N, M )        ( 0 )

void fir_xcorra16x16 ( void     * restrict s,
                       int16_t  * restrict r,
                 const int16_t  * restrict x, 
                 const int16_t  * restrict y, int N, int M);
void fir_xcorra24x24 (void * restrict s,
                       f24 * restrict r,
                 const f24 * restrict x,
                 const f24 * restrict y,
                 int N,
                 int M);
void fir_xcorra32x16 (void    * restrict s,
                      int32_t * restrict r,
                const int32_t * restrict x,
                const int16_t * restrict y,
                int N,
                int M);
void fir_xcorra32x32 ( void     * restrict s,
                       int32_t  * restrict r,
                 const int32_t  * restrict x, 
                 const int32_t  * restrict y, int N, int M);
void fir_xcorraf      (void      * restrict s,
                       float32_t * restrict r,
                 const float32_t * restrict x,
                 const float32_t * restrict y,
                 int N,
                 int M);
void cxfir_xcorraf    (void      * restrict s,
                       complex_float * restrict r,
                 const complex_float * restrict x,
                 const complex_float * restrict y,
                 int N,
                 int M);
/*-------------------------------------------------------------------------
  Linear Correlation
  Functions estimate the linear cross-correlation between vectors x (of 
  length N) and y (of length M) resulting in vector r of length N+M-1. It 
  is similar to convolution but y is read in opposite direction.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  24x24     24x24-bit data, 24-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  f         floating point (both real and complex data)

  Input:
  s[]           Scratch memory, 
                FIR_LXCORRA16X16_SCRATCH_SIZE(N, M) 
                FIR_LXCORRA32X16_SCRATCH_SIZE(N, M) 
                FIR_LXCORRA32X32_SCRATCH_SIZE(N, M) 
                FIR_LXCORRAF_SCRATCH_SIZE(N, M)     
                bytes
  x[N]          input data Q31, Q15  or floating point
  y[M]          input data Q31, Q15 or floating point
  N             length of x
  M             length of y
  Output:
  r[N+M-1]      output data, Q31, Q15  or floating point

  Restrictions:
  x,y,r,s should not overlap
  s should be aligned on 8-byte boundary
  N>0, M>0
  N>=M-1
-------------------------------------------------------------------------*/
#define FIR_LXCORRA16X16_SCRATCH_SIZE(N, M)      ( 2*(N + M + 16))
#define FIR_LXCORRA32X16_SCRATCH_SIZE(N, M)      ( 4*(N + 4) + 2*(M + 4))
#define FIR_LXCORRA32X32_SCRATCH_SIZE(N, M)      ( 4*(N + M + 9))
#define FIR_LXCORRAF_SCRATCH_SIZE(N, M)          ( 4*(M + 7) )

void fir_lxcorra16x16 ( void    * restrict s,
                       int16_t  * restrict r,
                 const int16_t  * restrict x, 
                 const int16_t  * restrict y, int N, int M);
void fir_lxcorra32x16 (void     * restrict s,
                      int32_t   * restrict r,
                const int32_t   * restrict x, 
                const int16_t   * restrict y, int N, int M);
void fir_lxcorra32x32 ( void    * restrict s,
                       int32_t  * restrict r,
                 const int32_t  * restrict x,
                 const int32_t  * restrict y, int N, int M);
void fir_lxcorraf      (void    * restrict s,
                       float32_t* restrict r,
                 const float32_t* restrict x, 
                 const float32_t* restrict y, int N, int M);

/*-------------------------------------------------------------------------
  Circular Autocorrelation 
  Estimates the auto-correlation of vector x. Returns autocorrelation of 
  length N.

  Precision: 
  16x16   16-bit data, 16-bit outputs
  24x24   24-bit data, 24-bit outputs
  32x32   32-bit data, 32-bit outputs
  f       floating point

  Input:
  x[N]      input data, Q31, Q15 or floating point
  N         length of x
  Output:
  r[N]      output data, Q31, Q15 or floating point

  Restrictions:
  x,r should not overlap
  x,r - aligned on an 8-bytes boundary
  N   - multiple of 4 and >0
-------------------------------------------------------------------------*/

void fir_acorr16x16 (   int16_t *   restrict r, const int16_t *  restrict x, int N);
void fir_acorr24x24 (   f24    *    restrict r, const f24     *  restrict x, int N);
void fir_acorr32x32 (   int32_t *   restrict r, const int32_t *  restrict x, int N);
void fir_acorrf (       float32_t*  restrict r, const float32_t* restrict x, int N);
 
/*-------------------------------------------------------------------------
  Circular Autocorrelation 
  Estimates the auto-correlation of vector x. Returns autocorrelation of 
  length N.
  These functions implement the circular autocorrelation algorithm described
  in the previous chapter with no limitations on x vector length and
  alignment at the cost of increased processing complexity. In addition, this
  implementation variant requires scratch memory area.

  Precision: 
  16x16   16-bit data, 16-bit outputs
  24x24   24-bit data, 24-bit outputs
  32x32   32-bit data, 32-bit outputs
  f       floating point

  Input:
  s[]       scratch area of
            FIR_ACORRA16X16_SCRATCH_SIZE( N )
            FIR_ACORRA24X24_SCRATCH_SIZE( N )
            FIR_ACORRA32X32_SCRATCH_SIZE( N )
            FIR_ACORRAF_SCRATCH_SIZE( N )    
            bytes
  x[N]      input data Q31, Q15 or floating point
  N         length of x
  Output:
  r[N]      output data, Q31, Q15 or floating point

  Restrictions:
  x,r,s should not overlap
  N >0
  s   - aligned on an 8-bytes boundary
-------------------------------------------------------------------------*/
#define FIR_ACORRA16X16_SCRATCH_SIZE( N )       2*( 3*(N) + 12+2)
#define FIR_ACORRA24X24_SCRATCH_SIZE( N )       4*( 2*(N) + 3 )
#define FIR_ACORRA32X32_SCRATCH_SIZE( N )       4*( 2*(N) + 3 )
#define FIR_ACORRAF_SCRATCH_SIZE( N )           4*(2*N-1)

void fir_acorra16x16 (void           * restrict s,
                      int16_t        * restrict r, 
                      const int16_t  * restrict x, int N);
void fir_acorra24x24 ( void * restrict s,
                       f24  * restrict r,
                 const f24  * restrict x,
                 int N );
void fir_acorra32x32 (void           * restrict s,
                      int32_t        * restrict r,
                      const int32_t  * restrict x, int N);
void fir_acorraf     ( void       * restrict s,
                       float32_t  * restrict r,
                 const float32_t  * restrict x,
                 int N );

/*-------------------------------------------------------------------------
  Linear Autocorrelation 
  Functions estimate the linear auto-correlation of vector x. Returns 
  autocorrelation of length N.

  Precision: 
  16x16   16-bit data, 16-bit outputs
  32x32   32-bit data, 32-bit outputs
  f       floating point

  Input:
  s[]       scratch area of
            FIR_LACORRA16X16_SCRATCH_SIZE( N )
            FIR_LACORRA32X32_SCRATCH_SIZE( N )
            FIR_LACORRAF_SCRATCH_SIZE( N )    
            bytes
  x[N]      input data Q31, Q15 or floating point
  N         length of x
  Output:
  r[N]      output data, Q31, Q15 or floating point

  Restrictions:
  x,r,s should not overlap
  N >0
  s   - aligned on an 8-bytes boundary
-------------------------------------------------------------------------*/
#define FIR_LACORRA16X16_SCRATCH_SIZE( N )       ( 2*(N + 6))
#define FIR_LACORRA32X32_SCRATCH_SIZE( N )       ( 4*(N + 4))
#define FIR_LACORRAF_SCRATCH_SIZE( N )           ( 0 )
void fir_lacorra16x16 (void* restrict s, int16_t  * restrict r, const int16_t  * restrict x, int N);
void fir_lacorra32x32 (void* restrict s, int32_t  * restrict r, const int32_t  * restrict x, int N);
void fir_lacorraf     (void* restrict s, float32_t* restrict r, const float32_t* restrict x, int N);

/*-------------------------------------------------------------------------
  Blockwise Adaptive LMS Algorithm for Real Data
  Blockwise LMS algorithm performs filtering of input samples x[N+M-1],
  computation of error e[N] over a block of reference data r[N] and makes
  blockwise update of IR to minimize the error output.
  Algorithm includes FIR filtering, calculation of correlation between the 
  error output e[N] and reference signal x[N+M-1] and IR taps update based
  on that correlation.
NOTES: 
  1. The algorithm must be provided with the normalization factor, which is
     the power of the input signal times N - the number of samples in a
     data block. This can be calculated using the vec_power24x24() or 
     vec_power16x16() function. In order to avoid the saturation of the 
     normalization factor, it may be biased, i.e. shifted to the right.
     If it's the case, then the adaptation coefficient must be also shifted
     to the right by the same number of bit positions.
  2. this algorithm consumes less CPU cycles per block than single 
     sample algorithm at similar convergence rate.
  3. Right selection of N depends on the change rate of impulse response:
     on static or slow varying channels convergence rate depends on
     selected mu and M, but not on N.
  4. 16x16 routine may converge slower on small errors due to roundoff 
     errors. In that cases, 16x32 rountine will give better results although 
     convergence rate on bigger errors is the same

  Precision: 
  16x16   16-bit coefficients, 16-bit data, 16-bit output
  24x24   24-bit coefficients, 24-bit data, 32-bit output
  16x32   32-bit coefficients, 16-bit data, 16-bit output
  32x32   32-bit coefficients, 32-bit data, 32-bit output
  f       floating point
  Input:
  h[M]     impulse response, Q15, Q31 or floating point
  r[N]     reference data vector (near end). First in time value is in 
           r[0], 24-bit or 16-bit, Qx or floating point
  x[N+M-1] input vector (far end). First in time value is in x[0],  
           24-bit or 16-bit, Qx or floating point
  norm     normalization factor: power of signal multiplied by N, Q15, 
           Q31 or floating point
           Fixed-point format for the 24x24-bit variant: Q(2*x-31-bias)
           Fixed-point format for the 32x16-bit variant: Q(2*x+1-bias)
  mu       adaptation coefficient (LMS step), Q(31-bias) or Q(15-bias)
  N        length of data block
  M        length of h
  Output:
  e[N]     estimated error, Q31,Q15 or floating point
  h[M]     updated impulse response, Q15, Q31 or floating point

  Restriction:
  x,r,h,e - should not overlap
  x,r,h,e - aligned on a 8-bytes boundary
  N,M     - multiples of 8 and >0
-------------------------------------------------------------------------*/
void fir_blms16x16 (  int16_t*  restrict e, int16_t * restrict h,
                const int16_t * restrict r,
                const int16_t * restrict x,
                int16_t norm, int16_t   mu,
                int   N,   int   M);
void fir_blms24x24 (  f24 * restrict e, f24 * restrict h,
                const f24 * restrict r,
                const f24 * restrict x,
                f24   norm,f24   mu,
                int   N,   int   M);
void fir_blms16x32 (  int32_t * restrict e, int32_t * restrict h,
                const int16_t * restrict r,
                const int16_t * restrict x,
                int32_t   norm,int16_t   mu,
                int       N,   int       M);
void fir_blms32x32 (  int32_t * restrict e, int32_t * restrict h,
                const int32_t * restrict r,
                const int32_t * restrict x,
                int32_t   norm, int32_t mu,
                int       N,   int       M);
void fir_blmsf      ( float32_t * e, float32_t * h, const float32_t * r,
                const float32_t * x, 
                float32_t norm, float32_t mu, 
                int          N, int       M );

/*===========================================================================
  IIR filters:
  bqxiir     Biquad block IIR
  latr       Lattice block Real IIR
===========================================================================*/

/*-------------------------------------------------------------------------
  Bi-quad Block IIR
  Computes a IIR filter (cascaded IIR direct form I or II using 5 
  coefficients per bi-quad + gain term) . Input data are stored in vector x. 
  Filter output samples are stored in vector r. The filter calculates N output 
  samples using SOS and G matrices.
  Filters are able to process data in following formats:
  -  real (just array of samples)
  -  2-way or complex (interleaved real/imaginary samples)
  -  3-way (stream of interleaved samples from 3 channels)
  The same coefficients are used for filtering of multiple channels or 
  real/imaginary parts and they are processed independently. 
  The same format have to be used both for input and output streams.
  NOTES:
  1. Bi-quad coefficients may be derived from standard SOS and G matrices
     generated by MATLAB. However, typically biquad stages have big peaks 
     in their step response which may cause undesirable overflows at the 
     intermediate outputs. To avoid that the additional scale factors 
     coef_g[M] may be applied. These per-section scale factors may require 
     some tuning to find a compromise between quantization noise and possible
     overflows. Output of the last section is directed to an additional 
     multiplier, with the gain factor being a power of two, either negative 
     or non-negative. It is specified through the total gain shift amount 
     parameter gain of each filter initialization function.
  2. 16x16 filters may suffer more from accumulation of the roundoff errors, 
     so filters should be properly designed to match noise requirements

  Precision: 
  16x16  16-bit data, 16-bit coefficients, 16-bit intermediate stage outputs 
         (DF I, DF II), real data
  16x16  16-bit data, 16-bit coefficients, 16-bit intermediate stage outputs 
         (DF I, DF II), 3-way data
  24x24  32-bit data, 24-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II), real data
  32x16  32-bit data, 16-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II), real data
  32x16  32-bit data, 16-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II), 3-way data
  32x32  32-bit data, 32-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II)
  32x32  32-bit data, 32-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II) 3-way data
  f      floating point (DF I, DF II and DF IIt)
  f      floating point (DF I), 2-way (complex) data
  f      floating point (DF I, DF II) 3-way data

   ----------------+--------------------------------
   Functon         | Scratch memory, bytes
   ----------------+--------------------------------
   bqriir16x16_df1 | BQRIIR16X16_DF1_SCRATCH_SIZE(M)
   bqriir16x16_df2 | BQRIIR16X16_DF2_SCRATCH_SIZE(M)
   bq3iir16x16_df1 | BQ3IIR16X16_DF1_SCRATCH_SIZE(M)
   bq3iir16x16_df2 | BQ3IIR16X16_DF2_SCRATCH_SIZE(M)
   bqriir24x24_df1 | BQRIIR24X24_DF1_SCRATCH_SIZE(M)
   bqriir24x24_df2 | BQRIIR24X24_DF2_SCRATCH_SIZE(M)
   bqriir32x16_df1 | BQRIIR32X16_DF1_SCRATCH_SIZE(M)
   bqriir32x16_df2 | BQRIIR32X16_DF2_SCRATCH_SIZE(M)
   bq3iir32x16_df1 | BQ3IIR32X16_DF1_SCRATCH_SIZE(M)
   bq3iir32x16_df2 | BQ3IIR32X16_DF2_SCRATCH_SIZE(M)
   bqriir32x32_df1 | BQRIIR32X32_DF1_SCRATCH_SIZE(M)
   bqriir32x32_df2 | BQRIIR32X32_DF2_SCRATCH_SIZE(M)
   bq3iir32x32_df1 | BQ3IIR32X32_DF1_SCRATCH_SIZE(M)
   bq3iir32x32_df2 | BQ3IIR32X32_DF2_SCRATCH_SIZE(M)
   ----------------+--------------------------------
  Input:
  N             length of input sample block. For 3-way functions (bq3iirxxx), 
                N is a number of triplets
  M             number of bi-quad sections
  s[]           scratch memory area (for fixed-point functions only), Minimum 
                number of bytes depends on selected filter structure and 
                precision. see table above .If a particular macro returns zero, 
                then the corresponding IIR doesn't require a scratch area and 
                parameter s may hold zero

  coef_sos[M*5] filter coefficients stored in blocks of 5 numbers: 
                b0 b1 b2 a1 a2. 
                For fixed-point funcions, fixed point format of filter 
                coefficients is Q1.14 for 32x16, or Q1.30 for 32x16 and 
                24x24 (in the latter case 8 LSBs are actually ignored). 
  coef_g[M]     scale factor for each section, Q15 (for fixed-point 
                functions only). Please note that 24x24 DFI implementation 
                internally truncates scale factors to Q7 values.
  gain          total gain shift amount applied to output signal of the
                last section, -48..15
  x[N]          input samples, Q31, Q15 or floating point. For 3-way functions 
                (bq3iirxxx), N is a number of triplets, so array size should be 
                3*N.

  Output:
  r[N]          output data, Q31, Q15 or floating point. For 3-way functions 
                (bq3iirxxx), N is a number of triplets, so array size should be 
                3*N.

  Restriction:
  x,r,s,coef_g,coef_sos should not overlap
  N   - must be a multiple of 2
  s[] - whenever supplied must be aligned on an 8-bytes boundary

-------------------------------------------------------------------------*/
#define BQRIIR16X16_DF1_SCRATCH_SIZE(N,M)    ( 0 )
#define BQRIIR16X16_DF2_SCRATCH_SIZE(N,M)    ( 0 )
#define BQ3IIR16X16_DF1_SCRATCH_SIZE(N,M)    ( 0 )
#define BQ3IIR16X16_DF2_SCRATCH_SIZE(N,M)    ( 0 )
#define BQRIIR24X24_DF1_SCRATCH_SIZE(N,M)    ( 4*(N) )
#define BQRIIR24X24_DF2_SCRATCH_SIZE(N,M)    ( 4*(N) )
#define BQRIIR32X16_DF1_SCRATCH_SIZE(N,M)    ( 4*(N) )
#define BQRIIR32X16_DF2_SCRATCH_SIZE(N,M)    ( 4*(N) )
#define BQRIIR32X32_DF1_SCRATCH_SIZE(N,M)    ( 0 )
#define BQRIIR32X32_DF2_SCRATCH_SIZE(N,M)    ( 4*(N) )
#define BQ3IIR32X16_DF1_SCRATCH_SIZE(N,M)    ( 0 )
#define BQ3IIR32X16_DF2_SCRATCH_SIZE(N,M)    ( 0 )
#define BQ3IIR32X32_DF1_SCRATCH_SIZE(N,M)    ( 0 )
#define BQ3IIR32X32_DF2_SCRATCH_SIZE(N,M)    ( 0 )

typedef void* bqriir16x16_df1_handle_t;
typedef void* bqriir16x16_df2_handle_t;
typedef void* bq3iir16x16_df1_handle_t;
typedef void* bq3iir16x16_df2_handle_t;
typedef void* bqriir24x24_df1_handle_t;
typedef void* bqriir24x24_df2_handle_t;
typedef void* bqriir32x16_df1_handle_t;
typedef void* bqriir32x16_df2_handle_t;
typedef void* bq3iir32x16_df1_handle_t; 
typedef void* bq3iir32x16_df2_handle_t;
typedef void* bqriir32x32_df1_handle_t; 
typedef void* bqriir32x32_df2_handle_t;
typedef void* bq3iir32x32_df1_handle_t; 
typedef void* bq3iir32x32_df2_handle_t; 
typedef void* bqriirf_df1_handle_t;
typedef void* bqriirf_df2_handle_t;
typedef void* bqriirf_df2t_handle_t;
typedef void* bqciirf_df1_handle_t;
typedef void* bq3iirf_df1_handle_t;
typedef void* bq3iirf_df2_handle_t;

// Returns: size of memory in bytes to be allocated
size_t bqriir16x16_df1_alloc(int M);
size_t bqriir16x16_df2_alloc(int M);
size_t bq3iir16x16_df1_alloc(int M);
size_t bq3iir16x16_df2_alloc(int M);
size_t bqriir24x24_df1_alloc(int M);
size_t bqriir24x24_df2_alloc(int M);
size_t bqriir32x16_df1_alloc(int M);
size_t bqriir32x16_df2_alloc(int M);
size_t bq3iir32x16_df1_alloc(int M);
size_t bq3iir32x16_df2_alloc(int M);
size_t bqriir32x32_df1_alloc(int M);
size_t bqriir32x32_df2_alloc(int M);
size_t bq3iir32x32_df1_alloc(int M);
size_t bq3iir32x32_df2_alloc(int M);
size_t bqriirf_df1_alloc(int M);
size_t bqriirf_df2_alloc(int M);
size_t bqriirf_df2t_alloc(int M);
size_t bqciirf_df1_alloc(int M);
size_t bq3iirf_df1_alloc(int M);
size_t bq3iirf_df2_alloc(int M);


// Returns: handle to the object
bqriir16x16_df1_handle_t bqriir16x16_df1_init(void * objmem, int M,
           const int16_t * coef_sos, const int16_t * coef_g, int16_t gain );
bqriir16x16_df2_handle_t bqriir16x16_df2_init(void * objmem, int M,
           const int16_t * coef_sos, const int16_t * coef_g,  int16_t gain);
bq3iir16x16_df1_handle_t bq3iir16x16_df1_init(void * objmem, int M,
           const int16_t * coef_sos, const int16_t * coef_g, int16_t gain );
bq3iir16x16_df2_handle_t bq3iir16x16_df2_init(void * objmem, int M,
           const int16_t * coef_sos, const int16_t * coef_g,  int16_t gain);
bqriir24x24_df1_handle_t bqriir24x24_df1_init(void * objmem, int M,
           const f24     * coef_sos, const int16_t * coef_g, int16_t gain );
bqriir24x24_df2_handle_t bqriir24x24_df2_init(void * objmem, int M,
           const f24     * coef_sos,const int16_t * coef_g,int16_t gain);
bqriir32x16_df1_handle_t bqriir32x16_df1_init(void * objmem, int M, 
           const int16_t * coef_sos, const int16_t * coef_g, int16_t gain);
bqriir32x16_df2_handle_t bqriir32x16_df2_init(void * objmem, int M,
           const int16_t * coef_sos, const int16_t * coef_g, int16_t gain);
bq3iir32x16_df1_handle_t bq3iir32x16_df1_init(void * objmem, int M, 
           const int16_t * coef_sos, const int16_t * coef_g, int16_t gain);
bq3iir32x16_df2_handle_t bq3iir32x16_df2_init(void * objmem, int M,
           const int16_t * coef_sos, const int16_t * coef_g, int16_t gain);
bqriir32x32_df1_handle_t bqriir32x32_df1_init(void * objmem, int M, 
           const int32_t * coef_sos, const int16_t * coef_g, int16_t gain);
bqriir32x32_df2_handle_t bqriir32x32_df2_init(void * objmem, int M, 
           const int32_t * coef_sos, const int16_t * coef_g, int16_t gain);
bq3iir32x32_df1_handle_t bq3iir32x32_df1_init(void * objmem, int M, 
           const int32_t * coef_sos, const int16_t * coef_g, int16_t gain);
bq3iir32x32_df2_handle_t bq3iir32x32_df2_init(void * objmem, int M, 
           const int32_t * coef_sos, const int16_t * coef_g, int16_t gain);
bqriirf_df1_handle_t bqriirf_df1_init(void * objmem, int M,
           const float32_t* coef_sos, int16_t gain );
bqriirf_df2_handle_t bqriirf_df2_init(void * objmem, int M,
           const float32_t * coef_sos, int16_t gain);
bqriirf_df2t_handle_t bqriirf_df2t_init(void * objmem, int M, 
           const float32_t * coef_sos, int16_t gain);
bqciirf_df1_handle_t bqciirf_df1_init(void * objmem, int M,
           const float32_t * coef_sos, int16_t gain);
bq3iirf_df1_handle_t bq3iirf_df1_init(void * objmem, int M,
           const float32_t* coef_sos, int16_t gain );
bq3iirf_df2_handle_t bq3iirf_df2_init(void * objmem, int M,
           const float32_t * coef_sos, int16_t gain);

// Update the delay line and compute filter output
void bqriir16x16_df1(bqriir16x16_df1_handle_t _bqriir,void * restrict s,int16_t *  restrict  r,const int16_t   *x, int N);
void bqriir16x16_df2(bqriir16x16_df2_handle_t _bqriir,void * restrict s,int16_t *  restrict  r,const int16_t   *x, int N);
void bq3iir16x16_df1(bq3iir16x16_df1_handle_t _bqriir,void * restrict s,int16_t *  restrict  r,const int16_t   *x, int N);
void bq3iir16x16_df2(bq3iir16x16_df2_handle_t _bqriir,void * restrict s,int16_t *  restrict  r,const int16_t   *x, int N);
void bqriir24x24_df1(bqriir24x24_df1_handle_t _bqriir,void * restrict s,int32_t *  restrict  r,const int32_t   *x, int N);
void bqriir24x24_df2(bqriir24x24_df2_handle_t _bqriir,void * restrict s,int32_t *  restrict  r,const int32_t   *x, int N);
void bqriir32x16_df1(bqriir32x16_df1_handle_t _bqriir,void * restrict s,int32_t *  restrict  r,const int32_t   *x, int N);
void bqriir32x16_df2(bqriir32x16_df2_handle_t _bqriir,void * restrict s,int32_t *  restrict  r,const int32_t   *x, int N);
void bq3iir32x16_df1(bq3iir32x16_df1_handle_t _bqriir,void * restrict s,int32_t *  restrict  r,const int32_t   *x, int N);
void bq3iir32x16_df2(bq3iir32x16_df2_handle_t _bqriir,void * restrict s,int32_t *  restrict  r,const int32_t   *x, int N);
void bqriir32x32_df1(bqriir32x32_df1_handle_t _bqriir,void * restrict s,int32_t *  restrict  r,const int32_t   *x, int N);
void bqriir32x32_df2(bqriir32x32_df2_handle_t _bqriir,void * restrict s,int32_t *  restrict  r,const int32_t   *x, int N);
void bqriirf_df1    (bqriirf_df1_handle_t     _bqriir,                  float32_t* restrict  r,const float32_t *x, int N);
void bq3iir32x32_df1(bq3iir32x32_df1_handle_t _bqriir,void * restrict s,int32_t *  restrict  r,const int32_t   *x, int N);
void bq3iir32x32_df2(bq3iir32x32_df2_handle_t _bqriir,void * restrict s,int32_t *  restrict  r,const int32_t   *x, int N);
void bqriirf_df2    (bqriirf_df2_handle_t     _bqriir,                  float32_t     * restrict r, const float32_t     * x, int N);
void bqriirf_df2t   (bqriirf_df2t_handle_t    _bqriir,                  float32_t     * restrict r, const float32_t     * x, int N);
void bqciirf_df1    (bqciirf_df1_handle_t     _bqriir,                  complex_float * restrict r, const complex_float * x, int N);
void bq3iirf_df1    (bq3iirf_df1_handle_t     _bqriir,                  float32_t     * restrict r, const float32_t     * x, int N);
void bq3iirf_df2    (bq3iirf_df2_handle_t     _bqriir,                  float32_t     * restrict r, const float32_t     * x, int N);

/*-------------------------------------------------------------------------
  Lattice Block Real IIR
  Computes a real cascaded lattice autoregressive IIR filter using reflection 
  coefficients stored in vector k. The real data input are stored in vector x.
  The filter output result is stored in vector r.  Input scaling is done before 
  the first cascade for normalization and overflow protection..

  Precision: 
  16x16  16-bit data, 16-bit coefficients
  24x24  24-bit data, 24-bit coefficients
  32x16  32-bit data, 16-bit coefficients
  32x32  32-bit data, 32-bit coefficients
  f      single precision floating point

  Input:
  N      length of input sample block
  M      number of reflection coefficients
  scale  input scale factor g, Q31, Q15 or floating point
  k[M]   reflection coefficients, Q31, Q15 or floating point
  x[N]   input samples, Q31, Q15 or floating point
  Output:
  r[N]   output data, Q31, Q15 or floating point

  Restriction:
  x,r,k should not overlap

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  M - from the range 1...8
-------------------------------------------------------------------------*/
typedef void* latr16x16_handle_t;
typedef void* latr24x24_handle_t;
typedef void* latr32x16_handle_t;
typedef void* latr32x32_handle_t;
typedef void* latrf_handle_t;

// Returns: size of memory in bytes to be allocated
size_t latr16x16_alloc(int M);
size_t latr24x24_alloc(int M);
size_t latr32x16_alloc(int M);
size_t latr32x32_alloc(int M);
size_t latrf_alloc    (int M);

// Returns: handle to the object
latr16x16_handle_t latr16x16_init(void * objmem, int M,const int16_t    * restrict k, int16_t   scale);
latr24x24_handle_t latr24x24_init(void * objmem, int M,const f24        * restrict k, f24       scale);
latr32x16_handle_t latr32x16_init(void * objmem, int M, const int16_t   * restrict k, int16_t   scale);
latr32x32_handle_t latr32x32_init(void * objmem, int M, const int32_t   * restrict k, int32_t   scale);
latrf_handle_t     latrf_init    (void * objmem, int M, const float32_t * restrict k, float32_t scale);

// Update the delay line and compute filter output
void latr16x16_process (latr16x16_handle_t handle, int16_t   * r, const int16_t   * x, int N);
void latr24x24_process (latr24x24_handle_t handle, f24       * r, const f24       * x, int N);
void latr32x16_process (latr32x16_handle_t handle, int32_t   * r, const int32_t   * x, int N);
void latr32x32_process (latr32x32_handle_t handle, int32_t   * r, const int32_t   * x, int N);
void latrf_process     (latrf_handle_t     handle, float32_t * r, const float32_t * x, int N);

/*===========================================================================
  Vector matematics:
  vec_dot              Vector Dot Product
  vec_add              Vector Sum
  vec_power            Power of a Vector
  vec_shift,vec_scale  Vector Scaling with Saturation
  vec_recip            Reciprocal on Q31/Q15 Numbers
  vec_divide           Division of Q31/Q15 Numbers
  vec_log              Logarithm 
  vec_antilog          Antilogarithm         
  vec_sqrt             Square Root
  vec_sine,vec_cosine  Sine/Cosine
  vec_tan              Tangent 
  vec_atan             Arctangent 
  vec_bexp             Common Exponent
  vec_min,vec_max      Vector Min/Max
  vec_poly             Vector polynomial approximation
===========================================================================*/

/*-------------------------------------------------------------------------
  Vector Dot product
  These routines take two vectors and calculates their dot product. Two 
  versions of routines are available: regular versions (vec_dot24x24, 
  vec_dot32x16, vec_dot32x32, vec_dot16x16, vec_dotf) work with arbitrary 
  arguments, faster versions (vec_dot24x24_fast, vec_dot32x16_fast, 
  vec_dot32x32_fast, vec_dot16x16_fast) apply some restrictions.

  Precision: 
  16x16  16x16-bit data, 64-bit output for regular version and 32-bit for 
         fast version
  24x24  24x24-bit data, 64-bit output
  32x16  32x16-bit data, 64-bit output
  32x32  32x32-bit data, 64-bit output
  f      floating point

  Input:
  x[N]  input data, Q31 or floating point
  y[N]  input data, Q31, Q15, or floating point
  N	length of vectors
  Returns: dot product of all data pairs, Q31 or floating point

  Restrictions:
  Regular versions:
  None
  Faster versions:
  x,y - aligned on 8-byte boundary
  N   - multiple of 4

  vec_dot16x16_fast utilizes 32-bit saturating accumulator, so, input data 
  should be scaled properly to avoid erroneous results
-------------------------------------------------------------------------*/
int64_t vec_dot24x24 (const f24 * restrict x,const f24 * restrict y,int N);
int64_t vec_dot32x16 (const int32_t * restrict x,const int16_t * restrict y,int N);
int64_t vec_dot16x16 (const int16_t * restrict x,const int16_t * restrict y,int N);
int64_t vec_dot32x32 (const int32_t * restrict  x, const int32_t * restrict  y, int N);
float32_t vec_dotf   (const float32_t * restrict x,const float32_t * restrict y,int N);

int64_t vec_dot24x24_fast (const f24 * restrict x,const f24 * restrict y,int N);
int64_t vec_dot32x16_fast (const int32_t * restrict x,const int16_t * restrict y,int N);
int64_t vec_dot32x32_fast (const int32_t * restrict  x, const int32_t * restrict  y, int N);
int32_t vec_dot16x16_fast (const int16_t * restrict x,const int16_t * restrict y,int N);

/*-------------------------------------------------------------------------
  Vector Sum
  This routine makes pair wise saturated summation of vectors.
  Two versions of routines are available: regular versions (vec_add32x32, 
  vec_add24x24, vec_add16x16, vec_addf) work with arbitrary arguments, 
  faster versions (vec_add32x32_fast, vec_add24x24_fast, vec_add16x16_fast) 
  apply some restrictions.

  Precision: 
  32x32 32-bit inputs, 32-bit output
  24x24 24-bit inputs, 24-bit output
  16x16 16-bit inputs, 16-bit output
  f     single precision floating point

  Input:
  x[N]   input data
  y[N]   input data
  N      length of vectors
  Output:
  z[N]   output data

  Restriction:
  Regular versions (vec_add32x32, vec_add24x24, vec_add16x16, vec_addf):
  x,y,z - should not be overlapped
  Faster versions (vec_add32x32_fast, vec_add24x24_fast, vec_add16x16_fast):
  z,x,y - aligned on 8-byte boundary
  N   - multiple of 4
-------------------------------------------------------------------------*/
void vec_add32x32 ( int32_t * restrict z,
              const int32_t * restrict x,
              const int32_t * restrict y,
              int N);
void vec_add32x32_fast
            ( int32_t * restrict z,
              const int32_t * restrict x,
              const int32_t * restrict y,
              int N );
void vec_add24x24 ( f24 * restrict z,
              const f24 * restrict x,
              const f24 * restrict y,
              int N);
void vec_addf ( float32_t * restrict z,
              const float32_t * restrict x,
              const float32_t * restrict y,
              int N);

void vec_add24x24_fast
            ( f24 * restrict z,
              const f24 * restrict x,
              const f24 * restrict y,
              int N );
void vec_add16x16 ( int16_t * restrict z,
              const int16_t * restrict x,
              const int16_t * restrict y,
              int N);
void vec_add16x16_fast
            ( int16_t * restrict z,
              const int16_t * restrict x,
              const int16_t * restrict y,
              int N );

/*-------------------------------------------------------------------------
  Power of a Vector
  These routines compute power of vector with scaling output result by rsh 
  bits. Fixed point rountines make accumulation in the 64-bit wide 
  accumulator and output may scaled down with saturation by rsh bits. 
  So, if representation of x input is Qx, result will be represented in 
  Q(2x-rsh) format.
  Two versions of routines are available: regular versions (vec_power24x24, 
  vec_power32x32, vec_power16x16, vec_powerf) work with arbitrary arguments, 
  faster versions (vec_power24x24_fast, vec_power32x32_fast, 
  vec_power16x16_fast) apply some restrictions.

  Precision: 
  24x24 24x24-bit data, 64-bit output
  32x32 32x32-bit data, 64-bit output
  16x16 16x16-bit data, 64-bit output
  f     single precision floating point

  Input:
  x[N]  input data, Q31, Q15 or floating point
  rsh   right shift of result
  N     length of vector

  Returns: Sum of squares of a vector, Q(2x-rsh)

  Restrictions:
  for vec_power32x32(): rsh in range 31...62
  for vec_power24x24(): rsh in range 15...46
  for vec_power16x16(): rsh in range 0...31
  For regular versions (vec_power24x24, vec_power32x32, 
  vec_power16x16, vec_powerf):
  none

  For faster versions (vec_power24x24_fast, 
  vec_power32x32_fast, vec_power16x16_fast ):
  x - aligned on 8-byte boundary
  N   - multiple of 4
-------------------------------------------------------------------------*/
int64_t     vec_power24x24 ( const f24 * restrict x,int rsh,int N);
int64_t     vec_power32x32 ( const int32_t * restrict x,int rsh,int N);
int64_t     vec_power16x16 ( const int16_t * restrict x,int rsh,int N);
float32_t   vec_powerf     ( const float32_t * restrict x,int N);

int64_t     vec_power24x24_fast ( const f24 * restrict x,int rsh,int N);
int64_t     vec_power32x32_fast ( const int32_t * restrict x,int rsh,int N);
int64_t     vec_power16x16_fast ( const int16_t * restrict x,int rsh,int N);

/*-------------------------------------------------------------------------
  Vector Scaling with Saturation
  These routines make shift with saturation of data values in the vector 
  by given scale factor (degree of 2). 24-bit routine works with f24 data 
  type and faster while 32-bit version keep all 32-bits and slower.
  Functions vec_scale() make multiplication of Q31 vector to Q31 coefficient 
  which is not a power of 2 forming Q31 result.
  Two versions of routines are available: regular versions (vec_shift24x24, 
  vec_shift32x32, vec_shift16x16, vec_shiftf, vec_scale32x24, vec_scale24x24, 
  vec_scale16x16, vec_scalef, vec_scale_sf) work with arbitrary arguments, faster 
  versions (vec_shift24x24_fast, vec_shift32x32_fast, vec_shift16x16_fast, 
  vec_scale32x24_fast, vec_scale24x24_fast, vec_scale16x16_fast) apply some 
  restrictions
For floating point:
  Fuction vec_shiftf() makes scaling without saturation of data values by given 
  scale factor (degree of 2). 
  Functions vec_scalef() and vec_scale_sf() make multiplication of input vector
  to coefficient which is not a power of 2.
  Two versions of routines are available: 
    without saturation - vec_scalef;
    with saturation - vec_scale_sf; 

Precision:
24x24 24-bit input, 32-bit output
32x32 32-bit input, 32-bit output
16x16 16-bit input, 16-bit output
f     single precision floating point

  Input:
  x[N]  input data , Q31,Q15 or floating point
  t     shift count. If positive, it shifts left with saturation, if
  negative it shifts right
  s     scale factor, Q31,Q15 or floating point
  N     length of vector
  fmin  minimum output value (only for vec_scale_sf)
  fmax  maximum output value (only for vec_scale_sf)

  Output:
  y[N]  output data, Q31 or Q15

  Restrictions:
  For regular versions (vec_shift24x24, vec_shift32x32, vec_shift16x16, 
  vec_shiftf, vec_scale32x24, vec_scale24x24, vec_scale16x16, vec_scalef, 
  vec_scale_sf):
  x,y should not overlap
  t   should be in range -31...31 for fixed-point functions and -129...146 
      for floating point

  For vec_scale_sf - fmin<=fmax;

  For faster versions (vec_shift24x24_fast, vec_shift32x32_fast, 
  vec_shift16x16_fast, vec_scale32x24_fast, vec_scale24x24_fast, 
  vec_scale16x16_fast):
  x,y should not overlap
  t should be in range -31...31 
  x,y - aligned on 8-byte boundary
  N   - multiple of 4 
-------------------------------------------------------------------------*/
void vec_shift24x24 (     f24 * restrict y,
                    const f24 * restrict x,
                    int t,
                    int N);
void vec_shift32x32 (     int32_t * restrict y,
                    const int32_t * restrict x,
                    int t,
                    int N);
void vec_shift16x16 (     int16_t * restrict y,
                    const int16_t * restrict x,
                    int t,
                    int N);
void vec_shiftf     (     float32_t * restrict y,
                    const float32_t * restrict x,
                    int t,
                    int N);
void vec_scale32x24 (     int32_t * restrict y,
                    const int32_t * restrict x,
                    f24 s,
                    int N);
void vec_scale24x24 (     f24 * restrict y,
                    const f24 * restrict x,
                    f24 s,
                    int N);
void vec_scale16x16 (     int16_t * restrict y,
                    const int16_t * restrict x,
                    int16_t s,
                    int N);
void vec_scalef     (     float32_t * restrict y,
                    const float32_t * restrict x,
                    float32_t s,
                    int N);
void vec_scale_sf   (     float32_t * restrict y,
                    const float32_t * restrict x,
                    float32_t s, float32_t fmin, float32_t fmax,
                    int N);
void vec_shift24x24_fast
                  ( f24 * restrict y,
                    const f24 * restrict x,
                    int t,
                    int N );
void vec_shift32x32_fast
                  ( int32_t * restrict y,
                    const int32_t * restrict x,
                    int t,
                    int N );
void vec_shift16x16_fast
                  ( int16_t * restrict y,
                    const int16_t * restrict x,
                    int t,
                    int N );
void vec_scale32x24_fast
                  ( int32_t * restrict y,
                    const int32_t * restrict x,
                    f24 s,
                    int N );
void vec_scale24x24_fast
                  ( f24 * restrict y,
                    const f24 * restrict x,
                    f24 s,
                    int N );
void vec_scale16x16_fast
                  ( int16_t * restrict y,
                    const int16_t * restrict x,
                    int16_t s,
                    int N );

/*-------------------------------------------------------------------------
  Reciprocal 
  Fixed point routines return the fractional and exponential portion of the 
  reciprocal of a vector x of Q31 or Q15 numbers. Since the reciprocal is 
  always greater than 1, it returns fractional portion frac in Q(31-exp) 
  or Q(15-exp) format and exponent exp so true reciprocal value in the 
  Q0.31/Q0.15 may be found by shifting fractional part left by exponent 
  value.

  Mantissa accuracy is 1 LSB, so relative accuracy is:
  vec_recip16x16, scl_recip16x16                   6.2e-5 
  vec_recip24x24, scl_recip32x32, scl_recip24x24   2.4e-7 
  vec_recip32x32                                   9.2e-10

  Floating point routines operate with standard floating point numbers. 
  Functions return +/-infinity on zero or denormalized input and provide 
  accuracy of 1 ULP.

  Precision: 
  32x32   32-bit input, 32-bit output. 
  24x24   24-bit input, 24-bit output. 
  16x16   16-bit input, 16-bit output. 
  f       floating point


  Input:
  x[N]    input data, Q31,Q15 or floating point
  N       length of vectors

  Output:
  frac[N] fractional part of result, Q(31-exp) or Q(15-exp) (fixed point 
          functions)
  exp[N]  exponent of result (fixed point functions) 
  y[N]    result (floating point function)

  Restriction:
  x,frac,exp should not overlap

  Scalar versions:
  ----------------
  Return packed value for fixed-point functions: 
  scl_recip24x24(),scl_recip32x32():
  bits 23:0 fractional part
  bits 31:24 exponent
  scl_recip16x16():
  bits 15:0 fractional part
  bits 31:16 exponent
-------------------------------------------------------------------------*/
void vec_recip32x32 (
                  int32_t * restrict frac, 
                  int16_t *exp, 
                  const int32_t * restrict x, 
                  int N);
void vec_recip24x24 (
                  f24 * restrict frac, 
                  int16_t *exp, 
                  const f24 * restrict x, 
                  int N);
void vec_recip16x16 (
                  int16_t * restrict frac, 
                  int16_t *exp, 
                  const int16_t * restrict x, 
                  int N);
void vec_recipf  (float32_t* restrict y,
                  const float32_t* restrict x, int N);
uint32_t  scl_recip32x32 (int32_t x);
uint32_t  scl_recip24x24 (f24 x);
uint32_t  scl_recip16x16 (int16_t x);
float32_t scl_recipf     (float32_t x);

/*-------------------------------------------------------------------------
  Division 
Fixed point routines perform pair-wise division of vectors written in Q31 or 
Q15 format. They return the fractional and exponential portion of the division 
result. Since the division may generate result greater than 1, it returns 
fractional portion frac in Q(31-exp) or or Q(15-exp) format and exponent 
exp so true division result in the Q0.31 may be found by shifting 
fractional part left by exponent value.
For division to 0, the result is not defined 

For fixed point finctions, mantissa accuracy is 2 LSB, so relative accuracy is:
vec_divide16x16, scl_divide16x16                   1.2e-4 
vec_divide24x24, scl_divide32x32, scl_divide24x24  4.8e-7 
vec_divide32x32                                    1.8e-9

Floating point routines operate with standard floating point numbers. Functions 
return +/-infinity in case of overflow and provide accuracy of 2 ULP.

Two versions of routines are available: regular versions (vec_divide32x32, 
vec_divide24x24, vec_divide16x16) work with arbitrary arguments, faster 
versions (vec_divide32x3_fast, vec_divide24x24_fast, vec_divide16x16_fast) 
apply some restrictions.

  Precision: 
  32x32  32-bit inputs, 32-bit output. 
  24x24  24-bit inputs, 24-bit output. 
  16x16  16-bit inputs, 16-bit output. 
  f      floating point

  Input:
  x[N]    nominator,Q31, Q15, floating point
  y[N]    denominator,Q31, Q15, floating point
  N       length of vectors
  Output:
  frac[N] fractional parts of result, Q(31-exp) or Q(15-exp) (for fixed 
          point functions)
  exp[N]  exponents of result (for fixed point functions) 
  z[N]    result (for floating point function)

  Restriction:
  For regular versions (vec_divide32x32, vec_divide24x24, 
  vec_divide16x16, vec_dividef) :
  x,y,frac,exp should not overlap

  For faster versions (vec_divide32x3_fast, vec_divide24x24_fast, 
  vec_divide16x16_fast) :
  x,y,frac,exp should not overlap
  x, y, frac to be aligned by 8-byte boundary, N - multiple of 4.

  Scalar versions:
  ----------------
  Return packed value (for fixed point functions): 
  scl_divide24x24(),scl_divide32x32():
  bits 23:0 fractional part
  bits 31:24 exponent
  scl_divide16x16():
  bits 15:0 fractional part
  bits 31:16 exponent
-------------------------------------------------------------------------*/
void vec_divide32x32 
                (int32_t * restrict frac, 
                 int16_t *exp, 
                 const int32_t * restrict x, 
                 const int32_t * restrict y, int N);
void vec_divide24x24 
                (f24 * restrict frac, 
                 int16_t *exp, 
                 const f24 * restrict x, 
                 const f24 * restrict y, int N) ;
void vec_divide16x16 
                (int16_t * restrict frac, 
                 int16_t *exp, 
                 const int16_t * restrict x, 
                 const int16_t * restrict y, int N);
void vec_dividef 
                (float32_t *restrict z, 
                 const float32_t * restrict x, const float32_t * restrict y, int N);

void vec_divide32x32_fast 
                (int32_t * restrict frac, 
                 int16_t *exp, 
                 const int32_t * restrict x, 
                 const int32_t * restrict y, int N);
void vec_divide24x24_fast 
                (f24 * restrict frac, 
                 short *exp, 
                 const f24 * restrict x, 
                 const f24 * restrict y, int N) ;
void vec_divide16x16_fast 
                (int16_t * restrict frac, 
                 int16_t *exp, 
                 const int16_t * restrict x, 
                 const int16_t * restrict y, int N);

uint32_t  scl_divide32x32 (int32_t x,int32_t y);
uint32_t  scl_divide24x24 (f24     x,f24     y);
uint32_t  scl_divide16x16 (int16_t x,int16_t y);
float32_t scl_dividef     (float32_t x, float32_t y);

/*-------------------------------------------------------------------------
  Logarithm:
  Different kinds of logarithm (base 2, natural, base 10). 32 and 24-bit 
  fixed point functions interpret input as Q16.15 and represent results in 
  Q25 format or return 0x80000000 on negative of zero input. 16-bit fixed-
  point functions interpret input as Q8.7 and represent result in Q3.12 or
  return 0x8000 on negative of zero input

  Precision:
  16x16  16-bit inputs, 16-bit outputs
  24x24  24-bit inputs, 24-bit outputs
  32x32  32-bit inputs, 32-bit outputs
  f      floating point

  Accuracy :
  16x16 functions                                                    2 LSB
  vec_log2_32x32,scl_log2_32x32  , vec_log2_24x24,scl_log2_24x24     730 (2.2e-5)
  vec_logn_32x32,scl_logn_32x32  , vec_logn_24x24,scl_logn_24x24     510 (1.5e-5)
  vec_log10_32x32,scl_log10_32x32, vec_log10_24x24,scl_log10_24x24   230 (6.9e-6)
  floating point                                                     2 ULP

  NOTES:
  1.  Although 32 and 24 bit functions provide the same accuracy, 32-bit 
      functions have better input/output resolution (dynamic range)
  2.  Scalar Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly.
  3.  Floating point functions limit the range of allowable input values:
      A) If x<0, the result is set to NaN. In addition, scalar floating point
         functions assign the value EDOM to errno and raise the "invalid" 
         floating-point exception.
      B) If x==0, the result is set to minus infinity. Scalar floating  point
         functions assign the value ERANGE to errno and raise the "divide-by-zero"
         floating-point exception.

  Input:
  x[N]  input data, Q16.15 (32 or 24-bit functions), Q8.7 (16-bit functions) or 
        floating point 
  N     length of vectors
  Output:
  y[N]  result, Q6.25 (32 or 24-bit functions), Q3.12 (16-bit functions) or 
        floating point 

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  return result result, Q6.25 (32 or 24-bit functions), Q3.12 (16-bit 
  functions) or floating point
-------------------------------------------------------------------------*/
void vec_log2_16x16 (int16_t * restrict y, const int16_t * restrict x, int N);
void vec_logn_16x16 (int16_t * restrict y, const int16_t * restrict x, int N);
void vec_log10_16x16(int16_t * restrict y, const int16_t * restrict x, int N);
void vec_log2_32x32 (int32_t * restrict y,const int32_t * restrict x, int N);
void vec_logn_32x32 (int32_t * restrict y,const int32_t * restrict x, int N);
void vec_log10_32x32(int32_t * restrict y,const int32_t * restrict x, int N);
void vec_log2_24x24 (f24 * restrict y,const f24 * restrict x, int N);
void vec_logn_24x24 (f24 * restrict y,const f24 * restrict x, int N);
void vec_log10_24x24(f24 * restrict y,const f24 * restrict x, int N);
void vec_log2f     (float32_t * restrict y,const float32_t * restrict x, int N);
void vec_lognf     (float32_t * restrict y,const float32_t * restrict x, int N);
void vec_log10f    (float32_t * restrict y,const float32_t * restrict x, int N);
int16_t scl_log2_16x16 (int16_t x);
int16_t scl_logn_16x16 (int16_t x);
int16_t scl_log10_16x16(int16_t x);
int32_t scl_log2_32x32 (int32_t x);
int32_t scl_logn_32x32 (int32_t x);
int32_t scl_log10_32x32(int32_t x);
f24 scl_log2_24x24 (f24 x);
f24 scl_logn_24x24 (f24 x);
f24 scl_log10_24x24(f24 x);
float32_t scl_log2f (float32_t x);
float32_t scl_lognf (float32_t x);
float32_t scl_log10f(float32_t x);

/*-------------------------------------------------------------------------
  Antilogarithm
  These routines calculate antilogarithm (base2, natural and base10). 32 
  and 24-bit fixed-point functions accept inputs in Q6.25 and form outputs 
  in Q16.15 format and return 0x7FFFFFFF in case of overflow and 0 in case 
  of underflow. 16-bit fixed-point functions accept inputs in Q3.12 and 
  form outputs in Q8.7 format and return 0x7FFF in case of overflow and 
  0 in case of underflow.

  Precision:
  16x16  16-bit inputs, 16-bit outputs. Accuracy: 2 LSB
  32x32  32-bit inputs, 32-bit outputs. Accuracy: 8*e-6*y+1LSB
  24x24  24-bit inputs, 24-bit outputs. Accuracy: 8*e-6*y+1LSB
  f      floating point: Accuracy: 2 ULP
  NOTE:
  1.  Although 32 and 24 bit functions provide the similar accuracy, 32-bit
      functions have better input/output resolution (dynamic range).
  2.  Floating point functions are compatible with standard ANSI C routines 
      and set errno and exception flags accordingly.

  Input:
  x[N]  input data,Q6.25 (for 32 and 24-bit functions), Q3.12 (for 16-bit 
        functions) or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q16.15 (for 32 and 24-bit functions), Q8.7 (for 16-bit 
        functions) or floating point  

  Restriction:
  x,y should not overlap

  Scalar versions:
  ----------------
  fixed point functions return result, Q16.15 (for 32 and 24-bit functions), 
  Q8.7 (for 16-bit functions) 

-------------------------------------------------------------------------*/
void vec_antilog2_16x16 (int16_t * restrict y, const int16_t * restrict x, int N);
void vec_antilogn_16x16 (int16_t * restrict y, const int16_t * restrict x, int N);
void vec_antilog10_16x16(int16_t * restrict y, const int16_t * restrict x, int N);
void vec_antilog2_32x32 (int32_t * restrict y, const int32_t* restrict x, int N);
void vec_antilogn_32x32 (int32_t * restrict y, const int32_t* restrict x, int N);
void vec_antilog10_32x32(int32_t * restrict y, const int32_t* restrict x, int N);
void vec_antilog2_24x24 (f24 * restrict y, const f24* restrict x, int N);
void vec_antilogn_24x24 (f24 * restrict y, const f24* restrict x, int N);
void vec_antilog10_24x24(f24 * restrict y, const f24* restrict x, int N);
void vec_antilog2f (float32_t * restrict y, const float32_t* restrict x, int N);
void vec_antilognf (float32_t * restrict y, const float32_t* restrict x, int N);
void vec_antilog10f(float32_t * restrict y, const float32_t* restrict x, int N);
int16_t scl_antilog2_16x16 (int16_t x);
int16_t scl_antilogn_16x16 (int16_t x);
int16_t scl_antilog10_16x16(int16_t x);
int32_t scl_antilog2_32x32 (int32_t x);
int32_t scl_antilogn_32x32 (int32_t x);
int32_t scl_antilog10_32x32(int32_t x);
f24 scl_antilog2_24x24 (f24 x);
f24 scl_antilogn_24x24 (f24 x);
f24 scl_antilog10_24x24(f24 x);
float32_t scl_antilog2f (float32_t x);
float32_t scl_antilognf (float32_t x);
float32_t scl_antilog10f(float32_t x);

/*-------------------------------------------------------------------------
  Square Root
  These routines calculate square root.
  NOTES: 
  1. Fixed point functions return 0x80000000 (for 24 and 32-bit functions), 
     0x8000 (for 16-bit functions) on negative argument
  2. For floating point function, whenever an input value is negative, 
     functions raise the "invalid" floating-point exception, assign EDOM 
     to errno and set output value to NaN. Negative zero is considered as
     a valid input, the result is also -0
  Two versions of functions available: regular version (vec_sqrt16x16, 
  vec_sqrt24x24, vec_sqrt32x32, vec_sqrtf) with arbitrary arguments and
  faster version (vec_sqrt24x24_fast, vec_sqrt32x32_fast) that apply some 
  restrictions.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: (2 LSB)
  24x24  24-bit inputs, 24-bit output. Accuracy: (2.6e-7*y+1LSB)
  32x32  32-bit inputs, 32-bit output. Accuracy: (2.6e-7*y+1LSB)
  f      floating point. Accuracy 1 ULP

  Input:
  x[N]  input data,Q31, Q15 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31, Q15 or floating point

  Restriction:
  Regular versions (vec_sqrt24x24, vec_sqrt32x32):
  x,y - should not overlap

  Faster versions (vec_sqrt24x24_fast, vec_sqrt32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result, Q31, Q15 or floating point
-------------------------------------------------------------------------*/
void vec_sqrt16x16  (int16_t  * restrict y, const int16_t  * restrict x, int N);
void vec_sqrt24x24  (f24      * restrict y, const f24      * restrict x, int N);
void vec_sqrt32x32  (int32_t  * restrict y, const int32_t  * restrict x, int N);
void vec_sqrtf      (float32_t* restrict y, const float32_t* restrict x, int N);
void vec_sqrt24x24_fast (f24    * restrict y, const f24    * restrict x, int N);
void vec_sqrt32x32_fast (int32_t* restrict y, const int32_t* restrict x, int N);
int16_t   scl_sqrt16x16(int16_t x);
f24       scl_sqrt24x24(f24 x);
int32_t   scl_sqrt32x32(int32_t x);
float32_t scl_sqrtf    (float32_t x);

/*-------------------------------------------------------------------------
  Reciprocal Square Root
  These routines compute reciprocals of positive square root.
   Whenever an input value is negative, functions raise the "invalid" 
   floating-point exception, assign EDOM to errno and set output value to 
   NaN. For x[n]==+/-0, functions set output to +/-infinity, raise the 
   "divide by zero" floating-point exception, and assign ERANGE to errno.

  Precision: 
  f      floating point. Accuracy 2 ULP

  Input:
  x[N]  input data
  N     length of vectors
  Output:
  y[N]  output data

  Restriction:
  x,y - should not overlap

  Scalar versions:
  ----------------
  return result
-------------------------------------------------------------------------*/
void vec_rsqrtf         (float32_t* restrict y, const float32_t* restrict x, int N);
float32_t scl_rsqrtf    (float32_t x);

/*-------------------------------------------------------------------------
  Sine/Cosine 
  Fixed-point functions calculate sin(pi*x) or cos(pi*x) for numbers written 
  in Q31 or Q15 format. Return results in the same format. 
  Floating point functions compute sin(x) or cos(x)
  Two versions of functions available: regular version (vec_sine16x16, 
  vec_cosine16x16, vec_sine24x24, vec_cosine24x24, vec_sine32x32, 
  vec_cosine32x32, , vec_sinef, vec_cosinef) with arbitrary arguments and 
  faster version (vec_sine24x24_fast, vec_cosine24x24_fast, 
  vec_sine32x32_fast, vec_cosine32x32_fast) that apply some restrictions.
  NOTE:
  1.  Scalar floating point functions are compatible with standard ANSI C
      routines and set errno and exception flags accordingly
  2.  Floating point functions limit the range of allowable input values:
      [-102940.0, 102940.0] Whenever the input value does not belong to this
      range, the result is set to NaN.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: 2 LSB  
  24x24  24-bit inputs, 24-bit output. Accuracy: 74000(3.4e-5)
  32x32  32-bit inputs, 32-bit output. Accuracy: 1700 (7.9e-7)
  f      floating point. Accuracy 2 ULP

  Input:
  x[N]  input data, Q15, Q31 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31,Q15 or floating point

  Restriction:
  Regular versions (vec_sine24x24, vec_cosine24x24, vec_sine32x32, 
  vec_cosine32x32, vec_sinef, vec_cosinef):
  x,y - should not overlap

  Faster versions (vec_sine24x24_fast, vec_cosine24x24_fast, 
  vec_sine32x32_fast, vec_cosine32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result in Q31,Q15 or floating point
-------------------------------------------------------------------------*/
void vec_sine16x16   (int16_t  * restrict y, 
                const int16_t  * restrict x, int N);
void vec_cosine16x16 (int16_t  * restrict y, 
                const int16_t  * restrict x, int N);
void vec_sine24x24 (  f24 * restrict y,
                const f24 * restrict x,
                int N);
void vec_sine24x24_fast (  f24 * restrict y,
                const f24 * restrict x,
                int N);
void vec_cosine24x24 (f24 * restrict y,
                const f24 * restrict x,
                int N);
void vec_cosine24x24_fast (f24 * restrict y,
                const f24 * restrict x,
                int N);
void vec_sine32x32 (  int32_t * restrict y,
                const int32_t * restrict x,
                int N);
void vec_sine32x32_fast (  int32_t * restrict y,
                const int32_t * restrict x,
                int N);
void vec_cosine32x32 (int32_t * restrict y,
                const int32_t * restrict x,
                int N);
void vec_cosine32x32_fast (int32_t * restrict y,
                const int32_t * restrict x,
                int N);
void vec_sinef     (  float32_t * restrict y,
                const float32_t * restrict x,
                int N);
void vec_cosinef(     float32_t * restrict y,
                const float32_t * restrict x,
                int N);
int16_t scl_sine16x16   (int16_t x);
int16_t scl_cosine16x16 (int16_t x);
f24     scl_sine24x24   (f24 x);
f24     scl_cosine24x24 (f24 x);
int32_t scl_sine32x32   (int32_t x);
int32_t scl_cosine32x32 (int32_t x);
float32_t scl_sinef   (float32_t x);
float32_t scl_cosinef (float32_t x);

/*-------------------------------------------------------------------------
  Tangent 
  Fixed point functions calculate tan(pi*x) for number written in Q15 or 
  Q31. Floating point functions compute tan(x)
  
  Precision:
  16x16  16-bit inputs (Q15), 16-bit outputs (Q8.7). Accuracy: 1 LSB
  24x24  24-bit inputs, 32-bit outputs. Accuracy: (1.3e-4*y+1LSB) 
                                        if abs(y)<=464873(14.19 in Q15) 
                                        or abs(x)<pi*0.4776
  32x32  32-bit inputs, 32-bit outputs. Accuracy: (1.3e-4*y+1LSB)
                                        if abs(y)<=464873(14.19 in Q15) 
                                        or abs(x)<pi*0.4776
  f      floating point.                Accuracy: 2 ULP

  NOTE:
  1.  Scalar floating point function is compatible with standard ANSI C routines 
      and set errno and exception flags accordingly
  2.  Floating point functions limit the range of allowable input values: [-9099, 9099]
      Whenever the input value does not belong to this range, the result is set to NaN.

  Input:
  x[N]   input data, Q15, Q31 or floating point
  N      length of vectors
  Output:
  y[N]   result, Q8.7 (16-bit function), Q16.15 (24 or 32-bit functions) 
         or floating point

  Restriction:
  x,y - should not overlap

  Scalar versions:
  ----------------
  Return result, Q8.7 (16-bit function), Q16.15 (24 or 32-bit functions) 
  or floating point
-------------------------------------------------------------------------*/
void vec_tan16x16 (int16_t* restrict y, const int16_t *restrict x, int N);
void vec_tan24x24 (
              int32_t* restrict   y, 
              const f24 *restrict x,
              int N);
void vec_tan32x32 ( 
              int32_t* restrict   y, 
              const int32_t *restrict x,
              int N);
void vec_tanf ( 
                    float32_t *restrict y, 
              const float32_t *restrict x,
              int N);
int16_t scl_tan16x16 (int16_t x);
int32_t scl_tan24x24 (f24 x);
int32_t scl_tan32x32 (int32_t x);
float32_t scl_tanf (float32_t x);

/*-------------------------------------------------------------------------
  Arc Sine/Cosine 
  The arc sine/cosine functions return the arc sine/cosine of x. Output is 
  in radians. For floating-point routines, input value should belong to 
  [-1,1], otherwise the functions raise the  "invalid" floating-point 
  exception, assign EDOM to errno and return NaN.

  Precision: 
  f    floating point. Accuracy: 2 ULP

  Input:
  x[N]   input data, floating point
  N      length of vectors
  Output:
  z[N]   result, floating point

  Restriction:
  x,z should not overlap

  Scalar versions:
  ----------------
  return result, floating point
-------------------------------------------------------------------------*/
void vec_asinf     (float32_t * restrict z, const float32_t * restrict x, int N );
void vec_acosf     (float32_t * restrict z, const float32_t * restrict x, int N );
float32_t scl_asinf (float32_t x);
float32_t scl_acosf (float32_t x);

/*-------------------------------------------------------------------------
  Arctangent 
  Functions calculate arctangent of number. Fixed point functions scale 
  output by pi which corresponds to the real phases +pi/4 and represent 
  input and output in Q15 or Q31
  NOTE:
  1.  Scalar floating point function is compatible with standard ANSI C
      routines and sets errno and exception flags accordingly

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: 2 LSB
  24x24  24-bit inputs, 24-bit output. Accuracy: 74000 (3.4e-5)
  32x32  32-bit inputs, 32-bit output. Accuracy: 42    (2.0e-8)
  f      floating point. Accuracy: 2 ULP


  Input:
  x[N]   input data, Q15, Q31 or floating point
  N      length of vectors
  Output:
  z[N]   result, Q15, Q31 or floating point

  Restriction:
  x,z should not overlap

  Scalar versions:
  ----------------
  return result, Q15, Q31 or floating point
-------------------------------------------------------------------------*/
void vec_atan16x16 (int16_t   * restrict z, 
              const int16_t   * restrict x, int N );
void vec_atan24x24 (f24 * restrict z, 
              const f24 * restrict x, 
              int N );
void vec_atan32x32 (int32_t * restrict z, 
              const int32_t * restrict x, 
              int N );
void vec_atanf (    float32_t * restrict z, 
              const float32_t * restrict x, 
              int N );
int16_t scl_atan16x16 (int16_t x);
f24 scl_atan24x24 (f24 x);
int32_t scl_atan32x32 (int32_t x);
float32_t scl_atanf (float32_t x);

/*-------------------------------------------------------------------------
Floating-Point Full-Quadrant Arc Tangent
The functions compute the full quadrant arc tangent of the ratio y/x. 
Floating point functions output is in radians. Fixed point functions 
scale its output by pi.

NOTE:
1. Scalar function is compatible with standard ANSI C routines and set 
   errno and exception flags accordingly
2. Scalar function assigns EDOM to errno whenever y==0 and x==0.

Special cases:
     y    |   x   |  result   |  extra conditions    
  --------|-------|-----------|---------------------
   +/-0   | -0    | +/-pi     |
   +/-0   | +0    | +/-0      |
   +/-0   |  x    | +/-pi     | x<0
   +/-0   |  x    | +/-0      | x>0
   y      | +/-0  | -pi/2     | y<0
   y      | +/-0  |  pi/2     | y>0
   +/-y   | -inf  | +/-pi     | finite y>0
   +/-y   | +inf  | +/-0      | finite y>0
   +/-inf | x     | +/-pi/2   | finite x
   +/-inf | -inf  | +/-3*pi/4 | 
   +/-inf | +inf  | +/-pi/4   |

Input:
  y[N]  input data, Q15 or floating point
  x[N]  input data, Q15 or floating point
  N     length of vectors
Output:
  z[N]  result, Q15 or floating point
  
Restrictions:
x, y, z should not overlap
---------------------------------------------------------------------------*/
void vec_atan2_16x16 (int16_t   * restrict z, const int16_t   * restrict y, const int16_t   * restrict x,int N);
void vec_atan2f      (float32_t * restrict z, const float32_t * restrict y, const float32_t * restrict x,int N);
int16_t   scl_atan2_16x16 (int16_t y, int16_t x);
float32_t scl_atan2f      (float32_t y, float32_t x);

/*-------------------------------------------------------------------------
  Common Exponent 
  These functions determine the number of redundant sign bits for each value 
  (as if it was loaded in a 32-bit register) and returns the minimum number 
  over the whole vector. This may be useful for a FFT implementation to 
  normalize data.  
  NOTES:
  Faster version of functions make the same task but in a different manner - 
  they compute exponent of maximum absolute value in the array. It allows 
  faster computations but not bitexact results - if minimum value in the 
  array will be -2^n , fast function returns max(0,30-n) while non-fast 
  function returns (31-n).
  Floating point function returns 0-floor(log2(max(abs(x)))). Returned 
  result will be always in range [-129...146]. 
  Special cases
  x       | result
  --------+-------
  0       |    0
  +/-Inf  | -129
  NaN     |    0

  If dimension N<=0 functions return 0

  Precision: 
  32 32-bit inputs 
  24 24-bit inputs 
  16 16-bit inputs 
  f  single precision floating point

  Input:
  x[N] input data
  N    length of vector
  Returned value: minimum exponent

  Restriction:
  Regular versions (vec_bexp16, vec_bexp24, vec_bexp32, vec_bexpf):
  none

  Faster versions (vec_bexp16_fast, vec_bexp24_fast, vec_bexp32_fast):
  x,y - aligned on 8-byte boundary
  N   - a multiple of 4
-------------------------------------------------------------------------*/
int vec_bexp32 (const int32_t   * restrict x, int N);
int vec_bexp24 (const f24       * restrict x, int N);
int vec_bexp16 (const int16_t   * restrict x, int N);
int vec_bexpf  (const float32_t * restrict x, int N);
int scl_bexp32 (int32_t x);
int scl_bexp24 (f24 x);
int scl_bexp16 (int16_t x);
int scl_bexpf  (float32_t x);
int vec_bexp32_fast (const int32_t * restrict x, int N);
int vec_bexp24_fast (const f24     * restrict x, int N);
int vec_bexp16_fast (const int16_t * restrict x, int N);
/*-------------------------------------------------------------------------
  Vector Min/Max
  These routines find maximum/minimum value in a vector.
  Two versions of functions available: regular version (vec_min32x32, 
  vec_max32x32, vec_min24x24, vec_max24x24,vec_max16x16, vec_min16x16, 
  vec_maxf, vec_minf) with arbitrary arguments and faster version 
  (vec_min32x32_fast, vec_max32x32_fast, vec_min24x24_fast, 
  vec_max24x24_fast,vec_min16x16_fast, vec_min16x16_fast) that apply some 
  restrictions
  NOTE: functions return zero if N is less or equal to zero

  Precision: 
  32x32 32-bit data, 32-bit output
  24x24 24-bit data, 24-bit output
  16x16 16-bit data, 16-bit output
  f     single precision floating point
  
  Input:
  x[N]  input data
  N     length of vector
  Function return minimum or maximum value correspondingly

  Restriction:
  For regular routines:
  none
  For faster routines:
  x aligned on 8-byte boundary
  N   - multiple of 4
-------------------------------------------------------------------------*/
int32_t   vec_min32x32 (const int32_t  * restrict x, int N);
f24       vec_min24x24 (const f24      * restrict x, int N);
int16_t   vec_min16x16 (const int16_t  * restrict x, int N);
float32_t vec_minf     (const float32_t* restrict x, int N);
int32_t vec_max32x32   (const int32_t  * restrict x, int N);
f24     vec_max24x24   (const f24      * restrict x, int N);
int16_t vec_max16x16   (const int16_t  * restrict x, int N);
float32_t vec_maxf     (const float32_t* restrict x, int N);
int32_t vec_min32x32_fast (const int32_t* restrict x, int N);
f24     vec_min24x24_fast (const f24    * restrict x, int N);
int16_t vec_min16x16_fast (const int16_t* restrict x, int N);
int32_t vec_max32x32_fast (const int32_t* restrict x, int N);
f24     vec_max24x24_fast (const f24    * restrict x, int N);
int16_t vec_max16x16_fast (const int16_t* restrict x, int N);

/*-------------------------------------------------------------------------
  integer to float conversion
  routine converts integer to float and scales result up by 2^t.

  Precision: 
  f     single precision floating point

  Output:
  y[N]  output data
  Input:
  x[N]  input data
  t     scale factor
  N     length of vector

  Restriction:
  t should be in range -126...126
-------------------------------------------------------------------------*/
void   vec_int2float ( float32_t  * restrict y, const int32_t  * restrict x, int t, int N);
float32_t scl_int2float (int32_t x, int t);

/*-------------------------------------------------------------------------
  float to integer conversion
  routine scale floating point input down by 2^t and convert it to integer 
  with saturation

  Precision: 
  f     single precision floating point

  Output:
  y[N]  output data
  Input:
  x[N]  input data
  t     scale factor
  N     length of vector

  Restriction:
  t should be in range -126...126
-------------------------------------------------------------------------*/
void   vec_float2int (  int32_t * restrict y, const float32_t * restrict x, int t, int N);
int32_t scl_float2int (float32_t x, int t);

/*-------------------------------------------------------------------------
  Rounding 
  Routines make floating point round to integral value. Input data are 
  rounded up/down to the nearest integral value but maintained in the 
  same floating point format.

  Precision: 
  f     single precision floating point

  Input:
  x[N]  input data, floating point
  N     length of vector
  Output:
  y[N]  rounded floating point values

  Restriction:
  x,y should not overlap
-------------------------------------------------------------------------*/
void   vec_float2floor   (float32_t * restrict y, const float32_t * restrict x, int N);
void   vec_float2ceil    (float32_t * restrict y, const float32_t * restrict x, int N);
float32_t scl_float2floor(float32_t x);
float32_t scl_float2ceil (float32_t x);

/*-------------------------------------------------------------------------
  Complex magnitude
  routines compute complex magnitude or its reciprocal

  Precision: 
  f     single precision floating point

  Output:
  y[N]  output data
  Input:
  x[N]  input complex data
  N     length of vector

  Restriction:
  none
-------------------------------------------------------------------------*/
void       vec_complex2mag    (float32_t  * restrict y, const complex_float  * restrict x, int N);
void       vec_complex2invmag (float32_t  * restrict y, const complex_float  * restrict x, int N);
float32_t  scl_complex2mag    (complex_float x);
float32_t  scl_complex2invmag (complex_float x);

/*-------------------------------------------------------------------------
  Polynomial approximation
  Functions calculate polynomial approximation for all values from given 
  vector. Fixed point functions take polynomial coefficients in Q15 or 
  Q31 precision. 
  NOTE:
  approximation is calculated like Taylor series that is why overflow may 
  potentially occur if cumulative sum of coefficients given from the last 
  to the first coefficient is bigger than 1. To avoid this negative effect,
  all the coefficients may be scaled down and result will be shifted left 
  after all intermediate computations.

  Precision: 
  16x16  16-bit inputs, 16-bit coefficients, 16-bit output. 
  24x24  24-bit inputs, 24-bit coefficients, 24-bit output. 
  32x32  32-bit inputs, 32-bit coefficients, 32-bit output.
  f      floating point

  Input:
  x[N]    input data, Q15, Q31 or floating point
  N       length of vector
  lsh     additional left shift for result
  c[M+1]  coefficients (5 coefficients for vec_poly4_xxx 
          and 9 coefficients for vec_poly8_xxx), Q15, Q31 
          or floating point
  Output:
  z[N]    result, Q15, Q31 or floating point

  Restriction:
  x,c,z should not overlap
  lsh   should be in range 0...31
-------------------------------------------------------------------------*/
void vec_poly4_16x16 (int16_t * restrict z, const int16_t * restrict x, const int16_t * restrict c, int lsh, int N );
void vec_poly8_16x16 (int16_t * restrict z, const int16_t * restrict x, const int16_t * restrict c, int lsh, int N );
void vec_poly4_24x24 (f24 * restrict z, const f24 * restrict x, const f24 * restrict c, int lsh,int N );
void vec_poly8_24x24 (f24 * restrict z, const f24 * restrict x, const f24 * restrict c, int lsh,int N );
void vec_poly4_32x32 (int32_t * restrict z, const int32_t * restrict x, const int32_t * restrict c, int lsh,int N );
void vec_poly8_32x32 (int32_t * restrict z, const int32_t * restrict x, const int32_t * restrict c, int lsh,int N );
void vec_poly4f      (float32_t * restrict z, const float32_t * restrict x, const float32_t * restrict c, int N );
void vec_poly8f      (float32_t * restrict z, const float32_t * restrict x, const float32_t * restrict c, int N );
/*===========================================================================
  Matrix Operations:
  mtx_mpy              Matrix Multiply
  mtx_vecmpy           Matrix by Vector Multiply
  mtx_inv              Matrix inversion
===========================================================================*/
/*-------------------------------------------------------------------------
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and y. The columnar dimension of x must match the row dimension of y. 
  The resulting matrix has the same number of rows as x and the same number 
  of columns as y.

  NOTES:
  In the fixed-point routines, rows of matrices z and y may be stored in 
  non consecutive manner. Matrix x will have all the elements in contiguous 
  memory locations.

  Functions require scratch memory for storing intermediate data. This 
  scratch memory area should be aligned on 8 byte boundary and its size 
  is calculated by macros SCRATCH_MTX_MPY32X32(M,N,P), 
  SCRATCH_MTX_MPY24X24(M,N,P), SCRATCH_MTX_MPY16X16(M,N,P)

  Two versions of functions available: regular version (mtx_mpy32x32, 
  mtx_mpy24x24, mtx_mpy16x16, mtx_mpyf) with arbitrary arguments and 
  faster version (mtx_mpy32x32_fast, mtx_mpy24x24_fast, 
  mtx_mpy16x16_fast, mtx_mpyf_fast) that apply some restrictions.

  Precision: 
  32x32 32-bit inputs, 32-bit output
  24x24 24-bit inputs, 24-bit output
  16x16 16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N]      input matrix,Q31, Q15 or floating point
  y[N][P]     input matrix y. For fixed point routines, these are N 
              vectors of size P,Q31 or Q15. For floating point, this 
              is just a matrix of size NxP.
  M           number of rows in matrix x and z
  N           number of columns in matrix x and number of rows in matrix y
  P           number of columns in matrices y and z
  lsh         additional left shift
  Output:
  z[M][P]     output matrix z. For fixed point routines, these are M 
              vectors of size P Q31 or Q15. For floating point, this 
              is single matrix of size MxP
  Scratch:
  pScr        Scratch memory area with size in bytes defined by macros 
              SCRATCH_MTX_MPY32X32, SCRATCH_MTX_MPY24X24, 
              SCRATCH_MTX_MPY16X16


  Restrictions:
  For regular routines (mtx_mpy32x32,mtx_mpy24x24, mtx_mpy16x16, mtx_mpyf):
  x,y,z should not overlap

  For faster routines (mtx_mpy32x32_fast, mtx_mpy24x24_fast, 
  mtx_mpy16x16_fast, mtx_mpyf_fast):
  x,y,z should not overlap
  x - aligned on 8-byte boundary
  all rows which addresses are written to y[] - aligned on 8-byte boundary
  N is a multiple of 4,M=8,P=2  

-------------------------------------------------------------------------*/
void mtx_mpy32x32 (  void* pScr,
                     int32_t** restrict z,
               const int32_t*  restrict x,
               const int32_t** restrict y,
               int M, int N, int P, int lsh );
void mtx_mpy24x24 (  void* pScr,
                     f24** restrict z,
               const f24*  restrict x,
               const f24** restrict y,
               int M, int N, int P, int lsh );
void mtx_mpy16x16 (  void* pScr,
                     int16_t** restrict z,
               const int16_t*  restrict x,
               const int16_t** restrict y,
               int M, int N, int P, int lsh );
void mtx_mpy32x32_fast (  int32_t** restrict z,
               const int32_t *  restrict x,
               const int32_t ** restrict y,
               int M, int N, int P, int lsh );

void mtx_mpy24x24_fast (  f24** restrict z,
               const f24*  restrict x,
               const f24** restrict y,
               int M, int N, int P, int lsh );
void mtx_mpy16x16_fast (  int16_t** restrict z,
               const int16_t*  restrict x,
               const int16_t** restrict y,
               int M, int N, int P, int lsh );
void mtx_mpyf (      float32_t*  restrict z,
               const float32_t*  restrict x,
               const float32_t*  restrict y,
               int M, int N, int P);
void mtx_mpyf_fast (      float32_t*  restrict z,
               const float32_t*  restrict x,
               const float32_t*  restrict y,
               int M, int N, int P);

#define SCRATCH_MTX_MPY32X32(M,N,P) ((((N)+1)&(~1))*2*sizeof(int32_t))
#define SCRATCH_MTX_MPY24X24(M,N,P) ((((N)+1)&(~1))*2*sizeof(f24))
#define SCRATCH_MTX_MPY16X16(M,N,P) ((((N)+3)&(~3))*4*sizeof(int16_t))


/*-------------------------------------------------------------------------
  Matrix by Vector Multiply
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and vector y. 

  Two versions of functions available: regular version (mtx_vecmpy32x32, 
  mtx_vecmpy24x24, mtx_vecmpy16x16,mtx_vecmpyf) with arbitrary arguments 
  and faster version (mtx_vecmpy32x32_fast, mtx_vecmpy24x24_fast, 
  mtx_vecmpy16x16_fast, mtx_vecmpyf_fast) that apply some restrictions.

  Precision: 
  32x32 32-bit inputs, 32-bit output
  24x24 24-bit inputs, 24-bit output
  16x16 16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N] input matrix,Q31,Q15 or floating point
  y[N]   input vector,Q31,Q15 or floating point
  M      number of rows in matrix x
  N      number of columns in matrix x
  lsh    additional left shift
  Output:
  z[M]   output vector,Q31,Q15 or floating point

  Restriction:
  For regular routines (mtx_vecmpy24x24, mtx_vecmpy16x16, mtx_vecmpyf)
  x,y,z should not overlap

  For faster routines (mtx_vecmpy24x24_fast, mtx_vecmpy16x16_fast,
  mtx_vecmpyf_fast)
  x,y,z should not overlap
  x,y   aligned on 8-byte boundary
  N and M are multiples of 4
-------------------------------------------------------------------------*/
void mtx_vecmpy32x32 (  int32_t* restrict z,
               const int32_t * restrict x,
               const int32_t * restrict y,
               int M, int N, int lsh);
void mtx_vecmpy24x24 (  f24* restrict z,
               const f24* restrict x,
               const f24* restrict y,
               int M, int N, int lsh);
void mtx_vecmpy16x16 (  int16_t* restrict z,
               const int16_t* restrict x,
               const int16_t* restrict y,
               int M, int N, int lsh);
void mtx_vecmpy24x24_fast (  f24* restrict z,
               const f24* restrict x,
               const f24* restrict y,
               int M, int N, int lsh);
void mtx_vecmpy32x32_fast (  int32_t* restrict z,
               const int32_t * restrict x,
               const int32_t * restrict y,
               int M, int N, int lsh);
void mtx_vecmpy16x16_fast (  int16_t* restrict z,
               const int16_t* restrict x,
               const int16_t* restrict y,
               int M, int N, int lsh);
void mtx_vecmpyf (   float32_t* restrict z,
               const float32_t* restrict x,
               const float32_t* restrict y,
               int M, int N);
void mtx_vecmpyf_fast (   float32_t* restrict z,
               const float32_t* restrict x,
               const float32_t* restrict y,
               int M, int N);

/*-------------------------------------------------------------------------
  Operations with Small Matrices
  These functions implement basic operations under the sequence of small 
  square matrices. Fixed point data are interpreted as Q15 or Q31 and 
  results might be saturated.
  NOTE: 
  Determinant is computed recursively via minors of submatrices. So, in 
  the fixed-point routines, intermediate results might be saturated 
  although final result is in range. To avoid this saturation, right shift 
  might be applied at the first stage of computations. It means that final 
  result would be represented in Q(15-rsh) or Q(31-rsh) respectively. 
  Ad-hoc formula for rsh is rsh>=N-2 for real matrices and rsh>=N-1 for 
  complex matrices.

  Precision: 
  16x16  16-bit input, 16-bit output (real and complex)
  32x32  32-bit input, 32-bit output (real and complex)
  f      floating point (real and complex)

  Matrix dimensions are 2x2, 3x3, 4x4

  Input:
  x[L][N*N]      L input matrices
  y[L][N*N]      L input matrices (for addition, subtraction, multiply 
                 functions)
  rsh            right shift for fixed-point multiply and determinant 
                 function
  L              number of matrices
  Output:
  z[L][N*N]      L output matrices (for addition, subtraction, multiply, 
                 transpose functions)
  d[L]           determinants for L matrices (for determinant functions)

  Restrictions:
  rsh should be in range 0..15
  x,y,z should not overlap
-------------------------------------------------------------------------*/
// real matrix addition
void mtx_add2x2_16x16(int16_t   * restrict z, const int16_t   *restrict x, const int16_t   *restrict y, int L);
void mtx_add3x3_16x16(int16_t   * restrict z, const int16_t   *restrict x, const int16_t   *restrict y, int L);
void mtx_add4x4_16x16(int16_t   * restrict z, const int16_t   *restrict x, const int16_t   *restrict y, int L);
void mtx_add2x2_32x32(int32_t   * restrict z, const int32_t   *restrict x, const int32_t   *restrict y, int L);
void mtx_add3x3_32x32(int32_t   * restrict z, const int32_t   *restrict x, const int32_t   *restrict y, int L);
void mtx_add4x4_32x32(int32_t   * restrict z, const int32_t   *restrict x, const int32_t   *restrict y, int L);
void mtx_add2x2f     (float32_t * restrict z, const float32_t *restrict x, const float32_t *restrict y, int L);
void mtx_add3x3f     (float32_t * restrict z, const float32_t *restrict x, const float32_t *restrict y, int L);
void mtx_add4x4f     (float32_t * restrict z, const float32_t *restrict x, const float32_t *restrict y, int L);

// complex matrix addition
void cmtx_add2x2_16x16(complex_fract16* restrict z, const complex_fract16 *restrict x, const complex_fract16 *restrict y, int L);
void cmtx_add3x3_16x16(complex_fract16* restrict z, const complex_fract16 *restrict x, const complex_fract16 *restrict y, int L);
void cmtx_add4x4_16x16(complex_fract16* restrict z, const complex_fract16 *restrict x, const complex_fract16 *restrict y, int L);
void cmtx_add2x2_32x32(complex_fract32* restrict z, const complex_fract32 *restrict x, const complex_fract32 *restrict y, int L);
void cmtx_add3x3_32x32(complex_fract32* restrict z, const complex_fract32 *restrict x, const complex_fract32 *restrict y, int L);
void cmtx_add4x4_32x32(complex_fract32* restrict z, const complex_fract32 *restrict x, const complex_fract32 *restrict y, int L);
void cmtx_add2x2f     (complex_float  * restrict z, const complex_float   *restrict x, const complex_float   *restrict y, int L);
void cmtx_add3x3f     (complex_float  * restrict z, const complex_float   *restrict x, const complex_float   *restrict y, int L);
void cmtx_add4x4f     (complex_float  * restrict z, const complex_float   *restrict x, const complex_float   *restrict y, int L);

// real matrix subtraction
void mtx_sub2x2_16x16(int16_t   * restrict z, const int16_t   *restrict x, const int16_t   *restrict y, int L);
void mtx_sub3x3_16x16(int16_t   * restrict z, const int16_t   *restrict x, const int16_t   *restrict y, int L);
void mtx_sub4x4_16x16(int16_t   * restrict z, const int16_t   *restrict x, const int16_t   *restrict y, int L);
void mtx_sub2x2_32x32(int32_t   * restrict z, const int32_t   *restrict x, const int32_t   *restrict y, int L);
void mtx_sub3x3_32x32(int32_t   * restrict z, const int32_t   *restrict x, const int32_t   *restrict y, int L);
void mtx_sub4x4_32x32(int32_t   * restrict z, const int32_t   *restrict x, const int32_t   *restrict y, int L);
void mtx_sub2x2f     (float32_t * restrict z, const float32_t *restrict x, const float32_t *restrict y, int L);
void mtx_sub3x3f     (float32_t * restrict z, const float32_t *restrict x, const float32_t *restrict y, int L);
void mtx_sub4x4f     (float32_t * restrict z, const float32_t *restrict x, const float32_t *restrict y, int L);

// complex matrix subtraction
void cmtx_sub2x2_16x16(complex_fract16* restrict z, const complex_fract16 *restrict x, const complex_fract16 *restrict y, int L);
void cmtx_sub3x3_16x16(complex_fract16* restrict z, const complex_fract16 *restrict x, const complex_fract16 *restrict y, int L);
void cmtx_sub4x4_16x16(complex_fract16* restrict z, const complex_fract16 *restrict x, const complex_fract16 *restrict y, int L);
void cmtx_sub2x2_32x32(complex_fract32* restrict z, const complex_fract32 *restrict x, const complex_fract32 *restrict y, int L);
void cmtx_sub3x3_32x32(complex_fract32* restrict z, const complex_fract32 *restrict x, const complex_fract32 *restrict y, int L);
void cmtx_sub4x4_32x32(complex_fract32* restrict z, const complex_fract32 *restrict x, const complex_fract32 *restrict y, int L);
void cmtx_sub2x2f     (complex_float  * restrict z, const complex_float   *restrict x, const complex_float   *restrict y, int L);
void cmtx_sub3x3f     (complex_float  * restrict z, const complex_float   *restrict x, const complex_float   *restrict y, int L);
void cmtx_sub4x4f     (complex_float  * restrict z, const complex_float   *restrict x, const complex_float   *restrict y, int L);

// real matrix multiply
void mtx_mul2x2_16x16(int16_t   * restrict z, const int16_t   *restrict x, const int16_t   *restrict y, int rsh, int L);
void mtx_mul3x3_16x16(int16_t   * restrict z, const int16_t   *restrict x, const int16_t   *restrict y, int rsh, int L);
void mtx_mul4x4_16x16(int16_t   * restrict z, const int16_t   *restrict x, const int16_t   *restrict y, int rsh, int L);
void mtx_mul2x2_32x32(int32_t   * restrict z, const int32_t   *restrict x, const int32_t   *restrict y, int rsh, int L);
void mtx_mul3x3_32x32(int32_t   * restrict z, const int32_t   *restrict x, const int32_t   *restrict y, int rsh, int L);
void mtx_mul4x4_32x32(int32_t   * restrict z, const int32_t   *restrict x, const int32_t   *restrict y, int rsh, int L);
void mtx_mul2x2f     (float32_t * restrict z, const float32_t *restrict x, const float32_t *restrict y,          int L);
void mtx_mul3x3f     (float32_t * restrict z, const float32_t *restrict x, const float32_t *restrict y,          int L);
void mtx_mul4x4f     (float32_t * restrict z, const float32_t *restrict x, const float32_t *restrict y,          int L);

// complex matrix multiply
void cmtx_mul2x2_16x16(complex_fract16 * restrict z, const complex_fract16 *restrict x, const complex_fract16 *restrict y, int rsh, int L);
void cmtx_mul3x3_16x16(complex_fract16 * restrict z, const complex_fract16 *restrict x, const complex_fract16 *restrict y, int rsh, int L);
void cmtx_mul4x4_16x16(complex_fract16 * restrict z, const complex_fract16 *restrict x, const complex_fract16 *restrict y, int rsh, int L);
void cmtx_mul2x2_32x32(complex_fract32 * restrict z, const complex_fract32 *restrict x, const complex_fract32 *restrict y, int rsh, int L);
void cmtx_mul3x3_32x32(complex_fract32 * restrict z, const complex_fract32 *restrict x, const complex_fract32 *restrict y, int rsh, int L);
void cmtx_mul4x4_32x32(complex_fract32 * restrict z, const complex_fract32 *restrict x, const complex_fract32 *restrict y, int rsh, int L);
void cmtx_mul2x2f     (complex_float   * restrict z, const complex_float   *restrict x, const complex_float   *restrict y,          int L);
void cmtx_mul3x3f     (complex_float   * restrict z, const complex_float   *restrict x, const complex_float   *restrict y,          int L);
void cmtx_mul4x4f     (complex_float   * restrict z, const complex_float   *restrict x, const complex_float   *restrict y,          int L);

// real matrix transpose
void mtx_tran2x2_16x16(int16_t   * restrict z, const int16_t   *restrict x, int L);
void mtx_tran3x3_16x16(int16_t   * restrict z, const int16_t   *restrict x, int L);
void mtx_tran4x4_16x16(int16_t   * restrict z, const int16_t   *restrict x, int L);
void mtx_tran2x2_32x32(int32_t   * restrict z, const int32_t   *restrict x, int L);
void mtx_tran3x3_32x32(int32_t   * restrict z, const int32_t   *restrict x, int L);
void mtx_tran4x4_32x32(int32_t   * restrict z, const int32_t   *restrict x, int L);
void mtx_tran2x2f     (float32_t * restrict z, const float32_t *restrict x, int L);
void mtx_tran3x3f     (float32_t * restrict z, const float32_t *restrict x, int L);
void mtx_tran4x4f     (float32_t * restrict z, const float32_t *restrict x, int L);

// complex matrix transpose
void cmtx_tran2x2_16x16(complex_fract16* restrict z, const complex_fract16 *restrict x, int L);
void cmtx_tran3x3_16x16(complex_fract16* restrict z, const complex_fract16 *restrict x, int L);
void cmtx_tran4x4_16x16(complex_fract16* restrict z, const complex_fract16 *restrict x, int L);
void cmtx_tran2x2_32x32(complex_fract32* restrict z, const complex_fract32 *restrict x, int L);
void cmtx_tran3x3_32x32(complex_fract32* restrict z, const complex_fract32 *restrict x, int L);
void cmtx_tran4x4_32x32(complex_fract32* restrict z, const complex_fract32 *restrict x, int L);
void cmtx_tran2x2f     (complex_float  * restrict z, const complex_float   *restrict x, int L);
void cmtx_tran3x3f     (complex_float  * restrict z, const complex_float   *restrict x, int L);
void cmtx_tran4x4f     (complex_float  * restrict z, const complex_float   *restrict x, int L);

// real matrix determinant
void mtx_det2x2_16x16(int16_t   * restrict z, const int16_t   *restrict x, int rsh, int L);
void mtx_det3x3_16x16(int16_t   * restrict z, const int16_t   *restrict x, int rsh, int L);
void mtx_det4x4_16x16(int16_t   * restrict z, const int16_t   *restrict x, int rsh, int L);
void mtx_det2x2_32x32(int32_t   * restrict z, const int32_t   *restrict x, int rsh, int L);
void mtx_det3x3_32x32(int32_t   * restrict z, const int32_t   *restrict x, int rsh, int L);
void mtx_det4x4_32x32(int32_t   * restrict z, const int32_t   *restrict x, int rsh, int L);
void mtx_det2x2f     (float32_t * restrict z, const float32_t *restrict x,          int L);
void mtx_det3x3f     (float32_t * restrict z, const float32_t *restrict x,          int L);
void mtx_det4x4f     (float32_t * restrict z, const float32_t *restrict x,          int L);

// complex matrix determinant
void cmtx_det2x2_16x16(complex_fract16 * restrict d, const complex_fract16 *restrict x, int rsh, int L);
void cmtx_det3x3_16x16(complex_fract16 * restrict d, const complex_fract16 *restrict x, int rsh, int L);
void cmtx_det4x4_16x16(complex_fract16 * restrict d, const complex_fract16 *restrict x, int rsh, int L);
void cmtx_det2x2_32x32(complex_fract32 * restrict d, const complex_fract32 *restrict x, int rsh, int L);
void cmtx_det3x3_32x32(complex_fract32 * restrict d, const complex_fract32 *restrict x, int rsh, int L);
void cmtx_det4x4_32x32(complex_fract32 * restrict d, const complex_fract32 *restrict x, int rsh, int L);
void cmtx_det2x2f     (complex_float   * restrict d, const complex_float   *restrict x,          int L);
void cmtx_det3x3f     (complex_float   * restrict d, const complex_float   *restrict x,          int L);
void cmtx_det4x4f     (complex_float   * restrict d, const complex_float   *restrict x,          int L);

/*-------------------------------------------------------------------------
  Quaternion to Rotation Matrix Conversion
  These functions convert sequence of unit quaternions to corresponding 
  rotation matrices

  Precision: 
  16x16  16-bit input, 16-bit output
  32x32  32-bit input, 32-bit output
  f      floating point 

  Input:
  q[L][4]    L quaternions
  L          number of matrices
  Output:
  r[L][3*3]  L rotation matrices

  Restrictions:
  q,r  should not overlap
-------------------------------------------------------------------------*/
void q2rot_16x16(int16_t   *r, const int16_t   *q, int L);
void q2rot_32x32(int32_t   *r, const int32_t   *q, int L);
void q2rotf     (float32_t *r, const float32_t *q, int L);

/*-------------------------------------------------------------------------
  These functions implement in-place matrix inversion by Gauss elimination 
  with full pivoting

  Precision: 
  f   floating point (real and complex)

  Input:
  x[N*N]      input matrix
  Output:
  x[N*N]      result
  N is 2,3 or 4

  Restrictions:
  none
-------------------------------------------------------------------------*/
void mtx_inv2x2f(float32_t *x);
void mtx_inv3x3f(float32_t *x);
void mtx_inv4x4f(float32_t *x);
void cmtx_inv2x2f(complex_float *x);
void cmtx_inv3x3f(complex_float *x);
void cmtx_inv4x4f(complex_float *x);

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
      FFT/IFFT    | Scaling options                        | Restrictions on the
                  |                                        | input dynamic range
  ----------------+---------------------------------------------------------------
  cplx24x24,      | 0 - no scaling                         | input signal < 2^23/(2*N),
                  |                                        | N-fft-size
  real24x24       | 1 - 24-bit scaling                     |        none
                  | 2 - 32-bit scaling on the first stage  |        none
                  | and 24-bit scaling later               |        none
                  | 3 - fixed scaling before each stage    |        none
------------------------------------------------------------------------------------
  cplx32x16       | 3 - fixed scaling before each stage    |        none
  cplx32x32       | 3 - fixed scaling before each stage    |        none
  cplx16x16       | 3 - fixed scaling before each stage    |        none
  cplx32x16_ie    | 3 - fixed scaling before each stage    |        none
  cplx24x24_ie    | 3 - fixed scaling before each stage    |        none
  real32x16       | 3 - fixed scaling before each stage    |        none
  real32x32       | 3 - fixed scaling before each stage    |        none
  real16x16       | 3 - fixed scaling before each stage    |        none
  real32x16_ie    | 3 - fixed scaling before each stage    |        none
  real24x24_ie    | 3 - fixed scaling before each stage    |        none
  real32x16_ie_24p| 3 - fixed scaling before each stage    |        none
  ----------------+---------------------------------------------------------------
  real24x24_ie_24p| 1 - 24-bit scaling                     |        none
  ----------------+---------------------------------------------------------------
  DCT:            |
  ----------------+---------------------------------------------------------------
  16x16           | 3 – fixed scaling before each stage    |        none
  24x24           | 3 – fixed scaling before each stage    |        none
  32x16           | 3 – fixed scaling before each stage    |        none
  ----------------+---------------------------------------------------------------

===========================================================================*/
/*-------------------------------------------------------------------------
  These functions make FFT on complex data.
  NOTES:
  1. Bit-reversing permutation is done here. 
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles
 
  Input:
  x[2*N]     complex input signal. Real and imaginary data are interleaved 
             and real data goes first
  N          FFT size
  scalingOpt scaling option
  Output:
  y[2*N]     output spectrum. Real and imaginary data are interleaved and 
             real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y should not overlap
  x,y - aligned on a 8-bytes boundary
  N   - 2^m, 16...4096

-------------------------------------------------------------------------*/
typedef const void* fft_handle_t;
// FFT handles for 16x16 
extern const fft_handle_t cfft16x16_16;
extern const fft_handle_t cfft16x16_32;
extern const fft_handle_t cfft16x16_64;
extern const fft_handle_t cfft16x16_128;
extern const fft_handle_t cfft16x16_256;
extern const fft_handle_t cfft16x16_512;
extern const fft_handle_t cfft16x16_1024;
extern const fft_handle_t cfft16x16_2048;
extern const fft_handle_t cfft16x16_4096;
// FFT handles for 32x16 
extern const fft_handle_t cfft16_16;
extern const fft_handle_t cfft16_32;
extern const fft_handle_t cfft16_64;
extern const fft_handle_t cfft16_128;
extern const fft_handle_t cfft16_256;
extern const fft_handle_t cfft16_512;
extern const fft_handle_t cfft16_1024;
extern const fft_handle_t cfft16_2048;
extern const fft_handle_t cfft16_4096;
// FFT handles for 24x24
extern const fft_handle_t cfft24_16;
extern const fft_handle_t cfft24_32;
extern const fft_handle_t cfft24_64;
extern const fft_handle_t cfft24_128;
extern const fft_handle_t cfft24_256;
extern const fft_handle_t cfft24_512;
extern const fft_handle_t cfft24_1024;
extern const fft_handle_t cfft24_2048;
extern const fft_handle_t cfft24_4096;
// FFT handles for 32x32
extern const fft_handle_t cfft32_16;
extern const fft_handle_t cfft32_32;
extern const fft_handle_t cfft32_64;
extern const fft_handle_t cfft32_128;
extern const fft_handle_t cfft32_256;
extern const fft_handle_t cfft32_512;
extern const fft_handle_t cfft32_1024;
extern const fft_handle_t cfft32_2048;
extern const fft_handle_t cfft32_4096;
int fft_cplx32x32(int32_t* y, int32_t * x, fft_handle_t h, int scalingOption);
int fft_cplx24x24( f24* y,f24* x,fft_handle_t h,int scalingOption);
int fft_cplx32x16( int32_t* y,int32_t* x,fft_handle_t h,int scalingOption);
int fft_cplx16x16( int16_t* y,int16_t* x,fft_handle_t h,int scalingOption);

/*-------------------------------------------------------------------------
  FFT on real data forming half of spectrum

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles

  NOTES:
  1. Bit-reversal reordering is done here. 
  2. FFT runs in-place so INPUT DATA WILL APPEAR DAMAGED after the call.
  3. Real data FFT function calls fft_cplx() to apply complex FFT of size
     N/2 to input data and then transforms the resulting spectrum.

  Input:
  x[N]         - input signal
  N            - FFT size
  scalingOpt   - scaling option:
                0 - no scaling
                1 - 24-bit scaling
                2 - 32-bit scaling on the first stage and 24-bit scaling later
                3 - fixed scaling
  Output:
  y[(N/2+1)*2] - output spectrum (positive side)

  Restrictions:
  Arrays should not overlap
  x,y - aligned on a 8-bytes boundary
  N   - 2^m: 32...8192

-------------------------------------------------------------------------*/
// FFT handles for 16x16
extern const fft_handle_t rfft16x16_32;
extern const fft_handle_t rfft16x16_64;
extern const fft_handle_t rfft16x16_128;
extern const fft_handle_t rfft16x16_256;
extern const fft_handle_t rfft16x16_512;
extern const fft_handle_t rfft16x16_1024;
extern const fft_handle_t rfft16x16_2048;
extern const fft_handle_t rfft16x16_4096;
extern const fft_handle_t rfft16x16_8192;
// FFT handles for 32x16 
extern const fft_handle_t rfft16_32;
extern const fft_handle_t rfft16_64;
extern const fft_handle_t rfft16_128;
extern const fft_handle_t rfft16_256;
extern const fft_handle_t rfft16_512;
extern const fft_handle_t rfft16_1024;
extern const fft_handle_t rfft16_2048;
extern const fft_handle_t rfft16_4096;
extern const fft_handle_t rfft16_8192;
// FFT handles for 24x24
extern const fft_handle_t rfft24_32;
extern const fft_handle_t rfft24_64;
extern const fft_handle_t rfft24_128;
extern const fft_handle_t rfft24_256;
extern const fft_handle_t rfft24_512;
extern const fft_handle_t rfft24_1024;
extern const fft_handle_t rfft24_2048;
extern const fft_handle_t rfft24_4096;
extern const fft_handle_t rfft24_8192;
// FFT handles for 32x32
extern const fft_handle_t rfft32_32;
extern const fft_handle_t rfft32_64;
extern const fft_handle_t rfft32_128;
extern const fft_handle_t rfft32_256;
extern const fft_handle_t rfft32_512;
extern const fft_handle_t rfft32_1024;
extern const fft_handle_t rfft32_2048;
extern const fft_handle_t rfft32_4096;
extern const fft_handle_t rfft32_8192;
int fft_real32x32(int32_t* y, int32_t* x, fft_handle_t h, int scalingOpt);
int fft_real24x24( f24* y,f24* x,fft_handle_t h,int scalingOpt);
int fft_real32x16( int32_t* y,int32_t* x,fft_handle_t h,int scalingOpt);
int fft_real16x16( int16_t* y,int16_t* x,fft_handle_t h,int scalingOpt);

/*-------------------------------------------------------------------------
  These functions make inverse FFT on complex data.
  NOTES:
  1. Bit-reversing permutation is done here. 
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles

  Input:
  x[2*N]     complex input spectrum. Real and imaginary data are interleaved 
             and real data goes first
  N          FFT size
  scalingOpt scaling option
  Output:
  y[2*N]     complex output signal. Real and imaginary data are interleaved 
             and real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y should not overlap
  x,y - aligned on a 8-bytes boundary
  N   - 2^m: 16...4096

-------------------------------------------------------------------------*/
// FFT handles for 16x16
extern const fft_handle_t cifft16x16_16;
extern const fft_handle_t cifft16x16_32;
extern const fft_handle_t cifft16x16_64;
extern const fft_handle_t cifft16x16_128;
extern const fft_handle_t cifft16x16_256;
extern const fft_handle_t cifft16x16_512;
extern const fft_handle_t cifft16x16_1024;
extern const fft_handle_t cifft16x16_2048;
extern const fft_handle_t cifft16x16_4096;
// FFT handles for 32x16 
extern const fft_handle_t cifft16_16;
extern const fft_handle_t cifft16_32;
extern const fft_handle_t cifft16_64;
extern const fft_handle_t cifft16_128;
extern const fft_handle_t cifft16_256;
extern const fft_handle_t cifft16_512;
extern const fft_handle_t cifft16_1024;
extern const fft_handle_t cifft16_2048;
extern const fft_handle_t cifft16_4096;
// FFT handles for 24x24
extern const fft_handle_t cifft24_16;
extern const fft_handle_t cifft24_32;
extern const fft_handle_t cifft24_64;
extern const fft_handle_t cifft24_128;
extern const fft_handle_t cifft24_256;
extern const fft_handle_t cifft24_512;
extern const fft_handle_t cifft24_1024;
extern const fft_handle_t cifft24_2048;
extern const fft_handle_t cifft24_4096;
// FFT handles for 32x32
extern const fft_handle_t cifft32_16;
extern const fft_handle_t cifft32_32;
extern const fft_handle_t cifft32_64;
extern const fft_handle_t cifft32_128;
extern const fft_handle_t cifft32_256;
extern const fft_handle_t cifft32_512;
extern const fft_handle_t cifft32_1024;
extern const fft_handle_t cifft32_2048;
extern const fft_handle_t cifft32_4096;

int ifft_cplx32x32(int32_t * y, int32_t* x, fft_handle_t h, int scalingOption);
int ifft_cplx24x24( f24* y,f24* x,fft_handle_t h,int scalingOption);
int ifft_cplx32x16( int32_t* y,int32_t* x,fft_handle_t h,int scalingOption);
int ifft_cplx16x16( int16_t* y,int16_t* x,fft_handle_t h,int scalingOption);

/*-------------------------------------------------------------------------
  Inverse FFT forming real data

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles
  
  NOTES:
  1. Bit-reversing reordering is done here. 
  2. IFFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after call.
  3. Inverse FFT function for real signal transforms the input spectrum and 
     then calls ifft_cplx() with FFT size set to N/2.

  Input:
  x[(N/2+1)*2]	input spectrum. Real and imaginary data are interleaved and 
                real data goes first
   N            FFT size
   scalingOpt   scaling option:
                0 - no scaling
                1 - 24-bit scaling
                2 - 32-bit scaling on the first stage and 24-bit scaling later
                3 - fixed scaling
   Output:
   y[N]         real output signal

  Restrictions:
  Arrays should not overlap
  x,y - aligned on a 8-bytes boundary
  N   - 2^m: 32...8192

-------------------------------------------------------------------------*/
// FFT handles for 16x16
extern const fft_handle_t rifft16x16_32;
extern const fft_handle_t rifft16x16_64;
extern const fft_handle_t rifft16x16_128;
extern const fft_handle_t rifft16x16_256;
extern const fft_handle_t rifft16x16_512;
extern const fft_handle_t rifft16x16_1024;
extern const fft_handle_t rifft16x16_2048;
extern const fft_handle_t rifft16x16_4096;
extern const fft_handle_t rifft16x16_8192;
// FFT handles for 32x16 
extern const fft_handle_t rifft16_32;
extern const fft_handle_t rifft16_64;
extern const fft_handle_t rifft16_128;
extern const fft_handle_t rifft16_256;
extern const fft_handle_t rifft16_512;
extern const fft_handle_t rifft16_1024;
extern const fft_handle_t rifft16_2048;
extern const fft_handle_t rifft16_4096;
extern const fft_handle_t rifft16_8192;
// FFT handles for 24x24
extern const fft_handle_t rifft24_32;
extern const fft_handle_t rifft24_64;
extern const fft_handle_t rifft24_128;
extern const fft_handle_t rifft24_256;
extern const fft_handle_t rifft24_512;
extern const fft_handle_t rifft24_1024;
extern const fft_handle_t rifft24_2048;
extern const fft_handle_t rifft24_4096;
extern const fft_handle_t rifft24_8192;
// FFT handles for 32x32
extern const fft_handle_t rifft32_32;
extern const fft_handle_t rifft32_64;
extern const fft_handle_t rifft32_128;
extern const fft_handle_t rifft32_256;
extern const fft_handle_t rifft32_512;
extern const fft_handle_t rifft32_1024;
extern const fft_handle_t rifft32_2048;
extern const fft_handle_t rifft32_4096;
extern const fft_handle_t rifft32_8192;

int ifft_real32x32(int32_t* y, int32_t* x, fft_handle_t h, int scalingOpt);
int ifft_real24x24( f24* y,f24* x,fft_handle_t h,int scalingOpt);
int ifft_real32x16( int32_t* y,int32_t* x,fft_handle_t h,int scalingOpt);
int ifft_real16x16( int16_t* y,int16_t* x,fft_handle_t h,int scalingOpt);

/*-------------------------------------------------------------------------
  These functions make FFT on complex data with optimized memory usage.
  Scaling  : Fixed data scaling at each stage
  NOTES:
  1. Bit-reversing reordering is done here.
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call
  3. Forward/inverse FFT of size N may be supplied with constant data
     (twiddle factors) of a larger-sized FFT = N*twdstep.

  Precision: 
  24x24_ie      24-bit input/outputs, 24-bit twiddles
  32x16_ie      32-bit input/outputs, 16-bit twiddles
  f             floating point
 
  Input:
  x[N]                  complex input signal. Real and imaginary data are interleaved 
                        and real data goes first
  twd[N*twdstep*3/4]    twiddle factor table of a complex-valued FFT of size N*twdstep
  N                     FFT size
  twdstep               twiddle step 
  scalingOpt            scaling option = 3 (fixed scaling)
  Output:
  y[N]                  output spectrum. Real and imaginary data are interleaved and 
                        real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y - should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 256, 512, 1024 for fixed-point routines, power of 2 and >=8 for 
        floating point

-------------------------------------------------------------------------*/
int fft_cplx32x16_ie(complex_fract32* y,complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int fft_cplx24x24_ie(complex_fract32* y,complex_fract32* x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int fft_cplxf_ie    (complex_float  * y, complex_float * x, const complex_float  * twd, int twdstep, int N );

/*-------------------------------------------------------------------------
  These functions make FFT on real data with optimized memory usage.
  Scaling  : 
      24x24_ie        - Fixed data scaling at each stage
      32x16_ie        - Fixed data scaling at each stage
      32x16_ie_24p    - Fixed data scaling at each stage
      24x24_ie_24p    - 24-bit scaling
    
  NOTES:
  1. Bit-reversing reordering is done here.
  2. INPUT DATA MAY APPEAR DAMAGED after the call
  3. FFT functions may use input and output buffers for temporal
  storage of intermediate 32-bit data, so FFT functions
  with 24-bit packed I/O (Nx3-byte data) require that the
  buffers are large enough to keep Nx4-byte data
  4. Forward/inverse FFT of size N may be supplied with constant data
  (twiddle factors) of a larger-sized FFT = N*twdstep.

  Precision:
  24x24_ie      24-bit input/outputs, 24-bit data, 24-bit twiddles
  32x16_ie      32-bit input/outputs, 32-bit data, 16-bit twiddles
  24x24_ie_24p  24-bit packed input/outputs, 24-bit data, 24-bit twiddles
  32x16_ie_24p  24-bit packed input/outputs, 32-bit data, 16-bit twiddles
  f             floating point

  Input:
  x - real input signal. Real and imaginary data are interleaved
  and real data goes first:
  --------------+----------+-----------------+-----------
  Function      |   Size   |  Allocated Size |  type    |
  --------------+----------+-----------------+-----------
  24x24_ie      |     N    |      N          |   f24    |
  32x16_ie      |     N    |      N          |  int32_t |
  24x24_ie_24p  |     3*N  |      4*N+8      |  uint8_t |
  32x16_ie_24p  |     3*N  |      4*N+8      |  uint8_t |
  --------------+----------+-----------------+-----------

  twd[N*twdstep*3/4]    twiddle factor table of a complex-valued 
                        FFT of size N*twdstep
  N                     FFT size
  twdstep               twiddle step
  scalingOpt            24x24_ie        - 3 (Fixed scaling)
                        32x16_ie        - 3 (Fixed scaling)
                        32x16_ie_24p    - 3 (Fixed scaling)
                        24x24_ie_24p    - 1 (24-bit scaling)

  Output:
  y - output spectrum. Real and imaginary data are interleaved and
  real data goes first:
  --------------+----------+-----------------+----------------
  Function      |   Size   |  Allocated Size |  type         |
  --------------+----------+-----------------+----------------
  24x24_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  32x16_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  24x24_ie_24p  |  3*(N+2) |      4*N+8      |  uint8_t      |
  32x16_ie_24p  |  3*(N+2) |      4*N+8      |  uint8_t      |
  f_ie          |   N/2+1  |      N/2+1      |complex_float  |
  --------------+----------+-----------------+----------------

  Returned value: total number of right shifts occurred during scaling
  procedure

  Restrictions:
  x,y - should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 256, 512, 1024 for fixed point functions, power of two and >=8 for 
        floating point

-------------------------------------------------------------------------*/
int fft_real32x16_ie    (complex_fract32* y, int32_t  * x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int fft_real24x24_ie    (complex_fract32* y, f24      * x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int fft_real32x16_ie_24p(      uint8_t  * y,  uint8_t * x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int fft_real24x24_ie_24p(      uint8_t  * y,  uint8_t * x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int fft_realf_ie        (complex_float  * y,float32_t * x, const complex_float  * twd, int twdstep, int N);

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
int ifft_cplx32x16_ie(complex_fract32* y,complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int ifft_cplx24x24_ie(complex_fract32* y,complex_fract32* x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int ifft_cplxf_ie    (complex_float  * y,complex_float  * x, const complex_float  * twd, int twdstep, int N);

/*-------------------------------------------------------------------------
  These functions make inverse FFT on real data with optimized memory usage.
  Scaling  : 
      24x24_ie        - Fixed data scaling at each stage
      32x16_ie        - Fixed data scaling at each stage
      32x16_ie_24p    - Fixed data scaling at each stage
      24x24_ie_24p    - 24-bit scaling
  NOTES:
  1. Bit-reversing reordering is done here.
  2. INPUT DATA MAY APPEAR DAMAGED after the call
  3. FFT functions may use input and output buffers for temporal
  storage of intermediate 32-bit data, so FFT functions
  with 24-bit packed I/O (Nx3-byte data) require that the
  buffers are large enough to keep Nx4-byte data
  4. Forward/inverse FFT of size N may be supplied with constant data
  (twiddle factors) of a larger-sized FFT = N*twdstep.

  Precision:
  24x24_ie      24-bit input/outputs, 24-bit data, 24-bit twiddles
  32x16_ie      32-bit input/outputs, 32-bit data, 16-bit twiddles
  24x24_ie_24p  24-bit packed input/outputs, 24-bit data, 24-bit twiddles
  32x16_ie_24p  24-bit packed input/outputs, 32-bit data, 16-bit twiddles
  f_ie          floating point

  Input:
  x - complex input signal. Real and imaginary data are interleaved
  and real data goes first:
  --------------+----------+-----------------+----------------
  Function      |   Size   |  Allocated Size |       type    |
  --------------+----------+-----------------+----------------
  24x24_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  32x16_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  24x24_ie_24p  |   3*(N+2)|      4*N+8      |       uint8_t |
  32x16_ie_24p  |   3*(N+2)|      4*N+8      |       uint8_t |
  f_ie          |   N/2+1  |      N/2+1      | complex_float |
  --------------+----------+-----------------+----------------

  twd[2*N*twdstep*3/4]  twiddle factor table of a complex-valued FFT of size N*twdstep
  N                     FFT size
  twdstep               twiddle step
  scalingOpt            24x24_ie        - 3 (Fixed scaling)
                        32x16_ie        - 3 (Fixed scaling)
                        32x16_ie_24p    - 3 (Fixed scaling)
                        24x24_ie_24p    - 1 (24-bit scaling)
  Output:
  y - output spectrum. Real and imaginary data are interleaved and
  real data goes first:
  --------------+----------+-----------------+-----------
  Function      |   Size   |  Allocated Size |  type    |
  --------------+----------+-----------------+-----------
  24x24_ie      |      N   |      N          |   f24    |
  32x16_ie      |      N   |      N          |  int32_t |
  24x24_ie_24p  |    3*N   |      4*N+8      |  uint8_t |
  32x16_ie_24p  |    3*N   |      4*N+8      |  uint8_t |
  f_ie          |      N   |      N          | float32_t|
  --------------+----------+-----------------+-----------

  Returned value: total number of right shifts occurred during scaling
  procedure

  Restrictions:
  x,y - should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 256, 512, 1024 for fixed point functions, power of two and >=8 for 
        floating point

-------------------------------------------------------------------------*/
int ifft_real32x16_ie    (  int32_t* y,complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int ifft_real24x24_ie    (      f24* y,complex_fract32* x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int ifft_real32x16_ie_24p(  uint8_t* y,       uint8_t * x, const complex_fract16* twd, int twdstep, int N, int scalingOpt);
int ifft_real24x24_ie_24p(  uint8_t* y,       uint8_t * x, const complex_fract32* twd, int twdstep, int N, int scalingOpt);
int ifft_realf_ie        (float32_t* y, complex_float * x, const  complex_float * twd, int twdstep, int N);

/*-------------------------------------------------------------------------
  Discrete Cosine Transform :
  These functions apply DCT (Type II) to input
  NOTES:
  1. DCT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call.

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles
  f      floating point

  Input:
  x[N]          input signal
  N             DCT size
  scalingOpt    scaling option
  Output:
  y[N]          transform output
  return ed value:
  total number of right shifts occurred during scaling procedure (always 5 
  for fixed point functions and 0 for floating point function)
  Restriction:
  x,y should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 32 for fixed point routines, 32 or 64 for floating point

-------------------------------------------------------------------------*/
int dct_32x32( int32_t   * y, int32_t   * x,int N, int scalingOpt);
int dct_24x24( f24       * y, f24       * x,int N, int scalingOpt);
int dct_32x16( int32_t   * y, int32_t   * x,int N, int scalingOpt);
int dct_16x16( int16_t   * y, int16_t   * x,int N, int scalingOpt);
int dctf     ( float32_t * y,float32_t  * x,int N                );

/*===========================================================================
  Identification Routines
  NatureDSP_Signal_get_library_version     Library Version Request
  NatureDSP_Signal_get_library_api_version Library API Version Request
===========================================================================*/

/*-------------------------------------------------------------------------
   Returns library version string
   Parameters:
    Input:
    none:
    Output:
     version_string   Pre-allocated buffer for version string.
   Restrictions:
     version_string must points to a buffer large enough to hold up to
     30 characters.
-------------------------------------------------------------------------*/
void NatureDSP_Signal_get_library_version(char *version_string);

/*-------------------------------------------------------------------------
   Returns library API version string
   Parameters:
    Input:
    none:
    Output:
    version_string   Pre-allocated buffer for API version string.
   Restrictions:
     version_string must points to a buffer large enough to hold up to
     30 characters.
-------------------------------------------------------------------------*/
void NatureDSP_Signal_get_library_api_version(char *version_string);

/*-------------------------------------------------------------------------
Returns non-zero if given function (by its address) is supported by
specific processor capabilities
Parameters:
    Input:
     fun    one of function from the list above

NOTE:
in gcc/xcc environment, calls of this function is not neccessary - if 
function pointer is non-zero it means it is supported. VisualStudio linker 
does not support section removal so this function might be used for 
running library under MSVC environment
-------------------------------------------------------------------------*/
typedef void(*NatureDSP_Signal_funptr)();
int __NatureDSP_Signal_isPresent(NatureDSP_Signal_funptr fun);
#define NatureDSP_Signal_isPresent(fun) __NatureDSP_Signal_isPresent((NatureDSP_Signal_funptr)fun)

#ifdef __cplusplus
}
#endif

#endif//__NATUREDSP_SIGNAL_H__
