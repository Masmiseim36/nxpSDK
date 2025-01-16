/*
 * Copyright 2018-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "srtm_config.h"
#include "srtm_utils.h"

#include "fsl_sema42.h"
#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include "xaf-api.h"
#if DUMP_TRACE_TO_BUF
#include "xf-debug.h"
#define TRACE_BUF_SIZE (512 * 1024)
#endif

#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DSP_THREAD_STACK_SIZE (12 * 1024)
#define DSP_THREAD_PRIORITY   (XOS_MAX_PRIORITY - 3)

#define AUDIO_BUFFER_SIZE (32 * 1024)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* SRTM prototypes */
int srtm_decoder(dsp_handle_t *dsp, unsigned int *pCmdParams);
int srtm_encoder(dsp_handle_t *dsp, unsigned int *pCmdParams);
int srtm_src(dsp_handle_t *dsp, unsigned int *pCmdParams);
int srtm_pcm_gain(dsp_handle_t *dsp, unsigned int *pCmdParams);
int srtm_file_dec_create(dsp_handle_t *dsp, srtm_audio_component_t type);
#ifdef MULTICHANNEL
int srtm_file_ren_create(dsp_handle_t *dsp, uint32_t *pCmdParams);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

dsp_handle_t dsp;
static uint8_t dsp_thread_stack[DSP_THREAD_STACK_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/

static void DSP_XAF_Init(dsp_handle_t *dsp)
{
    uint8_t *version[3];

    xos_mutex_create(&dsp->audioMutex, XOS_MUTEX_WAIT_PRIORITY, 0);
    xos_mutex_create(&dsp->rpmsgMutex, XOS_MUTEX_WAIT_PRIORITY, 0);
    xos_cond_create(&dsp->ipc_waiting_cond);
    xos_cond_create(&dsp->read_waiting_cond);
    dsp->file_frame_size = 0;

    dsp->audioBuffer = ringbuf_create(AUDIO_BUFFER_SIZE);
    if (!dsp->audioBuffer)
    {
        DSP_PRINTF("[DSP_Main] ringbuffer allocation failed\r\n");
    }

    xos_event_create(&dsp->pipeline_event, 0xFF, XOS_EVENT_AUTO_CLEAR);

    xaf_get_verinfo(version);

    DSP_PRINTF("\r\n");
    DSP_PRINTF("[DSP_Main] Cadence Xtensa Audio Framework\r\n");
    DSP_PRINTF("[DSP_Main] Library Name    : %s\r\n", version[0]);
    DSP_PRINTF("[DSP_Main] Library Version : %s\r\n", version[1]);
    DSP_PRINTF("[DSP_Main] API Version     : %s\r\n", version[2]);
    DSP_PRINTF("\r\n");
}

static int handleMSG_GENERAL(dsp_handle_t *dsp, srtm_message *msg)
{
    switch (msg->head.command)
    {
        /* Return SDK key component versions to host*/
        case SRTM_Command_ECHO:
            // 0 Audio Framework version high 16 bits major, lower 16 bits minor
            msg->param[0] = ((3) << 16 | (5));
            // 1 Audio Framework API version high 16 bits major, lower 16 bits minor
            msg->param[1] = ((3) << 16 | (2));
            // 2 NatureDSP Lib version high 16 bits major, lower 16 bits minor
            msg->param[2] = ((4) << 16 | (1));
            // 3 NatureDSP Lib API version high 16 bits major, lower 16 bits minor
            msg->param[3] = ((4) << 16 | (0));
            // 4 MP3 Decoder version high 16 bits major, lower 16 bits minor
            msg->param[4] = ((3) << 16 | (28));
            // 5 AAC Decoder version high 16 bits major, lower 16 bits minor
            msg->param[5] = ((3) << 16 | (16));
#if XA_VORBIS_DECODER
            // 6 VORBIS Decoder version high 16 bits major, lower 16 bits minor
            msg->param[6] = ((1) << 16 | (15));
#endif
            // 7 OPUS Codec version high 16 bits major, lower 16 bits minor
            msg->param[7] = ((1) << 16 | (24));
            // 8 SBC Decoder version high 16 bits major, lower 16 bits minor
            msg->param[8] = ((1) << 16 | (6));
            // 9 SBC Encoder version high 16 bits major, lower 16 bits minor
            msg->param[9] = ((1) << 16 | (7));
            break;

        /* Unknown message. */
        default:
            msg->head.type = SRTM_MessageTypeNotification;
            msg->error     = SRTM_Status_InvalidMessage;
            break;
    }

    return 0;
}

static int handleMSG_AUDIO(dsp_handle_t *dsp, srtm_message *msg)
{
    char *remote_addr;

    dsp->event_occur = false;

    switch (msg->head.command)
    {
        case SRTM_Command_Decode:
        	/* Param 0 Decoder type */
			/* Param 1 Buffer address with encoded data */
			/* Param 2 Encoded data size */
            DSP_PRINTF("[DSP_Main] Input buffer addr: 0x%X, buffer size: %d \r\n", msg->param[1], msg->param[2]);
            if ((msg->param[0] >= SRTM_Decoder_MAX) || (msg->param[1] == 0) || (msg->param[2] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
                break;
            }
            msg->error = srtm_decoder(dsp, &msg->param[0]);
            break;

        case SRTM_Command_Encode:
            /* Param 0 Encoder type */
            /* Param 1 Buffer address with PCM data */
            /* Param 2 PCM data size */
            /* Param 3 Buffer address to store encoded data */
            /* Param 4 Buffer size to store encoded data */
            /* Param 5 Return parameter, actual read size */
            /* Param 6 Return parameter, actual write size */
            DSP_PRINTF(
                "[DSP_Main] PCM input buffer addr: 0x%X, buffer size: %d, output buffer addr: 0x%X, output buffer "
                "size: %d\r\n",
                msg->param[1], msg->param[2], msg->param[3], msg->param[4]);
            if ((msg->param[0] >= SRTM_Encoder_MAX) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0) || (msg->param[4] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
                break;
            }
            msg->error = srtm_encoder(dsp, &msg->param[0]);
            break;

        case SRTM_Command_SRC:
            /* Param 0 SRC input buffer 1 address*/
            /* Param 1 SRC input buffer 1 size*/
            /* Param 2 SRC input sampling rate*/
            /* Param 3 SRC input channels*/
            /* Param 4 SRC input sample width*/
            /* Param 5 SRC output buffer address*/
            /* Param 6 SRC output buffer size*/
            /* Param 7 SRC output sampling rate*/
            /* Param 8 return paramter, actual read size of input*/
            /* Param 9 return paramter, actual write size of output*/
            DSP_PRINTF(
                "[DSP_Main] Input buffer addr 0x%X, size %d, rate %d, channels %d, sample width %d, output buffer "
                "addr 0x%X, size %d, rate %d\r\n",
                msg->param[0], msg->param[1], msg->param[2], msg->param[3], msg->param[4], msg->param[5], msg->param[6],
                msg->param[7]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0) ||
                (msg->param[4] == 0) || (msg->param[5] == 0) || (msg->param[6] == 0) || (msg->param[7] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_ServiceNotFound;
                break;
            }
            msg->error = srtm_src(dsp, &msg->param[0]);
            break;

        case SRTM_Command_GAIN:
            /* Param 0 PCM input buffer address*/
            /* Param 1 PCM input buffer size*/
            /* Param 2 PCM output buffer address*/
            /* Param 3 PCM output buffer size*/
            /* Param 4 PCM sampling rate, default 44100*/
            /* Param 5 PCM number of channels, only 1 or 2 supported, default 1*/
            /* Param 6 PCM sample width, default 16*/
            /* Param 7 Gain control index, default is 4, range is 0 to 6 -> {0db, -6db, -12db, -18db, 6db,
            12db,
             * 18db}*/
            /* Param 8 return parameter, actual read bytes*/
            /* Param 9 return parameter, actual written bytes*/
            msg->param[10] = msg->param[0];
            msg->param[11] = msg->param[1];
            msg->param[12] = msg->param[2];
            msg->param[13] = msg->param[3];
            msg->param[14] = msg->param[4];
            msg->param[15] = msg->param[5];
            msg->param[16] = msg->param[6];
            msg->param[17] = msg->param[7];
            DSP_PRINTF(
                "[DSP_Main] Input buffer addr 0x%X, size %d, output buffer addr 0x%X, size %d, sampling rate %d, num "
                "of channels %d, pcm sample width %d, gain control index %d\r\n",
                msg->param[0], msg->param[1], msg->param[2], msg->param[3], msg->param[4], msg->param[5], msg->param[6],
                msg->param[7]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0) ||
                (msg->param[4] == 0) || (msg->param[5] == 0) || (msg->param[6] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
            }
            else
            {
                msg->error = srtm_pcm_gain(dsp, &msg->param[0]);
            }
            break;

        case SRTM_Command_FileStart:
            /* Param 0 File buffer */
            /* Param 1 File buffer size */
            /* Param 2 EOF (true/false) */
            /* Param 3 Audio codec component type */
            /* Param 4 Number of channels */
            /* Param 5 Sample rate */
            /* Param 6 Pcm width */
            if ((msg->param[0] == 0) || (msg->param[1] == 0)
#ifdef MULTICHANNEL
                || ((msg->param[3] == DSP_COMPONENT_NONE) &&
                ((msg->param[4] == 0) || (msg->param[5] == 0) || (msg->param[6] == 0)))
#endif
            )
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
            }
            /* Check to see if file already playing */
            else if (dsp->file_playing)
            {
                msg->error = SRTM_Status_InvalidState;
            }
            else
            {
                DSP_PRINTF("[DSP_Main] File playback start, initial buffer size: %d\r\n", msg->param[1]);
                /* Clear ringbuffer from previous playback */
                ringbuf_clear(dsp->audioBuffer);

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
                remote_addr = (char *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA);
#else
                remote_addr = (char *)msg->param[0];
#endif

                /* Write initial data chunk to the ringbuffer */
                DSP_AudioWriteRing(dsp, remote_addr, msg->param[1]);
                /* Initialize pipeline */
                dsp->eof         = msg->param[2];
                dsp->ipc_waiting = false;
#ifdef MULTICHANNEL
                if (msg->param[3] == DSP_COMPONENT_NONE)
                {
                    msg->error = srtm_file_ren_create(dsp, &msg->param[3]);
                }
                else
#endif
                {
                    msg->error    = srtm_file_dec_create(dsp, msg->param[3]);
                    msg->param[0] = dsp->channel_num;
                }
            }
            break;

        case SRTM_Command_FileData:
            /* Param 0 File buffer */
            /* Param 1 File buffer size */
            /* Param 2 File EOF status (boolean true/false) */
            if (msg->param[0] == 0)
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
            }
            else
            {
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
                remote_addr = (char *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA);
#else
                remote_addr = (char *)msg->param[0];
#endif

                DSP_AudioWriteRing(dsp, remote_addr, msg->param[1]);
                /* Signal EOF to processing thread if set by remote */
                if (msg->param[2])
                {
                    dsp->eof = true;
                }
            }

            dsp->ipc_waiting = false;

            /* Send signal to wake DSP_BufferThread */
            xos_cond_signal(&dsp->ipc_waiting_cond, 0);
            break;

        case SRTM_Command_FileStop:
            /* File must be already playing */
            if (!dsp->file_playing)
            {
                msg->error = SRTM_Status_InvalidState;
            }
            else
            {
                DSP_PRINTF("[DSP_Main] File playback stop\r\n");
                xos_event_set(&dsp->pipeline_event, DSP_EVENT_STOP);
            }
            break;

        /* Unknown message. */
        default:
            msg->head.type = SRTM_MessageTypeNotification;
            msg->error     = SRTM_Status_InvalidMessage;
    }

    return 0;
}

