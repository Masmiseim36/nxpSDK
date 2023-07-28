/*
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** @file wifi.h
 *
 * @brief This file contains interface to wifi driver
 */

#ifndef __WIFI_H__
#define __WIFI_H__





#include <wifi-decl.h>
#include <wifi_events.h>
#include <wm_os.h>
#include <wmerrno.h>

#define BANDWIDTH_20MHZ 1U
#define BANDWIDTH_40MHZ 2U
#ifdef CONFIG_11AC
#define BANDWIDTH_80MHZ 3U
#endif

#define MAX_NUM_CHANS_IN_NBOR_RPT 6U

/** BIT value */
#ifndef MBIT
#define MBIT(x) (((t_u32)1) << (x))
#endif

#define WIFI_MGMT_DIASSOC MBIT(10)
#define WIFI_MGMT_AUTH    MBIT(11)
#define WIFI_MGMT_DEAUTH  MBIT(12)
/** BITMAP for Action frame */
#define WIFI_MGMT_ACTION MBIT(13)

extern t_u8 wifi_tx_status;
extern t_u8 wifi_tx_block_cnt;
extern t_u8 wifi_rx_status;
extern t_u8 wifi_rx_block_cnt;

extern int16_t g_bcn_nf_last;
extern uint8_t g_rssi;
extern uint16_t g_data_nf_last;
extern uint16_t g_data_snr_last;

/** WiFi Error Code */
enum
{
    WM_E_WIFI_ERRNO_START = MOD_ERROR_START(MOD_WIFI),
    /** The Firmware download operation failed. */
    WIFI_ERROR_FW_DNLD_FAILED,
    /** The Firmware ready register not set. */
    WIFI_ERROR_FW_NOT_READY,
    /** The WiFi card not found. */
    WIFI_ERROR_CARD_NOT_DETECTED,
    /** The WiFi Firmware not found. */
    WIFI_ERROR_FW_NOT_DETECTED,
};

/** WiFi driver TX/RX data status */
enum
{
    /** Data in running status */
    WIFI_DATA_RUNNING = 0,
    /** Data in block status */
    WIFI_DATA_BLOCK = 1,
};

typedef enum
{
    MGMT_RSN_IE = 48,
#ifdef CONFIG_11K
    MGMT_RRM_ENABLED_CAP = 70,
#endif
    MGMT_VENDOR_SPECIFIC_221 = 221,
    MGMT_WPA_IE              = MGMT_VENDOR_SPECIFIC_221,
    MGMT_WPS_IE              = MGMT_VENDOR_SPECIFIC_221,
    MGMT_MBO_IE              = MGMT_VENDOR_SPECIFIC_221,
} IEEEtypes_ElementId_t;

typedef struct wifi_uap_client_disassoc
{
    int reason_code;
    t_u8 sta_addr[MLAN_MAC_ADDR_LENGTH];
} wifi_uap_client_disassoc_t;

/**
 * Initialize Wi-Fi driver module.
 *
 * Performs SDIO init, downloads Wi-Fi Firmware, creates Wi-Fi Driver
 * and command response processor thread.
 *
 * Also creates mutex, and semaphores used in command and data synchronizations.
 *
 * \param[in] fw_start_addr address of stored Wi-Fi Firmware.
 * \param[in] size Size of Wi-Fi Firmware.
 *
 * \return WM_SUCCESS on success or -WM_FAIL on error.
 *
 */
int wifi_init(const uint8_t *fw_start_addr, const size_t size);

/**
 * Initialize Wi-Fi driver module for FCC Certification.
 *
 * Performs SDIO init, downloads Wi-Fi Firmware, creates Wi-Fi Driver
 * and command response processor thread.
 *
 * Also creates mutex, and semaphores used in command and data synchronizations.
 *
 * \param[in] fw_start_addr address of stored Manufacturing Wi-Fi Firmware.
 * \param[in] size Size of Manufacturing Wi-Fi Firmware.
 *
 * \return WM_SUCCESS on success or -WM_FAIL on error.
 *
 */
int wifi_init_fcc(const uint8_t *fw_start_addr, const size_t size);

/**
 * Deinitialize Wi-Fi driver module.
 *
 * Performs SDIO deinit, send shutdown command to Wi-Fi Firmware, deletes
 * Wi-Fi Driver and command processor thread.
 *
 * Also deletes mutex and semaphores used in command and data synchronizations.
 *
 */
void wifi_deinit(void);
/**
 * This API can be used to set wifi driver tx status.
 */
void wifi_set_tx_status(t_u8 status);

/**
 * This API can be used to set wifi driver rx status.
 */
void wifi_set_rx_status(t_u8 status);

/**
 * Register Data callback function with Wi-Fi Driver to receive
 * DATA from SDIO.
 *
 * This callback function is used to send data received from Wi-Fi
 * firmware to the networking stack.
 *
 * @param[in] data_intput_callback Function that needs to be called
 *
 * @return WM_SUCCESS
 */
int wifi_register_data_input_callback(void (*data_intput_callback)(const uint8_t interface,
                                                                   const uint8_t *buffer,
                                                                   const uint16_t len));

/** Deregister Data callback function from Wi-Fi Driver */
void wifi_deregister_data_input_callback(void);

