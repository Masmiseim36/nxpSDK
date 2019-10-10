/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "erpc_client_setup.h"
#include "erpc_error_handler.h"
#include "erpc_matrix_multiply.h"
#include "fsl_debug_console.h"
#include <stdlib.h>
#include "mcmgr.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUTTON_INIT() GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, &button_config)
#define IS_BUTTON_PRESSED() !GPIO_PinRead(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN)

/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS (void *)0x01000000

#define MATRIX_ITEM_MAX_VALUE 50
#define APP_ERPC_READY_EVENT_DATA (1)

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern bool g_erpc_error_occurred;
volatile uint16_t eRPCReadyEventData = 0;

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
            PRINTF("%4i ", (int)(matrix_ptr[a][b]));
        }
        PRINTF("\r\n");
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

/*!
 * @brief Main function
 */
int main()
{
    Matrix matrix1 = {0}, matrix2 = {0}, result_matrix = {0};

    BOARD_InitPins_Core0();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Define the init structure for the input switch pin */
    gpio_pin_config_t button_config = {
        kGPIO_DigitalInput,
        0,
    };

    /* Configure BUTTON */
    BUTTON_INIT();

    PRINTF("\r\nPrimary core started\r\n");

#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* Calculate size of the image */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    PRINTF("Copy CORE1 image to address: 0x%x, size: %d\n", CORE1_BOOT_ADDRESS, core1_image_size);

    /* Copy application from FLASH to RAM */
    memcpy(CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);
#endif

    /* Initialize MCMGR before calling its API */
    MCMGR_Init();

    /* Register the application event before starting the secondary core */
    MCMGR_RegisterEvent(kMCMGR_RemoteApplicationEvent, eRPCReadyEventHandler, NULL);

    /* Boot source for Core 1 */
    MCMGR_StartCore(kMCMGR_Core1, CORE1_BOOT_ADDRESS, (uint32_t)0, kMCMGR_Start_Asynchronous);

    /* Wait until the secondary core application signals the erpc server has been initialized and is ready to
     * communicate. */
    while (APP_ERPC_READY_EVENT_DATA != eRPCReadyEventData)
    {
    };

    /* MU transport layer initialization */
    erpc_transport_t transport;
    transport = erpc_transport_mu_init(MU_BASE);

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory;
    message_buffer_factory = erpc_mbf_dynamic_init();

    /* eRPC client side initialization */
    erpc_client_init(transport, message_buffer_factory);

    /* Set default error handler */
    erpc_client_set_error_handler(erpc_error_handler);

    /* Fill both matrices by random values */
    fill_matrices(matrix1, matrix2);

    /* Print both matrices on the console */
    PRINTF("\r\nMatrix #1");
    PRINTF("\r\n=========\r\n");
    print_matrix(matrix1);

    PRINTF("\r\nMatrix #2");
    PRINTF("\r\n=========\r\n");
    print_matrix(matrix2);

    while (1)
    {
        PRINTF("\r\neRPC request is sent to the server\r\n");

        erpcMatrixMultiply(matrix1, matrix2, result_matrix);

        /* Check if some error occurred in eRPC */
        if (g_erpc_error_occurred)
        {
            /* Exit program loop */
            break;
        }

        PRINTF("\r\nResult matrix");
        PRINTF("\r\n=============\r\n");
        print_matrix(result_matrix);

        PRINTF("\r\nPress the SW2 button to initiate the next matrix multiplication\r\n");
        /* Check for SW2 button push. Pin is grounded when button is pushed. */
        while (1 != IS_BUTTON_PRESSED())
        {
        }

        /* Wait to eliminate the button glitch */
        int i;
        for (i = 0; i < 1000000; i++)
        {
        }

        /* Fill both matrices by random values */
        fill_matrices(matrix1, matrix2);

        /* Print both matrices on the console */
        PRINTF("\r\nMatrix #1");
        PRINTF("\r\n=========\r\n");
        print_matrix(matrix1);

        PRINTF("\r\nMatrix #2");
        PRINTF("\r\n=========\r\n");
        print_matrix(matrix2);
    }
    while (1)
    {
    }
}
