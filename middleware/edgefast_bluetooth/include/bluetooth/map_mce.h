/** @file
 *  @brief Bluetooth MAP handling.
 */

/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_MAP_MCE_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_MAP_MCE_H_

/**
 * @brief Message Access Profile (MAP)
 * @defgroup bt_map Message Access Profile (MAP)
 * @ingroup bluetooth
 * @{
 */

#include <bluetooth/obex.h>
#include <bluetooth/map_types.h>

/**
 * @brief Reserve length for GetFolderListing.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 * @param flags refer to enum bt_obex_req_flags.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_GET_FOLDER_LISTING(mce_mas, flags) \
        (((flags & BT_OBEX_REQ_START) ? sizeof(struct bt_map_get_folder_listing_hdr) \
         : sizeof(struct bt_map_ops_get_cont_hdr)) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for SetFolder.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_SET_FOLDER(mce_mas) (sizeof(struct bt_map_set_folder_hdr) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for GetMessageListing.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 * @param name  the folder name string
 * @param flags refer to enum bt_obex_req_flags.
 
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_GET_MSG_LISTING(mce_mas, name, flags) \
        (((flags & BT_OBEX_REQ_START) ? \
         (sizeof(struct bt_map_get_msg_listing_hdr) + \
          (((name == NULL) || (strlen(name) == 0)) ? sizeof(struct bt_obex_hdr_bytes) : \
          ((strlen(name) + 1U) * 2U + sizeof(struct bt_obex_hdr_bytes)))) \
         : sizeof(struct bt_map_ops_get_cont_hdr)) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for GetMessage.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 * @param flags refer to enum bt_obex_req_flags.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_GET_MSG(mce_mas, flags) \
        (((flags & BT_OBEX_REQ_START) ? sizeof(struct bt_map_get_msg_hdr) \
         : sizeof(struct bt_map_ops_get_cont_hdr)) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for SetMessageStatus.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 * @param flags refer to enum bt_obex_req_flags.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_SET_MSG_STATUS(mce_mas, flags) \
        (((flags & BT_OBEX_REQ_START) ? sizeof(struct bt_map_set_msg_status_hdr) : \
        sizeof(struct bt_map_ops_put_cont_hdr)) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for PushMessage.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 * @param name  the folder name string
 * @param flags refer to enum bt_obex_req_flags.
 
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_PUSH_MSG(mce_mas, name, flags) \
        (((flags & BT_OBEX_REQ_START) ? \
         (sizeof(struct bt_map_push_msg_hdr) + \
          (((name == NULL) || (strlen(name) == 0)) ? sizeof(struct bt_obex_hdr_bytes) : \
          ((strlen(name) + 1U) * 2U + sizeof(struct bt_obex_hdr_bytes)))) \
         : sizeof(struct bt_map_ops_put_cont_hdr)) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for SetNotificationRegistration.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_SET_NTF_REG(mce_mas) (sizeof(struct bt_map_set_ntf_reg_hdr) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for UpdateInbox.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_UPDATE_INBOX(mce_mas) (sizeof(struct bt_map_update_inbox_hdr) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for GetMASInstanceInformation.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 * @param flags refer to enum bt_obex_req_flags.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_GET_MAS_INST_INFO(mce_mas, flags) \
        (((flags & BT_OBEX_REQ_START) ? sizeof(struct bt_map_get_mas_inst_info_hdr) \
         : sizeof(struct bt_map_ops_get_cont_hdr)) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for SetOwnerStatus.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 * @param flags refer to enum bt_obex_req_flags.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_SET_OWNER_STATUS(mce_mas, flags) \
        (((flags & BT_OBEX_REQ_START) ? sizeof(struct bt_map_set_owner_status_hdr) \
         : sizeof(struct bt_map_ops_put_cont_hdr)) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */
