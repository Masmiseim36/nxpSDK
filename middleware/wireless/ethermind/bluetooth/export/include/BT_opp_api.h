
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

#ifdef OPP_1_2
/* OBEX L2CAP PSM for local client entities */
#define OPP_OBEX_L2CAP_PSM    0x8401
#endif /* OPP_1_2 */

/**
 *
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
#define OPP_OP_NONE                                 0x01
#define OPP_OP_PUSH_OBJECT                          0x02
#define OPP_OP_PULL_BUSINESS_CARD                   0x03
#define OPP_OP_EXCH_BUSINESS_CARD                   0x04

#define OPP_TYPE_HDR_VCARD                          "text/x-vcard"


/* #### OPP Client Events #### */
/* For Connection */
#define OPP_CLIENT_CONNECT_CNF                   0x01
#define OPP_CLIENT_DISCONNECT_CNF                0x02
#define OPP_CLIENT_TRANSPORT_CLOSE_CNF           0x03
#define OPP_CLIENT_TRANSPORT_CLOSE_IND           0x04
#define OPP_CLIENT_PUSH_OBJECT_CNF               0x05
#define OPP_CLIENT_PULL_BUSINESS_CARD_CNF        0x06
#define OPP_CLIENT_EXCH_BUSINESS_CARD_CNF        0x07
#define OPP_CLIENT_ABORT_CNF                     0x08

/* #### OPP Server Events #### */
/* For Connection */
#define OPP_SERVER_CONNECT_IND                   0x01
#define OPP_SERVER_DISCONNECT_IND                0x02
#define OPP_SERVER_TRANSPORT_CLOSE_CNF           0x03
#define OPP_SERVER_TRANSPORT_CLOSE_IND           0x04
#define OPP_SERVER_PUSH_OBJECT_IND               0x05
#define OPP_SERVER_PULL_BUSINESS_CARD_IND        0x06
#define OPP_SERVER_ABORT_IND                     0x07

#define OPP_EVENT_INVALID                        0xFF
#define OPP_ENTITY_INVALID                       0xFF

/* OPP OBEX Response Codes */
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


/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Structures/Data Types */

/* OPP Handle */
typedef UCHAR OPP_HANDLE;

/* OPP Header structure to exchange data with application */
typedef struct _OPP_HEADER_STRUCT
{
    UCHAR   * value;
    UINT16    length;
} OPP_HEADER_STRUCT;


/* OPP Connection Structure */
typedef struct _OPP_CONNECT_STRUCT
{
    UCHAR       * bd_addr;
    UINT16      max_rx_size; /* Maximum Receive Size */
    UINT16      max_tx_size; /* Max. Transmit size */
    UINT8       server_channel; /* rfcomm channel */
#ifdef OPP_1_2
    UINT16      psm;         /* L2CAP PSM */
#endif /* OPP_1_2 */

}OPP_CONNECT_STRUCT;

/* OPP Request Structure */
typedef struct _OPP_REQUEST_STRUCT
{
    OPP_HEADER_STRUCT   *name;
    OPP_HEADER_STRUCT   *type;
    OPP_HEADER_STRUCT   *body;
    UINT32              obj_len_info; /* Object size to be sent in PUT request */
    UCHAR               wait;
} OPP_REQUEST_STRUCT;

/* OPP Response Structure */
typedef struct _OPP_RESPONSE_STRUCT
{
    OPP_HEADER_STRUCT   *name;
    OPP_HEADER_STRUCT   *body; /* Object contents */
    UCHAR               wait;
} OPP_RESPONSE_STRUCT;

/* OPP common headers */
typedef union _OPP_HEADERS
{
    OPP_REQUEST_STRUCT   *opp_req_info;     /* Used to send the request */
    OPP_RESPONSE_STRUCT  *opp_rsp_info;     /* Used to send the response */
    OPP_CONNECT_STRUCT   *opp_connect_info; /* Used for Connect request */
} OPP_HEADERS;

