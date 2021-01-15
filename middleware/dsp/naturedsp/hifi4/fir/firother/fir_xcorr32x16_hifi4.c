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
    Real data circular convolution, 32x16-bit
    C code optimized for HiFi4
    IntegrIT, 2006-2018
*/


/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"

/*-------------------------------------------------------------------------
  Circular Correlation
  Estimates the circular cross-correlation between vectors x (of length N) 
  and y (of length M)  resulting in vector r of length N. It is a similar 
  to correlation but x is read in opposite direction.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  32x16     32x16-bit data, 32-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  32x32ep   the same as above but using 72-bit accumulator for intermediate 
            computations
  f         floating point 


  Input:
  x[N]      input data Q15, Q31 or floating point
  y[M]      input data Q15, Q31 or floating point
  N         length of x
  M         length of y
  Output:
  r[N]      output data, Q15, Q31 or floating point

  Restriction:
  x,y,r     should not overlap
  x,y,r     aligned on an 8-bytes boundary
  N,M       multiples of 4 and >0
-------------------------------------------------------------------------*/
static void fir_xcorr32x16_M8( int32_t * restrict r,
               const int32_t * restrict x,
               const int16_t * restrict y,
               int N);
static void fir_xcorr32x16_M4( int32_t * restrict r,
               const int32_t * restrict x,
               const int16_t * restrict y,
               int N);

void fir_xcorr32x16( int32_t * restrict r,
               const int32_t * restrict x,
               const int16_t * restrict y,
               int N, int M )

