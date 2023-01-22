
/**
 *  \file BT_gatt_db_api.h
 *
 *  \brief Module API Header File contains structure definitions and API definitions
 *  offered to the application.
 */

/*
 *  Copyright (C) 2014. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_GATT_DB_API_
#define _H_BT_GATT_DB_API_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_att_api.h"
#include "gatt_defines.h"

/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/** Currently defining this here, need to move this to features later */
#define GATT_DB_UUID_SEARCH_SUPPORT

/**
 * \defgroup gatt_db_module GATT DB (Generic Attribute Profile Database)
 * \{
 *  This section describes the interfaces & APIs offered by
 *  Generic Attribute Profile Database (GATT_DB) module to the Application and
 *  other upper layers of the stack. GATT Procedures are
 *  realized using ATT APIs. This module abstracts the Server Database.
 */


/**
 *  \defgroup gatt_db_defines Defines
 *  \{
 *  This section describes the various defines for the module.
 */

/**
 *  \defgroup gatt_db_access_operations Access Operations
 *  \{
 *  These abstract access functions for attributes, including read, write,
 *  prepare. The direction of update is also included as this decides
 *  checking needed security for peer and if values are editable by the peer.
 *  Please also note the 'Update' access which is used for local updates of
 *  values based on \ref GATT_DB_HANDLE as against Attribute Handle.
 */

/** Read operation for Attribute  */
#define GATT_DB_READ                            0x01U

/** Write operation for Attribute */
#define GATT_DB_WRITE                           0x02U

/** Read blob operation for Attribute */
#define GATT_DB_READ_BLOB                       0x03U

/** Write Without Response operation for Attribute */
#define GATT_DB_WRITE_WITHOUT_RSP               0x04U

/** Read by UUID/Type operation for Attribute */
#define GATT_DB_READ_BY_TYPE                    0x05U

/** Signed Write operation for Attribute */
#define GATT_DB_SIGNED_WRITE                    0x06U

/** Execute Write operation for Attribute */
#define GATT_DB_EXECUTE                         0x07U

/** Prepare Write operation for Attribute */
#define GATT_DB_PREPARE                         0x08U

/**
 *  Local update of a Readable Attribute Value using \ref GATT_DB_HANDLE instead
 *  of Attribute Handle. See \ref BT_gatt_db_set_char_val for more details.
 */
#define GATT_DB_UPDATE                          0x10U

/* 128 Bit Attribute UUID  */
#define GATT_DB_128_BIT_UUID_FORMAT             0x20U

/**
 *  Peer initiated operation, needed when Peer is Reading or Writing Values
 *  either using GATT Read Procedures for Value and Descriptors.
 */
#define GATT_DB_PEER_INITIATED                  0x80U

/**
 *  Locally initiated operation, needed for local updates based on Attribute
 *  Handles. In case \ref GATT_DB_HANDLE is known, use of access through
 *  \ref GATT_DB_UPDATE is recommended.
 */
#define GATT_DB_LOCALLY_INITIATED               0x00U

/** \} */

/**
 *  \defgroup gatt_db_operations Operations Notified In Characteristic Callback
 *  \{
 *  This section describes the operations notified by the module to the
 *  application through the callback registered by the application.
 */

/** Characteristic Value Local Read Operation */
#define GATT_DB_CHAR_VALUE_READ_REQ             0x01U

/** Characteristic Value Local Write Operation */
#define GATT_DB_CHAR_VALUE_WRITE_REQ            0x02U

/** Characteristic Value Local Write Without Response Operation */
#define GATT_DB_CHAR_VALUE_WRITE_WITHOUT_REQ    0x03U

/** Characteristic Client Configuration Local Read Operation */
#define GATT_DB_CHAR_CLI_CNFG_READ_REQ          0x11U

/** Characteristic Client Configuration Local Write Operation */
#define GATT_DB_CHAR_CLI_CNFG_WRITE_REQ         0x12U

/** Characteristic Server Configuration Local Read Operation */
#define GATT_DB_CHAR_SER_CNFG_READ_REQ          0x21U

/** Characteristic Server Configuration Local Write Operation */
#define GATT_DB_CHAR_SER_CNFG_WRITE_REQ         0x22U

/** Characteristic Value Peer Read Operation */
#define GATT_DB_CHAR_PEER_READ_REQ              (GATT_DB_PEER_INITIATED | GATT_DB_READ)

/** Characteristic Value Peer Write Operation */
#define GATT_DB_CHAR_PEER_WRITE_REQ             (GATT_DB_PEER_INITIATED | GATT_DB_WRITE)

/** Characteristic Value Peer Read Blob Operation */
#define GATT_DB_CHAR_PEER_READ_BLOB_REQ         (GATT_DB_PEER_INITIATED | GATT_DB_READ_BLOB)

/** Characteristic Value Peer Write Command */
#define GATT_DB_CHAR_PEER_WRITE_CMD             (GATT_DB_PEER_INITIATED | GATT_DB_WRITE_WITHOUT_RSP)

/** Characteristic Value Peer Read by Type Operation */
#define GATT_DB_CHAR_PEER_READ_BY_TYPE_REQ      (GATT_DB_PEER_INITIATED | GATT_DB_READ_BY_TYPE)

/** Characteristic Value Peer Signed Write Operation */
#define GATT_DB_CHAR_PEER_SIGNED_WRITE_CMD      (GATT_DB_PEER_INITIATED | GATT_DB_SIGNED_WRITE)

/** Characteristic Value Peer Execute Write Req */
#define GATT_DB_CHAR_PEER_EXECUTE_WRITE_REQ     (GATT_DB_PEER_INITIATED | GATT_DB_EXECUTE)

/** Characteristic Value Peer Prepare Write Req */
#define GATT_DB_CHAR_PEER_PREPARE_WRITE_REQ     (GATT_DB_PEER_INITIATED | GATT_DB_PREPARE)

/** Characteristic Client Configuration Peer Read Operation */
#define GATT_DB_CHAR_PEER_CLI_CNFG_READ_REQ     0x91U

/** Characteristic Client Configuration Peer Write Operation */
#define GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ    0x92U

/** Characteristic Server Configuration Peer Read Operation */
#define GATT_DB_CHAR_PEER_SER_CNFG_READ_REQ     0xA1U

/** Characteristic Server Configuration Peer Write Operation */
#define GATT_DB_CHAR_PEER_SER_CNFG_WRITE_REQ    0xA2U

/** Characteristic User Description Peer Read Operation */
#define GATT_DB_CHAR_PEER_USR_DESC_READ_REQ     0xB1U

/** Characteristic User Description Peer Write Operation */
#define GATT_DB_CHAR_PEER_USR_DESC_WRITE_REQ    0xB2U

/** Characteristic Higher Layer Defined Descriptor Peer Read Operation */
#define GATT_DB_CHAR_PEER_HLD_DESC_READ_REQ     0xF1U

/** Characteristic Higher Layer Defined Descriptor Peer Write Operation */
#define GATT_DB_CHAR_PEER_HLD_DESC_WRITE_REQ    0xF2U

/** \} */

/**
 *  \defgroup gatt_characteristic_properties Characteristic Properties
 *  \{
 *  Characteristic Properties - Combination of these properties (combined
 *  using bitwise OR) describe properties of the Characteristic. Also
 *  see \ref GATT_DB_ATTR_LIST.
 */

/**
 *  Characteristic support Broadcast of its value to the peer.
 *  Setting this property automatically includes Characteristic Server
 *  Configuration Descriptor to the Characteristic
 */

/* Characteristic supports no Reading or Writing on its value by peer */
#define GATT_DB_CHAR_NONE_PROPERTY                     0x00000000U

/**
 * Characteristic supports Broadcasting its value to the peer.
 * Setting this property implies there needs to be Sever Characteristic
 * Configuration Descriptor to this Characteristic.
 */
#define GATT_DB_CHAR_BROADCAST_PROPERTY                0x00000001U

/** Characteristic support Reading its value by peer */
#define GATT_DB_CHAR_READ_PROPERTY                     0x00000002U

/** Characteristic support Writing its value by peer Without Response  */
#define GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY        0x00000004U

/** Characteristic supports Writing its value by peer */
#define GATT_DB_CHAR_WRITE_PROPERTY                    0x00000008U

/**
 * Characteristic supports Notifying its value to the peer.
 * Setting this property implies there needs to be a Client Characteristic
 * Configuration Descriptor to this Characteristic.
 */
#define GATT_DB_CHAR_NOTIFY_PROPERTY                   0x00000010U

/**
 * Characteristic supports Indicating its value to the peer.
 * Setting this property implies there needs to be a Client Characteristic
 * Configuration Descriptor to this Characteristic.
 */
#define GATT_DB_CHAR_INDICATE_PROPERTY                 0x00000020U

/** Characteristic supports Signed Write on its value */
#define GATT_DB_CHAR_SIGNED_WRITE_PROPERTY             0x00000040U

/**
 * Characteristic supports Write on its descriptor
 * Setting this property automatically includes Characteristic
 * Extended Properties Descriptor to the Characteristic
 */
#define GATT_DB_CHAR_EXTENDED_PROPERTY                 0x00000080U

/** Characteristic supports write on its User Description Descriptor */
#define GATT_DB_CHAR_WRITEABLE_AUX_PROPERTY            0x00000200U

/** \cond ignore_this unused */
/* Corrected Typo and Adding this macro for Backward Compatibility */
/** Characteristic supports write on its User Description Descriptor */
/** TODO: This is currently not used anywhere. To be made Obsolete */
#define GATT_DB_CHAR_WRIEABLE_AUX_PROPERTY             GATT_DB_CHAR_WRITEABLE_AUX_PROPERTY
/** \endcond */

/**
 * Auxiliary Property used to indicate if a Characteristic supports variable
 * length vales as Characteristic Values. This is to be passed as "property"
 * member of \ref BT_gatt_db_add_characteristic(...) when adding a
 * Characteristic in a GATT Database.
 * The value for this corresponds to BIT14 being SET.
 * This could also be used during the Characteristic Descriptor addition
 * to GATT Database.
 *
 * \note: This is not used in a static GATT DB i.e. is GATT_DB_DYNAMIC
 * feature is not used.
 */
#define GATT_DB_CHAR_VARIABLE_LENGTH_AUX_PROPERTY      0x00004000U

