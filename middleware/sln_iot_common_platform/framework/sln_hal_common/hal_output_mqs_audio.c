/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief MQS (medium quality sound) audio HAL output device implementation. Used for playing audio clips over the
 * board's speakers.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"
#include "math.h"

#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_cache.h"

#include "fwk_log.h"
#include "fwk_output_manager.h"
#include "fwk_sln_task.h"

#include "hal_output_dev.h"
#include "hal_event_descriptor_common.h"
#include "hal_smart_lock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FINISHED_BIT (1 << 0)
/* DMA */
#define MQS_DMA            DMA0
#define MQS_DMAMUX         DMAMUX0
#define MQS_SAI_EDMA_TX_CH 0
#define MQS_SAI_TX_SOURCE  kDmaRequestMuxSai3Tx

#define MQS_SAI_DMA_IRQ_PRIO (configMAX_SYSCALL_INTERRUPT_PRIORITY - 1)
/* Maximum audio play buffer size per blocking DMA transfer call */
#define MQS_AUDIO_CHUNK_SIZE 0x30000

#if defined(__cplusplus)
extern "C" {
#endif
void BOARD_InitMQSResource(void);
int HAL_OutputDev_MqsAudio_Register();
static hal_output_status_t HAL_OutputDev_MqsAudio_Init(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_MqsAudio_Start(const output_dev_t *dev);
static void _PlaySound(const uint8_t *buffer, int32_t size);
static status_t _GetVolume(char *valueToString);
#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Variable
 ******************************************************************************/

static AT_NONCACHEABLE_SECTION_ALIGN(sai_edma_handle_t s_SaiTxHandle, 4);
static AT_NONCACHEABLE_SECTION_ALIGN(edma_handle_t s_SaiDmaHandle, 4);
static AT_NONCACHEABLE_SECTION_ALIGN_OCRAM(uint8_t s_StreamBuffer[MQS_AUDIO_CHUNK_SIZE], 4);

static volatile EventGroupHandle_t s_EventFinished = NULL;

#define DEFER_PLAYBACK_TO_TASK 1
#if DEFER_PLAYBACK_TO_TASK
typedef struct
{
    fwk_task_data_t commonData;
    const output_dev_t *dev;
} mqs_task_data_t;

typedef struct
{
    fwk_task_t task;
    mqs_task_data_t data;
} mqs_task_t;

typedef enum _mqs_configs
{
    kMQSConfigs_Volume = 0,
} mqs_configs;

static mqs_task_t s_MqsAudioTask;

#define MQS_TASK_NAME     "mqs_audio"
#define MQS_TASK_STACK    1024
#define MQS_TASK_PRIORITY 3
#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_MqsAudioTaskStack[MQS_TASK_STACK];
FWKDATA static StaticTask_t s_MqsAudioTaskTcb;
static void *s_MqsAudioTaskTcbReference = (void *)&s_MqsAudioTaskTcb;
#else
static void *s_MqsAudioTaskStack        = NULL;
static void *s_MqsAudioTaskTcbReference = NULL;
#endif

typedef struct
{
    const uint8_t *buffer;
    int32_t size;
} sound_info_t;

const static output_dev_operator_t s_OutputDev_MqsAudioOps = {
    .init   = HAL_OutputDev_MqsAudio_Init,
    .deinit = NULL,
    .start  = HAL_OutputDev_MqsAudio_Start,
    .stop   = NULL,
};

static output_dev_t s_OutputDev_MqsAudio = {
    .name         = "sound",
    .attr.type    = kOutputDevType_Audio,
    .attr.reserve = NULL,
    .ops          = &s_OutputDev_MqsAudioOps,
    .configs =
        {
            [kMQSConfigs_Volume] = {.name          = "volume",
                                    .expectedValue = "<0-100>",
                                    .description   = "% volume of the speaker",
                                    .get           = _GetVolume},
        },
};

static void _postSoundPlayRequest(const uint8_t *buffer, int32_t size)
{
    fwk_message_t *Msg = (fwk_message_t *)SLN_MALLOC(sizeof(fwk_message_t));

    if (Msg != NULL)
    {
        Msg->freeAfterConsumed   = 1;
        Msg->id                  = kFWKMessageID_Raw;
        sound_info_t *pSoundInfo = SLN_MALLOC(sizeof(sound_info_t));
        if (pSoundInfo != NULL)
        {
            pSoundInfo->buffer         = buffer;
            pSoundInfo->size           = size;
            Msg->raw.data              = pSoundInfo;
            Msg->raw.freeAfterConsumed = 1;
            FWK_Message_Put(MQS_AUDIO_TASK_ID, &Msg);
        }
        else
        {
            LOGE("SLN_MALLOC");
            SLN_FREE(Msg);
        }
    }
    else
    {
        LOGE("SLN_MALLOC");
    }
}

static void HAL_OutputDev_MqsAudio_MsgHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    LOGI("HAL_OutputDev_MqsAudio_MsgHandle\r");
    if ((pMsg == NULL) || (pTaskData == NULL) || (pMsg->id != kFWKMessageID_Raw) || (pMsg->raw.data == NULL))
    {
        return;
    }

    sound_info_t *pSoundInfo = (sound_info_t *)pMsg->raw.data;
    _PlaySound(pSoundInfo->buffer, pSoundInfo->size);

    if (pMsg->raw.freeAfterConsumed)
    {
        SLN_FREE(pMsg->raw.data);
        pMsg->raw.data = NULL;
    }
}

#define MQS_SOUND_PLAY_FUNC(buffer, size) _postSoundPlayRequest((buffer), (size))
#else
#define MQS_SOUND_PLAY_FUNC(buffer, size) _PlaySound((buffer), (size))
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

__attribute__((weak)) int APP_OutputDev_MqsAudio_InferCompleteDecode(output_algo_source_t source,
                                                                     void *inferResult,
                                                                     void **audio,
                                                                     uint32_t *len)
{
    return 0;
}

__attribute__((weak)) int APP_OutputDev_MqsAudio_InputNotifyDecode(const output_dev_t *dev,
                                                                   void *inputData,
                                                                   void **audio,
                                                                   uint32_t *len)
{
    return 0;
}

static void _SaiCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    BaseType_t xHigherPriorityTaskWoken, result;
    result = xEventGroupSetBitsFromISR(s_EventFinished, FINISHED_BIT, &xHigherPriorityTaskWoken);
    if (result != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

static void _ConfigMqs(void)
{
    CLOCK_EnableClock(kCLOCK_Mqs); /* Enable MQS hmclk. */

    IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, true);  /* Reset MQS. */
    IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, false); /* Release reset MQS. */
    IOMUXC_MQSEnable(IOMUXC_GPR, true);              /* Enable MQS. */
    //    IOMUXC_MQSConfig(IOMUXC_GPR, kIOMUXC_MqsPwmOverSampleRate64, 0u);           /* 78.6432MHz/64/(0+1) = 1.2288MHz
    //                                                                                 Higher frequency PWM involves
    //                                                                                 less low frequency harmonic.*/
    IOMUXC_MQSConfig(IOMUXC_GPR, kIOMUXC_MqsPwmOverSampleRate32,
                     0u); /* 78.6432MHz/32/(0+1) = 2.4576MHz
                             Higher frequency PWM involves less low frequency harmonic.*/
}

