
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
#define BIP_OBEX_L2CAP_PSM                          0x8801
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
#define BIP_TARGET_HDR_SIZE                         0x10 /* 16 */

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
#define BIP_INITIATOR_PRI_CONNECT_CNF                   0x01
#define BIP_INITIATOR_PRI_DISCONNECT_CNF                0x02
#define BIP_INITIATOR_PRI_TRANSPORT_CLOSE_CNF           0x03
#define BIP_INITIATOR_PRI_TRANSPORT_CLOSE_IND           0x04
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
#define BIP_INITIATOR_SEC_TRANSPORT_CLOSE_CNF           0x05
#define BIP_INITIATOR_SEC_TRANSPORT_CLOSE_IND           0x06
#define BIP_INITIATOR_SEC_CONNECT_IND                   0x07
#define BIP_INITIATOR_SEC_DISCONNECT_IND                0x08

#define BIP_INITIATOR_PRI_GET_CAPABILITIES_CNF          0x09
#define BIP_INITIATOR_PRI_PUT_IMAGE_CNF                 0x0A
#define BIP_INITIATOR_PRI_PUT_LINKED_THUMBNAIL_CNF      0x0B
#define BIP_INITIATOR_PRI_PUT_LINKED_ATTACHMENT_CNF     0x0C
/*@}*/
/* For operation*/
/**
 *  \name BIP Initiator Events for Operation
 *
 *  Constant Defines for Operation \ref BIP_INITIATOR_EVENTS
 *
 */
/*@{*/
#define BIP_INITIATOR_PRI_REMOTE_DISPLAY_CNF            0x0D
#define BIP_INITIATOR_PRI_GET_IMAGE_LIST_CNF            0x0E
#define BIP_INITIATOR_PRI_GET_IMAGE_PROPERTIES_CNF      0x0F
#define BIP_INITIATOR_PRI_GET_IMAGE_CNF                 0x10
#define BIP_INITIATOR_PRI_GET_LINKED_THUMBNAIL_CNF      0x11
#define BIP_INITIATOR_PRI_GET_LINKED_ATTACHMENT_CNF     0x12
#define BIP_INITIATOR_PRI_DELETE_IMAGE_CNF              0x13
#define BIP_INITIATOR_PRI_START_PRINT_CNF               0x14
#define BIP_INITIATOR_PRI_START_ARCHIVE_CNF             0x15
#define BIP_INITIATOR_PRI_GET_PARTIAL_IMAGE_CNF         0x16
#define BIP_INITIATOR_PRI_GET_STATUS_CNF                0x17
#define BIP_INITIATOR_PRI_GET_MONITORING_IMAGE_CNF      0x18

#define BIP_INITIATOR_SEC_GET_CAPABILITIES_IND          0x19
#define BIP_INITIATOR_SEC_PUT_IMAGE_IND                 0x1A
#define BIP_INITIATOR_SEC_PUT_LINKED_THUMBNAIL_IND      0x1B
#define BIP_INITIATOR_SEC_PUT_LINKED_ATTACHMENT_IND     0x1C
#define BIP_INITIATOR_SEC_REMOTE_DISPLAY_IND            0x1D
#define BIP_INITIATOR_SEC_GET_IMAGE_LIST_IND            0x1E
#define BIP_INITIATOR_SEC_GET_IMAGE_PROPERTIES_IND      0x1F
#define BIP_INITIATOR_SEC_GET_IMAGE_IND                 0x20
#define BIP_INITIATOR_SEC_GET_LINKED_THUMBNAIL_IND      0x21
#define BIP_INITIATOR_SEC_GET_LINKED_ATTACHMENT_IND     0x22
#define BIP_INITIATOR_SEC_DELETE_IMAGE_IND              0x23
#define BIP_INITIATOR_SEC_START_PRINT_IND               0x24
#define BIP_INITIATOR_SEC_START_ARCHIVE_IND             0x25
#define BIP_INITIATOR_SEC_GET_PARTIAL_IMAGE_IND         0x26
#define BIP_INITIATOR_SEC_GET_STATUS_IND                0x27
#define BIP_INITIATOR_SEC_GET_MONITORING_IMAGE_IND      0x28
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
#define BIP_RESPONDER_PRI_CONNECT_IND                   0x61
#define BIP_RESPONDER_PRI_DISCONNECT_IND                0x62
#define BIP_RESPONDER_PRI_TRANSPORT_CLOSE_CNF           0x63
#define BIP_RESPONDER_PRI_TRANSPORT_CLOSE_IND           0x64
#define BIP_RESPONDER_SEC_TRANSPORT_CLOSE_CNF           0x65
#define BIP_RESPONDER_SEC_TRANSPORT_CLOSE_IND           0x66
#define BIP_RESPONDER_SEC_CONNECT_CNF                   0x67
#define BIP_RESPONDER_SEC_DISCONNECT_CNF                0x68
/*@}*/
/* For operation*/
/**
 *  \name BIP Responder Events for Operation
 *
 *  Constant Defines for Operation \ref BIP_RESPONDER_EVENTS
 *
 */
