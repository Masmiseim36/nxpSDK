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
#include "fsl_irqsteer.h"
#include "app_srtm.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RPMSG_LITE_LINK_ID (RL_PLATFORM_IMX8QM_M4_M4_USER_LINK_ID)
#define RPMSG_LITE_SHMEM_BASE 0x90200000
#define RPMSG_LITE_NS_ANNOUNCE_STRING "rpmsg-openamp-demo-channel"
#define APP_TASK_STACK_SIZE (256)
#ifndef LOCAL_EPT_ADDR
#define LOCAL_EPT_ADDR (30)
#endif
#define APP_RPMSG_READY_EVENT_DATA (1)

typedef struct the_message
{
    uint32_t DATA;
} volatile THE_MESSAGE, *THE_MESSAGE_PTR;

volatile THE_MESSAGE msg = {0};
#ifdef RPMSG_LITE_MASTER_IS_LINUX
static char helloMsg[13];
#endif /* RPMSG_LITE_MASTER_IS_LINUX */

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

#ifdef MCMGR_USED
/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    MCMGR_EarlyInit();
}
#endif /* MCMGR_USED */

void app_task(void *param)
{
    volatile unsigned long remote_addr;
    struct rpmsg_lite_endpoint *volatile my_ept;
    volatile rpmsg_queue_handle my_queue;
    struct rpmsg_lite_instance *volatile my_rpmsg;
    volatile rpmsg_ns_handle ns_handle;

    /* Print the initial banner */
    PRINTF("\r\nRPMSG Ping-Pong FreeRTOS RTOS API Demo...\r\n");

#ifdef MCMGR_USED
    uint32_t startupData;
    mcmgr_status_t status;

    /* Get the startup data */
    do
    {
        status = MCMGR_GetStartupData(&startupData);
    } while (status != kStatus_MCMGR_Success);

    my_rpmsg = rpmsg_lite_remote_init((void *)startupData, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);

    /* Signal the other core we are ready by triggering the event and passing the APP_RPMSG_READY_EVENT_DATA */
    MCMGR_TriggerEvent(kMCMGR_RemoteApplicationEvent, APP_RPMSG_READY_EVENT_DATA);
#else
    PRINTF("RPMSG Share Base Addr is 0x%x\r\n", RPMSG_LITE_SHMEM_BASE);
    my_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
#endif /* MCMGR_USED */
    while (!rpmsg_lite_is_link_up(my_rpmsg))
        ;
    PRINTF("Link is up!\r\n");

    my_queue  = rpmsg_queue_create(my_rpmsg);
    my_ept    = rpmsg_lite_create_ept(my_rpmsg, LOCAL_EPT_ADDR, rpmsg_queue_rx_cb, my_queue);
    ns_handle = rpmsg_ns_bind(my_rpmsg, app_nameservice_isr_cb, NULL);
    platform_time_delay(1000);
    rpmsg_ns_announce(my_rpmsg, my_ept, RPMSG_LITE_NS_ANNOUNCE_STRING, RL_NS_CREATE);
    PRINTF("Nameservice announce sent.\r\n");

#ifdef RPMSG_LITE_MASTER_IS_LINUX
    /* Wait Hello handshake message from Remote Core. */
    rpmsg_queue_recv(my_rpmsg, my_queue, (unsigned long *)&remote_addr, helloMsg, sizeof(helloMsg), NULL, RL_BLOCK);
#endif /* RPMSG_LITE_MASTER_IS_LINUX */

    while (msg.DATA <= 100)
    {
        PRINTF("Waiting for ping...\r\n");
        rpmsg_queue_recv(my_rpmsg, my_queue, (unsigned long *)&remote_addr, (char *)&msg, sizeof(THE_MESSAGE), NULL,
                         RL_BLOCK);
        msg.DATA++;
        PRINTF("Sending pong...\r\n");
        rpmsg_lite_send(my_rpmsg, my_ept, remote_addr, (char *)&msg, sizeof(THE_MESSAGE), RL_BLOCK);
    }

    PRINTF("Ping pong done, deinitializing...\r\n");

    rpmsg_lite_destroy_ept(my_rpmsg, my_ept);
    my_ept = NULL;
    rpmsg_queue_destroy(my_rpmsg, my_queue);
    my_queue = NULL;
    rpmsg_ns_unbind(my_rpmsg, ns_handle);
    rpmsg_lite_deinit(my_rpmsg);
    msg.DATA = 0;

    PRINTF("Looping forever...\r\n");

    /* End of the example */
    while (1)
        ;
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
    if (sc_pm_set_resource_power_mode(ipc, SC_R_MU_6B, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on MU\r\n");
    }
    if (sc_pm_set_resource_power_mode(ipc, SC_R_MU_7B, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on MU\r\n");
    }

    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_1, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTEER!\r\n");
    }

    /* Power the I2C module */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_M4_1_I2C, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable lpi2c");
    }
    /* Set LPI2C clock */
    if (CLOCK_SetIpFreq(kCLOCK_M4_1_Lpi2c, SC_133MHZ) == 0)
    {
        PRINTF("Error: Failed to set LPI2C frequency\r\n");
    }

    IRQSTEER_Init(IRQSTEER);
    APP_SRTM_Init();
    APP_SRTM_StartCommunication();

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
