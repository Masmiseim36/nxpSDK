/** @file mlan_glue.c
 *
 *  @brief This file acts as a glue between legacy wlan code and mlan based wlan
 *  code
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <wm_os.h>
#include <wifi_events.h>
#include <wifi-debug.h>

#include <wifi.h>
#include "wifi-internal.h"
#if defined(RW610)
#include "wifi-imu.h"
#else
#include "wifi-sdio.h"
#endif

#ifdef CONFIG_WIFI_FW_DEBUG
#define FW_DEBUG_INFO_SIZE 400
#endif

static const uint8_t wpa2_akmp_oui[4] = {0x00, 0x0f, 0xac, 0x01};
#ifdef WLAN_LOW_POWER_ENABLE
bool low_power_mode;
#endif
bool cal_data_valid;
static uint8_t *cal_data;
static uint32_t cal_data_len;

bool mac_addr_valid;
static uint8_t *mac_addr;

/* fixme: This structure seems to have been removed from mlan. This was
   copied from userif_ext.h file temporarily. Change the handling of events to
   make it similar to what mlan does */

/** Event structure */
typedef MLAN_PACK_START struct _Event_Ext_t
{
    /** No of bytes in packet including this field */
    uint16_t length;
    /** Type: Event (3) */
    uint16_t type;
    /** Event ID */
    uint16_t event_id;
    /** BSS index number for multiple BSS support */
    uint8_t bss_index;
    /** BSS type */
    uint8_t bss_type;
    /** Reason code */
    uint16_t reason_code;
    /** Source MAC address */
    uint8_t src_mac_addr[MLAN_MAC_ADDR_LENGTH];
} MLAN_PACK_END Event_Ext_t;


/*
 *  fixme: This entry is not present in mlan_fw.h. It is likely possible
 *  that this code is no-op in the firmware. Keeping it as it is for
 *  maintaining backward compatibility. Can be removed later.
 */
#define host_CMD_802_11_AUTHENTICATE 0x0011

/* Following is allocated in mlan_register */
mlan_adapter *mlan_adap;

static mlan_device mlan_dev;

uint8_t g_rssi;
int16_t g_bcn_nf_last;

/* fixme: This global variable is needed
 *  to save the correct event since SLP_CFRM command
 *  returns incorrect bitmap. This will be  further investigated
 *  and if possile global variable will be removed.*/
static t_u16 ps_event;

int mlan_subsys_init(void);
int mlan_subsys_deinit(void);

void wifi_get_mac_address_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint8_t *get_mac_addr);
void wifi_get_value1_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint32_t *dev_value1);
void wifi_get_firmware_ver_ext_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint8_t *fw_ver_ext);
int wifi_set_tx_power_ext(uint32_t len, uint32_t *power_data);
int wifi_send_bss_ioctl(mlan_ds_bss *bss);

void wifi_prepare_get_fw_ver_ext_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number, int version_str_sel);
void wifi_prepare_get_value1(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_enable_amsdu_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_get_mac_addr_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_get_hw_spec_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_set_mac_addr_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_set_cal_data_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_reconfigure_tx_buf_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wlan_prepare_mac_control_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);

#ifdef OTP_CHANINFO
void wifi_prepare_get_channel_region_cfg_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
#endif

void wrapper_wlan_cmd_11n_cfg(HostCmd_DS_COMMAND *cmd);
void wrapper_deliver_amsdu_subframe(pmlan_buffer amsdu_pmbuf, t_u8 *data, t_u16 pkt_len);
int wrapper_wlan_set_regiontable(t_u8 region, t_u16 band);
int wrapper_wlan_handle_rx_packet(t_u16 datalen, RxPD *rxpd, void *p, void *payload);
int wrapper_get_wpa_ie_in_assoc(uint8_t *wpa_ie);

int wrapper_wlan_handle_amsdu_rx_packet(const t_u8 *rcvdata, const t_u16 datalen);
int wrapper_bssdesc_second_set(int bss_index,
                               bool *phtcap_ie_present,
                               bool *phtinfo_ie_present,
                               bool *wmm_ie_present,
                               uint16_t *band,
                               bool *wps_IE_exist,
                               uint16_t *wps_session,
                               bool *wpa2_entp_IE_exist,
                               uint8_t *trans_mode,
                               uint8_t *trans_bssid,
                               int *trans_ssid_len,
                               uint8_t *trans_ssid);

int wrapper_bssdesc_first_set(int bss_index,
                              uint8_t *BssId,
                              bool *is_ibss_bit_set,
                              int *ssid_len,
                              uint8_t *ssid,
                              uint8_t *Channel,
                              uint8_t *RSSI,
                              uint16_t *beacon_period,
                              uint16_t *dtim_period,
                              _SecurityMode_t *WPA_WPA2_WEP,
                              _Cipher_t *wpa_mcstCipher,
                              _Cipher_t *wpa_ucstCipher,
                              _Cipher_t *rsn_mcstCipher,
                              _Cipher_t *rsn_ucstCipher,
                              bool *is_pmf_required);

// track whether ampdu is enabled
static bool ampdu_status_flag = MFALSE;

/*
 * The command event received from the firmware (e.g. EVENT_ADDBA) cannot
 * be processed immediately. We need to do it the WLC manager context. For
 * that we will save the buffer lest some other event destroy it. WLC
 * manager will be notified of this event.
 *
 * Returns NULL if buffer allocation failed.
 */
static void *wifi_11n_save_request(Event_Ext_t *evt)
{
    void *dot11n_eventbuf = os_mem_alloc(evt->length);

    if (dot11n_eventbuf == MNULL)
    {
        wifi_w("11n eventbuf alloc failed %d", evt->length);
        return NULL;
    }

    return memcpy((void *)dot11n_eventbuf, (const void *)evt, evt->length);
}


void wrapper_deliver_amsdu_subframe(pmlan_buffer amsdu_pmbuf, t_u8 *data, t_u16 pkt_len)
{
    RxPD *prx_pd = (RxPD *)(void *)amsdu_pmbuf->pbuf;
    w_pkt_d("[amsdu] [push]: BSS Type: %d L: %d", prx_pd->bss_type, pkt_len);
    wm_wifi.amsdu_data_intput_callback(prx_pd->bss_type, data, pkt_len);
}

static mlan_status wrapper_moal_recv_packet(IN t_void *pmoal_handle, IN pmlan_buffer pmbuf)
{
    RxPD *prx_pd = (RxPD *)(void *)(pmbuf->pbuf + pmbuf->data_offset);
    if (pmbuf->data_offset != 0U)
    {
        wifi_e("pmbuf->data_offset != 0 (%d)?", pmbuf->data_offset);
        while (true)
        {
            ;
        }
    }

    w_pkt_d("[push] %p + %d:  DELV: %p L: %d", pmbuf->pbuf, pmbuf->data_offset, pmbuf->lwip_pbuf, pmbuf->data_len);

    wm_wifi.deliver_packet_above_callback(prx_pd->bss_type, pmbuf->lwip_pbuf);

    /* Free RxPD */
    os_mem_free(pmbuf->pbuf);
    os_mem_free(pmbuf);
    return MLAN_STATUS_SUCCESS;
}

