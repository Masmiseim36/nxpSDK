
/**
 *  \file BT_ctn_api.h
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_CTN_API_
#define _H_BT_CTN_API_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_obex_api.h"


/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup  ctn_module  CTN (Calendar, Tasks, and Notes Profile)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Calendar, Tasks, and Notes Profile module to the Application and other upper
 *  layers of the stack.
 */
/**
 * \defgroup  ctn_defines Defines
 * \{
 * Describes defines for CTN module.
 */
/**
 * \defgroup ctn_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/* OBEX L2CAP PSM for local entities */
#define CTN_OBEX_L2CAP_PSM                       0x6001U

/* CTN Notification Callback Event types */
/**
 *  @name CTN Notification Callback Event types
 *  Constant Definitions for CTN Notification Callback Event types
 */
/*@{*/
#ifdef CTN_CCE

#define CTN_CCE_CONNECT_CNF                      0x01U
#define CTN_CCE_DISCONNECT_CNF                   0x02U
#define CTN_CCE_TRANSPORT_CLOSE_CNF              0x03U
#define CTN_CCE_TRANSPORT_CLOSE_IND              0x04U
#define CTN_CCE_GET_LISTING_CNF                  0x05U
#define CTN_CCE_GET_OBJECT_CNF                   0x06U
#define CTN_CCE_SET_OBJECT_STATUS_CNF            0x07U
#define CTN_CCE_PUSH_OBJECT_CNF                  0x08U
#define CTN_CCE_FORWARD_OBJECT_CNF               0x09U
#define CTN_CCE_SET_NTF_REGISTRATION_CNF         0x0AU
#define CTN_CCE_GET_ACCOUNT_INFO_CNF             0x0BU
#define CTN_CCE_SYNC_ACCOUNT_CNF                 0x0CU
#define CTN_CCE_ABORT_CNF                        0x0DU
#define CTN_CCE_SYNC_INSTANCE_CNF                0x0EU

#define CTN_CCE_NS_CONNECT_IND                   0x20U
#define CTN_CCE_NS_DISCONNECT_IND                0x21U
#define CTN_CCE_NS_TRANSPORT_CLOSE_IND           0x22U
#define CTN_CCE_NS_TRANSPORT_CLOSE_CNF           0x23U
#define CTN_CCE_NS_EVENT_REPORT_IND              0x24U

#endif /* CTN_CCE */

#ifdef CTN_CSE

#define CTN_CSE_CONNECT_IND                      0x31U
#define CTN_CSE_DISCONNECT_IND                   0x32U
#define CTN_CSE_TRANSPORT_CLOSE_IND              0x33U
#define CTN_CSE_TRANSPORT_CLOSE_CNF              0x34U
#define CTN_CSE_GET_LISTING_IND                  0x35U
#define CTN_CSE_GET_OBJECT_IND                   0x36U
#define CTN_CSE_SET_OBJECT_STATUS_IND            0x37U
#define CTN_CSE_PUSH_OBJECT_IND                  0x38U
#define CTN_CSE_SET_NTF_REGISTRATION_IND         0x39U
#define CTN_CSE_FORWARD_OBJECT_IND               0x3AU
#define CTN_CSE_GET_ACCOUNT_INFO_IND             0x3BU
#define CTN_CSE_SYNC_ACCOUNT_IND                 0x3CU
#define CTN_CSE_ABORT_IND                        0x3DU
#define CTN_CCE_SYNC_INSTANCE_IND                0x3EU

#define CTN_CSE_NS_CONNECT_CNF                   0x50U
#define CTN_CSE_NS_DISCONNECT_CNF                0x51U
#define CTN_CSE_NS_TRANSPORT_CLOSE_CNF           0x52U
#define CTN_CSE_NS_TRANSPORT_CLOSE_IND           0x53U
#define CTN_CSE_NS_SEND_EVENT_CNF                0x54U

#endif /* CTN_CSE */
/*@}*/
#define CTN_EVENT_INVALID                        0xFFU
#define CTN_ENTITY_INVALID                       0xFFU
#define CTN_INVALID_OBEX_CONNECTION_ID           0xFFFFFFFFU

/* CTN OBEX Response Codes */
/**
 *  @name CTN OBEX Response Codes
 *
 *  Constant Definitions for CTN OBEX Response Codes
 */
/*@{*/
#define CTN_SUCCESS_RSP                          OBEX_SUCCESS_RSP
#define CTN_CONTINUE_RSP                         OBEX_CONTINUE_RSP

#define CTN_BAD_REQ_RSP                          OBEX_REQ_IS_BAD_RSP
#define CTN_NOT_IMPLEMENTED_RSP                  OBEX_NOT_IMP_RSP
#define CTN_UNAUTH_RSP                           OBEX_UNAUTHORIZED_RSP
#define CTN_PRECOND_FAILED_RSP                   OBEX_PRECOND_FAIL_RSP
#define CTN_NOT_FOUND_RSP                        OBEX_NOT_FOUND_RSP
#define CTN_NOT_ACCEPTABLE_RSP                   OBEX_NOT_ACCEPTABLE_RSP
#define CTN_NO_SERVICE_RSP                       OBEX_NO_SERVICE_RSP
#define CTN_FORBIDDEN_RSP                        OBEX_FORBIDDEN_RSP
#define CTN_SERVER_ERROR                         OBEX_SERVER_ERR_RSP
/*@}*/
/* CTN Services OBEX Target header UUID values */
#define CTN_CAS_OBEX_TARGET_HDR_UUID            \
        0x115F1EC2292211E4B65DA6C5E4D22FB7U

