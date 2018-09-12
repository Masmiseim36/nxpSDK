/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_flexio_i2s.h"
#include "fsl_debug_console.h"
#include "fsl_wm8960.h"
#include "fsl_sai.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI and I2C instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_I2C LPI2C1
#define DEMO_FLEXIO_BASE FLEXIO2
#define DEMO_SAI SAI1
/* Select Audio PLL (786.43 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (3U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (31U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                            \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U))

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

/* Select Audio PLL (786.43 MHz) as flexio clock source, need to sync with sai clock, or the codec may not work */
#define DEMO_FLEXIO_CLKSRC_SEL (0U)
/* Clock pre divider for flexio clock source */
#define DEMO_FLEXIO_CLKSRC_PRE_DIV (7U)
/* Clock divider for flexio clock source */
#define DEMO_FLEXIO_CLKSRC_DIV (7U)
#define DEMO_FLEXIO_CLK_FREQ \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_FLEXIO_CLKSRC_PRE_DIV + 1U) / (DEMO_FLEXIO_CLKSRC_DIV + 1U))

#define BCLK_PIN (8U)
#define FRAME_SYNC_PIN (7U)
#define TX_DATA_PIN (6U)
#define RX_DATA_PIN (5U)
#define OVER_SAMPLE_RATE (384)
#define BUFFER_SIZE (128)
#define BUFFER_NUM (2)
#define PLAY_COUNT (5000)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* USB1 PLL configuration for RUN mode */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32U,         /*!< PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1U,          /*!< Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator = 0x05F5E100U,   /*!< 30 bit numerator of fractional loop divider.*/
    .denominator = 0x07C3097FU, /*!< 30 bit denominator of fractional loop divider */
};
flexio_i2s_handle_t txHandle = {0};
flexio_i2s_handle_t rxHandle = {0};
static volatile bool isTxFinished = false;
static volatile bool isRxFinished = false;
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM], 4);
codec_handle_t codecHandle = {0};
extern codec_config_t boardCodecConfig;
static volatile uint32_t beginCount = 0;
static volatile uint32_t sendCount = 0;
static volatile uint32_t receiveCount = 0;
static volatile uint8_t fullBlock = 0;
static volatile uint8_t emptyBlock = 0;

FLEXIO_I2S_Type base;
/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK);
    }
}

static void txCallback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_handle_t *handle, status_t status, void *userData)
{
    sendCount++;
    emptyBlock++;

    if (sendCount == beginCount)
    {
        isTxFinished = true;
    }
}

static void rxCallback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_handle_t *handle, status_t status, void *userData)
{
    receiveCount++;
    fullBlock++;

    if (receiveCount == beginCount)
    {
        isRxFinished = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexio_i2s_config_t config;
    uint32_t sourceClockHz = 0U;
    flexio_i2s_format_t format;
    flexio_i2s_transfer_t txXfer, rxXfer;
    uint8_t txIndex = 0, rxIndex = 0;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_I2C_ConfigurePins();
    BOARD_FLEXIO_ConfigurePins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_SAI_ConfigurePins();
    CLOCK_InitAudioPll(&audioPllConfig);

    /* Clock setting for LPI2C */
    CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);
    /* Clock setting for FLEXIO */
    CLOCK_SetMux(kCLOCK_Flexio2Mux, DEMO_FLEXIO_CLKSRC_SEL);
    CLOCK_SetDiv(kCLOCK_Flexio2PreDiv, DEMO_FLEXIO_CLKSRC_PRE_DIV);
    CLOCK_SetDiv(kCLOCK_Flexio2Div, DEMO_FLEXIO_CLKSRC_DIV);
    /* Clock setting for SAI1 */
    CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);
    /* Enable SAI1 MCLK output */
    BOARD_EnableSaiMclkOutput(true);
    BOARD_Codec_I2C_Init();
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

#if defined(DEMO_CODEC_WM8960) || defined(DEMO_CODEC_DA7212)
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

    /* Use default setting to init codec */
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, format.sampleRate_Hz * OVER_SAMPLE_RATE, format.sampleRate_Hz, format.bitWidth);
    
    FLEXIO_I2S_TransferTxCreateHandle(&base, &txHandle, txCallback, NULL);
    FLEXIO_I2S_TransferRxCreateHandle(&base, &rxHandle, rxCallback, NULL);
    sourceClockHz = DEMO_FLEXIO_CLK_FREQ;

    /* Set audio format for tx and rx */
    FLEXIO_I2S_TransferSetFormat(&base, &txHandle, &format, sourceClockHz);
    FLEXIO_I2S_TransferSetFormat(&base, &rxHandle, &format, sourceClockHz);

    emptyBlock = BUFFER_NUM;
    fullBlock = 1;
    beginCount = PLAY_COUNT;
    txXfer.dataSize = BUFFER_SIZE;
    rxXfer.dataSize = BUFFER_SIZE;

    /* Wait until finished */
    while ((isTxFinished != true) || (isRxFinished != true))
    {
        if ((isTxFinished != true) && (fullBlock > 0))
        {
            txXfer.data = audioBuff + txIndex * BUFFER_SIZE;
            FLEXIO_I2S_TransferSendNonBlocking(&base, &txHandle, &txXfer);
            fullBlock--;
            txIndex = (txIndex + 1) % BUFFER_NUM;
        }

        if ((isRxFinished != true) && (emptyBlock > 0))
        {
            rxXfer.data = audioBuff + rxIndex * BUFFER_SIZE;
            FLEXIO_I2S_TransferReceiveNonBlocking(&base, &rxHandle, &rxXfer);
            emptyBlock--;
            rxIndex = (rxIndex + 1) % BUFFER_NUM;
        }
    }
    PRINTF("\n\r FLEXIO_I2S interrupt example finished!\n\r ");

    while (1)
    {
    }
}
