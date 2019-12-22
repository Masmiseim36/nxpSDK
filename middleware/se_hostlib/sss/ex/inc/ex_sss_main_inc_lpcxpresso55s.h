/*
 * Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/* Common header fils used by Freedom K64F */

#ifndef HAVE_KSDK
#error "HAVE_KSDK must be defined"
#endif

/* Expsosed variables */
#define HAVE_KSDK_LED_APIS 1

#include "ax_reset.h"
#include "board.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "se_reset_config.h"
#include "sm_timer.h"

#if defined(MBEDTLS)
#include "ksdk_mbedtls.h"
#endif

void ex_sss_main_ksdk_bm()
{
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach 12 MHz clock to FLEXCOMM8 (I2C master) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC4_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();

    LED_BLUE_INIT(1);
    LED_GREEN_INIT(1);
    LED_RED_INIT(1);

    LED_BLUE_ON();

    axReset_HostConfigure();
    axReset_PowerUp();

#if defined(MBEDTLS)
    CRYPTO_InitHardware();
#if defined(FSL_FEATURE_SOC_SHA_COUNT) && (FSL_FEATURE_SOC_SHA_COUNT > 0)
    CLOCK_EnableClock(kCLOCK_Sha0);
    RESET_PeripheralReset(kSHA_RST_SHIFT_RSTn);
#endif /* SHA */
#endif /* defined(MBEDTLS) */

    sm_initSleep();
}

void ex_sss_main_ksdk_boot_rtos_task()
{
}

void ex_sss_main_ksdk_success()
{
    LED_BLUE_OFF();
    LED_RED_OFF();
    LED_GREEN_ON();
}

void ex_sss_main_ksdk_failure()
{
    LED_BLUE_OFF();
    LED_RED_ON();
    LED_GREEN_OFF();
}
