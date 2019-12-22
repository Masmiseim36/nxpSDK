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

#if (SSS_HAVE_MBEDTLS)
#include "ksdk_mbedtls.h"
#endif

#ifdef CPU_MIMXRT1052DVL6B
#include "fsl_dcp.h"
#include "fsl_trng.h"
#endif
#include "fsl_iomuxc.h"

#ifdef CPU_MIMXRT1052DVL6B
#define TRNG0 TRNG
#endif

void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {
        .enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

void ex_sss_main_ksdk_bm()
{
#if defined(_WIN32) && defined(WIN32) && defined(DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
    dcp_config_t dcpConfig;
    trng_config_t trngConfig;

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

#if defined(IMX_RT)
    /* Data cache must be temporarily disabled to be able to use sdram */
    SCB_DisableDCache();
#endif

    /* Initialize DCP */
    DCP_GetDefaultConfig(&dcpConfig);
    DCP_Init(DCP, &dcpConfig);

    /* Initialize TRNG */
    TRNG_GetDefaultConfig(&trngConfig);
    /* Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.
    * It is optional.*/
    trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;

    /* Initialize TRNG */
    TRNG_Init(TRNG0, &trngConfig);

#if !defined(USE_RTOS) || USE_RTOS == 0
#if defined(MBEDTLS)
    CRYPTO_InitHardware();
#endif
    sm_initSleep();
#endif
}

void ex_sss_main_ksdk_boot_rtos_task()
{
#if defined(MBEDTLS)
    CRYPTO_InitHardware();
#endif /* defined(MBEDTLS) */
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);
    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    sm_sleep(2);
    GPIO_WritePinOutput(GPIO1, 9, 1);
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
