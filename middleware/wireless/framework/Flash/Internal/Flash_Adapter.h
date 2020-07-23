/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef __FLASH_ADAPTER_H__
#define __FLASH_ADAPTER_H__

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "fsl_flash.h"


/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
/*
 * Name: gNvDisableIntCmdSeq_c
 * Description: this macro is used to enable/disable interrupts when the
 *              FTFL controller executes a command sequence. This macro
 *              has to be set according to NVM configuration. Therefore,
 *              if the FLASH region used by the NVM is placed in the same
 *              program block as the ISR's executable code, the interrupts
 *              MUST be disabled (because no code pre-fetching can be performed
 *              while FTFL controller executes a command sequence, i.e.
 *              program/erase). If the interrupts are not disabled, the
 *              system will assert a hard fault when the flash controller
 *              executes a command sequnce and an IRQ is about to be handled.
 *              If the NVM region is placed in a different program block than
 *              the ISR's code, this macro shall be set to FALSE (recommended).
 */
#ifndef gNvDisableIntCmdSeq_c
#define gNvDisableIntCmdSeq_c           (1)
#endif

/*
 * Name: gNvPauseOtherCoreInRAM_c
 * Description: this macro is used to enable/disable flash access from the other
 *              core in a multi-core system. This macro has to be set when the
 *              flash is shared between two cores runing at the same time.
 *              FTFL controller executes a command sequence. This macro
 *              has to be set according to NVM configuration. Therefore,
 *              if the FLASH region used by the NVM of one core is placed in the
 *              same flash where the other core is pre-fetching instructions
 *              this will cause core lock up.
 *              If the NVM region is placed in a different program block than
 *              the other core's code, this macro shall be set to FALSE (
 *              recommended).
 *              When this macro is enabled the function NV_BlackboxEnterRam must
 *              be implemented in the MULTICORE_APPLICATION_CORE to call the same function
 *              available in MULTICORE_CONNECTIVITY_CORE.
 */
#ifndef gNvPauseOtherCoreInRAM_c
#define gNvPauseOtherCoreInRAM_c        (0)
#endif

/* size of array to copy__Launch_Command function to.*/
/* It should be at least equal to actual size of __Launch_Command func */
/* User can change this value based on RAM size availability and actual size of __Launch_Command function */
#define LAUNCH_CMD_SIZE         64U
#define PGM_SIZE_BYTE           FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE
#define P_BLOCK_NUM             FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT
#define P_SECTOR_SIZE           FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE
/* Data Flash block information */
#define FLEXNVM_BASE            FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS
#define FLEXNVM_SECTOR_SIZE     FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE
#define FLEXNVM_BLOCK_SIZE      FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE
#define FLEXNVM_BLOCK_NUM       FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT
/* Other defines */
#define DEBUGENABLE             0x00U
#define FTFx_REG_BASE           0x40020000U
#define P_FLASH_BASE            0x00000000U

/* Flex Ram block information */
#define EERAM_BASE              FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS
#define EERAM_SIZE              FSL_FEATURE_FLASH_FLEX_RAM_SIZE

#define READ_NORMAL_MARGIN        0x00U
#define READ_USER_MARGIN          0x01U
#define READ_FACTORY_MARGIN       0x02U

#define NV_FlashRead(pSrc, pDest, size) FLib_MemCpy((void*)(pDest), (void*)(pSrc), size);

#ifdef CPU_QN908X
#define kStatus_FLASH_AlignmentError  MAKE_STATUS(kStatusGroup_FLASH, 6)
#endif

/* Flash address of the Product Data sector */
#if defined(__CC_ARM)
extern uint32_t Load$$LR$$LR_Prod_Data$$Base[];
#define FREESCALE_PROD_DATA_BASE_ADDR Load$$LR$$LR_Prod_Data$$Base
#else
extern uint32_t FREESCALE_PROD_DATA_BASE_ADDR[];
#endif /* defined(__CC_ARM) */

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef PACKED_STRUCT hardwareParameters_tag
{
    uint8_t  identificationWord[10];   /* valid data present */
    uint8_t  reserved[32];             /* for backward compatibillity */
    uint8_t  ieee_802_15_4_address[8]; /* IEEE 802.15.4 MAC address */
    uint8_t  bluetooth_address[6];     /* Bluetooth address */
    uint32_t xtalTrim;                 /* XTAL Trim value */
    uint32_t edCalibrationOffset;      /* KW01 ED offset */
    uint32_t pllFStepOffset;           /* KW01 fine tune pll */
    uint32_t rngSeed;                  /* KW38 RNG Seed Storage for faster boot */
    uint32_t xcvrCal[13];              /* KW38 XCVR Calibration storage for faster Radio init , should be at least sizeof xcvr_DcocDacTrim_t */
    uint8_t  rssiAdjustment;           /* KW38 Rssi Adjustment */
    /* For forward compatibility additional fields may be added here
       Existing data in flash will not be compatible after modifying the hardwareParameters_t typedef*/
    uint16_t hardwareParamsCrc;        /* crc for data between start of reserved area and start of hardwareParamsCrc field (not included). */
}hardwareParameters_t;

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern flash_config_t gFlashConfig;
#if ((defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM)) && (FSL_FEATURE_FLASH_HAS_FLEX_NVM > 0U))
extern flexnvm_config_t gFlexNvmDriver;
#endif/* FSL_FEATURE_FLASH_HAS_FLEX_NVM */

extern hardwareParameters_t gHardwareParameters;

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
void NV_Init(void);

uint32_t NV_FlashProgram(         uint32_t dest,
                                  uint32_t size,
                                  uint8_t* pData);

uint32_t NV_FlashProgramUnaligned(uint32_t dest,
                                  uint32_t size,
                                  uint8_t* pData);

uint32_t NV_FlashEraseSector(     uint32_t dest,
                                  uint32_t size);

uint32_t NV_FlashVerifyErase ( uint32_t start, uint32_t lengthInBytes
#ifndef CPU_QN908X
, ftfx_margin_value_t margin
#endif
);

uint32_t NV_ReadHWParameters(hardwareParameters_t *pHwParams);

uint32_t NV_WriteHWParameters(hardwareParameters_t *pHwParams);

void NV_Flash_SetCriticalSection(void);
void NV_Flash_ClearCriticalSection(void);

#if gNvPauseOtherCoreInRAM_c
/*! *********************************************************************************
 * \brief  Enter a loop with interrupts disabled, executed from RAM
********************************************************************************** */
void NV_BlackboxEnterRam();
#endif /* gNvPauseOtherCoreInRAM_c*/

#endif /* __FLASH_ADAPTER_H__ */
