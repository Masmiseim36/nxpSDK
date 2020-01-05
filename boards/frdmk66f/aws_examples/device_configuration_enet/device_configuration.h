/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DEVICE_CONFIGURATION_H_
#define _DEVICE_CONFIGURATION_H_

#include "fnet_mdns_port.h"
#ifdef DEMO_DEVICE_CONFIGURATION_WIFI
#include "iot_wifi.h"
#endif

/*! @brief Network connection type. */
typedef enum
{
    kDEV_CFG_ENET,
    kDEV_CFG_WIFI_AP,
    kDEV_CFG_WIFI_CLIENT
} connection_type_t;

/*! @brief Device configuration request commands. */
typedef enum
{
    kCMD_Login,
    kCMD_SetLoginPassword,
    kCMD_SetHostname,
    kCMD_Reset,
    kCMD_SetCertificate,
    kCMD_SetPrivateKey,
    kCMD_SetAwsThingName,
    kCMD_SetAwsEndpoint,
    kCMD_SetWifiNetwork,
    kCMD_GetDeviceInfo
} request_cmd_t;

/*! @brief Response to request command. */
typedef enum
{
    kCMD_Ok,
    kCMD_Fail,
    kCMD_Unknown
} response_cmd_t;

/*! @brief Response structure to request command. */
typedef struct
{
    response_cmd_t status;
    uint16_t data_length;
    void *data;
} response_t;

/*!
 * @brief Initialzation function.
 *
 * This function initializes the mflash file system
 *
 * @retval 0 Initialization was successful.
 * @retval -1 Error in initialization.
 */
int dev_cfg_init(void);

/*!
 * @brief Initialzation function of configuration server.
 *
 * This function initializes the SSL server and mDNS responder.
 * The SSL server is used to communication with mobile app, which can configure the aws iot device.
 * The mDNS respoder is used to be possible find the device on the LAN.
 *
 * @retval 0 Initialization was successful.
 * @retval -1 Error in initialization.
 */
int dev_cfg_init_config_server(fnet_netif_desc_t netif_desc, connection_type_t con_type);

/*!
 * @brief Function for checking aws credentials.
 *
 * This function checks if are in the mflash file system stored aws credentials and
 * other parameters used during connection to AWS IoT.
 *
 * @retval 0 All aws credentials are stored in mflash file system.
 * @retval -1 Some of aws credentials or connection parameters is not stored in mflash file system.
 */
int dev_cfg_check_aws_credentials(void);

/*!
 * @brief Function for checking login password.
 *
 * This function compares the login passwod sent by mobile app with login password stored in file system or
 * with the default password.
 *
 * @retval 0 Password is correct.
 * @retval -1 Password is incorrect.
 */
int dev_cfg_check_login_password(void *data, int data_len);

/*!
 * @brief Function for processing commands from the mobile app.
 *
 * This function process request command sent by mobile app.
 *
 * @retval response_t structure.
 */
response_t dev_cfg_process_cmd(request_cmd_t cmd, void *data, int data_len);

/*!
 * @brief Function for reading parameters of wifi network.
 *
 * This function try read and parse parameters of wifi netowrk from the mflash file system.
 *
 * @retval 0 The reading and parsing of wifi network parameters was successful.
 * @retval -1 The reading or parsing of wifi network parameters failed.
 */
#ifdef DEMO_DEVICE_CONFIGURATION_WIFI
int dev_cfg_get_wifi_params(WIFINetworkParams_t *network_params);
#endif

#endif /* _DEVICE_CONFIGURATION_H_ */
