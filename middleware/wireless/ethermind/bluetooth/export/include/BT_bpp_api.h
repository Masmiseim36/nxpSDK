
/**
 *  \file BT_bpp_api.h
 *
 *  \brief EtherMind BPP API Header File.
 *
 *  This Header File contains declaration of BPP APIs, Macros and
 *  Constant Definitions for use in applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_BPP_API_
#define _H_BT_BPP_API_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_obex_api.h"

/* ----------------------------------------- Exported Global Variables */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup  bpp_module  BPP (Basic Printing Profile)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Basic Printing Profile module to the Application.
 */
/**
 *
 * BPP  Operations
 *
 *  1. File Push                                    => OBEX PUT
 *  2. Simple Reference Push                        => OBEX PUT
 *  3. Create Job                                   => OBEX GET
 *  4. Send Document                                => OBEX PUT
 *  5. Get Job Attributes                           => OBEX GET
 *  6. Get Printer Attributes                       => OBEX GET
 *  7. Cancel Job                                   => OBEX GET
 *  8. Get Event                                    => OBEX GET
 *  9. Get Referenced Object                        => OBEX GET
 * 10. Send Reference                               => OBEX PUT
 * 11. Create Precise Job                           => OBEX GET
 * 12. Get Margins                                  => OBEX GET
 * 13. Get RUI                                      => OBEX GET
 *
 */

/**
 * \defgroup bpp_defines Defines
 * \{
 * Describes defines for the module.
 */
/**
 * \defgroup bpp_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/* #### BPP Sender Events #### */
/**
 *  \defgroup  BPP_SENDER_EVENTS BPP Sender Events
 *  \{
 *
 *  Constant Defines for BPP Sender Events
 */
/* For Connection */
/**
 *  \name BPP Sender Events for Connection
 *
 *  Constant Defines for Connection belonging to \ref BPP_SENDER_EVENTS
 *
 */
/*@{*/
#define BPP_SENDER_JOBCH_CONNECT_CNF                    0x01
#define BPP_SENDER_JOBCH_DISCONNECT_CNF                 0x02
#define BPP_SENDER_JOBCH_TRANSPORT_CLOSE_CNF            0x03
#define BPP_SENDER_JOBCH_TRANSPORT_CLOSE_IND            0x04

#define BPP_SENDER_STATUSCH_CONNECT_CNF                 0x05
#define BPP_SENDER_STATUSCH_DISCONNECT_CNF              0x06
#define BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_CNF         0x07
#define BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_IND         0x08

#define BPP_SENDER_OBJECTCH_CONNECT_IND                 0x09
#define BPP_SENDER_OBJECTCH_DISCONNECT_IND              0x0A
#define BPP_SENDER_OBJECTCH_TRANSPORT_CLOSE_CNF         0x0B
#define BPP_SENDER_OBJECTCH_TRANSPORT_CLOSE_IND         0x0C
/*@}*/
/* Simple Push transfer Event Types */
/**
 *  \name Simple Push transfer Event Types
 *
 *  Constant Defines for Simple Push transfer Event Types belonging to \ref BPP_SENDER_EVENTS
 *
 */
/*@{*/
#define BPP_SENDER_FILE_PUSH_CNF                        0x0D
#define BPP_SENDER_SIMPLE_REF_PUSH_CNF                  0x0E
/*@}*/
/* Job based transfer Event Types */
/**
 *  \name Job based transfer Event Types
 *
 *  Constant Defines for Job based transfer Event Types belonging to \ref BPP_SENDER_EVENTS
 */
/*@{*/
#define BPP_SENDER_CREATE_JOB_CNF                       0x0F
#define BPP_SENDER_GET_PRINTER_ATTRS_CNF                0x10
#define BPP_SENDER_GET_JOB_ATTRS_CNF                    0x11
#define BPP_SENDER_CANCEL_JOB_CNF                       0x12
#define BPP_SENDER_CREATE_PR_JOB_CNF                    0x13
#define BPP_SENDER_GET_MARGINS_CNF                      0x14
#define BPP_SENDER_GET_EVENT_CNF                        0x15
#define BPP_SENDER_SEND_DOC_CNF                         0x16
#define BPP_SENDER_SEND_REF_CNF                         0x17
#define BPP_SENDER_GET_REF_OBJ_IND                      0x18
#define BPP_SENDER_GET_RUI_CNF                          0x19
/*@}*/
/**\}*/

