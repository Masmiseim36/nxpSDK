/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "vglite_support.h"
/*-----------------------------------------------------------*/
#include "vg_lite.h"

#include "pin_mux.h"
#include "board.h"
#include "fsl_soc_src.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_BUFFER_COUNT 2
#define DEFAULT_SIZE     256.0f;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void sbengine_main_task(void *pvParameters);
void sbengine_input_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if (CUSTOM_VGLITE_MEMORY_CONFIG != 1)
#error "Application must be compiled with CUSTOM_VGLITE_MEMORY_CONFIG=1"
#else
#define VGLITE_COMMAND_BUFFER_SZ (128 * 1024)
/* On RT595S */
#if defined(CPU_MIMXRT595SFFOA_cm33)
	#define VGLITE_HEAP_SZ 3955776 /* 3.8 MB */
/* On RT1170 */
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
	#define VGLITE_HEAP_SZ 16777216 /* 16 MB */
#else
	#error "Unsupported CPU !"
#endif

/* Allocate the heap and set the command buffer(s) size */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t vglite_heap[VGLITE_HEAP_SZ], 64);

void *vglite_heap_base        = &vglite_heap;
uint32_t vglite_heap_size     = VGLITE_HEAP_SZ;
uint32_t vglite_cmd_buff_size = VGLITE_COMMAND_BUFFER_SZ;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_ResetDisplayMix(void)
{
    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);
    while (kSRC_SliceResetInProcess == SRC_GetSliceResetState(SRC, kSRC_DisplaySlice))
    {
    }
}


int main(void)
{
    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();
    BOARD_ResetDisplayMix();
    BOARD_InitLpuartPins();
    BOARD_InitMipiPanelPins();
    BOARD_MIPIPanelTouch_I2C_Init();
    BOARD_InitDebugConsole();

    if (xTaskCreate(sbengine_main_task, "sbengine_task", 8192, NULL, configMAX_PRIORITIES / 2, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    if (xTaskCreate(sbengine_input_task, "sbengine-input", 1024, NULL, configMAX_PRIORITIES / 2, NULL) != pdPASS)
    {
 	   PRINTF("Task creation failed!.\r\n");
 	   while (1);
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
