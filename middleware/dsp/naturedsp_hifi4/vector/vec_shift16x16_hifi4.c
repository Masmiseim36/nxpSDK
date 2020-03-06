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

/* Library API */
#include "NatureDSP_Signal_vector.h"
#include "common.h"


/*===========================================================================
  Vector matematics:
  vec_shift,vec_scale  Vector Scaling with Saturation
===========================================================================*/

/*-------------------------------------------------------------------------
  Vector Scaling with Saturation
  These routines make shift with saturation of data values in the vector 
  by given scale factor (degree of 2).
  Functions vec_scale() make multiplication of a vector to a coefficient 
  which is not a power of 2 forming Q31, Q15 or floating-point result.
  Two versions of routines are available: regular versions (vec_shift32x32, 
  vec_shift16x16, vec_shiftf, vec_scale32x32, vec_scale16x16, vec_scalef, 
  vec_scale_sf) work with arbitrary arguments, faster versions 
  (vec_shift32x32_fast, vec_shift16x16_fast, vec_scale32x32_fast, 
  vec_scale16x16_fast) apply some restrictions

  For floating point:
  Fuction vec_shiftf() makes scaling without saturation of data values by given 
  scale factor (degree of 2). 
  Functions vec_scalef() and vec_scale_sf() make multiplication of input vector
  to coefficient which is not a power of 2.
  Two versions of routines are available: 
    without saturation - vec_scalef;
    with saturation - vec_scale_sf; 

  Precision:
  32x32 32-bit input, 32-bit output
  16x16 16-bit input, 16-bit output
  f     single precision floating point

  Input:
  x[N]    input data, Q31, Q15 or floating point
  t       shift count. If positive, it shifts left with saturation, if
          negative it shifts right
  s       scale factor, Q31, Q15 or floating point
  N       length of vector
  fmin    minimum output value (only for vec_scale_sf)
  fmax    maximum output value (only for vec_scale_sf)
  Output:
  y[N]    output data, Q31, Q15 or floating point

  Restrictions:
  For regular versions (vec_shift32x32, vec_shift16x16, vec_shiftf, 
  vec_scale32x32, vec_scale16x16, vec_scalef, vec_scale_sf):
  x,y should not overlap
  t   should be in range -31...31 for fixed-point functions and -129...146 
      for floating point
  For vec_scale_sf:
  fmin<=fmax;

  For faster versions (vec_shift32x32_fast, vec_shift16x16_fast, 
  vec_scale32x32_fast,vec_scale16x16_fast):
  x,y should not overlap
  t should be in range -31...31 
  x,y - aligned on 8-byte boundary
  N   - multiple of 4 
-------------------------------------------------------------------------*/
void vec_shift16x16
(
        int16_t * restrict  y,
  const int16_t * restrict  x,
  int                       t,
  int                       N 
)
{

/* macros for emulation of unaligned loads via selects */
#define FIRST_LOAD(x,xnext,px,offset)          \
{                                              \
    if(offset==0) AE_L16X4_IP(x, px, 8);       \
    if(offset==2)                              \
    {                                          \
        ae_valign x_align;                     \
        x_align = AE_LA64_PP(px);              \
        AE_LA16X4_IP(x, x_align, px);          \
        x=AE_SEL16_4321(x,x);                  \
        px=(ae_int16x4*)(((int16_t*)px)-1);    \
        AE_L16X4_IP(xnext, px, 8);             \
        x=AE_SEL16_6543(x,xnext);              \
    }                                          \
    if(offset==4)                              \
    {                                          \
        ae_valign x_align;                     \
        x_align = AE_LA64_PP(px);              \
        AE_LA16X4_IP(x, x_align, px);          \
        x=AE_SEL16_5432(x,x);                  \
        px=(ae_int16x4*)(((int16_t*)px)-2);    \
        AE_L16X4_IP(xnext, px, 8);             \
        x=AE_SEL16_5432(x,xnext);              \
    }                                          \
    if(offset==6)                              \
    {                                          \
        ae_valign x_align;                     \
        x_align = AE_LA64_PP(px);              \
        AE_LA16X4_IP(x, x_align, px);          \
        x=AE_SEL16_6543(x,x);                  \
        px=(ae_int16x4*)(((int16_t*)px)-3);    \
        AE_L16X4_IP(xnext, px, 8);             \
        x=AE_SEL16_4321(x,xnext);              \
    }                                          \
}

#define NEXT_LOAD(x,xnext,px,offset)          \
{                                             \
    if(offset==0)                             \
    {                                         \
        AE_L16X4_IP(x, px, 8);                \
    }                                         \
    if(offset==2)                             \
    {                                         \
        x=xnext;                              \
        AE_L16X4_IP(xnext, px, 8);            \
        x=AE_SEL16_6543(x,xnext);             \
    }                                         \
    if(offset==4)                             \
    {                                         \
        x=xnext;                              \
        AE_L16X4_IP(xnext, px, 8);            \
        x=AE_SEL16_5432(x,xnext);             \
    }                                         \
    if(offset==6)                             \
    {                                         \
        x=xnext;                              \
        AE_L16X4_IP(xnext, px, 8);            \
        x=AE_SEL16_4321(x,xnext);             \
    }                                         \
}


  int n;

  ae_int16x4  vxh, vyh, vzh,x1;
  ae_f16x4    vzf;
  ae_int32x2  vaw, vbw;

  const ae_int16x4 * restrict px = (const ae_int16x4 *)x;
        ae_int16x4 * restrict py = (      ae_int16x4 *)y;

  ae_valign y_align;
  if(N<=0) return;
  NASSERT(t>=-31 && t<=31);
  y_align = AE_ZALIGN64();

  if (t<0) 
  {
        //----------------------------------
        // NEGATIVE SHIFTS
        //----------------------------------
        t=0x8000>>(-t);
        t=XT_MAX(t,1);
        vzh=AE_MOVDA16(t);vzf = (vzh);
        if (N >= 4)
        {

          switch (((uintptr_t)px)&7)
          {
          case 0:
              FIRST_LOAD(vxh,x1,px,0);
              for (n = 0; n<((N >> 2) - 1); n++)
              {
                  vyh = AE_MULFP16X4S(vxh, vzf);
                  AE_SA16X4_IP(vyh, y_align, py);
                  NEXT_LOAD(vxh,x1,px,0);
              }
              vyh = AE_MULFP16X4S(vxh, vzf);
              AE_SA16X4_IP(vyh, y_align, py);
              px = (const ae_int16x4 *)((const ae_int16 *)px -0);
              break;
          case 2:
              FIRST_LOAD(vxh,x1,px,2);
              for (n=0; n<((N>>2)-1); n++)
              {
                  vyh = AE_MULFP16X4S(vxh, vzf);
                  AE_SA16X4_IP(vyh, y_align, py);
                  NEXT_LOAD(vxh,x1,px,2);
              }
              vyh = AE_MULFP16X4S(vxh, vzf);
              AE_SA16X4_IP(vyh, y_align, py);
              px = (const ae_int16x4 *)((const ae_int16 *)px - 3);
              break;
          case 4: 
              FIRST_LOAD(vxh,x1,px,4);
              for (n=0; n<((N>>2)-1); n++)
              {
                  vyh = AE_MULFP16X4S(vxh, vzf);
                  AE_SA16X4_IP(vyh, y_align, py);
                  NEXT_LOAD(vxh,x1,px,4);
              }
              vyh = AE_MULFP16X4S(vxh, vzf);
              AE_SA16X4_IP(vyh, y_align, py);
              px = (const ae_int16x4 *)((const ae_int16 *)px - 2);
              break;
          case 6:
              FIRST_LOAD(vxh,x1,px,6);
              for (n=0; n<((N>>2)-1); n++)
              {
                  vyh = AE_MULFP16X4S(vxh, vzf);
                  AE_SA16X4_IP(vyh, y_align, py);
                  NEXT_LOAD(vxh,x1,px,6);
              }
              vyh = AE_MULFP16X4S(vxh, vzf);
              AE_SA16X4_IP(vyh, y_align, py);
              px = (const ae_int16x4 *)((const ae_int16 *)px - 1);
              break;
          }
          AE_SA64POS_FP(y_align, py);
        }
        switch (N & 3)
        {
        case 1:
          AE_L16_IP(vxh, castxcc(ae_int16, px), 0);
          vyh = AE_MULFP16X4S(vxh, vzf);
          AE_S16_0_I(vyh, (ae_int16 *)py, 0);
          break;
        case 2:
          AE_L16_IP(vxh, castxcc(ae_int16, px), sizeof(ae_int16));
          vyh = AE_MULFP16X4S(vxh, vzf);
          AE_S16_0_I(vyh, (ae_int16 *)py, 0);
          AE_L16_IP(vxh, castxcc(ae_int16, px), 0);
          vyh = AE_MULFP16X4S(vxh, vzf);
          AE_S16_0_I(vyh, (ae_int16 *)py, sizeof(ae_int16));
          break;
        case 3:
          AE_L16_IP(vxh, castxcc(ae_int16, px), sizeof(ae_int16));
          vyh = AE_MULFP16X4S(vxh, vzf);
          AE_S16_0_I(vyh, (ae_int16 *)py, 0);
          AE_L16_IP(vxh, castxcc(ae_int16, px), sizeof(ae_int16));
          vyh = AE_MULFP16X4S(vxh, vzf);
          AE_S16_0_I(vyh, (ae_int16 *)py, sizeof(ae_int16));
          AE_L16_IP(vxh, castxcc(ae_int16, px), 0);
          vyh = AE_MULFP16X4S(vxh, vzf);
          AE_S16_0_I(vyh, (ae_int16 *)py, 2*sizeof(ae_int16));
          break;
        }
    }
    else 
    {
        //----------------------------------
        // POSITIVE SHIFTS
        //----------------------------------
        ae_valign x_align;
        vzh = AE_MOVDA16(1);
        vzh = AE_SLAA16S(vzh, t);
        x_align = AE_LA64_PP(px);
        
        for (n=0; n<(N>>2); n++)
        {
            AE_LA16X4_IP(vxh, x_align, px); 
            AE_MUL16X4(vaw, vbw, vxh, vzh);
            vyh = AE_SAT16X4(vaw, vbw);
            AE_SA16X4_IP(vyh, y_align, py);
        }
          AE_SA64POS_FP(y_align, py);
        
        switch (N & 3) 
        {
          case 1:
            AE_L16_IP(vxh, castxcc(ae_int16, px), 0);
            AE_MUL16X4(vaw, vbw, vxh, vzh);
            vyh = AE_SAT16X4(vaw, vbw);
            AE_S16_0_I(vyh, (ae_int16 *)py, 0);
          break;
          case 2:
            AE_L16_IP(vxh, castxcc(ae_int16, px), sizeof(ae_int16));
            AE_MUL16X4(vaw, vbw, vxh, vzh);
            vyh = AE_SAT16X4(vaw, vbw);
            AE_S16_0_I(vyh, (ae_int16 *)py, 0);
            AE_L16_IP(vxh, castxcc(ae_int16, px), 0);
            AE_MUL16X4(vaw, vbw, vxh, vzh);
            vyh = AE_SAT16X4(vaw, vbw);
            AE_S16_0_I(vyh, (ae_int16 *)py, sizeof(ae_int16));
            break;
          case 3:
            AE_L16_IP(vxh, castxcc(ae_int16, px), sizeof(ae_int16));
            AE_MUL16X4(vaw, vbw, vxh, vzh);
            vyh = AE_SAT16X4(vaw, vbw);
            AE_S16_0_I(vyh, (ae_int16 *)py, 0);
            AE_L16_IP(vxh, castxcc(ae_int16, px), sizeof(ae_int16));
            AE_MUL16X4(vaw, vbw, vxh, vzh);
            vyh = AE_SAT16X4(vaw, vbw);
            AE_S16_0_I(vyh, (ae_int16 *)py, sizeof(ae_int16));
            AE_L16_IP(vxh, castxcc(ae_int16, px), sizeof(ae_int16));
            AE_MUL16X4(vaw, vbw, vxh, vzh);
            vyh = AE_SAT16X4(vaw, vbw);
            AE_S16_0_I(vyh, (ae_int16 *)py, 2*sizeof(ae_int16));
            break;
          default:AE_SA64POS_FP(y_align, py); break;
        }
  }

}
