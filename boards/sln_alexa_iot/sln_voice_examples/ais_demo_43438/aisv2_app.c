/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <string.h>
#include <time.h>

/* Board includes */
#include "board.h"

#include "ais_alerts.h"
#include "ais_streamer.h"
#include "aisv2_app.h"
#include "sln_flash.h"
#include "sln_flash_mgmt.h"
#include "sln_cfg_file.h"
#include "streamer_pcm.h"

/*! @brief Values for flow control buffer states. */
const char *ais_buffer_state_str[] = {
    "AIS_BUFFER_STATE_GOOD,",     "AIS_BUFFER_STATE_OVERRUN,",          "AIS_BUFFER_STATE_OVERRUN_WARNING,",
    "AIS_BUFFER_STATE_UNDERRUN,", "AIS_BUFFER_STATE_UNDERRUN_WARNING,", "AIS_BUFFER_STATE_INVALID"};

const char *ais_state_str[] = {"AIS_STATE_IDLE", "AIS_STATE_THINKING", "AIS_STATE_SPEAKING", "AIS_STATE_ALERTING",
                               "AIS_STATE_INVALID"};

const uint64_t kEpochAdjust = 2208988800ULL;

#include "audio_processing_task.h"
#include "limits.h"
#include "reconnection_task.h"
#include "task.h"
#include "ux_attention_system.h"
#include "sln_RT10xx_RGB_LED_driver.h"

extern TaskHandle_t appTaskHandle;
extern TaskHandle_t xUXAttentionTaskHandle;

static uint8_t *pu8PreambleData;
static uint32_t u32PreambleIdx  = 0;
static uint32_t u32PreambleSize = 0;

static TaskHandle_t s_alertsTask;
extern QueueHandle_t g_alertQueue;

TimerHandle_t s_thinkStateTimer = NULL;

__attribute__((section(".ocram_non_cacheable_data"))) ais_app_data_t appData;

static void vThinkStateTimerCallback(TimerHandle_t xTimer);

void AIS_AppInit(void)
{
    appData.state                     = AIS_STATE_IDLE;
    appData.prevState                 = AIS_STATE_IDLE;
    appData.currTime                  = 0;
    appData.prevTime                  = 0;
    appData.lastRefresh               = 0;
    appData.speakerOffsetStart        = 0;
    appData.speakerOffsetEnd          = 0;
    appData.speakerOffsetWritten      = 0;
    appData.speakerOpusFramesFlushed  = 0;
    appData.overrunSequence           = 0;
    appData.bargein                   = false;
    appData.speakerOpen               = false;
    appData.speakerOpenSent           = false;
    appData.speakerMarker.echoRequest = false;
    appData.speakerBufferState        = AIS_BUFFER_STATE_GOOD;

    s_alertsTask = AIS_Alerts_GetThisHandle();

    if (!s_thinkStateTimer)
    {
        s_thinkStateTimer = xTimerCreate("ThinkStateTimer",                               /* name */
                                         pdMS_TO_TICKS(AIS_APP_THINK_STATE_TIMEOUT_MSEC), /* period */
                                         pdFALSE,                                         /* self reload */
                                         (void *)0,                                       /* id */
                                         vThinkStateTimerCallback /* function to call when expiring */
        );

        if (!s_thinkStateTimer)
        {
            configPRINTF(("xTimerCreate failed\r\n"));
        }
    }
}

uint32_t AIS_AppCallback_Microphone(uint8_t **data, uint32_t *size)
{
    uint32_t ret = kStatus_NoTransferInProgress;

    if (appData.expectedTickTime < xTaskGetTickCount())
    {
        if (kMicCloudWakeVerifier == audio_processing_get_state())
        {
            ret = audio_processing_get_continuous_utterance(&pu8PreambleData, &u32PreambleSize, &u32PreambleIdx, size,
                                                            data);
        }
        else if (kMicRecording == audio_processing_get_state())
        {
            /* Needs to be an if else or the last of the wake word will be overwritten */
            ret = audio_processing_get_output_buffer(data, size);
        }
        appData.expectedTickTime = xTaskGetTickCount() + (portTICK_PERIOD_MS * 50);
    }

    return ret;
}

static void _SpeakerEchoMarker(ais_handle_t *handle)
{
    streamer_handle_t *streamer = (streamer_handle_t *)handle->audioPlayer;
    uint32_t buffered           = STREAMER_GetQueued(streamer);

    if (appData.speakerMarker.echoRequest)
    {
        uint64_t offsetReached;

        offsetReached = appData.speakerOffsetWritten - buffered;

        /* Send event when we have played back this data. */
        if (offsetReached >= appData.speakerMarker.offset)
        {
            AIS_EventSpeakerMarkerEncountered(handle, appData.speakerMarker.marker);
            appData.speakerMarker.echoRequest = false;
        }
    }
}

