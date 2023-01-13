/** @file mlan_api.h
 *
 *  @brief MLAN Interface
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

#ifndef __MLAN_API_H__
#define __MLAN_API_H__
#include <string.h>
#include <wmtypes.h>
#include <wlan.h>

#include "fsl_debug_console.h"

#define MLAN_WMSDK_MAX_WPA_IE_LEN 256U

#include "mlan.h"
#include "mlan_join.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_main_defs.h"
#include "mlan_wmm.h"
#include "mlan_11n.h"
#include "mlan_11h.h"
#include "mlan_11ac.h"
#ifdef CONFIG_11AX
#include "mlan_11ax.h"
#endif
#include "mlan_11n_aggr.h"
#ifndef RW610
#include "mlan_sdio.h"
#endif
#include "mlan_11n_rxreorder.h"
#include "mlan_meas.h"
#include "mlan_uap.h"
#include <wifi-debug.h>
#include "wifi-internal.h"

/* #define CONFIG_WIFI_DEBUG */

#ifdef CONFIG_WIFI_DEBUG
/* #define DEBUG_11N_ASSOC */
/* #define DEBUG_11N_AGGR */
/* #define DEBUG_11N_REORDERING */
#define DEBUG_MLAN
/* #define DEBUG_DEVELOPMENT */
/* #define DUMP_PACKET_MAC */
#endif /* CONFIG_WIFI_DEBUG */

#ifdef EXIT
#undef EXIT
#define EXIT(...)
#endif /* EXIT */

#ifdef DEBUG_MLAN

#ifdef PRINTM
#undef PRINTM
#define PRINTM(level, ...)                   \
    do                                       \
    {                                        \
        (void)PRINTF("[mlan] " __VA_ARGS__); \
        (void)PRINTF("\n\r");                \
    } while (0)
#else
#define PRINTM(...)
#endif /* PRINTM */

#ifdef DBG_HEXDUMP
#undef DBG_HEXDUMP
#define DBG_HEXDUMP(level, x, y, z)       \
    do                                    \
    {                                     \
        (void)PRINTF("[mlan] %s\r\n", x); \
        dump_hex(y, z);                   \
        (void)PRINTF("\r\n");             \
    } while (0)
#else
#define DBG_HEXDUMP(...)
#endif /* DBG_HEXDUMP */

#ifdef HEXDUMP
#undef HEXDUMP
#define HEXDUMP(x, y, z)                  \
    do                                    \
    {                                     \
        (void)PRINTF("[mlan] %s\r\n", x); \
        dump_hex(y, z);                   \
        (void)PRINTF("\r\n");             \
    } while (0)
#else
#define HEXDUMP(...)
#endif /* HEXDUMP */
#endif /* DEBUG_MLAN */

#define DOT11N_CFG_ENABLE_RIFS            0x08
#define DOT11N_CFG_ENABLE_GREENFIELD_XMIT (1U << 4)
#define DOT11N_CFG_ENABLE_SHORT_GI_20MHZ  (1U << 5)
#define DOT11N_CFG_ENABLE_SHORT_GI_40MHZ  (1U << 6)

#define CLOSEST_DTIM_TO_LISTEN_INTERVAL 65534

#define SDIO_DMA_ALIGNMENT 4

extern os_thread_t wifi_scan_thread;

/* Following is allocated in mlan_register */
extern mlan_adapter *mlan_adap;
extern country_code_t wifi_11d_country;


extern os_rw_lock_t sleep_rwlock;

extern bool sta_ampdu_rx_enable;
#ifdef DUMP_PACKET_MAC
void dump_mac_addr(const char *msg, unsigned char *addr);
#endif /* DUMP_PACKET_MAC */
mlan_status wifi_prepare_and_send_cmd(IN mlan_private *pmpriv,
                                      IN t_u16 cmd_no,
                                      IN t_u16 cmd_action,
                                      IN t_u32 cmd_oid,
                                      IN t_void *pioctl_buf,
                                      IN t_void *pdata_buf,
                                      mlan_bss_type bss_type,
                                      void *priv);
