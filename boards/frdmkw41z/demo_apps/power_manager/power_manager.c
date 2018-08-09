/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_common.h"
#include "fsl_smc.h"
#if defined(FSL_FEATURE_SOC_LLWU_COUNT) && FSL_FEATURE_SOC_LLWU_COUNT
#include "fsl_llwu.h"
#endif /* FSL_FEATURE_SOC_LLWU_COUNT */
#include "fsl_rcm.h"
#include "fsl_lptmr.h"
#include "fsl_port.h"
#include "fsl_debug_console.h"
#include "power_manager.h"
#include "fsl_notifier.h"
#include "board.h"

#include "pin_mux.h"
#include "fsl_pmc.h"
#include "fsl_lpuart.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_DEBUG_UART_BAUDRATE 9600U                /* Debug console baud rate. */
/* Default debug console clock source. */
#define APP_DEBUG_UART_DEFAULT_CLKSRC_NAME kCLOCK_Osc0ErClk /* OSC0 external reference clock (OSC0ERCLK) */
#define APP_DEBUG_UART_DEFAULT_CLKSRC 0x02U

/* Debug console clock source in VLPR mode. */
#define APP_DEBUG_UART_VLPR_CLKSRC_NAME kCLOCK_McgInternalRefClk /* MCGIRCLK */
#define APP_DEBUG_UART_VLPR_CLKSRC 0x03U

#define LLWU_LPTMR_IDX 0U       /* LLWU_M0IF */
#define LLWU_WAKEUP_PIN_IDX 12U /* LLWU_P12 */
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
 * Power mode switch callback.
 */
status_t callback0(notifier_notification_block_t *notify, void *dataPtr);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_wakeupTimeout;            /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */

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

void APP_SetClockVlpr(void)
{
    const sim_clock_config_t simConfig = {
        .er32kSrc = 0U,         /* ERCLK32K selection, use OSC. */
        .clkdiv1 = 0x00040000U, /* SIM_CLKDIV1. */
    };

    CLOCK_SetSimSafeDivs();
    CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcFast, 0U);

    /* MCG works in FEE mode now, will switch to BLPI mode. */

    CLOCK_SetFbiMode(kMCG_Dmx32Default, kMCG_DrsLow, NULL); /* Enter FBI. */
    CLOCK_SetLowPowerEnable(true);       /* Enter BLPI. */

    CLOCK_SetSimConfig(&simConfig);
}

void APP_SetClockRunFromVlpr(void)
{
    const sim_clock_config_t simConfig = {
        .er32kSrc = 0U,         /* ERCLK32K selection, use OSC. */
        .clkdiv1 = 0x00010000U, /* SIM_CLKDIV1. */
    };

    CLOCK_SetSimSafeDivs();

    /* Currently in BLPI mode, will switch to FEE mode. */
    /* Enter FBI. */
    CLOCK_SetLowPowerEnable(false);
    /* Enter FEE. */
    CLOCK_SetFeeMode(5U, kMCG_Dmx32Default, kMCG_DrsMid, APP_FllStableDelay);

    CLOCK_SetSimConfig(&simConfig);
}

static void APP_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;
    CLOCK_SetLpuartClock(APP_DEBUG_UART_DEFAULT_CLKSRC);
    uartClkSrcFreq = CLOCK_GetFreq(APP_DEBUG_UART_DEFAULT_CLKSRC_NAME);
    DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, APP_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

static void APP_InitDebugConsole_InVLPR(void)
{
    uint32_t uartClkSrcFreq;
    CLOCK_SetLpuartClock(APP_DEBUG_UART_VLPR_CLKSRC);
    uartClkSrcFreq = CLOCK_GetFreq(APP_DEBUG_UART_VLPR_CLKSRC_NAME);
    DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, APP_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}


