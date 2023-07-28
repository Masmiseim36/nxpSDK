
/**
 *  \file GA_mcp_se_api.h
 *
 *  \brief This file defines the GA Media Control Profile(MCP)
 *  Server Entity(SE) Interface - includes Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_MCP_SE_API_
#define _H_GA_MCP_SE_API_

/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"
#include "GA_property_config.h"

/**
 * \addtogroup bt_ga
 * \{
 */
/**
 * \addtogroup bt_ga_profiles
 * \{
 */
/**
 * \addtogroup bt_ga_content_control Content Control
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Generic Audio (GA) Content Control Profile module to the Application.
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
 * \defgroup ga_mcp_se_module_def MCP SE (Media Control Profile) Server Entity
 * \{
 * \brief This section describes the defines for MCP SE.
 */

/**
 * \defgroup ga_mcp_se_events Events
 * \{
 * \brief This section lists the Synchronous/Asynchronous Events notified to
 * Application by the Module through the callback \ref MCP_SE_NTF_CB callback.
 */

/**
 * \name MCP Server Events - Read
 * \{
 * \brief The event is notified whenever a read request from peer is received.
 * Once this event is notified, response to the request shall be sent by
 * calling \ref GA_mcp_se_send_rsp().
 * \if MCP_SUPPORT_OBJECT_TRANSFER
 *   If char requires data to be sent over data channel,
 *   \ref MCP_SE_OTP_SE_WRITE_OACP_IND will be triggered,
 *   Post which, \ref GA_mcp_se_OTS_indicate_OACP_rsp() to be called and then
 *   the actual data to be transferred by calling
 *   \ref GA_mcp_se_data_channel_write().
 * \endif
 *
 * If the value of the char exceeds the current MTU Size, then the same event
 * may be generated back to back if a Read Blob request from peer is received.
 * It is necessary to validate and take action: \n
 * If \ref GA_MCP_SE_RSP_CONTEXT is_blob is \ref GA_FALSE, data : NULL, len : 0,
 *     Respond with the read response assuming offset of object content as 0. \n
 * Else if is_blob is \ref GA_TRUE, data : Valid and len : 2,
 *     Indicates that this is a Read Blob Request, derive the offset from
 *     data and use it as a position of the object content that needs to be sent.
 */

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_MEDIA_PLAYER_NAME_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_MEDIA_PLAYER_NAME_IND             0x01U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_MP_ICON_OBJ_ID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_PARAM_OBJ_ID
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_MP_ICON_OBJ_ID_IND                0x02U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_MP_ICON_URL_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UTF8_STR
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_MP_ICON_URL_IND                   0x03U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_TRACK_TITLE_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UTF8_STR
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_TRACK_TITLE_IND                   0x04U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_TRACK_DUR_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_INT32
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_TRACK_DUR_IND                     0x05U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_TRACK_POS_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_INT32
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_TRACK_POS_IND                     0x06U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_PLAYBACK_SPEED_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_INT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_PLAYBACK_SPEED_IND                0x07U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_SEEKING_SPEED_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_INT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_SEEKING_SPEED_IND                 0x08U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_CURR_TRACK_SEG_OBJ_ID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_PARAM_OBJ_ID
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_CURR_TRACK_SEG_OBJ_ID_IND         0x09U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_CURR_TRACK_OBJ_ID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_PARAM_OBJ_ID
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_CURR_TRACK_OBJ_ID_IND             0x0AU

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_NEXT_TRACK_OBJ_ID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_PARAM_OBJ_ID
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_NEXT_TRACK_OBJ_ID_IND             0x0BU

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_PARENT_GROUP_OBJ_ID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_PARAM_OBJ_ID
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_PARENT_GROUP_OBJ_ID_IND           0x0CU

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_CURR_GROUP_OBJ_ID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_PARAM_OBJ_ID
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_CURR_GROUP_OBJ_ID_IND             0x0DU

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_PLAYING_ORDER_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_PLAYING_ORDER_IND                 0x0EU

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_PLAYING_ORDER_SUPP_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT16
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_PLAYING_ORDER_SUPP_IND            0x0FU

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_MEDIA_STATE_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_MEDIA_STATE_IND                   0x10U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_MCP_OPC_SUPP_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT32
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_MCP_OPC_SUPP_IND                  0x11U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_SEARCH_RES_OBJ_ID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_PARAM_OBJ_ID
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_SEARCH_RES_OBJ_ID_IND             0x12U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_READ_CCID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 2, \n
 *                                   - data - \ref UINT16 - Offset \n
 *                                   - len - \ref sizeof( \ref UINT16) -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_READ_CCID_IND                          0x13U

/** \} */

