
/**
 *  \file BT_ftp_api.h
 *
 *  \brief EtherMind FTP API Header File.
 *
 *  This Header File contains declaration of FTP APIs, Macros and
 *  Constant Definitions for use in applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_FTP_API_
#define _H_BT_FTP_API_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_obex_api.h"

/* ----------------------------------------- Exported Global Variables */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 *  \defgroup  ftp_module FTP (File Transfer Profile)
 *  \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  File Transfer Profile (FTP) module to the Application.
 */

/**
 * \defgroup ftp_defines Defines
 * \{
 * Describes defines for the module.
 */

/**
 * \defgroup ftp_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
#ifdef FTP_1_3
/* OBEX L2CAP PSM for local client entities */
#define FTP_OBEX_L2CAP_PSM    0x8201U
#endif /* FTP_1_3 */

/**
 * FTP Client Operations
 *
 *  1. Set folders paths -> forward, backward, root => OBEX SETPAH
 *  2. Get folder contents -> Folder Listing.       => OBEX GET
 *  3. Push Objects   -> can be either file/folders => OBEX PUT
 *  4. Pull Objects   -> can be either file/folders => OBEX GET
 *  5. Delete Objects -> can be files/folders       => OBEX PUT
 *  6. Rename/Move/Copy/Set permissions -> Using OBEX action opertions => OBEX ACTION
 */

/* #### FTP Client Events #### */
/**
 *  \defgroup  FTP_CLIENT_EVENTS FTP Client Events
 *  \{
 */
/**
 * @name FTP Client Events for Connection
 *
 * Constant Defines for Connection belonging to \ref FTP_CLIENT_EVENTS
 */
/*@{*/
#define FTP_CLIENT_CONNECT_CNF                   0x01U
#define FTP_CLIENT_DISCONNECT_CNF                0x02U
#define FTP_CLIENT_TRANSPORT_CLOSE_CNF           0x03U
#define FTP_CLIENT_TRANSPORT_CLOSE_IND           0x04U
/*@}*/

/**
 * @name FTP Client Events for Folder Browsing
 *
 * Constant Defines for Folder Browsing belonging to \ref FTP_CLIENT_EVENTS
 */
/*@{*/
#define FTP_CLIENT_SET_FOLDER_CNF                0x05U
#define FTP_CLIENT_GET_FOLDER_LISTING_CNF        0x06U
/*@}*/

/**
 * @name FTP Client Events for Object Transfer
 *
 * Constant Defines for Object Transfer belonging to \ref FTP_CLIENT_EVENTS
 */
/*@{*/
#define FTP_CLIENT_PUSH_OBJ_CNF                  0x07U
#define FTP_CLIENT_PULL_OBJ_CNF                  0x08U
/*@}*/

/**
 * @name FTP Client Events for Object Manipulation
 *
 * Constant Defines for Object Manipulation belonging to \ref FTP_CLIENT_EVENTS
 */
/*@{*/
#define FTP_CLIENT_DELETE_OBJ_CNF                0x09U
#define FTP_CLIENT_ACTION_CNF                    0x0AU
#define FTP_CLIENT_ABORT_CNF                     0x0BU
/*@}*/
/** \} */

/* #### FTP Server Events #### */
/**
 *  \defgroup  FTP_SERVER_EVENTS FTP Server Events
 *  \{
 */

/**
 * @name FTP Server Events for Connection
 *
 * Constant Defines for Connection belonging to \ref FTP_SERVER_EVENTS
 */
/*@{*/
#define FTP_SERVER_CONNECT_IND                   0x01U
#define FTP_SERVER_DISCONNECT_IND                0x02U
#define FTP_SERVER_TRANSPORT_CLOSE_CNF           0x03U
#define FTP_SERVER_TRANSPORT_CLOSE_IND           0x04U
/*@}*/

/**
 * @name FTP Server Events for Folder Browsing
 *
 * Constant Defines for Folder Browsing belonging to \ref FTP_SERVER_EVENTS
 */
