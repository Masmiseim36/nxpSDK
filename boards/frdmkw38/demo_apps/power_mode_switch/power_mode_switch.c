/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_smc.h"
#include "fsl_power.h"
#if (defined(FSL_FEATURE_SOC_LLWU_COUNT) && FSL_FEATURE_SOC_LLWU_COUNT)
#include "fsl_llwu.h"
#endif
#include "fsl_rcm.h"
#include "fsl_lptmr.h"
#include "fsl_port.h"
#include "power_mode_switch.h"
#include "board.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "fsl_pmc.h"
#include "fsl_lpuart.h"
#include "fsl_clock.h"
#include "fsl_dcdc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_DEBUG_UART_BAUDRATE 9600 /* Debug console baud rate. */
/* Default debug console clock source. */
#define APP_DEBUG_UART_DEFAULT_CLKSRC_NAME kCLOCK_Osc0ErClk /* OSC0 external reference clock (OSC0ERCLK) */
#define APP_DEBUG_UART_DEFAULT_CLKSRC 0x02U

/* Debug console clock source in VLPR mode. */
#define APP_DEBUG_UART_VLPR_CLKSRC_NAME kCLOCK_McgInternalRefClk /* MCGIRCLK */
#define APP_DEBUG_UART_VLPR_CLKSRC 0x03U

#define LLWU_LPTMR_IDX 0U       /* LLWU_M0IF */
#define LLWU_WAKEUP_PIN_IDX 10U /* LLWU_P12 */
#define LLWU_WAKEUP_PIN_TYPE kLLWU_ExternalPinFallingEdge

#define APP_HAS_WAKEUP_BUTTON 1 /* 1 if have wakeup button, 0 if not. */
#define APP_WAKEUP_BUTTON_GPIO BOARD_SW3_GPIO
#define APP_WAKEUP_BUTTON_PORT BOARD_SW3_PORT
#define APP_WAKEUP_BUTTON_GPIO_PIN BOARD_SW3_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ BOARD_SW3_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME BOARD_SW3_NAME
#define APP_WAKEUP_BUTTON_IRQ_TYPE kPORT_InterruptFallingEdge

/* Debug console RX pin: PORTC6 MUX: 4 */
#define DEBUG_CONSOLE_RX_PORT PORTC
#define DEBUG_CONSOLE_RX_GPIO GPIOC
#define DEBUG_CONSOLE_RX_PIN 6U
#define DEBUG_CONSOLE_RX_PINMUX kPORT_MuxAlt4
/* Debug console TX pin: PORTC7 MUX: 4 */
#define DEBUG_CONSOLE_TX_PORT PORTC
#define DEBUG_CONSOLE_TX_GPIO GPIOC
#define DEBUG_CONSOLE_TX_PIN 7U
#define DEBUG_CONSOLE_TX_PINMUX kPORT_MuxAlt4
#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, power_mode_t targetMode);
void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, power_mode_t targetMode);
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
/*
 * Set the clock configuration for HSRUN mode.
 */
extern void APP_SetClockHsrun(void);

/*
 * Set the clock configuration for RUN mode from HSRUN mode.
 */
extern void APP_SetClockRunFromHsrun(void);
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
extern void APP_SetClockRunFromVlpr(void);

/*
 * Set the clock configuration for VLPR mode.
 */
extern void APP_SetClockVlpr(void);

/*
 * Hook function called before power mode switch.
 */
extern void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, power_mode_t targetMode);

/*
 * Hook function called after power mode switch.
 */
extern void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, power_mode_t targetMode);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_wakeupTimeout;            /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */
#ifdef SUPPORT_WARMBOOT
static bool b_warmbootFlag = false;
static uint32_t s_dataCheckVal;
AT_WARMBOOTCONFIG_SECTION(warmboot_config_t warmbootConfig);
const uint32_t RAM_STORED_DATA = 0x5A5A5A5A;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Delay at least 1ms for FLL stable. */
static void APP_FllStableDelay(void)
{
    volatile uint32_t i = 30000U;
    while (i--)
    {
        __NOP();
    }
}