/**
 * Register Data callback function with Wi-Fi Driver to receive
 * processed AMSDU DATA from Wi-Fi driver.
 *
 * This callback function is used to send data received from Wi-Fi
 * firmware to the networking stack.
 *
 * @param[in] amsdu_data_intput_callback Function that needs to be called
 *
 * @return WM_SUCESS
 *
 */
int wifi_register_amsdu_data_input_callback(void (*amsdu_data_intput_callback)(uint8_t interface,
                                                                               uint8_t *buffer,
                                                                               uint16_t len));

/** Deregister Data callback function from Wi-Fi Driver */
void wifi_deregister_amsdu_data_input_callback(void);

int wifi_register_deliver_packet_above_callback(void (*deliver_packet_above_callback)(void *rxpd,
                                                                                      uint8_t interface,
                                                                                      void *lwip_pbuf));

void wifi_deregister_deliver_packet_above_callback(void);

int wifi_register_wrapper_net_is_ip_or_ipv6_callback(bool (*wrapper_net_is_ip_or_ipv6_callback)(const t_u8 *buffer));

void wifi_deregister_wrapper_net_is_ip_or_ipv6_callback(void);

/**
 * Wi-Fi Driver low level output function.
 *
 * Data received from upper layer is passed to Wi-Fi Driver for transmission.
 *
 * \param[in] interface Interface on which DATA frame will be transmitted.
 *  0 for Station interface, 1 for uAP interface and 2 for Wi-Fi
 *  Direct interface.
 * \param[in] buffer A pointer pointing to DATA frame.
 * \param[in] len Length of DATA frame.
 *
 * \return WM_SUCCESS on success or -WM_E_NOMEM if memory is not available
 *  or -WM_E_BUSY if SDIO is busy.
 *
 */
int wifi_low_level_output(const uint8_t interface,
                          const uint8_t *buffer,
                          const uint16_t len
#ifdef CONFIG_WMM
                          ,
                          uint8_t pkt_prio,
                          uint8_t tid
#endif
);

/**
 * API to enable packet retries at wifi driver level.
 *
 * This API sets retry count which will be used by wifi driver to retry packet
 * transmission in case there was failure in earlier attempt. Failure may
 * happen due to SDIO write port un-availability or other failures in SDIO
 * write operation.
 *
 * \note Default value of retry count is zero.
 *
 * \param[in] count No of retry attempts.
 *
 */
void wifi_set_packet_retry_count(const int count);

/**
 * This API can be used to enable AMPDU support on the go
 * when station is a transmitter.
 */
void wifi_sta_ampdu_tx_enable(void);

/**
 * This API can be used to disable AMPDU support on the go
 * when station is a transmitter.
 */
void wifi_sta_ampdu_tx_disable(void);

/**
 * This API can be used to set tid to enable AMPDU support on the go
 * when station is a transmitter.
 * @param[in] tid tid value
 */
void wifi_sta_ampdu_tx_enable_per_tid(t_u8 tid);

/**
 * This API can be used to check if tid to enable AMPDU is allowed
 * when station is a transmitter.
 * @param[in] tid tid value
 * @return MTRUE or MFALSE
 */
t_u8 wifi_sta_ampdu_tx_enable_per_tid_is_allowed(t_u8 tid);

/**
 * This API can be used to enable AMPDU support on the go
 * when station is a receiver.
 */
void wifi_sta_ampdu_rx_enable(void);

/**
 * This API can be used to set tid to enable AMPDU support on the go
 * when station is a receiver.
 * @param[in] tid tid value
 */
void wifi_sta_ampdu_rx_enable_per_tid(t_u8 tid);

/**
 * This API can be used to check if tid to enable AMPDU is allowed
 * when station is a receiver.
 * @param[in] tid tid value
 * @return MTRUE or MFALSE
 */
t_u8 wifi_sta_ampdu_rx_enable_per_tid_is_allowed(t_u8 tid);

/**
 * This API can be used to enable AMPDU support on the go
 * when uap is a receiver.
 */
void wifi_uap_ampdu_rx_enable(void);

/**
 * This API can be used to set tid to enable AMPDU support on the go
 * when uap is a receiver.
 * @param[in] tid tid value
 */
void wifi_uap_ampdu_rx_enable_per_tid(t_u8 tid);

/**
 * This API can be used to check if tid to enable AMPDU is allowed
 * when uap is a receiver.
 * @param[in] tid tid value
 * @return MTRUE or MFALSE
 */
t_u8 wifi_uap_ampdu_rx_enable_per_tid_is_allowed(t_u8 tid);

/**
 * This API can be used to disable AMPDU support on the go
 * when uap is a receiver.
 */
void wifi_uap_ampdu_rx_disable(void);

/**
 * This API can be used to enable AMPDU support on the go
 * when uap is a transmitter.
 */
void wifi_uap_ampdu_tx_enable(void);

/**
 * This API can be used to set tid to enable AMPDU support on the go
 * when uap is a transmitter.
 * @param[in] tid tid value
 */
void wifi_uap_ampdu_tx_enable_per_tid(t_u8 tid);

/**
 * This API can be used to check if tid to enable AMPDU is allowed
 * when uap is a transmitter.
 * @param[in] tid tid value
 * @return MTRUE or MFALSE
 */
