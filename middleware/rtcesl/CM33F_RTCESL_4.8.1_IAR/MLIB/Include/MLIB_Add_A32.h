/*******************************************************************************
*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
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