#define MCG_IRCLK_DISABLE 0U           /*!< MCGIRCLK disabled */
#define SIM_OSC32KSEL_OSC32KCLK_CLK 0U /*!< OSC32KSEL select: OSC32KCLK clock */

void APP_SetLowPowerClockConfig(void)
{
    const sim_clock_config_t simConfig = {
        .er32kSrc = SIM_OSC32KSEL_OSC32KCLK_CLK, /* OSC32KSEL select: OSC32KCLK clock */
        .clkdiv1  = 0x10000U,                    /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV4: /2 */
    };

    /* ERR010224 */
    RSIM->RF_OSC_CTRL |=
        RSIM_RF_OSC_CTRL_RADIO_EXT_OSC_OVRD_EN_MASK; /* Prevent XTAL_OUT_EN from generating XTAL_OUT request */
    /* Set the system clock dividers in SIM to safe value. */
    CLOCK_SetSimSafeDivs();
    /* Set MCG to FEI mode. */
#if FSL_CLOCK_DRIVER_VERSION >= MAKE_VERSION(2, 2, 0)
    CLOCK_BootToFeiMode(kMCG_Dmx32Default, kMCG_DrsLow, APP_FllStableDelay);
#else
    CLOCK_BootToFeiMode(kMCG_DrsMid, CLOCK_CONFIG_FllStableDelay);
#endif
    /* Configure the Internal Reference clock (MCGIRCLK). */
    CLOCK_SetInternalRefClkConfig(MCG_IRCLK_DISABLE, kMCG_IrcSlow, 0U);
    /* Set the clock configuration in SIM module. */
    CLOCK_SetSimConfig(&simConfig);
}

void APP_SetClockVlpr(void)
{
    const sim_clock_config_t simConfig = {
        .er32kSrc = 0U,          /* ERCLK32K selection, use OSC. */
        .clkdiv1  = 0x00040000U, /* SIM_CLKDIV1. */
    };

    CLOCK_SetSimSafeDivs();
    CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcFast, 0U);

    /* MCG works in FEE mode now, will switch to BLPI mode. */

    CLOCK_SetFbiMode(kMCG_Dmx32Default, kMCG_DrsLow, NULL); /* Enter FBI. */
    CLOCK_SetLowPowerEnable(true);                          /* Enter BLPI. */

    CLOCK_SetSimConfig(&simConfig);
}

void APP_SetClockRunFromVlpr(void)
{
    const sim_clock_config_t simConfig = {
        .er32kSrc = 0U,          /* ERCLK32K selection, use OSC. */
        .clkdiv1  = 0x00010000U, /* SIM_CLKDIV1. */
    };

    CLOCK_SetSimSafeDivs();

    /* Currently in BLPI mode, will switch to FEE mode. */
    /* Enter FBI. */
    CLOCK_SetLowPowerEnable(false);
    /* Enter FEE. */
    CLOCK_SetFeeMode(5U, kMCG_Dmx32Default, kMCG_DrsMid, APP_FllStableDelay);

    CLOCK_SetSimConfig(&simConfig);
}

static void BOARD_ConfigureDcdc(void)
{
    CLOCK_EnableClock(kCLOCK_Dcdc0);

    dcdc_low_power_config_t dcdcLowPowerConfig;
    dcdc_loop_control_config_t dcdcLoopControlConfig;

    DCDC_GetDefaultLowPowerConfig(&dcdcLowPowerConfig);
    dcdcLowPowerConfig.enableDiffComparators = true;
    DCDC_SetLowPowerConfig(DCDC, &dcdcLowPowerConfig);

    DCDC_GetDefaultLoopControlConfig(&dcdcLoopControlConfig);
    dcdcLoopControlConfig.enableDiffHysteresis       = true;
    dcdcLoopControlConfig.enableCommonHysteresis     = true;
    dcdcLoopControlConfig.enableInvertHysteresisSign = true;
    DCDC_SetLoopControlConfig(DCDC, &dcdcLoopControlConfig);

    DCDC_LockTargetVoltage(DCDC);
    DCDC_SetBatteryVoltageDivider(DCDC, kDCDC_VBatVoltageDividerOff);

    CLOCK_DisableClock(kCLOCK_Dcdc0);
}

