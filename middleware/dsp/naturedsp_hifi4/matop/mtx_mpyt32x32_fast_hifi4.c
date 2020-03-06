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
void mtx_mpyt32x32_fast ( void* pScr,
	                      int32_t* restrict z,
                    const int32_t* restrict x,
                    const int32_t* restrict yt,
                    int M, int N, int P, int lsh )
{ 
    int m, n, p;
    const ae_int32x2 * restrict px0;
    const ae_int32x2 * restrict px1;
    const ae_int32x2 * restrict px2;
    const ae_int32x2 * restrict px3;
    const ae_int32x2 * restrict py0;
    const ae_int32x2 * restrict py1;
          ae_int32x2 * restrict pz;
    ae_f64 B0, B1, B2, B3, B4, B5, B6, B7;
    ae_int32x2 X0, X1, X2, X3, Y0, Y1;

    NASSERT((N & 3) == 0);
    NASSERT((M & 3) == 0);
    NASSERT((P & 3) == 0);
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(yt);
    NASSERT_ALIGN8(z);

    if ((M <= 0) || (P <= 0)) return;
    if (N <= 0)
    {
        for (m = 0; m < M * P; m++) z[m] = 0;
        return;
    }

    __Pragma("loop_count min=1");
    for (p = 0; p < (P>>1); p++)
    {
        pz = (ae_int32x2 *)z;
        px3 = (const ae_int32x2 *)x;

        __Pragma("loop_count min=1");
        for (m = 0; m < (M>>2); m++)
        {
            px0 = (const ae_int32x2 *)px3;
            px1 = (const ae_int32x2 *)((int32_t *)px0 + N);
            px2 = (const ae_int32x2 *)((int32_t *)px1 + N);
            px3 = (const ae_int32x2 *)((int32_t *)px2 + N);
			py0 = (const ae_int32x2 *)yt;
			py1 = (const ae_int32x2 *)((int32_t *)py0 + N);

            B0 = B1 = B2 = B3 = B4 = B5 = B6 = B7 = AE_ZERO64();

            /* Innermost loop: compute 2 values for 4 rows */
            __Pragma("loop_count min=2, factor=2");
            for (n = 0; n < (N >> 1); n++)
            {
                /* load data from 'x' */
                AE_L32X2_IP(X0, px0, sizeof(ae_int32x2));
                AE_L32X2_IP(X1, px1, sizeof(ae_int32x2));
                AE_L32X2_IP(X2, px2, sizeof(ae_int32x2));
                AE_L32X2_IP(X3, px3, sizeof(ae_int32x2));
                /* load data from 'y' */
                AE_L32X2_IP(Y0, py0, sizeof(ae_int32x2));
                AE_L32X2_IP(Y1, py1, sizeof(ae_int32x2));
                /* perform multiplications */
                AE_MULAAFD32RA_HH_LL(B0, X0, Y0);
                AE_MULAAFD32RA_HH_LL(B1, X0, Y1);
                AE_MULAAFD32RA_HH_LL(B2, X1, Y0);
                AE_MULAAFD32RA_HH_LL(B3, X1, Y1);
                AE_MULAAFD32RA_HH_LL(B4, X2, Y0);
                AE_MULAAFD32RA_HH_LL(B5, X2, Y1);
                AE_MULAAFD32RA_HH_LL(B6, X3, Y0);
                AE_MULAAFD32RA_HH_LL(B7, X3, Y1);
            }
            /* format values */
            X0 = AE_TRUNCA32X2F64S(B0, B1, 16 + lsh);
            X1 = AE_TRUNCA32X2F64S(B2, B3, 16 + lsh);
            X2 = AE_TRUNCA32X2F64S(B4, B5, 16 + lsh);
            X3 = AE_TRUNCA32X2F64S(B6, B7, 16 + lsh);
            /* save values */
            AE_S32X2_XP(X0, pz, P*sizeof(int32_t));
            AE_S32X2_XP(X1, pz, P*sizeof(int32_t));
            AE_S32X2_XP(X2, pz, P*sizeof(int32_t));
            AE_S32X2_XP(X3, pz, P*sizeof(int32_t));

        }
        yt += 2*N;
        z += 2;
    }
}

size_t mtx_mpyt32x32_fast_getScratchSize ( int M, int N, int P )
{
	return 0;
} /* mtx_mpyt32x32_fast_getScratchSize() */
