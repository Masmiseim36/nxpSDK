/*
 * Amazon FreeRTOS Wi-Fi for NXP54018_IoT_Module V1.0.1
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

/**
 * @file aws_WIFI.c
 * @brief WiFi Interface.
 */

#include <stdio.h>
#include "aws_wifi.h"
#include "wifi_common.h"
#include "qcom_api.h"
#include "atheros_wifi.h"

/* This is here because the maximum DNS name length is defined in aws_secure_sockets.h.
 * Wifi must not have a dependency on aws_secure_sockets.h
 */
#define wifiMAX_DNS_NAME_LENGTH 253

/* Only 1 WiFi module is present at the time */
static uint8_t g_devid = 0;

/* NOTE: Could be located on stack */
static QCOM_SSID g_ssid = {0};

/* NOTE: Could be located on stack */
static QCOM_PASSPHRASE g_passphr = {0};

/* Semaphore for connection */
static SemaphoreHandle_t g_connect_semaph;

/* Protect API */
static SemaphoreHandle_t g_wifi_semaph;

/* WiFi/HW hardware configuration */
QCA_CONTEXT_STRUCT g_wifi_ctx = {0};

extern const QCA_MAC_IF_STRUCT ATHEROS_WIFI_IF;

/* WiFi interface object */
const QCA_IF_STRUCT g_wifi_if = {
    .MAC_IF         = &ATHEROS_WIFI_IF,
    .MAC_NUMBER     = 0,
    .PHY_NUMBER     = 0,
    .PHY_ADDRESS    = 0,
};

/* WiFi params */
const QCA_PARAM_STRUCT g_wifi_params = {
    .QCA_IF         = &g_wifi_if,
    .MODE           = Auto_Negotiate,
    .OPTIONS        = 0,
    .NUM_RX_PCBS    = WLAN_CONFIG_NUM_PRE_ALLOC_RX_BUFFERS,
};

/* Singleton, provides WiFi context structure */
QCA_CONTEXT_STRUCT *wlan_get_context(void)
{
    return &g_wifi_ctx;
}

static void ip_to_pxIPAddr(uint32_t ip, uint8_t *pxIPAddr)
{
    pxIPAddr[0] = (uint8_t)(ip >> 24);
    pxIPAddr[1] = (uint8_t)(ip >> 16);
    pxIPAddr[2] = (uint8_t)(ip >> 8);
    pxIPAddr[3] = (uint8_t)(ip);
}

static void pxIPAddr_to_ip(uint8_t *pxIPAddr, uint32_t *ip32)
{
    *ip32 = (((pxIPAddr[0]) & 0xFF) << 24) | \
            (((pxIPAddr[1]) & 0xFF) << 16) | \
            (((pxIPAddr[2]) & 0xFF) << 8) | \
            (((pxIPAddr[3]) & 0xFF));
}

/* Invoked from 'driver_task', on SUCCESS post semaphore */
static void aws_connect_cb(QCOM_ONCONNECT_EVENT event, uint8_t devid, QCOM_BSSID bssid, boolean bss_conn)
{
    BaseType_t result = pdFALSE;
    (void)result;
    if (QCOM_ONCONNECT_EVENT_SUCCESS == event)
    {
        result = xSemaphoreGive(g_connect_semaph);
    }
}

static WIFIReturnCode_t conv_security_to_qcom(WIFISecurity_t api_sec, WLAN_AUTH_MODE *qcom_auth, WLAN_CRYPT_TYPE *qcom_crypt)
{
    switch (api_sec)
    {
        case eWiFiSecurityOpen:
            *qcom_crypt = WLAN_CRYPT_NONE;
            *qcom_auth = WLAN_AUTH_NONE;
            break;
        case eWiFiSecurityWEP:
            *qcom_crypt = WLAN_CRYPT_WEP_CRYPT;
            *qcom_auth = WLAN_AUTH_WEP;
            break;
        case eWiFiSecurityWPA:
            // *qcom_crypt = WLAN_CRYPT_TKIP_CRYPT;
            *qcom_crypt = WLAN_CRYPT_AES_CRYPT;
            *qcom_auth = WLAN_AUTH_WPA_PSK;
            break;
        case eWiFiSecurityWPA2:
            // *qcom_crypt = WLAN_CRYPT_TKIP_CRYPT;
            *qcom_crypt = WLAN_CRYPT_AES_CRYPT;
            *qcom_auth = WLAN_AUTH_WPA2_PSK;
            break;
        default:
            return eWiFiFailure;
    }
    return eWiFiSuccess;
}

