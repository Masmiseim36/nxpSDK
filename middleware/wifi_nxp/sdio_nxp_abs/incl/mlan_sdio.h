/** @file mlan_sdio.h
 *
 *  @brief This file contains definitions for SDIO interface.
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
/****************************************************
Change log:
****************************************************/

#ifndef _MLAN_SDIO_H
#define _MLAN_SDIO_H

#include "mlan_sdio_defs.h"

/** Event header Len*/
#define MLAN_EVENT_HEADER_LEN 8

/** SDIO byte mode size */
#define MAX_BYTE_MODE_SIZE 512

/** The base address for packet with multiple ports aggregation */
#define SDIO_MPA_ADDR_BASE 0x1000U

#ifdef SDIO_MULTI_PORT_TX_AGGR

/** SDIO Tx aggregation in progress ? */
#define MP_TX_AGGR_IN_PROGRESS(a) (a->mpa_tx.pkt_cnt > 0)

/** SDIO Tx aggregation buffer room for next packet ? */
#define MP_TX_AGGR_BUF_HAS_ROOM(a, mbuf, len) ((a->mpa_tx.buf_len + len) <= a->mpa_tx.buf_size)

/** Copy current packet (SDIO Tx aggregation buffer) to SDIO buffer */
#define MP_TX_AGGR_BUF_PUT(a, mbuf, port)                                                     \
    do                                                                                        \
    {                                                                                         \
        pmadapter->callbacks.moal_memmove(a->pmoal_handle, &a->mpa_tx.buf[a->mpa_tx.buf_len], \
                                          mbuf->pbuf + mbuf->data_offset, mbuf->data_len);    \
        a->mpa_tx.buf_len += mbuf->data_len;                                                  \
        if (!a->mpa_tx.pkt_cnt)                                                               \
        {                                                                                     \
            a->mpa_tx.start_port = port;                                                      \
        }                                                                                     \
        if (a->mpa_tx.start_port <= port)                                                     \
        {                                                                                     \
            a->mpa_tx.ports |= (1 << (a->mpa_tx.pkt_cnt));                                    \
        }                                                                                     \
        else                                                                                  \
        {                                                                                     \
            a->mpa_tx.ports |= (1 << (a->mpa_tx.pkt_cnt + 1 + (MAX_PORT - a->mp_end_port)));  \
        }                                                                                     \
        a->mpa_tx.pkt_cnt++;                                                                  \
    } while (0);

/** SDIO Tx aggregation limit ? */
#define MP_TX_AGGR_PKT_LIMIT_REACHED(a) (a->mpa_tx.pkt_cnt == a->mpa_tx.pkt_aggr_limit)

/** SDIO Tx aggregation port limit ? */
#define MP_TX_AGGR_PORT_LIMIT_REACHED(a)         \
    ((a->curr_wr_port < a->mpa_tx.start_port) && \
     (((MAX_PORT - a->mpa_tx.start_port) + a->curr_wr_port) >= SDIO_MP_AGGR_DEF_PKT_LIMIT))

/** Reset SDIO Tx aggregation buffer parameters */
#define MP_TX_AGGR_BUF_RESET(a)   \
    do                            \
    {                             \
        a->mpa_tx.pkt_cnt    = 0; \
        a->mpa_tx.buf_len    = 0; \
        a->mpa_tx.ports      = 0; \
        a->mpa_tx.start_port = 0; \
    } while (0);

#endif /* SDIO_MULTI_PORT_TX_AGGR */



/** Read interrupt status */
t_void wlan_interrupt(mlan_adapter *pmadapter);
/** Process Interrupt Status */
/* wmsdk */
/* mlan_status wlan_process_int_status(mlan_adapter * pmadapter); */

#if CONFIG_WIFI_IND_DNLD
mlan_status wlan_reset_fw(pmlan_adapter pmadapter);
#endif

/** Transfer data to card */
#endif /* _MLAN_SDIO_H */
