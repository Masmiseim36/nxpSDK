/* att.c - Attribute protocol handling */

/*
 * Copyright 2021 NXP
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "porting.h"
#include <string.h>
#include <errno/errno.h>
#include <stdbool.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>

#include "BT_att_api.h"
#include "BT_gatt_db_api.h"

#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_ATT)
#define LOG_MODULE_NAME bt_att
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#include "bt_pal_hci_core.h"
#include "bt_pal_conn_internal.h"
#include "bt_pal_l2cap_internal.h"
#include "bt_pal_smp.h"
#include "bt_pal_att_internal.h"
#include "bt_pal_gatt_internal.h"

#define ATT_CHAN(_ch) CONTAINER_OF((_ch), struct bt_att_chan, chan.chan)
#define ATT_REQ(_node) CONTAINER_OF((_node), struct bt_att_req, node)

#define ATT_CMD_MASK				0x40


#ifndef BT_ATT_MAX_GROUP_COUNT
#define BT_ATT_MAX_GROUP_COUNT 4u
#endif /* BT_ATT_MAX_RSP_LENGTH */

#ifndef BT_ATT_MAX_LIST_COUNT
#define BT_ATT_MAX_LIST_COUNT 4u
#endif /* BT_ATT_MAX_RSP_LENGTH */

#ifndef BT_ATT_MAX_MULTIPLE_READ_COUNT
#define BT_ATT_MAX_MULTIPLE_READ_COUNT 4u
#endif /* BT_ATT_MAX_MULTIPLE_READ_COUNT */

#if (defined(CONFIG_BT_EATT) && ((CONFIG_BT_EATT) > 0U))
#define ATT_CHAN_MAX				(CONFIG_BT_EATT_MAX + 1)
#else
#define ATT_CHAN_MAX				1
#endif /* CONFIG_BT_EATT */

typedef enum {
		ATT_COMMAND,
		ATT_REQUEST,
		ATT_RESPONSE,
		ATT_NOTIFICATION,
		ATT_CONFIRMATION,
		ATT_INDICATION,
		ATT_UNKNOWN,
} att_type_t;

static att_type_t att_op_get_type(uint8_t op);

#if CONFIG_BT_ATT_PREPARE_COUNT > 0
struct bt_attr_data {
	uint16_t handle;
	uint16_t offset;
};

/* Pool for incoming ATT packets */
NET_BUF_POOL_DEFINE(prep_pool, CONFIG_BT_ATT_PREPARE_COUNT, BT_ATT_MTU,
		    sizeof(struct bt_attr_data), NULL);
#endif /* CONFIG_BT_ATT_PREPARE_COUNT */

/* Pool for incoming ATT packets */
NET_BUF_POOL_DEFINE(att_pool, CONFIG_BT_ATT_RX_MAX, BT_ATT_MTU,
		    CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

static osa_msgq_handle_t req_slab;
static OSA_MSGQ_HANDLE_DEFINE(req_slab_handle, CONFIG_BT_ATT_TX_MAX, sizeof(void*));

static struct bt_att_req req_slab_pool[CONFIG_BT_ATT_TX_MAX];

enum {
	ATT_PENDING_RSP,
	ATT_PENDING_CFM,
	ATT_DISCONNECTED,
	ATT_ENHANCED,
	ATT_PENDING_SENT,

	/* Total number of flags - must be at the end of the enum */
	ATT_NUM_FLAGS,
};

/* ATT channel specific data */
struct bt_att_chan {
	/* Connection this channel is associated with */
	struct bt_att		*att;
	struct bt_l2cap_le_chan	chan;
	ATOMIC_DEFINE(flags, ATT_NUM_FLAGS);
	struct bt_att_req	*req;
	osa_msgq_handle_t tx_queue;
    OSA_MSGQ_HANDLE_DEFINE(tx_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void *));
	osa_msgq_handle_t rx_queue;
    OSA_MSGQ_HANDLE_DEFINE(rx_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void *));
	ATT_HANDLE handle;
	struct k_work	rx_work;
	struct k_work_delayable	timeout_work;
	void (*sent)(struct bt_att_chan *chan);
	sys_snode_t		node;
};

/* ATT connection specific data */
struct bt_att {
	struct bt_conn		*conn;
	/* Shared request queue */
	sys_slist_t		reqs;
	osa_msgq_handle_t tx_queue;
    OSA_MSGQ_HANDLE_DEFINE(tx_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void *));
#if CONFIG_BT_ATT_PREPARE_COUNT > 0
	osa_msgq_handle_t prep_queue;
    OSA_MSGQ_HANDLE_DEFINE(prep_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void *));
#endif
	/* Contains bt_att_chan instance(s) */
	sys_slist_t		chans;
};

/* ATT handle */
struct bt_att_handle {
    ATT_HANDLE  handle;
	bool used;
};

static osa_msgq_handle_t att_slab;
static OSA_MSGQ_HANDLE_DEFINE(att_slab_handle, CONFIG_BT_MAX_CONN, sizeof(void*));
static struct bt_att att_slab_pool[CONFIG_BT_MAX_CONN];

static osa_msgq_handle_t chan_slab;
static OSA_MSGQ_HANDLE_DEFINE(chan_slab_handle, CONFIG_BT_MAX_CONN * ATT_CHAN_MAX, sizeof(void*));
static struct bt_att_chan chan_slab_pool[CONFIG_BT_MAX_CONN * ATT_CHAN_MAX];

static struct bt_att_handle att_handle_node[CONFIG_BT_MAX_CONN];

static struct bt_att_req cancel;

static atomic_t init;

static int ethermind_att_handle_alloc(ATT_HANDLE *handle);
static int ethermind_att_handle_free(ATT_HANDLE *handle);
static ATT_HANDLE *ethermind_att_handle_lookup_by_device_id(uint8_t device_id);
static int bt_att_chan_send_req(struct bt_att_chan *chan, struct net_buf *buf, bt_conn_tx_cb_t cb, void *user_data);

typedef void (*bt_att_chan_sent_t)(struct bt_att_chan *chan);

static bt_att_chan_sent_t chan_cb(struct net_buf *buf);
static bt_conn_tx_cb_t att_cb(bt_att_chan_sent_t cb);

static void bt_att_disconnected(struct bt_l2cap_chan *chan);

void att_sent(struct bt_conn *conn, void *user_data)
{
	struct bt_l2cap_chan *chan = (struct bt_l2cap_chan *)user_data;

	BT_DBG("conn %p chan %p", conn, chan);

	if (chan->ops->sent) {
		chan->ops->sent(chan);
	}
}

/* In case of success the ownership of the buffer is transferred to the stack
 * which takes care of releasing it when it completes transmitting to the
 * controller.
 *
 * In case bt_l2cap_send_cb fails the buffer state and ownership are retained
 * so the buffer can be safely pushed back to the queue to be processed later.
 */
static int chan_send(struct bt_att_chan *chan, struct net_buf *buf,
		     bt_att_chan_sent_t cb)
{
	struct bt_att_hdr *hdr;
	struct net_buf_simple_state state;
	int err;

	hdr = (struct bt_att_hdr *)buf->data;

	BT_DBG("code 0x%02x", hdr->code);

	if (IS_ENABLED(CONFIG_BT_EATT) &&
	    atomic_test_bit(chan->flags, ATT_ENHANCED)) {
		/* Check if sent is pending already, if it does it cannot be
		 * modified so the operation will need to be queued.
		 */
		if (atomic_test_and_set_bit(chan->flags, ATT_PENDING_SENT)) {
			return -EAGAIN;
		}

		chan->sent = cb ? cb : chan_cb(buf);

		if (hdr->code == BT_ATT_OP_SIGNED_WRITE_CMD) {
			return -ENOTSUP;
		}

		/* Check if the channel is ready to send in case of a request */
		if (att_op_get_type(hdr->code) == ATT_REQUEST &&
		    !atomic_test_bit(chan->chan.chan.status,
				     BT_L2CAP_STATUS_OUT)) {
			return -EAGAIN;
		}

		/* bt_l2cap_chan_send does actually return the number of bytes
		 * that could be sent immediatelly.
		 */
		err = bt_l2cap_chan_send(&chan->chan.chan, buf);
		if (err < 0) {
			return err;
		}

		return 0;
	}

	if (hdr->code == BT_ATT_OP_SIGNED_WRITE_CMD) {
		int err;

		err = bt_smp_sign(chan->att->conn, buf);
		if (err) {
			BT_ERR("Error signing data");
			net_buf_unref(buf);
			return err;
		}
	}

	net_buf_simple_save(&buf->b, &state);

	chan->sent = cb ? cb : chan_cb(buf);

	/* bt_l2cap_send_cb takes onwership of the buffer so take another
	 * reference to restore the state in case an error is returned.
	 */
	net_buf_ref(buf);
#if 0
	err = bt_l2cap_send_cb(chan->att->conn, BT_L2CAP_CID_ATT,
				net_buf_ref(buf), att_cb(chan->sent),
				&chan->chan.chan);
#else
	err = bt_att_chan_send_req(chan, buf, att_cb(chan->sent),
				&chan->chan.chan);
#endif
	if (err) {
		if (hdr->code == BT_ATT_OP_SIGNED_WRITE_CMD) {
			buf->len -= 12;
		}
		/* In case of an error has occurred restore the buffer state as
		 * the extra reference shall have prevented the buffer to be
		 * freed.
		 */
		net_buf_simple_restore(&buf->b, &state);
	} else {
		/* In case of success unref the extra reference taken */
		net_buf_unref(buf);
	}

	return err;
}

/* In case of success the ownership of the buffer is transferred to the stack
 * which takes care of releasing it when it completes transmitting to the
 * controller.
 *
 * In case bt_l2cap_send_cb fails the buffer state and ownership are retained
 * so the buffer can be safely pushed back to the queue to be processed later.
 */
static int chan_send_cb(struct bt_att_chan *chan, struct net_buf *buf,
		      bt_conn_tx_cb_t cb, void *user_data)
{
	struct bt_att_hdr *hdr;
	struct net_buf_simple_state state;
	int err;

	hdr = (struct bt_att_hdr *)buf->data;

	BT_DBG("code 0x%02x", hdr->code);

    (void)hdr;

	net_buf_simple_save(&buf->b, &state);

	chan->sent = chan_cb(buf);

	/* bt_l2cap_send_cb takes onwership of the buffer so take another
	 * reference to restore the state in case an error is returned.
	 */
	net_buf_ref(buf);

	err = bt_att_chan_send_req(chan, buf, cb,
				user_data);
	if (err) {
		/* In case of an error has occurred restore the buffer state as
		 * the extra reference shall have prevented the buffer to be
		 * freed.
		 */
		net_buf_simple_restore(&buf->b, &state);
	} else {
		/* In case of success unref the extra reference taken */
		net_buf_unref(buf);
	}

	return err;
}

static int process_queue(struct bt_att_chan *chan, osa_msgq_handle_t queue)
{
	struct net_buf *buf;
	struct net_buf *tmp_buf;
    OSA_MSGQ_HANDLE_DEFINE(tmp_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void *));
	int err;

	buf = net_buf_get(queue, osaWaitNone_c);
	if (buf) {
		err = chan_send(chan, buf, NULL);
		if (err) {
			/* Push it back if it could not be send */
            if (KOSA_StatusSuccess == OSA_MsgQCreate(tmp_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void*)))
            {
                do
                {
                    tmp_buf = net_buf_get(queue, osaWaitNone_c);
                    if (NULL != tmp_buf)
                    {
                        net_buf_put(tmp_queue_handle, tmp_buf);
                    }
                } while (NULL != tmp_buf);
                net_buf_put(queue, buf);
                do
                {
                    tmp_buf = net_buf_get(tmp_queue_handle, osaWaitNone_c);
                    if (NULL != tmp_buf)
                    {
                        net_buf_put(queue, tmp_buf);
                    }
                } while (NULL != tmp_buf);
                (void)OSA_MsgQDestroy(tmp_queue_handle);
            }
            else
            {
                net_buf_unref(buf);
            }
			return err;
		}

		return 0;
	}

	return -ENOENT;
}

/* Send requests without taking tx_sem */
static int chan_req_send(struct bt_att_chan *chan, struct bt_att_req *req)
{
	struct net_buf *buf;
	int err;

	if (chan->chan.tx.mtu < net_buf_frags_len(req->buf)) {
		return -EMSGSIZE;
	}

	BT_DBG("chan %p req %p len %lu", chan, req,
	       net_buf_frags_len(req->buf));

	chan->req = req;

	/* Release since bt_l2cap_send_cb takes ownership of the buffer */
	buf = req->buf;
	req->buf = NULL;

	err = chan_send(chan, buf, NULL);
	if (err) {
		/* We still have the ownership of the buffer */
		req->buf = buf;
	}

	return err;
}

static void bt_att_sent(struct bt_l2cap_chan *ch)
{
	struct bt_att_chan *chan = ATT_CHAN(ch);
	struct bt_att *att = chan->att;
	int err;

	BT_DBG("chan %p", chan);

	if (chan->sent) {
		chan->sent(chan);
	}

	atomic_clear_bit(chan->flags, ATT_PENDING_SENT);

	if (!att) {
		BT_DBG("Ignore sent on detached ATT chan");
		return;
	}

	/* Process pending requests first since they require a response they
	 * can only be processed one at time while if other queues were
	 * processed before they may always contain a buffer starving the
	 * request queue.
	 */
	if (!chan->req && !sys_slist_is_empty(&att->reqs)) {
		sys_snode_t *node = sys_slist_get(&att->reqs);

		if (chan_req_send(chan, ATT_REQ(node)) >= 0) {
			return;
		}

		/* Prepend back to the list as it could not be sent */
		sys_slist_prepend(&att->reqs, node);
	}

	/* Process channel queue */
	err = process_queue(chan, chan->tx_queue);
	if (!err) {
		return;
	}

	/* Process global queue */
	err = process_queue(chan, att->tx_queue);
	if (!err) {
		return;
	}
}

static void chan_cfm_sent(struct bt_att_chan *chan)
{
	BT_DBG("chan %p", chan);

	if (IS_ENABLED(CONFIG_BT_ATT_ENFORCE_FLOW)) {
		atomic_clear_bit(chan->flags, ATT_PENDING_CFM);
	}
}

static void chan_rsp_sent(struct bt_att_chan *chan)
{
	BT_DBG("chan %p", chan);

	if (IS_ENABLED(CONFIG_BT_ATT_ENFORCE_FLOW)) {
		atomic_clear_bit(chan->flags, ATT_PENDING_RSP);
	}
}

static void chan_req_sent(struct bt_att_chan *chan)
{
	BT_DBG("chan %p chan->req %p", chan, chan->req);

	/* Start timeout work */
	if (chan->req) {
		k_work_schedule(&chan->timeout_work, BT_ATT_TIMEOUT);
	}
}

static bt_att_chan_sent_t chan_cb(struct net_buf *buf)
{
	switch (att_op_get_type(buf->data[0])) {
	case ATT_RESPONSE:
		return chan_rsp_sent;
	case ATT_CONFIRMATION:
		return chan_cfm_sent;
	case ATT_REQUEST:
	case ATT_INDICATION:
		return chan_req_sent;
	default:
		return NULL;
	}
}

static void att_cfm_sent(struct bt_conn *conn, void *user_data)
{
	struct bt_l2cap_chan *ch = (struct bt_l2cap_chan *)user_data;
	struct bt_att_chan *chan = ATT_CHAN(ch);

	BT_DBG("conn %p chan %p", conn, chan);

	chan->sent = chan_cfm_sent;

	att_sent(conn, user_data);
}

static void att_rsp_sent(struct bt_conn *conn, void *user_data)
{
	struct bt_l2cap_chan *ch = (struct bt_l2cap_chan *)user_data;
	struct bt_att_chan *chan = ATT_CHAN(ch);

	BT_DBG("conn %p chan %p", conn, chan);

	chan->sent = chan_rsp_sent;

	att_sent(conn, user_data);
}

