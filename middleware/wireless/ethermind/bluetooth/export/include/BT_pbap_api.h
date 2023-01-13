
/**
 *  \file BT_pbap_api.h
 *
 *  \brief EtherMind PBAP API Header File.
 *
 *  This Header File contains declaration of PBAP APIs, Macros and
 *  Constant Definitions for use in upper layer applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_PBAP_API_
#define _H_BT_PBAP_API_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_obex_api.h"

/* ----------------------------------------- Global Definitions */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 *  \defgroup  pbap_module PBAP (Phone Book Access Profile)
 *  \{
 *
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Phone Book Access Profile module to the Application and other upper
 *  layers of the stack.
 */
/**
 * \defgroup pbap_defines Defines
 * \{
 * Describes defines for the module.
 */

/**
 * \defgroup pbap_constants Constants
 * \{
 * Describes Constants defined by the module.
 */

#ifdef PBAP_1_2
#define PBAP_OBEX_L2CAP_PSM                       0x8801U
#endif /* PBAP_1_2 */

/**
 *  PBAP Event Notification Callback Event Types
 */
#define PBAP_PCE_CONNECT_CFM                      0x01U
#define PBAP_PCE_GET_PHONEBOOK_CFM                0x02U
#define PBAP_PCE_GET_VCARD_LIST_CFM               0x03U
#define PBAP_PCE_GET_VCARD_CFM                    0x04U
#define PBAP_PCE_SET_PHONEBOOK_CFM                0x05U
#define PBAP_PCE_ABORT_CFM                        0x06U
#define PBAP_PCE_DISCONNECT_CFM                   0x07U
#define PBAP_PCE_TRANSPORT_CLOSE_IND              0x08U
#define PBAP_PCE_TRANSPORT_CLOSE_CFM              0x09U
#define PBAP_PCE_IDLE_TIMEOUT_IND                 0x0AU

#define PBAP_PSE_CONNECT_IND                      0x11U
#define PBAP_PSE_GET_PHONEBOOK_IND                0x12U
#define PBAP_PSE_GET_VCARD_LIST_IND               0x13U
#define PBAP_PSE_GET_VCARD_IND                    0x14U
#define PBAP_PSE_SET_PHONEBOOK_IND                0x15U
#define PBAP_PSE_ABORT_IND                        0x16U
#define PBAP_PSE_DISCONNECT_IND                   0x17U
#define PBAP_PSE_TRANSPORT_CLOSE_IND              0x18U
#define PBAP_PSE_TRANSPORT_CLOSE_CFM              0x19U
#define PBAP_PSE_IDLE_TIMEOUT_IND                 0x1AU

#define PBAP_PCE_ERROR_IND                        0x30U
#define PBAP_PSE_ERROR_IND                        0x31U


/**
 *  Results of the operation (OBEX Response codes)
 */
#define PBAP_SUCCESS_RSP                          OBEX_SUCCESS_RSP
#define PBAP_CONTINUE_RSP                         OBEX_CONTINUE_RSP
#define PBAP_BAD_REQ_RSP                          OBEX_REQ_IS_BAD_RSP
#define PBAP_NOT_IMPLEMENTED_RSP                  OBEX_NOT_IMP_RSP
#define PBAP_UNAUTH_RSP                           OBEX_UNAUTHORIZED_RSP
#define PBAP_PRECOND_FAILED_RSP                   OBEX_PRECOND_FAIL_RSP
#define PBAP_NOT_FOUND_RSP                        OBEX_NOT_FOUND_RSP
#define PBAP_NOT_ACCEPTABLE_RSP                   OBEX_NOT_ACCEPTABLE_RSP
#define PBAP_NO_SERVICE_RSP                       OBEX_NO_SERVICE_RSP
#define PBAP_FORBIDDEN_RSP                        OBEX_FORBIDDEN_RSP

/** Maximum number of Application Headers */
#ifdef PBAP_1_2
#define PBAP_MAX_NUM_APPL_HDRS                    0x10U /* 16 */
#else
#define PBAP_MAX_NUM_APPL_HDRS                    0x09U /* 9 */
#endif /* PBAP_1_2 */

/** Maximum OBEX PIN SIZE supported */
#define PBAP_OBEX_MAX_PIN_SIZE                    0x10U /* 16 */

/** Authentication Challenge/ Response Size */
#define PBAP_OBEX_AUTH_CHALLENGE_RESP_SIZE        0x10U /* 16 */

/** PBAP Target Header Size */
#define PBAP_TARGET_HDR_SIZE                      0x10U /* 16 */

/* ==== PBAPv1.2 Related Definitions =========  */

/** PBAP Version Counter size */
#define PBAP_FLDR_VER_CNTR_SIZE                   0x10U /* 16 */

/** PBAP vCARD Selector size */
#define PBAP_VCARD_SELECTOR_SIZE                  0x08U /* 8 */

/** PBAP Database Identifiers size */
#define PBAP_DATABASE_IDENTIFIER_SIZE             0x10U /* 16 */

/** PBAP Supported Features size */
#define PBAP_FEATURES_SIZE                        0x04U /* 4 */

/**
 *  Search Attributes
 */
#define PBAP_SEARCH_ATTR_NAME                     0x00U
#define PBAP_SEARCH_ATTR_NUMBER                   0x01U
#define PBAP_SEARCH_ATTR_SOUND                    0x02U

/**
 *  Application parameters Tag IDs
 */
#define PBAP_TAG_ID_ORDER                             0x01U /* 1  Byte Length  */
#define PBAP_TAG_ID_SEARCH_VALUE                      0x02U /* Variable Length */
#define PBAP_TAG_ID_SEARCH_ATTRIBUTE                  0x03U /* 1  Byte Length  */
#define PBAP_TAG_ID_MAX_LIST_COUNT                    0x04U /* 2       "       */
#define PBAP_TAG_ID_LIST_START_OFFSET                 0x05U /* 2       "       */
#define PBAP_TAG_ID_FILTER                            0x06U /* 8       "       */
#define PBAP_TAG_ID_FORMAT                            0x07U /* 1       "       */
#define PBAP_TAG_ID_PHONE_BOOK_SIZE                   0x08U /* 2       "       */
#define PBAP_TAG_ID_NEW_MISSED_CALLS                  0x09U /* 1       "       */

/** PBAPv1.2 Related TAG ID's */
#define PBAP_TAG_ID_PRIMARY_FOLDER_VERSION            0x0AU /* 16 Byte Length */
#define PBAP_TAG_ID_SECONDARY_FOLDER_VERSION          0x0BU /* 16     "       */
#define PBAP_TAG_ID_VCARD_SELECTOR                    0x0CU /* 8      "       */
#define PBAP_TAG_ID_DATABASE_IDENTIFIER               0x0DU /* 16     "       */
#define PBAP_TAG_ID_VCARD_SELECTOR_OPERATOR           0x0EU /* 1      "       */
#define PBAP_TAG_ID_RESET_NEW_MISSED_CALLS            0x0FU /* 1      "       */
#define PBAP_TAG_ID_PBAP_SUPPORTED_FEATURES           0x10U /* 4      "       */

/**
 *  Application parameters flags corresponding to each Tag
 */
#define PBAP_FLAG_ORDER                           0x0001U
#define PBAP_FLAG_SEARCH_VALUE                    0x0002U
#define PBAP_FLAG_SEARCH_ATTRIBUTE                0x0004U
#define PBAP_FLAG_MAX_LIST_COUNT                  0x0008U
#define PBAP_FLAG_LIST_START_OFFSET               0x0010U
#define PBAP_FLAG_FILTER                          0x0020U
#define PBAP_FLAG_FORMAT                          0x0040U
#define PBAP_FLAG_PHONE_BOOK_SIZE                 0x0080U
#define PBAP_FLAG_NEW_MISSED_CALLS                0x0100U

/** PBAP v1.2 related application flags */
#define PBAP_FLAG_PRIMARY_FOLDER_VERSION          0x0200U
#define PBAP_FLAG_SECONDARY_FOLDER_VERSION        0x0400U
#define PBAP_FLAG_VCARD_SELECTOR                  0x0800U
#define PBAP_FLAG_DATABASE_IDENTIFIER             0x1000U
#define PBAP_FLAG_VCARD_SELECTOR_OPERATOR         0x2000U
#define PBAP_FLAG_RESET_NEW_MISSED_CALLS          0x4000U
#define PBAP_FLAG_SUPPORTED_FEATURES              0x8000U

/**
 *  Attribute Mask: used in case of Filtering
 */
#define PBAP_FLAG_VCARD_VERSION                   0x00000001U
#define PBAP_FLAG_FN                              0x00000002U
#define PBAP_FLAG_N                               0x00000004U
#define PBAP_FLAG_PHOTO                           0x00000008U
#define PBAP_FLAG_BDAY                            0x00000010U
#define PBAP_FLAG_ADR                             0x00000020U
#define PBAP_FLAG_LABEL                           0x00000040U
#define PBAP_FLAG_TEL                             0x00000080U
#define PBAP_FLAG_EMAIL                           0x00000100U
#define PBAP_FLAG_MAILER                          0x00000200U
#define PBAP_FLAG_TZ                              0x00000400U
#define PBAP_FLAG_GEO                             0x00000800U
#define PBAP_FLAG_TITLE                           0x00001000U
#define PBAP_FLAG_ROLE                            0x00002000U
#define PBAP_FLAG_LOGO                            0x00004000U
#define PBAP_FLAG_AGENT                           0x00008000U
#define PBAP_FLAG_ORG                             0x00010000U
#define PBAP_FLAG_NOTE                            0x00020000U
#define PBAP_FLAG_REV                             0x00040000U
#define PBAP_FLAG_SOUND                           0x00080000U
#define PBAP_FLAG_URL                             0x00100000U
#define PBAP_FLAG_UID                             0x00200000U
#define PBAP_FLAG_KEY                             0x00400000U
#define PBAP_FLAG_NICKNAME                        0x00800000U
#define PBAP_FLAG_CATEGORIES                      0x01000000U
#define PBAP_FLAG_PROID                           0x02000000U
#define PBAP_FLAG_CLASS                           0x04000000U
#define PBAP_FLAG_SORTSTRING                      0x08000000U
#define PBAP_FLAG_X_IRMC_CALL_DATETIME            0x10000000U
#define PBAP_FLAG_X_BT_SPEEDDIALKEY               0x20000000U
#define PBAP_FLAG_X_BT_UCI                        0x40000000U
#define PBAP_FLAG_X_BT_UID                        0x80000000U

/**
 *  SET PATH Flags
 */
#define PBAP_SET_CHILD_FOLDER                     0x01U
#define PBAP_SET_PARENT_FOLDER                    0x02U
#define PBAP_SET_ROOT_FOLDER                      0x03U
/** \} */
/** \} */

/* ----------------------------------------- Macros  */
/**
 *  \defgroup pbap_marcos Utility Macros
 *  \{
 *  Initialization and other Utility Macros offered by the module.
 */
/**
 * To set application parameter flag
 */
#define PBAP_SET_APPL_PARAM_FLAG(flag, set_field)   \
        (flag) = (flag) | (set_field)

/**
 * To retrieve the field value (Set/Reset)
 */
#define PBAP_GET_APPL_PARAM_FLAG(flag, get_field)   \
        ((flag) & (get_field))

/**
 * Resets all the fields of the application parameter flag
 */
#define PBAP_RESET_APPL_PARAM_FLAG(flag)            \
        (flag) = 0x00U

/**
 * To set the filter flag
 */
#define PBAP_SET_FILTER_FLAG(flag, set_field)                                    \
        (((set_field) < 0x00010000U)? (((flag)[0U]) = (((flag)[0U]) | (set_field))):\
        (((flag)[1U]) = (((flag)[1U]) | ((set_field)>>16U))))

/**
 * To retrieve the field value (Set/Reset)
 */
#define PBAP_GET_FILTER_FLAG(flag, get_field)                     \
        (((get_field) < 0x00010000U) ? (((flag)[0U]) & (get_field)) :  \
        (((flag)[1U]) & ((get_field)>>16U)))

/**
 * Resets all the fields of the filter flag
 */
#define PBAP_RESET_FILTER_FLAG(flag)            \
        *((flag) + 0U) = 0x00U;                   \
        *((flag) + 1U) = 0x00U;                   \
        *((flag) + 2U) = 0x00U;                   \
        *((flag) + 3U) = 0x00U

/**
 * To update the header value and lenth fields
 */
#define PBAP_COPY_HEADER_STRUCT(hdr, len, val)  \
        (len) = (hdr)->length;                  \
        BT_str_n_copy((val), (hdr)->value, (len));    \
        (val)[(len)] = '\0'

/**
 * To copy the application parameter
 */
#define PBAP_COPY_APPL_PARAMS(to_params, from_params, search_value_out)     \
        BT_mem_set((to_params), 0, sizeof(PBAP_APPL_PARAMS));              \
        BT_mem_copy((to_params), (from_params), sizeof(PBAP_APPL_PARAMS));  \
        if((from_params)->search_value.length != 0U &&                      \
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

/* ----------------------------------------- Structures/Data Types */
/**
 *  \addtogroup pbap_defines Defines
 *  \{
 */

/**
 *  \addtogroup pbap_structures Structures
 *  \{
 */

/**
 *  \typedef PBAP_HANDLE
 *
 *  \brief  This indicates the index of array that holds instance specific
 *          data structures for PBAP PCE/PSE entity.
 */
typedef UINT8 PBAP_HANDLE;

/**
 *  \struct PBAP_HEADER_STRUCT
 *  \brief This structure is used to exchange information with the
 *         application.
 *
 */
typedef struct _PBAP_HEADER_STRUCT
{
    /**
     * pointer to the header value
     */
    UCHAR   *value;

    /**
     * length of the header value
     */
    UINT16  length;
}PBAP_HEADER_STRUCT;

/**
 *  This is a null terminated string : 8 digit Hexa-decimal String.
 *  1 extra byte is for '\0' - Null termination
 */
typedef UCHAR PBAP_VCARD_HANDLE[9];

/**
 *  \struct PBAP_APPL_PARAMS
 *  \brief
 */
typedef struct _PBAP_APPL_PARAMS
{
    /**
     * Attribute Mask
     */
    UINT16               filter[4U];

    /**
     * Maximum entries in the list
     */
    UINT16               max_list_count;

    /**
     * Offset of the first entry
     */
    UINT16               list_start_offset;

    /**
     * Number of indexes in phonebook
     */
    UINT16               phonebook_size;

     /**
      * Application Parameter Bit field
      */

    /**
     *  16 Bit Flag used as below
     *
     *  Usage:
     *       15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
     *      [  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ]
     *        |  |  |  |  |  | |  |   |  |  |  |  |  |  |  |____Order
     *        |  |  |  |  |  | |  |   |  |  |  |  |  |  |_______Search Value
     *        |  |  |  |  |  | |  |   |  |  |  |  |  |__________Search Attribute
     *        |  |  |  |  |  | |  |   |  |  |  |  |_____________Max List Count
     *        |  |  |  |  |  | |  |   |  |  |  |________________List Start Offset
     *        |  |  |  |  |  | |  |   |  |  |___________________Filter
     *        |  |  |  |  |  | |  |   |  |______________________Format
     *        |  |  |  |  |  | |  |   |_________________________Phone book size
     *        |  |  |  |  |  | |  |_____________________________New Missed Calls
     *        |  |  |  |  |  | |________________________________PrimaryVersionCounter
     *        |  |  |  |  |  |__________________________________SecondaryVersionCounter
     *        |  |  |  |  |_____________________________________vCardSelector
     *        |  |  |  |________________________________________DatabaseIdentifier
     *        |  |  |___________________________________________vCardSelectorOperator
     *        |  |______________________________________________ResetNewMissedCalls
     *        |_________________________________________________PbapSupportedFeatures
     *
     *
     */
    UINT16               appl_param_flag;

    /**
     * Sorting Order
     */
    UINT8                order;

    /**
     * Attribute to be searched
     */
    UINT8                search_attr;

    /**
     * vCard Format 2.1 or 3.0
     */
    UINT8                format;

    /**
     * Number of new missed calls
     */
    UINT8                new_missed_calls;

#ifdef PBAP_1_2
    /**
     * PSE primary folder version
     */
    UCHAR      primary_folder_ver[PBAP_FLDR_VER_CNTR_SIZE];

    /**
     * PSE secodary folder version
     */
    UCHAR      secondary_folder_ver[PBAP_FLDR_VER_CNTR_SIZE];

    /**
     * PSE database identifier
     */
    UCHAR      database_identifier[PBAP_DATABASE_IDENTIFIER_SIZE];

    /**
     * vCard Selector, Attribute mask
     */
    UINT16     vcard_selector[4U];

    /**
     * vCard selector operator when multiple
     * attributes are slector
     */
    UINT8      vcard_selector_operator;

    /**
     * To reset newmissed calls
     */
    UINT8      reset_new_missed_calls;

    /**
     * PBAP supported features
     */
    UINT32     supported_features;
#endif /* PBAP_1_2 */

    /**
     * Value of the Search attribute
     */
    PBAP_HEADER_STRUCT   search_value;
}PBAP_APPL_PARAMS;

/**
 *  \struct PBAP_CONNECT_STRUCT
 *  \brief
 */
typedef struct _PBAP_CONNECT_STRUCT
{
    /**
     * OBEX Pin Code
     */
    PBAP_HEADER_STRUCT  * pin_info;

    /**
     * Corresponding User ID
     */
    PBAP_HEADER_STRUCT  * user_id;

    /**
     * Pointer to Bluetooth Address
     */
    UCHAR               * bd_addr;

    /**
     * Maximum Receive Size
     */
    UINT16                max_recv_size;

    /**
     * RFCOMM server channel
     */
    UINT8                 server_ch_no;

    /**
     * Authentication required or not
     */
    UINT8                 auth_flag;

#ifdef PBAP_1_2
    /**
     * L2CAP psm
     */
    UINT16                psm;

    /**
     * PBAP supported features
     */
    UINT32                pbap_supported_features;
#endif /* PBAP_1_2 */
}PBAP_CONNECT_STRUCT;

/**
 *  \struct PBAP_REQUEST_STRUCT
 *  \brief
 */
typedef struct _PBAP_REQUEST_STRUCT
{
    /**
     * OBEX Pin Code
     */
    PBAP_HEADER_STRUCT   * pin_info;

    /**
     * Corresponding User ID
     */
    PBAP_HEADER_STRUCT   * user_id;

    /**
     * Name/Handle of vCard/Phonebook
     */
    PBAP_HEADER_STRUCT   * name;

    /**
     * Application Parameters
     */
    PBAP_APPL_PARAMS     * appl_params;

    /**
     * Authentication required or not
     */
    UINT8               auth_flag;

    /**
     * Set path flag
     */
    UCHAR               setpath_flag;

    /**
     * Wait specifier during SRM transaction
     */
    UCHAR               wait;
}PBAP_REQUEST_STRUCT;

/**
 *  \struct PBAP_RESPONSE_STRUCT
 *  \brief
 */
typedef struct _PBAP_RESPONSE_STRUCT
{
    /**
     * OBEX Pin Code
     */
    PBAP_HEADER_STRUCT  * pin_info;

    /**
     * Corresponding User ID
     */
    PBAP_HEADER_STRUCT  * user_id;

    /**
     * Phonebook/vCard contents
     */
    PBAP_HEADER_STRUCT  * body;

    /**
     * Application Parameters
     */
    PBAP_APPL_PARAMS    * appl_params;

    /**
     * Authentication required or not
     */
    UINT8               auth_flag;

    /**
     * Wait specifier during SRM transaction
     */
    UCHAR               wait;
}PBAP_RESPONSE_STRUCT;

/**
 *  \struct PBAP_HEADERS
 *  \brief
 */
typedef union _PBAP_HEADERS
{
    /**
     * Used to send the request
     */
    PBAP_REQUEST_STRUCT   * pbap_req_info;

    /**
     * Used to send the response
     */
    PBAP_RESPONSE_STRUCT  * pbap_resp_info;

    /**
     * Used for Connect request
     */
    PBAP_CONNECT_STRUCT   * pbap_connect_info;
}PBAP_HEADERS;
/** \} */
/** \} */

/**
 *  \defgroup pbap_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */

/**
 *  \typedef PBAP_EVENT_NOTIFY_CB
 *
 *  \brief   PBAP Event Notification Callback
 *
 *  This callback is called by PBAP to report various events generated for the
 *  requests made by application.
 *
 *  \li    To confirm result of an operation initiated by the upper layer
 *         application/profile.
 *
 *  \li    To inform upper layer application/profile of the asynchronous events.
 *
 *  \param [in] event_type
 *         This parameter describes the PBAP Event being reported.
 *         Refer PBAP API document for the possible Events.
 *
 *  \param [in] event_result
 *         If the Event being reported is a result of an earlier trigger
 *         (PBAP API) by the application, this parameter holds result of the
 *         procedure. On successful completion of the procedure this parameter
 *         will have API_SUCCESS, else it will contain an Error code describing
 *         the reason for failure.
 *         For asynchronous event indications, this parameter holds the
 *         possible reason due to which the event is being reported.
 *         This may also hold the OBEX response codes for some events. Refer
 *         PBAP API Document for more details.
 *
 *  \param [in] pbap_handle
 *         This parameter contains the PBAP_HANDLE for which the notification is
 *         reported.
 *
 *  \param [in] pbap_headers
 *         Depending upon the event_type, this parameter holds any data being
 *         given to the PBAP application.
 *
 *  \param [in] num_headers
 *         Number of Headers in the pbap_headers parameter.
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure or the OBEX Response codes.
 */
typedef API_RESULT (*PBAP_EVENT_NTF_CB)
                   (
                        /* IN */ UINT8            event_type,
                        /* IN */ UINT16           event_result,
                        /* IN */ PBAP_HANDLE      pbap_handle,
                        /* IN */ PBAP_HEADERS   * pbap_headers,
                        /* IN */ UINT8            num_headers
                   );
/** \} */

/* ----------------------------------------- Functions */
/**
 * \defgroup  pbap_api API Defintions
 * \{
 * Describes API defintiions of PBAP Module
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef PBAP_PSE

/**
 *  \brief To initialize the PSE module.
 *
 *  \par Description:
 *       This API is used to initialize the PBAP PSE module. It initializes
 *       global data structures and synchronization primitives used in this
 *       module.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 *  \note It should be the first function to be called for using PBAP PSE
 *        module.
 */
API_RESULT BT_pbap_pse_init
           (
                void
           );

/**
 *  \brief To shutdown PSE module.
 *
 *  \par Description:
 *       This API is used to de-initialize the PBAP PSE module. It closes
 *       all the opened channels if exists and resets all PBAP PSE Profile
 *       Instances.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 *  \note It should be the last function to be called after using PBAP PSE
 *        module.
 */
API_RESULT BT_pbap_pse_shutdown
           (
                void
           );

/**
 *  \brief To start an instance of PSE.
 *
 *  \par Description:
 *       This API starts an instance of PBAP PSE. On the successful
 *       instantiation, this API returns a PBAP Handle, which is an index to the
 *       PBAP PSE entity. This also registers the callback function that PBAP
 *       needs to call when any event occurs for this instance. Now the PSE
 *       is ready for accepting the incoming connection from PSE.
 *
 *  \param [out] pbap_handle
 *         Denotes the PBAP Profile instance for newly started PBAP PSE
 *         instance.
 *
 *  \param [in] event_cb
 *         Application event notification callback.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note  PBAP Handle shall be used by the upper layer for using the PSE
 *         feature APIs (Send Respone).
 */
API_RESULT BT_pbap_pse_start_instance
           (
                /* OUT */ PBAP_HANDLE        * pbap_handle,
                /* IN */  PBAP_EVENT_NTF_CB    event_cb
           );

/**
 *  \brief To stop an instance of PSE.
 *
 *  \par Description:
 *       This API stops the instance of PBAP PSE specified by pbap_handle. It
 *       closes all channels if exist between that PSE instance and the
 *       remote PCE.
 *
 *  \param [in] pbap_handle
 *         Handle to the PSE instance to be freed.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pse_stop_instance
           (
                /* IN */ PBAP_HANDLE         pbap_handle
           );

/**
 *  \brief To disconnect the OBEX transport connection from the remote device.
 *
 *  \par Description:
 *       This API is used to request for OBEX transport disconnection from the
 *       remote device for the instance specified by pbap_handle.
 *
 *  \param [in] pbap_handle
 *         Handle to the profile instance.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pse_trans_disconnect
           (
                /* IN */ PBAP_HANDLE            pbap_handle
           );

