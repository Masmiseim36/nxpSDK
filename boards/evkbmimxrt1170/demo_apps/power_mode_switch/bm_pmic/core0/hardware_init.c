/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_soc_src.h"
/*${header:end}*/

/*${function:start}*/
void APP_BootCore1(void)
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
}

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
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
    clock_root_config_t rootCfg = {0};

    BOARD_ConfigMPU();

    /* Workaround: Disable interrupt which might be enabled by ROM. */
    GPIO_DisableInterrupts(GPIO3, 1U << 24);
    GPIO_ClearPinsInterruptFlags(GPIO3, 1U << 24);

    /* Interrupt flags can't be cleared when wake up from SNVS mode, clear srtc and gpio interrupts here. */
    SNVS->LPSR |= SNVS_LPSR_LPTA_MASK;
    GPIO_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);

    BOARD_InitPins();

    rootCfg.mux = kCLOCK_LPUART1_ClockRoot_MuxOscRc16M;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Lpuart1, &rootCfg);

    BOARD_InitDebugConsole();

    // set start address after waking up from SUSPEND mode
    IOMUXC_LPSR_GPR->GPR26 &= ~IOMUXC_LPSR_GPR_GPR26_CM7_INIT_VTOR_MASK;
    IOMUXC_LPSR_GPR->GPR26 |= IOMUXC_LPSR_GPR_GPR26_CM7_INIT_VTOR(IMAGE_ENTRY_ADDRESS >> 7);
}
/*${function:end}*/
