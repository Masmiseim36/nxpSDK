
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
 * BPP request type.
 */
#define BPP_SRVS_DIRECT_PRINT                           0x00U
#define BPP_SRVS_PRINT_BY_REF                           0x01U
#define BPP_SRVS_REFLECTED_USER_INTERFACE               0x02U


/**
 * To Identify the BPP put request type
 */
#define BPP_PUT_REQ_TYPE_MIME_MEDIA_DOC                 0x00U
#define BPP_PUT_REQ_TYPE_REF_SIMPLE                     0x01U
#define BPP_PUT_REQ_TYPE_REF_XML                        0x02U
#define BPP_PUT_REQ_TYPE_REF_LIST                       0x03U
#define BPP_PUT_REQ_TYPE_SIMPLE_REF_PUSH                0x04U
#define BPP_PUT_REQ_TYPE_VCARD                          0x05U
#define BPP_PUT_REQ_TYPE_APPL_HTML                      0x06U
#define BPP_PUT_REQ_TYPE_PLAIN_TEXT                     0x07U
#define BPP_PUT_REQ_TYPE_VCALENDER                      0x08U
#define BPP_PUT_REQ_TYPE_VMSG                           0x09U
#define BPP_PUT_REQ_TYPE_INVALID                        0xFFU

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
#define BPP_SENDER_JOBCH_CONNECT_CNF                    0x01U
#define BPP_SENDER_JOBCH_DISCONNECT_CNF                 0x02U
#define BPP_SENDER_JOBCH_TRANSPORT_CLOSE_CNF            0x03U
#define BPP_SENDER_JOBCH_TRANSPORT_CLOSE_IND            0x04U

#define BPP_SENDER_STATUSCH_CONNECT_CNF                 0x05U
#define BPP_SENDER_STATUSCH_DISCONNECT_CNF              0x06U
#define BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_CNF         0x07U
#define BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_IND         0x08U

#define BPP_SENDER_OBJECTCH_CONNECT_IND                 0x09U
#define BPP_SENDER_OBJECTCH_DISCONNECT_IND              0x0AU
#define BPP_SENDER_OBJECTCH_TRANSPORT_CLOSE_CNF         0x0BU
#define BPP_SENDER_OBJECTCH_TRANSPORT_CLOSE_IND         0x0CU
/*@}*/
/* Simple Push transfer Event Types */
/**
 *  \name Simple Push transfer Event Types
 *
 *  Constant Defines for Simple Push transfer Event Types belonging to \ref BPP_SENDER_EVENTS
 *
 */
/*@{*/
#define BPP_SENDER_FILE_PUSH_CNF                        0x0DU
#define BPP_SENDER_SIMPLE_REF_PUSH_CNF                  0x0EU
/*@}*/
/* Job based transfer Event Types */
/**
 *  \name Job based transfer Event Types
 *
 *  Constant Defines for Job based transfer Event Types belonging to \ref BPP_SENDER_EVENTS
 */
/*@{*/
#define BPP_SENDER_CREATE_JOB_CNF                       0x0FU
#define BPP_SENDER_GET_PRINTER_ATTRS_CNF                0x10U
#define BPP_SENDER_GET_JOB_ATTRS_CNF                    0x11U
#define BPP_SENDER_CANCEL_JOB_CNF                       0x12U
#define BPP_SENDER_CREATE_PR_JOB_CNF                    0x13U
#define BPP_SENDER_GET_MARGINS_CNF                      0x14U
#define BPP_SENDER_GET_EVENT_CNF                        0x15U
#define BPP_SENDER_SEND_DOC_CNF                         0x16U
#define BPP_SENDER_SEND_REF_CNF                         0x17U
#define BPP_SENDER_GET_REF_OBJ_IND                      0x18U
#define BPP_SENDER_GET_RUI_CNF                          0x19U
#define BPP_SENDER_ABORT_CNF                            0x20U
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
#define BPP_PRINTER_JOBCH_CONNECT_IND                   0x61U
#define BPP_PRINTER_JOBCH_DISCONNECT_IND                0x62U
#define BPP_PRINTER_JOBCH_TRANSPORT_CLOSE_CNF           0x63U
#define BPP_PRINTER_JOBCH_TRANSPORT_CLOSE_IND           0x64U

#define BPP_PRINTER_STATUSCH_CONNECT_IND                0x65U
#define BPP_PRINTER_STATUSCH_DISCONNECT_IND             0x66U
#define BPP_PRINTER_STATUSCH_TRANSPORT_CLOSE_CNF        0x67U
#define BPP_PRINTER_STATUSCH_TRANSPORT_CLOSE_IND        0x68U

#define BPP_PRINTER_OBJECTCH_CONNECT_CNF                0x69U
#define BPP_PRINTER_OBJECTCH_DISCONNECT_CNF             0x6AU
#define BPP_PRINTER_OBJECTCH_TRANSPORT_CLOSE_CNF        0x6BU
#define BPP_PRINTER_OBJECTCH_TRANSPORT_CLOSE_IND        0x6CU
/*@}*/
/* Simple Push transfer Event Types */
/**
 *  \name BPP Printer Events for Simple Push transfer Event Types
 *
 *  Constant Defines for Simple Push transfer Event Types belonging to \ref BPP_PRINTER_EVENTS
 */
/*@{*/
#define BPP_PRINTER_FILE_PUSH_IND                       0x6DU
#define BPP_PRINTER_SIMPLE_REF_PUSH_IND                 0x6EU
/*@}*/
/* Job based transfer Event Types */
/**
 *  \name BPP Printer Events for Job based transfer Event Types
 *
 *  Constant Defines for Job based transfer Event Types belonging to \ref BPP_PRINTER_EVENTS
 */