static int DSP_MSG_Process(dsp_handle_t *dsp, srtm_message *msg)
{
    srtm_message_type_t input_type = msg->head.type;

    /* Sanity check */
    if ((msg->head.majorVersion != SRTM_VERSION_MAJOR) || (msg->head.minorVersion != SRTM_VERSION_MINOR))
    {
        DSP_PRINTF("[DSP_Main] SRTM version doesn't match!\r\n");
        return -1;
    }

    /* Process message */
    switch (msg->head.category)
    {
        case SRTM_MessageCategory_GENERAL:
            handleMSG_GENERAL(dsp, msg);
            break;
        case SRTM_MessageCategory_AUDIO:
            handleMSG_AUDIO(dsp, msg);
            break;
        default:
            msg->head.type = SRTM_MessageTypeNotification;
            msg->error     = SRTM_Status_InvalidMessage;
    }

    /* Send response message, if request was received */
    if (input_type == SRTM_MessageTypeRequest)
    {
        /* Prepare response msg */
        msg->head.type = SRTM_MessageTypeResponse;

        /* Send response */
        xos_mutex_lock(&dsp->rpmsgMutex);
        rpmsg_lite_send(dsp->rpmsg, dsp->ept, MCU_EPT_ADDR, (char *)msg, sizeof(srtm_message), RL_DONT_BLOCK);
        xos_mutex_unlock(&dsp->rpmsgMutex);
    }

    return 0;
}

