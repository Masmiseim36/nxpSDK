
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
/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup  bnep_module  BNEP (Bluetooth Network Encapsulation Protocol)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Bluetooth Network Encapsulation Protocol module to the Application and
 *   other upper layers of the stack.
 */
/**
 * \defgroup  bnep_defines Defines
 * \{
 * Describes defines for BNEP module.
 */
/**
 * \defgroup bnep_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/** Invalid Connection Identifier */
#define BNEP_CONNECTION_ID_INVALID              0xFFU


/** BNEP - PAN Profile Service Bitmap Values */
/**
 *  \name BNEP - PAN Profile Service Bitmap Values
 *
 *  Constant Defines for BNEP - PAN Profile Service Bitmap Values
 */
/*@{*/
#define BNEP_SERVICE_PANU                       0x01U
#define BNEP_SERVICE_NAP                        0x02U
#define BNEP_SERVICE_GN                         0x04U
#define BNEP_SERVICE_NONE                       0x00U
#define BNEP_SERVICE_ALL                        0x07U
/*@}*/

/** BNEP Event Indication Callback Event Types */
/**
 *  \name BNEP Event Indication Callback Event Types
 *
 *  Constant Defines for BNEP Event Indication Callback Event Types
 */
/*@{*/
#define BNEP_CONNECT_IND                        0x01U
#define BNEP_CONNECT_CNF                        0x02U
#define BNEP_SETUP_IND                          0x03U
#define BNEP_SETUP_CNF                          0x04U
#define BNEP_DISCONNECT_IND                     0x05U
#define BNEP_DISCONNECT_CNF                     0x06U
#define BNEP_FILTER_IND                         0x07U
#define BNEP_FILTER_CNF                         0x08U
#define BNEP_WRITE_CNF                          0x09U
/*@}*/

/* BNEP Setup Connection Response Codes */
/**
 *  \name BNEP Event Indication Callback Event Types
 *
 *  Constant Defines for BNEP Event Indication Callback Event Types
 */
/*@{*/
#define BNEP_SETUP_SUCCESSFUL                   0x0000U
#define BNEP_SETUP_INVALID_DEST_UUID            0x0001U
#define BNEP_SETUP_INVALID_SRC_UUID             0x0002U
#define BNEP_SETUP_INVALID_UUID_SIZE            0x0003U
#define BNEP_SETUP_NOT_ALLOWED                  0x0004U
/*@}*/

/* BNEP Network Protocol Type Filter Response Codes */
/**
 *  \name BNEP Network Protocol Type Filter Response Codes
 *
 *  Constant Defines for BNEP Network Protocol Type Filter Response Codes
 */
/*@{*/
#define BNEP_FILTER_NW_TYPE_SUCCESSFUL          0x0000U
#define BNEP_FILTER_NW_TYPE_UNSUPPORTED         0x0001U
#define BNEP_FILTER_NW_TYPE_INVALID_RANGE       0x0002U
#define BNEP_FILTER_NW_TYPE_LIMIT_REACHED       0x0003U
#define BNEP_FILTER_NW_TYPE_SECURITY_BLOCK      0x0004U
/*@}*/

/* BNEP Multicast Address Filter Response Codes */
/**
 *  \name BNEP Multicast Address Filter Response Codes
 *
 *  Constant Defines for BNEP Multicast Address Filter Response Codes
 */
/*@{*/
#define BNEP_FILTER_MC_ADDR_SUCCESSFUL          0x0000U
#define BNEP_FILTER_MC_ADDR_UNSUPPORTED         0x0001U
#define BNEP_FILTER_MC_ADDR_INVALID_RANGE       0x0002U
#define BNEP_FILTER_MC_ADDR_LIMIT_REACHED       0x0003U
#define BNEP_FILTER_MC_ADDR_SECURITY_BLOCK      0x0004U
/*@}*/

/** BNEP Filtering Type - NW Type or MC Address */
/**
 *  \name BNEP Filtering Type - NW Type or MC Address
 *
 *  Constant Defines for BNEP Filtering Type - NW Type or MC Address
 */
