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

#ifndef __NATUREDSP_SIGNAL_DIAG_H
#define __NATUREDSP_SIGNAL_DIAG_H

/* Portable data types */
#include "NatureDSP_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
  Discover hardware capabilities
  NatureDSP_Signal_get_isa_opt  Query of a particular ISA option/parameter
===========================================================================*/

/* ISA options */
#define NATUREDSP_ISA_OPT_INT16_SIMD_WIDTH     1 /* SIMD width w.r.t. 16-bit fixed-point ops. */
#define NATUREDSP_ISA_OPT_HAVE_FP              2 /* single prec floating point */
#define NATUREDSP_ISA_OPT_HAVE_DFP             3 /* double precision FP pkg */

/*-------------------------------------------------------------------------
Query of a particular ISA option or a parameter.
Input:
    isa_opt   ISA option or parameter identifier, one of NATUREDSP_ISA_OPT_
              symbols
Return value:
    Integral status value of the requested item, or -1 for an unknown value
    of the argument.
-------------------------------------------------------------------------*/
int NatureDSP_Signal_get_isa_opt( int isa_opt );

/*===========================================================================
  Floating-point exceptions control
  NatureDSP_Signal_feclearexcept  Clear floating-point exceptions
  NatureDSP_Signal_fetestexcept   Check if floating-point exceptions are set
  NatureDSP_Signal_feraiseexcept  Attempt to raise floating-point exceptions
===========================================================================*/

/*-------------------------------------------------------------------------
Attempt to clear the floating-point exceptions represented by the argument.
Input:
    excepts   Bitwise OR sum of floating-point exception macros defined in
              standard header fenv.h: FE_DIVBYZERO,  FE_INEXACT, FE_INVALID,
              FE_OVERFLOW, FE_UNDERFLOW, FE_ALL_EXCEPT.
Return value:
    Zero if the excepts argument is zero or if all the specified exceptions
    were successfully cleared. Otherwise, returns a nonzero value.
-------------------------------------------------------------------------*/
int NatureDSP_Signal_feclearexcept( int excepts );

/*-------------------------------------------------------------------------
Attempt to raise the supported floating-point exceptions represented by 
the argument.
Input:
    excepts   Bitwise OR sum of floating-point exception macros defined in
              standard header fenv.h: FE_DIVBYZERO,  FE_INEXACT, FE_INVALID,
              FE_OVERFLOW, FE_UNDERFLOW, FE_ALL_EXCEPT.
Return value:
    Zero if the excepts argument is zero or if all the specified exceptions
    were successfully raised. Otherwise, returns a nonzero value.
-------------------------------------------------------------------------*/
int NatureDSP_Signal_feraiseexcept( int excepts );

/*-------------------------------------------------------------------------
Determine which of a specified subset of the floating-point exception flags
are currently set.
Input:
    excepts   Bitwise OR sum of floating-point exception macros defined in
              standard header fenv.h: FE_DIVBYZERO,  FE_INEXACT, FE_INVALID,
              FE_OVERFLOW, FE_UNDERFLOW, FE_ALL_EXCEPT.
Return value:
    Bitwise OR sum of NATUREDSP_FE_ symbols correspoinding to the currently
    set floating-point exceptions included in excepts argument value.
-------------------------------------------------------------------------*/
int NatureDSP_Signal_fetestexcept( int excepts );

#ifdef __cplusplus
};
#endif

#endif /* __NATUREDSP_SIGNAL_DIAG_H */