/*!
 * @brief Main function
 */
int DSP_Main(void *arg, int wake_value)
{
    dsp_handle_t *dsp = (dsp_handle_t *)arg;
    void *rpmsg_shmem_base;
    srtm_message msg;
    int status;

    DSP_XAF_Init(dsp);

    SEMA42_Lock(APP_SEMA42, SEMA_STARTUP_NUM, SEMA_CORE_ID_DSP);

    DSP_PRINTF("[DSP_Main] start\r\n");

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    rpmsg_shmem_base = (void *)MEMORY_ConvertMemoryMapAddress((uint32_t)RPMSG_LITE_SHMEM_BASE, kMEMORY_Local2DMA);
#else
    rpmsg_shmem_base = RPMSG_LITE_SHMEM_BASE;
#endif

    dsp->rpmsg       = rpmsg_lite_remote_init(rpmsg_shmem_base, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
    dsp->rpmsg_queue = rpmsg_queue_create(dsp->rpmsg);

    rpmsg_lite_wait_for_link_up(dsp->rpmsg, RL_BLOCK);

    DSP_PRINTF("[DSP_Main] established RPMsg link\r\n");

#if (XF_TRACE && DUMP_TRACE_TO_BUF)
    // For trace buffer, to store 512kB of trace messages
    void * buffer_p = (void *) malloc(TRACE_BUF_SIZE);
    if (buffer_p != NULL)
    {
        TRACE_INIT("Xtensa Audio Framework - \'Playback\' Sample App", buffer_p, TRACE_BUF_SIZE);
    }
    else
    {
        DSP_PRINTF("[DSP_Main] Trace buffer could not be allocated!\r\n");
    }
#endif

    SEMA42_Unlock(APP_SEMA42, SEMA_STARTUP_NUM);

    dsp->ept = rpmsg_lite_create_ept(dsp->rpmsg, DSP_EPT_ADDR, rpmsg_queue_rx_cb, (void *)dsp->rpmsg_queue);

    while (1)
    {
        /* Block until receive message from ARM core */
        status =
            rpmsg_queue_recv(dsp->rpmsg, dsp->rpmsg_queue, NULL, (char *)&msg, sizeof(srtm_message), NULL, RL_BLOCK);
        if (status != RL_SUCCESS)
        {
            xos_fatal_error(status, "Failed to get item from RPMsg queue.\r\n");
        }

        /* Process request */
        status = DSP_MSG_Process(dsp, &msg);
    }

    rpmsg_lite_destroy_ept(dsp->rpmsg, dsp->ept);
    rpmsg_lite_deinit(dsp->rpmsg);

    return 0;
}

int main(void)
{
    XosThread thread_main;

    BOARD_InitHardware();

    xos_thread_create(&thread_main, NULL, DSP_Main, &dsp, "DSP Main", dsp_thread_stack, DSP_THREAD_STACK_SIZE,
                      DSP_THREAD_PRIORITY, 0, 0);

    /* Start XOS scheduler - does not return */
    xos_start(0);

    /* Should not reach this statement. */
    return 0;
}