static void att_req_sent(struct bt_conn *conn, void *user_data)
{
	struct bt_l2cap_chan *ch = (struct bt_l2cap_chan *)user_data;
	struct bt_att_chan *chan = ATT_CHAN(ch);

	BT_DBG("conn %p chan %p", conn, chan);

	chan->sent = chan_req_sent;

	att_sent(conn, user_data);
}

static bt_conn_tx_cb_t att_cb(bt_att_chan_sent_t cb)
{
	if (cb == chan_rsp_sent) {
		return att_rsp_sent;
	} else if (cb == chan_cfm_sent) {
		return att_cfm_sent;
	} else if (cb == chan_req_sent) {
		return att_req_sent;
	} else {
		return att_sent;
	}
}

static struct net_buf *bt_att_chan_create_pdu(struct bt_att_chan *chan, uint8_t op,
				       size_t len)
{
	struct bt_att_hdr *hdr;
	struct net_buf *buf;

	if (len + sizeof(op) > chan->chan.tx.mtu) {
		BT_WARN("ATT MTU exceeded, max %u, wanted %lu",
			chan->chan.tx.mtu, len + sizeof(op));
		return NULL;
	}

	switch (att_op_get_type(op)) {
	case ATT_RESPONSE:
	case ATT_CONFIRMATION:
		/* Use a timeout only when responding/confirming */
		buf = bt_l2cap_create_pdu_timeout(NULL, 0, BT_ATT_TIMEOUT);
		break;
	default:
		buf = bt_l2cap_create_pdu(NULL, 0);
	}

	if (!buf) {
		BT_ERR("Unable to allocate buffer for op 0x%02x", op);
		return NULL;
	}

	hdr = (struct bt_att_hdr *)net_buf_add(buf, sizeof(*hdr));
	hdr->code = op;

	return buf;
}
#if 0
static inline bool att_chan_is_connected(struct bt_att_chan *chan)
{
	return (chan->att->conn->state != BT_CONN_CONNECTED ||
		!atomic_test_bit(chan->flags, ATT_DISCONNECTED));
}
#endif
static int bt_att_chan_send(struct bt_att_chan *chan, struct net_buf *buf,
			    bt_att_chan_sent_t cb)
{
	struct bt_att_hdr *hdr;

	hdr = (struct bt_att_hdr *)buf->data;

	BT_DBG("chan %p flags %u code 0x%02x", chan, atomic_get(chan->flags),
	       hdr->code);
    (void)hdr;

	return chan_send(chan, buf, cb);
}

static void send_err_rsp(struct bt_att_chan *chan, uint8_t req, uint16_t handle,
			 uint8_t err)
{
    ATT_ERROR_RSP_PARAM        err_param;

    err_param.handle = sys_cpu_to_le16(handle);
    err_param.op_code = req;
    err_param.error_code = err;

    (void)BT_att_send_error_rsp(&chan->handle, &err_param);
    return;
}

static uint8_t att_mtu_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_exchange_mtu_req *req;
	uint16_t mtu_client, mtu_server;
	ATT_XCHG_MTU_RSP_PARAM    xcnhgRspParam;
    uint8_t ret = 0;

	/* Exchange MTU sub-procedure shall only be supported on the
	 * LE Fixed Channel Unenhanced ATT bearer.
	 */
	if (atomic_test_bit(chan->flags, ATT_ENHANCED)) {
		return BT_ATT_ERR_NOT_SUPPORTED;
	}

	req = (struct bt_att_exchange_mtu_req *)buf->data;

	mtu_client = sys_le16_to_cpu(req->mtu);

	BT_DBG("Client MTU %u", mtu_client);

	/* Check if MTU is valid */
	if (mtu_client < BT_ATT_DEFAULT_LE_MTU) {
		return BT_ATT_ERR_INVALID_PDU;
	}

	mtu_server = BT_ATT_MTU;

	BT_DBG("Server MTU %u", mtu_server);

	xcnhgRspParam.mtu = sys_cpu_to_le16(mtu_server);

    if(API_SUCCESS != BT_att_send_mtu_xcnhg_rsp
             (
                 &chan->handle,
                 &xcnhgRspParam
             ))
    {
        ret = BT_ATT_ERR_UNLIKELY;
    }

	if (0 == ret)
	{
		/* BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part F] page 484:
		*
		* A device's Exchange MTU Request shall contain the same MTU as the
		* device's Exchange MTU Response (i.e. the MTU shall be symmetric).
		*/
		chan->chan.rx.mtu = MIN(mtu_client, mtu_server);
		chan->chan.tx.mtu = chan->chan.rx.mtu;

		BT_DBG("Negotiated MTU %u", chan->chan.rx.mtu);
	}
	return 0;
}