/* #### BPP Printer Events #### */
/**
 *  \defgroup  BPP_PRINTER_EVENTS BPP Printer Events
 *  \{
 *
 *  Constant Defines for BPP Printer Events
 */
/* For Connection */
/**
 *  \name BPP Printer Events for Connection
 *
 *  Constant Defines for Connection belonging to \ref BPP_PRINTER_EVENTS
 */
/*@{*/
#define BPP_PRINTER_JOBCH_CONNECT_IND                   0x61
#define BPP_PRINTER_JOBCH_DISCONNECT_IND                0x62
#define BPP_PRINTER_JOBCH_TRANSPORT_CLOSE_CNF           0x63
#define BPP_PRINTER_JOBCH_TRANSPORT_CLOSE_IND           0x64

#define BPP_PRINTER_STATUSCH_CONNECT_IND                0x65
#define BPP_PRINTER_STATUSCH_DISCONNECT_IND             0x66
#define BPP_PRINTER_STATUSCH_TRANSPORT_CLOSE_CNF        0x67
#define BPP_PRINTER_STATUSCH_TRANSPORT_CLOSE_IND        0x68

#define BPP_PRINTER_OBJECTCH_CONNECT_CNF                0x69
#define BPP_PRINTER_OBJECTCH_DISCONNECT_CNF             0x6A
#define BPP_PRINTER_OBJECTCH_TRANSPORT_CLOSE_CNF        0x6B
#define BPP_PRINTER_OBJECTCH_TRANSPORT_CLOSE_IND        0x6C
/*@}*/
/* Simple Push transfer Event Types */
/**
 *  \name BPP Printer Events for Simple Push transfer Event Types
 *
 *  Constant Defines for Simple Push transfer Event Types belonging to \ref BPP_PRINTER_EVENTS
 */
/*@{*/
#define BPP_PRINTER_FILE_PUSH_IND                       0x6D
#define BPP_PRINTER_SIMPLE_REF_PUSH_IND                 0x6E
/*@}*/
/* Job based transfer Event Types */
/**
 *  \name BPP Printer Events for Job based transfer Event Types
 *
 *  Constant Defines for Job based transfer Event Types belonging to \ref BPP_PRINTER_EVENTS
 */
/*@{*/
#define BPP_PRINTER_CREATE_JOB_IND                      0X6F
#define BPP_PRINTER_GET_PRINTER_ATTRS_IND               0X70
#define BPP_PRINTER_GET_JOB_ATTRS_IND                   0X71
#define BPP_PRINTER_CANCEL_JOB_IND                      0X72
#define BPP_PRINTER_CREATE_PR_JOB_IND                   0X73
#define BPP_PRINTER_GET_MARGINS_IND                     0X74
#define BPP_PRINTER_GET_EVENT_IND                       0X75
#define BPP_PRINTER_SEND_DOC_IND                        0X76
#define BPP_PRINTER_SEND_REF_IND                        0X77
#define BPP_PRINTER_GET_REF_OBJ_CNF                     0X78
#define BPP_PRINTER_GET_RUI_IND                         0X79
/*@}*/
/**\}*/
#define BPP_EVENT_INVALID                               0xFF
#define BPP_ENTITY_INVALID                              0xFF

/** Maximum OBEX PIN SIZE supported */
#define BPP_OBEX_MAX_PIN_SIZE                           0x10 /* 16 */

/** Authentication Challenge/ Response Size */
#define BPP_OBEX_AUTH_CHALLENGE_RESP_SIZE               0x10 /* 16 */

/* BPP Target/Who Header Size */
#define BPP_TARGET_HDR_SIZE                             0x10 /* 16 */

/* BPP OBEX Response Codes */
/**
 *  \name BPP OBEX Response Codes
 *
 *  Constant Defines for BPP OBEX Response Codes
 *
 */
/*@{*/
#define BPP_SUCCESS_RSP                                 OBEX_SUCCESS_RSP
#define BPP_CONTINUE_RSP                                OBEX_CONTINUE_RSP

