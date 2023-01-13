
/**
 *  \file BT_bip_api.h
 *
 *  \brief EtherMind BIP API Header File.
 *
 *  This Header File contains declaration of BIP APIs, Macros and
 *  Constant Definitions for use in applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_BIP_API_
#define _H_BT_BIP_API_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_obex_api.h"

/* ----------------------------------------- Exported Global Variables */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup  bip_module  BIP (Basic Imaging Profile)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Basic Imaging Profile module to the Application and other upper
 *  layers of the stack.
 */
/**
 * \defgroup  bip_defines Defines
 * \{
 * Describes defines for BIP module.
 */
/**
 * \defgroup bip_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
#ifdef BIP_1_2
/* OBEX L2CAP PSM for local initiator entities */
#define BIP_OBEX_L2CAP_PSM                          0x8801U
#endif /* BIP_1_2 */

/**
 *
 * BIP  Operations
 *
 *  1. Get capabilities                             => OBEX GET
 *  2. Put Image                                    => OBEX PUT
 *  3. Put linked thumbnail                         => OBEX PUT
 *  4. Put linked attachment                        => OBEX PUT
 *  5. Remote Display                               => OBEX PUT
 *  6. Get image list                               => OBEX GET
 *  7. Get image properties                         => OBEX GET
 *  8. Get image                                    => OBEX GET
 *  9. Get linked thumbnail                         => OBEX GET
 * 10. Get linked attachment                        => OBEX GET
 * 11. Delete image                                 => OBEX PUT
 * 12. Start Print                                  => OBEX PUT
 * 13. Get partial image                            => OBEX GET
 * 14. Start archive                                => OBEX PUT
 * 15. Get Status                                   => OBEX GET
 * 16. Get monitoring image                         => OBEX GET
 *
 */

/* BIP Target/Who Header Size */
#define BIP_TARGET_HDR_SIZE                         0x10U /* 16 */

/* #### BIP Initiator Events #### */
/**
 *  \defgroup  BIP_INITIATOR_EVENTS BIP Initiator Events
 *
 *
 *  Constant Defines for BIP Initiator Events
 *
 *  \{
 *
 */
/* For Primary BIP Connection */
/**
 *  \name BIP Initiator Events for Primary BIP Connection
 *
 *  Constant Defines for Primary BIP Connection belonging
 *  to group \ref BIP_INITIATOR_EVENTS
 *
 */
/*@{*/
#define BIP_INITIATOR_PRI_CONNECT_CNF                   0x01U
#define BIP_INITIATOR_PRI_DISCONNECT_CNF                0x02U
#define BIP_INITIATOR_PRI_TRANSPORT_CLOSE_CNF           0x03U
#define BIP_INITIATOR_PRI_TRANSPORT_CLOSE_IND           0x04U
/*@}*/
/* For Secondary BIP Connection */
/**
 *  \name BIP Initiator Events for Secondary BIP Connection
 *
 *  Constant Defines for Secondary BIP Connection belonging
 *  to group \ref BIP_INITIATOR_EVENTS
 *
 */
/*@{*/
#define BIP_INITIATOR_SEC_TRANSPORT_CLOSE_CNF           0x05U
#define BIP_INITIATOR_SEC_TRANSPORT_CLOSE_IND           0x06U
#define BIP_INITIATOR_SEC_CONNECT_IND                   0x07U
#define BIP_INITIATOR_SEC_DISCONNECT_IND                0x08U
#define BIP_INITIATOR_PRI_GET_CAPABILITIES_CNF          0x09U
#define BIP_INITIATOR_PRI_PUT_IMAGE_CNF                 0x0AU
#define BIP_INITIATOR_PRI_PUT_LINKED_THUMBNAIL_CNF      0x0BU
#define BIP_INITIATOR_PRI_PUT_LINKED_ATTACHMENT_CNF     0x0CU
/*@}*/
/* For operation*/
/**
 *  \name BIP Initiator Events for Operation
 *
 *  Constant Defines for Operation \ref BIP_INITIATOR_EVENTS
 *
 */
/*@{*/
#define BIP_INITIATOR_PRI_REMOTE_DISPLAY_CNF            0x0DU
#define BIP_INITIATOR_PRI_GET_IMAGE_LIST_CNF            0x0EU
#define BIP_INITIATOR_PRI_GET_IMAGE_PROPERTIES_CNF      0x0FU
#define BIP_INITIATOR_PRI_GET_IMAGE_CNF                 0x10U
#define BIP_INITIATOR_PRI_GET_LINKED_THUMBNAIL_CNF      0x11U
#define BIP_INITIATOR_PRI_GET_LINKED_ATTACHMENT_CNF     0x12U
#define BIP_INITIATOR_PRI_DELETE_IMAGE_CNF              0x13U
#define BIP_INITIATOR_PRI_START_PRINT_CNF               0x14U
#define BIP_INITIATOR_PRI_START_ARCHIVE_CNF             0x15U
#define BIP_INITIATOR_PRI_GET_STATUS_CNF                0x16U
#define BIP_INITIATOR_PRI_GET_MONITORING_IMAGE_CNF      0x17U
#define BIP_INITIATOR_SEC_GET_CAPABILITIES_IND          0x18U
#define BIP_INITIATOR_SEC_GET_IMAGE_LIST_IND            0x19U
#define BIP_INITIATOR_SEC_GET_IMAGE_PROPERTIES_IND      0x1AU
#define BIP_INITIATOR_SEC_GET_IMAGE_IND                 0x1BU
#define BIP_INITIATOR_SEC_GET_LINKED_THUMBNAIL_IND      0x1CU
#define BIP_INITIATOR_SEC_GET_LINKED_ATTACHMENT_IND     0x1DU
#define BIP_INITIATOR_SEC_DELETE_IMAGE_IND              0x1EU
#define BIP_INITIATOR_SEC_GET_PARTIAL_IMAGE_IND         0x1FU
/*@}*/
/**
 *
 * \}
 */

/* #### BIP Responder Events #### */
/**
 *  \defgroup  BIP_RESPONDER_EVENTS BIP Responder Events
 *
 *  Constant Defines for BIP Responder Events
 *
 */
/*@{*/
/* For Connection */
/**
 *  \name BIP Responder Events for Connection
 *
 *  Constant Defines for Connection \ref BIP_RESPONDER_EVENTS
 *
 */
/*@{*/
#define BIP_RESPONDER_PRI_CONNECT_IND                   0x61U
#define BIP_RESPONDER_PRI_DISCONNECT_IND                0x62U
#define BIP_RESPONDER_PRI_TRANSPORT_CLOSE_CNF           0x63U
#define BIP_RESPONDER_PRI_TRANSPORT_CLOSE_IND           0x64U
#define BIP_RESPONDER_SEC_CONNECT_CNF                   0x65U
#define BIP_RESPONDER_SEC_DISCONNECT_CNF                0x66U
#define BIP_RESPONDER_SEC_TRANSPORT_CLOSE_CNF           0x67U
#define BIP_RESPONDER_SEC_TRANSPORT_CLOSE_IND           0x68U

/*@}*/
/* For operation*/
/**
 *  \name BIP Responder Events for Operation
 *
 *  Constant Defines for Operation \ref BIP_RESPONDER_EVENTS
 *
 */