static void _SpeakerFlowControl(ais_handle_t *handle)
{
    streamer_handle_t *streamer = (streamer_handle_t *)handle->audioPlayer;
    uint32_t buffered           = STREAMER_GetQueued(streamer);

    bool dataComplete = false;
    if (appData.speakerOffsetEnd > 0 && (appData.speakerOffsetWritten >= appData.speakerOffsetEnd))
    {
        dataComplete = true;
    }

    if (STREAMER_IsPlaying(streamer))
    {
        uint32_t lastSeq = handle->topicSequence[AIS_TOPIC_SPEAKER] - 1;

        /* NOTE: Hard overflow is signaled and sent from overflow callback! */
        if (appData.speakerBufferState == AIS_BUFFER_STATE_OVERRUN)
        {
            return;
        }

        /* CASE 1: above overrun threshold */
        if (buffered >= AWS_AUDIO_BUFFER_OVERRUN_THRESHOLD && !dataComplete)
        {
            if (appData.speakerBufferState == AIS_BUFFER_STATE_GOOD)
            {
                appData.prevSpeakerBufferState = appData.speakerBufferState;
                appData.speakerBufferState     = AIS_BUFFER_STATE_OVERRUN_WARNING;
                AIS_EventBufferStateChanged(handle, appData.prevSpeakerBufferState, AIS_BUFFER_STATE_OVERRUN_WARNING,
                                            lastSeq);
            }
        }
        else
            /* CASE 2: below underrun threshold */
            if ((buffered <= AWS_AUDIO_BUFFER_UNDERRUN_THRESHOLD) && !dataComplete)
        {
            if (buffered == 0)
            {
                /* Hard underflow */
                if (appData.speakerBufferState == AIS_BUFFER_STATE_UNDERRUN_WARNING)
                {
                    appData.prevSpeakerBufferState = appData.speakerBufferState;
                    appData.speakerBufferState     = AIS_BUFFER_STATE_UNDERRUN;
                    AIS_EventBufferStateChanged(handle, appData.prevSpeakerBufferState, AIS_BUFFER_STATE_UNDERRUN,
                                                lastSeq);
                }
            }
            else
            {
                /* Below underrun threshold, but not hard underflow */
                if (appData.speakerBufferState == AIS_BUFFER_STATE_GOOD ||
                    appData.speakerBufferState == AIS_BUFFER_STATE_UNDERRUN)
                {
                    appData.prevSpeakerBufferState = appData.speakerBufferState;
                    appData.speakerBufferState     = AIS_BUFFER_STATE_UNDERRUN_WARNING;
                    AIS_EventBufferStateChanged(handle, appData.prevSpeakerBufferState,
                                                AIS_BUFFER_STATE_UNDERRUN_WARNING, lastSeq);
                }
            }
        }
        /* CASE 3: above underrun threshold, below overrun threshold */
        else
        {
            /* Buffer in good state. */
            if (appData.speakerBufferState != AIS_BUFFER_STATE_GOOD &&
                appData.speakerBufferState != AIS_BUFFER_STATE_OVERRUN)
            {
                configPRINTF(("Previous speaker buffer state: %s new state: %s buffered: %d\r\n",
                              ais_buffer_state_str[appData.speakerBufferState],
                              ais_buffer_state_str[AIS_BUFFER_STATE_GOOD], buffered));

                appData.prevSpeakerBufferState = appData.speakerBufferState;
                appData.speakerBufferState     = AIS_BUFFER_STATE_GOOD;
            }
        }
    }
    else /* streamer not playing */
    {
        if (buffered >= AWS_AUDIO_BUFFER_OVERRUN_THRESHOLD)
        {
            /* Above overrun threshold */
            if (appData.speakerBufferState != AIS_BUFFER_STATE_GOOD)
            {
                /* If we are in barge-in state, pull the number of bytes and throw half of them away if overflow */
                if ((audio_processing_get_state() == kMicRecording))
                {
                    uint32_t queued = STREAMER_GetQueued(streamer);

                    uint32_t numberOfFrames = (queued / STREAMER_PCM_OPUS_FRAME_SIZE);
                    uint32_t bytesToDrop    = ((numberOfFrames / 2) * STREAMER_PCM_OPUS_FRAME_SIZE);

                    STREAMER_Read(NULL, bytesToDrop);
                }
            }
        }
        else
        {
            /* Buffer in good state. */
            if (appData.speakerBufferState != AIS_BUFFER_STATE_GOOD)
            {
                configPRINTF(("Previous speaker buffer state: %s new state: %s buffered: %d\r\n",
                              ais_buffer_state_str[appData.speakerBufferState],
                              ais_buffer_state_str[AIS_BUFFER_STATE_GOOD], buffered));
                appData.prevSpeakerBufferState = appData.speakerBufferState;
                appData.speakerBufferState     = AIS_BUFFER_STATE_GOOD;
            }
        }
    }
}