#define BPP_BAD_REQ_RSP                                 OBEX_REQ_IS_BAD_RSP
#define BPP_NOT_IMPLEMENTED_RSP                         OBEX_NOT_IMP_RSP
#define BPP_UNAUTH_RSP                                  OBEX_UNAUTHORIZED_RSP
#define BPP_PRECOND_FAILED_RSP                          OBEX_PRECOND_FAIL_RSP
#define BPP_NOT_FOUND_RSP                               OBEX_NOT_FOUND_RSP
#define BPP_NOT_ACCEPTABLE_RSP                          OBEX_NOT_ACCEPTABLE_RSP
#define BPP_NO_SERVICE_RSP                              OBEX_NO_SERVICE_RSP
#define BPP_FORBIDDEN_RSP                               OBEX_FORBIDDEN_RSP
#define BPP_PRINTER_ERROR                               OBEX_SERVER_ERR_RSP
#define BPP_NOT_MODIFIED_RSP                            OBEX_NOT_MODIFIED_RSP
#define BPP_DATABASE_FULL_RSP                           OBEX_DATABASE_FULL_RSP
#define BPP_CONFLICT_RSP                                OBEX_CONFLICT_RSP
#define BPP_UNSUPP_MEDIA_RSP                            OBEX_UNSUPP_MEDIA_RSP
/*@}*/
/* Application parameters tag ids */
/**
 *  \name Application parameters tag ids
 *
 *  Constant Defines for Application parameters tag ids
 *
 */
/*@{*/
#define BPP_TAG_ID_OFFSET                               0x01
#define BPP_TAG_ID_COUNT                                0x02
#define BPP_TAG_ID_JOB_ID                               0x03
#define BPP_TAG_ID_FILE_SIZE                            0x04
/*@}*/
/* No. of application headers */
#define  BPP_NUM_APPL_HEADERS                           0x04

/* BPP Application parameters flag corresponding to each Tag */
/**
 *  \name BPP Application parameters flag corresponding to each Tag
 *
 *  Constant Defines for BPP Application parameters flag corresponding to each Tag
 *
 */
/*@{*/
#define BPP_FLAG_OFFSET                                 0x00000001
#define BPP_FLAG_COUNT                                  0x00000002
#define BPP_FLAG_JOB_ID                                 0x00000004
#define BPP_FLAG_FILE_SIZE                              0x00000008
/*@}*/

/* BPP Channel Types */
/**
 *  \name BPP Channel Types
 *
 *  Constant Defines for BPP Channel Types
 */
/*@{*/
#define BPP_JOB_CHANNEL                                 0x00
#define BPP_STATUS_CHANNEL                              0x01
#define BPP_OBJECT_CHANNEL                              0x02
#define BPP_RUI_CHANNEL                                 0x03
/*@}*/
/* BPP Operations */
/**
 *  \name BPP Operations
 *
 *  Constant Defines for BPP Operations
 */
/*@{*/
#define BPP_SIMPLE_PUSH_OPERATION                       0x00
#define BPP_SIMPLE_REF_PUSH_OPERATION                   0x01
#define BPP_CREATE_JOB_OPERATION                        0x02
#define BPP_GET_PRINTER_ATTRS_OPERATION                 0x03
#define BPP_GET_JOB_ATTRS_OPERATION                     0x04
#define BPP_CANCEL_JOB_OPERATION                        0x05
#define BPP_CREATE_PRECISE_JOB_OPERATION                0x06
#define BPP_GET_MARGINS_OPERATION                       0x07
#define BPP_GET_EVENT_OPERATION                         0x08
#define BPP_SEND_DOCUMENT_OPERATION                     0x09
#define BPP_SEND_REFERENCE_OPERATION                    0x0A
#define BPP_GET_REF_OBJ_OPERATION                       0x0B
/*@}*/
/**\}*/
/**\}*/

/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Structures/Data Types */
/**
 *  \addtogroup bpp_defines Defines
 *  \{
 */
/**
 * \defgroup bpp_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
typedef UCHAR BPP_HANDLE;

/** BPP Header structure, to exchange data with application */
typedef struct _BPP_HEADER_STRUCT
{
    UCHAR   * value;

    UINT16    length;
} BPP_HEADER_STRUCT;

/** BPP Connection Structure */
typedef struct _BPP_CONNECT_STRUCT
{
    /**
     * OBEX Pin Code
     */
    BPP_HEADER_STRUCT  * pin_info;

    /**
     * Corresponding User ID
     */
    BPP_HEADER_STRUCT  * user_id;

    /**
     * Bluetooth address
     */
    UCHAR       * bd_addr;

    /**
     * Max. Receive size
     */
    UINT16      max_recv_size;

    /**
     * RFCOMM Server channel
     */
    UINT8       server_channel;

    /**
     * Authentication required or not
     */
    UINT8       auth_flag;

}BPP_CONNECT_STRUCT;


