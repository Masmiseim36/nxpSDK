/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "board.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
#if configSYSTICK_USE_LOW_POWER_TIMER
#include "fsl_lptmr.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_GPIO BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

#define BOARD_SW_GPIO BOARD_SW3_GPIO
#define BOARD_SW_PORT BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ BOARD_SW3_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define BOARD_SW_NAME BOARD_SW3_NAME
/* Task priorities. */
/* clang-format off */
#define tickless_task_PRIORITY   ( configMAX_PRIORITIES - 1 )
#define TIME_DELAY_SLEEP      5000
/* clang-format on */
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Whether the SW button is pressed */
static void Tickless_task(void *pvParameters);
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput, 0,
    };

#if configSYSTICK_USE_LOW_POWER_TIMER
    lptmr_config_t lptmrConfig;

    CLOCK_EnableClock(kCLOCK_Lptmr0);
    /* Configuration LPTMR  */
    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    LPTMR_Init(TICKLESS_LPTMR_BASE_PTR, &lptmrConfig);
    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(TICKLESS_LPTMR_BASE_PTR, kLPTMR_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(TICKLESS_LPTMR_IRQn);
#endif
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Print a note to terminal. */
    PRINTF("Tickless Demo example\r\n");
    PRINTF("Press %s to wake up the CPU\r\n", BOARD_SW_NAME);
    /* Init input switch GPIO. */
    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
    EnableIRQ(BOARD_SW_IRQ);
    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
    /*Create tickless task*/
    xTaskCreate(Tickless_task, "Tickless_task", configMINIMAL_STACK_SIZE, NULL, tickless_task_PRIORITY, NULL);
    /*Task Scheduler*/
    vTaskStartScheduler();
    for (;;)
        ;
}
static void Tickless_task(void *pvParameters)
{
    PRINTF("\r\nTick count :\r\n");
    for (;;)
    {
        PRINTF("%d\r\n", xTaskGetTickCount());
        vTaskDelay(TIME_DELAY_SLEEP);
    }
}
/*!
 * @brief Interrupt service fuction of switch.
 *
 * This function to wake up CPU
 */
void BOARD_SW_IRQ_HANDLER(void)
{
    PRINTF("CPU waked up by EXT interrupt\r\n");
    /* Clear external interrupt flag. */
    GPIO_ClearPinsInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
}
