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
* Macros 
*******************************************************************************/  
#define RTCESL_ON 1
#define RTCESL_OFF 0    
    
/*******************************************************************************
* User Modified Macros 
*******************************************************************************/     
/* Memory-Mapped Divide and Square Root Module (MMDVSQ) RTCESL_ON/RTCESL_OFF   
*  configuration. It is recommended to use MMDVSQ for KV1x devices, because Div,  
*  Rcp and Sqrt operations are quicker. */
    
/* The MMDVSQ can be enabled or disabled by the definition of the symbol in IDE
*  preprocessor setting.
*  To enable MMDVSQ module please define symbol: RTCESL_MMDVSQ_ON, to disable RTCESL_MMDVSQ please
*  define RTCESL_MMDVSQ_OFF (Div,Rcp of Sqrt functions which use them  will be
*  calculated by core).
*  If no symbol RTCESL_MMDVSQ_XXX is defined the RTCESL_MMDVSQ setting will be taken from 
*  this header file, see next paragraph. */
    
#if (defined (RTCESL_MMDVSQ_ON))   
    #define RTCESL_MMDVSQ RTCESL_ON
#elif (defined (RTCESL_MMDVSQ_OFF))
    #define RTCESL_MMDVSQ RTCESL_OFF
#else    
    /* User option to enable or disable MMDVSQ module when IDE preprocessor setting 
    *  was not used. Set the following define as RTCESL_ON or RTCESL_OFF to enable or  
    *  disable hardware MMDVSQ support. */  
    
    /* Next line can be modified by user */
    #define RTCESL_MMDVSQ RTCESL_OFF     
    
    /* Independent function call can be used as well, for example
    *  MLIB_Div_F16 ... depend on setting
    *  MLIB_DivHw_F16_C always make calculation in MMDVSQ module
    *  MLIB_Div_F16_Ci always make calculation in core */ 
#endif     

/* Warning if RTCESL_MMDVSQ was not correctly defined */   
#ifndef RTCESL_MMDVSQ 
    #warning "RTCESL_MMDVSQ was not defined !" 
#endif    
/* Warning if RTCESL_MMDVSQ_ON and RTCESL_MMDVSQ_OFF are defined in IDE */       
#if defined(RTCESL_MMDVSQ_ON) && defined(RTCESL_MMDVSQ_OFF)    
    #warning "RTCESL_MMDVSQ_ON and RTCESL_MMDVSQ_OFF are defined !" 
#endif       
    
/* Inline assembler function optimization setting */  
/* Only for functions written as inline assembler. The setting can be changed, but the RTCESL was tested
   with following original setting. In case of any change the functionality is not guaranteed.  */ 
   
#if defined(__IAR_SYSTEMS_ICC__)                         /* For IAR compiler   */
    #define RTCESL_INLINE_OPTIM_SAVE                     /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET                      /* Set specific level */
    #define RTCESL_INLINE_OPTIM_RESTORE                  /* Restore original level - no value*/
#elif defined(__CC_ARM)                                  /* For ARM(KEIL) compiler */
    #define RTCESL_INLINE_OPTIM_SAVE                     /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET                      /* Set specific level */
    #define RTCESL_INLINE_OPTIM_RESTORE                  /* Restore original level - no value*/
#elif defined(__GNUC__)                                  /* For GCC compiler */ 
    #define RTCESL_INLINE_OPTIM_SAVE                     /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET                      /* Set specific level */
    #define RTCESL_INLINE_OPTIM_RESTORE                  /* Restore original level - no value*/
#else                                                    /* Other compiler used */
    #warning "Unsupported compiler/IDE used !"    
#endif         

#endif  /*_RTCESL_CFG_H_*/

