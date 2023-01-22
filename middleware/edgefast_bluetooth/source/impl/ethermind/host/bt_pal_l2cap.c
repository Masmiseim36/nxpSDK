/* l2cap.c - L2CAP handling */

/*
 * Copyright 2021 NXP
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>
#include <string.h>
#include <errno/errno.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>

#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>

#include "BT_common.h"
#include "BT_features.h"
#include "BT_hci_api.h"
#include "l2cap.h"

#ifndef L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT
#error The macro L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT need to be defined!
#endif /* L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT */

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_L2CAP)
#define LOG_MODULE_NAME bt_l2cap
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#include "bt_pal_hci_core.h"
#include "bt_pal_conn_internal.h"
#include "bt_pal_l2cap_internal.h"
#include "bt_pal_keys.h"

#define LE_CHAN_RTX(_w) CONTAINER_OF((_w), struct bt_l2cap_le_chan, rtx_work)
#define CHAN_RX(_w) CONTAINER_OF((_w), struct bt_l2cap_le_chan, rx_work)

#define L2CAP_LE_MIN_MTU		23
#define L2CAP_ECRED_MIN_MTU		64

#if (defined(CONFIG_BT_HCI_ACL_FLOW_CONTROL) && ((CONFIG_BT_HCI_ACL_FLOW_CONTROL) > 0U))
#define L2CAP_LE_MAX_CREDITS		(CONFIG_BT_BUF_ACL_RX_COUNT - 1)
#else
#define L2CAP_LE_MAX_CREDITS		(CONFIG_BT_BUF_EVT_RX_COUNT - 1)
#endif

#define L2CAP_LE_CID_DYN_START	0x0040
#define L2CAP_LE_CID_DYN_END	0x007f
#define L2CAP_LE_CID_IS_DYN(_cid) \
	((_cid) >= L2CAP_LE_CID_DYN_START && (_cid) <= L2CAP_LE_CID_DYN_END)

#define L2CAP_LE_PSM_FIXED_START 0x0001
#define L2CAP_LE_PSM_FIXED_END   0x007f
#define L2CAP_LE_PSM_DYN_START   0x0080
#define L2CAP_LE_PSM_DYN_END     0x00ff
#define L2CAP_LE_PSM_IS_DYN(_psm) \
	((_psm) >= L2CAP_LE_PSM_DYN_START && (_psm) <= L2CAP_LE_PSM_DYN_END)

#define L2CAP_CONN_TIMEOUT  BT_SECONDS(40)
#define L2CAP_DISC_TIMEOUT  BT_SECONDS(2)
#define L2CAP_RTX_TIMEOUT   BT_SECONDS(2)

#define L2CAP_SC_CHECK_TIMEOUT	200

/* Dedicated pool for disconnect buffers so they are guaranteed to be send
 * even in case of data congestion due to flooding.
 */
NET_BUF_POOL_FIXED_DEFINE(disc_pool, 1,
			  BT_L2CAP_BUF_SIZE(CONFIG_BT_L2CAP_TX_MTU), NULL);

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))

#define L2CAP_ECRED_CHAN_MAX	5

#define l2cap_lookup_ident(conn, ident) __l2cap_lookup_ident(conn, ident, false)
#define l2cap_remove_ident(conn, ident) __l2cap_lookup_ident(conn, ident, true)

struct l2cap_tx_meta_data {
	int sent;
	uint16_t cid;
	bt_conn_tx_cb_t cb;
	void *user_data;
};

struct l2cap_tx_meta {
	struct l2cap_tx_meta_data *data;
};

static struct l2cap_tx_meta_data l2cap_tx_meta_data_storage[CONFIG_BT_CONN_TX_MAX];
static osa_msgq_handle_t free_l2cap_tx_meta_data;
static OSA_MSGQ_HANDLE_DEFINE(free_l2cap_tx_meta_data_handle, CONFIG_BT_CONN_TX_MAX, sizeof(void*));

static struct l2cap_tx_meta_data *alloc_tx_meta_data(void)
{
        struct l2cap_tx_meta_data *get;
        if (OSA_MsgQGet(free_l2cap_tx_meta_data, &get, 0) == KOSA_StatusSuccess)
        {
            return get;
        }

        return NULL;
}

static void free_tx_meta_data(struct l2cap_tx_meta_data *data)
{
	(void)memset(data, 0, sizeof(*data));
        void *msg = (void*)data;
	OSA_MsgQPut(free_l2cap_tx_meta_data, &msg);
}

#define l2cap_tx_meta_data_macro(buf) (((struct l2cap_tx_meta *)net_buf_user_data(buf))->data)

struct data_sent {
	uint32_t reserved;
	uint16_t len;
};


#define l2cap_tx_meta_data(buf) (((struct l2cap_tx_meta *)net_buf_user_data(buf))->data)


#define data_sent(buf) ((struct data_sent *)net_buf_user_data(buf))

static sys_slist_t servers;

#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */

/* L2CAP signalling channel specific context */
struct bt_l2cap {
	/* The channel this context is associated with */
	struct bt_l2cap_le_chan	chan;
};

#if (defined(CONFIG_BT_L2CAP_ECRED) && ((CONFIG_BT_L2CAP_ECRED) > 0))
static const struct bt_l2cap_ecred_cb *ecred_cb;
#endif
static struct bt_l2cap bt_l2cap_pool[CONFIG_BT_MAX_CONN];

void bt_l2cap_register_ecred_cb(const struct bt_l2cap_ecred_cb *cb)
{
#if (defined(CONFIG_BT_L2CAP_ECRED) && ((CONFIG_BT_L2CAP_ECRED) > 0))
	ecred_cb = cb;
#endif
}

STRUCT_SECTION_DEFINE(bt_l2cap_fixed_chan);

#ifdef L2CAP_SUPPORT_CBFC_MODE
API_RESULT ethermind_l2ca_connect_ind_cb
           (
               DEVICE_HANDLE        * handle,
               UINT16                     lcid,
               UINT16                     psm,
               L2CAP_CBFC_CONNECT_PARAM * param
           );

API_RESULT ethermind_l2ca_connect_cnf_cb
           (
               DEVICE_HANDLE        * handle,
               UINT16                     lcid,
               UINT16                     response,
               L2CAP_CBFC_CONNECT_PARAM * param
           );
API_RESULT ethermind_l2ca_disconnect_ind_cb(UINT16 lcid);
API_RESULT ethermind_l2ca_disconnect_cnf_cb(UINT16 lcid, UINT16 reason);
API_RESULT ethermind_l2ca_data_read_cb (UINT16 lcid, UINT16 result, UCHAR * data, UINT16 datalen);
API_RESULT ethermind_l2ca_low_rx_credit_ind_cb (UINT16 lcid, UINT16 credit);
API_RESULT ethermind_l2ca_tx_credit_ind_cb (UINT16 lcid, UINT16 result, UINT16 credit);
API_RESULT ethermind_l2ca_data_write_cb(UINT16 lcid, UINT16 result, UCHAR *buffer, UINT16 buffer_len);
#endif /* L2CAP_SUPPORT_CBFC_MODE */

static uint8_t get_ident(void)
{
	static uint8_t ident;

	ident++;
	/* handle integer overflow (0 is not valid) */
	if (!ident) {
		ident++;
	}

	return ident;
}

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
static struct bt_l2cap_le_chan *l2cap_chan_alloc_cid(struct bt_conn *conn,
						     struct bt_l2cap_chan *chan)
{
	struct bt_l2cap_le_chan *le_chan = BT_L2CAP_LE_CHAN(chan);
	uint16_t cid;

	/*
	 * No action needed if there's already a CID allocated, e.g. in
	 * the case of a fixed channel.
	 */
	if (le_chan->rx.cid > 0) {
		return le_chan;
	}

	for (cid = L2CAP_LE_CID_DYN_START; cid <= L2CAP_LE_CID_DYN_END; cid++) {
		if (!bt_l2cap_le_lookup_rx_cid(conn, cid)) {
			le_chan->rx.cid = cid;
			return le_chan;
		}
	}

	return NULL;
}

static struct bt_l2cap_le_chan *
__l2cap_lookup_ident(struct bt_conn *conn, uint16_t ident, bool remove)
{
	struct bt_l2cap_chan *chan;
	sys_snode_t *prev = NULL;

	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		if (BT_L2CAP_LE_CHAN(chan)->ident == ident) {
			if (remove) {
				sys_slist_remove(&conn->channels, prev,
						 &chan->node);
			}
			return BT_L2CAP_LE_CHAN(chan);
		}

		prev = &chan->node;
	}

	return NULL;
}
#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */

void bt_l2cap_chan_remove(struct bt_conn *conn, struct bt_l2cap_chan *ch)
{
	struct bt_l2cap_chan *chan;
	sys_snode_t *prev = NULL;

	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		if (chan == ch) {
			sys_slist_remove(&conn->channels, prev, &chan->node);
			return;
		}

		prev = &chan->node;
	}
}

const char *bt_l2cap_chan_state_str(bt_l2cap_chan_state_t state)
{
	switch (state) {
	case BT_L2CAP_DISCONNECTED:
		return "disconnected";
	case BT_L2CAP_CONNECTING:
		return "connecting";
	case BT_L2CAP_CONFIG:
		return "config";
	case BT_L2CAP_CONNECTED:
		return "connected";
	case BT_L2CAP_DISCONNECTING:
		return "disconnecting";
	default:
		return "unknown";
	}
}

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
#define SET_CHAN_MEMBER(ch, member, value) \
	(ch->conn->type == BT_CONN_TYPE_LE) ? (BT_L2CAP_LE_CHAN(ch)->member = value) : (BR_CHAN(ch)->member = value);
#define GET_CHAN_MEMBER(ch, member) \
	(ch->conn->type == BT_CONN_TYPE_LE) ? (BT_L2CAP_LE_CHAN(ch)->member) : (BR_CHAN(ch)->member);
#if (defined(CONFIG_BT_DEBUG_L2CAP) && ((CONFIG_BT_DEBUG_L2CAP) > 0U))
void bt_l2cap_chan_set_state_debug(struct bt_l2cap_chan *chan,
				   bt_l2cap_chan_state_t state,
				   const char *func, int line)
{
	uint16_t temp_psm = GET_CHAN_MEMBER(chan, psm);
	bt_l2cap_chan_state_t temp_state = GET_CHAN_MEMBER(chan, state);

	BT_DBG("chan %p psm 0x%04x %s -> %s", chan, temp_psm,
	       bt_l2cap_chan_state_str(temp_state),
	       bt_l2cap_chan_state_str(state));

	/* check transitions validness */
	switch (state) {
	case BT_L2CAP_DISCONNECTED:
		/* regardless of old state always allows this state */
		break;
	case BT_L2CAP_CONNECTING:
		if (temp_state != BT_L2CAP_DISCONNECTED) {
			BT_WARN("%s()%d: invalid transition", func, line);
		}
		break;
	case BT_L2CAP_CONFIG:
		if (temp_state != BT_L2CAP_CONNECTING) {
			BT_WARN("%s()%d: invalid transition", func, line);
		}
		break;
	case BT_L2CAP_CONNECTED:
		if (temp_state != BT_L2CAP_CONFIG &&
		    temp_state != BT_L2CAP_CONNECTING) {
			BT_WARN("%s()%d: invalid transition", func, line);
		}
		break;
	case BT_L2CAP_DISCONNECTING:
		if (temp_state != BT_L2CAP_CONFIG &&
		    temp_state != BT_L2CAP_CONNECTED) {
			BT_WARN("%s()%d: invalid transition", func, line);
		}
		break;
	default:
		BT_ERR("%s()%d: unknown (%u) state was set", func, line, state);
		return;
	}

	SET_CHAN_MEMBER(chan, state, state);

	(void)temp_psm;   /* Unused variable */
}
#else
void bt_l2cap_chan_set_state(struct bt_l2cap_chan *chan,
			     bt_l2cap_chan_state_t state)
{
    SET_CHAN_MEMBER(chan, state, state);
}
#endif /* CONFIG_BT_DEBUG_L2CAP */
#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */

void bt_l2cap_chan_del(struct bt_l2cap_chan *chan)
{
	const struct bt_l2cap_chan_ops *ops = chan->ops;

	BT_DBG("conn %p chan %p", chan->conn, chan);

	if (!chan->conn) {
		goto destroy;
	}

	if (ops->disconnected) {
		ops->disconnected(chan);
	}

	chan->conn = NULL;

destroy:
#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
	/* Reset internal members of common channel */
	bt_l2cap_chan_set_state(chan, BT_L2CAP_DISCONNECTED);
	BT_L2CAP_LE_CHAN(chan)->psm = 0U;
#endif
	if (chan->destroy) {
		chan->destroy(chan);
	}

	if (ops->released) {
		ops->released(chan);
	}
}

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
static void l2cap_rtx_timeout(struct k_work *work)
{
	struct bt_l2cap_le_chan *chan = LE_CHAN_RTX(work);
	struct bt_conn *conn = chan->chan.conn;

	BT_ERR("chan %p timeout", chan);

	bt_l2cap_chan_remove(conn, &chan->chan);
	bt_l2cap_chan_del(&chan->chan);

	/* Remove other channels if pending on the same ident */
	while ((chan = l2cap_remove_ident(conn, chan->ident))) {
		bt_l2cap_chan_del(&chan->chan);
	}
}

static void l2cap_chan_le_recv(struct bt_l2cap_le_chan *chan,
			       struct net_buf *buf);

static void l2cap_rx_process(struct k_work *work)
{
	struct bt_l2cap_le_chan *ch = CHAN_RX(work);
	struct net_buf *buf;

	while ((buf = net_buf_get(ch->rx_queue.queue, osaWaitNone_c))) {
		BT_DBG("ch %p buf %p", ch, buf);
		l2cap_chan_le_recv(ch, buf);
		net_buf_unref(buf);
	}
}
#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */

void bt_l2cap_chan_add(struct bt_conn *conn, struct bt_l2cap_chan *chan,
		       bt_l2cap_chan_destroy_t destroy)
{
	/* Attach channel to the connection */
	sys_slist_append(&conn->channels, &chan->node);
	chan->conn = conn;
	chan->destroy = destroy;

	BT_DBG("conn %p chan %p", conn, chan);
}

static bool l2cap_chan_add(struct bt_conn *conn, struct bt_l2cap_chan *chan,
			   bt_l2cap_chan_destroy_t destroy)
{
	struct bt_l2cap_le_chan *le_chan;
    osa_status_t ret = KOSA_StatusSuccess;

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
	le_chan = l2cap_chan_alloc_cid(conn, chan);
#else
	le_chan = BT_L2CAP_LE_CHAN(chan);
#endif

	if (!le_chan) {
		BT_ERR("Unable to allocate L2CAP channel ID");
		return false;
	}

	atomic_clear(chan->status);

	bt_l2cap_chan_add(conn, chan, destroy);

#if 0
	/* All dynamic channels have the destroy handler which makes sure that
	 * the RTX work structure is properly released with a cancel sync.
	 * The fixed signal channel is only removed when disconnected and the
	 * disconnected handler is always called from the workqueue itself so
	 * canceling from there should always succeed.
	 */
	k_work_init_delayable(&le_chan->rtx_work, l2cap_rtx_timeout);
#endif

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
	if (L2CAP_LE_CID_IS_DYN(le_chan->rx.cid)) {
		k_work_init(&le_chan->rx_work, l2cap_rx_process);
		k_fifo_init(&le_chan->rx_queue);
		bt_l2cap_chan_set_state(chan, BT_L2CAP_CONNECTING);
	}
#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */
    (void)ret;

	return true;
}

void bt_l2cap_connected(struct bt_conn *conn)
{
	struct bt_l2cap_chan *chan;

	if (IS_ENABLED(CONFIG_BT_BREDR) &&
	    conn->type == BT_CONN_TYPE_BR) {
		bt_l2cap_br_connected(conn);
		return;
	}

	STRUCT_SECTION_FOREACH(bt_l2cap_fixed_chan, fchan) {
		struct bt_l2cap_le_chan *le_chan;

		if (fchan->accept(conn, &chan) < 0) {
			continue;
		}

		le_chan = BT_L2CAP_LE_CHAN(chan);

		/* Fill up remaining fixed channel context attached in
		 * fchan->accept()
		 */
		le_chan->rx.cid = fchan->cid;
		le_chan->tx.cid = fchan->cid;

		if (!l2cap_chan_add(conn, chan, fchan->destroy)) {
			return;
		}

		if (chan->ops->connected) {
			chan->ops->connected(chan);
		}

		/* Always set output status to fixed channels */
		atomic_set_bit(chan->status, BT_L2CAP_STATUS_OUT);

		if (chan->ops->status) {
			chan->ops->status(chan, chan->status);
		}
	}
}

void bt_l2cap_disconnected(struct bt_conn *conn)
{
	struct bt_l2cap_chan *chan, *next;

	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&conn->channels, chan, next, node, struct bt_l2cap_chan) {
		bt_l2cap_chan_del(chan);
	}
}

static struct net_buf *l2cap_create_le_sig_pdu(struct net_buf *buf,
					       uint8_t code, uint8_t ident,
					       uint16_t len)
{
	struct bt_l2cap_sig_hdr *hdr;
	struct net_buf_pool *pool = NULL;

	if (code == BT_L2CAP_DISCONN_REQ) {
		pool = &disc_pool;
	}

	/* Don't wait more than the minimum RTX timeout of 2 seconds */
	buf = bt_l2cap_create_pdu_timeout(pool, 0, L2CAP_RTX_TIMEOUT);
	if (!buf) {
		/* If it was not possible to allocate a buffer within the
		 * timeout return NULL.
		 */
		BT_ERR("Unable to allocate buffer for op 0x%02x", code);
		return NULL;
	}

	hdr = net_buf_add(buf, sizeof(*hdr));
	hdr->code = code;
	hdr->ident = ident;
	hdr->len = sys_cpu_to_le16(len);

	return buf;
}

/* Send the buffer and release it in case of failure.
 * Any other cleanup in failure to send should be handled by the disconnected
 * handler.
 */
static inline void l2cap_send(struct bt_conn *conn, uint16_t cid,
			      struct net_buf *buf)
{
	if (bt_l2cap_send(conn, cid, buf)) {
		net_buf_unref(buf);
	}
}

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
static int l2cap_chan_send_req(struct bt_l2cap_chan *chan,
				struct net_buf *buf, size_t timeout)
{
    struct bt_l2cap_le_chan *leChan;
	/* BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part A] page 126:
	 *
	 * The value of this timer is implementation-dependent but the minimum
	 * initial value is 1 second and the maximum initial value is 60
	 * seconds. One RTX timer shall exist for each outstanding signaling
	 * request, including each Echo Request. The timer disappears on the
	 * final expiration, when the response is received, or the physical
	 * link is lost.
	 */
#if 0
	k_work_schedule(&(BT_L2CAP_LE_CHAN(chan)->rtx_work), timeout);
#endif
    L2CAP_PSM_CBFC l2cap_psm =
    {
        ethermind_l2ca_connect_ind_cb,
        ethermind_l2ca_connect_cnf_cb,
        ethermind_l2ca_disconnect_ind_cb,
        ethermind_l2ca_disconnect_cnf_cb,
        ethermind_l2ca_data_read_cb,
        ethermind_l2ca_low_rx_credit_ind_cb,
        ethermind_l2ca_tx_credit_ind_cb,
        L2CAP_LE_PSM_FIXED_START,
#ifdef L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
        L2CAP_LE_MAX_CREDITS,
#endif /* L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM */
        ethermind_l2ca_data_write_cb
    };
    struct bt_l2cap_le_conn_req *req;
    struct bt_l2cap_sig_hdr *hdr;
    L2CAP_CBFC_CONNECT_PARAM connect_param;
    API_RESULT retval;
    int err;