/*@{*/
#define FTP_SERVER_SET_FOLDER_IND                0x05U
#define FTP_SERVER_GET_FOLDER_LISTING_IND        0x06U
/*@}*/

/**
 * @name FTP Server Events for Object Transfer
 *
 * Constant Defines for Object Transfer belonging to \ref FTP_SERVER_EVENTS
 */
/*@{*/
#define FTP_SERVER_PUSH_FILE_IND                 0x07U
#define FTP_SERVER_PULL_FILE_IND                 0x08U
/*@}*/

/**
 * @name FTP Server Events for Object Manipulation
 *
 * Constant Defines for Object Manipulation belonging to \ref FTP_SERVER_EVENTS
 */
/*@{*/
#define FTP_SERVER_DELETE_FILE_IND               0x09U
#define FTP_SERVER_ACTION_IND                    0x0AU
#define FTP_SERVER_ABORT_IND                     0x0BU
/*@}*/

#define FTP_EVENT_INVALID                        0xFFU
#define FTP_ENTITY_INVALID                       0xFFU
/** \} */

/**
 * @name FTP OBEX Response Codes
 *
 * Constant Defines for OBEX Response Codes
 */
/*@{*/
#define FTP_SUCCESS_RSP                          OBEX_SUCCESS_RSP
#define FTP_CONTINUE_RSP                         OBEX_CONTINUE_RSP

#define FTP_BAD_REQ_RSP                          OBEX_REQ_IS_BAD_RSP
#define FTP_NOT_IMPLEMENTED_RSP                  OBEX_NOT_IMP_RSP
#define FTP_UNAUTH_RSP                           OBEX_UNAUTHORIZED_RSP
#define FTP_PRECOND_FAILED_RSP                   OBEX_PRECOND_FAIL_RSP
#define FTP_NOT_FOUND_RSP                        OBEX_NOT_FOUND_RSP
#define FTP_NOT_ACCEPTABLE_RSP                   OBEX_NOT_ACCEPTABLE_RSP
#define FTP_NO_SERVICE_RSP                       OBEX_NO_SERVICE_RSP
#define FTP_FORBIDDEN_RSP                        OBEX_FORBIDDEN_RSP
#define FTP_SERVER_ERROR                         OBEX_SERVER_ERR_RSP
#define FTP_NOT_MODIFIED_RSP                     OBEX_NOT_MODIFIED_RSP
#define FTP_DATABASE_FULL_RSP                    OBEX_DATABASE_FULL_RSP
#define FTP_CONFLICT_RSP                         OBEX_CONFLICT_RSP
/*@}*/

/**
 * @name FTP SetPath parameter values
 *
 * Constant Defines for FTP SetPath parameter values
 */
/*@{*/
#define FTP_SET_NEW_FOLDER                       0x00U
#define FTP_SET_CHILD_FOLDER                     0x01U
#define FTP_SET_PARENT_FOLDER                    0x02U
#define FTP_SET_ROOT_FOLDER                      0x03U
/*@}*/

#ifdef OBEX_ENH_SUPPORT
/**
 * @name FTP Action Identifiers
 *
 * Constant Defines for FTP Action Identifiers
 */
/*@{*/
#define  FTP_ACTION_ID_COPY                     0x00U /* OBEX_ACTION_COPY_ID */
#define  FTP_ACTION_ID_MOVE                     0x01U /* OBEX_ACTION_MOVE_ID */
#define  FTP_ACTION_ID_SET_PER                  0x02U /* OBEX_ACTION_SET_OBJ_PER_ID */
/*@}*/
#endif /* OBEX_ENH_SUPPORT */
/** \} */
/** \} */

/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Structures/Data Types */

/**
 *  \addtogroup ftp_defines Defines
 *  \{
 */

/**
 *  \addtogroup ftp_structures Structures
 *  \{
 */

/**
 *  \brief FTP Handle
 *
 *  \par Description:
 *       This datatype is used to associate a unique handle with each FTP instance.
 */
typedef UCHAR FTP_HANDLE;

