/** @file sdio.h
 *
 *  @brief SDIO Generic API related header file
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

extern uint8_t outbuf[DATA_BUFFER_SIZE];

/*! @brief Data read from the card */
#ifdef CONFIG_SDIO_MULTI_PORT_RX_AGGR
extern uint8_t inbuf[SDIO_MP_AGGR_DEF_PKT_LIMIT * 2 * DATA_BUFFER_SIZE];
#else
extern uint8_t inbuf[2 * DATA_BUFFER_SIZE];
#endif /*CONFIG_SDIO_MULTI_PORT_RX_AGGR*/

mlan_status sdio_init();
mlan_status sdio_ioport_init();
void calculate_sdio_write_params(t_u32 txlen, t_u32 *tx_blocks, t_u32 *buflen);
int wlan_card_status(t_u8 bits);
t_u16 wlan_card_read_f1_base_regs(void);
uint8_t *wifi_get_sdio_outbuf(uint32_t *outbuf_len);

#endif //_SDIO_H_