/**
 *  \brief To send PBAP Response to the latest request made by the PCE
 *
 *  \par Description:
 *       This API is used to send the response to the peer device
 *
 *  \param [in] pbap_handle
 *         Handle to the profile instance.
 *
 *  \param [in] event_type
 *         Event to which the response is being sent.
 *
 *  \param [in] event_result
 *         The response code for the response to be sent.
 *
 *  \param [in] pbap_headers
 *         The PBAP data to be sent in the response.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pse_send_response
           (
                /* IN */ PBAP_HANDLE             pbap_handle,
                /* IN */ UINT8                   event_type,
                /* IN */ UINT16                  event_result,
                /* IN */ PBAP_HEADERS          * pbap_headers
           );

#endif /* PBAP_PSE */

#ifdef PBAP_PCE
/**
 *  \brief To initialize the PCE module.
 *
 *  \par Description:
 *       This API is used to initialize the PBAP PCE module. It initializes
 *       global data structures and synchronization primitives used in this
 *       module.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 *  \note It should be the first function to be called for using PBAP PCE
 *        module.
 */
API_RESULT BT_pbap_pce_init
           (
                void
           );

/**
 *  \brief To shutdown PCE module.
 *
 *  \par Description:
 *       This API is used to de-initialize the PBAP PCE module. It closes
 *       all the opened connections if exists and resets all PBAP PCE Profile
 *       Instances.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 *  \note It should be the last function to be called after using PBAP PCE
 *        module.
 */
