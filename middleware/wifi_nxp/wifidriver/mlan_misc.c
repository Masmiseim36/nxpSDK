/** @file mlan_misc.c
 *
 *  @brief  This file provides Miscellaneous functions for MLAN module
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

/*************************************************************
Change Log:
    05/11/2009: initial version
************************************************************/
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <wm_os.h>
#include <mlan_fw.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>

/**
 *  @brief This function allocates a mlan_buffer.
 *
 *  @param pmadapter Pointer to mlan_adapter
 *  @param data_len   Data length
 *  @param head_room  head_room reserved in mlan_buffer
 *  @param malloc_flag  flag to user moal_malloc
 *  @return           mlan_buffer pointer or MNULL
 */
pmlan_buffer wlan_alloc_mlan_buffer(mlan_adapter *pmadapter, t_u32 data_len, t_u32 head_room, t_u32 malloc_flag)
{
    mlan_status ret    = MLAN_STATUS_SUCCESS;
    pmlan_buffer pmbuf = MNULL;
    t_u32 buf_size     = 0;
    /* t_u8 *tmp_buf = MNULL; */
    pmlan_callbacks pcb = &pmadapter->callbacks;

    ENTER();

    /* make sure that the data length is at least SDIO block size */
    data_len = ALIGN_SZ(data_len, MLAN_SDIO_BLOCK_SIZE);

    /* head_room is not implemented for malloc mlan buffer */
    if (malloc_flag == MTRUE)
    {
        buf_size = sizeof(mlan_buffer) + data_len + DMA_ALIGNMENT;
        ret =
            pcb->moal_malloc(pmadapter->pmoal_handle, buf_size, MLAN_MEM_DEF | MLAN_MEM_DMA, (t_u8 **)(void **)&pmbuf);
        if ((ret != MLAN_STATUS_SUCCESS) || (pmbuf == MNULL))
        {
            pmbuf = MNULL;
            goto exit;
        }
        (void)__memset(pmadapter, pmbuf, 0, sizeof(mlan_buffer));

        pmbuf->pdesc = MNULL;
        /* Align address */
        pmbuf->pbuf        = (t_u8 *)ALIGN_ADDR((t_u8 *)pmbuf + sizeof(mlan_buffer), DMA_ALIGNMENT);
        pmbuf->data_offset = 0;
        pmbuf->data_len    = data_len;
        pmbuf->flags |= MLAN_BUF_FLAG_MALLOC_BUF;
    }
exit:
    LEAVE();
    return pmbuf;
}

/**
 *  @brief This function frees a mlan_buffer.
 *
 *  @param pmadapter  Pointer to mlan_adapter
 *  @param pmbuf      Pointer to mlan_buffer
 *
 *  @return           N/A
 */
t_void wlan_free_mlan_buffer(mlan_adapter *pmadapter, pmlan_buffer pmbuf)
{
    return;
}



/**
 *   @brief This function processes the 802.11 mgmt Frame
 *
 *   @param priv      A pointer to mlan_private
 *   @param payload   A pointer to the received buffer
 *   @param payload_len Length of the received buffer
 *
 *   @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_process_802dot11_mgmt_pkt(IN mlan_private *priv, IN t_u8 *payload, IN t_u32 payload_len, RxPD *rxpd)
{
    mlan_status ret                   = MLAN_STATUS_SUCCESS;
    wlan_802_11_header *pieee_pkt_hdr = MNULL;
    t_u16 sub_type                    = 0;
    ENTER();
    /* Check packet type-subtype and compare with mgmt_passthru_mask If event
       is needed to host, just eventify it */
    pieee_pkt_hdr = (wlan_802_11_header *)payload;
    sub_type      = IEEE80211_GET_FC_MGMT_FRAME_SUBTYPE(pieee_pkt_hdr->frm_ctl);
    if ((((1 << sub_type) & priv->mgmt_frame_passthru_mask) == 0) && (sub_type != SUBTYPE_ACTION))
    {
        PRINTM(MINFO, "Dropping mgmt frame for subtype %d.\n", sub_type);
        LEAVE();
        return ret;
    }
    switch (sub_type)
    {
        case SUBTYPE_ACTION:
            ret = wlan_process_mgmt_action(payload, payload_len, rxpd);
            if (ret == MLAN_STATUS_SUCCESS)
            {
                return ret;
            }
            break;
        case SUBTYPE_ASSOC_REQUEST:
        case SUBTYPE_REASSOC_REQUEST:
        case SUBTYPE_DISASSOC:
        case SUBTYPE_DEAUTH:
        case SUBTYPE_AUTH:
        case SUBTYPE_PROBE_RESP:
            break;
        default:
            PRINTM(MINFO, "Unexpected pkt subtype \n");
            break;
    }
    LEAVE();
    return ret;
}


