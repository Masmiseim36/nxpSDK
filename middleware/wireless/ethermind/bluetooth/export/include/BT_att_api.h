
/**
 *  \file BT_att_api.h
 *
 *  \brief This file defines the ATT Application Interface - includes
 *  Data Structures and Methods.
 */

/*
 *  Copyright (C) 2014. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_ATT_API_
#define _H_BT_ATT_API_


/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_device_queue.h"
#include "l2cap.h"

/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_protocol Protocols
 * \{
 */

/**
 * \defgroup att_module ATT (Attribute Protocol)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Attribute Protocol (ATT) module to the Application and other upper
 *  layers of the stack.
 */

/**
 * \defgroup att_defines Defines
 * \{
 * Describes defines for the module.
 */

/**
 * \defgroup att_constants Constants
 * \{
 * Describes Constants defined by the module.
 */

/** ATT Default MTU */
#define ATT_DEFAULT_MTU                   23U

/** ATT Signature Size in Signed Write */
#define ATT_AUTH_SIGNATURE_SIZE           12U

/** ATT Execute Write Cancel Flag */
#define ATT_EXECUTE_WRITE_CANCEL_FLAG   0x00U

/** ATT Execute Write Execute Flag */
#define ATT_EXECUTE_WRITE_EXEC_FLAG     0x01U

/** ATT Identification for 16-bit UUID Format */
#define ATT_16_BIT_UUID_FORMAT          0x01U

/** ATT Identification for 128-bit UUID Format */
#define ATT_128_BIT_UUID_FORMAT         0x02U

/** ATT 16-bit UUID Size */
#define ATT_16_BIT_UUID_SIZE               2U

/** ATT 128-bit UUID Size */
#define ATT_128_BIT_UUID_SIZE             16U

/** ATT Connection Instance Initialization Value. */
#define ATT_CON_ID_INIT_VAL             0xFFU

/** ATT Application Callback Initialization Value. */
#define ATT_APPL_CB_INIT_VAL               NULL

/** ATT Invalid Attribute Handle Value */
#define ATT_INVALID_ATTR_HANDLE_VAL      0x0000U

/** ATT Attribute Handle Start Range */
#define ATT_ATTR_HANDLE_START_RANGE      0x0001U

/** ATT Attribute Handle End Range */
#define ATT_ATTR_HANDLE_END_RANGE        0xFFFFU

/** \} */

/** \} */

/**
 * \defgroup att_error_codes Error Codes
 * \{
 *  This section describes the defines for ATT protocol specified error codes
 *  for ATT Error Response PDU
 *
 */

/**
 * Invalid Handle error code is used in the case when the ATT handle in the
 * ATT request PDU is invalid.
 */
#define ATT_INVALID_HANDLE                          0x01U

/**
 * Read Not Permitted error code is used in the case when the permission to
 * read the value of an ATT handle is not permitted on the ATT server.
 */
#define ATT_READ_NOT_PERMITTED                      0x02U

/**
 * write Not Permitted error code is used in the case when the permission to
 * write the value of an ATT handle is not permitted on the ATT server.
 */
#define ATT_WRITE_NOT_PERMITTED                     0x03U

/**
 * Invalid PDU error code is used in the case when the format of the PDU sent
 * from the ATT Client is incorrect.
 */
#define ATT_INVALID_PDU                             0x04U

/**
 * Insufficient Authentication error code is used in the case when
 * an access to a handle is attempted on a un-authenticated link but the
 * attribute requires that the link be authenticated before any client can access
 * it.
 */
#define ATT_INSUFFICIENT_AUTHENTICATION             0x05U

/**
 * Request not supported error code is used in the case when
 * the server does not support the processing of an ATT request sent from the
 * client.
 */
#define ATT_REQUEST_NOT_SUPPORTED                   0x06U

/**
 * Invalid Offset error code is used in the case when
 * the offset sent by the client in the Read blob/Prepare Write Request is
 * invalid with respect to the length of the value in the server.
 */
#define ATT_INVALID_OFFSET                          0x07U

/**
 * Insufficient Authorization error code is used in the case when
 * the ATT server does not Authorize the client and hence prohibiting the
 * client from reading the handle value.
 */
#define ATT_INSUFFICIENT_AUTHORIZATION              0x08U

/**
 * Write queue full error code is used when there is no more space left in the
 * prepare write queue on the server to entertain any more prepare writes from a
 * client.
 */
#define ATT_PREPARE_WRITE_QUEUE_FULL                0x09U

/**
 * Attribute not found error is used when the ATT server cannot find any handles
 * that belong to the Attribute type in the given range of handles that the
 * client specified in its request. This error code can be sent to the client in
 * response to the following request PDUs - Find Information, Find by Type Value,
 * Read by Type, Read by Group Type requests.
 */
#define ATT_ATTRIBUTE_NOT_FOUND                     0x0AU

/**
 * Attribute Not Long error code is used when the client tries to read or write
 * a Attribute handle's value which cannot be read or written through Read Blob
 * or multiple prepare write requests.
 */
#define ATT_ATTRIBUTE_NOT_LONG                      0x0BU

/**
 * Insufficient encryption key size error code is used when the client tries to
 * access an Attribute Handle's Value for which the link need to be encrypted
 * with a key of certain minimum key size and the current link is encrypted
 * with a key of lesser size than the minimum required.
 */
#define ATT_INSUFFICIENT_ENC_KEY_SIZE               0x0CU

/**
 * Invalid Attribute length error code is used when the Attribute value's
 * length is not correct to process the request containing the value.
 */
#define ATT_INVALID_ATTRIBUTE_LEN                   0x0DU

/**
 * Unlikely error is used when the processing of the Attribute request has
 * encountered an error that is not covered by any other error code.
 */
#define ATT_UNLIKELY_ERROR                          0x0EU

/**
 * Insufficient encryption error code is used when the client tries to read or
 * write an Attribute handle which requires the link to be encrypted and the
 * link is currently not encrypted.
 *
 */
#define ATT_INSUFFICIENT_ENCRYPTION                 0x0FU

/**
 * Unsupported Group Type error code is used when the Attribute type requested
 * in the Read by Group Type request is not a valid grouping attribute on the
 * server.
 */
#define ATT_UNSUPPORTED_GROUP_TYPE                  0x10U

/**
 * Insufficient Resources error code is used when the ATT server does not have
 * enough resources such as memory etc. to process the request from the client.
 */
#define ATT_INSUFFICIENT_RESOURCES                  0x11U

/**
 * Database Out of Sync error code is used when the ATT server has updated/changed
 * or modified its Attribute Database which the Peer Client is unaware of.
 */
#define ATT_DB_OUT_OF_SYNC                          0x12U

/**
 * Attribute parameter value not allowed error code is used when the ATT client
 * attempts to set an attribute value that is not allowed in the current
 * circumstances.
 */
#define ATT_PARAM_VAL_NOT_ALLOWED                   0x13U

/**
 * Client Characteristic Configuration Descriptor Improperly Configured error
 * code is used when a Client Characteristic Configuration descriptor is not
 * configured according to the requirements of the profile or service.
 */
#define ATT_CCD_IMPROPERLY_CONFIGURED               0xFDU

