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
void mtx_transpose16x16_fast (int16_t  *  y, const int16_t*     x, int M, int N)
{
    xtbool4 beven=AE_MOVBA4(5);
    const ae_int16x4 * restrict pX;
          ae_int16x4 * restrict pY;
    int m, n;
    NASSERT( x );
    NASSERT( y );
    NASSERT(x != y);
    NASSERT(N%4==0 && M%4==0);
    if (M<=0 || N<=0) return;
    NASSERT_ALIGN(x,XCHAL_DATA_WIDTH);
    NASSERT_ALIGN(y,XCHAL_DATA_WIDTH);
    for ( m=0; m<(M>>2); m++,x+=(N<<2),y+=4)  
    {    
        ae_int16x4 x0,x1,x2,x3;
        ae_int16x4 y0,y1,y2,y3;
        pY=(      ae_int16x4 *)(y);
        pX=(const ae_int16x4 *)(x);
        __Pragma("loop_count min=1")
        for ( n=0; n<(N>>2); n++)    
        {
            /* load/save 2-nd and 4-th element in another endianess mode thus giving smaller number of permutations */
            ae_int32x2 t;
            t=AE_L32X2_X((const ae_int32x2*)pX,1*N*sizeof(int16_t)); x1= AE_MOVINT16X4_FROMINT32X2(t);
            x2=AE_L16X4_X(pX,2*N*sizeof(int16_t));
            t=AE_L32X2_X((const ae_int32x2*)pX,3*N*sizeof(int16_t)); x3= AE_MOVINT16X4_FROMINT32X2(t);
            AE_L16X4_IP(x0,pX,4*sizeof(int16_t));

            y0=x0; AE_MOVT16X4(y0,x1,beven);   /* 0  4  2  6 */
            y1=x1; AE_MOVT16X4(y1,x0,beven);   /* 5  1  7  3 */
            y2=x2; AE_MOVT16X4(y2,x3,beven);   /* 8 12 10 14 */
            y3=x3; AE_MOVT16X4(y3,x2,beven);   /*13  9 15 11 */

            x0=AE_SEL16_7632(y0,y2);  /* 0  4  8 12 */
            x1=AE_SEL16_7632(y1,y3);  /* 5  1 13  9 */
            x2=AE_SEL16_5410(y0,y2);  /* 2  6 10 14 */
            x3=AE_SEL16_5410(y1,y3);  /* 7  3 15 11 */
            AE_S16X4_XP(x0,pY,M*sizeof(int16_t));
            AE_S32X2_XP(AE_MOVINT32X2_FROMINT16X4(x1),castxcc(ae_int32x2,pY),M*sizeof(int16_t));
            AE_S16X4_XP(x2,pY,M*sizeof(int16_t));
            AE_S32X2_XP(AE_MOVINT32X2_FROMINT16X4(x3),castxcc(ae_int32x2,pY),M*sizeof(int16_t));
        }  
    }
}
