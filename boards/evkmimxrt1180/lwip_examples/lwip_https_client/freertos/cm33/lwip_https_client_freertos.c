/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lwip/opt.h"

#if LWIP_SOCKET
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "app_https_client_task.h"
#include "ethernetif.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_phy.h"

#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/arch.h"
#include "lwip/dns.h"
#include "lwip/api.h"
#include "lwip/netifapi.h"
#include "lwip/tcpip.h"
#include "lwip/ip.h"

#ifdef MBEDTLS_MCUX_ELE_S400_API
#include "ele_mbedtls.h"
#else
#include "ksdk_mbedtls.h"
#endif /* MBEDTLS_MCUX_ELE_S400_API */

#include "fsl_phyrtl8201.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_msgintr.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PHY_PAGE_SELECT_REG 0x1FU /*!< The PHY page select register. */
#define EXAMPLE_EP0_PORT  0x00U
#define EXAMPLE_SWT_PORT0 0x01U
#define EXAMPLE_SWT_PORT1 0x02U
#define EXAMPLE_SWT_PORT2 0x03U
#define EXAMPLE_SWT_PORT3 0x04U

#define EXAMPLE_EP0_PHY_ADDR       0x03U
#define EXAMPLE_SWT_PORT0_PHY_ADDR 0x02U
#define EXAMPLE_SWT_PORT1_PHY_ADDR 0x05U
#define EXAMPLE_SWT_PORT2_PHY_ADDR 0x04U
#define EXAMPLE_SWT_PORT3_PHY_ADDR 0x08U
#define EXAMPLE_NETC_FREQ          CLOCK_GetRootClockFreq(kCLOCK_Root_Netc)

#define EXAMPLE_EP_RING_NUM          3U
#define EXAMPLE_EP_RXBD_NUM          8U
#define EXAMPLE_EP_TXBD_NUM          8U
#define EXAMPLE_EP_BD_ALIGN          128U
#define EXAMPLE_EP_BUFF_SIZE_ALIGN   64U
#define EXAMPLE_EP_RXBUFF_SIZE       1518U
#define EXAMPLE_EP_RXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EXAMPLE_EP_RXBUFF_SIZE, EXAMPLE_EP_BUFF_SIZE_ALIGN)
#define EXAMPLE_EP_TEST_FRAME_SIZE   1000U

#define EXAMPLE_EP_TXFRAME_NUM 20U
#define EXAMPLE_TX_RX_INTERRUPT_HANDLE

/* @TEST_ANCHOR */

/* Ethernet configuration. */

#define configMAC_ADDR                     \
    {                                      \
        0x00, 0x00, 0xfa, 0xfa, 0xdd, 0x05 \
    }

#define EXAMPLE_PHY_ADDRESS  EXAMPLE_EP0_PHY_ADDR
#define EXAMPLE_PHY_OPS      &g_app_phy_rtl8201_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_rtl8201_resource
#define EXAMPLE_CLOCK_FREQ   EXAMPLE_NETC_FREQ // CLOCK_GetFreq(kCLOCK_IpgClk)

#define EXAMPLE_PHY_INT_PORT BOARD_INITPHYACCESSPINS_ENET4_INT_B_PERIPHERAL
#define EXAMPLE_PHY_INT_PIN  BOARD_INITPHYACCESSPINS_ENET4_INT_B_CHANNEL
/* Must be after include of app.h */
#ifndef configMAC_ADDR
#include "fsl_silicon_id.h"
#endif

#ifndef EXAMPLE_HOST
/*! @brief Host name of the HTTPS server to download data from. */
#define EXAMPLE_HOST "http.minori.work"
#endif /* EXAMPLE_HOST */

#ifndef EXAMPLE_PORT
/*! @brief Port number of the HTTPS server to download data from. */
#define EXAMPLE_PORT 443
#endif /* EXAMPLE_PORT */

#ifndef EXAMPLE_PATH
/*! @brief HTTP path of the resource to download from the HTTPS server. */
#define EXAMPLE_PATH "/get"
#endif /* EXAMPLE_PATH */

