/** @file wifi_pwrmgr.c
 *
 *  @brief This file provides all power management code for WIFI.
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

#include <mlan_api.h>

#include <wmerrno.h>
#include <wm_os.h>
#include <wifi.h>
#include <wifi_events.h>

#include <wmlog.h>
#if defined(RW610)
#include "wifi-imu.h"
#else
#include "wifi-sdio.h"
#endif
#include "wifi-internal.h"

#define pwr_e(...) wmlog_e("pwr", ##__VA_ARGS__)
#define pwr_w(...) wmlog_w("pwr", ##__VA_ARGS__)

#define pwr_d(...)

#define MAX_LISTEN_INTERVAL_IN_BCON     49
#define MIN_LISTEN_INTERVAL_IN_TU       50
#define MAX_LISTEN_INTERVAL_IN_TU       65000
#define CLOSEST_DTIM_TO_LISTEN_INTERVAL 65534

static bool ieeeps_enabled;
static bool deepsleepps_enabled;

static void wifi_set_ps_cfg(t_u16 multiple_dtims,
                            t_u16 bcn_miss_timeout,
                            t_u16 local_listen_interval,
                            t_u16 adhoc_wake_period,
                            t_u16 mode,
                            t_u16 delay_to_ps)
{
    pmlan_adapter pmadapter          = ((mlan_private *)mlan_adap->priv[0])->adapter;
    pmadapter->bcn_miss_time_out     = bcn_miss_timeout;
    pmadapter->multiple_dtim         = multiple_dtims;
    pmadapter->local_listen_interval = local_listen_interval;
    pmadapter->delay_to_ps           = delay_to_ps;
    pmadapter->enhanced_ps_mode      = mode;
    pmadapter->adhoc_awake_period    = adhoc_wake_period;
}

void wifi_configure_null_pkt_interval(unsigned int null_pkt_interval)
{
    pmlan_adapter pmadapter      = ((mlan_private *)mlan_adap->priv[0])->adapter;
    pmadapter->null_pkt_interval = (t_u16)null_pkt_interval;
}

void wifi_configure_listen_interval(int listen_interval)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    pmpriv->listen_interval = (t_u16)listen_interval;

    pmlan_adapter pmadapter = ((mlan_private *)mlan_adap->priv[0])->adapter;
    wifi_set_ps_cfg(CLOSEST_DTIM_TO_LISTEN_INTERVAL, DEFAULT_BCN_MISS_TIMEOUT, 0, 0, PS_MODE_AUTO, DELAY_TO_PS_DEFAULT);
    pmadapter->local_listen_interval = (t_u16)listen_interval;
}

int wifi_send_hs_cfg_cmd(mlan_bss_type interface, t_u32 ipv4_addr, t_u16 action, t_u32 conditions)
{
    arpfilter_header *arpfilter = NULL;
    filter_entry *entry         = NULL;
    void *pdata_buf             = NULL;
    hs_config_param hs_cfg_obj;

    (void)wifi_get_command_lock();

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    (void)memset(&hs_cfg_obj, 0x00, sizeof(hs_config_param));

    cmd->seq_num = (t_u16)(HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, (t_u32)(interface)));
    if (action == (t_u16)HS_CONFIGURE)
    {
        hs_cfg_obj.conditions = conditions;
        hs_cfg_obj.gap        = 0x2;
#ifdef RW610
        hs_cfg_obj.gpio = 0xff;
#else
        hs_cfg_obj.gpio       = HOST_WAKEUP_GPIO_PIN;
#endif
        pdata_buf             = &hs_cfg_obj;

        /* wake conditions for broadcast is
         * enabled when bit 0 is set.
         * The code below sets the correct bit which
         * firmware will use to give host wakeup
         */
        if ((conditions != (t_u32)(HOST_SLEEP_CFG_CANCEL)) &&
            ((conditions & (t_u32)(WIFI_WAKE_ON_ARP_BROADCAST)) != 0U))
        {
            hs_cfg_obj.conditions |= (t_u32)(WIFI_WAKE_ON_ALL_BROADCAST);
            hs_cfg_obj.conditions &= ~((t_u32)(WIFI_WAKE_ON_ARP_BROADCAST));
        }
    }

    mlan_status status = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_HS_CFG_ENH,
                                                  HostCmd_ACT_GEN_SET, 0, NULL, pdata_buf, cmd);
    /* Construct the ARP filter TLV */
    arpfilter       = (arpfilter_header *)((uint32_t)cmd + cmd->size);
    arpfilter->type = TLV_TYPE_ARP_FILTER;

    if ((ipv4_addr != 0U) && (action == (t_u16)HS_CONFIGURE) && (conditions != (t_u32)(HOST_SLEEP_CFG_CANCEL)))
    {
        entry            = (filter_entry *)((uint32_t)arpfilter + sizeof(arpfilter_header));
        entry->addr_type = ADDR_TYPE_MULTICAST;
        entry->eth_type  = ETHER_TYPE_ANY;
        entry->ipv4_addr = IPV4_ADDR_ANY;
        entry++;

        entry->addr_type = ADDR_TYPE_BROADCAST;
        if ((conditions & (t_u32)(WIFI_WAKE_ON_ALL_BROADCAST)) != 0U)
        {
            entry->eth_type  = ETHER_TYPE_ANY;
            entry->ipv4_addr = IPV4_ADDR_ANY;
        }
        else
        {
            entry->eth_type  = ETHER_TYPE_ARP;
            entry->ipv4_addr = ipv4_addr;
        }
        entry++;

        entry->addr_type = ADDR_TYPE_UNICAST;
        entry->eth_type  = ETHER_TYPE_ANY;
        entry->ipv4_addr = IPV4_ADDR_ANY;
        arpfilter->len   = 3U * sizeof(filter_entry);
        cmd->size        = (t_u16)(cmd->size + sizeof(arpfilter_header) + arpfilter->len);
    }
    else if (action == (t_u16)HS_ACTIVATE)
    {
        arpfilter->len = 0;
        cmd->size -= (t_u16)sizeof(t_u32);
    }
    else
    {
        /** Do nothing */
    }

    (void)wifi_wait_for_cmdresp(NULL);
    return (int)status;
}