/**
 * Procedure Already in Progress error code is used when a profile or service
 * request cannot be serviced because an operation that has been previously
 * triggered is still in progress.
 */
#define ATT_PROCEDURE_ALREADY_IN_PROGRESS           0xFEU

/**
 * Out of Range error code is used when an attribute value is out of range as
 * defined by a profile or service specification.
 */
#define ATT_OUT_OF_RANGE                            0xFFU

/** \} */

/**
 *  \defgroup att_events Events
 *  \{
 *  This section lists the Asynchronous Events notified to Application by the
 *  Module.
 */

/**
 *  \defgroup att_transport_events Transport Events
 *  \{
 *  Events Notified to application on transport connection/disconnection in the
 *  asynchronous notification callback \ref ATT_NTF_CB in \ref ATT_APPLICATION
 *  registered using \ref BT_att_register.
 */
/** Request from Peer to estalish Connection Oriented Channel for ATT */
#define ATT_CONNECT_REQ           0x80U

/** Transport Connection Event */
#define ATT_CONNECTION_IND        0x81U

/** Transport Disconnection Event */
#define ATT_DISCONNECTION_IND     0x82U

/** Transport Flow Control Indication (On or Off) for Transmission */
#define ATT_TX_FLOW_IND           0x90U
/** \} */

/**
 *  \defgroup att_pdu_events PDU Events
 *  \{
 * Events Notified on receiving PDUs from Peer, or on Request Timeouts
 * or Generated Locally.
 *
 * Most of these events are associated with ATT PDUs which are defined
 * in ATT specification section 'ATTRIBUTE PROTOCL PDUS'.
 * The parameters associated with these events are the same as defined in
 * the refered ATT section.
 *
 * For the locally generated events, the associated parameters are described
 * along with the event.
 *
 * In the ATT Callback \ref att_cb, ATT PDU Event is provided as 'att_event'
 * and associated parameter in octet stream as 'data_param'.
 */

/** ATT Error Response PDU */
#define ATT_ERROR_RSP              0x01U

/** ATT Exchange MTU Request PDU */
#define ATT_XCHNG_MTU_REQ          0x02U

/** ATT Exchange MTU Response PDU */
#define ATT_XCHNG_MTU_RSP          0x03U

/** ATT Find Information Request PDU */
#define ATT_FIND_INFO_REQ          0x04U

/** ATT Find Information Response PDU */
#define ATT_FIND_INFO_RSP          0x05U

/** ATT Find By Type Value Request PDU */
#define ATT_FIND_BY_TYPE_VAL_REQ   0x06U

/** ATT Find By Type Value Response PDU */
#define ATT_FIND_BY_TYPE_VAL_RSP   0x07U

/** ATT Read By Type Request PDU */
#define ATT_READ_BY_TYPE_REQ       0x08U

/** ATT Read By Type Response PDU */
#define ATT_READ_BY_TYPE_RSP       0x09U

/** ATT Read Request PDU */
#define ATT_READ_REQ               0x0AU

/** ATT Read Response PDU */
#define ATT_READ_RSP               0x0BU

/** ATT Read Blob Request PDU */
#define ATT_READ_BLOB_REQ          0x0CU

/** ATT Read Blob Response PDU */
#define ATT_READ_BLOB_RSP          0x0DU

/** ATT Read Multiple Request PDU */
#define ATT_READ_MULTIPLE_REQ      0x0EU

/** ATT Read Multiple Response PDU */
#define ATT_READ_MULTIPLE_RSP      0x0FU

/** ATT Read Group Type Request PDU */
#define ATT_READ_BY_GROUP_REQ      0x10U

/** ATT Read Group Type Response PDU */
#define ATT_READ_BY_GROUP_RSP      0x11U

/** ATT Write Request PDU */
#define ATT_WRITE_REQ              0x12U

/** ATT Write Response PDU */
#define ATT_WRITE_RSP              0x13U

/** ATT Write Command PDU */
#define ATT_WRITE_CMD              0x52U

/**
 * ATT Write Command - Transmission Complete - Locally generated event.
 *
 * Associated parameters 'Attribute Handle' and 'Attribute Value' are
 * as described in 'ATTRIBUTE PROTOCL PDUS' sub-section 'Write Command'
 * in the ATT specification.
 */
#define ATT_WRITE_CMD_TX_COMPLETE  0x53U

/** ATT Signed Write Command PDU */
#define ATT_SIGNED_WRITE_CMD       0xD2U

/**
 * ATT Signed Write Command - Transmission Complete - Locally generated event.
 *
 * Associated parameters 'Attribute Handle', 'Attribute Value' and
 * 'Authentication Signature' are as described in 'ATTRIBUTE PROTOCL PDUS'
 * sub-section 'Signed Write Command' in the ATT specification.
 */
#define ATT_SIGNED_WRITE_CMD_TX_COMPLETE \
                                   0xD3U

/** ATT Prepare Write Request PDU */
#define ATT_PREPARE_WRITE_REQ      0x16U

/** ATT Prepare Write Response PDU */
#define ATT_PREPARE_WRITE_RSP      0x17U

/** ATT Execute Write Request PDU */
#define ATT_EXECUTE_WRITE_REQ      0x18U

/** ATT Execute Write Response PDU */
#define ATT_EXECUTE_WRITE_RSP      0x19U

/** ATT Handle Value Notification PDU */
#define ATT_HANDLE_VALUE_NTF       0x1BU

/**
 * ATT Handle Value Notification - Transmission Complete - Locally generated
 * event.
 *
 * Associated parameters 'Attribute Handle' and 'Attribute Value' are
 * as described in 'ATTRIBUTE PROTOCL PDUS' sub-section 'Handle Value
 * Notification' in the ATT specification.
 */
#define ATT_HANDLE_VALUE_NTF_TX_COMPLETE \
                                   0x1CU

/** ATT Handle Value Indication PDU */
#define ATT_HANDLE_VALUE_IND       0x1DU

/** ATT Handle Value Confirmation PDU */
#define ATT_HANDLE_VALUE_CNF       0x1EU

/** ATT Read Multiple Variable Length Request PDU */
#define ATT_READ_MULTIPLE_VARIABLE_LENGTH_REQ \
                                   0x20U

/** ATT Read Multiple Variable Length Response PDU */
#define ATT_READ_MULTIPLE_VARIABLE_LENGTH_RSP \
                                   0x21U

/** ATT Handle Value Multiple Notification PDU */
#define ATT_HANDLE_VALUE_MULTIPLE_NTF \
                                   0x23U

/** ATT Handle Value Multiple Notification - Transmission Complete - Locally generated event */
#define ATT_HANDLE_VALUE_MULTIPLE_NTF_TX_COMPLETE \
                                   0x24U

/** Unknown or Unhandled PDU */
#define ATT_UNKNOWN_PDU_IND        0xFFU

/** \} */

#define ATT_ACCEPT_CONNECTION       0x00U
#define ATT_REJECT_CONNECTION       0x01U

/** \} */

/**
 *  \defgroup att_marcos Utility Macros
 *  \{
 *  Initialization and other Utility Macros offered by the module.
 */

/** ATT Handle Initialization Macro */
#define ATT_INIT_HANDLE(handle)\
        DEVICE_HANDLE_INIT ((handle).device_id);\
        (handle).att_id = ATT_CON_ID_INIT_VAL

