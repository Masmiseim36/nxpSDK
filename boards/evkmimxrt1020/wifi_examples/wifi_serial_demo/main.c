/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "serial_mwm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define MAIN_TASK_PRIORITY (configMAX_PRIORITIES - 1)

#define DEMO_BUFFER_LEN   2048
#define TEST_MESSAGE      "Hello World"
#define HTTP_GET          "GET / HTTP/1.0\r\n\r\n"
#define MAX_WLAN_SCAN     5
#define MAX_FILE_NAMES    8
#define MAX_LEN_FILE_NAME 40

/*******************************************************************************
 * Variables
 ******************************************************************************/
char g_file_name[MAX_FILE_NAMES][MAX_LEN_FILE_NAME];

mwm_sockaddr_t g_echo_srv_addr = {.host = "192.168.1.109", .port = 7};

mwm_sockaddr_t g_http_srv_addr = {.host = "192.168.1.109", .port = 8000};

char g_buffer[DEMO_BUFFER_LEN];
mwm_wlan_t g_wlans[MAX_WLAN_SCAN];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void wait_key()
{
    PRINTF("\r\nPress any key to continue\r\n");
    GETCHAR();
    PRINTF("\r\n");
}

void get_valid_address(mwm_sockaddr_t *addr)
{
    uint32_t ip[4];
    uint32_t port;
    char *ptr;
    int ret;

    while (1)
    {
        SCANF("%s", g_buffer);
        PRINTF("\r\n");

        ret = sscanf(g_buffer, "%lu.%lu.%lu.%lu:%lu", &ip[0], &ip[1], &ip[2], &ip[3], &port);
        if (ret == 5)
        {
            if (ip[0] <= 0xff && ip[1] <= 0xff && ip[2] <= 0xff && ip[3] <= 0xff && port <= 0xffff)
            {
                ptr = strchr(g_buffer, ':');
                memcpy(addr->host, g_buffer, ptr - g_buffer);
                addr->host[ptr - g_buffer] = '\0';
                addr->port                 = port;
                return;
            }
        }
        PRINTF("Enter address in valid format: ");
    }
}

int wlan_get_state()
{
    int ret = mwm_wlan_status();
    if (ret < 0)
    {
        PRINTF("Failed to get WLAN status, error: %d\r\n", ret);
        while (1)
            ;
    }

    return ret;
}

void wlan_state()
{
    PRINTF("WLAN State: ");

    int ret = wlan_get_state();
    if (ret == MWM_INITIALIZED)
    {
        PRINTF("Initialized\r\n");
    }
    else if (ret == MWM_CONNECTED)
    {
        char ssid[33]    = {0};
        char ip_addr[16] = {0};
        ret              = mwm_wlan_info(ssid, ip_addr);
        if (ret == 0)
        {
            PRINTF("Connected\r\nSSID: %s\r\nIP Address: %s\r\n", ssid, ip_addr);
            PRINTF(
                "\r\nFor configuration by web application: connect to WLAN: %s and open address: http://%s in web "
                "browser.\r\n",
                ssid, ip_addr);
        }
        else
        {
            PRINTF("Connected\r\n");
        }
    }
    else if (ret == MWM_CONNECTING)
    {
        ret = mwm_get_param(MWM_MOD_WLAN, MWM_WLAN_SSID, g_buffer, DEMO_BUFFER_LEN);
        if (ret < 0)
        {
            PRINTF("Connecting...\r\n");
        }
        else
        {
            PRINTF("Connecting to %s\r\n", g_buffer);
        }
    }
    else if (ret == MWM_AUTH_FAILED)
    {
        PRINTF("Connection failed, Wi-Fi authentication failed.\r\n");
    }
    else if (ret == MWM_NETWORK_NOT_FOUND)
    {
        PRINTF("Connection failed, WLAN not found.\r\n");
    }
    else if (ret == MWM_PROVISIONING)
    {
        ret = mwm_get_param(MWM_MOD_PROV, MWM_PROV_SSID, g_buffer, DEMO_BUFFER_LEN);
        if (ret < 0)
        {
            PRINTF("Could not get param: %s, error: %d\r\n", MWM_PROV_SSID, ret);
            while (1)
                ;
        }
        PRINTF("Provisioning mode\r\n\r\n");
        PRINTF(
            "For configuration by web application: connect to WLAN: %s and open address: http://192.168.10.1 "
            "in web browser.\r\n",
            g_buffer);
        PRINTF("For configuration by this application select from menu: '4 - WLAN Configuration - Client'\r\n\r\n");
    }
    else if (ret == MWM_DISCONNECTED)
    {
        PRINTF("Disconnected\r\n");
    }
    else
    {
        PRINTF("%d\r\n", ret);
    }
}

