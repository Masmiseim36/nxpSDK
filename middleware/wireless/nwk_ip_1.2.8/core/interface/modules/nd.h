/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _ND_H
#define _ND_H
/*!=================================================================================================
\file       nd.h
\brief      This is a header file for the Neighbor Discovery for IP version 6 (IPv6) module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

/* Network Includes */
#include "network_utils.h"
#include "nd_tables.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/**************************************************************
* RFC4861. 10. Protocol Constants.
***************************************************************
*    Router constants:
*        MAX_INITIAL_RTR_ADVERT_INTERVAL 16 seconds
*        MAX_INITIAL_RTR_ADVERTISEMENTS 3 transmissions
*        FINAL_RTR_ADVERTISEMENTS 3 transmissions
*        MIN_DELAY_BETWEEN_RAS 3 seconds
*        MAX_RA_DELAY_TIME 0.5 seconds
*    Host constants:
*        MAX_RTR_SOLICITATION_DELAY 1 second
*        RTR_SOLICITATION_INTERVAL 4 seconds
*        MAX_RTR_SOLICITATIONS 3 transmissions
*    Node constants:
*        MAX_MULTICAST_SOLICIT 3 transmissions
*        MAX_UNICAST_SOLICIT 3 transmissions
*        MAX_ANYCAST_DELAY_TIME 1 second
*        MAX_NEIGHBOR_ADVERTISEMENT 3 transmissions
*        REACHABLE_TIME 30,000 milliseconds
*        RETRANS_TIMER 1,000 milliseconds
*        DELAY_FIRST_PROBE_TIME 5 seconds
*        MIN_RANDOM_FACTOR .5
*        MAX_RANDOM_FACTOR 1.5
***************************************************************/

/* If a host sends MAX_RTR_SOLICITATIONS solicitations, and receives no
 * Router Advertisements after having waited MAX_RTR_SOLICITATION_DELAY
 * seconds after sending the last solicitation, the host concludes that
 * there are no routers on the link for the purpose of [ADDRCONF].
 * However, the host continues to receive and process Router
 * Advertisements messages in the event that routers appear on the link.
 */

/* Router constants */
#define ND6_MAX_INITIAL_RTR_ADVERT_INTERVAL     16U     /*!< sec */
#define ND6_MAX_INITIAL_RTR_ADVERTISEMENTS      3U
#define ND6_FINAL_RTR_ADVERTISEMENTS            3U
#define ND6_MIN_DELAY_BETWEEN_RAS               3U
#define ND6_MAX_RA_DELAY_TIME                   500U    /*!< ms */

/* Host constants */
#define ND6_MAX_RTR_SOLICITATION_DELAY          1U      /*!< sec */
#define ND6_RTR_SOLICITATION_INTERVAL           4U      /*!< sec */
#define ND6_MAX_RTR_SOLICITATIONS               1U

/* Node constants */
#define ND6_MAX_MULTICAST_SOLICIT               3U
#define ND6_MAX_UNICAST_SOLICIT                 3U
#define ND6_MAX_ANYCAST_DELAY_TIME              1U      /*!< sec */
#define ND6_MAX_NEIGHBOR_ADVERSTISEMENT         3U
#define ND6_REACHABLE_TIME                      30U     /*!< sec */
#define ND6_RETRANS_TIMER                       1U      /*!< sec */
#define ND6_DELAY_FIRST_PROBE_TIME              5U      /*!< sec */


/* ND PIB Attributes */

/* Messages */

#define ND_PIB_MSG_RS_SEND_MASK                          (1U)
#define ND_PIB_MSG_RS_RCV_MASK                           (1U << 1U)
#define ND_PIB_MSG_RA_SEND_MASK                          (1U << 2U)
#define ND_PIB_MSG_RA_RCV_MASK                           (1U << 3U)
#define ND_PIB_MSG_NS_SEND_MASK                          (1U << 4U)
#define ND_PIB_MSG_NS_RCV_MASK                           (1U << 5U)
#define ND_PIB_MSG_NA_SEND_MASK                          (1U << 6U)
#define ND_PIB_MSG_NA_RCV_MASK                           (1U << 7U)
#define ND_PIB_MSG_REDIRECT_SEND_MASK                    (1U << 8U)
#define ND_PIB_MSG_REDIRECT_RCV_MASK                     (1U << 9U)
#define ND_PIB_MSG_DAR_SEND_MASK                         (1U << 10U)
#define ND_PIB_MSG_DAR_RCV_MASK                          (1U << 11U)
#define ND_PIB_MSG_DAC_SEND_MASK                         (1U << 12U)
#define ND_PIB_MSG_DAC_RCV_MASK                          (1U << 13U)

/* Options */

