
/**
 *  \file BT_obex_api.h
 *
 *  This Header File contains declaration of OBEX APIs, Macros and
 *  Constant Definitions for use by applications/profiles.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_OBEX_API_
#define _H_BT_OBEX_API_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ----------------------------------------- Global Definitions */
/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup obex_module OBEX (Object Exchange Protocol)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  obex module to the Application
 *  and other upper layers of the stack.
 */
/**
 * \defgroup obex_defines Defines
 * \{
 * Describes defines for the module.
 */
/**
 * \defgroup obex_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/**
 *  OBEX Roles
 *  Constant Definitions for OBEX Roles.
 */
#define OBEX_SERVER_ROLE                    1U
#define OBEX_CLIENT_ROLE                    2U

/**
 *  OBEX Minimum Packet Size
 *  Constant Definitions for Minimum OBEX Packet Length.
 */
#define OBEX_MIN_PACKET_LENGTH              255U

/**
 *  OBEX Asynchronous Events
 *  Constant Definitions for OBEX Asynchronous Events.
 */

/* OBEX Client/Server Common Events */
#define OBEX_EVENT_OP_ERROR                 0xFFU
#define OBEX_EVENT_RFCOMM_ACCEPT_CANCEL     0x02U

#ifdef OBEX_SUPPORT_RELIABLE_SSN
#define OBEX_EVENT_SSN_SUSPEND_IND          0x03U
#define OBEX_EVENT_SSN_RESUME_IND           0x04U
#endif /* OBEX_SUPPORT_RELIABLE_SSN */

/**
 * OBEX Client Events
 */
#define OBEX_EVENT_TRANS_CONNECT_CFM        0x05U
#define OBEX_EVENT_TRANS_DISCONNECT_CFM     0x06U

#define OBEX_EVENT_OBEX_CONNECT_CFM         0x07U
#define OBEX_EVENT_OBEX_DISCONNECT_CFM      0x08U
#define OBEX_EVENT_GET_CFM                  0x09U
#define OBEX_EVENT_GET_FINAL_CONTINUE_CFM   0x0AU
#define OBEX_EVENT_GET_FINAL_SUCCESS_CFM    0x0BU
#define OBEX_EVENT_PUT_CFM                  0x0CU
#define OBEX_EVENT_PUT_FINAL_CFM            0x0DU
#define OBEX_EVENT_SETPATH_CFM              0x0EU
#define OBEX_EVENT_GET_ABORT_CFM            0x0FU
#define OBEX_EVENT_PUT_ABORT_CFM            0x10U

#ifdef OBEX_ENH_SUPPORT
#define OBEX_EVENT_ACTION_CFM               0x11U
#define OBEX_EVENT_ACTION_FINAL_CFM         0x12U
#endif /* OBEX_ENH_SUPPORT */

#ifdef OBEX_SUPPORT_RELIABLE_SSN
#define OBEX_EVENT_SSN_SUSPEND_CFM          0x13U
#define OBEX_EVENT_SSN_RESUME_CFM           0x14U
#endif /* OBEX_SUPPORT_RELIABLE_SSN */

/**
 * OBEX Server Events
 */
#define OBEX_EVENT_TRANS_CONNECT_IND        0x15U
#define OBEX_EVENT_TRANS_DISCONNECT_IND     0x16U

#define OBEX_EVENT_OBEX_CONNECT_IND         0x17U
#define OBEX_EVENT_OBEX_DISCONNECT_IND      0x18U
#define OBEX_EVENT_GET_IND                  0x19U
#define OBEX_EVENT_GET_FINAL_IND            0x1AU
#define OBEX_EVENT_PUT_IND                  0x1BU
#define OBEX_EVENT_PUT_FINAL_IND            0x1CU
#define OBEX_EVENT_SETPATH_IND              0x1DU
#define OBEX_EVENT_GET_ABORT_IND            0x1EU
#define OBEX_EVENT_PUT_ABORT_IND            0x1FU

#ifdef OBEX_ENH_SUPPORT
#define OBEX_EVENT_ACTION_IND               0x20U
#define OBEX_EVENT_ACTION_FINAL_IND         0x21U
#endif /* OBEX_ENH_SUPPORT */

/**
 *  Header identifiers for OBEX objects
 */
#define OBEX_HDR_COUNT                      0xC0U
#define OBEX_HDR_NAME                       0x01U
#define OBEX_HDR_TYPE                       0x42U
#define OBEX_HDR_LENGTH                     0xC3U
#define OBEX_HDR_TIME_ISO                   0x44U
#define OBEX_HDR_TIME_4BYTE                 0xC4U
#define OBEX_HDR_DESCRIPTION                0x05U
#define OBEX_HDR_TARGET                     0x46U
#define OBEX_HDR_HTTP                       0x47U
#define OBEX_HDR_BODY                       0x48U
#define OBEX_HDR_END_OF_BODY                0x49U
#define OBEX_HDR_WHO                        0x4AU
#define OBEX_HDR_CONNECTION_ID              0xCBU
#define OBEX_HDR_APP_PARAM                  0x4CU
#define OBEX_HDR_AUTH_CHALLENGE             0x4DU
#define OBEX_HDR_AUTH_RESPONSE              0x4EU
#define OBEX_HDR_OBJECT_CLASS               0x51U

#ifdef OBEX_SUPPORT_RELIABLE_SSN
#define OBEX_HDR_SSN_PARAMS                 0x52U
#define OBEX_HDR_SSN_SEQN                   0x93U
#endif /* OBEX_SUPPORT_RELIABLE_SSN */

#ifdef OBEX_ENH_SUPPORT
#define OBEX_HDR_ACTION_ID                  0x94U
#define OBEX_HDR_DESTNAME                   0x15U
#define OBEX_HDR_PERMISSIONS                0xD6U
#endif /* OBEX_ENH_SUPPORT */

#ifdef OBEX_OVER_L2CAP
#define OBEX_HDR_SRM                        0x97U
#define OBEX_HDR_SRMP                       0x98U
#endif /* OBEX_OVER_L2CAP */

/**
 * OBEX Action Operation Identifiers
 */