/*@{*/
#define BIP_RESPONDER_PRI_GET_CAPABILITIES_IND          0x69
#define BIP_RESPONDER_PRI_PUT_IMAGE_IND                 0x6A
#define BIP_RESPONDER_PRI_PUT_LINKED_THUMBNAIL_IND      0x6B
#define BIP_RESPONDER_PRI_PUT_LINKED_ATTACHMENT_IND     0x6C
#define BIP_RESPONDER_PRI_REMOTE_DISPLAY_IND            0x6D
#define BIP_RESPONDER_PRI_GET_IMAGE_LIST_IND            0x6E
#define BIP_RESPONDER_PRI_GET_IMAGE_PROPERTIES_IND      0x6F
#define BIP_RESPONDER_PRI_GET_IMAGE_IND                 0x70
#define BIP_RESPONDER_PRI_GET_LINKED_THUMBNAIL_IND      0x71
#define BIP_RESPONDER_PRI_GET_LINKED_ATTACHMENT_IND     0x72
#define BIP_RESPONDER_PRI_DELETE_IMAGE_IND              0x73
#define BIP_RESPONDER_PRI_START_PRINT_IND               0x74
#define BIP_RESPONDER_PRI_START_ARCHIVE_IND             0x75
#define BIP_RESPONDER_PRI_GET_PARTIAL_IMAGE_IND         0x76
#define BIP_RESPONDER_PRI_GET_STATUS_IND                0x77
#define BIP_RESPONDER_PRI_GET_MONITORING_IMAGE_IND      0x78

#define BIP_RESPONDER_SEC_GET_CAPABILITIES_CNF          0x79
#define BIP_RESPONDER_SEC_PUT_IMAGE_CNF                 0x7A
#define BIP_RESPONDER_SEC_PUT_LINKED_THUMBNAIL_CNF      0x7B
#define BIP_RESPONDER_SEC_PUT_LINKED_ATTACHMENT_CNF     0x7C
#define BIP_RESPONDER_SEC_REMOTE_DISPLAY_CNF            0x7D
#define BIP_RESPONDER_SEC_GET_IMAGE_LIST_CNF            0x7E
#define BIP_RESPONDER_SEC_GET_IMAGE_PROPERTIES_CNF      0x7F
#define BIP_RESPONDER_SEC_GET_IMAGE_CNF                 0x80
#define BIP_RESPONDER_SEC_GET_LINKED_THUMBNAIL_CNF      0x81
#define BIP_RESPONDER_SEC_GET_LINKED_ATTACHMENT_CNF     0x82
#define BIP_RESPONDER_SEC_DELETE_IMAGE_CNF              0x83
#define BIP_RESPONDER_SEC_START_PRINT_CNF               0x84
#define BIP_RESPONDER_SEC_START_ARCHIVE_CNF             0x85
#define BIP_RESPONDER_SEC_GET_PARTIAL_IMAGE_CNF         0x86
#define BIP_RESPONDER_SEC_GET_STATUS_CNF                0x87
#define BIP_RESPONDER_SEC_GET_MONITORING_IMAGE_CNF      0x88
/*@}*/
/*@}*/

#define BIP_EVENT_INVALID                               0xFF
#define BIP_ENTITY_INVALID                              0xFF

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
#define OBEX_HDR_IMG_HANDLE                         0x30
#define OBEX_HDR_IMG_DESCRIPTOR                     0x71
/*@}*/
#define PRIMARY_CONNECT_PENDING                     0x01
#define SECONDARY_CONNECT_PENDING                   0x02
#define NO_CONNECT_PENDING                          0xFF