/** ATT Application Information Intialization Macro */
#define ATT_INIT_APPLICATION(appl_inst)\
        (appl_inst)->cb = ATT_APPL_CB_INIT_VAL;

/** ATT Macro to Initialize ATT Value */
#define ATT_COPY_VALUE(dest,src,length)\
        BT_mem_copy((dest)->val, (src), (length));\
        (dest)->len = (length);

#define ATT_CHECK_VALID_HANDLE_RANGE(strt_hndl,end_hndl)\
        (((ATT_INVALID_ATTR_HANDLE_VAL == (strt_hndl))\
        || (ATT_INVALID_ATTR_HANDLE_VAL == (end_hndl))\
        || ((strt_hndl) > (end_hndl)))? BT_TRUE : BT_FALSE)

/** ATT Marco to extract 16 Bit UUID */
#ifdef ATT_SUPPORT_128_BIT_UUID
#define ATT_GET_16_BIT_UUID(uuid)  (uuid)->uuid_16
#else /* ATT_SUPPORT_128_BIT_UUID */
#define ATT_GET_16_BIT_UUID(uuid)  *(uuid)
#endif /* ATT_SUPPORT_128_BIT_UUID */

/** ATT Marco to set 16 Bit UUID */
#ifdef ATT_SUPPORT_128_BIT_UUID
#define ATT_SET_16_BIT_UUID(uuid,uuid_16_bit) ((uuid)->uuid_16 = (uuid_16_bit))
#else /* ATT_SUPPORT_128_BIT_UUID */
#define ATT_SET_16_BIT_UUID(uuid,uuid_16_bit) (*(uuid) = (uuid_16_bit))
#endif /* ATT_SUPPORT_128_BIT_UUID */

/** ATT Macro to Unpack 16-bit or 128-bit UUID */
#define ATT_UNPACK_UUID(uuid_out, buffer, size)         \
        if (ATT_128_BIT_UUID_SIZE == (size))            \
        {                                               \
            BT_mem_copy ((uuid_out), (buffer), (size)); \
        }                                               \
        else if (ATT_16_BIT_UUID_SIZE == (size))        \
        {                                               \
            BT_UNPACK_LE_2_BYTE((uuid_out), (buffer));  \
        }                                               \
        else                                            \
        {                                               \
            /* MISRA C-2012 Rule 15.7 */                \
        }

/** \} */

/* --------------------------------------------- Data Types/ Structures */

/**
 *  \addtogroup att_defines Defines
 *  \{
 */

/**
 *  \addtogroup att_structures Structures
 *  \{
 */

/** Identifies the  connection entity. */
typedef UCHAR ATT_CON_ID;


/** ATT Handle */
typedef struct
{
    /** Identifies the peer instance */
    DEVICE_HANDLE     device_id;

    /** Identifies the ATT Instance */
    ATT_CON_ID        att_id;

}ATT_HANDLE;

/** \} */

/** \} */

/**
 *  \defgroup att_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
/**
 * ATT Application Asynchronous Notification Callback.
 *
 * ATT calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param handle Reference identifying ATT Instance on which event occurred.
 * \param att_event Any of the Event occurred from the list at \ref att_events.
 * \param event_result \ref API_SUCCESS or an error code in
 * \ref att_error_codes or any other internal error code from the stack.
 * \param data_param Data or Parameter associated with the event if any or NULL.
 * \param data_len Size of the data_param. 0 if data_param is NULL.
 */
typedef API_RESULT (*ATT_NTF_CB)
        (
            ATT_HANDLE    * handle,
            UCHAR         att_event,
            API_RESULT    event_result,
            UCHAR         * data_param,
            UINT16        data_len
        ) DECL_REENTRANT;
/** \} */

/**
 *  \addtogroup att_defines Defines
 *  \{
 */

/**
 *  \addtogroup att_structures Structures
 *  \{
 */

/**
 *  Application details, which currently includes the notification callback used
 *  to notify it of ATT Events \ref att_events.
 */
typedef struct
{
    /** Call Back Function */
    ATT_NTF_CB    cb;

}ATT_APPLICATION;

/** ATT Attribute Handle */
typedef UINT16 ATT_ATTR_HANDLE;

/** ATT Attribute Handle Range */
typedef struct
{
    /** Start Handle */
    ATT_ATTR_HANDLE    start_handle;

    /** End Handle */
    ATT_ATTR_HANDLE    end_handle;

}ATT_HANDLE_RANGE;


/** Abstracts 16 Bit UUID */
typedef UINT16 ATT_UUID16;

/** Abstracts 128 Bit UUID */
typedef struct
{
    UCHAR   value[ATT_128_BIT_UUID_SIZE];
}ATT_UUID128;

#ifdef ATT_SUPPORT_128_BIT_UUID
/** Abstracts ATT UUID */
typedef union
{
    ATT_UUID16    uuid_16;
    ATT_UUID128   uuid_128;

}ATT_UUID;
#else /* ATT_SUPPORT_128_BIT_UUID */

/** Abstracts ATT UUID */
typedef ATT_UUID16 ATT_UUID;
#endif /* ATT_SUPPORT_128_BIT_UUID */

/**
 *  Abstracts Handle UUID Pair.
 */
typedef struct
{
    /** Attribute UUID */
    ATT_UUID           uuid;

    /** Attribute Handle */
    ATT_ATTR_HANDLE    handle;

}ATT_HANDLE_UUID_PAIR;

/**
 *  Abstracts Variable Length Values for ATT.
 *  Apart from data, and length, 'actual length' is needed so that ATT can
 *  indicate to the application actual length of data processed for a PDU.
 *
 *  Is used in multiple commands - see \ref ATT_READ_RSP_PARAM,
 *  \ref ATT_FIND_BY_TYPE_VAL_REQ_PARAM, \ref ATT_READ_BLOB_RSP_PARAM etc.
 *
 *  In ATT Read Response for example, if the attribute length is 30 octects,
 *  and the MTU is 23 octets, the only the first 22 octets can be sent by ATT,
 *  therefore actual length will be 22 (MTU-1).
 *  However, if the ATT MTU is configured to be 54 for example, all 30 octets
 *  can be transmitted and the actual length will be 30.
 *
 *  In case multiple values are being packed, the actual length processed will
 *  depend on the available MTU.
 */
typedef struct
{
    /** Value to be packed */
    UCHAR    * val;

    /** Length of Value */
    UINT16   len;

    /** Out Parameter Indicating Actual Length Packed */
    UINT16   actual_len;

}ATT_VALUE;

/** Abstracts Value List */
typedef struct
{
    /** Values to be packed */
    ATT_VALUE    * value;

    /** Number of values to be packed */
    UINT16       count;

    /** Number of elements packed */
    UINT16       actual_count;

}ATT_VALUE_LIST;

/**
 *  Abstracts Handle List needed for Read Multiple Request.
 *  See \ref ATT_READ_MULTIPLE_REQ_PARAM and \ref BT_att_send_read_multiple_req
 */
typedef struct
{
    /** Handle list where the UUID with value Indicated is found */
    UINT16    * handle_list;

    /** Number of Handles in the list */
    UINT16    list_count;

    /** Actual Number of Handles Packed */
    UINT16    actual_count;

}ATT_HANDLE_LIST;

