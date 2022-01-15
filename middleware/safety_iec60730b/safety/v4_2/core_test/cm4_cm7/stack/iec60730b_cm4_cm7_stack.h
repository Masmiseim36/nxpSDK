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
 * @brief Stack overflow test routines for Cortex-M4/M7 core - IEC60730 Class B.
 */

#ifndef IEC60730B_CM4_CM7_STACK_H_
#define IEC60730B_CM4_CM7_STACK_H_

/*
 * List of devices and supported functions can be found in library user guide,
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* Return values */
#define FS_FAIL_STACK (FS_STACK_CODE + 0x01)

/* Assembler skips following section */
#ifndef __ASM__

/******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 *
 * @brief    Function performs initialization of Stack test.
 *           blockSize must be aligned to multiple of 4. Min. blocksize = 0x4.
 *           Corresponds to one 32-bit memory space.
 *
 * @param    stackTestPattern - pattern to be written into the areas (e.g. 0x77777777).
 * @param    firstAddress     - first address of block under the stack area.
 * @param    secondAddress    - first address of block above the stack area.
 * @param    blockSize        - size of areas under and above the stack.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_CM4_CM7_STACK_Init(uint32_t stackTestPattern,
                           uint32_t firstAddress,
                           uint32_t secondAddress,
                           uint32_t blockSize);

/*******************************************************************************
 *
 * @brief    Function performs Stack overflow test.
 *           blockSize must be aligned to multiple of 4. Min. blocksize = 0x4.
 *           Corresponds to one 32-bit memory space.
 *
 * @param    stackTestPattern - test pattern (e.g. 0x77777777).
 * @param    firstAddress     - first address of block in front of stack.
 * @param    secondAddress    - first address of block behind the stack.
 * @param    blockSize        - block size.
 *
 * @return   FS_PASS
 *           FS_FAIL_STACK
 *
 ******************************************************************************/
FS_RESULT FS_CM4_CM7_STACK_Test(uint32_t stackTestPattern,
                                uint32_t firstAddress,
                                uint32_t secondAddress,
                                uint32_t blockSize);

#endif /* __ASM__ */
#endif /* IEC60730B_CM4_CM7_STACK_H_ */
