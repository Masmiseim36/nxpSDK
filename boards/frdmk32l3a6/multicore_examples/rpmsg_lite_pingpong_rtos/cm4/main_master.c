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

#include "fsl_common.h"
#include "pin_mux.h"
#include "mcmgr.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RPMSG_LITE_LINK_ID (RL_PLATFORM_K32L3A60_M4_M0_LINK_ID)
#define MCMGR_USED

/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS (void *)0x01000000

#define APP_TASK_STACK_SIZE (256)
#define LOCAL_EPT_ADDR (40)
#define APP_RPMSG_READY_EVENT_DATA (1)

typedef struct the_message
{
    uint32_t DATA;
} THE_MESSAGE, *THE_MESSAGE_PTR;

THE_MESSAGE msg = {0};

#define SH_MEM_TOTAL_SIZE (6144)
#ifdef MCMGR_USED
#if defined(__ICCARM__) /* IAR Workbench */
#pragma location = "rpmsg_sh_mem_section"
char rpmsg_lite_base[SH_MEM_TOTAL_SIZE];
#elif defined(__GNUC__)                             /* LPCXpresso */
char rpmsg_lite_base[SH_MEM_TOTAL_SIZE] __attribute__((section(".noinit.$rpmsg_sh_mem")));
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION) /* Keil MDK */
char rpmsg_lite_base[SH_MEM_TOTAL_SIZE] __attribute__((section("rpmsg_sh_mem_section")));
#else
#error "RPMsg: Please provide your definition of rpmsg_lite_base[]!"
#endif
#endif /* MCMGR_USED */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

TaskHandle_t app_task_handle = NULL;

void app_nameservice_isr_cb(unsigned int new_ept, const char *new_ept_name, unsigned long flags, void *user_data)
{
    unsigned long *data = (unsigned long *)user_data;

    *data = new_ept;
}

#ifdef MCMGR_USED
static void RPMsgRemoteReadyEventHandler(uint16_t eventData, void *context)
{
    uint16_t *data = (uint16_t *)context;

    *data = eventData;
}

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
#endif

void app_task(void *param)
{
#ifdef MCMGR_USED
    volatile uint16_t RPMsgRemoteReadyEventData = 0;
#endif
    volatile unsigned long remote_addr = 0;
    struct rpmsg_lite_endpoint *my_ept;
    rpmsg_queue_handle my_queue;
    struct rpmsg_lite_instance *my_rpmsg;
    rpmsg_ns_handle ns_handle;
    int len;

#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* Calculate size of the image */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    PRINTF("Copy CORE1 image to address: 0x%x, size: %d\r\n", CORE1_BOOT_ADDRESS, core1_image_size);

    /* Copy application from FLASH to RAM */
    memcpy(CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);
#endif

#ifdef MCMGR_USED
    /* Initialize MCMGR before calling its API */
    MCMGR_Init();

    /* Register the application event before starting the secondary core */
    MCMGR_RegisterEvent(kMCMGR_RemoteApplicationEvent, RPMsgRemoteReadyEventHandler,
                        (void *)&RPMsgRemoteReadyEventData);

    /* Boot Secondary core application */
    MCMGR_StartCore(kMCMGR_Core1, CORE1_BOOT_ADDRESS, (uint32_t)rpmsg_lite_base, kMCMGR_Start_Synchronous);

    /* Wait until the secondary core application signals the rpmsg remote has been initialized and is ready to
     * communicate. */
    while (APP_RPMSG_READY_EVENT_DATA != RPMsgRemoteReadyEventData)
    {
    };

    my_rpmsg = rpmsg_lite_master_init(rpmsg_lite_base, SH_MEM_TOTAL_SIZE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
#else
    my_rpmsg =
        rpmsg_lite_master_init((void *)RPMSG_LITE_SHMEM_BASE, SH_MEM_TOTAL_SIZE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
#endif
    my_queue  = rpmsg_queue_create(my_rpmsg);
    my_ept    = rpmsg_lite_create_ept(my_rpmsg, LOCAL_EPT_ADDR, rpmsg_queue_rx_cb, my_queue);
    ns_handle = rpmsg_ns_bind(my_rpmsg, app_nameservice_isr_cb, (void *)&remote_addr);

    /* Wait until the secondary core application issues the nameservice isr and the remote endpoint address is known. */
    while (0 == remote_addr)
    {
    };

    /* Send the first message to the remoteproc */
    msg.DATA = 0;
    rpmsg_lite_send(my_rpmsg, my_ept, remote_addr, (char *)&msg, sizeof(THE_MESSAGE), RL_DONT_BLOCK);

    while (msg.DATA <= 100)
    {
        rpmsg_queue_recv(my_rpmsg, my_queue, (unsigned long *)&remote_addr, (char *)&msg, sizeof(THE_MESSAGE), &len,
                         RL_BLOCK);
        PRINTF("Primary core received a msg\r\n");
        PRINTF("Message: Size=%x, DATA = %i\r\n", len, msg.DATA);
        msg.DATA++;

        rpmsg_lite_send(my_rpmsg, my_ept, remote_addr, (char *)&msg, sizeof(THE_MESSAGE), RL_BLOCK);
    }

    rpmsg_lite_destroy_ept(my_rpmsg, my_ept);
    my_ept = NULL;
    rpmsg_queue_destroy(my_rpmsg, my_queue);
    my_queue = NULL;
    rpmsg_ns_unbind(my_rpmsg, ns_handle);
    rpmsg_lite_deinit(my_rpmsg);

    // Print the ending banner
    PRINTF("\r\nRPMsg demo ends\r\n");
    while (1)
        ;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize standard SDK demo application pins */
    BOARD_InitPins_Core0();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Print the initial banner */
    PRINTF("\r\nRPMsg demo starts\r\n");

    if (xTaskCreate(app_task, "APP_TASK", APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &app_task_handle) != pdPASS)
    {
        PRINTF("\r\nFailed to create application task\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();

    PRINTF("Failed to start FreeRTOS on core0.\r\n");
    while (1)
        ;
}
