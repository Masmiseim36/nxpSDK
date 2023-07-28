
/**
 *  \file GA_mcp_ce_api.h
 *
 *  \brief This file defines the GA Media Control Profile(MCP)
 *  Client Entity(CE) Interface - includes Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_MCP_CE_API_
#define _H_GA_MCP_CE_API_

/**
 * \addtogroup bt_ga
 * \{
 */
/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"

/**
 * \addtogroup bt_ga_profiles
 * \{
 */
/**
 * \addtogroup bt_ga_content_control Content Control
 * \{
 */
/**
 * \addtogroup bt_ga_mcp Media Control
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Generic Audio (GA) Profile Media Control module to the Application.
 */
/* --------------------------------------------- Global Definitions */
/**
 * \defgroup ga_mcp_defines Defines
 * \{
 * \brief Describes defines for the module.
 */

/**
 * \defgroup ga_mcp_ce_module_def MCP CE (Media Control Profile) Client Entity
 * \{
 * \brief This section describes the defines for MCP CE.
 */

/**
 * \defgroup ga_mcp_ce_events Events
 * \{
 * \brief This section lists the Synchronous/Asynchronous Events notified to
 * Application by the Module through the callback \ref MCP_CE_NTF_CB callback.
 */

/**
 * \name MCP Client Events - Setup
 * \{
 * \brief This section describes the MCP Setup Events for Client.
 */

/**
 * This event indicates the setup of a GMCS/MCS Session.
 * \if MCP_SUPPORT_OBJECT_TRANSFER
 *   If \ref MCP_SUPPORT_OBJECT_TRANSFER is enabled, then the OTP Session is
 *   also established only for a GMCS Session.
 * \endif
 * A handle to the MCS/GMCS Session is derived and provided to the
 * application, this Handle to be further used during all MCP Client related
 * requests.
 * 1. GMCS Session Establishment
 *    \if MCP_SUPPORT_OBJECT_TRANSFER
 *      with \ref MCP_SUPPORT_OBJECT_TRANSFER enabled.
 *    \endif \n
 *    1.1. Service Discovery. \n
 *         If Service if not found, \ref MCP_CE_SETUP_CNF with \ref GA_FAILURE. \n
 *    1.2. Service Composition: GMCS Char and Desc Discovery. \n
 *         If Service Composition is triggered successfully, \n
 *            a) \ref MCP_CE_SETUP_CNF with \ref GA_CONTINUE. \n
 *            b) Else, \ref MCP_CE_SETUP_CNF with \ref GA_FAILURE. \n
 *    1.3. Discovery of Included Service - OTS is triggered for the
 *         GMCS Service Handle range.
 *            a) If Failure, \ref MCP_CE_SETUP_CNF with \ref GA_FAILURE. \n
 *            b) If Success and MCS, \ref MCP_CE_SETUP_CNF with \ref GA_SUCCESS. \n
 *    1.4. Discovery of Secondary Service - OTS is triggered. \n
 *            a) If Failure, \ref MCP_CE_SETUP_CNF with \ref GA_FAILURE. \n
 *            b) If Session is already established, \ref MCP_CE_SETUP_CNF with
 *            \ref GA_SUCCESS.
 *    1.5. If GMCS, Service Composition, Setup of Transport Control Channel and Data Channel. \n
 *            a) If Failure, \ref MCP_CE_SETUP_CNF with \ref GA_FAILURE. \n
 *            b) If Session is already established, \ref MCP_CE_SETUP_CNF with
 *            \ref GA_SUCCESS.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_SETUP_CNF
 * \param [in] evt_status  \ref GA_CONTINUE Setup Ongoing
 *                     \ref GA_SUCCESS Setup Complete \n
 *                     \ref GA_FAILURE Others \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If status is \ref GA_CONTINUE, \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_SETUP_CNF                      0x01U

/**
 * This Event is notified on discovery of optional MCS(s) with peer device
 * with the following values as parameters in the \ref MCP_CE_NTF_CB
 * callback.
 * If there are multiple instances 'n' of MCS found on the remote, this event
 * is generated 'n' times with status as \ref GA_CONTINUE.
 * This event with status as \ref GA_SUCCESS, indicates that the Service
 * discovery is complete.
 * In case of failure, \ref GA_FAILURE is triggered.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_DISC_MCS_CNF
 * \param [in] evt_status  \ref GA_CONTINUE Service Handle range of discovered MCS \n
 *                     \ref GA_SUCCESS Service Discovery Complete \n
 *                     \ref GA_FAILURE Others \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If status is \ref GA_CONTINUE \n
 *                           - data - Pointer to object of type \ref GA_BRR_SVC_INFO \n
 *                           - len - Size of \ref GA_BRR_SVC_INFO \n
 *                           .
 *                         If status is \ref GA_SUCCESS or \ref GA_FAILURE, \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 *
 */
#define MCP_CE_DISC_MCS_CNF                   0x02U

/** \} */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/**
 * \name MCP Client Events - OTP Setup
 * \{
 * \brief This section describes the MCP OTP Setup Events for Client.
 */

/**
 * On completion of discovery of service, char and desc of optional OTS,
 * MCP Handle is provided to the application with the event along with the status.
 * In case where service is not found, \ref GA_FAILURE is sent as the event status.
 */
/**
 * This event is notified on completion of establishment of Control channel
 * on the peer device with the following values as parameters in
 * the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_CONTROL_CHANNEL_SETUP_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Control Channel connected \n
 *                         \ref GA_FAILURE Control Channel connect failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE, \n
 *                           data - NULL \n
 *                           len - 0
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_CONTROL_CHANNEL_SETUP_CNF      0x03U

/**
 * This event is notified on completion of data channel connection
 * on the peer device with the following values as parameters in
 * the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_DATA_CHANNEL_UP_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Data Channel connected \n
 *                         \ref GA_FAILURE Data Channel connect failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE, \n
 *                           data - NULL \n
 *                           len - 0
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_DATA_CHANNEL_UP_CNF            0x04U

/**
 * This event is notified on completion of data channel disconnection
 * on the peer device with the following values as parameters in
 * the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_DATA_CHANNEL_DOWN_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Data Channel disconnected \n
 *                         \ref GA_FAILURE Data Channel disconnect failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE, \n
 *                           data - NULL \n
 *                           len - 0
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_DATA_CHANNEL_DOWN_CNF          0x05U

/** \} */

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/**
 * \name MCP Client Events - Setup
 * \{
 * \brief This section describes the MCP Setup Events for Client.
 */

/**
 * This event is notified on completion of configuration enable
 * of all char on the peer device with the following values as parameters in
 * the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_ENABLE_ALL_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Configuration Complete \n
 *                         \ref GA_FAILURE Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE, \n
 *                           data - NULL \n
 *                           len - 0
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_ENABLE_ALL_CFG_CNF             0x06U

/**
 * This event is notified on completion of configuration disable
 * of all char on the peer device with the following values as parameters in
 * the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_DISABLE_ALL_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Configuration Complete \n
 *                         \ref GA_FAILURE Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE, \n
 *                           data - NULL \n
 *                           len - 0
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_DISABLE_ALL_CFG_CNF            0x07U

/** \} */

/**
 * \name MCP Client Events - Configuration
 * \{
 * \brief This event is notified only when a request to configure notification
 * for a specific char is triggered via
 * \ref GA_mcp_ce_configure_context_char().
 */

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_MEDIA_PLAYER_NAME_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                       MCP Handle for the Device - \ref MCP_HANDLE \n
 *                       If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_MEDIA_PLAYER_NAME_CFG_CNF      0x08U

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_TRACK_CHANGED_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_TRACK_CHANGED_CFG_CNF          0x09U

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_TRACK_TITLE_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_TRACK_TITLE_CFG_CNF            0x0AU
/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_TRACK_DUR_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_TRACK_DUR_CFG_CNF              0x0BU

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_TRACK_POS_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_TRACK_POS_CFG_CNF              0x0CU

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_PLAYBACK_SPEED_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_PLAYBACK_SPEED_CFG_CNF         0x0DU

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_SEEKING_SPEED_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_SEEKING_SPEED_CFG_CNF          0x0EU

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_CURR_TRACK_OBJ_ID_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_CURR_TRACK_OBJ_ID_CFG_CNF      0x0FU
/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_NEXT_TRACK_OBJ_ID_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_NEXT_TRACK_OBJ_ID_CFG_CNF      0x10U

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_PARENT_GROUP_OBJ_ID_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_PARENT_GROUP_OBJ_ID_CFG_CNF    0x11U

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_CURR_GROUP_OBJ_ID_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_CURR_GROUP_OBJ_ID_CFG_CNF      0x12U

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_PLAYING_ORDER_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_PLAYING_ORDER_CFG_CNF          0x13U

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_MEDIA_STATE_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_MEDIA_STATE_CFG_CNF            0x14U

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_MCP_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_MCP_CFG_CNF                    0x15U

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_MCP_OPC_SUPP_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_MCP_OPC_SUPP_CFG_CNF           0x16U

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_SCP_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_SCP_CFG_CNF                    0x17U

/**
 * This event is notified on completion of configuration i.e. enable/disable
 * of a char on the peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_SEARCH_RES_OBJ_ID_CFG_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Configuration Complete \n
 *                         \ref GA_FAILURE : Configuration Failed \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS or \ref GA_FAILURE : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_SEARCH_RES_OBJ_ID_CFG_CNF      0x18U

/** \} */

