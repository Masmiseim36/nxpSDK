/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_IPSP_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_IPSP_H_

/**
 * @brief Internet Protocol Support Profile (IPSP)
 * @defgroup bt_ipsp Internet Protocol Support Profile (IPSP)
 * @ingroup bluetooth
 * @{
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#define USER_DATA_MIN   4

typedef int (*ipsp_rx_cb_t)(struct net_buf *buf);


/** @brief Initialize the service.
 *
 * This will setup the data receive callback.
 *
 *  @param pf_rx_cb Pointer to the callback used for receiiving data.
 *
 *  @return Zero in case of success and error code in case of error.
 */
int ipsp_init(ipsp_rx_cb_t pf_rx_cb);

/** @brief Start a connection to an IPSP Node using this connection.
 *
 * This will try to connect to the Node present.
 *
 *  @param conn Pointer to the connection to be used.
 *
 *  @return Zero in case of success and error code in case of error.
 */
int ipsp_connect(struct bt_conn *conn);

/** @brief Send data to the peer IPSP Node/Router.
 *
 *  @param conn Pointer to the buffer containing data.
 *
 *  @return Zero in case of success and error code in case of error.
 */
int ipsp_send(struct net_buf *buf);

/** @brief Setup an IPSP Server.
 *
 *  @return Zero in case of success and error code in case of error.
 */
int ipsp_listen(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_HRS_H_ */