status_t callback0(notifier_notification_block_t *notify, void *dataPtr)
{
    user_callback_data_t *userData = (user_callback_data_t *)dataPtr;
    status_t ret = kStatus_Fail;
    app_power_mode_t targetMode = ((power_user_config_t *)notify->targetConfig)->mode;
    smc_power_state_t originPowerState = userData->originPowerState;
    smc_power_state_t powerState;

    switch (notify->notifyType)
    {
        case kNOTIFIER_NotifyBefore:
            userData->beforeNotificationCounter++;
            /* Wait for debug console output finished. */
            while (
                !(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
            {
            }
            DbgConsole_Deinit();

            if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeVlpr != targetMode))
            {
                /*
                 * Set pin for current leakage.
                 * Debug console RX pin: Set to pinmux to disable.
                 * Debug console TX pin: Don't need to change.
                 */
                PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, kPORT_PinDisabledOrAnalog);
            }

            ret = kStatus_Success;
            break;
        case kNOTIFIER_NotifyRecover:
            break;
        case kNOTIFIER_CallbackAfter:
            userData->afterNotificationCounter++;
            powerState = SMC_GetPowerModeState(SMC);
            if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeVlpr != targetMode))
            {
                /*
                 * Debug console RX pin is set to disable for current leakage, nee to re-configure pinmux.
                 * Debug console TX pin: Don't need to change.
                 */
                PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, DEBUG_CONSOLE_RX_PINMUX);
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

            ret = kStatus_Success;
            break;
        default:
            break;
    }
    return ret;
}

#if defined(FSL_FEATURE_SOC_LLWU_COUNT) && FSL_FEATURE_SOC_LLWU_COUNT
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
#if defined(APP_HAS_WAKEUP_BUTTON) && APP_HAS_WAKEUP_BUTTON
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

/*!
 * @brief LPTMR0 interrupt handler.
 */
void LPTMR0_IRQHandler(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR0))
    {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);
    }
}

/*!
 * @brief button interrupt handler.
 */
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
 * @brief Get input from user to obtain wakeup timeout
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

/*!
 * @brief Get wakeup source by user input.
 */
static app_wakeup_source_t APP_GetWakeupSource(void)
{
#if defined(APP_HAS_WAKEUP_BUTTON) && APP_HAS_WAKEUP_BUTTON
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

/*! @brief Get wakeup timeout and wakeup source. */
void APP_GetWakeupConfig(app_power_mode_t targetMode)
{
/* Get wakeup source by user input. */
#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0
    if (targetMode == kAPP_PowerModeVlls0)
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
        PRINTF("Board don't support wakeup button, prese reset button to continue\r\n");
#endif
    }
}

/*! @brief Set wakeup timeout and wakeup source. */
void APP_SetWakeupConfig(app_power_mode_t targetMode)
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
#if defined(APP_HAS_WAKEUP_BUTTON) && APP_HAS_WAKEUP_BUTTON
        PORT_SetPinInterruptConfig(APP_WAKEUP_BUTTON_PORT, APP_WAKEUP_BUTTON_GPIO_PIN, APP_WAKEUP_BUTTON_IRQ_TYPE);
#endif /* APP_HAS_WAKEUP_BUTTON */
    }

    /* If targetMode is VLLS/LLS, setup LLWU. */
    if ((kAPP_PowerModeWait != targetMode) && (kAPP_PowerModeVlpw != targetMode) &&
        (kAPP_PowerModeVlps != targetMode) && (kAPP_PowerModeStop != targetMode))
    {
#if defined(FSL_FEATURE_SOC_LLWU_COUNT) && FSL_FEATURE_SOC_LLWU_COUNT
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
#endif /* FSL_FEATURE_SOC_LLWU_COUNT */
    }
}

