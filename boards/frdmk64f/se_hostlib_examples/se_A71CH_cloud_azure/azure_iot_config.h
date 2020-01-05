/* Copyright 2018 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*/

#ifndef AZURE_IOT_CONFIG_H_
#define AZURE_IOT_CONFIG_H_

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/* doc:start:azure-credentials */
#define AZURE_IOT_HUB_NAME "NXP-IOT-HUB-NAME"
#define AZURE_LOCATION_NAME "bengaluru"
#define AZURE_DEVICE_NAME "377813914287991534125055"
/* doc:end:azure-credentials */

#define AZURE_IOT_MQTT_HOST AZURE_IOT_HUB_NAME ".azure-devices.net" ///< Customer specific MQTT HOST.
#define AZURE_IOT_MQTT_PORT (8883)                                  ///< default port for MQTT/S
#define AZURE_IOT_MQTT_CLIENT_ID AZURE_DEVICE_NAME
#define AZURE_IOT_MQTT_USERNAME AZURE_IOT_MQTT_HOST "/" AZURE_DEVICE_NAME
#define AZURE_IOT_MQTT_PUB_TOPIC "devices/" AZURE_DEVICE_NAME "/messages/events/"
#define AZURE_IOT_MQTT_SUB_TOPIC "devices/" AZURE_DEVICE_NAME "/messages/devicebound/#"

/* doc:start:azure-keyids */
#define AZURE_IOT_KEY_INDEX_SM 0x223344         ///< Index where client key is kept           //Decimal - 2241348
#define AZURE_IOT_CLIENT_CERT_INDEX_SM 0x223345 ///< Index where client certificate is kept   //Decimal - 2241349
/* doc:end:azure-keyids */

// =================================================

#endif /* AZURE_IOT_CONFIG_H_ */