/**
 * \name MCP Client Events - Notifications
 * \{
 * \brief This event is notified when the char is configured for any
 * notification change and a notification has occurred from the peer device.
 */

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This is an alert indicating there is a change in the Media Player Name.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_MEDIA_PLAYER_NAME_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UCHAR String \n
 *                           - len - Length of the data \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_MEDIA_PLAYER_NAME_NTF          0x19

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This is an alert indicating there is a change in the Current Track.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_TRACK_CHANGED_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_TRACK_CHANGED_NTF              0x1AU

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This is an alert indicating there is a change in the Current Track Title.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_TRACK_TITLE_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - Valid \n
 *                               - data - \ref UCHAR String \n
 *                               - len - Length of the data \n
 *                               .
 *                           - No current track or Track title unavailable \n
 *                               - data - NULL \n
 *                               - len - 0 \n
 *                               .
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_TRACK_TITLE_NTF                0x1BU

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This is an alert indicating there is a change in the Current Track.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_TRACK_DUR_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - Valid \n
 *                               - data - \ref INT32 Length of the current track in
 *                               0.01-second resolution as a 32-bit signed integer \n
 *                               - len - sizeof(INT32) \n
 *                               .
 *                           - If the media player has no current
 *                           track or the duration of the current track is
 *                           unknown \n
 *                               - data - 0xFFFFFFFF \n
 *                               - len - sizeof(INT32) \n
 *                               .
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_TRACK_DUR_NTF                  0x1CU
/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * The Track Position characteristic exposes the current track position of the
 * current track in 0.01-second resolution as a 32-bit signed integer.
 * The Track Position characteristic exposes the offset from the start of the
 * track to the current playing position. If the start of the track is not
 * well defined (such as in a live stream), Track Position equals 0 or sets the
 * value to UNAVAILABLE (0xFFFFFFFF).
 * This is an alert indicating there is a change in the Track Position.
 * However, this event will not be notified when the Media state is "Playing"
 * and when playback happens at a constant speed.
 * The Track Position is notified when the Media State is set to Seeking.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_TRACK_POS_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - Valid \n
 *                               - data - \ref INT32 Length of the current track in
 *                               0.01-second resolution as a 32-bit signed integer \n
 *                               - len - sizeof(INT32) \n
 *                               .
 *                           - If the media player has no current
 *                           track or the duration of the current track is
 *                           unknown \n
 *                               - data - 0xFFFFFFFF \n
 *                               - len - sizeof(INT32) \n
 *                               .
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_TRACK_POS_NTF                  0x1DU

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * The Playback Speed characteristic exposes the current track playback speed.
 * The value of this characteristic shall be a signed 8-bit integer (p) that
 * is used to compute the multiple of the playback speed
 * (s) by calculating the value of 2 to the power of p divided by 64: \n
 * s = 2 ^ (p/64) \n
 * p - Characteristic Value s - Resultant Multiple of Playback Speed \n
 *     -128                                .25 \n
 *     -64                                 .50 \n
 *      0                                   1 \n
 *      64                                  2 \n
 *      127                                 3.957 (nearly 4) \n
 * This is an alert indicating there is a change in the Playback speed.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_PLAYBACK_SPEED_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref INT8 signed 8-bit integer (p) \n
 *                           - len - sizeof(INT8) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_PLAYBACK_SPEED_NTF             0x1EU

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * The Seeking Speed characteristic exposes the seeking speed of the current
 * track.
 * The value of this characteristic shall be a signed 8-bit integer (p) which
 * is the current multiple of the real-time playback speed at which the track
 * is being fast forwarded or fast rewound.
 *
 * This is an alert indicating there is a change in the Seeking speed.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_SEEKING_SPEED_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - Valid \n
 *                               - data - \ref INT8 signed 8-bit integer (p) \n
 *                               - len - sizeof(INT8) \n
 *                               .
 *                           - If the media player is not seeking \n
 *                               - data - 0 \n
 *                               .
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_SEEKING_SPEED_NTF              0x1FU

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * The Current Track Object ID characteristic is used to access a Track Object
 * type. This characteristic identifies the current track as an object in the
 * included OTS. The value of this characteristic shall be a uint48 value that
 * is in the range of a valid Object ID as defined in OTS.
 * If present, this value can be used in the Object ID characteristic in the
 * included OTS to allow access to the contents of the track.
 * This is an alert indicating there is a change in the Current track object
 * ID, this wont be generated in the case where the client changes the value.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_CURR_TRACK_OBJ_ID_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref GA_MCP_CE_OBJ_ID Object ID \n
 *                           - len - \ref OBJ_ID_LEN - Object ID Length\n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_CURR_TRACK_OBJ_ID_NTF          0x20U

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * The Next Track Object ID characteristic is used to access a Track Object
 * type.
 * This is an alert indicating there is a change in the Next track object
 * ID, this wont be generated in the case where the client changes the value.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_NEXT_TRACK_OBJ_ID_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - Valid \n
 *                               - data - \ref GA_MCP_CE_OBJ_ID - Object ID \n
 *                               - len - \ref OBJ_ID_LEN - Object ID Length \n
 *                               .
 *                           - If the media player has no next track \n
 *                               - data - NULL \n
 *                               - len - 0 \n
 *                               .
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_NEXT_TRACK_OBJ_ID_NTF          0x21U

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This characteristic identifies the parent group of the current group object
 * as a group object in the included OTS.
 * For a group that has no parent, the value of the Parent Group Object ID
 * shall be the value of the Current Group Object ID.
 * This is an alert indicating there is a change in the Group object
 * ID.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_PARENT_GROUP_OBJ_ID_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - Valid \n
 *                               - data - \ref GA_MCP_CE_OBJ_ID - Object ID \n
 *                               - len - \ref OBJ_ID_LEN - Object ID Length \n
 *                               .
 *                           - If the media player has not created a
 *                             parent group or a current group \n
 *                               - data - NULL \n
 *                               - len - 0 \n
 *                               .
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_PARENT_GROUP_OBJ_ID_NTF        0x22U

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This characteristic identifies the current group of the current group object
 * as a group object in the included OTS.
 * This is an alert indicating there is a change in the Current Group object
 * ID.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_CURR_GROUP_OBJ_ID_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - Valid \n
 *                               - data - \ref GA_MCP_CE_OBJ_ID - Object ID \n
 *                               - len - \ref OBJ_ID_LEN - Object ID Length \n
 *                               .
 *                           - If the media player has not created a
 *                             current group \n
 *                               - data - NULL \n
 *                               - len - 0 \n
 *                               .
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_CURR_GROUP_OBJ_ID_NTF          0x23U

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This characteristic identifies the playing order of the media player.
 * This is an alert indicating there is a change in the playing order.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_PLAYING_ORDER_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UINT8 - Playing Order \n
 *                           - len - sizeof(UINT8) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_PLAYING_ORDER_NTF              0x24U

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This characteristic identifies the media state of the media player.
 * This is an alert indicating there is a change in the media state.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_MEDIA_STATE_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UINT8 - Media State \n
 *                           - len - sizeof(UINT8) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_MEDIA_STATE_NTF                0x25U

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This is an alert indicating a response to the Media Control Point opcode
 * write \ref GA_mcp_ce_write_MCP.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_MCP_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - Pointer to a valid \ref UINT16 \n
 *                                  - \ref UINT8 - Req Opcode \ref MCP_CP_OPCODES \n
 *                                  - \ref UINT8 - Result Code \n
 *                                         - 0x01 - Success \n
 *                                         - 0x02 - Opcode not supported \n
 *                                         - 0x03 - Media Player Inactive \n
 *                                         - 0x04 - Command cannot be completed \n
 *                                         - Other Values - RFU \n
 *                                         .
 *                                  .
 *                           - len - sizeof(UINT16) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_MCP_NTF                        0x26U

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This characteristic identifies the media state of the media player.
 * This is an alert indicating there is a change in the set of supported o
 * opcodes.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_MCP_OPC_SUPP_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UINT32 - Bit values rep Opcode \n
 *                           - len - sizeof(UINT32) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_MCP_OPC_SUPP_NTF               0x27U

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This is an alert indicating a response to the Search Control Point opcode
 * write \ref GA_mcp_ce_write_SCP.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_SCP_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UINT8 \n
 *                                  - 0x01 - Search request was accepted;
 *                                         search has started \n
 *                                  - 0x02 - Search request was invalid;
 *                                         no search started \n
 *                                  .
 *                           - len - sizeof(UINT8) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_SCP_NTF                        0x28U

/**
 * This event is notified when a notification is received for a char from peer
 * device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 * This characteristic identifies the search results object as a group object in
 * the included OTS
 * This is an alert indicating the search results object is fully populated.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_SEARCH_RES_OBJ_ID_NTF
 * \param [in] evt_status  \ref GA_SUCCESS : Notification received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - Valid \n
 *                               - data - \ref GA_MCP_CE_OBJ_ID - Object ID \n
 *                               - len - \ref OBJ_ID_LEN - Object ID Length \n
 *                               .
 *                           - If there are no search results \n
 *                               - data - NULL \n
 *                               - len - 0 \n
 *                               .
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_SEARCH_RES_OBJ_ID_NTF          0x29U

/** \} */

/**
 * \name MCP Client Events - Read
 * \{
 * \brief This event is notified whenever read request is successful and a
 * response is received from the peer.
 */

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_MEDIA_PLAYER_NAME_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UCHAR String \n
 *                           - len - Length of the data \n
 *                           .
 *                         Else : \n
 *                           - data - \ref NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_MEDIA_PLAYER_NAME_CNF     0x2AU

/**
 * This event is notified when the contents of the object is received on the
 * data channel from the peer device with the following values as parameters
 * in the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_MEDIA_PLAYER_ICON_OBJ_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref GA_MCP_CE_MP_ICON_OBJ \n
 *                           - len - sizeof( \ref GA_MCP_CE_MP_ICON_OBJ) \n
 *                           .
 *                         Else : \n
 *                           - data - \ref NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_MEDIA_PLAYER_ICON_OBJ_CNF 0x2BU

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_MEDIA_PLAYER_ICON_URL_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UCHAR String \n
 *                           - len - sizeof(UCHAR) \n
 *                           .
 *                         Else : \n
 *                           - data - \ref NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_MEDIA_PLAYER_ICON_URL_CNF 0x2CU

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_TRACK_TITLE_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UCHAR String \n
 *                           - len - sizeof(UCHAR) \n
 *                           .
 *                         Else : \n
 *                           - data - \ref NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_TRACK_TITLE_CNF           0x2DU

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_TRACK_DUR_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - Valid \n
 *                               - data - \ref INT32 Length of the current track in
 *                               0.01-second resolution as a 32-bit signed integer \n
 *                               - len - sizeof(INT32) \n
 *                               .
 *                           - If the media player has no current
 *                           track or the duration of the current track is
 *                           unknown \n
 *                               - data - 0xFFFFFFFF \n
 *                               - len - sizeof(INT32) \n
 *                               .
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_TRACK_DUR_CNF             0x2EU

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_TRACK_POS_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                          If evt_status is \ref GA_SUCCESS : \n
 *                           - Valid \n
 *                               - data - \ref INT32 Length of the current track in
 *                               0.01-second resolution as a 32-bit signed integer \n
 *                               - len - sizeof(INT32) \n
 *                               .
 *                           - If the media player has no current
 *                           track or the duration of the current track is
 *                           unknown \n
 *                               - data - 0xFFFFFFFF \n
 *                               - len - sizeof(INT32) \n
 *                               .
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_TRACK_POS_CNF             0x2FU

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_PLAYBACK_SPEED_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref INT8 signed 8-bit integer (p) \n
 *                           - len - sizeof(INT8) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_PLAYBACK_SPEED_CNF        0x30U

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_SEEKING_SPEED_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - Valid \n
 *                               - data - \ref INT8 signed 8-bit integer (p) \n
 *                               - len - sizeof(INT8) \n
 *                               .
 *                           - If the media player is not seeking \n
 *                               - data - 0 \n
 *                               .
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_SEEKING_SPEED_CNF         0x31U

/**
 * This event is notified when the contents of the object is received on the
 * data channel from the peer device with the following values as parameters
 * in the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_CURR_TRACK_SEG_OBJ_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref GA_MCP_CE_TRACK_SEG_OBJ \n
 *                           - len - sizeof( \ref GA_MCP_CE_TRACK_SEG_OBJ) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_CURR_TRACK_SEG_OBJ_CNF    0x32U

/**
 * This event is notified when the contents of the object is received on the
 * data channel from the peer device with the following values as parameters
 * in the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_CURR_TRACK_OBJ_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref GA_MCP_CE_TRACK_OBJ \n
 *                           - len - sizeof( \ref GA_MCP_CE_TRACK_OBJ) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_CURR_TRACK_OBJ_CNF        0x33U

/**
 * This event is notified when the contents of the object is received on the
 * data channel from the peer device with the following values as parameters
 * in the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_NEXT_TRACK_OBJ_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref GA_MCP_CE_TRACK_OBJ \n
 *                           - len - sizeof( \ref GA_MCP_CE_TRACK_OBJ) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_NEXT_TRACK_OBJ_CNF        0x34U

/**
 * This event is notified when the contents of the object is received on the
 * data channel from the peer device with the following values as parameters
 * in the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_PARENT_GROUP_OBJ_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref GA_MCP_CE_GRP_OBJ \n
 *                           - len - sizeof( \ref GA_MCP_CE_GRP_OBJ) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_PARENT_GROUP_OBJ_CNF      0x35U

/**
 * This event is notified when the contents of the object is received on the
 * data channel from the peer device with the following values as parameters
 * in the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_CURR_GROUP_OBJ_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Read Response received \n
 *                         \ref GA_FAILURE No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref GA_MCP_CE_GRP_OBJ \n
 *                           - len - sizeof( \ref GA_MCP_CE_GRP_OBJ) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_CURR_GROUP_OBJ_CNF        0x36U

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_PLAYING_ORDER_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UINT8 - Playing Order \n
 *                           - len - sizeof(UINT8) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_PLAYING_ORDER_CNF         0x37U

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_PLAYING_ORDERS_SUPP_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UINT16 - Playing Order Supp \n
 *                           - len - sizeof(UINT16) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_PLAYING_ORDERS_SUPP_CNF   0x38U

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_MEDIA_STATE_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UINT8 - Media State \n
 *                           - len - sizeof(UINT8) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_MEDIA_STATE_CNF           0x39U

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_MCP_OPC_SUPP_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref UINT32 - Bit values rep Opcode \n
 *                           - len - sizeof(UINT32) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_MCP_OPC_SUPP_CNF          0x40U

/**
 * This event is notified when the contents of the object is received on the
 * data channel from the peer device with the following values as parameters
 * in the \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_SEARCH_RES_OBJ_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Read Response received \n
 *                         \ref GA_FAILURE No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS, \n
 *                           data - \ref GA_MCP_CE_GRP_OBJ
 *                           len - sizeof(GA_MCP_CE_GRP_OBJ)
 *
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_SEARCH_RES_OBJ_CNF        0x41U

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_READ_CONTENT_CONTROL_ID_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Read Response received \n
 *                         \ref GA_FAILURE No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS, \n
 *                           data - \ref
 *                           len - sizeof()
 *
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_READ_CONTENT_CONTROL_ID_CNF    0x42U

/** \} */

