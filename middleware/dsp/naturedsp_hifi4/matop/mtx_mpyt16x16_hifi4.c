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
void mtx_mpyt16x16 (  void* pScr,
                     int16_t* restrict z,
               const int16_t* restrict x,
               const int16_t* restrict yt,
               int M, int N, int P, int lsh )
{
    int m, n, p;
    const ae_int16x4 * restrict px;
    const ae_int16x4 * restrict px0;
    const ae_int16x4 * restrict px1;
    const ae_int16x4 * restrict py;
    const ae_int16x4 * restrict py0;
    const ae_int16x4 * restrict py1;
    const ae_int16x4 * restrict py2;
    const ae_int16x4 * restrict py3;
          ae_int16x4 * restrict pz;
          ae_int16x4 * restrict pz0;
          ae_int16x4 * restrict pz1;
    ae_int16x4 t0, t1, t2, t3;
    ae_int16x4 x0, x1;
    ae_int16x4 y0, y1, y2, y3;
    ae_valign vy0, vy1, vy2, vy3;
    int16_t * restrict scr = (int16_t *)pScr;
    int16_t * restrict scr_temp;

    if ((M <= 0) || (P <= 0)) return;
    if (N <= 0)
    {
        for (m = 0; m < M * P; m++) z[m] = 0;
        return;
    }

    NASSERT_ALIGN8(pScr);
    if (N & 3)
    {
        pz = (ae_int16x4 *)scr + (N&~3);
        AE_S16X4_IP(AE_ZERO16(), pz, sizeof(ae_int16x4));
        AE_S16X4_IP(AE_ZERO16(), pz, sizeof(ae_int16x4));
        AE_S16X4_IP(AE_ZERO16(), pz, sizeof(ae_int16x4));
        AE_S16X4_IP(AE_ZERO16(), pz, sizeof(ae_int16x4));
    }


    for (p = 0; p < (P >> 2); p++)
    {
        py0 = (const ae_int16x4 *)yt;
        py1 = (const ae_int16x4 *)XT_ADDX2(N, (uintptr_t)py0);
        py2 = (const ae_int16x4 *)XT_ADDX2(N, (uintptr_t)py1);
        py3 = (const ae_int16x4 *)XT_ADDX2(N, (uintptr_t)py2);
        pz = (ae_int16x4 *)scr;
        vy0 = AE_LA64_PP(py0);
        vy1 = AE_LA64_PP(py1);
        vy2 = AE_LA64_PP(py2);
        vy3 = AE_LA64_PP(py3);
        for (n = 0; n < (N >> 2); n++)
        {
            AE_LA16X4_IP(y0, vy0, py0);
            AE_LA16X4_IP(y1, vy1, py1);
            AE_LA16X4_IP(y2, vy2, py2);
            AE_LA16X4_IP(y3, vy3, py3);
            AE_S16X4_IP(y0, pz, sizeof(ae_int16x4));
            AE_S16X4_IP(y1, pz, sizeof(ae_int16x4));
            AE_S16X4_IP(y2, pz, sizeof(ae_int16x4));
            AE_S16X4_IP(y3, pz, sizeof(ae_int16x4));
        }
        scr_temp = (int16_t *)pz;
        for (n <<= 2; n < N; n++)
        {
			scr_temp[0] = yt[0*N+n];
			scr_temp[4] = yt[1*N+n];
			scr_temp[8] = yt[2*N+n];
			scr_temp[12] = yt[3*N+n];
            scr_temp = scr_temp + 1;
        }

        px = (const ae_int16x4 *)x;
        pz = (ae_int16x4 *)z;
        pz0 = (ae_int16x4 *)pz;
        pz1 = (ae_int16x4 *)XT_ADDX2(P, (uintptr_t)pz0);
        for (m = 0; m < (M >> 1); m++)
        {
            ae_int32x2 a0, a1, a2, a3;
            ae_int64 B0, B1, B2, B3, B4, B5, B6, B7;
            py = (const ae_int16x4 *)scr;
            px0 = (const ae_int16x4 *)px;
            px1 = (const ae_int16x4 *)XT_ADDX2(N, (uintptr_t)px);
            vy0 = AE_LA64_PP(px0);
            vy1 = AE_LA64_PP(px1);

            B0 = B1 = B2 = B3 = B4 = B5 = B6 = B7 = AE_ZERO64();
            for (n = 0; n < N; n += 4)
            {
                AE_L16X4_IP(y0, py, sizeof(ae_int16x4));
                AE_L16X4_IP(y1, py, sizeof(ae_int16x4));
                AE_L16X4_IP(y2, py, sizeof(ae_int16x4));
                AE_L16X4_IP(y3, py, sizeof(ae_int16x4));

                AE_LA16X4_IP(x0, vy0, px0);
                AE_LA16X4_IP(x1, vy1, px1);

                AE_MULAAAAQ16(B0, x0, y0);
                AE_MULAAAAQ16(B1, x0, y1);
                AE_MULAAAAQ16(B2, x0, y2);
                AE_MULAAAAQ16(B3, x0, y3);
                AE_MULAAAAQ16(B4, x1, y0);
                AE_MULAAAAQ16(B5, x1, y1);
                AE_MULAAAAQ16(B6, x1, y2);
                AE_MULAAAAQ16(B7, x1, y3);
            }
            a0 = AE_TRUNCA32X2F64S(B0, B1, lsh + 33);
            a1 = AE_TRUNCA32X2F64S(B2, B3, lsh + 33);
            a2 = AE_TRUNCA32X2F64S(B4, B5, lsh + 33);
            a3 = AE_TRUNCA32X2F64S(B6, B7, lsh + 33);
            t0 = AE_ROUND16X4F32SASYM(a0, a1);
            t1 = AE_ROUND16X4F32SASYM(a2, a3);
            vy2 = AE_ZALIGN64();
            vy3 = AE_ZALIGN64();
            AE_SA16X4_IP(t0, vy2, pz0);
            AE_SA16X4_IP(t1, vy3, pz1);
            AE_SA64POS_FP(vy2, pz0);
            AE_SA64POS_FP(vy3, pz1);
            pz0 = (ae_int16x4 *)XT_ADDX4(P - 2, (uintptr_t)pz0);
            pz1 = (ae_int16x4 *)XT_ADDX4(P - 2, (uintptr_t)pz1);

            px = (const ae_int16x4 *)XT_ADDX4(N, (uintptr_t)px);
        }
        if (M & 1)
        {
            ae_int32x2 a0, a1;
            ae_int64 B0, B1, B2, B3;
            py = (const ae_int16x4 *)scr;
            px0 = (const ae_int16x4 *)px;
            vy0 = AE_LA64_PP(px0);

            B0 = B1 = B2 = B3 = AE_ZERO64();
            for (n = 0; n < N; n += 4)
            {
                AE_L16X4_IP(y0, py, sizeof(ae_int16x4));
                AE_L16X4_IP(y1, py, sizeof(ae_int16x4));
                AE_L16X4_IP(y2, py, sizeof(ae_int16x4));
                AE_L16X4_IP(y3, py, sizeof(ae_int16x4));

                AE_LA16X4_IP(x0, vy0, px0);

                AE_MULAAAAQ16(B0, x0, y0);
                AE_MULAAAAQ16(B1, x0, y1);
                AE_MULAAAAQ16(B2, x0, y2);
                AE_MULAAAAQ16(B3, x0, y3);
            }
            a0 = AE_TRUNCA32X2F64S(B0, B1, lsh + 33);
            a1 = AE_TRUNCA32X2F64S(B2, B3, lsh + 33);
            t0 = AE_ROUND16X4F32SASYM(a0, a1);
            vy2 = AE_ZALIGN64();
            AE_SA16X4_IP(t0, vy2, pz0);
            AE_SA64POS_FP(vy2, pz0);
        }
        z += 4;
        yt += 4*N;
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////
    if (P & 3)
    {
        int offs1, offs2;/* offs used to avoid outbound reads/writes */
        int NN;
        offs1 = (int)sizeof(int16_t);
        offs2 = (int)(2 * sizeof(int16_t));
        p <<= 2;
        XT_MOVLTZ(offs1, 0, P - p - 2);
        XT_MOVLTZ(offs2, 0, P - p - 3);

        py0 = (const ae_int16x4 *)yt;
        NN=N;XT_MOVLTZ(NN, 0, P - p - 2);
        py1 = (const ae_int16x4 *)XT_ADDX2(NN, (uintptr_t)py0);
        NN=N;XT_MOVLTZ(NN, 0, P - p - 3);
        py2 = (const ae_int16x4 *)XT_ADDX2(NN, (uintptr_t)py1);
        pz = (ae_int16x4 *)scr;
        if ((N>>2)>0)
        {
            vy0 = AE_LA64_PP(py0);
            vy1 = AE_LA64_PP(py1);
            vy2 = AE_LA64_PP(py2);
            for (n = 0; n < (N >> 2); n++)
            {
                AE_LA16X4_IP(y0, vy0, py0);
                AE_LA16X4_IP(y1, vy1, py1);
                AE_LA16X4_IP(y2, vy2, py2);
                AE_S16X4_IP(y0, pz, sizeof(ae_int16x4));
                AE_S16X4_IP(y1, pz, sizeof(ae_int16x4));
                AE_S16X4_IP(y2, pz, 2 * sizeof(ae_int16x4));
            }
        }
        for (n =0; n < (N&3); n++)
        {
            AE_L16_IP(y0, castxcc(ae_int16,py0), sizeof(int16_t));
            AE_L16_IP(y1, castxcc(ae_int16,py1), sizeof(int16_t));
            AE_L16_IP(y2, castxcc(ae_int16,py2), sizeof(int16_t));
            AE_S16_0_I (y1,(ae_int16*)pz, 4*sizeof(int16_t));
            AE_S16_0_X (y2,(ae_int16*)pz, 8*sizeof(int16_t));
            AE_S16_0_IP(y0,castxcc(ae_int16,pz), 1*sizeof(int16_t));
        }

        px = (const ae_int16x4 *)x;
        pz = (ae_int16x4 *)z;
        pz0 = (ae_int16x4 *)pz;
        pz1 = (ae_int16x4 *)XT_ADDX2(P, (uintptr_t)pz0);
        for (m = 0; m < (M >> 1); m++)
        {
            ae_int32x2 a0, a1, a2, a3;
            ae_int64 B0, B1, B2, B3, B4, B5, B6, B7;
            py = (const ae_int16x4 *)scr;
            px0 = (const ae_int16x4 *)px;
            px1 = (const ae_int16x4 *)XT_ADDX2(N, (uintptr_t)px);
            vy0 = AE_LA64_PP(px0);
            vy1 = AE_LA64_PP(px1);

            B0 = B1 = B2 = B3 = B4 = B5 = B6 = B7 = AE_ZERO64();
            for (n = 0; n < N; n += 4)
            {
                AE_L16X4_IP(y0, py, sizeof(ae_int16x4));
                AE_L16X4_IP(y1, py, sizeof(ae_int16x4));
                AE_L16X4_IP(y2, py, 2 * sizeof(ae_int16x4));

                AE_LA16X4_IP(x0, vy0, px0);
                AE_LA16X4_IP(x1, vy1, px1);

                AE_MULAAAAQ16(B0, x0, y0);
                AE_MULAAAAQ16(B1, x0, y1);
                AE_MULAAAAQ16(B2, x0, y2);
                AE_MULAAAAQ16(B4, x1, y0);
                AE_MULAAAAQ16(B5, x1, y1);
                AE_MULAAAAQ16(B6, x1, y2);
            }
            a0 = AE_TRUNCA32X2F64S(B0, B1, lsh + 33);
            a1 = AE_TRUNCA32X2F64S(B2, B2, lsh + 33);
            a2 = AE_TRUNCA32X2F64S(B4, B5, lsh + 33);
            a3 = AE_TRUNCA32X2F64S(B6, B6, lsh + 33);
            t0 = AE_ROUND16X4F32SASYM(a0, a1);
            t1 = AE_ROUND16X4F32SASYM(a2, a3);

            AE_S16_0_X(t0, (ae_int16 *)pz0, offs2);
            t2 = AE_SEL16_5432(t0, t0);
            AE_S16_0_X(t2, (ae_int16 *)pz0, offs1);
            t2 = AE_SEL16_6543(t0, t0);
            AE_S16_0_X(t2, (ae_int16 *)pz0, 0);

            AE_S16_0_X(t1, (ae_int16 *)pz1, offs2);
            t3 = AE_SEL16_5432(t1, t1);
            AE_S16_0_X(t3, (ae_int16 *)pz1, offs1);
            t3 = AE_SEL16_6543(t1, t1);
            AE_S16_0_X(t3, (ae_int16 *)pz1, 0);

            pz0 = (ae_int16x4 *)XT_ADDX4(P, (uintptr_t)pz0);
            pz1 = (ae_int16x4 *)XT_ADDX4(P, (uintptr_t)pz1);

            px = (const ae_int16x4 *)XT_ADDX4(N, (uintptr_t)px);
        }
        if (M & 1)
        {
            ae_int32x2 a0, a1;
            ae_int64 B0, B1, B2;
            py = (const ae_int16x4 *)scr;
            px0 = (const ae_int16x4 *)px;
            vy0 = AE_LA64_PP(px0);

            B0 = B1 = B2 = AE_ZERO64();

			AE_LA16X4_IP(x0, vy0, px0);
            AE_L16X4_IP(y0, py, sizeof(ae_int16x4));
			for (n = 0; n < (N-4); n += 4)
            {
                AE_L16X4_IP(y1, py, sizeof(ae_int16x4));
                AE_L16X4_IP(y2, py, 2 * sizeof(ae_int16x4));

                AE_MULAAAAQ16(B0, x0, y0);
                AE_MULAAAAQ16(B1, x0, y1);
                AE_MULAAAAQ16(B2, x0, y2);

				AE_LA16X4_IP(x0, vy0, px0);
                AE_L16X4_IP(y0, py, sizeof(ae_int16x4));
            }
            AE_L16X4_IP(y1, py, sizeof(ae_int16x4));
            AE_L16X4_IP(y2, py, 2 * sizeof(ae_int16x4));

            AE_MULAAAAQ16(B0, x0, y0);
            AE_MULAAAAQ16(B1, x0, y1);
            AE_MULAAAAQ16(B2, x0, y2);
            a0 = AE_TRUNCA32X2F64S(B0, B1, lsh + 33);
            a1 = AE_TRUNCA32X2F64S(B2, B2, lsh + 33);
            t0 = AE_ROUND16X4F32SASYM(a0, a1);

            AE_S16_0_X(t0, (ae_int16 *)pz0, offs2);
            t2 = AE_SEL16_5432(t0, t0);
            AE_S16_0_X(t2, (ae_int16 *)pz0, offs1);
            t2 = AE_SEL16_6543(t0, t0);
            AE_S16_0_X(t2, (ae_int16 *)pz0, 0);
        }
    }
}

size_t mtx_mpyt16x16_getScratchSize ( int M, int N, int P )
{
    (void)M; (void)P;
	return N<=0 ? 0: ((((N)+3)&(~3))*4*sizeof(int16_t));
} /* mtx_mpyt16x16_getScratchSize() */
