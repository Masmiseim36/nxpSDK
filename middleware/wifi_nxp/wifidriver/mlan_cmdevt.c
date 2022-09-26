/** @file mlan_cmdevt.c
 *
 *  @brief  This file provides the handling of CMD/EVENT in MLAN
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

/*************************************************************
Change Log:
    05/12/2009: initial version
************************************************************/
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <wm_os.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>

/********************************************************
                Local Variables
********************************************************/

/*******************************************************
                Global Variables
********************************************************/

/********************************************************
                Local Functions
********************************************************/


/**
 *  @brief This function prepare the command before sending to firmware.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd_no       Command number
 *  @param cmd_action   Command action: GET or SET
 *  @param cmd_oid      Cmd oid: treated as sub command
 *  @param pioctl_buf   A pointer to MLAN IOCTL Request buffer
 *  @param pdata_buf    A pointer to information buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_prepare_cmd(IN mlan_private *pmpriv,
                             IN t_u16 cmd_no,
                             IN t_u16 cmd_action,
                             IN t_u32 cmd_oid,
                             IN t_void *pioctl_buf,
                             IN t_void *pdata_buf)
{
    /* Note: We send only one command at a time and do not need the linked
       list based implementation used here. So we will call our own
       implementation here.
    */

    return wifi_prepare_and_send_cmd(pmpriv, cmd_no, cmd_action, cmd_oid, pioctl_buf, pdata_buf, pmpriv->bss_type,
                                     NULL);
}

#ifdef CONFIG_11AX
/**
 *  @brief Fetch bitmap rate index
 *
 *  @param rate_scope  A pointer to MrvlRateScope_t
 *
 *  @return            bitmap rate index
 */
static t_u16 wlan_get_bitmap_index(MrvlRateScope_t *rate_scope)
{
    t_u16 index = 0;
    if (rate_scope != MNULL)
    {
        index += NELEMENTS(rate_scope->ht_mcs_rate_bitmap);
        index += NELEMENTS(rate_scope->vht_mcs_rate_bitmap);
    }
    return index;
}
#endif

/**
 *  @brief This function prepares command of power mode
 *
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param ps_bitmap    PS bitmap
 *  @param pdata_buf    A pointer to data buffer
 *  @return         MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_enh_power_mode(pmlan_private pmpriv,
                                    IN HostCmd_DS_COMMAND *cmd,
                                    IN ENH_PS_MODES cmd_action,
                                    IN t_u16 ps_bitmap,
                                    IN t_void *pdata_buf)
{
    HostCmd_DS_802_11_PS_MODE_ENH *psmode_enh = &cmd->params.psmode_enh;
    t_u8 *tlv                                 = MNULL;
    t_u16 cmd_size                            = 0;

    ENTER();

    PRINTM(MCMND, "PS Command: action = 0x%x, bitmap = 0x%x\n", cmd_action, ps_bitmap);

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_PS_MODE_ENH);
    if (cmd_action == DIS_AUTO_PS)
    {
        psmode_enh->action           = (ENH_PS_MODES)(wlan_cpu_to_le16(DIS_AUTO_PS));
        psmode_enh->params.ps_bitmap = wlan_cpu_to_le16(ps_bitmap);
        cmd->size                    = wlan_cpu_to_le16(S_DS_GEN + AUTO_PS_FIX_SIZE);
    }
    else if (cmd_action == GET_PS)
    {
        psmode_enh->action           = (ENH_PS_MODES)(wlan_cpu_to_le16(GET_PS));
        psmode_enh->params.ps_bitmap = wlan_cpu_to_le16(ps_bitmap);
        cmd->size                    = wlan_cpu_to_le16(S_DS_GEN + AUTO_PS_FIX_SIZE);
    }
    else if (cmd_action == EN_AUTO_PS)
    {
        psmode_enh->action                   = (ENH_PS_MODES)(wlan_cpu_to_le16(EN_AUTO_PS));
        psmode_enh->params.auto_ps.ps_bitmap = wlan_cpu_to_le16(ps_bitmap);
        cmd_size                             = S_DS_GEN + AUTO_PS_FIX_SIZE;
        tlv                                  = (t_u8 *)cmd + cmd_size;
        if ((ps_bitmap & BITMAP_STA_PS) != 0U)
        {
            pmlan_adapter pmadapter        = pmpriv->adapter;
            MrvlIEtypes_ps_param_t *ps_tlv = (MrvlIEtypes_ps_param_t *)(void *)tlv;
            ps_param *ps_mode              = (ps_param *)&ps_tlv->param;
            ps_tlv->header.type            = wlan_cpu_to_le16(TLV_TYPE_PS_PARAM);
            ps_tlv->header.len = wlan_cpu_to_le16(sizeof(MrvlIEtypes_ps_param_t) - sizeof(MrvlIEtypesHeader_t));
            cmd_size += (t_u16)sizeof(MrvlIEtypes_ps_param_t);
            tlv += (t_u8)sizeof(MrvlIEtypes_ps_param_t);
            ps_mode->null_pkt_interval     = wlan_cpu_to_le16(pmadapter->null_pkt_interval);
            ps_mode->multiple_dtims        = wlan_cpu_to_le16(pmadapter->multiple_dtim);
            ps_mode->bcn_miss_timeout      = wlan_cpu_to_le16(pmadapter->bcn_miss_time_out);
            ps_mode->local_listen_interval = wlan_cpu_to_le16(pmadapter->local_listen_interval);
            ps_mode->adhoc_wake_period     = wlan_cpu_to_le16(pmadapter->adhoc_awake_period);
            ps_mode->delay_to_ps           = wlan_cpu_to_le16(pmadapter->delay_to_ps);
            ps_mode->mode                  = wlan_cpu_to_le16(pmadapter->enhanced_ps_mode);
        }

        if ((ps_bitmap & BITMAP_AUTO_DS) != 0U)
        {
            MrvlIEtypes_auto_ds_param_t *auto_ps_tlv = (MrvlIEtypes_auto_ds_param_t *)(void *)tlv;
            auto_ds_param *auto_ds                   = (auto_ds_param *)&auto_ps_tlv->param;
            t_u16 idletime                           = 0;
            auto_ps_tlv->header.type                 = wlan_cpu_to_le16(TLV_TYPE_AUTO_DS_PARAM);
            auto_ps_tlv->header.len =
                wlan_cpu_to_le16(sizeof(MrvlIEtypes_auto_ds_param_t) - sizeof(MrvlIEtypesHeader_t));
            cmd_size += (t_u16)sizeof(MrvlIEtypes_auto_ds_param_t);
            tlv += (t_u8)sizeof(MrvlIEtypes_auto_ds_param_t);
            if (pdata_buf != NULL)
            {
                idletime = ((mlan_ds_auto_ds *)pdata_buf)->idletime;
            }
            auto_ds->deep_sleep_timeout = wlan_cpu_to_le16(idletime);
        }
        /* fixme :
         * This macro is not defined as if now
         * once full fledged support is added in the SDK
         * for UAP this macro will be defined and
         * line below will be uncommented*/
        /* #if defined(UAP_SUPPORT)*/
        if ((pdata_buf != MNULL) && (ps_bitmap & (BITMAP_UAP_INACT_PS | BITMAP_UAP_DTIM_PS)))
        {
            mlan_ds_ps_mgmt *ps_mgmt                   = (mlan_ds_ps_mgmt *)pdata_buf;
            MrvlIEtypes_sleep_param_t *sleep_tlv       = MNULL;
            MrvlIEtypes_inact_sleep_param_t *inact_tlv = MNULL;
            if ((ps_mgmt->flags & PS_FLAG_SLEEP_PARAM) != 0U)
            {
                sleep_tlv              = (MrvlIEtypes_sleep_param_t *)(void *)tlv;
                sleep_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_AP_SLEEP_PARAM);
                sleep_tlv->header.len =
                    wlan_cpu_to_le16(sizeof(MrvlIEtypes_sleep_param_t) - sizeof(MrvlIEtypesHeader_t));
                sleep_tlv->ctrl_bitmap = wlan_cpu_to_le32(ps_mgmt->sleep_param.ctrl_bitmap);
                sleep_tlv->min_sleep   = wlan_cpu_to_le32(ps_mgmt->sleep_param.min_sleep);
                sleep_tlv->max_sleep   = wlan_cpu_to_le32(ps_mgmt->sleep_param.max_sleep);
                cmd_size += (t_u16)sizeof(MrvlIEtypes_sleep_param_t);
                tlv += (t_u8)sizeof(MrvlIEtypes_sleep_param_t);
            }
            if ((ps_mgmt->flags & PS_FLAG_INACT_SLEEP_PARAM) != 0U)
            {
                inact_tlv              = (MrvlIEtypes_inact_sleep_param_t *)(void *)tlv;
                inact_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_AP_INACT_SLEEP_PARAM);
                inact_tlv->header.len =
                    wlan_cpu_to_le16(sizeof(MrvlIEtypes_inact_sleep_param_t) - sizeof(MrvlIEtypesHeader_t));
                inact_tlv->inactivity_to = wlan_cpu_to_le32(ps_mgmt->inact_param.inactivity_to);
                inact_tlv->min_awake     = wlan_cpu_to_le32(ps_mgmt->inact_param.min_awake);
                inact_tlv->max_awake     = wlan_cpu_to_le32(ps_mgmt->inact_param.max_awake);
                cmd_size += (t_u16)sizeof(MrvlIEtypes_inact_sleep_param_t);
                tlv += (t_u8)sizeof(MrvlIEtypes_inact_sleep_param_t);
            }
        }
        /*#endif*/
        cmd->size = wlan_cpu_to_le16(cmd_size);
    }
    else
    { /* Do Nothing */
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}


