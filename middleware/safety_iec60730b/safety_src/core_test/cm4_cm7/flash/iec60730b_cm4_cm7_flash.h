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


/************************************************************************************************
*
* @file      IEC60730_B_CM4_CM7_flash.h
*
* @author    b48632
* 
* @version   3.0
*
* @date      Nov-01-2018
* 
* @brief     IEC60730 Class B Flash test safety routine header file for CM4, CM7 cores
*
*******************************************************************************/
#ifndef _IEC60730_B_CM4_CM7_FLASH_H_
#define _IEC60730_B_CM4_CM7_FLASH_H_


/*******************************************************************************
* Constants
*******************************************************************************/
/* Registers address and masks for hardware CRC module. */

#define FLSH_REG_CRC_CTRL_WAS_MASK      0x2000000            /* Write CRC data register as seed. */

#ifndef __ASM__

/*! @brief DCP states and results */
typedef struct _flash_dcp_state
{
 unsigned long ui32Iec60730bCH0State;
 unsigned long ui32Iec60730bCH0Result;
 unsigned long ui32Iec60730bCH0Packet[8];
 unsigned long ui32Iec60730bCH1State;
 unsigned long ui32Iec60730bCH1Result;
 unsigned long ui32Iec60730bCH1Packet[8];
 unsigned long ui32Iec60730bCH2State;
 unsigned long ui32Iec60730bCH2Result;
 unsigned long ui32Iec60730bCH2Packet[8];
 unsigned long ui32Iec60730bCH3State;
 unsigned long ui32Iec60730bCH3Result;
 unsigned long ui32Iec60730bCH3Packet[8];
} flash_dcp_state_t;

/* DCP channels */
typedef enum dcp_channels
{ 
    DCP_CH0 = (unsigned long)(1u << 16),
    DCP_CH1 = (unsigned long)(1u << 17),
    DCP_CH2 = (unsigned long)(1u << 18),
    DCP_CH3 = (unsigned long)(1u << 19)
} dcp_channels_t;

/* Channel state */
#define IEC60730B_ST_DCP_BUSY        0x00000303 /* DCP channel busy */
#define IEC60730B_ST_DCP_AVAILABLE   0x00000304 /* DCP channel available */

/*******************************************************************************
* Global functions
*******************************************************************************/

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_Flash_HWTest 
*
* param startAdd      - first address of the tested memory
* param size          - size of tested memory
* param moduleAddress - address of CRC module	  
* param crcVal        - start condition seed
*
* return 16-bit crc value of the memory range (CRC-16-CCITT - normal 0x1021)
*		
* remarks This function generate the 16-bit CRC value using hw CRC module /CRC-CCITT. 
*
*******************************************************************************/
unsigned short IEC60730B_CM4_CM7_Flash_HWTest(const unsigned int startAdd, const unsigned int size, unsigned int moduleAddress, const unsigned short crcVal);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_Flash_SWTest 
*
* param startAdd      - first address of the tested memory
* param size          - size of tested memory	  
* param moduleAddress - has no effect. just because of compatibility with HW function
* param crcVal        - start condition seed
*
* return 16-bit crc value of the memory range (CRC-16-CCITT - normal 0x1021)
*		
* remarks This function generate the 16-bit CRC value using sw  /CRC-CCITT 
*
*******************************************************************************/
unsigned short IEC60730B_CM4_CM7_Flash_SWTest(const unsigned int startAdd, const unsigned int size, unsigned int moduleAddress, const unsigned short crcVal);          

/*******************************************************************************
*
* brief  IEC60730B_CM7_Flash_SWTest_32
*
* param startAdd      - first address of the tested memory
* param size          - size of tested block of memory	 
* param moduleAddress - has no effect. Just because of compatibility with HW function
* param crcVal        - start condition seed	  
*
* return 32-bit crc value of the memory range (CRC-32/MPEG-2 - 0x04C11DB7)
*		
* remarks This function generate the 32-bit CRC value (CRC-32/MPEG-2). 
*
*******************************************************************************/
unsigned long IEC60730B_CM4_CM7_Flash_SWTest_32(const unsigned long startAdd, const unsigned long size, unsigned long moduleAddress, const unsigned long crcVal); 

/*******************************************************************************
*
* brief  IEC60730B_CM7_Flash_HWTest_DCP 
*
* param startAdd   - first address of the tested memory
* param size       - size of tested block of memory	 
* param crcVal     - start condition seed	  
* param channel    - DCP channel used for calculation
* param psDCPState - state & result struct of each DCP channel
* param tag        - differentiates calculation on the same channel
*
* return 32-bit crc value of the memory range (CRC-32/MPEG-2 - 0x04C11DB7)
*		
* remarks This function generate the 32-bit CRC value using hw DCP module CRC-32/MPEG-2. 
*
*******************************************************************************/
void IEC60730B_CM7_Flash_HWTest_DCP(const unsigned long startAdd,  const unsigned long size, 
                                    const unsigned long crcVal,    const dcp_channels_t channel,
                                    flash_dcp_state_t *psDCPState, const unsigned long tag);

#endif
	
	
#endif /* _IEC60730_B_CM4_CM7_FLASH_H */