/*@{*/
#define BIP_RESPONDER_PRI_GET_CAPABILITIES_IND          0x69U
#define BIP_RESPONDER_PRI_PUT_IMAGE_IND                 0x6AU
#define BIP_RESPONDER_PRI_PUT_LINKED_THUMBNAIL_IND      0x6BU
#define BIP_RESPONDER_PRI_PUT_LINKED_ATTACHMENT_IND     0x6CU
#define BIP_RESPONDER_PRI_REMOTE_DISPLAY_IND            0x6DU
#define BIP_RESPONDER_PRI_GET_IMAGE_LIST_IND            0x6EU
#define BIP_RESPONDER_PRI_GET_IMAGE_PROPERTIES_IND      0x6FU
#define BIP_RESPONDER_PRI_GET_IMAGE_IND                 0x70U
#define BIP_RESPONDER_PRI_GET_LINKED_THUMBNAIL_IND      0x71U
#define BIP_RESPONDER_PRI_GET_LINKED_ATTACHMENT_IND     0x72U
#define BIP_RESPONDER_PRI_DELETE_IMAGE_IND              0x73U
#define BIP_RESPONDER_PRI_START_PRINT_IND               0x74U
#define BIP_RESPONDER_PRI_START_ARCHIVE_IND             0x75U
#define BIP_RESPONDER_PRI_GET_STATUS_IND                0x76U
#define BIP_RESPONDER_PRI_GET_MONITORING_IMAGE_IND      0x77U
#define BIP_RESPONDER_SEC_GET_CAPABILITIES_CNF          0x78U
#define BIP_RESPONDER_SEC_GET_IMAGE_LIST_CNF            0x79U
#define BIP_RESPONDER_SEC_GET_IMAGE_PROPERTIES_CNF      0x7AU
#define BIP_RESPONDER_SEC_GET_IMAGE_CNF                 0x7BU
#define BIP_RESPONDER_SEC_GET_LINKED_THUMBNAIL_CNF      0x7CU
#define BIP_RESPONDER_SEC_GET_LINKED_ATTACHMENT_CNF     0x7DU
#define BIP_RESPONDER_SEC_DELETE_IMAGE_CNF              0x7EU
#define BIP_RESPONDER_SEC_GET_PARTIAL_IMAGE_CNF         0x7FU
/*@}*/
/*@}*/

#define BIP_EVENT_INVALID                               0xFFU
#define BIP_ENTITY_INVALID                              0xFFU

/* BIP OBEX Response Codes */
/**
 *  \name BIP OBEX Response Codes
 *
 *  Constant Defines for BIP OBEX Response Codes
 */
/*@{*/
#define BIP_SUCCESS_RSP                             OBEX_SUCCESS_RSP
#define BIP_CONTINUE_RSP                            OBEX_CONTINUE_RSP
#define BIP_BAD_REQ_RSP                             OBEX_REQ_IS_BAD_RSP
#define BIP_NOT_IMPLEMENTED_RSP                     OBEX_NOT_IMP_RSP
#define BIP_UNAUTH_RSP                              OBEX_UNAUTHORIZED_RSP
#define BIP_PRECOND_FAILED_RSP                      OBEX_PRECOND_FAIL_RSP
#define BIP_NOT_FOUND_RSP                           OBEX_NOT_FOUND_RSP
#define BIP_NOT_ACCEPTABLE_RSP                      OBEX_NOT_ACCEPTABLE_RSP
#define BIP_NO_SERVICE_RSP                          OBEX_NO_SERVICE_RSP
#define BIP_FORBIDDEN_RSP                           OBEX_FORBIDDEN_RSP
#define BIP_RESPONDER_ERROR                         OBEX_SERVER_ERR_RSP
#define BIP_NOT_MODIFIED_RSP                        OBEX_NOT_MODIFIED_RSP
#define BIP_DATABASE_FULL_RSP                       OBEX_DATABASE_FULL_RSP
#define BIP_CONFLICT_RSP                            OBEX_CONFLICT_RSP
#define BIP_PARTIAL_CONTENT_RSP                     OBEX_PART_CONTENT_RSP
#define BIP_ACCEPT_REQ                              OBEX_ACCEPTED_RSP
/*@}*/
/**
 * OBEX Headers Specific to BIP.
 */
/**
 *  \name BIP OBEX Response Codes
 *
 *  Constant Defines for BIP OBEX Response Codes
 *
 */
/*@{*/
#define OBEX_HDR_IMG_HANDLE                         0x30U
#define OBEX_HDR_IMG_DESCRIPTOR                     0x71U
/*@}*/
#define PRIMARY_CONNECT_PENDING                     0x01U
#define SECONDARY_CONNECT_PENDING                   0x02U
#define NO_CONNECT_PENDING                          0xFFU

/* Application parameters tag ids */
/**
 *  \name Application parameters tag ids
 *
 *  Constant Defines for Application parameters tag ids
 *
 */
/*@{*/
#define BIP_TAG_ID_NB_RETURNED_HANDLES              0x01U
#define BIP_TAG_ID_LIST_START_OFFSET                0x02U
#define BIP_TAG_ID_LATEST_CAPTURED_IMAGES           0x03U
#define BIP_TAG_ID_PARTIAL_FILE_LENGTH              0x04U
#define BIP_TAG_ID_PARTIAL_FILE_START_OFFSET        0x05U
#define BIP_TAG_ID_TOTAL_FILE_SIZE                  0x06U
#define BIP_TAG_ID_END_FLAG                         0x07U
#define BIP_TAG_ID_REMOTE_DISPLAY                   0x08U
#define BIP_TAG_ID_SERVICE_ID                       0x09U
#define BIP_TAG_ID_STORE_FLAG                       0x0AU
/*@}*/
/* No. of application headers */
/**
 *  \name BIP application headers count
 *
 *  Constant Defines for Number of application headers
 *
 */
/*@{*/
#define  BIP_NUM_APPL_HEADERS                       0x0AU
/*@}*/
/* BIP Application parameters flag corresponding to each Tag */
/**
 *  \name BIP Application parameters flag corresponding to each Tag
 *
 *  Constant Defines for BIP Application parameters flag corresponding to each Tag
 *
 */
/*@{*/
#define BIP_FLAG_NB_RETURNED_HANDLES                0x00000001U
#define BIP_FLAG_LIST_START_OFFSET                  0x00000002U
#define BIP_FLAG_LATEST_CAPTURED_IMAGES             0x00000004U
#define BIP_FLAG_PARTIAL_FILE_LENGTH                0x00000008U
#define BIP_FLAG_PARTIAL_FILE_START_OFFSET          0x00000010U
#define BIP_FLAG_TOTAL_FILE_SIZE                    0x00000020U
#define BIP_FLAG_END_FLAG                           0x00000040U
#define BIP_FLAG_REMOTE_DISPLAY                     0x00000080U
#define BIP_FLAG_SERVICE_ID                         0x00000100U
#define BIP_FLAG_STORE_FLAG                         0x00000200U
/*@}*/
/* BIP OBEX Header types */
/**
 *  \name BIP OBEX Header types
 *
 *  Constant Defines for BIP OBEX Header types
 *
 */