t_u8 wifi_uap_ampdu_tx_enable_per_tid_is_allowed(t_u8 tid);

/**
 * This API can be used to disable AMPDU support on the go
 * when uap is a transmitter.
 */
void wifi_uap_ampdu_tx_disable(void);

/**
 * This API can be used to disable AMPDU support on the go
 * when station is a receiver.
 */
void wifi_sta_ampdu_rx_disable(void);

/**
 * Get the device sta MAC address
 *
 * @param[out] mac_addr Mac address
 *
 * @return WM_SUCESS
 */
int wifi_get_device_mac_addr(wifi_mac_addr_t *mac_addr);

/**
 * Get the device uap MAC address
 *
 * @param[out] mac_addr Mac address
 *
 * @return WM_SUCESS
 */
int wifi_get_device_uap_mac_addr(wifi_mac_addr_t *mac_addr_uap);

/**
 * Get the cached string representation of the wlan firmware extended version.
 *
 * @param[in] fw_ver_ext Firmware Version Extended
 *
 * @return WM_SUCCESS
 */
int wifi_get_device_firmware_version_ext(wifi_fw_version_ext_t *fw_ver_ext);

/**
 * Get the timestamp of the last command sent to the firmware
 *
 * @return Timestamp in millisec of the last command sent
 */
unsigned wifi_get_last_cmd_sent_ms(void);

uint32_t wifi_get_value1(void);

uint8_t *wifi_get_outbuf(uint32_t *outbuf_len);



#ifdef CONFIG_ROAMING
int wifi_config_roaming(const int enable, uint8_t *rssi_low);
#endif
int wifi_config_bgscan_and_rssi(const char *ssid);

/**
 * This will update the last command sent variable value to current
 * time. This is used for power management.
 */
void wifi_update_last_cmd_sent_ms(void);

/**
 * Register an event queue with the wifi driver to receive events
 *
 * The list of events which can be received from the wifi driver are
 * enumerated in the file wifi_events.h
 *
 * @param[in] event_queue The queue to which wifi driver will post events.
 *
 * @note Only one queue can be registered. If the registered queue needs to
 * be changed unregister the earlier queue first.
 *
 * @return Standard SDK return codes
 */
int wifi_register_event_queue(os_queue_t *event_queue);

/**
 * Unregister an event queue from the wifi driver.
 *
 * @param[in] event_queue The queue to which was registered earlier with
 * the wifi driver.
 *
 * @return Standard SDK return codes
 */
int wifi_unregister_event_queue(os_queue_t *event_queue);

/** Get scan list
 *
 * @param[in] index Index
 * @param[out] desc Descriptor of type \ref wifi_scan_result2
 *
 * @return WM_SUCCESS on success or error code.
 *
 */
int wifi_get_scan_result(unsigned int index, struct wifi_scan_result2 **desc);

/**
 * Get the count of elements in the scan list
 *
 * @param[in,out] count Pointer to a variable which will hold the count after
 * this call returns
 *
 * @warning The count returned by this function is the current count of the
 * elements. A scan command given to the driver or some other background
 * event may change this count in the wifi driver. Thus when the API
 * \ref wifi_get_scan_result is used to get individual elements of the scan
 * list, do not assume that it will return exactly 'count' number of
 * elements. Your application should not consider such situations as a
 * major event.
 *
 * @return Standard SDK return codes.
 */
int wifi_get_scan_result_count(unsigned *count);

/**
 * Returns the current STA list connected to our uAP
 *
 * This function gets its information after querying the firmware. It will
 * block till the response is received from firmware or a timeout.
 *
 * @param[in, out] list After this call returns this points to the
 * structure \ref wifi_sta_list_t allocated by the callee. This is variable
 * length structure and depends on count variable inside it. <b> The caller
 * needs to free this buffer after use.</b>. If this function is unable to
 * get the sta list, the value of list parameter will be NULL
 *
 * \note The caller needs to explicitly free the buffer returned by this
 * function.
 *
 * @return void
 */
int wifi_uap_bss_sta_list(wifi_sta_list_t **list);



#ifdef WLAN_LOW_POWER_ENABLE
void wifi_enable_low_pwr_mode();
#endif

/** Set wifi calibration data in firmware.
 *
 * This function may be used to set wifi calibration data in firmware.
 *
 * @param[in] cdata The calibration data
 * @param[in] clen Length of calibration data
 *
 */
void wifi_set_cal_data(uint8_t *cdata, unsigned int clen);

/** Set wifi MAC address in firmware at load time.
 *
 * This function may be used to set wifi MAC address in firmware.
 *
 * @param[in] mac The new MAC Address
 *
 */
void wifi_set_mac_addr(uint8_t *mac);

/** Set wifi MAC address in firmware at run time.
 *
 * This function may be used to set wifi MAC address in firmware as per passed bss type.
 *
 * @param[in] mac The new MAC Address
 * @param[in] bss_type BSS Type
 *
 */
void _wifi_set_mac_addr(const uint8_t *mac, mlan_bss_type bss_type);


