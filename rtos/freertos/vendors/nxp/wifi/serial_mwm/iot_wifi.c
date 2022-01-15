/*
 * FreeRTOS Wi-Fi V1.0.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright 2020 NXP
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file iot_wifi.c
 * @brief Wi-Fi Interface.
 */

/* Socket and Wi-Fi interface includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "iot_wifi.h"

/* Wi-Fi configuration includes. */
#include "aws_wifi_config.h"

/* Serial MWM */
#include "serial_mwm.h"
#include "fsl_debug_console.h"

/*-----------------------------------------------------------*/

static int wifi_reset_prov_mode()
{
    int ret;
    int c;

    PRINTF("\r\nDo you want to reset the Wi-Fi module to provisioning mode?\r\n");
    PRINTF("Press 'y' to reset Wi-Fi module.\r\n");
    c = GETCHAR();
    PRINTF("\r\n");
    if (c == 'y')
    {
        PRINTF("Resetting Wi-Fi module...\r\n\r\n");
        ret = mwm_wlan_prov();
        if (ret < 0)
        {
            PRINTF("Wi-Fi reset failed!\r\n");
            return -1;
        }
        vTaskDelay(pdMS_TO_TICKS(6000));
    }

    return 0;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_On(void)
{
    int ret;

    ret = mwm_init();
    if (ret < 0)
    {
        PRINTF("Could not initialize Serial MWM\r\n");
        return eWiFiFailure;
    }

    /* get wlan status */
    ret = mwm_wlan_status();
    if (ret < 0)
    {
        PRINTF("Failed to get WLAN status.\r\n");
        return eWiFiFailure;
    }

    if (ret == MWM_INITIALIZED)
    {
        /* start wlan */
        ret = mwm_wlan_start();
        if (ret < 0)
        {
            PRINTF("Could not start WLAN subsystem.\r\n");
            return eWiFiFailure;
        }
    }

    return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Off(void)
{
    /* FIX ME. */
    return eWiFiFailure;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_ConnectAP(const WIFINetworkParams_t *const pxNetworkParams)
{
    (void)pxNetworkParams;
    int ret;

    while (1)
    {
        /* get wlan status */
        ret = mwm_wlan_status();
        if (ret < 0)
        {
            PRINTF("Failed to get WLAN status.\r\n");
            return eWiFiFailure;
        }
        else if (ret == MWM_CONNECTED)
        {
            char ssid[33]    = {0};
            char ip_addr[16] = {0};
            ret              = mwm_wlan_info(ssid, ip_addr);
            PRINTF("Wi-Fi is connected to: %s, IP Address: %s\r\n", ssid, ip_addr);
            PRINTF(
                "\r\nTo reset Wi-Fi module to provisioning mode connect to WLAN: %s, open address: http://%s in web "
                "browser and select \"Reset to Provisioning\".\r\n",
                ssid, ip_addr);
            PRINTF("Then restart this application.\r\n\r\n");
            break;
        }
        else if (ret == MWM_CONNECTING)
        {
            PRINTF("Wi-Fi is connecting...\r\n");
            vTaskDelay(pdMS_TO_TICKS(4000));
        }
        else if (ret == MWM_AUTH_FAILED)
        {
            PRINTF("Connection failed: Wi-Fi authentication failed.\r\n");
            ret = wifi_reset_prov_mode();
            if (ret < 0)
            {
                return eWiFiFailure;
            }
        }
        else if (ret == MWM_NETWORK_NOT_FOUND)
        {
            PRINTF("Connection failed: WLAN not found.\r\n");
            ret = wifi_reset_prov_mode();
            if (ret < 0)
            {
                return eWiFiFailure;
            }
        }
        else
        {
            PRINTF("\r\nWi-Fi module is not configured for WLAN connection.\r\n");
            PRINTF(
                "Connect to Wi-Fi AP: Serial2Wifi, open address: http://192.168.10.1 in web browser and configure WLAN "
                "connection.\r\n");
            PRINTF("Press any key to continue.\r\n");
            GETCHAR();
            PRINTF("\r\n");
        }
    }

    return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Disconnect(void)
{
    /* FIX ME. */
    return eWiFiFailure;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Reset(void)
{
    /* FIX ME. */
    return eWiFiFailure;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Scan(WIFIScanResult_t *pxBuffer, uint8_t ucNumNetworks)
{
    /* FIX ME. */
    return eWiFiFailure;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_SetMode(WIFIDeviceMode_t xDeviceMode)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetMode(WIFIDeviceMode_t *pxDeviceMode)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkAdd(const WIFINetworkProfile_t *const pxNetworkProfile, uint16_t *pusIndex)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkGet(WIFINetworkProfile_t *pxNetworkProfile, uint16_t usIndex)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkDelete(uint16_t usIndex)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Ping(uint8_t *pucIPAddr, uint16_t usCount, uint32_t ulIntervalMS)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetIP(uint8_t *pucIPAddr)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetMAC(uint8_t *pucMac)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetHostIP(char *pcHost, uint8_t *pucIPAddr)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_StartAP(void)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_StopAP(void)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_ConfigureAP(const WIFINetworkParams_t *const pxNetworkParams)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_SetPMMode(WIFIPMMode_t xPMModeType, const void *pvOptionValue)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetPMMode(WIFIPMMode_t *pxPMModeType, void *pvOptionValue)
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

BaseType_t WIFI_IsConnected( const WIFINetworkParams_t * pxNetworkParams )
{
    /* FIX ME. */
    return pdFALSE;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_RegisterEvent( WIFIEventType_t xEventType,
                                     WIFIEventHandler_t xHandler )
{
    /** Needs to implement dispatching network state change events **/
    return eWiFiNotSupported;
}