/**
 * \name MCP Server Events - Write
 * \{
 * \brief The event is notified whenever a write request from peer is received.
 * Once this event is notified, response to the request shall be sent by
 * calling \ref GA_mcp_se_send_rsp() if \ref GA_MCP_SE_RSP_CONTEXT to_rsp
 * is set to \ref GA_TRUE. The setting of to_rsp depends on the type of write
 * requested by peer, Write Command or Write Request.
 */

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_WRITE_TRACK_POS_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref MCP_CHAR_PARAM_TYPE_INT32 - Track Position \n
 *                               - len - \ref sizeof( \ref MCP_CHAR_PARAM_TYPE_INT32 ) -
 *                                       Length of Track Position \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_WRITE_TRACK_POS_IND                    0x14U

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_WRITE_TRACK_PLAYBACK_SPEED_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref MCP_CHAR_PARAM_TYPE_INT8 - Playback Speed \n
 *                               - len - \ref sizeof( \ref MCP_CHAR_PARAM_TYPE_INT8 ) -
 *                                       Length of Playback Speed \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_WRITE_TRACK_PLAYBACK_SPEED_IND         0x15U

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_WRITE_CURR_TRACK_OBJ_ID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT8 - Pointer to Obj ID \n
 *                               - len - \ref MCP_OBJ_ID_LEN -
 *                                       Object ID Length \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_WRITE_CURR_TRACK_OBJ_ID_IND            0x16U

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_WRITE_NEXT_TRACK_OBJ_ID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT8 - Pointer to Obj ID \n
 *                               - len - \ref MCP_OBJ_ID_LEN -
 *                                       Object ID Length \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_WRITE_NEXT_TRACK_OBJ_ID_IND            0x17U

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_WRITE_CURR_GRP_OBJ_ID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT8 - Pointer to Obj ID \n
 *                               - len - \ref MCP_OBJ_ID_LEN -
 *                                       Object ID Length \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_WRITE_CURR_GRP_OBJ_ID_IND              0x18U

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_WRITE_PLAYING_ORDER_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref MCP_CHAR_PARAM_TYPE_UINT8 - Playing Order \n
 *                               - len - \ref sizeof( \ref MCP_CHAR_PARAM_TYPE_UINT8) -
 *                                       Length of Playing Order \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_WRITE_PLAYING_ORDER_IND                0x19U

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_WRITE_MCP_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - Pointer to a \ref UCHAR array where \n
 *                                 First Byte refers to \ref ga_mcp_se_constants . \n
 *                                 Based on the Requested Opcode,
 *                                 the following Bytes can be decoded. \n
 *                                 Opcodes \ref ga_mcp_se_constants with no
 *                                 parameters: \n
 *                                 \ref MCP_CP_OPCODE_PLAY \n
 *                                 \ref MCP_CP_OPCODE_PAUSE \n
 *                                 \ref MCP_CP_OPCODE_FAST_RWD \n
 *                                 \ref MCP_CP_OPCODE_FAST_FWD \n
 *                                 \ref MCP_CP_OPCODE_STOP \n
 *                                 \ref MCP_CP_OPCODE_PREV_SEG \n
 *                                 \ref MCP_CP_OPCODE_NEXT_SEG \n
 *                                 \ref MCP_CP_OPCODE_FIRST_SEG \n
 *                                 \ref MCP_CP_OPCODE_LAST_SEG \n
 *                                 \ref MCP_CP_OPCODE_PREV_TRACK \n
 *                                 \ref MCP_CP_OPCODE_NEXT_TRACK \n
 *                                 \ref MCP_CP_OPCODE_FIRST_TRACK \n
 *                                 \ref MCP_CP_OPCODE_LAST_TRACK \n
 *                                 \ref MCP_CP_OPCODE_PREV_GROUP \n
 *                                 \ref MCP_CP_OPCODE_NEXT_GROUP \n
 *                                 \ref MCP_CP_OPCODE_FIRST_GROUP \n
 *                                 \ref MCP_CP_OPCODE_LAST_GROUP \n
 *                                 For below Opcodes, there are parameters: \ref INT32 \n
 *                                 \ref MCP_CP_OPCODE_MOVE_RELATIVE -
 *                                                                  Offset \n
 *                                 \ref MCP_CP_OPCODE_GOTO_SEG -
 *                                                             nth seg \n
 *                                 \ref MCP_CP_OPCODE_GOTO_TRACK -
 *                                                               nth track \n
 *                                 \ref MCP_CP_OPCODE_GOTO_GROUP -
 *                                                               nth group \n
 *                               - len - Length as per above \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_WRITE_MCP_IND                          0x1AU

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_WRITE_SCP_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref - \n
 *                               - len - \ref
 *                                       Length of \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_WRITE_SCP_IND                          0x1BU

/** \} */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/**
 * \name MCP Server Events - OTP Read
 * \{
 * \brief This event is notified when a read request if received for OTP char.
 * Response to this request has to be sent by calling
 * \ref GA_mcp_se_send_rsp().
 */

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_READ_OTS_FEATURE_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 4, \n
 *                                   - data - \ref UINT32 - Pointer to a \ref UINT32 \n
 *                                   - len -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_READ_OTS_FEATURE_IND            0x1CU

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_READ_OBJ_NAME_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 4, \n
 *                                   - data - \ref UINT32 - Pointer to a \ref UINT32 \n
 *                                   - len -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_READ_OBJ_NAME_IND               0x1DU

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_READ_OBJ_TYPE_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 4, \n
 *                                   - data - \ref UINT32 - Pointer to a \ref UINT32 \n
 *                                   - len -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_READ_OBJ_TYPE_IND               0x1EU

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_READ_OBJ_SIZE_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 4, \n
 *                                   - data - \ref UINT32 - Pointer to a \ref UINT32 \n
 *                                   - len -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_READ_OBJ_SIZE_IND               0x1FU

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_READ_OBJ_FIRST_CREATED_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 4, \n
 *                                   - data - \ref UINT32 - Pointer to a \ref UINT32 \n
 *                                   - len -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_READ_OBJ_FIRST_CREATED_IND      0x20U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_READ_OBJ_LAST_MOD_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 4, \n
 *                                   - data - \ref UINT32 - Pointer to a \ref UINT32 \n
 *                                   - len -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_READ_OBJ_LAST_MOD_IND           0x21U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_READ_OBJ_ID_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 4, \n
 *                                   - data - \ref UINT32 - Pointer to a \ref UINT32 \n
 *                                   - len -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_READ_OBJ_ID_IND                 0x22U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_READ_OBJ_PROP_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 4, \n
 *                                   - data - \ref UINT32 - Pointer to a \ref UINT32 \n
 *                                   - len -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_READ_OBJ_PROP_IND               0x23U

/**
 * This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_READ_OBJ_FILTER_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - If \ref GA_MCP_SE_RSP_CONTEXT is_blob is
 *                                   \ref GA_TRUE, data is Valid and
 *                                   len is 4, \n
 *                                   - data - \ref UINT32 - Pointer to a \ref UINT32 \n
 *                                   - len -
 *                                     Offset Length \n
 *                                   .
 *                               - Else, \n
 *                                   - data - NULL, Ignore \n
 *                                   - len - 0, Ignore \n
 *                                   .
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref MCP_CHAR_PARAM_TYPE_UINT8
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_READ_OBJ_FILTER_IND             0x24U

/** \} */

/**
 * \name MCP Server Events - OTP Write
 * \{
 * \brief The event is notified whenever a write request from peer is received.
 * Once this event is notified, response to the request shall be sent by
 * calling \ref GA_mcp_se_send_rsp() if \ref GA_MCP_SE_RSP_CONTEXT to_rsp
 * is set to \ref GA_TRUE. The setting of to_rsp depends on the type of write
 * requested by peer, Write Command or Write Request.
 */

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_WRITE_OBJ_NAME_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT8 - Pointer to Param \n
 *                               - len - \ref Length of the parameter \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_WRITE_OBJ_NAME_IND              0x25U

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_WRITE_OBJ_FIRST_CREATED_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT8 - Pointer to Param \n
 *                               - len - \ref Length of the parameter \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_WRITE_OBJ_FIRST_CREATED_IND     0x26U

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_WRITE_OBJ_LAST_MOD_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT8 - Pointer to Param \n
 *                               - len - \ref Length of the parameter \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_WRITE_OBJ_LAST_MOD_IND          0x27U

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_WRITE_OBJ_PROP_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT8 - Pointer to Param \n
 *                               - len - \ref Length of the parameter \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_WRITE_OBJ_PROP_IND              0x28U

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_WRITE_OACP_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT8 - Pointer to Param \n
 *                               - len - \ref Length of the parameter \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_WRITE_OACP_IND                  0x29U

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_WRITE_OLCP_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT8 - Pointer to Param \n
 *                               - len - \ref Length of the parameter \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_WRITE_OLCP_IND                  0x2AU

