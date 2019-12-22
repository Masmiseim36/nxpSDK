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

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if (SSS_HAVE_MBEDTLS)
#include "ksdk_mbedtls.h"
#endif

#if SSS_HAVE_NXPNFCRDLIB
#include "fsl_pit.h"
#endif

#ifdef USE_SERGER_RTT
extern void nInit_segger_Log(void);
#endif

void ex_sss_main_ksdk_bm()
{
#if SSS_HAVE_NXPNFCRDLIB
    pit_config_t pitConfig; /* Structure of initialize PIT */
#endif

    BOARD_BootClockRUN();
    BOARD_InitPins();
    BOARD_InitDebugConsole();

    LED_BLUE_INIT(1);
    LED_GREEN_INIT(1);
    LED_RED_INIT(1);

    LED_BLUE_ON();

    /* For DHCP Ethernet */
    SYSMPU_Type *base = SYSMPU;
    base->CESR &= ~SYSMPU_CESR_VLD_MASK;

    axReset_HostConfigure();
    axReset_PowerUp();

#if (SSS_HAVE_MBEDTLS)
    CRYPTO_InitHardware();
#if defined(FSL_FEATURE_SOC_SHA_COUNT) && (FSL_FEATURE_SOC_SHA_COUNT > 0)
    CLOCK_EnableClock(kCLOCK_Sha0);
    RESET_PeripheralReset(kSHA_RST_SHIFT_RSTn);
#endif /* SHA */
#endif /* defined(MBEDTLS) */
#ifdef USE_SERGER_RTT
    nInit_segger_Log();
#endif

    sm_initSleep();

#if SSS_HAVE_NXPNFCRDLIB
    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);
#endif
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
