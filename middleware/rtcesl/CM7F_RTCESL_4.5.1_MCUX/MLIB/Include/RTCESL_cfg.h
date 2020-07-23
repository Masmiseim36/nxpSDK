/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
/* RTCESL version is 4.5.1 */  
#define RTCESL_VERSION 4.5.1

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
#elif defined(__CC_ARM)                                             /* For ARM(KEIL) version < 6 compiler */
    #define RTCESL_INLINE_OPTIM_SAVE                                /* No saving the original level - issue */
    #define RTCESL_INLINE_OPTIM_SET _Pragma("O0")                   /* Set lower optimatization level */
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* No restoring the original level - issue */
    #define RAM_FUNC_NAME __attribute__ ((section ("ram")))         /* Ram function */
#elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)            /* For ARM(KEIL) version >= 6 compiler */ 
    #define RTCESL_INLINE_OPTIM_SAVE                                /* Save original level - no value*/
    #define RTCESL_INLINE_OPTIM_SET                                 /* Set highest level - no value*/
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* Restore original level - no value*/
    #define RAM_FUNC_NAME __attribute__ ((section ("ram")))         /* Ram function */
#elif defined(__GNUC__)                                             /* For GCC compiler */ 
    #define RTCESL_INLINE_OPTIM_SAVE                                /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET __attribute__((optimize("O0"))) /* Set lower optimatization level */
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* Restore original level - no value*/
    #define RAM_FUNC_NAME __RAMFUNC(RAM)                            /* Ram function */
#else                                                               /* Other compiler used */
    #warning "Unsupported compiler/IDE used !"    
#endif       
  
  
 
/* Executing functions from RAM/ROM for RT devices to speed up function execution. If some functins should 
be executed from RAM for highed speed the user must define 
#define RAM_OPTIM_HIGH or 
#define RAM_OPTIM_MEDIUM or 
#define RAM_OPTIM_LOW
depending on required option
If any RAM_OPTIM_XXXX define is evaluatef to TRUE RTCESL funtion will be executed from RAM becuse the RAM_FUNC_LIB
is always defined. The RAM_FUNC_CRITICAL and the RAM_FUNC defines are determine for application code. */
#if defined(RAM_OPTIM_HIGH)                       /* All functions will be stored in variable memories*/
    #define RAM_FUNC             RAM_FUNC_NAME    /* function executed from RAM */
    #define RAM_FUNC_CRITICAL    RAM_FUNC_NAME    /* critical function executed from RAM */
    #define RAM_FUNC_LIB         RAM_FUNC_NAME    /* libraries executed from RAM */
#elif defined(RAM_OPTIM_MEDIUM)                   /* Some functions will be stored in variable memories*/
    #define RAM_FUNC                              /* function executed from ROM */
    #define RAM_FUNC_CRITICAL    RAM_FUNC_NAME    /* critical functions executed from RAM */
    #define RAM_FUNC_LIB         RAM_FUNC_NAME    /* libraries executed from RAM */
#elif defined(RAM_OPTIM_LOW)                      /* Some function will be stored in variable memories*/
    #define RAM_FUNC                              /* function executed from ROM */
    #define RAM_FUNC_CRITICAL                     /* critical function executed from ROM */
    #define RAM_FUNC_LIB         RAM_FUNC_NAME    /* libraries executed from RAM */ 
#else                                             /* All functions will be stored in imvariable memories*/
    #define RAM_FUNC                              /* function executed from ROM */
    #define RAM_FUNC_CRITICAL                     /* critical function executed from ROM */
    #define RAM_FUNC_LIB                          /* libraries executed from ROM */
#endif
#if defined(RAM_OPTIM_HIGH) && defined(RAM_OPTIM_MEDIUM) /* More RAM_OPTIM defines detected*/
    #warning "More RAM_OPTIM define were defined !"
#endif
#if defined(RAM_OPTIM_MEDIUM) && defined(RAM_OPTIM_LOW)  /* More RAM_OPTIM defines detected*/
    #warning "More RAM_OPTIM define were defined !"
#endif
#if defined(RAM_OPTIM_LOW) && defined(RAM_OPTIM_HIGH)    /* More RAM_OPTIM defines detected*/
    #warning "More RAM_OPTIM define were defined !"   
#endif
  
#endif  /*_RTCESL_CFG_H_*/
