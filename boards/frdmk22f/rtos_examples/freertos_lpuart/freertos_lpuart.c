/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_lpuart_freertos.h"
#include "fsl_lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPUART            LPUART0
#define DEMO_LPUART_CLKSRC     kCLOCK_Osc0ErClk
#define DEMO_LPUART_CLK_FREQ   CLOCK_GetFreq(kCLOCK_Osc0ErClk)
#define DEMO_LPUART_RX_TX_IRQn LPUART0_IRQn
/* Task priorities. */
#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void uart_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
char *to_send               = "FreeRTOS LPUART driver example!\r\n";
char *send_ring_overrun     = "\r\nRing buffer overrun!\r\n";
char *send_hardware_overrun = "\r\nHardware buffer overrun!\r\n";
uint8_t background_buffer[32];
uint8_t recv_buffer[4];

lpuart_rtos_handle_t handle;
struct _lpuart_handle t_handle;

lpuart_rtos_config_t lpuart_config = {
    .baudrate    = 115200,
    .parity      = kLPUART_ParityDisabled,
    .stopbits    = kLPUART_OneStopBit,
    .buffer      = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_LPUARTSRC_MASK) | SIM_SOPT2_LPUARTSRC(0x2U));
    NVIC_SetPriority(DEMO_LPUART_RX_TX_IRQn, 5);
    if (xTaskCreate(uart_task, "Uart_task", configMINIMAL_STACK_SIZE + 100, NULL, uart_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Task responsible for loopback.
 */
static void uart_task(void *pvParameters)
{
    int error;
    size_t n = 0;

    lpuart_config.srcclk = DEMO_LPUART_CLK_FREQ;
    lpuart_config.base   = DEMO_LPUART;

    if (kStatus_Success != LPUART_RTOS_Init(&handle, &t_handle, &lpuart_config))
    {
        vTaskSuspend(NULL);
    }

    /* Send introduction message. */
    if (kStatus_Success != LPUART_RTOS_Send(&handle, (uint8_t *)to_send, strlen(to_send)))
    {
        vTaskSuspend(NULL);
    }

    /* Receive user input and send it back to terminal. */
    do
    {
        error = LPUART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
        if (error == kStatus_LPUART_RxHardwareOverrun)
        {
            /* Notify about hardware buffer overrun */
            if (kStatus_Success !=
                LPUART_RTOS_Send(&handle, (uint8_t *)send_hardware_overrun, strlen(send_hardware_overrun)))
            {
                vTaskSuspend(NULL);
            }
        }
        if (error == kStatus_LPUART_RxRingBufferOverrun)
        {
            /* Notify about ring buffer overrun */
            if (kStatus_Success != LPUART_RTOS_Send(&handle, (uint8_t *)send_ring_overrun, strlen(send_ring_overrun)))
            {
                vTaskSuspend(NULL);
            }
        }
        if (n > 0)
        {
            /* send back the received data */
            if (kStatus_Success != LPUART_RTOS_Send(&handle, recv_buffer, n))
            {
                vTaskSuspend(NULL);
            }
        }
    } while (kStatus_Success == error);

    LPUART_RTOS_Deinit(&handle);
    vTaskSuspend(NULL);
}
