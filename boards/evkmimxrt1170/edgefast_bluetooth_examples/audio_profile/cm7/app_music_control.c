/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdint.h>
#include <porting.h>
#include "osa_common.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "app_streamer.h"
#include "streamer_pcm.h"
#include "ff.h"
#include "diskio.h"
#include "app_music_control.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/a2dp.h>
#include "app_a2dp.h"
#include "app_common.h"
#include "cmd.h"

enum _app_a2dp_speaker_state_t
{
    A2DP_SPEAKER_NOT_PRESENCE = 0,
    A2DP_SPEAKER_READY,
    A2DP_SPEAKER_PLAYING,
};

enum _app_streamer_state_t
{
    APP_STREAMER_NOT_STARTED = 0,
    APP_STREAMER_PLAYING,
    APP_STREAMER_PAUSE,
};

static uint8_t app_music_name_buffer[SUPPORT_MAX_MUSIC_FILE_COUNT][SUPPORT_MAX_MUSIC_NAME_LENGTH];
static uint32_t app_music_count;
static uint32_t app_plan_play_index;
static uint32_t app_playing_index;
static uint8_t app_playing_state;
static uint8_t app_a2dp_speaker_state;
static streamer_handle_t app_streamer_handle;
static QueueHandle_t app_music_control_queue = NULL;
extern void streamer_pcm_test_init(void);
extern void streamer_pcm_test_send_data(uint8_t *buffer, uint32_t length);

extern void app_music_shadow_update(uint8_t playIndexSet, uint8_t playStateSet, uint8_t playerReadySet);

static void app_music_list_reset(void)
{
    app_music_count = 0U;
    app_playing_index = 0U;
    app_playing_state = APP_STREAMER_NOT_STARTED;
    for (uint32_t i = 0; i < SUPPORT_MAX_MUSIC_FILE_COUNT * SUPPORT_MAX_MUSIC_NAME_LENGTH; ++i)
    {
        ((uint8_t *)&(app_music_name_buffer[0][0]))[i] = 0U;
    }
}

static void app_music_reset(void)
{
    app_a2dp_speaker_state = A2DP_SPEAKER_NOT_PRESENCE;
    app_music_list_reset();
}

static status_t app_music_play_index_internal(uint32_t index)
{
    int ret;

    if (index >= app_music_count)
    {
        return kStatus_Fail;
    }

    ret = STREAMER_file_Create(&app_streamer_handle, (char *)app_music_name_buffer[index]);
    if (ret == kStatus_Success)
    {
        app_playing_state = APP_STREAMER_PLAYING;
        STREAMER_Start(&app_streamer_handle);
        PRINTF("Starting playback %d\r\n", index);
        app_playing_index = index;
        return kStatus_Success;
    }
    else
    {
        app_playing_state = APP_STREAMER_NOT_STARTED;
        PRINTF("STREAMER_file_Create failed\r\n");
        PRINTF("Cleanup\r\n");
        STREAMER_Destroy(&app_streamer_handle);
        return kStatus_Fail;
    }
}

static void app_music_resume_internal(void)
{
    if (app_playing_state == APP_STREAMER_PAUSE)
    {
        app_playing_state = APP_STREAMER_PLAYING;
        STREAMER_Resume(&app_streamer_handle);
    }
}

static status_t app_music_stop_internal(void)
{
    if (app_playing_state != APP_STREAMER_NOT_STARTED)
    {
        app_playing_state = APP_STREAMER_NOT_STARTED;
        streamer_pcm_stop();
        STREAMER_Stop(&app_streamer_handle);
        STREAMER_Destroy(&app_streamer_handle);
    }
    return kStatus_Success;
}

static status_t app_music_pause_internal(void)
{
    if (app_playing_state == APP_STREAMER_PLAYING)
    {
        app_playing_state = APP_STREAMER_PAUSE;
        STREAMER_Pause(&app_streamer_handle);
    }
    return kStatus_Success;
}

