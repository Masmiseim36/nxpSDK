/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "audio_processing_task.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "board.h"
#include "event_groups.h"
#include "limits.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

/* Freescale includes. */
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "pdm_pcm_definitions.h"

#include "network_connection.h"

#include "ais_continuous_utterance.h"
#define SLN_Voice
#include "sln_intelligence_toolbox.h"

#include "sln_flash_mgmt.h"
#include "sln_cfg_file.h"

extern uint32_t SLN_AMAZON_WAKE_Initialize();
extern void SLN_AMAZON_WAKE_SetWakeupDetectedParams(uint8_t *pu8Wake, uint16_t *pu16WWLen);
extern uint32_t SLN_AMAZON_WAKE_ProcessWakeWord(int16_t *pi16AudioBuff, uint16_t u16BufferSize);

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RUN_GENERATED_TEST (0U)
#define BUFFER_SIZE (PCM_SAMPLE_COUNT * 3)
#define BUFFER_NUM (4)

#define AUDIO_QUEUE_NUM_ITEMS 75U
#define AUDIO_QUEUE_WATERMARK 15U
#define AUDIO_QUEUE_ITEM_LEN_BYTES (PCM_SAMPLE_SIZE_BYTES * PCM_SINGLE_CH_SMPL_COUNT)
#define AUDIO_QUEUE_WTRMRK_BYTES (AUDIO_QUEUE_WATERMARK * AUDIO_QUEUE_ITEM_LEN_BYTES)
#define AUDIO_QUEUE_LENGTH_BYTES (AUDIO_QUEUE_NUM_ITEMS * AUDIO_QUEUE_ITEM_LEN_BYTES)

/*******************************************************************************
 * Global Vars
 ******************************************************************************/

static TaskHandle_t s_appTask;

static SemaphoreHandle_t s_pushCtr;

SDK_ALIGN(uint8_t __attribute__((section(".ocram_cacheable_data"))) g_w8ExternallyAllocatedMem[(173 * 1024)], 8);

static TaskHandle_t s_thisTaskHandle                    = NULL;
static audio_processing_states_t s_audioProcessingState = kIdle;
static pcmPingPong_t *s_micInputStream;
static int16_t *s_ampInputStream;
static uint32_t s_numItems           = 0;
static uint32_t s_waterMark          = 0;
static uint32_t s_outputIndex        = 0;
static mic_mute_mode_t s_micMuteMode = kMicMuteModeOff;
__attribute__((section(".ocram_non_cacheable_data"))) static uint8_t s_outputStream[AUDIO_QUEUE_LENGTH_BYTES];

uint8_t *cloud_buffer;
uint32_t cloud_buffer_len = 0;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*! @brief Called to reset the index's to ensure old mic data isn't resent. */
static void audio_processing_reset_mic_capture_buffers()
{
    s_numItems    = 0;
    s_waterMark   = 0;
    s_outputIndex = 0;
}

/*! @brief Called by task to push data */
static int32_t audio_processing_push_mic_data(uint8_t **data, uint32_t *size)
{
    if (AUDIO_QUEUE_ITEM_LEN_BYTES < *size)
    {
        return -2;
    }

    memcpy(&s_outputStream[s_numItems * AUDIO_QUEUE_ITEM_LEN_BYTES], *data, *size);

    s_numItems++;
    s_waterMark++;

    if (AUDIO_QUEUE_WATERMARK == s_waterMark)
    {
        s_waterMark = 0;

        if (AUDIO_QUEUE_WATERMARK == s_numItems)
        {
            xSemaphoreGive(s_pushCtr);
            // s_outputIndex = 0;
        }

        if (AUDIO_QUEUE_WATERMARK * 2 == s_numItems)
        {
            xSemaphoreGive(s_pushCtr);
            // s_outputIndex = AUDIO_QUEUE_WTRMRK_BYTES;
        }

        if (AUDIO_QUEUE_WATERMARK * 3 == s_numItems)
        {
            xSemaphoreGive(s_pushCtr);
            // s_outputIndex = AUDIO_QUEUE_WTRMRK_BYTES * 2;
        }

        if (AUDIO_QUEUE_WATERMARK * 4 == s_numItems)
        {
            xSemaphoreGive(s_pushCtr);
            // s_outputIndex = AUDIO_QUEUE_WTRMRK_BYTES * 2;
        }
        if (AUDIO_QUEUE_NUM_ITEMS == s_numItems)
        {
            s_numItems = 0;
            // s_outputIndex = AUDIO_QUEUE_WTRMRK_BYTES * 3;
            xSemaphoreGive(s_pushCtr);
        }
    }

    *size = 0;

    return 0;
}

