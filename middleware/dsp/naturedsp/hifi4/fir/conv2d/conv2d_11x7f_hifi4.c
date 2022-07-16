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
  2D Convolution  
  IntegrIT, 2006-2020
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
#include "common_fpu.h"


/*-------------------------------------------------------------------------
  2D convolution
  Functions compute the two-dimensional convolution of input matrix x[M][N]
  and y[P][Q] and store the result in matrix z[M+P-1][N+Q-1]
  Additional parameter rsh allows to control fixed point representation of 
  output data.

  Precision: 
  8x8      8-bit coefficients, 8-bit data, 8-bit output, Q7
  8x16     8-bit coefficients Q7, 16-bit data, 16-bit output, Q15
  16x16    16-bit coefficients, 16-bit data, 16-bit output, Q15
  f        floating point data

  Input:
  x[M][N]   input data Q15, Q7, floating point
  y[P][Q]   input data Q15, Q7, floating point
  M         number of rows in the matrix x
  N         number of columns in the matrix x
  P         number of rows in the matrix y
  Q         number of columns in the matrix y
  rsh       additional right shift (for fixed point API only)

  Output:
  z	[M+P-1][N+Q-1] output data, Q(7-rsh), Q(15-rsh), floating point

  Temporary:
  pScr     scratch data. Should have size at least as requested by 
           corresponding scratch allocation function

  Restrictions:
  x,y,z        should not overlap
  x,y,z,pScr   aligned on a 8-bytes boundary
  P, Q	       >0 and multiplies of 8
-------------------------------------------------------------------------*/

#if (HAVE_FPU==0 && HAVE_VFPU==0)

size_t conv2d_11x7f_getScratchSize (int P, int Q) 
{ 
    (void)P,(void)Q;
    return 0;
}
DISCARD_FUN(void,conv2d_11x7f,(void* pScr, float32_t *z, const float32_t * x, const float32_t * y, int P, int Q))

#elif HAVE_VFPU

/* macro defines load I xtfloat with replicate to xtfloatx2 */
#define MACRO_LSI_REPL(v, p, off)   \
{                                               \
    ae_int32x2 v_i;                             \
    v_i = AE_L32_I((const ae_int32*)(p), off);  \
    v = XT_AE_MOVXTFLOATX2_FROMINT32X2(v_i);    \
}

/* macro defines load XP xtfloat with replicate to xtfloatx2 */
#define MACRO_LSIP_REPL(v, p, off) \
{                                                       \
    ae_int32x2 v_i;                                     \
    AE_L32_IP(v_i, castxcc(const ae_int32, p), off);    \
    v = XT_AE_MOVXTFLOATX2_FROMINT32X2(v_i);            \
}

/* macro defines load XP xtfloat with replicate to xtfloatx2 */
#define MACRO_LSXP_REPL(v, p, off)\
{                                                       \
    ae_int32x2 v_i;                                     \
    AE_L32_XP(v_i, castxcc(const ae_int32, p), (off));  \
    v = XT_AE_MOVXTFLOATX2_FROMINT32X2(v_i);            \
}

/* macro defines load XC xtfloat with replicate to xtfloatx2 */
#define MACRO_LSXC_REPL(v, p, off)\
{                                                       \
    ae_int32x2 v_i;                                     \
    AE_L32_XC(v_i, castxcc(const ae_int32, p), off);    \
    v = XT_AE_MOVXTFLOATX2_FROMINT32X2(v_i);            \
}

/* macro defines load XC1 xtfloatx2 */
#define MACRO_LSX2XC1(v, p, off)\
{                                                           \
    ae_int32x2 v_i;                                         \
    AE_L32X2_XC1(v_i, castxcc(const ae_int32x2, p), off);   \
    v = XT_AE_MOVXTFLOATX2_FROMINT32X2(v_i);                \
}

#define MACRO_ADD_HL_LH_XC2(v) XT_MADDMUX_S(v, (xtfloatx2)(1.0f), v, 5)