static WIFIReturnCode_t conv_qcom_to_mode(QCOM_WLAN_DEV_MODE dev_mode, WIFIDeviceMode_t *pxDeviceMode)
{
    switch (dev_mode)
    {
        case QCOM_WLAN_DEV_MODE_STATION:
            *pxDeviceMode = eWiFiModeStation;
            break;
        case QCOM_WLAN_DEV_MODE_AP:
            *pxDeviceMode = eWiFiModeAP;
            break;
        case QCOM_WLAN_DEV_MODE_ADHOC:
            *pxDeviceMode = eWiFiModeP2P;
            break;
        default:
            return eWiFiFailure;
    }
    return eWiFiSuccess;
}

static WIFIReturnCode_t conv_mode_to_qcom(WIFIDeviceMode_t xDeviceMode, QCOM_WLAN_DEV_MODE *dev_mode)
{
    switch (xDeviceMode)
    {
        case eWiFiModeStation:
            *dev_mode = QCOM_WLAN_DEV_MODE_STATION;
            break;
        case eWiFiModeAP:
            *dev_mode = QCOM_WLAN_DEV_MODE_AP;
            break;
        case eWiFiModeP2P:
            *dev_mode = QCOM_WLAN_DEV_MODE_ADHOC;
            break;
        default:
            return eWiFiFailure;
    }
    return eWiFiSuccess;
}

/**
 * @brief Initializes the WiFi module.
 *
 * This function must be called exactly once before any other
 * WiFi functions (including socket functions) can be used.
 *
 * @return eWiFiSuccess if everything succeeds, eWiFiFailure otherwise.
 */
WIFIReturnCode_t WIFI_On( void )
{
    A_STATUS result;

    /* Initialize WIFI shield */
    result = (A_STATUS)WIFISHIELD_Init();
    if (A_OK != result)
        return eWiFiFailure;

    /* Power off the WLAN and wait 30ms */
    CUSTOM_HW_POWER_UP_DOWN(NULL, false);
    vTaskDelay(MSEC_TO_TICK(30));

    g_wifi_ctx.PARAM_PTR = &g_wifi_params;
    if (A_OK != ATHEROS_WIFI_IF.INIT(&g_wifi_ctx))
        return eWiFiFailure;

    /* Disable low power mode to avoid SPI bus flood */
    qcom_power_set_mode(0, MAX_PERF_POWER, USER);

    /* Create a on_connect semaphore, */
    g_wifi_semaph = xSemaphoreCreateBinary();
    if (NULL == g_wifi_semaph)
        return eWiFiFailure;
    xSemaphoreGive(g_wifi_semaph);

    /* Create a on_connect semaphore, */
    g_connect_semaph = xSemaphoreCreateBinary();
    if (NULL == g_connect_semaph)
        return eWiFiFailure;

    /* Wait for Wifi */
    vTaskDelay(MSEC_TO_TICK(100));

    return eWiFiSuccess;
}

WIFIReturnCode_t WIFI_Off( void )
{
	return eWiFiNotSupported;
}

/**
 * @brief Connects to Access Point.
 *
 * @param[in] pxJoinAPParams Configuration to join AP.
 *
 * @return eWiFiSuccess if connection is successful, eWiFiFailure otherwise.
 */
