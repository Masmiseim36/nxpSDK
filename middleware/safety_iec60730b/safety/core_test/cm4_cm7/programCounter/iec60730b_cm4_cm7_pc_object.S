/************************************************************************************************
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2019 NXP.
 *
 * License: LA_OPT_NXP_Software_License
 *
 * This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * 
 * @version   4.0
 * 
 * @brief     attachment to PC test, this file should be placed in reliable address - by edditing the linker file
 *
 ***********************************************************************************************/

#define	__ASM__
   #include "iec60730b.h"
   #include "asm_mac_common.h"
#undef	__ASM__

ASM_COMP_SPECIFIC_DIRECTIVES
 ASM_CODE_SECTION(.text)


/* public_functions */ 
 ASM_PUBLIC(FS_PC_Object)

/*******************************************************************************
*
* brief    FS_PC_Object
*
* param    none
*
* return   void
*                         
* remarks  Function is used for performing PC test, it should be called only by FS_CM7_PC_Test() function
*
*******************************************************************************/ 
 ASM_PUBLIC_BEGIN(FS_PC_Object)
 ASM_PUBLIC_FUNC(FS_PC_Object)
ASM_LABEL(FS_PC_Object) 
     ADDS   R4, #4        /* increments the pattern */
     CMP    R4, R7        /* compare it with value taken from PC */
     BNE    PCErrorCM7    /* if the value in PC register is different (error) */  
     MOVS   R0, #0        /* pass return */
     STR    R0, [R2]      /* clear the flag */
     BX     LR
     
ASM_LABEL(PCErrorCM7)
     LDR    R0, =FS_PC_FAIL     /* pre-set the result to false */
     BX     LR
     
ASM_PUBLIC_END(FS_PC_Object)

     ASM_ALIGN(4)
     ASM_END