/**
 *  Abstracts list of Handle UUID pair needed for Find Information Response.
 *  See \ref ATT_FIND_INFO_RSP_PARAM and \ref BT_att_send_find_info_rsp
 */
typedef struct
{
    /** Array of Handle UUID Pair */
    ATT_HANDLE_UUID_PAIR    * list;

    /** Number of Elements in the Array */
    UINT16                  list_count;

    /** Out paramater indicating number of pairs actually sent */
    UINT16                  actual_count;

}ATT_HANDLE_UUID_LIST;

/**
 *  Abstracts Handle Value Pair
 *  This is used in multiple PDUs - see \ref ATT_WRITE_REQ_PARAM,
 *  \ref ATT_WRITE_CMD_PARAM etc.
 */
typedef struct
{
    /** Attribute Value  */
    ATT_VALUE          value;

    /** Attribute Handle */
    ATT_ATTR_HANDLE    handle;

}ATT_HANDLE_VALUE_PAIR;

/** Abstracts Handle Value Pair List */
typedef struct
{
    /** Handle Value Pair List */
    ATT_HANDLE_VALUE_PAIR    * handle_value_list;

    /** Number of pairs in the list */
    UINT16                   count;

    /** Number actually packed */
    UINT16                   actual_count;

}ATT_HANDLE_VALUE_PAIR_LIST;


/**
 * Abstracts Error Response Parameters
 *
 * See \ref BT_att_send_error_rsp
 */
typedef struct
{
    /** Handle in which error is generated */
    ATT_ATTR_HANDLE    handle;

    /** Opcode which has resulted in Error */
    UCHAR              op_code;

    /** Error Code describing cause of error */
    UCHAR              error_code;

}ATT_ERROR_RSP_PARAM;

/**
 * Abstracts Exchange MTU Request Parameters
 *
 * See \ref BT_att_send_mtu_xcnhg_req
 */
typedef struct
{
    /** Client Rx MTU Size */
    UINT16    mtu;

}ATT_XCHG_MTU_REQ_PARAM;

/**
 * Abstracts Exchange MTU Response Parameters
 *
 * See \ref BT_att_send_mtu_xcnhg_rsp
 */
typedef struct
{
    /** Server Rx MTU Size */
    UINT16    mtu;

}ATT_XCHG_MTU_RSP_PARAM;

/**
 * Abstracts Find Information Request Parameters
 *
 * See \ref BT_att_send_find_info_req
 */
typedef ATT_HANDLE_RANGE ATT_FIND_INFO_REQ_PARAM;

/**
 * Abstracts Find Information Response Parameters
 *
 * See \ref BT_att_send_find_info_rsp
 */
typedef struct
{
    /** Handle Value List */
    ATT_HANDLE_UUID_LIST    handle_value_list;

    /** Format indicating, 16 bit or 128 bit UUIDs */
    UCHAR                   uuid_format;
}ATT_FIND_INFO_RSP_PARAM;

/**
 * Abstracts Find By Type Value Request Parameters
 *
 * See \ref BT_att_send_find_by_type_val_req
 */
typedef struct
{
    /** Attribute Value to Find */
    ATT_VALUE           value;

    /** Handle Range - Start and End Handle */
    ATT_HANDLE_RANGE    range;

    /** 16-bit UUID to Find */
    ATT_UUID16          uuid;

}ATT_FIND_BY_TYPE_VAL_REQ_PARAM;

/**
 * Abstracts Find By Type Value Response Parameters
 *
 * See \ref BT_att_send_find_by_type_val_rsp
 */
typedef struct
{
    /** Handle Range List */
    ATT_HANDLE_RANGE     * range;

    /** Size of List */
    UINT16               count;

    /** Actual number of Handle Range packed */
    UINT16               actual_count;

}ATT_FIND_BY_TYPE_VAL_RSP_PARAM;


/**
 * Read By Type Request Parameters
 *
 * See \ref BT_att_send_read_by_type_req
 */
typedef struct
{
    /** Handle Range */
    ATT_HANDLE_RANGE    range;

    /** UUID */
    ATT_UUID           uuid;

    /** Format indicating, 16 bit or 128 bit UUIDs */
    UCHAR               uuid_format;

}ATT_READ_BY_TYPE_REQ_PARAM;

/**
 * Read By Type Response Parameters
 *
 * See \ref BT_att_read_by_type_rsp
 */
typedef ATT_HANDLE_VALUE_PAIR_LIST ATT_READ_BY_TYPE_RSP_PARAM;

/**
 * Read Request Parameters - Attribute handle is the only parameter
 *
 * See \ref BT_att_send_read_req
 */
typedef ATT_ATTR_HANDLE ATT_READ_REQ_PARAM;

/**
 * Read Response Parameters - Length, Value are the only parameters
 *
 * See \ref BT_att_read_rsp
 */
typedef ATT_VALUE ATT_READ_RSP_PARAM;

/**
 * Read Blob Request Parameters
 *
 * See \ref BT_att_send_read_blob_req
 */
typedef struct
{
    /** Handle on which Read Blob is requested */
    ATT_ATTR_HANDLE    handle;

    /** Value Offset from which the Read is Requested */
    UINT16             offset;

}ATT_READ_BLOB_REQ_PARAM;

/**
 * Read Blob Response Parameters - Length, Value are the only parameters
 *
 * See \ref BT_att_read_blob_rsp
 */
typedef ATT_VALUE ATT_READ_BLOB_RSP_PARAM;

/**
 * Read Multiple Request Parameters
 *
 * See \ref BT_att_send_read_multiple_req
 */
typedef ATT_HANDLE_LIST ATT_READ_MULTIPLE_REQ_PARAM;

/**
 * Read Multiple Response Parameters
 *
 * See \ref BT_att_read_multiple_rsp
 */
/* <comment> is sending partial value allowed? */
typedef ATT_VALUE_LIST ATT_READ_MULTIPLE_RSP_PARAM;

/**
 * Read By Group Type Request Parameters
 *
 * See \ref BT_att_send_read_by_group_req
 */
typedef struct
{
    /** Handle Range - Start and End Handle */
    ATT_HANDLE_RANGE    range;

    /** UUID representing an Attribute Group Type */
    ATT_UUID            group_type;

    /** UUID Format - 16-bit or 128-bit */
    UCHAR               uuid_format;

}ATT_READ_BY_GROUP_TYPE_REQ_PARAM;

/** Abstracts Attribute Data Element for Read By Group Type Response */
typedef struct
{
    /** Group attribute value, typically UUID */
    UCHAR              * attr_value;

    /** Group Handle Range */
    ATT_HANDLE_RANGE    range;

}ATT_GROUP_ATTR_DATA_ELEMENT;

/** Abstracts Data Element for Group Response */
typedef struct
{
    /** List containing Group Attribute Data Elements with Handle Range */
    ATT_GROUP_ATTR_DATA_ELEMENT    * list;

    /** Length of each Attribute Data Element including the Handle Range */
    UINT16                         length;

    /** Number of Attribute Data Elements */
    UINT16                         count;

    /** Actual Attribute Data Elements packed */
    UINT16                         actual_count;

}ATT_GROUP_ATTR_DATA_LIST;

