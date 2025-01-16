/** @file
 *  @brief Phone Book Access Profile handling.
 */

/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_PBAP_PCE_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_PBAP_PCE_H_

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

/** @brief PBAP PCE structure */
struct bt_pbap_pce;

/**
 * @brief Reserve length for PullPhoneBook.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the pbap.
 *
 * @param pbap_pce  PBAP PCE object.
 * @param flags     refer to enum bt_obex_req_flags.
 *
 * @return          the reservation length.
 */
#define BT_PBAP_PCE_RSV_LEN_PULL_PHONEBOOK(pbap_pce, flags)      \
        (((flags & BT_OBEX_REQ_START) ? sizeof(struct bt_pbap_pull_phonebook_hdr) : \
        sizeof(struct bt_pbap_ops_get_cont_hdr)) + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */
          
/**
 * @brief Reserve length for SetPath.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the pbap.
 *
 * @param pbap_pce  PBAP PCE object.
 *
 * @return          the reservation length.
 */
#define BT_PBAP_PCE_RSV_LEN_SET_PATH(pbap_pce)  sizeof(struct bt_pbap_set_path_hdr)  \
        + BT_L2CAP_BUF_SIZE(2U) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/**
 * @brief Reserve length for PullvCardListing.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the pbap.
 *
 * @param pbap_pce  PBAP PCE object.
 * @param flags     refer to enum bt_obex_req_flags.
 *
 * @return          the reservation length.
 */
#define BT_PBAP_PCE_RSV_LEN_PULL_VCARD_LISTING(pbap_pce, flags)      \
        (((flags & BT_OBEX_REQ_START) ? sizeof(struct bt_pbap_pull_vcard_listing_hdr) : \
        sizeof(struct bt_pbap_ops_get_cont_hdr)) + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/** 
 * @brief Reserve length for PullvCardEntry.
 *
 * The reservation length will be as the 2nd parameter of net_buf_reserve, 
 * which is used for reserving space in front of net buffer. The reserved buffer is filled in the pbap.
 *
 * @param pbap_pce  PBAP PCE object.
 * @param flags     refer to enum bt_obex_req_flags.
 *
 * @return          the reservation length.
 */
#define BT_PBAP_PCE_RSV_LEN_PULL_VCARD_ENTRY(pbap_pce, flags)      \
        (((flags & BT_OBEX_REQ_START) ? sizeof(struct bt_pbap_pull_vcard_entry_hdr) : \
        sizeof(struct bt_pbap_ops_get_cont_hdr)) + BT_L2CAP_BUF_SIZE(2U)) /* L2CAP I-frame Enhanced Control Field(2-byte) */

/** @brief PBAP_PCE profile application callback */
struct bt_pbap_pce_cb
{
    /** PBAP_PCE connected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection completes.
     *
     *  @param pbap_pce  PBAP PCE object.
     */
    void (*connected)(struct bt_pbap_pce *pbap_pce);

    /** PBAP_PCE disconnected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  disconnection completes.
     *
     *  @param pbap_pce  PBAP PCE object.
     *  @param result    BT_PBAP_SUCCESS_RSP in case of success or reason causes of disconnection.
     */
    void (*disconnected)(struct bt_pbap_pce *pbap_pce, uint8_t result);

    /** PBAP_PCE get authentication information callback to application
     *
     * If this callback is provided it will be called whenever pse asks to authenticate pce,
     * and pce does not provide authentication information when initiating the connection.
     * The application can provide validation authenticate information in this callback.
     *
     *  @param pbap_pce          PBAP PCE object.
     *  @param pbap_atuh_info    refer to struct bt_pbap_auth.
     */
    void (*get_auth_info)(struct bt_pbap_pce *pbap_pce, struct bt_pbap_auth *pbap_atuh_info);

    /** PBAP_PCE abort callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  abort completes.
     *
     *  @param pbap_pce  PBAP_PCE object.
     *  @param result    BT_PBAP_SUCCESS_RSP in case of success or otherwise in case of error.
     */
    void (*abort)(struct bt_pbap_pce *pbap_pce, uint8_t result);

    /** PBAP_PCE phonebook download callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  phonebook download completes.
     *  Application can use bt_pbap_pce_app_param_parse to obtain application parameters
     *  and use bt_pbap_pce_get_body to obtain body from RX net buffer.
     *
     *  @param pbap_pce   PBAP PCE object.
     *  @param result     BT_PBAP_SUCCESS_RSP or BT_PBAP_CONTINUE_RSP in case of success or otherwise in case of error.
     *  @param buf        RX net buffer contains PBAP application parameters and body of phonebook object.
     */
    void (*pull_phonebook)(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf);

    /** PBAP_PCE set phonebook path callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  set phonebook path completes.
     *
     *  @param pbap_pce   PBAP PCE object.
     *  @param result     BT_PBAP_SUCCESS_RSP or in case of success or otherwise in case of error.
     */
    void (*set_phonebook_path)(struct bt_pbap_pce *pbap_pce, uint8_t result);

    /** PBAP_PCE get phonebook vcardlist callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  get phonebook vcardlist completes.
     *  Appplication can use bt_pbap_pce_app_param_parse to obtain application parameters
     *  and use bt_pbap_pce_get_body to obtain body from RX net buffer.
     *
     *  @param pbap_pce   PBAP PCE object.
     *  @param result     BT_PBAP_SUCCESS_RSP or BT_PBAP_CONTINUE_RSP in case of success or otherwise in case of error.
     *  @param buf        RX net buffer contains PBAP application parameters and body of vcard listing object.
     */
    void (*pull_vcard_listing)(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf);

    /** PBAP_PCE get phonebook vcard callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  get phonebook vcard completes.
     *  Appplication can use bt_pbap_pce_app_param_parse to obtain application parameters
     *  and use bt_pbap_pce_get_body to obtain body from RX net buffer.
     *
     *  @param pbap_pce   PBAP PCE object.
     *  @param result     BT_PBAP_SUCCESS_RSP or BT_PBAP_CONTINUE_RSP in case of success or otherwise in case of error.
     *  @param buf        RX net buffer contains PBAP application parameters and body of vcard entry object.
     */
    void (*pull_vcard_entry)(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf);
};

/** PBAP PCE register
 *
 *  This function called to initialize pbap_pce and register callback.
 *
 *  @return 0 in case of success or otherwise in case of error.
 */
int bt_pbap_pce_register(struct bt_pbap_pce_cb *cb);

/** @brief pbap pce Connect over rfcomm .
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to establish pbap
 *  connection based on RFCOM channel between devices.
 *  After connection success, the callback that is registered by
 *  bt_pbap_pce_register is called.
 *  If want to initiate authentication to pse, please input @param auth.
 *  If auth is be provided, its memory cannot be released during the entire connection hold process.
 *  If pce has not negotiated authentication information with pse,
 *  it is recommended not to initiate authentication.
 *  For example, pse is a phone.
 *
 *  @param conn          Pointer to bt_conn structure.
 *  @param channel       RFCOM channel number, returned in SDP record
 *  @param auth          If want to authenticate pse, provide auth, refer to struct bt_pbap_auth.
 *  @param peer_feature  pbap_pse support feature.
 *  @param pbap_pce      Pointer to pointer of bt_pbap connection object
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_pbap_pce_scn_connect(struct bt_conn *conn, uint8_t channel, struct bt_pbap_auth *auth, uint32_t peer_feature, struct bt_pbap_pce **pbap_pce);

/** @brief pbap pce Connect over l2cap .
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to establish pbap pce
 *  connection between devices.
 *  This function can establish l2cap_psm connection.
 *  After connection success, the callback that is registered by
 *  bt_pbap_pce_register is called.
 *  if auth is be provided, its memory cannot be released during the entire connection hold process.
 *  If pce has not negotiated authentication information with pse,
 *  it is recommended not to initiate authentication.
 *  For example, pse is a phone.
 *
 *  @param conn          Pointer to bt_conn structure.
 *  @param psm           GoepL2capPsm, returned in SDP record
 *  @param auth          If want to authenticate pse, provide auth;
 *  @param peer_feature  pbap_pce support feature
 *  @param pbap_pce      PBAP PCE object
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_pbap_pce_psm_connect(struct bt_conn *conn, uint16_t psm, struct bt_pbap_auth *auth, uint32_t peer_feature, struct bt_pbap_pce **pbap_pce);

/** @brief pbap pce disconnect.
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to disconnect pbap
 *  connection between devices.
 *  After disconnect, the callback that is registered by
 *  bt_pbap_pce_register is called.
 *
 *  @param pbap_pce  PBAP PCE object
 *
 *  @return          0 in case of success or otherwise in case
 *  of error.
 */
int bt_pbap_pce_disconnect(struct bt_pbap_pce *pbap_pce);

/** @brief pbap pce get phonebook request.
 *
 *  This function is to be called after the pabp conntion is established.
 *  This function can be called multiple times untill GET request is send completely.
 *  Application uses @param flag to send the chunked packet. It's recommended to intelligently allocate
 *  tX buffer size to put GET request in the single packet.
 *  This function can be called multiple times till the Phonebook object is retrieved completely.
 *  The tX net buf needs to use BT_PBAP_PCE_RSV_LEN_PULL_PHONEBOOK to reserve the length after 
 *  it has been allocated and before it be used.
 *  Application can use BT_PBAP_ADD_xxx to add application parameters into tX net buffer.
 *  Setting @param wait to true is to ask the pse to wait after sending its next response.
 *  After receive responese, the callback that is registered by bt_pbap_pce_register is called.
 *
 *  @param pbap_pce  PBAP PCE object
 *  @param buf       tx net_buf
 *  @param name      Specific phone book path names
 *  @param wait      true - set SRMP is 1. false - exclude SRMP.
 *  @param flag      refer to enum bt_obex_req_flags.
 * 
 *  @return          0 in case of success or otherwise in case of error.
 */
int bt_pbap_pce_pull_phonebook(struct bt_pbap_pce *pbap_pce, struct net_buf *buf, char *name, bool wait, enum bt_obex_req_flags flag);

/** @brief pbap pce phone book path set.
 *
 *  This function is to be called after the pabp conn is established.
 *  This API is to be used to set sets the current folder in pse.
 *  When name is "/", go to parent directory.
 *  When name is ".." or "../", go up one level.
 *  When name is "child" or "./child", go to child
 *  For multilevel jumps, need to do it on a level-by-level basis
 *  After receive responese, the callback that is registered by
 *  bt_pbap_pce_register is called.
 *  The tx net_buf needs to use  BT_PBAP_PCE_RSV_LEN_SET_PATH to reserve the length after 
 *  it has been allocated and before it be used.
 *
 *  @param pbap_pce  PBAP PCE object
 *  @param buf       tx net_buf
 *  @param name      path name string.
 *
 *  @return          0 in case of success or otherwise in case of error.
 */
int bt_pbap_pce_set_phonebook_path(struct bt_pbap_pce *pbap_pce, struct net_buf *buf, char *name);

/** @brief pbap pce get vcardlisting request
 *
 *  This function is to be called after the pabp connction is established.
 *  This function can be called multiple times untill GET request is send completely.
 *  Application uses @param flag to send the chunked packet. It's recommended to intelligently allocate
 *  tX buffer size to put GET request in the single packet.
 *  This function can be called multiple times till the vcardlisting object is retrieved completely.
 *  The tX net buf needs to use BT_PBAP_PCE_RSV_LEN_PULL_VCARD_LISTING to reserve the length after 
 *  it has been allocated and before it be used.
 *  Application can use BT_PBAP_ADD_xxx to add application parameters into tx net buffer.
 *  Setting @param wait to true is to ask the pse to wait after sending its next response.
 *  After receive responese, the callback that is registered by bt_pbap_pce_register is called.
 *
 *  @param pbap_pce  pointer to bt pbap pce connection object.
 *  @param buf       tx net_buf.
 *  @param name      Specific vcard listing name.
 *  @param wait      true - set SRMP is 1. false - exclude SRMP.
 *  @param flag      refer to enum bt_obex_req_flags.
 *
 *  @return          0 in case of success or otherwise in case
 *  of error.
 */
int bt_pbap_pce_pull_vcard_listing(struct bt_pbap_pce *pbap_pce, struct net_buf *buf, char *name, bool wait, enum bt_obex_req_flags flag);

/** @brief pbap pce get vcard entry request.
 *
 *  This function is to be called after the pabp connection is established.
 *  This function can be called multiple times till GET request is send completely.
 *  Application uses @param flag to send the chunked packet. It's recommended to intelligently allocate
 *  tX buffer size to put GET request in the single packet.
 *  This function can be called multiple times till the vcard entry object is retrieved completely.
 *  The tx net buf needs to use BT_PBAP_PCE_RSV_LEN_PULL_VCARD_ENTRY to reserve the length after 
 *  it has been allocated and before it be used.
 *  Application can use BT_PBAP_ADD_xxx to add application parameters into tx net buffer.
 *  Setting @param wait to true is to ask the pse to wait after sending its next response.
 *  After receive responese, the callback that is registered by bt_pbap_pce_register is called.
 *
 *  @param pbap_pce  pointer to bt pbap pce connection object
 *  @param buf       tx net_buf
 *  @param name      Specific vcard entry name
 *  @param wait      Reserved for future use. true - the remote device sets SRMP to 1. false - the remote device don't set SRMP.
 *  @param flag      refer to enum bt_obex_req_flags.
 *
 *  @return          0 in case of success or otherwise in case
 *  of error.
 */
int bt_pbap_pce_pull_vcard_entry(struct bt_pbap_pce *pbap_pce, struct net_buf *buf, char *name, bool wait, enum bt_obex_req_flags flag);

/** @brief Abort PBAP_PCE operation
 *
 *  Abort PBAP_PCE GET operation. This cancels the current outstanding operation.
 *  The return value of -EINPROGRESS means abort is queued and pending. The current
 *  outstanding operation will be aborted when receiving next response from the pse.
 *  After abort, the callback that is registered by bt_pbap_pce_register is called.
 *
 *  @param mce_mas  PBAP_PCE object.
 *
 *  @return         0 in case of success or -EINPROGRESS in case abort is queued or otherwise in case of error.
 */
int bt_pbap_pce_abort(struct bt_pbap_pce *pbap_pce);

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
int bt_pbap_pce_get_max_pkt_len(struct bt_pbap_pce *pbap_pce, uint16_t *max_pkt_len);

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
#define bt_pbap_pce_app_param_parse(buf, func, user_data) bt_obex_app_param_parse(buf, func, user_data)

/**
 * @brief Helper for getting body.
 *
 * A helper for getting Body header. The most
 * common scenario is to call this helper on the body received in
 * the callback that was given to bt_pbap_register.
 *
 * @param buf       net buffer returned in the callback registered by bt_pbap_register.
 * @param body      pointer used for holding body data.
 * @param length    the length of body data.
 */
#define bt_pbap_pce_get_body(buf, body, length) bt_obex_get_body(buf, body, length)
#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /*ZEPHYR_INCLUDE_BLUETOOTH_PBAP_PCE_H_*/
