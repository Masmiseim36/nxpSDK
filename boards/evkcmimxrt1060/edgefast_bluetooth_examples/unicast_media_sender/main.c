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
#if defined(CONFIG_BT_A2DP_SINK) && (CONFIG_BT_A2DP_SINK > 0)
#include "app_a2dp_sink.h"
#endif

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

#if defined(CONFIG_BT_A2DP_SINK) && (CONFIG_BT_A2DP_SINK > 0)
    if (NULL == xTaskCreateStatic(app_a2dp_sink_task, "app_a2dp_sink_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, xStack, &xTaskBuffer))
    {
        PRINTF("a2dp task creation failed!\r\n");
        while (1)
            ;
    }
#else
    if (NULL == xTaskCreateStatic(unicast_media_sender_task, "unicast_media_sender_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, xStack, &xTaskBuffer))
    {
        PRINTF("unicast_media_sender_task creation failed!\r\n");
        while (1)
            ;
    }
#endif

    vTaskStartScheduler();
    for (;;)
        ;
}