/* Application parameters tag ids */
/**
 *  \name Application parameters tag ids
 *
 *  Constant Defines for Application parameters tag ids
 *
 */
/*@{*/
#define BIP_TAG_ID_NB_RETURNED_HANDLES              0x01
#define BIP_TAG_ID_LIST_START_OFFSET                0x02
#define BIP_TAG_ID_LATEST_CAPTURED_IMAGES           0x03
#define BIP_TAG_ID_PARTIAL_FILE_LENGTH              0x04
#define BIP_TAG_ID_PARTIAL_FILE_START_OFFSET        0x05
#define BIP_TAG_ID_TOTAL_FILE_SIZE                  0x06
#define BIP_TAG_ID_END_FLAG                         0x07
#define BIP_TAG_ID_REMOTE_DISPLAY                   0x08
#define BIP_TAG_ID_SERVICE_ID                       0x09
#define BIP_TAG_ID_STORE_FLAG                       0x0A
/*@}*/
/* No. of application headers */
/**
 *  \name BIP application headers count
 *
 *  Constant Defines for Number of application headers
 *
 */
/*@{*/
#define  BIP_NUM_APPL_HEADERS                       0x0A
/*@}*/
/* BIP Application parameters flag corresponding to each Tag */
/**
 *  \name BIP Application parameters flag corresponding to each Tag
 *
 *  Constant Defines for BIP Application parameters flag corresponding to each Tag
 *
 */
/*@{*/
#define BIP_FLAG_NB_RETURNED_HANDLES                0x00000001
#define BIP_FLAG_LIST_START_OFFSET                  0x00000002
#define BIP_FLAG_LATEST_CAPTURED_IMAGES             0x00000004
#define BIP_FLAG_PARTIAL_FILE_LENGTH                0x00000008
#define BIP_FLAG_PARTIAL_FILE_START_OFFSET          0x00000010
#define BIP_FLAG_TOTAL_FILE_SIZE                    0x00000020
#define BIP_FLAG_END_FLAG                           0x00000040
#define BIP_FLAG_REMOTE_DISPLAY                     0x00000080
#define BIP_FLAG_SERVICE_ID                         0x00000100
#define BIP_FLAG_STORE_FLAG                         0x00000200
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
#define BIP_HDR_IMAGE_HANDLE                        0x30
#define BIP_HDR_IMAGE_DESCRIPTOR                    0x71
#define BIP_HDR_REMOTE_BD_ADDR                      0x98
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
#define BIP_IMAGE_PUSH                              0x00
#define BIP_IMAGE_PULL                              0x01
#define BIP_ADVANCED_IMAGE_PRINTING                 0x02
#define BIP_AUTOMATIC_ARCHIVE                       0x03
#define BIP_REMOTE_CAMERA                           0x04
#define BIP_REMOTE_DISPLAY                          0x05
#define BIP_REFERENCED_OBJECTS                      0x06
#define BIP_ARCHIVED_OBJECTS                        0x07
/*@}*/

/* BIP PUSH Feature Operation IDs */
/**
 *  \name BIP PUSH Feature Operation IDs
 *
 *  Constant Defines for BIP PUSH Feature Operation IDs
 *
 */
/*@{*/
#define BIP_PUT_IMAGE_OPERATION                     0x01
#define BIP_PUT_LINKED_THUMBNAIL_OPERATION          0x02
#define BIP_PUT_LINKED_ATTACHMENT_OPERATION         0x03
/*@}*/
/* BIP PULL Feature Operation IDs */
/**
 *  \name BIP PULL Feature Operation IDs
 *
 *  Constant Defines for BIP PULL Feature Operation IDs
 *
 */
/*@{*/
#define BIP_GET_IMAGE_LISTING_OPERATION             0x04
#define BIP_GET_IMAGE_PROPERTIES_OPERATION          0x05
#define BIP_GET_IMAGE_OPERATION                     0x06
#define BIP_GET_LINKED_THUMBNAIL_OPERATION          0x07
#define BIP_GET_LINKED_ATTACHEMENT_OPERATION        0x08
/*@}*/
/* BIP Session Types */
/**
 *  \name BIP Session Types
 *
 *  Constant Defines for BIP Session Types
 *
 */
/*@{*/
#define BIP_SESSION_TYPE_PRIMARY                    0x00
#define BIP_SESSION_TYPE_SECONDARY                  0x01
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
} BIP_RESPONSE_STRUCT;