/**
 *  \brief Data type to specify the information used while calling different APIs.
 *
 *  \par Description:
 *       This structure contains the pointer to a buffer of UCHAR and the length of the buffer.
 *       This structure is filled by the application and given to the profile in various API's.
 */
typedef struct _FTP_HEADER_STRUCT
{
    /**
     * Pointer to the buffer holding the data
     */
    UCHAR   * value;

    /**
     * Length of the data in the buffer.
     */
    UINT16    length;
} FTP_HEADER_STRUCT;

/**
 *  \brief Data type to specify the FTP connect request or connect response parameters.
 *
 *  \par Description:
 *       This structure contains connect parameters filled by the application or the profile
 *       depending on connection request or connection response.
 *
 *       At FTP client side, this structure is filled by the application to send the connect
 *       request & filled by the profile to notify the application with the connect response.
 *       At FTP server side, this structure is filled by the profile to notify the application
 *       of the connect request and the application to send the connect response.
 *
 *  \note
 *       FTP OBEX connection over L2CAP is defined in FTPv1.2 or later specification.
 *       To have FTP OBEX connection over L2CAP support, enable OBEX_OVER_L2CAP feature flag.
 */
typedef struct _FTP_CONNECT_STRUCT
{
    /**
     * Bluetooth device address of remote device
     */
    UCHAR       * bd_addr;

    /**
     * Maximum receive size in bytes
     */
    UINT16      max_recv_size;

    /**
     * The server channel for the FTP OBEX Connection over RFCOMM
     */
    UINT8       server_channel;

#ifdef FTP_1_3
    /**
     * The L2CAP PSM for the FTP OBEX Connection over L2CAP
     */
    UINT16      psm;
#endif /* FTP_1_3 */
}FTP_CONNECT_STRUCT;

/**
 *  \brief Data type to specify the FTP request parameters.
 *
 *  \par Description:
 *       This structure contains the request parameters filled by the application or
 *       the profile depending on the role.
 *
 *       At FTP client, this structure shall be filled by the application when a request
 *       has to be sent to the peer side and at the FTP server, this will be filled by
 *       the profile to notify the application of the request received.
 *
 *  \note
 *       Typically, obj_size is set to size of file during file push operation,
 *       otherwise set this parameter to Zero.
 */
typedef struct _FTP_REQUEST_STRUCT
{
    /**
     * This contains the source file/folder name in the request.
     */
    FTP_HEADER_STRUCT   *name;

    /**
     * This contains the content of file body in the request.
     */
    FTP_HEADER_STRUCT   *body; /* Object content */

    /**
     * This contains the destination file/folder name in the request
     */
    FTP_HEADER_STRUCT   *dest_name;

    /**
     * This indicates the size of the file(object)
     */
    UINT32               obj_size;

    /**
     * This indicates the permissions for file or folder
     */
    UINT32              permissions;

    /**
     * This indicates the direction of the directory switch
     */
    UCHAR               setpath_flag;

    /**
     * This indicates the action on a file or folder
     */
    UCHAR               action_id;

    /**
     * Wait specifier during SRM transaction.
     */
    UCHAR               wait;
} FTP_REQUEST_STRUCT;

/**
 *  \brief Data type to specify the FTP response parameters.
 *
 *  \par Description:
 *       This structure contains the response parameters filled by the application
 *       or the profile depending on the role.
 *
 *       At FTP server, this structure shall be filled by the application to send
 *       the response for the recently received request and at FTP client, this
 *       structure will be filled by the profile to notify the application of the
 *       response indication for the previously sent request.
 *
 *  \note
 *       Typically, obj_size is set to size of file during file pull operation,
 *       otherwise set this parameter to Zero.
 */
typedef struct _FTP_RESPONSE_STRUCT
{
    /**
     * This contains the name of the file/folder in response.
     */
    FTP_HEADER_STRUCT   *name;

    /**
     * This contains the content of the file in response.
     */
    FTP_HEADER_STRUCT   *body;

    /**
     * This indicates the size of the file.
     */
    UINT32               obj_size;

    /**
     * Wait specifier during SRM transaction.
     */
    UCHAR                wait;
} FTP_RESPONSE_STRUCT;

