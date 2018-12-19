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
* @brief  RTCESL configuration file
* 
*******************************************************************************/
#ifndef _RTCESL_CFG_H_
#define _RTCESL_CFG_H_

#if defined(__cplusplus)
extern "C" {
#endif 
    
/*******************************************************************************
* User Modified Macros 
*******************************************************************************/     
/* Inline assembler function optimization setting */  
/* Only for functions written as inline assembler. The setting can be changed, but the RTCESL was tested
   with following original setting. In case of any change the functionality is not guaranteed.  
   This is a solution for the ARM(KEIL) compiler issue of passing function parameter on to the function 
   when maximum speed optimization is used. Therefore optimization level is decreased and no save/restore 
   pragma is used for inline assembler functions in ARM(KEIL). For IAR and GCC no inline function optimization 
   change is done */ 
   
#if defined(__IAR_SYSTEMS_ICC__)                         /* For IAR compiler   */
    #define RTCESL_INLINE_OPTIM_SAVE                     /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET                      /* Set highest level - no value */
    #define RTCESL_INLINE_OPTIM_RESTORE                  /* Restore original level - no value*/
#elif defined(__CC_ARM)                                  /* For ARM(KEIL) compiler */
    #define RTCESL_INLINE_OPTIM_SAVE                     /* No saving the original level - issue */
    #define RTCESL_INLINE_OPTIM_SET     _Pragma("O0")    /* Set lower level */
    #define RTCESL_INLINE_OPTIM_RESTORE                  /* No restoring the original level - issue */
#elif defined(__GNUC__)                                  /* For GCC compiler */ 
    #define RTCESL_INLINE_OPTIM_SAVE                                    /* Save original level */
    #define RTCESL_INLINE_OPTIM_SET     __attribute__((optimize("O0"))) /* Set highest level */
    #define RTCESL_INLINE_OPTIM_RESTORE                                 /* Restore original level */
#else                                                    /* Other compiler used */
    #warning "Unsupported compiler/IDE used !"    
#endif       

#endif  /*_RTCESL_CFG_H_*/
