/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
* 
*
*******************************************************************************/


/*******************************************************************************
*
* file      IEC60730B_B_CM4_CM7_pc_object.S
*
* author    B48632
* 
* version   3.0
*
* date      Nov-01-2018
* 
* brief     attachment to PC test, this file should be placed in reliable address - by edditing the linker file
*
*******************************************************************************/
#define	__ASM__
   #include "iec60730b.h"
   #include "compiler/asm_mac_common.h"
#undef	__ASM__

ASM_COMP_SPECIFIC_DIRECTIVES
 ASM_CODE_SECTION(.text)


/* public_functions */ 
 ASM_PUBLIC(IEC60730B_PC_object)

/*******************************************************************************
*
* brief    IEC60730B_PC_object
*
* param    none
*
* return   void
*                         
* remarks  Function is used for performing PC test, it should be called only by IEC60730B_CM7_PC_Test() function
*
*******************************************************************************/ 
 ASM_PUBLIC_BEGIN(IEC60730B_PC_object)
 ASM_PUBLIC_FUNC(IEC60730B_PC_object)
ASM_LABEL(IEC60730B_PC_object) 
     ADDS   R4, #4        /* increments the pattern */
     CMP    R4, R7        /* compare it with value taken from PC */
     BNE    PCErrorCM7    /* if the value in PC register is different (error) */  
     MOVS   R0, #0        /* pass return */
     STR    R0, [R2]      /* clear the PC_test_flag */
     BX     LR
     
ASM_LABEL(PCErrorCM7)
     LDR    R0, =IEC60730B_ST_PC_FAIL     /* pre-set the result to false */
     BX     LR
     
ASM_PUBLIC_END(IEC60730B_PC_object)

     ASM_ALIGN(4)
     ASM_END