/**
 * This event is notified when a write request is received for a char from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_OTP_SE_WRITE_OBJ_FILTER_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT8 - Pointer to Param \n
 *                               - len - \ref Length of the parameter \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_mcp_se_send_rsp()
 */
#define MCP_SE_OTP_SE_WRITE_OBJ_FILTER_IND            0x2BU

/** \} */

/**
 * \name MCP Server Events - OTP Setup
 * \{
 * \brief This event is notified when transaction occurs on the Data Channel.
 */

/**
 * This event is notified when a Data Channel is established from
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_DATA_CHANNEL_UP_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Data Channel established \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT16 - Pointer to Channel ID \n
 *                               - len - \ref sizeof(\ref UINT16) Length of the Channel ID \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_SE_DATA_CHANNEL_UP_IND                    0x2CU

/**
 * This event is notified when a Data Channel is disconnected with the
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_DATA_CHANNEL_DOWN_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Data Channel disconnected \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT16 - Pointer to Channel ID \n
 *                               - len - \ref sizeof(\ref UINT16) Length of the Channel ID \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_SE_DATA_CHANNEL_DOWN_IND                  0x2DU

/**
 * This event is notified when a Data Channel contents are written to the
 * peer device with the following values as parameters in the
 * \ref MCP_SE_NTF_CB callback. This is called once \ref
 * GA_mcp_se_data_channel_write() is called to write object contents to peer.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_MCP_SE_RSP_CONTEXT
 * \param [in] evt_id  \ref MCP_SE_DATA_CHANNEL_WRITE_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Data Channel disconnected \n
 * \param [in] evt_data  Pointer to object of type \ref MCP_SE_EVT \n
 *                         MCP Handle for the Device - \ref MCP_HANDLE \n
 *                           - If evt_status is \ref GA_SUCCESS : \n
 *                               - data - \ref UINT16 - Pointer to Number of Bytes written \n
 *                               - len - \ref sizeof(\ref UINT16) \n
 *                               .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define MCP_SE_DATA_CHANNEL_WRITE_IND                 0x2EU

/** \} */

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/** \} */

/**
 * \defgroup ga_mcp_se_constants Constants
 * \{
 * \brief Describes Constants defined by the module.
 */

/**
 * \name MCP Server Constants - General Macros
 * \{
 * \brief This section lists general macros used.
 */

/** By default, GMCS Session has to be supported */
#define MCP_SE_GMCS_SER_ENTITIES             1U

/** Maximum instances - GMCS + MCS Entities */
#define MCP_SE_MAX_MCS_SER_ENTITIES    (MCP_SE_GMCS_SER_ENTITIES + GA_CONFIG_LIMITS(MCP_MAX_MCS_ENTITIES))

/** Represents the session ID for a GMCS instance MCP_SE_HANDLE */
#define MCP_SE_GMCS_HANDLE                   0x00U

/** Length of an Object ID as per OTS */
#define MCP_OBJ_ID_LEN                       6U

/** \} */

/**
 * \name MCP Server Constants - Default Char Values
 * \{
 * \brief This section lists the default values to be used as defined per MCS
 * specification.
 */

/**
 * Track Value: As defined in MCS Specification.
 * Track Duration: No Current Track or Duration of current track is Unknown.
 * Represented as \ref MCP_CHAR_PARAM_TYPE_INT32.
 * Track Position: If start of the track is not well-defined(live stream),
 * can use below value or use 0. Represented as \ref MCP_CHAR_PARAM_TYPE_INT32.
 */
#define MCP_DEFAULT_TRACK_VALUE                 0xFFFFFFFFU

/**
 * Track Title Value: As defined in MCS Specification.
 * No Current Track or Track Title is unavailable, it is a zero-length string.
 * Otherwise, it is a UTF-8 string whose length should not exceed (ATT_MTU - 3)
 * octets.
 */
#define MCP_DEFAULT_UTF8_STR_LEN                0x0U

/**
 * As defined in MCS Specification: For streaming media, playback speed is
 * fixed to 1x normal speed.
 */
#define MCP_DEFAULT_PLAYBACK_SPEED              0x0U

/**
 * As defined in MCP Specification: When there is no seeking -
 * the value is set to 0.
 */
#define MCP_DEFAULT_SEEKING_SPEED               0x0U

/**
 * As defined in MCS Specification:
 * When there is no media player icon, current track, next track or no
 * created parent group or current group. When there is no current track,
 * current track segment Object value becomes 0.
 */
#define MCP_DEFAULT_OBJ_ID_LEN                  0x0U

/** \} */

/**
 * \name MCP Server Constants - Playing Order
 * \{
 * \brief This section lists Playing Order Values,
 * As defined in MCS Specification.
 * \ref MCP_CHAR_PARAM_TYPE_UINT8
 */

/** Playing Orders values - Single once */
#define MCP_PLAYING_ORDER_SINGLE_ONCE            0x01U
/** Playing Orders values - Single repeat */
#define MCP_PLAYING_ORDER_SINGLE_REPEAT          0x02U
/** Playing Orders values - In order once */
#define MCP_PLAYING_ORDER_SINGLE_IN_ORDER_ONCE   0x03U
/** Playing Orders values - In order repeat */
#define MCP_PLAYING_ORDER_SINGLE_IN_ORDER_REPEAT 0x04U
/** Playing Orders values - Oldest once */
#define MCP_PLAYING_ORDER_SINGLE_OLDEST_ONCE     0x05U
/** Playing Orders values -  Oldest repeat */
#define MCP_PLAYING_ORDER_SINGLE_OLDEST_REPEAT   0x06U
/** Playing Orders values - Newest once */
#define MCP_PLAYING_ORDER_SINGLE_NEWEST_ONCE     0x07U
/** Playing Orders values - Newest repeat */
#define MCP_PLAYING_ORDER_SINGLE_NEWEST_REPEAT   0x08U
/** Playing Orders values - Shuffle once */
#define MCP_PLAYING_ORDER_SINGLE_SHUFFLE_ONCE    0x09U
/** Playing Orders values - Shuffle repeat */
#define MCP_PLAYING_ORDER_SINGLE_SHUFFLE_REPEAT  0x0AU

/** \} */

/**
 * \name MCP Server Constants - Playing Order Supported
 * \{
 * \brief This section lists Playing Order Supported Values,
 * As defined in MCS Specification.
 * \ref MCP_CHAR_PARAM_TYPE_UINT16
 */