/**
 * \name MCP Client Events - Write
 * \{
 * \brief The event is notified whenever write is successful and a write
 * response is received from peer.
 * This applies to only requests that was triggered with write type -
 * Write Request \ref MCP_CE_WRITE_TYPE_REQUEST.
 */

/**
 * This event is notified when a write response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_TRACK_POS_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Write Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_TRACK_POS_CNF                  0x43U

/**
 * This event is notified when a write response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_TRACK_PLAYBACK_SPEED_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Write Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_TRACK_PLAYBACK_SPEED_CNF       0x44U

/**
 * This event is notified when a write response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_CURR_TRACK_OBJ_ID_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Write Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_CURR_TRACK_OBJ_ID_CNF          0x45U

/**
 * This event is notified when a write response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_NEXT_TRACK_OBJ_ID_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Write Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_NEXT_TRACK_OBJ_ID_CNF          0x46U

/**
 * This event is notified when a write response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_CURR_GRP_OBJ_ID_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Write Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_CURR_GRP_OBJ_ID_CNF            0x47U

/**
 * This event is notified when a write response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_PLAYING_ORDER_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Write Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_PLAYING_ORDER_CNF              0x48U

/**
 * This event is notified when a write response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_MCP_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Write Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_MCP_CNF                        0x49U

/**
 * This event is notified when a write response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_SCP_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Write Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_SCP_CNF                        0x4AU

/** \} */

/**
 * \name MCP Client Events - OTP Read/Write
 * \{
 * \brief Events related to Object ID read, Object Filter read, Object
 * Attributes read, setting Path of the object on the remote.
 */

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback and when the Object Filter is read and
 * the Object path is set on the remote.
 *
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_SET_OBJ_REF_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_CONTINUE : \n
 *                           - data - \ref GA_MCP_CE_OBJ_ID \n
 *                           - len - \ref OBJ_ID_LEN \n
 *                           .
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_SET_OBJ_REF_CNF                0x4BU

/**
 * This event is notified when a read response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_GET_OBJ_ATTR_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Read Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - \ref GA_MCP_CE_OBJ_ATTR Attribute Type and Value \n
 *                           - len -  sizeof( \ref GA_MCP_CE_OBJ_ATTR) \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_GET_OBJ_ATTR_CNF               0x4CU

/**
 * This event is notified when a write response is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_OBJ_DATA_RX_READY_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Write Response received \n
 *                         \ref GA_FAILURE : No Response received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_CE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                         If evt_status is \ref GA_SUCCESS : \n
 *                           - data - NULL \n
 *                           - len - 0 \n
 *                           .
 * \return \ref GA_SUCCESS (always)
 * \note If status is \ref GA_SUCCESS, data receive will begin and
 * corresponding event is generated. \n
 * Eg: \ref MCP_CE_READ_SEARCH_RES_OBJ_CNF \n
 * If status is \ref GA_FAILURE, the request is rejected and no object data
 * will be received. \n
 */
#define MCP_CE_OBJ_DATA_RX_READY_CNF          0x4DU

/** \} */


/**
 * \name MCP Client Events - Release
 * \{
 * \brief This section describes the Media Control Profile Release
 * Events for MCP CE.
 */

/**
 * \brief This event is notified when a GMCS Service context is released.
 * Callback is triggered with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_RELEASE_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Release completed \n
 *                        \ref GA_FAILURE : Release Failed \n
 * \param [in] evt_data  NULL \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_RELEASE_CNF                              0x4EU

/**
 * \brief This event is notified when a MCS Service context is released.
 * Callback is triggered with the following values as parameters in the
 * \ref MCP_CE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] evt_id  \ref MCP_CE_RELEASE_MCS_CNF
 * \param [in] evt_status  \ref GA_SUCCESS : Release completed \n
 *                        \ref GA_FAILURE : Release Failed \n
 * \param [in] evt_data  NULL \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_CE_RELEASE_MCS_CNF                          0x4FU

/** \} */

/** \} */

/**
 * \defgroup ga_mcp_ce_constants Constants
 * \{
 * \brief Describes Constants defined by the module.
 */

/**
 * \name MCP Client Constants - Object Attribute ID
 * \{
 * \brief This section lists the Attribute ID that can be used by the
 * Application to trigger read requests \if MCP_SUPPORT_OBJECT_TRANSFER while
 * calling \ref GA_mcp_ce_read_object_attr \endif.
 */

/** Feature Name */
#define MCP_CE_OBJ_ATTR_ID_FEATURE_NAME       0x01U
/** Name */
#define MCP_CE_OBJ_ATTR_ID_NAME               0x02U
/** Type */
#define MCP_CE_OBJ_ATTR_ID_TYPE               0x03U
/** MCP Object Attribute ID - Size
 * \note Includes First 4B Current Size and next 4B Alloc size.
 */
#define MCP_CE_OBJ_ATTR_ID_SIZE               0x04U
/** First Created */
#define MCP_CE_OBJ_ATTR_ID_FIRST_CREATED      0x05U
/** Last Modified */
#define MCP_CE_OBJ_ATTR_ID_LAST_MOD           0x06U
/** Object ID */
#define MCP_CE_OBJ_ATTR_ID_OBJ_ID             0x07U
/** Property */
#define MCP_CE_OBJ_ATTR_ID_PROP               0x08U
/** OACP */
#define MCP_CE_OBJ_ATTR_ID_OACP               0x09U
/** OLCP */
#define MCP_CE_OBJ_ATTR_ID_OLCP               0x0AU
/** List Filter */
#define MCP_CE_OBJ_ATTR_ID_LIST_FILTER        0x0BU
/** Changed */
#define MCP_CE_OBJ_ATTR_ID_CHGD               0x0CU

/** \} */

/**
 * \name MCP Client Constants - Write Type
 * \{
 * \brief This section lists the type of Write allowed in MCP
 * Profile.
 */

/** Request */
#define MCP_CE_WRITE_TYPE_REQUEST             0x00U

/** Command */
#define MCP_CE_WRITE_TYPE_WITHOUT_RSP         0x01U

/** \} */

/**
 * \name MCP Client Constants - Object Type
 * \{
 * \brief This section lists the type of Objects in MCP Profile.
 */

/** Media Player Icon */
#define MCP_CE_OBJ_TYPE_MP_ICON               0x01U
/** Track Segment */
#define MCP_CE_OBJ_TYPE_TRACK_SEG             0x02U
/** Current Track IDV32 */
#define MCP_CE_OBJ_TYPE_CURR_TRACK_ID3V2      0x03U
/** Next Track IDV32 */
#define MCP_CE_OBJ_TYPE_NEXT_TRACK_ID3V2      0x04U
/** Parent Group */
#define MCP_CE_OBJ_TYPE_PARENT_GROUP          0x05U
/** Current Group */
#define MCP_CE_OBJ_TYPE_CURR_GROUP            0x06U
/** Search Result Group */
#define MCP_CE_OBJ_TYPE_SEARCH_RES_GROUP      0x07U
/** Invalid */
#define MCP_CE_OBJ_TYPE_INVALID               0xFFU

/** \} */

/**
 * \name MCP Client Constants - Opcode
 * \{
 * \brief This section lists the type of MCP Opcodes used during MCP Write
 * request.
 * As defined in MCP Specification.
 */

/** Play */
#define MCP_CP_PLAY                           0x01U
/** Pause */
#define MCP_CP_PAUSE                          0x02U
/** Fast Rewind */
#define MCP_CP_FAST_RWD                       0x03U
/** Fast Forward */
#define MCP_CP_FAST_FWD                       0x04U
/** Stop */
#define MCP_CP_STOP                           0x05U
/** Move Relative */
#define MCP_CP_MOVE_RELATIVE                  0x10U
/** Previous Segment */
#define MCP_CP_PREV_SEG                       0x20U
/** Next Segment */
#define MCP_CP_NEXT_SEG                       0x21U
/** First Segment */
#define MCP_CP_FIRST_SEG                      0x22U
/** Last Segment */
#define MCP_CP_LAST_SEG                       0x23U
/** Goto Segment */
#define MCP_CP_GOTO_SEG                       0x24U
/** Previous Track */
#define MCP_CP_PREV_TRACK                     0x30U
/** Next Track */
#define MCP_CP_NEXT_TRACK                     0x31U
/** First Track */
#define MCP_CP_FIRST_TRACK                    0x32U
/** Last Track */
#define MCP_CP_LAST_TRACK                     0x33U
/** Goto Track */
#define MCP_CP_GOTO_TRACK                     0x34U
/** Previous Group */
#define MCP_CP_PREV_GROUP                     0x40U
/** Next Group */
#define MCP_CP_NEXT_GROUP                     0x41U
/** First Group */
#define MCP_CP_FIRST_GROUP                    0x42U
/** Last Group */
#define MCP_CP_LAST_GROUP                     0x43U
/** Goto Group */
#define MCP_CP_GOTO_GROUP                     0x44U

/** \} */

/**
 * \name MCP Client Constants - SCP Type
 * \{
 * \brief This section lists the type of SCP Opcodes used during MCP Write
 * request.
 * As defined in MCP Specification.
 */

/** Track Name */
#define MCP_SCP_TYPE_TRACK_NAME               0x01U
/** Artist Name */
#define MCP_SCP_TYPE_ARTIST_NAME              0x02U
/** Album Name */
#define MCP_SCP_TYPE_ALBUM_NAME               0x03U
/** Group Name */
#define MCP_SCP_TYPE_GROUP_NAME               0x04U
/** Earliest Year */
#define MCP_SCP_TYPE_EARLIEST_YEAR            0x05U
/** Latest Year */
#define MCP_SCP_TYPE_LATEST_YEAR              0x06U
/** Genre */
#define MCP_SCP_TYPE_GENRE                    0x07U
/** Tracks */
#define MCP_SCP_TYPE_ONLY_TRACKS              0x08U
/** Groups */
#define MCP_SCP_TYPE_ONLY_GROUPS              0x09U

/** \} */

/**
 * \name MCP Client Constants - General Macros
 * \{
 * \brief Initialization and other General Macros offered by the module.
 */

/**
 * Length of the Object ID as defined by OTP Module.
 */
#define OBJ_ID_LEN                            0x06U

/** MCP CE GMCS Enable Handle */
#define MCP_CE_GMCS_ENABLE                    0x80U

/**
 * Total number of Characteristic IDs,
 * \ref ga_mcs_char_prop
 */
#define MCP_CHAR_ID_COUNT                               22U

/** \} */

/**
 * \name MCP Client Constants - MCP Char ID
 * \{
 * This section lists the Characteristic ID references.
 */

/** MCS Char ID: Media Player Name,
 * \ref GA_CHAR_PROP_MCS_MEDIA_PLAYER_NAME
 */
#define MCP_CHAR_ID_MEDIA_PLAYER_NAME                   0U
/** MCS Char ID: Media Player Icon Object ID,
 * \ref GA_CHAR_PROP_MCS_MEDIA_PLAYER_ICON_OBJ_ID
 */
#define MCP_CHAR_ID_MEDIA_PLAYER_ICON_OBJ_ID            1U
/** MCS Char ID: Media Player Icon URL,
 * \ref GA_CHAR_PROP_MCS_MEDIA_PLAYER_ICON_URL
 */
