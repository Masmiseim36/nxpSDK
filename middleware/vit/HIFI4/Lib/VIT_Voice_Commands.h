/*
* Copyright 2020 NXP
*
* NXP Confidential.This software is owned or controlled by NXP and may only
* be used strictly in accordance with the applicable license terms found in
* file LICENSE.txt
*/

#ifndef __VIT_VOICE_COMMANDS_H__
#define __VIT_VOICE_COMMANDS_H__


// VIT Voice Commands supported
typedef enum
{
    UNKNOWN                = 0,
    MUTE                   = 1,
    NEXT                   = 2,
    SKIP                   = 3,
    PAIR_DEVICE            = 4,
    PAUSE                  = 5,
    STOP                   = 6,
    POWER_OFF              = 7,
    POWER_ON               = 8,
    PLAY_MUSIC             = 9,
    PLAY_GAME              = 10,
    WATCH_CARTOON          = 11,
    WATCH_MOVIE            = 12,
    DUMMY_CMD              = PL_MAXENUM
} VIT_VoiceCommands_en;

 #endif      /* __VIT_VOICE_COMMANDS_H__ */

/* End of file */

