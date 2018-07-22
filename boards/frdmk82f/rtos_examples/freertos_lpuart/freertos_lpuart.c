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

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_lpuart_freertos.h"
#include "fsl_lpuart.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPUART LPUART4
#define DEMO_LPUART_CLKSRC kCLOCK_Osc0ErClk
#define DEMO_LPUART_RX_TX_IRQn LPUART4_IRQn
/* Task priorities. */
#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void uart_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
const char *to_send = "Hello, world!\r\n";
uint8_t background_buffer[32];
uint8_t recv_buffer[4];

lpuart_rtos_handle_t handle;
struct _lpuart_handle t_handle;

struct rtos_lpuart_config lpuart_config = {
    .baudrate = 115200,
    .parity = kLPUART_ParityDisabled,
    .stopbits = kLPUART_OneStopBit,
    .buffer = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_SetLpuartClock(0x2U);
    NVIC_SetPriority(DEMO_LPUART_RX_TX_IRQn, 5);

    xTaskCreate(uart_task, "Uart_task", configMINIMAL_STACK_SIZE, NULL, uart_task_PRIORITY, NULL);

    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void uart_task(void *pvParameters)
{
    int error;
    size_t n;

    lpuart_config.srcclk = CLOCK_GetFreq(DEMO_LPUART_CLKSRC);
    lpuart_config.base = DEMO_LPUART;

    // PRINTF("Test");

    if (0 > LPUART_RTOS_Init(&handle, &t_handle, &lpuart_config))
    {
        PRINTF("Error during UART initialization.\r\n");
        vTaskSuspend(NULL);
    }

    /* Send some data */
    if (0 > LPUART_RTOS_Send(&handle, (uint8_t *)to_send, strlen(to_send)))
    {
        PRINTF("Error during UART send.\r\n");
        vTaskSuspend(NULL);
    }

    /* Send data */
    do
    {
        error = LPUART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
        if (n > 0)
        {
            /* send back the received data */
            LPUART_RTOS_Send(&handle, (uint8_t *)recv_buffer, n);
        }
    } while (kStatus_Success == error);

    LPUART_RTOS_Deinit(&handle);

    vTaskSuspend(NULL);
}