/*@{*/
#define BPP_PRINTER_CREATE_JOB_IND                      0x6FU
#define BPP_PRINTER_GET_PRINTER_ATTRS_IND               0x70U
#define BPP_PRINTER_GET_JOB_ATTRS_IND                   0x71U
#define BPP_PRINTER_CANCEL_JOB_IND                      0x72U
#define BPP_PRINTER_CREATE_PR_JOB_IND                   0x73U
#define BPP_PRINTER_GET_MARGINS_IND                     0x74U
#define BPP_PRINTER_GET_EVENT_IND                       0x75U
#define BPP_PRINTER_SEND_DOC_IND                        0x76U
#define BPP_PRINTER_SEND_REF_IND                        0x77U
#define BPP_PRINTER_GET_REF_OBJ_CNF                     0x78U
#define BPP_PRINTER_GET_RUI_IND                         0x79U
#define BPP_PRINTER_ABORT_IND                           0x7AU
/*@}*/
/**\}*/

#define BPP_EVENT_INVALID                               0xFFU
#define BPP_ENTITY_INVALID                              0xFFU

/** Maximum OBEX PIN SIZE supported */
#define BPP_OBEX_MAX_PIN_SIZE                           0x10U /* 16 */

/** Authentication Challenge/ Response Size */
#define BPP_OBEX_AUTH_CHALLENGE_RESP_SIZE               0x10U /* 16 */

/* BPP Target/Who Header Size */
#define BPP_TARGET_HDR_SIZE                             0x10U /* 16 */

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
#define BPP_TAG_ID_OFFSET                               0x01U
#define BPP_TAG_ID_COUNT                                0x02U
#define BPP_TAG_ID_JOB_ID                               0x03U
#define BPP_TAG_ID_FILE_SIZE                            0x04U
/*@}*/
/* No. of application headers */
#define  BPP_NUM_APPL_HEADERS                           0x04U

/* BPP Application parameters flag corresponding to each Tag */
/**
 *  \name BPP Application parameters flag corresponding to each Tag
 *
 *  Constant Defines for BPP Application parameters flag corresponding to each Tag
 *
 */
/*@{*/
#define BPP_FLAG_OFFSET                                 0x00000001U
#define BPP_FLAG_COUNT                                  0x00000002U
#define BPP_FLAG_JOB_ID                                 0x00000004U
#define BPP_FLAG_FILE_SIZE                              0x00000008U
/*@}*/

/* BPP Channel Types */
/**
 *  \name BPP Channel Types
 *
 *  Constant Defines for BPP Channel Types
 */
/*@{*/
#define BPP_JOB_CHANNEL                                 0x00U
#define BPP_STATUS_CHANNEL                              0x01U
#define BPP_OBJECT_CHANNEL                              0x02U
#define BPP_RUI_CHANNEL                                 0x03U
/*@}*/
/* BPP Operations */
/**
 *  \name BPP Operations
 *
 *  Constant Defines for BPP Operations
 */
/*@{*/
#define BPP_SIMPLE_PUSH_OPERATION                       0x00U
#define BPP_SIMPLE_REF_PUSH_OPERATION                   0x01U
#define BPP_CREATE_JOB_OPERATION                        0x02U
#define BPP_GET_PRINTER_ATTRS_OPERATION                 0x03U
#define BPP_GET_JOB_ATTRS_OPERATION                     0x04U
#define BPP_CANCEL_JOB_OPERATION                        0x05U
#define BPP_CREATE_PRECISE_JOB_OPERATION                0x06U
#define BPP_GET_MARGINS_OPERATION                       0x07U
#define BPP_GET_EVENT_OPERATION                         0x08U
#define BPP_SEND_DOCUMENT_OPERATION                     0x09U
#define BPP_SEND_REFERENCE_OPERATION                    0x0AU
#define BPP_GET_REF_OBJ_OPERATION                       0x0BU
/*@}*/

/**
 *  \name BPP Media Types
 *
 *  Constant Defines for BPP Media Types
 */
/*@{*/
#define BPP_TYPE_MIME_MEDIA_XHTML_PRINT             "application/vnd.pwg-xhtml-print+xml:0.95"
#define BPP_TYPE_MIME_MEDIA_BASIC_TEXT              "text/plain"
#define BPP_TYPE_MIME_MEDIA_VCARD                   "text/x-vcard:2.1"
#define BPP_TYPE_MIME_MEDIA_VCARD30                 "text/x-vcard:3.0"
#define BPP_TYPE_MIME_MEDIA_VCALENDAR               "text/x-vcalendar:1.0"
#define BPP_TYPE_MIME_MEDIA_ICALENDAR20             "text/calendar:2.0"
#define BPP_TYPE_MIME_MEDIA_VMESSAGE                "text/x-vmessage:1.1"
#define BPP_TYPE_MIME_MEDIA_POSTSCRIPT2             "application/PostScript:2"
#define BPP_TYPE_MIME_MEDIA_POSTSCRIPT3             "application/PostScript:3"
#define BPP_TYPE_MIME_MEDIA_PCL3E                   "application/vnd.hp-PCL:5E"
#define BPP_TYPE_MIME_MEDIA_PCL3C                   "application/vnd.hp-PCL:3C"
#define BPP_TYPE_MIME_MEDIA_PDF                     "application/PDF"
#define BPP_TYPE_MIME_MEDIA_JPEG                    "image/jpeg"
#define BPP_TYPE_MIME_MEDIA_GIF89A                  "image/gif:89a"
/*@}*/

