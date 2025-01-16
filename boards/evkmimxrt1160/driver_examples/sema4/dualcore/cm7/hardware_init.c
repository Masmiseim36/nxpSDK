/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "fsl_soc_src.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
/*
 * For the dual core project, generally primary core starts first, initializes
 * the system, then starts the secondary core to run.
 * In the case that debugging dual-core at the same time (for example, using IAR+DAPLink),
 * the secondary core is started by debugger. Then the secondary core might
 * run when the primary core initialization not finished. The SRC->GPR is used
 * here to indicate whether secondary core could go. When started, the secondary core
 * should check and wait the flag in SRC->GPR, the primary core sets the
 * flag in SRC->GPR when its initialization work finished.
 */
#define BOARD_SECONDARY_CORE_GO_FLAG 0xa5a5a5a5u
#define BOARD_SECONDARY_CORE_SRC_GPR kSRC_GeneralPurposeRegister20

void BOARD_SetSecondaryCoreGoFlag(void)
{
    SRC_SetGeneralPurposeRegister(SRC, BOARD_SECONDARY_CORE_SRC_GPR, BOARD_SECONDARY_CORE_GO_FLAG);
}

void DEMO_BootCore1(void)
{
    IOMUXC_LPSR_GPR->GPR0 = IOMUXC_LPSR_GPR_GPR0_CM4_INIT_VTOR_LOW(CORE1_BOOT_ADDRESS >> 3);
    IOMUXC_LPSR_GPR->GPR1 = IOMUXC_LPSR_GPR_GPR1_CM4_INIT_VTOR_HIGH(CORE1_BOOT_ADDRESS >> 16);

    /* Read back to make sure write takes effect. */
    (void)IOMUXC_LPSR_GPR->GPR0;
    (void)IOMUXC_LPSR_GPR->GPR1;

    /* If CM4 is already running (released by debugger), then reset it.
       If CM4 is not running, release it. */
    if ((SRC->SCR & SRC_SCR_BT_RELEASE_M4_MASK) == 0)
    {
        SRC_ReleaseCoreReset(SRC, kSRC_CM4Core);
    }
    else
    {
        SRC_AssertSliceSoftwareReset(SRC, kSRC_M4CoreSlice);
    }

    BOARD_SetSecondaryCoreGoFlag();
}

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size()
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)&core1_image_start;
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
