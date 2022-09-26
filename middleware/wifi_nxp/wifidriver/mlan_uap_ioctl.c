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
