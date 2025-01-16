/*
 * Copyright 2022-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lwip/opt.h"

#if !(CHECKSUM_GEN_TCP == CHECKSUM_GEN_UDP && CHECKSUM_GEN_UDP == CHECKSUM_GEN_ICMP)
#error \
    "CHECKSUM_GEN_TCP, CHECKSUM_GEN_UDP, CHECKSUM_GEN_ICMP preprocessor symbols must be set to the same value (hardware-assisted protocol checksum generation either turned on or off)."
#endif

#if !(CHECKSUM_CHECK_TCP == CHECKSUM_CHECK_UDP && CHECKSUM_CHECK_UDP == CHECKSUM_CHECK_ICMP)
#error \
    "CHECKSUM_CHECK_TCP, CHECKSUM_CHECK_UDP, CHECKSUM_CHECK_ICMP preprocessor symbols must be set to the same value (hardware-assisted protocol checksum validation either turned on or off)."
#endif

/* Errata ERR052152: ENET: The hardware acceleration feature for ICMP checksum generation and checking does not work for
 * IPv6. */
#if ((CHECKSUM_GEN_ICMP6 != 1) || (CHECKSUM_CHECK_ICMP6 != 1))
#error \
    "CHECKSUM_GEN_ICMP6 and CHECKSUM_CHECK_ICMP6 must be set to 1 because ENET peripheral HW does not generate or validate this type of checksum."
#endif

#if IP_REASSEMBLY == 1 && IP_REASS_MAX_PBUFS >= (ENET_RXBUFF_NUM - ENET_RXBD_NUM)
#error \
    "IP_REASS_MAX_PBUFS is equal to or higher than (ENET_RXBUFF_NUM - ENET_RXBD_NUM). This can cause congestion when dealing with fragmented packets."
#endif

#if IP_REASSEMBLY == 1 && \
    (CHECKSUM_CHECK_ICMP == 0 || CHECKSUM_CHECK_ICMP6 == 0 || CHECKSUM_CHECK_UDP == 0 || CHECKSUM_CHECK_TCP == 0)
#error \
    "IP_REASSEMBLY is enabled and hardware cannot do the verification of ICMP, ICMPv6, UDP and TCP checksums inside fragmented IP packets. " \
    "Please enable software verification of ICMP, ICMPv6, UDP and TCP checksums or disable IP_REASSEMBLY."
#endif

#if IP_FRAG == 1 && \
    (CHECKSUM_GEN_ICMP == 0 || CHECKSUM_GEN_ICMP6 == 0 || CHECKSUM_GEN_UDP == 0 || CHECKSUM_GEN_TCP == 0)
#error \
    "IP_FRAG is enabled and hardware cannot generate ICMP, ICMPv6, UDP and TCP checksums inside fragmented IP packets. " \
    "Please enable software generation of ICMP, ICMPv6, UDP and TCP checksums or disable IP_FRAG."
#endif
