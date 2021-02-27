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

/*
 * Inline variants of floating-point exceptions and environment control
 * functions.
 */

#ifndef ____FENV_H
#define ____FENV_H

/* Cross-platform data type and platform definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"

#include <fenv.h>

#if XCHAL_HAVE_FUSION_FP

#define XTENSA_FE_INEXACT     0x01
#define XTENSA_FE_UNDERFLOW   0x02
#define XTENSA_FE_OVERFLOW    0x04
#define XTENSA_FE_INVALID     0x10
#define XTENSA_FE_DIVBYZERO   0x08

#define XTENSA_FE_ALL_EXCEPT \
  ( XTENSA_FE_DIVBYZERO |		 \
    XTENSA_FE_INEXACT   |		 \
    XTENSA_FE_INVALID   |		 \
    XTENSA_FE_OVERFLOW  |		 \
    XTENSA_FE_UNDERFLOW )

#define XTENSA_FE_EXCEPTION_FLAGS_OFFSET 7
#define XTENSA_FE_EXCEPTION_FLAG_MASK (XTENSA_FE_ALL_EXCEPT << XTENSA_FE_EXCEPTION_FLAGS_OFFSET)
#define XTENSA_FE_EXCEPTION_ENABLE_OFFSET 2
#define XTENSA_FE_EXCEPTION_ENABLE_MASK (XTENSA_FE_ALL_EXCEPT << XTENSA_FE_EXCEPTION_ENABLE_OFFSET)
#define XTENSA_FE_ROUND_MODE_OFFSET 0
#define XTENSA_FE_ROUND_MODE_MASK (0x3 << XTENSA_FE_ROUND_MODE_OFFSET)
#define XTENSA_FE_FLOATING_ENV_MASK (XTENSA_FE_EXCEPTION_FLAG_MASK | XTENSA_FE_EXCEPTION_ENABLE_MASK | XTENSA_FE_ROUND_MODE_MASK)

#if defined (COMPILER_XTENSA) 

typedef struct __tag_fenv_t
{
  unsigned fcr;
  unsigned fsr;

} __fenv_t;

inline_ int __feclearexcept( int excepts )
{
  unsigned fsr = XT_RUR_FSR();
  XT_WUR_FSR( fsr & ~( excepts << XTENSA_FE_EXCEPTION_FLAGS_OFFSET ) );
  return (0);
}

inline_ int __fetestexcept( int excepts )
{
  unsigned fsr = XT_RUR_FSR();
  return ( ( fsr >> XTENSA_FE_EXCEPTION_FLAGS_OFFSET ) & excepts );
}

inline_ int __feraiseexcept( int excepts )
{
#if 1
  if ( 0 != ( excepts & ( FE_UNDERFLOW | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_DIVBYZERO ) ) )
  {
    /* Most of the time there are no exceptions to raise. */
    __Pragma( "frequency_hint never" );
    /* Extract a flag for each of supported exceptions. */
    xtbool b_inv  = ( 0 != ( excepts & FE_INVALID   ) );
    xtbool b_divz = ( 0 != ( excepts & FE_DIVBYZERO ) );
    xtbool b_unfl = ( 0 != ( excepts & FE_UNDERFLOW ) );
    xtbool b_ovfl = ( 0 != ( excepts & FE_OVERFLOW  ) );
    xtbool b_inex = ( 0 != ( excepts & FE_INEXACT   ) );
    /* Auxiliary vars. */
    xtfloat inv_neg    = XT_CONST_S(1); XT_MOVT_S ( inv_neg  , XT_NEG_S( XT_CONST_S(1) )               , b_inv  );
    xtfloat divz_zero  = XT_CONST_S(1); XT_MOVT_S ( divz_zero, XT_CONST_S(0)                           , b_divz );
    xtfloat unfl_tiny  = XT_CONST_S(1); XT_MOVT_S ( unfl_tiny, XT_AE_MOVXTFLOAT_FROMINT32( AE_MOVI(1) ), b_unfl );
    xtfloat ovfl_bigf  = XT_CONST_S(1); XT_MOVT_S ( ovfl_bigf, XT_AE_MOVXTFLOAT_FROMINT32( 0x7f7f7f7f ), b_ovfl ); /* 0x7f7f7f7f, 3.4e38f */
    ae_int32 inex_bigi = AE_MOVI_32(1); AE_MOVT_32( inex_bigi, 0x7f7f7f7f                              , b_inex ); /* 0x7f7f7f7f */

    /* For each exception type, conditionally issue an errorneous operation. Doing so, we
     * achieve two goals:
     *   1) Exception flag in FSR is set regardless of whether or not this exception is 
     *      enabled in FCR
     *   2) Exception is raised (in terms of program flow) iff the corresponding enable bit
     *      in FCR is 1. */
    __Pragma( "no_reorder" );
    __asm__ volatile ( "rsqrt0.s %0, %0"   : "+v" (inv_neg  ) ); /* sqrt(-1.f) -> "invalid"                */
    __asm__ volatile ( "recip0.s %0, %0"   : "+v" (divz_zero) ); /* 1.f/0.f -> "division-by-zero"          */
    __asm__ volatile ( "mul.s %0, %0, %0"  : "+v" (unfl_tiny) ); /* tiny*tine -> "underflow" and "inexact" */
    __asm__ volatile ( "add.s %0, %0, %0"  : "+v" (ovfl_bigf) ); /* bigf+bigf -> "overflow" and "inexact"  */
    __asm__ volatile ( "float.s %0, %0, 0" : "+v" (inex_bigi) ); /* (float)0x7f7f7f7f -> "inexact"         */
    __Pragma( "no_reorder" );
  }
