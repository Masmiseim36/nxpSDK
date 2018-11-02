/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#elif defined(__GNUC__)                                             /* For GCC compiler */ 
    #define RTCESL_INLINE_OPTIM_SAVE                                /* Save original level - no value */
    #define RTCESL_INLINE_OPTIM_SET __attribute__((optimize("O0"))) /* Set low level */
    #define RTCESL_INLINE_OPTIM_RESTORE                             /* Restore original level - no value*/
#else                                                               /* Other compiler used */
    #warning "Unsupported compiler/IDE used !"    
#endif       

#endif  /*_RTCESL_CFG_H_*/
