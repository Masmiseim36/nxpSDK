
/**
 *  \file BT_opp_api.h
 *
 *  \brief EtherMind OPP API Header File.
 *
 *  This Header File contains declaration of OPP APIs, Macros and
 *  Constant Definitions for use in applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_OPP_API_
#define _H_BT_OPP_API_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_obex_api.h"

/* ----------------------------------------- Exported Global Variables */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 *  \defgroup  opp_module OPP (Object Push Profile)
 *  \{
 *
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Object Push Profile module to the Application and other upper
 *  layers of the stack.
 */
/**
 * \defgroup opp_defines Defines
 * \{
 * Describes defines for the module.
 */

/**
 * \defgroup opp_constants Constants
 * \{
 * Describes Constants defined by the module.
 */

#ifdef OPP_1_2
/* OBEX L2CAP PSM for local client entities */
#define OPP_OBEX_L2CAP_PSM    0x8401U
#endif /* OPP_1_2 */

/**
 * OPP Client Operations
 *
 *  1. Push Objects
 *       - can be either vCard, vCal, vNote, vMessage => OBEX PUT
 * 2. Pull Business Card
 *       - can be either vCard, vCal, vNote, vMessage => OBEX_GET
 * 3. Exchange Business Card
 *       - can be either vCard, vCal, vNote, vMessage  => OBEX_PUT followed by OBEX_GET
 */

/* OPP Operations */
#define OPP_OP_NONE                                 0x01U
#define OPP_OP_PUSH_OBJECT                          0x02U
#define OPP_OP_PULL_BUSINESS_CARD                   0x03U
#define OPP_OP_EXCH_BUSINESS_CARD                   0x04U
#define OPP_OP_PULL_OBJECT                          0x05U

/**
 * OPP Object Type Header.Values
 * These defines are taken from OBEX specification.
 */
#define OPP_TYPE_HDR_VCARD                          "text/x-vcard"

/* #### OPP Client Events #### */
/**
 *  \defgroup  OPP_CLIENT_EVENTS OPP Client Events
 *  \{
 */
/* For Connection */
/**
 * @name OPP Client Events for Connection
 *
 * Constant Defines for Connection belonging to \ref OPP_CLIENT_EVENTS
 */
/*@{*/
#define OPP_CLIENT_CONNECT_CNF                   0x01U
#define OPP_CLIENT_DISCONNECT_CNF                0x02U
#define OPP_CLIENT_TRANSPORT_CLOSE_CNF           0x03U
#define OPP_CLIENT_TRANSPORT_CLOSE_IND           0x04U
#define OPP_CLIENT_PUSH_OBJECT_CNF               0x05U
#define OPP_CLIENT_PULL_BUSINESS_CARD_CNF        0x06U
#define OPP_CLIENT_EXCH_BUSINESS_CARD_CNF        0x07U
#define OPP_CLIENT_ABORT_CNF                     0x08U
/*@}*/
/** \} */

/* #### OPP Server Events #### */
/**
 *  \defgroup  OPP_SERVER_EVENTS OPP Server Events
 *  \{
 */
/**
 * @name OPP Server Events for Connection
 *
 * Constant Defines for Connection belonging to \ref OPP_SERVER_EVENTS
 */
/*@{*/
#define OPP_SERVER_CONNECT_IND                   0x01U
#define OPP_SERVER_DISCONNECT_IND                0x02U
#define OPP_SERVER_TRANSPORT_CLOSE_CNF           0x03U
#define OPP_SERVER_TRANSPORT_CLOSE_IND           0x04U
#define OPP_SERVER_PUSH_OBJECT_IND               0x05U
#define OPP_SERVER_PULL_BUSINESS_CARD_IND        0x06U
#define OPP_SERVER_ABORT_IND                     0x07U
/** \} */
#define OPP_EVENT_INVALID                        0xFFU
#define OPP_ENTITY_INVALID                       0xFFU
/** \} */

/* OPP OBEX Response Codes */
/**
 * @name OPP OBEX Response Codes
 *
 * Constant Defines for OPP OBEX Response Codes
 */
/*@{*/
#define OPP_SUCCESS_RSP                          OBEX_SUCCESS_RSP
#define OPP_CONTINUE_RSP                         OBEX_CONTINUE_RSP
#define OPP_BAD_REQ_RSP                          OBEX_REQ_IS_BAD_RSP
#define OPP_NOT_IMPLEMENTED_RSP                  OBEX_NOT_IMP_RSP
#define OPP_UNAUTH_RSP                           OBEX_UNAUTHORIZED_RSP
#define OPP_PRECOND_FAILED_RSP                   OBEX_PRECOND_FAIL_RSP
#define OPP_NOT_FOUND_RSP                        OBEX_NOT_FOUND_RSP
#define OPP_NOT_ACCEPTABLE_RSP                   OBEX_NOT_ACCEPTABLE_RSP
#define OPP_NO_SERVICE_RSP                       OBEX_NO_SERVICE_RSP
#define OPP_FORBIDDEN_RSP                        OBEX_FORBIDDEN_RSP
#define OPP_SERVER_ERROR                         OBEX_SERVER_ERR_RSP
#define OPP_NOT_MODIFIED_RSP                     OBEX_NOT_MODIFIED_RSP
#define OPP_DATABASE_FULL_RSP                    OBEX_DATABASE_FULL_RSP
#define OPP_CONFLICT_RSP                         OBEX_CONFLICT_RSP
#define OPP_UNSUPP_MEDIA_RSP                     OBEX_UNSUPP_MEDIA_RSP
#define OPP_REQ_ENTITY_LARGE_RSP                 OBEX_REQ_ENTITY_LARGE_RSP
/*@}*/

/** \} */
/** \} */
/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Structures/Data Types */

/**
 *  \addtogroup opp_defines Defines
 *  \{
 */

/**
 *  \addtogroup opp_structures Structures
 *  \{
 */

/**
 *  \brief OPP Handle
 *
 *  \par Description:
 *       This datatype is used to associate a unique handle with each OPP instance.
 */
typedef UCHAR OPP_HANDLE;

/**
 *  \brief Data type to specify the information used while calling different APIs.
 *
 *  \par Description:
 *       This structure contains the pointer to a buffer of UCHAR and the length of the buffer.
 *       This structure is filled by the application and given to the profile in various API's.
 */
typedef struct _OPP_HEADER_STRUCT
{
    /**
     * Pointer to the buffer holding the data
     */
    UCHAR   * value;

    /**
     * Length of the data in the buffer.
     */
    UINT16    length;
} OPP_HEADER_STRUCT;

/**
 *  \brief Data type to specify the OPP connect request or connect response parameters.
 *
 *  \par Description:
 *       This structure contains connect parameters filled by the application or the profile
 *       depending on connection request or connection response.
 *
 *       At OPP client side, this structure is filled by the application to send the connect
 *       request & filled by the profile to notify the application with the connect response.
 *       At OPP server side, this structure is filled by the profile to notify the application
 *       of the connect request and the application to send the connect response.
 *
 *  \note
 *       OPP OBEX connection over L2CAP is defined in OPPv1.2 or later specification.
 *       To have OPP OBEX connection over L2CAP support, enable OBEX_OVER_L2CAP feature flag
 *       in BT_features.h.
 */
typedef struct _OPP_CONNECT_STRUCT
{
    /**
     * Bluetooth device address of remote device
     */
    UCHAR       * bd_addr;

    /**
     * Maximum receive size in bytes
     */
    UINT16      max_rx_size;

    /**
     * Maximum tranmit size in bytes
     */
    UINT16      max_tx_size;

    /**
     * The server channel for the OPP OBEX Connection over RFCOMM
     */
    UINT8       server_channel;

#ifdef OPP_1_2
    /**
     * The L2CAP PSM for the OPP OBEX Connection over L2CAP
     */
    UINT16      psm;         /* L2CAP PSM */
#endif /* OPP_1_2 */
}OPP_CONNECT_STRUCT;

/**
 *  \brief Data type to specify the OPP request parameters.
 *
 *  \par Description:
 *       This structure contains the request parameters filled by the application
 *       or the profile depending on the role.
 *
 *       At OPP client, this structure shall be filled by the application when a request has
 *       to be sent to the peer side and at the OPP server, this will be filled by the profile
 *       to notify the application of the request received.
 */
typedef struct _OPP_REQUEST_STRUCT
{
    /**
     * This contains the object name in request.
     */
    OPP_HEADER_STRUCT   *name;

    /**
     * This contains the content of object format in request.
     */
    OPP_HEADER_STRUCT   *type;

    /**
     * This contains the content of file body in request
     */
    OPP_HEADER_STRUCT   *body;

    /**
     * This indicates the object length
     */
    UINT32              obj_len_info;

    /**
     * Wait specifier during SRM transaction
     */
    UCHAR               wait;
} OPP_REQUEST_STRUCT;

/**
 *  \brief Data type to specify the OPP response parameters.
 *
 *  \par Description:
 *       This structure contains the response parameters filled by the application
 *       or the profile depending on the role.
 *
 *       At OPP server, this structure shall be filled by the application to send the
 *       response for the recently received request and at OPP client, this structure
 *       will be filled by the profile to notify the application of the response indication
 *       for the previously sent request.
 */
typedef struct _OPP_RESPONSE_STRUCT
{
    /**
     * This contains the name of the object in response
     */
    OPP_HEADER_STRUCT   *name;

    /**
     * This contains the content of the object in response.
     */
    OPP_HEADER_STRUCT   *body;

    /**
     * Wait specifier during SRM transaction.
     */
    UCHAR               wait;
} OPP_RESPONSE_STRUCT;

/**
 *  \brief Data type to specify the OPP header information.
 *
 *  \par Description:
 *       Structure to specify the information related to connect request, connect response,
 *       other requests and responses both for the OPP client and server.
 *
 *       Depending upon the request/ response, the corresponding structure in this Union to be used.
 */
typedef union _OPP_HEADERS
{
    /**
     * This parameter is used to send/receive a request.
     */
    OPP_REQUEST_STRUCT   *opp_req_info;

    /**
     * This parameter is used to send/receive a response.
     */
    OPP_RESPONSE_STRUCT  *opp_rsp_info;

    /**
     * This parameter is used when the connect request or the connect
     * response is being sent.
     */
    OPP_CONNECT_STRUCT   *opp_connect_info;
} OPP_HEADERS;
/** \} */
/** \} */

/**
 *  \defgroup opp_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
/**
 *  \brief OPP event notification callback type
 *
 *  \par Description:
 *       This callback is called by OPP client/server to report various asynchronous
 *       events that has occurred. It can be due to the any request originating from the
 *       peer side or to confirm the result of any request made by the local application.
 *
 *       Purpose:
 *        - To confirm result of an operation initiated by the application.
 *        - To inform application of the asynchronous events.
 *
 *  \param [in] opp_handle
 *         Valid OPP handle of the OPP session.
 *
 *  \param [in] event_type
 *         This parameter defines the FTP server/client event being reported.
 *
 *  \param [in] event_result
 *         This parameter indicates the success or failure of the operation that
 *         is reported through the event type.
 *
 *  \param [in] opp_headers
 *         Depending upon the event_type, this parameter contains one of the following information:
 *          - Connection information
 *          - Request information
 *          - Response information
 *
 *  \param [in] num_headers
 *         This parameter indicates the number of headers passed through the
 *         opp_headers parameter.
 */
typedef API_RESULT (* OPP_EVENT_NTF_CB)
                   (
                       /* IN */ OPP_HANDLE      * opp_handle,
                       /* IN */ UINT8             event_type,
                       /* IN */ UINT16            event_result,
                       /* IN */ OPP_HEADERS     * opp_headers,
                       /* IN */ UINT16            num_headers
                   );
/** \} */

/* ----------------------------------------- Macros  */
/**
 *  \defgroup opp_marcos Utility Macros
 *  \{
 *  Initialization and other Utility Macros offered by the module.
 */
/** To initialize a Header Structure */
#define OPP_INIT_HEADER_STRUCT(hs)    \
        (hs).value = NULL;    \
        (hs).length = 0U;
/** \} */


/* ----------------------------------------- Functions */
/**
 * \defgroup  opp_api API defintions
 * \{
 * Describes API defintiions of OPP Module
 */
#ifdef __cplusplus
extern "C" {
#endif

/* === OPP Client API's ==== */
/**
 *  \brief To initialize OPP client module
 *
 *  \par Description:
 *       This API initializes the OPP client module. This function must be
 *       called (only once) before any other OPP client APIs are used.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 *       This API can be called again to re-initialize OPP client module, after a
 *       successful call to BT_opp_client_shutdown() API.
 */
API_RESULT BT_opp_client_init(void);

/**
 *  \brief To shutdown OPP client module.
 *
 *  \par Description:
 *       This API shuts down the OPP client module.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note This API shall be called only when there is no existing OPP connection
 *        with any remote device. If such a connection exists, it must be closed
 *        by calling BT_opp_client_disconnect() API.
 */
API_RESULT BT_opp_client_shutdown(void);

/**
 *  \brief To start an instance of OPP client.
 *
 *  \par Description:
 *       This API starts an OPP client instance. On the successful instantiation,
 *       this API sets and returns the OPP handle. This also registers the
 *       application callback function.
 *
 *  \param [out] opp_handle
 *         Identifier of the OPP client entity that is started.
 *
 *  \param [in] opp_client_cb
 *         The application callback function pointer to be registered.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_opp_client_start
           (
               /* OUT */ OPP_HANDLE       *opp_handle,
               /* IN */  OPP_EVENT_NTF_CB  opp_client_cb
           );

/**
 *  \brief To stop an active OPP client instance.
 *
 *  \par Description:
 *       This API stops the OPP client instance corresponding to the specified
 *       handle. An instance cannot be stopped when a transaction is going on
 *       or when the connection still exists. This API returns Error in such cases.
 *
 *  \param [in] opp_handle
 *         OPP client handle that has to be stopped.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 */
API_RESULT BT_opp_client_stop(/* IN */ OPP_HANDLE *opp_handle);

/**
 *  \brief To establish connection to the OPP server.
 *
 *  \par Description:
 *       This API establishes the OBEX connection with the remote OPP server.
 *       OPP client calls the registered application callback upon the
 *       completion of the connection process (when the OPP server responds
 *       with the connection response) with the notification OPP_CLIENT_CONNECT_CNF.
 *
 *  \param [in] opp_handle
 *          OPP handle of the client that has to be connected.
 *
 *  \param [in] opp_connect_info
 *          Structure containing the Bluetooth device address of the OPP server
 *          and the local maximum receive capacity (in bytes), RFCOMM server
 *          channel and L2capPsm value.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 *       1. Before calling this API the application needs to ensure that the OPP
 *          client session has been started by calling BT_opp_client_start().
 *       2. OPP OBEX connection over L2CAP is introduced from OPPv1.2.
 *          To have OPP OBEX connection over L2CAP support, enable OBEX_OVER_L2CAP feature flag in BT_features.h
 *       3. When OBEX_OVER_L2CAP is enabled:
 *           - For OPP OBEX connection over L2CAP, set the rfcomm server channel value to 0.
 *           - For OPP OBEX connection over RFCOMM, set the l2cap psm value to 0.
 */
API_RESULT BT_opp_client_connect
           (
               /* IN */ OPP_HANDLE          * opp_handle,
               /* IN */ OPP_CONNECT_STRUCT  * opp_connect_info
           );

/**
 *  \brief To disconnect the established OPP connection.
 *
 *  \par Description:
 *       This API is to disconnect the OPP connection with the remote OPP server.
 *       OPP client calls the registered application callback upon completion of
 *       this request with the notification OPP_CLIENT_DISCONNECT_CNF.
 *
 *  \param [in] opp_handle
 *         Valid OPP handle of the client  instance to be disconnected.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note OPP client automatically initiates transport disconnection once the
 *        OPP disconnection completes and the application shall be notified
 *        with OPP_CLIENT_DISCONNECT_CNF and OPP_CLIENT_TRANSPORT_CLOSE_CNF
 *        for OPP disconnection and transport disconnection completion respectively.
 *        The instance will be stopped after the transport disconnection completion.
 */
API_RESULT BT_opp_client_disconnect(/* IN */ OPP_HANDLE *opp_handle);

/**
 *  \brief To push an object to the connected OPP server.
 *
 *  \par Description:
 *       This API pushes an object of given format to the already connected OPP
 *       server device. The application should send the contents of the object
 *       in the OPP_REQUEST_STRUCT. If the file size exceeds the negotiated OBEX
 *       packet length, then BT_opp_client_send_request() can be used to push
 *       remaining part of the object. The application will be notified each
 *       time the OPP server responds to the request. The application can take
 *       further actions depending on the notification.
 *       The notification that are given to the application for the push operation is:
 *       OPP_CLIENT_PUSH_OBJECT_CNF
 *
 *  \param [in] opp_handle
 *         Valid OPP instance handle.
 *
 *  \param [in] opp_req_info
 *         This structure contains the type, name, body and size of the object.
 *
 *  \param [in] opp_operation
 *          This indicate the OPP operation. like
 *           - OPP_OP_PUSH_OBJECT
 *           - OPP_OP_EXCH_BUSINESS_CARD
 *
 *  \param [in] more
 *         It Indicates if application has remaining object still to be sent or
 *         it is the complete object content
 *         1 - Object part remaining
 *         0 - No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT opp_client_push_object
           (
                /* IN */  OPP_HANDLE           *opp_handle,
                /* IN */  OPP_REQUEST_STRUCT   *opp_req_info,
                /* IN */  UCHAR                 opp_operation,
                /* IN */  UCHAR                 more,
                /* OUT */ UINT16               *actual
           );