static void APP_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;
    CLOCK_SetLpuart0Clock(APP_DEBUG_UART_DEFAULT_CLKSRC);
    uartClkSrcFreq = CLOCK_GetFreq(APP_DEBUG_UART_DEFAULT_CLKSRC_NAME);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, APP_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

static void APP_InitDebugConsole_InVLPR(void)
{
    uint32_t uartClkSrcFreq;
    CLOCK_SetLpuart0Clock(APP_DEBUG_UART_VLPR_CLKSRC);
    uartClkSrcFreq = CLOCK_GetFreq(APP_DEBUG_UART_VLPR_CLKSRC_NAME);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, APP_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}


void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, power_mode_t targetMode)
{
    /* Wait for debug console output finished. */
    while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
    {
    }
    DbgConsole_Deinit();

    if ((kPWR_PowerModeRun != targetMode) && (kPWR_PowerModeVlpr != targetMode))
    {
        /*
         * Set pin for current leakage.
         * Debug console RX/TX pin: Set to pinmux to disable.
         */
        PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, kPORT_PinDisabledOrAnalog);
        PORT_SetPinMux(DEBUG_CONSOLE_TX_PORT, DEBUG_CONSOLE_TX_PIN, kPORT_PinDisabledOrAnalog);
    }

    /* Clock set to low power config. */
    switch (targetMode)
    {
        case kPWR_PowerModeStop:
        case kPWR_PowerModeVlps:
#if (defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE)
        case kPWR_PowerModeLls:
#endif
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0)
        case kPWR_PowerModeVlls0:
#endif
        case kPWR_PowerModeVlls1:
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2)
        case kPWR_PowerModeVlls2:
#endif
        case kPWR_PowerModeVlls3:
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */
            if (kSMC_PowerStateRun == originPowerState)
            {
                APP_SetLowPowerClockConfig();
            }
            RSIM->CONTROL &= ~RSIM_CONTROL_RF_OSC_EN_MASK;
            break;
        default:
            break;
    }
}

void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, power_mode_t targetMode)
{
    smc_power_state_t powerState = SMC_GetPowerModeState(SMC);

    if ((kPWR_PowerModeRun != targetMode) && (kPWR_PowerModeVlpr != targetMode))
    {
        /*
         * Debug console RX/TX pin is set to disable for current leakage, need to re-configure pinmux.
         */
        PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, DEBUG_CONSOLE_RX_PINMUX);
        PORT_SetPinMux(DEBUG_CONSOLE_TX_PORT, DEBUG_CONSOLE_TX_PIN, DEBUG_CONSOLE_TX_PINMUX);
    }

    switch (targetMode)
    {
        case kPWR_PowerModeVlpr:
            RSIM->CONTROL &= ~RSIM_CONTROL_RF_OSC_EN_MASK;
            break;
        case kPWR_PowerModeStop:
        case kPWR_PowerModeVlps:
#if (defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE)
        case kPWR_PowerModeLls:
#endif
            if (kSMC_PowerStateRun == originPowerState)
            {
                BOARD_BootClockRUN();
            }
            break;
#ifdef SUPPORT_WARMBOOT
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2)
        case kPWR_PowerModeVlls2:
#endif
        case kPWR_PowerModeVlls3:
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */
            /* Must configure pins before PMC_ClearPeriphIOIsolationFlag */
            BOARD_InitPins();
            /* Power related. */
            SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
            if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
            {
                PMC_ClearPeriphIOIsolationFlag(PMC);
                NVIC_ClearPendingIRQ(LLWU_IRQn);
            }
            BOARD_ConfigureDcdc();
            BOARD_BootClockRUN();
            break;
