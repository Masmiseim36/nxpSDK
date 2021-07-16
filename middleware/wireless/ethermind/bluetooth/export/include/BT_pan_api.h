
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
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup pan_module PAN (Personal Area Networking Profile)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Personal Area Networking Profile (PAN) module to the Application and
 *  other upper layers of the stack.
 */
/**
 * \defgroup pan_defines Defines
 * \{
 * Describes defines for the module.
 */
/**
 * \defgroup pan_constants Constants
 * \{
 * Describes Constants defined by the module.
 */


/* Maximum Number of PAN Connection Entities */
#define PAN_MAX_ENTITIES                        BNEP_MAX_ENTITIES


/* PAN Network Interface Name Length */
#define PAN_IFNAME_SIZE                         8U


/** PAN Profile Service Bitmap Values */
/**
 * @name PAN Profile Service Bitmap Values
 *
 * Constant Defines for PAN Profile Service Bitmap Values
 */
/*@{*/
#define PAN_SERVICE_PANU                        BNEP_SERVICE_PANU
#define PAN_SERVICE_NAP                         BNEP_SERVICE_NAP
#define PAN_SERVICE_GN                          BNEP_SERVICE_GN
#define PAN_SERVICE_NONE                        BNEP_SERVICE_NONE
#define PAN_SERVICE_ALL                         BNEP_SERVICE_ALL
/*@}*/

/** PAN Event Indication Callback Event Types */
/**
 * @name PAN Event Indication Callback Event Types
 *
 * Constant Defines for PAN Event Indication Callback Event Types
 */
/*@{*/
#define PAN_CONNECT_IND                         BNEP_CONNECT_IND
#define PAN_CONNECT_CNF                         BNEP_CONNECT_CNF
#define PAN_SETUP_IND                           BNEP_SETUP_IND
#define PAN_SETUP_CNF                           BNEP_SETUP_CNF
#define PAN_DISCONNECT_IND                      BNEP_DISCONNECT_IND
#define PAN_DISCONNECT_CNF                      BNEP_DISCONNECT_CNF
#define PAN_FILTER_IND                          BNEP_FILTER_IND
#define PAN_FILTER_CNF                          BNEP_FILTER_CNF
#define PAN_WRITE_CNF                           BNEP_WRITE_CNF
/*@}*/

/* PAN Setup Connection Response Codes */
/**
 * @name PAN Setup Connection Response Codes
 *
 * Constant Defines for PAN Setup Connection Response Codes
 */
/*@{*/
#define PAN_SETUP_SUCCESSFUL                    BNEP_SETUP_SUCCESSFUL
#define PAN_SETUP_INVALID_DEST_UUID             BNEP_SETUP_INVALID_DEST_UUID
#define PAN_SETUP_INVALID_SRC_UUID              BNEP_SETUP_INVALID_SRC_UUID
#define PAN_SETUP_INVALID_UUID_SIZE             BNEP_SETUP_INVALID_UUID_SIZE
#define PAN_SETUP_NOT_ALLOWED                   BNEP_SETUP_NOT_ALLOWED
/*@}*/

/* PAN Network Protocol Type Filter Response Codes */
/**
 * @name PAN Network Protocol Type Filter Response Codes
 *
 * Constant Defines for PAN Network Protocol Type Filter Response Codes
 */
/*@{*/
#define PAN_FILTER_NW_TYPE_SUCCESSFUL      BNEP_FILTER_NW_TYPE_SUCCESSFUL
#define PAN_FILTER_NW_TYPE_UNSUPPORTED     BNEP_FILTER_NW_TYPE_UNSUPPORTED
#define PAN_FILTER_NW_TYPE_INVALID_RANGE   BNEP_FILTER_NW_TYPE_INVALID_RANGE
#define PAN_FILTER_NW_TYPE_LIMIT_REACHED   BNEP_FILTER_NW_TYPE_LIMIT_REACHED
#define PAN_FILTER_NW_TYPE_SECURITY_BLOCK  BNEP_FILTER_NW_TYPE_SECURITY_BLOCK
/*@}*/

/* PAN Multicast Address Filter Response Codes */
/**
 * @name PAN Multicast Address Filter Response Codes
 *
 * Constant Defines for PAN Multicast Address Filter Response Codes
 */
/*@{*/
#define PAN_FILTER_MC_ADDR_SUCCESSFUL      BNEP_FILTER_MC_ADDR_SUCCESSFUL
#define PAN_FILTER_MC_ADDR_UNSUPPORTED     BNEP_FILTER_MC_ADDR_UNSUPPORTED
#define PAN_FILTER_MC_ADDR_INVALID_RANGE   BNEP_FILTER_MC_ADDR_INVALID_RANGE
#define PAN_FILTER_MC_ADDR_LIMIT_REACHED   BNEP_FILTER_MC_ADDR_LIMIT_REACHED
#define PAN_FILTER_MC_ADDR_SECURITY_BLOCK  BNEP_FILTER_MC_ADDR_SECURITY_BLOCK
/*@}*/