/**
 *  \brief Data type to specify the FTP header information.
 *
 *  \par Description:
 *       Data structure to specify the information related to connect Request,
 *       connect Response, other requests and responses both for the FTP client and server.
 *       Depending upon the request/ response, the corresponding structure in this Union to be used.
 */
typedef union _FTP_HEADERS
{
    /**
     * This parameter is used to send/receive a request.
     */
    FTP_REQUEST_STRUCT   *ftp_req_info;

    /**
     * This parameter is used to send/receive a response.
     */
    FTP_RESPONSE_STRUCT  *ftp_rsp_info;

    /**
     * This parameter is used when the connect request or the
     * connect response is being sent.
     */
    FTP_CONNECT_STRUCT   *ftp_connect_info;
} FTP_HEADERS;
/** \} */
/** \} */

/**
 *  \defgroup ftp_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
/**
 *  \brief FTP event notification callback type
 *
 *  \par Description:
 *       This callback is called by FTP client/server to report various asynchronous
 *       events that has occurred. It can be due to the any request originating from
 *       the peer side or to confirm the result of any request made by the local application.
 *
 *       Purpose:
 *        - To confirm result of an operation initiated by the application.
 *        - To inform application of the asynchronous events.
 *
 *  \param [in] ftp_handle
 *         Valid FTP handle of the FTP session
 *
 *  \param [in] event_type
 *         This parameter defines the FTP server/client event being reported.
 *
 *  \param [in] event_result
 *         This parameter indicates the success or failure of the operation that
 *         is reported through the event type
 *
 *  \param [in] ftp_headers
 *         Depending upon the event_type, this parameter contains one of the following information:
 *          - Connection information
 *          - Request information
 *          - Response information
 *
 *  \param [in] num_headers
 *         This parameter indicates the number of headers passed through the ftp_headers parameter
 */
typedef API_RESULT (* FTP_EVENT_NTF_CB)
                   (
                        /* IN */ FTP_HANDLE      * ftp_handle,
                        /* IN */ UINT8             event_type,
                        /* IN */ UINT16            event_result,
                        /* IN */ FTP_HEADERS     * ftp_headers,
                        /* IN */ UINT16            num_headers
                   );
/** \} */

/* ----------------------------------------- Macros  */
/**
 *  \defgroup ftp_marcos Utility Macros
 *  \{
 *  Initialization and other Utility Macros offered by the module.
 */
/** To initialize a Header Structure */
#define FTP_INIT_HEADER_STRUCT(hs)    \
        (hs).value = NULL;    \
        (hs).length = 0U;
/** \} */


/* ----------------------------------------- Functions */
/**
 * \defgroup  ftp_api API defintions
 * \{
 * Describes API defintiions of FTP Module
 */
#ifdef __cplusplus
extern "C" {
#endif

/* === FTP Client API ==== */
/**
 *  \brief To initialize FTP header structure
 *
 *  \par Description:
 *       This API initializes the FTP client module. This function must be
 *       called (only once) before any other FTP client APIs are used.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note This API can be called again to re-initialize FTP client module,
 *        after a successful call to BT_ftp_client_shutdown() API.
 */
API_RESULT BT_ftp_client_init(void);

/**
 *  \brief To shutdown FTP client module.
 *
 *  \par Description:
 *       This API shuts down the FTP client module.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note This API shall be called only when there is no existing FTP connection
 *        with any remote device. If such a connection exists, it must be closed
 *        by calling BT_ftp_client_disconnect() API.
 */
API_RESULT BT_ftp_client_shutdown(void);

/**
 *  \brief To start an instance of FTP client.
 *
 *  \par Description:
 *       This API starts a FTP client instance. On the successful instantiation,
 *       this API sets and returns the FTP handle. This also registers the
 *       application callback function.
 *
 *  \param [out] ftp_handle
 *         Identifier of the FTP client Entity that is started.
 *
 *  \param [in] ftp_client_cb
 *         The application Callback function pointer to be registered.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_ftp_client_start
           (
               /* OUT */ FTP_HANDLE       * ftp_handle,
               /* IN */  FTP_EVENT_NTF_CB   ftp_client_cb
           );