WIFIReturnCode_t WIFI_ConnectAP( const WIFINetworkParams_t * const pxNetworkParams )
{
    WLAN_AUTH_MODE auth_mode;
    WLAN_CRYPT_TYPE crypt_type;
    WIFIReturnCode_t status = eWiFiFailure;
    const TickType_t xTimeout = pdMS_TO_TICKS( 20000UL );

    /* Check params */
    if (NULL == pxNetworkParams || NULL == pxNetworkParams->pcSSID || NULL == pxNetworkParams->pcPassword)
        return eWiFiFailure;

    /* Acquire semaphore */
    if (xSemaphoreTake(g_wifi_semaph, portMAX_DELAY) == pdTRUE)
    {
        do {
            /* Set WiFi to device mode */
            if (A_OK != (A_STATUS)qcom_op_set_mode(g_devid, QCOM_WLAN_DEV_MODE_STATION))
                break;

            /* Set SSID, must be done before auth, cipher and passphrase */
            strncpy(g_ssid.ssid, pxNetworkParams->pcSSID, sizeof(g_ssid));
            if (A_OK != (A_STATUS)qcom_set_ssid(g_devid, &g_ssid))
                break;

            /* Convert 'WIFISecurity_t' to 'WLAN_AUTH_MODE', 'WLAN_CRYPT_TYPE' */
            if (eWiFiSuccess != conv_security_to_qcom(pxNetworkParams->xSecurity, &auth_mode, &crypt_type))
                break;

            /* Set encyption mode */
            if (A_OK != (A_STATUS)qcom_sec_set_encrypt_mode(g_devid, crypt_type))
                break;

            /* Set auth mode */
            if (A_OK != qcom_sec_set_auth_mode(g_devid, auth_mode))
                break;

            /* Set passphrase */
            strncpy(g_passphr.passphrase, pxNetworkParams->pcPassword, sizeof(g_passphr));
            if (A_OK != qcom_sec_set_passphrase(g_devid, &g_passphr))
                break;

            /* Set channel */
            if (0 != pxNetworkParams->cChannel)
            {
                if (A_OK != qcom_set_channel(g_devid, pxNetworkParams->cChannel))
                  break;
            }

            /* Set connect_callback */
            if (A_OK != qcom_set_connect_callback(g_devid, (void *)aws_connect_cb))
                break;

            /* Commit settings to WiFi module */
            if (A_OK != qcom_commit(g_devid))
                break;

            /* Wait for callback, that is invoked from 'driver_task' context */
            if (pdTRUE != xSemaphoreTake(g_connect_semaph, xTimeout))
                break;

            /* Remove callback ?? */
            if (A_OK != qcom_set_connect_callback(g_devid, NULL))
                break;

            /* Everything is OK */
            status = eWiFiSuccess;
        } while (0);

        /* Release semaphore */
        xSemaphoreGive(g_wifi_semaph);
    }
    return status;
}

/**
 * @brief Disconnects from Access Point.
 *
 * @param[in] None.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_Disconnect( void )
{
    //TODO: vSemaphoreDelete
    //      wait for disconnect cb ??
    WIFIReturnCode_t status = eWiFiFailure;

    /* Acquire semaphore */
    if (xSemaphoreTake(g_wifi_semaph, portMAX_DELAY) == pdTRUE)
    {
        if (A_OK == qcom_disconnect(g_devid))
            status = eWiFiSuccess;
        /* Release semaphore */
        xSemaphoreGive(g_wifi_semaph);
    }
    return status;
}

/**
 * @brief Resets the WiFi Module.
 *
 * @param[in] None.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_Reset( void )
{
    return eWiFiNotSupported;
}

/**
 * @brief Sets wifi mode.
 *
 * @param[in] xDeviceMode - Mode of the device Station / Access Point /P2P.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_SetMode( WIFIDeviceMode_t xDeviceMode )
{
    QCOM_WLAN_DEV_MODE dev_mode = QCOM_WLAN_DEV_MODE_INVALID;
    WIFIReturnCode_t status = eWiFiFailure;

    /* Acquire semaphore */
    if (xSemaphoreTake(g_wifi_semaph, portMAX_DELAY) == pdTRUE)
    {
        do {
            if (eWiFiSuccess != conv_mode_to_qcom(xDeviceMode, &dev_mode))
                break;
            if (A_OK != (A_STATUS)qcom_op_set_mode(g_devid, dev_mode))
                break;
            status = eWiFiSuccess;
        } while (0);
        /* Release semaphore */
        xSemaphoreGive(g_wifi_semaph);
    }
    return status;;
}