#define CTN_CNS_OBEX_TARGET_HDR_UUID            \
        0x4FD68D34292211E4B65DA6C5E4D22FB7U

/** CTN Target Header Size */
#define CTN_TARGET_HDR_SIZE                      0x10U /* 16 */

/* CTN Application Parameters header Tag IDs */
/**
 *  @name CTN Application Parameters header Tag IDs
 *
 *  Constant Definitions for CTN Application Parameters header Tag IDs
 */
/*@{*/
#define CTN_TAG_ID_MAXLISTCOUNT                  0x41U  /* 2  Byte Length  */
#define CTN_TAG_ID_LISTSTARTOFFSET               0x42U  /* 2  Byte Length  */
#define CTN_TAG_ID_NOTIFICATIONSTATUS            0x43U  /* 1  Byte Length  */
#define CTN_TAG_ID_INSTANCEID                    0x44U  /* 1  Byte Length  */
#define CTN_TAG_ID_LISTINGSIZE                   0x46U  /* 2  Byte Length  */
#define CTN_TAG_ID_ACOUSTICALARMSTATUS           0x01U  /* 1  Byte Length  */
#define CTN_TAG_ID_ATTACHMENT                    0x02U  /* 1  Byte Length  */
#define CTN_TAG_ID_SEND                          0x03U  /* 1  Byte Length  */
#define CTN_TAG_ID_FILTERPERIODBEGIN             0x04U  /* Variable Length */
#define CTN_TAG_ID_FILTERPERIODEND               0x05U  /* Variable Length */
#define CTN_TAG_ID_PARMETERMASK                  0x06U  /* 4  Byte Length  */
#define CTN_TAG_ID_STATUSINDICATOR               0x07U  /* 1  Byte Length  */
#define CTN_TAG_ID_STATUSVALUE                   0x08U  /* 1  Byte Length  */
#define CTN_TAG_ID_POSTPONEVAL                   0x09U  /* 4  Byte Length  */
#define CTN_TAG_ID_EMAILURI                      0x0AU  /* Variable Length */
#define CTN_TAG_ID_CSETIME                       0x0BU  /* Variable Length */
#define CTN_TAG_ID_RECURRENT                     0x0CU  /* 1  Byte Length  */
#define CTN_TAG_ID_ATTACHID                      0x0DU  /* 1  Byte Length  */
#define CTN_TAG_ID_LASTUPDATE                    0x0EU  /* Variable Length */
/*@}*/
/** Maximum number of Application Headers */
#define CTN_NUM_APPL_HEADERS                     0x13U /* 19 */

/* CTN Application parameters flag corresponding to each Tag */
/**
 *  @name CTN Application parameters flag corresponding to each Tag
 *
 *  Constant Definitions for CTN Application parameters flag corresponding to each Tag
 */
/*@{*/
#define CTN_FLAG_MAXLISTCOUNT                    0x00000001U
#define CTN_FLAG_LISTSTARTOFFSET                 0x00000002U
#define CTN_FLAG_NOTIFICATIONSTATUS              0x00000004U
#define CTN_FLAG_INSTANCEID                      0x00000008U
#define CTN_FLAG_LISTINGSIZE                     0x00000010U
#define CTN_FLAG_ACOUSTICALARMSTATUS             0x00000020U
#define CTN_FLAG_ATTACHMENT                      0x00000040U
#define CTN_FLAG_SEND                            0x00000080U
#define CTN_FLAG_FILTERPERIODBEGIN               0x00000100U
#define CTN_FLAG_FILTERPERIODEND                 0x00000200U
#define CTN_FLAG_PARMETERMASK                    0x00000400U
#define CTN_FLAG_STATUSINDICATOR                 0x00000800U
#define CTN_FLAG_STATUSVALUE                     0x00001000U
#define CTN_FLAG_POSTPONEVAL                     0x00002000U
#define CTN_FLAG_EMAILURI                        0x00004000U
#define CTN_FLAG_CSETIME                         0x00008000U
#define CTN_FLAG_RECURRENT                       0x00010000U
#define CTN_FLAG_ATTACHID                        0x00020000U
#define CTN_FLAG_LASTUPDATE                      0x00040000U
/*@}*/
/* Application Header Parameters Value Range Definitions */
/* ----------------------------------------------------- */
/**
 *  \defgroup  APP_VAL BPP Application Header Parameters Value Range Definitions
 *
 *
 *  Application Header Parameters Value Range Definitions
 *
 *  \{
 *
 */
/* CTN Application Parameter values  */
/**
 * @name CTN Application Parameter values
 */
/*@{*/
#define CTN_NOTIFICATIONSTATUS_OFF               0x00U
#define CTN_NOTIFICATIONSTATUS_ON                0x01U
/*@}*/
/**
 * @name
 */
/*@{*/
#define CTN_ACOUSTICALARM_OFF                    0x00U
#define CTN_ACOUSTICALARM_ON                     0x01U
/*@}*/
/**
 * @name
 */
/*@{*/
#define CTN_ATTACHMENT_ON                        0x00U
#define CTN_ATTACHMENT_OFF                       0x01U
#define CTN_ATTACHMENT_SELECT                    0x02U
/*@}*/
/**
 * @name
 */