static void _SpeakerFlushedKeepTrack(uint32_t dataFlushed)
{
    configPRINTF(("Flushed %lu bytes from the streamer!\r\n", dataFlushed));

    /* Remember what we flushed to use info at next interaction in OpenSpeaker */
    appData.speakerOpusFramesFlushed = dataFlushed / STREAMER_PCM_OPUS_FRAME_SIZE;

    /* Subtract the offset by the remaining data flushed from the streamer *
     * This is to ensure the offsets are in sync as the data we just flushed
     * from the streamer hasn't been played */
    appData.speakerOffsetWritten -= (appData.speakerOpusFramesFlushed * STREAMER_PCM_OPUS_DATA_SIZE);
}

static void _SpeakerStop(ais_handle_t *handle)
{
    streamer_handle_t *streamer = (streamer_handle_t *)handle->audioPlayer;

    /* Check for EOS from the streamer. */
    if (streamer->eos)
    {
        streamer->eos = false;
        /* Stop the streamer */
        configPRINTF(("[AIS App] Stopping streamer playback\r\n"));
        STREAMER_Stop(streamer);

        /* Send SpeakerClosed event response if we have safely finished */
        if ((appData.speakerOffsetEnd > 0) && (appData.speakerOffsetWritten >= appData.speakerOffsetEnd))
        {
            appData.speakerOpen     = false;
            appData.speakerOpenSent = false;
            AIS_EventSpeakerClosed(handle, appData.speakerOffsetWritten);

            configPRINTF(("[AIS App] Exiting Speaker State\r\n"));
            AIS_ClearState(handle, AIS_TASK_STATE_SPEAKER);

            /* Reset streamer playback state. */
            appData.speakerBufferState = AIS_BUFFER_STATE_GOOD;

            /* Return to default CPU speed when Idle */
            BOARD_RevertClock();
        }
    }
    else if (appData.bargein && appData.speakerOpen)
    {
        if (STREAMER_IsPlaying(streamer))
        {
            /* Stop the streamer */
            configPRINTF(("[AIS App] Stopping streamer playback.\r\n"));

            uint32_t flushedSize = STREAMER_Stop(streamer);
            if (flushedSize > 0)
            {
                /* Save offsets to use on next interaction */
                _SpeakerFlushedKeepTrack(flushedSize);
            }
        }

        /* Update the speaker flag and offsets */
        appData.bargein         = false;
        appData.speakerOpen     = false;
        appData.speakerOpenSent = false;

        if (appData.speakerOffsetWritten == appData.speakerOffsetStart)
        {
            /* TODO remove - add one for the good faith */
            appData.speakerOffsetWritten++;
        }
        appData.speakerOffsetEnd = appData.speakerOffsetWritten;

        /* Send SpeakerClosed event immediately */
        AIS_EventSpeakerClosed(handle, appData.speakerOffsetWritten);
    }
}

static void _SpeakerStart(ais_handle_t *handle)
{
    streamer_handle_t *streamer = (streamer_handle_t *)handle->audioPlayer;
    uint32_t totalExpected      = 0;

    if (appData.speakerOpen)
    {
        if (appData.speakerOffsetEnd > 0)
        {
            /* Calculate total amount of bytes received in this audio stream. */
            totalExpected = appData.speakerOffsetEnd - appData.speakerOffsetStart;
        }

        /* Check for conditions to begin streamer playback. */
        if (!STREAMER_IsPlaying(streamer) && (audio_processing_get_state() == kIdle))
        {
            uint32_t buffered = STREAMER_GetQueued(streamer);

            bool startThresholdMet = buffered > AWS_AUDIO_START_THRESHOLD;
            bool allDataCollected  = totalExpected && (buffered >= totalExpected);
            bool endOffsetMet =
                (appData.speakerOffsetEnd > 0) && (appData.speakerOffsetWritten >= appData.speakerOffsetEnd);

            if (startThresholdMet || /* Network buffer threshold has been exceeded */
                allDataCollected ||  /* All data collected already in the buffer */
                endOffsetMet)        /* All data received, based on offsets */
            {
                configPRINTF(("[AIS App] Starting streamer playback.\r\n"));
                STREAMER_Start(streamer);
                STREAMER_SetVolume(appData.volume);

                /* Avoid sending multiple SpeakerOpened messages during a playback */
                if (!appData.speakerOpenSent)
                {
                    /* Send speaker opened message with the start offset of playback */
                    AIS_EventSpeakerOpened(handle, appData.speakerOffsetStart);
                    appData.speakerOpenSent = true;
                }
            }
        }
    }
}

