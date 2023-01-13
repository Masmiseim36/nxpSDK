/** @file wifi-uap.c
 *
 *  @brief This file provides UAP related APIs.
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

#include <wifi.h>
#if defined(RW610)
#include "wifi-imu.h"
#else
#include "wifi-sdio.h"
#endif
#include "wifi-internal.h"

/* fixme: Some of the following options could be added to kconfig. While
   adding the ranges in kconfig use the ones given as macros in
   mlan_uap_cmdevent.c */
#define UAP_BEACON_PERIOD 200U
#define UAP_DTIM_PERIOD   10
#define MAX_RATES         14U

#ifdef CONFIG_5GHz_SUPPORT
static uint8_t rates_5ghz[] = {0x8c, 0x98, 0xb0, 0x12, 0x24, 0x48, 0x60, 0x6c};
#endif

static uint8_t rates_2ghz[] = {0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, 0x30, 0x48, 0x60, 0x6c};

int wifi_uap_downld_domain_params(MrvlIEtypes_DomainParamSet_t *dp);

#ifdef CONFIG_11AC
/**
 * @brief initialize AP bss config
 * @param pmpriv            A pointer to mlan_private structure
 * @param band            BAND_5G/BAND_2GHZ
 * @return                0 -- success, otherwise fail
 */
static bool wifi_check_11ac_capability(mlan_private *pmpriv, t_u8 band)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    bool enable_11ac        = MFALSE;

    ENTER();
    if ((band == BAND_CONFIG_5GHZ) && !(pmadapter->fw_bands & BAND_AAC))
    {
        PRINTM(MCMND, "FW don't support 5G AC\n");
        LEAVE();
        return enable_11ac;
    }
    if ((band == BAND_CONFIG_ACS_MODE || band == BAND_CONFIG_MANUAL) && !(pmadapter->fw_bands & BAND_GAC))
    {
        PRINTM(MCMND, "FW don't support 2G AC");
        LEAVE();
        return enable_11ac;
    }
    enable_11ac = MTRUE;

    LEAVE();
    return enable_11ac;
}

#define VHT_CAP_11AC_MASK 0x007fffff

/**
 *  @brief enable/disable 11AC
 *
 *  @param pmpriv   A pointer to mlan_private structure
 *  @param action   MLAN_ACT_DISABLE or MLAN_ACT_ENABLE
 *  @param band     band config
 *
 *  @return         0--success, otherwise failure
 */
static int wifi_uap_set_11ac_status(mlan_private *pmpriv, t_u8 action, t_u8 bandwidth, t_u8 channel)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    int ret                 = 0;
    mlan_ds_11ac_vht_cfg vht_cfg;

    (void)memset(&vht_cfg, 0, sizeof(vht_cfg));
#ifdef CONFIG_5GHz_SUPPORT
    if (channel > MAX_CHANNELS_BG)
    {
        vht_cfg.band = BAND_SELECT_A;
    }
    else
    {
        vht_cfg.band = BAND_SELECT_BG;
    }
#else
    vht_cfg.band = BAND_SELECT_BG;
#endif
    vht_cfg.txrx = MLAN_RADIO_TXRX;

    vht_cfg.vht_cap_info = pmadapter->usr_dot_11ac_dev_cap_a;
    if (action == MLAN_ACT_DISABLE)
    {
        vht_cfg.bwcfg = BW_FOLLOW_HTCAP;
        vht_cfg.vht_cap_info &= ~VHT_CAP_11AC_MASK;
        vht_cfg.vht_rx_mcs = vht_cfg.vht_tx_mcs = 0xffff;
        vht_cfg.skip_usr_11ac_mcs_cfg           = MTRUE;
    }
    else
    {
        if (BANDWIDTH_80MHZ == bandwidth)
        {
            vht_cfg.bwcfg = BW_FOLLOW_VHTCAP;
        }
        else
        {
            vht_cfg.bwcfg = BW_FOLLOW_HTCAP;
        }

        vht_cfg.vht_cap_info &= ~DEFALUT_11AC_CAP_BEAMFORMING_RESET_MASK;
#ifdef RW610
        vht_cfg.vht_cap_info &= ~DEFALUT_11AC_CAP_SHORTGI_80MHZ_RESET_MASK;
#endif
        vht_cfg.vht_tx_mcs            = pmadapter->usr_dot_11ac_mcs_support >> 16;
        vht_cfg.vht_rx_mcs            = pmadapter->usr_dot_11ac_mcs_support & 0xffff;
        vht_cfg.skip_usr_11ac_mcs_cfg = MTRUE;
    }

    if ((GET_VHTCAP_MAXMPDULEN(vht_cfg.vht_cap_info)) != 0U)
    {
        RESET_11ACMAXMPDULEN(vht_cfg.vht_cap_info);
    }
    else
    {
        /** Do Nothing */
    }

    PRINTM(MCMND, "Uap:11ac=%d vht_cap_info=0x%x, vht_tx_mcs=0x%x, vht_rx_mcs=0x%x\n", action, vht_cfg.vht_cap_info,
           vht_cfg.vht_tx_mcs, vht_cfg.vht_rx_mcs);
    ret = (int)wlan_11ac_ioctl_vhtcfg(pmpriv, (t_u8)MLAN_ACT_SET, &vht_cfg);
    return ret;
}
#endif

#ifdef CONFIG_11AX
#if 0
/**
 * @brief initialize AP bss config
 * @param pmpriv            A pointer to mlan_private structure
 * @param band            BAND_5G/BAND_2GHZ
 * @return                0 -- success, otherwise fail
 */
