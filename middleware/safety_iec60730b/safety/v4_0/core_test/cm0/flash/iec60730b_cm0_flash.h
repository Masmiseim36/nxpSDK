/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2020 NXP.
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
 * @version 4.0
 *
 * @brief Flash test routines for Cortex-M0+ core - IEC60730 Class B.
 */

#ifndef _IEC60730B_CM0_FLASH_H_
#define _IEC60730B_CM0_FLASH_H_

/*
 * List of devices and supported functions can be found in library user guide, 
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* WAS (Write As Seed) bit mask used in HW CRC module */  
#define FS_FLASH_CRC_CTRL_WAS_MASK  0x2000000

/* Assembler skips following section */
#ifndef __ASM__

/******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 *
 * @brief    This function generate the 16-bit CRC value using HW CRC module.
 *
 * @param    startAddress  - first address of the tested memory.
 * @param    size          - size of tested memory.
 * @param    moduleAddress - address of CRC module.
 * @param    crcVal        - start condition seed (for the first iteration it is
 *                           typically user defined value, for the next iterations
 *                           it is the result from the previous function call).
 *
 * @return   16-bit crc value of the memory range (CRC-16-CCITT - normal 0x1021).
 *
 ******************************************************************************/
uint16_t FS_CM0_FLASH_HW16(uint32_t startAddress, uint32_t size, uint32_t moduleAddress, uint16_t crcVal);

/*******************************************************************************
 *
 * @brief    This function generate the 16-bit CRC value using HW CRC module.
 *
 * @param    startAddress  - first address of the tested memory.
 * @param    size          - size of tested memory.
 * @param    moduleAddress - address of CRC module.
 * @param    crcVal        - start condition seed (for the first iteration it is
 *                           typically user defined value, for the next iterations
 *                           it is the result from the previous function call).
 *
 * @return   16-bit crc value of the memory range (CRC-16-CCITT - normal 0x1021).
 *
 ******************************************************************************/
uint16_t FS_CM0_FLASH_HW16_LPC(uint32_t startAddress, uint32_t size, uint32_t moduleAddress, uint16_t crcVal);

/*******************************************************************************
 *
 * @brief    This function generate the 16-bit CRC value using SW.
 *
 * @param    startAddress  - first address of the tested memory.
 * @param    size          - size of tested memory.
 * @param    moduleAddress - has no effect. just because of compatibility with HW function.
 * @param    crcVal        - start condition seed (for the first iteration it is
 *                           typically user defined value, for the next iterations
 *                           it is the result from the previous function call).
 *
 * @return   16-bit crc value of the memory range (CRC-16-CCITT - normal 0x1021).
 *
 ******************************************************************************/
uint16_t FS_CM0_FLASH_SW16(uint32_t startAddress, uint32_t size, uint32_t moduleAddress, uint16_t crcVal);   

#endif /* __ASM__ */
#endif /* _IEC60730B_CM0_FLASH_H_ */