/**
 * Abstracts Read By Group Type Response Parameters
 *
 * See \ref BT_att_read_by_group_rsp
 */
typedef ATT_GROUP_ATTR_DATA_LIST ATT_READ_BY_GROUP_TYPE_RSP_PARAM;

/**
 * Write Request Parameters
 *
 * See \ref BT_att_send_write_req
 */
typedef ATT_HANDLE_VALUE_PAIR ATT_WRITE_REQ_PARAM;

/** Currently No Write Response Parameters */

/**
 * Write Command Parameters
 *
 * See \ref BT_att_send_write_cmd
 */
typedef ATT_HANDLE_VALUE_PAIR ATT_WRITE_CMD_PARAM;

/**
 * Signed Write Command Parameters
 *
 * See \ref BT_att_send_signed_write_cmd
 */
typedef struct
{
    /** Value to be Written */
    ATT_HANDLE_VALUE_PAIR    handle_value;

    /** Signature for Write */
    UCHAR        auth_signature[ATT_AUTH_SIGNATURE_SIZE];

}ATT_SIGNED_WRITE_CMD_PARAM;

/**
 * Abstracts Handle Value Pair along with offset.
 * Used in Prepare Write PDUs - see \ref ATT_PREPARE_WRITE_REQ_PARAM
 * \ref ATT_PREPARE_WRITE_RSP_PARAM.
 */
typedef struct
{
    /** Attribute Handle & Value to be Written */
    ATT_HANDLE_VALUE_PAIR    handle_value;

    /** Offset at which Write is to be performed */
    UINT16                   offset;

}ATT_HANDLE_VALUE_OFFSET_PARAM;

/**
 * Prepare Write Request Parameters
 *
 * See \ref BT_att_send_prepare_write_req
 */
typedef ATT_HANDLE_VALUE_OFFSET_PARAM ATT_PREPARE_WRITE_REQ_PARAM;

/**
 * Prepare Write Response Parameters
 *
 * See \ref BT_att_send_prepare_write_rsp
 */
typedef ATT_HANDLE_VALUE_OFFSET_PARAM ATT_PREPARE_WRITE_RSP_PARAM;

/**
 * Execute Write Request Parameters
 *
 * See \ref BT_att_send_execute_write_req
 */
typedef struct
{
    /** Flags indicating if Queued Writes are to be executed or Canceled */
    UCHAR    flags;

}ATT_EXECUTE_WRITE_REQ_PARAM;


/** Currently No Execute Write Response Parameters */

/**
 * Read Multiple Variable Length Request Parameters
 *
 * See \ref BT_att_send_read_multiple_variable_length_req
 */
typedef ATT_HANDLE_LIST ATT_READ_MULTIPLE_VARIABLE_LENGTH_REQ_PARAM;

/**
 * Read Multiple Variable Length Response Parameters
 *
 * See \ref BT_att_send_read_multiple_variable_length_rsp
 */
typedef ATT_VALUE_LIST ATT_READ_MULTIPLE_VARIABLE_LENGTH_RSP_PARAM;

/**
 * Handle Value Notification Parameters
 *
 * See \ref BT_att_send_hndl_val_ntf
 */
typedef ATT_HANDLE_VALUE_PAIR ATT_HNDL_VAL_NTF_PARAM;

/**
 * Handle Value Indication Parameters
 *
 * See \ref BT_att_send_hndl_val_ind
 */
typedef ATT_HANDLE_VALUE_PAIR ATT_HNDL_VAL_IND_PARAM;

/* Handle Value Confirmation Parameters - None currently */

/**
 * Handle Value Multiple Notification Parameters
 *
 * See \ref BT_att_send_hndl_val_multiple_ntf
 */
typedef ATT_HANDLE_VALUE_PAIR_LIST ATT_HNDL_VAL_MULTIPLE_NTF_PARAM;

#ifdef ATT_ON_ECBFC_SUPPORT
/**
 * Information related to Enhanced ATT channel.
 *
 * See \ref BT_att_ecbfc_get_info
 */
typedef struct
{
    /** Negotiated MTU of the EATT Instance */
    UINT16   mtu;

    /** Local MPS of the EATT Instance */
    UINT16   l_mps;

    /** Remote MPS of the EATT Instance */
    UINT16   r_mps;

}ATT_ECBFC_INFO;
#endif /* ATT_ON_ECBFC_SUPPORT */
/** \} */

/** \} */

/* --------------------------------------------- Function */

/**
 * \defgroup att_api_defs API Definitions
 * \{
 * This section describes the EtherMind Attribute Protocol APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \brief Register Interface with ATT
 *
 *  \par Description:
 *       This routine registers interface with the ATT
 *
 *  \param [in] appl_info
 *         Details describing Application Notification Callback
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_att_register
           (
                /* IN */ ATT_APPLICATION    * appl_info
           );

/**
 *  \brief Common API to send ATT PDUs
 *
 *  \par Description:
 *  This common routine is used to send all type of ATT PDUs to peer device.
 *  ATT PDU types can be Requests, Responses, Commands, Notifications,
 *  Indications and Confirmations.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which the PDU is to be
 *         sent
 *
 *  \param [in] pdu_id
 *         Identifies the ATT PDU Type
 *
 *  \param [in] params
 *         PDU Specific parameter(s)
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_att_send_pdu
           (
                /* IN */ ATT_HANDLE    * handle,
                /* IN */ UCHAR         pdu_id,
                /* IN */ void          * params
           );

#ifdef ATT_ON_BR_EDR_SUPPORT
/**
 *  \brief To establish ATT connection for L2CAP Credit Based Flow Control mode.
 *
 *  \par Description:
 *  This ATT interface initiates L2CAP channel establishment
 *  procedure for CBFC mode, to the specified remote Bluetooth device over BR/EDR.
 *
 *  \param [in, out] handle
 *         Identifies the remote ATT entity to which the connection to be
 *         requested.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_att_connect_req
           (
                /* INOUT */ ATT_HANDLE    * handle
           );
/**
 *  \brief To respond to an incoming ATT connection request for
 *  L2CAP Credit Based Flow Control mode.
 *
 *  \par Description:
 *  This ATT interface enables an upper layer to respond
 *  to ATT connection request for L2CAP Credit Based Flow Control mode over BR/EDR.
 *  It is mandatory that the upper layer always responds back by calling
 *  this interface upon receiving ATT CBFC Connection Request.
 *
 *  \param [in] handle
 *         This parameter specifies a list of ATT entities to which the connection
 *         response to be sent.
 *  \param [in] response
 *         This parameter specifies the response of the upper layer for
 *         the new ATT connection establishment requests from the peer.
 *         It must be set to a value as specified in L2CAP Connect Result Codes.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_att_connect_rsp
           (
                /* IN */ ATT_HANDLE    * handle,
                /* IN */ UCHAR         response
           );
/**
 *  \brief To disconnect an established ATT CBFC connection.
 *
 *  \par Description:
 *  This ATT interface initiates disconnection of the referred ATT CBFC connection over BR/EDR.
 *  Disconnection of the ATT connection always succeeds - either by reception
 *  of the Disconnect Response message from the peer, or by timeout.
 *  In any case, ATT will confirm disconnection of the connection,
 *  by calling the registered ATT callback.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity which is to be disconnected.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_att_disconnect_req
           (
                /* IN */ ATT_HANDLE    * handle
           );