static status_t app_music_init_internal(void)
{
    FRESULT error;
    DIR directory;
    const TCHAR driver_number_buffer[4U] = {USBDISK + '0', ':', '/', 0x00u};
    FILINFO file_information;
    char *dot;

    app_music_list_reset();

    error = f_opendir(&directory, driver_number_buffer);
    if (error)
    {
        PRINTF("Failed to open root directory of SD card\r\n");
    }
    else
    {
        error = f_chdrive((char const *)&driver_number_buffer[0]);
        if (error)
        {
            PRINTF("Failed to change to usb disk\r\n");
        }
        else
        {
            PRINTF("Available audio files:\r\n");
            while (1)
            {
                error = f_readdir(&directory, &file_information);

                /* When dir end or error detected, break out */
                if ((error != FR_OK) || (file_information.fname[0U] == 0U))
                {
                    break;
                }
                /* Skip root directory */
                if (file_information.fname[0] == '.')
                {
                    continue;
                }
                if (!(file_information.fattrib & AM_DIR))
                {
                    /* Check file for supported audio extension */
                    dot = strrchr(file_information.fname, '.');

                    if ((dot && strncmp(dot + 1, "mp3", 3) == 0) ||
                        (dot && strncmp(dot + 1, "MP3", 3) == 0))
                    {
                        PRINTF("  %s\r\n", file_information.fname);
                        music_add((uint8_t *)file_information.fname, strlen(file_information.fname) + 1);
                    }
                }
            }

            if (error == FR_OK)
            {
                f_closedir(&directory);
            }
        }
    }

    return kStatus_Success;
}

void app_music_play_control_task(void *pvParameters)
{
    uint8_t music_control;

    STREAMER_Init();
    app_music_reset();

    for (;;)
    {
        /* process delta shadow JSON received in prvDeltaCallback() */
        if (xQueueReceive(app_music_control_queue, &music_control, portMAX_DELAY) == pdTRUE)
        {
            switch(music_control)
            {
                case MUSIC_CONTROL_AWS_READY:
                    app_bt_aws_ready();
                    break;
                case MUSIC_CONTROL_STOP:
                    if (app_playing_state != APP_STREAMER_NOT_STARTED)
                    {
                        app_music_stop_internal();
                    }

                    if (app_a2dp_speaker_state == A2DP_SPEAKER_PLAYING)
                    {
                        app_a2dp_speaker_state = A2DP_SPEAKER_READY;
                        app_edgefast_a2dp_suspend();
                    }
                    break;
                case MUSIC_CONTROL_PAUSE:
                    if (app_playing_state == APP_STREAMER_PLAYING)
                    {
                        app_music_pause_internal();
                    }

                    if (app_a2dp_speaker_state == A2DP_SPEAKER_PLAYING)
                    {
                        app_a2dp_speaker_state = A2DP_SPEAKER_READY;
                        app_edgefast_a2dp_suspend();
                    }
                    break;
                case MUSIC_CONTROL_NEXT:
                    if ((app_playing_index + 1) == app_music_count)
                    {
                        music_play_index(0U);
                    }
                    else
                    {
                        music_play_index(app_playing_index + 1);
                    }
                    break;
                case MUSIC_CONTROL_PLAY:
                    /* todo: set a2dp headphone's sample rate etc */
                    if ((app_a2dp_speaker_state != A2DP_SPEAKER_NOT_PRESENCE) && (app_playing_state != APP_STREAMER_PLAYING))
                    {
                        app_plan_play_index = app_playing_index;
                        if (app_a2dp_speaker_state == A2DP_SPEAKER_PLAYING)
                        {
                            app_music_play_index_internal(app_plan_play_index);
                        }
                        else
                        {
                            if (app_playing_state == APP_STREAMER_PAUSE)
                            {
                                app_music_resume_internal();
                                app_a2dp_speaker_state = A2DP_SPEAKER_PLAYING;
                                app_edgefast_a2dp_start();
                            }
                            else
                            {
                                app_a2dp_speaker_state = A2DP_SPEAKER_PLAYING;
                                app_music_play_index_internal(app_plan_play_index);
                                /* wait mp3 file's information to configure a2dp connection */
                                app_edgefast_a2dp_start();
                            }
                        }
                    }
                    break;
                case MUSIC_CONTROL_INIT_MUSIC_LIST:
                    app_music_init_internal();
                    break;
                case MUSIC_CONTROL_DEINIT_MUSIC_LIST:
                    if (app_a2dp_speaker_state == A2DP_SPEAKER_PLAYING)
                    {
                        app_a2dp_speaker_state = A2DP_SPEAKER_READY;
                        app_edgefast_a2dp_suspend();
                    }
                    app_music_stop_internal();
                    app_music_list_reset();
                    break;
                case MUSIC_CONTROL_PLAYER_READY:
                    app_a2dp_speaker_state = A2DP_SPEAKER_READY;
                    break;
                case MUSIC_CONTROL_PLAYER_LOST:
                    app_a2dp_speaker_state = A2DP_SPEAKER_NOT_PRESENCE;
                    app_music_stop_internal();
                    break;
                default:
                    app_plan_play_index = music_control - MUSIC_CONTROL_PLAY_INDEX;
                    if (app_plan_play_index == app_playing_index)
                    {
                        break;
                    }

                    if (app_playing_state == APP_STREAMER_PLAYING)
                    {
                        app_music_stop_internal();
                        if (app_a2dp_speaker_state != A2DP_SPEAKER_NOT_PRESENCE)
                        {
                            //if (app_a2dp_speaker_state == A2DP_SPEAKER_PLAYING)
                            {
                                //app_a2dp_speaker_state = kA2dpSpeaker_WaitMp3FileInfoAndPlaying;
                                app_music_play_index_internal(app_plan_play_index);
                            }
                        }
                    }
                    else if (app_playing_state == APP_STREAMER_PAUSE)
                    {
                        app_music_stop_internal();
                        app_playing_index = app_plan_play_index;
                    }
                    else
                    {
                        app_playing_index = app_plan_play_index;
                    }
                    break;
            }

            if (MUSIC_CONTROL_AWS_READY != music_control)
            {
                app_music_shadow_update(app_playing_index, ((app_playing_state == APP_STREAMER_PLAYING) ? 1 : 0) , ((app_a2dp_speaker_state > A2DP_SPEAKER_NOT_PRESENCE) ? 1 : 0));
            }
        }
    }
}