/**
 * @brief Gets wifi mode.
 *
 * @param[in] pxDeviceMode - return mode Station / Access Point /P2P
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_GetMode( WIFIDeviceMode_t * pxDeviceMode )
{
    QCOM_WLAN_DEV_MODE dev_mode = QCOM_WLAN_DEV_MODE_INVALID;
    WIFIReturnCode_t status = eWiFiFailure;

    /* Check params */
    if (NULL == pxDeviceMode)
        return eWiFiFailure;

    /* Acquire semaphore */
    if (xSemaphoreTake(g_wifi_semaph, portMAX_DELAY) == pdTRUE)
    {
        do {
            if (A_OK != (A_STATUS)qcom_op_get_mode(g_devid, &dev_mode))
                break;
            if (eWiFiSuccess != conv_qcom_to_mode(dev_mode, pxDeviceMode))
                break;
            status = eWiFiSuccess;
        } while(0);
        /* Release semaphore */
        xSemaphoreGive(g_wifi_semaph);
    }
    return status;
}

/**
 * @brief WiFi Add Network profile.
 *
 * Adds wifi network to network list in NV memory
 *
 * @param[in] pxNetworkProfile - network profile parameters
 * @param[out] pusIndex - network profile index
 *
 * @return Profile stored index on success, or negative error code on failure..
 *
 * @see WIFINetworkParams_t
 */
WIFIReturnCode_t WIFI_NetworkAdd(const WIFINetworkProfile_t * const pxNetworkProfile, uint16_t * pusIndex )
{
    return eWiFiNotSupported;
}


/**
 * @brief WiFi Get Network profile .
 *
 * Gets wifi network params at given index from network list in Non volatile memory
 *
 * @param[out] pxNetworkProfile - pointer to return network profile parameters
 * @param[in] usIndex - Index of the network profile, must be between 0 to wificonfigMAX_NETWORK_PROFILES
 * @return eWiFiSuccess if success, eWiFiFailure otherwise.
 *
 * @see WIFINetworkParams_t
 */
WIFIReturnCode_t WIFI_NetworkGet( WIFINetworkProfile_t * pxNetworkProfile,
                                            uint16_t uxIndex )
{
    return eWiFiNotSupported;
}

/**
 * @brief WiFi Delete Network profile .
 *
 * Deletes wifi network from network list at given index in NV memory
 *
 * @param[in] usIndex - Index of the network profile, must be between 0 to wificonfigMAX_NETWORK_PROFILES
 *                      wificonfigMAX_NETWORK_PROFILES as index will delete all network profiles
 *
 * @return eWiFiSuccess if deleted, eWiFiFailure otherwise.
 *
 */
WIFIReturnCode_t WIFI_NetworkDelete( uint16_t uxIndex )
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
 * @note 'WIFI_GetIP' must be invoked before
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_Ping( uint8_t * pxIPAddr,
                                       uint16_t xCount,
                                       uint32_t xIntervalMS )
{
    uint32_t failed_cnt = 0;
    uint32_t ip4_addr = 0;

    /* Check params */
    if ((NULL == pxIPAddr) || (0 == xCount))
        return eWiFiFailure;

    /* Acquire semaphore */
    if (xSemaphoreTake(g_wifi_semaph, portMAX_DELAY) == pdTRUE)
    {
        pxIPAddr_to_ip(pxIPAddr, &ip4_addr);
        for (uint16_t i = 0; i < xCount; i++)
        {
            if (xIntervalMS == 0)
                xIntervalMS = 20000; // max 20 seconds
            if (A_OK != qcom_ping_ms(ip4_addr, 32, xIntervalMS))
            {
                failed_cnt++;
            }
        }
        /* Release semaphore */
        xSemaphoreGive(g_wifi_semaph);
    }
    /* Report failure if all attempts failed */
    return failed_cnt == xCount ? eWiFiFailure : eWiFiSuccess;
}