#ifdef CONFIG_WIFI_TX_BUFF
/**
 * Check whether the tx buffer size setting is reasonable.
 *
 * \param[in]   buf_size The tx buffer size
 *
 */
bool wifi_calibrate_tx_buf_size(uint16_t buf_size);
void wifi_recfg_tx_buf_size(uint16_t buf_size);
void _wifi_recfg_tx_buf_size(uint16_t buf_size, mlan_bss_type bss_type);
#endif

int wifi_get_wpa_ie_in_assoc(uint8_t *wpa_ie);

/** Add Multicast Filter by MAC Address
 *
 * Multicast filters should be registered with the WiFi driver for IP-level
 * multicast addresses to work. This API allows for registration of such filters
 * with the WiFi driver.
 *
 * If multicast-mapped MAC address is 00:12:23:34:45:56 then pass mac_addr as
 * below:
 * mac_add[0] = 0x00
 * mac_add[1] = 0x12
 * mac_add[2] = 0x23
 * mac_add[3] = 0x34
 * mac_add[4] = 0x45
 * mac_add[5] = 0x56
 *
 * \param[in] mac_addr multicast mapped MAC address
 *
 * \return 0 on Success or else Error
 */
int wifi_add_mcast_filter(uint8_t *mac_addr);

/** Remove Multicast Filter by MAC Address
 *
 * This function removes multicast filters for the given multicast-mapped
 * MAC address. If multicast-mapped MAC address is 00:12:23:34:45:56
 * then pass mac_addr as below:
 * mac_add[0] = 0x00
 * mac_add[1] = 0x12
 * mac_add[2] = 0x23
 * mac_add[3] = 0x34
 * mac_add[4] = 0x45
 * mac_add[5] = 0x56
 *
 * \param[in] mac_addr multicast mapped MAC address
 *
 * \return  0 on Success or else Error
 */
int wifi_remove_mcast_filter(uint8_t *mac_addr);

/** Get Multicast Mapped Mac address from IPv4
 *
 * This function will generate Multicast Mapped MAC address from IPv4
 * Multicast Mapped MAC address will be in following format:
 * 1) Higher 24-bits filled with IANA Multicast OUI (01-00-5E)
 * 2) 24th bit set as Zero
 * 3) Lower 23-bits filled with IP address (ignoring higher 9bits).
 *
 * \param[in] ipaddr ipaddress(input)
 * \param[in] mac_addr multicast mapped MAC address(output)
 *
 * \return  void
 */
void wifi_get_ipv4_multicast_mac(uint32_t ipaddr, uint8_t *mac_addr);

#ifdef CONFIG_IPV6
/** Get Multicast Mapped Mac address from IPv6 address
 *
 * This function will generate Multicast Mapped MAC address from IPv6 address.
 * Multicast Mapped MAC address will be in following format:
 * 1) Higher 16-bits filled with IANA Multicast OUI (33-33)
 * 2) Lower 32-bits filled with last 4 bytes of IPv6 address
 *
 * \param[in] ipaddr last 4 bytes of IPv6 address
 * \param[in] mac_addr multicast mapped MAC address
 *
 * \return void
 */
void wifi_get_ipv6_multicast_mac(uint32_t ipaddr, uint8_t *mac_addr);
#endif /* CONFIG_IPV6 */


int wifi_set_antenna(t_u32 ant_mode, t_u16 evaluate_time);
int wifi_get_antenna(t_u32 *ant_mode, t_u16 *evaluate_time, t_u16 *current_antenna);

void wifi_process_hs_cfg_resp(t_u8 *cmd_res_buffer);
enum wifi_event_reason wifi_process_ps_enh_response(t_u8 *cmd_res_buffer, t_u16 *ps_event, t_u16 *action);

int wifi_uap_rates_getset(uint8_t action, char *rates, uint8_t num_rates);
int wifi_uap_sta_ageout_timer_getset(uint8_t action, uint32_t *sta_ageout_timer);
int wifi_uap_ps_sta_ageout_timer_getset(uint8_t action, uint32_t *ps_sta_ageout_timer);
typedef enum
{
    REG_MAC = 1,
    REG_BBP,
    REG_RF,
    REG_CAU
} wifi_reg_t;

int wifi_mem_access(uint16_t action, uint32_t addr, uint32_t *value);
/*
 * This function is supposed to be called after scan is complete from wlc
 * manager.
 */
void wifi_scan_process_results(void);

/**
 * Get the wifi region code
 *
 * This function will return one of the following values in the region_code
 * variable.\n
 * 0x10 : US FCC\n
 * 0x20 : CANADA\n
 * 0x30 : EU\n
 * 0x32 : FRANCE\n
 * 0x40 : JAPAN\n
 * 0x41 : JAPAN\n
 * 0x50 : China\n
 * 0xfe : JAPAN\n
 * 0xff : Special\n
 *
 * @param[out] region_code Region Code
 *
 * @return Standard WMSDK return codes.
 */
int wifi_get_region_code(t_u32 *region_code);

/**
 * Set the wifi region code.
 *
 * This function takes one of the values from the following array.\n
 * 0x10 : US FCC\n
 * 0x20 : CANADA\n
 * 0x30 : EU\n
 * 0x32 : FRANCE\n
 * 0x40 : JAPAN\n
 * 0x41 : JAPAN\n
 * 0x50 : China\n
 * 0xfe : JAPAN\n
 * 0xff : Special\n
 *
 * @param[in] region_code Region Code
 *
 * @return Standard WMSDK return codes.
 */