/** moal_init_timer*/
static mlan_status wrapper_moal_init_timer(IN t_void *pmoal_handle,
                                           OUT t_void **pptimer,
                                           IN t_void (*callback)(os_timer_arg_t arg),
                                           IN t_void *pcontext)
{
    w_tmr_d("Creating timer");

    /*
     * As can be seen from the arguments to our function, we do not
     * have a timer period parameter. Nor do we have a parameter to
     * specify whether the user wants a periodic or a one-shot
     * timer. We have to wait for these paramters till the next call to
     * wrapper_moal_start_timer.
     *
     * But a timer object cannot be created in
     * wrapper_moal_start_timer. Because the timer handle is passed by
     * value.
     *
     * Considering all this we will need to create the timer object in the
     * current function. We will provide a dummy time here one and
     * change in later. One assumption from the current observation is
     * that only one shot timers are created. So we will assume this
     * and raise assert in *timer_start if this is not the case.
     */
#define DUMMY_TIME_PARAM 10

    int rv = os_timer_create((os_timer_t *)pptimer, NULL, DUMMY_TIME_PARAM, callback, pcontext, OS_TIMER_ONE_SHOT,
                             OS_TIMER_NO_ACTIVATE);
    if (rv != WM_SUCCESS)
    {
        w_tmr_e("Unable to create timer");
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

/** moal_start_timer*/
static mlan_status wrapper_moal_start_timer(IN t_void *pmoal_handle, IN t_void *ptimer, IN bool periodic, IN t_u32 msec)
{
    w_tmr_d("Starting timer: %p for %d mS", ptimer, msec);

    if (periodic == MTRUE)
    {
        /* Check note in wrapper_moal_init_timer for details */
        w_tmr_w("Periodic is not supported temporarily");
        while (true)
        {
            ;
        }
        // wmpanic();
    }

    int rv = os_timer_change((os_timer_t *)&ptimer, os_msec_to_ticks(msec), 0);
    if (rv != WM_SUCCESS)
    {
        w_tmr_e("Unable to change period in timer");
        return MLAN_STATUS_FAILURE;
    }

    rv = os_timer_activate((os_timer_t *)&ptimer);
    if (rv != WM_SUCCESS)
    {
        w_tmr_e("Unable to start timer");
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

/** moal_stop_timer*/
static mlan_status wrapper_moal_stop_timer(IN t_void *pmoal_handle, IN t_void *ptimer)
{
    w_tmr_d("Stopping timer: %p", ptimer);

    int rv = os_timer_deactivate((os_timer_t *)&ptimer);
    if (rv != WM_SUCCESS)
    {
        w_tmr_e("Unable to stop timer.");
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

/** moal_free_timer */
static mlan_status wrapper_moal_free_timer(IN t_void *pmoal_handle, IN t_void **pptimer)
{
    w_tmr_d("Deleting timer: %p", *((uint32_t *)pptimer));

    int rv = os_timer_delete((os_timer_t *)pptimer);
    if (rv != WM_SUCCESS)
    {
        w_tmr_e("Unable to delete timer.");
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

static mlan_status wrapper_moal_init_lock(IN t_void *pmoal_handle, OUT t_void **pplock)
{
#ifdef DEBUG_DEVELOPMENT
    wmprintf("%s: Locks not needed for now\n\r", __func__);
#endif /* DEBUG_DEVELOPMENT */
    return MLAN_STATUS_SUCCESS;
}

/** moal_free_lock */
static mlan_status wrapper_moal_free_lock(IN t_void *pmoal_handle, IN t_void *plock)
{
#ifdef DEBUG_DEVELOPMENT
    wmprintf("%s: Locks not needed for now\n\r", __func__);
#endif /* DEBUG_DEVELOPMENT */
    return MLAN_STATUS_SUCCESS;
}

/** moal_spin_lock */
static mlan_status wrapper_moal_spin_lock(IN t_void *pmoal_handle, IN t_void *plock)
{
#ifdef DEBUG_DEVELOPMENT
    wmprintf("%s: Locks not needed for now\n\r", __func__);
#endif /* DEBUG_DEVELOPMENT */
    return MLAN_STATUS_SUCCESS;
}

/** moal_spin_unlock */
static mlan_status wrapper_moal_spin_unlock(IN t_void *pmoal_handle, IN t_void *plock)
{
#ifdef DEBUG_DEVELOPMENT
    wmprintf("%s: Locks not needed for now\n\r", __func__);
#endif /* DEBUG_DEVELOPMENT */
    return MLAN_STATUS_SUCCESS;
}

/** woal_callbacks */
static mlan_callbacks woal_callbacks = {
    .moal_malloc      = wrapper_moal_malloc,
    .moal_mfree       = wrapper_moal_mfree,
    .moal_recv_packet = wrapper_moal_recv_packet,
    .moal_init_timer  = wrapper_moal_init_timer,
    .moal_free_timer  = wrapper_moal_free_timer,
    .moal_start_timer = wrapper_moal_start_timer,
    .moal_stop_timer  = wrapper_moal_stop_timer,
    .moal_init_lock   = wrapper_moal_init_lock,
    .moal_free_lock   = wrapper_moal_free_lock,
    .moal_spin_lock   = wrapper_moal_spin_lock,
    .moal_spin_unlock = wrapper_moal_spin_unlock,
};

int mlan_subsys_init(void)
{
    (void)memcpy((void *)&mlan_dev.callbacks, (const void *)&woal_callbacks, sizeof(mlan_callbacks));

    /* The mlinux driver has an array of these which is dynamically allocated
     * in function woal_update_drv_tbl (moal_main.c). We have only one.
     */
    /** BSS type */
    mlan_dev.bss_attr[0].bss_type = MLAN_BSS_TYPE_STA;
    /** Data frame type: Ethernet II, 802.11, etc. */
    mlan_dev.bss_attr[0].frame_type = MLAN_DATA_FRAME_TYPE_ETH_II;
    /** The BSS is active (non-0) or not (0). */
    mlan_dev.bss_attr[0].active = MTRUE;
    /** BSS Priority */
    mlan_dev.bss_attr[0].bss_priority = 0;
    /** BSS number */
    mlan_dev.bss_attr[0].bss_num = 0;
    /* 11d initialization */
    /* mlan_dev.cfg_11d = 0; */

    mlan_dev.bss_attr[1].bss_type = MLAN_BSS_TYPE_UAP;
    /** Data frame type: Ethernet II, 802.11, etc. */
    mlan_dev.bss_attr[1].frame_type = MLAN_DATA_FRAME_TYPE_ETH_II;
    /** The BSS is active (non-0) or not (0). */
    mlan_dev.bss_attr[1].active = MTRUE;
    /** BSS Priority */
    mlan_dev.bss_attr[1].bss_priority = 0;
    /** BSS number */
    mlan_dev.bss_attr[1].bss_num = 0;

    /* fixme: check return value above status */
    (void)mlan_register(&mlan_dev, (t_void **)&mlan_adap);

    (void)mlan_init_fw(mlan_adap);
    /* wlan_init_adapter(mlan_adap); */

    return WM_SUCCESS;
}

int mlan_subsys_deinit(void)
{
    (void)mlan_unregister(mlan_adap);
    mlan_adap = MNULL;

    wlan_clear_scan_bss();

    return WM_SUCCESS;
}


/* Note: The saved_event_buff is allocated by the cmd resp/event handler
   thread. We need to free it in this function */
int wrapper_wlan_cmd_11n_addba_rspgen(void *saved_event_buff)
{
    Event_Ext_t *evt = (Event_Ext_t *)saved_event_buff;

    (void)wifi_get_command_lock();

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    if (evt->bss_type == BSS_TYPE_STA)
    {
        cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, BSS_TYPE_STA);
        (void)wlan_cmd_11n_addba_rspgen((mlan_private *)mlan_adap->priv[evt->bss_type], cmd, &evt->reason_code);
    }
    else if (evt->bss_type == BSS_TYPE_UAP)
    {
        cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, BSS_TYPE_UAP);

        (void)wlan_cmd_11n_uap_addba_rspgen((mlan_private *)mlan_adap->priv[evt->bss_type], cmd, &evt->reason_code);
    }
    else
    { /* Do Nothing */
    }


    os_mem_free(saved_event_buff);
    (void)wifi_wait_for_cmdresp(NULL);

    return 0;
}

int wrapper_wlan_cmd_11n_ba_stream_timeout(void *saved_event_buff)
{
    Event_Ext_t *evt = (Event_Ext_t *)saved_event_buff;

    wlan_11n_ba_stream_timeout(mlan_adap->priv[evt->bss_type], (HostCmd_DS_11N_BATIMEOUT *)(void *)&evt->reason_code);

    os_mem_free(saved_event_buff);

    return 0;
}

int wrapper_wlan_cmd_11n_delba_rspgen(void *saved_event_buff)
{
    Event_Ext_t *evt = (Event_Ext_t *)saved_event_buff;

    wlan_11n_delete_bastream((mlan_private *)mlan_adap->priv[0], (t_u8 *)&evt->reason_code);

    os_mem_free(saved_event_buff);

    return 0;
}

struct uap_ampdu_stat_t
{
    uint8_t mac_addr[MLAN_MAC_ADDR_LENGTH];
    bool ampudu_stat;
    bool ampudu_supported;
};

static struct uap_ampdu_stat_t uap_ampdu_stat_array[32];

static int wlan_find_ampud_info(const uint8_t *addr, struct uap_ampdu_stat_t **ampdu_info)
{
    int i;

    for (i = 0; i < 32; i++)
    {
        if (memcmp(addr, uap_ampdu_stat_array[i].mac_addr, MLAN_MAC_ADDR_LENGTH) == 0)
        {
            *ampdu_info = &uap_ampdu_stat_array[i];
            return (int)MLAN_STATUS_SUCCESS;
        }
    }

    return (int)MLAN_STATUS_FAILURE;
}

static void wlan_update_uap_ampdu_stat(uint8_t *addr, bool status)
{
    struct uap_ampdu_stat_t *ampdu_info;
    if (wlan_find_ampud_info(addr, &ampdu_info) == 0)
    {
        ampdu_info->ampudu_stat = status;
    }
    else
    {
        wifi_d("failed to udpate uap ampdu status");
    }
}

static void wlan_update_uap_ampdu_supported(uint8_t *addr, bool supported)
{
    struct uap_ampdu_stat_t *ampdu_info;
    if (wlan_find_ampud_info(addr, &ampdu_info) == 0)
    {
        ampdu_info->ampudu_supported = supported;
    }
    else
    {
        wifi_d("failed to udpate uap ampdu supported");
    }
}

static void wlan_update_uap_ampdu_info(uint8_t *addr, uint8_t action)
{
    struct uap_ampdu_stat_t *ampdu_info;
    uint8_t temp_addr[MLAN_MAC_ADDR_LENGTH];
#ifdef DUMP_PACKET_MAC
    dump_mac_addr("wlan_update_uap_ampdu_info", addr);
#endif
    if (wlan_find_ampud_info(addr, &ampdu_info) == 0)
    {
        if (action != 0U)
        {
            wifi_d("already exist for adding uap ampdu info");
        }
        else
        {
            (void)memset(ampdu_info->mac_addr, 0, MLAN_MAC_ADDR_LENGTH);
        }
    }
    else
    {
        if (action != 0U)
        {
            (void)memset(temp_addr, 0, MLAN_MAC_ADDR_LENGTH);
            if (wlan_find_ampud_info(temp_addr, &ampdu_info) == 0)
            {
                (void)memcpy((void *)ampdu_info->mac_addr, (const void *)addr, MLAN_MAC_ADDR_LENGTH);
                ampdu_info->ampudu_stat      = MFALSE;
                ampdu_info->ampudu_supported = MTRUE;
            }
            else
            {
                wifi_d(
                    "not available for adding "
                    "uap ampdu info");
            }
        }
        else
        {
            wifi_d("uap ampdu info has already removed");
        }
    }
}

mlan_status wrapper_wlan_upa_ampdu_enable(const uint8_t *addr)
{
    int ret;
    struct uap_ampdu_stat_t *ampdu_info;

    if (wlan_find_ampud_info(addr, &ampdu_info) == 0)
    {
        if ((ampdu_info->ampudu_stat == MFALSE) && ampdu_info->ampudu_supported)
        {
            ret = wlan_send_addba(mlan_adap->priv[1], 0, addr);
            if (ret != 0)
            {
                wifi_d("uap failed to send addba req");
                return MLAN_STATUS_FAILURE;
            }
        }
    }
    return MLAN_STATUS_SUCCESS;
}

static mlan_status do_wlan_ret_11n_addba_resp(HostCmd_DS_COMMAND *resp)
{
    mlan_status rv = MLAN_STATUS_SUCCESS;

    if (resp->result != HostCmd_RESULT_OK)
    {
        wifi_d("Failed: ADDBA RESULT: %d", resp->result);
        return MLAN_STATUS_FAILURE;
    }

    int bss_type = HostCmd_GET_BSS_TYPE(resp->seq_num);

    if (bss_type == BSS_TYPE_STA)
    {
        mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
        rv                   = wlan_ret_11n_addba_resp(pmpriv, resp);
    }

    return rv;
}

static mlan_status do_wlan_ret_11n_addba_req(mlan_private *priv, HostCmd_DS_COMMAND *resp)
{
    t_u8 tid;
    HostCmd_DS_11N_ADDBA_RSP *padd_ba_rsp = (HostCmd_DS_11N_ADDBA_RSP *)&resp->params.add_ba_rsp;
    padd_ba_rsp->status_code              = wlan_le16_to_cpu(padd_ba_rsp->status_code);

    tid = (padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_TID_MASK) >> BLOCKACKPARAM_TID_POS;

    mlan_bss_type bss_type = (mlan_bss_type)HostCmd_GET_BSS_TYPE(resp->seq_num);
    if (padd_ba_rsp->status_code == BA_RESULT_SUCCESS)
    {
        if (bss_type == MLAN_BSS_TYPE_STA)
        {
            ampdu_status_flag = MTRUE;
        }
        else if (bss_type == MLAN_BSS_TYPE_UAP)
        {
            wlan_update_uap_ampdu_stat(padd_ba_rsp->peer_mac_addr, MTRUE);
        }
        else
        { /* Do Nothing */
        }
    }
    else
    {
        if (padd_ba_rsp->add_rsp_result != BA_RESULT_TIMEOUT)
        {
            wifi_d("Failed: ADDBA req: %d", padd_ba_rsp->add_rsp_result);
            if (bss_type == MLAN_BSS_TYPE_STA)
            {
                priv->aggr_prio_tbl[tid].ampdu_ap = BA_STREAM_NOT_ALLOWED;
            }
            else if (bss_type == MLAN_BSS_TYPE_UAP)
            {
                wlan_update_uap_ampdu_supported(padd_ba_rsp->peer_mac_addr, MFALSE);
            }
            else
            { /* Do Nothing */
            }
        }
    }
    return MLAN_STATUS_SUCCESS;
}

static mlan_status do_wlan_ret_11n_delba(mlan_private *priv, HostCmd_DS_COMMAND *resp)
{
    HostCmd_DS_11N_DELBA *pdel_ba = (HostCmd_DS_11N_DELBA *)&resp->params.del_ba;
    mlan_bss_type bss_type        = (mlan_bss_type)HostCmd_GET_BSS_TYPE(resp->seq_num);
    if (pdel_ba->del_result == BA_RESULT_SUCCESS)
    {
        if (bss_type == MLAN_BSS_TYPE_STA)
        {
            ampdu_status_flag = MFALSE;
        }
        else if (bss_type == MLAN_BSS_TYPE_UAP)
        {
            wlan_update_uap_ampdu_stat(pdel_ba->peer_mac_addr, MFALSE);
        }
        else
        { /* Do Nothing */
        }
    }
    else
    {
        wifi_d("Failed: DELBA: %d", pdel_ba->del_result);
    }

    return MLAN_STATUS_SUCCESS;
}

static bool wlan_is_ampdu_allowed(mlan_private *priv, int tid)
{
    return (priv->aggr_prio_tbl[tid].ampdu_ap != BA_STREAM_NOT_ALLOWED) ? MTRUE : MFALSE;
}

// Only Enable AMPDU for station interface
#ifdef CONFIG_WMM
mlan_status wrapper_wlan_sta_ampdu_enable(t_u8 tid)
#else
mlan_status wrapper_wlan_sta_ampdu_enable(void)
#endif
{
    int ret;
#ifdef CONFIG_WMM
    static t_u8 ampdu_set_tid[MAX_NUM_TID];
#endif
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    t_u8 cur_mac[MLAN_MAC_ADDR_LENGTH];

#ifdef CONFIG_WMM
    if (!ampdu_status_flag || (!ampdu_set_tid[tid] && wlan_is_ampdu_allowed(mlan_adap->priv[0], tid)))
#else
    if (!ampdu_status_flag && wlan_is_ampdu_allowed(mlan_adap->priv[0], 0))
#endif
    {
#ifdef CONFIG_WMM
        ampdu_set_tid[tid] = true;
#endif
        if (pmpriv->media_connected == MTRUE)
        {
            (void)memcpy((void *)cur_mac, (const void *)pmpriv->curr_bss_params.bss_descriptor.mac_address,
                         MLAN_MAC_ADDR_LENGTH);
        }
        else
        {
            wifi_d("sta not connected: failed to send addba req");
            return MLAN_STATUS_FAILURE;
        }

        ret = wlan_send_addba(mlan_adap->priv[0],
#ifdef CONFIG_WMM
                              tid,
#else
                              0,
#endif
                              (t_u8 *)cur_mac);
        if (ret != 0)
        {
            wifi_d("sta: failed to send addba req");
            return MLAN_STATUS_FAILURE;
        }
    }

    return MLAN_STATUS_SUCCESS;
}

int wrapper_wlan_11d_enable(void)
{
    t_u32 enable_11d = (t_u32)ENABLE_11D;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    (void)wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_SNMP_MIB, HostCmd_ACT_GEN_SET,
                                   (t_u32)Dot11D_i, NULL, &enable_11d, cmd);

    return wifi_wait_for_cmdresp(NULL);
}

int wrapper_wlan_ecsa_enable(void)
{
    bool ecsa_enable = MTRUE;

    if ((mlan_adap->fw_cap_info & FW_CAPINFO_ECSA) == 0U)
    {
        return -WM_FAIL;
    }

    mlan_adap->ecsa_enable = MTRUE;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    (void)wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_SNMP_MIB, HostCmd_ACT_GEN_SET,
                                   (t_u32)ECSAEnable_i, NULL, &ecsa_enable, cmd);

    return wifi_wait_for_cmdresp(NULL);
}

int wrapper_wlan_cmd_get_hw_spec(void)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;
    (void)wlan_cmd_get_hw_spec((mlan_private *)mlan_adap->priv[0], cmd);
    (void)wifi_wait_for_cmdresp(NULL);
    return 0;
}

mlan_status wrapper_wlan_cmd_mgmt_ie(int bss_type, void *buffer, unsigned int len, t_u16 action)
{
    void *pdata_buf = NULL;
    HostCmd_DS_MGMT_IE_LIST_CFG ds_mgmt_ie_list_cfg;

    (void)wifi_get_command_lock();

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, bss_type);
    cmd->result = 0x0;

    (void)memset(&ds_mgmt_ie_list_cfg, 0x00, sizeof(HostCmd_DS_MGMT_IE_LIST_CFG));

    (void)memcpy((void *)&ds_mgmt_ie_list_cfg, (const void *)buffer, len);

    pdata_buf = &ds_mgmt_ie_list_cfg;

    mlan_status status = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MGMT_IE_LIST, action,
                                                  0, NULL, pdata_buf, cmd);

    (void)wifi_wait_for_cmdresp(buffer);
    return status;
}

int wrapper_wlan_handle_rx_packet(t_u16 datalen, RxPD *rxpd, void *p, void *payload)
{
    w_pkt_d("[recv]                  : %p T: %d L: %d", p, rxpd->rx_pkt_type, rxpd->rx_pkt_length);

    /* fixme: Check if mlan buffer can be allocated from standard mlan
       function */
    pmlan_buffer pmbuf = os_mem_alloc(sizeof(mlan_buffer));
    if (pmbuf == MNULL)
    {
        /* No mlan_buffer available. Drop this packet */
        /* fixme: Need to note this event. */
        wifi_w("No memory available. Have to drop packet.");
        return -WM_FAIL;
    }
    (void)memset(pmbuf, 0x00, sizeof(mlan_buffer));

    /** Buffer descriptor, e.g. skb in Linux */
    /* Note: We are storing payload member here. We need to unwind
       pointer when passing pbuf to lwip */
    pmbuf->pdesc = payload;

    /* Store it for later retrivval. mlan stack does not know about this */
    pmbuf->lwip_pbuf = p;

    /* fixme: check that no data is accessed after this pointer
      Note: Important: mlan requires RxPD in number of places. We
    already have duplicated the ethernet packet in the pbuf. We cannot
    afford to duplicate the entire SDIO buffer and reduplicate the
    ethernet part of it into pbuf which the lwip requires. So, we will
    duplicate only the RxPD for ourselves. We will have to change all
    the code which assumes that there is ethernet packet after RxPD */
    /** Pointer to buffer */
    /* fixme: CHK this*/
    pmbuf->pbuf = (t_u8 *)os_mem_alloc(sizeof(RxPD));
    if (pmbuf->pbuf == MNULL)
    {
        /* No buffer available. Drop this packet */
        /* fixme: Need to note this event. */
        os_mem_free(pmbuf);
        wifi_w("No memory available. Have to drop packet.");
        return -WM_FAIL;
    }
    (void)memcpy((void *)pmbuf->pbuf, (const void *)rxpd, sizeof(RxPD));

    /** Offset to data */
    /* This should ideally be INTF_HEADER_LEN. But we not be storing
       initial INTF_HEADER_LEN bytes. We will store RxPD directly */
    /* fixme: CHK this*/
    pmbuf->data_offset = 0;

    /** Data length */
    /* fixme: CHK this*/
    pmbuf->data_len = datalen;
    /** Buffer type: data, cmd, event etc. */
    pmbuf->buf_type = MLAN_BUF_TYPE_DATA;
    /** Fields below are valid for data packet only */
    /** QoS priority */
    /* t_u32 priority; */
    /** Time stamp when packet is received (seconds) */
    /* t_u32 in_ts_sec; */
    /** Time stamp when packet is received (micro seconds) */
    /* t_u32 in_ts_usec; */
    /** Time stamp when packet is processed (seconds) */
    /* t_u32 out_ts_sec; */
    /** Time stamp when packet is processed (micro seconds) */
    /* t_u32 out_ts_usec; */

    /** Fields below are valid for MLAN module only */
    /** Pointer to parent mlan_buffer */
    /* struct _mlan_buffer *pparent; */
    /** Use count for this buffer */
    /* t_u32 use_count; */

    mlan_status rv = wlan_handle_rx_packet(mlan_adap, pmbuf);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        /* mlan did not accept this packet for some reason. Some
           probable reasons are - duplicate packet - packet outside
           the current 11n aggregation window.

           We need to free allocated structures.
        */
        os_mem_free(pmbuf->pbuf);
        os_mem_free(pmbuf);
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

/* fixme: This AMSDU special handling needs to be redesigned. We could
   create a copy of the SDIO packet buffer for _every_ packet AMSDU and
   non-AMSDU. We could then use RAW_PBUF allocation to allocate pbufs
   during final step of packet delivery to lwip stack.

   Note: This is not AMSDU inside AMPDU. This is a pure AMSDU.
   Note: We will not be duplicating the AMSDU packet from the SDIO
   buffer. So, we have to ensure that there are no race
   conditions. i.e. the SDIO buffer is not overwritten in the background.
*/
int wrapper_wlan_handle_amsdu_rx_packet(const t_u8 *rcvdata, const t_u16 datalen)
{
    w_pkt_d("[amsdu] [recv]       : L: %d", datalen);

    RxPD *rxpd = (RxPD *)(void *)((t_u8 *)rcvdata + INTF_HEADER_LEN);

    /* fixme: Check if mlan buffer can be allocated from standard mlan
       function */
    pmlan_buffer pmbuf = os_mem_alloc(sizeof(mlan_buffer));
    if (pmbuf == MNULL)
    {
        /* No mlan_buffer available. Drop this packet */
        /* fixme: Need to note this event. */
        w_pkt_e("[amsdu] No memory available. Have to drop packet");
        return -WM_FAIL;
    }
    (void)memset(pmbuf, 0x00, sizeof(mlan_buffer));

    /** Buffer descriptor, e.g. skb in Linux */
    /* Note: We are storing payload member here. We need to unwind
       pointer when passing pbuf to lwip */
    pmbuf->pdesc = ((t_u8 *)rcvdata + INTF_HEADER_LEN + rxpd->rx_pkt_offset);

    /* Store it for later retrivval. mlan stack does not know about this */
    pmbuf->lwip_pbuf = NULL;

    /* Since this is an AMSDU packet we have the entire SDIO buffer
       with us. So we do not need to duplicate RxPD
    */
    pmbuf->pbuf = (t_u8 *)rxpd;
    /** Offset to data */
    pmbuf->data_offset = 0;

    /** Data length */
    /* fixme: CHK this*/
    pmbuf->data_len = datalen;
    /** Buffer type: data, cmd, event etc. */
    pmbuf->buf_type = MLAN_BUF_TYPE_DATA;
    /** Fields below are valid for data packet only */
    /** QoS priority */
    /* t_u32 priority; */
    /** Time stamp when packet is received (seconds) */
    /* t_u32 in_ts_sec; */
    /** Time stamp when packet is received (micro seconds) */
    /* t_u32 in_ts_usec; */
    /** Time stamp when packet is processed (seconds) */
    /* t_u32 out_ts_sec; */
    /** Time stamp when packet is processed (micro seconds) */
    /* t_u32 out_ts_usec; */

    /** Fields below are valid for MLAN module only */
    /** Pointer to parent mlan_buffer */
    /* struct _mlan_buffer *pparent; */
    /** Use count for this buffer */
    /* t_u32 use_count; */

    if (rxpd->bss_type == (t_u8)MLAN_BSS_ROLE_STA)
    {
        (void)wlan_handle_rx_packet(mlan_adap, pmbuf);
    }
    else
    {
        pmlan_private priv = wlan_get_priv(mlan_adap, MLAN_BSS_ROLE_UAP);
        pmbuf->data_len    = rxpd->rx_pkt_length;
        pmbuf->data_offset += rxpd->rx_pkt_offset;
        (void)wlan_11n_deaggregate_pkt(priv, pmbuf);
    }
    /* if (rv != MLAN_STATUS_SUCCESS) { */
    /*
      We need to free allocated structures. In case of AMSDU this pmbuf
      is not freed inside mlan
    */
    os_mem_free(pmbuf);
    /* return -WM_FAIL; */
    /* } */

    return WM_SUCCESS;
}

void wrapper_wlan_cmd_11n_cfg(HostCmd_DS_COMMAND *cmd)
{
    mlan_ds_11n_tx_cfg txcfg = {/** HTTxCap */
                                .httxcap = DOT11N_CFG_ENABLE_GREENFIELD_XMIT | DOT11N_CFG_ENABLE_SHORT_GI_20MHZ |
                                           DOT11N_CFG_ENABLE_SHORT_GI_40MHZ,
                                /** HTTxInfo */
                                .httxinfo = DOT11N_CFG_ENABLE_RIFS,
                                /** Band selection */
                                .misc_cfg = BAND_SELECT_BOTH};

    (void)wlan_cmd_11n_cfg((mlan_private *)mlan_adap->priv[0], cmd, HostCmd_ACT_GEN_SET, &txcfg);
}

/*
  fixme: Following three functions will not be needed when
  wlan_11h_handle_event_chanswann is used to handle the relevant event */
void set_event_chanswann(void)
{
    mlan_adap->state_11h.recvd_chanswann_event = MTRUE;
}

void clear_event_chanswann(void)
{
    mlan_adap->state_11h.recvd_chanswann_event = MFALSE;
}

void wrapper_wlan_11d_clear_parsedtable(void)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    (void)wlan_11d_clear_parsedtable(pmpriv);
}

/*
  This was added to solve the radar DFS issue. The function
  wlan_11h_radar_detect_required() checks for this and sets the scan to
  active if media is connected. The temporary problem with our current port
  of mlan is that after a deauth is sent after channel switch event from
  the AP, the media connected event is still MTRUE. Hence, during scan the
  old channel of the AP is marked as active scan which is not a correct way
  of scanning on that channel (since radar interference is detected on
  it). So, for now we clear this bit manually to avoid active scan on the
  old channel. This bit will be set again at the next association completion.

  fixme: Check this later.
 */
void wrapper_clear_media_connected_event(void)
{
    mlan_private *pmpriv    = (mlan_private *)mlan_adap->priv[0];
    pmpriv->media_connected = MFALSE;
}

mlan_status wifi_prepare_and_send_cmd(IN mlan_private *pmpriv,
                                      IN t_u16 cmd_no,
                                      IN t_u16 cmd_action,
                                      IN t_u32 cmd_oid,
                                      IN t_void *pioctl_buf,
                                      IN t_void *pdata_buf,
                                      mlan_bss_type bss_type,
                                      void *priv)
{
    pmlan_ioctl_req pioctl_req = (mlan_ioctl_req *)pioctl_buf;
    mlan_status rv             = MLAN_STATUS_SUCCESS;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    if (pioctl_req != NULL)
    {
        if (pioctl_req->bss_index == 1U)
        {
            bss_type = MLAN_BSS_TYPE_UAP;
        }
    }

    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, (t_u8)bss_type);
    cmd->result = 0x0;

    if (bss_type == MLAN_BSS_TYPE_UAP)
    {
        rv = wlan_ops_uap_prepare_cmd(pmpriv, cmd_no, cmd_action, cmd_oid, pioctl_buf, pdata_buf, cmd);
    }
    else
    {
        rv = wlan_ops_sta_prepare_cmd(pmpriv, cmd_no, cmd_action, cmd_oid, pioctl_buf, pdata_buf, cmd);
    }

    if (rv != MLAN_STATUS_SUCCESS)
    {
        wifi_d("Failed to prepare cmd.");
        wm_wifi.cmd_resp_priv = NULL;
        (void)wifi_put_command_lock();
        return rv;
    }

    (void)wifi_wait_for_cmdresp(priv);
    return MLAN_STATUS_SUCCESS;
}

