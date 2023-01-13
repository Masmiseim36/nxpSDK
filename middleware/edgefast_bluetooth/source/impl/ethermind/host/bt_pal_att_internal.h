/* bt_pal_att_internal.h - Attribute protocol handling */

/*
 * Copyright 2021 NXP
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __ATT_INTERNAL_H__
#define __ATT_INTERNAL_H__

#define BT_EATT_PSM		0x27
#define BT_ATT_DEFAULT_LE_MTU	23
#define BT_ATT_TIMEOUT		BT_SECONDS(30)
#define BT_ATT_REQ_SENT_TIMEOUT		BT_MSEC(1)

/* ATT MTU must be equal for RX and TX, so select the smallest value */
#define BT_ATT_MTU (MIN(BT_L2CAP_RX_MTU, BT_L2CAP_TX_MTU))

STRUCT_PACKED_PRE
struct bt_att_hdr {
	uint8_t  code;
} STRUCT_PACKED_POST;

#define BT_ATT_OP_ERROR_RSP			0x01
STRUCT_PACKED_PRE
struct bt_att_error_rsp {
	uint8_t  request;
	uint16_t handle;
	uint8_t  error;
} STRUCT_PACKED_POST;

#define BT_ATT_OP_MTU_REQ			0x02
STRUCT_PACKED_PRE
struct bt_att_exchange_mtu_req {
	uint16_t mtu;
} STRUCT_PACKED_POST;

#define BT_ATT_OP_MTU_RSP			0x03
STRUCT_PACKED_PRE
struct bt_att_exchange_mtu_rsp {
	uint16_t mtu;
} STRUCT_PACKED_POST;

/* Find Information Request */
#define BT_ATT_OP_FIND_INFO_REQ			0x04
STRUCT_PACKED_PRE
struct bt_att_find_info_req {
	uint16_t start_handle;
	uint16_t end_handle;
} STRUCT_PACKED_POST;

/* Format field values for BT_ATT_OP_FIND_INFO_RSP */
#define BT_ATT_INFO_16				0x01
#define BT_ATT_INFO_128				0x02
STRUCT_PACKED_PRE
struct bt_att_info_16 {
	uint16_t handle;
	uint16_t uuid;
} STRUCT_PACKED_POST;

STRUCT_PACKED_PRE
struct bt_att_info_128 {
	uint16_t handle;
	uint8_t  uuid[16];
} STRUCT_PACKED_POST;

/* Find Information Response */
#define BT_ATT_OP_FIND_INFO_RSP			0x05
STRUCT_PACKED_PRE
struct bt_att_find_info_rsp {
	uint8_t  format;
	uint8_t  info[0];
} STRUCT_PACKED_POST;

