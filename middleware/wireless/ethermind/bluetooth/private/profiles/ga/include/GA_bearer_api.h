
/**
 *  \file GA_bearer_api.h
 *
 *  \brief This file defines the GA bearer Interface - includes
 *  Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_BEARER_API_
#define _H_GA_BEARER_API_


/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"

/**
 * \addtogroup bt_ga
 * \{
 */

/* --------------------------------------------- Global Definitions */
/**
 * \defgroup ga_bearer_module GA Bearer
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * GA Bearer module.
 */

/**
 * \defgroup ga_brr_defines Defines
 * \{
 * \brief Describes defines for the module.
 */

/**
 * \defgroup ga_brr_utility_macros Macros
 * \{
 * \brief Describes Macros for the module.
 */

/** Unreferenced variable macro to avoid compilation warnings */
#define GA_IGNORE_UNUSED_PARAM(v) (void)(v)

/** \} */

/**
 * \defgroup ga_bearer_err_codes Error Codes
 * \{
 * \brief Describes Values of Error Codes defined for GA module.
 */

/**
 * \name GA Result Error Codes
 * \{
 * \brief This section lists the error codes for GA Result.
 */

/** Represents Operation is Success */
#define GA_SUCCESS                                  0x0000U

/** Represents Operation is Failure */
#define GA_FAILURE                                  0xFFFFU

/** Represents Event is consumed */
#define GA_EVT_CONSUMED                             0x8000U

/** Represents Operation is not complete, Events to Follow */
#define GA_CONTINUE                                 0x00C0U

/** \} */

/**
 * \name Bearer Error Codes - TODO: Move to GA_error.h?
 * \{
 * \brief This section lists the bearer error codes.
 */

/** Bearer Invalid State */
#define GA_BRR_INVALID_STATE                        0x1011U

/** Bearer Invalid Params */
#define GA_BRR_INVALID_PARAMS                       0x1012U

/** \} */

/** \} */

/**
 * \defgroup ga_bearer_constants Constants
 * \{
 * \brief This section lists Bearer Constants
 */

/**
 * \name Bearer Constants - General Macros
 * \{
 * \brief Describes general macros for the module.
 */

/** Invalid Bearer Profile ID */
#define GA_BRR_PROFILE_ID_INVALID                   0xFFU

/** Invalid Bearer Service ID */
#define GA_BRR_SERVICE_ID_INVALID                   0xFFU

/** Invalid Bearer Service Type */
#define GA_BRR_SERVICE_TYPE_INVALID                 0xFFU

/** Invalid Bearer UUID */
#define GA_BRR_UUID_INVALID                         0x0000U

/** Dont care Service and Characteristic instance */
#define GA_BRR_INST_DONTCARE                        0xFFU

/** HCI Connectionless Event Code - Adv/Scan/BIG related */
#define GA_BRR_CONNECTIONLESS_EVENT_CODE            0xC0U

/** \} */

/**
 * \name Bearer Constants - Service Discover Types
 * \{
 * \brief Describes Service Discover Types for the module, which is used in
 * Client Role
 */

/** Primary Service Discovery Type */
#define GA_BRR_PS_DISCOVERY                         0x00U
/** Included Service Discovery Type */
#define GA_BRR_IS_DISCOVERY                         0x01U
/** Secondary Service Discovery Type */
#define GA_BRR_SS_DISCOVERY                         0x02U

/** \} */

/**
 * \name Bearer Constants - Service Types
 * \{
 * \brief Describes Service Types for the module, which is used in Server Role
 */

/** Primary Service Type */
#define GA_BRR_PS_TYPE                              0x00U
/** Secondary Service Type */
#define GA_BRR_SS_TYPE                              0x01U

/** \} */

/**
 * \name Bearer Constants - Service Handle Range
 * \{
 * \brief Describes Service Handle Range for the module
 */

/** Invalid Service Handle Value */
#define GA_BRR_INVALID_SERV_HANDLE_VAL              0x0000U
/** Service Handle Start Range */
#define GA_BRR_SERV_HANDLE_START_RANGE              0x0001U
/** Service Handle End Range */
#define GA_BRR_SERV_HANDLE_END_RANGE                0xFFFFU

/** \} */

/**
 * \name Common Profile and Service Error Code Descriptions
 * \{
 * \brief Describes Common Error Codes for Profiles and Services
 * Reference: Bluetooth Core Specification Supplement, Version 9 or later.
 */

/** Out of Range */
#define GA_BRR_ERR_OOR                       0xFFU

/** Procedure Already in Progress */
#define GA_BRR_ERR_PROC_ALREADY_IN_PROGRESS  0xFEU

/** Client Characteristic Configuration Descriptor Improperly Configured */
#define GA_BRR_ERR_CCD_IMPROPERLY_CONFIGURED 0xFDU

/** Write Request Rejected */
#define GA_BRR_ERR_WRITE_REQ_REJECTED        0xFCU

/** \} */

/** \} */

/**
 * \defgroup ga_brr_events Events
 * \{
 * \brief Describes Events for the module
 */

/**
 * \defgroup ga_brr_client_evt_defs Bearer Client Events
 * \{
 * \brief This section describes the Generic Audio Bearer
 * Events for Client.
 */

/*
 * TODO: Verify the description
 */
/**
 * \brief This event is notified when transport setup is complete from the
 * bearer to a profile from peer device.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_BRR_PROFILE_CB callback.
 *
 * \param [in] device  NULL
 * \param [in] context  Pointer to peer device handle \ref
 * \param [in] event_type  \ref GA_BRR_SETUP_TRANSPORT_CNF
 * \param [in] event_status  \ref GA_SUCCESS Setup Complete \n
 *                           \ref GA_FAILURE Others \n
 * \param [in] event_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - event_data - NULL \n
 *                           .
 * \param [in] event_datalen  If status is \ref GA_SUCCESS or
 *                            \ref GA_FAILURE, \n
 *                              - event_datalen - 0 \n
 *                              .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_SETUP_TRANSPORT_CNF                  0x00U

/**
 * \brief This event is notified on discovery of primary/secondary/included
 * services with peer device with the following values as parameters in the
 * \ref GA_BRR_PROFILE_CB callback.
 *
 * \param [in] device  NULL
 * \param [in] context  Pointer to peer device handle \ref
 * \param [in] event_type  \ref GA_BRR_DISCOVER_SERVICE_CNF
 * \param [in] event_status  \ref GA_CONTINUE Service Handle range of discovered
 *                        service \n
 *                        \ref GA_SUCCESS Service Discovery Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] event_data  If status is \ref GA_CONTINUE \n
 *                         - event_data - Pointer to object of type
 *                           \ref GA_BRR_SVC_INFO \n
 *                         .
 * \param [in] event_datalen  If status is \ref GA_CONTINUE \n
 *                            - event_datalen - Size of \ref GA_BRR_SVC_INFO \n
 *                            .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_DISCOVER_SERVICE_CNF                 0x01U

/**
 * \brief This event is notified on Characteristics discovery.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_BRR_PROFILE_CB callback.
 *
 * \param [in] device  NULL
 * \param [in] context  Pointer to peer device handle \ref
 * \param [in] event_type  \ref GA_BRR_GET_SERVICE_COMPOSITION_CNF
 * \param [in] event_status  \ref GA_CONTINUE Perform Service Composition \n
 *                        \ref GA_SUCCESS Service Composition Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] event_data  If status is \ref GA_CONTINUE \n
 *                           - event_data - Pointer to object of type
 *                           \ref GA_BRR_SERVICE_COMPOSITION \n
 *                           .
 * \param [in] event_datalen  If status is \ref GA_CONTINUE \n
 *                           - event_datalen - Size of \ref
 *                             GA_BRR_SERVICE_COMPOSITION \n
 *                           .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_GET_SERVICE_COMPOSITION_CNF          0x02U

/**
 * \brief This event is notified on Characteristics Descriptor discovery.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_BRR_PROFILE_CB callback.
 *
 * \param [in] device  NULL
 * \param [in] context  Pointer to peer device handle \ref
 * \param [in] event_type  \ref GA_BRR_GET_CHARACTERISTIC_COMPOSITION_CNF
 * \param [in] event_status  \ref GA_CONTINUE Perform Characteristic Composition \n
 *                        \ref GA_SUCCESS Characteristic Composition Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] event_data  If status is \ref GA_CONTINUE \n
 *                           - event_data - Pointer to object of type
 *                           \ref GA_BRR_CHARACTERISTIC_COMPOSITION \n
 *                           .
 * \param [in] event_datalen  If status is \ref GA_CONTINUE \n
 *                           - event_datalen - Size of \ref
 *                             GA_BRR_CHARACTERISTIC_COMPOSITION \n
 *                           .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_GET_CHARACTERISTIC_COMPOSITION_CNF   0x03U

/**
 * \brief This event is notified when a write response is received for a
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_BRR_PROFILE_CB callback.
 *
 * \param [in] device  NULL
 * \param [in] context  Pointer to peer device handle \ref
 * \param [in] event_type  \ref GA_BRR_WRITE_CHARACTERISTIC_CNF
 * \param [in] event_status  \ref GA_SUCCESS : Write Response received \n
 * \param [in] event_data  NULL \n
 * \param [in] event_datalen  0
 *
 * \return\ref GA_SUCCESS (always)
 */
#define GA_BRR_WRITE_CHARACTERISTIC_CNF             0x04U

/*
 * TODO: Verify the parameter event_data
 */