/**
 * @brief Reserve length for GetOwnerStatus.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 * @param flags refer to enum bt_obex_req_flags.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_GET_OWNER_STATUS(mce_mas, flags) \
        (((flags & BT_OBEX_REQ_START) ? sizeof(struct bt_map_get_owner_status_hdr) \
         : sizeof(struct bt_map_ops_get_cont_hdr)) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for GetConversationListing.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 * @param flags refer to enum bt_obex_req_flags.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_GET_CONVO_LISTING(mce_mas, flags) \
        (((flags & BT_OBEX_REQ_START) ? sizeof(struct bt_map_get_convo_listing_hdr) \
         : sizeof(struct bt_map_ops_get_cont_hdr)) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for SetNotificationFilter.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mas  MCE MAS object.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_SET_NTF_FILTER(mce_mas) (sizeof(struct bt_map_set_ntf_filter_hdr) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for SendEventResponse.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mce_mns  MCE MNS object.
 *
 * @return the reservation length.
 */
#define BT_MAP_MCE_RSV_LEN_SEND_EVENT_RESP(mce_mns) (sizeof(struct bt_map_resp_hdr) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/** @brief MAP MCE MAS structure */
struct bt_map_mce_mas;

/** @brief MAP MCE MNS structure */
struct bt_map_mce_mns;

/** @brief MAP MCE MAS application callback */
struct bt_map_mce_mas_cb
{
    /** MCE MAS connected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection completes.
     *
     *  @param mce_mas  MAP MCE MAS object.
     * 
     */
    void (*connected)(struct bt_map_mce_mas *mce_mas);

    /** MCE MAS disconnected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection gets disconnected, including when a connection gets
     *  rejected or cancelled or any error in OBEX connection establisment.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS in case of success or otherwise in case of error.
     */
    void (*disconnected)(struct bt_map_mce_mas *mce_mas, uint8_t result);

    /** MCE MAS abort Callback
     *
     *  This callback provides the result of abortion to application.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS in case of success or otherwise in case of error.
     */
    void (*abort)(struct bt_map_mce_mas *mce_mas, uint8_t result);

    /** MCE MAS get folder listing Callback
     *
     *  This callback provides the folder listing object to application.
     *  Appplication can use bt_map_mce_app_param_parse to obtain application parameters
     *  and use bt_map_mce_get_body to obtain body from RX net buffer.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS or BT_MAP_RSP_CONTINUE in case of success or otherwise in case of error.
     *  @param buf  RX net buffer contains MAP application parameters and folder listing object.
     */
    void (*get_folder_listing)(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);

    /** MCE MAS set folder Callback
     *
     *  This callback provides the result of setting folder to application.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS in case of success or otherwise in case of error.
     */
    void (*set_folder)(struct bt_map_mce_mas *mce_mas, uint8_t result);

    /** MCE MAS get message listing Callback
     *
     *  This callback provides the message listing object to application.
     *  Appplication can use bt_map_mce_app_param_parse to obtain application parameters
     *  and use bt_map_mce_get_body to obtain body from RX net buffer.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS or BT_MAP_RSP_CONTINUE in case of success or otherwise in case of error.
     *  @param buf  RX net buffer contains MAP application parameters and message listing object.
     */
    void (*get_msg_listing)(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);

    /** MCE MAS get message Callback
     *
     *  This callback provides the message object to application.
     *  Appplication can use bt_map_mce_app_param_parse to obtain application parameters
     *  and use bt_map_mce_get_body to obtain body from RX net buffer.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS or BT_MAP_RSP_CONTINUE in case of success or otherwise in case of error.
     *  @param buf  RX net buffer contains MAP application parameters and message object.
     */
    void (*get_msg)(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);

    /** MCE MAS set message status Callback
     *
     *  This callback provides the result of setting message status to application.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS in case of success or otherwise in case of error.
     */
    void (*set_msg_status)(struct bt_map_mce_mas *mce_mas, uint8_t result);

    /** MCE MAS push message Callback
     *
     *  This callback provides the result and message handle to application.
     *  Appplication can use bt_map_mce_app_param_parse to obtain application parameters
     *  and use bt_map_mce_get_body to obtain body from RX net buffer.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS or BT_MAP_RSP_CONTINUE in case of success or otherwise in case of error.
     *  @param name  the message handle returned by MSE
     */
    void (*push_msg)(struct bt_map_mce_mas *mce_mas, uint8_t result, char *name);

