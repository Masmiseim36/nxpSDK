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
#ifndef __COMMON_FPU_H__
#define __COMMON_FPU_H__

#include "common.h"

/* all vector single precision floating point instructions */
#if ( (XCHAL_HAVE_HIFI4_VFPU) )
#define HAVE_VFPU 1
#else
#define HAVE_VFPU 0
#endif

/* all scalar single precision floating point instructions */
#if ( (XCHAL_HAVE_HIFI4_VFPU) || (XCHAL_HAVE_FP))
#define HAVE_FPU 1
#else
#define HAVE_FPU 0
#endif

#if (!(HAVE_VFPU)&&(HAVE_FPU))
#include <xtensa/tie/xt_FP.h>
#endif

//-------------------------------------------------------------------------
// emulation of some ISA capabilities on SFPU
//-------------------------------------------------------------------------
#if HAVE_FPU


#ifndef XT_MAX_S
#define XT_MAX_S(a, b) __XT_MAX_S(&a,&b)
inline_ xtfloat __XT_MAX_S(const void* pa, const void* pb)
{
  xtbool   tmp;
  xtfloat  a, b, c;
  a = *(const xtfloat *)pa;
  b = *(const xtfloat *)pb;
  tmp = XT_OLT_S(b, a);
  c = b;
  XT_MOVT_S(c, a, tmp);
  return c;
}
#endif
#ifndef XT_MIN_S
#define XT_MIN_S(a, b) __XT_MIN_S(&a,&b)
inline_ xtfloat __XT_MIN_S(const void* pa, const void* pb)
{
  xtbool   tmp;
  xtfloat  a, b, c;
  a = *(const xtfloat *)pa;
  b = *(const xtfloat *)pb;
  tmp = XT_OLT_S(a, b);
  c = b;
  XT_MOVT_S(c, a, tmp);
  return c;
}
#endif


#ifndef XT_LSXC
#define XT_LSXC(a,b,c)                     \
{                                          \
    a=XT_LSI(b,0);                         \
    AE_ADDCIRC_XC(castxcc(ae_int64,b),c);  \
}
#endif
#ifndef XT_SSXC
#define XT_SSXC(a,b,c)                     \
{                                          \
    XT_SSI(a,b,0);                         \
    AE_ADDCIRC_XC(castxcc(ae_int64,b),c);  \
}
#endif


#endif //HAVE_FPU
#endif