/** Playing Orders Supported values - Single once */
#define MCP_PLAYING_ORDER_SUPP_SINGLE_ONCE                 0x0001U
/** Playing Orders Supported values - Single repeat */
#define MCP_PLAYING_ORDER_SUPP_SINGLE_REPEAT               0x0002U
/** Playing Orders Supported values - In order once */
#define MCP_PLAYING_ORDER_SUPP_IN_ORDER_ONCE               0x0004U
/** Playing Orders Supported values - In order repeat */
#define MCP_PLAYING_ORDER_SUPP_IN_ORDER_REPEAT             0x0008U
/** Playing Orders Supported values - Oldest once */
#define MCP_PLAYING_ORDER_SUPP_OLDEST_ONCE                 0x0010U
/** Playing Orders Supported values -  Oldest repeat */
#define MCP_PLAYING_ORDER_SUPP_OLDEST_REPEAT               0x0020U
/** Playing Orders Supported values - Newest once */
#define MCP_PLAYING_ORDER_SUPP_NEWEST_ONCE                 0x0040U
/** Playing Orders Supported values - Newest repeat */
#define MCP_PLAYING_ORDER_SUPP_NEWEST_REPEAT               0x0080U
/** Playing Orders Supported values - Shuffle once */
#define MCP_PLAYING_ORDER_SUPP_SHUFFLE_ONCE                0x0100U
/** Playing Orders Supported values - Shuffle repeat */
#define MCP_PLAYING_ORDER_SUPP_SHUFFLE_REPEAT              0x0200U

/** \} */

/**
 * \name MCP Server Constants - Media State
 * \{
 * \brief This section lists Media State Opcode Values,
 * As defined in MCS Specification.
 * \ref MCP_CHAR_PARAM_TYPE_UINT8
 */

/** The current track is invalid, and no track has been selected */
#define MCP_MEDIA_STATE_INACTIVE                           0x00U

/** The media player is playing the current track */
#define MCP_MEDIA_STATE_PLAYING                            0x01U

/**
 * The current track is paused. The media player has a current track, but is
 * not playing.
 */
#define MCP_MEDIA_STATE_PAUSED                             0x02U

/** The current track is fast forwarding or fast rewinding. */
#define MCP_MEDIA_STATE_SEEKING                            0x03U

/** \} */

/**
 * \name MCP Server Constants - Request Opcode
 * \{
 * \brief This section lists MCP Opcodes, As defined in MCS Specification.
 * \ref MCP_CP_REQ_OPC
 */

/** MCP Request Opcode - Play */
#define MCP_CP_OPCODE_PLAY                           0x01U
/** MCP Request Opcode - Pause */
#define MCP_CP_OPCODE_PAUSE                          0x02U
/** MCP Request Opcode - Fast Rewind */
#define MCP_CP_OPCODE_FAST_RWD                       0x03U
/** MCP Request Opcode - Fast Forward */
#define MCP_CP_OPCODE_FAST_FWD                       0x04U
/** MCP Request Opcode - Stop */
#define MCP_CP_OPCODE_STOP                           0x05U
/** MCP Request Opcode - Move Relative */
#define MCP_CP_OPCODE_MOVE_RELATIVE                  0x10U
/** MCP Request Opcode - Previous Segment */
#define MCP_CP_OPCODE_PREV_SEG                       0x20U
/** MCP Request Opcode - Next Segment */
#define MCP_CP_OPCODE_NEXT_SEG                       0x21U
/** MCP Request Opcode - First Segment */
#define MCP_CP_OPCODE_FIRST_SEG                      0x22U
/** MCP Request Opcode - Last Segment */
#define MCP_CP_OPCODE_LAST_SEG                       0x23U
/** MCP Request Opcode - Goto Segment */
#define MCP_CP_OPCODE_GOTO_SEG                       0x24U
/** MCP Request Opcode - Previous Track */
#define MCP_CP_OPCODE_PREV_TRACK                     0x30U
/** MCP Request Opcode - Next Track */
#define MCP_CP_OPCODE_NEXT_TRACK                     0x31U
/** MCP Request Opcode - First Track */
#define MCP_CP_OPCODE_FIRST_TRACK                    0x32U
/** MCP Request Opcode - Last Track */
#define MCP_CP_OPCODE_LAST_TRACK                     0x33U
/** MCP Request Opcode - Goto Track */
#define MCP_CP_OPCODE_GOTO_TRACK                     0x34U
/** MCP Request Opcode - Previous Group */
#define MCP_CP_OPCODE_PREV_GROUP                     0x40U
/** MCP Request Opcode - Next Group */
#define MCP_CP_OPCODE_NEXT_GROUP                     0x41U
/** MCP Request Opcode - First Group */
#define MCP_CP_OPCODE_FIRST_GROUP                    0x42U
/** MCP Request Opcode - Last Group */
#define MCP_CP_OPCODE_LAST_GROUP                     0x43U
/** MCP Request Opcode - Goto Group */
#define MCP_CP_OPCODE_GOTO_GROUP                     0x44U

/** \} */

/**
 * \name MCP Server Constants - Opcode Supported
 * \{
 * \brief This section lists MCP Supported Opcodes, As defined in MCS
 * Specification.
 * \ref MCP_CHAR_PARAM_TYPE_UINT32
 */

/** MCP Opcode Supported - Play */
#define MCP_CP_OPC_SUPP_PLAY                  0x00000001U
/** MCP Opcode Supported - Pause */
#define MCP_CP_OPC_SUPP_PAUSE                 0x00000002U
/** MCP Opcode Supported - Fast Rewind */
#define MCP_CP_OPC_SUPP_FAST_REW              0x00000004U
/** MCP Opcode Supported - Fast Forward */
#define MCP_CP_OPC_SUPP_FAST_FWD              0x00000008U
/** MCP Opcode Supported - Stop */
#define MCP_CP_OPC_SUPP_STOP                  0x00000010U
/** MCP Opcode Supported - Move Relative */
#define MCP_CP_OPC_SUPP_MOVE_RELATIVE         0x00000020U
/** MCP Opcode Supported - Previous Segment */
#define MCP_CP_OPC_SUPP_PREV_SEG              0x00000040U
/** MCP Opcode Supported - Next Segment */
#define MCP_CP_OPC_SUPP_NEXT_SEG              0x00000080U
/** MCP Opcode Supported - First Segment */
#define MCP_CP_OPC_SUPP_FIRST_SEG             0x00000100U
/** MCP Opcode Supported - Last Segment */
#define MCP_CP_OPC_SUPP_LAST_SEG              0x00000200U
/** MCP Opcode Supported - Goto Segment */
#define MCP_CP_OPC_SUPP_GOTO_SEG              0x00000400U
/** MCP Opcode Supported - Previous Track */
#define MCP_CP_OPC_SUPP_PREV_TRACK            0x00000800U
/** MCP Opcode Supported - Next Track */
#define MCP_CP_OPC_SUPP_NEXT_TRACK            0x00001000U
/** MCP Opcode Supported - First Track */
#define MCP_CP_OPC_SUPP_FIRST_TRACK           0x00002000U
/** MCP Opcode Supported - Last Track */
#define MCP_CP_OPC_SUPP_LAST_TRACK            0x00004000U
/** MCP Opcode Supported - Goto Track */
#define MCP_CP_OPC_SUPP_GOTO_TRACK            0x00008000U
/** MCP Opcode Supported - Previous Group */
#define MCP_CP_OPC_SUPP_PREV_GRP              0x00010000U
/** MCP Opcode Supported - Next Group */
#define MCP_CP_OPC_SUPP_NEXT_GRP              0x00020000U
/** MCP Opcode Supported - First Group */
#define MCP_CP_OPC_SUPP_FIRST_GRP             0x00040000U
/** MCP Opcode Supported - Last Group */
#define MCP_CP_OPC_SUPP_LAST_GRP              0x00080000U
/** MCP Opcode Supported - Goto Group */
#define MCP_CP_OPC_SUPP_GOTO_GRP              0x00100000U

