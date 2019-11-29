/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _FLASH_KINETIS_H
#define _FLASH_KINETIS_H


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "main.h"

#ifdef __cplusplus
    extern "C" {
#endif

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
//  Flash hardware algorithm operation commands
#define FLASH_PROGRAM_BYTE        0x04
#define FLASH_PROGRAM_WORD        0x05
#define FLASH_PROGRAM_LONGWORD    0x06
#define FLASH_PROGRAM_PHRASE      0x07
#define FLASH_ERASE_BLOCK         0x08
#define FLASH_ERASE_SECTOR        0x09
#define FLASH_PROGRAM_SECTION     0x0B
#define FLASH_ERASE_ALL_BLOCK     0x44
#define FLASH_VERIFY_BACKDOOR_KEY 0x45
#define FLASH_SET_EERAM           0x81
#define FLASH_READ_RESOURCE       0x03

#define FLASH_LONG_WORD_SIZE     4
#define FLASH_PHASE_SIZE         8
#define FLASH_DOUBLE_PHASE_SIZE 16

#define FCCOB_REGS  12
#define FLASH_OK     0
#define FLASH_FAIL   1

#define FLASH_DFLASH_IFR_READRESOURCE_ADDRESS   0x8000FC
        
#define ERASE_MASS_INDEX       4
#define ERASE_BLOCK_INDEX      4
#define PROGRAM_BYTE_INDEX     5
#define PROGRAM_SECTION_INDEX  8
#define PROGRAM_LONGWORD_INDEX 8
#define SET_EERAM_INDEX        4
#define VERIFY_KEY_INDEX       11

#define FLASH_FLEXRAM_FNC_CTRL_CODE    0x00

/* Default Flash program size (bytes) */
#ifndef FLASH_PGM_SIZE
#define FLASH_PGM_SIZE (4)
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
//  FCOOB register structure
typedef union
{
  uint8_t all[FCCOB_REGS];
  struct
  {
    uint8_t fccob3;
    uint8_t fccob2;
    uint8_t fccob1;
    uint8_t fccob0;
    uint8_t fccob7;
    uint8_t fccob6;
    uint8_t fccob5;
    uint8_t fccob4;
    uint8_t fccobB;
    uint8_t fccobA;
    uint8_t fccob9;
    uint8_t fccob8;
  }regs;

  struct
  {
    uint32_t fccob3210;
    uint32_t fccob7654;
    uint32_t fccobBA98;
  }regsLong;
}FCC0B_STR;


/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */

//  API FUNCTION FOR KINETIS FLASH DRIVER
void FlashInitialization(void);
uint8_t FLASH_Boot_EraseSector(uint32_t destination);
uint8_t FLASH_Boot_Program(uint32_t dest, uint32_t source, uint32_t size);
uint8_t FLASH_Boot_ProgramByte(uint32_t destination, uint8_t data8b);
uint8_t FLASH_Boot_ProgramLongWords(uint32_t destination, uint32_t source, uint32_t size);
uint8_t FLASH_Boot_ProgramLongWord(uint32_t destination, uint32_t data);
uint8_t FLASH_Boot_ProgramSection(uint32_t destination, uint32_t* pSource, uint32_t size);
uint8_t FLASH_Boot_ProgramPreloadedSection(uint32_t destination, uint32_t size);
uint8_t FLASH_Boot_ProgramSectionPhrases(uint32_t destination, uint32_t* pSource, uint32_t size);
uint8_t FLASH_Boot_ProgramSectionDoublePhrases(uint32_t destination, uint32_t* pSource, uint32_t size);
uint8_t FLASH_Boot_MassErase(void);
uint8_t FLASH_Boot_SetEEEEnable(uint8_t ctrl_code);
uint8_t FLASH_Boot_VerifyBackdoorKey(uint64_t key);

#ifdef __cplusplus
}
#endif

#endif
