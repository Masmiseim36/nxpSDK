/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lwip/opt.h"

#if (CHECKSUM_GEN_IP == 0 || CHECKSUM_GEN_UDP == 0 || CHECKSUM_GEN_TCP == 0 || CHECKSUM_GEN_ICMP == 0 || \
     CHECKSUM_GEN_ICMP6 == 0 || CHECKSUM_CHECK_ICMP == 0 || CHECKSUM_CHECK_ICMP6 == 0)
#error "Selected hardware-assisted checksum computation/verification feature is not supported on the NETC peripheral."
#endif

#if (CHECKSUM_CHECK_UDP != CHECKSUM_CHECK_TCP)
#error "Both CHECKSUM_CHECK_UDP and CHECKSUM_CHECK_TCP must be either enabled or disabled at the same time."
#endif

#ifdef IP_REASSEMBLY
#if IP_REASSEMBLY == 1 && IP_REASS_MAX_PBUFS >= (NETC_RXBUFF_NUM - NETC_RXBD_NUM)
#error \
    "IP_REASS_MAX_PBUFS is equal to or higher than (NETC_RXBUFF_NUM - NETC_RXBD_NUM). This can cause congestion when dealing with fragmented packets."
#endif
#endif

#if (NETC_PROMISCUOUS) == 0 && \
    (((LWIP_IPV6 && LWIP_IPV6_MLD) ? MEMP_NUM_MLD6_GROUP : 0) + ((LWIP_IGMP) ? MEMP_NUM_IGMP_GROUP : 0)) > 4
#error \
    "There are more hardware multicast groups than the NETC MAC filter can handle. If more multicast groups are desired, enable promiscuous mode (NETC_PROMISCUOUS) - filtering will be done in software."
#endif
