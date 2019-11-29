/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _SIXLOWPAN_IB_H
#define _SIXLOWPAN_IB_H
/*!=================================================================================================
\file       sixlowpan_ib.h
\brief      This is a header file for the ADP module in which the ADP Information base attributes
            are defined.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "network_utils.h"
#include "mac_abs_802154.h"

/*==================================================================================================
Public macros
==================================================================================================*/

#ifndef SLWPCFG_SERVER_ENABLED
    #define SLWPCFG_SERVER_ENABLED 1
#endif

#ifndef SLWPCFG_NODE_ENABLED
    #define SLWPCFG_NODE_ENABLED 1
#endif

#ifndef SLWPCFG_MESH_ENABLED
   #define SLWPCFG_MESH_ENABLED 1
#endif

#ifndef SLWPCFG_FRAGMENTATION_ENABLED
   #define SLWPCFG_FRAGMENTATION_ENABLED 1
#endif

#ifndef SLWPCFG_HEADER_COMPRESSION_ENABLED
   #define SLWPCFG_HEADER_COMPRESSION_ENABLED 1
#endif

#if (SLWPCFG_HEADER_COMPRESSION_ENABLED)
    #ifndef SLWPCFG_RFC6282_COMPRESSION_ENABLED
        #define SLWPCFG_RFC6282_COMPRESSION_ENABLED 1
    #endif

    #ifndef SLWPCFG_RFC4944_COMPRESSION_ENABLED
        #define SLWPCFG_RFC4944_COMPRESSION_ENABLED 1
    #endif
#else
    #define SLWPCFG_RFC4944_COMPRESSION_ENABLED 0
    #define SLWPCFG_RFC6282_COMPRESSION_ENABLED 0
#endif

#ifndef SLWPCFG_BSTRAP_ENABLED
    #define SLWPCFG_BSTRAP_ENABLED  0
#endif

#if (SLWPCFG_BSTRAP_ENABLED)
    #ifndef SLWPCFG_LBP_ENABLED
        #define SLWPCFG_LBP_ENABLED 1
    #endif
#else
    #define SLWPCFG_LBP_ENABLED 0
#endif

#ifndef SLWPCFG_ROUTING_ENABLED
    #define SLWPCFG_ROUTING_ENABLED 0
#endif

#if (SLWPCFG_ROUTING_ENABLED)
    #ifndef SLWPCFG_LOADNG_ENABLED
        #define SLWPCFG_LOADNG_ENABLED 0
    #endif
#else
    #define SLWPCFG_LOADNG_ENABLED 0
#endif

#ifndef SLWPCFG_RFC6282_COMPRESS_UDP_CHECKSUM
    #define SLWPCFG_RFC6282_COMPRESS_UDP_CHECKSUM 0
#endif

#ifndef SLWPCFG_RFC6282_REMOVE_PAD
    #define SLWPCFG_RFC6282_REMOVE_PAD 0
#endif

#ifndef RFC4944_DETECT_FRAG_OVERLAP_ENABLED
    #define RFC4944_DETECT_FRAG_OVERLAP_ENABLED 0
#endif

#define SLWPIB_ADP_MAX_HOPS                 14U
#define SLWPCFG_IB_GROUP_TABLE_MAX_ELEMENTS 4


/*==================================================================================================
Public type definitions
==================================================================================================*/

typedef enum adpDeviceType_tag
{
    gAdpDeviceTypeNone_c        = 0x00U,
    gAdpDeviceTypeDevice_c      = 0x01U,
    gAdpDeviceTypeServer_c      = 0x02U,
    gAdpDeviceTypeSniffer       = 0x03U,
} adpDeviceType_t;

typedef enum adpRoutingEnable_tag
{
    gAdpRoutingNone_c           = 0x00U,
    gAdpRoutingLoad_c           = 0x01U,
    gAdpRoutingLoadNg_c         = 0x02U,
} adpRoutingEnable_t;

typedef enum adpHcEnable_tag
{
    gAdpHcNone_c                = 0x00U,
    gAdpHcRFC4944_c             = 0x01U,
    gAdpHcRFC6282_c             = 0x02U,
} adpHcEnable_t;

typedef enum adpMeshEnable_tag
{
    gAdpMeshNone_c              = 0x00U,
    gAdpMeshOnly_c              = 0x01U,
    gAdpMeshBroadcast_c         = 0x02U,
} adpMeshEnable_t;

typedef enum adpBStrapEnable_tag
{
    gAdpBStrapNone_c            = 0x00U,
    gAdpBStrapLbp_c             = 0x01U
} adpBStrapEnable_t;

typedef enum adpIIDType_tag
{
    gAdpIIDWithPanId_c          = 0x00U,
    gAdpIIDWithoutPanId_c       = 0x01U
} adpIIDType_t;

