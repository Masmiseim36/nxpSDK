/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2021 NXP.
 *
 * License: LA_OPT_NXP_Software_License
 *
 * This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms. If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * @version 4.1
 *
 * @brief Program counter test routines for Cortex-M4/M7 core - IEC60730 Class B.
 */

#define __ASM__
    #include "iec60730b_core.h"
    #include "asm_mac_common.h"
#undef  __ASM__

ASM_COMP_SPECIFIC_DIRECTIVES
 ASM_CODE_SECTION(.text)

/*******************************************************************************
 * Functions
 ******************************************************************************/
 ASM_PUBLIC(FS_PC_Object)

/*******************************************************************************
 * Implementation
 ******************************************************************************/
/*******************************************************************************
 ******************************************************************************/
ASM_PUBLIC_BEGIN(FS_PC_Object)
ASM_PUBLIC_FUNC(FS_PC_Object)
ASM_LABEL(FS_PC_Object) 
    ADDS   R4, #4             /* increments the pattern */
    CMP    R4, R7             /* compare it with value taken from PC */
    BNE    PCErrorCM7         /* if the value in PC register is different (error) */  
    MOVS   R0, #FS_PASS  /* pass return */
    STR    R0, [R2]           /* clear the flag */
    BX     LR

ASM_LABEL(PCErrorCM7)
    LDR    R0, =FS_FAIL_PC    /* pre-set the result to false */
    BX     LR

ASM_PUBLIC_END(FS_PC_Object)


 ASM_ALIGN(4)
 ASM_END