/**
 * \brief This event is notified when a read response is received for a
 * characteristic from peer device with the following values as
 * parameters in the \ref GA_BRR_PROFILE_CB callback.
 *
 * \param [in] device  NULL
 * \param [in] context  Pointer to peer device handle \ref
 * \param [in] event_type  \ref GA_BRR_READ_CHARACTERISTIC_CNF
 * \param [in] event_status  \ref GA_SUCCESS : Read Response received \n
 * \param [in] event_data  Pointer to void
 * \param [in] event_datalen  \ref sizeof ( \ref UINT16 )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_READ_CHARACTERISTIC_CNF              0x05U

/**
 * \brief This event indicates the Configuration of Notification/Indication of
 * Characteristics. Callback is triggered with the following values
 * as parameters in the \ref GA_BRR_PROFILE_CB callback.
 *
 * \param [in] device  NULL
 * \param [in] context  Pointer to peer device handle \ref
 * \param [in] event_type  \ref GA_BRR_CONFIG_CHARACTERISTIC_CNF
 * \param [in] event_status  \ref GA_SUCCESS Configure for Notification
 *                           Complete \n
 *                           \ref GA_FAILURE Others \n
 * \param [in] event_data  NULL \n
 * \param [in] event_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_CONFIG_CHARACTERISTIC_CNF            0x06U

/*
 * TODO: Verify the parameter event_data
 */
/**
 * \brief This event is notified when a notification/indication is received for
 * a characteristic from peer device with the following values as parameters in
 * the \ref GA_BRR_PROFILE_CB callback.
 *
 * \param [in] device  NULL
 * \param [in] context  Pointer to peer device handle \ref GA_BRR_CHR_HANDLE
 * \param [in] event_type  \ref GA_BRR_NOTIFICATION_IND
 * \param [in] event_status  \ref GA_SUCCESS Notification received
 * \param [in] event_data  Pointer to void
 * \param [in] event_datalen  \ref sizeof ( \ref UINT16 )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_NOTIFICATION_IND                     0x07U

/** \} */

/**
 * \defgroup ga_brr_server_evt_defs Bearer Server Events
 * \{
 * \brief This section describes the Generic Audio Bearer Events for Server.
 */

/*
 * TODO: Verify the description
 */
/**
 * \brief This event is notified when transport setup from bearer to profile
 * request is received from peer device.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_BRR_SERVICE_CB callback.
 *
 * \param [in] device  NULL
 * \param [in] context  Pointer to peer device handle \ref
 * \param [in] event_type  \ref GA_BRR_SETUP_TRANSPORT_IND
 * \param [in] event_status  \ref GA_SUCCESS Setup request received \n
 *                           \ref GA_FAILURE Others \n
 * \param [in] event_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - event_data - NULL \n
 *                           .
 * \param [in] event_datalen  If status is \ref GA_SUCCESS or
 *                            \ref GA_FAILURE, \n
 *                              - event_datalen - 0 \n
 *                              .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_SETUP_TRANSPORT_IND                  0x10U

/**
 * \brief This event is notified when a write request is received for a
 * characteristic from peer device with the following values as parameters in
 * the \ref GA_BRR_SERVICE_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] context  Pointer to \ref GA_BRR_CHR_CONTEXT
 * \param [in] event_type  \ref GA_BRR_WRITE_CHARACTERISTIC_IND
 * \param [in] event_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] event_data  Pointer to void
 * \param [in] event_datalen  \ref sizeof ( \ref UINT16 ).
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_brr_write_characteristic_rsp() has to be called to
 *       send the write response.
 */
#define GA_BRR_WRITE_CHARACTERISTIC_IND             0x11U

/*
 * TODO: Verify the parameter event_data
 */
/**
 * \brief This event is notified when a read request is received for a
 * characteristic from peer device with the following values as parameters in
 * the \ref GA_BRR_SERVICE_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] context  Pointer to \ref GA_BRR_CHR_CONTEXT
 * \param [in] event_type  \ref GA_BRR_READ_CHARACTERISTIC_IND
 * \param [in] event_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] event_data  Pointer to void
 * \param [in] event_datalen  \ref sizeof ( \ref UINT16 ).
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_brr_read_characteristic_rsp() has to be called to
 *       send the write response.
 */
#define GA_BRR_READ_CHARACTERISTIC_IND              0x12U

/*
 * TODO: Verify the parameter event_data and event_datalen
 */
/**
 * \brief This event indicates the Configuration of Notification/Indication of
 * Characteristics.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_BRR_PROFILE_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] context  Pointer to \ref GA_BRR_CHR_CONTEXT
 * \param [in] event_type  \ref GA_BRR_CONFIG_CHARACTERISTIC_IND
 * \param [in] event_status  \ref GA_SUCCESS  \n
 * \param [in] event_data
 * \param [in] event_datalen
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_CONFIG_CHARACTERISTIC_IND            0x13U

/*
 * TODO: Verify the parameter event_data
 */
/**
 * \brief This event is notified when a read blob request is received for a
 * characteristic from peer device with the following values as parameters in
 * the \ref GA_BRR_SERVICE_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] context  Pointer to \ref GA_BRR_CHR_CONTEXT
 * \param [in] event_type  \ref GA_BRR_READ_BLOB_CHARACTERISTIC_IND
 * \param [in] event_status  \ref GA_SUCCESS : Read Blob Request received \n
 * \param [in] event_data  Pointer to void
 * \param [in] event_datalen  \ref sizeof ( \ref UINT16 ).
 *
 * \return \ref GA_SUCCESS (always)
 * \note GA_brr_read_characteristic_rsp() has to be called to
 *       send the write response.
 */
#define GA_BRR_READ_BLOB_CHARACTERISTIC_IND         0x14U

/** \} */

/* HCI Connectionless Event Identifiers */
#define GA_BRR_CONFIGURE_ADV_CNF                    (0x00U | GA_BRR_CONNECTIONLESS_EVENT_CODE)
#define GA_BRR_CONFIGURE_SCAN_CNF                   (0x01U | GA_BRR_CONNECTIONLESS_EVENT_CODE)
#define GA_BRR_CONFIGURE_SYNC_CNF                   (0x02U | GA_BRR_CONNECTIONLESS_EVENT_CODE)
#define GA_BRR_CONFIGURE_BROADCAST_CNF              (0x03U | GA_BRR_CONNECTIONLESS_EVENT_CODE)
#define GA_BRR_CONFIGURE_BROADCAST_SYNC_CNF         (0x04U | GA_BRR_CONNECTIONLESS_EVENT_CODE)
#define GA_BRR_EXTENDED_ADV_REPORT_IND              (0x05U | GA_BRR_CONNECTIONLESS_EVENT_CODE)
#define GA_BRR_PERIODIC_ADV_REPORT_IND              (0x06U | GA_BRR_CONNECTIONLESS_EVENT_CODE)
#define GA_BRR_BIGINFO_ADV_REPORT_IND               (0x07U | GA_BRR_CONNECTIONLESS_EVENT_CODE)
#define GA_BRR_PERIODIC_ADV_SYNC_LOST_IND           (0x08U | GA_BRR_CONNECTIONLESS_EVENT_CODE)
#define GA_BRR_BROADCAST_SYNC_LOST_IND              (0x09U | GA_BRR_CONNECTIONLESS_EVENT_CODE)
#define GA_BRR_BIG_TERMINATE_IND                    (0x0AU | GA_BRR_CONNECTIONLESS_EVENT_CODE)

/**
 * \defgroup ga_brr_common_evt_defs Bearer Common Events
 * \{
 * \brief This section describes the Generic Audio Bearer Common Events.
 */

/*
 * TODO: Verify the description
 */
/**
 * \brief This event is notified when ATT Connection is requested. It sets the
 * peer device information.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_BRR_CB callback.
 *
 * \param [in] device  Pointer to \ref
 * \param [in] event_type  \ref GA_BRR_TRANSPORT_UP_IND
 * \param [in] event_status  \ref GA_SUCCESS : ATT Connection Request
 *                           received \n
 * \param [in] event_data  NULL \n
 * \param [in] event_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_TRANSPORT_UP_IND                     0x81U

/*
 * TODO: Verify the description
 */
/**
 * \brief This event is notified when ATT Disconnection is requested. It
 * initialize bearer device.
 * Callback is triggered with the following values as parameters in the
 * \ref GA_BRR_CB callback.
 *
 * \param [in] device  Pointer to \ref
 * \param [in] event_type  \ref GA_BRR_TRANSPORT_DOWN_IND
 * \param [in] event_status  \ref GA_SUCCESS : ATT Disconnection Request
 *                           received \n
 * \param [in] event_data  NULL \n
 * \param [in] event_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_TRANSPORT_DOWN_IND                   0x82U

/**
 * \brief This is an event to indicate random event. This event shall never be
 * generated. It is used to reset Event ID for the context.
 *
 * \param [in] device  NULL
 * \param [in] context  NULL
 * \param [in] event_type  \ref GA_BRR_EVENT_INVALID
 * \param [in] event_status  NULL \n
 * \param [in] event_data  NULL \n
 * \param [in] event_datalen  0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define GA_BRR_EVENT_INVALID                        0xFFU

/** \} */

/** \} */

/**
 * \addtogroup ga_bearer_constants
 * \{
 */

/**
 * \name Bearer Constants - General Macros
 * \{
 * \brief Describes general macros for the module.
 */

/** Mask to set notification with response (Indication) */
#define GA_BRR_NTF_RESPONSE_MASK                    0x80U

/** \} */

/**
 * \name Bearer Constants - Bearer Internal Role
 * \{
 * \brief Describes Bearer Internal Roles, used to identify modes of
 * interaction with upper layer.
 */

/** Bearer Internal Role - Generic */
#define GA_BRR_GENERIC_ROLE                         0x00U

/** Bearer Internal Role - Profile */
#define GA_BRR_PROFILE_ROLE                         0x01U

/** Bearer Internal Role - Service */
#define GA_BRR_SERVICE_ROLE                         0x02U

/** Bearer Internal Role - Invalid */
#define GA_BRR_INVALID_ROLE                         0xFFU

/** \} */

/**
 * \name Bearer Constants - General Macros
 * \{
 * \brief Describes general macros for the module.
 */

/** Offset Value Length : Read Blob Request */
#define GA_BRR_ATT_READ_BLOB_VALUE_OFFSET_LEN       0x02U

/** \} */

/*
 * TODO: Need events for Transport Disconnection too?
 * Currently looks like the Idea was to have
 * - GA_BRR_SETUP_TRANSPORT_CNF for Profile roles
 * - GA_BRR_SETUP_TRANSPORT_IND for Server roles
 * Need to check if these events are to be unified and
 * provided to GA profile and service callbacks or
 * provided only to GA Core Callback.
 */

