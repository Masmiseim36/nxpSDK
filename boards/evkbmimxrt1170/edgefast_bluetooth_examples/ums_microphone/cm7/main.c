/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

#include "unicast_media_sender.h"

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

    if (NULL == xTaskCreateStatic(ums_microphone_task, "ums_microphone_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, xStack, &xTaskBuffer))
    {
        PRINTF("ums_microphone_task creation failed!\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