/**
 *  \brief To pull an object from the connected OPP server.
 *
 *  \par Description:
 *       This API pulls the object of given type from the already connected OPP
 *       server device. The application should send the contents of the object
 *       in the OPP_REQUEST_STRUCT. The application will be notified each
 *       time the OPP server responds to the request.
 *
 *  \param [in] opp_handle
 *         Valid OPP instance handle.
 *
 *  \param [in] opp_req_info
 *         This structure contains the type, name, body and size of the object.
 *
 *  \param [in] opp_operation
 *          This indicate the OPP operation. like
 *          - OPP_OP_PULL_BUSINESS_CARD
 *          - OPP_OP_PULL_OBJECT
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT opp_client_pull_object
           (
               OPP_HANDLE           *opp_handle,
               OPP_REQUEST_STRUCT   *opp_req_info,
               UCHAR                 operation
           );

/**
 *  \brief To push an object to the remote OPP server.
 *
 *  \par Description:
 *       This API pushes the given object to the connected
 *       OPP server device.
 *
 *       The application is notified each time the server responds with the data
 *       through the following notification along with the data.
 *       OPP_CLIENT_PUSH_OBJECT_CNF
 *
 *  \param [in] opp_handle
 *         Valid OPP instance handle.
 *
 *  \param [in] opp_req_info
 *         This structure contains the type the business card to be pulled.
 *
 *  \param [in] more
 *         It Indicates if application has remaining object still to be sent or
 *         it is the complete object content
 *         1 - Object part remaining
 *         0 - No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission. *
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the OPP connection to a OPP server through
 *        BT_opp_client_connect() API before using this API.
 */