/*@{*/
#define BIP_HDR_NAME                                OBEX_HDR_NAME
#define BIP_HDR_LENGTH                              OBEX_HDR_LENGTH
#define BIP_HDR_BODY                                OBEX_HDR_BODY
#define BIP_HDR_END_BODY                            OBEX_HDR_END_BODY
#define BIP_HDR_TYPE                                OBEX_HDR_TYPE
/*@}*/
/* Header specific to BIP */
/**
 *  \name BIP Header specific to BIP
 *
 *  Constant Defines for BIP Header specific to BIP
 *
 */
/*@{*/
#define BIP_HDR_IMAGE_HANDLE                        0x30U
#define BIP_HDR_IMAGE_DESCRIPTOR                    0x71U
#define BIP_HDR_REMOTE_BD_ADDR                      0x98U
/*@}*/
/* BIP image handle size */
/**
 *  \name BIP image handle size
 *
 *  Constant Defines for BIP image handle size
 *
 */
/*@{*/
#define BIP_IMAGE_HANDLE_SIZE                       8 /* bytes */
/*@}*/
/* BIP Feature IDs */
/**
 *  \name BIP Feature IDs
 *
 *  Constant Defines for BIP Feature IDs
 *
 */
/*@{*/
#define BIP_IMAGE_PUSH                              0x00U
#define BIP_IMAGE_PULL                              0x01U
#define BIP_ADVANCED_IMAGE_PRINTING                 0x02U
#define BIP_AUTOMATIC_ARCHIVE                       0x03U
#define BIP_REMOTE_CAMERA                           0x04U
#define BIP_REMOTE_DISPLAY                          0x05U
/*@}*/

/* BIP PUSH Feature Operation IDs */
/**
 *  \name BIP PUSH Feature Operation IDs
 *
 *  Constant Defines for BIP PUSH Feature Operation IDs
 *
 */
/*@{*/
#define BIP_PUT_IMAGE_OPERATION                     0x01U
#define BIP_PUT_LINKED_THUMBNAIL_OPERATION          0x02U
#define BIP_PUT_LINKED_ATTACHMENT_OPERATION         0x03U
/*@}*/
/* BIP PULL Feature Operation IDs */
/**
 *  \name BIP PULL Feature Operation IDs
 *
 *  Constant Defines for BIP PULL Feature Operation IDs
 *
 */
/*@{*/
#define BIP_GET_IMAGE_LISTING_OPERATION             0x04U
#define BIP_GET_IMAGE_PROPERTIES_OPERATION          0x05U
#define BIP_GET_IMAGE_OPERATION                     0x06U
#define BIP_GET_LINKED_THUMBNAIL_OPERATION          0x07U
#define BIP_GET_LINKED_ATTACHEMENT_OPERATION        0x08U
/*@}*/
/* BIP Session Types */
/**
 *  \name BIP Session Types
 *
 *  Constant Defines for BIP Session Types
 *
 */
/*@{*/
#define BIP_SESSION_TYPE_PRIMARY                    0x00U
#define BIP_SESSION_TYPE_SECONDARY                  0x01U
/*@}*/

/** \} */
/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Structures/Data Types */
/**
 * \defgroup bip_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
typedef UCHAR BIP_HANDLE;

/* BIP Header structure, to exchange data with application */
typedef struct _BIP_HEADER_STRUCT
{
    UCHAR   * value;
    UINT16    length;
} BIP_HEADER_STRUCT;

typedef struct
{
    UCHAR byte[BIP_TARGET_HDR_SIZE];
}BIP_SERVICE_ID;

typedef UCHAR BIP_IMAGE_HANDLER;

typedef BIP_HEADER_STRUCT  BIP_IMAGE_LISTING;
typedef BIP_HEADER_STRUCT  BIP_IMAGE_CAPABILITIES;
typedef BIP_HEADER_STRUCT  BIP_IMAGE_PROPERTIES;
typedef BIP_HEADER_STRUCT  BIP_IMAGE_NAME;
typedef BIP_HEADER_STRUCT  BIP_ATTACHMENT_NAME;
typedef BIP_HEADER_STRUCT  BIP_IMAGE_DESC;
typedef BIP_HEADER_STRUCT  BIP_IMAGE_HANDLE;
typedef BIP_HEADER_STRUCT  BIP_ATTACHMENT_DESC;
typedef BIP_HEADER_STRUCT  BIP_PRINT_OBJECT;

/* BIP Connection Structure */
typedef struct _BIP_CONNECT_STRUCT
{
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

#ifdef BIP_1_2
    /**
     * L2CAP psm
     */
    UINT16      psm;
#endif /* BIP_1_2 */
}BIP_CONNECT_STRUCT;


typedef struct _BIP_APPL_PARAMS
{
    /**
     * 32 Bit appl_param_flag [10 Least Significant Bits used]
     *
     * Usage:
     *
     * BIT 0 - nb_returned_handles
     *     1 - list_start_offset
     *     2 - latest_captured_images
     *     3 - partial_file_length
     *     4 - partial_file_start_offset
     *     5 - total_file_size
     *     6 - end_flag
     *     7 - remote_display
     *     8 - service_id
     *     9 - store_flag
    */
    UINT32                 appl_param_flag;

    BIP_HEADER_STRUCT      service_id;
    UINT32                 partial_file_length;
    UINT32                 partial_file_start_offset;
    UINT32                 total_file_size;
    UINT16                 nb_returned_handles;
    UINT16                 list_start_offset;
    UINT8                  latest_captured_images;
    UINT8                  end_flag;
    UINT8                  remote_display;
    UINT8                  store_flag;
} BIP_APPL_PARAMS;


/* BIP Request Structure */
typedef struct _BIP_REQUEST_STRUCT
{
    /**
     * Name
     */
    BIP_HEADER_STRUCT   *name;

    /**
     * BIP object content
     */
    BIP_HEADER_STRUCT   *body;

    /**
     * Image Descriptor
     */
    BIP_HEADER_STRUCT   *image_descriptor;

    /**
     * Image Handle
     */
    BIP_HEADER_STRUCT   *image_handle;

    /**
     * Application parameters
     */
    BIP_APPL_PARAMS     * appl_params;

    /**
     * Wait specifier during SRM transaction
     */
    UCHAR                wait;
} BIP_REQUEST_STRUCT;

/* BIP Response Structure */
typedef struct _BIP_RESPONSE_STRUCT
{
    /**
     * Image Name
     */
    BIP_HEADER_STRUCT       *name;

    /**
     * BIP Object content
     */
    BIP_HEADER_STRUCT       *body;

    /**
     * Image Length
     */
    UINT32                  img_len_info;

    /**
     * Image Handle
     */
    BIP_HEADER_STRUCT       *image_handle;

    /**
     * Image Descriptor
     */
    BIP_HEADER_STRUCT       *image_descriptor;

    /**
     * Application parameter
     */
    BIP_APPL_PARAMS         *appl_params;

    /**
     * Wait specifier during SRM transaction
     */
    UCHAR                wait;
} BIP_RESPONSE_STRUCT;

/* BIP common headers */
typedef union _BIP_HEADERS
{
    /**
     * This parameter is used when the connect request
     * or the connect response is being sent.
     */
    BIP_CONNECT_STRUCT      *bip_connect_info;

    /**
     * This parameter is used to send/receive a request.
     */
    BIP_REQUEST_STRUCT      *bip_req_info;

    /**
     * This parameter is used to send/receive a response.
     */
    BIP_RESPONSE_STRUCT     *bip_rsp_info;

} BIP_HEADERS;
/** \} */
/** \} */

