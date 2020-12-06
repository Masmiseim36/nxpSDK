/*
 * Amazon FreeRTOS Wi-Fi for LPC54018 IoT Module V1.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

/*
 * Copyright 2020 NXP
 */

/**
 * @file iot_wifi.c
 * @brief Wi-Fi Interface.
 */

#include "lwip/ip_addr.h"

#include <stdio.h>
#include "FreeRTOS.h"

#include "iot_wifi.h"
#include "wwd.h"
#include "wwd_wiced.h"
#include "wwd_network.h"
#include "wwd_constants.h"
#include "lwip/netifapi.h"
#include "lwip/tcpip.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/prot/dhcp.h"

#include "network_connection.h"

#include "device_utils.h"
#include "wifi_credentials.h"

#ifndef WIFI_CONNECT_TIMEOUT_MS
#define WIFI_CONNECT_TIMEOUT_MS (10000)
#endif

#ifndef WIFI_DHCP_DELAY_MS
#define WIFI_DHCP_DELAY_MS (500)
#endif

#ifndef WIFI_DNS_TIMEOUT_MS
#define WIFI_DNS_TIMEOUT_MS (10000)
#endif

#ifndef WIFI_SCAN_TIMEOUT_MS
#define WIFI_SCAN_TIMEOUT_MS (10000)
#endif

typedef struct _wiced_scan_buf
{
    WIFIScanResult_t *pxBuffer;
    uint8_t uxNumNetworks;
    uint16_t result_buff_num;
} wiced_scan_buf;

extern wiced_result_t wiced_wlan_connectivity_init(void);
extern void etharp_cleanup_netif(struct netif *netif);
extern err_t dns_gethostbyname(const char *hostname, ip_addr_t *addr, dns_found_callback found, void *callback_arg);

/* Protect API */
static SemaphoreHandle_t g_api_mutex = NULL;
static SemaphoreHandle_t g_api_sema  = NULL;

/*
 * Some of the variables below are currently unused. This may change in later iterations.
 */
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

/* DNS result IP */
static ip_addr_t g_host_ip = {0};

/* WiFi interface is on */
static volatile uint8_t g_wifi_on = 0;

/* Initialize lwIP only once */
static volatile uint8_t g_lwip_is_ready = 0;

/* WiFi is connected to AP, mutex protected write */
static volatile uint8_t g_ap_connected = 0;

/* IP addresses */
static ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;

/* Network fface */
static struct netif fsl_netif0;

/* Temporary SSID */
static wiced_ssid_t ssid = {0};

#pragma GCC diagnostic pop
#endif /* GNUC pragma for Unused Variables */

/*
 * Some of the functions below are currently unused. This may change in later iterations.
 */
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static int wifi_is_enabled(void)
{
    return g_wifi_on;
}

static void wifi_set_enabled(int value)
{
    g_wifi_on = !!value;
}

static int wifi_is_connected(void)
{
    return g_wifi_on && g_ap_connected;
}

static void wifi_set_connected(int value)
{
    g_ap_connected = !!value;
}

static wiced_security_t conv_security_to_wiced(WIFISecurity_t api_sec)
{
    switch (api_sec)
    {
        case eWiFiSecurityOpen:
            return WICED_SECURITY_OPEN;
        case eWiFiSecurityWEP:
            return WICED_SECURITY_WEP_PSK;
        case eWiFiSecurityWPA:
            return WICED_SECURITY_WPA_MIXED_PSK;
        case eWiFiSecurityWPA2:
            return WICED_SECURITY_WPA2_MIXED_PSK;
        default:
            return WICED_SECURITY_UNKNOWN;
    }
}

static WIFISecurity_t conv_security_from_wiced(wiced_security_t api_sec)
{
    switch (api_sec)
    {
        case WICED_SECURITY_OPEN:
            return eWiFiSecurityOpen;
        case WICED_SECURITY_WEP_PSK:
            return eWiFiSecurityWEP;
        case WICED_SECURITY_WPA_MIXED_PSK:
            return eWiFiSecurityWPA;
        case WICED_SECURITY_WPA2_MIXED_PSK:
            return eWiFiSecurityWPA2;
        default:
            return eWiFiSecurityNotSupported;
    }
}

#pragma GCC diagnostic pop
#endif /* GNUC pragma for Unused Functions */

