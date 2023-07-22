/*
 * Copyright 2019-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app_data.h"
#include "cmd.h"

#include <string.h>
#include <stdint.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "ff.h"

#include "app_definitions.h"
#include "app_streamer.h"

#include "eap_att.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define STR(s)    #s
#define TO_STR(s) STR(s)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*${prototype:start}*/
static shell_status_t shellEcho(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellFile(shell_handle_t shellHandle, int32_t argc, char **argv);

/*${prototype:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*${variable:start}*/
SHELL_COMMAND_DEFINE(version, "\r\n\"version\": Display component versions\r\n", shellEcho, 0);

SHELL_COMMAND_DEFINE(
    file,
    "\r\n\"file\": Perform audio file decode and playback\r\n"
    "\r\n"
    "  USAGE: file [start|stop|pause|volume|"
#ifndef MULTICHANNEL_EXAMPLE
    "seek|"
#endif
#ifdef EAP_PROC
    "update|set|get|"
#endif
    "track|list|info]\r\n"
#ifdef MULTICHANNEL_EXAMPLE
    "    start             Play default (first found) file with default (8) channels.\r\n"
#else
                     "    start             Play default (first found) or specified audio track file.\r\n"
#endif
    "    stop              Stops actual playback.\r\n"
    "    pause             Pause actual track or resume if already paused.\r\n"
	"    volume=<volume>   Set volume. The volume can be set from 0 to 100.\r\n"
#ifndef MULTICHANNEL_EXAMPLE
    "    seek=<seek_time>  Seek currently paused track. Seek time is absolute time in milliseconds.\r\n"
#endif
#ifdef EAP_PROC
    "    update=<preset>   Apply current EAP parameters without attribute value\r\n"
    "                      or switch to preset 1-"TO_STR(EAP_MAX_PRESET)"\r\n"
    "    set=<preset>      Apply current EAP parameters without attribute value\r\n"
    "                      or switch to preset 1-"TO_STR(EAP_MAX_PRESET)"\r\n"
    "    get               Sync actual EAP parameters from library to ATT config structures.\r\n"

#endif
#ifdef MULTICHANNEL_EXAMPLE
    "    track <filename> [num_channels]  Select audio track to play. Select 2 or 8 channels. \r\n"
    "                                    - If channel number not specified, default 8 is used. \r\n"
#else
                     "    track=<filename>  Select audio track to play.\r\n"
#endif
    "    list              List audio files available on mounted SD card.\r\n"
    "    info              Prints playback info.\r\n"
#ifdef MULTICHANNEL_EXAMPLE
    "  NOTE: Selected audio track must always meet the following parameters:\r\n"
	"                  - Sample rate:        96 kHz\r\n"
    "                  - Width:              32 bit\r\n"
    "                  - Number of channels: Depending on the [num_channels] parameter\r\n"
#ifdef EAP_PROC
	"  NOTE: Only when 2 channels are selected EAP can be applied to the audio track."
#endif
#endif
,
    shellFile,
    SHELL_IGNORE_PARAMETER_COUNT);

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;
extern serial_handle_t g_serialHandle;
extern app_handle_t app;
streamer_handle_t streamerHandle;

/*${variable:end}*/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*${function:start}*/

static uint32_t isFileOnSDcard(char *filename)
{
    FRESULT error;
    DIR directory           = {0};
    FILINFO fileInformation = {0};
    uint32_t filePresent    = false;

    if (!app.sdcardInserted)
    {
        PRINTF("[CMD] Please insert an SD card with audio files and retry this command\r\n");
        return 0;
    }

    error = f_opendir(&directory, "/");
    if (error)
    {
        PRINTF("[CMD] Failed to open root directory of SD card\r\n");
        return 0;
    }

    while (1)
    {
        error = f_readdir(&directory, &fileInformation);
        /* When dir end or error detected, break out */
        if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
        {
            break;
        }
        /* Skip root directory */
        if (fileInformation.fname[0] == '.')
        {
            continue;
        }
        if (!(fileInformation.fattrib & AM_DIR))
        {
            if (strcmp(fileInformation.fname, filename) == 0)
            {
                filePresent = true;
                break;
            }
        }
    }

    if (error == FR_OK)
    {
        f_closedir(&directory);
    }

    return filePresent;
}

static shell_status_t shellEcho(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    PRINTF(" Maestro version: 1.2\r\n");

#ifdef EAP_PROC
    PRINTF(" EAP version: 3.0.12\r\n");
#endif

    return kStatus_SHELL_Success;
}

static shell_status_t shellFile(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    char *dot;
#ifdef MULTICHANNEL_EXAMPLE
    uint8_t num_channels = DEMO_CHANNEL_NUM; // default number of channels is 8
#endif
    shell_status_t retVal = kStatus_SHELL_Success;

    if (!app.sdcardInserted)
    {
        PRINTF("[CMD] Please insert an SD card with audio files and retry this command\r\n");
        return kStatus_SHELL_Error;
    }

    // lock ATT mutex if needed, but based on major usecase it is not necessary
    // be sure that this routine is as short as possible without any complex logic
#ifdef MULTICHANNEL_EXAMPLE
    if (argc >= 2)
    {
#endif
        if (strcmp(argv[1], "start") == 0)
        {
#ifdef MULTICHANNEL_EXAMPLE
            get_app_data()->num_channels = DEMO_CHANNEL_NUM; // set default channel number = 8
#endif
            get_eap_att_control()->command = kAttCmdStart;
        }
        else if (strcmp(argv[1], "stop") == 0)
        {
            get_eap_att_control()->command = kAttCmdStop;
        }
        else if (strcmp(argv[1], "pause") == 0)
        {
            get_eap_att_control()->command = kAttCmdPause;
        }
#ifndef MULTICHANNEL_EXAMPLE
        else if (strcmp(argv[1], "seek") == 0)
        {
            if ((get_eap_att_control()->status != kAttPaused) && (get_eap_att_control()->status != kAttRunning))
            {
                PRINTF("[CMD] First select an audio track to play.\r\n");
                retVal = kStatus_SHELL_Error;
            }
            else if (get_eap_att_control()->status != kAttPaused)
            {
                PRINTF("[CMD] First pause the track.\r\n");
                retVal = kStatus_SHELL_Error;
            }
            else
            {
                if (argc >= 3)
                {
                    dot = strrchr(get_eap_att_control()->input, '.');
                    if ((dot && strncmp(dot + 1, "aac", 3) == 0) && (get_eap_att_control()->status == kAttPaused))
                    {
                        PRINTF("[CMD] The AAC decoder does not support the seek command.\r\n");
                        retVal = kStatus_SHELL_Error;
                    }
                    else
                    {
                        if (atoi(argv[2]) < 0)
                        {
                            PRINTF("[CMD] The seek time must be a positive value.\r\n");
                            retVal = kStatus_SHELL_Error;
                        }
                        else
                        {
                            get_eap_att_control()->seek_time = atoi(argv[2]);
                            get_eap_att_control()->command   = kAttCmdSeek;
                        }
                    }
                }
                else
                {
                    PRINTF("[CMD] Enter a seek time value.\r\n");
                    retVal = kStatus_SHELL_Error;
                }
            }
        }
#endif
#if defined(EAP_PROC) && (ALGORITHM_XO == 1)
        else if (strcmp(argv[1], "xo") == 0) // this option is good for testing but could be removed for production
        {
            if (argc >= 3)
            {
                int value = abs(atoi((argv[2])));
                if (value == 0)
                {
                    get_eap_att_control()->controlParam->XO_OperatingMode = LVM_XO_MODE_OFF;
                }
                else if (value == 1)
                {
                    get_eap_att_control()->controlParam->XO_OperatingMode = LVM_XO_MODE_ON;
                }
                else if (value >= 60 && value <= 6000)
                {
                    get_eap_att_control()->controlParam->XO_OperatingMode   = LVM_XO_MODE_ON;
                    get_eap_att_control()->controlParam->XO_cutoffFrequency = value;
                }
                else
                {
                    PRINTF(
                        "[CMD] Undefined Crossover parameter. Use 'att xo 0|1|<60,6000>' where 0|1 are for "
                        "disable|enable or "
                        "specify frequency from range.\r\n");
                    return retVal;
                }
                get_eap_att_control()->command = kAttCmdSetConfig;
            }
            else
            {
                PRINTF(
                    "[CMD] Undefined Crossover parameter. Use 'att xo 0|1|<60,6000>' where 0|1 are for disable|enable "
                    "or "
                    "specify frequency from range.\r\n");
                retVal = kStatus_SHELL_Error;
            }
        }
#endif
        else if (strcmp(argv[1], "volume") == 0)
        {
            if (argc >= 3)
            {
                int value = atoi((argv[2]));
                if (value >= 0 && value <= 100)
                {
                    get_eap_att_control()->volume  = value;
                    get_eap_att_control()->command = kAttCmdVolume;
                }
                else
                {
                    PRINTF("[CMD] Ignoring wrong volume parameter.\r\n");
                    retVal = kStatus_SHELL_Error;
                }
            }
            else
            {
                PRINTF("[CMD] Enter volume parameter.\r\n");
                retVal = kStatus_SHELL_Error;
            }
        }
#if defined(EAP_PROC)
        else if (strcmp(argv[1], "set") == 0 || strcmp(argv[1], "update") == 0)
        {
            if (argc == 3)
            {
                int preset = abs(atoi((argv[2])));
                if (preset <= 0 || preset > EAP_MAX_PRESET)
                {
                    PRINTF("[CMD] EAP preset number out of range, setting EAP all effects OFF.\r\n");
                    preset = 1;
                }
                get_eap_att_control()->eapPreset = preset;
            }
            get_eap_att_control()->command = kAttCmdSetConfig;
        }
        else if (strcmp(argv[1], "get") == 0)
        {
            get_eap_att_control()->command = kAttCmdGetConfig;
        }
#endif
        else if (strcmp(argv[1], "track") == 0)
        {
            if (get_eap_att_control()->status == kAttIdle || get_eap_att_control()->status == kAttRunning)
            {
                if (argc >= 3)
                {
                    if (isFileOnSDcard(argv[2]))
                    {
                        dot = strrchr(argv[2], '.');
                        if (
#ifdef MULTICHANNEL_EXAMPLE
                            (dot && strncmp(dot + 1, "pcm", 4) == 0)
#else
#if (OGG_OPUS_DEC == 1)
                        (dot && strncmp(dot + 1, "opus", 4) == 0) || (dot && strncmp(dot + 1, "ogg", 3) == 0) ||
#endif
#if (AAC_DEC == 1)
                        (dot && strncmp(dot + 1, "aac", 3) == 0) ||
#endif
#if (WAV_DEC == 1)
                        (dot && strncmp(dot + 1, "wav", 3) == 0) ||
#endif
#if (FLAC_DEC == 1)
                        (dot && strncmp(dot + 1, "flac", 3) == 0) ||
#endif
                        (dot && strncmp(dot + 1, "mp3", 3) == 0)
#endif
                        )
                        {
                            strcpy(get_eap_att_control()->input, argv[2]);
                            get_eap_att_control()->command = kAttCmdStart;
                        }
                        else
                        {
                            PRINTF(
                                "[CMD] Input audio file name has to match one of the"
#ifdef MULTICHANNEL_EXAMPLE
                                " .pcm"
#else
                            " .mp3"
#if (OGG_OPUS_DEC == 1)
                            "|.opus|.ogg"
#endif
#if (AAC_DEC == 1)
                            "|.aac"
#endif
#if (WAV_DEC == 1)
                            "|.wav"
#endif
#if (FLAC_DEC == 1)
                            "|.flac"
#endif
#endif
                                " format.\r\n");

                            retVal = kStatus_SHELL_Error;
                        }
#ifdef MULTICHANNEL_EXAMPLE
                        if (argc == 4)
                        {
                            num_channels = abs(atoi((argv[3])));
                            if (num_channels == 2 || num_channels == 8)
                            {
                                get_app_data()->num_channels = num_channels;
                            }
                            else
                            {
                                PRINTF("Number of channels not allowed (2 or 8 allowed).\r\n");
                                retVal = kStatus_SHELL_Error;
                            }
                        }
                        else
                        {
                            get_app_data()->num_channels = num_channels;
                        }
#endif
                    }
                    else
                    {
                        PRINTF("[CMD] File is not on the SD card, please enter valid filename.\r\n");
                        retVal = kStatus_SHELL_Error;
                    }
                }
                else
                {
                    PRINTF("[CMD] Enter name of audio file.\r\n");
                    retVal = kStatus_SHELL_Error;
                }
            }
            else
            {
                PRINTF("[CMD] Use the stop command first.\r\n");
                retVal = kStatus_SHELL_Error;
            }
        }
        else if (strcmp(argv[1], "list") == 0)
        {
            if (list_files(false) != kStatus_Success)
            {
                retVal = kStatus_SHELL_Error;
            }
        }
        else if (strcmp(argv[1], "info") == 0)
        {
            PRINTF("[CMD] status: %d (last error: %d), (%d/%d)[%s]\r\n", get_eap_att_control()->status,
                   get_eap_att_control()->lastError, get_eap_att_control()->trackCurrent,
                   get_eap_att_control()->trackTotal, get_eap_att_control()->input);
        }
        else
        {
            PRINTF("[CMD] Undefined att command option. \r\n");
            retVal = kStatus_SHELL_Error;
        }
#ifdef MULTICHANNEL_EXAMPLE
    }
    else
    {
        PRINTF("[CMD] Enter correct command option. \r\n");
        retVal = kStatus_SHELL_Error;
    }
#endif
    // unlock APP mutex
    return retVal;
}

void shellCmd(void)
{
    /* Init SHELL */
    s_shellHandle = &s_shellHandleBuffer[0];
    SHELL_Init(s_shellHandle, g_serialHandle, ">> ");

    /* Add new command to commands list */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(version));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(file));

#if !(defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
    while (1)
    {
        SHELL_Task(s_shellHandle);
    }
#endif
}
/*${function:end}*/