#ifdef OBEX_ENH_SUPPORT
#define OBEX_ACTION_COPY_ID                 0x00U
#define OBEX_ACTION_MOVE_ID                 0x01U
#define OBEX_ACTION_SET_OBJ_PER_ID          0x02U
#endif /* OBEX_ENH_SUPPORT */

/**
 * Tag and Length Ids used in TLV scheme
 */
#define OBEX_TAG_NONCE_4DIG_CHALLENGE       0x00U
#define OBEX_TAG_DIGEST_4RESPONSE           OBEX_TAG_NONCE_4DIG_CHALLENGE

#define OBEX_TAG_LENGTH_NONCE               0x10U
#define OBEX_TAG_LENGTH_REQ_DIGEST          OBEX_TAG_LENGTH_NONCE

/**
 *  OBEX Operation Codes
 */
#define OBEX_CONNECT_OP                     0x80U
#define OBEX_DISCONNECT_OP                  0x81U
#define OBEX_PUT_OP                         0x02U
#define OBEX_PUT_FINAL_OP                   0x82U
#define OBEX_GET_OP                         0x03U
#define OBEX_GET_FINAL_OP                   0x83U
#define OBEX_SETPATH_OP                     0x85U

#ifdef OBEX_ENH_SUPPORT
#define OBEX_ACTION_OP                      0x06U
#define OBEX_ACTION_FINAL_OP                0x86U
#endif /* OBEX_ENH_SUPPORT */

#ifdef OBEX_SUPPORT_RELIABLE_SSN
#define OBEX_SESSION_OP                     0x87U
#endif /* OBEX_SUPPORT_RELIABLE_SSN */

#define OBEX_ABORT_OP                       0xFFU

/**
 *  OBEX Response Codes
 */
#define OBEX_CONTINUE_RSP                   0x90U /**< Continue response code */
#define OBEX_SUCCESS_RSP                    0xA0U /**< OK, Success */
#define OBEX_CREATED_RSP                    0xA1U /**< Created */
#define OBEX_ACCEPTED_RSP                   0xA2U /**< Accepted */
#define OBEX_NON_AUTH_RSP                   0xA3U /**< Non Authoritative Info */
#define OBEX_NO_CONTENT_RSP                 0xA4U /**< No content */
#define OBEX_RESET_CONTENT_RSP              0xA5U /**< Reset Content */
#define OBEX_PART_CONTENT_RSP               0xA6U /**< Partial Content */
#define OBEX_MULTI_CHOICE_RSP               0xB0U /**< Multiple Choices */
#define OBEX_MOVED_PERM_RSP                 0xB1U /**< Moved Permanently */
#define OBEX_MOVED_TEMP_RSP                 0xB2U /**< Moved Temporarily */
#define OBEX_SEE_OTHER_RSP                  0xB3U /**< See Other */
#define OBEX_NOT_MODIFIED_RSP               0xB4U /**< Not Modified */
#define OBEX_USE_PROXY_RSP                  0xB5U /**< Use Proxy */
#define OBEX_REQ_IS_BAD_RSP                 0xC0U /**< Bad Req server can't */
#define OBEX_UNAUTHORIZED_RSP               0xC1U /**< Unauthorized */
#define OBEX_PAYMENT_REQUIRED_RSP           0xC2U /**< Payment Required */
#define OBEX_FORBIDDEN_RSP                  0xC3U /**< op understood but refused */
#define OBEX_NOT_FOUND_RSP                  0xC4U /**< Not Found */
#define OBEX_METHOD_NA_RSP                  0xC5U /**< Method Not Allowed */
#define OBEX_NOT_ACCEPTABLE_RSP             0xC6U /**< Not Acceptable */
#define OBEX_PROXY_AUTH_REQD_RSP            0xC7U /**< Proxy Authentication Required */
#define OBEX_TIMEOUT_RSP                    0xC8U /**< Request Time out */
#define OBEX_CONFLICT_RSP                   0xC9U /**< Conflict */
#define OBEX_GONE_RSP                       0xCAU /**< Gone */
#define OBEX_LENGTH_REQUIRED_RSP            0xCBU /**< Length Required */
#define OBEX_PRECOND_FAIL_RSP               0xCCU /**< Precondition Failed */
#define OBEX_REQ_ENTITY_LARGE_RSP           0xCDU /**< Requested Entity too large */
#define OBEX_REQ_URL_LARGE_RSP              0xCEU /**< Requested URL too large */
#define OBEX_UNSUPP_MEDIA_RSP               0xCFU /**< Unsupported Media type */
#define OBEX_SERVER_ERR_RSP                 0xD0U /**< Internal Server error */
#define OBEX_NOT_IMP_RSP                    0xD1U /**< Not implemented */
#define OBEX_BAD_GATEWAY_RSP                0xD2U /**< Bad Gateway */
#define OBEX_NO_SERVICE_RSP                 0xD3U /**< Service Unavailable */
#define OBEX_GATEWAY_TIMEOUT_RSP            0xD4U /**< Gateway Timeout */
#define OBEX_HTTP_VER_RSP                   0xD5U /**< HTTP version not supported */
#define OBEX_DATABASE_FULL_RSP              0xE0U /**< Database Full */
#define OBEX_DATABASE_LOCKED_RSP            0xE1U /**< Database Locked */


#define OBEX_SSN_CREATE                     0x00U
#define OBEX_SSN_CLOSE                      0x01U
#define OBEX_SSN_SUSPEND                    0x02U
#define OBEX_SSN_RESUME                     0x03U
#define OBEX_SSN_SETTIMEOUT                 0x04U
#define OBEX_SSN_INVALID                    0xFFU

#define OBEX_SSN_TAG_DEVICEADDR             0x00U
#define OBEX_SSN_TAG_NONCE                  0x01U
#define OBEX_SSN_TAG_SESSIONID              0x02U
#define OBEX_SSN_TAG_NEXTSEQN               0x03U
#define OBEX_SSN_TAG_TIMEOUT                0x04U
#define OBEX_SSN_TAG_OPCODE                 0x05U

#define OBEX_SSN_NON_RELIABLE               0x00U
#define OBEX_SSN_PREF_RELIABLE              0x01U
#define OBEX_SSN_STRICT_RELIABLE            0x02U

