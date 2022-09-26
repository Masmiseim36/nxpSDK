/** @file mlan_uap_cmdevent.c
 *
 *  @brief  This file provides the handling of AP mode command and event
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

/********************************************************
Change log:
    02/05/2009: initial version
********************************************************/

#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <wm_os.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>


/**
 *  @brief This function prepares command for config uap settings
 *
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_uap_cmd_ap_config(pmlan_private pmpriv,
                                          IN HostCmd_DS_COMMAND *cmd,
                                          IN t_u16 cmd_action,
                                          IN pmlan_ioctl_req pioctl_buf)
{
    mlan_ds_bss *bss                               = MNULL;
    HostCmd_DS_SYS_CONFIG *sys_config              = (HostCmd_DS_SYS_CONFIG *)&cmd->params.sys_config;
    t_u8 *tlv                                      = MNULL;
    MrvlIEtypes_MacAddr_t *tlv_mac                 = MNULL;
    MrvlIEtypes_SsIdParamSet_t *tlv_ssid           = MNULL;
    MrvlIEtypes_beacon_period_t *tlv_beacon_period = MNULL;
    MrvlIEtypes_ecsa_config_t *tlv_ecsa_config     = MNULL;
    MrvlIEtypes_dtim_period_t *tlv_dtim_period     = MNULL;
    MrvlIEtypes_RatesParamSet_t *tlv_rates         = MNULL;
    MrvlIEtypes_bcast_ssid_t *tlv_bcast_ssid = MNULL;
    MrvlIEtypes_channel_band_t *tlv_chan_band            = MNULL;
    MrvlIEtypes_ChanListParamSet_t *tlv_chan_list        = MNULL;
    ChanScanParamSet_t *pscan_chan                       = MNULL;
    MrvlIEtypes_encrypt_protocol_t *tlv_encrypt_protocol = MNULL;
    MrvlIEtypes_akmp_t *tlv_akmp                         = MNULL;
    MrvlIEtypes_pwk_cipher_t *tlv_pwk_cipher             = MNULL;
    MrvlIEtypes_gwk_cipher_t *tlv_gwk_cipher             = MNULL;
    MrvlIEtypes_passphrase_t *tlv_passphrase = MNULL;
    MrvlIEtypes_password_t *tlv_password     = MNULL;
    MrvlIETypes_HTCap_t *tlv_htcap = MNULL;
    t_u32 cmd_size  = 0;
    t_u8 zero_mac[] = {0, 0, 0, 0, 0, 0};
    t_u16 i;
    /* t_u16 ac; */

    ENTER();
    if (pioctl_buf == MNULL)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    bss = (mlan_ds_bss *)(void *)pioctl_buf->pbuf;

    cmd->command       = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    sys_config->action = wlan_cpu_to_le16(cmd_action);
    cmd_size           = sizeof(HostCmd_DS_SYS_CONFIG) - 1U + S_DS_GEN;

    tlv = (t_u8 *)sys_config->tlv_buffer;
    if (__memcmp(pmpriv->adapter, zero_mac, &bss->param.bss_config.mac_addr, MLAN_MAC_ADDR_LENGTH) != 0)
    {
        tlv_mac              = (MrvlIEtypes_MacAddr_t *)(void *)tlv;
        tlv_mac->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_MAC_ADDRESS);
        tlv_mac->header.len  = wlan_cpu_to_le16(MLAN_MAC_ADDR_LENGTH);
        (void)__memcpy(pmpriv->adapter, tlv_mac->mac, &bss->param.bss_config.mac_addr, MLAN_MAC_ADDR_LENGTH);
        cmd_size += sizeof(MrvlIEtypes_MacAddr_t);
        tlv += sizeof(MrvlIEtypes_MacAddr_t);
    }

    if (bss->param.bss_config.ssid.ssid_len != 0U)
    {
        tlv_ssid              = (MrvlIEtypes_SsIdParamSet_t *)(void *)tlv;
        tlv_ssid->header.type = wlan_cpu_to_le16(TLV_TYPE_SSID);
        tlv_ssid->header.len  = wlan_cpu_to_le16((t_u16)bss->param.bss_config.ssid.ssid_len);
        (void)__memcpy(pmpriv->adapter, tlv_ssid->ssid, bss->param.bss_config.ssid.ssid,
                       bss->param.bss_config.ssid.ssid_len);
        cmd_size += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.ssid.ssid_len;
        tlv += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.ssid.ssid_len;
    }

    if ((bss->param.bss_config.beacon_period >= MIN_BEACON_PERIOD) &&
        (bss->param.bss_config.beacon_period <= MAX_BEACON_PERIOD))
    {
        tlv_beacon_period                = (MrvlIEtypes_beacon_period_t *)(void *)tlv;
        tlv_beacon_period->header.type   = wlan_cpu_to_le16(TLV_TYPE_UAP_BEACON_PERIOD);
        tlv_beacon_period->header.len    = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_beacon_period->beacon_period = wlan_cpu_to_le16(bss->param.bss_config.beacon_period);
        cmd_size += sizeof(MrvlIEtypes_beacon_period_t);
        tlv += sizeof(MrvlIEtypes_beacon_period_t);
    }

    if ((bss->param.bss_config.chan_sw_count >= MIN_CHSW_COUNT) &&
        (bss->param.bss_config.chan_sw_count <= MAX_CHSW_COUNT))
    {
        tlv_ecsa_config                     = (MrvlIEtypes_ecsa_config_t *)(void *)tlv;
        tlv_ecsa_config->header.type        = wlan_cpu_to_le16(TLV_TYPE_UAP_ECSA_CONFIG);
        tlv_ecsa_config->header.len         = wlan_cpu_to_le16(sizeof(t_u16) + sizeof(t_u8) + sizeof(t_u8));
        tlv_ecsa_config->enable             = 1;
        tlv_ecsa_config->ChannelSwitchMode  = 0;
        tlv_ecsa_config->ChannelSwitchCount = bss->param.bss_config.chan_sw_count;
        cmd_size += sizeof(MrvlIEtypes_ecsa_config_t);
        tlv += sizeof(MrvlIEtypes_ecsa_config_t);
    }

    if ((bss->param.bss_config.dtim_period >= MIN_DTIM_PERIOD) &&
        (bss->param.bss_config.dtim_period <= MAX_DTIM_PERIOD))
    {
        tlv_dtim_period              = (MrvlIEtypes_dtim_period_t *)(void *)tlv;
        tlv_dtim_period->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_DTIM_PERIOD);
        tlv_dtim_period->header.len  = wlan_cpu_to_le16(sizeof(t_u8));
        tlv_dtim_period->dtim_period = bss->param.bss_config.dtim_period;
        cmd_size += sizeof(MrvlIEtypes_dtim_period_t);
        tlv += sizeof(MrvlIEtypes_dtim_period_t);
    }

    if (bss->param.bss_config.rates[0] != 0U)
    {
        tlv_rates              = (MrvlIEtypes_RatesParamSet_t *)(void *)tlv;
        tlv_rates->header.type = wlan_cpu_to_le16(TLV_TYPE_RATES);
        for (i = 0; i < MAX_DATA_RATES && bss->param.bss_config.rates[i]; i++)
        {
            tlv_rates->rates[i] = bss->param.bss_config.rates[i];
        }
        tlv_rates->header.len = wlan_cpu_to_le16(i);
        cmd_size += sizeof(MrvlIEtypesHeader_t) + i;
        tlv += sizeof(MrvlIEtypesHeader_t) + i;
    }


    if (bss->param.bss_config.bcast_ssid_ctl <= MTRUE)
    {
        tlv_bcast_ssid                 = (MrvlIEtypes_bcast_ssid_t *)(void *)tlv;
        tlv_bcast_ssid->header.type    = wlan_cpu_to_le16(TLV_TYPE_UAP_BCAST_SSID_CTL);
        tlv_bcast_ssid->header.len     = wlan_cpu_to_le16(sizeof(t_u8));
        tlv_bcast_ssid->bcast_ssid_ctl = bss->param.bss_config.bcast_ssid_ctl;
        cmd_size += sizeof(MrvlIEtypes_bcast_ssid_t);
        tlv += sizeof(MrvlIEtypes_bcast_ssid_t);
    }


    if ((((bss->param.bss_config.band_cfg & BAND_CONFIG_ACS_MODE) == BAND_CONFIG_MANUAL) &&
         (bss->param.bss_config.channel > 0U) && (bss->param.bss_config.channel <= MLAN_MAX_CHANNEL)) ||
        (bss->param.bss_config.band_cfg & BAND_CONFIG_ACS_MODE))
    {
        tlv_chan_band              = (MrvlIEtypes_channel_band_t *)(void *)tlv;
        tlv_chan_band->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_CHAN_BAND_CONFIG);
        tlv_chan_band->header.len  = wlan_cpu_to_le16(sizeof(t_u8) + sizeof(t_u8));
        tlv_chan_band->band_config = bss->param.bss_config.band_cfg;
        tlv_chan_band->channel     = bss->param.bss_config.channel;
        cmd_size += sizeof(MrvlIEtypes_channel_band_t);
        tlv += sizeof(MrvlIEtypes_channel_band_t);
    }

    if ((bss->param.bss_config.num_of_chan) && (bss->param.bss_config.num_of_chan <= MLAN_MAX_CHANNEL))
    {
        tlv_chan_list              = (MrvlIEtypes_ChanListParamSet_t *)(void *)tlv;
        tlv_chan_list->header.type = wlan_cpu_to_le16(TLV_TYPE_CHANLIST);
        tlv_chan_list->header.len =
            wlan_cpu_to_le16((t_u16)(sizeof(ChanScanParamSet_t) * bss->param.bss_config.num_of_chan));
        pscan_chan = tlv_chan_list->chan_scan_param;
        for (i = 0; i < bss->param.bss_config.num_of_chan; i++)
        {
            (void)memset(pscan_chan, 0x00, sizeof(ChanScanParamSet_t));
            pscan_chan->chan_number = bss->param.bss_config.chan_list[i].chan_number;
            pscan_chan->radio_type  = bss->param.bss_config.chan_list[i].band_config_type;
            pscan_chan++;
        }
        cmd_size += sizeof(tlv_chan_list->header) + (sizeof(ChanScanParamSet_t) * bss->param.bss_config.num_of_chan);
        tlv += sizeof(tlv_chan_list->header) + (sizeof(ChanScanParamSet_t) * bss->param.bss_config.num_of_chan);
    }


    if (bss->param.bss_config.protocol != 0U)
    {
        tlv_encrypt_protocol              = (MrvlIEtypes_encrypt_protocol_t *)(void *)tlv;
        tlv_encrypt_protocol->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_ENCRYPT_PROTOCOL);
        tlv_encrypt_protocol->header.len  = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_encrypt_protocol->protocol    = wlan_cpu_to_le16(bss->param.bss_config.protocol);
        cmd_size += sizeof(MrvlIEtypes_encrypt_protocol_t);
        tlv += sizeof(MrvlIEtypes_encrypt_protocol_t);
    }

    if ((bss->param.bss_config.protocol & PROTOCOL_WPA) || (bss->param.bss_config.protocol & PROTOCOL_WPA2) ||
        (bss->param.bss_config.protocol & PROTOCOL_WPA3_SAE) ||
        (bss->param.bss_config.protocol & PROTOCOL_EAP))
    {
        tlv_akmp                     = (MrvlIEtypes_akmp_t *)(void *)tlv;
        tlv_akmp->header.type        = wlan_cpu_to_le16(TLV_TYPE_UAP_AKMP);
        tlv_akmp->key_mgmt           = wlan_cpu_to_le16(bss->param.bss_config.key_mgmt);
        tlv_akmp->header.len         = (t_u16)sizeof(t_u16);
        tlv_akmp->key_mgmt_operation = wlan_cpu_to_le16(bss->param.bss_config.key_mgmt_operation);
        tlv_akmp->header.len += (t_u16)sizeof(t_u16);
        tlv_akmp->header.len = wlan_cpu_to_le16(tlv_akmp->header.len);
        cmd_size += sizeof(MrvlIEtypes_akmp_t);
        tlv += sizeof(MrvlIEtypes_akmp_t);

        if ((bss->param.bss_config.wpa_cfg.pairwise_cipher_wpa & VALID_CIPHER_BITMAP) != 0U)
        {
            tlv_pwk_cipher                  = (MrvlIEtypes_pwk_cipher_t *)(void *)tlv;
            tlv_pwk_cipher->header.type     = wlan_cpu_to_le16(TLV_TYPE_PWK_CIPHER);
            tlv_pwk_cipher->header.len      = wlan_cpu_to_le16(sizeof(t_u16) + sizeof(t_u8) + sizeof(t_u8));
            tlv_pwk_cipher->protocol        = wlan_cpu_to_le16(PROTOCOL_WPA);
            tlv_pwk_cipher->pairwise_cipher = bss->param.bss_config.wpa_cfg.pairwise_cipher_wpa;
            cmd_size += sizeof(MrvlIEtypes_pwk_cipher_t);
            tlv += sizeof(MrvlIEtypes_pwk_cipher_t);
        }

        if ((bss->param.bss_config.wpa_cfg.pairwise_cipher_wpa2 & VALID_CIPHER_BITMAP) != 0U)
        {
            tlv_pwk_cipher              = (MrvlIEtypes_pwk_cipher_t *)(void *)tlv;
            tlv_pwk_cipher->header.type = wlan_cpu_to_le16(TLV_TYPE_PWK_CIPHER);
            tlv_pwk_cipher->header.len  = wlan_cpu_to_le16(sizeof(t_u16) + sizeof(t_u8) + sizeof(t_u8));
            if ((bss->param.bss_config.protocol & PROTOCOL_WPA3_SAE) != 0U)
            {
                tlv_pwk_cipher->protocol = wlan_cpu_to_le16(PROTOCOL_WPA3_SAE);
            }
            else
            {
                tlv_pwk_cipher->protocol = wlan_cpu_to_le16(PROTOCOL_WPA2);
            }
            tlv_pwk_cipher->pairwise_cipher = bss->param.bss_config.wpa_cfg.pairwise_cipher_wpa2;
            cmd_size += sizeof(MrvlIEtypes_pwk_cipher_t);
            tlv += sizeof(MrvlIEtypes_pwk_cipher_t);
        }

        if ((bss->param.bss_config.wpa_cfg.group_cipher & VALID_CIPHER_BITMAP) != 0U)
        {
            tlv_gwk_cipher               = (MrvlIEtypes_gwk_cipher_t *)(void *)tlv;
            tlv_gwk_cipher->header.type  = wlan_cpu_to_le16(TLV_TYPE_GWK_CIPHER);
            tlv_gwk_cipher->header.len   = wlan_cpu_to_le16(sizeof(t_u8) + sizeof(t_u8));
            tlv_gwk_cipher->group_cipher = bss->param.bss_config.wpa_cfg.group_cipher;
            cmd_size += sizeof(MrvlIEtypes_gwk_cipher_t);
            tlv += sizeof(MrvlIEtypes_gwk_cipher_t);
        }


        if (bss->param.bss_config.wpa_cfg.length != 0U)
        {
            tlv_passphrase              = (MrvlIEtypes_passphrase_t *)(void *)tlv;
            tlv_passphrase->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_WPA_PASSPHRASE);
            tlv_passphrase->header.len  = (t_u16)wlan_cpu_to_le16(bss->param.bss_config.wpa_cfg.length);
            (void)__memcpy(pmpriv->adapter, tlv_passphrase->passphrase, bss->param.bss_config.wpa_cfg.passphrase,
                           bss->param.bss_config.wpa_cfg.length);
            cmd_size += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.wpa_cfg.length;
            tlv += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.wpa_cfg.length;
        }

        if (bss->param.bss_config.wpa_cfg.password_length != 0U)
        {
            tlv_password              = (MrvlIEtypes_password_t *)(void *)tlv;
            tlv_password->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_WPA3_SAE_PASSWORD);
            tlv_password->header.len  = (t_u16)wlan_cpu_to_le16(bss->param.bss_config.wpa_cfg.password_length);
            (void)__memcpy(pmpriv->adapter, tlv_password->password, bss->param.bss_config.wpa_cfg.password,
                           bss->param.bss_config.wpa_cfg.password_length);
            cmd_size += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.wpa_cfg.password_length;
            tlv += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.wpa_cfg.password_length;
        }

    }
    else
    {
    }

    if ((bss->param.bss_config.ht_cap_info) != 0U)
    {
        /* wmsdk: All the values received will be zero by default. */
        tlv_htcap                     = (MrvlIETypes_HTCap_t *)(void *)tlv;
        tlv_htcap->header.type        = wlan_cpu_to_le16(HT_CAPABILITY);
        tlv_htcap->header.len         = wlan_cpu_to_le16(sizeof(HTCap_t));
        tlv_htcap->ht_cap.ht_cap_info = wlan_cpu_to_le16(bss->param.bss_config.ht_cap_info);
        tlv_htcap->ht_cap.ampdu_param = bss->param.bss_config.ampdu_param;
        (void)__memcpy(pmpriv->adapter, tlv_htcap->ht_cap.supported_mcs_set, bss->param.bss_config.supported_mcs_set,
                       16);
        tlv_htcap->ht_cap.ht_ext_cap = wlan_cpu_to_le16(bss->param.bss_config.ht_ext_cap);
        tlv_htcap->ht_cap.tx_bf_cap  = wlan_cpu_to_le32(bss->param.bss_config.tx_bf_cap);
        tlv_htcap->ht_cap.asel       = bss->param.bss_config.asel;
        cmd_size += sizeof(MrvlIETypes_HTCap_t);
        tlv += sizeof(MrvlIETypes_HTCap_t);
    }


    cmd->size = (t_u16)wlan_cpu_to_le16(cmd_size);
    PRINTM(MCMND, "AP config: cmd_size=%d\n", cmd_size);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of sys_config
 *
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_uap_cmd_sys_configure(pmlan_private pmpriv,
                                              IN HostCmd_DS_COMMAND *cmd,
                                              IN t_u16 cmd_action,
                                              IN pmlan_ioctl_req pioctl_buf,
                                              IN t_void *pdata_buf)
{
    mlan_ds_bss *bss                          = MNULL;
    HostCmd_DS_SYS_CONFIG *sys_config         = (HostCmd_DS_SYS_CONFIG *)&cmd->params.sys_config;
    MrvlIEtypes_channel_band_t *chan_band_tlv = MNULL, *pdat_tlv_cb = MNULL;
    MrvlIEtypes_max_sta_count_t *max_sta_cnt_tlv = MNULL, *pdat_tlv_ccb = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    cmd->command       = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    sys_config->action = wlan_cpu_to_le16(cmd_action);
    cmd->size          = wlan_cpu_to_le16(sizeof(HostCmd_DS_SYS_CONFIG) - 1U + S_DS_GEN);
    if (pioctl_buf == MNULL)
    {
        if (pdata_buf != NULL)
        {
            switch (*(t_u16 *)pdata_buf)
            {
                case TLV_TYPE_UAP_CHAN_BAND_CONFIG:
                    pdat_tlv_cb                = (MrvlIEtypes_channel_band_t *)pdata_buf;
                    chan_band_tlv              = (MrvlIEtypes_channel_band_t *)(void *)sys_config->tlv_buffer;
                    cmd->size                  = wlan_cpu_to_le16(sizeof(HostCmd_DS_SYS_CONFIG) - 1U + S_DS_GEN +
                                                 sizeof(MrvlIEtypes_channel_band_t));
                    chan_band_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_CHAN_BAND_CONFIG);
                    chan_band_tlv->header.len =
                        wlan_cpu_to_le16(sizeof(MrvlIEtypes_channel_band_t) - sizeof(MrvlIEtypesHeader_t));
                    if (cmd_action != 0U)
                    {
                        chan_band_tlv->band_config = pdat_tlv_cb->band_config;
                        chan_band_tlv->channel     = pdat_tlv_cb->channel;
                    }
                    ret = MLAN_STATUS_SUCCESS;
                    break;
                case TLV_TYPE_UAP_MAX_STA_CNT:
                    pdat_tlv_ccb                 = (MrvlIEtypes_max_sta_count_t *)pdata_buf;
                    max_sta_cnt_tlv              = (MrvlIEtypes_max_sta_count_t *)(void *)sys_config->tlv_buffer;
                    cmd->size                    = wlan_cpu_to_le16(sizeof(HostCmd_DS_SYS_CONFIG) - 1U + S_DS_GEN +
                                                 sizeof(MrvlIEtypes_max_sta_count_t));
                    max_sta_cnt_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_MAX_STA_CNT);

                    if (cmd_action != 0U)
                    {
                        max_sta_cnt_tlv->header.len =
                            wlan_cpu_to_le16(sizeof(MrvlIEtypes_max_sta_count_t) - sizeof(MrvlIEtypesHeader_t));
                        max_sta_cnt_tlv->max_sta_count = pdat_tlv_ccb->max_sta_count;
                    }
                    else
                    {
                        max_sta_cnt_tlv->header.len    = 0;
                        max_sta_cnt_tlv->max_sta_count = 0;
                    }
                    ret = MLAN_STATUS_SUCCESS;
                    break;
                default:
                    PRINTM(MERROR, "Wrong data, or missing TLV_TYPE 0x%04x handler.\n", *(t_u16 *)pdata_buf);
                    break;
            }
            goto done;
        }
        else
        {
        }
    }

    if (pioctl_buf->req_id == (t_u32)MLAN_IOCTL_BSS)
    {
        bss = (mlan_ds_bss *)(void *)pioctl_buf->pbuf;
        if (bss->sub_command == MLAN_OID_BSS_MAC_ADDR)
        {
        }
        else if ((bss->sub_command == MLAN_OID_UAP_BSS_CONFIG) && (cmd_action == HostCmd_ACT_GEN_SET))
        {
            ret = wlan_uap_cmd_ap_config(pmpriv, cmd, cmd_action, pioctl_buf);
            goto done;
        }
        else
        { /* Do Nothing */
        }
    }
    else if (pioctl_buf->req_id == (t_u32)MLAN_IOCTL_MISC_CFG)
    {
    }
    else
    {
        goto done;
    }
