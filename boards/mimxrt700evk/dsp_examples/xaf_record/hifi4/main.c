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
#define DSP_MAIN_THREAD_STACK_SIZE (20 * 1024)
#define DSP_MAIN_THREAD_PRIORITY   (XOS_MAX_PRIORITY - 3)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* SRTM prototypes */
int srtm_capturer_gain_renderer_init(unsigned int *pCmdParams, bool i2s);

/*******************************************************************************
 * Variables
 ******************************************************************************/

dsp_handle_t dsp;
static uint8_t dsp_main_thread_stack[DSP_MAIN_THREAD_STACK_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/

static void DSP_XAF_Init(dsp_handle_t *dsp)
{
    uint8_t *version[3];

    xos_mutex_create(&dsp->rpmsgMutex, XOS_MUTEX_WAIT_PRIORITY, 0);

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
            msg->param[0] = ((2) << 16 | (0));
            // 1 Audio Framework API version high 16 bits major, lower 16 bits minor
            msg->param[1] = ((1) << 16 | (3));
            // 2 NatureDSP Lib version high 16 bits major, lower 16 bits minor
            msg->param[2] = ((4) << 16 | (0));
            // 3 NatureDSP Lib API version high 16 bits major, lower 16 bits minor
            msg->param[3] = ((4) << 16 | (10));
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
    switch (msg->head.command)
    {
        case SRTM_Command_REC_DMIC:
            /* Param 0 Number of Channels*/
            /* Param 1 Sampling Rate*/
            /* Param 2 PCM bit Width*/
            DSP_PRINTF("[DSP_Main] Number of channels %d, sampling rate %d, PCM width %d\r\n", msg->param[0],
                       msg->param[1], msg->param[2]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
            }
            else
            {
#if XA_VIT_PRE_PROC
                if ((msg->param[3] == 0) || (msg->param[4] == 0))
                {
                    msg->head.type = SRTM_MessageTypeNotification;
                    msg->error     = SRTM_Status_InvalidParameter;
                }
                else
                {
                    dsp->VITModelCM_33     = (unsigned char *)msg->param[3];
                    dsp->size_of_VIT_model = (uint32_t)msg->param[4];
                }
                msg->head.command = SRTM_Command_VIT;
#endif
                // since srtm_capturer_gain_renderer_init is not running as separate thread and never returns, there is
                // need to send response before it starts
                msg->head.type = SRTM_MessageTypeResponse;
                xos_mutex_lock(&dsp->rpmsgMutex);
                rpmsg_lite_send(dsp->rpmsg, dsp->ept, MCU_EPT_ADDR, (char *)msg, sizeof(srtm_message), RL_DONT_BLOCK);
                xos_mutex_unlock(&dsp->rpmsgMutex);

                msg->error = srtm_capturer_gain_renderer_init(&msg->param[0], false);
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

    xos_thread_create(&thread_main, NULL, DSP_Main, &dsp, "DSP Main", dsp_main_thread_stack, DSP_MAIN_THREAD_STACK_SIZE,
                      DSP_MAIN_THREAD_PRIORITY, 0, 0);

    /* Start XOS scheduler - does not return */
    xos_start(0);

    /* Should not reach this statement. */
    return 0;
}
