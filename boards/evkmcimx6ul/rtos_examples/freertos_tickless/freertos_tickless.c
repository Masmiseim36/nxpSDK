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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "board.h"
#if defined(FSL_FEATURE_SOC_PORT_COUNT) && (FSL_FEATURE_SOC_PORT_COUNT > 0)
#include "fsl_port.h"
#endif
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#if configUSE_TICKLESS_IDLE
#include "fsl_epit.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_SW_GPIO GPIO3
#define BOARD_SW_GPIO_PIN 12U
#define BOARD_SW_IRQ GPIO3_Combined_0_15_IRQn
#define BOARD_SW_NAME "SW601-4"
#define BOARD_SW_GIC_PRIO 25
#define BOARD_SW_DELAY 1
/* @brief FreeRTOS tickless timer configuration. */
#define TIMER_Type EPIT_Type
#define TICKLESS_TIMER_BASE_PTR EPIT1  /*!< Tickless timer base address. */
#define TICKLESS_TIMER_IRQn EPIT1_IRQn /*!< Tickless timer IRQ number. */
/* Task priorities. */
/* clang-format off */
#define tickless_task_PRIORITY   ( configMAX_PRIORITIES - 2 )
#define SW_task_PRIORITY   ( configMAX_PRIORITIES - 1 )
#define TIME_DELAY_SLEEP      5000

/* Interrupt priorities. */
#define SW_NVIC_PRIO 2