done:
    LEAVE();
    return ret;
}


/**
 *  @brief This function prepares command of snmp_mib
 *
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param cmd_oid      Cmd oid: treated as sub command
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *  @param pdata_buf    A pointer to information buffer
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_uap_cmd_snmp_mib(pmlan_private pmpriv,
                                         IN HostCmd_DS_COMMAND *cmd,
                                         IN t_u16 cmd_action,
                                         IN t_u32 cmd_oid,
                                         IN pmlan_ioctl_req pioctl_buf,
                                         IN t_void *pdata_buf)
{
    HostCmd_DS_802_11_SNMP_MIB *psnmp_mib = &cmd->params.smib;
    mlan_status ret                       = MLAN_STATUS_SUCCESS;
    t_u8 *psnmp_oid                       = MNULL;
    t_u8 i;

    t_u8 snmp_oids[] = {
        (t_u8)tkip_mic_failures,
        (t_u8)ccmp_decrypt_errors,
        (t_u8)wep_undecryptable_count,
        (t_u8)wep_icv_error_count,
        (t_u8)decrypt_failure_count,
        (t_u8)dot11_mcast_tx_count,
        (t_u8)dot11_failed_count,
        (t_u8)dot11_retry_count,
        (t_u8)dot11_multi_retry_count,
        (t_u8)dot11_frame_dup_count,
        (t_u8)dot11_rts_success_count,
        (t_u8)dot11_rts_failure_count,
        (t_u8)dot11_ack_failure_count,
        (t_u8)dot11_rx_fragment_count,
        (t_u8)dot11_mcast_rx_frame_count,
        (t_u8)dot11_fcs_error_count,
        (t_u8)dot11_tx_frame_count,
        (t_u8)dot11_rsna_tkip_cm_invoked,
        (t_u8)dot11_rsna_4way_hshk_failures,
    };

    ENTER();

    if (cmd_action == HostCmd_ACT_GEN_GET)
    {
        cmd->command          = wlan_cpu_to_le16(HostCmd_CMD_802_11_SNMP_MIB);
        psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_GET);
        if (cmd_oid == (t_u32)StopDeauth_i)
        {
            psnmp_mib->oid      = wlan_cpu_to_le16((t_u16)StopDeauth_i);
            psnmp_mib->buf_size = wlan_cpu_to_le16(sizeof(t_u8));
            cmd->size           = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_SNMP_MIB) + S_DS_GEN);
        }
        else
        {
            cmd->size = wlan_cpu_to_le16(sizeof(t_u16) + S_DS_GEN + sizeof(snmp_oids) * sizeof(MrvlIEtypes_snmp_oid_t));
            psnmp_oid = (t_u8 *)&psnmp_mib->oid;
            for (i = 0; i < sizeof(snmp_oids); i++)
            {
                /* SNMP OID header type */
                *(t_u16 *)(void *)psnmp_oid = wlan_cpu_to_le16(snmp_oids[i]);
                psnmp_oid += sizeof(t_u16);
                /* SNMP OID header length */
                *(t_u16 *)(void *)psnmp_oid = wlan_cpu_to_le16(sizeof(t_u32));
                psnmp_oid += sizeof(t_u16) + sizeof(t_u32);
            }
        }
    }
    else
    { /* cmd_action == ACT_SET */
        cmd->command          = wlan_cpu_to_le16(HostCmd_CMD_802_11_SNMP_MIB);
        cmd->size             = sizeof(HostCmd_DS_802_11_SNMP_MIB) - 1U + S_DS_GEN;
        psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);

        switch (cmd_oid)
        {
            case Dot11D_i:
            case Dot11H_i:
                psnmp_mib->oid      = wlan_cpu_to_le16((t_u16)cmd_oid);
                psnmp_mib->buf_size = wlan_cpu_to_le16(sizeof(t_u16));
                // ul_temp = *(t_u32 *) pdata_buf;
                //*((t_u16 *) (psnmp_mib->value)) = wlan_cpu_to_le16((t_u16) ul_temp);
                cmd->size += (t_u16)sizeof(t_u16);
                break;
            case StopDeauth_i:
                psnmp_mib->oid      = wlan_cpu_to_le16((t_u16)cmd_oid);
                psnmp_mib->buf_size = wlan_cpu_to_le16(sizeof(t_u8));
                psnmp_mib->value[0] = *((t_u8 *)pdata_buf);
                cmd->size += (t_u16)sizeof(t_u8);
                break;
            default:
                PRINTM(MERROR, "Unsupported OID.\n");
                ret = MLAN_STATUS_FAILURE;
                break;
        }
        cmd->size = wlan_cpu_to_le16(cmd->size);
    }

    LEAVE();
    return ret;
}