void wiced_scan_results_handler(wiced_scan_result_t **result_ptr, void *user_data, wiced_scan_status_t status)
{
    wiced_scan_buf *scan_buf = (wiced_scan_buf *)user_data;

    if (result_ptr == NULL)
    {
        /* finished */
        scan_buf->pxBuffer[scan_buf->result_buff_num].cChannel = 0xff; // TODO: check
        xSemaphoreGive(g_api_sema);
        WPRINT_WWD_INFO(("scan completed\r\n"));
        return;
    }

    if (scan_buf->result_buff_num >= scan_buf->uxNumNetworks)
    {
        WPRINT_WWD_INFO(("scan result overflow %d vs %d \r\n", scan_buf->result_buff_num, scan_buf->uxNumNetworks));
        return;
    }

    strncpy((char *)scan_buf->pxBuffer[scan_buf->result_buff_num].cSSID, (char const *)result_ptr[0]->SSID.value,
            wificonfigMAX_SSID_LEN);
    memcpy((char *)scan_buf->pxBuffer[scan_buf->result_buff_num].ucBSSID, (char const *)result_ptr[0]->BSSID.octet,
           wificonfigMAX_BSSID_LEN);

    scan_buf->pxBuffer[scan_buf->result_buff_num].cRSSI     = result_ptr[0]->signal_strength;
    scan_buf->pxBuffer[scan_buf->result_buff_num].cChannel  = result_ptr[0]->channel;
    scan_buf->pxBuffer[scan_buf->result_buff_num].ucHidden  = 0;
    scan_buf->pxBuffer[scan_buf->result_buff_num].xSecurity = conv_security_from_wiced(result_ptr[0]->security);

    scan_buf->result_buff_num++;
}

/**
 * @brief Initializes the Wi-Fi module.
 *
 * This function must be called exactly once before any other
 * Wi-Fi functions (including socket functions) can be used.
 *
 * @return eWiFiSuccess if everything succeeds, eWiFiFailure otherwise.
 */
WIFIReturnCode_t WIFI_On(void)
{
    APP_NETWORK_Init();

    return eWiFiSuccess;
}

WIFIReturnCode_t WIFI_Off(void)
{
    APP_NETWORK_Uninit();

    return eWiFiSuccess;
}

WIFIReturnCode_t WIFI_DisconnectInternal(void)
{
    APP_NETWORK_Uninit();
    return eWiFiSuccess;
}

WIFIReturnCode_t WIFI_ConnectAP(const WIFINetworkParams_t *const pxNetworkParams)
{
    WIFIReturnCode_t result = eWiFiSuccess;
    status_t status         = 0;

    /* Connect to WiFi network */
    status = APP_NETWORK_Wifi_Connect(true, true);

    if (0 != status)
    {
        if (status == WWD_WLAN_WLAN_DOWN)
        {
            APP_NETWORK_Uninit();
        }

        result = eWiFiFailure;
    }

    return result;
}

/**
 * @brief Disconnects from Access Point.
 *
 * @param[in] None.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_Disconnect(void)
{
    WIFIReturnCode_t result = eWiFiFailure;

    /* WiFi not turned ON */
    if (!wifi_is_enabled())
        return eWiFiFailure;

    if (xSemaphoreTake(g_api_mutex, portMAX_DELAY) == pdTRUE)
    {
        /* If connected - disconnect */
        if (wifi_is_connected())
        {
            result = WIFI_DisconnectInternal();
        }
        else
        {
            result = eWiFiSuccess;
        }

        /* Release semaphore */
        xSemaphoreGive(g_api_mutex);
    }

    return result;
}

/**
 * @brief Resets the Wi-Fi Module.
 *
 * @param[in] None.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_Reset(void)
{
    return eWiFiNotSupported;
}

/**
 * @brief Sets Wi-Fi mode.
 *
 * @param[in] xDeviceMode - Mode of the device Station / Access Point /P2P.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_SetMode(WIFIDeviceMode_t xDeviceMode)
{
    return eWiFiNotSupported;
}

/**
 * @brief Gets Wi-Fi mode.
 *
 * @param[in] pxDeviceMode - return mode Station / Access Point /P2P
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_GetMode(WIFIDeviceMode_t *pxDeviceMode)
{
    return eWiFiNotSupported;
}

/**
 * @brief Wi-Fi Add Network profile.
 *
 * Adds Wi-Fi network to network list in non-volatile memory
 *
 * @param[in] pxNetworkProfile - network profile parameters
 * @param[out] pusIndex - network profile index
 *
 * @return Profile stored index on success, or negative error code on failure..
 *
 * @see WIFINetworkParams_t
 */