/**
 * \name Bearer Constants - Characteristic Properties
 * \{
 * \brief Characteristic Properties - Combination of these properties (combined
 * using bitwise OR) describe properties of the Characteristic.
 */

/**
 * Characteristic support Broadcast of its value to the peer.
 * Setting this property automatically includes Characteristic Server
 * Configuration Descriptor to the Characteristic
 */
#define GA_BRR_CHAR_BROADCAST_PROPERTY                0x00000001U

/** Characteristic support Reading its value by peer */
#define GA_BRR_CHAR_READ_PROPERTY                     0x00000002U

/** Characteristic support Writing its value by peer Without Response  */
#define GA_BRR_CHAR_WRITE_WITHOUT_RSP_PROPERTY        0x00000004U

/** Characteristic supports Writing its value by peer */
#define GA_BRR_CHAR_WRITE_PROPERTY                    0x00000008U

/**
 * Characteristic supports Notifying its value to the peer.
 * Setting this property automatically includes Characteristic Server
 * Configuration Descriptor to the Characteristic
 */
#define GA_BRR_CHAR_NOTIFY_PROPERTY                   0x00000010U

/** Characteristic supports Indicating its value to the peer */
#define GA_BRR_CHAR_INDICATE_PROPERTY                 0x00000020U

/** Characteristic supports Signed Write on its value */
#define GA_BRR_CHAR_SIGNED_WRITE_PROPERTY             0x00000040U

/** Characteristic supports write on its User Description Descriptor */
#define GA_BRR_CHAR_WRIEABLE_AUX_PROPERTY             0x00000200U

/** No Auxillary Property */
#define GA_BRR_NO_AUXILLARY_PROPERTY                  0x00U

/** Characteristic Value is Fixed Length */
#define GA_BRR_FIXED_LENGTH_PROPERTY                  0x01U

/** Authorization Property */
#define GA_BRR_AUTHORIZATION_PROPERTY                 0x02U

/** Peer Specific Value property */
#define GA_BRR_PEER_SPECIFIC_VAL_PROPERTY             0x40U

/** Constant Attribute Value Property */
#define GA_BRR_CONST_ATTR_VAL_PROPERTY                0x80U

/** \} */

/**
 *  \defgroup ga_brr_write_type Write Type
 *  \{
 *  Write Type - Request/Command.
 */

/** Write Type: Request */
#define GA_BRR_WRITE_TYPE_REQ                         0x00U
/** Write Type: Command/Write Without Response */
#define GA_BRR_WRITE_TYPE_CMD                         0x01U

/** \} */

/**
 * \name Bearer Constants - Advertising Types
 * \{
 * \brief Advertising types used for broadcast
 */

/** Advertising Types - Extended Advertising */
#define GA_BRR_ADV_TYPE_EA                            (0x01U << 0U)
/** Advertising Types - Periodic Advertising */
#define GA_BRR_ADV_TYPE_PA                            (0x01U << 1U)

/** \} */

/** \} */

/* --------------------------------------------- Data types /Structures */

/**
 * \defgroup ga_brr_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by the module.
 */

/** GA Result type */
typedef UINT16 GA_RESULT;

/** GA Bearer Device Indentifier */
typedef GA_ENDPOINT GA_BRR_DEVICE;

/** GA Bearer Profile Handle */
typedef UINT8 GA_BRR_PRF_HANDLE;

/** GA Bearer Characteristic Handle */
typedef UINT16 GA_BRR_CHR_HANDLE;

/** GA Bearer Service Instance */
typedef UINT8 GA_BRR_SVC_INST;

/** GA Bearer Characteristic Instance */
typedef UINT8 GA_BRR_CHR_INST;

/** GA Bearer UUID type */
/* TODO: <- Make provision for 128bit? */
typedef UINT16 GA_BRR_UUID;

/** GA Bearer Write Type */
typedef UINT8 GA_BRR_WRITE_TYPE;

/** GA Bearer Characteristic Context localy */
typedef struct _GA_BRR_CHR_CONTEXT
{
    /** UUID of Characteristic */
    GA_BRR_UUID uuid;

    /** Characteristic Instance */
    GA_BRR_CHR_INST inst;

    /** is response required */
    UINT8 to_rsp;

    /** Is Blob Request */
    UCHAR is_blob;

} GA_BRR_CHR_CONTEXT;

/** GA Bearer Characteristic Value */
typedef struct _GA_BRR_CHR_VAL
{
    /** Value array of octets */
    UCHAR * val;

    /** Value length */
    UINT16 len;

} GA_BRR_CHR_VAL;

/** GA Bearer Service Range */
typedef struct _GA_BRR_ATTR_RANGE
{
    /** Start of Service */
    UINT16 start;

    /** End of Service */
    UINT16 end;

} GA_BRR_ATTR_RANGE;

/** GA Bearer Service Range */
typedef GA_BRR_ATTR_RANGE GA_BRR_SVC_RANGE;

/** GA Bearer Characteristic Range */
typedef GA_BRR_ATTR_RANGE GA_BRR_CHAR_RANGE;

/**
 * Service UUID information - Will be a list of this as event_data in
 * \ref GA_BRR_DISCOVER_SERVICE_CNF
 */
typedef struct _GA_BRR_SVC_INFO
{
    /** UUID of the Service */
    GA_BRR_UUID uuid;

    /** Service Range */
    GA_BRR_ATTR_RANGE range;

} GA_BRR_SVC_INFO;

/** Write Request Format */
typedef struct _GA_BRR_WRITE_REQ
{
    /** Write request Value */
    GA_BRR_CHR_VAL     req_data;

    /** Write Type representation */
    GA_BRR_WRITE_TYPE  write_type;

}GA_BRR_WRITE_REQ;

/** Broadcast Parameters - To setup the BIG */
typedef struct _GA_BRR_BCAST_PARAM
{
    /** Broadcast code */
    UCHAR broadcast_code[16];

    /** SDU Interval */
    UINT32 sdu_interval;

    /** Maximum SDU */
    UINT16 max_sdu;

    /** Maximum latency */
    UINT16 max_latency;

    /** Retransmission count */
    UCHAR rtn;

    /** Phy to use */
    UCHAR phy;

    /** packing format */
    UCHAR packing;

    /** Framing format */
    UCHAR framing;

    /** Encryption state */
    UCHAR encryption;

    /** Number of BIS */
    UCHAR bis_count;

} GA_BRR_BCAST_PARAM;

/** Broadcast Sync Parameters - To sync with the available BIG reported */
typedef struct _GA_BRR_BCAST_SYNC_PARAM
{
    /** Broadcast code */
    UCHAR broadcast_code[16];

    /** Pointer to the BIS stream */
    UCHAR * bis;

    /** Sync Timeout */
    UINT16 sync_timeout;

    /** Encryption state */
    UCHAR encryption;

    /** Maximum SubEvents */
    UCHAR mse;

    /** Number of BIS */
    UCHAR bis_count;

} GA_BRR_BCAST_SYNC_PARAM;

/** Characteristic UUID information */
typedef struct _GA_BRR_CHR_INFO
{
    /** UUID of the characteristic */
    GA_BRR_UUID uuid;

    /** Characteristic Handle */
    UINT16 handle;

    /** Characteristic Property */
    UINT8 property;

} GA_BRR_CHR_INFO;

/** Characteristic Descriptor UUID information */
typedef struct _GA_BRR_CHRDESC_INFO
{
    /** UUID of the characteristic descriptor */
    GA_BRR_UUID uuid;

    /** Characteristic Descriptor Handle */
    UINT16 handle;

} GA_BRR_CHRDESC_INFO;

/**
 * Service Composition Data - Will be the event_data in
 * \ref GA_BRR_GET_SERVICE_COMPOSITION_CNF
 */
typedef struct _GA_BRR_SERVICE_COMPOSITION
{
    /** Service Information */
    GA_BRR_SVC_INFO * service;

    /** List of Characteristic Information */
    GA_BRR_CHR_INFO * characteristic;

    /** Number of Characteristics */
    UINT8 characteristic_count;

} GA_BRR_SERVICE_COMPOSITION;

/**
 * Characteristic Composition Data - Will be the event_data in
 * \ref GA_BRR_GET_CHARACTERISTIC_COMPOSITION_CNF
 */
typedef struct _GA_BRR_CHARACTERISTIC_COMPOSITION
{
    /** List of Characteristic Descriptor Information */
    GA_BRR_CHRDESC_INFO * desc;

    /** Number of Characteristic Descriptors */
    UINT8 desc_count;

} GA_BRR_CHARACTERISTIC_COMPOSITION;

/**
* Data - Will be the event_data in
* \ref
*/
typedef struct _GA_BRR_EVENT_PARAMS
{
    /** Event data pointer */
    void        * event_data;

    /** Event data length */
    UINT16        event_datalen;

    /** Event Status */
    UINT16        event_status;

    /** Event type */
    UINT8         event_type;

} GA_BRR_EVENT_PARAMS;

/** \} */

/** \} */

/**
 * \defgroup ga_brr_cb Application Callback
 * \{
 * \brief This Section Describes the module Callback interface offered
 * to the application
 *
 */

/**
 * \brief GA Bearer Callback to core
 *
 * \param [in] device        Peer Device Info
 * \param [in] event_type    Event Type
 * \param [in] event_status  Event Status
 * \param [in] event_data    Event Data
 * \param [in] event_datalen Event Data Length
 */
typedef GA_RESULT (* GA_BRR_CB)
                  (
                      /* IN */ GA_BRR_DEVICE * device,
                      /* IN */ UINT8           event_type,
                      /* IN */ UINT16          event_status,
                      /* IN */ void          * event_data,
                      /* IN */ UINT16          event_datalen
                  );

/*
 * Bearer Callback to Profile?
 */
/**
 * GA Profile Callback to the core
 *
 * \param [in] device        Peer Device Info
 * \param [in] context       Reference context
 * \param [in] event_type    Event Type
 * \param [in] event_status  Event Status
 * \param [in] event_data    Event Data
 * \param [in] event_datalen Event Data Length
 */