int wifi_set_region_code(t_u32 region_code);

/**
 * Set/Get country code
 * \param[in] alpha2 country code in 3bytes string, 2bytes country code and 1byte 0
 *            WW : World Wide Safe
 *            US : US FCC
 *            CA : IC Canada
 *            SG : Singapore
 *            EU : ETSI
 *            AU : Australia
 *            KR : Republic Of Korea
 *            FR : France
 *            JP : Japan
 *            CN : China
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_country_code(const char *alpha2);
int wifi_get_country_code(char *alpha2);

/**
 * Get the uAP channel number
 *
 *
 * @param[in] channel Pointer to channel number. Will be initialized by
 * callee
 * @return Standard WMSDK return code
 */
int wifi_get_uap_channel(int *channel);

/**
 * Get/Set the uAP mfpc and mfpr
 *
 * @param[in] action
 *
 * \param[in/out] mfpc: Management Frame Protection Capable (MFPC)
 *                       1: Management Frame Protection Capable
 *                       0: Management Frame Protection not Capable
 * \param[in/out] mfpr: Management Frame Protection Required (MFPR)
 *                       1: Management Frame Protection Required
 *                       0: Management Frame Protection Optional
 *
 * @return cmd response status
 */
int wifi_uap_pmf_getset(uint8_t action, uint8_t *mfpc, uint8_t *mfpr);

/**
 * enable/disable 80211d domain feature for the uAP.
 *
 * @note This API only set 80211d domain feature.
 * The actual application will happen only during
 * starting phase of uAP. So, if the uAP is already started then the
 * configuration will not apply till uAP re-start.
 *
 * @return WM_SUCCESS on success or error code.
 *
 */
int wifi_uap_enable_11d_support();
bool wifi_11d_is_channel_allowed(int channel);
wifi_sub_band_set_t *get_sub_band_from_region_code(int region_code, t_u8 *nr_sb);
#ifdef CONFIG_5GHz_SUPPORT
wifi_sub_band_set_t *get_sub_band_from_region_code_5ghz(int region_code, t_u8 *nr_sb);
#endif

int wifi_enable_11d_support();
int wifi_enable_uap_11d_support();
int wifi_disable_11d_support();
int wifi_disable_uap_11d_support();

#ifdef OTP_CHANINFO
int wifi_get_fw_region_and_cfp_tables(void);
void wifi_free_fw_region_and_cfp_tables(void);
#endif
int wifi_set_txbfcap(unsigned int tx_bf_cap);
int wifi_set_htcapinfo(unsigned int htcapinfo);
int wifi_set_httxcfg(unsigned short httxcfg);
void wifi_uap_set_httxcfg(const t_u16 ht_tx_cfg);
int wifi_uap_set_httxcfg_int(unsigned short httxcfg);
int wifi_get_tx_power(t_u32 *power_level);
int wifi_set_tx_power(t_u32 power_level);
int wrapper_wlan_cmd_get_hw_spec(void);
/* fixme: These need to be removed later after complete mlan integration */
void set_event_chanswann(void);
void clear_event_chanswann(void);
int wifi_send_hs_cfg_cmd(mlan_bss_type interface, t_u32 ipv4_addr, t_u16 action, t_u32 conditions);
#ifdef CONFIG_HOST_SLEEP
int wifi_cancel_host_sleep(mlan_bss_type interface);
#endif
bool wrapper_wlan_11d_support_is_enabled(void);
void wrapper_wlan_11d_clear_parsedtable(void);
void wrapper_clear_media_connected_event(void);
int wifi_uap_ps_inactivity_sleep_exit(mlan_bss_type type);
int wifi_uap_ps_inactivity_sleep_enter(mlan_bss_type type,
                                       unsigned int ctrl_bitmap,
                                       unsigned int min_sleep,
                                       unsigned int max_sleep,
                                       unsigned int inactivity_to,
                                       unsigned int min_awake,
                                       unsigned int max_awake);
int wifi_enter_ieee_power_save(void);
int wifi_exit_ieee_power_save(void);
int wifi_enter_deepsleep_power_save(void);
int wifi_exit_deepsleep_power_save(void);
void send_sleep_confirm_command(mlan_bss_type interface);
void wifi_configure_listen_interval(int listen_interval);
void wifi_configure_null_pkt_interval(unsigned int null_pkt_interval);
int wrapper_wifi_assoc(
    const unsigned char *bssid, int wlan_security, bool is_wpa_tkip, unsigned int owe_trans_mode, bool is_ft);
#ifdef CONFIG_WIFI_UAP_WORKAROUND_STICKY_TIM
void wifi_uap_enable_sticky_bit(const uint8_t *mac_addr);
#endif /* CONFIG_WIFI_UAP_WORKAROUND_STICKY_TIM */
bool wifi_get_xfer_pending(void);
void wifi_set_xfer_pending(bool xfer_val);
int wrapper_wlan_cmd_11n_ba_stream_timeout(void *saved_event_buff);

