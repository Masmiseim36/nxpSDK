/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is the header file for the Eeprom driver.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _EEPROM_H_
#define _EEPROM_H_

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "board.h"

#ifdef __cplusplus
    extern "C" {
#endif

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */


/* List of the EEPROM devices used on each of the FSL development boards */
#define gEepromDevice_None_c          0
#define gEepromDevice_InternalFlash_c 1
#define gEepromDevice_AT45DB161E_c    2 /* TWR-KW2x  */
#define gEepromDevice_AT26DF081A_c    3 /* TWR-MEM   */
#define gEepromDevice_AT45DB021E_c    4 /* FRDM-KW40 */
#define gEepromDevice_AT45DB041E_c    5 /* FRDM-KW41 */
#define gEepromDevice_MX25R3235F_c    6 /* FRDM-K32W */
#define gEepromDevice_MX25R2035F_c    7 /* QN908x-DK1.2 */

/* Default EEPROM type used */
#ifndef gEepromType_d
#define gEepromType_d gEepromDevice_None_c
#endif

/* Characteristics of the EEPROM device */
#if (gEepromType_d == gEepromDevice_AT26DF081A_c)
  #define gEepromParams_TotalSize_c           0x00100000U /* 1 MBytes */
  #define gEepromParams_SectorSize_c          (4*1024)    /* 4 KBytes */
  
#elif (gEepromType_d == gEepromDevice_AT45DB161E_c)
  #define gEepromParams_TotalSize_c           0x00200000U /* 2 MBytes */
  #define gEepromParams_SectorSize_c          (4*1024)    /* 4 KBytes */
  
#elif (gEepromType_d == gEepromDevice_AT45DB021E_c)
  #define gEepromParams_TotalSize_c           0x00040000U /* 256 KBytes */
  #define gEepromParams_SectorSize_c          (2*1024)    /* 2 KBytes */
  
#elif (gEepromType_d == gEepromDevice_AT45DB041E_c)
  #define gEepromParams_TotalSize_c           0x00080000U /* 512 KBytes */
  #define gEepromParams_SectorSize_c          (2*1024)    /* 2 KBytes */
  
#elif (gEepromType_d == gEepromDevice_MX25R3235F_c)
  #define gEepromParams_TotalSize_c           0x00400000U /* 4 MBytes */
  #define gEepromParams_SectorSize_c          (4*1024)    /* 4 KBytes */
  
#elif (gEepromType_d == gEepromDevice_MX25R2035F_c)  
  #define gEepromParams_TotalSize_c           0x00040000U /* 256 KBytes */
  #define gEepromParams_SectorSize_c          (4*1024)    /* 4 KBytes */

#elif (gEepromType_d == gEepromDevice_InternalFlash_c)

#if defined(__CC_ARM)

  extern uint32_t Image$$INT_STORAGE$$Base[];
  extern uint32_t Image$$INT_STORAGE$$Length[];

  #define gEepromParams_StartOffset_c          ((uint32_t)Image$$INT_STORAGE$$Base)
  #define gEepromParams_TotalSize_c            ((uint32_t)Image$$INT_STORAGE$$Length)
  #define gEepromParams_SectorSize_c           ((uint32_t)FSL_FEATURE_FLASH_PAGE_SIZE_BYTES)

#else /* defined(__CC_ARM) */

  extern uint32_t INT_STORAGE_SIZE[];
  extern uint32_t INT_STORAGE_START[];
  extern uint32_t INT_STORAGE_SECTOR_SIZE[];
  #define gEepromParams_StartOffset_c          ((uint32_t)INT_STORAGE_START)
  #define gEepromParams_TotalSize_c            ((uint32_t)INT_STORAGE_SIZE)
  #define gEepromParams_SectorSize_c           ((uint32_t)INT_STORAGE_SECTOR_SIZE)

#endif /* defined(__CC_ARM) */

#else
  #define gEepromParams_TotalSize_c            0x00000U
  #define gEepromParams_SectorSize_c           0
#endif


#ifndef gEepromParams_StartOffset_c
#define gEepromParams_StartOffset_c            0xFFFFFFFFU /* Not defined */
#endif

#ifndef gEepromParams_WriteAlignment_c
#define gEepromParams_WriteAlignment_c         1 /* bytes */
#endif

#if (gEepromParams_WriteAlignment_c > 1)
#define gEepromAlignAddr_d(x) ((((x)+gEepromParams_WriteAlignment_c-1)/gEepromParams_WriteAlignment_c)*gEepromParams_WriteAlignment_c)
#else
#define gEepromAlignAddr_d(x) (x)
#endif


/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef enum
{
  ee_ok,
  ee_too_big,
  ee_not_aligned,
  ee_busy,
  ee_error
} ee_err_t;


/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */

/******************************************************************************
* NAME: EEPROM_Init
* DESCRIPTION: Initializes the EEPROM peripheral
* PARAMETERS: None
* RETURN: ee_ok - if the EEPROM is initialized successfully
*         ee_error - otherwise
******************************************************************************/
ee_err_t EEPROM_Init
(
  void /*IN: No Input Parameters*/
);


/******************************************************************************
* NAME: EEPROM_ReadData
* DESCRIPTION: Reads the specified number of bytes from EEPROM memory
* PARAMETERS: [IN] NoOfBytes - number of bytes to be read
*             [IN] Addr - EEPROM address to start reading from
*             [OUT] inbuf - a pointer to a memory location where the
*                           data read out from EEPROM will be stored
* RETURN: ee_ok - if the read operation completed successfully
*         ee_too_big - if the provided address is out of range
*         ee_error - otherwise
******************************************************************************/
ee_err_t EEPROM_ReadData
(
  uint16_t NoOfBytes,/* IN: No of bytes to read */
  uint32_t Addr,		 /* IN: EEPROM address to start reading from */
  uint8_t  *inbuf		 /* OUT:Pointer to read buffer */
);

/******************************************************************************
* NAME: EEPROM_WriteData
* DESCRIPTION: Writes the specified number of bytes to EEPROM memory
* PARAMETERS: [IN] NoOfBytes - number of bytes to be written.
*             [IN] Addr - EEPROM address to start writing at.
*             [IN] Outbuf - a pointer to a memory location where the
*                           data to be written is stored.
* RETURN: ee_ok - if the write operation completed successfully
*         ee_too_big - if the provided address is out of range
*         ee_error - if the write operation fails
******************************************************************************/
ee_err_t EEPROM_WriteData
(
  uint32_t  NoOfBytes,/* IN: No of bytes to write */
  uint32_t Addr,		 /* IN: EEPROM address to start writing at. */
  uint8_t  *Outbuf	 /* IN: Pointer to data to write to EEPROM  */
);

/******************************************************************************
* NAME: EEPROM_isBusy
* DESCRIPTION: This function tests if the EEPROM is busy
* PARAMETERS: None
* RETURN: TRUE if EEPROM is busy or FALSE if EEPROM is ready(idle)
******************************************************************************/
uint8_t EEPROM_isBusy
(
  void /*IN: No Input Parameters*/
);


/******************************************************************************
* NAME: EEPROM_EraseBlock
* DESCRIPTION:  This function erase a block of 4Kbytes of memory
* PARAMETERS: None
* RETURN: ee_busy - the EEPROM is busy
*         ee_too_big - the specified address is out of range
*         ee_ok - the block erase operation completed successfully
******************************************************************************/
ee_err_t EEPROM_EraseBlock
(
  uint32_t Addr, /* IN: EEPROM address within the 4Kbyte block to be erased. */
  uint32_t size
);

/******************************************************************************
* NAME: EEPROM_ChipErase
* DESCRIPTION:  This function erase the entire EEPROM memory
* PARAMETERS: None
* RETURN: ee_busy - the EEPROM is busy         
*         ee_ok - the block erase operation completed successfully
******************************************************************************/
ee_err_t EEPROM_ChipErase
(
  void /*IN: No Input Parameters*/
);

#ifdef __cplusplus
}
#endif

#endif /* _EEPROM_H_ */
