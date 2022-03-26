/** @file firmware_load.c
 *
 *  @brief  This file provides firmware download related API
 *
 *  Copyright 2021-2022 NXP
 *
 *  NXP CONFIDENTIAL
 *  The source code contained or described herein and all documents related to
 *  the source code ("Materials") are owned by NXP, its
 *  suppliers and/or its licensors. Title to the Materials remains with NXP,
 *  its suppliers and/or its licensors. The Materials contain
 *  trade secrets and proprietary and confidential information of NXP, its
 *  suppliers and/or its licensors. The Materials are protected by worldwide copyright
 *  and trade secret laws and treaty provisions. No part of the Materials may be
 *  used, copied, reproduced, modified, published, uploaded, posted,
 *  transmitted, distributed, or disclosed in any way without NXP's prior
 *  express written permission.
 *
 *  No license under any patent, copyright, trade secret or other intellectual
 *  property right is granted to or conferred upon you by disclosure or delivery
 *  of the Materials, either expressly, by implication, inducement, estoppel or
 *  otherwise. Any license under such intellectual property rights must be
 *  express and approved by NXP in writing.
 *
 */

#include <mlan_sdio_api.h>


/* Additional WMSDK header files */
#include "mlan_sdio_defs.h"
#include "type_decls.h"
#include "fsl_sdmmc_common.h"
#include "fsl_sdmmc_host.h"
#include "fsl_common.h"
#include "sdmmc_config.h"
#include "sdio.h"
#include "firmware_dnld.h"

extern t_u32 ioport_g;

const uint8_t *wlanfw;

/* remove this after mlan integration complete */
enum
{
    FWDNLD_STATUS_FAILURE = 0xffffffff,
    FWDNLD_STATUS_SUCCESS = 0,
    FWDNLD_CARD_NOT_DETECTED = 3,
    FWDNLD_STATUS_FW_DNLD_FAILED,
    FWDNLD_STATUS_FW_NOT_DETECTED = 5,
    FWDNLD_STATUS_FW_NOT_READY,
    FWDNLD_STATUS_FW_XZ_FAILED,
    FWDNLD_CARD_CMD_TIMEOUT
};

static int wlan_card_fw_status(t_u16 *dat)
{
    uint32_t resp = 0;

    (void)sdio_drv_creg_read(CARD_FW_STATUS0_REG, 1, &resp);
    *dat = resp & 0xff;
    (void)sdio_drv_creg_read(CARD_FW_STATUS1_REG, 1, &resp);
    *dat |= (resp & 0xff) << 8;

    return true;
}

static bool wlan_card_ready_wait(t_u32 poll)
{
    t_u16 dat;
    int i;

    for (i = 0; i < poll; i++)
    {
        (void)wlan_card_fw_status(&dat);
        if (dat == FIRMWARE_READY)
        {
            fwdnld_io_d("Firmware Ready");
            return true;
        }
        os_thread_sleep(os_msec_to_ticks(5));
    }
    return false;
}

int32_t wlan_download_normal_fw(enum wlan_fw_storage_type st, const t_u8 *wlanfw, t_u32 firmwarelen, t_u32 ioport)
{
    t_u32 tx_blocks = 0, txlen = 0, buflen = 0;
    t_u16 len    = 0;
    t_u32 offset = 0;
    t_u32 tries  = 0;
    uint32_t resp;
    uint32_t outbuf_len;

    (void)memset(outbuf, 0, SDIO_OUTBUF_LEN);
    (void)wifi_get_sdio_outbuf(&outbuf_len);

    do
    {
        if (offset >= firmwarelen)
        {
            break;
        }

        /* Read CARD_STATUS_REG (0X30) FN =1 */
        for (tries = 0; tries < MAX_POLL_TRIES; tries++)
        {
            if (wlan_card_status(DN_LD_CARD_RDY | CARD_IO_READY) == true)
            {
                len = wlan_card_read_f1_base_regs();
            }
            else
            {
                fwdnld_io_e("Error in wlan_card_status()");
                break;
            }

            // (void)PRINTF("len %d =>", len);
            if (len != 0U)
            {
                break;
            }
        }

        if (!len)
        {
            fwdnld_io_e("Card timeout %s:%d", __func__, __LINE__);
            return FWDNLD_STATUS_FAILURE;
        }
        else if (len > outbuf_len)
        {
            fwdnld_io_e("FW Download Failure. Invalid len");
            return FWDNLD_STATUS_FAILURE;
        }
        else
        { /* Do Nothing */
        }

        txlen = len;

        /* Set blocksize to transfer - checking for last block */
        if (firmwarelen && (firmwarelen - offset) < txlen)
        {
            txlen = firmwarelen - offset;
        }

        calculate_sdio_write_params(txlen, &tx_blocks, &buflen);
#if 0
		if (st == WLAN_FW_IN_FLASH)
			flash_drv_read(fl_dev, outbuf, txlen,
				       (t_u32) (wlanfw + offset));
		else
#endif
        if (st == WLAN_FW_IN_RAM)
        {
            (void)memcpy((void *)outbuf, (const void *)(wlanfw + offset), txlen);
        }

        (void)sdio_drv_write(ioport, 1, tx_blocks, buflen, (t_u8 *)outbuf, &resp);
        offset += txlen;

        // (void)PRINTF("  offset %d\r\n", offset);
        len = 0;
    } while (true);

    return FWDNLD_STATUS_SUCCESS;
}


