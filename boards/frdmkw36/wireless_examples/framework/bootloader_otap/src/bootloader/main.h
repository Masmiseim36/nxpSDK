/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* Bootloader interface file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef __MAIN_H__
#define __MAIN_H__


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "fsl_device_registers.h"

#ifdef __cplusplus
    extern "C" {
#endif

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
/*
 * Name: gMcuFlashSize_c
 * Description: The size of the internal FLASH
 */
#if defined(FSL_FEATURE_FLASH_HAS_MULTIPLE_FLASH) && (FSL_FEATURE_FLASH_HAS_MULTIPLE_FLASH)
#define gMcuFlashSize_c      ((FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE * FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT) + \
                              (FSL_FEATURE_FLASH_PFLASH_1_BLOCK_SIZE * FSL_FEATURE_FLASH_PFLASH_1_BLOCK_COUNT))

#else
#define gMcuFlashSize_c      (FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE * FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT)
#endif
/*
 * Name: gFlashErasePage_c
 * Description: The minimum amount of FLASH that can be erased
 */

/* For QN9080C */
#ifndef FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE
#define FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE FSL_FEATURE_FLASH_PAGE_SIZE_BYTES
#endif

#define gFlashErasePage_c  FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE

/*
 * Name: gUserFlashStart_d
 * Description: Starting point of the application
 */
#if !(defined (__CC_ARM))
#define gUserFlashStart_d               ((uint32_t)(__region_BOOT_ROM_end__) + 1)
#else
#define gUserFlashStart_d                (uint32_t)(__region_BOOT_ROM_end__)
#endif

/*
 * Name: gBootImageFlagsAddress_c
 * Description: The flasg are located after the interrupt vector!
 */
#define gBootImageFlagsAddress_c        (gUserFlashStart_d + NUMBER_OF_INT_VECTORS*4)

#if FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT == 2
/*
 * Name: gUserFlash2Start_d
 * Description: Start address of the second Flash block
*/
#define gUserFlash2Start_d     USER_FLASH_START_2
/*
 * Name: gMcuFlash2End_c
 * Description: End address of the second Flash block
*/
#define gMcuFlash2End_c        USER_FLASH_END_2
#endif

/*
 * Name: gBootUseFlashProtection_c
 * Description: Enables/Disables the Flash protection settings
 */
#ifndef gBootUseFlashProtection_c
#define gBootUseFlashProtection_c 0
#endif

/*
 * Name: gFlashProtection_c
 * Description: Flash protection configuration
 */
#ifndef gFlashProtection_c
    #if gBootUseFlashProtection_c
        #ifdef CPU_QN9080C
            #define gFlashProtection_c (0x4 | 0x2 | 0x00000000U) /* (SRAM is protected) | (Flash is protected) | (Flash mass erase disallowed) */
        #else
            #define gFlashProtection_c 0xFFFFFFFEU /* The first Flash region is write protected */
        #endif
    #else
        #ifdef CPU_QN9080C
            #define gFlashProtection_c 0x00000001U /* Flash and SRAM read/write by SWD is allowed, Flash mass erase allowed*/
        #else
            #define gFlashProtection_c 0xFFFFFFFFU /* Flash is not write protected */
        #endif
    #endif
#endif

/*
 *  Default settings for the Flash protection 
 */
#if !defined(CPU_QN9080C)
#ifndef gFlashBackDoorKey1_d
#define gFlashBackDoorKey1_d 0xFFFFFFFFU
#endif

#ifndef gFlashBackDoorKey2_d
#define gFlashBackDoorKey2_d 0xFFFFFFFFU
#endif

#ifndef gFlashConfigOptions_d 
#define gFlashConfigOptions_d  0xFFFFFFFEU
#endif

#else /* defined(CPU_QN9080C) */
#ifndef gFlashblock0Lock_d
#define gFlashblock0Lock_d  0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU
#endif

#ifndef gFlashblock1Lock_d
#define gFlashblock1Lock_d  0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU
#endif
#endif /* defined(CPU_QN9080C) */

/* Defines how the bootloader should handle errors */
#ifdef gBootLoaderDebug_c
  #define gHandleBootError_d() while(1);
#else
  #define gHandleBootError_d() Boot_ResetMCU();
#endif

#define gBootFlagInvalid_d (0xFFFFFFFFFFFFFFFFU)

/* Defines the bootloader sector bitmap length */
#define gBootData_SectorsBitmap_Size_c (32)

/* Init value for the mask used to look through each byte in the bitmap that
indicates which Flash sectors are write protected and should not be updated */
#define gBitMaskInit_c (0x01U)

#define NoOfElements(x) (sizeof(x)/sizeof(x[0]))

/* Concatenate macros */
#define QU(x) #x
#define QUH(x) QU(x)

/* Bootloader version  */
#define gBootloaderVerMajor_c   5
#define gBootloaderVerMinor_c   3
#define gBootloaderVerPatch_c   3
#define gBootloaderBuildNo_c    0