#define OBEX_RELIABLE_SSN_ID_LEN            16U
#define OBEX_RELIABLE_SSN_NONCE_MAXLEN      16U
#define OBEX_SSN_TIMEOUT_DEFAULT            0xFFFFFFFFU

/**
 * Flags used for OBEX SRM enable/disable
 */
#define OBEX_SRM_ENABLE                     0x01U
#define OBEX_SRM_DISABLE                    0x00U

/**
 * Flags used for OBEX SRMP Wait
 */
#define OBEX_SRMP_WAIT                      0x01U
#define OBEX_WAIT_MASK                      0x08U
/** \} */
/** \} */

/* ----------------------------------------- Structures/Data Types */
/**
 *  \addtogroup obex_defines Defines
 *  \{
 */

/**
 * \defgroup obex_structures Structures
 * \{
 * Describes Structures defined by the module.
 */

/**
 *  \typedef OBEX_HANDLE
 *
 *  \par Description: This is used to refer the OBEX
 *  Client/Server instance.
 */
typedef UINT8 OBEX_HANDLE;

/**
 *  \typedef OBEX_OBJ_HEADER
 *
 *  \par Description: This structure is used by application to store
 *  OBEX header information that needs to be passed to OBEX requests/responses.
 */
typedef struct
{
    /**
     * Pointer to the header value.
     * If the header value is 4 byte/1 byte value assign the address to hv
     */
    union
    {
        UCHAR * value;
        UINT32 four_byte;
        UCHAR byte;
    }hv;

    /**
     * Size of header value.
     * For example, if the header value is of type UINT32 size is equal to 4.
     */
    UINT16 size;

    /**
     * OBEX header identifier
     */
    UCHAR hi;

}OBEX_OBJ_HEADER;
/** \} */
/** \} */

/**
 *  \defgroup obex_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */

/**
 *  \typedef OBEX_NOTIFY_CB
 *
 *  \brief   OBEX Event Notification Callback
 *
 *  This callback is invoked by OBEX to report Request Indications,
 *  Response Confirms or any other Asynchrosnous event to the
 *  application/profile.
 *
 *  \li    To confirm the result of an operation initiated by the upper
 *         layer application/profile.
 *
 *  \li    To inform upper layer application/profile of asynchronous
 *         events.
 *
 *  \param [in] obex_handle
 *         This parameter is the OBEX handle for which the
 *         notification is reported.
 *
 *  \param [in] event_result
 *         If the Event being reported is the result of an earlier trigger
 *         by the application (using an OBEX API), this parameter holds the
 *         result of the procedure. On successful completion of the procedure
 *         this parameter will have a value of 0x0000 (API_SUCCESS), else
 *         it will contain an Error Code describing the reason for failure.
 *         For asynchronous event indications, this parameter holds the
 *         possible reason due to which the event is being reported.
 *
 *  \param [in] event_type
 *         This parameter describes the OBEX Event being reported.
 *         Refer to the OBEX Asynchronous Events section above.
 *
 *  \param [in] event_data
 *         Depending upon the event_type, this parameters holds data
 *         to be provided to the OBEX application/profile.
 *
 *  \param [in] event_datalen
 *         This parameter specifies the size of event_data.
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the reason for failure (refer BT_error.h)
 *
 */
typedef API_RESULT (* OBEX_NOTIFY_CB)
                   (
                       /* IN */ OBEX_HANDLE * obex_handle,
                       /* IN */ UINT16 event_result,
                       /* IN */ UCHAR event_type,
                       /* IN */ void * event_data,
                       /* IN */ UINT16 event_datalen
                   );
/** \} */

/**
 *  \addtogroup obex_defines Defines
 *  \{
 */

/**
 *  \addtogroup obex_structures Structures
 *  \{
 */
/**
 *  \typedef OBEX_INIT_PARAMS
 *
 *  \par Description: This structure is used to initialize variables
 *  in the OBEX Client/Server instance.
 */
typedef struct _OBEX_INIT_PARAMS
{
    /**
     * Bluetooth device address
     */
    UCHAR       bd_addr [BT_BD_ADDR_SIZE];

#ifdef RFCOMM
    /**
     * RFCOMM channel number
     */
    UCHAR       channel_num;
#endif /* RFCOMM */

    /**
     * Max receive size (~MTU)
     */
    UINT16      max_rx_size;

    /**
     * Upper layer callback function
     */
    OBEX_NOTIFY_CB obex_notify_cb;

#ifdef OBEX_OVER_L2CAP
    /**
     * Local L2CAP PSM to use for connection
     * Used by both Client and Server
     */
    UINT16 l2cap_lpsm;

    /**
     * Remote L2CAP PSM to connect to
     * Used by Client
     */
    UINT16 l2cap_rpsm;
#endif /* OBEX_OVER_L2CAP */

#ifdef OBEX_SUPPORT_RELIABLE_SSN
    /**
     * Transport Quality of Service
     * 0x00 - Non Reliable
     * 0x01 - Preferred Reliable
     * 0x02 - Strictly Reliable
     */
    UCHAR qos;
#endif /* OBEX_SUPPORT_RELIABLE_SSN */

} OBEX_INIT_PARAMS;
/** \} */
/** \} */

/* ----------------------------------------- Function Declarations */
/**
 * \defgroup obex_api API Definitions
 * \{
 * Describes API definitions defined by the module.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ========== OBEX Client related APIs ==== */
#ifdef OBEX_CLIENT
/**
 *  \brief To initialize an OBEX client instance.
 *
 *  \par Description:
 *      This API is used to initialize an OBEX client instance.
 *      It initializes the client instance variables with the peer's
 *      Bluetooth Device address, the RFCOMM channel number and L2CAP psm on
 *      which it will listen for connection and the callback function to be
 *      called on receiving an event. This API provides a handle as the output
 *      parameter and this handle shall be used in future to refer a particular
 *      instance of a client.
 *
 *  \param [out] obex_handle
 *      Handle to refer the newly created OBEX client instance.
 *
 *  \param [in] init_params
 *      Parameters that will be used for initializing the OBEX client.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *     - For OBEX transport connection over RFCOMM, set L2CAP psm parameter to zero.
 *       In similar way for OBEX transport connection over L2CAP, set rfcomm server
 *       channel parameter to zero.
 */