API_RESULT BT_pbap_pce_shutdown
           (
                void
           );

/**
 *  \brief To start an instance of PCE.
 *
 *  \par Description:
 *       This API starts an instance of PBAP PCE. On the successful
 *       instantiation, this API returns a PBAP Handle, which is an index to the
 *       PBAP PCE entity. This also registers the callback function that PBAP
 *       needs to call when any event occurs for this instance. Now the PCE
 *       is ready to initiate a PBAP connection to PSE.
 *
 *  \param [out] pbap_handle
 *         Denotes the PBAP Profile instance for newly started PBAP PCE
 *         instance.
 *
 *  \param [in] event_cb
 *         Application event notificaton callback.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note  PBAP Handle shall be used by the upper layer for using the PCE
 *         feature APIs (Send Respone).
 */
API_RESULT BT_pbap_pce_start_instance
           (
                /* OUT */ PBAP_HANDLE        * pbap_handle,
                /* IN */  PBAP_EVENT_NTF_CB    event_cb
           );

/**
 *  \brief To stop an instance of PCE.
 *
 *  \par Description:
 *       This API stops the instance of PBAP PCE specified by pbap_handle.
 *       It closes the connection if exists with a remote PSE.
 *
 *  \param [in] pbap_handle
 *         Handle to the PCE instance to be freed.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pce_stop_instance
           (
                /* IN */ PBAP_HANDLE          pbap_handle
           );

