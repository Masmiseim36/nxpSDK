/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "board.h"
#include "fsl_flexio_i2s.h"
#include "fsl_debug_console.h"
#include "fsl_sgtl5000.h"
#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI and I2C instance and clock */
#define DEMO_I2C I2C0
#define DEMO_FLEXIO_BASE FLEXIO0
#define DEMO_I2C_CLKSRC kCLOCK_BusClk
#define DEMO_I2C_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
/* Flexio clock source frequency shall be even times of bit clock */
#define DEMO_FLEXIO_CLKSRC kCLOCK_PllFllSelClk
#define DEMO_FLEXIO_CLK_FREQ CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define BCLK_PIN 4U
#define FRAME_SYNC_PIN 5U
#define TX_DATA_PIN 1U
#define RX_DATA_PIN 0U
#define OVER_SAMPLE_RATE (384)
#define BUFFER_SIZE (128)
#define BUFFER_BLOCK (2)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexio_i2s_handle_t txHandle = {0};
flexio_i2s_handle_t rxHandle = {0};
static uint8_t txBuff[BUFFER_SIZE * BUFFER_BLOCK];
static uint8_t rxBuff[BUFFER_SIZE * BUFFER_BLOCK];
#if defined(DEMO_CODEC_WM8960)
wm8960_handle_t codecHandle = {0};
#elif defined (DEMO_CODEC_DA7212)
da7212_handle_t codecHandle = {0};
#else
sgtl_handle_t codecHandle = {0};
#endif

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
lpi2c_master_handle_t i2cHandle = {0};
#else
i2c_master_handle_t i2cHandle = {{0, 0, kI2C_Write, 0, 0, NULL, 0}, 0, 0, NULL, NULL};
#endif

static volatile uint8_t TxInIndex = 0;
static volatile uint8_t TxOutIndex = 0;
static volatile uint8_t RxInIndex = 0;
static volatile uint8_t RxOutIndex = 0;
static volatile uint8_t full_block = 0;
static volatile uint8_t empty_block = BUFFER_BLOCK;

FLEXIO_I2S_Type base;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void txCallback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_handle_t *handle, status_t status, void *userData)
{
    TxOutIndex = (TxOutIndex + 1) % BUFFER_BLOCK;
    full_block -= 1;
}