static t_u8 wifi_check_11ax_capability(mlan_private *pmpriv, t_u8 band)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    t_u8 enable_11ax = MFALSE;
    mlan_ds_11ax_he_cfg he_cfg;
    t_u8 he_txrx_mcs_support[4] = {0xff, 0xff, 0xff, 0xff};

    ENTER();
    if((band == BAND_CONFIG_5GHZ) && !(pmadapter->fw_bands & BAND_AAX))
    {
        PRINTM(MCMND, "FW don't support 5G AX\n");
        LEAVE();
        return enable_11ax;
    }
    if((band == BAND_CONFIG_ACS_MODE || band == BAND_CONFIG_MANUAL)
        && !(pmadapter->fw_bands & BAND_GAX))
    {
        PRINTM(MCMND, "FW don't support 2G AX\n");
        LEAVE();
        return enable_11ax;
    }
    memset(&he_cfg, 0, sizeof(he_cfg));
    if (band == BAND_CONFIG_5GHZ)
        he_cfg.band = MBIT(1);
    else if (band == BAND_CONFIG_ACS_MODE || band == BAND_CONFIG_MANUAL)
        he_cfg.band = MBIT(0);
    if (0 == wlan_cmd_11ax_cfg(pmpriv, HostCmd_ACT_GEN_GET, &he_cfg))
    {
        if (he_cfg.he_cap.len && (he_cfg.he_cap.ext_id == HE_CAPABILITY))
        {
            if (memcmp(he_cfg.he_cap.he_txrx_mcs_support, he_txrx_mcs_support,
                        sizeof(he_txrx_mcs_support)))
                enable_11ax = MTRUE;
        }
    }
    PRINTM(MCMND, "enable_11ax=%d\n", enable_11ax);
    LEAVE();
    return enable_11ax;
}
#endif

/**
 *  @brief enable/disable 11AX
 *
 *  @param pmpriv   A pointer to mlan_private structure
 *  @param action   MLAN_ACT_DISABLE or MLAN_ACT_ENABLE
 *  @param band     band config
 *
 *  @return         0--success, otherwise failure
 */
int wifi_uap_set_11ax_status(mlan_private *pmpriv, t_u8 action, t_u8 band)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    int ret                 = 0;
    mlan_ds_11ax_he_cfg he_cfg;

    ENTER();
    if ((band == BAND_CONFIG_5GHZ && !(pmadapter->fw_bands & BAND_AAX)) ||
        ((band == BAND_CONFIG_ACS_MODE || band == BAND_CONFIG_MANUAL) && !(pmadapter->fw_bands & BAND_GAX)))
    {
        PRINTM(MERROR, "fw doesn't support 11ax\n");
        ret = -WM_FAIL;
        goto done;
    }
    memset(&he_cfg, 0, sizeof(he_cfg));
    if (band == BAND_CONFIG_5GHZ)
    {
        he_cfg.band = MBIT(1);
        (void)memcpy((void *)&he_cfg.he_cap, (const void *)pmadapter->hw_he_cap, pmadapter->hw_hecap_len);
    }
    else if (band == BAND_CONFIG_ACS_MODE || band == BAND_CONFIG_MANUAL)
    {
        he_cfg.band = MBIT(0);
        (void)memcpy((void *)&he_cfg.he_cap, (const void *)pmadapter->hw_2g_he_cap, pmadapter->hw_2g_hecap_len);
    }
    else
    {
        PRINTM(MERROR, "Invalid band!\n");
        ret = -WM_E_INVAL;
        goto done;
    }
#ifdef RW610
    he_cfg.he_cap.he_phy_cap[0] &= ~DEFAULT_11AX_CAP_40MHZIH2_4GHZBAND_RESET_MASK;
#endif
#if 0
    if (wlan_cmd_11ax_cfg(pmpriv, HostCmd_ACT_GEN_GET, &he_cfg))
    {
        PRINTM(MERROR, "Fail to get 11ax cfg!\n");
        ret = -WM_FAIL;
        goto done;
    }
#endif
    if (action == MLAN_ACT_DISABLE)
    {
        if (he_cfg.he_cap.len && (he_cfg.he_cap.ext_id == HE_CAPABILITY))
            memset(he_cfg.he_cap.he_txrx_mcs_support, 0xff, sizeof(he_cfg.he_cap.he_txrx_mcs_support));
        else
        {
            PRINTM(MCMND, "11ax already disabled\n");
            goto done;
        }
    }
    DBG_HEXDUMP(MCMD_D, "HE_CFG ", (t_u8 *)&he_cfg, sizeof(he_cfg));
    ret = wlan_cmd_11ax_cfg(pmpriv, HostCmd_ACT_GEN_SET, &he_cfg);
done:
    LEAVE();
    return ret;
}
#endif

int wifi_uap_prepare_and_send_cmd(mlan_private *pmpriv,
                                  t_u16 cmd_no,
                                  t_u16 cmd_action,
                                  t_u32 cmd_oid,
                                  t_void *pioctl_buf,
                                  t_void *pdata_buf,
                                  mlan_bss_type bss_type,
                                  void *priv)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, (uint16_t)bss_type);
    cmd->result  = 0x0;

    mlan_status rv = wlan_ops_uap_prepare_cmd(pmpriv, cmd_no, cmd_action, cmd_oid, pioctl_buf, pdata_buf, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        wifi_w("Failed to prepare cmd for uAP");
        (void)wifi_put_command_lock();
        return (int)rv;
    }

    (void)wifi_wait_for_cmdresp(priv);
    return wm_wifi.cmd_resp_status;
}

int wifi_uap_set_country(country_code_t country)
{
    int ret;
    t_u8 nr_sb;

    if (wifi_uap_enable_11d() != WM_SUCCESS)
    {
        wifi_e("unable to enabled 11d feature\r\n");
        return (int)WM_FAIL;
    }

    wifi_11d_country = country;

    wifi_sub_band_set_t *sub_band = get_sub_band_from_country(country, &nr_sb);

    wifi_domain_param_t *dp = get_11d_domain_params(country, sub_band, nr_sb);

    ret = wifi_uap_set_domain_params(dp);

    if (ret != WM_SUCCESS)
    {
        wifi_11d_country = COUNTRY_NONE;
        os_mem_free(dp);
        return ret;
    }

    os_mem_free(dp);
    return WM_SUCCESS;
}

/*
 * Note: wlan_uap_domain_info() and wlan_uap_callback_domain_info() are the
 * original function which handles this functionality. However, it does it
 * through and IOCTL and its control flow will not work in our case. To
 * elaborate, it first gets the channel number by sending a command to
 * firmware. Then in the cmd resp handler to sends the domain info
 * command. As per the current design of our driver we cannot send command
 * from command resp handler. Hence, we have modified the control flow to
 * suit our design. The wifi_get_uap_channel() function also helps us in
 * this by updating pmpriv->uap_state_chan_cb.band_config and
 * pmpriv->uap_state_chan_cb.channel.
 *
 * fixme: Need to check how ACS works with this.
 */