/**
 *  \brief To connect an instance of PCE to a remote PSE.
 *
 *  \par Description:
 *       This API initiates PBAP connection of PCE instance specified by
 *       pbap_handle. It closes the connection if exists with a remote PSE.
 *
 *  \param [in] pbap_handle
 *         Handle to the PCE instance to be connected.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pce_connect
           (
                /* IN */ PBAP_HANDLE             pbap_handle,
                /* IN */ PBAP_CONNECT_STRUCT   * pbap_connect_info
           );

/**
 *  \brief To get a phonebook from the connected remote PSE.
 *
 *  \par Description:
 *       This API initiates PBAP get phone book operation from the remote PSE.
 *
 *  \param [in] pbap_handle
 *         Handle to the PCE instance
 *
 *  \param [in] pbap_get_info
 *         Object data to be sent with the Request
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pce_get_phonebook
           (
                /* IN */ PBAP_HANDLE             pbap_handle,
                /* IN */ PBAP_REQUEST_STRUCT   * pbap_get_info
           );

/**
 *  \brief To get list of the vcards from the connected remote PSE.
 *
 *  \par Description:
 *       This API initiates PBAP get vCard Lisitng operation from remote PSE.
 *
 *  \param [in] pbap_handle
 *         Handle to the PCE instance
 *
 *  \param [in] pbap_get_info
 *         Object data to be sent with the Request
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pce_get_vcard_list
           (
                /* IN */ PBAP_HANDLE             pbap_handle,
                /* IN */ PBAP_REQUEST_STRUCT   * pbap_get_info
           );

