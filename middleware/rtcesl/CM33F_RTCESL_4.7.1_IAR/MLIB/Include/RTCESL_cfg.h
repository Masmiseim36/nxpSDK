/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
* Macros 
*******************************************************************************/  
#define RTCESL_ON 1
#define RTCESL_OFF 0    
    
/*******************************************************************************
* User Modified Macros 
*******************************************************************************/        
/* PowerQuad (PQ) RTCESL_ON/RTCESL_OFF configuration. PowerQuad hardware accelerator 
*  for (fixed and floating point unit) CMSIS DSP functions with support of software 
*  API faster execution of ARM CMSIS instruction set. 
*  It is recommended to use PQ for LPC55S6x/LPC55S2x/LPC552x devices, 
*  because some function are quicker. */
    
/* The PQ can be enabled or disabled by the definition of the symbol in IDE
*  preprocessor setting.
*  To enable PQ module please define symbol: RTCESL_PQ_ON, to disable the PQ please
*  define RTCESL_PQ_OFF (All functions will be calculated by core).
*
*  If no symbol RTCESL_PQ_XXX is defined the RTCESL_PQ setting will be taken from 
*  this header file, see next paragraph. */
    
#if (defined (RTCESL_PQ_ON))   
    #define RTCESL_PQ RTCESL_ON
#elif (defined (RTCESL_PQ_OFF))
    #define RTCESL_PQ RTCESL_OFF
#else    
    /* User option to enable or disable PQ module when IDE preprocessor setting 
    *  was not used. Set the following define as RTCESL_ON or RTCESL_OFF to enable or  
    *  disable hardware PQ support. */  
    
    /* Next line can be modified by user */
    #define RTCESL_PQ RTCESL_ON     
    
    /* Independent function call can be used as well, for example
    *  MLIB_Div_F16 ... depend on setting
    *  MLIB_DivPQ_F16_C always make calculation in PQ module
    *  MLIB_Div_F16_Ci always make calculation in core */ 
#endif     

/* Warning if RTCESL_PQ was not correctly defined */   
#ifndef RTCESL_PQ 
    #warning "RTCESL_PQ was not defined !" 
#endif    
/* Warning if RTCESL_PQ_ON and RTCESL_PQ_OFF are defined in IDE */       
#if defined(RTCESL_PQ_ON) && defined(RTCESL_PQ_OFF)
    #warning "RTCESL_PQ_ON and RTCESL_PQ_OFF are defined !" 
#endif       
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