bool wmsdk_is_11N_enabled(void)
{
    return true;
}

/* static */ int wifi_send_bss_ioctl(mlan_ds_bss *bss)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)bss;
    req.buf_len   = sizeof(mlan_ds_bss);
    req.bss_index = 0;
    req.req_id    = MLAN_IOCTL_BSS;

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wrapper_get_wpa_ie_in_assoc(uint8_t *wpa_ie)
{
    mlan_private *priv = (mlan_private *)mlan_adap->priv[0];

    /* Reset all state variables */
    (void)memcpy((void *)wpa_ie, (const void *)&priv->wpa_ie, sizeof(priv->wpa_ie));

    return (int)priv->wpa_ie_len;
}

static int wifi_send_11n_cfg_ioctl(mlan_act_ioctl action, mlan_ds_11n_cfg *ds_11n_cfg)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)ds_11n_cfg;
    req.buf_len   = sizeof(mlan_ds_11n_cfg);
    req.bss_index = 0;
    req.req_id    = MLAN_IOCTL_11N_CFG;
    req.action    = action;

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

static int wifi_send_uap_11n_cfg_ioctl(mlan_act_ioctl action, mlan_ds_11n_cfg *ds_11n_cfg)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)ds_11n_cfg;
    req.buf_len   = sizeof(mlan_ds_11n_cfg);
    req.bss_index = 1;
    req.req_id    = MLAN_IOCTL_11N_CFG;
    req.action    = action;

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wifi_set_htcapinfo(unsigned int htcapinfo)
{
    mlan_ds_11n_cfg ds_11n_cfg;

    (void)memset(&ds_11n_cfg, 0x00, sizeof(mlan_ds_11n_cfg));

    ds_11n_cfg.sub_command           = MLAN_OID_11N_HTCAP_CFG;
    ds_11n_cfg.param.htcap_cfg.htcap = htcapinfo;
#ifdef CONFIG_5GHz_SUPPORT
    ds_11n_cfg.param.htcap_cfg.misc_cfg = BAND_SELECT_BOTH;
#else
    ds_11n_cfg.param.htcap_cfg.misc_cfg = BAND_SELECT_BG;
#endif

    return wifi_send_11n_cfg_ioctl(MLAN_ACT_SET, &ds_11n_cfg);
}

int wifi_set_httxcfg(unsigned short httxcfg)
{
    mlan_ds_11n_cfg ds_11n_cfg;

    (void)memset(&ds_11n_cfg, 0x00, sizeof(mlan_ds_11n_cfg));

    ds_11n_cfg.sub_command          = MLAN_OID_11N_CFG_TX;
    ds_11n_cfg.param.tx_cfg.httxcap = httxcfg;
#ifdef CONFIG_5GHz_SUPPORT
    ds_11n_cfg.param.tx_cfg.misc_cfg = BAND_SELECT_BOTH;
#else
    ds_11n_cfg.param.tx_cfg.misc_cfg    = BAND_SELECT_BG;
#endif

    return wifi_send_11n_cfg_ioctl(MLAN_ACT_SET, &ds_11n_cfg);
}

int wifi_uap_set_httxcfg_int(unsigned short httxcfg)
{
    mlan_ds_11n_cfg ds_11n_cfg;

    (void)memset(&ds_11n_cfg, 0x00, sizeof(mlan_ds_11n_cfg));

    ds_11n_cfg.sub_command          = MLAN_OID_11N_CFG_TX;
    ds_11n_cfg.param.tx_cfg.httxcap = httxcfg;
#ifdef CONFIG_5GHz_SUPPORT
    ds_11n_cfg.param.tx_cfg.misc_cfg = BAND_SELECT_BOTH;
#else
    ds_11n_cfg.param.tx_cfg.misc_cfg    = BAND_SELECT_BG;
#endif

    return wifi_send_uap_11n_cfg_ioctl(MLAN_ACT_SET, &ds_11n_cfg);
}

static int wifi_send_tx_power_cfg_ioctl(mlan_act_ioctl action, mlan_ds_power_cfg *ds_power_cfg)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)ds_power_cfg;
    req.buf_len   = sizeof(mlan_ds_power_cfg);
    req.bss_index = 0;
    req.req_id    = MLAN_IOCTL_POWER_CFG;
    req.action    = action;

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wifi_set_tx_power(t_u32 power_level)
{
    mlan_ds_power_cfg ds_power_cfg;

    (void)memset(&ds_power_cfg, 0x00, sizeof(mlan_ds_power_cfg));

    ds_power_cfg.sub_command                   = MLAN_OID_POWER_CFG;
    ds_power_cfg.param.power_cfg.is_power_auto = MFALSE;
    ds_power_cfg.param.power_cfg.power_level   = power_level;

    return wifi_send_tx_power_cfg_ioctl(MLAN_ACT_SET, &ds_power_cfg);
}

int wifi_set_tx_power_ext(uint32_t len, uint32_t *power_data)
{
    mlan_ds_power_cfg ds_power_cfg;

    (void)memset(&ds_power_cfg, 0x00, sizeof(mlan_ds_power_cfg));

    ds_power_cfg.sub_command         = MLAN_OID_POWER_CFG_EXT;
    ds_power_cfg.param.power_ext.len = len;
    (void)memcpy((void *)ds_power_cfg.param.power_ext.power_data, (const void *)power_data, len);

    return wifi_send_tx_power_cfg_ioctl(MLAN_ACT_SET, &ds_power_cfg);
}

int wifi_get_tx_power(t_u32 *power_level)
{
    int ret;
    mlan_ds_power_cfg ds_power_cfg;
    mlan_ioctl_req ioctl_req_pwr_cfg;
    (void)memset(&ds_power_cfg, 0x00, sizeof(mlan_ds_power_cfg));
    (void)memset(&ioctl_req_pwr_cfg, 0x00, sizeof(mlan_ioctl_req));

    ds_power_cfg.sub_command                   = MLAN_OID_POWER_CFG;
    ds_power_cfg.param.power_cfg.is_power_auto = MFALSE;
    wm_wifi.cmd_resp_ioctl                     = &ioctl_req_pwr_cfg;
    ioctl_req_pwr_cfg.pbuf                     = (t_u8 *)(&ds_power_cfg);
    ioctl_req_pwr_cfg.buf_len                  = sizeof(mlan_ds_power_cfg);
    ret                                        = wifi_send_tx_power_cfg_ioctl(MLAN_ACT_GET, &ds_power_cfg);
    wm_wifi.cmd_resp_ioctl                     = NULL;

    if (ret == WM_SUCCESS)
    {
        *power_level = ds_power_cfg.param.power_cfg.power_level;
    }
    return ret;
}

static int wifi_send_tx_rate_cfg_ioctl(mlan_act_ioctl action, mlan_ds_rate *ds_rate_cfg)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)ds_rate_cfg;
    req.buf_len   = sizeof(mlan_ds_rate);
    req.bss_index = 0;
    req.req_id    = MLAN_IOCTL_RATE;
    req.action    = action;

    mlan_status rv;
    if (is_sta_connected())
    {
        rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    }
    else
    {
        rv = wlan_ops_uap_ioctl(mlan_adap, &req);
    }
    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wifi_set_txratecfg(wifi_ds_rate ds_rate)
{
    mlan_ds_rate ds_rate_cfg;

    if (ds_rate.sub_command != WIFI_DS_RATE_CFG)
    {
        return -WM_FAIL;
    }

    (void)memset(&ds_rate_cfg, 0x00, sizeof(mlan_ds_rate));

    ds_rate_cfg.sub_command = MLAN_OID_RATE_CFG;
    if (ds_rate.param.rate_cfg.rate_format == MLAN_RATE_FORMAT_AUTO)
    {
        ds_rate_cfg.param.rate_cfg.is_rate_auto = MTRUE;
    }
    else
    {
        ds_rate_cfg.param.rate_cfg.is_rate_auto = MFALSE;
        ds_rate_cfg.param.rate_cfg.rate_type    = MLAN_RATE_INDEX;
        ds_rate_cfg.param.rate_cfg.rate         = ds_rate.param.rate_cfg.rate_index;
        ds_rate_cfg.param.rate_cfg.rate_format  = ds_rate.param.rate_cfg.rate_format;
#ifdef SD8801
        if (ds_rate.param.rate_cfg.rate_format == MLAN_RATE_FORMAT_HT)
            ds_rate_cfg.param.rate_cfg.rate += +MLAN_RATE_INDEX_MCS0;
#endif
#if defined(CONFIG_11AC) || defined(CONFIG_11AX)
        ds_rate_cfg.param.rate_cfg.nss = ds_rate.param.rate_cfg.nss;
#endif
    }
    return wifi_send_tx_rate_cfg_ioctl(MLAN_ACT_SET, &ds_rate_cfg);
}

int wifi_get_txratecfg(wifi_ds_rate *ds_rate)
{
    int ret;

    mlan_ds_rate ds_rate_cfg;

    if (ds_rate->sub_command != WIFI_DS_RATE_CFG)
    {
        return -WM_FAIL;
    }

    (void)memset(&ds_rate_cfg, 0x00, sizeof(mlan_ds_rate));

    ds_rate_cfg.sub_command = MLAN_OID_RATE_CFG;

    wm_wifi.cmd_resp_ioctl = ds_rate;

    ret = wifi_send_tx_rate_cfg_ioctl(MLAN_ACT_GET, &ds_rate_cfg);

    wm_wifi.cmd_resp_ioctl = NULL;

    return ret;
}

bool wrapper_wlan_11d_support_is_enabled(void)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    return wlan_11d_support_is_enabled(pmpriv);
}

/*
 * fixme: This function is temporarily present till the mlan transition is complete.
 */
int wrapper_wifi_assoc(
    const unsigned char *bssid, int wlan_security, bool is_wpa_tkip, unsigned int owe_trans_mode, bool is_ft)
{
    int ret = 0;
    (void)is_ft;
    mlan_private *priv = (mlan_private *)mlan_adap->priv[0];
    /* BSSDescriptor_t *bssDesc = os_mem_alloc(sizeof(BSSDescriptor_t)); */
    /* if (!bssDesc) */
    /* 	return -WM_FAIL; */
    int idx = wlan_find_bssid_in_list(priv, (const unsigned char *)bssid, MLAN_BSS_MODE_NEGATIVE);
    if (idx == -1)
    {
        wifi_w("Could not find BSSID in mlan scan list");
        ret = (int)WLAN_BSSID_NOT_FOUND_IN_SCAN_LIST;
        return -ret;
    }

    /* Reset all state variables */
    (void)memset(&priv->wpa_ie, 0, sizeof(priv->wpa_ie));
    priv->wpa_ie_len                   = 0;
    priv->sec_info.wpa2_enabled        = false;
    priv->sec_info.wapi_enabled        = false;
    priv->sec_info.ewpa_enabled        = false;
    priv->sec_info.wpa_enabled         = false;
    priv->sec_info.authentication_mode = MLAN_AUTH_MODE_AUTO;
    /* Reset ADDBA flag so STA sends request on each new connection */
    ampdu_status_flag = MFALSE;
    BSSDescriptor_t *d = &mlan_adap->pscan_table[idx];
    /* fixme: This code is quite hacky and is present only because
     * security part is yet not fully integrated into mlan. This will
     * not be necessary after the integration is complete.
     */
        if ((d->pwpa_ie != MNULL) && (d->prsn_ie != MNULL) && (wlan_security == WLAN_SECURITY_WPA_WPA2_MIXED))
    {
        priv->sec_info.is_wpa_tkip  = is_wpa_tkip;
        priv->sec_info.wpa2_enabled = true;
        if (d->rsn_ie_buff_len <= sizeof(priv->wpa_ie))
        {
            (void)memcpy((void *)priv->wpa_ie, (const void *)d->rsn_ie_buff, d->rsn_ie_buff_len);
            priv->wpa_ie_len = (t_u8)d->rsn_ie_buff_len;
        }
        else
        {
            wifi_e("Failed to copy RSN IE");
            return -WM_FAIL;
        }
    }
    else if ((d->pwpa_ie != MNULL) &&
             (wlan_security == WLAN_SECURITY_WPA || wlan_security == WLAN_SECURITY_WPA_WPA2_MIXED))
    {
        priv->sec_info.is_wpa_tkip = is_wpa_tkip;
        priv->sec_info.wpa_enabled = true;
        if (d->wpa_ie_buff_len <= sizeof(priv->wpa_ie))
        {
            (void)memcpy((void *)priv->wpa_ie, (const void *)d->wpa_ie_buff, d->wpa_ie_buff_len);
            priv->wpa_ie_len = (t_u8)d->wpa_ie_buff_len;
        }
        else
        {
            wifi_e("Failed to copy WPA IE");
            return -WM_FAIL;
        }
    }
    else if ((d->prsn_ie != MNULL) &&
             (wlan_security == WLAN_SECURITY_WPA2 || wlan_security == WLAN_SECURITY_WPA_WPA2_MIXED ||
              wlan_security == WLAN_SECURITY_WPA3_SAE))
    {
        if (wlan_security == WLAN_SECURITY_WPA3_SAE)
        {
            priv->sec_info.authentication_mode = MLAN_AUTH_MODE_SAE;
        }
        priv->sec_info.is_wpa_tkip  = is_wpa_tkip;
        priv->sec_info.wpa2_enabled = true;
        if (d->rsn_ie_buff_len <= sizeof(priv->wpa_ie))
        {
            (void)memcpy((void *)priv->wpa_ie, (const void *)d->rsn_ie_buff, d->rsn_ie_buff_len);
            priv->wpa_ie_len = (t_u8)d->rsn_ie_buff_len;
        }
        else
        {
            wifi_e("Failed to copy RSN IE.");
            return -WM_FAIL;
        }
        /* In case of WPA3 SAE-PSK mixed mode AP, RSN IE processing sets the SAE AKM,
         * but if the configured security is WPA2 PSK then AKM must be of PSK
         * hence update the AKM to WPA2 PSK and reset the PMF capabilities
         */
        if ((wlan_security == WLAN_SECURITY_WPA2) && (priv->wpa_ie[19] == 0x8U))
        {
            priv->wpa_ie[19] = 0x02;
            priv->wpa_ie[20] = 0x00;
            priv->wpa_ie[21] = 0x00;
        }
    }
    else
    { /* Do Nothing */
    }

    /* The original assoc cmd handling function of mlan sends
       additional two commands to the firmware; both
       HostCmd_CMD_802_11D_DOMAIN_INFO. In the current wmsdk wifi
       driver design we have only one command buffer. Thus, we cannot
       new cmd while the current cmd is being sent. We will change the
       default flow a little and send the 801.11D domain info commands
       first and skip that step later */
    if (wlan_11d_support_is_enabled(priv))
    {
        if (priv->support_11d != NULL)
        {
            if (priv->support_11d->wlan_11d_create_dnld_countryinfo_p(priv, d->bss_band) != MLAN_STATUS_SUCCESS)
            {
                PRINTM(MERROR, "Dnld_countryinfo_11d failed\n");
                return (int)MLAN_STATUS_FAILURE;
            }

            if (priv->support_11d->wlan_11d_parse_dnld_countryinfo_p(priv, d) != MLAN_STATUS_SUCCESS)
            {
                return (int)MLAN_STATUS_FAILURE;
            }
        }
    }
    /* fixme: check if this needs to go on heap */
    mlan_ds_bss bss;
    (void)memset(&bss, 0x00, sizeof(mlan_ds_bss));
    bss.sub_command          = MLAN_OID_BSS_START;
    bss.param.ssid_bssid.idx = (t_u32)idx + 1UL; /* + 1 req. by mlan */
    return wifi_send_bss_ioctl(&bss);
}