void AIS_AppCallback_SpeakerState(ais_handle_t *handle)
{
    AIS_State_Lock(handle);

    /* Check marker for sending SpeakerMarkerEncountered. */
    _SpeakerEchoMarker(handle);

    /* Check for audio buffer over/underrun issues. */
    _SpeakerFlowControl(handle);

    /* Check if playback is stopped or needs to - due to EOS or barge-in */
    _SpeakerStop(handle);

    /* Check if playback needs to start */
    _SpeakerStart(handle);

    AIS_State_Unlock(handle);
}

/* Return available audio buffer space. */
uint32_t AIS_AppCallback_SpeakerAvailableBuffer(ais_handle_t *handle)
{
    streamer_handle_t *streamer = (streamer_handle_t *)handle->audioPlayer;
    uint32_t buffered;

    buffered = STREAMER_GetQueued(streamer);

    return (AWS_AUDIO_BUFFER_SIZE - buffered);
}

void AIS_AppCallback_SpeakerOverflow(ais_handle_t *handle, uint32_t sequence)
{
    streamer_handle_t *streamer = (streamer_handle_t *)handle->audioPlayer;

    configPRINTF(("[AIS App] Speaker overflow detected, sequence: %d\r\n", sequence));

    /* send the overrun message directly from here */
    /* Need to send Overflow regardless of the previous state. This is a critical point */
    AIS_EventBufferStateChanged(handle, appData.speakerBufferState, AIS_BUFFER_STATE_OVERRUN, sequence);

    AIS_State_Lock(handle);

    appData.prevSpeakerBufferState = appData.speakerBufferState;
    appData.speakerBufferState     = AIS_BUFFER_STATE_OVERRUN;
    appData.overrunSequence        = sequence;

    if (!STREAMER_IsPlaying(streamer))
    {
        uint32_t queued = STREAMER_GetQueued(streamer);

        uint32_t numberOfFrames = (queued / STREAMER_PCM_OPUS_FRAME_SIZE);
        uint32_t bytesToDrop    = ((numberOfFrames / 2) * STREAMER_PCM_OPUS_FRAME_SIZE);

        STREAMER_Read(NULL, bytesToDrop);
    }

    AIS_State_Unlock(handle);
}

/* When this function is executed, there is enough space to buffer the data. */
void AIS_AppCallback_Speaker(ais_handle_t *handle, uint8_t *data, uint32_t size, uint64_t offset, uint8_t count)
{
    /* TODO: don't write data to the streamer if the offset is before the
     * OpenSpeaker value.  Need to buffer 3-5 message sequences for reorder and
     * throwaway detection. */

    uint32_t currentOffset = 0;
    uint32_t frameSize     = 0;
    uint32_t currentCount  = 0;

    AIS_State_Lock(handle);

    /* While there are still OPUS frames to process, keep pushing into the streamer with size */
    while (count >= currentCount)
    {
        /* Shift the offset to start processing the next frame */
        currentOffset += frameSize;
        frameSize = (size - sizeof(offset)) / (count + 1);

        if (handle->config->speakerDecoder == AIS_SPEAKER_DECODER_OPUS)
        {
            /* Write the size of the packet to the streamer. */
            STREAMER_Write((uint8_t *)&frameSize, sizeof(uint32_t));
        }

        /* Write the actual data shifted with the next offset
         * The frame size is normally 160 bytes but the count divisor should figure this out */
        STREAMER_Write(data + currentOffset, frameSize);

        /* Shift the new offset with the size we just wrote */
        offset += frameSize;

        /* Increment the count to indicate we processed this frame */
        currentCount++;
    }

    appData.speakerOffsetWritten = offset;

    AIS_State_Unlock(handle);
}

void AIS_AppCallback_SpeakerMarker(ais_handle_t *handle, uint32_t marker)
{
    configPRINTF(("[AIS App] Speaker Marker Received: %d\r\n", marker));

    AIS_State_Lock(handle);

    appData.speakerMarker.marker      = marker;
    appData.speakerMarker.offset      = appData.speakerOffsetWritten;
    appData.speakerMarker.echoRequest = true;

    AIS_State_Unlock(handle);
}