#else
  unsigned long fsr;
  if ( excepts & ~FE_ALL_EXCEPT ) return (-1);
  fsr = XT_RUR_FSR();
  /* Clear all the flags */
  XT_WUR_FSR( fsr & ~XTENSA_FE_EXCEPTION_FLAG_MASK );
  /* OR the newly raised flags with preserved flags. */
  XT_WUR_FSR( fsr | ( excepts << XTENSA_FE_EXCEPTION_FLAGS_OFFSET ) );
#endif
  return (0);
}

inline_ int __feholdexcept( __fenv_t * __envp )
{
  __envp->fcr = XT_RUR_FCR();
  __envp->fsr = XT_RUR_FSR();
  /* Clear the exception enable flags. */
  XT_WUR_FCR( __envp->fcr & ~XTENSA_FE_EXCEPTION_ENABLE_MASK );
  /* Clear the exception status flags. */
  XT_WUR_FSR( 0 );
  __Pragma( "no_reorder" );
  return (0);
}

inline_ int __fesetenv( __fenv_t * __envp )
{
  /* Temporarily zero the exception enable flags to avoid any spurious exceptions. */
  XT_WUR_FCR( 0 );
  /* Set the exception status flags. */
  XT_WUR_FSR( __envp->fsr & XTENSA_FE_EXCEPTION_FLAG_MASK );
  /* Set the exception enable flags and the rounding mode. */
  XT_WUR_FCR( __envp->fcr & ( XTENSA_FE_EXCEPTION_ENABLE_MASK | XTENSA_FE_ROUND_MODE_MASK ) );
  return (0);
}

#else

#define XTENSA_TO_FENV_MAP(except) \
  ( ( ( 0 != ( (except) & XTENSA_FE_INEXACT   ) ) ? FE_INEXACT   : 0 ) | \
    ( ( 0 != ( (except) & XTENSA_FE_UNDERFLOW ) ) ? FE_UNDERFLOW : 0 ) | \
    ( ( 0 != ( (except) & XTENSA_FE_OVERFLOW  ) ) ? FE_OVERFLOW  : 0 ) | \
    ( ( 0 != ( (except) & XTENSA_FE_INVALID   ) ) ? FE_INVALID   : 0 ) | \
    ( ( 0 != ( (except) & XTENSA_FE_DIVBYZERO ) ) ? FE_DIVBYZERO : 0 ) )

#define FENV_TO_XTENSA_MAP(except) \
  ( ( ( 0 != ( (except) & FE_INEXACT   ) ) ? XTENSA_FE_INEXACT   : 0 ) | \
    ( ( 0 != ( (except) & FE_UNDERFLOW ) ) ? XTENSA_FE_UNDERFLOW : 0 ) | \
    ( ( 0 != ( (except) & FE_OVERFLOW  ) ) ? XTENSA_FE_OVERFLOW  : 0 ) | \
    ( ( 0 != ( (except) & FE_INVALID   ) ) ? XTENSA_FE_INVALID   : 0 ) | \
    ( ( 0 != ( (except) & FE_DIVBYZERO ) ) ? XTENSA_FE_DIVBYZERO : 0 ) )

typedef struct __tag_fenv_t
{
  fenv_t   env;
  unsigned fcr;
  unsigned fsr;

} __fenv_t;

inline_ int __feclearexcept( int excepts )
{
  unsigned fsr = XT_RUR_FSR();
  unsigned mask = ( FENV_TO_XTENSA_MAP(excepts) << XTENSA_FE_EXCEPTION_FLAGS_OFFSET );
  XT_WUR_FSR( fsr & ~mask );
  return ( feclearexcept( excepts ) );
}

inline_ int __fetestexcept( int excepts )
{
  unsigned fsr = XT_RUR_FSR();
  unsigned flags = XTENSA_TO_FENV_MAP( fsr >> XTENSA_FE_EXCEPTION_FLAGS_OFFSET );
  return ( fetestexcept( excepts ) | ( (int)flags & excepts ) );
}

//#define __feraiseexcept feraiseexcept
inline_ int __feraiseexcept( int excepts )
{
  if ( excepts & (FE_OVERFLOW|FE_UNDERFLOW) ) excepts |= FE_INEXACT;
  return ( feraiseexcept(excepts) );
}

inline_ int __feholdexcept( __fenv_t * __envp )
{
  if ( 0 != feholdexcept( &__envp->env ) ) return (-1);
  __envp->fcr = XT_RUR_FCR();
  __envp->fsr = XT_RUR_FSR();
  /* Clear the exception enable flags. */
  XT_WUR_FCR( __envp->fcr & ~XTENSA_FE_EXCEPTION_ENABLE_MASK );
  /* Clear the exception status flags. */
  XT_WUR_FSR( 0 );
  return (0);
}

inline_ int __fesetenv( __fenv_t * __envp )
{
  if ( 0 != fesetenv( &__envp->env ) ) return (-1);
  /* Zero the exception enable flags to avoid spurious exception. */
  XT_WUR_FCR( 0 );
  /* Set the exception status flags, but do not raise an exception. */
  XT_WUR_FSR( __envp->fsr & XTENSA_FE_EXCEPTION_FLAG_MASK );
  /* Set the exception enable flags and the rounding mode. */
  XT_WUR_FCR( __envp->fcr & ( XTENSA_FE_EXCEPTION_ENABLE_MASK | XTENSA_FE_ROUND_MODE_MASK ) );
  return (0);
}

#endif

#endif // XCHAL_HAVE_FUSION_FP
#endif /* ____FENV_H */
