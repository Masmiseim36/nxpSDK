
/**
 *  \file GA_ccp_ce_api.h
 *
 *  \brief This file defines the GA Call Control Profile(CCP) Client Entity(CE) Interface - includes
 *   Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_CCP_CE_API_
#define _H_GA_CCP_CE_API_

/**
 * \addtogroup bt_ga
 * \{
 */
/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"

/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_ga_profiles
 * \{
 */

/**
 * \addtogroup bt_ga_content_control Content Control
 * \{
 */

/* --------------------------------------------- Global Definitions */

/**
 * \addtogroup bt_ga_ccp Call Control
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Generic Audio (GA) Profile Call Control module to the Application.
 */

/**
 * \addtogroup ga_ccp_defines Defines
 * \{
 * \brief This section describes the defines for the module.
 */

/**
 * \defgroup ga_ccp_ce_module_def CCP CE (Call Control Profile) Client Entity
 * \{
 * \brief This section describes the defines for CCP CE.
 */

 /**
 * \defgroup ga_ccp_ce_constants Constants
 * \{
 * \brief This section lists Constants,
 * As defined in TBS Specification.
 */

/**
 * \name CCP Client Constants - General Macros
 * \{
 * \brief This section describes the general macros for the module.
 * Initialization and other General Macros offered by the module.
 */

/** CCP CE GTBS Enable Handle */
#define CCP_CE_GTBS_ENABLE                      0x80U

/** Total number of Characteristic IDs,
 * \ref ga_tbs_char_prop
 */
#define CCP_CHAR_ID_COUNT                               16U

/** \} */

/**
 * \name CCP Client Constants - CCP Char ID
 * \{
 * This section lists the Characteristic ID references.
 */

/** TBS Char ID: Bearer Provider Name,
 * \ref GA_CHAR_PROP_TBS_BRR_PROVIDER_NAME
 */
#define CCP_CHAR_ID_BRR_PROVIDER_NAME                   0U
/** TBS Char ID: Bearer Uniform Caller Identifier (UCI),
 * \ref GA_CHAR_PROP_TBS_BRR_UCI
 */
#define CCP_CHAR_ID_BRR_UCI                             1U
/** TBS Char ID: Bearer Technology,
 * \ref GA_CHAR_PROP_TBS_BRR_TECHNOLOGY
 */
#define CCP_CHAR_ID_BRR_TECHNOLOGY                      2U
/** TBS Char ID: Bearer URI Schemes Supported List,
 * \ref GA_CHAR_PROP_TBS_BRR_URI_SCHMS_SUPPORT_LIST
 */
#define CCP_CHAR_ID_BRR_URI_SCHMS_SUPPORT_LIST          3U
/** TBS Char ID: Bearer Signal Strength,
 * \ref GA_CHAR_PROP_TBS_BRR_SIGSTRENGTH
 */
#define CCP_CHAR_ID_BRR_SIGSTRENGTH                     4U
/** TBS Char ID: Bearer Signal Strength Reporting Interval,
 * \ref GA_CHAR_PROP_TBS_BRR_SIGSTRENGTH_REPORTING_INTERVAL
 */
#define CCP_CHAR_ID_BRR_SIGSTRENGTH_REPORTING_INTERVAL  5U
/** TBS Char ID: Bearer List Current Calls,
 * \ref GA_CHAR_PROP_TBS_BRR_LIST_CUR_CALLS
 */
#define CCP_CHAR_ID_BRR_LIST_CUR_CALLS                  6U
/** TBS Char ID: Content Control ID (CCID),
 * \ref GA_CHAR_PROP_TBS_CONTENT_CONTROL_ID
 */
#define CCP_CHAR_ID_CONTENT_CONTROL_ID                  7U
/** TBS Char ID: Status Flags,
 * \ref GA_CHAR_PROP_TBS_STATUS_FLAGS
 */
#define CCP_CHAR_ID_STATUS_FLAGS                        8U
/** TBS Char ID: Incoming Call Target Bearer URI,
 * \ref GA_CHAR_PROP_TBS_INCOMING_CALL_TARGET_BRR_URI
 */
#define CCP_CHAR_ID_INCOMING_CALL_TARGET_BRR_URI        9U
/** TBS Char ID: Call State,
 * \ref GA_CHAR_PROP_TBS_CALL_STATE
 */
#define CCP_CHAR_ID_CALL_STATE                          10U
/** TBS Char ID: Call Control Point,
 * \ref GA_CHAR_PROP_TBS_CALL_CP
 */
#define CCP_CHAR_ID_CALL_CP                             11U
/** TBS Char ID: Call Control Point Optional Opcodes,
 * \ref GA_CHAR_PROP_TBS_CALL_CP_OPTIONAL_OPCODES
 */
#define CCP_CHAR_ID_CALL_CP_OPTIONAL_OPCODES            12U
/** TBS Char ID: Termination Reason,
 * \ref GA_CHAR_PROP_TBS_TERMINATION_REASON
 */
#define CCP_CHAR_ID_TERMINATION_REASON                  13U
/** TBS Char ID: Incoming Call,
 * \ref GA_CHAR_PROP_TBS_INCOMING_CALL
 */
#define CCP_CHAR_ID_INCOMING_CALL                       14U
/** TBS Char ID: Call Friendly Name,
 * \ref GA_CHAR_PROP_TBS_CALL_FRIENDLY_NAME
 */
#define CCP_CHAR_ID_CALL_FRIENDLY_NAME                  15U

/** \} */

/** \} */

/**
 * \defgroup ga_ccp_ce_utility_macros Macros
 * \{
 * \brief This section describers Initialization and other Macros
 * for the module.
 */

/** Validate CCP Handle refers to GTBS Macro */
#define CCP_CE_IS_HANDLE_GTBS(x) \
    (CCP_CE_GTBS_ENABLE & (x))

/** Set Generic service to handle */
#define CCP_CE_SET_HANDLE_GTBS(x) \
        (x) = (CCP_CE_GTBS_ENABLE | (x))

/** Derive the characteristic config mask for the Char ID */
#define CCP_CHAR_CONFIG(id)                             (1U << (id))

/** \} */

/**
 * \defgroup ga_ccp_ce_events Events
 * \{
 * \brief This section lists the Synchronous/Asynchronous Events notified to
 * Application by the Module through the callback \ref CCP_CE_NTF_CB
 * callback.
 */

/**
 * \name CCP Client Events - Setup
 * \{
 * \brief This section describes the CCP Setup Events for Client.
 */

/**
 * \brief This event indicates the setup of a GTBS Session.
 * A handle to the GTBS Session is derived and provided to the
 * application, this Handle to be further used during all CCP Client related
 * requests. This involves discovery of service, char
 * and descriptors. Once done, the configuration for Notifications is
 * also enabled.
 * Callback is triggered with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_SETUP_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS Setup Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ccp_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ccp_data - NULL \n
 *                           .
 * \param [in] ccp_datalen  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ccp_datalen - 0 \n
 *                           .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_SETUP_CNF                                0x00U

/**
 * \brief This Event is notified on discovery of optional TBS(s) with peer
 * device with the following values as parameters in the \ref CCP_CE_NTF_CB
 * callback.
 * This event is being notifed continuously for each discovered instance of
 * TBS with status \ref GA_CONTINUE and on notification of all the TBS
 * instance.
 * The event is notified with status \ref GA_SUCCESS, it indicates
 * the completion of the optional TBS service discovery.
 * In case of failure, \ref GA_FAILURE is triggered.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_DISCOVER_TBS_CNF
 * \param [in] ccp_status  \ref GA_CONTINUE Service Handle range of
 *                         discovered TBS \n
 *                     \ref GA_SUCCESS Service Discovery Complete \n
 *                     \ref GA_FAILURE Others \n
 * \param [in] ccp_data  If status is \ref GA_CONTINUE \n
 *                          - ccp_data - Pointer to object of type
 *                            \ref GA_BRR_SVC_INFO \n
 * \param [in] ccp_datalen  If status is \ref GA_CONTINUE \n
 *                             - ccp_datalen - Size of \ref GA_BRR_SVC_INFO \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_DISCOVER_TBS_CNF                         0x03U

/**
 * \brief This event indicates the setup of a TBS Session.
 * A handle to the TBS Session is derived and provided to the application, this
 * Handle to be further used during all CCP Client related requests. The Char
 * and Desc Discovery is performed post which the Configuration of Notification
 * of Char is done. Callback is triggered with the following values as
 * parameters in the \ref CCP_CE_NTF_CB callback
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_SETUP_TBS_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS Setup Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] ccp_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ccp_data - NULL \n
 *                           .
 * \param [in] ccp_datalen  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - ccp_datalen - 0 \n
 *                           .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_SETUP_TBS_CNF                            0x04U

/** \} */

/**
 * \name CCP Client Events - Read
 * \{
 * \brief This event is notified whenever read request is successful and a
 * response is received from the peer.
 */

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_BRR_PROVIDER_NAME_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                       data: Represents a UTF-8 String
 *                       datalen: Length of UTF-8 String
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_BRR_PROVIDER_NAME_CNF                 0x05U

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_BRR_UCI_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                       data: Represents a UTF-8 String,
 *                       Refer \ref ga_ccp_se_constants
 *                       datalen: Length of UTF-8 String
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_BRR_UCI_CNF                           0x06U

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_BRR_TECH_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                       data: Represents a \ref UINT8,
 *                       Refer \ref ga_ccp_se_constants
 *                       datalen: Length of \ref UINT8
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_BRR_TECH_CNF                          0x07U

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_BRR_URI_SCHMS_LIST_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO ,
 *                       data: Represents a UTF-8 String,
 *                       Refer \ref ga_ccp_se_constants
 *                       datalen: Length of UTF-8 String
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_BRR_URI_SCHMS_LIST_CNF                0x08U

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_BRR_SIG_STRNTH_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                       data: Represents a \ref UINT8
 *                       datalen: Length of \ref UINT8
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_BRR_SIG_STRNTH_CNF                    0x09U

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_BRR_SIG_STRTH_RPT_INTVAL_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                       data: Represents a \ref UINT8
 *                       datalen: Length of \ref UINT8
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_BRR_SIG_STRTH_RPT_INTVAL_CNF          0x0AU

/**
 * \brief This event is notified when a write response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_SET_BRR_SIG_STRTH_RPT_INTVAL_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Write Response received
 * \param [in] ccp_data  NULL \n
 * \param [in] ccp_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_SET_BRR_SIG_STRTH_RPT_INTVAL_CNF         0x0BU

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_BRR_LIST_CUR_CALLS_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO ,
 *                       data: If calls are present in the current call list: \n
 *                          - Pointer to \ref CCP_EVENT_INFO with format
 *                            as below:
 *                               - List_Item_Length ( \ref UINT8 ) \n
 *                               - Call_Index ( \ref UINT8 ) \n
 *                               - Call_State ( \ref UINT8 ) \n
 *                                 \ref ga_ccp_se_constants
 *                               - Call_Flags ( \ref UINT8 ) \n
 *                                 \ref ga_ccp_se_constants
 *                               - Call_URI (UTF-8 string) \n
 *                               .
 *                          .
 *                        datalen: 3 + length of UTF-8 String
 *                       Else, \n
 *                       data: NULL \n
 *                       datalen: 0 \n
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_BRR_LIST_CUR_CALLS_CNF                0x0CU

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_CNTNT_CTRL_ID_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 * \param [in] ccp_datalen
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_CNTNT_CTRL_ID_CNF                     0x0DU

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_STATUS_FLAGS_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                       data: Represents a \ref UINT16
 *                       datalen: Length of \ref UINT16
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_STATUS_FLAGS_CNF                      0x0EU

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_INCALL_TRGT_BRR_URI_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                       data:
 *                          - Call_Index ( \ref UINT8 ) \n
 *                          - Incoming_Call_Target_URI (UTF-8 string) \n
 *                          .
 *                       datalen: Length of \ref UINT8 + Length of UTF-8 String
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_INCALL_TRGT_BRR_URI_CNF               0x0FU

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_CALL_STATE_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  If active calls are present in the call list: \n
 *                          - Pointer to \ref CCP_EVENT_INFO,
 *                            data:
 *                               - Call_Index ( \ref UINT8 ) \n
 *                               - Call_State ( \ref UINT8 )
 *                                 \ref ga_ccp_se_constants \n
 *                               - Call_Flags ( \ref UINT8 )
 *                                 \ref ga_ccp_se_constants \n
 *                               .
 *                            datalen: 3
 *                          .
 *                       Else,
 *                            data:  NULL \n
 *                            datalen: 0
 * \param [in] ccp_datalen  sizeof ( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_CALL_STATE_CNF                        0x10U

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_CALL_CP_OPT_OPCODES_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                            data: \ref UINT16
 *                            datalen: 2
 * \param [in] ccp_datalen  sizeof ( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_CALL_CP_OPT_OPCODES_CNF               0x11U

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_INCOMING_CALL_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                            data: Call_Index \ref UINT8
 *                                  URI - Represented as UTF-8 String \n
 *                            datalen: 1 + Length of UTF-8 string
 * \param [in] ccp_datalen  sizeof ( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_INCOMING_CALL_CNF                     0x12U

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RD_CALL_FRIENDLY_NAME_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Read Response received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                          data: Call_Index ( \ref UINT8 ) \n
 *                                Friendly_Name (UTF-8 string) \n
 *                          datalen: 1 + Length of UTF-8 string
 * \param [in] ccp_datalen  sizeof ( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RD_CALL_FRIENDLY_NAME_CNF                0x13U

/** \} */

/**
 * \name CCP Client Events - Write
 * \{
 * \brief The event is notified whenever write is successful and a write
 * response is received from peer.
 * This applies to only requests that is triggered with write type -
 * Write Request.
 */


/**
 * \brief This event is notified when a write response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_ACCEPT_CALL_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Write Response received
 * \param [in] ccp_data  NULL \n
 * \param [in] ccp_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_ACCEPT_CALL_CNF                          0x14U

/**
 * \brief This event is notified when a write response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_TERMINATE_CALL_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Write Response received
 * \param [in] ccp_data  NULL \n
 * \param [in] ccp_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_TERMINATE_CALL_CNF                       0x15U

/**
 * \brief This event is notified when a write response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_LOCAL_HOLD_CALL_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Write Response received
 * \param [in] ccp_data  NULL \n
 * \param [in] ccp_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_LOCAL_HOLD_CALL_CNF                      0x16U

/**
 * \brief This event is notified when a write response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_LOCAL_RETRIEVE_CALL_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Write Response received
 * \param [in] ccp_data  NULL \n
 * \param [in] ccp_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_LOCAL_RETRIEVE_CALL_CNF                  0x17U

/**
 * \brief This event is notified when a write response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_ORGINATE_CALL_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Write Response received
 * \param [in] ccp_data  NULL \n
 * \param [in] ccp_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_ORGINATE_CALL_CNF                        0x18U

/**
 * \brief This event is notified when a write response is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_JOIN_CALL_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Write Response received
 * \param [in] ccp_data  NULL \n
 * \param [in] ccp_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_JOIN_CALL_CNF                            0x19U

/** \} */

/**
 * \name CCP Client Events - Notification
 * \{
 * \brief This event is notified when the char is configured for any
 * notification change and a notification has occurred from the peer device.
 */

/**
 * \brief This event is notified when a notification is received for Bearer
 * Provider Name characteristic from peer device with the following values as
 * parameters in the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_BRR_PROVIDER_NAME_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                       data: Represents a UTF-8 String
 *                       datalen: Length of UTF-8 String
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_BRR_PROVIDER_NAME_NTF                    0x1AU

/**
 * \brief This event is notified when a notification is received for Bearer
 * Technology characteristic from peer device with the following values as
 * parameters in the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_BRR_TECH_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                       data: Represents a \ref UINT8,
 *                       Refer \ref ga_ccp_se_constants
 *                       datalen: Length of \ref UINT8
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_BRR_TECH_NTF                             0x1BU

/**
 * \brief This event is notified when a notification is received for Bearer
 * Signal Strength characteristic from peer device with the following values as
 * parameters in the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_BRR_SIG_STRNTH_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                       data: Represents a \ref UINT8
 *                       datalen: Length of \ref UINT8
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_BRR_SIG_STRNTH_NTF                       0x1CU

/**
 * \brief This event is notified when a notification is received for Bearer
 * current calls characteristic from peer device with the following values as
 * parameters in the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_BRR_LIST_CUR_CALLS_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO ,
 *                       data: If calls are present in the current call list: \n
 *                          - Pointer to \ref CCP_EVENT_INFO with format
 *                            as below:
 *                               - List_Item_Length ( \ref UINT8 ) \n
 *                               - Call_Index ( \ref UINT8 ) \n
 *                               - Call_State ( \ref UINT8 ) \n
 *                                 \ref ga_ccp_se_constants
 *                               - Call_Flags ( \ref UINT8 ) \n
 *                                 \ref ga_ccp_se_constants
 *                               - Call_URI (UTF-8 string) \n
 *                               .
 *                          .
 *                        datalen: 3 + length of UTF-8 String
 *                       Else, \n
 *                       data: NULL \n
 *                       datalen: 0 \n
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_BRR_LIST_CUR_CALLS_NTF                   0x1DU

/**
 * \brief This event is notified when a notification is received for Status
 * Flags characteristic from peer device with the following values as
 * parameters in the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_STATUS_FLAGS_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO ,
 *                       expressed as \ref UINT16
 * \param [in] ccp_datalen  \ref sizeof ( \ref UINT16 )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_STATUS_FLAGS_NTF                         0x1EU

/**
 * \brief This event is notified when a notification is received for Incoming
 * Call Target Bearer characteristic from peer device with the following values
 * as parameters in the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_INCALL_TRGT_BRR_URI_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                       data:
 *                          - Call_Index ( \ref UINT8 ) \n
 *                          - Incoming_Call_Target_URI (UTF-8 string) \n
 *                          .
 *                       datalen: Length of \ref UINT8 + Length of UTF-8 String
 * \param [in] ccp_datalen  sizeof( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_INCALL_TRGT_BRR_URI_NTF                  0x1FU

/**
 * \brief This event is notified when a notification is received for Call State
 * characteristic from peer device with the following values as parameters in
 * the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_CALL_STATE_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  If active calls are present in the call list: \n
 *                          - Pointer to \ref CCP_EVENT_INFO,
 *                            data:
 *                               - Call_Index ( \ref UINT8 ) \n
 *                               - Call_State ( \ref UINT8 )
 *                                 \ref ga_ccp_se_constants \n
 *                               - Call_Flags ( \ref UINT8 )
 *                                 \ref ga_ccp_se_constants \n
 *                               .
 *                            datalen: 3
 *                          .
 *                       Else,
 *                            data:  NULL \n
 *                            datalen: 0
 * \param [in] ccp_datalen  sizeof ( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_CALL_STATE_NTF                           0x20U

/**
 * \brief This event is notified when a notification is received for Call
 * Control Point characteristic from peer device with the following values as
 * parameters in the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_CALL_CP_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                            data:
 *                               - Requested Opcode ( \ref UINT8 ) \n
 *                                 \ref ga_ccp_se_constants
 *                               - Call Index ( \ref UINT8 ) \n
 *                               - Result Code ( \ref UINT8 )
 *                                 \ref ga_ccp_se_constants \n
 *                               .
 *                            datalen: 3
 * \param [in] ccp_datalen  sizeof ( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_CALL_CP_NTF                              0x21U

/**
 * \brief This event is notified when a notification is received for Call
 * Termination reason characteristic from peer device with the following values
 * as parameters in the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_TERMINATION_REASON_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                            data:
 *                               - Call Index ( \ref UINT8 ) \n
 *                               - Reason Code ( \ref UINT8 )
 *                                 \ref ga_ccp_se_constants \n
 *                               .
 *                            datalen: 2
 * \param [in] ccp_datalen  sizeof ( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_TERMINATION_REASON_NTF                   0x22U

/**
 * \brief This event is notified when a notification is received for Incoming
 * Call characteristic from peer device with the following values as parameters
 * in the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_INCOMING_CALL_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                            data: Call_Index \ref UINT8
 *                                  URI - Represented as UTF-8 String \n
 *                            datalen: 1 + Length of UTF-8 string
 * \param [in] ccp_datalen  sizeof ( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_INCOMING_CALL_NTF                        0x23U

/**
 * \brief This event is notified when a notification is received for Call
 * Friendly Name characteristic from peer device with the following values as
 * parameters in the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_CALL_FRIENDLY_NAME_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                          data: Call_Index ( \ref UINT8 ) \n
 *                                Friendly_Name (UTF-8 string) \n
 *                          datalen: 1 + Length of UTF-8 string
 * \param [in] ccp_datalen  sizeof ( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_CALL_FRIENDLY_NAME_NTF                   0x24U

/**
 * \brief This event is notified when a notification is received for Call
 * Bearer URI Schemes Supported List characteristic from peer device with the
 * following values as parameters in the \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_BRR_URI_SCHMS_LIST_NTF
 * \param [in] ccp_status  \ref GA_SUCCESS : Notification received
 * \param [in] ccp_data  Pointer to \ref CCP_EVENT_INFO,
 *                          data: Call_Index ( \ref UINT8 ) \n
 *                                Bearer URI Schemes Supported List
                                  (UTF-8 string) \n
 *                          datalen: 1 + Length of UTF-8 string
 * \param [in] ccp_datalen  sizeof ( \ref CCP_EVENT_INFO )
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_BRR_URI_SCHMS_LIST_NTF                   0x25U

/** \} */

/**
 * \name CCP Client Events - Release
 * \{
 * \brief This section describes the Call Control Profile Release
 * Events for CCP CE.
 */

/**
 * \brief This event is notified when a GTBS Service context is released.
 * Configuration of Notifications for char are disabled and followed by release
 * of GTBS context.
 * Callback is triggered with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RELEASE_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Release completed \n
 *                        \ref GA_FAILURE : Release Failed \n
 * \param [in] ccp_data  NULL \n
 * \param [in] ccp_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RELEASE_CNF                              0x01U

/**
 * \brief This event is notified when a TBS Service context is released.
 * Configuration of Notifications for char are disabled and followed by release
 * of TBS context.
 * Callback is triggered with the following values as parameters in the
 * \ref CCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ccp_event  \ref CCP_CE_RELEASE_TBS_CNF
 * \param [in] ccp_status  \ref GA_SUCCESS : Release completed \n
 *                        \ref GA_FAILURE : Release Failed \n
 * \param [in] ccp_data  NULL \n
 * \param [in] ccp_datalen  0
 *
 * \return \ref GA_SUCCESS (always)
 */
#define CCP_CE_RELEASE_TBS_CNF                          0x02U

/** \} */

/** \} */

/**
 * \addtogroup ga_ccp_ce_constants
 * \{
 */

/**
 * \name CCP Client Constants - Call State Char Value
 * \{
 * \brief This section lists the Call State characteristic value to be used as
 * defined in TBS specification.
 */

/**
 * TBS Call State: Incoming \n
 * A remote party is calling (incoming call)
 */
#define CCP_CALL_STATE_INCOMING                         0x00

/**
 * TBS Call State: Dialing \n
 * The process to call the remote party has started on the server,
 * but the remote party is not being alerted (outgoing call)
 */
#define CCP_CALL_STATE_DIALING                          0x01U

/**
 * TBS Call State: Alerting \n
 * A remote party is being alerted (outgoing call)
 */
#define CCP_CALL_STATE_ALERTING                         0x02U

/**
 * TBS Call State: Active \n
 * The call is in an active conversation
 */
#define CCP_CALL_STATE_ACTIVE                           0x03U

/**
 * TBS Call State: Locally Held \n
 * The call is connected but held locally. Locally Held implies that
 * either the server or the client can affect the state
 */
#define CCP_CALL_STATE_L_HELD                           0x04U

/**
 * TBS Call State: Remotely Held \n
 * The call is connected but held remotely. Remotely Held means that
 * the state is controlled by the remote party of a call
 */
#define CCP_CALL_STATE_R_HELD                           0x05U

/**
 * TBS Call State: Locally and Remotely Held \n
 * The call is connected but held both locally and remotely
 */
#define CCP_CALL_STATE_LR_HELD                          0x06U

/** TBS Call State: Invalid */
#define CCP_CALL_STATE_INVALID                          0xFFU

/** \} */

/**
 * \name CCP Client Constants - Termination Reason Char Value
 * \{
 * \brief This section lists the Termination Reason characteristic value to be
 * used, as defined in TBS specification.
 */

/**
 * TBS Termination Reason: URI value used to originate a call was
 * formed improperly
 */
#define CCP_CALL_TER_RSN_URI_WRONG                      0x00U

/** TBS Termination Reason: Call failed */
#define CCP_CALL_TER_RSN_CALL_FAILED                    0x01U

/** TBS Termination Reason: Remote party ended the call */
#define CCP_CALL_TER_RSN_R_ENDED_CALL                   0x02U

/** TBS Termination Reason: Call ended from the server */
#define CCP_CALL_TER_RSN_SRV_ENDED_CALL                 0x03U

/** TBS Termination Reason: Line was busy */
#define CCP_CALL_TER_RSN_LINE_BUSY                      0x04U

/** TBS Termination Reason: Network congestion */
#define CCP_CALL_TER_RSN_NWT_CNGSTN                     0x05U

/** TBS Termination Reason: Client terminated the call */
#define CCP_CALL_TER_RSN_CLI_TER_CALL                   0x06U

/** TBS Termination Reason: No service */
#define CCP_CALL_TER_RSN_CLI_NO_SERVICE                 0x07U

/** TBS Termination Reason: No answer */
#define CCP_CALL_TER_RSN_CLI_NO_ANSWER                  0x08U

/** TBS Termination Reason: Unspecified */
#define CCP_CALL_TER_RSN_UNSPECIFIED                    0x09U

/** \} */

/**
 * \name CCP Client Constants - CCP_CP_OPCODES
 * \{
 * \brief This section lists the type of CCP Opcodes used during CCP Write
 * request.
 * As defined in TBS Specification.
 */

/** CCP Opcode - Accept */
#define CCP_CP_OP_ACCEPT                                0x00U
/** CCP Opcode - Terminate */
#define CCP_CP_OP_TERMINATE                             0x01U
/** CCP Opcode - Local Hold */
#define CCP_CP_OP_L_HLD                                 0x02U
/** CCP Opcode - Local Retrieve */
#define CCP_CP_OP_L_RTRV                                0x03U
/** CCP Opcode - Originate */
#define CCP_CP_OP_ORIGINATE                             0x04U
/** CCP Opcode - Join */
#define CCP_CP_OP_JOIN                                  0x05U

/** \} */

/**
 * \name CCP Client Constants - Call_Flags Field Value
 * \{
 * \brief This section lists the CCP Call_Flags Field Value to be used as
 * defined in TBS specification.
 */

/** CCP Call_Flags Field Value: Outgoing */
#define CCP_CALL_FLAGS_OUTGOING                         0x01U
/** CCP Call_Flags Field Value: Withheld */
#define CCP_CALL_FLAGS_WITHHELD                         0x02U
/** CCP Call_Flags Field Value: Withheld by network */
#define CCP_CALL_FLAGS_WITHHELD_BT_NTW                  0x04U

/** \} */

/**
 * \name CCP Client Constants - Uniform Call Identifier(UCI) Values
 * \{
 * \brief This section lists the CCP Uniform Call Identifier(UCI) Values to be
 * used. \n
 * Defined in Assigned numbers.
 */

/** CCP Uniform Call Identifier(UCI) ID : skype */
#define CCP_UCI_ID_SKYPE                                "skype"
/** CCP Uniform Call Identifier(UCI) ID : lync */
#define CCP_UCI_ID_LYNC                                 "lync"
/** CCP Uniform Call Identifier(UCI) ID : jabr */
#define CCP_UCI_ID_JABR                                 "jabr"
/** CCP Uniform Call Identifier(UCI) ID : bbv */
#define CCP_UCI_ID_BBV                                  "bbv"
/** CCP Uniform Call Identifier(UCI) ID : eyebm */
#define CCP_UCI_ID_EYEBM                                "eyebm"
/** CCP Uniform Call Identifier(UCI) ID : wbrtc */
#define CCP_UCI_ID_WBRTC                                "wbrtc"
/** CCP Uniform Call Identifier(UCI) ID : qq */
#define CCP_UCI_ID_QQ                                   "qq"
/** CCP Uniform Call Identifier(UCI) ID : spika */
#define CCP_UCI_ID_SPIKA                                "spika"
/** CCP Uniform Call Identifier(UCI) ID : gtalk */
#define CCP_UCI_ID_GTALK                                "gtalk"
/** CCP Uniform Call Identifier(UCI) ID : fbch */
#define CCP_UCI_ID_FBCH                                 "fbch"
/** CCP Uniform Call Identifier(UCI) ID : tgrm */
#define CCP_UCI_ID_TGRM                                 "tgrm"
/** CCP Uniform Call Identifier(UCI) ID : lne */
#define CCP_UCI_ID_LNE                                  "lne"
/** CCP Uniform Call Identifier(UCI) ID : wtsap */
#define CCP_UCI_ID_WTSAP                                "wtsap"
/** CCP Uniform Call Identifier(UCI) ID : vbr */
#define CCP_UCI_ID_VBR                                  "vbr"
/** CCP Uniform Call Identifier(UCI) ID : mme */
#define CCP_UCI_ID_MME                                  "mme"
/** CCP Uniform Call Identifier(UCI) ID : kkt */
#define CCP_UCI_ID_KKT                                  "kkt"
/** CCP Uniform Call Identifier(UCI) ID : wcht */
#define CCP_UCI_ID_WCHT                                 "wcht"
/** CCP Uniform Call Identifier(UCI) ID : tgo */
#define CCP_UCI_ID_TGO                                  "tgo"
/** CCP Uniform Call Identifier(UCI) ID : kik */
#define CCP_UCI_ID_KIK                                  "kik"
/** CCP Uniform Call Identifier(UCI) ID : nbuz */
#define CCP_UCI_ID_NBUZ                                 "nbuz"
/** CCP Uniform Call Identifier(UCI) ID : hgus */
#define CCP_UCI_ID_HGUS                                 "hgus"
/** CCP Uniform Call Identifier(UCI) ID : con */
#define CCP_UCI_ID_CON                                  "con"
/** CCP Uniform Call Identifier(UCI) ID : mngr */
#define CCP_UCI_ID_MNGR                                 "mngr"
/** CCP Uniform Call Identifier(UCI) ID : chsec */
#define CCP_UCI_ID_CHSEC                                "chsec"
/** CCP Uniform Call Identifier(UCI) ID : icht */
#define CCP_UCI_ID_ICHT                                 "icht"
/** CCP Uniform Call Identifier(UCI) ID : rnds */
#define CCP_UCI_ID_RNDS                                 "rnds"
/** CCP Uniform Call Identifier(UCI) ID : imo */
#define CCP_UCI_ID_IMO                                  "imo"
/** CCP Uniform Call Identifier(UCI) ID : qik */
#define CCP_UCI_ID_QIK                                  "qik"
/** CCP Uniform Call Identifier(UCI) ID : lbn */
#define CCP_UCI_ID_LBN                                  "lbn"
/** CCP Uniform Call Identifier(UCI) ID : vonm */
#define CCP_UCI_ID_VONM                                 "vonm"
/** CCP Uniform Call Identifier(UCI) ID : ov */
#define CCP_UCI_ID_OV                                   "ov"
/** CCP Uniform Call Identifier(UCI) ID : un000 to un999 */
#define CCP_UCI_ID_UN000                                "un000"

/** \} */

/**
 * \name CCP Client Constants - Uniform Resource Identifier(URI) Values
 * \{
 * \brief This section lists the CCP Uniform Resource Identifier(URI) Values to be
 * used. \n Defined in Assigned numbers.
 * \note Took only the selected URI, as there are too many Id's
 */

/** CCP Uniform Resource Identifier(URI) Scheme Name String- mailto: */
#define CCP_URI_ID_MAILTO                               "mailto:"
/** CCP Uniform Resource Identifier(URI) Scheme Name String- tel: */
#define CCP_URI_ID_TEL                                  "tel:"

/** \} */

/**
 * \name CCP Client Constants - Bearer Technology ID
 * \{
 * \brief This section lists the Bearer Technology IDs used as defined in
 * Assigned numbers.
 */

/** CCP Bearer Technology ID : 3G */
#define CCP_BRR_TECH_3G                                 0x01U
/** CCP Bearer Technology ID : 4G */
#define CCP_BRR_TECH_4G                                 0x02U
/** CCP Bearer Technology ID : LTE */
#define CCP_BRR_TECH_LTE                                0x03U
/** CCP Bearer Technology ID : WIFI */
#define CCP_BRR_TECH_WIFI                               0x04U
/** CCP Bearer Technology ID : 5G */
#define CCP_BRR_TECH_5G                                 0x05U
/** CCP Bearer Technology ID : GSM */
#define CCP_BRR_TECH_GSM                                0x06U
/** CCP Bearer Technology ID : CDMA */
#define CCP_BRR_TECH_CDMA                               0x07U
/** CCP Bearer Technology ID : 2G */
#define CCP_BRR_TECH_2G                                 0x08U
/** CCP Bearer Technology ID : WCDMA */
#define CCP_BRR_TECH_WCDMA                              0x09U
/** CCP Bearer Technology ID : IP */
#define CCP_BRR_TECH_IP                                 0x0AU

/** \} */

/**
 * \name CCP Client Constants - Bearer Signal Strength
 * \{
 * \brief This section lists the Bearer Signal Strength
 */

/** CCP Bearer Signal Strength: No Service */
#define CCP_BRR_SIG_NO_SERVICE                          0x00U
/** CCP Signal Strength - Max Strength */
#define CCP_BRR_SIG_MAX_STRENGTH                        0x64U
/** CCP Bearer Signal Strength: Unavailable */
#define CCP_BRR_SIG_UNAVAILABLE                         0xFFU

/** \} */

/**
 * \name CCP Client Constants - Service Type
 * \{
 * \brief This section lists the CCP service types used as defined in
 * TBS specification.
 */

/** CCP Service Type: Generic Telephone Bearer Service (GTBS) */
#define CCP_SVS_TYPE_GTBS                               0x00U
/** CCP Service Type: Telephone Bearer Service (TBS) */
#define CCP_SVS_TYPE_TBS                                0x01U

/** \} */

/**
 * \name CCP Client Constants - CCP_CP Result Code
 * \{
 * \brief This section lists the type of CCP Result Codes used
 * as defined in TBS Specification.
 */

/**
 * CCP_CP Result Code: SUCCESS \n
 * The opcode write was successful
 */
#define CCP_CP_NTF_RESULT_SUCCESS                       0x00U

/**
 * CCP_CP Result Code: OPCODE NOT SUPPORTED \n
 * An invalid opcode was used for the Call Control Point write
 */
#define CCP_CP_NTF_RESULT_OPCODE_NOT_SUPPORTED          0x01U

/**
 * CCP_CP Result Code: OPERATION NOT POSSIBLE \n
 * The requested operation cannot be completed
 */
#define CCP_CP_NTF_RESULT_OP_NOT_POSSIBLE               0x02U

/**
 * CCP_CP Result Code: INVALID CALL INDEX \n
 * The Call Index used for the Call Control Point write is invalid
 */
#define CCP_CP_NTF_RESULT_INVALID_CALL_INDEX            0x03U

/**
 * CCP_CP Result Code: STATE MISMATCH \n
 * The opcode written to the Call Control Point was received when the
 * current Call State for the Call Index was not in the expected state
 */
#define CCP_CP_NTF_RESULT_STATE_MISMATCH                0x04U

/**
 * CCP_CP Result Code: LACK OF RESOURCES \n
 * Lack of internal resources to complete the requested action
 */
#define CCP_CP_NTF_RESULT_LACK_OF_RESOURCES             0x05U

/**
 * CCP_CP Result Code: INVALID OUTGOING URI \n
 * The Outgoing URI is incorrect or invalid when an Originate opcode is sent
 */
#define CCP_CP_NTF_RESULT_INVALID_OUTGOING_URI          0x06U

/** \} */

/**
 * \name CCP Client Constants - Application Error Codes
 * \{
 * \brief This section describes application error codes,
 * as defined in TBS Specification.
 */

/**
  * Value Changed During Read Long - A characteristic value has changed while
  * a Read Long Value Characteristic sub-procedure is in progress
  */
#define CCP_ERR_CODE_VALUE_CHANGED_DURING_READ_LONG                  0x80U

/** \} */

/** \} */

/* --------------------------------------------- Data types /Structures */
/**
 * \defgroup ga_ccp_ce_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by the module.
 */

/** CCP Handle */
typedef UCHAR   CCP_HANDLE;

/** CCP Event Information */
typedef struct _CCP_EVENT_INFO_
{
    /** Service Type */
    UCHAR srvc_type;

    /** Service Handle */
    UCHAR srvc_handle;

    /** Char Info. */
    GA_BRR_CHR_CONTEXT *chr;

    /** Event Data */
    void  *data;

    /** Event Data Length */
    UINT16  datalen;

}CCP_EVENT_INFO;

/** CCP Response Information */
typedef struct _CCP_RSP_INFO
{
    /** Char Info */
    GA_BRR_CHR_CONTEXT * chr;

    /** Data */
    UCHAR * data;

    /** Data Length */
    UCHAR datalen;

}CCP_RSP_INF;

/** \} */

/** \} */
/** \} */

/**
 * \defgroup ga_ccp_cb Application Callback
 * \{
 * \brief This section describes the application callback for CCP.
 */

/**
 * \defgroup ga_ccp_ce_cb CCP CE (Call Control Profile) Client Entity
 * \{
 * \brief This section describes the application callback for CCP CE.
 *
 */

/**
 * \brief CCP Notification Callback Type
 * \param [in] device      Peer Device Info
 * \param [in] ccp_event   Event type
 * \param [in] ccp_status  Event status
 * \param [in] ccp_data    EventData
 * \param [in] ccp_datalen Event Data Length
 */
typedef GA_RESULT (* CCP_CE_NTF_CB)
                  (
                      /* IN */ GA_ENDPOINT * device,
                      /* IN */ UINT8         ccp_event,
                      /* IN */ UINT16        ccp_status,
                      /* IN */ void        * ccp_data,
                      /* IN */ UINT16        ccp_datalen
                  );

/** \} */
/** \} */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CCP_CLIENT

/**
 * \defgroup ga_ccp_api_sequences API Sequences
 * \{
 * \brief This section describes the API Sequences.
 */

/**
 * \defgroup ga_ccp_api_seq CCP API Sequences
 * \{
 * \brief This section describes the Call Control Profile API Sequences.
 * MSC depicting the flow of APIs and Events.
 * \mscfile CCP.signalling
 */

/** \} */
/** \} */

/**
 * \defgroup ga_ccp_api_defs API Definitions
 * \{
 * \brief This section describes the EtherMind Call Control Profile APIs.
 */

/**
 * \defgroup ga_ccp_ce_module_api CCP CE (Call Control Profile) Client Entity
 * \{
 * \brief This section describes the Call Control Profile APIs
 * for Client.
 */

/**
 * \name CCP Client APIs - Init
 * \{
 * \brief This section describes the Call Control Profile Init APIs
 * for Client.
 */

/**
 *  \brief To Initialize Call Control Client Module.
 *
 *  \par Description:
 *       This function enables to initiaze the Call Control Client.
 *       This function must be called (only once) before any other CCP CE
 *       APIs are used.
 *
 *  \param [in] cb
 *         Callback to be registered with CCP Client.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
GA_RESULT GA_ccp_ce_init(/* IN */ CCP_CE_NTF_CB cb);

