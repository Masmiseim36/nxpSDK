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
void conv2d_11x7_16x16(void* pScr, int16_t *z, const int16_t * x, const int16_t * y, int rsh, int P, int Q)
{
#   define M 11
#   define N 7
    int i,j,n,n0,n1,m1;
    int16_t * w;
    const ae_int16   * restrict pXrd;
    const ae_int16x4 * restrict pYrd0;
    const ae_int16x4 * restrict pYrd1;
          ae_int16x4 * restrict pZ;
          ae_int16x4 * restrict pWwr;
    const ae_int16x4 * restrict pWrd0;
    const ae_int16x4 * restrict pWrd1;
    ae_int16x4 Y0123, Y4567, Y89AB, YCDEF;
    ae_int16x4 Y2345, Y6789, YABCD;
    ae_int64   S0, S1, S2, S3, S4, S5, S6, S7;
    ae_int32x2 R32, T32;
    ae_int16x4 z0;
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

    if (P<=0 || Q<=0) return;
    w = (int16_t *)pScr;
    /* Store coefficients in the next order:
     *   x[2] x[1] x[0]  0
     *   x[3] x[2] x[1] x[0]
     *   x[4] x[3] x[2] x[1]
     *   x[5] x[4] x[3] x[2]
     *   x[6] x[5] x[4] x[3]
     *    0   x[6] x[5] x[4]
     * Start from the last row
     */
    pWwr = (ae_int16x4 *)w;
    pXrd = (ae_int16 *)x+10*7;
    for (i = 0; i < 11; i++)
    {
        ae_int16x4 Xin, Wout;

        Wout = AE_ZERO16();
        AE_L16_IP(Xin, pXrd, sizeof(int16_t));
        Wout = AE_SEL16_4321(Xin, Wout);
        AE_L16_IP(Xin, pXrd, sizeof(int16_t));
        Wout = AE_SEL16_4321(Xin, Wout);
        AE_L16_IP(Xin, pXrd, sizeof(int16_t));
        Wout = AE_SEL16_4321(Xin, Wout);
        AE_S16X4_IP(Wout, pWwr, sizeof(ae_int16x4));/* x[2] x[1] x[0]  0   */
        AE_L16_IP(Xin, pXrd, sizeof(int16_t));
        Wout = AE_SEL16_4321(Xin, Wout);
        AE_S16X4_IP(Wout, pWwr, sizeof(ae_int16x4));/* x[3] x[2] x[1] x[0] */
        AE_L16_IP(Xin, pXrd, sizeof(int16_t));
        Wout = AE_SEL16_4321(Xin, Wout);
        AE_S16X4_IP(Wout, pWwr, sizeof(ae_int16x4));/* x[4] x[3] x[2] x[1] */
        AE_L16_IP(Xin, pXrd, sizeof(int16_t));
        Wout = AE_SEL16_4321(Xin, Wout);
        AE_S16X4_IP(Wout, pWwr, sizeof(ae_int16x4));/* x[5] x[4] x[3] x[2] */
        AE_L16_XP(Xin, pXrd, (-6-7)*(int)sizeof(int16_t));/* jump to the previous row */
        Wout = AE_SEL16_4321(Xin, Wout);
        AE_S16X4_IP(Wout, pWwr, sizeof(ae_int16x4));/* x[6] x[5] x[4] x[3] */
        Xin = AE_ZERO16();
        Wout = AE_SEL16_4321(Xin, Wout);
        AE_S16X4_IP(Wout, pWwr, sizeof(ae_int16x4));/*  0   x[6] x[5] x[4] */
    }

    /*
     * Processing of convolution
     */
    pZ = (ae_int16x4 *)(z);
    alZ = AE_ZALIGN64();
    for (i=0; i<M+P-1; i++)
    {
        n0=XT_MAX(i+1-M,0);
        n1=XT_MIN(i+1,P);
        m1=XT_MIN(i+1,M);

        WAE_CBEGIN0((uintptr_t)(y + n0*Q + 8));
        WAE_CEND0((uintptr_t)(y + n1*Q));
        /* First 8 samples of the i-th row */
        {
            pYrd0 = (ae_int16x4 *)(y + n0*Q);
            pWrd1 = (ae_int16x4 *)w + (M-m1)*6;
            pWrd0 = pWrd1 + 4;
            S0 = S1 = S2 = S3 = AE_ZERO64();
            S4 = S5 = S6 = S7 = AE_ZERO64();
            __Pragma("loop_count min=1, max=11");
            for (n=0; n<n1-n0; n++)
            {
                ae_int16x4 Wz654, W6543, W5432, W4321, W3210, W210z, W10zz, W0zzz;
                AE_L16X4_IP(Y0123, pYrd0, sizeof(ae_int16x4));
                AE_L16X4_XP(Y4567, pYrd0, Q*sizeof(int16_t) - sizeof(ae_int16x4));

                AE_L16X4_IP(W210z, pWrd1, sizeof(ae_int16x4));
                AE_L16X4_IP(W6543, pWrd0, sizeof(ae_int16x4));
                W0zzz = AE_SEL16_5432(W210z, AE_ZERO16());
                W4321 = AE_SEL16_5432(W6543, W210z);
                AE_L16X4_XP(W3210, pWrd1, (6-1)*sizeof(ae_int16x4));
                AE_L16X4_XP(Wz654, pWrd0, (6-1)*sizeof(ae_int16x4));
                W10zz = AE_SEL16_5432(W3210, AE_ZERO16());
                W5432 = AE_SEL16_5432(Wz654, W3210);

                AE_MULAAAAQ16(S0, W0zzz, Y0123);
                AE_MULAAAAQ16(S1, W10zz, Y0123);
                AE_MULAAAAQ16(S2, W210z, Y0123);
                AE_MULAAAAQ16(S3, W3210, Y0123);
                AE_MULAAAAQ16(S4, W4321, Y0123); AE_MULAAAAQ16(S4, W0zzz, Y4567);
                AE_MULAAAAQ16(S5, W5432, Y0123); AE_MULAAAAQ16(S5, W10zz, Y4567);
                AE_MULAAAAQ16(S6, W6543, Y0123); AE_MULAAAAQ16(S6, W210z, Y4567);
                AE_MULAAAAQ16(S7, Wz654, Y0123); AE_MULAAAAQ16(S7, W3210, Y4567);
            }
            R32 = AE_TRUNCA32X2F64S(S0, S1, 48-(15+rsh));
            T32 = AE_TRUNCA32X2F64S(S2, S3, 48-(15+rsh));
            z0 = AE_ROUND16X4F32SASYM(R32, T32);
            AE_SA16X4_IP(z0, alZ, pZ);
            R32 = AE_TRUNCA32X2F64S(S4, S5, 48-(15+rsh));
            T32 = AE_TRUNCA32X2F64S(S6, S7, 48-(15+rsh));
            z0 = AE_ROUND16X4F32SASYM(R32, T32);
            AE_SA16X4_IP(z0, alZ, pZ);
        }
        /* Next samples */
        pYrd0 = (ae_int16x4 *)(y + n0*Q);
        pYrd1 = pYrd0 + 2;
        for (j=0; j<((Q>>3)-1); j++)
        {
            pWrd1 = (ae_int16x4 *)w + (M-m1)*6;
            pWrd0 = pWrd1 + 4;
            S0 = S1 = S2 = S3 = S4 = S5 = S6 = S7 = AE_ZERO64();
            __Pragma("loop_count min=1, max=11");
            for (n=0; n<n1-n0; n++)
            {
                ae_int16x4 W6543, W210z, Wz654, W3210;
                AE_L16X4_IP(Y0123, pYrd0, sizeof(ae_int16x4));
                AE_L16X4_XC(Y4567, pYrd0, Q*sizeof(int16_t) - sizeof(ae_int16x4));
                AE_L16X4_IP(Y89AB, pYrd1, sizeof(ae_int16x4));
                AE_L16X4_XC(YCDEF, pYrd1, Q*sizeof(int16_t) - sizeof(ae_int16x4));
                Y2345 = AE_SEL16_5432(Y0123, Y4567);
                Y6789 = AE_SEL16_5432(Y4567, Y89AB);
                YABCD = AE_SEL16_5432(Y89AB, YCDEF);

                AE_L16X4_IP(W6543, pWrd0, sizeof(ae_int16x4));
                AE_L16X4_IP(W210z, pWrd1, sizeof(ae_int16x4));
                AE_MULAAAAQ16(S0, W6543, Y2345); AE_MULAAAAQ16(S0, W210z, Y6789);
                AE_MULAAAAQ16(S2, W6543, Y4567); AE_MULAAAAQ16(S2, W210z, Y89AB);
                AE_MULAAAAQ16(S4, W6543, Y6789); AE_MULAAAAQ16(S4, W210z, YABCD);
                AE_MULAAAAQ16(S6, W6543, Y89AB); AE_MULAAAAQ16(S6, W210z, YCDEF);

                AE_L16X4_XP(Wz654, pWrd0, (6-1)*sizeof(ae_int16x4));
                AE_L16X4_XP(W3210, pWrd1, (6-1)*sizeof(ae_int16x4));
                AE_MULAAAAQ16(S1, Wz654, Y2345); AE_MULAAAAQ16(S1, W3210, Y6789);
                AE_MULAAAAQ16(S3, Wz654, Y4567); AE_MULAAAAQ16(S3, W3210, Y89AB);
                AE_MULAAAAQ16(S5, Wz654, Y6789); AE_MULAAAAQ16(S5, W3210, YABCD);
                AE_MULAAAAQ16(S7, Wz654, Y89AB); AE_MULAAAAQ16(S7, W3210, YCDEF);
            }
            R32 = AE_TRUNCA32X2F64S(S0, S1, 48-(15+rsh));
            T32 = AE_TRUNCA32X2F64S(S2, S3, 48-(15+rsh));
            z0 = AE_ROUND16X4F32SASYM(R32, T32);
            AE_SA16X4_IP(z0, alZ, pZ);
            R32 = AE_TRUNCA32X2F64S(S4, S5, 48-(15+rsh));
            T32 = AE_TRUNCA32X2F64S(S6, S7, 48-(15+rsh));
            z0 = AE_ROUND16X4F32SASYM(R32, T32);
            AE_SA16X4_IP(z0, alZ, pZ);
        }
        /* Last N-1 samples of the i-th row */
        {
            pYrd1 = pYrd0 + 1;
            pWrd1 = (ae_int16x4 *)w + (M-m1)*6 + 2;
            pWrd0 = pWrd1 + 2;
            S0 = S1 = S2 = S3 = S4 = S5 = AE_ZERO64();
            __Pragma("loop_count min=1, max=11");
            __Pragma("no_unroll");
            for (n=0; n<n1-n0; n++)
            {
                ae_int16x4 Wzzz6, Wzz65, Wz654, W6543, W5432, W4321;
                AE_L16X4_XP(Y0123, pYrd0, Q*sizeof(int16_t));
                AE_L16X4_XP(Y4567, pYrd1, Q*sizeof(int16_t));

                AE_L16X4_IP(W6543, pWrd0, sizeof(ae_int16x4));
                AE_L16X4_XP(Wz654, pWrd0, (6-1)*sizeof(ae_int16x4));
                Wzz65 = AE_SEL16_5432(AE_ZERO16(), W6543);
                Wzzz6 = AE_SEL16_5432(AE_ZERO16(), Wz654);
                AE_L16X4_IP(W4321, pWrd1, sizeof(ae_int16x4));
                AE_L16X4_XP(W5432, pWrd1, (6-1)*sizeof(ae_int16x4));

                AE_MULAAAAQ16(S0, Wzz65, Y0123); AE_MULAAAAQ16(S0, W4321, Y4567);
                AE_MULAAAAQ16(S1, Wzzz6, Y0123); AE_MULAAAAQ16(S1, W5432, Y4567);
                AE_MULAAAAQ16(S2, W6543, Y4567);
                AE_MULAAAAQ16(S3, Wz654, Y4567);
                AE_MULAAAAQ16(S4, Wzz65, Y4567);
                AE_MULAAAAQ16(S5, Wzzz6, Y4567);
            }
            R32 = AE_TRUNCA32X2F64S(S0, S1, 48-(15+rsh));
            T32 = AE_TRUNCA32X2F64S(S2, S3, 48-(15+rsh));
            z0 = AE_ROUND16X4F32SASYM(R32, T32);
            AE_SA16X4_IP(z0, alZ, pZ);
            AE_SA64POS_FP(alZ, pZ);
            R32 = AE_TRUNCA32X2F64S(S5, S4, 48-(15+rsh));
            R32 = AE_MOVINT32X2_FROMINT16X4(AE_ROUND16X4F32SASYM(R32, R32));
            AE_S32_L_IP(R32, castxcc(ae_int32,pZ), sizeof(int16_t)*2);
        }
    }
#   undef M
#   undef N
}

// scratch allocatation functions. return required scratch size in bytes
size_t conv2d_11x7_16x16_getScratchSize(int P, int Q)
{
    return 11*6*sizeof(ae_int16x4);
} // MxN=11x7