    hdr = (struct bt_l2cap_sig_hdr *)buf->data;
    req = (struct bt_l2cap_le_conn_req *)&buf->data[sizeof(*hdr)];
    (void)hdr;

    leChan = BT_L2CAP_LE_CHAN(chan);

    l2cap_psm.psm = req->psm;
#ifdef L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
    l2cap_psm.credit_lwm = req->credits;
#endif /* L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM */
    retval = l2cap_cbfc_register_psm (&l2cap_psm);

    connect_param.mtu = req->mtu;
    connect_param.mps = req->mps;
    connect_param.credit = req->credits;

    retval = l2ca_cbfc_connect_req
                 (
                     &chan->conn->deviceId,
                     req->psm,
                     l2cap_psm.psm,
                     &connect_param
                 );
    net_buf_unref(buf);
    if (API_SUCCESS == retval)
    {
        leChan->rx.cid = sys_le16_to_cpu(connect_param.cid);
        err = 0;
    }
    else
    {
        err = -EIO;
    }
    return err;
}

static int l2cap_le_conn_req(struct bt_l2cap_le_chan *ch)
{
	struct net_buf *buf;
	struct bt_l2cap_le_conn_req *req;

	ch->ident = get_ident();

	buf = l2cap_create_le_sig_pdu(NULL, BT_L2CAP_LE_CONN_REQ,
				      ch->ident, sizeof(*req));
	if (!buf) {
		return -ENOMEM;
	}

	req = net_buf_add(buf, sizeof(*req));
	req->psm = sys_cpu_to_le16(ch->psm);
	req->scid = sys_cpu_to_le16(ch->rx.cid);
	req->mtu = sys_cpu_to_le16(ch->rx.mtu);
	req->mps = sys_cpu_to_le16(ch->rx.mps);
	req->credits = sys_cpu_to_le16(ch->rx.init_credits);

	l2cap_chan_send_req(&ch->chan, buf, L2CAP_CONN_TIMEOUT);

	return 0;
}

#if (defined(CONFIG_BT_L2CAP_ECRED) && ((CONFIG_BT_L2CAP_ECRED) > 0))

static API_RESULT ethermind_l2ca_ecbfc_connect_ind_cb
           (
               DEVICE_HANDLE             * handle,
               UINT16                    * lcid,
               UINT16                      psm,
               L2CAP_ECBFC_CONNECT_PARAM * param
           );
static API_RESULT ethermind_l2ca_ecbfc_connect_cnf_cb
           (
               DEVICE_HANDLE             * handle,
               UINT16                    * lcid,
               UINT16                      response,
               L2CAP_ECBFC_CONNECT_PARAM * param
           );
static API_RESULT ethermind_l2ca_ecbfc_reconfig_ind_cb
           (
               DEVICE_HANDLE            * handle,
               UINT16                   * lcid,
               L2CAP_ECBFC_RECONFIG_PARAM * param
           );
static API_RESULT ethermind_l2ca_ecbfc_reconfig_cnf_cb
           (
               DEVICE_HANDLE            * handle
           );

static API_RESULT ethermind_l2ca_ecbfc_disconnect_ind_cb(UINT16 lcid);

static API_RESULT ethermind_l2ca_ecbfc_disconnect_cnf_cb(UINT16 lcid, UINT16 reason);

static API_RESULT ethermind_l2ca_ecbfc_data_read_cb (UINT16 lcid, UINT16 result, UCHAR * data, UINT16 datalen);

static API_RESULT ethermind_l2ca_ecbfc_low_rx_credit_ind_cb (UINT16 lcid, UINT16 credit);

static API_RESULT ethermind_l2ca_ecbfc_tx_credit_ind_cb (UINT16 lcid, UINT16 result, UINT16 credit);

static API_RESULT ethermind_l2ca_ecbfc_data_write_cb(UINT16 lcid, UINT16 result, UCHAR *buffer, UINT16 buffer_len);

static int l2cap_ecred_conn_req(struct bt_l2cap_chan **chan, int channels)
{
	//struct net_buf *buf;
	//struct bt_l2cap_ecred_conn_req *req;
	struct bt_l2cap_le_chan *ch;
	int i;
	uint8_t ident;
	uint16_t req_psm;
	API_RESULT retval;
	int err;

	if (!chan || !channels) {
		return -EINVAL;
	}
#if 0
	ident = get_ident();

	buf = l2cap_create_le_sig_pdu(NULL, BT_L2CAP_ECRED_CONN_REQ, ident,
				      sizeof(*req) +
				      (channels * sizeof(uint16_t)));

	req = net_buf_add(buf, sizeof(*req));

	ch = BT_L2CAP_LE_CHAN(chan[0]);

	/* Init common parameters */
	req->psm = sys_cpu_to_le16(ch->psm);
	req->mtu = sys_cpu_to_le16(ch->rx.mtu);
	req->mps = sys_cpu_to_le16(ch->rx.mps);
	req->credits = sys_cpu_to_le16(ch->rx.init_credits);
	req_psm = ch->psm;

	for (i = 0; i < channels; i++) {
		ch = BT_L2CAP_LE_CHAN(chan[i]);

		__ASSERT(ch->psm == req_psm,
			 "The PSM shall be the same for channels in the same request.");

		ch->ident = ident;

		net_buf_add_le16(buf, ch->rx.cid);
	}

	l2cap_chan_send_req(*chan, buf, L2CAP_CONN_TIMEOUT);
#endif
        ch = BT_L2CAP_LE_CHAN(chan[0]);

        L2CAP_PSM_ECBFC l2cap_ecbfc_psm =
        {
            ethermind_l2ca_ecbfc_connect_ind_cb,
            ethermind_l2ca_ecbfc_connect_cnf_cb,
            ethermind_l2ca_ecbfc_reconfig_ind_cb,
            ethermind_l2ca_ecbfc_reconfig_cnf_cb,
            ethermind_l2ca_ecbfc_disconnect_ind_cb,
            ethermind_l2ca_ecbfc_disconnect_cnf_cb,
            ethermind_l2ca_ecbfc_data_read_cb,
            ethermind_l2ca_ecbfc_low_rx_credit_ind_cb,
            ethermind_l2ca_ecbfc_tx_credit_ind_cb,
            L2CAP_LE_PSM_FIXED_START,

        #ifdef L2CAP_ECBFC_SUPPORT_UL_CREDIT_LWM
            L2CAP_LE_MAX_CREDITS,
        #endif /* L2CAP_ECBFC_SUPPORT_UL_CREDIT_LWM */

            ethermind_l2ca_ecbfc_data_write_cb
        };

        L2CAP_ECBFC_CONNECT_PARAM ecbfc_connect_param;

        l2cap_ecbfc_psm.psm = ch->psm;
#ifdef L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
        l2cap_ecbfc_psm.credit_lwm = ch->rx.init_credits;
#endif /* L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM */

        retval = l2cap_ecbfc_register_psm (&l2cap_ecbfc_psm);
        if ((retval != API_SUCCESS) && (retval != L2CAP_PSM_ALREADY_REGISTERED))
        {
            return -EIO;
        }

        ecbfc_connect_param.mtu = ch->rx.mtu;
        ecbfc_connect_param.mps = ch->rx.mps;
        ecbfc_connect_param.credit = ch->rx.init_credits;
        ecbfc_connect_param.num_cids = channels;
#if 0 /* allocate by EtherMind */
	for (i = 0; i < channels; i++)
        {
            ch = BT_L2CAP_LE_CHAN(chan[i]);

            ch->chan.ident = ident;

            ecbfc_connect_param.cid[i] = ch->rx.cid;
	}
#endif
        retval = l2ca_ecbfc_connect_req
                 (
                     &chan[0]->conn->deviceId,
                     ch->psm,
                     l2cap_ecbfc_psm.psm,
                     &ecbfc_connect_param
                 );
        if (API_SUCCESS == retval)
        {
            err = 0;
            /* allocate by EtherMind */
            for (i = 0; i < channels; i++)
            {
                ch = BT_L2CAP_LE_CHAN(chan[i]);
                ch->rx.cid = ecbfc_connect_param.cid[i];
            }
        }
        else
        {
            err = -EIO;
        }

	return err;
}
#endif /* defined(CONFIG_BT_L2CAP_ECRED) */

static void l2cap_le_encrypt_change(struct bt_l2cap_chan *chan, uint8_t status)
{
	int err;
	struct bt_l2cap_le_chan *le = BT_L2CAP_LE_CHAN(chan);

	/* Skip channels that are not pending waiting for encryption */
	if (!atomic_test_and_clear_bit(chan->status,
				       BT_L2CAP_STATUS_ENCRYPT_PENDING)) {
		return;
	}

	if (status) {
		goto fail;
	}

#if (defined(CONFIG_BT_L2CAP_ECRED) && ((CONFIG_BT_L2CAP_ECRED) > 0))
	if (le->ident) {
		struct bt_l2cap_chan *echan[L2CAP_ECRED_CHAN_MAX_PER_REQ];
		struct bt_l2cap_le_chan *ch;
		int i = 0;

		while ((ch = l2cap_remove_ident(chan->conn, BT_L2CAP_LE_CHAN(chan)->ident))) {
			echan[i++] = &ch->chan;
		}

		/* Retry ecred connect */
		l2cap_ecred_conn_req(echan, i);
		return;
	}
#endif /* defined(CONFIG_BT_L2CAP_ECRED) */

	/* Retry to connect */
	err = l2cap_le_conn_req(le);
	if (err) {
		goto fail;
	}

	return;
fail:
	bt_l2cap_chan_remove(chan->conn, chan);
	bt_l2cap_chan_del(chan);
}
#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */

void bt_l2cap_security_changed(struct bt_conn *conn, uint8_t hci_status)
{
	struct bt_l2cap_chan *chan, *next;

	if (IS_ENABLED(CONFIG_BT_BREDR) &&
	    conn->type == BT_CONN_TYPE_BR) {
		l2cap_br_encrypt_change(conn, hci_status);
		return;
	}

	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&conn->channels, chan, next, node, struct bt_l2cap_chan) {
#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
		l2cap_le_encrypt_change(chan, hci_status);
#endif

		if (chan->ops->encrypt_change) {
			chan->ops->encrypt_change(chan, hci_status);
		}
	}
}

struct net_buf *bt_l2cap_create_pdu_timeout(struct net_buf_pool *pool,
					    size_t reserve,
					    size_t timeout)
{
	return bt_conn_create_pdu_timeout(pool,
					  sizeof(struct bt_l2cap_hdr) + reserve,
					  timeout);
}

int bt_l2cap_send_cb(struct bt_conn *conn, uint16_t cid, struct net_buf *buf,
		     bt_conn_tx_cb_t cb, void *user_data)
{
#if 0
	struct bt_l2cap_hdr *hdr;

	BT_DBG("conn %p cid %u len %lu", conn, cid, net_buf_frags_len(buf));

	hdr = net_buf_push(buf, sizeof(*hdr));
	hdr->len = sys_cpu_to_le16(buf->len - sizeof(*hdr));
	hdr->cid = sys_cpu_to_le16(cid);

	return bt_conn_send_cb(conn, buf, cb, user_data);
#else
    return -EIO;
#endif
}

static void l2cap_send_reject(struct bt_conn *conn, uint8_t ident,
			      uint16_t reason, void *data, uint8_t data_len)
{
	struct bt_l2cap_cmd_reject *rej;
	struct net_buf *buf;

	buf = l2cap_create_le_sig_pdu(NULL, BT_L2CAP_CMD_REJECT, ident,
				      sizeof(*rej) + data_len);
	if (!buf) {
		return;
	}

	rej = net_buf_add(buf, sizeof(*rej));
	rej->reason = sys_cpu_to_le16(reason);

	if (data) {
		net_buf_add_mem(buf, data, data_len);
	}

	l2cap_send(conn, BT_L2CAP_CID_LE_SIG, buf);
}

static void le_conn_param_rsp(struct bt_l2cap *l2cap, struct net_buf *buf)
{
	struct bt_l2cap_conn_param_rsp *rsp = (void *)buf->data;

	if (buf->len < sizeof(*rsp)) {
		BT_ERR("Too small LE conn param rsp");
		return;
	}

	BT_DBG("LE conn param rsp result %u", sys_le16_to_cpu(rsp->result));
        
	(void)rsp;
}

static void le_conn_param_update_req(struct bt_l2cap *l2cap, uint8_t ident,
				     struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_le_conn_param param;
	struct bt_l2cap_conn_param_rsp *rsp;
	struct bt_l2cap_conn_param_req *req = (void *)buf->data;
	bool accepted;

	if (buf->len < sizeof(*req)) {
		BT_ERR("Too small LE conn update param req");
		return;
	}

	if (conn->state != BT_CONN_CONNECTED) {
		BT_WARN("Not connected");
		return;
	}

	if (conn->role != BT_HCI_ROLE_CENTRAL) {
		l2cap_send_reject(conn, ident, BT_L2CAP_REJ_NOT_UNDERSTOOD,
				  NULL, 0);
		return;
	}

	param.interval_min = sys_le16_to_cpu(req->min_interval);
	param.interval_max = sys_le16_to_cpu(req->max_interval);
	param.latency = sys_le16_to_cpu(req->latency);
	param.timeout = sys_le16_to_cpu(req->timeout);

	BT_DBG("min 0x%04x max 0x%04x latency: 0x%04x timeout: 0x%04x",
	       param.interval_min, param.interval_max, param.latency,
	       param.timeout);

	buf = l2cap_create_le_sig_pdu(buf, BT_L2CAP_CONN_PARAM_RSP, ident,
				      sizeof(*rsp));
	if (!buf) {
		return;
	}

	accepted = le_param_req(conn, &param);

	rsp = net_buf_add(buf, sizeof(*rsp));
	if (accepted) {
		rsp->result = sys_cpu_to_le16(BT_L2CAP_CONN_PARAM_ACCEPTED);
	} else {
		rsp->result = sys_cpu_to_le16(BT_L2CAP_CONN_PARAM_REJECTED);
	}

	l2cap_send(conn, BT_L2CAP_CID_LE_SIG, buf);

	if (accepted) {
		bt_conn_le_conn_update(conn, &param);
	}
}

struct bt_l2cap_chan *bt_l2cap_le_lookup_tx_cid(struct bt_conn *conn,
						uint16_t cid)
{
	struct bt_l2cap_chan *chan;

	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		if (BT_L2CAP_LE_CHAN(chan)->tx.cid == cid) {
			return chan;
		}
	}

	return NULL;
}

struct bt_l2cap_chan *bt_l2cap_le_lookup_rx_cid(struct bt_conn *conn,
						uint16_t cid)
{
	struct bt_l2cap_chan *chan;

	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		if (BT_L2CAP_LE_CHAN(chan)->rx.cid == cid) {
			return chan;
		}
	}

	return NULL;
}

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
static struct bt_l2cap_server *l2cap_server_lookup_psm(uint16_t psm)
{
	struct bt_l2cap_server *server;

	SYS_SLIST_FOR_EACH_CONTAINER(&servers, server, node, struct bt_l2cap_server) {
		if (server->psm == psm) {
			return server;
		}
	}

	return NULL;
}

int bt_l2cap_server_register(struct bt_l2cap_server *server)
{
    API_RESULT retval;
    int err = 0;

    if (server->psm != 0x27u)
#if 0
    {
        sys_slist_append(&servers, &server->node);
        return 0;
    }
#endif
    {
        L2CAP_PSM_CBFC l2cap_psm =
        {
            ethermind_l2ca_connect_ind_cb,
            ethermind_l2ca_connect_cnf_cb,
            ethermind_l2ca_disconnect_ind_cb,
            ethermind_l2ca_disconnect_cnf_cb,
            ethermind_l2ca_data_read_cb,
            ethermind_l2ca_low_rx_credit_ind_cb,
            ethermind_l2ca_tx_credit_ind_cb,
            L2CAP_LE_PSM_FIXED_START,
#ifdef L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
            L2CAP_LE_MAX_CREDITS,
#endif /* L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM */
            ethermind_l2ca_data_write_cb
        };

            if (!server->accept) {
                    return -EINVAL;
            }

            if (server->psm) {
                    if (server->psm < L2CAP_LE_PSM_FIXED_START ||
                        server->psm > L2CAP_LE_PSM_DYN_END) {
                            return -EINVAL;
                    }

                    /* Check if given PSM is already in use */
                    if (l2cap_server_lookup_psm(server->psm)) {
                            BT_DBG("PSM already registered");
                            return -EADDRINUSE;
                    }
            } else {
                    uint16_t psm;

                    for (psm = (L2CAP_LE_PSM_DYN_START + 1);
                         psm <= L2CAP_LE_PSM_DYN_END; psm += 2) {
                            if (!l2cap_server_lookup_psm(psm)) {
                                    break;
                            }
                    }

                    if (psm > L2CAP_LE_PSM_DYN_END) {
                            BT_WARN("No free dynamic PSMs available");
                            return -EADDRNOTAVAIL;
                    }

                    BT_DBG("Allocated PSM 0x%04x for new server", psm);
                    server->psm = psm;
            }

            if (server->sec_level > BT_SECURITY_L4) {
                    return -EINVAL;
            } else if (server->sec_level < BT_SECURITY_L1) {
                    /* Level 0 is only applicable for BR/EDR */
                    server->sec_level = BT_SECURITY_L1;
            }

            BT_DBG("PSM 0x%04x", server->psm);

        l2cap_psm.psm = server->psm;

        retval = l2cap_cbfc_register_psm(&l2cap_psm);
        if (API_SUCCESS != retval)
        {
            return -EIO;
        }
    }

#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED > 0U))
    L2CAP_PSM_ECBFC l2cap_ecbfc_psm =
    {
        ethermind_l2ca_ecbfc_connect_ind_cb,
        ethermind_l2ca_ecbfc_connect_cnf_cb,
        ethermind_l2ca_ecbfc_reconfig_ind_cb,
        ethermind_l2ca_ecbfc_reconfig_cnf_cb,
        ethermind_l2ca_ecbfc_disconnect_ind_cb,
        ethermind_l2ca_ecbfc_disconnect_cnf_cb,
        ethermind_l2ca_ecbfc_data_read_cb,
        ethermind_l2ca_ecbfc_low_rx_credit_ind_cb,
        ethermind_l2ca_ecbfc_tx_credit_ind_cb,
        L2CAP_LE_PSM_FIXED_START,

    #ifdef L2CAP_ECBFC_SUPPORT_UL_CREDIT_LWM
        L2CAP_LE_MAX_CREDITS,
    #endif /* L2CAP_ECBFC_SUPPORT_UL_CREDIT_LWM */

        ethermind_l2ca_ecbfc_data_write_cb
    };

    l2cap_ecbfc_psm.psm = server->psm;

    if (server->psm == 0x27u)
    {
        (void)l2cap_ecbfc_unregister_psm(server->psm);
    }
    retval = l2cap_ecbfc_register_psm (&l2cap_ecbfc_psm);
    if(retval != API_SUCCESS)
    {
        return -EIO;
    }
#endif

    sys_slist_append(&servers, &server->node);
    return err;
}

