/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dsp_config.h"
#include "dsp_xaf.h"
#include "message.h"
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "rtp.h"

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

#include <xtensa/xos.h>

#include <xtensa/config/core.h>
#include "board_hifi4.h"
#include "fsl_gpio.h"
#include "fsl_inputmux.h"
#include "fsl_dma.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define INIT_DEBUG_CONSOLE 0
#define BOARD_XTAL_SYS_CLK_HZ 24000000U /*!< Board xtal_sys frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ  32768U    /*!< Board xtal32K frequency in Hz */

#define DSP_THREAD_STACK_SIZE (8 * 1024)
#define DSP_THREAD_PRIORITY   (XOS_MAX_PRIORITY - 3)

/* Must be same size as number of RTP buffers on CM33 side so buffers are not lost */
#define RPMSG_QUEUE_SIZE SHARED_BUFFER_COUNT

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern int NonCacheable_start, NonCacheable_end;
extern int NonCacheable_init_start, NonCacheable_init_end;

/*! @brief Main application handle */
static dsp_handle_t dsp;

/*! @brief Main thread stack area */
static uint8_t dsp_thread_stack[DSP_THREAD_STACK_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_InitClock(void)
{
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* sets external XTAL OSC freq */

    CLOCK_EnableClock(kCLOCK_InputMux);

    /* DSP_INT0_SEL18 = DMA1 */
    INPUTMUX_AttachSignal(INPUTMUX, 18U, kINPUTMUX_Dmac1ToDspInterrupt);
    /* MUB interrupt signal is selected for DSP interrupt input 1 */
    INPUTMUX_AttachSignal(INPUTMUX, 1U, kINPUTMUX_MuBToDspInterrupt);

    /* attach AUDIO PLL clock to FLEXCOMM1 (I2S1) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM1);
    /* attach AUDIO PLL clock to FLEXCOMM3 (I2S3) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM3);
}

static void XOS_Init(void)
{
    xos_set_clock_freq(XOS_CLOCK_FREQ);

    xos_start_system_timer(-1, 0);
}



/*!
 * @brief Initializes RPMsg-Lite communication and wait until link with another core is up
 */
static void rpmsg_lite_init(dsp_handle_t *dsp)
{
    void *rpmsg_shmem_base;

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    rpmsg_shmem_base = (void *)MEMORY_ConvertMemoryMapAddress((uint32_t)RPMSG_LITE_SHMEM_BASE, kMEMORY_Local2DMA);
#else
    rpmsg_shmem_base = RPMSG_LITE_SHMEM_BASE;
#endif

    dsp->rpmsg       = rpmsg_lite_remote_init(rpmsg_shmem_base, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
    dsp->rpmsg_queue = rpmsg_queue_create(dsp->rpmsg);

    rpmsg_lite_wait_for_link_up(dsp->rpmsg, RL_BLOCK);

    DSP_PRINTF("[main_dsp] established RPMsg link\r\n");

    dsp->ept = rpmsg_lite_create_ept(dsp->rpmsg, DSP_EPT_ADDR, rpmsg_queue_rx_cb, (void *)dsp->rpmsg_queue);
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
 * @brief Enqueues input data for playback
 */
static void command_write(dsp_handle_t *dsp, message_t *msg)
{
    uint32_t buffer_address = 0U;
    uint32_t buffer_size    = 0U;
    int i;

    for (i = 0; i < MESSAGE_PARAMS_MAX; i += 2)
    {
        if (msg->params[i] == MessageParam_NULL)
        {
            break;
        }
        else if (msg->params[i] == MessageParam_PacketAddress)
        {
            buffer_address = msg->params[i + 1];
        }
        else if (msg->params[i] == MessageParam_PacketSize)
        {
            buffer_size = msg->params[i + 1];
        }
    }

    if ((buffer_address != 0U) && (buffer_size != 0U))
    {
        dsp_xaf_add_packet(dsp, (rtp_header_t *)buffer_address, buffer_size);
    }
}

/*!
 * @brief Handles incoming message from ARM core
 */
static void handle_message(dsp_handle_t *dsp, message_t *msg)
{
    switch (msg->command)
    {
        case AudioCommand_PlayPacket:
        {
            command_write(dsp, msg);
            break;
        }

        /* Unknown / unimplemented message */
        default:
        {
            break;
        }
    }
}

/*!
 * @brief Main thread function
 */
static int dsp_main_thread(void *arg, int wake_value)
{
    dsp_handle_t *dsp = (dsp_handle_t *)arg;
    message_t msg;
    int32_t status;

    DSP_PRINTF("[main_dsp] start\r\n");

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
        /* Block until received message from ARM core */
        status = rpmsg_queue_recv(dsp->rpmsg, dsp->rpmsg_queue, NULL, (char *)&msg, sizeof(message_t), NULL, RL_BLOCK);
        if (status != RL_SUCCESS)
        {
            xos_fatal_error(status, "Failed to get item from RPMsg queue.\r\n");
        }
        /* Process request */
        handle_message(dsp, &msg);
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

    /* Disable DSP cache for noncacheable sections. */
    xthal_set_region_attribute((uint32_t *)&NonCacheable_start,
                               (uint32_t)&NonCacheable_end - (uint32_t)&NonCacheable_start, XCHAL_CA_BYPASS, 0);
    xthal_set_region_attribute((uint32_t *)&NonCacheable_init_start,
                               (uint32_t)&NonCacheable_init_end - (uint32_t)&NonCacheable_init_start, XCHAL_CA_BYPASS,
                               0);

    XOS_Init();
    BOARD_InitBootPins();
#if INIT_DEBUG_CONSOLE || APP_DSP_ONLY
    BOARD_InitDebugConsole();
#endif
    BOARD_InitClock();

    /* Iniitalize DMA1 which will be shared by capturer and renderer. */
    DMA_Init(DMA1);

    /* Map DMA IRQ handler to INPUTMUX selection DSP_INT0_SEL18
     * EXTINT19 = DSP INT 23 */
    xos_register_interrupt_handler(XCHAL_EXTINT19_NUM, (XosIntFunc *)DMA_IRQHandle, DMA1);
    xos_interrupt_enable(XCHAL_EXTINT19_NUM);

    /* Create main thread */
    xos_thread_create(&thread_main, NULL, dsp_main_thread, &dsp, "dsp_main", dsp_thread_stack, DSP_THREAD_STACK_SIZE,
                      DSP_THREAD_PRIORITY, 0, 0);

    /* Start XOS scheduler - does not return */
    xos_start(0);

    /* Should not reach this statement */
    return 0;
}
