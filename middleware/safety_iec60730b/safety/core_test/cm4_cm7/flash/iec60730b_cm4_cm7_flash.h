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
 * @brief     IEC60730 Class B Flash test safety routine header file for CM4, CM7 cores
 *
 ***********************************************************************************************/

#ifndef _IEC60730B_CM4_CM7_FLASH_
#define _IEC60730B_CM4_CM7_FLASH_

/*******************************************************************************
* Constants
*******************************************************************************/
/* WAS (Write As Seed) bit mask used in HW CRC module */
#define FS_FLASH_CRC_CTRL_WAS_MASK  0x2000000

#ifndef __ASM__

/*! @brief DCP states and results */
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

/* Channel state */
#define FS_DCP_BUSY       0x00000303U /* DCP channel busy */
#define FS_DCP_AVAILABLE  0x00000304U /* DCP channel available * Global functions
*******************************************************************************/

/*******************************************************************************
*
* brief  FS_CM4_CM7_FLASH_HW16 
*
* param startAddress  - first address of the tested memory
* param size          - size of tested memory
* param moduleAddress - address of CRC module	  
* param crcVal        - start condition seed
*
* return 16-bit crc value of the memory range (CRC-16-CCITT - normal 0x1021)
*		
* remarks This function generate the 16-bit CRC value using hw CRC module /CRC-CCITT. 
*
*******************************************************************************/
uint16_t FS_CM4_CM7_FLASH_HW16(uint32_t startAddressress, uint32_t size, uint32_t moduleAddress, uint16_t crcVal);

/*******************************************************************************
*
* brief  FS_CM4_CM7_FLASH_SW16 
*
* param startAddress  - first address of the tested memory
* param size          - size of tested memory	  
* param moduleAddress - has no effect. just because of compatibility with HW function
* param crcVal        - start condition seed
*
* return 16-bit crc value of the memory range (CRC-16-CCITT - normal 0x1021)
*		
* remarks This function generate the 16-bit CRC value using sw  /CRC-CCITT 
*
*******************************************************************************/
uint16_t FS_CM4_CM7_FLASH_SW16(uint32_t startAddress, uint32_t size, uint32_t moduleAddress, uint16_t crcVal);          

/*******************************************************************************
*
* brief  FS_CM4_CM7_FLASH_SW32
*
* param startAddress  - first address of the tested memory
* param size          - size of tested block of memory	 
* param moduleAddress - has no effect. Just because of compatibility with HW function
* param crcVal        - start condition seed	  
*
* return 32-bit crc value of the memory range (CRC-32/MPEG-2 - 0x04C11DB7)
*		
* remarks This function generate the 32-bit CRC value (CRC-32/MPEG-2). 
*
*******************************************************************************/
uint32_t FS_CM4_CM7_FLASH_SW32(uint32_t startAddress, uint32_t size, uint32_t moduleAddress, uint32_t crcVal); 

/*******************************************************************************
*
* brief  FS_CM4_CM7_FLASH_HW32_DCP 
*
* param startAddress - first address of the tested memory
* param size         - size of tested block of memory	 
* param crcVal       - start condition seed	  
* param channel      - DCP channel used for calculation
* param psDCPState   - state & result struct of each DCP channel
* param tag          - differentiates calculation on the same channel
*
* return 32-bit crc value of the memory range (CRC-32/MPEG-2 - 0x04C11DB7)
*		
* remarks This function generate the 32-bit CRC value using hw DCP module CRC-32/MPEG-2. 
*
*******************************************************************************/
void FS_CM4_CM7_FLASH_HW32_DCP(uint32_t startAddress, uint32_t size, uint32_t crcVal,
                               fs_flash_dcp_channels_t channel, fs_flash_dcp_state_t *psDCPState, 
                               uint32_t tag);

#endif /* __ASM__ */

#endif /* _IEC60730B_CM4_CM7_FLASH_ */
