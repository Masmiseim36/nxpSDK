/**
 * @file sm_demo_utils.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 *
 * Copyright 2017,2018,2020,2022 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
 * @par Description
 * Initialize LWIP / Ethernet / DHCP Connection on board
 * Set and Get Flag in GP Storage
 * json utility function
 */

/*******************************************************************************
 * includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

#include <board.h>

#ifdef USE_RTOS

#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#if SSS_HAVE_HOSTCRYPTO_MBEDTLS
#include "ksdk_mbedtls.h"
#endif
#include "nxLog_App.h"

#if !defined(NORDIC_MCU)
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#endif // !defined(NORDIC_MCU)

//#include "aws_clientcredential.h"

#if defined(LPC_WIFI)
#   include "iot_wifi.h"
#   include "wifi_config.h"
#   include "serial_mwm.h"

#elif defined(LPC_ENET)
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "lwip/netifapi.h"
#ifdef EXAMPLE_USE_100M_ENET_PORT
#include "fsl_enet.h"
#include "fsl_phyksz8081.h"
#else
#include "fsl_enet.h"
#include "fsl_phyrtl8211f.h"
#endif
#endif

#if defined(LPC_ENET)
/* ENET clock frequency. */
#if defined(CPU_MIMXRT1176DVMAA_cm7)
#define EXAMPLE_CLOCK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
#elif defined(CPU_MIMXRT1062DVL6A) || defined (CPU_MIMXRT1062DVL6B)
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)
#else
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#endif // CPU_MIMXRT1176DVMAA_cm7
/* MDIO operations. */
#define EXAMPLE_MDIO_OPS enet_ops

#ifdef EXAMPLE_USE_100M_ENET_PORT
/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS
phy_ksz8081_resource_t g_phy_resource;
/* PHY operations. */
#define EXAMPLE_PHY_OPS &phyksz8081_ops
/* ENET instance select. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#define EXAMPLE_ENET ENET
#else
/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET1_PHY_ADDRESS
phy_rtl8211f_resource_t g_phy_resource;
/* PHY operations. */
#define EXAMPLE_PHY_OPS &phyrtl8211f_ops
/* ENET instance select. */
#define EXAMPLE_NETIF_INIT_FN ethernetif1_init
#define EXAMPLE_ENET          ENET_1G
#endif // EXAMPLE_USE_100M_ENET_PORT
#define EXAMPLE_PHY_RESOURCE &g_phy_resource

#ifdef EXAMPLE_USE_100M_ENET_PORT
extern phy_ksz8081_resource_t g_phy_resource;
#else
extern phy_rtl8211f_resource_t g_phy_resource;
#endif
/* PHY operations. */
#endif // (LPC_ENET)

#include "HLSEAPI.h"
#include "sm_demo_utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(LPC_ENET)
/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x04, 0x12, 0x13, 0xB1, 0x11, 0x90 \
    }

/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk

/* Facilitate a simple hash for unique MAC Address based on an input 18 byte UID */
#define MAC_HASH(N) \
    enet_config.macAddress[N] = buffer[(N + 2) + (5 * 0)] ^ buffer[(N + 2) + (5 * 1)] ^ buffer[(N + 2) + (5 * 2)]

/*******************************************************************************
 * Static variables
 ******************************************************************************/
static struct netif fsl_netif;
static phy_handle_t phyHandle;

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

#endif // LPC_ENET

/*******************************************************************************
 * Global variables
 ******************************************************************************/
#if (SSS_HAVE_APPLET_A71CH || SSS_HAVE_APPLET_A71CH_SIM)
HLSE_OBJECT_HANDLE Gpstorage_handle;
#endif

/*******************************************************************************
 * Global Function Definitions
 ******************************************************************************/

