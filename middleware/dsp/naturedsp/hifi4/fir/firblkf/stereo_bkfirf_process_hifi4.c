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
DISCARD_FUN(void,stereo_bkfirf_process,( stereo_bkfirf_handle_t handle, 
                         float32_t * restrict  y,
                   const float32_t * restrict  x, int N ))
#elif HAVE_VFPU

/* Circular load with using CBEGIN1/CEND1 */
#define XT_LSX2XC1(reg, addr, offs)\
{\
    ae_int32x2 t;\
    AE_L32X2_XC1(t, addr, offs);\
    reg = XT_AE_MOVXTFLOATX2_FROMINT32X2(t);\
}

#define XT_SSX2XC1(reg, addr, offs)\
{\
    ae_int32x2 t;\
    t = XT_AE_MOVINT32X2_FROMXTFLOATX2(reg);\
    AE_S32X2_XC1(t, castxcc(ae_int32x2,addr), offs);\
}

#define MAX_BUFFER_SZ ((int)(MAX_ALLOCA_SZ/(sizeof(float32_t)*4)))

/* process block of samples */
void stereo_bkfirf_process( stereo_bkfirf_handle_t handle, 
                         float32_t * restrict  y,
                   const float32_t * restrict  x, int N )
{
    stereo_bkfirf_ptr_t stereo_bkfir;
	stereo_bkfir = (stereo_bkfirf_ptr_t)handle;

	const xtfloatx2 * restrict pX;
	      xtfloatx2 * restrict pY;
	      xtfloatx2 * restrict pBxl;
	      xtfloatx2 * restrict pBxr;
	      xtfloatx2 * restrict pByl;
	      xtfloatx2 * restrict pByr;
	xtfloatx2 x0, x1;
	xtfloatx2 xl, xr;
	xtfloatx2 y0, y1;
	xtfloatx2 yl, yr;
	ae_valign yv;
	int n;
	int b;
	// Allocate the four buffers
	ALIGN(8) float32_t bxl[MAX_BUFFER_SZ];
	ALIGN(8) float32_t bxr[MAX_BUFFER_SZ];
	ALIGN(8) float32_t byl[MAX_BUFFER_SZ];
	ALIGN(8) float32_t byr[MAX_BUFFER_SZ];

    NASSERT(stereo_bkfir && y && x);
	NASSERT(stereo_bkfir->magic == STEREO_BKFIRF_MAGIC);
    NASSERT_ALIGN(x, 8);
    if (N <= 0) return;
    NASSERT(N % 4 == 0);
	yv = AE_ZALIGN64();

	pX   = (const xtfloatx2 *)(x);
	pY   = (      xtfloatx2 *)(y);

	// Aggregation: @ MAX_BUFFER_SZ stereo-samples
	for (b = 0; b < N; b+=MAX_BUFFER_SZ)
	{
		int NN = XT_MIN(N-b, MAX_BUFFER_SZ);
		pBxl = (      xtfloatx2 *)(bxl);
		pBxr = (      xtfloatx2 *)(bxr);
		pByl = (      xtfloatx2 *)(byl);
		pByr = (      xtfloatx2 *)(byr);

		// Split x into xl (left channel) and xr (right channel).
		for (n = 0; n < (NN >> 1); n++)
		{
			XT_LSX2IP(x0, pX, sizeof(xtfloatx2));
			XT_LSX2IP(x1, pX, sizeof(xtfloatx2));
			xl = XT_SEL32_HH_SX2(x0, x1);
			xr = XT_SEL32_LL_SX2(x0, x1);
			XT_SSX2IP(xl, pBxl, sizeof(xtfloatx2));
			XT_SSX2IP(xr, pBxr, sizeof(xtfloatx2));
		}
		pBxl = (      xtfloatx2 *)bxl;
		pBxr = (      xtfloatx2 *)bxr;

		// Aggregate left channel.
		bkfirf_process(stereo_bkfir->bkfir_left, (float32_t *)pByl, (const float32_t *)pBxl, NN);

		// Aggregate right channel.
		bkfirf_process(stereo_bkfir->bkfir_right, (float32_t *)pByr, (const float32_t *)pBxr, NN);

		// Merge yl (left channel) and yr (right channel) into y.
		for (n = 0; n < (NN >> 1); n++)
		{
			XT_LSX2IP(yl, pByl, sizeof(xtfloatx2));
			XT_LSX2IP(yr, pByr, sizeof(xtfloatx2));
			y0 = XT_SEL32_HH_SX2(yl, yr);
			y1 = XT_SEL32_LL_SX2(yl, yr);
			XT_SASX2IP(y0, yv, pY);
			XT_SASX2IP(y1, yv, pY);
		}
	}
	XT_SASX2POSFP(yv, pY);
} /* stereo_bkfirf_process() */
#else

