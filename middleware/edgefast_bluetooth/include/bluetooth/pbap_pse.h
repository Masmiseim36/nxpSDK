/** @file
 *  @brief Phone Book Access Profile handling.
 */

/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_PBAP_PSE_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_PBAP_PSE_H_

/**
 * @brief Phone Book Access Profile (PBAP)
 * @defgroup bt_pbap Phone Book Access Profile (PBAP)
 * @ingroup bluetooth
 * @{
 */

#include <bluetooth/obex.h>
#include <bluetooth/pbap_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief PBAP PSE structure */
struct bt_pbap_pse;

/**
 * @brief Reserve length for PULL RESPONSE.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve,
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the pbap.
 *
 * @param pbap_pse  PBAP PSE object.
 * @param flags     refer to enum bt_obex_req_flags.
 *
 * @return          the reservation length.
 */
#define BT_PBAP_PSE_RSV_LEN_SEND_RESPONSE(pbap_pse)    (sizeof(struct bt_pbap_push_response_hdr) +      \
        BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/** @brief PBAP_PSE profile application callback */
struct bt_pbap_pse_cb
{
    /** PBAP_PSE connected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection completes.
     *
     *  @param pbap_pse  PBAP PSE object.
     *  @param result    BT_PBAP_SUCCESS_RSP in case of success or otherwise in case of error.
     */
    void (*connected)(struct bt_pbap_pse *pbap_pse);

    /** PBAP_PSE disconnected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  disconnection completes.
     *
     *  @param pbap_pse  PBAP PSE object.
     *  @param result    BT_PBAP_SUCCESS_RSP in case of success or reason causes of disconnection.
     */
    void (*disconnected)(struct bt_pbap_pse *pbap_pse, uint8_t result);

    /** PBAP_PSE get authentication information callback to application
     *
     * If this callback is provided it will be called whenever pse asks to authenticate pse,
     * and pse does not provide authentication information when initiating the connection.
     * The application can provide validation authenticate information in this callback.
     *
     *  @param pbap_pse          PBAP PSE object.
     *  @param pbap_auth_info    refer to struct bt_pbap_auth.
     *  @param active_auth       if true that means pse actively authicate pce.
     */
    void (*get_auth_info)(struct bt_pbap_pse *pbap_pse, struct bt_pbap_auth *pbap_auth_info, bool *active_auth);

    /** PBAP_PSE abort callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  abort completes.
     *
     *  @param pbap_pse  PBAP_PSE object.
     *  @param result    BT_PBAP_SUCCESS_RSP in case of success or otherwise in case of error.
     */
    void (*abort)(struct bt_pbap_pse *pbap_pse, uint8_t result);

    /** PBAP_PSE pull phonebook object callback to application
     *
     *  If this callback is provided it will be called whenever get pull phonebook object
     *  request from pce.
     *
     *  @param pbap_pse   PBAP PSE object.
     *  @param buf        RX net buffer from pce.
     *  @param name       Specific phone book path name getting from pce.
     *  @param flag       refer to enum bt_obex_req_flags.
     */
    void (*pull_phonebook)(struct bt_pbap_pse *pbap_pse, struct net_buf *buf, char *name, enum bt_obex_req_flags flag);

    /** PBAP_PSE set phonebook path callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  set phonebook path request received.
     *  When name is "/", go to root directory.
     *  When name is "..", go up one level.
     *  When name is "child" or "./child", go to child
     *
     *  @param pbap_pse   PBAP PSE object.
     *  @param name       path name
     */
    void (*set_phonebook_path)(struct bt_pbap_pse *pbap_pse, char *name);

    /** PBAP_PSE pull vcardlisting object callback to application
     *
     *  If this callback is provided it will be called whenever get pull vcard listing object
     *  request from pce.
     *
     *  @param pbap_pse   PBAP PSE object.
     *  @param buf        RX net buffer from pce
     *  @param name       Specific vcard listing name getting from pce.
     *  @param flag       refer to enum bt_obex_req_flags.
     */
    void (*pull_vcard_listing)(struct bt_pbap_pse *pbap_pse, struct net_buf *buf, char *name, enum bt_obex_req_flags flag);

    /** PBAP_PSE pull vcard entry object callback to application
     *
     *  If this callback is provided it will be called whenever get pull vcard entry object
     *  request from pce.
     *
     *  @param pbap_pse   PBAP PSE object.
     *  @param buf        RX net buffer from pce
     *  @param name       Specific vcard entry name getting from pce.
     *  @param flag       refer to enum bt_obex_req_flags
     */
    void (*pull_vcard_entry)(struct bt_pbap_pse *pbap_pse, struct net_buf *buf, char *name, enum bt_obex_req_flags flag);
};

