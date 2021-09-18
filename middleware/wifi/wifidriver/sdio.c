/** @file sdio.c
 *
 *  @brief  This file provides sdio related Generic API
 *
 *  Copyright 2021 NXP
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

#include <mlan_api.h>

#include <mlan_sdio_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <wm_os.h>
#include <wm_utils.h>
#include <mlan_fw.h>
#include "wifi-sdio.h"
#include "fsl_sdmmc_common.h"
#include "fsl_sdmmc_host.h"
#include "fsl_common.h"
#include "sdmmc_config.h"
#include "sdio.h"

/*
 * Used to authorize the SDIO interrupt handler to accept the incoming
 * packet from the SDIO interface. If this flag is set a semaphore is
 * signalled.
 */

/* @brief decription about the read/write buffer
 * The size of the read/write buffer should be a multiple of 512, since SDHC/SDXC card uses 512-byte fixed
 * block length and this driver example is enabled with a SDHC/SDXC card.If you are using a SDSC card, you
 * can define the block length by yourself if the card supports partial access.
 * The address of the read/write buffer should align to the specific DMA data buffer address align value if
 * DMA transfer is used, otherwise the buffer address is not important.
 * At the same time buffer address/size should be aligned to the cache line size if cache is supported.
 */

/*! @brief Data written to the card */
SDK_ALIGN(uint8_t outbuf[DATA_BUFFER_SIZE], BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE);

/*! @brief Data read from the card */
#ifdef CONFIG_SDIO_MULTI_PORT_RX_AGGR
SDK_ALIGN(uint8_t inbuf[SDIO_MP_AGGR_DEF_PKT_LIMIT * 2 * DATA_BUFFER_SIZE], BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE);
#else
SDK_ALIGN(uint8_t inbuf[2 * DATA_BUFFER_SIZE], BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE);
#endif /*CONFIG_SDIO_MULTI_PORT_RX_AGGR*/

t_u32 ioport_g = 0;

/**
 * Function to set mlan ioport.
 * A weak definition of this is added here for compilation of
 * bt ble apps/non wifi apps
 * This funciton is defined in wifi-sdio.c as of this writing
 * for wifi files.
 */
__attribute__((weak)) void set_ioport_inmlan(t_u32 port)
{
    return;
}

uint8_t *wifi_get_sdio_outbuf(uint32_t *outbuf_len)
{
    *outbuf_len = sizeof(outbuf);
    return outbuf;
}

/**
 *  @brief This function reads the CARD_TO_HOST_EVENT_REG and
 *  checks if input bits are set
 *  @param bits		bits to check status against
 *  @return		true if bits are set
 *                      SDIO_POLLING_STATUS_TIMEOUT if bits
 *                      aren't set
 */
int wlan_card_status(t_u8 bits)
{
    uint32_t resp;
    t_u32 tries;

    for (tries = 0; tries < MAX_POLL_TRIES; tries++)
    {
        if (!(sdio_drv_creg_read(CARD_TO_HOST_EVENT_REG, 1, &resp)))
            return false;
        if ((resp & bits) == bits)
        {
            return true;
        }
        os_thread_sleep(os_msec_to_ticks(1));
    }
    return false;
}

void calculate_sdio_write_params(t_u32 txlen, t_u32 *tx_blocks, t_u32 *buflen)
{
    *tx_blocks = 1;
    *buflen    = MLAN_SDIO_BLOCK_SIZE;

    if (txlen > 512)
    {
        *tx_blocks = (txlen + MLAN_SDIO_BLOCK_SIZE_FW_DNLD - 1) / MLAN_SDIO_BLOCK_SIZE_FW_DNLD;
        /* this is really blksize */
        *buflen = MLAN_SDIO_BLOCK_SIZE_FW_DNLD;
    }
    else
    {
        *tx_blocks = (txlen + MLAN_SDIO_BLOCK_SIZE_FW_DNLD - 1) / MLAN_SDIO_BLOCK_SIZE_FW_DNLD;
        *buflen    = *tx_blocks * MLAN_SDIO_BLOCK_SIZE_FW_DNLD;

        *tx_blocks = 1; /* tx_blocks of size 512 */
    }
}

static uint32_t wlan_card_read_scratch_reg(void)
{
    uint32_t val    = 0;
    uint32_t rd_len = 0;

    sdio_drv_creg_read(0x64, 1, &val);
    rd_len = (val & 0xff);
    sdio_drv_creg_read(0x65, 1, &val);
    rd_len |= ((val & 0xff) << 8);
    sdio_drv_creg_read(0x66, 1, &val);
    rd_len |= ((val & 0xff) << 16);
    sdio_drv_creg_read(0x67, 1, &val);
    rd_len |= ((val & 0xff) << 24);

    return rd_len;
}

