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
 * @brief Flash test routines for Cortex-M4/M7 core - IEC60730 Class B.
 */

#ifndef IEC60730B_CM4_CM7_FLASH_H_
#define IEC60730B_CM4_CM7_FLASH_H_

/*
 * List of devices and supported functions can be found in library user guide,
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* WAS (Write As Seed) bit mask used in HW CRC module */
#define FS_FLASH_CRC_CTRL_WAS_MASK 0x2000000

/* Channel state */
#define FS_FAIL_FLASH_DCP      (FS_FLASH_CODE + 0x1U) /* DCP calculation error */
#define FS_FLASH_DCP_BUSY      (FS_FLASH_CODE + 0x2U) /* DCP channel busy */
#define FS_FLASH_DCP_AVAILABLE (FS_FLASH_CODE + 0x3U) /* DCP channel available */

/* Assembler skips following section */
#ifndef __ASM__

/******************************************************************************
 * Definitions
 ******************************************************************************/
/*! DCP states and results */
typedef struct
{
    uint32_t CH0State;
    uint32_t CH0Result;
    uint32_t CH0Packet[8];
    uint32_t CH1State;
    uint32_t CH1Result;
    uint32_t CH1Packet[8];
    uint32_t CH2State;
    uint32_t CH2Result;
    uint32_t CH2Packet[8];
    uint32_t CH3State;
    uint32_t CH3Result;
    uint32_t CH3Packet[8];
} fs_flash_dcp_state_t;

/* DCP channels */
typedef enum
{
    DCP_CH0 = (uint32_t)(1U << 16U),
    DCP_CH1 = (uint32_t)(1U << 17U),
    DCP_CH2 = (uint32_t)(1U << 18U),
    DCP_CH3 = (uint32_t)(1U << 19U)
} fs_flash_dcp_channels_t;

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
uint16_t FS_CM4_CM7_FLASH_HW16(uint32_t startAddress, uint32_t size, uint32_t moduleAddress, uint16_t crcVal);

/*******************************************************************************
 *
 * @brief    This function generate the 16-bit CRC value using SW.
 *
 * @param    startAddress  - first address of the tested memory.
 * @param    size          - size of tested memory.
 * @param    moduleAddress - has no effect. Just because of compatibility with HW function.
 * @param    crcVal        - start condition seed (for the first iteration it is
 *                           typically user defined value, for the next iterations
 *                           it is the result from the previous function call).
 *
 * @return   16-bit crc value of the memory range (CRC-16-CCITT - normal 0x1021).
 *
 ******************************************************************************/
uint16_t FS_CM4_CM7_FLASH_SW16(uint32_t startAddress, uint32_t size, uint32_t moduleAddress, uint16_t crcVal);

/*******************************************************************************
 *
 * @brief    This function generate the 32-bit CRC value using SW.
 *
 * @param    startAddress  - first address of the tested memory.
 * @param    size          - size of tested memory.
 * @param    moduleAddress - has no effect. Just because of compatibility with HW function.
 * @param    crcVal        - start condition seed (for the first iteration it is
 *                           typically user defined value, for the next iterations
 *                           it is the result from the previous function call).
 *
 * @return   32-bit crc value of the memory range (CRC-32/MPEG-2 - 0x04C11DB7).
 *
 ******************************************************************************/
uint32_t FS_CM4_CM7_FLASH_SW32(uint32_t startAddress, uint32_t size, uint32_t moduleAddress, uint32_t crcVal);

/*******************************************************************************
 *
 * @brief    This function generate the 32-bit CRC value using HW CRC module (DCP).
 *
 * @param    startAddress  - first address of the tested memory.
 * @param    size          - size of tested memory.
 * @param    moduleAddress - address of DCP module.
 * @param    crcVal        - start condition seed (for the first iteration it is
 *                           typically user defined value, for the next iterations
 *                           it is the result from the previous function call).
 * @param    channel       - DCP channel used for calculation.
 * @param    psDCPState    - state & result struct of each DCP channel.
 * @param    tag           - differentiates calculation on the same channel.
 *
 * @return   32-bit crc value of the memory range (CRC-32/MPEG-2 - 0x04C11DB7).
 *
 ******************************************************************************/
void FS_CM4_CM7_FLASH_HW32_DCP(uint32_t startAddress,
                               uint32_t size,
                               uint32_t moduleAddress,
                               uint32_t crcVal,
                               fs_flash_dcp_channels_t channel,
                               fs_flash_dcp_state_t *psDCPState,
                               uint32_t tag);


#endif /* __ASM__ */
#endif /* IEC60730B_CM4_CM7_FLASH_H_ */