/**
 *  \defgroup bip_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */

/**
 *  \brief BIP Event Notification Callback type
 *
 *  \par Description:
 *       This data type represents the BIP event notification callback.
 *       BIP uses the notification callback function for the following purposes:
 *        To confirm result of an operation initiated by the application.
 *        To inform the application of asynchronous events either initiated by peer side or the profile
 *
 *  \param [in] bip_handle
 *         Information of the Instance ID and service type of the  BIP entity
 *         responsible for this event.
 *
 *  \param [in] event_type
 *         This parameter defines the BIP event being reported. The valid event types are listed in
 *         BIP Event Notification Callback Event Types section.
 *
 *  \param [in] event_result
 *         The status of the event being reported to the application. In case of Initiator, this
 *         contains the response code being sent by the Responder along with the response data
 *         (optional) or an error code with protocol/profile ID, if an error is detected at the local
 *         side. In case of Responder, it is the request status as set by the profile or any error code, if detected
 *
 *  \param [in] bip_headers
 *         Depending upon the event_type, this parameter contains one of the
 *         following information:
 *           - Connect Information
 *           - Request Information
 *           - Response Information
 *
 *  \param [in] num_headers
 *         This parameter specifies the number of valid fields in the bip_headers
 *         information.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
typedef API_RESULT (* BIP_EVENT_NTF_CB)
                   (
                        /* IN */ BIP_HANDLE      * bip_handle,
                        /* IN */ UINT8             event_type,
                        /* IN */ UINT16            event_result,
                        /* IN */ BIP_HEADERS     * bip_headers,
                        /* IN */ UINT16            num_headers
                   );
/** \} */
/* ----------------------------------------- Macros  */
/**
 * \defgroup bip_utility_macros Utility Macros
 * \{
 * Describes Utility Macros defined by the module.
 */
/** To initialize a Header Structure */
#define BIP_INIT_HEADER_STRUCT(hs)    \
        (hs).value = NULL;    \
        (hs).length = 0U;

/** To set application parameter flag */
#define BIP_SET_APPL_PARAM_FLAG(flag, set_field)   \
        (flag) = (flag) | (set_field);

/** To retrieve the field value (Set/Reset) */
#define BIP_GET_APPL_PARAM_FLAG(flag, get_field)   \
        ((flag) & (get_field))

/** Resets all the fields of the application parameter flag */
#define BIP_RESET_APPL_PARAM_FLAG(flag)            \
        (flag) = 0x00000000U

/** Extract header structure value */
#define BIP_COPY_HEADER_STRUCT(hdr, len, val)      \
        (len) = (hdr)->length;                     \
        BT_str_n_copy((val), (hdr)->value, (len)); \
        (val)[(len)] = '\0';

/** Extract application parameters */
#define BIP_COPY_APPL_PARAMS(to_params, from_params, search_value_out)      \
        BT_mem_copy((to_params), (from_params), sizeof(BIP_APPL_PARAMS));   \
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
 * \defgroup bip_api API definitions
 * \{
 * Describes API definitions of BIP module.
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef BIP_INITIATOR
/* === BIP Initiator API ==== */
/**
 *  \brief To initialize BIP Initiator module
 *
 *  \par Description:
 *       This API initializes the BIP Initiator module. This function must be called
 *       (only once) before any other BIP Initiator APIs are used.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_bip_initiator_init(void);

/**
 *  \brief To shutdown BIP Initiator module
 *
 *  \par Description:
 *       This API shuts down the BIP Initiator module if it is already initialized.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_bip_initiator_shutdown(void);

/**
 *  \brief To start an instance of BIP Initiator Service as given
 *         by the bip_feature_id parameter.
 *
 *  \par Description:
 *       This API starts a BIP Initiator instance. On successful instantiation,
 *       this API sets and returns the BIP handle, which is an index to the BIP
 *       Initiator entity started. This also registers the application callback
 *       function for the started instance.
 *
 *  \param [out] bip_handle
 *         Index to the BIP Initiator entity that is started.
 *
 *  \param [in] bip_initiator_cb
 *         The application Callback function pointer to be registered.
 *
 *  \param [in] bip_feature_id
 *         To identify BIP feature to be started.
 *         Following are the feature IDs:
 *         - BIP_IMAGE_PUSH
 *         - BIP_IMAGE_PULL
 *         - BIP_ADVANCED_IMAGE_PRINTING
 *         - BIP_AUTOMATIC_ARCHIVE
 *         - BIP_REMOTE_CAMERA
 *         - BIP_REMOTE_DISPLAY
 *
 *  \param [in] session_type
 *         To identify BIP (primary/secondary) session to be started.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note All further communication exchanged between the application and
 *        profile for the BIP Initiator instance will be identified using the assigned
 *        BIP handle.
 */
API_RESULT BT_bip_initiator_start
           (
               /* OUT */ BIP_HANDLE       * bip_handle,
               /* IN */  BIP_EVENT_NTF_CB   bip_initiator_cb,
               /* IN */  UCHAR              bip_feature_id,
               /* IN */  UCHAR              session_type
           );

/**
 *  \brief To stop an active BIP Initiator instance as
 *         given by the bip_session_type parameter.
 *
 *  \par Description:
 *       This API stops the BIP Initiator instance corresponding to the specified handle.
 *
 *  \param [in] bip_handle
 *         Handle of the BIP Initiator instance that has to be stopped.
 *
 *  \param [in] bip_session_type
 *         BIP session Type (Primary/Secondary) to be stopped.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note In case an active connection exists in the specified instance to be
 *        stopped, the transport for the connection is closed and the instance
 *        is stopped.
 */
API_RESULT BT_bip_initiator_stop
           (
               /* IN */ BIP_HANDLE        *bip_handle,
               /* IN */ UCHAR              bip_session_type
           );

/**
 *  \brief To establish the BIP Primary session connection between initiator & responder.
 *
 *  \par Description:
 *       This API establishes the BIP transport & profile level connection with the remote
 *       BIP Responder for the BIP Primary session. BIP Initiator calls the registered
 *       application callback upon the completion of the connection procedure (when the
 *       Responder responds with the connection response) with the notification
 *       \ref BIP_INITIATOR_PRI_CONNECT_CNF.
 *
 *  \param [in] bip_handle
 *         BIP handle of the Initiator instance that has to be connected
 *
 *  \param [in] bip_connect_info
 *         Structure containing the Bluetooth device address of the Responder,
 *         the local maximum receive capacity (in Bytes) and RFCOMM channel/L2CAP
 *         psm of Responder.
 *
 *         The parameter decided in this parameter are:
 *         bd_addr - Bluetooth Device Address of the remote device.
 *         User must get the remote device Bluetooth Device Address using the
 *         inquiry procedure.
 *         server_channel - The RFCOMM server channel. User must get the remote
 *         device RFCOMM server channel using SDP query.
 *         L2CAP psm - The L2CAP psm. User must get the remote device L2CAP psm using SDP query.
 *         bip_mtu - The maximum packet size the application can receive.
 *
 *  \return API_RESULT
 *          API_SUCCESS, on successful initiation of connection establishment.
 *          Error Code, describing the reason for failure.
 */
API_RESULT BT_bip_initiator_connect
           (
               /* IN */ BIP_HANDLE          * bip_handle,
               /* IN */ BIP_CONNECT_STRUCT  * bip_connect_info
           );

/**
 *  \brief To disconnect the established BIP primary Connection.
 *
 *  \par Description:
 *       This API is to disconnect the active BIP Primary Session Connection with the
 *       remote BIP Responder. BIP Initiatror calls the registered application callback
 *       upon completion of this request with the notification \ref BIP_INITIATOR_PRI_DISCONNECT_CNF.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection with remote device.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
API_RESULT BT_bip_initiator_disconnect
           (
               /* IN */ BIP_HANDLE *bip_handle
           );

