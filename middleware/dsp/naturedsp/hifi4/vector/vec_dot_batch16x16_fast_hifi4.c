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
void vec_dot_batch16x16_fast(int32_t   *z, const int16_t     * x,const cint16ptr_t   * y,int rsh, int N, int M)
{
	int m,n;

	ae_f64      q0, q1, q2, q3;
	ae_int32x2  vai0, vai1;
	ae_int16x4  vxh;
	ae_int16x4  vyh0, vyh1, vyh2, vyh3;
	ae_valign   z_align;

	const ae_int16x4 * restrict px;
	const ae_int16x4 * restrict py0;
	const ae_int16x4 * restrict py1;
	const ae_int16x4 * restrict py2;
	const ae_int16x4 * restrict py3;
	      ae_int32x2 * restrict pz;

    NASSERT(z);  NASSERT_ALIGN(z,HIFI_SIMD_WIDTH);
    NASSERT(x);  NASSERT_ALIGN(x,HIFI_SIMD_WIDTH);
    NASSERT(y);  
    NASSERT(M%4==0 && N%8==0);

	pz = (ae_int32x2 *)z;
	z_align = AE_ZALIGN64();

	if (M <= 0) return;

	if (N <= 0)
	{
		vai0 = 0;
		vai1 = 0;
		for (m = 0; m < M >> 2; m++)
		{
			AE_SA32X2_IP(vai0, z_align, pz);
			AE_SA32X2_IP(vai1, z_align, pz);
		}
		AE_SA64POS_FP(z_align, pz);
		return;
	}

	rsh = 32-rsh;

	__Pragma("loop_count min=1");
	for (m = 0; m < M; m += 4)
	{
		NASSERT(y[m + 0]);
		NASSERT(y[m + 1]);
		NASSERT(y[m + 2]);
		NASSERT(y[m + 3]);

		NASSERT_ALIGN8(y[m + 0]);
		NASSERT_ALIGN8(y[m + 1]);
		NASSERT_ALIGN8(y[m + 2]);
		NASSERT_ALIGN8(y[m + 3]);

		// Process a batch of 4 vectors;
		px  = (const ae_int16x4 *)x;
		py0 = (const ae_int16x4 *)y[m + 0];
		py1 = (const ae_int16x4 *)y[m + 1];
		py2 = (const ae_int16x4 *)y[m + 2];
		py3 = (const ae_int16x4 *)y[m + 3];

		// load first 4 values
		AE_L16X4_IP(vxh, px, sizeof(*px));

		AE_L16X4_IP(vyh0, py0, sizeof(*py0));
		AE_L16X4_IP(vyh1, py1, sizeof(*py1));
		AE_L16X4_IP(vyh2, py2, sizeof(*py2));
		AE_L16X4_IP(vyh3, py3, sizeof(*py3));

		// Q16.47 <- [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ] + [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ]
		q0 = AE_MULZAAAAQ16(vyh0, vxh);
		q1 = AE_MULZAAAAQ16(vyh1, vxh);
		q2 = AE_MULZAAAAQ16(vyh2, vxh);
		q3 = AE_MULZAAAAQ16(vyh3, vxh);

		for (n = 0; n < (N >> 3)-1; n++)
		{
			AE_L16X4_IP(vxh, px, sizeof(*px));

			AE_L16X4_IP(vyh0, py0, sizeof(*py0));
			AE_L16X4_IP(vyh1, py1, sizeof(*py1));
			AE_L16X4_IP(vyh2, py2, sizeof(*py2));
			AE_L16X4_IP(vyh3, py3, sizeof(*py3));

			// Q16.47 <- [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ] + [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ]
			AE_MULAAAAQ16(q0, vyh0, vxh);
			AE_MULAAAAQ16(q1, vyh1, vxh);
			AE_MULAAAAQ16(q2, vyh2, vxh);
			AE_MULAAAAQ16(q3, vyh3, vxh);

			AE_L16X4_IP(vxh, px, sizeof(*px));

			AE_L16X4_IP(vyh0, py0, sizeof(*py0));
			AE_L16X4_IP(vyh1, py1, sizeof(*py1));
			AE_L16X4_IP(vyh2, py2, sizeof(*py2));
			AE_L16X4_IP(vyh3, py3, sizeof(*py3));

			// Q16.47 <- [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ] + [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ]
			AE_MULAAAAQ16(q0, vyh0, vxh);
			AE_MULAAAAQ16(q1, vyh1, vxh);
			AE_MULAAAAQ16(q2, vyh2, vxh);
			AE_MULAAAAQ16(q3, vyh3, vxh);
		}

		// load last 4 values
		AE_L16X4_IP(vxh, px, sizeof(*px));

		AE_L16X4_IP(vyh0, py0, sizeof(*py0));
		AE_L16X4_IP(vyh1, py1, sizeof(*py1));
		AE_L16X4_IP(vyh2, py2, sizeof(*py2));
		AE_L16X4_IP(vyh3, py3, sizeof(*py3));

		// Q16.47 <- [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ] + [ Q16*Q16 + 1] + [ Q16*Q16 + 1 ]
		AE_MULAAAAQ16(q0, vyh0, vxh);
		AE_MULAAAAQ16(q1, vyh1, vxh);
		AE_MULAAAAQ16(q2, vyh2, vxh);
		AE_MULAAAAQ16(q3, vyh3, vxh);

		// Store z[4*m+0]..z[4*m+3].
		// Q(15 - rsh) <- [ Q16.47 - w/ rounding and saturation ]
		vai0 = AE_TRUNCA32X2F64S(q0, q1, rsh);
		vai1 = AE_TRUNCA32X2F64S(q2, q3, rsh);

		AE_SA32X2_IP(vai0, z_align, pz);
		AE_SA32X2_IP(vai1, z_align, pz);
	}
	AE_SA64POS_FP(z_align, pz);
}