int wifi_uap_downld_domain_params(MrvlIEtypes_DomainParamSet_t *dp)
{
    mlan_status rv = MLAN_STATUS_FAILURE;

    if (dp == MNULL)
    {
        return -WM_E_INVAL;
    }

    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    (void)wifi_get_uap_channel(NULL);

    t_u16 band = (pmpriv->uap_state_chan_cb.band_config & BAND_CONFIG_5GHZ) ? BAND_A : BAND_B;

    if (pmpriv->support_11d_APIs != NULL)
    {
        rv = pmpriv->support_11d_APIs->wlan_11d_handle_uap_domain_info_p(pmpriv, band, (t_u8 *)dp, NULL);
    }
    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

static int wifi_cmd_uap_config(char *ssid,
                               t_u8 *mac_addr,
                               enum wlan_security_type security,
                               char *passphrase,
                               char *password,
                               t_u8 channel,
                               wifi_scan_chan_list_t scan_chan_list,
                               t_u16 beacon_period,
                               t_u8 bandwidth,
                               t_u8 dtim_period,
                               t_u8 chan_sw_count,
                               mlan_bss_type bss_type)
{
    t_u32 ssid_len = strlen(ssid);
    uint8_t i;
    int ret;
    t_u8 supported_mcs_set[] = {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#ifdef CONFIG_11AC
    bool enable_11ac = MFALSE;
#endif

    if (!(security == WLAN_SECURITY_NONE || security == WLAN_SECURITY_WPA2 || security == WLAN_SECURITY_WPA3_SAE ||
          security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED
          ))
    {
        return -WM_E_INVAL;
    }

    int passphrase_len = (int)strlen(passphrase);
    int password_len   = (int)strlen(password);

    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];

    /* fixme: check if this needs to go on heap */
    mlan_ds_bss bss;
    (void)memset(&bss, 0x00, sizeof(mlan_ds_bss));

    bss.sub_command = MLAN_OID_UAP_BSS_CONFIG;

    bss.param.bss_config.ssid.ssid_len = ssid_len;
    (void)memcpy((void *)bss.param.bss_config.ssid.ssid, (const void *)ssid, ssid_len);

    if (mac_addr != NULL)
    {
        (void)memcpy((void *)bss.param.bss_config.mac_addr, (const void *)mac_addr, MLAN_MAC_ADDR_LENGTH);
    }

    if (bss_type == MLAN_BSS_TYPE_UAP)
    { /* Not required for WFD */
        bss.param.bss_config.beacon_period  = beacon_period;
        bss.param.bss_config.dtim_period    = dtim_period;
        bss.param.bss_config.bcast_ssid_ctl = (t_u8)!wm_wifi.bcast_ssid_ctl;
        if (chan_sw_count != 0U)
        {
            bss.param.bss_config.dtim_period   = 1;
            bss.param.bss_config.chan_sw_count = chan_sw_count;
        }
    }
    /* Auto channel selection is not handled in 5GHz band, only
     * manual channel selection is supported right now.
     */
    if (channel != 0U)
    {
#ifdef CONFIG_5GHz_SUPPORT
        if (channel > MAX_CHANNELS_BG)
        {
            if (wlan_11h_radar_detect_required(pmpriv, channel))
            {
                wuap_e("Cannot start uAP on DFS channel %d", channel);
                return -WM_E_INVAL;
            }

            bss.param.bss_config.band_cfg = BAND_CONFIG_5GHZ;
            (void)memcpy((void *)bss.param.bss_config.rates, (const void *)rates_5ghz, sizeof(rates_5ghz));
        }
        else
        {
            (void)memcpy((void *)bss.param.bss_config.rates, (const void *)rates_2ghz, sizeof(rates_2ghz));
            bss.param.bss_config.band_cfg = BAND_CONFIG_MANUAL;
        }
#else
        (void)memcpy((void *)bss.param.bss_config.rates, (const void *)rates_2ghz, sizeof(rates_2ghz));
        bss.param.bss_config.band_cfg = BAND_CONFIG_MANUAL;
#endif
        bss.param.bss_config.channel = channel;
    }
    else
    {
        /* Auto channel selection from all channels*/
        bss.param.bss_config.band_cfg = BAND_CONFIG_ACS_MODE;
        bss.param.bss_config.channel  = 0;

        if (scan_chan_list.num_of_chan != 0U)
        {
            /* Specify channels if any for Auto channel selection */
            bss.param.bss_config.num_of_chan = scan_chan_list.num_of_chan;
            for (i = 0; i < scan_chan_list.num_of_chan; i++)
            {
                bss.param.bss_config.chan_list[i].chan_number = scan_chan_list.chan_number[i];
#ifdef CONFIG_5GHz_SUPPORT
                if (scan_chan_list.chan_number[i] > MAX_CHANNELS_BG)
                {
                    bss.param.bss_config.chan_list[i].band_config_type = BAND_CONFIG_5GHZ;
                    (void)memcpy((void *)bss.param.bss_config.rates, (const void *)rates_5ghz, sizeof(rates_5ghz));
                }
                else
#endif
                {
                    bss.param.bss_config.chan_list[i].band_config_type = 0x10;
                    (void)memcpy(bss.param.bss_config.rates, rates_2ghz, sizeof(rates_2ghz));
                }
            }
        }
    }

#ifdef CONFIG_11AC
    enable_11ac = wifi_check_11ac_capability(pmpriv, bss.param.bss_config.band_cfg);
#endif

    if (security == WLAN_SECURITY_NONE)
    {
        bss.param.bss_config.protocol = PROTOCOL_NO_SECURITY;
    }

    if (security == WLAN_SECURITY_WPA2 || security == WLAN_SECURITY_WPA3_SAE ||
        security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED
    )
    {
        if (security == WLAN_SECURITY_WPA2)
        {
            bss.param.bss_config.protocol = PROTOCOL_WPA2;
            bss.param.bss_config.key_mgmt = KEY_MGMT_PSK;
        }
        else if (security == WLAN_SECURITY_WPA3_SAE)
        {
            bss.param.bss_config.protocol = PROTOCOL_WPA3_SAE;
            bss.param.bss_config.key_mgmt = KEY_MGMT_SAE;
        }
        else if (security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED)
        {
            bss.param.bss_config.protocol = PROTOCOL_WPA2 | PROTOCOL_WPA3_SAE;
            bss.param.bss_config.key_mgmt = KEY_MGMT_SAE | KEY_MGMT_PSK;
        }
        else
        { /* Do Nothing */
        }

        bss.param.bss_config.wpa_cfg.pairwise_cipher_wpa2 = CIPHER_AES_CCMP;
        bss.param.bss_config.wpa_cfg.group_cipher         = CIPHER_AES_CCMP;

        /***************************************
         * Operation UINT16 Bits[15:2]: Reserved      *
         *                                  Bit[1]: Authenticator     *
         *                                  Bit[0]: KeyExchange      *
         ****************************************/
        bss.param.bss_config.key_mgmt_operation = 0x00;

            if (security == WLAN_SECURITY_WPA2 || security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED)
            {
                /*app has converted pmk with psk*/
                bss.param.bss_config.wpa_cfg.length = (t_u32)passphrase_len;
                (void)memcpy((void *)bss.param.bss_config.wpa_cfg.passphrase, (const void *)passphrase,
                             (size_t)passphrase_len);
            }
        if (security == WLAN_SECURITY_WPA3_SAE || security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED)
        {
            bss.param.bss_config.wpa_cfg.password_length = (t_u32)password_len;
            (void)memcpy((void *)bss.param.bss_config.wpa_cfg.password, (const void *)password, (size_t)password_len);
        }
    }

    bss.param.bss_config.ht_cap_info = wm_wifi.ht_cap_info == 0 ? (t_u16)0x112c : wm_wifi.ht_cap_info;
    wm_wifi.ht_tx_cfg                = wm_wifi.ht_tx_cfg == 0 ? (t_u16)0x002c : wm_wifi.ht_tx_cfg;

    if (bandwidth == BANDWIDTH_40MHZ
#ifdef CONFIG_11AC
        || bandwidth == BANDWIDTH_80MHZ
#endif
    )
    {
        if (ISSUPP_CHANWIDTH40(mlan_adap->hw_dot_11n_dev_cap) != 0U)
        {
            bss.param.bss_config.ht_cap_info |= MBIT(1);
            wm_wifi.ht_tx_cfg |= MBIT(1);
            if (ISSUPP_SHORTGI40(mlan_adap->hw_dot_11n_dev_cap) != 0U)
            {
                bss.param.bss_config.ht_cap_info |= MBIT(6);
                wm_wifi.ht_tx_cfg |= MBIT(6);
            }
        }
    }
    else if (bandwidth == BANDWIDTH_20MHZ)
    {
        wm_wifi.ht_tx_cfg &= ~MBIT(1);
        wm_wifi.ht_tx_cfg &= ~MBIT(6);
    }
    else
    {
        /*Do Nothing*/
    }

    if (ISSUPP_RXLDPC(mlan_adap->hw_dot_11n_dev_cap) != 0U)
    {
        SETHT_LDPCCODINGCAP(bss.param.bss_config.ht_cap_info);
        SETHT_LDPCCODINGCAP(wm_wifi.ht_tx_cfg);
    }
    ret = wifi_uap_set_httxcfg_int(wm_wifi.ht_tx_cfg);
    if (ret != WM_SUCCESS)
    {
        wuap_e("Cannot set uAP HT TX Cfg:%x", wm_wifi.ht_tx_cfg);
        return -WM_E_INVAL;
    }
#ifdef CONFIG_11AC
    if (enable_11ac)
    {
        (void)wifi_uap_set_11ac_status(pmpriv, MLAN_ACT_ENABLE, bandwidth, channel);
    }
    else
    {
        (void)wifi_uap_set_11ac_status(pmpriv, MLAN_ACT_DISABLE, bandwidth, channel);
    }
#endif
#ifdef CONFIG_11AX
    if (pmpriv->adapter->enable_11ax)
    {
        wifi_uap_set_11ax_status(pmpriv, MLAN_ACT_ENABLE, bss.param.bss_config.band_cfg);
    }
    else
    {
        wifi_uap_set_11ax_status(pmpriv, MLAN_ACT_DISABLE, bss.param.bss_config.band_cfg);
    }
#endif
    bss.param.bss_config.ampdu_param = 0x03;
    (void)memcpy((void *)bss.param.bss_config.supported_mcs_set, (const void *)supported_mcs_set,
                 sizeof(bss.param.bss_config.supported_mcs_set));
    /*
     * Note that we are leaving htcap info set to zero by default. This
     *  will ensure that 11N is disabled.
     */

    mlan_ioctl_req ioctl_buf;
    (void)memset(&ioctl_buf, 0x00, sizeof(mlan_ioctl_req));

    ioctl_buf.req_id = (t_u32)MLAN_IOCTL_BSS;
    /** Pointer to buffer */
    ioctl_buf.pbuf = (t_u8 *)&bss;

    return wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_SYS_CONFIGURE, HostCmd_ACT_GEN_SET, 0, &ioctl_buf, NULL,
                                         bss_type, NULL);
}