/* BIP common headers */
typedef union _BIP_HEADERS
{
    BIP_CONNECT_STRUCT      *bip_connect_info; /**< Used for Connect request */

    BIP_REQUEST_STRUCT      *bip_req_info;     /**< Used to send the request */

    BIP_RESPONSE_STRUCT     *bip_rsp_info;     /**< Used to send the response */

} BIP_HEADERS;
/** \} */
/** \} */

/**
 *  \defgroup bip_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
/** BIP Event Notification Callback type */
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
        (hs).length = 0;

/** To set application parameter flag */
#define BIP_SET_APPL_PARAM_FLAG(flag, set_field)   \
        (flag) = (flag) | (set_field);

/** To retrieve the field value (Set/Reset) */
#define BIP_GET_APPL_PARAM_FLAG(flag, get_field)   \
        ((flag) & (get_field))

/** Resets all the fields of the application parameter flag */
#define BIP_RESET_APPL_PARAM_FLAG(flag)            \
        (flag) = 0x00000000

/** Extract header structure value */
#define BIP_COPY_HEADER_STRUCT(hdr, len, val)      \
        (len) = (hdr)->length;                     \
        BT_str_n_copy((val), (hdr)->value, (len)); \
        (val)[(len)] = '\0';

/** Extract application parameters */
#define BIP_COPY_APPL_PARAMS(to_params, from_params, search_value_out)      \
        BT_mem_copy((to_params), (from_params), sizeof(BIP_APPL_PARAMS));   \
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

/* === BIP Initiator API ==== */

API_RESULT BT_bip_initiator_init(void);

API_RESULT BT_bip_initiator_shutdown(void);

API_RESULT BT_bip_initiator_start
           (
               /* OUT */ BIP_HANDLE       * bip_handle,
               /* IN */  BIP_EVENT_NTF_CB   bip_initiator_cb,
               /* IN */  UCHAR              bip_feature_id,
               /* IN */  UCHAR              bip_session_type
           );

API_RESULT BT_bip_initiator_stop
           (
               /* IN */ BIP_HANDLE        *bip_handle,
               /* IN */ UCHAR              bip_session_type
           );

/**
 *  \brief To establish BIP connection.
 *
 *  \par Description:
 *       This API issues request for BIP connection. When connection is
 *       established, application is notified with BIP_CONNECT_CNF event.
 *       The BIP connection will be established for a specific feature. 
 *       User must not start any operation that is not part of the feature 
 *       for which connection is established.
 *
 *  \param [in] bip_handle
 *         Valid BIP Handle
 *
 *  \param [in] bip_connect
 *         This is a structure which contains parameters required for connection.
 *         The parameter decided in this parameter are:
 *         bd_addr - Bluetooth Device Address of the remote device. 
 *         User must get the remote device Bluetooth Device Address using the 
 *         inquiry procedure. 
 *         server_channel - The RFCOMM server channel. User must get the remote 
 *         device RFCOMM server channel using SDP query.
 *         bip_feature - The feature for which connection is established 
 *         (BIP_IMAGE_PUSH, BIP_IMAGE_PULL, BIP_AUTOMATIC_ARCHIVE,
 *         BIP_REMOTE_CAMERA, BIP_REMOTE_DISPLAY)
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
 *  \brief To disconnect to the BIP connection.
 *
 *  \par Description:
 *       This API initiates the disconnection of the BIP connection.
 *
 *  \param [in] bip_handle 
 *         BIP Handle that represents a BIP connection with remote device.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_initiator_disconnect
           (
               /* IN */ BIP_HANDLE *bip_handle
           );

/**
 *  \brief To get the imaging capabilities of remote BIP Responder.
 *
 *  \par Description:
 *       This API issues a request to get the imaging capabilities of remote BIP
 *       Responder. When response of this request is received, the application
 *       is notified using BIP_GET_CAPABILITIES_CNF event. 
 *
 *  \param [in] bip_handle 
 *         BIP handle that represents a BIP connection with remote device.
 *         
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_initiator_get_capabilities
           (
               /* IN */ BIP_HANDLE  *bip_handle
           );

