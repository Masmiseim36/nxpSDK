/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "task.h"
#include "timers.h"

#include "board.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "fsl_lpi2c.h"
#if USE_COMMON_CODEC
#include "fsl_codec_common.h"
#else
#include "fsl_tfa9894.h"
#endif
#include "pdm_pcm_definitions.h"
#include "sln_amplifier.h"

#if USE_AUDIO_SPEAKER
#include "composite.h"
#endif

#include "sln_flash_mgmt.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define amplifier_send_task_PRIORITY (configMAX_PRIORITIES - 1)

#define PCM_AMP_DMA_CHUNK_SIZE          0x80000
#define PCM_AMP_DMA_TX_COMPLETE_EVT_BIT 1
#define PCM_AMP_AUDIO_ABORT_EVT_BIT     2
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static TaskHandle_t s_AmplifierSendTaskHandle;
static EventGroupHandle_t s_DmaTxComplete;
static EventBits_t s_LoopBackEvent;
static EventGroupHandle_t s_LoopBackEventGroup;

/* The DMA Handle for the TFA9894 audio amplifier SAI3 */
edma_handle_t s_AmpDmaTxHandle   = {0};
edma_handle_t s_AmpDmaRxHandle   = {0};
sai_edma_handle_t s_AmpTxHandler = {0};
sai_edma_handle_t s_AmpRxHandler = {0};

#if USE_COMMON_CODEC
codec_handle_t codecHandle;
#else
tfa98xx_handle_t tfa98xx_handle[NUM_TFA_DEVICE] = {0};
extern tfa98xx_operation_t g_TFA9894_operation;
#endif

uint32_t u16RxBufferIndex = 0;

__attribute__((section(".data.$SRAM_DTC"))) __attribute__((aligned(2)))
uint8_t s_AmpRxDmaBuffer[PCM_AMP_SAMPLE_COUNT * PCM_SAMPLE_SIZE_BYTES];
__attribute__((section(".data.$SRAM_DTC"))) __attribute__((aligned(2)))
uint8_t s_AmpRxDataBuffer[PCM_AMP_SAMPLE_COUNT * PCM_SAMPLE_SIZE_BYTES];

volatile uint32_t u32AudioLength = 0;
uint8_t *pu8AudioPlay            = NULL;

uint32_t startAudioLength = 0;
uint8_t *startAudioPlay   = NULL;
volatile uint8_t loop     = 0;

#if USE_AUDIO_SPEAKER
extern usb_device_composite_struct_t g_composite;
extern uint8_t audioPlayDataBuff[AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE];

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t audioPlayDMATempBuff[FS_ISO_OUT_ENDP_PACKET_SIZE];
#endif

static volatile uint8_t *pu8BufferPool   = NULL;
static uint8_t *pDefaultAudioData        = NULL;
static uint32_t s_DefaultAudioDataLength = 0;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
#if !(defined(USE_COMMON_CODEC) && USE_COMMON_CODEC)
static void wait_ms_func(uint32_t time_ms)
{
    vTaskDelay(portTICK_PERIOD_MS * time_ms);
}
#endif

void SAI_UserTxIRQHandler(void)
{
    SAI_TxClearStatusFlags(BOARD_AMP_SAI, kSAI_FIFOErrorFlag);
}

void SAI_UserRxIRQHandler(void)
{
    SAI_RxClearStatusFlags(BOARD_AMP_SAI, kSAI_FIFOErrorFlag);
}

static void SLN_AMP_RxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    static uint8_t firstTime = 1;

    if (!firstTime)
    {
        if (u16RxBufferIndex)
        {
            u16RxBufferIndex--;
        }
        else
        {
            u16RxBufferIndex++;
        }

        BaseType_t xHigherPriorityTaskWoken;
        xHigherPriorityTaskWoken = pdFALSE;

        GPIO_PinWrite(GPIO1, 2, 0U);
        sai_transfer_t sSaiRxBuffer = {0};

        memcpy(s_AmpRxDataBuffer, s_AmpRxDmaBuffer, PCM_AMP_SAMPLE_COUNT * PCM_SAMPLE_SIZE_BYTES);

        if (NULL != s_LoopBackEventGroup)
        {
            xEventGroupSetBitsFromISR(s_LoopBackEventGroup, s_LoopBackEvent, &xHigherPriorityTaskWoken);
        }

        // SLN_AMP_Read();
        /* Read the next 48KHz, Stereo, 16Bit sample */
        sSaiRxBuffer.data     = s_AmpRxDmaBuffer;
        sSaiRxBuffer.dataSize = PCM_AMP_SAMPLE_COUNT * PCM_SAMPLE_SIZE_BYTES;
        SAI_TransferReceiveEDMA(base, &s_AmpRxHandler, &sSaiRxBuffer);
        GPIO_PinWrite(GPIO1, 2, 1U);
    }
    firstTime = 0;
}

