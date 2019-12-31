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
#define gEepromDevice_AT45DB161E_c    2 /* TWR-KW2x */
#define gEepromDevice_AT26DF081A_c    3 /* TWR-MEM */
#define gEepromDevice_AT45DB021E_c    4 /* TWR-KW40*/
#define gEepromDevice_AT45DB041E_c    5 /* TWR-KW41*/

/* Default EEPROM type used */
#ifndef gEepromType_d
#define gEepromType_d gEepromDevice_AT45DB161E_c
#endif

/* Characteristics of the EEPROM device */
#if (gEepromType_d == gEepromDevice_AT26DF081A_c)
  #define gEepromParams_TotalSize_c           0x00100000 /* 1 MBytes */
  
#elif (gEepromType_d == gEepromDevice_AT45DB161E_c)
  #define gEepromParams_TotalSize_c           0x00200000 /* 2 MBytes */
  
#elif (gEepromType_d == gEepromDevice_AT45DB021E_c)
  #define gEepromParams_TotalSize_c           0x00040000 /* 256 KBytes */

#elif (gEepromType_d == gEepromDevice_AT45DB021E_c)
  #define gEepromParams_TotalSize_c           0x00080000 /* 512 KBytes */
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
#if gEepromType_d != gEepromDevice_InternalFlash_c
/******************************************************************************
*  This function initialises the port pins and the EEPROM
*
* Interface assumptions:
*
*
* Return value: ee_err_t
*
******************************************************************************/
ee_err_t EEPROM_Init
(
  void /*IN: No Input Parameters*/
);


/******************************************************************************
*  This function Reads x bytes from the EEPROM
*
* Interface assumptions:
*
*
* Return value: ee_err_t
*
******************************************************************************/
ee_err_t EEPROM_ReadData
(
  uint16_t NoOfBytes,/* IN: No of bytes to read */
  uint32_t Addr,		 /* IN: EEPROM address to start reading from */
  uint8_t  *inbuf		 /* OUT:Pointer to read buffer */
);

/******************************************************************************
*  This function writes x bytes to the EEPROM
*
* Interface assumptions:
*
*
* Return value: ee_err_t
*
******************************************************************************/
ee_err_t EEPROM_WriteData
(
  uint32_t  NoOfBytes,/* IN: No of bytes to write */
  uint32_t Addr,		 /* IN: EEPROM address to start writing at. */
  uint8_t  *Outbuf	 /* IN: Pointer to data to write to EEPROM  */
);

/******************************************************************************
*  This function tests if the EEPROM is busy.
*
* Interface assumptions:
*
*
* Return value: TRUE if EEPROM is busy or FALSE if EEPROM is ready(idle)
*
******************************************************************************/
uint8_t EEPROM_isBusy
(
  void /*IN: No Input Parameters*/
);


/******************************************************************************
*  This function erase 4Kbytes of memory
*
* Interface assumptions:
*
*
* Return value: ee_err_t
*
******************************************************************************/
ee_err_t EEPROM_EraseBlock
(
  uint32_t Addr, /* IN: EEPROM address within the 4Kbyte block to be erased. */
  uint32_t size
);


/******************************************************************************
*  This function erase all memory
*
* Interface assumptions:
*
*
* Return value: ee_err_t
*
******************************************************************************/
ee_err_t EEPROM_ChipErase
(
  void /*IN: No Input Parameters*/
);
#else
  #define EEPROM_Init() 0
  #define EEPROM_ReadData(NoOfBytes,Addr,outbuf) 0
#endif

#ifdef __cplusplus
}
#endif

#endif /* _EEPROM_H_ */
