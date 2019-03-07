/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
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
#include "power_mode_switch.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "lpm.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "fsl_epit.h"
#include "fsl_gpio.h"

#include "pin_mux.h"
#include "fsl_gpt.h"
#include "fsl_uart.h"
#include "fsl_iomuxc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_WAKEUP_BUTTON_GPIO GPIO3
#define APP_WAKEUP_BUTTON_GPIO_PIN 11U
#define APP_WAKEUP_BUTTON_IRQ GPIO3_Combined_0_15_IRQn
#define APP_WAKEUP_BUTTON_NAME "SW601-3"

/* Debug console RX pin: PORTC3 MUX: 3 */
#define DEBUG_CONSOLE_RX_PORT PORTC
#define DEBUG_CONSOLE_RX_GPIO GPIOC
#define DEBUG_CONSOLE_RX_PIN 3
#define DEBUG_CONSOLE_RX_PINMUX kPORT_MuxAlt3
/* Debug console TX pin: PORTC4 MUX: 3 */
#define DEBUG_CONSOLE_TX_PORT PORTC
#define DEBUG_CONSOLE_TX_GPIO GPIOC
#define DEBUG_CONSOLE_TX_PIN 4
#define DEBUG_CONSOLE_TX_PINMUX kPORT_MuxAlt3
#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode);
void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode);
/*
 * Hook function called before power mode switch.
 */
extern void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode);

/*
 * Hook function called after power mode switch.
 */
extern void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_wakeupTimeout;            /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */

static volatile lpm_power_mode_t s_targetPowerMode;
static SemaphoreHandle_t s_wakeupSig;
static const char *s_modeNames[] = {"RUN", "WAIT", "IDLE", "DSM", "OFF"};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*Provides own implementation of vApplicationIRQHandler() to call SystemIrqHandler()
 *directly without saving the FPU registers on interrupt entry.
 */
void vApplicationIRQHandler(uint32_t ulICCIAR)
{
    SystemIrqHandler(ulICCIAR);
}

void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeDsm || targetMode == LPM_PowerModeOff)
    {
        DbgConsole_Deinit();

        /*
         * Set pin for current leakage.
         * Debug console RX pin: Set to pinmux to GPIO input.
         * Debug console TX pin: Don't need to change.
         */
        IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_GPIO1_IO17, 0);
        IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_GPIO1_IO17, IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                                 IOMUXC_SW_PAD_CTL_PAD_PUS(2) |
                                                                 IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
    }
}

void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeDsm)
    {
        /*
         * Debug console RX pin is set to GPIO input, nee to re-configure pinmux.
         * Debug console TX pin: Don't need to change.
         */
        IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);
        IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, IOMUXC_SW_PAD_CTL_PAD_SPEED(2));

        BOARD_InitDebugConsole();
    }
}

void EPIT1_IRQHandler(uint32_t giccIar, void *param)
{
    if (kEPIT_OutputCompareInterruptEnable & EPIT_GetEnabledInterrupts(EPIT1))
    {
        EPIT_DisableInterrupts(EPIT1, kEPIT_OutputCompareInterruptEnable);
        EPIT_ClearStatusFlags(EPIT1, kEPIT_OutputCompareFlag);
        EPIT_StopTimer(EPIT1);
        LPM_DisableWakeupSource(EPIT1_IRQn);
    }

    xSemaphoreGiveFromISR(s_wakeupSig, NULL);
    portYIELD_FROM_ISR(pdTRUE);
}