/* OPP Event Notification Callback type */
typedef API_RESULT (* OPP_EVENT_NTF_CB)
                   (
                       /* IN */ OPP_HANDLE      * opp_handle,
                       /* IN */ UINT8             event_type,
                       /* IN */ UINT16            event_result,
                       /* IN */ OPP_HEADERS     * opp_headers,
                       /* IN */ UINT16            num_headers
                   );

/* ----------------------------------------- Macros  */
/* To initialize a Header Structure */
#define OPP_INIT_HEADER_STRUCT(hs)    \
        (hs).value = NULL;    \
        (hs).length = 0;


/* ----------------------------------------- Functions */
#ifdef __cplusplus
extern "C" {
#endif

/* === OPP Client API's ==== */

API_RESULT BT_opp_client_init(void);

API_RESULT BT_opp_client_shutdown(void);

API_RESULT BT_opp_client_start
           (
               /* OUT */ OPP_HANDLE       *opp_handle,
               /* IN */  OPP_EVENT_NTF_CB  opp_client_cb
           );

API_RESULT BT_opp_client_stop(/* IN */ OPP_HANDLE *opp_handle);

API_RESULT BT_opp_client_connect
           (
               /* IN */ OPP_HANDLE          * opp_handle,
               /* IN */ OPP_CONNECT_STRUCT  * opp_connect_info
           );

API_RESULT BT_opp_client_disconnect(/* IN */ OPP_HANDLE *opp_handle);

API_RESULT opp_client_push_object
           (
                /* IN */  OPP_HANDLE           *opp_handle,
                /* IN */  OPP_REQUEST_STRUCT   *opp_req_info,
                /* IN */  UCHAR                 opp_operation,
                /* IN */  UCHAR                 more,
                /* OUT */ UINT16               *actual
           );

API_RESULT BT_opp_client_pull_business_card
           (
               /* IN */ OPP_HANDLE           *opp_handle,
               /* IN */ OPP_REQUEST_STRUCT   *opp_req_info
           );

#define BT_opp_client_push_object(opp_handle, opp_req_info, more, actual)\
      opp_client_push_object ((opp_handle), (opp_req_info), OPP_OP_PUSH_OBJECT, (more), (actual))

#define BT_opp_client_exchange_business_card(opp_handle, opp_req_info, more, actual)\
      opp_client_push_object ((opp_handle), (opp_req_info), OPP_OP_EXCH_BUSINESS_CARD, (more), (actual))

API_RESULT BT_opp_client_send_request
           (
               /* IN */ OPP_HANDLE   *opp_handle,
               /* IN */ UINT8         event_type,
               /* IN */ UINT16        event_result,
               /* IN */ OPP_HEADERS  *opp_headers,
               /* IN */ UCHAR         more,
               /* OUT */ UINT16      *actual
           );

API_RESULT BT_opp_client_abort
           (
               /* IN */ OPP_HANDLE  *opp_handle
           );

API_RESULT BT_opp_client_transport_close
           (
               /* IN */ OPP_HANDLE  *opp_handle
           );

/* === OPP Server API's ==== */

API_RESULT BT_opp_server_init(void);

API_RESULT BT_opp_server_shutdown(void);

API_RESULT BT_opp_server_start
           (
               /* OUT */ OPP_HANDLE  *opp_handle,
               /* IN */  OPP_EVENT_NTF_CB opp_client_cb
           );

API_RESULT BT_opp_server_stop(/* IN */ OPP_HANDLE *opp_handle);

API_RESULT BT_opp_server_send_response
           (
               /* IN */ OPP_HANDLE      *opp_handle,
               /* IN */ UINT8           event_type,
               /* IN */ UCHAR           event_result,
               /* IN */ OPP_HEADERS     *opp_headers
           );

API_RESULT BT_opp_server_transport_close
           (
               /* IN */ OPP_HANDLE  *opp_handle
           );

#ifdef __cplusplus
};
#endif

#endif  /* _H_BT_OPP_API_ */