#ifdef SD8801
mlan_status wlan_ret_802_11_tx_rate_query(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN void *pioctl)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    wifi_ds_rate *rate      = MNULL;
    ENTER();

    pmpriv->tx_rate = resp->params.tx_rate.tx_rate;
    pmpriv->tx_htinfo = resp->params.tx_rate.ht_info;
    if (!pmpriv->is_data_rate_auto)
    {
        pmpriv->data_rate = wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate, pmpriv->tx_htinfo);
    }

    if (pioctl)
    {
        rate = (wifi_ds_rate *)pioctl;
        if (rate->sub_command == WIFI_DS_RATE_CFG)
        {
#if 0
            if(rate->param.rate_cfg.rate_type == MLAN_RATE_INDEX) {
#endif
            if (pmpriv->tx_htinfo & MBIT(0))
                rate->param.rate_cfg.rate = pmpriv->tx_rate + MLAN_RATE_INDEX_MCS0;
            else
                /* For HostCmd_CMD_802_11_TX_RATE_QUERY, there is a hole in rate table
                 * between HR/DSSS and OFDM rates, so minus 1 for OFDM rate index */
                rate->param.rate_cfg.rate =
                    (pmpriv->tx_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->tx_rate - 1 : pmpriv->tx_rate;
#if 0
            }
            else {
                    rate->param.rate_cfg.rate = wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate,
                                                pmpriv->tx_htinfo);
            }
#endif
        }
        else if (rate->sub_command == WIFI_DS_GET_DATA_RATE)
        {
            if (pmpriv->tx_htinfo & MBIT(0))
            {
                rate->param.data_rate.tx_data_rate = pmpriv->tx_rate + MLAN_RATE_INDEX_MCS0;
                if (pmpriv->tx_htinfo & MBIT(1))
                    rate->param.data_rate.tx_ht_bw = MLAN_HT_BW40;
                else
                    rate->param.data_rate.tx_ht_bw = MLAN_HT_BW20;
                if (pmpriv->tx_htinfo & MBIT(2))
                    rate->param.data_rate.tx_ht_gi = MLAN_HT_SGI;
                else
                    rate->param.data_rate.tx_ht_gi = MLAN_HT_LGI;
            }
            else
                /* For HostCmd_CMD_802_11_TX_RATE_QUERY, there is a hole in rate table
                   between HR/DSSS and OFDM rates, so minus 1 for OFDM rate index */
                rate->param.data_rate.tx_data_rate =
                    (pmpriv->tx_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->tx_rate - 1 : pmpriv->tx_rate;
            if (pmpriv->rxpd_htinfo & MBIT(0))
            {
                rate->param.data_rate.rx_data_rate = pmpriv->rxpd_rate + MLAN_RATE_INDEX_MCS0;
                if (pmpriv->rxpd_htinfo & MBIT(1))
                    rate->param.data_rate.rx_ht_bw = MLAN_HT_BW40;
                else
                    rate->param.data_rate.rx_ht_bw = MLAN_HT_BW20;
                if (pmpriv->rxpd_htinfo & MBIT(2))
                    rate->param.data_rate.rx_ht_gi = MLAN_HT_SGI;
                else
                    rate->param.data_rate.rx_ht_gi = MLAN_HT_LGI;
            }
            else
                /* For rate index in RxPD, there is a hole in rate table
                   between HR/DSSS and OFDM rates, so minus 1 for OFDM rate index */
                rate->param.data_rate.rx_data_rate =
                    (pmpriv->rxpd_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->rxpd_rate - 1 : pmpriv->rxpd_rate;
        }
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#else
/**
 *  @brief This function handles the command response of tx rate query
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_802_11_tx_rate_query(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN void *pioctl)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    wifi_ds_rate *rate      = MNULL;
    ENTER();

    pmpriv->tx_rate      = resp->params.tx_rate.tx_rate;
    pmpriv->tx_rate_info = resp->params.tx_rate.tx_rate_info;

#ifdef CONFIG_11AX
    if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HE)
        pmpriv->ext_tx_rate_info = resp->params.tx_rate.ext_tx_rate_info;
    else
#endif

        if (!pmpriv->is_data_rate_auto)
    {
        pmpriv->data_rate =
            wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate, pmpriv->tx_rate_info, pmpriv->ext_tx_rate_info);
    }

    if (pioctl != NULL)
    {
        rate = (wifi_ds_rate *)pioctl;
        if (rate->sub_command == WIFI_DS_RATE_CFG)
        {
#if 0
            if(rate->param.rate_cfg.rate_type == MLAN_RATE_INDEX) {
#endif
#ifdef CONFIG_11AC
            if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_VHT
#ifdef CONFIG_11AX
                || ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HE)
#endif
            )
            {
                /* VHT rate */
                rate->param.rate_cfg.rate = (t_u32)((pmpriv->tx_rate) & 0xF);
            }
            else
#endif
                if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HT)
            {
                /* HT rate */
                rate->param.rate_cfg.rate = pmpriv->tx_rate + MLAN_RATE_INDEX_MCS0;
            }
            else
            {
                /* LG rate */
                /* For HostCmd_CMD_802_11_TX_RATE_QUERY,
                 * there is a hole (0x4) in rate table
                 * between HR/DSSS and OFDM rates,
                 * so minus 1 for OFDM rate index */
                rate->param.rate_cfg.rate =
                    (pmpriv->tx_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->tx_rate - 1U : pmpriv->tx_rate;
            }
#if 0
            }
            else {
                /* rate_type = MLAN_RATE_VALUE */
                rate->param.rate_cfg.rate = wlan_index_to_data_rate(pmadapter,
                                                pmpriv->tx_rate,
                                                pmpriv->tx_rate_info);
            }
#endif
        }
        else if (rate->sub_command == WIFI_DS_GET_DATA_RATE)
        {
            /* Tx rate info */
#ifdef CONFIG_11AC
            if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_VHT
#ifdef CONFIG_11AX
                || (mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HE
#endif
            )
            {
                /* VHT/HE rate */
                rate->param.data_rate.tx_rate_format = (mlan_rate_format)(pmpriv->tx_rate_info & 0x3U);
                rate->param.data_rate.tx_ht_bw       = (t_u32)((pmpriv->tx_rate_info & 0xC) >> 2);

#ifdef CONFIG_11AX
                if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HE)
                    rate->param.data_rate.tx_ht_gi =
                        (pmpriv->tx_rate_info & 0x10) >> 4 | (pmpriv->tx_rate_info & 0x80) >> 6;
                else
#endif
                    rate->param.data_rate.tx_ht_gi = (t_u32)((pmpriv->tx_rate_info & 0x10) >> 4);
                rate->param.data_rate.tx_nss       = (pmpriv->tx_rate) >> 4;
                rate->param.data_rate.tx_mcs_index = (t_u32)((pmpriv->tx_rate) & 0xF);
                rate->param.data_rate.tx_data_rate =
                    wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate, pmpriv->tx_rate_info, pmpriv->ext_tx_rate_info);
            }
            else
