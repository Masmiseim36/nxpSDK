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
  code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
#include "common.h"

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

  Input:
  x[M][N]   input data Q15, Q7
  y[P][Q]   input data Q15, Q7
  M         number of rows in the matrix x
  N         number of columns in the matrix x
  P         number of rows in the matrix y
  Q         number of columns in the matrix y

  Output:
  z	[M+P-1][N+Q-1] output data, Q(7-rsh), Q(15-rsh)

  Temporary:
  pScr     scratch data. Should have size at least as requested by 
           corresponding scratch allocation function

  Restrictions:
  x,y,z        should not overlap
  x,y,z,pScr   aligned on a 8-bytes boundary
  P, Q	       >0 and multiplies of 8
-------------------------------------------------------------------------*/
void conv2d_3x3_16x16(void* pScr, int16_t *z, const int16_t * x, const int16_t * y, int rsh, int P, int Q)
{
#   define M 3
#   define N 3
    int16_t* w;// [5][2*4]: weights - reordered x
    const ae_int16x4 * restrict pY0;
    const ae_int16x4 * restrict pY1;
    const ae_int16x4 * restrict pY2;
          ae_int16x4 * restrict pZ=(ae_int16x4 *)z;
          ae_int16x4 * restrict pW;
    int i,j;
    ae_valign alZ;

    NASSERT(x);
    NASSERT(y);
    NASSERT(z);
    NASSERT(pScr);
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT_ALIGN(z,8);
    NASSERT_ALIGN(pScr,8);
    NASSERT(P>=0 && P%8==0);
    NASSERT(Q>=0 && Q%8==0);

    if(P<=0 || Q<=0) return;
    w=(int16_t*)pScr;
    alZ = AE_ZALIGN64();

    /* fill coefficients in right order
    order:
      x[2] x[1] x[0] 0
      0    x[2] x[1] x[0]
      .... next 3 coefficients ....
    */
    pW=(ae_int16x4*)w;
    {
        int m;
        ae_int16* pX;
        ae_int16x4 x0,x1,x2,zero;
        zero = AE_ZERO16();
        /* pad coefficients with zeroes for processing N-1 first and last rows */
        for (m=0; m<(M-1)*2; m++)
        {
            AE_S16X4_IP(zero,pW,sizeof(ae_int16x4));
        }
        for (m=0; m<M; m++)
        {
            pX=(ae_int16*)(x+(M-1-m)*N);
            AE_L16_IP(x0,pX,sizeof(int16_t));
            AE_L16_IP(x1,pX,sizeof(int16_t));
            AE_L16_IP(x2,pX,sizeof(int16_t));
            x0 = AE_SEL16_4321(x0, zero);
            x0 = AE_SEL16_4321(x1, x0);
            x0 = AE_SEL16_4321(x2, x0);
            AE_S16X4_IP(x0,pW,sizeof(ae_int16x4));
            x0 = AE_SEL16_4321(zero, x0);
            AE_S16X4_IP(x0,pW,sizeof(ae_int16x4));
        }
    }
    WAE_CBEGIN0((uintptr_t)(w));
    WAE_CEND0((uintptr_t)(w+(M+M-1)*4*2));

    for (i=0; i<(P+M-1); i++)
    {
        int wrow, yrow;
        ae_int16x4 y00,y01,y10,y11,y20,y21;
        ae_int16x4 w00,w01,w10,w11,w20,w21;
        ae_int64 S0,S1,S2,S3;
        ae_int32x2 R;
        ae_int16x4 r;

        yrow = XT_MAX(XT_MIN(i,P-1)-(M-1), 0);
        NASSERT(yrow>=0 && yrow<=(P-M));
        pY0 = (ae_int16x4 *)(y + yrow*Q);
        pY1 = (ae_int16x4 *)((int16_t *)pY0 + Q);
        pY2 = (ae_int16x4 *)((int16_t *)pY1 + Q);
        y00 = AE_ZERO16();
        y10 = AE_ZERO16();
        y20 = AE_ZERO16();

        /* Preload coefficients */
        wrow = 2*(M-1) - i + yrow;
        NASSERT(wrow>=0 && wrow<(M+M-1));
        pW = (ae_int16x4*)(w + 2*4*wrow);
        AE_L16X4_IP(w00, pW, sizeof(ae_int16x4));
        AE_L16X4_XC(w01, pW, sizeof(ae_int16x4));
        AE_L16X4_IP(w10, pW, sizeof(ae_int16x4));
        AE_L16X4_XC(w11, pW, sizeof(ae_int16x4));
        AE_L16X4_IP(w20, pW, sizeof(ae_int16x4));
        w21 = AE_L16X4_I(pW, 0);

        /* Process all but last N-1 samples of the i-th row */
        __Pragma("loop_count min=2");
        for (j=0; j<(Q>>2); j++)
        {

            AE_L16X4_IP(y01, pY0, sizeof(ae_int16x4));
            AE_L16X4_IP(y11, pY1, sizeof(ae_int16x4));
            AE_L16X4_IP(y21, pY2, sizeof(ae_int16x4));
            y00 = AE_SEL16_5432(y00, y01);
            y10 = AE_SEL16_5432(y10, y11);
            y20 = AE_SEL16_5432(y20, y21);

            S0=AE_MULZAAAAQ16(w00, y00);
            AE_MULAAAAQ16(S0, w10, y10);
            AE_MULAAAAQ16(S0, w20, y20);
            S1=AE_MULZAAAAQ16(w01, y00);
            AE_MULAAAAQ16(S1, w11, y10);
            AE_MULAAAAQ16(S1, w21, y20);
            S2=AE_MULZAAAAQ16(w00, y01);
            AE_MULAAAAQ16(S2, w10, y11);
            AE_MULAAAAQ16(S2, w20, y21);
            S3=AE_MULZAAAAQ16(w01, y01);
            AE_MULAAAAQ16(S3, w11, y11);
            AE_MULAAAAQ16(S3, w21, y21);

            r=AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(S0,S1,48-(15+rsh)),AE_TRUNCA32X2F64S(S2,S3,48-(15+rsh)));
            AE_SA16X4_IP(r,alZ,pZ);

            y00 = AE_L16X4_I(pY0, -(int)sizeof(ae_int16x4));
            y10 = AE_L16X4_I(pY1, -(int)sizeof(ae_int16x4));
            y20 = AE_L16X4_I(pY2, -(int)sizeof(ae_int16x4));
        }
        AE_SA64POS_FP(alZ, pZ);
        /* Last N-1 samples */
        {
            y01 = AE_ZERO16();
            y11 = AE_ZERO16();
            y21 = AE_ZERO16();
            y00 = AE_SEL16_5432(y00, y01);
            y10 = AE_SEL16_5432(y10, y11);
            y20 = AE_SEL16_5432(y20, y21);

            S0=AE_MULZAAAAQ16(w00, y00);
            AE_MULAAAAQ16(S0, w10, y10);
            AE_MULAAAAQ16(S0, w20, y20);
            S1=AE_MULZAAAAQ16(w01, y00);
            AE_MULAAAAQ16(S1, w11, y10);
            AE_MULAAAAQ16(S1, w21, y20);

            R=AE_MOVINT32X2_FROMINT16X4(AE_ROUND16X4F32SASYM(AE_TRUNCA32X2F64S(S1,S0,48-(15+rsh)),AE_TRUNCA32X2F64S(S1,S0,48-(15+rsh))));
            AE_S32_L_IP(R,castxcc(ae_int32,pZ),sizeof(int16_t)*2);
        }
    }
#   undef M
#   undef N
}

// scratch allocatation functions. return required scratch size in bytes
size_t conv2d_3x3_16x16_getScratchSize (int P, int Q) 
{
    return 5*2*4*sizeof(int16_t);/* weights (reordered x[3][3]) */
} 
