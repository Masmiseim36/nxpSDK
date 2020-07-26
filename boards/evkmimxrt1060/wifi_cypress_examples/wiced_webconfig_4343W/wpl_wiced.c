/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wpl.h"
#include "wwd.h"
#include "wwd_wiced.h"
#include "wwd_network.h"
#include "lwip/tcpip.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dhcp.h"
#include <string.h>
#include "dhcp_server.h"

#define RESULT_BUFF_SIZE (40)

/* Macros for comparing MAC addresses */
#define CMP_MAC(a, b)                                            \
    (((((unsigned char *)a)[0]) == (((unsigned char *)b)[0])) && \
     ((((unsigned char *)a)[1]) == (((unsigned char *)b)[1])) && \
     ((((unsigned char *)a)[2]) == (((unsigned char *)b)[2])) && \
     ((((unsigned char *)a)[3]) == (((unsigned char *)b)[3])) && \
     ((((unsigned char *)a)[4]) == (((unsigned char *)b)[4])) && \
     ((((unsigned char *)a)[5]) == (((unsigned char *)b)[5])))

/*******************************************************************************
 * Variables
 ******************************************************************************/
static wiced_mac_t bssid_list[RESULT_BUFF_SIZE]; /* List of BSSID (AP MAC addresses) that have been scanned */
static SemaphoreHandle_t scan_semaphore = NULL;

static SemaphoreHandle_t scan_result_buff_semaphore = NULL;
static wiced_scan_result_t result_buff[RESULT_BUFF_SIZE];
static TaskHandle_t xScanTaskNotify = NULL;

volatile static uint16_t result_buff_write_pos = 0;

static struct netif wiced_if, client_if;

char ssids_json[2048];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void WPL_add_wlan_interface();