static int wifi_send_power_save_command(ENH_PS_MODES action, t_u16 ps_bitmap, mlan_bss_type interface, void *pdata_buf)
{
    mlan_status status;
    mlan_ds_auto_ds ds_param;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, (t_u8)(interface));
    if (ps_bitmap == BITMAP_AUTO_DS)
    {
        ds_param.idletime = (*(t_u16 *)pdata_buf);
        pdata_buf         = &ds_param;
    }

    cmd->result = 0x0;
    status      = wlan_cmd_enh_power_mode((mlan_private *)mlan_adap->priv[0], cmd, action, (t_u16)ps_bitmap, pdata_buf);

    (void)wifi_wait_for_cmdresp(NULL);
    return (int)status;
}

int wifi_enter_ieee_power_save(void)
{
        return wifi_send_power_save_command(EN_AUTO_PS, BITMAP_STA_PS, MLAN_BSS_TYPE_STA, NULL);
}

int wifi_exit_ieee_power_save(void)
{
        return wifi_send_power_save_command(DIS_AUTO_PS, BITMAP_STA_PS, MLAN_BSS_TYPE_STA, NULL);
}

int wifi_enter_deepsleep_power_save(void)
{
    t_u16 idletime = 0;
    /* Set default idle time for deep sleep mode.
     * If not set, fw will use 10ms as default value and this will
     * cause small time gap between ps_wakeup and ps_sleep events
     */
    idletime = DEEP_SLEEP_IDLE_TIME;
    return wifi_send_power_save_command(EN_AUTO_PS, BITMAP_AUTO_DS, MLAN_BSS_TYPE_STA, &idletime);
}

int wifi_exit_deepsleep_power_save(void)
{
    t_u16 idletime = 0;
    return wifi_send_power_save_command(DIS_AUTO_PS, BITMAP_AUTO_DS, MLAN_BSS_TYPE_STA, &idletime);
}

int wifi_uap_ps_inactivity_sleep_enter(mlan_bss_type type,
                                       unsigned int ctrl_bitmap,
                                       unsigned int min_sleep,
                                       unsigned int max_sleep,
                                       unsigned int inactivity_to,
                                       unsigned int min_awake,
                                       unsigned int max_awake)
{
    mlan_ds_ps_mgmt data_buff;

    (void)memset(&data_buff, 0x00, sizeof(mlan_ds_ps_mgmt));

    data_buff.sleep_param.ctrl_bitmap   = ctrl_bitmap;
    data_buff.sleep_param.min_sleep     = min_sleep;
    data_buff.sleep_param.max_sleep     = max_sleep;
    data_buff.inact_param.inactivity_to = inactivity_to;
    data_buff.inact_param.min_awake     = min_awake;
    data_buff.inact_param.max_awake     = max_awake;
    data_buff.ps_mode                   = PS_MODE_INACTIVITY;
    data_buff.flags                     = PS_FLAG_INACT_SLEEP_PARAM | PS_FLAG_SLEEP_PARAM;

    return wifi_send_power_save_command(EN_AUTO_PS, BITMAP_UAP_INACT_PS, type, &data_buff);
}