#endif
                if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HT)
            {
                /* HT rate */
                rate->param.data_rate.tx_rate_format = MLAN_RATE_FORMAT_HT;
                rate->param.data_rate.tx_ht_bw       = (pmpriv->tx_rate_info & 0xCU) >> 2U;
                rate->param.data_rate.tx_ht_gi       = (pmpriv->tx_rate_info & 0x10U) >> 4U;
                rate->param.data_rate.tx_mcs_index   = pmpriv->tx_rate;
                rate->param.data_rate.tx_data_rate =
                    wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate, pmpriv->tx_rate_info, pmpriv->ext_tx_rate_info);
            }
            else
            {
                /* LG rate */
                rate->param.data_rate.tx_rate_format = MLAN_RATE_FORMAT_LG;
                /* For HostCmd_CMD_802_11_TX_RATE_QUERY,
                 * there is a hole in rate table
                 * between HR/DSSS and OFDM rates,
                 * so minus 1 for OFDM rate index */
                rate->param.data_rate.tx_data_rate =
                    (pmpriv->tx_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->tx_rate - 1U : pmpriv->tx_rate;
            }

            /* Rx rate info */
#ifdef CONFIG_11AC
            if ((mlan_rate_format)(pmpriv->rxpd_rate_info & 0x3U) == MLAN_RATE_FORMAT_VHT
#ifdef CONFIG_11AX
                || (pmpriv->rxpd_rate_info & 0x3) == MLAN_RATE_FORMAT_HE
#endif
            )
            {
                /* VHT/HE rate */
                rate->param.data_rate.rx_rate_format = (mlan_rate_format)(pmpriv->rxpd_rate_info & 0x3);
                rate->param.data_rate.rx_ht_bw       = (t_u32)((pmpriv->rxpd_rate_info & 0xC) >> 2);

#ifdef CONFIG_11AX
                if ((pmpriv->rxpd_rate_info & 0x3) == MLAN_RATE_FORMAT_HE)
                    rate->param.data_rate.rx_ht_gi =
                        (pmpriv->rxpd_rate_info & 0x10) >> 4 | (pmpriv->rxpd_rate_info & 0x80) >> 6;
                else
#endif
                    rate->param.data_rate.rx_ht_gi = (t_u32)((pmpriv->rxpd_rate_info & 0x10) >> 4);
                rate->param.data_rate.rx_nss       = (pmpriv->rxpd_rate) >> 4;
                rate->param.data_rate.rx_mcs_index = (t_u32)((pmpriv->rxpd_rate) & 0xF);
                rate->param.data_rate.rx_data_rate = wlan_index_to_data_rate(
                    pmadapter, pmpriv->rxpd_rate, pmpriv->rxpd_rate_info, pmpriv->ext_tx_rate_info);
            }
            else
#endif
                if ((pmpriv->rxpd_rate_info & 0x3) == MLAN_RATE_FORMAT_HT)
            {
                /* HT rate */
                rate->param.data_rate.rx_rate_format = MLAN_RATE_FORMAT_HT;
                rate->param.data_rate.rx_ht_bw       = (pmpriv->rxpd_rate_info & 0xCU) >> 2U;
                rate->param.data_rate.rx_ht_gi       = (pmpriv->rxpd_rate_info & 0x10U) >> 4U;
                rate->param.data_rate.rx_mcs_index   = pmpriv->rxpd_rate;
                rate->param.data_rate.rx_data_rate   = wlan_index_to_data_rate(
                    pmadapter, pmpriv->rxpd_rate, pmpriv->rxpd_rate_info, pmpriv->ext_tx_rate_info);
            }
            else
            {
                /* LG rate */
                rate->param.data_rate.rx_rate_format = MLAN_RATE_FORMAT_LG;
                /* For rate index in RxPD,
                 * there is a hole in rate table
                 * between HR/DSSS and OFDM rates,
                 * so minus 1 for OFDM rate index */
                rate->param.data_rate.rx_data_rate =
                    (t_u32)((pmpriv->rxpd_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->rxpd_rate - 1 : pmpriv->rxpd_rate);
            }
        }
        else
        { /* Do Nothing */
        }
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

/**
 *  @brief This function prepares command of tx_rate_cfg.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_tx_rate_cfg(IN pmlan_private pmpriv,
                                 IN HostCmd_DS_COMMAND *cmd,
                                 IN t_u16 cmd_action,
                                 IN t_void *pdata_buf)
{
    HostCmd_DS_TX_RATE_CFG *rate_cfg = &cmd->params.tx_rate_cfg;
    MrvlRateScope_t *rate_scope;
    MrvlRateDropPattern_t *rate_drop;
    t_u16 *pbitmap_rates = (t_u16 *)pdata_buf;

    t_u32 i;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_TX_RATE_CFG);

    rate_cfg->action    = wlan_cpu_to_le16(cmd_action);
    rate_cfg->cfg_index = 0;

    rate_scope         = (MrvlRateScope_t *)(void *)((t_u8 *)rate_cfg + sizeof(HostCmd_DS_TX_RATE_CFG));
    rate_scope->type   = wlan_cpu_to_le16(TLV_TYPE_RATE_SCOPE);
    rate_scope->length = wlan_cpu_to_le16(sizeof(MrvlRateScope_t) - sizeof(MrvlIEtypesHeader_t));
    if (pbitmap_rates != MNULL)
    {
        rate_scope->hr_dsss_rate_bitmap = wlan_cpu_to_le16(pbitmap_rates[0]);
        rate_scope->ofdm_rate_bitmap    = wlan_cpu_to_le16(pbitmap_rates[1]);
        for (i = 0; i < NELEMENTS(rate_scope->ht_mcs_rate_bitmap); i++)
        {
            rate_scope->ht_mcs_rate_bitmap[i] = wlan_cpu_to_le16(pbitmap_rates[2U + i]);
        }
        for (i = 0; i < NELEMENTS(rate_scope->vht_mcs_rate_bitmap); i++)
        {
            rate_scope->vht_mcs_rate_bitmap[i] =
                wlan_cpu_to_le16(pbitmap_rates[2U + NELEMENTS(rate_scope->ht_mcs_rate_bitmap) + i]);
        }
#ifdef CONFIG_11AX
        if (IS_FW_SUPPORT_11AX(pmpriv->adapter))
        {
            for (i = 0; i < NELEMENTS(rate_scope->he_mcs_rate_bitmap); i++)
                rate_scope->he_mcs_rate_bitmap[i] =
                    wlan_cpu_to_le16(pbitmap_rates[2U + wlan_get_bitmap_index(rate_scope) + i]);
        }
        else
        {
            rate_scope->length = wlan_cpu_to_le16(sizeof(MrvlRateScope_t) - sizeof(rate_scope->he_mcs_rate_bitmap) -
                                                  sizeof(MrvlIEtypesHeader_t));
        }
#endif
    }
    else
    {
        rate_scope->hr_dsss_rate_bitmap = wlan_cpu_to_le16(pmpriv->bitmap_rates[0]);
        rate_scope->ofdm_rate_bitmap    = wlan_cpu_to_le16(pmpriv->bitmap_rates[1]);
        for (i = 0; i < NELEMENTS(rate_scope->ht_mcs_rate_bitmap); i++)
        {
            rate_scope->ht_mcs_rate_bitmap[i] = wlan_cpu_to_le16(pmpriv->bitmap_rates[2U + i]);
        }
        for (i = 0; i < NELEMENTS(rate_scope->vht_mcs_rate_bitmap); i++)
        {
            rate_scope->vht_mcs_rate_bitmap[i] =
                wlan_cpu_to_le16(pmpriv->bitmap_rates[2U + NELEMENTS(rate_scope->ht_mcs_rate_bitmap) + i]);
        }
#ifdef CONFIG_11AX
        if (IS_FW_SUPPORT_11AX(pmpriv->adapter))
        {
            for (i = 0; i < NELEMENTS(rate_scope->he_mcs_rate_bitmap); i++)
                rate_scope->he_mcs_rate_bitmap[i] =
                    wlan_cpu_to_le16(pmpriv->bitmap_rates[2U + wlan_get_bitmap_index(rate_scope) + i]);
        }
        else
        {
            rate_scope->length = wlan_cpu_to_le16(sizeof(MrvlRateScope_t) - sizeof(rate_scope->he_mcs_rate_bitmap) -
                                                  sizeof(MrvlIEtypesHeader_t));
        }
#endif
    }

    rate_drop                 = (MrvlRateDropPattern_t *)(void *)((t_u8 *)rate_scope + sizeof(MrvlRateScope_t));
    rate_drop->type           = wlan_cpu_to_le16(TLV_TYPE_RATE_DROP_PATTERN);
    rate_drop->length         = wlan_cpu_to_le16(sizeof(rate_drop->rate_drop_mode));
    rate_drop->rate_drop_mode = 0;

    cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_TX_RATE_CFG) + sizeof(MrvlRateScope_t) +
                                 sizeof(MrvlRateDropPattern_t));

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of tx_rate_cfg
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_ret_tx_rate_cfg(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN void *pioctl)
{
    mlan_adapter *pmadapter           = pmpriv->adapter;
    wifi_ds_rate *ds_rate             = MNULL;
    HostCmd_DS_TX_RATE_CFG *prate_cfg = MNULL;
    MrvlRateScope_t *prate_scope;
    MrvlIEtypesHeader_t *head = MNULL;
    t_u16 tlv;
    t_u16 tlv_buf_len = 0;
    t_u8 *tlv_buf;
    t_u32 i;
    t_s32 index;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    if (resp == MNULL)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    prate_cfg = (HostCmd_DS_TX_RATE_CFG *)&(resp->params.tx_rate_cfg);

    tlv_buf     = (t_u8 *)((t_u8 *)prate_cfg) + sizeof(HostCmd_DS_TX_RATE_CFG);
    tlv_buf_len = *(t_u16 *)(void *)(tlv_buf + sizeof(t_u16));
    tlv_buf_len = wlan_le16_to_cpu(tlv_buf_len);

    while (tlv_buf_len > 0U)
    {
        tlv = (*tlv_buf);
        tlv = tlv | (*(tlv_buf + 1) << 8);

        switch (tlv)
        {
            case TLV_TYPE_RATE_SCOPE:
                prate_scope             = (MrvlRateScope_t *)(void *)tlv_buf;
                pmpriv->bitmap_rates[0] = wlan_le16_to_cpu(prate_scope->hr_dsss_rate_bitmap);
                pmpriv->bitmap_rates[1] = wlan_le16_to_cpu(prate_scope->ofdm_rate_bitmap);
                for (i = 0; i < sizeof(prate_scope->ht_mcs_rate_bitmap) / sizeof(t_u16); i++)
                {
                    pmpriv->bitmap_rates[2U + i] = wlan_le16_to_cpu(prate_scope->ht_mcs_rate_bitmap[i]);
                }
#ifdef CONFIG_11AC
                for (i = 0; i < NELEMENTS(prate_scope->vht_mcs_rate_bitmap); i++)
                {
                    pmpriv->bitmap_rates[2 + sizeof(prate_scope->ht_mcs_rate_bitmap) / sizeof(t_u16) + i] =
                        wlan_le16_to_cpu(prate_scope->vht_mcs_rate_bitmap[i]);
                }

#endif
#ifdef CONFIG_11AX
                if (IS_FW_SUPPORT_11AX(pmadapter))
                {
                    for (i = 0; i < NELEMENTS(prate_scope->he_mcs_rate_bitmap); i++)
                    {
                        pmpriv->bitmap_rates[2 + sizeof(prate_scope->ht_mcs_rate_bitmap) / sizeof(t_u16) +
                                             sizeof(prate_scope->vht_mcs_rate_bitmap) / sizeof(t_u16) + i] =
                            wlan_le16_to_cpu(prate_scope->he_mcs_rate_bitmap[i]);
                    }
                }
#endif
                break;
                /* Add RATE_DROP tlv here */
            default:
                PRINTM(MINFO, "Unexpected TLV for rate cfg \n");
                break;
        }

        head      = (MrvlIEtypesHeader_t *)(void *)tlv_buf;
        head->len = wlan_le16_to_cpu(head->len);
        tlv_buf += head->len + sizeof(MrvlIEtypesHeader_t);
        tlv_buf_len -= head->len;
    }

    pmpriv->is_data_rate_auto = wlan_is_rate_auto(pmpriv);

    if (pmpriv->is_data_rate_auto != 0U)
    {
        pmpriv->data_rate = 0;
        PRINTM(MINFO, "Rate is Auto\r\n");
    }

    if (pioctl != NULL)
    {
        ds_rate = (wifi_ds_rate *)pioctl;
        if (ds_rate == MNULL)
        {
            PRINTM(MERROR, "Request buffer not found!\n");
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        if (pmpriv->is_data_rate_auto != 0U)
        {
            // ds_rate->param.rate_cfg.is_rate_auto = MTRUE;
            ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_AUTO;
        }
        else
        {
            /* check the LG rate */
            index = wlan_get_rate_index(pmadapter, &pmpriv->bitmap_rates[0], 4);
            if (index != -1)
            {
                if ((index >= MLAN_RATE_BITMAP_OFDM0) && (index <= MLAN_RATE_BITMAP_OFDM7))
                {
                    index -= (MLAN_RATE_BITMAP_OFDM0 - MLAN_RATE_INDEX_OFDM0);
                }

#ifndef SD8801
                ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_LG;
#endif
                ds_rate->param.rate_cfg.rate = (t_u32)index;
            }
            /* check the HT rate */
            index = wlan_get_rate_index(pmadapter, &pmpriv->bitmap_rates[2], 16);
            if (index != -1)
            {
#ifdef SD8801
                ds_rate->param.rate_cfg.rate = index + MLAN_RATE_INDEX_MCS0;
#else
                ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_HT;
                ds_rate->param.rate_cfg.rate        = (t_u32)index;
#endif
            }

#ifdef CONFIG_11AC
            /* check the VHT rate */
            index = wlan_get_rate_index(pmadapter, &pmpriv->bitmap_rates[10], 16);

            if (index != -1)
            {
                ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_VHT;
                ds_rate->param.rate_cfg.rate        = (t_u32)(index % 16);
                ds_rate->param.rate_cfg.nss         = (t_u32)(index / 16);
                ds_rate->param.rate_cfg.nss += MLAN_RATE_NSS1;
            }
#endif
#ifdef CONFIG_11AX
            /* check the HE rate */
            if (IS_FW_SUPPORT_11AX(pmadapter))
            {
                index = wlan_get_rate_index(pmadapter, &pmpriv->bitmap_rates[18], 16);
                if (index != -1)
                {
                    ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_HE;
                    ds_rate->param.rate_cfg.rate        = index % 16;
                    ds_rate->param.rate_cfg.nss         = index / 16;
                    ds_rate->param.rate_cfg.nss += MLAN_RATE_NSS1;
                }
            }
#endif
            PRINTM(MINFO, "Rate index is %d\n", ds_rate->param.rate_cfg.rate);

#ifdef SD8801
            if (ds_rate->param.rate_cfg.rate < MLAN_RATE_INDEX_MCS0)
            {
                ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_LG;
                ds_rate->param.rate_cfg.rate_index  = ds_rate->param.rate_cfg.rate;
            }
            else
            {
                ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_HT;
                ds_rate->param.rate_cfg.rate_index  = ds_rate->param.rate_cfg.rate - MLAN_RATE_INDEX_MCS0;
            }
#else
            ds_rate->param.rate_cfg.rate_index = ds_rate->param.rate_cfg.rate;
#endif
        }
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function prepares command of get_hw_spec.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pcmd         A pointer to HostCmd_DS_COMMAND structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_get_hw_spec(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *pcmd)
{
    HostCmd_DS_GET_HW_SPEC *hw_spec = &pcmd->params.hw_spec;

    ENTER();

    pcmd->command = wlan_cpu_to_le16(HostCmd_CMD_GET_HW_SPEC);
    pcmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_GET_HW_SPEC) + S_DS_GEN);
    (void)__memcpy(pmpriv->adapter, hw_spec->permanent_addr, pmpriv->curr_addr, MLAN_MAC_ADDR_LENGTH);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of HostCmd_CMD_GET_TSF
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_get_tsf(pmlan_private pmpriv, IN HostCmd_DS_COMMAND *cmd, IN t_u16 cmd_action)
{
    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_GET_TSF);
    cmd->size    = wlan_cpu_to_le16((sizeof(HostCmd_DS_TSF)) + S_DS_GEN);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}