/**
 *  \brief To get a phonebook from the connected remote PSE.
 *
 *  \par Description:
 *       This API initiates PBAP set phone book operation from the remote PSE.
 *
 *  \param [in] pbap_handle
 *         Handle to the PCE instance
 *
 *  \param [in] pbap_get_info
 *         Object data to be sent with the Request
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pce_get_vcard
           (
                /* IN */ PBAP_HANDLE             pbap_handle,
                /* IN */ PBAP_REQUEST_STRUCT   * pbap_get_info
           );

/**
 *  \brief To set the path to phonebook from the connected remote PSE.
 *
 *  \par Description:
 *       This API initiates PBAP set phone book operation from the remote PSE.
 *
 *  \param [in] pbap_handle
 *         Handle to the PCE instance
 *
 *  \param [in] pbap_get_info
 *         Object data to be sent with the Request
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pce_set_phonebook
           (
                /* IN */ PBAP_HANDLE             pbap_handle,
                /* IN */ PBAP_REQUEST_STRUCT   * pbap_set_info
           );

/**
 *  \brief To abort the ongoing operation.
 *
 *  \par Description:
 *       This API initiates PBAP abort operation of the ongoing Get operation.
 *
 *  \param [in] pbap_handle
 *         Handle to the PCE instance
 *
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pce_abort
           (
                /* IN */ PBAP_HANDLE             pbap_handle
           );

