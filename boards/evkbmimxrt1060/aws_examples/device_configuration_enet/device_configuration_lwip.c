/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>

#include "device_configuration_lwip.h"
#include "FreeRTOS.h"
#include "mflash_file.h"
#include "fsl_debug_console.h"
#include "core_pkcs11_config.h"
#include "sslsrv.h"
#include "sslsrv_keys.h"

#if 0
#include "fnet_mdns.h"
#endif
#include "lwip/apps/mdns.h"

#define FILENAME_AWS_THING_NAME "aws_thing_name.dat"
#define FILENAME_AWS_ENDPOINT   "aws_endpoint.dat"
#define FILENAME_MDNS_HOSTNAME  "mdns_hostname.dat"
#define FILENAME_LOGIN_PASSWORD "login_password.dat"
#define FILENAME_WIFI_PARAMS    "wifi_params.dat"

#define MDNS_SERVICE_TYPE "_sslserver._tcp"
#define MDNS_SERVICE_PORT FNET_HTONS(443)

#define DEFAULT_MDNS_HOSTNAME  "aws-iot-device"
#define DEFAULT_LOGIN_PASSWORD "admin"

#define MAX_LENGTH_AWS_ENDPOINT   64
#define MAX_LENGTH_AWS_THING_NAME 32
#define MAX_LENGTH_LOGIN_PASSWORD 32
#define MAX_LENGTH_HOSTNAME       32
#define MAX_LENGTH_MDNS_KEY_VAL   32
#define MAX_LENGTH_DEVICE_INFO    128

//#define DEV_CFG_DEBUG

#if !defined(DEV_CFG_DEBUG)
#define DEV_CFG_PRINTF(...)
#else
#define DEV_CFG_PRINTF vLoggingPrintf
#endif

/* AWS IoT endpoint address */
char clientcredentialMQTT_BROKER_ENDPOINT[MAX_LENGTH_AWS_ENDPOINT] = {0};
/* AWS IoT Thing name */
char clientcredentialIOT_THING_NAME[MAX_LENGTH_AWS_THING_NAME] = {0};

/* Flash structure */
mflash_file_t g_file_table[] = {{.path = pkcs11configFILE_NAME_CLIENT_CERTIFICATE, .max_size = 2000},
                                {.path = pkcs11configFILE_NAME_KEY, .max_size = 2000},
                                {.path = FILENAME_AWS_THING_NAME, .max_size = 200},
                                {.path = FILENAME_AWS_ENDPOINT, .max_size = 200},
                                {.path = FILENAME_MDNS_HOSTNAME, .max_size = 200},
                                {.path = FILENAME_LOGIN_PASSWORD, .max_size = 200},
                                {.path = FILENAME_WIFI_PARAMS, .max_size = 200},
                                {0}};

static SSLSRV_TLS_PARAM_STRUCT tls_params;

/* mDNS server descriptor */
#if 0
fnet_mdns_desc_t g_mdns;

fnet_mdns_txt_key_t mdns_txt_key_table[1] = {0};
#endif

char g_device_info[MAX_LENGTH_DEVICE_INFO] = {0};

static int init_device_info(char *thing_name, bool has_set_credentials, connection_type_t con_type)
{
    int buff_len = MAX_LENGTH_DEVICE_INFO;
    int n        = snprintf(g_device_info, buff_len, "aws_thing_name=%s", thing_name);
    n += snprintf(g_device_info + n, buff_len, ",aws_credentials=%s", has_set_credentials ? "true" : "false");
    n += snprintf(g_device_info + n, buff_len, ",connection=%d", con_type);

    if (con_type == kDEV_CFG_WIFI_CLIENT)
    {
        /* Read parameters of Wifi network */
        uint8_t *fileData = NULL;
        uint32_t dataSize = 0;
        if (mflash_file_mmap(FILENAME_WIFI_PARAMS, &fileData, &dataSize) != kStatus_Success)
        {
            return -1;
        }

        n += snprintf(g_device_info + n, buff_len, ",wifi_security=%d", fileData[0]);
        n += snprintf(g_device_info + n, buff_len, ",wifi_ssid=%s", (char *)(fileData + 1));
    }

    return 0;
}
#if 0
static const fnet_mdns_txt_key_t *mdns_service_get_txt(void)
{
    return mdns_txt_key_table;
}

static const fnet_mdns_service_t mdns_srv_service = {
    .service_type = MDNS_SERVICE_TYPE, .service_port = MDNS_SERVICE_PORT, .service_get_txt = mdns_service_get_txt};

