/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define TASK_PRIO (configMAX_PRIORITIES - 1)

SemaphoreHandle_t xSemaphore_producer;
SemaphoreHandle_t xSemaphore_consumer;

/* Static memory size definitions */
#define PRODUCER_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 166)
#define CONSUMER_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 166)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void producer_task(void *pvParameters);
static void consumer_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Statically allocated memory for producer_task stack */
static StackType_t ProducerTaskStack[PRODUCER_TASK_STACK_SIZE];
/* Statically allocated memory for producer_task task control block */
static StaticTask_t ProducerTaskTCB;
/* Statically allocated memory for consumer task stacks */
static StackType_t ConsumerTaskStack0[PRODUCER_TASK_STACK_SIZE];
static StackType_t ConsumerTaskStack1[PRODUCER_TASK_STACK_SIZE];
static StackType_t ConsumerTaskStack2[PRODUCER_TASK_STACK_SIZE];
/* Statically allocated memory for consumer_task task control block */
static StaticTask_t ConsumerTaskTCB0;
static StaticTask_t ConsumerTaskTCB1;
static StaticTask_t ConsumerTaskTCB2;
/* Statically allocated memory for producer semaphore structure */
static StaticSemaphore_t ProducerSemaphoreBuffer;
/* Statically allocated memory for consumer semaphore structure */
static StaticSemaphore_t ConsumerSemaphoreBuffer;

TaskHandle_t TaskHandle = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    TaskHandle = xTaskCreateStatic(producer_task, "PRODUCER_TASK", PRODUCER_TASK_STACK_SIZE, NULL, TASK_PRIO,
                                   &(ProducerTaskStack[0]), &ProducerTaskTCB);
    if (TaskHandle != NULL)
    {
        PRINTF("Producer_task created.\r\n");
    }
    else
    {
        PRINTF("Failed to create producer task");
        while (1)
            ;
    }
    /* Start scheduling. */
    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Task producer_task.
 */
static void producer_task(void *pvParameters)
{
    xSemaphore_producer = xSemaphoreCreateBinaryStatic(&ProducerSemaphoreBuffer);
    if (xSemaphore_producer == NULL)
    {
        PRINTF("xSemaphore_producer creation failed.\r\n");
        vTaskSuspend(NULL);
    }

    xSemaphore_consumer = xSemaphoreCreateBinaryStatic(&ConsumerSemaphoreBuffer);
    if (xSemaphore_consumer == NULL)
    {
        PRINTF("xSemaphore_consumer creation failed.\r\n");
        vTaskSuspend(NULL);
    }

    TaskHandle = xTaskCreateStatic(consumer_task, "CONSUMER_TASK_0", CONSUMER_TASK_STACK_SIZE, (void *)0, TASK_PRIO,
                                   &(ConsumerTaskStack0[0]), &ConsumerTaskTCB0);
    if (TaskHandle != NULL)
    {
        PRINTF("Consumer_task 0 created.\r\n");
    }
    else
    {
        PRINTF("Failed to create consumer_task 0.\r\n");
        vTaskSuspend(NULL);
    }

    TaskHandle = xTaskCreateStatic(consumer_task, "CONSUMER_TASK_1", CONSUMER_TASK_STACK_SIZE, (void *)1, TASK_PRIO,
                                   &(ConsumerTaskStack1[0]), &ConsumerTaskTCB1);
    if (TaskHandle != NULL)
    {
        PRINTF("Consumer_task 1 created.\r\n");
    }
    else
    {
        PRINTF("Failed to create consumer_task 1.\r\n");
        vTaskSuspend(NULL);
    }

    TaskHandle = xTaskCreateStatic(consumer_task, "CONSUMER_TASK_2", CONSUMER_TASK_STACK_SIZE, (void *)2, TASK_PRIO,
                                   &(ConsumerTaskStack2[0]), &ConsumerTaskTCB2);
    if (TaskHandle != NULL)
    {
        PRINTF("Consumer_task 2 created.\r\n");
    }
    else
    {
        PRINTF("Failed to create consumer_task 2.\r\n");
        vTaskSuspend(NULL);
    }

    while (1)
    {
        /* Producer is ready to provide item. */
        xSemaphoreGive(xSemaphore_consumer);
        /* Producer is waiting when consumer will be ready to accept item. */
        if (xSemaphoreTake(xSemaphore_producer, portMAX_DELAY) == pdTRUE)
        {
            PRINTF("Producer released item.\r\n");
        }
        else
        {
            PRINTF("Producer is waiting for customer.\r\n");
        }
    }
}

/*!
 * @brief Task consumer_task.
 */
static void consumer_task(void *pvParameters)
{
    PRINTF("Consumer number: %d\r\n", pvParameters);
    while (1)
    {
        /* Consumer is ready to accept. */
        xSemaphoreGive(xSemaphore_producer);
        /* Consumer is waiting when producer will be ready to produce item. */
        if (xSemaphoreTake(xSemaphore_consumer, portMAX_DELAY) == pdTRUE)
        {
            PRINTF("Consumer %d accepted item.\r\n", pvParameters);
        }
        else
        {
            PRINTF("Consumer %d is waiting for producer.\r\n", pvParameters);
        }
    }
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