#define ND_PIB_OPT_STLL_ADDR_SEND_MASK                   (1U)
#define ND_PIB_OPT_STLL_ADDR_RCV_MASK                    (1U << 1U)
#define ND_PIB_OPT_PREFIX_INFO_SEND_MASK                 (1U << 2U)
#define ND_PIB_OPT_PREFIX_INFO_RCV_MASK                  (1U << 3U)
#define ND_PIB_OPT_RDR_HDR_SEND_MASK                     (1U << 4U)
#define ND_PIB_OPT_RDR_HDR_RCV_MASK                      (1U << 5U)
#define ND_PIB_OPT_MTU_SEND_MASK                         (1U << 6U)
#define ND_PIB_OPT_MTU_RCV_MASK                          (1U << 7U)
#define ND_PIB_OPT_AR_SEND_MASK                          (1U << 8U)
#define ND_PIB_OPT_AR_RCV_MASK                           (1U << 9U)
#define ND_PIB_OPT_CTXT_SEND_MASK                        (1U << 10U)
#define ND_PIB_OPT_CTXT_RCV_MASK                         (1U << 11U)
#define ND_PIB_OPT_ABRO_SEND_MASK                        (1U << 12U)
#define ND_PIB_OPT_ABRO_RCV_MASK                         (1U << 13U)

/* Behaviors */

#define ND_PIB_DISABLE_DAD_ON_EUI64_ADDR_MASK                  (1U)
#define ND_PIB_DISABLE_DAD_ON_LL_ADDR_MASK               (1U << 1U)
#define ND_PIB_JOIN_SOLICITED_MCAST_ADDR_MASK            (1U << 2U)
#define ND_PIB_NON_EUI64_ADDR_ASSIGN_WITH_DHCP_MASK      (1U << 3U)
#define ND_PIB_REG_ADDR_WITH_DAR_MASK                    (1U << 4U)
#define ND_PIB_SEND_PERIODIC_RA_MASK                     (1U << 5U)
#define ND_PIB_IGNORE_ONLINK_PREFIXES_MASK               (1U << 6U)
#define ND_PIB_RENEW_PREFIXES_ENABLED_MASK               (1U << 7U)


/* OTA message flags shift */
#define ND_OTA_M_FLAG_SHIFT                               7
#define ND_OTA_O_FLAG_SHIFT                               6
#define ND_OTA_PREF_FLAG_SHIFT                            3

/* ND PIB operations macros */

#define ND_PIB_CHECK(pibGroup, pibAttrId) (pibGroup & pibAttrId)

#define ND_MAX_DELAY_BEWTEEN_RAS    360      /* in seconds */

#define ND6_NDMSG_HOP_LIMIT         255U

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! ND message types (RFC4861). */
typedef enum
{
    gNdTypeRouterSolicitation_c    = 133U,
    gNdTypeRouterAdvertisement_c   = 134U,
    gNdTypeNeighborSolicitation_c  = 135U,
    gNdTypeNeighborAdvertisement_c = 136U,
    gNdTypeRedirect_c              = 137U,
    gNdTypeDuplAddrRequest_c       = 157U,
    gNdTypeDuplAddrConfirmation_c  = 158U,
} ndMsgTypes_e;

/*! ND option types (RFC4861). */
typedef enum
{
    gNdOptionSLLA_c            = 1U,    /*!< Source Link-layer Address */
    gNdOptionTLLA_c            = 2U,    /*!< Target Link-layer Address */
    gNdOptionPrefix_c          = 3U,    /*!< Prefix Information */
    gNdOptionRedirectHeader_c  = 4U,    /*!< Redirected Header */
    gNdOptionMTU_c             = 5U,    /*!< MTU */
    gNdOptionRouteInfo_c       = 24U,   /*!< Route Information Option */
    gNdOptionRDNSS_c           = 25U,   /*!< Recursive DNS Server Option */
    gNdOptionDNSSL_c           = 31U,   /*!< DNS Search List Option */
    gNdOptionAddrReg_c         = 33U,   /*!< Address Registration */
    gNdOptionContext_c         = 34U,   /*!< 6LoWPAN Context Option */
    gNdOptionAbro_c            = 35U,   /*!< Authoritative Border Router Option */
} ndOptionsTypes_e;

/*! ND AR Option status */
typedef enum
{
    gNdOptAroStatusSuccess_c   = 0U,    /*!< Success */
    gNdOptAroStatusDuplAddr_c  = 1U,    /*!< Duplicate Address */
    gNdOptAroStatusNCFull_c    = 2U     /*!< Neighbor Cache is Full */
} ndOptAroStatus_e;

typedef uint8_t ndOptAroStatus_t;

/*! RA flags */
typedef enum
{
    gNdRaFlagM_c            = 0x80U,    /*!< 1-bit "Managed address configuration" flag. When
                                         * set, it indicates that addresses are available via
                                         * Dynamic Host Configuration Protocol */

    gNdRaFlagO_c            = 0x40U     /*!< 1-bit "Other configuration" flag. When set, it
                                         * indicates that other configuration information is
                                         * available via DHCPv6. */
} raFlags_e;

