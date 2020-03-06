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
   Matrix transpose
 * Reference code
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Library API */
#include "NatureDSP_Signal_matop.h"
/* Common helper macros. */
#include "common.h"

/*-------------------------------------------------------------------------
  Matrix Transpose
  These functions transpose matrices.

  Precision: 
  32x32 32-bit input, 32-bit output
  16x16 16-bit input, 16-bit output
  8x8   8-bit inputs, 8-bit output
  f     floating point

  Input:
  x[M][N] input matrix,Q31,Q15,Q7 or floating point
  M       number of rows in matrix x
  N       number of columns in matrix x
  Output:
  y[N][M] output vector,Q31,Q15,Q7 or floating point

  Restriction:
  For regular routines (mtx_transpose_32x32, mtx_transpose_16x16, 
  mtx_transpose_8x8, mtx_transposef):
  x,y should not overlap

  For faster routines (mtx_transpose 32x32_fast, mtx_transpose 16x16_fast, 
  mtx_transpose_8x8_fast, mtx_transposef_fast)
  x,y   should not overlap
  x,y   aligned on 8-byte boundary
  N and M are multiples of 4
-------------------------------------------------------------------------*/
void mtx_transpose32x32_fast (int32_t  *  y, const int32_t*     x, int M, int N)
{
    const ae_int32x2 * restrict pX;
          ae_int32x2 * restrict pY;
    int m, n;
    NASSERT( x );
    NASSERT( y );
    NASSERT(x != y);
    NASSERT(N%4==0 && M%4==0);
    if (M<=0 || N<=0) return;
    NASSERT_ALIGN(x,XCHAL_DATA_WIDTH);
    NASSERT_ALIGN(y,XCHAL_DATA_WIDTH);
    pX=(const ae_int32x2*)(x);
    for ( m=0; m<M; m+=2)  
    {
        pY=(      ae_int32x2*)(y+m);
        __Pragma("loop_count min=1")
        for ( n=0; n<(N>>2); n++)    
        {
            ae_int32x2 x0,x1,y0,y1;
            x1=AE_L32X2_X(pX,N*sizeof(int32_t));
            AE_L32X2_IP(x0,pX,2*sizeof(int32_t));
            y0=AE_SEL32_HH(x0,x1);
            y1=AE_SEL32_LL(x0,x1);
            AE_S32X2_XP(y0,pY, M*sizeof(int32_t));
            AE_S32X2_XP(y1,pY, M*sizeof(int32_t));
            x1=AE_L32X2_X(pX,N*sizeof(int32_t));
            AE_L32X2_IP(x0,pX,2*sizeof(int32_t));
            y0=AE_SEL32_HH(x0,x1);
            y1=AE_SEL32_LL(x0,x1);
            AE_S32X2_XP(y0,pY, M*sizeof(int32_t));
            AE_S32X2_XP(y1,pY, M*sizeof(int32_t));
        }
        pX+=(N>>1);
    }
}
