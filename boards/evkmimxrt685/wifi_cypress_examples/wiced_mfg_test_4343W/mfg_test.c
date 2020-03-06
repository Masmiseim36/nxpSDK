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

#include "fsl_usart_freertos.h"
#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SERIAL_PORT_NVIC_PRIO 5

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 2048
#define BOARD_DEBUG_CONSOLE_INSTANCE 4U
#define DEMO_SERIAL_PORT USART0
#define DEMO_SERIAL_PORT_CLK_SRC kCLOCK_Flexcomm0
#define DEMO_SERIAL_PORT_CLK_FREQ CLOCK_GetFlexCommClkFreq(0U)
#define DEMO_SERIAL_PORT_IRQn FLEXCOMM0_IRQn
/*! @brief Task stack size. */
#define WIFICARD_TASK_STACK_SIZE (6144 / sizeof(portSTACK_TYPE))
/*! @brief Task stack priority. */
#define WIFICARD_TASK_PRIORITY (configMAX_PRIORITIES - 1U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
int serial_port_init();
extern void test_join(void);
static void APP_WifiTask(void *parameter);
extern wwd_result_t test_scan();
extern wiced_result_t wiced_wlan_connectivity_init(void);
extern void add_wlan_interface(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t background_buffer[DEMO_RING_BUFFER_SIZE];

usart_rtos_handle_t handle;
struct _usart_handle t_handle;

struct rtos_usart_config usart_config = {
    .baudrate    = 115200,
    .parity      = kUSART_ParityDisabled,
    .stopbits    = kUSART_OneStopBit,
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
    error    = USART_RTOS_Receive(&handle, (uint8_t *)read_buf, data_size, &n);
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
    error = USART_RTOS_Send(&handle, (uint8_t *)write_buf, size);
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

int serial_port_init()
{
    usart_config.srcclk = DEMO_SERIAL_PORT_CLK_FREQ;
    usart_config.base   = DEMO_SERIAL_PORT;

    NVIC_SetPriority(DEMO_SERIAL_PORT_IRQn, SERIAL_PORT_NVIC_PRIO);

    if (0 > USART_RTOS_Init(&handle, &t_handle, &usart_config))
    {
        return -1;
    }

    return 0;
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
    BOARD_InitPins();
    BOARD_BootClockRUN();

    /* FLEXCOMM4 - DebugConsole */
    RESET_ClearPeripheralReset(kFC4_RST_SHIFT_RSTn);
    /* attach FRG0 clock to FLEXCOMM4 */
    const clock_frg_clk_config_t clk_cfg = {4, kCLOCK_FrgPllDiv, 255U, 0}; /*!< Select FRG0 mux as frg_pll */
    CLOCK_SetFRGClock(&clk_cfg);
    CLOCK_AttachClk(kFRG_to_FLEXCOMM4);
    DbgConsole_Init(BOARD_DEBUG_CONSOLE_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    CLOCK_GetFlexCommClkFreq(BOARD_DEBUG_CONSOLE_INSTANCE));

    /*Make sure USDHC ram buffer has power up*/
    POWER_DisablePD(kPDRUNCFG_APD_USDHC0_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_USDHC0_SRAM);
    POWER_ApplyPD();

    /* SDIO0 */
    RESET_ClearPeripheralReset(kSDIO0_RST_SHIFT_RSTn);
    CLOCK_AttachClk(kAUX0_PLL_to_SDIO0_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivSdio0Clk, 1);

    /* FLEXCOMM0 */
    RESET_ClearPeripheralReset(kFC0_RST_SHIFT_RSTn);
    /* attach FRG0 clock to FLEXCOMM0 */
    const clock_frg_clk_config_t clk_cfg2 = {0, kCLOCK_FrgPllDiv, 255U, 0}; /*!< Select FRG0 mux as frg_pll */
    CLOCK_SetFRGClock(&clk_cfg2);
    CLOCK_AttachClk(kFRG_to_FLEXCOMM0);

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
