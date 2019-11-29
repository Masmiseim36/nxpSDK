/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* OTAP Bootloader interface file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef __OTAPBOOTLOADER_H__
#define __OTAPBOOTLOADER_H__


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "Eeprom_Boot.h"
#include "main.h"
#include "flash_boot_kinetis.h"

#ifdef __cplusplus
    extern "C" {
#endif

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

/* Concatenate macros */
#define QU(x) #x
#define QUH(x) QU(x)

/* Bootloader version  */
#define gBootloaderVerMajor_c   5
#define gBootloaderVerMinor_c   3
#define gBootloaderVerPatch_c   7
#define gBootloaderBuildNo_c    0
#define gBootloaderModuleId_c   0xAB

#define gBootloaderVerString_c "OTAP BOOTLOADER v" \
                        QUH(gBootloaderVerMajor_c) "." \
                        QUH(gBootloaderVerMinor_c) "." \
                        QUH(gBootloaderVerPatch_c)

#ifndef gUseOTAPBootloader_d
#define gUseOTAPBootloader_d 0
#endif

#if FLASH_PGM_SIZE > 4
#define gEepromAlignAddr_d(x) ((((x)+FLASH_PGM_SIZE-1)/FLASH_PGM_SIZE)*FLASH_PGM_SIZE)
#else
#define gEepromAlignAddr_d(x) (x)
#endif

/*
 * Name: gBootImageFlagsAddress_c
 * Description: The flags are located after the interrupt vector!
 */
#define gBootImageFlagsAddress_c        (gUserFlashStart_d + VECTOR_TABLE_SIZE*4)

/*
 * Name: gBootProductInfoAddress_c
 * Description: Address of the ProductInfo
 *              The Product info is located in the last sector of the FLASH
 */
#define gBootProductInfoAddress_c      (gMcuFlashSize_c - gFlashErasePage_c)

/*
 * Name: gInternalStorageStartAddress_c
 * Description: The start address of the internal storage
 */
#define gInternalStorageStartAddressOffset_c (0x44)

/* Defines used for implementing the boolean types when working with Flash */
#define gBootInvalidAddress_c          0xFFFFFFFF
#define gBootValueForTRUE_c            0x00
#define gBootValueForFALSE_c           0xFF

#define gBootData_ImageLength_Offset_c       0x00
#define gBootData_ImageLength_Size_c         0x04
#define gBootData_SectorsBitmap_Offset_c     gEepromAlignAddr_d(gBootData_ImageLength_Offset_c + \
                                                                gBootData_ImageLength_Size_c)
#define gBootData_SectorsBitmap_Size_c       (32)
#define gBootData_Image_Offset_c             gEepromAlignAddr_d(gBootData_SectorsBitmap_Offset_c + \
                                                                gBootData_SectorsBitmap_Size_c)

#if defined(MCU_MK21DX256)
#define gBootFlexRAMBaseAddress_c    0x14000000
#define gBootFlexRam_4K_Size_c       0x1000
#define gBootFlexRam_2K_Size_c       0x800
#endif

#ifndef gBootFlagSize_c
#define gBootFlagSize_c    FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/*
 * Name: bootFlags_t
 * Description: Structure used to inform the Bootloader if a new Image needs
 *              to be loaded.
 */
typedef PACKED_STRUCT
{
  union {
        uint8_t  aNewBootImageAvailable[gBootFlagSize_c];
        uint32_t newBootImageAvailable;
    }u0;
    union {
        uint8_t  aBootProcessCompleted[gBootFlagSize_c];
        uint32_t bootProcessCompleted;
    }u1;

    uint8_t  aBootVersion[gBootFlagSize_c];

    union {
        uint8_t  aInternalStorageStart[gBootFlagSize_c];
        uint32_t internalStorageStart;
    }u2;
} bootInfo_t;

/* module info structure */
typedef PACKED_STRUCT moduleInfo_tag {
  char moduleTag[4];
  const char* const moduleString;
  uint8_t  moduleId;
  uint8_t  versionNumber[3];
  uint16_t buildNumber;
  uint8_t  padding[2];
} moduleInfo_t;

/******************************************************************************
 *******************************************************************************
 * Public memory declarations
 *******************************************************************************
 ******************************************************************************/

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
void Boot_CheckOtapFlags(void);

#ifdef __cplusplus
}
#endif

#endif /* __OTAPBOOTLOADER_H__ */
