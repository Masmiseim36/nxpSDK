/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno/errno.h>
#include <porting.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/services/hps.h>

#define LOG_LEVEL CONFIG_BT_HPS_LOG_LEVEL
#if 0
#include <logging/log.h>
LOG_MODULE_REGISTER(hps);
#endif

static osa_msgq_handle_t hps_queue;

static hps_config_t service_config;
static bool notif_enabled;

static uint8_t https_security_status = HTTPS_CERTIFICATE_INVALID;

ssize_t write_http_headers(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    /* Written by client */
    if (conn != NULL)
    {
        memset(&service_config.http_headers, 0, MAX_HEADERS_LEN);
        memcpy(&service_config.http_headers, (uint8_t*)buf, len);

        service_config.http_headers_len = len;
        service_config.flags |= HEADERS_SET;
    }
    /* Updated after processing HTTP Request */
    else
    {
        service_config.flags &= ~HEADERS_SET;
        memset(&service_config.http_headers, 0, MAX_HEADERS_LEN);
        service_config.status_code.data_status |= HPS_HEADERS_RECEIVED;

        if (len > MAX_HEADERS_LEN)
        {
            memcpy(&service_config.http_headers, buf, MAX_HEADERS_LEN);
            service_config.http_headers_len = MAX_HEADERS_LEN;
            service_config.status_code.data_status |= HPS_HEADERS_TRUNCATED;
        }
        else
        {
            memcpy(&service_config.http_headers, buf, len);
            service_config.http_headers_len = len;
            service_config.status_code.data_status &= ~HPS_HEADERS_TRUNCATED;
        }
    }
    return len;
}

ssize_t write_http_entity_body(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    /* Written by client */
    if (conn != NULL)
    {
        memset(&service_config.http_body, 0, MAX_BODY_LEN);
        memcpy(&service_config.http_body, (uint8_t*)buf, len);

        service_config.http_body_len = len;
        service_config.flags |= BODY_SET;
    }
    /* Updated after processing HTTP Request */
    else
    {
        service_config.flags &= ~BODY_SET;
        memset(&service_config.http_body, 0, MAX_BODY_LEN);
        service_config.status_code.data_status |= HPS_BODY_RECEIVED;

        if (len > MAX_BODY_LEN)
        {
            memcpy(&service_config.http_body, buf, MAX_BODY_LEN);
            service_config.http_body_len = MAX_BODY_LEN;
            service_config.status_code.data_status |= HPS_BODY_TRUNCATED;
        }
        else
        {
            memcpy(&service_config.http_body, buf, len);
            service_config.http_body_len = len;
            service_config.status_code.data_status &= ~HPS_BODY_TRUNCATED;
        }
    }
    return len;
}

static void httpsc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);

	notif_enabled = (value == BT_GATT_CCC_NOTIFY);

	/* BT_INFO("HTTP Status Code notifications %s", notif_enabled ? "enabled" : "disabled"); */
    (void)notif_enabled;
}

static ssize_t read_http_headers(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &service_config.http_headers,
                             service_config.http_headers_len);
}

static ssize_t read_http_entity_body(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &service_config.http_body,
                             service_config.http_body_len);
}

static ssize_t read_https_security(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
    uint8_t status = https_security_status;
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &status, sizeof(uint8_t));
}

static ssize_t write_uri(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    memset(&service_config.uri, 0, MAX_URI_LEN);
    memcpy(&service_config.uri, (uint8_t*)buf, len);
    service_config.uri_len = len;
    service_config.flags |= URI_SET;
    return len;
}

static ssize_t write_http_control_point(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    service_config.command = ((uint8_t*)buf)[0];

    if ((service_config.flags & (URI_SET | HEADERS_SET | BODY_SET)) == 0)
    {
        return BT_GATT_ERR(HPS_ERR_INVALID_REQUEST);
    }

    if (service_config.command > HTTP_REQ_CANCEL)
    {
        return BT_GATT_ERR(HPS_ERR_INVALID_REQUEST);
    }

    if (notif_enabled == false)
    {
        return BT_GATT_ERR(HPS_ERR_CCCD_IMPROPERLY_CONFIGURED);
    }

    if (service_config.state == BUSY_STATE)
    {
        return BT_GATT_ERR(HPS_ERR_PROC_ALREADY_IN_PROGRESS);
    }
    else
    {
        service_config.state = BUSY_STATE;
        (void)OSA_MsgQPut(hps_queue, (void*)&service_config);
        return len;
    }
}

/* HTTP Proxy Service Declaration */
static BT_GATT_SERVICE_DEFINE(hps_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_HPS),
	BT_GATT_CHARACTERISTIC(BT_UUID_URI, BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_WRITE, NULL, write_uri, NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_HTTP_HEADERS, BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_http_headers, write_http_headers, NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_HTTP_ENTITY_BODY, BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_http_entity_body, write_http_entity_body, NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_HTTP_CONTROL_POINT, BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_WRITE, NULL, write_http_control_point, NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_HTTP_STATUS_CODE, BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_NONE, NULL, NULL, NULL),
	BT_GATT_CCC(httpsc_ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_HTTPS_SECURITY, BT_GATT_CHRC_READ,
			       BT_GATT_PERM_READ, read_https_security, NULL, NULL),
);

int bt_hps_init(osa_msgq_handle_t queue)
{
    int err = 0;

    hps_queue = queue;

    service_config.state = IDLE_STATE;

    return err;
}

void bt_hps_set_status_code(uint16_t http_status)
{
    sys_put_le16(http_status, service_config.status_code.http_status);
    service_config.flags &= ~URI_SET;
    service_config.state = IDLE_STATE;

    bt_hps_notify();
}

int bt_hps_notify(void)
{
    int rc;

	rc = bt_gatt_notify(NULL, &hps_svc.attrs[10], &service_config.status_code, sizeof(service_config.status_code));

	return rc == -ENOTCONN ? 0 : rc;
}