static wifi_uap_11d_apis_t wifi_uap_11d_apis = {
    .wifi_uap_set_params_p           = wifi_uap_set_params,
    .wifi_uap_downld_domain_params_p = wifi_uap_downld_domain_params,
    .wifi_uap_enable_11d_p           = wifi_uap_enable_11d,

};

static void *wifi_uap_11d_support_apis = (wifi_uap_11d_apis_t *)&wifi_uap_11d_apis;

/*
 * We only save the params to be used later when BSS is (re-)started.
 */
int wifi_uap_set_domain_params(wifi_domain_param_t *dp)
{
    if (dp == MNULL)
    {
        return -WM_E_INVAL;
    }

    if (wm_wifi.dp != NULL)
    {
        wuap_w("Overwriting previous configuration");
    }

    /*
     * We have domain information from caller. Allocate tlv, fill it
     * up and store as pointer in global wifi structure. It will be
     * used later by the uap start code.
     */
    size_t sz = sizeof(MrvlIEtypes_DomainParamSet_t) + (sizeof(IEEEtypes_SubbandSet_t) * (dp->no_of_sub_band - 1));
    MrvlIEtypes_DomainParamSet_t *n_dp = os_mem_alloc(sz);
    if (n_dp == MNULL)
    {
        return -WM_E_NOMEM;
    }

    n_dp->header.type = TLV_TYPE_DOMAIN;
    n_dp->header.len  = (t_u16)(sizeof(MrvlIEtypes_DomainParamSet_t) - sizeof(MrvlIEtypesHeader_t) +
                               ((dp->no_of_sub_band - 1) * sizeof(IEEEtypes_SubbandSet_t)));

    (void)memcpy((void *)n_dp->country_code, (const void *)dp->country_code, COUNTRY_CODE_LEN);

    wifi_sub_band_set_t *is   = dp->sub_band;
    IEEEtypes_SubbandSet_t *s = n_dp->sub_band;
    t_u8 i;
    for (i = 0; i < dp->no_of_sub_band; i++)
    {
        s[i].first_chan = is[i].first_chan;
        s[i].no_of_chan = is[i].no_of_chan;
        s[i].max_tx_pwr = is[i].max_tx_pwr;
    }

    /* Store for use later */
    wuap_d("Saved domain info for later use");
    wm_wifi.dp = n_dp;

    (void)wifi_enable_11d_support_APIs();

    wm_wifi.uap_support_11d_apis = wifi_uap_11d_support_apis;

    return WM_SUCCESS;
}

