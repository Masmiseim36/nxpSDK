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
  NatureDSP Signal Processing Library. Vector matematics
    Square Root
    Optimized code for HiFi4
  IntegrIT, 2006-2018
*/

#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_math.h"
#include "common.h"
#include "scl_sqrt_table.h"

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
int32_t scl_sqrt64x32(int64_t x)
{
    int         nsa, idx, shft_a;
    int32_t     y;
    ae_int64    vxw;
    ae_int32x2  vbw, vcw, vdw, viw, vxx, vzw;
    ae_f32x2    vxf, vzf, vcf;
    xtbool2     inf, zero;

    viw = AE_MOVDA32X2(MIN_INT32, MIN_INT32);
    vzw = AE_MOVDA32X2(0, 0);

    {
        ae_valign   movx;
        const ae_int32x2 * px = (const ae_int32x2 *)&x;
        movx = AE_LA64_PP(px);
        AE_LA32X2_IP(vxx, movx, px);
        vxx = AE_SEL32_LH(vxx, vxx);
        vxw = AE_MOVINT64_FROMINT32X2(vxx);
    }
    //vxw = AE_MOV(x);

    nsa = AE_NSA64(vxw);
    shft_a = (nsa & ~1);
    nsa = nsa >> 1;
    vxw = AE_SLAA64(vxw, shft_a);
    vxx = AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(vxw), AE_MOVINT32X2_FROMINT64(vxw));
    zero = AE_EQ32(vxx, 0);
    inf = AE_LT32(vxx, 0);

    /* get table indexes */
    vdw = AE_SRAI32(vxx, 24);
    vdw = AE_SLAI32(vdw, 3);
    idx = AE_MOVAD32_H(vdw);
    vcw = AE_L32X2_X((const ae_int32x2 *)sqrt_table, idx);
    vbw = AE_SEL32_LL(vcw, vcw); //f'(x0)

    vxf = (vxx);
    vzf = (vcw);
    AE_MULSFP32X2RAS(vxf, vzf, vzf); //dx
    vcf = (vbw);
    AE_MULAFP32X2RAS(vzf, vcf, vxf); //y 

    /*second iteration*/
    vxf = (vxx);
    AE_MULSFP32X2RAS(vxf, vzf, vzf);
    vcf = (vbw);
    AE_MULAFP32X2RAS(vzf, vcf, vxf); //y

    /*third iteration*/
    vxf = (vxx);
    AE_MULSFP32X2RAS(vxf, vzf, vzf);
    vcf = (vbw);
    AE_MULAFP32X2RAS(vzf, vcf, vxf); //y

    vdw = (vzf);
    vdw = AE_SRAA32(vdw, nsa);
    AE_MOVT32X2(vdw, viw, inf);
    AE_MOVT32X2(vdw, vzw, zero);
    y = AE_MOVAD32_H(vdw);
    return y;
} /* scl_sqrt64x32() */