/** \} */

/**
 * \name MCP Server Constants - CP Result Code
 * \{
 * \brief This section lists the result codes for Media Control Point
 * Notification.
 * As defined in MCS Specification.
 * \ref MCP_CP_RES_CODE
 */

/** MCP Notification Result Code - Success */
#define MCP_CP_RES_CODE_SUCCESS                    0x01U
/** MCP Notification Result Code - Opcode Not Supported */
#define MCP_CP_RES_CODE_OPCODE_NOT_SUPP            0x02U
/** MCP Notification Result Code - Media Player Inactive */
#define MCP_CP_RES_CODE_MP_INACTIVE                0x03U
/** MCP Notification Result Code - Command Cannot Be Completed */
#define MCP_CP_RES_CODE_CMD_CANNOT_BE_COMPLETED    0x04U

/** \} */

/**
 * \name MCP Server Constants - SCP Type
 * \{
 * \brief This section lists the SCP Types used.
 * As defined in MCS Specification.
 * \ref MCP_SCP_TYPE
 */

/** SCP Type - Track Name */
#define MCP_SCP_TYPE_TRACK_NAME               0x01U
/** SCP Type - Artist Name */
#define MCP_SCP_TYPE_ARTIST_NAME              0x02U
/** SCP Type - Album Name */
#define MCP_SCP_TYPE_ALBUM_NAME               0x03U
/** SCP Type - Group Name */
#define MCP_SCP_TYPE_GROUP_NAME               0x04U
/** SCP Type - Earliest Year */
#define MCP_SCP_TYPE_EARLIEST_YEAR            0x05U
/** SCP Type - Latest Year */
#define MCP_SCP_TYPE_LATEST_YEAR              0x06U
/** SCP Type - Genre */
#define MCP_SCP_TYPE_GENRE                    0x07U
/** SCP Type - Tracks */
#define MCP_SCP_TYPE_ONLY_TRACKS              0x08U
/** SCP Type - Groups */
#define MCP_SCP_TYPE_ONLY_GROUPS              0x09U

/** \} */

/**
 * \name MCP Server Constants - SCP Result Code
 * \{
 * \brief This section lists the result codes for Search Control Point
 * Notification.
 * As defined in MCS Specification.
 * \ref MCP_SCP_RES_CODE
 */

/** Search request was accepted; search has started */
#define MCP_SCP_RES_CODE_SUCCESS                  0x01U

/** Search request was invalid; search start failed */
#define MCP_SCP_RES_CODE_FAILURE                  0x02U

/** \} */

/**
 * \name MCP Server Constants - Group Object Type
 * \{
 * \brief This section lists the Group Object Type.
 * As defined in MCS Specification.
 * \ref MCP_GROUP_OBJ_TYPE
 */

/** Object Type - Track */
#define MCP_SE_GRP_OBJ_TYPE_TRACK             0x0U
/** Object Type - Group*/
#define MCP_SE_GRP_OBJ_TYPE_GROUP             0x1U

/** \} */

/** \} */

/* --------------------------------------------- Data types /Structures */

/**
 * \defgroup ga_mcp_se_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by MCP SE.
 */

/**
 * This identifier refers to the registered GMCS/MCS instance.
 * The application needs to provide this as a reference for all interface calls
 * to have transaction on the required GMCS/MCS instance.
 * By default, \ref MCP_SE_HANDLE is \ref MCP_SE_GMCS_HANDLE for a GMCS
 * instance.
 */
typedef UINT8 MCP_SE_HANDLE;

/**
 * Event that will be notified by the MCP SE module.
 */
typedef UINT8 MCP_SE_EVT_ID;

/**
 * \defgroup ga_mcp_se_char_param_type_typedef Char Param Type
 * \{
 * \brief DataType to be used during sending response to read requests or while
 * sending notifications.
 */

/**
 * Used for Media Player Name, Media Player Icon URL, Track Title.
 * 1. \ref GA_mcp_se_notify_media_player_name(), \ref GA_mcp_se_notify_track_title()
 *    OR
 *    while responding to event \ref MCP_SE_READ_MEDIA_PLAYER_NAME_IND,
 *    \ref MCP_SE_READ_MP_ICON_OBJ_ID_IND, \ref MCP_SE_READ_TRACK_TITLE_IND
 *    by calling \ref GA_mcp_se_send_rsp().
 * 2. If the media player has no current track or the track title is unavailable,
 *    the Track Title characteristic value shall be a zero-length string.
 * 3. If the characteristic value is longer than (ATT_MTU-3), then the first
 *    (ATT_MTU-3) octets shall be included in the notification
 */
typedef UCHAR MCP_CHAR_PARAM_TYPE_UTF8_STR;

/**
 * Used for Track Duration, Track Position.
 * 1. \ref GA_mcp_se_notify_track_duration(), \ref GA_mcp_se_notify_track_pos()
 *    OR
 *    while responding to event \ref MCP_SE_READ_MEDIA_PLAYER_NAME_IND,
 *    \ref MCP_SE_READ_TRACK_DUR_IND, \ref MCP_SE_READ_TRACK_POS_IND
 *    by calling \ref GA_mcp_se_send_rsp().
 * 2. The Track Duration characteristic exposes the length of the current track
 *    in 0.01-second resolution as a 32-bit signed integer. If the media
 *    player has no current track or the duration of the current track is
 *    unknown, the Track Duration characteristic value shall be 0xFFFFFFFF.
 *    Otherwise, the duration of the track shall be zero or greater.
 * 3. The Track Position characteristic exposes the current track position of
 *    the current track in 0.01-second resolution as a 32-bit signed integer.
 *    The Track Position characteristic exposes the offset from the start of
 *    the track to the current playing position. If the start of the track is
 *    not well defined (such as in a livestream), the server sets a starting
 *    position (where Track Position equals 0) or sets the value to
 *    UNAVAILABLE (0xFFFFFFFF). If the media player has no current track, the
 *    Track Position characteristic value shall be 0xFFFFFFFF.
 */