typedef GA_RESULT (* GA_BRR_PROFILE_CB)
                  (
                      /* IN */ GA_BRR_DEVICE * device,
                      /* IN */ void          * context,
                      /* IN */ UINT8         event_type,
                      /* IN */ UINT16        event_status,
                      /* IN */ void          * event_data,
                      /* IN */ UINT16        event_datalen
                   );

/*
 * Bearer Callback to a Given Service?
 */
/**
 * GA Service Callback to the core
 *
 * \param [in] device        Peer Device Info
 * \param [in] context       Reference context
 * \param [in] event_type    Event Type
 * \param [in] event_status  Event Status
 * \param [in] event_data    Event Data
 * \param [in] event_datalen Event Data Length
 */
typedef GA_RESULT (* GA_BRR_SERVICE_CB)
                  (
                      /* IN */ GA_BRR_DEVICE * device,
                      /* IN */ void          * context,
                      /* IN */ UINT8         event_type,
                      /* IN */ UINT16        event_status,
                      /* IN */ void          * event_data,
                      /* IN */ UINT16        event_datalen
                   );

/**
 * Call Back to Inform GA BRR Internal layer from GA BRR Platform Abstraction
 *
 * \param [in] device        Peer Device Info
 * \param [in] context       Reference context
 * \param [in] brr_role      Bearer Internal Role
 * \param [in] inst_handle   Instance handle
 * \param [in] event_type    Event Type
 * \param [in] event_status  Event Status
 * \param [in] event_data    Event Data
 * \param [in] event_datalen Event Data Length
 */
typedef GA_RESULT (* GA_BRR_PL_CB)
                  (
                      /* IN */ GA_BRR_DEVICE * device,
                      /* IN */ void          * context,
                      /* IN */ UCHAR         brr_role,
                      /* IN */ void          * inst_handle,
                      /* IN */ UINT8         event_type,
                      /* IN */ UINT16        event_status,
                      /* IN */ void          * event_data,
                      /* IN */ UINT16        event_datalen
                   );

/** \} */

/**
 * \addtogroup ga_brr_defines
 * \{
 */

/**
 * \addtogroup ga_brr_structures
 * \{
 */

/**
 * \defgroup ga_brr_interface Bearer Interface
 * \{
 * \brief This section lists the bearer interface for use
 * by the module.
 */

