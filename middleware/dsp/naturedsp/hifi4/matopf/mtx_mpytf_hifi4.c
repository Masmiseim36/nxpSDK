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
  NatureDSP Signal Processing Library. Matrix Operations
   Matrix multiply
 * Optimized code for HiFi4
  IntegrIT, 2006-2018
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_matop.h"
/* Common helper macros. */
#include "common.h"
#include "common_fpu.h"

#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(void,mtx_mpytf,( void* pScr, float32_t * z, const float32_t * x,  const float32_t * yt, int M, int N, int P ))
#elif (HAVE_VFPU)
/*-------------------------------------------------------------------------
  Matrix Multiply
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and y. The columnar dimension of x must match the row dimension of y. 
  The resulting matrix has the same number of rows as x and the same number 
  of columns as y.
  Transposing API allows to interpret input yt as transposed matrix y.

  NOTE: lsh factor is not relevant for floating point routines.

  Functions require scratch memory for storing intermediate data. This 
  scratch memory area should be aligned on 8 byte boundary and its size is 
  calculated by dedicated scratch allocation functions.

  Two versions of functions available: regular version (mtx_mpy[t]32x32, 
  mtx_mpy[t]16x16, mtx_mpy[t]8x16, mtx_mpy[t]8x8, mtx[t]_mpyf) with 
  arbitrary arguments and faster version (mtx_mpy[t]32x32_fast, 
  mtx_mpy[t]16x16_fast, mtx_mpy[t]8x16_fast, mtx_mpy[t]8x8_fast, 
  mtx_mpy[t]f_fast) that apply some restrictions

  Precision:
  32x32 32-bit inputs, 32-bit output
  16x16 16-bit inputs, 16-bit output
  8x8   8-bit inputs, 8-bit output
  8x16  8/16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N]      input matrix x, Q7, Q15, Q31 or floating point
  y[N*P]      input matrix y, Q7, Q15, Q31 or floating point
  yt[P*N]     transposed input matrix y. Q31,Q15, Q7 floating point. (for 
              transposing API only)
  M           number of rows in matrix x and z
  N           number of columns in matrix x and number of rows in matrix y
  P           number of columns in matrices y and z
  lsh         left shift applied to the result (applied to the fixed-
              point functions only) 
  Output:
  z[M*P]      output matrix z, Q7, Q15, Q31 or floating point 
  Scratch:
  pScr        size in bytes defined by corresponding scratch allocation 
              functions

  Restrictions:
  For regular routines mpy[t]32x32, mtx_mpy[t]16x16, mtx_mpy[t]8x8, 
  mtx_mpy[t]8x16, mtx_mpy[t]f):
  pScr    aligned on 8-byte boundary
  x,y,z   should not overlap

  For faster routines (mtx_mpy[t]32x32_fast, mtx_mpy[t]16x16_fast, 
  mtx_mpy[t]8x8_fast, mtx_mpy[t]8x16_fast, 
  mtx_mpy[t]f_fast):
  x,y,z        should not overlap
  x,y,z,pScr   aligned on 8-byte boundary
  M,N,P        multiplies of 4
  lsh         should be in range:
              -31...31 for mtx_mpy32x32, mtx_mpy32x32_fast
              -15...15 for mtx_mpy16x16, mtx_mpy16x16_fast, mtx_mpy[t]8x8, 
                       mtx_mpy[t]8x8_fast, mtx_mpy[t]8x16, 
                       mtx_mpy[t]8x16_fast 

-------------------------------------------------------------------------*/
#define SZ_F32 (sizeof(float32_t))

/* Load scalar and replicate */
#define XT_LSRX2IP(d, a, off)\
{\
  ae_int32x2 t;\
  AE_L32_IP(t, castxcc(ae_int32,a), off);\
  d = XT_AE_MOVXTFLOATX2_FROMINT32X2(t);\
}

