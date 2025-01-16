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
#ifndef __NATUREDSP_SIGNAL_ID_H__
#define __NATUREDSP_SIGNAL_ID_H__

#include "NatureDSP_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
  Identification Routines
  NatureDSP_Signal_get_library_version     Library Version Request
  NatureDSP_Signal_get_library_api_version Library API Version Request
===========================================================================*/

/*-------------------------------------------------------------------------
   Library Version Request
   This function returns library version information.
   Parameters:
    Input:
    none:
    Output:
     version_string   Pre-allocated buffer for version string.
   Restrictions:
     version_string must points to a buffer large enough to hold up to
     30 characters.
-------------------------------------------------------------------------*/
void NatureDSP_Signal_get_library_version(char *version_string);

/*-------------------------------------------------------------------------
   Library API Version Request
   This function returns library API version information
   Parameters:
    Input:
    none
    Output:
    version_string   Pre-allocated buffer for API version string.
   Restrictions:
     version_string must points to a buffer large enough to hold up to
     30 characters.
-------------------------------------------------------------------------*/
void NatureDSP_Signal_get_library_api_version(char *version_string);

/*-------------------------------------------------------------------------
  Library API Capability Request
  This function returns non-zero if given function (by its address) is 
  supported by specific processor capabilities (i.e. VFPU option). 
  NOTE:
  1.	in the gcc/xcc environment, calls of this function are not necessary
      - if function pointer is non-zero it means it is supported. 
      VisualStudio linker does not support section removal so this function 
      might be used for running library under MSVC environment.
  2.	Very few library functions may disable their capabilities dynamically 
      (only for particular combination of input parameters). Behavior for 
      such situation is defined in the description of those functions.  

  Parameters:
      Input:
       fun    one of library functions
  Returned Value:
    	non-zero if function is supported by library
-------------------------------------------------------------------------*/
typedef void(*NatureDSP_Signal_funptr)();
int __NatureDSP_Signal_isPresent(NatureDSP_Signal_funptr fun);
#define NatureDSP_Signal_isPresent(fun) __NatureDSP_Signal_isPresent((NatureDSP_Signal_funptr)fun)

#ifdef __cplusplus
}
#endif

#endif/* __NATUREDSP_SIGNAL_ID_H__ */
