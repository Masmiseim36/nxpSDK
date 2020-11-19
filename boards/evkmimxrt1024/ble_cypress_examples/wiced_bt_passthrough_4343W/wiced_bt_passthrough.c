/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_lpuart.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_DEBUG_CONSOLE_INSTANCE 2U

/*
 * Properties of the external UART used by a user for HCI commands sent from a PC.
 * Debug console is redirected to another UART in hardware_init.c.
 */
#define EXTERNAL_UART             ((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)
#define EXTERNAL_UART_BAUDRATE    BOARD_DEBUG_UART_BAUDRATE
#define EXTERNAL_UART_CLK_FREQ    BOARD_DEBUG_UART_CLK_FREQ
#define EXTERNAL_UART_IRQ         BOARD_UART_IRQ
#define EXTERNAL_UART_IRQ_HANDLER BOARD_UART_IRQ_HANDLER

/*
 * Properties of the UART which is an internal HCI interface to a Bluetooth chip.
 */
#define INTERNAL_UART             BOARD_BT_UART_BASEADDR
#define INTERNAL_UART_BAUDRATE    BOARD_DEBUG_UART_BAUDRATE
#define INTERNAL_UART_CLK_FREQ    BOARD_BT_UART_CLK_FREQ
#define INTERNAL_UART_IRQ         BOARD_BT_UART_IRQ
#define INTERNAL_UART_IRQ_HANDLER BOARD_BT_UART_IRQ_HANDLER

#define UART_IRQ_PRIO        (configMAX_PRIORITIES - 1)
#define PASS_TASK_PRIO       (UART_IRQ_PRIO - 1)
#define PASS_TASK_STACK_SIZE 1024U

#define QUEUE_SIZE 512U

#define DEBUG_PASS_THROUGH 0

#if DEBUG_PASS_THROUGH

#if configUSE_TIME_SLICING != 0
#warning "The option configUSE_TIME_SLICING is enabled, LOG_DATA output from different tasks may be mixed."
#endif /* configUSE_TIME_SLICING != 0 */

#define LOG_DATA(format, log_id, data) \
    do                                 \
    {                                  \
        PRINTF(format, log_id, data);  \
    } while (0)

#else

#define LOG_DATA(format, log_id, data)

#endif /* DEBUG_PASS_THROUGH */

/*! @brief Argument of pass_task */
typedef struct
{
    QueueHandle_t input_queue;
    LPUART_Type *output_uart;
    char log_id;
} pass_task_args_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Queue of bytes received from internal UART to be forwarded to external UART */
static QueueHandle_t external_uart_tx_queue;

/*! @brief Queue of bytes received from external UART to be forwarded to internal UART */
static QueueHandle_t internal_uart_tx_queue;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Outputs data from queue into UART.
 */
static void pass_task(void *arg)
{
    QueueHandle_t input_queue = ((pass_task_args_t *)arg)->input_queue;
    LPUART_Type *output_uart  = ((pass_task_args_t *)arg)->output_uart;
#if DEBUG_PASS_THROUGH
    char log_id = ((pass_task_args_t *)arg)->log_id;
#endif /* DEBUG_PASS_THROUGH */
    uint8_t data;

    while (true)
    {
        if (uxQueueSpacesAvailable(input_queue) != QUEUE_SIZE)
        {
            if (xQueueReceive(input_queue, &data, (TickType_t)10) == pdTRUE)
            {
                LOG_DATA("%c: %02x\r\n", log_id, data);
                LPUART_WriteBlocking(output_uart, &data, 1U);
            }
        }
        taskYIELD();
    }
}

/*!
 * @brief Reads a data received from UART and puts it into a queue for subsequent forwarding.
 */
static inline void uart_irq_handler(LPUART_Type *uart, QueueHandle_t queue)
{
    uint8_t data;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* If new data arrived */
    if (LPUART_GetStatusFlags(uart) & kLPUART_RxDataRegFullFlag)
    {
        data = LPUART_ReadByte(uart);
        xQueueSendFromISR(queue, (void *)&data, &xHigherPriorityTaskWoken);

        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR(0);
        }
    }
}

/*!
 * @brief External UART IRQ handler.
 */
