/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "tickless_api.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
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
static void Tickless_task(void *pvParameters);
#ifdef BOARD_SW_NAME
static void SW_task(void *pvParameters);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
SemaphoreHandle_t xSWSemaphore = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("Press any key to start the example\r\n");
    GETCHAR();

    /* Print a note to terminal. */
    PRINTF("Tickless Demo example\r\n");
#ifdef BOARD_SW_NAME
    PRINTF("Press or turn on %s to wake up the CPU\r\n", BOARD_SW_NAME);

    /* Initialize GPIO/button HAL */
    const tickless_gpio_hal_t *gpio_hal = tickless_get_gpio_hal();
    gpio_hal->init();

/* Enable button interrupt */
#if defined(__CORTEX_M)
    NVIC_SetPriority(gpio_hal->get_irqn(), SW_NVIC_PRIO);
#else
    GIC_SetPriority(gpio_hal->get_irqn(), BOARD_SW_GIC_PRIO);
#endif

    EnableIRQ(gpio_hal->get_irqn());
#endif

    /* Create tickless task */
    if (xTaskCreate(Tickless_task, "Tickless_task", configMINIMAL_STACK_SIZE + 100, NULL, tickless_task_PRIORITY,
                    NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
#ifdef BOARD_SW_NAME
    if (xTaskCreate(SW_task, "Switch_task", configMINIMAL_STACK_SIZE + 100, NULL, SW_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
#endif
    PRINTF("\r\nTick count :\r\n");
    vTaskStartScheduler();
    for (;;)
        ;
}

/* Tickless Task */
static void Tickless_task(void *pvParameters)
{
    for (;;)
    {
        PRINTF("%d\r\n", xTaskGetTickCount());
        vTaskDelay(TIME_DELAY_SLEEP);
    }
}

#ifdef BOARD_SW_NAME
/* Switch Task */
static void SW_task(void *pvParameters)
{
    xSWSemaphore = xSemaphoreCreateBinary();
    for (;;)
    {
        if (xSemaphoreTake(xSWSemaphore, portMAX_DELAY) == pdTRUE)
        {
            PRINTF("CPU woken up by external interrupt\r\n");
        }
    }
}

#ifndef BOARD_SW_IRQ_HANDLER
#error "FreeRTOS Tickless Please define BOARD_SW_IRQ_HANDLER in app.h"
#endif
/*!
 * @brief Interrupt service function of switch.
 *
 * This function to wake up CPU
 */
void BOARD_SW_IRQ_HANDLER(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    /* Call HAL-specific handler */
    const tickless_gpio_hal_t *gpio_hal = tickless_get_gpio_hal();
    gpio_hal->isr_handler();

    xSemaphoreGiveFromISR(xSWSemaphore, &xHigherPriorityTaskWoken);
}
#endif

#if configUSE_TICKLESS_IDLE == 2

#ifndef BOARD_TIMER_IRQ_HANDLER
#error "FreeRTOS Tickless Please define BOARD_TIMER_IRQ_HANDLER in app.h"
#endif
/*!
 * @brief Interrupt service fuction of timer.
 *
 * This function to call low power timer ISR
 */
void BOARD_TIMER_IRQ_HANDLER(void)
{
    vPortTimerISRHandler();
}

#endif /* configUSE_TICKLESS_IDLE */