/** Interface structure for all bearer procedures */
typedef struct _GA_BRR_INTERFACE
{
    /**
     *  \brief To Init the GA Bearer Layer.
     *
     *  \par Description:
     *       This function enables to inilialize the GA Bearer Layer.
     *
     *  \param [in] cb
     *         Common callback of GA core
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* init) (/* IN */ GA_BRR_CB cb);

    /**
     *  \brief To Shutdown the GA Bearer Layer.
     *
     *  \par Description:
     *       This function enables to shutdown the GA Bearer Layer.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* shutdown) (void);

    /**
     *  \brief Add a GATT Service.
     *
     *  \par Description:
     *       This function enables to add a GATT Service with the GA Bearer.
     *
     *  \param [in] service_uuid
     *         Assigned UUID of this GATT Service.
     *
     *  \param [in] service_type
     *         Type associated with the service.
     *
     *  \param [in] cb
     *         Callback associated with Service ID.
     *
     *  \param [out] sid
     *         Service context ID.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (*add_service) \
              (
                  /* IN  */ GA_BRR_UUID service_uuid,
                  /* IN  */ UINT8 service_type,
                  /* IN  */ GA_BRR_SERVICE_CB cb,
                  /* OUT */ GA_BRR_SVC_INST * sid
              );

    /**
     *  \brief Add a Included GATT Service.
     *
     *  \par Description:
     *       This function enables to add a Included GATT Service with the
     *       GA Bearer.
     *
     *  \param [in] sid
     *         Service context ID.
     *
     *  \param [in] service_uuid
     *         Assigned UUID of this GATT Service.
     *
     *  \param [in] service_state
     *         Initial state of the Included service.
     *         Enabled (GA_TRUE) or Disabled (GA_FALSE)
     *
     *  \param [out] isid
     *         Included Service context ID.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* include_service) \
              (
                  /* IN */ GA_BRR_SVC_INST sid,
                  /* IN */ GA_BRR_UUID service_uuid,
                  /* IN */ UINT8 service_state,
                  /* IN */ GA_BRR_SVC_INST * isid
              );

    /**
     *  \brief Remove a GATT Service.
     *
     *  \par Description:
     *       This function enables to remove a GATT Service with the GA Bearer.
     *
     *  \param [in] sid
     *         Service context ID.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* remove_service) (/* IN  */ GA_BRR_SVC_INST sid);

    /**
     *  \brief Add a GATT Characteristic to a Service.
     *
     *  \par Description:
     *       This function enables to add a GATT Characteristic to a given
     *       Service with the GA Bearer.
     *
     *  \param [in] sid
     *         Service context ID.
     *
     *  \param [in] char_uuid
     *         Assigned UUID of this GATT Characteristic.
     *
     *  \param [in] char_property
     *         Properties associated with this GATT Characteristic.
     *
     *  \param [in] value
     *         Any value associated with this GATT Characteristic.
     *
     *  \param [out] cid
     *         Characteristic context ID.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* add_characteristic) \
              (
                  /* IN  */ GA_BRR_SVC_INST sid,
                  /* IN  */ GA_BRR_UUID     char_uuid,
                  /* IN  */ UINT16          char_property,
                  /* IN  */ GA_BRR_CHR_VAL  * value,
                  /* OUT */ GA_BRR_CHR_INST * cid
              );

    /**
     *  \brief Enable/Disable a GATT Service.
     *
     *  \par Description:
     *       This function enables/disables GATT Service.
     *
     *  \param [in] sid
     *         Service ID
     *
     *  \param [in] iid
     *         Including Service ID, if sid is of an included service.
     *         GA_BRR_SERVICE_ID_INVALID if not applicable
     *
     *  \param [in] state
     *         Enable (GA_TRUE) or Disable (GA_FALSE).
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* manage_service)
              (
                  /* IN  */ UINT8 sid,
                  /* IN  */ UINT8 iid,
                  /* IN  */ UINT8 state
              );

    /**
     *  \brief Enable/Disable a GATT Characteristic.
     *
     *  \par Description:
     *       This function enables/disables GATT Characteristic.
     *
     *  \param [in] sid
     *         Service ID
     *
     *  \param [in] cid
     *         Characteristic ID
     *
     *  \param [in] state
     *         Enable (GA_TRUE) or Disable (GA_FALSE).
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* manage_characteristic)
              (
                  /* IN  */ UINT8 sid,
                  /* IN  */ UINT8 cid,
                  /* IN  */ UINT8 state
              );

    /*
     * Provision to update the characteric value
     */
    /**
     *  \brief Update a GATT Characteristic to a Service.
     *
     *  \par Description:
     *       This function enables to Update a GATT Characteristic to a given
     *       Service with the GA Bearer.
     *
     *  \param [in] sid
     *         Service context ID.
     *
     *  \param [in] char_uuid
     *         Assigned UUID of this GATT Characteristic.
     *
     *  \param [in] cid
     *         Characteristic context ID.
     *
     *  \param [in] value
     *         Any value associated with this GATT Characteristic.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* update_characteristic_value) \
              (
                  /* IN */ GA_BRR_SVC_INST sid,
                  /* IN */ GA_BRR_UUID char_uuid,
                  /* IN */ GA_BRR_CHR_INST cid,
                  /* IN */ GA_BRR_CHR_VAL * value
              );

    /**
     *  \brief Send response for a Characterisitc Write.
     *
     *  \par Description:
     *       This function enables to respond to a Characteritis Write
     *       from a peer device.
     *
     *  \param [in] device
     *         Peer Device Connection Identifier.
     *
     *  \param [in] ctx
     *         Assigned UUID of this GATT Characteristic.
     *
     *  \param [in] status
     *         Characteristic context ID.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* write_characteristic_rsp) \
              (
                  /* IN */ GA_BRR_DEVICE      * device,
                  /* IN */ GA_BRR_CHR_CONTEXT * ctx,
                  /* IN */ UINT16             status
              );

    /**
     *  \brief Send response for a Characterisitc Read.
     *
     *  \par Description:
     *       This function enables to respond to a Characteritis Read
     *       from a peer device.
     *
     *  \param [in] device
     *         Peer Device Connection Identifier.
     *
     *  \param [in] ctx
     *         Assigned UUID of this GATT Characteristic.
     *
     *  \param [in] status
     *         Characteristic context ID.
     *
     *  \param [in] value
     *         Any value associated with this GATT Characteristic.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* read_characteristic_rsp) \
              (
                  /* IN */ GA_BRR_DEVICE      * device,
                  /* IN */ GA_BRR_CHR_CONTEXT * ctx,
                  /* IN */ UINT16             status,
                  /* IN */ GA_BRR_CHR_VAL     * value
              );

    /*
     * Notify/Indicate value of the specified characteristic UUID for
     * the peer device
     */
    /**
     *  \brief Notify a Characterisitic Value to peer device.
     *
     *  \par Description:
     *       This function enables to send a charactertisic Notifications to
     *       peer device.
     *
     *  \param [in] device
     *         Peer Device Connection Identifier.
     *
     *  \param [in] sid
     *         Service context ID.
     *
     *  \param [in] char_uuid
     *         Assigned UUID of this GATT Characteristic.
     *
     *  \param [in] cid
     *         Characteristic context ID where needed, else can be
     *         \ref GA_BRR_INST_DONTCARE.
     *
     *  \param [in] value
     *         Any value associated with this GATT Characteristic.
     *
     *  \param [in] nwr
     *         TODO: Check what this is!
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* notify_characteristic) \
              (
                  /* IN */ GA_BRR_DEVICE   * device,
                  /* IN */ GA_BRR_SVC_INST sid,
                  /* IN */ GA_BRR_UUID     char_uuid,
                  /* IN */ GA_BRR_CHR_INST cid,
                  /* IN */ GA_BRR_CHR_VAL  * value,
                  /* IN */ UINT8           nwr
              );

    /*
     * Register a profile callback from GA and assign a profile ID for further
     * referencing.
     * Assign a profile ID or Profile ID gets assigned internally and comes
     * as Out?
     */
    /**
     *  \brief Register a profile client/collector.
     *
     *  \par Description:
     *       This function enables to Register a profile client/collector module
     *       with the GA Bearer.
     *
     *  \param [in] cb
     *         Profile Callback.
     *
     *  \param [out] pid
     *         Profile context ID.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* register_profile) \
              (
                  /* IN  */ UINT32 id,
                  /* IN  */ GA_BRR_PROFILE_CB cb,
                  /* OUT */ GA_BRR_PRF_HANDLE * pid
              );

    /**
     *  \brief To Unregister a GA Profile with GA Bearer.
     *
     *  \par Description:
     *       This function enables to unregister a profile with GA Bearer.
     *
     *  \param [in] pid
     *         Profile context ID.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* unregister_profile) (/* IN */ GA_BRR_PRF_HANDLE pid);


    /*
     * Enhance the bearer transport if required by the profile or application - eATT
     * Should the needed MTU(or other associated things incase of eATT) also be
     * part of this interface? Else, we might need some other interface for that.
     */
    /**
     *  \brief Update or Setup a new Transport.
     *
     *  \par Description:
     *       TODO: Need to revist for both Profile and Server roles.
     *
     *  \param [in] pid
     *         Profile context ID.
     *
     *  \param [in] type
     *         Type of Transport.
     *
     *  \param [in] enable
     *         State of Transport.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* setup_transport) \
              (
                  /* IN */ GA_BRR_PRF_HANDLE pid,
                  /* IN */ UINT8 type,
                  /* IN */ UINT8 enable
              );

    /*
     * Discover the service information from a profile instance for a service from
     * the peer device.
     */
    /**
     *  \brief Discover a peer GATT Service.
     *
     *  \par Description:
     *       This function enables to discover a Peer GATT Service.
     *
     *  \param [in] pid
     *         Profile context ID.
     *
     *  \param [in] device
     *         Peer Device Connection Identifier.
     *
     *  \param [in] svc_info
     *         Contains the assigned UUID of this GATT Primary Service to be looked
     *         for in remote. Or, contains the Service Handle Range where Included
     *         services are to be looked for in remote.
     *
     *  \param [in] disc_type
     *         Type of Service discovery to be performed in the remote.
     *          \ref GA_BRR_PS_DISCOVERY or \ref GA_BRR_IS_DISCOVERY
     *
     *  \param [in] context
     *         Any(Optional) Upper Layer provided context for Book-keeping.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* discover_service) \
              (
                  /* IN */ GA_BRR_PRF_HANDLE pid,
                  /* IN */ GA_BRR_DEVICE     * device,
                  /* IN */ GA_BRR_SVC_INFO   * svc_info,
                  /* IN */ UCHAR             disc_type,
                  /* IN */ void              * context
              );

    /*
     * Load the service information (characteristics and descriptors
     * of the specified service) and have it in the datastructure for
     * lookup during read/write.
     * - Should this have an associated service id?
     */
    /**
     *  \brief Discover composition of a given peer Service.
     *
     *  \par Description:
     *       This function enables to discover constituents of a given
     *       peer service.
     *
     *  \param [in] pid
     *         Profile context ID.
     *
     *  \param [in] device
     *         Peer Device Connection Identifier.
     *
     *  \param [in] service
     *         Service Related Information.
     *
     *  \param [in] context
     *         Any(Optional) Upper Layer provided context for Book-keeping.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* get_service_composition) \
              (
                  /* IN */ GA_BRR_PRF_HANDLE pid,
                  /* IN */ GA_BRR_DEVICE     * device,
                  /* IN */ GA_BRR_SVC_INFO   * service,
                  /* IN */ void              * context
              );

    /*
     * Load the characteristic information (descriptors
     * of the specified characteristic) and have it in the datastructure for
     * lookup during read/write.
     * - Should this have an associated characteristic id?
     */
    /**
     *  \brief Discover composition of a given peer Characteristic.
     *
     *  \par Description:
     *       This function enables to discover descriptors of a given
     *       peer characteristic.
     *
     *  \param [in] pid
     *         Profile context ID.
     *
     *  \param [in] device
     *         Peer Device Connection Identifier.
     *
     *  \param [in] characteristic
     *         Characterisitc Related Information.
     *
     *  \param [in] context
     *         Any(Optional) Upper Layer provided context for Book-keeping.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* get_characteristic_composition) \
              (
                  /* IN */ GA_BRR_PRF_HANDLE pid,
                  /* IN */ GA_BRR_DEVICE     * device,
                  /* IN */ GA_BRR_CHAR_RANGE * characteristic,
                  /* IN */ void              * context
              );

    /* Write value to the specified characteristic UUID for the peer device */
    /*
     * NOTE:
     * - Should this Interface also handle Prepare Write/ Reliable Write etc?
     * - Also, Will this interface need a Char Instance associated with it
     *   esp in case of multiple instance of same char?
     */
    /**
     *  \brief Write the Characterisitc Value.
     *
     *  \par Description:
     *       This function enables to Write value of a Peer GATT Characteristic.
     *
     *  \param [in] pid
     *         Profile context ID.
     *
     *  \param [in] device
     *         Peer Device Connection Identifier.
     *
     *  \param [in] characteristic
     *         Characteristic Handle to be Read.
     *
     *  \param [in] value
     *         Value to be written to the Characteristic
     *
     *  \param [in] wwr
     *         Flag to inform if Write Without Response or otherwise.
     *
     *  \param [in] context
     *         Any(Optional) Upper Layer provided context for Book-keeping.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* write_characteristic) \
              (
                  /* IN */ GA_BRR_PRF_HANDLE pid,
                  /* IN */ GA_BRR_DEVICE     * device,
                  /* IN */ GA_BRR_CHR_HANDLE characteristic,
                  /* IN */ GA_BRR_CHR_VAL    * value,
                  /* IN */ UINT8             wwr,
                  /* IN */ void              * context
              );

    /* Read value of the specified characteristic UUID for the peer device */
    /*
     * NOTE:
     * - Should this Interface take care of handling Read Long/Blob?
     *   Else, it might need an Offset too.
     * - Also, Will this interface need a Char Instance associated with it
     *   esp in case of multiple instance of same char?
     */
    /**
     *  \brief Read the Characterisitc Value.
     *
     *  \par Description:
     *       This function enables to read value of a Peer GATT Characteristic.
     *
     *  \param [in] pid
     *         Profile context ID.
     *
     *  \param [in] device
     *         Peer Device Connection Identifier.
     *
     *  \param [in] characteristic
     *         Characteristic Handle to be Read.
     *
     *  \param [in] context
     *         Any(Optional) Upper Layer provided context for Book-keeping.
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     */
    GA_RESULT (* read_characteristic) \
              (
                  /* IN */ GA_BRR_PRF_HANDLE  pid,
                  /* IN */ GA_BRR_DEVICE      *device,
                  /* IN */ GA_BRR_CHR_HANDLE  characteristic,
                  /* IN */ void               *context
              );

    /*
     * Configure notification/Indication enable/disable specified
     * characteristic UUID for the peer device.
     * NOTE:
     * - Either the state can be 0, 1, 2 or it can be TRUE/FALSE.
     *   If its the latter, then internally it should detect if its IND/NTF
     *   based on each character.
     * - Also, Will this interface need a Char Instance associated with it
     *   esp in case of multiple instance of same char?
     */

    /**
     *  \brief GA_brr_config_notification
     *
     *  \par Description
     *  This routine Configure notification/Indication enable/disable specified
     *  characteristic UUID for
     *  the peer device.
     *
     *  \param [in] pid
     *
     *  \param [in] device
     *
     *  \param [in] characteristic
     *  TODO: This needs to probably be changed to Descriptor/CCCD Handle.
     *
     *  \param [in] state
     *
     *  \param [in] context
     *
     *  \return \ref GA_SUCCESS or an error code indicating reason for failure
     *
     */
    GA_RESULT (* config_notification) \
              (
                  /* IN */ GA_BRR_PRF_HANDLE    pid,
                  /* IN */ GA_BRR_DEVICE        *device,
                  /* IN */ GA_BRR_CHR_HANDLE    characteristic,
                  /* IN */ UINT8                state,
                  /* IN */ void                 *context
              );

    /**
     *  \brief GA_brr_configure_adv
     *
     *  \par Description
     *  This routine Configures the BLE Advertisment
     *
     *  \param [in] pid
     *
     *  \param [in] type
     *
     *  \param [in] handle
     *
     *  \param [in] enable
     *
     *  \param [in] data
     *
     *  \param [in] datalen
     *
     *  \param [in] context
     *
     *  \return API_SUCCESS or an error code indicating reason for failure
     *
     */
    GA_RESULT (* config_adv)
              (
                  /* IN */ GA_BRR_PRF_HANDLE pid,
                  /* IN */ UINT8             type,
                  /* IN */ UINT8             handle,
                  /* IN */ UINT8             enable,
                  /* IN */ UCHAR             *data,
                  /* IN */ UINT8             datalen,
                  /* IN */ void              *context
              );

    /**
     *  \brief GA_brr_configure_scan
     *
     *  \par Description
     *  This routine Configures the BLE Scanning
     *
     *  \param [in] pid
     *
     *  \param [in] enable
     *
     *  \param [in] context
     *
     *  \return API_SUCCESS or an error code indicating reason for failure
     *
     */
    GA_RESULT (* config_scan)
              (
                  /* IN */ GA_BRR_PRF_HANDLE pid,
                  /* IN */ UINT8             enable,
                  /* IN */ void              *context
              );

    /**
     *  \brief GA_brr_configure_sync
     *
     *  \par Description
     *  This routine Configures the BLE scan syncing with PA
     *
     *  \param [in] pid
     *
     *  \param [in] handle
     *
     *  \param [in] device
     *
     *  \param [in] context
     *
     *  \return API_SUCCESS or an error code indicating reason for failure
     *
     */
    GA_RESULT (* config_sync)
              (
                  /* IN */ GA_BRR_PRF_HANDLE pid,
                  /* IN */ UINT16            handle,
                  /* IN */ GA_BRR_DEVICE     *device,
                  /* IN */ void              *context
              );

    /**
     *  \brief GA_brr_configure_broadcast
     *
     *  \par Description
     *  This routine Configures the BLE BIG for broadcast
     *
     *  \param [in] pid
     *
     *  \param [in] handle
     *
     *  \param [in] adv_handle
     *
     *  \param [in] param
     *
     *  \param [in] context
     *
     *  \return API_SUCCESS or an error code indicating reason for failure
     *
     */
    GA_RESULT (* config_broadcast)
              (
                  /* IN */ GA_BRR_PRF_HANDLE  pid,
                  /* IN */ UINT8              handle,
                  /* IN */ UINT8              adv_handle,
                  /* IN */ GA_BRR_BCAST_PARAM *param,
                  /* IN */ void               *context
              );

    /**
     *  \brief GA_brr_configure_broadcast_sync
     *
     *  \par Description
     *  This routine Configures the BLE BIG sync for broadcast
     *
     *  \param [in] pid
     *
     *  \param [in] handle
     *
     *  \param [in] sync_handle
     *
     *  \param [in] param
     *
     *  \param [in] context
     *
     *  \return API_SUCCESS or an error code indicating reason for failure
     *
     */
    GA_RESULT (* config_broadcast_sync)
              (
                  /* IN */ GA_BRR_PRF_HANDLE        pid,
                  /* IN */ UINT8                    handle,
                  /* IN */ UINT16                   sync_handle,
                  /* IN */ GA_BRR_BCAST_SYNC_PARAM  *param,
                  /* IN */ void                     *context
              );

    /**
     *  \brief GA_brr_dispatch_event
     *
     *  \par Description
     *  This routine dispatches the said event with parameters to the
     *  respective profile callback
     *
     *  \param [in] pid
     *
     *  \param [in] device
     *
     *  \param [in] event_params
     *
     *  \param [in] context
     *
     *  \return API_SUCCESS or an error code indicating reason for failure
     *
     */
    GA_RESULT (* dispatch_event)
              (
                  /* IN */ GA_BRR_PRF_HANDLE        pid,
                  /* IN */ GA_BRR_DEVICE           *device,
                  /* IN */ GA_BRR_EVENT_PARAMS     *event_params,
                  /* IN */ void                    *context
              );

} GA_BRR_INTERFACE;

