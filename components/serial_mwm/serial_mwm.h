/*
 * Copyright 2020-2021, NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SERIAL_MWM_H_
#define _SERIAL_MWM_H_

#include <stdio.h>
#include <stdint.h>

/*!
 * @defgroup serial_mwm SerialMWM
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Socket type */
#define MWM_TCP 0
#define MWM_UDP 1

/* State codes */
#define MWM_INITIALIZED       350
#define MWM_PROVISIONING      351
#define MWM_PROVISIONING_DONE 352
#define MWM_CONNECTING        353
#define MWM_CONNECTED         354
#define MWM_CONNECT_FAILED    355
#define MWM_DISCONNECTED      356

/* Authentication failed */
#define MWM_AUTH_FAILED 381
/* Network not found */
#define MWM_NETWORK_NOT_FOUND 382

#define MWM_SOCKET_ERROR -700
#define EAGAIN           11

/* Device Configuration */
/* Provisioning Configuration */
#define MWM_MOD_PROV          "prov"
#define MWM_PROV_SSID         "ssid"
#define MWM_PROV_SECURITY     "security"
#define MWM_PROV_PASSPHRASE   "passphrase"
#define MWM_PROV_MDNS_ENABLED "mdns_enabled"
#define MWM_PROV_DEVICE_KEY   "device_key"
/* WLAN Configuration */
#define MWM_MOD_WLAN                  "wlan"
#define MWM_WLAN_SSID                 "ssid"
#define MWM_WLAN_BSSID                "bssid"
#define MWM_WLAN_SECURITY             "security"
#define MWM_WLAN_PASSPHRASE           "passphrase"
#define MWM_WLAN_CHANNEL              "channel"
#define MWM_WLAN_CONFIGURED           "configured"
#define MWM_WLAN_RECONNECT_DELAY      "reconnect_delay"
#define MWM_WLAN_RECONNECT_ATTEMPTS   "reconnect_attempts"
#define MWM_WLAN_IPADDR_TYPE          "ipaddr_type"
#define MWM_WLAN_IPADDR               "ipaddr"
#define MWM_WLAN_IPADDR_GATEWAY       "gateway"
#define MWM_WLAN_IPADDR_NETMASK       "netmask"
#define MWM_WLAN_IPADDR_DNS1          "dns1"
#define MWM_WLAN_IPADDR_DNS2          "dns2"
#define MWM_WLAN_CHECK_SERVER_CERT    "chk_server_cert"
#define MWM_WLAN_VALID_CERT_PARTITION "valid_cert_partition"
#define MWM_WLAN_MAC                  "mac"
#define MWM_WLAN_REGION_CODE          "region_code"
/* System Configuration */
#define MWM_MOD_SYSTEM                  "system"
#define MWM_SYS_IEEE_LISTEN_INTERVAL    "ieee_listen_interval"
#define MWM_SYS_PROTO_UART_BAUDRATE     "proto_uart_baudrate"
#define MWM_SYS_PROTO_UART_STOPBITS     "proto_uart_stopbits"
#define MWM_SYS_PROTO_UART_PARITY       "proto_uart_parity"
#define MWM_SYS_PROTO_UART_FLOW_CONTROL "proto_uart_flow_control"
#define MWM_SYS_PM_EXT_GPIO             "pm_ext_gpio"
#define MWM_SYS_PM_EXT_GPIO_LEVEL       "pm_ext_gpio_level"
#define MWM_SYS_HOST_WK_GPIONO          "host_wk_gpio"
#define MWM_SYS_HOST_WK_GPIO_LEVEL      "host_wk_gpio_level"
#define MWM_SYS_HOST_WK_DELAY           "host_wk_delay"
#define MWM_SYS_HOSTNAME                "hostname"
#define MWM_SYS_PROTO_UART_ID           "proto_uart_id"
#define MWM_SYS_DEBUG_UART_ID           "debug_uart_id"

/* Firmware Upgrade */
#define MWM_UPGRADE_MCU_FW  "fw"
#define MWM_UPGRADE_WLAN_FW "wf"
#define MWM_UPGRADE_STATUS  "?"