/**
 *  \name BPP OBEX Header Type
 *
 *  Constant Defines for BPP OBEX Header types
 */
/*@{*/
#define BPP_TYPE_REF_SIMPLE                             "text/x-ref-simple"
#define BPP_TYPE_REF_XML                                "text/x-ref-xml"
#define BPP_TYPE_REF_LIST                               "text/x-ref-list"
#define BPP_TYPE_SOAP                                   "x-obex/bt-SOAP"
#define BPP_TYPE_REF_OBJ                                "x-obex/referencedobject"
#define BPP_TYPE_RUI                                    "x-obex/RUI"

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

    /**
     * This specifies the byte offset into the image or file
     */
    UINT32                 offset;

    /**
     * This value indicates the number of bytes of the image or
     * file to be sent.
     */
    INT32                  count;

    /**
     * This holds the Job Identifier of the print job.
     */
    UINT32                 job_id;

    /**
     * This holds the size (in bytes) of the object or file
     */
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

/** To initialize a Header Structure */
#define BPP_INIT_HEADER_STRUCT(hs)        \
        (hs).value = NULL;                \
        (hs).length = 0U;

/** To set application parameter flag */
#define BPP_SET_APPL_PARAM_FLAG(flag, set_field)   \
        (flag) = (flag) | (set_field);

/** To retrieve the field value (Set/Reset) */
#define BPP_GET_APPL_PARAM_FLAG(flag, get_field)   \
        ((flag) & (get_field))

/** Resets all the fields of the application parameter flag */
#define BPP_RESET_APPL_PARAM_FLAG(flag)            \
        (flag) = 0x00000000U

/** Extract header structure value */
#define BPP_COPY_HEADER_STRUCT(hdr, len, val)      \
        (len) = (hdr)->length;                     \
        BT_str_n_copy((val), (hdr)->value, (len)); \
        (val)[(len)] = '\0';

/** Extract application parameters */
#define BPP_COPY_APPL_PARAMS(to_params, from_params, search_value_out)      \
        BT_mem_copy((to_params), (from_params), sizeof(BPP_APPL_PARAMS));   \
        if(((from_params)->search_value.length != 0U) &&                    \
           ((from_params)->search_value.value != NULL))                     \
        {                                                                   \
            BT_mem_copy((search_value_out),                                 \
                   (from_params)->search_value.value,                       \
                   (from_params)->search_value.length);                     \
            (to_params)->search_value.value = (search_value_out);           \
            (to_params)->search_value.length =                              \
                                (from_params)->search_value.length;         \
        }

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
 *  \brief To initialize BPP Sender module.
 *
 *  \par Description:
 *       This API initializes the BPP Sender module. It initializes global data
 *       structures and synchronization primitives used in this module.
 *       This function must be called (only once) before any other BPP Sender APIs
 *       are used.
 *
 *  \return
 *       API_SUCCESS, on successful initialization of the Initiator module.
 *       Error Code, describing the reason for failure.
 *
 *  \note This API can be called again to re-initialize BPP Sender module,
 *        after a successful call to \ref BT_bpp_sender_shutdown() API.
 *
 */
API_RESULT BT_bpp_sender_init(void);

/**
 *  \brief To shutdown BPP Sender module.
 *
 *  \par Description:
 *       This API shuts down the BPP Sender module if it is already initialized.
 *
 *  \return
 *       API_SUCCESS, on successful shutdown of BPP sender module.
 *       Error Code, describing the reason for failure.
 *
 *  \note This API shall be called only when there are no active/started
 *         instances of the Sender. In case of active Sender instances,
 *         they should be stopped by calling \ref BT_bpp_sender_stop() API.
 *
 */
API_RESULT BT_bpp_sender_shutdown(void);

/**
 *  \brief To start an instance of BPP Sender Service with specified feature as
 *         given by the bpp_service_type parameter on a given channel specified
 *         by 'bpp_channel_type'
 *
 *  \par Description:
 *       This API starts a BPP Sender instance for a specified feature.
 *       On successful instantiation, this API sets and returns the BPP handle,
 *       which is an index to the BPP Sender entity started. This also registers
 *       the application callback function for the started instance.
 *
 *  \param [out] bpp_handle
 *         Index to the Sender entity that is started.
 *
 *  \param [in] bpp_sender_cb
 *         The application callback function pointer to be registered.
 *
 *  \param [in] bpp_service_type
 *         BPP service/feature type to be started.
 *         Following are the BPP feature IDs:
 *         - \ref BPP_SRVS_DIRECT_PRINT
 *         - \ref BPP_SRVS_PRINT_BY_REF
 *         - \ref BPP_SRVS_REFLECTED_USER_INTERFACE
 *
 *  \param [in] bpp_channel_type
 *         BPP session channel type to be started.
 *         Following are the BPP feature IDs:
 *         - \ref BPP_JOB_CHANNEL
 *         - \ref BPP_STATUS_CHANNEL
 *         - \ref BPP_OBJECT_CHANNEL
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful creation of the instance.
 *       Error Code, describing the reason for failure.
 *
 *  \note All further communication exchanged between the application and
 *        profile for the Initiator instance will be identified using the
 *        assigned BPP handle.
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
 *  \brief To stop an active BPP Sender instance on a given bpp_channel_type
 *         parameter.
 *
 *  \par Description:
 *       This API stops the BPP Sender instance corresponding to the specified
 *       handle.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance that has to be stopped.
 *
 *  \param [in] bpp_channel_type
 *         BPP session channel type to be stopped.
 *         Following are the allowed values:
 *         - \ref BPP_JOB_CHANNEL
 *         - \ref BPP_STATUS_CHANNEL
 *         - \ref BPP_OBJECT_CHANNEL
 *
 *  \return
 *      API_RESULT
 *      API_SUCCESS, on successful termination of the instance.
 *      Error Code, describing the reason for failure.
 *
 *
 *  \note In case an active connection exists in the specified instance to be
 *        stopped, the transport for the connection is closed and the instance
 *        is stopped.
 *
 */