/**
 *  \brief To stop an active FTP client instance.
 *
 *  \par Description:
 *       This API stops the FTP client instance corresponding to the specified
 *       handle. An instance cannot be stopped when a transaction is going on
 *       or when the connection still exists. This API returns Error
 *       in such cases.
 *
 *  \param [in] ftp_handle
 *         FTP client handle that has to be stopped.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_ftp_client_stop(/* IN */ FTP_HANDLE *ftp_handle);

/**
 *  \brief To establish connection to the FTP server.
 *
 *  \par Description:
 *       This API establishes the OBEX connection with the remote FTP server.
 *       FTP client calls the registered application callback upon
 *       the completion of the connection process (when the FTP server responds
 *       with the connection response) with the notification FTP_CLIENT_CONNECT_CNF.
 *
 *  \param [in] ftp_handle
 *         FTP handle of the client that has to be connected.
 *
 *  \param [in] ftp_connect_info
 *         Structure containing the Bluetooth Device Address of the FTP server
 *         and the local maximum receive capacity (in Bytes),
 *         RFCOMM server channel and L2cap PSM value.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 *         1. Before calling this API, the application needs to ensure that the FTP client
 *            session has been started by calling BT_ftp_client_start().
 *         2. FTP OBEX connection over L2CAP is introduced from FTPv1.2. To have FTP OBEX
 *            connection over L2CAP support, enable the OBEX_OVER_L2CAP feature flag in BT_features.h
 *         3. When OBEX_OVER_L2CAP is enabled:
 *              - For FTP OBEX connection over L2CAP, set the rfcomm server channel value to 0.
 *              - For FTP OBEX connection over RFCOMM, set the l2cap psm value to 0.
 */
API_RESULT BT_ftp_client_connect
           (
               /* IN */ FTP_HANDLE          * ftp_handle,
               /* IN */ FTP_CONNECT_STRUCT  * ftp_connect_info
           );

/**
 *  \brief To disconnect the established FTP connection.
 *
 *  \par Description:
 *       This API is to disconnect the FTP connection with the remote FTP server.
 *       FTP client calls the registered application callback upon completion of
 *       this request with the notification FTP_CLIENT_DISCONNECT_CNF.
 *
 *  \param [in] ftp_handle
 *         FTP handle of the client instance to be disconnected.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 *        FTP client automatically initiates transport disconnection once
 *        the FTP disconnection completes and the application shall be notified
 *        with FTP_CLIENT_DISCONNECT_CNF and FTP_CLIENT_TRANSPORT_CLOSE_CNF for
 *        FTP disconnection and transport disconnection completion respectively.
 */
API_RESULT BT_ftp_client_disconnect(/* IN */ FTP_HANDLE *ftp_handle);

/**
 *  \brief To push a file to the connected FTP server.
 *
 *  \par Description:
 *       This API pushes a file of any format to the already connected FTP
 *       server device. The application should send the contents of the file
 *       in the FTP_REQUEST_STRUCT. If the file size exceeds the negotiated
 *       OBEX packet length, then BT_ftp_client_send_request() can be used to
 *       push remaining part of the file. The application will be notified each
 *       time the FTP server responds to the request. The application can take
 *       further actions depending on the Notification.
 *       The notification that are given to the application for the
 *       push operation is: FTP_CLIENT_PUSH_OBJ_CNF
 *
 *  \param [in] ftp_handle
 *         Valid FTP instance handle.
 *
 *  \param [in] ftp_get_info
 *         This structure contains the name, body and the size of the file.
 *
 *  \param [in] more
 *         It Indicates if application has remaining file still to be sent or
 *         it is the complete file content
 *         1 - File part remaining
 *         0 - No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 *         1. Establish the FTP Connection to a FTP Server through BT_ftp_client_connect() API before
 *            using this API.
 *         2. If the size of file is known in advance then set 'obj_size'(in FTP_REQUEST_STRUCT) to
 *            the size of the file, otherwise set to zero.
 */