/**
 *  \brief To disconnect the underlying transport of BIP Primary session Connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the Primary session with
 *       the remote BIP Responder. BIP Initiator calls the registered application callback
 *       upon completion of this request with the notification \ref BIP_INITIATOR_PRI_TRANSPORT_CLOSE_CNF.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection with remote device.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
API_RESULT BT_bip_initiator_pri_transport_close
           (
               /* IN */    BIP_HANDLE   *bip_handle
           );

/**
 *  \brief To disconnect the underlying transport of BIP Secondary session connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the Secondary session with
 *       the remote BIP Responder. BIP Initiator calls the registered application callback
 *       upon completion of this request with the notification \ref BIP_INITIATOR_SEC_TRANSPORT_CLOSE_CNF
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection with remote device.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
API_RESULT BT_bip_initiator_sec_transport_close
           (
               /* IN */    BIP_HANDLE   *bip_handle
           );

/**
 *  \brief To get the imaging capabilities of remote BIP Responder.
 *
 *  \par Description:
 *       This API retrieves the imaging-capabilities object from the responder entity.
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_GET_CAPABILITIES_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
API_RESULT BT_bip_initiator_get_capabilities
           (
               /* IN */ BIP_HANDLE  *bip_handle
           );

/**
 *  \brief To monitor the Secondary Session connection from the Initiator side.
 *
 *  \par Description:
 *       This API retrieves the status of the secondary session connection.
 *       A Success response code in the GetStatus response indicates that the secondary
 *       connection has successfully terminated. A Continue response code indicates that
 *       the secondary connection is still active. An error code indicates that the secondary
 *       connection is being affected by an error.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_GET_STATUS_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
API_RESULT BT_bip_initiator_get_status
           (
               /* IN */ BIP_HANDLE  *bip_handle
           );

/**
 *  \brief To Push an image to the Responder.
 *
 *  \par Description:
 *       This API is used to push an image to the remote Responder along with image
 *       file name and description. On successful transmission of the image, an image
 *       handle is retrieved from the responder side.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_PUT_IMAGE_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         This structure contains the name and body of the image file.
 *
 *  \param [in] more
 *         It indicates if application has remaining file still to be sent or
 *         it is the complete file content
 *         1 -> File part remaining
 *         0 -> No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included in
 *         the request, that is packed for transmission.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_initiator_push_image(bip_handle, req_info, more, actual)  \
        bip_initiator_push_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_PUT_IMAGE_OPERATION),                                   \
            (more),                                                      \
            (actual)                                                     \
        )

/**
 *  \brief To push thumbnail file related to respective image to the Responder.
 *
 *  \par Description:
 *       This API is used to push thumbnail file of the respective image to the remote
 *       Responder along with received image handle.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_PUT_LINKED_THUMBNAIL_CNF
 *       event along with the status of the procedure
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         This structure contains the image handle and body of the thumbnail file.
 *
 *  \param [in] more
 *         It indicates if application has remaining file still to be sent or
 *         it is the complete file content
 *         1 -> File part remaining
 *         0 -> No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included in
 *         the request, that is packed for transmission.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_initiator_put_linked_thumbnail(bip_handle, req_info, more, actual)\
        bip_initiator_push_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_PUT_LINKED_THUMBNAIL_OPERATION),                        \
            (more),                                                      \
            (actual)                                                     \
        )

/**
 *  \brief To push attachment file related to respective image to the Responder.
 *
 *  \par Description:
 *       This API is used to push attachment file of the respective image to the remote
 *       Responder along with received image handle and file description.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_PUT_LINKED_ATTACHMENT_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         This structure contains the image handle, image description and body of the file.
 *
 *  \param [in] more
 *         It indicates if application has remaining file still to be sent or
 *         it is the complete file content
 *         1 -> File part remaining
 *         0 -> No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included in
 *         the request, that is packed for transmission.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_initiator_put_linked_attachment(bip_handle, req_info, more, actual)\
        bip_initiator_push_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_PUT_LINKED_ATTACHMENT_OPERATION),                       \
            (more),                                                      \
            (actual)                                                     \
        )

/**
 *  \brief To retrieve the list of images from the Responder.
 *
 *  \par Description:
 *       This API retrieves the Images listing object from the requested directory/folder
 *       at the connected Responder entity. The application shall include the application
 *       parameters for the request using the req_info parameter. On return, the Responder
 *       will be send the Image-listing object with Number of returned handles  in the Application
 *       Parameters.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_GET_IMAGE_LISTING_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         Structure containing application parameters (like 1. number of Image handles to be returned,
 *         2. Image List start offset and 3. Latest Catured Images), which may be included by the
 *         application and Image handles descriptor.
 *         The following application parameters are supported as part of this request for this parameter.
 *         NbReturnedHandles, ListStartOffset, LatestCapturedImages.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_initiator_get_image_listing(bip_handle, req_info)         \
        bip_initiator_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_LISTING_OPERATION)                            \
        )

/**
 *  \brief To retrieve a description of an image's characteristics from the Responder.
 *
 *  \par Description:
 *       This API retrieves the Image's characteristics object from the requested directory/folder
 *       at the connected Responder entity. The application must include the image handle details
 *       in the req_info parameter.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_GET_IMAGE_PROPERTIES_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         Structure containing Image handle details for which the characteristics need to be
 *         retrieved from the responder.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_initiator_get_image_properties(bip_handle, req_info)      \
        bip_initiator_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_PROPERTIES_OPERATION)                         \
        )

/**
 *  \brief To retrieve the thumbnail version of an image given its handle from the Responder.
 *
 *  \par Description:
 *       This API retrieves the thumbnail version of an image given its handle from the Responder
 *       entity. The application must include the image handle in the req_info parameter.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_GET_LINKED_THUMBNAIL_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         Structure containing Image handle for which the thumbnail object need to be retrieved
 *         from the responder.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_initiator_get_linked_thumbnail(bip_handle, req_info)      \
        bip_initiator_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_LINKED_THUMBNAIL_OPERATION)                         \
        )

/**
 *  \brief To retrieve an image from the Responder.
 *
 *  \par Description:
 *       This API retrieves an image object from the requested directory/folder at the connected
 *       Responder entity. The application must include the image handle & image description in the
 *       req_info parameter. On return the Responder responses with image data and file size.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_GET_IMAGE_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         Structure containing Image handle and image description details for which the image object
 *         need to be retrieved from the responder.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_initiator_get_image(bip_handle, req_info)                 \
        bip_initiator_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_OPERATION)                                    \
        )

/**
 *  \brief To retrieve attachment file of a specific image from the Responder.
 *
 *  \par Description:
 *       This API retrieves an attachment associated with an image from the requested
 *       directory/folder at the connected Responder entity. The application should include
 *       image handle details and name of the file details for the request using the req_info parameter.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_GET_LINKED_ATTACHMENT_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         Structure containing Image handle and File name details for which the attachment file
 *         need to be retrieved from the responder.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_initiator_get_linked_attachment(bip_handle, req_info)     \
        bip_initiator_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_LINKED_ATTACHEMENT_OPERATION)                       \
        )

/**
 *  \brief To delete an image in the BIP responder.
 *
 *  \par Description:
 *       This function sends a request for image delete operation on the BIP Responder.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_DELETE_IMAGE_CNF
 *       notification event
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] bip_req_info
 *         This structure contains the image handle of the image to be deleted.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h
 */
