/** @file mlan_uap_ioctl.c
 *
 *  @brief  This file provides handling of AP mode ioctls
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
 *  @brief Set/Get deauth control.
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
mlan_status wlan_uap_snmp_mib_ctrl_deauth(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv  = pmadapter->priv[pioctl_req->bss_index];
    mlan_status ret       = MLAN_STATUS_SUCCESS;
    mlan_ds_snmp_mib *mib = (mlan_ds_snmp_mib *)(void *)pioctl_req->pbuf;
    t_u16 cmd_action      = 0;

    ENTER();

    mib = (mlan_ds_snmp_mib *)(void *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
    {
        cmd_action = HostCmd_ACT_GEN_SET;
    }
    else
    {
        cmd_action = HostCmd_ACT_GEN_GET;
    }

    /* Send command to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_SNMP_MIB, cmd_action, StopDeauth_i, (t_void *)pioctl_req,
                           &mib->param.deauthctrl);

    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief MLAN uap ioctl handler
 *
 *  @param adapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status wlan_ops_uap_ioctl(t_void *adapter, pmlan_ioctl_req pioctl_req)
{
    pmlan_adapter pmadapter = (pmlan_adapter)adapter;
    mlan_status status      = MLAN_STATUS_SUCCESS;
    mlan_ds_rate *rate = MNULL;

    ENTER();

    switch (pioctl_req->req_id)
    {
        case MLAN_IOCTL_RATE:
            rate = (mlan_ds_rate *)(void *)pioctl_req->pbuf;
            if (rate->sub_command == MLAN_OID_RATE_CFG)
            {
                status = wlan_rate_ioctl_cfg(pmadapter, pioctl_req);
            }
            break;
        default:
            pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
            break;
    }
    LEAVE();
    return status;
}
