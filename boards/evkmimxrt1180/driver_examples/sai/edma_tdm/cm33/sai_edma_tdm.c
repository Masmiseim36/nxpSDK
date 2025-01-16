/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#include "fsl_debug_console.h"
#include "fsl_sai_edma.h"
#include "fsl_sd.h"
#include "ff.h"
#include "diskio.h"
#include "ffconf.h"
#include "fsl_sd_disk.h"
#include "fsl_codec_common.h"
#include "sdmmc_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef DEMO_DMAMUX
#define DEMO_DMAMUX DMAMUX0
#endif

#define BUFFER_SIZE (2048U)
#define BUFFER_NUM  (4U)

/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (8U)
/* demo audio bitwidth */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth32bits
#define DEMO_FRMAE_SYNC_LEN  kSAI_FrameSyncLenOneBitClk
#define DEMO_SAI_CHANNEL     kSAI_Channel0Mask
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
static status_t DEMO_MountFileSystem(void);
extern void BORAD_CodecReset(bool state);
static void DEMO_InitCodec(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(DEMO_QUICKACCESS_SECTION_CACHEABLE) && DEMO_QUICKACCESS_SECTION_CACHEABLE
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle);
#else
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t txHandle);
#endif
edma_handle_t dmaTxHandle = {0};
extern codec_config_t boardCodecConfig;
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_buffer[BUFFER_NUM][BUFFER_SIZE], 4);
volatile bool isFinished      = false;
volatile uint32_t finishIndex = 0U;
volatile uint32_t emptyBlock  = BUFFER_NUM;
/*! @brief Card descriptor. */
extern sd_card_t g_sd;
static uint32_t volatile s_writeIndex = 0U;
static uint32_t volatile s_readIndex  = 0U;
static uint32_t volatile s_emptyBlock = BUFFER_NUM;
static FATFS s_fileSystem; /* File system object */
static FIL s_fileObject;
static FILINFO s_fileInfo;
static volatile bool s_saiTransferFinish = false;
codec_handle_t codecHandle;