/*
 * FW dnld blocksize set 0x110 to 0 and 0x111 to 0x01 => 0x100 => 256
 * Note this only applies to the blockmode we use 256 bytes
 * as block because MLAN_SDIO_BLOCK_SIZE = 256
 */
static int32_t wlan_set_fw_dnld_size(void)
{
    uint32_t resp;

    int rv = sdio_drv_creg_write(FN1_BLOCK_SIZE_0, 0, 0, &resp);
    if (rv == false)
    {
        return FWDNLD_STATUS_FAILURE;
    }

    rv = sdio_drv_creg_write(FN1_BLOCK_SIZE_1, 0, 1, &resp);
    if (rv == false)
    {
        return FWDNLD_STATUS_FAILURE;
    }
    return FWDNLD_STATUS_SUCCESS;
}

/*
 * Download firmware to the card through SDIO.
 * The firmware is stored in Flash.
 */
int32_t firmware_download(enum wlan_fw_storage_type st, const uint8_t *fw_ram_start_addr, const size_t size)
{
    t_u32 firmwarelen;
    wlanfw_hdr_type wlanfwhdr;
    int32_t ret;

    /* set fw download block size */
    ret = wlan_set_fw_dnld_size();
    if (ret != FWDNLD_STATUS_SUCCESS)
    {
        return ret;
    }
#if 0
	if (st == WLAN_FW_IN_FLASH) {
		fl_dev = flash_drv_open(fl->fl_dev);
		if (fl_dev == NULL) {
			fwdnld_io_e("Flash drv init is required before open");
			return FWDNLD_STATUS_FW_NOT_DETECTED;
		}
	}

	if (st == WLAN_FW_IN_FLASH)
		wlanfw = (t_u8 *)fl->fl_start;
	else
#endif
    if (st == WLAN_FW_IN_RAM)
    {
        wlanfw = fw_ram_start_addr;
    }

    fwdnld_io_d("Start copying wlan firmware over sdio from 0x%x", (t_u32)wlanfw);

#if 0
	if (st == WLAN_FW_IN_FLASH)
		flash_drv_read(fl_dev, (t_u8 *) &wlanfwhdr, sizeof(wlanfwhdr),
			       (t_u32) wlanfw);
	else
#endif
    if (st == WLAN_FW_IN_RAM)
    {
        (void)memcpy((void *)&wlanfwhdr, (const void *)wlanfw, sizeof(wlanfwhdr));
    }

    //	if (wlanfwhdr.magic_number != WLAN_MAGIC_NUM) {
    //		fwdnld_io_e("WLAN FW not detected in Flash.");
    //		return MLAN_STATUS_FW_NOT_DETECTED;
    //	}

    //	fwdnld_io_d("Valid WLAN FW found in %s flash",
    //			fl->fl_dev ? "external" : "internal");

    /* skip the wlanhdr and move wlanfw to beginning of the firmware */
    //	wlanfw += sizeof(wlanfwhdr);
    firmwarelen = size;

    {
        fwdnld_io_d(
            "Un-compressed image found, start download,"
            " len: %d",
            firmwarelen);
        ret = wlan_download_normal_fw(st, wlanfw, firmwarelen, ioport_g);
    }
#if 0
	if (st == WLAN_FW_IN_FLASH)
		flash_drv_close(fl_dev);
#endif
    if (ret != FWDNLD_STATUS_SUCCESS)
    {
        return ret;
    }
    if (wlan_card_ready_wait(1000) != true)
    {
        fwdnld_io_e("SDIO - FW Ready Registers not set");
        return FWDNLD_STATUS_FAILURE;
    }
    else
    {
        fwdnld_io_d("WLAN FW download Successful");
        return FWDNLD_STATUS_SUCCESS;
    }
}
