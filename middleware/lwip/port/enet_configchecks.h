/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lwip/opt.h"

#if !(CHECKSUM_GEN_IP == CHECKSUM_GEN_UDP && CHECKSUM_GEN_UDP == CHECKSUM_GEN_TCP &&           \
      CHECKSUM_GEN_TCP == CHECKSUM_GEN_ICMP && CHECKSUM_GEN_ICMP == CHECKSUM_GEN_ICMP6 &&      \
      CHECKSUM_GEN_ICMP6 == CHECKSUM_CHECK_IP && CHECKSUM_CHECK_IP == CHECKSUM_CHECK_UDP &&    \
      CHECKSUM_CHECK_UDP == CHECKSUM_CHECK_TCP && CHECKSUM_CHECK_TCP == CHECKSUM_CHECK_ICMP && \
      CHECKSUM_CHECK_ICMP == CHECKSUM_CHECK_ICMP6)
#error \
    "All CHECKSUM_*_* preprocessor symbols must be set to the same value (all hardware-assisted checksum features either turned on or off)."
#endif

#ifdef IP_REASSEMBLY
#if IP_REASSEMBLY == 1 && IP_REASS_MAX_PBUFS >= (ENET_RXBUFF_NUM - ENET_RXBD_NUM)
#error \
    "IP_REASS_MAX_PBUFS is equal to or higher than (ENET_RXBUFF_NUM - ENET_RXBD_NUM). This can cause congestion when dealing with fragmented packets."
#endif
#endif