#endif
        default:
            break;
    }

    /*
     * For some other platforms, if enter LLS mode from VLPR mode, when wakeup, the
     * power mode is VLPR. But for some platforms, if enter LLS mode from VLPR mode,
     * when wakeup, the power mode is RUN. In this case, the clock setting is still
     * VLPR mode setting, so change to RUN mode setting here.
     */
    if ((kSMC_PowerStateVlpr == originPowerState) && (kSMC_PowerStateRun == SMC_GetPowerModeState(SMC)))
    {
        APP_SetClockRunFromVlpr();
    }

    /* Set debug console clock source. */
    if (kSMC_PowerStateVlpr == powerState)
    {
        APP_InitDebugConsole_InVLPR();
    }
    else
    {
        APP_InitDebugConsole();
    }
}

#if (defined(FSL_FEATURE_SOC_LLWU_COUNT) && FSL_FEATURE_SOC_LLWU_COUNT)
/*!
 * @brief LLWU interrupt handler.
 */
void LLWU_IRQHandler(void)
{
    /* If wakeup by LPTMR. */
    if (LLWU_GetInternalWakeupModuleFlag(LLWU, LLWU_LPTMR_IDX))
    {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);
    }
#if (defined(APP_HAS_WAKEUP_BUTTON) && APP_HAS_WAKEUP_BUTTON)
    /* If wakeup by external pin. */
    if (LLWU_GetExternalWakeupPinFlag(LLWU, LLWU_WAKEUP_PIN_IDX))
    {
        PORT_SetPinInterruptConfig(APP_WAKEUP_BUTTON_PORT, APP_WAKEUP_BUTTON_GPIO_PIN, kPORT_InterruptOrDMADisabled);
        PORT_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_PORT, (1U << APP_WAKEUP_BUTTON_GPIO_PIN));
        LLWU_ClearExternalWakeupPinFlag(LLWU, LLWU_WAKEUP_PIN_IDX);
    }
#endif /* APP_HAS_WAKEUP_BUTTON */
}
#endif /* FSL_FEATURE_SOC_LLWU_COUNT */

void LPTMR0_IRQHandler(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR0))
    {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);
    }
}

#if (defined(APP_HAS_WAKEUP_BUTTON) && APP_HAS_WAKEUP_BUTTON)
void APP_WAKEUP_BUTTON_IRQ_HANDLER(void)
{
    if ((1U << APP_WAKEUP_BUTTON_GPIO_PIN) & PORT_GetPinsInterruptFlags(APP_WAKEUP_BUTTON_PORT))
    {
        /* Disable interrupt. */
        PORT_SetPinInterruptConfig(APP_WAKEUP_BUTTON_PORT, APP_WAKEUP_BUTTON_GPIO_PIN, kPORT_InterruptOrDMADisabled);
        PORT_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_PORT, (1U << APP_WAKEUP_BUTTON_GPIO_PIN));
    }
}
#endif /* APP_HAS_WAKEUP_BUTTON */

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
static app_wakeup_source_t APP_GetWakeupSource(void)
{
#if (defined(APP_HAS_WAKEUP_BUTTON) && APP_HAS_WAKEUP_BUTTON)
    uint8_t ch;

    while (1)
    {
        PRINTF("Select the wake up source:\r\n");
        PRINTF("Press T for LPTMR - Low Power Timer\r\n");
        PRINTF("Press S for switch/button %s. \r\n", APP_WAKEUP_BUTTON_NAME);

        PRINTF("\r\nWaiting for key press..\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        if (ch == 'T')
        {
            return kAPP_WakeupSourceLptmr;
        }
        else if (ch == 'S')
        {
            return kAPP_WakeupSourcePin;
        }
        else
        {
            PRINTF("Wrong value!\r\n");
        }
    }

#else /* APP_HAS_WAKEUP_BUTTON */

    PRINTF("Will be wake up by LPTMR - Low Power Timer\r\n");
    return kAPP_WakeupSourceLptmr;

#endif /* APP_HAS_WAKEUP_BUTTON */
}

/* Get wakeup timeout and wakeup source. */
void APP_GetWakeupConfig(power_mode_t targetMode)
{
/* Get wakeup source by user input. */
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0)
    if (targetMode == kPWR_PowerModeVlls0)
    {
        /* In VLLS0 mode, the LPO is disabled, LPTMR could not work. */
        PRINTF("Not support LPTMR wakeup because LPO is disabled in VLLS0 mode.\r\n");
        s_wakeupSource = kAPP_WakeupSourcePin;
    }
    else
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */
    {
        /* Get wakeup source by user input. */
        s_wakeupSource = APP_GetWakeupSource();
    }

    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        /* Wakeup source is LPTMR, user should input wakeup timeout value. */
        s_wakeupTimeout = APP_GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
    }
    else
    {
#if (defined(APP_HAS_WAKEUP_BUTTON) && APP_HAS_WAKEUP_BUTTON)
        PRINTF("Press %s to wake up.\r\n", APP_WAKEUP_BUTTON_NAME);
#else
        PRINTF("Board don't support wakeup button, press reset button to continue\r\n");
#endif
    }
}