#define MCP_CHAR_ID_MEDIA_PLAYER_ICON_URL               2U
/** MCS Char ID: Track Changed,
 * \ref GA_CHAR_PROP_MCS_TRACK_CHANGED
 */
#define MCP_CHAR_ID_TRACK_CHANGED                       3U
/** MCS Char ID: Track Title,
 * \ref GA_CHAR_PROP_MCS_TRACK_TITLE
 */
#define MCP_CHAR_ID_TRACK_TITLE                         4U
/** MCS Char ID: Track Duration,
 * \ref GA_CHAR_PROP_MCS_TRACK_DURATION
 */
#define MCP_CHAR_ID_TRACK_DURATION                      5U
/** MCS Char ID: Track Position,
 * \ref GA_CHAR_PROP_MCS_TRACK_POS
 */
#define MCP_CHAR_ID_TRACK_POS                           6U
/** MCS Char ID: Playback Speed,
 * \ref GA_CHAR_PROP_MCS_PLAYBACK_SPEED
 */
#define MCP_CHAR_ID_PLAYBACK_SPEED                      7U
/** MCS Char ID: Seeking Speed,
 * \ref GA_CHAR_PROP_MCS_SEEKING_SPEED
 */
#define MCP_CHAR_ID_SEEKING_SPEED                       8U
/** MCS Char ID: Current Track Segments Object ID,
 * \ref GA_CHAR_PROP_MCS_CURR_TRACK_SEG_OBJ_ID
 */
#define MCP_CHAR_ID_CURR_TRACK_SEG_OBJ_ID               9U
/** MCS Char ID: Current Track Object ID,
 * \ref GA_CHAR_PROP_MCS_CURR_TRACK_OBJ_ID
 */
#define MCP_CHAR_ID_CURR_TRACK_OBJ_ID                   10U
/** MCS Char ID: Next Track Object ID,
 * \ref GA_CHAR_PROP_MCS_NEXT_TRACK_OBJ_ID
 */
#define MCP_CHAR_ID_NEXT_TRACK_OBJ_ID                   11U
/** MCS Char ID: Parent Group Object ID,
 * \ref GA_CHAR_PROP_MCS_PARENT_GROUP_OBJ_ID
 */
#define MCP_CHAR_ID_PARENT_GROUP_OBJ_ID                 12U
/** MCS Char ID: Current Group Object ID,
 * \ref GA_CHAR_PROP_MCS_CURR_GROUP_OBJ_ID
 */
#define MCP_CHAR_ID_CURR_GROUP_OBJ_ID                   13U
/** MCS Char ID: Playing Order,
 * \ref GA_CHAR_PROP_MCS_PLAYING_ORDER
 */
#define MCP_CHAR_ID_PLAYING_ORDER                       14U
/** MCS Char ID: Playing Orders Supported,
 * \ref GA_CHAR_PROP_MCS_PLAYING_ORDERS_SUPP
 */
#define MCP_CHAR_ID_PLAYING_ORDERS_SUPP                 15U
/** MCS Char ID: Media State,
 * \ref GA_CHAR_PROP_MCS_MEDIA_STATE
 */
#define MCP_CHAR_ID_MEDIA_STATE                         16U
/** MCS Char ID: Media Control Point,
 * \ref GA_CHAR_PROP_MCS_MEDIA_CONTROL_POINT
 */
#define MCP_CHAR_ID_MEDIA_CONTROL_POINT                 17U
/** MCS Char ID: Media Control Point Opcodes Supported,
 * \ref GA_CHAR_PROP_MCS_CP_OPC_SUPP
 */
#define MCP_CHAR_ID_CP_OPC_SUPP                         18U
 /** MCS Char ID: Search Control Point,
 * \ref GA_CHAR_PROP_MCS_SEARCH_CONTROL_POINT
 */
#define MCP_CHAR_ID_SEARCH_CONTROL_POINT                19U
/** MCS Char ID: Search Results Object ID,
 * \ref GA_CHAR_PROP_MCS_SEARCH_RES_OBJ_ID
 */
#define MCP_CHAR_ID_SEARCH_RES_OBJ_ID                   20U
 /** MCS Char ID: Content Control ID,
 * \ref GA_CHAR_PROP_MCS_CONTENT_CONTROL_ID
 */
#define MCP_CHAR_ID_CONTENT_CONTROL_ID                  21U
/** \} */
/** \} */

/**
 * \defgroup ga_mcp_ce_macros Macros
 * \{
 * \brief Initialization and other Macros offered by the module.
 */

 /** Set Generic service to handle */
#define MCP_CE_SET_HANDLE_GMCS(x) \
        (x) = (MCP_CE_GMCS_ENABLE | (x))

 /** Validate MCP Handle refers to GMCS Macro */
#define MCP_CE_IS_HANDLE_GMCS(x) \
    (MCP_CE_GMCS_ENABLE & (x))

/** Derive the characteristic config mask for the Char ID */
#define MCP_CHAR_CONFIG(id)                             (1U << (id))

/** \} */

/* --------------------------------------------- Data types /Structures */

/**
 * \defgroup ga_mcp_ce_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by MCP CE.
 */

/**
 * The field in the handle indicates the Handle type and Handle ID.
 * Handle type indicates if it is a GMCS or a MCS session.
 * The application needs to provide this as a input param for all the calls
 * to have transaction on the required GMCS/MCS handle.
 * A call to \ref MCP_CE_IS_HANDLE_GMCS() can validate the Handle Type.
 */
typedef UINT8 MCP_HANDLE;

/** Event that will be notified by the MCP CE module. */
typedef UINT8 MCP_CE_EVT_ID;

/** Types of write operations available */
typedef UINT8 MCP_CE_WRITE_TYPE;

/** Object Type received during read request */
typedef UINT8 MCP_CE_OBJ_TYPE;

/** MCP Opcodes */
typedef UINT8 MCP_CP_OPCODES;

/** SCP Opcodes */
typedef UINT8 MCP_SCP_OPCODES;

/** Object attributes/metadata of an object */
typedef UINT8 MCP_CE_OBJ_ATTR_ID;

/** MCP CE Write Request Format */
typedef struct _GA_MCP_CE_WRITE_REQ_
{
    /** Write request Value */
    GA_BRR_CHR_VAL     req_data;

    /** Write Type representation */
    MCP_CE_WRITE_TYPE  write_type;

}GA_MCP_CE_WRITE_REQ;

/** MCP CE Object ID defined as per OTP specification */
typedef struct _GA_MCP_CE_OBJ_ID_
{
    /** Object ID */
    UINT8 obj[OBJ_ID_LEN];

}GA_MCP_CE_OBJ_ID;

/** MCP CE Object Attribute response */
typedef struct _GA_MCP_CE_OBJ_ATTR_
{
    /** Object Attribute ID representing the received Attribute */
    MCP_CE_OBJ_ATTR_ID   attr_id;

    /** Object Attribute data */
    UINT8              * data;

    /** Object Attribute Data length */
    UINT16               len;

}GA_MCP_CE_OBJ_ATTR;

/** MCP CE Object Type - Media Player Icon */
typedef struct _GA_MCP_CE_MP_ICON_OBJ_
{
    /** Represented as .png format */
    void   * icon_png_data;

    /** Total Object length */
    UINT16   total_obj_len;

}GA_MCP_CE_MP_ICON_OBJ;

/** MCP CE Object Type - Track Segments */
typedef struct _GA_MCP_CE_TRACK_SEG_OBJ_
{
    /** Length of the segment */
    UINT8   seg_name_len;

    /** Name of the segment */
    UINT8 * seg_name;

    /** Position of the Segment */
    INT32   seg_pos;

    /** Obj len will be Seg Name len field len + Seg Name len + Seg Position length */
    UINT16  total_obj_len;

}GA_MCP_CE_TRACK_SEG_OBJ;

/** MCP CE Object Type - Track */
typedef struct _GA_MCP_CE_TRACK_OBJ_
{
    /** Represented as IDV32 format */
    void   * IDV32_data;

    /** Obj len will be complete ID3v2 length */
    UINT16   total_obj_len;

}GA_MCP_CE_TRACK_OBJ;

/** MCP CE Object Type - Group */
typedef struct _GA_MCP_CE_GRP_OBJ_
{
    /** Type of the Object */
    UINT8  obj_type;

    /** Object ID */
    UINT8  obj[OBJ_ID_LEN];

    /** Obj len will be [(Obj Type Len + OBJ_ID_LEN ) * Number of such obj] */
    UINT16 total_obj_len;

}GA_MCP_CE_GRP_OBJ;

/** MCP CE Event */
typedef struct _MCP_CE_EVT_
{
    /** MCP Handle on which MCP event is received */
    MCP_HANDLE   mcp_handle;

    /** Data associated with the event */
    void       * data;

    /** Length of the event data */
    UINT16       len;

}MCP_CE_EVT;

/** \} */

/** \} */
/** \} */

/**
 * \addtogroup ga_mcp_cb Application Callback
 * \{
 * \brief This Section Describes the module Callback interface offered
 * to the application
 */

/**
 * \defgroup ga_mcp_ce_module_cb MCP CE (Media Control Profile) Client Entity
 * \{
 * \brief This section describes the callback for MCP CE.
 */

/**
 * MCP CE Callback to be registered by the Application
 *
 * \param [in] device     Peer Device Info
 * \param [in] evt_id     Event type
 * \param [in] evt_status Event status
 * \param [in] evt_data   Event Data and Datalength
 */
typedef GA_RESULT (* MCP_CE_NTF_CB)
                  (
                      GA_ENDPOINT    * device,
                      MCP_CE_EVT_ID    evt_id,
                      UINT16           evt_status,
                      MCP_CE_EVT     * evt_data
                  );

/** \} */
/** \} */

/* --------------------------------------------- Macros */
/* --------------------------------------------- APIs */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup ga_mcp_api_sequences API Sequences
 * \{
 * \brief This section describes the API Sequences.
 */
/**
 * \defgroup ga_mcp_api_seq MCP API Sequences
 * \{
 * \brief This section describes the Media Control Profile API Sequences.
 * MSC depicting the flow of APIs and Events.
 * \mscfile MCP.signalling
 */

/** \} */
/** \} */

/**
 * \addtogroup ga_mcp_api_defs API Definitions
 * \{
 * \brief This section describes the EtherMind Media Control Profile APIs.
 */

/**
 * \defgroup ga_mcp_ce_api_defs MCP CE (Media Control Profile) Client Entity
 * \{
 * \brief This section describes the Media Control Profile APIs
 * for Client.
 */

/**
 * \name MCP Client APIs - Init
 * \{
 * \brief This section describes the Media Control Profile Init APIs
 * for Client.
 */

/**
 *  \brief To Init the MCP Client Module
 *
 *  \par Description:
 *       This function enables to initialize the MCP Client Entity Module.
 *       and register a callback with MCP Module.
 *       This callback will be triggered whenever there are events generated
 *       as part of the functionality and also to indicate Notifications
 *       received from peer.
 *
 *  \param [in] cb
 *         MCP CE Callback.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_init
          (
              /* IN */ MCP_CE_NTF_CB cb
          );

/** \} */

/**
 * \name MCP Client APIs - Setup
 * \{
 * \brief This section describes the Media Control Profile Setup APIs
 * for Client.
 */

/**
 * \cond EXCLUDE_PART
 */

#ifdef MCP_SUPPORT_CONTEXT_MANAGE
/**
 *  \brief To Retrieve/Save the MCP Context Information.
 *
 *  \par Description:
 *       This function enables to get/set the context information of given GMCS/MCS.
 *
 *  \param [in] set
 *         Flag to indicate get/set the context information.
 *          - GA_TRUE: Will save the service context information.
 *          - GA_FALSE: Will retrieve the service the context information.
 *
 *  \param [in] device
 *         Remote Device Address.
 *
 *  \param [inout] handle
 *         MCP Handle which is allocated during GMCS/MCS context setup.
 *
 *  \param [inout] range
 *         GMCS/MCS characteristics handle range.
 *
 *  \param [inout] info
 *          GMCS/MCS characteristics information.
 *
 *  \param [inout] info_count
 *          GMCS/MCS characteristics count.
 *
 *  \return GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT GA_mcp_ce_manage_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT      * device,
              /* INOUT  */ MCP_HANDLE       * handle,
              /* INOUT  */ GA_BRR_SVC_RANGE ** range,
              /* INOUT  */ GA_CHAR_INFO     ** info,
              /* INOUT  */ UINT16           * info_count
          );