API_RESULT BT_ftp_client_push_file
           (
               /* IN */  FTP_HANDLE           *ftp_handle,
               /* IN */  FTP_REQUEST_STRUCT   *ftp_req_info,
               /* IN */  UCHAR                 more,
               /* OUT */ UINT16               *actual
           );

/**
 *  \brief To pull file from the remote device.
 *
 *  \par Description:
 *       This API pulls the specified file from the already connected FTP server
 *       device. The application is notified each time the server responds with
 *       the data through the following notification along with the data:
 *       FTP_CLIENT_PULL_OBJ_CNF
 *
 *  \param [in] ftp_handle
 *         Valid FTP instance handle.
 *
 *  \param [in] ftp_req_info
 *         This structure contains the name of the file to be pulled.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 *         Establish the FTP connection to a FTP server through
 *         BT_ftp_client_connect() API before using this API.
 */
API_RESULT BT_ftp_client_pull_file
           (
               /* IN */ FTP_HANDLE           *ftp_handle,
               /* IN */ FTP_REQUEST_STRUCT   *ftp_req_info
           );

/**
 *  \brief To delete a file/folder in the FTP server.
 *
 *  \par Description:
 *       This function sends a request for file delete operation on the FTP server.
 *       The completion of the delete operation is notified through
 *       FTP_CLIENT_DELETE_OBJ_CNF notification event.
 *
 *  \param [in] ftp_handle
 *         Valid FTP instance handle.
 *
 *  \param [in] ftp_req_info
 *         This structure contains the name of the file/folder to be deleted.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 *        Establish the FTP connection to a FTP server through
 *        BT_ftp_client_connect() API before using this API.
 */
API_RESULT BT_ftp_client_delete_file
           (
               /* IN */ FTP_HANDLE           *ftp_handle,
               /* IN */ FTP_REQUEST_STRUCT   *ftp_req_info
           );

/**
 *  \brief To change the current working directory of FTP server.
 *
 *  \par Description:
 *       This function sends a setpath request to set current directory in FTP server.
 *       This API makes use of OBEX API to send setpath request.
 *       The setpath request can be used for the following operations:
 *        - Create new directory
 *        - Switch forward to a directory
 *        - Switch to the previous directory
 *        - Switch to the root directory
 *
 *       The purpose of set folder request is determined by the contents of ftp_req_info parameter.
 *       The completion of the setpath operation is notified through FTP_CLIENT_SET_FOLDER_CNF
 *       notification event.
 *
 *  \param [in] ftp_handle
 *         Valid FTP instance handle.
 *
 *  \param [in] ftp_req_info
 *         This structure contains the name of the folder to be set as current
 *         directory and setpath flag to indicate set path direction.
 *         The setpath flag has to be one of the following
 *         values(defined in BT_ftp_api.h) corresponding to the request:
 *         * FTP_SET_NEW_FOLDER for create directory
 *         * FTP_SET_CHILD_FOLDER for switch forward to a directory
 *         * FTP_SET_PARENT_FOLDER for switch to its parent directory
 *         * FTP_SET_ROOT_FOLDER for switch to the root directory
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note  Establish the FTP connection to a FTP server through
 *         BT_ftp_client_connect() API before using this API.
 */
API_RESULT BT_ftp_client_set_folder
           (
               /* IN */ FTP_HANDLE           *ftp_handle,
               /* IN */ FTP_REQUEST_STRUCT   *ftp_req_info
           );