static void l2cap_chan_rx_init(struct bt_l2cap_le_chan *chan)
{
	BT_DBG("chan %p", chan);

	/* Use existing MTU if defined */
	if (!chan->rx.mtu) {
		/* If application has not provide the incoming L2CAP SDU MTU use
		 * an MTU that does not require segmentation.
		 */
		chan->rx.mtu = BT_L2CAP_SDU_RX_MTU;
	}

	/* Init general queues when attaching the first channel */
    k_fifo_init(&chan->rx_queue);

	/* MPS shall not be bigger than MTU + BT_L2CAP_SDU_HDR_SIZE as the
	 * remaining bytes cannot be used.
	 */
	chan->rx.mps = MIN(chan->rx.mtu + BT_L2CAP_SDU_HDR_SIZE,
			   BT_L2CAP_RX_MTU);

	/* Truncate MTU if channel have disabled segmentation but still have
	 * set an MTU which requires it.
	 */
	if (!chan->chan.ops->alloc_buf &&
	    (chan->rx.mps < chan->rx.mtu + BT_L2CAP_SDU_HDR_SIZE)) {
		BT_WARN("Segmentation disabled but MTU > MPS, truncating MTU");
		chan->rx.mtu = chan->rx.mps - BT_L2CAP_SDU_HDR_SIZE;
	}

	/* Use existing credits if defined */
	if (!chan->rx.init_credits) {
		if (chan->chan.ops->alloc_buf) {
			/* Auto tune credits to receive a full packet */
			chan->rx.init_credits =
				ceiling_fraction(chan->rx.mtu,
						 BT_L2CAP_RX_MTU);
		} else {
			chan->rx.init_credits = L2CAP_LE_MAX_CREDITS;
		}
	}

	atomic_set(&chan->rx.credits,  0);

	if (LOG_ENABLE &&
	    chan->rx.init_credits * chan->rx.mps <
	    chan->rx.mtu + BT_L2CAP_SDU_HDR_SIZE) {
		BT_WARN("Not enough credits for a full packet");
	}
}

static struct net_buf *l2cap_chan_le_get_tx_buf(struct bt_l2cap_le_chan *ch)
{
	struct net_buf *buf;

	/* Return current buffer */
	if (ch->tx_buf) {
		buf = ch->tx_buf;
		ch->tx_buf = NULL;
		return buf;
	}

	return net_buf_get(ch->tx_queue.queue, osaWaitNone_c);
}

static int l2cap_chan_le_send_sdu(struct bt_l2cap_le_chan *ch,
				  struct net_buf **buf, uint16_t sent);
static void l2cap_chan_sdu_sent(struct bt_conn *conn, void *user_data, int err);
#if 1
static void l2cap_chan_tx_process(struct k_work *work)
{
	struct bt_l2cap_le_chan *ch;
	struct net_buf *buf;

	ch = CONTAINER_OF(work, struct bt_l2cap_le_chan, tx_work);

	/* Resume tx in case there are buffers in the queue */
	while ((buf = l2cap_chan_le_get_tx_buf(ch))) {
		int sent = l2cap_tx_meta_data(buf)->sent;

		BT_DBG("buf %p sent %u", buf, sent);

		sent = l2cap_chan_le_send_sdu(ch, &buf, sent);
		if (sent < 0) {
			if (sent == -EAGAIN) {
				ch->tx_buf = buf;
				/* If we don't reschedule, and the app doesn't nudge l2cap (e.g. by
				 * sending another SDU), the channel will be stuck in limbo. To
				 * prevent this, we attempt to re-schedule the work item for every
				 * channel on every connection when an SDU has successfully been
				 * sent.
				 */
			} else {
				net_buf_unref(buf);
			}
			break;
		}
	}
}
#endif

static void l2cap_chan_tx_init(struct bt_l2cap_le_chan *chan)
{
	BT_DBG("chan %p", chan);

	(void)memset(&chan->tx, 0, sizeof(chan->tx));
	atomic_set(&chan->tx.credits, 0);
	/* Init general queues when attaching the first channel */
    k_fifo_init(&chan->tx_queue);
#if 1
	k_work_init(&chan->tx_work, l2cap_chan_tx_process);
#endif
}

static void l2cap_chan_tx_give_credits(struct bt_l2cap_le_chan *chan,
				       uint16_t credits)
{
	BT_DBG("chan %p credits %u", chan, credits);

	atomic_add(&chan->tx.credits, credits);

	if (!atomic_test_and_set_bit(chan->chan.status, BT_L2CAP_STATUS_OUT) &&
	    chan->chan.ops->status) {
		chan->chan.ops->status(&chan->chan, chan->chan.status);
	}
}

static void l2cap_chan_rx_give_credits(struct bt_l2cap_le_chan *chan,
				       uint16_t credits)
{
	BT_DBG("chan %p credits %u", chan, credits);

	atomic_add(&chan->rx.credits, credits);
}

static void l2cap_chan_destroy(struct bt_l2cap_chan *chan)
{
	struct bt_l2cap_le_chan *le_chan = BT_L2CAP_LE_CHAN(chan);
	struct net_buf *buf;

	BT_DBG("chan %p cid 0x%04x", le_chan, le_chan->rx.cid);

#if 0
	/* Cancel ongoing work. Since the channel can be re-used after this
	 * we need to sync to make sure that the kernel does not have it
	 * in its queue anymore.
	 *
	 * In the case where we are in the context of executing the rtx_work
	 * item, we don't sync as it will deadlock the workqueue.
	 */
	if (k_current_get() != &le_chan->rtx_work.queue->thread) {
		k_work_cancel_delayable_sync(&le_chan->rtx_work, &le_chan->rtx_sync);
	} else {
		k_work_cancel_delayable(&le_chan->rtx_work);
	}
#endif

	if (le_chan->tx_buf) {
		net_buf_unref(le_chan->tx_buf);
		le_chan->tx_buf = NULL;
	}

	/* Remove buffers on the TX queue */
	while ((buf = net_buf_get(le_chan->tx_queue.queue, osaWaitNone_c))) {
		net_buf_unref(buf);
	}

	/* Remove buffers on the RX queue */
	while ((buf = net_buf_get(le_chan->rx_queue.queue, osaWaitNone_c))) {
		net_buf_unref(buf);
	}

	/* Destroy segmented SDU if it exists */
	if (le_chan->_sdu) {
		net_buf_unref(le_chan->_sdu);
		le_chan->_sdu = NULL;
		le_chan->_sdu_len = 0U;
	}
}

static uint16_t le_err_to_result(int err)
{
	switch (err) {
	case -ENOMEM:
		return BT_L2CAP_LE_ERR_NO_RESOURCES;
	case -EACCES:
		return BT_L2CAP_LE_ERR_AUTHORIZATION;
	case -EPERM:
		return BT_L2CAP_LE_ERR_KEY_SIZE;
	case -ENOTSUP:
		/* This handle the cases where a fixed channel is registered but
		 * for some reason (e.g. controller not suporting a feature)
		 * cannot be used.
		 */
		return BT_L2CAP_LE_ERR_PSM_NOT_SUPP;
	default:
		return BT_L2CAP_LE_ERR_UNACCEPT_PARAMS;
	}
}

static uint16_t l2cap_chan_accept(struct bt_conn *conn,
			       struct bt_l2cap_server *server, uint16_t scid,
			       uint16_t mtu, uint16_t mps, uint16_t credits,
			       struct bt_l2cap_chan **chan)
{
	struct bt_l2cap_le_chan *le_chan;
	int err;

	BT_DBG("conn %p scid 0x%04x chan %p", conn, scid, chan);

	if (!L2CAP_LE_CID_IS_DYN(scid)) {
		return BT_L2CAP_LE_ERR_INVALID_SCID;
	}

	*chan = bt_l2cap_le_lookup_tx_cid(conn, scid);
	if (*chan) {
		return BT_L2CAP_LE_ERR_SCID_IN_USE;
	}

	/* Request server to accept the new connection and allocate the
	 * channel.
	 */
	err = server->accept(conn, chan);
	if (err < 0) {
		return le_err_to_result(err);
	}

	if (!(*chan)->ops->recv) {
		BT_ERR("Mandatory callback 'recv' missing");
		return BT_L2CAP_LE_ERR_UNACCEPT_PARAMS;
	}

	le_chan = BT_L2CAP_LE_CHAN(*chan);

	le_chan->required_sec_level = server->sec_level;

	if (!l2cap_chan_add(conn, *chan, l2cap_chan_destroy)) {
		return BT_L2CAP_LE_ERR_NO_RESOURCES;
	}

	/* Init TX parameters */
	l2cap_chan_tx_init(le_chan);
	le_chan->tx.cid = scid;
	le_chan->tx.mps = mps;
	le_chan->tx.mtu = mtu;
	le_chan->tx.init_credits = credits;
	l2cap_chan_tx_give_credits(le_chan, credits);

	/* Init RX parameters */
	l2cap_chan_rx_init(le_chan);
	l2cap_chan_rx_give_credits(le_chan, le_chan->rx.init_credits);

	/* Set channel PSM */
	le_chan->psm = server->psm;

	/* Update state */
	bt_l2cap_chan_set_state(*chan, BT_L2CAP_CONNECTED);

	if ((*chan)->ops->connected) {
		(*chan)->ops->connected(*chan);
	}

	return BT_L2CAP_LE_SUCCESS;
}

static uint16_t l2cap_check_security(struct bt_conn *conn,
				 struct bt_l2cap_server *server)
{
	const struct bt_keys *keys = bt_keys_find_addr(conn->id, &conn->le.dst);
	bool ltk_present;

	/* Try L2CAP_SC_CHECK_TIMEOUT to take semaphore to wait until the security level updated. */
	osa_status_t status = OSA_SemaphoreWait(conn->sem_security_level_updated, L2CAP_SC_CHECK_TIMEOUT);
	if(KOSA_StatusSuccess == status)
	{
		(void)OSA_SemaphorePost(conn->sem_security_level_updated);
	}
	else
	{
		BT_ERR("conn: %p, security level semaphore wait fail %d", conn, status);
		return BT_L2CAP_LE_ERR_AUTHENTICATION;
	}

#if (defined(CONFIG_BT_CONN_DISABLE_SECURITY) && ((CONFIG_BT_CONN_DISABLE_SECURITY) > 0U))
	if (IS_ENABLED(CONFIG_BT_CONN_DISABLE_SECURITY)) {
		return BT_L2CAP_LE_SUCCESS;
	}
#else
	if (conn->sec_level >= server->sec_level) {
		return BT_L2CAP_LE_SUCCESS;
	}

	if (conn->sec_level > BT_SECURITY_L1) {
		return BT_L2CAP_LE_ERR_AUTHENTICATION;
	}

	if (keys) {
		if (conn->role == BT_HCI_ROLE_CENTRAL) {
			ltk_present = keys->keys & (BT_KEYS_LTK_P256 | BT_KEYS_PERIPH_LTK);
		} else {
			ltk_present = keys->keys & (BT_KEYS_LTK_P256 | BT_KEYS_LTK);
		}
	} else {
		ltk_present = false;
	}

	/* If an LTK or an STK is available and encryption is required
	 * (LE security mode 1) but encryption is not enabled, the
	 * service request shall be rejected with the error code
	 * "Insufficient Encryption".
	 */
	if (ltk_present) {
		return BT_L2CAP_LE_ERR_ENCRYPTION;
	}

	return BT_L2CAP_LE_ERR_AUTHENTICATION;
#endif /* CONFIG_BT_CONN_DISABLE_SECURITY */
}

static void le_conn_req(struct bt_l2cap *l2cap, uint8_t ident,
			struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_l2cap_chan *chan;
	struct bt_l2cap_le_chan *le_chan;
	struct bt_l2cap_server *server;
	struct bt_l2cap_le_conn_req *req = (void *)buf->data;
	struct bt_l2cap_le_conn_rsp *rsp;
	uint16_t psm, scid, mtu, mps, credits;
	uint16_t result;

	if (buf->len < sizeof(*req)) {
		BT_ERR("Too small LE conn req packet size");
		return;
	}

	psm = sys_le16_to_cpu(req->psm);
	scid = sys_le16_to_cpu(req->scid);
	mtu = sys_le16_to_cpu(req->mtu);
	mps = sys_le16_to_cpu(req->mps);
	credits = sys_le16_to_cpu(req->credits);

	BT_DBG("psm 0x%02x scid 0x%04x mtu %u mps %u credits %u", psm, scid,
	       mtu, mps, credits);

	if (mtu < L2CAP_LE_MIN_MTU || mps < L2CAP_LE_MIN_MTU) {
		BT_ERR("Invalid LE-Conn Req params");
		return;
	}

	buf = l2cap_create_le_sig_pdu(buf, BT_L2CAP_LE_CONN_RSP, ident,
				      sizeof(*rsp));
	if (!buf) {
		return;
	}

	rsp = net_buf_add(buf, sizeof(*rsp));
	(void)memset(rsp, 0, sizeof(*rsp));

	/* Check if there is a server registered */
	server = l2cap_server_lookup_psm(psm);
	if (!server) {
		rsp->result = sys_cpu_to_le16(BT_L2CAP_LE_ERR_PSM_NOT_SUPP);
		goto rsp;
	}

	/* Check if connection has minimum required security level */
	result = l2cap_check_security(conn, server);
	if (result != BT_L2CAP_LE_SUCCESS) {
		rsp->result = sys_cpu_to_le16(result);
		goto rsp;
	}

	result = l2cap_chan_accept(conn, server, scid, mtu, mps, credits,
				   &chan);
	if (result != BT_L2CAP_LE_SUCCESS) {
		rsp->result = sys_cpu_to_le16(result);
		goto rsp;
	}

	le_chan = BT_L2CAP_LE_CHAN(chan);

	/* Prepare response protocol data */
	rsp->dcid = sys_cpu_to_le16(le_chan->rx.cid);
	rsp->mps = sys_cpu_to_le16(le_chan->rx.mps);
	rsp->mtu = sys_cpu_to_le16(le_chan->rx.mtu);
	rsp->credits = sys_cpu_to_le16(le_chan->rx.init_credits);
	rsp->result = BT_L2CAP_LE_SUCCESS;

rsp:
	l2cap_send(conn, BT_L2CAP_CID_LE_SIG, buf);
}

#if (defined(CONFIG_BT_L2CAP_ECRED) && ((CONFIG_BT_L2CAP_ECRED) > 0))
static void le_ecred_conn_req(struct bt_l2cap *l2cap, uint8_t ident,
			      struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_l2cap_chan *chan[L2CAP_ECRED_CHAN_MAX_PER_REQ];
	struct bt_l2cap_le_chan *ch = NULL;
	struct bt_l2cap_server *server;
	struct bt_l2cap_ecred_conn_req *req;
	struct bt_l2cap_ecred_conn_rsp *rsp;
	uint16_t mtu, mps, credits, result = BT_L2CAP_LE_SUCCESS;
	uint16_t psm = 0x0000;
	uint16_t scid, dcid[L2CAP_ECRED_CHAN_MAX_PER_REQ];
	uint16_t lcid[L2CAP_ECRED_CHAN_MAX_PER_REQ];
	int i = 0;
	uint8_t req_cid_count;
	L2CAP_ECBFC_CONNECT_PARAM ethermind_ecbfc_connect_param;

	/* set dcid to zeros here, in case of all connections refused error */
	memset(dcid, 0, sizeof(dcid));
	if (buf->len < sizeof(*req)) {
		BT_ERR("Too small LE conn req packet size");
		result = BT_L2CAP_LE_ERR_INVALID_PARAMS;
		req_cid_count = 0;
		goto response;
	}

	req = net_buf_pull_mem(buf, sizeof(*req));
	req_cid_count = buf->len / sizeof(scid);

	if (buf->len > sizeof(dcid)) {
		BT_ERR("Too large LE conn req packet size");
		req_cid_count = L2CAP_ECRED_CHAN_MAX_PER_REQ;
		result = BT_L2CAP_LE_ERR_INVALID_PARAMS;
		goto response;
	}

	psm = sys_le16_to_cpu(req->psm);
	mtu = sys_le16_to_cpu(req->mtu);
	mps = sys_le16_to_cpu(req->mps);
	credits = sys_le16_to_cpu(req->credits);

	/* Save local cid. */
	(void)memcpy(lcid, net_buf_simple_tail(&buf->b), sizeof(lcid[0]) * req_cid_count);

	BT_DBG("psm 0x%02x mtu %u mps %u credits %u", psm, mtu, mps, credits);

	if (mtu < L2CAP_ECRED_MIN_MTU || mps < L2CAP_ECRED_MIN_MTU) {
		BT_ERR("Invalid ecred conn req params");
		result = BT_L2CAP_LE_ERR_INVALID_PARAMS;
		goto response;
	}

	/* Check if there is a server registered */
	server = l2cap_server_lookup_psm(psm);
	if (!server) {
		result = BT_L2CAP_LE_ERR_PSM_NOT_SUPP;
		goto response;
	}

	/* Check if connection has minimum required security level */
	result = l2cap_check_security(conn, server);
	if (result != BT_L2CAP_LE_SUCCESS) {
		goto response;
	}

	while (buf->len >= sizeof(scid)) {
		uint16_t rc;
		scid = net_buf_pull_le16(buf);

		rc = l2cap_chan_accept(conn, server, scid, mtu, mps,
				credits, &chan[i]);
		if (rc != BT_L2CAP_LE_SUCCESS) {
			/* Walkaround, here we just accept the result if lcid is not 0x0000,
			 because the error code already packed in ethermind, so we only need to give others cid result. */
			if(0x0000 != lcid[i])
			{
				result = rc;
			}
		}
		switch (rc) {
		case BT_L2CAP_LE_SUCCESS:
			ch = BT_L2CAP_LE_CHAN(chan[i]);
			ch->rx.cid = lcid[i];
			dcid[i++] = sys_cpu_to_le16(ch->rx.cid);
			continue;
		/* Some connections refused – invalid Source CID */
		/* Some connections refused – Source CID already allocated */
		/* Some connections refused – not enough resources
		 * available.
		 */
		default:
			/* If a Destination CID is 0x0000, the channel was not
			 * established.
			 */
			dcid[i++] = 0x0000;
			continue;
		}
	}

response:
#if 0
	buf = l2cap_create_le_sig_pdu(buf, BT_L2CAP_ECRED_CONN_RSP, ident,
				      sizeof(*rsp) +
				      (sizeof(scid) * req_cid_count));
	if (!buf) {
		goto callback;
	}

	rsp = net_buf_add(buf, sizeof(*rsp));
	(void)memset(rsp, 0, sizeof(*rsp));
	if (ch) {
		rsp->mps = sys_cpu_to_le16(ch->rx.mps);
		rsp->mtu = sys_cpu_to_le16(ch->rx.mtu);
		rsp->credits = sys_cpu_to_le16(ch->rx.init_credits);
	}
	rsp->result = sys_cpu_to_le16(result);

	net_buf_add_mem(buf, dcid, sizeof(scid) * req_cid_count);

	l2cap_send(conn, BT_L2CAP_CID_LE_SIG, buf);
#endif
	(void)memset(&ethermind_ecbfc_connect_param, 0, sizeof(ethermind_ecbfc_connect_param));
	if(ch)
	{
		ethermind_ecbfc_connect_param.mtu = ch->rx.mtu;
		ethermind_ecbfc_connect_param.mps = ch->rx.mps;
		ethermind_ecbfc_connect_param.credit = ch->rx.init_credits;
	}
	ethermind_ecbfc_connect_param.num_cids = req_cid_count;
	for (uint8_t index = 0; ((index < req_cid_count) && (index < L2CAP_ECBFC_MAX_NUM_CIDS)); ++index)
	{
		ethermind_ecbfc_connect_param.cid[index] = dcid[index];
	}

	(void)l2ca_ecbfc_connect_rsp
	(
		lcid,
		req_cid_count,
		result,
		&ethermind_ecbfc_connect_param
	);
}