void AIS_AppCallback_OpenSpeaker(ais_handle_t *handle, uint64_t offset)
{
    int64_t bytesToDrop               = 0;
    uint64_t u64SpeakerStreamerOffset = ((offset / STREAMER_PCM_OPUS_DATA_SIZE) * STREAMER_PCM_OPUS_FRAME_SIZE);
    uint64_t u64SpeakerStreamerEnd =
        ((appData.speakerOffsetEnd / STREAMER_PCM_OPUS_DATA_SIZE) * STREAMER_PCM_OPUS_FRAME_SIZE);
    uint32_t u32SpeakerFlushedSize = appData.speakerOpusFramesFlushed * STREAMER_PCM_OPUS_FRAME_SIZE;

    /* Discard any data prior to 'offset'. Also consider what we have flushed at barge-in */
    bytesToDrop = u64SpeakerStreamerOffset - u64SpeakerStreamerEnd - u32SpeakerFlushedSize;

    /* Need to dump all the speaker data from the previous request for barge-in use case
     * If u64SpeakerStreamerEnd is 0 then the last request reached the end so we are not in the barge-in use case*/
    if (bytesToDrop > 0 && u64SpeakerStreamerEnd > 0)
    {
        configPRINTF(("[AIS App] Dropping extra packets %lld\r\n", bytesToDrop));
        STREAMER_Read(NULL, bytesToDrop);
    }

    configPRINTF(("[AIS App] OpenSpeaker offset: %llu\r\n", offset));

    appData.speakerOpen = true;
    /* This offset value is high, it doesn't know when you interrupted
     * the last message so it contains data that has been dropped. But we
     * need it's value so we can determine when the interrupted message has ended */
    appData.speakerOffsetStart       = offset;
    appData.speakerOffsetEnd         = 0;
    appData.speakerOpusFramesFlushed = 0;

    /* Make sure CPU boost remains active */
    BOARD_BoostClock();

    /* Only go to idle if we aren't in a wakeup or mic sending state. This is a guard against
     * Messages that have been sent from the client and server side and they are in flight (timing)
     */
    if (audio_processing_get_state() == kMicStopRecording || audio_processing_get_state() == kMicStop)
    {
        audio_processing_set_state(kIdle);
    }
}

void AIS_AppCallback_CloseSpeaker(ais_handle_t *handle, uint64_t offset, bool immediate)
{
    if (!immediate)
    {
        configPRINTF(("[AIS App] CloseSpeaker offset: %llu\r\n", offset));
    }
    else
    {
        configPRINTF(("[AIS App] CloseSpeaker (immediately)\r\n"));
    }

    AIS_State_Lock(handle);
    /* Protect streamer updates */
    if (appData.speakerOpen)
    {
        if (!immediate)
        {
            appData.speakerOffsetEnd = offset;
        }
        else
        {
            /* Signal a barge-in situation */
            appData.bargein = true;
        }
    }
    /* Finished streamer updates */
    AIS_State_Unlock(handle);
}

void AIS_AppCallback_TerminateSpeaker(ais_handle_t *handle)
{
    streamer_handle_t *streamer = (streamer_handle_t *)handle->audioPlayer;

    AIS_State_Lock(handle);

    /* Stop the streamer and flush/ignore all data in flight  */
    configPRINTF(("Resetting streamer playback!\r\n"));
    STREAMER_Stop(streamer);

    appData.speakerOpen = false;
    AIS_ClearState(handle, AIS_TASK_STATE_SPEAKER);

    AIS_State_Unlock(handle);
}

/*!
 * @brief Set UX and Audioprocessing "idle" state (used for IDLE, DO_NOT_DISTURB, Speaking End, and
 * NOTIFICATION_AVAILABLE)
 *
 * @param uxState Target LED UX state to set for "idle" state
 * @param micState Target state of audio processing task
 */
static void set_device_idle_state(ux_attention_states_t uxState, audio_processing_states_t micState)
{
    ux_attention_set_state(uxState);
    audio_processing_set_state(micState);
}