void audio_processing_set_task_handle(TaskHandle_t *handle)
{
    s_thisTaskHandle = *handle;
}

TaskHandle_t audio_processing_get_task_handle(void)
{
    return s_thisTaskHandle;
}

void audio_processing_set_app_task_handle(TaskHandle_t *handle)
{
    if ((NULL != handle) && (NULL != *handle))
    {
        s_appTask = *handle;
    }
}

void audio_processing_set_mic_input_buffer(int16_t **buf)
{
    if ((NULL != buf) && (NULL != *buf))
    {
        s_micInputStream = (pcmPingPong_t *)(*buf);
    }
}

void audio_processing_set_amp_input_buffer(int16_t **buf)
{
    if ((NULL != buf) && (NULL != *buf))
    {
        s_ampInputStream = (int16_t *)(*buf);
    }
}

uint32_t audio_processing_get_continuous_utterance(
    uint8_t **outBuf, uint32_t *outLen, uint32_t *index, uint32_t *size, uint8_t **data)
{
    uint32_t ret = kStatus_Fail;

    if (*outBuf != NULL)
    {
        if ((AUDIO_QUEUE_WTRMRK_BYTES + *index) < *outLen)
        {
            *size = AUDIO_QUEUE_WTRMRK_BYTES;
            memcpy(*data, (uint8_t *)(*outBuf) + (*index), *size);
            *index += AUDIO_QUEUE_WTRMRK_BYTES;
        }
        else
        {
            *size = *outLen - *index;
            memcpy(*data, (uint8_t *)(*outBuf) + (*index), *size);
            *index  = 0;
            *outLen = 0;
            *outBuf = NULL;
            audio_processing_set_state(kMicRecording);
        }
        ret = kStatus_Success;
    }
    else
    {
        if (cloud_buffer != NULL || cloud_buffer_len > 0)
        {
            *outBuf = cloud_buffer;
            *outLen = cloud_buffer_len;
            *size   = AUDIO_QUEUE_WTRMRK_BYTES;
            memcpy(*data, (uint8_t *)(*outBuf), *size);
            *index = AUDIO_QUEUE_WTRMRK_BYTES;

            ret = kStatus_Success;
        }
    }

    return ret;
}

uint32_t audio_processing_get_output_buffer(uint8_t **outBuf, uint32_t *outLen)
{
    uint32_t ret = kStatus_InvalidArgument;

    if ((NULL != outBuf) && (NULL != outLen))
    {
        ret = kStatus_Timeout;

        if (xSemaphoreTake(s_pushCtr, (TickType_t)200) == pdTRUE)
        {
            *outLen = AUDIO_QUEUE_WTRMRK_BYTES;
            memcpy(*outBuf, (uint8_t *)&s_outputStream[s_outputIndex], *outLen);

            if (s_outputIndex >= AUDIO_QUEUE_WTRMRK_BYTES * 4)
            {
                s_outputIndex = 0;
            }
            else
            {
                s_outputIndex += AUDIO_QUEUE_WTRMRK_BYTES;
            }
            ret = kStatus_Success;
        }
    }

    return ret;
}