static void le_ecred_reconf_req(struct bt_l2cap *l2cap, uint8_t ident,
				struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_l2cap_chan *chans[L2CAP_ECRED_CHAN_MAX_PER_REQ];
	struct bt_l2cap_ecred_reconf_req *req;
	struct bt_l2cap_ecred_reconf_rsp *rsp;
	uint16_t mtu, mps;
	uint16_t scid, result = BT_L2CAP_RECONF_SUCCESS;
        uint16_t lcids[L2CAP_ECRED_CHAN_MAX];
	int chan_count = 0;
	bool mps_reduced = false;

	if (buf->len < sizeof(*req)) {
		BT_ERR("Too small ecred reconf req packet size");
		return;
	}

	req = net_buf_pull_mem(buf, sizeof(*req));

	mtu = sys_le16_to_cpu(req->mtu);
	mps = sys_le16_to_cpu(req->mps);

	if (mps < L2CAP_ECRED_MIN_MTU) {
		result = BT_L2CAP_RECONF_OTHER_UNACCEPT;
		goto response;
	}

	if (mtu < L2CAP_ECRED_MIN_MTU) {
		result = BT_L2CAP_RECONF_INVALID_MTU;
		goto response;
	}

	while (buf->len >= sizeof(scid)) {
		struct bt_l2cap_chan *chan;
		scid = net_buf_pull_le16(buf);
		chan = bt_l2cap_le_lookup_tx_cid(conn, scid);
		if (!chan) {
			result = BT_L2CAP_RECONF_INVALID_CID;
			goto response;
		}

		if (BT_L2CAP_LE_CHAN(chan)->tx.mtu > mtu) {
			BT_ERR("chan %p decreased MTU %u -> %u", chan,
			       BT_L2CAP_LE_CHAN(chan)->tx.mtu, mtu);
			result = BT_L2CAP_RECONF_INVALID_MTU;
			goto response;
		}

		if (BT_L2CAP_LE_CHAN(chan)->tx.mps > mps) {
			mps_reduced = true;
		}

		chans[chan_count] = chan;
		chan_count++;
	}

	/* As per BT Core Spec V5.2 Vol. 3, Part A, section 7.11
	 * The request (...) shall not decrease the MPS of a channel
	 * if more than one channel is specified.
	 */
	if (mps_reduced && chan_count > 1) {
		result = BT_L2CAP_RECONF_INVALID_MPS;
		goto response;
	}

	for (int i = 0; i < chan_count; i++) {
		BT_L2CAP_LE_CHAN(chans[i])->tx.mtu = mtu;
		BT_L2CAP_LE_CHAN(chans[i])->tx.mps = mps;

		if (chans[i]->ops->reconfigured) {
			chans[i]->ops->reconfigured(chans[i]);
		}
	}

	BT_DBG("mtu %u mps %u", mtu, mps);

response:
#if 0
	buf = l2cap_create_le_sig_pdu(buf, BT_L2CAP_ECRED_RECONF_RSP, ident,
				      sizeof(*rsp));

	rsp = net_buf_add(buf, sizeof(*rsp));
	rsp->result = sys_cpu_to_le16(result);

	l2cap_send(conn, BT_L2CAP_CID_LE_SIG, buf);
#endif
        for (int i = 0; i < chan_count; i++)
        {
            lcids[i] = BT_L2CAP_LE_CHAN(chans[i])->rx.cid;
        }

        (void)l2ca_ecbfc_reconfig_rsp
         (
             lcids,
             chan_count,
             result
         );
}

static void le_ecred_reconf_rsp(struct bt_l2cap *l2cap, uint8_t ident,
				struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_l2cap_ecred_reconf_rsp *rsp;
	struct bt_l2cap_le_chan *ch;
	uint16_t result;

	if (buf->len < sizeof(*rsp)) {
		BT_ERR("Too small ecred reconf rsp packet size");
		return;
	}

	rsp = net_buf_pull_mem(buf, sizeof(*rsp));
	result = sys_le16_to_cpu(rsp->result);

	while ((ch = l2cap_lookup_ident(conn, ident))) {
		/* Stop timer started on REQ send. The timer is only set on one
		 * of the channels, but we don't want to make assumptions on
		 * which one it is.
		 */
		k_work_cancel_delayable(&ch->rtx_work);

		if (result == BT_L2CAP_LE_SUCCESS) {
			ch->rx.mtu = ch->pending_rx_mtu;
		}

		ch->pending_rx_mtu = 0;
		ch->ident = 0U;

		if (ch->chan.ops->reconfigured) {
			ch->chan.ops->reconfigured(&ch->chan);
		}
	}
}
#endif /* defined(CONFIG_BT_L2CAP_ECRED) */

static struct bt_l2cap_le_chan *l2cap_remove_rx_cid(struct bt_conn *conn,
						    uint16_t cid)
{
	struct bt_l2cap_chan *chan;
	sys_snode_t *prev = NULL;

	/* Protect fixed channels against accidental removal */
	if (!L2CAP_LE_CID_IS_DYN(cid)) {
		return NULL;
	}

	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		if (BT_L2CAP_LE_CHAN(chan)->rx.cid == cid) {
			sys_slist_remove(&conn->channels, prev, &chan->node);
			return BT_L2CAP_LE_CHAN(chan);
		}

		prev = &chan->node;
	}

	return NULL;
}

static void le_disconn_req(struct bt_l2cap *l2cap, uint8_t ident,
			   struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_l2cap_le_chan *chan;
	struct bt_l2cap_disconn_req *req = (void *)buf->data;
	struct bt_l2cap_disconn_rsp *rsp;
	uint16_t dcid;

	if (buf->len < sizeof(*req)) {
		BT_ERR("Too small LE conn req packet size");
		return;
	}

	dcid = sys_le16_to_cpu(req->dcid);

	BT_DBG("dcid 0x%04x scid 0x%04x", dcid, sys_le16_to_cpu(req->scid));

	chan = l2cap_remove_rx_cid(conn, dcid);
#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED > 0U))
#if 0
#endif
	if (!chan) {
		struct bt_l2cap_cmd_reject_cid_data data;

		data.scid = req->scid;
		data.dcid = req->dcid;

		l2cap_send_reject(conn, ident, BT_L2CAP_REJ_INVALID_CID, &data,
				  sizeof(data));
		return;
	}

	buf = l2cap_create_le_sig_pdu(buf, BT_L2CAP_DISCONN_RSP, ident,
				      sizeof(*rsp));
	if (!buf) {
		return;
	}

	rsp = net_buf_add(buf, sizeof(*rsp));
	rsp->dcid = sys_cpu_to_le16(chan->rx.cid);
	rsp->scid = sys_cpu_to_le16(chan->tx.cid);
#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED > 0U))
#endif
#endif
	bt_l2cap_chan_del(&chan->chan);
#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED > 0U))
#if 0
#endif
	l2cap_send(conn, BT_L2CAP_CID_LE_SIG, buf);
#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED > 0U))
#endif
#endif

    (void)rsp;  /* Unused variable */
}

static int l2cap_change_security(struct bt_l2cap_le_chan *chan, uint16_t err)
{
#if ((defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U)) || (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U)))
	struct bt_conn *conn = chan->chan.conn;
#endif
	bt_security_t sec;
	int ret = 0;

	if (atomic_test_bit(chan->chan.status,
			    BT_L2CAP_STATUS_ENCRYPT_PENDING)) {
		return -EINPROGRESS;
	}
#if ((defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U)) || (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U)))
	switch (err) {
	case BT_L2CAP_LE_ERR_ENCRYPTION:
		if (conn->sec_level >= BT_SECURITY_L2) {
			return -EALREADY;
		}

		sec = BT_SECURITY_L2;
		break;
	case BT_L2CAP_LE_ERR_AUTHENTICATION:
		if (conn->sec_level < BT_SECURITY_L2) {
			sec = BT_SECURITY_L2;
		} else if (conn->sec_level < BT_SECURITY_L3) {
			sec = BT_SECURITY_L3;
		} else if (conn->sec_level < BT_SECURITY_L4) {
			sec = BT_SECURITY_L4;
		} else {
			return -EALREADY;
		}
		break;
	default:
		return -EINVAL;
	}

	ret = bt_conn_set_security(chan->chan.conn, sec);
	if (ret < 0) {
		return ret;
	}

	atomic_set_bit(chan->chan.status, BT_L2CAP_STATUS_ENCRYPT_PENDING);

	return 0;
#else
	(void)ret;
	(void)sec;
    return -ESRCH;
#endif
}

#if (defined(CONFIG_BT_L2CAP_ECRED) && ((CONFIG_BT_L2CAP_ECRED) > 0))
static void le_ecred_conn_rsp(struct bt_l2cap *l2cap, uint8_t ident,
			      struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_l2cap_le_chan *chan;
	struct bt_l2cap_ecred_conn_rsp *rsp;
	uint16_t dcid, mtu, mps, credits, result, psm;
	uint8_t attempted = 0;
	uint8_t succeeded = 0;

	if (buf->len < sizeof(*rsp)) {
		BT_ERR("Too small ecred conn rsp packet size");
		return;
	}

	rsp = net_buf_pull_mem(buf, sizeof(*rsp));
	mtu = sys_le16_to_cpu(rsp->mtu);
	mps = sys_le16_to_cpu(rsp->mps);
	credits = sys_le16_to_cpu(rsp->credits);
	result = sys_le16_to_cpu(rsp->result);

	BT_DBG("mtu 0x%04x mps 0x%04x credits 0x%04x result %u", mtu,
	       mps, credits, result);

	chan = l2cap_lookup_ident(conn, ident);
	if (chan) {
		psm = chan->psm;
	} else {
		psm = 0x0000;
	}

	switch (result) {
	case BT_L2CAP_LE_ERR_AUTHENTICATION:
	case BT_L2CAP_LE_ERR_ENCRYPTION:
		while ((chan = l2cap_lookup_ident(conn, ident))) {
			/* Cancel RTX work */
#if 0
			k_work_cancel_delayable(&chan->rtx_work);
#endif

			/* If security needs changing wait it to be completed */
			if (!l2cap_change_security(chan, result)) {
				return;
			}
			bt_l2cap_chan_remove(conn, &chan->chan);
			bt_l2cap_chan_del(&chan->chan);
		}
		break;
	case BT_L2CAP_LE_SUCCESS:
	/* Some connections refused – invalid Source CID */
	case BT_L2CAP_LE_ERR_INVALID_SCID:
	/* Some connections refused – Source CID already allocated */
	case BT_L2CAP_LE_ERR_SCID_IN_USE:
	/* Some connections refused – not enough resources available */
	case BT_L2CAP_LE_ERR_NO_RESOURCES:
		while ((chan = l2cap_lookup_ident(conn, ident))) {
			struct bt_l2cap_chan *c;

			/* Cancel RTX work */
#if 0
			k_work_cancel_delayable(&chan->rtx_work);
#endif
			if (buf->len < sizeof(dcid)) {
				BT_ERR("Fewer dcid values than expected");
				bt_l2cap_chan_remove(conn, &chan->chan);
				bt_l2cap_chan_del(&chan->chan);
				continue;
			}

			if (buf->len < sizeof(dcid)) {
				BT_ERR("Fewer dcid values than expected");
				bt_l2cap_chan_remove(conn, &chan->chan);
				bt_l2cap_chan_del(&chan->chan);
				continue;
			}

			dcid = net_buf_pull_le16(buf);
			attempted++;

			BT_DBG("dcid 0x%04x", dcid);

			/* If a Destination CID is 0x0000, the channel was not
			 * established.
			 */
			if (!dcid) {
				bt_l2cap_chan_remove(conn, &chan->chan);
				bt_l2cap_chan_del(&chan->chan);
				continue;
			}

			c = bt_l2cap_le_lookup_tx_cid(conn, dcid);
			if (c) {
				/* If a device receives a
				 * L2CAP_CREDIT_BASED_CONNECTION_RSP packet
				 * with an already assigned Destination CID,
				 * then both the original channel and the new
				 * channel shall be immediately discarded and
				 * not used.
				 */
				bt_l2cap_chan_remove(conn, &chan->chan);
				bt_l2cap_chan_del(&chan->chan);
				bt_l2cap_chan_disconnect(c);
				continue;
			}

			chan->tx.cid = dcid;

			chan->ident = 0U;

			chan->tx.mtu = mtu;
			chan->tx.mps = mps;

			/* Update state */
			bt_l2cap_chan_set_state(&chan->chan,
						BT_L2CAP_CONNECTED);

			if (chan->chan.ops->connected) {
				chan->chan.ops->connected(&chan->chan);
			}

			/* Give credits */
			l2cap_chan_tx_give_credits(chan, credits);
			l2cap_chan_rx_give_credits(chan, chan->rx.init_credits);

			succeeded++;
		}
		break;
	case BT_L2CAP_LE_ERR_PSM_NOT_SUPP:
	default:
		while ((chan = l2cap_remove_ident(conn, ident))) {
			bt_l2cap_chan_del(&chan->chan);
		}
		break;
	}

	if (ecred_cb && ecred_cb->ecred_conn_rsp) {
		ecred_cb->ecred_conn_rsp(conn, result, attempted, succeeded, psm);
	}
}
#endif /* CONFIG_BT_L2CAP_ECRED */

static void le_conn_rsp(struct bt_l2cap *l2cap, uint8_t ident,
			struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_l2cap_le_chan *chan;
	struct bt_l2cap_le_conn_rsp *rsp = (void *)buf->data;
	uint16_t dcid, mtu, mps, credits, result;

	if (buf->len < sizeof(*rsp)) {
		BT_ERR("Too small LE conn rsp packet size");
		return;
	}

	dcid = sys_le16_to_cpu(rsp->dcid);
	mtu = sys_le16_to_cpu(rsp->mtu);
	mps = sys_le16_to_cpu(rsp->mps);
	credits = sys_le16_to_cpu(rsp->credits);
	result = sys_le16_to_cpu(rsp->result);

	BT_DBG("dcid 0x%04x mtu %u mps %u credits %u result 0x%04x", dcid,
	       mtu, mps, credits, result);

	/* Keep the channel in case of security errors */
	if (result == BT_L2CAP_LE_SUCCESS ||
	    result == BT_L2CAP_LE_ERR_AUTHENTICATION ||
	    result == BT_L2CAP_LE_ERR_ENCRYPTION) {
		chan = l2cap_lookup_ident(conn, ident);
	} else {
		chan = l2cap_remove_ident(conn, ident);
	}

	if (!chan) {
		BT_ERR("Cannot find channel for ident %u", ident);
		return;
	}

	/* Cancel RTX work */
#if 0
	k_work_cancel_delayable(&chan->rtx_work);
#endif

	/* Reset ident since it got a response */
	chan->ident = 0U;

	switch (result) {
	case BT_L2CAP_LE_SUCCESS:
		chan->tx.cid = dcid;
		chan->tx.mtu = mtu;
		chan->tx.mps = mps;

		/* Update state */
		bt_l2cap_chan_set_state(&chan->chan, BT_L2CAP_CONNECTED);

		if (chan->chan.ops->connected) {
			chan->chan.ops->connected(&chan->chan);
		}

		/* Give credits */
		l2cap_chan_tx_give_credits(chan, credits);
		l2cap_chan_rx_give_credits(chan, chan->rx.init_credits);

		break;
	case BT_L2CAP_LE_ERR_AUTHENTICATION:
	case BT_L2CAP_LE_ERR_ENCRYPTION:
		/* If security needs changing wait it to be completed */
		if (l2cap_change_security(chan, result) == 0) {
			return;
		}
		bt_l2cap_chan_remove(conn, &chan->chan);
		__fallthrough;
	default:
		bt_l2cap_chan_del(&chan->chan);
	}
}

static void le_disconn_rsp(struct bt_l2cap *l2cap, uint8_t ident,
			   struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_l2cap_le_chan *chan;
	struct bt_l2cap_disconn_rsp *rsp = (void *)buf->data;
	uint16_t scid;

	if (buf->len < sizeof(*rsp)) {
		BT_ERR("Too small LE disconn rsp packet size");
		return;
	}

	scid = sys_le16_to_cpu(rsp->scid);

	BT_DBG("dcid 0x%04x scid 0x%04x", sys_le16_to_cpu(rsp->dcid), scid);

	chan = l2cap_remove_rx_cid(conn, scid);
	if (!chan) {
		return;
	}

	bt_l2cap_chan_del(&chan->chan);
}
#if 0
static inline struct net_buf *l2cap_alloc_seg(struct net_buf *buf)
{
	struct net_buf_pool *pool = net_buf_pool_get(buf->pool_id);
	struct net_buf *seg;

	/* Use the dedicated segment callback if registered */
	if (ch->chan.ops->alloc_seg) {
		seg = ch->chan.ops->alloc_seg(&ch->chan);
		__ASSERT_NO_MSG(seg);
	} else {
		/* Try to use original pool if possible */
		seg = net_buf_alloc(pool, osaWaitNone_c);
	}

	if (seg) {
		net_buf_reserve(seg, BT_L2CAP_CHAN_SEND_RESERVE);
		return seg;
	}

	/* Fallback to using global connection tx pool */
	return bt_l2cap_create_pdu_timeout(NULL, 0, osaWaitNone_c);
}

static struct net_buf *l2cap_chan_create_seg(struct bt_l2cap_le_chan *ch,
					     struct net_buf *buf,
					     size_t sdu_hdr_len)
{
	struct net_buf *seg;
	uint16_t headroom;
	uint16_t len;

	/* Segment if data (+ data headroom) is bigger than MPS */
	if (buf->len + sdu_hdr_len > ch->tx.mps) {
		goto segment;
	}

	headroom = BT_L2CAP_CHAN_SEND_RESERVE + sdu_hdr_len;

	/* Check if original buffer has enough headroom and don't have any
	 * fragments.
	 */
	if (net_buf_headroom(buf) >= headroom && !buf->frags) {
		if (sdu_hdr_len) {
			/* Push SDU length if set */
			net_buf_push_le16(buf, net_buf_frags_len(buf));
		}
		return net_buf_ref(buf);
	}

segment:
	seg = l2cap_alloc_seg(buf, ch);

	if (!seg) {
		return NULL;
	}

	if (sdu_hdr_len) {
		net_buf_add_le16(seg, net_buf_frags_len(buf));
	}

	/* Don't send more that TX MPS including SDU length */
	len = MIN(net_buf_tailroom(seg), ch->tx.mps - sdu_hdr_len);
	/* Limit if original buffer is smaller than the segment */
	len = MIN(buf->len, len);
	net_buf_add_mem(seg, buf->data, len);
	net_buf_pull(buf, len);

	BT_DBG("ch %p seg %p len %u", ch, seg, seg->len);

	return seg;
}
#endif
static void l2cap_chan_tx_resume(struct bt_l2cap_le_chan *ch)
{
	if (!atomic_get(&ch->tx.credits) ||
	    ((0 == OSA_MsgQAvailableMsgs(ch->tx_queue.queue)) && !ch->tx_buf)) {
		return;
	}
#if 1
	k_work_submit(&ch->tx_work);
#endif
}

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && (CONFIG_BT_L2CAP_DYNAMIC_CHANNEL > 0))
static void resume_all_channels(struct bt_conn *conn, void *data)
{
	struct bt_l2cap_chan *chan;

	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		l2cap_chan_tx_resume(BT_L2CAP_LE_CHAN(chan));
	}
}
#endif

static void l2cap_chan_sdu_sent(struct bt_conn *conn, void *user_data, int err)
{
	struct l2cap_tx_meta_data *data = user_data;
	struct bt_l2cap_chan *chan;
	bt_conn_tx_cb_t cb = data->cb;
	void *cb_user_data = data->user_data;
	uint16_t cid = data->cid;

	BT_DBG("conn %p CID 0x%04x err %d", conn, cid, err);

	if ((uint32_t)cb_user_data != 0xFFFFFFFFu)
	{
		free_tx_meta_data(data);
	}

	if (err) {
		if (cb) {
			cb(conn, cb_user_data, err);
		}

		return;
	}

	chan = bt_l2cap_le_lookup_tx_cid(conn, cid);
	if (!chan) {
		/* Received SDU sent callback for disconnected channel */
		return;
	}

	if (chan->ops->sent) {
		chan->ops->sent(chan);
	}

	if (cb) {
		cb(conn, cb_user_data, 0);
	}

	/* Resume the current channel */
	l2cap_chan_tx_resume(BT_L2CAP_LE_CHAN(chan));

	if (IS_ENABLED(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL)) {
		/* Resume all other channels in case one might be stuck.
		 * The current channel has already been given priority.
		 */
		bt_conn_foreach(BT_CONN_TYPE_LE, resume_all_channels, NULL);
	}
}
#if 0
static void l2cap_chan_seg_sent(struct bt_conn *conn, void *user_data, int err)
{
	struct l2cap_tx_meta_data *data = user_data;
	struct bt_l2cap_chan *chan;

	BT_DBG("conn %p CID 0x%04x err %d", conn, data->cid, err);

	if (err) {
		return;
	}

	chan = bt_l2cap_le_lookup_tx_cid(conn, data->cid);
	if (!chan) {
		/* Received segment sent callback for disconnected channel */
		return;
	}

	l2cap_chan_tx_resume(BT_L2CAP_LE_CHAN(chan));
}