static void SLN_AMP_TxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_DmaTxComplete, PCM_AMP_DMA_TX_COMPLETE_EVT_BIT, &xHigherPriorityTaskWoken);

    if (pu8BufferPool != NULL)
    {
        (*pu8BufferPool)++;
    }

#if USE_AUDIO_SPEAKER
    sai_transfer_t xfer = {0};
    if ((g_composite.audioUnified.audioSendTimes >= g_composite.audioUnified.usbRecvTimes) &&
        (g_composite.audioUnified.startPlayHalfFull == 1))
    {
        g_composite.audioUnified.startPlayHalfFull      = 0;
        g_composite.audioUnified.speakerDetachOrNoInput = 1;
    }
    if (g_composite.audioUnified.startPlayHalfFull)
    {
        xfer.dataSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
        xfer.data     = audioPlayDataBuff + g_composite.audioUnified.tdWriteNumberPlay;
        g_composite.audioUnified.audioSendCount += FS_ISO_OUT_ENDP_PACKET_SIZE;
        g_composite.audioUnified.audioSendTimes++;
        g_composite.audioUnified.tdWriteNumberPlay += FS_ISO_OUT_ENDP_PACKET_SIZE;
        if (g_composite.audioUnified.tdWriteNumberPlay >=
            AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE)
        {
            g_composite.audioUnified.tdWriteNumberPlay = 0;
        }
    }
    else
    {
        xfer.dataSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
        xfer.data     = audioPlayDMATempBuff;
    }

    SAI_TransferSendEDMA(base, handle, &xfer);
#endif
}

void SLN_AMP_SetDefaultAudioData(uint8_t *data, uint32_t len)
{
    if (data)
    {
        pDefaultAudioData        = data;
        s_DefaultAudioDataLength = len;
    }
}

amplifier_status_t SLN_AMP_WriteDefault(void)
{
#if USE_SIGNAL_TEST_SIGNAL
    PlaybackSine();
#else
    sai_transfer_t xfer_codec = {0};

    // Take advantage of 32 being power of 2 to replace % to reduce # of instructions
    size_t overFlow = s_DefaultAudioDataLength & 31U;
    if (overFlow)
    {
        xfer_codec.dataSize = s_DefaultAudioDataLength - (overFlow);
    }
    xfer_codec.data = pDefaultAudioData;
    SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler, &xfer_codec);
#endif
    return 0;
}

void audio_send_task(void *pvParameters)
{
    sai_transfer_t write_xfer = {0};
    EventBits_t events;

    /* Chop into 512KB chunks so the DMA can handle it. */
    while (u32AudioLength)
    {
        events = xEventGroupWaitBits(s_DmaTxComplete, PCM_AMP_AUDIO_ABORT_EVT_BIT | PCM_AMP_DMA_TX_COMPLETE_EVT_BIT,
                                     pdTRUE, pdFALSE, portMAX_DELAY);

        // Abort
        if (events & 2)
        {
            SAI_TransferTerminateSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler);
            u32AudioLength = 0;
            pu8AudioPlay   = NULL;
            loop           = 0;
            break;
        }
        else
        {
            if (u32AudioLength > PCM_AMP_DMA_CHUNK_SIZE)
            {
                write_xfer.dataSize = PCM_AMP_DMA_CHUNK_SIZE;
                write_xfer.data     = pu8AudioPlay;
                SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler, &write_xfer);

                pu8AudioPlay += PCM_AMP_DMA_CHUNK_SIZE;
                u32AudioLength -= PCM_AMP_DMA_CHUNK_SIZE;
            }
            else
            {
                write_xfer.dataSize = u32AudioLength;
                write_xfer.data     = pu8AudioPlay;
                SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler, &write_xfer);

                if (loop)
                {
                    u32AudioLength = startAudioLength;
                    pu8AudioPlay   = startAudioPlay;
                }
                else
                {
                    u32AudioLength = 0;
                    pu8AudioPlay   = NULL;
                }
            }
        }
    }

    vTaskDelete(NULL);
}