#define MWM_UPGRADE_NOT_STARTED 213
#define MWM_UPGRADE_STARTED     214
#define MWM_UPGRADE_IN_PROGRESS 215
#define MWM_UPGRADE_SUCCESSFUL  216

/*! @brief SerialMWM socket address structure. */
typedef struct
{
    char host[64]; /*!< Hostname or IP Address */
    uint16_t port;
} mwm_sockaddr_t;

/*! @brief SerialMWM WLAN structure. */
typedef struct
{
    char bssid[18];
    char ssid[33];
    uint8_t channel;
    int8_t rssi;
    uint8_t security;
} mwm_wlan_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization
 * @{
 */

/*!
 * @brief Initializes SerialMWM library.
 *
 * This function resets connected Wi-Fi module by reset pin (if supported) and initializes
 * the UART interface for communication with Wi-Fi module.
 *
 * @retval -1 Error
 * @retval  0 Success
 */
int mwm_init(void);

/* @} */

/*!
 * @name WLAN API
 * @{
 */

/*!
 * @brief Gets SerialMWM firmware version.
 *
 * This function gets SerialMWM firmware version of the Wi-Fi module.
 *
 * @param value Pointer to buffer where the version string will be stored
 * @param val_len Length of value buffer
 * @return 0 - success, negative number - error
 */
int mwm_version(char *value, uint32_t val_len);

/*!
 * @brief Upgrades firmware of Wi-Fi module.
 *
 * This function upgrades SerialMWM firmware, WLAN firmware or it can be used to check the upgrade status.
 *
 * @param type MWM_UPGRADE_MCU_FW for SerialMWM FW upgrade, MWM_UPGRADE_WLAN_FW for WLAN FW upgrade or
 * MWM_UPGRADE_STATUS to check upgrade status
 * @param url URL of firmware image file for upgrade or NULL in case of upgrade status check
 * @return 0 - success, negative number - error
 */
int mwm_upgrade(char *type, char *url);

/*!
 * @brief Reboots Wi-Fi module.
 *
 * This function reboots Wi-Fi module and re-initializes UART interface.
 *
 * @return 0 - success, negative number - error
 */
int mwm_reboot(void);
/* @} */

/*!
 * @name WLAN API
 * @{
 */

/*!
 * @brief Gets WLAN firmware version.
 *
 * This function gets WLAN firmware version of the Wi-Fi module.
 *
 * @param value Pointer to buffer where the version string will be stored
 * @param val_len Length of value buffer
 * @return 0 - success, negative number - error
 */
int mwm_wlan_fw_version(char *value, uint32_t val_len);

/*!
 * @brief Start WLAN subsystem.
 *
 * @return 0 - success, negative number - error
 */
int mwm_wlan_start(void);

/*!
 * @brief Gets WLAN status.
 *
 * @return 0 - success, negative number - error
 */
int mwm_wlan_status(void);

/*!
 * @brief Resets WLAN module to provisioning mode.
 *
 * @return 0 - success, negative number - error
 */
int mwm_wlan_prov(void);

/*!
 * @brief Gets WLAN information.
 *
 * This function gets WLAN information (SSID and IP Address) from the Wi-Fi module.
 *
 * @param ssid Pointer to buffer where the SSID string will be stored
 * @param ip_addr Pointer to buffer where the IP Address string will be stored
 * @return 0 - success, negative number - error
 */
int mwm_wlan_info(char *ssid, char *ip_addr);

/*!
 * @brief Scans WLAN.
 *
 * This function gets cached scan results and schedules a new scan.
 *
 * @param wlans Pointer to array of mwm_wlan_t where the WLAN struct will be stored
 * @param wlan_count Length of wlans array
 * @return 0 - success, negative number - error
 */
int mwm_wlan_scan(mwm_wlan_t *wlans, uint32_t wlan_count);

/*!
 * @brief Connects to WLAN.
 *
 * This function attempts to make an explicit connection with the configured WLAN.
 *
 * @return 0 - success, negative number - error
 */
int mwm_wlan_connect(void);

/*!
 * @brief Disconnects from WLAN.
 *
 * This function disconnects Wi-Fi module from WLAN it is connected to.
 *
 * @return 0 - success, negative number - error
 */
int mwm_wlan_disconnect(void);

/* @} */

/*!
 * @name Device Configuration API
 * @{
 */

/*!
 * @brief Gets parameter from Wi-Fi module.
 *
 * This function gets parameter from Wi-Fi module.
 *
 * @param module Module of requested parameter, it can be MWM_MOD_PROV, MWM_MOD_WLAN or MWM_MOD_SYSTEM
 * @param param Name of requested parameter, it can be MWM_PROV_*, MWM_WLAN_* or MWM_SYS_* based on selected module
 * @param value Pointer to buffer where the value of requested parameter will be stored
 * @param val_len Length of value buffer
 * @return 0 - success, negative number - error
 */
int mwm_get_param(char *module, char *param, char *value, uint32_t val_len);

/*!
 * @brief Sets parameter from Wi-Fi module.
 *
 * This function sets parameter of Wi-Fi module.
 *
 * @param module Module whose parameter will be set, it can be MWM_MOD_PROV, MWM_MOD_WLAN or MWM_MOD_SYSTEM
 * @param param Name of configured parameter, it can be MWM_PROV_*, MWM_WLAN_* or MWM_SYS_* based on selected module
 * @param value Pointer to string value of configured parameter
 * @return 0 - success, negative number - error
 */
int mwm_set_param(char *module, char *param, char *value);
/* @} */

/*!
 * @name Socket API
 * @{
 */

/*!
 * @brief Opens socket.
 *
 * This function opens socket.
 *
 * @param type Type of socket, it can be MWM_TCP or MWM_UDP
 * @return handle of socket, negative number - error
 */
int mwm_socket(int type);

/*!
 * @brief Opens socket.
 *
 * This function connects the socket to remote host.
 *
 * @param socket Handle of socket
 * @param addr Valid address of remote host
 * @param addrlen Size of addr
 * @return 0 - success, negative number - error
 */
int mwm_connect(int socket, mwm_sockaddr_t *addr, uint32_t addrlen);

/*!
 * @brief Sends data.
 *
 * This function sends data over a connected socket.
 *
 * @param socket Handle of socket
 * @param buf Pointer to data buffer
 * @param len Size of buf
 * @return Number of bytes sent, negative number - error
 */
int mwm_send(int socket, void *buf, size_t len);

/*!
 * @brief Sends data.
 *
 * This function sends data to a specified destination host and port (mwm_sockaddr_t).
 *
 * @param socket Handle of socket
 * @param buf Pointer to data buffer
 * @param len Size of buf
 * @param to Address of remote host
 * @param tolen Size of to
 * @return Number of bytes sent, negative number - error
 */
int mwm_sendto(int socket, char *buf, int len, mwm_sockaddr_t *to, int tolen);

/*!
 * @brief Receives data.
 *
 * This function receives data on a connected socket.
 *
 * @param socket Handle of socket
 * @param buf Pointer to data buffer
 * @param len Size of buf
 * @param timeout_ms Timeout in ms - maximum amount of time that the request may block while waiting for data, 0
 * indicates it blocks forever
 * @return Number of bytes received, negative number - error
 */
int mwm_recv_timeout(int socket, void *buf, size_t len, int timeout_ms);

/*!
 * @brief Receives data.
 *
 * This function receives data on the opened socket.
 *
 * @param socket Handle of socket
 * @param buf Pointer to data buffer
 * @param len Size of buf
 * @param timeout_ms Timeout in ms - maximum amount of time that the request may block while waiting for data, 0
 * indicates it blocks forever
 * @return Number of bytes received, negative number - error
 */
int mwm_recvfrom_timeout(int socket, void *buf, size_t len, int timeout_ms);

/*!
 * @brief Closes socket.
 *
 * This function closes a socket.
 *
 * @param socket Handle of socket
 * @return 0 - success, negative number - error
 */
int mwm_close(int socket);
/* @} */

#if defined(__cplusplus)
}
#endif

/* @} */
#endif /* _SERIAL_MWM_H_ */
