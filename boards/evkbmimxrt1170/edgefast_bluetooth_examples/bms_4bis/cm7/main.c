/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

#include "broadcast_media_sender.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_InitHardware(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static StackType_t xStack[ configMINIMAL_STACK_SIZE * 8 ];
static StaticTask_t xTaskBuffer;

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    BOARD_InitHardware();

    if (NULL == xTaskCreateStatic(broadcast_media_sender_task, "broadcast_media_sender_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, xStack, &xTaskBuffer))
    {
        PRINTF("broadcast_media_sender_task creation failed!\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