    /** MCE MAS set notification registration Callback
     *
     *  This callback provides the result of setting notification registration to application.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS in case of success or otherwise in case of error.
     */
    void (*set_ntf_reg)(struct bt_map_mce_mas *mce_mas, uint8_t result);

    /** MCE MAS update inbox Callback
     *
     *  This callback provides the result of updating inbox to application.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS in case of success or otherwise in case of error.
     */
    void (*update_inbox)(struct bt_map_mce_mas *mce_mas, uint8_t result);

    /** MCE MAS get MAS instance information Callback
     *
     *  This callback provides MAS instance information to application.
     *  Appplication can use bt_map_mce_app_param_parse to obtain application parameters
     *  and use bt_map_mce_get_body to obtain body from RX net buffer.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS or BT_MAP_RSP_CONTINUE in case of success or otherwise in case of error.
     *  @param buf  RX net buffer contains MAP application parameters and MAS instance information.
     */
    void (*get_mas_inst_info)(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);

    /** MCE MAS set owner status Callback
     *
     *  This callback provides the result of setting owner status to application.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS or BT_MAP_RSP_CONTINUE in case of success or otherwise in case of error.
     */
    void (*set_owner_status)(struct bt_map_mce_mas *mce_mas, uint8_t result);

    /** MCE MAS get owner status Callback
     *
     *  This callback provides Presence, Chat State, or Last Activity of the owner on the MSE to application.
     *  Appplication can use bt_map_mce_app_param_parse to obtain application parameters
     *  and use bt_map_mce_get_body to obtain body from RX net buffer.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS or BT_MAP_RSP_CONTINUE in case of success or otherwise in case of error.
     *  @param buf  RX net buffer contains MAP application parameters(Presence, Chat State, or Last Activity).
     */
    void (*get_owner_status)(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);

    /** MCE MAS get conversation listing Callback
     *
     *  This callback provides conversation listing object to application.
     *  Appplication can use bt_map_mce_app_param_parse to obtain application parameters
     *  and use bt_map_mce_get_body to obtain body from RX net buffer.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS or BT_MAP_RSP_CONTINUE in case of success or otherwise in case of error.
     *  @param buf  RX net buffer contains MAP application parameters and conversation listing object.
     *  @param wait  if true the remote device ask the local device to wait after sending the next message.
     */
    void (*get_convo_listing)(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);

    /** MCE MAS set notification filter Callback
     *
     *  This callback provides the result of setting notification filter to application.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS in case of success or otherwise in case of error.
     */
    void (*set_ntf_filter)(struct bt_map_mce_mas *mce_mas, uint8_t result);
};

/** @brief MAP MCE MNS application callback */
struct bt_map_mce_mns_cb
{
    /** MCE MNS connected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection completes.
     *
     *  @param mce_mas  MAP MCE MNS object.
     * 
     */
    void (*connected)(struct bt_map_mce_mns *mce_mns);

    /** MCE MNS disconnected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection gets disconnected, including when a connection gets
     *  rejected or cancelled or any error in OBEX connection establisment.
     *
     *  @param mce_mas  MAP MCE MNS object.
     *  @param result  BT_MAP_RSP_SUCCESS in case of success or otherwise in case of error.
     */
    void (*disconnected)(struct bt_map_mce_mns *mce_mns, uint8_t result);

