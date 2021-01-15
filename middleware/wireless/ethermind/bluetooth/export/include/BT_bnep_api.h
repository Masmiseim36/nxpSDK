
/**
 *  \file BT_bnep_api.h
 *
 *  \brief EtherMind BNEP API Header File
 *
 *  This header file contains declaration of BNEP APIs, various Macros and
 *  Constant Definitions for use in upper layer profiles/applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_BNEP_API_
#define _H_BT_BNEP_API_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* --------------------------------------------- Global Definitions */
/** Invalid Connection Identifier */
#define BNEP_CONNECTION_ID_INVALID              0xFF


/** BNEP - PAN Profile Service Bitmap Values */
#define BNEP_SERVICE_PANU                       0x01
#define BNEP_SERVICE_NAP                        0x02
#define BNEP_SERVICE_GN                         0x04
#define BNEP_SERVICE_NONE                       0x00
#define BNEP_SERVICE_ALL                        0x07


/** BNEP Event Indication Callback Event Types */
#define BNEP_CONNECT_IND                        0x01
#define BNEP_CONNECT_CNF                        0x02
#define BNEP_SETUP_IND                          0x03
#define BNEP_SETUP_CNF                          0x04
#define BNEP_DISCONNECT_IND                     0x05
#define BNEP_DISCONNECT_CNF                     0x06
#define BNEP_FILTER_IND                         0x07
#define BNEP_FILTER_CNF                         0x08
#define BNEP_WRITE_CNF                          0x09


/* BNEP Setup Connection Response Codes */
#define BNEP_SETUP_SUCCESSFUL                   0x0000
#define BNEP_SETUP_INVALID_DEST_UUID            0x0001
#define BNEP_SETUP_INVALID_SRC_UUID             0x0002
#define BNEP_SETUP_INVALID_UUID_SIZE            0x0003
#define BNEP_SETUP_NOT_ALLOWED                  0x0004


/* BNEP Network Protocol Type Filter Response Codes */
#define BNEP_FILTER_NW_TYPE_SUCCESSFUL          0x0000
#define BNEP_FILTER_NW_TYPE_UNSUPPORTED         0x0001
#define BNEP_FILTER_NW_TYPE_INVALID_RANGE       0x0002
#define BNEP_FILTER_NW_TYPE_LIMIT_REACHED       0x0003
#define BNEP_FILTER_NW_TYPE_SECURITY_BLOCK      0x0004


/* BNEP Multicast Address Filter Response Codes */
#define BNEP_FILTER_MC_ADDR_SUCCESSFUL          0x0000
#define BNEP_FILTER_MC_ADDR_UNSUPPORTED         0x0001
#define BNEP_FILTER_MC_ADDR_INVALID_RANGE       0x0002
#define BNEP_FILTER_MC_ADDR_LIMIT_REACHED       0x0003
#define BNEP_FILTER_MC_ADDR_SECURITY_BLOCK      0x0004


/** BNEP Filtering Type - NW Type or MC Address */
#define BNEP_FILTER_TYPE_NW_TYPE                0x01
#define BNEP_FILTER_TYPE_MC_ADDR                0x02


/** BNEP Filtering Type - Local or Remote */
#define BNEP_FILTER_TYPE_INVALID                0x00
#define BNEP_FILTER_TYPE_LOCAL                  0x01
#define BNEP_FILTER_TYPE_REMOTE                 0x02


/** Size of BNEP Network Type Filter Range */
#define BNEP_NW_TYPE_RANGE_SIZE                 4


/** Size of BNEP Multicast Address Filter Range */
#define BNEP_MC_ADDR_RANGE_SIZE                 12


/** The Ethernet 802.1Q (also known as IEEE 802.1p) Protocol Type Value */
#define BNEP_ETH_PROTO_TYPE_802_1Q              0x8100


/** Ethernet Source & Destination Address Size */
#define BNEP_ETH_ADDR_SIZE                      BT_BD_ADDR_SIZE


/** Ethernet Protocol Type/Length Field Size */
#define BNEP_ETH_PROTO_LEN_SIZE                 2


/** Ethernet Header Size */
#define BNEP_ETH_HEADER_SIZE \
        ((2 * BNEP_ETH_ADDR_SIZE) + BNEP_ETH_PROTO_LEN_SIZE)


/** Number of Octets occupied by Ethernet 802.1Q Tag */
#define BNEP_ETH_PROTO_802_1Q_HEADER_LEN        4


/** BNEP Policy Settings for handling 'Setup' Requests */
#define BNEP_SETUP_POLICY_ACCEPT                0x00
#define BNEP_SETUP_POLICY_REJECT                0x01
#define BNEP_SETUP_POLICY_ASK                   0x02
#define BNEP_SETUP_POLICY_IGNORE                0x03
#define BNEP_SETUP_POLICY_DEFAULT               BNEP_SETUP_POLICY_ACCEPT


/** BNEP Default Policy Settings */
#define BNEP_POLICY_DEFAULTS                    BNEP_SETUP_POLICY_DEFAULT


/* --------------------------------------------- Structures/Data Types */
/** BNEP Event Indication Callback */
typedef API_RESULT (* BNEP_EVENT_IND_CB)
                   (
                       UCHAR      /* Event Identifier */,
                       UCHAR *    /* Remote BD_ADDR */,
                       UINT16     /* Result/Response */,
                       void *     /* Event Data */,
                       UINT16     /* Event Data Length */
                   );


/** BNEP Data Read Indication Callback */
typedef API_RESULT (* BNEP_DATA_IND_CB)
                   (
                       UCHAR *    /* Remote BD_ADDR */,
                       UCHAR *    /* Ethernet Header (14 Octets) */,
                       UCHAR *    /* Ethernet Payload */,
                       UINT16     /* Ethernet Payload Length */
                   );


