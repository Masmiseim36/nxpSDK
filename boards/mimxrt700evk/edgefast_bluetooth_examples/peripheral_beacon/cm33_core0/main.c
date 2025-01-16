/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

#if defined(IBEACON_APP) && (IBEACON_APP == 1)
#include "ibeacon.h"
#elif defined(EDDYSTONE) && (EDDYSTONE == 1)
#include "eddystone.h" 
#elif  defined(BEACON_APP) && (BEACON_APP == 1)
#include "beacon.h"
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

/*******************************************************************************
 * Code
 ******************************************************************************/

  int main(void)
{
    BOARD_InitHardware();

    if (xTaskCreate(beacon_task, "beacon_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("beacon task creation failed!\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