/*! NA flags.*/
typedef enum
{
    gNdNaFlagRouter_c       = 0x80U,    /*!< Router flag. When set, the R-bit indicates that
                                         * the sender is a router. The R-bit is used by
                                         * Neighbor Unreachability Detection to detect a
                                         * router that changes to a host.*/

    gNdNaFlagSolicited_c    = 0x40U,    /*!< Solicited flag. When set, the S-bit indicates that
                                         * the advertisement was sent in response to a
                                         * Neighbor Solicitation from the Destination address.
                                         * The S-bit is used as a reachability confirmation
                                         * for Neighbor Unreachability Detection. It MUST NOT
                                         * be set in multicast advertisements or in
                                         * unsolicited unicast advertisements.*/

    gNdNaFlagOverride_c     = 0x20U,    /*!< Override flag. When set, the O-bit indicates that
                                         * the advertisement should override an existing cache
                                         * entry and update the cached link-layer address.
                                         * When it is not set the advertisement will not
                                         * update a cached link-layer address though it will
                                         * update an existing Neighbor Cache entry for which
                                         * no link-layer address is known. It SHOULD NOT be
                                         * set in solicited advertisements for anycast
                                         * addresses and in solicited proxy advertisements.
                                         * It SHOULD be set in other solicited advertisements
                                         * and in unsolicited advertisements.*/
} naFlags_e;

/*! Prefix Option flags */
typedef enum
{
    gNdPrefixOptionFlagL_c  = 0x80U,    /*!< 1-bit on-link flag. When set, indicates that this
                                         * prefix can be used for on-link determination. When
                                         * not set the advertisement makes no statement about
                                         * on-link or off-link properties of the prefix. In
                                         * other words, if the L flag is not set a host MUST
                                         * NOT conclude that an address derived from the
                                         * prefix is off-link. That is, it MUST NOT update a
                                         * previous indication that the address is on-link.*/

    gNdPrefixOptionFlagA_c  = 0x40U     /*!< 1-bit autonomous address-configuration flag. When
                                         * set indicates that this prefix can be used for
                                         * stateless address configuration as specified in
                                         * [ADDRCONF].*/
} prefixOptionFlags_e;

/*! Context Option flags */
typedef enum
{
    gNdContextOptionFlagC_c = 0x10U,    /*!< 1-bit context Compression flag.  This flag indicates
                                         * if the context is valid for use in compression. */

    gNdContextOptionCID_c   = 0x0F,     /* 4-bit Context Identifier for the prefix information */
} contextOptionFlags_e;

/**********************************************************************
* Router Solicitation Message Format
***********************************************************************
* Hosts send Router Solicitations in order to prompt routers to
* generate Router Advertisements quickly.

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |     Code      |          Checksum             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           Reserved                            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Options ...
    +-+-+-+-+-+-+-+-+-+-+-+-

***********************************************************************/
typedef struct ndRsHeader_tag
{
    uint8_t        reserved[4];
} ndRsHeader_t;

/**********************************************************************
* Router Advertisement Message Format
***********************************************************************
* Routers send out Router Advertisement messages periodically, or in
* response to Router Solicitations.

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |     Code      |          Checksum             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | Cur Hop Limit |M|O|0|Prf|Resvd|       Router Lifetime         |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                      Reachable Time                           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                      Retrans Timer                            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Options ...
    +-+-+-+-+-+-+-+-+-+-+-+-

***********************************************************************/
typedef struct ndRaHeader_tag
{
    uint8_t          curHopLimit;       /*!< 8-bit unsigned integer. The default value that
                                         * should be placed in the Hop Count field of the IP
                                         * header for outgoing IP packets. A value of zero
                                         * means unspecified (by this router). */
    uint8_t          flag;                /* ND6_RS_FLAG_M and/or ND6_RS_FLAG_O flags + Preference
                                            field.*/
    uint8_t          routerLifetime[2]; /*!< 16-bit unsigned integer. The lifetime associated
                                         * with the default router in units of seconds. The
                                         * field can contain values up to 65535 and receivers
                                         * should handle any value, while the sending rules in
                                         * Section 6 limit the lifetime to 9000 seconds. A
                                         * Lifetime of 0 indicates that the router is not a
                                         * default router and SHOULD NOT appear on the default
                                         * router list. The Router Lifetime applies only to
                                         * the router's usefulness as a default router; it
                                         * does not apply to information contained in other
                                         * message fields or options. Options that need time
                                         * limits for their information include their own
                                         * lifetime fields.*/
    uint8_t       reachableTime[4];     /*!< 32-bit unsigned integer. The time, in
                                         * milliseconds, that a node assumes a neighbor is
                                         * reachable after having received a reachability
                                         * confirmation. Used by the Neighbor Unreachability
                                         * Detection algorithm (see Section 7.3). A value of
                                         * zero means unspecified (by this router). */
    uint8_t       retransTimer[4];      /*!< 32-bit unsigned integer. The time, in
                                         * milliseconds, between retransmitted Neighbor
                                         * Solicitation messages. Used by address resolution
                                         * and the Neighbor Unreachability Detection algorithm
                                         * (see Sections 7.2 and 7.3). A value of zero means
                                         * unspecified (by this router).*/

} ndRaHeader_t;