/**
 * Auxiliary Property used to indicate if a Characteristic supports
 * maintenance of Characteristic Values specific to each Peer Device.
 * This is to be passed as "property" member of
 * \ref BT_gatt_db_add_characteristic(...) when adding a Characteristic
 * in a GATT Database.
 * The value for this corresponds to BIT15 being SET.
 * This could also be used during the Characteristic Descriptor addition
 * to GATT Database.
 *
 * \note: This is not used in a static GATT DB i.e. is GATT_DB_DYNAMIC
 * feature is not used.
 */
#define GATT_DB_CHAR_PEER_SPECIFIC_AUX_PROPERTY        0x00008000

/** No Auxillary Property */
#define GATT_DB_NO_AUXILLARY_PROPERTY                  0x00U

/** Characteristic Value is Fixed Length */
#define GATT_DB_FIXED_LENGTH_PROPERTY                  0x01U

#define GATT_DB_AUTHORIZATION_PROPERTY                 0x02U

#define GATT_DB_PEER_SPECIFIC_VAL_PROPERTY             0x40U

#define GATT_DB_CONST_ATTR_VAL_PROPERTY                0x80U

/** \} */

/**
 *  \defgroup gatt_service_properties Service Properties
 *  \{
 *  Service Properties - Combination of these properties (combined
 *  using bitwise OR) describe properties of the Service. Also
 *  see \ref GATT_DB_SERVICE_DESC.
 */

/**
 *  Security related - combination of Level, Mode and Encryption Key Size
 *  (if applicable) describes complete security needs
 */
/** Service employs Level 0 */
#define GATT_DB_SER_SECURITY_LEVEL0                    0x00000000U

/** Service employs Level 1 */
#define GATT_DB_SER_SECURITY_LEVEL1                    0x00000001U

/** Service employs Level 2 */
#define GATT_DB_SER_SECURITY_LEVEL2                    0x00000002U

/** Service employs Level 3 */
#define GATT_DB_SER_SECURITY_LEVEL3                    0x00000004U

/** Service employs Mode 1 */
#define GATT_DB_SER_SECURITY_MODE1                     0x00000010U

/** Service employs Mode 2 */
#define GATT_DB_SER_SECURITY_MODE2                     0x00000020U

/** \cond ignore_this Unused right now */
#define GATT_DB_SER_MULTIPLE_CLIENTS_SUPPORT           0x00000040U
/** \endcond */

/**
 *  Service is a Secondary Service. If this not used, Service is by default
 *  considered to be Primary
 */
#define GATT_DB_SER_SECONDARY_SERVICE_PROPERTY         0x00000080U

#ifdef GATT_DB_SUPPORT_128_BIT_UUID

/** Service UUID is 128-bit */
#define GATT_DB_SER_128_BIT_UUID_FORMAT                0x00008000U

#endif /* GATT_DB_SUPPORT_128_BIT_UUID */

/** Set this property to relax all Security on the Service */
#define GATT_DB_SER_NO_SECURITY_PROPERTY               0x00000000U

/**
 *  This is employed to describe Service is not rigid on any Encryption
 *  Key Size. Any Size used by other Services or on the whole by the device
 *  will do
 */
#define GATT_DB_SER_ENC_KEY_SIZE_DONT_CARE             0x00000000U

/** Encryption Key Size 7 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_7                 0x01000000U

/** Encryption Key Size 8 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_8                 0x02000000U

/** Encryption Key Size 9 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_9                 0x03000000U

/** Encryption Key Size 10 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_10                0x04000000U

/** Encryption Key Size 11 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_11                0x05000000U

/** Encryption Key Size 12 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_12                0x06000000U

/** Encryption Key Size 13 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_13                0x07000000U

/** Encryption Key Size 14 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_14                0x08000000U

/** Encryption Key Size 15 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_15                0x09000000U

/** Encryption Key Size 16 Needed for the Service */
#define GATT_DB_SER_ENCRYPT_KEY_SIZE_16                0x0A000000U

/** \cond ignore_this unused */
#define GATT_DB_SER_NO_ENCRYPT_PROPERTY                0x00000000U

/**
 *  Macros to describe the Transport access of the Service over BR/EDR only,
 *  LE only or Any Transport.
 */
/** Service accessible over BR/EDR Link Only */
#define GATT_DB_SER_SUPPORT_BR_LINK_TYPE               0x10000000U

/** Service accessible over LE Link Only */
#define GATT_DB_SER_SUPPORT_LE_LINK_TYPE               0x20000000U

/** Service accessible over Any Link */
#define GATT_DB_SER_SUPPORT_ANY_LINK_TYPE              0xF0000000U
/** \endcond */

/** \} */

/** \cond ignore_this This Block is for internal use */

#define GATT_DB_SECURITY_MASK                          0x00000037U
#define GATT_DB_ENC_KEY_SIZE_MASK                      0x0F000000U
#define GATT_DB_SER_ENC_KEY_SIZE_LAG                   6U

#ifndef GATT_DB_DYNAMIC
#define GATT_PRIMARY_SERVICE_TYPE_OFFSET      0U
#define GATT_SECONDARY_SERVICE_TYPE_OFFSET    2U
#define GATT_INCLUDE_TYPE_OFFSET              4U
#define GATT_CHARACTERISTIC_TYPE_OFFSET       6U
#endif /* GATT_DB_DYNAMIC */

#define GATT_VERIFY_UUID_VALUE(v,o,l)\
        ((0U == BT_mem_cmp((v),GATT_GET_UUID_VALUE(o),(l)))? BT_TRUE : BT_FALSE)

#define GATT_GET_UUID_VALUE(o)\
        &gatt_const_uuid_arr_g[(o)]

#define GATT_DB_GET_UUID_OFFSET(i)\
        gatt_db_attr_table_g[(i)].uuid_offset

#define GATT_DB_GET_UUID(i)\
        GATT_GET_UUID_VALUE(GATT_DB_GET_UUID_OFFSET(i))

#define GATT_CHK_ATTR_HNDL_AUTHORIZATION(hndl) \
        ((GATT_DB_AUTHORIZATION_PROPERTY == \
        (gatt_db_attr_table_g[( hndl )].aux_property & GATT_DB_AUTHORIZATION_PROPERTY)) ? BT_TRUE : BT_FALSE)

#ifdef GATT_DB_DYNAMIC
/** GATT Characteristic and Descriptor permissions */
#define GATT_DB_PERM_NONE                    0x0000U
#define GATT_DB_PERM_READ                    0x0001U
#define GATT_DB_PERM_READ_ENCRYPTED          0x0002U
#define GATT_DB_PERM_READ_ENCRYPTED_MITM     0x0004U
#define GATT_DB_PERM_WRITE                   0x0010U
#define GATT_DB_PERM_WRITE_ENCRYPTED         0x0020U
#define GATT_DB_PERM_WRITE_ENCRYPTED_MIMT    0x0040U
#define GATT_DB_PERM_WRITE_SIGNED            0x0080U
#define GATT_DB_PERM_WRITE_SIGNED_MITM       0x0100U
#define GATT_DB_PERM_AUTHORIZATION           0x8000U
#endif /* GATT_DB_DYNAMIC */

/** \endcond */

/**
 *  This is dummy macro for Characteristic Value UUID used to locally
 *  read or update Characteristic Value using \ref GATT_DB_HANDLE.
 */
#define GATT_DUMMY_CHAR_VALUE_UUID                           0x0000U

#ifdef GATT_DB_HASH_SUPPORT
/**
 *  \defgroup gatt_db_events Events
 *  \{
 *  This section lists the Asynchronous Events notified to Application by the
 *  Module.
 */

/** GATT Database Hash Calculation Complete Event Indication */
#define GATT_DB_HASH_IND                                     0x01U
/** \} */

/**
 * \defgroup gatt_db_constants Constants
 * \{
 * \brief This section describes the various Constants in EtherMind
 * GATT DB Interface Layer.
 */

/**
 * \name GATT Database Hash
 * \{
 */
/** GATT Database Hash Value Length */
#define GATT_DB_HASH_VALUE_LEN                               16U

/** GATT Database Hash Key Length */
#define GATT_DB_HASH_KEY_LEN                                 16U
/** \} */

/** \} */
#endif /* GATT_DB_HASH_SUPPORT */

/** \} */
/* --------------------------------------------- Data Types/ Structures */

#ifdef GATT_DB_HASH_SUPPORT
/**
 *  \defgroup gatt_db_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
/**
 * GATT Database Application Asynchronous Notification Callback.
 *
 * GATT Database calls the registered callback to indicate events occurred
 * to the application.
 *
 * \param gatt_db_event Any of the Event occurred from the list at \ref gatt_db_events.
 * \param event_result \ref API_SUCCESS or an error code in \ref gatt_db_error_codes
 *        or any other internal error code from the stack.
 * \param data_param Data associated with the event if any or NULL.
 * \param data_len Size of the event data. 0 if event data is NULL.
 */
typedef API_RESULT (* GATT_DB_NTF_CB)
        (
            UCHAR         gatt_db_event,
            API_RESULT    event_result,
            UCHAR         * data_param,
            UINT16        data_len
        ) DECL_REENTRANT;
/** \} */
#endif /* GATT_DB_HASH_SUPPORT */

/**
 * \defgroup gatt_db_typedefs Typedefs
 * \{
 * This section describes the types used in the module.
 */

/**
 *  \cond ignore_this Unused, was intended for use in
 *  \ref GATT_DB_HANDLE, not being used right now
 */

typedef UCHAR GATT_DB_CHAR_HANDLE;
typedef UCHAR GATT_DB_SERVICE_HANDLE;

/** \endcond */

typedef UCHAR GATT_TYPE_INDEX;

typedef struct
{
    UINT16 uuid_offset;

    UINT16 fo;

    UINT16 lo;

}GATT_ATTR_TYPE;

/**
 * This has varying meaning depending on the context in which it is used.
 *
 * 1. This depicts the total number of attributes that comprise a given
 *    characteristic definition when used in \ref GATT_DB_CHARACERISTIC
 *    For example:
 *    In a Battery Service, the "Battery Level" characteristic definition
 *    comprises of the following
 *      a. Battery Level Characteristic Declaration
 *      b. Battery Level Characteristic Value Declaration
 *      c. Battery Level Client Characteristic Configuration Descriptor
 *         Declaration
 *      d. Battery Level Characteristic Presentation Format Descriptor
 *         Declaration
 *    Hence, the \ref GATT_DB_ATTR_LIST corresponding to Batter Level
 *    Characteristic needs to be "4".
 *
 * 2. This depicts the total number of Service Declarations contained in
 *    a given service when used in \ref GATT_DB_SERVICE.
 *    It is recommended to have value of "1" for all \ref GATT_DB_ATTR_LIST
 *    in the \ref GATT_DB_SERVICE structure
 *
 * NOTE: Currently this parameter is not used inside the Core GATT DB
 *       Interface.
 */
