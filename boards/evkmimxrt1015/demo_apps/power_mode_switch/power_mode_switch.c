/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "fsl_common.h"
#include "power_mode_switch.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "lpm.h"
#include "fsl_gpt.h"
#include "fsl_lpuart.h"
#include "specific.h"
#include "peripherals.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CPU_NAME "iMXRT1015"

#define APP_WAKEUP_BUTTON_GPIO BOARD_USER_BUTTON_GPIO
#define APP_WAKEUP_BUTTON_GPIO_PIN BOARD_USER_BUTTON_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ BOARD_USER_BUTTON_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME BOARD_USER_BUTTON_NAME

#define APP_WAKEUP_GPT_BASE DEMO_GPT_PERIPHERAL
#define APP_WAKEUP_GPT_IRQn GPT2_IRQn
#define APP_WAKEUP_GPT_IRQn_HANDLER GPT2_IRQHandler

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_wakeupTimeout;            /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */

static lpm_power_mode_t s_targetPowerMode;
static lpm_power_mode_t s_curRunMode = LPM_PowerModeOverRun;
static SemaphoreHandle_t s_wakeupSig;

static const char *s_modeNames[] = {"Over RUN",    "Full Run",       "Low Speed Run", "Low Power Run",
                                    "System Idle", "Low Power Idle", "Suspend",       "SNVS"};

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_WAKEUP_GPT_IRQn_HANDLER(void)
{
    GPT_ClearStatusFlags(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare1Flag);
    GPT_StopTimer(APP_WAKEUP_GPT_BASE);
    LPM_DisableWakeupSource(APP_WAKEUP_GPT_IRQn);

    xSemaphoreGiveFromISR(s_wakeupSig, NULL);
    portYIELD_FROM_ISR(pdTRUE);
    __DSB();
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

    xSemaphoreGiveFromISR(s_wakeupSig, NULL);
    portYIELD_FROM_ISR(pdTRUE);
    __DSB();
}

lpm_power_mode_t APP_GetRunMode(void)
{
    return s_curRunMode;
}

void APP_SetRunMode(lpm_power_mode_t powerMode)
{
    s_curRunMode = powerMode;
}

/*
 * Check whether could switch to target power mode from current mode.
 * Return true if could switch, return false if could not switch.
 */
bool APP_CheckPowerMode(lpm_power_mode_t originPowerMode, lpm_power_mode_t targetPowerMode)
{
    bool modeValid = true;

    /* If current mode is Lowpower run mode, the target mode should not be system idle mode. */
    if ((originPowerMode == LPM_PowerModeLowPowerRun) && (targetPowerMode == LPM_PowerModeSysIdle))
    {
        PRINTF("Low Power Run mode can't enter System Idle mode.\r\n");
        modeValid = false;
    }

    /* Don't need to change power mode if current mode is already the target mode. */
    if (originPowerMode == targetPowerMode)
    {
        PRINTF("Already in the target power mode.\r\n");
        modeValid = false;
    }

    return modeValid;
}

void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeSNVS)
    {
        PRINTF("Now shutting down the system...\r\n");
    }

    if (targetMode > LPM_PowerModeRunEnd)
    {
        /* Wait for debug console output finished. */
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();

        /*
         * Set pin for current leakage.
         * Debug console RX pin: Set pinmux to GPIO input.
         * Debug console TX pin: Don't need to change.
         */
        ConfigUartRxPinToGpio();
    }
}

void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode > LPM_PowerModeRunEnd)
    {
        /*
         * Debug console RX pin is set to GPIO input, need to re-configure pinmux.
         * Debug console TX pin: Don't need to change.
         */
        ReConfigUartRxPin();
        BOARD_InitDebugConsole();

        /* recover to previous run mode */
        switch (APP_GetRunMode())
        {
            case LPM_PowerModeOverRun:
                LPM_OverDriveRun();
                break;
            case LPM_PowerModeFullRun:
                LPM_FullSpeedRun();
                break;
            case LPM_PowerModeLowSpeedRun:
                LPM_LowSpeedRun();
                break;
            case LPM_PowerModeLowPowerRun:
                LPM_LowPowerRun();
                break;
            default:
                break;
        }
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
    if (targetMode == LPM_PowerModeSNVS)
    {
        /* In SNVS mode, only SNVS domain is powered, GPT could not work. */
        s_wakeupSource = kAPP_WakeupSourcePin;
    }
    else
    {
        /* Get wakeup source by user input. */
        s_wakeupSource = APP_GetWakeupSource(targetMode);
    }

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
        NVIC_SetPriority(APP_WAKEUP_GPT_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY + 2);
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
        NVIC_SetPriority(APP_WAKEUP_BUTTON_IRQ, configMAX_SYSCALL_INTERRUPT_PRIORITY + 2);
        /* Enable the Interrupt */
        EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        /* Enable GPC interrupt */
        LPM_EnableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
    }
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

/* Called in idle task */
static bool APP_LpmListener(lpm_power_mode_t curMode, lpm_power_mode_t newMode, void *data)
{
    PRINTF("WorkingTask %d: Transfer from %s to %s\r\n", (uint32_t)data, s_modeNames[curMode], s_modeNames[newMode]);

    /* Do necessary preparation for this mode change */

    return true; /* allow this switch */
}