int wifi_set_txratecfg(wifi_ds_rate ds_rate, mlan_bss_type bss_type);
int wifi_get_txratecfg(wifi_ds_rate *ds_rate, mlan_bss_type bss_type);
void wifi_wake_up_card(uint32_t *resp);



int wrapper_wlan_11d_enable(t_u32 state);
int wrapper_wlan_uap_11d_enable(t_u32 state);

int wifi_11h_enable(void);

int wrapper_wlan_cmd_11n_addba_rspgen(void *saved_event_buff);

int wrapper_wlan_cmd_11n_delba_rspgen(void *saved_event_buff);

int wrapper_wlan_ecsa_enable(void);

int wifi_uap_start(mlan_bss_type type,
                   char *ssid,
                   uint8_t *mac_addr,
                   int security,
                   char *passphrase,
                   char *password,
                   int channel,
                   wifi_scan_chan_list_t scan_chan_list,
                   uint8_t pwe_derivation,
                   uint8_t transition_disable,
                   bool mfpc,
                   bool mfpr
);

int wrapper_wlan_sta_ampdu_enable(
#ifdef CONFIG_WMM
    t_u8 tid
#endif
);

int wrapper_wlan_uap_ampdu_enable(uint8_t *addr
#ifdef CONFIG_WMM
                                  ,
                                  t_u8 tid
#endif
);



int wifi_set_packet_filters(wifi_flt_cfg_t *flt_cfg);

int wifi_uap_stop();
#ifdef CONFIG_WPA_SUPP_AP
int wifi_uap_do_acs(const int *freq_list);
#endif

#ifdef CONFIG_WIFI_CAPA
/**
 * Set uAP capability
 *
 * User can set uAP capability of 11ax/11ac/11n/legacy. Default is 11ax.
 *
 * @param[in] wlan_capa uAP capability bitmap.
 *                      1111 - 11AX
 *                      0111 - 11AC
 *                      0011 - 11N
 *                      0001 - legacy
 *
 * @return void
 */
void wifi_uap_config_wifi_capa(uint8_t wlan_capa);
void wifi_get_fw_info(mlan_bss_type type, t_u16 *fw_bands);
#endif
int wifi_get_data_rate(wifi_ds_rate *ds_rate, mlan_bss_type bss_type);

int wifi_uap_set_bandwidth(const t_u8 bandwidth);

int wifi_uap_get_pmfcfg(t_u8 *mfpc, t_u8 *mfpr);




#ifdef CONFIG_11R
bool wifi_same_ess_ft();
#endif


#ifdef CONFIG_11K
#define BEACON_REPORT_BUF_SIZE 1400

/* Reporting Detail values */
enum wlan_rrm_beacon_reporting_detail
{
    WLAN_RRM_REPORTING_DETAIL_NONE                    = 0,
    WLAN_RRM_REPORTING_DETAIL_AS_REQUEST              = 1,
    WLAN_RRM_REPORTING_DETAIL_ALL_FIELDS_AND_ELEMENTS = 2,
};

typedef struct _wlan_rrm_beacon_report_data
{
    t_u8 token;
    t_u8 ssid[MLAN_MAX_SSID_LENGTH];
    t_u8 ssid_length;
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    t_u8 channel[MAX_CHANNEL_LIST];
    t_u8 channel_num;
    t_u8 last_ind;
    t_u16 duration;
    enum wlan_rrm_beacon_reporting_detail report_detail;
    t_u8 bits_field[32];
} wlan_rrm_beacon_report_data;

typedef struct _wlan_rrm_scan_cb_param
{
    wlan_rrm_beacon_report_data rep_data;
    t_u8 dialog_tok;
    t_u8 dst_addr[MLAN_MAC_ADDR_LENGTH];
    t_u8 protect;
} wlan_rrm_scan_cb_param;

int wifi_host_11k_cfg(int enable_11k);

/**
 * host send neighbor report request
 * \param[in] ssid ssid for neighbor report
 */
int wifi_host_11k_neighbor_req(t_u8 *ssid);
#endif

#ifdef CONFIG_11V
/**
 * host send bss transition management query
 */
int wifi_host_11v_bss_trans_query(t_u8 query_reason);
#endif

#if defined(CONFIG_11K) || defined(CONFIG_11V)
/* Neighbor List Mode values */
enum wlan_nlist_mode
{
#if defined(CONFIG_11K)
    WLAN_NLIST_11K = 1,
#endif
#if defined(CONFIG_11V)
    WLAN_NLIST_11V           = 2,
    WLAN_NLIST_11V_PREFERRED = 3,
#endif
};

#define MAX_NEIGHBOR_AP_LIMIT 6U

typedef struct _wlan_rrm_neighbor_ap_t
{
    char ssid[MLAN_MAX_SSID_LENGTH];
    t_u8 bssid[MLAN_MAX_SSID_LENGTH];
    t_u8 bssidInfo[32];
    int op_class;
    int channel;
    int phy_type;
    int freq;
} wlan_rrm_neighbor_ap_t;

typedef struct _wlan_neighbor_report_t
{
    wlan_rrm_neighbor_ap_t neighbor_ap[MAX_NEIGHBOR_AP_LIMIT];
    int neighbor_cnt;
} wlan_rrm_neighbor_report_t;

