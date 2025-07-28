/** @file supp_api.h
 *
 *  @brief  This file provides wpa supplicant APIs.
 *
 *  Copyright 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef RTOS_SUPP_MGMT_H
#define RTOS_SUPP_MGMT_H

#ifndef __ZEPHYR__
#include <lwip/netif.h>
#include <lwip/netifapi.h>
#endif
#include <wlan.h>

#define MAX_SSID_LEN 32
#define MAC_ADDR_LEN 6

enum requested_ops
{
    ADD = 0,
    REMOVE,
    CONNECT,
    DISCONNECT,
    RECONNECT,
    AUTOCONNECT,
    ROAM,
    FT_DS,
#if CONFIG_WPA_SUPP_WPS
    OP_WPS_PBC,
    OP_WPS_PIN,
    OP_WPS_CANCEL,
#endif
    START,
    STOP
};

#if CONFIG_11K
#define RRM_EVENT_NEIGHBOR_REP_COMPLETED "neighbor report parse completed"
#endif

int wpa_supp_init(void (*msg_cb)(const char *txt, size_t len));

int wpa_supp_status(const struct netif *dev);

int wpa_supp_scan(const struct netif *dev, wlan_scan_params_v2_t *params);

int wpa_supp_abort_scan(const struct netif *dev);

int wpa_supp_cancel_scan(const struct netif *dev);

int wpa_supp_disable(const struct netif *dev, struct wlan_network *network);

int wpa_supp_add_network(const struct netif *dev, struct wlan_network *network);
/**
 * @brief Request a connection
 *
 * @param iface_name: Wi-Fi interface name to use
 * @param params: Connection details
 *
 * @return: 0 for OK; -1 for ERROR
 */
int wpa_supp_connect(const struct netif *dev, struct wlan_network *network);
/**
 * @brief Forces station to disconnect and stops any subsequent scan
 *  or connection attempts
 *
 * @param iface_name: Wi-Fi interface name to use
 *
 * @return: 0 for OK; -1 for ERROR
 */
int wpa_supp_disconnect(const struct netif *dev);

int wpa_supp_remove_network(const struct netif *dev, struct wlan_network *network);

int wpa_supp_pmksa_list(const struct netif *dev, char *buf, size_t buflen);

int wpa_supp_pmksa_flush(const struct netif *dev);

int wpa_supp_reassociate(const struct netif *dev);

int wpa_supp_set_autoconnect(const struct netif *dev, int enable);

int wpa_supp_set_scan_interval(const struct netif *dev, int scan_int);

/**
 * @brief
 *
 * @param iface_name: Wi-Fi interface name to use
 * @param wifi_iface_status: Status structure to fill
 *
 * @return: 0 for OK; -1 for ERROR
 */
int wpa_supp_network_status(const struct netif *dev, struct wlan_network *network);

int wpa_supp_send_neighbor_rep(const struct netif *dev, char *pssid, int lci, int civic);

int wpa_supp_send_btm_query(const struct netif *dev, int query_reason);

int wpa_supp_mbo_update_non_pref_chan(const struct netif *dev, const char *non_pref_chan);

int wpa_supp_mbo_set_cell_capa(const struct netif *dev, unsigned char mbo_cell_capa);

int wpa_supp_mbo_set_oce(const struct netif *dev, unsigned char oce);

void wpa_supp_set_bgscan(const struct netif *dev, const int short_interval, const int signal_threshold, const int long_interval);

void wpa_supp_stop_bgscan(const struct netif *dev);

int wpa_supp_set_okc(const struct netif *dev, unsigned char okc);

int wpa_supp_roam(const struct netif *dev, unsigned char *bssid);

int wpa_supp_ft_ds(const struct netif *dev, unsigned char *bssid);

int wpa_supp_notify_assoc(const struct netif *dev);

int wpa_supp_req_status(enum requested_ops ops);

#if CONFIG_WPA_SUPP_WPS
int wpa_supp_start_wps_pbc(const struct netif *dev, int is_ap);
int wpa_supp_start_wps_pin(const struct netif *dev, const char *pin, int is_ap);
int wpa_supp_wps_pin_valid(const struct netif *dev, const unsigned char *pin);
int wpa_supp_wps_generate_pin(const struct netif *dev, unsigned int *pin);
int wpa_supp_cancel_wps(const struct netif *dev, int is_ap);
#endif

