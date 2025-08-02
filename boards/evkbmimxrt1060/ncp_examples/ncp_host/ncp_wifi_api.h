/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __NCP_WIFI_API__
#define __NCP_WIFI_API__

#include "ncp_host_command_wifi.h"
#include "ncp_cmd_node.h"

/** Wifi current network */
typedef struct ncp_current_network_t
{
    /** Get current network result */
    uint16_t result;
    /** WLAN Network Profile */
    NCP_CMD_GET_CURRENT_NETWORK sta_network;
} ncp_current_network;

/** MAC address */
typedef struct ncp_get_mac_addr_t
{
    /** Get MAC address result */
    uint16_t result;
    /** MAC address */
    NCP_CMD_GET_MAC_ADDRESS mac_addr;
} ncp_get_mac_addr;

/** Wi-Fi Statistics counter */
typedef struct ncp_pkt_stats_t
{
    /** Get pkt stats result */
    uint16_t result;
    /** Pkt stats */
    NCP_CMD_PKT_STATS pkt_stats;
} ncp_pkt_stats;

/** MAC address */
typedef struct ncp_mac_addr_t
{
    /** Mac address array */
    char mac[MLAN_MAC_ADDR_LENGTH];
} ncp_mac_addr;

/** Network ip configuration */
typedef struct ncp_ip_config_t
{
    /** Get ip config result */
    uint16_t result;
    /** Network IP configuration. */
    NCP_CMD_IP_CONFIG ip_config;
} ncp_ip_config;

/**
 * This API can be used to get connection state.
 *
 * \return connection state.
 */
char * wlan_ncp_get_state(void);

/**
 * Remove a network profile from the list of known networks.
 *
 * \param[in] network    structure of network profile.
 *
 * \return TRUE if success.
 * \return FALSE if failure.
 */
bool wlan_ncp_remove_network(NCP_WLAN_NETWORK * network);

/**
 * Add a network profile to the list of known networks.
 *
 * \note This API can be used to add profiles for station or UAP interfaces.
 *
 * \param[in] ssid      The network SSID, represented as a C string of up to 32 characters in length. \n
 * \param[in] key       The network key, the lenth is set by key_len. \n
 * \param[in] mode      The value of security mode \n
 *                      The network security configuration specified for the network \n
 * \param[in] frequency The network frequency, set as channel \n
 * \param[in] network_name
 *                      profile name, the name of network profile \n
 * \param[in] key_len   The value of key_len, the lenth of network key \n
 *                    
 * \return TRUE if success.
 * \return FALSE if failure.
 */
bool wlan_ncp_add_network(char * ssid, char * key, int8_t mode, int8_t frequency, char * network_name, int8_t key_len);

/**
 * Disconnect from the current Wi-Fi network (access point).
 *
 * \return TRUE if success.
 * \return FALSE if failure.
 */
bool wlan_ncp_disconnect(void);

/**
 * Scan for Wi-Fi networks.
 *
 * \return TRUE if success.
 * \return FALSE if failure.
 */
bool wlan_ncp_scan(void);

/**
 * Get the count of network profile from scan result.
 *
 * * \param[out] count  Number of networks from scan result list
 * 
 * \return TRUE if success.
 * \return FALSE if failure.
 */
bool wlan_ncp_get_scan_result_count(uint8_t * count);

/**
 * Connect to a Wi-Fi network (access point).
 *
 * \param[in] network_name    string value of name \n
 *                            A string representing the name of the network to connect to.
 * 
 * \return TRUE if success.
 * \return FALSE if failure.
 */
bool wlan_ncp_connect(char * network_name);

/**
 * Get current network profile which is connected.
 *
 * \param[in] network_name    string value of name \n
 *                            A string representing the name of the network to connect to.
 * 
 * \return TRUE if success.
 * \return FALSE if failure.
 */
int wlan_ncp_get_current_network(NCP_WLAN_NETWORK * net_work);

/**
 * Remove all network profiles.
 *
 * \return TRUE if success.
 * \return FALSE if failure.
 */
bool wlan_ncp_remove_all_networks(void);

/**
 * Get Wi-Fi MAC Address in Wi-Fi firmware.
 *
 * \param[out] dest    The Wi-Fi MAC Address get from Wi-Fi firmware \n
 * 
 * \return TRUE if success.
 * \return FALSE if failure.
 */
int wlan_ncp_get_mac_address(unsigned char * dest);

/**
 * Get Wi-Fi Statistics counter.
 *
 * \param[out] stats    The Wi-Fi Statistics counter \n
 * 
 * \return TRUE if success.
 * \return FALSE if failure.
 */
int wlan_ncp_get_pkt_stats(NCP_CMD_PKT_STATS *stats);

/**
 * Get scan result.
 *
 * \param[in]  index    The index from the scan table \n
 * \param[out] res      Get the scan result from the scan table as the index \n
 * 
 * \return TRUE if success.
 * \return FALSE if failure.
 */
int wlan_ncp_get_scan_result(unsigned int index, NCP_WLAN_SCAN_RESULT * res);

/**
 * Get rssi.
 *
 * \param[out] rssi     The signal strength of the beacon \n
 * 
 * \return TRUE if success.
 * \return FALSE if failure.
 */
int wlan_ncp_get_current_rssi(short * rssi);

/**
 * Get channel number.
 *
 * \return Channel number.
 */
uint8_t wlan_ncp_get_current_channel(void);

/**
 * Get Network IP configuration.
 *
 * \param[out] ip_config     The Network IP configuration \n
 * 
 * \return TRUE if success.
 * \return FALSE if failure.
 */
int wlan_ncp_get_ip_config(NCP_CMD_IP_CONFIG * ip_config);
#endif /* __NCP_WIFI_API__ */