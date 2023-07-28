/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "rpmsg_lite.h"
#include "srtm_config.h"
#include "fsl_debug_console.h"
#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include "dsp_nn.h"

#include "board_hifi4.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_inputmux.h"
#include "fsl_dma.h"
#include "pin_mux.h"
#include "dsp_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_XTAL_SYS_CLK_HZ 24000000U /*!< Board xtal_sys frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ  32768U    /*!< Board xtal32K frequency in Hz */
#define NOTIF_EVT_RPMSG_RECEIVED_DATA (1 << 0)
#define NOTIF_EVT                     (NOTIF_EVT_RPMSG_RECEIVED_DATA)

typedef struct _rpmsg_user_data_t
{
    int has_received;
    XosEvent *notif_evt;
} rpmsg_user_data_t;

typedef struct the_message
{
    srtm_message DAT;
} THE_MESSAGE, *THE_MESSAGE_PTR;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

THE_MESSAGE msg;
unsigned long remote_addr;

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
}

static void XOS_Init(void)
{
    xos_set_clock_freq(XOS_CLOCK_FREQ);

    xos_start_system_timer(-1, 0);
}



/* Internal functions */
static int32_t my_ept_read_cb(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    rpmsg_user_data_t *rpmsg_user_data = priv;

    if (payload_len <= sizeof(THE_MESSAGE))
    {
        memcpy((void *)&msg, payload, payload_len);
        remote_addr                   = src;
        rpmsg_user_data->has_received = 1;
        xos_event_clear_and_set(rpmsg_user_data->notif_evt, XOS_EVENT_BITS_ALL, NOTIF_EVT_RPMSG_RECEIVED_DATA);
    }
    return RL_RELEASE;
}

int handleMSG_GENERAL(void)
{
    void *addr;

    switch (msg.DAT.head.command)
    {
        /* Return SDK key component versions to host*/
        case SRTM_Command_ECHO:
            // 6 NNLib version high 16 bits major, lower 16 bits minor
            msg.DAT.param[6] = ((2) << 16 | (1));
            break;

        case SRTM_Command_xa_nn_malloc:
            addr = malloc((unsigned int)msg.DAT.param[0]);
            if (addr == NULL)
            {
                msg.DAT.head.type = SRTM_MessageTypeNotification;
                msg.DAT.error     = SRTM_Status_OutOfMemory;
            }
            else
            {
                msg.DAT.error    = SRTM_Status_Success;
                msg.DAT.param[0] = (unsigned int)addr;
            }

            break;

        case SRTM_Command_xa_nn_free:
            addr = (void *)(msg.DAT.param[0]);
            if (addr == NULL)
            {
                msg.DAT.head.type = SRTM_MessageTypeNotification;
                msg.DAT.error     = SRTM_Status_InvalidParameter;
            }
            else
            {
                msg.DAT.error = SRTM_Status_Success;
                free(addr);
            }

            break;

        /* Unknown message. */
        default:
            msg.DAT.head.type = SRTM_MessageTypeNotification;
            msg.DAT.error     = SRTM_Status_InvalidMessage;
    }

    return 0;
}

int handleMSG(void)
{
    /* Sanity check */
    if ((msg.DAT.head.majorVersion != SRTM_VERSION_MAJOR) || (msg.DAT.head.minorVersion != SRTM_VERSION_MINOR))
    {
        return -1;
    }
    if (msg.DAT.head.type != SRTM_MessageTypeRequest)
    {
        return -1;
    }

    /* Prepare response msg */
    msg.DAT.head.type = SRTM_MessageTypeResponse;
    // Process each of messages
    switch (msg.DAT.head.category)
    {
        case SRTM_MessageCategory_GENERAL:
            handleMSG_GENERAL();
            break;
        case SRTM_MessageCategory_NN:
            handleMSG_NN(&msg.DAT);
            break;
        default:
            msg.DAT.head.type = SRTM_MessageTypeNotification;
            msg.DAT.error     = SRTM_Status_InvalidMessage;
    }
    return 0;
}

/*!
 * @brief Main function
 */
int SCHEDULER_MULTICORE(void)
{
    int msg_count = 0;
    int eventStatus;
    struct rpmsg_lite_ept_static_context my_ept_context;
    struct rpmsg_lite_endpoint *my_ept;
    struct rpmsg_lite_instance rpmsg_ctxt;
    struct rpmsg_lite_instance *my_rpmsg;
    rpmsg_user_data_t rpmsg_user_data;
    XosEvent notif_evt;

    /* DSP test bit set */
    /* Initialize standard SDK demo application pins */
    my_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS, &rpmsg_ctxt);

    while (RL_TRUE != rpmsg_lite_is_link_up(my_rpmsg))
        ;

    rpmsg_user_data.has_received = 0;
    rpmsg_user_data.notif_evt    = &notif_evt;
    xos_event_create(&notif_evt, NOTIF_EVT, 0);

    my_ept = rpmsg_lite_create_ept(my_rpmsg, DSP_EPT_ADDR, my_ept_read_cb, (void *)&rpmsg_user_data, &my_ept_context);

    /* Send an empty message signaling DSP is ready */
    memset(&msg, 0, sizeof(srtm_message));
    rpmsg_lite_send(my_rpmsg, my_ept, MCU_EPT_ADDR, (char*)&msg, sizeof(srtm_message), RL_DONT_BLOCK);

    while (1)
    {
        if (rpmsg_user_data.has_received)
        {
            rpmsg_user_data.has_received = 0;
            msg_count++;

            int status = handleMSG();
            xthal_dcache_all_writeback_inv();
            if (status == 0)
            {
                /*Send response message*/
                rpmsg_lite_send(my_rpmsg, my_ept, remote_addr, (char *)&msg, sizeof(THE_MESSAGE), RL_DONT_BLOCK);
            }
        }
        else
        {
            eventStatus = xos_event_wait_any(&notif_evt, NOTIF_EVT_RPMSG_RECEIVED_DATA);
            if (eventStatus != XOS_OK)
                continue;
            xos_event_clear(&notif_evt, NOTIF_EVT_RPMSG_RECEIVED_DATA);
        }
    }

    rpmsg_lite_destroy_ept(my_rpmsg, my_ept);
    my_ept = NULL;
    rpmsg_lite_deinit(my_rpmsg);

    return 0;
}
int main()
{
    xos_start_main("main", 7, 0);

    /* Setup cache */
    xthal_set_region_attribute((void *)0x00000000, 0x480000, XCHAL_CA_WRITETHRU, 0);
    xthal_set_region_attribute((void *)0x20000000, 0x480000, XCHAL_CA_BYPASS, 0);

    XOS_Init();
    BOARD_InitBootPins();
    BOARD_InitClock();

    /* Iniitalize DMA1 which will be shared by capturer and renderer. */
    DMA_Init(DMA1);

    /* Map DMA IRQ handler to INPUTMUX selection DSP_INT0_SEL18
     * EXTINT19 = DSP INT 23 */
    xos_register_interrupt_handler(XCHAL_EXTINT19_NUM, (XosIntFunc *)DMA_IRQHandle, DMA1);
    xos_interrupt_enable(XCHAL_EXTINT19_NUM);

    SCHEDULER_MULTICORE();

    return 0;
}