uint32_t idleCount = 0U, thinkCount = 0U, speakCount = 0U;
void AIS_AppCallback_SetAttentionState(ais_state_t state, uint64_t offset, bool immediate)
{
    /* App TODO: handle service state as part of Alexa UX attention system. */

    if (!immediate)
    {
        configPRINTF(("[AIS App] SetAttentionState: %d, offset: %llu\r\n", state, offset));
        /* TODO: transition when 'offset' is hit in the audio stream. */
    }
    else
    {
        configPRINTF(("[AIS App] SetAttentionState: %d (immediately)\r\n", state));
    }

    appData.prevState = appData.state;
    appData.state     = state;

    if (AIS_STATE_IDLE == appData.state)
    {
        idleCount++;
        if (AIS_STATE_SPEAKING == appData.prevState)
        {
            // Set Speaking End idle state
            set_device_idle_state(uxSpeakingEnd, kIdle);
        }
        else
        {
            // Set true IDLE state
            set_device_idle_state(uxIdle, kIdle);
        }

        /* Return to default CPU speed when Idle */
        BOARD_RevertClock();
    }
    else if (AIS_STATE_THINKING == appData.state)
    {
        thinkCount++;
        ux_attention_set_state(uxThinking);

        if (s_thinkStateTimer && (xTimerReset(s_thinkStateTimer, 0) != pdPASS))
        {
            configPRINTF(("xTimerReset failed\r\n"));
        }
    }
    else if (AIS_STATE_SPEAKING == appData.state)
    {
        speakCount++;
        ux_attention_set_state(uxSpeaking);
    }
    else if (AIS_STATE_ALERTING == appData.state)
    {
        AIS_Alerts_Trigger(0, FALSE);
        configPRINTF(("[AIS] Alerting!\r\n"));
    }
    else if (AIS_STATE_DO_NOT_DISTURB == appData.state)
    {
        // Idle state to "DO_NOT_DISTURB"
        set_device_idle_state(uxDoNotDisturb, kIdle);
    }
    else if (AIS_STATE_NOTIFICATION == appData.state)
    {
        // Idle state to "NOTIFICATION_AVAILABLE"
        set_device_idle_state(uxNotificationQueued, kIdle);
    }
    else if (AIS_STATE_INVALID == appData.state)
    {
        // Handle invalid state by returning to idle
        set_device_idle_state(uxIdle, kIdle);
    }
}

void AIS_AppCallback_SetVolume(ais_handle_t *handle, uint32_t volume, uint64_t offset, bool immediate)
{
    int32_t status    = SLN_FLASH_MGMT_OK;
    uint32_t len      = 0;
    sln_dev_cfg_t cfg = DEFAULT_CFG_VALUES;

    if (!immediate)
    {
        configPRINTF(("[AIS App] SetVolume: %d, offset: %llu\r\n", volume, offset));

        /* TODO: transition when 'offset' is hit in the audios tream. */
    }
    else
    {
        configPRINTF(("[AIS App] SetVolume: %d (immediately)\r\n", volume));
    }

    if (appData.volume != volume)
    {
        ux_attention_set_state(uxDeviceChange);
    }

    STREAMER_SetVolume(volume);
    appData.volume = volume;

    status = SLN_FLASH_MGMT_Read(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, &len);

    if (SLN_FLASH_MGMT_OK == status)
    {
        if (appData.volume != cfg.streamVolume)
        {
            cfg.streamVolume = appData.volume;
            status           = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));

            if ((SLN_FLASH_MGMT_EOVERFLOW == status) || (SLN_FLASH_MGMT_EOVERFLOW2 == status))
            {
                SLN_FLASH_MGMT_Erase(DEVICE_CONFIG_FILE_NAME);

                status = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));
            }
        }
    }
    else if (SLN_FLASH_MGMT_ENOENTRY2 == status)
    {
        // We should have an empty file so we can save a new one
        cfg.streamVolume = appData.volume;
        status           = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));
    }

    if (SLN_FLASH_MGMT_OK != status)
    {
        configPRINTF(("[AIS App] ERROR: Unable to store stream volume %d to NVM\r\n", volume));
        configPRINTF(("[AIS App] ERROR: NVM operation returned with %d\r\n", status));
    }

    // Notify app task to send VolumeChanged
    xTaskNotify(appTaskHandle, kVolChanged, eSetBits);
}

