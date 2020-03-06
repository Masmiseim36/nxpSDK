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
void mtx_vecmpy32x32 (  int32_t* restrict z,
               const int32_t* restrict x,
               const int32_t* restrict y,
               int M, int N, int lsh)
{
    ae_valign ax0,ax1,ay,az;
    const ae_int32x2 * restrict px0;
    const ae_int32x2 * restrict px1;
    const ae_int32x2 * restrict py;
          ae_int32x2 * restrict pz;
    ae_int32x2 Y0,X0,X1;
    xtbool2 bmask;
    ae_f64 B0,B1;
    int m,n;
    if (N<=0 || M<=0)    /* exceptional situation */
    {
        for (m=0; m<M; m++) z[m]=0;
        return ;
    }

    bmask=AE_MOVBA2(N<<1);  // 0 for even, 2 for odd
    az=AE_ZALIGN64();
    px0=(const ae_int32x2 *)x;
    py =(const ae_int32x2 *)y;
    pz =(      ae_int32x2 *)z;
    for(m=0; m<(M&~1); m+=2)
    {
        B0=B1=AE_ZERO64();
        py =(const ae_int32x2 *)y;
        px0=(const ae_int32x2 *)(x+m*N);
        px1=(const ae_int32x2 *)(x+(m+1)*N);
        ax0=AE_LA64_PP(px0);
        ax1=AE_LA64_PP(px1);
        ay =AE_LA64_PP(py);
        for(n=0; n<(N>>1); n++)
        {
            AE_LA32X2_IP(Y0,ay ,py);
            AE_LA32X2_IP(X0,ax0,px0);
            AE_LA32X2_IP(X1,ax1,px1);
            AE_MULAAFD32RA_HH_LL(B0,X0,Y0);
            AE_MULAAFD32RA_HH_LL(B1,X1,Y0);
        }
        AE_LA32X2_IP(Y0,ay ,py);
        AE_LA32X2_IP(X0,ax0,px0);
        AE_LA32X2_IP(X1,ax1,px1);
        AE_MOVF32X2(Y0,AE_ZERO32(),bmask);
        AE_MULAAFD32RA_HH_LL(B0,X0,Y0);
        AE_MULAAFD32RA_HH_LL(B1,X1,Y0);

        X0=AE_TRUNCA32X2F64S(B0,B1,16+lsh);
        AE_SA32X2_IP(X0,az,pz);
    } 
    AE_SA64POS_FP(az,pz);
    if(M&1)
    {
        B0=B1=AE_ZERO64();
        py =(const ae_int32x2 *)y;
        px0=(const ae_int32x2 *)(x+m*N);
        ax0=AE_LA64_PP(px0);
        ay =AE_LA64_PP(py);
        for(n=0; n<(N>>1); n++)
        {
            AE_LA32X2_IP(Y0,ay ,py);
            AE_LA32X2_IP(X0,ax0,px0);
            AE_MULAAFD32RA_HH_LL(B0,X0,Y0);
        }
        AE_LA32X2_IP(Y0,ay ,py);
        AE_LA32X2_IP(X0,ax0,px0);
        AE_MOVF32X2(Y0,AE_ZERO32(),bmask);
        AE_MULAAFD32RA_HH_LL(B0,X0,Y0);

        X0=AE_TRUNCA32X2F64S(B0,B0,16+lsh);
        AE_S32_L_IP(X0,castxcc(ae_int32,pz),4);
    }
}