API_RESULT BT_bpp_sender_stop
           (
                /* IN */ BPP_HANDLE *bpp_handle,
                /* IN */ UCHAR       bpp_channel_type
           );

/**
 *  \brief To establish the BPP session connection between Sender and Printer on
 *         a given channel specified by bpp_channel_type parameter.
 *
 *  \par Description:
 *       This API establishes the BPP transport & profile level connection with
 *       the remote BPP Printer on a given channel type. BPP Sender calls the
 *       registered application callback upon the completion of the connection
 *       procedure (when the Printer responds with the connection response) with
 *       the notification BPP_SENDER_JOBCH_CONNECT_CNF for Job Channel Type and
 *       BPP_SENDER_STATUSCH_CONNECT_CNF for Status Channel type.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance that has to be connected.
 *
 *  \param [in] bpp_connect_info
 *         Structure containing the Bluetooth device address of the Printer,
 *         the local maximum receive capacity (in Bytes) and RFCOMM channel of
 *         the Printer.
 *
 *  \param [in] bpp_channel_type
 *         Channel type to establish the BPP session.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the procedure.
 *       Error Code, describing the reason for failure.
 *
 *  \note Before calling this API the application needs to ensure that the BPP
 *        session has been started by calling \ref BT_bpp_sender_start()
 *
 */
API_RESULT BT_bpp_sender_connect
           (
               /* IN */ BPP_HANDLE          * bpp_handle,
               /* IN */ BPP_CONNECT_STRUCT  * bpp_connect_info,
               /* IN */ UCHAR                 bpp_channel_type
           );

/**
 *  \brief To disconnect the established BPP service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the active BPP Session Connection with
 *       the remote BPP Printer. BPP Sender calls the registered application
 *       callback upon completion of this request with the notification
 *       \ref BPP_SENDER_JOBCH_DISCONNECT_CNF for Job Channel
 *       and \ref BPP_SENDER_STATUSCH_DISCONNECT_CNF for Status Channel.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance to be disconnected.
 *
 *  \param [in] bpp_channel_type
 *         Channel type to disconnect the BPP session.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the procedure.
 *       Error Code, describing the reason for failure.
 *
 *  \note BPP Sender automatically initiates transport disconnection once the
 *        BPP disconnection completes and the application shall be notified with
 *        BPP_SENDER_JOBCH_DISCONNECT_CNF and BPP_SENDER_JOBCH_TRANSPORT_CLOSE_CNF
 *        for Job Channel BPP disconnection and its transport disconnection
 *        completion. Similarly BPP_SENDER_STATUSCH_DISCONNECT_CNF and
 *        BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_CNF for Status channel
 *        BPP disconnection and transport disconnection completion respectively.
 *
 */
API_RESULT BT_bpp_sender_disconnect
           (
                /* IN */ BPP_HANDLE           *bpp_handle,
                /* IN */ UCHAR                 bpp_channel_type
           );

/**
 *  \brief To disconnect the underlying transport of BPP Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the BPP service
 *       with the remote BPP Printer. BPP Sender calls the registered application
 *       callback upon completion of this request with the notification event
 *       \ref BPP_SENDER_JOBCH_TRANSPORT_CLOSE_CNF for the Job channel and
 *       \ref BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_CNF for the Status channel.
 *       \ref BPP_SENDER_OBJECTCH_TRANSPORT_CLOSE_CNF for the Object channel.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance to be disconnected.
 *
 *  \param [in] bpp_channel_type
 *         BPP Service channel associated the bpp handle.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the procedure.
 *       Error Code, describing the reason for failure.
 *
 */
API_RESULT BT_bpp_sender_transport_close
           (
               /* IN */ BPP_HANDLE           *bpp_handle,
               /* IN */ UCHAR                 bpp_channel_type
           );

/**
 *  \brief To push a data file to printer.
 *
 *  \par Description:
 *       This API used to push a file to the printer for printing when no print
 *       jobs were created. This is a simple file push operation.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_FILE_PUSH_CNF event along with the status of the procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains the name, type, description and body of the file.
 *
 *  \param [in] more
 *         It Indicates if application has remaining file still to be sent or it
 *         is the complete file content
 *         1 - File part remaining
 *         0 - No more file part remaining
 *
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the Simple Push operation.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note Establish the BPP connection to a BPP Printer through
 *        \ref BT_bpp_sender_connect() API before using this API.
 *
 */
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

/**
 *  \brief To push a file reference (URL) to the printer.
 *
 *  \par Description:
 *       This API used to push the reference of the document to be printed when
 *       no printer jobs were created. The printer gets the document from the
 *       specified location and prints it.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_SIMPLE_REF_PUSH_CNF  event along with the status of the
 *       procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains the name, type, description,
 *         http authorization and body of the file.
 *
 *  \param [in] more
 *         It Indicates if application has remaining file still to be sent or it
 *         is the complete file content
 *         1 - File part remaining
 *         0 - No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 *
 */
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