/**********************************************************************
* Neighbor Solicitation Message Format
***********************************************************************
* Nodes send Neighbor Solicitations to request the link-layer address
* of a target node while also providing their own link-layer address to
* the target.

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |     Code      |          Checksum             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           Reserved                            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    +                                                               +
    |                                                               |
    +                       Target Address                          +
    |                                                               |
    +                                                               +
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Options ...
    +-+-+-+-+-+-+-+-+-+-+-+-

***********************************************************************/
typedef struct ndNsHeader_tag
{
    uint8_t        reserved[4];
    uint8_t        targetAddr[16];
} ndNsHeader_t;

/**********************************************************************
* Neighbor Advertisement Message Format
***********************************************************************
* A node sends Neighbor Advertisements in response to Neighbor
* Solicitations and sends unsolicited Neighbor Advertisements in order
* to (unreliably) propagate new information quickly.

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |     Code      |          Checksum             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |R|S|O|                     Reserved                            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    +                                                               +
    |                                                               |
    +                       Target Address                          +
    |                                                               |
    +                                                               +
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Options ...
    +-+-+-+-+-+-+-+-+-+-+-+-

***********************************************************************/
typedef struct ndNaHeader_tag
{
    uint8_t        flags;
    uint8_t        reserved[3];
    uint8_t        targetAddr[16];
} ndNaHeader_t;

/**********************************************************************
* Redirect Message Format (RFC 4861)
***********************************************************************
* Routers send Redirect packets to inform a host of a better first-hop
* node on the path to a destination.

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |     Code      |          Checksum             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           Reserved                            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    +                                                               +
    |                                                               |
    +                       Target Address                          +
    |                                                               |
    +                                                               +
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    +                                                               +
    |                                                               |
    +                    Destination Address                        +
    |                                                               |
    +                                                               +
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Options ...
    +-+-+-+-+-+-+-+-+-+-+-+-

 ***********************************************************************/
typedef struct ndRedirectHeader_tag
{
    uint8_t      reserved[4];
    uint8_t      targetAddr[16];
    uint8_t      destAddr[16];
} ndRedirectHeader_t;

/**********************************************************************
* Duplicate Address Message Format (RFC 6775)
***********************************************************************
* Duplicate address messages are used to register an address to a LBR.

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |     Code      |          Checksum             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |    Status     |   Reserved    |     Registration Lifetime     |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    +                            EUI-64                             +
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    +                                                               +
    |                                                               |
    +                       Registered Address                      +
    |                                                               |
    +                                                               +
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 ***********************************************************************/
typedef struct ndDuplAddrHeader_tag
{
    uint8_t      status;
    uint8_t      reserved;
    uint8_t      regLifetime[2];
    uint8_t      eui64[8];
    uint8_t      regAddr[16];
} ndDuplAddrHeader_t;

/***********************************************************************
 * ND option header
 ***********************************************************************/
typedef struct ndOptionHeader_tag
{
    uint8_t type;     /*!< Identifier of the type of option.*/
    uint8_t length;   /*!< The length of the option
                       * (including the type and length fields) in units of
                       * 8 octets.  The value 0 is invalid.  Nodes MUST
                       * silently discard an ND packet that contains an
                       * option with length zero.*/
} ndOptionHeader_t;

/***********************************************************************
 * Source/Target Link-layer Address option:

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |     Length    |       Link-Layer Address ...
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 ***********************************************************************/
typedef struct ndOptionLLA_tag
{
    ndOptionHeader_t optionHdr;
    uint8_t linkLayerAddr[8U];
} ndOptionLLA_t;