void wlan_fw_version()
{
    /* Get Serial MWM Version */
    int ret = mwm_version(g_buffer, DEMO_BUFFER_LEN);
    if (ret < 0)
    {
        PRINTF("Could not get SerialMWM FW Version, error: %d\r\n", ret);
        while (1)
            ;
    }
    PRINTF("SerialMWM FW Version: %s\r\n", g_buffer);

    /* Get WLAN FW Version */
    ret = mwm_wlan_fw_version(g_buffer, DEMO_BUFFER_LEN);
    if (ret < 0)
    {
        PRINTF("Could not get WLAN FW Version, error: %d\r\n", ret);
        while (1)
            ;
    }
    PRINTF("WLAN FW Version: %s\r\n", g_buffer);
}

void wlan_scan()
{
    int ret;

    PRINTF("Scanning...\r\n");
    ret = mwm_wlan_scan(g_wlans, MAX_WLAN_SCAN);
    if (ret < 0)
    {
        PRINTF("WLAN scan failed, error: %d\r\n", ret);
        while (1)
            ;
    }

    if (ret == 0)
    {
        PRINTF("WLANs not found\r\n");
        return;
    }

    int i;
    PRINTF("\r\n");
    for (i = 0; i < ret; i++)
    {
        PRINTF("SSID: %s\r\n", g_wlans[i].ssid);
        PRINTF("Channel: %d\r\n", g_wlans[i].channel);
        PRINTF("RSSI: %d\r\n", g_wlans[i].rssi);
        PRINTF("Security: %d\r\n", g_wlans[i].security);
        PRINTF("\r\n");
    }
}

void wlan_connect()
{
    int ret;

    ret = wlan_get_state();
    if (ret != MWM_DISCONNECTED)
    {
        PRINTF("WLAN must be in disconnected state\r\n");
        return;
    }

    ret = mwm_get_param(MWM_MOD_WLAN, MWM_WLAN_SSID, g_buffer, DEMO_BUFFER_LEN);
    if (ret < 0)
    {
        PRINTF("Connecting...\r\n");
    }
    else
    {
        PRINTF("Connecting to %s\r\n", g_buffer);
    }

    ret = mwm_wlan_connect();
    if (ret < 0)
    {
        PRINTF("WLAN connect failed, error: %d\r\n", ret);
        while (1)
            ;
    }
}

void wlan_disconnect()
{
    int ret;

    ret = wlan_get_state();
    if (ret != MWM_CONNECTED)
    {
        PRINTF("WLAN must be in connected state\r\n");
        return;
    }

    PRINTF("Disconnecting...\r\n");
    ret = mwm_wlan_disconnect();
    if (ret < 0)
    {
        PRINTF("WLAN disconnect failed, error: %d\r\n", ret);
        while (1)
            ;
    }
}

void wlan_reboot()
{
    int ret;

    PRINTF("Rebooting WLAN module...\r\n");
    ret = mwm_reboot();
    if (ret < 0)
    {
        PRINTF("WLAN reboot failed, error: %d\r\n", ret);
        while (1)
            ;
    }

    PRINTF("Starting WLAN...\r\n");
    ret = mwm_wlan_start();
    if (ret < 0)
    {
        PRINTF("Could not start WLAN subsystem, error: %d\r\n", ret);
        while (1)
            ;
    }
}

