/** @file
 *  @brief Bluetooth SPP handling
 */

/*
 * Copyright (c) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SPP_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SPP_H_

/**
 * @brief Serial Port Profile (SPP)
 * @defgroup bt_spp Serial Port Profile (SPP)
 * @ingroup bluetooth
 * @{
 */

#include <bluetooth/conn.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief SPP Generic Data Element Value. */
struct bt_spp;

/*! @brief spp application callback function */
typedef struct _bt_spp_callback
{
    void (*connected)(struct bt_spp *spp, int error);
    void (*disconnected)(struct bt_spp *spp, int error);
    void (*data_received)(struct bt_spp *spp, uint8_t *data, uint16_t len, int error);
    void (*data_sent)(struct bt_spp *spp, uint8_t *data, uint16_t len, int error);
}bt_spp_callback;

/*! @brief spp sdp discover callback function */
typedef int (*bt_spp_discover_callback)(struct bt_conn *conn, uint8_t count, uint16_t *channel);

/*! @brief bt_spp_discover callback parameter */
typedef struct
{
    bt_spp_discover_callback cb;
    struct bt_conn *conn;
    void *param;
    void *next;
} discover_cb_t;

/* SPP API */

/** @brief Register a SPP server
 *
 *  Register a SPP server channel, wait for spp connection from SPP client.
 *  Once it's connected by spp client, will notify application by calling cb->connected.
 *
 *  @param channel Registered server channel.
 *
 *  @param cb      Application callback.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_server_register(uint8_t channel, bt_spp_callback *cb);


/** @brief Discover SPP server channel
 *
 *  Discover peer SPP server channel after basic BR connection is created.
 *  Will notify application discover results by calling cb->cb.
 *
 *  @param conn BR connection handle.
 *
 *  @param cb   Discover callback.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_discover(struct bt_conn *conn, discover_cb_t *cb);

/** @brief Connect SPP server channel
 *
 *  Create SPP connection with remote SPP server channel.
 *  Once connection is created successfully, will notify application by calling cb->connected.
 *
 *  @param conn    Conn handle created with remote device.
 *
 *  @param channel Remote server channel to be connected, if it's 0, will connect remote BT_RFCOMM_CHAN_SPP channel.
 *
 *  @param cb      Application callback.
 *
 *  @param spp     SPP handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_client_connect(struct bt_conn *conn, uint8_t channel, bt_spp_callback *cb, struct bt_spp **spp);

/** @brief Send data to peer SPP device
 *
 *  Send data to connected peer spp.
 *  Once data is sent, will notify application by calling cb->data_sent, which is provided by bt_spp_server_register or bt_spp_client_connect.
 *  If peer spp receives data, will notify application by calling cb->data_received.
 *
 *  @param spp  SPP handle.
 *  @param data Data buffer.
 *  @param len  Data length.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_data_send(struct bt_spp *spp, uint8_t *data, uint16_t len);

/** @brief Disconnect SPP connection
 *
 *  Disconnect SPP connection.
 *  Once connection is disconnected, will notify application by calling cb->disconnected, which is provided by bt_spp_server_register or bt_spp_client_connect.
 *
 *  @param spp SPP handle.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_spp_disconnect(struct bt_spp *spp);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SPP_H_ */
