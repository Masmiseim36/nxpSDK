/*
 * Copyright 2022 NXP
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
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)__section_begin("__core1_image");
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif

void GPIO_ClearStopRequest(void)
{
    CCM->GPR_SHARED8 &= ~CCM_GPR_SHARED8_m33_gpio1_ipg_stop_MASK;
    CCM->GPR_SHARED12 &= ~CCM_GPR_SHARED12_m7_gpio1_ipg_stop_MASK;
}

void GPIO_SetStopRequest(void)
{
    CCM->GPR_SHARED8 |= CCM_GPR_SHARED8_m33_gpio1_ipg_stop_MASK;
    CCM->GPR_SHARED12 |= CCM_GPR_SHARED12_m7_gpio1_ipg_stop_MASK;
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Workaround: Disable interrupt which might be enabled by ROM. */
    RGPIO_SetPinInterruptConfig(RGPIO1, 9U, kRGPIO_InterruptOutput0, kRGPIO_InterruptOrDMADisabled);
    NVIC_ClearPendingIRQ(GPIO1_0_IRQn);
}
/*${function:end}*/