void wlan_reset_prov_mode()
{
    int ret;

    PRINTF("Resetting WLAN module...\r\n");
    ret = mwm_wlan_prov();
    if (ret < 0)
    {
        PRINTF("WLAN reset failed, error: %d\r\n", ret);
        while (1)
            ;
    }
}

void wlan_config()
{
    int ret;
    bool configured = false;

    PRINTF("WLAN Configuration - Client\r\n");
    ret = mwm_get_param(MWM_MOD_WLAN, MWM_WLAN_CONFIGURED, g_buffer, DEMO_BUFFER_LEN);
    if (ret < 0)
    {
        PRINTF("Could not get param: %s, error: %d\r\n", MWM_PROV_SSID, ret);
        while (1)
            ;
    }

    if (strcmp(g_buffer, "1") == 0)
    {
        /* WLAN is configured */
        ret = mwm_get_param(MWM_MOD_WLAN, MWM_WLAN_SSID, g_buffer, DEMO_BUFFER_LEN);
        if (ret < 0)
        {
            PRINTF("Could not get param: %s, error: %d\r\n", MWM_PROV_SSID, ret);
            while (1)
                ;
        }
        PRINTF("SSID: %s\r\n", g_buffer);

        ret = mwm_get_param(MWM_MOD_WLAN, MWM_WLAN_SECURITY, g_buffer, DEMO_BUFFER_LEN);
        if (ret < 0)
        {
            PRINTF("Could not get param: %s, error: %d\r\n", MWM_PROV_SSID, ret);
            while (1)
                ;
        }
        PRINTF("Security: %s\r\n", g_buffer);

        configured = true;

        PRINTF("\r\nDo you want change this configuration? (y-yes, n-no)\r\n");
        char c = GETCHAR();
        if (c != 'y')
        {
            return;
        }
    }

    PRINTF("\r\n");

    PRINTF("Enter new SSID: ");
    SCANF("%s", g_buffer);
    PRINTF("\r\n");
    ret = mwm_set_param(MWM_MOD_WLAN, MWM_WLAN_SSID, g_buffer);
    if (ret < 0)
    {
        PRINTF("Could not set param: %s, error: %d\r\n", MWM_WLAN_SSID, ret);
        while (1)
            ;
    }

    PRINTF(
        "0 - Open\r\n1 - WEP (Open mode)\r\n2 - WEP (Shared mode)\r\n3 - WPA-PSK\r\n4 - WPA2-PSK\r\n9 - WPA3-SAE\r\n");
    PRINTF("Enter new security: ");
    SCANF("%s", g_buffer);
    PRINTF("\r\n");
    while ((g_buffer[1] != '\0') || (g_buffer[0] < '0') || ((g_buffer[0] > '4') && (g_buffer[0] != '9')))
    {
        PRINTF("Enter valid security: ");
        SCANF("%s", g_buffer);
        PRINTF("\r\n");
    }

    ret = mwm_set_param(MWM_MOD_WLAN, MWM_WLAN_SECURITY, g_buffer);
    if (ret < 0)
    {
        PRINTF("Could not set param: %s, error: %d\r\n", MWM_WLAN_SSID, ret);
        while (1)
            ;
    }

    if (g_buffer[0] > '1')
    {
        /* Security is not open - passphrase is required */
        size_t pass_len = 0u;
        do
        {
            PRINTF("Enter new passphrase (minimum length is 8 characters): ");
            SCANF("%s", g_buffer);
            PRINTF("\r\n");
            pass_len = strlen(g_buffer);
        } while (pass_len < 8u);

        ret = mwm_set_param(MWM_MOD_WLAN, MWM_WLAN_PASSPHRASE, g_buffer);
        if (ret < 0)
        {
            PRINTF("Could not set param: %s, error: %d\r\n", MWM_WLAN_SSID, ret);
            while (1)
                ;
        }
    }

    if (configured == false)
    {
        ret = mwm_set_param(MWM_MOD_WLAN, MWM_WLAN_CONFIGURED, "1");
        if (ret < 0)
        {
            PRINTF("Could not set param: %s, error: %d\r\n", MWM_WLAN_CONFIGURED, ret);
            while (1)
                ;
        }
    }
    PRINTF("\r\n");

    wlan_reboot();
}

