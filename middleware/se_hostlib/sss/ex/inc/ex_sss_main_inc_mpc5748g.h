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
#define HAVE_KSDK_LED_APIS 1

#include "ax_reset.h"
// #include "board.h"
// #include "fsl_gpio.h"
#include "Cpu.h"
#include "pin_mux.h"
#include "dmaController1.h"
#include "clockMan1.h"

// #include "se_reset_config.h"
// #include "sm_timer.h"

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

void ex_sss_main_ksdk_bm()
{
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);

    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    // axReset_PowerUp();
    // sm_initSleep();
}

void ex_sss_main_ksdk_boot_rtos_task()
{
}

void ex_sss_main_ksdk_success()
{
}

void ex_sss_main_ksdk_failure()
{
}
