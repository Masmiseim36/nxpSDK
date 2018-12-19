/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
* 
*
****************************************************************************//*!
*
* @brief  Addition
* 
*******************************************************************************/
#ifndef _MLIB_ADD_A32_H_
#define _MLIB_ADD_A32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define MLIB_Add_A32ss_Ci(f16Add1, f16Add2) MLIB_Add_A32ss_FCi(f16Add1, f16Add2) 
#define MLIB_Add_A32as_Ci(a32Accum, f16Add) MLIB_Add_A32as_FCi(a32Accum, f16Add)  

/***************************************************************************//*!
*  Addition        
*  a32Out = f16Add1 + f16Add2
*  Result can overflow through fractional range and is stored in accumulator type
*******************************************************************************/
static inline acc32_t MLIB_Add_A32ss_FCi(register frac16_t f16Add1, register frac16_t f16Add2)
{ 
    return((acc32_t)((acc32_t)f16Add1 + (acc32_t)f16Add2));     
}
 
/***************************************************************************//*!
*  Addition        
*  a32Out = a32Accum + f16Add
*  Result can overflow through fractional range and is stored in accumulator type
*******************************************************************************/
static inline acc32_t MLIB_Add_A32as_FCi(register acc32_t a32Accum, register frac16_t f16Add)
{  
    return((acc32_t)(a32Accum + (acc32_t)(f16Add)));        
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_ADD_A32_H_*/
