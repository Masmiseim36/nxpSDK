/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "power_mode_switch_bm.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "lpm.h"

#include "fsl_gpio.h"

#include "pin_mux.h"
#include "fsl_gpt.h"
#include "fsl_lpuart.h"
#include "fsl_iomuxc.h"

#if (HAS_SEMC_MODULE)
#include "fsl_semc.h"
#endif

#include "fsl_gpio.h"
#include "clock_config.h"
#include "fsl_dcdc.h"
#if (HAS_SEMC_MODULE)
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_SDRAM0_BASE_ADDRESS 0x80000000U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined(__GNUC__)
void Board_CopyToRam();
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_wakeupTimeout;            /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */

static lpm_power_mode_t s_targetPowerMode;
static lpm_power_mode_t s_curRunMode = LPM_PowerModeOverRun;

static const char *s_modeNames[] = {"Over RUN",    "Full Run",       "Low Speed Run", "Low Power Run",
                                    "System Idle", "Low Power Idle", "Suspend",       "SNVS"};

int32_t is_suspend_reset = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_SetLPClockGate(void)
{
    CCM->CCGR0 = CCM_CCGR0_CG0(1) | CCM_CCGR0_CG1(1) | CCM_CCGR0_CG3(3) | CCM_CCGR0_CG11(1) | CCM_CCGR0_CG12(1);
    CCM->CCGR1 = CCM_CCGR1_CG9(3) | CCM_CCGR1_CG10(1) | CCM_CCGR1_CG13(1) | CCM_CCGR1_CG14(1) | CCM_CCGR1_CG15(1);
    CCM->CCGR2 = CCM_CCGR2_CG2(1) | CCM_CCGR2_CG8(1) | CCM_CCGR2_CG9(1) | CCM_CCGR2_CG10(1);
    CCM->CCGR3 = CCM_CCGR3_CG2(1) | CCM_CCGR3_CG4(1) | CCM_CCGR3_CG9(1) | CCM_CCGR3_CG14(1) | CCM_CCGR3_CG15(1);

    CCM->CCGR4 =
        CCM_CCGR4_CG1(1) | CCM_CCGR4_CG2(1) | CCM_CCGR4_CG4(1) | CCM_CCGR4_CG5(1) | CCM_CCGR4_CG6(1) | CCM_CCGR4_CG7(1);
    CCM->CCGR5 = CCM_CCGR5_CG0(1) | CCM_CCGR5_CG1(1) | CCM_CCGR5_CG4(1) | CCM_CCGR5_CG6(1) | CCM_CCGR5_CG12(1) |
                 CCM_CCGR5_CG14(1) | CCM_CCGR5_CG15(1);
    /* We can enable DCDC when need to config it and close it after configuration */
    CCM->CCGR6 = CCM_CCGR6_CG3(1) | CCM_CCGR6_CG4(1) | CCM_CCGR6_CG5(1) | CCM_CCGR6_CG9(1) | CCM_CCGR6_CG10(1) |
                 CCM_CCGR6_CG11(1);
}

void APP_WAKEUP_GPT_IRQn_HANDLER(void)
{
    GPT_ClearStatusFlags(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare1Flag);
    GPT_StopTimer(APP_WAKEUP_GPT_BASE);
    LPM_DisableWakeupSource(APP_WAKEUP_GPT_IRQn);
}

void APP_WAKEUP_BUTTON_IRQ_HANDLER(void)
{
    if ((1U << APP_WAKEUP_BUTTON_GPIO_PIN) & GPIO_GetPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO))
    {
        /* Disable interrupt. */
        GPIO_DisableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        GPIO_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        LPM_DisableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
    }
}

void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeSuspend || targetMode == LPM_PowerModeSNVS)
    {
        /* Wait for debug console output finished. */
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();

        /*
         * Set pin for current leakage.
         * Debug console RX pin: Set to pinmux to GPIO input.
         * Debug console TX pin: Don't need to change.
         */
        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_GPIO1_IO13, 0);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_GPIO1_IO13, IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                                 IOMUXC_SW_PAD_CTL_PAD_PUS(2) |
                                                                 IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
    }
}

