/** @file wifi-internal.h
 *
 *  @brief WLAN Internal API
 *
 *  Copyright 2008-2021 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

#ifndef __WIFI_INTERNAL_H__
#define __WIFI_INTERNAL_H__

#include <mlan_api.h>

#include <wm_os.h>
#include <wifi_events.h>
#include <wifi-decl.h>

typedef struct
{
    int (*wifi_uap_set_params_p)(int channel);
    int (*wifi_uap_downld_domain_params_p)(MrvlIEtypes_DomainParamSet_t *dp);
    int (*wifi_uap_enable_11d_p)(void);
} wifi_uap_11d_apis_t;

typedef struct mcast_filter
{
    uint8_t mac_addr[MLAN_MAC_ADDR_LENGTH];
    struct mcast_filter *next;
} mcast_filter;

/* User response buffer parameters for hostcmd */
typedef struct _hostcmd_cfg
{
    void *resp_buf;
    uint32_t resp_buf_len;
    uint32_t *reqd_resp_len;
    bool is_hostcmd;
} hostcmd_cfg_t;

typedef struct
{
    os_thread_t wm_wifi_main_thread;
    os_thread_t wm_wifi_core_thread;
    os_thread_t wm_wifi_scan_thread;
#ifdef CONFIG_WMM
    /** Thread handle for sending data */
    os_thread_t wm_wifi_driver_tx;
#endif
    os_queue_t *wlc_mgr_event_queue;

    void (*data_intput_callback)(const uint8_t interface, const uint8_t *buffer, const uint16_t len);
    void (*amsdu_data_intput_callback)(uint8_t interface, uint8_t *buffer, uint16_t len);
    void (*deliver_packet_above_callback)(t_u8 interface, t_void *lwip_pbuf);
    bool (*wrapper_net_is_ip_or_ipv6_callback)(const t_u8 *buffer);

    os_mutex_t command_lock;
    os_semaphore_t command_resp_sem;
    os_mutex_t mcastf_mutex;
#ifdef CONFIG_WMM
    /** Semaphore to protect data parameters */
    os_semaphore_t tx_data_sem;
#endif
    unsigned last_sent_cmd_msec;

    /* Queue for events/data from low level interface driver */
    os_queue_t io_events;
#ifdef CONFIG_WMM
    /** Queue for sending data packets to fw */
    os_queue_t tx_data;
    os_queue_pool_t tx_data_queue_data;
#endif
    os_queue_pool_t io_events_queue_data;

    mcast_filter *start_list;

    /*
     * Usage note:
     * There are a number of API's (for e.g. wifi_get_antenna()) which
     * return some data in the buffer passed by the caller. Most of the
     * time this data needs to be retrived from the firmware. This
     * retrival happens in a different thread context. Hence, we need
     * to store the buffer pointer passed by the user at a shared
     * location. This pointer to used for this purpose.
     *
     * Note to the developer: Please ensure to set this to NULL after
     * use in the wifi driver thread context.
     */
    void *cmd_resp_priv;
    void *cmd_resp_ioctl;
    /*
     * In continuation with the description written for the
     * cmd_resp_priv member above, the below member indicates the
     * result of the retrieval operation from the firmware.
     */
    int cmd_resp_status;

    /*
     * Store 11D support status in Wi-Fi driver.
     */
    bool enable_11d_support;
    wifi_uap_11d_apis_t *uap_support_11d_apis;
    /*
     * This is updated when user calls the wifi_uap_set_domain_params()
     * functions. This is used later during uAP startup. Since the uAP
     * configuration needs to be done befor uAP is started we keep this
     * cache. This is needed to enable 11d support in uAP.
     */
    MrvlIEtypes_DomainParamSet_t *dp;
    /** Broadcast ssid control */
    bool bcast_ssid_ctl;
    /** beacon period */
    t_u16 beacon_period;
    /** Wi-Fi Bandwidth */
    t_u8 bandwidth;
    /** channel switch time to send ECSA */
    t_u8 chan_sw_count;
    /** Sniffer channel number */
    t_u8 chan_num;
    /** HT Capability Info */
    t_u16 ht_cap_info;
    /** HTTX Cfg */
    t_u16 ht_tx_cfg;
#ifdef CONFIG_WMM
    /** Outbuf index */
    t_u8 pkt_index[MAX_AC_QUEUES];
    /** packet count */
    t_u8 pkt_cnt[MAX_AC_QUEUES];
    /** send packet index */
    t_u8 send_index[MAX_AC_QUEUES];
    /** WMM queues block lengths */
    t_u32 bk_pkt_len[BK_MAX_BUF];
    t_u32 be_pkt_len[BE_MAX_BUF];
    t_u32 vi_pkt_len[VI_MAX_BUF];
    t_u32 vo_pkt_len[VO_MAX_BUF];
#endif
#ifdef CONFIG_WIFI_FW_DEBUG
    /** This function mount USB device.
     *
     * return WM_SUCCESS on success
     * return -WM_FAIL on failure.
     */
    int (*wifi_usb_mount_cb)();
    /** This function will open file for writing FW dump.
     *
     * \param[in] test_file_name Name of file to write FW dump data.
     *
     * \return WM_SUCCESS if opening of file is successful.
     * \return -WM_FAIL in case of failure.
     */
    int (*wifi_usb_file_open_cb)(char *test_file_name);
    /** This function will write data to file opened using wifi_usb_file_open_cb()
     *
     * \param[in] data Buffer containing FW dump data.
     * \param[in] data_len Length of data that needs to be written.
     *
     * \return WM_SUCCESS if write is successful
     * \return -WM_FAIL in case of failure.
     */
    int (*wifi_usb_file_write_cb)(uint8_t *data, size_t data_len);
    /** This function will close the file on which FW dump is written.
     *
     * \note This will close file that is opened using wifi_usb_file_open_cb().
     *
     * \return WM_SUCCESS on success.
     * \return -WM_FAIL on failure.
     */
    int (*wifi_usb_file_close_cb)();
#endif
    /** Structure to store the response from WiFi firmware in
     * response buffer provided by application layers
     * structure also stores lengths for usage and validation internally*/
    hostcmd_cfg_t hostcmd_cfg;
    wlan_user_scan_cfg *g_user_scan_cfg;

    bool scan_stop;
} wm_wifi_t;

