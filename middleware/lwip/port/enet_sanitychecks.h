/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lwip/opt.h"

#ifdef IP_REASSEMBLY
#if IP_REASSEMBLY == 1 && IP_REASS_MAX_PBUFS >= (ENET_RXBUFF_NUM - ENET_RXBD_NUM)
#error \
    "IP_REASS_MAX_PBUFS is equal to or higher than (ENET_RXBUFF_NUM - ENET_RXBD_NUM). This can cause congestion when dealing with fragmented packets."
#endif
#endif
