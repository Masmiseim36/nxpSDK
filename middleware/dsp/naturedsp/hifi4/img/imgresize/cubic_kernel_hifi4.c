/* ------------------------------------------------------------------------ */
/* Copyright (c) 2020 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2020 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_types.h"
#include "common.h"
#include "cubic_kernel.h"

/*--------------------------------------------------
 compute cubic kernel: input in Q14, output in Q15

See Keys, "Cubic Convolution Interpolation for Digital Image
Processing," IEEE Transactions on Acoustics, Speech, and Signal
Processing, Vol. ASSP-29, No. 6, December 1981, p. 1155.


 Input:
 w[N] input in Q14
 Output:
 w[N] output Q15
 
 array is aligned, N is a multiple of 4
-------------------------------------------------*/
void cubic_kernel(int16_t *w, int N)
#if 0
{
    int n;
    NASSERT_ALIGN(w,HIFI_SIMD_WIDTH);
    NASSERT(N%4==0);
    for (n=0; n<N; n++)
    {
        int16_t x_q14,x2_q13,x3_q12;
        int64_t p1,p2;
        x_q14=w[n];
        x_q14=S_abs_s(x_q14);
        x2_q13=S_round_l(L_mpy_ss(x_q14 ,x_q14));
        x3_q12=S_round_l(L_mpy_ss(x2_q13,x_q14));

        p1=(int64_t)L_mul_ss(MIN_INT16, -2048)+ // Q11
           (int64_t)L_mul_ss(x_q14 ,     0)+ // Q12
           (int64_t)L_mul_ss(x2_q13,-20480)+ // Q13
           (int64_t)L_mul_ss(x3_q12, 24576); // Q14
        p2=(int64_t)L_mul_ss(MIN_INT16, -4096)+ // Q11
           (int64_t)L_mul_ss(x_q14 ,-16384)+ // Q12
           (int64_t)L_mul_ss(x2_q13, 20480)+ // Q13
           (int64_t)L_mul_ss(x3_q12, -8192); // Q14
//        w[n]=S_sature_l(
//                x_q14<16384 ?
//                satQ31((p1+((int32_t)1<<10))>>11):
//                satQ31((p2+((int32_t)1<<10))>>11));
        p1<<=5;
        p2<<=5;
        w[n]=x_q14<16384 ? S_round_l(satQ31(p1)) : S_round_l(satQ31(p2));

        w[n]=w[n];
    }
}
#else
{
    // polynomial coefficients
    static const ALIGN(8) int16_t c[]={-2048,0,-20480,24576,-4096,-16384,20480,-8192};
          ae_int16x4* restrict pWwr=(      ae_int16x4*)w;
    const ae_int16x4* restrict pWrd=(const ae_int16x4*)w;
    ae_int16x4 c1,c2;
    int n;

    c1=AE_L16X4_I((const ae_int16x4*)c,0);
    c2=AE_L16X4_I((const ae_int16x4*)c,sizeof(ae_int16x4));
    NASSERT_ALIGN(w,HIFI_SIMD_WIDTH);
    NASSERT(N%4==0);
    for (n=0; n<(N>>2); n++)
    {
        ae_int16x4 x0,x1,x2,x3,y0,y1,y2,y3,x_q14,x2_q13,x3_q12;
        ae_int64 p1,p2;
        ae_int16x4 r1,r2;
        AE_L16X4_IP(x_q14,pWrd,sizeof(ae_int16x4));
        x_q14=AE_ABS16S(x_q14);
        x2_q13=AE_MULFP16X4RAS(x_q14 ,x_q14);
        x3_q12=AE_MULFP16X4RAS(x2_q13,x_q14);
        x0=MIN_INT16;
        x1=x_q14;
        x2=x2_q13;
        x3=x3_q12;
        /* transpose */
        y0=AE_SEL16_7531(x0,x1); y1=AE_SEL16_6420(x0,x1);
        y2=AE_SEL16_7531(x2,x3); y3=AE_SEL16_6420(x2,x3);
        x0=AE_SEL16_7531(y0,y2); x2=AE_SEL16_6420(y0,y2);
        x1=AE_SEL16_7531(y1,y3); x3=AE_SEL16_6420(y1,y3);
        /* compute 2 polynomials */
        p1=AE_MULZAAAAQ16(x0,c1); p2=AE_MULZAAAAQ16(x0,c2);
        p1=AE_MULZAAAAQ16(x1,c1); p2=AE_MULZAAAAQ16(x1,c2);
        p1=AE_MULZAAAAQ16(x2,c1); p2=AE_MULZAAAAQ16(x2,c2);
        p1=AE_MULZAAAAQ16(x3,c1); p2=AE_MULZAAAAQ16(x3,c2);
        /* round and select one of them */
        r1=AE_ROUND16X4F32SASYM(
            AE_TRUNCA32X2F64S(AE_MULZAAAAQ16(x0,c1),AE_MULZAAAAQ16(x1,c1),37),
            AE_TRUNCA32X2F64S(AE_MULZAAAAQ16(x2,c1),AE_MULZAAAAQ16(x3,c1),37));
        r2=AE_ROUND16X4F32SASYM(
            AE_TRUNCA32X2F64S(AE_MULZAAAAQ16(x0,c2),AE_MULZAAAAQ16(x1,c2),37),
            AE_TRUNCA32X2F64S(AE_MULZAAAAQ16(x2,c2),AE_MULZAAAAQ16(x3,c2),37));
        AE_MOVF16X4(r1,r2,AE_LT16(x_q14,16384));
        AE_S16X4_IP(r1,pWwr,sizeof(ae_int16x4));
    }
}
#endif