extern wm_wifi_t wm_wifi;
extern bool split_scan_in_progress;

struct bus_message
{
    uint16_t event;
    uint16_t reason;
    void *data;
};

PACK_START struct ieee80211_hdr
{
    t_u16 frame_control;
    t_u16 duration_id;
    t_u8 addr1[6];
    t_u8 addr2[6];
    t_u8 addr3[6];
    t_u16 seq_ctrl;
    t_u8 addr4[6];
} PACK_END;

/**
 * This function handles events received from the firmware.
 */
int wifi_handle_fw_event(struct bus_message *msg);

/**
 * This function is used to send events to the upper layer through the
 * message queue registered by the upper layer.
 */
int wifi_event_completion(enum wifi_event event, enum wifi_event_reason result, void *data);

/**
 * Use this function to know whether a split scan is in progress.
 */
bool is_split_scan_complete(void);

/**
 * Waits for Command processing to complete and waits for command response
 */
int wifi_wait_for_cmdresp(void *cmd_resp_priv);

/**
 * Register an event queue
 *
 * This queue is used to send events and command responses to the wifi
 * driver from the stack dispatcher thread.
 */
int bus_register_event_queue(os_queue_t *event_queue);

/**
 * De-register the event queue.
 */
void bus_deregister_event_queue(void);

/**
 * Register DATA input function with SDIO driver.
 *
 * This queue is used to DATA frames to the wifi
 * driver from the stack dispatcher thread.
 */
int bus_register_data_input_function(int (*wifi_low_level_input)(const uint8_t interface,
                                                                 const uint8_t *buffer,
                                                                 const uint16_t len));

/**
 * De-register the DATA input function with SDIO driver.
 */
void bus_deregister_data_input_funtion(void);

/*
 * @internal
 *
 *
 */
int wifi_get_command_lock(void);

/*
 * @internal
 *
 *
 */
int wifi_put_command_lock(void);

/*
 * Process the command reponse received from the firmware.
 *
 * Change the type of param below to HostCmd_DS_COMMAND after mlan
 * integration complete and then move it to header file.
 */
int wifi_process_cmd_response(HostCmd_DS_COMMAND *resp);

/*
 * @internal
 *
 *
 */
void *wifi_mem_malloc_cmdrespbuf(void);

/*
 * @internal
 *
 *
 */
void *wifi_malloc_eventbuf(size_t size);
void wifi_free_eventbuf(void *buffer);

void wifi_uap_handle_cmd_resp(HostCmd_DS_COMMAND *resp);

mlan_status wrapper_moal_malloc(t_void *pmoal_handle, t_u32 size, t_u32 flag, t_u8 **ppbuf);
mlan_status wrapper_moal_mfree(t_void *pmoal_handle, t_u8 *pbuf);

#if defined(RW610)
int wifi_imu_lock(void);
void wifi_imu_unlock(void);
#else
int wifi_sdio_lock(void);
void wifi_sdio_unlock(void);
#endif

mlan_status wrapper_wlan_cmd_mgmt_ie(int bss_type, void *buffer, unsigned int len, t_u16 action);

/**
 * This function should be called when user scan is
 * finished with success/failure.
 *
 */
void wifi_user_scan_config_cleanup(void);

/**
 * This function should be called to wait for scan task done before resetting.
 *
 */
void wifi_scan_stop(void);
#endif /* __WIFI_INTERNAL_H__ */