/**
 *  \brief API to get the status of secondary OBEX session.
 *
 *  \par Description:
 *       This API is used to get the status of the secondary OBEX session. This 
 *       is used to monitor a secondary session. Primary OBEX exchange client 
 *       issues GetStatus request. The response of this request is notified 
 *       using BIP_GET_STATUS_CNF event.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_initiator_get_status
           (
               /* IN */ BIP_HANDLE  *bip_handle
           );

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
 *  \brief To send the response of image operations request.
 *
 *  \par Description:
 *       This API is used to send the response of a request in the Initiator 
 *       Role. The response of a request is given using this API only. 
 *       If request is accepted, the result must be set to API_SUCCESS.
 *       If request is rejected, the result should be set to API_FAILURE. 
 *
 *  \param [in] bip_handle 
 *         BIP Handle that represents a BIP connection. 
 *
 *  \param [in] resp
 *         It contains information specific to event given to the application.
 *
 *  \param [in] result 
 *         Result of the operation (Accept or Reject the event)  
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_initiator_send_response
           (
               /* IN */  BIP_HANDLE    * bip_handle,
               /* IN */  UINT8           event_type,
               /* IN */  UCHAR           event_result,
               /* IN */  BIP_HEADERS   * bip_headers
           );

/**
 *  \brief To delete an image from BIP Responder.
 *
 *  \par Description:
 *       This API requests BIP Responder to delete an image. The response of 
 *       this request is notified using the event BIP_DELETE_IMAGE_CNF. 
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] image_handle 
 *         Handle of the image to be deleted.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_initiator_delete_image
           (
               /* IN */ BIP_HANDLE         *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT *bip_req_info
           );

/**
 *  \brief API to retrieve monitoring images from BIP Responder.
 *
 *  \par Description:
 *       This API is used to retrieve monitoring images from an OBEX server with
 *       image capturing capability. Monitoring images are retrieved in the
 *       monitoring-image format. The response of this request is notified using\
 *       BIP_GET_MONIT_IMAGE_CNF event.
 *
 *  \param [in] bip_handle 
 *         BIP Handle that represents a BIP connection
 *
 *  \param [in] file_handle 
 *         Handle to write the received monitoring image data.
 *
 *  \param [in] store_flag 
 *         Store flag. Following values are allowed:
 *         0x00 - Server should not store the full size image
 *         0x01 - Server should store the full size image, any other value is illegal.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_initiator_get_monitoring_image
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info
           );

/**
 *  \brief API to pilot the screen of a BIP Responder with display capability
 *
 *  \par Description:
 *       This API is used to pilot the screen of a BIP Responder with display capability. 
 *       The screen control commands are: 
 *          NextImage (display the next image)
 *          PreviousImage (display the previous image)
 *          SelectImage (display a specific image)
 *          CurrentImage (retrieve the handle of the currently displayed image)
 *       The response of this request is notified using BIP_REMOTE_DISPLAY_CNF event.
 *
 *  \param [in] bip_handle 
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] image_handle 
 *         Handle of the image to be displayed. If SelectImage value is used for
 *         remote_display, image_handle must be present. For other values of
 *         remote_display, image_handle must be set to NULL.
 *
 *  \param [in] remote_display 
 *         Command to specify the image to be displayed. 
 *         The allowed values are:
 *         0x01 : NextImage 
 *         0x02 : PreviousImage
 *         0x03 : SelectImage
 *         0x04 : Current image         
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_initiator_remote_display
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info
           );

/**
 *  \brief API to trigger a BIP Responder with print capability to execute a print job
 *
 *  \par Description:
 *       This API is used to trigger a BIP Responder with print capability to
 *       execute a print job. The print job is described in the printer-control 
 *       object sent to BIP Responder. As a result of StartPrint request, 
 *       BIP Responder opens a new OBEX connection to the referenced objects
 *       service of BIP Initiator. The response of this request is notified 
 *       using BIP_START_PRINT_CNF event.
 *
 *  \param [in] bip_handle 
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] ref_obj_cap 
 *         Referenced object service capabilities to be registered with SDP. 
 *         It also contains the serviceID.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_initiator_start_print
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info,
               /* IN */  UCHAR                 more,
               /* OUT */ UINT16               *actual
           );

