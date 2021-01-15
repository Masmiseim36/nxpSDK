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

/* DSP Library API */
#include "NatureDSP_Signal_math.h"
#include "common.h"


/*===========================================================================
  Vector matematics:
  vec_divide           Division of Q31/Q15 Numbers
===========================================================================*/

/*-------------------------------------------------------------------------
  Division
  These routines perform pair-wise division of vectors written in Q63, Q31 or 
  Q15 format. They return the fractional and exponential portion of the division 
  result. Since the division may generate result greater than 1, it returns 
  fractional portion frac in Q(63-exp), Q(31-exp) or Q(15-exp) format and 
  exponent exp so true division result in the Q0.31 may be found by shifting 
  fractional part left by exponent value.
  Additional routine makes integer division of 64-bit number to 32-bit 
  denominator forming 32-bit result. If result is overflown, 0x7fffffff 
  or 0x80000000 is returned depending on the signs of inputs.
  For division to 0, the result is not defined.

  Two versions of routines are available: regular versions (vec_divide64x32i,
  vec_divide64x64, vec_divide32x32, vec_divide16x16) work 
  with arbitrary arguments, faster versions (vec_divide32x32_fast, 
  vec_divide16x16_fast) apply some restrictions.

  Accuracy is measured as accuracy of fractional part (mantissa):
  vec_divide64x32i, scl_divide64x32                      :  1 LSB   
  vec_divide64x64                                        :  2 LSB 
  vec_divide32x32, vec_divide32x32_fast                  :  2 LSB (1.8e-9) 
  scl_divide32x32                                        :  2 LSB (4.8e-7) 
  vec_divide16x16, scl_divide16x16, vec_divide16x16_fast :  2 LSB (1.2e-4)

  Precision: 
  64x32i integer division, 64-bit nominator, 32-bit denominator, 32-bit output. 
  64x64  fractional division, 64-bit inputs, 64-bit output. 
  32x32  fractional division, 32-bit inputs, 32-bit output. 
  16x16  fractional division, 16-bit inputs, 16-bit output. 

  Input:
  x[N]    nominator, 64-bit integer, Q63, Q31 or Q15
  y[N]    denominator, 32-bit integer, Q63, Q31 or Q15
  N       length of vectors
  Output:
  frac[N] fractional parts of result, Q(63-exp), Q(31-exp) or Q(15-exp)
  exp[N]  exponents of result 

  Restriction:
  For regular versions (vec_divide64x32i, vec_divide64x64, vec_divide32x32,
  vec_divide16x16) :
  x,y,frac,exp should not overlap

  For faster versions (vec_divide32x32_fast, vec_divide16x16_fast) :
  x,y,frac,exp  should not overlap
  x,y,frac      to be aligned by 8-byte boundary, N - multiple of 4.

  Scalar versions:
  ----------------
  scl_divide64x32(): integer remainder
  Return packed value: 
  scl_divide64x64():
  bits 55:0 fractional part
  bits 63:56 exponent
  scl_divide32x32():
  bits 23:0 fractional part
  bits 31:24 exponent
  scl_divide16x16():
  bits 15:0 fractional part
  bits 31:16 exponent
-------------------------------------------------------------------------*/
#if 0
#include "recip_table.h"
void vec_divide16x16_fast 
(
/*algorithm:
  f(x,y) = div(x,y) = x/y = x*recip(y)

1)  f(y) = recip(y) = 1/y
Rough calculation:
  f(y) = f(y0) + f'(y0)*(y1-y0)
  1/y0 = f(y0)
  1/(y0^2) = f'(y0)
  y1 = y*2^nsa, y1 in 0.5..1
  y1-y0 = dy
2)f(x,y) = x*recip(y)
  */
  int16_t *       restrict  frac,
  int16_t *       restrict  exp,
  const int16_t * restrict  x,
  const int16_t * restrict  y,
  int N
)
{
  int   n, a_nsa, b_nsa, c_nsa, d_nsa;
  int   a_nsy, b_nsy, c_nsy, d_nsy;
  int   ind1, ind2, ind3, ind4;
  int   nsa, nsb, nsc, nsd; 

  const ae_int16x4 * restrict px = (const ae_int16x4 *)x;
  const ae_int16x4 * restrict py = (const ae_int16x4 *) y;
        ae_int16x4 * restrict pf = (      ae_int16x4 *)frac;
        ae_int16x4 * restrict pe = (      ae_int16x4 *)exp;

  ae_int16x4  vxh,vah,vbh,vch,vdh,vyh;
  
  ae_f32x2    vaf,vcf;
  ae_int32x2  vxa,vxb,vxc,vxd;
  ae_int16x4  vya,vyb,vyc,vyd;
  ae_int32x2  y0, y1;
  ae_int32x2  vf1,vf2,vf3,vf4,vff,vfe;
  ae_int32x2 ab_ns,cd_ns;
  ae_int32x2 ab,cd,nsab,nscd;
  ae_int16x4 abcd,abcd_;

  xtbool4     sgnx, sgny, infy;
  ae_int16x4  z0, z1, z2, z3, z4, z5;

  NASSERT_ALIGN8(px);
  NASSERT_ALIGN8(py);
  NASSERT_ALIGN8(pf);
  NASSERT_ALIGN8(pe);
  NASSERT((N&3)==0);

  z0 = AE_ZERO16();
  z1 = AE_MOVDA16(0xFFFC);
  z2 = AE_MOVDA16(0x4000);
  z3 = AE_MOVDA16(16);
  z5 = AE_MOVDA16(0x8000);
  //__Pragma("concurrent");
  __Pragma("loop_count min=1")
  for (n=0; n<(N>>2); ++n)
  {
    /*Load 4 nominators*/
    AE_L16X4_IP(vxh, px, sizeof(ae_int16x4));
    /*Load 4 denominators*/
    AE_L16X4_IP(vyh, py, sizeof(ae_int16x4));

    sgnx = AE_LT16(vxh,z0);
    z4 = AE_MOVDA16(0x7FFF);
    AE_MOVT16X4(z4, z5, sgnx);

    sgny = AE_LT16(vyh,z0);
    vyh = AE_ABS16S(vyh);
    infy= AE_EQ16(vyh ,z0);

    vah = vxh;
    vbh = AE_SEL16_2301(vxh,vxh);
    vch = AE_SEL16_5342(vxh,vxh);
    vdh = AE_SEL16_6543(vxh,vxh);

    vya = vyh;
    vyb = AE_SEL16_2301(vyh,vyh);
    vyc = AE_SEL16_5342(vyh,vyh);
    vyd = AE_SEL16_6543(vyh,vyh);


    a_nsa = AE_NSAZ16_0(vah);
    b_nsa = AE_NSAZ16_0(vbh);
    c_nsa = AE_NSAZ16_0(vch);
    d_nsa = AE_NSAZ16_0(vdh);

    a_nsy = AE_NSAZ16_0(vya);
    b_nsy = AE_NSAZ16_0(vyb);
    c_nsy = AE_NSAZ16_0(vyc);
    d_nsy = AE_NSAZ16_0(vyd);

    /*Normalize x,y*/
    vah = AE_SLAA16S(vah,a_nsa);
    vbh = AE_SLAA16S(vbh,b_nsa);
    vch = AE_SLAA16S(vch,c_nsa);
    vdh = AE_SLAA16S(vdh,d_nsa);

    vya = AE_SLAA16S(vya,a_nsy);
    vyb = AE_SLAA16S(vyb,b_nsy);
    vyc = AE_SLAA16S(vyc,c_nsy);
    vyd = AE_SLAA16S(vyd,d_nsy);

    vya = AE_SEL16_5140(vya,vyb);
    vyc = AE_SEL16_5140(vyc,vyd);
    vya = AE_SEL16_5410(vya,vyc);

    vah = AE_SEL16_5140(vah,vbh);
    vch = AE_SEL16_5140(vch,vdh);
    vah = AE_SEL16_5410(vah,vch);
    vah = AE_SEL16_2301(vah,vah);

    a_nsy -= a_nsa;
    b_nsy -= b_nsa;
    c_nsy -= c_nsa;
    d_nsy -= d_nsa;
    

    /*Calculate reciprocal:
    f(y) = f(y0) + f'(y0)*(y1-y0)
    1/y0 = f(y0)
    1/(y0^2) = f'(y0)
    y1 = y*2^nsa, y1 in 0.5..1
    y1-y0 = dy
    */

    vyc = AE_SUB16(vya,z2);
    vyb = AE_SRAI16(vyc,5);
    vyb = AE_AND16(vyb,z1);

    ind1 = AE_MOVAD16_0(vyb);
    ind2 = AE_MOVAD16_1(vyb);
    ind3 = AE_MOVAD16_2(vyb);
    ind4 = AE_MOVAD16_3(vyb);

    vf1 = AE_L16X2M_X((const ae_p16x2s *)recip_table, ind1);
    vf2 = AE_L16X2M_X((const ae_p16x2s *)recip_table, ind2);
    vf3 = AE_L16X2M_X((const ae_p16x2s *)recip_table, ind3);
    vf4 = AE_L16X2M_X((const ae_p16x2s *)recip_table, ind4);

    vff = AE_SEL32_LL(vf1,vf2);/*f'=(1/y0^2),Q.23*/
    vaf = AE_SEL32_HH(vf1,vf2);/*f=1/y0,Q.23*/
    vfe = AE_SEL32_LL(vf3,vf4);/*f'=(1/y0^2),Q.23*/
    vcf = AE_SEL32_HH(vf3,vf4);/*f=1/y0,Q.23*/

    vyb = AE_SLAI16S(vyb,5);

    vyb = AE_SUB16S(vyc,vyb);/*dy = y1-y0,Q1.14*/
    vyb = AE_SLAI16S(vyb,1);
    vyb = AE_SEL16_2301(vyb,vyb);

    AE_MULSFP32X16X2RAS_L(vaf,vff,vyb);/*recip(y1) = 1/y0 - (1/y0^2)*dy,Q1.23*/
    AE_MULSFP32X16X2RAS_H(vcf,vfe,vyb);/*recip(y1) = 1/y0 - (1/y0^2)*dy,Q1.23*/
    y0 = AE_SLAI32(vaf, 8); /*Q1.31*/
    y1 = AE_SLAI32(vcf, 8); /*Q1.31*/

    vaf = AE_MULFP32X16X2RAS_L(y0,vah); /*x*recip(y), Q.31*/
    vcf = AE_MULFP32X16X2RAS_H(y1,vah); /*x*recip(y), Q.31*/

    vxb = (vaf);
    vxd = (vcf);
    vxa = AE_SEL32_HH(vaf,vaf);
    vxc = AE_SEL32_HH(vcf,vcf);
   
    nsa = AE_NSAZ32_L(vxa);
    nsb = AE_NSAZ32_L(vxb);
    nsc = AE_NSAZ32_L(vxc);
    nsd = AE_NSAZ32_L(vxd);
    //Calculate exponential part
    nsab=AE_MOVDA32X2(nsa,nsb);
    nscd=AE_MOVDA32X2(nsc,nsd);
    ab_ns=AE_MOVDA32X2(d_nsy,c_nsy);
    cd_ns=AE_MOVDA32X2(b_nsy,a_nsy);
    abcd =AE_CVT16X4(nsab,nscd);
    abcd_=AE_CVT16X4(ab_ns,cd_ns);
    abcd_=AE_SUB16(abcd_,abcd);
    abcd_=AE_ADD16(abcd_,AE_MOVDA16(1));

    //Calculate fractional part
    AE_MOVSARD7(nsab);
    ab=AE_SLAS32S(vaf);
    AE_MOVSARD7(nscd);
    cd=AE_SLAS32S(vcf);
    vxh =AE_SEL16_7531(AE_MOVINT16X4_FROMF32X2(ab),AE_MOVINT16X4_FROMF32X2(cd));
                                 
    AE_MOVT16X4(vah, z3, infy);
    AE_MOVT16X4(vxh, z4, infy);
    vyh = AE_NEG16S(vxh);
    AE_MOVT16X4(vxh, vyh, sgny);

    AE_S16X4_IP(abcd_, pe, 8);//put exp
    AE_S16X4_IP(vxh, pf, sizeof(ae_int16x4));//put frac 
  }  
}
#else
void vec_divide16x16_fast 
(
  int16_t *       restrict  frac,
  int16_t *       restrict  exp,
  const int16_t * restrict  x,
  const int16_t * restrict  y,
  int M)