void APP_SetWakeupConfig(power_mode_t targetMode)
{
    /* Set LPTMR timeout value. */
    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        LPTMR_SetTimerPeriod(LPTMR0, (1000U * s_wakeupTimeout) - 1U);
        LPTMR_StartTimer(LPTMR0);
    }

    /* Set the wakeup module. */
    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    }
    else
    {
#if (defined(APP_HAS_WAKEUP_BUTTON) && APP_HAS_WAKEUP_BUTTON)
        PORT_SetPinInterruptConfig(APP_WAKEUP_BUTTON_PORT, APP_WAKEUP_BUTTON_GPIO_PIN, APP_WAKEUP_BUTTON_IRQ_TYPE);
#endif /* APP_HAS_WAKEUP_BUTTON */
    }

    /* If targetMode is VLLS/LLS, setup LLWU. */
    if ((kPWR_PowerModeWait != targetMode) && (kPWR_PowerModeVlpw != targetMode) &&
        (kPWR_PowerModeVlps != targetMode) && (kPWR_PowerModeStop != targetMode))
    {
#if (defined(FSL_FEATURE_SOC_LLWU_COUNT) && FSL_FEATURE_SOC_LLWU_COUNT)
        if (kAPP_WakeupSourceLptmr == s_wakeupSource)
        {
            LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_LPTMR_IDX, true);
        }
        else
        {
#if (defined(APP_HAS_WAKEUP_BUTTON) && APP_HAS_WAKEUP_BUTTON)
            LLWU_SetExternalWakeupPinMode(LLWU, LLWU_WAKEUP_PIN_IDX, LLWU_WAKEUP_PIN_TYPE);
#endif /* APP_HAS_WAKEUP_BUTTON */
        }
        NVIC_EnableIRQ(LLWU_IRQn);
#endif
    }
}

void APP_ShowPowerMode(smc_power_state_t powerMode)
{
    switch (powerMode)
    {
        case kSMC_PowerStateRun:
            PRINTF("    Power mode: RUN\r\n");
            break;
        case kSMC_PowerStateVlpr:
            PRINTF("    Power mode: VLPR\r\n");
            break;
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
        case kSMC_PowerStateHsrun:
            PRINTF("    Power mode: HSRUN\r\n");
            break;
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */
        default:
            PRINTF("    Power mode wrong\r\n");
            break;
    }
}

/*
 * Check whether could switch to target power mode from current mode.
 * Return true if could switch, return false if could not switch.
 */
