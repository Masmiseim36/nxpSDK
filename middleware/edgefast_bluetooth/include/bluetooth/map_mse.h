/** @file
 *  @brief Bluetooth MAP handling.
 */

/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_MAP_MSE_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_MAP_MSE_H_

/**
 * @brief Message Access Profile (MAP)
 * @defgroup bt_map Message Access Profile (MAP)
 * @ingroup bluetooth
 * @{
 */

#include <bluetooth/obex.h>
#include <bluetooth/map_types.h>

/**
 * @brief Reserve length for SendEvent.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mse_mns  MSE MNS object.
 * @param flags refer to enum bt_obex_req_flags.
 
 * @return the reservation length.
 */
#define BT_MAP_MSE_RSV_LEN_SEND_EVENT(mse_mns, flags) \
        (((flags & BT_OBEX_REQ_START) ? sizeof(struct bt_map_send_event_hdr) \
         : sizeof(struct bt_map_ops_put_cont_hdr)) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for SendResponse.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the map.
 *
 * @param mse_mas  MSE MAS object.
 *
 * @return the reservation length.
 */
#define BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas) (sizeof(struct bt_map_resp_hdr) \
        + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/** @brief MAP MSE MAS structure */
struct bt_map_mse_mas;

/** @brief MAP MSE MNS structure */
struct bt_map_mse_mns;

/** @brief MAP MSE MAS application callback */
struct bt_map_mse_mas_cb
{
    /** MSE MAS connected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection completes.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param psm  local L2CAP PSM.
     *  @param scn  local RFCOM server channel.
     */
    void (*connected)(struct bt_map_mse_mas *mse_mas, uint16_t psm, uint8_t scn);

    /** MSE MAS disconnected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection gets disconnected, including when a connection gets
     *  rejected or cancelled or any error in OBEX connection establisment.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS in case of success or otherwise in case of error.
     */
    void (*disconnected)(struct bt_map_mse_mas *mse_mas, uint8_t result);

    /** MSE MAS abort Callback
     *
     *  This callback informs application to abort the current operation.
     *  The abort response is sent internally.
     *
     *  @param mse_mas  MAP MSE MAS object.
     */
    void (*abort)(struct bt_map_mse_mas *mse_mas);

    /** MSE MAS get folder listing Callback
     *
     *  It is called when receiving get folder listing request.
     *  Appplication can use bt_map_mse_app_param_parse to obtain application parameters.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param buf  RX net buffer contains MAP application parameters.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*get_folder_listing)(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);

    /** MSE MAS set folder Callback
     *
     *  This callback provides the path to application.
     *  When name is "/", go to root directory.
     *  When name is "../", go up one level.
     *  When name is "../child", go up one level and then go to child directory.
     *  When name is "child", go to child directory.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param name  the folder name string.
     */
    void (*set_folder)(struct bt_map_mse_mas *mse_mas, char *name);

    /** MSE MAS get message listing Callback
     *
     *  It is called when receiving get message listing request.
     *  Appplication can use bt_map_mse_app_param_parse to obtain application parameters.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param buf  RX net buffer contains MAP application parameters.
     *  @param name  string - the child folder name, NULL - current directory.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*get_msg_listing)(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag);

    /** MSE MAS get message Callback
     *
     *  It is called when receiving get message request.
     *  Appplication can use bt_map_mse_app_param_parse to obtain application parameters.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param buf  RX net buffer contains MAP application parameters.
     *  @param name  string - the child folder name, NULL - current directory.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*get_msg)(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag);

    /** MSE MAS set message status Callback
     *
     *  It is called when receiving set message status request.
     *  Appplication can use bt_map_mse_app_param_parse to obtain application parameters
     *  and use bt_map_mse_get_body to obtain body from RX net buffer.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param buf  RX net buffer contains MAP application parameters and body.
     *  @param name  string - the child folder name, NULL - current directory.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*set_msg_status)(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag);

    /** MSE MAS push message Callback
     *
     *  It is called when receiving push message request.
     *  Appplication can use bt_map_mse_app_param_parse to obtain application parameters
     *  and use bt_map_mse_get_body to obtain body from RX net buffer.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param buf  RX net buffer contains MAP application parameters and body.
     *  @param name  string - the child folder name, NULL - current directory.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*push_msg)(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag);

    /** MSE MAS set notification registration Callback
     *
     *  It is called when receiving set notification registration request.
     *  Appplication can use bt_map_mse_app_param_parse to obtain application parameters
     *  and use bt_map_mse_get_body to obtain body from RX net buffer.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param buf  RX net buffer contains notification status and body.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*set_ntf_reg)(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);

    /** MSE MAS update inbox Callback
     *
     *  It is called when receiving update inbox request.
     *
     *  @param mse_mas  MAP MSE MAS object.
     */
    void (*update_inbox)(struct bt_map_mse_mas *mse_mas);

    /** MSE MAS get MAS instance information Callback
     *
     *  It is called when receiving get MAS instance infomation request.
     *  Appplication can use bt_map_mse_app_param_parse to obtain application parameters.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param buf  RX net buffer contains MAP application parameters.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*get_mas_inst_info)(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);

    /** MSE MAS set owner status Callback
     *
     *  It is called when receiving get set owner status request.
     *  Appplication can use bt_map_mse_app_param_parse to obtain application parameters.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param buf  RX net buffer contains MAP application parameters.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*set_owner_status)(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);

    /** MSE MAS get owner status Callback
     *
     *  It is called when receiving get owner status request.
     *  Appplication can use bt_map_mse_app_param_parse to obtain application parameters.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param buf  RX net buffer contains MAP application parameters.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*get_owner_status)(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);

    /** MSE MAS get conversation listing Callback
     *
     *  It is called when receiving get conversation listing request.
     *  Appplication can use bt_map_mse_app_param_parse to obtain application parameters.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param buf  RX net buffer contains MAP application parameters.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*get_convo_listing)(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);

    /** MSE MAS set notification filter Callback
     *
     *  It is called when receiving get set notification filter request.
     *  Appplication can use bt_map_mse_app_param_parse to obtain application parameters
     *  and use bt_map_mse_get_body to obtain body from RX net buffer.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param buf  RX net buffer contains MAP application parameters.
     *  @param flags  refer to enum bt_obex_req_flags.
     */
    void (*set_ntf_filter)(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);
};

/** @brief MAP MSE MNS application callback */
struct bt_map_mse_mns_cb
{
    /** MSE MNS connected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection completes.
     *
     *  @param mse_mas  MAP MSE MNS object.
     * 
     */
    void (*connected)(struct bt_map_mse_mns *mse_mns);

    /** MSE MNS disconnected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection gets disconnected, including when a connection gets
     *  rejected or cancelled or any error in OBEX connection establisment.
     *
     *  @param mse_mas  MAP MSE MNS object.
     *  @param result  BT_MAP_RSP_SUCCESS in case of success or otherwise in case of error.
     */
    void (*disconnected)(struct bt_map_mse_mns *mse_mns, uint8_t result);

    /** MSE MNS send event Callback
     *
     *  This callback provides send event result to application.
     *
     *  @param mse_mas  MAP MSE MAS object.
     *  @param result  BT_MAP_RSP_SUCCESS or BT_MAP_RSP_CONTINUE in case of success or otherwise in case of error.
     */
    void (*send_event)(struct bt_map_mse_mns *mse_mns, uint8_t result);
};

/** @brief Register MSE MAS
 *
 *  Register MAP profile MSE MAS callback to monitor the event from MCE.
 *  This function just needs to be call one time for multiple MAS server instances.
 *  Application can use the parameter of mse_mas in the callback to determine the MAS
 *  server instance.
 *
 *  @param cb callback structure.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_map_mse_mas_register(struct bt_map_mse_mas_cb *cb);

/** @brief Unregister MSE MAS
 *
 *  Unregister MAP profile MSE MAS callback. This function is to be called when
 *  there is no MAS OBEX connection.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_map_mse_mas_unregister(void);

/** @brief Disconnect MSE MAS connection
 *
 *  Disconnect MSE MAS OBEX connection.
 *  This function can be called multiple times to disconnect multiple MAS OBEX connections.
 *
 *  @param mse_mas  MSE MAS object.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_disconnect(struct bt_map_mse_mas *mse_mas);

/** @brief Send response when receiving get folder listing request from MCE.
 *
 *  This function is to send response when receiving get folder listing request from MCE.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_MAX_LIST_COUNT and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  Setting wait to true is to ask the partner to wait after sending its next request.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *  @note the data size of the net_buf is managed by application. Application need to
 *  make sure the data size doesn't exceed the maximum packet length.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *  @param buf  TX net buffer allocated by application.
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_get_folder_listing_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait);

/** @brief Send response when receiving set folder request from MCE.
 *
 *  This function is to send response when receiving set folder request from MCE.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_set_folder_response(struct bt_map_mse_mas *mse_mas, uint8_t result);

/** @brief Send response when receiving get message listing request from MCE.
 *
 *  This function is to send response when receiving get message listing request from MCE.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_MAX_LIST_COUNT and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  Setting wait to true is to ask the partner to wait after sending its next request.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *  @note the data size of the net_buf is managed by application. Application need to
 *  make sure the data size doesn't exceed the maximum packet length.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *  @param buf  TX net buffer allocated by application.
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_get_msg_listing_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait);

/** @brief Send response when receiving get message request from MCE.
 *
 *  This function is to send response when receiving get message request from MCE.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_MAX_LIST_COUNT and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  Setting wait to true is to ask the partner to wait after sending its next request.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *  @note the data size of the net_buf is managed by application. Application need to
 *  make sure the data size doesn't exceed the maximum packet length.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *  @param buf  TX net buffer allocated by application.
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_get_msg_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait);

/** @brief Send response when receiving set message status request from MCE.
 *
 *  This function is to send response when receiving set message status request from MCE.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_set_msg_status_response(struct bt_map_mse_mas *mse_mas, uint8_t result);

/** @brief Send response when receiving push message request from MCE.
 *
 *  This function is to send response when receiving push message request from MCE.
 *  Setting wait to true is to ask the partner to wait after sending its next request.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *  @param name pass a string if message handle is generated or NULL.
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_push_msg_response(struct bt_map_mse_mas *mse_mas, uint8_t result, char *name, bool wait);

/** @brief Send response when receiving set notification registration request from MCE.
 *
 *  This function is to send response when receiving set notification registration from MCE.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_set_ntf_reg_response(struct bt_map_mse_mas *mse_mas, uint8_t result);

/** @brief Send response when receiving update inbox request from MCE.
 *
 *  This function is to send response when receiving update inbox request from MCE.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_update_inbox_response(struct bt_map_mse_mas *mse_mas, uint8_t result);

/** @brief Send response when receiving get MAS instance infomation request from MCE.
 *
 *  This function is to send response when receiving get MAS instance infomation request from MCE.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_OWNER_UCI and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  Setting wait to true is to ask the partner to wait after sending its next request.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *  @note the data size of the net_buf is managed by application. Application need to
 *  make sure the data size doesn't exceed the maximum packet length.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *  @param buf  TX net buffer allocated by application.
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_get_mas_inst_info_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait);

/** @brief Send response when receiving set owner status request from MCE.
 *
 *  This function is to send response when receiving set owner status request from MCE.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_set_owner_status_response(struct bt_map_mse_mas *mse_mas, uint8_t result);

/** @brief Send response when receiving get owner status request from MCE.
 *
 *  This function is to send response when receiving get owner status from MCE.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_PRESENCE_AVAILABILITY and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  Setting wait to true is to ask the partner to wait after sending its next request.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *  @note the data size of the net_buf is managed by application. Application need to
 *  make sure the data size doesn't exceed the maximum packet length.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *  @param buf  TX net buffer allocated by application.
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_get_owner_status_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait);

/** @brief Send response when receiving get conversation listing request from MCE.
 *
 *  This function is to send response when receiving get conversation listing from MCE.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_MAX_LIST_COUNT and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  Setting wait to true is to ask the partner to wait after sending its next request.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *  @note the data size of the net_buf is managed by application. Application need to
 *  make sure the data size doesn't exceed the maximum packet length.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *  @param buf  TX net buffer allocated by application.
 *  @param wait  true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_get_convo_listing_response(struct bt_map_mse_mas *mse_mas, uint8_t result, struct net_buf *buf, bool wait);

/** @brief Send response when receiving set notification filter request from MCE.
 *
 *  This function is to send response when receiving set notification filter request from MCE.
 *  When receiving request, the callback that is registered by
 *  bt_map_mse_mas_register is called.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param result response code - BT_MAP_RSP_SUCCESS, BT_MAP_RSP_CONTINUE or other error codes.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_set_ntf_filter_response(struct bt_map_mse_mas *mse_mas, uint8_t result);

/** @brief Get MAS maximum packet length
 *
 *  This function is to get the maximum packet length in MSE MAS OBEX connection.
 *  This function returns immediately.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param max_pkt_len  the return maximum packet length.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_get_max_pkt_len(struct bt_map_mse_mas *mse_mas, uint16_t *max_pkt_len);

/** @brief Register MSE MNS
 *
 *  Register MAP profile MSE MNS callback to monitor the event from MCE.
 *  This function just needs to be call one time.
 *
 *  @param cb callback structure.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_map_mse_mns_register(struct bt_map_mse_mns_cb *cb);

/** @brief Unregister MSE MNS
 *
 *  Unregister MAP profile MSE MNS callback. This function is to be called when
 *  there is no MNS OBEX connection.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_map_mse_mns_unregister(void);

/** @brief Create MSE MNS connection based on L2CAP
 *
 *  This function is to be called after MCE sets SetNotificationRegistration to ON.
 *  The API is to be used to establish MNS OBEX connection between devices.
 *  This function establishes L2CAP connection.
 *  This function can be called once as there is only one MNS connection for all MAS Instances.
 *  After connection success, the callback that is registered by
 *  bt_map_mse_mns_register is called.
 *
 *  @param conn Pointer to bt_conn structure.
 *  @param psm  GoepL2capPsm, returned in SDP record.
 *  @param supported_features  partner device's supported features, returned in SDP record.
 *  @param mse_mns  MSE MNS object.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_mns_psm_connect(struct bt_conn *conn, uint16_t psm, uint32_t supported_features, struct bt_map_mse_mns **mse_mns);

/** @brief Create MSE MNS connection based on RFCOM
 *
 *  This function is to be called after MCE sets SetNotificationRegistration to ON.
 *  The API is to be used to establish MNS OBEX connection between devices.
 *  This function establishes RFCOM connection.
 *  This function can be called once as there is only one MNS connection for all MAS Instances.
 *  After connection success, the callback that is registered by
 *  bt_map_mse_mns_register is called.
 *
 *  @param conn Pointer to bt_conn structure.
 *  @param scn  RFCOM server channel number, returned in SDP record.
 *  @param supported_features  partner device's supported features, returned in SDP record.
 *  @param mse_mns  MSE MNS object.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_mns_scn_connect(struct bt_conn *conn, uint8_t scn, uint32_t supported_features, struct bt_map_mse_mns **mse_mns);

/** @brief Disconnect MSE MNS connection
 *
 *  Disconnect MSE MNS OBEX connection.
 *
 *  @param mse_mas  MSE MNS object.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_mns_disconnect(struct bt_map_mse_mns *mse_mns);

/** @brief Send event to MCE
 *
 *  This function is to send event to MCE.
 *  This function can be called multiple times till the event object is sent completely.
 *  Application uses the parameter of flags to send the chunked packet.
 *  Application can use BT_MAP_ADD_xxx, such as BT_MAP_ADD_MAS_INSTANCE_ID and BT_MAP_ADD_BODY,
 *  to add application parameters and add body into TX net buffer.
 *  Calling BT_MAP_ADD_xxx to add application parameters should follow a call to net_buf_reserve.
 *  When receiving response, the callback that is registered by
 *  bt_map_mse_mns_register is called.
 *  @note the data size of the net_buf is managed by application. Application need to
 *  make sure the data size doesn't exceed the maximum packet length.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param buf  TX net buffer allocated by application.
 *  @param flags refer to enum bt_obex_req_flags.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_send_event(struct bt_map_mse_mns *mse_mns, struct net_buf *buf, enum bt_obex_req_flags flags);

/** @brief Get MNS maximum packet length
 *
 *  This function is to get the maximum packet length in MSE MNS OBEX connection.
 *  This function returns immediately.
 *
 *  @param mse_mas  MSE MAS object.
 *  @param max_pkt_len  the return maximum packet length.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_map_mse_mns_get_max_pkt_len(struct bt_map_mse_mns *mse_mns, uint16_t *max_pkt_len);

/**
 * @brief Helper for parsing application parameters.
 *
 * A helper for parsing the application parameters. The most
 * common scenario is to call this helper on the application parameters
 * received in the callback that was given to bt_map_mse_mas_register or bt_map_mse_mns_register.
 *
 * @param buf       net buffer received in the callback.
 * @param func      Callback function which will be called for each tag
 *                  that's found in the buffer. The callback should return
 *                  true to continue parsing, or false to stop parsing.
 * @param user_data User data to be passed to the callback.
 */
#define bt_map_mse_app_param_parse(buf, func, user_data) bt_obex_app_param_parse(buf, func, user_data)

/**
 * @brief Helper for getting body data.
 *
 * A helper for getting the body data. The most common scenario 
 * is to call this helper on the body received in the callback that
 * was given to bt_map_mse_mas_register or bt_map_mse_mns_register.
 *
 * @param buf       net buffer received in the callback.
 * @param body      pointer used for holding body data.
 * @param length    the length of the body data.
 *
 * @return 0 in case of success or negative value in case of error.
 */
#define bt_map_mse_get_body(buf, body, length) bt_obex_get_body(buf, body, length)

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_MAP_H_ */
