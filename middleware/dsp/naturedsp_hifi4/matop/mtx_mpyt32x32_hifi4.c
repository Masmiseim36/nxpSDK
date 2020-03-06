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
void mtx_mpyt32x32 (  void* pScr,
                     int32_t* restrict z,
               const int32_t* restrict x,
               const int32_t* restrict yt,
               int M, int N, int P, int lsh )
{
    const ae_int32x2 * restrict px;
    const ae_int32x2 * restrict px0;
    const ae_int32x2 * restrict px1;
    const ae_int32x2 * restrict py;
    const ae_int32x2 * restrict py0;
    const ae_int32x2 * restrict py1;
    ae_int32x2 * restrict pz;
    ae_int32x2 * restrict pz0;
    ae_int32x2 * restrict pz1;
    int32_t * restrict scr = (int32_t *)pScr;
    ae_valign ay0, ay1, ax0, ax1, az0, az1;
    ae_int32x2 vx0, vx1,
        vy0, vy1, vt0, vt1,
        vz0, vz1;
    ae_f64   ACC00, ACC01, ACC10, ACC11;
    int n, m, p;

    if ((M <= 0) || (P <= 0)) return;
    if (N <= 0)
    {
        for (m = 0; m < M * P; m++) z[m] = 0;
        return;
    }


    for (p = 0; p < (P >> 1); p++)
    {
        py0 = (const ae_int32x2 *)yt;
        py1 = (const ae_int32x2 *)XT_ADDX4(N, (uintptr_t)py0);
        pz = (ae_int32x2 *)pScr;
        ay0 = AE_LA64_PP(py0);
        ay1 = AE_LA64_PP(py1);
        for (n = 0; n < (N >> 1); n++)
        {
            AE_LA32X2_IP(vt0, ay0, py0);
            AE_LA32X2_IP(vt1, ay1, py1);
            AE_S32X2_IP(vt0, pz, sizeof(ae_int32x2));
            AE_S32X2_IP(vt1, pz, sizeof(ae_int32x2));
        }
        if (N & 1)
        {
            AE_LA32X2_IP(vt0, ay0, py0);
            AE_LA32X2_IP(vt1, ay1, py1);
            vy0 = AE_SEL32_HH(vt0, AE_ZERO32());
            vy1 = AE_SEL32_HH(vt1, AE_ZERO32());
            AE_S32X2_IP(vy0, pz, sizeof(ae_int32x2));
            AE_S32X2_IP(vy1, pz, sizeof(ae_int32x2));
        }

        pz = (ae_int32x2 *)z;
        pz0 = (ae_int32x2 *)pz;
        pz1 = (ae_int32x2 *)XT_ADDX4(P, (uintptr_t)pz0);
        px = (const ae_int32x2 *)(x);

        for (m = 0; m < (M >> 1); m++)
        {
            px0 = (const ae_int32x2 *)px;
            px1 = (const ae_int32x2 *)XT_ADDX4(N, (uintptr_t)px);
            ax0 = AE_LA64_PP(px0);
            ax1 = AE_LA64_PP(px1);
            py = (const ae_int32x2 *)pScr;
            py0 = (const ae_int32x2 *)py;
            py1 = (const ae_int32x2 *)XT_ADDX8(1, (uintptr_t)py);

            ACC00 = ACC01 = ACC10 = ACC11 = AE_ZERO64();

	        for (n = 0; n < N; n += 2)
            {
                /* load data from 'x' */
                AE_LA32X2_IP(vx0, ax0, px0);
                AE_LA32X2_IP(vx1, ax1, px1);
                /* load data from 'y' */
                AE_L32X2_IP(vy0, py0, 2*sizeof(ae_int32x2));
                AE_L32X2_IP(vy1, py1, 2*sizeof(ae_int32x2));
                /* perform multiplications */
                AE_MULAAFD32RA_HH_LL(ACC00, vx0, vy0);
                AE_MULAAFD32RA_HH_LL(ACC01, vx0, vy1);
                AE_MULAAFD32RA_HH_LL(ACC10, vx1, vy0);
                AE_MULAAFD32RA_HH_LL(ACC11, vx1, vy1);
            }
            /* format values */
            vz0 = AE_TRUNCA32X2F64S(ACC00, ACC01, 16 + lsh);
            vz1 = AE_TRUNCA32X2F64S(ACC10, ACC11, 16 + lsh);
            /* save values */
            az0 = AE_ZALIGN64();
            az1 = AE_ZALIGN64();
            AE_SA32X2_IP(vz0, az0, pz0);
            AE_SA32X2_IP(vz1, az1, pz1);
            AE_SA64POS_FP(az0, pz0);
            AE_SA64POS_FP(az1, pz1);
            pz0 = (ae_int32x2 *)XT_ADDX8(P - 1, (uintptr_t)pz0);
            pz1 = (ae_int32x2 *)XT_ADDX8(P - 1, (uintptr_t)pz1);

            px = (const ae_int32x2 *)XT_ADDX8(N, (uintptr_t)px);
        }
        if (M & 1)
        {
            px0 = (const ae_int32x2 *)px;
            ax0 = AE_LA64_PP(px0);
            py = (const ae_int32x2 *)pScr;

            ACC00 = ACC01 = AE_ZERO64();

            for (n = 0; n < N; n += 2)
            {
                /* load data from 'x' */
                AE_LA32X2_IP(vx0, ax0, px0);
                /* load data from 'y' */
                AE_L32X2_IP(vy0, py, sizeof(ae_int32x2));
                AE_L32X2_IP(vy1, py, sizeof(ae_int32x2));
                /* perform multiplications */
                AE_MULAAFD32RA_HH_LL(ACC00, vx0, vy0);
                AE_MULAAFD32RA_HH_LL(ACC01, vx0, vy1);
            }
            /* format values */
            vz0 = AE_TRUNCA32X2F64S(ACC00, ACC01, 16 + lsh);
            /* save values */
            az0 = AE_ZALIGN64();
            AE_SA32X2_IP(vz0, az0, pz0);
            AE_SA64POS_FP(az0, pz0);
        }

        yt += 2*N;
        z += 2;
    }

    if (P & 1)
    {
        for (n = 0; n < N; n++) scr[n] = yt[n];
        if (N & 1) scr[n] = 0;

        pz = (ae_int32x2 *)z;
        pz0 = (ae_int32x2 *)pz;
        pz1 = (ae_int32x2 *)XT_ADDX4(P, (uintptr_t)pz0);
        px = (const ae_int32x2 *)(x);

        for (m = 0; m < (M >> 1); m++)
        {
            px0 = (const ae_int32x2 *)px;
            px1 = (const ae_int32x2 *)XT_ADDX4(N, (uintptr_t)px);
            ax0 = AE_LA64_PP(px0);
            ax1 = AE_LA64_PP(px1);
            py = (const ae_int32x2 *)pScr;

            ACC00 = ACC10 = AE_ZERO64();

            for (n = 0; n < N; n += 2)
            {
                /* load data from 'x' */
                AE_LA32X2_IP(vx0, ax0, px0);
                AE_LA32X2_IP(vx1, ax1, px1);
                /* load data from 'y' */
                AE_L32X2_IP(vy0, py, sizeof(ae_int32x2));
                /* perform multiplications */
                AE_MULAAFD32RA_HH_LL(ACC00, vx0, vy0);
                AE_MULAAFD32RA_HH_LL(ACC10, vx1, vy0);
            }
            /* format values */
            vz0 = AE_TRUNCA32X2F64S(ACC00, ACC00, 16 + lsh);
            vz1 = AE_TRUNCA32X2F64S(ACC10, ACC10, 16 + lsh);
            /* save values */
            AE_S32_L_I(vz0, (ae_int32 *)pz0, 0);
            AE_S32_L_I(vz1, (ae_int32 *)pz1, 0);
            pz0 = (ae_int32x2 *)XT_ADDX8(P, (uintptr_t)pz0);
            pz1 = (ae_int32x2 *)XT_ADDX8(P, (uintptr_t)pz1);

            px = (const ae_int32x2 *)XT_ADDX8(N, (uintptr_t)px);
        }
        if (M & 1)
        {
            px0 = (const ae_int32x2 *)px;
            ax0 = AE_LA64_PP(px0);
            py = (const ae_int32x2 *)pScr;

            ACC00 = AE_ZERO64();

            /* pre-load data from 'x' */
            AE_LA32X2_IP(vx0, ax0, px0);
            /* pre-load data from 'y' */
            AE_L32X2_IP(vy0, py, sizeof(ae_int32x2));
            for (n = 0; n < N-2; n += 2)
            {
                /* perform multiplications */
                AE_MULAAFD32RA_HH_LL(ACC00, vx0, vy0);
                /* load data from 'x' */
                AE_LA32X2_IP(vx0, ax0, px0);
                /* load data from 'y' */
                AE_L32X2_IP(vy0, py, sizeof(ae_int32x2));
            }
            /* perform multiplications */
            AE_MULAAFD32RA_HH_LL(ACC00, vx0, vy0);
            /* format values */
            vz0 = AE_TRUNCA32X2F64S(ACC00, ACC00, 16 + lsh);
            /* save values */
            AE_S32_L_I(vz0, (ae_int32 *)pz0, 0);
        }
    }
}

size_t mtx_mpyt32x32_getScratchSize ( int M, int N, int P )
{
	return N<=0? 0: ((((N)+1)&(~1))*2*sizeof(int32_t));
} /* mtx_mpyt32x32_getScratchSize() */