int wifi_uap_prepare_and_send_cmd(mlan_private *pmpriv,
                                  t_u16 cmd_no,
                                  t_u16 cmd_action,
                                  t_u32 cmd_oid,
                                  t_void *pioctl_buf,
                                  t_void *pdata_buf,
                                  mlan_bss_type bss_type,
                                  void *priv);

bool wmsdk_is_11N_enabled(void);

/**
 * Abort the split scan if it is in progress.
 *
 * After this call returns this scan function will abort the current split
 * scan and return back to the caller. The scan list may be incomplete at
 * this moment. There are no other side effects on the scan function apart
 * from this. The next call to scan function should proceed as normal.
 */
void wlan_abort_split_scan(void);

void wlan_scan_process_results(IN mlan_private *pmpriv);

bool check_for_wpa2_entp_ie(bool *wpa2_entp_IE_exist, const void *element_data, unsigned element_len);

static inline mlan_status wifi_check_bss_entry_wpa2_entp_only(BSSDescriptor_t *pbss_entry,
                                                              IEEEtypes_ElementId_e element_id)
{
    if (element_id == RSN_IE)
    {
        (void)check_for_wpa2_entp_ie(&pbss_entry->wpa2_entp_IE_exist, pbss_entry->rsn_ie_buff + 8,
                                     pbss_entry->rsn_ie_buff_len - 10);
    }
    return MLAN_STATUS_SUCCESS;
}

int wifi_send_hostcmd(
    void *cmd_buf, uint32_t cmd_buf_len, void *resp_buf, uint32_t resp_buf_len, uint32_t *reqd_resp_len);

int wifi_send_get_wpa_pmk(int mode, char *ssid);
int wifi_deauthenticate(uint8_t *bssid);
int wifi_get_eeprom_data(uint32_t offset, uint32_t byte_count, uint8_t *buf);
int wifi_get_mgmt_ie(mlan_bss_type bss_type, IEEEtypes_ElementId_t index, void *buf, unsigned int *buf_len);
int wifi_send_remain_on_channel_cmd(unsigned int bss_type, wifi_remain_on_channel_t *remain_on_channel);
int wifi_set_smart_mode_cfg(char *ssid,
                            int beacon_period,
                            wifi_chan_list_param_set_t *chan_list,
                            uint8_t *smc_start_addr,
                            uint8_t *smc_end_addr,
                            uint16_t filter_type,
                            int smc_frame_filter_len,
                            uint8_t *smc_frame_filter,
                            int custom_ie_len,
                            uint8_t *custom_ie);
wifi_sub_band_set_t *get_sub_band_from_country(country_code_t country, t_u8 *nr_sb);
#ifdef CONFIG_5GHz_SUPPORT
wifi_sub_band_set_t *get_sub_band_from_country_5ghz(country_code_t country, t_u8 *nr_sb);
#endif
int wifi_set_mgmt_ie(mlan_bss_type bss_type, IEEEtypes_ElementId_t id, void *buf, unsigned int buf_len);
int wifi_clear_mgmt_ie(mlan_bss_type bss_type, IEEEtypes_ElementId_t index);
#ifdef SD8801
int wifi_get_ext_coex_stats(wifi_ext_coex_stats_t *ext_coex_stats);
int wifi_set_ext_coex_config(const wifi_ext_coex_config_t *ext_coex_config);
#endif
int wifi_send_enable_supplicant(int mode, const char *ssid);
int wifi_send_clear_wpa_psk(int mode, const char *ssid);
int wifi_send_add_wpa_psk(int mode, char *ssid, char *passphrase, unsigned int len);
int wifi_send_add_wpa3_password(int mode, char *ssid, char *password, unsigned int len);
int wifi_send_add_wpa_pmk(int mode, char *ssid, char *bssid, char *pmk, unsigned int len);
bool wifi_11d_is_channel_allowed(int channel);
/**
 * Get the string representation of the wlan firmware extended version.
 *
 * @param[out] version_ext Extended Version
 *
 * @return WM_SUCCESS on success or error code.
 */
