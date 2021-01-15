
/**
 *  \file BT_pan_api.h
 *
 *  \brief EtherMind PAN API Header File
 *
 *  This header file contains declaration of PAN APIs, various Macros and
 *  Constant Definitions for use in applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_PAN_API_
#define _H_BT_PAN_API_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_bnep_api.h"


/* --------------------------------------------- Global Definitions */
/* Maximum Number of PAN Connection Entities */
#define PAN_MAX_ENTITIES                        BNEP_MAX_ENTITIES


/* PAN Network Interface Name Length */
#define PAN_IFNAME_SIZE                         8


/** PAN Profile Service Bitmap Values */
#define PAN_SERVICE_PANU                        BNEP_SERVICE_PANU
#define PAN_SERVICE_NAP                         BNEP_SERVICE_NAP
#define PAN_SERVICE_GN                          BNEP_SERVICE_GN
#define PAN_SERVICE_NONE                        BNEP_SERVICE_NONE
#define PAN_SERVICE_ALL                         BNEP_SERVICE_ALL


/** PAN Event Indication Callback Event Types */
#define PAN_CONNECT_IND                         BNEP_CONNECT_IND
#define PAN_CONNECT_CNF                         BNEP_CONNECT_CNF
#define PAN_SETUP_IND                           BNEP_SETUP_IND
#define PAN_SETUP_CNF                           BNEP_SETUP_CNF
#define PAN_DISCONNECT_IND                      BNEP_DISCONNECT_IND
#define PAN_DISCONNECT_CNF                      BNEP_DISCONNECT_CNF
#define PAN_FILTER_IND                          BNEP_FILTER_IND
#define PAN_FILTER_CNF                          BNEP_FILTER_CNF
#define PAN_WRITE_CNF                           BNEP_WRITE_CNF


/* PAN Setup Connection Response Codes */
#define PAN_SETUP_SUCCESSFUL                    BNEP_SETUP_SUCCESSFUL
#define PAN_SETUP_INVALID_DEST_UUID             BNEP_SETUP_INVALID_DEST_UUID
#define PAN_SETUP_INVALID_SRC_UUID              BNEP_SETUP_INVALID_SRC_UUID
#define PAN_SETUP_INVALID_UUID_SIZE             BNEP_SETUP_INVALID_UUID_SIZE
#define PAN_SETUP_NOT_ALLOWED                   BNEP_SETUP_NOT_ALLOWED


/* PAN Network Protocol Type Filter Response Codes */
#define PAN_FILTER_NW_TYPE_SUCCESSFUL      BNEP_FILTER_NW_TYPE_SUCCESSFUL
#define PAN_FILTER_NW_TYPE_UNSUPPORTED     BNEP_FILTER_NW_TYPE_UNSUPPORTED
#define PAN_FILTER_NW_TYPE_INVALID_RANGE   BNEP_FILTER_NW_TYPE_INVALID_RANGE
#define PAN_FILTER_NW_TYPE_LIMIT_REACHED   BNEP_FILTER_NW_TYPE_LIMIT_REACHED
#define PAN_FILTER_NW_TYPE_SECURITY_BLOCK  BNEP_FILTER_NW_TYPE_SECURITY_BLOCK


/* PAN Multicast Address Filter Response Codes */
#define PAN_FILTER_MC_ADDR_SUCCESSFUL      BNEP_FILTER_MC_ADDR_SUCCESSFUL
#define PAN_FILTER_MC_ADDR_UNSUPPORTED     BNEP_FILTER_MC_ADDR_UNSUPPORTED
#define PAN_FILTER_MC_ADDR_INVALID_RANGE   BNEP_FILTER_MC_ADDR_INVALID_RANGE
#define PAN_FILTER_MC_ADDR_LIMIT_REACHED   BNEP_FILTER_MC_ADDR_LIMIT_REACHED
#define PAN_FILTER_MC_ADDR_SECURITY_BLOCK  BNEP_FILTER_MC_ADDR_SECURITY_BLOCK


