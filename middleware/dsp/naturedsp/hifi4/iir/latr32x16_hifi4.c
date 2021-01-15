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
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, 32x16-bit
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/


/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_iir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "latr32x16_common.h"
/*-------------------------------------------------------------------------
  Lattice Block Real IIR
  Computes a real cascaded lattice autoregressive IIR filter using reflection 
  coefficients stored in vector k. The real data input are stored in vector x.
  The filter output result is stored in vector r. Input scaling is done before 
  the first cascade for normalization and overflow protection.

  Precision: 
  16x16   16-bit data, 16-bit coefficients
  32x16   32-bit data, 16-bit coefficients
  32x32   32-bit data, 32-bit coefficients
  f       single precision floating point

  Input:
  N       length of input sample block
  M       number of reflection coefficients
  scale   input scale factor g, Q15, Q31 or floating point
  k[M]    reflection coefficients, Q15, Q31 or floating point
  x[N]    input samples, Q15, Q31 or floating point
  Output:
  r[N]    output data, Q15, Q31 or floating point

  Restriction:
  x,r,k   should not overlap

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  M - from the range 1...8
-------------------------------------------------------------------------*/
/* Instance pointer validation number. */
#define MAGIC     0x1c6951b2

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i16 sizeof(int16_t)
#define sz_i32 sizeof(int32_t)


/* Filter instance structure. */
typedef struct tag_latr32x16_t
{
  uint32_t        magic;     // Instance pointer validation number
  int             M;         // Lattice filter order
  proc_fxn_t *    procFxn;   // Custom data processing function
  int16_t         scale;     // Input signal prescaling factor, Q15
  const int16_t * coef;      // M reflection coefficients, Q15
  int32_t *       delayLine; // M delay elements, Q30

} latr32x16_t, *latr32x16_ptr_t;

/* Allocation routine for lattice filters. Returns: size of memory in bytes to be allocated */
size_t latr32x16_alloc( int M )
{
  int M2, M4;

  NASSERT(  M > 0 );

  // Complement M to the next multiples of 2 and 4.
  M2 = M + ( -M & 1 );
  M4 = M + ( -M & 3 );

  return ( ALIGNED_SIZE( sizeof( latr32x16_t ), 4 )
           + // M delay elements
           ALIGNED_SIZE( M2*sz_i32, 8 )
           + // M reflection coefficients
           ALIGNED_SIZE( M4*sz_i16, 8 ) );

} // latr32x16_alloc()

/* Initialization routine for lattice filters. Returns: handle to the object */
latr32x16_handle_t latr32x16_init( void *             objmem, 
                                   int                M,
                             const int16_t * restrict k,
                                   int16_t            scale )
{
  latr32x16_ptr_t latr;

  proc_fxn_t * procFxn;
  int32_t *    delayLine;
  int16_t *    coef16;
  int32_t *    coef32;
  void *       ptr;

  int M2, M4;
  int m;

  NASSERT( objmem && M > 0 && k );

  //
  // Select the data processing function.
  //

  procFxn = ( M == 1 ? &latr32x16_1_proc :
              M == 2 ? &latr32x16_2_proc :
              M == 3 ? &latr32x16_3_proc :
              M == 4 ? &latr32x16_4_proc :
              M == 5 ? &latr32x16_5_proc :
              M == 6 ? &latr32x16_6_proc :
              M == 7 ? &latr32x16_7_proc : 
              M == 8 ? &latr32x16_8_proc : &latr32x16_X_proc );

  //
  // Partition the memory block
  //

  // Complement M to the next multiples of 2 and 4.
  M2 = M + ( -M & 1 );
  M4 = M + ( -M & 3 );

  ptr       = objmem;
  latr      = (latr32x16_ptr_t)(ALIGNED_ADDR( ptr, 4 ));
  ptr       = latr + 1;
  delayLine = (int32_t*)(ALIGNED_ADDR( ptr, 8 ));
  ptr       = delayLine + M2;
  coef16    = (int16_t*)(ALIGNED_ADDR( ptr, 8 ));
  ptr       = coef16 + M4;

  NASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)latr32x16_alloc( M ) );

  //
  // Copy reflection coefficients.
  //

  if ( procFxn == latr32x16_X_proc )
  {
    //
    // For a lattice filter based on the generic processing function, pack
    // the coefficients into 32-bit words such that the higher numbered
    // coefficient in each pair occupies 16 MSBs. Pairs are stored in reverted
    // order. The processing function uses 32-bit memory loads to read
    // coefficients, and the code is invariant over the endianness selection.
    //

    coef32 = (int32_t*)coef16;

    for ( m=0; m<(M>>1); m++ )
    {
      coef32[m] = ( ( int32_t)k[M-1-2*m-0] << 16 ) |
                  ( (uint16_t)k[M-1-2*m-1] <<  0 );
    }

    if ( M & 1 )
    {
      coef32[m++] = ( (int32_t)k[0] << 16 );
    }

    for ( ; m<(M4>>1); m++ )
    {
      coef32[m] = 0;
    }
  }
  else
  {
    //
    // For a lattice filter posessing custom data processing function, just copy
    // the coefficients in reverted order, and append zeros to make the whole
    // number of coefficients a multiple of 4.
    //

    for ( m=0; m<M; m++ )
    {
      coef16[m] = k[M-1-m];
    }

    for ( ; m<M4; m++ )
    {
      coef16[m] = 0;
    }
  }

  //
  // Zero the delay line.
  //

  for ( m=0; m<M2; m++ )
  {
    delayLine[m] = 0;
  }

  //
  // Initialize the filter instance.
  //

  latr->magic     = MAGIC;
  latr->M         = M;
  latr->procFxn   = procFxn;
  latr->scale     = scale;
  latr->coef      = coef16;
  latr->delayLine = delayLine;

  return (latr);

} // latr32x16_init()


void latr32x16_process( latr32x16_handle_t _latr, 
                        f24 * restrict     r,
                  const f24 *              x , int N)
{
  latr32x16_ptr_t latr = (latr32x16_ptr_t)_latr;

  NASSERT( latr && latr->magic == MAGIC && r && x );

  NASSERT( latr->procFxn );

  ( *latr->procFxn )( r, x,
                      latr->delayLine,
                      latr->coef,
                      latr->scale,
                      N,
                      latr->M );

} // latr32x16_process()
