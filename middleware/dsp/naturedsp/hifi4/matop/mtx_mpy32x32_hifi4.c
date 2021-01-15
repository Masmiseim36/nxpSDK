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

#include "NatureDSP_Signal_matop.h"
#include "NatureDSP_types.h"
#include "common.h"

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
void mtx_mpy32x32 (  void* pScr,
                     int32_t* restrict z,
               const int32_t* restrict x,
               const int32_t* restrict y,
               int M, int N, int P, int lsh )
{
    int m, n, p;
    int K;
    int32_t * restrict Y0;
    int32_t * restrict Y1;
    int32_t * restrict Y2;
    int32_t * restrict Y3;
    const ae_int32x2 * restrict pY;
    ae_int32x2 * restrict pY0;
    ae_int32x2 * restrict pY1;
    ae_int32x2 * restrict pY2;
    ae_int32x2 * restrict pY3;
    const ae_int32x2 * restrict px;
    const ae_int32x2 * restrict pX;
    const ae_int32x2 * restrict pX1;
    const ae_int32x2 * restrict pX2;
    const ae_int32x2 * restrict pX3;
          ae_int32   * restrict pZ;
    ae_valign aX,aX1,aX2,aX3;

    NASSERT(lsh >= -31 && lsh <= 31);
    /* exceptional situations */
    if ((M <= 0) || (P <= 0)) return;
    if (N <= 0)
    {   
        int m;
        for (m = 0; m < M * P; m++) z[m] = 0;
        return;
    }
    K = ((N + 1)&(~1));
    Y0 = (int32_t *)pScr;
    Y1 = Y0 + K;
    Y2 = Y1 + K;
    Y3 = Y2 + K;
    /*
        main loop unrolled 4x over P 
    */
    for (p = 0; p < (P>>2); p++,z+=4,y+=4)
    {
        NASSERT_ALIGN8(Y0);
        NASSERT_ALIGN8(Y1);
        pZ=(ae_int32*)(z);
        /* copy 4 input columns to the srcatch memory with zero padding */
        pY0=(ae_int32x2 *)Y0;
        pY1=(ae_int32x2 *)Y1;
        pY2=(ae_int32x2 *)Y2;
        pY3=(ae_int32x2 *)Y3;
        for (n = 0; n < (N>>1); n++)
        {
            ae_int32x2 y0,y1,y2,y3;
            ae_valign aY;
            pY=(const ae_int32x2 *)(y+2*n*P );
            aY=AE_LA64_PP(pY);
            AE_LA32X2_IP(y0,aY,pY);
            AE_LA32X2_IP(y1,aY,pY);
            pY=(const ae_int32x2 *)(y+2*n*P+P);
            aY=AE_LA64_PP(pY);
            AE_LA32X2_IP(y2,aY,pY);
            AE_LA32X2_IP(y3,aY,pY);
            AE_S32X2_IP(AE_SEL32_HH(y0,y2),pY0,sizeof(ae_int32x2));
            AE_S32X2_IP(AE_SEL32_LL(y0,y2),pY1,sizeof(ae_int32x2));
            AE_S32X2_IP(AE_SEL32_HH(y1,y3),pY2,sizeof(ae_int32x2));
            AE_S32X2_IP(AE_SEL32_LL(y1,y3),pY3,sizeof(ae_int32x2));
        }
        if (N & 1)
        {
            ae_int32x2 y0,y1;
            ae_valign aY;
            pY=(const ae_int32x2 *)(y+2*n*P );
            aY=AE_LA64_PP(pY);
            AE_LA32X2_IP(y0,aY,pY);
            AE_LA32X2_IP(y1,aY,pY);
            AE_S32X2_IP(AE_SEL32_HH(y0,AE_ZERO32()),pY0,sizeof(ae_int32x2));
            AE_S32X2_IP(AE_SEL32_LL(y0,AE_ZERO32()),pY1,sizeof(ae_int32x2));
            AE_S32X2_IP(AE_SEL32_HH(y1,AE_ZERO32()),pY2,sizeof(ae_int32x2));
            AE_S32X2_IP(AE_SEL32_LL(y1,AE_ZERO32()),pY3,sizeof(ae_int32x2));
        }
        /* compute 4*M outputs  */
        px =(ae_int32x2 *)x;
        for (m = 0; m < (M>>1); m++)
        {
            ae_int64 q0,q1,q2,q3;
            ae_int64 q4,q5,q6,q7;
            ae_int32x2 z0,z1,z2,z3;
            pY0=(ae_int32x2 *)Y0;
            pY1=(ae_int32x2 *)Y1;
            pY2=(ae_int32x2 *)Y2;
            pY3=(ae_int32x2 *)Y3;
            pX =  px;
            pX1 = (const ae_int32x2 *)XT_ADDX4(N, (uintptr_t)pX );
            px  = (const ae_int32x2 *)XT_ADDX8(N, (uintptr_t)px );
            aX = AE_LA64_PP(pX);
            aX1= AE_LA64_PP(pX1);
            q0=q1=q2=q3=AE_ZERO64();
            q4=q5=q6=q7=AE_ZERO64();
            __Pragma("loop_count min=1")
            for (n = 0; n < (K>>1); n++)
            {
                ae_int32x2 y0,y1,y2,y3,x0,x1;
                AE_LA32X2_IP(x0,aX,pX);
                AE_LA32X2_IP(x1,aX1,pX1);
                AE_L32X2_IP(y0,pY0, sizeof(ae_int32x2));
                AE_L32X2_IP(y1,pY1, sizeof(ae_int32x2));
                AE_L32X2_IP(y2,pY2, sizeof(ae_int32x2));
                AE_L32X2_IP(y3,pY3, sizeof(ae_int32x2));
                AE_MULAAFD32RA_HH_LL(q0, x0, y0);
                AE_MULAAFD32RA_HH_LL(q1, x0, y1);
                AE_MULAAFD32RA_HH_LL(q2, x0, y2);
                AE_MULAAFD32RA_HH_LL(q3, x0, y3);
                AE_MULAAFD32RA_HH_LL(q4, x1, y0);
                AE_MULAAFD32RA_HH_LL(q5, x1, y1);
                AE_MULAAFD32RA_HH_LL(q6, x1, y2);
                AE_MULAAFD32RA_HH_LL(q7, x1, y3);
            }
            z0 = AE_TRUNCA32X2F64S(q0, q1, 16 + lsh);
            z1 = AE_TRUNCA32X2F64S(q2, q3, 16 + lsh);
            z2 = AE_TRUNCA32X2F64S(q4, q5, 16 + lsh);
            z3 = AE_TRUNCA32X2F64S(q6, q7, 16 + lsh);
            AE_S32_L_I (            z0    ,pZ,1*sizeof(int32_t));
            AE_S32_L_I (AE_SEL32_HH(z1,z1),pZ,2*sizeof(int32_t));
            AE_S32_L_I (            z1    ,pZ,3*sizeof(int32_t));
            AE_S32_L_XP(AE_SEL32_HH(z0,z0),pZ,P*sizeof(int32_t));
            AE_S32_L_I (            z2    ,pZ,1*sizeof(int32_t));
            AE_S32_L_I (AE_SEL32_HH(z3,z3),pZ,2*sizeof(int32_t));
            AE_S32_L_I (            z3    ,pZ,3*sizeof(int32_t));
            AE_S32_L_XP(AE_SEL32_HH(z2,z2),pZ,P*sizeof(int32_t));
        }
        if (M&1)
        {
            ae_int32x2 z0,z1;
            ae_int64 q0,q1,q2,q3;
            pY0=(ae_int32x2 *)Y0;
            pY1=(ae_int32x2 *)Y1;
            pY2=(ae_int32x2 *)Y2;
            pY3=(ae_int32x2 *)Y3;
            pX = px;
            aX = AE_LA64_PP(pX);
            q0=q1=q2=q3=AE_ZERO64();
            __Pragma("loop_count min=1")
            for (n = 0; n < (K>>1); n++)
            {
                ae_int32x2 y0,y1,y2,y3,x;
                AE_LA32X2_IP(x,aX,pX);
                AE_L32X2_IP(y0,pY0, sizeof(ae_int32x2));
                AE_L32X2_IP(y1,pY1, sizeof(ae_int32x2));
                AE_L32X2_IP(y2,pY2, sizeof(ae_int32x2));
                AE_L32X2_IP(y3,pY3, sizeof(ae_int32x2));
                AE_MULAAFD32RA_HH_LL(q0, x, y0);
                AE_MULAAFD32RA_HH_LL(q1, x, y1);
                AE_MULAAFD32RA_HH_LL(q2, x, y2);
                AE_MULAAFD32RA_HH_LL(q3, x, y3);
            }
            z0 = AE_TRUNCA32X2F64S(q0, q1, 16 + lsh);
            z1 = AE_TRUNCA32X2F64S(q2, q3, 16 + lsh);
            AE_S32_L_I (            z0    ,pZ,1*sizeof(int32_t));
            AE_S32_L_I (AE_SEL32_HH(z1,z1),pZ,2*sizeof(int32_t));
            AE_S32_L_I (            z1    ,pZ,3*sizeof(int32_t));
            AE_S32_L_XP(AE_SEL32_HH(z0,z0),pZ,P*sizeof(int32_t));
        }
    }
    /* process last 2 columns if any */
    if (P&2)
    {
        NASSERT_ALIGN8(Y0);
        NASSERT_ALIGN8(Y1);
        pZ=(ae_int32*)(z);
        /* copy 2 input columns to the srcatch memory with zero padding */
        pY0=(ae_int32x2 *)Y0;
        pY1=(ae_int32x2 *)Y1;
        for (n = 0; n < (N>>1); n++)
        {
            ae_int32x2 y0,y2;
            ae_valign aY;
            pY=(const ae_int32x2 *)(y+2*n*P);
            aY=AE_LA64_PP(pY);
            AE_LA32X2_IP(y0,aY,pY);
            pY=(const ae_int32x2 *)(y+2*n*P+P);
            aY=AE_LA64_PP(pY);
            AE_LA32X2_IP(y2,aY,pY);
            AE_S32X2_IP(AE_SEL32_HH(y0,y2),pY0,sizeof(ae_int32x2));
            AE_S32X2_IP(AE_SEL32_LL(y0,y2),pY1,sizeof(ae_int32x2));
        }
        if (N & 1)
        {
            ae_int32x2 y0,y1;
            ae_valign aY;
            pY=(const ae_int32x2 *)(y+2*n*P);
            aY=AE_LA64_PP(pY);
            AE_LA32X2_IP(y0,aY,pY);
            AE_LA32X2_IP(y1,aY,pY);
            AE_S32X2_IP(AE_SEL32_HH(y0,AE_ZERO32()),pY0,sizeof(ae_int32x2));
            AE_S32X2_IP(AE_SEL32_LL(y0,AE_ZERO32()),pY1,sizeof(ae_int32x2));
        }
        /* compute 2*M outputs  */
        px =(ae_int32x2 *)x;
         for (m = 0; m < (M>>2); m++)
        {
            ae_int64 q0,q1,q2,q3;
            ae_int64 q4,q5,q6,q7;
            ae_int32x2 z0,z1,z2,z3;
            pY0=(ae_int32x2 *)Y0;
            pY1=(ae_int32x2 *)Y1;
            pX = px;
            pX1 = (const ae_int32x2 *)XT_ADDX4(N, (uintptr_t)pX );
            pX2 = (const ae_int32x2 *)XT_ADDX4(N, (uintptr_t)pX1);
            pX3 = (const ae_int32x2 *)XT_ADDX4(N, (uintptr_t)pX2);
            px  = (const ae_int32x2 *)XT_ADDX4(N, (uintptr_t)pX3);
            aX = AE_LA64_PP(pX);
            aX1= AE_LA64_PP(pX1);
            aX2= AE_LA64_PP(pX2);
            aX3= AE_LA64_PP(pX3);
            q0=q1=q2=q3=
            q4=q5=q6=q7=AE_ZERO64();
            __Pragma("loop_count min=1")
            for (n = 0; n < (K>>1); n++)
            {
                ae_int32x2 y0,y1,x0,x1,x2,x3;
                AE_LA32X2_IP(x0,aX,pX);
                AE_LA32X2_IP(x1,aX1,pX1);
                AE_LA32X2_IP(x2,aX2,pX2);
                AE_LA32X2_IP(x3,aX3,pX3);
                AE_L32X2_IP(y0,pY0, sizeof(ae_int32x2));
                AE_L32X2_IP(y1,pY1, sizeof(ae_int32x2));
                AE_MULAAFD32RA_HH_LL(q0, x0, y0);
                AE_MULAAFD32RA_HH_LL(q1, x0, y1);
                AE_MULAAFD32RA_HH_LL(q2, x1, y0);
                AE_MULAAFD32RA_HH_LL(q3, x1, y1);
                AE_MULAAFD32RA_HH_LL(q4, x2, y0);
                AE_MULAAFD32RA_HH_LL(q5, x2, y1);
                AE_MULAAFD32RA_HH_LL(q6, x3, y0);
                AE_MULAAFD32RA_HH_LL(q7, x3, y1);
            }
            z0 = AE_TRUNCA32X2F64S(q0, q1, 16 + lsh);
            z1 = AE_TRUNCA32X2F64S(q2, q3, 16 + lsh);
            z2 = AE_TRUNCA32X2F64S(q4, q5, 16 + lsh);
            z3 = AE_TRUNCA32X2F64S(q6, q7, 16 + lsh);
            AE_S32_L_I (            z0    ,pZ,1*sizeof(int32_t));
            AE_S32_L_XP(AE_SEL32_HH(z0,z0),pZ,P*sizeof(int32_t));
            AE_S32_L_I (            z1    ,pZ,1*sizeof(int32_t));
            AE_S32_L_XP(AE_SEL32_HH(z1,z1),pZ,P*sizeof(int32_t));
            AE_S32_L_I (            z2    ,pZ,1*sizeof(int32_t));
            AE_S32_L_XP(AE_SEL32_HH(z2,z2),pZ,P*sizeof(int32_t));
            AE_S32_L_I (            z3    ,pZ,1*sizeof(int32_t));
            AE_S32_L_XP(AE_SEL32_HH(z3,z3),pZ,P*sizeof(int32_t));
        }
        m<<=2;
        if (M&2)
        {
            ae_int64 q0,q1,q2,q3;
            ae_int32x2 z0,z1;
            pY0=(ae_int32x2 *)Y0;
            pY1=(ae_int32x2 *)Y1;
            pX = px;
            pX1 = (const ae_int32x2 *)XT_ADDX4(N, (uintptr_t)pX );
            px  = (const ae_int32x2 *)XT_ADDX4(N, (uintptr_t)pX1);
            aX = AE_LA64_PP(pX);
            aX1= AE_LA64_PP(pX1);
            q0=q1=q2=q3=AE_ZERO64();
            __Pragma("loop_count min=1")
            for (n = 0; n < (K>>1); n++)
            {
                ae_int32x2 y0,y1,x0,x1;
                AE_LA32X2_IP(x0,aX,pX);
                AE_LA32X2_IP(x1,aX1,pX1);
                AE_L32X2_IP(y0,pY0, sizeof(ae_int32x2));
                AE_L32X2_IP(y1,pY1, sizeof(ae_int32x2));
                AE_MULAAFD32RA_HH_LL(q0, x0, y0);
                AE_MULAAFD32RA_HH_LL(q1, x0, y1);
                AE_MULAAFD32RA_HH_LL(q2, x1, y0);
                AE_MULAAFD32RA_HH_LL(q3, x1, y1);
            }
            z0 = AE_TRUNCA32X2F64S(q0, q1, 16 + lsh);
            z1 = AE_TRUNCA32X2F64S(q2, q3, 16 + lsh);
            AE_S32_L_I (            z0    ,pZ,1*sizeof(int32_t));
            AE_S32_L_XP(AE_SEL32_HH(z0,z0),pZ,P*sizeof(int32_t));
            AE_S32_L_I (            z1    ,pZ,1*sizeof(int32_t));
            AE_S32_L_XP(AE_SEL32_HH(z1,z1),pZ,P*sizeof(int32_t));
            m+=2;
        }
        if (M&1)
        {
            ae_int64 q0,q1;
            ae_int32x2 z0;
            pY0=(ae_int32x2 *)Y0;
            pY1=(ae_int32x2 *)Y1;
            pX = px;
            aX = AE_LA64_PP(pX);
            q0=q1=AE_ZERO64();
            __Pragma("loop_count min=1")
            for (n = 0; n < (K>>1); n++)
            {
                ae_int32x2 y0,y1,x;
                AE_LA32X2_IP(x,aX,pX);
                AE_L32X2_IP(y0,pY0, sizeof(ae_int32x2));
                AE_L32X2_IP(y1,pY1, sizeof(ae_int32x2));
                AE_MULAAFD32RA_HH_LL(q0, x, y0);
                AE_MULAAFD32RA_HH_LL(q1, x, y1);
            }
            z0 = AE_TRUNCA32X2F64S(q0, q1, 16 + lsh);
            AE_S32_L_I (            z0    ,pZ,1*sizeof(int32_t));
            AE_S32_L_XP(AE_SEL32_HH(z0,z0),pZ,P*sizeof(int32_t));
        }
        z+=2,y+=2;  /* go to the next 2 columns */
    }
    /* process last column if any */
    if (P&1)
    {
        NASSERT_ALIGN8(Y0);
        NASSERT_ALIGN8(Y1);
        pZ=(ae_int32*)(z);
        /* copy input column to the srcatch memory with zero padding */
        pY0=(ae_int32x2 *)Y0;
        pY=(const ae_int32x2 *)(y);
        for (n = 0; n < (N); n++)
        {
            ae_int32x2 y0;
            AE_L32_XP(y0,castxcc(ae_int32,pY),P*sizeof(int32_t));
            AE_S32_L_IP(y0,castxcc(ae_int32,pY0),sizeof(int32_t));
        }
        if (N & 1)
        {
            AE_S32_L_IP(AE_ZERO32(),castxcc(ae_int32,pY0),sizeof(int32_t));
        }
        /* compute M outputs  */
        px =(ae_int32x2 *)x;
        for (m = 0; m < M; m++)
        {
            ae_int64 q0;
            ae_int32x2 z0;
            pY0=(ae_int32x2 *)Y0;
            pX = px;
            px  = (const ae_int32x2 *)XT_ADDX4(N, (uintptr_t)px);
            aX = AE_LA64_PP(pX);
            q0=AE_ZERO64();
            __Pragma("loop_count min=1")
            for (n = 0; n < (K>>1); n++)
            {
                ae_int32x2 y0,x;
                AE_LA32X2_IP(x,aX,pX);
                AE_L32X2_IP(y0,pY0, sizeof(ae_int32x2));
                AE_MULAAFD32RA_HH_LL(q0, x, y0);
            }
            z0 = AE_TRUNCA32X2F64S(q0, q0, 16 + lsh);
            AE_S32_L_XP(z0,pZ,P*sizeof(int32_t));
        }
    }
}

size_t mtx_mpy32x32_getScratchSize( int M, int N, int P )
{
    (void)M;(void)P;
	return N<=0 ? 0: ((((N)+1)&(~1))*4*sizeof(int32_t));
}