#define BT_obex_register_client(obex_handle, init_params) \
    BT_obex_register((obex_handle), (init_params), OBEX_CLIENT_ROLE)

/**
 *  \brief To uninitialize an OBEX client instance.
 *
 *  \par Description:
 *      This API is used to put the OBEX client instance in an inactive state
 *      and it members are set to default initial values.
 *
 *  \param [in] handle
 *      Handle to refer the OBEX client instance to be unregistered
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_unregister_client(handle) BT_obex_unregister ((handle))

/**
 *  \brief To establish an OBEX transport layer connection.
 *
 *  \par Description:
 *      This API is used by the OBEX client to establish a transport layer
 *      connection before doing an OBEX operation.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX client instance.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      Transport layer can be either RFCOMM or L2CAP layer.
 */
API_RESULT BT_obex_transport_connect
           (
               /* IN */ OBEX_HANDLE * obex_handle
           );

/**
 *  \brief To establish connection to an OBEX server
 *
 *  \par Description:
 *      This API is used to establish an OBEX connection from an OBEX client to
 *      an OBEX server, on a specific RFCOMM channel or L2CAP psm. It sends the
 *      required headers using the obj_hdr parameter.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX client instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the connect request.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      - Before calling thie API, make sure the OBEX transport connection
 *        is established using BT_obex_transport_connect () API.
 */
#define BT_obex_connect_req(obex_handle, obj_hdr, num_of_hdrs) \
        BT_obex_reqrsp ((obex_handle), OBEX_CONNECT_OP, (obj_hdr), (num_of_hdrs), 0x00U, 0x00U)

/**
 *  \brief To terminate the OBEX connection with remote OBEX server.
 *
 *  \par Description:
 *      The API is used by OBEX client to close an existing OBEX connection with
 *      the remote OBEX server.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX client instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the disconnect request.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_disconnect_req(obex_handle, obj_hdr, num_of_hdrs) \
        BT_obex_reqrsp ((obex_handle), OBEX_DISCONNECT_OP, (obj_hdr), (num_of_hdrs), 0x00U, 0x00U)

/**
 *  \brief To retrieve an object from an OBEX server.
 *
 *  \par Description:
 *  The client uses this API to retrieve the specified object
 *  from an OBEX server.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX client instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the get request.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \param [in] flags
 *      Specifies if the request is a final request or not.
 *         - (flags != 0) correspond to a final request.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_get_req(obex_handle, obj_hdr, num_of_hdrs, flags) \
        BT_obex_reqrsp ((obex_handle), ((flags)? OBEX_GET_FINAL_OP: OBEX_GET_OP), \
                        (obj_hdr), (num_of_hdrs), 0x00U, (flags))

/**
 *  \brief To transfer an object to an OBEX server.
 *
 *  \par Description:
 *      The client uses this API to transfer the specified object to an
 *      OBEX server. It uses the obj_hdr parameter to specify the required
 *      headers.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX client instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the put request.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \param [in] flags
 *      Specifies if the request is a final request or not.
 *         - (flags != 0) correspond to a final request.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_put_req(obex_handle, obj_hdr, num_of_hdrs, flags) \
        BT_obex_reqrsp ((obex_handle), ((flags)? OBEX_PUT_FINAL_OP: OBEX_PUT_OP), \
                        (obj_hdr), (num_of_hdrs), 0x00U, (flags))

/**
 *  \brief To set the current folder in an OBEX server.
 *
 *  \par Description:
 *      The client uses this API to set the current folder in an OBEX server.
 *      It uses the obj_hdr parameter to specify the required headers.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX client instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the setpath request.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \param [in] flags
 *      Specifies whether to backup a level or return without
 *      creating a folder if it does not exist
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_setpath_req(obex_handle, obj_hdr, num_of_hdrs, flags) \
        BT_obex_reqrsp ((obex_handle), OBEX_SETPATH_OP, (obj_hdr), (num_of_hdrs), 0x00U, (flags))

/**
 *  \brief To terminate an ongoing OBEX operation.
 *
 *  \par Description:
 *      The client uses this API to terminate an ongoing multi-packet
 *      OBEX operation.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX client instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the abort request.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_abort_req(obex_handle, obj_hdr, num_of_hdrs) \
        BT_obex_reqrsp ((obex_handle), OBEX_ABORT_OP, (obj_hdr), (num_of_hdrs), 0x00U, 0x00U)

#ifdef OBEX_ENH_SUPPORT
/**
 *  \brief To perform an action on the object present in the OBEX server.
 *
 *  \par Description:
 *       The client uses this API to perform an action (Example:copy, move/rename,
 *       and set permissions) on the objects present in the OBEX server.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX client instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the action request.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \param [in] flags
 *      Specifies if the request is a final request or not.
 *         - (flags != 0) correspond to a final request.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      For using this API, enable OBEX_ENH_SUPPORT feature flag
 *      in BT_feature.h.
 */
#define BT_obex_action_req(obex_handle, obj_hdr, num_of_hdrs, flags) \
        BT_obex_reqrsp ((obex_handle), ((flags)? OBEX_ACTION_FINAL_OP: OBEX_ACTION_OP), \
                        (obj_hdr), (num_of_hdrs), 0x00U, (flags))
#endif /* OBEX_ENH_SUPPORT */

/**
 *  \brief To suspend the OBEX reliable session.
 *
 *  \par Description:
 *       This API enables to suspend the OBEX reliable session that was already created.
 *
 *  \param [in] obex_handle
 *      Handle to refer to the OBEX Client instance
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_ssn_suspend_req(obex_handle) \
        BT_obex_ssn_reqrsp ((obex_handle), OBEX_SSN_SUSPEND, OBEX_SSN_TIMEOUT_DEFAULT, NULL, 0U, 0x00U)

/**
 *  \brief To resume the OBEX reliable session.
 *
 *  \par Description:
 *       This API enables to resme the OBEX reliable session that was already suspended.
 *
 *  \param [in] obex_handle
 *      Handle to refer to the OBEX Client instance
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_ssn_resume_req(obex_handle, obj_hdr, num_of_hdrs) \
        BT_obex_ssn_reqrsp ((obex_handle), OBEX_SSN_RESUME, OBEX_SSN_TIMEOUT_DEFAULT, obj_hdr, num_of_hdrs, 0x00U)
#endif /* OBEX_CLIENT */