/** \} */

/**
 * \name CCP Client APIs - Setup
 * \{
 * \brief This section describes the Call Control Profile Setup APIs
 * for Client.
 */

/**
 *  \brief To Discover the TBS(s) from the Peer.
 *
 *  \par Description:
 *       This function enables to discover TBS(s) supported by the peer device.
 *
 *  \param [in] device
 *         Peer Device Address Information.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_DISCOVER_TBS_CNF event
 *          will be triggered.
 *
 *  \note If TBS context has to be setup, \ref GA_ccp_ce_setup_tbs_context()
 *        has to be called with the service handle range received as part of
 *        this event.
 *
 *  \sa ga_ccp_error_code
 */
GA_RESULT GA_ccp_ce_discover_tbs
          (
              /* IN  */ GA_ENDPOINT * device
          );

/**
 *  \brief To allocate and setup a GTBS Call Control Context.
 *
 *  \par Description:
 *       This function enables to allocate GTBS context with peer device,
 *       discovers the GTBS and its characteristics.
 *       Configure characterstics for notification.
 *
 *  \param [in] device
 *         Peer Device Address Information.
 *
 *  \param [out] handle
 *         Allocated GTBS handle for GTBS.
 *         Application is expected to use this handle for all the further
 *         GTBS transactions with the peer device.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_SETUP_CNF event will be triggered.
 *
 *  \sa ga_ccp_error_code
 */
