/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "scl_sqrt_table.h"
#include "common.h"

/*===========================================================================
  Vector matematics:
  vec_sqrt          Square Root         
===========================================================================*/

/*-------------------------------------------------------------------------
  Square Root
  These routines calculate square root.
  NOTES: 
  1. Fixed point functions return 0x80000000 (for 24 and 32-bit functions), 
     0x8000 (for 16-bit functions) on negative argument
  2. For floating point function, whenever an input value is negative, 
     functions raise the "invalid" floating-point exception, assign EDOM 
     to errno and set output value to NaN. Negative zero is considered as
     a valid input, the result is also -0
  Two versions of functions available: regular version (vec_sqrt16x16, 
  vec_sqrt24x24, vec_sqrt32x32, vec_sqrtf) with arbitrary arguments and
  faster version (vec_sqrt24x24_fast, vec_sqrt32x32_fast) that apply some 
  restrictions.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: (2 LSB)
  24x24  24-bit inputs, 24-bit output. Accuracy: (2.6e-7*y+1LSB)
  32x32  32-bit inputs, 32-bit output. Accuracy: (2.6e-7*y+1LSB)
  f      floating point. Accuracy 1 ULP

  Input:
  x[N]  input data,Q31, Q15 or floating point
  N     length of vectors
  Output:
  y[N]  output data,Q31, Q15 or floating point

  Restriction:
  Regular versions (vec_sqrt24x24, vec_sqrt32x32):
  x,y - should not overlap

  Faster versions (vec_sqrt24x24_fast, vec_sqrt32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result, Q31, Q15 or floating point
-------------------------------------------------------------------------*/

void vec_sqrt32x32 (int32_t * restrict y, const int32_t * restrict x, int N)
{
          ae_int32x2   * restrict py  = (      ae_int32x2   *)y;
  const ae_int32     * restrict px  = (const ae_int32     *)x;
  int         n, nsaa, nsab, idxa, idxb, shft_a, shft_b;
  ae_valign      y_align;
  ae_int32x2  vcw, vxw, vzw, viw,vxa,vxb, vxc, vxd, t0, t1, c1, c4;
  ae_f32x2    vxf, vzf, vcf;
  xtbool2     inf_ab, zero;
         
  if (N<=0) return;
  vzw = AE_MOVI(0);
  c1 = AE_MOVI( ~1 );
  c4 = AE_MOVDA32X2( 0xFFFFFFF8,0xFFFFFFF8 );
  viw = AE_MOVDA32X2(MIN_INT32, MIN_INT32);
  y_align = AE_ZALIGN64();

  /*algorithm*/
  //   f(x) = sqrt(x)  
  //   f(x) = f(xo) + f'(x0)*(x-x0)
  //   x-x0=dx
  //   x in range 0..1
  //   x0 in range 0.25..1

  for (n=0; n<N-1; n+=2)
  {
    /*Load two input numbers */
    AE_L32_IP( vxa, px, +4 );
    AE_L32_IP( vxb, px, +4 );

    /* Normalize x to x0*/

    nsaa = AE_NSAZ32_L(vxa);
    nsab = AE_NSAZ32_L(vxb);

    t0 = AE_MOVDA32X2( nsaa, nsab );
    t1 = AE_AND32( t0, c1 );

    shft_a = AE_MOVAD32_H( t1 );
    shft_b = AE_MOVAD32_L( t1 );

    vxa = AE_SLAA32( vxa, shft_a );
    vxb = AE_SLAA32( vxb, shft_b );

    t0 = AE_SRAI32( t0, 1 );
    nsaa = AE_MOVAD32_H( t0 );
    nsab = AE_MOVAD32_L( t0 );

    vxw = AE_SEL32_HH( vxa, vxb );
    zero = AE_EQ32(vxw,vzw);
    inf_ab = AE_LE32(vxw, vzw);

    /*get table indexes*/
    vxa = AE_SRAI32(vxw, 21);
    vxa = AE_AND32( vxa, c4 );
    idxa=AE_MOVAD32_H(vxa); //
    idxb=AE_MOVAD32_L(vxa); //
    
    vxa = AE_L32X2_X((const ae_int32x2 *)sqrt_table, idxa); 
    vxb = AE_L32X2_X((const ae_int32x2 *)sqrt_table, idxb);

    vcw = AE_SEL32_LL(vxa, vxb);//f'(x0)
    vxc = AE_SEL32_HH(vxa,vxb);// f(x0)
    
    /* Calculate dx for first iteration*/  
    vzf = (vxc);//
    vxf = (vxw);
    AE_MULSFP32X2RAS(vxf,vzf, vzf);// 
    /*Calculate y in first iteration*/
    vcf = (vcw);//
    AE_MULAFP32X2RAS(vzf, vcf, vxf);// vzf = y,Q31
    vxf = (vxw);//
    /* Calculate dx for second iteration*/ 
    AE_MULSFP32X2RAS(vxf,vzf, vzf);//
    /*Calculate y in second iteration*/
    vcf = (vcw);//
    AE_MULAFP32X2RAS(vzf, vcf, vxf);// y, Q31

    vxc =  (vzf);//
    vxd = AE_SRAA32(vxc, nsab);
    vxc = AE_SRAA32(vxc, nsaa);//
    vxc = AE_SEL32_HL(vxc, vxd);// put answer for two numbers

    AE_MOVT32X2(vxc, viw, inf_ab);
    AE_MOVT32X2(vxc, vzw, zero);
    AE_SA32X2_IP(vxc, y_align, py);//
  }

  AE_SA64POS_FP(y_align, py);
  /* If N is odd - calculate last number*/
  if (N&1)
  {
    AE_L32_IP( vxa, px, +4 );
    zero = AE_EQ32(vxa, vzw);
    /* Normalize x to x0*/
    nsaa = AE_NSAZ32_L(vxa);
    shft_a = ( nsaa & ~1 );
    nsaa   = ( nsaa )/2;
    vxw = AE_SLAA32( vxa, shft_a );
    inf_ab = AE_LE32(vxw, vzw);
    /* get table indexes for next numbers*/
    vxa = AE_SRAI32(vxw, 24);
    vxa = AE_SLAI32(vxa, 3);
    idxa=AE_MOVAD32_H(vxa); //
    vcw = AE_L32X2_X((const ae_int32x2 *)sqrt_table, idxa); 
    vxb = AE_SEL32_LL(vcw, vcw); //f'(x0)
    vzf = (vcw);
    vxf = (vxw);
    AE_MULSFP32X2RAS(vxf,vzf, vzf);//dx
    vcf = (vxb);
    AE_MULAFP32X2RAS(vzf, vcf, vxf); //y 
    /*second iteration*/
    vxf = (vxw);
    AE_MULSFP32X2RAS(vxf,vzf, vzf);
    vcf = (vxb);
    AE_MULAFP32X2RAS(vzf, vcf, vxf); //y
    vxa =  (vzf);
    vxa = AE_SRAA32(vxa, nsaa);
    AE_MOVT32X2(vxa, viw, inf_ab);
    AE_MOVT32X2(vxa, vzw, zero);
    vxa = AE_SEL32_HH(vxa, vxa);
    AE_S32_L_I(vxa, (ae_int32 *)py, 0);
  }
}
