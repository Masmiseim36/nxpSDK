/*
 *
 * Copyright 2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* Common header file used by Freedom K64F */

#ifndef HAVE_KSDK
#error "HAVE_KSDK must be defined"
#endif

/* Exposed variables */
#define HAVE_KSDK_LED_APIS 0

#include "ax_reset.h"
#include "board.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "se_reset_config.h"
#include "sm_timer.h"
#include <board.h>

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if (SSS_HAVE_HOSTCRYPTO_MBEDTLS)
#include "ksdk_mbedtls.h"
#endif

#if SSS_HAVE_NXPNFCRDLIB
#include "fsl_pit.h"
#endif

#ifdef USE_SERGER_RTT
extern void nInit_segger_Log(void);
#endif

#define EX_SSS_BOOT_RTOS_STACK_SIZE 7000

void ex_sss_main_ksdk_bm()
{
    hardware_init();
#if (SSS_HAVE_HOSTCRYPTO_MBEDTLS)
    CRYPTO_InitHardware();
#if defined(FSL_FEATURE_SOC_SHA_COUNT) && (FSL_FEATURE_SOC_SHA_COUNT > 0)
    CLOCK_EnableClock(kCLOCK_Sha0);
    RESET_PeripheralReset(kHASH_RST_SHIFT_RSTn);
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