/** PAN Filtering Type - NW Type or MC Address */
/**
 * @name PAN Filtering Type - NW Type or MC Address
 *
 * Constant Defines for PAN Filtering Type - NW Type or MC Address
 */
/*@{*/
#define PAN_FILTER_TYPE_NW_TYPE                 BNEP_FILTER_TYPE_NW_TYPE
#define PAN_FILTER_TYPE_MC_ADDR                 BNEP_FILTER_TYPE_MC_ADDR
/*@}*/

/** PAN Filtering Type - Local or Remote */
/**
 * @name PAN Filtering Type - Local or Remote
 *
 * Constant Defines for PAN Filtering Type - Local or Remote
 */
/*@{*/
#define PAN_FILTER_TYPE_INVALID                 BNEP_FILTER_TYPE_INVALID
#define PAN_FILTER_TYPE_LOCAL                   BNEP_FILTER_TYPE_LOCAL
#define PAN_FILTER_TYPE_REMOTE                  BNEP_FILTER_TYPE_REMOTE
/*@}*/

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
/**
 * @name PAN Policy Settings for handling 'Setup' Requests
 *
 * Constant Defines for PAN Policy Settings for handling 'Setup' Requests
 */
/*@{*/
#define PAN_SETUP_POLICY_ACCEPT                 BNEP_SETUP_POLICY_ACCEPT
#define PAN_SETUP_POLICY_REJECT                 BNEP_SETUP_POLICY_REJECT
#define PAN_SETUP_POLICY_ASK                    BNEP_SETUP_POLICY_ASK
#define PAN_SETUP_POLICY_IGNORE                 BNEP_SETUP_POLICY_IGNORE
#define PAN_SETUP_POLICY_DEFAULT                BNEP_SETUP_POLICY_DEFAULT
/*@}*/

/** PAN Default Policy Settings */
#define PAN_POLICY_DEFAULTS                     BNEP_POLICY_DEFAULTS

/** \} */
/** \} */
/* --------------------------------------------- Structures/Data Types */

/**
 * \defgroup pan_appl_callback Application Callback
 * \{
 * Describes Application Callback defined by the module.
 */

/* PAN Event Indication Callback */
typedef API_RESULT (* PAN_EVENT_IND_CB)
                   (
                       UCHAR      event_type,    /* Event Identifier */
                       UCHAR *    bd_addr,       /* Remote BD_ADDR */
                       UINT16     event_result,  /* Result/Response */
                       void *     event_data,    /* Event Data */
                       UINT16     event_datalen  /* Event Data Length */
                   );

/* PAN Data Read Indication Callback */
typedef API_RESULT (* PAN_DATA_IND_CB)
                   (
                       UCHAR      local_service,   /* Local Service */
                       UCHAR *    bd_addr,         /* Remote BD_ADDR */
                       UCHAR *    eth_header,      /* Ethernet Header (14 Octets) */
                       UCHAR *    eth_payload,     /* Ethernet Payload */
                       UINT16     eth_payload_len  /* Ethernet Payload Length */
                   );

/** PAN Callbacks for Registration */
typedef struct
{
    /** PAN Event Indication/Confirmation Callback */
    PAN_EVENT_IND_CB    pan_event_ind;

    /** PAN Data Received Indication */
    PAN_DATA_IND_CB     pan_read_ind;

} PAN_CALLBACKS;
/** \} */
/**
 * \addtogroup pan_defines Defines
 * \{
 */
/**
 * \defgroup pan_structures Structures
 * \{
 * Describes Structures defined by the module.
 */

/** PAN Filter - Network Type */
typedef BNEP_FILTER_NW_TYPE     PAN_FILTER_NW_TYPE;


/** PAN Filter - Multicast Address */
typedef BNEP_FILTER_MC_ADDR     PAN_FILTER_MC_ADDR;


/** BNEP Filter Parameters (delivered with BNEP_EVENT_IND_CB) */
typedef BNEP_FILTER             PAN_FILTER;


/** BNEP Setup Parameters (delivered with BNEP_SETUP_IND/BNEP_SETUP_CNF) */
typedef BNEP_SETUP              PAN_SETUP;

/** BNEP Extension Headers */
typedef BNEP_EXT_HEADER         PAN_EXT_HEADER;