typedef struct
{
    UCHAR   count;

}GATT_DB_ATTR_LIST;

/**
 *  This abstracts properties of service, including needed security, encryption
 *  key size, distinction of primary/secondary service. See
 *  \ref gatt_service_properties.
 *
 *  NOTE: GATT Specification allows Each Characteristic/Service to specify the
 *  Endianness of its Characteristic Values.
 */
typedef UINT32 GATT_DB_SERVICE_DESC;

/** \} */

/**
 * \defgroup gatt_db_structures Structures
 * \{
 * This section describes the data structures for the module.
 */

/** Abstracts GATT Characteristic */
typedef struct
{
    /** Count of Attributes in Characteristic Definition */
    GATT_DB_ATTR_LIST       list;

    /**
     *  Characteristic Handle Range
     *  Please Note:
     *  Characteristic Value                              is at start_handle + 1
     *  Client Configuration                (if present)  is at start_handle + 2
     *  Server Configuration                (if present)  is at start_handle + 3
     *  Characteristic Presentation Format  (if present)  is at start_handle + 4
     *  User Description                    (if present)  is at start_handle + 5
     *  Characteristic Aggregate Format     (if present)  is at start_handle + 6
     *  Extended Property Format            (if present)  is at start_handle + 7
     *  Profile Defined Descriptor          (if present)  from start_handle + 8
     */
    /** Start Handle */
    ATT_ATTR_HANDLE        start_handle;

    /** Service to which Characteristic Belongs */
    UCHAR                  service_id;

}GATT_DB_CHARACERISTIC;

/** Abstracts  GATT Service */
typedef struct
{
    /** Count of Service Declarations inside a service */
    GATT_DB_ATTR_LIST       list;

    GATT_DB_SERVICE_DESC    desc;

    /** Service Handle Range */
    ATT_ATTR_HANDLE         start_handle;

    ATT_ATTR_HANDLE         end_handle;

    /** Range of Characteristics grouped in the service */
    UCHAR                   char_start_index;

    UCHAR                   char_end_index;

}GATT_DB_SERVICE;

/**
 *  Abstracts Not So Often Used Descriptor Information
 *  Including descriptors like Characteristic Presentation Format,
 *  Aggregate, User Defined Descriptor, Higher Layer Defined Descriptors.
 *  'length' indicates size of descriptor value and data_offset contains
 *  offset to the value in gatt_db_desc_data_array.
 *
 *  Note: In case of Higher Layer Defined Descriptors, first 2 octets contain
 *  UUID, next octet contains Descriptor Properties (Read/Write/Indicate etc.),
 *  followed  'length' octets of value.
 */
typedef struct
{
    /** Describe how the descriptor could be accessed */
    UCHAR     desc_property;

    /**
     *  Provides information if the Descriptor value is fixed length or the UUID
     *  format info, if the value is maintained peer specific etc.
     */
    UCHAR     aux_property;

    /**  Length of Descriptor Value */
    UINT16    length;

    /** Pointer to Next Attribute of this type */
    UINT16     next;

    /** Offset to UUID value */
    UINT16     uuid_offset;

    /**  Offset where the Descriptor Value is located */
    DECL_CONST UCHAR     * data_offset;

#ifdef GATT_DB_DYNAMIC
    /**
     * This member tracks the current state of an attribute
     * Discovery/Operations on this attribute needs to be valid
     * only when its set to \ref BT_TRUE.
     * If the state is set to \ref BT_FALSE this attribute is out
     * of bounds.
     */
    UCHAR      is_active;
#endif /* GATT_DB_DYNAMIC */



}GATT_DB_DESC_DATA;

typedef struct
{
    /** Identifies the peer accessing the database */
    DEVICE_HANDLE    device_id;

    /** Identifies the Service being Accessed */
    UCHAR            service_id;

    /** Identifies the Characteristic being Accessed */
    UCHAR            char_id;

}GATT_DB_HANDLE;

/**
 *  Abstracts the Access Operation when notifying the application of the Access
 *  Request from Peer through the Callback for the Characteristic. See also
 *  \ref gatt_db_operations
 */
typedef UCHAR GATT_DB_OPERATION;

/**
 *  Abstracts parameters for application specific handling for characteristics.
 *  See also \ref GATT_DB_OPERATION and \ref gatt_db_operations
 */
typedef struct
{
    /** Abstracts Value to get/set the Attribute Value */
    ATT_VALUE            value;

    /** Abstracts the handle information */
    ATT_ATTR_HANDLE      handle;

    /** Abstracts the Access Operation */
    GATT_DB_OPERATION    db_op;

#ifdef GATT_DB_HAVE_OFFSET_IN_PARAMS_SUPPORT
    UINT16               offset;
#endif /* GATT_DB_HAVE_OFFSET_IN_PARAMS_SUPPORT */

}GATT_DB_PARAMS;

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT

/**
 * GATT Database Application Characteristic Handler Callback.
 *
 * GATT Database calls the registered callback to intimate the application
 * of the ongoing operation occurring at the GATT DB interface.
 *
 * \param handle handle of type \ref GATT_DB_HANDLE for reference.
 * \param param \ref GATT_DB_PARAMS specifying the type of GATT operations.
 */
typedef API_RESULT(*GATT_DB_CHAR_HANDLER_CB)
        (
            GATT_DB_HANDLE * handle,
            GATT_DB_PARAMS * param
        ) DECL_REENTRANT;

#ifdef GATT_DB_HAVE_DB_SIGNATURE
/* GATT DB Signature, used to uniquely distinguish a registered DB */
typedef UINT32 GATT_DB_SIGNATURE;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

/**
 * Data structure used by application to register GATT Database
 * with GATT DB module.
 */
typedef struct _GATT_DB_STRUCT
{
    /** GATT Services */
    GATT_DB_SERVICE * gatt_service;

    /** GATT Characteristics */
    GATT_DB_CHARACERISTIC * gatt_characteristic;

    /** GATT Constant Value Array */
    UCHAR * gatt_const_value_arr;

    /** GATT Constant UUID Array */
    UCHAR * gatt_const_uuid_arr;

    /** GATT Attribute Table */
    GATT_DB_DESC_DATA * gatt_db_attr_table;

    /** GATT Type Table */
    GATT_ATTR_TYPE * gatt_type_table;

    /** GATT Peer Specific Attribute Value Array */
    UCHAR * gatt_db_peer_specific_val_arr;

    /** GATT Peer Specific Constant Attribute Value Array */
    UCHAR * gatt_db_const_peer_specific_val_arr;

    /** GATT Value Array */
    UCHAR * gatt_value_arr;

    /** GATT Service Count */
    UCHAR  gatt_service_count;

    /** GATT Characteristic Count */
    UCHAR  gatt_characteristic_count;

    /** GATT MAX Type Count */
    UCHAR  gatt_db_max_type_count;

    /** GATT Peer Value Array Size */
    UINT16  gatt_db_peer_val_arr_size;

    /** GATT Characteristic Handler */
    GATT_DB_CHAR_HANDLER_CB gatt_db_char_cb;

#ifdef STORAGE_RETENTION_SUPPORT
    /** GATT Value Array Size */
    UINT16 gatt_value_array_size;

    /** GATT Max Peer Configuration */
    UINT16 gatt_db_max_peer_config;
#endif /* STORAGE_RETENTION_SUPPORT */

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* GATT DB Signature, used to uniquely distinguish a registered DB */
    GATT_DB_SIGNATURE gatt_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

}GATT_DB_STRUCT;
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

/**
 *  Dynamic configuration of GATT DB Data-structure.
 *  Used only if 'BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY' is defined.
 */
#ifdef BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY
typedef struct _GATT_DB_DYNAMIC_CONFIG
{
    /** Number of GATT Services */
    UINT16 config_GATT_DB_DYN_MAX_SERVICE_COUNT;

#ifdef BT_HAVE_GATT_DB_RECORDS_SORTED_BY_UUID_SIZE
    /** Number Number of GATT Service - with 16-bit UUID */
    UINT16 config_GATT_DB_DYN_MAX_16_BIT_SERVICE_COUNT;

    /** Number of GATT Service - with 128-bit UUID */
    UINT16 config_GATT_DB_DYN_MAX_128_BIT_SERVICE_COUNT;
#endif /* BT_HAVE_GATT_DB_RECORDS_SORTED_BY_UUID_SIZE */


    /** Number of GATT Characteristics */
    UINT16 config_GATT_DB_DYN_CHARACTERISTIC_COUNT;

    /** Number of GATT Attributes */
    UINT16 config_GATT_DB_DYN_MAX_ATTRIBUTES;

#ifdef BT_HAVE_GATT_DB_RECORDS_SORTED_BY_UUID_SIZE
    /** Number of GATT Attributes - corresponding to Services with 16-bit UUID */
    UINT16 config_GATT_DB_DYN_MAX_ATTRIBUTES_16_BIT;

    /** Number of GATT Attributes - corresponding to Services with 128-bit UUID */
    UINT16 config_GATT_DB_DYN_MAX_ATTRIBUTES_128_BIT;
#endif /* BT_HAVE_GATT_DB_RECORDS_SORTED_BY_UUID_SIZE */

    /** Number of GATT attribute types or UUIDs */
    UINT16 config_GATT_DB_DYN_MAX_TYPE_COUNT;

    /**
     *  Peer specific characteristic or characteristic descriptor
     *  attribute value array size for a single instance of peer device
     */
    UINT16 config_GATT_DB_DYN_PEER_VALUE_ARRAY_SIZE;

    /**
     *  Characteristic attribute value array size, which are not
     *  peer specific, but fixed/constant
     */
    UINT16 config_GATT_DB_DYN_VALUE_ARRAY_SIZE;

    /**
     *  Characteristic attribute value array size, which are not
     *  peer specific, but can be changed
     */
    UINT16 config_GATT_VALUE_ARRAY_SIZE;

    /** Flattened list size of UUID values (both 16 and 128 bit) */
    UINT16 config_GATT_DB_DYN_UUID_ARRAY_SIZE;

    /**
     *  'config_GATT_DB_DYN_PEER_VALUE_ARRAY_SIZE' times maximum number of
     *  supported peer devices 'BT_MAX_DEVICE_QUEUE_SIZE'
     */
    UINT16 config_GATT_DB_DYN_MAX_PEER_CONFIGURATION;

} GATT_DB_DYNAMIC_CONFIG;
#endif /* BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY */

