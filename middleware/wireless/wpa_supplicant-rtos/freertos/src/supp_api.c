/** @file supp_api.c
 *
 *  @brief  This file provides wpa supplicant APIs.
 *
 *  Copyright 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fsl_os_abstraction.h"

#include "includes.h"
#include "common.h"
#include "common/defs.h"
#include "common/ptksa_cache.h"
#include "wpa_supplicant/config.h"
#include "wpa_supplicant_i.h"
#include "driver_i.h"
#include "ap.h"
#include "crc32.h"

#include "supp_main.h"
#include "supp_api.h"
#include "wpa_supplicant/bss.h"
#include "wpa_supplicant/wps_supplicant.h"
#include "rsn_supp/wpa.h"
#include "wpa_supplicant/wnm_sta.h"
#include "eap_defs.h"
#include "eap.h"
#include "crypto/tls.h"
#include "wpa_supplicant/scan.h"
#include "wpa_supplicant/sme.h"

#ifdef CONFIG_HOSTAPD
#include "ap/hostapd.h"
#include "ap/ap_config.h"
#include "ap/wps_hostapd.h"
#include "ap/sta_info.h"
#endif

#define EAP_TTLS_AUTH_PAP      1
#define EAP_TTLS_AUTH_CHAP     2
#define EAP_TTLS_AUTH_MSCHAP   4
#define EAP_TTLS_AUTH_MSCHAPV2 8

enum supp_thread_state
{
    SUPP_THREAD_STOPPED = 0,
    SUPP_THREAD_RUNNING,
};

unsigned char bandwidth = 2;
unsigned char h_hidden_ssid = 0;
unsigned short h_beacon_int = 100;
int h_max_num_sta = 8;

int cli_main(int, const char **);

OSA_SEMAPHORE_HANDLE_DEFINE(wpaSuppReadySemaphoreHandle);

#ifdef CONFIG_HOSTAPD
OSA_SEMAPHORE_HANDLE_DEFINE(hostapdReadySemaphoreHandle);
#endif

extern struct wpa_global *global;

OSA_MUTEX_HANDLE_DEFINE(wpa_supplicant_mutex);

struct wpa_supp_api_ctrl
{
    const struct netif *dev;
    enum requested_ops requested_op;
    enum supp_thread_state supp_thread_state;
};

static struct wpa_supp_api_ctrl wpa_supp_api_ctrl;
static void wpa_supp_scan_res_fail_handler(struct wpa_supplicant *wpa_s);

static inline struct wpa_supplicant *get_wpa_s_handle(const struct netif *dev)
{
    struct wpa_supplicant *wpa_s = NULL;
    int ret = OSA_SemaphoreWait((osa_semaphore_handle_t)wpaSuppReadySemaphoreHandle, osaWaitForever_c);
    char ifname[NETIF_NAMESIZE];

    if (ret)
    {
        wpa_printf(MSG_DEBUG, "%s: WPA supplicant not ready: %d", __func__, ret);
        return NULL;
    }

    OSA_SemaphorePost((osa_semaphore_handle_t)wpaSuppReadySemaphoreHandle);

    (void)netifapi_netif_index_to_name(dev->num + 1, ifname);

    wpa_s = wpa_supplicant_get_iface(global, ifname);
    if (!wpa_s)
    {
        wpa_printf(MSG_DEBUG, "%s: Unable to get wpa_s handle for %s", __func__, dev->name);
        return NULL;
    }

    return wpa_s;
}

#ifdef CONFIG_HOSTAPD
static inline struct hostapd_iface *get_hostapd_handle(const struct netif *dev)
{
    struct hostapd_iface *hapd_s = NULL;
    int ret = OSA_SemaphoreWait((osa_semaphore_handle_t)hostapdReadySemaphoreHandle, osaWaitForever_c);
    char ifname[NETIF_NAMESIZE];

    if (ret)
    {
        wpa_printf(MSG_DEBUG, "%s: HOSTAPD not ready: %d", __func__, ret);
        return NULL;
    }

    OSA_SemaphorePost((osa_semaphore_handle_t)hostapdReadySemaphoreHandle);

    (void)netifapi_netif_index_to_name(dev->num + 1, ifname);

    hapd_s = hostapd_get_interface(ifname);
    if (!hapd_s)
    {
        wpa_printf(MSG_DEBUG, "%s: Unable to get hapd_s handle for %s", __func__, dev->name);
        return NULL;
    }

    return hapd_s;
}
#endif

int wpa_supp_req_status(enum requested_ops ops)
{
    int status = -1;
    struct wpa_supplicant *wpa_s;
    struct wpa_supp_api_ctrl *ctrl = &wpa_supp_api_ctrl;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

#ifdef CONFIG_HOSTAPD
    if (ops == START)
    {
        struct hostapd_iface *hapd_s;

        hapd_s = get_hostapd_handle(ctrl->dev);
        if (!hapd_s)
        {
            status = -1;
            goto out;
        }

        if (hapd_s->state == HAPD_IFACE_ENABLED)
            status = 0;
    }
    else
    {
#endif
        wpa_s = get_wpa_s_handle(ctrl->dev);
        if (!wpa_s)
        {
            goto out;
        }

        if ((ctrl->requested_op == CONNECT) || (ctrl->requested_op == START) || (ctrl->requested_op == RECONNECT))
        {
            if (wpa_s->wpa_state == WPA_COMPLETED)
                status = 0;
        }
#ifdef CONFIG_HOSTAPD
    }
#endif

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);
    return status;
}

static int *freq_range_to_channel_list(struct wpa_supplicant *wpa_s, char *val)
{
    struct wpa_freq_range_list ranges;
    int *freqs = NULL;
    struct hostapd_hw_modes *mode;
    u16 i;

    if (wpa_s->hw.modes == NULL)
        return NULL;

    os_memset(&ranges, 0, sizeof(ranges));
    if (freq_range_list_parse(&ranges, val) < 0)
        return NULL;

    for (i = 0; i < wpa_s->hw.num_modes; i++)
    {
        int j;

        // Skip HOSTAPD_MODE_IEEE80211B
        if (i == HOSTAPD_MODE_IEEE80211B)
        {
            continue;
        }

        mode = &wpa_s->hw.modes[i];
        for (j = 0; j < mode->num_channels; j++)
        {
            unsigned int freq;

            if (mode->channels[j].flag & HOSTAPD_CHAN_DISABLED)
                continue;

            freq = mode->channels[j].freq;
            if (!freq_range_list_includes(&ranges, freq))
                continue;

            int_array_add_unique(&freqs, freq);
        }
    }

    os_free(ranges.range);
    return freqs;
}

int wpa_supp_scan(const struct netif *dev, wlan_scan_params_v2_t *params)
{
    unsigned int manual_scan_passive  = 0;
    unsigned int manual_scan_use_id   = 0;
    unsigned int manual_scan_only_new = 1;
    unsigned int scan_only            = 0;
    unsigned int scan_id_count        = 0;
    int scan_id[MAX_SCAN_ID]          = {0};
    void (*scan_res_handler)(struct wpa_supplicant * wpa_s, struct wpa_scan_results * scan_res);
    int *manual_scan_freqs      = NULL;
    struct wpa_ssid_value *ssid = NULL;
    unsigned int ssid_count     = 0;
    char frequency[256]         = {0};
    char *pos                   = frequency;
    struct wpa_supplicant *wpa_s;
    int ret = 0, i, freq;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    if (wpa_s->wpa_state == WPA_INTERFACE_DISABLED)
    {
        ret = -1;
        goto out;
    }

    if (radio_work_pending(wpa_s, "scan"))
    {
        wpa_printf(MSG_DEBUG, "Pending scan scheduled - reject new request");
        ret = -1;
        goto out;
    }

    if (params)
    {
        if (params->scan_only)
            scan_only = 1;

        if (params->num_channels)
        {
            for (i = 0; i < params->num_channels; i++)
            {
                freq = ieee80211_chan_to_freq(NULL, 81, params->chan_list[i].chan_number);
                if (freq <= 0)
                {
                    freq = ieee80211_chan_to_freq(NULL, 128, params->chan_list[i].chan_number);
                }
                if (freq <= 0)
                {
                    wpa_printf(MSG_DEBUG, "Invalid channel %d", params->chan_list[i].chan_number);
                    ret = -1;
                    goto out;
                }
                if (i == (params->num_channels - 1))
                {
                    (void)sprintf(pos, "%d", freq);
                }
                else
                {
                    ret = sprintf(pos, "%d,", freq);
                    if (ret < 0)
                    {
                        ret = -1;
                        goto out;
                    }
                    pos += ret;
                }
            }

            manual_scan_freqs = freq_range_to_channel_list(wpa_s, frequency);
            if (manual_scan_freqs == NULL)
            {
                ret = -1;
                goto out;
            }
        }

        if (params->is_bssid)
        {
            os_memcpy(wpa_s->next_scan_bssid, params->bssid, ETH_ALEN);
            wpa_s->next_scan_bssid_wildcard_ssid = 0;
        }

        if (params->is_ssid)
        {
            ssid = os_zalloc(sizeof(*ssid));
            if (ssid == NULL)
            {
                ret = -1;
                goto out;
            }
            ssid->ssid_len = os_strlen((const char *)&params->ssid[0]);
            os_memcpy(ssid->ssid, &params->ssid[0], ssid->ssid_len);
            wpa_hexdump_ascii(MSG_DEBUG, "scan SSID", ssid->ssid, ssid->ssid_len);
            ssid_count++;
        }
    }

    wpa_s->num_ssids_from_scan_req = ssid_count;
    os_free(wpa_s->ssids_from_scan_req);
    if (ssid_count)
    {
        wpa_s->ssids_from_scan_req = ssid;
    }
    else
    {
        wpa_s->ssids_from_scan_req = NULL;
    }

    if (scan_only)
        scan_res_handler = scan_only_handler;
    else if (wpa_s->scan_res_handler == scan_only_handler)
        scan_res_handler = NULL;
    else
        scan_res_handler = wpa_s->scan_res_handler;

    if (!wpa_s->sched_scanning && !wpa_s->scanning &&
        ((wpa_s->wpa_state <= WPA_SCANNING) || (wpa_s->wpa_state == WPA_COMPLETED)))
    {
        wpa_s->manual_scan_passive  = manual_scan_passive;
        wpa_s->manual_scan_use_id   = manual_scan_use_id;
        wpa_s->manual_scan_only_new = manual_scan_only_new;
        wpa_s->scan_id_count        = scan_id_count;
        os_memcpy(wpa_s->scan_id, scan_id, scan_id_count * sizeof(int));
        wpa_s->scan_res_handler = scan_res_handler;
        os_free(wpa_s->manual_scan_freqs);
        wpa_s->manual_scan_freqs = manual_scan_freqs;
        manual_scan_freqs        = NULL;

        wpa_s->normal_scans   = 0;
        wpa_s->scan_req       = MANUAL_SCAN_REQ;
        wpa_s->after_wps      = 0;
        wpa_s->known_wps_freq = 0;
        wpa_supplicant_req_scan(wpa_s, 0, 0);
        if (wpa_s->manual_scan_use_id)
        {
            wpa_s->manual_scan_id++;
            wpa_dbg(wpa_s, MSG_DEBUG, "Assigned scan id %u", wpa_s->manual_scan_id);
        }
    }
    else if (wpa_s->sched_scanning)
    {
        wpa_s->manual_scan_passive  = manual_scan_passive;
        wpa_s->manual_scan_use_id   = manual_scan_use_id;
        wpa_s->manual_scan_only_new = manual_scan_only_new;
        wpa_s->scan_id_count        = scan_id_count;
        os_memcpy(wpa_s->scan_id, scan_id, scan_id_count * sizeof(int));
        wpa_s->scan_res_handler = scan_res_handler;
        os_free(wpa_s->manual_scan_freqs);
        wpa_s->manual_scan_freqs = manual_scan_freqs;
        manual_scan_freqs        = NULL;

        wpa_printf(MSG_DEBUG, "Stop ongoing sched_scan to allow requested full scan to proceed");
        wpa_supplicant_cancel_sched_scan(wpa_s);
        wpa_s->scan_req = MANUAL_SCAN_REQ;
        wpa_supplicant_req_scan(wpa_s, 0, 0);
        if (wpa_s->manual_scan_use_id)
        {
            wpa_s->manual_scan_id++;
            wpa_dbg(wpa_s, MSG_DEBUG, "Assigned scan id %u", wpa_s->manual_scan_id);
        }
    }
    else
    {
        wpa_printf(MSG_DEBUG, "Ongoing scan action - reject new request");
        ret = -1;
        goto out;
    }

    send_wpa_supplicant_dummy_event();
    ret = 0;

out:
    if (ret != 0)
    {
        os_free(manual_scan_freqs);
    }

    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_abort_scan(const struct netif *dev)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    wpa_s->scan_res_fail_handler = wpa_supp_scan_res_fail_handler;

    ret = wpas_abort_ongoing_scan(wpa_s);
    if (ret != 0)
    {
        goto out;
    }
    send_wpa_supplicant_dummy_event();

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;

}

#ifdef CONFIG_HOSTAPD
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE

static int wpa_config_process_blob(struct wpa_config *config, char *name, u8 *data, size_t data_len)
{
    struct wpa_config_blob *blob;

    blob = os_zalloc(sizeof(*blob));
    if (blob == NULL)
    {
        return -1;
    }

    blob->data = os_zalloc(data_len);
    if (blob->data == NULL)
    {
        os_free(blob);
        return -1;
    }

    blob->name = os_strdup(name);

    if (blob->name == NULL)
    {
        wpa_config_free_blob(blob);
        return -1;
    }

    os_memcpy(blob->data, data, data_len);
    blob->len = data_len;

    wpa_config_set_blob(config, blob);

    return 0;
}

static struct hostapd_eap_user *hostapd_config_read_default_eap_user(struct wlan_network *network,
                                                                     struct hostapd_eap_user **pnew_user)
{
    struct hostapd_eap_user *user = NULL, *tail = NULL, *new_user = NULL;
#if 0
    size_t i;
    unsigned char *id[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8"};
#endif

    user = os_zalloc(sizeof(*user));
    if (user == NULL)
    {
        wpa_printf(MSG_DEBUG, "EAP user allocation failed");
        goto failed;
    }
    user->force_version = -1;

    if ((network->security.type == WLAN_SECURITY_EAP_PEAP_MSCHAPV2) ||
        (network->security.type == WLAN_SECURITY_EAP_PEAP_TLS) ||
        (network->security.type == WLAN_SECURITY_EAP_PEAP_GTC))
    {
        user->methods[0].method = EAP_TYPE_PEAP;
        user->methods[0].vendor = EAP_VENDOR_IETF;
    }
    else if ((network->security.type == WLAN_SECURITY_EAP_TTLS_MSCHAPV2) ||
             (network->security.type == WLAN_SECURITY_EAP_TTLS))
    {
        user->methods[0].method = EAP_TYPE_TTLS;
        user->methods[0].vendor = EAP_VENDOR_IETF;
    }
    else if ((network->security.type == WLAN_SECURITY_EAP_FAST_MSCHAPV2) ||
             (network->security.type == WLAN_SECURITY_EAP_FAST_GTC))
    {
        user->methods[0].method = EAP_TYPE_FAST;
        user->methods[0].vendor = EAP_VENDOR_IETF;
    }

    if (tail == NULL)
    {
        tail = new_user = user;
    }
    else
    {
        tail->next = user;
        tail       = user;
    }

#if 0
    for (i = 0; i < 9; i++)
    {
        user = os_zalloc(sizeof(*user));
        if (user == NULL)
        {
            wpa_printf(MSG_DEBUG, "EAP user allocation failed");
            goto failed;
        }
        user->force_version = -1;

        user->identity = os_memdup(id[i], os_strlen(id[i]));
        if (user->identity == NULL)
        {
            wpa_printf(MSG_DEBUG,
                       "Failed to allocate "
                       "memory for EAP identity");
            goto failed;
        }

        user->identity_len    = os_strlen(id[i]);
        user->wildcard_prefix = 1;

        user->methods[0].method = EAP_TYPE_PEAP;
        user->methods[0].vendor = EAP_VENDOR_IETF;
        user->methods[1].method = EAP_TYPE_TTLS;
        user->methods[1].vendor = EAP_VENDOR_IETF;
        user->methods[2].method = EAP_TYPE_TLS;
        user->methods[2].vendor = EAP_VENDOR_IETF;
        user->methods[3].method = EAP_TYPE_SIM;
        user->methods[3].vendor = EAP_VENDOR_IETF;
        user->methods[4].method = EAP_TYPE_AKA;
        user->methods[4].vendor = EAP_VENDOR_IETF;

        if (tail == NULL)
        {
            tail = new_user = user;
        }
        else
        {
            tail->next = user;
            tail       = user;
        }
    }
#endif

    *pnew_user = new_user;

    return tail;

failed:
    if (new_user)
        hostapd_config_free_eap_user(new_user);

    return NULL;
}

static int hostapd_config_read_eap_user(const size_t nusers,
                                        const char (*identities)[IDENTITY_MAX_LENGTH],
                                        struct hostapd_bss_config *conf)
{
    struct hostapd_eap_user *user = NULL, *tail = NULL, *new_user = NULL;
    size_t i;
    int ret = 0;

    if (!nusers)
    {
        wpa_printf(MSG_DEBUG, "EAP users not found.");
        return -1;
    }

    for (i = 0; i < nusers; i++)
    {
        user = os_zalloc(sizeof(*user));
        if (user == NULL)
        {
            wpa_printf(MSG_DEBUG, "EAP user allocation failed");
            goto failed;
        }
        user->force_version = -1;

        user->identity = os_memdup(identities[i], os_strlen(identities[i]));
        if (user->identity == NULL)
        {
            wpa_printf(MSG_DEBUG,
                       "Failed to allocate "
                       "memory for EAP identity");
            goto failed;
        }

        user->identity_len = os_strlen(identities[i]);

        user->methods[0].method = EAP_TYPE_TLS;
        user->methods[0].vendor = EAP_VENDOR_IETF;

        if (tail == NULL)
        {
            tail = new_user = user;
        }
        else
        {
            tail->next = user;
            tail       = user;
        }
        continue;

    failed:
        if (user)
            hostapd_config_free_eap_user(user);
        ret = -1;
        break;
    }

    if (ret == 0)
    {
        hostapd_config_free_eap_users(conf->eap_user);
        conf->eap_user = new_user;
    }
    else
    {
        hostapd_config_free_eap_users(new_user);
    }

    return ret;
}

static int hostapd_config_read_eap_phase2_user(struct wlan_network *network, struct hostapd_bss_config *conf)
{
    struct hostapd_eap_user *user = NULL, *tail = NULL, *new_user = NULL;
    size_t i, nusers = network->security.nusers;
    int ret = 0;
    const char *identity, *password;

    if (!nusers)
    {
        wpa_printf(MSG_DEBUG, "EAP users not found.");
        return -1;
    }

    tail = hostapd_config_read_default_eap_user(network, &new_user);

    for (i = 0; i < nusers; i++)
    {
        identity = (const char *)&network->security.identities[i];
        password = (const char *)&network->security.passwords[i];

        if ((network->security.type == WLAN_SECURITY_EAP_PEAP_MSCHAPV2) ||
            (network->security.type == WLAN_SECURITY_EAP_PEAP_GTC) ||
            (network->security.type == WLAN_SECURITY_EAP_FAST_MSCHAPV2) ||
            (network->security.type == WLAN_SECURITY_EAP_FAST_GTC))
        {
            user = os_zalloc(sizeof(*user));
            if (user == NULL)
            {
                wpa_printf(MSG_DEBUG, "EAP user allocation failed");
                goto failed;
            }
            user->force_version = -1;

            user->identity = os_memdup(identity, os_strlen(identity));
            if (user->identity == NULL)
            {
                wpa_printf(MSG_DEBUG,
                           "Failed to allocate "
                           "memory for EAP identity");
                goto failed;
            }

            user->identity_len = os_strlen(identity);

            user->methods[0].method = EAP_TYPE_MSCHAPV2;
            user->methods[0].vendor = EAP_VENDOR_IETF;
            user->methods[1].method = EAP_TYPE_GTC;
            user->methods[1].vendor = EAP_VENDOR_IETF;

            user->password = os_memdup(password, os_strlen(password));
            if (user->password == NULL)
            {
                wpa_printf(MSG_DEBUG,
                           "Failed to allocate "
                           "memory for EAP password");
                goto failed;
            }
            user->password_len = os_strlen(password);

            user->phase2 = 1;

            if (tail == NULL)
            {
                tail = new_user = user;
            }
            else
            {
                tail->next = user;
                tail       = user;
            }
        }
        else if ((network->security.type == WLAN_SECURITY_EAP_PEAP_TLS) || (network->security.type == WLAN_SECURITY_EAP_TTLS))
        {
            user = os_zalloc(sizeof(*user));
            if (user == NULL)
            {
                wpa_printf(MSG_DEBUG, "EAP user allocation failed");
                goto failed;
            }
            user->force_version = -1;

            user->identity = os_memdup(identity, os_strlen(identity));
            if (user->identity == NULL)
            {
                wpa_printf(MSG_DEBUG,
                           "Failed to allocate "
                           "memory for EAP identity");
                goto failed;
            }

            user->identity_len = os_strlen(identity);

            user->methods[0].method = EAP_TYPE_TLS;
            user->methods[0].vendor = EAP_VENDOR_IETF;

            user->password = os_memdup(password, os_strlen(password));
            if (user->password == NULL)
            {
                wpa_printf(MSG_DEBUG,
                           "Failed to allocate "
                           "memory for EAP password");
                goto failed;
            }
            user->password_len = os_strlen(password);

            user->phase2 = 1;

            if (tail == NULL)
            {
                tail = new_user = user;
            }
            else
            {
                tail->next = user;
                tail       = user;
            }
        }
        else if (network->security.type == WLAN_SECURITY_EAP_TTLS_MSCHAPV2)
        {
            user = os_zalloc(sizeof(*user));
            if (user == NULL)
            {
                wpa_printf(MSG_DEBUG, "EAP user allocation failed");
                goto failed;
            }
            user->force_version = -1;

            user->identity = os_memdup(identity, os_strlen(identity));
            if (user->identity == NULL)
            {
                wpa_printf(MSG_DEBUG,
                           "Failed to allocate "
                           "memory for EAP identity");
                goto failed;
            }

            user->identity_len = os_strlen(identity);

            user->methods[0].method = EAP_TYPE_NONE;
            user->methods[0].vendor = EAP_VENDOR_IETF;

            user->ttls_auth |= EAP_TTLS_AUTH_PAP;
            user->ttls_auth |= EAP_TTLS_AUTH_CHAP;
            user->ttls_auth |= EAP_TTLS_AUTH_MSCHAP;
            user->ttls_auth |= EAP_TTLS_AUTH_MSCHAPV2;

            user->password = os_memdup(password, os_strlen(password));
            if (user->password == NULL)
            {
                wpa_printf(MSG_DEBUG,
                           "Failed to allocate "
                           "memory for EAP password");
                goto failed;
            }
            user->password_len = os_strlen(password);

            user->phase2 = 1;

            if (tail == NULL)
            {
                tail = new_user = user;
            }
            else
            {
                tail->next = user;
                tail       = user;
            }
        }
        continue;

    failed:
        if (user)
            hostapd_config_free_eap_user(user);
        ret = -1;
        break;
    }

    if (ret == 0)
    {
        hostapd_config_free_eap_users(conf->eap_user);
        conf->eap_user = new_user;
    }
    else
    {
        hostapd_config_free_eap_users(new_user);
    }

    return ret;
}

#endif

#ifdef CONFIG_WPA_SUPP_WPA3
static int parse_sae_password(struct hostapd_bss_config *bss, const char *sae_password)
{
    struct sae_password_entry *pw;

    pw = os_zalloc(sizeof(*pw));
    if (!pw)
        return -1;
    os_memset(pw->peer_addr, 0xff, ETH_ALEN); /* default to wildcard */

