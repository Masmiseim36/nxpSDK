
/**
 *  \file BT_hfp_ag_api.h
 *
 *  This file contains the data structures, the type definitions
 *  and API functions that HFP Audio Gateway exports to the application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_HFP_AG_API_
#define _H_BT_HFP_AG_API_

/* ----------------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ----------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup hfp_ag_module HFP AG (Hands Free Profile - Audio Gateway)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Hands Free Profile - Audio Gateway module to the Application and other upper
 *  layers of the stack.
 */
/**
 * \defgroup hfp_ag_events Events
 *
 * @name HFP Audio Gateway Notification Callback Event Types
 *
 * Constant Definitions for HFP Audio Gateway Notification Callback Event Types
 */
/*@{*/
#define HFP_AG_CONNECT_CNF                      0x01U
#define HFP_AG_CONNECT_IND                      0x02U
#define HFP_AG_DISCONNECT_CNF                   0x03U
#define HFP_AG_DISCONNECT_IND                   0x04U
#define HFP_AG_STOP_CNF                         0x05U
#define HFP_AG_SEND_DATA_CNF                    0x06U
#define HFP_AG_RECVD_DATA_IND                   0x07U
/*@}*/
/* ----------------------------------------------- Structures/Data Types */
/**
 *  \defgroup hfp_ag_defines Defines
 *  \{
 *
 *  Describes Defines defined by the module.
 */
/**
 *  \defgroup hfp_ag_structures Structures
 *  \{
 *
 *  Describes Structures defined by the module.
 */
/**
 *
 * @name HFP Audio Gateway Notification Callback Events
 *
 * Constant Definitions for HFP Audio Gateway Notification Callback Events
 */
/*@{*/
typedef UCHAR HFP_AG_EVENTS;
/*@}*/
/** \} */
/** \} */
/**
 *  \defgroup hfp_ag_appl_callback Application Callback
 *  \{
 *
 *  Describes Application Callback defined by the module.
 */
/** HFP Audio Gateway Event Notification Callback */
typedef API_RESULT (* HFP_AG_EVENT_NOTIFY_CB)
                   (
                       /* IN */ HFP_AG_EVENTS    hfp_ag_event,
                       /* IN */ API_RESULT       result,
                       /* IN */ void *           data,
                       /* IN */ UINT16           data_length
                   );

/** \} */
/* ----------------------------------------------- Macros */


/* ----------------------------------------------- Function Declarations */
/**
 * \defgroup hfp_ag_API API Definitions
 * \{
 * This section describes the EtherMind Hands Free Profile Audio Gateway APIs.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \brief  To initialize HFP Audio Gateway
 *
 *  \par Description:
 *       This API registers the application callback function pointer with the
 *       HFP AG and initializes the HFP AG profile.
 *
 *  \param [in] appl_cb
 *         Application callback function pointer.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hfp_ag_init
           (
               /* IN */  HFP_AG_EVENT_NOTIFY_CB    appl_cb
           );

/**
 *  \brief To start HFP Audio Gateway
 *
 *  \par Description:
 *       This API will switch on HFP AG function on the local device (i.e.)
 *       enable AG to listen to peer (HFPU) initiated connections on a RFCOMM
 *       channel and enables application to use the services of HFP AG profile.
 *
 *  \param [in] server_channel
 *         The RFCOMM server channel in which the HFP AG will be listening for
 *         peer initiated connection.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hfp_ag_start
           (
               /* IN */  UCHAR    server_channel
           );

/**
 *  \brief To stop HFP Audio Gateway
 *
 *  \par Description:
 *       This API enables user to switch off HFP AG function on the local device.
 *       Existing active Hands Free connection if any will be released.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hfp_ag_stop ( void );

/**
 *  \brief  To establish connection with remote HFP Unit.
 *
 *  \par Description:
 *       This API establishes service level connection between AG and a HFP Unit
 *
 *  \param [in] server_channel
 *         The server channel in which connection has to be initiated.
 *
 *  \param [in] bd_addr
 *         BD_ADDR of HFP Unit with whom connection need to be established.
 *
 *  \return
 *          API_SUCCESS: Indicates connection procedure initiated successfully
 *          HFP_AG_ERR_INVALID_STATE: Profile is not started or already connected.
 *          HFP_AG_ERR_MUTEX_LOCK_FAILED: Failed to Lock HFP AG Mutex.
 *          HFP_AG_ERR_MUTEX_UNLOCK_FAILED: Failed to Unlock HFP AG Mutex.
 *          Relevant error code from RFCOMM if any RFCOMM operation failed.
 *  \note
 *       Result of the connection setup procedure completion will be indicated
 *       through application callback.
 *
 */
API_RESULT BT_hfp_ag_connect
           (
               /* IN */  UCHAR      server_channel,
               /* IN */  UCHAR *    bd_addr
           );

/**
 *  \brief To disconnect from remote HFP Unit.
 *
 *  \par Description:
 *       This API disconnects existing service level connection with the HFP Unit.
 *
 *  \param [in] bd_addr
 *         BD_ADDR of HFP Unit with whom connection need to be released
 *
 *  \return
 *      API_SUCCESS: Indicates disconnection procedure initiated successfully.
 *      HFP_AG_ERR_NULL_PARAMETER: Indicates BD_ADDR is NULL.
 *      HFP_AG_ERR_INVALID_STATE: Profile is not started or not connected.
 *      HFP_AG_ERR_MUTEX_LOCK_FAILED: Failed to Lock HFP AG Mutex.
 *      HFP_AG_ERR_MUTEX_UNLOCK_FAILED: Failed to Unlock HFP AG Mutex.
 *      HFP_AG_ERR_INVALID_PARAMETER: Failed to disconnect, wrong BD_ADDR is provided.
 *      Relevant error code from RFCOMM if any RFCOMM operation failed.
 *
 *  \note Result of the disconnection procedure completion will be indicated
 *        through application callback
 */
API_RESULT BT_hfp_ag_disconnect
           (
               /* IN */  UCHAR *    bd_addr
           );

/**
 *  \brief To send Data/Response/Result Code to HFP Unit.
 *
 *  \par Description:
 *       This API allows the application to send result code strings to
 *       currently connected HFP Unit.
 *
 *  \param [in] result_code
 *         Contains information of the HDP Event Notification Callback which HDP
 *         should use to report HDP events, the control channel PSM on which HDP
 *         must receive control channel connection requests, the L2CAP configuration
 *         parameters to be used during control channel connection, and the data
 *         channel PSM on which data channel connection shall by HDP be accepted.
 *
 *  \param [in] result_code_length
 *         Contains information of the HDP Event Notification Callback which HDP
 *         should use to report HDP events, the control channel PSM on which HDP
 *         must receive control channel connection requests, the L2CAP
 *         configuration parameters to be used during control channel connection,
 *         and the data channel PSM on which data channel connection shall by HDP
 *         be accepted.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note The mep_id is the output parameter; the application shall store this
 *        mep_id for initiating HDP control channel establishment procedures.
 */
API_RESULT BT_hfp_ag_send_data
           (
               /* IN */ UCHAR *    result_code,
               /* IN */ UINT16     result_code_length
           );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_HFP_AG_API_ */