void EXTERNAL_UART_IRQ_HANDLER(void)
{
    uart_irq_handler(EXTERNAL_UART, internal_uart_tx_queue);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Internal UART IRQ handler.
 */
void INTERNAL_UART_IRQ_HANDLER(void)
{
    uart_irq_handler(INTERNAL_UART, external_uart_tx_queue);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Initializes external UART.
 */
static void external_uart_init(void)
{
    lpuart_config_t config;
    status_t status;

    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = EXTERNAL_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    /* RTS/CTS not connected on a board */
    config.enableRxRTS = false;
    config.enableTxCTS = false;

    status = LPUART_Init(EXTERNAL_UART, &config, EXTERNAL_UART_CLK_FREQ);
    if (status != kStatus_Success)
    {
        PRINTF("Failed to initialize external UART!\r\n");
        while (true)
        {
        }
    }

    /* Enable RX interrupt */
    LPUART_EnableInterrupts(EXTERNAL_UART, kLPUART_RxDataRegFullInterruptEnable);
    EnableIRQ(EXTERNAL_UART_IRQ);

    /* Change priority of UART1 IRQ */
    NVIC_SetPriority((IRQn_Type)EXTERNAL_UART_IRQ, UART_IRQ_PRIO);
}

/*!
 * @brief Initializes internal Bluetooth HCI UART.
 */
static void internal_uart_init(void)
{
    lpuart_config_t config;
    status_t status;

    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = INTERNAL_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;
    config.enableRxRTS  = true;
    config.enableTxCTS  = true;
    config.txCtsConfig  = kLPUART_CtsSampleAtStart;
    config.txCtsSource  = kLPUART_CtsSourcePin;

    status = LPUART_Init(INTERNAL_UART, &config, INTERNAL_UART_CLK_FREQ);
    if (status != kStatus_Success)
    {
        PRINTF("Failed to initialize internal Bluetooth HCI UART!\r\n");
        while (true)
        {
        }
    }

    /* Enable RX interrupt */
    LPUART_EnableInterrupts(INTERNAL_UART, kLPUART_RxDataRegFullInterruptEnable);
    EnableIRQ(INTERNAL_UART_IRQ);

    /* Change priority of BT UART IRQ */
    NVIC_SetPriority((IRQn_Type)INTERNAL_UART_IRQ, UART_IRQ_PRIO);
}

/*!
 * @brief Main function.
 */
int main(void)
{
    static pass_task_args_t pass_to_internal_args;
    static pass_task_args_t pass_to_external_args;

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    uint32_t uartClkSrcFreq = BOARD_DebugConsoleSrcFreq();
    DbgConsole_Init(BOARD_DEBUG_CONSOLE_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" Bluetooth HCI Pass-Through example\r\n");
    PRINTF("************************************************\r\n");

    /* Turn on bluetooth */
    GPIO_PinWrite(BOARD_INITPINS_BT_REG_ON_GPIO, BOARD_INITPINS_BT_REG_ON_PIN, 1U);

    /* Queue creation */
    external_uart_tx_queue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));
    internal_uart_tx_queue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));

    /* Queue Creation failed? */
    assert(external_uart_tx_queue);
    assert(internal_uart_tx_queue);

    /* Initialize UART instances */
    external_uart_init();
    internal_uart_init();

    /* Create a task to pass data received from external UART to internal HCI UART */
    pass_to_internal_args.input_queue = internal_uart_tx_queue;
    pass_to_internal_args.output_uart = INTERNAL_UART;
    pass_to_internal_args.log_id      = 'w';
    if (xTaskCreate(pass_task, "pass_to_internal_task", PASS_TASK_STACK_SIZE / sizeof(StackType_t),
                    &pass_to_internal_args, PASS_TASK_PRIO, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!\r\n");
        while (true)
        {
        }
    }

    /* Create a task to pass data received from internal HCI UART to external UART */
    pass_to_external_args.input_queue = external_uart_tx_queue;
    pass_to_external_args.output_uart = EXTERNAL_UART;
    pass_to_external_args.log_id      = 'r';
    if (xTaskCreate(pass_task, "pass_to_external_task", PASS_TASK_STACK_SIZE / sizeof(StackType_t),
                    &pass_to_external_args, PASS_TASK_PRIO, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!\r\n");
        while (true)
        {
        }
    }

    /* Run RTOS */
    vTaskStartScheduler();

    /* Should not reach this statement */
    while (true)
    {
    }
}