/*@{*/
#define CTN_SEND_NO                              0x00U
#define CTN_SEND_YES                             0x01U
/*@}*/
/**
 * @name
 */
/*@{*/
#define CTN_STATUSINDICATOR_PSTATUS              0x00U
#define CTN_STATUSINDICATOR_ALARMSTATUS          0x01U
#define CTN_STATUSINDICATOR_SENDSTATUS           0x02U
#define CTN_STATUSINDICATOR_DELETEDSTATUS        0x03U
/*@}*/
/**
 * @name
 */
/*@{*/
#define CTN_STATUSVALUE_NO                       0x00U
#define CTN_STATUSVALUE_YES                      0x01U
#define CTN_STATUSVALUE_POSTPONE                 0x02U
#define CTN_STATUSVALUE_TENTATIVE                0x03U
#define CTN_STATUSVALUE_NEED_ACTION              0x04U
#define CTN_STATUSVALUE_ACCEPTED                 0x05U
#define CTN_STATUSVALUE_DECLINED                 0x06U
#define CTN_STATUSVALUE_DELEGATED                0x07U
#define CTN_STATUSVALUE_COMPLETED                0x08U
#define CTN_STATUSVALUE_IN_PROGRESS              0x09U
/*@}*/
/**
 * @name
 */
/*@{*/
#define CTN_RECURRENT_NO                         0x00U
#define CTN_RECURRENT_YES                        0x01U
/*@}*/
/**
 */
/*@{*/
/**
 * @name CTN parameter mask for Get Listing Request
 */
#define CTN_PARAM_MASK_ATTACHMENT                0x00000001U
#define CTN_PARAM_MASK_SUMMARY                   0x00000002U
#define CTN_PARAM_MASK_ENDTIME                   0x00000004U
#define CTN_PARAM_MASK_ORIGINATOR_NAME           0x00000008U
#define CTN_PARAM_MASK_ORIGINATOR_ADDRESS        0x00000010U
#define CTN_PARAM_MASK_PRIORITY                  0x00000020U
#define CTN_PARAM_MASK_PSTATUS                   0x00000040U
#define CTN_PARAM_MASK_ALARMSTATUS               0x00000080U
#define CTN_PARAM_MASK_SENDSTATUS                0x00000100U
#define CTN_PARAM_MASK_RECURRENT                 0x00000200U
/*@}*/

/**\}*/
/** \} */
/* ----------------------------------------------------- */

/* --------------------------------------------- Structures/Data Types */
/**
 * \defgroup ctn_strcutures Structures
 * \{
 * Describes Structures defined by the module.
 */
/* CTN Handle type */
typedef UINT8 CTN_HANDLE;

/* CTN Service Types */
typedef enum
{
    CTN_ACCESS_SERVICE = 0x01U,
    CTN_NTF_SERVICE = 0x02U

} CTN_SERVICE_TYPE;

/* CTN Entity Handle */
typedef struct _CTN_HANDLE_INFO
{
    /**
     * Service Type
     */
    CTN_SERVICE_TYPE    service;

    /**
     * Entity CTN Handle ID
     */
    CTN_HANDLE    entity_id;

} CTN_HANDLE_INFO;

/**
 * CTN Header structure, to exchange data with application
 */
typedef struct _CTN_HEADER_STRUCT
{
    /**
     * Pointer to data
     */
    UCHAR   * value;

    /**
     * Length of the data
     */
    UINT16    length;
} CTN_HEADER_STRUCT;

/* CTN Application Parameter Structure */
typedef struct _CTN_APPL_PARAMS
{
    /**
     * 32 Bit Flag [19 Least Significant Bits used]
     *
     * Usage:
     *
     * BIT 0 - Max List Count
     *     1 - List Start Offset
     *     2 - Notification Status
     *     3 - Instance ID
     *     4 - Listing Size
     *     5 - Acoustic Alarm Size
     *     6 - Attachment
     *     7 - Send
     *     8 - Filter Period Begin
     *     9 - Filter Period End
     *    10 - Parameter Mask
     *    11 - Status Indicator
     *    12 - Status Value
     *    13 - Postpone Val
     *    14 - Email URI
     *    15 - CSE Time
     *    16 - Recurrent
     *    17 - Attach ID
     *    18 - Last Update
     */

    UINT32                 appl_param_flag;


    CTN_HEADER_STRUCT      filter_period_begin;
    CTN_HEADER_STRUCT      filter_period_end;
    CTN_HEADER_STRUCT      emailuri;
    CTN_HEADER_STRUCT      cse_time;
    CTN_HEADER_STRUCT      lastupdate;

    UINT32                 postpone_val;
    UINT32                 parameter_mask;

    UINT16                 max_list_count;
    UINT16                 list_start_offset;
    UINT16                 listing_size;

    UINT8                  notification_status;
    UINT8                  cas_instance_id;
    UINT8                  acoustic_alarm_status;
    UINT8                  attachment;
    UINT8                  send;
    UINT8                  status_indicator;
    UINT8                  status_value;
    UINT8                  recurrent;
    UINT8                  attach_id;

} CTN_APPL_PARAMS;


