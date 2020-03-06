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
void conv2d_5x5_8x8(void* pScr, int8_t  *z, const int8_t  * x, const int8_t  * y, int rsh, int P, int Q)
{
#   define M 5
#   define N 5
    int16_t * w;// [9][4*8]: weights - reordered x
    const int8_t     * restrict pY0;
    const int8_t     * restrict pY1;
    const int8_t     * restrict pY2;
    const int8_t     * restrict pY3;
    const int8_t     * restrict pY4;
          ae_int32   * restrict pZ=(ae_int32 *)z;
          ae_int16x4 * restrict pW;
    int i,j;
    ae_int16x4 rnd;
    ae_int64 mask;

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
    rnd = AE_MOVDA16(128);
    mask = AE_MOVINT64_FROMINT16X4(AE_MOVDA16(0xFF00));

    /* fill coefficients in right order
    order:
    x[4] x[3] x[2] x[1] x[0] 0    0    0 
    0    x[4] x[3] x[2] x[1] x[0] 0    0
    0    0    x[4] x[3] x[2] x[1] x[0] 0 
    0    0    0    x[4] x[3] x[2] x[1] x[0] 
       .... next 5 coefficients ....
    */
    pW=(ae_int16x4*)w;
    {
        int m;
        const int8_t * pX;
        ae_int16x4 x0, x1, x2, x3, x4;
        /* pad coefficients with zeroes for processing N-1 first and last rows */
        x0 = AE_ZERO16();
        for (m=0; m<(M-1)*8; m++)
        {
            AE_S16X4_IP(x0,pW,sizeof(ae_int16x4));
        }
        for (m=0; m<M; m++)
        {
            pX=x+(M-1-m)*M;
            x0 = (int16_t)(*pX)<<8; pX++;
            x1 = (int16_t)(*pX)<<8; pX++;
            x2 = (int16_t)(*pX)<<8; pX++;
            x3 = (int16_t)(*pX)<<8; pX++;
            x4 = (int16_t)(*pX)<<8;
            x0=AE_SEL16_4321(x0,AE_ZERO16());
            x1=AE_SEL16_4321(x2,x1);
            x1=AE_SEL16_4321(x3,x1);
            x1=AE_SEL16_4321(x4,x1);
            AE_S16X4_IP(x1,pW,sizeof(ae_int16x4));
            AE_S16X4_IP(x0,pW,sizeof(ae_int16x4));
            x0=AE_SEL16_4321(x1,x0);
            x1=AE_SEL16_4321(AE_ZERO16(),x1);
            AE_S16X4_IP(x1,pW,sizeof(ae_int16x4));
            AE_S16X4_IP(x0,pW,sizeof(ae_int16x4));
            x0=AE_SEL16_4321(x1,x0);
            x1=AE_SEL16_4321(AE_ZERO16(),x1);
            AE_S16X4_IP(x1,pW,sizeof(ae_int16x4));
            AE_S16X4_IP(x0,pW,sizeof(ae_int16x4));
            x0=AE_SEL16_4321(x1,x0);
            x1=AE_SEL16_4321(AE_ZERO16(),x1);
            AE_S16X4_IP(x1,pW,sizeof(ae_int16x4));
            AE_S16X4_IP(x0,pW,sizeof(ae_int16x4));
        }
    }
    WAE_CBEGIN0((uintptr_t)(w));
    WAE_CEND0((uintptr_t)(w+(M+M-1)*4*8));

    for (i=0; i<(P+M-1); i++)
    {
        int wrow, yrow;
        ae_int16x4 y00,y01,y10,y11,y20,y21,y30,y31,y40,y41;
        ae_int16x4 w00,w01,w10,w11,w20,w21,w30,w31;
        ae_int16x4 w00l,w01l,w10l,w11l,w20l,w21l,w30l,w31l;
        ae_int64 S0,S1,S2,S3,tmp64;
        ae_int16x4 tmp16;
        ae_int32x2 R32, T32;

        yrow = XT_MAX(XT_MIN(i,P-1)-(M-1), 0);
        wrow = 2*(M-1) - i + yrow;
        NASSERT(yrow>=0 && yrow<=(P-M));
        NASSERT(wrow>=0 && wrow<=8);
        pY0=y + yrow*Q;
        pY1=pY0 + Q;
        pY2=pY1 + Q;
        pY3=pY2 + Q;
        pY4=pY3 + Q;
        /* First N-1 samples */
        {
            pW=(ae_int16x4*)(w + 4*8*wrow) + 1;

            AE_L8X4F_IP(y01,pY0,sizeof(int8_t)*4);
            AE_L8X4F_IP(y11,pY1,sizeof(int8_t)*4);
            AE_L8X4F_IP(y21,pY2,sizeof(int8_t)*4);
            AE_L8X4F_IP(y31,pY3,sizeof(int8_t)*4);
            AE_L8X4F_IP(y41,pY4,sizeof(int8_t)*4);

            AE_L16X4_IP(w01,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w11,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w21,pW,2*sizeof(ae_int16x4));
            AE_L16X4_XC(w31,pW,2*sizeof(ae_int16x4));
            S0=AE_MULZAAAAQ16(w01,y01);
            S1=AE_MULZAAAAQ16(w11,y01);
            S2=AE_MULZAAAAQ16(w21,y01);
            S3=AE_MULZAAAAQ16(w31,y01);

            AE_L16X4_IP(w01,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w11,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w21,pW,2*sizeof(ae_int16x4));
            AE_L16X4_XC(w31,pW,2*sizeof(ae_int16x4));
            AE_MULAAAAQ16(S0,w01,y11);
            AE_MULAAAAQ16(S1,w11,y11);
            AE_MULAAAAQ16(S2,w21,y11);
            AE_MULAAAAQ16(S3,w31,y11);

            AE_L16X4_IP(w01,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w11,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w21,pW,2*sizeof(ae_int16x4));
            AE_L16X4_XC(w31,pW,2*sizeof(ae_int16x4));
            AE_MULAAAAQ16(S0,w01,y21);
            AE_MULAAAAQ16(S1,w11,y21);
            AE_MULAAAAQ16(S2,w21,y21);
            AE_MULAAAAQ16(S3,w31,y21);

            AE_L16X4_IP(w01,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w11,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w21,pW,2*sizeof(ae_int16x4));
            AE_L16X4_XC(w31,pW,2*sizeof(ae_int16x4));
            AE_MULAAAAQ16(S0,w01,y31);
            AE_MULAAAAQ16(S1,w11,y31);
            AE_MULAAAAQ16(S2,w21,y31);
            AE_MULAAAAQ16(S3,w31,y31);

            AE_L16X4_IP(w01l,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w11l,pW,sizeof(ae_int16x4));
            /* preload last 4 coefficients to use it in the loop further */
            AE_L16X4_IP(w20l,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w21l,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w30l,pW,sizeof(ae_int16x4)); w31l=AE_L16X4_I (pW,0);
            AE_MULAAAAQ16(S0,w01l,y41);
            AE_MULAAAAQ16(S1,w11l,y41);
            AE_MULAAAAQ16(S2,w21l,y41);
            AE_MULAAAAQ16(S3,w31l,y41);

            tmp16=AE_TRUNC16X4F32(AE_TRUNCA32X2F64S(S2,S0,48-(15+rsh)),AE_TRUNCA32X2F64S(S3,S1,48-(15+rsh)));
            tmp16=AE_ADD16S(tmp16,rnd);/* add rounding bit */
            tmp64=AE_MOVINT64_FROMINT16X4(tmp16);
            /* pack 4 8-bit samples into the 32-bit number */
            tmp64 = AE_AND64(tmp64, mask);
            R32 = AE_MOVINT32X2_FROMINT64(tmp64);
            T32 = AE_MOVINT32X2_FROMINT64(AE_SRLI64(tmp64, 40));
            R32 = AE_OR32(R32, T32);
            AE_S32_L_IP(R32,pZ,4*sizeof(int8_t));
        }
        /* Next samples */
        __Pragma("loop_count min=1");
        for (j=0; j<((Q+N-1)>>2)-2; j++)
        {
            /* all coefficients except last 3 are reloaded for better scheduling */
            pW=(ae_int16x4*)(w + 4*8*wrow);

            y00=AE_L8X4F_I(pY0,-(int)sizeof(int8_t)*4); AE_L8X4F_IP(y01,pY0,sizeof(int8_t)*4);
            y10=AE_L8X4F_I(pY1,-(int)sizeof(int8_t)*4); AE_L8X4F_IP(y11,pY1,sizeof(int8_t)*4);
            y20=AE_L8X4F_I(pY2,-(int)sizeof(int8_t)*4); AE_L8X4F_IP(y21,pY2,sizeof(int8_t)*4);
            y30=AE_L8X4F_I(pY3,-(int)sizeof(int8_t)*4); AE_L8X4F_IP(y31,pY3,sizeof(int8_t)*4);
            y40=AE_L8X4F_I(pY4,-(int)sizeof(int8_t)*4); AE_L8X4F_IP(y41,pY4,sizeof(int8_t)*4);

            AE_L16X4_IP(w00,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w01,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w10,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w11,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w20,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w21,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w30,pW,sizeof(ae_int16x4)); AE_L16X4_XC(w31,pW,sizeof(ae_int16x4));
            S0=AE_MULZAAAAQ16(w00,y00); AE_MULAAAAQ16(S0,w01,y01);
            S1=AE_MULZAAAAQ16(w10,y00); AE_MULAAAAQ16(S1,w11,y01);
            S2=AE_MULZAAAAQ16(w20,y00); AE_MULAAAAQ16(S2,w21,y01);
            S3=AE_MULZAAAAQ16(w30,y00); AE_MULAAAAQ16(S3,w31,y01);

            AE_L16X4_IP(w00,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w01,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w10,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w11,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w20,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w21,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w30,pW,sizeof(ae_int16x4)); AE_L16X4_XC(w31,pW,sizeof(ae_int16x4));
            AE_MULAAAAQ16(S0,w00,y10); AE_MULAAAAQ16(S0,w01,y11);
            AE_MULAAAAQ16(S1,w10,y10); AE_MULAAAAQ16(S1,w11,y11);
            AE_MULAAAAQ16(S2,w20,y10); AE_MULAAAAQ16(S2,w21,y11);
            AE_MULAAAAQ16(S3,w30,y10); AE_MULAAAAQ16(S3,w31,y11);

            AE_L16X4_IP(w00,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w01,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w10,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w11,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w20,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w21,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w30,pW,sizeof(ae_int16x4)); AE_L16X4_XC(w31,pW,sizeof(ae_int16x4));
            AE_MULAAAAQ16(S0,w00,y20); AE_MULAAAAQ16(S0,w01,y21);
            AE_MULAAAAQ16(S1,w10,y20); AE_MULAAAAQ16(S1,w11,y21);
            AE_MULAAAAQ16(S2,w20,y20); AE_MULAAAAQ16(S2,w21,y21);
            AE_MULAAAAQ16(S3,w30,y20); AE_MULAAAAQ16(S3,w31,y21);

            AE_L16X4_IP(w00,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w01,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w10,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w11,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w20,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w21,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w30,pW,sizeof(ae_int16x4)); AE_L16X4_XC(w31,pW,sizeof(ae_int16x4));
            AE_MULAAAAQ16(S0,w00,y30); AE_MULAAAAQ16(S0,w01,y31);
            AE_MULAAAAQ16(S1,w10,y30); AE_MULAAAAQ16(S1,w11,y31);
            AE_MULAAAAQ16(S2,w20,y30); AE_MULAAAAQ16(S2,w21,y31);
            AE_MULAAAAQ16(S3,w30,y30); AE_MULAAAAQ16(S3,w31,y31);

            AE_L16X4_IP(w00l,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w01l,pW,sizeof(ae_int16x4));
            AE_L16X4_IP(w10l,pW,sizeof(ae_int16x4)); AE_L16X4_IP(w11l,pW,sizeof(ae_int16x4));
            w20l=AE_L16X4_I (pW,0);
            AE_MULAAAAQ16(S0,w00l,y40); AE_MULAAAAQ16(S0,w01l,y41);
            AE_MULAAAAQ16(S1,w10l,y40); AE_MULAAAAQ16(S1,w11l,y41);
            AE_MULAAAAQ16(S2,w20l,y40); AE_MULAAAAQ16(S2,w21l,y41);
            AE_MULAAAAQ16(S3,w30l,y40); AE_MULAAAAQ16(S3,w31l,y41);

            tmp16=AE_TRUNC16X4F32(AE_TRUNCA32X2F64S(S2,S0,48-(15+rsh)),AE_TRUNCA32X2F64S(S3,S1,48-(15+rsh)));
            tmp16=AE_ADD16S(tmp16,rnd);/* add rounding bit */
            tmp64=AE_MOVINT64_FROMINT16X4(tmp16);
            /* pack 4 8-bit samples into the 32-bit number */
            tmp64 = AE_AND64(tmp64, mask);
            R32 = AE_MOVINT32X2_FROMINT64(tmp64);
            T32 = AE_MOVINT32X2_FROMINT64(AE_SRLI64(tmp64, 40));
            R32 = AE_OR32(R32, T32);
            AE_S32_L_IP(R32,pZ,4*sizeof(int8_t));
        }
        /* Last N-1 samples */
        {
            pW=(ae_int16x4*)(w + 4*8*wrow);

            y00=AE_L8X4F_I(pY0,-(int)sizeof(int8_t)*4);
            y10=AE_L8X4F_I(pY1,-(int)sizeof(int8_t)*4);
            y20=AE_L8X4F_I(pY2,-(int)sizeof(int8_t)*4);
            y30=AE_L8X4F_I(pY3,-(int)sizeof(int8_t)*4);
            y40=AE_L8X4F_I(pY4,-(int)sizeof(int8_t)*4);

            AE_L16X4_IP(w00,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w10,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w20,pW,2*sizeof(ae_int16x4));
            AE_L16X4_XC(w30,pW,2*sizeof(ae_int16x4));
            S0=AE_MULZAAAAQ16(w00,y00);
            S1=AE_MULZAAAAQ16(w10,y00);
            S2=AE_MULZAAAAQ16(w20,y00);
            S3=AE_MULZAAAAQ16(w30,y00);

            AE_L16X4_IP(w00,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w10,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w20,pW,2*sizeof(ae_int16x4));
            AE_L16X4_XC(w30,pW,2*sizeof(ae_int16x4));
            AE_MULAAAAQ16(S0,w00,y10);
            AE_MULAAAAQ16(S1,w10,y10);
            AE_MULAAAAQ16(S2,w20,y10);
            AE_MULAAAAQ16(S3,w30,y10);

            AE_L16X4_IP(w00,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w10,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w20,pW,2*sizeof(ae_int16x4));
            AE_L16X4_XC(w30,pW,2*sizeof(ae_int16x4));
            AE_MULAAAAQ16(S0,w00,y20);
            AE_MULAAAAQ16(S1,w10,y20);
            AE_MULAAAAQ16(S2,w20,y20);
            AE_MULAAAAQ16(S3,w30,y20);

            AE_L16X4_IP(w00,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w10,pW,2*sizeof(ae_int16x4));
            AE_L16X4_IP(w20,pW,2*sizeof(ae_int16x4));
            w30=AE_L16X4_I (pW,0);
            AE_MULAAAAQ16(S0,w00,y30);
            AE_MULAAAAQ16(S1,w10,y30);
            AE_MULAAAAQ16(S2,w20,y30);
            AE_MULAAAAQ16(S3,w30,y30);

            AE_MULAAAAQ16(S0,w00l,y40);
            AE_MULAAAAQ16(S1,w10l,y40);
            AE_MULAAAAQ16(S2,w20l,y40);
            AE_MULAAAAQ16(S3,w30l,y40);

            tmp16=AE_TRUNC16X4F32(AE_TRUNCA32X2F64S(S2,S0,48-(15+rsh)),AE_TRUNCA32X2F64S(S3,S1,48-(15+rsh)));
            tmp16=AE_ADD16S(tmp16,rnd);/* add rounding bit */
            tmp64=AE_MOVINT64_FROMINT16X4(tmp16);
            /* pack 4 8-bit samples into the 32-bit number */
            tmp64 = AE_AND64(tmp64, mask);
            R32 = AE_MOVINT32X2_FROMINT64(tmp64);
            T32 = AE_MOVINT32X2_FROMINT64(AE_SRLI64(tmp64, 40));
            R32 = AE_OR32(R32, T32);
            AE_S32_L_IP(R32,pZ,4*sizeof(int8_t));
        }
    }
#   undef M
#   undef N
}

// scratch allocatation functions. return required scratch size in bytes
size_t conv2d_5x5_8x8_getScratchSize   (int P, int Q)
{
    return 9*8*4*sizeof(int16_t);/* weights (reordered x[5][5]) */
} // MxN=5x5
