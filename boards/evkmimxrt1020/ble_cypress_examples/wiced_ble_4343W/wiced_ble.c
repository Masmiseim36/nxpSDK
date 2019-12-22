/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lwip/tcpip.h"
#include "board.h"
#include "wwd.h"
#include "wwd_wiced.h"
#include "platform_imxrt.h"
#include "fsl_debug_console.h"
#include "ble_hello_sensor.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AP_SSID "nxp"
#define AP_PASS "NXP0123456789"
#define AP_SEC WICED_SECURITY_WPA2_MIXED_PSK

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern wwd_result_t test_scan();
extern wiced_result_t wiced_wlan_connectivity_init(void);
extern void add_wlan_interface(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_USDHCClockConfiguration(void)
{
    CLOCK_InitSysPll(&sysPllConfig_BOARD_BootClockRUN);
    /*configure system pll PFD0 fractional divider to 24, output clock is 528MHZ * 18 / 24 = 396 MHZ*/
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 24U);
    /* Configure USDHC clock source and divider */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 1U);
}


static void BOARD_InitNetwork()
{
    wwd_result_t err         = WWD_SUCCESS;
    wiced_result_t wiced_err = WICED_SUCCESS;

    wiced_ssid_t ap_ssid = {
        .length = sizeof(AP_SSID) - 1,
        .value  = AP_SSID,
    };

    PRINTF("Initializing WiFi Connection... \r\n");
    wiced_err = wiced_wlan_connectivity_init();
    if (wiced_err != WICED_SUCCESS)
    {
        PRINTF("Could not initialize wifi connection \r\n");
        vTaskSuspend(NULL);
        return;
    }

    PRINTF("Successfully Initialized WiFi Connection \r\n");

    err = test_scan();
    if (err != WWD_SUCCESS)
    {
        PRINTF(" Scan Error \r\n");
    }

    PRINTF("Joining : " AP_SSID "\r\n");
    (void)host_rtos_delay_milliseconds((uint32_t)1000);
    err = wwd_wifi_join(&ap_ssid, AP_SEC, (uint8_t *)AP_PASS, sizeof(AP_PASS) - 1, NULL, WWD_STA_INTERFACE);
    if (err != WWD_SUCCESS)
    {
        PRINTF("Failed to join  : " AP_SSID " \r\n");
    }
    else
    {
        PRINTF("Successfully joined : " AP_SSID "\r\n");
        (void)host_rtos_delay_milliseconds((uint32_t)1000);
        add_wlan_interface();

        /* Start BLE */
        PRINTF("\r\nInitializing BLE... \r\n");
        hello_sensor_start();
    }
}

/*!
 * @brief Initializes wifi and BLE.
 */
static void wifi_ble_init(void *arg)
{
    BOARD_InitNetwork();

#if defined(INCLUDE_uxTaskGetStackHighWaterMark) && INCLUDE_uxTaskGetStackHighWaterMark
    int32_t freeStack = uxTaskGetStackHighWaterMark(NULL);
    PRINTF("Stack high water mark: %d\r\n", freeStack);
#endif

#if defined(configTOTAL_HEAP_SIZE)
    int32_t freeHeap = xPortGetMinimumEverFreeHeapSize();
    PRINTF("Minimum ever free heap size: %d\r\n", freeHeap);
#endif

    vTaskDelete(NULL);
}

static int32_t nvm_save(uint32_t addr, uint8_t *data, uint32_t length)
{
    assert(0); /* Not implemented. */
    return 0;
}

static int32_t nvm_read(uint32_t addr, uint8_t *data, uint32_t length)
{
    assert(0); /* Not implemented. */
    return 0;
}

static int32_t nvm_erase(uint32_t addr)
{
    assert(0); /* Not implemented. */
    return 0;
}

/*!
 * @brief Main function.
 */
int main(void)
{
    platform_nvm_t nvm_cfg;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_USDHCClockConfiguration();
    BOARD_InitDebugConsole();

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" Wi-Fi + BLE example\r\n");
    PRINTF("************************************************\r\n");

    tcpip_init(NULL, NULL);

    nvm_cfg.platform_nvm_save  = nvm_save;
    nvm_cfg.platform_nvm_read  = nvm_read;
    nvm_cfg.platform_nvm_erase = nvm_erase;
    platform_set_nvm(&nvm_cfg);

    if (xTaskCreate(wifi_ble_init, "wifi_ble_init", 1000, NULL, 1, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!\r\n");
        while (1)
            ;
    }

    /* Run RTOS */
    vTaskStartScheduler();

    /* Should not reach this statement */
    for (;;)
        ;
}