#define XT_LSXC1(reg, addr, offs)\
{\
    ae_int32x2 t;\
    AE_L32_XC1(t, castxcc(ae_int32,addr), offs);\
    reg = XT_WFR(t);\
}

#define XT_SSXC1(reg, addr, offs)\
{\
    ae_int32 t;\
	t = XT_RFR(reg);\
	AE_S32_L_XC1(t, castxcc(ae_int32,addr), offs);\
}

#define MAX_BUFFER_SZ ((int)(MAX_ALLOCA_SZ/(sizeof(float32_t)*4)))

/* process block of samples */
void stereo_bkfirf_process( stereo_bkfirf_handle_t _bkfir, 
                         float32_t * restrict  y,
                   const float32_t * restrict  x, int N )
{
    stereo_bkfirf_ptr_t stereo_bkfir;
	stereo_bkfir = (stereo_bkfirf_ptr_t)_bkfir;

	const xtfloat * restrict pX;
	      xtfloat * restrict pY;
	      xtfloat * restrict pBxl;
	      xtfloat * restrict pBxr;
	      xtfloat * restrict pByl;
	      xtfloat * restrict pByr;
	xtfloat xl, xr;
	xtfloat yl, yr;
	int n;
	int b;
	// Allocate the four buffers
	ALIGN(8) float32_t bxl[MAX_BUFFER_SZ];
	ALIGN(8) float32_t bxr[MAX_BUFFER_SZ];
	ALIGN(8) float32_t byl[MAX_BUFFER_SZ];
	ALIGN(8) float32_t byr[MAX_BUFFER_SZ];

    NASSERT(stereo_bkfir && y && x);
	NASSERT(stereo_bkfir->magic == STEREO_BKFIRF_MAGIC);
    NASSERT_ALIGN(x, 8);
    if (N <= 0) return;
    NASSERT(N % 4 == 0);

	pX   = (const xtfloat *)(x);
	pY   = (      xtfloat *)(y);

	// Aggregation: @ MAX_BUFFER_SZ stereo-samples
	for (b = 0; b < N; b+=MAX_BUFFER_SZ)
	{
		int NN = XT_MIN(N-b, MAX_BUFFER_SZ);
		pBxl = (      xtfloat *)(bxl);
		pBxr = (      xtfloat *)(bxr);
		pByl = (      xtfloat *)(byl);
		pByr = (      xtfloat *)(byr);

		// Split x into xl (left channel) and xr (right channel).
		for (n = 0; n < (NN); n++)
		{
			XT_LSIP(xl, pX, sizeof(xtfloat));
			XT_LSIP(xr, pX, sizeof(xtfloat));
			XT_SSIP(xl, pBxl, sizeof(xtfloat));
			XT_SSIP(xr, pBxr, sizeof(xtfloat));
		}
		pBxl = (      xtfloat *)bxl;
		pBxr = (      xtfloat *)bxr;

		// Aggregate left channel.
		bkfirf_process(stereo_bkfir->bkfir_left, (float32_t *)pByl, (const float32_t *)pBxl, NN);

		// Aggregate right channel.
		bkfirf_process(stereo_bkfir->bkfir_right, (float32_t *)pByr, (const float32_t *)pBxr, NN);

		// Merge yl (left channel) and yr (right channel) into y.
		for (n = 0; n < (NN); n++)
		{
			XT_LSIP(yl, pByl, sizeof(xtfloat));
			XT_LSIP(yr, pByr, sizeof(xtfloat));
			XT_SSIP(yl, pY, sizeof(xtfloat));
			XT_SSIP(yr, pY, sizeof(xtfloat));
		}
	}
} /* stereo_bkfirf_process() */
#endif
