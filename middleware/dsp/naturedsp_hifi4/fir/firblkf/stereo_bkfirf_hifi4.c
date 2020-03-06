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
    Real block FIR filter, floating point
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

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

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "common_fpu.h"
/* Filter instance structure. */
#include "stereo_bkfirf_common.h"

#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(size_t,stereo_bkfirf_alloc,( int M , int extIR))
DISCARD_FUN(stereo_bkfirf_handle_t,stereo_bkfirf_init,( void * objmem, int M, int extIR, const float32_t * hl, const float32_t * hr ))
#else

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t stereo_bkfirf_alloc( int M , int extIR)
{
	NASSERT( M > 0 );
	NASSERT(M%4==0);
	return (sizeof(stereo_bkfirf_t) + 2*bkfirf_alloc(M, extIR) + 7);
} /* stereo_bkfirf_alloc() */

/* Initialize the filter structure. The delay line is zeroed. */
stereo_bkfirf_handle_t stereo_bkfirf_init( void * objmem, int M, int extIR, const float32_t * hl, const float32_t * hr )
{
    stereo_bkfirf_ptr_t   stereo_bkfir;
    void                * ptr;
	size_t szbkfir;
	szbkfir = bkfirf_alloc(M,extIR);

    NASSERT(objmem && (M>0) && hl && hr);
    NASSERT(!(M&3) && IS_ALIGN(hl) && IS_ALIGN(hr));

    // Partition the memory block
	ptr = objmem;
    stereo_bkfir = (stereo_bkfirf_ptr_t)((((uintptr_t)ptr)+7)&~7);
	stereo_bkfir->magic = STEREO_BKFIRF_MAGIC;
	ptr = stereo_bkfir + 1;
    stereo_bkfir->bkfir_left_mem = ptr;
	ptr = (void*)(((uintptr_t)ptr)+szbkfir);
    stereo_bkfir->bkfir_right_mem = ptr;
	stereo_bkfir->bkfir_left  = bkfirf_init(stereo_bkfir->bkfir_left_mem ,M,extIR,hl);
	stereo_bkfir->bkfir_right = bkfirf_init(stereo_bkfir->bkfir_right_mem,M,extIR,hr);
	return stereo_bkfir;
} /* stereo_bkfirf_init() */
#endif
