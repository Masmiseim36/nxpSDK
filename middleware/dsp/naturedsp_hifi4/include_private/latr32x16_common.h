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
#ifndef __LATR32x16_COMMON_H__
#define __LATR32x16_COMMON_H__

#include "NatureDSP_types.h"
#include "common.h"

/* AE_SEL32_LL adapted for ae_f32x2 data type.
   useSEL16IN - if nonzero AE_SEL16IN instruction will be used, otherwise generic select 
*/
#define select_f32x2_ll( s_out, s_in0, s_in1, useSEL16IN ) \
  {                                                     \
    if (useSEL16IN)                                     \
    {                                                   \
    ae_int16x4  t0 = AE_MOVINT16X4_FROMF32X2( s_in0 );  \
    ae_int16x4  t1 = AE_MOVINT16X4_FROMF32X2( s_in1 );  \
    ae_int16x4  t2;                                     \
    t2 = AE_SEL16I_N(t0,t1,3 );                         \
    s_out = AE_MOVF32X2_FROMINT16X4( t2 );              \
    }                                                   \
    else                                                \
    {                                                   \
    ae_int32x2 t0 = ( s_in0 );                          \
    ae_int32x2 t1 = ( s_in1 );                          \
    ae_int32x2 t2;                                      \
    t2 = AE_SEL32_LL( t0, t1 );                         \
    s_out = ( t2 );                                     \
    }                                                   \
  }                                                    

/* AE_SEL32_LH adapted for ae_f32x2 data type. */
#define select_f32x2_lh( s_out, s_in0, s_in1, useSEL16IN ) \
  { \
    if (useSEL16IN)                                     \
    {                                                   \
    ae_int16x4  t0 = AE_MOVINT16X4_FROMF32X2( s_in0 );  \
    ae_int16x4  t1 = AE_MOVINT16X4_FROMF32X2( s_in1 );  \
    ae_int16x4  t2;                                     \
    t2 = AE_SEL16I_N(t0,t1,0 );                         \
    s_out = AE_MOVF32X2_FROMINT16X4( t2 );              \
    }                                                   \
    else                                                \
    {                                                   \
                                                        \
    ae_int32x2 t0 = ( s_in0 );                          \
    ae_int32x2 t1 = ( s_in1 );                          \
    ae_int32x2 t2;                                      \
    t2 = AE_SEL32_LH( t0, t1 );                         \
    s_out = ( t2 );                                     \
    }                                                   \
  }

/* AE_SEL32_HH adapted for ae_f32x2 data type. */
#define select_f32x2_hh( s_out, s_in0, s_in1 ) \
  { \
    ae_int32x2 t0 = ( s_in0 ); \
    ae_int32x2 t1 = ( s_in1 ); \
    ae_int32x2 t2; \
    \
    t2 = AE_SEL32_HH( t0, t1 ); \
    \
    s_out = ( t2 ); \
  }

/* Lattice filter data processing function. */
typedef void (proc_fxn_t)( int32_t * restrict r,     // r[N]     [out   ] Q31
                     const int32_t * restrict x,     // x[N]     [in    ] Q31
                           int32_t *          dline, // dline[M] [in/out] Q30
                     const int16_t *          coef,  // coef[M]  [in    ] Q15
                     int16_t scale, int N, int M );  // scale    [in    ] Q15

/* Custom data processing functions for particular lattice orders. */
proc_fxn_t latr32x16_1_proc;
proc_fxn_t latr32x16_2_proc;
proc_fxn_t latr32x16_3_proc;
proc_fxn_t latr32x16_4_proc;
proc_fxn_t latr32x16_5_proc;
proc_fxn_t latr32x16_6_proc;
proc_fxn_t latr32x16_7_proc;
proc_fxn_t latr32x16_8_proc;
proc_fxn_t latr32x16_X_proc;

#endif
