/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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
 * o Neither the name of the copyright holder nor the names of its
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
#include "fsl_device_registers.h"
#include "fsl_wm8960.h"
#include "fsl_sai.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
#if defined(USING_CODEC_WM8960)
wm8960_handle_t codecHandle = {0};
#elif defined(USING_CODEC_DA7212)
da7212_handle_t codecHandle = {0};
#else
sgtl_handle_t codecHandle = {0};
#endif

/*******************************************************************************
* Code
******************************************************************************/
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
void BOARD_LPI2C_Init(uint32_t boardI2CClock, LPI2C_Type *LPI2CBase)
{
    lpi2c_master_config_t i2cConfig = {0};

    uint32_t i2cSourceClock;
    i2cSourceClock = boardI2CClock;

    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(LPI2CBase, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(LPI2CBase, &i2cHandle, NULL, NULL);
}

void BOARD_Codec_Init(LPI2C_Type *LPI2CBase)
{
    wm8960_handle_t codecHandle = {0};

    /* Configure Sgtl5000 I2C */
    codecHandle.base = LPI2CBase;
    codecHandle.i2cHandle = &i2cHandle;

    WM8960_Init(&codecHandle, NULL);
    WM8960_ConfigDataFormat(&codecHandle, audioFormat.masterClockHz, audioFormat.sampleRate_Hz, audioFormat.bitWidth);
}
#else
void BOARD_I2C_Init(uint32_t boardI2CClock, I2C_Type *I2CBase)
{
    i2c_master_config_t i2cConfig = {0};

    uint32_t i2cSourceClock;
    i2cSourceClock = boardI2CClock;

    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(BOARD_SAI_DEMO_I2C_BASEADDR, &i2cConfig, i2cSourceClock);
    I2C_MasterTransferCreateHandle(BOARD_SAI_DEMO_I2C_BASEADDR, &i2cHandle, NULL, NULL);
}

void BOARD_Codec_Init(I2C_Type *I2CBase)
{
    /* Configure Sgtl5000 I2C */
    codecHandle.base = I2CBase;
    codecHandle.i2cHandle = &i2cHandle;

    WM8960_Init(&codecHandle, NULL);
    WM8960_ConfigDataFormat(&codecHandle, audioFormat.masterClockHz, audioFormat.sampleRate_Hz, audioFormat.bitWidth);
}
#endif

void BOARD_Config_Audio_Formats(uint32_t saiClockFreq)
{
    /* Configure the audio audioFormat */
    audioFormat.bitWidth = kSAI_WordWidth16bits;
    audioFormat.channel = 0U;
    audioFormat.sampleRate_Hz = kSAI_SampleRate16KHz;

    audioFormat.masterClockHz = saiClockFreq;
    audioFormat.protocol = saiConfig.protocol;
    audioFormat.stereo = kSAI_Stereo;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    audioFormat.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif
}

void BOARD_SAI_TxInit(I2S_Type *SAIBase, uint32_t saiClockFreq)
{
    SAI_TxGetDefaultConfig(&saiConfig);

    SAI_TxInit(SAIBase, &saiConfig);
    BOARD_Config_Audio_Formats(saiClockFreq);
}

void BOARD_SAI_TransferTxSetFormat(I2S_Type *SAIBase, uint32_t saiClockFreq)
{
    uint32_t mclkSourceClockHz = 0U;
    mclkSourceClockHz = saiClockFreq;

    SAI_TxSetFormat(SAIBase, &audioFormat, mclkSourceClockHz, audioFormat.masterClockHz);
}