static int bt_att_chan_send_mtu_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_exchange_mtu_req *mtuReq;
	API_RESULT retval;

	mtuReq = (struct bt_att_exchange_mtu_req *)&buf->data[sizeof(struct bt_att_hdr)];

    retval = BT_att_send_mtu_xcnhg_req
                (
                    &chan->handle,
                    &mtuReq->mtu
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_find_info_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_find_info_req *findIfoReq;
	ATT_FIND_INFO_REQ_PARAM findInfoReqParam;
	API_RESULT retval;

	findIfoReq = (struct bt_att_find_info_req *)&buf->data[sizeof(struct bt_att_hdr)];

    findInfoReqParam.start_handle = findIfoReq->start_handle;
    findInfoReqParam.end_handle = findIfoReq->end_handle;

    retval = BT_att_send_find_info_req
                (
                    &chan->handle,
                    &findInfoReqParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_find_type_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_find_type_req *findTypeReq;
    ATT_FIND_BY_TYPE_VAL_REQ_PARAM findTypeValReqParam;
	API_RESULT retval;

	findTypeReq = (struct bt_att_find_type_req *)&buf->data[sizeof(struct bt_att_hdr)];

    findTypeValReqParam.range.start_handle = findTypeReq->start_handle;
    findTypeValReqParam.range.end_handle = findTypeReq->end_handle;
    findTypeValReqParam.uuid = findTypeReq->type;
    findTypeValReqParam.value.len = buf->len - sizeof(struct bt_att_find_type_req) - sizeof(struct bt_att_hdr);
    findTypeValReqParam.value.val = &findTypeReq->value[0];

    retval = BT_att_send_find_by_type_val_req
                (
                    &chan->handle,
                    &findTypeValReqParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_read_type_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_read_type_req *readTypeReq;
    ATT_READ_BY_TYPE_REQ_PARAM readByTypeReq;
	API_RESULT retval;

	readTypeReq = (struct bt_att_read_type_req *)&buf->data[sizeof(struct bt_att_hdr)];

    readByTypeReq.range.start_handle = readTypeReq->start_handle;
    readByTypeReq.range.end_handle = readTypeReq->end_handle;
    if (2 == (buf->len - sizeof(struct bt_att_read_type_req) - sizeof(struct bt_att_hdr)))
    {
        readByTypeReq.uuid_format = ATT_16_BIT_UUID_FORMAT;
        memcpy(&readByTypeReq.uuid.uuid_16, readTypeReq->uuid, sizeof(readByTypeReq.uuid.uuid_16));
    }
    else
    {
        readByTypeReq.uuid_format = ATT_128_BIT_UUID_FORMAT;
        memcpy(&readByTypeReq.uuid.uuid_128.value[0], readTypeReq->uuid, sizeof(readByTypeReq.uuid.uuid_128.value));
    }

    retval = BT_att_send_read_by_type_req
                (
                    &chan->handle,
                    &readByTypeReq
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_read_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_read_req *readReq;
    ATT_READ_REQ_PARAM readReqParam;
	API_RESULT retval;

	readReq = (struct bt_att_read_req *)&buf->data[sizeof(struct bt_att_hdr)];

    readReqParam = (ATT_ATTR_HANDLE) readReq->handle;

    retval = BT_att_send_read_req
                (
                    &chan->handle,
                    &readReqParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_read_blob_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_read_blob_req *readBlobReq;
    ATT_READ_BLOB_REQ_PARAM readBlobReqParam;
	API_RESULT retval;

	readBlobReq = (struct bt_att_read_blob_req *)&buf->data[sizeof(struct bt_att_hdr)];

    readBlobReqParam.handle = (ATT_ATTR_HANDLE) readBlobReq->handle;
    readBlobReqParam.offset = (ATT_ATTR_HANDLE) readBlobReq->offset;

    retval = BT_att_send_read_blob_req
                (
                    &chan->handle,
                    &readBlobReqParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_read_mult_req(struct bt_att_chan *chan, struct net_buf *buf)
{
    ATT_READ_MULTIPLE_REQ_PARAM readMultipleReqParam;
	API_RESULT retval;

    readMultipleReqParam.list_count = (buf->len - sizeof(struct bt_att_hdr))/sizeof(uint16_t);
    readMultipleReqParam.handle_list = (uint16_t *)&buf->data[sizeof(struct bt_att_hdr)];

    retval = BT_att_send_read_multiple_req
                (
                    &chan->handle,
                    &readMultipleReqParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_read_group_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_read_group_req *readGroupReq;
    ATT_READ_BY_GROUP_TYPE_REQ_PARAM readByGroupParam;
	API_RESULT retval;

	readGroupReq = (struct bt_att_read_group_req *)&buf->data[sizeof(struct bt_att_hdr)];

    readByGroupParam.range.start_handle = (ATT_ATTR_HANDLE) readGroupReq->start_handle;
    readByGroupParam.range.end_handle = (ATT_ATTR_HANDLE) readGroupReq->end_handle;
    readByGroupParam.uuid_format = (UCHAR) ((2 == ((buf->len - sizeof(struct bt_att_hdr)) - sizeof(*readGroupReq))) ? ATT_16_BIT_UUID_FORMAT : ATT_128_BIT_UUID_FORMAT);

    if (ATT_16_BIT_UUID_FORMAT == readByGroupParam.uuid_format)
    {
        memcpy(&readByGroupParam.group_type.uuid_16, readGroupReq->uuid, sizeof(readByGroupParam.group_type.uuid_16));
    }
    else /* if (ATT_128_BIT_UUID_FORMAT == readByGroupParam.uuid_format) */
    {
        memcpy(&readByGroupParam.group_type.uuid_128.value[0], readGroupReq->uuid, sizeof(readByGroupParam.group_type.uuid_128.value));
    }

    retval = BT_att_send_read_by_group_req
                (
                    &chan->handle,
                    &readByGroupParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_write_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_write_req *writeReq;
    ATT_WRITE_REQ_PARAM writeReqParam;
	API_RESULT retval;

	writeReq = (struct bt_att_write_req *)&buf->data[sizeof(struct bt_att_hdr)];

	writeReqParam.handle = (ATT_ATTR_HANDLE) writeReq->handle;
    writeReqParam.value.len = (UINT16) ((buf->len - sizeof(struct bt_att_hdr)) - sizeof(*writeReq));
    writeReqParam.value.val = writeReq->value;

    retval = BT_att_send_write_req
                (
                    &chan->handle,
                    &writeReqParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_prepare_write_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_prepare_write_req *prepareWriteReq;
    ATT_PREPARE_WRITE_REQ_PARAM prepareWriteReqParam;
	API_RESULT retval;

	prepareWriteReq = (struct bt_att_prepare_write_req *)&buf->data[sizeof(struct bt_att_hdr)];

    prepareWriteReqParam.handle_value.handle = (ATT_ATTR_HANDLE) prepareWriteReq->handle;
    prepareWriteReqParam.offset = (UINT16) prepareWriteReq->offset;
    prepareWriteReqParam.handle_value.value.len = (UINT16) ((buf->len - sizeof(struct bt_att_hdr)) - sizeof(*prepareWriteReq));
    prepareWriteReqParam.handle_value.value.val = prepareWriteReq->value;

    retval = BT_att_send_prepare_write_req
                (
                    &chan->handle,
                    &prepareWriteReqParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_exec_write_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_exec_write_req *execWriteReq;
    ATT_EXECUTE_WRITE_REQ_PARAM executeWriteReqParam;
	API_RESULT retval;

	execWriteReq = (struct bt_att_exec_write_req *)&buf->data[sizeof(struct bt_att_hdr)];

    executeWriteReqParam.flags = (UCHAR)execWriteReq->flags;

    retval = BT_att_send_execute_write_req
                (
                    &chan->handle,
                    &executeWriteReqParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_write_cmd(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_write_cmd *writeCmd;
    ATT_WRITE_CMD_PARAM writeCmdParam;
	API_RESULT retval;

	writeCmd = (struct bt_att_write_cmd *)&buf->data[sizeof(struct bt_att_hdr)];

    writeCmdParam.handle = (ATT_ATTR_HANDLE) writeCmd->handle;
    writeCmdParam.value.len = (UINT16) ((buf->len - sizeof(struct bt_att_hdr)) - sizeof(*writeCmd));
    writeCmdParam.value.val = writeCmd->value;

    retval = BT_att_send_write_cmd
                (
                    &chan->handle,
                    &writeCmdParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

#if (defined(CONFIG_BT_SIGNING) && ((CONFIG_BT_SIGNING) > 0U))
static int bt_att_chan_send_signed_write_cmd(struct bt_att_chan *chan, struct net_buf *buf)
{
#if (defined(CONFIG_BT_SIGNING) && (CONFIG_BT_SIGNING > 0U))
	struct bt_att_signed_write_cmd *signedWriteReq;
    ATT_SIGNED_WRITE_CMD_PARAM signWriteReqParam;
	API_RESULT retval;

	signedWriteReq = (struct bt_att_signed_write_cmd *)&buf->data[sizeof(struct bt_att_hdr)];

	signWriteReqParam.handle_value.handle = signedWriteReq->handle;
	signWriteReqParam.handle_value.value.len = buf->len - sizeof(struct bt_att_hdr) - sizeof(struct bt_att_signed_write_cmd) - sizeof(signWriteReqParam.auth_signature);
	signWriteReqParam.handle_value.value.val = signedWriteReq->value;

	memcpy(&signWriteReqParam.auth_signature[0], &signedWriteReq->value[signWriteReqParam.handle_value.value.len], sizeof(signWriteReqParam.auth_signature));

    retval = BT_att_send_signed_write_cmd
                (
                    &chan->handle,
                    &signWriteReqParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
#else
	return -ENOTSUP;
#endif
}
#endif /* CONFIG_BT_SIGNING */

static int bt_att_chan_send_notify(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_notify *notify;
    ATT_HANDLE_VALUE_PAIR hndlValParam;
	API_RESULT retval;

	notify = (struct bt_att_notify *)&buf->data[sizeof(struct bt_att_hdr)];

    hndlValParam.handle    = sys_cpu_to_le16(notify->handle);
    hndlValParam.value.val = (UCHAR *)notify->value;
    hndlValParam.value.len = buf->len - sizeof(struct bt_att_hdr) - sizeof(struct bt_att_notify);

    retval = BT_att_send_hndl_val_ntf
                (
                    &chan->handle,
                    &hndlValParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_indicate(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_indicate *indicate;
    ATT_HANDLE_VALUE_PAIR hndlValParam;
	API_RESULT retval;

	indicate = (struct bt_att_indicate *)&buf->data[sizeof(struct bt_att_hdr)];

    hndlValParam.handle    = sys_cpu_to_le16(indicate->handle);
    hndlValParam.value.val = (UCHAR *)indicate->value;
    hndlValParam.value.len = buf->len - sizeof(struct bt_att_hdr) - sizeof(struct bt_att_indicate);

    retval = BT_att_send_hndl_val_ind
                (
                    &chan->handle,
                    &hndlValParam
                );
	if (API_SUCCESS != retval)
	{
		return -EIO;
	}
	else
	{
		return 0;
	}
}

static int bt_att_chan_send_req(struct bt_att_chan *chan, struct net_buf *buf, bt_conn_tx_cb_t cb, void *user_data)
{
	struct bt_att_hdr *hdr;
	int err = -EINVAL;

	hdr = (struct bt_att_hdr *)buf->data;
	switch (hdr->code)
	{
	case BT_ATT_OP_MTU_REQ:
		err = bt_att_chan_send_mtu_req(chan, buf);
	break;
	case BT_ATT_OP_FIND_INFO_REQ:
		err = bt_att_chan_send_find_info_req(chan, buf);
	break;
	case BT_ATT_OP_FIND_TYPE_REQ:
		err = bt_att_chan_send_find_type_req(chan, buf);
	break;
	case BT_ATT_OP_READ_TYPE_REQ:
		err = bt_att_chan_send_read_type_req(chan, buf);
	break;
	case BT_ATT_OP_READ_REQ:
		err = bt_att_chan_send_read_req(chan, buf);
	break;
	case BT_ATT_OP_READ_BLOB_REQ:
		err = bt_att_chan_send_read_blob_req(chan, buf);
	break;
	case BT_ATT_OP_READ_MULT_REQ:
		err = bt_att_chan_send_read_mult_req(chan, buf);
	break;
	case BT_ATT_OP_READ_GROUP_REQ:
		err = bt_att_chan_send_read_group_req(chan, buf);
	break;
	case BT_ATT_OP_WRITE_REQ:
		err = bt_att_chan_send_write_req(chan, buf);
	break;
	case BT_ATT_OP_PREPARE_WRITE_REQ:
		err = bt_att_chan_send_prepare_write_req(chan, buf);
	break;
	case BT_ATT_OP_EXEC_WRITE_REQ:
		err = bt_att_chan_send_exec_write_req(chan, buf);
	break;
	case BT_ATT_OP_WRITE_CMD:
		err = bt_att_chan_send_write_cmd(chan, buf);
	break;
#if (defined(CONFIG_BT_SIGNING) && ((CONFIG_BT_SIGNING) > 0U))
	case BT_ATT_OP_SIGNED_WRITE_CMD:
		err = bt_att_chan_send_signed_write_cmd(chan, buf);
	break;
#endif
	case BT_ATT_OP_NOTIFY:
		err = bt_att_chan_send_notify(chan, buf);
	break;
	case BT_ATT_OP_INDICATE:
		err = bt_att_chan_send_indicate(chan, buf);
	break;
	default:
		err = -ENOTSUP;
	break;
	}
	net_buf_unref(buf);
	if (0 == err)
	{
		if (NULL != cb)
		{
			cb(chan->att->conn, user_data);
		}
	}
	return err;
}

static int bt_att_chan_req_send(struct bt_att_chan *chan,
				struct bt_att_req *req)
{
	assert(chan);
	assert(req);
	assert(req->func);
	assert(!chan->req);

	BT_DBG("req %p", req);

	return chan_req_send(chan, req);
}

static void att_process(struct bt_att *att)
{
	sys_snode_t *node;
	struct bt_att_chan *chan, *tmp;

	/* Pull next request from the list */
	node = sys_slist_get(&att->reqs);
	if (!node) {
		return;
	}

	BT_DBG("req %p", ATT_REQ(node));

	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&att->chans, chan, tmp, node, struct bt_att_chan) {
		/* If there is nothing pending use the channel */
		if (!chan->req) {
			if (bt_att_chan_req_send(chan, ATT_REQ(node)) >= 0) {
				return;
			}
		}
	}

	/* Prepend back to the list as it could not be sent */
	sys_slist_prepend(&att->reqs, node);
}

static uint8_t att_handle_rsp(struct bt_att_chan *chan, void *pdu, uint16_t len,
			   uint8_t err)
{
	bt_att_func_t func = NULL;
	void *params;

	BT_DBG("chan %p err 0x%02x len %u: %s", chan, err, len,
	       bt_hex(pdu, len));

	/* Cancel timeout if ongoing */
	k_work_cancel_delayable(&chan->timeout_work);

	if (!chan->req) {
		BT_WARN("No pending ATT request");
		goto process;
	}

	/* Check if request has been cancelled */
	if (chan->req == &cancel) {
		chan->req = NULL;
		goto process;
	}

	/* Reset func so it can be reused by the callback */
	func = chan->req->func;
	chan->req->func = NULL;
	params = chan->req->user_data;

	/* free allocated request so its memory can be reused */
	bt_att_req_free(chan->req);
	chan->req = NULL;

process:
	/* Process pending requests */
	att_process(chan->att);
	if (func) {
		func(chan->att->conn, err, pdu, len, params);
	}

	return 0;
}

#if (defined(CONFIG_BT_GATT_CLIENT) && ((CONFIG_BT_GATT_CLIENT) > 0U))
static uint8_t att_mtu_rsp(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_exchange_mtu_rsp *rsp;
	uint16_t mtu;

	rsp = (struct bt_att_exchange_mtu_rsp *)buf->data;

	mtu = sys_le16_to_cpu(rsp->mtu);

	BT_DBG("Server MTU %u", mtu);

	/* Check if MTU is valid */
	if (mtu < BT_ATT_DEFAULT_LE_MTU) {
		return att_handle_rsp(chan, NULL, 0, BT_ATT_ERR_INVALID_PDU);
	}

	chan->chan.rx.mtu = MIN(mtu, BT_ATT_MTU);

	/* BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part F] page 484:
	 *
	 * A device's Exchange MTU Request shall contain the same MTU as the
	 * device's Exchange MTU Response (i.e. the MTU shall be symmetric).
	 */
	chan->chan.tx.mtu = chan->chan.rx.mtu;

	BT_DBG("Negotiated MTU %u", chan->chan.rx.mtu);

	return att_handle_rsp(chan, rsp, buf->len, 0);
}
#endif /* CONFIG_BT_GATT_CLIENT */

static bool range_is_valid(uint16_t start, uint16_t end, uint16_t *err)
{
	/* Handle 0 is invalid */
	if (!start || !end) {
		if (err) {
			*err = 0U;
		}
		return false;
	}

	/* Check if range is valid */
	if (start > end) {
		if (err) {
			*err = start;
		}
		return false;
	}

	return true;
}

struct find_info_data {
	struct bt_att_chan *chan;
    size_t sofar;
    ATT_FIND_INFO_RSP_PARAM    param;
    ATT_HANDLE_UUID_PAIR       handle_uuid_list[BT_ATT_MAX_LIST_COUNT];
};

static uint8_t find_info_cb(const struct bt_gatt_attr *attr, uint16_t handle,
			    void *user_data)
{
	struct find_info_data *data = (struct find_info_data *)user_data;
	struct bt_att_chan *chan = data->chan;
    size_t len;

	BT_DBG("handle 0x%04x", handle);

	/* Initialize rsp at first entry */
    if (NULL == data->param.handle_value_list.list)
    {
        data->param.handle_value_list.list = data->handle_uuid_list;
        data->param.uuid_format = (BT_UUID_TYPE_16 == attr->uuid->type) ?
                    ATT_16_BIT_UUID_FORMAT : ATT_128_BIT_UUID_FORMAT;
        data->sofar += 1;
    }


	switch (data->param.uuid_format) {
	case ATT_16_BIT_UUID_FORMAT:
		if (attr->uuid->type != BT_UUID_TYPE_16) {
			return BT_GATT_ITER_STOP;
		}

        /* Check the sending length */
        len = sizeof(handle) + sizeof(data->param.handle_value_list.list[data->param.handle_value_list.list_count].uuid.uuid_16);
        if (chan->chan.tx.mtu < (data->sofar + len))
        {
            return BT_GATT_ITER_STOP;
        }
        /* Fast forward to next item position */
        data->param.handle_value_list.list[data->param.handle_value_list.list_count].handle = sys_cpu_to_le16(handle);
        data->param.handle_value_list.list[data->param.handle_value_list.list_count].uuid.uuid_16 = sys_cpu_to_le16(BT_UUID_16(attr->uuid)->val);
        data->param.handle_value_list.list_count++;
        data->sofar += len;

        if (data->param.handle_value_list.list_count < BT_ATT_MAX_LIST_COUNT)
        {
            return BT_GATT_ITER_CONTINUE;
        }

		break;
	case ATT_128_BIT_UUID_FORMAT:
		if (attr->uuid->type != BT_UUID_TYPE_128) {
			return BT_GATT_ITER_STOP;
		}

		/* Check the sending length */
        len = sizeof(handle) + sizeof(data->param.handle_value_list.list[data->param.handle_value_list.list_count].uuid.uuid_128.value);
        if (chan->chan.tx.mtu < (data->sofar + len))
        {
            return BT_GATT_ITER_STOP;
        }
        /* Fast forward to next item position */
        data->param.handle_value_list.list[data->param.handle_value_list.list_count].handle = sys_cpu_to_le16(handle);
        (void)memcpy(data->param.handle_value_list.list[data->param.handle_value_list.list_count].uuid.uuid_128.value, BT_UUID_128(attr->uuid)->val,
               sizeof(data->param.handle_value_list.list[data->param.handle_value_list.list_count].uuid.uuid_128.value));
        data->param.handle_value_list.list_count++;
        data->sofar += len;

        if (data->param.handle_value_list.list_count < BT_ATT_MAX_LIST_COUNT)
        {
            return BT_GATT_ITER_CONTINUE;
        }
		break;
	default:
	  /* MISRA 16.4 : The switch statement does not have a non-empty default clause. */
		break;
	}

	return BT_GATT_ITER_STOP;
}

static uint8_t att_find_info_rsp(struct bt_att_chan *chan, uint16_t start_handle,
			      uint16_t end_handle)
{
	struct find_info_data data;
	uint8_t ret = 0;

	(void)memset(&data, 0, sizeof(data));

	data.chan = chan;
	bt_gatt_foreach_attr(start_handle, end_handle, find_info_cb, &data);

	if (NULL != data.param.handle_value_list.list)
	{
		if (API_SUCCESS != BT_att_send_find_info_rsp (&chan->handle,&data.param))
		{
			ret = BT_ATT_ERR_UNLIKELY;
		}
	}
	else
	{
		send_err_rsp(chan, BT_ATT_OP_FIND_INFO_REQ, start_handle, BT_ATT_ERR_ATTRIBUTE_NOT_FOUND);
	}

	return ret;
}

static uint8_t att_find_info_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_find_info_req *req;
	uint16_t start_handle, end_handle, err_handle;

	req = (struct bt_att_find_info_req *)buf->data;

	start_handle = sys_le16_to_cpu(req->start_handle);
	end_handle = sys_le16_to_cpu(req->end_handle);

	BT_DBG("start_handle 0x%04x end_handle 0x%04x", start_handle,
	       end_handle);

	if (!range_is_valid(start_handle, end_handle, &err_handle)) {
		send_err_rsp(chan, BT_ATT_OP_FIND_INFO_REQ, err_handle,
			     BT_ATT_ERR_INVALID_HANDLE);
		return 0;
	}

	return att_find_info_rsp(chan, start_handle, end_handle);
}

struct find_type_data {
	struct bt_att_chan *chan;
	uint8_t buffer[BT_ATT_MTU];
    size_t len;
    size_t sofar;
	struct bt_att_handle_group *group;
	const void *value;
    ATT_FIND_BY_TYPE_VAL_RSP_PARAM    list;
    ATT_HANDLE_RANGE                  group_list[BT_ATT_MAX_GROUP_COUNT];
	uint8_t value_len;
	uint8_t err;
};

static uint8_t find_type_cb(const struct bt_gatt_attr *attr, uint16_t handle,
			    void *user_data)
{
	struct find_type_data *data = (struct find_type_data *)user_data;
	struct bt_att_chan *chan = data->chan;
	struct bt_conn *conn = chan->chan.chan.conn;
	int read;
	uint8_t uuid[16];

	/* Skip secondary services */
	if (!bt_uuid_cmp(attr->uuid, BT_UUID_GATT_SECONDARY)) {
		goto skip;
	}

	/* Update group end_handle if not a primary service */
	if (bt_uuid_cmp(attr->uuid, BT_UUID_GATT_PRIMARY)) {
		if (data->group &&
		    handle > sys_le16_to_cpu(data->group->end_handle)) {
			data->group->end_handle = sys_cpu_to_le16(handle);
			data->list.range[data->list.count-1].end_handle = sys_cpu_to_le16(handle);
		}
		return BT_GATT_ITER_CONTINUE;
	}

	BT_DBG("handle 0x%04x", handle);

	/* stop if there is no space left */
	if ((data->len - data->sofar) < sizeof(*data->group)) {
		return BT_GATT_ITER_STOP;
	}

	/* Read attribute value and store in the buffer */
	read = attr->read(conn, attr, uuid, sizeof(uuid), 0);
	if (read < 0) {
		/*
		 * Since we don't know if it is the service with requested UUID,
		 * we cannot respond with an error to this request.
		 */
		goto skip;
	}

	/* Check if data matches */
	if (read != data->value_len) {
		/* Use bt_uuid_cmp() to compare UUIDs of different form. */
		struct bt_uuid_128 ref_uuid;
		struct bt_uuid_128 recvd_uuid;

		if (!bt_uuid_create(&recvd_uuid.uuid, (const uint8_t *)data->value, data->value_len)) {
			BT_WARN("Unable to create UUID: size %u", data->value_len);
			goto skip;
		}
		if (!bt_uuid_create(&ref_uuid.uuid, uuid, read)) {
			BT_WARN("Unable to create UUID: size %d", read);
			goto skip;
		}
		if (bt_uuid_cmp(&recvd_uuid.uuid, &ref_uuid.uuid)) {
			goto skip;
		}
	} else if (memcmp(data->value, uuid, read)) {
		goto skip;
	}

	/* If service has been found, error should be cleared */
	data->err = 0x00;

	/* Fast forward to next item position */
	data->group = (struct bt_att_handle_group *)&data->buffer[data->sofar];
	data->group->start_handle = sys_cpu_to_le16(handle);
	data->group->end_handle = sys_cpu_to_le16(handle);
    data->sofar += sizeof(*data->group);

	data->list.range[data->list.count].start_handle = sys_cpu_to_le16(handle);
	data->list.range[data->list.count].end_handle = sys_cpu_to_le16(handle);
	data->list.count++;

	/* continue to find the end_handle */
	return BT_GATT_ITER_CONTINUE;

skip:
	data->group = NULL;
	return BT_GATT_ITER_CONTINUE;
}

static uint8_t att_find_type_rsp(struct bt_att_chan *chan, uint16_t start_handle,
			      uint16_t end_handle, const void *value,
			      uint8_t value_len)
{
	struct find_type_data data;
	uint8_t ret = 0;

	(void)memset(&data, 0, sizeof(data));

	data.chan = chan;
	data.group = NULL;
	data.value = value;
	data.value_len = value_len;
	data.list.range = data.group_list;
	data.len = MIN(sizeof(data.buffer), chan->chan.tx.mtu);

	/* Pre-set error in case no service will be found */
	data.err = BT_ATT_ERR_ATTRIBUTE_NOT_FOUND;

	bt_gatt_foreach_attr(start_handle, end_handle, find_type_cb, &data);

	/* If error has not been cleared, no service has been found */
	if (data.err) {
		/* Respond since handle is set */
		send_err_rsp(chan, BT_ATT_OP_FIND_TYPE_REQ, start_handle,
			     data.err);
		return 0;
	}
	else
	{
		if (API_SUCCESS != BT_att_send_find_by_type_val_rsp(&chan->handle, &data.list))
		{
			ret = BT_ATT_ERR_UNLIKELY;
		}
	}

	return ret;
}

static uint8_t att_find_type_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_find_type_req *req;
	uint16_t start_handle, end_handle, err_handle, type;
	uint8_t *value;

	req = (struct bt_att_find_type_req *)net_buf_pull_mem(buf, sizeof(*req));

	start_handle = sys_le16_to_cpu(req->start_handle);
	end_handle = sys_le16_to_cpu(req->end_handle);
	type = sys_le16_to_cpu(req->type);
	value = buf->data;

	BT_DBG("start_handle 0x%04x end_handle 0x%04x type %u", start_handle,
	       end_handle, type);

	if (!range_is_valid(start_handle, end_handle, &err_handle)) {
		send_err_rsp(chan, BT_ATT_OP_FIND_TYPE_REQ, err_handle,
			     BT_ATT_ERR_INVALID_HANDLE);
		return 0;
	}

	/* The Attribute Protocol Find By Type Value Request shall be used with
	 * the Attribute Type parameter set to the UUID for "Primary Service"
	 * and the Attribute Value set to the 16-bit Bluetooth UUID or 128-bit
	 * UUID for the specific primary service.
	 */
	if (bt_uuid_cmp(BT_UUID_DECLARE_16(type), BT_UUID_GATT_PRIMARY)) {
		send_err_rsp(chan, BT_ATT_OP_FIND_TYPE_REQ, start_handle,
			     BT_ATT_ERR_ATTRIBUTE_NOT_FOUND);
		return 0;
	}

	return att_find_type_rsp(chan, start_handle, end_handle, value,
				 buf->len);
}

static uint8_t err_to_att(int err)
{
	BT_DBG("%d", err);

	if (err < 0 && err >= -0xff) {
		return -err;
	}

	return BT_ATT_ERR_UNLIKELY;
}

struct read_type_data {
	struct bt_att_chan *chan;
	struct bt_uuid *uuid;
    uint8_t buffer[BT_ATT_MTU];
    size_t len;
    size_t sofar;
	struct bt_att_read_type_rsp *rsp;
	struct bt_att_data *item;
    ATT_READ_BY_TYPE_RSP_PARAM     param;
    ATT_HANDLE_VALUE_PAIR          handle_value_list[BT_ATT_MAX_GROUP_COUNT];
	uint8_t err;
};

typedef bool (*attr_read_cb)(struct net_buf *buf, ssize_t read,
			     void *user_data);
#if 0
static bool attr_read_type_cb(struct net_buf *frag, ssize_t read,
			      void *user_data)
{
	struct read_type_data *data = user_data;

	if (!data->rsp->len) {
		/* Set len to be the first item found */
		data->rsp->len = read + sizeof(*data->item);
	} else if (data->rsp->len != read + sizeof(*data->item)) {
		/* All items should have the same size */
		frag->len -= sizeof(*data->item);
		data->item = NULL;
		return false;
	}

	return true;
}

static ssize_t att_chan_read(struct bt_att_chan *chan,
			     const struct bt_gatt_attr *attr,
			     struct net_buf *buf, uint16_t offset,
			     attr_read_cb cb, void *user_data)
{
	struct bt_conn *conn = chan->chan.chan.conn;
	ssize_t read;
	struct net_buf *frag;
	size_t len, total = 0;

	if (chan->chan.tx.mtu <= net_buf_frags_len(buf)) {
		return 0;
	}

	frag = net_buf_frag_last(buf);

	/* Create necessary fragments if MTU is bigger than what a buffer can
	 * hold.
	 */
	do {
		len = MIN(chan->chan.tx.mtu - net_buf_frags_len(buf),
			  net_buf_tailroom(frag));
		if (!len) {
			frag = net_buf_alloc(net_buf_pool_get(buf->pool_id),
					     osaWaitNone_c);
			/* If not buffer can be allocated immediately return */
			if (!frag) {
				return total;
			}

			net_buf_frag_add(buf, frag);

			len = MIN(chan->chan.tx.mtu - net_buf_frags_len(buf),
				  net_buf_tailroom(frag));
		}

		read = attr->read(conn, attr, frag->data + frag->len, len,
				  offset);
		if (read < 0) {
			if (total) {
				return total;
			}

			return read;
		}

		if (cb && !cb(frag, read, user_data)) {
			break;
		}

		net_buf_add(frag, read);
		total += read;
		offset += read;
	} while (chan->chan.tx.mtu > net_buf_frags_len(buf) && read == len);

	return total;
}
#endif
static uint8_t read_type_cb(const struct bt_gatt_attr *attr, uint16_t handle,
			    void *user_data)
{
	struct read_type_data *data = (struct read_type_data *)user_data;
	struct bt_att_chan *chan = data->chan;
	struct bt_conn *conn = chan->chan.chan.conn;
	ssize_t read;

	/* Skip if doesn't match */
	if (bt_uuid_cmp(attr->uuid, data->uuid)) {
		return BT_GATT_ITER_CONTINUE;
	}

	BT_DBG("handle 0x%04x", handle);

	/*
	 * If an attribute in the set of requested attributes would cause an
	 * Error Response then this attribute cannot be included in a
	 * Read By Type Response and the attributes before this attribute
	 * shall be returned
	 *
	 * If the first attribute in the set of requested attributes would
	 * cause an Error Response then no other attributes in the requested
	 * attributes can be considered.
	 */
	data->err = bt_gatt_check_perm(conn, attr, BT_GATT_PERM_READ_MASK);
	if (data->err) {
		if (data->rsp->len) {
			data->err = 0x00;
		}
		return BT_GATT_ITER_STOP;
	}

	/*
	 * If any attribute is founded in handle range it means that error
	 * should be changed from pre-set: attr not found error to no error.
	 */
	data->err = 0x00;

	/* Fast forward to next item position */
    data->item = (struct bt_att_data *)&data->buffer[data->sofar];
    data->item->handle = sys_cpu_to_le16(handle);
    data->sofar += sizeof(*data->item);

#if 0
	read = att_chan_read(chan, attr, data->buf, 0, attr_read_type_cb, data);
#else
	read = attr->read(conn, attr, &data->buffer[data->sofar],
              data->len - data->sofar, 0);
#endif
	if (read < 0) {
		data->err = err_to_att(read);
		return BT_GATT_ITER_STOP;
	}

    if (!data->rsp->len) {
        /* Set len to be the first item found */
        data->rsp->len = read + sizeof(*data->item);
    } else if (data->rsp->len != (read + sizeof(*data->item))) {
        /* All items should have the same size */
        data->sofar -= sizeof(*data->item);
        return BT_GATT_ITER_STOP;
    }

    data->param.handle_value_list[data->param.count].handle = data->item->handle;
    data->param.handle_value_list[data->param.count].value.len = read;
    data->param.handle_value_list[data->param.count].value.val = &data->buffer[data->sofar];
    data->param.count++;

    data->sofar += read;

    if (data->param.count >= ARRAY_SIZE(data->handle_value_list))
    {
        return BT_GATT_ITER_STOP;
    }

	/* continue only if there are still space for more items */
	return ((data->len - data->sofar) > data->rsp->len) ?
           BT_GATT_ITER_CONTINUE : BT_GATT_ITER_STOP;
}

static uint8_t att_read_type_rsp(struct bt_att_chan *chan, struct bt_uuid *uuid,
			      uint16_t start_handle, uint16_t end_handle)
{
	struct read_type_data data;
	uint8_t ret = 0;

	(void)memset(&data, 0, sizeof(data));

	data.len = MIN(sizeof(data.buffer), chan->chan.tx.mtu);
	data.chan = chan;
	data.uuid = uuid;
	data.rsp = (struct bt_att_read_type_rsp *)data.buffer;
    data.sofar = sizeof(*data.rsp);
	data.param.handle_value_list = data.handle_value_list;

	/* Pre-set error if no attr will be found in handle */
	data.err = BT_ATT_ERR_ATTRIBUTE_NOT_FOUND;

	bt_gatt_foreach_attr(start_handle, end_handle, read_type_cb, &data);

	if (data.err) {
		/* Response here since handle is set */
		send_err_rsp(chan, BT_ATT_OP_READ_TYPE_REQ, start_handle,
			     data.err);
	}
	else
	{
		if (API_SUCCESS != BT_att_read_by_type_rsp(&chan->handle, &data.param))
		{
			ret = BT_ATT_ERR_UNLIKELY;
		}
	}

	return ret;
}

static uint8_t att_read_type_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_read_type_req *req;
	uint16_t start_handle, end_handle, err_handle;
	union {
		struct bt_uuid uuid;
		struct bt_uuid_16 u16;
		struct bt_uuid_128 u128;
	} u;
	uint8_t uuid_len = buf->len - sizeof(*req);

	/* Type can only be UUID16 or UUID128 */
	if (uuid_len != 2 && uuid_len != 16) {
		return BT_ATT_ERR_INVALID_PDU;
	}

	req = (struct bt_att_read_type_req *)net_buf_pull_mem(buf, sizeof(*req));

	start_handle = sys_le16_to_cpu(req->start_handle);
	end_handle = sys_le16_to_cpu(req->end_handle);
	if (!bt_uuid_create(&u.uuid, req->uuid, uuid_len)) {
		return BT_ATT_ERR_UNLIKELY;
	}

	BT_DBG("start_handle 0x%04x end_handle 0x%04x type %s",
	       start_handle, end_handle, bt_uuid_str(&u.uuid));

	if (!range_is_valid(start_handle, end_handle, &err_handle)) {
		send_err_rsp(chan, BT_ATT_OP_READ_TYPE_REQ, err_handle,
			     BT_ATT_ERR_INVALID_HANDLE);
		return 0;
	}

	return att_read_type_rsp(chan, &u.uuid, start_handle, end_handle);
}

struct read_data {
	struct bt_att_chan *chan;
	uint16_t offset;
    uint8_t buffer[BT_ATT_MTU];
    size_t len;
    size_t sofar;
	ATT_READ_RSP_PARAM rspParam;
	uint8_t err;
};

static uint8_t read_cb(const struct bt_gatt_attr *attr, uint16_t handle,
		       void *user_data)
{
	struct read_data *data = (struct read_data *)user_data;
	struct bt_att_chan *chan = data->chan;
	struct bt_conn *conn = chan->chan.chan.conn;
	int ret;

	BT_DBG("handle 0x%04x", handle);

	data->rspParam.val = data->buffer;

	/*
	 * If any attribute is founded in handle range it means that error
	 * should be changed from pre-set: invalid handle error to no error.
	 */
	data->err = 0x00;

	/* Check attribute permissions */
	data->err = bt_gatt_check_perm(conn, attr, BT_GATT_PERM_READ_MASK);
	if (data->err) {
		return BT_GATT_ITER_STOP;
	}

	/* Read attribute value and store in the buffer */
#if 0
	ret = att_chan_read(chan, attr, data->buf, data->offset, NULL, NULL);
#else
	ret = attr->read(conn, attr, &data->buffer[data->offset],
              data->len - data->sofar, data->offset);
#endif
	if (ret < 0) {
		data->err = err_to_att(ret);
		return BT_GATT_ITER_STOP;
	}

    data->sofar += ret;
	return BT_GATT_ITER_CONTINUE;
}

static uint8_t att_read_rsp(struct bt_att_chan *chan, uint8_t op, uint8_t rsp,
			 uint16_t handle, uint16_t offset)
{
	struct bt_conn *conn = chan->chan.chan.conn;
	struct read_data data;
	uint8_t ret = 0;

	if (!bt_gatt_change_aware(conn, true)) {
		return BT_ATT_ERR_DB_OUT_OF_SYNC;
	}

	if (!handle) {
		return BT_ATT_ERR_INVALID_HANDLE;
	}

	(void)memset(&data, 0, sizeof(data));

    data.len = MIN(chan->chan.tx.mtu, sizeof(data.buffer));
    data.offset = offset;
	data.chan = chan;

	/* Pre-set error if no attr will be found in handle */
	data.err = BT_ATT_ERR_INVALID_HANDLE;

	bt_gatt_foreach_attr(handle, handle, read_cb, &data);

	/* In case of error discard data and respond with an error */
	if (data.err) {
		/* Respond here since handle is set */
		send_err_rsp(chan, op, handle, data.err);
	}
	else
	{
        data.rspParam.len = data.sofar;
		if (API_SUCCESS != BT_att_send_pdu(&chan->handle, rsp, &data.rspParam))
		{
			ret = BT_ATT_ERR_UNLIKELY;
		}
	}
	return ret;
}

static uint8_t att_read_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_read_req *req;
	uint16_t handle;

	req = (struct bt_att_read_req *)buf->data;

	handle = sys_le16_to_cpu(req->handle);

	BT_DBG("handle 0x%04x", handle);

	return att_read_rsp(chan, BT_ATT_OP_READ_REQ, BT_ATT_OP_READ_RSP,
			    handle, 0);
}

static uint8_t att_read_blob_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_read_blob_req *req;
	uint16_t handle, offset;

	req = (struct bt_att_read_blob_req *)buf->data;

	handle = sys_le16_to_cpu(req->handle);
	offset = sys_le16_to_cpu(req->offset);

	BT_DBG("handle 0x%04x offset %u", handle, offset);

	return att_read_rsp(chan, BT_ATT_OP_READ_BLOB_REQ,
			    BT_ATT_OP_READ_BLOB_RSP, handle, offset);
}

#if (defined(CONFIG_BT_GATT_READ_MULTIPLE) && ((CONFIG_BT_GATT_READ_MULTIPLE) > 0U))
static uint8_t att_read_mult_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct read_data data;
    ATT_READ_MULTIPLE_RSP_PARAM    rsp_param;
    ATT_VALUE                      value[BT_ATT_MAX_MULTIPLE_READ_COUNT];
	uint16_t handle;
    uint8_t ret = 0;

	(void)memset(&data, 0, sizeof(data));

    data.len = MIN(chan->chan.tx.mtu, sizeof(data.buffer));
	data.chan = chan;

    rsp_param.actual_count = 0;
    rsp_param.count = 0;
    rsp_param.value = value;

	while (buf->len >= sizeof(uint16_t)) {
		handle = net_buf_pull_le16(buf);

		BT_DBG("handle 0x%04x ", handle);

		/* An Error Response shall be sent by the server in response to
		 * the Read Multiple Request [....] if a read operation is not
		 * permitted on any of the Characteristic Values.
		 *
		 * If handle is not valid then return invalid handle error.
		 * If handle is found error will be cleared by read_cb.
		 */
		data.err = BT_ATT_ERR_INVALID_HANDLE;

		bt_gatt_foreach_attr(handle, handle, read_cb, &data);

		/* Stop reading in case of error */
		if (data.err) {
			/* Respond here since handle is set */
			send_err_rsp(chan, BT_ATT_OP_READ_MULT_REQ, handle,
				     data.err);
			return 0;
		}
        value[rsp_param.count] = data.rspParam;
        rsp_param.count++;

        if (rsp_param.count >= BT_ATT_MAX_MULTIPLE_READ_COUNT)
        {
            break;
        }
	}

    if (API_SUCCESS != BT_att_read_multiple_rsp(&chan->handle, &rsp_param))
    {
        ret = BT_ATT_ERR_UNLIKELY;
    }

	return ret;
}

#if (defined(CONFIG_BT_EATT) && ((CONFIG_BT_EATT) > 0U))

static void bt_att_chan_send_rsp(struct bt_att_chan *chan, struct net_buf *buf,
				 bt_att_chan_sent_t cb)
{
	int err;

	err = bt_att_chan_send(chan, buf, cb);
	if (err) {
		/* Responses need to be sent back using the same channel */
		net_buf_put(chan->tx_queue, buf);
	}
}

static uint8_t read_vl_cb(const struct bt_gatt_attr *attr, uint16_t handle,
			  void *user_data)
{
	struct read_data *data = user_data;
	struct bt_att_chan *chan = data->chan;
	struct bt_conn *conn = chan->chan.chan.conn;
	struct bt_att_read_mult_vl_rsp *rsp;
	int read;

	BT_DBG("handle 0x%04x", handle);

	data->rsp = net_buf_add(data->buf, sizeof(*data->rsp));

	/*
	 * If any attribute is founded in handle range it means that error
	 * should be changed from pre-set: invalid handle error to no error.
	 */
	data->err = 0x00;

	/* Check attribute permissions */
	data->err = bt_gatt_check_perm(conn, attr, BT_GATT_PERM_READ_MASK);
	if (data->err) {
		return BT_GATT_ITER_STOP;
	}

	/* The Length Value Tuple List may be truncated within the first two
	 * octets of a tuple due to the size limits of the current ATT_MTU.
	 */
	if (chan->chan.tx.mtu - data->buf->len < 2) {
		return BT_GATT_ITER_STOP;
	}

	rsp = net_buf_add(data->buf, sizeof(*rsp));

	read = att_chan_read(chan, attr, data->buf, data->offset, NULL, NULL);
	if (read < 0) {
		data->err = err_to_att(read);
		return BT_GATT_ITER_STOP;
	}

	rsp->len = read;

	return BT_GATT_ITER_CONTINUE;
}

static uint8_t att_read_mult_vl_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_conn *conn = chan->chan.chan.conn;
	struct read_data data;
	uint16_t handle;

	(void)memset(&data, 0, sizeof(data));

	data.buf = bt_att_create_pdu(conn, BT_ATT_OP_READ_MULT_VL_RSP, 0);
	if (!data.buf) {
		return BT_ATT_ERR_UNLIKELY;
	}

	data.chan = chan;

	while (buf->len >= sizeof(uint16_t)) {
		handle = net_buf_pull_le16(buf);

		BT_DBG("handle 0x%04x ", handle);

		/* If handle is not valid then return invalid handle error.
		 * If handle is found error will be cleared by read_cb.
		 */
		data.err = BT_ATT_ERR_INVALID_HANDLE;

		bt_gatt_foreach_attr(handle, handle, read_vl_cb, &data);

		/* Stop reading in case of error */
		if (data.err) {
			net_buf_unref(data.buf);
			/* Respond here since handle is set */
			send_err_rsp(chan, BT_ATT_OP_READ_MULT_VL_REQ, handle,
				     data.err);
			return 0;
		}
	}

	bt_att_chan_send_rsp(chan, data.buf, chan_rsp_sent);

	return 0;
}
#endif /* CONFIG_BT_EATT */
#endif /* CONFIG_BT_GATT_READ_MULTIPLE */

struct read_group_data {
	struct bt_att_chan *chan;
	struct bt_uuid *uuid;
    uint8_t buffer[BT_ATT_MTU];
    size_t len;
    size_t sofar;
	struct bt_att_read_group_rsp *rsp;
	struct bt_att_group_data *group;
    ATT_GROUP_ATTR_DATA_LIST           list;
    ATT_GROUP_ATTR_DATA_ELEMENT        group_list[BT_ATT_MAX_GROUP_COUNT];
};

#if 0
static bool attr_read_group_cb(struct net_buf *frag, ssize_t read,
			       void *user_data)
{
	struct read_group_data *data = user_data;

	if (!data->rsp->len) {
		/* Set len to be the first group found */
		data->rsp->len = read + sizeof(*data->group);
	} else if (data->rsp->len != read + sizeof(*data->group)) {
		/* All groups entries should have the same size */
		data->buf->len -= sizeof(*data->group);
		data->group = NULL;
		return false;
	}

	return true;
}
#endif

static uint8_t read_group_cb(const struct bt_gatt_attr *attr, uint16_t handle,
			     void *user_data)
{
	struct read_group_data *data = (struct read_group_data *)user_data;
	struct bt_att_chan *chan = data->chan;
	int read;

	/* Update group end_handle if attribute is not a service */
	if (bt_uuid_cmp(attr->uuid, BT_UUID_GATT_PRIMARY) &&
	    bt_uuid_cmp(attr->uuid, BT_UUID_GATT_SECONDARY)) {
		if (data->group &&
		    handle > sys_le16_to_cpu(data->group->end_handle)) {
			data->group->end_handle = sys_cpu_to_le16(handle);
            data->list.list[data->list.count-1].range.end_handle = sys_cpu_to_le16(handle);
		}
		return BT_GATT_ITER_CONTINUE;
	}

	/* If Group Type don't match skip */
	if (bt_uuid_cmp(attr->uuid, data->uuid)) {
		data->group = NULL;
		return BT_GATT_ITER_CONTINUE;
	}

	BT_DBG("handle 0x%04x", handle);

	/* Stop if there is no space left */
    if (data->rsp->len &&
       (data->len - data->sofar) < (data->rsp->len))
    {
        return BT_GATT_ITER_STOP;
    }

	/* Fast forward to next group position */
	data->group = (struct bt_att_group_data *)&data->buffer[data->sofar];

	/* Initialize group handle range */
	data->group->start_handle = sys_cpu_to_le16(handle);
	data->group->end_handle = sys_cpu_to_le16(handle);
    data->sofar += sizeof(*data->group);

	/* Read attribute value and store in the buffer */
#if 0
	read = att_chan_read(chan, attr, data->buf, 0, attr_read_group_cb,
			     data);
#else
    read = attr->read(chan->chan.chan.conn, attr, &data->buffer[data->sofar],
              data->len - data->sofar, 0);
#endif
	if (read < 0) {
		/* TODO: Handle read errors */
		return BT_GATT_ITER_STOP;
	}

    if (!data->rsp->len) {
        /* Set len to be the first group found */
        data->rsp->len = read + sizeof(*data->group);
        data->list.length = read;
    } else if (data->rsp->len != (read + sizeof(*data->group))) {
        /* All groups entries should have the same size */
        data->sofar -= sizeof(*data->group);
        return BT_GATT_ITER_STOP;
    }

    data->list.count++;
    data->list.list[data->list.count-1].attr_value = &data->buffer[data->sofar];
    data->list.list[data->list.count-1].range.start_handle = sys_cpu_to_le16(handle);
    data->list.list[data->list.count-1].range.end_handle = sys_cpu_to_le16(handle);

    data->sofar += read;

	/* continue only if there are still space for more items */
	return BT_GATT_ITER_CONTINUE;
}

static uint8_t att_read_group_rsp(struct bt_att_chan *chan, struct bt_uuid *uuid,
			       uint16_t start_handle, uint16_t end_handle)
{
	struct read_group_data data;
    uint16_t ret = 0;

	(void)memset(&data, 0, sizeof(data));

    data.len = MIN(sizeof(data.buffer), chan->chan.tx.mtu);
	data.chan = chan;
	data.uuid = uuid;
	data.rsp = (struct bt_att_read_group_rsp *)data.buffer;
	data.group = NULL;
    data.sofar = sizeof(*data.rsp);
    data.list.list = data.group_list;

	bt_gatt_foreach_attr(start_handle, end_handle, read_group_cb, &data);

	if (!data.list.count) {
		/* Respond here since handle is set */
		send_err_rsp(chan, BT_ATT_OP_READ_GROUP_REQ, start_handle,
			     BT_ATT_ERR_ATTRIBUTE_NOT_FOUND);
		return 0;
	}
	else
	{
	    if (API_SUCCESS != BT_att_read_by_group_rsp(&chan->handle, &data.list))
		{
			ret = BT_ATT_ERR_UNLIKELY;
		}
	}

	return ret;
}

static uint8_t att_read_group_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_read_group_req *req;
	uint16_t start_handle, end_handle, err_handle;
	union {
		struct bt_uuid uuid;
		struct bt_uuid_16 u16;
		struct bt_uuid_128 u128;
	} u;
	uint8_t uuid_len = buf->len - sizeof(*req);

	/* Type can only be UUID16 or UUID128 */
	if (uuid_len != 2 && uuid_len != 16) {
		return BT_ATT_ERR_INVALID_PDU;
	}

	req = (struct bt_att_read_group_req *)net_buf_pull_mem(buf, sizeof(*req));

	start_handle = sys_le16_to_cpu(req->start_handle);
	end_handle = sys_le16_to_cpu(req->end_handle);

	if (!bt_uuid_create(&u.uuid, req->uuid, uuid_len)) {
		return BT_ATT_ERR_UNLIKELY;
	}

	BT_DBG("start_handle 0x%04x end_handle 0x%04x type %s",
	       start_handle, end_handle, bt_uuid_str(&u.uuid));

	if (!range_is_valid(start_handle, end_handle, &err_handle)) {
		send_err_rsp(chan, BT_ATT_OP_READ_GROUP_REQ, err_handle,
			     BT_ATT_ERR_INVALID_HANDLE);
		return 0;
	}

	/* Core v4.2, Vol 3, sec 2.5.3 Attribute Grouping:
	 * Not all of the grouping attributes can be used in the ATT
	 * Read By Group Type Request. The "Primary Service" and "Secondary
	 * Service" grouping types may be used in the Read By Group Type
	 * Request. The "Characteristic" grouping type shall not be used in
	 * the ATT Read By Group Type Request.
	 */
	if (bt_uuid_cmp(&u.uuid, BT_UUID_GATT_PRIMARY) &&
	    bt_uuid_cmp(&u.uuid, BT_UUID_GATT_SECONDARY)) {
		send_err_rsp(chan, BT_ATT_OP_READ_GROUP_REQ, start_handle,
			     BT_ATT_ERR_UNSUPPORTED_GROUP_TYPE);
		return 0;
	}

	return att_read_group_rsp(chan, &u.uuid, start_handle, end_handle);
}

struct write_data {
	struct bt_conn *conn;
	uint8_t req;
	const void *value;
	uint16_t len;
	uint16_t offset;
	uint8_t err;
};

static uint8_t write_cb(const struct bt_gatt_attr *attr, uint16_t handle,
			void *user_data)
{
	struct write_data *data = (struct write_data *)user_data;
	int write;
	uint8_t flags = 0U;

	BT_DBG("handle 0x%04x offset %u", handle, data->offset);

	/* Check attribute permissions */
	data->err = bt_gatt_check_perm(data->conn, attr,
				       BT_GATT_PERM_WRITE_MASK);
	if (data->err) {
		return BT_GATT_ITER_STOP;
	}

	/* Set command flag if not a request */
	if (!data->req) {
		flags |= BT_GATT_WRITE_FLAG_CMD;
	}

	/* Write attribute value */
	write = attr->write(data->conn, attr, data->value, data->len,
			    data->offset, flags);
	if (write < 0 || write != data->len) {
		data->err = err_to_att(write);
		return BT_GATT_ITER_STOP;
	}

	data->err = 0U;

	return BT_GATT_ITER_CONTINUE;
}

static uint8_t att_write_rsp(struct bt_att_chan *chan, uint8_t req, uint8_t rsp,
			  uint16_t handle, uint16_t offset, const void *value,
			  uint16_t len)
{
	struct write_data data;
    uint8_t ret = 0;

	if (!bt_gatt_change_aware(chan->att->conn, req ? true : false)) {
		return BT_ATT_ERR_DB_OUT_OF_SYNC;
	}

	if (!handle) {
		return BT_ATT_ERR_INVALID_HANDLE;
	}

	(void)memset(&data, 0, sizeof(data));

	data.conn = chan->att->conn;
	data.req = req;
	data.offset = offset;
	data.value = value;
	data.len = len;
	data.err = BT_ATT_ERR_INVALID_HANDLE;

	bt_gatt_foreach_attr(handle, handle, write_cb, &data);

	if (data.err) {
		/* In case of error discard data and respond with an error */
		if (rsp) {
			/* Respond here since handle is set */
			send_err_rsp(chan, req, handle, data.err);
		}
		return req == BT_ATT_OP_EXEC_WRITE_REQ ? data.err : 0;
	}
	else
	{
		if (rsp) {
			API_RESULT retVal = BT_att_write_rsp(&chan->handle);
			if (API_SUCCESS != retVal)
			{
				ret = BT_ATT_ERR_UNLIKELY;
			}
		}
	}

	return ret;
}

static uint8_t att_write_req(struct bt_att_chan *chan, struct net_buf *buf)
{
	uint16_t handle;

	handle = net_buf_pull_le16(buf);

	BT_DBG("handle 0x%04x", handle);

	return att_write_rsp(chan, BT_ATT_OP_WRITE_REQ, BT_ATT_OP_WRITE_RSP,
			     handle, 0, buf->data, buf->len);
}

#if CONFIG_BT_ATT_PREPARE_COUNT > 0
struct prep_data {
	struct bt_conn *conn;
	struct net_buf *buf;
	const void *value;
	uint16_t len;
	uint16_t offset;
	uint8_t err;
};

static uint8_t prep_write_cb(const struct bt_gatt_attr *attr, uint16_t handle,
			     void *user_data)
{
	struct prep_data *data = user_data;
	struct bt_attr_data *attr_data;
	int write;

	BT_DBG("handle 0x%04x offset %u", handle, data->offset);

	/* Check attribute permissions */
	data->err = bt_gatt_check_perm(data->conn, attr,
				       BT_GATT_PERM_WRITE_MASK);
	if (data->err) {
		return BT_GATT_ITER_STOP;
	}

	/* Check if attribute requires handler to accept the data */
	if (!(attr->perm & BT_GATT_PERM_PREPARE_WRITE)) {
		goto append;
	}

	/* Write attribute value to check if device is authorized */
	write = attr->write(data->conn, attr, data->value, data->len,
			    data->offset, BT_GATT_WRITE_FLAG_PREPARE);
	if (write != 0) {
		data->err = err_to_att(write);
		return BT_GATT_ITER_STOP;
	}

append:
	/* Copy data into the outstanding queue */
	data->buf = net_buf_alloc(&prep_pool, osaWaitNone_c);
	if (!data->buf) {
		data->err = BT_ATT_ERR_PREPARE_QUEUE_FULL;
		return BT_GATT_ITER_STOP;
	}

	attr_data = net_buf_user_data(data->buf);
	attr_data->handle = handle;
	attr_data->offset = data->offset;

	net_buf_add_mem(data->buf, data->value, data->len);

	data->err = 0U;

	return BT_GATT_ITER_CONTINUE;
}

static uint8_t att_prep_write_rsp(struct bt_att_chan *chan, uint16_t handle,
			       uint16_t offset, const void *value, uint8_t len)
{
	struct bt_conn *conn = chan->chan.chan.conn;
	struct prep_data data;
	struct bt_att_prepare_write_rsp *rsp;
	ATT_PREPARE_WRITE_RSP_PARAM    prepare_write_rsp;
	API_RESULT retval;
	uint8_t ret = 0;

	if (!bt_gatt_change_aware(conn, true)) {
		return BT_ATT_ERR_DB_OUT_OF_SYNC;
	}

	if (!handle) {
		return BT_ATT_ERR_INVALID_HANDLE;
	}

	(void)memset(&data, 0, sizeof(data));

	data.conn = conn;
	data.offset = offset;
	data.value = value;
	data.len = len;
	data.err = BT_ATT_ERR_INVALID_HANDLE;

	bt_gatt_foreach_attr(handle, handle, prep_write_cb, &data);

	if (data.err) {
		/* Respond here since handle is set */
		send_err_rsp(chan, BT_ATT_OP_PREPARE_WRITE_REQ, handle,
			     data.err);
		return 0;
	}

	BT_DBG("buf %p handle 0x%04x offset %u", data.buf, handle, offset);

	/* Store buffer in the outstanding queue */
	net_buf_put(chan->att->prep_queue, data.buf);

	/* Generate response */
	prepare_write_rsp.offset = sys_cpu_to_le16(offset);;
	prepare_write_rsp.handle_value.handle = sys_cpu_to_le16(handle);;
	prepare_write_rsp.handle_value.value.val = value;
	prepare_write_rsp.handle_value.value.len = len;

	retval = BT_att_send_prepare_write_rsp
				(
					handle,
					&prepare_write_rsp
				);
	if (API_SUCCESS != retVal)
	{
		ret = BT_ATT_ERR_UNLIKELY;
	}
	return ret;
}
#endif /* CONFIG_BT_ATT_PREPARE_COUNT */

static uint8_t att_prepare_write_req(struct bt_att_chan *chan, struct net_buf *buf)
{
#if CONFIG_BT_ATT_PREPARE_COUNT == 0
	return BT_ATT_ERR_NOT_SUPPORTED;
#else
	struct bt_att_prepare_write_req *req;
	uint16_t handle, offset;

	req = net_buf_pull_mem(buf, sizeof(*req));

	handle = sys_le16_to_cpu(req->handle);
	offset = sys_le16_to_cpu(req->offset);

	BT_DBG("handle 0x%04x offset %u", handle, offset);

	return att_prep_write_rsp(chan, handle, offset, buf->data, buf->len);
#endif /* CONFIG_BT_ATT_PREPARE_COUNT */
}

#if CONFIG_BT_ATT_PREPARE_COUNT > 0
static uint8_t att_exec_write_rsp(struct bt_att_chan *chan, uint8_t flags)
{
	struct bt_conn *conn = chan->chan.chan.conn;
	struct net_buf *buf;
	API_RESULT retval;
	uint8_t err = 0U;
	uint8_t ret = 0;

	while ((buf = net_buf_get(chan->att->prep_queue, osaWaitNone_c))) {
		struct bt_attr_data *data = net_buf_user_data(buf);

		BT_DBG("buf %p handle 0x%04x offset %u", buf, data->handle,
		       data->offset);

		/* Just discard the data if an error was set */
		if (!err && flags == BT_ATT_FLAG_EXEC) {
			err = att_write_rsp(chan, BT_ATT_OP_EXEC_WRITE_REQ, 0,
					    data->handle, data->offset,
					    buf->data, buf->len);
			if (err) {
				/* Respond here since handle is set */
				send_err_rsp(chan, BT_ATT_OP_EXEC_WRITE_REQ,
					     data->handle, err);
			}
		}

		net_buf_unref(buf);
	}

	if (err) {
		return 0;
	}

	/* Generate response */
	retval = BT_att_send_execute_write_rsp
			(
				handle,
			);
	if (API_SUCCESS != retVal)
	{
		ret = BT_ATT_ERR_UNLIKELY;
	}
	return ret;
}
#endif /* CONFIG_BT_ATT_PREPARE_COUNT */


static uint8_t att_exec_write_req(struct bt_att_chan *chan, struct net_buf *buf)
{
#if CONFIG_BT_ATT_PREPARE_COUNT == 0
	return BT_ATT_ERR_NOT_SUPPORTED;
#else
	struct bt_att_exec_write_req *req;

	req = (void *)buf->data;

	BT_DBG("flags 0x%02x", req->flags);

	return att_exec_write_rsp(chan, req->flags);
#endif /* CONFIG_BT_ATT_PREPARE_COUNT */
}

static uint8_t att_write_cmd(struct bt_att_chan *chan, struct net_buf *buf)
{
	uint16_t handle;

	handle = net_buf_pull_le16(buf);

	BT_DBG("handle 0x%04x", handle);

	return att_write_rsp(chan, 0, 0, handle, 0, buf->data, buf->len);
}

#if (defined(CONFIG_BT_SIGNING) && ((CONFIG_BT_SIGNING) > 0U))
static uint8_t att_signed_write_cmd(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_conn *conn = chan->chan.chan.conn;
	struct bt_att_signed_write_cmd *req;
	uint16_t handle;
	int err;

	req = (struct bt_att_signed_write_cmd *)buf->data;

	handle = sys_le16_to_cpu(req->handle);

	BT_DBG("handle 0x%04x", handle);

	/* Verifying data requires full buffer including attribute header */
	net_buf_push(buf, sizeof(struct bt_att_hdr));
	err = bt_smp_sign_verify(conn, buf);
	if (err) {
		BT_ERR("Error verifying data");
		/* No response for this command */
		return 0;
	}

	net_buf_pull(buf, sizeof(struct bt_att_hdr));
	net_buf_pull(buf, sizeof(*req));

	return att_write_rsp(chan, 0, 0, handle, 0, buf->data,
			     buf->len - sizeof(struct bt_att_signature));
}
#endif /* CONFIG_BT_SIGNING */

#if (defined(CONFIG_BT_GATT_CLIENT) && ((CONFIG_BT_GATT_CLIENT) > 0U))
#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
static int att_change_security(struct bt_conn *conn, uint8_t err)
{
	bt_security_t sec;

	switch (err) {
	case BT_ATT_ERR_INSUFFICIENT_ENCRYPTION:
		if (conn->sec_level >= BT_SECURITY_L2)
			return -EALREADY;
		sec = BT_SECURITY_L2;
		break;
	case BT_ATT_ERR_AUTHENTICATION:
		if (conn->sec_level < BT_SECURITY_L2) {
			/* BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part C]
			 * page 375:
			 *
			 * If an LTK is not available, the service request
			 * shall be rejected with the error code 'Insufficient
			 * Authentication'.
			 * Note: When the link is not encrypted, the error code
			 * "Insufficient Authentication" does not indicate that
			 * MITM protection is required.
			 */
			sec = BT_SECURITY_L2;
		} else if (conn->sec_level < BT_SECURITY_L3) {
			/* BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part C]
			 * page 375:
			 *
			 * If an authenticated pairing is required but only an
			 * unauthenticated pairing has occurred and the link is
			 * currently encrypted, the service request shall be
			 * rejected with the error code 'Insufficient
			 * Authentication'.
			 * Note: When unauthenticated pairing has occurred and
			 * the link is currently encrypted, the error code
			 * 'Insufficient Authentication' indicates that MITM
			 * protection is required.
			 */
			sec = BT_SECURITY_L3;
		} else if (conn->sec_level < BT_SECURITY_L4) {
			/* BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part C]
			 * page 375:
			 *
			 * If LE Secure Connections authenticated pairing is
			 * required but LE legacy pairing has occurred and the
			 * link is currently encrypted, the service request
			 * shall be rejected with the error code ''Insufficient
			 * Authentication'.
			 */
			sec = BT_SECURITY_L4;
		} else {
			return -EALREADY;
		}
		break;
	default:
		return -EINVAL;
	}

	return bt_conn_set_security(conn, sec);
}
#endif /* CONFIG_BT_SMP */

static uint8_t att_error_rsp(struct bt_att_chan *chan, struct net_buf *buf)
{
	struct bt_att_error_rsp *rsp;
	uint8_t err;

	rsp = (struct bt_att_error_rsp *)buf->data;

	BT_DBG("request 0x%02x handle 0x%04x error 0x%02x", rsp->request,
	       sys_le16_to_cpu(rsp->handle), rsp->error);

	/* Don't retry if there is no req pending or it has been cancelled */
	if (!chan->req || chan->req == &cancel) {
		err = BT_ATT_ERR_UNLIKELY;
		goto done;
	}

	err = rsp->error;
#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
	/* Check if error can be handled by elevating security. */
	if (!att_change_security(chan->chan.chan.conn, err)) {
		chan->req->retrying = true;
		return 0;
	}
#endif /* CONFIG_BT_SMP */

done:
	return att_handle_rsp(chan, NULL, 0, err);
}

static uint8_t att_handle_find_info_rsp(struct bt_att_chan *chan,
				     struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}

static uint8_t att_handle_find_type_rsp(struct bt_att_chan *chan,
				     struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}

static uint8_t att_handle_read_type_rsp(struct bt_att_chan *chan,
				     struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}

static uint8_t att_handle_read_rsp(struct bt_att_chan *chan,
				struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}

static uint8_t att_handle_read_blob_rsp(struct bt_att_chan *chan,
				     struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}

#if (defined(CONFIG_BT_GATT_READ_MULTIPLE) && ((CONFIG_BT_GATT_READ_MULTIPLE) > 0U))
static uint8_t att_handle_read_mult_rsp(struct bt_att_chan *chan,
				     struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}

#if (defined(CONFIG_BT_EATT) && ((CONFIG_BT_EATT) > 0U))
static uint8_t att_handle_read_mult_vl_rsp(struct bt_att_chan *chan,
					struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}
#endif /* CONFIG_BT_EATT */
#endif /* CONFIG_BT_GATT_READ_MULTIPLE */

static uint8_t att_handle_read_group_rsp(struct bt_att_chan *chan,
				      struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}

static uint8_t att_handle_write_rsp(struct bt_att_chan *chan,
				 struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}

static uint8_t att_handle_prepare_write_rsp(struct bt_att_chan *chan,
					 struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}

static uint8_t att_handle_exec_write_rsp(struct bt_att_chan *chan,
				      struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}

static uint8_t att_notify(struct bt_att_chan *chan, struct net_buf *buf)
{
	uint16_t handle;

	handle = net_buf_pull_le16(buf);

	BT_DBG("chan %p handle 0x%04x", chan, handle);

	bt_gatt_notification(chan->att->conn, handle, buf->data, buf->len);

	return 0;
}

static uint8_t att_indicate(struct bt_att_chan *chan, struct net_buf *buf)
{
	uint16_t handle;
    API_RESULT retval;

	handle = net_buf_pull_le16(buf);

	BT_DBG("chan %p handle 0x%04x", chan, handle);

	bt_gatt_notification(chan->att->conn, handle, buf->data, buf->len);

    retval = BT_att_send_hndl_val_cnf(&chan->handle);
    assert(API_SUCCESS == retval);
    (void)retval;

	return 0;
}

static uint8_t att_notify_mult(struct bt_att_chan *chan, struct net_buf *buf)
{
	BT_DBG("chan %p", chan);

	bt_gatt_mult_notification(chan->att->conn, buf->data, buf->len);

	return 0;
}
#endif /* CONFIG_BT_GATT_CLIENT */

static uint8_t att_confirm(struct bt_att_chan *chan, struct net_buf *buf)
{
	BT_DBG("");

	return att_handle_rsp(chan, buf->data, buf->len, 0);
}

static uint8_t att_notify_cfm(struct bt_att_chan *chan, struct net_buf *buf)
{
	uint16_t handle;

	handle = net_buf_pull_le16(buf);

	BT_DBG("chan %p handle 0x%04x", chan, handle);

    (void)handle;

	return 0;
}

static const struct att_handler {
	uint8_t       op;
	uint8_t       expect_len;
	att_type_t type;
	uint8_t       (*func)(struct bt_att_chan *chan, struct net_buf *buf);
} handlers[] = {
	{ BT_ATT_OP_NOTIFY + 1,
		0,
		ATT_UNKNOWN,
		att_notify_cfm },
	{ BT_ATT_OP_MTU_REQ,
		sizeof(struct bt_att_exchange_mtu_req),
		ATT_REQUEST,
		att_mtu_req },
	{ BT_ATT_OP_FIND_INFO_REQ,
		sizeof(struct bt_att_find_info_req),
		ATT_REQUEST,
		att_find_info_req },
	{ BT_ATT_OP_FIND_TYPE_REQ,
		sizeof(struct bt_att_find_type_req),
		ATT_REQUEST,
		att_find_type_req },
	{ BT_ATT_OP_READ_TYPE_REQ,
		sizeof(struct bt_att_read_type_req),
		ATT_REQUEST,
		att_read_type_req },
	{ BT_ATT_OP_READ_REQ,
		sizeof(struct bt_att_read_req),
		ATT_REQUEST,
		att_read_req },
	{ BT_ATT_OP_READ_BLOB_REQ,
		sizeof(struct bt_att_read_blob_req),
		ATT_REQUEST,
		att_read_blob_req },
#if (defined(CONFIG_BT_GATT_READ_MULTIPLE) && ((CONFIG_BT_GATT_READ_MULTIPLE) > 0U))
	{ BT_ATT_OP_READ_MULT_REQ,
		BT_ATT_READ_MULT_MIN_LEN_REQ,
		ATT_REQUEST,
		att_read_mult_req },
#if (defined(CONFIG_BT_EATT) && ((CONFIG_BT_EATT) > 0U))
	{ BT_ATT_OP_READ_MULT_VL_REQ,
		BT_ATT_READ_MULT_MIN_LEN_REQ,
		ATT_REQUEST,
		att_read_mult_vl_req },
#endif /* CONFIG_BT_EATT */
#endif /* CONFIG_BT_GATT_READ_MULTIPLE */
	{ BT_ATT_OP_READ_GROUP_REQ,
		sizeof(struct bt_att_read_group_req),
		ATT_REQUEST,
		att_read_group_req },
	{ BT_ATT_OP_WRITE_REQ,
		sizeof(struct bt_att_write_req),
		ATT_REQUEST,
		att_write_req },
	{ BT_ATT_OP_PREPARE_WRITE_REQ,
		sizeof(struct bt_att_prepare_write_req),
		ATT_REQUEST,
		att_prepare_write_req },
	{ BT_ATT_OP_EXEC_WRITE_REQ,
		sizeof(struct bt_att_exec_write_req),
		ATT_REQUEST,
		att_exec_write_req },
	{ BT_ATT_OP_CONFIRM,
		0,
		ATT_CONFIRMATION,
		att_confirm },
	{ BT_ATT_OP_WRITE_CMD,
		sizeof(struct bt_att_write_cmd),
		ATT_COMMAND,
		att_write_cmd },
#if (defined(CONFIG_BT_SIGNING) && ((CONFIG_BT_SIGNING) > 0U))
	{ BT_ATT_OP_SIGNED_WRITE_CMD,
		(sizeof(struct bt_att_write_cmd) +
		 sizeof(struct bt_att_signature)),
		ATT_COMMAND,
		att_signed_write_cmd },
#endif /* CONFIG_BT_SIGNING */
#if (defined(CONFIG_BT_GATT_CLIENT) && ((CONFIG_BT_GATT_CLIENT) > 0U))
	{ BT_ATT_OP_ERROR_RSP,
		sizeof(struct bt_att_error_rsp),
		ATT_RESPONSE,
		att_error_rsp },
	{ BT_ATT_OP_MTU_RSP,
		sizeof(struct bt_att_exchange_mtu_rsp),
		ATT_RESPONSE,
		att_mtu_rsp },
	{ BT_ATT_OP_FIND_INFO_RSP,
		sizeof(struct bt_att_find_info_rsp),
		ATT_RESPONSE,
		att_handle_find_info_rsp },
	{ BT_ATT_OP_FIND_TYPE_RSP,
		sizeof(struct bt_att_handle_group),
		ATT_RESPONSE,
		att_handle_find_type_rsp },
	{ BT_ATT_OP_READ_TYPE_RSP,
		sizeof(struct bt_att_read_type_rsp),
		ATT_RESPONSE,
		att_handle_read_type_rsp },
	{ BT_ATT_OP_READ_RSP,
		0,
		ATT_RESPONSE,
		att_handle_read_rsp },
	{ BT_ATT_OP_READ_BLOB_RSP,
		0,
		ATT_RESPONSE,
		att_handle_read_blob_rsp },
#if (defined(CONFIG_BT_GATT_READ_MULTIPLE) && ((CONFIG_BT_GATT_READ_MULTIPLE) > 0U))
	{ BT_ATT_OP_READ_MULT_RSP,
		0,
		ATT_RESPONSE,
		att_handle_read_mult_rsp },
#if (defined(CONFIG_BT_EATT) && ((CONFIG_BT_EATT) > 0U))
	{ BT_ATT_OP_READ_MULT_VL_RSP,
		sizeof(struct bt_att_read_mult_vl_rsp),
		ATT_RESPONSE,
		att_handle_read_mult_vl_rsp },
#endif /* CONFIG_BT_EATT */
#endif /* CONFIG_BT_GATT_READ_MULTIPLE */
	{ BT_ATT_OP_READ_GROUP_RSP,
		sizeof(struct bt_att_read_group_rsp),
		ATT_RESPONSE,
		att_handle_read_group_rsp },
	{ BT_ATT_OP_WRITE_RSP,
		0,
		ATT_RESPONSE,
		att_handle_write_rsp },
	{ BT_ATT_OP_PREPARE_WRITE_RSP,
		sizeof(struct bt_att_prepare_write_rsp),
		ATT_RESPONSE,
		att_handle_prepare_write_rsp },
	{ BT_ATT_OP_EXEC_WRITE_RSP,
		0,
		ATT_RESPONSE,
		att_handle_exec_write_rsp },
	{ BT_ATT_OP_NOTIFY,
		sizeof(struct bt_att_notify),
		ATT_NOTIFICATION,
		att_notify },
	{ BT_ATT_OP_INDICATE,
		sizeof(struct bt_att_indicate),
		ATT_INDICATION,
		att_indicate },
	{ BT_ATT_OP_NOTIFY_MULT,
		sizeof(struct bt_att_notify_mult),
		ATT_NOTIFICATION,
		att_notify_mult },
#endif /* CONFIG_BT_GATT_CLIENT */
};

static att_type_t att_op_get_type(uint8_t op)
{
	switch (op) {
	case BT_ATT_OP_MTU_REQ:
	case BT_ATT_OP_FIND_INFO_REQ:
	case BT_ATT_OP_FIND_TYPE_REQ:
	case BT_ATT_OP_READ_TYPE_REQ:
	case BT_ATT_OP_READ_REQ:
	case BT_ATT_OP_READ_BLOB_REQ:
	case BT_ATT_OP_READ_MULT_REQ:
	case BT_ATT_OP_READ_GROUP_REQ:
	case BT_ATT_OP_WRITE_REQ:
	case BT_ATT_OP_PREPARE_WRITE_REQ:
	case BT_ATT_OP_EXEC_WRITE_REQ:
		return ATT_REQUEST;
	case BT_ATT_OP_CONFIRM:
		return ATT_CONFIRMATION;
	case BT_ATT_OP_WRITE_CMD:
	case BT_ATT_OP_SIGNED_WRITE_CMD:
		return ATT_COMMAND;
	case BT_ATT_OP_ERROR_RSP:
	case BT_ATT_OP_MTU_RSP:
	case BT_ATT_OP_FIND_INFO_RSP:
	case BT_ATT_OP_FIND_TYPE_RSP:
	case BT_ATT_OP_READ_TYPE_RSP:
	case BT_ATT_OP_READ_RSP:
	case BT_ATT_OP_READ_BLOB_RSP:
	case BT_ATT_OP_READ_MULT_RSP:
	case BT_ATT_OP_READ_GROUP_RSP:
	case BT_ATT_OP_WRITE_RSP:
	case BT_ATT_OP_PREPARE_WRITE_RSP:
	case BT_ATT_OP_EXEC_WRITE_RSP:
		return ATT_RESPONSE;
	case BT_ATT_OP_NOTIFY:
		return ATT_NOTIFICATION;
	case BT_ATT_OP_INDICATE:
		return ATT_INDICATION;
	}

	if (op & ATT_CMD_MASK) {
		return ATT_COMMAND;
	}

	return ATT_UNKNOWN;
}

static int bt_att_recv(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
	struct bt_att_chan *att_chan = ATT_CHAN(chan);
	struct bt_att_hdr *hdr;
	const struct att_handler *handler;
	uint8_t err;
	size_t i;

	if (buf->len < sizeof(*hdr)) {
		BT_ERR("Too small ATT PDU received");
		return 0;
	}

	hdr = (struct bt_att_hdr *)net_buf_pull_mem(buf, sizeof(*hdr));
	BT_DBG("Received ATT chan %p code 0x%02x len %lu", att_chan, hdr->code,
	       net_buf_frags_len(buf));

	if (!att_chan->att) {
		BT_DBG("Ignore recv on detached ATT chan");
		return 0;
	}

	for (i = 0, handler = NULL; i < ARRAY_SIZE(handlers); i++) {
		if (hdr->code == handlers[i].op) {
			handler = &handlers[i];
			break;
		}
	}

	if (!handler) {
		BT_WARN("Unhandled ATT code 0x%02x", hdr->code);
		if (att_op_get_type(hdr->code) != ATT_COMMAND) {
			send_err_rsp(att_chan, hdr->code, 0,
				     BT_ATT_ERR_NOT_SUPPORTED);
		}
		return 0;
	}

	if (IS_ENABLED(CONFIG_BT_ATT_ENFORCE_FLOW)) {
		if (handler->type == ATT_REQUEST &&
		    atomic_test_and_set_bit(att_chan->flags, ATT_PENDING_RSP)) {
			BT_WARN("Ignoring unexpected request");
			return 0;
		} else if (handler->type == ATT_INDICATION &&
			   atomic_test_and_set_bit(att_chan->flags,
						   ATT_PENDING_CFM)) {
			BT_WARN("Ignoring unexpected indication");
			return 0;
		}
	}

	if (buf->len < handler->expect_len) {
		BT_ERR("Invalid len %u for code 0x%02x", buf->len, hdr->code);
		err = BT_ATT_ERR_INVALID_PDU;
	} else {
		err = handler->func(att_chan, buf);
	}

	if (handler->type == ATT_REQUEST && err) {
		BT_DBG("ATT error 0x%02x", err);
		send_err_rsp(att_chan, hdr->code, 0, err);
	}

	return 0;
}

static struct bt_att *att_get(struct bt_conn *conn)
{
	struct bt_l2cap_chan *chan;
	struct bt_att_chan *att_chan;

	if (conn->state != BT_CONN_CONNECTED) {
		BT_WARN("Not connected");
		return NULL;
	}

	chan = bt_l2cap_le_lookup_rx_cid(conn, BT_L2CAP_CID_ATT);
	if (!chan) {
		BT_ERR("Unable to find ATT channel");
		return NULL;
	}

	att_chan = ATT_CHAN(chan);
	if (atomic_test_bit(att_chan->flags, ATT_DISCONNECTED)) {
		BT_WARN("ATT channel flagged as disconnected");
		return NULL;
	}

	return att_chan->att;
}

static struct bt_att_chan *att_chan_get(struct bt_conn *conn)
{
	struct bt_l2cap_chan *chan;
	struct bt_att_chan *att_chan;

	if (conn->state != BT_CONN_CONNECTED) {
		BT_WARN("Not connected");
		return NULL;
	}

	chan = bt_l2cap_le_lookup_rx_cid(conn, BT_L2CAP_CID_ATT);
	if (!chan) {
		BT_ERR("Unable to find ATT channel");
		return NULL;
	}

	att_chan = ATT_CHAN(chan);
	if (atomic_test_bit(att_chan->flags, ATT_DISCONNECTED)) {
		BT_WARN("ATT channel flagged as disconnected");
		return NULL;
	}

	return att_chan;
}

struct net_buf *bt_att_create_pdu(struct bt_conn *conn, uint8_t op, size_t len)
{
	struct bt_att *att;
	struct bt_att_chan *chan, *tmp;

	att = att_get(conn);
	if (!att) {
		return NULL;
	}

	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&att->chans, chan, tmp, node, struct bt_att_chan) {
		if (len + sizeof(op) > chan->chan.tx.mtu) {
			continue;
		}

		return bt_att_chan_create_pdu(chan, op, len);
	}

	BT_WARN("No ATT channel for MTU %lu", len + sizeof(op));

	return NULL;
}

static void att_reset(struct bt_att *att)
{
	struct bt_att_req *req, *tmp;
	struct net_buf *buf;
    osa_status_t ret;

#if CONFIG_BT_ATT_PREPARE_COUNT > 0
	/* Discard queued buffers */
	while ((buf = net_buf_get(att->prep_queue, osaWaitNone_c))) {
		net_buf_unref(buf);
	}
	(void)OSA_MsgQDestroy(att->prep_queue);
#endif /* CONFIG_BT_ATT_PREPARE_COUNT > 0 */

	while ((buf = net_buf_get(att->tx_queue, osaWaitNone_c))) {
		net_buf_unref(buf);
	}
	(void)OSA_MsgQDestroy(att->tx_queue);

	att->conn = NULL;

	/* Notify pending requests */
	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&att->reqs, req, tmp, node, struct bt_att_req) {
		if (req->func) {
			req->func(NULL, BT_ATT_ERR_UNLIKELY, NULL, 0,
				  req->user_data);
		}

		bt_att_req_free(req);
	}

    ret = OSA_MsgQPut(att_slab, &att);
    assert (KOSA_StatusSuccess == ret);
    (void)ret;
}

static void att_chan_detach(struct bt_att_chan *chan)
{
	struct net_buf *buf;

	BT_DBG("chan %p", chan);

	sys_slist_find_and_remove(&chan->att->chans, &chan->node);

	/* Release pending buffers */
	while ((buf = net_buf_get(chan->tx_queue, osaWaitNone_c))) {
		net_buf_unref(buf);
	}

	/* Release pending buffers */
	while ((buf = net_buf_get(chan->rx_queue, osaWaitNone_c))) {
		net_buf_unref(buf);
	}

	if (chan->req) {
		/* Notify outstanding request */
		att_handle_rsp(chan, NULL, 0, BT_ATT_ERR_UNLIKELY);
	}

	chan->att = NULL;
}

static void rx_async_work(struct k_work *work)
{
	struct bt_att_chan *chan = CONTAINER_OF(work, struct bt_att_chan,
						rx_work);
	struct bt_l2cap_le_chan *ch = &chan->chan;
	struct net_buf *buf = NULL;
	osa_status_t ret;
	int err = -ENOENT;

	BT_DBG("RX workqueue");

	do
	{
		ret = OSA_MsgQGet(chan->rx_queue, &buf, (uint32_t)osaWaitNone_c);
		if (KOSA_StatusSuccess != ret)
		{
			buf = NULL;
		}
		else
		{
			err = bt_att_recv(&ch->chan, buf);
			net_buf_unref(buf);
		}
	} while (NULL != buf);
	(void)err;
}

static void att_timeout(struct k_work *work)
{
	struct bt_att_chan *chan = CONTAINER_OF(work, struct bt_att_chan,
						timeout_work);

	BT_ERR("ATT Timeout");

	/* BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part F] page 480:
	 *
	 * A transaction not completed within 30 seconds shall time out. Such a
	 * transaction shall be considered to have failed and the local higher
	 * layers shall be informed of this failure. No more attribute protocol
	 * requests, commands, indications or notifications shall be sent to the
	 * target device on this ATT Bearer.
	 */
	bt_att_disconnected(&chan->chan.chan);
}

static struct bt_att_chan *att_get_fixed_chan(struct bt_conn *conn)
{
	struct bt_l2cap_chan *chan;

	chan = bt_l2cap_le_lookup_tx_cid(conn, BT_L2CAP_CID_ATT);
	__ASSERT(chan, "No ATT channel found");

	return ATT_CHAN(chan);
}

static void att_chan_attach(struct bt_att *att, struct bt_att_chan *chan)
{
	BT_DBG("att %p chan %p flags %u", att, chan, atomic_get(chan->flags));

	if (sys_slist_is_empty(&att->chans)) {
        osa_status_t ret;
		/* Init general queues when attaching the first channel */
		if (NULL == att->tx_queue)
		{
			ret = OSA_MsgQCreate((osa_msgq_handle_t)att->tx_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void*));
			assert(KOSA_StatusSuccess == ret);
			if (KOSA_StatusSuccess == ret)
			{
				att->tx_queue = (osa_msgq_handle_t)att->tx_queue_handle;
			}
		}
#if CONFIG_BT_ATT_PREPARE_COUNT > 0
		if (NULL == att->prep_queue)
		{
			ret = OSA_MsgQCreate((osa_msgq_handle_t)att->prep_queue, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void*));
			assert(KOSA_StatusSuccess == ret);
			if (KOSA_StatusSuccess == ret)
			{
				att->prep_queue = (osa_msgq_handle_t)att->prep_queue_handle;
			}
		}
#endif
	}

	sys_slist_prepend(&att->chans, &chan->node);
}

static void bt_att_connected(struct bt_l2cap_chan *chan)
{
	struct bt_att_chan *att_chan = att_get_fixed_chan(chan->conn);
	struct bt_att *att = att_chan->att;
	struct bt_l2cap_le_chan *ch = BT_L2CAP_LE_CHAN(chan);

	BT_DBG("chan %p cid 0x%04x", ch, ch->tx.cid);

	att_chan = ATT_CHAN(chan);

	att_chan_attach(att, att_chan);

	if (!atomic_test_bit(att_chan->flags, ATT_ENHANCED)) {
		ch->tx.mtu = BT_ATT_DEFAULT_LE_MTU;
		ch->rx.mtu = BT_ATT_DEFAULT_LE_MTU;
	}
	k_work_init_delayable(&att_chan->timeout_work, att_timeout);
}

static void bt_att_disconnected(struct bt_l2cap_chan *chan)
{
	struct bt_att_chan *att_chan = ATT_CHAN(chan);
	struct bt_att *att = att_chan->att;
	struct bt_l2cap_le_chan *ch = BT_L2CAP_LE_CHAN(chan);

	BT_DBG("chan %p cid 0x%04x", ch, ch->tx.cid);

	if (!att_chan->att) {
		BT_DBG("Ignore disconnect on detached ATT chan");
		return;
	}

	att_chan_detach(att_chan);

	/* Don't reset if there are still channels to be used */
	if (!sys_slist_is_empty(&att->chans)) {
		return;
	}

	att_reset(att);

	bt_gatt_disconnected(ch->chan.conn);
}

#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
static uint8_t att_req_retry(struct bt_att_chan *att_chan)
{
	struct bt_att_req *req = att_chan->req;
	struct net_buf *buf;

	/* Resend buffer */
	if (!req->encode) {
		/* This request does not support resending */
		return BT_ATT_ERR_AUTHENTICATION;
	}


	buf = bt_att_chan_create_pdu(att_chan, req->att_op, req->len);
	if (!buf) {
		return BT_ATT_ERR_UNLIKELY;
	}

	if (req->encode(buf, req->len, req->user_data)) {
		net_buf_unref(buf);
		return BT_ATT_ERR_UNLIKELY;
	}

	if (chan_send(att_chan, buf, NULL)) {
		net_buf_unref(buf);
		return BT_ATT_ERR_UNLIKELY;
	}

	return BT_ATT_ERR_SUCCESS;
}

static void bt_att_encrypt_change(struct bt_l2cap_chan *chan,
				  uint8_t hci_status)
{
	struct bt_att_chan *att_chan = ATT_CHAN(chan);
	struct bt_l2cap_le_chan *ch = BT_L2CAP_LE_CHAN(chan);
	struct bt_conn *conn = ch->chan.conn;
	uint8_t err;

	BT_DBG("chan %p conn %p handle %u sec_level 0x%02x status 0x%02x", ch,
	       conn, conn->handle, conn->sec_level, hci_status);

	if (!att_chan->att) {
		BT_DBG("Ignore encrypt change on detached ATT chan");
		return;
	}

	/*
	 * If status (HCI status of security procedure) is non-zero, notify
	 * outstanding request about security failure.
	 */
	if (hci_status) {
		if (att_chan->req && att_chan->req->retrying) {
			att_handle_rsp(att_chan, NULL, 0,
				       BT_ATT_ERR_AUTHENTICATION);
		}

		return;
	}

	bt_gatt_encrypt_change(conn);

	if (conn->sec_level == BT_SECURITY_L1) {
		return;
	}

	if (!(att_chan->req && att_chan->req->retrying)) {
		return;
	}

	BT_DBG("Retrying");

	err = att_req_retry(att_chan);
	if (err) {
		BT_DBG("Retry failed (%d)", err);
		att_handle_rsp(att_chan, NULL, 0, err);
	}
}
#endif /* CONFIG_BT_SMP */

static void bt_att_status(struct bt_l2cap_chan *ch, atomic_t *status)
{
	struct bt_att_chan *chan = ATT_CHAN(ch);
	sys_snode_t *node;

	BT_DBG("chan %p status %p", ch, status);

	if (!atomic_test_bit(status, BT_L2CAP_STATUS_OUT)) {
		return;
	}

	if (!chan->att) {
		BT_DBG("Ignore status on detached ATT chan");
		return;
	}

	/* If there is a request pending don't attempt to send */
	if (chan->req) {
		return;
	}

	/* Pull next request from the list */
	node = sys_slist_get(&chan->att->reqs);
	if (!node) {
		return;
	}

	if (bt_att_chan_req_send(chan, ATT_REQ(node)) >= 0) {
		return;
	}

	/* Prepend back to the list as it could not be sent */
	sys_slist_prepend(&chan->att->reqs, node);
}

static void bt_att_released(struct bt_l2cap_chan *ch)
{
	struct bt_att_chan *chan = ATT_CHAN(ch);
    osa_status_t ret;

	BT_DBG("chan %p", chan);

	if (NULL != chan->tx_queue)
	{
		(void)OSA_MsgQDestroy(chan->tx_queue);
		chan->tx_queue = NULL;
	}

	if (NULL != chan->rx_queue)
	{
		(void)OSA_MsgQDestroy(chan->rx_queue);
		chan->rx_queue = NULL;
	}

    ret = OSA_MsgQPut(chan_slab, &chan);
    assert (KOSA_StatusSuccess == ret);
    (void)ret;
}

static struct bt_att_chan *att_chan_new(struct bt_att *att, atomic_val_t flags)
{
	int quota = 0;
	static struct bt_l2cap_chan_ops ops = {
		.connected = bt_att_connected,
		.disconnected = bt_att_disconnected,
		.recv = bt_att_recv,
		.sent = bt_att_sent,
		.status = bt_att_status,
#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
		.encrypt_change = bt_att_encrypt_change,
#endif /* CONFIG_BT_SMP */
		.released = bt_att_released,
	};
	struct bt_att_chan *chan;
    osa_status_t ret;

	SYS_SLIST_FOR_EACH_CONTAINER(&att->chans, chan, node, struct bt_att_chan) {
		if (chan->att == att) {
			quota++;
		}

		if (quota == ATT_CHAN_MAX) {
			BT_ERR("Maximum number of channels reached: %d", quota);
			return NULL;
		}
	}

    ret = OSA_MsgQGet(chan_slab, &chan, (uint32_t)osaWaitNone_c);
	if (KOSA_StatusSuccess != ret) {
		BT_ERR("No available ATT channel for conn %p", att->conn);
		return NULL;
	}

	(void)memset(chan, 0, sizeof(*chan));
	chan->chan.chan.ops = &ops;

	k_work_init(&chan->rx_work, rx_async_work);

	if (NULL == chan->tx_queue)
	{
		ret = OSA_MsgQCreate(chan->tx_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void*));
		if (KOSA_StatusSuccess == ret)
		{
			chan->tx_queue = (osa_msgq_handle_t)chan->tx_queue_handle;
		}
		assert(KOSA_StatusSuccess == ret);
	}

	if (NULL == chan->rx_queue)
	{
		ret = OSA_MsgQCreate(chan->rx_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void*));
		if (KOSA_StatusSuccess == ret)
		{
			chan->rx_queue = (osa_msgq_handle_t)chan->rx_queue_handle;
		}
		assert(KOSA_StatusSuccess == ret);
	}

	atomic_set(chan->flags, flags);
	chan->att = att;

	return chan;
}

static int bt_att_accept(struct bt_conn *conn, struct bt_l2cap_chan **ch)
{
	struct bt_att *att;
	struct bt_att_chan *chan;
    ATT_HANDLE *handle;
    osa_status_t ret;

	BT_DBG("conn %p handle %u", conn, conn->handle);

    handle = ethermind_att_handle_lookup_by_device_id(conn->deviceId);
	if (NULL == handle) {
		BT_ERR("ATT Handle is not found of conn %p", conn);
		return -EINVAL;
	}

	ret = OSA_MsgQGet(att_slab, &att, (uint32_t)osaWaitNone_c);
	if (KOSA_StatusSuccess != ret) {
		BT_ERR("No available ATT context for conn %p", conn);
		return -ENOMEM;
	}

	(void)memset(att, 0, sizeof(*att));
	att->conn = conn;
	sys_slist_init(&att->reqs);
	sys_slist_init(&att->chans);

	chan = att_chan_new(att, 0);
	if (!chan) {
		return -ENOMEM;
	}

    chan->handle = *handle;

	*ch = &chan->chan.chan;

	return 0;
}

BT_L2CAP_CHANNEL_DEFINE(att_fixed_chan, BT_L2CAP_CID_ATT, bt_att_accept, NULL);


#if (defined(CONFIG_BT_EATT) && ((CONFIG_BT_EATT) > 0U))
int bt_eatt_connect(struct bt_conn *conn, uint8_t num_channels)
{
	struct bt_att_chan *att_chan = att_get_fixed_chan(conn);
	struct bt_att *att = att_chan->att;
	struct bt_l2cap_chan *chan[CONFIG_BT_EATT_MAX] = {{0}};
	int i = 0;

	if (num_channels > CONFIG_BT_EATT_MAX) {
		return -EINVAL;
	}

	while (num_channels--) {
		att_chan = att_chan_new(att, BIT(ATT_ENHANCED));
		if (!att_chan) {
			break;
		}

		chan[i] = &att_chan->chan.chan;
		i++;
	}

	if (!i) {
		return -ENOMEM;
	}

	return bt_l2cap_ecred_chan_connect(conn, chan, BT_EATT_PSM);
}

int bt_eatt_disconnect(struct bt_conn *conn)
{
	struct bt_att_chan *chan = att_get_fixed_chan(conn);
	struct bt_att *att = chan->att;
	int err = -ENOTCONN;

	if (!conn) {
		return -EINVAL;
	}

	SYS_SLIST_FOR_EACH_CONTAINER(&att->chans, chan, node, struct bt_att_chan) {
		if (atomic_test_bit(chan->flags, ATT_ENHANCED)) {
			err = bt_l2cap_chan_disconnect(&chan->chan.chan);
		}
	}

	return err;
}

#endif /* CONFIG_BT_EATT */

static int bt_eatt_accept(struct bt_conn *conn, struct bt_l2cap_chan **chan)
{
	struct bt_att_chan *att_chan = att_get_fixed_chan(conn);
	struct bt_att *att = att_chan->att;

	BT_DBG("conn %p handle %u", conn, conn->handle);

	att_chan = att_chan_new(att, BIT(ATT_ENHANCED));
	if (att_chan) {
		*chan = &att_chan->chan.chan;
		return 0;
	}

	return -ENOMEM;
}

static void bt_eatt_init(void)
{
	int err;
	static struct bt_l2cap_server eatt_l2cap = {
		.psm = BT_EATT_PSM,
#if (defined(CONFIG_BT_EATT_SEC_LEVEL) && ((CONFIG_BT_EATT_SEC_LEVEL) > 0U))
		.sec_level = CONFIG_BT_EATT_SEC_LEVEL,
#endif
		.accept = bt_eatt_accept,
	};

	BT_DBG("");

	err = bt_l2cap_server_register(&eatt_l2cap);
	if (err < 0) {
		BT_ERR("EATT Server registration failed %d", err);
	}
}

static int ethermind_att_handle_alloc(ATT_HANDLE *handle)
{
    size_t index;
    unsigned int reg;

    reg = DisableGlobalIRQ();
    for (index = 0;index < CONFIG_BT_MAX_CONN;index++)
    {
        if (false == att_handle_node[index].used)
        {
            att_handle_node[index].used = true;
            att_handle_node[index].handle = *handle;
            EnableGlobalIRQ(reg);
            return 0;
        }
    }
    EnableGlobalIRQ(reg);
    return -ENOSR;
}

static int ethermind_att_handle_free(ATT_HANDLE *handle)
{
    size_t index;
    unsigned int reg;

    reg = DisableGlobalIRQ();
    for (index = 0;index < CONFIG_BT_MAX_CONN;index++)
    {
        if (true == att_handle_node[index].used)
        {
            if (att_handle_node[index].handle.att_id == handle->att_id)
            {
                att_handle_node[index].used = false;
                att_handle_node[index].handle.att_id = 0;
                att_handle_node[index].handle.device_id = 0;
                EnableGlobalIRQ(reg);
                return 0;
            }
        }
    }
    EnableGlobalIRQ(reg);
    return -ENXIO;
}

static ATT_HANDLE *ethermind_att_handle_lookup_by_device_id(uint8_t device_id)
{
    size_t index;
    unsigned int reg;

    reg = DisableGlobalIRQ();
    for (index = 0;index < CONFIG_BT_MAX_CONN;index++)
    {
        if (true == att_handle_node[index].used)
        {
            if (att_handle_node[index].handle.device_id == device_id)
            {
                EnableGlobalIRQ(reg);
                return &att_handle_node[index].handle;
            }
        }
    }
    EnableGlobalIRQ(reg);
    return NULL;
}

static API_RESULT ethermind_bt_att_cb
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               UCHAR         * eventdata,
               UINT16        event_datalen
           )
{
	struct bt_att_chan *attChan = NULL;
    struct bt_conn *conn = NULL;
	int err = -ENOENT;
    osa_status_t ret;

    BT_DBG("att rx %d", att_event);

    conn = bt_conn_lookup_device_id(handle->device_id);

    if (NULL != conn)
    {
        attChan = att_chan_get(conn);
    }

    if (ATT_CONNECTION_IND == att_event)
    {
        err = ethermind_att_handle_alloc(handle);
        assert(0 == err);
    }
    else if (ATT_DISCONNECTION_IND == att_event)
    {
        err = ethermind_att_handle_free(handle);
        assert(0 == err);
    }
    else
    {
        if (NULL != attChan)
        {
            struct net_buf *buf;

            attChan->handle = *handle;
            buf = net_buf_alloc(&att_pool, 0);
            if (NULL != buf)
            {
                struct bt_att_hdr hdr;

                hdr.code = att_event;
                (void)net_buf_add_mem(buf, &hdr, sizeof(hdr));
                (void)net_buf_add_mem(buf, eventdata, event_datalen);
                BT_DBG("RX queue put buf %p", buf);
                ret = OSA_MsgQPut(attChan->rx_queue, &buf);
                assert(KOSA_StatusSuccess == ret);
                if (KOSA_StatusSuccess != ret)
                {
                    BT_ERR("RX queue put failure!");
                    net_buf_unref(buf);
                }
				k_work_submit(&attChan->rx_work);
            }
            else
            {
                BT_ERR("Get RX buf failure!");
            }
        }
    }
    if (NULL != conn)
    {
        bt_conn_unref(conn);
    }
    return (0 == err) ? API_SUCCESS : API_FAILURE;
}

void bt_att_init(void)
{
	ATT_APPLICATION attInterface;
    osa_status_t ret;

	if (!atomic_cas(&init, 0, 1)) {
		return;
	}

	if (NULL == att_slab)
    {
        ret = OSA_MsgQCreate((osa_msgq_handle_t)att_slab_handle, CONFIG_BT_MAX_CONN, sizeof(void*));
        assert(KOSA_StatusSuccess == ret);
        if (KOSA_StatusSuccess != ret)
        {
            return;
        }
        att_slab = (osa_msgq_handle_t)att_slab_handle;

        for (size_t index = 0;index < CONFIG_BT_MAX_CONN;index++)
        {
            void *node = (void *)&att_slab_pool[index];
            ret = OSA_MsgQPut(att_slab, (void *)&node);
            assert(KOSA_StatusSuccess == ret);
        }
    }

    if (NULL == chan_slab)
    {
        ret = OSA_MsgQCreate((osa_msgq_handle_t)chan_slab_handle, CONFIG_BT_MAX_CONN * ATT_CHAN_MAX, sizeof(void*));
        assert(KOSA_StatusSuccess == ret);
        if (KOSA_StatusSuccess != ret)
        {
            return;
        }
        chan_slab = (osa_msgq_handle_t)chan_slab_handle;

        for (size_t index = 0;index < (CONFIG_BT_MAX_CONN * ATT_CHAN_MAX);index++)
        {
            void *node = (void *)&chan_slab_pool[index];
            ret = OSA_MsgQPut(chan_slab, (void *)&node);
            assert(KOSA_StatusSuccess == ret);
        }
    }

	attInterface.cb = ethermind_bt_att_cb;
	(void)BT_att_register (&attInterface);

	bt_gatt_init();

	if (IS_ENABLED(CONFIG_BT_EATT)) {
		bt_eatt_init();
	}
}

ATT_HANDLE *bt_att_lookup_conn(struct bt_conn *conn)
{
	struct bt_att_chan *attChan = att_chan_get(conn);
	if (NULL != attChan)
	{
		return &attChan->handle;
	}
	else
	{
		return NULL;
	}
}

uint16_t bt_att_get_mtu(struct bt_conn *conn)
{
	struct bt_att_chan *chan, *tmp;
	struct bt_att *att;
	uint16_t mtu = 0;

	att = att_get(conn);
	if (!att) {
		return 0;
	}

	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&att->chans, chan, tmp, node, struct bt_att_chan) {
		if (chan->chan.tx.mtu > mtu) {
			mtu = chan->chan.tx.mtu;
		}
	}

	return mtu;
}