/**
 *  \brief To configure a print job with given attributes.
 *
 *  \par Description:
 *       This API is used to create the print job at the printer with requested
 *       job attributes. The sender can control the printing operations with the
 *       print job.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_CREATE_JOB_CNF event along with the status of the procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains body of SOAP-encoded CreateJob request.
 *
 *  \param [in] more
 *         It Indicates if application has remaining body still to be sent or it
 *         is the complete body(request) content
 *         1 - File part remaining
 *         0 - No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the CreateJob operation.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note
 *      Establish the BPP connection to a BPP Printer through
 *      \ref BT_bpp_sender_connect() API before using this API.
 */
#define BT_bpp_sender_create_job(bpp_handle, req_info, more, actual)     \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_CREATE_JOB_OPERATION),                                  \
            (BPP_JOB_CHANNEL),                                           \
            (more),                                                      \
            (actual)                                                     \
        )

/**
 *  \brief To query the attributes and capabilities of the printer.
 *
 *  \par Description:
 *       This API is used to get the requested printer attributes.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_GET_PRINTER_ATTRS_CNF event along with the status of
 *       the procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains body of SOAP-encoded Get Printer Attributes request.
 *
 *  \param [in] bpp_channel_type
 *         BPP channel type (Job channel or Status channel) to send this request.
 *
 *  \param [in] more
 *         It Indicates if application has remaining body still to be sent or it
 *         is the complete body(request) content
 *         1 - Body part remaining
 *         0 - No more Body part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the Get printer attributes operation.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note Establish the BPP connection to a BPP Printer through
 *        \ref BT_bpp_sender_connect() API before using this API.
 *
 */
#define BT_bpp_sender_get_printer_attrs(bpp_handle, req_info, bpp_channel_type, more, actual)     \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_GET_PRINTER_ATTRS_OPERATION),                           \
            (bpp_channel_type),                                          \
            (more),                                                      \
            (actual)                                                     \
        )

/**
 *  \brief To query the attributes of a printer job.
 *
 *  \par Description:
 *       This API is used to get the requested job attributes of the printer job
 *       with the given Job Id.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_GET_JOB_ATTRS_CNF event along with the status of the procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains body of SOAP-encoded GetJobAttributes request.
 *
 *  \param [in] bpp_channel_type
 *         BPP channel type (Job channel or Status channel) to send this request.
 *
 *  \param [in] more
 *         It Indicates if application has remaining body still to be sent or it
 *         is the complete body(request) content
 *         1 - Body part remaining
 *         0 - No more body part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the Get Job Attributes operation.
 *       Error Code, describing the reason for failure.
.*
 *
 *  \note Establish the BPP connection to a BPP Printer through
 *        \ref BT_bpp_sender_connect() API before using this API.
 *
 */
#define BT_bpp_sender_get_job_attrs(bpp_handle, req_info, bpp_channel_type, more, actual)     \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_GET_JOB_ATTRS_OPERATION),                               \
            (bpp_channel_type),                                          \
            (more),                                                      \
            (actual)                                                     \
        )

/**
 *  \brief To cancel a Job.
 *
 *  \par Description:
 *       This API is used to cancel the created printer job with the given Job Id.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_CANCEL_JOB_CNF event along with the status of the procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains body of SOAP-encoded CancelJob request.
 *
 *  \param [in] bpp_channel_type
 *         BPP channel type (Job channel or Status channel) to send this request.
 *
 *  \param [in] more
 *         It Indicates if application has remaining body still to be sent or it
 *         is the complete body(request) content
 *         1 - Body part remaining
 *         0 - No more body part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the get image listing operation.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note Establish the BPP connection to a BPP Printer through
 *        \ref BT_bpp_sender_connect() API before using this API.
 *
 */
#define BT_bpp_sender_cancel_job(bpp_handle, req_info, bpp_channel_type, more, actual)     \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_CANCEL_JOB_OPERATION),                                  \
            (bpp_channel_type),                                          \
            (more),                                                      \
            (actual)                                                     \
        )

/**
 *  \brief To configure a printer Job with given attributes (Enhanced layout feature).
 *
 *  \par Description:
 *       This API is used to configure a print Job. If the Printer supports
 *       enhanced layout then all job attributes should be honored.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_CREATE_PR_JOB_CNF event along with the status of the procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains body of SOAP-encoded CreatePreciseJob request.
 *
 *  \param [in] more
 *         It Indicates if application has remaining body still to be sent or it
 *         is the complete body(request) content
 *         1 - Body part remaining
 *         0 - No more body part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the get image Properties operation.
 *       Error Code, describing the reason for failure.
 *
 *  \note Establish the BPP connection to a BPP Printer through
 *        \ref BT_bpp_sender_connect() API before using this API.
 *
 */
#define BT_bpp_sender_create_precise_job(bpp_handle, req_info, more, actual)     \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_CREATE_PRECISE_JOB_OPERATION),                          \
            (BPP_JOB_CHANNEL),                                           \
            (more),                                                      \
            (actual)                                                     \
        )

/**
 *  \brief To obtain margin information for a particular medium size and type
 *         (Enhanced layout feature).
 *
 *  \par Description:
 *       This API is used to obtain the margin information for a particular
 *       media size and type. This is an enhanced layout feature.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_GET_MARGINS_CNF event along with the status of the procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains body of SOAP-encoded GetMargins request.
 *
 *  \param [in] more
 *         It Indicates if application has remaining body still to be sent or it
 *         is the complete body(request) content
 *         1 - Body part remaining
 *         0 - No more body part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the Get Margins operation.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note Establish the BPP connection to a BPP Printer through
 *        \ref BT_bpp_sender_connect() API before using this API.
 *
 */
#define BT_bpp_sender_get_margins(bpp_handle, req_info, more, actual)    \
        bpp_sender_get_operation                                         \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_GET_MARGINS_OPERATION),                                 \
            (BPP_JOB_CHANNEL),                                           \
            (more),                                                      \
            (actual)                                                     \
        )