#ifndef EXAMPLE_MAX_REQUEST_LENGTH
/*! @brief Maximum length of HTTP request. */
#define EXAMPLE_MAX_REQUEST_LENGTH 256
#endif /* EXAMPLE_MAX_REQUEST_LENGTH */

#ifndef EXAMPLE_MAX_RESPONSE_LENGTH
/*! @brief Maximum length of HTTP response. */
#define EXAMPLE_MAX_RESPONSE_LENGTH 1024
#endif /* EXAMPLE_MAX_RESPONSE_LENGTH */

#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

#ifndef INIT_THREAD_STACKSIZE
/*! @brief Stack size of the temporary initialization thread in words. */
#define INIT_THREAD_STACKSIZE 1024
#endif /* INIT_THREAD_STACKSIZE */

#ifndef INIT_THREAD_PRIO
/*! @brief Priority of the temporary initialization thread. */
#define INIT_THREAD_PRIO 5
#endif /* INIT_THREAD_PRIO */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t APP_EP0_MDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data);
status_t APP_EP0_MDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData);

#if LWIP_IPV6
static void print_ipv6_addresses(struct netif *netif);
static void netif_ipv6_callback(struct netif *cb_netif);
#endif /* LWIP_IPV6 */

static void init_task(void *arg);
static void init_task_init(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static status_t APP_PHY_RTL8201_Init(phy_handle_t *handle, const phy_config_t *config);
static void APP_MDIO_Init(void);

/* PHY operation. */
static netc_mdio_handle_t s_mdio_handle;
// static phy_handle_t s_phy_handle;
// static uint8_t s_phy_address       = EXAMPLE_EP0_PHY_ADDR;
static mdioRead s_mdio_func_read   = APP_EP0_MDIORead;
static mdioWrite s_mdio_func_write = APP_EP0_MDIOWrite;

phy_rtl8201_resource_t g_phy_rtl8201_resource /*= {.read = }*/;

const phy_operations_t g_app_phy_rtl8201_ops = {.phyInit            = APP_PHY_RTL8201_Init,
                                                .phyWrite           = PHY_RTL8201_Write,
                                                .phyRead            = PHY_RTL8201_Read,
                                                .getAutoNegoStatus  = PHY_RTL8201_GetAutoNegotiationStatus,
                                                .getLinkStatus      = PHY_RTL8201_GetLinkStatus,
                                                .getLinkSpeedDuplex = PHY_RTL8201_GetLinkSpeedDuplex,
                                                .setLinkSpeedDuplex = PHY_RTL8201_SetLinkSpeedDuplex,
                                                .enableLoopback     = PHY_RTL8201_EnableLoopback,
                                                .enableLinkInterrupt= PHY_RTL8201_EnableLinkInterrupt,
                                                .clearInterrupt     = PHY_RTL8201_ClearInterrupt};

static phy_handle_t phyHandle;
static struct netif netif;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* This does initialization and then reconfigures CRS/DV pin to RXDV signal. */
static status_t APP_PHY_RTL8201_Init(phy_handle_t *handle, const phy_config_t *config)
{
    status_t result;
    uint16_t data;

    APP_MDIO_Init();
	
    /* Reset PHY8201 for ETH4. Reset 10ms, wait 72ms. */
    RGPIO_PinWrite(BOARD_INITPHYACCESSPINS_ENET4_RST_B_GPIO, BOARD_INITPHYACCESSPINS_ENET4_RST_B_GPIO_PIN, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    RGPIO_PinWrite(BOARD_INITPHYACCESSPINS_ENET4_RST_B_GPIO, BOARD_INITPHYACCESSPINS_ENET4_RST_B_GPIO_PIN, 1);
    SDK_DelayAtLeastUs(72000, CLOCK_GetFreq(kCLOCK_CpuClk));

    result = PHY_RTL8201_Init(handle, config);
    if (result != kStatus_Success)
    {
        return result;
    }

    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 7);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Read(handle, 16, &data);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* CRS/DV pin is RXDV signal. */
    data |= (1U << 2);
    result = PHY_Write(handle, 16, data);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 0);

    return result;
}