void prov_config()
{
    int ret;

    PRINTF("Rebooting WLAN module...\r\n");
    ret = mwm_reboot();
    if (ret < 0)
    {
        PRINTF("WLAN reboot failed, error: %d\r\n", ret);
        while (1)
            ;
    }

    PRINTF("WLAN Configuration - Provisioning mode\r\n\r\n");
    ret = mwm_get_param(MWM_MOD_PROV, MWM_PROV_SSID, g_buffer, DEMO_BUFFER_LEN);
    if (ret < 0)
    {
        PRINTF("Could not get param: %s, error: %d\r\n", MWM_PROV_SSID, ret);
        while (1)
            ;
    }
    PRINTF("SSID: %s\r\n", g_buffer);

    ret = mwm_get_param(MWM_MOD_PROV, MWM_PROV_SECURITY, g_buffer, DEMO_BUFFER_LEN);
    if (ret < 0)
    {
        PRINTF("Could not get param: %s, error: %d\r\n", MWM_PROV_SSID, ret);
        while (1)
            ;
    }
    PRINTF("Security: %s\r\n", g_buffer);

    PRINTF("\r\nDo you want change this configuration? (y-yes, n-no)\r\n");
    char c = GETCHAR();
    if (c != 'y')
    {
        return;
    }
    PRINTF("\r\n");

    PRINTF("Enter new SSID: ");
    SCANF("%s", g_buffer);
    PRINTF("\r\n");
    ret = mwm_set_param(MWM_MOD_PROV, MWM_PROV_SSID, g_buffer);
    if (ret < 0)
    {
        PRINTF("Could not set param: %s, error: %d\r\n", MWM_PROV_SSID, ret);
        while (1)
            ;
    }

    PRINTF("0 - Open\r\n3 - WPA-PSK\r\n4 - WPA2-PSK\r\n");
    PRINTF("Enter new security: ");
    SCANF("%s", g_buffer);
    PRINTF("\r\n");
    while ((g_buffer[1] != '\0') || ((g_buffer[0] != '0') && (g_buffer[0] != '3') && (g_buffer[0] != '4')))
    {
        PRINTF("Enter valid security: ");
        SCANF("%s", g_buffer);
        PRINTF("\r\n");
    }
    ret = mwm_set_param(MWM_MOD_PROV, MWM_PROV_SECURITY, g_buffer);
    if (ret < 0)
    {
        PRINTF("Could not set param: %s, error: %d\r\n", MWM_PROV_SECURITY, ret);
        while (1)
            ;
    }

    if (g_buffer[0] != '0')
    {
        size_t pass_len = 0u;
        do
        {
            PRINTF("Enter new passphrase (minimum length is 8 characters): ");
            SCANF("%s", g_buffer);
            PRINTF("\r\n");
            pass_len = strlen(g_buffer);
        } while (pass_len < 8u);

        ret = mwm_set_param(MWM_MOD_PROV, MWM_PROV_PASSPHRASE, g_buffer);
        if (ret < 0)
        {
            PRINTF("Could not set param: %s, error: %d\r\n", MWM_PROV_PASSPHRASE, ret);
            while (1)
                ;
        }
        PRINTF("\r\n");
    }

    PRINTF("\r\nStarting WLAN...\r\n");
    ret = mwm_wlan_start();
    if (ret < 0)
    {
        PRINTF("Could not start WLAN subsystem, error: %d\r\n", ret);
        while (1)
            ;
    }
}