/*!
 * brief set audio volume for this amp.
 *
 * param volume volume value, support 0 ~ 100, only in multiples of 10, 0 is mute, 100 is the maximum volume value.
 */
static float _AdaptVolume(uint32_t volume)
{
    assert(volume >= 0 && volume <= 100);

    volume /= 10;

    /*
     * This is the function used for generating a nice polynomial curve for the volume levels.
     *
     *                  y = -0.0018 * x ^ 3 + 0.028 * x ^ 2
     *
     * In this case it's more like a linear function with the lower and upper ends slightly curved.
     *
     * The levels go from 0 to 1, making sure that level 1 stays low at 0.0262
     * while still being able to reach the value 1 at level 10.
     *
     * This function is called once for every volume change, so these operations shouldn't be
     * that much of a burden
     */
    return (-0.0018 * pow(volume, 3) + 0.028 * pow(volume, 2));
}

static void _VolDiffControl(void *data, uint32_t len)
{
    uint32_t i;
    int16_t *data16 = (int16_t *)data;
    float volume    = _AdaptVolume(s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value);
    uint32_t len16  = len / sizeof(int16_t);

    for (i = 0; i < len16 - 1; i += 2)
    {
        // the volume is decreased by multiplying the samples with values between 0 and 1
        data16[i]     = (int16_t)(data16[i] * volume);
        data16[i + 1] = -data16[i];
    }
}