static void APP_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .mdio =
            {
                .type = kNETC_EMdio,
            },
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = EXAMPLE_NETC_FREQ,
    };

    mdioConfig.mdio.port = (netc_hw_eth_port_idx_t)kNETC_ENETC0EthPort;
    result               = NETC_MDIOInit(&s_mdio_handle, &mdioConfig);
    while (result != kStatus_Success)
    {
        // failed
    }
}

status_t APP_EP0_MDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_mdio_handle, phyAddr, regAddr, data);
}

status_t APP_EP0_MDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_mdio_handle, phyAddr, regAddr, pData);
}



/*!
 * @brief Main function.
 */
int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* RMII mode */
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[0] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(1);
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[4] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(1);

    /* RGMII mode */
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[1] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(2);

    /* Output reference clock for RMII */
    BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG |= BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT0_RMII_REF_CLK_DIR_MASK |
                                              BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT4_RMII_REF_CLK_DIR_MASK;

    /* Unlock the IERB. It will warm reset whole NETC. */
    NETC_PRIV->NETCRR &= ~NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U)
    {
    }

    /* Set PHY address in IERB to use MAC port MDIO, otherwise the access will be blocked. */
    NETC_IERB->L0BCR = NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(EXAMPLE_SWT_PORT0_PHY_ADDR);
    NETC_IERB->L1BCR = NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(EXAMPLE_SWT_PORT1_PHY_ADDR);
    NETC_IERB->L4BCR = NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(EXAMPLE_EP0_PHY_ADDR);

    /* Set the access attribute, otherwise MSIX access will be blocked. */
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR &= ~(7U << 27);
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR |= (1U << 27);

    /* Lock the IERB. */
    NETC_PRIV->NETCRR |= NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCSR & NETC_PRIV_NETCSR_STATE_MASK) != 0U)
    {
    }

    /*result = APP_MDIO_Init();

    if (result != kStatus_Success)
    {
        while (true)
        {
        }
    }*/

    g_phy_rtl8201_resource.write    = s_mdio_func_write;
    g_phy_rtl8201_resource.writeExt = NULL;
    g_phy_rtl8201_resource.read     = s_mdio_func_read;
    g_phy_rtl8201_resource.readExt  = NULL;

    init_task_init();

    /* run RTOS */
    vTaskStartScheduler();

    /* should not reach this statement */
    for (;;)
        ;
}

/*!
 * @brief Starts initialization task.
 */
static void init_task_init(void)
{
    /* create server thread in RTOS */
    if (sys_thread_new("init_task", init_task, NULL, INIT_THREAD_STACKSIZE, INIT_THREAD_PRIO) == NULL)
    {
        LWIP_ASSERT("init_task creation failed", 0);
    }
}

/*!
 * @brief Initializes crypto hw, lwIP stack and starts application task.
 */