#define BT_opp_client_push_object(opp_handle, opp_req_info, more, actual)\
      opp_client_push_object ((opp_handle), (opp_req_info), OPP_OP_PUSH_OBJECT, (more), (actual))

/**
 *  \brief To exchange a business card with the connected OPP server.
 *
 *  \par Description:
 *       This API exchanges an object of given format to the already connected OPP server device.
 *       First the application should send the contents of the object in the OPP_REQUEST_STRUCT.
 *       If the file size exceeds the negotiated OBEX packet length, then BT_opp_client_send_request()
 *       can be used to push remaining part of the object.
 *       The application will be notified each time the OPP server responds to the request
 *       The application can take further actions depending on the notification.
 *
 *       The notification that are given to the application for the push operation is:
 *       OPP_CLIENT_EXCH_BUSINESS_CARD_CNF
 *
 *  \param [in] opp_handle
 *         Valid OPP instance handle.
 *
 *  \param [in] opp_req_info
 *         This structure contains the type the business card to be pulled.
 *
 *  \param [in] more
 *         It Indicates if application has remaining object still to be sent or
 *         it is the complete object content
 *         1 - Object part remaining
 *         0 - No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the OPP connection to a OPP server through
 *        BT_opp_client_connect() API before using this API.
 */
#define BT_opp_client_exchange_business_card(opp_handle, opp_req_info, more, actual) \
      opp_client_push_object ((opp_handle), (opp_req_info), OPP_OP_EXCH_BUSINESS_CARD, (more), (actual))

