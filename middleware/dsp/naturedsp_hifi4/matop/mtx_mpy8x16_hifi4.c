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
void mtx_mpy8x16(  void* pScr,
                     int16_t* restrict z,
               const  int8_t* restrict x,
               const int16_t* restrict y,
               int M, int N, int P, int lsh )
{
    int m,n,p;
    NASSERT(lsh >= -15 && lsh <= 15);
    NASSERT_ALIGN(pScr,XCHAL_DATA_WIDTH);
    (void)pScr;
    if (P<=0 || M<=0) return;
    if (N<=0)    /* exceptional situation */
    {
        for (m=0; m<M; m++) 
        for (p=0; p<P; p++)
                z[m*P+p]=0;
        return ;
    }
    int rsh=7-lsh;

    /*----------------------------------------*/
    /* main processing loop: 4x unroll over P */
    /* special case N==1 is required to set up*/
    /* min loop count for inner loops         */
    /*----------------------------------------*/
    if (N==1)
    {
        if (M>1) for (p=0; p<(P&~3); p+=4)
        {
            __Pragma("loop_count min=1")
            for (m=0; m<(M&~1);m+=2)
            {
                ae_int64 a0,a1,a2,a3;
                ae_int64 b0,b1,b2,b3;
                ae_int32x2 t0,t1,s0;
                t0=AE_MOVDA32X2((uint16_t)y[p+0],(uint16_t)y[p+1]);
                t1=AE_MOVDA32X2((uint16_t)y[p+2],(uint16_t)y[p+3]);
                s0=AE_MOVDA32X2((uint8_t)x[m*N]  ,(uint8_t)x[m*N+N]);
                t0=AE_SEXT32(t0,15);
                t1=AE_SEXT32(t1,15);
                s0=AE_SEXT32(s0,7);
                a0=AE_MUL32_HH(s0,t0);
                b0=AE_MUL32_HL(s0,t0);
                a1=AE_MUL32_LH(s0,t0);
                b1=AE_MUL32_LL(s0,t0);
                a2=AE_MUL32_HH(s0,t1);
                b2=AE_MUL32_HL(s0,t1);
                a3=AE_MUL32_LH(s0,t1);
                b3=AE_MUL32_LL(s0,t1);
                // pack and round
                {
                    ae_f16x4 t0,t1;
                    t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,b0,48-rsh),AE_TRUNCA32X2F64S(a1,b1,48-rsh));
                    t1=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a2,b2,48-rsh),AE_TRUNCA32X2F64S(a3,b3,48-rsh));

                    z[m*P+p  ]    =(int16_t)AE_MOVAD16_3(t0);
                    z[m*P+p+1]    =(int16_t)AE_MOVAD16_2(t0);
                    z[m*P+p  +1*P]=(int16_t)AE_MOVAD16_1(t0);
                    z[m*P+p+1+1*P]=(int16_t)AE_MOVAD16_0(t0);
                    z[m*P+p+2    ]=(int16_t)AE_MOVAD16_3(t1);
                    z[m*P+p+3    ]=(int16_t)AE_MOVAD16_2(t1);
                    z[m*P+p+2+1*P]=(int16_t)AE_MOVAD16_1(t1);
                    z[m*P+p+3+1*P]=(int16_t)AE_MOVAD16_0(t1);
                }
            }
        }
        if (M&1)
        {
            for (p=0; p<(P&~3); p+=4)
            {
                ae_int64 a0,a2;
                ae_int64 b0,b2;
                ae_int32x2 s0,t0,t1;
                m=M&~1;
                t0=AE_MOVDA32X2((uint16_t)y[p+0],(uint16_t)y[p+1]);
                t1=AE_MOVDA32X2((uint16_t)y[p+2],(uint16_t)y[p+3]);
                s0=AE_MOVDA32((uint8_t)x[m*N]);
                t0=AE_SEXT32(t0,15);
                t1=AE_SEXT32(t1,15);
                s0=AE_SEXT32(s0,7);
                a0=AE_MUL32_HH(s0,t0);
                b0=AE_MUL32_HL(s0,t0);
                a2=AE_MUL32_HH(s0,t1);
                b2=AE_MUL32_HL(s0,t1);
                // pack, round and store
                {
                    ae_f16x4 t0;
                    t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,b0,48-rsh),AE_TRUNCA32X2F64S(a2,b2,48-rsh));
                    z[m*P+p  ]=(int16_t)AE_MOVAD16_3(t0);
                    z[m*P+p+1]=(int16_t)AE_MOVAD16_2(t0);
                    z[m*P+p+2]=(int16_t)AE_MOVAD16_1(t0);
                    z[m*P+p+3]=(int16_t)AE_MOVAD16_0(t0);
                }
            }
        }
    }
    else // N>1: the most important code affecting to the total cycles
    {
		/*----------------------------------------------------*/
		/* Use scratch to store y-components in optimal order */
		/* Every quad of the row (i.e. (y00,y01,y02,y03)) is  */
		/*  taken as a single element of the initial matrix   */
		/* The last row (if N is odd) is omitted              */
		/* Then the matrix of quads is transposed and written */
		/*  into scratch, row by row                          */
		/* The last row (if N is odd) is copied to scratch    */
		/* The rest elements (the last 1...3 columns, if P is */
		/*  not multiple of 4) are not handled at all         */
		/*----------------------------------------------------*/
		ae_int32x2 * pS;
		pS = (ae_int32x2 *)pScr;
		const ae_int16x4 * pY0;
		const ae_int16x4 * pY1;
		ae_valign yv0,yv1;
		for (p=0; p<(P&~3); p+=4)
		{
			pY0 = (const ae_int16x4 *)(y+p);
			pY1 = (const ae_int16x4 *)(y+p+P);
			for (n=0; n<(N&~1); n+=2)
			{
				ae_int16x4 y0,y1;
				ae_int32x2 q0,q1,q2,q3;
				ae_int32x2 t0,t1,t2,t3;

				// Load (p/4)-th quads of (n)-th and (n+1)-th rows 
				yv0 = AE_LA64_PP(pY0);
				yv1 = AE_LA64_PP(pY1);
				AE_LA16X4_IP(y0,yv0,pY0);
				AE_LA16X4_IP(y1,yv1,pY1);
				pY0=(const ae_int16x4 *)XT_ADDX4(P-2,(intptr_t)pY0);
				pY1=(const ae_int16x4 *)XT_ADDX4(P-2,(intptr_t)pY1);

				q0=AE_SEXT32(          AE_MOVINT32X2_FROMINT16X4(y0)    ,15);
				q1=AE_SEXT32(          AE_MOVINT32X2_FROMINT16X4(y1)    ,15);
				q2=AE_SEXT32(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(y0),16),15);
				q3=AE_SEXT32(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(y1),16),15);
				t0=AE_SEL32_HH(q2,q3);
				t2=AE_SEL32_HH(q0,q1);
				t1=AE_SEL32_LL(q2,q3);
				t3=AE_SEL32_LL(q0,q1);

				// Store these quads one after another
				AE_S32X2_IP(t0,pS,2*sizeof(int32_t));
				AE_S32X2_IP(t2,pS,2*sizeof(int32_t));
				AE_S32X2_IP(t1,pS,2*sizeof(int32_t));
				AE_S32X2_IP(t3,pS,2*sizeof(int32_t));
			}
			if (N&1)
			{
				ae_int16x4 y0;
				ae_int32x2 q0,q2;
				ae_int32x2 t0,t1;

				// Load (p/4)-th quad of the last row (if N is odd)
				yv0 = AE_LA64_PP(pY0);
				AE_LA16X4_IP(y0,yv0,pY0);
				q0=AE_SEXT32(          AE_MOVINT32X2_FROMINT16X4(y0)    ,15);
				q2=AE_SEXT32(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(y0),16),15);
				t0=AE_SEL32_HH(q2,q0);
				t1=AE_SEL32_LL(q2,q0);
				// Store this quad one by one
				AE_S32X2_IP(t0,pS,2*sizeof(int32_t));
				AE_S32X2_IP(t1,pS,2*sizeof(int32_t));
			}
		}
		
		/*--------------------------------------------*/
		/* Aggregate x by couples, y by quads         */
		/* y is read from scratch in 'direct' order,  */
		/*  i.e. so that corresponding y components   */
		/*  are loaded simultaniously                 */
		/* If P is not multiple of 4, the last blocks */
		/*  of corresponding y components are loaded  */
		/*  without scratch involved                  */
		/*--------------------------------------------*/
        for (p=0; p<(P&~3); p+=4)
        {
			      ae_int16x4 * pZ0;
			      ae_int16x4 * pZ1;
			ae_valign zv0, zv1;
			zv0=zv1=AE_ZALIGN64();
            for (m=0; m<(M&~1);m+=2)
            {
                ae_int64 a0,a1,a2,a3;
                ae_int64 b0,b1,b2,b3;
                a0=b0=a1=b1=a2=b2=a3=b3=AE_ZERO64();
				pS = (ae_int32x2 *)((intptr_t)pScr+p*N*sizeof(int32_t));
				__Pragma("loop_count min=1")
                for (n=0; n<(N&~1); n+=2)
                {
                    ae_int32x2 t0,t1,s0;
                    ae_int32x2 t2,t3,s1;

                    s0=AE_MOVDA32X2((uint8_t)x[m*N+n]  ,(uint8_t)x[m*N+n+1]);
                    s1=AE_MOVDA32X2((uint8_t)x[m*N+n+N],(uint8_t)x[m*N+n+N+1]);
                    s0=AE_SEXT32(s0,7);
                    s1=AE_SEXT32(s1,7);
					// Load (p/4)-th quads of (n)-th and (n+1)-th rows
					AE_L32X2_IP(t0,pS,2*sizeof(int32_t));
					AE_L32X2_IP(t2,pS,2*sizeof(int32_t));
					AE_L32X2_IP(t1,pS,2*sizeof(int32_t));
					AE_L32X2_IP(t3,pS,2*sizeof(int32_t));

                    AE_MULAAD32_HH_LL(a0,s0,t0);
                    AE_MULAAD32_HH_LL(b0,s0,t2);
                    AE_MULAAD32_HH_LL(a1,s1,t0);
                    AE_MULAAD32_HH_LL(b1,s1,t2);
                    AE_MULAAD32_HH_LL(a2,s0,t1);
                    AE_MULAAD32_HH_LL(b2,s0,t3);
                    AE_MULAAD32_HH_LL(a3,s1,t1);
                    AE_MULAAD32_HH_LL(b3,s1,t3);
                }
                if (N&1)
                {
                    ae_int32x2 t0,t1,s0;
                    s0=AE_MOVDA32X2((uint8_t)x[m*N+n],(uint8_t)x[m*N+n+N]);
                    s0=AE_SEXT32(s0,7);
					// Load (p/4)-th quad of the last row (if N is odd)
					AE_L32X2_IP(t0,pS,2*sizeof(int32_t));
					AE_L32X2_IP(t1,pS,2*sizeof(int32_t));
                    AE_MULA32_HH(a0,s0,t0);
                    AE_MULA32_HL(b0,s0,t0);
                    AE_MULA32_LH(a1,s0,t0);
                    AE_MULA32_LL(b1,s0,t0);
                    AE_MULA32_HH(a2,s0,t1);
                    AE_MULA32_HL(b2,s0,t1);
                    AE_MULA32_LH(a3,s0,t1);
                    AE_MULA32_LL(b3,s0,t1);
                }
                // pack and round
                {
                    ae_f16x4 t0,t1;
                    t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,b0,48-rsh),AE_TRUNCA32X2F64S(a2,b2,48-rsh));
                    t1=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a1,b1,48-rsh),AE_TRUNCA32X2F64S(a3,b3,48-rsh));

					zv0=zv1=AE_ZALIGN64();
					pZ0 = (ae_int16x4 *)(&z[m*P+p]);
					pZ1 = (ae_int16x4 *)(&z[m*P+p+P]);
					AE_SA16X4_IP(t0,zv0,pZ0);
					AE_SA16X4_IP(t1,zv1,pZ1);
					AE_SA64POS_FP(zv0,pZ0);
					AE_SA64POS_FP(zv1,pZ1);
                }
            }
            if (M&1)
            {
                ae_int64 a0,a2;
                ae_int64 b0,b2;
                a0=b0=a2=b2=AE_ZERO64();
				pS = (ae_int32x2 *)((intptr_t)pScr+p*N*sizeof(int32_t));
                __Pragma("loop_count min=1")
                for (n=0; n<(N&~1); n+=2)
                {
                    ae_int32x2 s0,t0,t1,t2,t3;

					s0=AE_MOVDA32X2((uint8_t)x[m*N+n]  ,(uint8_t)x[m*N+n+1]);
                    s0=AE_SEXT32(s0,7);

					// Load (p/4)-th quads of (n)-th and (n+1)-th rows
					AE_L32X2_IP(t0,pS,2*sizeof(int32_t));
					AE_L32X2_IP(t1,pS,2*sizeof(int32_t));
					AE_L32X2_IP(t2,pS,2*sizeof(int32_t));
					AE_L32X2_IP(t3,pS,2*sizeof(int32_t));

                    AE_MULAAD32_HH_LL(a0,s0,t0);
                    AE_MULAAD32_HH_LL(b0,s0,t1);
                    AE_MULAAD32_HH_LL(a2,s0,t2);
                    AE_MULAAD32_HH_LL(b2,s0,t3);
                }
                if (N&1)
                {
                    ae_int32x2 s0,t0,t1;
                    s0=AE_MOVDA32((uint8_t)x[m*N+n]);
                    s0=AE_SEXT32(s0,7);
					// Load (p/4)-th quad of the last row (if N is odd)
					AE_L32X2_IP(t0,pS,2*sizeof(int32_t));
					AE_L32X2_IP(t1,pS,2*sizeof(int32_t));
                    AE_MULA32_HH(a0,s0,t0);
                    AE_MULA32_HL(b0,s0,t0);
                    AE_MULA32_HH(a2,s0,t1);
                    AE_MULA32_HL(b2,s0,t1);
                }
                // pack, round and store
                {
                    ae_f16x4 t0;
                    t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,b0,48-rsh),AE_TRUNCA32X2F64S(a2,b2,48-rsh));

					zv0=AE_ZALIGN64();
					pZ0 = (ae_int16x4 *)(&z[m*P+p]);
					AE_SA16X4_IP(t0,zv0,pZ0);
					AE_SA64POS_FP(zv0,pZ0);
                }
            }
        }
    }
    /*---------------------------------------------------------*/
    /* remaining 2 iterations over p                           */
    /*---------------------------------------------------------*/
    if(P&2)
    {
        for (m=0; m<(M&~3);m+=4)
        {
            ae_int64 a0,a1,a2,a3;
            ae_int64 b0,b1,b2,b3;
            a0=b0=a1=b1=a2=b2=a3=b3=AE_ZERO64();
            for (n=0; n<(N&~1); n+=2)
            {
                ae_int32x2 s0,s1,t0,t1,t2,t3;
                s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0  ],(uint16_t)y[n*P+p+0+P]);
                s1=AE_MOVDA32X2((uint16_t)y[n*P+p+1  ],(uint16_t)y[n*P+p+1+P]);
                t0=AE_MOVDA32X2((uint8_t)x[m*N+n    ],(uint8_t)x[m*N+n    +1]);
                t1=AE_MOVDA32X2((uint8_t)x[m*N+n+1*N],(uint8_t)x[m*N+n+1*N+1]);
                t2=AE_MOVDA32X2((uint8_t)x[m*N+n+2*N],(uint8_t)x[m*N+n+2*N+1]);
                t3=AE_MOVDA32X2((uint8_t)x[m*N+n+3*N],(uint8_t)x[m*N+n+3*N+1]);
                t0=AE_SEXT32(t0,7);
                t1=AE_SEXT32(t1,7);
                t2=AE_SEXT32(t2,7);
                t3=AE_SEXT32(t3,7);
                s0=AE_SEXT32(s0,15);
                s1=AE_SEXT32(s1,15);
                AE_MULAAD32_HH_LL(a0,s0,t0);
                AE_MULAAD32_HH_LL(a1,s0,t1);
                AE_MULAAD32_HH_LL(a2,s0,t2);
                AE_MULAAD32_HH_LL(a3,s0,t3);
                AE_MULAAD32_HH_LL(b0,s1,t0);
                AE_MULAAD32_HH_LL(b1,s1,t1);
                AE_MULAAD32_HH_LL(b2,s1,t2);
                AE_MULAAD32_HH_LL(b3,s1,t3);
            }
            if (N&1)
            {
                ae_int32x2 s0,t0,t1;
                s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0  ],(uint16_t)y[n*P+p+1]);
                t0=AE_MOVDA32X2((uint8_t)x[m*N+n    ],(uint8_t)x[m*N+n+1*N]);
                t1=AE_MOVDA32X2((uint8_t)x[m*N+n+2*N],(uint8_t)x[m*N+n+3*N]);
                t0=AE_SEXT32(t0,7);
                t1=AE_SEXT32(t1,7);
                s0=AE_SEXT32(s0,15);

                 AE_MULA32_HH(a0,t0,s0);
                 AE_MULA32_HL(b0,t0,s0);
                 AE_MULA32_LH(a1,t0,s0);
                 AE_MULA32_LL(b1,t0,s0);
                 AE_MULA32_HH(a2,t1,s0);
                 AE_MULA32_HL(b2,t1,s0);
                 AE_MULA32_LH(a3,t1,s0);
                 AE_MULA32_LL(b3,t1,s0);
            }
            // pack, round and store
            {
                ae_f16x4 t0,t1;
                t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,b0,48-rsh),AE_TRUNCA32X2F64S(a1,b1,48-rsh));
                t1=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a2,b2,48-rsh),AE_TRUNCA32X2F64S(a3,b3,48-rsh));
                z[m*P+p      ]=(int16_t)AE_MOVAD16_3(t0);
                z[m*P+p+1    ]=(int16_t)AE_MOVAD16_2(t0);
                z[m*P+p  +1*P]=(int16_t)AE_MOVAD16_1(t0);
                z[m*P+p+1+1*P]=(int16_t)AE_MOVAD16_0(t0);
                z[m*P+p  +2*P]=(int16_t)AE_MOVAD16_3(t1);
                z[m*P+p+1+2*P]=(int16_t)AE_MOVAD16_2(t1);
                z[m*P+p  +3*P]=(int16_t)AE_MOVAD16_1(t1);
                z[m*P+p+1+3*P]=(int16_t)AE_MOVAD16_0(t1);
            }
        }
        switch(M&3)
        {
        case 0: break;
        case 3:
            {
                ae_int64 a0,a1,a2;
                ae_int64 b0,b1,b2;
                a0=b0=a1=b1=a2=b2=AE_ZERO64();
                for (n=0; n<(N&~1); n+=2)
                {
                    ae_int32x2 t0,t1,t2,s0,s1;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0],(uint16_t)y[n*P+p+0+P]);
                    s1=AE_MOVDA32X2((uint16_t)y[n*P+p+1],(uint16_t)y[n*P+p+1+P]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n    ],(uint8_t)x[m*N+n    +1]);
                    t1=AE_MOVDA32X2((uint8_t)x[m*N+n+1*N],(uint8_t)x[m*N+n+1*N+1]);
                    t2=AE_MOVDA32X2((uint8_t)x[m*N+n+2*N],(uint8_t)x[m*N+n+2*N+1]);
                    t0=AE_SEXT32(t0,7);
                    t1=AE_SEXT32(t1,7);
                    t2=AE_SEXT32(t2,7);
                    s0=AE_SEXT32(s0,15);
                    s1=AE_SEXT32(s1,15);
                    AE_MULAAD32_HH_LL(a0,t0,s0);
                    AE_MULAAD32_HH_LL(a1,t1,s0);
                    AE_MULAAD32_HH_LL(a2,t2,s0);
                    AE_MULAAD32_HH_LL(b0,t0,s1);
                    AE_MULAAD32_HH_LL(b1,t1,s1);
                    AE_MULAAD32_HH_LL(b2,t2,s1);
                }
                if (N&1)
                {
                    ae_int32x2 t0,t1,s0;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0],(uint16_t)y[n*P+p+1]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n    ],(uint8_t)x[m*N+n+1*N]);
                    t1=AE_MOVDA32X2((uint8_t)x[m*N+n+2*N],(uint8_t)x[m*N+n+2*N]);
                    t0=AE_SEXT32(t0,7);
                    t1=AE_SEXT32(t1,7);
                    s0=AE_SEXT32(s0,15);
                    AE_MULA32_HH(a0,t0,s0);
                    AE_MULA32_HL(b0,t0,s0);
                    AE_MULA32_LH(a1,t0,s0);
                    AE_MULA32_LL(b1,t0,s0);
                    AE_MULA32_HH(a2,t1,s0);
                    AE_MULA32_HL(b2,t1,s0);
                }
                {
                    ae_f16x4 t0,t1;
                    t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,b0,48-rsh),AE_TRUNCA32X2F64S(a1,b1,48-rsh));
                    t1=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a2,b2,48-rsh),AE_TRUNCA32X2F64S(a2,b2,48-rsh));
                    z[m*P+p      ]=(int16_t)AE_MOVAD16_3(t0);
                    z[m*P+p+1    ]=(int16_t)AE_MOVAD16_2(t0);
                    z[m*P+p  +1*P]=(int16_t)AE_MOVAD16_1(t0);
                    z[m*P+p+1+1*P]=(int16_t)AE_MOVAD16_0(t0);
                    z[m*P+p  +2*P]=(int16_t)AE_MOVAD16_3(t1);
                    z[m*P+p+1+2*P]=(int16_t)AE_MOVAD16_2(t1);
                }
            }
            break;
        case 2:
            {
                ae_int64 a0,a1;
                ae_int64 b0,b1;
                a0=b0=a1=b1=AE_ZERO64();
                for (n=0; n<(N&~1); n+=2)
                {
                    ae_int32x2 t0,t1,s0,s1;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0],(uint16_t)y[n*P+p+0+P]);
                    s1=AE_MOVDA32X2((uint16_t)y[n*P+p+1],(uint16_t)y[n*P+p+1+P]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n    ],(uint8_t)x[m*N+n    +1]);
                    t1=AE_MOVDA32X2((uint8_t)x[m*N+n+1*N],(uint8_t)x[m*N+n+1*N+1]);
                    t0=AE_SEXT32(t0,7);
                    t1=AE_SEXT32(t1,7);
                    s0=AE_SEXT32(s0,15);
                    s1=AE_SEXT32(s1,15);
                    AE_MULAAD32_HH_LL(a0,t0,s0);
                    AE_MULAAD32_HH_LL(a1,t1,s0);
                    AE_MULAAD32_HH_LL(b0,t0,s1);
                    AE_MULAAD32_HH_LL(b1,t1,s1);
                }
                if (N&1)
                {
                    ae_int32x2 t0,s0;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0],(uint16_t)y[n*P+p+1]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n    ],(uint8_t)x[m*N+n+1*N]);
                    t0=AE_SEXT32(t0,7);
                    s0=AE_SEXT32(s0,15);
                    AE_MULA32_HH(a0,t0,s0);
                    AE_MULA32_HL(b0,t0,s0);
                    AE_MULA32_LH(a1,t0,s0);
                    AE_MULA32_LL(b1,t0,s0);
                }
                {
                    ae_f16x4 t0;
                    t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,b0,48-rsh),AE_TRUNCA32X2F64S(a1,b1,48-rsh));
                    z[m*P+p      ]=(int16_t)AE_MOVAD16_3(t0);
                    z[m*P+p+1    ]=(int16_t)AE_MOVAD16_2(t0);
                    z[m*P+p  +1*P]=(int16_t)AE_MOVAD16_1(t0);
                    z[m*P+p+1+1*P]=(int16_t)AE_MOVAD16_0(t0);
                }
            }
            break;
        case 1:
            {
                ae_int64 a0;
                ae_int64 b0;
                a0=b0=AE_ZERO64();
                for (n=0; n<(N&~1); n+=2)
                {
                    ae_int32x2 t0,s0,s1;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0],(uint16_t)y[n*P+p+0+P]);
                    s1=AE_MOVDA32X2((uint16_t)y[n*P+p+1],(uint16_t)y[n*P+p+1+P]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n  ],(uint8_t)x[m*N+n  +1]);
                    t0=AE_SEXT32(t0,7);
                    s0=AE_SEXT32(s0,15);
                    s1=AE_SEXT32(s1,15);
                    AE_MULAAD32_HH_LL(a0,t0,s0);
                    AE_MULAAD32_HH_LL(b0,t0,s1);
                }
                if (N&1)
                {
                    ae_int32x2 t0,s0;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0],(uint16_t)y[n*P+p+1]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n  ],(uint8_t)x[m*N+n  ]);
                    t0=AE_SEXT32(t0,7); 
                    s0=AE_SEXT32(s0,15);
                    AE_MULA32_HH(a0,t0,s0);
                    AE_MULA32_HL(b0,t0,s0);
                }
                {
                    ae_f16x4 t0;
                    t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,b0,48-rsh),AE_TRUNCA32X2F64S(a0,b0,48-rsh));
                    z[m*P+p      ]=(int16_t)AE_MOVAD16_3(t0);
                    z[m*P+p+1    ]=(int16_t)AE_MOVAD16_2(t0);
                }
            }
            break;
        }
        p+=2;
    }
    /*---------------------------------------------------------*/
    /* remaining last iteration over p                         */
    /*---------------------------------------------------------*/
    if (P&1)
    {
        for (m=0; m<(M&~3);m+=4)
        {
            ae_int64 a0,a1,a2,a3;
            a0=a1=a2=a3=AE_ZERO64();
            for (n=0; n<(N&~1); n+=2)
            {
                ae_int32x2 s0,t0,t1,t2,t3;
                s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0  ],(uint16_t)y[n*P+p+0+P]);
                t0=AE_MOVDA32X2((uint8_t)x[m*N+n+0*N],(uint8_t)x[m*N+n+0*N+1]);
                t1=AE_MOVDA32X2((uint8_t)x[m*N+n+1*N],(uint8_t)x[m*N+n+1*N+1]);
                t2=AE_MOVDA32X2((uint8_t)x[m*N+n+2*N],(uint8_t)x[m*N+n+2*N+1]);
                t3=AE_MOVDA32X2((uint8_t)x[m*N+n+3*N],(uint8_t)x[m*N+n+3*N+1]);
                t0=AE_SEXT32(t0,7);
                t1=AE_SEXT32(t1,7);
                t2=AE_SEXT32(t2,7);
                t3=AE_SEXT32(t3,7);
                s0=AE_SEXT32(s0,15);
                AE_MULAAD32_HH_LL(a0,t0,s0);
                AE_MULAAD32_HH_LL(a1,t1,s0);
                AE_MULAAD32_HH_LL(a2,t2,s0);
                AE_MULAAD32_HH_LL(a3,t3,s0);
            }
            if(N&1)
            {
                ae_int32x2 s0,t0,t1;
                s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0  ],(uint16_t)y[n*P+p+0  ]);
                t0=AE_MOVDA32X2((uint8_t)x[m*N+n+0*N],(uint8_t)x[m*N+n+1*N]);
                t1=AE_MOVDA32X2((uint8_t)x[m*N+n+2*N],(uint8_t)x[m*N+n+3*N]);
                t0=AE_SEXT32(t0,7);
                t1=AE_SEXT32(t1,7);
                s0=AE_SEXT32(s0,15);
                AE_MULA32_HH(a0,t0,s0);
                AE_MULA32_LH(a1,t0,s0);
                AE_MULA32_HH(a2,t1,s0);
                AE_MULA32_LH(a3,t1,s0);
            }
            {
                ae_f16x4 t0;
                t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,a1,48-rsh),AE_TRUNCA32X2F64S(a2,a3,48-rsh));
                z[m*P+p+0*P]=(int16_t)AE_MOVAD16_3(t0);
                z[m*P+p+1*P]=(int16_t)AE_MOVAD16_2(t0);
                z[m*P+p+2*P]=(int16_t)AE_MOVAD16_1(t0);
                z[m*P+p+3*P]=(int16_t)AE_MOVAD16_0(t0);
            }
        }
        switch(M&3)
        {
        case 0: break;
        case 3:
            {
                ae_int64 a0,a1,a2;
                a0=a1=a2=AE_ZERO64();
                for (n=0; n<(N&~1); n+=2)
                {
                    ae_int32x2 s0,t0,t1,t2;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0  ],(uint16_t)y[n*P+p+0+P]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n+0*N],(uint8_t)x[m*N+n+0*N+1]);
                    t1=AE_MOVDA32X2((uint8_t)x[m*N+n+1*N],(uint8_t)x[m*N+n+1*N+1]);
                    t2=AE_MOVDA32X2((uint8_t)x[m*N+n+2*N],(uint8_t)x[m*N+n+2*N+1]);
                    t0=AE_SEXT32(t0,7);
                    t1=AE_SEXT32(t1,7);
                    t2=AE_SEXT32(t2,7);
                    s0=AE_SEXT32(s0,15);
                    AE_MULAAD32_HH_LL(a0,t0,s0);
                    AE_MULAAD32_HH_LL(a1,t1,s0);
                    AE_MULAAD32_HH_LL(a2,t2,s0);
                }
                if(N&1)
                {
                    ae_int32x2 s0,t0,t1;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0  ],(uint16_t)y[n*P+p+0  ]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n+0*N],(uint8_t)x[m*N+n+1*N]);
                    t1=AE_MOVDA32X2((uint8_t)x[m*N+n+2*N],(uint8_t)x[m*N+n+2*N]);
                    t0=AE_SEXT32(t0,7);
                    t1=AE_SEXT32(t1,7);
                    s0=AE_SEXT32(s0,15);
                    AE_MULA32_HH(a0,t0,s0);
                    AE_MULA32_LH(a1,t0,s0);
                    AE_MULA32_HH(a2,t1,s0);
                }
                {
                    ae_f16x4 t0;
                    t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,a1,48-rsh),AE_TRUNCA32X2F64S(a2,a2,48-rsh));
                    z[m*P+p+0*P]=(int16_t)AE_MOVAD16_3(t0);
                    z[m*P+p+1*P]=(int16_t)AE_MOVAD16_2(t0);
                    z[m*P+p+2*P]=(int16_t)AE_MOVAD16_1(t0);
                }
            }
            break;
        case 2:
            {
                ae_int64 a0,a1;
                a0=a1=AE_ZERO64();
                for (n=0; n<(N&~1); n+=2)
                {
                    ae_int32x2 s0,t0,t1;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0  ],(uint16_t)y[n*P+p+0+P]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n+0*N],(uint8_t)x[m*N+n+0*N+1]);
                    t1=AE_MOVDA32X2((uint8_t)x[m*N+n+1*N],(uint8_t)x[m*N+n+1*N+1]);
                    t0=AE_SEXT32(t0,7);
                    t1=AE_SEXT32(t1,7);
                    s0=AE_SEXT32(s0,15);
                    AE_MULAAD32_HH_LL(a0,t0,s0);
                    AE_MULAAD32_HH_LL(a1,t1,s0);
                }
                if(N&1)
                {
                    ae_int32x2 s0,t0;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0  ],(uint16_t)y[n*P+p+0  ]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n+0*N],(uint8_t)x[m*N+n+1*N]);
                    t0=AE_SEXT32(t0,7);
                    s0=AE_SEXT32(s0,15);
                    AE_MULA32_HH(a0,t0,s0);
                    AE_MULA32_LH(a1,t0,s0);
                }
                {
                    ae_f16x4 t0;
                    t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,a1,48-rsh),AE_TRUNCA32X2F64S(a0,a1,48-rsh));
                    z[m*P+p+0*P]=(int16_t)AE_MOVAD16_3(t0);
                    z[m*P+p+1*P]=(int16_t)AE_MOVAD16_2(t0);
                }
            }
            break;
        case 1:
            {
                ae_int64 a0;
                a0=AE_ZERO64();
                for (n=0; n<(N&~1); n+=2)
                {
                    ae_int32x2 s0,t0;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0  ],(uint16_t)y[n*P+p+0+P]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n+0*N],(uint8_t)x[m*N+n+0*N+1]);
                    t0=AE_SEXT32(t0,7);
                    s0=AE_SEXT32(s0,15);
                    AE_MULAAD32_HH_LL(a0,t0,s0);
                }
                if(N&1)
                {
                    ae_int32x2 s0,t0;
                    s0=AE_MOVDA32X2((uint16_t)y[n*P+p+0  ],(uint16_t)y[n*P+p+0  ]);
                    t0=AE_MOVDA32X2((uint8_t)x[m*N+n+0*N],(uint8_t)x[m*N+n+0*N]);
                    t0=AE_SEXT32(t0,7);
                    s0=AE_SEXT32(s0,15);
                    AE_MULA32_HH(a0,t0,s0);
                }
                {
                    ae_f16x4 t0;
                    t0=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(a0,a0,48-rsh),AE_TRUNCA32X2F64S(a0,a0,48-rsh));
                    z[m*P+p+0*P]=(int16_t)AE_MOVAD16_3(t0);
                }
            }
            break;
        }
    }
}

size_t mtx_mpy8x16_getScratchSize(int M, int N, int P)
{
    return (M>0 && N>0 && P>0) ? ((P&~3)*(N)*sizeof(int32_t)) : 0;
}