/**
 *  @brief This function handles the command response of get_hw_spec
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to command buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_ret_get_hw_spec(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN t_void *pioctl_buf)
{
    HostCmd_DS_GET_HW_SPEC *hw_spec = &resp->params.hw_spec;
    mlan_adapter *pmadapter         = pmpriv->adapter;
    mlan_status ret                 = MLAN_STATUS_SUCCESS;
    t_u32 i;
    pmlan_ioctl_req pioctl_req = (mlan_ioctl_req *)pioctl_buf;
    t_u16 left_len;
    t_u16 tlv_type           = 0;
    t_u16 tlv_len            = 0;
    MrvlIEtypesHeader_t *tlv = MNULL;
#ifdef CONFIG_11AX
    MrvlIEtypes_Extension_t *ext_tlv = MNULL;
#endif
    MrvlIEtypes_fw_cap_info_t *fw_cap_tlv = MNULL;
    ENTER();

    pmadapter->fw_cap_info = wlan_le32_to_cpu(hw_spec->fw_cap_info);
    if ((IS_SUPPORT_MULTI_BANDS(pmadapter)) != 0U)
    {
        pmadapter->fw_bands = (t_u16)GET_FW_DEFAULT_BANDS(pmadapter);
#ifndef CONFIG_5GHz_SUPPORT
        /* fixme: Re-check if this is the correct way to disable 5 GHz. */
        pmadapter->fw_bands &= ~(BAND_A | BAND_AN | BAND_AAC);