/* =============== OBEX SERVER related APIs ========= */
#ifdef OBEX_SERVER
/**
 *  \brief To Initialize an OBEX Server instance.
 *
 *  \par Description:
 *      This API is used to initialize an OBEX Server instance with the RFCOMM
 *      channel and L2CAP psm specified (OBEX server listens on this channel
 *      or L2CAP psm) and registers the callback functions to be called on receiving
 *      an event. This API provides a handle as the output parameter and this handle shall
 *      be used in future to refer to a particular instance of a OBEX server.
 *
 *  \param [out] obex_handle
 *      Handle to refer the newly created OBEX server instance
 *
 *  \param [in] init_params
 *      Parameters that will be used for initializing the server
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *    - The decision of requiring a OBEX target header can be left to the layer above and
        the request received can simply be passed to the  upper layer.

 *    - After registration, the OBEX Server can accept only one connection on a given
 *      RFCOMM channel or L2CAP psm
 */
#define BT_obex_register_server(obex_handle, init_params) \
        BT_obex_register((obex_handle), (init_params), OBEX_SERVER_ROLE)

/**
 *  \brief To uninitialize an OBEX server instance.
 *
 *  \par Description:
 *      This API is used to put an OBEX OBEX_ENH_SUPPORTserver instance into
 *      a non-connectable state
 *
 *  \param [in] obex_handle
 *      Handle to refer to the OBEX server instance
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_unregister_server(handle) BT_obex_unregister ((handle))

/**
 *  \brief To respond to an OBEX connect request
 *
 *  \par Description:
 *      The server uses this API to respond to an OBEX connect request.
 *      It sends the required headers in the obj_hdr parameter and the
 *      response code in rsp_code.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX server instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the connect response.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \param [in] rsp_code
 *      Response code being sent by the server.
 *      For response codes refer to the section "OBEX Response codes" above.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_connect_rsp(obex_handle, obj_hdr, num_of_hdrs, rsp_code)  \
        BT_obex_reqrsp ((obex_handle), OBEX_CONNECT_OP, (obj_hdr), (num_of_hdrs), (rsp_code), 0x00U)

/**
 *  \brief To respond to an OBEX disconnect request.
 *
 *  \par Description:
 *      The server uses this API to respond to an OBEX disconnect request.
 *      It sends the required headers in the obj_hdr parameter and the
 *      response code in rsp_code.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX server instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the disconnect response.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \param [in] rsp_code
 *      Response code being sent by the server.
 *      For response codes refer to the section "OBEX Response codes" above.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_disconnect_rsp(obex_handle, obj_hdr, num_of_hdrs, rsp_code)  \
        BT_obex_reqrsp ((obex_handle), OBEX_DISCONNECT_OP, (obj_hdr), (num_of_hdrs), (rsp_code), 0x00U)

/**
 *  \brief To respond to an OBEX get request.
 *
 *  \par Description:
 *      The server uses this API to respond to an OBEX get request.
 *      It sends the required headers in the obj_hdr parameter and the
 *      response code in rsp_code.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX server instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the get response.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \param [in] rsp_code
 *      Response code being sent by the server.
 *      For response codes refer to the section "OBEX Response codes" above.
 *
 *  \param [in] flags
 *      Specifies if the request is a final response or not.
 *         - (flags != 0) correspond to a final response.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_get_rsp(obex_handle, obj_hdr, num_of_hdrs, rsp_code, flags) \
        BT_obex_reqrsp ((obex_handle), ((flags)? OBEX_GET_FINAL_OP: OBEX_GET_OP), \
                        (obj_hdr), (num_of_hdrs), (rsp_code), (flags))

/**
 *  \brief To respond to an OBEX put request.
 *
 *  \par Description:
 *      The server uses this API to respond to an OBEX put request.
 *      It sends the required headers in the obj_hdr parameter and the
 *      response code in rsp_code.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX server instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the put response.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \param [in] rsp_code
 *      Response code being sent by the server.
 *      For response codes refer to the section "OBEX Response codes" above.
 *
 *  \param [in] flags
 *      Specifies if the request is a final response or not.
 *         - (flags != 0) correspond to a final response.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_put_rsp(obex_handle, obj_hdr, num_of_hdrs, rsp_code, flags) \
        BT_obex_reqrsp ((obex_handle), ((flags)? OBEX_PUT_FINAL_OP: OBEX_PUT_OP), \
                        (obj_hdr), (num_of_hdrs), (rsp_code), (flags))

/**
 *  \brief To respond to an OBEX setpath request.
 *
 *  \par Description:
 *      The server uses this API to respond to an OBEX setpath request.
 *      It sends the required headers in the obj_hdr parameter and the
 *      response code in rsp_code.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX server instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the setpath response.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \param [in] rsp_code
 *      Response code being sent by the server.
 *      For response codes refer to the section "OBEX Response codes" above.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_setpath_rsp(obex_handle, obj_hdr, num_of_hdrs, rsp_code)  \
        BT_obex_reqrsp ((obex_handle), OBEX_SETPATH_OP, (obj_hdr), (num_of_hdrs), (rsp_code), 0x00U)

/**
 *  \brief To respond to an OBEX abort request.
 *
 *  \par Description:
 *      The server uses this API to respond to an OBEX abort request.
 *      It sends the required headers in the obj_hdr parameter and the
 *      response code in rsp_code.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX server instance.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the abort response.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \param [in] rsp_code
 *      Response code being sent by the server.
 *      For response codes refer to the section "OBEX Response codes" above.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_abort_rsp(obex_handle, obj_hdr, num_of_hdrs, rsp_code) \
        BT_obex_reqrsp ((obex_handle), OBEX_ABORT_OP, (obj_hdr), (num_of_hdrs), (rsp_code), 0x00U)

#ifdef OBEX_ENH_SUPPORT
/**
 *  \brief To respond to an OBEX action request.
 *
 *  \par Description:
 *      The server uses this API to respond to an OBEX action request.
 *      It sends the required headers in the obj_hdr parameter and the
 *      response code in rsp_code.
 *
 *  \param [in] obex_handle
 *      Handle to refer to the OBEX Server instance
 *
 *  \param [in] obj_hdr
 *      List of headers to be transmitted in the abort response
 *
 *  \param [in] num_of_hdrs
 *      Number of headers being transmitted
 *
 *  \param [in] rsp_code
 *      Response code being sent by the server.
 *      For response codes refer to the section "OBEX Response codes" above.
 *
 *  \param [in] flags
 *      Specifies if the request is a final response or not.
 *         - (flags != 0) correspond to a final response.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_action_rsp(obex_handle, obj_hdr, num_of_hdrs, rsp_code, flags) \
        BT_obex_reqrsp ((obex_handle), ((flags)? OBEX_ACTION_FINAL_OP: OBEX_ACTION_OP), \
                        (obj_hdr), (num_of_hdrs), (rsp_code), (flags))

#endif /*  OBEX_ENH_SUPPORT */