static void compute_rssi_values(HostCmd_DS_802_11_RSSI_INFO_RSP *prssi_info_rsp, wifi_rssi_info_t *rssi_info)
{
    g_rssi        = prssi_info_rsp->bcn_rssi_last;
    g_bcn_nf_last = prssi_info_rsp->bcn_nf_last;

    rssi_info->bcn_rssi_last  = prssi_info_rsp->bcn_rssi_last;
    rssi_info->bcn_rssi_avg   = prssi_info_rsp->bcn_rssi_avg;
    rssi_info->data_rssi_last = prssi_info_rsp->data_rssi_last;
    rssi_info->data_rssi_avg  = prssi_info_rsp->data_rssi_avg;

    rssi_info->bcn_snr_last  = CAL_SNR(prssi_info_rsp->bcn_rssi_last, prssi_info_rsp->bcn_nf_last);
    rssi_info->bcn_snr_avg   = CAL_SNR(prssi_info_rsp->bcn_rssi_avg, prssi_info_rsp->bcn_nf_avg);
    rssi_info->data_snr_last = CAL_SNR(prssi_info_rsp->data_rssi_last, prssi_info_rsp->data_nf_last);
    rssi_info->data_snr_avg  = CAL_SNR(prssi_info_rsp->data_rssi_avg, prssi_info_rsp->data_nf_avg);

    rssi_info->bcn_nf_last  = prssi_info_rsp->bcn_nf_last;
    rssi_info->bcn_nf_avg   = prssi_info_rsp->bcn_nf_avg;
    rssi_info->data_nf_last = prssi_info_rsp->data_nf_last;
    rssi_info->data_nf_avg  = prssi_info_rsp->data_nf_avg;
}
/* Function of copy hostcmd response to buffers set in wm_wifi structure */
static void wifi_set_hostcmd_resp(const HostCmd_DS_COMMAND *resp)
{
    if (wm_wifi.cmd_resp_priv == MNULL)
    {
        return;
    }
    /* reset is_hostcmd to 0, so that other commands can be processed */
    wm_wifi.hostcmd_cfg.is_hostcmd = false;

    hostcmd_cfg_t *hcmd_cfg = (hostcmd_cfg_t *)wm_wifi.cmd_resp_priv;

    *hcmd_cfg->reqd_resp_len = resp->size;
    /* Copy if response buffer is suffcient to hold response data */
    if (hcmd_cfg->resp_buf_len >= resp->size)
    {
        (void)memcpy((void *)hcmd_cfg->resp_buf, (const void *)resp, resp->size);
    }
    /* Initialize user pointer to NULL*/
    wm_wifi.cmd_resp_priv = NULL;
}

static void load_bss_list(const HostCmd_DS_STA_LIST *sta_list)
{
    if (wm_wifi.cmd_resp_priv == MNULL)
    {
        return;
    }

    /* Initialize user pointer */
    *(wifi_sta_list_t **)wm_wifi.cmd_resp_priv = NULL;

    t_u16 c = sta_list->sta_count;
    if (c > MAX_NUM_CLIENTS)
    {
        wifi_w("Too many clients: %d", c);
        return;
    }

    wifi_sta_list_t *sl = os_mem_alloc(sizeof(wifi_sta_list_t) + c * sizeof(wifi_sta_info_t));

    if (sl == MNULL)
    {
        return;
    }

    sl->count            = c;
    wifi_sta_info_t *sta = (wifi_sta_info_t *)(void *)(((t_u8 *)&sl->count) + sizeof(int));

    int i;
    const MrvlIEtypes_sta_info_t *si =
        (const MrvlIEtypes_sta_info_t *)(const void *)(((const t_u8 *)&sta_list->sta_count) + sizeof(t_u16));
    for (i = 0; i < c; i++)
    {
        if ((si[i].rssi & 0x80) != 0)
        {
            sta[i].rssi = -(256 - si[i].rssi);
        }
        else
        {
            sta[i].rssi = si[i].rssi;
        }

        (void)memcpy((void *)sta[i].mac, (const void *)si[i].mac_address, MLAN_MAC_ADDR_LENGTH);
        sta[i].power_mgmt_status = si[i].power_mfg_status;

        wifi_d("RSSI: 0x%x %d dbm", sta[i].rssi, sta[i].rssi);
    }

    /* Note: Caller needs to free this */
    *(wifi_sta_list_t **)wm_wifi.cmd_resp_priv = sl;

    wm_wifi.cmd_resp_status = WM_SUCCESS;
    wm_wifi.cmd_resp_priv   = NULL;
}

static void load_ver_ext(HostCmd_DS_COMMAND *resp)
{
    if (wm_wifi.cmd_resp_priv == MNULL)
    {
        return;
    }

    HostCmd_DS_VERSION_EXT *ver_ext     = &resp->params.verext;
    wifi_fw_version_ext_t *user_ver_ext = (wifi_fw_version_ext_t *)wm_wifi.cmd_resp_priv;

    (void)memcpy((void *)user_ver_ext->version_str, (const void *)ver_ext->version_str, resp->size - 10U);

    wm_wifi.cmd_resp_status = WM_SUCCESS;
    wm_wifi.cmd_resp_priv   = NULL;
}

