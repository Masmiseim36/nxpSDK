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
* @brief  MLIB_MMDVSQ.h
* 
*******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/******************************************************************************
* Includes
******************************************************************************/   
#include "mlib_types.h"

/*******************************************************************************
* Macros
*******************************************************************************/    
/* Base address of Memory-Mapped Divide and Square Root peripheral module(MMDVSQ) */ 
#define RTCESL_MMDVSQ_BASE_PTR 0xF0004000   

/* MMDVSQ registers definitions
*  Absolute      Register name                        Width      Access Reset value
*  address (hex)                                      (in bits)
*  F000_4000     Dividend Register (MMDVSQ_DEND)      32         R/W    Undefined
*  F000_4004     Divisor Register (MMDVSQ_DSOR)       32         R/W    Undefined
*  F000_4008     Control/Status Register (MMDVSQ_CSR) 32         R/W    Some bits undefined
*  F000_400C     Result Register (MMDVSQ_RES)         32         R/W    Undefined
*  F000_4010     Radicand Register (MMDVSQ_RCND)      32         W      Undefined    */
    
#define RTCESL_MMDVSQ_DEND  *(volatile unsigned int*)(RTCESL_MMDVSQ_BASE_PTR + 0x0)
#define RTCESL_MMDVSQ_DSOR  *(volatile unsigned int*)(RTCESL_MMDVSQ_BASE_PTR + 0x4)
#define RTCESL_MMDVSQ_CSR   *(volatile unsigned int*)(RTCESL_MMDVSQ_BASE_PTR + 0x8)
#define RTCESL_MMDVSQ_RES   *(volatile unsigned int*)(RTCESL_MMDVSQ_BASE_PTR + 0xC)
#define RTCESL_MMDVSQ_RCND  *(volatile unsigned int*)(RTCESL_MMDVSQ_BASE_PTR + 0x10)

/*******************************************************************************
* Variables
*******************************************************************************/    
/* Global variable which store actual MMDVSQ state to avoid register 
   rewriting during interrupt service routine handling */  
extern volatile uint16_t gu16CntMmdvsq;  

#if defined(__cplusplus)
}
#endif