int wifi_get_firmware_version_ext(wifi_fw_version_ext_t *version_ext);
/**
 * Get the string representation of the wlan firmware version.
 *
 * @param[out] ver Version
 *
 * @return WM_SUCCESS on success or error code.
 */
int wifi_get_firmware_version(wifi_fw_version_t *ver);
int wifi_get_smart_mode_cfg(void);
int wifi_start_smart_mode(void);
/**
 * Get Calibration data from WLAN firmware
 *
 * \param[out] cal_data Pointer to calibration data structure where
 *	      calibration data and it's length will be stored.
 *
 * \return WM_SUCCESS if cal data read operation is successful.
 * \return -WM_FAIL if cal data field is not present or command fails.
 *
 * \note The user of this API should free the allocated buffer for
 *	 calibration data.
 *
 */
int wifi_get_cal_data(wifi_cal_data_t *cal_data);
int wifi_set_key(int bss_index,
                 bool is_pairwise,
                 const uint8_t key_index,
                 const uint8_t *key,
                 unsigned key_len,
                 const uint8_t *mac_addr);
/**
 * Get User Data from OTP Memory
 *
 * \param[in] buf Pointer to buffer where data will be stored
 * \param[in] len Number of bytes to read
 *
 * \return WM_SUCCESS if user data read operation is successful.
 * \return -WM_FAIL if user data field is not present or command fails.
 */
int wifi_get_otp_user_data(uint8_t *buf, uint16_t len);
int wifi_reg_access(wifi_reg_t reg_type, uint16_t action, uint32_t offset, uint32_t *value);
int wifi_enable_11d_support_APIs(void);
int wifi_get_tsf(uint32_t *tsf_high, uint32_t *tsf_low);
int wifi_set_igtk_key(int bss_index, const uint8_t *pn, const uint16_t key_index, const uint8_t *key, unsigned key_len);
int wifi_send_scan_cmd(t_u8 bss_mode,
                       const t_u8 *specific_bssid,
                       const char *ssid,
                       const char *ssid2,
                       const t_u8 num_channels,
                       const wifi_scan_channel_list_t *chan_list,
                       const t_u8 num_probes,
                       const bool keep_previous_scan,
                       const bool active_scan_triggered);
int wifi_stop_smart_mode(void);
const char *wifi_get_country_str(country_code_t country);
int wifi_remove_key(int bss_index, bool is_pairwise, const uint8_t key_index, const uint8_t *mac_addr);
int wifi_enable_ecsa_support(void);
int wifi_set_ed_mac_mode(wifi_ed_mac_ctrl_t *wifi_ed_mac_ctrl);
int wifi_get_pmfcfg(t_u8 *mfpc, t_u8 *mfpr);
int wifi_get_ed_mac_mode(wifi_ed_mac_ctrl_t *wifi_ed_mac_ctrl);
int wifi_set_pmfcfg(t_u8 mfpc, t_u8 mfpr);
int wifi_set_chanlist(wifi_chanlist_t *chanlist);
int wifi_get_txpwrlimit(wifi_SubBand_t subband, wifi_txpwrlimit_t *txpwrlimit);
int wifi_get_data_rate(wifi_ds_rate *ds_rate);
void wifi_get_active_channel_list(t_u8 *chan_list, t_u8 *num_chans);
bool wifi_is_ecsa_enabled(void);
int wifi_set_txpwrlimit(wifi_txpwrlimit_t *txpwrlimit);
int wifi_send_rssi_info_cmd(wifi_rssi_info_t *rssi_info);
void wifi_set_curr_bss_channel(uint8_t channel);
int wifi_get_chanlist(wifi_chanlist_t *chanlist);
int wifi_set_eu_crypto(EU_Crypto *Crypto_Data, enum _crypto_algorithm Algorithm, t_u16 EncDec);
#endif /* __MLAN_API_H__ */
