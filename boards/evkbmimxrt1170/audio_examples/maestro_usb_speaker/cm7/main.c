/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Board includes */
#include "board.h"
#include "app.h"
#include "main.h"
#include "cmd.h"

#include "audio_speaker.h"

#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_TASK_STACK_SIZE (8 * 1024)

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

void handleShellMessage(void *arg)
{
    /* Wait for response message to be processed before returning to shell. */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

void APP_Shell_Task(void *param)
{
    PRINTF("[APP_Shell_Task] start\r\n");

    /* Handle shell commands.  Return when 'exit' command entered. */
    shellCmd(handleShellMessage, param);
    vTaskSuspend(NULL);
    while (1)
        ;
}

int main(void)
{
    int ret;

    BOARD_InitHardware();

    PRINTF("\r\n");
    PRINTF("**********************************************\r\n");
    PRINTF("Maestro audio USB speaker solutions demo start\r\n");
    PRINTF("**********************************************\r\n");
    PRINTF("\r\n");

    /* Initialize OSA*/
    OSA_Init();

    ret = BOARD_CODEC_Init();
    if (ret)
    {
        PRINTF("CODEC_Init failed\r\n");
        return -1;
    }

    /* USB speaker initialization */
    USB_DeviceApplicationInit();

    /* Set shell command task priority = 4 */
    if (xTaskCreate(APP_Shell_Task, "Shell Task", APP_TASK_STACK_SIZE, &app, configMAX_PRIORITIES - 3,
                    &app.shell_task_handle) != pdPASS)
    {
        PRINTF("\r\nFailed to create application task\r\n");
        while (1)
            ;
    }

    /* Run RTOS */
    vTaskStartScheduler();

    /* Should not reach this statement */
    return 0;
}