extern wiced_result_t wiced_wlan_connectivity_init(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

int WPL_Init()
{
    tcpip_init(NULL, NULL);

    scan_semaphore             = xSemaphoreCreateBinary();
    scan_result_buff_semaphore = xSemaphoreCreateBinary();

    if (scan_semaphore == NULL || scan_result_buff_semaphore == NULL)
    {
        PRINTF("[!] Failed to create scan semaphore.\n");
        return WPL_ERROR;
    }

    // Give semaphores by default
    xSemaphoreGive(scan_semaphore);
    xSemaphoreGive(scan_result_buff_semaphore);

    return WPL_SUCCESS;
}

int WPL_Start()
{
    wiced_result_t result = wiced_wlan_connectivity_init();
    if (result == WICED_SUCCESS)
        return WPL_SUCCESS;
    else
        return result;
}

int WPL_Stop()
{
    return WPL_SUCCESS;
}

int WPL_Start_AP(char *ssid, char *password, int chan)
{
    wwd_result_t result;
    wiced_ssid_t ap_ssid;
    ap_ssid.length = strlen(ssid);
    strcpy((char *)ap_ssid.value, ssid);

    result = wwd_wifi_start_ap(&ap_ssid, WICED_SECURITY_WPA2_MIXED_PSK, (uint8_t *)password, strlen(password), chan);

    if (result == WWD_SUCCESS)
        return WPL_SUCCESS;
    else
        return result;
}

int WPL_Stop_AP()
{
    wwd_result_t result;
    result = wwd_wifi_stop_ap();

    if (result == WWD_SUCCESS)
        return WPL_SUCCESS;
    else
        return result;
}

static void WPL_scan_results_handler(wiced_scan_result_t **result_ptr, void *user_data, wiced_scan_status_t status)
{
    if (result_ptr == NULL)
    {
        /* finished, set last channel to 0xFF */
        result_buff[result_buff_write_pos].channel = 0xff;
        // Notify the calling task that scan is complete
        xTaskNotify(xScanTaskNotify, WPL_SUCCESS, eSetValueWithOverwrite);
        return;
    }
    if (result_buff_write_pos >= RESULT_BUFF_SIZE)
    {
        // Resume scan trigger task with error notification
        WPRINT_WWD_INFO(("scan result overflow\r\n"));
        xTaskNotify(xScanTaskNotify, WPL_ERROR, eSetValueWithOverwrite);
        return;
    }

    wiced_scan_result_t *record = (*result_ptr);

    /* Check the list of BSSID values which have already been printed */
    wiced_mac_t *tmp_mac = bssid_list;
    while (NULL_MAC(tmp_mac->octet) == WICED_FALSE)
    {
        if (CMP_MAC(tmp_mac->octet, record->BSSID.octet) == WICED_TRUE)
        {
            /* already seen this BSSID */
            return;
        }
        tmp_mac++;
    }

    /* New BSSID - add it to the list */
    memcpy(&tmp_mac->octet, record->BSSID.octet, sizeof(wiced_mac_t));

    /* Add the result to the list and set the pointer for the next result */
    result_buff_write_pos++;
    *result_ptr = &result_buff[result_buff_write_pos];
}

int WPL_Scan()
{
    wwd_result_t result;
    wiced_scan_result_t *result_ptr = (wiced_scan_result_t *)&result_buff;

    // Check if previous scan is running...
    if (pdTRUE != xSemaphoreTake(scan_semaphore, 0))
    {
        // Previous scan is still runnin
        PRINTF("Previous scan is still running. Semaphore count: %d.\n", uxSemaphoreGetCount(scan_semaphore));
        // Wait till the scan finishes, then return
        xSemaphoreTake(scan_semaphore, portMAX_DELAY);
        xSemaphoreGive(scan_semaphore);
        return WPL_SUCCESS;
    }

    PRINTF("Scanning available networks...\r\n");

    // Lock the scan result_buff, in case anyone wants to read the JSON while scanning
    xSemaphoreTake(scan_result_buff_semaphore, portMAX_DELAY);
    // Handle to the task that called the scan
    xScanTaskNotify = xTaskGetCurrentTaskHandle();

    // Clear the previous buffers
    memset(bssid_list, 0, sizeof(bssid_list));
    memset(result_buff, 0, sizeof(result_buff));
    result_buff_write_pos = 0;

    wwd_wifi_set_scan_suppress(WICED_FALSE);

    // Trigger scan
    result = wwd_wifi_scan(WICED_SCAN_TYPE_ACTIVE, WICED_BSS_TYPE_ANY, NULL, NULL, NULL, NULL, WPL_scan_results_handler,
                           (wiced_scan_result_t **)&result_ptr, NULL, WWD_STA_INTERFACE);

    if (result != WWD_SUCCESS)
    {
        PRINTF(("[!] Error starting scan\r\n"));
        goto ERROR;
    }

    // Wait for the scan to finish
    if (WPL_SUCCESS == ulTaskNotifyTake(pdTRUE, portMAX_DELAY))
    {
        // We have the scan results, we can return...
        xSemaphoreGive(scan_semaphore);
        xSemaphoreGive(scan_result_buff_semaphore);
        return WPL_SUCCESS;
    }
    else
    {
        // We received an error in the notification...
        PRINTF("[!] Error occured during scan\r\n");
        goto ERROR;
    }

ERROR:
    // The scan faild, make sure there is no garbage in the result_buff
    memset(bssid_list, 0, sizeof(bssid_list));
    memset(result_buff, 0, sizeof(result_buff));
    xSemaphoreGive(scan_semaphore);
    xSemaphoreGive(scan_result_buff_semaphore);
    return WPL_ERROR;
}

/* Function adapted from wlanif_add.c but modified to allow for IP address extraction */
static void WPL_add_wlan_interface()
{
    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
    struct dhcp *pdhcp;

    IP4_ADDR(&fsl_netif0_ipaddr, 0, 0, 0, 0);
    IP4_ADDR(&fsl_netif0_netmask, 0, 0, 0, 0);
    IP4_ADDR(&fsl_netif0_gw, 0, 0, 0, 0);

    netifapi_netif_add(&client_if, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, (void *)WWD_STA_INTERFACE,
                       wlanif_init, tcpip_input);

    netifapi_netif_set_default(&client_if);
    netifapi_netif_set_up(&client_if);

    PRINTF("Getting IP address from DHCP server\r\n");
    netifapi_dhcp_start(&client_if);
    pdhcp = (struct dhcp *)netif_get_client_data(&client_if, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
    while (pdhcp->state != DHCP_STATE_BOUND)
    {
        vTaskDelay(1000);
    }
}

int WPL_Join(char *ssid, char *password)
{
    wiced_ssid_t ap_ssid;
    wwd_result_t wwd_result    = WWD_SUCCESS;
    wiced_security_t auth_type = WICED_SECURITY_WPA2_MIXED_PSK;
    uint32_t pwd_len           = 0;

    ap_ssid.length = strlen(ssid);
    strcpy((char *)ap_ssid.value, ssid);

    pwd_len = strlen(password);
    if (pwd_len == 0)
    {
        auth_type = WICED_SECURITY_OPEN;
    }

    wwd_result = wwd_wifi_join(&ap_ssid, auth_type, (uint8_t *)password, pwd_len, NULL, WWD_STA_INTERFACE);

    if (wwd_result == WWD_SUCCESS)
    {
        (void)host_rtos_delay_milliseconds((uint32_t)1000);
        WPL_add_wlan_interface();

        return WPL_SUCCESS;
    }
    else
    {
        return wwd_result;
    }
}

int WPL_Leave()
{
    LOCK_TCPIP_CORE();
    // netifapi_dhcp_stop(&client_if);
    netif_remove(&client_if);
    UNLOCK_TCPIP_CORE();
    return WPL_SUCCESS;
}

int WPL_StartDHCPServer(char *ip, char *net)
{
    ip4_addr_t ap_ipaddr;
    ip4_addr_t ap_netmask;

    if ((ip4addr_aton(ip, &ap_ipaddr) == 0) || (ip4addr_aton(net, &ap_netmask) == 0))
    {
        PRINTF("Invalid IP address\r\n");
        return -1;
    }

    /* Configure network interface */
    LOCK_TCPIP_CORE();
    if (NULL ==
        netif_add(&wiced_if, &ap_ipaddr, &ap_netmask, &ap_ipaddr, (void *)WWD_AP_INTERFACE, wlanif_init, tcpip_input))
    {
        PRINTF("[!] Failed to start network interface.\r\n");
        return -1;
    }
    netif_set_default(&wiced_if);
    netif_set_up(&wiced_if);
    UNLOCK_TCPIP_CORE();

    LOCK_TCPIP_CORE();
    start_dhcp_server(ap_ipaddr.addr);
    UNLOCK_TCPIP_CORE();
    return 0;
}

int WPL_StopDHCPServer()
{
    LOCK_TCPIP_CORE();
    quit_dhcp_server();
    netif_remove(&wiced_if);
    UNLOCK_TCPIP_CORE();
    return 0;
}

char *WPL_getSSIDs()
{
    // Start building JSON
    strcpy(ssids_json, "[");
    char network_buf[512];
    char security[32];

    // Make sure to lock the scan result_buff
    xSemaphoreTake(scan_result_buff_semaphore, portMAX_DELAY);
    wiced_scan_result_t *record;
    for (int ii = 0; ii < result_buff_write_pos; ii++)
    {
        record = &result_buff[ii];

        switch (record->security)
        {
            case WICED_SECURITY_OPEN:
                strcpy(security, "Open");
                break;

            case WICED_SECURITY_WEP_PSK:
                strcpy(security, "WEP");
                break;
            case WICED_SECURITY_WEP_SHARED:
                strcpy(security, "WEP Shared");
                break;

            case WICED_SECURITY_WPA_TKIP_PSK:
                strcpy(security, "WPA TKIP");
                break;
            case WICED_SECURITY_WPA_AES_PSK:
                strcpy(security, "WPA AES");
                break;
            case WICED_SECURITY_WPA_MIXED_PSK:
                strcpy(security, "WPA Mixed");
                break;

            case WICED_SECURITY_WPA2_TKIP_PSK:
                strcpy(security, "WPA2 TKIP");
                break;
            case WICED_SECURITY_WPA2_AES_PSK:
                strcpy(security, "WPA2 AES");
                break;
            case WICED_SECURITY_WPA2_MIXED_PSK:
                strcpy(security, "WPA2 Mixed");
                break;

            case WICED_SECURITY_WPA3_SAE:
                strcpy(security, "WPA3 SAE");
                break;
            case WICED_SECURITY_WPA3_WPA2_PSK:
                strcpy(security, "WPA3 WPA2_PSK");
                break;

            case WICED_SECURITY_WPA_TKIP_ENT:
                strcpy(security, "WPA Ent TKIP");
                break;
            case WICED_SECURITY_WPA_AES_ENT:
                strcpy(security, "WPA Ent AES");
                break;
            case WICED_SECURITY_WPA_MIXED_ENT:
                strcpy(security, "WPA Ent Mixed");
                break;

            case WICED_SECURITY_WPA2_TKIP_ENT:
                strcpy(security, "WPA2 Ent TKIP");
                break;
            case WICED_SECURITY_WPA2_AES_ENT:
                strcpy(security, "WPA2 Ent AES");
                break;
            case WICED_SECURITY_WPA2_MIXED_ENT:
                strcpy(security, "WPA2 Ent Mixed");
                break;

            default:
                strcpy(security, "Unknown");
                break;
        }

        PRINTF("%s\r\n", record->SSID.value);
        PRINTF("     BSSID         : %02X:%02X:%02X:%02X:%02X:%02X\r\n", (unsigned int)record->BSSID.octet[0],
               (unsigned int)record->BSSID.octet[1], (unsigned int)record->BSSID.octet[2],
               (unsigned int)record->BSSID.octet[3], (unsigned int)record->BSSID.octet[4],
               (unsigned int)record->BSSID.octet[5]);
        PRINTF("     RSSI          : %ddBm\r\n", (int)record->signal_strength);

        PRINTF("     Max Data Rate : %d Mbits/s\r\n", record->max_data_rate / 1000);
        PRINTF("     Radio Band    : %s\r\n", (record->band == WICED_802_11_BAND_5GHZ) ? "5GHz" : "2.4GHz");
        PRINTF("     Channel       : %d\r\n", (int)record->channel);

        if (strlen(ssids_json) + 256 > sizeof(ssids_json))
        {
            PRINTF("[!] The SSID_JSON is too small, can not fill all the SSIDS \r\n");
            // We could just stop here and return....
            __BKPT(0);
        }

        if (ii != 0)
        {
            // Add , separator after first entry
            strcat(ssids_json, ",");
        }

        snprintf(network_buf, sizeof(network_buf) - 1,
                 "{\"ssid\":\"%s\",\"bssid\":\"%02X:%02X:%02X:%02X:%02X:%02X\",\"signal\":\"%ddBm\",\"channel\":%d,"
                 "\"security\":\"%s\",\"data_rate\":%d, \"band\":\"%s\"}",
                 record->SSID.value, (unsigned int)record->BSSID.octet[0], (unsigned int)record->BSSID.octet[1],
                 (unsigned int)record->BSSID.octet[2], (unsigned int)record->BSSID.octet[3],
                 (unsigned int)record->BSSID.octet[4], (unsigned int)record->BSSID.octet[5],
                 (int)record->signal_strength, (int)record->channel, security, (int)record->max_data_rate,
                 ((record->band == WICED_802_11_BAND_5GHZ) ? "5GHz" : "2.4GHz"));
        strcat(ssids_json, network_buf);
    }
    xSemaphoreGive(scan_result_buff_semaphore);
    strcat(ssids_json, "]");
    return ssids_json;
}

int WPL_GetIP(char *ip, int client)
{
    struct netif *network;
    if (client)
        network = &client_if;
    else
        network = &wiced_if;

    sprintf(ip, "%u.%u.%u.%u", (unsigned char)((htonl(network->ip_addr.addr) >> 24) & 0xff),
            (unsigned char)((htonl(network->ip_addr.addr) >> 16) & 0xff),
            (unsigned char)((htonl(network->ip_addr.addr) >> 8) & 0xff),
            (unsigned char)((htonl(network->ip_addr.addr) >> 0) & 0xff));

    return WPL_SUCCESS;
}