/*! @brief Show current power mode. */
void APP_ShowPowerMode(smc_power_state_t currentPowerState)
{
    switch (currentPowerState)
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

/*!
 * @brief check whether could switch to target power mode from current mode.
 * Return true if could switch, return false if could not switch.
 */
bool APP_CheckPowerMode(smc_power_state_t currentPowerState, app_power_mode_t targetPowerMode)
{
    bool modeValid = true;

    /*
     * Check wether the mode change is allowed.
     *
     * 1. If current mode is HSRUN mode, the target mode must be RUN mode.
     * 2. If current mode is RUN mode, the target mode must not be VLPW mode.
     * 3. If current mode is VLPR mode, the target mode must not be HSRUN/WAIT/STOP mode.
     * 4. If already in the target mode, don't need to change.
     */
    switch (currentPowerState)
    {
#if defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
        case kSMC_PowerStateHsrun:
            if (kAPP_PowerModeRun != targetPowerMode)
            {
                PRINTF("Current mode is HSRUN, please choose RUN mode as the target mode.\r\n");
                modeValid = false;
            }
            break;
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

        case kSMC_PowerStateRun:
            if (kAPP_PowerModeVlpw == targetPowerMode)
            {
                PRINTF("Could not enter VLPW mode from RUN mode.\r\n");
                modeValid = false;
            }
            break;

        case kSMC_PowerStateVlpr:
            if ((kAPP_PowerModeWait == targetPowerMode) ||
#if defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                (kAPP_PowerModeHsrun == targetPowerMode) ||
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */
                (kAPP_PowerModeStop == targetPowerMode))
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
    if (((kAPP_PowerModeRun == targetPowerMode) && (kSMC_PowerStateRun == currentPowerState)) ||
#if defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
        ((kAPP_PowerModeHsrun == targetPowerMode) && (kSMC_PowerStateHsrun == currentPowerState)) ||
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */
        ((kAPP_PowerModeVlpr == targetPowerMode) && (kSMC_PowerStateVlpr == currentPowerState)))
    {
        PRINTF("Already in the target power mode.\r\n");
        return false;
    }

    return true;
}

/*!
 * @brief Power mode switch.
 * This function is used to register the notifier_handle_t struct's member userFunction.
 */
status_t APP_PowerModeSwitch(notifier_user_config_t *targetConfig, void *userData)
{
    smc_power_state_t currentPowerMode;         /* Local variable with current power mode */
    app_power_mode_t targetPowerMode;           /* Local variable with target power mode name*/
    power_user_config_t *targetPowerModeConfig; /* Local variable with target power mode configruation */

#if (defined(FSL_FEATURE_SMC_HAS_LLS_SUBMODE) && FSL_FEATURE_SMC_HAS_LLS_SUBMODE) || \
    (defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO)
    smc_power_mode_lls_config_t lls_config; /* Local variable for lls configuration */
#endif                                      /* (FSL_FEATURE_SMC_HAS_LLS_SUBMODE || FSL_FEATURE_SMC_HAS_LPOPO) */

#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
    smc_power_mode_vlls_config_t vlls_config; /* Local variable for vlls configuration */
#endif

    targetPowerModeConfig = (power_user_config_t *)targetConfig;
    currentPowerMode = SMC_GetPowerModeState(SMC);
    targetPowerMode = targetPowerModeConfig->mode;

    switch (targetPowerMode)
    {
        case kAPP_PowerModeVlpr:
            APP_SetClockVlpr();
            SMC_SetPowerModeVlpr(SMC
#if defined(FSL_FEATURE_SMC_HAS_LPWUI) && FSL_FEATURE_SMC_HAS_LPWUI
                                 ,
                                 targetPowerModeConfig->enableLowPowerWakeUpOnInterrupt
#endif /* FSL_FEATURE_SMC_HAS_LPWUI */
                                 );
            while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(SMC))
            {
            }
            break;

        case kAPP_PowerModeRun:
#if defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
            /* If enter RUN from HSRUN, fisrt change clock. */
            if (kSMC_PowerStateHsrun == currentPowerMode)
            {
                APP_SetClockRunFromHsrun();
            }
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

            /* Power mode change. */
            SMC_SetPowerModeRun(SMC);
            while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
            {
            }

            /* If enter RUN from VLPR, change clock after the power mode change. */
            if (kSMC_PowerStateVlpr == currentPowerMode)
            {
                APP_SetClockRunFromVlpr();
            }
            break;

#if defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
        case kAPP_PowerModeHsrun:
            SMC_SetPowerModeHsrun(SMC);
            while (kSMC_PowerStateHsrun != SMC_GetPowerModeState(SMC))
            {
            }

            APP_SetClockHsrun(); /* Change clock setting after power mode change. */
            break;
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

        /* For wait modes. */
        case kAPP_PowerModeWait:
            SMC_PreEnterWaitModes();
            SMC_SetPowerModeWait(SMC);
            SMC_PostExitWaitModes();
            break;
        case kAPP_PowerModeVlpw:
            SMC_PreEnterWaitModes();
            SMC_SetPowerModeVlpw(SMC);
            SMC_PostExitWaitModes();
            break;

        /* For stop modes. */
        case kAPP_PowerModeStop:
            SMC_PreEnterStopModes();
            SMC_SetPowerModeStop(SMC, kSMC_PartialStop);
            SMC_PostExitStopModes();
            break;

        case kAPP_PowerModeVlps:
            SMC_PreEnterStopModes();
            SMC_SetPowerModeVlps(SMC);
            SMC_PostExitStopModes();
            break;

#if defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
        case kAPP_PowerModeLls:
#if defined(FSL_FEATURE_SMC_HAS_LLS_SUBMODE) && FSL_FEATURE_SMC_HAS_LLS_SUBMODE
            lls_config.subMode = kSMC_StopSub3;
#endif /* FSL_FEATURE_SMC_HAS_LLS_SUBMODE */
#if defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO
            lls_config.enableLpoClock = targetPowerModeConfig->enableLpoClock;
#endif /* FSL_FEATURE_SMC_HAS_LPOPO */
#if ((defined(FSL_FEATURE_SMC_HAS_LLS_SUBMODE) && FSL_FEATURE_SMC_HAS_LLS_SUBMODE) || \
     (defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO))
            SMC_PreEnterStopModes();
            SMC_SetPowerModeLls(SMC, &lls_config);
            SMC_PostExitStopModes();
#else
            SMC_PreEnterStopModes();
            SMC_SetPowerModeLls(SMC);
            SMC_PostExitStopModes();
#endif /* (FSL_FEATURE_SMC_HAS_LLS_SUBMODE || FSL_FEATURE_SMC_HAS_LPOPO) */
            break;
#endif /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */

#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0
        case kAPP_PowerModeVlls0:
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */
        case kAPP_PowerModeVlls1:
#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2
        case kAPP_PowerModeVlls2:
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 */
        case kAPP_PowerModeVlls3:
            if (targetPowerMode == kAPP_PowerModeVlls3)
            {
                vlls_config.subMode = kSMC_StopSub3;
            }
#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2
            else if (targetPowerMode == kAPP_PowerModeVlls2)
            {
                vlls_config.subMode = kSMC_StopSub2;
            }
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 */
#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0
            else if (targetPowerMode == kAPP_PowerModeVlls0)
            {
                vlls_config.subMode = kSMC_StopSub0;
            }
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */
            else
            {
                vlls_config.subMode = kSMC_StopSub1;
            }
#if defined(FSL_FEATURE_SMC_HAS_PORPO) && FSL_FEATURE_SMC_HAS_PORPO
            vlls_config.enablePorDetectInVlls0 = targetPowerModeConfig->enablePorDetectInVlls0;
#endif /* FSL_FEATURE_SMC_HAS_PORPO */
#if defined(FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION) && FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION
            vlls_config.enableRam2InVlls2 = targetPowerModeConfig->enableRam2InVlls2;
#endif /* FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION */
#if defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO
            vlls_config.enableLpoClock = targetPowerModeConfig->enableLpoClock;
#endif /* FSL_FEATURE_SMC_HAS_LPOPO */
            SMC_PreEnterStopModes();
            SMC_SetPowerModeVlls(SMC, &vlls_config);
            SMC_PostExitStopModes();
            break;
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */
        default:
            PRINTF("Wrong value");
            break;
    }
    return kStatus_Success;
}