/* CTN Connection Structure */
typedef struct _CTN_CONNECT_STRUCT
{
    /**
     * Bluetooth address of the peer device
     */
    UCHAR               * bd_addr;

    /**
     *  Maximum Receive Size
     */
    UINT16                max_recv_size;

#ifdef OBEX_OVER_L2CAP
    /**
     * L2CAP PSM
     */
    UINT16                l2cap_rpsm;
#endif /* OBEX_OVER_L2CAP */

} CTN_CONNECT_STRUCT;


/* CTN Request Structure */
typedef struct _CTN_REQUEST_STRUCT
{
    /**
     * Name/Handle of the object
     */
    CTN_HEADER_STRUCT   * name;

    /**
     * List of receipients to forward a bCalendar
     * object
     */
    CTN_HEADER_STRUCT   * description;

    /**
     * Object content
     */
    CTN_HEADER_STRUCT   * body;

    /**
     * Application Parameters
     */
    CTN_APPL_PARAMS     * appl_params;

    /**
     * Wait specifier during SRM transaction
     */
    UCHAR                wait;
} CTN_REQUEST_STRUCT;


/* CTN Response Structure */
typedef struct _CTN_RESPONSE_STRUCT
{
     /**
      * Name/Handle of the Object
      */
    CTN_HEADER_STRUCT  * name;

     /**
      * Object contents
      */
    CTN_HEADER_STRUCT  * body;

    /**
     * Application Parameters
     */
    CTN_APPL_PARAMS    * appl_params;

    /**
     * Wait specifier during SRM transaction
     */
    UCHAR                wait;
} CTN_RESPONSE_STRUCT;


/* CTN common headers */
typedef union _CTN_HEADERS
{
    /**
     * Request structure, used to send the CTN request
     */
    CTN_REQUEST_STRUCT   * ctn_req_info;

    /**
     * Response structure, used to send the CTN response
     */
    CTN_RESPONSE_STRUCT  * ctn_rsp_info;

    /**
     * Connect structure, used to send the CTN connect request
     */
    CTN_CONNECT_STRUCT   * ctn_connect_info;
} CTN_HEADERS;
/**\}*/
/**\}*/

/**
 *  \defgroup ctn_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
/** CTN Event Notification Callback Type */
typedef API_RESULT (* CTN_EVENT_NTF_CB)
                   (
                        CTN_HANDLE_INFO * ctn_handle,
                        UINT8             event_type,
                        UINT16            event_result,
                        CTN_HEADERS     * ctn_headers,
                        UINT16            num_headers
                   );
/**\}*/

/* --------------------------------------------- Macros */
/**
 * \defgroup ctn_utility_macros Utility Macros
 * \{
 * Describes Utility Macros defined by the module.
 */
/* To initialize a Header Structure */
#define CTN_INIT_HEADER_STRUCT(hs)                                          \
        (hs).value = NULL;                                                  \
        (hs).length = 0U;

/* To set application parameter flag */
#define CTN_SET_APPL_PARAM_FLAG(flag, set_field)                            \
        (flag) = (flag) | (set_field);

/* To retrieve the field value (Set/Reset) */
#define CTN_GET_APPL_PARAM_FLAG(flag, get_field)                            \
        ((flag) & (get_field))

/* Resets all the fields of the application parameter flag */
#define CTN_RESET_APPL_PARAM_FLAG(flag)                                     \
        (flag) = 0x00000000U

/* Extract header structure value */
#define CTN_COPY_HEADER_STRUCT(hdr, len, val)                               \
        (len) = (hdr)->length;                                              \
        BT_str_n_copy((val), (hdr)->value, (len));                          \
        (val)[(len)] = '\0';

/* Extract application parameters */
#define CTN_COPY_APPL_PARAMS(to_params, from_params, search_value_out)      \
        BT_mem_copy((to_params), (from_params), sizeof(CTN_APPL_PARAMS));   \
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
/**\}*/

/* --------------------------------------------- API Declarations */
/**
 * \defgroup ctn_api_definitions API Definitions
 * \{
 * Describes CTN API definitions of the module.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* CTN Client APIs */
#ifdef CTN_CCE
/**
 *  \brief To initialize the CTN CCE module.
 *
 *  \par Description:
 *       This API is used to initialize the CTN CCE module. It initializes
 *       global data structures and synchronization primitives used in this
 *       module.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 *  \note It should be the first function to be called for using CTN CCE
 *        module.
 */
API_RESULT BT_ctn_cce_init
           (
               void
           );

/**
 *  \brief To shutdown CTN CCE module.
 *
 *  \par Description:
*        This API shuts down the CTN CCE module if it is already initialized.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 *  \note This API shall be called only when there are no active/started instances of
 *        the CCE. In case of active CCE instances, they should be stopped by
 *        calling BT_ctn_cce_stop() API..
 */
API_RESULT BT_ctn_cce_shutdown
           (
               void
           );

/**
 *  \brief To start an instance of CTN CCE.
 *
 *  \par Description:
 *       This API starts an instance of CTN CCE. On the successful
 *       instantiation, this API returns a CTN handle which is an index to the
 *       CTN CCE entity. This also registers the callback function that CTN
 *       needs to call when any event occurs for this instance. Now the CCE
 *       is ready to initiate a CTN connection to CSE.
 *
 *  \param [in] ctn_service
 *         CTN Service Type (Access/Notification) to be started
 *
 *  \param [in] ctn_cce_cb
 *         The application callback function pointer to be registered.
 *
 *  \param [out] ctn_handle
 *         Index to the client CAS/CNS entity that is started.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note All further communication exchanged between the application and profile
 *        for the CCE instance will be identified using the assigned CTN handle.
 */