void GPIO_IRQHandler(uint32_t giccIar, void *param)
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
        PRINTF("Press T for EPIT - EPIT Timer\r\n");
        PRINTF("Press S for switch/button %s. \r\n", APP_WAKEUP_BUTTON_NAME);

        PRINTF("\r\nWaiting for key press..\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        if (ch == 'T')
        {
            return kAPP_WakeupSourceEpit;
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
}

/* Get wakeup timeout and wakeup source. */
void APP_GetWakeupConfig(lpm_power_mode_t targetMode)
{
    /* Get wakeup source by user input. */
    s_wakeupSource = APP_GetWakeupSource(targetMode);

    if (kAPP_WakeupSourceEpit == s_wakeupSource)
    {
        /* Wakeup source is EPIT, user should input wakeup timeout value. */
        s_wakeupTimeout = APP_GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
    }
    else
    {
        PRINTF("Switch %s from off to on to wake up.\r\n", APP_WAKEUP_BUTTON_NAME);
    }
}

void APP_SetWakeupConfig(lpm_power_mode_t targetMode)
{
    /* Set EPIT timeout value. */
    if (kAPP_WakeupSourceEpit == s_wakeupSource)
    {
        EPIT_EnableInterrupts(EPIT1, kEPIT_OutputCompareInterruptEnable);
        EPIT_SetTimerPeriod(EPIT1, (32768U * s_wakeupTimeout) - 1U);
        EPIT_StartTimer(EPIT1);
        LPM_EnableWakeupSource(EPIT1_IRQn);
    }
    else
    {
        GPIO_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        /* Enable GPIO pin interrupt */
        GPIO_EnableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        /* Enable GPC interrupt */
        LPM_EnableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
    }
}

void APP_ShowPowerMode(lpm_power_mode_t powerMode)
{
    switch (powerMode)
    {
        case LPM_PowerModeRun:
            PRINTF("    Power mode: RUN\r\n");
            break;
        default:
            PRINTF("    Power mode wrong\r\n");
            break;
    }
}

/*!
 * @brief main task.
 */
static void PowerModeSwitchTask(void *pvParameters)
{
    uint8_t ch;
    uint32_t freq;
    lpm_power_mode_t targetPowerMode;
    /* Structure of initialize EPIT */
    epit_config_t epitConfig;
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t swConfig = {
        kGPIO_DigitalInput, 0, kGPIO_IntRisingEdge,
    };

    /*
    epitConfig.clockSource = kEPIT_ClockSource_Periph;
    epitConfig.divider = 1U;
    epitConfig.enableRunInStop = true;
    epitConfig.enableRunInWait = true;
    epitConfig.enableRunInDbg = false;
    epitConfig.enableCounterOverwrite = false;
    epitConfig.enableFreeRun = false;
    epitConfig.enableResetMode = true;
    */
    EPIT_GetDefaultConfig(&epitConfig);
    epitConfig.clockSource = kEPIT_ClockSource_LowFreq; /* 32K RTC OSC */

    /* Init EPIT module */
    EPIT_Init(EPIT1, &epitConfig);

    EPIT_SetOutputCompareValue(EPIT1, 0);

    /* Enable the Interrupt */
    SystemInstallIrqHandler(EPIT1_IRQn, EPIT1_IRQHandler, NULL);
    GIC_SetPriority(EPIT1_IRQn, configMAX_API_CALL_INTERRUPT_PRIORITY + 2);
    EnableIRQ(EPIT1_IRQn);

    /* Init input switch GPIO. */
    GPIO_PinInit(APP_WAKEUP_BUTTON_GPIO, APP_WAKEUP_BUTTON_GPIO_PIN, &swConfig);

    /* Enable the Interrupt */
    SystemInstallIrqHandler(APP_WAKEUP_BUTTON_IRQ, GPIO_IRQHandler, NULL);
    GIC_SetPriority(APP_WAKEUP_BUTTON_IRQ, configMAX_API_CALL_INTERRUPT_PRIORITY + 2);
    EnableIRQ(APP_WAKEUP_BUTTON_IRQ);

    PRINTF("\r\nCPU wakeup source 0x%x...\r\n", SRC->SRSR);

    while (1)
    {
        freq = CLOCK_GetFreq(kCLOCK_CpuClk);

        PRINTF("\r\n########## Power Mode Switch Demo (build %s) ###########\n\r\n", __DATE__);
        PRINTF("    Core Clock = %dHz \r\n", freq);

        APP_ShowPowerMode(LPM_PowerModeRun);

        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c for enter: RUN      - System Run mode\r\n", (uint8_t)'A' + (uint8_t)LPM_PowerModeRun);
        PRINTF("Press  %c for enter: WAIT     - System Wait mode\r\n", (uint8_t)'A' + (uint8_t)LPM_PowerModeWait);
        PRINTF("Press  %c for enter: IDLE     - Low Power Idle mode\r\n", (uint8_t)'A' + (uint8_t)LPM_PowerModeIdle);
        PRINTF("Press  %c for enter: DSM      - Deep Sleep mode\r\n", (uint8_t)'A' + (uint8_t)LPM_PowerModeDsm);
        PRINTF("Press  %c for enter: OFF      - Shutdown the system\r\n", (uint8_t)'A' + (uint8_t)LPM_PowerModeOff);

        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");

        /* Wait for user response */
        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        targetPowerMode = (lpm_power_mode_t)(ch - 'A');

        if (targetPowerMode <= LPM_PowerModeOff)
        {
            /* If target mode is RUN, don't need to change anything. */
            if (LPM_PowerModeRun == targetPowerMode)
            {
                continue;
            }

            if (!LPM_NotifyPowerMode(targetPowerMode))
            {
                PRINTF("Some task doesn't allow to enter mode %s\r\n", s_modeNames[targetPowerMode]);
            }
            else
            {
                if (LPM_PowerModeOff == targetPowerMode)
                {
                    PRINTF("Now shutting down the system...\r\n");
                    /* flush debug console */
                    DbgConsole_Flush();

                    APP_PowerPreSwitchHook(targetPowerMode);
                    CLOCK_EnableClock(kCLOCK_SnvsHp);
                    SNVS->LPCR |= SNVS_LPCR_DP_EN_MASK;
                    SNVS->LPCR |= SNVS_LPCR_TOP_MASK;
                    while (1) /* Shutdown */
                    {
                    }
                }
                else
                {
                    APP_GetWakeupConfig(targetPowerMode);
                    APP_SetWakeupConfig(targetPowerMode);
                    /* flush debug console */
                    DbgConsole_Flush();
                    LPM_SetPowerMode(targetPowerMode);
                    s_targetPowerMode = targetPowerMode;
                    xSemaphoreTake(s_wakeupSig, portMAX_DELAY);
                }
                LPM_NotifyPowerMode(LPM_PowerModeRun);
                LPM_SetPowerMode(LPM_PowerModeRun);
                s_targetPowerMode = LPM_PowerModeRun;
            }
        }
        PRINTF("\r\nNext loop\r\n");
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
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Install IRQ Handler */
    SystemInitIrqTable();

    /* FOLLOWING CONFIGURATION NEED TO BE CUSTOMIZED BY REAL APPLICATION.
     * User need to configure the pads to low leakage in APP_PowerPreSwitchHook()
     * and restore them to functional in APP_PowerPostSwitchHook().
     */
    /* Power off LCD power */
    BOARD_NXP74LV595_SetValue(kNXP74LV595_LCD_nPWREN, kSignal_NXP74LV595_High);
    /* Hold Camera reset */
    BOARD_NXP74LV595_SetValue(kNXP74LV595_CSI_RST, kSignal_NXP74LV595_High);
    /* Power down Camera power */
    BOARD_NXP74LV595_SetValue(kNXP74LV595_CSI_PWDN, kSignal_NXP74LV595_High);
    /* Standby CAN transceiver */
    BOARD_NXP74LV595_SetValue(kNXP74LV595_CAN1_2_STBY, kSignal_NXP74LV595_High);
    /* Turn off Bluetooth */
    BOARD_NXP74LV595_SetValue(kNXP74LV595_BT_nPWD, kSignal_NXP74LV595_High);
    /* Hold ENET1 reset */
    BOARD_NXP74LV595_SetValue(kNXP74LV595_ENET1_nRST, kSignal_NXP74LV595_Low);
    /* Hold ENET2 reset */
    BOARD_NXP74LV595_SetValue(kNXP74LV595_ENET2_nRST, kSignal_NXP74LV595_Low);
    /* Hold HDMI reset */
    BOARD_NXP74LV595_SetValue(kNXP74LV595_HDMI_nRST, kSignal_NXP74LV595_High);

    LPM_Init();

    s_wakeupSig = xSemaphoreCreateBinary();
    /* Make current resource count 0 for signal purpose */
    xSemaphoreTake(s_wakeupSig, 0);

    xTaskCreate(PowerModeSwitchTask, "Power Mode Switch Task", configMINIMAL_STACK_SIZE + 512, NULL,
                tskIDLE_PRIORITY + 1U, NULL);
    xTaskCreate(WorkingTask, "Working Task1", configMINIMAL_STACK_SIZE, (void *)1, tskIDLE_PRIORITY + 2U, NULL);
    xTaskCreate(WorkingTask, "Working Task2", configMINIMAL_STACK_SIZE, (void *)2, tskIDLE_PRIORITY + 3U, NULL);
    vTaskStartScheduler();

    /* Application should never reach this point. */
    for (;;)
    {
    }
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint32_t irqMask;

    irqMask = DisableGlobalIRQ();

    /* Only when no context switch is pending and no task is waiting for the scheduler
     * to be unsuspended then enter low power entry.
     */
    if (eTaskConfirmSleepModeStatus() != eAbortSleep)
    {
        APP_PowerPreSwitchHook(s_targetPowerMode);
        LPM_WaitForInterrupt((uint64_t)1000 * xExpectedIdleTime / configTICK_RATE_HZ);
        APP_PowerPostSwitchHook(s_targetPowerMode);
    }

    EnableGlobalIRQ(irqMask);
}