/**
 *  \brief API to trigger a BIP Responder to start archiving the image files
 *
 *  \par Description:
 *       This API is used to trigger a BIP Responder to start archiving the 
 *       image files from BIP Initiator. As a result of StartArchive request,
 *       BIP Responder opens a new OBEX connection to the Automatic Archive 
 *       service of BIP Initiator. The response of this request is notified 
 *       using BIP_START_ARCHIVE_CNF event.
 *
 *  \param [in] bip_handle
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] arch_obj_cap 
 *         Automatic archive service capabilities to be registered with SDP. 
 *         It also contains the serviceID
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_initiator_start_archive
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info
           );

#define BT_bip_initiator_push_image(bip_handle, req_info, more, actual)  \
        bip_initiator_push_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_PUT_IMAGE_OPERATION),                                   \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bip_initiator_put_linked_thumbnail(bip_handle, req_info, more, actual)\
        bip_initiator_push_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_PUT_LINKED_THUMBNAIL_OPERATION),                        \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bip_initiator_put_linked_attachment(bip_handle, req_info, more, actual)\
        bip_initiator_push_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_PUT_LINKED_ATTACHMENT_OPERATION),                       \
            (more),                                                      \
            (actual)                                                     \
        )

#define BT_bip_initiator_get_image_listing(bip_handle, req_info)         \
        bip_initiator_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_LISTING_OPERATION)                            \
        )

#define BT_bip_initiator_get_image_properties(bip_handle, req_info)      \
        bip_initiator_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_PROPERTIES_OPERATION)                         \
        )

#define BT_bip_initiator_get_linked_thumbnail(bip_handle, req_info)      \
        bip_initiator_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_LINKED_THUMBNAIL_OPERATION)                         \
        )

#define BT_bip_initiator_get_image(bip_handle, req_info)                 \
        bip_initiator_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_OPERATION)                                    \
        )

#define BT_bip_initiator_get_linked_attachment(bip_handle, req_info)     \
        bip_initiator_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_LINKED_ATTACHEMENT_OPERATION)                       \
        )

/* BIP API to handle PUSH operations */
API_RESULT bip_initiator_push_operation
           (
               /* IN */  BIP_HANDLE           *bip_handle,
               /* IN */  BIP_REQUEST_STRUCT   *bip_req_info,
               /* IN */  UCHAR                 bip_operation_id,
               /* IN */  UCHAR                 more,
               /* OUT */ UINT16               *actual
           );

API_RESULT bip_initiator_pull_operation
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info,
               /* IN */ UCHAR                 bip_operation_id
           );

API_RESULT BT_bip_initiator_pri_transport_close
           (
               /* IN */    BIP_HANDLE   *bip_handle
           );

API_RESULT BT_bip_initiator_sec_transport_close
           (
               /* IN */    BIP_HANDLE   *bip_handle
           );

/* === BIP Responder API ==== */

API_RESULT BT_bip_responder_init(void);

API_RESULT BT_bip_responder_shutdown(void);

API_RESULT BT_bip_responder_start
           (
               /* OUT */ BIP_HANDLE        *bip_handle,
               /* IN */  BIP_EVENT_NTF_CB   bip_responder_cb,
               /* IN */  UCHAR              bip_feature_id,
               /* IN */  UCHAR              bip_session_type
           );

API_RESULT BT_bip_responder_get_capabilities
           (
               /* IN */ BIP_HANDLE  *bip_handle
           );

/**
 *  \brief To get the imaging capabilities of remote BIP Initiator.
 *
 *  \par Description:
 *       This API issues a request to get the imaging capabilities of remote BIP
         Initiator. When response of this request is received, the application 
         is notified using BIP_GET_CAPABILITIES_CNF event. 
 *
 *  \param [in] bip_handle 
 *         BIP handle that represents a BIP connection with remote device.
 *
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_responder_stop
           (
               /* IN */ BIP_HANDLE        *bip_handle,
               /* IN */ UCHAR              bip_session_type
           );

/**
 *  \brief To send the response of image operations request.
 *
 *  \par Description:
 *       This API is used to send the response of a request in the Responder 
         Role. The response of a request is given using this API only. 
         If request is accepted, the result must be set to API_SUCCESS.
         If request is rejected, the result should be set to API_FAILURE. 
 *
 *  \param [in] bip_handle 
 *         BIP Handle that represents a BIP connection. 
 *
 *  \param [in] resp 
 *         It contains information specific to event given to the application.
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

API_RESULT BT_bip_responder_pri_transport_close
           (
               /* IN */ BIP_HANDLE   *bip_handle
           );

API_RESULT BT_bip_responder_sec_transport_close
           (
               /* IN */ BIP_HANDLE   *bip_handle
           );