void AIS_AppCallback_SetAlertVolume(uint32_t volume)
{
    int32_t status    = SLN_FLASH_MGMT_OK;
    uint32_t len      = 0;
    sln_dev_cfg_t cfg = DEFAULT_CFG_VALUES;

    appData.alertVolume = volume;
    configPRINTF(("[AIS App] SetAlertVolume: %d\r\n", volume));

    status = SLN_FLASH_MGMT_Read(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, &len);

    if (SLN_FLASH_MGMT_OK == status)
    {
        if (appData.alertVolume != cfg.alertVolume)
        {
            cfg.alertVolume = appData.alertVolume;
            status          = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));

            if ((SLN_FLASH_MGMT_EOVERFLOW == status) || (SLN_FLASH_MGMT_EOVERFLOW2 == status))
            {
                SLN_FLASH_MGMT_Erase(DEVICE_CONFIG_FILE_NAME);

                status = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));
            }
        }
    }
    else if (SLN_FLASH_MGMT_ENOENTRY2 == status)
    {
        // We should have an empty file so we can save a new one
        cfg.alertVolume = appData.alertVolume;
        status          = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, sizeof(sln_dev_cfg_t));
    }

    if (SLN_FLASH_MGMT_OK != status)
    {
        configPRINTF(("[AIS App] ERROR: Unable to store alert volume %d to NVM\r\n", volume));
        configPRINTF(("[AIS App] ERROR: NVM operation returned with %d\r\n", status));
    }

    // Notify app task to send AlertVolumeChanged
    xTaskNotify(appTaskHandle, kAlertVolChanged, eSetBits);
}

void AIS_AppCallback_SetClock(uint64_t time)
{
    /* App TODO: store clock value and use for timer/alerts. */

    appData.prevTime          = appData.currTime;
    appData.currTime          = time;
    appData.lastRefresh       = appData.currTime;
    appData.syncClockReceived = true;

    // configPRINTF(("[AIS App] SetClock: %llu\r\n", appData.currTime));

    // Adjust epoch to 1970 base year
    time_t epoch       = appData.currTime - kEpochAdjust;
    struct tm *timeNow = (struct tm *)pvPortMalloc(sizeof(struct tm));
    configPRINTF(("[AIS App] SetClock: %s\r\n", asctime(gmtime(&epoch))));
    vPortFree(timeNow);
    timeNow = NULL;
}

void AIS_AppCallback_SetAlert(const char *token,
                              uint64_t scheduledTime,
                              uint32_t durationInMilliseconds,
                              ais_alert_type_t type)
{
    ais_alert_t alert = {0};
    memcpy(alert.token, token, safe_strlen(token, AIS_MAX_ALERT_TOKEN_LEN_BYTES));
    alert.scheduledTime = scheduledTime;
    alert.durationMs    = durationInMilliseconds;
    alert.type          = type;
    alert.valid         = true;
    alert.idle          = true;

    int32_t status = AIS_Alerts_SaveAlert(&alert);

    if (0 < status)
    {
        // Existing Alert has been triggered by AIS [status is alert index + 1]
        uint32_t alertIdx = status - 1;

        // Send notification to ux
        AIS_Alerts_Trigger(alertIdx, false);

        // Mark triggered in memory
        AIS_Alerts_MarkAsTriggered(alertIdx);

        configPRINTF(("[AIS App] Alert Triggered: %s\r\n", token));

        // Push token to queue to read from app task
        xQueueSend(g_alertQueue, token, (TickType_t)0);

        // Notify app task of online trigger
        xTaskNotify(appTaskHandle, kAlertOnlineTrigger, eSetBits);
    }
    else if (0 == status)
    {
        configPRINTF(("[AIS App] SetAlert process success: %s\r\n", token));

        // Push token to queue to read from app task
        xQueueSend(g_alertQueue, token, (TickType_t)0);

        // Notify alerts task to save to NVM
        xTaskNotify(s_alertsTask, kNewAlertSet, eSetBits);

        // Notify app task of success
        xTaskNotify(appTaskHandle, kNewAlertSet, eSetBits);
    }
    else
    {
        configPRINTF(("[AIS App] SetAlert process failure: %s\r\n", token));

        // Push token to queue to read from app task
        xQueueSend(g_alertQueue, token, (TickType_t)0);

        // Notify app task of failure
        xTaskNotify(appTaskHandle, kNewAlertFail, eSetBits);
    }
}

void AIS_AppCallback_DeleteAlert(const char *token)
{
    /* App TODO: Delete alert from local memory. */

    configPRINTF(("[AIS App] DeleteAlert: %s\r\n", token));

    size_t len     = safe_strlen(token, AIS_MAX_ALERT_TOKEN_LEN_BYTES);
    int32_t status = AIS_Alerts_MarkForDelete(token, len);

    // Push token to queue to read from app task
    xQueueSend(g_alertQueue, token, (TickType_t)0);

    if (0 == status)
    {
        // Notify alerts task to remove from NVM
        xTaskNotify(s_alertsTask, kAlertDelete, eSetBits);

        // Notify app task of success
        xTaskNotify(appTaskHandle, kAlertDelete, eSetBits);
    }
    else
    {
        // Notify app task of failure
        xTaskNotify(appTaskHandle, kFailDelete, eSetBits);
    }
}

