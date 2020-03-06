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
void mtx_transpose32x32 (int32_t*    y, const int32_t*     x, int M, int N)
{
    ae_int32x2 * restrict pY;
    ae_valign aY,aX;
    const ae_int32 * restrict pX;
    ae_int32x2 x0,x1,y0;
    int m, n;
    NASSERT( x );
    NASSERT( y );
    NASSERT(x != y);
    if (M<=0 || N<=0) return;
    if (M>1)
    {
        for ( n=0; n<N; n++ )    
        {    
            pY=(ae_int32x2 *)(y+n*M);
            pX=(ae_int32*)(x+n);
            aY=AE_ZALIGN64();
            __Pragma("loop_count min=1")
            for ( m=0; m<(M&~1); m+=2 )  
            {      
                AE_L32_XP(x0,pX,N*sizeof(int32_t));
                AE_L32_XP(x1,pX,N*sizeof(int32_t));
                y0=AE_SEL32_HH(x0,x1);
                AE_SA32X2_IP(y0,aY,pY);
            }  
            AE_SA64POS_FP(aY,pY);
        }
    }
    if (M&1)
    {   // process last odd column
        pX=(const ae_int32  *)(x+(M-1)*N); 
        pY=(      ae_int32x2*)(y+M-1); 
        if(N&1)
        {
            AE_L32_IP(x0,pX,sizeof(int32_t));
            AE_S32_L_XP(x0,castxcc(ae_int32,pY),M*sizeof(int32_t));
        }
        if (N>1)
        {
            aX=AE_LA64_PP(pX);
            __Pragma("loop_count min=1")
            for ( n=0; n<(N>>1); n++ )    
            {    
                AE_LA32X2_IP(x0,aX,castxcc(ae_int32x2,pX));
                AE_S32_L_XP(AE_SEL32_HH(x0,x0),castxcc(ae_int32,pY),M*sizeof(int32_t));
                AE_S32_L_XP(x0,castxcc(ae_int32,pY),M*sizeof(int32_t));
            }
        }
    }
}
