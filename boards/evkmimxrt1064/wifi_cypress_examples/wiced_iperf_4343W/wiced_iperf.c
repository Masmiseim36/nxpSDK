/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
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
#include "wwd_network.h"
#include "dhcp_server.h"

#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Common WiFi parameters */
#ifndef WIFI_SSID
#define WIFI_SSID "nxp"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "NXP0123456789"
#endif

#ifndef WIFI_SECURITY
#define WIFI_SECURITY WICED_SECURITY_WPA2_MIXED_PSK
#endif

/* Parameters that apply to AP mode only */
#ifndef WIFI_AP_MODE
#define WIFI_AP_MODE 0
#endif

#ifndef WIFI_AP_CHANNEL
#define WIFI_AP_CHANNEL 1
#endif

#define WIFI_AP_IP_ADDR "192.168.1.1"
#define WIFI_AP_NET_MASK "255.255.0.0"

/* IPerf related parameters */
#ifndef IPERF_SERVER_ADDRESS
#define IPERF_SERVER_ADDRESS "192.168.2.100"
#endif

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
    CLOCK_InitSysPll(&sysPllConfig_BOARD_BootClockRUN);
    /*configure system pll PFD2 fractional divider to 24*/
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 24U);
    /* Configure USDHC clock source and divider */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 0U);
}


static struct netif wiced_if;