#endif /* ATT_ON_BR_EDR_SUPPORT */

#ifdef ATT_ON_ECBFC_SUPPORT

/**
 *  \brief To establish ATT connection for L2CAP Enhanced Credit Based Flow Control mode.
 *
 *  \par Description:
 *  This ATT interface initiates L2CAP channel establishment
 *  procedure for ECBFC mode, to the specified remote Bluetooth device.
 *
 *  \param [in, out] handle
 *         Identifies the remote ATT entity to which the connection to be
 *         requested.
 *  \param [in] ecbfc_connect_param
 *         Connection parameters for L2CAP ECBFC mode.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_att_ecbfc_connect_req
           (
                /* INOUT */ ATT_HANDLE                * handle,
                /* IN */    L2CAP_ECBFC_CONNECT_PARAM * ecbfc_connect_param
           );

/**
 *  \brief To respond to an incoming ATT connection request for
 *  L2CAP Enhanced Credit Based Flow Control mode.
 *
 *  \par Description:
 *  This ATT interface enables an upper layer to respond
 *  to ATT connection request for L2CAP Enhanced Credit Based Flow Control mode.
 *  It is mandatory that the upper layer always responds back by calling
 *  this interface upon receiving ATT ECBFC Connection Request.
 *
 *  \param [in] handle
 *         This parameter specifies a list of ATT entities to which the connection
 *         response to be sent.
 *  \param [in] num_handles
 *         This parameter specifies the number of ATT entities to which the connection
 *         response to be sent.
 *  \param [in] response
 *         This parameter specifies the response of the upper layer for
 *         the new ATT connection establishment requests from the peer.
 *         It must be set to a value as specified in L2CAP Connect Result Codes.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_att_ecbfc_connect_rsp
           (
                /* IN */ ATT_HANDLE  * handle,
                /* IN */ UINT16        num_handles,
                /* IN */ UCHAR         response
           );

/**
 *  \brief To disconnect an established ATT connection.
 *
 *  \par Description:
 *  This ATT interface initiates disconnection of the referred ATT connection.
 *  Disconnection of the ATT connection always succeeds - either by reception
 *  of the Disconnect Response message from the peer, or by timeout.
 *  In any case, ATT will confirm disconnection of the connection,
 *  by calling the registered ATT callback.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity which is to be disconnected.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_att_ecbfc_disconnect_req
           (
                /* IN */ ATT_HANDLE    * handle
           );

/**
 *  \brief Access the Enhanced ATT channel Informations
 *
 *  \par Description:
 *       This routine fetches the EATT channel parameters of the
 *       requested ATT connection instance.
 *
 *  \param [in] handle
 *         Identifies the ATT EATT entity whose Information is requested.
 *
 *  \param info
 *         Pointer to an ATT_ECBFC_INFO variable allocated by the caller to which
 *         the EATT Channel related informations are loaded if the requested
 *         ATT instance belongs to an EATT channel.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_att_ecbfc_get_info
           (
               /* IN */  ATT_HANDLE     * handle,
               /* OUT */ ATT_ECBFC_INFO * info
           );
#endif /* ATT_ON_ECBFC_SUPPORT */

/**
 * Note:
 * Though the following definition are MACROs, they are to be treated like APIs
 */

/**
 *  \brief API to send Error Response
 *
 *  \par Description:
 *  This routine sends Error Response to peer device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Error Response is to be
 *         sent
 *
 *  \param [in] param
 *         Identifies the op code for which error response is sent,
 *         handle resulting in error and the reason for error.
 *         See \ref ATT_ERROR_RSP_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_error_rsp(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_ERROR_RSP,\
            (void *)(param)\
        );


#ifdef ATT_CLIENT

#ifdef ATT_MTU_EXCHANGE_SUPPORT
/**
 *  \brief API to Exchange MTU
 *
 *  \par Description:
 *  This routine sends ATT Client's Rx MTU to the peer Server Device. This API
 *  shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which MTU Exchange is to be
 *         requested.
 *
 *  \param [in] param
 *         Indicates Client's RX MTU Size. See \ref ATT_XCHG_MTU_REQ_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_mtu_xcnhg_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_XCHNG_MTU_REQ,\
            (void *)(param)\
        );
#endif /* ATT_MTU_EXCHANGE_SUPPORT */

#ifdef ATT_FIND_INFO_SUPPORT
/**
 *  \brief API to Send Find Information Request to Server.
 *
 *  \par Description:
 *  This routine sends Find Information Request to the peer Server Device. This
 *  API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Find Information is to be
 *         requested.
 *
 *  \param [in] param
 *         Indicates Handle Range. See \ref ATT_FIND_INFO_REQ_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_find_info_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_FIND_INFO_REQ,\
            (void *)(param)\
        );
#endif /* ATT_FIND_INFO_SUPPORT */

#ifdef ATT_FIND_BY_TYPE_VALUE_SUPPORT
/**
 *  \brief API to Send Find By Type Value Request to Server.
 *
 *  \par Description:
 *  This routine sends Find By Type Value Request to the peer Server Device.
 *  This API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Find By Type Value is to
 *         be requested.
 *
 *  \param [in] param
 *         Indicates Handle Range. See \ref ATT_FIND_BY_TYPE_VAL_REQ_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_find_by_type_val_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_FIND_BY_TYPE_VAL_REQ,\
            (void *)(param)\
        );
#endif /* ATT_FIND_BY_TYPE_VALUE_SUPPORT */

#ifdef ATT_READ_BY_TYPE_SUPPORT
/**
 *  \brief API to Send Read By Type Request to Server.
 *
 *  \par Description:
 *  This routine sends Read By Type Request to the peer Server Device. This
 *  API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read By Type is to be
 *         requested.
 *
 *  \param [in] param
 *         Indicates Handle Range and UUID. See \ref ATT_READ_BY_TYPE_REQ_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_read_by_type_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_BY_TYPE_REQ,\
            (void *)(param)\
        );
#endif /* ATT_READ_BY_TYPE_SUPPORT */

#ifdef ATT_READ_REQUEST_SUPPORT
/**
 *  \brief API to Send Read Request to Server.
 *
 *  \par Description:
 *  This routine sends Read Request to the peer Server Device. This
 *  API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read is to be requested.
 *
 *  \param [in] param
 *         Indicates Handle to be read. See \ref ATT_READ_REQ_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_read_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_REQ,\
            (void *)(param)\
        );
#endif /* ATT_READ_REQUEST_SUPPORT */

#ifdef ATT_READ_BLOB_SUPPORT
/**
 *  \brief API to Send Read Blob Request to Server.
 *
 *  \par Description:
 *  This routine sends Read Blob Request to the peer Server Device. This
 *  API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read Blob is to be
 *         requested.
 *
 *  \param [in] param
 *         Indicates Handle and the offset from which value is to be read.
 *         See \ref ATT_READ_BLOB_REQ_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_read_blob_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_BLOB_REQ,\
            (void *)(param)\
        );
#endif /* ATT_READ_BLOB_SUPPORT */