int wifi_uap_set_params(int channel)
{
    int ret;
    wifi_sub_band_set_t *sub_band = NULL;
    t_u8 nr_sb                    = 0;
    if (channel > 14)
    {
#ifdef CONFIG_5GHz_SUPPORT
        sub_band = get_sub_band_from_country_5ghz(wifi_11d_country, &nr_sb);
#endif /* CONFIG_5GHz_SUPPORT */
    }
    else
    {
        sub_band = get_sub_band_from_country(wifi_11d_country, &nr_sb);
    }
    wifi_domain_param_t *dp = get_11d_domain_params(wifi_11d_country, sub_band, nr_sb);
    ret                     = wifi_uap_set_domain_params(dp);

    if (dp != NULL)
    {
        os_mem_free(dp);
    }

    return ret;
}

int wifi_uap_enable_11d_support(void)
{
    wuap_d("Enabling 11d support APIs");
    (void)wifi_enable_11d_support_APIs();

    wm_wifi.enable_11d_support   = true;
    wm_wifi.uap_support_11d_apis = wifi_uap_11d_support_apis;
    return WM_SUCCESS;
}

int wifi_uap_enable_11d(void)
{
    state_11d_t enable = ENABLE_11D;

    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    return wifi_uap_prepare_and_send_cmd(pmpriv, HostCmd_CMD_802_11_SNMP_MIB, HostCmd_ACT_GEN_SET, (t_u32)Dot11D_i,
                                         NULL, &enable, MLAN_BSS_TYPE_UAP, NULL);
}

int wifi_uap_ctrl_deauth(bool enable)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    return wifi_uap_prepare_and_send_cmd(pmpriv, HostCmd_CMD_802_11_SNMP_MIB, HostCmd_ACT_GEN_SET, (t_u32)StopDeauth_i,
                                         NULL, &enable, MLAN_BSS_TYPE_UAP, NULL);
}

void wifi_uap_set_beacon_period(const t_u16 beacon_period)
{
    wm_wifi.beacon_period = beacon_period;
}

int wifi_uap_set_bandwidth(const t_u8 bandwidth)
{
    if (bandwidth == BANDWIDTH_20MHZ || bandwidth == BANDWIDTH_40MHZ
#ifdef CONFIG_11AC
        || bandwidth == BANDWIDTH_80MHZ
#endif
    )
    {
        wm_wifi.bandwidth = bandwidth;
        return WM_SUCCESS;
    }
    return (-WM_FAIL);
}

void wifi_uap_set_hidden_ssid(const bool bcast_ssid_ctl)
{
    wm_wifi.bcast_ssid_ctl = bcast_ssid_ctl;
}

void wifi_uap_set_ecsa(void)
{
#if defined(SD8801)
    wm_wifi.chan_sw_count = 7;
#endif
}

void wifi_uap_set_htcapinfo(const t_u16 ht_cap_info)
{
    wm_wifi.ht_cap_info = ht_cap_info;
}

void wifi_uap_set_httxcfg(const t_u16 ht_tx_cfg)
{
    wm_wifi.ht_tx_cfg = ht_tx_cfg;
}

static int wifi_uap_pmf_getset(uint8_t action, uint8_t *mfpc, uint8_t *mfpr);

int wifi_uap_start(mlan_bss_type type,
                   char *ssid,
                   uint8_t *mac_addr,
                   int security,
                   char *passphrase,
                   char *password,
                   int channel,
                   wifi_scan_chan_list_t scan_chan_list,
                   bool mfpc,
                   bool mfpr
)
{
    wuap_d("Configuring");
#ifdef CONFIG_11AC
    if ((BANDWIDTH_80MHZ == wm_wifi.bandwidth) && (channel <= MAX_CHANNELS_BG))
    {
        wuap_e(
            "Cannot start uAP if bandwidth is configured to 80MHz, "
            "while channel is selected automatically or set to less than %d.",
            MAX_CHANNELS_BG);
        return -WM_FAIL;
    }
    else
    {
        /** Do Nothing */
    }
#endif
    /* Configure SSID */
    int rv =
        wifi_cmd_uap_config(ssid, mac_addr, (enum wlan_security_type)security, passphrase, password, (t_u8)channel,
                            scan_chan_list, wm_wifi.beacon_period == 0U ? UAP_BEACON_PERIOD : wm_wifi.beacon_period,
                            wm_wifi.bandwidth == 0U ? BANDWIDTH_40MHZ : wm_wifi.bandwidth,
                            UAP_DTIM_PERIOD,
                            wm_wifi.chan_sw_count, type);

    if (rv != WM_SUCCESS)
    {
        wuap_e("config failed. Cannot start");
        return rv;
    }

    if ((security == WLAN_SECURITY_WPA2 || security == WLAN_SECURITY_WPA3_SAE ||
         security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED
         ) &&
        mfpc)
    {
        (void)wifi_uap_pmf_getset(HostCmd_ACT_GEN_SET, (uint8_t *)&mfpc, (uint8_t *)&mfpr);
    }

    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    wifi_uap_enable_11d_support();

    if (wm_wifi.enable_11d_support && (wm_wifi.uap_support_11d_apis != MNULL))
    {
        wuap_d("Setting default domain params");
        (void)wm_wifi.uap_support_11d_apis->wifi_uap_set_params_p(channel);
    }

    if ((wm_wifi.dp != MNULL) && (wm_wifi.uap_support_11d_apis != MNULL) && type == MLAN_BSS_TYPE_UAP)
    {
        wuap_d("Downloading domain params");
        (void)wm_wifi.uap_support_11d_apis->wifi_uap_downld_domain_params_p(wm_wifi.dp);
        wuap_d("Enabling 11d");
        (void)wm_wifi.uap_support_11d_apis->wifi_uap_enable_11d_p();
    }

    wuap_d("Starting BSS");
    /* Start BSS */
    return wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_BSS_START, HostCmd_ACT_GEN_SET, 0, NULL, NULL, type,
                                         NULL);
}