uint32_t audio_processing_get_wake_word_end(void)
{
    return cloud_buffer_len;
}

void audio_processing_set_state(audio_processing_states_t state)
{
    configPRINTF(("Setting audio processing state to %d\r\n", state));
    s_audioProcessingState = state;
}

audio_processing_states_t audio_processing_get_state(void)
{
    return s_audioProcessingState;
}

mic_mute_mode_t audio_processing_get_mic_mute(void)
{
    return s_micMuteMode;
}

uint32_t audio_processing_set_mic_mute(mic_mute_mode_t mute_mode, bool persistent)
{
    int32_t status    = SLN_FLASH_MGMT_OK;
    uint32_t len      = 0;
    sln_dev_cfg_t cfg = DEFAULT_CFG_VALUES;

    s_micMuteMode = mute_mode;

    /* stop here if it is not desired to save mute mode in flash */
    if (true == persistent)
    {
        /* make the mute settings persistent by saving them in flash */
        status = SLN_FLASH_MGMT_Read(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, &len);

        if (SLN_FLASH_MGMT_OK == status)
        {
            /* theoretically this should always be true */
            if (s_micMuteMode != cfg.mic_mute_mode)
            {
                cfg.mic_mute_mode = s_micMuteMode;
                status = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));

                if ((SLN_FLASH_MGMT_EOVERFLOW == status) || (SLN_FLASH_MGMT_EOVERFLOW2 == status))
                {
                    SLN_FLASH_MGMT_Erase(DEVICE_CONFIG_FILE_NAME);
                    status = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));
                }
            }
        }
        else if (SLN_FLASH_MGMT_ENOENTRY2 == status)
        {
            /* We should have an empty file so we can save a new one */
            cfg.mic_mute_mode = s_micMuteMode;
            status            = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));
        }

        if (SLN_FLASH_MGMT_OK != status)
        {
            configPRINTF(("Failed to save mute mode in flash, error %d.\r\n", status));
        }
    }

    return status;
}