/**
 *  \brief To response to OBEX reliable session suspend request.
 *
 *  \par Description:
 *       This API enable to send response to OBEX reliable session suspend request.
 *
 *  \param [in] obex_handle
 *      Handle to refer to the OBEX instance
 *
 *  \param [in] rsp_code
 *      Response code to be sent
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_ssn_suspend_rsp(obex_handle, rsp_code) \
        BT_obex_ssn_reqrsp ((obex_handle), OBEX_SSN_SUSPEND, OBEX_SSN_TIMEOUT_DEFAULT, NULL, 0U, (rsp_code))

/**
 *  \brief To response to OBEX reliable session resume request.
 *
 *  \par Description:
 *       This API enable to send response to OBEX reliable session resume request.
 *
 *  \param [in] obex_handle
 *      Handle to refer to the OBEX instance
 *
 *  \param [in] rsp_code
 *      Response code to be sent
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the response.
 *
 *  \param [in] num_of_hdrs
 *      Number of OBEX headers being transmitted.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_ssn_resume_rsp(obex_handle, rsp_code, obj_hdr, num_of_hdrs) \
        BT_obex_ssn_reqrsp ((obex_handle), OBEX_SSN_RESUME, OBEX_SSN_TIMEOUT_DEFAULT, obj_hdr, num_of_hdrs, (rsp_code))
#endif /* OBEX_SERVER */

/* =========== APIs Common for OBEX Server & OBEX Client ===============*/
/**
 *  \brief To terminate an OBEX transport layer connection.
 *
 *  \par Description:
 *      This API is used to terminate an OBEX transport layer connection. It will
 *      put both the OBEX Server and Client into the IDLE state.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX client/server instance to be disconnected.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      Transport layer can be either RFCOMM or L2CAP layer.
 */
API_RESULT BT_obex_transport_disconnect
           (
               /* IN */ OBEX_HANDLE * obex_handle
           );

/**
 *
 *  \brief To initialize an obex entity.
 *
 *  \par Description:
 *       This API initializes the OBEX with the RFCOMM channel number or L2CAP
 *       PSM on which it will listen and the callback function to be called
 *       on receiving an event.
 *
 *  \param [out] obex_handle
 *      Handle that points to the OBEX client/server session
 *
 *  \param [in] init_params
 *      The parameters that will be used for initializing the server instance
 *
 *  \param [in] role
 *      OBEX role (Client/Server)
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_obex_register
           (
               /* OUT */ OBEX_HANDLE      * obex_handle,
               /* IN */  OBEX_INIT_PARAMS * init_params,
               /* IN */  UCHAR              role
           );

/**
 *  \brief To un-initialize an obex entity.
 *
 *  \par Description:
 *      This API puts the OBEX instance in a non-connectable state.
 *
 *  \param [in] obex_handle
 *      Handle that points to the OBEX client/server session
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_obex_unregister
           (
               /* IN */ OBEX_HANDLE * obex_handle
           );

/**
 *  \brief To send OBEX request or response.
 *
 *  \par Description:
 *      This API enables to send an OBEX request(like PUT/GET request) or to send
 *      resonse for OBEX request.
 *
 *  \param [in] handle
 *      Handle to refer the OBEX instance.
 *
 *  \param [in] opcode
 *      OBEX operation code.
 *
 *  \param [in] obj_hdr
 *      List of OBEX headers to be transmitted in the request/response.
 *
 *  \param [in] num_hdr
 *      Number of OBEX headers being transmitted.
 *
 *  \param [in] rsp_code
 *      Response code being sent by the server.
 *      For response codes refer to the section "OBEX Response codes" above.
 *
 *  \param [in] flags
 *      Specifies if the request/response is a final request/response or not.
 *         - (flags != 0) correspond to a final request/response.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
API_RESULT BT_obex_reqrsp
           (
               /* IN */ OBEX_HANDLE     * handle,
               /* IN */ UCHAR             opcode,
               /* IN */ OBEX_OBJ_HEADER * obj_hdr,
               /* IN */ UINT16            num_hdr,
               /* IN */ UCHAR             rsp_code,
               /* IN */ UCHAR             flags
           );

#ifdef OBEX_SUPPORT_RELIABLE_SSN
/**
 *  \brief To send OBEX reliable request/response.
 *
 *  \par Description:
 *      This API enables to send an OBEX reliable session request/response.
 *
 *  \param [in] handle
 *      Handle to refer the OBEX instance.
 *
 *  \param [in] opcode
 *      OBEX operation code.
 *
 *  \param [in] timeout
 *      Session Timeout Value.
 *
 *  \param [in] ssn_params
 *      Session parameters.
 *
 *  \param [in] ssn_params_len
 *      Session parameters length.
 *
 *  \param [in] rsp_code
 *      Response op code to OBEX request.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
API_RESULT BT_obex_ssn_reqrsp
           (
               /* IN */ OBEX_HANDLE     * handle,
               /* IN */ UCHAR             opcode,
               /* IN */ UINT32            timeout,
               /* IN */ UCHAR           * ssn_params,
               /* IN */ UCHAR             ssn_params_len,
               /* IN */ UCHAR             rsp_code
           );