/**
 *  \brief To send a document to the print job for printing.
 *
 *  \par Description:
 *       This API is used to send the document for the printer.
 *       Before sending the document, the printer Job is to be created and the
 *       Job Id should be specified with the request.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_SEND_DOC_CNF event along with the status of the procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains the name, type, description and body of the file.
 *
 *  \param [in] more
 *         It Indicates if application has remaining body(of the document) still
 *         to be sent or it is the complete body(of the document) content
 *         1 - Body(of the document) part remaining
 *         0 - No more body part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the Send Document operation.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note Establish the BPP connection to a BPP Printer through
 *        \ref BT_bpp_sender_connect() API before using this API.
 *
 */
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

/**
 *  \brief To query the status of the Job being sent.
 *
 *  \par Description:
 *       This API is used by the Sender to query status from the printer over
 *       status channel, while print job is being sent on the Job Channel.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_GET_EVENT_CNF event along with the status of the procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains body of SOAP-encoded GetEvent request.
 *
 *  \param [in] more
 *         It Indicates if application has remaining body still to be sent or it
 *         is the complete body(request) content
 *         1 - Body part remaining
 *         0 - No more body part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the Get Event operation.
 *       Error Code, describing the reason for failure.
 *
 *  \note Establish the BPP connection to a BPP Printer through
 *        \ref BT_bpp_sender_connect() API before using this API.
 *
 */
#define BT_bpp_sender_get_event(bpp_handle, req_info, more, actual)       \
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
 *  \brief To send a document reference to the printer on printer job.
 *
 *  \par Description:
 *       This API is used to send  the reference of the document to be printed
 *       to the Printer Job with given Job Id.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_SEND_REF_CNF notification event.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains the name, type, description and body of the file.
 *
 *  \param [in] more
 *         It Indicates if application has remaining body still to be sent or it
 *         is the complete body(request) content
 *         1 - Body part remaining
 *         0 - No more body part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *      API_RESULT
 *      API_SUCCESS, on successful initiation of the delete file operation.
 *      Error Code, describing the reason for failure.
 *
 *
 *  \note Establish the BPP connection to a BPP Printer through
 *        \ref BT_bpp_sender_connect() API before using this API.
 *
 */
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

/**
 *  \brief To request a web page from the printer for pinter/print
 *         job configuration and control.
 *
 *  \par Description:
 *       This API is used to request the Printer for web page that controls
 *       Printer or Printer Job configuration.
 *       The completion of this procedure is notified through
 *       \ref BPP_SENDER_GET_RUI_CNF event along with the status of the procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains body, HTTP accept header, HTTP accept
 *         language header and body (with RUI reference) of SOAP-encoded
 *         GetMargins request.
 *
 *  \param [in] bpp_channel_type
 *         BPP Channel type.
 *
 *  \param [in] more
 *         It Indicates if application has remaining body still to be sent or
 *         it is the complete body(request) content
 *         1 - Body part remaining
 *         0 - No more body part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the get monitoring image operation.
 *       Error Code, describing the reason for failure.
 *
 *  \note Establish the BPP connection to a BPP Printer through
 *        \ref BT_bpp_sender_connect() API before using this API.
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

/**
 *  \brief To abort ongoing operation.
 *
 *  \par Description:
 *       This API is used to abort the ongoing operation on a given channel.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Printer instance.
 *
 *  \param [in] bpp_channel_type
 *         BPP Channel type.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 */
API_RESULT BT_bpp_sender_send_abort
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
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
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
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
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

/**
 *  \brief To send a request to the Printer.
 *
 *  \par Description:
 *       This API is used send the request to the printer with the evert type
 *       and data given by the application contained in the bpp_headers.
 *       The application calls this API to continue any operation initiated
 *       and to which the first notification (or response) has been received.
 *       This, this API shall be called from the Sender notification callback
 *       function with event_type set to the received event.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback, to indicate
 *         that the request is the continuation of the operation for which it
 *         was notified.
 *
 *  \param [in] event_result
 *         This is the status of the request.
 *
 *  \param [in] bpp_headers
 *         Structure containing either the connect information in case of
 *         connect operation or get request information, in case of get
 *         operation or put request information in case of put operation.
 *
 *  \param [in] bpp_channel_type
 *         Channel type  on which the BPP request being sent.
 *
 *  \param [in] more
 *         Indicates if application has remaining of object body still to be
 *         sent or object complete
 *         1 - Object part remaining
 *         0 - No more object part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the request.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note
 *      - This API is used only in case of the operations with multiple get or put
 *      requests.
 *      - This API shall never be used to send other requests for which direct
 *      APIs are provided.
 *      - The application shall make use of the actual parameter to track the
 *      offset of the object used for BODY header, which may have to be
 *      transmitted in subsequent chunks.
 *
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
 *  \brief To send response to the latest request received from the Printer.
 *
 *  \par Description:
 *       This API is used to send the response to the requests made by the BPP
 *       Sender. Response to connect, send event etc.  shall be sent through
 *       this API.
 *       The application shall call this API with the event type of the latest
 *       notification received. This indicates the response is for the latest
 *       indication received.
 *       BPP Sender frames the OBEX specific packets based on the response &
 *       associated response data and send it to the BIP Responder.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback. This is to
 *         indicate that the response being sent is response to the request for
 *         which the application was notified.
 *
 *  \param [in] event_result
 *         This is the OBEX response code.
 *
 *  \param [in] bip_headers
 *         Structure containing either the connect information, in case of
 *         connect operation or the response data, in case of get/put operation.
 *
 *  \param [in] bpp_channel_type
 *         Channel type on which the BPP response being sent.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the procedure.
 *       Error Code, describing the reason for failure.
 *
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



/* === BPP Printer API ==== */
/**
 *  \brief To initialize BPP Printer module
 *
 *  \par Description:
 *       This API initializes the BPP Printer module. It initializes global
 *       structures and synchronization primitives used in this module.
 *       This function must be called (only once) before any other BPP Printer
 *       APIs are used.
 *
 *  \param None
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initialization of the Printer module.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note This API can be called again to re-initialize BPP Printer module,
 *        after a successful call to \ref BT_bpp_printer_shutdown() API.
 *
 */