/*!
 * @brief main task.
 */
static void PowerModeSwitchTask(void *pvParameters)
{
    uint8_t ch;
    uint32_t freq;

    while (1)
    {
        freq = CLOCK_GetFreq(kCLOCK_CpuClk);

        PRINTF("\r\n########## Power Mode Switch Demo (build %s) ###########\n\r\n", __DATE__);
        PRINTF("    Core Clock = %dHz \r\n", freq);

        APP_ShowPowerMode(s_curRunMode);

        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c for enter: Over RUN       - System Over Run mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeOverRun);
        PRINTF("Press  %c for enter: Full RUN       - System Full Run mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeFullRun);
        PRINTF("Press  %c for enter: Low Speed RUN  - System Low Speed Run mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowSpeedRun);
        PRINTF("Press  %c for enter: Low Power RUN  - System Low Power Run mode\r\n",
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
        PRINTF("\r\nWaiting for power mode select...\r\n\r\n");

        /* Wait for user response */
        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        s_targetPowerMode = (lpm_power_mode_t)(ch - 'A');

        if (s_targetPowerMode <= LPM_PowerModeEnd)
        {
            /* If could not set the target power mode, loop continue. */
            if (!APP_CheckPowerMode(s_curRunMode, s_targetPowerMode))
            {
                continue;
            }

            if (!LPM_SetPowerMode(s_targetPowerMode))
            {
                PRINTF("Some task doesn't allow to enter mode %s\r\n", s_modeNames[s_targetPowerMode]);
            }
            else
            {
                if (s_targetPowerMode <= LPM_PowerModeRunEnd)
                {
                    switch (s_targetPowerMode)
                    {
                        case LPM_PowerModeOverRun:
                            LPM_OverDriveRun();
                            break;
                        case LPM_PowerModeFullRun:
                            LPM_FullSpeedRun();
                            break;
                        case LPM_PowerModeLowSpeedRun:
                            LPM_LowSpeedRun();
                            break;
                        case LPM_PowerModeLowPowerRun:
                            LPM_LowPowerRun();
                            break;
                        default:
                            break;
                    }
                    APP_SetRunMode(s_targetPowerMode);
                    continue;
                }
#if (HAS_WAKEUP_PIN)
                else if (LPM_PowerModeSNVS == s_targetPowerMode)
                {
                    APP_GetWakeupConfig(s_targetPowerMode);
                    APP_SetWakeupConfig(s_targetPowerMode);
                    APP_PowerPreSwitchHook(s_targetPowerMode);
                    LPM_EnterSNVS();
                }
#endif
                else
                {
                    APP_GetWakeupConfig(s_targetPowerMode);
                    APP_SetWakeupConfig(s_targetPowerMode);
                    if (xSemaphoreTake(s_wakeupSig, portMAX_DELAY) == pdFALSE)
                    {
                        assert(0);
                    }
                }
                LPM_SetPowerMode(s_curRunMode);
            }
        }
        PRINTF("\r\nNext loop\r\n");
    }
}

/*!
 * @brief simulating working task.
 */
static void WorkingTask(void *pvParameters)
{
    LPM_RegisterPowerListener(APP_LpmListener, pvParameters);

    for (;;)
    {
        /* Use App task logic to replace vTaskDelay */
        PRINTF("Task %d is working now\r\n", (uint32_t)pvParameters);
        vTaskDelay(portMAX_DELAY);
    }
}

/*!
 * @brief main demo function.
 */
int main(void)
{
    BaseType_t xReturn;

    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitBootClocks();

    /* Configure UART divider to default */
    CLOCK_SetMux(kCLOCK_UartMux, 1); /* Set UART source to OSC 24M */
    CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* Set UART divider to 1 */

    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();

    PRINTF("\r\nCPU wakeup source 0x%x...\r\n", SRC->SRSR);

    PRINTF("\r\n***********************************************************\r\n");
    PRINTF("\tPower Mode Switch Demo for %s\r\n", CPU_NAME);
    PRINTF("***********************************************************\r\n");
    APP_PrintRunFrequency(0);

    if (true != LPM_Init(s_curRunMode))
    {
        PRINTF("LPM Init Failed!\r\n");
        return -1;
    }

    s_wakeupSig = xSemaphoreCreateBinary();
    /* Make current resource count 0 for signal purpose */
    if (xSemaphoreTake(s_wakeupSig, 0) == pdTRUE)
    {
        assert(0);
    }

    xReturn = xTaskCreate(PowerModeSwitchTask, "Power Mode Switch Task", configMINIMAL_STACK_SIZE + 512, NULL,
                          tskIDLE_PRIORITY + 1U, NULL);
    configASSERT(xReturn);

    xReturn =
        xTaskCreate(WorkingTask, "Working Task1", configMINIMAL_STACK_SIZE, (void *)1, tskIDLE_PRIORITY + 2U, NULL);
    configASSERT(xReturn);

    xReturn =
        xTaskCreate(WorkingTask, "Working Task2", configMINIMAL_STACK_SIZE, (void *)2, tskIDLE_PRIORITY + 3U, NULL);
    configASSERT(xReturn);

    vTaskStartScheduler();

    /* Application should never reach this point. */
    for (;;)
    {
    }
}