#if CONFIG_WPA_SUPP_DPP
int wpa_supp_dpp_configurator_add(const struct netif *dev, int is_ap, const char *cmd);

void wpa_supp_dpp_configurator_params(const struct netif *dev, int is_ap, const char *cmd);

void wpa_supp_dpp_mud_url(const struct netif *dev, int is_ap, const char *cmd);

int wpa_supp_dpp_bootstrap_gen(const struct netif *dev, int is_ap, const char *buf);

const char *wpa_supp_dpp_bootstrap_get_uri(const struct netif *dev, int is_ap, unsigned int id);

int wpa_supp_dpp_listen(const struct netif *dev, int is_ap, const char *cmd);

int wpa_supp_dpp_stop_listen(const struct netif *dev, int is_ap);

int wpa_supp_dpp_configurator_get_key(const struct netif *dev, int is_ap, unsigned int id, char *buf, size_t buflen);

int wpa_supp_dpp_qr_code(const struct netif *dev, int is_ap, char *cmd);

int wpa_supp_dpp_auth_init(const struct netif *dev, int is_ap, const char *cmd);

int wpa_supp_dpp_pkex_add(const struct netif *dev, int is_ap, const char *cmd);

int wpa_supp_dpp_chirp(const struct netif *dev, int is_ap, const char *cmd);

int wpa_supp_dpp_reconfig(const struct netif *dev, const char *cmd);

int wpa_supp_dpp_configurator_sign(const struct netif *dev, int is_ap, const char *cmd);
#endif

#if CONFIG_WPA_SUPP_P2P
int wpa_supp_p2p_find(const struct netif *dev, const char *cmd);
int wpa_supp_p2p_stop_find(const struct netif *dev);
int wpa_supp_p2p_listen(const struct netif *dev, const char *cmd);
int wpa_supp_p2p_connect(const struct netif *dev, char *cmd);
int wpa_supp_p2p_group_add(const struct netif *dev, char *cmd);
int wpa_supp_p2p_get_passphrase(const struct netif *dev);
int wpa_supp_p2p_invite(const struct netif *dev, char *cmd);
int wpa_supp_p2p_prov_disc(const struct netif *dev, char *cmd);
int wpas_supp_p2p_cancel(const struct netif *dev);
int wpa_supp_p2p_remove_client(const struct netif *dev, char *cmd);
int wpa_supp_p2p_update_security_info(struct wlan_network *network);
int wpa_supp_p2p_update_mode_info(struct wlan_network *network);
int wpa_supp_p2p_service_add(const struct netif *dev, char *cmd);
int wpa_supp_p2p_serv_disc_req(const struct netif *dev, char *cmd);
int wpa_supp_p2p_serv_disc_resp(const struct netif *dev, char *cmd);
int wpa_supp_p2p_group_remove(const struct netif *dev, char *cmd);
int wpa_supp_p2p_peer(const struct netif *dev, char *cmd, char *buf, size_t buflen);
int wpa_supp_p2p_status(const struct netif *dev, char *buf, size_t buflen);
#endif

#if CONFIG_HOSTAPD
int wpa_supp_start_ap(const struct netif *dev, struct wlan_network *network, int reload);

void wpa_supp_set_ap_max_num_sta(const struct netif *dev, unsigned int max_num_sta);

void wpa_supp_set_ap_beacon_int(const struct netif *dev, unsigned short beacon_int);

void wpa_supp_set_ap_max_num_sta(const struct netif *dev, unsigned int max_num_sta);

void wpa_supp_set_ap_bw(const struct netif *dev, unsigned char bw);

void wpa_supp_set_ap_hidden_ssid(const struct netif *dev, const t_u8 hidden_ssid);

int wpa_supp_set_ap_country(const struct netif *dev, const char *country, const unsigned char country3);

void wpa_supp_set_ap_11d_state(const struct netif *dev, int state);

int wpa_supp_stop_ap(const struct netif *dev, struct wlan_network *network);

int wpa_supp_set_mac_acl(const struct netif *dev, int filter_mode, char mac_count, unsigned char *mac_addr);

int wpa_supp_deauth_sta(struct netif *dev, const u8* sta_addr);

void wpa_supp_notify_acs(const struct netif *dev);

int wpa_supp_get_sta_info(const struct netif *dev, unsigned char *sta_addr, unsigned char *is_11n_enabled);
#endif

int wpa_supp_deinit(void);

#endif /* RTOS_SUPP_MGMT_H */