/**
 *  \brief To set the GA MCP characteristic handles of a device.
 *
 *  \par Description
 *       The routine enables the application to set the service and
 *       characteristic handle ranges from a device context.
 *
 *  \param [in] d
 *              Remote device endpoint
 *  \param [in] h
 *              Context handle for the endpoint
 *  \param [in] r
 *              Service range array for GMCS and MCS
 *  \param [in] i
 *              Array of characteristic handles for GMCS and MCS
 *  \param [in] c
 *              Count of characteristics handles in above parameter
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h
 */
#define GA_mcp_ce_set_context_info(d, h, r, i, c) \
        GA_mcp_ce_manage_context_info(GA_TRUE, (d), (h), (r), (i), (c))

/**
 *  \brief To get the GA MCP characteristic handles of a device.
 *
 *  \par Description
 *       The routine enables the application to get the service and
 *       characteristic handle ranges from a device context.
 *
 *  \param [in] d
 *              Remote device endpoint
 *  \param [in] h
 *              Context handle for the endpoint
 *  \param [in] r
 *              Service range array for GMCS and MCS
 *  \param [in] i
 *              Array of characteristic handles for GMCS and MCS
 *  \param [in] c
 *              Count of characteristics handles in above parameter
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *          \ref GA_error.h
 */
#define GA_mcp_ce_get_context_info(d, h, r, i, c) \
        GA_mcp_ce_manage_context_info(GA_FALSE, (d), (h), (r), (i), (c))
#endif /* MCP_SUPPORT_CONTEXT_MANAGE */

/**
 * \endcond
 */

/**
 *  \brief To Setup Generic Media Control Service(GMCS) Context
 *
 *  \par Description:
 *       This function enables to setup GMCS context with given device.
 *       Internally, GMCS service, char and descriptors for the GMCS service
 *       will be discovered. If OTS support is present on the remote,
 *       and is enabled locally, MCP Control and Data Channel will be
 *       established and corresponding events with status are notified.
 *       mcp_handle to be used by application for further calls to have
 *       transaction on this GMCS Handle.
 *
 *  \param [in] device
 *         Peer Device with which the context needs to be set-up.
 *
 *  \param [out] mcp_handle
 *         GMCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref MCP_CE_SETUP_CNF is notified on
 *          completion with status as success or failure.
 *          \if MCP_SUPPORT_OBJECT_TRANSFER
 *            If OTS support is present, \ref MCP_CE_CONTROL_CHANNEL_SETUP_CNF
 *            and/or \ref MCP_CE_DATA_CHANNEL_UP_CNF will be notified with
 *            status.
 *          \endif
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_setup_context
          (
              /* IN */   GA_ENDPOINT * device,
              /* OUT */  MCP_HANDLE  * mcp_handle
          );

/**
 *  \brief To Discover Media Control Service(MCS) instances on peer.
 *
 *  \par Description:
 *       This function enables to discover MCS service handles.
 *
 *  \param [in] device
 *         Peer Device on which discovery has to happen.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If \ref GA_SUCCESS, \ref MCP_CE_DISC_MCS_CNF is notified with service
 *          handle range and status.
 *
 *  \note The device must have performed GMCS setup before calling
 *        this function.
 *        If MCS context has to be setup, \ref GA_mcp_ce_setup_mcs_context() has to
 *        be called with the service handle range received as part of this
 *        event.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_discover_mcs
          (
              /* IN */  GA_ENDPOINT * device
          );

/**
 *  \brief To Setup Media Control Service(MCS) Context
 *
 *  \par Description:
 *       This function enables to setup MCS context with given device.
 *       Internally, MCS service, char and descriptors for the GMCS service
 *       instance will be discovered.
 *       mcp_handle to be used by application for further calls to have
 *       transaction on this MCS Handle.
 *
 *  \param [in] device
 *         Peer Device with which the context needs to be set-up.
 *
 *  \param [in] srv_info
 *         Service Handle range to be provided for MCS Service Instance Search
 *
 *  \param [out] mcp_handle
 *         MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If \ref GA_SUCCESS, \ref MCP_CE_SETUP_CNF is notified on
 *          completion with status as success or failure.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_setup_mcs_context
          (
              /* IN */   GA_ENDPOINT     * device,
              /* IN */   GA_BRR_SVC_INFO * srv_info,
              /* OUT */  MCP_HANDLE      * mcp_handle
          );

/**
 *  \brief Release or free the given GMCS/MCS context.
 *
 *  \par Description
 *  When 'free' is set to \ref GA_FALSE, this routine initiates the release
 *  procedure for the context. Once release is done, the context is freed up
 *  and the setup must be freshly done by calling
 *  \ref GA_mcp_ce_setup_context or
 *  \ref GA_mcp_ce_setup_mcs_context() if required for
 *  the same device again.
 *
 *  If the 'free' parameter is set to \ref GA_TRUE,
 *  this API just frees up the context without the release procedure.
 *  If the given handle is GMCS Handle, any related MCS contexts shall be
 *  freed by the application before freeing up the GMCS context.
 *  If the given handle is a MCS Handle, the respective MCS context is freed.
 *
 *  \param [in] context Context to be released/freed
 *  \param [in] free Indicate free only without release
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref  is notified on
 *          completion with status as success or failure.
 */
GA_RESULT GA_mcp_ce_terminate
          (
              /* IN */ MCP_HANDLE handle,
              /* IN */ UCHAR      free
          );

/**
 *  \brief Release the given GA context.
 *
 *  \par Description
 *  This routine initiates the release procedure for the context. Once release
 *  is done, the context is freed up and the setup must be freshly done by
 *  calling \ref GA_mcp_ce_setup_context or
 * \ref GA_mcp_ce_setup_mcs_context() if required for the same device again.
 *
 *  \param [in] context Context to be released
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref MCP_CE_RELEASE_CNF or
 *          \ref MCP_CE_RELEASE_MCS_CNF is notified on completion with status
 *          as success or failure.
 */
#define GA_mcp_ce_release(ctx) \
        GA_mcp_ce_terminate((ctx), GA_FALSE)

/**
 *  \brief Free the given GA context.
 *
 *  \par Description
 *  This routine frees up the given context of the GA layer.
 *  If the given handle is GMCS Handle, any related MCS contexts shall be
 *  freed by the application before freeing up the GMCS context.
 *  If the given handle is a MCS Handle, the respective MCS context is freed.
 *
 *  \param [in] context Context to be freed
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_mcp_ce_close(ctx) \
        GA_mcp_ce_terminate((ctx), GA_TRUE)

/**
 *  \brief To Configure a Char for Notifications.
 *
 *  \par Description:
 *       This function enables/disables notifications on the requested Char
 *       UUID \ref GA_assigned_numbers.h.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcp_ce_exp_evt
 *         Expected event ID to be generated once this operation is performed.
 *
 *  \param [in] char_uuid
 *         Char UUID on which Config req is requested.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 *
 *  \note If using this function to configure char,
 *        set mcp_ce_exp_evt by referring to \ref
 *        ga_mcp_ce_events.
 */
GA_RESULT GA_mcp_ce_configure_context_char
          (
              /* IN */  MCP_HANDLE    mcp_handle,
              /* IN */  MCP_CE_EVT_ID mcp_ce_exp_evt,
              /* IN */  UINT16        char_uuid,
              /* IN */  UINT8         mcs_ntf_flag
          );

/** \} */

/**
 * \name MCP Client APIs - Read
 * \{
 * \brief This section describes the Media Control Profile Read APIs for
 * Client.
 */

/**
 *  \brief To perform Read operation on a Char.
 *
 *  \par Description:
 *       This function triggers read request.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcp_ce_exp_evt
 *         Expected event ID to be generated once this operation is performed.
 *
 *  \param [in] char_uuid
 *         Char UUID on which read is requested.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref ga_mcp_ce_events is received.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 *
 *  \note If using this function to perform read,
 *        set mcp_ce_exp_evt by referring to \ref
 *        ga_mcp_ce_events.
 */
GA_RESULT GA_mcp_ce_read_request
          (
              /* IN */  MCP_HANDLE    mcp_handle,
              /* IN */  MCP_CE_EVT_ID mcp_ce_exp_evt,
              /* IN */  UINT16        char_uuid
          );

/**
 *  \brief To Set Object reference of MCP Char.
 *
 *  \par Description:
 *       This function reads object ID by performing read request and sets the
 *       Obj reference for further transaction.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcp_ce_exp_evt
 *         Expected event ID to be generated once this operation is performed.
 *
 *  \param [in] char_uuid
 *         Char UUID on which write is requested.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer and the obj ref is set,
 *          \ref MCP_CE_SET_OBJ_REF_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_set_object_reference
          (
              /* IN */  MCP_HANDLE    mcp_handle,
              /* IN */  MCP_CE_EVT_ID mcp_ce_exp_evt,
              /* IN */  UINT16        char_uuid
          );

/** \} */

/**
 * \name MCP Client APIs - Write
 * \{
 * \brief This section describes the Media Control Profile Write APIs for
 * Client.
 */

/**
 *  \brief To perform Write operation on a Char.
 *
 *  \par Description:
 *       This function triggers write request.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcp_ce_exp_evt
 *         Expected event ID to be generated once this operation is performed.
 *
 *  \param [in] char_uuid
 *         Char UUID on which write is requested.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If the requested operation is a Write Request,
 *          \ref ga_mcp_ce_events is received.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 *
 *  \note If using this function to write,
 *        set mcp_ce_exp_evt by referring to \ref
 *        ga_mcp_ce_events.
 */
GA_RESULT GA_mcp_ce_write_request
          (
              /* IN */  MCP_HANDLE            mcp_handle,
              /* IN */  MCP_CE_EVT_ID         mcp_ce_exp_evt,
              /* IN */  UINT16                char_uuid,
              /* IN */  GA_MCP_CE_WRITE_REQ * req
          );
/** \} */

/**
 * \name MCP Client APIs - Setup
 * \{
 * \brief This section describes the Media Control Profile Setup APIs
 * for Client.
 */

/**
 * \cond EXCLUDE_PART
 */

#ifdef MCP_SUPPORT_CONFIG_SELECTION
/**
 *  \brief To update the configuration mask
 *
 *  \par Description:
 *       This function helps to update the configuration setting of which
 *       of the MCS characteristics of a peer needs configuration at setup.
 *
 *  \param [in] config
 *         Bitmask of the Characteristic IDs for configuration
 *
 *  \return GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 *  \ref
 */
GA_RESULT GA_mcp_ce_update_ntf_configuration(UINT32 config);
#endif /* MCP_SUPPORT_CONFIG_SELECTION */

/**
 * \endcond
 */

/**
 *  \brief To Configure all chars for Notifications.
 *
 *  \par Description:
 *       This function enables/disables all chars for notifications in a
 *       chained manner.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          Once all the config is complete,
 *          \ref MCP_CE_ENABLE_ALL_CFG_CNF /
 *          \ref MCP_CE_DISABLE_ALL_CFG_CNF event is notified.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_ENABLE_ALL_CFG_CNF / \ref MCP_CE_DISABLE_ALL_CFG_CNF
 *          respectively is notified.
 *          If Notification is received from peer device,
 *          \ref ga_mcp_ce_events is notified based on the char.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_configure_context
          (
              /* IN */  MCP_HANDLE mcp_handle,
              /* IN */  UINT8      mcs_ntf_flag
          );