amplifier_status_t SLN_AMP_Write(uint8_t *data, uint32_t length)
{
    sai_transfer_t write_xfer     = {0};
    amplifier_status_t eAmpStatus = 1;

    if (0 == u32AudioLength)
    {
        pu8AudioPlay = data;
        if (length)
        {
            u32AudioLength = length - (length % 32);
        }
        else
        {
            u32AudioLength = length;
        }

        /* Chop into 512KB chunks so the DMA can handle it. */
        if (u32AudioLength > PCM_AMP_DMA_CHUNK_SIZE)
        {
            write_xfer.dataSize = PCM_AMP_DMA_CHUNK_SIZE;
            write_xfer.data     = pu8AudioPlay;
            SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler, &write_xfer);

            pu8AudioPlay += PCM_AMP_DMA_CHUNK_SIZE;
            u32AudioLength -= PCM_AMP_DMA_CHUNK_SIZE;

            if (xTaskCreate(audio_send_task, "amplifier_send_task", 1024U, NULL, amplifier_send_task_PRIORITY,
                            &s_AmplifierSendTaskHandle) != pdPASS)
            {
                eAmpStatus = 1;
            }
            eAmpStatus = 0;
        }
        else
        {
            write_xfer.dataSize = u32AudioLength;
            write_xfer.data     = pu8AudioPlay;
            SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler, &write_xfer);

            u32AudioLength = 0;
            pu8AudioPlay   = NULL;

            eAmpStatus = 0;
        }
    }
    return eAmpStatus;
}

amplifier_status_t SLN_AMP_WriteLoop(uint8_t *data, uint32_t length)
{
    sai_transfer_t write_xfer     = {0};
    amplifier_status_t eAmpStatus = 1;

    if (0 == u32AudioLength)
    {
        pu8AudioPlay = data;
        if (length)
        {
            u32AudioLength = length - (length % 32);
        }
        else
        {
            u32AudioLength = length;
        }

        startAudioLength = u32AudioLength;
        startAudioPlay   = data;
        loop             = 1;

        /* Chop into 512KB chunks so the DMA can handle it. */
        if (u32AudioLength > PCM_AMP_DMA_CHUNK_SIZE)
        {
            write_xfer.dataSize = PCM_AMP_DMA_CHUNK_SIZE;
            write_xfer.data     = pu8AudioPlay;
            SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler, &write_xfer);

            pu8AudioPlay += PCM_AMP_DMA_CHUNK_SIZE;
            u32AudioLength -= PCM_AMP_DMA_CHUNK_SIZE;

            /* Clear all the bits before creating the task */
            xEventGroupClearBits(s_DmaTxComplete, PCM_AMP_DMA_TX_COMPLETE_EVT_BIT | PCM_AMP_AUDIO_ABORT_EVT_BIT);

            if (xTaskCreate(audio_send_task, "amplifier_send_task", 1024U, NULL, amplifier_send_task_PRIORITY,
                            &s_AmplifierSendTaskHandle) != pdPASS)
            {
                eAmpStatus = 1;
                configPRINTF(("Fail to create amplifier_send_task! \r\n"));
            }
            eAmpStatus = 0;
        }
        else
        {
            write_xfer.dataSize = u32AudioLength;
            write_xfer.data     = pu8AudioPlay;
            SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler, &write_xfer);

            u32AudioLength = 0;
            pu8AudioPlay   = NULL;

            eAmpStatus = 0;
        }
    }
    return eAmpStatus;
}

amplifier_status_t SLN_AMP_WriteNoWait(uint8_t *data, uint32_t length)
{
    uint32_t total_len;
    uint8_t *ptr              = data;
    sai_transfer_t write_xfer = {0};
    status_t ret              = 0;

    if (length)
    {
        total_len = length - (length % 32);
    }
    else
    {
        total_len = length;
    }

    /* Chop into 512KB chunks so the DMA can handle it. */
    write_xfer.dataSize = total_len;
    write_xfer.data     = ptr;
    ret                 = SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler, &write_xfer);

    return ret;
}