static mlan_status wlan_sdio_init_ioport(void)
{
    uint32_t resp = 0;
    t_u8 data;

#if defined(SD8977) || defined(SD8978) || defined(SD8987) || defined(SD8997) || defined(SD9097) || defined(SD9098)
    ioport_g = MEM_PORT;

    wifi_io_d("IOPORT : (0x%x)", ioport_g);

    /* Enable sdio cmd53 new mode */
    sdio_drv_creg_read(CARD_CONFIG_2_1_REG, 1, &resp);
    data = (resp & 0xff) | CMD53_NEW_MODE;
    sdio_drv_creg_write(CARD_CONFIG_2_1_REG, 1, data, &resp);
    sdio_drv_creg_read(CARD_CONFIG_2_1_REG, 1, &resp);

    /* configure cmd port  */
    /* enable reading rx length from the register  */
    sdio_drv_creg_read(CMD_CONFIG_0, 1, &resp);
    data = (resp & 0xff) | CMD_PORT_RD_LEN_EN;
    sdio_drv_creg_write(CMD_CONFIG_0, 1, data, &resp);
    sdio_drv_creg_read(CMD_CONFIG_0, 1, &resp);

    /* enable Dnld/Upld ready auto reset for cmd port
     * after cmd53 is completed */
    sdio_drv_creg_read(CMD_CONFIG_1, 1, &resp);
    data = (resp & 0xff) | CMD_PORT_AUTO_EN;
    sdio_drv_creg_write(CMD_CONFIG_1, 1, data, &resp);
    sdio_drv_creg_read(CMD_CONFIG_1, 1, &resp);
#elif defined(SD8801)
    /* Read the PORT regs for IOPORT address */
    sdio_drv_creg_read(IO_PORT_0_REG, 1, &resp);
    ioport_g = (resp & 0xff);

    sdio_drv_creg_read(IO_PORT_1_REG, 1, &resp);
    ioport_g |= ((resp & 0xff) << 8);

    sdio_drv_creg_read(IO_PORT_2_REG, 1, &resp);
    ioport_g |= ((resp & 0xff) << 16);

    wifi_io_d("IOPORT : (0x%x)", ioport_g);
#endif

    /* Set Host interrupt reset to read to clear */
    sdio_drv_creg_read(HOST_INT_RSR_REG, 1, &resp);
    data = (resp & 0xff) | HOST_INT_RSR_MASK;
    sdio_drv_creg_write(HOST_INT_RSR_REG, 1, data, &resp);

    /* Dnld/Upld ready set to auto reset */
    sdio_drv_creg_read(CARD_MISC_CFG_REG, 1, &resp);
    data = (resp & 0xff) | AUTO_RE_ENABLE_INT;
    sdio_drv_creg_write(CARD_MISC_CFG_REG, 1, data, &resp);
    set_ioport_inmlan(ioport_g);
    return true;
}

t_u16 wlan_card_read_f1_base_regs(void)
{
    t_u16 reg;
    uint32_t resp = 0;

    sdio_drv_creg_read(READ_BASE_0_REG, 1, &resp);
    reg = resp & 0xff;
    sdio_drv_creg_read(READ_BASE_1_REG, 1, &resp);
    reg |= (resp & 0xff) << 8;

    return reg;
}

mlan_status sdio_init()
{
    uint32_t resp;
    /* Initialize SDIO driver */
    int rv = sdio_drv_init(NULL);
    if (rv != WM_SUCCESS)
    {
        wifi_io_e("SDIO driver init failed.");
        return MLAN_STATUS_FAILURE;
    }

#if 0
	sdio_drv = sdio_drv_open("MDEV_SDIO");
	if (!sdio_drv) {
		wifi_io_e("SDIO driver open failed.");
		return MLAN_STATUS_FAILURE;
	}
#endif
    int ret;
    ret = sdio_drv_creg_read(CARD_TO_HOST_EVENT_REG, 1, &resp);
    if (ret && (resp & (DN_LD_CARD_RDY)) == 0)
    {
        ret = wlan_card_status(UP_LD_CARD_RDY);
        if (ret != 0)
        {
            uint32_t rd_len;
            rd_len = wlan_card_read_scratch_reg();
            if (rd_len > 0)
            {
                sdio_drv_creg_write(FN1_BLOCK_SIZE_0, 0, 0x8, &resp);
                sdio_drv_creg_write(FN1_BLOCK_SIZE_1, 0, 0x0, &resp);

                uint8_t buf[256];
                ret = sdio_drv_read(0x10000, 1, rd_len, 8, buf, &resp);
                if (!ret)
                {
                    wifi_io_e(
                        "SDIO read failed, "
                        "resp:%x",
                        resp);
                    return MLAN_STATUS_FAILURE;
                }
            }
        }
    }
    else if (!ret)
    {
        wifi_io_e("failed to read EVENT_REG");
        return MLAN_STATUS_FAILURE;
    }
    else
    { /* Do Nothing */
    }
    return MLAN_STATUS_SUCCESS;
}

mlan_status sdio_ioport_init()
{
    int sdiostatus = MLAN_STATUS_SUCCESS;
    /* this sets intmask on card and makes interrupts repeatable */
    sdiostatus = wlan_sdio_init_ioport();

    if (sdiostatus != true)
    {
        wifi_io_e("SDIO - Failed to read IOPORT");
        return MLAN_STATUS_FAILURE;
    }
    return MLAN_STATUS_SUCCESS;
}

/**
 * Interrupt callback handler registered with the SDIO driver.
 * A weak definition of this is added here for compilation of
 * bt ble apps/non wifi apps
 * This funciton is defined in wifi-sdio.c as of this writing
 * for wifi files.
 */
__attribute__((weak)) void handle_cdint(int error)
{
    return;
}
