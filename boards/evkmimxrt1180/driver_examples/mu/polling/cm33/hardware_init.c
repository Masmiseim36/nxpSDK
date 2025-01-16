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

/*${macro:start}*/
#define ELE_TRDC_AON_ID    0x74
#define ELE_TRDC_WAKEUP_ID 0x78
#define ELE_CORE_CM33_ID   0x1
#define ELE_CORE_CM7_ID    0x2

/*
 * Set ELE_STICK_FAILED_STS to 0 when ELE status check is not required,
 * which is useful when debug reset, where the core has already get the
 * TRDC ownership at first time and ELE is not able to release TRDC
 * ownership again for the following TRDC ownership request.
 */
#define ELE_STICK_FAILED_STS 1

#if ELE_STICK_FAILED_STS
#define ELE_IS_FAILED(x) (x != kStatus_Success)
#else
#define ELE_IS_FAILED(x) false
#endif
/*${macro:end}*/

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

#if !(defined(BOOT_CORE1_BY_MU) && BOOT_CORE1_BY_MU)
void APP_BootCore1(void)
{
    status_t sts;

    /* Enble CM7 */
    do
    {
        sts = ELE_BaseAPI_EnableAPC(MU_RT_S3MUA);
    } while (ELE_IS_FAILED(sts));

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