/** \} */

/**
 *  \defgroup gatt_db_marcos Utility Macros
 *  \{
 *  Utility Macros offered by the module.
 */
/** Macro to fetch needed security for a Service */
#define GATT_DB_GET_NEEDED_SECURITY(sid)\
        (UCHAR)(gatt_service_g[(sid)].desc & GATT_DB_SECURITY_MASK)

/** Macro to fetch needed Encryption Key Size for a Service */
#define GATT_DB_GET_NEEDED_ENC_KEY_SIZE(sid)\
        (UCHAR)((GATT_DB_SER_ENC_KEY_SIZE_DONT_CARE == \
        ((gatt_service_g[(sid)].desc & GATT_DB_ENC_KEY_SIZE_MASK) >> 24U)) ? \
        GATT_DB_SER_ENC_KEY_SIZE_DONT_CARE : (((gatt_service_g[(sid)].desc & \
        GATT_DB_ENC_KEY_SIZE_MASK) >> 24) + GATT_DB_SER_ENC_KEY_SIZE_LAG))

#define GATT_DB_GET_CHAR_VAL_HANDLE(i,j)\
        (j) = (gatt_characteristic_g[(i)].start_handle + 1)

/** \} */

#define GET_GATT_SERVICE_COUNT()                 gatt_service_count_g
#define GET_GATT_CHARACTERISTIC_COUNT()          gatt_characteristic_count_g
#define GET_GATT_MAX_TYPE_COUNT()                gatt_db_max_type_count_g
#define GET_GATT_DB_PEER_VALUE_ARRAY_SIZE()      gatt_db_peer_val_arr_size_g

#ifdef STORAGE_RETENTION_SUPPORT
#define GET_GATT_VALUE_ARRAY_SIZE()              gatt_value_array_size_g
#define GET_GATT_DB_MAX_PEER_CONFIG()            gatt_db_max_peer_config_g
#endif /* STORAGE_RETENTION_SUPPORT */

#ifdef GATT_DB_DYNAMIC
#define GATT_PRIMARY_SERVICE_TYPE_OFFSET         gatt_ps_uuid_type_offset
#define GATT_SECONDARY_SERVICE_TYPE_OFFSET       gatt_ss_uuid_type_offset
#define GATT_INCLUDE_TYPE_OFFSET                 gatt_is_uuid_type_offset
#define GATT_CHARACTERISTIC_TYPE_OFFSET          gatt_char_uuid_type_offset
#endif /* GATT_DB_DYNAMIC */

#ifdef BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY
#ifndef BT_HAVE_GATT_DB_RECORDS_SORTED_BY_UUID_SIZE
#define GATT_DB_INIT_DYNAMIC_CONFIG(config)                                                 \
    (config).config_GATT_DB_DYN_MAX_SERVICE_COUNT      = GATT_DB_DYN_MAX_SERVICE_COUNT;     \
    (config).config_GATT_DB_DYN_CHARACTERISTIC_COUNT   = GATT_DB_DYN_CHARACTERISTIC_COUNT;  \
    (config).config_GATT_DB_DYN_MAX_ATTRIBUTES         = GATT_DB_DYN_MAX_ATTRIBUTES;        \
    (config).config_GATT_DB_DYN_MAX_TYPE_COUNT         = GATT_DB_DYN_MAX_TYPE_COUNT;        \
    (config).config_GATT_DB_DYN_PEER_VALUE_ARRAY_SIZE  = GATT_DB_DYN_PEER_VALUE_ARRAY_SIZE; \
    (config).config_GATT_DB_DYN_VALUE_ARRAY_SIZE       = GATT_DB_DYN_VALUE_ARRAY_SIZE;      \
    (config).config_GATT_VALUE_ARRAY_SIZE              = GATT_VALUE_ARRAY_SIZE;             \
    (config).config_GATT_DB_DYN_UUID_ARRAY_SIZE        = GATT_DB_DYN_UUID_ARRAY_SIZE;       \
    (config).config_GATT_DB_DYN_MAX_PEER_CONFIGURATION = GATT_DB_DYN_MAX_PEER_CONFIGURATION
#else
#define GATT_DB_INIT_DYNAMIC_CONFIG(config)                                                          \
    (config).config_GATT_DB_DYN_MAX_SERVICE_COUNT           = GATT_DB_DYN_MAX_SERVICE_COUNT;         \
    (config).config_GATT_DB_DYN_MAX_16_BIT_SERVICE_COUNT    = GATT_DB_DYN_MAX_16_BIT_SERVICE_COUNT;  \
    (config).config_GATT_DB_DYN_MAX_128_BIT_SERVICE_COUNT   = GATT_DB_DYN_MAX_128_BIT_SERVICE_COUNT; \
    (config).config_GATT_DB_DYN_CHARACTERISTIC_COUNT        = GATT_DB_DYN_CHARACTERISTIC_COUNT;      \
    (config).config_GATT_DB_DYN_MAX_ATTRIBUTES              = GATT_DB_DYN_MAX_ATTRIBUTES;            \
    (config).config_GATT_DB_DYN_MAX_ATTRIBUTES_16_BIT       = GATT_DB_DYN_MAX_ATTRIBUTES_16_BIT;     \
    (config).config_GATT_DB_DYN_MAX_ATTRIBUTES_128_BIT      = GATT_DB_DYN_MAX_ATTRIBUTES_128_BIT;    \
    (config).config_GATT_DB_DYN_MAX_TYPE_COUNT              = GATT_DB_DYN_MAX_TYPE_COUNT;            \
    (config).config_GATT_DB_DYN_PEER_VALUE_ARRAY_SIZE       = GATT_DB_DYN_PEER_VALUE_ARRAY_SIZE;     \
    (config).config_GATT_DB_DYN_VALUE_ARRAY_SIZE            = GATT_DB_DYN_VALUE_ARRAY_SIZE;          \
    (config).config_GATT_VALUE_ARRAY_SIZE                   = GATT_VALUE_ARRAY_SIZE;                 \
    (config).config_GATT_DB_DYN_UUID_ARRAY_SIZE             = GATT_DB_DYN_UUID_ARRAY_SIZE;           \
    (config).config_GATT_DB_DYN_MAX_PEER_CONFIGURATION      = GATT_DB_DYN_MAX_PEER_CONFIGURATION
#endif /* BT_HAVE_GATT_DB_RECORDS_SORTED_BY_UUID_SIZE */

#define GATT_DB_DYNAMIC_CONFIG_LIMITS(x) \
        gatt_db_dynamic_global_config.config_##x

#define BT_DEFINE_GLOBAL_ARRAY(type, var, s) \
        type * var

#define BT_DECLARE_GLOBAL_ARRAY(type, var, s) \
        extern type * var

