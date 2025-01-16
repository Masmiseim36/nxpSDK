/*
 * Copyright 2018-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rpmsg_lite.h"
#include "board.h"
#include "erpc_arbitrated_client_setup.h"
#include "erpc_server_setup.h"
#include "erpc_error_handler.h"
#include "erpc_two_way_rpc_Core0Interface_common.h"
#include "c_erpc_two_way_rpc_Core1Interface_server.h"
#include "c_erpc_two_way_rpc_Core0Interface_client.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app.h"
#include "mcmgr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MATRIX_ITEM_MAX_VALUE     (50)
#define APP_TASK_STACK_SIZE       (256)
#define APP_ERPC_READY_EVENT_DATA (1U)

/*******************************************************************************
 * Variables
 ******************************************************************************/

static TaskHandle_t s_client_task_handle          = NULL;
static TaskHandle_t s_server_task_handle          = NULL;
static erpc_transport_t s_transportArbitrator     = NULL;
static erpc_transport_t s_transport               = NULL;
static getNumberCallback_t s_getNumberCallbackPtr = NULL;
static erpc_server_t s_server                     = NULL;
static erpc_client_t s_client                     = NULL;
extern bool g_erpc_error_occurred;
static volatile uint32_t s_number           = 0U;
static volatile uint16_t eRPCReadyEventData = 0U;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief eRPC server side ready event handler
 */
static void eRPCReadyEventHandler(uint16_t eventData, void *context)
{
    eRPCReadyEventData = eventData;
}

/* Implementation of RPC function increaseNumber. */
void increaseNumber(uint32_t *number)
{
    *number += 1U;
    s_number = *number;
}

static void client_task(void *param)
{
    uint32_t number;
    (void)PRINTF("\r\nPrimary core started\r\n\n");

#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* This section ensures the secondary core image is copied from flash location to the target RAM memory.
       It consists of several steps: image size calculation and image copying.
       These steps are not required on MCUXpresso IDE which copies the secondary core image to the target memory during
       startup automatically. */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    (void)PRINTF("Copy CORE1 image to address: 0x%x, size: %d\r\n", (void *)(char *)CORE1_BOOT_ADDRESS,
                 core1_image_size);

    /* Copy application from FLASH to RAM */
    (void)memcpy((void *)(char *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
    invalidate_cache_for_core1_image_memory(CORE1_BOOT_ADDRESS, core1_image_size);
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY*/

#endif /* CORE1_IMAGE_COPY_TO_RAM */

    /* Initialize MCMGR before calling its API */
    (void)MCMGR_Init();

    /* Register the application event before starting the secondary core */
    (void)MCMGR_RegisterEvent(kMCMGR_RemoteApplicationEvent, eRPCReadyEventHandler, ((void *)0));

    /* Boot source for Core 1 */
    (void)MCMGR_StartCore(kMCMGR_Core1, (void *)(char *)CORE1_BOOT_ADDRESS, (uint32_t)rpmsg_lite_base,
                          kMCMGR_Start_Synchronous);

    /* Wait until the secondary core application signals the rpmsg remote has been initialized and is ready to
     * communicate. */
    while (APP_ERPC_READY_EVENT_DATA != eRPCReadyEventData)
    {
    };

    /* RPMsg-Lite transport layer initialization */
    erpc_transport_t transport;

    transport = erpc_transport_rpmsg_lite_rtos_master_init(100U, 101U, ERPC_TRANSPORT_RPMSG_LITE_LINK_ID);

    s_transport = transport;

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory;
    message_buffer_factory = erpc_mbf_rpmsg_init(transport);

    /* eRPC client side initialization */
    s_client = erpc_arbitrated_client_init(transport, message_buffer_factory, &s_transportArbitrator);
    initCore0Interface_client(s_client);

    /* Set default error handler */
    erpc_arbitrated_client_set_error_handler(s_client, erpc_error_handler);

    /* Add server to client is necessary when do nesting RPC call. */
    while (s_server == NULL)
    {
        vTaskDelay(100);
    }
    erpc_arbitrated_client_set_server(s_client, s_server);
    erpc_arbitrated_client_set_server_thread_id(s_client, (void *)s_server_task_handle);

    s_getNumberCallbackPtr = &getNumberFromCore1;

    /* Simple synchronization system.
       To be sure that second core server is ready before rpc call. */
    vTaskDelay(500);

    /* RPC call to set callback function on the core1 side. */
    setGetNumberFunction(s_getNumberCallbackPtr);

    s_getNumberCallbackPtr = NULL;

    /* RPC call to get callback function from second side. */
    getGetNumberFunction(&s_getNumberCallbackPtr);
    erpc_assert(s_getNumberCallbackPtr == &getNumberFromCore1);

    while (g_erpc_error_occurred == false)
    {
        (void)PRINTF("Get number from core1:\r\n");
        s_getNumberCallbackPtr(&number);
        (void)PRINTF("Got number: %d\r\n", number);

        (void)PRINTF("Start normal rpc call example.\r\n");
        nestedCallGetNumber(s_getNumberCallbackPtr);
        (void)PRINTF("RPC call example finished.\r\n\n\n");

        vTaskDelay(700);
    }

    vTaskDelete(s_client_task_handle);
}

static void server_task(void *param)
{
    /* Wait for client initialization. */
    while (s_transportArbitrator == NULL)
    {
        vTaskDelay(100);
    }

    /* MessageBufferFactory initialization. */
    erpc_mbf_t message_buffer_factory;
    message_buffer_factory = erpc_mbf_rpmsg_init(s_transport);

    /* eRPC server initialization */
    s_server               = erpc_server_init(s_transportArbitrator, message_buffer_factory);
    erpc_service_t service = create_Core1Interface_service();
    erpc_add_service_to_server(s_server, service);

    erpc_status_t status = erpc_server_run(s_server);

    /* handle error status */
    if (status != (erpc_status_t)kErpcStatus_Success)
    {
        /* print error description */
        (void)PRINTF("Error occurred in server task. Task end with %d\r\n", status);
        erpc_error_handler(status, 0);

        /* eRPC server de-initialization */
        erpc_remove_service_from_server(s_server, service);
        destroy_Core1Interface_service(service);
        erpc_server_deinit(s_server);
    }

    vTaskDelete(s_server_task_handle);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    if (xTaskCreate(client_task, "APP_TASK", APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1U, &s_client_task_handle) !=
        pdPASS)
    {
        (void)PRINTF("\r\nFailed to create client task\r\n");
        return -1;
    }

    if (xTaskCreate(server_task, "APP_TASK", APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1U, &s_server_task_handle) !=
        pdPASS)
    {
        (void)PRINTF("\r\nFailed to create server task\r\n");
        return -1;
    }

    vTaskStartScheduler();

    return 0;
}