/* clang-format on */
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void vPortTimerIsr(void);
TIMER_Type *vPortGetTimerBase(void);
IRQn_Type vPortGetTimerIrqn(void);
void BOARD_SW_IRQ_HANDLER(void);
void BOARD_TIMER_IRQ_HANDLER(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static void Tickless_task(void *pvParameters);
static void SW_task(void *pvParameters);

SemaphoreHandle_t xSWSemaphore = NULL;
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
/*!
 * @brief Main function
 */
int main(void)
{
/* Define the init structure for the input switch pin */
#ifdef BOARD_SW_NAME
    gpio_pin_config_t sw_config =
    {
        kGPIO_DigitalInput,
        0,
#if defined(FSL_FEATURE_SOC_IGPIO_COUNT) && (FSL_FEATURE_SOC_IGPIO_COUNT > 0)
        kGPIO_IntRisingEdge,
#endif
    };
#endif
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Install IRQ Handler */
    SystemInitIrqTable();
    SystemInstallIrqHandler(BOARD_SW_IRQ, (system_irq_handler_t)(uint32_t)BOARD_SW_IRQ_HANDLER, NULL);
#if configUSE_TICKLESS_IDLE
    epit_config_t epitConfig;

    /* Configuration LPTMR  */
    /*
     * epitConfig.clockSource = kEPIT_ClockSource_Periph;
     * epitConfig.divider = 1U;
     * epitConfig.enableRunInStop = true;
     * epitConfig.enableRunInWait = true;
     * epitConfig.enableRunInDbg = false;
     * epitConfig.enableCounterOverwrite = false;
     * epitConfig.enableFreeRun = false;
     * epitConfig.enableResetMode = true;
     */
    EPIT_GetDefaultConfig(&epitConfig);
    EPIT_Init(TICKLESS_TIMER_BASE_PTR, &epitConfig);
    /* Enable timer interrupt */
    EPIT_EnableInterrupts(TICKLESS_TIMER_BASE_PTR, kEPIT_OutputCompareInterruptEnable);
    /* Enable at the GIC */
    EnableIRQ(TICKLESS_TIMER_IRQn);
    SystemInstallIrqHandler(TICKLESS_TIMER_IRQn, (system_irq_handler_t)(uint32_t)BOARD_TIMER_IRQ_HANDLER, NULL);
#endif

    /* Print a note to terminal. */
    PRINTF("Tickless Demo example\r\n");
#ifdef BOARD_SW_NAME
    PRINTF("Press or turn on %s to wake up the CPU\r\n", BOARD_SW_NAME);

/* Init input switch GPIO. */
#if defined(FSL_FEATURE_SOC_PORT_COUNT) && (FSL_FEATURE_SOC_PORT_COUNT > 0)
    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
#endif

#if defined(__CORTEX_M)
    NVIC_SetPriority(BOARD_SW_IRQ, SW_NVIC_PRIO);
#else
    GIC_SetPriority(BOARD_SW_IRQ, BOARD_SW_GIC_PRIO);
#endif

    EnableIRQ(BOARD_SW_IRQ);
    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
#if defined(FSL_FEATURE_SOC_IGPIO_COUNT) && (FSL_FEATURE_SOC_IGPIO_COUNT > 0)
    GPIO_ClearPinsInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
    GPIO_EnableInterrupts(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
#endif
#endif

    /*Create tickless task*/
    if (xTaskCreate(Tickless_task, "Tickless_task", configMINIMAL_STACK_SIZE + 38, NULL, tickless_task_PRIORITY,
                    NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    if (xTaskCreate(SW_task, "Switch_task", configMINIMAL_STACK_SIZE + 38, NULL, SW_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    /*Task Scheduler*/
    vTaskStartScheduler();
    for (;;)
        ;
}

/* Tickless Task */
static void Tickless_task(void *pvParameters)
{
    PRINTF("\r\nTick count :\r\n");
    for (;;)
    {
        PRINTF("%d\r\n", xTaskGetTickCount());
        vTaskDelay(TIME_DELAY_SLEEP);
    }
}

/* Switch Task */
static void SW_task(void *pvParameters)
{
    xSWSemaphore = xSemaphoreCreateBinary();
    for (;;)
    {
        if (xSemaphoreTake(xSWSemaphore, portMAX_DELAY) == pdTRUE)
        {
            PRINTF("CPU waked up by EXT interrupt\r\n");
        }
    }
}
/*!
 * @brief Interrupt service fuction of switch.
 *
 * This function to wake up CPU
 */
#ifdef BOARD_SW_NAME
void BOARD_SW_IRQ_HANDLER(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

/* Clear external interrupt flag. */
#ifdef BOARD_SW_DELAY
    volatile uint32_t i = 0;
    for (i = 0; i < 10000000; ++i)
    {
        __NOP(); /* delay */
    }
    GPIO_ClearPinsInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
    if (1 == GPIO_ReadPinInput(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN))
    {
        xSemaphoreGiveFromISR(xSWSemaphore, &xHigherPriorityTaskWoken);
    }
#else
    GPIO_ClearPinsInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
    xSemaphoreGiveFromISR(xSWSemaphore, &xHigherPriorityTaskWoken);
#endif
}
#endif

#if configUSE_TICKLESS_IDLE == 1
#if defined(FSL_FEATURE_SOC_LPTMR_COUNT) && (FSL_FEATURE_SOC_LPTMR_COUNT > 0)
/*!
 * @brief Interrupt service fuction of LPT timer.
 *
 * This function to call vPortLptmrIsr
 */
void BOARD_LPTMR_IRQ_HANDLER(void)
{
    vPortLptmrIsr();
}

/*!
 * @brief Fuction of LPT timer.
 *
 * This function to return LPT timer base address
 */

LPTMR_Type *vPortGetLptrmBase(void)
{
    return TICKLESS_LPTMR_BASE_PTR;
}

/*!
 * @brief Fuction of LPT timer.
 *
 * This function to return LPT timer interrupt number
 */

IRQn_Type vPortGetLptmrIrqn(void)
{
    return TICKLESS_LPTMR_IRQn;
}
#else
/*!
 * @brief Interrupt service fuction of other timer.
 *
 * This function to call vPortTimerIsr
 */
void BOARD_TIMER_IRQ_HANDLER(void)
{
    vPortTimerIsr();
}

/*!
 * @brief Fuction of other timer.
 *
 * This function to return other timer base address
 */

TIMER_Type *vPortGetTimerBase(void)
{
    return TICKLESS_TIMER_BASE_PTR;
}

/*!
 * @brief Fuction of other timer.
 *
 * This function to return other timer interrupt number
 */

IRQn_Type vPortGetTimerIrqn(void)
{
    return TICKLESS_TIMER_IRQn;
}
#endif
#endif /* configUSE_TICKLESS_IDLE */
