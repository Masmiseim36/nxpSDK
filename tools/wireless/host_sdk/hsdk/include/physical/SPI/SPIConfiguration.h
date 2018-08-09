/*
 * \file SPIConfiguration.h
 * This is the header file for the SPIConfiguration module.
 *
 * The Clear BSD License
 * Copyright 2016-2017 NXP
 * All rights reserved.
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

#ifndef __SPI_CONFIGURATION__
#define __SPI_CONFIGURATION__

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include <linux/spi/spidev.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>

#include "hsdkOSCommon.h"
#include "hsdkLogger.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/**
 * @brief Structure to encompass the attributes required for SPI configuration.
 */
typedef struct {
    uint8_t lengthFieldSize;  // the number of bytes the length field has in protocols
    uint8_t transferMode;
    // uint32_t fullTransferMode;
    // uint8_t bitJustification;
    uint8_t bitsPerWord;
    uint32_t maxSpeedHz;
} SPIConfigurationData;

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
SPIConfigurationData *defaultSettingsSPI();
void freeSettingsSPI(SPIConfigurationData *);
void setLengthFieldSize(SPIConfigurationData *, uint8_t);
void setSpeedHzSPI(SPIConfigurationData *, uint32_t);
int initPortSPI(File, SPIConfigurationData *);

#ifdef __cplusplus
}
#endif

#endif
