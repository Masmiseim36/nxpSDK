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
  NatureDSP Signal Processing Library. FIR part
    Real block FIR filter, 32x16-bit
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/



/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "bkfir32x16_common.h"

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

#define sz_i16   sizeof(int16_t)
#define sz_i32   sizeof(int32_t)

/* Allocation routine for filters. Returns: size of memory in bytes to be allocated */
size_t bkfir32x16_alloc( int M,int extIR )
{
  int delayLen,coefLen;
  NASSERT(  M > 0 );
  NASSERT (M%4==0);

  delayLen  = (M>32? (M + 4):( M + 8));
  coefLen   = (M>32? (M + 8):( M + 4 ));
  if(extIR) coefLen = 0;

  return 15 + 
         sizeof(int32_t)*delayLen +
         sizeof(int16_t)*coefLen  +
         sizeof( bkfir32x16_t );
} 
// bkfir32x16_alloc()

/* Initialization for filters. Returns: handle to the object */
bkfir32x16_handle_t bkfir32x16_init( void *              objmem, 
                                     int                 M, 
                                     int extIR,
                               const int16_t * restrict  h )
{
  bkfir32x16_ptr_t bkfir;
  int32_t *        delLine; 
  int16_t *        coef;
  int delayLen,coefLen;
  int m;
  int id=0;

  delayLen  = (M>32? (M + 4):( M + 8));
  coefLen   = (M>32? (M + 8):( M + 4 ));
  if(extIR) coefLen = 0;

  NASSERT( objmem && M > 0 && h );
  NASSERT (M%4==0);
  NASSERT_ALIGN (h, 8);

  // Partition the memory block
  delLine = (int32_t*)((((uintptr_t)objmem)+15)&~15); 
  coef    = (int16_t *)(delLine+delayLen); 
  bkfir   = (bkfir32x16_ptr_t)(coef+coefLen);
  if (extIR) coef=(int16_t*)h;
  NASSERT_ALIGN(delLine,16);
  NASSERT_ALIGN(coef   ,8);
  NASSERT_ALIGN(bkfir  ,4);
  //
  // Copy the filter coefficients and zero the delay line. Original impulse
  // response is padded with zeros: three zeros go before the first tap
  // (corresponds to the newest sample), one zero follows the last tap,
  // which matches the oldest sample. After that the order of filter
  // coefficients is reverted.
  //
  if (M>32)
  {
    id = 4;
  }
  if(extIR==0)
  {
    for ( m=0; m<1+id; m++ )
    {
      coef[m] = 0;
    }
    for ( m=1; m<M+1; m++ )
    {
      coef[m+id] = h[M-m];
    }

    for ( ; m<M+4; m++ )
    {
      coef[m+id] = 0;
    }
  }

  for ( m=0; m<delayLen; m++ )
  {
    delLine[m] = 0;
  }

  // Initialize the filter instance.
  bkfir->magic     = BKFIR32X16_MAGIC;
  bkfir->M         = M;
  bkfir->coef      = coef;
  bkfir->delayLine = delLine;
  bkfir->delayLen  = delayLen;
  bkfir->wrIx      = 0;

  if(M==4) 
  {
    bkfir->fnprocess=bkfir32x16_process4; 
  }
  else
  {
    bkfir->fnprocess=(M>32) ? bkfir32x16_processBig : bkfir32x16_processSmall; 
  }
  return (bkfir);
} // bkfir32x16_init()