API_RESULT BT_ctn_cce_start
           (
               /* IN */  CTN_SERVICE_TYPE    ctn_service,
               /* IN */  CTN_EVENT_NTF_CB    ctn_cce_cb,
               /* OUT */ CTN_HANDLE        * ctn_handle
           );

/**
 *  \brief To stop an active CTN CCE instance of Access/Notification Service
 *         as given by the ctn_service parameter
 *
 *  \par Description:
 *       This API stops the CTN CCE instance corresponding
 *       to the specified handle.
 *
 *  \param [in] ctn_service
 *         CTN Service Type (Access/Notification) to be stopped.
 *
 *  \param [in] ctn_handle
 *         Index to the client CAS/CNS entity that has to be stopped.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note In case an active connection exists in the specified instance to be stopped,
 *        the transport for the connection is closed and the instance is stopped.
 */
API_RESULT BT_ctn_cce_stop
           (
               /* IN */ CTN_SERVICE_TYPE    ctn_service,
               /* IN */ CTN_HANDLE        * ctn_handle
           );

/**
 *  \brief To establish the CTN Access Service (CAS) connection.
 *
 *  \par Description:
 *       This API establishes the CTN transport &  profile connection with the remote
 *       CTN server (CSE) for the CTN Access Service (CAS). CTN CCE calls the registered
 *       application callback upon the completion of the connection procedure (when the CSE
 *       responds with the connection response) with the notification CTN_CCE_CONNECT_CNF event
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance that has to be connected.
 *
 *  \param [in] ctn_connect_info
 *         Structure containing the Bluetooth Device Address of the CSE,
 *         the local maximum receive capacity (in Bytes) and L2CAP psm of CSE.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_connect
           (
               /* IN */ CTN_HANDLE         * ctn_handle,
               /* IN */ CTN_CONNECT_STRUCT * ctn_connect_info
           );

/**
 *  \brief To disconnect the established CTN Access Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the active CTN Access Service Connection
 *       with the remote CTN CSE. CTN CCE calls the registered application
 *       callback upon completion of this request with the notification
*        CTN_CCE_DISCONNECT_CNF event.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance that has to be disconnected.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_disconnect
           (
               /* IN */ CTN_HANDLE   * ctn_handle
           );

/**
 *  \brief To disconnect the underlying transport of CTN Access Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the Access
 *       Service with the remote CTN CSE. CTN CCE calls the registered
 *       application callback upon completion of this procedure with the
 *       notification CTN_CCE_TRANSPORT_CLOSE_CNF event.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance that has to be disconnected.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_transport_close
           (
               /* IN */ CTN_HANDLE   * ctn_handle
           );

/**
 *  \brief To get the bCalendar object listing from the given
 *         directory in the CSE.
 *
 *  \par Description:
 *       This API retrieves the bCalendar object listing from the
 *       requested folder at the connected CSE entity. The application
 *       shall include the folder name and may include the optional
 *       application parameters for the request using the ctn_get_info parameter.
 *
 *       The completion of this procedure is notified through CTN_CCE_GET_LISTING_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance.
 *
 *  \param [in] ctn_get_info
 *         Structure containing folder name to retrieve the CTN bCalendar object
 *         listing from and the application parameters if any, that may be included
 *         by the application. The following application parameters are supported
 *         as part of this request for this parameter.
 *         MaxListCount, ListStartOffset, ParameterMask, FilterPeriodBegin, FilterPeriodEnd.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_get_listing
           (
               /* IN */ CTN_HANDLE           * ctn_handle,
               /* IN */ CTN_REQUEST_STRUCT   * ctn_get_info
           );

/**
 *  \brief To get the bCalendar object with given handle from the CSE's repository.
 *
 *  \par Description:
 *       This API retrieves the bCalendar object with the given object handle from
 *       the connected CSE device. The application shall specify the object handle
 *       and application parameters through the ctn_get_info parameter.
 *
 *       The completion of this procedure is notified through CTN_CCE_GET_OBJECT_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance.
 *
 *  \param [in] ctn_get_info
 *         Structure containing the object handle that is to be retrieved and the
 *         application parameters. The following are the mandatory and optional
 *         application parameters that are supported for the request through this parameter.
 *         Attachment (M), AttachmentID (O), Recurrent (O)
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_get_object
           (
               /* IN */ CTN_HANDLE           * ctn_handle,
               /* IN */ CTN_REQUEST_STRUCT   * ctn_get_info
           );

/**
 *  \brief To modify the status of the CTN object entry in the CSE with given handle.
 *
 *  \par Description:
 *       This API modifies the status indicator value of a CTN object with given
 *       handle in the CSE. The application shall specify the object handle and
 *       application parameters through the ctn_set_info parameter.
 *
 *       The completion of this procedure is notified through CTN_CCE_SET_OBJECT_STATUS_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance.
 *
 *  \param [in] ctn_set_info
 *         Structure containing the object handle whose status indicator has to be updated
 *         and the application parameters that convey the indicator parameter to be updated
 *         and the value. The following are the mandatory & optional application parameters
 *         that are supported for the request through this parameter.
 *         StatusIndicator(M), StatusValue(M), PostponeVal(O)
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_set_status
           (
               /* IN */ CTN_HANDLE           * ctn_handle,
               /* IN */ CTN_REQUEST_STRUCT   * ctn_set_info
           );