/**
 *  @brief Add Extended Capabilities IE
 *
 *  @param pmpriv             A pointer to mlan_private structure
 *  @param pptlv_out          A pointer to TLV to fill in
 *  @param BSSDescriptor      A poiter to bss descriptor
 *  @return                   N/A
 */
void wlan_add_ext_capa_info_ie(IN mlan_private *pmpriv, IN BSSDescriptor_t *pbss_desc, OUT t_u8 **pptlv_out)
{
    MrvlIETypes_ExtCap_t *pext_cap = MNULL;

    ENTER();

    pext_cap = (MrvlIETypes_ExtCap_t *)(void *)*pptlv_out;
    (void)__memset(pmpriv->adapter, pext_cap, 0, sizeof(MrvlIETypes_ExtCap_t));
    pext_cap->header.type = wlan_cpu_to_le16(EXT_CAPABILITY);
    pext_cap->header.len  = wlan_cpu_to_le16(sizeof(ExtCap_t));
    if ((((t_u8)(pmpriv->hotspot_cfg >> 8)) & HOTSPOT_ENABLE_INTERWORKING_IND) != 0U)
    {
        pext_cap->ext_cap.Interworking = 1;
    }
    if ((((t_u8)(pmpriv->hotspot_cfg >> 8)) & HOTSPOT_ENABLE_TDLS_IND) != 0U)
    {
        pext_cap->ext_cap.TDLSSupport = 1;
    }

#ifdef CONFIG_11V
    if (pbss_desc->pext_cap->ext_cap.BSS_Transition == true)
    {
        pext_cap->ext_cap.BSS_Transition = 1;
    }
    else
    {
        pext_cap->ext_cap.BSS_Transition = 0;
    }
#endif
    *pptlv_out += sizeof(MrvlIETypes_ExtCap_t);

    LEAVE();
}