#endif /* OBEX_SUPPORT_RELIABLE_SSN */


/* ================== OBEX Utility APIs =================================*/

/**
 *  \brief To retrieve a peer's maximum receive data size.
 *
 *  \par Description:
 *      This API gets the maximum size of an OBEX packet that the
 *      peer device can receive.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX instance.
 *
 *  \param [out] max_rx_size
 *      Maximum size of OBEX packet that the peer device can receive.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
API_RESULT BT_obex_get_peer_recv_size
           (
               /* IN  */ OBEX_HANDLE * obex_handle,
               /* OUT */ UINT16      * max_rx_size
           );

/**
 *  \brief Utility to convert a string from ASCII to unicode.
 *
 *  \par Description:
 *      This API converts a string from ASCII to unicode format.
 *
 *  \param [out] wchar_str
 *      The converted unicode string.
 *
 *  \param [in] ascii_str
 *      ASCII string that is to be converted.
 *
 *  \return API_RESULT
 *      Length of the unicode string
 *
 *  \note
 *      None
 */
UINT16 BT_obex_char_to_unicode
       (
           /* OUT */ UINT16 * wchar_str,
           /* IN  */ UINT8  * ascii_str
       );

/**
 *  \brief Utility to convert a string from unicode to ASCII.
 *
 *  \par Description:
 *      This API converts a string from unicode to ASCII format.
 *
 *  \param [in] wchar_str
 *      Unicode string that is to be converted.
 *
 *  \param [out] ascii_str
 *      The converted ASCII string.
 *
 *  \return API_RESULT
 *      Length of the string converted to ASCII
 *
 *  \note
 *      None
 */
UINT16 BT_obex_unicode_to_char
       (
           /* IN  */ UINT8 * wchar_str,
           /* OUT */ UINT8 * ascii_str
       );

/**
 *  \brief Utility to extract the OBEX header and related data
 *         from a given OBEX received message.
 *
 *  \par Description:
 *      This API extracts the OBEX header and its related data from the data
 *      stream that is passed by the profile/application layer. This function
 *      is called repeatedly to parse the string of headers and related data
 *      received in a data stream.
 *
 *  \param [in] data_stream
 *      The data stream to be parsed.
 *
 *  \param [in] data_length
 *      The total length of the data stream left to be parsed.
 *
 *  \param [out] length_parsed
 *      The length of the headers and data that is parsed is returned in this
 *      argument. This can be used in the following invocation of the
 *      function to offset the data pointer forward by 'length_parsed' length.
 *
 *  \param [out] obj_hdr
 *      Structure into which the parsed data is filled in.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
API_RESULT BT_obex_get_next_header
           (
               /* IN  */ UCHAR           * data_stream,
               /* IN  */ UINT16            data_length,
               /* OUT */ UINT16          * length_parsed,
               /* OUT */ OBEX_OBJ_HEADER * obj_hdr
           );

/**
 *  \brief Utility to extract headers and related data from
 *         a given received OBEX message.
 *
 *  \par Description:
 *      This API extracts OBEX headers and its related data from the data
 *      stream that is passed by the profile/application. It fills up obj_hdr with the
 *      extracted OBEX headers.
 *
 *  \param [in] data
 *      The OBEX data stream to be parsed.
 *
 *  \param [in] datalen
 *      The total length of the data stream left to be parsed. It can be
 *      used to ensure that the length of the received data as parsed
 *      from the data stream is less than or equal to, the total
 *      length of the packet.
 *
 *  \param [out] hdr
 *      Structure into which the parsed data is filled in.
 *
 *  \param [out] header_num
 *      Number of OBEX headers extracted.
 *
 *  \return API_RESULT
 *      Always returns API_SUCCESS.
 *      Caller has to ensure that pointer 'hdr' point to array of OBEX header
 *      and also need to use the extracted number of OBEX headers 'header_num'.
 */
API_RESULT BT_obex_extract_obex_headers
           (
               /* IN  */  UCHAR             * data,
               /* IN  */  UINT16              datalen,
               /* OUT */  OBEX_OBJ_HEADER   * hdr,
               /* OUT */  UCHAR             * header_num
           );

/**
 *  \brief Utility to search a given OBEX header identifier.
 *
 *  \par Description:
 *      This API is used to search a given OBEX header identifier and
 *      get the header value and value length.
 *
 *  \param [in] data
 *      The OBEX data stream to be parsed.
 *
 *  \param [in] datalen
 *      The total length of the data stream to be parsed.
 *
 *  \param [in] header_id
 *      The OBEX header identifier to be searched.
 *
 *  \param [out] value
 *      Searched OBEX header value.
 *
 *  \param [out] value_len
 *      Searched OBEX header value length.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 */
API_RESULT BT_obex_search_header_val
           (
               /* IN  */ UCHAR   * data,
               /* IN  */ UINT16    datalen,
               /* IN  */ UCHAR     header_id,
               /* OUT */ UCHAR  ** value,
               /* OUT */ UINT16  * value_len
           );

/**
 *  \brief Utility to get the available free OBEX frame length.
 *
 *  \par Description:
 *      This API is used to get the available free OBEX packet length from
 *      the given OBEX headers. It uses the maximum OBEX packet length to calculate the
 *      available free data length.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX instance.
 *
 *  \param [in] obj_hdr
 *      OBEX headers with value & corresponding value length.
 *
 *  \param [in] num_of_hdrs
 *      No. of OBEX headers.
 *
 *  \param [out] frame_len
 *      Total available free data length.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 */
API_RESULT BT_obex_get_available_frame_len
           (
               /* IN  */ OBEX_HANDLE     * obex_handle,
               /* IN  */ OBEX_OBJ_HEADER * obj_hdr,
               /* IN  */ UINT16            num_of_hdrs,
               /* OUT */ UINT16          * frame_len
           );

/**
 *  \brief Utility to extract the OBEX connect flags.
 *
 *  \par Description:
 *      This API is used to extract the connect flags in the OBEX connect request
 *      or OBEX connect response.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX instance.
 *
 *  \param [out] connect_flag
 *      8-bit connect flags.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      All connect request flags except bit 0 are currently reserved and are set to zero,
 *      and shall be ignored by the receiving side. All connect response flags are also set
 *      to zero and shall be ignored at the receiving side.
 */