sai_transfer_t saiTxPingPongTransfer[4U] = {
    {
        .data     = s_buffer[0],
        .dataSize = BUFFER_SIZE,
    },
    {
        .data     = s_buffer[1],
        .dataSize = BUFFER_SIZE,
    },
    {
        .data     = s_buffer[2],
        .dataSize = BUFFER_SIZE,
    },
    {
        .data     = s_buffer[3],
        .dataSize = BUFFER_SIZE,
    },
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxError == status)
    {
    }
    else
    {
        s_saiTransferFinish = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    edma_config_t dmaConfig = {0};
    sai_transceiver_t saiConfig;
    UINT oneTimeRW = 0U;
    UINT bytesRead;
    FRESULT error;
    uint32_t leftWAVData = 0U;

    BOARD_InitHardware();
    BOARD_SD_Config(&g_sd, NULL, BOARD_SDMMC_SD_HOST_IRQ, NULL);

    PRINTF("\r\nSAI edma TDM example started.\n\r");

    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
#if defined(BOARD_GetEDMAConfig)
    BOARD_GetEDMAConfig(dmaConfig);
#endif
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(EXAMPLE_DMA, EXAMPLE_TX_CHANNEL, EXAMPLE_SAI_TX_SOURCE);
#endif

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
    DMAMUX_Init(DEMO_DMAMUX);
    DMAMUX_SetSource(DEMO_DMAMUX, EXAMPLE_CHANNEL, EXAMPLE_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, EXAMPLE_CHANNEL);
#endif

    if (DEMO_MountFileSystem() != kStatus_Success)
    {
        PRINTF("Mount file system failed, make sure card is formatted.\r\n");
        return -1;
    }

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, tx_callback, NULL, &dmaTxHandle);

    /* TDM mode configurations */
    SAI_GetTDMConfig(&saiConfig, DEMO_FRMAE_SYNC_LEN, DEMO_AUDIO_BIT_WIDTH, DEMO_AUDIO_DATA_CHANNEL, DEMO_SAI_CHANNEL);
    saiConfig.frameSync.frameSyncEarly = true;
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &saiConfig);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    /* CS42888 initialization */
    DEMO_InitCodec();

    /* The 8_TDM.wav file process flow:
     * 1.Full fill the transfer buffer firstly, it is important to make sure the audio data is transferred continuously.
     * 2.Send one buffer block through SAI when there is at least one buffer block is full.
     * 3.read one buffer block from sdcard when there is at least one buffer block is empty and transfer done.
     * with step2/step3 repeat, audio data will be sent out continuously.
     * */
    if (s_fileInfo.fsize > BUFFER_SIZE * BUFFER_NUM)
    {
        oneTimeRW = BUFFER_SIZE * BUFFER_NUM;
    }
    else
    {
        oneTimeRW = s_fileInfo.fsize;
    }
    error = f_read(&s_fileObject, s_buffer, oneTimeRW, &bytesRead);
    if ((error) || (bytesRead != oneTimeRW))
    {
        PRINTF("Read file failed.\r\n");
        return -1;
    }
    s_emptyBlock -= BUFFER_NUM;
    s_readIndex += BUFFER_NUM;
    leftWAVData = s_fileInfo.fsize - oneTimeRW;
    oneTimeRW   = BUFFER_SIZE;

    PRINTF("\r\nStart play 8_TDM.wav file.\n\r");
    SAI_TransferSendLoopEDMA(DEMO_SAI, &txHandle, &saiTxPingPongTransfer[0], 4U);

    while (leftWAVData > BUFFER_SIZE)
    {
        if ((s_saiTransferFinish))
        {
            if (s_readIndex >= BUFFER_NUM)
            {
                s_readIndex = 0U;
            }

            error = f_read(&s_fileObject, (uint8_t *)((uint32_t)s_buffer[s_readIndex]), BUFFER_SIZE, &bytesRead);
            if ((error) || (bytesRead != BUFFER_SIZE))
            {
                PRINTF("Read file failed.\r\n");
                return -1;
            }

            s_readIndex++;
            leftWAVData -= BUFFER_SIZE;
            s_saiTransferFinish = false;
        }
    }
    f_close(&s_fileObject);
    /* Once transfer finish, disable SAI instance. */
    SAI_TransferAbortSendEDMA(DEMO_SAI, &txHandle);
    SAI_Deinit(DEMO_SAI);
    CODEC_Deinit(&codecHandle);
    PRINTF("\r\nSAI TDM EDMA example finished.\n\r ");
    while (1)
    {
    }
}

#if defined(SAI_ErrorIRQHandler)
void SAI_ErrorIRQHandler(void)
{
    /* Clear the FIFO error flag */
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);

    /* Reset FIFO */
    SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);
    SDK_ISR_EXIT_BARRIER;
}
#endif

static status_t DEMO_MountFileSystem(void)
{
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
    FRESULT error;

    if (f_mount(&s_fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("Mount volume failed.\r\n");
        return kStatus_Fail;
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("Change drive failed.\r\n");
        return kStatus_Fail;
    }
#endif

    error = f_open(&s_fileObject, _T("/8_TDM.wav"), FA_READ);
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("8_TDM.wav File exists.\r\n");
        }
        else
        {
            PRINTF("8_TDM file not exist.\r\n");
            return kStatus_Fail;
        }
    }

    error = f_stat(_T("/8_TDM.wav"), &s_fileInfo);
    if (error != FR_OK)
    {
        PRINTF("Get file status failed\r\n");
        return kStatus_Fail;
    }

    PRINTF("\r\n8_TDM.wav File is available\r\n");

    return kStatus_Success;
}

static void DEMO_InitCodec(void)
{
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        PRINTF("CODEC_Init failed!\r\n");
        assert(false);
    }

    PRINTF("\r\nCodec Init Done.\r\n");
}