/* TODO: Check if the allocation is success */
#define BT_INIT_GLOBAL_ARRAY(type, var, s, i)   \
        (var) = BT_alloc_mem((s) * sizeof(type)); \
        COMMON_TRC(BT_MODULE_ID_GATT_DB, "Allocated %lu bytes for %s. Ptr: %p\n", ((s) * sizeof(type)), #var, (var)); \
        BT_mem_set((var), (i), ((s) * sizeof(type)))

#define BT_DEINIT_GLOBAL_ARRAY(type, var, s) \
        if (NULL != (var))            \
        {                           \
            COMMON_TRC(BT_MODULE_ID_GATT_DB, "Freeing %s with Allocated Ptr: %p\n", #var, (var));\
            BT_free_mem((var));     \
        }

#else
#define GATT_DB_DYNAMIC_CONFIG_LIMITS(x) (x)

#define BT_DEFINE_GLOBAL_ARRAY(type, var, s) \
        type var[(s)]

#define BT_DECLARE_GLOBAL_ARRAY(type, var, s) \
        extern type var[(s)]

#define BT_INIT_GLOBAL_ARRAY(type, var, s, i) \
        BT_mem_set(var, (i), ((s) * sizeof(type)))

#define BT_DEINIT_GLOBAL_ARRAY(type, var, s)

#endif /* BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY */

/** \cond ignore_this Ignore this block */
extern DECL_CONST GATT_DB_SERVICE * gatt_service_g;
extern DECL_CONST GATT_DB_CHARACERISTIC * gatt_characteristic_g;
/**/extern DECL_CONST UCHAR * gatt_const_value_arr_g;
extern DECL_CONST UCHAR * gatt_const_uuid_arr_g;
extern DECL_CONST GATT_DB_DESC_DATA * gatt_db_attr_table_g;
/**/extern DECL_CONST GATT_ATTR_TYPE * gatt_type_table_g;
/**/extern UCHAR * gatt_db_peer_specific_val_arr_g;
/**/extern DECL_CONST UCHAR * gatt_db_const_peer_specific_val_arr_g;
/**/extern UCHAR * gatt_value_arr_g;

extern UCHAR  gatt_service_count_g;
/**/extern UCHAR  gatt_characteristic_count_g;
/**/extern UCHAR  gatt_db_max_type_count_g;
/**/extern UINT16  gatt_db_peer_val_arr_size_g;

#ifdef STORAGE_RETENTION_SUPPORT
extern UINT16 gatt_value_array_size_g;
extern UINT16 gatt_db_max_peer_config_g;
#endif /* STORAGE_RETENTION_SUPPORT */

#ifdef GATT_DB_DYNAMIC
/** Abstract UUID with associated format */
typedef struct _GATT_DB_UUID_TYPE
{
    /** Format indicating, 16 bit or 128 bit UUIDs */
    UCHAR              uuid_format;

    /** Attribute UUID */
    ATT_UUID           uuid;

} GATT_DB_UUID_TYPE;

/** Service Information */
typedef struct _GATT_DB_SERVICE_INFO
{
    /** UUID */
    GATT_DB_UUID_TYPE    uuid;

    /** If this service is primary or not */
    UCHAR                is_primary;

    /**
     * Security Requirements for the Service
     * Logical OR of desired combinations of
     * 1. Security Mode
     * 2. Security Level
     * 3. Encryption Key Size
     *
     * Acceptable values for Security Mode are:
     * \ref GATT_DB_SER_SECURITY_MODE1
     * \ref GATT_DB_SER_SECURITY_MODE2
     *
     * Acceptable values for Security Level are:
     * \ref GATT_DB_SER_SECURITY_LEVEL1
     * \ref GATT_DB_SER_SECURITY_LEVEL2
     * \ref GATT_DB_SER_SECURITY_LEVEL3
     *
     * Acceptable values for Encryption Key Size are:
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_7
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_8
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_9
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_10
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_11
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_12
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_13
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_14
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_15
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_16
     * When the Service does not mandate any specific Key size
     * \ref GATT_DB_SER_ENC_KEY_SIZE_DONT_CARE shall be used.
     *
     * When No Security is mandated for the service
     * Value \ref GATT_DB_SER_NO_SECURITY_PROPERTY shall be used.
     */
    GATT_DB_SERVICE_DESC sec_req;

    /**
     * Transport Requirements for the Service.
     * This describes the Transport on which this required
     * to be operational.
     * \ref GATT_DB_SER_SUPPORT_ANY_LINK_TYPE
     * \ref GATT_DB_SER_SUPPORT_LE_LINK_TYPE
     * \ref GATT_DB_SER_SUPPORT_BR_LINK_TYPE
     */
    GATT_DB_SERVICE_DESC link_req;

} GATT_DB_SERVICE_INFO;

/* Declaration for UUID Type Offsets */
extern UINT16 gatt_ps_uuid_type_offset;
extern UINT16 gatt_ss_uuid_type_offset;
extern UINT16 gatt_is_uuid_type_offset;
extern UINT16 gatt_char_uuid_type_offset;
#endif /* GATT_DB_DYNAMIC */

/** \endcond */

/* --------------------------------------------- Functions */
#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \defgroup gatt_db_api API Definitions
 *  \{
 *  This section describes the APIs offered by the module to the application.
 */

/**
 *  \brief To Initialize the Module.
 *
 *  \par Description
 *  This API is used to initialize the module and shall be called before using
 *  any of the other module routines.
 *
 *  \param [in] blob
 *         If 'BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY' defined,
 *             application shall provide the desired dynamic configuration
 *             using a pointer to GATT_DB_DYNAMIC_CONFIG data structure instance.
 *         else,
 *             this parameter shall be NULL and ignored by the API.
 */
void BT_gatt_db_init
     (
         /* IN */ void * blob
     );

/**
 *  \brief To De-Initialize the Module.
 *
 *  \par Description
 *  This API is used to De-initialize the module and shall be called during
 *  proper/clean shutdown of this module.
 *
 *  \param None
 *
 *  \return None
 */
void BT_gatt_db_deinit(void);

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
/**
 *  \brief To register a GATT database.
 *
 *  \par Description:
 *  This API registers a GATT database with the GATT module.
 *  This allows the application to change the GATT database if required
 *  based on the usage scenario.
 *
 *  \param [in] gatt_db
 *         This parameter must be a pointer to the GATT_DB_STRUCT variable
 *         containing the GATT database related data structures.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 *
 *  \note HAVE_GATT_DB_REGISTRATION_SUPPORT feature flag shall be defined
 *  for this API.
 */
API_RESULT BT_gatt_db_register
           (
               /* IN */ GATT_DB_STRUCT   * gatt_db
           );
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

API_RESULT gatt_db_access_value
           (
               /* IN */  ATT_HANDLE        * inst,
               /* OUT */ ATT_VALUE         * value,
               /* IN */  ATT_ATTR_HANDLE   handle,
               /* IN */  UINT16            offset,
               /* IN */  UCHAR             flag
           );

/**
 *
 *  \brief To search for group entities or get values of a specific group.
 *
 *  \par Description:
 *       This routine searches for group entities (services) or get values of a
 *       specific group(PRIMARY/SECONDARY). This API is used to formulate response
 *       of Read By Group Type Request.
 *
 *  \param [in] range
 *         Requested Range for Group Search
 *
 *  \param [out] group_range
 *         Range of Group Entity on Successful Search.
 *
 *  \param [in,out] value
 *         Value of Service (Needed only for Read By Group Type)
 *
 *  \param [in] uuid
 *         Identifies the UUID of Group Entity (PRIMARY/SECONDARY).
 *
 *  \param [in] compare_val_flag
 *         Indicates if Value needs to be compared or not
 *
 *  \return API_SUCCESS on Successful Search and no more Group Entities
 *          (Services) exist
 *          GATT_DB_MORE_MATCHING_RESULT_FOUND on Successful Search and more
 *          Group Entities (Services) exist to Search Further
 *          Others - Appropriate Response Code indicating reason why Search did
 *          not succeed.
 */
API_RESULT gatt_db_get_group_hndl_val_list
           (
               /* IN */    ATT_HANDLE           * att_inst,
               /* IN */    ATT_HANDLE_RANGE     * range,
               /* OUT */   ATT_HANDLE_RANGE     * group_range,
               /* INOUT */ ATT_VALUE            * value,
               /* IN */    ATT_VALUE            * uuid,
               /* IN */    UCHAR                  compare_val_flag
           );

API_RESULT BT_gatt_db_get_attr_handle_prpty
           (
               /* IN */  GATT_DB_HANDLE      * handle,
               /* IN */  ATT_HANDLE_RANGE    * range,
               /* IN */  UINT16                uuid,
               /* OUT */ ATT_ATTR_HANDLE     * attr_handle,
               /* OUT */ UCHAR               * attr_prpty
           );

/**
 *  \brief To get Handle Range for Attribute Type Value Pair in the requested
 *  Range for local or remote initiated operation.
 *
 *  \par Description
 *  This routine fetches Handle Attribute Value Pair in the requested Range of
 *  a particular Attribute Type, providing one Search Result at a time for
 *  a locally initiated or Peer Initiated read operation.
 *
 *  \param range
 *         Requested Range for Handle Attribute Value List.
 *
 *  \param uuid
 *         Identifies the Attribute Type whose Values are requested.
 *
 *  \param handle_value_pair
 *         On Successful Search, contains Handle Value Pair in Requested Range
 *         and of requested Attribute Type.
 *
 *  \param flags
 *         Identifies the direction of operation \ref GATT_DB_PEER_INITIATED
 *         or \ref GATT_DB_LOCALLY_INITIATED.
 *
 *  \return API_SUCCESS on successful search and no more attributes exist.
 *          GATT_DB_MORE_MATCHING_RESULT_FOUND on successful search and more
 *          attributes exist to search further.
 *          Others - Appropriate Code indicating reason why search did not
 *          succeed.
 */
API_RESULT BT_gatt_db_fetch_handle_value_pair
           (
                /* IN */  ATT_HANDLE               * inst,
                /* IN */  ATT_HANDLE_RANGE         * range,
                /* IN */  ATT_VALUE                * uuid,
                /* OUT */ ATT_HANDLE_VALUE_PAIR    * handle_value_pair,
                /* IN */  UCHAR                      flags
           );

/**
 *  \brief To get Handle Range for Attribute Type Value Pair in the requested
 *  Range locally.
 *
 *  \par Description
 *  This routine fetches Handle Attribute Value Pair in the requested Range of
 *  a particular Attribute Type, providing one Search Result at a time for
 *  a locally initiated read.
 *
 *  \param inst
 *         Identifies the Peer Entity corresponding to Read of Attribute.
 *         This is can be NULL. But, when NULL is provided for a handle
 *         value pair of a characteristic that is Client/Peer specific,
 *         an appropriate error is returned. It is important for the
 *         application to then provide the correct instance for such
 *         characteristics so that value belonging to the corresponding
 *         peer is fetched.
 *
 *  \param range
 *         Requested Range for Handle Attribute Value List.
 *
 *  \param uuid
 *         Identifies the Attribute Type whose Values are requested.
 *
 *  \param handle_value_pair
 *         On Successful Search, contains Handle Value Pair in Requested Range
 *         and of requested Attribute Type.
 *
 *  \return API_SUCCESS on successful search and no more attributes exist.
 *          GATT_DB_MORE_MATCHING_RESULT_FOUND on successful search and more
 *          attributes exist to search further.
 *          Others - Appropriate Code indicating reason why search did not
 *          succeed.
 */
#define BT_gatt_db_read_handle_value_pair(inst, range, uuid, hvp) \
        BT_gatt_db_fetch_handle_value_pair((inst), (range), (uuid), (hvp), (GATT_DB_READ | GATT_DB_LOCALLY_INITIATED))

/**
 *  \brief To search for group entities or get values of a specific group.
 *
 *  \par Description
 *  This routine searches for group entities (services) or get values of a
 *  specific group(PRIMARY/SECONDARY). This API is used to formulate response
 *  of Read By Group Type Request. This routine provides one search result at a
 *  time. Note, Currently only Primary Services are Supported.
 *
 *  \param att_inst
 *         Identifies the Peer Entity Requesting the Read of Attribute. This is
 *         important so that necessary Security Checks are performed before the
 *         the value is fetched.
 *
 *  \param range
 *         Requested Range for Group Search
 *
 *  \param group_range
 *         Range of Group Entity on Successful Search.
 *
 *  \param value
 *         Group (Service) Value.
 *
 *  \param uuid
 *         Identifies the UUID of Group Entity (PRIMARY/SECONDARY).
 *
 *  \return API_SUCCESS on successful search and no more group entities
 *          (Services) exist.
 *          GATT_DB_MORE_MATCHING_RESULT_FOUND on successful search and more
 *          Group Entities (Services) exist to search further.
 *          Others - Appropriate Code indicating reason why search did not
 *          succeed.
 */
#define BT_gatt_db_get_group_range_val_pair(att_inst,range,uuid,group_range,value)\
        gatt_db_get_group_hndl_val_list\
        (\
            (att_inst),\
            (range),\
            (group_range),\
            (value),\
            (uuid),\
            BT_FALSE\
        )

/**
 *  \brief To get Handle Attribute Type Pair in the requested Range.
 *
 *  \par Description
 *  This routine fetches Handle Attribute Type Pair in the requested Range,
 *  providing one Search Result at a time and is used to formulate the response
 *  for Find Info Request.
 *
 *  \param att_inst
 *         Identifies the Peer Entity Requesting the Read of Attribute. This is
 *         important so that necessary Security Checks are performed before the
 *         the value is fetched.
 *
 *  \param range
 *         Requested Range for Handle Attribute Type List.
 *
 *  \param handle_uuid
 *         On Successful Search, contains Handle Attribute Type (UUID) Pair in
 *         Requested Range.
 *
 *  \return API_SUCCESS on successful search and no more attributes exist.
 *          GATT_DB_MORE_MATCHING_RESULT_FOUND on successful search and more
 *          attributes exist to search further.
 *          Others - Appropriate Code indicating reason why search did not
 *          succeed.
 */
API_RESULT BT_gatt_db_get_handle_uuid_pair
           (
               /* IN */  ATT_HANDLE               * att_inst,
               /* IN */  ATT_HANDLE_RANGE         * range,
               /* OUT */ ATT_HANDLE_VALUE_PAIR    * handle_uuid
           );


/**
 *  \brief To get Handle Range for Attribute Type Value Pair in the requested
 *  Range.
 *
 *  \par Description
 *  This routine fetches Handle Attribute Value Pair in the requested Range of
 *  a particular Attribute Type, providing one Search Result at a time and
 *  is used to formulate the response for Read By Type Request.
 *
 *  \param inst
 *         Identifies the Peer Entity Requesting the Read of Attribute. This is
 *         important so that necessary Security Checks are performed before the
 *         the value is fetched.
 *
 *  \param range
 *         Requested Range for Handle Attribute Value List.
 *
 *  \param uuid
 *         Identifies the Attribute Type whose Values are requested.
 *
 *  \param handle_value_pair
 *         On Successful Search, contains Handle Value Pair in Requested Range
 *         and of requested Attribute Type.
 *
 *  \return API_SUCCESS on successful search and no more attributes exist.
 *          GATT_DB_MORE_MATCHING_RESULT_FOUND on successful search and more
 *          attributes exist to search further.
 *          Others - Appropriate Code indicating reason why search did not
 *          succeed.
 */
#define BT_gatt_db_get_handle_value_pair(inst, range, uuid, hvp)\
        BT_gatt_db_fetch_handle_value_pair((inst), (range), (uuid), (hvp), (GATT_DB_READ | GATT_DB_PEER_INITIATED))

/**
 *  \brief To get Handle Attribute Value Pair in the requested Range and of a
 *         particular Attribute Type.
 *
 *  \par Description
 *  This routine fetches Handle Range for Attribute Value Type Pair in the
 *  requested Range, providing one Search Result at a time and is used to
 *  formulate the response for Find By Type Value Request. Note, if the
 *  Attribute is not a group range, start end handle will be equal to end
 *  handle.
 *
 *  \param inst
 *         Identifies the Peer Entity Requesting the Read of Attribute. This is
 *         important so that necessary Security Checks are performed before the
 *         the value is fetched.
 *
 *  \param range
 *         Requested Range for Attribute search.
 *
 *  \param type
 *         Identifies 16-bit Type of Attribute to be matched.
 *
 *  \param value
 *         Contains the Value of Attribute to be matched.
 *
 *  \param attr_range
 *         On Successful Search, contains Handle Range of the attribute requested
 *         by Type & Value.
 *
 *  \return API_SUCCESS on successful search and no more attributes exist.
 *          GATT_DB_MORE_MATCHING_RESULT_FOUND on successful search and more
 *          attributes exist to search further.
 *          Others - Appropriate Code indicating reason why search did not
 *          succeed.
 */
API_RESULT BT_gatt_db_get_range_by_type_val
           (
               /* IN */  ATT_HANDLE          * inst,
               /* IN */  ATT_HANDLE_RANGE    * range,
               /* IN */  ATT_VALUE           * type,
               /* IN */  ATT_VALUE           * value,
               /* OUT */ ATT_HANDLE_RANGE    * attr_range
           );

/**
 *  \brief To Get Characteristic Value Attribute Handle.
 *
 *  \par Description
 *  This routine provides Attribute Handle for Characteristic Value of a
 *  Characteristic.
 *
 *  \param handle
 *         \ref GATT_DB_HANDLE, identifying the Characteristic whose Value handle
 *         is to be fetched.
 *
 *  \param attr_handle
 *         On successful return of the routine contains the Attribute handle
 *         as visible to the peer.
 *
 *  \return API_SUCCESS on success or an appropriate error code.
 */
#define BT_gatt_db_get_char_val_hndl(handle,attr_handle)\
        GATT_DB_GET_CHAR_VAL_HANDLE((handle)->char_id,(*(attr_handle)))

API_RESULT gatt_db_access_val_by_db_handle
           (
                /* IN */ GATT_DB_HANDLE    * handle,
                /* IN */ ATT_VALUE         * attr_value,
                /* IN */ UINT16            uuid,
                /* IN */ UCHAR             flag
           );

/**
 *
 *  \brief To Locally Set Characteristic Value based on \ref GATT_DB_HANDLE.
 *
 *  \par Description
 *  This routine updates Characteristic Value of the Characteristic identified
 *  by handle.
 *
 *  \param [in] handle
 *         \ref GATT_DB_HANDLE, identifying the Characteristic whose value is Set.
 *
 *  \param [in] value
 *         Value to which Characteristic Value is being updated.
 *
 *  \return API_SUCCESS on success or an appropriate error code.
 */
#define BT_gatt_db_set_char_val(handle,value)\
        gatt_db_access_val_by_db_handle\
        (\
                (handle),\
                (value),\
                GATT_DUMMY_CHAR_VALUE_UUID,\
                GATT_DB_UPDATE\
        )\

/**
 *
 *  \brief To Locally Get Characteristic Value based on \ref GATT_DB_HANDLE.
 *
 *  \par Description
 *  This routine Gets/Reads Characteristic Value of the Characteristic
 *  identified by handle.
 *
 *  \param [in] handle
 *         \ref GATT_DB_HANDLE, identifying the Characteristic whose value is Read.
 *
 *  \param [out] value
 *         Contains Characteristic Value in case of successful return of this
 *         API.
 *
 *  \return API_SUCCESS on success or an appropriate error code.
 */
#define BT_gatt_db_get_char_val(handle,value)\
        gatt_db_access_val_by_db_handle\
        (\
                (handle),\
                (value),\
                GATT_DUMMY_CHAR_VALUE_UUID,\
                GATT_DB_READ\
        )

/**
 *
 *  \brief To Locally Get Client Configuration Descriptor Value of
 *         Characteristic.
 *
 *  \par Description
 *  This routine Gets/Reads Value of the Client Configuration Descriptor for the
 *  characteristic identified by handle.
 *
 *  \param [in] handle
 *         \ref GATT_DB_HANDLE, identifying the Characteristic whose value is Read.
 *
 *  \param [out] value
 *         Contains Characteristic Value in case of successful return of this
 *         API.
 *
 *  \return API_SUCCESS on success or an appropriate error code.
 */
#define BT_gatt_db_get_char_cli_cnfg(handle,value)\
        gatt_db_access_val_by_db_handle\
        (\
                (handle),\
                (value),\
                GATT_CLIENT_CONFIG,\
                GATT_DB_READ\
        )

/**
 *
 *  \brief To Locally Get Application/Profile defined Characteristic Descriptor
 *         Value based on \ref GATT_DB_HANDLE.
 *
 *  \par Description
 *  This routine Gets/Reads Value of the Application/Profile defined
 *  Characteristic Descriptor (referred to as HLD standing for Higher Layer
 *  Defined Descriptor) for the characteristic identified by handle.
 *  Note: Used only for characteristic with single Higher Layer Descriptor.
 *
 *  \param [in] handle
 *         \ref GATT_DB_HANDLE, identifying the Characteristic whose value is Read.
 *
 *  \param [out] value
 *         Contains Characteristic Value in case of successful return of this
 *         API.
 *
 *  \param [in] uuid
 *         UUID, identifying the Characteristic whose value is Read.
 *
 *  \return API_SUCCESS on success or an appropriate error code.
 */
#define BT_gatt_db_get_hld_char_desc_val(handle,value,uuid)\
        gatt_db_access_val_by_db_handle\
        (\
                (handle),\
                (value),\
                (uuid),\
                GATT_DB_READ\
        )

/**
 *
 *  \brief To Read/Write Attribute Value identified by Handle.
 *
 *  \par Description
 *  This routine performs read/write of Attribute Value for the requested
 *  Handle. This is used to formulate the response for Read, Read Blob, Write,
 *  Write Command. Before the access is allowed necessary Security & Permission
 *  Checks are performed for the peer and Application is notified of the access
 *  if the Application has registered a callback for the Characteristic to which
 *  the attribute belongs.
 *  This routine can be used for local updates of the database as well using
 *  necessary flags.
 *
 *  \param handle_value
 *         Contains Attribute Handle to be accessed and if the access is
 *         successful, contains the Attribute Value.
 *
 *  \param att_instance
 *         Identifies the Peer Entity Requesting access of Attribute. This is
 *         important so that necessary Security Checks are performed before the
 *         the value is fetched. In case of Local Read, this is important if
 *         client specific values are maintained.
 *
 *  \param offset
 *         Identifies the value offset from where the access is requested.
 *
 *  \param flags
 *         Describes nature of access, Read, Write, Write, Without Rsp, Local or
 *         Peer.
 *
 *  \return API_SUCCESS on success access or an appropriate error code.
 */
API_RESULT BT_gatt_db_access_handle
           (
                /* INOUT */ ATT_HANDLE_VALUE_PAIR     * handle_value,
                /* IN */    ATT_HANDLE                * att_instance,
                /* IN */    UINT16                    offset,
                /* IN */    UCHAR                     flags
           );

/**
 *
 *  \brief To Convert a 128-bit UUID in BT Space to its 16-bit equivalent.
 *
 *  \par Description
 *  This routine converts a 128-bit UUID in BT Space to its 16-bit equivalent.
 *  If the 128-bit UUID is outside the BT Space, an error is indicated.
 *
 *  \param uuid128
 *         128-bit UUID to be converted to 16-bit equivalent.
 *
 *  \param uuid16
 *         On Success contains the 16-bit equivalent of the 128-bit UUID.
 *
 *  \return API_SUCCESS on success or an appropriate error code.
 */
API_RESULT BT_gatt_db_get_16_bit_uuid
           (
                 /* IN */  ATT_VALUE    * uuid128,
                 /* OUT */ ATT_VALUE    * uuid16
           );


/**
 *
 *  \brief To Get the Security requirement of the Service
 *
 *  \par Description
 *  This routine reads the needed security level on the Service identified by
 *  the handle. Note: Even though this is a macro, it is to be treated as a
 *  function.
 *
 *  \param [in] hndl
 *         Identifies the Service whose Security needs are being read.
 *
 *  \param [out] sec
 *         Security Level and Mode of the Service.
 */
#define BT_gatt_db_get_service_security(hndl,sec)\
        ((*(UCHAR *)(sec)) = GATT_DB_GET_NEEDED_SECURITY((hndl)->service_id))


/**
 *
 *  \brief To Get the Encryption Key Size of the Service
 *
 *  \par Description
 *  This routine reads the needed encryption key size for the Service identified by
 *  the handle. Note: Even though this is a macro, it is to be treated as a
 *  function.
 *
 *  \param [in] hndl
 *         Identifies the Service whose Encryption Key Size is being read.
 *
 *  \param [out] key_size
 *         Needed Encryption Key Size for the Service.
 */
#define BT_gatt_db_get_service_enc_key_size(hndl,key_size)\
        ((*(UCHAR *)(key_size)) = GATT_DB_GET_NEEDED_ENC_KEY_SIZE((hndl)->service_id))

/**
 *
 *  \brief Book Keeping of Peer Specific Data on Disconnect or Shutdown.
 *
 *  \par Description
 *  This routine is used for Book Keeping of Peer Specific Data on Disconnect or
 *  Shutdown. For Bonded Devices, Client Specific Configurations will be stored
 *  persistently while for non bonded devices, all Client specific information
 *  will be reinitialized.
 *
 *  \param [in] inst
 *         Identifies the Peer instance for which Book Keeping is to be
 *         performed.
 *
 *  \return API_SUCCESS on success or an appropriate error code.
 */
API_RESULT BT_gatt_db_peer_session_shutdown_handler
           (
               /* IN */ ATT_HANDLE   * inst
           );

#ifdef GATT_DB_HASH_SUPPORT
/**
 *  \brief To calculate the GATT Database Hash characteristic value.
 *
 *  \par Description:
 *  This API calculates the GATT Database Hash and provides the calculated
 *  Hash value asynchronously to the application through \ref GATT_DB_HASH_IND
 *  event trough the registred callback.
 *  The application could then update the Characteristic Value of
 *  GATT DB Hash Characteristic if present in its Database.
 *
 *  \param [in] hash_cb
 *         GATT Database Hash calculation completion callback
 *         \ref GATT_DB_NTF_CB.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 *
 *  \note HAVE_GATT_DB_REGISTRATION_SUPPORT feature flag shall be defined
 *  for this API.
 */
API_RESULT BT_gatt_db_calculate_db_hash(GATT_DB_NTF_CB hash_cb);
#endif /* GATT_DB_HASH_SUPPORT */


/**
 *  \brief To verify the if the security is satisfied for a given service and
 *  the remote device corresponding to the \ref GATT_DB_HANDLE
 *
 *  \par Description:
 *  This API checks if the Security permissions corresponding to a given service
 *  is satisfied w.r.t to the corresponding remote device.
 *
 *  \param [in] handle
 *              \ref GATT_DB_HANDLE, identifies the Service and Remote Device
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_assert_security(GATT_DB_HANDLE * handle);

#ifdef GATT_DB_UUID_SEARCH_SUPPORT
/**
 *
 *  \brief To fetch the Service Instance ID of a Given UUID
 *
 *  \par Description
 *  This routine can be used to fetch the Service Instance for a given UUID.
 *
 *  \param [in] s_uuid
 *         Either a 16bit or 128bit UUID of a Service.
 *
 *  \param [in] frmt
 *         Format of the UUID
 *         1. \ref ATT_16_BIT_UUID_FORMAT or
 *         2. \ref ATT_128_BIT_UUID_FORMAT
 *
 *  \param [in] lkid
 *         Last known Service Instance ID for the same UUID to be provided
 *         by the calling module. This is needed in case of multiple instances
 *         of same Service in the Database. This can be 0xFF which corresponds
 *         to Invalid Service Index, if there is no knowledge of previous ID.
 *
 *  \param [in] is_ps
 *         \ref BT_TRUE if the requested search is for a Primary Service.
 *         \ref BT_FALSE if the requested search is for a Secondary Service.
 *
 *  \param [out] sid
 *         Service Instance ID.
 *
 *  \return API_SUCCESS on success or an appropriate error code.
 */
API_RESULT BT_gatt_db_get_sid_from_uuid
           (
               /* IN */  ATT_UUID * s_uuid,
               /* IN */  UCHAR    frmt,
               /* IN */  UCHAR    lkid,
               /* IN */  UCHAR    is_ps,
               /* OUT */ UCHAR    * sid
           );

/**
 *  \brief To fetch the Characteristic Instance ID of a Given UUID
 *
 *  \par Description
 *  This routine can be used to fetch the Characteristic Instance for a given
 *  UUID.
 *
 *  \param [in] c_uuid
 *         Either a 16bit or 128bit UUID of a Characteristic.
 *
 *  \param [in] frmt
 *         Format of the UUID
 *         1. \ref ATT_16_BIT_UUID_FORMAT or
 *         2. \ref ATT_128_BIT_UUID_FORMAT
 *
 *  \param [in] lkid
 *         Last known Characteristic Instance ID for the same UUID to be
 *         provided by the calling module. This is needed in case of multiple
 *         instances of same Characteristic inside a service in the Database.
 *         This can be 0xFF which corresponds to Invalid Characteristic
 *         Instance, if there is no knowledge of previous ID.
 *
 *  \param [in] sid
 *         Service Instance ID where this Characteristic UUID needs to be
 *         looked for.
 *
 *  \param [out] cid
 *         Characteristic Instance ID.
 *
 *  \return API_SUCCESS on success or an appropriate error code.
 */
API_RESULT BT_gatt_db_get_cid_from_uuid
           (
               /* IN */  ATT_UUID * c_uuid,
               /* IN */  UCHAR    frmt,
               /* IN */  UCHAR    lkid,
               /* IN */  UCHAR    sid,
               /* OUT */ UCHAR    * cid
           );
#endif /* GATT_DB_UUID_SEARCH_SUPPORT */


#ifdef GATT_DB_DYNAMIC
/**
 *  \brief To add a service to GATT Database
 *
 *  \par Description:
 *  This API adds a service to GATT database.
 *
 *  \param [in]  service_info        GATT Service Information.
 *  \param [in]  num_attr_handles    Number of attribute handles associated with the service.
 *  \param [out] service_handle      Service Handle returned by the API, shall be used
 *                                   when adding included services and characteristics/descriptors
 *                                   into the service.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_add_service
           (
               /* IN */   GATT_DB_SERVICE_INFO * service_info,
               /* IN */   UINT16                 num_attr_handles,
               /* OUT */  UINT16               * service_handle
           );

/**
 *  \brief To add an included service to GATT Database
 *
 *  \par Description:
 *  This API adds an included service to GATT database.
 *
 *  \param [in] service_handle             Service Handle to which the included service to be added.
 *  \param [in] included_service_handle    Service Handle of the included service.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_add_included_service
           (
               /* IN */  UINT16    service_handle,
               /* IN */  UINT16    included_service_handle
           );

/**
 *  \brief To add a characteristic to GATT Database
 *
 *  \par Description:
 *  This API adds a GATT characteristic to the GATT database.
 *
 *  \param [in]  service_handle    Service Handle to which the characteristic to be added.
 *  \param [in]  char_uuid         Characteristic UUID.
 *  \param [in]  perm              Attribute Permission of Characteristic value.
 *  \param [in]  property          Characteristic property.
 *  \param [in]  char_value        Characteristic value.
 *  \param [out] char_handle       Characteristic Handle returned by the API, shall be used
 *                                 when adding descriptors/values into the characteristic.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_add_characteristic
           (
               /* IN */  UINT16              service_handle,
               /* IN */  GATT_DB_UUID_TYPE * char_uuid,
               /* IN */  UINT16              perm,
               /* IN */  UINT16              property,
               /* IN */  ATT_VALUE         * char_value,
               /* OUT */ UINT16            * char_handle
           );