/*!
 * @brief main demo function.
 */
int main(void)
{
    uint32_t freq = 0;
    uint8_t ch;
    uint8_t targetConfigIndex;
    notifier_handle_t powerModeHandle;
    smc_power_state_t currentPowerState;
    app_power_mode_t targetPowerMode;
    bool needSetWakeup; /* Flag of whether or not need to set wakeup. */
    lptmr_config_t lptmrConfig;

    /*Power mode configurations*/
    power_user_config_t vlprConfig =
    {
        kAPP_PowerModeVlpr,

#if defined(FSL_FEATURE_SMC_HAS_LPWUI) && FSL_FEATURE_SMC_HAS_LPWUI
        false,
#endif /* FSL_FEATURE_SMC_HAS_LPWUI */

#if defined(FSL_FEATURE_SMC_HAS_PORPO) && FSL_FEATURE_SMC_HAS_PORPO
        true,
#endif /* FSL_FEATURE_SMC_HAS_PORPO */

#if defined(FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION) && FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION
        true,
#endif /* FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION */

#if defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO
        true,
#endif /* FSL_FEATURE_SMC_HAS_LPOPO */
    };

    power_user_config_t vlpwConfig = vlprConfig;
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
    power_user_config_t vlls1Config = vlprConfig;
    power_user_config_t vlls3Config = vlprConfig;
#endif
    power_user_config_t vlpsConfig = vlprConfig;
    power_user_config_t waitConfig = vlprConfig;
    power_user_config_t stopConfig = vlprConfig;
    power_user_config_t runConfig = vlprConfig;

#if defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
    power_user_config_t llsConfig = vlprConfig;
#endif /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */

#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0
    power_user_config_t vlls0Config = vlprConfig;
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */

#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2
    power_user_config_t vlls2Config = vlprConfig;
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 */

#if defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
    power_user_config_t hsrunConfig = vlprConfig;
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

    /* Initializes array of pointers to power mode configurations */
    notifier_user_config_t *powerConfigs[] =
    {
        &runConfig,
        &waitConfig,
        &stopConfig,
        &vlprConfig,
        &vlpwConfig,
        &vlpsConfig,
#if defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
        &llsConfig,
#endif /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */

#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0
        &vlls0Config,
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */

        &vlls1Config,

#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2
        &vlls2Config,
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 */

        &vlls3Config,
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */

#if defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
        &hsrunConfig,
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */
    };

    /* User callback data0 */
    user_callback_data_t callbackData0;

    /* Initializes callback configuration structure */
    notifier_callback_config_t callbackCfg0 = {callback0, kNOTIFIER_CallbackBeforeAfter, (void *)&callbackData0};

    /* Initializes array of callback configurations */
    notifier_callback_config_t callbacks[] = {callbackCfg0};

    memset(&callbackData0, 0, sizeof(user_callback_data_t));

    /* Initializes configuration structures */
    vlpwConfig.mode = kAPP_PowerModeVlpw;
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
    vlls1Config.mode = kAPP_PowerModeVlls1;
    vlls3Config.mode = kAPP_PowerModeVlls3;
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */
    vlpsConfig.mode = kAPP_PowerModeVlps;
    waitConfig.mode = kAPP_PowerModeWait;
    stopConfig.mode = kAPP_PowerModeStop;
    runConfig.mode = kAPP_PowerModeRun;

#if defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
    llsConfig.mode = kAPP_PowerModeLls;
#endif /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */

#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0
    vlls0Config.mode = kAPP_PowerModeVlls0;
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */

#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2
    vlls2Config.mode = kAPP_PowerModeVlls2;
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 */
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */

#if defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
    hsrunConfig.mode = kAPP_PowerModeHsrun;
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

    /* Create Notifier Handle */
    NOTIFIER_CreateHandle(&powerModeHandle, powerConfigs, ARRAY_SIZE(powerConfigs), callbacks, 1U, APP_PowerModeSwitch,
                          NULL);

    /* Power related. */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
    {
        PMC_ClearPeriphIOIsolationFlag(PMC);
        NVIC_ClearPendingIRQ(LLWU_IRQn);
    }

    BOARD_InitPins();
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
    /* Use LPO as clock source. */
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
    lptmrConfig.bypassPrescaler = true;

    LPTMR_Init(LPTMR0, &lptmrConfig);

#if defined(FSL_FEATURE_SOC_LLWU_COUNT) && FSL_FEATURE_SOC_LLWU_COUNT
    NVIC_EnableIRQ(LLWU_IRQn);
#endif /* FSL_FEATURE_SOC_LLWU_COUNT */

    NVIC_EnableIRQ(LPTMR0_IRQn);

#if defined(APP_HAS_WAKEUP_BUTTON) && APP_HAS_WAKEUP_BUTTON
    NVIC_EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
#endif /* APP_HAS_WAKEUP_BUTTON */

#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
    /* Wakeup from VLLS. */
    if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM))
    {
        PRINTF("\r\nMCU wakeup from VLLS modes...\r\n");
    }