/**
 *  @brief This function prepares command of deauth station
 *
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param pdata_buf    A pointer to data buffer
 *  @return         MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_uap_cmd_sta_deauth(pmlan_private pmpriv, IN HostCmd_DS_COMMAND *cmd, IN t_void *pdata_buf)
{
    HostCmd_DS_STA_DEAUTH *pcmd_sta_deauth = (HostCmd_DS_STA_DEAUTH *)&cmd->params.sta_deauth;
    mlan_deauth_param *deauth              = (mlan_deauth_param *)pdata_buf;

    ENTER();
    cmd->command = wlan_cpu_to_le16(HOST_CMD_APCMD_STA_DEAUTH);
    cmd->size    = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_STA_DEAUTH));
    (void)__memcpy(pmpriv->adapter, pcmd_sta_deauth->mac, deauth->mac_addr, MLAN_MAC_ADDR_LENGTH);
    pcmd_sta_deauth->reason = wlan_cpu_to_le16(deauth->reason_code);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}




/********************************************************
    Global Functions
********************************************************/
/**
 *  @brief This function prepare the command before sending to firmware.
 *
 *  @param priv       A pointer to mlan_private structure
 *  @param cmd_no       Command number
 *  @param cmd_action   Command action: GET or SET
 *  @param cmd_oid      Cmd oid: treated as sub command
 *  @param pioctl_buf   A pointer to MLAN IOCTL Request buffer
 *  @param pdata_buf    A pointer to information buffer
 *  @param pcmd_buf      A pointer to cmd buf
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_ops_uap_prepare_cmd(IN t_void *priv,
                                     IN t_u16 cmd_no,
                                     IN t_u16 cmd_action,
                                     IN t_u32 cmd_oid,
                                     IN t_void *pioctl_buf,
                                     IN t_void *pdata_buf,
                                     IN t_void *pcmd_buf)
{
    HostCmd_DS_COMMAND *cmd_ptr = (HostCmd_DS_COMMAND *)pcmd_buf;
    mlan_private *pmpriv        = (mlan_private *)priv;
    mlan_status ret             = MLAN_STATUS_SUCCESS;
    pmlan_ioctl_req pioctl_req  = (mlan_ioctl_req *)pioctl_buf;

    ENTER();

    /* Prepare command */
    switch (cmd_no)
    {
        case HostCmd_CMD_SOFT_RESET:
        case HOST_CMD_APCMD_BSS_STOP:
        case HOST_CMD_APCMD_BSS_START:
        case HOST_CMD_APCMD_SYS_INFO:
        case HOST_CMD_APCMD_SYS_RESET:
        case HOST_CMD_APCMD_STA_LIST:
            cmd_ptr->command = wlan_cpu_to_le16(cmd_no);
            cmd_ptr->size    = wlan_cpu_to_le16(S_DS_GEN);
            break;
        case HOST_CMD_APCMD_SYS_CONFIGURE:
            ret = wlan_uap_cmd_sys_configure(pmpriv, cmd_ptr, cmd_action, (pmlan_ioctl_req)pioctl_buf, pdata_buf);
            break;
        case HostCmd_CMD_802_11_SNMP_MIB:
            ret = wlan_uap_cmd_snmp_mib(pmpriv, cmd_ptr, cmd_action, cmd_oid, (pmlan_ioctl_req)pioctl_buf, pdata_buf);
            break;
        case HostCmd_CMD_802_11D_DOMAIN_INFO:
            if (pmpriv->support_11d_APIs != NULL)
            {
                ret = pmpriv->support_11d_APIs->wlan_cmd_802_11d_domain_info_p(pmpriv, cmd_ptr, cmd_action);
            }
            break;
        case HOST_CMD_APCMD_STA_DEAUTH:
            ret = wlan_uap_cmd_sta_deauth(pmpriv, cmd_ptr, pdata_buf);
            break;
        case HostCmd_CMD_11N_CFG:
            ret = wlan_cmd_11n_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf);
            break;
        case HostCmd_CMD_11N_ADDBA_REQ:
            ret = wlan_cmd_11n_addba_req(pmpriv, cmd_ptr, pdata_buf);
            break;
        case HostCmd_CMD_11N_DELBA:
            ret = wlan_cmd_11n_delba(pmpriv, cmd_ptr, pdata_buf);
            break;
        case HostCmd_CMD_TX_RATE_CFG:
            ret = wlan_cmd_tx_rate_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf);
            break;
        case HostCmd_CMD_802_11_TX_RATE_QUERY:
            cmd_ptr->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_TX_RATE_QUERY);
            cmd_ptr->size    = wlan_cpu_to_le16(sizeof(HostCmd_TX_RATE_QUERY) + S_DS_GEN);
            pmpriv->tx_rate  = 0;
            ret              = MLAN_STATUS_SUCCESS;
            break;
        case HostCmd_CMD_11AC_CFG:
            ret = wlan_cmd_11ac_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf);
            break;
        default:
            PRINTM(MERROR, "PREP_CMD: unknown command- %#x\n", cmd_no);
            if (pioctl_req != NULL)
            {
                pioctl_req->status_code = MLAN_ERROR_CMD_INVALID;
            }
            ret = MLAN_STATUS_FAILURE;
            break;
    }
    LEAVE();
    return ret;
}