/*@{*/
#define BNEP_FILTER_TYPE_NW_TYPE                0x01U
#define BNEP_FILTER_TYPE_MC_ADDR                0x02U
/*@}*/

/** BNEP Filtering Type - Local or Remote */
/**
 *  \name BNEP Filtering Type - Local or Remote
 *
 *  Constant Defines for BNEP Filtering Type - Local or Remote
 */
/*@{*/
#define BNEP_FILTER_TYPE_INVALID                0x00U
#define BNEP_FILTER_TYPE_LOCAL                  0x01U
#define BNEP_FILTER_TYPE_REMOTE                 0x02U
/*@}*/

/** Size of BNEP Network Type Filter Range */
#define BNEP_NW_TYPE_RANGE_SIZE                 4U

/** Size of BNEP Multicast Address Filter Range */
#define BNEP_MC_ADDR_RANGE_SIZE                 12U

/** The Ethernet 802.1Q (also known as IEEE 802.1p) Protocol Type Value */
#define BNEP_ETH_PROTO_TYPE_802_1Q              0x8100U

/** Ethernet Source & Destination Address Size */
#define BNEP_ETH_ADDR_SIZE                      BT_BD_ADDR_SIZE

/** Ethernet Protocol Type/Length Field Size */
#define BNEP_ETH_PROTO_LEN_SIZE                 2U

/** Ethernet Header Size */
#define BNEP_ETH_HEADER_SIZE \
        ((2U * BNEP_ETH_ADDR_SIZE) + BNEP_ETH_PROTO_LEN_SIZE)

/** Number of Octets occupied by Ethernet 802.1Q Tag */
#define BNEP_ETH_PROTO_802_1Q_HEADER_LEN        4U

/** BNEP Policy Settings for handling 'Setup' Requests */
/**
 *  \name BNEP Policy Settings for handling 'Setup' Requests
 *
 *  Constant Defines for BNEP Policy Settings for handling 'Setup' Requests
 */
/*@{*/
#define BNEP_SETUP_POLICY_ACCEPT                0x00U
#define BNEP_SETUP_POLICY_REJECT                0x01U
#define BNEP_SETUP_POLICY_ASK                   0x02U
#define BNEP_SETUP_POLICY_IGNORE                0x03U
#define BNEP_SETUP_POLICY_DEFAULT               BNEP_SETUP_POLICY_ACCEPT
/*@}*/

/** BNEP Default Policy Settings */
#define BNEP_POLICY_DEFAULTS                    BNEP_SETUP_POLICY_DEFAULT
/** \} */
/** \} */

/* --------------------------------------------- Structures/Data Types */

/**
 *  \defgroup bnep_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
/**
 * BNEP Event Indication Callback.
 *
 * BNEP calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param [in] event_type  Event Identifier.
 * \param [in] bd_addr     Peer BD_ADDR.
 * \param [in] event_result \ref API_SUCCESS or an error code in
 * \ref att_error_codes or any other internal error code from the stack.
 * \param [in] data_param  Data or Parameter associated with the event if any or NULL.
 * \param [in] data_len    Size of the data_param. 0 if data_param is NULL.
 */
typedef API_RESULT (* BNEP_EVENT_IND_CB)
                   (
                       UCHAR      event_type,     /* Event Identifier */
                       UCHAR *    bd_addr,        /* Remote BD_ADDR */
                       UINT16     event_result,   /* Result/Response */
                       void *     event_data,     /* Event Data */
                       UINT16     event_data_len  /* Event Data Length */
                   );

/**
 * BNEP Data Read Indication Callback.
 *
 * BNEP calls the registered callback to inform data received from peer to the application.
 *
 * \param [in] bd_addr     Peer BD_ADDR.
 * \param [in] header      Ethernet Header (14 Octets).
 * \param [in] payload     Ethernet Payload.
 * \param [in] payload_len Ethernet Payload Length.
 */
