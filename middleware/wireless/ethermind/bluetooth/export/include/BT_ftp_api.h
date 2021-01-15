
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

#ifdef FTP_1_3
/* OBEX L2CAP PSM for local client entities */
#define FTP_OBEX_L2CAP_PSM    0x8201
#endif /* FTP_1_3 */

/**
 *
 * FTP Client Operations
 *
 *  1. Set folders paths -> forward, backward, root => OBEX SETPAH
 *  2. Get folder contents -> Folder Listing.       => OBEX GET
 *  3. Push Objects   -> can be either file/folders => OBEX PUT
 *  4. Pull Objects   -> can be either file/folders => OBEX GET
 *  5. Delete Objects -> can be files/folders       => OBEX PUT
 *  6. Rename/Move/Copy/Set permissions -> Using OBEX action opertions => OBEX ACTION
 *
 */

/* #### FTP Client Events #### */
/* For Connection */
#define FTP_CLIENT_CONNECT_CNF                   0x01
#define FTP_CLIENT_DISCONNECT_CNF                0x02
#define FTP_CLIENT_TRANSPORT_CLOSE_CNF           0x03
#define FTP_CLIENT_TRANSPORT_CLOSE_IND           0x04

/* For Folder Browsing */
#define FTP_CLIENT_SET_FOLDER_CNF                0x05
#define FTP_CLIENT_GET_FOLDER_LISTING_CNF        0x06

/* For Object Transfer */
#define FTP_CLIENT_PUSH_OBJ_CNF                  0x07
#define FTP_CLIENT_PULL_OBJ_CNF                  0x08

/* For Object Manipulation */
#define FTP_CLIENT_DELETE_OBJ_CNF                0x09
#define FTP_CLIENT_ACTION_CNF                    0x0A
#define FTP_CLIENT_ABORT_CNF                     0x0B

/* #### FTP Server Events #### */
/* For Connection */
#define FTP_SERVER_CONNECT_IND                   0x01
#define FTP_SERVER_DISCONNECT_IND                0x02
#define FTP_SERVER_TRANSPORT_CLOSE_CNF           0x03
#define FTP_SERVER_TRANSPORT_CLOSE_IND           0x04

/* For Folder Browsing */
#define FTP_SERVER_SET_FOLDER_IND                0x05
#define FTP_SERVER_GET_FOLDER_LISTING_IND        0x06

/* For File Transfer */
#define FTP_SERVER_PUSH_FILE_IND                 0x07
#define FTP_SERVER_PULL_FILE_IND                 0x08

/* For Object Manipulation */
#define FTP_SERVER_DELETE_FILE_IND               0x09
#define FTP_SERVER_ACTION_IND                    0x0A
#define FTP_SERVER_ABORT_IND                     0x0B

#define FTP_EVENT_INVALID                        0xFF
#define FTP_ENTITY_INVALID                       0xFF

/* FTP OBEX Response Codes */
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

/* FTP SetPath parameter values */
#define FTP_SET_NEW_FOLDER                       0x00
#define FTP_SET_CHILD_FOLDER                     0x01
#define FTP_SET_PARENT_FOLDER                    0x02
#define FTP_SET_ROOT_FOLDER                      0x03

#ifdef OBEX_ENH_SUPPORT
/* FTP Action Identifiers */
#define  FTP_ACTION_ID_COPY                     0x00 /* OBEX_ACTION_COPY_ID */
#define  FTP_ACTION_ID_MOVE                     0x01 /* OBEX_ACTION_MOVE_ID */
#define  FTP_ACTION_ID_SET_PER                  0x02 /* OBEX_ACTION_SET_OBJ_PER_ID */
#endif /* OBEX_ENH_SUPPORT */

/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Structures/Data Types */

typedef UCHAR FTP_HANDLE;

/* FTP Header structure, to exchange data with application */
typedef struct _FTP_HEADER_STRUCT
{
    UCHAR   * value;
    UINT16    length;
} FTP_HEADER_STRUCT;


/* FTP Connection Structure */
typedef struct _FTP_CONNECT_STRUCT
{
    UCHAR       * bd_addr;
    UINT16      max_recv_size; /* Maximum Receive Size */
    UINT8       server_channel;

#ifdef FTP_1_3
    UINT16      psm;
#endif /* FTP_1_3 */
}FTP_CONNECT_STRUCT;

/* FTP Request Structure */
typedef struct _FTP_REQUEST_STRUCT
{
    FTP_HEADER_STRUCT   *name;
    FTP_HEADER_STRUCT   *body; /* Object content */
    FTP_HEADER_STRUCT   *dest_name;
    UINT32               obj_size; /* Size of the object */
    UINT32              permissions; /* Permissions to file */
    UCHAR               setpath_flag; /* Set path flag */
    UCHAR               action_id; /* ACTION on file */
    UCHAR               wait;
} FTP_REQUEST_STRUCT;