/* Find By Type Value Request */
#define BT_ATT_OP_FIND_TYPE_REQ			0x06
STRUCT_PACKED_PRE
struct bt_att_find_type_req {
	uint16_t start_handle;
	uint16_t end_handle;
	uint16_t type;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

STRUCT_PACKED_PRE
struct bt_att_handle_group {
	uint16_t start_handle;
	uint16_t end_handle;
} STRUCT_PACKED_POST;

/* Find By Type Value Response */
#define BT_ATT_OP_FIND_TYPE_RSP			0x07
STRUCT_PACKED_PRE
struct bt_att_find_type_rsp {
	struct bt_att_handle_group list[1];
} STRUCT_PACKED_POST;

/* Read By Type Request */
#define BT_ATT_OP_READ_TYPE_REQ			0x08
STRUCT_PACKED_PRE
struct bt_att_read_type_req {
	uint16_t start_handle;
	uint16_t end_handle;
	uint8_t  uuid[0];
} STRUCT_PACKED_POST;

STRUCT_PACKED_PRE
struct bt_att_data {
	uint16_t handle;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

/* Read By Type Response */
#define BT_ATT_OP_READ_TYPE_RSP			0x09
STRUCT_PACKED_PRE
struct bt_att_read_type_rsp {
	uint8_t  len;
	uint8_t data[0];
} STRUCT_PACKED_POST;

/* Read Request */
#define BT_ATT_OP_READ_REQ			0x0a
STRUCT_PACKED_PRE
struct bt_att_read_req {
	uint16_t handle;
} STRUCT_PACKED_POST;

/* Read Response */
#define BT_ATT_OP_READ_RSP			0x0b
STRUCT_PACKED_PRE
struct bt_att_read_rsp {
	uint8_t  value[1];
} STRUCT_PACKED_POST;

/* Read Blob Request */
#define BT_ATT_OP_READ_BLOB_REQ			0x0c
STRUCT_PACKED_PRE
struct bt_att_read_blob_req {
	uint16_t handle;
	uint16_t offset;
} STRUCT_PACKED_POST;

/* Read Blob Response */
#define BT_ATT_OP_READ_BLOB_RSP			0x0d
STRUCT_PACKED_PRE
struct bt_att_read_blob_rsp {
	uint8_t  value[1];
} STRUCT_PACKED_POST;

/* Read Multiple Request */
#define BT_ATT_READ_MULT_MIN_LEN_REQ		0x04

#define BT_ATT_OP_READ_MULT_REQ			0x0e
STRUCT_PACKED_PRE
struct bt_att_read_mult_req {
	uint16_t handles[1];
} STRUCT_PACKED_POST;

/* Read Multiple Response */
#define BT_ATT_OP_READ_MULT_RSP			0x0f
STRUCT_PACKED_PRE
struct bt_att_read_mult_rsp {
	uint8_t  value[1];
} STRUCT_PACKED_POST;

/* Read by Group Type Request */
#define BT_ATT_OP_READ_GROUP_REQ		0x10
STRUCT_PACKED_PRE
struct bt_att_read_group_req {
	uint16_t start_handle;
	uint16_t end_handle;
	uint8_t  uuid[0];
} STRUCT_PACKED_POST;

STRUCT_PACKED_PRE
struct bt_att_group_data {
	uint16_t start_handle;
	uint16_t end_handle;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

/* Read by Group Type Response */
#define BT_ATT_OP_READ_GROUP_RSP		0x11
STRUCT_PACKED_PRE
struct bt_att_read_group_rsp {
	uint8_t  len;
	uint8_t  data[0];
} STRUCT_PACKED_POST;

/* Write Request */
#define BT_ATT_OP_WRITE_REQ			0x12
STRUCT_PACKED_PRE
struct bt_att_write_req {
	uint16_t handle;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

/* Write Response */
#define BT_ATT_OP_WRITE_RSP			0x13

/* Prepare Write Request */
#define BT_ATT_OP_PREPARE_WRITE_REQ		0x16
STRUCT_PACKED_PRE
struct bt_att_prepare_write_req {
	uint16_t handle;
	uint16_t offset;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

/* Prepare Write Respond */
#define BT_ATT_OP_PREPARE_WRITE_RSP		0x17
STRUCT_PACKED_PRE
struct bt_att_prepare_write_rsp {
	uint16_t handle;
	uint16_t offset;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

/* Execute Write Request */
#define BT_ATT_FLAG_CANCEL			0x00
#define BT_ATT_FLAG_EXEC			0x01

#define BT_ATT_OP_EXEC_WRITE_REQ		0x18
STRUCT_PACKED_PRE
struct bt_att_exec_write_req {
	uint8_t  flags;
} STRUCT_PACKED_POST;

/* Execute Write Response */
#define BT_ATT_OP_EXEC_WRITE_RSP		0x19

/* Handle Value Notification */
#define BT_ATT_OP_NOTIFY			0x1b
STRUCT_PACKED_PRE
struct bt_att_notify {
	uint16_t handle;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

/* Handle Value Indication */
#define BT_ATT_OP_INDICATE			0x1d
STRUCT_PACKED_PRE
struct bt_att_indicate {
	uint16_t handle;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

/* Handle Value Confirm */
#define BT_ATT_OP_CONFIRM			0x1e
STRUCT_PACKED_PRE
struct bt_att_signature {
	uint8_t  value[12];
} STRUCT_PACKED_POST;

#define BT_ATT_OP_READ_MULT_VL_REQ		0x20
STRUCT_PACKED_PRE
struct bt_att_read_mult_vl_req {
	uint16_t handles[1];
} STRUCT_PACKED_POST;

/* Read Multiple Response */
#define BT_ATT_OP_READ_MULT_VL_RSP		0x21
STRUCT_PACKED_PRE
struct bt_att_read_mult_vl_rsp {
	uint16_t len;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

/* Handle Multiple Value Notification */
#define BT_ATT_OP_NOTIFY_MULT			0x23
STRUCT_PACKED_PRE
struct bt_att_notify_mult {
	uint16_t handle;
	uint16_t len;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

/* Write Command */
#define BT_ATT_OP_WRITE_CMD			0x52
STRUCT_PACKED_PRE
struct bt_att_write_cmd {
	uint16_t handle;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

/* Signed Write Command */
#define BT_ATT_OP_SIGNED_WRITE_CMD		0xd2
STRUCT_PACKED_PRE
struct bt_att_signed_write_cmd {
	uint16_t handle;
	uint8_t  value[0];
} STRUCT_PACKED_POST;

typedef void (*bt_att_func_t)(struct bt_conn *conn, uint8_t err,
			      const void *pdu, uint16_t length,
			      void *user_data);

typedef int (*bt_att_encode_t)(struct net_buf *buf, size_t len,
			       void *user_data);

/* ATT request context */
struct bt_att_req {
	sys_snode_t node;
	bt_att_func_t func;
	struct net_buf *buf;
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
	bt_att_encode_t encode;
	uint8_t retrying : 1;
	uint8_t att_op;
	size_t len;
#endif /* CONFIG_BT_SMP */
	void *user_data;
};

void att_sent(struct bt_conn *conn, void *user_data);

void bt_att_init(void);
uint16_t bt_att_get_mtu(struct bt_conn *conn);
struct net_buf *bt_att_create_pdu(struct bt_conn *conn, uint8_t op,
				  size_t len);

/* Allocate a new request */
struct bt_att_req *bt_att_req_alloc(k_timeout_t timeout);

/* Free a request */
void bt_att_req_free(struct bt_att_req *req);

/* att Handle lookup by conn */
ATT_HANDLE *bt_att_lookup_conn(struct bt_conn *conn);

/* Send ATT PDU over a connection */
int bt_att_send(struct bt_conn *conn, struct net_buf *buf);

/* Send ATT Request over a connection */
int bt_att_req_send(struct bt_conn *conn, struct bt_att_req *req);

/* Cancel ATT request */
void bt_att_req_cancel(struct bt_conn *conn, struct bt_att_req *req);

/* Disconnect EATT channels */
int bt_eatt_disconnect(struct bt_conn *conn);

/** @brief Find a pending ATT request by its user_data pointer.
 *  @param conn The connection the request was issued on.
 *  @param user_data The pointer value to look for.
 *  @return The found request. NULL if not found.
 */
struct bt_att_req *bt_att_find_req_by_user_data(struct bt_conn *conn, const void *user_data);

/* Checks if only the fixed ATT channel is connected */
bool bt_att_fixed_chan_only(struct bt_conn *conn);

/* Clear the out of sync flag on all channels */
void bt_att_clear_out_of_sync_sent(struct bt_conn *conn);

/* Check if BT_ATT_ERR_DB_OUT_OF_SYNC has been sent on the fixed ATT channel */
bool bt_att_out_of_sync_sent_on_fixed(struct bt_conn *conn);

#if 0 /* Include gatt.h to walkaround bt_gatt_complete_func_t duplicate definiation warning. */
typedef void (*bt_gatt_complete_func_t) (struct bt_conn *conn, void *user_data);
#else
#include "bluetooth/gatt.h"
#endif
void bt_att_set_tx_meta_data(struct net_buf *buf, bt_gatt_complete_func_t func, void *user_data,
			     enum bt_att_chan_opt chan_opt);
void bt_att_increment_tx_meta_data_attr_count(struct net_buf *buf, uint16_t attr_count);

bool bt_att_tx_meta_data_match(const struct net_buf *buf, bt_gatt_complete_func_t func,
			       const void *user_data, enum bt_att_chan_opt chan_opt);

void bt_att_free_tx_meta_data(const struct net_buf *buf);

#if (defined(CONFIG_BT_EATT) && (CONFIG_BT_EATT > 0U))
#define BT_ATT_CHAN_OPT(_params) (_params)->chan_opt
#else
#define BT_ATT_CHAN_OPT(_params) BT_ATT_CHAN_OPT_UNENHANCED_ONLY
#endif /* CONFIG_BT_EATT */

bool bt_att_chan_opt_valid(struct bt_conn *conn, enum bt_att_chan_opt chan_opt);

#endif /* __ATT_INTERNAL_H__ */