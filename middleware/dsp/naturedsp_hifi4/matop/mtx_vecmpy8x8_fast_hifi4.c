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

/* Library API */
#include "NatureDSP_Signal_matop.h"
#include "NatureDSP_types.h"
#include "common.h"
/* Code optimized for HiFi4 core */

/*-------------------------------------------------------------------------
  Matrix by Vector Multiply
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and vector y. 
  NOTE: lsh factor is not relevant for floating point routines.

  Two versions of functions available: regular version (mtx_vecmpy32x32,  
  mtx_vecmpy16x16, mtx_vecmpy8x8, mtx_vecmpy8x16, mtx_vecmpyf) with arbitrary 
  arguments and faster version (mtx_vecmpy32x32_fast, mtx_vecmpy16x16_fast, 
  mtx_vecmpy8x8_fast, mtx_vecmpy8x16_fast,  mtx_vecmpyf_fast) that apply 
  some restrictions

  Precision: 
  32x32 32-bit input, 32-bit output
  16x16 16-bit input, 16-bit output
  8x8   8-bit inputs, 8-bit output
  8x16  8/16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N] input matrix,Q31,Q15 or floating point
  y[N]   input vector,Q31,Q15 or floating point
  M      number of rows in matrix x
  N      number of columns in matrix x
  lsh    additional left shift(applied to the fixed-
         point functions only) 
  Output:
  z[M]   output vector,Q31,Q15 or floating point

  Restriction:
  For regular routines (mtx_vecmpy32x32, mtx_vecmpy16x16, mtx_vecmpy8x8,
  mtx_vecmpy8x16,  mtx_vecmpyf)
  x,y,z should not overlap

  For faster routines  (mtx_vecmpy32x32_fast, mtx_vecmpy16x16_fast, 
  mtx_vecmpy8x8_fast, mtx_vecmpy8x16_fast, mtx_vecmpyf_fast)
  x,y,z   should not overlap
  x,y     aligned on 8-byte boundary
  N, M    multiples of 4
  lsh     should be in range:
          -31...31 for mtx_vecmpy32x32, mtx_vecmpy32x32_fast
          -15...15 for mtx_vecmpy16x16, mtx_vecmpy16x16_fast, 
                   mtx_vecmpy8x8_fast, mtx_vecmpy8x16_fast   
-------------------------------------------------------------------------*/
void mtx_vecmpy8x8_fast( int8_t* restrict z,
                 const int8_t* restrict x,
                 const int8_t* restrict y,
                 int M, int N, int lsh)
{
    int m,n;
    NASSERT_ALIGN(z,XCHAL_DATA_WIDTH);
    NASSERT_ALIGN(x,XCHAL_DATA_WIDTH);
    NASSERT_ALIGN(y,XCHAL_DATA_WIDTH);
    NASSERT(M%4==0);
    NASSERT(N%4==0);
    NASSERT(lsh >= -15 && lsh <= 15);
    if (M<=0) return;
    if (N<=0)    /* exceptional situation */
    {
        for (m=0; m<M; m++) z[m]=0;
        return ;
    }

    const int8_t   * restrict pX0;
    const int8_t   * restrict pX1;
    const int8_t   * restrict pX2;
    const int8_t   * restrict pX3;
    const int8_t   * restrict pX1_;
    const int8_t   * restrict pX3_;
    const int8_t   * restrict pY;
    const int8_t   * restrict pY_;
          ae_int16 * restrict pZ;
    ae_int64 rnd;
    int rsh=7-lsh;

    rnd=AE_SLAA64S(1,rsh+7);
    pZ=(ae_int16 *)z;

	pX0=(const int8_t *)(x);
	pX1=(const int8_t *)(x+N);
	pX2=(const int8_t *)(x+N*2);
	pX3=(const int8_t *)(x+N*3);
    pY =(const int8_t *)y;

	if ((N&4)&&(N&8))
	{
		// Even rows (indexing from 0: 0,2,4,...) are such that every couple of quads of x belongs to the same bank
		// Odd rows are such that the first quad belongs to the same bank as the last quad of the previous row,
		//  and the rest couples of quads belong to the same banks
		// E.g. for N=28 and Nbanks=4 (every number represents the number of bank, the corresponding quad belongs to):
		//  1 1 2 2 3 3 4       1 1 1 1 1 ...
		//  4 1 1 2 2 3 3   ~   2 1 2 1 2 ...
		//  4 4 1 1 2 2 3       2 2 2 2 2 ...
		//  3 4 4 1 1 2 2       1 2 1 2 1 ...
		// Mark "~" denotes that quads with the same numbers (1 or 2) can be loaded together in a single cycle
		__Pragma("ymemory(pX0)")
		__Pragma("ymemory(pX2)")
		for (m=0; m<(M>>2); m++)
		{
			ae_int64 A0,A1,A2,A3;
			ae_int16x4 y0,y1,x0,x1,x2,x3;
			A0=A1=A2=A3=rnd;
			pY =(const int8_t *)y;

			AE_L8X4F_IP(y0,pY,4*sizeof(int8_t));

			AE_L8X4F_IP(x0,pX0,4);
			__Pragma("ymemory(pX1_)")
			pX1_=pX1;
			AE_L8X4F_IP(x1,pX1_,4);
			pX1=pX1_;
			AE_L8X4F_IP(x2,pX2,4);
			AE_L8X4F_IP(x3,pX3,4);
			AE_MULAAAAQ16(A0,x0,y0);
			AE_MULAAAAQ16(A1,x1,y0);
			AE_MULAAAAQ16(A2,x2,y0);
			AE_MULAAAAQ16(A3,x3,y0);

			__Pragma("loop_count min=1")
			for (n=0; n<(N>>3); n++)
			{
				AE_L8X4F_IP(y0,pY,4*sizeof(int8_t));
				__Pragma("ymemory(pY_)")
				pY_=pY;
				AE_L8X4F_IP(y1,pY_,4*sizeof(int8_t));
				pY=pY_;

				AE_L8X4F_IP(x0,pX0,4);
				AE_L8X4F_IP(x1,pX1,4);
				AE_L8X4F_IP(x2,pX2,4);
				AE_L8X4F_IP(x3,pX3,4);
				AE_MULAAAAQ16(A0,x0,y0);
				AE_MULAAAAQ16(A1,x1,y0);
				AE_MULAAAAQ16(A2,x2,y0);
				AE_MULAAAAQ16(A3,x3,y0);

				AE_L8X4F_IP(x0,pX0,4);
				__Pragma("ymemory(pX1_)")
				pX1_=pX1;
				AE_L8X4F_IP(x1,pX1_,4);
				pX1=pX1_;
				AE_L8X4F_IP(x2,pX2,4);
				AE_L8X4F_IP(x3,pX3,4);
				AE_MULAAAAQ16(A0,x0,y1);
				AE_MULAAAAQ16(A1,x1,y1);
				AE_MULAAAAQ16(A2,x2,y1);
				AE_MULAAAAQ16(A3,x3,y1);
			}
			/* shift with saturation and pack results */
			{
				ae_int32x2 a01,a23;
				a01=AE_TRUNCA32X2F64S(A1,A0,40-rsh);
				a23=AE_TRUNCA32X2F64S(A3,A2,40-rsh);
				a01=AE_SRLI32(a01,24);
				a23=AE_SRLI32(a23,24);
				a01=AE_OR32(a01,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(a01),24)));
				a23=AE_OR32(a23,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(a23),24)));
				AE_S16_0_IP(AE_MOVINT16X4_FROMINT32X2(a01),pZ,2);
				AE_S16_0_IP(AE_MOVINT16X4_FROMINT32X2(a23),pZ,2);
			}
			pX0+=3*N;
			pX1+=3*N;
			pX2+=3*N;
			pX3+=3*N;
		}
	}
	else if (N&4)
	{
		// Even rows (indexing from 0: 0,2,4,...) are such that every couple of quads of x belongs to the same bank
		// Odd rows are such that the first quad belongs to the same bank as the last quad of the previous row,
		//  and the rest couples of quads belong to the same banks
		// E.g. for N=20 and Nbanks=4 (every number represents the number of bank, the corresponding quad belongs to):
		//  1 1 2 2 3       1 1 1 1 1 ...
		//  3 4 4 1 1   ~   1 2 1 2 1 ...
		//  2 2 3 3 4       2 2 2 2 2 ...
		//  4 1 1 2 2       2 1 2 1 2 ...
		// Mark "~" denotes that quads with the same numbers (1 or 2) can be loaded together in a single cycle
		__Pragma("ymemory(pX0)")
		__Pragma("ymemory(pX2)")
		for (m=0; m<(M>>2); m++)
		{
			ae_int64 A0,A1,A2,A3;
			ae_int16x4 y0,y1,x0,x1,x2,x3;
			A0=A1=A2=A3=rnd;
			pY =(const int8_t *)y;

			AE_L8X4F_IP(y0,pY,4*sizeof(int8_t));

			AE_L8X4F_IP(x0,pX0,4);
			AE_L8X4F_IP(x1,pX1,4);
			AE_L8X4F_IP(x2,pX2,4);
			AE_L8X4F_IP(x3,pX3,4);
			AE_MULAAAAQ16(A0,x0,y0);
			AE_MULAAAAQ16(A1,x1,y0);
			AE_MULAAAAQ16(A2,x2,y0);
			AE_MULAAAAQ16(A3,x3,y0);

			for (n=0; n<(N>>3); n++)
			{
				AE_L8X4F_IP(y0,pY,4*sizeof(int8_t));
				__Pragma("ymemory(pY_)")
				pY_=pY;
				AE_L8X4F_IP(y1,pY_,4*sizeof(int8_t));
				pY=pY_;

				AE_L8X4F_IP(x0,pX0,4);
				__Pragma("ymemory(pX1_)")
				pX1_=pX1;
				AE_L8X4F_IP(x1,pX1_,4);
				pX1=pX1_;
				AE_L8X4F_IP(x2,pX2,4);
				AE_L8X4F_IP(x3,pX3,4);
				AE_MULAAAAQ16(A0,x0,y0);
				AE_MULAAAAQ16(A1,x1,y0);
				AE_MULAAAAQ16(A2,x2,y0);
				AE_MULAAAAQ16(A3,x3,y0);

				AE_L8X4F_IP(x0,pX0,4);
				AE_L8X4F_IP(x1,pX1,4);
				AE_L8X4F_IP(x2,pX2,4);
				AE_L8X4F_IP(x3,pX3,4);
				AE_MULAAAAQ16(A0,x0,y1);
				AE_MULAAAAQ16(A1,x1,y1);
				AE_MULAAAAQ16(A2,x2,y1);
				AE_MULAAAAQ16(A3,x3,y1);
			}
			/* shift with saturation and pack results */
			{
				ae_int32x2 a01,a23;
				a01=AE_TRUNCA32X2F64S(A1,A0,40-rsh);
				a23=AE_TRUNCA32X2F64S(A3,A2,40-rsh);
				a01=AE_SRLI32(a01,24);
				a23=AE_SRLI32(a23,24);
				a01=AE_OR32(a01,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(a01),24)));
				a23=AE_OR32(a23,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(a23),24)));
				AE_S16_0_IP(AE_MOVINT16X4_FROMINT32X2(a01),pZ,2);
				AE_S16_0_IP(AE_MOVINT16X4_FROMINT32X2(a23),pZ,2);
			}
			pX0+=3*N;
			pX1+=3*N;
			pX2+=3*N;
			pX3+=3*N;
		}
	}
	else if (N&8)
	{
		// Every couple of quads of x belongs to the bank different from the bank the quad from the previous row belongs to,
		//  i.e. every quad from the even line can be loaded together with the quad from the odd line in a single cycle
		// E.g. for N=24 and Nbanks=4 (every number represents the number of bank, the corresponding quad belongs to):
		//  1 1 2 2 3 3       1 1 ...
		//  4 4 1 1 2 2   ~   2 2 ...
		//  3 3 4 4 1 1       1 1 ...
		//  2 2 3 3 4 4       2 2 ...
		// Mark "~" denotes that quads with the same numbers (1 or 2) can be loaded together in a single cycle

		__Pragma("ymemory(pX0)")
		__Pragma("ymemory(pX2)")
		for (m=0; m<M; m+=4)
		{
			ae_int64 A0,A1,A2,A3;
			ae_int16x4 y0,y1,x0,x1,x2,x3;
			A0=A1=A2=A3=rnd;
			pY =(const int8_t *)y;

			__Pragma("loop_count min=1")
			for (n=0; n<(N>>3); n++)
			{
				AE_L8X4F_IP(y0,pY,4*sizeof(int8_t));
				__Pragma("ymemory(pY_)")
				pY_=pY;
				AE_L8X4F_IP(y1,pY_,4*sizeof(int8_t));
				pY=pY_;

				AE_L8X4F_IP(x0,pX0,4);
				AE_L8X4F_IP(x1,pX1,4);
				AE_L8X4F_IP(x2,pX2,4);
				AE_L8X4F_IP(x3,pX3,4);
				AE_MULAAAAQ16(A0,x0,y0);
				AE_MULAAAAQ16(A1,x1,y0);
				AE_MULAAAAQ16(A2,x2,y0);
				AE_MULAAAAQ16(A3,x3,y0);

				AE_L8X4F_IP(x0,pX0,4);
				AE_L8X4F_IP(x1,pX1,4);
				AE_L8X4F_IP(x2,pX2,4);
				AE_L8X4F_IP(x3,pX3,4);
				AE_MULAAAAQ16(A0,x0,y1);
				AE_MULAAAAQ16(A1,x1,y1);
				AE_MULAAAAQ16(A2,x2,y1);
				AE_MULAAAAQ16(A3,x3,y1);
			}
			/* shift with saturation and pack results */
			{
				ae_int32x2 a01,a23;
				a01=AE_TRUNCA32X2F64S(A1,A0,40-rsh);
				a23=AE_TRUNCA32X2F64S(A3,A2,40-rsh);
				a01=AE_SRLI32(a01,24);
				a23=AE_SRLI32(a23,24);
				a01=AE_OR32(a01,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(a01),24)));
				a23=AE_OR32(a23,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(a23),24)));
				AE_S16_0_IP(AE_MOVINT16X4_FROMINT32X2(a01),pZ,2);
				AE_S16_0_IP(AE_MOVINT16X4_FROMINT32X2(a23),pZ,2);
			}
			pX0+=3*N;
			pX1+=3*N;
			pX2+=3*N;
			pX3+=3*N;
		}
	}
	else
	{
		// All couples of quads of x from all lines belong to the same bank
		// E.g. for N=32 and Nbanks=4 (every number represents the number of bank, the corresponding quad belongs to):
		//  1 1 2 2 3 3 4 4       1 1 2 2 ...
		//  1 1 2 2 3 3 4 4   ~   1 1 2 2 ...
		//  1 1 2 2 3 3 4 4       1 1 2 2 ...
		//  1 1 2 2 3 3 4 4       1 1 2 2 ...
		// Mark "~" denotes that quads with the same numbers (1 or 2) can be loaded together in a single cycle
		__Pragma("ymemory(pX0)")
		__Pragma("ymemory(pX2)")
		for (m=0; m<M; m+=4)
		{
			ae_int64 A0,A1,A2,A3;
			ae_int16x4 y0,y1,x0,x1,x2,x3;
			A0=A1=A2=A3=rnd;
			pY =(const int8_t *)y;

			__Pragma("loop_count min=2")
			for (n=0; n<(N>>3); n++)
			{
				AE_L8X4F_IP(y0,pY,4*sizeof(int8_t));
				__Pragma("ymemory(pY_)")
				pY_=pY;
				AE_L8X4F_IP(y1,pY_,4*sizeof(int8_t));
				pY=pY_;

				AE_L8X4F_IP(x0,pX0,4);
				__Pragma("ymemory(pX1_)")
				pX1_=pX1;
				AE_L8X4F_IP(x1,pX1_,4);
				pX1=pX1_;
				AE_L8X4F_IP(x2,pX2,4);
				__Pragma("ymemory(pX3_)")
				pX3_=pX3;
				AE_L8X4F_IP(x3,pX3_,4);
				pX3=pX3_;
				AE_MULAAAAQ16(A0,x0,y0);
				AE_MULAAAAQ16(A1,x1,y0);
				AE_MULAAAAQ16(A2,x2,y0);
				AE_MULAAAAQ16(A3,x3,y0);

				AE_L8X4F_IP(x0,pX0,4);
				__Pragma("ymemory(pX1_)")
				pX1_=pX1;
				AE_L8X4F_IP(x1,pX1_,4);
				pX1=pX1_;
				AE_L8X4F_IP(x2,pX2,4);
				__Pragma("ymemory(pX3_)")
				pX3_=pX3;
				AE_L8X4F_IP(x3,pX3_,4);
				pX3=pX3_;
				AE_MULAAAAQ16(A0,x0,y1);
				AE_MULAAAAQ16(A1,x1,y1);
				AE_MULAAAAQ16(A2,x2,y1);
				AE_MULAAAAQ16(A3,x3,y1);
			}
			/* shift with saturation and pack results */
			{
				ae_int32x2 a01,a23;
				a01=AE_TRUNCA32X2F64S(A1,A0,40-rsh);
				a23=AE_TRUNCA32X2F64S(A3,A2,40-rsh);
				a01=AE_SRLI32(a01,24);
				a23=AE_SRLI32(a23,24);
				a01=AE_OR32(a01,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(a01),24)));
				a23=AE_OR32(a23,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(a23),24)));
				AE_S16_0_IP(AE_MOVINT16X4_FROMINT32X2(a01),pZ,2);
				AE_S16_0_IP(AE_MOVINT16X4_FROMINT32X2(a23),pZ,2);
			}
			pX0+=3*N;
			pX1+=3*N;
			pX2+=3*N;
			pX3+=3*N;
		}
	}
}