typedef struct _BPP_APPL_PARAMS
{
    /**
     * 32 Bit appl_param_flag [10 Least Significant Bits used]
     *
     * Usage:
     *
     * BIT 0 - offset
     *     1 - count
     *     2 - job_id
     *     3 - file_size
    */
    UINT32                 appl_param_flag;

    UINT32                 offset;

    INT32                  count;

    UINT32                 job_id;

    INT32                  file_size;

} BPP_APPL_PARAMS;

/** BPP Request Structure */
typedef struct _BPP_REQUEST_STRUCT
{
    /** Name */
    BPP_HEADER_STRUCT   *name;

    /** BPP object content */
    BPP_HEADER_STRUCT   *body;

    /** Image Descriptor */
    BPP_HEADER_STRUCT   *description;

    /** Image Handle */
    BPP_HEADER_STRUCT   *type_header;

    /** Application parameters */
    BPP_APPL_PARAMS     * appl_params;

    /** HTTP ACCEPT */
    BPP_HEADER_STRUCT   *http_accept_header;

    /** HTTP ACCEPT LANGUAGE */
    BPP_HEADER_STRUCT   *http_accept_lang_header;

    /** HTTP Authorization */
    BPP_HEADER_STRUCT   *http_authorization_header;

} BPP_REQUEST_STRUCT;

/** BPP Response Structure */
typedef struct _BPP_RESPONSE_STRUCT
{
    /** Image Name */
    BPP_HEADER_STRUCT       *name;

    /** BPP Object content */
    BPP_HEADER_STRUCT       *body;

    /** Image Length */
    UINT32                  img_len_info;

    /** Type Header */
    BPP_HEADER_STRUCT       *type_header;

    /** Description (optional) */
    BPP_HEADER_STRUCT       *description;

    /** Application parameter */
    BPP_APPL_PARAMS         *appl_params;

    /** HTTP ACCEPT */
    BPP_HEADER_STRUCT   *http_accept_header;

    /** HTTP ACCEPT LANGUAGE */
    BPP_HEADER_STRUCT   *http_accept_lang_header;

    /** HTTP Authorization */
    BPP_HEADER_STRUCT   *http_authorization_header;

} BPP_RESPONSE_STRUCT;

/** BPP common headers */
typedef union _BPP_HEADERS
{
    /** Used for Connect request */
    BPP_CONNECT_STRUCT      *bpp_connect_info;

    /** Used to send the request */
    BPP_REQUEST_STRUCT      *bpp_req_info;

    /** Used to send the response */
    BPP_RESPONSE_STRUCT     *bpp_rsp_info;

} BPP_HEADERS;
/** \} */
/** \} */

/**
 *  \defgroup bpp_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
/** BPP Event Notification Callback type */
typedef API_RESULT (* BPP_EVENT_NTF_CB)
                   (
                        /* IN */ BPP_HANDLE      * bpp_handle,
                        /* IN */ UINT8             event_type,
                        /* IN */ UINT16            event_result,
                        /* IN */ BPP_HEADERS     * bpp_headers,
                        /* IN */ UINT16            num_headers,
                        /* IN */ UCHAR             bpp_channel_type
                   );
/** \} */

/* ----------------------------------------- Macros  */
/**
 * \defgroup bpp_utility_macros Utility Macros
 * \{
 * Describes BPP Utility Macros defined by the module.
 */
/** To set application parameter flag */
#define BPP_SET_APPL_PARAM_FLAG(flag, set_field)   \
        (flag) = (flag) | (set_field);

/** To retrieve the field value (Set/Reset) */
#define BPP_GET_APPL_PARAM_FLAG(flag, get_field)   \
        ((flag) & (get_field))

/** Resets all the fields of the application parameter flag */
#define BPP_RESET_APPL_PARAM_FLAG(flag)            \
        (flag) = 0x00000000

/** Extract header structure value */
#define BPP_COPY_HEADER_STRUCT(hdr, len, val)      \
        (len) = (hdr)->length;                     \
        BT_str_n_copy((val), (hdr)->value, (len)); \
        (val)[(len)] = '\0';

/** Extract application parameters */
#define BPP_COPY_APPL_PARAMS(to_params, from_params, search_value_out)      \
        BT_mem_copy((to_params), (from_params), sizeof(BPP_APPL_PARAMS));   \
        if((from_params)->search_value.length != 0 &&                       \
           (from_params)->search_value.value != NULL)                       \
        {                                                                   \
            BT_mem_copy((search_value_out),                                 \
                   (from_params)->search_value.value,                       \
                   (from_params)->search_value.length);                     \
            (to_params)->search_value.value = (search_value_out);           \
            (to_params)->search_value.length =                              \
                                (from_params)->search_value.length;         \
        }

