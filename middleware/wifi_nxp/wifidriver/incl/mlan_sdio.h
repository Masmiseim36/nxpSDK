/** @file mlan_sdio.h
 *
 *  @brief This file contains definitions for SDIO interface.
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
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




/** Read interrupt status */
t_void wlan_interrupt(mlan_adapter *pmadapter);
/** Process Interrupt Status */
/* wmsdk */
/* mlan_status wlan_process_int_status(mlan_adapter * pmadapter); */
/** Transfer data to card */
#endif /* _MLAN_SDIO_H */
