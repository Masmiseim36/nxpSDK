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
    Real block FIR filter, 16x16-bit
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
/* Filter instance structure. */
#include "stereo_bkfir16x16_common.h"

#define MAX_BUFFER_SZ ((int)(MAX_ALLOCA_SZ/(sizeof(int16_t)*4)))

void stereo_bkfir16x16_process( stereo_bkfir16x16_handle_t handle,
                         int16_t * restrict  y,
                   const int16_t * restrict  x, int N)
{
    stereo_bkfir16x16_ptr_t stereo_bkfir = (stereo_bkfir16x16_ptr_t)handle;
	const ae_int16x4 * restrict pX;
	      ae_int16x4 * restrict pY;
	      ae_int16x4 * restrict pBxl;
	      ae_int16x4 * restrict pBxr;
	      ae_int16x4 * restrict pByl;
	      ae_int16x4 * restrict pByr;
	ae_int16x4 x0, x1;
	ae_int16x4 xl, xr;
	ae_int16x4 y0, y1;
	ae_int16x4 yl, yr;
	ae_valign yv;
	int n;
	int b;
	// Allocate the four buffers
	ALIGN(8) int16_t bxl[MAX_BUFFER_SZ];
	ALIGN(8) int16_t bxr[MAX_BUFFER_SZ];
	ALIGN(8) int16_t byl[MAX_BUFFER_SZ];
	ALIGN(8) int16_t byr[MAX_BUFFER_SZ];

    NASSERT(stereo_bkfir && y && x);
	NASSERT(stereo_bkfir->magic == STEREO_BKFIR16X16_MAGIC);
    NASSERT_ALIGN(x, 8);
    if (N <= 0) return;
    NASSERT(N % 4 == 0);
	yv = AE_ZALIGN64();

	pX   = (const ae_int16x4 *)(x);
	pY   = (      ae_int16x4 *)(y);

	// Aggregation: @ MAX_BUFFER_SZ stereo-samples
	for (b = 0; b < N; b+=MAX_BUFFER_SZ)
	{
		int NN = XT_MIN(N-b, MAX_BUFFER_SZ);
		pBxl = (      ae_int16x4 *)(bxl);
		pBxr = (      ae_int16x4 *)(bxr);
		pByl = (      ae_int16x4 *)(byl);
		pByr = (      ae_int16x4 *)(byr);

		// Split x into xl (left channel) and xr (right channel).
		for (n = 0; n < (NN >> 2); n++)
		{
			AE_L16X4_IP(x0, pX, sizeof(ae_int16x4));
			AE_L16X4_IP(x1, pX, sizeof(ae_int16x4));
			xl = AE_SEL16_7531(x0, x1);
			xr = AE_SEL16_6420(x0, x1);
			AE_S16X4_IP(xl, pBxl, sizeof(ae_int16x4));
			AE_S16X4_IP(xr, pBxr, sizeof(ae_int16x4));
		}
		pBxl = (      ae_int16x4 *)bxl;
		pBxr = (      ae_int16x4 *)bxr;

		// Aggregate left channel.
		bkfir16x16_process(stereo_bkfir->bkfir_left, (int16_t *)pByl, (const int16_t *)pBxl, NN);

		// Aggregate right channel.
		bkfir16x16_process(stereo_bkfir->bkfir_right, (int16_t *)pByr, (const int16_t *)pBxr, NN);

		// Merge yl (left channel) and yr (right channel) into y.
		for (n = 0; n < (NN >> 2); n++)
		{
			AE_L16X4_IP(yl, pByl, sizeof(ae_int16x4));
			AE_L16X4_IP(yr, pByr, sizeof(ae_int16x4));
			y0 = AE_SEL16_7362(yl, yr);
			y1 = AE_SEL16_5140(yl, yr);
			AE_SA16X4_IP(y0, yv, pY);
			AE_SA16X4_IP(y1, yv, pY);
		}
	}
	AE_SA64POS_FP(yv, pY);
} /* stereo_bkfir16x16_process() */
