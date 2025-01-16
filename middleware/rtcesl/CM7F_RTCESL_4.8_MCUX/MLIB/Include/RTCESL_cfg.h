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

/* RAMFUNC macros */
#if defined(__IAR_SYSTEMS_ICC__)
#elif defined(__GNUC__)
  #define __SECTION(type, bank) __attribute__ ((section("." #type ".$" #bank)))
  #define __RAMFUNC(bank) __SECTION(ramfunc, bank)
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
   
#if defined(__IAR_SYSTEMS_ICC__)                                    /* For IAR compiler   */
    #define RTCESL_INLINE_OPTIM_SAVE                                /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET _Pragma("optimize=none")        /* Set lower optimatization level */
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* Restore original level - no value*/
    #define RAM_FUNC_NAME __ramfunc                                 /* Ram function */
    #define RTCESL_INLINE                                           /* Always inline option to force function inline */  
#elif defined(__CC_ARM)                                             /* For ARM(KEIL) compiler */
    #define RTCESL_INLINE_OPTIM_SAVE                                /* No saving the original level - issue */
    #define RTCESL_INLINE_OPTIM_SET _Pragma("O0")                   /* Set lower optimatization level */
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* No restoring the original level - issue */
    #define RAM_FUNC_NAME                                           /* Ram function */
    #define RTCESL_INLINE __attribute__((always_inline))            /* Always inline option to force function inline */ 
#elif defined(__GNUC__) && (__ARMCC_VERSION)                        /* For ARM(KEIL) version >= 6 compiler */
    #define RTCESL_INLINE_OPTIM_SAVE                                /* Save original level - no value*/
    #define RTCESL_INLINE_OPTIM_SET                                 /* Set highest level - no value*/
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* Restore original level - no value*/
    #define RAM_FUNC_NAME __attribute__ ((section ("ram")))         /* Ram function */
    #define RTCESL_INLINE                                           /* Always inline option to force function inline */ 
#elif defined(__GNUC__)                                             /* For GCC compiler */ 
    #define RTCESL_INLINE_OPTIM_SAVE                                /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET __attribute__((optimize("O0"))) /* Set lower optimatization level */
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* Restore original level - no value*/
    #define RAM_FUNC_NAME __RAMFUNC(RAM)                            /* Ram function */
    #define RTCESL_INLINE __attribute__((always_inline))            /* Always inline option to force function inline */ 
#else                                                               /* Other compiler used */
    #warning "Unsupported compiler/IDE used !"    
#endif       
  
/* Executing functions from RAM/ROM for RT devices to speed up function execution. If some functions should
be executed from RAM for higher speed the user must define
#define RAM_RELOCATION
define. */
#if defined(RAM_RELOCATION)                           /* All functions will be stored in variable memories*/
    #define RAM_FUNC_LIB             RAM_FUNC_NAME    /* function executed from RAM */
#else                                                 /* All functions will be stored in flash memories*/
    #define RAM_FUNC_LIB                              /* function executed from ROM */
#endif
  
#if defined(__cplusplus)
}
#endif

#endif  /*_RTCESL_CFG_H_*/