/** To initialize a Header Structure */
#define BPP_INIT_HEADER_STRUCT(hs)    \
        (hs).value = NULL;    \
        (hs).length = 0;

/** \} */
/* ----------------------------------------- Functions */
/**
 * \defgroup bpp_api API Definitions
 * \{
 * Describes BPP API definitions defined by the module.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* === BPP Sender API ==== */
/**
 *  \brief To initialize Sender module.
 *
 *  \par Description:
 *       This API is used to initialize the BPP Sender module. It initializes
 *       global data structures and synchronization primitives used in this module.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_bpp_sender_init(void);
/**
 *  \brief To initialize Printer module.
 *
 *  \par Description:
 *       This API is used to initialize the BPP Printer module. It initializes
 *       global data structures and synchronization primitives used in this module.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_bpp_sender_shutdown(void);
/**
 *  \brief To start an instance of Sender.
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_sender_start
           (
               /* OUT */ BPP_HANDLE       * bpp_handle,
               /* IN */  BPP_EVENT_NTF_CB   bpp_sender_cb,
               /* IN */  UCHAR              bpp_service_type,
               /* IN */  UCHAR              bpp_channel_type
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_sender_stop
           (
                /* IN */ BPP_HANDLE *bpp_handle,
                /* IN */ UCHAR       bpp_channel_type
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_sender_connect
           (
               /* IN */ BPP_HANDLE          * bpp_handle,
               /* IN */ BPP_CONNECT_STRUCT  * bpp_connect_info,
               /* IN */ UCHAR                 bpp_channel_type
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_sender_disconnect
           (
                /* IN */ BPP_HANDLE           *bpp_handle,
                /* IN */ UCHAR                 bpp_channel_type
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_sender_send_request
           (
               /* IN */    BPP_HANDLE          *bpp_handle,
               /* IN */    UINT8               event_type,
               /* IN */    UINT16              event_result,
               /* IN */    BPP_HEADERS         *bpp_headers,
               /* IN */    UCHAR               bpp_channel_type,
               /* IN */    UCHAR               more,
               /* OUT */ UINT16              *actual
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_sender_send_response
           (
               /* IN */  BPP_HANDLE         * bpp_handle,
               /* IN */  UINT8                event_type,
               /* IN */  UCHAR                event_result,
               /* IN */  BPP_HEADERS        * bpp_headers,
               /* IN */  UCHAR                bpp_channel_type
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_sender_transport_close
           (
               /* IN */ BPP_HANDLE           *bpp_handle,
               /* IN */ UCHAR                 bpp_channel_type
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_sender_get_rui
           (
               /* IN */ BPP_HANDLE           *bpp_handle,
               /* IN */ BPP_REQUEST_STRUCT   *bpp_req_info,
               /* IN */ UCHAR                 bpp_channel_type,
               /* IN */ UCHAR                 more,
               /* OUT */ UINT16               *actual
           );

#define BT_bpp_sender_simple_push(bpp_handle, req_info, more, actual)    \
        bpp_sender_put_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_SIMPLE_PUSH_OPERATION),                                 \
            (BPP_JOB_CHANNEL),                                           \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bpp_sender_simple_ref_push(bpp_handle, req_info, more, actual) \
        bpp_sender_put_operation                                          \
        (                                                                 \
            (bpp_handle),                                                 \
            (req_info),                                                   \
            (BPP_SIMPLE_REF_PUSH_OPERATION),                              \
            (BPP_JOB_CHANNEL),                                            \
            (more),                                                       \
            (actual)                                                      \
        )

#define BT_bpp_sender_send_document(bpp_handle, req_info, more, actual)  \
        bpp_sender_put_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_SEND_DOCUMENT_OPERATION),                               \
            (BPP_JOB_CHANNEL),                                           \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bpp_sender_send_reference(bpp_handle, req_info, more, actual) \
        bpp_sender_put_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_SEND_REFERENCE_OPERATION),                              \
            (BPP_JOB_CHANNEL),                                           \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bpp_sender_create_job(bpp_handle, req_info , more , actual)   \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_CREATE_JOB_OPERATION),                                  \
            (BPP_JOB_CHANNEL),                                           \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bpp_sender_get_printer_attrs(bpp_handle, req_info , bpp_channel_type , more , actual)     \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_GET_PRINTER_ATTRS_OPERATION),                           \
            (bpp_channel_type),                                          \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bpp_sender_get_job_attrs(bpp_handle, req_info , bpp_channel_type , more , actual)     \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_GET_JOB_ATTRS_OPERATION),                               \
            (bpp_channel_type),                                          \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bpp_sender_cancel_job(bpp_handle, req_info , bpp_channel_type , more , actual)     \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_CANCEL_JOB_OPERATION),                                  \
            (bpp_channel_type),                                          \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bpp_sender_create_precise_job(bpp_handle, req_info , more , actual)     \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_CREATE_PRECISE_JOB_OPERATION),                          \
            (BPP_JOB_CHANNEL),                                           \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bpp_sender_get_margins(bpp_handle, req_info , more , actual)  \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_GET_MARGINS_OPERATION),                                 \
            (BPP_JOB_CHANNEL),                                           \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bpp_sender_get_event(bpp_handle, req_info , more , actual)     \
        bpp_sender_get_operation                                          \
        (                                                                 \
            (bpp_handle),                                                 \
            (req_info),                                                   \
            (BPP_GET_EVENT_OPERATION),                                    \
            (BPP_STATUS_CHANNEL),                                         \
            (more),                                                       \
            (actual)                                                      \
        )
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT bpp_sender_put_operation
           (
               /* IN */  BPP_HANDLE           *bpp_handle,
               /* IN */  BPP_REQUEST_STRUCT   *bpp_req_info,
               /* IN */  UCHAR                 bpp_operation_id,
               /* IN */  UCHAR                 bpp_channel_type,
               /* IN */  UCHAR                 more,
               /* OUT */ UINT16               *actual
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT bpp_sender_get_operation
           (
               /* IN */ BPP_HANDLE            *bpp_handle,
               /* IN */ BPP_REQUEST_STRUCT    *bpp_req_info,
               /* IN */ UCHAR                  bpp_operation_id,
               /* IN */ UCHAR                  bpp_channel_type,
               /* IN */  UCHAR                 more,
               /* OUT */ UINT16               *actual
           );

/* === BPP Printer API ==== */
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_printer_init(void);
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_printer_shutdown(void);
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_printer_start
           (
               /* OUT */ BPP_HANDLE        *bpp_handle,
               /* IN */  BPP_EVENT_NTF_CB   bpp_printer_cb,
               /* IN */  UCHAR              bpp_service_type,
               /* IN */  UCHAR              bpp_channel_type
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_printer_stop
           (
                /* IN */ BPP_HANDLE *bpp_handle,
                /* IN */ UCHAR       bpp_channel_type
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_printer_connect
           (
               /* IN */ BPP_HANDLE          * bpp_handle,
               /* IN */ BPP_CONNECT_STRUCT  * bpp_connect_info,
               /* IN */ UCHAR                 bpp_channel_type
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_printer_disconnect
           (
                /* IN */ BPP_HANDLE           *bpp_handle,
                /* IN */ UCHAR                 bpp_channel_type
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_printer_send_response
           (
               /* IN */  BPP_HANDLE    * bpp_handle,
               /* IN */  UINT8           event_type,
               /* IN */  UCHAR           event_result,
               /* IN */  BPP_HEADERS   * bpp_headers,
               /* IN */  UCHAR           bpp_channel_type
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_printer_transport_close
           (
               /* IN */ BPP_HANDLE   *bpp_handle,
               /* IN */ UCHAR         bpp_channel_type
           );

#define BT_bpp_printer_get_ref_obj(bpp_handle, req_info )                \
        bpp_printer_get_operation                                        \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_GET_REF_OBJ_OPERATION),                                 \
            (BPP_OBJECT_CHANNEL),                                        \
            (0),                                                         \
            (0)                                                          \
        )
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT bpp_printer_get_operation
           (
               /* IN */ BPP_HANDLE           *bpp_handle,
               /* IN */ BPP_REQUEST_STRUCT   *bpp_req_info,
               /* IN */ UCHAR                 bpp_operation_id,
               /* IN */ UCHAR                 bpp_channel_type,
               /* IN */ UCHAR                 more,
               /* OUT */ UINT16              *actual
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_bpp_printer_send_request
           (
               /* IN */ BPP_HANDLE   *bpp_handle,
               /* IN */ UINT8         event_type,
               /* IN */ UINT16        event_result,
               /* IN */ BPP_HEADERS  *bpp_headers,
               /* IN */ UCHAR         bpp_channel_type,
               /* IN */ UCHAR         more,
               /* OUT */ UINT16       *actual
           );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_BPP_API_ */