{
    ae_f64 A0,A1,A2,A3;
    const ae_int32x2 * restrict px0;
    const ae_int32x2 * restrict px1;
    const ae_int16x4 * restrict py0;
    const ae_int16x4 * restrict py1;
          ae_int32x2 * restrict pr=(ae_int32x2 *)r;
    ae_int16x4 y0,y1,y2,y3;
    ae_int32x2 x0,x1,x2;
    ae_valign ay;
    int n, m;

    NASSERT( r && x && y && N >= 0 && M >= 0 );
    NASSERT(M%4==0 && N%4==0);
    if(M==8)  { fir_xcorr32x16_M8 (r,x,y,N); return; }
    if(M==4)  { fir_xcorr32x16_M4 (r,x,y,N); return; }

    WUR_AE_CBEGIN0( (uintptr_t)( x + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( x + N ) );

    if ((M&4)==0)
    {   // M is a multiple of 8
        for ( n=0; n<N; n+=4 )
        {
            px0=(const ae_int32x2 *)&x[n];
            py0=(const ae_int16x4 *)y;
            py1=(const ae_int16x4 *)(y-1);
            ay = AE_LA64_PP(py1);
            A2=A3=AE_ZERO();
            AE_LA16X4_IP(y1,ay,py1);
            AE_L16X4_IP (y0,py0,8);
            AE_L32X2_XC ( x0, px0,  8 );
            px1=px0;AE_ADDCIRC_XC(castxcc(ae_int64,px1),8);
            AE_MOVT16X4(y1,AE_ZERO16(),AE_MOVBA4(8));    // mask 1-st item
            AE_L32X2_XC ( x1, px0, 16 );
            AE_L32X2_XC ( x2, px1, 16 );
            A0=AE_MULZAAAAFQ32X16   (x0,x1,y0);
            A1=AE_MULZAAAAFQ32X16_S2(x0,x1,y1);
            __Pragma("loop_count min=1")
            for ( m=0; m<(M>>3)-1; m++)
            {
                AE_LA16X4_IP(y3,ay,py1);
                AE_L16X4_IP (y2,py0,8);
                AE_MULAAAAFQ32X16   (A2,x1,x2,y0);
                AE_MULAAAAFQ32X16_S2(A3,x1,x2,y1);
                AE_L32X2_XC ( x1, px0, 16 );
                AE_L32X2_XC ( x0, px1, 16 );
                AE_MULAAAAFQ32X16   (A0,x2,x1,y2);
                AE_MULAAAAFQ32X16_S2(A1,x2,x1,y3);
                AE_LA16X4_IP(y1,ay,py1);
                AE_L16X4_IP (y0,py0,8);
                AE_MULAAAAFQ32X16   (A2,x1,x0,y2);
                AE_MULAAAAFQ32X16_S2(A3,x1,x0,y3);
                AE_L32X2_XC ( x1, px0, 16 );
                AE_L32X2_XC ( x2, px1, 16 );
                AE_MULAAAAFQ32X16   (A0,x0,x1,y0);
                AE_MULAAAAFQ32X16_S2(A1,x0,x1,y1);
            }
            AE_LA16X4_IP(y3,ay,py1);
            AE_L16X4_IP (y2,py0,8);
            AE_MULAAAAFQ32X16   (A2,x1,x2,y0);
            AE_MULAAAAFQ32X16_S2(A3,x1,x2,y1);
            AE_L32X2_XC ( x1, px0, 16 );
            AE_L32X2_XC ( x0, px1, 16 );
            AE_MULAAAAFQ32X16   (A0,x2,x1,y2);
            AE_MULAAAAFQ32X16_S2(A1,x2,x1,y3);
            AE_LA16X4_IP(y1,ay,py1);
            AE_L16X4_IP (y0,py0,8);
            AE_MULAAAAFQ32X16   (A2,x1,x0,y2);
            AE_MULAAAAFQ32X16_S2(A3,x1,x0,y3);
            AE_L32X2_XC ( x1, px0, 16);
            AE_MULAF32X16_H0(A1,x0,y2);
            AE_MULAF32X16_H0(A3,x1,y2);

            x0=AE_ROUND32X2F48SASYM(A0,A1 );
            x1=AE_ROUND32X2F48SASYM(A2,A3 );
            AE_S32X2_IP( x0, pr, 8);
            AE_S32X2_IP( x1, pr, 8);
        }
    }
    else
    {
        // M is not a multiple of 8
        for ( n=0; n<N; n+=4 )
        {
            px0=(const ae_int32x2 *)&x[n];
            py0=(const ae_int16x4 *)y;
            py1=(const ae_int16x4 *)(y-1);
            ay = AE_LA64_PP(py1);

            AE_LA16X4_IP(y1,ay,py1);
            AE_L16X4_IP (y0,py0,8);
            AE_L32X2_XC ( x0, px0,  8);
            px1=px0;AE_ADDCIRC_XC(castxcc(ae_int64,px1),8);
            AE_L32X2_XC ( x1, px0, 16);
            AE_L32X2_XC ( x2, px1, 16);
            AE_MOVT16X4(y1,AE_ZERO16(),AE_MOVBA4(8));    // mask 1-st item

            A0=AE_MULZAAAAFQ32X16 (x0,x1,y0);
            A1=AE_MULZAAAAFQ32X16 (x0,x1,y1);
            A2=AE_MULZAAAAFQ32X16 (x1,x2,y0);
            A3=AE_MULZAAAAFQ32X16 (x1,x2,y1);
            x0=x2;
            AE_LA16X4_IP(y3,ay,py1);
            AE_L16X4_IP (y2,py0,8);
            __Pragma("loop_count min=1")
            for ( m=0; m<(M>>3); m++)
            {
                AE_L32X2_XC ( x1, px0, 16);
                AE_L32X2_XC ( x2, px1, 16);
                AE_MULAAAAFQ32X16 (A0,x0,x1,y2);
                AE_MULAAAAFQ32X16 (A1,x0,x1,y3);
                AE_LA16X4_IP(y1,ay,py1);
                AE_L16X4_IP (y0,py0,8);
                AE_MULAAAAFQ32X16 (A2,x1,x2,y2);
                AE_MULAAAAFQ32X16 (A3,x1,x2,y3);
                AE_L32X2_XC ( x1, px0, 16);
                AE_L32X2_XC ( x0, px1, 16);
                AE_MULAAAAFQ32X16 (A0,x2,x1,y0);
                AE_MULAAAAFQ32X16 (A1,x2,x1,y1);
                AE_LA16X4_IP(y3,ay,py1);
                AE_L16X4_IP (y2,py0,8);
                AE_MULAAAAFQ32X16 (A2,x1,x0,y0);
                AE_MULAAAAFQ32X16 (A3,x1,x0,y1);
            }
            AE_L32X2_XC ( x1, px0, 16);
            AE_MULAF32X16_H0(A1,x0,y0);
            AE_MULAF32X16_H0(A3,x1,y0);

            x0=AE_ROUND32X2F48SASYM(A0,A1 );
            x1=AE_ROUND32X2F48SASYM(A2,A3 );
            AE_S32X2_IP( x0, pr, 8);
            AE_S32X2_IP( x1, pr, 8);
        }
    }
}

/* special variant for M==8 */
void fir_xcorr32x16_M8( int32_t * restrict r,
               const int32_t * restrict x,
               const int16_t * restrict y,
               int N)
{
    ae_f64 A0,A1,A2,A3;
    const ae_int32x2 * restrict px0;
    const ae_int32x2 * restrict px1;
    const ae_int16x4 * restrict py0;
    const ae_int16x4 * restrict py1;
          ae_int32x2 * restrict pr=(ae_int32x2 *)r;
    ae_int16x4 y0,y1,y2,y3;
    ae_int32x2 x0,x1,x2;
    ae_valign ay;
    int n;

    NASSERT( r && x && y && N >= 0  );
    NASSERT(N%4==0);

    WUR_AE_CBEGIN0( (uintptr_t)( x + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( x + N ) );

    py0=(const ae_int16x4 *)y;
    py1=(const ae_int16x4 *)(y-1);
    ay = AE_LA64_PP(py1);
    AE_LA16X4_IP(y1,ay,py1);
    AE_L16X4_IP (y0,py0,8);
    AE_LA16X4_IP(y3,ay,py1);
    AE_L16X4_IP (y2,py0,8);
    AE_MOVT16X4(y1,AE_ZERO16(),AE_MOVBA4(8));    // mask 1-st item
    px0=(const ae_int32x2 *)&x[0];
    px1=px0;AE_ADDCIRC_XC(castxcc(ae_int64,px1),8);
    for ( n=0; n<N; n+=4 )
    {
       // px0=(const ae_int32x2 *)&x[n];
        AE_L32X2_XC ( x0, px0, 16 );
        AE_L32X2_XC ( x1, px1, 16 );
        AE_L32X2_XC ( x2, px0, 16 );
        A0=AE_MULZAAAAFQ32X16   (x0,x1,y0);
        A1=AE_MULZAAAAFQ32X16_S2(x0,x1,y1);
        A2=AE_MULZAAAAFQ32X16   (x1,x2,y0);
        A3=AE_MULZAAAAFQ32X16_S2(x1,x2,y1);
        AE_L32X2_XC ( x1, px1, 16 );
        AE_L32X2_XC ( x0, px0,-16 );
        AE_MULAAAAFQ32X16   (A0,x2,x1,y2);
        AE_MULAAAAFQ32X16_S2(A1,x2,x1,y3);
        AE_MULAAAAFQ32X16   (A2,x1,x0,y2);
        AE_MULAAAAFQ32X16_S2(A3,x1,x0,y3);
        AE_L32X2_XC ( x1, px1,-16);
        AE_MULAF32X16_H0(A1,x0,y2);
        AE_MULAF32X16_H0(A3,x1,y2);
        x0=AE_ROUND32X2F48SASYM(A0,A1 );
        x1=AE_ROUND32X2F48SASYM(A2,A3 );
        AE_S32X2_IP( x0, pr, 8);
        AE_S32X2_IP( x1, pr, 8);
    }
}

/* special variant for M==4 */
void fir_xcorr32x16_M4( int32_t * restrict r,
               const int32_t * restrict x,
               const int16_t * restrict y,
               int N)
{
    ae_f64 A0,A1,A2,A3;
    const ae_int32x2 * restrict px0;
    const ae_int32x2 * restrict px1;
    const ae_int16x4 * restrict py0;
    const ae_int16x4 * restrict py1;
          ae_int32x2 * restrict pr=(ae_int32x2 *)r;
    ae_int16x4 y0,y1;
    ae_int32x2 x0,x1,x2;
    ae_valign ay;
    int n;

    NASSERT( r && x && y && N >= 0 );
    NASSERT(N%4==0);

    WUR_AE_CBEGIN0( (uintptr_t)( x + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( x + N ) );

    py0=(const ae_int16x4 *)y;
    py1=(const ae_int16x4 *)(y-1);
    ay = AE_LA64_PP(py1);
    AE_LA16X4_IP(y1,ay,py1);
    AE_L16X4_IP (y0,py0,8);
    AE_MOVT16X4(y1,AE_ZERO16(),AE_MOVBA4(8));    // mask 1-st item
    px0=(const ae_int32x2 *)&x[0];
    px1=px0; AE_ADDCIRC_XC(castxcc(ae_int64,px1),8);
    for ( n=0; n<N; n+=4 )
    {
        AE_L32X2_XC ( x0, px0,16);
        AE_L32X2_XC ( x1, px1,16);

        A0=AE_MULZAAAAFQ32X16 (x0,x1,y0);
        A1=AE_MULZAAAAFQ32X16 (x0,x1,y1);
        x2=AE_L32X2_I (px0, 0);
        A2=AE_MULZAAAAFQ32X16 (x1,x2,y0);
        A3=AE_MULZAAAAFQ32X16 (x1,x2,y1);
        x1=AE_L32X2_I (px1, 0);
        AE_MULAF32X16_H0(A1,x2,y0);
        AE_MULAF32X16_H0(A3,x1,y0);

        x0=AE_ROUND32X2F48SASYM(A0,A1 );
        x1=AE_ROUND32X2F48SASYM(A2,A3 );
        AE_S32X2_IP( x0, pr, 8);
        AE_S32X2_IP( x1, pr, 8);
    }
}