typedef INT32 MCP_CHAR_PARAM_TYPE_INT32;

/**
 * Used for Playback Speed, Seeking Speed.
 * 1. \ref GA_mcp_se_notify_playback_speed(), \ref GA_mcp_se_notify_seeking_speed()
 *    OR
 *    while responding to event \ref MCP_SE_READ_PLAYBACK_SPEED_IND,
 *    \ref MCP_SE_READ_SEEKING_SPEED_IND.
 *    by calling \ref GA_mcp_se_send_rsp().
 * 2. The Playback Speed characteristic exposes the current track playback speed.
 *    Playback Speed Value(p) indicates the Resultant Multiple of Playback Speed(s).
 *    s = 2^(p/64)
 *
 *    'p'  : 's' \n
 *    -128 : .25 \n
 *    -64  : .50 \n
 *     0   :  1 \n
 *     64  :  2 \n
 *    127  :  3.957(4) \n
 *    If the value written is not supported and is greater than the existing
 *    Playback Speed characteristic, then the server should set the Playback
 *    Speed characteristic to the next higher supported playback speed.
 *    If the value written is not supported and is less than the existing
 *    Playback Speed characteristic, then the server should set the Playback
 *    Speed characteristic to the next lower supported playback speed.
 *    For streaming media, the playback speed may be fixed to 1x normal speed.
 * 3. The Seeking Speed characteristic exposes the seeking speed of the
 *    current track. It is the current multiple of the real-time playback speed
 *    at which the track is being fast forwarded or fast rewound.
 *    (Real-time is the point at which p=0 in the Playback Speed
 *    characteristic calculation.)
 *    Fast forward is a positive current track-seeking speed.
 *    Fast rewind is a negative current track-seeking speed.
 *    If the media player is not seeking, the value of this characteristic
 *    shall be 0.
 */
typedef INT8 MCP_CHAR_PARAM_TYPE_INT8;

/**
 * Used for Playback Order, Media State.
 * 1. \ref GA_mcp_se_notify_playing_order(), GA_mcp_se_notify_media_state()
 *    OR
 *    while responding to event \ref MCP_SE_READ_PLAYING_ORDER_IND,
 *    \ref MCP_SE_READ_MEDIA_STATE_IND by calling \ref GA_mcp_se_send_rsp().
 * 2. The overall playing order is left up to the media player implementation.
 *    For example, if the end of a group is reached, the media player may
 *    continue to play from another group.
 * 3. It is possible for the media player to be in the Playing state without a
 *    related audio stream, or in the Inactive and Paused state with a related
 *    audio stream. A media player should manage the state of the associated
 *    audio stream to match the state of the media player, but this
 *    relationship is not defined within this service and is left to the
 *    implementation.
 */
typedef UINT8 MCP_CHAR_PARAM_TYPE_UINT8;

/**
 * Used for Playback Orders Supported.
 * 1. While responding to event \ref MCP_SE_READ_PLAYING_ORDER_SUPP_IND
 *    by calling \ref GA_mcp_se_send_rsp().
 * 2. The overall playing order is left up to the media player implementation.
 *    For example, if the end of a group is reached, the media player may
 *    continue to play from another group.
 * 3. Bit field values,
 *    If more than one order is supported, use Bitwise OR of all supported orders.
 *
 * Eg: \ref MCP_PLAYING_ORDER_SUPP_SINGLE_ONCE
 */
typedef UINT16 MCP_CHAR_PARAM_TYPE_UINT16;

/**
 * Used for MCP Opcodes Supported.
 * 1. While responding to event \ref MCP_SE_READ_MCP_OPC_SUPP_IND
 *    by calling \ref GA_mcp_se_send_rsp().
 * 2. The Media Control Point Opcodes Supported characteristic exposes the
 *    currently supported media control opcodes on the server.
 *    The value of this characteristic is a bit field of 32 bits, where bit n
 *    is set to 1 when opcode n is supported; otherwise, the bit is set to 0.
 */
typedef UINT32 MCP_CHAR_PARAM_TYPE_UINT32;

/** Represents the req opcode on which Media Control Point proc is requested */
typedef UINT8 MCP_CP_REQ_OPC;

/** Result code for the requested Media Control Point proc */
typedef UINT8 MCP_CP_RES_CODE;

/** SCP Type used in Search CP procedure */
typedef UINT8 MCP_SCP_TYPE;

/** SCP Result as response for SCP proc */
typedef UINT8 MCP_SCP_RES_CODE;

/** MCP Group Object Type */
typedef UINT8 MCP_GROUP_OBJ_TYPE;

/** \} */

/** Context to identify the request and response */
typedef GA_BRR_CHR_CONTEXT GA_MCP_SE_RSP_CONTEXT;

/** MCP response format */
typedef struct _MCP_PARAM_MCP_RSP
{
    /** Requested Opcode */
    MCP_CP_REQ_OPC   req_opc;

    /** Result Code for requested Opcode */
    MCP_CP_RES_CODE  result;

}MCP_PARAM_MCP_RSP;

/**
 * UINT48 length representing an Object ID -
 * Used for Media Player Icon/Track/Group/Current Track Segment Object.
 */
typedef struct _MCP_PARAM_OBJ_ID
{
    /** Object ID */
    UINT8 mcp_obj_id[MCP_OBJ_ID_LEN];

}MCP_PARAM_OBJ_ID;

/** MCP SE Event */
typedef struct _MCP_SE_EVT_
{
    /** Reference Identifier to GMCS/MCS instance */
    MCP_SE_HANDLE   mcp_se_handle;

    /** Data associated with the event */
    void          * data;

    /** Length of the event data */
    UINT16          len;

}MCP_SE_EVT;

/** MCP SE Object Type - Media Player Icon */
typedef struct _MCP_SE_MP_ICON_OBJ_
{
    /** Represented as .png format */
    void   * icon_png_data;

    /** Total Object length */
    UINT16   total_obj_len;

}MCP_SE_MP_ICON_OBJ;