#ifdef CONFIG_SAE_PK
    pos2 = os_strstr(pos, "|pk=");
    if (pos2)
    {
        const char *epos;
        char *tmp;

        if (!end)
            end = pos2;
        pos2 += 4;
        epos = os_strchr(pos2, '|');
        if (epos)
        {
            tmp = os_malloc(epos - pos2 + 1);
            if (!tmp)
                goto fail;
            os_memcpy(tmp, pos2, epos - pos2);
            tmp[epos - pos2] = '\0';
        }
        else
        {
            tmp = os_strdup(pos2);
            if (!tmp)
                goto fail;
        }

        pw->pk = sae_parse_pk(tmp);
        str_clear_free(tmp);
        if (!pw->pk)
            goto fail;
    }
#endif /* CONFIG_SAE_PK */

    pw->password = os_strdup(sae_password);
    if (!pw->password)
        goto fail;

#ifdef CONFIG_SAE_PK
    if (pw->pk &&
#ifdef CONFIG_TESTING_OPTIONS
        !bss->sae_pk_password_check_skip &&
#endif /* CONFIG_TESTING_OPTIONS */
        !sae_pk_valid_password(pw->password))
    {
        wpa_printf(MSG_INFO, "Invalid SAE password for a SAE-PK sae_password entry");
        goto fail;
    }
#endif /* CONFIG_SAE_PK */

    pw->next           = bss->sae_passwords;
    bss->sae_passwords = pw;

    return 0;
fail:
    // str_clear_free(pw->password);
    // os_free(pw->identifier);
#ifdef CONFIG_SAE_PK
    sae_deinit_pk(pw->pk);
#endif /* CONFIG_SAE_PK */
    os_free(pw);
    return -1;
}
#endif

