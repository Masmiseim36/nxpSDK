/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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
  NatureDSP Signal Processing Library. FIR part
    Real block FIR filter, 24x24-bit, packed delay line and coefficient storage
    C code optimized for HiFi4
  IntegrIT, 2006-2014
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "bkfir24x24p_common.h"

/*-------------------------------------------------------------------------
  Real FIR filter.
  Computes a real FIR filter (direct-form) using IR stored in vector h. The 
  real data input is stored in vector x. The filter output result is stored 
  in vector y. The filter calculates N output samples using M coefficients 
  and requires last M-1 samples in the delay line.
  NOTE: 
  1. User application is not responsible for management of delay lines
  2. User has an option to set IR externally or copy from original location 
     (i.e. from the slower constant memory). In the first case, user is 
     responsible for right alignment, ordering and zero padding of filter 
     coefficients - usually array is composed from zeroes (left padding), 
     reverted IR and right zero padding.


  Precision: 
  16x16    16-bit data, 16-bit coefficients, 16-bit outputs. Ordinary variant 
           and stereo
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  32x32    32-bit data, 32-bit coefficients, 32-bit outputs. Ordinary variant 
           and stereo
  32x32ep  the same as above but using 72-bit accumulator for intermediate 
           computations
  f        floating point. Ordinary variant and stereo

  Input:
  x[N*S]   input samples, Q31, Q15, floating point
  h[M]     filter coefficients in normal order, Q31, Q15, floating point
  hl[M]    for stereo filters: filter coefficients for left channel
  hr[M]    for stereo filters: filter coefficients for right channel
  N        length of sample block, should be a multiple of 4
  M        length of filter, should be a multiple of 4
  extIR    if zero, IR is copied from original location, otherwise not
           but user should keep alignment, order of coefficients 
           and zero padding requirements shown below
  S        1 for ordinary (single channel) filters, 2 - for stereo variant
  
  Output:
  y[N*S]   output samples, Q31, Q15, floating point

  Alignment, ordering and zero padding for external IR  (extIR!=0)
  ------------------------+----------+--------------+--------------+----------------
  Function                |Alignment,|Left zero     |   Coefficient| Right zero 
                          | bytes    |padding, bytes|   order      | padding, bytes
  ------------------------+----------+--------------+--------------+----------------
  bkfir16x16_init         |     8    |      2       |  inverted    |  6
  bkfir32x16_init (M>32)  |     8    |     10       |  inverted    |  6
  bkfir32x16_init (M<=32) |     8    |      2       |  inverted    |  6
  bkfir32x32_init         |     8    |      4       |  inverted    |  12
  bkfir32x32ep_init       |     8    |      4       |  inverted    |  12
  bkfirf_init             |     8    |      0       |  direct      |  0
  stereo_bkfir16x16_init  |     8    |      2       |  inverted    |  6
  stereo_bkfir32x32_init  |     8    |      4       |  inverted    |  12
  stereo_bkfirf_init      |     8    |      0       |  direct      |  0
  ------------------------+----------+--------------+--------------+----------------

  Restrictions:
  x, y     should not be overlapping
  x, h     aligned on a 8-bytes boundary
  N, M     multiples of 4 
-------------------------------------------------------------------------*/
/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define MIN(a,b)  ( (a) < (b) ? (a) : (b) )

static f24p store_f24p( int32_t x )
{
  f24p p;

  p.b2 = ( int8_t)( x >> 24 );
  p.b1 = (uint8_t)( x >> 16 );
  p.b0 = (uint8_t)( x >>  8 );

  return (p);
}

/* Allocation routine for filters. Returns: size of memory in bytes to be allocated */
size_t bkfir24x24p_alloc( int M, int extIR )
{
  int _M;

  NASSERT(  M > 0 );
  NASSERT (  !( M & 3 ) );
  _M = M + (-M&4);
  return ( ALIGNED_SIZE( sizeof( bkfir24x24p_t ), 4 )
           + // Delay line
           ALIGNED_SIZE( ( _M + 8 )*sz_f24p, 8 )
           + // Filter coefficients
           (extIR?0:ALIGNED_SIZE( ( _M + 8)*sz_f24p, 8 )) );
} // bkfir24x24p_alloc()

/* Initialization for filters. Returns: handle to the object */
bkfir24x24p_handle_t bkfir24x24p_init( void *         objmem, 
                                       int            M,
                                       int            extIR ,
                                 const f24 * restrict h )
{
  bkfir24x24p_ptr_t bkfir;

  void * ptr;
  f24p * coef;
  f24p * delLine;  
 
  int    m,n;
  int   _M;

  NASSERT( objmem && M > 0 && h );

  NASSERT(  !( M & 3 ) && IS_ALIGN( h ) );

  // Complement the filter length to the next multiple of 8.
  
  _M = M + (-M&4);

  //
  // Partition the memory block
  //

  ptr     = objmem;
  bkfir   = (bkfir24x24p_ptr_t)ALIGNED_ADDR( ptr, 8 );
  ptr     = bkfir + 1;
  delLine = (f24p *)ALIGNED_ADDR( ptr, 8 );
  ptr     = delLine + _M + 8;
  if (extIR) coef  = (f24p*)h;
  else
  {
      coef=(f24p *)ALIGNED_ADDR( ptr,8 );
      ptr     = coef + _M + 4;
  }
  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)bkfir24x24p_alloc( M,extIR ) );

  //
  // Copy the filter coefficients in reverted order and zero the delay line.
  //

  if (extIR==0)
  {
    for ( n=0; n<(-M&4)+1; n++ )
    {
      coef[n] = store_f24p( 0 );
    }

    for (m=1; m<M+1; m++,n++ )
    {
      coef[n] = store_f24p( h[M-m] );
    }

    for (; n<_M+8; n++ )
    {
      coef[n] = store_f24p( 0 );
    }
  }

  //
  // Zero the delay line.
  //

  for ( m=0; m<_M+8; m++ )
  {
    delLine[m] = store_f24p( 0 );
  }

  //
  // Initialize the filter instance.
  //

  bkfir->magic     = BKFIR24X24P_MAGIC;
  bkfir->M         = _M;
  bkfir->coef      = coef;
  bkfir->delayLine = delLine;
  bkfir->delayLen  = _M+8;
  bkfir->wrIx      = 0;

  return (bkfir);
} // bkfir24x24p_init()