API_RESULT BT_bip_initiator_delete_image
           (
               /* IN */ BIP_HANDLE         *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT *bip_req_info
           );

/**
 *  \brief To retrieve monitoring images from the Responder.
 *
 *  \par Description:
 *       This API retrieves monitoring images from the requested directory/folder at
 *       the connected Responder entity. The application shall include the application
 *       parameters for the request using the bip_req_info parameter.
 *       On return, the Responder will be send the Image object and image handle.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_GET_MONITORING_IMAGE_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection
 *
 *  \param [in] bip_req_info
 *         Structure containing image handle and application parameters, which may be included
 *         by the application.
 *         The StoreFlag application parameter is supported as part of this request for this parameter.
 *         StoreFlag. Following values are allowed:
 *         0x00 - Server should not store the full size image.
 *         0x01 - Server should store the full size image, any other value is illegal.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
API_RESULT BT_bip_initiator_get_monitoring_image
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info
           );

/**
 *  \brief To retrieve the image handle from the Responder with respect to the request sent by the Initiator
 *
 *  \par Description:
 *       This API is used to pilot the screen of the connected Responder entity. The application shall
 *       include the application parameters and image handle for the request using the bip_req_info parameter.
 *       On return, the Responder will be send the image handle.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_REMOTE_DISPLAY_CNF event
 *       along with the status of the procedure.
 *
 *       Application paramter:
 *          NextImage (display the next image)
 *          PreviousImage (display the previous image)
 *          SelectImage (display a specific image)
 *          CurrentImage (retrieve the handle of the currently displayed image)
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] bip_handle
 *          Structure containing application parameters and image handle, which may be included
 *          by the application. The RemoteDisplay application parameter is supported as part of
 *          this request for this parameter.
 *
 *          remote_display, Command to specify the image to be displayed.
 *          The allowed values are:
 *          0x01 : NextImage
 *          0x02 : PreviousImage
 *          0x03 : SelectImage
 *          0x04 : Current image
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
API_RESULT BT_bip_initiator_remote_display
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info
           );

/**
 *  \brief To trigger a printjob in the Repsonder from the Initiator.
 *
 *  \par Description:
 *       This API is used to trigger a BIP Responder with print capability to execute a print job.
 *       The print job is described in the printer-control object sent to BIP Responder. As a result
 *       of StartPrint request, BIP Responder opens a new OBEX connection to the referenced objects
 *       service of BIP Initiator.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_START_PRINT_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] bip_req_info
 *         This structure contains the name and body of the file.
 *
 *  \param [in] more
 *         It indicates if application has remaining file still to be sent or
 *         it is the complete file content
 *          1 - File part remaining
 *          0 - No more file part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included in the
 *         request, that is packed for transmission.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h
 */
API_RESULT BT_bip_initiator_start_print
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info,
               /* IN */  UCHAR                 more,
               /* OUT */ UINT16               *actual
           );

/**
 *  \brief To trigger Responder to retrieve images from the Initiator.
 *
 *  \par Description:
 *       This API is used to trigger a BIP Responder to start archiving the image files
 *       from BIP Initiator. As a result of StartArchive request, BIP Responder opens a
 *       new OBEX connection to the Automatic Archive service of BIP Initiator.
 *
 *       The completion of this procedure is notified through \ref BIP_INITIATOR_PRI_START_ARCHIVE_CNF
 *       event along with the status of the procedure
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] bip_req_info
 *         Structure containing the serviceID application parameter.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h
 */
API_RESULT BT_bip_initiator_start_archive
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info
           );

/**
 *  \brief To send a request to the BIP Responder.
 *
 *  \par Description:
 *       This API sends the request to the BIP Responder with the corresponding BIP event and
 *       the data given by the application contained in the BIP_headers.  The application shall
 *       call this API to continue any operation initiated and to which the first notification
 *       has been received. Thus this API shall be called from the BIP Initiator application
 *       callback function with the event_type set to the current received event
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback, to indicate that
 *         the request is the continuation of the operation for which it was notified.
 *
 *  \param [in] event_result
 *         This is the status of the request.
 *
 *  \param [in] bip_headers
 *         Structure containing either the connect information in case of connect operation
 *         or get request information, in case of get operation or put request information in
 *         case of put operation.
 *
 *  \param [in] more
 *         Indicates if application has remaining of object body still to be sent or object complete.
 *         1 - Object part remaining
 *         0 - No more object part remaining.
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included in the request,
 *         that is packed for transmission.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h
 */
API_RESULT BT_bip_initiator_send_request
           (
               /* IN */    BIP_HANDLE   *bip_handle,
               /* IN */    UINT8         event_type,
               /* IN */    UINT16        event_result,
               /* IN */    BIP_HEADERS  *bip_headers,
               /* IN */    UCHAR         more,
               /* OUT */   UINT16       *actual
           );

/**
 *  \brief To send response to the latest request received over Secondary channel
 *         from the BIP Responder.
 *
 *  \par Description:
 *       This API is used to send the response to the requests made by the BIP Responder.
 *       Response to connect, send event etc. shall be sent through this API.
 *
 *       The application shall call this API with the event type of the latest notification
 *       received. This indicates the response is for the latest indication received.
 *       BIP Initiator frames the OBEX specific packets based on the response and associated
 *       response data and send it to the BIP Responder.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback. This is to indicate that
 *         the response being sent is response to the request for which the application was notified.
 *
 *  \param [in] event_result
 *         This is the OBEX response code.
 *
 *  \param [in] bip_headers
 *         Structure containing either the connect information, in case of connect operation or the
 *         response data, in case of get/put operation
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h
 */
API_RESULT BT_bip_initiator_send_response
           (
               /* IN */  BIP_HANDLE    * bip_handle,
               /* IN */  UINT8           event_type,
               /* IN */  UCHAR           event_result,
               /* IN */  BIP_HEADERS   * bip_headers
           );

/**
 * Internal function to handle PUSH operations
 * Application need not to call this API.
 */
API_RESULT bip_initiator_push_operation
           (
               /* IN */  BIP_HANDLE           *bip_handle,
               /* IN */  BIP_REQUEST_STRUCT   *bip_req_info,
               /* IN */  UCHAR                 bip_operation_id,
               /* IN */  UCHAR                 more,
               /* OUT */ UINT16               *actual
           );