struct bt_att_req *bt_att_req_alloc(size_t timeout)
{
	struct bt_att_req *req = NULL;
    osa_status_t ret;

    if (NULL == req_slab)
    {
        ret = OSA_MsgQCreate((osa_msgq_handle_t)req_slab_handle, CONFIG_BT_ATT_TX_MAX, sizeof(void*));
        assert(KOSA_StatusSuccess == ret);
        if (KOSA_StatusSuccess != ret)
        {
            return NULL;
        }
        req_slab = (osa_msgq_handle_t)req_slab_handle;

        for (size_t index = 0;index < CONFIG_BT_ATT_TX_MAX;index++)
        {
            void *node = (void *)&req_slab_pool[index];
            ret = OSA_MsgQPut(req_slab, (void *)&node);
            assert(KOSA_StatusSuccess == ret);
        }
    }
	/* Reserve space for request */
    ret = OSA_MsgQGet(req_slab, &req, (uint32_t)timeout);
	if (KOSA_StatusSuccess != ret) {
		return NULL;
	}

	BT_DBG("req %p", req);

	memset(req, 0, sizeof(*req));

	return req;
}

void bt_att_req_free(struct bt_att_req *req)
{
    osa_status_t ret;

	BT_DBG("req %p", req);

	if (req->buf) {
		net_buf_unref(req->buf);
		req->buf = NULL;
	}

    ret = OSA_MsgQPut(req_slab, &req);
    assert (KOSA_StatusSuccess == ret);
    (void)ret;
}