/***********************************************************************
 * Prefix Information option:

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |     Length    | Prefix Length |L|A| Reserved1 |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                         Valid Lifetime                        |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                       Preferred Lifetime                      |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           Reserved2                           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    +                                                               +
    |                                                               |
    +                             Prefix                            +
    |                                                               |
    +                                                               +
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 ***********************************************************************/
typedef struct ndOptionPrefix_tag
{
    ndOptionHeader_t optionHdr;
    uint8_t   prefixLength;         /*!< The number of leading bits
                                     * in the Prefix that are valid. The value ranges
                                     * from 0 to 128. The prefix length field provides
                                     * necessary information for on-link determination
                                     * (when combined with the L flag in the prefix
                                     * information option). It also assists with address
                                     * autoconfiguration as specified in [ADDRCONF], for
                                     * which there may be more restrictions on the prefix
                                     * length.*/
    uint8_t   flag;                 /*!< ND6_OPTION_FLAG_L and/or ND6_OPTION_FLAG_O flags.*/
    uint8_t   validLifetime[4];     /*!< The length of time in
                                     * seconds (relative to the time the packet is sent)
                                     * that the prefix is valid for the purpose of on-link
                                     * determination. A value of all one bits
                                     * (0xffffffff) represents infinity. The Valid
                                     * Lifetime is also used by [ADDRCONF].*/
    uint8_t   preferredLifetime[4]; /*!< The length of time in
                                     * seconds (relative to the time the packet is sent)
                                     * that addresses generated from the prefix via
                                     * stateless address autoconfiguration remain
                                     * preferred [ADDRCONF]. A value of all one bits
                                     * (0xffffffff) represents infinity. See [ADDRCONF].
                                     * Note that the value of this field MUST NOT exceed
                                     * the Valid Lifetime field to avoid preferring
                                     * addresses that are no longer valid.*/
    uint8_t   reserved[4];
    uint8_t   prefix[16];           /*!< An IP address or a prefix of an IP address. The
                                     * Prefix Length field contains the number of valid
                                     * leading bits in the prefix. The bits in the prefix
                                     * after the prefix length are reserved and MUST be
                                     * initialized to zero by the sender and ignored by
                                     * the receiver. A router SHOULD NOT send a prefix
                                     * option for the link-local prefix and a host SHOULD
                                     * ignore such a prefix option.*/

} ndOptionPrefix_t;

/***********************************************************************
 * Redirected header option:

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |    Length     |            Reserved           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           Reserved                            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    ~                       IP header + data                        ~
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 ***********************************************************************/
typedef struct ndOptionRedirectedHeader_tag
{
    ndOptionHeader_t optionHdr;
    uint8_t          reserved[4];
    uint8_t          ipHeaderStartByte;
} ndOptionRedirectedHeader_t;

/***********************************************************************
 * MTU option:

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |     Length    |           Reserved            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                          MTU                                  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 ***********************************************************************/
typedef struct ndOptionMtu_tag
{
    ndOptionHeader_t optionHdr;
    uint8_t   reserved[2];
    uint8_t   mtu[4];         /* The recommended MTU for the link.*/
} ndOptionMtu_t;

/***********************************************************************
 * Route Information Option:

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |    Length     | Prefix Length |Resvd|Prf|Resvd|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                        Route Lifetime                         |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                   Prefix (Variable Length)                    |
    .                                                               .
    .                                                               .
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 ***********************************************************************/
typedef struct ndOptionRouteInfo_tag
{
    ndOptionHeader_t optionHdr;
    uint8_t     prefixLength;
    uint8_t     prf;
    uint8_t     lifetime[4];
    ipAddr_t    prefix;
} ndOptionRouteInfo_t;

/***********************************************************************
 * Recursive DNS Server Option:

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |     Length    |           Reserved            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           Lifetime                            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    :            Addresses of IPv6 Recursive DNS Servers            :
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 ***********************************************************************/
typedef struct ndOptionRDNSS_tag
{
    ndOptionHeader_t optionHdr;
    uint8_t   reserved[2];
    uint8_t   lifetime[4];
    ipAddr_t  *pServers;
} ndOptionRDNSS_t;

/***********************************************************************
 * DNS Search List Option:

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |     Length    |           Reserved            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           Lifetime                            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    :                Domain Names of DNS Search List                :
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 ***********************************************************************/
typedef struct ndOptionDNSSL_tag
{
    ndOptionHeader_t optionHdr;
    uint8_t   reserved[2];
    uint8_t   lifetime[4];
    char      *pDomainNames;
} ndOptionDNSSL_t;

/***********************************************************************
 * Address Registration option:

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |     Type      |   Length = 2  |    Status     |   Reserved    |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |           Reserved            |     Registration Lifetime     |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                                                               |
     +                            EUI-64                             +
     |                                                               |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 ***********************************************************************/
typedef struct ndOptionAro_tag
{
    ndOptionHeader_t  optionHdr;
    uint8_t           status;
    uint8_t           reserved1;
    uint8_t           reserved2[2];
    uint8_t           registrationLifetime[2];
    uint8_t           eui64[8];
} ndOptionAr_t;

/***********************************************************************
 * 6LoWPAN Context option:

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |     Type      |     Length    |Context Length | Res |C|  CID  |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |            Reserved           |         Valid Lifetime        |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     .                                                               .
     .                       Context Prefix                          .
     .                                                               .
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 ***********************************************************************/
typedef struct ndOptionContext_tag
{
    ndOptionHeader_t  optionHdr;
    uint8_t           contextLen;
    uint8_t           cAndCid;
    uint8_t           reserved[2];
    uint8_t           validLifetime[2];
    uint8_t           contextPrefix[16];
} ndOptionContext_t;

