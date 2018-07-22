/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
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
#include "board.h"
#include "fsl_sgtl5000.h"
#include "fsl_sai.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SGTL_ON_BOARD_OSCILLATOR_FREQUENCY (24576000)
#define OVER_SAMPLE_RATE (384U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
sai_config_t saiConfig;
sai_transfer_format_t audioFormat;
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
lpi2c_master_handle_t i2cHandle = {0};
#else
i2c_master_handle_t i2cHandle = {{0, 0, kI2C_Write, 0, 0, NULL, 0}, 0, 0, NULL, NULL};
#endif

/*******************************************************************************
* Code
******************************************************************************/
void BOARD_I2C_Init(void)
{
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_config_t i2cConfig = {0};
#else
    i2c_master_config_t i2cConfig = {0};
#endif
    uint32_t i2cSourceClock;
    i2cSourceClock = CLOCK_GetFreq(BOARD_DEMO_I2C_CLKSRC);

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(BOARD_SAI_DEMO_I2C_BASEADDR, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(BOARD_SAI_DEMO_I2C_BASEADDR, &i2cHandle, NULL, NULL);
#else
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(BOARD_SAI_DEMO_I2C_BASEADDR, &i2cConfig, i2cSourceClock);
    I2C_MasterTransferCreateHandle(BOARD_SAI_DEMO_I2C_BASEADDR, &i2cHandle, NULL, NULL);
#endif
}

void BOARD_Config_Audio_Formats(void)
{
    /* Configure the audio audioFormat */
    audioFormat.bitWidth = kSAI_WordWidth16bits;
    audioFormat.channel = 0U;
    audioFormat.sampleRate_Hz = kSAI_SampleRate16KHz;

    audioFormat.masterClockHz = SGTL_ON_BOARD_OSCILLATOR_FREQUENCY;
    audioFormat.protocol = saiConfig.protocol;
    audioFormat.stereo = kSAI_Stereo;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    audioFormat.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif
}

void BOARD_SAI_TxInit(void)
{
    SAI_TxGetDefaultConfig(&saiConfig);

    saiConfig.masterSlave = kSAI_Slave;
#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    saiConfig.mclkOutputEnable = false;
#endif

    SAI_TxInit(BOARD_DEMO_SAI, &saiConfig);
    BOARD_Config_Audio_Formats();
}

void BOARD_Codec_Init(void)
{
    sgtl_handle_t codecHandle = {0};

    /* Configure Sgtl5000 I2C */
    codecHandle.base = BOARD_SAI_DEMO_I2C_BASEADDR;
    codecHandle.i2cHandle = &i2cHandle;

    sgtl_config_t codecConfig;
    codecConfig.bus = kSGTL_BusLeftJustified;
    codecConfig.master_slave = true;
    codecConfig.route = kSGTL_RoutePlayback;
    SGTL_Init(&codecHandle, &codecConfig);
    /* Configure codec audioFormat */
    SGTL_ConfigDataFormat(&codecHandle, audioFormat.masterClockHz, audioFormat.sampleRate_Hz, audioFormat.bitWidth);
}

void BOARD_SAI_TransferTxSetFormat(void)
{
    uint32_t mclkSourceClockHz = 0U;
    mclkSourceClockHz = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    SAI_TxSetFormat(BOARD_DEMO_SAI, &audioFormat, mclkSourceClockHz, audioFormat.masterClockHz);
}