void mtx_mpytf( void* pScr, float32_t * z, const float32_t * x,  const float32_t * yt, int M, int N, int P )
{
  const xtfloatx2 *restrict pX0;
  const xtfloatx2 *restrict pX1;
  const xtfloatx2 *restrict pX2;
  const xtfloatx2 *restrict pX3;
  const xtfloatx2 *restrict pY0;
  const xtfloatx2 *restrict pY1;
  const xtfloatx2 *restrict pY2;
  const xtfloatx2 *restrict pY3;
        xtfloatx2 *restrict pZ0;
        xtfloatx2 *restrict pZ1;
        xtfloatx2 *restrict pZ2;
        xtfloatx2 *restrict pZ3;
        xtfloatx2 *restrict pS0;
  xtfloatx2 y0, y1, y2, y3;
  xtfloatx2 x0, x1, x2, x3;
  xtfloatx2 z00, z01, z10, z11, z20, z21, z30, z31;
  xtfloat rx, ry, rz;
  ae_valign v_z0, v_z1, v_z2, v_z3;
  int m, n, p;

  NASSERT(x);
  NASSERT(yt);
  NASSERT(z);
  NASSERT((z != x) && (z != yt));

  if (M<=0 || P<=0) return;
  /* If N<=0 then clear output matrix and return */
  if (N<=0)
  {
    int MP = M*P;
    pZ0 = (xtfloatx2 *)z;
    v_z0 = AE_ZALIGN64();
    z00 = (xtfloatx2)0.0f;
    for (n = 0; n < (MP>>1); n++)
    {
      XT_SASX2IP(z00, v_z0, pZ0);
    }
    XT_SASX2POSFP(v_z0, pZ0);
    if (MP&1) XT_SSI(z00, (xtfloat *)pZ0, 0);
    return;
  }

  /* Compute output matrix by 4x4 squares */
  if ((M>>2)>0)
  {
        for ( p = 0; p < (P>>2); p++ )
        {
            int p4 = p<<2;

			// Scratch
			pS0 = (xtfloatx2 *)pScr;
			pY0 = (const xtfloatx2 *)(yt+p4*N);
            pY1 = (const xtfloatx2 *)((float32_t *)pY0+N);
            pY2 = (const xtfloatx2 *)((float32_t *)pY1+N);
            pY3 = (const xtfloatx2 *)((float32_t *)pY2+N);
			for (n = 0; n < N; n++)
			{
				XT_LSRX2IP(y0, pY0, SZ_F32);
				XT_LSRX2IP(y1, pY1, SZ_F32);
				XT_LSRX2IP(y2, pY2, SZ_F32);
				XT_LSRX2IP(y3, pY3, SZ_F32);
				y0 = XT_SEL32_HH_SX2(y0, y1);
				y1 = XT_SEL32_HH_SX2(y2, y3);
				XT_SSX2IP(y0, pS0, sizeof(xtfloatx2));
				XT_SSX2IP(y1, pS0, sizeof(xtfloatx2));
			}

			pZ0 = (xtfloatx2 *)(z+p4);
            pZ1 = (xtfloatx2 *)((float32_t *)pZ0+P);
            pZ2 = (xtfloatx2 *)((float32_t *)pZ1+P);
            pZ3 = (xtfloatx2 *)((float32_t *)pZ2+P);
            v_z0 = v_z1 = v_z2 = v_z3 = AE_ZALIGN64();
            for ( m = 0; m < (M>>2); m++ )
            {
                int m4 = m<<2;

                pX0 = (const xtfloatx2 *)(x+m4*N);
                pX1 = (const xtfloatx2 *)((float32_t *)pX0+N);
                pX2 = (const xtfloatx2 *)((float32_t *)pX1+N);
                pX3 = (const xtfloatx2 *)((float32_t *)pX2+N);
      
                pY0 = (const xtfloatx2 *)(pScr);

                z00 = z01 = z10 = z11 = (xtfloatx2)0.0f;
                z20 = z21 = z30 = z31 = (xtfloatx2)0.0f;

                __Pragma("loop_count min=1")
                for ( n = 0; n < N; n++ )
                {
					/* load 4 rows from matrix 'x' */
					XT_LSRX2IP(x0, pX0, SZ_F32);
					XT_LSRX2IP(x1, pX1, SZ_F32);
					XT_LSRX2IP(x2, pX2, SZ_F32);
					XT_LSRX2IP(x3, pX3, SZ_F32);
					/* load 4 columns from matrix 'y' */
					XT_LSX2IP(y0, pY0, sizeof(xtfloatx2));
					XT_LSX2IP(y1, pY0, sizeof(xtfloatx2));

					XT_MADD_SX2(z00, x0, y0);
					XT_MADD_SX2(z01, x0, y1);
					XT_MADD_SX2(z10, x1, y0);
					XT_MADD_SX2(z11, x1, y1);
					XT_MADD_SX2(z20, x2, y0);
					XT_MADD_SX2(z21, x2, y1);
					XT_MADD_SX2(z30, x3, y0);
					XT_MADD_SX2(z31, x3, y1);
                }
                XT_SASX2IP(z00, v_z0, pZ0);
                XT_SASX2IP(z01, v_z0, pZ0);
	            XT_SASX2POSFP(v_z0, pZ0);
	            pZ0 = (xtfloatx2 *)((float32_t *)pZ0+4*(P-1));
                XT_SASX2IP(z10, v_z1, pZ1);
                XT_SASX2IP(z11, v_z1, pZ1);
	            XT_SASX2POSFP(v_z1, pZ1);
	            pZ1 = (xtfloatx2 *)((float32_t *)pZ1+4*(P-1));
                XT_SASX2IP(z20, v_z2, pZ2);
                XT_SASX2IP(z21, v_z2, pZ2);
	            XT_SASX2POSFP(v_z2, pZ2);
	            pZ2 = (xtfloatx2 *)((float32_t *)pZ2+4*(P-1));
                XT_SASX2IP(z30, v_z3, pZ3);
                XT_SASX2IP(z31, v_z3, pZ3);
	            XT_SASX2POSFP(v_z3, pZ3);
	            pZ3 = (xtfloatx2 *)((float32_t *)pZ3+4*(P-1));
            }
        }
    }
    /* Compute last odd pair of rows: */
    if (M&2)
    {
        for ( p = 0; p < (P>>2); p++ )
        {
            int p4 = p<<2;

			// Scratch
			pS0 = (xtfloatx2 *)pScr;
			pY0 = (const xtfloatx2 *)(yt+p4*N);
            pY1 = (const xtfloatx2 *)((float32_t *)pY0+N);
            pY2 = (const xtfloatx2 *)((float32_t *)pY1+N);
            pY3 = (const xtfloatx2 *)((float32_t *)pY2+N);
			for (n = 0; n < N; n++)
			{
				XT_LSRX2IP(y0, pY0, SZ_F32);
				XT_LSRX2IP(y1, pY1, SZ_F32);
				XT_LSRX2IP(y2, pY2, SZ_F32);
				XT_LSRX2IP(y3, pY3, SZ_F32);
				y0 = XT_SEL32_HH_SX2(y0, y1);
				y1 = XT_SEL32_HH_SX2(y2, y3);
				XT_SSX2IP(y0, pS0, sizeof(xtfloatx2));
				XT_SSX2IP(y1, pS0, sizeof(xtfloatx2));
			}

            pZ0 = (xtfloatx2 *)(z+(M&~3)*P+p4);
            pZ1 = (xtfloatx2 *)((float32_t *)pZ0+P);

            v_z0 = v_z1 = AE_ZALIGN64();
            pX0 = (const xtfloatx2 *)(x+(M&~3)*N);
            pX1 = (const xtfloatx2 *)((float32_t *)pX0+N);
            pY0 = (const xtfloatx2 *)pScr;
            z00 = z01 = z10 = z11 = (xtfloatx2)0.0f;
            __Pragma("loop_count min=1")
            for ( n = 0; n < N; n++ )
            {
                /* load 2 rows from matrix 'x' */
                XT_LSRX2IP(x0, pX0, SZ_F32);
                XT_LSRX2IP(x1, pX1, SZ_F32);
                /* load 4 columns from matrix 'y' */
                XT_LSX2IP(y0, pY0, sizeof(xtfloatx2));
                XT_LSX2IP(y1, pY0, sizeof(xtfloatx2));

				XT_MADD_SX2(z00, x0, y0);
                XT_MADD_SX2(z01, x0, y1);
                XT_MADD_SX2(z10, x1, y0);
                XT_MADD_SX2(z11, x1, y1);
			}
            XT_SASX2IP(z00, v_z0, pZ0);
            XT_SASX2IP(z01, v_z0, pZ0);
            XT_SASX2POSFP(v_z0, pZ0);
            XT_SASX2IP(z10, v_z1, pZ1);
            XT_SASX2IP(z11, v_z1, pZ1);
            XT_SASX2POSFP(v_z1, pZ1);
        }
    }
    /* compute last odd row */
    if (M&1)
    {
        for (p = 0; p < (P>>2); p++)
        {
            int p4 = p<<2;

			// Scratch
			pS0 = (xtfloatx2 *)pScr;
			pY0 = (const xtfloatx2 *)(yt+p4*N);
            pY1 = (const xtfloatx2 *)((float32_t *)pY0+N);
            pY2 = (const xtfloatx2 *)((float32_t *)pY1+N);
            pY3 = (const xtfloatx2 *)((float32_t *)pY2+N);
			for (n = 0; n < N; n++)
			{
				XT_LSRX2IP(y0, pY0, SZ_F32);
				XT_LSRX2IP(y1, pY1, SZ_F32);
				XT_LSRX2IP(y2, pY2, SZ_F32);
				XT_LSRX2IP(y3, pY3, SZ_F32);
				y0 = XT_SEL32_HH_SX2(y0, y1);
				y1 = XT_SEL32_HH_SX2(y2, y3);
				XT_SSX2IP(y0, pS0,sizeof(xtfloatx2));
				XT_SSX2IP(y1, pS0,sizeof(xtfloatx2));
			}

            v_z0 = AE_ZALIGN64();
			pZ0 = (xtfloatx2 *)(z+(M&~1)*P+p4);
            pX0 = (const xtfloatx2 *)(x+(M&~1)*N);
            pY0 = (const xtfloatx2 *)(pScr);
			z00 = z01 = (xtfloatx2)0.0f;
            __Pragma("loop_count min=1")
            for ( n = 0; n < N; n++ )
            {
                XT_LSRX2IP(x0, pX0, SZ_F32);
                XT_LSX2IP(y0, pY0, sizeof(xtfloatx2));
                XT_LSX2IP(y1, pY0, sizeof(xtfloatx2));
                XT_MADD_SX2(z00, x0, y0);
                XT_MADD_SX2(z01, x0, y1);
            }
            XT_SASX2IP(z00, v_z0, pZ0);
            XT_SASX2IP(z01, v_z0, pZ0);
            XT_SASX2POSFP(v_z0, pZ0);
        }
    }
    /* compute last P%4 columns. */
	if (P & 2)
	{
        pZ0 = (xtfloatx2 *)(z+(P&~3));
        pZ1 = (xtfloatx2 *)((float32_t *)pZ0+P);
        pZ2 = (xtfloatx2 *)((float32_t *)pZ1+P);
        pZ3 = (xtfloatx2 *)((float32_t *)pZ2+P);
        for (m = 0; m < (M >> 2); m++)
        {
			int m4 = m << 2;
            pX0 = (const xtfloatx2 *)(x + m4*N);
            pX1 = (const xtfloatx2 *)((float32_t *)pX0+N);
            pX2 = (const xtfloatx2 *)((float32_t *)pX1+N);
            pX3 = (const xtfloatx2 *)((float32_t *)pX2+N);
            pY0 = (const xtfloatx2 *)(yt + (P&~3)*N);
            pY1 = (const xtfloatx2 *)((float32_t *)pY0+N);

            z00 = z10 = z20 = z30 = (xtfloatx2)0.0f;
      
			v_z0 = v_z1 = v_z2 = v_z3 = AE_ZALIGN64();
            XT_LSIP(rx, castxcc(xtfloat,pX0), SZ_F32); x0 = rx;
            XT_LSIP(rx, castxcc(xtfloat,pX1), SZ_F32); x1 = rx;
            XT_LSIP(rx, castxcc(xtfloat,pX2), SZ_F32); x2 = rx;
            XT_LSIP(rx, castxcc(xtfloat,pX3), SZ_F32); x3 = rx;
			XT_LSIP(ry, castxcc(xtfloat,pY0), SZ_F32); y0 = ry;
			XT_LSIP(ry, castxcc(xtfloat,pY1), SZ_F32); y1 = ry;
			y0 = XT_SEL32_HH_SX2(y0, y1);
            __Pragma("loop_count min=1")
            for (n = 0; n < N; n++)
            {
                XT_MADD_SX2(z00, x0, y0);
                XT_MADD_SX2(z10, x1, y0);
                XT_MADD_SX2(z20, x2, y0);
                XT_MADD_SX2(z30, x3, y0);
                XT_LSIP(rx, castxcc(xtfloat,pX0), SZ_F32); x0 = rx;
                XT_LSIP(rx, castxcc(xtfloat,pX1), SZ_F32); x1 = rx;
                XT_LSIP(rx, castxcc(xtfloat,pX2), SZ_F32); x2 = rx;
                XT_LSIP(rx, castxcc(xtfloat,pX3), SZ_F32); x3 = rx;
				XT_LSIP(ry, castxcc(xtfloat,pY0), SZ_F32); y0 = ry;
				XT_LSIP(ry, castxcc(xtfloat,pY1), SZ_F32); y1 = ry;
				y0 = XT_SEL32_HH_SX2(y0, y1);
            }
            XT_SASX2IP(z00, v_z0, pZ0);
			XT_SASX2POSFP(v_z0, pZ0);
			pZ0 = (xtfloatx2 *)XT_ADDX8(P-1, (intptr_t)pZ0);
			pZ0 = (xtfloatx2 *)XT_ADDX8(P, (intptr_t)pZ0);
            XT_SASX2IP(z10, v_z1, pZ1);
			XT_SASX2POSFP(v_z1, pZ1);
			pZ1 = (xtfloatx2 *)XT_ADDX8(P-1, (intptr_t)pZ1);
			pZ1 = (xtfloatx2 *)XT_ADDX8(P, (intptr_t)pZ1);
            XT_SASX2IP(z20, v_z2, pZ2);
			XT_SASX2POSFP(v_z2, pZ2);
			pZ2 = (xtfloatx2 *)XT_ADDX8(P-1, (intptr_t)pZ2);
			pZ2 = (xtfloatx2 *)XT_ADDX8(P, (intptr_t)pZ2);
            XT_SASX2IP(z30, v_z3, pZ3);
			XT_SASX2POSFP(v_z3, pZ3);
			pZ3 = (xtfloatx2 *)XT_ADDX8(P-1, (intptr_t)pZ3);
			pZ3 = (xtfloatx2 *)XT_ADDX8(P, (intptr_t)pZ3);
        }
		if (M & 2)
		{
			xtfloat rx0, rx1, ry0, ry1;
			xtfloatx2 t0;

            pX0 = (const xtfloatx2 *)(x + (M&~3)*N);
            pX1 = (const xtfloatx2 *)((float32_t *)pX0+N);
            pY0 = (const xtfloatx2 *)(yt + (P&~3)*N);
            pY1 = (const xtfloatx2 *)((float32_t *)pY0+N);

            z00 = z10 = (xtfloatx2)0.0f;
      
			v_z0 = v_z1 = AE_ZALIGN64();
			XT_LSIP(ry0, castxcc(xtfloat,pY0), SZ_F32); y0 = ry0;
			XT_LSIP(ry1, castxcc(xtfloat,pY1), SZ_F32); y1 = ry1;
            __Pragma("loop_count min=1")
            for (n = 0; n < N; n++)
            {
                XT_LSIP(rx0, castxcc(xtfloat,pX0), SZ_F32); x0 = rx0;
                XT_LSIP(rx1, castxcc(xtfloat,pX1), SZ_F32); x1 = rx1;
				t0 = XT_SEL32_HH_SX2(y0, y1);
				XT_LSIP(ry0, castxcc(xtfloat,pY0), SZ_F32); y0 = ry0;
				XT_LSIP(ry1, castxcc(xtfloat,pY1), SZ_F32); y1 = ry1;
                XT_MADD_SX2(z00, x0, t0);
                XT_MADD_SX2(z10, x1, t0);
            }
            XT_SASX2IP(z00, v_z0, pZ0);
			XT_SASX2POSFP(v_z0, pZ0);
			pZ0 = (xtfloatx2 *)XT_ADDX8(P-1, (intptr_t)pZ0);
            XT_SASX2IP(z10, v_z1, pZ1);
			XT_SASX2POSFP(v_z1, pZ1);
			pZ1 = (xtfloatx2 *)XT_ADDX8(P-1, (intptr_t)pZ1);
		}
		if (M & 1)
		{
            pX0 = (const xtfloatx2 *)(x + (M&~1)*N);
            pY0 = (const xtfloatx2 *)(yt + (P&~3)*N);
            pY1 = (const xtfloatx2 *)((float32_t *)pY0+N);

            z00 = (xtfloatx2)0.0f;
      
			v_z0 = AE_ZALIGN64();
			XT_LSIP(ry, castxcc(xtfloat,pY0), SZ_F32); y0 = ry;
			XT_LSIP(ry, castxcc(xtfloat,pY1), SZ_F32); y1 = ry;
            __Pragma("loop_count min=1")
            for (n = 0; n < N; n++)
            {
                XT_LSIP(rx, castxcc(xtfloat,pX0), SZ_F32); x0 = rx;
				y0 = XT_SEL32_HH_SX2(y0, y1);
                XT_MADD_SX2(z00, x0, y0);
				XT_LSIP(ry, castxcc(xtfloat,pY0), SZ_F32); y0 = ry;
				XT_LSIP(ry, castxcc(xtfloat,pY1), SZ_F32); y1 = ry;
            }
            XT_SASX2IP(z00, v_z0, pZ0);
			XT_SASX2POSFP(v_z0, pZ0);
			pZ0 = (xtfloatx2 *)XT_ADDX4(P-2, (intptr_t)pZ1);
		}
	}
	if (P & 1)
	{
        pZ0 = (xtfloatx2 *)(z + (P&~1));
        __Pragma("loop_count min=1")
        for (m = 0; m < M; m++)
        {
            pX0 = (const xtfloatx2 *)(x + m*N);
            pY0 = (const xtfloatx2 *)(yt + (P&~1)*N);

            rz = 0.0f;
      
            __Pragma("loop_count min=1")
            for (n = 0; n < N; n++)
            {
                XT_LSIP(rx, castxcc(xtfloat,pX0), SZ_F32);
                XT_LSIP(ry, castxcc(xtfloat,pY0), SZ_F32);
                XT_MADD_S(rz, rx, ry);
            }
            XT_SSXP(rz, castxcc(xtfloat,pZ0), SZ_F32*P);
        }
	}
} /* mtx_mpytf() */
#elif (HAVE_FPU)
void mtx_mpytf( void* pScr, float32_t * z, const float32_t * x,  const float32_t * yt, int M, int N, int P )
{
    int m, n, p, P0=(P&~3);
          xtfloat * restrict pZ;
    const xtfloat * restrict pX;
    const xtfloat * restrict pY;
    const xtfloat * restrict pY0;
    const xtfloat * restrict pY1;
    const xtfloat * restrict pY2;
    const xtfloat * restrict pY3;

    NASSERT(x);
    NASSERT(yt);
    NASSERT(z);
    NASSERT((z != x) && (z != yt));
    if ((M <= 0) || (P <= 0)) return;
    if (N <= 0)
    {
        pZ = (xtfloat *)(z);
        for (m = 0; m < (M * P); m++)
        {
            XT_SSIP(XT_CONST_S(0), pZ, sizeof(xtfloat));
        }
        return;
    }
    pZ = (xtfloat  *)(z);
    if (P0>0)
    {
        for (m = 0; m<M; m++)
        {
          pZ = (xtfloat  *)(z+m*P);
          for (p = 0; p<P0; p+=4)
          {
            xtfloat A0,A1,A2,A3,x0,y0,y1,y2,y3;
            pX = (const xtfloat  *)(x+m*N);
            pY = (const xtfloat  *)(yt+p*N);
            pY0 = (const xtfloat  *)pY;
            pY1 = (const xtfloat  *)(pY0+N);
            pY2 = (const xtfloat  *)(pY1+N);
            pY3 = (const xtfloat  *)(pY2+N);
            A0=A1=A2=A3=XT_CONST_S(0);
            for ( n=0; n<N; n++ )
            {
              XT_LSIP(x0, pX, sizeof(xtfloat));
			  /*
              y1=XT_LSI(pY, 1*sizeof(xtfloat));
              y2=XT_LSI(pY, 2*sizeof(xtfloat));
              y3=XT_LSI(pY, 3*sizeof(xtfloat));
              XT_LSXP(y0, pY, P*sizeof(xtfloat));
			  */
			  XT_LSIP(y0, pY0, sizeof(xtfloat));
			  XT_LSIP(y1, pY1, sizeof(xtfloat));
			  XT_LSIP(y2, pY2, sizeof(xtfloat));
			  XT_LSIP(y3, pY3, sizeof(xtfloat));
              XT_MADD_S(A0, x0, y0);
              XT_MADD_S(A1, x0, y1);
              XT_MADD_S(A2, x0, y2);
              XT_MADD_S(A3, x0, y3);
            }
            XT_SSIP(A0, pZ, sizeof(xtfloat));
            XT_SSIP(A1, pZ, sizeof(xtfloat));
            XT_SSIP(A2, pZ, sizeof(xtfloat));
            XT_SSIP(A3, pZ, sizeof(xtfloat));
          }
        }
    }
    if (P&2)
    {
        for (m = 0; m<M; m++)
        {
            xtfloat A0,A1,x0,y0,y1;
            p = (P&~3);
            pZ = (xtfloat  *)(z+m*P+p);
            pX = (const xtfloat  *)(x+m*N);
            pY = (const xtfloat  *)(yt+p*N);
            pY0 = (const xtfloat  *)pY;
            pY1 = (const xtfloat  *)(pY0+N);
            A0=A1=XT_CONST_S(0);
            for ( n=0; n<N; n++ )
            {
              XT_LSIP(x0, pX, sizeof(xtfloat));
			  /*
              y1=XT_LSI(pY, 1*sizeof(xtfloat));
              XT_LSXP(y0, pY, P*sizeof(xtfloat));
			  */
			  XT_LSIP(y0, pY0, sizeof(xtfloat));
			  XT_LSIP(y1, pY1, sizeof(xtfloat));
              XT_MADD_S(A0, x0, y0);
              XT_MADD_S(A1, x0, y1);
            }
            XT_SSIP(A0, pZ, sizeof(xtfloat));
            XT_SSIP(A1, pZ, sizeof(xtfloat));
        }
    }
    if (P&1)
    {
        for (m = 0; m<M; m++)
        {
            xtfloat A0,x0,y0;
            p = (P&~1);
            pZ = (xtfloat  *)(z+m*P+p);
            pX = (const xtfloat  *)(x+m*N);
            pY = (const xtfloat  *)(yt+p*N);
            pY0 = (const xtfloat  *)pY;
            A0=XT_CONST_S(0);
            for ( n=0; n<N; n++ )
            {
              XT_LSIP(x0, pX, sizeof(xtfloat));
//              XT_LSXP(y0, pY, P*sizeof(xtfloat));
			  XT_LSIP(y0, pY0, sizeof(xtfloat));
              XT_MADD_S(A0, x0, y0);
            }
            XT_SSIP(A0, pZ, sizeof(xtfloat));
        }
    }

} /* mtx_mpytf() */

#endif

size_t mtx_mpytf_getScratchSize ( int M, int N, int P )
{
    (void)M; (void)P;
	return N<=0?0:((((N)+1)&(~1))*4*sizeof(float32_t));
} /* mtx_mpytf_getScratchSize() */
