/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "vglite_support.h"
#include "fsl_clock.h"
#include "FreeRTOS.h"
#include "task.h"
#include "vg_lite.h"
#include "vg_lite_platform.h"
#include "display_support.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t registerMemBase = 0x41800000;
static uint32_t gpu_mem_base    = 0x0;

/*
 * In case custom VGLite memory parameters are used, the application needs to
 * allocate and publish the VGLite heap base, its size and the size of the
 * command buffer(s) using the following global variables:
 */
extern void *vglite_heap_base;
extern uint32_t vglite_heap_size;

#if (CUSTOM_VGLITE_MEMORY_CONFIG == 0)
/* VGLite driver heap */
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t contiguous_mem[MAX_CONTIGUOUS_SIZE], FRAME_BUFFER_ALIGN);

void *vglite_heap_base    = &contiguous_mem;
uint32_t vglite_heap_size = MAX_CONTIGUOUS_SIZE;
#endif /* CUSTOM_VGLITE_MEMORY_CONFIG */

/*******************************************************************************
 * Code
 ******************************************************************************/
void GPU2D_IRQHandler(void)
{
    vg_lite_IRQHandler();
}

static status_t BOARD_InitVGliteClock(void)
{
    const clock_root_config_t gc355ClockConfig = {
        .clockOff = false,
        .mux      = kCLOCK_GC355_ClockRoot_MuxVideoPllOut, /*!< 984MHz */
        .div      = 2,
    };

    CLOCK_SetRootClock(kCLOCK_Root_Gc355, &gc355ClockConfig);

    CLOCK_GetRootClockFreq(kCLOCK_Root_Gc355);

    CLOCK_EnableClock(kCLOCK_Gpu2d);

    NVIC_SetPriority(GPU2D_IRQn, 3);

    EnableIRQ(GPU2D_IRQn);

    return kStatus_Success;
}

status_t BOARD_PrepareVGLiteController(void)
{
    status_t status;
    vg_module_parameters_t param;
    uint32_t i;

    status = BOARD_InitVGliteClock();

    if (kStatus_Success != status)
    {
        return status;
    }

    /* The default value of VG_SYSTEM_RESERVE_COUNT is 1,
     * indicating that it supports one contiguous memory,
     * if multiple need be supported, the parameter param
     * needs to be modified */
    param.register_mem_base = registerMemBase;
    for (i = 0; i < VG_SYSTEM_RESERVE_COUNT; i++) {
        param.gpu_mem_base[i] = gpu_mem_base;
        param.contiguous_mem_base[i] = vglite_heap_base;
        param.contiguous_mem_size[i] = vglite_heap_size;
    }
    vg_lite_init_mem(&param);

    return kStatus_Success;
}