typedef struct _wlan_nlist_report_param
{
    enum wlan_nlist_mode nlist_mode;
    t_u8 num_channels;
    t_u8 channels[MAX_NUM_CHANS_IN_NBOR_RPT];
#if defined(CONFIG_11V)
    t_u8 btm_mode;
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    t_u8 dialog_token;
    t_u8 dst_addr[MLAN_MAC_ADDR_LENGTH];
    t_u8 protect;
#endif
} wlan_nlist_report_param;
#endif

int wifi_clear_mgmt_ie(mlan_bss_type bss_type, IEEEtypes_ElementId_t index, int mgmt_bitmap_index);


int wifi_set_auto_arp(t_u32 *ipv4_addr);

int wifi_tcp_keep_alive(wifi_tcp_keep_alive_t *keep_alive, t_u8 *src_mac, t_u32 src_ip);


#ifdef CONFIG_CLOUD_KEEP_ALIVE
int wifi_cloud_keep_alive(wifi_cloud_keep_alive_t *keep_alive, t_u16 action, t_u8 *enable);
#endif

#ifdef CONFIG_HOST_SLEEP
int wifi_set_packet_filters(wifi_flt_cfg_t *flt_cfg);
int wakelock_get(void);
int wakelock_put(void);
int wakelock_isheld(void);
#endif

int wifi_raw_packet_send(const t_u8 *packet, t_u32 length);

int wifi_raw_packet_recv(t_u8 **data, t_u32 *pkt_type);


#ifdef CONFIG_WIFI_CLOCKSYNC
int wifi_set_clocksync_cfg(const wifi_clock_sync_gpio_tsf_t *tsf_latch, mlan_bss_type bss_type);
int wifi_get_tsf_info(wifi_tsf_info_t *tsf_info);
#endif /* CONFIG_WIFI_CLOCKSYNC */

#ifdef CONFIG_RF_TEST_MODE

int wifi_set_rf_test_mode(void);

int wifi_set_rf_channel(const uint8_t channel);

int wifi_set_rf_radio_mode(const uint8_t mode);

int wifi_get_rf_channel(uint8_t *channel);

int wifi_get_rf_radio_mode(uint8_t *mode);

int wifi_set_rf_band(const uint8_t band);

int wifi_get_rf_band(uint8_t *band);

int wifi_set_rf_bandwidth(const uint8_t bandwidth);

int wifi_get_rf_bandwidth(uint8_t *bandwidth);

int wifi_get_rf_per(uint32_t *rx_tot_pkt_count, uint32_t *rx_mcast_bcast_count, uint32_t *rx_pkt_fcs_error);

int wifi_set_rf_tx_cont_mode(const uint32_t enable_tx,
                             const uint32_t cw_mode,
                             const uint32_t payload_pattern,
                             const uint32_t cs_mode,
                             const uint32_t act_sub_ch,
                             const uint32_t tx_rate);

int wifi_set_rf_tx_antenna(const uint8_t antenna);

int wifi_get_rf_tx_antenna(uint8_t *antenna);

int wifi_set_rf_rx_antenna(const uint8_t antenna);

int wifi_get_rf_rx_antenna(uint8_t *antenna);

int wifi_set_rf_tx_power(const uint32_t power, const uint8_t mod, const uint8_t path_id);

int wifi_cfg_rf_he_tb_tx(uint16_t enable, uint16_t qnum, uint16_t aid, uint16_t axq_mu_timer, int16_t tx_power);

int wifi_rf_trigger_frame_cfg(uint32_t Enable_tx,
                              uint32_t Standalone_hetb,
                              uint8_t FRAME_CTRL_TYPE,
                              uint8_t FRAME_CTRL_SUBTYPE,
                              uint16_t FRAME_DURATION,
                              uint64_t TriggerType,
                              uint64_t UlLen,
                              uint64_t MoreTF,
                              uint64_t CSRequired,
                              uint64_t UlBw,
                              uint64_t LTFType,
                              uint64_t LTFMode,
                              uint64_t LTFSymbol,
                              uint64_t UlSTBC,
                              uint64_t LdpcESS,
                              uint64_t ApTxPwr,
                              uint64_t PreFecPadFct,
                              uint64_t PeDisambig,
                              uint64_t SpatialReuse,
                              uint64_t Doppler,
                              uint64_t HeSig2,
                              uint32_t AID12,
                              uint32_t RUAllocReg,
                              uint32_t RUAlloc,
                              uint32_t UlCodingType,
                              uint32_t UlMCS,
                              uint32_t UlDCM,
                              uint32_t SSAlloc,
                              uint8_t UlTargetRSSI,
                              uint8_t MPDU_MU_SF,
                              uint8_t TID_AL,
                              uint8_t AC_PL,
                              uint8_t Pref_AC);

int wifi_set_rf_tx_frame(const uint32_t enable,
                         const uint32_t data_rate,
                         const uint32_t frame_pattern,
                         const uint32_t frame_length,
                         const uint16_t adjust_burst_sifs,
                         const uint32_t burst_sifs_in_us,
                         const uint32_t short_preamble,
                         const uint32_t act_sub_ch,
                         const uint32_t short_gi,
                         const uint32_t adv_coding,
                         const uint32_t tx_bf,
                         const uint32_t gf_mode,
                         const uint32_t stbc,
                         const uint8_t *bssid);
