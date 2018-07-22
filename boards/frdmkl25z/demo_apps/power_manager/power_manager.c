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
#include "fsl_llwu.h"
#include "fsl_rcm.h"
#include "fsl_lptmr.h"
#include "fsl_port.h"
#include "fsl_debug_console.h"
#include "power_manager.h"
#include "fsl_notifier.h"
#include "board.h"

#include "pin_mux.h"
#include "fsl_pmc.h"
#include "fsl_uart.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Default debug console clock source. */
#define APP_DEBUG_UART_BAUDRATE 9600                 /* Debug console baud rate. */
#define APP_DEBUG_UART_CLKSRC_NAME kCLOCK_Osc0ErClk /* OSC0 external reference clock */
#define APP_DEBUG_UART_DEFAULT_CLKSRC 0x02

#define LLWU_LPTMR_IDX 0U      /* LLWU_M0IF */

/* Debug console RX pin: PORTA1 MUX: 2 */
#define DEBUG_CONSOLE_RX_PORT PORTA
#define DEBUG_CONSOLE_RX_GPIO GPIOA
#define DEBUG_CONSOLE_RX_PIN 1
#define DEBUG_CONSOLE_RX_PINMUX kPORT_MuxAlt2
/* Debug console TX pin: PORTA2 MUX: 2 */
#define DEBUG_CONSOLE_TX_PORT PORTA
#define DEBUG_CONSOLE_TX_GPIO GPIOA
#define DEBUG_CONSOLE_TX_PIN 2
#define DEBUG_CONSOLE_TX_PINMUX kPORT_MuxAlt2
#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

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

void APP_SetClockVlpr(void)
{
    const sim_clock_config_t simConfig = {
        .pllFllSel = 0U,        /* PLLFLLSEL select FLL */
        .er32kSrc = 3U,         /* ERCLK32K selection, use LPO */
        .clkdiv1 = 0x00040000U, /* SIM_CLKDIV1 */
    };

    CLOCK_SetSimSafeDivs();
    CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcFast, 0U);

    /* MCG works in PEE mode now, will switch to BLPI mode. */

    CLOCK_ExternalModeToFbeModeQuick();  /* Enter FBE. */
    CLOCK_SetFbiMode(kMCG_Dmx32Default, kMCG_DrsLow, NULL); /* Enter FBI. */
    CLOCK_SetLowPowerEnable(true);       /* Enter BLPI. */

    CLOCK_SetSimConfig(&simConfig);
}

void APP_SetClockRunFromVlpr(void)
{
    const sim_clock_config_t simConfig = {
        .pllFllSel = 1U,        /* PLLFLLSEL select PLL */
        .er32kSrc = 3U,         /* ERCLK32K selection, use LPO */
        .clkdiv1 = 0x10010000U, /* SIM_CLKDIV1 */
    };

    const mcg_pll_config_t pll0Config = {
        .enableMode = 0U, .prdiv = 0x1U, .vdiv = 0x0U,
    };

    CLOCK_SetSimSafeDivs();

    /* Currently in BLPI mode, will switch to PEE mode. */
    /* Enter FBI. */
    CLOCK_SetLowPowerEnable(false);
    /* Enter FBE. */
    CLOCK_SetFbeMode(3U, kMCG_Dmx32Default, kMCG_DrsLow, NULL);
    /* Enter PBE. */
    CLOCK_SetPbeMode(kMCG_PllClkSelPll0, &pll0Config);
    /* Enter PEE. */
    CLOCK_SetPeeMode();

    CLOCK_SetSimConfig(&simConfig);
}

static void APP_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;
    uartClkSrcFreq = CLOCK_GetFreq(APP_DEBUG_UART_CLKSRC_NAME);
    CLOCK_SetLpsci0Clock(APP_DEBUG_UART_DEFAULT_CLKSRC);
    DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, APP_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}


status_t callback0(notifier_notification_block_t *notify, void *dataPtr)
{
    user_callback_data_t *userData = (user_callback_data_t *)dataPtr;
    status_t ret = kStatus_Fail;
    app_power_mode_t targetMode = ((power_user_config_t *)notify->targetConfig)->mode;
    smc_power_state_t originPowerState = userData->originPowerState;

    switch (notify->notifyType)
    {
        case kNOTIFIER_NotifyBefore:
            userData->beforeNotificationCounter++;
            /* Wait for debug console output finished. */
            while (!(kUART_TransmissionCompleteFlag & UART_GetStatusFlags((UART_Type *)BOARD_DEBUG_UART_BASEADDR)))
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

            /*
             * If enter stop modes when MCG in PEE mode, then after wakeup, the MCG is in PBE mode,
             * need to enter PEE mode manually.
             */
            if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeWait != targetMode) &&
                (kAPP_PowerModeVlpw != targetMode) && (kAPP_PowerModeVlpr != targetMode))
            {
                if (kSMC_PowerStateRun == originPowerState)
                {
                    /* Wait for PLL lock. */
                    while (!(kMCG_Pll0LockFlag & CLOCK_GetStatusFlags()))
                    {
                    }
                    CLOCK_SetPeeMode();
                }
            }

            APP_InitDebugConsole();

            ret = kStatus_Success;
            break;
        default:
            break;
    }
    return ret;
}

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
}

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

    PRINTF("Will be wake up by LPTMR - Low Power Timer\r\n");
    return kAPP_WakeupSourceLptmr;

}