/** PBAP PSE register
 *
 *  This function called to initialize pbap_pse and register callback.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_pbap_pse_register(struct bt_pbap_pse_cb *cb);

/** @brief Disconnect PBAP connection
 *
 *  Disconnect PBAP connection.
 *
 *  @param pbap_pse  PBAP PSE object.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_pbap_pse_disconnect(struct bt_pbap_pse *pbap_pse);

/** PBAP PSE send response when receiving pull phonebook request from PCE.
 *
 *  This function called to send response when receiving pull phonebook request from PCE.
 *  This function is to be called after receiving pull phonebook request.
 *  This function can be called multiple times untill reponse is send completely.
 *  Application uses @param flag to send the chunked packet.
 *  The tx net buf needs to use BT_PBAP_PCE_RSV_LEN_SEND_RESOPNSE to reserve the length after
 *  it has been allocated and before it be used.
 *  Application can use BT_PBAP_ADD_xxx to add application parameters and body into tX net buffer.
 *  Setting @param wait to true is to ask the PCE to wait after sending its next response.
 *  @note the data size of the net_buf is managed by application. Application need to
 *  make sure the data size doesn't exceed the maximum packet length.
 *
 *  @param pbap_pse   PBAP PSE object.
 *  @param result     response code: BT_PBAP_SUCCESS_RSP or  BT_PBAP_CONTINUE_RSP in case of success or otherwise in case of error.
 *  @param buf        tX net buffer from pbap_pse
 *  @param wait       true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_pbap_pse_pull_phonebook_response(struct bt_pbap_pse *pbap_pse, uint8_t result, struct net_buf *buf, bool wait);

/** @brief PBAP PSE send response when receiving set phonebook path request from PCE.
 *
 *  This function is to send response when receiving set phonebook path request from PCE.
 *  When receiving request, the callback that is registered by
 *  bt_pbap_pse_register is called.
 *
 *  @param pbap_pse  PBAP PSE object.
 *  @param result    response code: BT_PBAP_SUCCESS_RSP or  BT_PBAP_CONTINUE_RSP in case of success or otherwise in case of error.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_pbap_pse_set_phonebook_path_response(struct bt_pbap_pse *pbap_pse, uint8_t result);

/** PBAP PSE send response when receiving pull vcard listing request from PCE.
 *
 *  This function called to send response when receiving pull vcard listing request from PCE.
 *  This function is to be called after receiving pull vcard listing request.
 *  This function can be called multiple times untill reponse is send completely.
 *  Application uses @param flag to send the chunked packet.
 *  The tx net buf needs to use BT_PBAP_PCE_RSV_LEN_SEND_RESOPNSE to reserve the length after
 *  it has been allocated and before it be used.
 *  Application can use BT_PBAP_ADD_xxx to add application parameters and body into tx net buffer.
 *  Setting @param wait to true is to ask the PCE to wait after sending its next response.
 *  @note the data size of the net_buf is managed by application. Application need to
 *  make sure the data size doesn't exceed the maximum packet length.
 *
 *  @param pbap_pse   PBAP PSE object.
 *  @param result     response code: BT_PBAP_SUCCESS_RSP or  BT_PBAP_CONTINUE_RSP in case of success or otherwise in case of error.
 *  @param buf        tX net buffer from pbap_pse
 *  @param wait       true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_pbap_pse_pull_vcard_listing_response(struct bt_pbap_pse *pbap_pse, uint8_t result, struct net_buf *buf, bool wait);

/** PBAP PSE send response when receiving pull vcard entry request from PCE.
 *
 *  This function called to send response when receiving pull vcard entry request from PCE.
 *  This function is to be called after pull vcard entry request.
 *  This function can be called multiple times untill reponse is send completely.
 *  Application uses @param flag to send the chunked packet.
 *  The tX net buf needs to use BT_PBAP_PCE_RSV_LEN_PULL_RESOPNSE to reserve the length after
 *  it has been allocated and before it be used.
 *  Application can use BT_PBAP_ADD_xxx to add application parameters and body into tX net buffer.
 *  Setting @param wait to true is to ask the PCE to wait after sending its next response.
 *  @note the data size of the net_buf is managed by application. Application need to
 *  make sure the data size doesn't exceed the maximum packet length.
 * 
 *  @param pbap_pse   PBAP PSE object.
 *  @param result     response code: BT_PBAP_SUCCESS_RSP or  BT_PBAP_CONTINUE_RSP in case of success or otherwise in case of error.
 *  @param buf        tX net buffer from pbap_pse
 *  @param wait       true - set SRMP is 1. false - exclude SRMP.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_pbap_pse_pull_vcard_entry_response(struct bt_pbap_pse *pbap_pse, uint8_t result, struct net_buf *buf, bool wait);

/** @brief Get maximum packet length
 *
 *  This function is to get the maximum packet length in PBAP OBEX connection.
 *  This function returns immediately.
 *
 *  @param pbap_pce     pbap object.
 *  @param max_pkt_len  the return maximum packet length.
 *
 *  @return             0 in case of success or otherwise in case of error.
 */
int bt_pbap_pse_get_max_pkt_len(struct bt_pbap_pse *pbap_pse, uint16_t *max_pkt_len);

/** @brief Get PBAP PCE supported features
 *
 *  This function is to get PBAP PCE supported features.
 *  This function returns immediately.
 *
 *  @param pbap_pce            pbap object.
 *  @param supported_features  PBAP PCE supported feature.
 *
 *  @return             0 in case of success or otherwise in case of error.
 */
int bt_pbap_pse_get_peer_supported_features(struct bt_pbap_pse *pbap_pse, uint32_t *supported_features);

/**
 * @brief Helper for parsing application parameters.
 *
 * A helper for parsing the application parameters. The most
 * common scenario is to call this helper on the application parameters
 * received in the callback that was given to bt_pbap_register.
 *
 * @param buf       net buffer returned in the callback registered by bt_pbap_register.
 * @param func      Callback function which will be called for each tag
 *                  that's found in the buffer. The callback should return
 *                  true to continue parsing, or false to stop parsing.
 * @param user_data User data to be passed to the callback registered by bt_pbap_register.
 */
#define bt_pbap_pse_app_param_parse(buf, func, user_data) bt_obex_app_param_parse(buf, func, user_data)

#ifdef __cplusplus
}
#endif
/**
 * @}
 */

#endif /*ZEPHYR_INCLUDE_BLUETOOTH_PBAP_PSE_H_*/
