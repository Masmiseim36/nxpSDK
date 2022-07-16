/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_lite.h"
#include "srtm_config.h"

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include "dsp_nn.h"

#include "fsl_gpio.h"

#include "board_fusionf1.h"
#include "fsl_inputmux.h"
#include "fsl_dma.h"
#include "pin_mux.h"
#include "dsp_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_XTAL_SYS_CLK_HZ 24000000U /*!< Board xtal_sys frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ  32768U    /*!< Board xtal32K frequency in Hz */
#define INIT_DEBUG_CONSOLE 0

#define APP_RPMSG_READY_EVENT_DATA    (1)
#define APP_RPMSG_EP_READY_EVENT_DATA (2)

#define NOTIF_EVT_RPMSG_RECEIVED_DATA (1 << 0)
#define NOTIF_EVT                     (NOTIF_EVT_RPMSG_RECEIVED_DATA)
#define DSP_THREAD_STACK_SIZE (8 * 1024)
#define DSP_THREAD_PRIORITY   (XOS_MAX_PRIORITY - 3)

#define AUDIO_BUFFER_SIZE (32 * 1024)


#if (INIT_DEBUG_CONSOLE == 1)
#define DSP_PRINTF PRINTF
#else
#define DSP_PRINTF printf
#endif

typedef struct dsp_handle_t_
{
    struct rpmsg_lite_instance *rpmsg;
    struct rpmsg_lite_endpoint *ept;
    XosMsgQueue *rpmsg_queue;

    XosMutex rpmsgMutex;
} dsp_handle_t;


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* SRTM prototypes */

/*******************************************************************************
 * Variables
 ******************************************************************************/

dsp_handle_t dsp;
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

    /* I2S clocks */
    /* attach AUDIO PLL clock to FLEXCOMM1 (I2S1) for codec output*/
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM1);
    /* attach AUDIO PLL clock to FLEXCOMM5 (I2S5) for I2S capture*/
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM5);

    /* DMIC uses 24.576MHz AUDIO_PLL clock */
    CLOCK_AttachClk(kAUDIO_PLL_to_DMIC);

    /* 24.576MHz divided by 12 = 2048 KHz PDM clock --> gives 16kHz sample rate */
    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 12);

    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM4);
}

static void XOS_Init(void)
{
    xos_set_clock_freq(XOS_CLOCK_FREQ);

    xos_start_system_timer(-1, 0);
}


static int rpmsg_callback(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    XosMsgQueue *queue = (XosMsgQueue *)priv;

    if (payload_len == sizeof(srtm_message))
    {
        xos_msgq_put(queue, payload);
    }
    else
    {
        /* Error / invalid message received. */
    }

    return RL_RELEASE;
}

static int handleMSG_GENERAL(dsp_handle_t *dsp, srtm_message *msg)
{
    void *addr;

    switch (msg->head.command)
    {
        /* Return SDK key component versions to host*/
        case SRTM_Command_ECHO:
            // 6 NNLib version high 16 bits major, lower 16 bits minor
            msg->param[6] = ((2) << 16 | (1));
            DSP_PRINTF("NNLib Lib version %d.%d\r\n", msg->param[6] >> 16, msg->param[6] & 0xFF);
            break;

        case SRTM_Command_xa_nn_malloc:
            addr = malloc((unsigned int)msg->param[0]);
            if (addr == NULL)
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_OutOfMemory;
            }
            else
            {
                msg->error    = SRTM_Status_Success;
                msg->param[0] = (unsigned int)addr;
            }

            break;

        case SRTM_Command_xa_nn_free:
            addr = (void *)(msg->param[0]);
            if (addr == NULL)
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
            }
            else
            {
                msg->error = SRTM_Status_Success;
                free(addr);
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
        DSP_PRINTF("SRTM version doesn't match!\r\n");
        return -1;
    }

    /* Process message */
    switch (msg->head.category)
    {
        case SRTM_MessageCategory_GENERAL:
            handleMSG_GENERAL(dsp, msg);
            break;
        case SRTM_MessageCategory_NN:
            handleMSG_NN(msg);
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
    struct rpmsg_lite_ept_static_context ept_ctx;
    struct rpmsg_lite_instance rpmsg_ctx;
    void *rpmsg_shmem_base;
    srtm_message msg;
    int status;


    DSP_PRINTF("[DSP_Main] start\r\n");

    dsp->rpmsg_queue = malloc(XOS_MSGQ_SIZE(10, sizeof(srtm_message)));
    xos_msgq_create(dsp->rpmsg_queue, 10, sizeof(srtm_message), XOS_MSGQ_WAIT_PRIORITY);

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    rpmsg_shmem_base = (void *)MEMORY_ConvertMemoryMapAddress((uint32_t)RPMSG_LITE_SHMEM_BASE, kMEMORY_Local2DMA);
#else
    rpmsg_shmem_base = RPMSG_LITE_SHMEM_BASE;
#endif

    dsp->rpmsg = rpmsg_lite_remote_init(rpmsg_shmem_base, RPMSG_LITE_LINK_ID, RL_NO_FLAGS, &rpmsg_ctx);

    while (!rpmsg_lite_is_link_up(dsp->rpmsg))
    {
    }

    DSP_PRINTF("[DSP_Main] established RPMsg link\r\n");

    dsp->ept = rpmsg_lite_create_ept(dsp->rpmsg, DSP_EPT_ADDR, (rl_ept_rx_cb_t)rpmsg_callback, (void *)dsp->rpmsg_queue,
                                     &ept_ctx);

    while (1)
    {
        /* Block until receive message from ARM core */
        xos_msgq_get(dsp->rpmsg_queue, (uint32_t *)&msg);

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

    XOS_Init();
    BOARD_InitPins();
    BOARD_InitClock();
#if INIT_DEBUG_CONSOLE || APP_DSP_ONLY
    BOARD_InitDebugConsole();
#endif

    /* Iniitalize DMA1 which will be shared by capturer and renderer. */
    DMA_Init(DMA1);

    /* Map DMA IRQ handler to INPUTMUX selection DSP_INT0_SEL18
     * EXTINT19 = DSP INT 23 */
    xos_register_interrupt_handler(XCHAL_EXTINT19_NUM, (XosIntFunc *)DMA_IRQHandle, DMA1);
    xos_interrupt_enable(XCHAL_EXTINT19_NUM);

    xos_thread_create(&thread_main, NULL, DSP_Main, &dsp, "DSP Main", dsp_thread_stack, DSP_THREAD_STACK_SIZE,
                      DSP_THREAD_PRIORITY, 0, 0);

    /* Start XOS scheduler - does not return */
    xos_start(0);

    /* Should not reach this statement. */
    return 0;
}

