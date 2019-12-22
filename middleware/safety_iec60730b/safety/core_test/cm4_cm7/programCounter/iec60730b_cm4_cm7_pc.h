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
 * @brief     IEC60730 Class B Safety Routines Program counter core register tests for Cortex CM4, CM7 cores
 *
 ***********************************************************************************************/

#ifndef _IEC60730B_CM4_CM7_PC_
#define _IEC60730B_CM4_CM7_PC_

/*******************************************************************************
* Macros
*******************************************************************************/
#define FS_PC_PASS  (0x0000)
#define FS_PC_FAIL  (FS_PC_FAIL_CODE + 0x01)

/******************************************************************************
* Global functions
******************************************************************************/
#ifndef __ASM__

typedef void (*tFcn_pc)(void);  /*  for using the address of function FS_PC_Object as a parameter of FS_CM7_PC_Test ***********
* brief        This function executes the PC test for devices with CM4 and CM7 cores .
* param[in]    pattern1         Address from RAM memory, adequate as a pattern for PC.
* param[in]    pObjectFunction Address of FS_PC_Object() function
* param[in]    *flag address of variable / place in memory used as a flag. If it is 0, test was successful, if 1, test went wrong
* return      FS_PC_PASS
*             FS_PC_FAIL
**********************************************************************************************************/
FS_RESULT FS_CM4_CM7_PC_Test(uint32_t pattern1, tFcn_pc pObjectFunction, uint32_t *flag);

/*********************************************************************************************************
* brief        This function is internally used by the FS_CM4_CM7_PC_Test() function.
* param[in]    none
* return       void
**********************************************************************************************************/
void FS_PC_Object(void);

#endif /* __ASM__ */

#endif /* _IEC60730B_CM4_CM7_PC_ */