GA_RESULT GA_ccp_ce_setup_context
          (
              /* IN  */ GA_ENDPOINT *device,
              /* OUT */ CCP_HANDLE  *handle
          );

/**
 * \cond EXCLUDE_PART
 */

#ifdef CCP_SUPPORT_CONFIG_SELECTION
/**
 *  \brief To update the configuration mask
 *
 *  \par Description:
 *       This function helps to update the configuration setting of which
 *       of the TBS characteristics of a peer needs configuration at setup.
 *
 *  \param [in] config
 *         Bitmask of the Characteristic IDs for configuration
 *
 *  \return GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 *  \ref
 */
GA_RESULT GA_ccp_ce_update_ntf_configuration(UINT32 config);
#endif /* CCP_SUPPORT_CONFIG_SELECTION */

/**
 * \endcond
 */

/**
 *  \brief To allocate and setup a TBS Call Control Context.
 *
 *  \par Description:
 *       This function enables to allocate TBS context with the peer device,
 *       discover the TBS characteristics.
 *       Configure the characterstics for notification.
 *
 *  \param [in] device
 *         Peer Device Address Information.
 *
 *  \param [in] srvs_info
 *          TBS service information, containing the service handle range.
 *
 *  \param [out] handle
 *         Allocated CCP handle for TBS.
 *         Application is expected to use this handle for all the further TBS
 *         transactions with the peer device.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_SETUP_TBS_CNF event
 *          will be notified.
 *
 *  \sa ga_ccp_error_code
 */
