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
 * @brief     IEC60730 Class B Safety Routines Stack overflow test for Cortex CM4, CM7 cores
 *
 ***********************************************************************************************/

#ifndef _IEC60730B_CM4_CM7_STACK_
#define _IEC60730B_CM4_CM7_STACK_

/*******************************************************************************
* Macros
*******************************************************************************/
#define FS_STACK_PASS    (0x0000)
#define FS_STACK_FAIL    (FS_STACK_FAIL_CODE + 0x01)

/*******************************************************************************
* Global functions
*******************************************************************************/
#ifndef __ASM__

/*******************************************************************************
*
* brief    FS_CM4_CM7_STACK_Init
*
* param    stackTestPattern, firstAddress, secondAddress, blockSize
*
* return   void
*
* remarks  Function performs initialization of Stack test. blockSize must be multiple of 4.
* blockSize = 0x4 - the minimal size. Corresponds to one 32-bit memory space
*
*******************************************************************************/
void FS_CM4_CM7_STACK_Init(uint32_t stackTestPattern, uint32_t firstAddress, uint32_t secondAddress, uint32_t blockSize);

/*******************************************************************************
*
* brief  FS_CM4_CM7_STACK_Test
*
* param  stackTestPattern, firstAddress, secondAddress, blockSize
*
* return Result of test: FS_STACK_PASS  - Test successfuly done
*                        FS_STACK_FAIL  - Test failed
*
* remarks Function performs Stack overflow test. blockSize must be multiple of 4.
* blockSize = 0x4 - the minimal size. Corresponds to one 32-bit memory space
*
*******************************************************************************/
FS_RESULT FS_CM4_CM7_STACK_Test(uint32_t stackTestPattern, uint32_t firstAddress, uint32_t secondAddress, uint32_t blockSize);

#endif /* __ASM__ */

#endif /* _IEC60730B_CM4_CM7_STACK_ */