int wifi_process_cmd_response(HostCmd_DS_COMMAND *resp)
{
    mlan_private *pmpriv          = (mlan_private *)mlan_adap->priv[0];
    mlan_status rv                = MLAN_STATUS_SUCCESS;
    enum wifi_event_reason result = WIFI_EVENT_REASON_FAILURE;
    t_u8 *sta_addr;

    t_u16 command = (resp->command & HostCmd_CMD_ID_MASK);

    wcmdr_d("CMD-RESP: 0x%x Size: %d Seq: %d Result: %d", command, resp->size, resp->seq_num, resp->result);

    mlan_bss_type bss_type = (mlan_bss_type)HostCmd_GET_BSS_TYPE(resp->seq_num);

    if (bss_type == MLAN_BSS_TYPE_UAP)
    {
        pmpriv = (mlan_private *)mlan_adap->priv[1];
    }
    else
    {
        pmpriv = (mlan_private *)mlan_adap->priv[0];
    }

    if (mlan_adap->ps_state == PS_STATE_SLEEP)
    {
        os_rwlock_write_unlock(&sleep_rwlock);
        mlan_adap->ps_state = PS_STATE_AWAKE;
    }

    /* Check if the command is a user issued host command */
    if (wm_wifi.hostcmd_cfg.is_hostcmd == true)
    {
        wifi_set_hostcmd_resp(resp);
        rv = MLAN_STATUS_SUCCESS;
    }
    else
    {
        switch (command)
        {
            case HOST_CMD_SMART_MODE_CFG:
            {
#ifdef CONFIG_WIFI_EXTRA_DEBUG
                HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)&resp->params.sys_config;

                if (sys_config_cmd->action == HostCmd_ACT_GEN_GET)
                    dump_hex(resp, resp->size);
#endif
            }
            break;
            case HOST_CMD_APCMD_BSS_STOP:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    pmpriv->uap_bss_started = MFALSE;
                    bss_type                = (mlan_bss_type)HostCmd_GET_BSS_TYPE(resp->seq_num);
                    if ((bss_type == MLAN_BSS_TYPE_UAP)
                    )
                    {
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                        (void)wifi_event_completion(WIFI_EVENT_UAP_STOPPED, WIFI_EVENT_REASON_SUCCESS, NULL);
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = WM_FAIL;
                }
            }
            break;
            case HOST_CMD_APCMD_BSS_START:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    bss_type = (mlan_bss_type)HostCmd_GET_BSS_TYPE(resp->seq_num);
                    if ((bss_type == MLAN_BSS_TYPE_UAP)
                    )
                    {
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                        (void)wifi_event_completion(WIFI_EVENT_UAP_STARTED, WIFI_EVENT_REASON_SUCCESS, NULL);
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = WM_FAIL;
                }
            }
            break;
            case HOST_CMD_APCMD_STA_LIST:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    const HostCmd_DS_STA_LIST *sta_list = &resp->params.sta_list;
                    load_bss_list(sta_list);
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_802_11_TX_RATE_QUERY:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        wifi_ds_rate *ds_rate = (wifi_ds_rate *)wm_wifi.cmd_resp_priv;
                        rv                    = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, ds_rate);
                        if (rv != MLAN_STATUS_SUCCESS)
                        {
                            wm_wifi.cmd_resp_status = -WM_FAIL;
                        }
                        else
                        {
                            wm_wifi.cmd_resp_status = WM_SUCCESS;
                        }
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_TBTT_OFFSET:
            {
                HostCmd_DS_TBTT_OFFSET *tbtt_offset;
                tbtt_offset = (HostCmd_DS_TBTT_OFFSET *)&resp->params.tbtt_offset;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (tbtt_offset->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            wifi_tbtt_offset_t *tbtt_offset_t = (wifi_tbtt_offset_t *)wm_wifi.cmd_resp_priv;

                            tbtt_offset_t->min_tbtt_offset = tbtt_offset->min_tbtt_offset;
                            tbtt_offset_t->max_tbtt_offset = tbtt_offset->max_tbtt_offset;
                            tbtt_offset_t->avg_tbtt_offset = tbtt_offset->avg_tbtt_offset;
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_MEF_CFG:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_802_11_RF_TX_POWER:
            {
                const HostCmd_DS_802_11_RF_TX_POWER *txp = &resp->params.txp;
                if (txp->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        wifi_tx_power_t *tx_p = (wifi_tx_power_t *)wm_wifi.cmd_resp_priv;
                        tx_p->current_level   = txp->current_level;
                        tx_p->max_power       = txp->max_power;
                        tx_p->min_power       = txp->min_power;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_802_11D_DOMAIN_INFO:
                /* No processing needed as of now */
                break;
            case HostCmd_CMD_GET_HW_SPEC:
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
                if (rv != MLAN_STATUS_SUCCESS)
                {
                    return -WM_FAIL;
                }

                (void)wifi_event_completion(WIFI_EVENT_GET_HW_SPEC, WIFI_EVENT_REASON_SUCCESS, NULL);
                break;
            case HostCmd_CMD_802_11_SCAN:
                if (resp->result != HostCmd_RESULT_OK)
                {
                    if (resp->result == HostCmd_RESULT_BUSY)
                    {
                        wifi_w("ED MAC: Scan temporary aborted, due to noisy environment");
                    }
                    else
                    {
                        wifi_w("Scan temporary failure");
                    }
                    /*
                     * Abort the split scan. The firmware has returned
                     * scan failure.
                     */
                    wlan_abort_split_scan();
                    wifi_user_scan_config_cleanup();
                    (void)wifi_event_completion(WIFI_EVENT_SCAN_RESULT, WIFI_EVENT_REASON_FAILURE, NULL);
                    break;
                }

                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
                if (rv != MLAN_STATUS_SUCCESS)
                {
                    return -WM_FAIL;
                }

                if (is_split_scan_complete())
                {
                    wifi_d("Split scan complete");
                    wifi_user_scan_config_cleanup();
                    (void)wifi_event_completion(WIFI_EVENT_SCAN_RESULT, WIFI_EVENT_REASON_SUCCESS, NULL);
                }
                break;
#ifdef CONFIG_EXT_SCAN_SUPPORT
            case HostCmd_CMD_802_11_SCAN_EXT:
                if (resp->result != 0)
                {
                    wifi_w("Scan temporary failure");
                    /*
                     * Abort the split scan. The firmware has returned
                     * scan failure.
                     */
                    wlan_abort_split_scan();
                    wifi_user_scan_config_cleanup();
                    (void)wifi_event_completion(WIFI_EVENT_SCAN_RESULT, WIFI_EVENT_REASON_FAILURE, (void *)-1);
                }
                break;
#endif
            case HostCmd_CMD_802_11_DEAUTHENTICATE:
                (void)wlan_ret_802_11_deauthenticate(pmpriv, resp, NULL);
                (void)wifi_event_completion(WIFI_EVENT_DEAUTHENTICATION, WIFI_EVENT_REASON_SUCCESS, NULL);
                break;
            case HostCmd_CMD_802_11_HS_CFG_ENH:
                wifi_process_hs_cfg_resp((t_u8 *)resp);
                break;
            case HostCmd_CMD_802_11_PS_MODE_ENH:
            {
                t_u16 *ps_action_p = (t_u16 *)os_mem_alloc(sizeof(t_u16));

                if (ps_action_p != NULL)
                {
                    result = wifi_process_ps_enh_response((t_u8 *)resp, &ps_event, ps_action_p);

                    if (ps_event != (t_u16)WIFI_EVENT_PS_INVALID)
                    {
                        if (wifi_event_completion((enum wifi_event)ps_event, result, (void *)ps_action_p) != WM_SUCCESS)
                        {
                            os_mem_free((void *)ps_action_p);
                        }
                        else
                        {
                            /*do nothing*/
                        }
                    }
                    else
                    {
                        os_mem_free((void *)ps_action_p);
                    }
                }
            }
            break;
#if 0
            case HostCmd_CMD_SUPPLICANT_PMK:
                (void)wifi_event_completion(WIFI_EVENT_SUPPLICANT_PMK,
                        WIFI_EVENT_REASON_SUCCESS,
                        resp);
                break;
#endif
            case HostCmd_CMD_802_11_RF_CHANNEL:
            {
                const HostCmd_DS_802_11_RF_CHANNEL *ch = &resp->params.rf_channel;
                if (ch->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        wifi_rf_channel_t *rf_c = (wifi_rf_channel_t *)wm_wifi.cmd_resp_priv;
                        rf_c->current_channel   = ch->current_channel;
                        rf_c->rf_type           = ch->rf_type;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
                /* Is this required */
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
            }
            break;
#ifdef SD8801
            case HostCmd_CMD_ROBUST_COEX:
            {
                const HostCmd_DS_ExtBLECoex_Config_t *coex = &resp->params.ext_ble_coex_cfg;
                if (coex->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        wifi_ext_coex_stats_t *stats = (wifi_ext_coex_stats_t *)wm_wifi.cmd_resp_priv;
                        stats->ext_radio_req_count   = coex->coex_cfg_data.EXT_RADIO_REQ_count;
                        stats->ext_radio_pri_count   = coex->coex_cfg_data.EXT_RADIO_PRI_count;
                        stats->wlan_grant_count      = coex->coex_cfg_data.WLAN_GRANT_count;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
#endif
            case HostCmd_CMD_11N_CFG:
                rv = wlan_ret_11n_cfg(pmpriv, resp, NULL);
                break;
            case HostCmd_CMD_11N_ADDBA_REQ:
                (void)do_wlan_ret_11n_addba_req(pmpriv, resp);
                break;
            case HostCmd_CMD_11N_DELBA:
                (void)do_wlan_ret_11n_delba(pmpriv, resp);
                break;
            case HostCmd_CMD_11N_ADDBA_RSP:
                (void)do_wlan_ret_11n_addba_resp(resp);
                break;
            case HostCmd_CMD_AMSDU_AGGR_CTRL:
                rv = wlan_ret_amsdu_aggr_ctrl(pmpriv, resp, NULL);
                break;
            case HostCmd_CMD_MAC_MULTICAST_ADR:
                break;
            case HostCmd_CMD_802_11_ASSOCIATE:
            {
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
                if (rv != MLAN_STATUS_SUCCESS)
                {
                    return -WM_FAIL;
                }

                IEEEtypes_AssocRsp_t *passoc_rsp = (IEEEtypes_AssocRsp_t *)(void *)&resp->params;

                if (passoc_rsp->status_code == 0U)
                {
                    result = WIFI_EVENT_REASON_SUCCESS;
                }
                else
                {
#ifdef CONFIG_WIFI_EXTRA_DEBUG
                    wifi_w("Assoc failed: Status: %d\n\r", passoc_rsp->status_code);
                    wifi_show_assoc_fail_reason(passoc_rsp->status_code);
#endif
                    /* Since we have failed assoc attempt clear this */
                    pmpriv->media_connected = MFALSE;
                }

                (void)wifi_event_completion(WIFI_EVENT_ASSOCIATION, result, NULL);
            }
            break;
            case HostCmd_CMD_802_11_MAC_ADDRESS:
            {
                HostCmd_DS_802_11_MAC_ADDRESS *pmac_addr = &resp->params.mac_addr;

                sta_addr = os_mem_alloc(MLAN_MAC_ADDR_LENGTH);
                if (sta_addr == MNULL)
                {
                    wifi_w("No mem. Cannot process MAC address command");
                    break;
                }

                (void)memcpy((void *)sta_addr, (const void *)((uint8_t *)&pmac_addr->mac_addr), MLAN_MAC_ADDR_LENGTH);

                if (wifi_event_completion(WIFI_EVENT_MAC_ADDR_CONFIG, WIFI_EVENT_REASON_SUCCESS, sta_addr) !=
                    WM_SUCCESS)
                {
                    /* If fail to send message on queue, free allocated memory ! */
                    os_mem_free((void *)sta_addr);
                }
            }
            break;
            case HostCmd_CMD_802_11_KEY_MATERIAL:
                break;
            case HostCmd_CMD_MAC_REG_ACCESS:
            {
                HostCmd_DS_MAC_REG_ACCESS *reg;
                reg = (HostCmd_DS_MAC_REG_ACCESS *)&resp->params.mac_reg;
                if (reg->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *mac_reg = (uint32_t *)wm_wifi.cmd_resp_priv;
                        *mac_reg          = reg->value;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_BBP_REG_ACCESS:
            {
                HostCmd_DS_BBP_REG_ACCESS *reg;
                reg = (HostCmd_DS_BBP_REG_ACCESS *)&resp->params.bbp_reg;
                if (reg->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *bbp_reg = (uint32_t *)wm_wifi.cmd_resp_priv;
                        *bbp_reg          = reg->value;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_RF_REG_ACCESS:
            {
                HostCmd_DS_RF_REG_ACCESS *reg;
                reg = (HostCmd_DS_RF_REG_ACCESS *)&resp->params.rf_reg;
                if (reg->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *rf_reg = (uint32_t *)wm_wifi.cmd_resp_priv;
                        *rf_reg          = reg->value;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_802_11_EEPROM_ACCESS:
            {
                HostCmd_DS_802_11_EEPROM_ACCESS *eeprom;
                eeprom = (HostCmd_DS_802_11_EEPROM_ACCESS *)&resp->params.eeprom;
                if (wm_wifi.cmd_resp_priv != NULL)
                {
                    uint8_t *buf = (uint8_t *)wm_wifi.cmd_resp_priv;
                    (void)memcpy((void *)buf, (const void *)&eeprom->value, eeprom->byte_count);
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_MEM_ACCESS:
            {
                HostCmd_DS_MEM_ACCESS *mem;
                mem = (HostCmd_DS_MEM_ACCESS *)&resp->params.mem;
                if (mem->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *mem_value = (uint32_t *)wm_wifi.cmd_resp_priv;
                        *mem_value          = mem->value;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_MGMT_IE_LIST:
            {
                HostCmd_DS_MGMT_IE_LIST_CFG *ie_list_cfg;
                ie_list_cfg = (HostCmd_DS_MGMT_IE_LIST_CFG *)&resp->params.mgmt_ie_list;
                if (wm_wifi.cmd_resp_priv != NULL)
                {
                    uint8_t *buf = (uint8_t *)wm_wifi.cmd_resp_priv;
                    (void)memcpy((void *)buf, (const void *)(&ie_list_cfg->ds_mgmt_ie.ie_data_list[0]),
                                 ie_list_cfg->ds_mgmt_ie.len);
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_VERSION_EXT:
                load_ver_ext(resp);
                break;
            case HostCmd_CMD_802_11_RF_ANTENNA:
            {
                HostCmd_DS_802_11_RF_ANTENNA *rf_antenna_ctrl;
                rf_antenna_ctrl = (HostCmd_DS_802_11_RF_ANTENNA *)&resp->params.antenna;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (rf_antenna_ctrl->action == HostCmd_ACT_GET_BOTH)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            wifi_antcfg_t *antcfg = (wifi_antcfg_t *)wm_wifi.cmd_resp_priv;
                            antcfg->ant_mode      = wlan_cpu_to_le32(rf_antenna_ctrl->antenna_mode);
                            antcfg->evaluate_time = wlan_cpu_to_le16(rf_antenna_ctrl->evaluate_time);
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_CW_MODE_CTRL:
            {
                HostCmd_DS_CW_MODE_CTRL *cw_mode_ctrl;
                cw_mode_ctrl = (HostCmd_DS_CW_MODE_CTRL *)&resp->params.cwmode;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (cw_mode_ctrl->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            wifi_cw_mode_ctrl_t *cwmode_ctrl = (wifi_cw_mode_ctrl_t *)wm_wifi.cmd_resp_priv;
                            cwmode_ctrl->mode                = cw_mode_ctrl->mode;
                            cwmode_ctrl->channel             = cw_mode_ctrl->channel;
                            cwmode_ctrl->chanInfo            = cw_mode_ctrl->chanInfo;
                            cwmode_ctrl->txPower             = wlan_cpu_to_le16(cw_mode_ctrl->txPower);
                            cwmode_ctrl->rateInfo            = wlan_cpu_to_le32(cw_mode_ctrl->rateInfo);
                            cwmode_ctrl->pktLength           = wlan_cpu_to_le16(cw_mode_ctrl->pktLength);
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_RSSI_INFO:
            {
                HostCmd_DS_802_11_RSSI_INFO_RSP *prssi_info_rsp =
                    (HostCmd_DS_802_11_RSSI_INFO_RSP *)(void *)&resp->params;
                if (wm_wifi.cmd_resp_priv != NULL)
                {
                    wifi_rssi_info_t *rssi_info = (wifi_rssi_info_t *)wm_wifi.cmd_resp_priv;
                    compute_rssi_values(prssi_info_rsp, rssi_info);
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_802_11_NET_MONITOR:
                wm_wifi.cmd_resp_status = WM_SUCCESS;
                break;
            case HOST_CMD_APCMD_SYS_CONFIGURE:
                wifi_uap_handle_cmd_resp(resp);
                break;
            case HostCmd_CMD_TXPWR_CFG:
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, wm_wifi.cmd_resp_ioctl);
                if (rv != MLAN_STATUS_SUCCESS)
                {
                    return -WM_FAIL;
                }
                break;
            case HostCmd_CMD_TX_RATE_CFG:
            {
                const HostCmd_DS_TX_RATE_CFG *tx_rate_cfg = &resp->params.tx_rate_cfg;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (tx_rate_cfg->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_ioctl != NULL)
                        {
                            wifi_ds_rate *ds_rate = (wifi_ds_rate *)wm_wifi.cmd_resp_ioctl;
                            rv                    = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, ds_rate);
                            if (rv != MLAN_STATUS_SUCCESS)
                            {
                                wm_wifi.cmd_resp_status = -WM_FAIL;
                            }
                            else
                            {
                                wm_wifi.cmd_resp_status = WM_SUCCESS;
                            }
                        }
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_802_11_SNMP_MIB:
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
                if (rv != MLAN_STATUS_SUCCESS)
                {
                    return -WM_FAIL;
                }
                break;
            case HostCmd_CMD_OTP_READ_USER_DATA:
            {
                const HostCmd_DS_OTP_USER_DATA *user_data = &resp->params.otp_user_data;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (user_data->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            uint8_t *buf = (uint8_t *)wm_wifi.cmd_resp_priv;
                            (void)memcpy((void *)buf, (const void *)user_data->user_data, user_data->user_data_length);
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_CFG_DATA:
            {
                const HostCmd_DS_802_11_CFG_DATA *cfg_data = &resp->params.cfg_data;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (cfg_data->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            wifi_cal_data_t *cal_data_cfg = (wifi_cal_data_t *)wm_wifi.cmd_resp_priv;
                            cal_data_cfg->data            = (uint8_t *)os_mem_alloc(cfg_data->data_len);
                            if (cal_data_cfg->data == MNULL)
                            {
                                wifi_w(
                                    "No mem. Cannot"
                                    "process CAL DATA command");
                                break;
                            }
                            cal_data_cfg->data_len = cfg_data->data_len;

                            (void)memcpy((void *)cal_data_cfg->data, (const void *)cfg_data->data, cfg_data->data_len);
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_AUTO_RECONNECT:
            {
                const HostCmd_DS_AUTO_RECONNECT *auto_reconn = &resp->params.auto_reconnect;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (auto_reconn->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            wifi_auto_reconnect_config_t *auto_reconnect_config =
                                (wifi_auto_reconnect_config_t *)wm_wifi.cmd_resp_priv;

                            auto_reconnect_config->reconnect_counter  = auto_reconn->reconnect_counter;
                            auto_reconnect_config->reconnect_interval = auto_reconn->reconnect_interval;
                            auto_reconnect_config->flags              = auto_reconn->flags;
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_RX_MGMT_IND:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_CHANNEL_TRPC_CONFIG:
            {
                HostCmd_DS_CHAN_TRPC_CONFIG *chan_trpc_cfg = &resp->params.chan_trpc_cfg;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (chan_trpc_cfg->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            t_u8 i;
                            t_u8 mod_num = 0;
                            t_u8 *pByte  = NULL;
                            t_u16 left_len;
                            MrvlIETypes_ChanTRPCConfig_t *trpc_tlv = NULL;
                            MrvlIEtypes_Data_t *pTlvHdr;
                            wifi_txpwrlimit_t *txpwrlimit = (wifi_txpwrlimit_t *)wm_wifi.cmd_resp_priv;

                            txpwrlimit->subband = (wifi_SubBand_t)chan_trpc_cfg->reserved;

                            pByte    = (t_u8 *)chan_trpc_cfg + 4;
                            left_len = resp->size - (t_u16)S_DS_GEN - 4U;
                            while (left_len >= sizeof(pTlvHdr->header))
                            {
                                pTlvHdr             = (MrvlIEtypes_Data_t *)(void *)pByte;
                                pTlvHdr->header.len = wlan_le16_to_cpu(pTlvHdr->header.len);

                                switch (wlan_le16_to_cpu(pTlvHdr->header.type))
                                {
                                    case TLV_TYPE_CHAN_TRPC_CONFIG:
                                        trpc_tlv = (MrvlIETypes_ChanTRPCConfig_t *)(void *)pTlvHdr;
#ifndef CONFIG_11AC
                                        /*
                                         * For 2.4 GHz band, we do not support HT40 Modulation Groups.
                                         * Limit the number of mod groups to 7.
                                         */
                                        if (!txpwrlimit->subband)
                                        {
                                            mod_num = 7;
                                        }
                                        else
                                        {
#endif /* CONFIG_11AC */
                                            mod_num =
                                                ((t_u8)pTlvHdr->header.len - 4U) / ((t_u8)sizeof(mod_group_setting));
#ifndef CONFIG_11AC
                                        }
#endif /* CONFIG_11AC */
                                        txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans].num_mod_grps = mod_num;
                                        txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans].chan_desc.start_freq =
                                            trpc_tlv->start_freq;
                                        txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans].chan_desc.chan_width =
                                            trpc_tlv->width;
                                        txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans].chan_desc.chan_num =
                                            trpc_tlv->chan_num;
                                        for (i = 0; i < mod_num; i++)
                                        {
                                            txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans]
                                                .txpwrlimit_entry[i]
                                                .mod_group = trpc_tlv->mod_group[i].mod_group;
                                            txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans]
                                                .txpwrlimit_entry[i]
                                                .tx_power = trpc_tlv->mod_group[i].power;
                                        }
                                        txpwrlimit->num_chans++;
                                        break;
                                    default:
                                        PRINTM(MINFO, "Unexpected host cmd response \n");
                                        break;
                                }
                                left_len -= (pTlvHdr->header.len + (t_u16)sizeof(pTlvHdr->header));
                                pByte += (t_u8)pTlvHdr->header.len + (t_u8)sizeof(pTlvHdr->header);
                            }
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_802_11_GET_LOG:
            {
                const HostCmd_DS_802_11_GET_LOG *get_log = &resp->params.get_log;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint8_t *buf = (uint8_t *)wm_wifi.cmd_resp_priv;
                        (void)memcpy((void *)buf, (const void *)get_log, sizeof(HostCmd_DS_802_11_GET_LOG));
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_RECONFIGURE_TX_BUFF:
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
                break;
            case HostCmd_CMD_PMF_PARAMS:
            {
                const HostCmd_DS_PMF_PARAMS *get_pmf_params = &resp->params.pmf_params;
                if ((resp->result == HostCmd_RESULT_OK) && (get_pmf_params->action == HostCmd_ACT_GEN_GET))
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        wifi_pmf_params_t *wifi_pmf_params = (wifi_pmf_params_t *)wm_wifi.cmd_resp_priv;

                        wifi_pmf_params->mfpc = get_pmf_params->params.mfpc;
                        wifi_pmf_params->mfpr = get_pmf_params->params.mfpr;
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#ifdef CONFIG_RF_TEST_MODE
            case HostCmd_CMD_MFG_COMMAND:
            {
                const HostCmd_DS_MFG_CMD_GENERIC_CFG *mfg_generic_cfg = &resp->params.mfg_generic_cfg;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (mfg_generic_cfg->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != MNULL)
                        {
                            switch (wlan_le32_to_cpu(mfg_generic_cfg->mfg_cmd))
                            {
                                case MFG_CMD_SET_TEST_MODE:
                                case MFG_CMD_UNSET_TEST_MODE:
                                case MFG_CMD_TX_ANT:
                                case MFG_CMD_RX_ANT:
                                case MFG_CMD_RF_CHAN:
                                case MFG_CMD_CLR_RX_ERR:
                                case MFG_CMD_RF_BAND_AG:
                                case MFG_CMD_RF_CHANNELBW:
                                {
                                    wifi_mfg_cmd_generic_cfg_t *wifi_mfg_cmd_generic_cfg =
                                        (wifi_mfg_cmd_generic_cfg_t *)wm_wifi.cmd_resp_priv;
                                    rv = wlan_ret_mfg(pmpriv, resp, wifi_mfg_cmd_generic_cfg);
                                    if (rv != MLAN_STATUS_SUCCESS)
                                    {
                                        wm_wifi.cmd_resp_status = -WM_FAIL;
                                    }
                                    else
                                    {
                                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                                    }
                                }
                                break;
                                default:
                                    wm_wifi.cmd_resp_status = -WM_FAIL;
                            }
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#endif
            case HostCmd_CMD_GET_TSF:
            {
                const HostCmd_DS_TSF *tsf_pointer = (HostCmd_DS_TSF *)&resp->params.tsf_cfg;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        t_u64 *tsf = (t_u64 *)(wm_wifi.cmd_resp_priv);

                        *tsf = tsf_pointer->tsf;

                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                    }
                    else
                    {
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_AUTO_TX:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#ifdef OTP_CHANINFO
            case HostCmd_CMD_CHAN_REGION_CFG:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
                    if (rv != MLAN_STATUS_SUCCESS)
                    {
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    }
                    else
                    {
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#endif
#ifdef CONFIG_11AX
            case HostCmd_CMD_11AX_CFG:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        mlan_ds_11ax_he_cfg *cfg = (mlan_ds_11ax_he_cfg *)wm_wifi.cmd_resp_priv;
                        rv                       = wlan_ret_11ax_cfg(pmpriv, resp, cfg);
                        if (rv != MLAN_STATUS_SUCCESS)
                            wm_wifi.cmd_resp_status = -WM_FAIL;
                        else
                            wm_wifi.cmd_resp_status = WM_SUCCESS;
                    }
                }
                else
                {
                    rv                      = MLAN_STATUS_FAILURE;
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_11AX_CMD:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, wm_wifi.cmd_resp_ioctl);
                    if (rv != MLAN_STATUS_SUCCESS)
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    else
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    rv                      = MLAN_STATUS_FAILURE;
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#endif
            case HostCmd_CMD_EU_CRYPTO:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        EU_Crypto *EU_Data = (EU_Crypto *)wm_wifi.cmd_resp_priv;
                        HostCmd_DS_EU_AES_CRYPTO *cmd_aes_ccmp =
                            (HostCmd_DS_EU_AES_CRYPTO *)&resp->params.eu_aes_crypto;
                        HostCmd_DS_EU_CRYPTO *cmd_eu_crypto = (HostCmd_DS_EU_CRYPTO *)&resp->params.eu_crypto;
                        if (cmd_eu_crypto->Algorithm == CRYPTO_RC4 || cmd_eu_crypto->Algorithm == CRYPTO_AES_ECB ||
                            cmd_eu_crypto->Algorithm == CRYPTO_AES_WRAP)
                        {
                            memcpy(EU_Data->DataLength, &cmd_eu_crypto->DataLength, sizeof(t_u16) / sizeof(t_u8));
                            memcpy(EU_Data->Data, cmd_eu_crypto->Data, cmd_eu_crypto->DataLength);
                        }
                        else if (cmd_aes_ccmp->Algorithm == CRYPTO_AES_CCMP ||
                                 cmd_aes_ccmp->Algorithm == CRYPTO_AES_GCMP)
                        {
                            memcpy(EU_Data->DataLength, &cmd_aes_ccmp->DataLength, sizeof(t_u16) / sizeof(t_u8));
                            memcpy(EU_Data->Data, cmd_aes_ccmp->Data, cmd_aes_ccmp->DataLength);
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    rv                      = MLAN_STATUS_FAILURE;
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            default:
                /* fixme: Currently handled by the legacy code. Change this
                   handling later. Also check the default return value then*/
                break;
        }
    }

    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

/**
 *  @brief This function will search for the specific ie
 *
 *
 *  @param priv    A pointer to mlan_private
 *  @param ie_buf  A pointer to ie_buf
 *  @param ie_len  total ie length
 *  @param id      ie's id
 *
 *  @return	       ie's poiner or MNULL
 */
static t_u8 *wlan_get_specific_ie(pmlan_private priv, t_u8 *ie_buf, t_u8 ie_len, IEEEtypes_ElementId_e id, t_u8 ext_id)
{
    t_u8 bytes_left    = ie_len;
    t_u8 *pcurrent_ptr = ie_buf;
    t_u8 total_ie_len;
    t_u8 *ie_ptr = MNULL;
    IEEEtypes_ElementId_e element_id;
    t_u8 element_len;
    t_u8 element_eid;
    ENTER();

    DBG_HEXDUMP(MCMD_D, "ie", ie_buf, ie_len);
    while (bytes_left >= 2U)
    {
        element_id   = (IEEEtypes_ElementId_e)(*((t_u8 *)pcurrent_ptr));
        element_len  = *((t_u8 *)pcurrent_ptr + 1);
        element_eid  = *((t_u8 *)pcurrent_ptr + 2);
        total_ie_len = element_len + (t_u8)sizeof(IEEEtypes_Header_t);
        if (bytes_left < total_ie_len)
        {
            PRINTM(MERROR,
                   "InterpretIE: Error in processing IE, "
                   "bytes left < IE length\n");
            break;
        }
        if (((ext_id == 0U) && element_id == id) || (id == EXTENSION && element_id == id && ext_id == element_eid))
        {
            PRINTM(MCMND, "Find IE: id=%d ext_id=%d\n", id, ext_id);
            DBG_HEXDUMP(MCMND, "IE", pcurrent_ptr, total_ie_len);
            ie_ptr = pcurrent_ptr;
            break;
        }
        pcurrent_ptr += element_len + 2U;
        /* Need to account for IE ID and IE Len */
        bytes_left -= (element_len + 2U);
    }

    LEAVE();

    return ie_ptr;
}

/**
 *  @brief This function will search for the specific ie
 *
 *
 *  @param priv    A pointer to mlan_private
 *  @param pevent  A pointer to event buf
 *  @param sta_ptr A pointer to sta_node
 *
 *  @return	       N/A
 */
static void wrapper_wlan_check_sta_capability(pmlan_private priv, Event_Ext_t *pevent, sta_node *sta_ptr)
{
    t_u16 tlv_type, tlv_len;
    t_u16 frame_control, frame_sub_type = 0;
    t_u8 *assoc_req_ie = MNULL;
    t_u8 ie_len = 0, assoc_ie_len = 0;
    IEEEtypes_HTCap_t *pht_cap = MNULL;
#ifdef CONFIG_11AC
    IEEEtypes_VHTCap_t *pvht_cap = MNULL;
#endif
#ifdef CONFIG_11AX
    IEEEtypes_Extension_t *phe_cap = MNULL;
#endif
    int tlv_buf_left         = (int)pevent->length - (int)INTF_HEADER_LEN - (int)ASSOC_EVENT_FIX_SIZE;
    MrvlIEtypesHeader_t *tlv = (MrvlIEtypesHeader_t *)(void *)((char *)pevent + INTF_HEADER_LEN + ASSOC_EVENT_FIX_SIZE);
    MrvlIETypes_MgmtFrameSet_t *mgmt_tlv = MNULL;

    ENTER();
    while (tlv_buf_left >= (int)sizeof(MrvlIEtypesHeader_t))
    {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len  = wlan_le16_to_cpu(tlv->len);
        if ((sizeof(MrvlIEtypesHeader_t) + tlv_len) > (unsigned int)tlv_buf_left)
        {
            PRINTM(MERROR, "wrong tlv: tlvLen=%d, tlvBufLeft=%d\n", tlv_len, tlv_buf_left);
            break;
        }
        if (tlv_type == TLV_TYPE_UAP_MGMT_FRAME)
        {
            mgmt_tlv = (MrvlIETypes_MgmtFrameSet_t *)(void *)tlv;
            (void)memcpy((void *)&frame_control, (const void *)((t_u8 *)&(mgmt_tlv->frame_control)),
                         sizeof(frame_control));
            frame_sub_type = IEEE80211_GET_FC_MGMT_FRAME_SUBTYPE(frame_control);
            if ((mgmt_tlv->frame_control.type == 0U) &&
                ((frame_sub_type == SUBTYPE_ASSOC_REQUEST) || (frame_sub_type == SUBTYPE_REASSOC_REQUEST)))
            {
                if (frame_sub_type == SUBTYPE_ASSOC_REQUEST)
                {
                    assoc_ie_len = (t_u8)sizeof(IEEEtypes_AssocRqst_t);
                }
                else if (frame_sub_type == SUBTYPE_REASSOC_REQUEST)
                {
                    assoc_ie_len = (t_u8)sizeof(IEEEtypes_ReAssocRqst_t);
                }
                else
                { /* Do Nothing */
                }

                ie_len       = (t_u8)tlv_len - (t_u8)sizeof(IEEEtypes_FrameCtl_t) - assoc_ie_len;
                assoc_req_ie = (t_u8 *)tlv + sizeof(MrvlIETypes_MgmtFrameSet_t) + assoc_ie_len;
                pht_cap =
                    (IEEEtypes_HTCap_t *)(void *)wlan_get_specific_ie(priv, assoc_req_ie, ie_len, HT_CAPABILITY, 0);

                if (pht_cap != NULL)
                {
                    PRINTM(MCMND, "STA supports 11n\n");
                    sta_ptr->is_11n_enabled = MTRUE;
                    if (GETHT_MAXAMSDU(pht_cap->ht_cap.ht_cap_info) != 0U)
                    {
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_8K;
                    }
                    else
                    {
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_4K;
                    }
                }
                else
                {
                    PRINTM(MCMND,
                           "STA doesn't "
                           "support 11n\n");
                }
#ifdef CONFIG_11AC
                pvht_cap =
                    (IEEEtypes_VHTCap_t *)(void *)wlan_get_specific_ie(priv, assoc_req_ie, ie_len, VHT_CAPABILITY, 0);
                if ((pvht_cap != MNULL) && (priv->is_11ac_enabled == MTRUE))
                {
                    PRINTM(MCMND, "STA supports 11ac\n");
                    sta_ptr->is_11ac_enabled = MTRUE;
                    if (GET_VHTCAP_MAXMPDULEN(wlan_le32_to_cpu(pvht_cap->vht_cap.vht_cap_info)) == 2U)
                    {
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_12K;
                    }
                    else if (GET_VHTCAP_MAXMPDULEN(wlan_le32_to_cpu(pvht_cap->vht_cap.vht_cap_info)) == 1U)
                    {
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_8K;
                    }
                    else
                    {
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_4K;
                    }
                }
                else
                {
                    PRINTM(MCMND, "STA doesn't support 11ac\n");
                }
#endif
#ifdef CONFIG_11AX
                phe_cap =
                    (IEEEtypes_Extension_t *)wlan_get_specific_ie(priv, assoc_req_ie, ie_len, EXTENSION, HE_CAPABILITY);
                if (phe_cap && (priv->is_11ax_enabled == MTRUE))
                {
                    PRINTM(MCMND, "STA supports 11ax\n");
                    sta_ptr->is_11ax_enabled = MTRUE;
                    (void)memcpy((void *)((t_u8 *)&sta_ptr->he_cap), (const void *)phe_cap,
                                 phe_cap->ieee_hdr.len + sizeof(IEEEtypes_Header_t));
                    sta_ptr->he_cap.ieee_hdr.len =
                        MIN(phe_cap->ieee_hdr.len, sizeof(IEEEtypes_HECap_t) - sizeof(IEEEtypes_Header_t));
                }
                else
                    PRINTM(MCMND, "STA doesn't support 11ax\n");
#endif
                break;
            }
        }
        tlv_buf_left -= (int)sizeof(MrvlIEtypesHeader_t) + (int)tlv_len;
        tlv = (MrvlIEtypesHeader_t *)(void *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
    }
    LEAVE();

    return;
}

/** Fixed size of bss start event */
#define BSS_START_EVENT_FIX_SIZE 12U

/**
 *  @brief This function will search for the specific ie
 *
 *
 *  @param priv    A pointer to mlan_private
 *  @param pevent  A pointer to event buf
 *
 *  @return           N/A
 */
static void wrapper_wlan_check_uap_capability(pmlan_private priv, Event_Ext_t *pevent)
{
    t_u16 tlv_type, tlv_len;
    int tlv_buf_left = (int)pevent->length - (int)INTF_HEADER_LEN - (int)BSS_START_EVENT_FIX_SIZE;
    MrvlIEtypesHeader_t *tlv =
        (MrvlIEtypesHeader_t *)(void *)((char *)pevent + INTF_HEADER_LEN + BSS_START_EVENT_FIX_SIZE);
#ifdef CONFIG_11AX
    MrvlIEtypes_He_cap_t *pext_tlv = MNULL;
#endif
    priv->wmm_enabled    = MFALSE;
    priv->pkt_fwd        = 0U;
    priv->is_11n_enabled = MFALSE;
#ifdef CONFIG_11AC
    priv->is_11ac_enabled = MFALSE;
#endif
#ifdef CONFIG_11AX
    priv->is_11ax_enabled = MFALSE;
#endif
    ENTER();

    while (tlv_buf_left >= (int)sizeof(MrvlIEtypesHeader_t))
    {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len  = wlan_le16_to_cpu(tlv->len);
        if ((sizeof(MrvlIEtypesHeader_t) + tlv_len) > (unsigned int)tlv_buf_left)
        {
            PRINTM(MERROR, "wrong tlv: tlvLen=%d, tlvBufLeft=%d\n", tlv_len, tlv_buf_left);
            break;
        }
        if (tlv_type == (t_u16)HT_CAPABILITY)
        {
            DBG_HEXDUMP(MCMD_D, "HT_CAP tlv", tlv, tlv_len + sizeof(MrvlIEtypesHeader_t));
            priv->is_11n_enabled = MTRUE;
        }
#ifdef CONFIG_11AC
        if (tlv_type == (t_u16)VHT_CAPABILITY)
        {
            DBG_HEXDUMP(MCMD_D, "VHT_CAP tlv", tlv, tlv_len + sizeof(MrvlIEtypesHeader_t));
            priv->is_11ac_enabled = MTRUE;
        }
#endif
#ifdef CONFIG_11AX
        if (tlv_type == EXTENSION)
        {
            pext_tlv = (MrvlIEtypes_He_cap_t *)tlv;
            if (pext_tlv->ext_id == HE_CAPABILITY)
            {
                DBG_HEXDUMP(MCMD_D, "HE_CAP tlv", tlv, tlv_len + sizeof(MrvlIEtypesHeader_t));
                priv->is_11ax_enabled = MTRUE;
            }
        }
#endif
        if (tlv_type == (t_u16)TLV_TYPE_UAP_PKT_FWD_CTL)
        {
            DBG_HEXDUMP(MCMD_D, "pkt_fwd tlv", tlv, tlv_len + sizeof(MrvlIEtypesHeader_t));
            priv->pkt_fwd = *((t_u8 *)tlv + sizeof(MrvlIEtypesHeader_t));
            PRINTM(MCMND, "pkt_fwd FW: 0x%x\n", priv->pkt_fwd);
            if ((priv->pkt_fwd & PKT_FWD_FW_BIT) != 0U)
            {
                priv->pkt_fwd = 0U;
            }
            else
            {
                priv->pkt_fwd |= PKT_FWD_ENABLE_BIT;
            }
            PRINTM(MCMND, "pkt_fwd DRV: 0x%x\n", priv->pkt_fwd);
        }
        tlv_buf_left -= (int)sizeof(MrvlIEtypesHeader_t) + (int)tlv_len;
        tlv = (MrvlIEtypesHeader_t *)(void *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
    }
    LEAVE();
}


/* fixme: duplicated from legacy. needs to be cleaned up later */
#define IEEEtypes_REASON_UNSPEC             1U
#define IEEEtypes_REASON_PRIOR_AUTH_INVALID 2U
#define IEEEtypes_REASON_DEAUTH_LEAVING     3
#define AP_DEAUTH_REASON_MAC_ADDR_BLOCKED   6U



int wifi_handle_fw_event(struct bus_message *msg)
{
    mlan_private *pmpriv     = (mlan_private *)mlan_adap->priv[0];
    mlan_private *pmpriv_uap = (mlan_private *)mlan_adap->priv[1];
#ifdef CONFIG_EXT_SCAN_SUPPORT
    mlan_status rv = MLAN_STATUS_SUCCESS;
#endif

    Event_Ext_t *evt = ((Event_Ext_t *)msg->data);
    t_u8 *sta_addr = NULL, *event_sta_addr = NULL, *new_channel = NULL;

    sta_node *sta_node_ptr;
#ifdef CONFIG_EXT_SCAN_SUPPORT
    mlan_event_scan_result *pext_scan_result;
#endif
    if (evt == NULL)
    {
        wevt_d("No mem allocated for msg.data");
        return -WM_FAIL;
    }
    wevt_d("EVENT - : 0x%x Len : %d Reason: %d", evt->event_id, evt->length, evt->reason_code);

    if ((evt->event_id != EVENT_PS_AWAKE) && (mlan_adap->ps_state == PS_STATE_SLEEP))
    {
        os_rwlock_write_unlock(&sleep_rwlock);
        pmpriv->adapter->ps_state = PS_STATE_AWAKE;
    }

    switch (evt->event_id)
    {
        case EVENT_LINK_LOST:
            (void)wifi_event_completion(WIFI_EVENT_LINK_LOSS, WIFI_EVENT_REASON_FAILURE,
                                        (void *)IEEEtypes_REASON_DEAUTH_LEAVING);
            /* fixme: Should this be outside CONFIG_11N ? */
            wlan_handle_disconnect_event(pmpriv);
            break;
        case EVENT_DEAUTHENTICATED:
            if (evt->reason_code == 0U)
            {
                (void)wifi_event_completion(WIFI_EVENT_LINK_LOSS, WIFI_EVENT_REASON_FAILURE,
                                            (void *)IEEEtypes_REASON_DEAUTH_LEAVING);
            }
            else
            {
                (void)wifi_event_completion(WIFI_EVENT_AUTHENTICATION, WIFI_EVENT_REASON_FAILURE,
                                            (void *)&evt->reason_code);
            }
            /* fixme: Should this be outside CONFIG_11N ? */
            wlan_handle_disconnect_event(pmpriv);
            break;
        case EVENT_DISASSOCIATED:
            (void)wifi_event_completion(WIFI_EVENT_DISASSOCIATION, WIFI_EVENT_REASON_FAILURE,
                                        (void *)IEEEtypes_REASON_DEAUTH_LEAVING);
            /* fixme: Should this be outside CONFIG_11N ? */
            wlan_handle_disconnect_event(pmpriv);
            break;
        case EVENT_PORT_RELEASE:
            (void)wifi_event_completion(WIFI_EVENT_AUTHENTICATION, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_PS_SLEEP:
            wevt_d("_");
            if (mlan_adap->ps_state != PS_STATE_PRE_SLEEP)
            {
                mlan_adap->ps_state = PS_STATE_PRE_SLEEP;
                if (split_scan_in_progress == false)
                {
                    (void)wifi_event_completion(WIFI_EVENT_SLEEP, WIFI_EVENT_REASON_SUCCESS, NULL);
                }
                else
                {
                    /** Do Nothing */
                }
            }
            else
            {
                /* Unexpected PS SLEEP event */
                wevt_w("Receive PS SLEEP event when presleep: %d", mlan_adap->ps_state);
            }
            break;
        case EVENT_PS_AWAKE:
            wevt_d("|");
            if (mlan_adap->ps_state == PS_STATE_SLEEP)
            {
                os_rwlock_write_unlock(&sleep_rwlock);
                mlan_adap->ps_state = PS_STATE_AWAKE;
            }
            else if (mlan_adap->ps_state == PS_STATE_PRE_SLEEP)
            {
                /* If driver did not send out sleep confirm in the expected time,
                   FW would take it as timeout, switch to awake and send out PS AWAKE event */
                wevt_w("Receive PS AWAKE event when presleep: %d", mlan_adap->ps_state);
                os_rwlock_write_unlock(&sleep_rwlock);
                mlan_adap->ps_state = PS_STATE_AWAKE;
            }
            else
            {
                /*Do Nothing*/
            }
            break;
        case EVENT_MIC_ERR_MULTICAST:
            (void)wifi_event_completion(WIFI_EVENT_ERR_MULTICAST, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_MIC_ERR_UNICAST:
            (void)wifi_event_completion(WIFI_EVENT_ERR_UNICAST, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_HS_ACT_REQ:
            (void)wifi_event_completion(WIFI_EVENT_HS_CONFIG, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_RSSI_LOW:
            (void)wifi_event_completion(WIFI_EVENT_RSSI_LOW, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_ADDBA:
        {
            void *saved_event_buff = wifi_11n_save_request(evt);
            if (saved_event_buff != NULL)
            {
                if (wifi_event_completion(WIFI_EVENT_11N_ADDBA, WIFI_EVENT_REASON_SUCCESS, saved_event_buff) !=
                    WM_SUCCESS)
                {
                    /* If fail to send message on queue, free allocated memory ! */
                    os_mem_free((void *)saved_event_buff);
                }
            }
            /* If allocation failed ignore this event quietly ! */
        }
        break;
        case EVENT_DELBA:
        {
            if (evt->bss_type == BSS_TYPE_STA)
            {
                void *saved_event_buff = wifi_11n_save_request(evt);
                if (saved_event_buff != NULL)
                {
                    (void)wifi_event_completion(WIFI_EVENT_11N_DELBA, WIFI_EVENT_REASON_SUCCESS, saved_event_buff);
                }

                /* If allocation failed ignore this event quietly ! */
            }
        }
        break;
        case EVENT_BA_STREAM_TIMEOUT:
        {
            void *saved_event_buff = wifi_11n_save_request(evt);
            if (saved_event_buff != NULL)
            {
                if (wifi_event_completion(WIFI_EVENT_11N_BA_STREAM_TIMEOUT, WIFI_EVENT_REASON_SUCCESS,
                                          saved_event_buff) != WM_SUCCESS)
                {
                    /* If fail to send message on queue, free allocated memory ! */
                    os_mem_free((void *)saved_event_buff);
                }
            }
            /* If allocation failed ignore this event quietly ! */
        }
        break;
        case EVENT_AMSDU_AGGR_CTRL:
            /*
             * Currently this event buffer isn't being used. Enable
             * this when required.
             */
            /* wifi_11n_save_request(evt); */
            (void)wifi_event_completion(WIFI_EVENT_11N_AGGR_CTRL, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_CHANNEL_SWITCH_ANN:
            (void)wifi_event_completion(WIFI_EVENT_CHAN_SWITCH_ANN, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_CHANNEL_SWITCH:
        {
            MrvlIEtypes_channel_band_t *tlv = (MrvlIEtypes_channel_band_t *)(void *)&evt->reason_code;

            new_channel = os_mem_alloc(sizeof(t_u8));
            if (new_channel == MNULL)
            {
                wifi_w("No mem. Cannot process new channel from channel switch");
                break;
            }

            *new_channel = tlv->channel;

            if (wifi_event_completion(WIFI_EVENT_CHAN_SWITCH, WIFI_EVENT_REASON_SUCCESS, new_channel) != WM_SUCCESS)
            {
                /* If fail to send message on queue, free allocated memory ! */
                os_mem_free((void *)new_channel);
            }
        }
        break;
        case EVENT_MICRO_AP_STA_ASSOC:
        {
            /*
             * Alloc memory to store the STA mac id. This will be
             * passed to event receiver thread. Freeing this is
             * responsibility of the receiving thread.
             */
            sta_addr = os_mem_alloc(MLAN_MAC_ADDR_LENGTH);
            if (sta_addr == MNULL)
            {
                wifi_w("No mem. Cannot process MAC address from assoc");
                break;
            }

            event_sta_addr = (t_u8 *)&evt->src_mac_addr;
            (void)memcpy((void *)sta_addr, (const void *)event_sta_addr, MLAN_MAC_ADDR_LENGTH);

            wlan_update_uap_ampdu_info(sta_addr, 1);

            sta_node_ptr = os_mem_alloc(sizeof(sta_node));
            if (sta_node_ptr == MNULL)
            {
                wifi_w("No mem. Cannot check station type");
                os_mem_free(sta_addr);
                break;
            }

            wrapper_wlan_check_sta_capability((mlan_private *)mlan_adap->priv[1], msg->data, sta_node_ptr);

            if (sta_node_ptr->is_11n_enabled == MFALSE)
            {
                wlan_update_uap_ampdu_supported(sta_addr, MFALSE);
            }

            os_mem_free(sta_node_ptr);

            if (wifi_event_completion(WIFI_EVENT_UAP_CLIENT_ASSOC, WIFI_EVENT_REASON_SUCCESS, sta_addr) != WM_SUCCESS)
            {
                /* If fail to send message on queue, free allocated memory ! */
                os_mem_free((void *)sta_addr);
            }
        }
        break;
        case EVENT_MICRO_AP_STA_DEAUTH:
            if ((evt->reason_code == IEEEtypes_REASON_PRIOR_AUTH_INVALID) ||
                (evt->reason_code == IEEEtypes_REASON_UNSPEC))
            {
                /*
                 * Alloc memory to store the STA mac id. This will be
                 * passed to event receiver thread. Freeing this is
                 * responsibility of the receiving thread.
                 */
                sta_addr = os_mem_alloc(MLAN_MAC_ADDR_LENGTH);
                if (sta_addr == MNULL)
                {
                    wifi_w("No mem. Cannot process MAC address from assoc");
                    break;
                }
                event_sta_addr = (t_u8 *)&evt->src_mac_addr;
                (void)memcpy((void *)sta_addr, (const void *)event_sta_addr, MLAN_MAC_ADDR_LENGTH);
                if (wifi_event_completion(WIFI_EVENT_UAP_CLIENT_DEAUTH, WIFI_EVENT_REASON_SUCCESS, sta_addr) !=
                    WM_SUCCESS)
                {
                    /* If fail to send message on queue, free allocated memory ! */
                    os_mem_free((void *)sta_addr);
                }
            }
            wlan_update_uap_ampdu_info(evt->src_mac_addr, 0);
            if (evt->reason_code == AP_DEAUTH_REASON_MAC_ADDR_BLOCKED)
            {
                wevt_d("EVENT: Blacklist sta %02x:%02x:%02x:%02x:%02x:%02x: try to join the network \r\n",
                       evt->src_mac_addr[0], evt->src_mac_addr[1], evt->src_mac_addr[2], evt->src_mac_addr[3],
                       evt->src_mac_addr[4], evt->src_mac_addr[5]);
            }
            break;
        case EVENT_MICRO_AP_BSS_START:
            wifi_d("uAP start event received");
            /*
             * wmsdk: statement copied from
             * mlan_uap_cmdevent.c. Necessary for other uAP functions.
             */
            wrapper_wlan_check_uap_capability((mlan_private *)mlan_adap->priv[1], msg->data);
            pmpriv->uap_bss_started = MTRUE;
            break;
        case EVENT_MICRO_AP_BSS_ACTIVE:
            PRINTM(MEVENT, "EVENT: MICRO_AP_BSS_ACTIVE\n");
            pmpriv_uap->media_connected = MTRUE;
            break;
        case EVENT_MICRO_AP_BSS_IDLE:
            PRINTM(MEVENT, "EVENT: MICRO_AP_BSS_IDLE\n");
            pmpriv_uap->media_connected = MFALSE;
            break;
#ifdef CONFIG_EXT_SCAN_SUPPORT
        case EVENT_EXT_SCAN_REPORT:
            pext_scan_result = (mlan_event_scan_result *)((t_u8 *)msg->data + 4);

            rv = wlan_handle_event_ext_scan_report(pmpriv, (t_u8 *)pext_scan_result);

            if (rv != MLAN_STATUS_SUCCESS)
                return -WM_FAIL;

            if (is_split_scan_complete() && !pext_scan_result->more_event)
            {
                wifi_d("Split scan complete");
                wifi_user_scan_config_cleanup();
                (void)wifi_event_completion(WIFI_EVENT_SCAN_RESULT, WIFI_EVENT_REASON_SUCCESS, NULL);
            }
            break;
#endif
#ifdef CONFIG_WIFI_FW_DEBUG
        case EVENT_FW_DEBUG_INFO:
        {
            t_u8 *debug = (t_u8 *)os_mem_alloc(FW_DEBUG_INFO_SIZE);
            if (!debug)
            {
                wifi_w("No mem. Cannot print debug event");
                break;
            }

            (void)memcpy((void *)debug, (const void *)((uint8_t *)&evt->reason_code), evt->length - 8);
            if (wifi_event_completion(WIFI_EVENT_FW_DEBUG_INFO, WIFI_EVENT_REASON_SUCCESS, debug) != WM_SUCCESS)
            {
                /* If fail to send message on queue, free allocated memory ! */
                os_mem_free((void *)debug);
            }
        }
        break;
#endif
        case EVENT_MEF_HOST_WAKEUP:
            wifi_d("Host recevied host wake-up event from firmware");
            break;
        default:
            wifi_d(
                "Event %x not implemented."
                " Please report the event hex value.\n\r",
                evt->event_id);
            break;
    }

    return WM_SUCCESS;
}

// static const uint8_t wpa_oui_none[4] = {0x00, 0x50, 0xf2, 0x00};
static const uint8_t wpa_oui01[4] = {0x00, 0x50, 0xf2, 0x01};
static const uint8_t wpa_oui02[4] = {0x00, 0x50, 0xf2, 0x02};
// static const uint8_t wpa_oui03[4]    = {0x00, 0x50, 0xf2, 0x03};
static const uint8_t wpa_oui04[4] = {0x00, 0x50, 0xf2, 0x04};
static const uint8_t wpa_oui05[4] = {0x00, 0x50, 0xf2, 0x05};
// static const uint8_t wpa_oui06[4]    = {0x00, 0x50, 0xf2, 0x06};

static const uint8_t wpa2_oui01[4] = {0x00, 0x0f, 0xac, 0x01};
static const uint8_t wpa2_oui02[4] = {0x00, 0x0f, 0xac, 0x02};
// static const uint8_t wpa2_oui03[4] = {0x00, 0x0f, 0xac, 0x03};
static const uint8_t wpa2_oui04[4] = {0x00, 0x0f, 0xac, 0x04};
static const uint8_t wpa2_oui05[4] = {0x00, 0x0f, 0xac, 0x05};
static const uint8_t wpa2_oui06[4] = {0x00, 0x0f, 0xac, 0x06};

static const uint8_t wpa3_oui08[4] = {0x00, 0x0f, 0xac, 0x08};
static const uint8_t wpa3_oui12[4] = {0x00, 0x0f, 0xac, 0x12};


static unsigned char process_rsn_ie(
    uint8_t *rsn_ie, _Cipher_t *mcstCipher, _Cipher_t *ucstCipher, bool *is_pmf_required, _SecurityMode_t *WPA_WPA2_WEP)
{
    IEEEtypes_Rsn_t *prsn_ie = (IEEEtypes_Rsn_t *)(void *)rsn_ie;
    uint16_t akmp_count      = 0;
    uint8_t akmp, i;

    if (prsn_ie->pairwise_cipher.count == 2U)
    {
        prsn_ie->len -= sizeof(wpa_suite);
        prsn_ie->pairwise_cipher.count = 1;

        if (!memcmp((const void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite)), wpa2_oui04,
                    sizeof(wpa_suite)))
        {
            (void)memcpy((void *)((uint8_t *)&prsn_ie->pairwise_cipher.list),
                         (const void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite)),
                         sizeof(wpa_suite));
        }
        (void)memmove((((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite)),
                      (uint8_t *)&prsn_ie->auth_key_mgmt, sizeof(wpa_suite_auth_key_mgmt_t));
    }

    if (prsn_ie->pairwise_cipher.count == 1U)
    {
        (void)memcpy((void *)&akmp_count,
                     (const void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite)), sizeof(uint16_t));

        for (i = 0; i < akmp_count; i++)
        {
            if (!memcmp((const void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite) +
                                       sizeof(uint16_t) + sizeof(wpa_suite) * i),
                        (const void *)wpa3_oui08, sizeof(wpa_suite)))
            {
                WPA_WPA2_WEP->wpa3_sae = 1;
            }
            if (!memcmp((const void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite) +
                                       sizeof(uint16_t) + sizeof(wpa_suite) * i),
                        (const void *)wpa3_oui12, sizeof(wpa_suite)))
            {
                WPA_WPA2_WEP->owe = 1;
            }
            if (!memcmp((const void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite) +
                                       sizeof(uint16_t) + sizeof(wpa_suite) * i),
                        (const void *)wpa2_oui02, sizeof(wpa_suite)))
            {
                WPA_WPA2_WEP->wpa2 = 1;
            }
            if (!memcmp((const void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite) +
                                       sizeof(uint16_t) + sizeof(wpa_suite) * i),
                        (const void *)wpa2_oui06, sizeof(wpa_suite)))
            {
                WPA_WPA2_WEP->wpa2 = 1;
            }
        }
    }
    if (akmp_count == 2U && (WPA_WPA2_WEP->wpa3_sae == 1U
                             ))
    {
        prsn_ie->len = 20;
        akmp_count   = 1;
        (void)memcpy((void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite)),
                     (const void *)&akmp_count, sizeof(uint16_t));

        if (WPA_WPA2_WEP->wpa3_sae != 0U)
        {
            (void)memcpy((void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite) + sizeof(uint16_t)),
                         (const void *)wpa3_oui08, sizeof(wpa_suite));
        }
        (void)memcpy(
            (void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + 2U * sizeof(wpa_suite) + sizeof(uint16_t)),
            (const void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + 3U * sizeof(wpa_suite) + sizeof(uint16_t)),
            sizeof(uint16_t));
    }

    if (akmp_count == 3U && WPA_WPA2_WEP->wpa3_sae)
    {
        prsn_ie->len = 20;
        akmp_count   = 1;
        (void)memcpy((void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite)),
                     (const void *)&akmp_count, sizeof(uint16_t));

        if (WPA_WPA2_WEP->wpa3_sae != 0U)
        {
            (void)memcpy((void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite) + sizeof(uint16_t)),
                         (const void *)wpa3_oui08, sizeof(wpa_suite));
        }
        (void)memcpy(
            (void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + 2U * sizeof(wpa_suite) + sizeof(uint16_t)),
            (const void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + 4U * sizeof(wpa_suite) + sizeof(uint16_t)),
            sizeof(uint16_t));
    }

    if (!memcmp((const void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + sizeof(wpa_suite) + sizeof(uint16_t)),
                (const void *)wpa2_oui06, sizeof(wpa_suite)))
    {
        (void)memcpy(
            (void *)&akmp,
            (const void *)(((uint8_t *)&prsn_ie->pairwise_cipher.list) + 2U * sizeof(wpa_suite) + sizeof(uint16_t)),
            sizeof(uint8_t));
        if ((akmp & 0xC0U) != 0)
        {
            *is_pmf_required = true;
        }
    }

    if (!memcmp((const void *)&prsn_ie->pairwise_cipher.list, (const void *)wpa2_oui04, sizeof(wpa_suite)))
    {
        ucstCipher->ccmp = true;
    }
    else if (!memcmp((const void *)&prsn_ie->pairwise_cipher.list, (const void *)wpa2_oui02, sizeof(wpa_suite)))
    {
        ucstCipher->tkip = true;
    }
    else
    { /* Do Nothing */
    }

    if (!memcmp((const void *)&prsn_ie->group_cipher, (const void *)wpa2_oui04, sizeof(wpa_suite)))
    {
        mcstCipher->ccmp = true;
    }
    else if (!memcmp((const void *)&prsn_ie->group_cipher, (const void *)wpa2_oui02, sizeof(wpa_suite)))
    {
        mcstCipher->tkip = true;
    }
    else if (!memcmp((const void *)&prsn_ie->group_cipher, (const void *)wpa2_oui05, sizeof(wpa_suite)))
    {
        mcstCipher->wep104 = true;
    }
    else if (!memcmp((const void *)&prsn_ie->group_cipher, (const void *)wpa2_oui01, sizeof(wpa_suite)))
    {
        mcstCipher->wep40 = true;
    }
    else
    { /* Do nothing */
    }

    return prsn_ie->len + 2U;
}

static unsigned char process_wpa_ie(uint8_t *wpa_ie, _Cipher_t *mcstCipher, _Cipher_t *ucstCipher)
{
    IEEEtypes_Wpa_t *pwpa_ie = (IEEEtypes_Wpa_t *)(void *)wpa_ie;

    if (pwpa_ie->pairwise_cipher.count == 2U)
    {
        pwpa_ie->len                   = 22;
        pwpa_ie->pairwise_cipher.count = 1;

        if (!memcmp((const void *)(((uint8_t *)&pwpa_ie->pairwise_cipher.list) + sizeof(wpa_suite)),
                    (const void *)wpa_oui04, sizeof(wpa_suite)))
        {
            (void)memcpy((void *)((uint8_t *)&pwpa_ie->pairwise_cipher.list),
                         (const void *)(((uint8_t *)&pwpa_ie->pairwise_cipher.list) + sizeof(wpa_suite)),
                         sizeof(wpa_suite));
        }

        (void)memmove((((uint8_t *)&pwpa_ie->pairwise_cipher.list) + sizeof(wpa_suite)),
                      (uint8_t *)&pwpa_ie->auth_key_mgmt, sizeof(wpa_suite_auth_key_mgmt_t));
    }

    if (!memcmp((const void *)&pwpa_ie->pairwise_cipher.list, (const void *)wpa_oui04, sizeof(wpa_suite)))
    {
        ucstCipher->ccmp = true;
    }
    else if (!memcmp((const void *)&pwpa_ie->pairwise_cipher.list, (const void *)wpa_oui02, sizeof(wpa_suite)))
    {
        ucstCipher->tkip = true;
    }
    else
    { /* Do Nothing */
    }

    if (!memcmp((const void *)&pwpa_ie->group_cipher, (const void *)wpa_oui04, sizeof(wpa_suite)))
    {
        mcstCipher->ccmp = true;
    }
    else if (!memcmp((const void *)&pwpa_ie->group_cipher, (const void *)wpa_oui02, sizeof(wpa_suite)))
    {
        mcstCipher->tkip = true;
    }
    else if (!memcmp((const void *)&pwpa_ie->group_cipher, (const void *)wpa_oui05, sizeof(wpa_suite)))
    {
        mcstCipher->wep104 = true;
    }
    else if (!memcmp((const void *)&pwpa_ie->group_cipher, (const void *)wpa_oui01, sizeof(wpa_suite)))
    {
        mcstCipher->wep40 = true;
    }
    else
    { /* Do Nothing */
    }

    return pwpa_ie->len + 2U;
}

int wrapper_bssdesc_first_set(int bss_index,
                              uint8_t *BssId,
                              bool *is_ibss_bit_set,
                              int *ssid_len,
                              uint8_t *ssid,
                              uint8_t *Channel,
                              uint8_t *RSSI,
                              uint16_t *beacon_period,
                              uint16_t *dtim_period,
                              _SecurityMode_t *WPA_WPA2_WEP,
                              _Cipher_t *wpa_mcstCipher,
                              _Cipher_t *wpa_ucstCipher,
                              _Cipher_t *rsn_mcstCipher,
                              _Cipher_t *rsn_ucstCipher,
                              bool *is_pmf_required)
{
    if (bss_index >= (int)mlan_adap->num_in_scan_table)
    {
        wifi_w("Unable to find given entry %d in BSS table", bss_index);
        return -WM_FAIL;
    }

    /* TODO
     * Removed : const BSSDescriptor_t *d = &mlan_adap->pscan_table[bss_index]; */
    BSSDescriptor_t *d = &mlan_adap->pscan_table[bss_index];

    (void)memcpy((void *)BssId, (const void *)d->mac_address, MLAN_MAC_ADDR_LENGTH);

    if (d->cap_info.ibss != 0U)
    {
        *is_ibss_bit_set = true;
    }
    else
    {
        *is_ibss_bit_set = false;
    }

    if (d->ssid.ssid_len <= MLAN_MAX_SSID_LENGTH)
    {
        *ssid_len = (int)d->ssid.ssid_len;
        (void)memcpy((void *)ssid, (const void *)d->ssid.ssid, d->ssid.ssid_len);
    }

    *Channel = d->channel; /*!< Channel associated to the BSSID */

    *RSSI = (uint8_t)d->rssi;
    /* uint8_t RSSI; */ /*!< Received signal strength */
    *beacon_period = d->beacon_period;
    *dtim_period   = d->dtim_period;

    if (d->pwpa_ie != MNULL || d->prsn_ie != MNULL)
    {
        if (d->pwpa_ie != MNULL)
        {
            WPA_WPA2_WEP->wpa = 1;
        }
    }
    else
    {
        /* Check if WEP */
        if (d->cap_info.privacy != 0U)
        {
            WPA_WPA2_WEP->wepStatic = 1;
        }
    }

    if (d->pwpa_ie != NULL)
    {
        d->wpa_ie_buff_len = process_wpa_ie(d->wpa_ie_buff, wpa_mcstCipher, wpa_ucstCipher);
    }

    if (d->prsn_ie != NULL)
    {
        d->rsn_ie_buff_len =
            process_rsn_ie(d->rsn_ie_buff, rsn_mcstCipher, rsn_ucstCipher, is_pmf_required, WPA_WPA2_WEP);
    }

    return WM_SUCCESS;
}

int wrapper_bssdesc_second_set(int bss_index,
                               bool *phtcap_ie_present,
                               bool *phtinfo_ie_present,
                               bool *wmm_ie_present,
                               uint16_t *band,
                               bool *wps_IE_exist,
                               uint16_t *wps_session,
                               bool *wpa2_entp_IE_exist,
                               uint8_t *trans_mode,
                               uint8_t *trans_bssid,
                               int *trans_ssid_len,
                               uint8_t *trans_ssid)
{
    if (bss_index >= mlan_adap->num_in_scan_table)
    {
        wifi_w("Unable to find given entry %d in BSS table", bss_index);
        return -WM_FAIL;
    }
    const BSSDescriptor_t *d = &mlan_adap->pscan_table[bss_index];

    if (d->pht_cap != NULL)
    {
        *phtcap_ie_present = true;
    }
    else
    {
        *phtcap_ie_present = false;
    }

    if (d->pht_info != NULL)
    {
        *phtinfo_ie_present = true;
    }
    else
    {
        *phtinfo_ie_present = false;
    }

    if (d->wmm_ie.vend_hdr.element_id == WMM_IE)
    {
        *wmm_ie_present = true;
    }
    else
    {
        *wmm_ie_present = false;
    }

    *band = d->bss_band;

    if (wps_IE_exist != NULL)
    {
        *wps_IE_exist = d->wps_IE_exist;
    }
    if (wps_session != NULL)
    {
        *wps_session = d->wps_session;
    }

    if (wpa2_entp_IE_exist != NULL)
    {
        *wpa2_entp_IE_exist = d->wpa2_entp_IE_exist;
    }
    (void)memcpy((void *)trans_bssid, (const void *)d->trans_mac_address, MLAN_MAC_ADDR_LENGTH);

    if (d->trans_ssid.ssid_len <= MLAN_MAX_SSID_LENGTH)
    {
        *trans_ssid_len = (int)d->trans_ssid.ssid_len;
        (void)memcpy((void *)trans_ssid, (const void *)d->trans_ssid.ssid, d->trans_ssid.ssid_len);
    }

    return WM_SUCCESS;
}

int wifi_get_scan_result_count(unsigned *count)
{
    if (count == MNULL)
    {
        return -WM_E_INVAL;
    }

    *count = mlan_adap->num_in_scan_table;
    return WM_SUCCESS;
}

int wrapper_wlan_set_regiontable(t_u8 region, t_u16 band)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    mlan_status rv       = wlan_set_regiontable(pmpriv, region, band);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

/*
 * This function is supposed to be called after scan is complete.
 */
void wifi_scan_process_results(void)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    /*
     * This function is normally supposed to be called from inside
     * mlan_scan.c during parsing of scanning results. But we do not
     * have async. cmd send functionality in the current design. So, we
     * have moved this outside.
     */
    wlan_scan_process_results(pmpriv);
}


bool check_for_wpa2_entp_ie(bool *wpa2_entp_IE_exist, const void *element_data, unsigned element_len)
{
    uint8_t wpa2_ent_IE[4];
    uint16_t len;

    (void)memcpy((void *)&len, (const void *)element_data, sizeof(len));

    if (len * 4U >= element_len)
    {
        return false;
    }

    (void)memcpy((void *)&wpa2_ent_IE, (const void *)((const char *)element_data + len * 4U + 2U * sizeof(len)), 4U);

    if (!memcmp(wpa2_ent_IE, wpa2_akmp_oui, sizeof(wpa2_akmp_oui)))
    {
        /* WPA2 Enterprise IE is present in probe response. */
        wifi_d("WPA2 Enterprise AKMP IE :: %x:%x:%x:%x\r\n", wpa2_ent_IE[0], wpa2_ent_IE[1], wpa2_ent_IE[2],
               wpa2_ent_IE[3]);

        *wpa2_entp_IE_exist = true;

        return true;
    }

    return false;
}

/*
 * The below wifi authenticate code, data structures and macros seems to be
 * completely absent from mlan. Our current code, including the wlc manager
 * currently depends on this command and its response (though it may be a
 * no-op).
 *
 * fixme: remove this later when appropriate.
 */
#define host_CMD_802_11_AUTHENTICATE 0x0011
#define AUTH_OPEN_SYSTEM             0

typedef PACK_START struct host_802_11Authenticate_t
{
    uint8_t MacAddr[MLAN_MAC_ADDR_LENGTH];
    uint8_t AuthType;
    uint16_t TimeOut;
    uint8_t Reserved[2];
} PACK_END host_802_11Authenticate_t;

void wifi_get_value1_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint32_t *dev_value1)
{
    if (resp->params.mac_reg.offset == wlan_cpu_to_le16((t_u16)WLAN_VALUE1))
    {
        *dev_value1 = resp->params.mac_reg.value;
    }
}

/*
 * fixme: This function will be present till mlan integration is complete
 */
void wifi_get_mac_address_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint8_t *get_mac_addr)
{
    (void)memcpy((void *)get_mac_addr, (const void *)&resp->params.mac_addr.mac_addr, MLAN_MAC_ADDR_LENGTH);
}

void wifi_get_firmware_ver_ext_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint8_t *fw_ver_ext)
{
    uint8_t comma = 0x2C, space = 0x20;

    if (!resp->params.verext.version_str_sel)
    {
        /* TODO: Below change is added to change 8978 firmware name to IW416.
         * This change is temporary and can be removed once firmware changes are in place */
        if (strstr((const char *)&resp->params.verext.version_str, "w8978o") != NULL)
        {
            (void)memcpy((void *)fw_ver_ext, (const void *)"IW416", 6);
            (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)),
                         (const void *)&resp->params.verext.version_str[6],
                         strlen((const char *)(&resp->params.verext.version_str)) - strlen((const char *)fw_ver_ext));
        }
        else
        {
            (void)memcpy((void *)fw_ver_ext, (const void *)&resp->params.verext.version_str,
                         strlen((const char *)(&resp->params.verext.version_str)));
        }
    }
    else if (resp->params.verext.version_str_sel == 3U && strlen((const char *)(&resp->params.verext.version_str)))
    {
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)), (const void *)&comma, 1);
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)), (const void *)&space, 1);
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)),
                     (const void *)&resp->params.verext.version_str,
                     strlen((const char *)(&resp->params.verext.version_str)));
    }
    else if (resp->params.verext.version_str_sel == 4U && strlen((const char *)(&resp->params.verext.version_str)))
    {
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)), (const void *)&comma, 1);
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)), (const void *)&space, 1);
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)),
                     (const void *)&resp->params.verext.version_str,
                     strlen((const char *)(&resp->params.verext.version_str)));
    }
    else
    { /* Do Nothing */
    }
}

void wifi_prepare_set_cal_data_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command = HostCmd_CMD_CFG_DATA;
    cmd->size    = (t_u16)sizeof(HostCmd_DS_802_11_CFG_DATA) + (t_u16)S_DS_GEN + (t_u16)cal_data_len - (t_u16)1;
    cmd->seq_num = seq_number;
    cmd->result  = 0;
    cmd->params.cfg_data.action   = HostCmd_ACT_GEN_SET;
    cmd->params.cfg_data.type     = 0x02;
    cmd->params.cfg_data.data_len = cal_data_len;
    (void)memcpy((void *)cmd->params.cfg_data.data, (const void *)cal_data, cal_data_len);
}

#ifdef OTP_CHANINFO
void wifi_prepare_get_channel_region_cfg_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command               = HostCmd_CMD_CHAN_REGION_CFG;
    cmd->size                  = sizeof(HostCmd_DS_CHAN_REGION_CFG) + S_DS_GEN;
    cmd->seq_num               = seq_number;
    cmd->result                = 0;
    cmd->params.reg_cfg.action = HostCmd_ACT_GEN_GET;
}
#endif

void wifi_prepare_get_hw_spec_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command = HostCmd_CMD_GET_HW_SPEC;
    cmd->size    = sizeof(HostCmd_DS_GET_HW_SPEC) + S_DS_GEN;
    cmd->seq_num = seq_number;
    cmd->result  = 0;
}

