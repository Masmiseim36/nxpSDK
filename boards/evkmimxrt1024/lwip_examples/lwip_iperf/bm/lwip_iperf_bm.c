/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020,2022-2023 NXP
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
#include "ethernetif.h"

#ifndef configMAC_ADDR
#include "fsl_silicon_id.h"
#endif
#include "fsl_phy.h"
#include "pin_mux.h"
#include "board.h"

#include "fsl_iomuxc.h"
#include "fsl_enet.h"
#include "fsl_phyksz8081.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* @TEST_ANCHOR */

/* IP address configuration. */
#ifndef configIP_ADDR0
#define configIP_ADDR0 192
#endif
#ifndef configIP_ADDR1
#define configIP_ADDR1 168
#endif
#ifndef configIP_ADDR2
#define configIP_ADDR2 0
#endif
#ifndef configIP_ADDR3
#define configIP_ADDR3 102
#endif

/* Netmask configuration. */
#ifndef configNET_MASK0
#define configNET_MASK0 255
#endif
#ifndef configNET_MASK1
#define configNET_MASK1 255
#endif
#ifndef configNET_MASK2
#define configNET_MASK2 255
#endif
#ifndef configNET_MASK3
#define configNET_MASK3 0
#endif

/* Gateway address configuration. */
#ifndef configGW_ADDR0
#define configGW_ADDR0 192
#endif
#ifndef configGW_ADDR1
#define configGW_ADDR1 168
#endif
#ifndef configGW_ADDR2
#define configGW_ADDR2 0
#endif
#ifndef configGW_ADDR3
#define configGW_ADDR3 100
#endif

/* Ethernet configuration. */
extern phy_ksz8081_resource_t g_phy_resource;
#define EXAMPLE_ENET         ENET
#define EXAMPLE_PHY_ADDRESS  0x02U
#define EXAMPLE_PHY_OPS      &phyksz8081_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetFreq(kCLOCK_IpgClk)

#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

#ifndef IPERF_UDP_CLIENT_RATE
#define IPERF_UDP_CLIENT_RATE (100 * 1024 * 1024) /* 100 Mbit/s */
#endif                                            /* IPERF_UDP_CLIENT_RATE */

#ifndef IPERF_CLIENT_AMOUNT
#define IPERF_CLIENT_AMOUNT (-1000) /* 10 seconds */
#endif                              /* IPERF_CLIENT_AMOUNT */

/* @TEST_ANCHOR */

#ifndef EXAMPLE_PORT
#define EXAMPLE_PORT LWIPERF_TCP_PORT_DEFAULT
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
phy_ksz8081_resource_t g_phy_resource;