#ifdef ATT_READ_MULTIPLE_SUPPORT
/**
 *  \brief API to Send Read Multiple Request to Server.
 *
 *  \par Description:
 *  This routine sends Read Multiple Request to the peer Server Device. This
 *  API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read Multiple is to be
 *         requested.
 *
 *  \param [in] param
 *         Indicates Handle List whose values are to be read.
 *         See \ref ATT_READ_MULTIPLE_REQ_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_read_multiple_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_MULTIPLE_REQ,\
            (void *)(param)\
        );
#endif /* ATT_READ_MULTIPLE_SUPPORT */

#ifdef ATT_READ_BY_GROUP_TYPE_SUPPORT
/**
 *  \brief API to Send Read By Group Type Request to Server.
 *
 *  \par Description:
 *  This routine sends Read By Group Type Request to the peer Server Device.
 *  This API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read By Group Type
 *         is to be requested.
 *
 *  \param [in] param
 *         Indicates Handle Range in which Read By Group Type is requested
 *         and UUID representing the Group Type.
 *         See \ref ATT_READ_BY_GROUP_TYPE_REQ_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_read_by_group_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_BY_GROUP_REQ,\
            (void *)(param)\
        );
#endif /* ATT_READ_BY_GROUP_TYPE_SUPPORT */

#ifdef ATT_WRITE_REQUEST_SUPPORT
/**
 *  \brief API to Send Write Request to Server.
 *
 *  \par Description:
 *  This routine sends Write Request to the peer Server Device. This
 *  API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Write is to be requested.
 *
 *  \param [in] param
 *         Indicates Handle and Value to be written.
 *         See \ref ATT_WRITE_REQ_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_write_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_WRITE_REQ,\
            (void *)(param)\
        );
#endif /* ATT_WRITE_REQUEST_SUPPORT */


#ifdef ATT_WRITE_COMMAND_SUPPORT
/**
 *  \brief API to Send Write Command to Server.
 *
 *  \par Description:
 *  This routine sends Write Command to the peer Server Device. This
 *  API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Write is to be requested.
 *
 *  \param [in] param
 *         Indicates Handle and Value to be written.
 *         See \ref ATT_WRITE_CMD_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_write_cmd(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_WRITE_CMD,\
            (void *)(param)\
        );
#endif /* ATT_WRITE_COMMAND_SUPPORT */

#ifdef ATT_SIGNED_WRITE_SUPPORT
/**
 *  \brief API to Send Signed Write Command to Server.
 *
 *  \par Description:
 *  This routine sends Signed Write Command to the peer Server Device. This
 *  API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Signed Write
 *         is to be requested.
 *
 *  \param [in] param
 *         Indicates Handle & Value to be written and Authentication Signature.
 *         See \ref ATT_WRITE_CMD_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_signed_write_cmd(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_SIGNED_WRITE_CMD,\
            (void *)(param)\
        );
#endif /* ATT_SIGNED_WRITE_SUPPORT */

#ifdef ATT_QUEUED_WRITE_SUPPORT
/**
 *  \brief API to Send Prepare Write Request to Server.
 *
 *  \par Description:
 *  This routine sends Prepare Write Request to the peer Server Device. This
 *  API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Write is to be requested.
 *
 *  \param [in] param
 *         Indicates Handle & Value to be written and offset at which the value
 *         is to be written. See \ref ATT_PREPARE_WRITE_REQ_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_prepare_write_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_PREPARE_WRITE_REQ,\
            (void *)(param)\
        );

/**
 *  \brief API to Send Execute Write Request to Server.
 *
 *  \par Description:
 *  This routine sends Execute Write Request to the peer Server Device. This
 *  API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Execute Write is to be
 *         requested.
 *
 *  \param [in] param
 *         Indicates whether Queued Write is to be executed or canceled.
 *         See \ref ATT_EXECUTE_WRITE_REQ_PARAM,
 *         \ref ATT_EXECUTE_WRITE_CANCEL_FLAG and
 *         \ref ATT_EXECUTE_WRITE_EXEC_FLAG.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_execute_write_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_EXECUTE_WRITE_REQ,\
            (void *)(param)\
        );

#endif /* ATT_QUEUED_WRITE_SUPPORT */

#ifdef ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT
/**
 *  \brief API to Send Read Multiple Variable Length Request to Server.
 *
 *  \par Description:
 *  This routine sends Read Multiple Variable Length Request to the peer Server
*   Device. This API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read Multiple Variable Length
 *         is to be requested.
 *
 *  \param [in] param
 *         Indicates Handle List whose values are to be read.
 *         See \ref ATT_READ_MULTIPLE_VARIABLE_LENGTH_REQ_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_read_multiple_variable_length_req(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_MULTIPLE_VARIABLE_LENGTH_REQ,\
            (void *)(param)\
        );
#endif /* ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT */

#ifdef ATT_HNDL_VAL_INDICATION_SUPPORT
/**
 *  \brief API to Send Handle Value Confirmation to Server.
 *
 *  \par Description:
 *  This routine sends Handle Value Confirmation to the peer Server Device. This
 *  API shall be used by ATT Client Applications only.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Handle Value Confirmation
 *         is to be requested.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_hndl_val_cnf(handle)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_HANDLE_VALUE_CNF,\
            NULL\
        )
#endif /* ATT_HNDL_VAL_INDICATION_SUPPORT */

#endif /* ATT_CLIENT */

#ifdef ATT_MTU_EXCHANGE_SUPPORT
/**
 *  \brief API to Respond to Client's Exchange MTU Request
 *
 *  \par Description:
 *  This routine sends ATT Server's Rx MTU to the peer Client Device.
 *  Note: MTU should be greater than or equal to \ref ATT_DEFAULT_MTU.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Exchange MTU Response
 *         is to be sent.
 *
 *  \param [in] param
 *         Indicates Server's RX MTU Size. See \ref ATT_XCHG_MTU_RSP_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_mtu_xcnhg_rsp(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_XCHNG_MTU_RSP,\
            (void *)(param)\
        )
#endif /* ATT_MTU_EXCHANGE_SUPPORT */

#ifdef ATT_FIND_INFO_SUPPORT
/**
 *  \brief API to Send Find Information Response to Client.
 *
 *  \par Description:
 *  This routine sends Find Information Response to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Find Information Response
 *         is to be sent.
 *
 *  \param [in] param
 *         Indicates Handle - UUID List and UUID Format.
 *         See \ref ATT_FIND_INFO_RSP_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_find_info_rsp(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_FIND_INFO_RSP,\
            (void *)(param)\
        )
#endif /* ATT_FIND_INFO_SUPPORT */

#ifdef ATT_FIND_BY_TYPE_VALUE_SUPPORT
/**
 *  \brief API to Send Find By Type Response to Client.
 *
 *  \par Description:
 *  This routine sends Find By Type Response to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Find By Type Value
 *         Response is to be sent.
 *
 *  \param [in] param
 *         Indicates Group Handle List where Type Value Pair are found.
 *         See \ref ATT_FIND_BY_TYPE_VAL_RSP_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_find_by_type_val_rsp(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_FIND_BY_TYPE_VAL_RSP,\
            (void *)(param)\
        )
#endif /* ATT_FIND_BY_TYPE_VALUE_SUPPORT */