/** BNEP Callbacks for Registration */
typedef struct
{
    /* BNEP Event Indication/Confirmation Callback */
    BNEP_EVENT_IND_CB    bnep_event_ind;

    /* BNEP Data Received Indication */
    BNEP_DATA_IND_CB     bnep_read_ind;

} BNEP_CALLBACKS;


/** BNEP Filter - Network Type */
typedef struct
{
    /* Ethernet Network Type Start & End */
    UINT16    start;
    UINT16    end;

} BNEP_FILTER_NW_TYPE;


/** BNEP Filter - Multicast Address */
typedef struct
{
    /* Ethernet Multicast Address Start & End */
    UCHAR    start [BNEP_ETH_ADDR_SIZE];
    UCHAR    end [BNEP_ETH_ADDR_SIZE];

} BNEP_FILTER_MC_ADDR;


/** BNEP Filter Parameters (delivered with BNEP_EVENT_IND_CB) */
typedef struct
{
    /* Type of BNEP Filter - Network Type or Multicast Address */
    UCHAR      type;

    /* Number of Filter Ranges */
    UINT16     num_range;

    /* Data Buffer containing the Filtering Ranges */
    void *     filter;

} BNEP_FILTER;


/** BNEP Setup Parameters (delivered with BNEP_SETUP_IND/BNEP_SETUP_CNF) */
typedef struct
{
    /* Configured Local/Remote PAN Services */
    UCHAR    local_service;
    UCHAR    remote_service;

    /*
     *  Flag to indicate what action needs to be taken. Flag is valid
     *  only when the Event Type is 'BNEP_SETUP_IND'.
     *
     *    0x00 - BNEP 'Setup' accepted automatically
     *    0x01 - Response pending. Call BT_bnep_setup_rsp() API.
     */
    UCHAR    flag;

} BNEP_SETUP;


/** BNEP Extension header structure */
typedef struct
{
    /* Extension Type */
    UCHAR type;

    /* Extension Length */
    UCHAR length;

    /* Extension Value */
    UCHAR * value;

} BNEP_EXT_HEADER;

/* --------------------------------------------- Unions */


/* --------------------------------------------- Macros */
/**
 *  To convert a Bluetooth Device Address (BD_ADDR, as represented within
 *  EtherMind Protocol Stack) to Ethernet Address, and vice-versa
 */
#define bnep_convert_address(addr_to, addr_from) \
        (addr_to)[5] = (addr_from)[0]; \
        (addr_to)[4] = (addr_from)[1]; \
        (addr_to)[3] = (addr_from)[2]; \
        (addr_to)[2] = (addr_from)[3]; \
        (addr_to)[1] = (addr_from)[4]; \
        (addr_to)[0] = (addr_from)[5];


/**
 *  To Copy Ethernet Addresses from one place to other.
 *  Since Ethernet Source/Destination addresses are of same
 *  size as that of Bluetooth Device Addresses (BD_ADDR), the
 *  BT_COPY_BD_ADDR macro can be used.
 */
#define BNEP_COPY_ETH_ADDR              BT_COPY_BD_ADDR


/* --------------------------------------------- API Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/** Registration of Callbacks to BNEP */
API_RESULT BT_bnep_register
           (
               BNEP_CALLBACKS *    /* BNEP Callbacks */
           );

/** Start Accepting Connection for BNEP (PAN) Services */
API_RESULT BT_bnep_accept
           (
               UCHAR      /* Local Service Bitmap */
           );

/** To Open a BNEP (L2CAP) Connection to a BD_ADDR */
API_RESULT BT_bnep_connect
           (
               UCHAR *    /* Remote BD_ADDR */
           );

/** To Setup a BNEP Connection */
API_RESULT BT_bnep_setup
           (
               UCHAR *    /* Remote BD_ADDR */,
               UCHAR      /* Local Service */,
               UCHAR      /* Remote Service */
           );

/** To Close a BNEP (L2CAP) Connection to a BD_ADDR */
API_RESULT BT_bnep_disconnect
           (
               UCHAR *    /* Remote BD_ADDR */
           );

/** To Set a Filter, Local or Remote, for a BNEP Connection */
API_RESULT BT_bnep_filter
           (
               UCHAR *    /* Remote BD_ADDR */,
               UCHAR      /* Local or Remote Filter */,
               UCHAR      /* Filter Type - NW Type or MC Address */,
               UINT16     /* Number of Filter Ranges */,
               void *     /* Filter Ranges */
           );

/** To Write Data for Transmission on a BNEP Connection */
API_RESULT BT_bnep_write
           (
               UCHAR *              /* Remote BD_ADDR */,
               UCHAR *              /* Ethernet Header (14 Octets) */,
               BNEP_EXT_HEADER *    /* Extension Header */,
               UINT16               /* Number of Extension headers */,
               UCHAR *              /* Ethernet Payload */,
               UINT16               /* Ethernet Payload Length */
           );

/** To Respond to BNEP_SETUP_IND for 'Setup' Request from Peer */
API_RESULT BT_bnep_setup_rsp
           (
               UCHAR *    /* Remote BD_ADDR */,
               UINT16     /* Result/Response */
           );

/**
 *   To Set Policy for a BNEP Connection
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
API_RESULT BT_bnep_set_policy
           (
               UCHAR *    /* Remote BD_ADDR */,
               UCHAR      /* Flag */
           );

/** To enable or disable header compression during transmission */
void BT_bnep_set_compression_state (UCHAR state);

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_BNEP_API_ */