static int hostapd_update_bss(struct hostapd_iface *hapd_s, struct wlan_network *network)
{
    struct hostapd_config *conf;
    struct hostapd_bss_config *bss;
    struct hostapd_ssid *ssid;
    struct hostapd_data *hapd;
#if defined(CONFIG_WPA_SUPP_WPA3) || defined(CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE)
    int ret = -1;
#endif

    hapd = hapd_s->bss[0];
    conf = hapd_s->conf;
    bss  = conf->last_bss;

    hapd_s->freq = 0;

    bss->max_num_sta = 10;

    os_memcpy(bss->bssid, network->bssid, ETH_ALEN);
    os_memcpy(hapd->own_addr, network->bssid, ETH_ALEN);

    conf->acs_exclude_dfs = 1;
    if (network->channel > 14 || (network->channel == 0 && network->acs_band == 1))
    {
        conf->hw_mode = HOSTAPD_MODE_IEEE80211A;
    }
    else
    {
        conf->hw_mode = HOSTAPD_MODE_IEEE80211G;
    }

    conf->channel = network->channel;
    conf->acs     = conf->channel == 0;

    conf->spectrum_mgmt_required = 1;
    conf->local_pwr_constraint = 3;

    conf->obss_interval = 10;

    if (network->dot11n)
    {
        conf->ieee80211n              = 1;
        conf->ht_capab                = network->ht_capab;
        conf->ht40_plus_minus_allowed = 1;
        conf->secondary_channel = 0;

        bss->wmm_enabled = 1;
        bss->wmm_uapsd = 0;

        if (network->sec_channel_offset == 1)
        {
            conf->secondary_channel = 1;
        }
        else if (network->sec_channel_offset == 3)
        {
            conf->secondary_channel = -1;
        }
    }
    else
    {
        conf->ieee80211n = 0;
        conf->ht_capab = 0;
        conf->secondary_channel = 0;
        bss->wmm_enabled = 0;
    }

#ifdef CONFIG_11AC
    if (network->dot11ac)
    {
        conf->ieee80211ac = 1;

        if (conf->acs == 0)
        {
            if (network->channel > 14)
            {
                conf->vht_oper_chwidth             = network->vht_oper_chwidth;
                conf->vht_oper_centr_freq_seg0_idx = network->channel + 6;
            }
            else
            {
                conf->vht_oper_chwidth             = 0;
                conf->vht_oper_centr_freq_seg0_idx = 0;
            }
        }
        conf->vht_capab = network->vht_capab;
    }
    else
    {
        conf->ieee80211ac = 0;
    }
#endif

#ifdef CONFIG_11AX
    if (network->dot11ax)
    {
        conf->ieee80211ax = 1;

        if (conf->acs == 0)
        {
            if (network->channel > 14)
            {
                conf->he_oper_chwidth             = network->he_oper_chwidth;
                conf->he_oper_centr_freq_seg0_idx = network->channel + 6;
            }
            else
            {
                conf->he_oper_chwidth             = 0;
                conf->he_oper_centr_freq_seg0_idx = 0;
            }
        }
        conf->he_phy_capab.he_su_beamformer = 0;
        conf->he_phy_capab.he_su_beamformee = 1;
        conf->he_phy_capab.he_mu_beamformer = 0;
        conf->he_op.he_bss_color            = 1;
        conf->he_op.he_default_pe_duration  = 0;
        conf->he_op.he_basic_mcs_nss_set    = 2;
    }
    else
    {
        conf->ieee80211ax = 0;
    }
#endif

    conf->no_pri_sec_switch = 1;
    conf->ht_op_mode_fixed  = 1;

    ssid           = &bss->ssid;
    ssid->ssid_len = os_strlen(network->ssid);
    if (ssid->ssid_len > SSID_MAX_LEN || ssid->ssid_len < 1)
    {
        wpa_printf(MSG_DEBUG, "invalid SSID '%s'", ssid->ssid);
        return -1;
    }
    os_memcpy(ssid->ssid, network->ssid, ssid->ssid_len);
    ssid->ssid_set   = 1;
    ssid->short_ssid = crc32(ssid->ssid, ssid->ssid_len);

    bss->wpa_key_mgmt = WPA_KEY_MGMT_NONE;

    if (network->security.type != WLAN_SECURITY_NONE)
    {
        bss->wpa = WPA_PROTO_RSN;

        bss->wpa_group         = network->security.group_cipher;
        bss->rsn_pairwise      = network->security.pairwise_cipher;
        bss->wpa_pairwise      = network->security.pairwise_cipher;
        bss->group_mgmt_cipher = network->security.group_mgmt_cipher;
    }

    switch (network->security.type)
    {
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
        case WLAN_SECURITY_EAP_TLS:
        case WLAN_SECURITY_EAP_TLS_SHA256:
#ifdef CONFIG_11R
        case WLAN_SECURITY_EAP_TLS_FT:
        case WLAN_SECURITY_EAP_TLS_FT_SHA384:
#endif
        case WLAN_SECURITY_EAP_TTLS:
        case WLAN_SECURITY_EAP_TTLS_MSCHAPV2:
        case WLAN_SECURITY_EAP_PEAP_MSCHAPV2:
        case WLAN_SECURITY_EAP_PEAP_TLS:
        case WLAN_SECURITY_EAP_PEAP_GTC:
        case WLAN_SECURITY_EAP_FAST_MSCHAPV2:
        case WLAN_SECURITY_EAP_FAST_GTC:

            if (network->security.type == WLAN_SECURITY_EAP_TLS_SHA256)
                bss->wpa_key_mgmt = WPA_KEY_MGMT_IEEE8021X_SHA256;
#ifdef CONFIG_11R
            else if (network->security.type == WLAN_SECURITY_EAP_TLS_FT)
            {
                bss->wpa_key_mgmt          = WPA_KEY_MGMT_IEEE8021X | WPA_KEY_MGMT_FT_IEEE8021X;
                bss->mobility_domain[0]    = 'a';
                bss->mobility_domain[1]    = '1';
                bss->pmk_r1_push           = 1;
                bss->ft_over_ds            = 0;
                bss->ft_psk_generate_local = 1;
            }
            else if (network->security.type == WLAN_SECURITY_EAP_TLS_FT_SHA384)
            {
                bss->wpa_key_mgmt          = WPA_KEY_MGMT_IEEE8021X | WPA_KEY_MGMT_FT_IEEE8021X_SHA384;
                bss->mobility_domain[0]    = 'a';
                bss->mobility_domain[1]    = '2';
                bss->pmk_r1_push           = 1;
                bss->ft_over_ds            = 0;
                bss->ft_psk_generate_local = 1;
            }
#endif
            else
            {
                if (network->security.wpa3_sb_192)
                {
                    bss->wpa_key_mgmt      = WPA_KEY_MGMT_IEEE8021X_SUITE_B_192;
                    bss->group_mgmt_cipher = WPA_CIPHER_BIP_GMAC_256;
                    bss->tls_flags |= TLS_CONN_SUITEB;
                }
                else if (network->security.wpa3_sb)
                {
                    bss->wpa_key_mgmt = WPA_KEY_MGMT_IEEE8021X_SUITE_B;
                    bss->tls_flags |= TLS_CONN_SUITEB;
                }
                else
                {
                    bss->wpa_key_mgmt = WPA_KEY_MGMT_IEEE8021X;
                }
            }

            bss->ieee802_1x    = 1; /* use IEEE 802.1X */
            bss->eapol_version = 2;
            bss->eap_server    = 1;

            if ((network->security.type == WLAN_SECURITY_EAP_FAST_MSCHAPV2) ||
                (network->security.type == WLAN_SECURITY_EAP_FAST_GTC))
            {
                bss->pac_opaque_encr_key = os_zalloc(16);
                if (bss->pac_opaque_encr_key == NULL)
                {
                    wpa_printf(MSG_DEBUG, "%s: EAP FAST encr key alloc failed", __func__);
                    return -1;
                }

                os_get_random(bss->pac_opaque_encr_key, 16);

                bss->eap_fast_a_id = os_zalloc(16);
                if (bss->pac_opaque_encr_key == NULL)
                {
                    wpa_printf(MSG_DEBUG, "%s: EAP FAST a id alloc failed", __func__);
                    return -1;
                }

                os_get_random(bss->eap_fast_a_id, 16);

                bss->eap_fast_a_id_len = 16;

                bss->eap_fast_a_id_info = dup_binstr("hostapd_test_server", os_strlen("hostapd_test_server"));

                bss->eap_fast_prov = 3;
            }

            bss->ca_cert_blob_len     = network->security.ca_cert_len;
            bss->ca_cert_blob         = network->security.ca_cert_data;
            bss->server_cert_blob_len = network->security.server_cert_len;
            bss->server_cert_blob     = network->security.server_cert_data;
            bss->private_key_blob_len = network->security.server_key_len;
            bss->private_key_blob     = network->security.server_key_data;
            bss->dh_blob_len          = network->security.dh_len;
            bss->dh_blob              = network->security.dh_data;

            str_clear_free(bss->private_key_passwd);
            bss->private_key_passwd =
                dup_binstr(network->security.server_key_passwd, os_strlen(network->security.server_key_passwd));

            if ((network->security.type == WLAN_SECURITY_EAP_TLS) ||
                (network->security.type == WLAN_SECURITY_EAP_TLS_SHA256)
#ifdef CONFIG_11R
                ||
                (network->security.type == WLAN_SECURITY_EAP_TLS_FT) ||
                (network->security.type == WLAN_SECURITY_EAP_TLS_FT_SHA384)
#endif
                )
            {
                ret = hostapd_config_read_eap_user(network->security.nusers, network->security.identities, bss);
                if (ret != 0)
                {
                    wpa_printf(MSG_DEBUG, "%s:Failed to read eap users", __func__);
                    return -1;
                }
            }

            if ((network->security.type == WLAN_SECURITY_EAP_TTLS) ||
                (network->security.type == WLAN_SECURITY_EAP_TTLS_MSCHAPV2) ||
                (network->security.type == WLAN_SECURITY_EAP_PEAP_MSCHAPV2) ||
                (network->security.type == WLAN_SECURITY_EAP_PEAP_TLS) ||
                (network->security.type == WLAN_SECURITY_EAP_PEAP_GTC) ||
                (network->security.type == WLAN_SECURITY_EAP_FAST_MSCHAPV2) ||
                (network->security.type == WLAN_SECURITY_EAP_FAST_GTC))
            {
                ret = hostapd_config_read_eap_phase2_user(network, bss);
                if (ret != 0)
                {
                    wpa_printf(MSG_DEBUG, "%s:Failed to read eap phase2 users", __func__);
                    return -1;
                }
            }

            break;
#endif
#ifdef CONFIG_WPA_SUPP_WPA3
        case WLAN_SECURITY_WPA3_SAE:
        case WLAN_SECURITY_WPA2_WPA3_SAE_MIXED:
#ifdef CONFIG_11R
        case WLAN_SECURITY_WPA3_SAE_FT:
            if (network->security.type == WLAN_SECURITY_WPA3_SAE_FT)
            {
                bss->wpa_key_mgmt          = WPA_KEY_MGMT_SAE | WPA_KEY_MGMT_FT_SAE;
                bss->mobility_domain[0]    = 'a';
                bss->mobility_domain[1]    = '3';
                bss->pmk_r1_push           = 1;
                bss->ft_over_ds            = 0;
                bss->ft_psk_generate_local = 1;
            }
            else
#endif
                if (network->security.type == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED)
            {
                bss->wpa_key_mgmt        = WPA_KEY_MGMT_PSK | WPA_KEY_MGMT_SAE;
                bss->ssid.wpa_passphrase = os_strdup(network->security.psk);
                if (bss->ssid.wpa_passphrase)
                {
                    hostapd_config_clear_wpa_psk(&bss->ssid.wpa_psk);
                    bss->ssid.wpa_passphrase_set = 1;
                }
                else
                {
                    wpa_printf(MSG_DEBUG, "%s:Failed to copy passphrase", __func__);
                    return -1;
                }
            }
            else
            {
                bss->wpa_key_mgmt = WPA_KEY_MGMT_SAE;
            }

            bss->sae_pwe            = network->security.pwe_derivation;
            bss->transition_disable = network->security.transition_disable;

            ret = parse_sae_password(bss, network->security.password);
            if (ret != 0)
            {
                wpa_printf(MSG_DEBUG, "%s:Failed to set sae_password", __func__);
                return -1;
            }

            break;
#endif
#ifdef CONFIG_OWE
        case WLAN_SECURITY_OWE_ONLY:
            bss->wpa_key_mgmt = WPA_KEY_MGMT_OWE;
            break;
#endif
        case WLAN_SECURITY_WPA2:
#ifdef CONFIG_11R
        case WLAN_SECURITY_WPA2_FT:
#endif
        case WLAN_SECURITY_WPA2_SHA256:
        case WLAN_SECURITY_WPA_WPA2_MIXED:

#ifdef CONFIG_11R
            if (network->security.type == WLAN_SECURITY_WPA2_FT)
            {
                bss->wpa_key_mgmt          = WPA_KEY_MGMT_PSK | WPA_KEY_MGMT_FT_PSK;
                bss->mobility_domain[0]    = 'a';
                bss->mobility_domain[1]    = '4';
                bss->pmk_r1_push           = 1;
                bss->ft_over_ds            = 0;
                bss->ft_psk_generate_local = 1;
            }
            else
#endif
                if (network->security.type == WLAN_SECURITY_WPA2_SHA256)
                bss->wpa_key_mgmt = WPA_KEY_MGMT_PSK_SHA256;
            else
            {
                bss->wpa_key_mgmt = WPA_KEY_MGMT_PSK;
                if (network->security.type == WLAN_SECURITY_WPA_WPA2_MIXED)
                {
                    bss->wpa = WPA_PROTO_WPA | WPA_PROTO_RSN;
                }
            }

            // str_clear_free(bss->ssid.wpa_passphrase);
            bss->ssid.wpa_passphrase = os_strdup(network->security.psk);
            if (bss->ssid.wpa_passphrase)
            {
                hostapd_config_clear_wpa_psk(&bss->ssid.wpa_psk);
                bss->ssid.wpa_passphrase_set = 1;
            }
            else
            {
                wpa_printf(MSG_DEBUG, "%s:Failed to copy passphrase", __func__);
                return -1;
            }
            break;
        default:
            break;
    }

    if ((network->security.mfpc) && (network->security.mfpr))
        bss->ieee80211w = MGMT_FRAME_PROTECTION_REQUIRED;
    else if (network->security.mfpc)
        bss->ieee80211w = MGMT_FRAME_PROTECTION_OPTIONAL;

    return 0;
}

static void hostapd_reset_bss(struct hostapd_bss_config *bss)
{
    bss->wpa_key_mgmt = 0;

    bss->wpa = 0;

    bss->wpa_group         = 0;
    bss->rsn_pairwise      = 0;
    bss->wpa_pairwise      = 0;
    bss->group_mgmt_cipher = 0;

#ifdef CONFIG_11R
    bss->pmk_r1_push           = 0;
    bss->ft_over_ds            = 0;
    bss->ft_psk_generate_local = 0;
#endif

    bss->tls_flags     = 0;
    bss->ieee802_1x    = 0;
    bss->eapol_version = 0;
    bss->eap_server    = 0;

    bss->sae_pwe = 0;
    bss->transition_disable = 0;

    bss->ssid.wpa_passphrase_set = 0;

    bss->ieee80211w = NO_MGMT_FRAME_PROTECTION;
}