/** \} */
/** \} */
/* --------------------------------------------- Unions */


/* --------------------------------------------- Macros */
/**
 * \defgroup pan_utility_macros Utility Macros
 * \{
 * Describes Utility Macros defined by the module.
 */
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
/** \} */

/* --------------------------------------------- API Declarations */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \defgroup pan_api API Definitions
 * \{
 * Describes API definitions defined by the module.
 */
/**
 *  \brief To initialize PAN Profile
 *
 *  \par Description:
 *       This API initializes the PAN profile. This API must be called before
 *       any other PAN Profile API is called.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note This API must be called after BT_bluetooth_on() [1] is called to
 *        initialize the EtherMind Protocol Stack.
 */
API_RESULT BT_pan_init ( void );

/**
 *  \brief To shutdown PAN Profile
 *
 *  \par Description:
 *       This API shuts down the PAN profile.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note This API must be called before the BT_bluetooth_off() [1] is called to
 *        shutdown the EtherMind Protocol Stack. If there are active PAN profile
 *        connections, then these connections must be disconnected prior to
 *        calling this API.
 */
API_RESULT BT_pan_shutdown ( void );

/**
 *  \brief To register PAN Profile Callbacks
 *
 *  \par Description:
 *       This API allows an application to register its PAN Profile Event
 *       Indication and Data Indication Callbacks to the PAN Profile.
 *       These callbacks are described in detail in the PAN Profile
 *       Callbacks section.
 *
 *  \param [in] callbacks
 *         This parameter must be a pointer to a caller allocated PAN_CALLBACKS
 *         data type, containing the required callback function information.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 */
API_RESULT BT_pan_register
           (
               PAN_CALLBACKS  * callbacks
           );

/** Start Accepting Connection for PAN Services */
#define BT_pan_accept           BT_bnep_accept

/** To Open a PAN (BNEP/L2CAP) Connection to a BD_ADDR */
#define BT_pan_connect          BT_bnep_connect

/** To Setup a BNEP Connection */
#define BT_pan_setup            BT_bnep_setup

/**
 *  \brief To respond a Setup Connection request for a PAN Profile connection
 *
 *  \par Description:
 *       This API enables application to respond to received PAN Event Indication
 *       Callback with the event type set to PAN_SETUP_IND for a Setup Connection
 *       request from the remote Bluetooth device for a PAN Profile connection.
 *       At the time of delivering the PAN_SETUP_IND event, the PAN Profile
 *       indicates whether a response from the application is required.
 *       The application can control this behavior using the BT_pan_set_policy()
 *       API, once a PAN Connection is established (either by using
 *       the BT_pan_connect() API, or, by the PAN_CONNECT_IND callback).
 *       Once BNEP Setup Connection is completed, the PAN Profile confirms the
 *       result of the procedure to the application using the registered PAN Event
 *       Indication Callback with the event type set to PAN_SETUP_CNF.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address (BD_ADDR) of the remote Bluetooth device
 *         supporting PAN Profile. This parameter must be a pointer to a caller
 *         allocated UCHAR array of 6 octets containing the BD_ADDR.
 *
 *  \param [in] response
 *         This parameter must be a value from the PAN Setup Connection Response Codes.
 *         Application must use only the following values:
 *        i)PAN_SETUP_SUCCESSFUL
 *       ii)PAN_SETUP_NOT_ALLOWED
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_pan_setup_rsp
           (
               UCHAR   * bd_addr,
               UINT16    response
           );

/** To Close a PAN (BNEP/L2CAP) Connection to a BD_ADDR */
#define BT_pan_disconnect       BT_bnep_disconnect

/** To Set a Filter, Local or Remote, for a PAN Connection */
#define BT_pan_filter           BT_bnep_filter

/**
 *  \brief To transmit Ethernet packets over a PAN Profile connection, when the
 *         local service is PANU
 *
 *  \par Description:
 *       This API enables the application/profile to transmit Ethernet packets
 *       over a connected PAN Profile connection, as identified by the remote
 *       Bluetooth device address.
 *       The application must provide the 14 octets of Ethernet Packet Header,
 *       containing the Ethernet Destination & Source Address and the Ethernet
 *       Type/Length field, separately.
 *       As part of the handling of this API, the PAN Profile makes use of
 *       appropriate BNEP API to initiate transmission of the Ethernet Packet
 *       to the remote device. From the Destination and Source Address fields
 *       of the Ethernet header BNEP decides what BNEP Packet Type to use for
 *       transmitting the Ethernet Payload over the BNEP connection to remote
 *       Bluetooth device.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address (BD_ADDR) of the remote Bluetooth device
 *         supporting PAN Profile. This parameter must be a pointer to a caller
 *         allocated UCHAR array of 6 octets containing the BD_ADDR.
 *
 *  \param [in] eth_header
 *         This parameter must be a caller allocated UCHAR array of 14 octets
 *         containing the Ethernet Packet Header.
 *
 *  \param [in] ext_header
 *         This parameter must be a caller allocated PAN_EXT_HEADER array of size
 *         ext_header_count containing the Extension Headers. If no extension
 *         header is present, this parameter shall be NULL.
 *
 *  \param [in] ext_header_count
 *         This parameter specifies the number of Extension Headers provided in
 *         the ext_header parameter. If no extension header is present,
 *         this parameter shall be 0.
 *
 *  \param [in] eth_payload
 *         This parameter must be a caller allocated UCHAR array of
 *         eth_payload_len octets containing the Ethernet Packet Payload.
 *
 *  \param [in] eth_payload_len
 *         This parameter specifies the size of the Ethernet Packet Payload, as
 *         provided in the eth_payload parameter. Size of the Ethernet Packet
 *         payload must be less than or equal to 1500 octets.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note The PAN Profile allocates memory for the BNEP packet, which is
 *        constructed based on the Ethernet Packet Header supplied.
 *        Hence, if memory is allocated for eth_header and/or eth_payload,
 *        the same can be freed when this API returns.
 *        If an error is detected during actual transmission of the Ethernet packet
 *        (such as, abrupt disconnection etc.), the PAN_WRITE_CNF is called to
 *        notify the application of such errors. The PAN_WRITE_CNF is not called
 *        every time when the transmission is successful.
 */
API_RESULT BT_pan_write_panu
           (
               UCHAR *             bd_addr,
               UCHAR *             eth_header,
               PAN_EXT_HEADER *    ext_header,
               UINT16              ext_header_count,
               UCHAR *             eth_payload,
               UINT16              eth_payload_len
           );

/**
 *  \brief To transmit Ethernet packets over a PAN Profile connection, when the
 *         local service is NAP/GN
 *
 *  \par Description:
 *       This API enables the application/profile to transmit Ethernet packets
 *       over a connected PAN Profile connection, as identified by the remote
 *       Bluetooth device address.
 *       The application must provide the 14 octets of Ethernet Packet Header,
 *       containing the Ethernet Destination & Source Address and the Ethernet
 *       Type/Length field, separately.
 *       As part of the handling of this API, the PAN Profile makes use of
 *       appropriate BNEP API to initiate transmission of the Ethernet Packet
 *       to the remote device. From the Destination and Source Address fields
 *       of the Ethernet header BNEP decides what BNEP Packet Type to use for
 *       transmitting the Ethernet Payload over the BNEP connection to remote
 *       Bluetooth device.
 *       For NAP and GN roles, the PAN Profile handles the basic Ethernet
 *       bridging of Ethernet packets. For example, if the Destination Ethernet
 *       Address is an Ethernet Broadcast/Multicast address, then the Ethernet
 *       Packet is routed to all connected remote Bluetooth devices with PANU
 *       as the remote service.
 *
 *  \param [in] local_service
 *         This parameter specifies the local PAN Service for which the Packet
 *         transmission needs to be handled. Valid values are:
 *          i) PAN_SERVICE_NAP - Local Service is NAP
 *         ii) PAN_SERVICE_GN  - Local Service is GN
 *
 *  \param [in] eth_header
 *         This parameter must be a caller allocated UCHAR array of 14 octets
 *         containing the Ethernet Packet Header.
 *
 *  \param [in] eth_payload
 *         This parameter must be a caller allocated UCHAR array of
 *         eth_payload_len octets containing the Ethernet Packet Payload.
 *
 *  \param [in] eth_payload_len
 *         This parameter specifies the size of the Ethernet Packet Payload, as
 *         provided in the eth_payload parameter. Size of the Ethernet Packet
 *         payload must be less than or equal to 1500 octets.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note The PAN Profile allocates memory for the BNEP packet, which is
 *        constructed based on the Ethernet Packet Header supplied. Hence,
 *        if memory is allocated for eth_header and/or eth_payload, the same
 *        can be freed when this API returns.
 *        If an error is detected during actual transmission of the Ethernet packet
 *        (such as, abrupt disconnection etc.), the PAN_WRITE_CNF is called to
 *        notify the application of such errors. The PAN_WRITE_CNF is not called
 *        every time when the transmission is successful.
 */
API_RESULT BT_pan_write_nap_gn
           (
               UCHAR      local_service,
               UCHAR *    eth_header,
               UCHAR *    eth_payload,
               UINT16     eth_payload_len
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
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_PAN_API_ */