/**
 *  \brief To pull business card from the remote OPP server.
 *
 *  \par Description:
 *       This API pulls the specified business card from the already connected
 *       OPP server device.
 *
 *       The application is notified each time the server responds with the data
 *       through the following notification along with the data.
 *       OPP_CLIENT_PULL_BUSINESS_CARD_CNF
 *
 *  \param [in] opp_handle
 *         Valid OPP instance handle.
 *
 *  \param [in] opp_req_info
 *         This structure contains the type the business card to be pulled.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the OPP connection to a OPP server through
 *        BT_opp_client_connect() API before using this API.
 */
#define BT_opp_client_pull_business_card(opp_handle, opp_req_info) \
      opp_client_pull_object ((opp_handle), (opp_req_info), OPP_OP_PULL_BUSINESS_CARD)

/**
 *  \brief To send request to the OPP server.
 *
 *  \par Description:
 *       This API sends the request to the OPP server with the event type and
 *       the data given by the application.  The application can call this API
 *       to continue any operation initiated and to which the first notification
 *       has been received. Thus this API shall be called from the OPP application
 *       notification callback function with the event type set to the received event.
 *
 *  \param [in] opp_handle
 *         Valid OPP instance handle.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback, to indicate
 *         that the request is the continuation of the operation for which it was
 *         notified.
 *
 *  \param [in] event_result
 *         This is the status of the request.
 *
 *  \param [in] opp_headers
 *         Structure containing either get request information, in case of get
 *         operation or put request information in case of put operation.
 *
 *  \param [in] more
 *         Indicates if application has remaining of object still to be sent or
 *         object complete
 *         1 - Object part remaining
 *         0 - No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the OPP connection to a OPP server through
 *        BT_opp_client_connect() API before using this API.
 */