int wifi_uap_ps_inactivity_sleep_exit(mlan_bss_type type)
{
    return wifi_send_power_save_command(DIS_AUTO_PS, BITMAP_UAP_INACT_PS, type, NULL);
}

void send_sleep_confirm_command(mlan_bss_type interface)
{
    OPT_Confirm_Sleep *ps_cfm_sleep;

    HostCmd_DS_COMMAND *command = wifi_get_command_buffer();

    (void)wifi_get_command_lock();
    ps_cfm_sleep = (OPT_Confirm_Sleep *)(void *)(command);

    (void)memset(ps_cfm_sleep, 0, sizeof(OPT_Confirm_Sleep));
    ps_cfm_sleep->command = HostCmd_CMD_802_11_PS_MODE_ENH;
    ps_cfm_sleep->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, (t_u8)(interface));

    ps_cfm_sleep->size                = (t_u16)sizeof(OPT_Confirm_Sleep);
    ps_cfm_sleep->result              = 0;
    ps_cfm_sleep->action              = (t_u16)SLEEP_CONFIRM;
    ps_cfm_sleep->sleep_cfm.resp_ctrl = (t_u16)RESP_NEEDED;

    /* If driver did not send out sleep confirm in the expected time,
       FW would switch to awake and send out PS AWAKE event.
       After received AWAKE event when presleep, state would switch to AWAKE.
       So here only send out sleep confirm when state is presleep,
       and would not send out sleep confirm if state has switched to AWAKE */
    if (mlan_adap->ps_state == PS_STATE_PRE_SLEEP)
    {
        mlan_adap->ps_state = PS_STATE_SLEEP_CFM;
        wcmdr_d("+");
        (void)wifi_wait_for_cmdresp(NULL);
    }
    else
    {
        (void)wifi_put_command_lock();
    }
}

/* fixme: accept HostCmd_DS_COMMAND directly */
void wifi_process_hs_cfg_resp(t_u8 *cmd_res_buffer)
{
    pmlan_adapter pmadapter              = ((mlan_private *)mlan_adap->priv[0])->adapter;
    HostCmd_DS_802_11_HS_CFG_ENH *hs_cfg = (HostCmd_DS_802_11_HS_CFG_ENH *)(void *)(cmd_res_buffer + S_DS_GEN);
    if (hs_cfg->action == (t_u16)HS_ACTIVATE)
    {
        pwr_d("Host sleep activated");
        wlan_update_rxreorder_tbl(pmadapter, MTRUE);
    }
    else
    {
        pwr_d("Host sleep configuration done");
    }
}

enum wifi_event_reason wifi_process_ps_enh_response(t_u8 *cmd_res_buffer, t_u16 *ps_event, t_u16 *action)
{
    enum wifi_event_reason result = WIFI_EVENT_REASON_FAILURE;
    MrvlIEtypesHeader_t *mrvl_tlv = NULL;
    HostCmd_DS_802_11_PS_MODE_ENH *ps_mode = (HostCmd_DS_802_11_PS_MODE_ENH *)(void *)(cmd_res_buffer + S_DS_GEN);