static bool test_and_dec(atomic_t *target)
{
	atomic_t old_value, new_value;

	do {
		old_value = atomic_get(target);
		if (!old_value) {
			return false;
		}

		new_value = old_value - 1;
	} while (atomic_cas(target, old_value, new_value) == 0);

	return true;
}
#endif
#if 0
/* This returns -EAGAIN whenever a segment cannot be send immediately which can
 * happen under the following circuntances:
 *
 * 1. There are no credits
 * 2. There are no buffers
 * 3. There are no TX contexts
 *
 * In all cases the original buffer is unaffected so it can be pushed back to
 * be sent later.
 */
static int l2cap_chan_le_send(struct bt_l2cap_le_chan *ch,
			      struct net_buf *buf, uint16_t sdu_hdr_len)
{
	struct net_buf *seg;
	struct net_buf_simple_state state;
	int len, err;

	if (!test_and_dec(&ch->tx.credits)) {
		BT_WARN("No credits to transmit packet");
		return -EAGAIN;
	}

	/* Save state so it can be restored if we failed to send */
	net_buf_simple_save(&buf->b, &state);

	seg = l2cap_chan_create_seg(ch, buf, sdu_hdr_len);
	if (!seg) {
		atomic_inc(&ch->tx.credits);
		return -EAGAIN;
	}

	BT_DBG("ch %p cid 0x%04x len %u credits %lu", ch, ch->tx.cid,
	       seg->len, atomic_get(&ch->tx.credits));

	len = seg->len - sdu_hdr_len;

	/* Set a callback if there is no data left in the buffer */
	if (buf == seg || !buf->len) {
		err = bt_l2cap_send_cb(ch->chan.conn, ch->tx.cid, seg,
				       l2cap_chan_sdu_sent,
				       l2cap_tx_meta_data(buf));
	} else {
		err = bt_l2cap_send_cb(ch->chan.conn, ch->tx.cid, seg,
				       l2cap_chan_seg_sent,
				       l2cap_tx_meta_data(buf));
	}

	if (err) {
		BT_WARN("Unable to send seg %d", err);
		atomic_inc(&ch->tx.credits);

		/* The host takes ownership of the reference in seg when
		 * bt_l2cap_send_cb is successful. The call returned an error,
		 * so we must get rid of the reference that was taken in
		 * l2cap_chan_create_seg.
		 */
		net_buf_unref(seg);

		if (err == -ENOBUFS) {
			/* Restore state since segment could not be sent */
			net_buf_simple_restore(&buf->b, &state);
			return -EAGAIN;
		}

		return err;
	}

	/* Check if there is no credits left clear output status and notify its
	 * change.
	 */
	if (!atomic_get(&ch->tx.credits)) {
		atomic_clear_bit(ch->chan.status, BT_L2CAP_STATUS_OUT);
		if (ch->chan.ops->status) {
			ch->chan.ops->status(&ch->chan, ch->chan.status);
		}
	}

	return len;
}
#endif
static int l2cap_chan_le_send_sdu(struct bt_l2cap_le_chan *ch,
				  struct net_buf **buf, uint16_t sent)
{
#if 0
	int ret, total_len;
	struct net_buf *frag;

	total_len = net_buf_frags_len(*buf) + sent;

	if (total_len > ch->tx.mtu) {
		return -EMSGSIZE;
	}

	frag = *buf;
	if (!frag->len && frag->frags) {
		frag = frag->frags;
	}

	if (!sent) {
		/* Add SDU length for the first segment */
		ret = l2cap_chan_le_send(ch, frag, BT_L2CAP_SDU_HDR_SIZE);
		if (ret < 0) {
			if (ret == -EAGAIN) {
				/* Store sent data into user_data */
				l2cap_tx_meta_data(frag)->sent = sent;
			}
			*buf = frag;
			return ret;
		}
		sent = ret;
	}

	/* Send remaining segments */
	for (ret = 0; sent < total_len; sent += ret) {
		/* Proceed to next fragment */
		if (!frag->len) {
			frag = net_buf_frag_del(NULL, frag);
		}

		ret = l2cap_chan_le_send(ch, frag, 0);
		if (ret < 0) {
			if (ret == -EAGAIN) {
				/* Store sent data into user_data */
				l2cap_tx_meta_data(frag)->sent = sent;
			}
			*buf = frag;
			return ret;
		}
	}

	BT_DBG("ch %p cid 0x%04x sent %u total_len %u", ch, ch->tx.cid, sent,
	       total_len);

	net_buf_unref(frag);

	return ret;
#else
    API_RESULT retval;

	if (NULL != ch->_sdu)
	{
		return -EAGAIN;
	}

	ch->_sdu = *buf;
	ch->_sdu_len = (*buf)->len;
    BT_DBG("ch %p cid 0x%04x buf %p total_len %u\r\n", ch, ch->rx.cid, *buf,
       ch->_sdu_len);
	retval = l2ca_channel_data_write
				(
					&ch->chan.conn->deviceId,
					ch->rx.cid,
					(*buf)->data,
					(*buf)->len
				);
	if(retval != API_SUCCESS)
	{
		ch->_sdu = NULL;
		ch->_sdu_len = 0;
		return -EAGAIN;
	}
	return (*buf)->len;
#endif
}

static void le_credits(struct bt_l2cap *l2cap, uint8_t ident,
		       struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_l2cap_chan *chan;
	struct bt_l2cap_le_credits *ev = (void *)buf->data;
	struct bt_l2cap_le_chan *le_chan;
	uint16_t credits, cid;

	if (buf->len < sizeof(*ev)) {
		BT_ERR("Too small LE Credits packet size");
		return;
	}

	cid = sys_le16_to_cpu(ev->cid);
	credits = sys_le16_to_cpu(ev->credits);

	BT_DBG("cid 0x%04x credits %u", cid, credits);

	chan = bt_l2cap_le_lookup_tx_cid(conn, cid);
	if (!chan) {
		BT_ERR("Unable to find channel of LE Credits packet");
		return;
	}

	le_chan = BT_L2CAP_LE_CHAN(chan);

	if (atomic_get(&le_chan->tx.credits) + credits > UINT16_MAX) {
		BT_ERR("Credits overflow");
		bt_l2cap_chan_disconnect(chan);
		return;
	}

	l2cap_chan_tx_give_credits(le_chan, credits);

	BT_DBG("chan %p total credits %lu",
	       le_chan, atomic_get(&le_chan->tx.credits));

	l2cap_chan_tx_resume(le_chan);
}

static void reject_cmd(struct bt_l2cap *l2cap, uint8_t ident,
		       struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_l2cap_le_chan *chan;

	/* Check if there is a outstanding channel */
	chan = l2cap_remove_ident(conn, ident);
	if (!chan) {
		return;
	}

	bt_l2cap_chan_del(&chan->chan);
}
#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */

