/** @file firmware_load.c
 *
 *  @brief  This file provides firmware download related API
 *
 *  Copyright 2021-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */


#include <wmlog.h>
/* Additional WMSDK header files */
#include "type_decls.h"
#include "fsl_common.h"
#include "firmware_dnld.h"
#include "fwdnld_intf_abs.h"

static const uint8_t *conn_fw;

/* remove this after mlan integration complete */
enum
{
    FWDNLD_STATUS_FAILURE = 0xffffffff,
    FWDNLD_STATUS_SUCCESS = 0,
    FWDNLD_STATUS_SKIP,
    FWDNLD_CARD_NOT_DETECTED = 3,
    FWDNLD_STATUS_FW_DNLD_FAILED,
    FWDNLD_STATUS_FW_NOT_DETECTED = 5,
    FWDNLD_STATUS_FW_NOT_READY,
    FWDNLD_STATUS_FW_XZ_FAILED,
    FWDNLD_CARD_CMD_TIMEOUT
};

static int32_t conn_download_normal_fw(const t_u8 *connfw_dl, t_u32 firmwarelen, fwdnld_intf_t *intf)
{
    t_u32 offset        = 0;
    int32_t ret         = FWDNLD_STATUS_FAILURE;
    t_u32 remaining_len = firmwarelen;
    uint32_t len        = 0;

    if (firmwarelen <= 0)
    {
        return ret;
    }

    if (intf->intf_s.fwdnld_intf_send)
    {
        ret = intf->intf_s.fwdnld_intf_send(intf, (const void *)(connfw_dl + offset), remaining_len, &len);
    }

    return ret;
}

/*
 * Download firmware to the card through SDIO.
 * The firmware is stored in Flash.
 * in param intf returned from the interface init
 */
int32_t firmware_download(const uint8_t *fw_start_addr, const size_t size, void *interface, uint8_t fw_reload)
{
    t_u32 firmwarelen;
    int32_t ret         = FWDNLD_STATUS_SUCCESS;
    fwdnld_intf_t *intf = (fwdnld_intf_t *)interface;

    if (size == 0)
    {
        return ret;
    }

    if (intf->intf_s.fwdnld_intf_prepare)
    {
        ret = intf->intf_s.fwdnld_intf_prepare(intf, NULL);
        if (ret != FWDNLD_STATUS_SUCCESS)
        {
            return ret;
        }
    }

#if defined(CONFIG_WIFI_IND_DNLD)
    if ((fw_reload != 0) && (intf->intf_s.fwdnld_intf_check_reload))
    {
        ret = intf->intf_s.fwdnld_intf_check_reload(intf, fw_reload);
        if (ret != FWDNLD_STATUS_SUCCESS)
        {
            return ret;
        }
    }
#endif

    conn_fw = fw_start_addr;

    fwdnld_io_d("Start copying connectivity firmware from 0x%x", (t_u32)conn_fw);

    firmwarelen = size;
    /*Making this section as #if 00 for now, as the decopress and
     * verification of compression etc funcitons are not present*/
    {
        fwdnld_io_d(
            "Un-compressed image found, start download,"
            " len: %d",
            firmwarelen);
        ret = conn_download_normal_fw(conn_fw, firmwarelen, intf);
    }

    if (ret != FWDNLD_STATUS_SUCCESS)
    {
        return FWDNLD_STATUS_FAILURE;
    }
    if (intf->intf_s.fwdnld_intf_check_ready)
    {
        return intf->intf_s.fwdnld_intf_check_ready(intf, NULL);
    }
    return ret;
}