/**
 *  \brief To get the file listing from the FTP server.
 *
 *  \par Description:
 *       This function gets the list of files on the server in xml format.
 *       The application shall open a file for the received list to be written
 *       before calling this API and can parse the list after the entire list
 *       is received. The application is notified each time the Server responds
 *       with the data through the following notification along with the data:
 *       FTP_CLIENT_GET_FOLDER_LISTING_CNF
 *
 *  \param [in] ftp_handle
 *         Valid FTP instance handle.
 *
 *  \param [in] ftp_req_info
 *         This structure contains the name of the folder from which list of
 *         files/folders to be retrieved.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the FTP connection to a FTP server through
 *        BT_ftp_client_connect() API before using this API.
 */
API_RESULT BT_ftp_client_get_folder_listing
           (
               /* IN */ FTP_HANDLE           *ftp_handle,
               /* IN */ FTP_REQUEST_STRUCT   *ftp_req_info
           );

#ifdef FTP_1_3
/**
 *  \brief To manipulate a file/folder in the FTP server.
 *
 *  \par Description:
 *       This function sends a request for file/folder manipulate operation on
 *       the FTP server.
 *       The completion of the delete operation is notified through
 *       FTP_CLIENT_ACTION_CNF notification event.
 *
 *  \param [in] ftp_handle
 *         Valid FTP instance handle.
 *
 *  \param [in] ftp_req_info
 *         This structure contains the action_id, source,
 *         destination file/folder names and permissions for the manipulate
 *         operation. action_id flag has to be one of the following values
 *         (Defined in BT_ftp_api.h) corresponding to the request:
 *          - FTP_ACTION_ID_COPY  for copy file/folder
 *          - FTP_ACTION_ID_MOVE  for move/rename file/folder
 *          - FTP_ACTION_ID_SET_PER for set permissions to file/folder
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note Establish the FTP connection to a FTP server through
 *        BT_ftp_client_connect() API before using this API.
 */
API_RESULT BT_ftp_client_manipulate_file
           (
               /* IN */ FTP_HANDLE           *ftp_handle,
               /* IN */ FTP_REQUEST_STRUCT   *ftp_req_info
           );
#endif /* FTP_1_3 */

/**
 *  \brief To abort the ongoing FTP operation (push/pull).
 *
 *  \par Description:
 *       This API aborts the recent operation initiated by the Client.
 *       The completion of abort operation is notified through
 *       FTP_CLIENT_ABORT_CNF notification event.
 *
 *  \param [in] ftp_handle
 *         Valid FTP instance handle.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note  Establish the FTP connection to a FTP server through
 *         BT_ftp_client_connect() API before using this API.
 */
API_RESULT BT_ftp_client_abort
           (
               /* IN */ FTP_HANDLE   *ftp_handle
           );

/**
 *  \brief To send request to the FTP server.
 *
 *  \par Description:
 *       This API sends the request to the FTP server with the event type and
 *       the data given by the application.  The application can call this API
 *       to continue any operation initiated and to which the first notification
 *       has been received. Thus this API shall be called from the FTP application
 *       notification callback function with the event_type set to the received event.
 *
 *
 *  \param [in] ftp_handle
 *         Valid FTP instance handle.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback,
 *         to indicate that the request is the continuation of the operation for
 *         which it was notified.
 *
 *  \param [in] event_result
 *         This is the status of the request.
 *
 *  \param [in] ftp_headers
 *         Structure containing either get request information, in case of get
 *         operation or put request information in case of put operation.
 *
 *  \param [in] more
 *         Indicates if application has remaining of file still to be sent or
 *         file complete
 *         1 - File part remaining
 *         0 - No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header,
 *         if any included in the request, that is packed for transmission.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note  Establish the FTP connection to a FTP server through
 *         BT_ftp_client_connect() API before using this API.
 */
API_RESULT BT_ftp_client_send_request
           (
               /* IN */    FTP_HANDLE   *ftp_handle,
               /* IN */    UINT8         event_type,
               /* IN */    UINT16        event_result,
               /* IN */    FTP_HEADERS  *ftp_headers,
               /* IN */    UCHAR         more,
               /* OUT */   UINT16       *actual
           );

