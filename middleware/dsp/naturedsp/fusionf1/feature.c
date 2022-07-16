/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
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
    support for conditionalization: detect if some function is present or 
    not
*/
#include "NatureDSP_types.h"
#include "NatureDSP_Signal.h"
#include "common.h"

/*-------------------------------------------------------------------------
Returns non-zero if given function (by its address) is supported by
specific processor capabilities
Parameters:
    Input:
     fun    one of function from the list above

NOTE:
in gcc/xcc environment, calls of this function is not neccessary - if 
function pointer is non-zero it means it is supported. VisualStudio linker 
does not support section removal so this function might be used for 
running library under MSVC environment
-------------------------------------------------------------------------*/

#ifdef COMPILER_MSVC
/*  ------------------------- MSVC code ------------------------- */

#if (_MSC_VER<1500) // (Visual Studio 2005 and earlier)
unsigned char _BitScanReverse(
   unsigned long * Index,
   unsigned long Mask
)
{
    int idx=0;
    if(Mask==0) return 0;
    while(Mask & 1)
    {
        idx++;
        Mask>>=1;
    }
    return idx;
}
#endif  


/* 
    dummy function that does nothing. It just marks the 
    memory region where discarding library functions 
    begins from
*/
extern  "C" void ____begin_discarding____ (void) ;
#pragma alloc_text( "$$$$$$$$$$", ____begin_discarding____ )
static void ____begin_discarding____ (void) {}

int __NatureDSP_Signal_isPresent(NatureDSP_Signal_funptr fun)
{
    return ((uintptr_t)fun)<((uintptr_t)____begin_discarding____) && (fun!=NULL);
}

#endif
/*  ------------------------- GCC code ------------------------- */
#ifdef COMPILER_GNU
int __NatureDSP_Signal_isPresent(NatureDSP_Signal_funptr fun)
{
    return fun!=NULL;
}
#endif

#ifdef COMPILER_XTENSA
/*  ------------------------- XCC code ------------------------- */
int __NatureDSP_Signal_isPresent(NatureDSP_Signal_funptr fun)
{
    int* ptr=(int*)fun;  
    if (ptr==NULL) return 0;
    if (ptr[0]!=0x49438B96) return 1;
    if (ptr[1]!=0x4D73F192) return 1;
    return 0;
}
#endif