/**
 *  \brief To add a characteristic descriptor to GATT Database
 *
 *  \par Description:
 *  This API adds a GATT characteristic descriptor to the GATT database.
 *
 *  \param [in]  sh Service Handle to which the descriptor to be added.
 *  \param [in]  ch Characteristic Handle to which the descriptor to be added.
 *  \param [in]  u  Characteristic Descriptor UUID.
 *  \param [in]  p  Characteristic Descriptor access permission.
 *  \param [in]  dv Characteristic Descriptor value.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
#define BT_gatt_db_add_characteristic_descriptor(sh, ch, u, p, dv) \
        BT_gatt_db_add_characteristic_descriptor_with_property     \
        (                                                          \
            (sh),                                                  \
            (ch),                                                  \
            (u),                                                   \
            (p),                                                   \
            GATT_DB_NO_AUXILLARY_PROPERTY,                         \
            (dv)                                                   \
        );

/**
 *  \brief To add a characteristic descriptor to GATT Database
 *
 *  \par Description:
 *  This API adds a GATT characteristic descriptor to the GATT database.
 *
 *  \param [in]  service_handle    Service Handle to which the descriptor to be added.
 *  \param [in]  char_handle       Characteristic Handle to which the descriptor to be added.
 *  \param [in]  desc_uuid         Characteristic Descriptor UUID.
 *  \param [in]  perm              Characteristic Descriptor access permission.
 *  \param [in]  property          Characteristic Descriptor property.
 *  \param [in]  desc_value        Characteristic Descriptor value.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_add_characteristic_descriptor_with_property
           (
               /* IN */  UINT16              service_handle,
               /* IN */  UINT16              char_handle,
               /* IN */  GATT_DB_UUID_TYPE * desc_uuid,
               /* IN */  UINT16              perm,
               /* IN */  UINT16              property,
               /* IN */  ATT_VALUE         * desc_value
           );

