/*
 * Copyright (c) 2015, 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "rpmsg_lite.h"
#include "erpc_server_setup.h"
#include "erpc_matrix_multiply_server.h"
#include "erpc_matrix_multiply.h"
#include "erpc_error_handler.h"
#include "mcmgr.h"

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ERPC_TRANSPORT_RPMSG_LITE_LINK_ID (RL_PLATFORM_K32L3A60_M4_M0_LINK_ID)
#define APP_ERPC_READY_EVENT_DATA (1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

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

/*!
 * @brief Main function
 */
int main()
{
    uint32_t startupData;
    mcmgr_status_t mcmgrStatus;

    BOARD_InitPins_Core1();

    /* Initialize MCMGR before calling its API */
    MCMGR_Init();

    /* Get the startup data */
    do
    {
        mcmgrStatus = MCMGR_GetStartupData(&startupData);
    } while (mcmgrStatus != kStatus_MCMGR_Success);

    /* RPMsg-Lite transport layer initialization */
    erpc_transport_t transport;

    transport = erpc_transport_rpmsg_lite_remote_init(101, 100, (void *)startupData, ERPC_TRANSPORT_RPMSG_LITE_LINK_ID,
                                                      SignalReady, NULL);

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory;
    message_buffer_factory = erpc_mbf_rpmsg_init(transport);

    /* eRPC server side initialization */
    erpc_server_init(transport, message_buffer_factory);

    /* adding the service to the server */
    erpc_add_service_to_server(create_MatrixMultiplyService_service());

    while (1)
    {
        /* process message */
        erpc_status_t status = erpc_server_poll();

        /* handle error status */
        if (status != kErpcStatus_Success)
        {
            /* print error description */
            erpc_error_handler(status, 0);

            /* stop erpc server */
            erpc_server_stop();

            /* exit program loop */
            break;
        }

        /* do other tasks */
        int i;
        for (i = 0; i < 10000; i++)
        {
        }
    }
    while (1)
    {
    }
}