void fw_upgrade(char *type)
{
    int ret;
    int s;
    size_t data_len;
    uint32_t m;

    ret = wlan_get_state();
    if (ret != MWM_CONNECTED)
    {
        PRINTF("WLAN must be in connected state\r\n");
        return;
    }

    s = mwm_socket(MWM_TCP);
    if (s < 0)
    {
        PRINTF("Could not create socket, error: %d\r\n", ret);
        while (1)
            ;
    }

    PRINTF("Please start HTTP server on your PC to provide file for upgrade\r\n");
    wait_key();

    PRINTF("Enter address of HTTP server in format 'ip_address:port (192.168.1.109:8000)': ");
    get_valid_address(&g_http_srv_addr);
    PRINTF("\r\n");

    PRINTF("Connecting to: %s:%d\r\n", g_http_srv_addr.host, g_http_srv_addr.port);
    ret = mwm_connect(s, &g_http_srv_addr, sizeof(g_http_srv_addr));
    if (ret != 0)
    {
        PRINTF("Could not connect to server, error: %d\r\n", ret);
        while (1)
            ;
    }

    /* Send HTTP GET request */
    data_len = strlen(HTTP_GET);
    ret      = mwm_send(s, HTTP_GET, data_len);
    if (ret <= 0)
    {
        PRINTF("Could not send data, error: %d\r\n", ret);
        while (1)
            ;
    }

    data_len = 0;
    ret      = mwm_recv_timeout(s, g_buffer, DEMO_BUFFER_LEN, 0);
    if (ret <= 0)
    {
        PRINTF("Could not receive data, error: %d\r\n", ret);
        while (1)
            ;
    }
    data_len = ret;
    ret      = mwm_recv_timeout(s, g_buffer + ret, DEMO_BUFFER_LEN - ret, 0);
    if (ret <= 0)
    {
        PRINTF("Could not receive data, error: %d\r\n", ret);
        while (1)
            ;
    }
    data_len += ret;

    ret = mwm_close(s);
    if (ret < 0)
    {
        PRINTF("Could not close socket, error: %d\r\n", ret);
        while (1)
            ;
    }

    /* Parse file names from html page - response from HTTP server */
    char *ptr = strstr(g_buffer, "<body>");
    ptr       = strchr(ptr, '"');
    char *ptr_end;
    uint32_t l;
    uint32_t idx = 0;
    uint32_t file_name_cnt;
    while (ptr != NULL)
    {
        ptr_end = strchr(ptr + 1, '"');
        if (ptr_end != NULL)
        {
            l = ptr_end - ptr - 1;
            if (l > MAX_LEN_FILE_NAME)
            {
                l = MAX_LEN_FILE_NAME - 1;
            }
            memcpy(g_file_name[idx], ptr + 1, l);
            g_file_name[idx++][l] = '\0';
        }

        if (idx == MAX_FILE_NAMES)
        {
            break;
        }

        ptr = strchr(ptr_end + 1, '"');
    }
    file_name_cnt = idx;

    if (file_name_cnt == 0)
    {
        PRINTF("File for upgrade not found\r\n");
        return;
    }

    /* Print names of parsed files */
    PRINTF("\r\n");
    for (idx = 0; idx < file_name_cnt; idx++)
    {
        PRINTF("%d - %s\r\n", idx, g_file_name[idx]);
    }
    PRINTF("Select file for upgrade: ");

    m = 0xff;
    SCANF("%lu", &m);
    PRINTF("\r\n");
    while (m >= file_name_cnt)
    {
        PRINTF("Enter valid number: ");
        SCANF("%d", &m);
        PRINTF("\r\n");
    }

    snprintf(g_buffer, DEMO_BUFFER_LEN, "http://%s:%u/%s", g_http_srv_addr.host, g_http_srv_addr.port, g_file_name[m]);
    PRINTF("Using this file address: %s\r\n", g_buffer);

    PRINTF("Upgrading...\r\n");
    ret = mwm_upgrade(type, g_buffer);
    if (ret < 0)
    {
        PRINTF("Failed to upgrade FW, error: %d\r\n", ret);
        while (1)
            ;
    }

    do
    {
        /* get upgrade status */
        ret = mwm_upgrade(MWM_UPGRADE_STATUS, NULL);
        if (ret < 0)
        {
            PRINTF("Failed to get state of FW upgrade, error: %d\r\n", ret);
            while (1)
                ;
        }
        PRINTF("Upgrade state: ");
        if (ret == MWM_UPGRADE_STARTED)
        {
            PRINTF("started\r\n");
        }
        else if (ret == MWM_UPGRADE_IN_PROGRESS)
        {
            PRINTF("in progress\r\n");
        }
        else if (ret == MWM_UPGRADE_SUCCESSFUL)
        {
            PRINTF("successful done\r\n");
        }
        else
        {
            PRINTF("%d\r\n", ret);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    } while ((ret == MWM_UPGRADE_STARTED) || (ret == MWM_UPGRADE_IN_PROGRESS));
    PRINTF("\r\n");

    wlan_reboot();
    PRINTF("\r\n");

    wlan_fw_version();
}

void socket_test_tcp()
{
    int ret;
    int s;
    int data_len;

    ret = wlan_get_state();
    if (ret != MWM_CONNECTED)
    {
        PRINTF("WLAN must be in connected state\r\n");
        return;
    }

    PRINTF("TCP Socket test (Client)\r\n");
    s = mwm_socket(MWM_TCP);
    if (s < 0)
    {
        PRINTF("Could not create socket, error: %d\r\n", ret);
        while (1)
            ;
    }

    PRINTF("Please start TCP Echo server on your PC\r\n");
    wait_key();

    PRINTF("Enter address of TCP Echo server in format 'ip_address:port (192.168.1.109:7)': ");
    get_valid_address(&g_echo_srv_addr);

    PRINTF("Connecting to: %s:%d\r\n", g_echo_srv_addr.host, g_echo_srv_addr.port);
    ret = mwm_connect(s, &g_echo_srv_addr, sizeof(g_echo_srv_addr));
    if (ret != 0)
    {
        PRINTF("Could not connect to server, error: %d\r\n", ret);
        while (1)
            ;
    }

    PRINTF("Sending: %s\r\n", TEST_MESSAGE);
    data_len = strlen(TEST_MESSAGE);
    ret      = mwm_send(s, TEST_MESSAGE, data_len);
    if (ret != data_len)
    {
        PRINTF("Could not send data, error: %d\r\n", ret);
        while (1)
            ;
    }

    ret = mwm_recv_timeout(s, g_buffer, data_len, 0);
    if (ret != data_len)
    {
        PRINTF("Could not receive data, error: %d\r\n", ret);
        while (1)
            ;
    }
    g_buffer[data_len] = '\0';
    PRINTF("Received: %s\r\n", g_buffer);
    PRINTF("\r\n");

    ret = mwm_close(s);
    if (ret != 0)
    {
        PRINTF("Could not close socket, error: %d\r\n", ret);
        while (1)
            ;
    }
}

void socket_test_udp()
{
    int ret;
    int s;
    int data_len;

    ret = wlan_get_state();
    if (ret != MWM_CONNECTED)
    {
        PRINTF("WLAN must be in connected state\r\n");
        return;
    }

    PRINTF("UDP Socket test\r\n");
    s = mwm_socket(MWM_UDP);
    if (s < 0)
    {
        PRINTF("Could not create socket, error: %d\r\n", ret);
        while (1)
            ;
    }

    PRINTF("Please start UDP Echo server on your PC\r\n");
    wait_key();

    PRINTF("Enter address of UDP Echo server in format 'ip_address:port (192.168.1.109:7)': ");
    get_valid_address(&g_echo_srv_addr);

    PRINTF("Sending: %s\r\n", TEST_MESSAGE);
    data_len = strlen(TEST_MESSAGE);
    ret      = mwm_sendto(s, TEST_MESSAGE, data_len, &g_echo_srv_addr, sizeof(g_echo_srv_addr));
    if (ret != data_len)
    {
        PRINTF("Could not send data, error: %d\r\n", ret);
        while (1)
            ;
    }

    ret = mwm_recvfrom_timeout(s, g_buffer, data_len, 0);
    if (ret != data_len)
    {
        PRINTF("Could not receive data, error: %d\r\n", ret);
        while (1)
            ;
    }

    g_buffer[data_len] = '\0';
    PRINTF("Received: %s\r\n", g_buffer);
    PRINTF("\r\n");

    ret = mwm_close(s);
    if (ret != 0)
    {
        PRINTF("Could not close socket, error: %d\r\n", ret);
        while (1)
            ;
    }
}

void main_task(void *pvParameters)
{
    int ret;
    uint32_t m;

    PRINTF("\r\n=================\r\n");
    PRINTF("Serial Wi-Fi Demo\r\n");
    PRINTF("=================\r\n\r\n");

    /* Initialize Serial MWM */
    PRINTF("Initializing...\r\n");
    ret = mwm_init();
    if (ret < 0)
    {
        PRINTF("Could not initialize Serial MWM, error: %d\r\n", ret);
        while (1)
            ;
    }

    PRINTF("\r\n");
    wlan_fw_version();

    ret = wlan_get_state();
    if (ret == MWM_INITIALIZED)
    {
        PRINTF("\r\nStarting WLAN...\r\n");
        ret = mwm_wlan_start();
        if (ret < 0)
        {
            PRINTF("Could not start WLAN subsystem, error: %d\r\n", ret);
            while (1)
                ;
        }
    }

    PRINTF("\r\n");
    wlan_state();
    PRINTF("\r\n");

    while (1)
    {
        PRINTF("\r\n-------- MENU --------\r\n");
        PRINTF(" 0 - WLAN State\r\n");
        PRINTF(" 1 - WLAN Scan\r\n");
        PRINTF(" 2 - WLAN Connect\r\n");
        PRINTF(" 3 - WLAN Disconnect\r\n");
        PRINTF(" 4 - WLAN Configuration - Client\r\n");
        PRINTF(" 5 - WLAN Configuration - Provisioning mode\r\n");
        PRINTF(" 6 - WLAN Reset to provisioning mode\r\n");
        PRINTF(" 7 - WLAN Reboot\r\n");
        PRINTF(" 8 - SerialMWM FW Upgrade\r\n");
        PRINTF(" 9 - WLAN FW Upgrade\r\n");
        PRINTF("10 - TCP Socket test (Client)\r\n");
        PRINTF("11 - UDP Socket test\r\n");

        m = 0xff;
        PRINTF("Select from menu: ");
        ret = SCANF("%lu", &m);
        PRINTF("\r\n\r\n");

        if (ret != 1)
        {
            continue;
        }

        switch (m)
        {
            case 0:
                wlan_state();
                break;

            case 1:
                wlan_scan();
                break;

            case 2:
                wlan_connect();
                break;

            case 3:
                wlan_disconnect();
                break;

            case 4:
                wlan_config();
                break;

            case 5:
                prov_config();
                break;

            case 6:
                wlan_reset_prov_mode();
                break;

            case 7:
                wlan_reboot();
                break;

            case 8:
                fw_upgrade(MWM_UPGRADE_MCU_FW);
                break;

            case 9:
                fw_upgrade(MWM_UPGRADE_WLAN_FW);
                break;

            case 10:
                socket_test_tcp();
                break;

            case 11:
                socket_test_udp();
                break;

            case 12:
                PRINTF("Memory usage:\r\n");
                PRINTF("Stack high water mark: %d\r\n", uxTaskGetStackHighWaterMark(NULL));
                PRINTF("Minimum ever free heap size: %d\r\n", xPortGetMinimumEverFreeHeapSize());
                break;

            default:
                PRINTF("Enter number from menu\r\n");
                break;
        }
        wait_key();
    }
}

int main(void)
{
    BOARD_InitHardware();

    if (xTaskCreate(main_task, "main_task", 350, NULL, MAIN_TASK_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