#ifdef ATT_READ_BY_TYPE_SUPPORT
/**
 *  \brief API to Send Read By Type Response to Client.
 *
 *  \par Description:
 *  This routine sends Read By Type Response to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read By Type Response is
 *         to be sent.
 *
 *  \param [in] param
 *         Indicates Handle Value List where the UUID requested is found.
 *         See \ref ATT_READ_BY_TYPE_RSP_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_read_by_type_rsp(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_BY_TYPE_RSP,\
            (void *)(param)\
        )
#endif /* ATT_READ_BY_TYPE_SUPPORT */

#ifdef ATT_READ_REQUEST_SUPPORT
/**
 *  \brief API to Send Read Response to Client.
 *
 *  \par Description:
 *  This routine sends Read Response to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read Response
 *         is to be sent.
 *
 *  \param [in] param
 *         Indicates Value of the handle requested. See \ref ATT_READ_RSP_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_read_rsp(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_RSP,\
            (void *)(param)\
        )
#endif /* ATT_READ_REQUEST_SUPPORT */

#ifdef ATT_READ_BLOB_SUPPORT
/**
 *  \brief API to Send Read Blob Response to Client.
 *
 *  \par Description:
 *  This routine sends Read Blob Response to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read Blob Response is
 *         to be sent.
 *
 *  \param [in] param
 *         Indicates Value of the handle and offset requested.
 *         See \ref ATT_READ_BLOB_RSP_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_read_blob_rsp(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_BLOB_RSP,\
            (void *)(param)\
        )
#endif /* ATT_READ_BLOB_SUPPORT */


#ifdef ATT_READ_MULTIPLE_SUPPORT
/**
 *  \brief API to Send Read Multiple Response to Client.
 *
 *  \par Description:
 *  This routine sends Read Multiple Response to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read Multiple Response is
 *         to be sent.
 *
 *  \param [in] param
 *         Indicates Values of the list of handles requested.
 *         See \ref ATT_READ_MULTIPLE_RSP_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_read_multiple_rsp(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_MULTIPLE_RSP,\
            (void *)(param)\
        )
#endif /* ATT_READ_MULTIPLE_SUPPORT */

#ifdef ATT_READ_BY_GROUP_TYPE_SUPPORT
/**
 *  \brief API to Send Read By Group Type Response to Client.
 *
 *  \par Description:
 *  This routine sends Read By Group Type Response to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read By Group Type Response
 *         is to be sent.
 *
 *  \param [in] param
 *         Indicates Group Attribute Value and Range for each occurrence.
 *         See \ref ATT_READ_BY_GROUP_TYPE_RSP_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_read_by_group_rsp(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_BY_GROUP_RSP,\
            (void *)(param)\
        )
#endif /* ATT_READ_BY_GROUP_TYPE_SUPPORT */


#ifdef ATT_WRITE_REQUEST_SUPPORT
/**
 *  \brief API to Send Write Response to Client.
 *
 *  \par Description:
 *  This routine sends Write Response to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Write Response is
 *         to be sent.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_write_rsp(handle)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_WRITE_RSP,\
            NULL\
        );
#endif /* ATT_WRITE_REQUEST_SUPPORT */

#ifdef ATT_QUEUED_WRITE_SUPPORT
/**
 *  \brief API to Send Prepare Write Response to Client.
 *
 *  \par Description:
 *  This routine sends Prepare Write Response to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Response is to be sent.
 *
 *  \param [in] param
 *         Indicates Handle and Value to be written and offset at which
 *          the value is to be written. See \ref ATT_PREPARE_WRITE_RSP_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_prepare_write_rsp(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_PREPARE_WRITE_RSP,\
            (void *)(param)\
        );

/**
 *  \brief API to Send Execute Write Response to Client.
 *
 *  \par Description:
 *  This routine sends Execute Write Response to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Execute Write Response is
 *         to be sent.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_execute_write_rsp(handle)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_EXECUTE_WRITE_RSP,\
            NULL\
        );

#endif /* ATT_QUEUED_WRITE_SUPPORT */

#ifdef ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT
/**
 *  \brief API to Send Read Multiple Variable Length Response to Client.
 *
 *  \par Description:
 *  This routine sends Read Multiple Variable Length Response to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Read Multiple Variable Length
 *         Response is to be sent.
 *
 *  \param [in] param
 *         Indicates Values of the list of handles requested.
 *         See \ref ATT_READ_MULTIPLE_VARIABLE_LENGTH_RSP_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_read_multiple_variable_length_rsp(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_READ_MULTIPLE_VARIABLE_LENGTH_RSP,\
            (void *)(param)\
        )
#endif /* ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT */

#ifdef ATT_HNDL_VAL_NOTIFICATION_SUPPORT
/**
 *  \brief API to Send Notification to Client.
 *
 *  \par Description:
 *  This routine sends Notification to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Notification
 *         is to be sent.
 *
 *  \param [in] param
 *         Indicates Handle and Value to be notified.
 *         See \ref ATT_HNDL_VAL_NTF_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_hndl_val_ntf(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_HANDLE_VALUE_NTF,\
            (void *)(param)\
        );
#endif /* ATT_HNDL_VAL_NOTIFICATION_SUPPORT */

#ifdef ATT_HNDL_VAL_INDICATION_SUPPORT
/**
 *  \brief API to Send Indication to Client.
 *
 *  \par Description:
 *  This routine sends Indication to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Indication is to be sent.
 *
 *  \param [in] param
 *         Indicates Handle and Value to be indicated.
 *         See \ref ATT_HNDL_VAL_IND_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_hndl_val_ind(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_HANDLE_VALUE_IND,\
            (void *)(param)\
        );
#endif /* ATT_HNDL_VAL_INDICATION_SUPPORT */

#ifdef ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT
/**
 *  \brief API to Send Handle Value Multiple Notification to Client.
 *
 *  \par Description:
 *  This routine sends Handle Value Multiple Notification to the peer Client Device.
 *
 *  \param [in] handle
 *         Identifies the remote ATT entity to which Handle Value Multiple
 *         Notification is to be sent.
 *
 *  \param [in] param
 *         Indicates Handle, Length and Value Tuple List to be notified.
 *         See \ref ATT_HNDL_VAL_MULTIPLE_NTF_PARAM.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure.
 */
#define BT_att_send_hndl_val_multiple_ntf(handle,param)\
        BT_att_send_pdu\
        (\
            (handle),\
            ATT_HANDLE_VALUE_MULTIPLE_NTF,\
            (void *)(param)\
        );
#endif /* ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT */

/**
 *  \brief Access the last configured MTU
 *
 *  \par Description:
 *       This routine access the lastly configured MTU based on the ATT
 *       connection instance
 *
 *  \param [in] handle
 *         Identifies the ATT entity for which the MTU needs to be accessed.
 *
 *  \param mtu
 *         Pointer to an UINT16 variable allocated by the caller to which the
 *         last configured MTU value is returned.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_att_access_mtu
           (
                /* IN */  ATT_HANDLE    * handle,
                /* OUT */ UINT16        * mtu
           );

#ifdef __cplusplus
};
#endif

/** \} */

/** \} */
/** \} */

#endif /* _H_BT_ATT_API_ */

