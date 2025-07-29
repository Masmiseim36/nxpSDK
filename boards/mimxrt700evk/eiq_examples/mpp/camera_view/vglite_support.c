/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "vglite_support.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "FreeRTOS.h"
#include "vg_lite_platform.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t registerMemBase = 0x40486000;
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

void *vglite_heap_base    = contiguous_mem;
uint32_t vglite_heap_size = MAX_CONTIGUOUS_SIZE;
#endif /* CUSTOM_VGLITE_MEMORY_CONFIG */

/*******************************************************************************
 * Code
 ******************************************************************************/
void VGPU_DriverIRQHandler(void)
{
    vg_lite_IRQHandler();
}

static status_t BOARD_InitVGliteClock(void)
{
    POWER_DisablePD(kPDRUNCFG_APD_GPU);
    POWER_DisablePD(kPDRUNCFG_PPD_GPU);
    POWER_ApplyPD();

    /* VGPU clock enablement and divider selection */
    CLOCK_EnableClock(kCLOCK_MediaAccessRamArbiter0);
    CLOCK_EnableClock(kCLOCK_MediaAccessRamArbiter1);
    CLOCK_EnableClock(kCLOCK_Gpu);

    CLOCK_AttachClk(kFRO0_DIV1_to_VGPU);
    CLOCK_SetClkDiv(kCLOCK_DivVgpuClk, 1U);

    /* Enable GPU interrupt */
    RESET_ClearPeripheralReset(kVGPU_RST_SHIFT_RSTn);
    NVIC_SetPriority(VGPU_IRQn, 3);
    EnableIRQ((IRQn_Type)VGPU_IRQn);

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
