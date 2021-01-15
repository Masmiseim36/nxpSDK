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
/* code optimimized for HiFi4 core */

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
void mtx_mpy8x8_fast (  void* pScr,
                     int8_t* restrict z,
               const int8_t* restrict x,
               const int8_t* restrict y,
               int M, int N, int P, int lsh )
{
    const ae_int16x4* restrict pX0;
    const ae_int16x4* restrict pX1;
    const ae_int16x4* restrict pX2;
    const ae_int16x4* restrict pX3;
    const ae_int16  * restrict pY;
          ae_int16  * restrict pZ;
    int m,n,p;
    ae_int64 rnd;
    int rsh=7-lsh;
    NASSERT_ALIGN(pScr,XCHAL_DATA_WIDTH);
    NASSERT_ALIGN(z,XCHAL_DATA_WIDTH);
    NASSERT_ALIGN(x,XCHAL_DATA_WIDTH);
    NASSERT_ALIGN(y,XCHAL_DATA_WIDTH);
    NASSERT(M%4==0);
    NASSERT(N%4==0);
    NASSERT(P%4==0);
    (void)pScr;
    if (P<=0 || M<=0) return;
    if (N<=0)    /* exceptional situation */
    {
        pZ=(ae_int16*)z;
        for (m=0; m<((M*P)>>3); m++) AE_S16X4_IP(AE_ZERO16(),castxcc(ae_int16x4,pZ),sizeof(ae_int16x4));
        return ;
    }
    rnd=AE_SLAA64S(1,rsh+15);
    for (p=0; p<(P>>1); p++,z+=2,y+=2)
    {
        pZ=(ae_int16*)z;
        for (m=0; m<M;m+=4)
        {
            ae_int64 A0,A1,A2,A3;
            ae_int64 B0,B1,B2,B3;

            A0=A1=A2=A3=
            B0=B1=B2=B3=rnd;
            pX0=(const ae_int16x4*)&x[m*N+0*N];
            pX1=(const ae_int16x4*)&x[m*N+1*N];
            pX2=(const ae_int16x4*)&x[m*N+2*N];
            pX3=(const ae_int16x4*)&x[m*N+3*N];
            pY=(const ae_int16*)y;
            __Pragma("loop_count min=1")
            for (n=0; n<(N>>2); n++)
            {
                ae_int16x4 y0,y1,y2,y3,t0,t1,x0,x1,x2,x3;
                AE_L16_XP(y0,pY,P);
                AE_L16_XP(y1,pY,P);
                AE_L16_XP(y2,pY,P);
                AE_L16_XP(y3,pY,P);
                
                t0=y0; AE_MOVT16X4(t0,y1,AE_MOVAB4(5));
                t1=y2; AE_MOVT16X4(t1,y3,AE_MOVAB4(5));
                y0=t0; AE_MOVT16X4(y0,t1,AE_MOVAB4(3));
                t1=AE_AND16(y0,AE_MOVDA16(0xff00));
                /* note: no non-saturating 16-bit left shift in HiFi, so we have to combine it from 64-bit shift and AND */
                t0=AE_AND16(AE_MOVINT16X4_FROMINT64(AE_SLLI64(AE_MOVINT64_FROMINT16X4(y0),8)),AE_MOVDA16(0xff00));

                AE_L8X4F_IP(x0,castxcc(int8_t,pX0),4);
                AE_L8X4F_IP(x1,castxcc(int8_t,pX1),4);
                AE_L8X4F_IP(x2,castxcc(int8_t,pX2),4);
                AE_L8X4F_IP(x3,castxcc(int8_t,pX3),4);

                AE_MULAAAAQ16(A0,x0,t0);
                AE_MULAAAAQ16(A1,x1,t0);
                AE_MULAAAAQ16(A2,x2,t0);
                AE_MULAAAAQ16(A3,x3,t0);
                AE_MULAAAAQ16(B0,x0,t1);
                AE_MULAAAAQ16(B1,x1,t1);
                AE_MULAAAAQ16(B2,x2,t1);
                AE_MULAAAAQ16(B3,x3,t1);
            }
            /* shift with saturation and pack results */
            {
                ae_int32x2 ab0,ab1,ab2,ab3;
                ab0=AE_TRUNCA32X2F64S(B0,A0,40-rsh);
                ab1=AE_TRUNCA32X2F64S(B1,A1,40-rsh);
                ab2=AE_TRUNCA32X2F64S(B2,A2,40-rsh);
                ab3=AE_TRUNCA32X2F64S(B3,A3,40-rsh);
                ab0=AE_SRLI32(ab0,24);
                ab1=AE_SRLI32(ab1,24);
                ab2=AE_SRLI32(ab2,24);
                ab3=AE_SRLI32(ab3,24);
                ab0=AE_OR32(ab0,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(ab0),24)));
                ab1=AE_OR32(ab1,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(ab1),24)));
                ab2=AE_OR32(ab2,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(ab2),24)));
                ab3=AE_OR32(ab3,AE_MOVINT32X2_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT32X2(ab3),24)));

                AE_S16_0_XP(AE_MOVINT16X4_FROMINT32X2(ab0),pZ,P);
                AE_S16_0_XP(AE_MOVINT16X4_FROMINT32X2(ab1),pZ,P);
                AE_S16_0_XP(AE_MOVINT16X4_FROMINT32X2(ab2),pZ,P);
                AE_S16_0_XP(AE_MOVINT16X4_FROMINT32X2(ab3),pZ,P);
            }
        }
    }
}

size_t mtx_mpy8x8_fast_getScratchSize ( int M, int N, int P)
{
    (void)M;(void)N;(void)P;
    return 0;
}