/**
 *  \brief To push a CTN object to the CTN CSE.
 *
 *  \par Description:
 *       This API pushes the given CTN object to the CSE repository. The application shall
 *       specify the name of folder to which the object has to be directed to at
 *       the CSE, the application parameters and the object body through the
 *       parameter ctn_req_info.
 *
 *       The completion of this procedure is notified through CTN_CCE_PUSH_OBJECT_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance.
 *
 *  \param [in] ctn_req_info
 *         Structure containing name of the folder at CSE to which the object is
 *         directed along with the application parameters and message body.
 *         The following mandatory application parameter is supported for the
 *         request through this parameter. Send (M)
 *
 *  \param [in] more
 *         Indicates if application has remaining of object still to be sent or object complete
 *         1 -> Object part remaining
 *         0 -> No more object part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included in
 *         the request, that is packed for transmission.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_push_object
           (
               /* IN */  CTN_HANDLE           * ctn_handle,
               /* IN */  CTN_REQUEST_STRUCT   * ctn_req_info,
               /* IN */  UCHAR                  more,
               /* OUT */ UINT16               * actual
           );

/**
 *  \brief To forward a bCalendar from CSE's repository to one or more additional recipients.
 *
 *  \par Description:
 *       This API requests the connected CSE to forward a bCalendar object. Upon receiving
 *       a response from the Server, the application is notified using the registered event
 *       callback through the event CTN_CCE_FORWARD_OBJECT_CNF along with the result of the
 *       operation
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance.
 *
 *  \param [in] ctn_set_info
 *         Structure containing handle of the object to be forwarded and the description
 *         containing the list of recipients to forward the given object.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_forward_object
           (
               /* IN */ CTN_HANDLE          * ctn_handle,
               /* IN */ CTN_REQUEST_STRUCT  * ctn_req_info
           );

/**
 *  \brief To retrieve account information of CSE.
 *
 *  \par Description:
 *       This API requests the connected CSE to retrieve user-readable information
 *       about CTN accounts(instances) provided by the CSE. Upon receiving a response
 *       from the Server, the application is notified using the registered event callback
 *       through the event CTN_CCE_GET_ACCOUNT_INFO_CNF along with the result of the operation
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance.
 *
 *  \param [in] ctn_set_info
 *         Structure containing the mandatory InstanceID application parameter
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_get_account_information
           (
               /* IN */ CTN_HANDLE          * ctn_handle,
               /* IN */ CTN_REQUEST_STRUCT  * ctn_set_info
           );

/**
 *  \brief To initiate synchronization of the CSE's account with external server.
 *
 *  \par Description:
 *       This API requests the connected CSE to synchronize the CSE's repository in case added,
 *       removed or updated entries. Upon receiving a response from the server, the application is
 *       notified using the registered event callback through the event CTN_CCE_SYNC_ACCOUNT_CNF
 *       along with the result of the operation.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_sync_account
           (
               /* IN */ CTN_HANDLE          * ctn_handle
           );

/**
 *  \brief To register for enabling or disabling notification with CSE.
 *
 *  \par Description:
 *       This API registers with the CSE to enable or disable the
 *       notification feature. The application shall include the
 *       NotificationStatus application parameter through the ctn_set_info parameter.
 *       The completion of this procedure is notified through
 *       CTN_CCE_SET_NTF_REGISTRATION_CNF event along with the status of the procedure.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance.
 *
 *  \param [in] ctn_set_info
 *         Structure containing the mandatory NotificationStatus, AcousticAlarmStatus
 *         application parameters
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_set_ntf_registration
           (
               /* IN */ CTN_HANDLE           * ctn_handle,
               /* IN */ CTN_REQUEST_STRUCT   * ctn_set_info
           );

/**
 *  \brief To send a request to the CTN server.
 *
 *  \par Description:
 *       This API sends the request to the CTN server with the corresponding
 *       CTN event and the data given by the application contained in the ctn_headers.
 *       The application shall call this API to continue any operation initiated and to
 *       which the first notification has been received. Thus this API shall be called from the
 *       CTN CCE application callback function with the event_type set to the current received event.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback,
 *         to indicate that the request is the continuation of the
 *         operation for which it was notified.
 *
 *  \param [in] event_result
 *         This is the status of the request.
 *
 *  \param [in] ctn_headers
 *           Structure containing either the connect information in case
 *           of connect operation or Get request information, in case of
 *           Get operation or Put request information in case of Put operation.
 *
 *  \param [in] more
 *          Indicates if application has remaining of object body still to be
 *          sent or object complete
 *          1 -> Object part remaining
 *          0 -> No more object part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included
 *         in the request, that is packed for transmission.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_send_request
           (
               /* IN */  CTN_HANDLE   * ctn_handle,
               /* IN */  UINT8          event_type,
               /* IN */  UINT16         event_result,
               /* IN */  CTN_HEADERS  * ctn_headers,
               /* IN */  UCHAR          more,
               /* OUT */ UINT16       * actual
           );