/**
 *  \brief To establish BIP connection.
 *
 *  \par Description:
 *       This API issues request for BIP connection. When connection is 
 *       established, application is notified with BIP_CONNECT_CNF event. 
 *       The BIP connection will be established for a specific feature. 
 *       User must not start any operation that is not part of the feature 
 *       for which connection is established. 
 *
 *  \param [in] bip_handle 
 *         Valid BIP Handle.
 *
 *  \param [in] 
 *         This is a structure which contains parameters required for connection.
 *         The parameter decided in this parameter are:
 *         bd_addr - Bluetooth Device Address of the remote device. 
 *         User must get the remote device Bluetooth Device Address using the
 *         inquiry procedure.
 *         server_channel - The RFCOMM server channel. 
 *         User must get the remote device RFCOMM server channel using SDP query.
 *         bip_feature - The feature for which connection is established 
 *         (BIP_IMAGE_PUSH, BIP_IMAGE_PULL, BIP_AUTOMATIC_ARCHIVE,
 *         BIP_REMOTE_CAMERA, BIP_REMOTE_DISPLAY)
 *         bip_mtu - The maximum packet size the application can receive.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_responder_connect
           (
               /* IN */ BIP_HANDLE          * bip_handle,
               /* IN */ BIP_CONNECT_STRUCT  * bip_connect_info
           );

API_RESULT BT_bip_responder_send_request
           (
               /* IN */    BIP_HANDLE   *bip_handle,
               /* IN */    UINT8         event_type,
               /* IN */    UINT16        event_result,
               /* IN */    BIP_HEADERS  *bip_headers,
               /* IN */    UCHAR         more,
               /* OUT */ UINT16         *actual
           );

API_RESULT BT_bip_responder_disconnect
           (
               /* IN */ BIP_HANDLE *bip_handle
           );

/**
 *  \brief API to retrieve partial image - used to get images for printing
 *
 *  \par Description:
 *       This API is used to retrieve either an entire image file or part of an 
 *       image file, depending on the requirement of OBEX server. Images are 
 *       referenced using their path and image file name. The response of this 
 *       request is notified using BIP_GET_PARTIAL_IMAGE_CNF event.
 *
 *  \param [in] bip_handle 
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] image_name
 *         Path and name of the image file to be retrieved.
 *
 *  \param [in] file_handle 
 *         Handle to write the imaging data.
 *
 *  \param [in] partial_file_len 
 *         Length of the partial file returned in response 
 *         Valid range is : 0x00000000 - 0xFFFFFFFF
 *         To retrieve entire image set partial_file_len = 0xFFFFFFFF;
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
 *  \brief To delete an image from BIP Initiator.
 *
 *  \par Description:
 *       This API requests BIP Initiator to delete an image. The response of 
 *       this request is notified using the event BIP_DELETE_IMAGE_CNF. 
 *
 *  \param [in] bip_handle 
 *         BIP Handle that represents a BIP connection.
 *
 *  \param [in] image_handle 
 *         Handle of the image to be deleted.
 *
 *  \return API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                      \ref BLE_ERROR_CODES.
 */
API_RESULT BT_bip_responder_delete_image
           (
               /* IN */ BIP_HANDLE         *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT *bip_req_info
           );

#define BT_bip_responder_get_image_listing(bip_handle, req_info)         \
        bip_responder_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_LISTING_OPERATION)                            \
        )

#define BT_bip_responder_get_image_properties(bip_handle, req_info)      \
        bip_responder_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_PROPERTIES_OPERATION)                         \
        )

#define BT_bip_responder_get_linked_thumbnail(bip_handle, req_info)      \
        bip_responder_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_LINKED_THUMBNAIL_OPERATION)                         \
        )

#define BT_bip_responder_get_image(bip_handle, req_info)                 \
        bip_responder_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_IMAGE_OPERATION)                                    \
        )

#define BT_bip_responder_get_linked_attachment(bip_handle, req_info)     \
        bip_responder_pull_operation                                     \
        (                                                                \
            (bip_handle),                                                \
            (req_info),                                                  \
            (BIP_GET_LINKED_ATTACHEMENT_OPERATION)                       \
        )

API_RESULT bip_responder_pull_operation
           (
               /* IN */ BIP_HANDLE           *bip_handle,
               /* IN */ BIP_REQUEST_STRUCT   *bip_req_info,
               /* IN */ UCHAR                 bip_operation_id
           );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_BIP_API_ */

