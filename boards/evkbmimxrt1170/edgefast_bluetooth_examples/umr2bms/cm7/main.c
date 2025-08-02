/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

#include "umr2bms.h"

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

    if (NULL == xTaskCreateStatic(umr_to_bms_task, "umr2bms task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, xStack, &xTaskBuffer))
    {
        PRINTF("umr_to_bms_task creation failed!\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
