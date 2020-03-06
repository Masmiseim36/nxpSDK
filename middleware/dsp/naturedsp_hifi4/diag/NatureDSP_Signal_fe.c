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
    NatureDSP_Signal library. Diagnostic routines.
    Floating-point exceptions control
    IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* NatureDSP_Signal library diagnostic routines. */
#include "NatureDSP_Signal_diag.h"
/* Common utility declarations. */
#include "common.h"
/* Floating-point environment. */
#include "__fenv.h"

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
int NatureDSP_Signal_feclearexcept( int excepts )
{
#if HAVE_FPU
    return __feclearexcept(excepts);
#else
    return excepts;
#endif
} /* NatureDSP_Signal_feclearexcept() */

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
int NatureDSP_Signal_feraiseexcept( int excepts )
{
#if HAVE_FPU
    return __feraiseexcept(excepts);
#else
    return excepts;
#endif
} /* NatureDSP_Signal_feraiseexcept() */

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
int NatureDSP_Signal_fetestexcept( int excepts )
{
#if HAVE_FPU
    return __fetestexcept(excepts);
#else
    return excepts;
#endif
} /* NatureDSP_Signal_fetestexcept() */