/**
 *  \brief To Configure Media Player Name for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Media Player Name for notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_MEDIA_PLAYER_NAME_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_MEDIA_PLAYER_NAME_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_media_player_name(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_MEDIA_PLAYER_NAME_CFG_CNF), (GA_CHAR_MCS_MEDIA_PLAYER_NAME), (mcs_ntf_flag))

/**
 *  \brief To Configure Track Changed for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Track Changed for notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_TRACK_CHANGED_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_TRACK_CHANGED_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_track_changed(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_TRACK_CHANGED_CFG_CNF), (GA_CHAR_MCS_TRACK_CHANGED), (mcs_ntf_flag))

/**
 *  \brief To Configure Track Title for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Track Title for notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_TRACK_TITLE_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_TRACK_TITLE_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_track_title(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char  \
        ((mcp_handle), (MCP_CE_TRACK_TITLE_CFG_CNF), (GA_CHAR_MCS_TRACK_TITLE), (mcs_ntf_flag))

/**
 *  \brief To Configure Track Duration for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Track Duration for notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_TRACK_DUR_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_TRACK_DUR_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_track_duration(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_TRACK_DUR_CFG_CNF), (GA_CHAR_MCS_TRACK_DURATION), (mcs_ntf_flag))

/**
 *  \brief To Configure Track Position for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Track Position for notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_TRACK_POS_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_TRACK_POS_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_track_pos(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_TRACK_POS_CFG_CNF), (GA_CHAR_MCS_TRACK_POS), (mcs_ntf_flag))

/**
 *  \brief To Configure Playback Speed for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Playback Speed for notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_PLAYBACK_SPEED_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_PLAYBACK_SPEED_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_playback_speed(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_PLAYBACK_SPEED_CFG_CNF), (GA_CHAR_MCS_PLAYBACK_SPEED), (mcs_ntf_flag))

/**
 *  \brief To Configure Seeking Speed for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Seeking Speed for notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_SEEKING_SPEED_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_SEEKING_SPEED_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_seeking_speed(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_SEEKING_SPEED_CFG_CNF), (GA_CHAR_MCS_SEEKING_SPEED), (mcs_ntf_flag))

/**
 *  \brief To Configure Current Track Object ID for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Current Track Object ID for
 *       notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_CURR_TRACK_OBJ_ID_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_CURR_TRACK_OBJ_ID_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_curr_track_obj_id(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_CURR_TRACK_OBJ_ID_CFG_CNF), (GA_CHAR_MCS_CURR_TRACK_OBJ_ID), (mcs_ntf_flag))

/**
 *  \brief To Configure Next Track Object ID for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Next Track Object ID for
 *       notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_NEXT_TRACK_OBJ_ID_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_NEXT_TRACK_OBJ_ID_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_next_track_obj_id(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_NEXT_TRACK_OBJ_ID_CFG_CNF), (GA_CHAR_MCS_NEXT_TRACK_OBJ_ID), (mcs_ntf_flag))

/**
 *  \brief To Configure Parent Group Object ID for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Parent Group Object ID for
 *       notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_PARENT_GROUP_OBJ_ID_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_PARENT_GROUP_OBJ_ID_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_parent_grp_obj_id(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_PARENT_GROUP_OBJ_ID_CFG_CNF), (GA_CHAR_MCS_PARENT_GROUP_OBJ_ID), (mcs_ntf_flag))

/**
 *  \brief To Configure Current Group Object ID for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Current Group Object ID for
 *       notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_CURR_GROUP_OBJ_ID_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_CURR_GROUP_OBJ_ID_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_curr_group_obj_id(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_CURR_GROUP_OBJ_ID_CFG_CNF), (GA_CHAR_MCS_CURR_GROUP_OBJ_ID), (mcs_ntf_flag))

/**
 *  \brief To Configure Playing Order for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Playing Order for
 *       notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_PLAYING_ORDER_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_PLAYING_ORDER_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_playing_order(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_PLAYING_ORDER_CFG_CNF), (GA_CHAR_MCS_PLAYING_ORDER), (mcs_ntf_flag))

/**
 *  \brief To Configure Media State for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Media State for
 *       notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_MEDIA_STATE_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_MEDIA_STATE_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_media_state(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_MEDIA_STATE_CFG_CNF), (GA_CHAR_MCS_MEDIA_STATE), (mcs_ntf_flag))

/**
 *  \brief To Configure Media Control Point(MCP) for Notifications.
 *
 *  \par Description:
 *       This function enables/disables MCP for notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_MCP_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_MCP_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_mcp(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_MCP_CFG_CNF), (GA_CHAR_MCS_MEDIA_CONTROL_POINT), (mcs_ntf_flag))

/**
 *  \brief To Configure MCP Opcode Supported for Notifications.
 *
 *  \par Description:
 *       This function enables/disables MCP Opcode Supported for notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_MCP_OPC_SUPP_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_MCP_OPC_SUPP_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_mcp_opc_supp(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_MCP_OPC_SUPP_CFG_CNF), (GA_CHAR_MCS_CP_OPC_SUPP), (mcs_ntf_flag))

/**
 *  \brief To Configure SCP for Notifications.
 *
 *  \par Description:
 *       This function enables/disables SCP for notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_SCP_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_SCP_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_scp(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_SCP_CFG_CNF), (GA_CHAR_MCS_SEARCH_CONTROL_POINT), (mcs_ntf_flag))

/**
 *  \brief To Configure Search Results Object ID for Notifications.
 *
 *  \par Description:
 *       This function enables/disables Search Results Object ID for
 *       notifications.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcs_ntf_flag
 *         Flag to indicate enable/disable notifications
 *         - 0x00 -> Disable
 *         - 0x01 -> Enable
 *         .
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          If Notification is enabled/disabled,
 *          \ref MCP_CE_SEARCH_RES_OBJ_ID_CFG_CNF is notified.
 *          If Notification is received from peer device,
 *          \ref MCP_CE_SEARCH_RES_OBJ_ID_IND is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_config_ntf_search_results_obj_id(mcp_handle, mcs_ntf_flag) \
        GA_mcp_ce_configure_context_char \
        ((mcp_handle), (MCP_CE_SEARCH_RES_OBJ_ID_CFG_CNF), (GA_CHAR_MCS_SEARCH_RES_OBJ_ID), (mcs_ntf_flag))

/** \} */

/**
 * \name MCP Client APIs - Read
 * \{
 * \brief This section describes the Media Control Profile Read APIs for
 * Client.
 */

/**
 *  \brief To Read Media Player Name.
 *
 *  \par Description:
 *       This function reads Media Player Name.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure. \n
 *               If \ref GA_SUCCESS, read response is received and notified
 *               via \ref MCP_CE_READ_MEDIA_PLAYER_NAME_CNF.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_media_player_name(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_MEDIA_PLAYER_NAME_CNF), (GA_CHAR_MCS_MEDIA_PLAYER_NAME))

/**
 *  \brief To Set Object reference of Media Player Icon.
 *
 *  \par Description:
 *       This function reads Media Player Icon Object ID and sets the Obj
 *       reference.
 *       \if MCP_SUPPORT_OBJECT_TRANSFER
 *         If app wants to read the object,
 *         \ref GA_mcp_ce_read_media_player_icon_obj_attr() can be
 *         called to fetch the object attributes.
 *         \ref GA_mcp_ce_read_media_player_icon_obj() can be called with
 *         required object ID, length and offset to fetch the contents.
 *       \endif
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer and the obj is set,
 *          \ref MCP_CE_SET_OBJ_REF_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_set_media_player_icon_obj(mcp_handle) \
        GA_mcp_ce_set_object_reference((mcp_handle), (MCP_CE_SET_OBJ_REF_CNF), (GA_CHAR_MCS_MEDIA_PLAYER_ICON_OBJ_ID))
/** \} */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/**
 * \name MCP Client APIs - OTP Read
 * \{
 * \brief This section describes the Media Control Profile OTP Read APIs for
 * Client.
 */

/**
 *  \brief To fetch the Object attributes of Media Player Icon.
 *
 *  \par Description:
 *       This function retrieves Media Player Icon Object attribute as
 *       requested.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID.
 *
 *  \param [in] attr_id
 *         Attribute ID to be read.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_GET_OBJ_ATTR_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_media_player_icon_obj_attr(mcp_handle, obj_id, attr_id) \
        GA_mcp_ce_read_object_attr((mcp_handle), (MCP_CE_GET_OBJ_ATTR_CNF), (obj_id), (attr_id))

/**
 *  \brief To fetch the Media Player Icon Object.
 *
 *  \par Description:
 *       This function fetches Media Player Icon Object.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID on which the Attribute is requested.
 *
 *  \param [in] offset
 *         Offset position to read data from.
 *
 *  \param [in] len
 *         Length of the object to be read.
 *
 *  \return \ref GA_CONTINUE This event will be called again if there are
 *          more contents. \n
 *          \ref GA_SUCCESS indicating completion of contents. \n
 *          Others indicating reason for failure.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_media_player_icon_obj(mcp_handle, obj_id, offset, length) \
        GA_mcp_ce_read_object((mcp_handle), (MCP_CE_READ_MEDIA_PLAYER_ICON_OBJ_CNF), (MCP_CE_OBJ_TYPE_MP_ICON), (obj_id), (offset), (length))

/** \} */

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/**
 * \name MCP Client APIs - Read
 * \{
 * \brief This section describes the Media Control Profile Read APIs for
 * Client.
 */

/**
 *  \brief To Read Media Player Icon URL.
 *
 *  \par Description:
 *       This function reads Media Player Icon URL.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_MEDIA_PLAYER_ICON_URL_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_media_player_icon_URL(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_MEDIA_PLAYER_ICON_URL_CNF), (GA_CHAR_MCS_MEDIA_PLAYER_ICON_URL))

/**
 *  \brief To Read Track Title.
 *
 *  \par Description:
 *       This function reads Track Title.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_TRACK_TITLE_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_track_title(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_TRACK_TITLE_CNF), (GA_CHAR_MCS_TRACK_TITLE))

/**
 *  \brief To Read Track Duration.
 *
 *  \par Description:
 *       This function reads Track Duration.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_TRACK_DUR_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_track_duration(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_TRACK_DUR_CNF), (GA_CHAR_MCS_TRACK_DURATION))

/**
 *  \brief To Read Track Position.
 *
 *  \par Description:
 *       This function reads Track Position.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_TRACK_POS_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_track_pos(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_TRACK_POS_CNF), (GA_CHAR_MCS_TRACK_POS))

 /**
 *  \brief To Read Playback Speed.
 *
 *  \par Description:
 *       This function reads Playback Speed.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_PLAYBACK_SPEED_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_playback_speed(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_PLAYBACK_SPEED_CNF), (GA_CHAR_MCS_PLAYBACK_SPEED))

/**
 *  \brief To Read Seeking Speed.
 *
 *  \par Description:
 *       This function reads Seeking Speed.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_SEEKING_SPEED_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_seeking_speed(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_SEEKING_SPEED_CNF), (GA_CHAR_MCS_SEEKING_SPEED))

/**
 *  \brief To Set Object reference of Current Track Segment.
 *
 *  \par Description:
 *       This function reads Current Track Segments Object ID and sets the Obj
 *       reference.
 *       \if MCP_SUPPORT_OBJECT_TRANSFER
 *         If app wants to read the object,
 *         \ref GA_mcp_ce_read_curr_track_seg_obj_attr() can be called
 *         to fetch the object attributes.
 *         \ref GA_mcp_ce_read_curr_track_seg_obj() can be called with required
 *         object ID, length and offset to fetch the contents.
 *       \endif
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer and the obj is set,
 *          \ref MCP_CE_SET_OBJ_REF_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_set_curr_track_seg_obj(mcp_handle) \
        GA_mcp_ce_set_object_reference((mcp_handle), (MCP_CE_SET_OBJ_REF_CNF), (GA_CHAR_MCS_CURR_TRACK_SEG_OBJ_ID))

/** \} */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/**
 * \name MCP Client APIs - OTP Read
 * \{
 * \brief This section describes the Media Control Profile OTP Read APIs for
 * Client.
 */

