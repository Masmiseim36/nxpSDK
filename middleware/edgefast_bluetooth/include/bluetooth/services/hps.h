/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_HPS_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_HPS_H_

/**
 * @brief HTTP Proxy Service (HPS)
 * @defgroup bt_hps HTTP Proxy Service (HPS)
 * @ingroup bluetooth
 * @{
 *
 * [Experimental] Users should note that the APIs can change
 * as a part of ongoing development.
 */

#define MAX_URI_LEN         512
#define MAX_HEADERS_LEN     512
#define MAX_BODY_LEN        512

enum
{
    HPS_HEADERS_RECEIVED      = 0x01,
    HPS_HEADERS_TRUNCATED     = 0x02,
    HPS_BODY_RECEIVED         = 0x04,
    HPS_BODY_TRUNCATED        = 0x08,
};

enum
{
    HTTP_GET_REQ = 0x01,
    HTTP_HEAD_REQ,
    HTTP_POST_REQ,
    HTTP_PUT_REQ,
    HTTP_DELETE_REQ,
    HTTPS_GET_REQ,
    HTTPS_HEAD_REQ,
    HTTPS_POST_REQ,
    HTTPS_PUT_REQ,
    HTTPS_DELETE_REQ,
    HTTP_REQ_CANCEL,  
};

enum
{
    IDLE_STATE = 0x00,
    BUSY_STATE
};

enum
{
    URI_SET     = 0x01,
    HEADERS_SET = 0x02,
    BODY_SET    = 0x04
};

enum
{
    HPS_ERR_INVALID_REQUEST              = 0x81,
    HPS_ERR_CCCD_IMPROPERLY_CONFIGURED   = 0xFD,
    HPS_ERR_PROC_ALREADY_IN_PROGRESS     = 0xFE,
};

enum
{
    HTTPS_CERTIFICATE_INVALID = 0x00,
    HTTPS_CERTIFICATE_VALID   = 0x01
};

typedef uint8_t hps_data_status_t;
typedef uint8_t hps_http_command_t;
typedef uint8_t hps_state_t;
typedef uint8_t hps_flags_t;

typedef struct
{
    uint8_t               http_status[2];
    hps_data_status_t     data_status;
} hps_status_t;

typedef struct
{
    uint8_t                 uri_len;
    uint8_t                 uri[MAX_URI_LEN];
    uint16_t                http_headers_len;
    uint8_t                 http_headers[MAX_HEADERS_LEN];
    uint16_t                http_body_len;
    uint8_t                 http_body[MAX_BODY_LEN];
    hps_status_t            status_code;
    hps_http_command_t      command;
    hps_state_t             state;
    hps_flags_t             flags;    
} hps_config_t;

#ifdef __cplusplus
extern "C" {
#endif


 /** @brief HTTP Headers GATT write callback.
 *
 *  If called with conn == NULL, it is a local write.
 *
 *  @return Number of bytes written.
 */
ssize_t write_http_headers(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

 /** @brief HTTP Entity Body GATT write callback.
 *
 *  If called with conn == NULL, it is a local write.
 *
 *  @return Number of bytes written.
 */
ssize_t write_http_entity_body(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

 /** @brief HTTP Proxy Server initialization
 *
 *  @return Zero in case of success and error code in case of error.
 */
int bt_hps_init(osa_msgq_handle_t queue);

 /** @brief Sets Status Code after HTTP request was fulfilled.
 */
void bt_hps_set_status_code(uint16_t http_status);


 /** @brief Notify HTTP Status after Request was fulfilled.
 *
 * This will send a GATT notification to the subscriber.
 *
 *  @return Zero in case of success and error code in case of error.
 */
int bt_hps_notify(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_HPS_H_ */
