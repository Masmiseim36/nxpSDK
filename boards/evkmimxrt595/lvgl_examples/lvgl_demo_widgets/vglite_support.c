/*
 * Copyright (c) 2019, NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "vglite_support.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "FreeRTOS.h"
#include "task.h"
#include "vg_lite.h"
#include "vg_lite_platform.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_CONTIGUOUS_SIZE         0x100000
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t registerMemBase = 0x40240000;
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
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t contiguous_mem[MAX_CONTIGUOUS_SIZE], 64);

void *vglite_heap_base        = &contiguous_mem;
uint32_t vglite_heap_size     = MAX_CONTIGUOUS_SIZE;
#endif /* CUSTOM_VGLITE_MEMORY_CONFIG */

/*******************************************************************************
 * Code
 ******************************************************************************/
void GPU_DriverIRQHandler(void)
{
    vg_lite_IRQHandler();
}

static status_t BOARD_InitVGliteClock(void)
{
    SYSCTL0->PDRUNCFG1_CLR = SYSCTL0_PDRUNCFG1_GPU_SRAM_APD_MASK;
    SYSCTL0->PDRUNCFG1_CLR = SYSCTL0_PDRUNCFG1_GPU_SRAM_PPD_MASK;
    POWER_ApplyPD();

    CLOCK_AttachClk(kMAIN_CLK_to_GPU_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivGpuClk, 2);
    CLOCK_EnableClock(kCLOCK_Gpu);
    CLOCK_EnableClock(kCLOCK_AxiSwitch);

    RESET_ClearPeripheralReset(kGPU_RST_SHIFT_RSTn);
    RESET_ClearPeripheralReset(kAXI_SWITCH_RST_SHIFT_RSTn);

    NVIC_SetPriority(GPU_IRQn, 3);
    EnableIRQ((IRQn_Type)GPU_IRQn);

    return kStatus_Success;
}

status_t BOARD_PrepareVGLiteController(void)
{
    status_t status;

    status = BOARD_InitVGliteClock();

    if (kStatus_Success != status)
    {
        return status;
    }

    vg_lite_init_mem(registerMemBase, gpu_mem_base, vglite_heap_base, vglite_heap_size);

    return kStatus_Success;
}
