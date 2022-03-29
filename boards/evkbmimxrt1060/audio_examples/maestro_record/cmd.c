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
#include "portable.h"

#ifdef VIT_PROC
#include "PL_platformTypes_CortexM7.h"
#include "VIT.h"
#include "vit_proc.h"
#endif
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
static shell_status_t shellRecMIC(shell_handle_t shellHandle, int32_t argc, char **argv);
#ifdef OPUS_ENCODE
static shell_status_t shellOpusEncode(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif
/*${prototype:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*${variable:start}*/
SHELL_COMMAND_DEFINE(version, "\r\n\"version\": Display component versions\r\n", shellEcho, 0);

SHELL_COMMAND_DEFINE(record_mic,
                     "\r\n\"record_mic\": Record MIC audio and either:\r\n"
#ifdef VIT_PROC
                     " - perform voice recognition (VIT)\r\n"
#endif
                     " - playback on WM8904 codec\r\n"
                     " - store samples to file.\r\n"
                     "\r\n"
#ifdef VIT_PROC
                     " USAGE: record_mic [audio|file|<file_name>|vit] 20 [en|cn]\r\n"
#else
                     " USAGE: record_mic [audio|file|<file_name>] 20\r\n"
#endif
                     " The number defines length of recording in seconds.\r\n"
#ifdef VIT_PROC
                     " For voice recognition say supported WakeWord and in 3s frame supported command.\r\n"
                     " Please note that this VIT demo is near-field and uses 1 on-board microphone.\r\n"
#endif
                     " NOTES: This command returns to shell after record finished.\r\n"
                     "        To store samples to a file, the \"file\" option can be used to create a file\r\n"
                     "        with a predefined name, or any file name (without whitespaces) can be specified\r\n"
                     "        instead of the \"file\" option.\r\n",
                     shellRecMIC,
                     SHELL_IGNORE_PARAMETER_COUNT);

#ifdef OPUS_ENCODE
SHELL_COMMAND_DEFINE(opus_encode,
                     "\r\n\"opus_encode\": Initializes the streamer with the Opus memory-to-memory pipeline and\r\n"
                     "encodes a hardcoded buffer.\r\n",
                     shellOpusEncode,
                     0);
#endif

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

extern serial_handle_t g_serialHandle;

streamer_handle_t streamerHandle;

/*${variable:end}*/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*${function:start}*/

static shell_status_t shellEcho(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    PRINTF(" Maestro version: 1.2\r\n");

#ifdef VIT_PROC
    PRINTF(" VIT version: 5.4.0\r\n");
#endif

    return kStatus_SHELL_Success;
}

static shell_status_t shellRecMIC(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    status_t ret;
    out_sink_t out_sink;
    int duration    = 20;
    char *file_name = NULL;
#ifdef VIT_PROC
    Vit_Language = EN;
#endif

    if ((argc > 1) && (strcmp(argv[1], "file") == 0))
    {
        out_sink = FILE_SINK;
    }
    else if ((argc > 1) && (strcmp(argv[1], "audio") == 0))
    {
        out_sink = AUDIO_SINK;
    }
#ifdef VIT_PROC
    else if ((argc > 1) && (strcmp(argv[1], "vit") == 0))
    {
        out_sink = VIT_SINK;
    }
#endif
    else
    {
        PRINTF("Undefined command - recording to a file on sd-card.\r\n");
        /* Save the samples to the file with the defined name */
        out_sink  = FILE_SINK;
        file_name = argv[1];
    }

    if ((argc > 2))
    {
        if (strcmp(argv[2], "\0") != 0)
        {
            duration = abs(atoi(argv[2]));
        }
    }
#ifdef VIT_PROC
    if ((argc > 3))
    {
        if (strcmp(argv[3], "cn") == 0)
        {
            Vit_Language = CN;
        }
    }
#endif

    PRINTF("\r\nStarting streamer demo application for %d sec\r\n", duration);

    STREAMER_Init();

    ret = STREAMER_mic_Create(&streamerHandle, out_sink, file_name);

    if (ret != kStatus_Success)
    {
        PRINTF("STREAMER_Create failed\r\n");
        goto error;
    }

    PRINTF("Starting recording\r\n");
#ifdef VIT_PROC
    if (out_sink == VIT_SINK)
    {
        PRINTF("\r\nTo see VIT functionality say wake-word and command.\r\n");
    }
#endif
    STREAMER_Start(&streamerHandle);

    osa_time_delay(duration * 1000);

    STREAMER_Stop(&streamerHandle);

error:
    PRINTF("Cleanup\r\n");
    STREAMER_Destroy(&streamerHandle);
    /* Delay for cleanup */
    osa_time_delay(100);
    return kStatus_SHELL_Success;
}

#ifdef OPUS_ENCODE
static shell_status_t shellOpusEncode(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    void *inBuf                   = NULL;
    void *outBuf                  = NULL;
    MEMSRC_SET_BUFFER_T inBufInfo = {0};
    SET_BUFFER_DESC_T outBufInfo  = {0};
    bool streamerInitialized      = false;

    status_t ret;
    CeiBitstreamInfo info = {
        .sample_rate = 48000, .num_channels = 1, .endian = 0, .sign = TRUE, .sample_size = 16, .interleaved = TRUE};

    PRINTF("Starting streamer with the preliminary Opus memory-to-memory pipeline.\r\n");

    PRINTF("Allocating buffers...\r\n");

    inBuf = pvPortMalloc(OPUSMEM2MEM_INBUF_SIZE);
    if (inBuf == NULL)
    {
        PRINTF("Inbuf allocation failed\r\n");
        goto error;
    }
    memcpy(inBuf, &OPUSMEM2MEM_INBUF_CONTENT, OPUSMEM2MEM_INBUF_SIZE);

    outBuf = pvPortMalloc(OPUSMEM2MEM_OUTBUF_SIZE);
    if (outBuf == NULL)
    {
        PRINTF("Outbuf allocation failed\r\n");
        goto error;
    }
    memset(outBuf, 0, OPUSMEM2MEM_OUTBUF_SIZE);

    inBufInfo  = (MEMSRC_SET_BUFFER_T){.location = (int8_t *)inBuf, .size = OPUSMEM2MEM_INBUF_SIZE};
    outBufInfo = (SET_BUFFER_DESC_T){.ptr = (int8_t *)outBuf, .size = OPUSMEM2MEM_OUTBUF_SIZE};

    PRINTF("Initializing streamer...\r\n");

    STREAMER_Init();
    streamerInitialized = true;

    ret = STREAMER_opusmem2mem_Create(&streamerHandle, &info, &inBufInfo, &outBufInfo);
    if (ret != kStatus_Success)
    {
        PRINTF("Streamer create failed\r\n");
        goto error;
    }

    CeiOpusConfig cfg;
    streamer_get_property(streamerHandle.streamer, PROP_ENCODER_CONFIG, (uint32_t *)&cfg, true);

    cfg.application = OPUS_APPLICATION_VOIP;
    streamer_set_property(streamerHandle.streamer,
                          (ELEMENT_PROPERTY_T){.prop = PROP_ENCODER_CONFIG, .val = (uintptr_t)&cfg}, true);

    PRINTF("Start encoding...\r\n");
    STREAMER_Start(&streamerHandle);
    while (streamerHandle.audioPlaying)
        ;
    PRINTF("Encoding finished.\r\n");

error:
    PRINTF("Cleanup\r\n");
    vPortFree(inBuf);
    vPortFree(outBuf);

    if (streamerInitialized)
    {
        STREAMER_Destroy(&streamerHandle);
    }

    /* Delay for cleanup */
    osa_time_delay(100);
    return kStatus_SHELL_Success;
}
#endif

void shellCmd(void)
{
    /* Init SHELL */
    s_shellHandle = &s_shellHandleBuffer[0];
    SHELL_Init(s_shellHandle, g_serialHandle, ">> ");

    /* Add new command to commands list */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(version));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(record_mic));
#ifdef OPUS_ENCODE
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(opus_encode));
#endif

#if !(defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
    while (1)
    {
        SHELL_Task(s_shellHandle);
    }
#endif
}
/*${function:end}*/