int wifi_uap_bss_sta_list(wifi_sta_list_t **list)
{
    if (list == MNULL)
    {
        return -WM_E_INVAL;
    }

    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    /* Start BSS */
    return wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_STA_LIST, HostCmd_ACT_GEN_SET, 0, NULL, NULL,
                                         MLAN_BSS_TYPE_UAP, list);

    /* *list must have been filled now if everything went well */
}


int wifi_uap_stop(enum wlan_bss_type type)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    /* Start BSS */
    return wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_BSS_STOP, HostCmd_ACT_GEN_SET, 0, NULL, NULL,
                                         (mlan_bss_type)type, NULL);
}

#ifdef CONFIG_WIFI_UAP_WORKAROUND_STICKY_TIM
/*
 * The following configuration was added because of a particular
 * bug on 8787 and 8782 firmware. The firmware bugzilla ID for the
 * bug is 39609. The bug causes the firmware to send packets to the
 * STA even after giving ACK for IEEE PS. The prov. mode scanning
 * coupled with this bug causes large scale packet losses and TCP
 * backoffs finally resulting in long load times for HTTP served
 * pages for prov. UI.
 */

/** TLV header length */
#define TLVHEADER_LEN 4

#define MRVL_STICKY_TIM_CONFIG_TLV_ID (PROPRIETARY_TLV_BASE_ID + 0x96)
/** TLV : Sticky TIM MAC address */
#define MRVL_STICKY_TIM_STA_MAC_ADDR_TLV_ID (PROPRIETARY_TLV_BASE_ID + 0x97)

/** TLV buffer : sticky tim config */
typedef PACK_START struct _tlvbuf_sticky_tim_config
{
    /** Header */
    t_u16 tag;
    /** Length */
    t_u16 length;
    /** Enable */
    t_u16 enable;
    /** Duration */
    t_u16 duration;
    /** Sticky Bitmask */
    t_u16 sticky_bitmask;
} PACK_END tlvbuf_sticky_tim_config;

/** TLV buffer : sticky tim sta mac address */
typedef PACK_START struct _tlvbuf_sticky_tim_sta_mac_addr
{
    /** Header */
    t_u16 tag;
    /** Length */
    t_u16 length;
    /** Control */
    t_u16 control;
    /** Station MAC address */
    t_u8 sta_mac_address[MLAN_MAC_ADDR_LENGTH];
} PACK_END tlvbuf_sticky_tim_sta_mac_addr;

/*
 * This configures sticky TIM configuration for given MAC.
 *
 * Note that this is not present in mlan. So we have to add it here.
 */
void wifi_uap_enable_sticky_bit(const uint8_t *mac_addr)
{
    static bool global_sticky_enabled;

    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1;

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    sys_config_cmd->action                = HostCmd_ACT_GEN_SET;
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    if (!global_sticky_enabled)
    {
        /*
         * This enables the global sticky TIM bit enable. This
         * needs to be done only once.
         */
        tlvbuf_sticky_tim_config *tlv_sticky_tim_cfg = (tlvbuf_sticky_tim_config *)tlv;

        tlv_sticky_tim_cfg->tag    = MRVL_STICKY_TIM_CONFIG_TLV_ID;
        tlv_sticky_tim_cfg->length = sizeof(tlvbuf_sticky_tim_config) - TLVHEADER_LEN;
        tlv_sticky_tim_cfg->enable = 1;
        /* Set it permanently */
        tlv_sticky_tim_cfg->duration = 0;
        /* MoreData bit and TIM bit is made sticky */
        tlv_sticky_tim_cfg->sticky_bitmask = (t_u16)0x3;

        size += sizeof(MrvlIEtypesHeader_t) + tlv_sticky_tim_cfg->length;
        tlv += sizeof(MrvlIEtypesHeader_t) + tlv_sticky_tim_cfg->length;

        global_sticky_enabled = true;
    }

    tlvbuf_sticky_tim_sta_mac_addr *tim_cfg = (tlvbuf_sticky_tim_sta_mac_addr *)tlv;

    tim_cfg->tag    = MRVL_STICKY_TIM_STA_MAC_ADDR_TLV_ID;
    tim_cfg->length = sizeof(tlvbuf_sticky_tim_sta_mac_addr) - TLVHEADER_LEN;
    (void)memcpy((void *)tim_cfg->sta_mac_address, (const void *)mac_addr, MLAN_MAC_ADDR_LENGTH);
    tim_cfg->control = 1;

    size += sizeof(MrvlIEtypesHeader_t) + tim_cfg->length;

    cmd->size    = size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    wifi_wait_for_cmdresp(NULL);
}
#endif /* CONFIG_WIFI_UAP_WORKAROUND_STICKY_TIM */

/*
 * Note: This function handles only one (first) TLV from the response.
 */