/**
 *  \brief To fetch the Object attributes of Current Track Segment.
 *
 *  \par Description:
 *       This function retrieves Current Track Segment Object attribute as
 *       requested.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID.
 *
 *  \param [in] attr_id
 *         Attribute ID to be read.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_GET_OBJ_ATTR_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_curr_track_seg_obj_attr(mcp_handle, obj_id, attr_id) \
        GA_mcp_ce_read_object_attr((mcp_handle), (MCP_CE_GET_OBJ_ATTR_CNF), (obj_id), (attr_id))

/**
 *  \brief To fetch the Current Track Segment Object.
 *
 *  \par Description:
 *       This function fetches Current Track Segment Object.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID on which the Attribute is requested.
 *
 *  \param [in] offset
 *         Offset position to read data from.
 *
 *  \param [in] len
 *         Length of the object to be read.
 *
 *  \return \ref GA_CONTINUE This event will be called again if there are
 *          more contents. \n
 *          \ref GA_SUCCESS indicating completion of contents. \n
 *          Others indicating reason for failure. \n
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_CURR_TRACK_SEG_OBJ_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_curr_track_seg_obj(mcp_handle, obj_id, offset, length) \
        GA_mcp_ce_read_object((mcp_handle), (MCP_CE_READ_CURR_TRACK_SEG_OBJ_CNF), (MCP_CE_OBJ_TYPE_CURR_TRACK_ID3V2), (obj_id), (offset), (length))
/** \} */
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/**
 * \name MCP Client APIs - Read
 * \{
 * \brief This section describes the Media Control Profile Read APIs for
 * Client.
 */

/**
 *  \brief To Set Object reference of Current Track.
 *
 *  \par Description:
 *       This function reads Current Track Object ID and sets the Obj
 *       reference.
 *       \if MCP_SUPPORT_OBJECT_TRANSFER
 *         If app wants to read the object,
 *         \ref GA_mcp_ce_read_curr_track_obj_attr() can be called
 *         to fetch the object attributes.
 *         \ref GA_mcp_ce_read_curr_track_obj() can be called with required
 *         object ID, length and offset to fetch the contents.
 *       \endif
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer and the obj is set,
 *          \ref MCP_CE_SET_OBJ_REF_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_set_curr_track_obj(mcp_handle) \
        GA_mcp_ce_set_object_reference((mcp_handle), (MCP_CE_SET_OBJ_REF_CNF), (GA_CHAR_MCS_CURR_TRACK_OBJ_ID))

/** \} */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/**
 * \name MCP Client APIs - OTP Read
 * \{
 * \brief This section describes the Media Control Profile OTP Read APIs for
 * Client.
 */

/**
 *  \brief To fetch the Object attributes of Current Track.
 *
 *  \par Description:
 *       This function retrieves Current Track Object attribute as
 *       requested.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID.
 *
 *  \param [in] attr_id
 *         Attribute ID to be read.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_GET_OBJ_ATTR_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_curr_track_obj_attr(mcp_handle, obj_id, attr_id) \
        GA_mcp_ce_read_object_attr((mcp_handle), (MCP_CE_GET_OBJ_ATTR_CNF), (obj_id), (attr_id))

/**
 *  \brief To fetch the Current Track Object.
 *
 *  \par Description:
 *       This function fetches Current Track Object.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID on which the Attribute is requested.
 *
 *  \param [in] offset
 *         Offset position to read data from.
 *
 *  \param [in] len
 *         Length of the object to be read.
 *
 *  \return \ref GA_CONTINUE This event will be called again if there are
 *          more contents. \n
 *          \ref GA_SUCCESS indicating completion of contents. \n
 *          Others indicating reason for failure. \n
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_CURR_TRACK_OBJ_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_curr_track_obj(mcp_handle, obj_id, offset, length) \
        GA_mcp_ce_read_object((mcp_handle), (MCP_CE_READ_CURR_TRACK_OBJ_CNF), (MCP_CE_OBJ_TYPE_CURR_TRACK_ID3V2), (obj_id), (offset), (length))
/** \} */
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/**
 * \name MCP Client APIs - Read
 * \{
 * \brief This section describes the Media Control Profile Read APIs for
 * Client.
 */

/**
 *  \brief To Set Object reference of Next Track.
 *
 *  \par Description:
 *       This function reads Next Track Object ID and sets the Obj
 *       reference.
 *       \if MCP_SUPPORT_OBJECT_TRANSFER
 *         If app wants to read the object,
 *         \ref GA_mcp_ce_read_next_track_obj_attr() can be called to
 *         fetch the object attributes.
 *         \ref GA_mcp_ce_read_next_track_obj() can be called with required
 *         object ID, length and offset to fetch the contents.
 *       \endif
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer and the obj is set,
 *          \ref MCP_CE_SET_OBJ_REF_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_set_next_track_obj(mcp_handle) \
        GA_mcp_ce_set_object_reference((mcp_handle), (MCP_CE_SET_OBJ_REF_CNF), (GA_CHAR_MCS_NEXT_TRACK_OBJ_ID))

/** \} */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/**
 * \name MCP Client APIs - OTP Read
 * \{
 * \brief This section describes the Media Control Profile OTP Read APIs for
 * Client.
 */

/**
 *  \brief To fetch the Object attributes of Next Track.
 *
 *  \par Description:
 *       This function retrieves Next Track Object attribute as
 *       requested.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID.
 *
 *  \param [in] attr_id
 *         Attribute ID to be read.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_GET_OBJ_ATTR_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_next_track_obj_attr(mcp_handle, obj_id, attr_id) \
        GA_mcp_ce_read_object_attr((mcp_handle), (MCP_CE_GET_OBJ_ATTR_CNF), (obj_id), (attr_id))

/**
 *  \brief To fetch the Next Track Object.
 *
 *  \par Description:
 *       This function fetches Next Track Object.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID on which the Attribute is requested.
 *
 *  \param [in] offset
 *         Offset position to read data from.
 *
 *  \param [in] len
 *         Length of the object to be read.
 *
 *  \return \ref GA_CONTINUE This event will be called again if there are
 *          more contents \n
 *          \ref GA_SUCCESS indicating completion of contents. \n
 *          Others indicating reason for failure. \n
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_NEXT_TRACK_OBJ_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_next_track_obj(mcp_handle, obj_id, offset, length) \
        GA_mcp_ce_read_object((mcp_handle), (MCP_CE_READ_NEXT_TRACK_OBJ_CNF), (MCP_CE_OBJ_TYPE_NEXT_TRACK_ID3V2), (obj_id), (offset), (length))
/** \} */
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/**
 * \name MCP Client APIs - Read
 * \{
 * \brief This section describes the Media Control Profile Read APIs for
 * Client.
 */

/**
 *  \brief To Set Object reference of Parent Group.
 *
 *  \par Description:
 *       This function reads Parent Group Object ID and sets the Obj
 *       reference.
 *       \if MCP_SUPPORT_OBJECT_TRANSFER
 *         If app wants to read the object,
 *         \ref GA_mcp_ce_read_parent_grp_obj_attr() can be called to
 *         fetch the object attributes.
 *         \ref GA_mcp_ce_read_parent_grp_obj() can be called with required
 *         object ID, length and offset to fetch the contents.
 *       \endif
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer and the obj is set,
 *          \ref MCP_CE_SET_OBJ_REF_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_set_parent_grp_obj(mcp_handle) \
        GA_mcp_ce_set_object_reference((mcp_handle), (MCP_CE_SET_OBJ_REF_CNF), (GA_CHAR_MCS_PARENT_GROUP_OBJ_ID))

/** \} */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/**
 * \name MCP Client APIs - OTP Read
 * \{
 * \brief This section describes the Media Control Profile OTP Read APIs for
 * Client.
 */

/**
 *  \brief To fetch the Object attributes of Parent Group.
 *
 *  \par Description:
 *       This function retrieves Parent Group Object attribute as
 *       requested.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID.
 *
 *  \param [in] attr_id
 *         Attribute ID to be read.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_GET_OBJ_ATTR_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_parent_grp_obj_attr(mcp_handle, obj_id, attr_id) \
        GA_mcp_ce_read_object_attr((mcp_handle), (MCP_CE_GET_OBJ_ATTR_CNF), (obj_id), (attr_id))

/**
 *  \brief To fetch the Parent Group Object.
 *
 *  \par Description:
 *       This function fetches Parent Group Object.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID on which the Attribute is requested.
 *
 *  \param [in] offset
 *         Offset position to read data from.
 *
 *  \param [in] len
 *         Length of the object to be read.
 *
 *  \return \ref GA_CONTINUE This event will be called again if there are
 *          more contents. \n
 *          \ref GA_SUCCESS indicating completion of contents. \n
 *          Others indicating reason for failure. \n
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_PARENT_GROUP_OBJ_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_parent_grp_obj(mcp_handle, obj_id, offset, length) \
        GA_mcp_ce_read_object((mcp_handle), (MCP_CE_READ_PARENT_GROUP_OBJ_CNF), (MCP_CE_OBJ_TYPE_PARENT_GROUP), (obj_id), (offset), (length))
/** \} */
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/**
 * \name MCP Client APIs - Read
 * \{
 * \brief This section describes the Media Control Profile Read APIs for
 * Client.
 */

/**
 *  \brief To Set Object reference of Current Group.
 *
 *  \par Description:
 *       This function reads Current Group Object ID and sets the Obj
 *       reference.
 *       \if MCP_SUPPORT_OBJECT_TRANSFER
 *         If app wants to read the object,
 *         \ref GA_mcp_ce_read_curr_grp_obj_attr() can be called to
 *         fetch the object attributes.
 *         \ref GA_mcp_ce_read_curr_grp_obj() can be called with required
 *         object ID, length and offset to fetch the contents.
 *       \endif
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer and the obj is set,
 *          \ref MCP_CE_SET_OBJ_REF_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_set_curr_grp_obj(mcp_handle) \
        GA_mcp_ce_set_object_reference((mcp_handle), (MCP_CE_SET_OBJ_REF_CNF), (GA_CHAR_MCS_CURR_GROUP_OBJ_ID))

/** \} */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/**
 * \name MCP Client APIs - OTP Read
 * \{
 * \brief This section describes the Media Control Profile OTP Read APIs for
 * Client.
 */

/**
 *  \brief To fetch the Object attributes of Current Group.
 *
 *  \par Description:
 *       This function retrieves Current Group Object attribute as
 *       requested.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID.
 *
 *  \param [in] attr_id
 *         Attribute ID to be read.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_GET_OBJ_ATTR_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_curr_grp_obj_attr(mcp_handle, obj_id, attr_id) \
        GA_mcp_ce_read_object_attr((mcp_handle), (MCP_CE_GET_OBJ_ATTR_CNF), (obj_id), (attr_id))

/**
 *  \brief To fetch the Current Group Object.
 *
 *  \par Description:
 *       This function fetches Current Group Object.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID on which the Attribute is requested.
 *
 *  \param [in] offset
 *         Offset position to read data from.
 *
 *  \param [in] len
 *         Length of the object to be read.
 *
 *  \return \ref GA_CONTINUE This event will be called again if there are
 *          more contents. \n
 *          \ref GA_SUCCESS indicating completion of contents. \n
 *          Others indicating reason for failure. \n
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_CURR_GROUP_OBJ_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_curr_grp_obj(mcp_handle, obj_id, offset, length) \
        GA_mcp_ce_read_object((mcp_handle), (MCP_CE_READ_CURR_GROUP_OBJ_CNF), (MCP_CE_OBJ_TYPE_CURR_GROUP), (obj_id), (offset), (length))
/** \} */
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/**
 * \name MCP Client APIs - Read
 * \{
 * \brief This section describes the Media Control Profile Read APIs for
 * Client.
 */