API_RESULT BT_opp_client_send_request
           (
               /* IN */ OPP_HANDLE   *opp_handle,
               /* IN */ UINT8         event_type,
               /* IN */ UINT16        event_result,
               /* IN */ OPP_HEADERS  *opp_headers,
               /* IN */ UCHAR         more,
               /* OUT */ UINT16      *actual
           );

/**
 *  \brief To abort the ongoing OPP operation (push/pull).
 *
 *  \par Description:
 *       This API aborts the recent operation initiated by the client.
 *       The completion of abort operation is notified through
 *       OPP_CLIENT_ABORT_CNF notification event.
 *
 *  \param [in] opp_handle
 *         Valid OPP instance handle.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the OPP connection to a OPP server through
 *        BT_opp_client_connect() API before using this API.
 */
API_RESULT BT_opp_client_abort
           (
               /* IN */ OPP_HANDLE  *opp_handle
           );

/**
 *  \brief To disconnect the underlying transport connection with OPP server
 *
 *  \par Description:
 *       This API is to disconnect the transport connection with the remote OPP
 *       server. The OPP client calls the registered application callback upon
 *       completion of this request with notification OPP_CLIENT_TRANSPORT_CLOSE_CNF.
 *
 *  \param [in] opp_handle
 *         Valid OPP handle of the client instance.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_opp_client_transport_close
           (
               /* IN */ OPP_HANDLE  *opp_handle
           );

