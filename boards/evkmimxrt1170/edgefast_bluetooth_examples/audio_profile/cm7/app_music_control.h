/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_MUSIC_CONTROL__
#define __APP_MUSIC_CONTROL__

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SUPPORT_MAX_MUSIC_FILE_COUNT (50)
#define SUPPORT_MAX_MUSIC_NAME_LENGTH (50)

typedef enum _music_control_code
{
    MUSIC_CONTROL_STOP = 0,
    MUSIC_CONTROL_PAUSE,
    MUSIC_CONTROL_PLAY,
    MUSIC_CONTROL_NEXT,
    MUSIC_CONTROL_INIT_MUSIC_LIST,
    MUSIC_CONTROL_DEINIT_MUSIC_LIST,
    MUSIC_CONTROL_PLAYER_READY,
    MUSIC_CONTROL_PLAYER_LOST,
    MUSIC_CONTROL_AWS_READY,
    MUSIC_CONTROL_PLAY_INDEX,
} music_control_code_t;

/*******************************************************************************
 * API
 ******************************************************************************/   
#if defined(__cplusplus)
extern "C" {
#endif

void music_start_control_task(void);

status_t music_play(void);

status_t music_stop(void);

status_t music_pause(void);

status_t music_play_next(void);

status_t music_play_index(uint8_t index);

void music_add(uint8_t *musicName, uint32_t length);

uint32_t music_get_count(void);

uint8_t *music_get(uint32_t index);

status_t music_initialize(void);

status_t music_deinitialize(void);

status_t music_player_ready(void);

status_t music_player_lost(void);

uint8_t music_get_play_state(void);

uint8_t music_get_play_index(void);

uint8_t music_get_player_ready(void);

status_t music_play_aws_ready(void);

#if defined(__cplusplus)
}
#endif

#endif
