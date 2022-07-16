/* ------------------------------------------------------------------------ */
/* Copyright (c) 2020 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2020 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

/* Library API */
#include "NatureDSP_Signal_vector.h"
#include "NatureDSP_types.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Batch Computation of Vector Dot products
  These routines take a set of input vectors and compute their dot product 
  with specific reference data.
  Two versions of routines are available: 
  - regular versions (vec_dot_batch8x8, vec_dot_batch8x16, vec_dot_batch16x16, 
    vec_dot_batchf). They work with arbitratry arguments
  - fast versions (vec_dot_batch8x8_fast, vec_dot_batch8x16_fast, 
    vec_dot_batch16x16_fast, vec_dot_batchf_fast) apply some restrictions.  

  Precision: 
  8x8    8x8-bit data, 16-bit output (fractional multiply Q7xQ7->Q15)
  8x16   8x16-bit data, 16-bit output (fractional multiply Q7xQ15->Q15)
  16x16  16x16-bit data, 16-bit output (fractional multiply Q15xQ15->Q31)
  f      single precision floating point

  Input:
  x[N]     input (reference) data, Q7, Q15 or floating point
  y[M][N]  pointers to M input vectors, Q7, Q15 or floating point
  N        length of vectors
  M        number of vectors
  rsh      right shift for output (for fixed point API only!)
  Output:
  z[M]     dot products between references and M inputs, Q15, Q31 or 
           floating point

  Restrictions:
  Regular versions:
    none
  Faster versions:
    x,y[m] - aligned on 8-byte boundary
    N      - multiple of 8
    M        multiple of 4
-------------------------------------------------------------------------*/

/* macro for emulation of unaligned load 8x8 */
#define MACRO_LA8X8_IP(v0, v1, alig, px)				\
{														\
	ae_int16x4  v_8;									\
	ae_int16x4 mask = AE_MOVDA16(0xFF00);				\
	AE_LA16X4_IP(v_8, alig, castxcc(ae_int16x4, px));	\
	v1 = AE_MOVINT16X4_FROMINT64(AE_SLAI64(AE_MOVINT64_FROMINT16X4(v_8), 8));\
	v1 = AE_INT16X4_AND_INT16X4(v1, mask);				\
	v_8 = AE_INT16X4_AND_INT16X4(v_8, mask);			\
	v0 = AE_SEL16_7362(v1, v_8);						\
	v1 = AE_SEL16_5140(v1, v_8);						\
}

#define MACRO_LAF16X2_TO_F32X2(s, p)					\
{														\
	s = AE_MOVDA32X2((uint16_t)(p)[0],(uint16_t)(p)[1]);\
    s = AE_SEXT32(s,15);								\
}

#define MACRO_LAF16_TO_F32X2(s, p)						\
{														\
	s = AE_MOVDA32X2((uint16_t)(p)[0],(uint16_t)(p)[0]);\
    s = AE_SEXT32(s,15);								\
}

#define MACRO_LAF8X2_TO_F32X2_Q16(s, p)					\
{														\
	s = AE_MOVDA32X2((uint8_t)(p)[0], (uint8_t)(p)[1]);	\
	s = AE_SLAI32(s, 8);								\
    s = AE_SEXT32(s,15);								\
}

#define MACRO_LAF8_TO_F32X2_Q16(s, p)					\
{														\
	s = AE_MOVDA32X2((uint8_t)(p)[0], (uint8_t)(p)[0]);	\
	s = AE_SLAI32(s, 8);								\
    s = AE_SEXT32(s,15);								\
}