/**
 * @brief Retrieves the WiFi interface's IP address.
 *
 * @param[in] IP Address buffer.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_GetIP( uint8_t * pxIPAddr )
{
    uint32_t ip4_addr = 0, ip4_mask = 0, ip4_gw = 0;
    WIFIReturnCode_t status = eWiFiFailure;

    /* Check params */
    if (NULL == pxIPAddr)
        return eWiFiFailure;

    /* Acquire semaphore */
    if (xSemaphoreTake(g_wifi_semaph, portMAX_DELAY) == pdTRUE)
    {
        for (uint32_t j = 0; j < 10; j++)
        {
            /* Call DHCP after connection */
            if (A_OK != qcom_ipconfig(g_devid, QCOM_IPCONFIG_DHCP, &ip4_addr, &ip4_mask, &ip4_gw))
                break;
            /* Retrieve IP info. Perform several attempts because of bug in QCA FW 3.3.5 */
            for (uint32_t attempt_i = 0; attempt_i < 30; attempt_i++)
            {
                if (A_OK != qcom_ipconfig(g_devid, QCOM_IPCONFIG_QUERY, &ip4_addr, &ip4_mask, &ip4_gw))
                    break;

                if (0 == ip4_addr)
                {
                    /* Provide some delay to start next loop to retrieve IP data */
                    vTaskDelay(MSEC_TO_TICK(110));
                }
                else
                {
                    /* Valid IP, terminate loop */
                    status = eWiFiSuccess;
                    break;
                }
            }
            if (eWiFiSuccess == status)
            {
                /* Convert IP addr */
                ip_to_pxIPAddr(ip4_addr, pxIPAddr);
                break;
            }
        }
        /* Release semaphore */
        xSemaphoreGive(g_wifi_semaph);
    }
    return status;
}

/**
 * @brief Retrieves the WiFi interface's MAC address.
 *
 * @param[in] MAC Address buffer.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_GetMAC( uint8_t * pxMac )
{
    WIFIReturnCode_t status = eWiFiFailure;
    uint8_t mac_addr[ATH_MAC_LEN] = {0};

    /* Check params */
    if (NULL == pxMac)
        return eWiFiFailure;

    /* Acquire semaphore */
    if (xSemaphoreTake(g_wifi_semaph, portMAX_DELAY) == pdTRUE)
    {
        do {
            if (A_OK != qcom_get_bssid(g_devid, mac_addr))
                break;
            memcpy(pxMac, mac_addr, ATH_MAC_LEN);
            status = eWiFiSuccess;
        } while (0);
        /* Release semaphore */
        xSemaphoreGive(g_wifi_semaph);
    }
    return status;
}

/**
 * @brief Retrieves host IP address from URL using DNS
 *
 * @param[in] pxHost - Host URL.
 * @param[in] pxIPAddr - IP Address buffer.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_GetHostIP( char * pxHost,
                                            uint8_t * pxIPAddr )
{
    WIFIReturnCode_t status = eWiFiFailure;
    uint32_t result = A_OK;
    IP_ADDR_T tmp_ip = {0};
    IP_ADDR_T dns_ip = {0};
    uint32_t dns_servers[MAX_DNSADDRS] = {0};
    uint32_t dns_servers_num = 0;

    /* Check params */
    if ((NULL == pxIPAddr) || (NULL == pxHost))
        return eWiFiFailure;

    /* Acquire semaphore */
    if (xSemaphoreTake(g_wifi_semaph, portMAX_DELAY) == pdTRUE)
    {
        do {
            /* 'pxHost' is an IP address */
            int tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0;
            if (4 == sscanf(pxHost, "%u.%u.%u.%u", &tmp1, &tmp2, &tmp3, &tmp4))
            {
                pxIPAddr[0] = tmp4;
                pxIPAddr[1] = tmp3;
                pxIPAddr[2] = tmp2;
                pxIPAddr[3] = tmp1;
                status = eWiFiSuccess;
                break;
            }
            /* Obtain DNS IP from DHCP result */
            result = qcom_dns_server_address_get(dns_servers, &dns_servers_num);
            if ((A_OK != result) || (dns_servers_num == 0))
                break;

            /* Perform DNS/UDP request, loop over DNS servers until first success */
            for (uint32_t dns_idx = 0; dns_idx < dns_servers_num; dns_idx++)
            {
                dns_ip.s_addr = dns_servers[dns_idx];
                if (A_OK != qcom_dns_resolver(dns_ip, pxHost, &tmp_ip, wificonfigDNS_QUERY_TIMEOUT))
                    continue;

                /* Copy to output format and terminate loop*/
                pxIPAddr[0] = (uint8_t)(tmp_ip.s_addr >> 24);
                pxIPAddr[1] = (uint8_t)(tmp_ip.s_addr >> 16);
                pxIPAddr[2] = (uint8_t)(tmp_ip.s_addr >> 8);
                pxIPAddr[3] = (uint8_t)(tmp_ip.s_addr);
                status = eWiFiSuccess;
                break;
            }
        } while(0);
        /* Release semaphore */
        xSemaphoreGive(g_wifi_semaph);
    }

    return status;
}