#endif /* CONFIG_5GHz_SUPPORT */
    }
    else
    {
        pmadapter->fw_bands = BAND_B;
    }

    pmadapter->config_bands = pmadapter->fw_bands;
    for (i = 0; i < pmadapter->priv_num; i++)
    {
        if (pmadapter->priv[i] != MNULL)
        {
            pmadapter->priv[i]->config_bands = pmadapter->fw_bands;
        }
    }

    if ((pmadapter->fw_bands & BAND_A) != 0U)
    {
        if ((pmadapter->fw_bands & BAND_GN) != 0U)
        {
            pmadapter->config_bands |= BAND_AN;
            for (i = 0; i < pmadapter->priv_num; i++)
            {
                if (pmadapter->priv[i] != MNULL)
                {
                    pmadapter->priv[i]->config_bands |= BAND_AN;
                }
            }

            pmadapter->fw_bands |= BAND_AN;
        }
        if ((pmadapter->fw_bands & BAND_AAC) != 0U)
        {
            pmadapter->config_bands |= BAND_AAC;
            for (i = 0; i < pmadapter->priv_num; i++)
            {
                if (pmadapter->priv[i] != MNULL)
                {
                    pmadapter->priv[i]->config_bands |= BAND_AAC;
                }
            }
        }
        if ((pmadapter->fw_bands & BAND_AN) != 0U)
        {
            pmadapter->adhoc_start_band  = (BAND_A | BAND_AN);
            pmadapter->adhoc_11n_enabled = MTRUE;
        }
        else
        {
            pmadapter->adhoc_start_band = BAND_A;
        }
        pmpriv->adhoc_channel = DEFAULT_AD_HOC_CHANNEL_A;
    }
    else if ((pmadapter->fw_bands & BAND_GN) != 0U)
    {
        pmadapter->adhoc_start_band  = (BAND_G | BAND_B | BAND_GN);
        pmpriv->adhoc_channel        = DEFAULT_AD_HOC_CHANNEL;
        pmadapter->adhoc_11n_enabled = MTRUE;
    }
    else if ((pmadapter->fw_bands & BAND_G) != 0U)
    {
        pmadapter->adhoc_start_band = (BAND_G | BAND_B);
        pmpriv->adhoc_channel       = DEFAULT_AD_HOC_CHANNEL;
    }
    else if ((pmadapter->fw_bands & BAND_B) != 0U)
    {
        pmadapter->adhoc_start_band = BAND_B;
        pmpriv->adhoc_channel       = DEFAULT_AD_HOC_CHANNEL;
    }
    else
    {
        /* Do nothing */
    }

    pmadapter->fw_release_number = hw_spec->fw_release_number;
    pmadapter->number_of_antenna = wlan_le16_to_cpu(hw_spec->number_of_antenna);

    PRINTM(MINFO, "GET_HW_SPEC: fw_release_number- 0x%X\n", wlan_le32_to_cpu(pmadapter->fw_release_number));
    PRINTM(MINFO, "GET_HW_SPEC: Permanent addr- %2x:%2x:%2x:%2x:%2x:%2x\n", hw_spec->permanent_addr[0],
           hw_spec->permanent_addr[1], hw_spec->permanent_addr[2], hw_spec->permanent_addr[3],
           hw_spec->permanent_addr[4], hw_spec->permanent_addr[5]);
    PRINTM(MINFO, "GET_HW_SPEC: hw_if_version=0x%X  version=0x%X\n", wlan_le16_to_cpu(hw_spec->hw_if_version),
           wlan_le16_to_cpu(hw_spec->version));

    if (pmpriv->curr_addr[0] == 0xffU)
    {
        (void)__memmove(pmadapter, pmpriv->curr_addr, hw_spec->permanent_addr, MLAN_MAC_ADDR_LENGTH);
    }

    pmadapter->hw_dot_11n_dev_cap     = wlan_le32_to_cpu(hw_spec->dot_11n_dev_cap);
    pmadapter->usr_dot_11n_dev_cap_bg = pmadapter->hw_dot_11n_dev_cap & DEFAULT_11N_CAP_MASK_BG;
    pmadapter->usr_dot_11n_dev_cap_a  = pmadapter->hw_dot_11n_dev_cap & DEFAULT_11N_CAP_MASK_A;
    pmadapter->usr_dev_mcs_support = pmadapter->hw_dev_mcs_support = hw_spec->dev_mcs_support;
    wlan_show_dot11ndevcap(pmadapter, pmadapter->hw_dot_11n_dev_cap);
    wlan_show_devmcssupport(pmadapter, pmadapter->hw_dev_mcs_support);

    pmadapter->hw_dot_11ac_dev_cap     = wlan_le32_to_cpu(hw_spec->Dot11acDevCap);
    pmadapter->hw_dot_11ac_mcs_support = wlan_le32_to_cpu(hw_spec->Dot11acMcsSupport);

    pmadapter->usr_dot_11ac_mcs_support = pmadapter->hw_dot_11ac_mcs_support;

    pmadapter->usr_dot_11ac_dev_cap_bg = pmadapter->hw_dot_11ac_dev_cap & ~DEFALUT_11AC_CAP_BEAMFORMING_RESET_MASK;
