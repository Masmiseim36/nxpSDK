/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "task.h"
#include "fsl_component_mem_manager.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#if (configSUPPORT_DYNAMIC_ALLOCATION == 0)
#error This file must not be used if configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

/*-----------------------------------------------------------*/
void *pvPortMalloc(size_t xWantedSize)
{
    void *         pvReturn = NULL;
    static uint8_t init     = 0U;

    vTaskSuspendAll();
    {
        if (init == 0U)
        {
            MEM_Init();
            init = 1U;
        }
        /* allocate here */
        pvReturn = MEM_BufferAlloc(xWantedSize);
    }
    (void)xTaskResumeAll();

#if (configUSE_MALLOC_FAILED_HOOK == 1)
    {
        if (pvReturn == NULL)
        {
            extern void vApplicationMallocFailedHook(void);
            vApplicationMallocFailedHook();
        }
        else
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
#endif /* if ( configUSE_MALLOC_FAILED_HOOK == 1 ) */

    // configASSERT( ( ( ( size_t ) pvReturn ) & ( size_t ) portBYTE_ALIGNMENT_MASK ) == 0 );
    return pvReturn;
}
/*-----------------------------------------------------------*/

void vPortFree(void *pv)
{
    if (pv != NULL)
    {
        vTaskSuspendAll();
        {
            /* free */
            MEM_BufferFree(pv);
        }
        (void)xTaskResumeAll();
    }
}
/*-----------------------------------------------------------*/

size_t xPortGetFreeHeapSize(void)
{
    return -1;
}
/*-----------------------------------------------------------*/

size_t xPortGetMinimumEverFreeHeapSize(void)
{
    return 0;
}
/*-----------------------------------------------------------*/

void vPortInitialiseBlocks(void)
{
    /* This just exists to keep the linker quiet. */
}

void vPortGetHeapStats(HeapStats_t *pxHeapStats)
{
    (void)pxHeapStats;
}