bool APP_CheckPowerMode(smc_power_state_t curPowerState, power_mode_t targetPowerMode)
{
    bool modeValid = true;

    /*
     * Check wether the mode change is allowed.
     *
     * 1. If current mode is HSRUN mode, the target mode must be RUN mode.
     * 2. If current mode is RUN mode, the target mode must not be VLPW mode.
     * 3. If current mode is VLPR mode, the target mode must not be HSRUN/WAIT/STOP mode.
     * 4. If already in the target mode.
     */
    switch (curPowerState)
    {
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
        case kSMC_PowerStateHsrun:
            if (kPWR_PowerModeRun != targetPowerMode)
            {
                PRINTF("Current mode is HSRUN, please choose RUN mode as the target mode.\r\n");
                modeValid = false;
            }
            break;
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

        case kSMC_PowerStateRun:
            if (kPWR_PowerModeVlpw == targetPowerMode)
            {
                PRINTF("Could not enter VLPW mode from RUN mode.\r\n");
                modeValid = false;
            }
            break;

        case kSMC_PowerStateVlpr:
            if ((kPWR_PowerModeWait == targetPowerMode) ||
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
                (kPWR_PowerModeHsrun == targetPowerMode) ||
#endif
                (kPWR_PowerModeStop == targetPowerMode))
            {
                PRINTF("Could not enter HSRUN/STOP/WAIT modes from VLPR mode.\r\n");
                modeValid = false;
            }
            break;
        default:
            PRINTF("Wrong power state.\r\n");
            modeValid = false;
            break;
    }

    if (!modeValid)
    {
        return false;
    }

    /* Don't need to change power mode if current mode is already the target mode. */
    if (((kPWR_PowerModeRun == targetPowerMode) && (kSMC_PowerStateRun == curPowerState)) ||
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
        ((kPWR_PowerModeHsrun == targetPowerMode) && (kSMC_PowerStateHsrun == curPowerState)) ||
#endif
        ((kPWR_PowerModeVlpr == targetPowerMode) && (kSMC_PowerStateVlpr == curPowerState)))
    {
        PRINTF("Already in the target power mode.\r\n");
        return false;
    }

    return true;
}

/*
 * Power mode switch.
 */
void APP_PowerModeSwitch(smc_power_state_t curPowerState, power_mode_t targetPowerMode)
{
#ifdef SUPPORT_WARMBOOT
    warmbootConfig.doWarmbootSequence = 0;
    warmbootConfig.useWarmbootSp      = 0;
#endif

    switch (targetPowerMode)
    {
        case kPWR_PowerModeVlpr:
            APP_SetClockVlpr();
            POWER_EnterPowerMode(kPWR_PowerModeVlpr);
            break;

        case kPWR_PowerModeRun:
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
            /* If enter RUN from HSRUN, fisrt change clock. */
            if (kSMC_PowerStateHsrun == curPowerState)
            {
                APP_SetClockRunFromHsrun();
            }
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

            POWER_EnterPowerMode(kPWR_PowerModeRun);

            /* If enter RUN from VLPR, change clock after the power mode change. */
            if (kSMC_PowerStateVlpr == curPowerState)
            {
                APP_SetClockRunFromVlpr();
            }
            break;

#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
        case kPWR_PowerModeHsrun:
            POWER_EnterPowerMode(kPWR_PowerModeHsrun);

            APP_SetClockHsrun(); /* Change clock setting after power mode change. */
            break;
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

        case kPWR_PowerModeWait:
            POWER_EnterPowerMode(kPWR_PowerModeWait);
            break;

        case kPWR_PowerModeStop:
            POWER_EnterPowerMode(kPWR_PowerModeStop);
            break;

        case kPWR_PowerModeVlpw:
            POWER_EnterPowerMode(kPWR_PowerModeVlpw);
            break;

        case kPWR_PowerModeVlps:
            POWER_EnterPowerMode(kPWR_PowerModeVlps);
            break;

#if (defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE)
        case kPWR_PowerModeLls:
            POWER_EnterPowerMode(kPWR_PowerModeLls);
            break;
#endif /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */

#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0)
        case kPWR_PowerModeVlls0:
            POWER_EnterPowerMode(kPWR_PowerModeVlls0);
            break;
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */

        case kPWR_PowerModeVlls1:
            POWER_EnterPowerMode(kPWR_PowerModeVlls1);
            break;

#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2)
        case kPWR_PowerModeVlls2:
#ifdef SUPPORT_WARMBOOT
            warmbootConfig.doWarmbootSequence = WARMBOOT_SEQUENCE;
            warmbootConfig.useWarmbootSp      = USE_WARMBOOT_SP;
            s_dataCheckVal                    = RAM_STORED_DATA;
#endif
            POWER_EnterPowerMode(kPWR_PowerModeVlls2);
            break;
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 */

        case kPWR_PowerModeVlls3:
#ifdef SUPPORT_WARMBOOT
            warmbootConfig.doWarmbootSequence = WARMBOOT_SEQUENCE;
            warmbootConfig.useWarmbootSp      = USE_WARMBOOT_SP;
            s_dataCheckVal                    = RAM_STORED_DATA;
#endif
            POWER_EnterPowerMode(kPWR_PowerModeVlls3);
            break;
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */

        default:
            PRINTF("Wrong value");
            break;
    }
}

