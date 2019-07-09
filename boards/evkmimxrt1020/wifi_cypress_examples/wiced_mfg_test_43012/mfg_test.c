/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_sdio.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "wwd.h"
#include "wwd_wiced.h"
#include "lwip/tcpip.h"

#include "mfg_test.h"

#include "fsl_lpuart_freertos.h"
#include "serial_manager.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SERIAL_PORT_NVIC_PRIO 5

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 2048
#define BOARD_DEBUG_CONSOLE_INSTANCE 2U
#define DEMO_SERIAL_PORT LPUART1
#define DEMO_SERIAL_PORT_CLK_FREQ BOARD_DebugConsoleSrcFreq()
#define DEMO_SERIAL_PORT_IRQn BOARD_UART_IRQ
/*! @brief Task stack size. */
#define WIFICARD_TASK_STACK_SIZE (6144 / sizeof(portSTACK_TYPE))
/*! @brief Task stack priority. */
#define WIFICARD_TASK_PRIORITY (configMAX_PRIORITIES - 1U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
int serial_port_init(void);
extern void test_join(void);
static void APP_WifiTask(void *parameter);
extern wwd_result_t test_scan();
extern wiced_result_t wiced_wlan_connectivity_init(void);
extern void add_wlan_interface(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t background_buffer[DEMO_RING_BUFFER_SIZE];

lpuart_rtos_handle_t handle;
struct _lpuart_handle t_handle;

lpuart_rtos_config_t lpuart_config = {
    .baudrate    = 115200,
    .parity      = kLPUART_ParityDisabled,
    .stopbits    = kLPUART_OneStopBit,
    .buffer      = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

/*******************************************************************************
 * Code
 ******************************************************************************/
int rwl_read_serial_port(void *hndle, char *read_buf, unsigned int data_size, unsigned int *numread)
{
    if (data_size == 0)
    {
        return 1;
    }
    if ((read_buf == NULL) || (numread == NULL))
    {
        return -3;
    }

    int error;
    size_t n = 0;
    error    = LPUART_RTOS_Receive(&handle, (uint8_t *)read_buf, data_size, &n);
    if (error == kStatus_Success)
    {
        *numread = n;
        return 1;
    }
    else
    {
        return -3;
    }
}

int rwl_write_serial_port(void *hndle, char *write_buf, unsigned long size, unsigned long *numwritten)
{
    if ((write_buf == NULL) || (size == 0) || (numwritten == NULL))
    {
        return -3;
    }

    int error;
    error = LPUART_RTOS_Send(&handle, (uint8_t *)write_buf, size);
    if (error == kStatus_Success)
    {
        *numwritten = size;
        return 1;
    }
    else
    {
        return -3;
    }
}

int serial_port_init(void)
{
    lpuart_config.srcclk = DEMO_SERIAL_PORT_CLK_FREQ;
    lpuart_config.base   = DEMO_SERIAL_PORT;

    NVIC_SetPriority(DEMO_SERIAL_PORT_IRQn, SERIAL_PORT_NVIC_PRIO);

    if (0 > LPUART_RTOS_Init(&handle, &t_handle, &lpuart_config))
    {
        return -1;
    }

    return 0;
}
static void BOARD_USDHCClockConfiguration(void)
{
    CLOCK_InitSysPll(&sysPllConfig_BOARD_BootClockRUN);
    /*configure system pll PFD0 fractional divider to 24, output clock is 528MHZ * 18 / 24 = 396 MHZ*/
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 24U);
    /* Configure USDHC clock source and divider */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 1U);
}


static void APP_WifiTask(void *parameter)
{
    wwd_result_t err = WWD_SUCCESS;

    /* Main function to initialize wifi platform */
    PRINTF("Initializing WiFi Connection... \r\n");
    err = wiced_wlan_connectivity_init();
    if (err != WWD_SUCCESS)
    {
        PRINTF("Could not initialize wifi connection \n");
    }
    else
    {
        PRINTF("Successfully Initialized WiFi Connection \r\n");

        if (serial_port_init() != 0)
        {
            PRINTF("serial_port_init() failed\r\n");
            vTaskSuspend(NULL);
        }

        PRINTF("Starting WL server \r\n");
        int ret;
        ret = remote_server_exec(0, NULL, NULL);

        PRINTF("remote_server_exec() returns: %d\r\n", ret);
    }

    vTaskSuspend(NULL);
}

/*@brief Main function */
int main(void)
{
    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_USDHCClockConfiguration();

    uint32_t uartClkSrcFreq = BOARD_DebugConsoleSrcFreq();
    DbgConsole_Init(BOARD_DEBUG_CONSOLE_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);

    SCB_DisableDCache();

    tcpip_init(NULL, NULL);

    if (pdPASS !=
        xTaskCreate(APP_WifiTask, "APP_WifiTask", WIFICARD_TASK_STACK_SIZE, NULL, WIFICARD_TASK_PRIORITY, NULL))
    {
        return -1;
    }

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    /* Scheduler should never reach this point. */
    while (true)
    {
    }
}
