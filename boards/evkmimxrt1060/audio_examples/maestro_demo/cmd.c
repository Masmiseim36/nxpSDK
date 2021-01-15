/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "cmd.h"

#include <string.h>
#include <stdint.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"

#include "app_streamer.h"
#include "fsl_sd_disk.h"

/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*${prototype:start}*/
static shell_status_t shellEcho(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellFile(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellRecMIC(shell_handle_t shellHandle, int32_t argc, char **argv);

/*${prototype:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*${variable:start}*/
SHELL_COMMAND_DEFINE(version, "\r\n\"version\": Display component versions\r\n", shellEcho, 0);

SHELL_COMMAND_DEFINE(file,
                     "\r\n\"file\": Perform audio file decode and playback\r\n"
                     "  USAGE: file [list|<audio_file>|all]\r\n"
                     "    list          List audio files on SD card available for playback\r\n"
                     "    <audio_file>  Select file from SD card and start playback\r\n"
                     "    all           Play all supported audio files from SD card.\r\n"
#ifdef EAP_PROC
                     "    eap           choose one of the options:\r\n"
                     "                    1: All effect Off, 2: Voice enhancer, 3: Music enhancer\r\n"
                     "                    4: Auto volume leveler, 5: Loudness max, 6: 3D Concert snd\r\n"
                     "                    7: Custom, 8: Tone generator\r\n"
                     "  EXAMPLE: Playback with eap effect: file <audio_file> eap 3\r\n"
#endif
                     ,
                     shellFile,
                     SHELL_IGNORE_PARAMETER_COUNT);

SHELL_COMMAND_DEFINE(record_mic,
                     "\r\n\"record_mic\": Record MIC audio"
#ifdef VIT_PROC
                     ", perform voice recognition (VIT)"
#endif
                     " and playback on WM8904 codec.\r\n"
#ifdef VIT_PROC
                     "  For voice recognition say supported WakeWord (Hey NXP) and in 3s frame supported command.\r\n"
                     "  List of supported commands:\r\n"
                     "    MUTE, NEXT, SKIP, PAIR_DEVICE, PAUSE, STOP, POWER_OFF, POWER_ON, PLAY_MUSIC\r\n"
                     "    PLAY_GAME, WATCH_CARTOON, WATCH_MOVIE\r\n"
                     "  Please note that this VIT demo is near-field and uses 1 on-board microphone.\r\n"
#endif
                     "  NOTE: this command returns to shell after 20 seconds.\r\n",
                     shellRecMIC,
                     0);
static bool file_playing = false;

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

extern serial_handle_t g_serialHandle;
static handleShellMessageCallback_t *g_handleShellMessageCallback;
static void *g_handleShellMessageCallbackData;

streamer_handle_t streamerHandle;

/*${variable:end}*/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*${function:start}*/

void play_file(char *filename, char *eap_par)
{
    STREAMER_Init();
    int ret = STREAMER_file_Create(&streamerHandle, filename, eap_par);
    if (ret != kStatus_Success)
    {
        PRINTF("STREAMER_file_Create failed\r\n");
        goto file_error;
    }

    STREAMER_Start(&streamerHandle);

    PRINTF("Starting playback\r\n");

    file_playing = true;
    while (streamerHandle.audioPlaying)
    {
    }
    file_playing = false;

file_error:
    PRINTF("Cleanup\r\n");
    STREAMER_Destroy(&streamerHandle);
    osa_time_delay(1000);
}

static shell_status_t shellEcho(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    PRINTF(" Maestro version: 1.0\r\n");
#if defined(__MCUXPRESSO)
    PRINTF("\r\n Component versions:\r\n");
    PRINTF(" EAP version: 2.4\r\n");
    PRINTF(" VIT version: 2.3.0\r\n");
#endif
    return kStatus_SHELL_Success;
}

static shell_status_t shellFile(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    app_handle_t *app = (app_handle_t *)g_handleShellMessageCallbackData;
    DIR directory;
    FILINFO fileInformation;
    char *filename, *dot;
    uint32_t count = 0;
    FRESULT error;
    char *eap_par = "0";

    if (!app->sdcardInserted)
    {
        PRINTF("Please insert an SD card with audio files and retry this command\r\n");
        return kStatus_SHELL_Success;
    }

    // check if eap setting is presented
    if ((argc > 2) && (strcmp(argv[2], "eap") == 0))
    {
        eap_par = argv[3];
    }

    if (strcmp(argv[1], "list") == 0)
    {
        error = f_opendir(&directory, "/");
        if (error)
        {
            PRINTF("Failed to open root directory of SD card\r\n");
            return kStatus_SHELL_Error;
        }

        PRINTF("Available audio files:\r\n");

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
                /* Check file for supported audio extension */
                dot = strrchr(fileInformation.fname, '.');
                if ((dot && strncmp(dot + 1, "opus", 4) == 0) || (dot && strncmp(dot + 1, "ogg", 3) == 0) ||
                    (dot && strncmp(dot + 1, "mp3", 3) == 0))
                {
                    PRINTF("  %s\r\n", fileInformation.fname);
                    count++;
                }
            }
        }

        if (error == FR_OK)
        {
            f_closedir(&directory);
        }

        if (!count)
        {
            PRINTF("  (none)\r\n");
        }
    }
    else if (strcmp(argv[1], "stop") == 0)
    {
        if (file_playing)
        {
            g_handleShellMessageCallback(g_handleShellMessageCallbackData);
            return kStatus_SHELL_Success;
        }
        else
        {
            PRINTF("File is not playing \r\n");
            return kStatus_SHELL_Error;
        }
    }
    else if (strcmp(argv[1], "all") == 0)
    {
        if (!file_playing)
        {
            error = f_opendir(&directory, "/");
            if (error)
            {
                PRINTF("Failed to open root directory of SD card\r\n");
                return kStatus_SHELL_Error;
            }

            PRINTF("Looking for available files:\r\n");

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
                    /* Check file for supported audio extension */
                    dot = strrchr(fileInformation.fname, '.');
                    if ((dot && strncmp(dot + 1, "opus", 4) == 0) || (dot && strncmp(dot + 1, "ogg", 3) == 0) ||
                        (dot && strncmp(dot + 1, "mp3", 3) == 0))
                    {
                        PRINTF("Play file: %s \r\n", fileInformation.fname);
                        play_file(fileInformation.fname, eap_par);
                        count++;
                    }
                }
            }

            if (error == FR_OK)
            {
                f_closedir(&directory);
            }

            if (!count)
            {
                PRINTF("  (none)\r\n");
            }
        }
        else
        {
            PRINTF("File is already playing\r\n");
            return kStatus_SHELL_Error;
        }
    }
    else if (!file_playing)
    {
        filename = argv[1];

        dot = strrchr(filename, '.');
        if ((dot && strncmp(dot + 1, "mp3", 3) == 0) || (dot && strncmp(dot + 1, "ogg", 3) == 0) ||
            (dot && strncmp(dot + 1, "opus", 4) == 0))
        {
            count = 1;
        }
        if (!count)
        {
            PRINTF("Unsupported file type %s\r\n", filename);
            return kStatus_SHELL_Error;
        }

        PRINTF("\r\nStarting streamer demo application\r\n");

        error = f_stat(filename, NULL);
        if (error != FR_OK)
        {
            PRINTF("Error: File checking error.\r\n");
        }
        else
        {
            play_file(filename, eap_par);
        }
    }
    else
    {
        PRINTF("File is already playing\r\n");
        return kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}