void conv2d_11x7f     (void* pScr, float32_t *z, const float32_t * x, const float32_t * y, int P, int Q)
{
#   define M 11
#   define N 7
	int i, j, m, y_start, y_end, m_end, m_start;

	const xtfloatx2 * restrict pX;
	const xtfloatx2 * restrict pY;
	const xtfloatx2 * restrict pW;
	xtfloatx2 * restrict pWwr;
	xtfloatx2 * restrict pZ;
	ae_valign aX;
	ae_valign alZ;

	xtfloatx2 Y01, Y23, Y45, Y67, Y89;
	xtfloatx2 W0z, W10, W21, W32, W43, W54, W65, Wz6;

	NASSERT(x);
	NASSERT(y);
	NASSERT(z);
	NASSERT(pScr);
    NASSERT_ALIGN(x,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(y, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(z, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(pScr, HIFI_SIMD_WIDTH);
    NASSERT(P>=0 && P%8==0);
	NASSERT(Q >= 0 && Q % 8 == 0);

	if (P <= 0 || Q <= 0) return;

	/* Store coefficients in the next order:
	*     0  x[6]
	*   x[5] x[4]
	*   x[3] x[2]
	*   x[1] x[0]
	* Start from the last row
	* Double up data to reduce stalls
	*/
#define WLEN 8
	pWwr = (xtfloatx2 *)pScr;
	pX = (xtfloatx2 *)(x + (M - 1) * N + 6);
	for (i = M - 1; i >= 0; i--)
	{
		xtfloatx2 vx;

		// N/2 - times unroll
		MACRO_LSXP_REPL(vx, pX, -(int)sizeof(float32_t)); // 6
		vx = XT_SEL32_LH_SX2((xtfloatx2)(0.0f), vx); // z 6
		XT_SSX2IP(vx, pWwr, sizeof(xtfloatx2));
		XT_SSX2IP(vx, pWwr, sizeof(xtfloatx2));

		aX = AE_LA64_PP(pX);

		XT_LASX2RIP(vx, aX, pX);	// 5 4
		XT_SSX2IP(vx, pWwr, sizeof(xtfloatx2));
		XT_SSX2IP(vx, pWwr, sizeof(xtfloatx2));

		XT_LASX2RIP(vx, aX, pX); // 3 2
		XT_SSX2IP(vx, pWwr, sizeof(xtfloatx2));
		XT_SSX2IP(vx, pWwr, sizeof(xtfloatx2));

		XT_LASX2RIP(vx, aX, pX); // 1 0
		XT_SSX2IP(vx, pWwr, sizeof(xtfloatx2));
		XT_SSX2IP(vx, pWwr, sizeof(xtfloatx2));
	}

	/*
	* Processing of convolution
	*/
	pZ = (xtfloatx2 *)(z);
	alZ = AE_ZALIGN64();
	for (i = 0; i < M + P - 1; i++)
	{
		y_start = XT_MAX(i + 1 - M, 0);
		y_end = XT_MIN(i + 1, P);
		m_end = XT_MIN(i + 1, M);
		m_start = m_end - (y_end - y_start);
		
		WAE_CBEGIN0((uintptr_t)(y + y_start*Q + 4));
		WAE_CEND0((uintptr_t)(y + y_end*Q));

		WAE_CBEGIN1((uintptr_t)((float32_t *)pScr + (M - m_end) * WLEN * 2));
		WAE_CEND1((uintptr_t)((float32_t *)pScr + (M - m_start) * WLEN * 2));

		pW = (const xtfloatx2 *)pScr + (M - m_end) * WLEN;

		/* First N + 1 samples of the i-th row */
		{
			xtfloatx2 S0, S2, S4, S6;
			xtfloatx2 S1, S3, S5, S7;

			pY = (const xtfloatx2 *)(y + y_start * Q);

			S0 = S2 = S4 = S5 = (xtfloatx2)(0.0f);
			S1 = S3 = S6 = S7 = (xtfloatx2)(0.0f);

			__Pragma("loop_count min=1, max=11");
			for (m = m_start; m < m_end; m++)
			{
				XT_LSX2IP(Y01, pY, sizeof(xtfloatx2));
				XT_LSX2IP(Y23, pY, sizeof(xtfloatx2));
				XT_LSX2IP(Y45, pY, sizeof(xtfloatx2));
				XT_LSX2XP(Y67, pY, (Q - 6) * sizeof(float32_t));

				XT_LSX2IP(Wz6, pW, 2*sizeof(xtfloatx2));
				XT_LSX2IP(W54, pW, 2*sizeof(xtfloatx2));
				XT_LSX2IP(W32, pW, 2*sizeof(xtfloatx2));
				MACRO_LSX2XC1(W10, pW, 2*sizeof(xtfloatx2));

				W65 = XT_SEL32_LH_SX2(Wz6, W54);
				W43 = XT_SEL32_LH_SX2(W54, W32);
				W21 = XT_SEL32_LH_SX2(W32, W10);
				W0z = XT_SEL32_LH_SX2(W10, (xtfloatx2)(0.0f));

				XT_MADD_SX2(S6, W65, Y01);	XT_MADD_SX2(S7, Wz6, Y01);
				XT_MADD_SX2(S4, W43, Y01);	XT_MADD_SX2(S5, W54, Y01);
				XT_MADD_SX2(S2, W21, Y01);	XT_MADD_SX2(S3, W32, Y01);
				XT_MADD_SX2(S0, W0z, Y01);	XT_MADD_SX2(S1, W10, Y01);

				XT_MADD_SX2(S6, W43, Y23);	XT_MADD_SX2(S7, W54, Y23);
				XT_MADD_SX2(S4, W21, Y23);	XT_MADD_SX2(S5, W32, Y23);
				XT_MADD_SX2(S2, W0z, Y23);	XT_MADD_SX2(S3, W10, Y23);

				XT_MADD_SX2(S6, W21, Y45);	XT_MADD_SX2(S7, W32, Y45);
				XT_MADD_SX2(S4, W0z, Y45);	XT_MADD_SX2(S5, W10, Y45);

				XT_MADD_SX2(S6, W0z, Y67);	XT_MADD_SX2(S7, W10, Y67);
			}

			MACRO_ADD_HL_LH_XC2(S0); MACRO_ADD_HL_LH_XC2(S4);
			MACRO_ADD_HL_LH_XC2(S1); MACRO_ADD_HL_LH_XC2(S5);
			MACRO_ADD_HL_LH_XC2(S2); MACRO_ADD_HL_LH_XC2(S6);
			MACRO_ADD_HL_LH_XC2(S3); MACRO_ADD_HL_LH_XC2(S7);

			S0 = XT_SEL32_HL_SX2(S0, S1);
			S2 = XT_SEL32_HL_SX2(S2, S3);
			S4 = XT_SEL32_HL_SX2(S4, S5);
			S6 = XT_SEL32_HL_SX2(S6, S7);

			XT_SASX2IP(S0, alZ, pZ);
			XT_SASX2IP(S2, alZ, pZ);
			XT_SASX2IP(S4, alZ, pZ);
			XT_SASX2IP(S6, alZ, pZ);
		}

		/* Next samples */
		pY = (const xtfloatx2 *)(y + y_start * Q + 2);

		XT_LSX2IP(Y01, pY, sizeof(xtfloatx2));
		XT_LSX2IP(Y23, pY, sizeof(xtfloatx2));
		XT_LSX2IP(Wz6, pW, 2*sizeof(xtfloatx2));

		for (j = N + 1; j < Q; j += 4)
		{
			xtfloatx2 S0_0, S1_0, S2_0, S3_0;
			xtfloatx2 S0_1, S1_1, S2_1, S3_1;
			S0_0 = S1_0 = S2_0 = S3_0 = (xtfloatx2)(0.0f);
			S0_1 = S1_1 = S2_1 = S3_1 = (xtfloatx2)(0.0f);

			__Pragma("loop_count min=1, max=11");
			for (m = m_start; m < m_end; m++)
			{
				XT_LSX2IP(W54, pW, 2*sizeof(xtfloatx2));
				W65 = XT_SEL32_LH_SX2(Wz6, W54);

				XT_MADD_SX2(S0_0, W65, Y01); XT_MADD_SX2(S1_0, Wz6, Y01);
				XT_MADD_SX2(S2_0, W65, Y23); XT_MADD_SX2(S3_0, Wz6, Y23);

				XT_LSX2IP(Y45, pY, sizeof(xtfloatx2));
				XT_LSX2IP(Y67, pY, sizeof(xtfloatx2));
				XT_LSX2IP(W32, pW, 2*sizeof(xtfloatx2));
				W43 = XT_SEL32_LH_SX2(W54, W32);

				XT_MADD_SX2(S0_1, W43, Y23); XT_MADD_SX2(S1_1, W54, Y23);
				XT_MADD_SX2(S2_1, W43, Y45); XT_MADD_SX2(S3_1, W54, Y45);

				XT_LSX2XC(Y89, pY, (Q - 8) * sizeof(float32_t));
				MACRO_LSX2XC1(W10, pW, 2*sizeof(xtfloatx2));
				W21 = XT_SEL32_LH_SX2(W32, W10);

				XT_MADD_SX2(S0_0, W21, Y45); XT_MADD_SX2(S1_0, W32, Y45);
				XT_MADD_SX2(S2_0, W21, Y67); XT_MADD_SX2(S3_0, W32, Y67);

				W0z = XT_SEL32_LH_SX2(W10, (xtfloatx2)(0.0f));

				XT_MADD_SX2(S0_1, W0z, Y67); XT_MADD_SX2(S1_1, W10, Y67);
				XT_MADD_SX2(S2_1, W0z, Y89); XT_MADD_SX2(S3_1, W10, Y89);

				XT_LSX2IP(Y01, pY, sizeof(xtfloatx2));
				XT_LSX2IP(Y23, pY, sizeof(xtfloatx2));
				XT_LSX2IP(Wz6, pW, 2*sizeof(xtfloatx2));
			}

			S0_0 += S0_1; S2_0 += S2_1;
			S1_0 += S1_1; S3_0 += S3_1;

			MACRO_ADD_HL_LH_XC2(S0_0);
			MACRO_ADD_HL_LH_XC2(S1_0);
			MACRO_ADD_HL_LH_XC2(S2_0);
			MACRO_ADD_HL_LH_XC2(S3_0);

			S0_0 = XT_SEL32_HL_SX2(S0_0, S1_0);
			S2_0 = XT_SEL32_HL_SX2(S2_0, S3_0);

			XT_SASX2IP(S0_0, alZ, pZ);
			XT_SASX2IP(S2_0, alZ, pZ);
		}

		/* Last N-1 samples of the i-th row */
		{
			xtfloatx2 S0, S2, S4, S0_1;
			xtfloatx2 S1, S3, S5, S1_1;
			S0 = S2 = S4 = S0_1 = (xtfloatx2)(0.0f);
			S1 = S3 = S5 = S1_1 = (xtfloatx2)(0.0f);

			__Pragma("loop_count min=1, max=11");
			for (m = m_start; m < m_end; m++)
			{
				XT_LSX2XP(Y45, pY, (Q - 4) * sizeof(float32_t));

				XT_LSX2IP(W54, pW, 2*sizeof(xtfloatx2));
				XT_LSX2IP(W32, pW, 2*sizeof(xtfloatx2));
				MACRO_LSX2XC1(W10, pW, 2*sizeof(xtfloatx2));

				W65 = XT_SEL32_LH_SX2(Wz6, W54);
				W43 = XT_SEL32_LH_SX2(W54, W32);
				W21 = XT_SEL32_LH_SX2(W32, W10);

				XT_MADD_SX2(S0, W65, Y01);		XT_MADD_SX2(S1, Wz6, Y01);
				XT_MADD_SX2(S2, W65, Y23);		XT_MADD_SX2(S3, Wz6, Y23);
				XT_MADD_SX2(S4, W65, Y45);		XT_MADD_SX2(S5, Wz6, Y45);

				XT_MADD_SX2(S0_1, W43, Y23);	XT_MADD_SX2(S1_1, W54, Y23);
				XT_MADD_SX2(S2, W43, Y45);		XT_MADD_SX2(S3, W54, Y45);

				XT_MADD_SX2(S0, W21, Y45);		XT_MADD_SX2(S1, W32, Y45);

				XT_LSX2IP(Y01, pY, sizeof(xtfloatx2));
				XT_LSX2IP(Y23, pY, sizeof(xtfloatx2));
				XT_LSX2IP(Wz6, pW, 2*sizeof(xtfloatx2));
			}

			S0 += S0_1;
			S1 += S1_1;

			MACRO_ADD_HL_LH_XC2(S0); MACRO_ADD_HL_LH_XC2(S3);
			MACRO_ADD_HL_LH_XC2(S1); MACRO_ADD_HL_LH_XC2(S4);
			MACRO_ADD_HL_LH_XC2(S2); MACRO_ADD_HL_LH_XC2(S5);

			S0 = XT_SEL32_HL_SX2(S0, S1);
			S2 = XT_SEL32_HL_SX2(S2, S3);
			S4 = XT_SEL32_HL_SX2(S4, S5);

			XT_SASX2IP(S0, alZ, pZ);
			XT_SASX2IP(S2, alZ, pZ);
			XT_SASX2IP(S4, alZ, pZ);
		}
	}
	XT_SASX2POSFP(alZ, pZ);
#   undef M
#   undef N
}

size_t conv2d_11x7f_getScratchSize(int P, int Q)
{
	(void)P, (void)Q;
	return 11 * WLEN * sizeof(xtfloatx2);
} // MxN=11x7


#else 

void conv2d_11x7f     (void* pScr, float32_t *z, const float32_t * x, const float32_t * y, int P, int Q)
{
    NASSERT_ALIGN(x,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(y,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(z,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(pScr,HIFI_SIMD_WIDTH);
    NASSERT(P>=0 && P%8==0);
    NASSERT(Q>=0 && Q%8==0);
    int i, j, m, n, n0, n1, m0, m1;
    float32_t * pwr = (float32_t*)pScr;
    xtfloat * restrict pWwr = (xtfloat*)pwr;
    const xtfloat * restrict pY;
    xtfloat * restrict pX;
    xtfloat * restrict pW;
    xtfloat * restrict pZ;
    xtfloat w0,w1, y0, y1, y2, y3, y4;
    xtfloat s0, s1, s2, s3;
    if (P <= 0 || Q <= 0) return;

#define N 7
#define M 11
    if (P <= 11)
    {
        for (i = 0; i < M + P - 1; i++)
        {
            for (j = 0; j < N + Q - 1; j++)
            {
                float32_t s;
                m0 = XT_MAX(i - P + 1, 0);
                m1 = XT_MIN(i + 1, M);
                n0 = XT_MAX(j - Q + 1, 0);
                n1 = XT_MIN(j + 1, N);
                s = 0;
                for (n = n0; n < n1; n++)
                for (m = m0; m < m1; m++)
                {
                    s += x[m*N + n] * y[(i - m)*Q + (j - n)];
                }
                z[i*(N + Q - 1) + j] = s;
            }
        }
    }
    else
    {
        
        pX = (xtfloat*)x;
        pW = (xtfloat*)pScr + M*N - 1;
        for (i = 0; i < M*N; i++)
        {
            XT_LSIP(w0, pX, sizeof(xtfloat));
            XT_SSXP(w0, pW, -1 * ((int)sizeof(xtfloat)));
        }

        /* first M-1 rows */
        for (i = 0; i < M - 1; i++)
        {
            for (j = 0; j < N - 1; j++)
            {
                float32_t s;
                m0 = XT_MAX(i - P + 1, 0);
                m1 = XT_MIN(i + 1, M);
                n0 = XT_MAX(j - Q + 1, 0);
                n1 = XT_MIN(j + 1, N);
                s = 0;
                for (n = n0; n < n1; n++)
                for (m = m0; m < m1; m++)
                {
                    s += x[m*N + n] * y[(i - m)*Q + (j - n)];
                }
                z[i*(N + Q - 1) + j] = s;
            }
            pZ = (xtfloat *)(z + i*(N + Q - 1) + (N - 1));
            for (j = 0; j < (Q - (N - 1)) >> 2; j++)
            {
                xtfloat w2, y5;
                s0 = XT_MOV_S(0.0f);
                s1 = XT_MOV_S(0.0f);
                s2 = XT_MOV_S(0.0f);
                s3 = XT_MOV_S(0.0f);
                pWwr = (xtfloat*)(pwr + ((M - 1) - i)*N);
                n0 = 4 * j;
                for (m = 0; m < i+1; m++)
                {
                    pY = (const xtfloat *)(y + (m)*Q + n0);
                    for (n = 0; n < 2; n++)
                    {
                        XT_LSIP(w0, pWwr, sizeof(xtfloat));
                        XT_LSIP(w1, pWwr, sizeof(xtfloat));
                        XT_LSIP(w2, pWwr, sizeof(xtfloat));
                        XT_LSIP(y0, pY, sizeof(xtfloat));
                        XT_LSIP(y1, pY, sizeof(xtfloat));
                        XT_LSIP(y2, pY, sizeof(xtfloat));
                        y3 = XT_LSI(pY, 0 * sizeof(xtfloat));
                        y4 = XT_LSI(pY, 1 * sizeof(xtfloat));
                        y5 = XT_LSI(pY, 2 * sizeof(xtfloat));
                        XT_MADD_S(s0, w0, y0);
                        XT_MADD_S(s1, w0, y1);
                        XT_MADD_S(s2, w0, y2);
                        XT_MADD_S(s3, w0, y3);

                        XT_MADD_S(s0, w1, y1);
                        XT_MADD_S(s1, w1, y2);
                        XT_MADD_S(s2, w1, y3);
                        XT_MADD_S(s3, w1, y4);

                        XT_MADD_S(s0, w2, y2);
                        XT_MADD_S(s1, w2, y3);
                        XT_MADD_S(s2, w2, y4);
                        XT_MADD_S(s3, w2, y5);
                    }
                    {
                        XT_LSIP(w0, pWwr, sizeof(xtfloat));
                        XT_LSIP(y0, pY, sizeof(xtfloat));
                        y1 = XT_LSI(pY, 0 * sizeof(xtfloat));
                        y2 = XT_LSI(pY, 1 * sizeof(xtfloat));
                        y3 = XT_LSI(pY, 2 * sizeof(xtfloat));
                        XT_MADD_S(s0, w0, y0);
                        XT_MADD_S(s1, w0, y1);
                        XT_MADD_S(s2, w0, y2);
                        XT_MADD_S(s3, w0, y3);
                    }
                }
                XT_SSIP(s0, pZ, sizeof(xtfloat));
                XT_SSIP(s1, pZ, sizeof(xtfloat));
                XT_SSIP(s2, pZ, sizeof(xtfloat));
                XT_SSIP(s3, pZ, sizeof(xtfloat));
            }
            for (j = (N - 1) + ((Q - (N - 1))&(~3)); j < Q; j++)
            {
                xtfloat s = XT_MOV_S(0.0f);
                pWwr = (xtfloat*)(pwr + ((M - 1) - i)*N);
                n0 = (j - (N - 1));
                for (m = 0; m < i + 1; m++)
                {
                    pY = (const xtfloat *)(y + (m)*Q + n0);
                    for (n = 0; n < N; n++)
                    {
                        XT_LSIP(w0, pWwr, sizeof(xtfloat));
                        XT_LSIP(y0, pY, sizeof(xtfloat));
                        XT_MADD_S(s, w0, y0);
                    }
                }
                XT_SSIP(s, pZ, sizeof(xtfloat));
            }

            for (j = Q; j < N + Q - 1; j++)
            {
                float32_t s;
                m0 = XT_MAX(i - P + 1, 0);
                m1 = XT_MIN(i + 1, M);
                n0 = XT_MAX(j - Q + 1, 0);
                n1 = XT_MIN(j + 1, N);
                s = 0;
                for (n = n0; n < n1; n++)
                for (m = m0; m < m1; m++)
                {
                    s += x[m*N + n] * y[(i - m)*Q + (j - n)];
                }
                z[i*(N + Q - 1) + j] = s;
            }

        }

        for (i = M - 1; i < P; i++)
        {
            pZ = (xtfloat *)(z + i*(N + Q - 1));
            for (j = 0; j < N - 1; j++)
            {
                m0 = i - (M - 1);
                n0 = (N - 1) - j;
                xtfloat s = XT_MOV_S(0.0f);
                for (m = 0; m < M; m++)
                {
                    pY = (const xtfloat *)(y + (m0 + m)*Q);
                    pWwr = (xtfloat *)(pwr + m*N + n0);
                    for (n = 0; n < j + 1; n++)
                    {
                        XT_LSIP(w0, pWwr, sizeof(xtfloat));
                        XT_LSIP(y0, pY, sizeof(xtfloat));
                        XT_MADD_S(s, w0, y0);
                    }
                }
                XT_SSIP(s, pZ, sizeof(xtfloat));
            }

            /* Main section */
            for (j = 0; j < (Q - (N - 1)) >> 2; j++)
            {
                xtfloat w2, y5;
                s0 = XT_MOV_S(0.0f);
                s1 = XT_MOV_S(0.0f);
                s2 = XT_MOV_S(0.0f);
                s3 = XT_MOV_S(0.0f);
                pWwr = (xtfloat*)pwr;
                m0 = i - (M - 1);
                n0 = 4*j;
                for (m = 0; m < M; m++)
                {
                    pY = (const xtfloat *)(y + (m0 + m)*Q + n0);
                    for (n = 0; n < 2; n++)
                    {
                        XT_LSIP(w0, pWwr, sizeof(xtfloat));
                        XT_LSIP(w1, pWwr, sizeof(xtfloat));
                        XT_LSIP(w2, pWwr, sizeof(xtfloat));
                        XT_LSIP(y0, pY, sizeof(xtfloat));
                        XT_LSIP(y1, pY, sizeof(xtfloat));
                        XT_LSIP(y2, pY, sizeof(xtfloat));
                        y3 = XT_LSI(pY, 0*sizeof(xtfloat));
                        y4 = XT_LSI(pY, 1 * sizeof(xtfloat));
                        y5 = XT_LSI(pY, 2 * sizeof(xtfloat));
                        XT_MADD_S(s0, w0, y0);
                        XT_MADD_S(s1, w0, y1);
                        XT_MADD_S(s2, w0, y2);
                        XT_MADD_S(s3, w0, y3);

                        XT_MADD_S(s0, w1, y1);
                        XT_MADD_S(s1, w1, y2);
                        XT_MADD_S(s2, w1, y3);
                        XT_MADD_S(s3, w1, y4);

                        XT_MADD_S(s0, w2, y2);
                        XT_MADD_S(s1, w2, y3);
                        XT_MADD_S(s2, w2, y4);
                        XT_MADD_S(s3, w2, y5);
                    }
                    {
                        XT_LSIP(w0, pWwr, sizeof(xtfloat));
                        XT_LSIP(y0, pY, sizeof(xtfloat));
                        y1 = XT_LSI(pY, 0 * sizeof(xtfloat));
                        y2 = XT_LSI(pY, 1 * sizeof(xtfloat));
                        y3 = XT_LSI(pY, 2 * sizeof(xtfloat));
                        XT_MADD_S(s0, w0, y0);
                        XT_MADD_S(s1, w0, y1);
                        XT_MADD_S(s2, w0, y2);
                        XT_MADD_S(s3, w0, y3);
                    }
                }
                XT_SSIP(s0, pZ, sizeof(xtfloat));
                XT_SSIP(s1, pZ, sizeof(xtfloat));
                XT_SSIP(s2, pZ, sizeof(xtfloat));
                XT_SSIP(s3, pZ, sizeof(xtfloat));
            }
            for (j = (N-1) + ((Q- (N-1))&(~3)); j < Q; j++)
            {
                xtfloat s = XT_MOV_S(0.0f);
                pWwr = (xtfloat*)pwr;
                m0 = i - (M - 1);
                n0 = (j - (N - 1));
                for (m = 0; m < M; m++)
                {
                    pY = (const xtfloat *)(y + (m0 + m)*Q + n0);
                    for (n = 0; n < N; n++)
                    {
                        XT_LSIP(w0, pWwr, sizeof(xtfloat));
                        XT_LSIP(y0, pY, sizeof(xtfloat));
                        XT_MADD_S(s, w0, y0);
                    }
                }
                XT_SSIP(s, pZ, sizeof(xtfloat));
            }

            for (j = Q; j < N + Q - 1; j++)
            {
                xtfloat s = XT_MOV_S(0.0f);
                n1 = N + Q - j - 1;
                m0 = i - (M - 1);
                n0 = j - (N - 1);
                for (m = 0; m < M; m++)
                {
                    pWwr = (xtfloat *)(pwr + m*N);
                    pY = (const xtfloat *)(y + (m0 + m)*Q + n0);
                    for (n = 0; n < n1; n++)
                    {
                        XT_LSIP(w0, pWwr, sizeof(xtfloat));
                        XT_LSIP(y0, pY, sizeof(xtfloat));
                        XT_MADD_S(s, w0, y0);
                    }
                }
                XT_SSIP(s, pZ, sizeof(xtfloat));
            }
        }

        /* Last M-1 rows */
        for (i = P; i < M + P - 1; i++)
        {
            for (j = 0; j < N - 1; j++)
            {
                float32_t s;
                m0 = XT_MAX(i - P + 1, 0);
                m1 = XT_MIN(i + 1, M);
                n0 = XT_MAX(j - Q + 1, 0);
                n1 = XT_MIN(j + 1, N);
                s = 0;
                for (n = n0; n < n1; n++)
                for (m = m0; m < m1; m++)
                {
                    s += x[m*N + n] * y[(i - m)*Q + (j - n)];
                }
                z[i*(N + Q - 1) + j] = s;
            }

            pZ = (xtfloat *)(z + i*(N + Q - 1) + (N - 1));
            for (j = 0; j < (Q - (N - 1)) >> 2; j++)
            {
                xtfloat w2, y5;
                s0 = XT_MOV_S(0.0f);
                s1 = XT_MOV_S(0.0f);
                s2 = XT_MOV_S(0.0f);
                s3 = XT_MOV_S(0.0f);
                pWwr = (xtfloat*)pwr;
                m1 = (M + P - i - 1);
                m0 = i - M + 1;
                n0 = 4*j;
                for (m = 0; m < m1; m++)
                {
                    pY = (const xtfloat *)(y + (m0 + m)*Q + n0);
                    for (n = 0; n < 2; n++)
                    {
                        XT_LSIP(w0, pWwr, sizeof(xtfloat));
                        XT_LSIP(w1, pWwr, sizeof(xtfloat));
                        XT_LSIP(w2, pWwr, sizeof(xtfloat));
                        XT_LSIP(y0, pY, sizeof(xtfloat));
                        XT_LSIP(y1, pY, sizeof(xtfloat));
                        XT_LSIP(y2, pY, sizeof(xtfloat));
                        y3 = XT_LSI(pY, 0*sizeof(xtfloat));
                        y4 = XT_LSI(pY, 1 * sizeof(xtfloat));
                        y5 = XT_LSI(pY, 2 * sizeof(xtfloat));
                        XT_MADD_S(s0, w0, y0);
                        XT_MADD_S(s1, w0, y1);
                        XT_MADD_S(s2, w0, y2);
                        XT_MADD_S(s3, w0, y3);

                        XT_MADD_S(s0, w1, y1);
                        XT_MADD_S(s1, w1, y2);
                        XT_MADD_S(s2, w1, y3);
                        XT_MADD_S(s3, w1, y4);

                        XT_MADD_S(s0, w2, y2);
                        XT_MADD_S(s1, w2, y3);
                        XT_MADD_S(s2, w2, y4);
                        XT_MADD_S(s3, w2, y5);
                    }
                    {
                        XT_LSIP(w0, pWwr, sizeof(xtfloat));
                        XT_LSIP(y0, pY, sizeof(xtfloat));
                        y1 = XT_LSI(pY, 0 * sizeof(xtfloat));
                        y2 = XT_LSI(pY, 1 * sizeof(xtfloat));
                        y3 = XT_LSI(pY, 2 * sizeof(xtfloat));
                        XT_MADD_S(s0, w0, y0);
                        XT_MADD_S(s1, w0, y1);
                        XT_MADD_S(s2, w0, y2);
                        XT_MADD_S(s3, w0, y3);
                    }
                }
                XT_SSIP(s0, pZ, sizeof(xtfloat));
                XT_SSIP(s1, pZ, sizeof(xtfloat));
                XT_SSIP(s2, pZ, sizeof(xtfloat));
                XT_SSIP(s3, pZ, sizeof(xtfloat));
            }
            for (j = (N - 1) + ((Q - (N - 1))&(~3)); j < Q; j++)
            {
                xtfloat s = XT_MOV_S(0.0f);
                pWwr = (xtfloat*)pwr;
                n0 = (j - (N - 1));
                m1 = (M + P - i - 1);
                m0 = i - M + 1;
                for (m = 0; m < m1; m++)
                {
                    pY = (const xtfloat *)(y + (m0 + m)*Q + n0);
                    for (n = 0; n < N; n++)
                    {
                        XT_LSIP(w0, pWwr, sizeof(xtfloat));
                        XT_LSIP(y0, pY, sizeof(xtfloat));
                        XT_MADD_S(s, w0, y0);
                    }
                }
                XT_SSIP(s, pZ, sizeof(xtfloat));
            }
            for (j = Q; j < N + Q - 1; j++)
            {
                float32_t s;
                m0 = XT_MAX(i - P + 1, 0);
                m1 = XT_MIN(i + 1, M);
                n0 = XT_MAX(j - Q + 1, 0);
                n1 = XT_MIN(j + 1, N);
                s = 0;
                for (n = n0; n < n1; n++)
                for (m = m0; m < m1; m++)
                {
                    s += x[m*N + n] * y[(i - m)*Q + (j - n)];
                }
                z[i*(N + Q - 1) + j] = s;
            }
        }
    }
#undef M
#undef N
}

size_t conv2d_11x7f_getScratchSize(int P, int Q)
{ 
    return 11 * 7 * sizeof(xtfloat);
} // MxN=11x7

#endif
