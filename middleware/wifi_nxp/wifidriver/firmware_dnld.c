/** @file firmware_load.c
 *
 *  @brief  This file provides firmware download related API
 *
 *  Copyright 2021-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
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

static const uint8_t *wlanfw;

/* remove this after mlan integration complete */
enum
{
    FWDNLD_STATUS_FAILURE    = 0xffffffff,
    FWDNLD_STATUS_SUCCESS    = 0,
    FWDNLD_CARD_NOT_DETECTED = 3,
    FWDNLD_STATUS_FW_DNLD_FAILED,
    FWDNLD_STATUS_FW_NOT_DETECTED = 5,
    FWDNLD_STATUS_FW_NOT_READY,
    FWDNLD_STATUS_FW_XZ_FAILED,
    FWDNLD_CARD_CMD_TIMEOUT
};

static void wlan_card_fw_status(t_u16 *dat)
{
    uint32_t resp = 0;

    (void)sdio_drv_creg_read(CARD_FW_STATUS0_REG, 1, &resp);
    *dat = (t_u16)(resp & 0xffU);
    (void)sdio_drv_creg_read(CARD_FW_STATUS1_REG, 1, &resp);
    *dat |= (t_u16)((resp & 0xffU) << 8);
}

static bool wlan_card_ready_wait(t_u32 card_poll)
{
    t_u16 dat = 0U;
    t_u32 i   = 0U;

    for (i = 0; i < card_poll; i++)
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

static int32_t wlan_download_normal_fw(const t_u8 *wlanfw_dl, t_u32 firmwarelen, t_u32 ioport)
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

        if (len == 0U)
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
        if ((firmwarelen - offset) < txlen)
        {
            txlen = firmwarelen - offset;
        }

        calculate_sdio_write_params(txlen, &tx_blocks, &buflen);
        (void)memcpy((void *)outbuf, (const void *)(wlanfw_dl + offset), txlen);

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

    bool rv = sdio_drv_creg_write(FN1_BLOCK_SIZE_0, 0, 0, &resp);
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
int32_t firmware_download(const uint8_t *fw_start_addr, const size_t size)
{
    t_u32 firmwarelen;
    int32_t ret;

    /* set fw download block size */
    ret = wlan_set_fw_dnld_size();
    if (ret != FWDNLD_STATUS_SUCCESS)
    {
        return ret;
    }

    wlanfw = fw_start_addr;

    fwdnld_io_d("Start copying wlan firmware over sdio from 0x%x", (t_u32)wlanfw);

    firmwarelen = size;

    {
        fwdnld_io_d(
            "Un-compressed image found, start download,"
            " len: %d",
            firmwarelen);
        ret = wlan_download_normal_fw(wlanfw, firmwarelen, ioport_g);
    }

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