static void rxCallback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_handle_t *handle, status_t status, void *userData)
{
    RxInIndex = (RxInIndex + 1) % BUFFER_BLOCK;
    empty_block -= 1;
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexio_i2s_config_t config;
    uint32_t sourceClockHz = 0U;
    flexio_i2s_format_t format;
    flexio_i2s_transfer_t xferTx, xferRx;
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_config_t i2cConfig = {0};
#else
    i2c_master_config_t i2cConfig = {0};
#endif
    uint32_t i2cSourceClock;
    uint32_t i = 0;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetFlexio0Clock(1U);
    CLOCK_SetPllFllSelClock(3, 0, 0);

    PRINTF("FLEXIO_I2S interrupt example started!\n\r");

    /* Set flexio i2s pin, shifter and timer */
    base.bclkPinIndex = BCLK_PIN;
    base.fsPinIndex = FRAME_SYNC_PIN;
    base.txPinIndex = TX_DATA_PIN;
    base.rxPinIndex = RX_DATA_PIN;
    base.txShifterIndex = 0;
    base.rxShifterIndex = 1;
    base.bclkTimerIndex = 0;
    base.fsTimerIndex = 1;
    base.flexioBase = DEMO_FLEXIO_BASE;

#if defined(DEMO_CODEC_WM8960) || defined (DEMO_CODEC_DA7212)
    /* Use SAI to do master clock */
    sai_config_t saiConfig;
    /*
     * saiConfig.masterSlave = kSAI_Master;
     * saiConfig.mclkSource = kSAI_MclkSourceSysclk;
     * saiConfig.protocol = kSAI_BusLeftJustified;
     * saiConfig.syncMode = kSAI_ModeAsync;
     * saiConfig.mclkOutputEnable = true;
     */
    SAI_TxGetDefaultConfig(&saiConfig);
    SAI_TxInit(DEMO_SAI, &saiConfig);

    /* Configure the audio format */
    sai_transfer_format_t saiFormat = {0};
    saiFormat.bitWidth = kSAI_WordWidth16bits;
    saiFormat.channel = 0U;
    saiFormat.sampleRate_Hz = kSAI_SampleRate16KHz;
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    saiFormat.masterClockHz = OVER_SAMPLE_RATE * saiFormat.sampleRate_Hz;
#else
    saiFormat.masterClockHz = DEMO_SAI_CLK_FREQ;
#endif
    saiFormat.protocol = saiConfig.protocol;
    saiFormat.stereo = kSAI_Stereo;

    uint32_t masterClockSrc = DEMO_SAI_CLK_FREQ;
    SAI_TxSetFormat(DEMO_SAI, &saiFormat, masterClockSrc, saiFormat.masterClockHz);
    SAI_TxEnable(DEMO_SAI, true);
#endif
    
    /*
     * config.enableI2S = true;
     */
    FLEXIO_I2S_GetDefaultConfig(&config);
    config.masterSlave = kFLEXIO_I2S_Master;
    FLEXIO_I2S_Init(&base, &config);

    /* Configure the audio format */
    format.bitWidth = kFLEXIO_I2S_WordWidth16bits;
    format.sampleRate_Hz = kFLEXIO_I2S_SampleRate16KHz;

    /* Configure Sgtl5000 I2C */
    codecHandle.base = DEMO_I2C;
    codecHandle.i2cHandle = &i2cHandle;
    i2cSourceClock = DEMO_I2C_CLK_FREQ;

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#else
    /*
     * i2cConfig.baudRate_Bps = 100000U;
     * i2cConfig.enableStopHold = false;
     * i2cConfig.glitchFilterWidth = 0U;
     * i2cConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
#endif

#if defined(DEMO_CODEC_WM8960)
    WM8960_Init(&codecHandle, NULL);
    WM8960_SetProtocol(&codecHandle, kWM8960_BusI2S);
    WM8960_ConfigDataFormat(&codecHandle, format.sampleRate_Hz * OVER_SAMPLE_RATE, format.sampleRate_Hz,
                            format.bitWidth);
#elif defined (DEMO_CODEC_DA7212)
    DA7212_Init(&codecHandle, NULL);
    DA7212_ConfigAudioFormat(&codecHandle, format.sampleRate_Hz, DEMO_SAI_CLK_FREQ, format.bitWidth);
    DA7212_ChangeOutput(&codecHandle, kDA7212_Output_HP);
    DA7212_WriteRegister(&codecHandle, DIALOG7212_DAI_CTRL, (DIALOG7212_DAI_EN_MASK | DIALOG7212_DAI_OE_MASK | DIALOG7212_DAI_WORD_LENGTH_16B |
                         DIALOG7212_DAI_FORMAT_I2S_MODE));
    /* Set the polarity of BCLK */
    DA7212_WriteRegister(&codecHandle, DIALOG7212_DAI_CLK_MODE, DIALOG7212_DAI_BCLKS_PER_WCLK_BCLK64 | DIALOG7212_DAI_CLK_POL_MASK);
#else
    /* configure sgtl5000 */
    sgtl_config_t codecConfig;
    codecConfig.bus = kSGTL_BusI2S;
    codecConfig.master_slave = false;
    codecConfig.route = kSGTL_RoutePlaybackandRecord;
    SGTL_Init(&codecHandle, &codecConfig);
    SGTL_ModifyReg(&codecHandle, CHIP_I2S_CTRL, SGTL5000_I2S_SCLK_INV_CLR_MASK, SGTL5000_I2S_VAILD_FALLING_EDGE);
    /* Configure codec format */
    SGTL_ConfigDataFormat(&codecHandle, OVER_SAMPLE_RATE * format.sampleRate_Hz, format.sampleRate_Hz, format.bitWidth);
#endif
    FLEXIO_I2S_TransferTxCreateHandle(&base, &txHandle, txCallback, NULL);
    FLEXIO_I2S_TransferRxCreateHandle(&base, &rxHandle, rxCallback, NULL);
    sourceClockHz = DEMO_FLEXIO_CLK_FREQ;

    /* Set audio format for tx and rx */
    FLEXIO_I2S_TransferSetFormat(&base, &txHandle, &format, sourceClockHz);
    FLEXIO_I2S_TransferSetFormat(&base, &rxHandle, &format, sourceClockHz);

    /*  xfer structure */
    xferTx.data = txBuff;
    xferTx.dataSize = BUFFER_SIZE;
    
    xferRx.data = rxBuff;
    xferRx.dataSize = BUFFER_SIZE;
    /* Add a transfer to trigger bit clock */
    full_block += 1;
    FLEXIO_I2S_TransferSendNonBlocking(&base, &txHandle, &xferTx);

    /* First record a piece of data */
    for (i = 0; i < 5000; i ++)
    {
        /* While there is empty block, setup a receive */
        if (empty_block > 0)
        {
            xferRx.data = rxBuff + RxInIndex * BUFFER_SIZE;
            FLEXIO_I2S_TransferReceiveNonBlocking(&base, &rxHandle, &xferRx);
        }
        /* If there is data for Rx buffer and there is empty block for Tx buffer */
        while ((empty_block >= BUFFER_BLOCK) || (full_block >= BUFFER_BLOCK))
        {
        }
        /* Copy data from RX buffer to tx buffer */
        memcpy(txBuff + TxInIndex * BUFFER_SIZE, rxBuff + RxOutIndex * BUFFER_SIZE, BUFFER_SIZE);

        /* Update Tx and Rx buffer information */
        TxInIndex = (TxInIndex + 1) % BUFFER_BLOCK;
        RxOutIndex = (RxOutIndex + 1) % BUFFER_BLOCK;
        empty_block += 1;
        full_block += 1;

        /* Setup a transfer */
        xferTx.data = txBuff + TxOutIndex * BUFFER_SIZE;
        FLEXIO_I2S_TransferSendNonBlocking(&base, &txHandle, &xferTx);
    }

#if defined(DEMO_CODEC_WM8960)
    WM8960_Deinit(&codecHandle);
#elif defined (DEMO_CODEC_DA7212)
#else
    SGTL_Deinit(&codecHandle);
#endif

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    LPI2C_MasterDeinit(DEMO_I2C);
#else
    I2C_MasterDeinit(DEMO_I2C);
#endif
    FLEXIO_I2S_Deinit(&base);
    PRINTF("\n\r FLEXIO_I2S interrupt example finished!\n\r ");

    while (1)
    {
    }
}
