/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_flash.h"
#include "usb_flash.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief flash configuration */
static flash_config_t dfuFlashConfig;
static ftfx_cache_config_t s_cacheDriver;
uint32_t pflashBlockBase = 0;
uint32_t pflashTotalSize = 0;
uint32_t pflashSectorSize = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief USB Memmory initialization function.
 *
 * This function initializes the Memmory driver structure and variables.
 *
 * @return A Memmory error or kStatus_Memmory_Success.
 */
usb_memmory_status_t USB_MemmoryInit(void)
{
    usb_memmory_status_t status = kStatus_USB_MemmorySuccess;
    status_t result; 
    ftfx_security_state_t securityStatus = kFTFx_SecurityStateNotSecure;
    /* clean the dfu flash configuration */
    memset(&dfuFlashConfig, 0U, sizeof(flash_config_t));
    /* Initializes the memmory driver */
    if (kStatus_FLASH_Success != FLASH_Init(&dfuFlashConfig))
    {
        status = kStatus_USB_MemmoryErrorUnknown;
        return status;
    }
    result = FTFx_CACHE_Init(&s_cacheDriver);
    if (kStatus_FTFx_Success != result)
    {
        status = kStatus_USB_MemmoryErrorUnknown;
        return status;
    }
     /* Get memmory properties*/
    FLASH_GetProperty(&dfuFlashConfig, kFLASH_PropertyPflash0BlockBaseAddr, &pflashBlockBase);
    FLASH_GetProperty(&dfuFlashConfig, kFLASH_PropertyPflash0TotalSize, &pflashTotalSize);
    FLASH_GetProperty(&dfuFlashConfig, kFLASH_PropertyPflash0SectorSize, &pflashSectorSize);
    /* Security checking */
    if (kStatus_FLASH_Success != FLASH_GetSecurityState(&dfuFlashConfig, &securityStatus))
    {
        status = kStatus_USB_MemmoryErrorUnknown;
        return status;
    }
    if (kFTFx_SecurityStateNotSecure != securityStatus)
    {
        status = kStatus_USB_MemmoryErrorSecure;
        return status;
    }
    return status;
}

/*!
 * @brief USB memmory erasing function.
 *
 * This function erases the memmory area from start address to the end.
 *
 * @param address  The start address.
 * @param address  The erase size.
 *
 * @return A FLASH error or kStatus_FLASH_Success.
 */
usb_memmory_status_t USB_MemmoryErase(uint32_t address, uint32_t size)
{
    usb_memmory_status_t status = kStatus_USB_MemmorySuccess;
    uint32_t eraseLength;
    eraseLength = size;
    FTFx_CACHE_ClearCachePrefetchSpeculation(&s_cacheDriver, true);
    if (kStatus_FLASH_Success != FLASH_Erase(&dfuFlashConfig, address, eraseLength, kFLASH_ApiEraseKey))
    {
        status = kStatus_USB_MemmoryErrorErase;
        return status;
    }
    if (kStatus_FLASH_Success != FLASH_VerifyErase(&dfuFlashConfig, address, eraseLength, kFTFx_MarginValueUser))
    {
        status = kStatus_USB_MemmoryErrorEraseVerify;
        return status;
    }
    FTFx_CACHE_ClearCachePrefetchSpeculation(&s_cacheDriver, false);
    return status;
}

/*!
 * @brief DFU memmory programming function.
 *
 * This function program memmory with data at locations passed in through parameters.
 *
 * @param address The start address to be programmed.
 * @param buffer  Pointer to buffer data.
 * @param length  The length of data in byte.
 *
 * @return A FLASH error or kStatus_FLASH_Success.
 */
usb_memmory_status_t USB_MemmoryProgram(uint32_t address, uint8_t *buffer, uint32_t length)
{
    usb_memmory_status_t status = kStatus_USB_MemmorySuccess;
    int32_t flashStatus = 0U;
    /* Round up to alignment of FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE */
    length = (length + (FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE - 1U)) &
             ~(FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE - 1U);
    FTFx_CACHE_ClearCachePrefetchSpeculation(&s_cacheDriver, true);
    flashStatus = FLASH_Program(&dfuFlashConfig, address, (uint8_t *)buffer, length);
    if (kStatus_FLASH_Success != flashStatus)
    {
        if (kStatus_FTFx_AddressError == flashStatus)
        {
            status = kStatus_USB_MemmoryErrorProgramAddress;
            return status;
        }
        else
        {
            status = kStatus_USB_MemmoryErrorProgram;
            return status;
        }
    }
    flashStatus =
        FLASH_VerifyProgram(&dfuFlashConfig, address, length, (uint8_t *)buffer, kFTFx_MarginValueUser, NULL, NULL);
    if (kStatus_FLASH_Success != flashStatus)
    {
        status = kStatus_USB_MemmoryErrorProgramVerify;
    }
    FTFx_CACHE_ClearCachePrefetchSpeculation(&s_cacheDriver, false);
    return status;
}