/*!
 * @brief main demo function.
 */
int main(void)
{
    uint32_t freq = 0;
    uint8_t ch;
    smc_power_state_t curPowerState;
    power_mode_t targetPowerMode;
    bool needSetWakeup; /* Need to set wakeup. */
    lptmr_config_t lptmrConfig;

    /* Must configure pins before PMC_ClearPeriphIOIsolationFlag */
    BOARD_InitPins();

    /* Power related. */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
    {
        PMC_ClearPeriphIOIsolationFlag(PMC);
        NVIC_ClearPendingIRQ(LLWU_IRQn);
    }

    BOARD_ConfigureDcdc();
    BOARD_BootClockRUN();
    APP_InitDebugConsole();

    /* Setup LPTMR. */
    /*
     * lptmrConfig->timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig->pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig->pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig->enableFreeRunning = false;
     * lptmrConfig->bypassPrescaler = true;
     * lptmrConfig->prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig->value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1; /* Use LPO as clock source. */
    lptmrConfig.bypassPrescaler      = true;

    LPTMR_Init(LPTMR0, &lptmrConfig);

#if (defined(FSL_FEATURE_SOC_LLWU_COUNT) && FSL_FEATURE_SOC_LLWU_COUNT)
    NVIC_EnableIRQ(LLWU_IRQn);
#endif
    NVIC_EnableIRQ(LPTMR0_IRQn);
#if (defined(APP_HAS_WAKEUP_BUTTON) && APP_HAS_WAKEUP_BUTTON)
    NVIC_EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
#endif

#if defined(FSL_FEATURE_RCM_HAS_WAKEUP) && FSL_FEATURE_RCM_HAS_WAKEUP
    if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
    {
        PRINTF("\r\nMCU wakeup from VLLS modes...\r\n");
    }
#endif /* FSL_FEATURE_RCM_HAS_WAKEUP */
    while (1)
    {
        curPowerState = SMC_GetPowerModeState(SMC);

        freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);

        PRINTF("\r\n####################  Power Mode Switch Demo ####################\n\r\n");
        PRINTF("    Core Clock = %dHz \r\n", freq);

        APP_ShowPowerMode(curPowerState);

        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c for enter: RUN      - Normal RUN mode\r\n", kPWR_PowerModeRun);
        PRINTF("Press  %c for enter: WAIT     - Wait mode\r\n", kPWR_PowerModeWait);
        PRINTF("Press  %c for enter: STOP     - Stop mode\r\n", kPWR_PowerModeStop);
        PRINTF("Press  %c for enter: VLPR     - Very Low Power Run mode\r\n", kPWR_PowerModeVlpr);
        PRINTF("Press  %c for enter: VLPW     - Very Low Power Wait mode\r\n", kPWR_PowerModeVlpw);
        PRINTF("Press  %c for enter: VLPS     - Very Low Power Stop mode\r\n", kPWR_PowerModeVlps);
#if (defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE)
        PRINTF("Press  %c for enter: LLS/LLS3 - Low Leakage Stop mode\r\n", kPWR_PowerModeLls);
#endif
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0)
        PRINTF("Press  %c for enter: VLLS0    - Very Low Leakage Stop 0 mode\r\n", kPWR_PowerModeVlls0);