GA_RESULT GA_ccp_ce_setup_tbs_context
          (
              /* IN  */ GA_ENDPOINT      * device,
              /* IN  */ GA_BRR_SVC_INFO  * srvs_info,
              /* OUT */ CCP_HANDLE       * handle
          );

/** \} */

/**
 * \name CCP Client APIs - Read
 * \{
 * \brief This section describes the Call Control Profile Read APIs
 * for Client.
 */

/**
 *  \brief To get Bearer Provider Name.
 *
 *  \par Description:
 *       This function enables to get thet GTBS/TBS Bearer Provider Name.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_BRR_PROVIDER_NAME_CNF event
 *          will be notified.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_brr_provider_name(handle)                     \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_BRR_PROVIDER_NAME_CNF)                       \
        )

/**
 *  \brief To get Bearer Uniform Caller Identifier(UCI).
 *
 *  \par Description:
 *       This function enables to get the identifier for the
 *       Telephone Bearer.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_BRR_UCI_CNF event
 *          will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_brr_uci(handle)                               \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_BRR_UCI_CNF)                                 \
        )

/**
 *  \brief To get Bearer Technology.
 *
 *  \par Description:
 *       This function enables to get the information about the type of
 *       Technology that is being used for the Telephone Bearer.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_BRR_TECH_CNF event
 *          will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_brr_technology(handle)                        \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_BRR_TECH_CNF)                                \
        )

/**
 *  \brief To get Bearer URI Schemes Supported List.
 *
 *  \par Description:
 *       This function enables to get the Bearer Supported URI Schemes List.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_BRR_URI_SCHMS_LIST_CNF event
 *          will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_brr_uci_schemes_supported_list(handle)        \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_BRR_URI_SCHMS_LIST_CNF)                      \
        )

/**
 *  \brief To get Bearer Signal Strength.
 *
 *  \par Description:
 *       This function enables to get the level of Telephone Bearer Signal.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_BRR_SIG_STRNTH_CNF event
 *          will be notified.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_signal_strength(handle)                       \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_BRR_SIG_STRNTH_CNF)                          \
        )

/**
 *  \brief To get Bearer Signal Strength Reporting Interval.
 *
 *  \par Description:
 *       This function enables to get the Bearer Signal Strength Reporting Interval
 *       (in seconds).
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_BRR_SIG_STRTH_RPT_INTVAL_CNF
 *          event will be notified.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_signal_reporting_interval(handle)             \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_BRR_SIG_STRTH_RPT_INTVAL_CNF)                \
        )

/**
 *  \brief To get Bearer Current Calls List.
 *
 *  \par Description:
 *       This function enables to get the list of current calls.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_BRR_LIST_CUR_CALLS_CNF event
 *          will be notified.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_brr_list_cur_calls(handle)                    \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_BRR_LIST_CUR_CALLS_CNF)                      \
        )

/**
 *  \brief To get Content Control Identifier.
 *
 *  \par Description:
 *       This function enables to get the Content Control Identifier associated
 *       with Bearer.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_CNTNT_CTRL_ID_CNF event
 *          will be notified.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_content_control_id(handle)                    \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_CNTNT_CTRL_ID_CNF)                           \
        )

/**
 *  \brief To get Status Flags.
 *
 *  \par Description:
 *       This function enables to get the current status of features
 *       that the server device supports with Bearer.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_STATUS_FLAGS_CNF event
 *          will be notified.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_status_flag(handle)                           \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_STATUS_FLAGS_CNF)                            \
        )

/**
 *  \brief To get Incoming call Target Bearer URI.
 *
 *  \par Description:
 *       This function enables to get the Call Index followed by URI of the
 *       Target of an Incoming Call.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_INCALL_TRGT_BRR_URI_CNF event
 *          will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_incall_trgt_brr_uri(handle)                   \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_INCALL_TRGT_BRR_URI_CNF)                     \
        )

/**
 *  \brief To get the Call state.
 *
 *  \par Description:
 *       This function enables to get the an array of members(each 3 octets)
 *       and represents an abstraction of the different states that telephone
 *       calls are in.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_CALL_STATE_CNF event
 *          will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_call_state(handle)                            \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_CALL_STATE_CNF)                              \
        )

/**
 *  \brief To get Call Optional Control Point Opcodes.
 *
 *  \par Description:
 *       This function enables to get 2 octets, that describes the optional
 *       opcodes of the Call Control Point.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_CALL_CP_OPT_OPCODES_CNF event
 *          is notified.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_call_cp_optional_opcodes(handle)              \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_CALL_CP_OPT_OPCODES_CNF)                     \
        )

/**
 *  \brief To get Incoming call details.
 *
 *  \par Description:
 *       This function enables to get the call index and URI of the
 *       incoming caller.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_INCOMING_CALL_CNF event
 *          will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_incoming_call(handle)                         \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_INCOMING_CALL_CNF)                           \
        )

/**
 *  \brief To get Call Friendly Name.
 *
 *  \par Description:
 *       This function enables to get the Call Index and friendly name
 *       of the incoming or outgoing call.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_RD_CALL_FRIENDLY_NAME_CNF event
 *          will be notified.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_get_call_friendly_name(handle)                    \
        ccp_ce_read_request                                         \
        (                                                           \
            (handle),                                               \
            (CCP_CE_RD_CALL_FRIENDLY_NAME_CNF)                      \
        )
/** \} */

