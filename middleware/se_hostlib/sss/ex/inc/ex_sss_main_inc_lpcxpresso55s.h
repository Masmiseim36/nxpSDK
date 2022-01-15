/*
 *
 * Copyright 2019 NXP
 * SPDX-License-Identifier: Apache-2.0
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
#include "fsl_puf.h"
#include "fsl_power.h"

#if defined(MBEDTLS)
#include "ksdk_mbedtls.h"
#endif

void ex_sss_main_ksdk_bm()
{
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);

    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach 12 MHz clock to FLEXCOMM8 (I2C master) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);

    /* attach main clock divide to FLEXCOMM2 */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC4_RST_SHIFT_RSTn);

    BOARD_InitBootPins();
    // BOARD_BootClockPLL100M();
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

#if defined(SECURE_WORLD)
    puf_config_t conf;
    PUF_GetDefaultConfig(&conf);
    srand(0xbabadeda);

    if (kStatus_Success != PUF_Init(PUF, &conf)) {
        LOG_E("PUF_Init failed");
    }
#endif
}

void ex_sss_main_ksdk_boot_rtos_task()
{
}

void ex_sss_main_ksdk_success()
{
#if defined(SECURE_WORLD)
    puf_config_t conf;
    PUF_GetDefaultConfig(&conf);
    PUF_Deinit(PUF, &conf);
#endif

    LED_BLUE_OFF();
    LED_RED_OFF();
    LED_GREEN_ON();
}

void ex_sss_main_ksdk_failure()
{
#if defined(SECURE_WORLD)
    puf_config_t conf;
    PUF_GetDefaultConfig(&conf);
    PUF_Deinit(PUF, &conf);
#endif
    LED_BLUE_OFF();
    LED_RED_ON();
    LED_GREEN_OFF();
}
