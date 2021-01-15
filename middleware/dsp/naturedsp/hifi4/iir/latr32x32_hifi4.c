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
    Lattice Block Real IIR, 32x32-bit
    Reference C code
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_iir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "latr32x32_common.h"
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
#define MAGIC     0xfc660883

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

typedef struct tag_latr32x32_t
{
  uint32_t    magic;     // Instance pointer validation number
  int         M;         // Lattice filter order
  proc_fxn_t* proc_fxn;  // processing function optimized for given M
  int32_t         scale;     // Input signal prescaling factor, Q31
  const int32_t * coef;      // M reflection coefficients, Q31
  int32_t *       delayLine; // M delay elements, Q30
} latr32x32_t, *latr32x32_ptr_t;

typedef proc_fxn_t* funptr;
static const funptr funs[]={ NULL,
                             latr32x32_1_proc,latr32x32_2_proc,latr32x32_3_proc,latr32x32_4_proc,
                             latr32x32_5_proc,latr32x32_6_proc,latr32x32_7_proc,latr32x32_8_proc};


/* Allocation routine for lattice filters. Returns: size of memory in bytes to be allocated */
size_t latr32x32_alloc( int M )
{
  NASSERT( M > 0 );

  return ( ALIGNED_SIZE( sizeof( latr32x32_t ), 4 )
           + // M delay elements
           ALIGNED_SIZE( M*sizeof(int32_t), 8 )
           + // M reflection coefficients
           ALIGNED_SIZE( M*sizeof(int32_t), 8 ) );  

} // latr32x32_alloc()

/* Initialization routine for lattice filters. Returns: handle to the object */
latr32x32_handle_t latr32x32_init( void *         objmem, 
                                   int            M,
                             const int32_t * restrict k,
                                   int32_t            scale )
{ 
  latr32x32_ptr_t latr;
  int32_t *   restrict    delayLine;
  int32_t *   restrict    coef;
  void *          ptr;
  int m;

  NASSERT( objmem && M > 0 && k );

  //
  // Partition the memory block
  //

  ptr = objmem;

  latr      = (latr32x32_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr       = latr + 1;
  delayLine = (int32_t*)ALIGNED_ADDR( ptr, 8 );
  ptr       = delayLine + M;
  coef      = (int32_t*)ALIGNED_ADDR( ptr, 8 );
  ptr       = coef + M;

  NASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)latr32x32_alloc( M ) );

  //
  // Copy reflection coefficients, zero the delay line.
  //
  for ( m=0; m<M; m++ )
  {
    coef[m] =  k[m];
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

  latr->proc_fxn=(M>8) ? &latr32x32_X_proc : funs[M];
  return (latr); 

} // latr32x32_init()


void latr32x32_process( latr32x32_handle_t _latr, 
                        int32_t * restrict     r,
                  const int32_t *              x, int N )
{ 
  if(N<=0) return; 
  latr32x32_ptr_t latr = (latr32x32_ptr_t)_latr;
  NASSERT( latr && latr->magic == MAGIC && r && x );
  latr->proc_fxn(r,x,latr->delayLine,latr->coef,latr->scale,N,latr->M); 
  return;
} // latr32x32_process()
