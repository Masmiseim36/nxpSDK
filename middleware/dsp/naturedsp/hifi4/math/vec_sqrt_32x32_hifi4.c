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
#include "sqrt_table.h"
#include "common.h"
#include "NatureDSP_Signal_math.h"

/*===========================================================================
  Vector matematics:
  vec_sqrt          Square Root         
===========================================================================*/

/*-------------------------------------------------------------------------
  Square Root
  These routines calculate square root.
  NOTE: functions return 0x80000000 on negative argument for 32-bit outputs
  or 0x8000 for 16-bit outputs.
  Two versions of functions available: regular version (vec_sqrt16x16, vec_sqrt32x32, 
  vec_sqrt32x16, vec_sqrt64x32) with arbitrary 
  arguments and faster version (vec_sqrt32x32_fast) that 
  apply some restrictions.

  Precision: 
  16x16  16-bit inputs, 16-bit output. Accuracy: 2LSB
  32x32  32-bit inputs, 32-bit output. Accuracy: (2.6e-7*y+1LSB)
  32x16  32-bit input, 16-bit output.  Accuracy: 2 LSB
  64x32  64-bit inputs, 32-bit output. Accuracy: 2LSB

  Input:
  x[N]  input data, Q15, Q31, Q63 
  N     length of vectors
  Output:
  y[N]  output data, Q15, Q31

  Restriction:
  Regular versions (vec_sqrt16x16, vec_sqrt32x32, vec_sqrt32x16, vec_sqrt64x32):
  x,y - should not overlap

  Faster versions (vec_sqrt32x32_fast):
  x,y - should not overlap
  x,y - aligned on 8-byte boundary
  N   - multiple of 2

  Scalar versions:
  ----------------
  return result, Q15, Q31
-------------------------------------------------------------------------*/
void vec_sqrt32x32 (int32_t * restrict y, const int32_t * restrict x, int N)
{
        ae_int32x2     * restrict py  = (      ae_int32x2   *)y;
  const ae_int32x2     * restrict px  = (const ae_int32x2   *)x;
  const ae_int32x2     * restrict tbl = (const ae_int32x2   *)sqrt_table;
  int         n, nsaa, nsab, idxa, idxb;
  ae_int32x2  vcw, vxw, zero, min_int32, vxa, vxb, vxc,  t0, t1, c1, c4;
  ae_f32x2    vxf, vzf;
  xtbool2     inf_ab, is_zero;
  ae_valign    x_align,y_align;
         
  if (N<=0) return;
  zero = AE_ZERO32();
  c1 = AE_MOVI( ~1 );
  c4 = AE_MOVDA32X2( 0xFFFFFFF8,0xFFFFFFF8 );
  min_int32 = AE_MOVDA32X2(MIN_INT32, MIN_INT32);

  /*algorithm*/
  //   f(x) = sqrt(x)  
  //   f(x) = f(xo) + f'(x0)*(x-x0)
  //   x-x0=dx
  //   x in range 0..1
  //   x0 in range 0.25..1
  x_align = AE_LA64_PP(px);
  y_align = AE_ZALIGN64();

  __Pragma("concurrent")
  for (n=0; n<(N>>1); n++)
  {
    /*Load two input numbers */
    AE_LA32X2_IP( vxb, x_align, px);
    is_zero = AE_EQ32(vxb, zero);
    /* Normalize x to x0*/
    nsab = AE_NSAZ32_L(vxb);
    nsaa = AE_NSAZ32_L(AE_SEL32_HH(vxb,vxb));

    t0 = AE_MOVDA32X2( nsaa, nsab );
    t1 = AE_AND32( t0, c1 );

    AE_MOVSARD7(t1);
    vxw = AE_SLAS32S( vxb);
    t0 = AE_SRAI32( t0, 1 );
    inf_ab = AE_LT32(vxw, zero);

    /*get table indexes*/
    vxa = AE_SRAI32(vxw, 21);
    vxa = AE_AND32( vxa, c4 );
    idxa= AE_MOVAD32_H(vxa); //
    idxb= AE_MOVAD32_L(vxa); //
    
    vxa = AE_L32X2_X(tbl, idxa); 
    vxb = AE_L32X2_X(tbl, idxb);

    vcw = AE_SEL32_LL(vxa, vxb);//f'(x0)
    vxc = AE_SEL32_HH(vxa, vxb);// f(x0)
    
    /*Calculate dx for first iteration*/  
    vzf = (vxc);//
    vxf = (vxw);
    AE_MULSFP32X2RAS(vxf, vxc, vxc);// 
    /*Calculate y in first iteration*/
    AE_MULAFP32X2RAS(vzf, vcw, vxf);// vzf = y,Q31
    vxf = (vxw);//
    /*Calculate dx for second iteration*/ 
    AE_MULSFP32X2RAS(vxf, vzf, vzf);//
    /*Calculate y in second iteration*/
    AE_MULAFP32X2RAS(vzf, vcw, vxf);// y, Q31
    {
        ae_int32x2 vxd;
        nsab=AE_MOVAD32_L(t0);
        nsaa=AE_MOVAD32_H(t0);
        vxd = AE_SRAA32(vzf, nsab);
        vxc = AE_SRAA32(vzf, nsaa);
        vxc = AE_SEL32_HL(vxc, vxd);
    }

    AE_MOVT32X2(vxc, min_int32, inf_ab);
    AE_MOVT32X2(vxc, zero, is_zero);
    AE_SA32X2_IP(vxc, y_align, py);
  }
  AE_SA64POS_FP(y_align, py);
  if (N&1)
  {
    /*Load two input numbers */
    vxb=AE_L32_I( (const ae_int32*)px, 0);
    is_zero = AE_EQ32(vxb, zero);
    /* Normalize x to x0*/
    nsab = AE_NSAZ32_L(vxb);
    nsaa = AE_NSAZ32_L(AE_SEL32_HH(vxb,vxb));

    t0 = AE_MOVDA32X2( nsaa, nsab );
    t1 = AE_AND32( t0, c1 );

    AE_MOVSARD7(t1);
    vxw = AE_SLAS32S( vxb);
    t0 = AE_SRAI32( t0, 1 );
    inf_ab = AE_LT32(vxw, zero);

    /*get table indexes*/
    vxa = AE_SRAI32(vxw, 21);
    vxa = AE_AND32( vxa, c4 );
    idxa= AE_MOVAD32_H(vxa); //
    idxb= AE_MOVAD32_L(vxa); //
    
    vxa = AE_L32X2_X(tbl, idxa); 
    vxb = AE_L32X2_X(tbl, idxb);

    vcw = AE_SEL32_LL(vxa, vxb);//f'(x0)
    vxc = AE_SEL32_HH(vxa, vxb);// f(x0)
    
    /*Calculate dx for first iteration*/  
    vzf = (vxc);//
    vxf = (vxw);
    AE_MULSFP32X2RAS(vxf, vxc, vxc);// 
    /*Calculate y in first iteration*/
    AE_MULAFP32X2RAS(vzf, vcw, vxf);// vzf = y,Q31
    vxf = (vxw);//
    /*Calculate dx for second iteration*/ 
    AE_MULSFP32X2RAS(vxf, vzf, vzf);//
    /*Calculate y in second iteration*/
    AE_MULAFP32X2RAS(vzf, vcw, vxf);// y, Q31
    AE_MOVSARD7(t0);
    vxc=AE_SRAS32(vzf);
    {
    ae_int32x2 vxd;
    nsab=AE_MOVAD32_L(t0);
    nsaa=AE_MOVAD32_H(t0);
    vxd = AE_SRAA32(vzf, nsab);
    vxc = AE_SRAA32(vzf, nsaa);
    vxc = AE_SEL32_HL(vxc, vxd);
    }

    AE_MOVT32X2(vxc, min_int32, inf_ab);
    AE_MOVT32X2(vxc, zero, is_zero);
    AE_S32_L_I(vxc, (ae_int32*)py, 0);
  }
}
