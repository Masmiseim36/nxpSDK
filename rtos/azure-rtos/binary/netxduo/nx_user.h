/*
 * Copyright 2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef NX_USER_H
#define NX_USER_H

/* Refer to nx_user_sample.h for more information. */

/* The driver enables the checksum offload feature. Therefore
 * the following symbols must be defined.
 */

#define NX_PACKET_ALIGNMENT 32

#define NX_DISABLE_ICMPV4_RX_CHECKSUM
#define NX_DISABLE_ICMPV4_TX_CHECKSUM
#define NX_DISABLE_IP_RX_CHECKSUM
#define NX_DISABLE_IP_TX_CHECKSUM
#define NX_DISABLE_TCP_RX_CHECKSUM
#define NX_DISABLE_TCP_TX_CHECKSUM
#define NX_DISABLE_UDP_RX_CHECKSUM
#define NX_DISABLE_UDP_TX_CHECKSUM

#define NX_DISABLE_ERROR_CHECKING
#define NX_TCP_ACK_EVERY_N_PACKETS 2
#define NX_DISABLE_RX_SIZE_CHECKING
#define NX_DISABLE_ARP_INFO
#define NX_DISABLE_IP_INFO
#define NX_DISABLE_ICMP_INFO
#define NX_DISABLE_IGMPV2
#define NX_DISABLE_IGMP_INFO
#define NX_DISABLE_PACKET_INFO
#define NX_DISABLE_RARP_INFO
#define NX_DISABLE_TCP_INFO
#define NX_DISABLE_UDP_INFO
#define NX_DISABLE_EXTENDED_NOTIFY_SUPPORT
#define NX_DISABLE_INCLUDE_SOURCE_CODE

#define NX_SECURE_ENABLE

/* config for DNS */
#define NX_DNS_CLIENT_USER_CREATE_PACKET_POOL

/* config for MQTT */
#define NXD_MQTT_REQUIRE_TLS

#endif /* NX_USER_H */
