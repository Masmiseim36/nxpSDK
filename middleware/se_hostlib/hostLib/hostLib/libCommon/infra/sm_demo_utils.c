/**
 * @file sm_demo_utils.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017,2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
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
#include "ksdk_mbedtls.h"
#include "nxLog_App.h"

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"

//#include "aws_clientcredential.h"

#if defined(LPC_WIFI)
#   include "iot_wifi.h"
#   include "wifi_config.h"

#elif defined(LPC_ENET)
#   include "lwip/opt.h"
#   include "lwip/tcpip.h"
#   include "lwip/dhcp.h"
#   include "lwip/prot/dhcp.h"
#   include "netif/ethernet.h"
#   include "enet_ethernetif.h"
#   include "lwip/netifapi.h"
#endif

#include "HLSEAPI.h"
#include "sm_demo_utils.h"
#include "fsl_debug_console.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(LPC_ENET)
/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x04, 0x12, 0x13, 0xB1, 0x11, 0x90 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk

/* Facilitate a simple hash for unique MAC Address based on an input 18 byte UID */
#define MAC_HASH(N)  \
    fsl_enet_config0.macAddress[N] = buffer[(N+2)+(5*0)] ^ buffer[(N+2)+(5*1)] ^ buffer[(N+2)+(5*2)]

/*******************************************************************************
 * Static variables
 ******************************************************************************/
static struct netif fsl_netif0;

#else // LPC_ENET
    uint8_t Wifi_IP[4] = {0};
#endif // LPC_ENET

/*******************************************************************************
 * Global variables
 ******************************************************************************/
#if (SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH)
HLSE_OBJECT_HANDLE Gpstorage_handle;
#endif

/*******************************************************************************
 * Global Function Definitions
 ******************************************************************************/

/*Init the board network */
void BOARD_InitNetwork_MAC(const unsigned char buffer[18])
{
#if defined(LPC_WIFI)

    const WIFINetworkParams_t pxNetworkParams = {
        .pcSSID = clientcredentialWIFI_SSID,
        .pcPassword = clientcredentialWIFI_PASSWORD,
        .xSecurity = clientcredentialWIFI_SECURITY,
    };
    WIFIReturnCode_t result;
    LOG_I("Turning WIFI ON\r\n");
    result = WIFI_On();
    assert(eWiFiSuccess == result);

    LOG_I("Connecting to network:%s status:0x%x\r\n",clientcredentialWIFI_SSID,result);
    result = WIFI_ConnectAP(&pxNetworkParams);
    assert(eWiFiSuccess == result);

    LOG_I("Getting the IP address of Connected Network\r\n");
    result = WIFI_GetIP(Wifi_IP);
    assert(eWiFiSuccess == result);
    LOG_I("\r\n IPv4 Address     : %u.%u.%u.%u\r\n", Wifi_IP[0],Wifi_IP[1],Wifi_IP[2],Wifi_IP[3]);

#elif defined(LPC_ENET)
#if FSL_FEATURE_SOC_ENET_COUNT > 0  || FSL_FEATURE_SOC_LPC_ENET_COUNT > 0

    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
    ethernetif_config_t fsl_enet_config0 = {
        .phyAddress = EXAMPLE_PHY_ADDRESS, .clockName = EXAMPLE_CLOCK_NAME, .macAddress = configMAC_ADDR,
    };
    if (buffer != NULL)
    {
        MAC_HASH(1);
        MAC_HASH(2);
        MAC_HASH(3);
        MAC_HASH(4);
        MAC_HASH(5);
    }

    LOG_I("Connecting to network\r\n");
    tcpip_init(NULL, NULL);

    IP4_ADDR(&fsl_netif0_ipaddr, 0U, 0U, 0U, 0U);
    IP4_ADDR(&fsl_netif0_netmask, 0U, 0U, 0U, 0U);
    IP4_ADDR(&fsl_netif0_gw, 0U, 0U, 0U, 0U);

    netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, &fsl_enet_config0, ethernetif0_init,
              tcpip_input);
    netif_set_default(&fsl_netif0);
    netif_set_up(&fsl_netif0);