/**
 * \name CCP Client APIs - Write
 * \{
 * \brief This section describes the Call Control Profile Write APIs
 * for Client.
 */

/**
 *  \brief To set signal reporting interval.
 *
 *  \par Description:
 *       This function enables to set the signal reporting interval.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [in] data
 *         Pointer to the signal strength value.
 *
 *  \param [in] wwr
 *         Flag to indicate Write or Write without Response.
 *          1 -> Write without response
 *          0 -> Write
 *
 *  \param [in] datalen
 *         Length of the signal strength value.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_SET_BRR_SIG_STRTH_RPT_INTVAL_CNF
 *          event is notified.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_set_signal_reporting_interval(handle, wwr, data, datalen) \
        ccp_ce_write_request                                                \
        (                                                                   \
            (handle),                                                       \
            (0xFF),                                                         \
            (wwr),                                                          \
            (data),                                                         \
            (datalen),                                                      \
            (CCP_CE_SET_BRR_SIG_STRTH_RPT_INTVAL_CNF)                       \
        )

/**
 *  \brief To accept incoming call.
 *
 *  \par Description:
 *       This function enables to accept the incoming call.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [in] wwr
 *         Flag to indicate Write or Write without Response.
 *          1 -> Write without response
 *          0 -> Write
 *
 *  \param [in] data
 *         Pointer to the call index to be accepted.
 *
 *  \param [in] datalen
 *         Parameter Length, one octet.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_ACCEPT_CALL_CNF event
 *          will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_accept_call(handle, wwr, data, datalen)                   \
        ccp_ce_write_request                                                \
        (                                                                   \
            (handle),                                                       \
            (CCP_CP_OP_ACCEPT),                                             \
            (wwr),                                                          \
            (data),                                                         \
            (datalen),                                                      \
            (CCP_CE_ACCEPT_CALL_CNF)                                        \
        )

/**
 *  \brief To terminate a call
 *
 *  \par Description:
 *       This function enables to terminate a call.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [in] wwr
 *         Flag to indicate Write or Write without Response.
 *          1 -> Write without response
 *          0 -> Write
 *
 *  \param [in] data
 *         Pointer to the call index to be terminated.
 *
 *  \param [in] datalen
 *         Parameter Length, one octet.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_TERMINATE_CALL_CNF event
 *          will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_terminate_call(handle, wwr, data, datalen)                \
        ccp_ce_write_request                                                \
        (                                                                   \
            (handle),                                                       \
            (CCP_CP_OP_TERMINATE),                                          \
            (wwr),                                                          \
            (data),                                                         \
            (datalen),                                                      \
            (CCP_CE_TERMINATE_CALL_CNF)                                     \
        )

/**
 *  \brief To set a call to locally held state.
 *
 *  \par Description:
 *       This function enables to set a call to locally held state.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [in] wwr
 *         Flag to indicate Write or Write without Response.
 *          1 -> Write without response
 *          0 -> Write
 *
 *  \param [in] data
 *         Pointer to the call index to Local Hold.
 *
 *  \param [in] datalen
 *         Parameter Length, one octet.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_LOCAL_HOLD_CALL_CNF event
 *          will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_call_local_hold(handle, wwr, data, datalen)               \
        ccp_ce_write_request                                                \
        (                                                                   \
            (handle),                                                       \
            (CCP_CP_OP_L_HLD),                                              \
            (wwr),                                                          \
            (data),                                                         \
            (datalen),                                                      \
            (CCP_CE_LOCAL_HOLD_CALL_CNF)                                    \
        )

/**
 *  \brief To retrieve a locally held call.
 *
 *  \par Description:
 *       This function enables to retrieve a locally held call.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [in] wwr
 *         Flag to indicate Write or Write without Response.
 *          1 -> Write without response
 *          0 -> Write
 *
 *  \param [in] data
 *         Pointer to the call index to Local Hold.
 *
 *  \param [in] datalen
 *         Parameter Length, one octet.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_LOCAL_RETRIEVE_CALL_CNF event
 *          will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_call_local_retrieve(handle, wwr, data, datalen)           \
        ccp_ce_write_request                                                \
        (                                                                   \
            (handle),                                                       \
            (CCP_CP_OP_L_RTRV),                                             \
            (wwr),                                                          \
            (data),                                                         \
            (datalen),                                                      \
            (CCP_CE_LOCAL_RETRIEVE_CALL_CNF)                                \
        )

/**
 *  \brief To originate a Outgoing call.
 *
 *  \par Description:
 *       This function enables to initiate a outgoing call.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [in] wwr
 *         Flag to indicate Write or Write without Response.
 *          1 -> Write without response
 *          0 -> Write
 *
 *  \param [in] data
 *         Pointer to the buffer, that contains URI of the outgoing call.
 *
 *  \param [in] datalen
 *         Parameter Length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_ORGINATE_CALL_CNF event
 *          will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_originate_call(handle, wwr, data, datalen)                \
        ccp_ce_write_request                                                \
        (                                                                   \
            (handle),                                                       \
            (CCP_CP_OP_ORIGINATE),                                          \
            (wwr),                                                          \
            (data),                                                         \
            (datalen),                                                      \
            (CCP_CE_ORGINATE_CALL_CNF)                                      \
        )
/**
 *  \brief To Join the calls
 *
 *  \par Description:
 *       This function enables to merge/join the calls.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [in] wwr
 *         Flag to indicate Write or Write without Response.
 *          1 -> Write without response
 *          0 -> Write
 *
 *  \param [in] data
 *         Pointer to the buffer containing, the list of call index to be
 *         joined.
 *
 *  \param [in] datalen
 *         Parameter Length, one octet.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *          On \ref GA_SUCCESS, \ref CCP_CE_JOIN_CALL_CNF will be notified.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_ce_join_call(handle, wwr, data, datalen)                     \
        ccp_ce_write_request                                                \
        (                                                                   \
            (handle),                                                       \
            (CCP_CP_OP_JOIN),                                               \
            (wwr),                                                          \
            (data),                                                         \
            (datalen),                                                      \
            (CCP_CE_JOIN_CALL_CNF)                                          \
        )

/**
 * \name CCP Client APIs - Read
 * \{
 * \brief This section describes the Call Control Profile Read APIs
 * for Client.
 */