/** PAN Filtering Type - NW Type or MC Address */
#define PAN_FILTER_TYPE_NW_TYPE                 BNEP_FILTER_TYPE_NW_TYPE
#define PAN_FILTER_TYPE_MC_ADDR                 BNEP_FILTER_TYPE_MC_ADDR


/** PAN Filtering Type - Local or Remote */
#define PAN_FILTER_TYPE_INVALID                 BNEP_FILTER_TYPE_INVALID
#define PAN_FILTER_TYPE_LOCAL                   BNEP_FILTER_TYPE_LOCAL
#define PAN_FILTER_TYPE_REMOTE                  BNEP_FILTER_TYPE_REMOTE


/** Size of PAN Network Type Filter Range */
#define PAN_NW_TYPE_RANGE_SIZE                  BNEP_NW_TYPE_RANGE_SIZE


/** Size of PAN Multicast Address Filter Range */
#define PAN_MC_ADDR_RANGE_SIZE                  BNEP_MC_ADDR_RANGE_SIZE


/** The Ethernet 802.1Q (also known as IEEE 802.1p) Protocol Type Value */
#define PAN_ETH_PROTO_TYPE_802_1Q               BNEP_ETH_PROTO_TYPE_802_1Q


/** Ethernet Source & Destination Address Size */
#define PAN_ETH_ADDR_SIZE                       BNEP_ETH_ADDR_SIZE


/** Ethernet Protocol Type/Length Field Size */
#define PAN_ETH_PROTO_LEN_SIZE                  BNEP_ETH_PROTO_LEN_SIZE


/** Ethernet Header Size */
#define PAN_ETH_HEADER_SIZE                     BNEP_ETH_HEADER_SIZE


/** Number of Octets occupied by Ethernet 802.1Q Tag */
#define PAN_ETH_PROTO_802_1Q_HEADER_LEN         BNEP_ETH_PROTO_802_1Q_HEADER_LEN


/** PAN Policy Settings for handling 'Setup' Requests */
#define PAN_SETUP_POLICY_ACCEPT                 BNEP_SETUP_POLICY_ACCEPT
#define PAN_SETUP_POLICY_REJECT                 BNEP_SETUP_POLICY_REJECT
#define PAN_SETUP_POLICY_ASK                    BNEP_SETUP_POLICY_ASK
#define PAN_SETUP_POLICY_IGNORE                 BNEP_SETUP_POLICY_IGNORE
#define PAN_SETUP_POLICY_DEFAULT                BNEP_SETUP_POLICY_DEFAULT


/** PAN Default Policy Settings */
#define PAN_POLICY_DEFAULTS                     BNEP_POLICY_DEFAULTS


/* --------------------------------------------- Structures/Data Types */
/* PAN Event Indication Callback */
typedef API_RESULT (* PAN_EVENT_IND_CB)
                   (
                       UCHAR      /* Event Identifier */,
                       UCHAR *    /* Remote BD_ADDR */,
                       UINT16     /* Result/Response */,
                       void *     /* Event Data */,
                       UINT16     /* Event Data Length */
                   );


/* PAN Data Read Indication Callback */
typedef API_RESULT (* PAN_DATA_IND_CB)
                   (
                       UCHAR      /* Local Service */,
                       UCHAR *    /* Remote BD_ADDR */,
                       UCHAR *    /* Ethernet Header (14 Octets) */,
                       UCHAR *    /* Ethernet Payload */,
                       UINT16     /* Ethernet Payload Length */
                   );


/* PAN Callbacks for Registration */
typedef struct
{
    /* PAN Event Indication/Confirmation Callback */
    PAN_EVENT_IND_CB    pan_event_ind;

    /* PAN Data Received Indication */
    PAN_DATA_IND_CB     pan_read_ind;

} PAN_CALLBACKS;


