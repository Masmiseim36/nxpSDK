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
    Vector dot product for Emulated Floating Point
    optimized code for HiFi4 core
*/
/* Library API */
#include "NatureDSP_Signal_vector.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Vector Dot Product for Emulated Floating Point
  routines compute dot product of vectors represented in emulated floating 
  point format

  Input:
  xmant[N]  mantissa of input data
  ymant[N]  mantissa of input data
  xexp[N]   exponent of input data
  yexp[N]   exponent of input data
  N         length of vectors
  Output:
  zmant[1]  mantissa of output data
  zexp[1]   exponent of output data

  Restriction:
  xmant,ymant,xexp,yexp,zmant,zexp should not overlap
-------------------------------------------------------------------------*/
void   vec_dot_32x16ef (      int32_t  * restrict zmant,       int16_t  * restrict zexp, 
                        const int32_t  * restrict xmant, const int16_t  * restrict xexp, 
                        const int32_t  * restrict ymant, const int16_t  * restrict yexp, 
                        int N)
{
    ae_valign aXexp,aXmant,aYexp,aYmant;
    const ae_int16x4 * restrict pXexp;
    const ae_int16x4 * restrict pYexp;
    const ae_int32x2 * restrict pXmant;
    const ae_int32x2 * restrict pYmant;
    ae_int32x2 me=MIN_INT16;
    ae_int64 a;
    int maxexp;
    int n;
    zmant[0]=0; zexp[0]=0;
    if (N<=0) return;
    /* compute common exponent taking into account zero mantissa words */
    pXexp =(const ae_int16x4 *)xexp ;
    pYexp =(const ae_int16x4 *)yexp ;
    pXmant=(const ae_int32x2 *)xmant;
    pYmant=(const ae_int32x2 *)ymant;
    __Pragma("loop_count min=0,max=3")
    __Pragma("no_unroll")
    for(n=0; n<(N&3); n++) 
    {
        ae_int16x4 xe,ye;
        ae_int32x2 xm,ym,xe0,ye0;
        AE_L16_IP(xe,castxcc(ae_int16,pXexp),sizeof(int16_t));
        AE_L16_IP(ye,castxcc(ae_int16,pYexp),sizeof(int16_t));
        AE_L32_IP(xm,castxcc(ae_int32,pXmant),sizeof(int32_t));
        AE_L32_IP(ym,castxcc(ae_int32,pYmant),sizeof(int32_t));
        
        xe0=AE_SEXT32X2D16_10(xe);
        ye0=AE_SEXT32X2D16_10(ye);
        AE_MOVT32X2(xe0,MIN_INT16,AE_EQ32(xm,0));
        AE_MOVT32X2(ye0,MIN_INT16,AE_EQ32(ym,0));
        me=AE_MAX32(me,AE_ADD32(xe0,ye0));
    }
    aXexp =AE_LA64_PP(pXexp );
    aYexp =AE_LA64_PP(pYexp );
    aXmant=AE_LA64_PP(pXmant);
    aYmant=AE_LA64_PP(pYmant);
    for(n=0; n<((N&~3)>>2); n++) 
    {
        ae_int16x4 xe,ye;
        ae_int32x2 xm0,xm1,ym0,ym1,xe0,ye0,xe1,ye1;
        AE_LA16X4_IP(xe,aXexp,pXexp);
        AE_LA16X4_IP(ye,aYexp,pYexp);
        AE_LA32X2_IP(xm0,aXmant,pXmant);
        AE_LA32X2_IP(xm1,aXmant,pXmant);
        AE_LA32X2_IP(ym0,aYmant,pYmant);
        AE_LA32X2_IP(ym1,aYmant,pYmant);
        
        xe0=AE_SEXT32X2D16_32(xe);
        xe1=AE_SEXT32X2D16_10(xe);
        ye0=AE_SEXT32X2D16_32(ye);
        ye1=AE_SEXT32X2D16_10(ye);
        AE_MOVT32X2(xe0,MIN_INT16,AE_EQ32(xm0,0));
        AE_MOVT32X2(xe1,MIN_INT16,AE_EQ32(xm1,0));
        AE_MOVT32X2(ye0,MIN_INT16,AE_EQ32(ym0,0));
        AE_MOVT32X2(ye1,MIN_INT16,AE_EQ32(ym1,0));
        me=AE_MAX32(me,AE_ADD32(xe0,ye0));
        me=AE_MAX32(me,AE_ADD32(xe1,ye1));
    }
    me=AE_MAX32(me,AE_SEL32_LH(me,me));
    maxexp=AE_MOVAD16_0(AE_SAT16X4(me,me));
    /* accumulate products with proper right shift */
    a=AE_ZERO64();
    __Pragma("loop_count min=0,max=3")
    __Pragma("no_unroll")
    for(n=0; n<(N&3); n++) 
    {
        ae_int64 t;
        int sh;
        sh=AE_MOVAD16_0(AE_SUB16S(AE_ADD16S(*xexp++,*yexp++),(ae_int16x4)(int16_t)(maxexp)));
        t=AE_MULF32RA_HH(*xmant++,*ymant++);    /* accumulate in Q47 */
        a=AE_ADD64S(a,AE_SLAA64S(t,sh));
    }
    N&=~3;
    /* process the most data block by 4-element portions */
    ae_int64 a0=AE_ZERO64();
    aXexp =AE_LA64_PP(castxcc(ae_int16x4,xexp ));
    aXmant=AE_LA64_PP(castxcc(ae_int32x2,xmant));
    aYexp =AE_LA64_PP(castxcc(ae_int16x4,yexp ));
    aYmant=AE_LA64_PP(castxcc(ae_int32x2,ymant));
    for(n=0; n<(N>>2); n++) 
    {
        ae_int16x4 sh,xsh,ysh;
        ae_int32x2 x0,x1,y0,y1;
        ae_int64 t0,t1,t2,t3;
        AE_LA16X4_IP(xsh,aXexp,castxcc(ae_int16x4,xexp ));
        AE_LA16X4_IP(ysh,aYexp,castxcc(ae_int16x4,yexp ));
        sh=AE_SUB16S(AE_ADD16S(xsh,ysh),(ae_int16x4)(int16_t)maxexp);
        AE_LA32X2_IP(x0,aXmant,castxcc(ae_int32x2,xmant));
        AE_LA32X2_IP(x1,aXmant,castxcc(ae_int32x2,xmant));
        AE_LA32X2_IP(y0,aYmant,castxcc(ae_int32x2,ymant));
        AE_LA32X2_IP(y1,aYmant,castxcc(ae_int32x2,ymant));
        t0=AE_SLAA64S(AE_MULF32RA_HH(x0,y0),AE_MOVAD16_3(sh));
        t1=AE_SLAA64S(AE_MULF32RA_LL(x0,y0),AE_MOVAD16_2(sh));
        t2=AE_SLAA64S(AE_MULF32RA_HH(x1,y1),AE_MOVAD16_1(sh));
        t3=AE_SLAA64S(AE_MULF32RA_LL(x1,y1),AE_MOVAD16_0(sh));
        a =AE_ADD64S(a,t0);
        a =AE_ADD64S(a,t1);
        a =AE_ADD64S(a,t2);
        a =AE_ADD64S(a,t3);
    }
    a=AE_ADD64S(a,a0);

    /* normalize Q47 result */
    {
        int nsa0;
        ae_int32x2 nsa,x;
        nsa0=AE_NSA64(a);
        x=AE_TRUNCA32X2F64S(a,a,nsa0);
        nsa=AE_MOVDA32(maxexp-nsa0+16);
        /* underflow processing */
        AE_MOVT32X2(x,0,AE_LT32(nsa,MIN_INT16));
        /* overflow processing */
        AE_MOVT32X2(x,AE_SLLI32S(x,31),AE_LT32(MAX_INT16,nsa));
        AE_S32_L_IP(x,castxcc(ae_int32,zmant),sizeof(int32_t));
        AE_S16_0_IP(AE_SAT16X4(nsa,nsa),castxcc(ae_int16,zexp),sizeof(int16_t));
    }
}
