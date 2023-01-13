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
 * @version 4.2
 *
 * @brief Invariable memory common routines - IEC60730 Class B.
 */

#ifndef IEC60730B_INVARIABLE_MEMORY_H_
#define IEC60730B_INVARIABLE_MEMORY_H_

#include "iec60730b_types.h"

/*
 * List of devices and supported functions can be found in library user guide,
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* Return values */
#define FS_FAIL_FLASH_C              (FS_FLASH_C_CODE + 0x1U)
#define FS_FAIL_FLASH_MODULO_C       (FS_FLASH_C_CODE + 0x2U)
#define FS_FAIL_FLASH_NULL_POINTER_C (FS_FLASH_C_CODE + 0x3U)
#define FS_FAIL_FLASH_SIZE_C         (FS_FLASH_C_CODE + 0x4U)


/* Assembler skips following section */
#ifndef __ASM__

/*******************************************************************************
 *
 * @brief    This function generate the 16-bit CRC value using K type of HW CRC module.
 *
 * @param    startAddress  - first address of the tested memory.
 * @param    size          - size of tested memory.
 * @param    moduleAddress - address of CRC module.
 * @param    crcVal        - Pointer to 16bit variable to return of resulted CRC.
 *                           Value of this variable is used asstart condition seed
 *                           for the first iteration it is
 *                           typically user defined value, for the next iterations
 *                           it is the result from the previous function call).
 *
 * @return   FS_RESULT : FS_FAIL_FLASH_C, FS_FAIL_FLASH_MODULO_C, FS_FAIL_FLASH_NULL_POINTER_C, FS_FAIL_FLASH_SIZE_C.
 *
 ******************************************************************************/
FS_RESULT FS_FLASH_C_HW16_K(uint32_t startAddress, uint32_t size, FS_CRC_Type * moduleAddress, uint16_t* crcVal);

/*******************************************************************************
 *
 * @brief    This function generate the 16-bit CRC value using L type of HW CRC module .
 *
 * @param    startAddress  - first address of the tested memory.
 * @param    size          - size of tested memory.
 * @param    moduleAddress - address of CRC module.
 * @param    crcVal        - Pointer to 16bit variable to return of resulted CRC.
 *                           Value of this variable is used asstart condition seed
 *                           for the first iteration it is
 *                           typically user defined value, for the next iterations
 *                           it is the result from the previous function call).
 *
 * @return   FS_RESULT : FS_FAIL_FLASH_C, FS_FAIL_FLASH_MODULO_C, FS_FAIL_FLASH_NULL_POINTER_C, FS_FAIL_FLASH_SIZE_C.
 *
 ******************************************************************************/
FS_RESULT FS_FLASH_C_HW16_L(uint32_t startAddress, uint32_t size, FS_CRC_L_Type * moduleAddress, uint16_t* crcVal);

#endif /* __ASM__ */
#endif /* IEC60730B_INVARIABLE_MEMORY_H_ */