//    netifapi_netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, &fsl_enet_config0,
//                       ethernetif0_init, tcpip_input);
//    netifapi_netif_set_default(&fsl_netif0);
//    netifapi_netif_set_up(&fsl_netif0);

    LOG_I("Getting IP address from DHCP ...\n");
    dhcp_start(&fsl_netif0);

    struct dhcp *dhcp;
    dhcp = (struct dhcp *)netif_get_client_data(&fsl_netif0, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

    while (dhcp->state != DHCP_STATE_BOUND)
    {
        vTaskDelay(1000);
    }

    if (dhcp->state == DHCP_STATE_BOUND)
    {
        LOG_I("\r\n IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *) &fsl_netif0.ip_addr.addr)[0],
            ((u8_t *) &fsl_netif0.ip_addr.addr)[1], ((u8_t *) &fsl_netif0.ip_addr.addr)[2],
            ((u8_t *) &fsl_netif0.ip_addr.addr)[3]);
    }
    LOG_I("DHCP OK\r\n");
#endif /* FSL_FEATURE_SOC_ENET_COUNT > 0 */
#endif
}

#if (SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH)

/*Set and Get the flag value from GP Storage */
int SetGetFlag_GPstorage(U32 *p_val, GpStorageMode_t mode, HLSE_OBJECT_HANDLE handle)
{
    int ret = 1;
    HLSE_RET_CODE hlseRc;
    HLSE_ATTRIBUTE attr;
    uint8_t tempBuffer[32] = {0};
    memcpy((void *)tempBuffer,(void *)p_val,sizeof(*p_val));

    attr.type = HLSE_ATTR_OBJECT_VALUE;
    attr.value = tempBuffer;
    attr.valueLen = 32;
    if (mode == SET)
    {
        hlseRc = HLSE_SetObjectAttribute(handle, &attr);
    }
    else if (mode == GET)
    {
        attr.value = tempBuffer;
        hlseRc = HLSE_GetObjectAttribute(handle, &attr);
        if (hlseRc == HLSE_SW_OK)
        {
            memcpy((void *)p_val,(void *)tempBuffer,sizeof(*p_val));
        }
    }
    else
    {
        hlseRc = 0;
    }
    if (hlseRc == HLSE_SW_OK)
    {
        ret = 0;
    }

    return ret;
}

static int GetHandle_GPstorageType(HLSE_OBJECT_INDEX index, HLSE_OBJECT_TYPE objType) {
    HLSE_RET_CODE hlseret;
    HLSE_OBJECT_HANDLE Handles[5];
    U16 HandlesNum = sizeof(Handles) / sizeof(HLSE_OBJECT_HANDLE);
    U16 HandlesNum_copy;
    U8 i = 0;
    int ret = 1;

    hlseret = HLSE_EnumerateObjects(objType, Handles, &HandlesNum);
    if (hlseret == HLSE_SW_OK)
    {
        HandlesNum_copy = HandlesNum;
        while(HandlesNum_copy)
        {
            if (HLSE_GET_OBJECT_INDEX(Handles[i]) == index)
            {
                Gpstorage_handle = Handles[i];
                ret = 0;
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

    if ( ret == 1 )
    {
        /* iMX UL Way. No entry. So insert 3rd entry in table. */
        HLSE_OBJECT_TYPE objType = HLSE_DATA;
        U16 templateSize = 3;
        HLSE_ATTRIBUTE attr[3];
        U8  objData[] = {0xFF,0xFF,0xFF,0xFF};
        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = &objData;
        attr[2].valueLen = 4;

        ret = HLSE_CreateObject(attr, templateSize, &Gpstorage_handle);
    }
    return ret;
}

#endif

/*JSON utility function to check equality */
int8_t jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if(tok->type == JSMN_STRING) {
        if((int) strlen(s) == tok->end - tok->start) {
            if(strncmp(json + tok->start, s, (size_t) (tok->end - tok->start)) == 0) {
                return 0;
            }
        }
    }
    return -1;
}

#endif /* USE_RTOS */
