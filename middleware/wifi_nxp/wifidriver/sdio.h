/** @file sdio.h
 *
 *  @brief SDIO Generic API related header file
 *
 *  Copyright 2021-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */
#ifndef _SDIO_H_
#define _SDIO_H_

#include "fsl_sdmmc_common.h"
#include "fsl_sdmmc_host.h"
#include "fsl_common.h"
#include "sdmmc_config.h"

#include <wifi.h>

/*! @brief Data block count accessed in card */
#define DATA_BLOCK_COUNT (4U)
/*! @brief Data buffer size. */
#define DATA_BUFFER_SIZE (FSL_SDMMC_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)

#define sdio_io_e(...) wmlog_e("wifi_io", ##__VA_ARGS__)
#define sdio_io_w(...) wmlog_w("wifi_io", ##__VA_ARGS__)

#ifdef CONFIG_SDIO_IO_DEBUG
#define sdio_io_d(...) wmlog("wifi_io", ##__VA_ARGS__)
#else
#define sdio_io_d(...)
#endif /* ! CONFIG_SDIO_IO_DEBUG */

extern uint8_t outbuf[DATA_BUFFER_SIZE];

extern uint8_t inbuf[];

mlan_status sdio_init(void);
mlan_status sdio_ioport_init(void);
void calculate_sdio_write_params(t_u32 txlen, t_u32 *tx_blocks, t_u32 *buflen);
bool wlan_card_status(t_u8 bits);
t_u16 wlan_card_read_f1_base_regs(void);
uint8_t *wifi_get_sdio_outbuf(uint32_t *outbuf_len);

#endif //_SDIO_H_