static shell_status_t shellRecMIC(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    status_t ret;
    PRINTF("\r\nStarting streamer demo application\r\n");

    STREAMER_Init();
    ret = STREAMER_mic_Create(&streamerHandle);
    if (ret != kStatus_Success)
    {
        PRINTF("STREAMER_Create failed\r\n");
        goto error;
    }

    PRINTF("Starting recording\r\n");
#ifdef VIT_PROC
    PRINTF("\r\nTo see VIT functionality say wake-word and command.\r\n");
#endif
    STREAMER_Start(&streamerHandle);

    osa_time_delay(20000);

    STREAMER_Stop(&streamerHandle);

error:
    PRINTF("Cleanup\r\n");
    STREAMER_Destroy(&streamerHandle);
    return kStatus_SHELL_Success;
}

void shellCmd(handleShellMessageCallback_t *handleShellMessageCallback, void *arg)
{
    /* Init SHELL */
    s_shellHandle = &s_shellHandleBuffer[0];
    SHELL_Init(s_shellHandle, g_serialHandle, ">> ");

    /* Add new command to commands list */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(version));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(file));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(record_mic));

    g_handleShellMessageCallback     = handleShellMessageCallback;
    g_handleShellMessageCallbackData = arg;

#if !(defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
    while (1)
    {
        SHELL_Task(s_shellHandle);
    }
#endif
}
/*${function:end}*/