/***********************************************************************
 * Authoritative Border Router option:

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |     Type      |  Length = 3   |          Version Low          |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |          Version High         |        Valid Lifetime         |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                                                               |
     +                                                               +
     |                                                               |
     +                          6LBR Address                         +
     |                                                               |
     +                                                               +
     |                                                               |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 ***********************************************************************/
typedef struct ndOptionAbro_tag
{
    ndOptionHeader_t  optionHdr;
    uint8_t           versionLow[2];
    uint8_t           versionHigh[2];
    uint8_t           validLifetime[2];
    uint8_t           lbrAddress[16];
} ndOptionAbro_t;

typedef bool_t (*ndProxyCb_t)(ifHandle_t ifHandle, ipAddr_t *pTargetAddr, uint8_t flags);

/*! ND module interface */
typedef struct ndIf_tag
{
    llAddr_t *(*ndResolveDstLinkLayerAddr)(ipPktInfo_t *pIpPktInfo);
    void (*ndDadStart)(ifHandle_t ifHandle, ip6IfAddrData_t **ppIfAddr);
    void (*ndLinkStateChanged)(ifHandle_t ifHandle, uint8_t newState);
    ndProxyCb_t  pfNdProxyCb;
} ndIf_t, * ndIfPtr;

/*! ND constants structure kept per interface */
typedef struct ndConstants_tag
{
    uint8_t  maxInitialRtrAdvInterval;
    uint8_t  maxInitialRtrAdv;
    uint8_t  maxFinalRtrAdv;
    uint8_t  minDelayBetweenRas;

    uint16_t maxRaDelayTime;
    uint8_t  maxRtrSolicitationDelay;
    uint8_t  rtrSolicitationInterval;

    uint8_t  maxRtrSolicitations;
    uint8_t  maxMulticastSolicit;
    uint8_t  maxUnicastSolicit;
    uint8_t  maxAnycastDelayTime;

    uint8_t  maxNeighborAdvertisement;
    uint8_t  reachableTime;
    uint8_t  retransTime;
    uint8_t  delayFirstProbeTime;

    uint16_t minRandomFactor;
    uint16_t maxRandomFactor;

    uint16_t minContextChangeDelay;
    uint16_t tentativeNceLifetime;

    uint16_t multihopHopLimit;
    uint16_t maxRtrSolicitationInterval;
} ndConstants_t;

/*! ND variables default values structure kept per interface */
typedef struct ndVariablesDefaults_tag
{
    uint32_t regLifetime;
    uint16_t linkMTU;
    uint8_t  hopLimit;
    uint8_t  managedConfig;
} ndVariablesDefaults_t;

/*! ND PIB structure */
typedef struct ndPib_tag
{
    uint16_t messages;
    uint16_t options;
    uint16_t tables;
    uint16_t behaviors;
    ndConstants_t constants;
    ndVariablesDefaults_t variablesDefaults;
    uint8_t devType;
} ndPib_t;

typedef enum nd6DevType_tag
{
    gNd6DevBorderRouter_c,
    gNd6DevRouter_c,
    gNd6DevHost_c,
} nd6DevType_t;

typedef enum nd6LinkState_tag
{
    gNd6LinkDown,
    gNd6LinkUp
} nd6LinkState_t;

/*! Neighbor Discovery Configuration */
typedef struct ndCfg_tag
{
    ndIf_t             *ndIf;                   /*!< Pointer to ND6 interface functions.*/

    uint32_t            rdTimestamp;            /*!< Time of last RS transmit (in seconds).*/

    uint32_t            reachableTime;          /*!< The time, in seconds,
                                                 * that a node assumes a neighbor is
                                                 * reachable after having received a reachability
                                                 * confirmation. Used by the Neighbor Unreachability
                                                   Detection algorithm.*/

    uint32_t            retransTimer;           /*!< The time, in seconds,
                                                 * between retransmitted Neighbor
                                                 * Solicitation messages. Used by address resolution
                                                 * and the Neighbor Unreachability Detection algorithm
                                                 * (see Sections 7.2 and 7.3).*/

    /* Interface variables */
    uint16_t            mtu;                    /*!< The recommended MTU for the link.
                                                 * Updated by RA messages.*/

    uint8_t             curHopLimit;            /*!< The default value that
                                                 * should be placed in the Hop Count field of the IP
                                                 * header for outgoing IP packets.*/

    bool_t              ip6Disabled;            /*!< IP operation on the interface is disabled.*/

    /* Router Discovery variables. */
    uint16_t            rdTransmitCounter;      /*!< Counter used by RD. Equals to the number
                                                 * of RS transmits till RD is finished. */

    bool_t              bManagedConfig;         /*!< Flag that indicates if the configuration is managed */

    bool_t              bOtherConfig;           /*!< Flag that indicates if Other configuration options are available */

#if ND_OPT_ABRO_RCV_ENABLED | ND_OPT_ABRO_SEND_ENABLED

    uint16_t            brVersionHigh;          /*!< Version number received from the Border Router */

    uint16_t            brVersionLow;           /*!< Version number received from the Border Router */

#endif

#if ND_OPT_AR_RCV_ENABLED

    uint32_t            regLifetime;            /*!< The lifetime of the registration to a Router */

#endif

#if ND_MSG_RA_ENABLED

    uint32_t            routerLifetime;         /*!< Used when sending RAs if the device can act as a
                                                   default router */
    routePrefFlags_e    defaultRoutePref;

#endif

    const ndPib_t      *pNdPib;                 /*!< Pointer to the ND PIB */
} ndCfg_t, * nd6CfgPtr_t;

