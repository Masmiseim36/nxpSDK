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
void mtx_vecmpy16x16_fast (  int16_t* restrict z,
               const int16_t* restrict x,
               const int16_t* restrict y,
               int M, int N, int lsh)
{
    ae_valign az;
    const ae_int16x4 * restrict px0;
    const ae_int16x4 * restrict px1;
    const ae_int16x4 * restrict px2;
    const ae_int16x4 * restrict px3;
    const ae_int16x4 * restrict py;
    const ae_int16x4 * restrict py1;
          ae_int16x4 * restrict pz;
    ae_int16x4 X0,X1,X2,X3,Y0,Y1;
    ae_int32x2 Z0,Z1;
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT((N&3)==0);
    NASSERT((M&3)==0);
    int m,n;
    if (N<=0 || M<=0)    /* exceptional situation */
    {
        for (m=0; m<M; m++) z[m]=0;
        return ;
    }
    py=(const ae_int16x4 *)y;
    pz=(      ae_int16x4 *)z;
    az=AE_ZALIGN64();

    px0=(const ae_int16x4 *)(x);
    px1=px0+N/4;
    px2=px0+N/2;
    px3=px0+3*N/4;
    if(N&4)
    {
        /* optimized for low-bit ordering: N is a multiple of 4, but not a multiple of 8    
           Here, 2 successive raws begin in different banks
        */
        px2=px1;
        __Pragma("ymemory(px2)")
        __Pragma("loop_count min=1")
        for(m=0; m<(M>>2); m++)
        {
            ae_int64 B0,B1,B2,B3;
            py=(const ae_int16x4 *)y;
            X2=AE_L16X4_X (px0,4*N);
            X3=AE_L16X4_X (px2,4*N);
            AE_L16X4_IP(Y0,py,8);
            AE_L16X4_IP(X0,px0,8);
            AE_L16X4_IP(X1,px2,8);
            B0=AE_MULZAAAAQ16(Y0,X0);
            B1=AE_MULZAAAAQ16(Y0,X1);
            B2=AE_MULZAAAAQ16(Y0,X2);
            B3=AE_MULZAAAAQ16(Y0,X3);
            for(n=0; n<(N>>3); n++)
            {
                AE_L16X4_IP(Y0,py,8);
                __Pragma("ymemory(py1)")
                py1=py;
                AE_L16X4_IP(Y1,py1,8);
                py=py1;
                X2=AE_L16X4_X (px0,4*N);
                X3=AE_L16X4_X (px2,4*N);
                AE_L16X4_IP(X0,px0,8);
                AE_L16X4_IP(X1,px2,8);
                AE_MULAAAAQ16(B0,Y0,X0);
                AE_MULAAAAQ16(B1,Y0,X1);
                AE_MULAAAAQ16(B2,Y0,X2);
                AE_MULAAAAQ16(B3,Y0,X3);
                X2=AE_L16X4_X (px0,4*N);
                X3=AE_L16X4_X (px2,4*N);
                AE_L16X4_IP(X0,px0,8);
                AE_L16X4_IP(X1,px2,8);
                AE_MULAAAAQ16(B0,Y1,X0);
                AE_MULAAAAQ16(B1,Y1,X1);
                AE_MULAAAAQ16(B2,Y1,X2);
                AE_MULAAAAQ16(B3,Y1,X3);
            }
            Z0=AE_TRUNCA32X2F64S(B0,B1,lsh+33);
            Z1=AE_TRUNCA32X2F64S(B2,B3,lsh+33);
            Y0=AE_ROUND16X4F32SASYM(Z0,Z1);
            AE_SA16X4_IP(Y0,az,pz);
            px0+=3*(N/4);
            px2+=3*(N/4);
        }
    }
    else
    {
        /*        optimized for low-bit ordering: N is a multiple of 8    */
        __Pragma("loop_count min=1")
        for(m=0; m<(M>>2); m++)
        {
            ae_int64 B0,B1,B2,B3;
            py=(const ae_int16x4 *)y;
            B0=B1=B2=B3=AE_ZERO64();
            __Pragma("loop_count min=1")
            for(n=0; n<(N>>3); n++)
            {
                AE_L16X4_IP(Y0,py,8);
                __Pragma("ymemory(py1)")
                py1=py;
                AE_L16X4_IP(Y1,py1,8);
                py=py1;
                X2=AE_L16X4_X (px0,4*N);
                X3=AE_L16X4_X (px1,4*N);
                AE_L16X4_IP(X0,px0,8);
                AE_L16X4_IP(X1,px1,8);
                AE_MULAAAAQ16(B0,Y0,X0);
                AE_MULAAAAQ16(B1,Y0,X1);
                AE_MULAAAAQ16(B2,Y0,X2);
                AE_MULAAAAQ16(B3,Y0,X3);
                __Pragma("ymemory(px2)")
                __Pragma("ymemory(px3)")
                px2=px0;
                px3=px1;
                X2=AE_L16X4_X (px2,4*N);
                X3=AE_L16X4_X (px3,4*N);
                AE_L16X4_IP(X0,px2,8);
                AE_L16X4_IP(X1,px3,8);
                px0=px2;
                px1=px3;
                AE_MULAAAAQ16(B0,Y1,X0);
                AE_MULAAAAQ16(B1,Y1,X1);
                AE_MULAAAAQ16(B2,Y1,X2);
                AE_MULAAAAQ16(B3,Y1,X3);
            }
            Z0=AE_TRUNCA32X2F64S(B0,B1,lsh+33);
            Z1=AE_TRUNCA32X2F64S(B2,B3,lsh+33);
            Y0=AE_ROUND16X4F32SASYM(Z0,Z1);
            AE_SA16X4_IP(Y0,az,pz);
            px0+=3*(N/4);
            px1+=3*(N/4);
        }
    }
    AE_SA64POS_FP(az,pz);
}