void wifi_prepare_reconfigure_tx_buf_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command = HostCmd_CMD_RECONFIGURE_TX_BUFF;
    /* TODO: Replace hardcoded size with logical implementation */
    cmd->size                    = 16;
    cmd->seq_num                 = seq_number;
    cmd->result                  = 0;
    cmd->params.tx_buf.action    = HostCmd_ACT_GEN_SET;
    cmd->params.tx_buf.buff_size = 2048;
}

/*
 * fixme: This function will be present till mlan integration is complete
 */
void wifi_prepare_get_mac_addr_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command                = HostCmd_CMD_802_11_MAC_ADDRESS;
    cmd->size                   = sizeof(HostCmd_DS_802_11_MAC_ADDRESS) + S_DS_GEN;
    cmd->seq_num                = seq_number;
    cmd->result                 = 0;
    cmd->params.mac_addr.action = HostCmd_ACT_GEN_GET;
}

void wifi_prepare_get_fw_ver_ext_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number, int version_str_sel)
{
    cmd->command                       = HostCmd_CMD_VERSION_EXT;
    cmd->size                          = sizeof(HostCmd_DS_VERSION_EXT) + S_DS_GEN;
    cmd->seq_num                       = seq_number;
    cmd->result                        = 0;
    cmd->params.verext.version_str_sel = version_str_sel;
}