/**
 *  \brief To delete a service from GATT Database
 *
 *  \par Description:
 *  This API is to delete a service from GATT database.
 *
 *  \param [in]  service_handle    Service Handle to be deleted.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_delete_service
           (
               /* IN */  UINT16                service_handle
           );

/**
 *  \brief To enable or disable a service in GATT Database
 *
 *  \par Description:
 *  This API is to enable a service in GATT database.
 *
 *  \param [in]  service_handle  Service Handle to be enabled.
 *  \param [in]  is_enable       \ref BT_TRUE - Service is to be Enabled
 *                               \ref BT_FALSE - Service is to be Disabled
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_enable_disable_service
           (
               /* IN */ UINT16 service_handle,
               /* IN */ UCHAR  is_enable
           );

/**
 *  \brief To enable a service in GATT Database
 *
 *  \par Description:
 *  This API is to enable a service in GATT database.
 *
 *  \param [in] sh    Service Handle to be enabled.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
#define BT_gatt_db_enable_service(sh) \
        BT_gatt_db_enable_disable_service((sh), BT_TRUE)

/**
 *  \brief To disable a service in GATT Database
 *
 *  \par Description:
 *  This API is to disable a service in GATT database.
 *
 *  \param [in] sh    Service Handle to be disabled.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
#define BT_gatt_db_disable_service(sh) \
        BT_gatt_db_enable_disable_service((sh), BT_FALSE)

/**
 *  \brief To fetch the current state of a Service from GATT Database
 *
 *  \par Description:
 *  This API is to fetch/get the current state of a Service from the GATT
 *  database.
 *
 *  \param [in]  service_handle  Service Handle corresponding to the service
 *                               whose state is to be known.
 *  \param [out] is_enable       Reference to a variable where the current
 *                               state of the service will be informed.
 *                               \ref BT_TRUE - Service is Enabled
 *                               \ref BT_FALSE - Service is Disabled
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_get_service_state
           (
               /* IN */  UINT16 service_handle,
               /* OUT */ UCHAR  * is_enable
           );