#endif
        PRINTF("Press  %c for enter: VLLS1    - Very Low Leakage Stop 1 mode\r\n", kPWR_PowerModeVlls1);
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2)
        PRINTF("Press  %c for enter: VLLS2    - Very Low Leakage Stop 2 mode\r\n", kPWR_PowerModeVlls2);
#endif
        PRINTF("Press  %c for enter: VLLS3    - Very Low Leakage Stop 3 mode\r\n", kPWR_PowerModeVlls3);
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
        PRINTF("Press  %c for enter: HSRUN    - High Speed RUN mode\r\n", kPWR_PowerModeHsrun);
#endif

        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");

        /* Wait for user response */
        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        targetPowerMode = (power_mode_t)ch;

        if ((targetPowerMode > kPWR_PowerModeMin) && (targetPowerMode < kPWR_PowerModeMax))
        {
            /* If could not set the target power mode, loop continue. */
            if (!APP_CheckPowerMode(curPowerState, targetPowerMode))
            {
                continue;
            }

            /* If target mode is RUN/VLPR/HSRUN, don't need to set wakeup source. */
            if ((kPWR_PowerModeRun == targetPowerMode) ||
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
                (kPWR_PowerModeHsrun == targetPowerMode) ||
#endif
                (kPWR_PowerModeVlpr == targetPowerMode))
            {
                needSetWakeup = false;
            }
#if (!APP_HAS_WAKEUP_BUTTON)
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0)
            /*
             * If there is not wakeup pin and target power mode is VLLS0,
             * then could only wakeup by RESET pin.
             */
            else if (kPWR_PowerModeVlls0 == targetPowerMode)
            {
                PRINTF("Press RESET pin to wakeup. \r\n");
                needSetWakeup = false;
            }
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */
#endif
#if (defined(FSL_FEATURE_SOC_LLWU_COUNT) && (!FSL_FEATURE_SOC_LLWU_COUNT))
/*
 * If no LLWU module and target power mode is LLS/VLLS, then should only wakeup
 * by RESET, don't need to set wakeup source.
 */
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
            else if ((kPWR_PowerModeVlls3 == targetPowerMode) ||
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2)
                     (kPWR_PowerModeVlls2 == targetPowerMode) ||
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 */
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0)
                     (kPWR_PowerModeVlls0 == targetPowerMode) ||
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */
#if (defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE)
                     (kPWR_PowerModeVlls == targetPowerMode) ||
#endif /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */
                     (kPWR_PowerModeVlls1 == targetPowerMode))
            {
                PRINTF("Press RESET pin to wakeup. \r\n");
                needSetWakeup = false;
            }
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */
#endif /* FSL_FEATURE_SOC_LLWU_COUNT */
            else
            {
                needSetWakeup = true;
            }

            if (needSetWakeup)
            {
                APP_GetWakeupConfig(targetPowerMode);
            }

            APP_PowerPreSwitchHook(curPowerState, targetPowerMode);

            if (needSetWakeup)
            {
                APP_SetWakeupConfig(targetPowerMode);
            }

            APP_PowerModeSwitch(curPowerState, targetPowerMode);
            APP_PowerPostSwitchHook(curPowerState, targetPowerMode);

#ifdef SUPPORT_WARMBOOT
            if (b_warmbootFlag)
            {
                b_warmbootFlag = false;
                PRINTF("\r\nIt is a warmboot process...\r\n");
                if (s_dataCheckVal == RAM_STORED_DATA)
                {
                    PRINTF("\r\nDATA is retained...\r\n");
                }
                else
                {
                    PRINTF("\r\nDATA is NOT retained...\r\n");
                }
            }
#endif

            PRINTF("\r\nNext loop\r\n");
        }
    }
}

#ifdef SUPPORT_WARMBOOT
int warmmain(void)
{
    b_warmbootFlag = true;

    WarmbootRestore();
    while (1)
        ;
}
#endif
