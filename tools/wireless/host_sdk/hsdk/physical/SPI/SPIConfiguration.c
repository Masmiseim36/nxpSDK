/*
 * \file SPIConfiguration.c
 * This is a source file for the SPIConfiguration module.
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

#include "SPIConfiguration.h"


SPIConfigurationData *defaultSettingsSPI(void)
{
    SPIConfigurationData *config = (SPIConfigurationData *) calloc (1, sizeof(SPIConfigurationData));
    if (config == NULL) {
        logMessage(HSDK_ERROR, "[SPIConfiguration]defaultSettingsSPI", "Memory allocation failed", HSDKThreadId());
        return NULL;
    }

    config->lengthFieldSize = 2;  // Thread, FlexIP, BLE; set to 1 for MAC
    config->transferMode = SPI_MODE_0;
    config->bitsPerWord = 8;
    config->maxSpeedHz = 1000000;  // 1MHz

    return config;
}

void freeSettingsSPI(SPIConfigurationData *config)
{
    if (config != NULL) {
        free(config);
    }
}

void setLengthFieldSize(SPIConfigurationData *config, uint8_t lengthFieldSize)
{
    config->lengthFieldSize = lengthFieldSize;
}

void setSpeedHzSPI(SPIConfigurationData *config, uint32_t maxSpeedHz)
{
    config->maxSpeedHz = maxSpeedHz;
}


/*! *********************************************************************************
* \brief  Initialize the SPI device with the configuration attributes.
*
* \param[in] SPI device file descriptor
* \param[in] configuration structure holding the attributes
*
* \return 0 in case of success, -1 otherwise
********************************************************************************** */
int initPortSPI(File portHandle, SPIConfigurationData *config)
{
    int ret = 0;

    /*
     * SPI mode
     */
    ret = ioctl(portHandle, SPI_IOC_WR_MODE, &config->transferMode);
    if (ret == -1)
        perror("can't set spi mode");

    ret = ioctl(portHandle, SPI_IOC_RD_MODE, &config->transferMode);
    if (ret == -1)
        perror("can't get spi mode");

    /*
     * bits per word
     */
    ret = ioctl(portHandle, SPI_IOC_WR_BITS_PER_WORD, &config->bitsPerWord);
    if (ret == -1)
        perror("can't set bits per word");

    ret = ioctl(portHandle, SPI_IOC_RD_BITS_PER_WORD, &config->bitsPerWord);
    if (ret == -1)
        perror("can't get bits per word");

    /*
     * max speed in Hz
     */
    ret = ioctl(portHandle, SPI_IOC_WR_MAX_SPEED_HZ, &config->maxSpeedHz);
    if (ret == -1)
        perror("can't set max speed hz");

    ret = ioctl(portHandle, SPI_IOC_RD_MAX_SPEED_HZ, &config->maxSpeedHz);
    if (ret == -1)
        perror("can't get max speed hz");

#ifdef DEBUG
    printf("spi mode: %d\n", config->transferMode);
    printf("bits per word: %d\n", config->bitsPerWord);
    printf("max speed: %d Hz (%d KHz)\n", config->maxSpeedHz, config->maxSpeedHz / 1000);
#endif

    return 0;
}