/**
 *  \brief To abort the ongoing CTN operation.
 *
 *  \par Description:
 *       This API aborts the ongoing OBEX Get/Put operation
 *       that was initiated by the Client. The completion of the
 *       Abort operation is notified through CTN_CCE_ABORT_CNF notification
 *       event along with the status of the procedure.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance of which the operation is to be aborted.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_abort
           (
               /* IN */ CTN_HANDLE   * ctn_handle
           );

#ifdef CTN_SUPPORT_NOTIFICATION

/**
 *  \brief To disconnect the underlying transport of CTN Notificatin Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the Notification
 *       Service with the remote CTN CSE. CTN CCE calls the registered application
 *       callback upon completion of this procedure with the notification
 *       CTN_CCE_NS_TRANSPORT_CLOSE_CNF event.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CNS instance to be disconnected.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_ns_transport_close
           (
               /* IN */ CTN_HANDLE   * ctn_handle
           );

/**
 *  \brief To send response to the latest request received over notification
 *         channel from the CTN CSE.
 *
 *  \par Description:
 *       This API is used to send the response to the requests made by the CTN CSE.
 *       Response to connect, send event etc. shall be sent through this API.
 *       The application shall call this API with the event type of the latest
 *       notification received. This indicates the response is for the latest
 *       indication received. CTN CCE frames the OBEX specific packets based on the
 *       response & associated response_data and send it to the CTN CSE.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CNS instance.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback. This is to
 *         indicate that the response being sent is response to the request for
 *         which the application was notified.
 *
 *  \param [in] response
 *         This is the OBEX response code.
 *
 *  \param [in] ctn_headers
 *         Structure containing either the connect information, in case of connect operation
 *         or the response data, in case of Get/Put operation.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cce_ns_send_response
           (
                /* IN */ CTN_HANDLE    * ctn_handle,
                /* IN */ UINT8           event_type,
                /* IN */ UCHAR           response,
                /* IN */ CTN_HEADERS   * ctn_headers
           );
#endif /* CTN_SUPPORT_NOTIFICATION */

#ifdef HAVE_CTN_SYNC_INSTANCE
/**
 * CTN Sync CTN Instance with external Server
 * Note: This feature is related to Generic PIM Profile(GPP)
 */
API_RESULT BT_ctn_cce_sync_instance
           (
               /* IN */ CTN_HANDLE          * ctn_handle
           );
#endif /* HAVE_CTN_SYNC_INSTANCE */

#endif /* CTN_CCE */

/* CTN Server APIs */
#ifdef CTN_CSE

/**
 *  \brief To initialize the CTN CSE module.
 *
 *  \par Description:
 *       This API is used to initialize the CTN CSE module. It initializes
 *       global data structures and synchronization primitives used in this
 *       module.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 *  \note It should be the first function to be called for using CTN CSE
 *        module.
 */
API_RESULT BT_ctn_cse_init
           (
               void
           );

/**
 *  \brief To shutdown CTN CSE module.
 *
 *  \par Description:
 *       This API shuts down the CTN CSE module if it is already initialized.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 *  \note This API shall be called only when there are no active/started instances of
 *        the CSE. In case of active CSE instances, they should be stopped by
 *        calling BT_ctn_cse_stop() API.
 */
API_RESULT BT_ctn_cse_shutdown
           (
               void
           );

/**
 *  \brief To start an instance of CTN CSE.
 *
 *  \par Description:
 *       This API starts an instance of CTN CSE. On the successful
 *       instantiation, this API returns a CTN handle which is an index to the
 *       CTN CSE entity. This also registers the callback function that CTN
 *       needs to call when any event occurs for this instance.
 *
 *  \param [in] ctn_service
 *         CTN Service Type (Access/Notification) to be started
 *
 *  \param [in] ctn_cse_cb
 *         The application callback function pointer to be registered.
 *
 *  \param [out] ctn_handle
 *         Index to the client CAS/CNS entity that is started.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note All further communication exchanged between the application and profile
 *        for the CSE instance will be identified using the assigned CTN handle.
 */
API_RESULT BT_ctn_cse_start
           (
               /* IN */  CTN_SERVICE_TYPE    ctn_service,
               /* IN */  CTN_EVENT_NTF_CB    ctn_cse_cb,
               /* OUT */ CTN_HANDLE        * ctn_handle
           );

/**
 *  \brief To stop an active CTN CSE instance of Access/Notification Service
 *         as given by the ctn_service parameter
 *
 *  \par Description:
 *       This API stops the CTN CSE instance corresponding
 *       to the specified handle.
 *
 *  \param [in] ctn_service
 *         CTN Service type (Access/Notification) to be stopped.
 *
 *  \param [in] ctn_handle
 *         Index to the client CAS/CNS entity that has to be stopped.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note In case an active connection exists in the specified instance to be stopped,
 *        the transport for the connection is closed and the instance is stopped.
 */
API_RESULT BT_ctn_cse_stop
           (
               /* IN */ CTN_SERVICE_TYPE    ctn_service,
               /* IN */ CTN_HANDLE        * ctn_handle
           );

/**
 *  \brief To disconnect the underlying transport of CTN Access Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the Access
 *       Service with the remote CTN CCE. CTN CSE calls the registered
 *       application callback upon completion of this request with the
 *       notification CTN_CSE_TRANSPORT_CLOSE_CNF event.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance that has to be disconnected.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cse_transport_close
           (
               /* IN */ CTN_HANDLE  * ctn_handle
           );