#ifdef CONFIG_WPA_SUPP_WPA3
static void hostapd_config_free_sae_passwords(struct hostapd_bss_config *conf)
{
    struct sae_password_entry *pw, *tmp;

    pw                  = conf->sae_passwords;
    conf->sae_passwords = NULL;
    while (pw)
    {
        tmp = pw;
        pw  = pw->next;
        str_clear_free(tmp->password);
        os_free(tmp->identifier);
#ifdef CONFIG_SAE
        sae_deinit_pt(tmp->pt);
#endif /* CONFIG_SAE */
#ifdef CONFIG_SAE_PK
        sae_deinit_pk(tmp->pk);
#endif /* CONFIG_SAE_PK */
        os_free(tmp);
    }
}
#endif

static int hostapd_free_bss(struct hostapd_iface *hapd_s, struct wlan_network *network)
{
    struct hostapd_config *conf;
    struct hostapd_bss_config *bss;

    conf = hapd_s->conf;
    bss  = conf->last_bss;

    switch (network->security.type)
    {
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
        case WLAN_SECURITY_EAP_TLS:
        case WLAN_SECURITY_EAP_TLS_SHA256:
#ifdef CONFIG_11R
        case WLAN_SECURITY_EAP_TLS_FT:
        case WLAN_SECURITY_EAP_TLS_FT_SHA384:
#endif
        case WLAN_SECURITY_EAP_TTLS:
        case WLAN_SECURITY_EAP_TTLS_MSCHAPV2:
        case WLAN_SECURITY_EAP_PEAP_MSCHAPV2:
        case WLAN_SECURITY_EAP_PEAP_TLS:
        case WLAN_SECURITY_EAP_PEAP_GTC:
        case WLAN_SECURITY_EAP_FAST_MSCHAPV2:
        case WLAN_SECURITY_EAP_FAST_GTC:

            if (bss->pac_opaque_encr_key)
            {
                os_free(bss->pac_opaque_encr_key);
            }
            if (bss->eap_fast_a_id)
            {
                os_free(bss->eap_fast_a_id);
            }
            str_clear_free(bss->eap_fast_a_id_info);
            bss->eap_fast_a_id_info = NULL;

            hostapd_config_free_eap_users(bss->eap_user);
            bss->eap_user = NULL;
            break;
#endif
#ifdef CONFIG_WPA_SUPP_WPA3
        case WLAN_SECURITY_WPA2_WPA3_SAE_MIXED:
            str_clear_free(bss->ssid.wpa_passphrase);
            bss->ssid.wpa_passphrase = NULL;
            hostapd_config_clear_wpa_psk(&bss->ssid.wpa_psk);
        case WLAN_SECURITY_WPA3_SAE:
#ifdef CONFIG_11R
        case WLAN_SECURITY_WPA3_SAE_FT:
#endif
            hostapd_config_free_sae_passwords(bss);
            break;
#endif
#ifdef CONFIG_OWE
        case WLAN_SECURITY_OWE_ONLY:
            break;
#endif
        case WLAN_SECURITY_WPA2:
#ifdef CONFIG_11R
        case WLAN_SECURITY_WPA2_FT:
#endif
        case WLAN_SECURITY_WPA2_SHA256:
        case WLAN_SECURITY_WPA_WPA2_MIXED:

            str_clear_free(bss->ssid.wpa_passphrase);
            bss->ssid.wpa_passphrase = NULL;
            hostapd_config_clear_wpa_psk(&bss->ssid.wpa_psk);
            break;
        default:
            break;
    }

    hostapd_reset_bss(bss);

    return 0;
}

#endif

#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE

static int wpa_config_parse_eap(struct wpa_ssid *ssid, const char *value)
{
    int last, errors = 0;
    char *start, *end, *buf;
    struct eap_method_type *methods = NULL, *tmp;
    size_t num_methods              = 0;

    buf = os_strdup(value);
    if (buf == NULL)
        return -1;
    start = buf;

    while (*start != '\0')
    {
        while (*start == ' ' || *start == '\t')
            start++;
        if (*start == '\0')
            break;
        end = start;
        while (*end != ' ' && *end != '\t' && *end != '\0')
            end++;
        last    = *end == '\0';
        *end    = '\0';
        tmp     = methods;
        methods = os_realloc_array(methods, num_methods + 1, sizeof(*methods));
        if (methods == NULL)
        {
            os_free(tmp);
            os_free(buf);
            return -1;
        }
        methods[num_methods].method = eap_peer_get_type(start, &methods[num_methods].vendor);
        if (methods[num_methods].vendor == EAP_VENDOR_IETF && methods[num_methods].method == EAP_TYPE_NONE)
        {
            wpa_printf(MSG_DEBUG,
                       "unknown EAP method "
                       "'%s'",
                       start);
            wpa_printf(MSG_DEBUG,
                       "You may need to add support for"
                       " this EAP method during wpa_supplicant\n"
                       "build time configuration.\n"
                       "See README for more information.");
            errors++;
        }
        else if (methods[num_methods].vendor == EAP_VENDOR_IETF && methods[num_methods].method == EAP_TYPE_LEAP)
            ssid->leap++;
        else
            ssid->non_leap++;
        num_methods++;
        if (last)
            break;
        start = end + 1;
    }
    os_free(buf);

    tmp     = methods;
    methods = os_realloc_array(methods, num_methods + 1, sizeof(*methods));
    if (methods == NULL)
    {
        os_free(tmp);
        return -1;
    }
    methods[num_methods].vendor = EAP_VENDOR_IETF;
    methods[num_methods].method = EAP_TYPE_NONE;
    num_methods++;

    if (!errors && ssid->eap.eap_methods)
    {
        struct eap_method_type *prev_m;
        size_t i, j, prev_methods, match = 0;

        prev_m = ssid->eap.eap_methods;
        for (i = 0; prev_m[i].vendor != EAP_VENDOR_IETF || prev_m[i].method != EAP_TYPE_NONE; i++)
        {
            /* Count the methods */
        }
        prev_methods = i + 1;

        for (i = 0; prev_methods == num_methods && i < prev_methods; i++)
        {
            for (j = 0; j < num_methods; j++)
            {
                if (prev_m[i].vendor == methods[j].vendor && prev_m[i].method == methods[j].method)
                {
                    match++;
                    break;
                }
            }
        }
        if (match == num_methods)
        {
            os_free(methods);
            return 1;
        }
    }
    wpa_hexdump(MSG_MSGDUMP, "eap methods", (u8 *)methods, num_methods * sizeof(*methods));
    os_free(ssid->eap.eap_methods);
    ssid->eap.eap_methods = methods;
    return errors ? -1 : 0;
}
#endif

static void str2hex(const char *str, char *strH)
{
    unsigned int i, j;

    for (i = 0, j = 0; i < strlen(str); i++, j += 2)
    {
        sprintf((char*)strH + j, "%02X", str[i]);
    }

    strH[j] = '\0';
}