void audio_processing_task(void *pvParameters)
{
    uint8_t pingPongIdx        = 0;
    uint8_t *pu8CleanAudioBuff = NULL;
    int32_t status             = 0;
    uint32_t micRecordingLen   = 0;
    uint8_t pingPongAmpIdx     = 0;

    uint32_t taskNotification = 0U;
    uint32_t currentEvent     = 0U;

    uint8_t u8WakeWordActive = 0U;
    uint16_t wwLen           = 0;
    sln_afe_configuration_params_t afeConfig;

    s_pushCtr = xSemaphoreCreateCounting(2, 0);

    uint32_t reqSize = SLN_Voice_Req_Mem_Size();

    assert(sizeof(g_w8ExternallyAllocatedMem) >= reqSize);

    /* Make sure we memset the buffer to zero */
    audio_processing_reset_mic_capture_buffers();

    afeConfig.u16PostProcessedGain = 0x0600;
    afeConfig.u8NumberOfMics       = PDM_MIC_COUNT;

    status = SLN_Voice_Init(g_w8ExternallyAllocatedMem, &afeConfig);

    if (status != 1)
    {
        // Should not get here, should output some error
        while (1)
            ;
    }
    SLN_AMAZON_WAKE_Initialize();
    SLN_AMAZON_WAKE_SetWakeupDetectedParams(&u8WakeWordActive, &wwLen);

    while (1)
    {
        // Suspend waiting to be activated when receiving PDM mic data after Decimation
        xTaskNotifyWait(0U, ULONG_MAX, &taskNotification, portMAX_DELAY);

        // Figure out if it's a PING or PONG buffer received
        if (taskNotification & (1U << PCM_PING))
        {
            pingPongIdx    = 1U;
            pingPongAmpIdx = 1U;
            currentEvent   = (1U << PCM_PING);
        }

        if (taskNotification & (1U << PCM_PONG))
        {
            pingPongIdx    = 0U;
            pingPongAmpIdx = 0U;
            currentEvent   = (1U << PCM_PONG);
        }

        int16_t *pcmIn = (int16_t *)((*s_micInputStream)[pingPongIdx]);
        SLN_Voice_Process_Audio(g_w8ExternallyAllocatedMem, pcmIn,
                                &s_ampInputStream[pingPongAmpIdx * PCM_SINGLE_CH_SMPL_COUNT], &pu8CleanAudioBuff, NULL,
                                NULL);
        SLN_AMAZON_WAKE_ProcessWakeWord((int16_t *)pu8CleanAudioBuff, 320);
        taskNotification &= ~currentEvent;

        // If devices is muted, then skip over state machine
        if (s_micMuteMode)
        {
            if (u8WakeWordActive)
            {
                u8WakeWordActive = 0U;
            }

            memset(pu8CleanAudioBuff, 0x00, AUDIO_QUEUE_ITEM_LEN_BYTES);
        }

        if (u8WakeWordActive)
        {
            configPRINTF(("Wake word detected locally\r\n"));
            /* Boost CPU now for best performance */
            BOARD_BoostClock();
        }

        // Execute intended state
        switch (s_audioProcessingState)
        {
            case kIdle:

                /* add clean buff to cloud wake word ring buffer */
                continuous_utterance_samples_add(pu8CleanAudioBuff, PCM_SINGLE_CH_SMPL_COUNT * PCM_SAMPLE_SIZE_BYTES);
                if (u8WakeWordActive)
                {
                    continuous_utterance_buffer_set(&cloud_buffer, &cloud_buffer_len, wwLen);

                    u8WakeWordActive = 0U;
                    wwLen            = 0;
                    // Notify App Task Wake Word Detected
                    xTaskNotify(s_appTask, kWakeWordDetected, eSetBits);

                    // App Task will now determine if we begin recording/publishing data
                }

                break;

            case kMicKeepOpen:
                // Notify App_Task to indicate mic is back open
                xTaskNotify(s_appTask, kMicKeepOpen, eSetBits);
                s_audioProcessingState = kMicRecording;
            case kWakeWordDetected:

                audio_processing_reset_mic_capture_buffers();
                // Notify App_Task to indicate recording
                xTaskNotify(s_appTask, kMicRecording, eSetBits);

                if (s_audioProcessingState != kMicRecording)
                {
                    s_audioProcessingState = kMicCloudWakeVerifier;
                }

                configPRINTF(("[audio processing] Mic Recording Start.\r\n"));
                // Roll into next state

            case kMicCloudWakeVerifier:
            case kMicRecording:

                micRecordingLen = AUDIO_QUEUE_ITEM_LEN_BYTES;
                if (u8WakeWordActive)
                {
                    u8WakeWordActive = 0U;
                }

                // Push data into buffer for consumption by AIS task
                status = audio_processing_push_mic_data(&pu8CleanAudioBuff, &micRecordingLen);

                if (status)
                {
                    // TODO: error handle
                }

                break;

            case kMicStopRecording:
                // Set our internal state back to waiting
                s_audioProcessingState = kMicStop;

                // Signal to other tasks we have stopped
                xTaskNotify(s_appTask, kMicStop, eSetBits);

                // Clear counter
                uint32_t pushCnt = 0;
                pushCnt          = uxSemaphoreGetCount(s_pushCtr);
                while (pushCnt)
                {
                    xSemaphoreTake(s_pushCtr, (TickType_t)5);
                    pushCnt = uxSemaphoreGetCount(s_pushCtr);
                }

                // audio_processing_reset_mic_capture_buffers();

                configPRINTF(("[audio processing] Mic Recording Stopped.\r\n"));
                break;

            case kReconnect:
                s_audioProcessingState = kIdle;
                if (u8WakeWordActive)
                {
                    u8WakeWordActive = 0U;
                }
                break;
            default:
                break;
        }
        u8WakeWordActive = 0;
    }
}
