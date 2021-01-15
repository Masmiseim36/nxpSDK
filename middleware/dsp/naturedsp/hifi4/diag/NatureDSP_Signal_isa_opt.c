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
    Discover hardware capabilities
    IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* NatureDSP_Signal library diagnostic routines. */
#include "NatureDSP_Signal_diag.h"
/* Common utility declarations. */
#include "common.h"

/*-------------------------------------------------------------------------
Query of a particular ISA option or a parameter.
Input:
    isa_opt   ISA option or parameter identifier, one of NATUREDSP_ISA_OPT_
              symbols
Return value:
    Integral status value of the requested item, or -1 for an unknown value
    of the argument.
-------------------------------------------------------------------------*/
int NatureDSP_Signal_get_isa_opt( int isa_opt )
{
    switch (isa_opt) {
    case NATUREDSP_ISA_OPT_INT16_SIMD_WIDTH: return ((XCHAL_DATA_WIDTH)/sizeof(int16_t)); break;
    case NATUREDSP_ISA_OPT_HAVE_FP:          return (XCHAL_HAVE_FP                     ); break;
    case NATUREDSP_ISA_OPT_HAVE_DFP:         return (XCHAL_HAVE_DFP                    ); break;
    default: return (-1);
    }

} /* NatureDSP_Signal_get_isa_opt() */