/* PAN Filter - Network Type */
typedef BNEP_FILTER_NW_TYPE     PAN_FILTER_NW_TYPE;


/* PAN Filter - Multicast Address */
typedef BNEP_FILTER_MC_ADDR     PAN_FILTER_MC_ADDR;


/* BNEP Filter Parameters (delivered with BNEP_EVENT_IND_CB) */
typedef BNEP_FILTER             PAN_FILTER;


/* BNEP Setup Parameters (delivered with BNEP_SETUP_IND/BNEP_SETUP_CNF) */
typedef BNEP_SETUP              PAN_SETUP;

/* BNEP Extension Headers */
typedef BNEP_EXT_HEADER         PAN_EXT_HEADER;


/* --------------------------------------------- Unions */


/* --------------------------------------------- Macros */
/**
 *  To convert a Bluetooth Device Address (BD_ADDR, as represented within
 *  EtherMind Protocol Stack) to Ethernet Address, and vice-versa
 */
#define pan_convert_address                     bnep_convert_address


/**
 *  To Copy Ethernet Addresses from one place to other.
 *  Since Ethernet Source/Destination addresses are of same
 *  size as that of Bluetooth Device Addresses (BD_ADDR), the
 *  BT_COPY_BD_ADDR macro can be used.
 */
#define PAN_COPY_ETH_ADDR                       BNEP_COPY_ETH_ADDR


/* --------------------------------------------- API Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/* PAN Profile Initialization */
API_RESULT BT_pan_init ( void );

/* PAN Profile Shutdown */
API_RESULT BT_pan_shutdown ( void );

/* Registration of PAN Callbacks */
API_RESULT BT_pan_register
           (
               PAN_CALLBACKS *    /* PAN Callbacks */
           );

/** Start Accepting Connection for PAN Services */
#define BT_pan_accept           BT_bnep_accept

/** To Open a PAN (BNEP/L2CAP) Connection to a BD_ADDR */
#define BT_pan_connect          BT_bnep_connect

/** To Setup a BNEP Connection */
#define BT_pan_setup            BT_bnep_setup

/** To Respond to PAN_SETUP_IND for 'Setup' Request from Peer */
API_RESULT BT_pan_setup_rsp
           (
               UCHAR *    /* Remote BD_ADDR */,
               UINT16     /* Result/Response */
           );

/** To Close a PAN (BNEP/L2CAP) Connection to a BD_ADDR */
#define BT_pan_disconnect       BT_bnep_disconnect

/** To Set a Filter, Local or Remote, for a PAN Connection */
#define BT_pan_filter           BT_bnep_filter

/** To Write Data for Transmission with Local Service as PANU */
API_RESULT BT_pan_write_panu
           (
               UCHAR *             bd_addr,
               UCHAR *             eth_header,
               PAN_EXT_HEADER *    ext_header,
               UINT16              ext_header_count,
               UCHAR *             eth_payload,
               UINT16              eth_payload_len
           );

/** To Write Data for Transmission with Local Service as NAP/GN */
API_RESULT BT_pan_write_nap_gn
           (
               UCHAR               /* Local PAN Service */,
               UCHAR *             /* Ethernet Header (14 Octets) */,
               UCHAR *             /* Ethernet Payload */,
               UINT16              /* Ethernet Payload Length */
           );

/**
 *   To Set Policy for a PAN Connection
 *
 *   Flag is a Bitmap. Description of each Bit is given below:
 *
 *   Bit 0 & 1 -> How remotely initiated Role Switching will be handled?
 *                00: Always accept (default)
 *                01: Always reject
 *                10: Ask application
 *                11: Ignore (Only in Test Mode)
 *
 *   Bit 2 to 7 -> Reserved
 */
#define BT_pan_set_policy       BT_bnep_set_policy

/** To enable/disable header compression during transmission */
#define BT_pan_set_compression_state BT_bnep_set_compression_state

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_PAN_API_ */