static void init_task(void *arg)
{
    const ip_addr_t *dns_address;
    const char *str_ip;
    status_t status;
    uint8_t numdns;

    LWIP_UNUSED_ARG(arg);

    ethernetif_config_t enet_config = {.phyHandle   = &phyHandle,
                                       .phyAddr     = EXAMPLE_PHY_ADDRESS,
                                       .phyOps      = EXAMPLE_PHY_OPS,
                                       .phyResource = EXAMPLE_PHY_RESOURCE,
                                       .srcClockHz  = EXAMPLE_CLOCK_FREQ,
#ifdef configMAC_ADDR
                                       .macAddress = configMAC_ADDR
#endif
    };

    https_client_task_context_t client_context = {
        .host             = EXAMPLE_HOST,
        .port             = EXAMPLE_PORT,
        .path             = EXAMPLE_PATH,
        .max_request_len  = EXAMPLE_MAX_REQUEST_LENGTH,
        .max_response_len = EXAMPLE_MAX_RESPONSE_LENGTH,
    };

    LWIP_PLATFORM_DIAG(("***********************************************************\r\n"));
    LWIP_PLATFORM_DIAG(("lwIP HTTPS client example\r\n"));
    LWIP_PLATFORM_DIAG(("***********************************************************\r\n"));

    status = CRYPTO_InitHardware();
    LWIP_ASSERT("CRYPTO_InitHardware() has failed\r\n", status == kStatus_Success);

    tcpip_init(NULL, NULL);

    /* Set MAC address. */
#ifndef configMAC_ADDR
    (void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);
#endif

#if LWIP_IPV4
    netifapi_netif_add(&netif, NULL, NULL, NULL, &enet_config, EXAMPLE_NETIF_INIT_FN, tcpip_input);
#else
    netifapi_netif_add(&netif, &enet_config, EXAMPLE_NETIF_INIT_FN, tcpip_input);
#endif /* LWIP_IPV4 */
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);
#if LWIP_IPV4
    netifapi_dhcp_start(&netif);
#endif /* LWIP_IPV4 */

#if LWIP_IPV6
    LOCK_TCPIP_CORE();
    netif_create_ip6_linklocal_address(&netif, 1);
    UNLOCK_TCPIP_CORE();
#endif /* LWIP_IPV6 */

    while (ethernetif_wait_linkup(&netif, 5000) != ERR_OK)
    {
        LWIP_PLATFORM_DIAG(
            ("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n"));
    }

#if LWIP_IPV4
    LWIP_PLATFORM_DIAG(("Getting IPv4 address from DHCP...\r\n"));
    (void)ethernetif_wait_ipv4_valid(&netif, ETHERNETIF_WAIT_FOREVER);
#endif /* LWIP_IPV4 */

    LOCK_TCPIP_CORE();
#if LWIP_IPV4
    LWIP_PLATFORM_DIAG((" IPv4 Address     : %s\r\n", ipaddr_ntoa(&netif.ip_addr)));
    LWIP_PLATFORM_DIAG((" IPv4 Subnet mask : %s\r\n", ipaddr_ntoa(&netif.netmask)));
    LWIP_PLATFORM_DIAG((" IPv4 Gateway     : %s\r\n", ipaddr_ntoa(&netif.gw)));
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
    set_ipv6_valid_state_cb(netif_ipv6_callback);
    print_ipv6_addresses(&netif);
#endif /* LWIP_IPV6 */

    for (numdns = 0U; numdns < DNS_MAX_SERVERS; numdns++)
    {
        dns_address = dns_getserver(numdns);
        str_ip      = "-";
        if (!ip_addr_isany(dns_address))
        {
            str_ip = ipaddr_ntoa(dns_address);
        }
        LWIP_PLATFORM_DIAG((" DNS Server%u      : %s\r\n", numdns, str_ip));
    }

    LWIP_PLATFORM_DIAG(("***********************************************************\r\n\r\n"));
    UNLOCK_TCPIP_CORE();

    /* Start application task */
    app_https_client_task_init(&client_context);

    vTaskDelete(NULL);
}

#if LWIP_IPV6
static void print_ipv6_addresses(struct netif *netif)
{
    for (int i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++)
    {
        const char *str_ip = "-";
        if (ip6_addr_isvalid(netif_ip6_addr_state(netif, i)))
        {
            str_ip = ip6addr_ntoa(netif_ip6_addr(netif, i));
        }
        LWIP_PLATFORM_DIAG((" IPv6 Address%d    : %s\r\n", i, str_ip));
    }
}

static void netif_ipv6_callback(struct netif *cb_netif)
{
    LWIP_PLATFORM_DIAG(("\r\nIPv6 address update, valid addresses:\r\n"));
    print_ipv6_addresses(cb_netif);
    LWIP_PLATFORM_DIAG(("\r\n"));
}
#endif /* LWIP_IPV6 */

#endif /* LWIP_SOCKET */
