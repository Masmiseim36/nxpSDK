/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
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
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, floating point
*/

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

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, latrf_process, ( latrf_handle_t _latr, 
                    float32_t * restrict     r,
                    const float32_t*         x, int N ))
DISCARD_FUN(latr32x16_handle_t, latrf_init, ( void *             objmem, 
                               int                M,
                               const float32_t * restrict k,
                               float32_t            scale ))
DISCARD_FUN(size_t, latrf_alloc, ( int M ))                               

#else
/* NON OPTIMIZED REFERENCE CODE: to be use it for educational purposes only */
#include "baseop.h"
#include "latrf_common.h"

/* Instance pointer validation number. */
#define MAGIC     0x52efa963

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_f sizeof(float32_t)

// Determine the memory area size for a filter instance.
size_t latrf_alloc( int M )
{
  ASSERT( M > 0 );

  return ( ALIGNED_SIZE( sizeof( latrf_t ), 4 )
           + // M delay elements
           ALIGNED_SIZE( M*sz_f, sz_f )
           + // M reflection coefficients
           ALIGNED_SIZE( M*sz_f, sz_f ) );

} // latrf_alloc()

// Given a memory area of sufficient size, initialize a filter instance and 
// return a handle to it.
latr32x16_handle_t latrf_init( void *             objmem, 
                               int                M,
                               const float32_t * restrict k,
                               float32_t            scale )
{
  latrf_ptr_t latr;
  float32_t *     delayLine;
  float32_t *     coef;
  void *          ptr;

  int m;

  ASSERT( objmem && M > 0 && k );

  //
  // Partition the memory block
  //

  ptr = objmem;

  latr      = (latrf_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr       = latr + 1;
  delayLine = (float32_t*)ALIGNED_ADDR( ptr, sz_f );
  ptr       = delayLine + M;
  coef      = (float32_t*)ALIGNED_ADDR( ptr, sz_f );
  ptr       = coef + M;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)latrf_alloc( M ) );

  //
  // Copy reflection coefficients, zero the delay line.
  //

  for ( m=0; m<M; m++ )
  {
    coef[m] = k[m];

    delayLine[m] = 0;
  }

  //
  // Initialize the filter instance.
  //

  latr->magic     = MAGIC;
  latr->M         = M;
  latr->scale     = scale;
  latr->coef      = coef;
  latr->delayLine = delayLine;

  {
        typedef latrf_processFxns* fxns_ptr;
        static const fxns_ptr  fxns[]=
        {
            latrf_process1,
            latrf_process2,
            latrf_process3,
            latrf_process4,
            latrf_process5,
            latrf_process6,
            latrf_process7,
            latrf_process8
        };
        latr->fxns= (M>=1 && M<=8) ? fxns[M-1] : &latrf_processX;
  }
  return (latr);

} // latrf_init()

// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void latrf_process( latrf_handle_t _latr, 
                    float32_t * restrict     r,
                    const float32_t*         x, int N )
{
    struct tag_latrf_t* latr=(struct tag_latrf_t* )_latr;
    NASSERT( latr && latr->magic == MAGIC);
    NASSERT( latr->fxns);
    NASSERT( r && x );
    NASSERT( latr->M>0);
    if(N>0)    latr->fxns(latr,r,x,N);
}
#endif