API_RESULT BT_bpp_printer_init(void);

/**
 *  \brief To shutdown BPP Printer module.
 *
 *  \par Description:
 *       This API shuts down the BPP Printer module if it is already initialized.
 *
 *  \param None
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful shutdown of Printer module.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note This API shall be called only when there are no active/started
 *        instance of the Printer. In case of active Printer instance,
 *        they should be stopped by calling \ref BT_bpp_printer_stop() API.
 *
 */
API_RESULT BT_bpp_printer_shutdown(void);

/**
 *  \brief To start an instance of BPP Printer Service with specified feature as
 *         given by the bpp_service_type parameter on a given channel specified
 *         by ‘bpp_channel_type’.
 *
 *  \par Description:
 *       This API starts a BPP Printer instance for a specified feature.
 *       On successful instantiation, this API sets and returns the BPP handle,
 *       which is an index to the BPP Printer entity started. This also
 *       registers the application callback function for the started instance.
 *
 *  \param [out] bpp_handle
 *         Index to the Sender entity that is started.
 *
 *  \param [in] bpp_printer_cb
 *         The application callback function pointer to be registered.
 *
 *  \param [in] bpp_service_type
 *         BPP service/feature type to be started.
 *         Following are the BPP feature IDs:
 *         - \ref BPP_SRVS_DIRECT_PRINT
 *         - \ref BPP_SRVS_PRINT_BY_REF
 *         - \ref BPP_SRVS_REFLECTED_USER_INTERFACE
 *
 *
 *  \param [in] bpp_channel_type
 *         BPP session channel type to be started.
 *         Following are the BPP feature IDs:
 *         - \ref BPP_JOB_CHANNEL
 *         - \ref BPP_STATUS_CHANNEL
 *         - \ref BPP_OBJECT_CHANNEL
 *
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful creation of the instance.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note All further communication exchanged between the application and profile
 *        for the Printer instance will be identified using the assigned BPP handle.
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
 *  \brief To stop an active BPP Printer instance on a given bpp_channel_type
 *         parameter
 *
 *  \par Description:
 *       This API stops the BIP Responder instance corresponding to the specified
 *       handle.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Printer instance that has to be stopped.
 *
 *  \param [in] bpp_channel_type
 *         BPP session channel type to be stopped.
 *         Following are the allowed values:
 *         - \ref BPP_JOB_CHANNEL
 *         - \ref BPP_STATUS_CHANNEL
 *         - \ref BPP_OBJECT_CHANNEL
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful termination of the instance.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note In case an active connection exists in the specified instance to be
 *        stopped, the transport for the connection is closed and the instance
 *        is stopped.
 *
 */
API_RESULT BT_bpp_printer_stop
           (
                /* IN */ BPP_HANDLE *bpp_handle,
                /* IN */ UCHAR       bpp_channel_type
           );
/**
 *  \brief To establish the BPP session connection between Printer and Sender on
 *         a given channel specified by ‘bpp_channel_type’ parameter.
 *
 *  \par Description:
 *       This API establishes the BPP transport & profile level connection with
 *       the remote BPP Sender on a given channel type. BPP Sender calls the
 *       registered application callback upon the completion of the connection
 *       procedure (when the Sender responds with the connection response) with
 *       the notification BPP_PRINTER_OBJECTCH_CONNECT_CNF for Object Channel Type.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance that has to be connected.
 *
 *  \param [in] bpp_connect_info
 *         Structure containing the Bluetooth device address of the Printer,
 *         the local maximum receive capacity (in Bytes) and RFCOMM channel of
 *         the Printer.
 *
 *  \param [in] bpp_channel_type
 *         Channel type (BPP_OBJECT_CHANNEL) to establish the BPP session.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the procedure.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note Before calling this API the application needs to ensure that the BPP
 *        session has been started by calling \ref BT_bpp_printer_start()
 *
 */
API_RESULT BT_bpp_printer_connect
           (
               /* IN */ BPP_HANDLE          * bpp_handle,
               /* IN */ BPP_CONNECT_STRUCT  * bpp_connect_info,
               /* IN */ UCHAR                 bpp_channel_type
           );

/**
 *  \brief To disconnect the established BPP Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the active BPP Session Connection with the
 *       remote BPP Sender. BPP Printer calls the registered application
 *       callback upon completion of this request with the notification
 *       BPP_PRINTER_OBJECTCH_DISCONNECT_CNF for Object Channel.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Printer instance to be disconnected.
 *
 *  \param [in] bpp_channel_type
 *         Channel type (BPP_OBJECT_CHANNEL) to disconnect the BPP session.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the procedure.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note BPP Printer Module automatically initiates transport disconnection
 *        once the BPP disconnection completes and the application shall be
 *        notified with  BPP_PRINTER_OBJECTCH_DISCONNECT_CNF &
 *        BPP_PRINTER_OBJECT CH_TRANSPORT_CLOSE_CNF for Object Channel BPP
 *        disconnection and its transport disconnection completion.
 *
 */