/**
 * Internal function to handle PULL operations
 * Application need not to call this API.
 */
API_RESULT bip_initiator_pull_operation
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info,
               /* IN */ UCHAR                 bip_operation_id
           );

#endif /* BIP_INITIATOR */

#ifdef BIP_RESPONDER

/* === BIP Responder API ==== */

/**
 *  \brief To initialize BIP Responder module
 *
 *  \par Description:
 *       This API initializes the BIP Resonder module. This function must be called
 *       (only once) before any other BIP Responder APIs are used.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_bip_responder_init(void);

/**
 *  \brief To shutdown BIP Responder module
 *
 *  \par Description:
 *       This API shutsdown the BIP Responder module if it is already initialized.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 */
API_RESULT BT_bip_responder_shutdown(void);

/**
 *  \brief To start an instance of BIP Initiator Service as given
 *         by the bip_feature_id parameter.
 *
 *  \par Description:
 *       This API starts a BIP Initiator instance. On successful instantiation,
 *       this API sets and returns the BIP handle, which is an index to the BIP
 *       Initiator entity started. This also registers the application callback
 *       function for the started instance.
 *
 *  \param [out] bip_handle
 *         Index to the BIP Initiator entity that is started.
 *
 *  \param [in] bip_responder_cb
 *         The application Callback function pointer to be registered.
 *
 *  \param [in] bip_feature_id
 *         To identify BIP feature to be started.
 *         Following are the feature IDs:
 *         - BIP_IMAGE_PUSH
 *         - BIP_IMAGE_PULL
 *         - BIP_ADVANCED_IMAGE_PRINTING
 *         - BIP_AUTOMATIC_ARCHIVE
 *         - BIP_REMOTE_CAMERA
 *         - BIP_REMOTE_DISPLAY
 *
 *  \param [in] session_type
 *         To identify BIP (primary/secondary) session to be started.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note All further communication exchanged between the application and
 *        profile for the BIP Initiator instance will be identified using the assigned
 *        BIP handle.
 */
API_RESULT BT_bip_responder_start
           (
               /* OUT */ BIP_HANDLE        *bip_handle,
               /* IN */  BIP_EVENT_NTF_CB   bip_responder_cb,
               /* IN */  UCHAR              bip_feature_id,
               /* IN */  UCHAR              session_type
           );

/**
 *  \brief To stop an active BIP Responder instance as
 *         given by the bip_session_type parameter.
 *
 *  \par Description:
 *       This API stops the BIP Responder instance corresponding to the specified handle.
 *
 *  \param [in] bip_handle
 *         Handle of the BIP Responder instance that has to be stopped.
 *
 *  \param [in] bip_session_type
 *         BIP session Type (Primary/Secondary) to be stopped.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note In case an active connection exists in the specified instance to be
 *        stopped, the transport for the connection is closed and the instance
 *        is stopped.
 */
API_RESULT BT_bip_responder_stop
           (
               /* IN */ BIP_HANDLE        *bip_handle,
               /* IN */ UCHAR              bip_session_type
           );

/**
 *  \brief To establish the BIP Secondary session connection between initiator & responder.
 *
 *  \par Description:
 *       This API establishes the BIP transport & profile level connection with the remote
 *       BIP Initiator for the BIP Secondary session. BIP Responder calls the registered
 *       application callback upon the completion of the connection procedure (when the
 *       initiator responds with the connection response) with the notification
 *       \ref BIP_RESPONDER_SEC_CONNECT_CNF.
 *
 *  \param [in] bip_handle
 *         BIP handle of the Initiator instance that has to be connected
 *
 *  \param [in] bip_connect_info
 *         Structure containing the Bluetooth device address of the Initiator,
 *         the local maximum receive capacity (in Bytes) and RFCOMM channel/L2CAP
 *         psm of Initiator.
 *
 *         The parameter decided in this parameter are:
 *         bd_addr - Bluetooth Device Address of the remote device.
 *         User must get the remote device Bluetooth Device Address using the
 *         inquiry procedure.
 *         server_channel - The RFCOMM server channel. User must get the remote
 *         device RFCOMM server channel using SDP query.
 *         L2CAP psm - The L2CAP psm. User must get the remote device L2CAP psm using SDP query.
 *         bip_mtu - The maximum packet size the application can receive.
 *
 *  \return API_RESULT
 *          API_SUCCESS, on successful initiation of connection establishment.
 *          Error Code, describing the reason for failure.
 */
API_RESULT BT_bip_responder_connect
           (
               /* IN */ BIP_HANDLE          * bip_handle,
               /* IN */ BIP_CONNECT_STRUCT  * bip_connect_info
           );

/**
 *  \brief To disconnect the established BIP Secondary Connection.
 *
 *  \par Description:
 *       This API is to disconnect the active BIP Secondary Session Connection with the
 *       remote BIP Initiator. BIP Responder calls the registered application callback
 *       upon completion of this request with the notification \ref BIP_RESPONDER_SEC_DISCONNECT_CNF.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection with remote device.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
API_RESULT BT_bip_responder_disconnect
           (
               /* IN */ BIP_HANDLE *bip_handle
           );

/**
 *  \brief To disconnect the underlying transport of BIP Secondary session Connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the Secondary session with
 *       the remote BIP Initiator. BIP Responder calls the registered application callback
 *       upon completion of this request with the notification \ref BIP_RESPONDER_SEC_TRANSPORT_CLOSE_CNF.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection with remote device.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
API_RESULT BT_bip_responder_sec_transport_close
           (
               /* IN */ BIP_HANDLE   *bip_handle
           );

/**
 *  \brief To disconnect the underlying transport of BIP Primary session Connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the Primary session with
 *       the remote BIP Initiator. BIP Responder calls the registered application callback
 *       upon completion of this request with the notification \ref BIP_RESPONDER_PRI_TRANSPORT_CLOSE_CNF.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection with remote device.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
API_RESULT BT_bip_responder_pri_transport_close
           (
               /* IN */ BIP_HANDLE   *bip_handle
           );

/**
 *  \brief To get the imaging capabilities of remote BIP Initiator.
 *
 *  \par Description:
 *       This API retrieves the imaging-capabilities object from the initiator entity.
 *       The completion of this procedure is notified through \ref BIP_RESPONDER_SEC_GET_CAPABILITIES_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
API_RESULT BT_bip_responder_get_capabilities
           (
               /* IN */ BIP_HANDLE  *bip_handle
           );