typedef API_RESULT (* BNEP_DATA_IND_CB)
                   (
                       UCHAR *    bd_addr,         /* Remote BD_ADDR */
                       UCHAR *    eth_header,      /* Ethernet Header (14 Octets) */
                       UCHAR *    eth_payload,     /* Ethernet Payload */
                       UINT16     eth_payload_len  /* Ethernet Payload Length */
                   );
/** \} */

/**
 *  \addtogroup bnep_defines Defines
 *  \{
 */
/**
 * \defgroup bnep_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/** BNEP Callbacks for Registration */
typedef struct
{
    /** BNEP Event Indication/Confirmation Callback */
    BNEP_EVENT_IND_CB    bnep_event_ind;

    /** BNEP Data Received Indication */
    BNEP_DATA_IND_CB     bnep_read_ind;

} BNEP_CALLBACKS;

/** BNEP Filter - Network Type */
typedef struct
{
    /** Ethernet Network Type Start */
    UINT16    start;

    /** Ethernet Network Type End */
    UINT16    end;

} BNEP_FILTER_NW_TYPE;

/** BNEP Filter - Multicast Address */
typedef struct
{
    /** Ethernet Multicast Address Start */
    UCHAR    start [BNEP_ETH_ADDR_SIZE];

    /** Ethernet Multicast Address End */
    UCHAR    end [BNEP_ETH_ADDR_SIZE];

} BNEP_FILTER_MC_ADDR;

/** BNEP Filter Parameters (delivered with BNEP_EVENT_IND_CB) */
typedef struct
{
    /** Type of BNEP Filter - Network Type or Multicast Address */
    UCHAR      type;

    /** Number of Filter Ranges */
    UINT16     num_range;

    /** Data Buffer containing the Filtering Ranges */
    void *     filter;

} BNEP_FILTER;

/** BNEP Setup Parameters (delivered with BNEP_SETUP_IND/BNEP_SETUP_CNF) */
typedef struct
{
    /** Configured Local PAN Service */
    UCHAR    local_service;

    /** Configured Remote PAN Service */
    UCHAR    remote_service;

    /**
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
    /** Extension Type */
    UCHAR type;

    /** Extension Length */
    UCHAR length;

    /** Extension Value */
    UCHAR * value;

} BNEP_EXT_HEADER;
/** \} */
/** \} */

/* --------------------------------------------- Unions */


/* --------------------------------------------- Macros */
/**
 * \defgroup bnep_utility_macros Utility Macros
 * \{
 * Describes Utility Macros defined by the module.
 */
/**
 *  To convert a Bluetooth Device Address (BD_ADDR, as represented within
 *  EtherMind Protocol Stack) to Ethernet Address, and vice-versa
 */
#define bnep_convert_address(addr_to, addr_from) \
        (addr_to)[5U] = (addr_from)[0U]; \
        (addr_to)[4U] = (addr_from)[1U]; \
        (addr_to)[3U] = (addr_from)[2U]; \
        (addr_to)[2U] = (addr_from)[3U]; \
        (addr_to)[1U] = (addr_from)[4U]; \
        (addr_to)[0U] = (addr_from)[5U];

/**
 *  To Copy Ethernet Addresses from one place to other.
 *  Since Ethernet Source/Destination addresses are of same
 *  size as that of Bluetooth Device Addresses (BD_ADDR), the
 *  BT_COPY_BD_ADDR macro can be used.
 */
#define BNEP_COPY_ETH_ADDR              BT_COPY_BD_ADDR

/** \} */

/* --------------------------------------------- API Declarations */
/**
 * \defgroup bnep_api API Definitions
 * \{
 * Describes BNEP API definitions of the module.
 */