    *ps_event = (t_u16)WIFI_EVENT_PS_INVALID;
    *action   = (t_u16)(ps_mode->action);
    if (ps_mode->action == (t_u16)EN_AUTO_PS)
    {
        if ((ps_mode->params.auto_ps.ps_bitmap & BITMAP_AUTO_DS) != 0U)
        {
            pwr_d("Enabled deep sleep mode");
            mrvl_tlv = (MrvlIEtypesHeader_t *)(void *)((uint8_t *)ps_mode + AUTO_PS_FIX_SIZE);
            while (mrvl_tlv->type != TLV_TYPE_AUTO_DS_PARAM)
            {
                mrvl_tlv =
                    (MrvlIEtypesHeader_t *)(void *)((uint8_t *)mrvl_tlv + mrvl_tlv->len + sizeof(MrvlIEtypesHeader_t));
            }
            pwr_d("Enabled Deep Sleep mode");
            deepsleepps_enabled = true;
        }
        if ((ps_mode->params.auto_ps.ps_bitmap & BITMAP_STA_PS) != 0U)
        {
            pwr_d(
                "Enabled IEEE power "
                "save mode");
            mrvl_tlv = (MrvlIEtypesHeader_t *)(void *)((uint8_t *)ps_mode + AUTO_PS_FIX_SIZE);
            while (mrvl_tlv->type != TLV_TYPE_PS_PARAM)
            {
                mrvl_tlv =
                    (MrvlIEtypesHeader_t *)(void *)((uint8_t *)mrvl_tlv + mrvl_tlv->len + sizeof(MrvlIEtypesHeader_t));
            }
            pwr_d(
                "pscfg: %u %u %u %u "
                "%u %u",
                ps_tlv->param.null_pkt_interval, ps_tlv->param.multiple_dtims, ps_tlv->param.local_listen_interval,
                ps_tlv->param.bcn_miss_timeout, ps_tlv->param.delay_to_ps, ps_tlv->param.mode);
            ieeeps_enabled = true;
        }
        result = WIFI_EVENT_REASON_SUCCESS;
    }
    else if (ps_mode->action == (t_u16)DIS_AUTO_PS)
    {
        if ((ps_mode->params.ps_bitmap & BITMAP_AUTO_DS) != 0U)
        {
            pwr_d("Disabled DeepSleep mode");
            *ps_event           = (t_u16)WIFI_EVENT_DEEP_SLEEP;
            deepsleepps_enabled = false;
        }
        if ((ps_mode->params.ps_bitmap & BITMAP_STA_PS) != 0U)
        {
            pwr_d(
                "Disabled IEEE power "
                "save mode");
            *ps_event      = (t_u16)WIFI_EVENT_IEEE_PS;
            ieeeps_enabled = false;
        }
        result = WIFI_EVENT_REASON_SUCCESS;
    }
    else if (ps_mode->action == (t_u16)GET_PS)
    {
        if ((ps_mode->params.ps_bitmap & BITMAP_AUTO_DS) != 0U)
        {
            pwr_d("Deep sleep mode is on");
            mrvl_tlv = (MrvlIEtypesHeader_t *)(void *)((uint8_t *)ps_mode + AUTO_PS_FIX_SIZE);
            while (mrvl_tlv->type != TLV_TYPE_AUTO_DS_PARAM)
            {
                mrvl_tlv =
                    (MrvlIEtypesHeader_t *)(void *)((uint8_t *)mrvl_tlv + mrvl_tlv->len + sizeof(MrvlIEtypesHeader_t));
            }
        }
        if ((ps_mode->params.ps_bitmap & BITMAP_STA_PS) != 0U)
        {
            pwr_d("IEEE power save mode is on");
            mrvl_tlv = (MrvlIEtypesHeader_t *)(void *)((uint8_t *)ps_mode + AUTO_PS_FIX_SIZE);
            while (mrvl_tlv->type != TLV_TYPE_PS_PARAM)
            {
                mrvl_tlv =
                    (MrvlIEtypesHeader_t *)(void *)((uint8_t *)mrvl_tlv + mrvl_tlv->len + sizeof(MrvlIEtypesHeader_t));
            }
            pwr_d(
                "pscfg: %u %u %u %u "
                "%u %u\r\n",
                ps_tlv->param.null_pkt_interval, ps_tlv->param.multiple_dtims, ps_tlv->param.local_listen_interval,
                ps_tlv->param.bcn_miss_timeout, ps_tlv->param.delay_to_ps, ps_tlv->param.mode);
        }
    }
    else if (ps_mode->action == (t_u16)SLEEP_CONFIRM)
    {
        wcmdr_d("#");
        if (ieeeps_enabled || deepsleepps_enabled)
        {
            mlan_adap->ps_state = PS_STATE_SLEEP;
        }
        else
        {
            return WIFI_EVENT_REASON_FAILURE;
        }

        if (ieeeps_enabled || deepsleepps_enabled
        )
        {
            /* sleep confirm response needs to get the sleep_rwlock, for this lock
             * is an indication that host needs to wakeup FW when reader (cmd/tx)
             * could not get the sleep_rwlock */
            int ret = os_rwlock_write_lock(&sleep_rwlock, OS_WAIT_FOREVER);
            if (ret == WM_SUCCESS)
            {
                wcmdr_d("Get sleep rw lock successfully");
            }
            else
            {
                pwr_e("Failed to get sleep rw lock");
                return WIFI_EVENT_REASON_FAILURE;
            }
        }

        result = WIFI_EVENT_REASON_SUCCESS;
    }
    else
    { /* Do Nothing */
    }

    return result;
}