/*! @brief Get wakeup timeout and wakeup source. */
void APP_GetWakeupConfig(app_power_mode_t targetMode)
{
/* Get wakeup source by user input. */
    if (targetMode == kAPP_PowerModeVlls0)
    {
        /* In VLLS0 mode, the LPO is disabled, LPTMR could not work. */
        PRINTF("Not support LPTMR wakeup because LPO is disabled in VLLS0 mode.\r\n");
        s_wakeupSource = kAPP_WakeupSourcePin;
    }
    else
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

        PRINTF("Board don't support wakeup button, prese reset button to continue\r\n");

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

    }

    /* If targetMode is VLLS/LLS, setup LLWU. */
    if ((kAPP_PowerModeWait != targetMode) && (kAPP_PowerModeVlpw != targetMode) &&
        (kAPP_PowerModeVlps != targetMode) && (kAPP_PowerModeStop != targetMode))
    {
        if (kAPP_WakeupSourceLptmr == s_wakeupSource)
        {
            LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_LPTMR_IDX, true);
        }
        else
        {

        }
        NVIC_EnableIRQ(LLWU_IRQn);
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

        case kSMC_PowerStateRun:
            if (kAPP_PowerModeVlpw == targetPowerMode)
            {
                PRINTF("Could not enter VLPW mode from RUN mode.\r\n");
                modeValid = false;
            }
            break;

        case kSMC_PowerStateVlpr:
            if ((kAPP_PowerModeWait == targetPowerMode) ||
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


    smc_power_mode_vlls_config_t vlls_config; /* Local variable for vlls configuration */

    targetPowerModeConfig = (power_user_config_t *)targetConfig;
    currentPowerMode = SMC_GetPowerModeState(SMC);
    targetPowerMode = targetPowerModeConfig->mode;

    switch (targetPowerMode)
    {
        case kAPP_PowerModeVlpr:
            APP_SetClockVlpr();
            SMC_SetPowerModeVlpr(SMC
                                 );
            while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(SMC))
            {
            }
            break;

        case kAPP_PowerModeRun:

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

        case kAPP_PowerModeLls:
            SMC_PreEnterStopModes();
            SMC_SetPowerModeLls(SMC);
            SMC_PostExitStopModes();
            break;

        case kAPP_PowerModeVlls0:
        case kAPP_PowerModeVlls1:
        case kAPP_PowerModeVlls3:
            if (targetPowerMode == kAPP_PowerModeVlls3)
            {
                vlls_config.subMode = kSMC_StopSub3;
            }
            else if (targetPowerMode == kAPP_PowerModeVlls0)
            {
                vlls_config.subMode = kSMC_StopSub0;
            }
            else
            {
                vlls_config.subMode = kSMC_StopSub1;
            }
            vlls_config.enablePorDetectInVlls0 = targetPowerModeConfig->enablePorDetectInVlls0;
            SMC_PreEnterStopModes();
            SMC_SetPowerModeVlls(SMC, &vlls_config);
            SMC_PostExitStopModes();
            break;
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


        true,


    };

    power_user_config_t vlpwConfig = vlprConfig;
    power_user_config_t vlls1Config = vlprConfig;
    power_user_config_t vlls3Config = vlprConfig;
    power_user_config_t vlpsConfig = vlprConfig;
    power_user_config_t waitConfig = vlprConfig;
    power_user_config_t stopConfig = vlprConfig;
    power_user_config_t runConfig = vlprConfig;

    power_user_config_t llsConfig = vlprConfig;

    power_user_config_t vlls0Config = vlprConfig;



    /* Initializes array of pointers to power mode configurations */
    notifier_user_config_t *powerConfigs[] =
    {
        &runConfig,
        &waitConfig,
        &stopConfig,
        &vlprConfig,
        &vlpwConfig,
        &vlpsConfig,
        &llsConfig,

        &vlls0Config,

        &vlls1Config,


        &vlls3Config,

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
    vlls1Config.mode = kAPP_PowerModeVlls1;
    vlls3Config.mode = kAPP_PowerModeVlls3;
    vlpsConfig.mode = kAPP_PowerModeVlps;
    waitConfig.mode = kAPP_PowerModeWait;
    stopConfig.mode = kAPP_PowerModeStop;
    runConfig.mode = kAPP_PowerModeRun;

    llsConfig.mode = kAPP_PowerModeLls;

    vlls0Config.mode = kAPP_PowerModeVlls0;



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
    LPTMR_GetDefaultConfig(&lptmrConfig);
    /* Use LPO as clock source. */
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
    lptmrConfig.bypassPrescaler = true;

    LPTMR_Init(LPTMR0, &lptmrConfig);

    NVIC_EnableIRQ(LLWU_IRQn);

    NVIC_EnableIRQ(LPTMR0_IRQn);


    /* Wakeup from VLLS. */
    if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM))
    {
        PRINTF("\r\nMCU wakeup from VLLS modes...\r\n");
    }

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
        PRINTF("Press  %c for enter: LLS/LLS3 - Low Leakage Stop mode\r\n", kAPP_PowerModeLls);

        PRINTF("Press  %c for enter: VLLS0    - Very Low Leakage Stop 0 mode\r\n", kAPP_PowerModeVlls0);

        PRINTF("Press  %c for enter: VLLS1    - Very Low Leakage Stop 1 mode\r\n", kAPP_PowerModeVlls1);


        PRINTF("Press  %c for enter: VLLS3    - Very Low Leakage Stop 3 mode\r\n", kAPP_PowerModeVlls3);


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
                (kAPP_PowerModeVlpr == targetPowerMode))
            {
                needSetWakeup = false;
            }

            /*
             * If there is not wakeup pin and target power mode is VLLS0,
             * then could only wakeup by RESET pin.
             */
            else if (kAPP_PowerModeVlls0 == targetPowerMode)
            {
                PRINTF("Press RESET pin to wakeup. \r\n");
                needSetWakeup = false;
            }

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