/* FTP Response Structure */
typedef struct _FTP_RESPONSE_STRUCT
{
    FTP_HEADER_STRUCT   *name;
    FTP_HEADER_STRUCT   *body; /* Message contents */
    UINT32               obj_size; /* Size of the object */
    UCHAR                wait;
} FTP_RESPONSE_STRUCT;

/* FTP common headers */
typedef union _FTP_HEADERS
{
    FTP_REQUEST_STRUCT   *ftp_req_info;     /* Used to send the request */
    FTP_RESPONSE_STRUCT  *ftp_rsp_info;     /* Used to send the response */
    FTP_CONNECT_STRUCT   *ftp_connect_info; /* Used for Connect request */
} FTP_HEADERS;

/* FTP Event Notification Callback type */
typedef API_RESULT (* FTP_EVENT_NTF_CB)
                   (
                        /* IN */ FTP_HANDLE      * ftp_handle,
                        /* IN */ UINT8             event_type,
                        /* IN */ UINT16            event_result,
                        /* IN */ FTP_HEADERS     * ftp_headers,
                        /* IN */ UINT16            num_headers
                   );

/* ----------------------------------------- Macros  */
/* To initialize a Header Structure */
#define FTP_INIT_HEADER_STRUCT(hs)    \
        (hs).value = NULL;    \
        (hs).length = 0;


/* ----------------------------------------- Functions */
#ifdef __cplusplus
extern "C" {
#endif

/* === FTP Client API ==== */
API_RESULT BT_ftp_client_init(void);

API_RESULT BT_ftp_client_shutdown(void);

API_RESULT BT_ftp_client_start
           (
               /* OUT */ FTP_HANDLE       * ftp_handle,
               /* IN */  FTP_EVENT_NTF_CB   ftp_client_cb
           );

API_RESULT BT_ftp_client_stop(/* IN */ FTP_HANDLE *ftp_handle);

API_RESULT BT_ftp_client_connect
           (
               /* IN */ FTP_HANDLE          * ftp_handle,
               /* IN */ FTP_CONNECT_STRUCT  * ftp_connect_info
           );

API_RESULT BT_ftp_client_disconnect(/* IN */ FTP_HANDLE *ftp_handle);

API_RESULT BT_ftp_client_push_file
           (
               /* IN */  FTP_HANDLE           *ftp_handle,
               /* IN */  FTP_REQUEST_STRUCT   *ftp_req_info,
               /* IN */  UCHAR                 more,
               /* OUT */ UINT16               *actual
           );

API_RESULT BT_ftp_client_pull_file
           (
               /* IN */ FTP_HANDLE           *ftp_handle,
               /* IN */ FTP_REQUEST_STRUCT   *ftp_req_info
           );

API_RESULT BT_ftp_client_delete_file
           (
               /* IN */ FTP_HANDLE           *ftp_handle,
               /* IN */ FTP_REQUEST_STRUCT   *ftp_req_info
           );

API_RESULT BT_ftp_client_set_folder
           (
               /* IN */ FTP_HANDLE           *ftp_handle,
               /* IN */ FTP_REQUEST_STRUCT   *ftp_req_info
           );

API_RESULT BT_ftp_client_get_folder_listing
           (
               /* IN */ FTP_HANDLE           *ftp_handle,
               /* IN */ FTP_REQUEST_STRUCT   *ftp_req_info
           );

#ifdef FTP_1_3
API_RESULT BT_ftp_client_manipulate_file
           (
               /* IN */ FTP_HANDLE           *ftp_handle,
               /* IN */ FTP_REQUEST_STRUCT   *ftp_req_info
           );
#endif /* FTP_1_3 */

API_RESULT BT_ftp_client_abort
           (
               /* IN */ FTP_HANDLE   *ftp_handle
           );

API_RESULT BT_ftp_client_send_request
           (
               /* IN */    FTP_HANDLE   *ftp_handle,
               /* IN */    UINT8         event_type,
               /* IN */    UINT16        event_result,
               /* IN */    FTP_HEADERS  *ftp_headers,
               /* IN */    UCHAR         more,
               /* INOUT */ UINT16       *actual
           );

API_RESULT BT_ftp_client_transport_close
           (
               /* IN */ FTP_HANDLE   *ftp_handle
           );

/* === FTP Server API ==== */

API_RESULT BT_ftp_server_init(void);

API_RESULT BT_ftp_server_shutdown(void);

API_RESULT BT_ftp_server_start
           (
               /* OUT */ FTP_HANDLE       *ftp_handle,
               /* IN */  FTP_EVENT_NTF_CB  ftp_server_cb
           );

API_RESULT BT_ftp_server_stop(/* IN */ FTP_HANDLE *ftp_handle);

API_RESULT BT_ftp_server_send_response
           (
               /* IN */  FTP_HANDLE    * ftp_handle,
               /* IN */  UINT8           event_type,
               /* IN */  UCHAR           event_result,
               /* IN */  FTP_HEADERS   * ftp_headers
           );

API_RESULT BT_ftp_server_transport_close
           (
               /* IN */ FTP_HANDLE   *ftp_handle
           );

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_FTP_API_ */