/**
 *  \brief To abort the ongoing operation.
 *
 *  \par Description:
 *       This API initiates PBAP Disconnect operation from the remote PSE.
 *
 *  \param [in] pbap_handle
 *         Handle to the PCE instance
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pce_disconnect
           (
                /* IN */ PBAP_HANDLE             pbap_handle
           );

/**
 *  \brief To disconnect the OBEX transport connection from the remote device.
 *
 *  \par Description:
 *       This API is used to request for OBEX transport disconnection from the
 *       remote device for the instance specified by pbap_handle.
 *
 *  \param [in] pbap_handle
 *         Handle to the profile instance.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pce_trans_disconnect
           (
                /* IN */ PBAP_HANDLE             pbap_handle
           );

/**
 *  \brief To send PBAP subsequent requests to the PSE.
 *
 *  \par Description:
 *       This API is used to send the request to the peer device after
 *       receiving the first response.
 *
 *  \param [in] pbap_handle
 *         Handle to the profile instance.
 *
 *  \param [in] event_type
 *         Event to which the next request has to be sent.
 *
 *  \param [in] event_result
 *         The response code for the response to be sent.
 *
 *  \param [in] pbap_headers
 *         The PBAP data to be sent in the request (request parameters).
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_pbap_pce_send_request
           (
                /* IN */ PBAP_HANDLE             pbap_handle,
                /* IN */ UINT8                   event_type,
                /* IN */ UINT16                  event_result,
                /* IN */ PBAP_HEADERS          * pbap_headers
           );
#endif /* PBAP_PCE */

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_PBAP_API_ */