int wpa_supp_add_network(const struct netif *dev, struct wlan_network *network)
{
    int ret               = 0;
    struct wpa_ssid *ssid = NULL;
    bool pmf              = true;
    struct wpa_supplicant *wpa_s;
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
    char phase1[256];
    char *openssl_ciphers;
#endif
#ifdef CONFIG_HOSTAPD
    struct hostapd_iface *hapd_s;
#endif
    char HashH[65] = {0};
    char hashstr[128];

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

#ifdef CONFIG_HOSTAPD
    if (network->role == WLAN_BSS_ROLE_UAP)
    {
        hapd_s = get_hostapd_handle(dev);
        if (!hapd_s)
        {
            ret = -1;
            goto out;
        }
    }
    else
    {
#endif
        wpa_s = get_wpa_s_handle(dev);
        if (!wpa_s)
        {
            ret = -1;
            goto out;
        }

        ssid       = wpa_supplicant_add_network(wpa_s);
        ssid->ssid = os_zalloc(sizeof(u8) * MAX_SSID_LEN);

        ssid->ssid_len = os_strlen(network->ssid);
        memcpy(ssid->ssid, network->ssid, ssid->ssid_len);
        ssid->disabled = 1;
        ssid->key_mgmt = WPA_KEY_MGMT_NONE;
        ssid->scan_ssid = 1;
        ssid->proactive_key_caching = network->security.pkc;

        if (network->security.type == WLAN_SECURITY_WILDCARD)
        {
            if (network->security.psk_len)
            {
                network->security.type = WLAN_SECURITY_WPA2;
            }
            else
            {
                network->security.type = WLAN_SECURITY_NONE;
            }
        }

        if (network->security.type != WLAN_SECURITY_NONE)
        {
            ssid->proto             = WPA_PROTO_RSN;
            ssid->group_cipher      = network->security.group_cipher;
            ssid->pairwise_cipher   = network->security.pairwise_cipher;
            ssid->group_mgmt_cipher = network->security.group_mgmt_cipher;
        }

        if (network->channel != 0U)
        {
            /* We use global channel list here and also use the widest
             * op_class for 5GHz channels as there is no user input
             * for these.
             */
            int freq = ieee80211_chan_to_freq(NULL, 81, network->channel);

            if (freq <= 0)
            {
                freq = ieee80211_chan_to_freq(NULL, 128, network->channel);
            }

            if (freq <= 0)
            {
                wpa_printf(MSG_DEBUG, "Invalid channel %d", network->channel);
                ret = -1;
                goto out;
            }

            if (network->role == WLAN_BSS_ROLE_UAP)
            {
                ssid->frequency = freq;
            }
            else
            {
                ssid->scan_freq = os_zalloc(2 * sizeof(int));
                if (!ssid->scan_freq)
                {
                    ret = -1;
                    goto out;
                }
                ssid->scan_freq[0] = freq;
                ssid->scan_freq[1] = 0;

                ssid->freq_list = os_zalloc(2 * sizeof(int));
                if (!ssid->freq_list)
                {
                    os_free(ssid->scan_freq);
                    ret = -1;
                    goto out;
                }
                ssid->freq_list[0] = freq;
                ssid->freq_list[1] = 0;
            }
        }

        wpa_s->conf->filter_ssids = 1;
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
        openssl_ciphers           = "DEFAULT:!EXP:!LOW";
#endif

        if (network->role == WLAN_BSS_ROLE_UAP)
        {
            wpa_s->conf->ap_scan = 2;
            ssid->mode           = WPAS_MODE_AP;
        }
        else
        {
            wpa_s->conf->ap_scan = 1;
            ssid->mode           = WPAS_MODE_INFRA;
        }

        switch (network->security.type)
        {
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
            case WLAN_SECURITY_EAP_TLS:
            case WLAN_SECURITY_EAP_TLS_SHA256:
#ifdef CONFIG_11R
            case WLAN_SECURITY_EAP_TLS_FT:
            case WLAN_SECURITY_EAP_TLS_FT_SHA384:
#endif
            case WLAN_SECURITY_EAP_TTLS:
            case WLAN_SECURITY_EAP_TTLS_MSCHAPV2:
            case WLAN_SECURITY_EAP_PEAP_MSCHAPV2:
            case WLAN_SECURITY_EAP_PEAP_TLS:
            case WLAN_SECURITY_EAP_PEAP_GTC:
            case WLAN_SECURITY_EAP_SIM:
            case WLAN_SECURITY_EAP_AKA:
            case WLAN_SECURITY_EAP_AKA_PRIME:
            case WLAN_SECURITY_EAP_FAST_MSCHAPV2:
            case WLAN_SECURITY_EAP_FAST_GTC:
            case WLAN_SECURITY_EAP_WILDCARD:

                if (network->role == WLAN_BSS_ROLE_UAP)
                {
                    ret = -1;
                    goto out;
                }
                if (network->security.type == WLAN_SECURITY_EAP_TLS_SHA256)
                    ssid->key_mgmt = WPA_KEY_MGMT_IEEE8021X_SHA256;
#ifdef CONFIG_11R
                else if (network->security.type == WLAN_SECURITY_EAP_TLS_FT)
                    ssid->key_mgmt = WPA_KEY_MGMT_FT_IEEE8021X;
                else if (network->security.type == WLAN_SECURITY_EAP_TLS_FT_SHA384)
                    ssid->key_mgmt = WPA_KEY_MGMT_FT_IEEE8021X_SHA384;
#endif
                else
                {
                    if (network->security.wpa3_sb_192)
                    {
                        ssid->key_mgmt  = WPA_KEY_MGMT_IEEE8021X_SUITE_B_192;
                        openssl_ciphers = "SUITEB192";
                    }
                    else if (network->security.wpa3_sb)
                    {
                        ssid->key_mgmt  = WPA_KEY_MGMT_IEEE8021X_SUITE_B;
                        openssl_ciphers = "SUITEB128";
                    }
                    else
                    {
                        ssid->key_mgmt = WPA_KEY_MGMT_IEEE8021X;
                    }
                }

                str_clear_free((char *)wpa_s->conf->openssl_ciphers);
                wpa_s->conf->openssl_ciphers = dup_binstr(openssl_ciphers, os_strlen(openssl_ciphers));

                if (network->security.type == WLAN_SECURITY_EAP_WILDCARD)
                {
                    const char *methods = "TLS PEAP TTLS FAST SIM AKA AKA'";
                    const char *phase2  = "auth=MSCHAPV2 auth=TLS autheap=TLS auth=GTC";
                    int prov            = network->security.pac_len == 0 ? 1 : 3;

                    os_snprintf(phase1, sizeof(phase1),
                                "peapver=%d peaplabel=%d fast_provisioning=%d fast_pac_format=binary",
                                network->security.eap_ver, network->security.peap_label, prov);

                    ret = wpa_config_parse_eap(ssid, methods);
                    if (ret < 0)
                    {
                        ret = -1;
                        goto out;
                    }

                    str_clear_free(ssid->eap.phase1);
                    ssid->eap.phase1 = dup_binstr(phase1, os_strlen(phase1));

                    str_clear_free(ssid->eap.phase2);
                    ssid->eap.phase2 = dup_binstr(phase2, os_strlen(phase2));
                }
                else
                {
                    ssid->eap.eap_methods = os_zalloc(sizeof(struct eap_method_type));
                    if (!ssid->eap.eap_methods)
                    {
                        ret = -1;
                        goto out;
                    }

                    ssid->eap.eap_methods->vendor = EAP_VENDOR_IETF;
                    ssid->eap.eap_methods->method = EAP_TYPE_TLS;
                }

                ssid->eap.anonymous_identity_len = os_strlen(network->security.anonymous_identity);
                if (ssid->eap.anonymous_identity_len)
                {
                    str_clear_free((char *)ssid->eap.anonymous_identity);
                    ssid->eap.anonymous_identity =
                        (u8 *)dup_binstr(network->security.anonymous_identity, ssid->eap.anonymous_identity_len);
                }

                ssid->eap.identity_len = os_strlen(network->security.identity);
                if (ssid->eap.identity_len)
                {
                    str_clear_free((char *)ssid->eap.identity);
                    ssid->eap.identity = (u8 *)dup_binstr(network->security.identity, ssid->eap.identity_len);
                }

                ssid->eap.password_len = os_strlen(network->security.eap_password);
                if (ssid->eap.password_len)
                {
                    str_clear_free((char *)ssid->eap.password);
                    ssid->eap.password = (u8 *)dup_binstr(network->security.eap_password, ssid->eap.password_len);
                }

                if (os_strlen(network->security.domain_match))
                {
                    str_clear_free(ssid->eap.cert.domain_match);
                    ssid->eap.cert.domain_match = dup_binstr(network->security.domain_match, os_strlen(network->security.domain_match));
                }

                if (os_strlen(network->security.ca_cert_hash))
                {
                    str2hex(network->security.ca_cert_hash, HashH);
                    os_snprintf(hashstr, os_strlen(hashstr), "hash://server/sha256/%s", HashH);
                    str_clear_free(ssid->eap.cert.ca_cert);
                    ssid->eap.cert.ca_cert = dup_binstr(hashstr, os_strlen(hashstr));
                }
                else if (network->security.ca_cert_len)
                {
                    wpa_config_process_blob(wpa_s->conf, "ca_cert", network->security.ca_cert_data,
                                            network->security.ca_cert_len);

                    str_clear_free(ssid->eap.cert.ca_cert);
                    ssid->eap.cert.ca_cert = dup_binstr("blob://ca_cert", os_strlen("blob://ca_cert"));
                }

                if (network->security.client_cert_len)
                {
                    wpa_config_process_blob(wpa_s->conf, "client_cert", network->security.client_cert_data,
                                            network->security.client_cert_len);

                    str_clear_free(ssid->eap.cert.client_cert);
                    ssid->eap.cert.client_cert = dup_binstr("blob://client_cert", os_strlen("blob://client_cert"));
                }

                if (network->security.client_key_len)
                {
                    wpa_config_process_blob(wpa_s->conf, "private_key", network->security.client_key_data,
                                            network->security.client_key_len);

                    str_clear_free(ssid->eap.cert.private_key);
                    ssid->eap.cert.private_key = dup_binstr("blob://private_key", os_strlen("blob://private_key"));

                    str_clear_free(ssid->eap.cert.private_key_passwd);
                    ssid->eap.cert.private_key_passwd =
                        dup_binstr(network->security.client_key_passwd, os_strlen(network->security.client_key_passwd));
                }

                if (network->security.ca_cert2_len)
                {
                    wpa_config_process_blob(wpa_s->conf, "ca_cert2", network->security.ca_cert2_data,
                                            network->security.ca_cert2_len);

                    str_clear_free(ssid->eap.phase2_cert.ca_cert);
                    ssid->eap.phase2_cert.ca_cert = dup_binstr("blob://ca_cert2", os_strlen("blob://ca_cert2"));
                }
                else
                {
                    str_clear_free(ssid->eap.phase2_cert.ca_cert);
                    ssid->eap.phase2_cert.ca_cert = dup_binstr("blob://ca_cert", os_strlen("blob://ca_cert"));
                }

                if (network->security.client_cert2_len)
                {
                    wpa_config_process_blob(wpa_s->conf, "client_cert2", network->security.client_cert2_data,
                                            network->security.client_cert2_len);

                    str_clear_free(ssid->eap.phase2_cert.client_cert);
                    ssid->eap.phase2_cert.client_cert =
                        dup_binstr("blob://client_cert2", os_strlen("blob://client_cert2"));
                }
                else
                {
                    str_clear_free(ssid->eap.phase2_cert.client_cert);
                    ssid->eap.phase2_cert.client_cert =
                        dup_binstr("blob://client_cert", os_strlen("blob://client_cert"));
                }

                if (network->security.client_key2_len)
                {
                    wpa_config_process_blob(wpa_s->conf, "private_key2", network->security.client_key2_data,
                                            network->security.client_key2_len);

                    str_clear_free(ssid->eap.phase2_cert.private_key);
                    ssid->eap.phase2_cert.private_key =
                        dup_binstr("blob://private_key2", os_strlen("blob://private_key2"));

                    str_clear_free(ssid->eap.phase2_cert.private_key_passwd);
                    ssid->eap.phase2_cert.private_key_passwd = dup_binstr(
                        network->security.client_key2_passwd, os_strlen(network->security.client_key2_passwd));
                }
                else
                {
                    ssid->eap.phase2_cert.private_key =
                        dup_binstr("blob://private_key", os_strlen("blob://private_key"));
                    str_clear_free(ssid->eap.phase2_cert.private_key_passwd);
                    ssid->eap.phase2_cert.private_key_passwd =
                        dup_binstr(network->security.client_key_passwd, os_strlen(network->security.client_key_passwd));
                }

                if (network->security.pac_len)
                {
                    wpa_config_process_blob(wpa_s->conf, "eap-fast-pac", network->security.pac_data,
                                            network->security.pac_len);
                }

                str_clear_free(ssid->eap.pac_file);
                ssid->eap.pac_file = dup_binstr("blob://eap-fast-pac", os_strlen("blob://eap-fast-pac"));

                if (network->security.type == WLAN_SECURITY_EAP_TTLS)
                {
                    ssid->eap.eap_methods->method = EAP_TYPE_TTLS;

                    const char *phase2 = "autheap=TLS";

                    str_clear_free(ssid->eap.phase2);
                    ssid->eap.phase2 = dup_binstr(phase2, os_strlen(phase2));
                }
                else if (network->security.type == WLAN_SECURITY_EAP_TTLS_MSCHAPV2)
                {
                    ssid->eap.eap_methods->method = EAP_TYPE_TTLS;

                    const char *phase2 = "auth=MSCHAPV2";

                    str_clear_free(ssid->eap.phase2);
                    ssid->eap.phase2 = dup_binstr(phase2, os_strlen(phase2));
                }
                else if (network->security.type == WLAN_SECURITY_EAP_PEAP_MSCHAPV2)
                {
                    ssid->eap.eap_methods->method = EAP_TYPE_PEAP;

                    const char *phase2 = "auth=MSCHAPV2";

                    os_snprintf(phase1, sizeof(phase1), "peapver=%d peaplabel=%d", network->security.eap_ver,
                                network->security.peap_label);

                    str_clear_free(ssid->eap.phase1);
                    ssid->eap.phase1 = dup_binstr(phase1, os_strlen(phase1));

                    str_clear_free(ssid->eap.phase2);
                    ssid->eap.phase2 = dup_binstr(phase2, os_strlen(phase2));
                }
                else if (network->security.type == WLAN_SECURITY_EAP_PEAP_TLS)
                {
                    ssid->eap.eap_methods->method = EAP_TYPE_PEAP;

                    const char *phase2 = "auth=TLS";

                    os_snprintf(phase1, sizeof(phase1), "peapver=%d peaplabel=%d", network->security.eap_ver,
                                network->security.peap_label);

                    str_clear_free(ssid->eap.phase1);
                    ssid->eap.phase1 = dup_binstr(phase1, os_strlen(phase1));

                    str_clear_free(ssid->eap.phase2);
                    ssid->eap.phase2 = dup_binstr(phase2, os_strlen(phase2));
                }
                else if (network->security.type == WLAN_SECURITY_EAP_PEAP_GTC)
                {
                    ssid->eap.eap_methods->method = EAP_TYPE_PEAP;

                    const char *phase2 = "auth=GTC";

                    os_snprintf(phase1, sizeof(phase1), "peapver=%d peaplabel=%d", network->security.eap_ver,
                                network->security.peap_label);

                    str_clear_free(ssid->eap.phase1);
                    ssid->eap.phase1 = dup_binstr(phase1, os_strlen(phase1));

                    str_clear_free(ssid->eap.phase2);
                    ssid->eap.phase2 = dup_binstr(phase2, os_strlen(phase2));
                }
                else if (network->security.type == WLAN_SECURITY_EAP_SIM)
                {
                    ssid->eap.eap_methods->method = EAP_TYPE_SIM;
                }
                else if (network->security.type == WLAN_SECURITY_EAP_AKA)
                {
                    ssid->eap.eap_methods->method = EAP_TYPE_AKA;
                }
                else if (network->security.type == WLAN_SECURITY_EAP_AKA_PRIME)
                {
                    ssid->eap.eap_methods->method = EAP_TYPE_AKA_PRIME;
                }
                else if (network->security.type == WLAN_SECURITY_EAP_FAST_MSCHAPV2)
                {
                    ssid->eap.eap_methods->method = EAP_TYPE_FAST;

                    int prov           = network->security.pac_len == 0 ? 1 : 3;
                    const char *phase2 = "auth=MSCHAPV2";

                    os_snprintf(phase1, sizeof(phase1), "fast_provisioning=%d fast_pac_format=binary", prov);

                    str_clear_free(ssid->eap.phase1);
                    ssid->eap.phase1 = dup_binstr(phase1, os_strlen(phase1));

                    str_clear_free(ssid->eap.phase2);
                    ssid->eap.phase2 = dup_binstr(phase2, os_strlen(phase2));
                }
                else if (network->security.type == WLAN_SECURITY_EAP_FAST_GTC)
                {
                    ssid->eap.eap_methods->method = EAP_TYPE_FAST;

                    int prov           = network->security.pac_len == 0 ? 1 : 3;
                    const char *phase2 = "auth=GTC";

                    os_snprintf(phase1, sizeof(phase1), "fast_provisioning=%d fast_pac_format=binary", prov);

                    str_clear_free(ssid->eap.phase1);
                    ssid->eap.phase1 = dup_binstr(phase1, os_strlen(phase1));

                    str_clear_free(ssid->eap.phase2);
                    ssid->eap.phase2 = dup_binstr(phase2, os_strlen(phase2));
                }

                break;
#endif
#ifdef CONFIG_WPA_SUPP_WPA3
            case WLAN_SECURITY_WPA3_SAE:
            case WLAN_SECURITY_WPA2_WPA3_SAE_MIXED:
#ifdef CONFIG_11R
            case WLAN_SECURITY_WPA3_SAE_FT:
                if (network->security.type == WLAN_SECURITY_WPA3_SAE_FT)
                {
                    if (network->role == WLAN_BSS_ROLE_UAP)
                    {
                        ret = -1;
                        goto out;
                    }
                    ssid->key_mgmt = WPA_KEY_MGMT_FT_SAE;
                }
                else
#endif
                    if (network->security.type == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED)
                    {
                        ssid->key_mgmt        = WPA_KEY_MGMT_PSK | WPA_KEY_MGMT_SAE;
                        str_clear_free(ssid->passphrase);
                        ssid->passphrase = dup_binstr(network->security.psk, network->security.psk_len);

                        if (ssid->passphrase == NULL)
                        {
                            wpa_printf(MSG_DEBUG, "%s:Failed to copy passphrase\n", __func__);
                            ret = -1;
                            goto out;
                        }
                        wpa_config_update_psk(ssid);

                    }
                    else
                    {
                        ssid->key_mgmt = WPA_KEY_MGMT_SAE;
                    }

                wpa_s->conf->sae_pwe     = network->security.pwe_derivation;
                ssid->transition_disable = network->security.transition_disable;
                str_clear_free(ssid->sae_password);
                ssid->sae_password = dup_binstr(network->security.password, network->security.password_len);

                if (ssid->sae_password == NULL)
                {
                    wpa_printf(MSG_DEBUG, "%s:Failed to copy sae_password\n", __func__);
                    ret = -1;
                    goto out;
                }
                break;
#endif
#ifdef CONFIG_OWE
            case WLAN_SECURITY_OWE_ONLY:
                ssid->key_mgmt = WPA_KEY_MGMT_OWE;
                ssid->owe_only = 1;
                break;
#endif
            case WLAN_SECURITY_WPA:
            case WLAN_SECURITY_WPA2:
            case WLAN_SECURITY_WPA2_SHA256:
            case WLAN_SECURITY_WPA_WPA2_MIXED:
#ifdef CONFIG_11R
            case WLAN_SECURITY_WPA2_FT:
                if (network->security.type == WLAN_SECURITY_WPA2_FT)
                {
                    if (network->role == WLAN_BSS_ROLE_UAP)
                    {
                        ret = -1;
                        goto out;
                    }
                    ssid->key_mgmt = WPA_KEY_MGMT_FT_PSK;
                }
                else
#endif
                    if (network->security.type == WLAN_SECURITY_WPA2_SHA256)
                {
                    ssid->key_mgmt = WPA_KEY_MGMT_PSK_SHA256;
                }
                else
                {
                    ssid->key_mgmt = WPA_KEY_MGMT_PSK;
                    if ((network->security.mfpc) || (network->security.mfpr))
                    {
                        ssid->key_mgmt |= WPA_KEY_MGMT_PSK_SHA256;
                    }
                }

                ssid->group_cipher    = WPA_CIPHER_CCMP | WPA_CIPHER_TKIP;
                ssid->proto           = WPA_PROTO_RSN;
                ssid->pairwise_cipher = WPA_CIPHER_CCMP;

                if ((network->security.type == WLAN_SECURITY_WPA) || (network->security.type == WLAN_SECURITY_WPA_WPA2_MIXED))
                {
                    ssid->proto           |= WPA_PROTO_WPA;
                    ssid->group_cipher    |= WPA_CIPHER_TKIP;
                    ssid->pairwise_cipher |= WPA_CIPHER_TKIP;
                }

                if (network->security.pmk_valid)
                {
                    os_memcpy(ssid->psk, network->security.pmk, PMK_LEN);
                    ssid->psk_set = 1;
                }
                else
                {
                    str_clear_free(ssid->passphrase);
                    ssid->passphrase = dup_binstr(network->security.psk, network->security.psk_len);

                    if (ssid->passphrase == NULL)
                    {
                        wpa_printf(MSG_DEBUG, "%s:Failed to copy passphrase\n", __func__);
                        ret = -1;
                        goto out;
                    }
                    wpa_config_update_psk(ssid);
                }
                break;
            default:
                break;
        }

        if (pmf)
        {
            if ((network->security.mfpc) && (network->security.mfpr))
                ssid->ieee80211w = MGMT_FRAME_PROTECTION_REQUIRED;
            else if (network->security.mfpc)
                ssid->ieee80211w = MGMT_FRAME_PROTECTION_OPTIONAL;
        }

        wpa_sm_pmksa_cache_flush(wpa_s->wpa, ssid);

        wpa_config_update_prio_list(wpa_s->conf);

        network->id = ssid->id;

        wpa_supp_api_ctrl.dev          = dev;
        wpa_supp_api_ctrl.requested_op = ADD;

#ifdef CONFIG_HOSTAPD
    }
#endif

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

static void wpa_supp_scan_res_fail_handler(struct wpa_supplicant *wpa_s)
{
    if (wpa_s->scan_work)
    {
        struct wpa_radio_work *work = wpa_s->scan_work;
        wpa_s->scan_work            = NULL;
        radio_work_done(work);
    }

    wpa_msg(wpa_s, MSG_INFO, WPA_EVENT_SCAN_FAILED "ret=-1");
}

int wpa_supp_connect(const struct netif *dev, struct wlan_network *network)
{
    struct wpa_ssid *ssid = NULL;
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    ssid = wpa_s->conf->ssid;

    while (ssid)
    {
        if (network->id == ssid->id)
        {
            break;
        }
        ssid = ssid->next;
    }

    if (ssid == NULL)
    {
        ret = -1;
        goto out;
    }

    if (ssid && ssid == wpa_s->current_ssid && wpa_s->current_ssid && wpa_s->wpa_state >= WPA_AUTHENTICATING)
    {
        /* We are already associated with the selected network */
        wpa_printf(MSG_DEBUG,
                   "Already associated with the "
                   "selected network - do Deauthentication");
        wpa_supplicant_deauthenticate(wpa_s, WLAN_REASON_DEAUTH_LEAVING);
        goto out;
    }

    wpa_s->scan_min_time.sec  = 0;
    wpa_s->scan_min_time.usec = 0;

    //wpa_supplicant_enable_network(wpa_s, ssid);

    wpa_supplicant_select_network(wpa_s, ssid);

    send_wpa_supplicant_dummy_event();

    wpa_supp_api_ctrl.dev          = dev;
    wpa_supp_api_ctrl.requested_op = CONNECT;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_disable(const struct netif *dev, struct wlan_network *network)
{
    struct wpa_ssid *ssid = NULL;
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    ssid = wpa_s->conf->ssid;

    while (ssid)
    {
        if (network->id == ssid->id)
        {
            break;
        }
        ssid = ssid->next;
    }

    if (ssid == NULL)
    {
        ret = -1;
        goto out;
    }

    wpa_supplicant_disable_network(wpa_s, ssid);

    send_wpa_supplicant_dummy_event();

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_disconnect(const struct netif *dev)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }
#if 0
    sme_clear_on_disassoc(wpa_s);

    wpabuf_free(wpa_s->pending_eapol_rx);
    wpa_s->pending_eapol_rx = NULL;

#ifdef CONFIG_SAE
    os_free(wpa_s->sme.sae_rejected_groups);
    wpa_s->sme.sae_rejected_groups = NULL;
#endif /* CONFIG_SAE */

    os_free(wpa_s->next_scan_freqs);
    wpa_s->next_scan_freqs = NULL;

    os_free(wpa_s->manual_scan_freqs);
    wpa_s->manual_scan_freqs = NULL;

    os_free(wpa_s->select_network_scan_freqs);
    wpa_s->select_network_scan_freqs = NULL;

    os_free(wpa_s->manual_sched_scan_freqs);
    wpa_s->manual_sched_scan_freqs = NULL;

    os_free(wpa_s->last_scan_res);
    wpa_s->last_scan_res = NULL;
#endif

    wpa_s->scan_res_fail_handler = NULL;

    eapol_sm_invalidate_cached_session(wpa_s->eapol);
    wpas_request_disconnection(wpa_s);

    send_wpa_supplicant_dummy_event();

    wpa_supp_api_ctrl.dev          = dev;
    wpa_supp_api_ctrl.requested_op = DISCONNECT;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_remove_network(const struct netif *dev, struct wlan_network *network)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;
#ifdef CONFIG_HOSTAPD
    struct hostapd_iface *hapd_s;
#endif

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

#ifdef CONFIG_HOSTAPD
    if (network->role == WLAN_BSS_ROLE_UAP)
    {
        hapd_s = get_hostapd_handle(dev);
        if (!hapd_s)
        {
            ret = -1;
            goto out;
        }
    }
    else
    {
#endif
        wpa_s = get_wpa_s_handle(dev);
        if (!wpa_s || network->id == -1)
        {
            ret = -1;
            goto out;
        }

        switch (network->security.type)
        {
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
            case WLAN_SECURITY_EAP_TLS:
            case WLAN_SECURITY_EAP_TLS_SHA256:
#ifdef CONFIG_11R
            case WLAN_SECURITY_EAP_TLS_FT:
            case WLAN_SECURITY_EAP_TLS_FT_SHA384:
#endif
            case WLAN_SECURITY_EAP_TTLS:
            case WLAN_SECURITY_EAP_TTLS_MSCHAPV2:
            case WLAN_SECURITY_EAP_PEAP_MSCHAPV2:
            case WLAN_SECURITY_EAP_PEAP_TLS:
            case WLAN_SECURITY_EAP_PEAP_GTC:
            case WLAN_SECURITY_EAP_SIM:
            case WLAN_SECURITY_EAP_AKA:
            case WLAN_SECURITY_EAP_AKA_PRIME:
            case WLAN_SECURITY_EAP_FAST_MSCHAPV2:
            case WLAN_SECURITY_EAP_FAST_GTC:
                wpa_config_remove_blob(wpa_s->conf, "ca_cert");
                wpa_config_remove_blob(wpa_s->conf, "client_cert");
                wpa_config_remove_blob(wpa_s->conf, "private_key");
                wpa_config_remove_blob(wpa_s->conf, "ca_cert2");
                wpa_config_remove_blob(wpa_s->conf, "cloent_cert2");
                wpa_config_remove_blob(wpa_s->conf, "private_key2");
                break;
#endif
            default:
                break;
        }

        ret = wpa_supplicant_remove_network(wpa_s, network->id);
        if (ret == -1)
        {
            wpa_printf(MSG_DEBUG,
                    "Could not find network "
                    "id=%d",
                    network->id);
            ret = -1;
            goto out;
        }
        if (ret == -2)
        {
            wpa_printf(MSG_DEBUG,
                    "Not able to remove the "
                    "network id=%d",
                    network->id);
            ret = -1;
            goto out;
        }

        ret = 0;

        wpa_supp_api_ctrl.dev          = dev;
        wpa_supp_api_ctrl.requested_op = REMOVE;

#ifdef CONFIG_HOSTAPD
    }
#endif

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}


int wpa_supp_pmksa_list(const struct netif *dev, char *buf, size_t buflen)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;
    size_t reply_len = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    reply_len = wpa_sm_pmksa_cache_list(wpa_s->wpa, buf, buflen);
#ifdef CONFIG_AP
    reply_len += wpas_ap_pmksa_cache_list(wpa_s, &buf[reply_len], buflen - reply_len);
#endif /* CONFIG_AP */
    
    if (reply_len == 0)
    {
        ret = -1;
    }

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_pmksa_flush(const struct netif *dev)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    ptksa_cache_flush(wpa_s->ptksa, NULL, WPA_CIPHER_NONE);
    wpa_sm_pmksa_cache_flush(wpa_s->wpa, NULL);
#ifdef CONFIG_AP
    wpas_ap_pmksa_cache_flush(wpa_s);
#endif /* CONFIG_AP */

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_reassociate(const struct netif *dev)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    wpas_request_connection(wpa_s);

    send_wpa_supplicant_dummy_event();

    wpa_supp_api_ctrl.dev          = dev;
    wpa_supp_api_ctrl.requested_op = RECONNECT;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_set_autoconnect(const struct netif *dev, int enable)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    wpa_s->auto_reconnect_disabled = !enable;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_set_scan_interval(const struct netif *dev, int scan_int)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    ret = wpa_supplicant_set_scan_interval(wpa_s, scan_int);

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

void wpa_supp_set_ap_max_num_sta(const struct netif *dev, unsigned int max_num_sta)
{
    h_max_num_sta = (int)max_num_sta;
}

void wpa_supp_set_ap_beacon_int(const struct netif *dev, unsigned short beacon_int)
{
    h_beacon_int = beacon_int;
}

void wpa_supp_set_ap_bw(const struct netif *dev, unsigned char bw)
{
    bandwidth = bw;
}

void wpa_supp_set_ap_hidden_ssid(const struct netif *dev, const t_u8 hidden_ssid)
{
    h_hidden_ssid = hidden_ssid;
}

int wpa_supp_set_ap_country(const struct netif *dev, const char *country, const unsigned char country3)
{
    int ret = 0;

#ifdef CONFIG_HOSTAPD
    struct hostapd_iface *hapd_s;
    struct hostapd_config *conf;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    hapd_s = get_hostapd_handle(dev);
    if (!hapd_s)
    {
        ret = -1;
        goto out;
    }

    conf = hapd_s->conf;

    conf->country[0] = country[0];
    conf->country[1] = country[1];
    conf->country[2] = country3;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

#else
    struct wpa_ssid *ssid = NULL;
    struct wpa_supplicant *wpa_s;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    wpa_s->conf->country[0] = country[0];
    wpa_s->conf->country[1] = country[1];
    wpa_s->conf->country[2] = country3;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);
#endif

    return ret;
}