#ifdef CONFIG_5GHz_SUPPORT
    pmadapter->usr_dot_11ac_dev_cap_a = pmadapter->hw_dot_11ac_dev_cap & ~DEFALUT_11AC_CAP_BEAMFORMING_RESET_MASK;
#endif
    pmadapter->usr_dot_11ac_bw = BW_FOLLOW_VHTCAP;

    pmadapter->mp_end_port = wlan_le16_to_cpu(hw_spec->mp_end_port);

#ifndef RW610
    for (i = 1; i <= (unsigned)(MAX_PORT - pmadapter->mp_end_port); i++)
    {
        pmadapter->mp_data_port_mask &= ~(1U << (MAX_PORT - i));
    }
#endif


#ifdef OTP_CHANINFO
    if ((pmadapter->otp_region != MNULL) && (pmadapter->otp_region->force_reg != 0U))
    {
#endif

        /* Set the region code to WWSM by default */
        pmadapter->region_code = MRVDRV_DEFAULT_REGION_CODE;
        for (i = 0; i < MRVDRV_MAX_REGION_CODE; i++)
        {
            /* Use the region code to search for the index */
            if (pmadapter->region_code == region_code_index[i])
            {
                break;
            }
        }
        /* If it's unidentified region code, use the default */
        if (i >= MRVDRV_MAX_REGION_CODE)
        {
            pmadapter->region_code = MRVDRV_DEFAULT_REGION_CODE;
            PRINTM(MWARN, "unidentified region code, use the default (0x%02x)\n", MRVDRV_DEFAULT_REGION_CODE);
        }
        /* Synchronize CFP code with region code */
        pmadapter->cfp_code_bg = (t_u8)pmadapter->region_code;
        pmadapter->cfp_code_a  = (t_u8)pmadapter->region_code;
#ifdef OTP_CHANINFO
    }
