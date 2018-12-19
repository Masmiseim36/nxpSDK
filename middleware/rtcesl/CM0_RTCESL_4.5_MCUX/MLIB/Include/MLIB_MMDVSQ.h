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