static phy_handle_t phyHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {
        .enableClkOutput = true, .enableClkOutput500M = false, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
    ENET_SetSMI(EXAMPLE_ENET, EXAMPLE_CLOCK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
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
    "TCP_DONE_SERVER (RX)",        /* LWIPERF_TCP_DONE_SERVER_RX,*/
    "TCP_DONE_SERVER (TX)",        /* LWIPERF_TCP_DONE_SERVER_TX,*/
    "TCP_DONE_CLIENT (TX)",        /* LWIPERF_TCP_DONE_CLIENT_TX,*/
    "TCP_DONE_CLIENT (RX)",        /* LWIPERF_TCP_DONE_CLIENT_RX,*/
    "TCP_ABORTED_LOCAL",           /* LWIPERF_TCP_ABORTED_LOCAL, */
    "TCP_ABORTED_LOCAL_DATAERROR", /* LWIPERF_TCP_ABORTED_LOCAL_DATAERROR, */
    "TCP_ABORTED_LOCAL_TXERROR",   /* LWIPERF_TCP_ABORTED_LOCAL_TXERROR, */
    "TCP_ABORTED_REMOTE",          /* LWIPERF_TCP_ABORTED_REMOTE, */
    "UDP_DONE_SERVER (RX)",        /* LWIPERF_UDP_DONE_SERVER_RX, */
    "UDP_DONE_SERVER (TX)",        /* LWIPERF_UDP_DONE_SERVER_TX, */
    "UDP_DONE_CLIENT (TX)",        /* LWIPERF_UDP_DONE_CLIENT_TX, */
    "UDP_DONE_CLIENT (RX)",        /* LWIPERF_UDP_DONE_CLIENT_RX, */
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
                           u64_t bytes_transferred,
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
            PRINTF(" Port %u \r\n", remote_port);
            PRINTF(" Bytes Transferred %llu \r\n", bytes_transferred);
            PRINTF(" Duration (ms) %u \r\n", ms_duration);
            PRINTF(" Bandwidth (kbitpsec) %u \r\n", bandwidth_kbitpsec);
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
        PRINTF("    0: TCP server mode (RX test)\r\n");
        PRINTF("    1: TCP client mode (TX only test)\r\n");
        PRINTF("    2: TCP client reverse mode (RX only test)\r\n");
        PRINTF("    3: TCP client dual mode (TX and RX in parallel)\r\n");
        PRINTF("    4: TCP client tradeoff mode (TX and RX sequentially)\r\n");
        PRINTF("    5: UDP server mode (RX test)\r\n");
        PRINTF("    6: UDP client mode (TX only test)\r\n");
        PRINTF("    7: UDP client reverse mode (RX only test)\r\n");
        PRINTF("    8: UDP client dual mode (TX and RX in parallel)\r\n");
        PRINTF("    9: UDP client tradeoff mode (TX and RX sequentially)\r\n\r\n");
        PRINTF("Enter mode number: ");

        option = GETCHAR();
        PUTCHAR(option);
        PRINTF("\r\n");

        switch (option)
        {
            case '0':
                *server_mode = true;
                *tcp         = true;
                *client_type = LWIPERF_CLIENT;
                return;
            case '1':
                *server_mode = false;
                *tcp         = true;
                *client_type = LWIPERF_CLIENT;
                return;
            case '2':
                *server_mode = false;
                *tcp         = true;
                *client_type = LWIPERF_REVERSE;
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
                *client_type = LWIPERF_REVERSE;
                return;
            case '8':
                *server_mode = false;
                *tcp         = false;
                *client_type = LWIPERF_DUAL;
                return;
            case '9':
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
            iperf_session = lwiperf_start_tcp_server(IP_ADDR_ANY, EXAMPLE_PORT, lwiperf_report, 0);
        }
        else
        {
            iperf_session = lwiperf_start_udp_server(netif_ip_addr4(netif_default), EXAMPLE_PORT, lwiperf_report, 0);
        }
    }
    else
    {
        if (tcp)
        {
            iperf_session = lwiperf_start_tcp_client(remote_addr, EXAMPLE_PORT, client_type, IPERF_CLIENT_AMOUNT, 0,
                                                     LWIPERF_TOS_DEFAULT, lwiperf_report, 0);
        }
        else
        {
            iperf_session = lwiperf_start_udp_client(netif_ip_addr4(netif_default), EXAMPLE_PORT, remote_addr,
                                                     EXAMPLE_PORT, client_type, IPERF_CLIENT_AMOUNT, 0,
                                                     IPERF_UDP_CLIENT_RATE, 0, lwiperf_report, NULL);
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
    struct netif netif;
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {.phyHandle   = &phyHandle,
                                       .phyAddr     = EXAMPLE_PHY_ADDRESS,
                                       .phyOps      = EXAMPLE_PHY_OPS,
                                       .phyResource = EXAMPLE_PHY_RESOURCE,
#ifdef configMAC_ADDR
                                       .macAddress = configMAC_ADDR
#endif
    };

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    GPIO_PinInit(GPIO1, 4, &gpio_config);
    GPIO_PinInit(GPIO1, 22, &gpio_config);
    /* Pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 22, 1);
    GPIO_WritePinOutput(GPIO1, 4, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO1, 4, 1);

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    time_init();

    /* Set MAC address. */
#ifndef configMAC_ADDR
    (void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);
#endif

    /* Get clock after hardware init. */
        enet_config.srcClockHz = EXAMPLE_CLOCK_FREQ;

    IP4_ADDR(&netif_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&netif_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&netif_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);

    lwip_init();

    netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN, ethernet_input);
    netif_set_default(&netif);
    netif_set_up(&netif);

    while (ethernetif_wait_linkup(&netif, 5000) != ERR_OK)
    {
        PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
    }

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" IPERF example\r\n");
    PRINTF("************************************************\r\n");
    PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&netif_ipaddr)[0], ((u8_t *)&netif_ipaddr)[1],
           ((u8_t *)&netif_ipaddr)[2], ((u8_t *)&netif_ipaddr)[3]);
    PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&netif_netmask)[0], ((u8_t *)&netif_netmask)[1],
           ((u8_t *)&netif_netmask)[2], ((u8_t *)&netif_netmask)[3]);
    PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&netif_gw)[0], ((u8_t *)&netif_gw)[1],
           ((u8_t *)&netif_gw)[2], ((u8_t *)&netif_gw)[3]);
    PRINTF("************************************************\r\n");

    while (1)
    {
        if (iperf_session == NULL)
        {
            iperf_session = start_iperf(&netif_gw);
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
        ethernetif_input(&netif);

        sys_check_timeouts(); /* Handle all system timeouts for all core protocols */
    }
}
#endif
