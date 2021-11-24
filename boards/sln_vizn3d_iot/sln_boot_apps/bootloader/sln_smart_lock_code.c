/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <stdbool.h>
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"
#include "sln_app_specific.h"

#define BOARD_3D_CAMERA_PWR_GPIO       GPIO13
#define BOARD_3D_CAMERA_PWR_PIN        9U
#define BOARD_3D_CAMERA_RESET_GPIO     GPIO13
#define BOARD_3D_CAMERA_RESET_PIN      5U
#define BOARD_3D_CAMERA_TEST_MODE_GPIO GPIO13
#define BOARD_3D_CAMERA_TEST_MODE_PIN  11U
#define BOARD_3D_CAMERA_LDP_INT_GPIO   GPIO13
#define BOARD_3D_CAMERA_LDP_INT_PIN    12U

static void BOARD_Pull3DCameraResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_RESET_GPIO, BOARD_3D_CAMERA_RESET_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_RESET_GPIO, BOARD_3D_CAMERA_RESET_PIN, 0U);
    }
}

static void BOARD_Pull3DCameraPowerEnablePin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_PWR_GPIO, BOARD_3D_CAMERA_PWR_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_PWR_GPIO, BOARD_3D_CAMERA_PWR_PIN, 0U);
    }
}

static void BOARD_Pull3DCameraTestModePin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_TEST_MODE_GPIO, BOARD_3D_CAMERA_TEST_MODE_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_TEST_MODE_GPIO, BOARD_3D_CAMERA_TEST_MODE_PIN, 0U);
    }
}

static void BOARD_Pull3DCameraLDPIntPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_LDP_INT_GPIO, BOARD_3D_CAMERA_LDP_INT_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_3D_CAMERA_LDP_INT_GPIO, BOARD_3D_CAMERA_LDP_INT_PIN, 0U);
    }
}

#define PIT PIT1
static void Time_Init(unsigned int unit_us)
{
    pit_config_t pitConfig;
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(unit_us, CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)));
    PIT_SetTimerChainMode(PIT, kPIT_Chnl_1, 1);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, 0xFFFFFFFF);
    PIT_StartTimer(PIT, kPIT_Chnl_1);
    PIT_StartTimer(PIT, kPIT_Chnl_0);
}

static void Time_Reset(void)
{
    /* reset timer count with 0xFFFFFFFF */
    PIT_StopTimer(PIT, kPIT_Chnl_1);
    PIT_StartTimer(PIT, kPIT_Chnl_1);
}

static void OBU1S_DelayMs(uint32_t ms)
{
    uint32_t pit_count = PIT_GetCurrentTimerCount(PIT, kPIT_Chnl_1);
    while (pit_count - PIT_GetCurrentTimerCount(PIT, kPIT_Chnl_1) < (ms * 1000))
        ;
}
static void OrbbecU1S_Reset(void)
{
    Time_Init(1);

    BOARD_Pull3DCameraTestModePin(false);
    BOARD_Pull3DCameraLDPIntPin(false);

    /* Pwr Enable */
    BOARD_Pull3DCameraPowerEnablePin(true);
    OBU1S_DelayMs(1);
    BOARD_Pull3DCameraLDPIntPin(true);

    /* Hard reset */
    BOARD_Pull3DCameraResetPin(false);
    OBU1S_DelayMs(1);
    BOARD_Pull3DCameraResetPin(true);
    OBU1S_DelayMs(5);
    BOARD_Pull3DCameraResetPin(false);

    Time_Reset();
}

void APP_SpecificCode(void)
{
#if ORBBECU1S_RESET_IN_BOOTLOADER
    OrbbecU1S_Reset();
#endif
}