void wifi_uap_handle_cmd_resp(HostCmd_DS_COMMAND *resp)
{
    mlan_private *pmpriv              = (mlan_private *)mlan_adap->priv[0];
    HostCmd_DS_SYS_CONFIG *sys_config = (HostCmd_DS_SYS_CONFIG *)&resp->params.sys_config;
    uint8_t *tlv                      = sys_config->tlv_buffer;
    MrvlIEtypesHeader_t *header       = (MrvlIEtypesHeader_t *)(void *)tlv;
    if (resp->result == 0U)
    {
        switch (header->type)
        {
            case TLV_TYPE_UAP_TX_POWER:
            {
                MrvlIEtypes_tx_power_t *tlv_tx_power = (MrvlIEtypes_tx_power_t *)(void *)tlv;
                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint8_t *tx_power     = (uint8_t *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv = NULL;
                        *tx_power             = tlv_tx_power->tx_power;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_STA_AGEOUT_TIMER:
            {
                MrvlIEtypes_sta_ageout_t *tlv_sta_ageout_timer = (MrvlIEtypes_sta_ageout_t *)(void *)tlv;
                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *sta_ageout_timer = (uint32_t *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv      = NULL;
                        *sta_ageout_timer          = tlv_sta_ageout_timer->sta_ageout_timer;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_PS_STA_AGEOUT_TIMER:
            {
                MrvlIEtypes_ps_sta_ageout_t *tlv_ps_sta_ageout_timer = (MrvlIEtypes_ps_sta_ageout_t *)(void *)tlv;
                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *ps_sta_ageout_timer = (uint32_t *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv         = NULL;
                        *ps_sta_ageout_timer          = tlv_ps_sta_ageout_timer->ps_sta_ageout_timer;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_GRP_REKEY_TIME:
            {
                MrvlIEtypes_group_rekey_time_t *tlv_group_rekey_timer = (MrvlIEtypes_group_rekey_time_t *)(void *)tlv;
                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *group_rekey_timer = (uint32_t *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv       = NULL;
                        *group_rekey_timer          = tlv_group_rekey_timer->gk_rekey_time;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_MCBC_DATA_RATE:
            {
                MrvlIEtypes_mcbc_rate_t *tlv_mcbc_rate = (MrvlIEtypes_mcbc_rate_t *)(void *)tlv;
                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint16_t *mcbc_rate   = (uint16_t *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv = NULL;
                        *mcbc_rate            = tlv_mcbc_rate->mcbc_data_rate;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_RATES:
            {
                MrvlIEtypes_RatesParamSet_t *tlv_rates = (MrvlIEtypes_RatesParamSet_t *)(void *)tlv;

                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        char *rates           = (char *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv = NULL;
                        (void)memset((void *)rates, 0, MAX_RATES);
                        (void)memcpy((void *)rates, (const void *)tlv_rates->rates,
                                     MIN(MAX_RATES, tlv_rates->header.len));
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_CHAN_BAND_CONFIG:
            {
                MrvlIEtypes_channel_band_t *tlv_cb    = (MrvlIEtypes_channel_band_t *)(void *)tlv;
                pmpriv->uap_state_chan_cb.band_config = tlv_cb->band_config;
                pmpriv->uap_state_chan_cb.channel     = tlv_cb->channel;
                if (!(sys_config->action == HostCmd_ACT_GEN_GET))
                {
                    break;
                }
                if (wm_wifi.cmd_resp_priv != NULL)
                {
                    t_u8 *channel         = (t_u8 *)wm_wifi.cmd_resp_priv;
                    wm_wifi.cmd_resp_priv = NULL;
                    *channel              = tlv_cb->channel;
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_MAX_STA_CNT:
            {
                MrvlIEtypes_max_sta_count_t *tlv_sta_cnt = (MrvlIEtypes_max_sta_count_t *)(void *)tlv;
                pmpriv->uap_state_chan_cb.max_sta_count  = tlv_sta_cnt->max_sta_count;
                if (!(sys_config->action == HostCmd_ACT_GEN_GET))
                {
                    break;
                }
                if (wm_wifi.cmd_resp_priv != NULL)
                {
                    int *sta_count        = (int *)wm_wifi.cmd_resp_priv;
                    wm_wifi.cmd_resp_priv = NULL;
                    *sta_count            = (int)tlv_sta_cnt->max_sta_count;
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            default:
                wm_wifi.cmd_resp_status = WM_SUCCESS;
                break;
        }
    }
    else
    {
        wm_wifi.cmd_resp_status = -WM_FAIL;
    }
}

int wifi_uap_rates_getset(uint8_t action, char *rates, uint8_t num_rates)
{
    uint8_t i     = 0;
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_RatesParamSet_t *tlv_rates = (MrvlIEtypes_RatesParamSet_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_SET;
        for (i = 0; i < num_rates; i++)
        {
            tlv_rates->rates[i] = (t_u8)rates[i];
        }
    }

    tlv_rates->header.type = wlan_cpu_to_le16(TLV_TYPE_RATES);
    tlv_rates->header.len  = wlan_cpu_to_le16(i);

    size += sizeof(MrvlIEtypesHeader_t) + i;
    tlv += sizeof(MrvlIEtypesHeader_t) + i;

    cmd->size    = (t_u16)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? rates : NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_uap_mcbc_rate_getset(uint8_t action, uint16_t *mcbc_rate)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_mcbc_rate_t *tlv_mcbc_rate = (MrvlIEtypes_mcbc_rate_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action        = HostCmd_ACT_GEN_SET;
        tlv_mcbc_rate->mcbc_data_rate = *mcbc_rate;
    }

    tlv_mcbc_rate->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_MCBC_DATA_RATE);
    tlv_mcbc_rate->header.len  = wlan_cpu_to_le16(sizeof(t_u16));

    size += sizeof(MrvlIEtypes_mcbc_rate_t);
    tlv += sizeof(MrvlIEtypes_mcbc_rate_t);

    cmd->size    = (t_u16)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? mcbc_rate : NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_uap_tx_power_getset(uint8_t action, uint8_t *tx_power_dbm)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_tx_power_t *tlv_tx_power = (MrvlIEtypes_tx_power_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_SET;
        tlv_tx_power->tx_power = *tx_power_dbm;
    }
    tlv_tx_power->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_TX_POWER);
    tlv_tx_power->header.len  = wlan_cpu_to_le16(sizeof(t_u8));

    size += sizeof(MrvlIEtypes_tx_power_t);
    tlv += sizeof(MrvlIEtypes_tx_power_t);

    cmd->size    = (uint16_t)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? tx_power_dbm : NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_uap_sta_ageout_timer_getset(uint8_t action, uint32_t *sta_ageout_timer)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_sta_ageout_t *tlv_sta_ageout_timer = (MrvlIEtypes_sta_ageout_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action                 = HostCmd_ACT_GEN_SET;
        tlv_sta_ageout_timer->sta_ageout_timer = wlan_cpu_to_le32(*sta_ageout_timer);
    }
    tlv_sta_ageout_timer->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_STA_AGEOUT_TIMER);
    tlv_sta_ageout_timer->header.len  = wlan_cpu_to_le16(sizeof(t_u32));

    size += sizeof(MrvlIEtypes_sta_ageout_t);
    tlv += sizeof(MrvlIEtypes_sta_ageout_t);

    cmd->size    = (t_u16)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? sta_ageout_timer : NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_uap_ps_sta_ageout_timer_getset(uint8_t action, uint32_t *ps_sta_ageout_timer)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_ps_sta_ageout_t *tlv_ps_sta_ageout_timer = (MrvlIEtypes_ps_sta_ageout_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action                       = HostCmd_ACT_GEN_SET;
        tlv_ps_sta_ageout_timer->ps_sta_ageout_timer = wlan_cpu_to_le32(*ps_sta_ageout_timer);
    }
    tlv_ps_sta_ageout_timer->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_PS_STA_AGEOUT_TIMER);
    tlv_ps_sta_ageout_timer->header.len  = wlan_cpu_to_le16(sizeof(t_u32));

    size += sizeof(MrvlIEtypes_ps_sta_ageout_t);
    tlv += sizeof(MrvlIEtypes_ps_sta_ageout_t);

    cmd->size    = (uint16_t)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? ps_sta_ageout_timer : NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_uap_group_rekey_timer_getset(uint8_t action, uint32_t *group_rekey_timer)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_group_rekey_time_t *tlv_group_rekey_timer = (MrvlIEtypes_group_rekey_time_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action               = HostCmd_ACT_GEN_SET;
        tlv_group_rekey_timer->gk_rekey_time = wlan_cpu_to_le32(*group_rekey_timer);
    }
    tlv_group_rekey_timer->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_GRP_REKEY_TIME);
    tlv_group_rekey_timer->header.len  = wlan_cpu_to_le16(sizeof(t_u32));

    size += sizeof(MrvlIEtypes_group_rekey_time_t);
    tlv += sizeof(MrvlIEtypes_group_rekey_time_t);

    cmd->size    = (t_u16)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? group_rekey_timer : NULL);

    return wm_wifi.cmd_resp_status;
}

/* Content reproduced from wlan_uap_get_channel() */
static int wifi_send_uap_get_channel_cmd(int *channel)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    t_u16 tlv_type       = TLV_TYPE_UAP_CHAN_BAND_CONFIG;

    int rv = wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_SYS_CONFIGURE, HostCmd_ACT_GEN_GET, 0, MNULL,
                                           &tlv_type, MLAN_BSS_TYPE_UAP, channel);
    if (rv != WM_SUCCESS)
    {
        return rv;
    }

    if (wm_wifi.cmd_resp_status != 0)
    {
        wifi_w("Unable to uap channel");
        return wm_wifi.cmd_resp_status;
    }

    return rv;
}

/* Content reproduced from wlan_uap_get_channel() */
static int wifi_send_uap_max_sta_num_cmd(uint8_t action, unsigned int *max_sta_num)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    MrvlIEtypes_max_sta_count_t max_sta_count_tlv;

    (void)memset(&max_sta_count_tlv, 0, sizeof(MrvlIEtypes_max_sta_count_t));

    max_sta_count_tlv.header.type = TLV_TYPE_UAP_MAX_STA_CNT;

    if (action == HostCmd_ACT_GEN_SET)
    {
        max_sta_count_tlv.header.len    = (t_u16)sizeof(MrvlIEtypes_max_sta_count_t);
        max_sta_count_tlv.max_sta_count = (t_u16)*max_sta_num;
    }

    int rv = wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_SYS_CONFIGURE, action, 0, MNULL, &max_sta_count_tlv,
                                           MLAN_BSS_TYPE_UAP, max_sta_num);
    if (rv != WM_SUCCESS)
    {
        return rv;
    }

    if (wm_wifi.cmd_resp_status != 0)
    {
        wifi_w("Unable to uap max_sta_num");
        return wm_wifi.cmd_resp_status;
    }

    return rv;
}

int wifi_get_uap_max_clients(unsigned int *max_sta_num)
{
    return wifi_send_uap_max_sta_num_cmd(HostCmd_ACT_GEN_GET, max_sta_num);
}

int wifi_set_uap_max_clients(unsigned int *max_sta_num)
{
    return wifi_send_uap_max_sta_num_cmd(HostCmd_ACT_GEN_SET, max_sta_num);
}

/*
 * Note: channel can be NULL. The side effects of this function also are of
 * interest to us as pmpriv->uap_state_chan_cb.band_config and
 * pmpriv->uap_state_chan_cb.channel are updated.
 */
int wifi_get_uap_channel(int *channel)
{
    return wifi_send_uap_get_channel_cmd(channel);
}

static int wifi_uap_pmf_getset(uint8_t action, uint8_t *mfpc, uint8_t *mfpr)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_PMF_PARAMS);
    wifi_pmf_params_t wifi_pmf_params;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HostCmd_CMD_PMF_PARAMS);
    HostCmd_DS_PMF_PARAMS *sys_pmf_params = (HostCmd_DS_PMF_PARAMS *)((uint32_t)cmd + S_DS_GEN);

    (void)memset(sys_pmf_params, 0x00, sizeof(HostCmd_DS_PMF_PARAMS));

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_pmf_params->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_pmf_params->action = HostCmd_ACT_GEN_SET;
    }

    sys_pmf_params->params.mfpc = *mfpc;
    sys_pmf_params->params.mfpr = *mfpr;

    cmd->size    = (t_u16)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? &wifi_pmf_params : NULL);

    if (action == HostCmd_ACT_GEN_GET)
    {
        *mfpc = wifi_pmf_params.mfpc;
        *mfpr = wifi_pmf_params.mfpr;
    }

    return wm_wifi.cmd_resp_status;
}