/** MCP SE Object Type - Track Segments */
typedef struct _MCP_SE_TRACK_SEG_OBJ_
{
    /** Length of the segment */
    UINT8   seg_name_len;

    /** Name of the segment */
    UINT8 * seg_name;

    /** Position of the Segment */
    INT32   seg_pos;

    /** Obj len will be Seg Name len field len + Seg Name len + Seg Position length */
    UINT16  total_obj_len;

}MCP_SE_TRACK_SEG_OBJ;

/** MCP SE Object Type - Track */
typedef struct _MCP_SE_TRACK_OBJ_
{
    /** Represented as IDV32 format */
    void   * IDV32_data;

    /** Obj len will be complete ID3v2 length */
    UINT16   total_obj_len;

}MCP_SE_TRACK_OBJ;

/** MCP SE Object Type - Group, Can have 1 or more of below */
typedef struct _MCP_SE_GRP_OBJ_
{
    /** Type of the Object */
    UINT8  obj_type;

    /** Object ID */
    UINT8  obj[MCP_OBJ_ID_LEN];

    /** Obj len will be [(Obj Type Len + OBJ_ID_LEN ) * Number of such obj] */
    UINT16 total_obj_len;

}MCP_SE_GRP_OBJ;

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
 * \defgroup ga_mcp_se_module_cb MCP SE (Media Control Profile) Server Entity
 * \{
 * \brief This section describes the callback for MCP SE.
 */

/**
 * MCP SE Callback to be registered by the Application
 *
 * \param [in] device     Peer Device Info
 * \param [in] chr        Response context
 * \param [in] evt_id     Event type
 * \param [in] evt_status Event status
 * \param [in] evt_data   Event Data and Datalength
 */
typedef GA_RESULT (*MCP_SE_NTF_CB)
                  (
                      GA_ENDPOINT           * device,
                      GA_MCP_SE_RSP_CONTEXT * chr,
                      MCP_SE_EVT_ID           evt_id,
                      UINT16                  evt_status,
                      MCP_SE_EVT            * evt_data
                  );

/** \} */
/** \} */

/* --------------------------------------------- Macros */


/* --------------------------------------------- APIs */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup ga_mcp_api_defs API Definitions
 * \{
 * \brief This section describes the EtherMind Media Control Profile APIs.
 */

/**
 * \defgroup ga_mcp_se_api_defs MCP SE (Media Control Profile) Server Entity
 * \{
 * \brief This section describes the Media Control Profile APIs for Server.
 */

/**
 * \name MCP Server APIs - Init
 * \{
 * \brief This section describes the Media Control Profile Init APIs
 * for Server.
 */

/**
 *  \brief To Init the MCP Server Module.
 *
 *  \par Description:
 *       This function enables to initialize the MCP Server Entity Module.
 *       and register a callback with MCP Module.
 *       This callback will be triggered whenever there are events generated
 *       as part of the functionality and also to indicate Configuration,
 *       Read, Write requests received from peer.
 *       If OTS is enabled, OTS related events are also received.
 *       This function adds GMCS instance by default with the instance ID as
 *       \ref MCP_SE_GMCS_HANDLE.
 *
 *  \param [in] cb
 *         MCP SE Callback.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_mcp_se_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_se_init(MCP_SE_NTF_CB cb);

/**
 *  \brief To add an instance of MCS.
 *
 *  \par Description:
 *       This function adds the MCS instance to the DB.
 *
 *  \param [out] mcp_se_handle
 *         MCS Session Instance Identifier.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \note Maximum number of instances of MCS that is supported
 *       \ref MCP_SE_MAX_MCS_ONLY_SER_ENTITIES.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_se_add_mcs_inst(MCP_SE_HANDLE * mcp_se_handle);

/** \} */

/**
 * \name MCP Server APIs - Notify
 * \{
 * \brief This section describes the Media Control Profile Notify APIs for
 * Server.
 */

/**
 *  \brief To send Notifications to the peer MCP Client.
 *
 *  \par Description:
 *       This function sends notifications to the peer if the configuration
 *       is enabled by the peer before.
 *
 *  \param [in] ga_dev
 *         If NULL,
 *         Notification to all connected GMCS sessions on peer will be sent.
 *         Else,
 *         To the corresponding peer with the address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_uuid
 *         Char UUID.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification.
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_se_send_ntf
          (
              /* IN */ GA_ENDPOINT   * ga_dev,
              /* IN */ MCP_SE_HANDLE   mcp_se_handle,
              /* IN */ UINT16          char_uuid,
              /* IN */ void          * char_info,
              /* IN */ UCHAR           char_info_len
          );

/**
 *  \brief To send Notifications - Media Player Name.
 *
 *  \par Description:
 *       If configured, this function sends notifications for Media Player Name.
 *
 *  \param [in] ga_dev
 *         If NULL,
 *         Notification to all connected GMCS sessions on peer will be sent.
 *         Else,
 *         To the corresponding peer with the address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_CHAR_PARAM_TYPE_UTF8_STR
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_media_player_name(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_MEDIA_PLAYER_NAME), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Track Changed.
 *
 *  \par Description:
 *       This function sends notifications if configured for Track Changed.
 *       Whenever there is a change in Track, this function needs
 *       to be triggered.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_CHAR_PARAM_TYPE_UTF8_STR
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_track_changed(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_TRACK_CHANGED), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Track Title.
 *
 *  \par Description:
 *       This function sends notifications if configured for Track Title.
 *       Whenever there is a change in Track Title value, this function needs
 *       to be triggered.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_CHAR_PARAM_TYPE_UTF8_STR
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_track_title(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_TRACK_TITLE), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Track Duration.
 *
 *  \par Description:
 *       This function sends notifications if configured for Track Duration.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_CHAR_PARAM_TYPE_INT32
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_track_duration(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_TRACK_DURATION), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Track Position.
 *
 *  \par Description:
 *       This function sends notifications if configured for Track Position.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_CHAR_PARAM_TYPE_INT32
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_track_pos(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_TRACK_POS), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Playback Speed.
 *
 *  \par Description:
 *       This function sends notifications if configured for Playback Speed.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_CHAR_PARAM_TYPE_INT8
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_playback_speed(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_PLAYBACK_SPEED), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Seeking Speed.
 *
 *  \par Description:
 *       This function sends notifications if configured for Seeking Speed.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_CHAR_PARAM_TYPE_INT8
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_seeking_speed(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_SEEKING_SPEED), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Current Track Obj ID.
 *
 *  \par Description:
 *       This function sends notifications if configured for Current Track Obj ID.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_PARAM_OBJ_ID
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_curr_track_obj_id(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_CURR_TRACK_OBJ_ID), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Next Track Object ID.
 *
 *  \par Description:
 *       This function sends notifications if configured for Next Track Object ID.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_PARAM_OBJ_ID
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_next_track_obj_id(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_NEXT_TRACK_OBJ_ID), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Parent Group Object ID.
 *
 *  \par Description:
 *       This function sends notifications if configured for Parent Group Object ID.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_PARAM_OBJ_ID
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_parent_grp_obj_id(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_PARENT_GROUP_OBJ_ID), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Current Group Object ID.
 *
 *  \par Description:
 *       This function sends notifications if configured for Current Group Object ID.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_PARAM_OBJ_ID
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_curr_grp_obj_id(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_CURR_GROUP_OBJ_ID), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Playing Order.
 *
 *  \par Description:
 *       This function sends notifications if configured for Playing Order.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_CHAR_PARAM_TYPE_UINT8
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_playing_order(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_PLAYING_ORDER), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Media State.
 *
 *  \par Description:
 *       This function sends notifications if configured for Media State.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_CHAR_PARAM_TYPE_UINT8
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_media_state(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_MEDIA_STATE), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Media Control Point(MCP).
 *
 *  \par Description:
 *       This function sends notifications if configured for MCP.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_PARAM_MCP_RSP
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_mcp(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_MEDIA_CONTROL_POINT), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - MCP Opcode Supported.
 *
 *  \par Description:
 *       This function sends notifications if configured for MCP Opcode Supported.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_CHAR_PARAM_TYPE_UINT32
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_mcp_opc_supp(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_CP_OPC_SUPP), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - SCP.
 *
 *  \par Description:
 *       This function sends notifications if configured for SCP Opcode.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_SCP_RES_CODE
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_scp(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_SEARCH_CONTROL_POINT), (char_info), (char_info_len))

/**
 *  \brief To send Notifications - Search Results Object ID.
 *
 *  \par Description:
 *       This function sends notifications if configured for Search Results Object ID.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref MCP_PARAM_OBJ_ID
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_notify_search_res_obj_id(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_send_ntf((ga_dev), (mcp_se_handle), (GA_CHAR_MCS_SEARCH_RES_OBJ_ID), (char_info), (char_info_len))

/** \} */

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/**
 * \name MCP Server APIs - OTP Indicate
 * \{
 * \brief This section describes the Media Control Profile - OTP Data write.
 */

/**
 *  \brief To send Indications - OTS.
 *
 *  \par Description:
 *       This function sends indications if configured.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_uuid
 *         UUID of the char on which Indication has to go.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the indication.
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_se_OTS_send_ind
          (
              /* IN */ GA_ENDPOINT   * ga_dev,
              /* IN */ MCP_SE_HANDLE   mcp_se_handle,
              /* IN */ UINT16          char_uuid,
              /* IN */ void          * char_info,
              /* IN */ UINT16          char_info_len
          );