#if (gFsciIncluded_c) && (gUseOTAPBootloader_d)
  #define gBootloaderModuleId_c   0xAC
  #define gBootloaderVerString_c "OTAP-FSCI BOOTLOADER v" \
                        QUH(gBootloaderVerMajor_c) "."    \
                        QUH(gBootloaderVerMinor_c) "."    \
                        QUH(gBootloaderVerPatch_c)

#elif (gFsciIncluded_c)
  #define gBootloaderModuleId_c   0xAA
  #define gBootloaderVerString_c "FSCI BOOTLOADER v"   \
                        QUH(gBootloaderVerMajor_c) "." \
                        QUH(gBootloaderVerMinor_c) "." \
                        QUH(gBootloaderVerPatch_c)

#elif (gUseOTAPBootloader_d)
  #define gBootloaderModuleId_c   0xAB
  #define gBootloaderVerString_c "OTAP BOOTLOADER v"   \
                        QUH(gBootloaderVerMajor_c) "." \
                        QUH(gBootloaderVerMinor_c) "." \
                        QUH(gBootloaderVerPatch_c)   
#endif

#define gOtapBootTrigger_c                 (0x07AF8007)
#define gFsciBootTrigger_c                 (0xF5C18007)

#if FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE == 4
    #define gBootFlagDefaultValue_c 0xFF,0xFF,0xFF,0xFF
#elif FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE == 8
    #define gBootFlagDefaultValue_c 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
#else
    #error The gBootFlagDefaultValue_c macro is not defined for this FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE.
#endif


/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/*
 * Name: FlashConfig_t
 * Description: Structure used to access Flash Configuration
 */
#if defined(__CC_ARM)
#pragma push
#pragma anon_unions
#endif /* defined(__CC_ARM) */
typedef union _FlashConfig_t
{
#ifndef CPU_QN9080C
    uint32_t Data[4];
    struct {
        uint32_t BackDoorKey[2];
        uint32_t Protection;
        uint32_t Config;
    };
#else
    uint32_t Data[9];
    struct {
        uint32_t Block0Lock[4];
        uint32_t Block1Lock[4];
        uint32_t Protection; // Defaults to 0x0000_0001 for QN9080C !!!!!
    };
#endif
} FlashConfig_t;
#if defined(__CC_ARM)
#pragma pop
#endif /* defined(__CC_ARM) */

typedef void (*vector_entry)(void);
typedef void pointer(void);

typedef PACKED_STRUCT bootFlags_tag{
    union {
        uint8_t  aNewBootImageAvailable[FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE];
        uint32_t newBootImageAvailable;
    }u0;
    union {
        uint8_t  aBootProcessCompleted[FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE];
        uint32_t bootProcessCompleted;
    }u1;
    uint8_t  version[FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE];
    union {
        uint8_t  aInternalStorageStart[FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE];
        uint32_t internalStorageStart;
    }u2;
}bootFlags_t;

/* module info structure */
typedef PACKED_STRUCT moduleInfo_tag {
  char const moduleTag[4];
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
#if defined(__IAR_SYSTEMS_ICC__)
    extern uint32_t __BOOT_STACK_ADDRESS[];
    extern uint32_t __BOOT_Flags_start__[];
    extern uint32_t __region_BOOT_ROM_end__[];
    extern uint32_t __region_BOOT_ROM_start__[];
    extern uint32_t __ROM_SIZE__[];
#elif defined(__GNUC__)
    extern uint32_t __SP_INIT[];
    extern uint32_t __BOOT_Flags_start__[];
    extern uint32_t __region_BOOT_ROM_end__[];
    extern uint32_t __region_BOOT_ROM_start__[];
    extern uint32_t __ROM_SIZE__[];
#elif defined(__CC_ARM)
    extern uint32_t Image$$BootFlags_region$$Base[];
    extern uint32_t Load$$LR$$ROM_region$$Base[];
    extern uint32_t Load$$LR$$ROM_region$$Limit[];

    #define __BOOT_Flags_start__ ((uint32_t*)Image$$BootFlags_region$$Base)
    #define __region_BOOT_ROM_start__ ((uint32_t*)Load$$LR$$ROM_region$$Base)
    #define __region_BOOT_ROM_end__ ((uint32_t*)Load$$LR$$ROM_region$$Limit)
    #define __ROM_SIZE__ (__region_BOOT_ROM_end__ - __region_BOOT_ROM_start__ + 1)
#endif

extern uint32_t gMcuFlashSize;

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
void defaultISR(void);
void NMI_handler(void);
void ResetISR(void);
int main(int argc, char **argv);
void Boot_ResetMCU(void);
void EnableRamVectorTable(void);
void InstallIrqHandler(IRQn_Type irqType, void (*handler)(void));
void BOOT_ClockInit(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */
