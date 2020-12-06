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
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SERIAL_PORT_NVIC_PRIO 5

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 2048

#define DEMO_SERIAL_PORT          LPUART6
#define DEMO_SERIAL_PORT_CLK_FREQ BOARD_DebugConsoleSrcFreq()
#define DEMO_SERIAL_PORT_IRQn     BOARD_UART_IRQ
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

/* The function sets the cacheable memory to shareable, this suggestion is referred from chapter 2.2.1 Memory regions,
 * types and attributes in Cortex-M7 Devices, Generic User Guide */
void BOARD_ConfigUSBMPU()
{
    /* Disable I cache and D cache */
    SCB_DisableICache();
    SCB_DisableDCache();

    /* Disable MPU */
    ARM_MPU_Disable();
    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
     * SubRegionDisable, Size)
     * API in core_cm7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
     * disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
     * Privileged mode.
     *      Use MACROS defined in core_cm7.h:
     * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write
     * through no write allocate
     *     0             0           1           1              Normal             not shareable   Outer and inner write
     * back no write allocate
     *     0             1           1           0              Normal             shareable       Outer and inner write
     * through no write allocate
     *     0             1           1           1              Normal             shareable       Outer and inner write
     * back no write allocate
     *     1             0           0           0              Normal             not shareable   outer and inner
     * noncache
     *     1             1           0           0              Normal             shareable       outer and inner
     * noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write
     * back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write
     * back write/read acllocate
     *     2             x           0           0              Device              not shareable
     *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache
     * policy.
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in
     * core_cm7.h.
     */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 1, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}


void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    if (CONTROLLER_ID == kSerialManager_UsbControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, 480000000U);
    }
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
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
    BOARD_ConfigUSBMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

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
