/*
 * Copyright 2018-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_sema42.h"

#include "clock_config.h"
#include "pin_mux.h"

#include "board.h"
#include "dsp_support.h"
#include "dsp_ipc.h"
#include "cmd.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_TASK_STACK_SIZE (6 * 1024)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
int BOARD_CODEC_Init(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static app_handle_t app;
/*******************************************************************************
 * Code
 ******************************************************************************/

void CM33_PRINTF(const char* ptr, ...)
{
    va_list ap;
    SEMA42_Lock(APP_SEMA42, SEMA_PRINTF_NUM, SEMA_CORE_ID_CM33);
    va_start(ap, ptr);
    DbgConsole_Vprintf(ptr, ap);
    va_end(ap);
    SEMA42_Unlock(APP_SEMA42, SEMA_PRINTF_NUM);
}

void handleShellMessage(srtm_message *msg, void *arg)
{
    /* Send message to the DSP */
    dsp_ipc_send_sync(msg);

    /* Wait for response message to be processed before returning to shell. */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

void APP_Shell_Task(void *param)
{
    PRINTF("[CM33 Main][APP_Shell_Task] start\r\n");

    /* Handle shell commands.  Return when 'exit' command entered. */
    shellCmd(handleShellMessage, param);

    PRINTF("\r\n[CM33 Main][APP_Shell_Task] audio demo end\r\n");
    while (1)
        ;
}

void APP_DSP_IPC_Task(void *param)
{
    srtm_message msg = {0};
    app_handle_t *app = (app_handle_t *)param;

    PRINTF("[CM33 Main][APP_DSP_IPC_Task] start\r\n");

    while (1)
    {
        /* Block for IPC message from DSP */
        dsp_ipc_recv_sync(&msg);
        /* Process message */
        handleDSPMessage(app, &msg);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    int ret;

    BOARD_InitHardware();

    PRINTF("\r\n");
    PRINTF("******************************\r\n");
    PRINTF("DSP audio framework demo start\r\n");
    PRINTF("******************************\r\n");
    PRINTF("\r\n");

    ret = BOARD_CODEC_Init();
    if (ret)
    {
        PRINTF("[CM33 Main] CODEC_Init failed!\r\n");
        return -1;
    }

    /* Initialize RPMsg IPC interface between ARM and DSP cores. */
    BOARD_DSP_IPC_Init();

    /* Copy DSP image to RAM and start DSP core. */
    BOARD_DSP_Init();

    /* Wait for the DSP to lock the semaphore */
    while (SEMA_LOCKED_BY_DSP != SEMA42_GetGateStatus(APP_SEMA42, SEMA_STARTUP_NUM))
    {
    }

    /* Wait for the DSP to unlock the semaphore 1 */
    while (SEMA42_GetGateStatus(APP_SEMA42, SEMA_STARTUP_NUM))
    {
    }

#if DSP_IMAGE_COPY_TO_RAM
    PRINTF("[CM33 Main] DSP image copied to DSP TCM\r\n");
#endif

    /* Set IPC processing task priority = 2 */
    if (xTaskCreate(APP_DSP_IPC_Task, "DSP Msg Task", APP_TASK_STACK_SIZE, &app, tskIDLE_PRIORITY + 2,
                    &app.ipc_task_handle) != pdPASS)
    {
        PRINTF("\r\n[CM33 Main] Failed to create application task\r\n");
        while (1)
            ;
    }

    /* Set shell command task priority = 1 */
    if (xTaskCreate(APP_Shell_Task, "Shell Task", APP_TASK_STACK_SIZE, &app, tskIDLE_PRIORITY + 1,
                    &app.shell_task_handle) != pdPASS)
    {
        PRINTF("\r\n[CM33 Main] Failed to create application task\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();

    /* Shoud not reach this statement. */
    return 0;
}
