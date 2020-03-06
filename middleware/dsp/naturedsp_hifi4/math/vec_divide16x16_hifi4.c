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
void vec_divide16x16 
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
  
  ae_f32x2    vdf,vaf,vbf,vcf;
  ae_int32x2  vxa,vxb,vxc,vxd;
  ae_int16x4  vya,vyb,vyc,vyd;
  ae_int32x2  y0, y1;
  ae_int32x2  vf1,vf2,vf3,vf4,vff,vfe;

  ae_valign   x_align, y_align, f_align, e_align;
  xtbool4     sgnx, sgny, infy;
  ae_int16x4  z0, z1, z2, z3, z4, z5;

  z0 = AE_ZERO16();
  z1 = AE_MOVDA16(0xFFFC);
  z2 = AE_MOVDA16(0x4000);
  z3 = AE_MOVDA16(16);
  z5 = AE_MOVDA16(0x8000);

  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);
  f_align = AE_ZALIGN64();
  e_align = AE_ZALIGN64();
  //__Pragma("concurrent");
  for (n=0; n<(N>>2); ++n)
  {
    /*Load 4 nominators*/
    AE_LA16X4_IP(vxh, x_align, px);
    /*Load 4 denominators*/
    AE_LA16X4_IP(vyh, y_align, py);

    sgnx = AE_LT16(vxh,z0);
    z4 = AE_MOVDA16(0x7FFF);
    AE_MOVT16X4(z4, z5, sgnx);

    sgny = AE_LT16(vyh,z0);
    vyh = AE_ABS16S(vyh);

    vah = vxh;
    vbh = AE_SEL16_2301(vxh,vxh);
    vch = AE_SEL16_5342(vxh,vxh);
    vdh = AE_SEL16_6543(vxh,vxh);

    vya = vyh;
    vyb = AE_SEL16_2301(vyh,vyh);
    vyc = AE_SEL16_5342(vyh,vyh);
    vyd = AE_SEL16_6543(vyh,vyh);

    infy = AE_EQ16(vya,z0);

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
    vf1 = AE_SEL32_HH(vf1,vf2);/*f=1/y0,Q.23*/
    vfe = AE_SEL32_LL(vf3,vf4);/*f'=(1/y0^2),Q.23*/
    vf3 = AE_SEL32_HH(vf3,vf4);/*f=1/y0,Q.23*/

    vyb = AE_SLAI16S(vyb,5);

    vyb = AE_SUB16S(vyc,vyb);/*dy = y1-y0,Q1.14*/
    vyb = AE_SLAI16S(vyb,1);
    vyb = AE_SEL16_2301(vyb,vyb);

    vbf = (vff);
    vdf = (vfe);
    vaf = (vf1);
    vcf = (vf3);

    AE_MULSFP32X16X2RAS_L(vaf,vff,vyb);/*recip(y1) = 1/y0 - (1/y0^2)*dy,Q1.23*/
    AE_MULSFP32X16X2RAS_H(vcf,vfe,vyb);/*recip(y1) = 1/y0 - (1/y0^2)*dy,Q1.23*/
    y0 = AE_SLAI32(vaf, 8); /*Q1.31*/
    y1 = AE_SLAI32(vcf, 8); /*Q1.31*/

    vaf = AE_MULFP32X16X2RAS_L(y0,vah); /*x*recip(y), Q.31*/
    vcf = AE_MULFP32X16X2RAS_H(y1,vah); /*x*recip(y), Q.31*/

    vxb = (vaf);
    vxd = (vcf);

    vxa = AE_SEL32_HH(vxb,vxb);
    vxc = AE_SEL32_HH(vxd,vxd);
   
    nsa = AE_NSAZ32_L(vxa);
    nsb = AE_NSAZ32_L(vxb);
    nsc = AE_NSAZ32_L(vxc);
    nsd = AE_NSAZ32_L(vxd);
    //Calculate exponential part
    d_nsa = d_nsy-nsa+1;
    c_nsa = c_nsy-nsb+1;
    vah = AE_MOVDA16X2(d_nsa,c_nsa);
    b_nsa = b_nsy-nsc+1;
    a_nsa = a_nsy-nsd+1;
    vbh = AE_MOVDA16X2(b_nsa,a_nsa);
    vah = AE_SEL16_5410(vah,vbh);
                                  
    //Calculate fractional part
    vxa = AE_SLAA32S(vxa,nsa);
    vxb = AE_SLAA32S(vxb,nsb);
    vxc = AE_SLAA32S(vxc,nsc);
    vxd = AE_SLAA32S(vxd,nsd);
                                  
    vxa = AE_SEL32_HL(vxa,vxb);
    vxc = AE_SEL32_HL(vxc,vxd);
    vxa = AE_SRAI32(vxa,16);
    vxc = AE_SRAI32(vxc,16);
    vxh = AE_CVT16X4(vxa,vxc);
                                 
    AE_MOVT16X4(vah, z3, infy);
    AE_MOVT16X4(vxh, z4, infy);
    vyh = AE_NEG16S(vxh);
    AE_MOVT16X4(vxh, vyh, sgny);

    AE_SA16X4_IP(vah, e_align, pe);//put exp
    AE_SA16X4_IP(vxh, f_align, pf);//put frac 
  }  
  AE_SA64POS_FP(e_align,pe);
  AE_SA64POS_FP(f_align,pf);

  switch(N&3)
  {
    case 1:
      AE_L16_IP(vxh,castxcc(ae_int16,px), 2);
      AE_L16_IP(vyh,castxcc(ae_int16,py), 2);

      sgnx = AE_LT16(vxh,z0);
      z4 = AE_MOVDA16(0x7FFF);
      AE_MOVT16X4(z4, z5, sgnx);

      sgny = AE_LT16(vyh,z0);
      vyh = AE_ABS16S(vyh);

      infy = AE_EQ16(vyh,z0);

      a_nsa = AE_NSAZ16_0(vxh);
      a_nsy = AE_NSAZ16_0(vyh);


      /*Normalize x,y*/
      vah = AE_SLAA16S(vxh,a_nsa);
      vya = AE_SLAA16S(vyh,a_nsy);

      a_nsy -= a_nsa;
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

      vf1 = AE_L16X2M_X((const ae_p16x2s *)recip_table, ind1);

      vff = AE_SEL32_LL(vf1,vf1);/*f'=(1/y0^2),Q.23*/
      vf1 = AE_SEL32_HH(vf1,vf1);/*f=1/y0,Q.23*/

      vyb = AE_SLAI16S(vyb,5);

      vyb = AE_SUB16S(vyc,vyb);/*dy = y1-y0,Q1.14*/
      vyb = AE_SLAI16S(vyb,1);

      vaf = (vf1);
      AE_MULSFP32X16X2RAS_L(vaf,vff,vyb);/*recip(y1) = 1/y0 - (1/y0^2)*dy,Q1.23*/
      y0 = AE_SLAI32(vaf, 8); /*Q1.31*/

      vaf = AE_MULFP32X16X2RAS_L(y0,vah); /*x*recip(y), Q.31*/

      vxb = (vaf);
      vxa = AE_SEL32_LL(vxb,vxb);

      nsa = AE_NSAZ32_L(vxa);

      //Calculate exponential part
      a_nsa = a_nsy-nsa+1;
      vah = AE_MOVDA16(a_nsa);
                                    
      //Calculate fractional part
      vxa = AE_SLAA32S(vxa,nsa);
      vxa = AE_SRAI32(vxa,16);
      vxh = AE_CVT16X4(vxa,vxa);
                                   
      AE_MOVT16X4(vah, z3, infy);
      AE_MOVT16X4(vxh, z4, infy);
      vyh = AE_NEG16S(vxh);
      AE_MOVT16X4(vxh, vyh, sgny);

      AE_S16_0_IP(vah, castxcc(ae_int16,pe), 0);
      AE_S16_0_IP(vxh, castxcc(ae_int16,pf), 0);
      break;
    case 2:
      AE_L16_IP(vah,castxcc(ae_int16,px), 2);
      AE_L16_IP(vbh,castxcc(ae_int16,px), 0);
      AE_L16_IP(vch,castxcc(ae_int16,py), 2);
      AE_L16_IP(vdh,castxcc(ae_int16,py), 0);

      vxh = AE_SEL16_5140(vah,vbh);
      vyh = AE_SEL16_5140(vch,vdh);

      sgnx = AE_LT16(vxh,z0);
      z4 = AE_MOVDA16(0x7FFF);
      AE_MOVT16X4(z4, z5, sgnx);

      sgny = AE_LT16(vyh,z0);
      vyh = AE_ABS16S(vyh);

      vah = vxh;
      vbh = AE_SEL16_2301(vxh,vxh);

      vya = vyh;
      vyb = AE_SEL16_2301(vyh,vyh);

      infy = AE_EQ16(vya,z0);

      a_nsa = AE_NSAZ16_0(vah);
      b_nsa = AE_NSAZ16_0(vbh);

      a_nsy = AE_NSAZ16_0(vya);
      b_nsy = AE_NSAZ16_0(vyb);

      /*Normalize x,y*/
      vah = AE_SLAA16S(vah,a_nsa);
      vbh = AE_SLAA16S(vbh,b_nsa);

      vya = AE_SLAA16S(vya,a_nsy);
      vyb = AE_SLAA16S(vyb,b_nsy);

      vya = AE_SEL16_5140(vya,vyb);
      vah = AE_SEL16_5140(vah,vbh);

      a_nsy -= a_nsa;
      b_nsy -= b_nsa;

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

      vf1 = AE_L16X2M_X((const ae_p16x2s *)recip_table, ind1);
      vf2 = AE_L16X2M_X((const ae_p16x2s *)recip_table, ind2);

      vff = AE_SEL32_LL(vf1,vf2);/*f'=(1/y0^2),Q.23*/
      vf1 = AE_SEL32_HH(vf1,vf2);/*f=1/y0,Q.23*/

      vyb = AE_SLAI16S(vyb,5);

      vyb = AE_SUB16S(vyc,vyb);/*dy = y1-y0,Q1.14*/
      vyb = AE_SLAI16S(vyb,1);
      vyb = AE_SEL16_2301(vyb,vyb);

      vaf = (vf1);
      vah = AE_SEL16_2301(vah,vah);

      AE_MULSFP32X16X2RAS_L(vaf,vff,vyb);/*recip(y1) = 1/y0 - (1/y0^2)*dy,Q1.23*/
      y0 = AE_SLAI32(vaf, 8); /*Q1.31*/

      vaf = AE_MULFP32X16X2RAS_L(y0,vah); /*x*recip(y), Q.31*/

      vxb = (vaf);

      vxa = AE_SEL32_HH(vxb,vxb);
     
      nsa = AE_NSAZ32_L(vxa);
      nsb = AE_NSAZ32_L(vxb);
      //Calculate exponential part
      b_nsa = b_nsy-nsa+1;
      a_nsa = a_nsy-nsb+1;
      vbh = AE_MOVDA16X2(b_nsa,a_nsa);
      vah = AE_SEL16_5410(vbh,vbh);
                                    
      //Calculate fractional part
      vxa = AE_SLAA32S(vxa,nsa);
      vxb = AE_SLAA32S(vxb,nsb);

                                    
      vxa = AE_SEL32_HL(vxa,vxb);
      vxa = AE_SRAI32(vxa,16);
      vxh = AE_CVT16X4(vxa,vxa);
                                   
      AE_MOVT16X4(vah, z3, infy);
      AE_MOVT16X4(vxh, z4, infy);
      vyh = AE_NEG16S(vxh);
      AE_MOVT16X4(vxh, vyh, sgny);

      vbh = AE_SEL16_2301(vah,vah);
      vdh = AE_SEL16_2301(vxh,vxh);

      AE_S16_0_IP(vbh, castxcc(ae_int16,pe), 2);
      AE_S16_0_IP(vah, castxcc(ae_int16,pe), 0);
      AE_S16_0_IP(vdh, castxcc(ae_int16,pf), 2);
      AE_S16_0_IP(vxh, castxcc(ae_int16,pf), 0);
      break;
    case 3:
      /*Load 4 nominators*/
      AE_L16_IP(vah,castxcc(ae_int16,px), 2);
      AE_L16_IP(vbh,castxcc(ae_int16,px), 2);
      AE_L16_IP(vch,castxcc(ae_int16,px), 0);

      vxh = AE_SEL16_5140(vah,vbh);
      vyh = AE_SEL16_5140(vch,vch);
      vxh = AE_SEL16_5410(vxh,vyh);


      /*Load 4 denominators*/
      AE_L16_IP(vah,castxcc(ae_int16,py), 2);
      AE_L16_IP(vbh,castxcc(ae_int16,py), 2);
      AE_L16_IP(vch,castxcc(ae_int16,py), 0);

      vah = AE_SEL16_5140(vah,vbh);
      vyh = AE_SEL16_5140(vch,vch);
      vyh = AE_SEL16_5410(vah,vyh);

      sgnx = AE_LT16(vxh,z0);
      z4 = AE_MOVDA16(0x7FFF);
      AE_MOVT16X4(z4, z5, sgnx);

      sgny = AE_LT16(vyh,z0);
      vyh = AE_ABS16S(vyh);

      vah = vxh;
      vbh = AE_SEL16_2301(vxh,vxh);
      vch = AE_SEL16_5342(vxh,vxh);
      vdh = AE_SEL16_6543(vxh,vxh);

      vya = vyh;
      vyb = AE_SEL16_2301(vyh,vyh);
      vyc = AE_SEL16_5342(vyh,vyh);
      vyd = AE_SEL16_6543(vyh,vyh);

      infy = AE_EQ16(vya,z0);

      b_nsa = AE_NSAZ16_0(vbh);
      c_nsa = AE_NSAZ16_0(vch);
      d_nsa = AE_NSAZ16_0(vdh);

      b_nsy = AE_NSAZ16_0(vyb);
      c_nsy = AE_NSAZ16_0(vyc);
      d_nsy = AE_NSAZ16_0(vyd);

      /*Normalize x,y*/
      vbh = AE_SLAA16S(vbh,b_nsa);
      vch = AE_SLAA16S(vch,c_nsa);
      vdh = AE_SLAA16S(vdh,d_nsa);

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

      vf1 = AE_L16X2M_X((const ae_p16x2s *)recip_table, ind1);
      vf2 = AE_L16X2M_X((const ae_p16x2s *)recip_table, ind2);
      vf3 = AE_L16X2M_X((const ae_p16x2s *)recip_table, ind3);


      vff = AE_SEL32_LL(vf1,vf2);/*f'=(1/y0^2),Q.23*/
      vf1 = AE_SEL32_HH(vf1,vf2);/*f=1/y0,Q.23*/
      vfe = AE_SEL32_LL(vf3,vf3);/*f'=(1/y0^2),Q.23*/
      vf3 = AE_SEL32_HH(vf3,vf3);/*f=1/y0,Q.23*/

      vyb = AE_SLAI16S(vyb,5);

      vyb = AE_SUB16S(vyc,vyb);/*dy = y1-y0,Q1.14*/
      vyb = AE_SLAI16S(vyb,1);
      vyb = AE_SEL16_2301(vyb,vyb);

      vbf = (vff);
      vdf = (vfe);
      vaf = (vf1);
      vcf = (vf3);

      AE_MULSFP32X16X2RAS_L(vaf,vff,vyb);/*recip(y1) = 1/y0 - (1/y0^2)*dy,Q1.23*/
      AE_MULSFP32X16X2RAS_H(vcf,vfe,vyb);/*recip(y1) = 1/y0 - (1/y0^2)*dy,Q1.23*/
      y0 = AE_SLAI32(vaf, 8); /*Q1.31*/
      y1 = AE_SLAI32(vcf, 8); /*Q1.31*/

      vaf = AE_MULFP32X16X2RAS_L(y0,vah); /*x*recip(y), Q.31*/
      vcf = AE_MULFP32X16X2RAS_H(y1,vah); /*x*recip(y), Q.31*/

      vxb = (vaf);
      vxd = (vcf);

      vxa = AE_SEL32_HH(vxb,vxb);
      vxc = AE_SEL32_HH(vxd,vxd);
     
      nsa = AE_NSAZ32_L(vxa);
      nsb = AE_NSAZ32_L(vxb);
      nsc = AE_NSAZ32_L(vxc);
 
      //Calculate exponential part
      d_nsa = d_nsy-nsa+1;
      c_nsa = c_nsy-nsb+1;
      vah = AE_MOVDA16X2(d_nsa,c_nsa);
      b_nsa = b_nsy-nsc+1;

      vbh = AE_MOVDA16X2(b_nsa,b_nsa);
      vah = AE_SEL16_5410(vah,vbh);
                                    
      //Calculate fractional part
      vxa = AE_SLAA32S(vxa,nsa);
      vxb = AE_SLAA32S(vxb,nsb);
      vxc = AE_SLAA32S(vxc,nsc);
 
      vxa = AE_SEL32_HL(vxa,vxb);
      vxc = AE_SEL32_HL(vxc,vxd);
      vxa = AE_SRAI32(vxa,16);
      vxc = AE_SRAI32(vxc,16);
      vxh = AE_CVT16X4(vxa,vxc);
                                   
      AE_MOVT16X4(vah, z3, infy);
      AE_MOVT16X4(vxh, z4, infy);
      vyh = AE_NEG16S(vxh);
      AE_MOVT16X4(vxh, vyh, sgny);

  
      vbh = AE_SEL16_6543(vah,vah);
      vch = AE_SEL16_5342(vah,vah);

      //put exp
      AE_S16_0_IP(vbh, castxcc(ae_int16,pe), 2);
      AE_S16_0_IP(vch, castxcc(ae_int16,pe), 2);
      AE_S16_0_IP(vah, castxcc(ae_int16,pe), 0);

      vah = AE_SEL16_6543(vxh,vxh);
      vbh = AE_SEL16_5342(vxh,vxh);
      vch = AE_SEL16_7531(vxh,vxh);
      //put frac 
      AE_S16_0_IP(vah, castxcc(ae_int16,pf), 2);
      AE_S16_0_IP(vbh, castxcc(ae_int16,pf), 2);
      AE_S16_0_IP(vch, castxcc(ae_int16,pf), 0);

      break;
    default:break;
  }
}
#endif
void vec_divide16x16 
(
  int16_t *       restrict  frac,
  int16_t *       restrict  exp,
  const int16_t * restrict  x,
  const int16_t * restrict  y,
  int M)
{
#define SCR_SZ (MAX_ALLOCA_SZ/sizeof(int16_t))
    int16_t ALIGN(8) scr[SCR_SZ];   /* local scratch */
    static const ALIGN(8) int16_t cnst0123[]={0,1,2,3};
    ae_int16x4 _0123;
    ae_int16x4 X,Y,Z,E;
    int n,N;
    const ae_int16   * restrict px;
    const ae_int16   * restrict py;
          ae_int16   * restrict pf;
          ae_int16   * restrict ps;
    const ae_int16x4 * restrict pfRd;
          ae_int16x4 * restrict pfWr;
    const ae_int16x4 * restrict psRd;
          ae_valign   wr_align,rd_align;

    while(M>0)
    {
        N=XT_MIN(SCR_SZ,M); /* size of portion */
        /* take exponent and normalize inputs. Y is saved to the scratch */
        px=(const ae_int16 *)x;
        py=(const ae_int16 *)y;
        pf=(      ae_int16 *)frac;
        ps=(      ae_int16 *)scr;
        for (n=0; n<N; n++)
        {
            int expx,expy;
            AE_L16_IP(X,px,sizeof(int16_t));
            AE_L16_IP(Y,py,sizeof(int16_t));
            expx = AE_NSAZ16_0(X);
            expy = AE_NSAZ16_0(Y);
            X=AE_SLAA16S(X,expx);
            Y=AE_SLAA16S(Y,expy);
            AE_S16_0_IP(X,pf,sizeof(int16_t));
            AE_S16_0_IP(Y,ps,sizeof(int16_t));
            exp[n]=(int16_t)(expy-expx+1);
        }
        __Pragma("no_reorder");
        pfRd = (const ae_int16x4*)frac;
        pfWr = (      ae_int16x4*)frac;
        psRd = (const ae_int16x4*)scr;
        wr_align=AE_ZALIGN64();
        rd_align=AE_LA64_PP(pfRd);
        _0123 = AE_L16X4_I((const ae_int16x4*)cnst0123,0);
        for(n=N; n>0; n-=4)
        {
            xtbool4 sy,wr_mask;
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
            AE_LA16X4_IP(X,rd_align,pfRd);
            Z=AE_MULFP16X4RAS(X,Z);
            wr_mask=AE_LT16(_0123,AE_MOVDA16(n)); /* compute mask for last incomplete iteration */
            AE_MOVT16X4(X,Z,wr_mask);
            AE_SA16X4_IP(X,wr_align,pfWr);
        }
        AE_SA64POS_FP(wr_align,pfWr);
        /* process next portion */
        M-=N;
        x+=N;
        y+=N;
        frac+=N;
        exp+=N;
    }
}