/**
 *  \brief To enable/disable an already included service with a given Service in the GATT Database
 *
 *  \par Description:
 *  This API enables/disables an already included service with a given Service in the GATT Database.
 *  This unhides/hides the Included Service Declaration which is present with Parent Service.
 *
 *  \param [in] service_handle           Service Handle in which the included service to be enabled/disabled.
 *  \param [in] included_service_handle  Service Handle of the included service.
 *  \param [in] is_enable                \ref BT_TRUE  - Include Service Declaration is to be Enabled
 *                                       \ref BT_FALSE - Include Service Declaration is to be Disabled
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_enable_disable_included_service
           (
               /* IN */ UINT16 service_handle,
               /* IN */ UINT16 included_service_handle,
               /* IN */ UCHAR  is_enable
           );

/**
 *  \brief To enable an already included service with a given Service in the GATT Database
 *
 *  \par Description:
 *  This API enables an already included service with a given Service in the GATT Database.
 *  This unhides the Included Service Declaration which is present with Parent Service.
 *
 *  \param [in] sh   Service Handle in which the included service to be enabled.
 *  \param [in] ish  Service Handle of the included service.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
#define BT_gatt_db_enable_included_service(sh, ish) \
        BT_gatt_db_enable_disable_included_service((sh), (ish), BT_TRUE)

/**
 *  \brief To disable an already included service with a given Service in the GATT Database
 *
 *  \par Description:
 *  This API disables an already included service with a given Service in the GATT Database.
 *  This hides the Included Service Declaration which is present with Parent Service.
 *
 *  \param [in] sh   Service Handle in which the included service to be disabled.
 *  \param [in] ish  Service Handle of the included service.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
#define BT_gatt_db_disable_included_service(sh, ish) \
        BT_gatt_db_enable_disable_included_service((sh), (ish), BT_FALSE)

/**
 *  \brief To enable or disable a characteristic in GATT Database
 *
 *  \par Description:
 *  This API is to enable a characteristic in GATT database. The said
 *  characteristic is expected to be already present in the GATT database,
 *  but the current state of the characteristic is such that it is not
 *  visible to remote device which performs GATT Discovery.
 *
 *  \param [in]  char_handle Characteristic Handle to be enabled.
 *  \param [in]  is_enable   \ref BT_TRUE - Characteristic is to be Enabled
 *                           \ref BT_FALSE - Characteristic is to be Disabled
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_enable_disable_characteristic
           (
               /* IN */ UINT16 char_handle,
               /* IN */ UCHAR  is_enable
           );

/**
 *  \brief To enable a characteristic in GATT Database
 *
 *  \par Description:
 *  This API is to enable a characteristic in GATT database. The said
 *  characteristic is expected to be already present in the GATT database,
 *  but the current state of the characteristic is such that it is not
 *  visible to remote device which performs GATT Discovery.
 *
 *  \param [in] ch    Characteristic Handle to be enabled.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
#define BT_gatt_db_enable_characteristic(ch) \
        BT_gatt_db_enable_disable_characteristic((ch), BT_TRUE)

/**
 *  \brief To disable a characteristic in GATT Database
 *
 *  \par Description:
 *  This API is to disable a characteristic in GATT database. The said
 *  characteristic is expected to be already present in the GATT database,
 *  but the current state of the characteristic is such that it is
 *  visible to remote device which performs GATT Discovery.
 *
 *  \param [in] ch    Characteristic Handle to be disabled.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
#define BT_gatt_db_disable_characteristic(ch) \
        BT_gatt_db_enable_disable_characteristic((ch), BT_FALSE)

/**
 *  \brief To fetch the current state of a Characteristic from GATT Database
 *
 *  \par Description:
 *  This API is to fetch/get the current state of a Characteristic from the GATT
 *  database.
 *
 *  \param [in]  char_handle  Characteristic Handle corresponding to the characteristic
 *                            whose state is to be known.
 *  \param [out] is_enable    Reference to a variable where the current
 *                            state of the Characteristic will be informed.
 *                            \ref BT_TRUE - Characterised is Enabled
 *                            \ref BT_FALSE - Characteristic is Disabled
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_get_characteristic_state
           (
               /* IN */  UINT16 char_handle,
               /* OUT */ UCHAR  * is_enable
           );

/**
 *  \brief To set attribute value.
 *
 *  \par Description:
 *  This API is to set attribute value in GATT database.
 *
 *  \param [in] attr_handle    Attribute Handle which is to be set.
 *  \param [in] value          Pointer to the attribute value.
 *  \param [in] length         Length of the attribute value.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_set_attr_value
           (
               /* IN */  UINT16    attr_handle,
               /* IN */  UCHAR   * value,
               /* IN */  UINT16    length
           );

/**
 *  \brief To get attribute value.
 *
 *  \par Description:
 *  This API is to get attribute value in GATT database.
 *
 *  \param [in] attr_handle    Attribute Handle which is to be retrieved.
 *  \param [in] value          Pointer to the attribute value.
 *  \param [in] length         Pointer to the length of the attribute value.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_get_attr_value
           (
               /* IN */   UINT16          attr_handle,
               /* OUT */  const UCHAR  ** value,
               /* OUT */  UINT16        * length
           );

/**
 *  \brief To register dynamic GATT Database
 *
 *  \par Description:
 *  This API registers dynamic GATT database.
 *  This shall be called after constructing the dynamic GATT database,
 *  using the exposed APIs (to add service, characteristic, descriptor etc.)
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_dyn_register(void);

/**
 *  \brief To get number of attributes in GATT Database
 *
 *  \par Description:
 *  This API is to get number of attributes in GATT database.
 *
 *  \return UINT16
 *          - Attribute Count
 */
UINT16 BT_gatt_db_get_attribute_count(void);

#ifdef GATT_DB_HAVE_DB_SIGNATURE
/**
 *  \brief To set Unique Signature for the GATT DB.
 *
 *  \par Description:
 *  This API is to set a unique signature for the GATT DB.
 *
 *  \param [in] gatt_db_sign
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_dyn_set_db_signature(GATT_DB_SIGNATURE db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

#endif /* GATT_DB_DYNAMIC */

/**
 *  \brief Fetch the Attribute Handle Range of Service
 *
 *  \par Description:
 *  This API fetches the Attribute Handle Range associated with a given
 *  Service Instance.
 *
 *  \param [in]  service_handle  Service Handle corresponding to the service
 *
 *  \param [out] service_range   Reference to corresponding handle range of the
 *                               service.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_get_service_range
           (
               /* IN */  UINT16           service_handle,
               /* OUT */ ATT_HANDLE_RANGE * service_range
           );

/**
 *  \brief Fetch the Attribute Handle Range of Characteristic
 *
 *  \par Description:
 *  This API fetches the Attribute Handle Range associated with a given
 *  Service Instance.
 *
 *  \param [in] service_handle        Service Handle corresponding to the
 *                                    service where the Characteristic belongs
 *
 *  \param [in] characteristic_handle Characteristic Handle corresponding to
 *                                    the Characteristic.
 *
 *  \param [out] characteristic_range Reference to corresponding handle range of the
 *                                    Characteristic.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_get_characteristic_range
           (
               /* IN */  UINT16           service_handle,
               /* IN */  UINT16           characteristic_handle,
               /* OUT */ ATT_HANDLE_RANGE * characteristic_range
           );

/**
 *  \brief Fetch the Descriptor Count associated with a Characteristic
 *
 *  \par Description:
 *  This API fetches Descriptor Count associated with a Characteristic.
 *
 *  \param [in] service_handle        Service Handle corresponding to the
 *                                    service where the Characteristic belongs
 *
 *  \param [in] characteristic_handle Characteristic Handle corresponding to
 *                                    the Characteristic.
 *
 *  \param [out] desc_count           Count of Descriptors associated with the
 *                                    Characteristic.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT BT_gatt_db_get_characteristic_desc_count
           (
               /* IN */  UINT16 service_handle,
               /* IN */  UINT16 characteristic_handle,
               /* OUT */ UINT16 * desc_count
           );

/** \cond ignore_this Internal interface - called only from HCI */
void gatt_db_handle_hci_command_complete
     (
         UCHAR  * packet
     );
/** \endcond */

/** \} */
/** \} */

/** \} */

API_RESULT gatt_char_handler (GATT_DB_HANDLE * handle,GATT_DB_PARAMS * param);

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_GATT_DB_API_ */