/**
 *  \brief Conmmon interface to read a GTBS/TBS characteristic.
 *
 *  \par Description:
 *       This function enables to get characteristic value of GTBS/TBS.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [in] cevent
 *         Expected Event on completion this the procedure.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *
 *  \sa ga_ccp_ce_events
 *  \sa ga_ccp_error_code
 */
GA_RESULT ccp_ce_read_request
          (
              /* IN  */ CCP_HANDLE  handle,
              /* IN */  UINT8       cevent
          );

/** \} */

/**
 * \name CCP Client APIs - Write
 * \{
 * \brief This section describes the Call Control Profile Write APIs
 * for Client.
 */

/**
 *  \brief Common Interface for CP and Char Write.
 *
 *  \par Description:
 *       This function enables to get characteristic value of GTBS/TBS.
 *
 *  \param [in] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [in] tbs_cp_op
 *         Control Point Opcode.
 *         - 0xFF, indicate the operation is characteristic write.
 *
 *  \param [in] wwr
 *         Flag to indicate Write or Write Without Response.
 *          1 -> Write without response
 *          0 -> Write
 *
 *  \param [in] tbs_cp_req_data
 *         Pointer to data containing the Control Point write or
 *         characterstic write.
 *
 *  \param [in] tbs_cp_req_datalen
 *         Control Point write or char. write data length.
 *
 *  \param [in] cevent
 *         Expected Event on completion this the procedure.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *
 *  \sa ga_ccp_ce_events
 *  \sa ga_ccp_error_code
 */