/**
 *  @brief Get rate index
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_rate_ioctl_get_rate_index(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];

    ENTER();

    /* Send request to firmware */
    if (is_sta_connected())
    {
        ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_TX_RATE_CFG, HostCmd_ACT_GEN_GET, 0, (t_void *)pioctl_req, MNULL);
    }
    else
    {
        ret = (mlan_status)wifi_uap_prepare_and_send_cmd(pmpriv, HostCmd_CMD_TX_RATE_CFG, HostCmd_ACT_GEN_GET, 0,
                                                         (t_void *)pioctl_req, NULL, MLAN_BSS_TYPE_UAP, NULL);
    }
    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Set rate index
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_rate_ioctl_set_rate_index(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    t_s32 rate_index;
    mlan_rate_format rate_format;
#if defined(CONFIG_11AC) || defined(CONFIG_11AX)
    t_u32 nss;
#endif
    t_u32 i;
    mlan_ds_rate *ds_rate = MNULL;
    mlan_status ret       = MLAN_STATUS_FAILURE;
    mlan_private *pmpriv  = pmadapter->priv[pioctl_req->bss_index];
    t_u16 bitmap_rates[MAX_BITMAP_RATES_SIZE];
    t_u16 shift_index = 1U;

    ENTER();

    ds_rate = (mlan_ds_rate *)(void *)pioctl_req->pbuf;

    rate_format = ds_rate->param.rate_cfg.rate_format;
#if defined(CONFIG_11AC) || defined(CONFIG_11AX)
    nss = ds_rate->param.rate_cfg.nss;
#endif
    rate_index = (t_s32)ds_rate->param.rate_cfg.rate;

    if (ds_rate->param.rate_cfg.is_rate_auto == MTRUE)
    {
        (void)__memset(pmadapter, bitmap_rates, 0, sizeof(bitmap_rates));
        /* Rates talbe [0]: HR/DSSS;[1]: OFDM; [2..9] HT; */
        /* Support all HR/DSSS rates */
        bitmap_rates[0] = 0x000F;
        /* Support all OFDM rates */
        bitmap_rates[1] = 0x00FF;
        /* Support all HT-MCSs rate */
        for (i = 2; i < 9U; i++)
        {
            bitmap_rates[i] = 0xFFFF;
        }
        bitmap_rates[9] = 0x3FFF;
#ifdef CONFIG_11AC
        /* [10..17] VHT */
        /* Support all VHT-MCSs rate for NSS 1 and 2 */
        for (i = 10; i < 12; i++)
        {
            bitmap_rates[i] = 0x03FF; /* 10 Bits valid */
        }
        /* Set to 0 as default value for all other NSSs */
        for (i = 12; i < NELEMENTS(bitmap_rates); i++)
        {
            bitmap_rates[i] = 0x0;
        }
#endif
    }
    else
    {
        PRINTM(MINFO, "Rate index is %d\n", rate_index);


        (void)__memset(pmadapter, bitmap_rates, 0, sizeof(bitmap_rates));
        if (rate_format == MLAN_RATE_FORMAT_LG)
        {
            /* Bitmap of HR/DSSS rates */
            if (rate_index <= MLAN_RATE_INDEX_HRDSSS3)
            {
                bitmap_rates[0] = (shift_index << rate_index);
                ret             = MLAN_STATUS_SUCCESS;
                /* Bitmap of OFDM rates */
            }
            else if (rate_index <= MLAN_RATE_INDEX_OFDM7)
            {
                bitmap_rates[1] = (shift_index << (rate_index - MLAN_RATE_INDEX_OFDM0));
                ret             = MLAN_STATUS_SUCCESS;
            }
            else
            {
                /*Do Nothing*/
            }
        }
        else if (rate_format == MLAN_RATE_FORMAT_HT)
        {
            if (rate_index <= MLAN_RATE_INDEX_MCS32)
            {
#ifdef SD8801
                rate_index -= MLAN_RATE_INDEX_MCS0;
#endif
                bitmap_rates[2 + (rate_index / 16)] = (shift_index << (rate_index % 16));
                ret                                 = MLAN_STATUS_SUCCESS;
            }
        }
        else
        {
            /*DO Nothing*/
        }

#ifdef CONFIG_11AC
        if (rate_format == MLAN_RATE_FORMAT_VHT)
        {
            if ((rate_index <= MLAN_RATE_INDEX_MCS9) && (MLAN_RATE_NSS1 <= nss) && (nss <= MLAN_RATE_NSS2))
            {
                bitmap_rates[10 + nss - MLAN_RATE_NSS1] = (shift_index << rate_index);
                ret                                     = MLAN_STATUS_SUCCESS;
            }
        }
#endif
        if (ret == MLAN_STATUS_FAILURE)
        {
            PRINTM(MERROR, "Invalid MCS index=%d. \n", rate_index);
            pioctl_req->status_code = MLAN_ERROR_INVALID_PARAMETER;
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
    }

    PRINTM(MINFO,
           "RateBitmap=%04x%04x%04x%04x%04x%04x%04x%04x"
           "%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x, "
           "IsRateAuto=%d, DataRate=%d\n",
           bitmap_rates[17], bitmap_rates[16], bitmap_rates[15], bitmap_rates[14], bitmap_rates[13], bitmap_rates[12],
           bitmap_rates[11], bitmap_rates[10], bitmap_rates[9], bitmap_rates[8], bitmap_rates[7], bitmap_rates[6],
           bitmap_rates[5], bitmap_rates[4], bitmap_rates[3], bitmap_rates[2], bitmap_rates[1], bitmap_rates[0],
           pmpriv->is_data_rate_auto, pmpriv->data_rate);

    /* Send request to firmware */
    if (is_sta_connected())
    {
        ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_TX_RATE_CFG, HostCmd_ACT_GEN_SET, 0, (t_void *)pioctl_req,
                               (t_void *)bitmap_rates);
    }
    else
    {
        ret = (mlan_status)wifi_uap_prepare_and_send_cmd(pmpriv, HostCmd_CMD_TX_RATE_CFG, HostCmd_ACT_GEN_SET, 0,
                                                         (t_void *)pioctl_req, (t_void *)bitmap_rates,
                                                         MLAN_BSS_TYPE_UAP, NULL);
    }
    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Rate configuration command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING --success, otherwise fail
 */
mlan_status wlan_rate_ioctl_cfg(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

        if (pioctl_req->action == MLAN_ACT_GET)
        {
            status = wlan_rate_ioctl_get_rate_index(pmadapter, pioctl_req);
        }
        else
        {
            status = wlan_rate_ioctl_set_rate_index(pmadapter, pioctl_req);
        }

    LEAVE();
    return status;
}

/**
 *  @brief This function prepares command of rf_antenna.
 *
 *  @param pmpriv   A pointer to mlan_private structure
 *  @param cmd      A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return         MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_802_11_rf_antenna(IN pmlan_private pmpriv,
                                       IN HostCmd_DS_COMMAND *cmd,
                                       IN t_u16 cmd_action,
                                       IN t_void *pdata_buf)
{
    HostCmd_DS_802_11_RF_ANTENNA *pantenna = &cmd->params.antenna;
    mlan_ds_ant_cfg_1x1 *ant_cfg_1x1 = (mlan_ds_ant_cfg_1x1 *)pdata_buf;

    ENTER();
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_RF_ANTENNA);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_RF_ANTENNA) + S_DS_GEN);

    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        pantenna->action        = wlan_cpu_to_le16(HostCmd_ACT_SET_BOTH);
        pantenna->antenna_mode  = wlan_cpu_to_le16((t_u16)ant_cfg_1x1->antenna);
        pantenna->evaluate_time = wlan_cpu_to_le16((t_u16)ant_cfg_1x1->evaluate_time);
    }
    else
    {
        pantenna->action        = wlan_cpu_to_le16(HostCmd_ACT_GET_BOTH);
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}


#ifdef WLAN_LOW_POWER_ENABLE
/**
 *  @brief Set/Get Low Power Mode
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   A pointer to ioctl request buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_misc_ioctl_low_pwr_mode(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret        = MLAN_STATUS_SUCCESS;
    mlan_ds_misc_cfg *misc = MNULL;
    mlan_private *pmpriv   = pmadapter->priv[pioctl_req->bss_index];

    ENTER();

    misc = (mlan_ds_misc_cfg *)pioctl_req->pbuf;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_LOW_POWER_MODE, HostCmd_ACT_GEN_SET, 0, (t_void *)pioctl_req,
                           &misc->param.low_pwr_mode);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

    LEAVE();
    return ret;
}
#endif // WLAN_LOW_POWER_ENABLE

#ifdef CONFIG_WIFI_CLOCKSYNC
/**
 *  @brief Set/Get GPIO TSF Latch config
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status wlan_misc_gpio_tsf_latch_config(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_status ret            = MLAN_STATUS_SUCCESS;
    mlan_ds_misc_cfg *misc_cfg = MNULL;
    t_u16 cmd_action           = 0;
    mlan_private *pmpriv       = pmadapter->priv[pioctl_req->bss_index];

    ENTER();

    misc_cfg = (mlan_ds_misc_cfg *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
        cmd_action = HostCmd_ACT_GEN_SET;
    else
        cmd_action = HostCmd_ACT_GEN_GET;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_GPIO_TSF_LATCH_PARAM_CONFIG, cmd_action, 0, (t_void *)pioctl_req,
                           &misc_cfg->param.gpio_tsf_latch_config);

    LEAVE();
    return ret;
}

/**
 *  @brief Get TSF info
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status wlan_misc_get_tsf_info(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_status ret            = MLAN_STATUS_SUCCESS;
    mlan_ds_misc_cfg *misc_cfg = MNULL;
    t_u16 cmd_action           = 0;
    mlan_private *pmpriv       = pmadapter->priv[pioctl_req->bss_index];

    ENTER();

    misc_cfg   = (mlan_ds_misc_cfg *)pioctl_req->pbuf;
    cmd_action = HostCmd_ACT_GEN_GET;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_GPIO_TSF_LATCH_PARAM_CONFIG, cmd_action, 0, (t_void *)pioctl_req,
                           &misc_cfg->param.tsf_info);

    LEAVE();
    return ret;
}
#endif /* CONFIG_WIFI_CLOCKSYNC */

