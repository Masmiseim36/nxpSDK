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
void conv2d_5x5_8x16  (void* pScr, int16_t *z, const int8_t  * x, const int16_t * y, int rsh, int P, int Q)
{
    int m;
    ae_int16x4 t;
    ae_int16x4 *pW=(ae_int16x4 *)pScr;  /* first 28 int16_t elements are temporary storage for modified coefficients */
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT_ALIGN(z,8);
    NASSERT_ALIGN(pScr,8);
    NASSERT(P>=0 && P%8==0);
    NASSERT(Q>=0 && Q%8==0);
    /* just copy original 8-bit coefficients to 16-bit shifting left by 8 bits */
    for (m=0; m<6; m++) 
    {
        AE_L8X4F_IP(t,x,4*sizeof(int8_t)); 
        AE_S16X4_IP(t,pW,sizeof(ae_int16x4));
    }
    t=AE_MOVDA16(x[0]); t=AE_SLLI16S(t,7); t=AE_ADD16(t,t); AE_S16X4_IP(t,pW,sizeof(ae_int16x4));
    conv2d_5x5_16x16 (pW,z, (const int16_t*)pScr, y, rsh, P, Q);
}

// scratch allocatation functions. return required scratch size in bytes
size_t conv2d_5x5_8x16_getScratchSize  (int P, int Q) 
{ 
    return 28*sizeof(int16_t)+conv2d_5x5_16x16_getScratchSize(P,Q) ;
} 
