/*
 *
 * Copyright 2024 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* Common header fils used by mcxn947 */

#ifndef HAVE_KSDK
#error "HAVE_KSDK must be defined"
#endif

/* Expsosed variables */
#define HAVE_KSDK_LED_APIS 0
#include "ax_reset.h"
#include "board.h"
#include "fsl_gpio.h"
#include "ledHandler.h"
#include "pin_mux.h"
#include "se_reset_config.h"
#include "sm_timer.h"

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if (SSS_HAVE_HOSTCRYPTO_MBEDTLS)
#include "els_pkc_mbedtls.h"
#endif

void BOARD_InitModuleClock(void) {
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FROM 12M to FLEXCOMM2 */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);
}

void ex_sss_main_ksdk_bm() {
    /*#if defined(_WIN32) && defined(WIN32) && defined(DEBUG)
      _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
      _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  #endif*/
    BOARD_InitModuleClock();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    sm_initSleep();
}

void ex_sss_main_ksdk_boot_rtos_task() {}

void ex_sss_main_ksdk_success() {
    LED_BLUE_OFF();
    LED_RED_OFF();
    LED_GREEN_ON();
}

void ex_sss_main_ksdk_failure() {
    LED_BLUE_OFF();
    LED_RED_ON();
    LED_GREEN_OFF();
}
