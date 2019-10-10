/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "erpc_server_setup.h"
#include "erpc_matrix_multiply_server.h"
#include "erpc_matrix_multiply.h"
#include "erpc_error_handler.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mcmgr.h"

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define APP_TASK_STACK_SIZE 256
#define APP_ERPC_READY_EVENT_DATA (1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
TaskHandle_t app_task_handle = NULL;

/*!
 * @brief erpcMatrixMultiply function implementation.
 *
 * This is the implementation of the erpcMatrixMultiply function called by the primary core.
 *
 * @param matrix1 First matrix
 * @param matrix2 Second matrix
 * @param result_matrix Result matrix
 */
void erpcMatrixMultiply(Matrix matrix1, Matrix matrix2, Matrix result_matrix)
{
    int32_t i, j, k;

    /* Clear the result matrix */
    for (i = 0; i < matrix_size; ++i)
    {
        for (j = 0; j < matrix_size; ++j)
        {
            result_matrix[i][j] = 0;
        }
    }

    /* Multiply two matrices */
    for (i = 0; i < matrix_size; ++i)
    {
        for (j = 0; j < matrix_size; ++j)
        {
            for (k = 0; k < matrix_size; ++k)
            {
                result_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

static void SignalReady(void)
{
    /* Signal the other core we are ready by triggering the event and passing the APP_ERPC_READY_EVENT_DATA */
    MCMGR_TriggerEvent(kMCMGR_RemoteApplicationEvent, APP_ERPC_READY_EVENT_DATA);
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

void app_task(void *param)
{
    /* RPMsg-Lite transport layer initialization */
    erpc_transport_t transport;

    transport = erpc_transport_mu_init(MUB);

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory;
    message_buffer_factory = erpc_mbf_dynamic_init();

    /* eRPC server side initialization */
    erpc_server_init(transport, message_buffer_factory);

    /* adding the service to the server */
    erpc_add_service_to_server(create_MatrixMultiplyService_service());

    SignalReady();

    erpc_status_t status = erpc_server_run();

    /* handle error status */
    if (status != kErpcStatus_Success)
    {
        /* print error description */
        erpc_error_handler(status, 0);

        /* stop erpc server */
        erpc_server_stop();
    }

    while (1)
    {
    }
}

/*!
 * @brief Main function
 */
int main()
{
    BOARD_InitPins_Core1();

    /* Initialize MCMGR before calling its API */
    MCMGR_Init();

    if (xTaskCreate(app_task, "APP_TASK", APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &app_task_handle) != pdPASS)
    {
        /* Failed to create application task */
        return -1;
    }

    vTaskStartScheduler();

    return 0;
}