static int l2cap_recv(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
	struct bt_l2cap *l2cap = CONTAINER_OF(chan, struct bt_l2cap, chan);
	struct bt_l2cap_sig_hdr *hdr;
	uint16_t len;

	if (buf->len < sizeof(*hdr)) {
		BT_ERR("Too small L2CAP signaling PDU");
		return 0;
	}

	hdr = net_buf_pull_mem(buf, sizeof(*hdr));
	len = sys_le16_to_cpu(hdr->len);

	BT_DBG("Signaling code 0x%02x ident %u len %u", hdr->code,
	       hdr->ident, len);

	if (buf->len != len) {
		BT_ERR("L2CAP length mismatch (%u != %u)", buf->len, len);
		return 0;
	}
#if 0
	if (!hdr->ident) {
		BT_ERR("Invalid ident value in L2CAP PDU");
		return 0;
	}
#endif
	switch (hdr->code) {
	case BT_L2CAP_CONN_PARAM_RSP:
		le_conn_param_rsp(l2cap, buf);
		break;
#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
	case BT_L2CAP_LE_CONN_REQ:
		le_conn_req(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_LE_CONN_RSP:
		le_conn_rsp(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_DISCONN_REQ:
		le_disconn_req(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_DISCONN_RSP:
		le_disconn_rsp(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_LE_CREDITS:
		le_credits(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_CMD_REJECT:
		reject_cmd(l2cap, hdr->ident, buf);
		break;
#if (defined(CONFIG_BT_L2CAP_ECRED) && ((CONFIG_BT_L2CAP_ECRED) > 0U))
	case BT_L2CAP_ECRED_CONN_REQ:
		le_ecred_conn_req(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_ECRED_CONN_RSP:
		le_ecred_conn_rsp(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_ECRED_RECONF_REQ:
		le_ecred_reconf_req(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_ECRED_RECONF_RSP:
		le_ecred_reconf_rsp(l2cap, hdr->ident, buf);
		break;
#endif /* defined(CONFIG_BT_L2CAP_ECRED) */
#else
	case BT_L2CAP_CMD_REJECT:
		/* Ignored */
		break;
#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */
	case BT_L2CAP_CONN_PARAM_REQ:
		if (IS_ENABLED(CONFIG_BT_CENTRAL)) {
			le_conn_param_update_req(l2cap, hdr->ident, buf);
			break;
		}
		__fallthrough;
	default:
		BT_WARN("Unknown L2CAP PDU code 0x%02x", hdr->code);
		l2cap_send_reject(chan->conn, hdr->ident,
				  BT_L2CAP_REJ_NOT_UNDERSTOOD, NULL, 0);
		break;
	}

	return 0;
}

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
#if 0
static void l2cap_chan_shutdown(struct bt_l2cap_chan *chan)
{
	struct bt_l2cap_le_chan *le_chan = BT_L2CAP_LE_CHAN(chan);
	struct net_buf *buf;

	BT_DBG("chan %p", chan);

	atomic_set_bit(chan->status, BT_L2CAP_STATUS_SHUTDOWN);

	/* Destroy segmented SDU if it exists */
	if (le_chan->_sdu) {
		net_buf_unref(le_chan->_sdu);
		le_chan->_sdu = NULL;
		le_chan->_sdu_len = 0U;
	}

	/* Cleanup outstanding request */
	if (le_chan->tx_buf) {
		net_buf_unref(le_chan->tx_buf);
		le_chan->tx_buf = NULL;
	}

	/* Remove buffers on the TX queue */
	while ((buf = net_buf_get(le_chan->tx_queue, osaWaitNone_c))) {
		net_buf_unref(buf);
	}

	/* Remove buffers on the RX queue */
	while ((buf = net_buf_get(le_chan->rx_queue, osaWaitNone_c))) {
		net_buf_unref(buf);
	}

	/* Update status */
	if (chan->ops->status) {
		chan->ops->status(chan, chan->status);
	}
}
#endif

/** @brief Get @c chan->state.
 *
 * This field does not exist when @kconfig{CONFIG_BT_L2CAP_DYNAMIC_CHANNEL} is
 * disabled. In that case, this function returns @ref BT_L2CAP_CONNECTED since
 * the struct can only represent static channels in that case and static
 * channels are always connected.
 */
static inline bt_l2cap_chan_state_t bt_l2cap_chan_get_state(struct bt_l2cap_chan *chan)
{
#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && (CONFIG_BT_L2CAP_DYNAMIC_CHANNEL > 0))
	return GET_CHAN_MEMBER(chan, state);
#else
	return BT_L2CAP_CONNECTED;
#endif
}

static void l2cap_chan_send_credits(struct bt_l2cap_le_chan *chan,
				    struct net_buf *buf, uint16_t credits)
{
	uint16_t old_credits;

	__ASSERT_NO_MSG(bt_l2cap_chan_get_state(&chan->chan) == BT_L2CAP_CONNECTED);

	/* Cap the number of credits given */
	if (credits > chan->rx.init_credits) {
		credits = chan->rx.init_credits;
	}

	/* Don't send back more than the initial amount. */
	old_credits = atomic_get(&chan->rx.credits);
	if (credits + old_credits > chan->rx.init_credits) {
		credits = chan->rx.init_credits - old_credits;
	}

	l2cap_chan_rx_give_credits(chan, credits);

    (void)l2ca_cbfc_send_flow_control_credit
             (
                 sys_cpu_to_le16(chan->rx.cid),
                 sys_cpu_to_le16(credits)
             );

	BT_DBG("chan %p credits %lu", chan, atomic_get(&chan->rx.credits));
}
#if 0
static void l2cap_chan_update_credits(struct bt_l2cap_le_chan *chan,
				      struct net_buf *buf)
{
	uint16_t credits;
	atomic_val_t old_credits = atomic_get(&chan->rx.credits);

	/* Restore enough credits to complete the sdu */
	credits = ((chan->_sdu_len - net_buf_frags_len(buf)) +
		   (chan->rx.mps - 1)) / chan->rx.mps;

	BT_DBG("cred %d old %d", credits, (int)old_credits);

	if (credits < old_credits) {
		return;
	}

	credits -= old_credits;

	l2cap_chan_send_credits(chan, buf, credits);
}
#endif
int bt_l2cap_chan_recv_complete(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
	struct bt_l2cap_le_chan *le_chan = BT_L2CAP_LE_CHAN(chan);
	struct bt_conn *conn = chan->conn;

	__ASSERT_NO_MSG(chan);
	__ASSERT_NO_MSG(buf);

	if (!conn) {
		return -ENOTCONN;
	}

	if (conn->type != BT_CONN_TYPE_LE) {
		return -ENOTSUP;
	}

	BT_DBG("chan %p buf %p", chan, buf);

	if (bt_l2cap_chan_get_state(&le_chan->chan) == BT_L2CAP_CONNECTED) {
		uint16_t credits;

		/* Restore credits used by packet */
		memcpy(&credits, net_buf_user_data(buf), sizeof(credits));

		l2cap_chan_send_credits(le_chan, buf, credits);
	}

	net_buf_unref(buf);

	return 0;
}
#if 0
static struct net_buf *l2cap_alloc_frag(size_t timeout, void *user_data)
{
	struct bt_l2cap_le_chan *chan = user_data;
	struct net_buf *frag = NULL;

	frag = chan->chan.ops->alloc_buf(&chan->chan);
	if (!frag) {
		return NULL;
	}

	BT_DBG("frag %p tailroom %lu", frag, net_buf_tailroom(frag));

	return frag;
}

static void l2cap_chan_le_recv_sdu(struct bt_l2cap_le_chan *chan,
				   struct net_buf *buf, uint16_t seg)
{
	int err;

	BT_DBG("chan %p len %lu", chan, net_buf_frags_len(buf));

	__ASSERT_NO_MSG(bt_l2cap_chan_get_state(&chan->chan) == BT_L2CAP_CONNECTED);

	/* Receiving complete SDU, notify channel and reset SDU buf */
	err = chan->chan.ops->recv(&chan->chan, buf);
	if (err < 0) {
		if (err != -EINPROGRESS) {
			BT_ERR("err %d", err);
			bt_l2cap_chan_disconnect(&chan->chan);
			net_buf_unref(buf);
		}
		return;
	}

	if (bt_l2cap_chan_get_state(&chan->chan) == BT_L2CAP_CONNECTED) {
		l2cap_chan_send_credits(chan, buf, seg);
	}

	net_buf_unref(buf);
}
#endif
#if 0
static void l2cap_chan_le_recv_seg(struct bt_l2cap_le_chan *chan,
				   struct net_buf *buf)
{
	uint16_t len;
	uint16_t seg = 0U;

	len = net_buf_frags_len(chan->_sdu);
	if (len) {
		memcpy(&seg, net_buf_user_data(chan->_sdu), sizeof(seg));
	}

	if (len + buf->len > chan->_sdu_len) {
		BT_ERR("SDU length mismatch");
		bt_l2cap_chan_disconnect(&chan->chan);
		return;
	}

	seg++;
	/* Store received segments in user_data */
	memcpy(net_buf_user_data(chan->_sdu), &seg, sizeof(seg));

	BT_DBG("chan %p seg %d len %lu", chan, seg, net_buf_frags_len(buf));

	/* Append received segment to SDU */
	len = net_buf_append_bytes(chan->_sdu, buf->len, buf->data, osaWaitNone_c,
				   l2cap_alloc_frag, chan);
	if (len != buf->len) {
		BT_ERR("Unable to store SDU");
		bt_l2cap_chan_disconnect(&chan->chan);
		return;
	}

	if (net_buf_frags_len(chan->_sdu) < chan->_sdu_len) {
		/* Give more credits if remote has run out of them, this
		 * should only happen if the remote cannot fully utilize the
		 * MPS for some reason.
		 */
		if (!atomic_get(&chan->rx.credits) &&
		    seg == chan->rx.init_credits) {
			l2cap_chan_update_credits(chan, buf);
		}
		return;
	}

	buf = chan->_sdu;
	chan->_sdu = NULL;
	chan->_sdu_len = 0U;

	l2cap_chan_le_recv_sdu(chan, buf, seg);
}
#endif
static void l2cap_chan_le_recv(struct bt_l2cap_le_chan *chan,
			       struct net_buf *buf)
{
#if 0
	uint16_t sdu_len;
#endif
	uint16_t credit = 0;
	int err;

#if 0
	if (!test_and_dec(&chan->rx.credits)) {
		BT_ERR("No credits to receive packet");
		bt_l2cap_chan_disconnect(&chan->chan);
		return;
	}
#endif

#if 0
	/* Check if segments already exist */
	if (chan->_sdu) {
		l2cap_chan_le_recv_seg(chan, buf);
		return;
	}

	if (buf->len < 2) {
		BT_WARN("Too short data packet");
		bt_l2cap_chan_disconnect(&chan->chan);
		return;
	}

	sdu_len = net_buf_pull_le16(buf);

	BT_DBG("chan %p len %u sdu_len %u", chan, buf->len, sdu_len);

	if (sdu_len > chan->rx.mtu) {
		BT_ERR("Invalid SDU length");
		bt_l2cap_chan_disconnect(&chan->chan);
		return;
	}

	/* Always allocate buffer from the channel if supported. */
	if (chan->chan.ops->alloc_buf) {
		chan->_sdu = chan->chan.ops->alloc_buf(&chan->chan);
		if (!chan->_sdu) {
			BT_ERR("Unable to allocate buffer for SDU");
			bt_l2cap_chan_disconnect(&chan->chan);
			return;
		}
		chan->_sdu_len = sdu_len;
		l2cap_chan_le_recv_seg(chan, buf);
		return;
	}
#endif

	/* Store received segments in user_data */
    /**
     * Calculate possible number of LE L2CAP Frames.
     *
     * Maximum size of each frame shall not exceed local MPS.
     * Account for the 2 octet of SDU length.
     */
	credit = ceiling_fraction((buf->len + 2), chan->rx.mps);
	memcpy(net_buf_user_data(buf), &credit, sizeof(credit));

	err = chan->chan.ops->recv(&chan->chan, buf);
	if (err < 0) {
		if (err != -EINPROGRESS) {
			BT_ERR("err %d", err);
			bt_l2cap_chan_disconnect(&chan->chan);
		}
		return;
	}
#if 1
	l2cap_chan_send_credits(chan, buf, credit);
#endif
}

static void l2cap_chan_recv_queue(struct bt_l2cap_le_chan *chan,
				  struct net_buf *buf)
{
	if (chan->state == BT_L2CAP_DISCONNECTING) {
		BT_WARN("Ignoring data received while disconnecting");
		net_buf_unref(buf);
		return;
	}

	if (atomic_test_bit(chan->chan.status, BT_L2CAP_STATUS_SHUTDOWN)) {
		BT_WARN("Ignoring data received while channel has shutdown");
		net_buf_unref(buf);
		return;
	}

	if (!L2CAP_LE_PSM_IS_DYN(chan->psm)) {
		l2cap_chan_le_recv(chan, buf);
		net_buf_unref(buf);
		return;
	}

	net_buf_put(chan->rx_queue.queue, buf);
#if 1
	k_work_submit(&chan->rx_work);
#endif
}
#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */

static void l2cap_chan_recv(struct bt_l2cap_chan *chan, struct net_buf *buf,
			    bool complete)
{
#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
	struct bt_l2cap_le_chan *le_chan = BT_L2CAP_LE_CHAN(chan);

	if (L2CAP_LE_CID_IS_DYN(le_chan->rx.cid)) {
		if (complete) {
			l2cap_chan_recv_queue(le_chan, buf);
		} else {
			/* if packet was not complete this means peer device
			 * overflowed our RX and channel shall be disconnected
			 */
			bt_l2cap_chan_disconnect(chan);
			net_buf_unref(buf);
		}

		return;
	}
#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */

	BT_DBG("chan %p len %u", chan, buf->len);

	chan->ops->recv(chan, buf);
	net_buf_unref(buf);
}

void bt_l2cap_recv(struct bt_conn *conn, struct net_buf *buf, bool complete)
{
	struct bt_l2cap_hdr *hdr;
	struct bt_l2cap_chan *chan;
	uint16_t cid;

	if (IS_ENABLED(CONFIG_BT_BREDR) &&
	    conn->type == BT_CONN_TYPE_BR) {
		bt_l2cap_br_recv(conn, buf);
		return;
	}

	if (buf->len < sizeof(*hdr)) {
		BT_ERR("Too small L2CAP PDU received");
		net_buf_unref(buf);
		return;
	}

	hdr = net_buf_pull_mem(buf, sizeof(*hdr));
	cid = sys_le16_to_cpu(hdr->cid);

	BT_DBG("Packet for CID %u len %u", cid, buf->len);

	chan = bt_l2cap_le_lookup_rx_cid(conn, cid);
	if (!chan) {
		BT_WARN("Ignoring data for unknown channel ID 0x%04x", cid);
		net_buf_unref(buf);
		return;
	}

	l2cap_chan_recv(chan, buf, complete);
}

int bt_l2cap_update_conn_param(struct bt_conn *conn,
			       const struct bt_le_conn_param *param)
{
#if 0
	struct bt_l2cap_conn_param_req *req;
	struct net_buf *buf;
	int err;

	buf = l2cap_create_le_sig_pdu(NULL, BT_L2CAP_CONN_PARAM_REQ,
				      get_ident(), sizeof(*req));
	if (!buf) {
		return -ENOMEM;
	}

	req = net_buf_add(buf, sizeof(*req));
	req->min_interval = sys_cpu_to_le16(param->interval_min);
	req->max_interval = sys_cpu_to_le16(param->interval_max);
	req->latency = sys_cpu_to_le16(param->latency);
	req->timeout = sys_cpu_to_le16(param->timeout);

	err = bt_l2cap_send(conn, BT_L2CAP_CID_LE_SIG, buf);
	if (err) {
		net_buf_unref(buf);
		return err;
	}

	return 0;
#endif
    API_RESULT retval;

    retval = BT_l2cap_le_connection_param_update_request
                     (
                         &conn->deviceId,
                         param->interval_min,
                         param->interval_max,
                         param->latency,
                         param->timeout
                     );
    if (API_SUCCESS != retval)
    {
        return -EIO;
    }

    return 0;
}

static void l2cap_connected(struct bt_l2cap_chan *chan)
{
	BT_DBG("ch %p cid 0x%04x", BT_L2CAP_LE_CHAN(chan),
	       BT_L2CAP_LE_CHAN(chan)->rx.cid);
}

static void l2cap_disconnected(struct bt_l2cap_chan *chan)
{
	struct bt_l2cap_le_chan *le_chan = BT_L2CAP_LE_CHAN(chan);

	BT_DBG("ch %p cid 0x%04x", le_chan, le_chan->rx.cid);

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && (CONFIG_BT_L2CAP_DYNAMIC_CHANNEL > 0))
	/* Cancel RTX work on signal channel.
	 * Disconnected callback is always called from system workqueue
	 * so this should always succeed.
	 */
	(void)k_work_cancel_delayable(&le_chan->rtx_work);
#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */

	(void)le_chan;   /* Unused variable */
}

static int l2cap_accept(struct bt_conn *conn, struct bt_l2cap_chan **chan)
{
	int i;
	static const struct bt_l2cap_chan_ops ops = {
		.connected = l2cap_connected,
		.disconnected = l2cap_disconnected,
		.recv = l2cap_recv,
	};

	BT_DBG("conn %p handle %u", conn, conn->handle);

	for (i = 0; i < ARRAY_SIZE(bt_l2cap_pool); i++) {
		struct bt_l2cap *l2cap = &bt_l2cap_pool[i];

		if (l2cap->chan.chan.conn) {
			continue;
		}

		l2cap->chan.chan.ops = &ops;
		*chan = &l2cap->chan.chan;

		return 0;
	}

	BT_ERR("No available L2CAP context for conn %p", conn);

	return -ENOMEM;
}

BT_L2CAP_CHANNEL_DEFINE(le_fixed_chan, BT_L2CAP_CID_LE_SIG, l2cap_accept, NULL);

static void ethermind_l2cap_le_callback
     (
         DEVICE_HANDLE    * handle,
         UCHAR              event_code,
         UCHAR            * data,
         UINT16             packet_len
     )
{
    UINT16 length;
    UINT16 min_interval;
    UINT16 max_interval;
    UINT16 slave_latency;
    UINT16 supervision_timeout;
    UINT16 result;
    UINT16 connection_handle;
    BT_DEVICE_ADDR peer_bd_addr;
    UCHAR   * bd_addr;
    UCHAR     bd_addr_type;
    API_RESULT retval;
    UINT16 reason;

    retval = device_queue_get_remote_addr (handle,&peer_bd_addr);

    if (API_SUCCESS != retval)
    {
        BT_ERR("Received LE-L2CAP Callback. No device found\n");
        return;
    }

    bd_addr = BT_BD_ADDR(&peer_bd_addr);
    bd_addr_type = BT_BD_ADDR_TYPE (&peer_bd_addr);

    (void)bd_addr;
    (void)bd_addr_type;

    /*
     * This event is sent to application from Stack only when the
     * Local Device is Master. Application doesn't have to check
     * the Local device role again.
     */
    if (L2CAP_CONNECTION_UPDATE_REQUEST_EVENT == event_code)
    {
        /* Default - Set Connection Parameter Rejected as Result */
        result = L2CAP_CONNECTION_PARAMETERS_REJECTED;

        /* Data Packet Length Validation */
        if (0 != packet_len)
        {
            /* Extract L2CAP packet Length */
            BT_UNPACK_LE_2_BYTE(&length, &data[0]);

            /**
             *  Connection Parameter Update Request shall have
             *  8 octet of data fields
             */
            if (8 == length)
            {
                BT_UNPACK_LE_2_BYTE(&min_interval, &data[2]);
                BT_UNPACK_LE_2_BYTE(&max_interval, &data[4]);
                BT_UNPACK_LE_2_BYTE(&slave_latency, &data[6]);
                BT_UNPACK_LE_2_BYTE(&supervision_timeout, &data[8]);

                BT_DBG("L2CAP Connection Update Request\n");
                BT_DBG("Received : L2CAP_CONNECTION_UPDATE\n");
                BT_DBG("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
                bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
                BT_DBG("\tBD_ADDR_TYPE : %02X\n", bd_addr_type);
                BT_DBG("\tLength : 0x%04X\n", length);
                BT_DBG("\tMinimum Interval : 0x%04X\n", min_interval);
                BT_DBG("\tMaximum Interval : 0x%04X\n", max_interval);
                BT_DBG("\tSlave Latency : 0x%04X\n", slave_latency);
                BT_DBG("\tSupervision Timeout : 0x%04X\n", supervision_timeout);

                /* Verify received parameters */
                if ((min_interval < HCI_LE_MIN_CONN_INTRVL_MIN_RANGE) ||
                    (min_interval > HCI_LE_MIN_CONN_INTRVL_MAX_RANGE) ||
                    (max_interval < HCI_LE_MAX_CONN_INTRVL_MIN_RANGE) ||
                    (max_interval > HCI_LE_MAX_CONN_INTRVL_MAX_RANGE) ||
                    (min_interval > max_interval) ||
                    /* (slave_latency < HCI_LE_CONN_LATENCY_MIN_RANGE) || */ /* fix for warning: pointless comparison */
                    (slave_latency > HCI_LE_CONN_LATENCY_MAX_RANGE) ||
                    (supervision_timeout < HCI_LE_SUPERVISION_TO_MIN_RANGE) ||
                    (supervision_timeout > HCI_LE_SUPERVISION_TO_MAX_RANGE))
                {
                    BT_ERR("Invalid Parameter\n");
                }
                else
                {
                    result = L2CAP_CONNECTION_PARAMETERS_ACCEPTED;
                }
            }
        }

        /* Send Response */
        retval = BT_l2cap_le_connection_param_update_response
                 (
                     handle,
                     result
                 );

        if (L2CAP_CONNECTION_PARAMETERS_ACCEPTED == result)
        {
            /* Get LE Connection Handle */
            retval = BT_hci_get_le_connection_handle (&peer_bd_addr, &connection_handle);

            if (API_SUCCESS == retval)
            {
                BT_hci_le_connection_update
                (
                     connection_handle,
                     min_interval,
                     max_interval,
                     slave_latency,
                     supervision_timeout,
                     0x0000, /* minimum_ce_length, */
                     0xFFFF  /* maximum_ce_length */
                );
            }
        }
    }
    else if (L2CAP_CONNECTION_UPDATE_RESPONSE_EVENT == event_code)
    {
        BT_UNPACK_LE_2_BYTE(&length, &data[0]);
        BT_UNPACK_LE_2_BYTE(&result, &data[2]);

        BT_DBG("L2CAP Connection Update Response. Result: 0x%04X\n",
            result);
        BT_DBG("Received : L2CAP_CONNECTION_UPDATE\n");
        BT_DBG("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
        BT_DBG("\tbd_addr_type : %02X\n", bd_addr_type);
        BT_DBG("\tLength       : %04X\n", length);
        BT_DBG("\tResult       : %04X\n", result);
    }
    else if (L2CAP_COMMAND_REJECTED_EVENT == event_code)
    {
        BT_UNPACK_LE_2_BYTE(&reason, &data[0]);

        BT_DBG("L2CAP Command Rejected. Reason: 0x%04X\n", reason);
        BT_DBG("Received : L2CAP_COMMAND_REJ\n");
        BT_DBG("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
        BT_DBG("\tbd_addr_type : %02X\n", bd_addr_type);
        BT_DBG("\tReason       : %04X\n", reason);
    }
    else
    {
        BT_DBG("Received Invalid Event. Event = 0x%02X\n", event_code);
        BT_DBG("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
        BT_DBG("\tbd_addr_type : %02X\n", bd_addr_type);
    }
}

void bt_l2cap_init(void)
{
    API_RESULT retval;

	if (IS_ENABLED(CONFIG_BT_BREDR)) {
		bt_l2cap_br_init();
	}

    retval = BT_l2cap_register_le_event_cb(ethermind_l2cap_le_callback);

    if (retval != API_SUCCESS)
    {
        BT_ERR("ERROR!!! retval = 0x%04X", retval);
    }

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && (CONFIG_BT_L2CAP_DYNAMIC_CHANNEL > 0U))
    if (NULL == free_l2cap_tx_meta_data)
    {
        osa_status_t ret = OSA_MsgQCreate((osa_msgq_handle_t)free_l2cap_tx_meta_data_handle, CONFIG_BT_CONN_TX_MAX, sizeof(void*));
        assert(KOSA_StatusSuccess == ret);
        if (KOSA_StatusSuccess != ret)
        {
            return;
        }
        free_l2cap_tx_meta_data = (osa_msgq_handle_t)free_l2cap_tx_meta_data_handle;
    }
	for (size_t i = 0; i < ARRAY_SIZE(l2cap_tx_meta_data_storage); i++) {
                void *msg = (void *)&l2cap_tx_meta_data_storage[i];
		OSA_MsgQPut(free_l2cap_tx_meta_data, (void*)&msg);
	}
#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */
}

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
static int l2cap_le_connect(struct bt_conn *conn, struct bt_l2cap_le_chan *ch,
			    uint16_t psm)
{
	int err;

	if (psm < L2CAP_LE_PSM_FIXED_START || psm > L2CAP_LE_PSM_DYN_END) {
		return -EINVAL;
	}

	l2cap_chan_tx_init(ch);
	l2cap_chan_rx_init(ch);

	if (!l2cap_chan_add(conn, &ch->chan, l2cap_chan_destroy)) {
		return -ENOMEM;
	}

	ch->psm = psm;

#if ((defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U)) || (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U)))
	if (conn->sec_level < ch->required_sec_level) {
		err = bt_conn_set_security(conn, ch->required_sec_level);
		if (err) {
			goto fail;
		}

		atomic_set_bit(ch->chan.status,
			       BT_L2CAP_STATUS_ENCRYPT_PENDING);

		return 0;
	}
#endif

	err = l2cap_le_conn_req(ch);
	if (err) {
		goto fail;
	}

	return 0;

fail:
	bt_l2cap_chan_remove(conn, &ch->chan);
	bt_l2cap_chan_del(&ch->chan);
	return err;
}

#if (defined(CONFIG_BT_L2CAP_ECRED) && ((CONFIG_BT_L2CAP_ECRED) > 0))
static int l2cap_ecred_init(struct bt_conn *conn,
			       struct bt_l2cap_le_chan *ch, uint16_t psm)
{

	if (psm < L2CAP_LE_PSM_FIXED_START || psm > L2CAP_LE_PSM_DYN_END) {
		return -EINVAL;
	}

	l2cap_chan_tx_init(ch);
	l2cap_chan_rx_init(ch);

	if (!l2cap_chan_add(conn, &ch->chan, l2cap_chan_destroy)) {
		return -ENOMEM;
	}

	ch->psm = psm;
	ch->psm = psm;

	BT_DBG("ch %p psm 0x%02x mtu %u mps %u credits %u", ch, ch->psm,
		ch->rx.mtu, ch->rx.mps, ch->rx.init_credits);

	return 0;
}

int bt_l2cap_ecred_chan_connect(struct bt_conn *conn,
				struct bt_l2cap_chan **chans, uint16_t psm)
{
	int i, err;

	BT_DBG("conn %p chans %p psm 0x%04x", conn, chans, psm);

	if (!conn || !chans) {
		return -EINVAL;
	}

	/* Init non-null channels */
	for (i = 0; i < L2CAP_ECRED_CHAN_MAX_PER_REQ; i++) {
		if (!chans[i]) {
			break;
		}

		err = l2cap_ecred_init(conn, BT_L2CAP_LE_CHAN(chans[i]), psm);
		if (err < 0) {
			i--;
			goto fail;
		}
	}

	return l2cap_ecred_conn_req(chans, i);
fail:
	/* Remove channels added */
	for (; i >= 0; i--) {
		if (!chans[i]) {
			continue;
		}

		bt_l2cap_chan_remove(conn, chans[i]);
	}

	return err;
}

static struct bt_l2cap_le_chan *l2cap_find_pending_reconf(struct bt_conn *conn)
{
	struct bt_l2cap_chan *chan;

	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		if (BT_L2CAP_LE_CHAN(chan)->pending_rx_mtu) {
			return BT_L2CAP_LE_CHAN(chan);
		}
	}

	return NULL;
}

int bt_l2cap_ecred_chan_reconfigure(struct bt_l2cap_chan **chans, uint16_t mtu)
{
	struct bt_l2cap_ecred_reconf_req *req;
	struct bt_conn *conn = NULL;
	struct bt_l2cap_le_chan *ch;
	struct net_buf *buf;
	uint8_t ident;
	int i;

	BT_DBG("chans %p mtu 0x%04x", chans, mtu);

	if (!chans) {
		return -EINVAL;
	}

	for (i = 0; i < L2CAP_ECRED_CHAN_MAX_PER_REQ; i++) {
		if (!chans[i]) {
			break;
		}

		/* validate that all channels are from same connection */
		if (conn) {
			if (conn != chans[i]->conn) {
				return -EINVAL;
			}
		} else {
			conn = chans[i]->conn;
		}

		/* validate MTU is not decreased */
		if (mtu < BT_L2CAP_LE_CHAN(chans[i])->rx.mtu) {
			return -EINVAL;
		}
	}

	if (i == 0) {
		return -EINVAL;
	}

	if (!conn) {
		return -ENOTCONN;
	}

	if (conn->type != BT_CONN_TYPE_LE) {
		return -EINVAL;
	}

	/* allow only 1 request at time */
	if (l2cap_find_pending_reconf(conn)) {
		return -EBUSY;
	}

	ident = get_ident();

#if 1
	API_RESULT result = API_SUCCESS;
	L2CAP_ECBFC_RECONFIG_PARAM param;

	param.mtu = mtu;
	param.mps = MIN(mtu + BT_L2CAP_SDU_HDR_SIZE, BT_L2CAP_RX_MTU);
	param.num_cids = i;
	for(int j = 0; j < i; j++)
	{
		ch = BT_L2CAP_LE_CHAN(chans[j]);
		ch->ident = ident;
		ch->pending_rx_mtu = mtu;
		param.cid[j] = ch->rx.cid;
	}
	result = l2ca_ecbfc_reconfig_req
           (
               &chans[0]->conn->deviceId,
               &param
           );

	if (API_SUCCESS != result)
	{
		return -EINVAL;
	}

	return 0;
#else


	buf = l2cap_create_le_sig_pdu(NULL, BT_L2CAP_ECRED_RECONF_REQ,
				      ident,
				      sizeof(*req) + (i * sizeof(uint16_t)));
	if (!buf) {
		return -ENOMEM;
	}

	req = net_buf_add(buf, sizeof(*req));
	req->mtu = sys_cpu_to_le16(mtu);

	/* MPS shall not be bigger than MTU + BT_L2CAP_SDU_HDR_SIZE
	 * as the remaining bytes cannot be used.
	 */
	req->mps = sys_cpu_to_le16(MIN(mtu + BT_L2CAP_SDU_HDR_SIZE,
				       BT_L2CAP_RX_MTU));

	for (int j = 0; j < i; j++) {
		ch = BT_L2CAP_LE_CHAN(chans[j]);

		ch->ident = ident;
		ch->pending_rx_mtu = mtu;

		net_buf_add_le16(buf, ch->rx.cid);
	};

	/* We set the RTX timer on one of the supplied channels, but when the
	 * request resolves or times out we will act on all the channels in the
	 * supplied array, using the ident field to find them.
	 */
	l2cap_chan_send_req(chans[0], buf, L2CAP_CONN_TIMEOUT);

	return 0;

#endif
}

#endif /* defined(CONFIG_BT_L2CAP_ECRED) */

int bt_l2cap_chan_connect(struct bt_conn *conn, struct bt_l2cap_chan *chan,
			  uint16_t psm)
{
	struct bt_l2cap_le_chan *le_chan = BT_L2CAP_LE_CHAN(chan);

	BT_DBG("conn %p chan %p psm 0x%04x", conn, chan, psm);

	if (!conn || conn->state != BT_CONN_CONNECTED) {
		return -ENOTCONN;
	}

	if (!chan) {
		return -EINVAL;
	}

	if (IS_ENABLED(CONFIG_BT_BREDR) &&
	    conn->type == BT_CONN_TYPE_BR) {
		return bt_l2cap_br_chan_connect(conn, chan, psm);
	}

	if (le_chan->required_sec_level > BT_SECURITY_L4) {
		return -EINVAL;
	} else if (le_chan->required_sec_level == BT_SECURITY_L0) {
		le_chan->required_sec_level = BT_SECURITY_L1;
	}

	return l2cap_le_connect(conn, le_chan, psm);
}

int bt_l2cap_chan_disconnect(struct bt_l2cap_chan *chan)
{
	struct bt_conn *conn = chan->conn;
	struct bt_l2cap_le_chan *le_chan;

	if (!conn) {
		return -ENOTCONN;
	}

	if (IS_ENABLED(CONFIG_BT_BREDR) &&
	    conn->type == BT_CONN_TYPE_BR) {
		return bt_l2cap_br_chan_disconnect(chan);
	}

	le_chan = BT_L2CAP_LE_CHAN(chan);

	BT_DBG("chan %p scid 0x%04x dcid 0x%04x", chan, le_chan->rx.cid,
	       le_chan->tx.cid);

	le_chan->ident = get_ident();

	l2ca_cbfc_disconnect_req(le_chan->rx.cid);
	bt_l2cap_chan_set_state(chan, BT_L2CAP_DISCONNECTING);

	return 0;
}

int bt_l2cap_chan_send_cb(struct bt_l2cap_chan *chan, struct net_buf *buf, bt_conn_tx_cb_t cb,
			  void *user_data)
{
    struct l2cap_tx_meta_data *data;
    struct bt_l2cap_le_chan *le_chan = BT_L2CAP_LE_CHAN(chan);

	data = alloc_tx_meta_data();
	if (!data) {
		BT_WARN("Unable to allocate TX context");
		return -ENOBUFS;
	}

	struct bt_l2cap_le_chan *ch = BT_L2CAP_LE_CHAN(chan);

	if (!buf) {
		return -EINVAL;
	}

	BT_DBG("chan %p buf %p len %lu", chan, buf, net_buf_frags_len(buf));

	if (!chan->conn || chan->conn->state != BT_CONN_CONNECTED) {
		return -ENOTCONN;
	}

	if (atomic_test_bit(chan->status, BT_L2CAP_STATUS_SHUTDOWN)) {
		return -ESHUTDOWN;
	}

	if (IS_ENABLED(CONFIG_BT_BREDR) &&
	    chan->conn->type == BT_CONN_TYPE_BR) {
		return bt_l2cap_br_chan_send(chan, buf);
	}

#ifndef L2CAP_ALLOC_FOR_CBFC_SDU
	net_buf_push_le16(buf, buf->len);
#endif

	data->sent = 0;
	data->cid = le_chan->tx.cid;
	data->cb = cb;
	data->user_data = user_data;
	l2cap_tx_meta_data_macro(buf) = data;
    data_sent(buf)->len = 0;
    net_buf_put(ch->tx_queue.queue, buf);
#if 1
    k_work_submit(&ch->tx_work);
#endif
    return 0;
}
#if 0
int bt_l2cap_chan_send(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
	struct bt_l2cap_le_chan *le_chan = BT_L2CAP_LE_CHAN(chan);
	struct l2cap_tx_meta_data *data;

	if (!buf) {
		return -EINVAL;
	}

	BT_DBG("chan %p buf %p len %lu", chan, buf, net_buf_frags_len(buf));

	if (!chan->conn || chan->conn->state != BT_CONN_CONNECTED) {
		return -ENOTCONN;
	}

	if (atomic_test_bit(chan->status, BT_L2CAP_STATUS_SHUTDOWN)) {
		return -ESHUTDOWN;
	}

	if (IS_ENABLED(CONFIG_BT_BREDR) &&
	    chan->conn->type == BT_CONN_TYPE_BR) {
		return bt_l2cap_br_chan_send_cb(chan, buf, cb, user_data);
	}

#ifndef L2CAP_ALLOC_FOR_CBFC_SDU
	net_buf_push_le16(buf, buf->len);
#endif

	/* Queue if there are pending segments left from previous packet or
	 * there are no credits available.
	 */
	 l2cap_tx_meta_data_macro(buf) = NULL;
    data_sent(buf)->len = 0;
		net_buf_put(&le_chan->tx_queue, buf);
#if 1
		k_work_submit(&le_chan->tx_work);
#endif
    return 0;
}
#endif

int bt_l2cap_chan_send(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
	return bt_l2cap_chan_send_cb(chan, buf, NULL, NULL);
}

#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */

#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && ((CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) > 0U))
#ifdef L2CAP_SUPPORT_CBFC_MODE
API_RESULT ethermind_l2ca_connect_ind_cb
           (
               DEVICE_HANDLE        * handle,
               UINT16                     lcid,
               UINT16                     psm,
               L2CAP_CBFC_CONNECT_PARAM * param
           )
{
	struct bt_l2cap_chan *chan;
	struct bt_l2cap_le_chan *ch;
	struct bt_l2cap_server *server;
    struct bt_conn *conn;
	uint16_t mtu, mps, credits;
	uint16_t result;
	uint16_t security_result;
    L2CAP_CBFC_CONNECT_PARAM connect_param;
    API_RESULT retval;
    UINT16 response = L2CAP_CONNECTION_SUCCESSFUL;

	conn = bt_conn_lookup_device_id(*handle);
	assert(NULL != conn);
	if (NULL == conn)
	{
		return API_SUCCESS;
	}
    bt_conn_unref(conn);

	psm = sys_le16_to_cpu(psm);
	mtu = sys_le16_to_cpu(param->mtu);
	mps = sys_le16_to_cpu(param->mps);
	credits = sys_le16_to_cpu(param->credit);

	BT_DBG("psm 0x%02x mtu %u mps %u credits %u", psm,
	       mtu, mps, credits);

	if (mtu < L2CAP_LE_MIN_MTU || mps < L2CAP_LE_MIN_MTU)
	{
		BT_ERR("Invalid LE-Conn Req params");
		response = L2CAP_CONNECTION_REFUSED_UNACCEPTABLE_PARAMETERS;
	}

	if (L2CAP_CONNECTION_SUCCESSFUL == response)
	{
		/* Check if there is a server registered */
		server = l2cap_server_lookup_psm(psm);
		if (NULL == server)
		{
			response = L2CAP_CONNECTION_REFUSED_PSM_UNSUPPORTED;
		}
	}

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0u))
	if (L2CAP_CONNECTION_SUCCESSFUL == response)
	{
		/* Check if connection has minimum required security level */
		security_result = l2cap_check_security(conn, server);
		if (BT_L2CAP_LE_ERR_ENCRYPTION == security_result)
		{
			response = L2CAP_CONNECTION_REFUSED_INSUFFICIENT_ENCRYPTION;
		}
		else if (BT_L2CAP_LE_ERR_AUTHENTICATION == security_result)
		{
			response = L2CAP_CONNECTION_REFUSED_AUTHENTICATION_INSUFFICIENT;
		}
	}
#endif

	if (L2CAP_CONNECTION_SUCCESSFUL == response)
	{
		result = l2cap_chan_accept(conn, server, lcid, mtu, mps, credits,
					&chan);
		if (result != BT_L2CAP_LE_SUCCESS) {
			response = sys_cpu_to_le16(result);
		}
	}

	if (L2CAP_CONNECTION_SUCCESSFUL == response)
	{
		ch = BT_L2CAP_LE_CHAN(chan);

        ch->tx.credits = credits;
        ch->tx.init_credits = credits;
        ch->tx.mps = mps;
        ch->tx.mtu = mtu;

		/* Prepare response protocol data */
		ch->rx.cid = sys_le16_to_cpu(lcid);
		connect_param.mps = sys_cpu_to_le16(ch->rx.mps);
		connect_param.mtu = sys_cpu_to_le16(ch->rx.mtu);
		connect_param.credit = sys_cpu_to_le16(ch->rx.init_credits);
	}

    retval = l2ca_cbfc_connect_rsp
                (
                    lcid,
                    response,
                    &connect_param
                );
    return retval;
}

API_RESULT ethermind_l2ca_connect_cnf_cb
           (
               DEVICE_HANDLE        * handle,
               UINT16                     lcid,
               UINT16                     response,
               L2CAP_CBFC_CONNECT_PARAM * param
           )
{
	struct bt_l2cap_le_chan *chan;
	struct bt_l2cap_chan *l2capChan;
	struct bt_l2cap_le_chan *le_chan;
        struct bt_conn *conn;
	uint16_t mtu, mps, credits;

	conn = bt_conn_lookup_device_id(*handle);
	assert(NULL != conn);
	if (NULL == conn)
	{
		return API_SUCCESS;
	}

	/* Looking for the channel of the request */
	l2capChan = bt_l2cap_le_lookup_rx_cid(conn, sys_le16_to_cpu(lcid));
	if (NULL == l2capChan)
	{
		bt_conn_unref(conn);
		return API_SUCCESS;
	}
        
        le_chan = BT_L2CAP_LE_CHAN(l2capChan);

	mtu = sys_le16_to_cpu(param->mtu);
	mps = sys_le16_to_cpu(param->mps);
	credits = sys_le16_to_cpu(param->credit);

	BT_DBG("lcid 0x%04x mtu %u mps %u credits %u result 0x%04x", sys_le16_to_cpu(lcid),
	       mtu, mps, credits, response);

	/* Keep the channel in case of security errors */
	if (response == BT_L2CAP_LE_SUCCESS ||
	    response == BT_L2CAP_LE_ERR_AUTHENTICATION ||
	    response == BT_L2CAP_LE_ERR_ENCRYPTION) {
		chan = l2cap_lookup_ident(conn, le_chan->ident);
	} else {
		chan = l2cap_remove_ident(conn, le_chan->ident);
	}

	if (!chan) {
		BT_ERR("Cannot find channel for ident %u", le_chan->ident);
		bt_conn_unref(conn);
		return API_SUCCESS;
	}

	/* Cancel RTX work */
#if 0
	k_work_cancel_delayable(&chan->chan.rtx_work);
#endif

	/* Reset ident since it got a response */
	chan->ident = 0U;

	switch (response) {
	case BT_L2CAP_LE_SUCCESS:
#if 0
		chan->tx.cid = dcid;
#endif
		chan->rx.cid = sys_le16_to_cpu(lcid);
		chan->tx.mtu = mtu;
		chan->tx.mps = mps;

		/* Update state */
		bt_l2cap_chan_set_state(&chan->chan, BT_L2CAP_CONNECTED);

		if (chan->chan.ops->connected) {
			chan->chan.ops->connected(&chan->chan);
		}

		/* Give credits */
		l2cap_chan_tx_give_credits(chan, credits);
		l2cap_chan_rx_give_credits(chan, chan->rx.init_credits);

		break;
	case BT_L2CAP_LE_ERR_AUTHENTICATION:
	case BT_L2CAP_LE_ERR_ENCRYPTION:
		/* If security needs changing wait it to be completed */
		if (l2cap_change_security(chan, response) == 0) {
			bt_conn_unref(conn);
			return API_SUCCESS;
		}
		bt_l2cap_chan_remove(conn, &chan->chan);
		__fallthrough;
	default:
		bt_l2cap_chan_del(&chan->chan);
	}
	bt_conn_unref(conn);
    return API_SUCCESS;
}

typedef struct bt_l2cap_conn_scan_param
{
	uint16_t lcid;
	struct bt_l2cap_le_chan *chan;
} bt_l2cap_conn_scan_param_t;

static bool ethermind_bt_conn_scan_cb(struct bt_conn *conn, void *param)
{
	bt_l2cap_conn_scan_param_t *conn_scan = (bt_l2cap_conn_scan_param_t *)param;
    struct bt_l2cap_chan *chan;

	chan = bt_l2cap_le_lookup_rx_cid(conn, conn_scan->lcid);
	if (NULL != chan)
	{
        conn_scan->chan = BT_L2CAP_LE_CHAN(chan);
		return true;
	}
	else
	{
		return false;
	}
}

API_RESULT ethermind_l2ca_disconnect_ind_cb(UINT16 lcid)
{
	struct bt_l2cap_le_chan *chan;
	bt_l2cap_conn_scan_param_t conn_scan;
	uint16_t dcid;

	dcid = sys_le16_to_cpu(lcid);
	conn_scan.lcid = dcid;
	conn_scan.chan = NULL;

	BT_DBG("dcid 0x%04x", dcid);

	(void)bt_conn_scan(ethermind_bt_conn_scan_cb, &conn_scan);
	chan = conn_scan.chan;
	if (NULL != chan)
	{
		bt_l2cap_chan_del(&chan->chan);
	}

	return API_SUCCESS;
}

API_RESULT ethermind_l2ca_disconnect_cnf_cb(UINT16 lcid, UINT16 reason)
{
	BT_IGNORE_UNUSED_PARAM(reason);

	return ethermind_l2ca_disconnect_ind_cb(lcid);
}

API_RESULT ethermind_l2ca_data_read_cb (UINT16 lcid, UINT16 result, UCHAR * data, UINT16 datalen)
{
	struct bt_l2cap_le_chan *chan;
	bt_l2cap_conn_scan_param_t conn_scan;
	struct bt_conn *conn;
	struct net_buf *buf;
	struct bt_l2cap_hdr hdr;
	uint16_t dcid;
	API_RESULT retval = API_SUCCESS;

	dcid = sys_le16_to_cpu(lcid);
	conn_scan.lcid = dcid;
	conn_scan.chan = NULL;

	BT_DBG("dcid 0x%04x", dcid);

	(void)bt_conn_scan(ethermind_bt_conn_scan_cb, &conn_scan);
	chan = conn_scan.chan;

	if (NULL != chan)
	{
		conn = chan->chan.conn;
		if (NULL != chan->chan.ops->alloc_buf)
		{
			buf = chan->chan.ops->alloc_buf(&chan->chan);
		}
		else
		{
			buf = bt_buf_get_rx(BT_BUF_ACL_IN, osaWaitNone_c);
		}
		if (NULL != buf)
		{
			hdr.len = datalen;
			hdr.cid = dcid;
			(void)net_buf_add_mem(buf, &hdr, sizeof(hdr));
			(void)net_buf_add_mem(buf, data, datalen);
			bt_l2cap_recv(conn, buf, true);
		}
		else
		{
			retval = API_FAILURE;
			bt_l2cap_chan_disconnect(&chan->chan);
		}
	}
	else
	{
		retval = API_FAILURE;
		l2ca_cbfc_disconnect_req(lcid);
	}

	return retval;
}

API_RESULT ethermind_l2ca_low_rx_credit_ind_cb (UINT16 lcid, UINT16 credit)
{
	struct bt_l2cap_le_chan *chan;
	bt_l2cap_conn_scan_param_t conn_scan;

	conn_scan.lcid = sys_le16_to_cpu(lcid);
	conn_scan.chan = NULL;

	BT_DBG("dcid 0x%04x credit %d", conn_scan.lcid, credit);

	(void)bt_conn_scan(ethermind_bt_conn_scan_cb, &conn_scan);

	chan = conn_scan.chan;

	if (NULL != chan)
	{
#if 0
        (void)l2ca_cbfc_send_flow_control_credit
                 (
                     lcid,
                     chan->rx.init_credits - credit
                 );
#endif
		atomic_set(&chan->rx.credits, credit);
	}
	else
	{
		l2ca_cbfc_disconnect_req(lcid);
	}


	return API_SUCCESS;
}

API_RESULT ethermind_l2ca_tx_credit_ind_cb (UINT16 lcid, UINT16 result, UINT16 credit)

{
	struct bt_l2cap_le_chan *chan;
	bt_l2cap_conn_scan_param_t conn_scan;
	struct bt_conn *conn;
	uint16_t dcid;
	uint16_t credits;
	API_RESULT retval = API_SUCCESS;

	dcid = sys_le16_to_cpu(lcid);
	conn_scan.lcid = dcid;
	conn_scan.chan = NULL;

	BT_DBG("dcid 0x%04x credit %d", dcid, credit);

	/**
	 * Disconnect channel if credit overflow has occurred. This is in accordance
	 * text in section 10.1 of the Core 5.0 specification
	 */
	if (L2CAP_CREDIT_OVERFLOW == result)
	{
		l2ca_cbfc_disconnect_req(lcid);
		return retval;
	}

	(void)bt_conn_scan(ethermind_bt_conn_scan_cb, &conn_scan);
	chan = conn_scan.chan;

	if (NULL != chan)
	{
		credits = sys_le16_to_cpu(credit);
		conn = chan->chan.conn;
		atomic_set(&chan->tx.credits, 0);
		if ((atomic_get(&chan->tx.credits) + credits) > UINT16_MAX) {
			BT_ERR("Credits overflow");
			bt_l2cap_chan_disconnect(&chan->chan);
			return retval;
		}

		l2cap_chan_tx_give_credits(chan, credits);

		BT_DBG("chan %p total credits %u", chan, atomic_get(&chan->tx.credits));

		l2cap_chan_tx_resume(chan);
	}
	else
	{
		retval = API_FAILURE;
		l2ca_cbfc_disconnect_req(lcid);
	}

	(void)conn;

	return retval;
}

API_RESULT ethermind_l2ca_data_write_cb(UINT16 lcid, UINT16 result, UCHAR *buffer, UINT16 buffer_len)
{
 	struct bt_l2cap_le_chan *chan;
	bt_l2cap_conn_scan_param_t conn_scan;
	struct l2cap_tx_meta_data *user_data = NULL;
	int err = 0;

	conn_scan.lcid = sys_le16_to_cpu(lcid);
	conn_scan.chan = NULL;

	BT_DBG("dcid 0x%04x", conn_scan.lcid);

	(void)bt_conn_scan(ethermind_bt_conn_scan_cb, &conn_scan);

	chan = conn_scan.chan;

	if (API_SUCCESS != result)
	{
		err = -1;
	}

	if (NULL != chan)
	{
		struct l2cap_tx_meta_data user_data_temp;
		memset(&user_data_temp, 0, sizeof(user_data_temp));
		user_data_temp.user_data = (void*)0xFFFFFFFFu;
		user_data_temp.cid = chan->tx.cid;
		if (NULL != chan->_sdu)
		{
			if (l2cap_tx_meta_data_macro(chan->_sdu) != NULL)
			{
				user_data = l2cap_tx_meta_data_macro(chan->_sdu);
			}
			else
			{
				user_data = &user_data_temp;
			}
			net_buf_unref(chan->_sdu);
			chan->_sdu = NULL;
			chan->_sdu_len = 0;
		}
		l2cap_chan_sdu_sent(chan->chan.conn, user_data, err);
	}
	else
	{
		l2ca_cbfc_disconnect_req(lcid);
	}

	return API_SUCCESS;
}
#endif /* L2CAP_SUPPORT_CBFC_MODE */

#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED > 0U))
struct ethermind_ecbfc_edgefast_state
{
    struct bt_conn *conn;
    struct bt_l2cap_chan *l2cap_att_sig_chan;
    struct net_buf *buf;
};

static struct bt_l2cap_chan *ethermind_find_le_sig_chan(struct bt_conn *conn)
{
    for (int i = 0; i < ARRAY_SIZE(bt_l2cap_pool); i++)
    {
        struct bt_l2cap *l2cap = &bt_l2cap_pool[i];

        if ((l2cap->chan.chan.conn) && (l2cap->chan.chan.conn == conn))
        {
             if ((l2cap->chan.rx.cid == BT_L2CAP_CID_LE_SIG) && (l2cap->chan.tx.cid == BT_L2CAP_CID_LE_SIG))
             {
                 return &l2cap->chan.chan;
             }
        }
    }
    
    return NULL;
}

static API_RESULT ethermind_ecbfc_get_edgefast_state(DEVICE_HANDLE *handle, struct ethermind_ecbfc_edgefast_state *state)
{
    state->conn = bt_conn_lookup_device_id(*handle);
    assert(NULL != state->conn);
    if (NULL == state->conn)
    {
        return API_FAILURE;
    }
    state->l2cap_att_sig_chan = ethermind_find_le_sig_chan(state->conn);
    assert(NULL != state->l2cap_att_sig_chan);
    if (NULL == state->l2cap_att_sig_chan)
    {
        return API_FAILURE;
    }

    if (NULL != state->l2cap_att_sig_chan->ops->alloc_buf)
    {
        state->buf = state->l2cap_att_sig_chan->ops->alloc_buf(state->l2cap_att_sig_chan);
    }
    else
    {
        state->buf = bt_buf_get_rx(BT_BUF_ACL_IN, osaWaitNone_c);
    }

    if (state->buf == NULL)
    {
        return API_FAILURE;
    }

    return API_SUCCESS;
}

static API_RESULT ethermind_ecbfc_get_edgefast_state_by_lcid(UINT16 lcid, struct ethermind_ecbfc_edgefast_state *state)
{
    struct bt_l2cap_chan *l2cap = NULL;

    for (int i = 0; i < ARRAY_SIZE(bt_l2cap_pool); i++)
    {
        if (bt_l2cap_pool[i].chan.chan.conn)
        {
             if (bt_l2cap_pool[i].chan.rx.cid == lcid)
             {
                 l2cap = &bt_l2cap_pool[i].chan.chan;
                 state->conn = l2cap->conn;
             }
        }
    }
    
    if (l2cap == NULL)
    {
        return API_FAILURE;
    }

    assert(NULL != state->conn);
    if (NULL == state->conn)
    {
        return API_FAILURE;
    }
    state->l2cap_att_sig_chan = l2cap;
    assert(NULL != state->l2cap_att_sig_chan);
    if (NULL == state->l2cap_att_sig_chan)
    {
        return API_FAILURE;
    }

    if (NULL != state->l2cap_att_sig_chan->ops->alloc_buf)
    {
        state->buf = state->l2cap_att_sig_chan->ops->alloc_buf(state->l2cap_att_sig_chan);
    }
    else
    {
        state->buf = bt_buf_get_rx(BT_BUF_ACL_IN, osaWaitNone_c);
    }

    if (state->buf == NULL)
    {
        return API_FAILURE;
    }

    return API_SUCCESS;
}

static API_RESULT ethermind_l2ca_ecbfc_connect_ind_cb
           (
               DEVICE_HANDLE             * handle,
               UINT16                    * lcid,
               UINT16                      psm,
               L2CAP_ECBFC_CONNECT_PARAM * param
           )
{
    struct bt_l2cap_chan *l2cap_att_sig_chan;
    struct net_buf *buf;
    API_RESULT retval = API_FAILURE;
    struct ethermind_ecbfc_edgefast_state state;
    struct bt_l2cap_sig_hdr hdr;
    struct bt_l2cap_ecred_conn_req req;
    uint32_t len_temp;

    retval = ethermind_ecbfc_get_edgefast_state(handle, &state);
    if (retval != API_SUCCESS)
    {
        return API_FAILURE;
    }

    buf = state.buf;
    l2cap_att_sig_chan = state.l2cap_att_sig_chan;
    /* re-struct as buf */
    hdr.code = BT_L2CAP_ECRED_CONN_REQ;
    hdr.ident = 1; /* don't care, ethermind maintain it */
    hdr.len = sizeof(req) + param->num_cids * 2u;
    (void)net_buf_add_mem(buf, &hdr, sizeof(hdr));

    req.psm = sys_cpu_to_le16(psm);
    req.mtu = sys_cpu_to_le16(param->mtu);
    req.mps = sys_cpu_to_le16(param->mps);
    req.credits = sys_cpu_to_le16(param->credit);
    (void)net_buf_add_mem(buf, &req, sizeof(req));
    for (uint8_t index = 0; ((index < param->num_cids) && (index < L2CAP_ECBFC_MAX_NUM_CIDS)); ++index)
    {
        net_buf_add_le16(buf, param->cid[index]);
    }

    /* put the lcid here */
    len_temp = 0;
    for (uint8_t index = 0; ((index < param->num_cids) && (index < L2CAP_ECBFC_MAX_NUM_CIDS)); ++index)
    {
        net_buf_add_le16(buf, lcid[index]);
        len_temp += 2u;
    }
    buf->len -= len_temp;

    //le_ecred_conn_req(l2cap, hdr->ident, buf);
    if (!l2cap_recv(l2cap_att_sig_chan, buf))
    {
        retval = API_SUCCESS;
    }
    net_buf_unref(buf);

    return retval;
}

static API_RESULT ethermind_l2ca_ecbfc_connect_cnf_cb
           (
               DEVICE_HANDLE             * handle,
               UINT16                    * lcid,
               UINT16                      response,
               L2CAP_ECBFC_CONNECT_PARAM * param
           )
{
    struct bt_conn *conn;
    struct bt_l2cap_chan *l2cap_att_sig_chan;
    struct net_buf *buf;
    API_RESULT retval = API_FAILURE;
    struct bt_l2cap_sig_hdr hdr;
    struct bt_l2cap_ecred_conn_rsp rsp;
    
    struct ethermind_ecbfc_edgefast_state state;
    
    retval = ethermind_ecbfc_get_edgefast_state(handle, &state);
    if (retval != API_SUCCESS)
    {
        return API_FAILURE;
    }

    conn = state.conn;
    buf = state.buf;
    l2cap_att_sig_chan = state.l2cap_att_sig_chan;

    hdr.code = BT_L2CAP_ECRED_CONN_RSP;
    hdr.ident = get_ident(); /* ethermind doesn't provide it */
    /* zephyr use this ident to find all the creating chan. so fake the ident here */
    for (uint8_t index = 0; ((index < param->num_cids) && (index < L2CAP_ECBFC_MAX_NUM_CIDS)); ++index)
    {
        struct bt_l2cap_chan *chan;

        SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan)
        {
            if (BT_L2CAP_LE_CHAN(chan)->rx.cid == lcid[index])
            {
                BT_L2CAP_LE_CHAN(chan)->ident = hdr.ident;
                break;
            }
        }
    }
    hdr.len = sizeof(rsp) + param->num_cids * 2u;
    (void)net_buf_add_mem(buf, &hdr, sizeof(hdr));

    rsp.mtu = sys_cpu_to_le16(param->mtu);
    rsp.mps = sys_cpu_to_le16(param->mps);
    rsp.credits = sys_cpu_to_le16(param->credit);
    rsp.result = sys_cpu_to_le16(0); /* ethermind doesn't give it */
    (void)net_buf_add_mem(buf, &rsp, sizeof(rsp));
    for (uint8_t index = 0; ((index < param->num_cids) && (index < L2CAP_ECBFC_MAX_NUM_CIDS)); ++index)
    {
        net_buf_add_le16(buf, param->cid[index]);
    }
    //le_ecred_conn_rsp(l2cap, hdr->ident, buf);
    if (!l2cap_recv(l2cap_att_sig_chan, buf))
    {
        retval = API_SUCCESS;
    }
    net_buf_unref(buf);

    return retval;
}

static API_RESULT ethermind_l2ca_ecbfc_reconfig_ind_cb
           (
               DEVICE_HANDLE            * handle,
               UINT16                   * lcid,
               L2CAP_ECBFC_RECONFIG_PARAM * param
           )
{
    struct bt_l2cap_chan *l2cap_att_sig_chan;
    struct net_buf *buf;
    API_RESULT retval = API_FAILURE;
    struct bt_l2cap_sig_hdr hdr;
    struct bt_l2cap_ecred_reconf_req req;
    
    struct ethermind_ecbfc_edgefast_state state;
    
    retval = ethermind_ecbfc_get_edgefast_state(handle, &state);
    if (retval != API_SUCCESS)
    {
        return API_FAILURE;
    }

    buf = state.buf;
    l2cap_att_sig_chan = state.l2cap_att_sig_chan;
        
    hdr.code = BT_L2CAP_ECRED_RECONF_REQ;
    hdr.ident = 1; /* don't care, ethermind maintain it */
    hdr.len = sizeof(req) + param->num_cids * 2u;
    (void)net_buf_add_mem(buf, &hdr, sizeof(hdr));

    req.mtu = sys_cpu_to_le16(param->mtu);
    req.mps = sys_cpu_to_le16(param->mps);
    (void)net_buf_add_mem(buf, &req, sizeof(req));
    for (uint8_t index = 0; ((index < param->num_cids) && (index < L2CAP_ECBFC_MAX_NUM_CIDS)); ++index)
    {
        net_buf_add_le16(buf, param->cid[index]);
    }
    //le_ecred_reconf_req(l2cap, hdr->ident, buf);
    if (!l2cap_recv(l2cap_att_sig_chan, buf))
    {
        retval = API_SUCCESS;
    }
    net_buf_unref(buf);

    return retval;
}

static API_RESULT ethermind_l2ca_ecbfc_reconfig_cnf_cb
           (
               DEVICE_HANDLE            * handle
           )
{
    struct bt_l2cap_chan *l2cap_att_sig_chan;
    struct net_buf *buf;
    API_RESULT retval = API_FAILURE;
    struct bt_l2cap_sig_hdr hdr;
    struct bt_l2cap_ecred_reconf_rsp rsp;
	struct bt_conn *conn;
	struct bt_l2cap_chan *chan;
    
    struct ethermind_ecbfc_edgefast_state state;
    retval = ethermind_ecbfc_get_edgefast_state(handle, &state);
    if (retval != API_SUCCESS)
    {
        return API_FAILURE;
    }

	conn = state.conn;
    buf = state.buf;
    l2cap_att_sig_chan = state.l2cap_att_sig_chan;
    
    hdr.code = BT_L2CAP_ECRED_RECONF_RSP;
	/* Find the pending ident for the device. */
	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		if (chan->conn->deviceId == *handle)
		{
			if (BT_L2CAP_LE_CHAN(chan)->pending_rx_mtu)
			{
				hdr.ident = BT_L2CAP_LE_CHAN(chan)->ident;
				break;
			}
		}
	}
    hdr.len = sizeof(rsp);
    (void)net_buf_add_mem(buf, &hdr, sizeof(hdr));

    rsp.result = 0; /* ethermind doesn't callback it */
    (void)net_buf_add_mem(buf, &rsp, sizeof(rsp));
    //le_ecred_reconf_rsp(l2cap, hdr->ident, buf);
    if (!l2cap_recv(l2cap_att_sig_chan, buf))
    {
        retval = API_SUCCESS;
    }
    net_buf_unref(buf);

    return retval;
}

static API_RESULT ethermind_l2ca_ecbfc_disconnect_ind_cb(UINT16 lcid)
{
    struct bt_l2cap_chan *l2cap_att_sig_chan;
    struct net_buf *buf;
    API_RESULT retval = API_FAILURE;
    struct bt_l2cap_sig_hdr hdr;
    struct bt_l2cap_disconn_req req;
    
    struct ethermind_ecbfc_edgefast_state state;
    retval = ethermind_ecbfc_get_edgefast_state_by_lcid(lcid, &state);
    if (retval != API_SUCCESS)
    {
        return API_FAILURE;
    }

    buf = state.buf;
    l2cap_att_sig_chan = state.l2cap_att_sig_chan;
    
    hdr.code = BT_L2CAP_DISCONN_REQ;
    hdr.ident = 1; /* don't care, ethermind maintain it */
    hdr.len = sizeof(req);
    (void)net_buf_add_mem(buf, &hdr, sizeof(hdr));

    req.dcid = sys_cpu_to_le16(lcid);
    req.scid = 0; /* ethermind doesn't give it */
    (void)net_buf_add_mem(buf, &req, sizeof(req));
    //le_disconn_req(l2cap, hdr->ident, buf);
    if (!l2cap_recv(l2cap_att_sig_chan, buf))
    {
        retval = API_SUCCESS;
    }
    net_buf_unref(buf);

    return retval;
}

static API_RESULT ethermind_l2ca_ecbfc_disconnect_cnf_cb(UINT16 lcid, UINT16 reason)
{
    struct bt_l2cap_chan *l2cap_att_sig_chan;
    struct net_buf *buf;
    API_RESULT retval = API_FAILURE;
    struct bt_l2cap_sig_hdr hdr;
    struct bt_l2cap_disconn_rsp rsp;

    struct ethermind_ecbfc_edgefast_state state;
    retval = ethermind_ecbfc_get_edgefast_state_by_lcid(lcid, &state);
    if (retval != API_SUCCESS)
    {
        return API_FAILURE;
    }

    buf = state.buf;
    l2cap_att_sig_chan = state.l2cap_att_sig_chan;
    
    hdr.code = BT_L2CAP_DISCONN_RSP;
    hdr.ident = 1; /* don't care, ethermind maintain it */
    hdr.len = sizeof(rsp);
    (void)net_buf_add_mem(buf, &hdr, sizeof(hdr));

    rsp.dcid = 0; /* ethermind doesn't give it */
    rsp.scid = lcid;
    (void)net_buf_add_mem(buf, &rsp, sizeof(rsp));
    //le_disconn_rsp(l2cap, hdr->ident, buf);
    if (!l2cap_recv(l2cap_att_sig_chan, buf))
    {
        retval = API_SUCCESS;
    }
    net_buf_unref(buf);

    return retval;
}

static API_RESULT ethermind_l2ca_ecbfc_data_read_cb (UINT16 lcid, UINT16 result, UCHAR * data, UINT16 datalen)
{
	struct bt_l2cap_le_chan *chan;
	bt_l2cap_conn_scan_param_t conn_scan;
	struct bt_conn *conn;
	struct net_buf *buf;
	struct bt_l2cap_hdr hdr;
	uint16_t dcid;
	API_RESULT retval = API_SUCCESS;

	dcid = sys_le16_to_cpu(lcid);
	conn_scan.lcid = dcid;
	conn_scan.chan = NULL;

	BT_DBG("dcid 0x%04x", dcid);

	(void)bt_conn_scan(ethermind_bt_conn_scan_cb, &conn_scan);
	chan = conn_scan.chan;

	if (NULL != chan)
	{
		conn = chan->chan.conn;
		if (NULL != chan->chan.ops->alloc_buf)
		{
			buf = chan->chan.ops->alloc_buf(&chan->chan);
		}
		else
		{
			buf = bt_buf_get_rx(BT_BUF_ACL_IN, osaWaitNone_c);
		}
		if (NULL != buf)
		{
			hdr.len = datalen;
			hdr.cid = dcid;
			(void)net_buf_add_mem(buf, &hdr, sizeof(hdr));
			(void)net_buf_add_mem(buf, data, datalen);
			bt_l2cap_recv(conn, buf, true);
		}
		else
		{
			retval = API_FAILURE;
			bt_l2cap_chan_disconnect(&chan->chan);
		}
	}
	else
	{
		retval = API_FAILURE;
		l2ca_cbfc_disconnect_req(lcid);
	}

        /* l2ca_ecbfc_send_flow_control_credit is same with l2ca_cbfc_send_flow_control_credit */
	return retval;
}

static API_RESULT ethermind_l2ca_ecbfc_low_rx_credit_ind_cb (UINT16 lcid, UINT16 credit)
{
	struct bt_l2cap_le_chan *chan;
	bt_l2cap_conn_scan_param_t conn_scan;

	conn_scan.lcid = sys_le16_to_cpu(lcid);
	conn_scan.chan = NULL;

	BT_DBG("dcid 0x%04x credit %d", conn_scan.lcid, credit);

	(void)bt_conn_scan(ethermind_bt_conn_scan_cb, &conn_scan);

	chan = conn_scan.chan;

	if (NULL != chan)
	{
#if 0
        (void)l2ca_cbfc_send_flow_control_credit
                 (
                     lcid,
                     chan->rx.init_credits - credit
                 );
#endif
		atomic_set(&chan->rx.credits, credit);
	}
	else
	{
		l2ca_ecbfc_disconnect_req(lcid);
	}


	return API_SUCCESS;
}

static API_RESULT ethermind_l2ca_ecbfc_tx_credit_ind_cb (UINT16 lcid, UINT16 result, UINT16 credit)
{
	struct bt_l2cap_le_chan *chan;
	bt_l2cap_conn_scan_param_t conn_scan;
	struct bt_conn *conn;
	uint16_t dcid;
	uint16_t credits;
	API_RESULT retval = API_SUCCESS;

	dcid = sys_le16_to_cpu(lcid);
	conn_scan.lcid = dcid;
	conn_scan.chan = NULL;

	BT_DBG("dcid 0x%04x credit %d", dcid, credit);

	/**
	 * Disconnect channel if credit overflow has occurred. This is in accordance
	 * text in section 10.1 of the Core 5.0 specification
	 */
	if (L2CAP_CREDIT_OVERFLOW == result)
	{
		l2ca_ecbfc_disconnect_req(lcid);
		return retval;
	}

	(void)bt_conn_scan(ethermind_bt_conn_scan_cb, &conn_scan);
	chan = conn_scan.chan;

	if (NULL != chan)
	{
		credits = sys_le16_to_cpu(credit);
		conn = chan->chan.conn;
		atomic_set(&chan->tx.credits, 0);
		if ((atomic_get(&chan->tx.credits) + credits) > UINT16_MAX) {
			BT_ERR("Credits overflow");
			bt_l2cap_chan_disconnect(&chan->chan);
			return retval;
		}

		l2cap_chan_tx_give_credits(chan, credits);

		BT_DBG("chan %p total credits %u", chan, atomic_get(&chan->tx.credits));

		l2cap_chan_tx_resume(chan);
	}
	else
	{
		retval = API_FAILURE;
		l2ca_ecbfc_disconnect_req(lcid);
	}

	(void)conn;

	return retval;
}

static API_RESULT ethermind_l2ca_ecbfc_data_write_cb(UINT16 lcid, UINT16 result, UCHAR *buffer, UINT16 buffer_len)
{
 	struct bt_l2cap_le_chan *chan;
	bt_l2cap_conn_scan_param_t conn_scan;
	struct l2cap_tx_meta_data *user_data = NULL;
	int err = 0;

	conn_scan.lcid = sys_le16_to_cpu(lcid);
	conn_scan.chan = NULL;

	BT_DBG("dcid 0x%04x", conn_scan.lcid);

	(void)bt_conn_scan(ethermind_bt_conn_scan_cb, &conn_scan);

	chan = conn_scan.chan;

	if (API_SUCCESS != result)
	{
		err = -1;
	}

	if (NULL != chan)
	{
		struct l2cap_tx_meta_data user_data_temp;
		memset(&user_data_temp, 0, sizeof(user_data_temp));
		user_data_temp.user_data = (void*)0xFFFFFFFFu;
		user_data_temp.cid = chan->tx.cid;
		if (NULL != chan->_sdu)
		{
			if (l2cap_tx_meta_data_macro(chan->_sdu) != NULL)
			{
				user_data = l2cap_tx_meta_data_macro(chan->_sdu);
			}
			else
			{
				user_data = &user_data_temp;
			}
			net_buf_unref(chan->_sdu);
			chan->_sdu = NULL;
			chan->_sdu_len = 0;
		}
		l2cap_chan_sdu_sent(chan->chan.conn, user_data, err);
	}
	else
	{
		l2ca_cbfc_disconnect_req(lcid);
	}

	return API_SUCCESS;
}

#endif

#endif /* CONFIG_BT_L2CAP_DYNAMIC_CHANNEL */