    /** MCE MNS send event Callback
     *
     *  This callback provides event report object to application.
     *
     *  @param mce_mas  MAP MCE MAS object.
     *  @param buf  RX net buffer contains MAS instance ID and event report object.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*send_event)(struct bt_map_mce_mns *mce_mns, struct net_buf *buf, enum bt_obex_req_flags flag);
};

/** @brief Register MCE MAS
 *
 *  Register MAP profile MCE MAS callback to monitor the state change and event from MSE.
 *  This function just needs to be call one time for multiple MAS server instances.
 *  Application can use the parameter of mce_mas in the callback to determine the MAS
 *  server instance.
 *
 *  @param cb callback structure.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_map_mce_mas_register(struct bt_map_mce_mas_cb *cb);

/** @brief Unregister MCE MAS
 *
 *  Unregister MAP profile MCE MAS callback. This function is to be called when
 *  there is no MAS OBEX connection.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_map_mce_mas_unregister(void);

/** @brief Create MCE MAS connection based on L2CAP
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to establish MAS OBEX
 *  connection between devices.
 *  This function establishes L2CAP connection.
 *  This function can be called multiple times to establish multiple MAS OBEX connections.
 *  After connection success, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param conn Pointer to bt_conn structure.
 *  @param psm  GoepL2capPsm, returned in SDP record.
 *  @param supported_features  partner device's supported features, returned in SDP record.
 *  @param mce_mas  MCE MAS object.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_psm_connect(struct bt_conn *conn, uint16_t psm, uint32_t supported_features, struct bt_map_mce_mas **mce_mas);

/** @brief Create MCE MAS connection based on RFCOM
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to establish MAS OBEX
 *  connection between devices.
 *  This function establishes RFCOM connection.
 *  This function can be called multiple times to establish multiple MAS OBEX connections.
 *  After connection success, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param conn Pointer to bt_conn structure.
 *  @param scn  RFCOM server channel number, returned in SDP record.
 *  @param supported_features  partner device's supported features, returned in SDP record.
 *  @param mce_mas  MCE MAS object.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_scn_connect(struct bt_conn *conn, uint8_t scn, uint32_t supported_features, struct bt_map_mce_mas **mce_mas);

/** @brief Disconnect MCE MAS connection
 *
 *  Disconnect MCE MAS OBEX connection.
 *  This function can be called multiple times to disconnect multiple MAS OBEX connections.
 *
 *  @param mce_mas  MCE MAS object.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_disconnect(struct bt_map_mce_mas *mce_mas);

/** @brief Abort MCE MAS operation
 *
 *  Abort MCE MAS PUT or GET operation. This cancels the current outstanding operation.
 *  The return value of -EINPROGRESS means abort is queued and pending and the abort request
 *  will be sent after receiving next response from the MSE.
 *
 *  @param mce_mas  MCE MAS object.
 *
 *  @return 0 in case of success or -EINPROGRESS in case abort is queued or otherwise in case of error.
 */
int bt_map_mce_abort(struct bt_map_mce_mas *mce_mas);

/** @brief Send get folder listing request
 *
 *  This function is to retrieve the Folder-Listing object from the current folder of the MSE.
 *  This function can be called multiple times till the message is sent completely. Application uses 
 *  the parameter of flags to send the chunked packet. It's recommended to intelligently allocate
 *  TX buffer size to put GET request in the single packet.
 *  This function can be called multiple times till the Folder-Listing object is retrieved completely.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_MAX_LIST_COUNT and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  Setting wait to true is to ask the partner to wait after sending its next message.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *  @param flags refer to enum bt_obex_req_flags.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_get_folder_listing(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, bool wait, enum bt_obex_req_flags flags);

/** @brief Send set folder request
 *
 *  This function is to navigate the folders of the MSE.
 *  When name is "/", go to root directory.
 *  When name is ".." or "../", go up one level.
 *  When name is "../child", go up one level and then go to child directory.
 *  When name is "child" or "./child", go to child directory.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param name  the folder name string
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_set_folder(struct bt_map_mce_mas *mce_mas, char *name);

/** @brief Send get messages listing request
 *
 *  This function is to retrieve the Messages-Listing objects from the current folder of the MSE.
 *  This function can be called multiple times till the message is sent completely. Application uses 
 *  the parameter of flags to send the chunked packet. It's recommended to intelligently allocate
 *  TX buffer size to put GET request in the single packet.
 *  This function can be called multiple times till the Messages-Listing objects are retrieved completely.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_MAX_LIST_COUNT and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  When name is NULL, get messages listing in the current folder.
 *  When name is not NULL, get messages listing in the child folder.
 *  Setting wait to true is to ask the partner to wait after sending its next message.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *  @param name  the folder name string
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *  @param flags refer to enum bt_obex_req_flags.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_get_msg_listing(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, char *name, bool wait, enum bt_obex_req_flags flags);

/** @brief Send get messages request
 *
 *  This function is to retrieve a specific message from the MSE.
 *  This function can be called multiple times till the message is sent completely. Application uses 
 *  the parameter of flags to send the chunked packet. It's recommended to intelligently allocate
 *  TX buffer size to put GET request in the single packet.
 *  This function can be called multiple times till the message is retrieved completely.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_MAX_LIST_COUNT and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  When name is NULL, get messages listing in the current folder.
 *  When name is not NULL, get messages listing in the child folder.
 *  Setting wait to true is to ask the partner to wait after sending its next message.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *  @param name  the message handle string
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *  @param flags refer to enum bt_obex_req_flags.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_get_msg(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, char *name, bool wait, enum bt_obex_req_flags flags);

/** @brief Send set message status request
 *
 *  This function is to modify the status of a message on the MSE.
 *  This function can be called multiple times till the message is sent completely. Application uses 
 *  the parameter of flags to send the chunked packet.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_MAX_LIST_COUNT and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *  @param name  the message handle string
 *  @param flags refer to enum bt_obex_req_flags.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_set_msg_status(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flags);

/** @brief Send push message request
 *
 *  This function is to push a message to a folder or conversation of the MSE.
 *  This function can be called multiple times till the message is sent completely. Application uses 
 *  the parameter of flags to send the chunked packet.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_MAX_LIST_COUNT and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  When name is NULL, push message into the current folder.
 *  When name is not NULL, push message into the child folder.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *  @note the data size of the net_buf is managed by application. Application need to
 *  make sure the data size doesn't exceed the maximum packet length.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *  @param name  the folder name string
 *  @param actual  actual number of bytes is sent.
 *  @param flags refer to enum bt_obex_req_flags.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_push_msg(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flags);

/** @brief Send set notification registration request
 *
 *  This function is to register itself for being notified of the arrival of new messages.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_MAX_LIST_COUNT and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_set_ntf_reg(struct bt_map_mce_mas *mce_mas, struct net_buf *buf);

/** @brief Send update inbox request
 *
 *  This function is to initiate an update of the MSE's inbox.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_update_inbox(struct bt_map_mce_mas *mce_mas);

/** @brief Send get MAS instance information request
 *
 *  This function is to retrieve user-readable information about the MAS Instances provided by the MSE.
 *  This function can be called multiple times till the message is sent completely. Application uses 
 *  the parameter of flags to send the chunked packet. It's recommended to intelligently allocate
 *  TX buffer size to put GET request in the single packet.
 *  This function can be called multiple times till the information is retrieved completely.
 *  Application can use BT_MAP_ADD_xxx to add application parameters into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  Setting wait to true is to ask the partner to wait after sending its next message.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *  @param flags refer to enum bt_obex_req_flags.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_get_mas_inst_info(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, bool wait, enum bt_obex_req_flags flags);

/** @brief Send set owner status request
 *
 *  This function is to change the Presence, Chat State, or Last Activity of the owner on the MSE.
 *  This function can be called multiple times till the message is sent completely. Application uses 
 *  the parameter of flags to send the chunked packet.
 *  Application can use BT_MAP_ADD_xxx to add application parameters into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *  @param flags refer to enum bt_obex_req_flags.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_set_owner_status(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, enum bt_obex_req_flags flags);

/** @brief Send get owner status request
 *
 *  This function is to retrieve the Presence, Chat State, or Last Activity of the owner on the MSE.
 *  This function can be called multiple times till the message is sent completely. Application uses 
 *  the parameter of flags to send the chunked packet. It's recommended to intelligently allocate
 *  TX buffer size to put GET request in the single packet.
 *  This function can be called multiple times till the message is retrieved completely.
 *  Application can use BT_MAP_ADD_xxx to add application parameters into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  Setting wait to true is to ask the partner to wait after sending its next message.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *  @param wait  true - set SRMP is 1. false - exculde SMRP.
 *  @param flags refer to enum bt_obex_req_flags.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_get_owner_status(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, bool wait, enum bt_obex_req_flags flags);

/** @brief Send get conversation listing request
 *
 *  This function is retrieve Conversation-Listing objects from the MSE.
 *  This function can be called multiple times till the message is sent completely. Application uses 
 *  the parameter of flags to send the chunked packet. It's recommended to intelligently allocate
 *  TX buffer size to put GET request in the single packet.
 *  This function can be called multiple times till the Conversation-Listing objects are retrieved completely.
 *  Application can use BT_MAP_ADD_xxx to add application parameters into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  Setting wait to true is to ask the partner to wait after sending its next message.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *  @param flags refer to enum bt_obex_req_flags.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_get_convo_listing(struct bt_map_mce_mas *mce_mas, struct net_buf *buf, bool wait, enum bt_obex_req_flags flags);

/** @brief Send set notification filter request
 *
 *  This function is to specify which notifications to receive from the MSE.
 *  Application can use BT_MAP_ADD_xxx to add application parameters into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  When receiving response, the callback that is registered by
 *  bt_map_mce_mas_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_set_ntf_filter(struct bt_map_mce_mas *mce_mas, struct net_buf *buf);

/** @brief Get MAS maximum packet length
 *
 *  This function is to get the maximum packet length in MCE MAS OBEX connection.
 *  This function returns immediately.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param max_pkt_len  the return maximum packet length.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_get_max_pkt_len(struct bt_map_mce_mas *mce_mas, uint16_t *max_pkt_len);


/** @brief Register MCE MNS
 *
 *  Register MAP profile MCE MNS callback to monitor the state change and event from MSE.
 *  This function just needs to be call one time.
 *
 *  @param cb callback structure.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_map_mce_mns_register(struct bt_map_mce_mns_cb *cb);

/** @brief Unregister MCE MNS
 *
 *  Unregister MAP profile MCE MNS callback. This function is to be called when
 *  there is no MNS OBEX connection.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_map_mce_mns_unregister(void);

/** @brief Disconnect MCE MNS connection
 *
 *  Disconnect MCE MNS OBEX connection.
 *
 *  @param mce_mns  MCE MNS object.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_mns_disconnect(struct bt_map_mce_mns *mce_mns);

/** @brief Send event response
 *
 *  This function is to send response when receiving Event-Report object from MSE.
 *  Setting wait to true is to ask the partner to wait after sending its next message.
 *  When receiving Event-Report object, the callback that is registered by
 *  bt_map_mce_mns_register is called.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param result BT_MAP_RSP_SUCCESS or BT_MAP_RSP_CONTINUE in case of success or otherwise in case of error.
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_send_event_response(struct bt_map_mce_mns *mce_mns, uint8_t result, bool wait);

/** @brief Get MNS maximum packet length
 *
 *  This function is to get the maximum packet length in MCE MNS OBEX connection.
 *  This function returns immediately.
 *
 *  @param mce_mas  MCE MAS object.
 *  @param max_pkt_len  the return maximum packet length.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mce_mns_get_max_pkt_len(struct bt_map_mce_mns *mce_mns, uint16_t *max_pkt_len);

/**
 * @brief Helper for parsing application parameters.
 *
 * A helper for parsing the application parameters. The most
 * common scenario is to call this helper on the application parameters
 * received in the callback that was given to bt_map_mce_mas_register or bt_map_mce_mns_register.
 *
 * @param buf       net buffer received in the callback.
 * @param func      Callback function which will be called for each tag
 *                  that's found in the buffer. The callback should return
 *                  true to continue parsing, or false to stop parsing.
 * @param user_data User data to be passed to the callback.
 */
#define bt_map_mce_app_param_parse(buf, func, user_data) bt_obex_app_param_parse(buf, func, user_data)

/**
 * @brief Helper for getting body data.
 *
 * A helper for getting the body data. The most common scenario 
 * is to call this helper on the body received in the callback that
 * was given to bt_map_mce_mas_register or bt_map_mce_mns_register.
 *
 * @param buf       net buffer received in the callback.
 * @param body      pointer used for holding body data.
 * @param length    the length of the body data.
 *
 * @return 0 in case of success or negative value in case of error.
 */
#define bt_map_mce_get_body(buf, body, length) bt_obex_get_body(buf, body, length)

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_MAP_MCE_H_ */
