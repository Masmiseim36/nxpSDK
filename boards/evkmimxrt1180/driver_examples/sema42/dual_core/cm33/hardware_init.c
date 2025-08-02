/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "fsl_soc_src.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_ele_base_api.h"
/*${header:end}*/

/*${function:start}*/

static void BOARD_InitLedPin(void)
{
    const rgpio_pin_config_t config = {
        .pinDirection = kRGPIO_DigitalOutput,
        .outputLogic  = 1,
    };

    RGPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &config);
}

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    Prepare_CM7(CORE1_KICKOFF_ADDRESS);
}

void APP_InitDomain(void)
{
    /* Use static domain ID, don't need to do anything here. */
}

void APP_DeinitDomain(void)
{
    /* Use static domain ID, don't need to do anything here. */
}

#if !(defined(BOOT_CORE1_BY_MU) && BOOT_CORE1_BY_MU)
void APP_BootCore1(void)
{
    status_t sts;

    /* Enble CM7 */
    sts = ELE_BaseAPI_EnableAPC(MU_RT_S3MUA);
    if(sts != kStatus_Success)
    {
        while(1)
        {
        }
    }

    /* Deassert Wait */
    BLK_CTRL_S_AONMIX->M7_CFG =
        (BLK_CTRL_S_AONMIX->M7_CFG & (~BLK_CTRL_S_AONMIX_M7_CFG_WAIT_MASK)) | BLK_CTRL_S_AONMIX_M7_CFG_WAIT(0);
}
#endif

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

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitLedPin();
}
/*${function:end}*/