void music_start_control_task(void)
{
    app_music_control_queue = xQueueCreate(8, sizeof(uint8_t));
    if (app_music_control_queue == NULL)
    {
        configPRINTF(("Failed to create app_music_control_queue queue.\r\n"));
    }
    else
    {
        if (xTaskCreate(app_music_play_control_task, "app_music_play_control_task", 2000L / sizeof(portSTACK_TYPE), NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
        {
            PRINTF("music control task creation failed!.\r\n");
        }
    }
}

static status_t app_music_control_send_msg_internal(uint8_t state)
{
    if (0U != __get_IPSR())
    {
        portBASE_TYPE task_to_wake = pdFALSE;
        if (xQueueSendToBackFromISR(app_music_control_queue, &state, &task_to_wake) == pdTRUE)
        {
            portYIELD_FROM_ISR((task_to_wake));
        }
        else
        {
            return kStatus_Fail;
        }
    }
    else
    {
        if (xQueueSendToBack(app_music_control_queue, &state, portMAX_DELAY) != pdPASS)
        {
            configPRINTF(("Fail to send message to app_music_control_queue.\r\n"));
            return kStatus_Fail;
        }
    }

    return kStatus_Success;
}

status_t music_play(void)
{
    return app_music_control_send_msg_internal(MUSIC_CONTROL_PLAY);
}

status_t music_pause(void)
{
    return app_music_control_send_msg_internal(MUSIC_CONTROL_PAUSE);
}

status_t music_stop(void)
{
    return app_music_control_send_msg_internal(MUSIC_CONTROL_STOP);
}

status_t music_play_next(void)
{
    return app_music_control_send_msg_internal(MUSIC_CONTROL_NEXT);
}

status_t music_play_index(uint8_t index)
{
    return app_music_control_send_msg_internal(MUSIC_CONTROL_PLAY_INDEX + index);
}

void music_add(uint8_t *musicName, uint32_t length)
{
    if (app_music_count < SUPPORT_MAX_MUSIC_FILE_COUNT)
    {
        memcpy(&app_music_name_buffer[app_music_count][0], musicName,
                (length > SUPPORT_MAX_MUSIC_NAME_LENGTH ? SUPPORT_MAX_MUSIC_NAME_LENGTH : length));
        app_music_count++;
    }
}

uint32_t music_get_count(void)
{
    return app_music_count;
}

uint8_t *music_get(uint32_t index)
{
    if (index < app_music_count)
    {
        return app_music_name_buffer[index];
    }
    else
    {
        return NULL;
    }
}

status_t music_initialize(void)
{
    return app_music_control_send_msg_internal(MUSIC_CONTROL_INIT_MUSIC_LIST);
}

status_t music_deinitialize(void)
{
    return app_music_control_send_msg_internal(MUSIC_CONTROL_DEINIT_MUSIC_LIST);
}

status_t music_player_ready(void)
{
    return app_music_control_send_msg_internal(MUSIC_CONTROL_PLAYER_READY);
}

status_t music_player_lost(void)
{
    return app_music_control_send_msg_internal(MUSIC_CONTROL_PLAYER_LOST);
}

uint8_t music_get_play_state(void)
{
    return ((app_playing_state == APP_STREAMER_PLAYING) ? 1 : 0);
}

uint8_t music_get_play_index(void)
{
    return app_playing_index;
}

uint8_t music_get_player_ready(void)
{
    return ((app_a2dp_speaker_state > A2DP_SPEAKER_NOT_PRESENCE) ? 1 : 0);
}

status_t music_play_aws_ready(void)
{
    return app_music_control_send_msg_internal(MUSIC_CONTROL_AWS_READY);
}
