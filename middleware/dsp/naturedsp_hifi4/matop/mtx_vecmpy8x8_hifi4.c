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
void mtx_vecmpy8x8   ( int8_t* restrict z,
                 const int8_t* restrict x,
                 const int8_t* restrict y,
                 int M, int N, int lsh)
{
    int m,n;
    NASSERT(lsh >= -15 && lsh <= 15);
    if (M<=0) return;
    if (N<=0)    /* exceptional situation */
    {
        for (m=0; m<M; m++) z[m]=0;
        return ;
    }
    ae_int64 rnd;
    int rsh=7-lsh;
	rnd=AE_SLAA64S(1,rsh+15);
	for (m=0; m<(M&~3); m+=4)
	{
        ae_int64 a0,a1,a2,a3;
        a0=a1=a2=a3=AE_ZERO64();
		for (n=0; n<(N&~1); n+=2)
		{
            ae_int32x2 s0,t0,t1,t2,t3;
            s0=AE_MOVDA32X2((uint8_t)y[        n+0],(uint8_t)y[        n+1]);
            t0=AE_MOVDA32X2((uint8_t)x[(m+0)*N+n+0],(uint8_t)x[(m+0)*N+n+1]);
            t1=AE_MOVDA32X2((uint8_t)x[(m+1)*N+n+0],(uint8_t)x[(m+1)*N+n+1]);
            t2=AE_MOVDA32X2((uint8_t)x[(m+2)*N+n+0],(uint8_t)x[(m+2)*N+n+1]);
            t3=AE_MOVDA32X2((uint8_t)x[(m+3)*N+n+0],(uint8_t)x[(m+3)*N+n+1]);
            t0=AE_SEXT32(t0,7);
            t1=AE_SEXT32(t1,7);
            t2=AE_SEXT32(t2,7);
            t3=AE_SEXT32(t3,7);
            s0=AE_SEXT32(s0,7);
            AE_MULAAD32_HH_LL(a0,t0,s0);
            AE_MULAAD32_HH_LL(a1,t1,s0);
            AE_MULAAD32_HH_LL(a2,t2,s0);
            AE_MULAAD32_HH_LL(a3,t3,s0);
		}
		if (N&1)
		{
            ae_int32x2 s0,t0,t1;
            s0=AE_MOVDA32X2((uint8_t)y[        n],(uint8_t)y[        n]);
            t0=AE_MOVDA32X2((uint8_t)x[(m+0)*N+n],(uint8_t)x[(m+1)*N+n]);
            t1=AE_MOVDA32X2((uint8_t)x[(m+2)*N+n],(uint8_t)x[(m+3)*N+n]);
            t0=AE_SEXT32(t0,7);
            t1=AE_SEXT32(t1,7);
            s0=AE_SEXT32(s0,7);
            AE_MULA32_HH(a0,t0,s0);
            AE_MULA32_LH(a1,t0,s0);
            AE_MULA32_HH(a2,t1,s0);
            AE_MULA32_LH(a3,t1,s0);
		}
		// Pack, round, store
		{
            ae_f16x4 t0;
            t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,a1,56-rsh),AE_TRUNCA32X2F64S(a2,a3,56-rsh));
            t0=AE_SRAI16(AE_ADD16S(t0,AE_MOVDA16(128)),8);
            z[m+0]=(int8_t)AE_MOVAD16_3(t0);
            z[m+1]=(int8_t)AE_MOVAD16_2(t0);
            z[m+2]=(int8_t)AE_MOVAD16_1(t0);
            z[m+3]=(int8_t)AE_MOVAD16_0(t0);
		}
	}
	if (M&2)
	{
		m=M&~3;
        ae_int64 a0,a1;
        a0=a1=AE_ZERO64();
        for (n=0; n<(N&~1); n+=2)
        {
            ae_int32x2 s0,t0,t1;
            s0=AE_MOVDA32X2((uint8_t)y[        n+0],(uint8_t)y[        n+1]);
            t0=AE_MOVDA32X2((uint8_t)x[(m+0)*N+n+0],(uint8_t)x[(m+0)*N+n+1]);
            t1=AE_MOVDA32X2((uint8_t)x[(m+1)*N+n+0],(uint8_t)x[(m+1)*N+n+1]);
            t0=AE_SEXT32(t0,7);
            t1=AE_SEXT32(t1,7);
            s0=AE_SEXT32(s0,7);
            AE_MULAAD32_HH_LL(a0,t0,s0);
            AE_MULAAD32_HH_LL(a1,t1,s0);
        }
        if (N&1)
        {
            ae_int32x2 s0,t0;
            s0=AE_MOVDA32X2((uint8_t)y[        n],(uint8_t)y[        n]);
            t0=AE_MOVDA32X2((uint8_t)x[(m+0)*N+n],(uint8_t)x[(m+1)*N+n]);
            t0=AE_SEXT32(t0,7);
            s0=AE_SEXT32(s0,7);
            AE_MULA32_HH(a0,t0,s0);
            AE_MULA32_LH(a1,t0,s0);
        }
		// Pack, round, store
        {
            ae_f16x4 t0;
            t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,a1,56-rsh),AE_TRUNCA32X2F64S(a0,a1,56-rsh));
            t0=AE_SRAI16(AE_ADD16S(t0,AE_MOVDA16(128)),8);
            z[m+0]=(int8_t)AE_MOVAD16_3(t0);
            z[m+1]=(int8_t)AE_MOVAD16_2(t0);
        }
	}
	if (M&1)
	{
		m=M&~1;
        ae_int64 a0;
        a0=AE_ZERO64();
        for (n=0; n<(N&~1); n+=2)
        {
            ae_int32x2 s0,t0;
            s0=AE_MOVDA32X2((uint8_t)y[    n+0],(uint8_t)y[    n+1]);
            t0=AE_MOVDA32X2((uint8_t)x[m*N+n+0],(uint8_t)x[m*N+n+1]);
            t0=AE_SEXT32(t0,7);
            s0=AE_SEXT32(s0,7);
            AE_MULAAD32_HH_LL(a0,t0,s0);
        }
        if (N&1)
        {
            ae_int32x2 s0,t0;
            s0=AE_MOVDA32X2((uint8_t)y[    n],(uint8_t)y[    n]);
            t0=AE_MOVDA32X2((uint8_t)x[m*N+n],(uint8_t)x[m*N+n]);
            t0=AE_SEXT32(t0,7);
            s0=AE_SEXT32(s0,7);
            AE_MULA32_HH(a0,t0,s0);
        }
		// Pack, round, store
        {
            ae_f16x4 t0;
            t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,a0,56-rsh),AE_TRUNCA32X2F64S(a0,a0,56-rsh));
            t0=AE_SRAI16(AE_ADD16S(t0,AE_MOVDA16(128)),8);
            z[m+0]=(int8_t)AE_MOVAD16_3(t0);
        }
	}
}