/*Init the board network */
void BOARD_InitNetwork_MAC(const unsigned char buffer[18])
{
#if defined(LPC_WIFI)

    WIFINetworkParams_t pxNetworkParams;

    if (strlen(clientcredentialWIFI_SSID) > sizeof(pxNetworkParams.ucSSID)) {
        LOG_E("Insufficient memory for clientcredentialWIFI_SSID");
        while (1) {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }
    memcpy(pxNetworkParams.ucSSID, clientcredentialWIFI_SSID, strlen(clientcredentialWIFI_SSID));
    pxNetworkParams.ucSSIDLength = strlen(clientcredentialWIFI_SSID);

    if (strlen(clientcredentialWIFI_PASSWORD) > sizeof(pxNetworkParams.xPassword.xWPA.cPassphrase)) {
        LOG_E("Insufficient memory for clientcredentialWIFI_PASSWORD");
        while (1) {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }
    memcpy(pxNetworkParams.xPassword.xWPA.cPassphrase, clientcredentialWIFI_PASSWORD, strlen(clientcredentialWIFI_PASSWORD));
    pxNetworkParams.xPassword.xWPA.ucLength = strlen(clientcredentialWIFI_PASSWORD);

    pxNetworkParams.xSecurity = clientcredentialWIFI_SECURITY;

    WIFIReturnCode_t result;
    LOG_I("Turning WIFI ON");
    result = network_wifi_init();
    if (result != eWiFiSuccess) {
        LOG_E("network_wifi_init failed");
        while (1) {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }

    LOG_I("Connecting to network:%s", clientcredentialWIFI_SSID);
    result = network_wifi_connect_ap();
    if (result != eWiFiSuccess) {
        LOG_E("network_wifi_connect_ap failed");
        while (1) {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }

#elif defined(LPC_ENET)
#if FSL_FEATURE_SOC_ENET_COUNT > 0 || FSL_FEATURE_SOC_LPC_ENET_COUNT > 0

ethernetif_config_t fsl_enet_config0 = {.phyHandle   = &phyHandle,
                                        .phyAddr     = EXAMPLE_PHY_ADDRESS,
                                        .phyOps      = EXAMPLE_PHY_OPS,
                                        .phyResource = EXAMPLE_PHY_RESOURCE,
                                        .srcClockHz  = EXAMPLE_CLOCK_FREQ,
                                        .macAddress = configMAC_ADDR
    };

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    tcpip_init(NULL, NULL);

    netifapi_netif_add(&fsl_netif, NULL, NULL, NULL, &fsl_enet_config0, EXAMPLE_NETIF_INIT_FN, tcpip_input);
    netifapi_netif_set_default(&fsl_netif);
    netifapi_netif_set_up(&fsl_netif);

    while (ethernetif_wait_linkup(&fsl_netif, 5000) != ERR_OK)
    {
        PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
    }

    LOG_I("Getting IP address from DHCP ...\n");
    netifapi_dhcp_start(&fsl_netif);

    struct dhcp *dhcp;
    dhcp = (struct dhcp *)netif_get_client_data(&fsl_netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

    while (dhcp->state != DHCP_STATE_BOUND) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    if (dhcp->state == DHCP_STATE_BOUND) {
        LOG_I("\r\n IPv4 Address     : %u.%u.%u.%u\r\n",
            ((u8_t *)&fsl_netif.ip_addr.addr)[0],
            ((u8_t *)&fsl_netif.ip_addr.addr)[1],
            ((u8_t *)&fsl_netif.ip_addr.addr)[2],
            ((u8_t *)&fsl_netif.ip_addr.addr)[3]);
    }
    LOG_I("DHCP OK\r\n");
#endif /* FSL_FEATURE_SOC_ENET_COUNT > 0 */
#endif
}

#if defined(LPC_WIFI)

static void get_mwm_security(WIFISecurity_t security, char *security_ch)
{
    switch (security) {
    case eWiFiSecurityOpen:
        security_ch[0] = '0';
        break;
    case eWiFiSecurityWEP:
        security_ch[0] = '1';
        break;
    case eWiFiSecurityWPA:
        security_ch[0] = '3';
        break;
    case eWiFiSecurityWPA2:
        security_ch[0] = '4';
        break;
    case eWiFiSecurityWPA3:
        security_ch[0] = '9';
        break;
    default:
        LOG_W("Security mode not supported by the module, security set to eWiFiSecurityWPA2");
        security_ch[0] = '4';
        break;
    }
}

WIFIReturnCode_t network_wifi_init(void)
{
    int ret;
    ret = mwm_init();
    if (ret < 0) {
        LOG_E("Failed in initializing the Wi-Fi.\r\n");
        return eWiFiFailure;
    }

    return eWiFiSuccess;
}

WIFIReturnCode_t network_wifi_connect_ap(void)
{
    int ret;
    while (1) {
        /* get wlan status */
        ret = mwm_wlan_status();
        if (ret < 0) {
            LOG_E("Failed to get WLAN status:%d\r\n", ret);
            return eWiFiFailure;
        }
        else if (ret == MWM_INITIALIZED) {
            ret = mwm_set_param(MWM_MOD_WLAN, MWM_WLAN_SSID, clientcredentialWIFI_SSID);
            if (ret != 0) {
                LOG_E("Error in setting ssid:%d\r\n", ret);
                return eWiFiFailure;
            }
            char security[2] = {0};
            get_mwm_security(clientcredentialWIFI_SECURITY, &security[0]);
            ret = mwm_set_param(MWM_MOD_WLAN, "security", &security[0]);
            if (ret != 0) {
                LOG_E("Error in setting security:%d\r\n", ret);
                return eWiFiFailure;
            }

            ret = mwm_set_param(MWM_MOD_WLAN, MWM_WLAN_PASSPHRASE, clientcredentialWIFI_PASSWORD);
            if (ret != 0) {
                LOG_E("Error in setting passphrase:%d\r\n", ret);
                return eWiFiFailure;
            }
            /* start wlan */
            ret = mwm_wlan_start();
            if (ret != 0) {
                LOG_E("Error in starting wlan:%d\r\n", ret);
                return eWiFiFailure;
            }
        }
        else if (ret == MWM_CONNECTED) {
            char ssid[33]    = {0};
            char ip_addr[16] = {0};
            ret              = mwm_wlan_info(ssid, ip_addr);
            LOG_I("Wi-Fi is connected to: %s, IP Address: %s\r\n", ssid, ip_addr);
            break;
        }
        else if (ret == MWM_CONNECTING) {
            LOG_I("Wi-Fi is connecting...\r\n");
            vTaskDelay(pdMS_TO_TICKS(4000));
        }
        else if (ret == MWM_AUTH_FAILED) {
            LOG_E("Connection failed: Wi-Fi authentication failed.\r\n");
            return eWiFiFailure;
        }
        else if (ret == MWM_NETWORK_NOT_FOUND) {
            LOG_E("Connection failed: WLAN not found.\r\n");
            return eWiFiFailure;
        }
        else {
            LOG_E("\r\nError in getting the state from Wi-Fi chip.\r\n");
            return eWiFiFailure;
        }
    }

    return eWiFiSuccess;
}

#endif //LPC_WIFI

#if (SSS_HAVE_APPLET_A71CH || SSS_HAVE_APPLET_A71CH_SIM)

/*Set and Get the flag value from GP Storage */
int SetGetFlag_GPstorage(U32 *p_val, GpStorageMode_t mode, HLSE_OBJECT_HANDLE handle)
{
    int ret = 1;
    HLSE_RET_CODE hlseRc;
    HLSE_ATTRIBUTE attr;
    uint8_t tempBuffer[32] = {0};
    memcpy((void *)tempBuffer, (void *)p_val, sizeof(*p_val));

    attr.type     = HLSE_ATTR_OBJECT_VALUE;
    attr.value    = tempBuffer;
    attr.valueLen = 32;
    if (mode == SET) {
        hlseRc = HLSE_SetObjectAttribute(handle, &attr);
    }
    else if (mode == GET) {
        attr.value = tempBuffer;
        hlseRc     = HLSE_GetObjectAttribute(handle, &attr);
        if (hlseRc == HLSE_SW_OK) {
            memcpy((void *)p_val, (void *)tempBuffer, sizeof(*p_val));
        }
    }
    else {
        hlseRc = 0;
    }
    if (hlseRc == HLSE_SW_OK) {
        ret = 0;
    }

    return ret;
}

static int GetHandle_GPstorageType(HLSE_OBJECT_INDEX index, HLSE_OBJECT_TYPE objType)
{
    HLSE_RET_CODE hlseret;
    HLSE_OBJECT_HANDLE Handles[5];
    U16 HandlesNum = sizeof(Handles) / sizeof(HLSE_OBJECT_HANDLE);
    U16 HandlesNum_copy;
    U8 i    = 0;
    int ret = 1;

    hlseret = HLSE_EnumerateObjects(objType, Handles, &HandlesNum);
    if (hlseret == HLSE_SW_OK) {
        HandlesNum_copy = HandlesNum;
        while (HandlesNum_copy) {
            if (HLSE_GET_OBJECT_INDEX(Handles[i]) == index) {
                Gpstorage_handle = Handles[i];
                ret              = 0;
                break;
            }
            i++;
            HandlesNum_copy--;
        }
    }
    return ret;
}

/*Get the Handle of the GP storage */
int GetHandle_GPstorage(HLSE_OBJECT_INDEX index)
{
    /* New way. 3rd entry in table would be HLSE_DATA  */
    int ret = GetHandle_GPstorageType(index, HLSE_DATA);

    if (ret == 1) {
        /* iMX UL Way. No entry. So insert 3rd entry in table. */
        HLSE_OBJECT_TYPE objType = HLSE_DATA;
        U16 templateSize         = 3;
        HLSE_ATTRIBUTE attr[3];
        U8 objData[]     = {0xFF, 0xFF, 0xFF, 0xFF};
        attr[0].type     = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value    = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type     = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value    = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type     = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value    = &objData;
        attr[2].valueLen = 4;

        ret = HLSE_CreateObject(attr, templateSize, &Gpstorage_handle);
    }
    return ret;
}

#endif

#endif /* USE_RTOS */