#endif

    while (1)
    {
        currentPowerState = SMC_GetPowerModeState(SMC);

        freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);

        PRINTF("\r\n####################  Power Manager Demo ####################\n\r\n");
        PRINTF("    Core Clock = %dHz \r\n", freq);

        APP_ShowPowerMode(currentPowerState);

        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c for enter: RUN      - Normal RUN mode\r\n", kAPP_PowerModeRun);
        PRINTF("Press  %c for enter: WAIT     - Wait mode\r\n", kAPP_PowerModeWait);
        PRINTF("Press  %c for enter: STOP     - Stop mode\r\n", kAPP_PowerModeStop);
        PRINTF("Press  %c for enter: VLPR     - Very Low Power Run mode\r\n", kAPP_PowerModeVlpr);
        PRINTF("Press  %c for enter: VLPW     - Very Low Power Wait mode\r\n", kAPP_PowerModeVlpw);
        PRINTF("Press  %c for enter: VLPS     - Very Low Power Stop mode\r\n", kAPP_PowerModeVlps);
#if defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
        PRINTF("Press  %c for enter: LLS/LLS3 - Low Leakage Stop mode\r\n", kAPP_PowerModeLls);
#endif /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */

#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0
        PRINTF("Press  %c for enter: VLLS0    - Very Low Leakage Stop 0 mode\r\n", kAPP_PowerModeVlls0);
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */

        PRINTF("Press  %c for enter: VLLS1    - Very Low Leakage Stop 1 mode\r\n", kAPP_PowerModeVlls1);