/**
 *  \brief To send response to the latest request received from the CTN CCE.
 *
 *  \par Description:
 *       This API is used to send the response to the requests made by the CTN CCE.
 *       Response to Connect, GetListing, GetAcountInformation, GetObject,
 *       PutObject etc. shall be sent through this API.The application shall call
 *       this API with the event type of the latest notification received.
 *       This indicates the response is for the latest indication received.
 *       CTN CSE frames the OBEX specific packets based on the response and associated
 *       response data and send it to the CTN CCE.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback.
 *         This is to indicate that the response being sent is response to the
 *         request for which the application was notified
 *
 *  \param [in] event_result
 *         This is the OBEX response code.
 *
 *  \param [in] ctn_headers
 *         Structure containing either the connect information, in case of connect operation
 *         or the response data, in case of Get/Put operation.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cse_send_response
           (
                /* IN */ CTN_HANDLE           * ctn_handle,
                /* IN */ UINT8                  event_type,
                /* IN */ UCHAR                  event_result,
                /* IN */ CTN_HEADERS          * ctn_headers
           );

#ifdef CTN_SUPPORT_NOTIFICATION
/**
 *  \brief To establish the CTN Notification Service (CNS) Connection.
 *
 *  \par Description:
 *       This API establishes the OBEX connection with the remote CTN Client (CCE)
 *       for the CTN Notification Service (CNS). CTN CSE calls the registered
 *       application callback upon the completion of the connection procedure
 *       (when the CCE responds with the connection response) with the notification
 *       CTN_CSE_NS_CONNECT_CNF event.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CNS instance that has to be connected.
 *
 *  \param [in] ctn_connect_info
 *         Structure containing the Bluetooth Device Address of the CCE,
 *         the local maximum receive capacity (in Bytes) and L2CAP psm of CCE.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cse_ns_connect
           (
               /* IN */ CTN_HANDLE          * ctn_handle,
               /* IN */ CTN_CONNECT_STRUCT  * ctn_connect_info
           );

/**
 *  \brief To disconnect the established CTN Notification Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the active CTN Notification Service Connection
 *       with the remote CTN CCE. CTN CSE calls the registered application
 *       callback upon completion of this procedure with the notification
*        CTN_CSE_DISCONNECT_CNF event.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CNS instance that has to be disconnected.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cse_ns_disconnect
           (
               /* IN */ CTN_HANDLE  * ctn_handle
           );

/**
 *  \brief To disconnect the underlying transport of CTN Notificatin Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the Notification
 *       Service with the remote CTN CCE. CTN CSE calls the registered application
 *       callback upon completion of this procedure with the notification
 *       CTN_CCE_NS_TRANSPORT_CLOSE_CNF event.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CNS instance to be disconnected.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cse_ns_transport_close
           (
               /* IN */ CTN_HANDLE  * ctn_handle
           );

/**
 *  \brief To Send a notification event to the CNS server in the CTN client.
 *
 *  \par Description:
 *       This API send a notification event over notification channel from
 *       the CTN server to the CTN Client. The application shall call
 *       this API with the event report content.
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CAS instance.
 *
 *  \param [in] ctn_req_info
 *         Structure containing the notification event report content.
 *
 *  \param [in] more
 *         Indicates if application has remaining of event body still to be sent or event complete
 *         1 -> Event body part remaining
 *         0 -> No more event body part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included in
 *         the request, that is packed for transmission.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cse_ns_send_event
           (
               /* IN */  CTN_HANDLE           * ctn_handle,
               /* IN */  CTN_REQUEST_STRUCT   * ctn_req_info,
               /* IN */  UCHAR                  more,
               /* OUT */ UINT16               * actual
           );

/**
 *  \brief To send a request(Notification event) over notification channel to the CNS server in the CTN client.
 *
 *  \par Description:
 *       This API sends the request over notification channel to the CTN client with the corresponding
 *       CTN event and the data given by the application contained in the ctn_headers.
 *       The application shall call this API to continue any operation initiated and to
 *       which the first notification has been received. Thus this API shall be called from the
 *       CTN CSE application callback function with the event_type set to the current received event
 *
 *  \param [in] ctn_handle
 *         CTN handle of the CNS instance.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback,
 *         to indicate that the request is the continuation of the
 *         operation for which it was notified.
 *
 *  \param [in] event_result
 *         This is the status of the request.
 *
 *  \param [in] ctn_headers
 *           Structure containing either the get request information, in case of
 *           get operation or put request information in case of put operation.
 *
 *  \param [in] more
 *         Indicates if application has remaining of event still to be sent or event complete
 *         1 -> Event part remaining
 *         0 -> No more event part remaining
 *
 *  \param [out] actual
 *         Contains the actual number of bytes of BODY header, if any included in
 *         the request, that is packed for transmission.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_ctn_cse_ns_send_request
           (
               /* IN */  CTN_HANDLE   * ctn_handle,
               /* IN */  UINT8          event_type,
               /* IN */  UINT16         event_result,
               /* IN */  CTN_HEADERS  * ctn_headers,
               /* IN */  UCHAR          more,
               /* OUT */ UINT16       * actual
           );

#endif /* CTN_SUPPORT_NOTIFICATION */

#endif /* CTN_CSE */
#ifdef __cplusplus
};
#endif
/**\}*/
/**\}*/
/**\}*/
#endif /* _H_BT_CTN_API_ */