int bt_att_send(struct bt_conn *conn, struct net_buf *buf, bt_conn_tx_cb_t cb,
		void *user_data)
{
	struct bt_att_chan *chan, *tmp;
	struct bt_att *att;
	int ret;

	assert(conn);
	assert(buf);

	att = att_get(conn);
	if (!att) {
		net_buf_unref(buf);
		return -ENOTCONN;
	}

	/* If callback is set use the fixed channel since bt_l2cap_chan_send
	 * cannot be used with a custom user_data.
	 */
	if (NULL != cb) {
#if 0
		return bt_l2cap_send_cb(conn, BT_L2CAP_CID_ATT, buf, cb,
					user_data);
#endif
	}

	ret = 0;

	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&att->chans, chan, tmp, node, struct bt_att_chan) {
        if (NULL != cb)
        {
            ret = chan_send_cb(chan, buf, cb, user_data);
        }
		else
        {
            ret = bt_att_chan_send(chan, buf, NULL);
        }
		if (ret >= 0) {
			break;
		}
	}

	if ((ret < 0) && (-ENOTSUP != ret) && (NULL == cb)) {
		/* Queue buffer to be send later */
		BT_DBG("Queueing buffer %p", buf);
		net_buf_put(att->tx_queue, buf);
		return 0;
	}
	else
	{
		return ret;
	}
}