/* === OPP Server API's ==== */
/**
 *  \brief To initialize OPP server module.
 *
 *  \par Description:
 *       This API initializes the OPP server module. This function must be
 *       called (only once) before any other OPP server APIs are used.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note This API can be called again to re-initialize OPP Server module after
 *        a successful call to BT_opp_server_shutdown().
 */
API_RESULT BT_opp_server_init(void);

/**
 *  \brief To shutdown OPP server module.
 *
 *  \par Description:
 *       This API shuts down the OPP server.
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note This API shall be called only when there are no active/started
 *        instances of the OPP server. In case of active instances, they should
 *        be stopped by calling BT_opp_server_stop() API.
 */
API_RESULT BT_opp_server_shutdown(void);

/**
 *  \brief To start an OPP server instance.
 *
 *  \par Description:
 *       This API starts an OPP server instance. On the successful instantiation,
 *       this API returns OPP handle. This also registers the application callback
 *       function for the started OPP server instance.
 *
 *  \param [out] opp_handle
 *         Identifier of the OPP server entity that is started.
 *
 *  \param [in] opp_server_cb
 *         The application callback function pointer to be registered.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_opp_server_start
           (
               /* OUT */ OPP_HANDLE  *opp_handle,
               /* IN */  OPP_EVENT_NTF_CB opp_server_cb
           );