static int BOARD_InitNetwork()
{
    wiced_result_t wiced_result = WICED_SUCCESS;
    wwd_result_t wwd_result     = WWD_SUCCESS;
    bool wifi_ap_mode           = WIFI_AP_MODE;

    wiced_ssid_t ap_ssid = {
        .length = sizeof(WIFI_SSID) - 1,
        .value  = WIFI_SSID,
    };

    PRINTF("Initializing WiFi connection... \r\n");
    wiced_result = wiced_wlan_connectivity_init();
    if (wiced_result != WICED_SUCCESS)
    {
        PRINTF("Could not initialize WiFi module %d\r\n", (uint32_t)wiced_result);
        return -1;
    }
    else
    {
        PRINTF("Successfully initialized WiFi module\r\n");
    }

    if (wifi_ap_mode)
    {
        /* AP mode */
        ip4_addr_t ap_ipaddr;
        ip4_addr_t ap_netmask;
        uint8_t ap_channel = WIFI_AP_CHANNEL;

        if ((ip4addr_aton(WIFI_AP_IP_ADDR, &ap_ipaddr) == 0) || (ip4addr_aton(WIFI_AP_NET_MASK, &ap_netmask) == 0))
        {
            PRINTF("Invalid IP address\r\n");
            return -1;
        }

        PRINTF("Starting Access Point: SSID: %s, Chnl: %d\r\n", (char *)ap_ssid.value, ap_channel);

        /* Start the access point */
        wwd_result =
            wwd_wifi_start_ap(&ap_ssid, WIFI_SECURITY, (uint8_t *)WIFI_PASSWORD, sizeof(WIFI_PASSWORD) - 1, ap_channel);
        if (wwd_result != WWD_SUCCESS)
        {
            PRINTF("Failed to start access point\r\n");
            return -1;
        }

        /* Configure network interface */
        if (NULL == netif_add(&wiced_if, &ap_ipaddr, &ap_netmask, &ap_ipaddr, (void *)WWD_AP_INTERFACE, wlanif_init,
                              tcpip_input))
        {
            PRINTF("Failed to start network interface\r\n");
            return -1;
        }
        netif_set_default(&wiced_if);
        netif_set_up(&wiced_if);

        PRINTF("Network ready IP: %u.%u.%u.%u\r\n", (unsigned char)((htonl(wiced_if.ip_addr.addr) >> 24) & 0xff),
               (unsigned char)((htonl(wiced_if.ip_addr.addr) >> 16) & 0xff),
               (unsigned char)((htonl(wiced_if.ip_addr.addr) >> 8) & 0xff),
               (unsigned char)((htonl(wiced_if.ip_addr.addr) >> 0) & 0xff));
        /* Start DHCP server */
        start_dhcp_server(ap_ipaddr.addr);

        return 0;
    }

    /* Client mode */
    wwd_result = test_scan();
    if (wwd_result != WWD_SUCCESS)
    {
        PRINTF(" Scan Error\r\n");
        return -1;
    }

    PRINTF("Joining: " WIFI_SSID "\r\n");
    (void)host_rtos_delay_milliseconds((uint32_t)1000);
    wwd_result = wwd_wifi_join(&ap_ssid, WIFI_SECURITY, (uint8_t *)WIFI_PASSWORD, sizeof(WIFI_PASSWORD) - 1, NULL,
                               WWD_STA_INTERFACE);
    if (wwd_result != WWD_SUCCESS)
    {
        PRINTF("Failed to join: " WIFI_SSID "\r\n");
        return -1;
    }
    else
    {
        PRINTF("Successfully joined: " WIFI_SSID "\r\n");
        (void)host_rtos_delay_milliseconds((uint32_t)1000);
        add_wlan_interface();
    }

    return 0;
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
    if (report_type < (sizeof(report_type_str) / sizeof(report_type_str[0])))
    {
        PRINTF(" %s \r\n", report_type_str[report_type]);
        if (local_addr && remote_addr)
        {
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
    }
    else
    {
        PRINTF(" IPERF Report error\r\n");
    }
    PRINTF("\r\n");
}

/** Lets user select a mode to run IPERF with. */
static void select_mode(bool *server_mode, enum lwiperf_client_type *client_type)
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

struct iperf_test_context
{
    bool server_mode;
    enum lwiperf_client_type client_type;
    void *iperf_session;
};

/*!
 * @brief Function to start iperf test.
 */
static void iperf_test_start(void *arg)
{
    struct iperf_test_context *ctx = (struct iperf_test_context *)arg;
    ip4_addr_t server_address;

    if (ctx->server_mode)
    {
        ctx->iperf_session = lwiperf_start_tcp_server(IP_ADDR_ANY, LWIPERF_TCP_PORT_DEFAULT, lwiperf_report, 0);
    }
    else
    {
        if (ipaddr_aton(IPERF_SERVER_ADDRESS, &server_address) && IP_IS_V4(&server_address))
        {
            ctx->iperf_session = lwiperf_start_tcp_client(&server_address, LWIPERF_TCP_PORT_DEFAULT, ctx->client_type,
                                                          lwiperf_report, 0);
        }
        else
        {
            PRINTF("IPERF_SERVER_ADDRESS is not a valid IPv4 address!\r\n");
        }
    }

    if (ctx->iperf_session == NULL)
    {
        PRINTF("IPERF initialization failed!\r\n");
    }
}

/*!
 * @brief Function to abort iperf test.
 */
static void iperf_test_abort(void *arg)
{
    struct iperf_test_context *ctx = (struct iperf_test_context *)arg;

    if (ctx->iperf_session != NULL)
    {
        lwiperf_abort(ctx->iperf_session);
        ctx->iperf_session = NULL;
    }
}

/*!
 * @brief The main function containing client thread.
 */
static void iperf_task(void *arg)
{
    char key;
    struct iperf_test_context ctx;

    if (BOARD_InitNetwork() != 0)
    {
        PRINTF("No connectivity.\r\n");
        vTaskSuspend(NULL);
    }

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" IPERF example\r\n");
    PRINTF("************************************************\r\n");

    while (true)
    {
        select_mode(&ctx.server_mode, &ctx.client_type);
        tcpip_callback(iperf_test_start, (void *)&ctx);

        PRINTF("Press SPACE to abort the test and return to main menu\r\n");
        do
        {
            key = GETCHAR();
        } while (key != ' ');

        tcpip_callback(iperf_test_abort, (void *)&ctx);
    }
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