int bt_att_req_send(struct bt_conn *conn, struct bt_att_req *req)
{
	struct bt_att *att;
	struct bt_att_chan *chan, *tmp;

	BT_DBG("conn %p req %p", conn, req);

	assert(conn);
	assert(req);

	att = att_get(conn);
	if (!att) {
		return -ENOTCONN;
	}

	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&att->chans, chan, tmp, node, struct bt_att_chan) {
		/* If there is nothing pending use the channel */
		if (NULL == chan->req) {
			int ret;

			ret = bt_att_chan_req_send(chan, req);
			if (ret >= 0) {
				return ret;
			}
		}
	}

	/* Queue the request to be send later */
	sys_slist_append(&att->reqs, &req->node);

	BT_DBG("req %p queued", req);

	return 0;
}

static bool bt_att_chan_req_cancel(struct bt_att_chan *chan,
				   struct bt_att_req *req)
{
	if (chan->req != req) {
		return false;
	}

	chan->req = &cancel;

	bt_att_req_free(req);

	return true;
}

void bt_att_req_cancel(struct bt_conn *conn, struct bt_att_req *req)
{
	struct bt_att *att;
	struct bt_att_chan *chan, *tmp;

	BT_DBG("req %p", req);

	if (!conn || !req) {
		return;
	}

	att = att_get(conn);
	if (!att) {
		return;
	}

	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&att->chans, chan, tmp, node, struct bt_att_chan) {
		/* Check if request is outstanding */
		if (bt_att_chan_req_cancel(chan, req)) {
			return;
		}
	}

	/* Remove request from the list */
	sys_slist_find_and_remove(&att->reqs, &req->node);

	bt_att_req_free(req);
}
