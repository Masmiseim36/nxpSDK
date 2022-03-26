/** @file mlan_sdio.h
 *
 *  @brief This file contains definitions for SDIO interface.
 *
 *  Copyright 2008-2022 NXP
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
t_void wlan_interrupt(pmlan_adapter pmadapter);
/** Process Interrupt Status */
/* wmsdk */
/* mlan_status wlan_process_int_status(mlan_adapter * pmadapter); */
/** Transfer data to card */
#endif /* _MLAN_SDIO_H */
