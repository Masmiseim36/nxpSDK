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
#include "nghttp2client.h"
#include "board.h"
#include "ksdk_mbedtls.h"
#include "wwd.h"
#include "wwd_wiced.h"

#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AP_SSID "nxp"
#define AP_PASS "NXP0123456789"
#define AP_SEC WICED_SECURITY_WPA2_MIXED_PSK

#define APP_VERSION_MAJOR "0"
#define APP_VERSION_MINOR "4"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void test_join(void);
extern wwd_result_t test_scan();
extern wiced_result_t wiced_wlan_connectivity_init(void);
extern void add_wlan_interface(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BOARD_InitNetwork()
{
    wwd_result_t err               = WWD_SUCCESS;
    bool wifi_platform_initialized = false;
    bool wifi_station_mode         = true;

    wiced_ssid_t ap_ssid = {
        .length = sizeof(AP_SSID) - 1,
        .value  = AP_SSID,
    };

    PRINTF(
        "****************************************************\r\n"
        "** WICED WiFi Card Demo Application\r\n"
        "** Version: " APP_VERSION_MAJOR "." APP_VERSION_MINOR
        "\r\n"
        "****************************************************\r\n");

    // Main function to initialize wifi platform
    PRINTF("Initializing WiFi Connection... \r\n");
    err = wiced_wlan_connectivity_init();
    if (err != WWD_SUCCESS)
    {
        PRINTF("Could not initialize wifi connection \n");
        vTaskSuspend(NULL);
    }
    else
    {
        wifi_platform_initialized = true;
    }

    if (wifi_platform_initialized)
    {
        PRINTF("Successfully Initialized WiFi Connection \r\n");

        if (wifi_station_mode)
        {
            PRINTF("Joining : " AP_SSID "\n");
            (void)host_rtos_delay_milliseconds((uint32_t)1000);
            err = wwd_wifi_join(&ap_ssid, AP_SEC, (uint8_t *)AP_PASS, sizeof(AP_PASS) - 1, NULL, WWD_STA_INTERFACE);
            if (err != WWD_SUCCESS)
            {
                PRINTF("Failed to join  : " AP_SSID " \n");
                vTaskSuspend(NULL);
            }
            else
            {
                PRINTF("Successfully joined : " AP_SSID "\n");
                (void)host_rtos_delay_milliseconds((uint32_t)1000);
                add_wlan_interface();
            }
        }
    }
    else
    {
        vTaskSuspend(NULL);
    }
}

/*!
 * @brief The main function containing client thread.
 */
static void httpsclient_task(void *arg)
{
    BOARD_InitNetwork();
    nghttp2_client();

#if defined(INCLUDE_uxTaskGetStackHighWaterMark) && INCLUDE_uxTaskGetStackHighWaterMark
    int32_t freeStack = uxTaskGetStackHighWaterMark(NULL);
    PRINTF("Stack high water mark: %d\n", freeStack);
#endif

#if defined(configTOTAL_HEAP_SIZE)
    int32_t freeHeap = xPortGetMinimumEverFreeHeapSize();
    PRINTF("Minimum ever free heap size: %d\n", freeHeap);
#endif

    vTaskDelete(NULL);
}

/*!
 * @brief Main function.
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Make sure casper ram buffer has power up */
    POWER_DisablePD(kPDRUNCFG_APD_CASPER_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_CASPER_SRAM);

    /*Make sure USDHC ram buffer has power up*/
    POWER_DisablePD(kPDRUNCFG_APD_USDHC0_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_USDHC0_SRAM);
    POWER_ApplyPD();

    /* SDIO0 */
    RESET_ClearPeripheralReset(kSDIO0_RST_SHIFT_RSTn);
    CLOCK_AttachClk(kAUX0_PLL_to_SDIO0_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivSdio0Clk, 1);
    CRYPTO_InitHardware();

    tcpip_init(NULL, NULL);

    if (xTaskCreate(httpsclient_task, "httpsclient_task", 1400, NULL, configMAX_PRIORITIES - 4 /*3*/, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    /* Run RTOS */
    vTaskStartScheduler();

    /* Should not reach this statement */
    for (;;)
        ;
}