/**
 * @brief Retrieves IP address in Access Point mode
 *
 * @param[in] pxIPAddr - IP Address buffer.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_GetAccessPointIP( uint8_t * pxIPAddr )
{
    uint32_t ip4_addr = 0, ip4_mask = 0, ip4_gw = 0;
    WIFIReturnCode_t status = eWiFiFailure;

    /* Check params */
    if (NULL == pxIPAddr)
        return eWiFiFailure;

    /* Acquire semaphore */
    if (xSemaphoreTake(g_wifi_semaph, portMAX_DELAY) == pdTRUE)
    {
        for (uint32_t j = 0; j < 10; j++)
        {
            /* Call DHCP after connection */
            if (A_OK != qcom_ipconfig(g_devid, QCOM_IPCONFIG_DHCP, &ip4_addr, &ip4_mask, &ip4_gw))
                break;
            /* Retrieve IP info. Perform several attempts because of bug in QCA FW 3.3.5 */
            for (uint32_t attempt_i = 0; attempt_i < 30; attempt_i++)
            {
                if (A_OK != qcom_ipconfig(g_devid, QCOM_IPCONFIG_QUERY, &ip4_addr, &ip4_mask, &ip4_gw))
                    break;

                if (0 == ip4_addr)
                {
                    /* Provide some delay to start next loop to retrieve IP data */
                    vTaskDelay(MSEC_TO_TICK(110));
                }
                else
                {
                    /* Valid IP, terminate loop */
                    status = eWiFiSuccess;
                    break;
                }
            }
            if (eWiFiSuccess == status)
            {
                /* Convert IP addr */
                ip_to_pxIPAddr(ip4_addr, pxIPAddr);
                break;
            }
        }
        /* Release semaphore */
        xSemaphoreGive(g_wifi_semaph);
    }
    return status;
}

/**
 * @brief Perform WiF Scan
 *
 * @param[in] pxBuffer - Buffer for scan results.
 * @param[in] uxNumNetworks - Number of networks in scan result.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_Scan( WIFIScanResult_t * pxBuffer,
                                       uint8_t uxNumNetworks )
{
    WIFIReturnCode_t status = eWiFiFailure;
    QCOM_BSS_SCAN_INFO *scan_result = NULL;
    int16_t scan_result_num = 0;

    /* Check params */
    if ((NULL == pxBuffer) || (0 == uxNumNetworks))
        return eWiFiFailure;

    /* Acquire semaphore */
    if (xSemaphoreTake(g_wifi_semaph, portMAX_DELAY) == pdTRUE)
    {
        do {
            /* Note: won't work if SSID is set already */
            if (A_OK != qcom_set_scan(g_devid, NULL))
                break;
            if (qcom_get_scan(g_devid, &scan_result, &scan_result_num))
                break;
            /* Get MIN(scan_result_num, uxNumNetworks) */
            if (scan_result_num > (int16_t)uxNumNetworks)
                scan_result_num = (int16_t)uxNumNetworks;
            for (int16_t i  = 0 ; i < scan_result_num; i++)
            {
                strncpy((char*)pxBuffer->cSSID, (char const*)scan_result[i].ssid, wificonfigMAX_SSID_LEN);
                strncpy((char*)pxBuffer->ucBSSID, (char const*)scan_result[i].bssid, wificonfigMAX_BSSID_LEN);
                pxBuffer->cRSSI = scan_result[i].rssi;
                pxBuffer->cChannel = scan_result[i].channel;
                pxBuffer->ucHidden = 0;
                if (!scan_result[i].security_enabled)
                {
                    pxBuffer->xSecurity = eWiFiSecurityOpen;
                }
                else if (
                    (0 == scan_result[i].rsn_cipher) ||
                    (ATH_CIPHER_TYPE_WEP & scan_result[i].rsn_cipher)
                )
                {
                    pxBuffer->xSecurity = eWiFiSecurityWEP;
                }
                else if (ATH_CIPHER_TYPE_CCMP & scan_result[i].rsn_cipher)
                {
                    pxBuffer->xSecurity = eWiFiSecurityWPA2;
                }
                else
                {
                    /* TODO: Expect WPA */
                    pxBuffer->xSecurity = eWiFiSecurityWPA;
                }
#if 0
                PRINTF("-----------------------\r\n");
                PRINTF("channel: %d \r\n", scan_result[i].channel);
                PRINTF("ssid_len: %d \r\n", scan_result[i].ssid_len);
                PRINTF("rssi: %d \r\n", scan_result[i].rssi);
                PRINTF("security_enabled: %d \r\n", scan_result[i].security_enabled);
                PRINTF("beacon_period: %d \r\n", scan_result[i].beacon_period);
                PRINTF("preamble: %d \r\n", scan_result[i].preamble);
                PRINTF("bss_type: %d \r\n", scan_result[i].bss_type);
                PRINTF("bssid: %x%x%x%x%x%x \r\n",
                    scan_result[i].bssid[0],
                    scan_result[i].bssid[1],
                    scan_result[i].bssid[2],
                    scan_result[i].bssid[3],
                    scan_result[i].bssid[4],
                    scan_result[i].bssid[5]
                );
                PRINTF("ssid: %s \r\n", scan_result[i].ssid);
                PRINTF("rsn_cipher: %d \r\n", scan_result[i].rsn_cipher);
                PRINTF("rsn_auth: %d \r\n", scan_result[i].rsn_auth);
                PRINTF("wpa_cipher: %d \r\n", scan_result[i].wpa_cipher);
                PRINTF("wpa_auth: %d \r\n", scan_result[i].wpa_auth);
#endif

                pxBuffer += 1;
            }
            status = eWiFiSuccess;
        } while (0);
        /* Release semaphore */
        xSemaphoreGive(g_wifi_semaph);
    }
    return status;
}