/** \} */

/** \} */

/* --------------------------------------------- Macros */
/**
 * \addtogroup ga_brr_utility_macros
 * \{
 */

/** Check for connectionless event code */
#define GA_BRR_ISEVENT_CONNECTIONLESS(ev) \
    (GA_BRR_CONNECTIONLESS_EVENT_CODE == (GA_BRR_CONNECTIONLESS_EVENT_CODE & (ev)))

/** Compare Bearer Device address type */
#define GA_COMPARE_BRR_DEV_ADDR_TYPE(type_a,type_b)\
        (((type_a) == (type_b))?GA_TRUE:GA_FALSE)

/** Compare Bearer Device address */
#define GA_COMPARE_BRR_DEV_ADDR(dev_a,dev_b)\
        ((0 == GA_mem_cmp((dev_a), (dev_b), GA_BD_ADDR_SIZE))?GA_TRUE:GA_FALSE)

/**
 * Compare Bearer Device.
 * It compares device address and address type
 */
#define GA_COMPARE_BRR_DEVICE(dev_a,dev_b)\
        ((GA_COMPARE_BRR_DEV_ADDR_TYPE((dev_a)->bd_type,(dev_b)->bd_type)) &&\
         (GA_COMPARE_BRR_DEV_ADDR((dev_a)->bd_addr,(dev_b)->bd_addr)))

/** Initialize Bearer Device */
#define GA_INIT_BRR_DEVICE(dev) \
        GA_mem_set ((dev)->bd_addr, 0, GA_BD_ADDR_SIZE); \
        (dev)->bd_type = 0x00

/** Check Bearer Device address for Non-Zero */
#define GA_BRR_DEV_ADDR_IS_NON_ZERO(addr)\
        ((0x00 == (*((addr) + 0) | *((addr) + 1) | *((addr) + 2) | *((addr) + 3) | *((addr) + 4) | *((addr) + 5)))?\
        GA_FALSE:GA_TRUE)

/** Bearer Device address */
#define GA_BRR_DEV_ADDR(dev) (dev)->bd_addr

/** Bearer Device address type */
#define GA_BRR_DEV_ADDR_TYPE(dev) (dev)->bd_type

/** Copy Bearer Device address */
#define GA_COPY_BRR_DEV_ADDR(dest, src) \
        GA_mem_copy ((dest), (src), GA_BD_ADDR_SIZE);

/** Copy Bearer Device type */
#define GA_COPY_BRR_DEV_TYPE(dest,src)\
        (dest) = (src);

/**
 * Copy Bearer Device.
 * It copies Bearer device type and address.
 */
#define GA_COPY_BRR_DEVICE(dest,src)\
        GA_COPY_BRR_DEV_TYPE((dest)->bd_type,(src)->bd_type)\
        GA_COPY_BRR_DEV_ADDR((dest)->bd_addr,(src)->bd_addr)

/** Bearer Device format specifier */
#define GA_BRR_DEVICE_FRMT_SPECIFIER\
        "ADDR: %02X:%02X:%02X:%02X:%02X:%02X, TYPE: %02X"

/** Prints device address and type */
#define GA_BRR_DEVICE_PRINT_STR(ref)\
        (ref)->bd_addr[0],(ref)->bd_addr[1],(ref)->bd_addr[2],\
        (ref)->bd_addr[3],(ref)->bd_addr[4],(ref)->bd_addr[5],\
        (ref)->bd_type

/** \} */

/** \} */

/* --------------------------------------------- APIs */

/**
 * \defgroup ga_brr_api_defs API Definitions
 * \{
 * \brief This section describes the EtherMind Generic Audio Bearer APIs.
 */

/**
 * \defgroup ga_brr_server_api_defs Bearer Server APIs
 * \{
 * \brief This section describes the Generic Audio Bearer APIs for Server.
 */

/**
 * \name Bearer Server APIs - Init
 * \{
 * \brief This section describes the Generic Audio Bearer Init APIs for Server.
 */

/**
 *  \brief To Register a BLE bearer to the GA Layer.
 *
 *  \par Description:
 *       This function enables to register the BLE bearer to GA Bearer Layer.
 *
 *  \param [in] iface
 *         The interface structure with assigned functions to different interfaces
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_register(GA_BRR_INTERFACE * iface);

/** \} */

/** \} */


/**
 * \defgroup ga_brr_common_api_defs Bearer Common APIs
 * \{
 * \brief This section describes the Generic Audio Bearer APIs for both Client
 * and Server.
 */

/**
 * \name Bearer Common API Definitions - Init
 * \{
 * \brief This section describes the Generic Audio Bearer Common Init APIs.
 */

/**
 *  \brief To Init the GA Bearer Layer.
 *
 *  \par Description:
 *       This function enables to inilialize the GA Bearer Layer.
 *
 *  \param [in] cb
 *         Common callback of GA core
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_init(GA_BRR_CB cb);

/** \} */

/**
 * \name Bearer Common API Definitions - Shutdown
 * \{
 * \brief This section describes the Generic Audio Bearer Common Shutdown APIs.
 */

/**
 *  \brief To Shutdown the GA Bearer Layer.
 *
 *  \par Description:
 *       This function enables to shutdown the GA Bearer Layer.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_shutdown(void);

/** \} */

/** \} */

/**
 * \addtogroup ga_brr_server_api_defs
 * \{
 */

/**
 * \name Bearer Server APIs - Setup
 * \{
 * \brief This section describes the Generic Audio Bearer Setup APIs for Server.
 */

/**
 *  \brief Add a GATT Service.
 *
 *  \par Description:
 *       This function enables to add a GATT Service with the GA Bearer.
 *
 *  \param [in] service_uuid
 *         Assigned UUID of this GATT Service.
 *
 *  \param [in] service_type
 *         Type associated with the service.
 *
 *  \param [in] cb
 *         Callback associated with Service ID.
 *
 *  \param [out] sid
 *         Service context ID.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_add_service
          (
              /* IN  */ GA_BRR_UUID       service_uuid,
              /* IN  */ UINT8             service_type,
              /* IN  */ GA_BRR_SERVICE_CB cb,
              /* OUT */ GA_BRR_SVC_INST   * sid
          );

/** \} */

/**
 * \name Bearer Server APIs - Release
 * \{
 * \brief This section describes the Generic Audio Bearer Release APIs for
 * Server.
 */

/**
 *  \brief Remove a GATT Service.
 *
 *  \par Description:
 *       This function enables to remove a GATT Service with the GA Bearer.
 *
 *  \param [in] sid
 *         Service context ID.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_remove_service
          (
              /* IN */ GA_BRR_SVC_INST sid
          );

/** \} */

/**
 * \name Bearer Server APIs - Setup
 * \{
 * \brief This section describes the Generic Audio Bearer Setup APIs for Server.
 */

/**
 *  \brief Add a Included GATT Service.
 *
 *  \par Description:
 *       This function enables to add a Included GATT Service with the
 *       GA Bearer.
 *
 *  \param [in] sid
 *         Service context ID.
 *
 *  \param [in] service_uuid
 *         Assigned UUID of this GATT Service.
 *
 *  \param [in] service_state
 *         Initial state of the Included service.
 *         Enabled (GA_TRUE) or Disabled (GA_FALSE)
 *
 *  \param [in] isid
 *         Included Service context ID.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_include_service
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ GA_BRR_UUID     service_uuid,
              /* IN  */ UINT8           service_state,
              /* IN */ GA_BRR_SVC_INST * isid
          );