/**
 *  \brief To Read Playing Order
 *
 *  \par Description:
 *       This function reads Playing Order.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_PLAYING_ORDER_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_playing_order(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_PLAYING_ORDER_CNF), (GA_CHAR_MCS_PLAYING_ORDER))

/**
 *  \brief To Read Playing Orders Supported
 *
 *  \par Description:
 *       This function reads Playing Orders Supported.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_PLAYING_ORDERS_SUPP_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_playing_orders_supp(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_PLAYING_ORDERS_SUPP_CNF), (GA_CHAR_MCS_PLAYING_ORDERS_SUPP))

/**
 *  \brief To Read Media State
 *
 *  \par Description:
 *       This function reads Media State.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_MEDIA_STATE_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_media_state(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_MEDIA_STATE_CNF), (GA_CHAR_MCS_MEDIA_STATE))

/**
 *  \brief To Read MCP Opcodes Supported
 *
 *  \par Description:
 *       This function reads Media State.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_MCP_OPC_SUPP_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_mcp_opc_supp(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_MCP_OPC_SUPP_CNF), (GA_CHAR_MCS_CP_OPC_SUPP))

/**
 *  \brief To Set Object reference of Search Results.
 *
 *  \par Description:
 *       This function reads Search Results Object ID and sets the Obj
 *       reference.
 *       \if MCP_SUPPORT_OBJECT_TRANSFER
 *         If app wants to read the object,
 *         \ref GA_mcp_ce_read_search_res_obj_attr() can be called to
 *         fetch the object attributes.
 *         \ref GA_mcp_ce_read_search_res_obj() can be called with required
 *         object ID, length and offset to fetch the contents.
 *       \endif
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer and the obj is set,
 *          \ref MCP_CE_SET_OBJ_REF_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_set_search_res_obj(mcp_handle) \
        GA_mcp_ce_set_object_reference((mcp_handle), (MCP_CE_SET_OBJ_REF_CNF), (GA_CHAR_MCS_SEARCH_RES_OBJ_ID))

/** \} */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/**
 * \name MCP Client APIs - OTP Read
 * \{
 * \brief This section describes the Media Control Profile OTP Read APIs for
 * Client.
 */

/**
 *  \brief To fetch the Object attributes of Search Result.
 *
 *  \par Description:
 *       This function retrieves Search Result Object attribute as
 *       requested.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID.
 *
 *  \param [in] attr_id
 *         Attribute ID to be read.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_GET_OBJ_ATTR_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_search_res_obj_attr(mcp_handle, obj_id, attr_id) \
        GA_mcp_ce_read_object_attr((mcp_handle), (MCP_CE_GET_OBJ_ATTR_CNF), (obj_id), (attr_id))

/**
 *  \brief To fetch the Search Result Object.
 *
 *  \par Description:
 *       This function fetches Search Result Object.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] obj_id
 *         Object ID on which the Attribute is requested.
 *
 *  \param [in] offset
 *         Offset position to read data from.
 *
 *  \param [in] len
 *         Length of the object to be read.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When contents are read from the peer,
 *          \ref MCP_CE_READ_SEARCH_RES_OBJ_CNF is notified with status
 *          indicating if this event will be called again and again.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_search_res_obj(mcp_handle, obj_id, offset, length) \
        GA_mcp_ce_read_object((mcp_handle), (MCP_CE_READ_SEARCH_RES_OBJ_CNF), (MCP_CE_OBJ_TYPE_SEARCH_RES_GROUP), (obj_id), (offset), (length))

/** \} */
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/**
 * \name MCP Client APIs - Read
 * \{
 * \brief This section describes the Media Control Profile Read APIs for
 * Client.
 */

/**
 *  \brief To Read Content Control ID.
 *
 *  \par Description:
 *       This function reads Content Control ID.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref MCP_CE_READ_CONTENT_CONTROL_ID_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_read_content_control_ID(mcp_handle) \
        GA_mcp_ce_read_request((mcp_handle), (MCP_CE_READ_CONTENT_CONTROL_ID_CNF), (GA_CHAR_CONTENT_CONTROL_ID))

/** \} */

/**
 * \name MCP Client APIs - Write
 * \{
 * \brief This section describes the Media Control Profile Write APIs for
 * Client.
 */

/**
 *  \brief To Write Track Position.
 *
 *  \par Description:
 *       This function writes Track Position.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          \ref MCP_CE_TRACK_POS_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_write_track_pos(mcp_handle, req) \
        GA_mcp_ce_write_request((mcp_handle), (MCP_CE_TRACK_POS_CNF), (GA_CHAR_MCS_TRACK_POS), (req))

/**
 *  \brief To Write Playback Speed.
 *
 *  \par Description:
 *       This function writes Playback Speed.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          \ref MCP_CE_TRACK_PLAYBACK_SPEED_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_write_playback_speed(mcp_handle, req) \
        GA_mcp_ce_write_request((mcp_handle), (MCP_CE_TRACK_PLAYBACK_SPEED_CNF), (GA_CHAR_MCS_PLAYBACK_SPEED), (req))

/**
 *  \brief To Write Current Track Obj ID.
 *
 *  \par Description:
 *       This function writes Current Track Obj ID.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          \ref MCP_CE_CURR_TRACK_OBJ_ID_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_write_curr_track_obj_ID(mcp_handle, req) \
        GA_mcp_ce_write_request((mcp_handle), (MCP_CE_CURR_TRACK_OBJ_ID_CNF), (GA_CHAR_MCS_CURR_TRACK_OBJ_ID), (req))

 /**
 *  \brief To Write Next Track Obj ID.
 *
 *  \par Description:
 *       This function writes Next Track Obj ID.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          \ref MCP_CE_NEXT_TRACK_OBJ_ID_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_write_next_track_obj_ID(mcp_handle, req) \
        GA_mcp_ce_write_request((mcp_handle), (MCP_CE_NEXT_TRACK_OBJ_ID_CNF), (GA_CHAR_MCS_NEXT_TRACK_OBJ_ID), (req))

/**
 *  \brief To Write Current Group Obj ID
 *
 *  \par Description:
 *       This function writes Current Group Obj ID.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          \ref MCP_CE_CURR_GRP_OBJ_ID_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_write_curr_group_obj_ID(mcp_handle, req) \
        GA_mcp_ce_write_request((mcp_handle), (MCP_CE_CURR_GRP_OBJ_ID_CNF), (GA_CHAR_MCS_CURR_GROUP_OBJ_ID), (req))

/**
 *  \brief To Write Playing Order
 *
 *  \par Description:
 *       This function writes Playing Order.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          \ref MCP_CE_PLAYING_ORDER_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_write_playing_order(mcp_handle, req) \
        GA_mcp_ce_write_request((mcp_handle), (MCP_CE_PLAYING_ORDER_CNF), (GA_CHAR_MCS_PLAYING_ORDER), (req))

/**
 *  \brief To Write MCP
 *
 *  \par Description:
 *       This function writes MCP.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          \ref MCP_CE_MCP_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_write_MCP(mcp_handle, req) \
        GA_mcp_ce_write_request((mcp_handle), (MCP_CE_MCP_CNF), (GA_CHAR_MCS_MEDIA_CONTROL_POINT), (req))

/**
 *  \brief To Write SCP
 *
 *  \par Description:
 *       This function writes SCP.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          \ref MCP_CE_SCP_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_ce_write_SCP(mcp_handle, req) \
        GA_mcp_ce_write_request((mcp_handle), (MCP_CE_SCP_CNF), (GA_CHAR_MCS_SEARCH_CONTROL_POINT), (req))

/** \} */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/**
 * \name MCP Client APIs - OTP Setup
 * \{
 * \brief This section describes the Media Control Profile OTP Setup APIs for
 * Client.
 */

/**
 *  \brief To Setup MCP Control Channel.
 *
 *  \par Description:
 *       This function enables to discover if OTS service is included for this
 *       GMCS/MCS session. It also performs char and desc discovery.
 *       If requested for a GMCS handle, MCP Data channel is established along
 *       with the Control channel.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref MCP_CE_CONTROL_CHANNEL_SETUP_CNF is
 *          notified on completion.
 *          For GMCS, If OTS Transport Channel is established,
 *          \ref MCP_CE_DATA_CHANNEL_UP_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_setup_object_ctrl_channel
          (
              /* IN */  MCP_HANDLE mcp_handle
          );

/**
 *  \brief To Setup MCP Data Channel.
 *
 *  \par Description:
 *       This function enables to connect the data channel. In case
 *       the app finds that the usage of data channel is not required in the
 *       current scenario, the channel can be disconnected and
 *       re-connected when required. If in case there is already an existing
 *       data channel up, then this will return a failure.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS,
 *          If MCP Data Channel is established, \ref MCP_CE_DATA_CHANNEL_UP_CNF is
 *          notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_setup_object_data_channel
          (
              /* IN */  MCP_HANDLE mcp_handle
          );
/** \} */

/**
 * \name MCP Client APIs - OTP Release
 * \{
 * \brief This section describes the Media Control Profile OTP Release APIs for
 * Client.
 */

/**
 *  \brief To Release MCP Data Channel
 *
 *  \par Description:
 *       This function enables to release the MCP Data channel whenever
 *       required.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          On successful release, \ref MCP_CE_DATA_CHANNEL_DOWN_CNF is notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_release_object_data_channel
          (
              /* IN */  MCP_HANDLE mcp_handle
          );
/** \} */

/**
 * \name MCP Client APIs - OTP Write
 * \{
 * \brief This section describes the Media Control Profile OTP Write APIs for
 * Client.
 */

/**
 *  \brief To Set Object reference of MCP Char.
 *
 *  \par Description:
 *       This function reads the attribute requested on the current object on
 *       the MCP Control Channel.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcp_ce_exp_evt
 *         Expected event ID to be generated once this operation is performed.
 *
 *  \param [in] obj_id
 *         Object ID on which the Attribute is requested.
 *
 *  \param [in] attr_id
 *         Attribute ID requested to be read.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer, \ref MCP_CE_GET_OBJ_ATTR_CNF is
 *          notified.
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_read_object_attr
          (
              /* IN */  MCP_HANDLE           mcp_handle,
              /* IN */  MCP_CE_EVT_ID        mcp_ce_exp_evt,
              /* IN */  GA_MCP_CE_OBJ_ID   * obj_id,
              /* IN */  MCP_CE_OBJ_ATTR_ID   attr_id
          );

/**
 *  \brief To fetch the contents of the Object.
 *
 *  \par Description:
 *       This function performs transaction to request data via MCP Control
 *       channel and then fetches contents of the Object on the MCP Data Channel.
 *
 *  \param [in] mcp_handle
 *         GMCS/MCS Handle.
 *
 *  \param [in] mcp_ce_exp_evt
 *         Expected event ID to be generated once this operation is performed.
 *
 *  \param [in] mcc_obj_type
 *         Expected type of object.
 *
 *  \param [in] obj_id
 *         Object ID on which the Attribute is requested.
 *
 *  \param [in] offset
 *         Offset position to read data from.
 *
 *  \param [in] len
 *         Length of the object to be read.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer, event is generated.
 *          Eg: \ref MCP_CE_READ_SEARCH_RES_OBJ_CNF
 *
 *  \sa ga_mcp_ce_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_read_object
          (
              /* IN */  MCP_HANDLE        mcp_handle,
              /* IN */  MCP_CE_EVT_ID     mcp_ce_exp_evt,
              /* IN */  MCP_CE_OBJ_TYPE   mcc_obj_type,
              /* IN */  GA_MCP_CE_OBJ_ID * obj_id,
              /* IN */  UINT32            offset,
              /* IN */  UINT32            len
          );
/** \} */

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/**
 * \name MCP Client APIs - De-Init
 * \{
 * \brief This section describes the Media Control Profile DeInit APIs for
 * Client.
 */

/**
 *  \brief To Shutdown the MCP Client Module
 *
 *  \par Description:
 *       This function enables to Shutdown the MCP Client Entity Module.
 *       This function de-references the callback registered with GA MCP
 *       Module.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note No events generated at the MCP_CE layer will be triggered
 *       post this function call.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_ce_shutdown(void);

/** \} */

#ifdef __cplusplus
};
#endif

/** \} */
/** \} */
/** \} */
/** \} */
/** \} */

/** \} */

#endif /* _H_GA_MCP_CE_API_ */
