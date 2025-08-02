/*******************************************************************************
*
* Copyright 2013-2016, Freescale Semiconductor, Inc.
* Copyright 2024 NXP
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
* @brief  MLIB_MAU.h
* 
*******************************************************************************/
#ifndef _MLIB_MAU_H_
#define _MLIB_MAU_H_

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
/* ----------------------------------------------------------------------------
   -- MAU Peripheral Access Layer
   ---------------------------------------------------------------------------- */
/* Base address of Math Accelerator Unit(MAU) */ 
#define RTCESL_MAU_BASE_PTR     0x40108000U 
/** Peripheral MRCC0 base address */
#define RTCESL_MRCC0_PTR        0x40091000U
/** Peripheral SYSCON0 base pointer */
#define RTCESL_SYSCON_PTR       0x40091000U
    
#define RTCESL_MAU_DT_SET(dt)     ((dt) << 9U)
#define RTCESL_MAU_RES_SET(res)   ((res) << 7U)
#define RTCESL_MAU_MOPC_SET(mopc) ((mopc) << 2U)

#define RTCESL_MAU_IND_ADDR(base, dt, ds, mopc) ((base) | 0x800U | RTCESL_MAU_DT_SET(dt) | \
                                                 RTCESL_MAU_RES_SET(ds) | RTCESL_MAU_MOPC_SET(mopc))

#define RTCESL_MAU_REG_UINT32(addr) (*((volatile uint32_t *)(addr)))
#define RTCESL_MAU_REG_Q15(addr)    (*((volatile frac16_t *)(addr)))
#define RTCESL_MAU_REG_Q31(addr)    (*((volatile frac32_t *)(addr)))
#define RTCESL_MAU_REG_FLOAT(addr)  (*((volatile  float_t *)(addr)))    

/* MAU Registers access macros */
#define RTCESL_MAU_RES0_ADDR      (RTCESL_MAU_BASE_PTR + 0x40)
#define RTCESL_MAU_RES1_ADDR      (RTCESL_MAU_BASE_PTR + 0x44)
#define RTCESL_MAU_RES2_ADDR      (RTCESL_MAU_BASE_PTR + 0x48)
#define RTCESL_MAU_RES3_ADDR      (RTCESL_MAU_BASE_PTR + 0x4C)
#define RTCESL_MAU_SYS_CTLR       *(volatile uint32_t*)(RTCESL_MAU_BASE_PTR + 0x10)
#define RTCESL_MAU_GEXP_STATUS_IE *(volatile uint32_t*)(RTCESL_MAU_BASE_PTR + 0x14)
#define RTCESL_MAU_GEXP_STATUS    *(volatile uint32_t*)(RTCESL_MAU_BASE_PTR + 0x30)
#define RTCESL_MAU_OP_CTRL        *(volatile uint32_t*)(RTCESL_MAU_BASE_PTR + 0x30)
#define RTCESL_MAU_RES_STATUS_IE  *(volatile uint32_t*)(RTCESL_MAU_BASE_PTR + 0x38)
#define RTCESL_MAU_RES_STATUS     *(volatile uint32_t*)(RTCESL_MAU_BASE_PTR + 0x3C)
#define RTCESL_MAU_RES0           *(volatile uint32_t*)(RTCESL_MAU_RES0_ADDR)
#define RTCESL_MAU_RES1           *(volatile uint32_t*)(RTCESL_MAU_RES1_ADDR)
#define RTCESL_MAU_RES2           *(volatile uint32_t*)(RTCESL_MAU_RES2_ADDR)
#define RTCESL_MAU_RES3           *(volatile uint32_t*)(RTCESL_MAU_RES3_ADDR)
#define RTCESL_MAU_CLOCK          *(volatile uint32_t*)(RTCESL_SYSCON_PTR + 0x64 ) /* MRCC0->MRCC_GLB_CC2_SET */
#define RTCESL_SYSCON_CLKUNLOCK   *(volatile uint32_t*)(RTCESL_SYSCON_PTR + 0x3FC) /* SYSCON->CLKUNLOCK */
#define RTCESL_MRCC0_GLB_RST2     *(volatile uint32_t*)(RTCESL_MRCC0_PTR  + 0x24 ) /* MRCC0->MRCC_GLB_RST2_SET */

/* MAU data type definitions */
#define RTCESL_MAU_DT_UINT  0
#define RTCESL_MAU_DT_INT   1
#define RTCESL_MAU_DT_Q1X   2
#define RTCESL_MAU_DT_FLOAT 3

/* MAU result register definitions. */
#define RTCESL_MAU_DT_RES0 0
#define RTCESL_MAU_DT_RES1 1
#define RTCESL_MAU_DT_RES2 2
#define RTCESL_MAU_DT_RES3 3

/* MAU calculation code. */
#define RTCESL_MAU_MOPC_BYPASS     0U
#define RTCESL_MAU_MOPC_RECIP      1U
#define RTCESL_MAU_MOPC_SQRT       2U
#define RTCESL_MAU_MOPC_SQRT_RECIP 3U
#define RTCESL_MAU_MOPC_COS        8U
#define RTCESL_MAU_MOPC_SIN        9U
#define RTCESL_MAU_MOPC_ATAN       12
    
/*******************************************************************************
* Types
*******************************************************************************/
/* ----------------------------------------------------------------------------
   -- MAU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/****************************************************************************
* Inline functions 
****************************************************************************/ 
/***************************************************************************//*!
* @brief  The function initialize MAU0 module for operation. Shall be called 
*         before first AMU using. This function has been checked only with 
*         MCXA166 for other can be replaced by MAU_Init() function from SDK.
*
* @param  void
*
*******************************************************************************/
static inline void RTCESL_MAU_Init()
{
    /*** Enable MAU0 clock ***/
    /* Unlock clock configuration */
    RTCESL_SYSCON_CLKUNLOCK &= ~(0x1U);
    /* Enable MAU clock */
    RTCESL_MAU_CLOCK = (1UL << 9u);
    /* Freeze clock configuration */
    RTCESL_SYSCON_CLKUNLOCK |= (0x1U);
    
    /*** Release peripheral Reset configuration ***/
    /* Unlock clock configuration */
    RTCESL_SYSCON_CLKUNLOCK &= ~(0x1U);  
    /* reset register is in MRCC */
    RTCESL_MRCC0_GLB_RST2 = 0x200;
    /* Freeze clock configuration */
    RTCESL_SYSCON_CLKUNLOCK |= (0x1U);   
    
    /*** Disable all MAU interrupts ***/
    RTCESL_MAU_RES_STATUS_IE = (uint32_t)0;
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAU_H_ */