void wifi_prepare_get_value1(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command               = HostCmd_CMD_MAC_REG_ACCESS;
    cmd->command               = wlan_cpu_to_le16(cmd->command);
    cmd->size                  = wlan_cpu_to_le16(sizeof(HostCmd_DS_MAC_REG_ACCESS) + S_DS_GEN);
    cmd->seq_num               = 0x0;
    cmd->result                = 0x0;
    cmd->params.mac_reg.action = wlan_cpu_to_le16(HostCmd_ACT_GEN_GET);
    cmd->params.mac_reg.offset = wlan_cpu_to_le16((t_u16)WLAN_VALUE1);
    cmd->params.mac_reg.value  = 0x0;
}

void wifi_prepare_set_mac_addr_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command                = HostCmd_CMD_802_11_MAC_ADDRESS;
    cmd->size                   = sizeof(HostCmd_DS_802_11_MAC_ADDRESS) + S_DS_GEN;
    cmd->seq_num                = seq_number;
    cmd->result                 = 0;
    cmd->params.mac_addr.action = HostCmd_ACT_GEN_SET;
    (void)memcpy((void *)cmd->params.mac_addr.mac_addr, (const void *)mac_addr, MLAN_MAC_ADDR_LENGTH);
}


void wifi_prepare_enable_amsdu_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command                              = HostCmd_CMD_AMSDU_AGGR_CTRL;
    cmd->size                                 = sizeof(HostCmd_DS_AMSDU_AGGR_CTRL) + S_DS_GEN;
    cmd->seq_num                              = seq_number;
    cmd->result                               = 0;
    cmd->params.amsdu_aggr_ctrl.action        = HostCmd_ACT_GEN_SET;
    cmd->params.amsdu_aggr_ctrl.enable        = 0x1;
    cmd->params.amsdu_aggr_ctrl.curr_buf_size = 0x0;
}