/**
 *  \brief To send Indications - OTS on OACP char.
 *
 *  \par Description:
 *       This function sends indications(OACP response) if configured on OACP.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the indications.
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_OTS_indicate_OACP_rsp(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_OTS_send_ind((ga_dev), (mcp_se_handle), (GA_CHAR_OTS_CHAR_OACP), (char_info), (char_info_len))

/**
 *  \brief To send Indications - OTS on OLCP char.
 *
 *  \par Description:
 *       This function sends indications(OLCP response) if configured on OACP.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected GMCS sessions will be passed.
 *         Else, to the corresponding address passed.
 *
 *  \param [in] mcp_se_handle
 *         GMCS/MCS Session Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the indications.
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_mcp_se_OTS_indicate_OLCP_rsp(ga_dev, mcp_se_handle, char_info, char_info_len) \
        GA_mcp_se_OTS_send_ind((ga_dev), (mcp_se_handle), (GA_CHAR_OTS_CHAR_OLCP), (char_info), (char_info_len))

/** \} */

/**
 * \name MCP Server APIs - OTP Data Write
 * \{
 * \brief This section describes the Media Control Profile - OTP Data write.
 */

/**
 *  \brief To trigger a write on Data Channel established.
 *
 *  \par Description:
 *       This function triggers a write on the data channel established.
 *
 *  \param [in] dev
 *         Remote device address
 *
 *  \param [in] cid
 *         Channel Identifier provided as part of \ref MCS_DATA_CHANNEL_UP_IND.
 *
 *  \param [in] buffer
 *         Buffer containing packet to be sent
 *
 *  \param [in] buffer_len
 *         Packet length
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa \ref MCP_SE_DATA_CHANNEL_WRITE_IND
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_se_data_channel_write
          (
              /* IN */ GA_ENDPOINT * dev,
              /* IN */ UINT16        cid,
              /* IN */ UCHAR       * buffer,
              /* IN */ UINT16        buffer_len
          );

/** \} */

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/**
 * \name MCP Server APIs - Read/Write
 * \{
 * \brief This section describes the Media Control Profile Read/Write APIs for
 * Server.
 */

/**
 *  \brief To trigger sending of response for read or write request.
 *
 *  \par Description:
 *       This function triggers sending of response for read/write.
 *
 *  \param [in] device
 *         Remote device address
 *
 *  \param [in] ctx
 *         Context to be passed back as sent.
 *
 *  \param [in] mcp_se_handle
 *         MCS Session ID on which request is received and response is to be sent.
 *
 *  \param [in] mcs_evt_id
 *         Event ID to indicate the request.
 *
 *  \param [in] rsp_status
 *         Response Status for read/write request.
 *
 *  \param [in] rsp_info
 *         Response packet to be sent - Valid for Read request event type only.
 *         NULL - For Write request.
 *
 *  \param [in] rsp_info_len
 *         Response Packet length - Valid for Read request event type only.
 *         0 - For Write request.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_mcp_se_events
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_mcp_se_send_rsp
          (
              /* IN */ GA_ENDPOINT           * device,
              /* IN */ GA_MCP_SE_RSP_CONTEXT * ctx,
              /* IN */ MCP_SE_HANDLE           mcp_se_handle,
              /* IN */ MCP_SE_EVT_ID           mcs_evt_id,
              /* IN */ GA_RESULT               rsp_status,
              /* IN */ void                  * rsp_info,
              /* IN */ UINT16                  rsp_info_len
          );

/** \} */

/**
 * \name MCP Server APIs - De-Init
 * \{
 * \brief This section describes the Media Control Profile DeInit APIs for
 * Server.
 */

/**
 *  \brief To Shutdown the MCP Server Module.
 *
 *  \par Description:
 *       This function enables to Shutdown the MCP Server Entity Module.
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
GA_RESULT GA_mcp_se_shutdown(void);

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

#endif /* _H_GA_MCP_SE_API_ */