typedef struct adpMeshIb_tag
{
    uint8_t             adpMaxHops;
    uint8_t             adpBcastSeqNum;
    uint16_t            adpBroadcastLogTableEntryTTL;
    void **             adpMeshRouteTbl;
    void **             adpMCastGroupTbl;
    //uint16_t            adpGroupTable[SLWPCFG_IB_GROUP_TABLE_MAX_ELEMENTS];
} adpMeshIb_t;

/*! ND module interface */
typedef struct adpFwdFct_tag
{
    void** (*init) (uint32_t instanceNb);
    bool_t (*isMeshNeeded)(void * param);
    llAddr_t (*ucastGetNextHop)(llAddr_t* myAddr, llAddr_t* finalAddr, llAddr_t* pMacSrcAddr, instanceId_t instanceId);
    void (*mcastForward)(macAbsMcpsDataInd_t * pMcpsDataInd);
} adpFwdFct_t;


typedef struct adpLoadNgIb_tag
{
    uint32_t            adpRREQRetries;
    uint32_t            adpRREQRERRWait;
    uint32_t            adpNetTraversalTime;
    uint16_t            adpPathDiscoveryTime;
    uint16_t            adpRoutingTableEntryTTL;
    uint16_t            adpBlacklistedNeighborSetTTL;
    uint8_t             adpUnicastRREQGenEnable;
    uint8_t             adpCustomAggregateRouteReplies;
    uint8_t             adpWeakLQIValue;
    uint8_t             adpKr;
    uint8_t             adpKm;
    uint8_t             adpKc;
    uint8_t             adpKq;
    uint8_t             adpKh;
    uint8_t             adpKrt;
    uint8_t             adpBlacklistedNeighborSet[16];
    uint8_t             adpCustomPrefixTable[8];
    uint8_t             adpContextInformationTable[16];
} adpLoadNgIb_t;

typedef struct adpLbpIb_tag
{
    uint16_t            adpUseNewGMKTime;
    uint16_t            adpExpPrecGMKTime;
    uint16_t            adpMaxJoinWaitTime;
    uint16_t            adpDiscoveryAttemptsSpeed;
    uint8_t             adpMaxDiscoveryPerHour;
    uint8_t             adpNumDiscoveryAttempts;
} adpLbpIb_t;

/*! Structure which holds the 6LoWPAN information base attributes */
typedef struct adpIb_tag
{
    adpDeviceType_t     adpDeviceType;          /* select device type */
    adpHcEnable_t       adpHcEnable;            /* enable Header Compression */
    adpIIDType_t        adpIIDType;             /* use PanId to create IPv6 address IID */

    uint8_t             adpSecurityLevel;       /* maximum MAC security level to accept
                                                   incomming frames and the security level
                                                   used to transmit outgoing frames */
    uint8_t             adpActiveKeyIndex;      /* current MAC security Key Index used
                                                   to transmit outgoing frames. Index not
                                                   needed for receiving frames because once
                                                   a key is stored in MAC it will immediately
                                                   be available to receive frames encoded with
                                                   that key */
    uint8_t             msduHandle;             /* msduHandle that is passed to the MAC */

    uint8_t             sedTrackTblEntries;     /* number of unique SED devices a router can handle */
    uint8_t             sedTrackTblPktNb;       /* number of packets from a unique SED device that 
                                                   a router can handle */
    uint8_t             sedIndQueueSize;        /* number of fragmented indirect packets that 
                                                   stored for transmission */

#if (SLWPCFG_FRAGMENTATION_ENABLED)
    uint16_t            datagramTag;
#endif

#if (SLWPCFG_MESH_ENABLED)
    adpMeshEnable_t     adpMeshEnable;          /* enable Mesh & BCast Header */
    adpMeshIb_t         adpMeshIb;              /* Mesh & BCast related Ibs */
    adpFwdFct_t         adpFwdIb;               /* Function pointers for Forwarding */
    void *              adpRouteTbl;            /* Pointer to mesh routing table */
#endif

#if (SLWPCFG_BSTRAP_ENABLED)
    adpBStrapEnable_t   adpBStrapEnable;        /* enable L2 bootstrapping */

    #if (SLWPCFG_LBP_ENABLED)
    adpLbpIb_t          adpLbpIb;               /* LBP related Ibs */
    #endif
#endif

#if (SLWPCFG_ROUTING_ENABLED)
    adpRoutingEnable_t  adpRoutingEnable;       /* enable routing */

#if (SLWPCFG_LOADNG_ENABLED)
    adpLoadNgIb_t       adpLoadNgIb;            /* LoadNG related Ibs */
#endif
#endif
} adpIb_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/


/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _SIXLOWPAN_IB_H */