#endif
    if (wlan_set_regiontable(pmpriv, (t_u8)pmadapter->region_code, pmadapter->fw_bands) != MLAN_STATUS_SUCCESS)
    {
        if (pioctl_req != MNULL)
        {
            pioctl_req->status_code = MLAN_ERROR_CMD_SCAN_FAIL;
        }
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    if (wlan_11d_set_universaltable(pmpriv, pmadapter->fw_bands) != MLAN_STATUS_SUCCESS)
    {
        if (pioctl_req != MNULL)
        {
            pioctl_req->status_code = MLAN_ERROR_CMD_SCAN_FAIL;
        }
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    left_len = resp->size - (t_u16)sizeof(HostCmd_DS_GET_HW_SPEC) - (t_u16)S_DS_GEN;
    tlv      = (MrvlIEtypesHeader_t *)(void *)((t_u8 *)(&resp->params) + sizeof(HostCmd_DS_GET_HW_SPEC));
    while (left_len > sizeof(MrvlIEtypesHeader_t))
    {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len  = wlan_le16_to_cpu(tlv->len);
        switch (tlv_type)
        {
#ifdef CONFIG_11AX
            case TLV_TYPE_EXTENSION_ID:
                ext_tlv = (MrvlIEtypes_Extension_t *)tlv;
                if (ext_tlv->ext_id == HE_CAPABILITY)
                {
                    ext_tlv->type          = tlv_type;
                    ext_tlv->len           = tlv_len;
                    pmadapter->enable_11ax = MTRUE;
                    wlan_update_11ax_cap(pmadapter, (MrvlIEtypes_Extension_t *)ext_tlv);
                }
                break;
#endif
            case TLV_TYPE_FW_CAP_INFO:
                fw_cap_tlv             = (MrvlIEtypes_fw_cap_info_t *)(void *)tlv;
                pmadapter->fw_cap_info = wlan_le32_to_cpu(fw_cap_tlv->fw_cap_info);
                pmadapter->fw_cap_ext  = wlan_le32_to_cpu(fw_cap_tlv->fw_cap_ext);
                PRINTM(MCMND, "fw_cap_info=0x%x fw_cap_ext=0x%x\n", pmadapter->fw_cap_info, pmadapter->fw_cap_ext);
                break;
            default:
                PRINTM(MINFO, "Unexpected TLV hw spec \n");
                break;
        }
        left_len -= (t_u16)(sizeof(MrvlIEtypesHeader_t) + tlv_len);
        tlv = (MrvlIEtypesHeader_t *)(void *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
    }
done:
    LEAVE();
    return ret;
}


/**
 *  @brief This function prepares command of remain_on_channel.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_remain_on_channel(IN pmlan_private pmpriv,
                                       IN HostCmd_DS_COMMAND *cmd,
                                       IN t_u16 cmd_action,
                                       IN t_void *pdata_buf)
{
    HostCmd_DS_REMAIN_ON_CHANNEL *remain_channel = &cmd->params.remain_on_chan;
    mlan_ds_remain_chan *cfg                     = (mlan_ds_remain_chan *)pdata_buf;
    ENTER();
    cmd->size              = wlan_cpu_to_le16((sizeof(HostCmd_DS_REMAIN_ON_CHANNEL)) + S_DS_GEN);
    cmd->command           = wlan_cpu_to_le16(HostCmd_CMD_802_11_REMAIN_ON_CHANNEL);
    remain_channel->action = cmd_action;
    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        if (cfg->remove != 0U)
        {
            remain_channel->action = HostCmd_ACT_GEN_REMOVE;
        }
        else
        {
            remain_channel->status        = 0;
            remain_channel->reserved      = 0;
            remain_channel->bandcfg       = cfg->bandcfg;
            remain_channel->channel       = cfg->channel;
            remain_channel->remain_period = wlan_cpu_to_le32(cfg->remain_period);
        }
    }
    remain_channel->action = wlan_cpu_to_le16(remain_channel->action);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

#ifdef OTP_CHANINFO
/**
 *  @brief This function handles the command response of chan_region_cfg
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to command buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_chan_region_cfg(IN pmlan_private pmpriv,
                                     IN HostCmd_DS_COMMAND *resp,
                                     IN mlan_ioctl_req *pioctl_buf)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    t_u16 action;
    HostCmd_DS_CHAN_REGION_CFG *reg = MNULL;
    t_u8 *tlv_buf                   = MNULL;
    t_u16 tlv_buf_left;
    mlan_ds_misc_cfg *misc_cfg     = MNULL;
    mlan_ds_misc_chnrgpwr_cfg *cfg = MNULL;
    mlan_status ret                = MLAN_STATUS_SUCCESS;

    ENTER();

    reg = (HostCmd_DS_CHAN_REGION_CFG *)(void *)&resp->params;
    if (reg == MNULL)
    {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    action = wlan_le16_to_cpu(reg->action);
    if (action != HostCmd_ACT_GEN_GET)
    {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    tlv_buf      = (t_u8 *)reg + sizeof(*reg);
    tlv_buf_left = (t_u16)(wlan_le16_to_cpu(resp->size) - S_DS_GEN - sizeof(*reg));

    /* Add FW cfp tables and region info */
    wlan_add_fw_cfp_tables(pmpriv, tlv_buf, tlv_buf_left);

    if (pioctl_buf == MNULL)
    {
        goto done;
    }

    if (pioctl_buf->pbuf == MNULL)
    {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    misc_cfg = (mlan_ds_misc_cfg *)(void *)pioctl_buf->pbuf;

    if (misc_cfg->sub_command == MLAN_OID_MISC_GET_REGIONPWR_CFG)
    {
        cfg         = (mlan_ds_misc_chnrgpwr_cfg *)&(misc_cfg->param.rgchnpwr_cfg);
        cfg->length = wlan_le16_to_cpu(resp->size);
        (void)__memcpy(pmpriv->adapter, cfg->chnrgpwr_buf, (t_u8 *)resp, cfg->length);
    }
    else
    {
        (void)__memset(pmpriv->adapter, &misc_cfg->param.custom_reg_domain, 0, sizeof(mlan_ds_custom_reg_domain));
        if (pmadapter->otp_region != MNULL)
        {
            (void)__memcpy(pmpriv->adapter, &misc_cfg->param.custom_reg_domain.region, pmadapter->otp_region,
                           sizeof(otp_region_info_t));
        }
        if (pmadapter->cfp_otp_bg != MNULL)
        {
            misc_cfg->param.custom_reg_domain.num_bg_chan = pmadapter->tx_power_table_bg_rows;
            (void)__memcpy(pmpriv->adapter, (t_u8 *)misc_cfg->param.custom_reg_domain.cfp_tbl,
                           (t_u8 *)pmadapter->cfp_otp_bg,
                           pmadapter->tx_power_table_bg_rows * sizeof(chan_freq_power_t));
        }
#ifdef CONFIG_5GHz_SUPPORT
        if (pmadapter->cfp_otp_a != MNULL)
        {
            misc_cfg->param.custom_reg_domain.num_a_chan = pmadapter->tx_power_table_a_rows;
            (void)__memcpy(pmpriv->adapter,
                           (t_u8 *)misc_cfg->param.custom_reg_domain.cfp_tbl +
                               pmadapter->tx_power_table_bg_rows * sizeof(chan_freq_power_t),
                           (t_u8 *)pmadapter->cfp_otp_a, pmadapter->tx_power_table_a_rows * sizeof(chan_freq_power_t));
        }
#endif
    }
done:
    LEAVE();
    return ret;
}
#endif


