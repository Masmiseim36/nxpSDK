/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_lpuart.h"
#include "fsl_irqsteer.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RPMSG_LITE_SHMEM_BASE (0x90010000U)
#define RPMSG_LITE_LINK_ID (RL_PLATFORM_IMX8QM_M4_A_USER_LINK_ID)
#define RPMSG_LITE_NS_ANNOUNCE_STRING "rpmsg-virtual-tty-channel"
#define APP_TASK_STACK_SIZE (256)
#ifndef LOCAL_EPT_ADDR
#define LOCAL_EPT_ADDR (30)
#endif

/* Globals */
static char app_buf[512]; /* Each RPMSG buffer can carry less than 512 payload */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
TaskHandle_t app_task_handle = NULL;

void app_nameservice_isr_cb(unsigned int new_ept, const char *new_ept_name, unsigned long flags, void *user_data)
{
}

void app_task(void *param)
{
    volatile unsigned long remote_addr;
    struct rpmsg_lite_endpoint *volatile my_ept;
    volatile rpmsg_queue_handle my_queue;
    struct rpmsg_lite_instance *volatile my_rpmsg;
    void *rx_buf;
    int len;
    int result;
    void *tx_buf;
    unsigned long size;

    /* Print the initial banner */
    PRINTF("\r\nRPMSG String Echo FreeRTOS RTOS API Demo...\r\n");

#ifdef MCMGR_USED
    uint32_t startupData;

    /* Get the startup data */
    MCMGR_GetStartupData(kMCMGR_Core1, &startupData);

    my_rpmsg = rpmsg_lite_remote_init((void *)startupData, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);

    /* Signal the other core we are ready */
    MCMGR_SignalReady(kMCMGR_Core1);
#else
    my_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
#endif /* MCMGR_USED */

    while (!rpmsg_lite_is_link_up(my_rpmsg))
        ;

    my_queue = rpmsg_queue_create(my_rpmsg);
    my_ept   = rpmsg_lite_create_ept(my_rpmsg, LOCAL_EPT_ADDR, rpmsg_queue_rx_cb, my_queue);
    rpmsg_ns_bind(my_rpmsg, app_nameservice_isr_cb, NULL);
    rpmsg_ns_announce(my_rpmsg, my_ept, RPMSG_LITE_NS_ANNOUNCE_STRING, RL_NS_CREATE);

    PRINTF("\r\nNameservice sent, ready for incoming messages...\r\n");

    for (;;)
    {
        /* Get RPMsg rx buffer with message */
        result = rpmsg_queue_recv_nocopy(my_rpmsg, my_queue, (unsigned long *)&remote_addr, (char **)&rx_buf, &len,
                                         RL_BLOCK);
        if (result != 0)
        {
            assert(false);
        }

        /* Copy string from RPMsg rx buffer */
        assert(len < sizeof(app_buf));
        memcpy(app_buf, rx_buf, len);
        app_buf[len] = 0; /* End string by '\0' */

        if ((len == 2) && (app_buf[0] == 0xd) && (app_buf[1] == 0xa))
            PRINTF("Get New Line From Master Side\r\n");
        else
            PRINTF("Get Message From Master Side : \"%s\" [len : %d]\r\n", app_buf, len);

        /* Get tx buffer from RPMsg */
        tx_buf = rpmsg_lite_alloc_tx_buffer(my_rpmsg, &size, RL_BLOCK);
        assert(tx_buf);
        /* Copy string to RPMsg tx buffer */
        memcpy(tx_buf, app_buf, len);
        /* Echo back received message with nocopy send */
        result = rpmsg_lite_send_nocopy(my_rpmsg, my_ept, remote_addr, tx_buf, len);
        if (result != 0)
        {
            assert(false);
        }
        /* Release held RPMsg rx buffer */
        result = rpmsg_queue_nocopy_free(my_rpmsg, rx_buf);
        if (result != 0)
        {
            assert(false);
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize standard SDK demo application pins */
    sc_ipc_t ipc;
    ipc = BOARD_InitRpc();

    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();

    /* Power up the MU used for RPMSG */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_MU_5B, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on MU!\r\n");
    }
    if (sc_pm_set_resource_power_mode(ipc, SC_R_MU_7A, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on MU!\r\n");
    }

    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTEER!\r\n");
    }

    IRQSTEER_Init(IRQSTEER);

#ifdef MCMGR_USED
    /* Initialize MCMGR before calling its API */
    MCMGR_Init();
#endif /* MCMGR_USED */

    if (xTaskCreate(app_task, "APP_TASK", APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &app_task_handle) != pdPASS)
    {
        PRINTF("\r\nFailed to create application task\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();

    PRINTF("Failed to start FreeRTOS on core0.\n");
    while (1)
        ;
}
