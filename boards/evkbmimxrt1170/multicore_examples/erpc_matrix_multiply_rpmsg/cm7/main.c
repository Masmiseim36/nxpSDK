/*
 * Copyright (c) 2015, 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rpmsg_lite.h"
#include "board.h"
#include "erpc_client_setup.h"
#include "erpc_error_handler.h"
#include "c_erpc_matrix_multiply_client.h"
#include "erpc_matrix_multiply_common.h"
#include "fsl_debug_console.h"
#include "app.h"
#include <stdlib.h>
#include "mcmgr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MATRIX_ITEM_MAX_VALUE     (50)
#define APP_ERPC_READY_EVENT_DATA (1U)

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern bool g_erpc_error_occurred;
static volatile uint16_t eRPCReadyEventData = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Fill matrices by random values
 */
static void fill_matrices(Matrix matrix1_ptr, Matrix matrix2_ptr)
{
    int32_t a, b;

    /* Fill both matrices by random values */
    for (a = 0; a < matrix_size; ++a)
    {
        for (b = 0; b < matrix_size; ++b)
        {
            matrix1_ptr[a][b] = rand() % MATRIX_ITEM_MAX_VALUE;
            matrix2_ptr[a][b] = rand() % MATRIX_ITEM_MAX_VALUE;
        }
    }
}

/*!
 * @brief Printing a matrix to the console
 */
static void print_matrix(Matrix matrix_ptr)
{
    int32_t a, b;

    for (a = 0; a < matrix_size; ++a)
    {
        for (b = 0; b < matrix_size; ++b)
        {
            (void)PRINTF("%4i ", matrix_ptr[a][b]);
        }
        (void)PRINTF("\r\n");
    }
}

/*!
 * @brief eRPC server side ready event handler
 */
static void eRPCReadyEventHandler(uint16_t eventData, void *context)
{
    eRPCReadyEventData = eventData;
}

/*!
 * @brief Main function
 */
int main(void)
{
    Matrix matrix1 = {0}, matrix2 = {0}, result_matrix = {0};

    BOARD_InitHardware();

    /* Configure BUTTON */
    BUTTON_INIT();

    (void)PRINTF("\r\nPrimary core started\r\n");

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
    erpc_client_t client;

    transport = erpc_transport_rpmsg_lite_master_init(100, 101, ERPC_TRANSPORT_RPMSG_LITE_LINK_ID);

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory;
    message_buffer_factory = erpc_mbf_rpmsg_init(transport);

    /* eRPC client side initialization */
    client = erpc_client_init(transport, message_buffer_factory);
    initMatrixMultiplyService_client(client);

    /* Set default error handler */
    erpc_client_set_error_handler(client, erpc_error_handler);

    /* Fill both matrices by random values */
    fill_matrices(matrix1, matrix2);

    /* Print both matrices on the console */
    (void)PRINTF("\r\nMatrix #1");
    (void)PRINTF("\r\n=========\r\n");
    print_matrix(matrix1);

    (void)PRINTF("\r\nMatrix #2");
    (void)PRINTF("\r\n=========\r\n");
    print_matrix(matrix2);

    for (;;)
    {
        (void)PRINTF("\r\neRPC request is sent to the server\r\n");

        erpcMatrixMultiply(matrix1, matrix2, result_matrix);

        /* Check if some error occurred in eRPC */
        if (g_erpc_error_occurred)
        {
            /* Exit program loop */
            break;
        }

        (void)PRINTF("\r\nResult matrix");
        (void)PRINTF("\r\n=============\r\n");
        (void)print_matrix(result_matrix);

        (void)PRINTF("\r\nPress the %s button to initiate the next matrix multiplication\r\n", BUTTON_NAME);
        /* Check for SWx button push. Pin is grounded when button is pushed. */
        while (!IS_BUTTON_PRESSED())
        {
        }

        /* Wait for 200ms to eliminate the button glitch */
        env_sleep_msec(200);

        /* Fill both matrices by random values */
        fill_matrices(matrix1, matrix2);

        /* Print both matrices on the console */
        (void)PRINTF("\r\nMatrix #1");
        (void)PRINTF("\r\n=========\r\n");
        print_matrix(matrix1);

        (void)PRINTF("\r\nMatrix #2");
        (void)PRINTF("\r\n=========\r\n");
        print_matrix(matrix2);
    }
    for (;;)
    {
    }
}