/*!
 * @brief play audio clip
 *
 * @param buffer pointer to audio clip
 * @param size size of audio buffer
 */
static void _PlaySound(const uint8_t *buffer, int32_t size)
{
    sai_transfer_t xfer;

    /*  xfer structure (truncated) */
    xfer.data = s_StreamBuffer;

    /* Enable output of Audio amplifier */
    GPIO_PinWrite(BOARD_MQS_OE_GPIO_PORT, BOARD_MQS_OE_GPIO_PIN, 1);

    /* The size of the played sound should be multiple of 32 */
    size -= size % 32;

    while (size > 0)
    {
        /* Wait until finished */
        xfer.dataSize = (size <= MQS_AUDIO_CHUNK_SIZE) ? size : MQS_AUDIO_CHUNK_SIZE;

        /* copy from flash into local buffer for processing */
        memcpy(s_StreamBuffer, buffer, xfer.dataSize);

        /* differentiate and adjust volume */
        _VolDiffControl(s_StreamBuffer, xfer.dataSize);
        /* play this chunk */
        SAI_TransferSendEDMA(MQS_SAI, &s_SaiTxHandle, &xfer);
        /* move to next chunk */
        buffer += xfer.dataSize;
        size -= xfer.dataSize;
        xEventGroupWaitBits(s_EventFinished, FINISHED_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
    }
    /* Disable output of Audio amplifier */
    GPIO_PinWrite(BOARD_MQS_OE_GPIO_PORT, BOARD_MQS_OE_GPIO_PIN, 0);
}

static hal_output_status_t HAL_OutputDev_MqsAudio_InferComplete(const output_dev_t *dev,
                                                                output_algo_source_t source,
                                                                void *inferResult)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    uint8_t *audioBuffer      = NULL;
    uint32_t audioLen         = 0;

    APP_OutputDev_MqsAudio_InferCompleteDecode(source, inferResult, (void *)&audioBuffer, &audioLen);
    if (audioBuffer != NULL && audioLen != 0)
    {
        MQS_SOUND_PLAY_FUNC(audioBuffer, audioLen);
    }

    return error;
}

static hal_output_status_t HAL_OutputDev_MqsAudio_Input_notify(const output_dev_t *dev, void *data)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    uint8_t *audioBuffer      = NULL;
    uint32_t audioLen         = 0;
    event_base_t eventBase    = *(event_base_t *)data;

    if (eventBase.eventId == kEventID_GetSpeakerVolume)
    {
        speaker_volume_event_t speaker;
        speaker.volume = HAL_OutputDev_SmartLockConfig_GetSpeakerVolume();
        LOGD("Current volume is: %d", speaker.volume);
        if (eventBase.respond != NULL)
        {
            eventBase.respond(kEventID_GetSpeakerVolume, &speaker, kEventStatus_Ok, true);
        }
    }
    else if (eventBase.eventId == kEventID_SetSpeakerVolume)
    {
        event_common_t event               = *(event_common_t *)data;
        event_status_t eventResponseStatus = kEventStatus_Ok;
        if (kSLNConfigStatus_Success != HAL_OutputDev_SmartLockConfig_SetSpeakerVolume(event.speakerVolume.volume))
        {
            error               = kStatus_HAL_OutputError;
            eventResponseStatus = kEventStatus_Error;
            LOGE("Failed to save speaker volume to flash");
        }
        else
        {
            s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value = event.speakerVolume.volume;
        }
        if (eventBase.respond != NULL)
        {
            eventBase.respond(kEventID_SetSpeakerVolume, &event.speakerVolume, eventResponseStatus, true);
        }
    }
    else
    {
        APP_OutputDev_MqsAudio_InputNotifyDecode(dev, data, (void *)&audioBuffer, &audioLen);
        if (audioBuffer != NULL && audioLen != 0)
        {
            MQS_SOUND_PLAY_FUNC(audioBuffer, audioLen);
        }
    }
    return error;
}

const static output_dev_event_handler_t s_MqsAudioHandler = {
    .inferenceComplete = HAL_OutputDev_MqsAudio_InferComplete,
    .inputNotify       = HAL_OutputDev_MqsAudio_Input_notify,
};

static hal_output_status_t HAL_OutputDev_MqsAudio_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    if (FWK_OutputManager_RegisterEventHandler(dev, &s_MqsAudioHandler) != 0)
    {
        error = kStatus_HAL_OutputError;
    }

    _ConfigMqs();

    s_EventFinished = xEventGroupCreate();
    if (s_EventFinished == NULL)
    {
        error = kStatus_HAL_OutputError;
    }