WIFIReturnCode_t WIFI_NetworkAdd(const WIFINetworkProfile_t *const pxNetworkProfile, uint16_t *pusIndex)
{
    return eWiFiNotSupported;
}

/**
 * @brief Wi-Fi Get Network profile .
 *
 * Gets Wi-Fi network params at given index from network list in non-volatile memory
 *
 * @param[out] pxNetworkProfile - pointer to return network profile parameters
 * @param[in] usIndex - Index of the network profile, must be between 0 to wificonfigMAX_NETWORK_PROFILES
 * @return eWiFiSuccess if success, eWiFiFailure otherwise.
 *
 * @see WIFINetworkParams_t
 */
WIFIReturnCode_t WIFI_NetworkGet(WIFINetworkProfile_t *pxNetworkProfile, uint16_t uxIndex)
{
    return eWiFiNotSupported;
}

/**
 * @brief Wi-Fi Delete Network profile .
 *
 * Deletes Wi-Fi network from network list at given index in non-volatile memory
 *
 * @param[in] usIndex - Index of the network profile, must be between 0 to wificonfigMAX_NETWORK_PROFILES
 *                      wificonfigMAX_NETWORK_PROFILES as index will delete all network profiles
 *
 * @return eWiFiSuccess if deleted, eWiFiFailure otherwise.
 *
 */
WIFIReturnCode_t WIFI_NetworkDelete(uint16_t uxIndex)
{
    return eWiFiNotSupported;
}

/**
 * @brief Ping an IP address in the network.
 *
 * @param[in] IP Address to ping.
 * @param[in] Number of times to ping
 * @param[in] Interval in mili seconds for ping operation
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_Ping(uint8_t *pxIPAddr, uint16_t xCount, uint32_t xIntervalMS)
{
    return eWiFiNotSupported;
}

/**
 * @brief Retrieves the Wi-Fi interface's IP address.
 *
 * @param[in] IP Address buffer.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_GetIP(uint8_t *pxIPAddr)
{
    return eWiFiNotSupported;
}

/**
 * @brief Retrieves the Wi-Fi interface's MAC address.
 *
 * @param[in] MAC Address buffer.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_GetMAC(uint8_t *pxMac)
{
    wiced_mac_t mac = {0};

    /* Check params */
    if (NULL == pxMac)
        return eWiFiFailure;

    /*TODO: ?? */
    if (!wifi_is_enabled())
        return eWiFiFailure;

    if (WWD_SUCCESS != wwd_wifi_get_mac_address(&mac, WWD_STA_INTERFACE))
        return eWiFiFailure;

    strncpy((char *)pxMac, (const char *)mac.octet, sizeof(mac.octet));
    return eWiFiSuccess;
}

/**
 * @brief Retrieves host IP address from URL using DNS
 *
 * @param[in] pxHost - Host URL.
 * @param[in] pxIPAddr - IP Address buffer.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_GetHostIP(char *pxHost, uint8_t *pxIPAddr)
{
    return eWiFiNotSupported;
}

/**
 * @brief Retrieves IP address in Access Point mode
 *
 * @param[in] pxIPAddr - IP Address buffer.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_GetAccessPointIP(uint8_t *pxIPAddr)
{
    return eWiFiNotSupported;
}

/**
 * @brief Perform WiF Scan
 *
 * @param[in] pxBuffer - Buffer for scan results.
 * @param[in] uxNumNetworks - Number of networks in scan result.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_Scan(WIFIScanResult_t *pxBuffer, uint8_t uxNumNetworks)
{
    return eWiFiNotSupported;
}

/**
 * @brief Configure SoftAP
 *
 * @param[in] pxNetworkParams - Network params to configure AP.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_ConfigureAP(const WIFINetworkParams_t *const pxNetworkParams)
{
    return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_SetPMMode(WIFIPMMode_t xPMModeType, const void *pvOptionValue)
{
    return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_GetPMMode(WIFIPMMode_t *pxPMModeType, void *pvOptionValue)
{
    return eWiFiNotSupported;
}

BaseType_t WIFI_IsConnected(void)
{
    // TODO: assert
    return wwd_wifi_sta_is_only_connected() == WICED_TRUE;
}

WIFIReturnCode_t WIFI_RegisterNetworkStateChangeEventCallback(IotNetworkStateChangeEventCallback_t xCallback)
{
    /** Needs to implement dispatching network state change events **/
    return eWiFiNotSupported;
}
