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
#include "lwip/apps/lwiperf.h"
#include "board.h"
#include "wwd.h"
#include "wwd_wiced.h"

#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AP_SSID "nxp"
#define AP_PASS "NXP0123456789"
#define AP_SEC WICED_SECURITY_WPA2_MIXED_PSK
#define IPERF_SERVER_ADDRESS "192.168.2.101"

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
static void BOARD_USDHCClockConfiguration(void)
{
    /*configure system pll PFD2 fractional divider to 18*/
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 0x12U);
    /* Configure USDHC clock source and divider */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 1U);
}


static void BOARD_InitNetwork()
{
    wwd_result_t err = WWD_SUCCESS;
    bool wifi_platform_initialized = false;
    bool wifi_station_mode = true;

    wiced_ssid_t ap_ssid = {
        .length = sizeof(AP_SSID) - 1, .value = AP_SSID,
    };

    // Main function to initialize wifi platform
    PRINTF("Initializing WiFi Connection... \r\n");
    err = wiced_wlan_connectivity_init();
    if (err != WWD_SUCCESS)
    {
        PRINTF("Could not initialize wifi connection \n");
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
            err = test_scan();
            if (err != WWD_SUCCESS)
            {
                PRINTF(" Scan Error \n");
            }

            PRINTF("Joining : " AP_SSID "\n");
            (void)host_rtos_delay_milliseconds((uint32_t)1000);
            err = wwd_wifi_join(&ap_ssid, AP_SEC, (uint8_t *)AP_PASS, sizeof(AP_PASS) - 1, NULL, WWD_STA_INTERFACE);
            if (err != WWD_SUCCESS)
            {
                PRINTF("Failed to join  : " AP_SSID " \n");
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

/* Report state => string */
const char *report_type_str[] = {
    "TCP_DONE_SERVER (RX)",        /* LWIPERF_TCP_DONE_SERVER,*/
    "TCP_DONE_CLIENT (TX)",        /* LWIPERF_TCP_DONE_CLIENT,*/
    "TCP_ABORTED_LOCAL",           /* LWIPERF_TCP_ABORTED_LOCAL, */
    "TCP_ABORTED_LOCAL_DATAERROR", /* LWIPERF_TCP_ABORTED_LOCAL_DATAERROR, */
    "TCP_ABORTED_LOCAL_TXERROR",   /* LWIPERF_TCP_ABORTED_LOCAL_TXERROR, */
    "TCP_ABORTED_REMOTE",          /* LWIPERF_TCP_ABORTED_REMOTE, */
};

/** Prototype of a report function that is called when a session is finished.
    This report function shows the test results. */
static void lwiperf_report(void *arg,
                           enum lwiperf_report_type report_type,
                           const ip_addr_t *local_addr,
                           u16_t local_port,
                           const ip_addr_t *remote_addr,
                           u16_t remote_port,
                           u32_t bytes_transferred,
                           u32_t ms_duration,
                           u32_t bandwidth_kbitpsec)
{
    PRINTF("-------------------------------------------------\r\n");
    if ((report_type < (sizeof(report_type_str) / sizeof(report_type_str[0]))) && local_addr && remote_addr)
    {
        PRINTF(" %s \r\n", report_type_str[report_type]);
        PRINTF(" Local address : %u.%u.%u.%u ", ((u8_t *)local_addr)[0], ((u8_t *)local_addr)[1],
               ((u8_t *)local_addr)[2], ((u8_t *)local_addr)[3]);
        PRINTF(" Port %d \r\n", local_port);
        PRINTF(" Remote address : %u.%u.%u.%u ", ((u8_t *)remote_addr)[0], ((u8_t *)remote_addr)[1],
               ((u8_t *)remote_addr)[2], ((u8_t *)remote_addr)[3]);
        PRINTF(" Port %d \r\n", remote_port);
        PRINTF(" Bytes Transferred %d \r\n", bytes_transferred);
        PRINTF(" Duration (ms) %d \r\n", ms_duration);
        PRINTF(" Bandwidth (kbitpsec) %d \r\n", bandwidth_kbitpsec);
    }
    else
    {
        PRINTF(" IPERF Report error\r\n");
    }
}

/** Lets user select a mode to run IPERF with. */
static void select_mode(bool *server_mode,
                        enum lwiperf_client_type *client_type)
{
    char option;

    while (true)
    {
        PRINTF("Please select one of the following modes to run IPERF with:\r\n\r\n");
        PRINTF("    1: server mode (RX only test)\r\n");
        PRINTF("    2: client mode (TX only test)\r\n");
        PRINTF("    3: client dual mode (TX and RX in parallel)\r\n");
        PRINTF("    4: client tradeoff mode (TX and RX sequentially)\r\n\r\n");
        PRINTF("Enter mode number: ");

        option = GETCHAR();
        PUTCHAR(option);
        PRINTF("\r\n");

        switch (option)
        {
            case '1':
                *server_mode = true;
                *client_type = LWIPERF_CLIENT;
                return;
            case '2':
                *server_mode = false;
                *client_type = LWIPERF_CLIENT;
                return;
            case '3':
                *server_mode = false;
                *client_type = LWIPERF_DUAL;
                return;
            case '4':
                *server_mode = false;
                *client_type = LWIPERF_TRADEOFF;
                return;
        }
    }
}

/*!
 * @brief The main function containing client thread.
 */
static void iperf_task(void *arg)
{
    bool server_mode;
    enum lwiperf_client_type client_type;
    void *iperf_session = NULL;
    ip4_addr_t server_address;

    BOARD_InitNetwork();

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" IPERF example\r\n");
    PRINTF("************************************************\r\n");

    select_mode(&server_mode, &client_type);

    if (server_mode)
    {
        iperf_session = lwiperf_start_tcp_server(IP_ADDR_ANY, LWIPERF_TCP_PORT_DEFAULT, lwiperf_report, 0);
    }
    else
    {
        if (ipaddr_aton(IPERF_SERVER_ADDRESS, &server_address) && IP_IS_V4(&server_address))
        {
            iperf_session = lwiperf_start_tcp_client(&server_address, LWIPERF_TCP_PORT_DEFAULT, client_type, lwiperf_report, 0);
        }
        else
        {
            PRINTF("IPERF_SERVER_ADDRESS is not a valid IPv4 address!\r\n");
        }
    }

    if (iperf_session == NULL)
    {
        PRINTF("IPERF initialization failed!\r\n");
    }

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
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_USDHCClockConfiguration();
    BOARD_InitDebugConsole();

    tcpip_init(NULL, NULL);

    if (xTaskCreate(iperf_task, "iperf_task", 1000, NULL, configMAX_PRIORITIES - 4 /*3*/, NULL) != pdPASS)
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
