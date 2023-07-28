/*
 * Copyright 2020,2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * NOTE that if there is any change in this file, please make sure
 * rebuild the corresponding library and use the new library to
 * replace the one in your project.
 */

#ifndef NX_USER_H
#define NX_USER_H

/* Refer to nx_user_sample.h for more information. */

#define NX_PACKET_ALIGNMENT 32

#define NX_DISABLE_ERROR_CHECKING
#define NX_TCP_ACK_EVERY_N_PACKETS 2
#define NX_DISABLE_RX_SIZE_CHECKING
#define NX_DISABLE_ARP_INFO
#define NX_DISABLE_IP_INFO
//#define NX_DISABLE_ICMP_INFO
#define NX_DISABLE_IGMPV2
#define NX_DISABLE_IGMP_INFO
#define NX_DISABLE_PACKET_INFO
#define NX_DISABLE_RARP_INFO
#define NX_DISABLE_TCP_INFO
#define NX_DISABLE_UDP_INFO
#define NX_DISABLE_EXTENDED_NOTIFY_SUPPORT
#define NX_DISABLE_INCLUDE_SOURCE_CODE

/* config for DNS */
#define NX_DNS_CLIENT_USER_CREATE_PACKET_POOL
#define NX_DNS_CLIENT_CLEAR_QUEUE

/* config for MQTT */
#define NXD_MQTT_REQUIRE_TLS

/* NXD for MQTT non-blocking.  */
#define NX_ENABLE_EXTENDED_NOTIFY_SUPPORT

/* MQTT */
#define NXD_MQTT_CLOUD_ENABLE
#define NXD_MQTT_PING_TIMEOUT_DELAY     500
#define NXD_MQTT_SOCKET_TIMEOUT         0

/* Secure */
#define NX_SECURE_ENABLE
#define NX_SECURE_TLS_DISABLE_TLS_1_1
#define NX_ENABLE_IP_PACKET_FILTER

/* This option enables deferred driver packet handling. This allows the driver to place a raw
   packet on the IP instance and have the driver's real processing routine called from the NetX internal
   IP helper thread.  */
#define NX_DRIVER_DEFERRED_PROCESSING

/* The link driver is able to specify extra capability, such as checksum offloading features. */
#define NX_ENABLE_INTERFACE_CAPABILITY


#endif /* NX_USER_H */