#if DEFER_PLAYBACK_TO_TASK
    s_MqsAudioTask.task.msgHandle  = HAL_OutputDev_MqsAudio_MsgHandle;
    s_MqsAudioTask.task.taskInit   = NULL;
    s_MqsAudioTask.task.data       = (fwk_task_data_t *)&(s_MqsAudioTask.data);
    s_MqsAudioTask.task.taskId     = MQS_AUDIO_TASK_ID;
    s_MqsAudioTask.task.delayMs    = 1;
    s_MqsAudioTask.task.taskStack  = s_MqsAudioTaskStack;
    s_MqsAudioTask.task.taskBuffer = s_MqsAudioTaskTcbReference;
    s_MqsAudioTask.data.dev        = dev;
    FWK_Task_Start((fwk_task_t *)&s_MqsAudioTask, MQS_TASK_NAME, MQS_TASK_STACK, MQS_TASK_PRIORITY);
#endif

    // TODO: Update 'HAL_OutputDev_SmartLockConfig_Get...' function to be one getter/setter
    s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value = HAL_OutputDev_SmartLockConfig_GetSpeakerVolume();
    return error;
}

static hal_output_status_t HAL_OutputDev_MqsAudio_Init(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    sai_config_t saiConfig                               = {0};
    sai_transfer_format_t saiAmpFormat                   = {0};
    edma_channel_Preemption_config_t dmaPreemptionConfig = {0};
    edma_config_t dmaConfig                              = {0};

    BOARD_InitMQSResource();
    saiAmpFormat.bitWidth           = kSAI_WordWidth16bits;
    saiAmpFormat.channel            = 0U;
    saiAmpFormat.protocol           = kSAI_BusLeftJustified;
    saiAmpFormat.sampleRate_Hz      = kSAI_SampleRate48KHz;
    saiAmpFormat.isFrameSyncCompact = true;
    saiAmpFormat.stereo             = kSAI_Stereo;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    saiAmpFormat.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif

    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(MQS_DMA, &dmaConfig);
    EDMA_CreateHandle(&s_SaiDmaHandle, MQS_DMA, MQS_SAI_EDMA_TX_CH);

    dmaPreemptionConfig.enableChannelPreemption = true;
    dmaPreemptionConfig.enablePreemptAbility    = false;
    dmaPreemptionConfig.channelPriority         = 0;
    EDMA_SetChannelPreemptionConfig(MQS_DMA, MQS_SAI_EDMA_TX_CH, &dmaPreemptionConfig);

    DMAMUX_Init(MQS_DMAMUX);
    DMAMUX_SetSource(MQS_DMAMUX, MQS_SAI_EDMA_TX_CH, MQS_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(MQS_DMAMUX, MQS_SAI_EDMA_TX_CH);

    NVIC_SetPriority(DMA0_DMA16_IRQn, MQS_SAI_DMA_IRQ_PRIO);

    /* Initialize SAI Tx */
    SAI_TxGetDefaultConfig(&saiConfig);
    saiConfig.protocol = kSAI_BusLeftJustified;
    SAI_TxInit(MQS_SAI, &saiConfig);

    SAI_TransferTxCreateHandleEDMA(MQS_SAI, &s_SaiTxHandle, _SaiCallback, NULL, &s_SaiDmaHandle);
    SAI_TransferTxSetFormatEDMA(MQS_SAI, &s_SaiTxHandle, &saiAmpFormat, MQS_SAI_CLK_FREQ, MQS_SAI_CLK_FREQ);

    /* Force bit clock to override standard enablement */
    SAI_TxSetBitClockRate(MQS_SAI, MQS_SAI_CLK_FREQ, saiAmpFormat.sampleRate_Hz, saiAmpFormat.bitWidth, 1U);

    return error;
}

static status_t _GetVolume(char *valueToString)
{
    itoa(s_OutputDev_MqsAudio.configs[kMQSConfigs_Volume].value, valueToString, 10);
    strcat(valueToString, "%");
    return kStatus_Success;
}

int HAL_OutputDev_MqsAudio_Register()
{
    int error = 0;
    LOGD("HAL_OutputDev_MqsAudio_Register");
    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_MqsAudio);
    return error;
}