void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeSuspend)
    {
        /*
         * Debug console RX pin is set to GPIO input, nee to re-configure pinmux.
         * Debug console TX pin: Don't need to change.
         */
        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_07_LPUART1_RX, 0);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_07_LPUART1_RX, IOMUXC_SW_PAD_CTL_PAD_SPEED(2));

        BOARD_InitDebugConsole();
    }
}

lpm_power_mode_t APP_GetLPMPowerMode(void)
{
    return s_targetPowerMode;
}

/*!
 * @brief Get input from user about wakeup timeout
 */
static uint8_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");
        PRINTF("The allowed range is 1s ~ 9s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        timeout = GETCHAR();
        PRINTF("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        PRINTF("Wrong value!\r\n");
    }
}

/* Get wakeup source by user input. */
static app_wakeup_source_t APP_GetWakeupSource(lpm_power_mode_t targetMode)
{
    uint8_t ch;

    while (1)
    {
        PRINTF("Select the wake up source:\r\n");
        PRINTF("Press T for GPT - GPT Timer\r\n");
#if (HAS_WAKEUP_PIN)
        PRINTF("Press S for switch/button %s. \r\n", APP_WAKEUP_BUTTON_NAME);
#endif

        PRINTF("\r\nWaiting for key press..\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        if (ch == 'T')
        {
            return kAPP_WakeupSourceGPT;
        }
#if (HAS_WAKEUP_PIN)
        else if (ch == 'S')
        {
            return kAPP_WakeupSourcePin;
        }
#endif
        else
        {
            PRINTF("Wrong value!\r\n");
        }
    }
}

/* Get wakeup timeout and wakeup source. */
static void APP_GetWakeupConfig(lpm_power_mode_t targetMode)
{
    /* Get wakeup source by user input. */
    s_wakeupSource = APP_GetWakeupSource(targetMode);

    if (kAPP_WakeupSourceGPT == s_wakeupSource)
    {
        /* Wakeup source is GPT, user should input wakeup timeout value. */
        s_wakeupTimeout = APP_GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
    }
    else
    {
        PRINTF("Switch %s from off to on to wake up.\r\n", APP_WAKEUP_BUTTON_NAME);
    }
}

static void APP_SetWakeupConfig(lpm_power_mode_t targetMode)
{
    /* Set GPT timeout value. */
    if (kAPP_WakeupSourceGPT == s_wakeupSource)
    {
        GPT_StopTimer(APP_WAKEUP_GPT_BASE);
        /* Update compare channel1 value will reset counter */
        GPT_SetOutputCompareValue(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare_Channel1,
                                  (CLOCK_GetRtcFreq() * s_wakeupTimeout) - 1U);

        /* Enable GPT Output Compare1 interrupt */
        GPT_EnableInterrupts(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare1InterruptEnable);
        NVIC_EnableIRQ(APP_WAKEUP_GPT_IRQn);
        EnableIRQ(APP_WAKEUP_GPT_IRQn);

        /* Restart timer */
        GPT_StartTimer(APP_WAKEUP_GPT_BASE);

        LPM_EnableWakeupSource(APP_WAKEUP_GPT_IRQn);
    }
    else
    {
        GPIO_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        /* Enable GPIO pin interrupt */
        GPIO_EnableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        NVIC_EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        /* Enable the Interrupt */
        EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        /* Enable GPC interrupt */
        LPM_EnableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
    }
    is_suspend_reset = 0;
}

static void APP_PrintRunFrequency(int32_t run_freq_only)
{
    PRINTF("\r\n");
    PRINTF("***********************************************************\r\n");
    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("IPG:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_IpgClk));
    PRINTF("OSC:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_OscClk));
    PRINTF("RTC:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_RtcClk));
    if (!run_freq_only)
    {
        PRINTF("USB1PLL:         %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
        PRINTF("USB1PLLPFD0:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
        PRINTF("USB1PLLPFD1:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
        PRINTF("USB1PLLPFD2:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
        PRINTF("USB1PLLPFD3:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));
        PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
        PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
        PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
        PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
        PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
        PRINTF("ENETPLL0:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPllClk));
        PRINTF("ENETPLL1:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll25MClk));
        PRINTF("ENETPLL2:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll500MClk));
        PRINTF("AUDIOPLL:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AudioPllClk));
    }
    PRINTF("***********************************************************\r\n");
    PRINTF("\r\n");
}

static void APP_ShowPowerMode(lpm_power_mode_t powerMode)
{
    if (powerMode <= LPM_PowerModeEnd)
    {
        PRINTF("    Power mode: %s\r\n", s_modeNames[powerMode]);
        APP_PrintRunFrequency(1);
    }
    else
    {
        assert(0);
    }
}

/*!
 * @brief main switch.
 */
static void APP_PowerModeSwitch(void)
{
    uint8_t ch;
    uint32_t freq;
    gpt_config_t gptConfig;
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t swConfig = {
        kGPIO_DigitalInput,
        0,
        kGPIO_IntRisingEdge,
    };

    /* Init GPT for wakeup */
    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.clockSource = kGPT_ClockSource_LowFreq; /* 32K RTC OSC */
    // gptConfig.enableMode = false;                     /* Keep counter when stop */
    gptConfig.enableMode      = true; /* Don't keep counter when stop */
    gptConfig.enableRunInDoze = true;
    /* Initialize GPT module */
    GPT_Init(APP_WAKEUP_GPT_BASE, &gptConfig);
    GPT_SetClockDivider(APP_WAKEUP_GPT_BASE, 1);

    /* Init input switch GPIO. */
    GPIO_PinInit(APP_WAKEUP_BUTTON_GPIO, APP_WAKEUP_BUTTON_GPIO_PIN, &swConfig);
    GPIO_EnableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);

    while (1)
    {
        freq = CLOCK_GetFreq(kCLOCK_CpuClk);

        PRINTF("\r\n########## Power Mode Switch Demo (build %s) ###########\n\r\n", __DATE__);
        PRINTF("    Core Clock = %dHz \r\n", freq);

        APP_ShowPowerMode(s_curRunMode);

        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c for enter: Over RUN       - System Over Run mode (500MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeOverRun);
        PRINTF("Press  %c for enter: Full RUN       - System Full Run mode (396MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeFullRun);
        PRINTF("Press  %c for enter: Low Speed RUN  - System Low Speed Run mode (132MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowSpeedRun);
        PRINTF("Press  %c for enter: Low Power RUN  - System Low Power Run mode (24MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowPowerRun);
        PRINTF("Press  %c for enter: System Idle    - System Wait mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeSysIdle);
        PRINTF("Press  %c for enter: Low Power Idle - Low Power Idle mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLPIdle);
        PRINTF("Press  %c for enter: Suspend        - Suspend mode\r\n", (uint8_t)'A' + (uint8_t)LPM_PowerModeSuspend);

#if (HAS_WAKEUP_PIN)
        PRINTF("Press  %c for enter: SNVS           - Shutdown the system\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeSNVS);
#endif

        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");

        if (s_targetPowerMode == LPM_PowerModeLowPowerRun)
        {
            LPM_RestoreLowPowerRunPLLs();
            PRINTF("\r\n ***** Enter low power run *****\r\n\r\n");
        }

        /* Wait for user response */
        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        s_targetPowerMode = (lpm_power_mode_t)(ch - 'A');

        if (s_targetPowerMode <= LPM_PowerModeSNVS)
        {
            if (!LPM_SetPowerMode(s_targetPowerMode))
            {
                PRINTF("Do not allow to enter mode %s\r\n", s_modeNames[s_targetPowerMode]);
            }
            else
            {
                if (s_targetPowerMode <= LPM_PowerModeRunEnd)
                {
                    switch (s_targetPowerMode)
                    {
                        case LPM_PowerModeOverRun:
                            LPM_SystemOverRun();
                            break;
                        case LPM_PowerModeFullRun:
                            LPM_SystemFullRun();
                            break;
                        case LPM_PowerModeLowSpeedRun:
                            LPM_SystemLowSpeedRun();
                            break;
                        case LPM_PowerModeLowPowerRun:
                            LPM_SystemLowPowerRun();
                            break;
                        default:
                            break;
                    }
                    s_curRunMode = s_targetPowerMode;
                    continue;
                }
#if (HAS_WAKEUP_PIN)
                else if (LPM_PowerModeSNVS == s_targetPowerMode)
                {
                    PRINTF("Now shutting down the system...\r\n");
                    APP_PowerPreSwitchHook(s_targetPowerMode);
                    CLOCK_EnableClock(kCLOCK_SnvsHp);
                    SNVS->LPCR |= SNVS_LPCR_DP_EN_MASK;
                    SNVS->LPCR |= SNVS_LPCR_TOP_MASK;
                    while (1) /* Shutdown */
                    {
                    }
                }
#endif
                else
                {
                    APP_GetWakeupConfig(s_targetPowerMode);
                    APP_SetWakeupConfig(s_targetPowerMode);
                    LPM_SetSleepPowerMode(s_targetPowerMode);
                }
                LPM_SetPowerMode(s_curRunMode);
                s_targetPowerMode = s_curRunMode;
            }
        }
        PRINTF("\r\nNext loop\r\n");
    }
}

/*!
 * @brief main demo function.
 */
int main(void)
{
    if (PGC->CPU_SR & PGC_CPU_SR_PSR_MASK)
    {
        /* MPU config */
        BOARD_ConfigMPU();

        /* Boot ROM did initialize the XTAL, here we only sets external XTAL OSC freq */
        CLOCK_SetXtalFreq(BOARD_XTAL0_CLK_HZ);
        CLOCK_SetRtcXtalFreq(BOARD_XTAL32K_CLK_HZ);

        /* Restore when wakeup from suspend reset */
        LPM_SystemResumeDsm();

        /* Recover handshaking */
        IOMUXC_GPR->GPR4  = 0x00000000;
        IOMUXC_GPR->GPR7  = 0x00000000;
        IOMUXC_GPR->GPR8  = 0x00000000;
        IOMUXC_GPR->GPR12 = 0x00000000;

        CCM->CCR &= ~CCM_CCR_REG_BYPASS_COUNT_MASK;

        /* Set the flag to mark system reeset from SUSPEND */
        is_suspend_reset = 1;

        EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        EnableIRQ(APP_WAKEUP_GPT_IRQn);

        APP_PowerPostSwitchHook(LPM_PowerModeSuspend);

        PRINTF("\r\nWakeup from suspend reset!\r\n");
        APP_PrintRunFrequency(0);

        /* Recover to Over Run after suspend reset */
        LPM_SystemOverRun();
    }
    else
    {
        /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    /* Configure UART divider to default */
    CLOCK_SetMux(kCLOCK_UartMux, 1); /* Set UART source to OSC 24M */
    CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* Set UART divider to 1 */

    /* Select OSC as PIT clock source */
    CLOCK_SetMux(kCLOCK_PerclkMux, 1); /* Set PIT clock source to OSC 24M */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0); /* Set PIT clock divider to 1 */

#ifndef XIP_EXTERNAL_FLASH
    /* In low power, better to use core pll for flexspi
       Use AXI clock for flexspi, AXI clock is from SEMC clock, 100MHz */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x0);
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0x0);

    /* In low power, better to use core pll for semc */
    CLOCK_SetMux(kCLOCK_SemcMux, 0x0); /* Use periph clock as semc clock source */
    CLOCK_SetDiv(kCLOCK_SemcDiv, 0x4); /* Semc 100MHz */
#endif

    BOARD_InitDebugConsole();
    }

    PRINTF("\r\nCPU wakeup source 0x%x...\r\n", SRC->SRSR);

    PRINTF("\r\n***********************************************************\r\n");
    PRINTF("\tPower Mode Switch Demo for iMXRT10xx\r\n");
    PRINTF("***********************************************************\r\n");
    APP_PrintRunFrequency(0);

    if (true != LPM_Init(s_curRunMode))
    {
        PRINTF("LPM Init Failed!\r\n");
        return -1;
    }

    APP_PowerModeSwitch();
    for (;;)
    {
    }
}