#if 0
{
#define SCR_SZ (MAX_ALLOCA_SZ/sizeof(int16_t))
    int16_t ALIGN(8) scr[SCR_SZ];   /* local scratch */
    xtbool4 mask5;
    ae_int16x4 X,Y,Z,E;
    int n,N;
    const ae_int16x4 * restrict px;
    const ae_int16x4 * restrict py;
          ae_int16x4 * restrict pf;
          ae_int16x4 * restrict ps;
    const ae_int16x4 * restrict pfRd;
          ae_int16x4 * restrict pfWr;
    const ae_int16x4 * restrict psRd;
    static const int16_t ALIGN(8) _0101[]={0,1,0,1};
    mask5=AE_EQ16(AE_L16X4_I((const ae_int16x4*)_0101,0),AE_ZERO16());
    while(M>0)
    {
        N=XT_MIN(SCR_SZ,M); /* size of portion */
        NASSERT_ALIGN(x,8);
        NASSERT_ALIGN(y,8);
        NASSERT_ALIGN(frac,8);
        NASSERT(N%4==0);
        /* take exponent and normalize inputs. Y is saved to the scratch */
        px=(const ae_int16x4 *)x;
        py=(const ae_int16x4 *)y;
        pf=(      ae_int16x4*)frac;
        ps=(      ae_int16x4*)scr;
        for (n=0; n<(N>>2); n++)
        {
            ae_int16x4 X0,X1,X2,X3,Y0,Y1,Y2,Y3;
            int expx,expy;
            AE_L16X4_IP(X,px,sizeof(X));
            AE_L16X4_IP(Y,py,sizeof(Y));
            expx = AE_NSAZ16_0(X);
            expy = AE_NSAZ16_0(Y);
            X0=AE_SLAA16S(X,expx);
            Y0=AE_SLAA16S(Y,expy);
            exp[3]=(int16_t)(expy-expx+1);
            X=AE_SEL16_4321(X,X);
            Y=AE_SEL16_4321(Y,Y);

            expx = AE_NSAZ16_0(X);
            expy = AE_NSAZ16_0(Y);
            X1=AE_SLAA16S(X,expx);
            Y1=AE_SLAA16S(Y,expy);
            exp[2]=(int16_t)(expy-expx+1);
            X=AE_SEL16_4321(X,X);
            Y=AE_SEL16_4321(Y,Y);

            expx = AE_NSAZ16_0(X);
            expy = AE_NSAZ16_0(Y);
            X2=AE_SLAA16S(X,expx);
            Y2=AE_SLAA16S(Y,expy);
            exp[1]=(int16_t)(expy-expx+1);
            X=AE_SEL16_4321(X,X);
            Y=AE_SEL16_4321(Y,Y);

            expx = AE_NSAZ16_0(X);
            expy = AE_NSAZ16_0(Y);
            X3=AE_SLAA16S(X,expx);
            Y3=AE_SLAA16S(Y,expy);

            X =AE_SEL16_6420(X2,X0);
            X1=AE_SEL16_6420(X3,X1);
            X1=AE_SEL16_6543(X1,X1);
            AE_MOVT16X4(X ,X1,mask5);
            AE_S16X4_IP(X,pf,sizeof(X));
            Y =AE_SEL16_6420(Y2,Y0);
            Y1=AE_SEL16_6420(Y3,Y1);
            Y1=AE_SEL16_6543(Y1,Y1);
            AE_MOVT16X4(Y ,Y1,mask5);
            AE_S16X4_IP(Y,ps,sizeof(Y));

            exp[0]=(int16_t)(expy-expx+1);
            exp+=4;
        }
        __Pragma("no_reorder");
        pfRd = (const ae_int16x4*)frac;
        pfWr = (      ae_int16x4*)frac;
        psRd = (const ae_int16x4*)scr;
        __Pragma("loop_count min=1")
        for(n=0; n<(N>>2); n++)
        {
            xtbool4 sy;
            ae_int16x4 _0x4000=AE_MOVDA16(16384);
            AE_L16X4_IP(Y,psRd,sizeof(Y));
            sy=AE_LT16(Y,AE_ZERO16());
            Y=AE_ABS16S_vector(Y);
            /* first approximation */
            Z=AE_SUB16(AE_MOVDA16((int16_t)47852),Y); 
             /* 3 iterations to achieve 1 LSB accuracy in mantissa */
            E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(Y,Z)); 
            E=AE_ADD16(E,E);
            Z=AE_ADD16S_vector(Z,AE_MULFP16X4S_vector(Z,E));
            E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(Y,Z)); 
            E=AE_ADD16(E,E);
            Z=AE_ADD16S_vector(Z,AE_MULFP16X4S_vector(Z,E));
            E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(Y,Z)); 
            E=AE_ADD16(E,E);
            Z=AE_ADD16S_vector(Z,AE_MULFP16X4S_vector(Z,E));
            /* restore original sign */
            Y=AE_NEG16S_vector(Z);
            AE_MOVT16X4(Z,Y,sy);
            /* multiply by X */
            AE_L16X4_IP(X,pfRd,sizeof(X));
            Z=AE_MULFP16X4RAS(X,Z);
            AE_S16X4_IP(Z,pfWr,sizeof(Z));
        }
        /* process next portion */
        M-=N;
        x+=N;
        y+=N;
        frac+=N;
        exp+=N;
    }
}
#else
{
    ae_int16x4 X,Y,Z,E;
    int n,N=M;
    xtbool4 mask5;
    const ae_int16x4 * restrict px;
    const ae_int16x4 * restrict py;
          ae_int16x4 * restrict pf;
    static const int16_t ALIGN(8) _0101[]={0,1,0,1};
    mask5=AE_EQ16(AE_L16X4_I((const ae_int16x4*)_0101,0),AE_ZERO16());
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT_ALIGN(frac,8);
    NASSERT(N%4==0);

    if(N<=0) return;
    /* take exponent and normalize inputs. Y is saved to the scratch */
    px=(const ae_int16x4 *)x;
    py=(const ae_int16x4 *)y;
    pf=(      ae_int16x4*)frac;
    for (n=0; n<(N>>2); n++)
    {
        ae_int16x4 X0,X1,X2,X3,Y0,Y1,Y2,Y3;
        xtbool4 sy;
        ae_int16x4 _0x4000=AE_MOVDA16(16384);
        int expx,expy;
        AE_L16X4_IP(X,px,sizeof(X));
        AE_L16X4_IP(Y,py,sizeof(Y));
        expx = AE_NSAZ16_0(X);
        expy = AE_NSAZ16_0(Y);
        X0=AE_SLAA16S(X,expx);
        Y0=AE_SLAA16S(Y,expy);
        exp[3]=(int16_t)(expy-expx+1);
        X=AE_SEL16_4321(X,X);
        Y=AE_SEL16_4321(Y,Y);

        expx = AE_NSAZ16_0(X);
        expy = AE_NSAZ16_0(Y);
        X1=AE_SLAA16S(X,expx);
        Y1=AE_SLAA16S(Y,expy);
        exp[2]=(int16_t)(expy-expx+1);
        X=AE_SEL16_4321(X,X);
        Y=AE_SEL16_4321(Y,Y);

        expx = AE_NSAZ16_0(X);
        expy = AE_NSAZ16_0(Y);
        X2=AE_SLAA16S(X,expx);
        Y2=AE_SLAA16S(Y,expy);
        exp[1]=(int16_t)(expy-expx+1);
        X=AE_SEL16_4321(X,X);
        Y=AE_SEL16_4321(Y,Y);

        expx = AE_NSAZ16_0(X);
        expy = AE_NSAZ16_0(Y);
        X3=AE_SLAA16S(X,expx);
        Y3=AE_SLAA16S(Y,expy);
        exp[0]=(int16_t)(expy-expx+1);
        exp+=4;

        X =AE_SEL16_6420(X2,X0);
        X1=AE_SEL16_6420(X3,X1);
        X1=AE_SEL16_6543(X1,X1);
        AE_MOVT16X4(X ,X1,mask5);
        Y =AE_SEL16_6420(Y2,Y0);
        Y1=AE_SEL16_6420(Y3,Y1);
        Y1=AE_SEL16_6543(Y1,Y1);
        AE_MOVT16X4(Y ,Y1,mask5);

        sy=AE_LT16(Y,AE_ZERO16());
        Y=AE_ABS16S_vector(Y);
        /* first approximation */
        Z=AE_SUB16(AE_MOVDA16((int16_t)47852),Y); 
            /* 3 iterations to achieve 1 LSB accuracy in mantissa */
        E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(Y,Z)); 
        E=AE_ADD16(E,E);
        Z=AE_ADD16S_vector(Z,AE_MULFP16X4S_vector(Z,E));
        E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(Y,Z)); 
        E=AE_ADD16(E,E);
        Z=AE_ADD16S_vector(Z,AE_MULFP16X4S_vector(Z,E));
        E=AE_SUB16(_0x4000,AE_MULFP16X4S_vector(Y,Z)); 
        E=AE_ADD16(E,E);
        Z=AE_ADD16S_vector(Z,AE_MULFP16X4S_vector(Z,E));
        /* restore original sign */
        Y=AE_NEG16S_vector(Z);
        AE_MOVT16X4(Z,Y,sy);
        /* multiply by X */
        Z=AE_MULFP16X4RAS(X,Z);
        AE_S16X4_IP(Z,pf,sizeof(Z));
    }
}
#endif
#endif