#ifdef WLAN_LOW_POWER_ENABLE
void wifi_prepare_low_power_mode_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command = HostCmd_CMD_LOW_POWER_MODE;
    cmd->size    = sizeof(HostCmd_CONFIG_LOW_PWR_MODE) + S_DS_GEN;
    cmd->seq_num = seq_number;
    cmd->result  = 0;

    cmd->params.low_pwr_mode_cfg.enable = 1;

    return;
}
#endif

void wlan_prepare_mac_control_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command = HostCmd_CMD_MAC_CONTROL;
    cmd->size    = sizeof(HostCmd_DS_MAC_CONTROL) + S_DS_GEN;
    cmd->seq_num = seq_number;
    cmd->result  = 0;

    cmd->params.mac_ctrl.action =
        HostCmd_ACT_MAC_RX_ON | HostCmd_ACT_MAC_TX_ON | HostCmd_ACT_MAC_ETHERNETII_ENABLE | HostCmd_ACT_MAC_WEP_ENABLE;

    return;
}

void wifi_set_cal_data(uint8_t *cdata, unsigned int clen)
{
    cal_data       = cdata;
    cal_data_len   = clen;
    cal_data_valid = true;
}

void wifi_set_mac_addr(uint8_t *mac)
{
    mac_addr       = mac;
    mac_addr_valid = true;
}

void _wifi_set_mac_addr(uint8_t *mac)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, MLAN_BSS_TYPE_UAP);

    cmd->result = 0x0;
    (void)wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_MAC_ADDRESS,
                                   HostCmd_ACT_GEN_SET, 0, NULL, mac, cmd);
    (void)wifi_wait_for_cmdresp(NULL);
    /* Also need to update priv->curr_addr, as rx reorder will check mac address using priv->curr_addr */
    (void)memcpy(&mlan_adap->priv[0]->curr_addr[0], &mac[0], MLAN_MAC_ADDR_LENGTH);
    (void)memcpy(&mlan_adap->priv[1]->curr_addr[0], &mac[0], MLAN_MAC_ADDR_LENGTH);
}


#ifdef WLAN_LOW_POWER_ENABLE
void wifi_enable_low_pwr_mode()
{
    low_power_mode = true;
}
#endif


#ifdef CONFIG_11AX
int wifi_set_11ax_tx_omi(const t_u16 tx_omi)
{
    mlan_ioctl_req req;

    mlan_ds_11ax_cmd_cfg cfg;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    (void)memset(&cfg, 0x00, sizeof(mlan_ds_11ax_cmd_cfg));

    req.action  = MLAN_ACT_SET;
    req.pbuf    = (t_u8 *)&cfg;
    req.buf_len = sizeof(mlan_ds_11ax_cmd_cfg);

    cfg.sub_id              = MLAN_11AXCMD_TXOMI_SUBID;
    cfg.param.txomi_cfg.omi = tx_omi;

    mlan_status rv;

    rv = wlan_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_11AX_CMD, HostCmd_ACT_GEN_SET, 0,
                          (t_void *)&req, (t_void *)&cfg);

    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}
#endif