/**
 *  \brief Add a GATT Characteristic to a Service.
 *
 *  \par Description:
 *       This function enables to add a GATT Characteristic to a given
 *       Service with the GA Bearer.
 *
 *  \param [in] sid
 *         Service context ID.
 *
 *  \param [in] char_uuid
 *         Assigned UUID of this GATT Characteristic.
 *
 *  \param [in] char_property
 *         Properties associated with this GATT Characteristic.
 *
 *  \param [in] value
 *         Any value associated with this GATT Characteristic.
 *
 *  \param [out] cid
 *         Characteristic context ID.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_add_characteristic
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ GA_BRR_UUID     char_uuid,
              /* IN  */ UINT16          char_property,
              /* IN  */ GA_BRR_CHR_VAL  * value,
              /* OUT */ GA_BRR_CHR_INST * cid
          );

/**
 *  \brief Enable/Disable a GATT Service.
 *
 *  \par Description:
 *       This function enables/disables GATT Service.
 *
 *  \param [in] sid
 *         Service ID
 *
 *  \param [in] iid
 *         Including Service ID, if sid is of an included service.
 *         GA_BRR_SERVICE_ID_INVALID if not applicable
 *
 *  \param [in] state
 *         Enable (GA_TRUE) or Disable (GA_FALSE).
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_manage_service
          (
              /* IN  */ UINT8 sid,
              /* IN  */ UINT8 iid,
              /* IN  */ UINT8 state
          );

/**
 *  \brief Enable a GATT Service.
 *
 *  \par Description:
 *       This function enables GATT Service.
 *
 *  \param [in] sid
 *         Service ID
 *
 *  \param [in] iid
 *         Including Service ID, if sid is of an included service.
 *         GA_BRR_SERVICE_ID_INVALID if not applicable
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_brr_enable_service(sid, iid) \
    GA_brr_manage_service((sid), (iid), GA_TRUE)

/**
 *  \brief Disable a GATT Service.
 *
 *  \par Description:
 *       This function disables GATT Service.
 *
 *  \param [in] sid
 *         Service ID
 *
 *  \param [in] iid
 *         Including Service ID, if sid is of an included service.
 *         GA_BRR_SERVICE_ID_INVALID if not applicable
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_brr_disable_service(sid, iid) \
    GA_brr_manage_service((sid), (iid), GA_FALSE)

/**
 *  \brief Enable/Disable a GATT Characteristic.
 *
 *  \par Description:
 *       This function enables/disables GATT Characteristic.
 *
 *  \param [in] sid
 *         Service ID
 *
 *  \param [in] cid
 *         Characteristic ID
 *
 *  \param [in] state
 *         Enable (GA_TRUE) or Disable (GA_FALSE).
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_manage_characteristic
          (
              /* IN  */ UINT8 sid,
              /* IN  */ UINT8 cid,
              /* IN  */ UINT8 state
          );

/**
 *  \brief Enable a GATT Characteristic.
 *
 *  \par Description:
 *       This function enables GATT Characteristic.
 *
 *  \param [in] sid
 *         Service ID
 *
 *  \param [in] id
 *         Characteristic ID
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_brr_enable_characteristic(sid, cid) \
    GA_brr_manage_characteristic((sid), (cid), GA_TRUE)

/**
 *  \brief Disable a GATT Characteristic.
 *
 *  \par Description:
 *       This function disables GATT Characteristic.
 *
 *  \param [in] sid
 *         Service ID
 *
 *  \param [in] cid
 *         Service/Characteristic ID
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_brr_disable_characteristic(sid, cid) \
    GA_brr_manage_characteristic((sid), (cid), GA_FALSE)

/** \} */

/**
 * \name Bearer Server APIs - Read/Write
 * \{
 * \brief This section describes the Generic Audio Bearer Read/Write APIs
 * for Server.
 */

/*
 * Provision to update the characteric value
 */
/**
 *  \brief Update a GATT Characteristic to a Service.
 *
 *  \par Description:
 *       This function enables to Update a GATT Characteristic to a given
 *       Service with the GA Bearer.
 *
 *  \param [in] sid
 *         Service context ID.
 *
 *  \param [in] char_uuid
 *         Assigned UUID of this GATT Characteristic.
 *
 *  \param [in] cid
 *         Characteristic context ID.
 *
 *  \param [in] value
 *         Any value associated with this GATT Characteristic.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_update_characteristic_value
          (
              /* IN */ GA_BRR_SVC_INST sid,
              /* IN */ GA_BRR_UUID     char_uuid,
              /* IN */ GA_BRR_CHR_INST cid,
              /* IN */ GA_BRR_CHR_VAL  * value
          );

/**
 *  \brief Send response for a Characterisitc Write.
 *
 *  \par Description:
 *       This function enables to respond to a Characteritis Write
 *       from a peer device.
 *
 *  \param [in] device
 *         Peer Device Connection Identifier.
 *
 *  \param [in] ctx
 *         Assigned UUID of this GATT Characteristic.
 *
 *  \param [in] status
 *         Characteristic context ID.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_write_characteristic_rsp
          (
              /* IN */ GA_BRR_DEVICE      * device,
              /* IN */ GA_BRR_CHR_CONTEXT * ctx,
              /* IN */ UINT16             status
          );

/**
 *  \brief Send response for a Characterisitc Read.
 *
 *  \par Description:
 *       This function enables to respond to a Characteritis Read
 *       from a peer device.
 *
 *  \param [in] device
 *         Peer Device Connection Identifier.
 *
 *  \param [in] ctx
 *         Assigned UUID of this GATT Characteristic.
 *
 *  \param [in] status
 *         Characteristic context ID.
 *
 *  \param [in] value
 *         Any value associated with this GATT Characteristic.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_read_characteristic_rsp
          (
              /* IN */ GA_BRR_DEVICE      * device,
              /* IN */ GA_BRR_CHR_CONTEXT * ctx,
              /* IN */ UINT16             status,
              /* IN */ GA_BRR_CHR_VAL     * value
          );

/** \} */

/**
 * \name Bearer Server APIs - Notify
 * \{
 * \brief This section describes the Generic Audio Bearer Notify APIs
 * for Server.
 */

/* Notify/Indicate value of the specified characteristic UUID for the peer device */
/**
 *  \brief Notify a Characterisitic Value to peer device.
 *
 *  \par Description:
 *       This function enables to send a charactertisic Notifications to
 *       peer device.
 *
 *  \param [in] device
 *         Peer Device Connection Identifier.
 *
 *  \param [in] sid
 *         Service context ID.
 *
 *  \param [in] char_uuid
 *         Assigned UUID of this GATT Characteristic.
 *
 *  \param [in] cid
 *         Characteristic context ID where needed, else can be
 *         \ref GA_BRR_INST_DONTCARE.
 *
 *  \param [in] value
 *         Any value associated with this GATT Characteristic.
 *
 *  \param [in] nwr
 *         TODO: Check what this is!
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_notify_characteristic
          (
              /* IN */ GA_BRR_DEVICE   * device,
              /* IN */ GA_BRR_SVC_INST sid,
              /* IN */ GA_BRR_UUID     char_uuid,
              /* IN */ GA_BRR_CHR_INST cid,
              /* IN */ GA_BRR_CHR_VAL  * value,
              /* IN */ UINT8           nwr
          );

/** \} */

/** \} */

/**
 * \defgroup ga_brr_client_api_defs Bearer Client APIs
 * \{
 * \brief This section describes the Generic Audio Bearer APIs for Client.
 */

/**
 * \name Bearer Client APIs - Init
 * \{
 * \brief This section describes the Generic Audio Bearer Init APIs for Client.
 */

/*
 * Register a profile callback from GA and assign a profile ID for further
 * referencing.
 * Assign a profile ID or Profile ID gets assigned internally and comes
 * as Out?
 */
/**
 *  \brief Register a profile client/collector.
 *
 *  \par Description:
 *       This function enables to Register a profile client/collector module
 *       with the GA Bearer.
 *
 *  \param [in] cb
 *         Profile Callback.
 *
 *  \param [out] pid
 *         Profile context ID.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_register_profile
          (
              /* IN  */ UINT32 id,
              /* IN  */ GA_BRR_PROFILE_CB  cb,
              /* OUT */ GA_BRR_PRF_HANDLE * pid
          );

/** \} */

/**
 * \name Bearer Client APIs - De-Init
 * \{
 * \brief This section describes the Generic Audio Bearer De-Init APIs for
 * Client.
 */

/**
 *  \brief To Unregister a GA Profile with GA Bearer.
 *
 *  \par Description:
 *       This function enables to unregister a profile with GA Bearer.
 *
 *  \param [in] pid
 *         Profile context ID.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_unregister_profile
          (
              /* IN */ GA_BRR_PRF_HANDLE pid
          );

/** \} */

/**
 * \name Bearer Client APIs - Setup
 * \{
 * \brief This section describes the Generic Audio Bearer Setup APIs
 * for Client.
 */

/*
 * Enhance the bearer transport if required by the profile or application - eATT
 * Should the needed MTU(or other associated things incase of eATT) also be
 * part of this interface? Else, we might need some other interface for that.
 */
/**
 *  \brief Update or Setup a new Transport.
 *
 *  \par Description:
 *       TODO: Need to revist for both Profile and Server roles.
 *
 *  \param [in] pid
 *         Profile context ID.
 *
 *  \param [in] type
 *         Type of Transport.
 *
 *  \param [in] enable
 *         State of Transport.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_setup_transport
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ UINT8             type,
              /* IN */ UINT8             enable
          );

/*
 * Discover the service information from a profile instance for a service from
 * the peer device.
 */
/**
 *  \brief Discover a peer GATT Service.
 *
 *  \par Description:
 *       This function enables to discover a Peer GATT Service.
 *
 *  \param [in] pid
 *         Profile context ID.
 *
 *  \param [in] device
 *         Peer Device Connection Identifier.
 *
 *  \param [in] svc_info
 *         Contains the assigned UUID of this GATT Primary Service to be looked
 *         for in remote. Or, contains the Service Handle Range where Included
 *         services are to be looked for in remote.
 *
 *  \param [in] disc_type
 *         Type of Service discovery to be performed in the remote.
 *          \ref GA_BRR_PS_DISCOVERY or \ref GA_BRR_IS_DISCOVERY
 *
 *  \param [in] context
 *         Any(Optional) Upper Layer provided context for Book-keeping.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_discover_service
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_SVC_INFO   * svc_info,
              /* IN */ UCHAR             disc_type,
              /* IN */ void              * context
          );

