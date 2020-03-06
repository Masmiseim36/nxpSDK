/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
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
    Helper for circular correlation/convolution with originally non-aligned inputs
    This function takes aligned inputs (allocated from the scratch) with circularly 
    duplicated inputs

    C code optimized for HiFi4
    IntegrIT, 2006-2018

    raw linear correlation
    input:
    x[N+M-1 ]  - input . should be aligned on 8-byte boundary
    y[1+M+7]   - input padded with 1 zero from the left side and 7 zeroes from 
                 the right side. Should be aligned on 8-byte boundary
    output:
    r[N]       - correlations
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "raw_corr32x16.h"

void raw_corr32x16(int32_t* restrict r, const int32_t* restrict  x,const int16_t* restrict y, int N,int M)
{
    int n,m;
    ae_f64 A0,A1,A2,A3;
    ae_int32x2 x10,x32,x54;
    ae_int16x4 y3210,y4321;
    const ae_int16x4* restrict py0;
    const ae_int16x4* restrict py1;
    const ae_int32x2* restrict px;
          ae_int32x2* restrict R;
    ae_valign ay1,ar;
    ar=AE_ZALIGN64();
    M=(M+7)&(~7);
    R = (ae_int32x2*)r;

    if(M==8)    // special variant to minimize outer loop overhead for M>8
    {
        ae_int16x4 y0,y1,y2;
        py0=(const ae_int16x4*)(y);
        py1=(const ae_int16x4*)(y+1);
        ay1=AE_LA64_PP(py1);
        AE_LA16X4_IP(y1,ay1,py1);
        AE_LA16X4_IP(y4321,ay1,py1);
        AE_L16X4_IP (y0,py0,8);
        AE_L16X4_IP (y3210,py0,8);
        AE_L16X4_IP (y2,py0,8);
        px =(const ae_int32x2*)(x);
        for ( n=0; n<(N&(~3)); n+=4 )
        {
            x10=AE_L32X2_I (px,8*0);
            x32=AE_L32X2_I (px,8*1);
            x54=AE_L32X2_I (px,8*2);
            A0=AE_MULZAAAAFQ32X16 (x10,x32,y1);
            A1=AE_MULZAAAAFQ32X16 (x10,x32,y0);
            A2=AE_MULZAAAAFQ32X16 (x32,x54,y1);
            A3=AE_MULZAAAAFQ32X16 (x32,x54,y0);
            x32=AE_L32X2_I(px,8*3);
            x10=AE_L32X2_I(px,8*4);
            AE_MULAAAAFQ32X16 (A0,x54,x32,y4321);
            AE_MULAAAAFQ32X16 (A1,x54,x32,y3210);
            AE_MULAAAAFQ32X16 (A2,x32,x10,y4321);
            AE_MULAAAAFQ32X16 (A3,x32,x10,y3210);
            x32=AE_L32X2_I (px,8*5);
            px+=2;
            AE_MULAF32X16_H3(A1,x10,y2);
            AE_MULAF32X16_H3(A3,x32,y2);
            x10=AE_ROUND32X2F48SASYM(A0,A1 );
            x32=AE_ROUND32X2F48SASYM(A2,A3 );
            AE_SA32X2_IP( x10, ar,R);
            AE_SA32X2_IP( x32, ar,R);
        }
        // tail
        AE_SA64POS_FP(ar,R);
        N&=3;
        if(N)
        {
            x10=AE_L32X2_I (px,8*0);
            x32=AE_L32X2_I (px,8*1);
            x54=AE_L32X2_I (px,8*2);
            A0=AE_MULZAAAAFQ32X16 (x10,x32,y1);
            A1=AE_MULZAAAAFQ32X16 (x10,x32,y0);
            A2=AE_MULZAAAAFQ32X16 (x32,x54,y1);
            x32=AE_L32X2_I(px,8*3);
            x10=AE_L32X2_I(px,8*4);
            AE_MULAAAAFQ32X16 (A0,x54,x32,y4321);
            AE_MULAAAAFQ32X16 (A1,x54,x32,y3210);
            AE_MULAAAAFQ32X16 (A2,x32,x10,y4321);
            AE_MULAF32X16_H3(A1,x10,y2);
            x10=AE_ROUND32X2F48SASYM(A0,A1 );
            x32=AE_ROUND32X2F48SASYM(A2,A2 );
            if (N>2)  ((int32_t*)R)[2]=AE_MOVAD32_H(x32);
            if (N>1)  ((int32_t*)R)[1]=AE_MOVAD32_L(x10);
            ((int32_t*)R)[0]=AE_MOVAD32_H(x10);
        }
    }
    else    // M>8
    {
        ae_int16x4 y0,y1;
        px =(const ae_int32x2*)(x);
        A2=A3=AE_ZERO();
        py0=(const ae_int16x4*)(y);
        py1=(const ae_int16x4*)(y+1);
        ay1=AE_LA64_PP(py1);
        AE_LA16X4_IP(y1,ay1,py1);
        AE_L16X4_IP (y0,py0,8);
        AE_L32X2_IP(x10,px,8);
        AE_L32X2_IP(x32,px,8);
        for ( n=0; n<(N&(~3)); n+=4 )
        {
            AE_L32X2_IP(x54,px,8);
            A0=AE_MULZAAAAFQ32X16    (x10,x32,y1);
            A1=AE_MULZAAAAFQ32X16_S2 (x10,x32,y0);
            __Pragma("loop_count min=1") // M is a multiple of 8
            for ( m=0; m<(M>>3)-1; m++ )
            {
                AE_LA16X4_IP(y4321,ay1,py1);
                AE_L16X4_IP (y3210,py0,8);
                AE_MULAAAAFQ32X16(A2,x32,x54,y1);
                AE_MULAAAAFQ32X16_S2(A3,x32,x54,y0);
                AE_L32X2_IP(x32,px,8);
                AE_L32X2_IP(x10,px,8);
                AE_MULAAAAFQ32X16(A0,x54,x32,y4321);
                AE_MULAAAAFQ32X16_S2(A1,x54,x32,y3210);
                AE_LA16X4_IP(y1,ay1,py1);
                AE_L16X4_IP (y0,py0,8);
                AE_MULAAAAFQ32X16(A2,x32,x10,y4321);
                AE_MULAAAAFQ32X16_S2(A3,x32,x10,y3210);
                AE_L32X2_IP(x32,px,8);
                AE_L32X2_IP(x54,px,8);
                AE_MULAAAAFQ32X16(A0,x10,x32,y1);
                AE_MULAAAAFQ32X16_S2(A1,x10,x32,y0);
            }
            AE_LA16X4_IP(y4321,ay1,py1);
            AE_L16X4_IP (y3210,py0,8);
            AE_MULAAAAFQ32X16   (A2,x32,x54,y1);
            AE_MULAAAAFQ32X16_S2(A3,x32,x54,y0);
            AE_L32X2_IP(x32,px,8);
            AE_L32X2_IP(x10,px,8);
            AE_MULAAAAFQ32X16   (A0,x54,x32,y4321);
            AE_MULAAAAFQ32X16_S2(A1,x54,x32,y3210);
            AE_L16X4_IP (y0,py0,8);
            AE_MULAAAAFQ32X16   (A2,x32,x10,y4321);
            AE_MULAAAAFQ32X16_S2(A3,x32,x10,y3210);
            AE_L32X2_XP(x32,px,8-(M<<2));
            AE_MULAF32X16_H3(A1,x10,y0);
            AE_MULAF32X16_H3(A3,x32,y0);
            //px-=(M>>1);
            x10=AE_ROUND32X2F48SASYM(A0,A1 );
            x32=AE_ROUND32X2F48SASYM(A2,A3 );
            AE_SA32X2_IP( x10, ar,R);
            AE_SA32X2_IP( x32, ar,R);

            A2=A3=AE_ZERO();
            py0=(const ae_int16x4*)(y);
            py1=(const ae_int16x4*)(y+1);
            ay1=AE_LA64_PP(py1);
            AE_LA16X4_IP(y1,ay1,py1);
            AE_L16X4_IP (y0,py0,8);
            AE_L32X2_IP(x10,px,8);
            AE_L32X2_IP(x32,px,8);
        }
        AE_SA64POS_FP(ar,R);

        // tail
        N&=3;
        if(N)
        {
            AE_L32X2_IP(x54,px,8);
            A0=AE_MULZAAAAFQ32X16    (x10,x32,y1);
            A1=AE_MULZAAAAFQ32X16_S2 (x10,x32,y0);
            __Pragma("loop_count min=1") // M is a multiple of 8
            for ( m=0; m<(M>>3)-1; m++ )
            {
                AE_LA16X4_IP(y4321,ay1,py1);
                AE_L16X4_IP (y3210,py0,8);
                AE_MULAAAAFQ32X16(A2,x32,x54,y1);
                AE_MULAAAAFQ32X16_S2(A3,x32,x54,y0);
                AE_L32X2_IP(x32,px,8);
                AE_L32X2_IP(x10,px,8);
                AE_MULAAAAFQ32X16(A0,x54,x32,y4321);
                AE_MULAAAAFQ32X16_S2(A1,x54,x32,y3210);
                AE_LA16X4_IP(y1,ay1,py1);
                AE_L16X4_IP (y0,py0,8);
                AE_MULAAAAFQ32X16(A2,x32,x10,y4321);
                AE_MULAAAAFQ32X16_S2(A3,x32,x10,y3210);
                AE_L32X2_IP(x32,px,8);
                AE_L32X2_IP(x54,px,8);
                AE_MULAAAAFQ32X16(A0,x10,x32,y1);
                AE_MULAAAAFQ32X16_S2(A1,x10,x32,y0);
            }
            AE_LA16X4_IP(y4321,ay1,py1);
            AE_L16X4_IP (y3210,py0,8);
            AE_MULAAAAFQ32X16   (A2,x32,x54,y1);
            AE_MULAAAAFQ32X16_S2(A3,x32,x54,y0);
            AE_L32X2_IP(x32,px,8);
            AE_L32X2_IP(x10,px,8);
            AE_MULAAAAFQ32X16   (A0,x54,x32,y4321);
            AE_MULAAAAFQ32X16_S2(A1,x54,x32,y3210);
            AE_L16X4_IP (y0,py0,8);
            AE_MULAAAAFQ32X16   (A2,x32,x10,y4321);
            AE_MULAAAAFQ32X16_S2(A3,x32,x10,y3210);
            AE_L32X2_XP(x32,px,8-(M<<2));
            AE_MULAF32X16_H3(A1,x10,y0);
            AE_MULAF32X16_H3(A3,x32,y0);
            //px-=(M>>1);
            x10=AE_ROUND32X2F48SASYM(A0,A1 );
            x32=AE_ROUND32X2F48SASYM(A2,A3 );
            if (N>2)  ((int32_t*)R)[2]=AE_MOVAD32_H(x32);
            if (N>1)  ((int32_t*)R)[1]=AE_MOVAD32_L(x10);
            ((int32_t*)R)[0]=AE_MOVAD32_H(x10);
        }
    }
}