void AIS_AppCallback_OpenMicrophone(uint32_t timeout, const char *type, const char *token)
{
    /* App TODO: AIS requests mic input.
     * Respond with MicrophoneOpened within 'timeout' time, or send
     * OpenMicrophoneTimedOut. Pass the 'type' and 'token' parameters back with
     * the MicrophoneOpened request, if non-NULL. */
    audio_processing_set_state(kMicKeepOpen);
    /* Make sure CPU boost remains active */
    BOARD_BoostClock();

    // configPRINTF(("[AIS App] OpenMicrophone timeout: %d\r\n", timeout));
}

void AIS_AppCallback_CloseMicrophone(ais_handle_t *handle)
{
    /* App TODO: AIS requests close mic input. */

    /* XXX: need mutex around the changing of this in mic callback.
     * If we modify this here while state machine is updating, can become
     * non-zero before the start of another microphone send. */
    appData.bargein = false;
    audio_processing_set_state(kMicStopRecording);
}

void AIS_AppCallback_Exception(const char *code, const char *description)
{
    /* App TODO: handle exception error data. */

    configPRINTF(("[AIS App] Exception: %s: %s\r\n", code, description));
}

void AIS_AppError(ais_status_t error)
{
    /* App TODO: handle AIS error indication.
     * Microphone errors will transition AIS to IDLE state.  All others are
     * critical and require a restart of the service. */
}

void AIS_AppRegistrationInfo(ais_reg_config *registrationInfo)
{
    uint32_t status = kStatus_Success;

    /* Write shared secret to hyperflash. */
    status = SLN_FLASH_MGMT_Save(AIS_REGISTRATION_INFO_FILE_NAME, (uint8_t *)registrationInfo, sizeof(ais_reg_config));
    if (kStatus_Success == status)
    {
        configPRINTF(("[AIS App] Registration info saved to flash\r\n"));
    }
    else
    {
        configPRINTF(("[AIS App] Error: %d, Registration secret key could not be saved to flash\r\n", status));
    }
}

void AIS_Deregister(void)
{
    configPRINTF(("Starting factory reset ...\r\n"));

    RGB_LED_SetColor(LED_COLOR_ORANGE);

    /* Factory reset stuff */
    SLN_FLASH_MGMT_Erase(AIS_REGISTRATION_INFO_FILE_NAME);
    SLN_FLASH_MGMT_Erase(AIS_ALERT_FILE_NAME);
#if USE_WIFI_CONNECTION
    SLN_FLASH_MGMT_Erase(WIFI_CRED_FILE_NAME);
#endif

    /* a bit of delay to let everyone admire the beautiful orange LED */
    osa_time_delay(2000);

    NVIC_SystemReset();
}

void AIS_AppDisconnected(ais_handle_t *handle, ais_disconnect_code_t code, const char *description)
{
    bool reconnect = false;
    /* Signal reconnection task that disconnect has occurred */
    switch (code)
    {
        case AIS_DISCONNECT_NONE:
            break;
        case AIS_DISCONNECT_INVALID_SEQUENCE:
            reconnect = reconnection_task_set_event(kReconnectInvalidSequence);
            break;

#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
        case AIS_DISCONNECT_MESSAGE_TAMPERED:
            reconnect = reconnection_task_set_event(kReconnectMessageTampered);
            break;
#endif

        case AIS_DISCONNECT_API_DEPRECATED:
            reconnect = reconnection_task_set_event(kReconnectAPIDeprecated);
            break;

        case AIS_DISCONNECT_ENCRYPTION_ERROR:
            reconnect = reconnection_task_set_event(kReconnectEncryptionError);
            break;

        case AIS_DISCONNECT_GOING_OFFLINE:
        default:
            reconnect = reconnection_task_set_event(kReconnectAISDisconnect);
            break;
    }

    if (reconnect)
    {
        configPRINTF(("[AIS App] Disconnected from service: %s\r\n", description));

        handle->pendDisconnectSent = true;
    }
}

ais_app_data_t *AIS_APP_GetAppData(void)
{
    return &appData;
}

/**
 * @brief Function called when the registered timer's timeout is triggered.
 *
 * @param xTimer[in]    Pointer to the timer structure
 * @return              Void
 */
static void vThinkStateTimerCallback(TimerHandle_t xTimer)
{
    if (AIS_STATE_THINKING == appData.state)
    {
        if (reconnection_task_set_event(kReconnectAISDisconnect))
        {
            /* still in thinking state when the timeout expired, trigger reconnect */
            configPRINTF(("Thinking state timeout, triggering a reconnection\r\n"));
        }
    }
}
