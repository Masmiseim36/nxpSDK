/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_sai.h"
#include "fsl_sdadc.h"
#include "fsl_codec_common.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t txBuff[DEMO_BUFFER_SIZE * DEMO_BUFFER_NUMBER], 4);
codec_handle_t codecHandle;
sai_handle_t saiTxHandle = {0};
extern codec_config_t boardCodecConfig;
static volatile uint32_t readIndex  = 0U;
static volatile uint32_t writeIndex = 0U;

sdadc_channel_config_t chanConf[] = {
    {
        .mode   = kSDADC_SingleEnd_Mode,
        .number = kSDADC_Channel0,
        .type   = kSDADC_Channel_PSide_Type,
        .volume =
            {
                .pSideVolume = 20U,
            },
        .samplerate =
            {
                .pSideSampleRate = kSDADC_DecimatorSampleRate48kHz,
            },
        .watermark =
            {
                .pSideWatermark = 0U,
            },
        .enableDacCompensation = true,
        .enableDcFilter        = true,
        .enableDCLoop          = false,
        .enablePolarityInvert  = false,
    },
    {
        .mode   = kSDADC_SingleEnd_Mode,
        .number = kSDADC_Channel1,
        .type   = kSDADC_Channel_PSide_Type,
        .volume =
            {
                .pSideVolume = 20U,
            },
        .samplerate =
            {
                .pSideSampleRate = kSDADC_DecimatorSampleRate48kHz,
            },
        .watermark =
            {
                .pSideWatermark = 0U,
            },
        .enableDacCompensation = true,
        .enableDcFilter        = true,
        .enableDCLoop          = false,
        .enablePolarityInvert  = false,
    },
};

static sdadc_channel_group group[] = {
    [0U] =
        {
            .number = kSDADC_Channel0,
            .type   = kSDADC_Channel_PSide_Type,
        },
    [1U] =
        {
            .number = kSDADC_Channel1,
            .type   = kSDADC_Channel_PSide_Type,
        },
};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief SDADC interrupt handler.
 */
void DEMO_SDADC_IRQ_HANDLER_FUNC(void)
{
    if (SDADC_CheckGlobalFifoInterrupted(DEMO_SDADC_BASE))
    {
        if (((readIndex >= writeIndex) || (readIndex <= (writeIndex - 1U))))
        {
            SDADC_CopyConvChannelFifoToBuffer(DEMO_SDADC_BASE, &(group[0U]), sizeof(group) / sizeof(*group),
                                              &txBuff[readIndex * DEMO_BUFFER_SIZE]);
            readIndex++;

            if (readIndex == DEMO_BUFFER_NUMBER)
            {
                readIndex = 0U;
            }
        }
        SDADC_ClearGlobalFifoIntStatusFlag(DEMO_SDADC_BASE);
    }
}

/*!
 * @brief SDADC initialize function.
 */
static void DEMO_SDADC_INIT(void)
{
    sdadc_config_t config;

    /* Do sdadc configuration. */
    SDADC_GetDefaultConfig(&config);
    config.channelCount  = sizeof(group) / sizeof(*group);
    config.channelConfig = &(chanConf[0U]);
    SDADC_Init(DEMO_SDADC_BASE, &config);

    SDADC_ControlFifoIntEnable(
        DEMO_SDADC_BASE, (kSDADC_Channel0_PSide_FifoFullIntEnable | kSDADC_Channel1_PSide_FifoFullIntEnable), true);
    SDADC_ControlGlobalFifoIntEnable(DEMO_SDADC_BASE, true);

    /* Do sdadc power-up. */
    SDADC_DoInitPowerUp(DEMO_SDADC_BASE, &config, SystemCoreClock);

    EnableIRQ(DEMO_SDADC_IRQn);
}

/*!
 * @brief SAI and codec initialize function.
 */
static void DEMO_SAI_INIT(void)
{
    sai_transceiver_t config;

    /* Do SAI initialization. */
    SAI_Init(DEMO_SAI_BASE);
    SAI_TransferTxCreateHandle(DEMO_SAI_BASE, &saiTxHandle, NULL, NULL);

    /* Do SAI I2S mode configuration. */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, DEMO_SOUND_MODE, 1U << DEMO_SAI_CHANNEL);
    config.serialData.dataFirstBitShifted = (DEMO_AUDIO_BIT_WIDTH - 8U);
    config.bitClock.bclkSource            = DEMO_SAI_CLOCK_SOURCE;
#if defined BOARD_SAI_RXCONFIG
    config.syncMode = DEMO_SAI_TX_SYNC_MODE;
#endif
    config.masterSlave = DEMO_SAI_MASTER_SLAVE;
    SAI_TransferTxSetConfig(DEMO_SAI_BASE, &saiTxHandle, &config);

    /* Set bit clock divider. */
    SAI_TxSetBitClockRate(DEMO_SAI_BASE, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
#if defined BOARD_SAI_RXCONFIG
    BOARD_SAI_RXCONFIG(&config, DEMO_SAI_RX_SYNC_MODE);
#endif

    /* Set master clock. */
    BOARD_MasterClockConfig();

    /* Do codec initialization. */
#if defined DEMO_BOARD_CODEC_INIT
    DEMO_BOARD_CODEC_INIT();
#else
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        assert(false);
    }
#endif
}

/*!
 * @brief Main function.
 */
int main(void)
{
    sai_transfer_t transfer;

    BOARD_InitHardware();

    PRINTF("\r\n SDADC SAI Interrupt Example.");

    memset(txBuff, 0U, sizeof(txBuff));

    DEMO_SAI_INIT();
    DEMO_SDADC_INIT();

    while (1)
    {
        if (readIndex != writeIndex)
        {
            transfer.data     = (uint8_t *)(&txBuff[writeIndex * DEMO_BUFFER_SIZE]);
            transfer.dataSize = DEMO_BUFFER_SIZE;

            if (SAI_TransferSendNonBlocking(DEMO_SAI_BASE, &saiTxHandle, &transfer) != kStatus_SAI_QueueFull)
            {
                writeIndex++;

                if (writeIndex == DEMO_BUFFER_NUMBER)
                {
                    writeIndex = 0U;
                }
            }
        }
    }
}