#endif
#ifdef CONFIG_WIFI_FW_DEBUG
/** This function registers callbacks which are used to generate FW Dump on USB
 * device.
 *
 * \param[in] wifi_usb_mount_cb Callback to mount usb device.
 * \param[in] wifi_usb_file_open_cb Callback to open file on usb device for FW dump.
 * \param[in] wifi_usb_file_write_cb Callback to write FW dump data to opened file.
 * \param[in] wifi_usb_file_close_cb Callback to close FW dump file.
 *
 * \return void
 */
void wifi_register_fw_dump_cb(int (*wifi_usb_mount_cb)(),
                              int (*wifi_usb_file_open_cb)(char *test_file_name),
                              int (*wifi_usb_file_write_cb)(uint8_t *data, size_t data_len),
                              int (*wifi_usb_file_close_cb)());
#endif

#ifdef CONFIG_WMM
void wifi_wmm_init();
t_u32 wifi_wmm_get_pkt_prio(t_u8 *buf, t_u8 *tid);
t_u8 wifi_wmm_get_packet_cnt(void);
/* handle EVENT_TX_DATA_PAUSE */
void wifi_handle_event_data_pause(void *data);
void wifi_wmm_tx_stats_dump(int bss_type);
#endif /* CONFIG_WMM */

int wifi_set_rssi_low_threshold(uint8_t *low_rssi);

#ifdef CONFIG_HEAP_DEBUG
/**
 * Show os mem alloc and free info.
 *
 * \return void.
 */
void wifi_show_os_mem_stat();
#endif


/**
 *Frame Tx - Injecting Wireless frames from Host
 *
 * This function is used to Inject Wireless frames from application
 * directly.
 *
 * \note All injected frames will be sent on station interface. Application
 * needs minimum of 2 KBytes stack for successful operation.
 * Also application have to take care of allocating buffer for 802.11 Wireless
 * frame (Header + Data) and freeing allocated buffer. Also this
 * API may not work when Power Save is enabled on station interface.
 *
 * \param[in] bss_type The interface on which management frame needs to be send.
 * \param[in] buff Buffer holding 802.11 Wireless frame (Header + Data).
 * \param[in] len Length of the 802.11 Wireless frame.
 *
 * \return WM_SUCCESS on success or error code.
 *
 **/

int wifi_inject_frame(const enum wlan_bss_type bss_type, const uint8_t *buff, const size_t len);

int wifi_supp_inject_frame(const enum wlan_bss_type bss_type, const uint8_t *buff, const size_t len);
#ifdef CONFIG_WPA_SUPP
t_u8 wifi_get_sec_channel_offset(unsigned int chan);
int wifi_nxp_scan_res_get(void);
int wifi_nxp_survey_res_get(void);
int wifi_nxp_set_default_scan_ies(const u8 *ies, size_t ies_len);
void wifi_nxp_reset_scan_flag();
#endif

#ifdef CONFIG_MBO
int wifi_host_mbo_cfg(int enable_mbo);
int wifi_mbo_preferch_cfg(t_u8 ch0, t_u8 pefer0, t_u8 ch1, t_u8 pefer1);
int wifi_mbo_send_preferch_wnm(t_u8 *src_addr, t_u8 *target_bssid, t_u8 ch0, t_u8 pefer0, t_u8 ch1, t_u8 pefer1);
#endif


#if defined(CONFIG_WMM_UAPSD) || defined(CONFIG_WMM)
int wifi_wmm_qos_cfg(t_u8 *qos_cfg, t_u8 action);
void wifi_sleep_period(unsigned int *sleep_period, int action);
#endif




int wifi_send_mgmt_auth_request(const t_u8 channel,
                                const t_u8 auth_alg,
                                const t_u8 *auth_seq_num,
                                const t_u8 *status_code,
                                const t_u8 *dest,
                                const t_u8 *sae_data,
                                const t_u16 sae_data_len);
int wifi_send_scan_cmd(t_u8 bss_mode,
                       const t_u8 *specific_bssid,
                       const char *ssid,
                       const char *ssid2,
                       const t_u8 num_channels,
                       const wifi_scan_channel_list_t *chan_list,
                       const t_u8 num_probes,
                       const t_u16 scan_chan_gap,
                       const bool keep_previous_scan,
                       const bool active_scan_triggered);
int wifi_deauthenticate(uint8_t *bssid);

#ifdef CONFIG_TURBO_MODE
int wifi_get_turbo_mode(t_u8 *mode);
int wifi_get_uap_turbo_mode(t_u8 *mode);
int wifi_set_turbo_mode(t_u8 mode);
int wifi_set_uap_turbo_mode(t_u8 mode);
#endif

#ifdef CONFIG_WPA_SUPP_AP
t_u16 wifi_get_default_ht_capab();
t_u32 wifi_get_default_vht_capab();

void wifi_uap_client_assoc(t_u8 *sta_addr, unsigned char is_11n_enabled);
void wifi_uap_client_deauth(t_u8 *sta_addr);

#endif

#endif /* __WIFI_H__ */
