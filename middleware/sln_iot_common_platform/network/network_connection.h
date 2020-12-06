/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/**
 * @file network_connection.h
 * @brief This file contains the network configuration
 */

#ifndef NETWORK_CONNECTION_H_
#define NETWORK_CONNECTION_H_

#include "assert.h"
#include "stdbool.h"
#include "stdint.h"

#include "board.h"
#include "FreeRTOS.h"

#include "lwip/ip_addr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Provisioning Method (for WiFi SSID and password) */
#define PROVISIONING_USE_USB_CDC 0
#define PROVISIONING_USE_WIFI    1
#define PROVISIONING_USE_BLE     2

#ifndef PROVISIONING_METHOD
#define PROVISIONING_METHOD PROVISIONING_USE_WIFI
#endif

/* choose your weapon */
#ifndef USE_ETHERNET_CONNECTION
#define USE_ETHERNET_CONNECTION 0
#endif

#ifndef USE_WIFI_CONNECTION
#define USE_WIFI_CONNECTION 1
#endif

#ifndef NETWORK_MQTT_CONNECTION_ATTEMPTS
#define NETWORK_MQTT_CONNECTION_ATTEMPTS (3)
#endif

#ifndef NETWORK_MQTT_CONNECTION_DELAY_MS
#define NETWORK_MQTT_CONNECTION_DELAY_MS (500)
#endif

#if USE_WIFI_CONNECTION
#include "wwd_structures.h"
#endif

/* IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 1
#define configIP_ADDR3 222

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 192
#define configGW_ADDR1 168
#define configGW_ADDR2 1
#define configGW_ADDR3 1

#if USE_WIFI_CONNECTION
/* Max password length */
#define WSEC_MAX_PSK_LEN 64

/**
 * @brief The UX states of the wifi used by APP_Wifi_UX_Callback
 */
typedef enum network_wifi_conn_state
{
    NETWORK_WIFI_CONN_WIFI_SETUP,        /**< The wifi is in setup mode */
    NETWORK_WIFI_CONN_INVALID_WIFI_CRED, /**< The wifi credentials are invalid */
    NETWORK_WIFI_CONN_NO_ACCESS_POINT,   /**< The wifi access point was not found */
    NETWORK_WIFI_CONN_ACCESS_POINT_FOUND /**< The wifi access point was found */
} network_wifi_conn_state_t;

/**
 * @brief The wifi network modes
 */
typedef enum network_mode
{
    NETWORK_MODE_AP,     /**< Device is configured as AP (when wifi is used)*/
    NETWORK_MODE_STATION /**< Device is configured as Station (when either wifi or ethernet is used) */
} network_mode_t;

/**
 * @brief The wifi Pre-Shared Key structure
 */
typedef struct
{
    uint8_t length;                  /**< psk length */
    uint8_t value[WSEC_MAX_PSK_LEN]; /**< actual psk value */
} wiced_psk_t;

/**
 * @brief The wifi credentials
 */
typedef struct wifi_cred
{
    wiced_ssid_t ssid;    /**< The network name */
    wiced_psk_t password; /**< The network password, can be \0 */
} wifi_cred_t;

/**
 * @brief The wifi return codes
 */
typedef enum wifi_connect_state
{
    WIFI_CONNECT_SUCCESS = 0, /**< Successfully connected to wifi network */
    WIFI_CONNECT_NO_CRED,     /**< No wifi credentials found in flash */
    WIFI_CONNECT_WRONG_CRED,  /**<  Wrong wifi credentials found in flash */
    WIFI_CONNECT_FAILED       /**< Failed to connect to wifi network because of other error */
} wifi_connect_state_t;

/**
 * @brief Initializes the WLAN parts of WICED
 */
void APP_NETWORK_Init(void);

/**
 * @brief Tries to get the wifi credentials from flash and connect to network
 *
 * @param use_dhcp: True if the IP address should be obtained from DHCP server
 * @param provideUxLed: True UX LED blinks during connection establishment
 *
 * @return          0 on success
 * @return          1 when no credentials found in flash
 * @return          2 when wrong credentials found in flash
 */
status_t APP_NETWORK_Wifi_Connect(bool use_dhcp, bool provideUxLed);

/**
 * @brief Starts the Wifi Access Point
 *
 * @param ap_ssid:      The AP SSID
 * @param ap_passwd:    The AP password (null-terminated string)
 *
 * @return              0 on success, 1 otherwise
 */
status_t APP_NETWORK_Wifi_StartAP(wiced_ssid_t ap_ssid, char *ap_passwd);

/**
 * @brief Stops the Wifi Access Point
 *
 * @return              0 on success, 1 otherwise
 */
status_t APP_NETWORK_Wifi_StopAP(void);

/**
 * @brief Checks if valid wifi credentials exist in flash
 *
 * @return              0 if credentials available, 1 otherwise
 */
status_t APP_NETWORK_Wifi_CheckCred(void);
/**
 * @brief A callback that signals the state of the wifi to the UX
 *
 * @param state         The state of the wifi
 */
void APP_Wifi_UX_Callback(network_wifi_conn_state_t state);

#elif USE_ETHERNET_CONNECTION
/**
 * @brief   Initializes the ethernet connection and gets the IP address from DHCP
 *          or uses a static ip address
 *
 * @param use_dhcp      A boolean that specifies if we initialize the connection
 *                      using a static IP or the DHCP server
 */
void APP_NETWORK_Init(bool use_dhcp);

typedef enum network_mode
{
    NETWORK_MODE_STATION /* Device is configured as Station (when either wifi or ethernet is used) */
} network_mode_t;

#endif /* USE_WIFI_CONNECTION */

/* Definition for the callback handler for connection state updates */
typedef void(connection_update_fn)(bool);

/**
 * @brief Blocks for link to re-establish. For WiFi this involves reconnecting to AccessPoint.
 * @return  0 on success, an error otherwise
 */
status_t APP_NETWORK_Re_Link(void);

/**
 * @brief Uninitialize the network connection
 *
 * In case of a ethernet connection (USE_ETHERNET_CONNECTION is set to 1) this function dose nothing
 * and in case of a wifi connection it leaves the wifi network
 */
void APP_NETWORK_Uninit(void);

/**
 * @brief This function gets the state of the wifi connection
 *
 * @return  true if connected to the network
 * @return  false if not connected to the network
 */
bool get_connect_state(void);

/**
 * @brief This function gets the ip address of the wifi connection
 *
 * @return the ipaddress. 0 if the board is not connected.
 */
ip_addr_t get_ipaddress(void);

/*!
 * @brief Generates an incremental backoff delay between MQTT connections
 *
 * @param retry_attempts number of retries that have occurred
 * @param base_delay is the minimum delay between connections
 *
 * @returns Number of ticks to delay between connections
 */
TickType_t get_mqtt_connection_backoff(uint32_t retry_attempts, uint32_t base_delay);

/*!
 * @brief Sets the connection update callback
 *
 * @param *func Callback to receive the update
 */
void APP_Connect_Update_Handler_Set(connection_update_fn *func);

#endif /* NETWORK_CONNECTION_H_ */