#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2
        PRINTF("Press  %c for enter: VLLS2    - Very Low Leakage Stop 2 mode\r\n", kAPP_PowerModeVlls2);
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 */

        PRINTF("Press  %c for enter: VLLS3    - Very Low Leakage Stop 3 mode\r\n", kAPP_PowerModeVlls3);
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */

#if defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
        PRINTF("Press  %c for enter: HSRUN    - High Speed RUN mode\r\n", kAPP_PowerModeHsrun);
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");

        /* Wait for user response */
        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        targetPowerMode = (app_power_mode_t)ch;

        if ((targetPowerMode > kAPP_PowerModeMin) && (targetPowerMode < kAPP_PowerModeMax))
        {
            /* If could not set the target power mode, loop continue. */
            if (!APP_CheckPowerMode(currentPowerState, targetPowerMode))
            {
                continue;
            }

            /* If target mode is RUN/VLPR/HSRUN, don't need to set wakeup source. */
            if ((kAPP_PowerModeRun == targetPowerMode) ||
#if defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                (kAPP_PowerModeHsrun == targetPowerMode) ||
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */
                (kAPP_PowerModeVlpr == targetPowerMode))
            {
                needSetWakeup = false;
            }
#if (!APP_HAS_WAKEUP_BUTTON)
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0)
            /*
             * If there is not wakeup pin and target power mode is VLLS0,
             * then could only wakeup by RESET pin.
             */
            else if (kAPP_PowerModeVlls0 == targetPowerMode)
            {
                PRINTF("Press RESET pin to wakeup. \r\n");
                needSetWakeup = false;
            }
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */
#endif
#if defined(FSL_FEATURE_SOC_LLWU_COUNT) && (!FSL_FEATURE_SOC_LLWU_COUNT)
            /*
             * If no LLWU module and target power mode is LLS/VLLS, then should only wakeup
             * by RESET, don't need to set wakeup source.
             */
            else if (false
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
                     || (kAPP_PowerModeVlls3 == targetPowerMode)
#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2
                     || (kAPP_PowerModeVlls2 == targetPowerMode)
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 */

#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0
                     || (kAPP_PowerModeVlls0 == targetPowerMode)
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */

                     || (kAPP_PowerModeVlls1 == targetPowerMode)
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */
#if defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
                     || (kAPP_PowerModeLls == targetPowerMode)
#endif /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */
                     )
            {
                PRINTF("Press RESET pin to wakeup. \r\n");
                needSetWakeup = false;
            }
#endif /* FSL_FEATURE_SOC_LLWU_COUNT */
            else
            {
                needSetWakeup = true;
            }

            if (needSetWakeup)
            {
                APP_GetWakeupConfig(targetPowerMode);
                APP_SetWakeupConfig(targetPowerMode);
            }

            callbackData0.originPowerState = currentPowerState;
            targetConfigIndex = targetPowerMode - kAPP_PowerModeMin - 1;
            NOTIFIER_SwitchConfig(&powerModeHandle, targetConfigIndex, kNOTIFIER_PolicyAgreement);
            PRINTF("\r\nNext loop\r\n");
        }
    }
}