void wpa_supp_set_ap_11d_state(const struct netif *dev, int state)
{
#ifdef CONFIG_HOSTAPD
    struct hostapd_iface *hapd_s;
    struct hostapd_config *conf;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    hapd_s = get_hostapd_handle(dev);
    if (!hapd_s)
    {
        goto out;
    }

    conf = hapd_s->conf;
    conf->ieee80211d = state ? 1 : 0;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);
#endif
}

int wpa_supp_start_ap(const struct netif *dev, struct wlan_network *network)
{
    int ret = 0;

#ifdef CONFIG_HOSTAPD
    struct hostapd_iface *hapd_s;
    struct hostapd_config *conf;
    struct hostapd_bss_config *bss;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    hapd_s = get_hostapd_handle(dev);
    if (!hapd_s)
    {
        ret = -1;
        goto out;
    }

    ret = hostapd_update_bss(hapd_s, network);
    if (ret != 0)
    {
        ret = -1;
        goto out;
    }

    conf = hapd_s->conf;
    bss  = conf->last_bss;

    bss->ignore_broadcast_ssid = h_hidden_ssid;
    conf->beacon_int = h_beacon_int;
    bss->max_num_sta = h_max_num_sta;

    if (bandwidth == 1)
    {
        conf->secondary_channel = 0;
        conf->ht_capab &= ~HT_CAP_INFO_SUPP_CHANNEL_WIDTH_SET;
        conf->ht_capab &= ~HT_CAP_INFO_SHORT_GI40MHZ;


#ifdef CONFIG_11AC
        conf->vht_oper_chwidth             = 0;
        conf->vht_oper_centr_freq_seg0_idx = 0;
#endif
#ifdef CONFIG_11AX
        conf->he_oper_chwidth             = 0;
        conf->he_oper_centr_freq_seg0_idx = 0;
#endif
    }
    else if (bandwidth == 2)
    {
        conf->ht_capab |= HT_CAP_INFO_SUPP_CHANNEL_WIDTH_SET;
        conf->ht_capab |= HT_CAP_INFO_SHORT_GI40MHZ;

#ifdef CONFIG_11AC
        conf->vht_oper_chwidth             = 0;
        conf->vht_oper_centr_freq_seg0_idx = 0;
#endif
#ifdef CONFIG_11AX
        conf->he_oper_chwidth             = 0;
        conf->he_oper_centr_freq_seg0_idx = 0;
#endif
    }
    else if (bandwidth == 3)
    {
        conf->ht_capab |= HT_CAP_INFO_SUPP_CHANNEL_WIDTH_SET;
        conf->ht_capab |= HT_CAP_INFO_SHORT_GI40MHZ;
    }

    ret                            = hostapd_enable_iface(hapd_s);
    wpa_supp_api_ctrl.dev          = dev;
    wpa_supp_api_ctrl.requested_op = START;
out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

#else
    struct wpa_ssid *ssid = NULL;
    struct wpa_supplicant *wpa_s;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    ssid = wpa_s->conf->ssid;

    while (ssid)
    {
        if (network->id == ssid->id)
        {
            break;
        }
        ssid = ssid->next;
    }

    if (ssid == NULL)
    {
        ret = -1;
        goto out;
    }

    wpa_s->connect_without_scan = ssid;

    wpa_supplicant_enable_network(wpa_s, ssid);

    wpa_supplicant_select_network(wpa_s, ssid);

    send_wpa_supplicant_dummy_event();

    wpa_supp_api_ctrl.dev = dev;
    wpa_supp_api_ctrl.requested_op = START;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);