GA_RESULT ccp_ce_write_request
          (
              /* IN */ UINT8  handle,
              /* IN */ UINT8  tbs_cp_op,
              /* IN */ UINT8  wwr,
              /* IN */ void  *tbs_cp_req_data,
              /* IN */ UINT16 tbs_cp_req_datalen,
              /* IN */ UINT8  cevent
          );

/**
 * \cond EXCLUDE_PART
 */

#ifdef CCP_SUPPORT_CONTEXT_MANAGE
/**
 *  \brief Interface to Get/Set the CCP Context Information.
 *
 *  \par Description:
 *       This function enables to get/set the context information of given GTBS/TBS.
 *
 *  \param [in] set
 *         Flag to indicate get/set the context information.
 *
 *  \param [in] device
 *         Remote Device Address.
 *
 *  \param [inout] handle
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [inout] range
 *         GTBS/TBS characteristics handle range.
 *
 *  \param [inout] info
 *          GTBS/TBS characteristics information.
 *
 *  \param [inout] info_count
 *          GTBS/TBS characteristics count.
 *
 *  \return GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT GA_ccp_ce_manage_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT      * device,
              /* INOUT  */ CCP_HANDLE       * handle,
              /* INOUT  */ GA_BRR_SVC_RANGE ** range,
              /* INOUT  */ GA_CHAR_INFO     ** info,
              /* INOUT  */ UINT16           * info_count
          );

