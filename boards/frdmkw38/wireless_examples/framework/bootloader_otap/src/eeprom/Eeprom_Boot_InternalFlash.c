/*! *********************************************************************************
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This is a source file which implements the driver for the internal storage memory.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "Eeprom_Boot.h"
#include "FunctionLib.h"
#include "fsl_device_registers.h"

#if gEepromType_d == gEepromDevice_InternalFlash_c

/*****************************************************************************
*  EEPROM_ReadData
*
*  Reads a data buffer from EEPROM, from a given address
*
*****************************************************************************/
ee_err_t EEPROM_ReadData(uint16_t NoOfBytes, uint32_t Addr, uint8_t *inbuf)
{
    FLib_MemCpy(inbuf, (void*)((FSL_FEATURE_FLASH_PFLASH_START_ADDRESS +FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE * FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT) + Addr), NoOfBytes);

    return ee_ok;
}
#endif /* gEepromType_d == gEepromDevice_InternalFlash_c */