#endif

    return ret;
}

int wpa_supp_stop_ap(const struct netif *dev, struct wlan_network *network)

{
    int ret = 0;

#ifdef CONFIG_HOSTAPD
    struct hostapd_iface *hapd_s;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    hapd_s = get_hostapd_handle(dev);
    if (!hapd_s)
    {
        ret = -1;
        goto out;
    }

    ret = hostapd_disable_iface(hapd_s);

    bandwidth = 2;
    h_hidden_ssid = 0;
    h_beacon_int = 100;
    h_max_num_sta = 8;

    ret = hostapd_free_bss(hapd_s, network);
    if (ret != 0)
    {
        ret = -1;
        goto out;
    }

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

#else
    struct wpa_supplicant *wpa_s;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    ret = wpas_ap_stop_ap(wpa_s);

    // wpa_supplicant_ap_deinit(wpa_s);

    wpa_supp_api_ctrl.dev = dev;
    wpa_supp_api_ctrl.requested_op = STOP;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);
#endif

    return ret;
}

void wpa_supp_set_bgscan(const struct netif *dev, const int short_interval, const int signal_threshold, const int long_interval)
{
    struct wpa_supplicant *wpa_s;
    char bgscan_str[128] = {0};

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        goto out;
    }

    snprintf(bgscan_str, 128, "simple:%d:%d:%d", short_interval, signal_threshold, long_interval);

    os_free(wpa_s->conf->bgscan);

    wpa_s->conf->bgscan = os_strdup(bgscan_str);

    //wpa_supplicant_reset_bgscan(wpa_s);

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);
}

int wpa_supp_set_okc(const struct netif *dev, unsigned char okc)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    wpa_s->conf->okc = okc;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_roam(const struct netif *dev, unsigned char *bssid)
{
    struct wpa_supplicant *wpa_s;
    // struct wpa_bss *bss;
    // struct wpa_ssid *ssid;
    struct wpa_radio_work *already_connecting;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

#if 0
    ssid = wpa_s->current_ssid;

    wpa_printf(MSG_DEBUG, "ROAM " MACSTR, MAC2STR(bssid));

    if (!ssid)
    {
        wpa_printf(MSG_DEBUG,
                   "ROAM: No network "
                   "configuration known for the target AP");
        ret = -1;
        goto out;
    }

    bss = wpa_bss_get(wpa_s, bssid, ssid->ssid, ssid->ssid_len);
    if (!bss)
    {
        wpa_printf(MSG_DEBUG,
                   "ROAM: Target AP not found "
                   "from BSS table");
        ret = -1;
        goto out;
    }

    /*
     * TODO: Find best network configuration block from configuration to
     * allow roaming to other networks
     */
#endif
    already_connecting       = radio_work_pending(wpa_s, "sme-connect");
    wpa_s->own_reconnect_req = 1;
    // wpa_supplicant_connect(wpa_s, bss, ssid);

    send_wpa_supplicant_dummy_event();

    /*
     * Indicate that an explicitly requested roam is in progress so scan
     * results that come in before the 'sme-connect' radio work gets
     * executed do not override the original connection attempt.
     */
    if (!already_connecting && radio_work_pending(wpa_s, "sme-connect"))
        wpa_s->roam_in_progress = true;

    wpa_supp_api_ctrl.dev          = dev;
    wpa_supp_api_ctrl.requested_op = ROAM;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

static void wpas_neighbor_rep_cb(void *ctx, struct wpabuf *neighbor_rep)
{
    struct wpa_supplicant *wpa_s = ctx;
    size_t len;
    const u8 *data;

    /*
     * Neighbor Report element (IEEE P802.11-REVmc/D5.0)
     * BSSID[6]
     * BSSID Information[4]
     * Operating Class[1]
     * Channel Number[1]
     * PHY Type[1]
     * Optional Subelements[variable]
     */
#define NR_IE_MIN_LEN (ETH_ALEN + 4 + 1 + 1 + 1)

    if (!neighbor_rep || wpabuf_len(neighbor_rep) == 0)
    {
        wpa_msg_ctrl(wpa_s, MSG_INFO, RRM_EVENT_NEIGHBOR_REP_FAILED);
        goto out;
    }

    data = wpabuf_head_u8(neighbor_rep);
    len  = wpabuf_len(neighbor_rep);

    while (len >= 2 + NR_IE_MIN_LEN)
    {
        const u8 *nr;
        char lci[256 * 2 + 1];
        char civic[256 * 2 + 1];
        u8 nr_len     = data[1];
        const u8 *pos = data, *end;

        if (pos[0] != WLAN_EID_NEIGHBOR_REPORT || nr_len < NR_IE_MIN_LEN)
        {
            wpa_dbg(wpa_s, MSG_DEBUG, "CTRL: Invalid Neighbor Report element: id=%u len=%u", data[0], nr_len);
            goto out;
        }

        if (2U + nr_len > len)
        {
            wpa_dbg(wpa_s, MSG_DEBUG, "CTRL: Invalid Neighbor Report element: id=%u len=%zu nr_len=%u", data[0], len,
                    nr_len);
            goto out;
        }
        pos += 2;
        end = pos + nr_len;

        nr = pos;
        pos += NR_IE_MIN_LEN;

        lci[0]   = '\0';
        civic[0] = '\0';
        while (end - pos > 2)
        {
            u8 s_id, s_len;

            s_id  = *pos++;
            s_len = *pos++;
            if (s_len > end - pos)
                goto out;
            if (s_id == WLAN_EID_MEASURE_REPORT && s_len > 3)
            {
                /* Measurement Token[1] */
                /* Measurement Report Mode[1] */
                /* Measurement Type[1] */
                /* Measurement Report[variable] */
                switch (pos[2])
                {
                    case MEASURE_TYPE_LCI:
                        if (lci[0])
                            break;
                        wpa_snprintf_hex(lci, sizeof(lci), pos, s_len);
                        break;
                    case MEASURE_TYPE_LOCATION_CIVIC:
                        if (civic[0])
                            break;
                        wpa_snprintf_hex(civic, sizeof(civic), pos, s_len);
                        break;
                }
            }

            pos += s_len;
        }

        wpa_msg_ctrl(wpa_s, MSG_INFO,
                     RRM_EVENT_NEIGHBOR_REP_RXED "bssid=" MACSTR " info=0x%x op_class=%u chan=%u phy_type=%u%s%s%s%s",
                     MAC2STR(nr), WPA_GET_LE32(nr + ETH_ALEN), nr[ETH_ALEN + 4], nr[ETH_ALEN + 5], nr[ETH_ALEN + 6],
                     lci[0] ? " lci=" : "", lci, civic[0] ? " civic=" : "", civic);

        data = end;
        len -= 2 + nr_len;
    }

out:
    wpabuf_free(neighbor_rep);
}

int wpa_supp_send_neighbor_rep(const struct netif *dev, char *pssid, int lci, int civic)
{
    struct wpa_supplicant *wpa_s;
    struct wpa_ssid_value ssid, *ssid_p = NULL;
    int ret;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (wpa_s == NULL)
    {
        ret = -1;
        goto out;
    }

    if (pssid != NULL)
    {
        os_memset(&ssid, 0x00, sizeof(struct wpa_ssid_value));
        ssid.ssid_len = os_strlen(pssid);
        os_memcpy(ssid.ssid, pssid, ssid.ssid_len);
        ssid_p = &ssid;
    }

    ret = wpas_rrm_send_neighbor_rep_request(wpa_s, ssid_p, lci, civic, wpas_neighbor_rep_cb, wpa_s);

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_send_btm_query(const struct netif *dev, int query_reason)
{
    struct wpa_supplicant *wpa_s;
    int ret;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    ret = wnm_send_bss_transition_mgmt_query(wpa_s, query_reason, NULL, 0);

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_mbo_update_non_pref_chan(const struct netif *dev, const char *non_pref_chan)
{
    struct wpa_supplicant *wpa_s;
    int ret;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    ret = wpas_mbo_update_non_pref_chan(wpa_s, non_pref_chan);

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_mbo_set_cell_capa(const struct netif *dev, unsigned char mbo_cell_capa)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    wpas_mbo_update_cell_capa(wpa_s, mbo_cell_capa);

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_mbo_set_oce(const struct netif *dev, unsigned char oce)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    wpa_s->conf->oce = oce;
    if (wpa_s->conf->oce)
    {
        if ((wpa_s->conf->oce & OCE_STA) && (wpa_s->drv_flags & WPA_DRIVER_FLAGS_OCE_STA))
            wpa_s->enable_oce = OCE_STA;

        if ((wpa_s->conf->oce & OCE_STA_CFON) && (wpa_s->drv_flags & WPA_DRIVER_FLAGS_OCE_STA_CFON))
        {
            /* TODO: Need to add STA-CFON support */
            wpa_printf(MSG_DEBUG, "OCE STA-CFON feature is not yet supported");
            ret = -1;
        }
    }
    else
    {
        wpa_s->enable_oce = 0;
    }
    wpa_supplicant_set_default_scan_ies(wpa_s);

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

#ifdef CONFIG_11R
int wpa_supp_ft_ds(const struct netif *dev, unsigned char *bssid)
{
    struct wpa_supplicant *wpa_s;
    struct wpa_bss *bss;
    const u8 *mdie;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        ret = -1;
        goto out;
    }

    bss = wpa_bss_get_bssid(wpa_s, bssid);
    if (bss)
        mdie = wpa_bss_get_ie(bss, WLAN_EID_MOBILITY_DOMAIN);
    else
        mdie = NULL;

    ret = wpa_ft_start_over_ds(wpa_s->wpa, bssid, mdie);

    wpa_supp_api_ctrl.dev          = dev;
    wpa_supp_api_ctrl.requested_op = FT_DS;

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}
#endif /* CONFIG_11R */

#ifdef CONFIG_HOSTAPD
int wpa_supp_get_sta_info(const struct netif *dev, unsigned char *sta_addr, unsigned char *is_11n_enabled)
{
    // struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    struct hostapd_iface *hapd_s;
    struct hostapd_data *hapd;
    struct sta_info *sta;

    hapd_s = get_hostapd_handle(dev);
    if (!hapd_s)
    {
        ret = -1;
        goto out;
    }
    hapd = hapd_s->bss[0];

    sta = ap_get_sta(hapd, sta_addr);
    if (!sta)
    {
        ret = -1;
        goto out;
    }

    if (sta->flags & WLAN_STA_HT)
    {
        *is_11n_enabled = 1;
    }
    else
    {
        *is_11n_enabled = 0;
    }

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}
#endif

#ifdef CONFIG_WPA_SUPP_WPS
int wpa_supp_start_wps_pbc(const struct netif *dev, int is_ap)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

#ifdef CONFIG_HOSTAPD
    if (is_ap)
    {
        struct hostapd_iface *hapd_s;
        struct hostapd_data *hapd;

        hapd_s = get_hostapd_handle(dev);
        if (!hapd_s)
        {
            ret = -1;
            goto out;
        }
        hapd = hapd_s->bss[0];

        ret = hostapd_wps_button_pushed(hapd, NULL);
    }
    else
    {
#endif
        wpa_s = get_wpa_s_handle(dev);
        if (!wpa_s)
        {
            ret = -1;
            goto out;
        }

#ifdef CONFIG_AP
        if (wpa_s->ap_iface)
        {
            ret = wpa_supplicant_ap_wps_pbc(wpa_s, NULL, NULL);
            goto out;
        }
#endif /* CONFIG_AP */

        ret = wpas_wps_start_pbc(wpa_s, NULL, 0, 0);

        wpa_supp_api_ctrl.dev          = dev;
        wpa_supp_api_ctrl.requested_op = WPS_PBC;
#ifdef CONFIG_HOSTAPD
    }
#endif
out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_start_wps_pin(const struct netif *dev, const char *pin, int is_ap)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

#ifdef CONFIG_HOSTAPD
    if (is_ap)
    {
        struct hostapd_iface *hapd_s;
        struct hostapd_data *hapd;

        hapd_s = get_hostapd_handle(dev);
        if (!hapd_s)
        {
            ret = -1;
            goto out;
        }
        hapd = hapd_s->bss[0];

        ret = hostapd_wps_add_pin(hapd, NULL, "any", pin, 0);
        if (ret != 0)
        {
            ret = -1;
            goto out;
        }
    }
    else
    {
#endif
        wpa_s = get_wpa_s_handle(dev);
        if (!wpa_s)
        {
            ret = -1;
            goto out;
        }

#ifdef CONFIG_AP
        if (wpa_s->ap_iface)
        {
            ret = wpa_supplicant_ap_wps_pin(wpa_s, NULL, pin, NULL, 0, 0);
            goto out;
        }
#endif /* CONFIG_AP */

        if (pin)
        {
            ret = wpas_wps_start_pin(wpa_s, NULL, pin, 0, DEV_PW_DEFAULT);
            goto out;
        }

        ret = wpas_wps_start_pin(wpa_s, NULL, NULL, 0, DEV_PW_DEFAULT);

        wpa_supp_api_ctrl.dev          = dev;
        wpa_supp_api_ctrl.requested_op = WPS_PIN;
#ifdef CONFIG_HOSTAPD
    }
#endif

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}

int wpa_supp_wps_pin_valid(const struct netif *dev, const unsigned char *tpin)
{
    (void)dev;

    char pin[9];
    size_t len;
    char *pos;

    wpa_hexdump_ascii_key(MSG_DEBUG, "WPS_CHECK_PIN", (u8 *)tpin, os_strlen((const char *)tpin));
    for (pos = (char *)tpin, len = 0; *pos != '\0'; pos++)
    {
        if (*pos < '0' || *pos > '9')
            continue;
        pin[len++] = *pos;
        if (len == 9)
        {
            wpa_printf(MSG_DEBUG, "WPS: Too long PIN");
            return -1;
        }
    }
    if (len != 4 && len != 8)
    {
        wpa_printf(MSG_DEBUG, "WPS: Invalid PIN length %d", (int)len);
        return -1;
    }
    pin[len] = '\0';

    if (len == 8)
    {
        unsigned int pin_val;
        pin_val = atoi(pin);
        if (!wps_pin_valid(pin_val))
        {
            wpa_printf(MSG_DEBUG, "WPS: Invalid checksum digit");
            return -1;
        }
    }

    return 0;
}

int wpa_supp_wps_generate_pin(const struct netif *dev, unsigned int *pin)
{
    (void)dev;

    return wps_generate_pin(pin);
}

int wpa_supp_cancel_wps(const struct netif *dev, int is_ap)
{
    struct wpa_supplicant *wpa_s;
    int ret = 0;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

#ifdef CONFIG_HOSTAPD
    if (is_ap)
    {
        struct hostapd_iface *hapd_s;
        struct hostapd_data *hapd;

        hapd_s = get_hostapd_handle(dev);
        if (!hapd_s)
        {
            ret = -1;
            goto out;
        }
        hapd = hapd_s->bss[0];

        ret = hostapd_wps_cancel(hapd);
    }
    else
    {
#endif
        wpa_s = get_wpa_s_handle(dev);
        if (!wpa_s)
        {
            ret = -1;
            goto out;
        }

        ret = wpas_wps_cancel(wpa_s);

        wpa_supp_api_ctrl.dev          = dev;
        wpa_supp_api_ctrl.requested_op = WPS_CANCEL;
#ifdef CONFIG_HOSTAPD
    }
#endif

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);

    return ret;
}
#endif

static inline enum wlan_security_type wpas_key_mgmt_to_wpa(int key_mgmt)
{
    switch (key_mgmt)
    {
        case WPA_KEY_MGMT_NONE:
            return WLAN_SECURITY_NONE;
        case WPA_KEY_MGMT_PSK:
            return WLAN_SECURITY_WPA2;
        case WPA_KEY_MGMT_PSK_SHA256:
            return WLAN_SECURITY_WPA2_SHA256;
        case WPA_KEY_MGMT_SAE:
            return WLAN_SECURITY_WPA3_SAE;
        default:
            return WLAN_SECURITY_WILDCARD;
    }
}

int wpa_supp_status(const struct netif *dev)
{
    int status = 0;
    struct wpa_supplicant *wpa_s;
    struct wpa_supp_api_ctrl *ctrl = &wpa_supp_api_ctrl;

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

    wpa_s = get_wpa_s_handle(dev);
    if (!wpa_s)
    {
        status = -1;
        goto out;
    }

    ctrl->supp_thread_state = SUPP_THREAD_RUNNING;

#ifdef CONFIG_HOSTAPD
    struct hostapd_iface *hapd_s;

    hapd_s = get_hostapd_handle(dev);
    if (!hapd_s)
    {
        status = -1;
        goto out;
    }
#endif

out:
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);
    return status;
}