amplifier_status_t SLN_AMP_WriteBlocking(uint8_t *data, uint32_t length)
{
    uint32_t total_len;
    uint8_t *ptr              = data;
    sai_transfer_t write_xfer = {0};

    if (length)
    {
        total_len = length - (length % 32);
    }
    else
    {
        total_len = length;
    }

    /* Chop into 512KB chunks so the DMA can handle it. */
    while (total_len)
    {
        if (total_len > PCM_AMP_DMA_CHUNK_SIZE)
        {
            write_xfer.dataSize = PCM_AMP_DMA_CHUNK_SIZE;
            write_xfer.data     = ptr;
            SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler, &write_xfer);

            ptr += PCM_AMP_DMA_CHUNK_SIZE;
            total_len -= PCM_AMP_DMA_CHUNK_SIZE;
        }
        else
        {
            write_xfer.dataSize = total_len;
            write_xfer.data     = ptr;
            SAI_TransferSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler, &write_xfer);

            total_len = 0;
        }

        xEventGroupWaitBits(s_DmaTxComplete, PCM_AMP_DMA_TX_COMPLETE_EVT_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
    }

    return 0;
}

amplifier_status_t SLN_AMP_AbortWrite(void)
{
    xEventGroupSetBits(s_DmaTxComplete, PCM_AMP_AUDIO_ABORT_EVT_BIT);
    return 0;
}

amplifier_status_t SLN_AMP_Read(void)
{
    sai_transfer_t xfer = {0};
    xfer.data           = s_AmpRxDmaBuffer;
    xfer.dataSize       = sizeof(s_AmpRxDmaBuffer);

    SAI_TransferReceiveEDMA(BOARD_AMP_SAI, &s_AmpRxHandler, &xfer);
    return 0;
}

amplifier_status_t SLN_AMP_Init(volatile uint8_t *pu8BuffNum)
{
    status_t ret = kStatus_Fail;
    uint8_t do_calibration;
    uint8_t current_cal_state;

    sai_init_handle_t saiInitHandle = {
        .amp_dma_tx_handle = &s_AmpDmaTxHandle,
        .amp_dma_rx_handle = &s_AmpDmaRxHandle,
        .amp_sai_tx_handle = &s_AmpTxHandler,
        .amp_sai_rx_handle = &s_AmpRxHandler,
        .sai_tx_callback   = SLN_AMP_TxCallback,
        .sai_rx_callback   = SLN_AMP_RxCallback,
    };

    pu8BufferPool   = pu8BuffNum;
    s_DmaTxComplete = xEventGroupCreate();

    BOARD_Codec_I2C_Init();

    BOARD_SAI_Init(saiInitHandle);

    xEventGroupWaitBits(s_DmaTxComplete, PCM_AMP_DMA_TX_COMPLETE_EVT_BIT, pdTRUE, pdTRUE, portMAX_DELAY);

#if USE_COMMON_CODEC

    ret = CODEC_Init(&codecHandle, (codec_config_t *)BOARD_GetBoardCodecConfig());

#else

    BOARD_Get_Calibration_State(&current_cal_state);
    do_calibration = !(current_cal_state); /* current_cal_state == 0 triggers calibration */

    /* Configure TFA9894 */
    if (NUM_TFA_DEVICE == TFA_STEREO)
    {
        tfa98xx_handle[TFA_RIGHT].slaveAddress = TFA_RIGHT_AMP;
        tfa98xx_handle[TFA_RIGHT].base         = BOARD_CODEC_I2C_BASEADDR;
        tfa98xx_handle[TFA_RIGHT].msWait       = wait_ms_func;
        memcpy(&tfa98xx_handle[TFA_RIGHT].op, &g_TFA9894_operation, sizeof(tfa98xx_operation_t));
        if (!do_calibration)
        {
            /* check the internal calibration state */
            do_calibration = !TFA9894_IsCalibrated(&tfa98xx_handle[TFA_RIGHT]);
        }
    }
    tfa98xx_handle[TFA_LEFT].slaveAddress = TFA_LEFT_AMP;
    tfa98xx_handle[TFA_LEFT].base         = BOARD_CODEC_I2C_BASEADDR;
    tfa98xx_handle[TFA_LEFT].msWait       = wait_ms_func;
    memcpy(&tfa98xx_handle[TFA_LEFT].op, &g_TFA9894_operation, sizeof(tfa98xx_operation_t));
    if (!do_calibration)
    {
        /* check the internal calibration state */
        do_calibration = !TFA9894_IsCalibrated(&tfa98xx_handle[TFA_LEFT]);
    }

    ret = TFA9894_Init(&tfa98xx_handle[TFA_LEFT], NULL, do_calibration);
    if (kStatus_Success == ret)
    {
        if (NUM_TFA_DEVICE == TFA_STEREO)
        {
            ret = TFA9894_Init(&tfa98xx_handle[TFA_RIGHT], NULL, do_calibration);
        }
        if ((kStatus_Success == ret) && do_calibration)
        {
            BOARD_Set_Calibration_State(1); /* cal not needed anymore */
        }
    }

    /* If the calibration has just complete, re-initialize the TFA with the new calibration
     * If this is not done, the audio quality will be bad
     * */
    if (do_calibration)
    {
        /* Initialize again as calibration is complete and need to apply new settings without calibration */
        ret = TFA9894_Init(&tfa98xx_handle[TFA_LEFT], NULL, 0);
        if (kStatus_Success == ret)
        {
            if (NUM_TFA_DEVICE == TFA_STEREO)
            {
                ret = TFA9894_Init(&tfa98xx_handle[TFA_RIGHT], NULL, 0);
            }
        }
    }
#endif

    return ret;
}