/**
 *  \brief To stop an active OPP server instance.
 *
 *  \par Description:
 *       This API stops the OPP server instance corresponding to the specified handle.
 *
 *  \param [in] opp_handle
 *         OPP handle of the server instance that has to be stopped.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note In case an active connection exists in the specified instance to be
 *        stopped, the transport for the connection is closed and the instance
 *        is stopped.
 */
API_RESULT BT_opp_server_stop(/* IN */ OPP_HANDLE *opp_handle);

/**
 *  \brief To send response to the latest request received from the OPP client.
 *
 *  \par Description:
 *       This API is used to send the response to the requests made by the OPP
 *       client. Response to connect, PUSH, PULL, Exchange business card etc.
 *       shall be sent through this API.
 *       The application shall call this API with the event type of the latest
 *       notification received. This indicates the response is for the latest
 *       indication received.
 *       OPP server shall frame the OBEX specific packets based on the response
 *       and associated response data and send it to the OPP client.
 *
 *  \param [in] opp_handle
 *         Valid OPP handle of the OPP session.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback. This is to
 *         indicate that the response being sent is response to the request for
 *         which the application was notified.
 *
 *  \param [in] event_result
 *         This is the OBEX response code.
 *
 *  \param [in] opp_headers
 *         Structure containing either the connect information in case of connect
 *         operation or the response data in case of other operation.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_opp_server_send_response
           (
               /* IN */ OPP_HANDLE      *opp_handle,
               /* IN */ UINT8           event_type,
               /* IN */ UCHAR           event_result,
               /* IN */ OPP_HEADERS     *opp_headers
           );

/**
 *  \brief To disconnect the underlying transport connection with OPP client.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection with the remote OPP
 *       client. The OPP server calls the registered application callback upon
 *       completion of this request with notification OPP_SERVER_TRANSPORT_CLOSE_CNF.
 *
 *  \param [in] opp_handle
 *         Valid OPP handle of the server instance.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_opp_server_transport_close
           (
               /* IN */ OPP_HANDLE  *opp_handle
           );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif  /* _H_BT_OPP_API_ */