void mdns_task(void *fnet_mdns_if)
{
    while (1)
    {
        fnet_mdns_poll(fnet_mdns_if);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
#endif
/*!
 * @brief Initializes server.
 */
static int ssl_server_init(void)
{
    SSLSRV_PARAM_STRUCT params;
    uint32_t httpsrv_handle;

    /* Init SSLSRV parameters.*/
    memset(&params, 0, sizeof(params));

    tls_params.certificate_buffer      = (const unsigned char *)ssl_srv_cert;
    tls_params.certificate_buffer_size = ssl_srv_cert_len;
    tls_params.private_key_buffer      = (const unsigned char *)ssl_srv_pkey;
    tls_params.private_key_buffer_size = ssl_srv_pkey_len;

    params.tls_param = &tls_params;

    /* Init SSL Server.*/
    httpsrv_handle = SSLSRV_init(&params);
    if (httpsrv_handle == 0)
    {
        return -1;
    }

    return 0;
}

static void srv_txt(struct mdns_service *service, void *txt_userdata)
{
    err_t res;
    res = mdns_resp_add_service_txtitem(service, "Data: empty", 6);
    LWIP_ERROR("mdns add service txt failed\n", (res == ERR_OK), return );
}

static char conflict_hostname[64] = {"ahoj"};

static int hostname_digit_add(char *hostname, uint32_t start_of, uint32_t digit_of)
{
    if (hostname[start_of + digit_of] >= '9')
    {
        hostname[start_of + digit_of] = '0';
        return 1;
    }
    else if (hostname[start_of + digit_of] >= '0')
    {
        hostname[start_of + digit_of] += 1;
        return 0;
    }
    else
    {
        hostname[start_of + digit_of] = '0';
        return 0;
    }
}

static u8_t mdns_service_slot;

void mdns_check(struct netif *netif, int information)
{
    static int basehostname_len = 0;

    // keep position of former hostname
    if (basehostname_len == 0)
    {
        basehostname_len = strlen(conflict_hostname);
    }

    if (information == MDNS_PROBING_CONFLICT)
    {
        // not enough space, drop last 5 chars
        if (basehostname_len + 5 >= 64)
        {
            basehostname_len = basehostname_len - 5;
        }
        // add suffix from '0' to '9999'
        if (hostname_digit_add(conflict_hostname, basehostname_len, 0))
            if (hostname_digit_add(conflict_hostname, basehostname_len, 1))
                if (hostname_digit_add(conflict_hostname, basehostname_len, 2))
                    if (hostname_digit_add(conflict_hostname, basehostname_len, 3))
                    {
                        // counter overflows, do not continue further
                        assert(0);
                        return;
                    }

        mdns_resp_rename_netif(netif, conflict_hostname);
        mdns_resp_rename_service(netif, mdns_service_slot, conflict_hostname);
        mdns_resp_restart(netif);
    }
}

static int mdns_init(char *hostname, struct netif *netif, connection_type_t con_type)
{
    char *thing_name  = "";
    uint8_t *fileData = NULL;
    uint32_t dataSize = 0;
    if (mflash_file_mmap(FILENAME_AWS_THING_NAME, &fileData, &dataSize) == kStatus_Success)
    {
        thing_name = (char *)fileData;
    }

    bool has_set_credentials = false;
    if (dev_cfg_check_aws_credentials() == 0)
    {
        has_set_credentials = true;
    }

    /* Initialize device info */
    if (init_device_info(thing_name, has_set_credentials, con_type) != 0)
    {
        return -1;
    }

    strcpy(conflict_hostname, hostname);

    LOCK_TCPIP_CORE();
    mdns_resp_init();
    mdns_resp_add_netif(netif, hostname);
    mdns_resp_register_name_result_cb((mdns_name_result_cb_t)mdns_check);
    mdns_service_slot = mdns_resp_add_service(netif, hostname, "_sslserver", DNSSD_PROTO_TCP, 443, srv_txt, NULL);
    // mdns_resp_add_service(netif, name, const char *service, enum mdns_sd_proto proto, u16_t port,
    // service_get_txt_fn_t txt_fn, void *txt_data)
    UNLOCK_TCPIP_CORE();

#if 0
    struct fnet_mdns_params mdns_params;
    memset(&mdns_params, 0, sizeof(mdns_params));
    mdns_params.netif_desc = netif_desc;
    mdns_params.name       = hostname;

    /* Start mDNS server */
    g_mdns = fnet_mdns_init(&mdns_params);
    if (g_mdns)
    {
        if (fnet_mdns_service_register(g_mdns, &mdns_srv_service) == 0)
        {
            return -1;
        }

        if (xTaskCreate(mdns_task, "mdns_task", 400, g_mdns, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        {
            return -1;
        }
    }
    else
    {
        DEV_CFG_PRINTF("mDNS init failed");
        return -1;
    }
#endif
    return 0;
}

/* Store data to mflash */
static int save_file(char *file_name, char *buffer, size_t data_len)
{
    if ((file_name == NULL) || (buffer == NULL) || (data_len <= 0))
    {
        return -1;
    }

    int ret_val = -1;

    /* Write the data to file. */
    if (kStatus_Success != mflash_file_save(file_name, (uint8_t *)buffer, data_len))
    {
        DEV_CFG_PRINTF("mflash_file_save failed\n");
    }
    else
    {
        DEV_CFG_PRINTF("mflash_file_save success\n");
        ret_val = 0;
    }

    return ret_val;
}

int dev_cfg_init()
{
    if (mflash_init(g_file_table, 1) != kStatus_Success)
    {
        /* Error in mflash_init */
        return -1;
    }

    return 0;
}

int dev_cfg_init_config_server(struct netif *netif, connection_type_t con_type)
{
    /* Start ssl server to be possible configure the device by mobile app */
    if (ssl_server_init() != 0)
    {
        /* Failed to initialze the SSL server */
        return -1;
    }

    char *mdns_hostname = DEFAULT_MDNS_HOSTNAME;
    /* Try read mDNS hostname from file */
    uint8_t *fileData = NULL;
    uint32_t dataSize = 0;
    if (mflash_file_mmap(FILENAME_MDNS_HOSTNAME, &fileData, &dataSize) == kStatus_Success)
    {
        /* Find hostneme in the file, replace the default hostname by data from file */
        mdns_hostname = (char *)fileData;
    }

    /* Start mDNS responder to be possible find the device on local network by mobile app */
    mdns_init(mdns_hostname, netif, con_type);

    return 0;
}

int dev_cfg_check_aws_credentials()
{
    /* Try read device certificate */
    uint8_t *fileData = NULL;
    uint32_t dataSize = 0;
    if (mflash_file_mmap(pkcs11configFILE_NAME_CLIENT_CERTIFICATE, &fileData, &dataSize) != kStatus_Success)
    {
        DEV_CFG_PRINTF("Device has not set certificate\n");
        return -1;
    }

    /* Try read device private key */
    if (mflash_file_mmap(pkcs11configFILE_NAME_KEY, &fileData, &dataSize) != kStatus_Success)
    {
        DEV_CFG_PRINTF("Device has not set private key\n");
        return -1;
    }

    /* Try read aws iot thing name */
    if (mflash_file_mmap(FILENAME_AWS_THING_NAME, &fileData, &dataSize) != kStatus_Success)
    {
        DEV_CFG_PRINTF("Device has not set aws iot endpoint address\n");
        return -1;
    }
    if (dataSize > MAX_LENGTH_AWS_THING_NAME)
    {
        return -1;
    }
    memcpy(clientcredentialIOT_THING_NAME, fileData, dataSize);

    /* Try read aws iot endpoint address */
    if (mflash_file_mmap(FILENAME_AWS_ENDPOINT, &fileData, &dataSize) != kStatus_Success)
    {
        DEV_CFG_PRINTF("Device has not set aws iot endpoint address\n");
        return -1;
    }
    if (dataSize > MAX_LENGTH_AWS_ENDPOINT)
    {
        return -1;
    }
    memcpy(clientcredentialMQTT_BROKER_ENDPOINT, fileData, dataSize);

    DEV_CFG_PRINTF("AWS IoT endpoint address: %s\n", clientcredentialMQTT_BROKER_ENDPOINT);
    DEV_CFG_PRINTF("AWS IoT thing name: %s\n", clientcredentialIOT_THING_NAME);

    return 0;
}

int dev_cfg_check_login_password(void *data, int data_len)
{
    uint8_t *fileData    = NULL;
    uint32_t dataSize    = 0;
    char *login_password = DEFAULT_LOGIN_PASSWORD;
    if (mflash_file_mmap(FILENAME_LOGIN_PASSWORD, &fileData, &dataSize) == kStatus_Success)
    {
        login_password = (char *)fileData;
    }
    return strcmp(login_password, data);
}

response_t dev_cfg_process_cmd(request_cmd_t cmd, void *data, int data_len)
{
    response_t resp;
    memset(&resp, 0, sizeof(response_t));
    resp.status = kCMD_Fail;

    switch (cmd)
    {
        case kCMD_SetLoginPassword:
            DEV_CFG_PRINTF("kCMD_SetLoginPassword\n");
            if (data_len > MAX_LENGTH_LOGIN_PASSWORD)
            {
                return resp;
            }
            if (save_file(FILENAME_LOGIN_PASSWORD, data, data_len) == 0)
            {
                resp.status = kCMD_Ok;
            }
            break;
        case kCMD_SetHostname:
            DEV_CFG_PRINTF("kCMD_SetHostname\n");
            if (data_len > MAX_LENGTH_HOSTNAME)
            {
                return resp;
            }
            if (save_file(FILENAME_MDNS_HOSTNAME, data, data_len) == 0)
            {
                resp.status = kCMD_Ok;
            }
            break;
        case kCMD_Reset:
            DEV_CFG_PRINTF("kCMD_Reset\n");
            __NVIC_SystemReset();
            break;
        case kCMD_SetCertificate:
            DEV_CFG_PRINTF("kCMD_SetCertificate\n");
            if (save_file(pkcs11configFILE_NAME_CLIENT_CERTIFICATE, data, data_len) == 0)
            {
                resp.status = kCMD_Ok;
            }
            break;
        case kCMD_SetPrivateKey:
            DEV_CFG_PRINTF("kCMD_SetPrivateKey\n");
            if (save_file(pkcs11configFILE_NAME_KEY, data, data_len) == 0)
            {
                resp.status = kCMD_Ok;
            }
            break;
        case kCMD_SetAwsThingName:
            DEV_CFG_PRINTF("kCMD_SetAwsThingName\n");
            if (data_len >= MAX_LENGTH_AWS_THING_NAME)
            {
                return resp;
            }
            if (save_file(FILENAME_AWS_THING_NAME, data, data_len) == 0)
            {
                resp.status = kCMD_Ok;
            }
            break;
        case kCMD_SetAwsEndpoint:
            DEV_CFG_PRINTF("kCMD_SetAwsEndpoint\n");
            if (data_len >= MAX_LENGTH_AWS_ENDPOINT)
            {
                return resp;
            }
            if (save_file(FILENAME_AWS_ENDPOINT, data, data_len) == 0)
            {
                resp.status = kCMD_Ok;
            }
            break;
        case kCMD_SetWifiNetwork:
            DEV_CFG_PRINTF("kCMD_SetWifiNetwork\n");
            if (save_file(FILENAME_WIFI_PARAMS, data, data_len) == 0)
            {
                resp.status = kCMD_Ok;
            }
            break;
        case kCMD_GetDeviceInfo:
            DEV_CFG_PRINTF("kCMD_GetDeviceInfo\n");
            resp.status      = kCMD_Ok;
            resp.data        = g_device_info;
            resp.data_length = strlen(g_device_info);
            break;

        default:
            DEV_CFG_PRINTF("Unknown CMD\n");
            resp.status = kCMD_Unknown;
            break;
    }
    return resp;
}

#ifdef DEMO_DEVICE_CONFIGURATION_WIFI
int dev_cfg_get_wifi_params(WIFINetworkParams_t *network_params)
{
    uint8_t *fileData = NULL;
    uint32_t idx      = 0;
    uint32_t dataSize = 0;

    /* Try read parameters of wifi network */
    if (mflash_file_mmap(FILENAME_WIFI_PARAMS, &fileData, &dataSize) != kStatus_Success)
    {
        DEV_CFG_PRINTF("Device has not set parameters of wifi network\n");
        return -1;
    }

    /* File format: 1B-WIFISecurity_t | nB-SSID (null-terminated) | nB-password (null-terminated) */

    WIFISecurity_t wifiSec = (WIFISecurity_t)fileData[idx];
    /* Check value of WIFISecurity_t */
    if (wifiSec > eWiFiSecurityWPA2)
    {
        return -1;
    }

    network_params->xSecurity = wifiSec;
    idx++;

    /* Read SSID of wifi network */
    uint32_t length        = strlen((char *)&fileData[idx]);
    network_params->pcSSID = pvPortMalloc(length + 1);
    if (network_params->pcSSID == NULL)
    {
        return -1;
    }
    memcpy((void *)network_params->pcSSID, &fileData[idx], length + 1);

    if (wifiSec != eWiFiSecurityOpen)
    {
        /* Read password of wifi network */
        idx += length + 1;
        length                     = strlen((char *)&fileData[idx]);
        network_params->pcPassword = pvPortMalloc(length + 1);
        if (network_params->pcPassword == NULL)
        {
            return -1;
        }
        memcpy((void *)network_params->pcPassword, &fileData[idx], length + 1);
    }

    return 0;
}
#endif