uint8_t *SLN_AMP_GetLoopBackBuffer(void)
{
    return s_AmpRxDataBuffer;
}

void SLN_AMP_SetLoopBackEventGroup(EventGroupHandle_t *eventGroup)
{
    if ((NULL != eventGroup) && (NULL != *eventGroup))
    {
        s_LoopBackEventGroup = *eventGroup;
    }
}

void SLN_AMP_SetLoopBackEventBits(EventBits_t event)
{
    s_LoopBackEvent = event;
}

void SLN_AMP_SetVolume(uint8_t volume)
{
#if USE_COMMON_CODEC
    /* Set Volume 0(min) to 100 (max) */
    CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelLeft0 | kCODEC_PlayChannelRight0, (uint32_t)volume);

#else
    /* Example volume table for the TFA
     * Volume level index and corresponding VOL value(dec) from TFA9894 register 0x51
     * Vol_lvl  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14 (Maximum attenuation)
     * Vol_val  0   1   2   4   6   10  16  24  30  40  50  60  80  100 255 (Soft mute) */

    switch (volume / 10)
    {
        case 0:
            // volume = 255;
            break;
        case 1:
            volume = 40;
            break;
        case 2:
            volume = 30;
            break;
        case 3:
            volume = 24;
            break;
        case 4:
            volume = 16;
            break;
        case 5:
            volume = 10;
            break;
        case 6:
            volume = 6;
            break;
        case 7:
            volume = 4;
            break;
        case 8:
            volume = 2;
            break;
        case 9:
            volume = 1;
            break;
        case 10:
            volume = 0;
            break;
    }

    /* Expects a value between 0(max volume) to 255 (max attenuation) */
    TFA9894_SetVolume(&tfa98xx_handle[TFA_LEFT], volume);
#endif
}

void *SLN_AMP_GetAmpTxHandler()
{
    return &s_AmpTxHandler;
}

void SLN_AMP_Abort(void)
{
    /* Stop playback. This will flush the SAI transmit buffers. */
    SAI_TransferTerminateSendEDMA(BOARD_AMP_SAI, &s_AmpTxHandler);
}

void SLN_AMP_LoopbackEnable(void)
{
    DMAMUX_EnableChannel(DMAMUX, 3U);
    NVIC_EnableIRQ(SAI3_RX_IRQn);
}

void SLN_AMP_LoopbackDisable(void)
{
    NVIC_DisableIRQ(SAI3_RX_IRQn);
    DMAMUX_DisableChannel(DMAMUX, 3U);
}