API_RESULT BT_bpp_printer_disconnect
           (
                /* IN */ BPP_HANDLE           *bpp_handle,
                /* IN */ UCHAR                 bpp_channel_type
           );

/**
 *  \brief To disconnect the underlying transport of BPP Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the BPP service
 *       with the remote BPP Printer. BPP Sender calls the registered application
 *       callback upon completion of this request with the notification event
 *       \ref BPP_SENDER_JOBCH_TRANSPORT_CLOSE_CNF for the Job channel and
 *       \ref BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_CNF for the Status channel.
 *       \ref BPP_SENDER_OBJECTCH_TRANSPORT_CLOSE_CNF for the Object channel.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Sender instance to be disconnected.
 *
 *  \param [in] bpp_channel_type
 *         BPP Service channel associated the bpp handle.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the procedure.
 *       Error Code, describing the reason for failure.
 *
 *
 *  \note The BIP primary session connection on this transport will be closed
 *        automatically with the transport disconnection.
 *
 */
API_RESULT BT_bpp_printer_transport_close
           (
               /* IN */ BPP_HANDLE   *bpp_handle,
               /* IN */ UCHAR         bpp_channel_type
           );

/**
 *  \brief To request for retrieving the referenced object from the Sender.
 *
 *  \par Description:
 *       This API is used by the Printer to get the referenced objects from the
 *       Sender. When the Sender makes the print request by sending the document
 *       reference, the printer uses the above API to get the actual content of
 *       the document.
 *       The completion of this procedure is notified through
 *       \ref BPP_PRINTER_GET_REF_OBJ_CNF event along with the status of the
 *       procedure.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Printer instance.
 *
 *  \param [in] bpp_req_info
 *         This structure contains the name and size of the reference file.
 *
 *  \return
 *      API_RESULT
 *      API_SUCCESS, on successful initiation of the procedure.
 *      Error Code, describing the reason for failure.
 *
 *  \note Establish the BPP connection to a BPP Sender through
 *        \ref BT_bpp_printer_connect() API before using this API
 *
 */
#define BT_bpp_printer_get_ref_obj(bpp_handle, req_info )                \
        bpp_printer_get_operation                                        \
        (                                                                \
            (bpp_handle),                                                \
            (req_info),                                                  \
            (BPP_GET_REF_OBJ_OPERATION),                                 \
            (BPP_OBJECT_CHANNEL),                                        \
            (0),                                                         \
            (NULL)                                                       \
        )


/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
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
 *  \brief To send response to the latest request received over
 *         ‘bpp_channel_type’ channel from the BPP Sender.
 *
 *  \par Description:
 *       This API is used to send the response to the requests made by
 *       the BPP Sender. Response to connect, send event etc. shall be sent
 *       through this API.
 *       The application shall call this API with the event type of the latest
 *       notification received. This indicates the response is for the latest
 *       indication received.
 *       BPP Printer frames the OBEX specific packets based on the response and
 *       associated response data and send it to the BPP Sender.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Printer instance.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback. This is to
 *         indicate that the response being sent is response to the request for
 *         which the application was notified.
 *
 *  \param [in] event_result
 *         This is the OBEX response code.
 *
 *  \param [in] bpp_headers
 *         Structure containing either the connect information, in case of
 *         connect operation or the response data, in case of get/put operation.
 *
 *  \param [in] bpp_channel_type
 *         Channel type on which the BPP response being sent.
 *
 *  \return
 *      API_RESULT
 *      API_SUCCESS, on successful initiation of the procedure.
 *      Error Code, describing the reason for failure.
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
 *  \brief To send a request to the Sender.
 *
 *  \par Description:
 *       This API is used send the request to the sender with the event type
 *       and data given by the application contained in the bpp_headers.
 *       The application calls this API to continue any operation initiated
 *       and to which the first notification (or response) has been received.
 *       This, API shall be called from the Printer notification callback
 *       function with event_type set to the received event.
 *
 *  \param [in] bpp_handle
 *         BPP handle of the Printer instance.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback, to indicate
 *         that the request is the continuation of the operation for which it
 *         was notified.
 *
 *  \param [in] event_result
 *         This is the status of the request.
 *
 *  \param [in] bpp_headers
 *         Structure containing either the connect information in case of
 *         connect operation or get request information, in case of get
 *         operation or put request information in case of put operation.
 *
 *  \param [in] bpp_channel_type
 *         Channel type  on which the BPP request being sent.
 *
 *  \param [in] more
 *         Indicates if application has remaining of object body still to be
 *         sent or object complete
 *         1 - Object part remaining
 *         0 - No more object part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return
 *       API_RESULT
 *       API_SUCCESS, on successful initiation of the request.
 *       Error Code, describing the reason for failure.
 *
 *  \note
 *      - This API is used only in case of the operations with multiple get or put
 *      requests.
 *      - This API shall never be used to send other requests for which direct
 *      APIs are provided.
 *      - The application shall make use of the actual parameter to track the
 *      offset of the object used for BODY header, which may have to be
 *      transmitted in subsequent chunks.
 */
API_RESULT BT_bpp_printer_send_request
           (
                /* IN  */ BPP_HANDLE     *bpp_handle,
                /* IN  */ UINT8           event_type,
                /* IN  */ UINT16          event_result,
                /* IN  */ BPP_HEADERS    *bpp_headers,
                /* IN  */ UCHAR           bpp_channel_type,
                /* IN  */ UCHAR           more,
                /* OUT */ UINT16         *actual
           );
#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_BPP_API_ */

