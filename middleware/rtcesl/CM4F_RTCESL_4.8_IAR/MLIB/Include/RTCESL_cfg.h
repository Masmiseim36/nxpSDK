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
* @brief  RTCESL configuration file
* 
*******************************************************************************/
#ifndef _RTCESL_CFG_H_
#define _RTCESL_CFG_H_

#if defined(__cplusplus)
extern "C" {
#endif 
    
/*******************************************************************************
* RTCESL vesion 
*******************************************************************************/  
/* RTCESL version is 4.7 */  
#define RTCESL_VERSION 4.7

  
/*******************************************************************************
* User Modified Macros 
*******************************************************************************/     
/* Inline assembler function optimization setting */  
/* Only for functions written as inline assembler. The setting can be changed, but the RTCESL was tested
   with following original setting. In case of any change the functionality is not guaranteed.  
   This is a solution for each compiler which have the issue of passing function parameter on to the function 
   when maximum speed optimization is used. Therefore optimization level is decreased and no save/restore 
   pragma is used for inline assembler functions. */ 
   
#if defined(__IAR_SYSTEMS_ICC__)                                    /* For IAR compiler   */
    #define RTCESL_INLINE_OPTIM_SAVE                                /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET _Pragma("optimize=none")        /* Set low level */
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* Restore original level - no value*/
#elif defined(__CC_ARM)                                             /* For ARM(KEIL) compiler */
    #define RTCESL_INLINE_OPTIM_SAVE                                /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET _Pragma("O0")                   /* Set low level */
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* Restore original level - no value*/
#elif defined(__GNUC__) && (__ARMCC_VERSION)                        /* For ARM(KEIL) version >= 6 compiler */
    #define RTCESL_INLINE_OPTIM_SAVE                                /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET                                 /* Set low level */
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* Restore original level - no value*/
#elif defined(__GNUC__)                                             /* For GCC compiler */ 
    #define RTCESL_INLINE_OPTIM_SAVE                                /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET __attribute__((optimize("O0"))) /* Set low level */
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* Restore original level - no value*/
#else                                                               /* Other compiler used */
    #warning "Unsupported compiler/IDE used !"    
#endif       

#define RAM_FUNC_LIB                              /* function executed from ROM */

#if defined(__cplusplus)
}
#endif

#endif  /*_RTCESL_CFG_H_*/