/**
 *  \brief Interface to Get the CCP Context Information.
 *
 *  \par Description:
 *       This function enables to get the context information of given GTBS/TBS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [inout] r
 *         GTBS/TBS characteristics handle range.
 *
 *  \param [inout] i
 *          GTBS/TBS characteristics information.
 *
 *  \param [inout] c
 *          GTBS/TBS characteristics count.
 *
 *  \return GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 *
 *  \note
 *       Before calling this interface, the application is expected to setup the GTBS/TBS context
 *       by calling \ref GA_ccp_ce_setup_context() or \ref GA_ccp_ce_setup_tbs_context().
 */
#define GA_ccp_ce_get_context_info(d, h, r, i, c) \
        GA_ccp_ce_manage_context_info(GA_FALSE, (d), (h), (r), (i), (c))

/**
 *  \brief Interface to Set the CCP Context Information.
 *
 *  \par Description:
 *       This function enables to set the context information of given GTBS/TBS.
 *
 *  \param [in] d
 *         Remote Device Address.
 *
 *  \param [inout] h
 *         CCP Handle which is allocated during GTBS/TBS context setup.
 *
 *  \param [inout] r
 *         GTBS/TBS characteristics handle range.
 *
 *  \param [inout] i
 *          GTBS/TBS characteristics information.
 *
 *  \param [inout] c
 *          GTBS/TBS characteristics count.
 *
 *  \return GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 *
 *  \note
 *       Before calling this interface, the application is expected to save the GTBS/TBS context
 *       information by calling \ref GA_ccp_ce_get_context_info().
 */
#define GA_ccp_ce_set_context_info(d, h, r, i, c) \
        GA_ccp_ce_manage_context_info(GA_TRUE, (d), (h), (r), (i), (c))
#endif /* CCP_SUPPORT_CONTEXT_MANAGE */

/**
 * \endcond
 */

/** \} */

/**
 * \name CCP Client APIs - Release
 * \{
 * \brief This section describes the Call Control Profile Release
 * APIs for Client.
 */

/**
 *  \brief Release or free the given GTBS context.
 *
 *  \par Description
 *  When 'free' is set to \ref GA_FALSE, this routine initiates the release
 *  procedure for the context. Once release is done, the context is freed up
 *  and the setup must be freshly done by calling
 *  \ref GA_ccp_ce_setup_context if required for
 *  the same device again.
 *
 *  If the 'free' parameter is set to \ref GA_TRUE,
 *  this API just frees up the context without the release procedure.
 *  If the given handle is GTBS Handle, any related TBS contexts shall be
 *  freed by the application before freeing up the GTBS context.
 *  If the given handle is a TBS Handle, the respective TBS context is freed.
 *
 *  \param [in] context Context to be released/freed
 *  \param [in] free Indicate free only without release
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref  is notified on
 *          completion with status as success or failure.
 */
GA_RESULT GA_ccp_ce_terminate
          (
              /* IN */ CCP_HANDLE handle,
              /* IN */ UCHAR      free
          );

/**
 *  \brief Release the given GA context.
 *
 *  \par Description
 *  This routine initiates the release procedure for the context. Once release
 *  is done, the context is freed up and the setup must be freshly done by
 *  calling \ref GA_ccp_ce_setup_context if required for the same device again.
 *
 *  \param [in] context Context to be released
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref CCP_CE_RELEASE_CNF is notified on
 *          completion with status as success or failure.
 */
#define GA_ccp_ce_release(ctx) \
        GA_ccp_ce_terminate((ctx), GA_FALSE)

/**
 *  \brief Free the given GTBS context.
 *
 *  \par Description
 *  This routine frees up the given context of the GA layer.
 *  If the given handle is GTBS Handle, any related TBS contexts shall be
 *  freed by the application before freeing up the GTBS context.
 *  If the given handle is a TBS Handle, the respective TBS context is freed.
 *
 *  \param [in] context Context to be freed
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_ccp_ce_close(ctx) \
        GA_ccp_ce_terminate((ctx), GA_TRUE)

/**
 *  \brief Release or free the given TBS context.
 *
 *  \par Description
 *  When 'free' is set to \ref GA_FALSE, this routine initiates the release
 *  procedure for the context. Once release is done, the context is freed up
 *  and the setup must be freshly done by calling
 *  \ref GA_ccp_ce_setup_tbs_context if required for
 *  the same device again.
 *
 *  If the 'free' parameter is set to \ref GA_TRUE,
 *  this API just frees up the context without the release procedure.
 *
 *  \param [in] context Context to be released/freed
 *  \param [in] free Indicate free only without release
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref  is notified on
 *          completion with status as success or failure.
 */
GA_RESULT GA_ccp_ce_terminate_tbs
          (
              /* IN */ CCP_HANDLE handle,
              /* IN */ UCHAR      free
          );

/**
 *  \brief Release the given GA context.
 *
 *  \par Description
 *  This routine initiates the release procedure for the context. Once release
 *  is done, the context is freed up and the setup must be freshly done by
 *  calling \ref GA_ccp_ce_setup_tbs_context if required for the same device
 *  again.
 *
 *  \param [in] context Context to be released
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref CCP_CE_RELEASE_TBS_CNF is notified on
 *          completion with status as success or failure.
 */
#define GA_ccp_ce_release_tbs(ctx) \
        GA_ccp_ce_terminate_tbs((ctx), GA_FALSE)

/**
 *  \brief Free the given GA context.
 *
 *  \par Description
 *  This routine frees up the given context of the TBS.
 *
 *  \param [in] context Context to be freed
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_ccp_ce_close_tbs(ctx) \
        GA_ccp_ce_terminate_tbs((ctx), GA_TRUE)

/** \} */

/**
 * \name CCP Client APIs - De-Init
 * \{
 * \brief This section describes the Call Control Profile DeInit
 * APIs for Client.
 */

/**
 *  \brief To shutdown Call Control Client Module.
 *
 *  \par Description:
 *       This function enables to shutdown the call control client.
 *       This function de-references the callback registered with CCP
 *       Module.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
GA_RESULT GA_ccp_ce_shutdown(void);

/** \} */
/** \} */
/** \} */
/** \} */

#endif /* CCP_CLIENT */

#ifdef __cplusplus
};
#endif

/** \} */
/** \} */
/** \} */
/** \} */
#endif /* _H_GA_CCP_CE_API_ */
