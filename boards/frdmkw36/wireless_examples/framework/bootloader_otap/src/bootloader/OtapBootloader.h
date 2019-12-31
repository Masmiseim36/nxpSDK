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

#ifdef __cplusplus
    extern "C" {
#endif

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#ifndef gUseOTAPBootloader_d
#define gUseOTAPBootloader_d 0
#endif

#if FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE > 4
#define gEepromAlignAddr_d(x) ((((x)+FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE-1)/FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE)*FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE)
#else
#define gEepromAlignAddr_d(x) (x)
#endif

/* Defines used for implementing the boolean types when working with Flash */
#define gBootInvalidAddress_c          0xFFFFFFFF
#define gBootValueForTRUE_c            0x00
#define gBootValueForFALSE_c           0xFF

/* The value of this define should be in sync with the value from OtaSuport.h */
#define gBootData_StartMarker_Value_c        0xDE, 0xAD, 0xAC, 0xE5

#define gBootData_ImageLength_Offset_c       0x00
#define gBootData_ImageLength_Size_c         0x04
#define gBootData_SectorsBitmap_Offset_c     gEepromAlignAddr_d(gBootData_ImageLength_Offset_c + \
                                                                gBootData_ImageLength_Size_c)

#define gBootData_Image_Offset_c             gEepromAlignAddr_d(gBootData_SectorsBitmap_Offset_c + \
                                                                gBootData_SectorsBitmap_Size_c)

#if defined(MCU_MK21DX256)
#define gBootFlexRAMBaseAddress_c    0x14000000
#define gBootFlexRam_4K_Size_c       0x1000
#define gBootFlexRam_2K_Size_c       0x800
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */


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