/**
 *  \brief To disconnect the underlying transport connection with FTP server.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection with the remote FTP
 *       server. The FTP client calls the registered application callback upon
 *       completion of this request with notification FTP_CLIENT_TRANSPORT_CLOSE_CNF.
 *
 *  \param [in] ftp_handle
 *         FTP handle of the client instance to be disconnected.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_ftp_client_transport_close
           (
               /* IN */ FTP_HANDLE   *ftp_handle
           );

/* === FTP Server API ==== */
/**
 *  \brief To initialize FTP server module.
 *
 *  \par Description:
 *       This API initializes the FTP server module. This function must be
 *       called (only once) before any other FTP server APIs are used.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note  This API can be called again to re-initialize FTP Server module after
 *         a successful call to BT_ftp_server_shutdown().
 */
API_RESULT BT_ftp_server_init(void);

/**
 *  \brief To shutdown FTP server module.
 *
 *  \par Description:
 *       This API shuts down the FTP server.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note This API shall be called only when there are no active/started
 *        instances of the FTP server. In case of active instances,
 *        they should be stopped by calling BT_ftp_server_stop() API.
 */
API_RESULT BT_ftp_server_shutdown(void);

/**
 *  \brief To start a FTP server instance.
 *
 *  \par Description:
 *       This API starts a FTP server instance. On the successful instantiation,
 *       this API returns a FTP handle. This also registers the application
 *       callback function for the started FTP server instance.
 *
 *  \param [out] ftp_handle
 *         Identifier of the FTP server entity that is started.
 *
 *  \param [in] ftp_server_cb
 *         The application callback function pointer to be registered.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_ftp_server_start
           (
               /* OUT */ FTP_HANDLE       *ftp_handle,
               /* IN */  FTP_EVENT_NTF_CB  ftp_server_cb
           );

/**
 *  \brief To stop an active FTP server instance.
 *
 *  \par Description:
 *       This API stops the FTP server instance corresponding to the specified handle.
 *
 *  \param [in] ftp_handle
 *         FTP Handle of the server instance that has to be stopped.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note In case an active connection exists in the specified instance to be
 *        stopped, the transport for the connection is closed and the instance
 *        is stopped.
 *
 */
API_RESULT BT_ftp_server_stop(/* IN */ FTP_HANDLE *ftp_handle);

/**
 *  \brief To send response to the latest request received from the FTP client.
 *
 *  \par Description:
 *       This API is used to send the response to the requests made by the FTP
 *       client. Response to Connect, PUSH, PULL, Get folder listing etc. shall
 *       be sent through this API.
 *       The application shall call this API with the event type of the latest
 *       notification received. This indicates the response is for the latest
 *       indication received.
 *       FTP server shall frame the OBEX specific packets based on the response
 *       and associated response_data and send it to the FTP client.
 *
 *  \param [in] ftp_handle
 *         Valid FTP handle of the FTP session.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback.
 *         This is to indicate that the response being sent is response to the
 *         request for which the application was notified.
 *
 *  \param [in] event_result
 *         This is the OBEX Response Code.
 *
 *  \param [in] ftp_headers
 *         Structure containing either the connect information in case of
 *         Connect operation or the response data in case of other operation.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note In the first response to the file PULL request, if the size of file is
 *        known in advance then set 'obj_size'(in FTP_RESPONSE_STRUCT) to the
 *        size of the file, otherwise set to zero.
 */
API_RESULT BT_ftp_server_send_response
           (
               /* IN */  FTP_HANDLE    * ftp_handle,
               /* IN */  UINT8           event_type,
               /* IN */  UCHAR           event_result,
               /* IN */  FTP_HEADERS   * ftp_headers
           );

/**
 *  \brief To disconnect the underlying transport connection with FTP client.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection with the remote FTP
 *       client. The FTP server calls the registered application callback upon
 *       completion of this request with notification FTP_SERVER_TRANSPORT_CLOSE_CNF.
 *
 *  \param [in] ftp_handle
 *         Valid FTP handle of the server instance.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_ftp_server_transport_close
           (
               /* IN */ FTP_HANDLE   *ftp_handle
           );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_FTP_API_ */