/*
 * Discover the primary service information from a profile instance for a
 * service from the peer device.
 */
/**
 *  \brief Discover a peer GATT Primary Service.
 *
 *  \par Description:
 *       This function enables to discover a Peer GATT Service with a given
 *       UUID present as a Primary Service in the Peer GATT DB.
 *
 *  \param [in] pid
 *         Profile context ID.
 *
 *  \param [in] device
 *         Peer Device Connection Identifier.
 *
 *  \param [in] svc_info
 *         Contains the assigned UUID of this GATT Primary Service to be looked
 *         for in remote.
 *
 *  \param [in] context
 *         Any(Optional) Upper Layer provided context for Book-keeping.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_brr_discover_ps(pid, device, svc_info, context)\
        GA_brr_discover_service\
        (\
            (pid),\
            (device),\
            (svc_info),\
            GA_BRR_PS_DISCOVERY,\
            (context)\
        );

/*
 * Discover the included service information from a profile instance for a
 * service range from the peer device.
 */
/**
 *  \brief Discover a peer GATT Included Service.
 *
 *  \par Description:
 *       This function enables to discover a Peer GATT Services in a given
 *       Service range present as Included Service in the Peer GATT DB.
 *
 *  \param [in] pid
 *         Profile context ID.
 *
 *  \param [in] device
 *         Peer Device Connection Identifier.
 *
 *  \param [in] svc_info
 *         Contains the Service Handle Range where Included services are to
 *         be looked for in remote.
 *
 *  \param [in] context
 *         Any(Optional) Upper Layer provided context for Book-keeping.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_brr_discover_is(pid, device, svc_info, context)\
        GA_brr_discover_service\
        (\
            (pid),\
            (device),\
            (svc_info),\
            GA_BRR_IS_DISCOVERY,\
            (context)\
        );

/*
 * Discover the secondary service information from a profile instance for a
 * service from the peer device.
 */
/**
 *  \brief Discover a peer GATT Secondary Service.
 *
 *  \par Description:
 *       This function enables to discover a Peer GATT Service with a given
 *       UUID present as a Secondary Service in the Peer GATT DB.
 *
 *  \param [in] pid
 *         Profile context ID.
 *
 *  \param [in] device
 *         Peer Device Connection Identifier.
 *
 *  \param [in] svc_info
 *         Contains the assigned UUID of this GATT Secondary Service to be looked
 *         for in remote.
 *
 *  \param [in] context
 *         Any(Optional) Upper Layer provided context for Book-keeping.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_brr_discover_ss(pid, device, svc_info, context)\
        GA_brr_discover_service\
        (\
            (pid),\
            (device),\
            (svc_info),\
            GA_BRR_SS_DISCOVERY,\
            (context)\
        );

/*
 * Load the service information (characteristics and descriptors
 * of the specified service) and have it in the datastructure for
 * lookup during read/write.
 * - Should this have an associated service id?
 */
/**
 *  \brief Discover composition of a given peer Service.
 *
 *  \par Description:
 *       This function enables to discover constituents of a given
 *       peer service.
 *
 *  \param [in] pid
 *         Profile context ID.
 *
 *  \param [in] device
 *         Peer Device Connection Identifier.
 *
 *  \param [in] service
 *         Service Related Information.
 *
 *  \param [in] context
 *         Any(Optional) Upper Layer provided context for Book-keeping.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_get_service_composition
          (
              /* IN */ GA_BRR_PRF_HANDLE   pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_SVC_INFO   * service,
              /* IN */ void              * context
          );

/*
 * Load the characteristic information (descriptors
 * of the specified characteristic) and have it in the datastructure for
 * lookup during read/write.
 * - Should this have an associated characterisitc id?
 */
/**
 *  \brief Discover composition of a given peer Characteristic.
 *
 *  \par Description:
 *       This function enables to discover constituents of a given
 *       peer characteristic.
 *
 *  \param [in] pid
 *         Profile context ID.
 *
 *  \param [in] device
 *         Peer Device Connection Identifier.
 *
 *  \param [in] characteristic
 *         Characteristic Related Information.
 *
 *  \param [in] context
 *         Any(Optional) Upper Layer provided context for Book-keeping.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_get_characteristic_composition
          (
              /* IN */ GA_BRR_PRF_HANDLE   pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHAR_RANGE * characteristic,
              /* IN */ void              * context
          );

/** \} */

/**
 * \name Bearer Client APIs - Write
 * \{
 * \brief This section describes the Generic Audio Bearer Write APIs for Client.
 */

/* Write value to the specified characteristic UUID for the peer device */
/*
 * NOTE:
 * - Should this Interface also handle Prepare Write/ Reliable Write etc?
 * - Also, Will this interface need a Char Instance associated with it
 *   esp in case of multiple instance of same char?
 */
/**
 *  \brief Write the Characterisitc Value.
 *
 *  \par Description:
 *       This function enables to Write value of a Peer GATT Characteristic.
 *
 *  \param [in] pid
 *         Profile context ID.
 *
 *  \param [in] device
 *         Peer Device Connection Identifier.
 *
 *  \param [in] characteristic
 *         Characteristic Handle to be Read.
 *
 *  \param [in] value
 *         Value to be written to the Characteristic
 *
 *  \param [in] wwr
 *         Flag to inform if Write Without Response or otherwise.
 *
 *  \param [in] context
 *         Any(Optional) Upper Layer provided context for Book-keeping.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_write_characteristic
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ GA_BRR_CHR_VAL    * value,
              /* IN */ UINT8             wwr,
              /* IN */ void              * context
          );

/** \} */

/**
 * \name Bearer Client APIs - Read
 * \{
 * \brief This section describes the Generic Audio Bearer Read APIs for Client.
 */

/* Read value of the specified characteristic UUID for the peer device */
/*
 * NOTE:
 * - Should this Interface take care of handling Read Long/Blob?
 *   Else, it might need an Offset too.
 * - Also, Will this interface need a Char Instance associated with it
 *   esp in case of multiple instance of same char?
 */
/**
 *  \brief Read the Characterisitc Value.
 *
 *  \par Description:
 *       This function enables to read value of a Peer GATT Characteristic.
 *
 *  \param [in] pid
 *         Profile context ID.
 *
 *  \param [in] device
 *         Peer Device Connection Identifier.
 *
 *  \param [in] characteristic
 *         Characteristic Handle to be Read.
 *
 *  \param [in] context
 *         Any(Optional) Upper Layer provided context for Book-keeping.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_brr_read_characteristic
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ void              * context
          );

/** \} */

/**
 * \name Bearer Client APIs - Setup
 * \{
 * \brief This section describes the Generic Audio Bearer Setup APIs for
 * Client.
 */

/*
 * Configure notification/Indication enable/disable specified characteristic UUID for
 * the peer device.
 * NOTE:
 * - Either the state can be 0, 1, 2 or it can be TRUE/FALSE.
 *   If its the latter, then internally it should detect if its IND/NTF
 *   based on each character.
 * - Also, Will this interface need a Char Instance associated with it
 *   esp in case of multiple instance of same char?
 */

/**
 *  \brief GA_brr_config_notification
 *
 *  \par Description
 *  This routine Configure notification/Indication enable/disable specified
 *  characteristic UUID for
 *  the peer device.
 *
 *  \param [in] pid
 *
 *  \param [in] device
 *
 *  \param [in] characteristic
 *  TODO: This needs to probably be changed to Descriptor/CCCD Handle.
 *
 *  \param [in] state
 *
 *  \param [in] context
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 */
GA_RESULT GA_brr_config_notification
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ UINT8             state,
              /* IN */ void              * context
          );

/**
 *  \brief GA_brr_config_indication
 *
 *  \par Description
 *  This routine Configure Indication enable/disable specified
 *  characteristic UUID for the peer device.
 *
 *  \param [in] pid
 *
 *  \param [in] device
 *
 *  \param [in] characteristic
 *  TODO: This needs to probably be changed to Descriptor/CCCD Handle.
 *
 *  \param [in] state
 *
 *  \param [in] context
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 */
GA_RESULT GA_brr_config_indication
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ UINT8             state,
              /* IN */ void              * context
          );

GA_RESULT GA_brr_configure_adv
          (
              GA_BRR_PRF_HANDLE pid,
              UINT8   type,
              UINT8   handle,
              UINT8   enable,
              UCHAR * data,
              UINT8   datalen,
              void  * context
          );

GA_RESULT GA_brr_configure_scan
          (
              GA_BRR_PRF_HANDLE   pid,
              UCHAR               enable,
              void              * context
          );

GA_RESULT GA_brr_configure_sync
          (
              GA_BRR_PRF_HANDLE pid,
              UINT16            handle,
              GA_BRR_DEVICE   * device,
              void            * context
          );

GA_RESULT GA_brr_configure_broadcast
          (
              GA_BRR_PRF_HANDLE pid,
              UCHAR   handle,
              UCHAR   adv_handle,
              GA_BRR_BCAST_PARAM * param,
              void  * context
          );

GA_RESULT GA_brr_configure_broadcast_sync
          (
              GA_BRR_PRF_HANDLE pid,
              UCHAR   handle,
              UINT16  sync_handle,
              GA_BRR_BCAST_SYNC_PARAM * param,
              void  * context
          );

GA_RESULT GA_brr_dispatch_event
          (
              /* IN */ GA_BRR_PRF_HANDLE     pid,
              /* IN */ GA_BRR_DEVICE       * device,
              /* IN */ GA_BRR_EVENT_PARAMS * event_params,
              /* IN */ void                * context
          );

/** \} */

/** \} */

/** \} */

/** \} */
/** \} */

#endif /* _H_GA_BEARER_API_ */

