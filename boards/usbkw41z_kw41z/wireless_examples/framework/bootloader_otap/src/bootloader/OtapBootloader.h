/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* OTAP Bootloader interface file
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
#define gBootloaderVerPatch_c   3
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
 * Description: The flasg are located after the interrupt vector!
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

#if FLASH_PGM_SIZE > 4
#define gBootFlagSize_c FLASH_PGM_SIZE
#else
#define gBootFlagSize_c 1
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
  uint8_t  newBootImageAvailable[gBootFlagSize_c];
  uint8_t  bootProcessCompleted[gBootFlagSize_c];
  //uint8_t  bootVersion[2];
}bootInfo_t;

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