/**
 *  \brief To retrieve the list of images from the Initiator.
 *
 *  \par Description:
 *       This API retrieves the Images listing object from the requested directory/folder
 *       at the connected Initiator entity. The application shall include the application
 *       parameters for the request using the req_info parameter. On return, the Initiator
 *       will be send the Image-listing object with Number of returned handles  in the Application
 *       Parameters.
 *
 *       The completion of this procedure is notified through \ref BIP_RESPONDER_SEC_GET_IMAGE_LISTING_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         Structure containing application parameters (like 1. number of Image handles to be returned,
 *         2. Image List start offset and 3. Latest Catured Images), which may be included by the
 *         application and Image handles descriptor.
 *         The following application parameters are supported as part of this request for this parameter.
 *         NbReturnedHandles, ListStartOffset, LatestCapturedImages.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_responder_get_image_listing(bip_handle, req_info)         \
        bip_responder_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_LISTING_OPERATION)                            \
        )

/**
 *  \brief To retrieve a description of an image's characteristics from the Initiator.
 *
 *  \par Description:
 *       This API retrieves the Image's characteristics object from the requested directory/folder
 *       at the connected Responder entity. The application must include the image handle details
 *       in the req_info parameter.
 *
 *       The completion of this procedure is notified through \ref BIP_RESPONDER_SEC_GET_IMAGE_PROPERTIES_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         Structure containing Image handle details for which the characteristics need to be
 *         retrieved from the initiator.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_responder_get_image_properties(bip_handle, req_info)      \
        bip_responder_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_PROPERTIES_OPERATION)                         \
        )

/**
 *  \brief To retrieve the thumbnail version of an image given its handle from the Initiator.
 *
 *  \par Description:
 *       This API retrieves the thumbnail version of an image given its handle from the Initiator
 *       entity. The application must include the image handle in the req_info parameter.
 *
 *       The completion of this procedure is notified through \ref BIP_RESPONDER_SEC_GET_LINKED_THUMBNAIL_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         Structure containing Image handle for which the thumbnail object need to be retrieved
 *         from the initiator.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_responder_get_linked_thumbnail(bip_handle, req_info)      \
        bip_responder_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_LINKED_THUMBNAIL_OPERATION)                         \
        )

/**
 *  \brief To retrieve an image from the initiator.
 *
 *  \par Description:
 *       This API retrieves an image object from the requested directory/folder at the connected
 *       Responder entity. The application must include the image handle & image description in the
 *       req_info parameter. On return the initiator responses with image data and file size.
 *
 *       The completion of this procedure is notified through \ref BIP_RESPONDER_SEC_GET_IMAGE_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         Structure containing Image handle and image description details for which the image object
 *         need to be retrieved from the initiator.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_responder_get_image(bip_handle, req_info)                 \
        bip_responder_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_OPERATION)                                    \
        )

/**
 *  \brief To retrieve attachment file of a specific image from the Initiator.
 *
 *  \par Description:
 *       This API retrieves an attachment associated with an image from the requested
 *       directory/folder at the connected initiator entity. The application should include
 *       image handle details and name of the file details for the request using the req_info parameter.
 *
 *       The completion of this procedure is notified through \ref BIP_RESPONDER_SEC_GET_LINKED_ATTACHMENT_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] bip_handle
 *         BIP handle that represents a BIP connection with remote device.
 *
 *  \param [in] req_info
 *         Structure containing Image handle and File name details for which the attachment file
 *         need to be retrieved from the initiator.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h.
 */
#define BT_bip_responder_get_linked_attachment(bip_handle, req_info)     \
        bip_responder_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_LINKED_ATTACHEMENT_OPERATION)                       \
        )

/**
 *  \brief To retrieve an image or part of an image from the Initiator.
 *
 *  \par Description:
 *       This API retrieves either an entire image file or part of an image file, depending on
 *       the requirements of the Responder entity. The application shall include the application
 *       parameters for the request using the bip_req_info parameter. On return, the Initiator will
 *       be send the Image object with file size and end flag in the Application Parameters.
 *
 *       The completion of this procedure is notified through \ref BIP_RESPONDER_SEC_GET_PARTIAL_IMAGE_CNF
 *       event along with the status of the procedure
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *   \param [in] bip_req_info
 *         Structure containing image name and application parameters, which may be included by the
 *         application. The following application parameters are supported as part of this request for
 *         this parameter.
 *         PartialFileLength: Valid range is : 0x00000000 - 0xFFFFFFFF
 *         PartialFileStartOffset: To retrieve entire image set partial_file_len = 0xFFFFFFFF
 *
 *  \param [in] partial_file_start_offset
 *         Specifies the first byte of the file from which the partial file starts
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_responder_get_partial_image
           (
               /* IN */  BIP_HANDLE           *bip_handle,
               /* IN */  BIP_REQUEST_STRUCT   *bip_req_info
           );

/**
 *  \brief To delete an image from BIP Initiator Entity.
 *
 *  \par Description:
 *       This function sends a request for image delete operation on the BIP Initiator.
 *       The completion of this procedure is notified through \ref BIP_RESPONDER_SEC_DELETE_IMAGE_CNF
 *       notification event.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] bip_req_info
 *         This structure contains the image handle of the image to be deleted.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_responder_delete_image
           (
               /* IN */ BIP_HANDLE         *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT *bip_req_info
           );

/**
 *  \brief To send a request to the BIP Initiator.
 *
 *  \par Description:
 *       This API sends the request to the BIP Initiator with the corresponding BIP event and
 *       the data given by the application contained in the BIP_headers.  The application shall
 *       call this API to continue any operation initiated and to which the first notification
 *       has been received. Thus this API shall be called from the BIP Responder application
 *       callback function with the event_type set to the current received event
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback, to indicate that
 *         the request is the continuation of the operation for which it was notified.
 *
 *  \param [in] event_result
 *         This is the status of the request.
 *
 *  \param [in] bip_headers
 *         Structure containing either the connect information in case of connect operation
 *         or get request information, in case of get operation or put request information in
 *         case of put operation.
 *
 *  \param [in] more
 *         Indicates if application has remaining of object body still to be sent or object complete.
 *         1 - Object part remaining
 *         0 - No more object part remaining.
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included in the request,
 *         that is packed for transmission.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BT_error.h
 */
API_RESULT BT_bip_responder_send_request
           (
               /* IN */    BIP_HANDLE   *bip_handle,
               /* IN */    UINT8         event_type,
               /* IN */    UINT16        event_result,
               /* IN */    BIP_HEADERS  *bip_headers,
               /* IN */    UCHAR         more,
               /* OUT */ UINT16         *actual
           );

/**
 *  \brief To send response to the latest request received over Primary channel from the BIP Initiator.
 *
 *  \par Description:
 *       This API is used to send the response to the requests made by the BIP Initiator.
 *       Response to connect, send event etc. shall be sent through this API.
 *
 *       The application shall call this API with the event type of the latest notification received.
 *       This indicates the response is for the latest indication received.
 *       BIP Initiator frames the OBEX specific packets based on the response and associated response
 *       data and send it to the BIP Initiator.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback.
 *         This is to indicate that the response being sent is response to the request for which the
 *         application was notified.
 *
 *  \param [in] event_result
 *         This is the OBEX response code.
 *
 *  \param [in] bip_headers
 *         Structure containing either the connect information, in case of connect operation or the
 *         response data, in case of get/put operation.
 *
 *  \return Result of the operation (Accept or Reject  the event)
 */
API_RESULT BT_bip_responder_send_response
           (
               /* IN */  BIP_HANDLE    * bip_handle,
               /* IN */  UINT8           event_type,
               /* IN */  UCHAR           event_result,
               /* IN */  BIP_HEADERS   * bip_headers
           );

/**
 * Internal function to handle PULL operations
 * Application need not to call this API.
 */
API_RESULT bip_responder_pull_operation
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info,
               /* IN */ UCHAR                 bip_operation_id
           );

#endif /* BIP_RESPONDER */

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_BIP_API_ */

