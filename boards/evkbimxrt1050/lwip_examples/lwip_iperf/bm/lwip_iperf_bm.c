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

#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_TCP && LWIP_UDP

#include "lwip/apps/lwiperf.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"

#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 0
#define configIP_ADDR3 102

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 192
#define configGW_ADDR1 168
#define configGW_ADDR2 0
#define configGW_ADDR3 100

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk

#ifndef IPERF_UDP_CLIENT_RATE
#define IPERF_UDP_CLIENT_RATE (1 * 1024 * 1024) /* 1 Mbit/s */
#endif                                          /* IPERF_UDP_CLIENT_RATE */

#ifndef IPERF_CLIENT_AMOUNT
#define IPERF_CLIENT_AMOUNT (-1000) /* 10 seconds */
#endif                              /* IPERF_CLIENT_AMOUNT */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}



/*!
 * @brief Interrupt service for SysTick timer.
 */
void SysTick_Handler(void)
{
    time_isr();
}

/* Report state => string */
const char *report_type_str[] = {
    "TCP_DONE_SERVER (RX)",        /* LWIPERF_TCP_DONE_SERVER,*/
    "TCP_DONE_CLIENT (TX)",        /* LWIPERF_TCP_DONE_CLIENT,*/
    "TCP_ABORTED_LOCAL",           /* LWIPERF_TCP_ABORTED_LOCAL, */
    "TCP_ABORTED_LOCAL_DATAERROR", /* LWIPERF_TCP_ABORTED_LOCAL_DATAERROR, */
    "TCP_ABORTED_LOCAL_TXERROR",   /* LWIPERF_TCP_ABORTED_LOCAL_TXERROR, */
    "TCP_ABORTED_REMOTE",          /* LWIPERF_TCP_ABORTED_REMOTE, */
    "UDP_DONE_SERVER (RX)",        /* LWIPERF_UDP_DONE_SERVER, */
    "UDP_DONE_CLIENT (TX)",        /* LWIPERF_UDP_DONE_CLIENT, */
    "UDP_ABORTED_LOCAL",           /* LWIPERF_UDP_ABORTED_LOCAL, */
    "UDP_ABORTED_LOCAL_DATAERROR", /* LWIPERF_UDP_ABORTED_LOCAL_DATAERROR, */
    "UDP_ABORTED_LOCAL_TXERROR",   /* LWIPERF_UDP_ABORTED_LOCAL_TXERROR, */
    "UDP_ABORTED_REMOTE",          /* LWIPERF_UDP_ABORTED_REMOTE, */
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
static void select_mode(bool *server_mode, bool *tcp, enum lwiperf_client_type *client_type)
{
    char option;

    while (true)
    {
        PRINTF("Please select one of the following modes to run IPERF with:\r\n\r\n");
        PRINTF("    1: TCP server mode (RX only test)\r\n");
        PRINTF("    2: TCP client mode (TX only test)\r\n");
        PRINTF("    3: TCP client dual mode (TX and RX in parallel)\r\n");
        PRINTF("    4: TCP client tradeoff mode (TX and RX sequentially)\r\n");
        PRINTF("    5: UDP server mode (RX only test)\r\n");
        PRINTF("    6: UDP client mode (TX only test)\r\n");
        PRINTF("    7: UDP client dual mode (TX and RX in parallel)\r\n");
        PRINTF("    8: UDP client tradeoff mode (TX and RX sequentially)\r\n\r\n");
        PRINTF("Enter mode number: ");

        option = GETCHAR();
        PUTCHAR(option);
        PRINTF("\r\n");

        switch (option)
        {
            case '1':
                *server_mode = true;
                *tcp         = true;
                *client_type = LWIPERF_CLIENT;
                return;
            case '2':
                *server_mode = false;
                *tcp         = true;
                *client_type = LWIPERF_CLIENT;
                return;
            case '3':
                *server_mode = false;
                *tcp         = true;
                *client_type = LWIPERF_DUAL;
                return;
            case '4':
                *server_mode = false;
                *tcp         = true;
                *client_type = LWIPERF_TRADEOFF;
                return;
            case '5':
                *server_mode = true;
                *tcp         = false;
                *client_type = LWIPERF_CLIENT;
                return;
            case '6':
                *server_mode = false;
                *tcp         = false;
                *client_type = LWIPERF_CLIENT;
                return;
            case '7':
                *server_mode = false;
                *tcp         = false;
                *client_type = LWIPERF_DUAL;
                return;
            case '8':
                *server_mode = false;
                *tcp         = false;
                *client_type = LWIPERF_TRADEOFF;
                return;
        }
    }
}

static void *start_iperf(ip4_addr_t *remote_addr)
{
    bool server_mode;
    bool tcp;
    enum lwiperf_client_type client_type;
    void *iperf_session;

    select_mode(&server_mode, &tcp, &client_type);

    if (server_mode)
    {
        if (tcp)
        {
            iperf_session = lwiperf_start_tcp_server(IP_ADDR_ANY, LWIPERF_TCP_PORT_DEFAULT, lwiperf_report, 0);
        }
        else
        {
            iperf_session =
                lwiperf_start_udp_server(netif_ip_addr4(netif_default), LWIPERF_TCP_PORT_DEFAULT, lwiperf_report, 0);
        }
    }
    else
    {
        if (tcp)
        {
            iperf_session = lwiperf_start_tcp_client(remote_addr, LWIPERF_TCP_PORT_DEFAULT, client_type,
                                                     IPERF_CLIENT_AMOUNT, lwiperf_report, 0);
        }
        else
        {
            iperf_session = lwiperf_start_udp_client(
                netif_ip_addr4(netif_default), LWIPERF_TCP_PORT_DEFAULT, remote_addr, LWIPERF_TCP_PORT_DEFAULT,
                client_type, IPERF_CLIENT_AMOUNT, IPERF_UDP_CLIENT_RATE, 0, lwiperf_report, NULL);
        }
    }

    if (iperf_session == NULL)
    {
        PRINTF("IPERF initialization failed!\r\n");
    }
    else
    {
        PRINTF("Press SPACE to abort the test and return to main menu\r\n");
    }

    return iperf_session;
}

/*!
 * @brief Main function.
 */
int main(void)
{
    void *iperf_session = NULL;
    status_t status;
    char key;
    struct netif fsl_netif0;
#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
    mem_range_t non_dma_memory[] = NON_DMA_MEMORY_ARRAY;
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */
    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
    ethernetif_config_t fsl_enet_config0 = {
        .phyAddress = EXAMPLE_PHY_ADDRESS,
        .clockName  = EXAMPLE_CLOCK_NAME,
        .macAddress = configMAC_ADDR,
#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
        .non_dma_memory = non_dma_memory,
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */
    };

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    delay();
    GPIO_WritePinOutput(GPIO1, 9, 1);

    time_init();

    IP4_ADDR(&fsl_netif0_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&fsl_netif0_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&fsl_netif0_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);

    lwip_init();

    netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, &fsl_enet_config0, ethernetif0_init,
              ethernet_input);
    netif_set_default(&fsl_netif0);
    netif_set_up(&fsl_netif0);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" IPERF example\r\n");
    PRINTF("************************************************\r\n");
    PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_ipaddr)[0], ((u8_t *)&fsl_netif0_ipaddr)[1],
           ((u8_t *)&fsl_netif0_ipaddr)[2], ((u8_t *)&fsl_netif0_ipaddr)[3]);
    PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_netmask)[0], ((u8_t *)&fsl_netif0_netmask)[1],
           ((u8_t *)&fsl_netif0_netmask)[2], ((u8_t *)&fsl_netif0_netmask)[3]);
    PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_gw)[0], ((u8_t *)&fsl_netif0_gw)[1],
           ((u8_t *)&fsl_netif0_gw)[2], ((u8_t *)&fsl_netif0_gw)[3]);
    PRINTF("************************************************\r\n");

    while (1)
    {
        if (iperf_session == NULL)
        {
            iperf_session = start_iperf(&fsl_netif0_gw);
        }
        else
        {
            status = DbgConsole_TryGetchar(&key);
            if ((status == kStatus_Success) && (key == ' '))
            {
                lwiperf_abort(iperf_session);
                iperf_session = NULL;
            }
        }

        /* Poll UDP client */
        lwiperf_poll_udp_client();

        /* Poll the driver, get any outstanding frames */
        ethernetif_input(&fsl_netif0);

        sys_check_timeouts(); /* Handle all system timeouts for all core protocols */
    }
}
#endif
