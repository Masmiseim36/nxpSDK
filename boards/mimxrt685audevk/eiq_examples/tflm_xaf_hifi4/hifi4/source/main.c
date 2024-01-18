/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include <string.h>
#include <xtensa/xos.h>
#include <xtensa/config/core.h>

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif
#include "board_init.h"
#include "dsp_config.h"
#include "dsp_xaf.h"
#include "fsl_debug_console.h"
#include "message.h"
#include "rpmsg_lite.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DSP_THREAD_STACK_SIZE (8 * 1024)
#define DSP_THREAD_PRIORITY   (XOS_MAX_PRIORITY - 3)

#define RPMSG_QUEUE_SIZE 8

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if (XCHAL_DCACHE_SIZE > 0)
extern int NonCacheable_start, NonCacheable_end;
extern int NonCacheable_init_start, NonCacheable_init_end;
#endif

/*! @brief Main application handle */
static dsp_handle_t dsp;

/*! @brief Main thread stack area */
static uint8_t dsp_thread_stack[DSP_THREAD_STACK_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Invoked when message is received from RPMsg-Lite
 */
static int rpmsg_callback(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    XosMsgQueue *queue = (XosMsgQueue *)priv;
    int status;

    if (payload_len == sizeof(message_t))
    {
        status = xos_msgq_put(queue, payload);
        if (status != XOS_OK)
        {
            xos_fatal_error(status, "Failed to put RPMsg message into queue.\r\n");
        }
    }
    else
    {
        /* Error / invalid message received. */
        xos_fatal_error(XOS_ERR_INVALID_PARAMETER,
                        "RPMsg message with invalid length received from the other core.\r\n");
    }

    return RL_RELEASE;
}

/*!
 * @brief Initializes RPMsg-Lite communication and wait until link with another core is up
 */
static void rpmsg_lite_init(dsp_handle_t *dsp)
{
    static struct rpmsg_lite_ept_static_context ept_ctx;
    static struct rpmsg_lite_instance rpmsg_ctx;
    void *rpmsg_shmem_base;
    int status = -1;

    dsp->rpmsg_queue = malloc(XOS_MSGQ_SIZE(RPMSG_QUEUE_SIZE, sizeof(message_t)));
    if (dsp->rpmsg_queue == NULL)
    {
        xos_fatal_error(status, "Failed to allocate RPMsg queue memory.\r\n");
    }

    status = xos_msgq_create(dsp->rpmsg_queue, RPMSG_QUEUE_SIZE, sizeof(message_t), XOS_MSGQ_WAIT_PRIORITY);
    if (status != XOS_OK)
    {
        xos_fatal_error(status, "Failed to create RPMsg queue.\r\n");
    }

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    rpmsg_shmem_base = (void *)MEMORY_ConvertMemoryMapAddress((uint32_t)RPMSG_LITE_SHMEM_BASE, kMEMORY_Local2DMA);
#else
    rpmsg_shmem_base = RPMSG_LITE_SHMEM_BASE;
#endif

    dsp->rpmsg = rpmsg_lite_remote_init(rpmsg_shmem_base, RPMSG_LITE_LINK_ID, RL_NO_FLAGS, &rpmsg_ctx);

    while (!rpmsg_lite_is_link_up(dsp->rpmsg))
    {
    }

    PRINTF("[main_dsp] Established RPMsg link\r\n");

    dsp->ept = rpmsg_lite_create_ept(dsp->rpmsg, DSP_EPT_ADDR, (rl_ept_rx_cb_t)rpmsg_callback, (void *)dsp->rpmsg_queue,
                                     &ept_ctx);
}

/*!
 * @brief Starts playback thread
 */
static void playback_start(dsp_handle_t *dsp)
{
    int status;

    status = dsp_xaf_start_thread(dsp);
    if (status != 0)
    {
        xos_fatal_error(status, "dsp_xaf_start_thread() failed.\r\n");
    }
}

/*!
 * @brief Main thread function
 */
static int dsp_main_thread(void *arg, int wake_value)
{
    dsp_handle_t *dsp = (dsp_handle_t *)arg;
    message_t msg;
    int status;

    PRINTF("[main_dsp] Start\r\n");

    /* XAF related initializations */
    dsp_xaf_init(dsp);

    /* Initialize RPMsg-Lite */
    rpmsg_lite_init(dsp);

    /* Initialize audio playback processing chain */
    playback_start(dsp);

    /* Wait until audio processing is ready */
    while (dsp->state != DspState_Running)
    {
        xos_thread_sleep_msec(10);
    }

    /* Receive RPMsg-Lite messages */
    while (true)
    {
        /* Block until inference is done */
        status = xos_sem_get(&dsp->inference_done);
        if (status != XOS_OK)
        {
            xos_fatal_error(status, "Failed to wait for inference.\r\n");
        }

        msg.params[0] = dsp->inference_results[0];
        msg.params[1] = dsp->inference_results[1];
        msg.params[2] = dsp->inference_results[2];

        /* Process request */
        status = rpmsg_lite_send(dsp->rpmsg, dsp->ept, MCU_EPT_ADDR, (char*)&msg, sizeof(message_t), RL_BLOCK);
        if (status != RL_SUCCESS)
        {
            PRINTF("rpmsg_lite_send failed: %d\r\n", status);
        }
    }

    /* Cleanup - will not get here anyway */
    rpmsg_lite_destroy_ept(dsp->rpmsg, dsp->ept);
    rpmsg_lite_deinit(dsp->rpmsg);

    return 0;
}

/*!
 * @brief Main function
 */
int main(void)
{
    XosThread thread_main;

    /* Disable DSP cache for RPMsg-Lite shared memory. */
    xthal_set_region_attribute((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_SHMEM_SIZE, XCHAL_CA_BYPASS, 0);

#if (XCHAL_DCACHE_SIZE > 0)
    /* Disable DSP cache for noncacheable sections. */
    xthal_set_region_attribute((uint32_t *)&NonCacheable_start,
                               (uint32_t)&NonCacheable_end - (uint32_t)&NonCacheable_start, XCHAL_CA_BYPASS, 0);
    xthal_set_region_attribute((uint32_t *)&NonCacheable_init_start,
                               (uint32_t)&NonCacheable_init_end - (uint32_t)&NonCacheable_init_start, XCHAL_CA_BYPASS,
                               0);
#endif

    BOARD_Init();

    /* Create main thread */
    xos_thread_create(&thread_main, NULL, dsp_main_thread, &dsp, "dsp_main", dsp_thread_stack, DSP_THREAD_STACK_SIZE,
                      DSP_THREAD_PRIORITY, 0, 0);

    /* Start XOS scheduler - does not return */
    xos_start(0);

    /* Should not reach this statement */
    return 0;
}
