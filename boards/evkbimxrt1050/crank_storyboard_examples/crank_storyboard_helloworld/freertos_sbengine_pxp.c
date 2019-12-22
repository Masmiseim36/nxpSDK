/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* BSP includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "peripherals.h"
#include "touch.h"

#include "pin_mux.h"
#include "clock_config.h"

/* Storyboard includes. */
#include <gre/gre.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Task priorities. */

#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void hello_task(void *pvParameters);

void sbengine_main_task(void *argument);
void sbengine_input_task(void *argument);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_PrintBanner(void)
{
    PRINTF("***********************************************\r\n");
    PRINTF("* Crank Software Storyboard Demo \r\n");
    PRINTF("* Storyboard Runtime version :%d.%d.%d build %d \r\n",GR_MAJOR_VERSION,GR_MINOR_VERSION,GR_SERVICE_VERSION,GR_BUILD_VERSION);
    PRINTF("* Target platform : %s\r\n",BOARD_NAME);
    PRINTF("* Target OS : FreeRTOS %s\r\n",tskKERNEL_VERSION_NUMBER);
#if defined(GRE_FEATURE_VFS_RESOURCES)
    PRINTF("* Target Filesystem : SBVFS\r\n");
#else
    PRINTF("* Target Filesystem : SD Card\r\n");
#endif
    PRINTF("***********************************************\r\n");
}

/*!
 * @brief Application entry point.
 */
int main(void)
{
	/* Init board hardware. */
	BOARD_ConfigMPU();
	BOARD_InitPins();
	BOARD_InitSemcPins();
	BOARD_InitBootPeripherals();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	if (xTaskCreate(sbengine_main_task, "sbengine", 4096, NULL, configMAX_PRIORITIES / 2, NULL) != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1)
			;
	}

	if (xTaskCreate(sbengine_input_task, "StoryboardInput", 2048, NULL, configMAX_PRIORITIES / 2, NULL) != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1)
			;
	}

    if (xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

	vTaskStartScheduler();
	for (;;)
		;
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void hello_task(void *pvParameters)
{
    for (;;)
    {
    	BOARD_PrintBanner();
        vTaskSuspend(NULL);
    }
}