void vec_dot_batch8x16 (int16_t   *z, const int8_t      * x,const cint16ptr_t   * y,int rsh, int N, int M)
{
	int m, n, off;

	ae_f64      q0, q1;
	ae_int16x4  vai;
	ae_int32x2  vai0, vai1;
	ae_int16x4  vxh0, vxh1;
	ae_int16x4  vyh0, vyh1;
	ae_valign   x_align, z_align, y0_align, y1_align;

	const signed char * restrict px;
	const ae_int16x4 * restrict py0;
	const ae_int16x4 * restrict py1;
		  ae_int16x4 * restrict pz;

	NASSERT(x);
	NASSERT(y);
	NASSERT(z);

	pz = (ae_int16x4 *)z;
	z_align = AE_ZALIGN64();

	if (M <= 0) return;

	if (N <= 0)
	{
		vai = 0;
		for (m = 0; m < M - 3; m += 4)
		{
			AE_SA16X4_IP(vai, z_align, pz);
		}
		AE_SA64POS_FP(z_align, pz);
		for (; m < M; m++)
		{
			z[m] = 0;
		}
		return;
	}

	rsh = 32 - rsh + 8;
	off = (size_t)x & 1;

	// Unrolled process of 4 vectors (align register saving)
	for (m = 0; m < M - 3; m += 4)
	{
		// Process a batch of 2 vectors
		NASSERT(y[m + 0]);
		NASSERT(y[m + 1]);

		q0 = AE_ZERO64();
		q1 = AE_ZERO64();

		// unaligned skip
		if (off)
		{
			ae_int32x2 _x, _y01;
			MACRO_LAF8_TO_F32X2_Q16(_x, x);
			_y01 = AE_MOVDA32X2((uint16_t)y[m + 0][0], (uint16_t)y[m + 1][0]);
			_y01 = AE_SEXT32(_y01, 15);
			AE_MULA32_HH(q0, _x, _y01);
			AE_MULA32_LL(q1, _x, _y01);
		}

		px = (const signed char *)(x + off);
		py0 = (const ae_int16x4 *)(y[m + 0] + off);
		py1 = (const ae_int16x4 *)(y[m + 1] + off);

		x_align = AE_LA64_PP(px);
		y0_align = AE_LA64_PP(py0);
		y1_align = AE_LA64_PP(py1);

		for (n = off; n < N - 7; n += 8)
		{
			MACRO_LA8X8_IP(vxh0, vxh1, x_align, px);
			AE_LA16X4_IP(vyh0, y0_align, py0);
			AE_LA16X4_IP(vyh1, y0_align, py0);

			// Q16.47 <- [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ] + [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ]
			AE_MULAAAAQ16(q0, vyh0, vxh0);
			AE_MULAAAAQ16(q0, vyh1, vxh1);

			AE_LA16X4_IP(vyh0, y1_align, py1);
			AE_LA16X4_IP(vyh1, y1_align, py1);

			// Q16.47 <- [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ] + [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ]
			AE_MULAAAAQ16(q1, vyh0, vxh0);
			AE_MULAAAAQ16(q1, vyh1, vxh1);
		}

		for (; n < N - 1; n += 2)
		{
			ae_int32x2 _x, _y0, _y1;
			MACRO_LAF8X2_TO_F32X2_Q16(_x, x + n);
			MACRO_LAF16X2_TO_F32X2(_y0, y[m + 0] + n);
			MACRO_LAF16X2_TO_F32X2(_y1, y[m + 1] + n);
			AE_MULAAD32_HH_LL(q0, _x, _y0);
			AE_MULAAD32_HH_LL(q1, _x, _y1);
		}

		if (n < N)
		{
			ae_int32x2 _x, _y01;
			MACRO_LAF8_TO_F32X2_Q16(_x, x + n);
			_y01 = AE_MOVDA32X2((uint16_t)y[m + 0][n], (uint16_t)y[m + 1][n]);
			_y01 = AE_SEXT32(_y01, 15);
			AE_MULA32_HH(q0, _x, _y01);
			AE_MULA32_LL(q1, _x, _y01);
		}

		// Store z[4*m+0]..z[4*m+3].
		// Q(15 - rsh) <- [ Q16.47 - w/ rounding and saturation ]
		vai0 = AE_TRUNCA32X2F64S(q0, q1, rsh);

		//
		// UNROLL
		//

		// Process a next batch of 2 vectors
		NASSERT(y[m + 2]);
		NASSERT(y[m + 3]);

		q0 = AE_ZERO64();
		q1 = AE_ZERO64();

		// unaligned skip
		if (off)
		{
			ae_int32x2 _x, _y01;
			MACRO_LAF8_TO_F32X2_Q16(_x, x);
			_y01 = AE_MOVDA32X2((uint16_t)y[m + 2][0], (uint16_t)y[m + 3][0]);
			_y01 = AE_SEXT32(_y01, 15);
			AE_MULA32_HH(q0, _x, _y01);
			AE_MULA32_LL(q1, _x, _y01);
		}

		px = (const signed char *)(x + off);
		py0 = (const ae_int16x4 *)(y[m + 2] + off);
		py1 = (const ae_int16x4 *)(y[m + 3] + off);

		x_align = AE_LA64_PP(px);
		y0_align = AE_LA64_PP(py0);
		y1_align = AE_LA64_PP(py1);

		for (n = off; n < N - 7; n += 8)
		{
			MACRO_LA8X8_IP(vxh0, vxh1, x_align, px);
			AE_LA16X4_IP(vyh0, y0_align, py0);
			AE_LA16X4_IP(vyh1, y0_align, py0);

			// Q16.47 <- [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ] + [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ]
			AE_MULAAAAQ16(q0, vyh0, vxh0);
			AE_MULAAAAQ16(q0, vyh1, vxh1);

			AE_LA16X4_IP(vyh0, y1_align, py1);
			AE_LA16X4_IP(vyh1, y1_align, py1);

			// Q16.47 <- [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ] + [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ]
			AE_MULAAAAQ16(q1, vyh0, vxh0);
			AE_MULAAAAQ16(q1, vyh1, vxh1);
		}

		for (; n < N - 1; n += 2)
		{
			ae_int32x2 _x, _y0, _y1;
			MACRO_LAF8X2_TO_F32X2_Q16(_x, x + n);
			MACRO_LAF16X2_TO_F32X2(_y0, y[m + 2] + n);
			MACRO_LAF16X2_TO_F32X2(_y1, y[m + 3] + n);
			AE_MULAAD32_HH_LL(q0, _x, _y0);
			AE_MULAAD32_HH_LL(q1, _x, _y1);
		}

		if (n < N)
		{
			ae_int32x2 _x, _y01;
			MACRO_LAF8_TO_F32X2_Q16(_x, x + n);
			_y01 = AE_MOVDA32X2((uint16_t)y[m + 2][n], (uint16_t)y[m + 3][n]);
			_y01 = AE_SEXT32(_y01, 15);
			AE_MULA32_HH(q0, _x, _y01);
			AE_MULA32_LL(q1, _x, _y01);
		}

		// Store z[4*m+0]..z[4*m+3].
		// Q(15 - rsh) <- [ Q16.47 - w/ rounding and saturation ]
		vai1 = AE_TRUNCA32X2F64S(q0, q1, rsh);

		vai = AE_TRUNC16X4F32(vai0, vai1);

		AE_SA16X4_IP(vai, z_align, pz);
	}
	AE_SA64POS_FP(z_align, pz);

	if (M & 2)
	{
		NASSERT(y[m + 0]);
		NASSERT(y[m + 1]);

		// Process a batch of 2 vectors;
		q0 = AE_ZERO64();
		q1 = AE_ZERO64();

		// unaligned skip
		if (off)
		{
			ae_int32x2 _x, _y01;
			MACRO_LAF8_TO_F32X2_Q16(_x, x);
			_y01 = AE_MOVDA32X2((uint16_t)y[m + 0][0], (uint16_t)y[m + 1][0]);
			_y01 = AE_SEXT32(_y01, 15);
			AE_MULA32_HH(q0, _x, _y01);
			AE_MULA32_LL(q1, _x, _y01);
		}

		px = (const signed char *)(x + off);
		py0 = (const ae_int16x4 *)(y[m + 0] + off);
		py1 = (const ae_int16x4 *)(y[m + 1] + off);

		x_align = AE_LA64_PP(px);
		y0_align = AE_LA64_PP(py0);
		y1_align = AE_LA64_PP(py1);

		for (n = off; n < N - 7; n += 8)
		{
			MACRO_LA8X8_IP(vxh0, vxh1, x_align, px);
			AE_LA16X4_IP(vyh0, y0_align, py0);
			AE_LA16X4_IP(vyh1, y0_align, py0);

			// Q16.47 <- [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ] + [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ]
			AE_MULAAAAQ16(q0, vyh0, vxh0);
			AE_MULAAAAQ16(q0, vyh1, vxh1);

			AE_LA16X4_IP(vyh0, y1_align, py1);
			AE_LA16X4_IP(vyh1, y1_align, py1);

			// Q16.47 <- [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ] + [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ]
			AE_MULAAAAQ16(q1, vyh0, vxh0);
			AE_MULAAAAQ16(q1, vyh1, vxh1);
		}

		for (; n < N - 1; n += 2)
		{
			ae_int32x2 _x, _y0, _y1;
			MACRO_LAF8X2_TO_F32X2_Q16(_x, x + n);
			MACRO_LAF16X2_TO_F32X2(_y0, y[m + 0] + n);
			MACRO_LAF16X2_TO_F32X2(_y1, y[m + 1] + n);
			AE_MULAAD32_HH_LL(q0, _x, _y0);
			AE_MULAAD32_HH_LL(q1, _x, _y1);
		}

		if (n < N)
		{
			ae_int32x2 _x, _y01;
			MACRO_LAF8_TO_F32X2_Q16(_x, x + n);
			_y01 = AE_MOVDA32X2((uint16_t)y[m + 0][n], (uint16_t)y[m + 1][n]);
			_y01 = AE_SEXT32(_y01, 15);
			AE_MULA32_HH(q0, _x, _y01);
			AE_MULA32_LL(q1, _x, _y01);
		}

		// Store z[4*m+0]..z[4*m+3].
		// Q(15 - rsh) <- [ Q16.47 - w/ rounding and saturation ]
		vai0 = AE_TRUNCA32X2F64S(q0, q1, rsh);
		
		z[m + 0] = AE_MOVAD16_3(AE_MOVF16X4_FROMF32X2(vai0));
		z[m + 1] = AE_MOVAD16_1(AE_MOVF16X4_FROMF32X2(vai0));

		m += 2;
	}

	if (M & 1)
	{
		NASSERT(y[m]);

		// Process a batch of 1 vector
		q0 = AE_ZERO64();

		// unaligned skip
		if (off)
		{
			ae_int32x2 _x, _y0;
			MACRO_LAF8_TO_F32X2_Q16(_x, x);
			_y0 = AE_MOVDA32X2((uint16_t)y[m + 0][0], 0);
			_y0 = AE_SEXT32(_y0, 15);
			AE_MULA32_HH(q0, _x, _y0);
		}

		px = (const signed char *)(x + off);
		py0 = (const ae_int16x4 *)(y[m + 0] + off);

		x_align = AE_LA64_PP(px);
		y0_align = AE_LA64_PP(py0);

		for (n = off; n < N - 7; n += 8)
		{
			MACRO_LA8X8_IP(vxh0, vxh1, x_align, px);
			AE_LA16X4_IP(vyh0, y0_align, py0);
			AE_LA16X4_IP(vyh1, y0_align, py0);

			// Q16.47 <- [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ] + [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ]
			AE_MULAAAAQ16(q0, vyh0, vxh0);
			AE_MULAAAAQ16(q0, vyh1, vxh1);
		}

		for (; n < N - 1; n += 2)
		{
			ae_int32x2 _x, _y0;
			MACRO_LAF8X2_TO_F32X2_Q16(_x, x + n);
			MACRO_LAF16X2_TO_F32X2(_y0, y[m + 0] + n);
			AE_MULAAD32_HH_LL(q0, _x, _y0);
		}

		if (n < N)
		{
			ae_int32x2 _x, _y0;
			MACRO_LAF8_TO_F32X2_Q16(_x, x + n);
			_y0 = AE_MOVDA32X2((uint16_t)y[m + 0][n], 0);
			_y0 = AE_SEXT32(_y0, 15);
			AE_MULA32_HH(q0, _x, _y0);
		}

		// Store z[4*m+0]..z[4*m+3].
		// Q(15 - rsh) <- [ Q16.47 - w/ rounding and saturation ]
		q0 = AE_SLAA64S(q0, rsh);

		z[m + 0] = AE_MOVAD16_3(AE_MOVF16X4_FROMF64(q0));
	}
}