#ifdef __cplusplus
extern "C" {
#endif
/**
 *  \brief To register BNEP profile/application Callbacks
 *
 *  \par Description:
 *       This API allows an application/profile to register its BNEP Profile
 *       Event Indication and Data Indication Callbacks to the BNEP Protocol.
 *       These callbacks are described in detail in the BNEP Protocol Callbacks section.
 *
 *  \param [in] callbacks
 *         This parameter must be a pointer to a caller allocated BNEP_CALLBACKS
           data type, containing the required callback function information.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bnep_register
           (
               BNEP_CALLBACKS *    callbacks
           );

/**
 *  \brief To start accept BNEP connections
 *
 *  \par Description:
 *       This API enables application to register BNEP services that the local device will allow any remote device to connect.
 *       There are three defined BNEP services:
 *       BNEP_SERVICE_PANU - Enable remote devices to connect to PANU Local Service
 *       BNEP_SERVICE_NAP  - Enable remote devices to connect to NAP Local Service
 *       BNEP_SERVICE_GN   - Enable remote devices to connect to GN Local Service
 *
 *  \param [in] services
 *         This parameter is a bitmap of BNEP services that the application
 *         wishes to enable. The BNEP Service values can be used in conjunction
 *         to specify the services.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bnep_accept
           (
               UCHAR  services
           );

/**
 *  \brief To initiate a BNEP connection
 *
 *  \par Description:
 *       This API enables application to initiate BNEP connection
 *       (BNEP-L2CAP connection) for the specified remote Bluetooth device.
 *       As part of the handling of this API, BNEP makes use of underlying L2CAP
 *       to initiate the connection. Once BNEP connection is established,
 *       the BNEP confirms the result of the connection procedure to
 *       the application/profile using the registered BNEP Event Indication
 *       Callback with the event type set to BNEP_CONNECT_CNF.
 *       This API only initiates establishment of the L2CAP level connection for
 *       the BNEP PSM. It does not initiate the BNEP Setup procedure required to
 *       configure the BNEP services. For the Setup Connection procedure,
 *       the application/profile needs to call the BT_bnep_setup() API,
 *       once the BNEP Connection is successfully established.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address (BD_ADDR) of the remote Bluetooth device
 *         supporting BNEP. This parameter must be a pointer to a caller
 *         allocated UCHAR array of 6 octets containing the BD_ADDR.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bnep_connect
           (
               UCHAR  * bd_addr
           );

/**
 *  \brief To initiate Setup of a BNEP connection
 *
 *  \par Description:
 *       This API enables application to initiate BNEP Setup Connection
 *       procedure for a BNEP connection for the specified remote
 *       Bluetooth device. The BNEP connection must be established for the
 *       specified remote Bluetooth device using the BT_bnep_connect() API.
 *       Once BNEP Setup Connection is completed, the BNEP confirms the result
 *       of the procedure to the application/profile using the registered BNEP
 *       Event Indication Callback with the event type set to BNEP_SETUP_CNF.
 *       The Setup Connection procedure must be done once a BNEP connection is
 *       established using BT_bnep_connect() API. However, this API can also be
 *       used later, on a BNEP connection which is already setup,
 *       to change the BNEP roles.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address (BD_ADDR) of the remote Bluetooth device
 *         supporting BNEP. This parameter must be a pointer to a caller
 *         allocated UCHAR array of 6 octets containing the BD_ADDR.
 *
 *  \param [in] local_service
 *         The local BNEP Service for which the Setup Connection to be initiated.
 *
 *  \param [in] remote_service
 *         The remote BNEP Service for which the Setup Connection to be initiated.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bnep_setup
           (
               UCHAR  * bd_addr,
               UCHAR    local_service,
               UCHAR    remote_service
           );

/**
 *  \brief To disconnect a BNEP connection
 *
 *  \par Description:
 *       This API enables application/profile to disconnect a BNEP connection
 *       for the specified remote Bluetooth device.
 *       Once the BNEP level connection is disconnected, confirmation of the
 *       disconnection procedure will be delivered to the application using
 *       the registered BNEP Event Indication Callback with the event type
 *       set to BNEP_DISCONNECT_CNF.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address (BD_ADDR) of the remote Bluetooth device
 *         supporting BNEP. This parameter must be a pointer to a caller
 *         allocated UCHAR array of 6 octets containing the BD_ADDR.
 *         The corresponding BNEP Connection must have been connected to
 *         a remote Bluetooth device for BNEP using the BT_bnep_connect() API,
 *         or, by the BNEP_CONNECT_IND callback.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bnep_disconnect
           (
               UCHAR * bd_addr
           );

/**
 *  \brief To set/reset local/remote BNEP Network Type or Multicast Address
 *         Filters for a BNEP connection
 *
 *  \par Description:
 *       This API can be used to set/reset either Network Type or Multicast
 *       Address filters for a BNEP connection locally or remotely.
 *       The Network Type filter provides the method to filter the Ethernet
 *       packets having Networking Protocol Types allowed by the filter to be
 *       transmitted over BNEP connection.
 *       The Multicast Address filter provides the method to filter, or, allow
 *       transmission of the Ethernet packets with Destination Addresses
 *       (with Multicast bit set) falling in the Filter Ranges.
 *       The corresponding BNEP Connection must have been connected to a remote
 *       Bluetooth device, and setup for BNEP using the BT_bnep_setup() API,
 *       or, by the BNEP_SETUP_IND callback.
 *       Once BNEP procedure is completed, the BNEP confirms the result of the
 *       procedure to the application using the registered
 *       BNEP Event Indication Callback with the event type set to BNEP_FILTER_CNF.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address (BD_ADDR) of the remote Bluetooth device
 *         supporting BNEP. This parameter must be a pointer to a caller
 *         allocated UCHAR array of 6 octets containing the BD_ADDR.
 *
 *  \param [in] local
 *         This parameter is a Boolean flag, which denotes whether the specified
 *         filters to be set locally or remotely.
 *         If set locally, then BNEP will refuse to transmit Ethernet packets
 *         containing Network Type outside the ranges defined in the filter.
 *         If set remotely, BNEP uses its protocol to set the filter on the
 *         remote Bluetooth device, so that only Ethernet Packets containing
 *         the Network Types allowed in the filter ranges will be allowed by
 *         the remote Bluetooth device when it transmit data to the local device.
 *         Valid values are:
 *         * BNEP_FILTER_TYPE_LOCAL
 *         * BNEP_FILTER_TYPE_REMOTE
 *
 *  \param [in] filter_type
 *         This parameter specifies whether the filter being set/reset is a
 *         Network Type or Multicast Address filter.
 *         The valid values are:
 *         i) BNEP_FILTER_TYPE_NW_TYPE
 *         ii) BNEP_FILTER_TYPE_MC_ADDR
 *
 *  \param [in] num_range
 *         This parameter specifies number of filter ranges specified for
 *         Network Type/Multicast Address filters that are being set/reset
 *         locally/remotely. To reset filters, this parameter must be specified
 *         to be Zero.
 *
 *  \param [in] filter
 *         This parameter is a pointer to a caller allocated array of either
 *         BNEP_FILTER_NW_TYPE or BNEP_FILTER_MC_ADDR data type,
 *         depending upon whether the filter_type is Network Type of
 *         Multicast Address respectively.
 *         This parameter must be set to NULL, if the filters are being reset,
 *         and the num_range is specified to be Zero.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bnep_filter
           (
               UCHAR *    bd_addr,
               UCHAR      local,
               UCHAR      filter_type,
               UINT16     num_range,
               void *     filter
           );

/**
 *  \brief To transmit Ethernet packets over a BNEP connection.
 *
 *  \par Description:
 *       This API enables the application/profile to transmit Ethernet packets
 *       over a BNEP connection, as identified by the remote Bluetooth device
 *       address.
 *       The application/profile must provide the 14 octets of Ethernet Packet
 *       Header, containing the Ethernet Destination & Source Address and the
 *       Ethernet Type/Length field, separately.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address (BD_ADDR) of the remote Bluetooth
 *         device supporting BNEP. This parameter must be a pointer to a caller
 *         allocated UCHAR array of 6 octets containing the BD_ADDR.
 *
 *  \param [in] eth_header
 *         This parameter must be a caller allocated UCHAR array of 14 octets
 *         containing the Ethernet Packet Header.
 *
 *  \param [in] ext_header
 *         This parameter must be a caller allocated PAN_EXT_HEADER array of
 *         size ext_header_count containing the Extension Headers.
 *         If no extension header is present, this parameter shall be NULL.
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
 *         This parameter specifies the size of the Ethernet Packet Payload,
 *         as provided in the eth_payload parameter. Size of the Ethernet
 *         Packet payload must be less than or equal to 1500 octets.
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note  The BNEP allocates memory for the BNEP packet, which is constructed
 *         based on the Ethernet Packet Header supplied. Hence, if memory is
 *         allocated for eth_header, ext_header and/or eth_payload,
 *         the same can be freed when this API returns.
 *         If an error is detected during actual transmission of
 *         the Ethernet packet (such as, abrupt disconnection etc.),
 *         the BNEP_WRITE_CNF is called to notify the application of such errors.
 *         The BNEP_WRITE_CNF is not called every time when the transmission is
 *         successful.
 */
API_RESULT BT_bnep_write
           (
               UCHAR            * bd_addr,
               UCHAR            * eth_header,
               BNEP_EXT_HEADER  * ext_header,
               UINT16             ext_header_count,
               UCHAR            * eth_payload,
               UINT16             eth_payload_len
           );

/**
 *  \brief To respond a Setup Connection request for a BNEP connection
 *
 *  \par Description:
 *       This API enables application/profile to respond to received BNEP Event
 *       Indication Callback with the event type set to BNEP_SETUP_IND for a
 *       Setup Connection request from the remote Bluetooth device for a BNEP connection.
 *       At the time of delivering the BNEP_SETUP_IND event, the BNEP indicates
 *       whether a response from the application is required.
 *       The application can control this behavior using the
 *       BT_bnep_set_policy() API, once a BNEP Connection is established
 *       (either by using the BT_bnep_connect() API, or, by the BNEP_CONNECT_IND callback).
 *       Once BNEP Setup Connection is completed, the BNEP confirms the result
 *       of the procedure to the application using the registered BNEP Event
 *       Indication Callback with the event type set to BNEP_SETUP_CNF.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address (BD_ADDR) of the remote Bluetooth device
 *         supporting BNEP. This parameter must be a pointer to a caller
 *         allocated UCHAR array of 6 octets containing the BD_ADDR.
 *
 *  \param [in] response
 *         This parameter must be a value from the BNEP Setup Connection
 *         Response Codes.
 *         Application/Profile must use only the following values:
 *         i) BNEP_SETUP_SUCCESSFUL
 *         ii) BNEP_SETUP_NOT_ALLOWED
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bnep_setup_rsp
           (
               UCHAR  * bd_addr,
               UINT16   response
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
/**
 *  \brief To respond a Setup Connection request for a BNEP connection
 *
 *  \par Description:
 *       This API enables application to configure how Setup Connection request
 *       received from a remote Bluetooth device, for a BNEP connection,
 *       to be handled.
 *       If required, this API must be called when a BNEP Connection is
 *       established, either by using the BT_bnep_connect() API, or,
 *       by the BNEP_CONNECT_IND callback.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address (BD_ADDR) of the remote Bluetooth devic
 *         supporting BNEP. This parameter must be a pointer to a caller
 *         allocated UCHAR array of 6 octets containing the BD_ADDR.
 *
 *  \param [in] policy
 *         This parameter is a bitmap, as defined below:
 *         Bit 0-1 : How remotely initiated Setup Connection needs to be handled
 *         00 - Always accept (default)
 *         01 - Always reject
 *         10 - Ask application
 *         11 - Reserved
 *         Bit 2-7 : Reserved
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bnep_set_policy
           (
               UCHAR  * bd_addr,
               UCHAR    policy
           );

/**
 *  \brief To enable/disable header compression during transmission
 *
 *  \par Description:
 *       This API enables or disables header compression during transmission
 *
 *  \param [in] state
 *         BT_TRUE  :  Enable compression
 *         BT_FALSE :  Disable compression 
 *
 *  \return None
 */
void BT_bnep_set_compression_state (/* IN */ UCHAR state);

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_BNEP_API_ */