/**
 * @brief Configure SoftAP
 *
 * @param[in] pxNetworkParams - Network params to configure AP.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_ConfigureAP(const WIFINetworkParams_t * const pxNetworkParams )
{
    WIFIReturnCode_t status = eWiFiFailure;
    WLAN_AUTH_MODE auth_mode = WLAN_AUTH_INVALID;
    WLAN_CRYPT_TYPE crypt_type = WLAN_CRYPT_INVALID;

    /* Check params */
    if (NULL == pxNetworkParams || NULL == pxNetworkParams->pcSSID || NULL == pxNetworkParams->pcPassword)
        return eWiFiFailure;

    /* Acquire semaphore */
    if (xSemaphoreTake(g_wifi_semaph, portMAX_DELAY) == pdTRUE)
    {
        do {
            /* Set WiFi to device mode */
            if (A_OK != (A_STATUS)qcom_op_set_mode(g_devid, QCOM_WLAN_DEV_MODE_AP))
                break;

            /* Set SSID, must be done before auth, cipher and passphrase */
            strncpy(g_ssid.ssid, pxNetworkParams->pcSSID, sizeof(g_ssid));
            if (A_OK != (A_STATUS)qcom_set_ssid(g_devid, &g_ssid))
                break;

            /* Convert 'WIFISecurity_t' to 'WLAN_AUTH_MODE', 'WLAN_CRYPT_TYPE' */
            if (eWiFiSuccess != conv_security_to_qcom(pxNetworkParams->xSecurity, &auth_mode, &crypt_type))
                break;

            /* Set encyption mode */
            if (A_OK != (A_STATUS)qcom_sec_set_encrypt_mode(g_devid, crypt_type))
                break;

            /* Set auth mode */
            if (A_OK != qcom_sec_set_auth_mode(g_devid, auth_mode))
                break;

            /* Set passphrase */
            strncpy(g_passphr.passphrase, pxNetworkParams->pcPassword, sizeof(g_passphr));
            if (A_OK != qcom_sec_set_passphrase(g_devid, &g_passphr))
                break;

            /* Commit settings to WiFi module */
            if (A_OK != qcom_commit(g_devid))
                break;

            status = eWiFiSuccess;
        } while (0);

        /* Release semaphore */
        xSemaphoreGive(g_wifi_semaph);
    }

    return status;
}

WIFIReturnCode_t WIFI_SetPMMode( WIFIPMMode_t xPMModeType,
                                 const void * pvOptionValue )
{
	return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_GetPMMode( WIFIPMMode_t * pxPMModeType,
                                 void * pvOptionValue )
{
	return eWiFiNotSupported;
}
