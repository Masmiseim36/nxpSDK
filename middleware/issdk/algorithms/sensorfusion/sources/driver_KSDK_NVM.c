/*
 * The Clear BSD License
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*! \file driver_KSDK_NVM.c
    \brief middleware driver for NVM on Kinetis devices
*/
#include "sensor_fusion.h"
#include "driver_KSDK_NVM.h"

#ifndef CPU_LPC54114J256BD64_cm4
#include "fsl_flash.h"
#else
#include "fsl_flashiap.h"
#endif

#define ERROR 1
#define SUCCESS 0;
#ifndef CPU_LPC54114J256BD64_cm4
byte NVM_SetBlockFlash(uint8_t *Source, uint32_t Dest, uint16_t Count)
{
    byte retVal=0;
    status_t result;
    uint32_t pflashSectorSize = ERROR;
    flash_config_t flashDriver;                                            // Flash driver Structure

    // Clean up Flash driver Structure
    memset(&flashDriver, 0, sizeof(flash_config_t));
    // Setup flash driver structure for device and initialize variables.
    result = FLASH_Init(&flashDriver);
    if (kStatus_FLASH_Success == result)
    {
        FLASH_GetProperty(&flashDriver, FLASH_SECTOR_SIZE_PROPERTY, &pflashSectorSize);
        result = FLASH_Erase(&flashDriver, Dest, pflashSectorSize,  FLASH_ERASE_KEY);

        if (kStatus_FLASH_Success == result) {
            result = FLASH_Program(&flashDriver, Dest, Source, Count);
            if (kStatus_FLASH_Success == result)  retVal=SUCCESS;
        }
    }
    return(retVal);
}
#else
byte checkIAPStatus(status_t result) {
    byte retVal=0;
      switch (result) {
      case kStatus_FLASHIAP_Success:
        retVal = SUCCESS;
        break;
      case kStatus_FLASHIAP_InvalidCommand :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_SrcAddrError :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_DstAddrError :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_SrcAddrNotMapped :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_DstAddrNotMapped :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_CountError :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_InvalidSector :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_NotPrepared :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_CompareError :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_Busy :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_ParamError :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_AddrError :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_AddrNotMapped :
        retVal = ERROR;
        break;
       case kStatus_FLASHIAP_NoPower :
        retVal = ERROR;
        break;
      case kStatus_FLASHIAP_NoClock :
        retVal = ERROR;
        break;
      default:
        break;
      }
      return retVal;
}

byte NVM_SetBlockFlash(uint8_t *Source, uint32_t Dest, uint16_t Count)
{
    byte retVal=ERROR;
    status_t result;
    result = FLASHIAP_PrepareSectorForWrite(NVM_SECTOR_NUMBER, NVM_SECTOR_NUMBER);
    if (result == kStatus_FLASHIAP_Success) {
      result = FLASHIAP_ErasePage(NVM_PAGE_NUMBER, NVM_PAGE_NUMBER, SystemCoreClock);
      if (result == kStatus_FLASHIAP_Success) {
        result = FLASHIAP_PrepareSectorForWrite(NVM_SECTOR_NUMBER, NVM_SECTOR_NUMBER);
        if (result == kStatus_FLASHIAP_Success) {

            result = FLASHIAP_CopyRamToFlash(CALIBRATION_NVM_ADDR, (uint32_t *) Source, (uint32_t) Count, SystemCoreClock);
            //if (result == kStatus_FLASHIAP_Success) retVal = SUCCESS;
            retVal = checkIAPStatus(result);
        }
      }
    }
    return(retVal);
}
#endif