typedef struct ndRuntimeVariables_tag
{
    uint8_t  ifIndex;
    uint8_t  nbOfAdv;
    uint16_t remainingAdvSec;
} ndRuntimeVariables_t;


/*==================================================================================================
Public global variables declarations
==================================================================================================*/



/*==================================================================================================
Public function prototypes
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#if IP_IP6_ENABLE

/*!*************************************************************************************************
\fn     void ND_Init(taskMsgQueue_t *pTaskMsgQueue)
\brief  Interface function for the Neighbor Discovery module. It initializes the module.

\param  [in]    pTaskMsgQueue   Pointer to the message queue
***************************************************************************************************/
void ND_Init(taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     bool_t ND_Open(ipIfUniqueId_t ipIfId, ndPib_t *pNdPib)
\brief  Interface function for the Neighbor Discovery module. It associates an ND configuration with
        an interface.

\param  [in]    ipIfId        Media interface id
\param  [in]    pNdPib        Pointer to the static const structure which holds the ND PIB

\return         bool_t        TRUE if module is started, FALSE if function fails.
***************************************************************************************************/
bool_t ND_Open(ipIfUniqueId_t ipIfId, ndPib_t *pNdPib);

#if ND_CLOSE_FEATURES_ENABLED

/*!*************************************************************************************************
\fn     bool_t ND_Close(ifHandle_t ifHandle)
\brief  Interface function for the Neighbor Discovery module. It shuts down the ND protocol on
        this particular media interface.

\param  [in]    ifHandle      Pointer to media interface configuration structure

\return         bool_t        TRUE if module is closed, FALSE if function fails.
***************************************************************************************************/
bool_t ND_Close(ifHandle_t ifHandle);

#endif /* ND_CLOSE_FEATURES_ENABLED */

/*!*************************************************************************************************
\fn     ndCfg_t **ND_GetCfg(ipIfUniqueId_t ipIfId)
\brief  Interface function for the Neighbor Discovery module. It returns a double pointer to the ND
        configuration.

\param  [in]    ipIfId          media interface id

\return         ndCfg_t**       Double pointer to ND configuration structure
***************************************************************************************************/
ndCfg_t **ND_GetCfg(ipIfUniqueId_t ipIfId);

/*!*************************************************************************************************
\fn     ND_SetProxyCb(ipIfUniqueId_t ipIfId, ndProxyCb_t pfNdProxyCb)
\brief  Interface function for the Neighbor Discovery module. It sets callback function for ND Proxy

\param  [in]    ipIfId          media interface id
\param  [in]    pfNdProxyCb     ND Proxy Callback
***************************************************************************************************/
void ND_SetProxyCb(ipIfUniqueId_t ipIfId, ndProxyCb_t pfNdProxyCb);

/*!*************************************************************************************************
\fn     void ND_MsgRaSend(ifHandle_t ifHandle, ipAddr_t *pDevIpAddr)
\brief  Interface function for the Neighbor Discovery module. It transmits a Router Advertisement
        message.

\param  [in]    ifHandle        Pointer to media interface configuration structure
\param  [in]    pDevIpAddr      Pointer to the RA destination ip address
***************************************************************************************************/
void ND_MsgRaSend(ifHandle_t ifHandle, ipAddr_t *pDevIpAddr);

/*!*************************************************************************************************
\fn     void ND_MsgNsSend(ifHandle_t ifHandle, ipAddr_t *pSrcIpAddr, ipAddr_t *pDstIpAddr,
                                          ipAddr_t *pTargetAddr)
\brief  Interface function for the Neighbor Discovery module. It transmits a Neighbor
        Solicitation message.

\param  [in]    ifHandle      Pointer to media interface configuration structure (mustn't be NULL)
\param  [in]    pSrcIpAddr    Pointer to the IPv6 source address of the NS (NULL for DAD)
\param  [in]    pDstIpAddr    Pointer to the IPv6 destination address of the NS
                              (set for NUD,  NULL for DAD & AR)
\param  [in]    pTargetAddr   Pointer to the target address field from the NS message (mustn't be NULL)
***************************************************************************************************/
void ND_MsgNsSend(ifHandle_t ifHandle, ipAddr_t *pSrcIpAddr, ipAddr_t *pDstIpAddr,
                  ipAddr_t *pTargetAddr);

/*!*************************************************************************************************
\fn     void ND_MsgNaSend(ifHandle_t ifHandle, ipAddr_t *pTargetIpAddr, ipAddr_t *pDstIpAddr,
                       uint8_t flags, uint32_t aroStatus, ndOptionAr_t *pOptAr, uint8_t macAddrIndex)
\brief  This function transmits an NA message.

\param  [in]   ifHandle         Pointer to media interface structure handle
\param  [in]   pTargetIpAddr    Pointer to the IPv6 target address of the NA
\param  [in]   pDstIpAddr       Pointer to the IPv6 destination address of the NA
\param  [in]   flags            NA flags
\param  [in]   aroStatus        Status of the ARO option
\param  [in]   pOptAr           Pointer to nd packet options
\param  [in]   macAddrIndex     Mac address index in interface struct to be used for TLLA option
***************************************************************************************************/
void ND_MsgNaSend(ifHandle_t ifHandle, ipAddr_t *pTargetIpAddr, ipAddr_t *pDstIpAddr,
                     uint8_t flags, uint32_t aroStatus, ndOptionAr_t *pOptAr, uint8_t macAddrIndex);

/*!*************************************************************************************************
\fn     void ND_MsgRsSend(ifHandle_t ifHandle, ipAddr_t * pRouterAddr)
\brief  Interface function for the Neighbor Discovery module. It transmits a Router Solicitation
        message.

\param  [in]    ifHandle     Pointer to media interface configuration structure
\param  [in]    pRouterAddr   Pointer to the router to which the solicitation is being sent
***************************************************************************************************/
void ND_MsgRsSend(ifHandle_t ifHandle, ipAddr_t *pRouterAddr);

/*!*************************************************************************************************
\fn     void ND_MsgNaProcess(ifHandle_t ifHandle, ipAddr_t* pIpTargetAddr,
                            uint32_t targetLinkLayerAddrSize,uint8_t* pTargetLinkLayerAddr,
                            bool_t routerFlag, bool_t solicitedFlag, bool_t overrideFlag)
\brief  This function processes an NA message.

\param [in]   ifHandle                 Pointer to media interface configuration structure
\param [in]   pIpTargetAddr             IPv6 target address
\param [in]   targetLinkLayerAddrSize   Target link layer address size
\param [in]   pTargetLinkLayerAddr      Pointer to the target link layer address or NULL (NULL is sent
                                        when TLLA was not received)
\param [in]   routerFlag                Router flag
\param [in]   solicitedFlag             Solicited flag
\param [in]   overrideFlag              Override flag
***************************************************************************************************/
void ND_MsgNaProcess(ifHandle_t ifHandle, ipAddr_t *pIpTargetAddr, uint32_t targetLinkLayerAddrSize,
                     uint8_t *pTargetLinkLayerAddr, bool_t routerFlag, bool_t solicitedFlag,
                     bool_t overrideFlag);

/*!*************************************************************************************************
\fn     void *ND_GetEui64(ifHandle_t ifHandle)
\brief  This function gets the EUI 64.

\param  [in]    ifHandle   Pointer to media interface configuration structure

\return         void*       EUI-64 address
***************************************************************************************************/
void *ND_GetEui64(ifHandle_t ifHandle);

/*!*************************************************************************************************
\fn     void ND_TimerHandler(uint32_t timerPeriodInSec)
\brief  ND periodical timer handler.

\param  [in]    timerPeriodInSec      The interval the timer is triggered.
***************************************************************************************************/
void ND_TimerHandler(uint32_t timerPeriodInSec);

/*!*************************************************************************************************
\fn     uint32_t ND_GetHopLimit(ifHandle_t ifHandle)
\brief  This function gets the default hop limit.

\param  [in]  ifHandle  Pointer to media interface configuration structure

\return       uint32_t   hop limit
***************************************************************************************************/
uint32_t ND_GetHopLimit(ifHandle_t ifHandle);

/*!*************************************************************************************************
\fn     uint32_t ND_GetMtu(ifHandle_t ifHandle)
\brief  This function gets the default MTU.

\param  [in]  pIfHandle     Pointer to media interface configuration structure

\return       uint32_t      MTU
***************************************************************************************************/
uint32_t ND_GetMtu(ifHandle_t ifHandle);

#endif /* IPv6_ENABLE */

#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /* _ND_H */
