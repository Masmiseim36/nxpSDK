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

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_lpsci_freertos.h"
#include "fsl_lpsci.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPSCI UART0
#define DEMO_LPSCI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_LPSCI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
/* Task priorities. */
#define lpsci_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void lpsci_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
const char *to_send = "FreeRTOS LPSCI driver example!\r\n";
const char *send_ring_overrun = "\r\nRing buffer overrun!\r\n";
const char *send_hardware_overrun = "\r\nHardware buffer overrun!\r\n";
uint8_t background_buffer[32];
uint8_t recv_buffer[4];

lpsci_rtos_handle_t handle;
struct _lpsci_handle t_handle;

lpsci_rtos_config_t lpsci_config = {
    .baudrate = 115200,
    .parity = kLPSCI_ParityDisabled,
    .stopbits = kLPSCI_OneStopBit,
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
    CLOCK_SetLpsci0Clock(0x1U);

    xTaskCreate(lpsci_task, "Uart_task", configMINIMAL_STACK_SIZE, NULL, lpsci_task_PRIORITY, NULL);

    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void lpsci_task(void *pvParameters)
{
    int error;
    size_t n;

    lpsci_config.srcclk = DEMO_LPSCI_CLK_FREQ;
    lpsci_config.base = DEMO_LPSCI;

    if (0 > LPSCI_RTOS_Init(&handle, &t_handle, &lpsci_config))
    {
        vTaskSuspend(NULL);
    }

    /* Send some data */
    if (0 > LPSCI_RTOS_Send(&handle, (uint8_t *)to_send, strlen(to_send)))
    {
        vTaskSuspend(NULL);
    }

    /* Send data */
    do
    {
        error = LPSCI_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
        if (error == kStatus_LPSCI_RxHardwareOverrun)
        {
            /* Notify about hardware buffer overrun */
            if (kStatus_Success !=
                LPSCI_RTOS_Send(&handle, (uint8_t *)send_hardware_overrun, strlen(send_hardware_overrun)))
            {
                vTaskSuspend(NULL);
            }
        }
        if (error == kStatus_LPSCI_RxRingBufferOverrun)
        {
            /* Notify about ring buffer overrun */
            if (kStatus_Success != LPSCI_RTOS_Send(&handle, (uint8_t *)send_ring_overrun, strlen(send_ring_overrun)))
            {
                vTaskSuspend(NULL);
            }
        }

        if (n > 0)
        {
            /* send back the received data */
            LPSCI_RTOS_Send(&handle, (uint8_t *)recv_buffer, n);
        }
    } while (kStatus_Success == error);

    LPSCI_RTOS_Deinit(&handle);

    vTaskSuspend(NULL);
}