#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE

static int eapol_get_status(struct wpa_supplicant *wpa_s, struct wlan_network *network)
{
    int ret = 0;
    char *reply;
    const int reply_size = 512;
    int ver, method = EAP_TYPE_NONE;
    char *pos, *pos2, *pos3, *m;
    char a[128], name[10] = {0}, p2m[20] = {0};
    int name_set = 0;

    reply = os_zalloc(reply_size);
    if (reply == NULL)
    {
        ret = -1;
        goto out;
    }

    ret = eapol_sm_get_status(wpa_s->eapol, reply, reply_size, 0);
    if (ret == 0)
    {
        ret = -1;
        goto out;
    }

    pos2 = reply;

    do
    {
        if (pos2 == NULL)
        {
            break;
        }

        (void)sscanf(pos2, "%[^\r\n]s", a);

        pos = os_strstr(pos2, "\r\n");

        if (pos == NULL)
        {
            break;
        }

        pos2 = pos + 2;

        pos3 = os_strstr(a, "selectedMethod=");

        if (pos3)
        {
            method = atoi(pos3 + 15);

            if ((method != EAP_TYPE_TTLS) && (method != EAP_TYPE_PEAP))
            {
                break;
            }

            pos3 = os_strstr(a, "(");

            m = pos3 + 1;

            pos3 = os_strstr(a, ")");

            *pos3 = '\0';

            strcpy(name, m);

            name_set = 1;

            continue;
        }

        if (name_set)
        {
            pos3 = os_strstr(a, name);

            if (pos3)
            {
                ver = atoi(pos3 + 9);

                network->security.eap_ver = ver;

                pos3 = os_strstr(a, "Phase2 method=");

                if (pos3)
                {
                    pos3 = pos3 + 14;

                    strcpy(p2m, pos3);
                }
            }
        }
    } while (pos);

    if (method == EAP_TYPE_TLS)
    {
        network->security.type = WLAN_SECURITY_EAP_TLS;
    }
    else if (method == EAP_TYPE_SIM)
    {
        network->security.type = WLAN_SECURITY_EAP_SIM;
    }
    else if (method == EAP_TYPE_AKA)
    {
        network->security.type = WLAN_SECURITY_EAP_AKA;
    }
    else if (method == EAP_TYPE_AKA_PRIME)
    {
        network->security.type = WLAN_SECURITY_EAP_AKA_PRIME;
    }
    else if (method == EAP_TYPE_TTLS)
    {
        if (os_strstr(p2m, "EAP-TLS") != 0)
        {
            network->security.type = WLAN_SECURITY_EAP_TTLS;
        }
        else if (os_strstr(p2m, "MSCHAPV2") != 0)
        {
            network->security.type = WLAN_SECURITY_EAP_TTLS_MSCHAPV2;
        }
    }
    else if (method == EAP_TYPE_PEAP)
    {
        if (os_strstr(p2m, "TLS") != 0)
        {
            network->security.type = WLAN_SECURITY_EAP_PEAP_TLS;
        }
        else if (os_strstr(p2m, "MSCHAPV2") != 0)
        {
            network->security.type = WLAN_SECURITY_EAP_PEAP_MSCHAPV2;
        }
        else if (os_strstr(p2m, "GTC") != 0)
        {
            network->security.type = WLAN_SECURITY_EAP_PEAP_GTC;
        }
    }
    else if (method == EAP_TYPE_FAST)
    {
        if (os_strstr(p2m, "MSCHAPV2") != 0)
        {
            network->security.type = WLAN_SECURITY_EAP_FAST_MSCHAPV2;
        }
        else if (os_strstr(p2m, "GTC") != 0)
        {
            network->security.type = WLAN_SECURITY_EAP_FAST_GTC;
        }
    }

out:
    if (reply)
    {
        os_free(reply);
    }

    return ret;
}
#endif

int wpa_supp_network_status(const struct netif *dev, struct wlan_network *network)
{
    struct wpa_supplicant *wpa_s;
    int ret                    = -1;
    struct wpa_signal_info *si = NULL;
#ifdef CONFIG_HOSTAPD
    struct hostapd_iface *hapd_s;
#endif

    OSA_MutexLock((osa_mutex_handle_t)wpa_supplicant_mutex, osaWaitForever_c);

#ifdef CONFIG_HOSTAPD
    if (network->role == WLAN_BSS_ROLE_UAP)
    {
        hapd_s = get_hostapd_handle(dev);
        if (!hapd_s)
        {
            ret = -1;
            goto out;
        }
    }
    else
    {
#endif
        wpa_s = get_wpa_s_handle(dev);
        if (!wpa_s)
        {
            goto out;
        }

        si = os_zalloc(sizeof(struct wpa_signal_info));
        if (!si)
        {
            goto out;
        }

        if (wpa_s->wpa_state >= WPA_ASSOCIATED)
        {
            struct wpa_ssid *ssid = wpa_s->current_ssid;
            u8 channel;

            os_memcpy(network->bssid, wpa_s->bssid, ETH_ALEN);

            if (ssid->ieee80211w == MGMT_FRAME_PROTECTION_REQUIRED)
            {
                network->security.mfpc = 1;
                network->security.mfpr = 1;
            }
            else if (ssid->ieee80211w == MGMT_FRAME_PROTECTION_OPTIONAL)
            {
                network->security.mfpc = 1;
            }

            ieee80211_freq_to_chan(wpa_s->assoc_freq, &channel);
            network->channel = channel;

            if (ssid)
            {
                u8 *_ssid                 = ssid->ssid;
                size_t ssid_len           = ssid->ssid_len;
                u8 ssid_buf[SSID_MAX_LEN] = {0};

                if (ssid_len == 0)
                {
                    int _res = wpa_drv_get_ssid(wpa_s, ssid_buf);

                    if (_res < 0)
                        ssid_len = 0;
                    else
                        ssid_len = _res;
                    _ssid = ssid_buf;
                }
                os_memcpy(network->ssid, _ssid, ssid_len);

#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
                if ((network->role == WLAN_BSS_ROLE_STA) && (network->security.type == WLAN_SECURITY_EAP_WILDCARD))
                {
                    (void)eapol_get_status(wpa_s, network);
                }
#endif
                if (os_strstr(network->name, "wps_network"))
                {
                    os_memset(network->name, 0x00, sizeof(network->name));
                    os_memcpy(network->name, _ssid, ssid_len);
                    network->id            = ssid->id;
                    network->security.type = wpas_key_mgmt_to_wpa(ssid->key_mgmt);

                    if (ssid->export_keys)
                    {
                        if (ssid->psk_set)
                        {
                            network->security.pmk_valid = true;
                            os_memcpy(network->security.pmk, ssid->psk, PMK_LEN);
                        }
                        else if (network->security.type == WLAN_SECURITY_WPA3_SAE)
                        {
                            network->security.password_len = os_strlen(ssid->passphrase);
                            os_memcpy(network->security.password, ssid->passphrase, network->security.password_len);
                        }
                        else
                        {
                            network->security.psk_len = os_strlen(ssid->passphrase);
                            os_memcpy(network->security.psk, ssid->passphrase, network->security.psk_len);
                        }
                    }
                }
            }
            ret = wpa_drv_signal_poll(wpa_s, si);
            if (!ret)
            {
                network->rssi = si->current_signal;
            }
            else
            {
                network->rssi = -WPA_INVALID_NOISE;
            }

            if (wpa_bss_get_ie(wpa_s->current_bss, WLAN_EID_HT_CAP))
            {
                network->dot11n = true;
            }
#ifdef CONFIG_11AC
            if (wpa_bss_get_ie(wpa_s->current_bss, WLAN_EID_VHT_CAP))
            {
                network->dot11ac = true;
            }
#endif
#ifdef CONFIG_11AX
            if (wpa_bss_get_ie(wpa_s->current_bss, WLAN_EID_EXTENSION))
            {
                network->dot11ax = true;
            }
#endif

#ifdef CONFIG_11K
            network->neighbor_report_supported = wpa_s->rrm.rrm_used == 1 ? true : false;
#endif
#ifdef CONFIG_11V
            if (wpa_bss_ext_capab(wpa_s->current_bss, WLAN_EXT_CAPAB_BSS_TRANSITION))
            {
                network->bss_transition_supported = true;
            }
            else
            {
                network->bss_transition_supported = false;
            }
#endif
        }
        else
        {
            ret = 0;
        }
#ifdef CONFIG_HOSTAPD
    }
#endif
out:
    os_free(si);
    OSA_MutexUnlock((osa_mutex_handle_t)wpa_supplicant_mutex);
    return ret;
}

static void (*msg_cb_ptr)(const char *txt, size_t len);

static void wpa_supplicant_msg_cb(void *ctx, int level, enum wpa_msg_type type, const char *txt, size_t len)
{
    msg_cb_ptr(txt, len);
}

int wpa_supp_init(void (*msg_cb)(const char *txt, size_t len))
{
    osa_status_t status;

    status = OSA_MutexCreate((osa_mutex_handle_t)wpa_supplicant_mutex);
    if (status != KOSA_StatusSuccess)
    {
        wpa_printf(MSG_DEBUG, "Failed to initialize wpa supp mutex");
        return -1;
    }

    status = OSA_SemaphoreCreateBinary((osa_semaphore_handle_t)wpaSuppReadySemaphoreHandle);
    if (status != KOSA_StatusSuccess)
    {
        wpa_printf(MSG_DEBUG, "Failed to initialize wpa supp ready semaphore");
        return -1;
    }

    msg_cb_ptr = msg_cb;

    wpa_msg_register_cb(wpa_supplicant_msg_cb);

#ifdef CONFIG_HOSTAPD
    status = OSA_SemaphoreCreateBinary((osa_semaphore_handle_t)hostapdReadySemaphoreHandle);
    if (status != KOSA_StatusSuccess)
    {
        wpa_printf(MSG_DEBUG, "Failed to initialize wpa hostapd ready semaphore");
        return -1;
    }
#endif

    return 0;
}

int wpa_supp_deinit(void)
{
    osa_status_t status;

    status = OSA_MutexDestroy((osa_mutex_handle_t)wpa_supplicant_mutex);
    if (status != KOSA_StatusSuccess)
    {
        wpa_printf(MSG_DEBUG, "Failed to destroy wpa supp mutex");
        return -1;
    }

    status = OSA_SemaphoreDestroy((osa_semaphore_handle_t)wpaSuppReadySemaphoreHandle);
    if (status != KOSA_StatusSuccess)
    {
        wpa_printf(MSG_DEBUG, "Failed to destroy wpa supp ready semaphore");
        return -1;
    }

#ifdef CONFIG_HOSTAPD
    status = OSA_SemaphoreDestroy((osa_semaphore_handle_t)hostapdReadySemaphoreHandle);
    if (status != KOSA_StatusSuccess)
    {
        wpa_printf(MSG_DEBUG, "Failed to destroy wpa hostapd ready semaphore");
        return -1;
    }
#endif

    return 0;
}