API_RESULT BT_obex_get_connect_flag
           (
               /* IN  */ OBEX_HANDLE * obex_handle,
               /* OUT */ UCHAR       * connect_flag
           );

/**
 *  \brief Utility to extract the OBEX setpath flags.
 *
 *  \par Description:
 *      This API is used to extract the OBEX setpath flags of latest setpath request.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX server instance.
 *
 *  \param [out] setpath_flag
 *      8-bit setpath flags.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      The valid flags are only the first two bits of the setpath_flag is valid.
 *      Bit 0: Backup a level before applying the name (equivalent to ../ on many systems)
 *      Bit 1: Don't create folder if it does not exist, instead return an Error
 */
API_RESULT BT_obex_get_setpath_flag
           (
               /* IN  */ OBEX_HANDLE * obex_handle,
               /* OUT */ UCHAR       * setpath_flag
           );

/**
 *  \brief Utility to extract the Bluetooth address of remote device.
 *
 *  \par Description:
 *      This API is used to extract the Bluetooth address of peer device.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX instance.
 *
 *  \param [out] remote_bd_addr
 *      Bluetooth address of the peer device.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
API_RESULT BT_obex_get_remote_bd_addr
           (
               /* IN  */ OBEX_HANDLE * obex_handle,
               /* OUT */ UCHAR       * remote_bd_addr
           );

#ifdef OBEX_ENH_SUPPORT
/**
 *  \brief Utility to extract the OBEX action flags.
 *
 *  \par Description:
 *      This API is used to extract the OBEX action flags of latest action request.
 *
 *  \param [in] obex_handle
 *      Handle to refer the OBEX server instance.
 *
 *  \param [out] action_flag
 *      8-bit action flags.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      0x00: Copy
 *      0x01: Move/Rename
 *      0x02: Set Permissions
 */
API_RESULT BT_obex_get_action_flag
           (
               /* IN  */ OBEX_HANDLE * obex_handle,
               /* OUT */ UCHAR       * action_flag
           );
#endif /* OBEX_ENH_SUPPORT */

#ifdef OBEX_OVER_L2CAP
/**
 *  \brief To get the OBEX SRM status.
 *
 *  \par Description:
 *      This API is used to extract the OBEX SRM status of a device.
 *
 *  \param [in] handle
 *      Handle to refer the OBEX instance.
 *
 *  \param [in] device
 *      Flag to indicate local or remote device SRM status.
 *
 *  \param [out] value
 *      OBEX SRM status (OBEX_SRM_ENABLE or OBEX_SRM_DISABLE)
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
API_RESULT BT_obex_get_srm_status
           (
               /* IN  */ OBEX_HANDLE * handle,
               /* IN  */ UCHAR         device,
               /* OUT */ UCHAR       * value
           );

/**
 *  \brief Utility to get the OBEX SRM status of local device.
 *
 *  \par Description:
 *      This API is used to extract the OBEX SRM status of local device.
 *
 *  \param [in] handle
 *      Handle to refer the OBEX instance.
 *
 *  \param [out] value
 *      OBEX SRM status (OBEX_SRM_ENABLE or OBEX_SRM_DISABLE)
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_get_local_srm_status(handle, value) \
        BT_obex_get_srm_status ((handle), 0x01U, (value))

/**
 *  \brief Utility to get the OBEX SRM status of peer device.
 *
 *  \par Description:
 *      This API is used to extract the OBEX SRM status of peer device.
 *
 *  \param [in] handle
 *      Handle to refer the OBEX instance.
 *
 *  \param [out] value
 *      OBEX SRM status (OBEX_SRM_ENABLE or OBEX_SRM_DISABLE)
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
#define BT_obex_get_peer_srm_status(handle, value) \
        BT_obex_get_srm_status((handle), 0x02U, (value))
#else /* OBEX_OVER_L2CAP */
#define BT_obex_get_local_srm_status(handle, value) API_FAILURE
#define BT_obex_get_peer_srm_status(handle, value)  API_FAILURE
#define BT_obex_get_srm_status(handle, type, value) API_FAILURE
#endif /* OBEX_OVER_L2CAP */

/**
 *  \brief To generate a digest string from a nonce.
 *
 *  \par Description:
 *      This API generates a digest string from a nonce value.
 *      Time-stamp can be used as nonce
 *
 *  \param [out] digest_string
 *      The generated digest string
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
API_RESULT BT_obex_generate_digest_string
           (
               /* OUT */ UCHAR * digest_string
           );

/**
 *  \brief To generate a digest response for a given authentication challenge.
 *
 *  \par Description:
 *      This API generates a digest response for the received digest
 *      challenge. In addtion to the received challenge string, the password
 *      and password length are also used in generating the digest reponse.
 *
 *  \param [in] digest_challenge
 *      The received digest challenge.
 *
 *  \param [out] digest_response
 *      The generated digest response.
 *
 *  \param [in] pwd
 *      The password to be used for response calculation.
 *
 *  \param [in] pwd_len
 *      The length of the password.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
API_RESULT BT_obex_generate_digest_rsp
           (
               /* IN  */ UCHAR * digest_challenge,
               /* OUT */ UCHAR * digest_response,
               /* IN  */ UCHAR * pwd,
               /* IN  */ UCHAR   pwd_len
           );

/**
 *  \brief To validate an authentication response.
 *
 *  \par Description:
 *      This API validates the authentication response from a peer device.
 *
 *  \param [in] local_nonce
 *      Nonce to be used for generating a digest.
 *
 *  \param [in] digest_response
 *      The received digest string which needs to be validated.
 *
 *  \param [in] pwd
 *      The password to be used.
 *
 *  \param [in] pwd_len
 *      Length of the password.
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing
 *      the reason for failure
 *
 *  \note
 *      None
 */
API_RESULT BT_obex_validate_auth_rsp
           (
               /* IN */ UCHAR * local_nonce,
               /* IN */ UCHAR * digest_response,
               /* IN */ UCHAR * pwd,
               /* IN */ UCHAR   pwd_len
           );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_OBEX_API_ */

