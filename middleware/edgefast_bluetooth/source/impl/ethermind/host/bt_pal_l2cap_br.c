/* l2cap_br.c - L2CAP BREDR oriented handling */

/*
 * Copyright (c) 2016 Intel Corporation
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
#include "sm_internal.h"
#include "sm_extern.h"
#include "BT_sm_api.h"

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_L2CAP)
#define LOG_MODULE_NAME bt_l2cap_br
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
#include "bt_pal_hci_core.h"
#include "bt_pal_conn_internal.h"
#include "bt_pal_l2cap_internal.h"
#include "bt_pal_avdtp_internal.h"
#include "bt_pal_a2dp_internal.h"
#include "bt_pal_rfcomm_internal.h"
#include "bt_pal_sdp_internal.h"
#include "bt_pal_avrcp_internal.h"
#include "l2cap.h"

#define BR_CHAN_RTX(_w) CONTAINER_OF(_w, struct bt_l2cap_br_chan, rtx_work)

#define L2CAP_BR_PSM_START	0x0001
#define L2CAP_BR_PSM_END	0xffff

#define L2CAP_BR_CID_DYN_START	0x0040u
#define L2CAP_BR_CID_DYN_END	0xffffu
#if 0
#define L2CAP_BR_CID_IS_DYN(_cid) \
	(((_cid) >= L2CAP_BR_CID_DYN_START) && ((_cid) <= L2CAP_BR_CID_DYN_END))
#else
#define L2CAP_BR_CID_IS_DYN(_cid) \
	(((_cid) >= L2CAP_BR_CID_DYN_START))
#endif
#define L2CAP_BR_MIN_MTU	48
#define L2CAP_BR_DEFAULT_MTU	672

#define L2CAP_BR_PSM_SDP	0x0001

#define L2CAP_BR_INFO_TIMEOUT		BT_SECONDS(4)
#define L2CAP_BR_CFG_TIMEOUT		BT_SECONDS(4)
#define L2CAP_BR_DISCONN_TIMEOUT	BT_SECONDS(1)
#define L2CAP_BR_CONN_TIMEOUT		BT_SECONDS(40)

/*
 * L2CAP extended feature mask:
 * BR/EDR fixed channel support enabled
 */
#define L2CAP_FEAT_FIXED_CHAN_MASK	0x00000080

enum {
	/* Connection oriented channels flags */
	L2CAP_FLAG_CONN_LCONF_DONE,	/* local config accepted by remote */
	L2CAP_FLAG_CONN_RCONF_DONE,	/* remote config accepted by local */
	L2CAP_FLAG_CONN_ACCEPTOR,	/* getting incoming connection req */
	L2CAP_FLAG_CONN_PENDING,	/* remote sent pending result in rsp */

	/* Signaling channel flags */
	L2CAP_FLAG_SIG_INFO_PENDING,	/* retrieving remote l2cap info */
	L2CAP_FLAG_SIG_INFO_DONE,	/* remote l2cap info is done */

	/* fixed channels flags */
	L2CAP_FLAG_FIXED_CONNECTED,		/* fixed connected */
};

static sys_slist_t br_servers;


/* Pool for outgoing BR/EDR signaling packets, min MTU is 48 */
NET_BUF_POOL_FIXED_DEFINE(br_sig_pool, CONFIG_BT_MAX_CONN,
			  BT_L2CAP_BUF_SIZE(L2CAP_BR_MIN_MTU), NULL);

/* BR/EDR L2CAP signalling channel specific context */
struct bt_l2cap_br {
	/* The channel this context is associated with */
	struct bt_l2cap_br_chan	chan;
	uint8_t			info_ident;
	uint8_t			info_fixed_chan;
	uint32_t			info_feat_mask;
};

static struct bt_l2cap_br bt_l2cap_br_pool[CONFIG_BT_MAX_CONN];

STRUCT_SECTION_DEFINE(bt_l2cap_br_fixed_chan);

struct bt_l2cap_chan *bt_l2cap_br_lookup_rx_cid(struct bt_conn *conn,
						uint16_t cid)
{
	struct bt_l2cap_chan *chan;

	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		if (BR_CHAN(chan)->rx.cid == cid) {
			return chan;
		}
	}

	return NULL;
}

#if 0
static struct bt_l2cap_chan *bt_l2cap_br_lookup_tx_cid(struct bt_conn *conn,
						uint16_t cid)
{
	struct bt_l2cap_chan *chan;

	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		if (BR_CHAN(chan)->tx.cid == cid) {
			return chan;
		}
	}

	return NULL;
}

static struct bt_l2cap_br_chan*
l2cap_br_chan_alloc_cid(struct bt_conn *conn, struct bt_l2cap_chan *chan)
{
	struct bt_l2cap_br_chan *br_chan = BR_CHAN(chan);
	uint16_t cid;

	/*
	 * No action needed if there's already a CID allocated, e.g. in
	 * the case of a fixed channel.
	 */
	if (br_chan->rx.cid > 0) {
		return br_chan;
	}

	/*
	 * L2CAP_BR_CID_DYN_END is 0xffff so we don't check against it since
	 * cid is uint16_t, just check against uint16_t overflow
	 */
	for (cid = L2CAP_BR_CID_DYN_START; cid; cid++) {
		if (!bt_l2cap_br_lookup_rx_cid(conn, cid)) {
			br_chan->rx.cid = cid;
			return br_chan;
		}
	}

	return NULL;
}
#endif

static void l2cap_br_chan_cleanup(struct bt_l2cap_chan *chan)
{
	bt_l2cap_chan_remove(chan->conn, chan);
	bt_l2cap_chan_del(chan);
}

static void l2cap_br_chan_destroy(struct bt_l2cap_chan *chan)
{
	struct bt_l2cap_br_chan *br_chan = BR_CHAN(chan);

	BT_DBG("chan %p cid 0x%04x", br_chan, br_chan->rx.cid);
#if 0
	/* Cancel ongoing work. Since the channel can be re-used after this
	 * we need to sync to make sure that the kernel does not have it
	 * in its queue anymore.
	 *
	 * In the case where we are in the context of executing the rtx_work
	 * item, we don't sync as it will deadlock the workqueue.
	 */
	if (k_current_get() != &br_chan->rtx_work.queue->thread) {
		k_work_cancel_delayable_sync(&br_chan->rtx_work, &br_chan->rtx_sync);
	} else {
		k_work_cancel_delayable(&br_chan->rtx_work);
	}
#endif
	atomic_clear(BR_CHAN(chan)->flags);

	(void)br_chan;  /* Unused variable */
}
#if 0
static void l2cap_br_rtx_timeout(struct k_work *work)
{
	struct bt_l2cap_br_chan *chan = BR_CHAN_RTX(work);

	BT_WARN("chan %p timeout", chan);

	if (chan->rx.cid == BT_L2CAP_CID_BR_SIG) {
		BT_DBG("Skip BR/EDR signalling channel ");
		atomic_clear_bit(chan->flags, L2CAP_FLAG_SIG_INFO_PENDING);
		return;
	}

	BT_DBG("chan %p %s scid 0x%04x", chan,
	       bt_l2cap_chan_state_str(chan->state),
	       chan->rx.cid);

	switch (chan->state) {
	case BT_L2CAP_CONFIG:
		bt_l2cap_br_chan_disconnect(&chan->chan);
		break;
	case BT_L2CAP_DISCONNECTING:
	case BT_L2CAP_CONNECTING:
		l2cap_br_chan_cleanup(&chan->chan);
		break;
	default:
	  /* MISRA 16.4 : The switch statement does not have a non-empty default clause. */
		break;
	}
}
#endif
static bool l2cap_br_chan_add(struct bt_conn *conn, struct bt_l2cap_chan *chan,
				  bt_l2cap_chan_destroy_t destroy, uint16_t rxcid)
{
	struct bt_l2cap_br_chan *ch = BR_CHAN(chan);

	ch->rx.cid = rxcid;
	if (!ch) {
		BT_DBG("Unable to allocate L2CAP CID");
		return false;
	}
#if 0
	/* All dynamic channels have the destroy handler which makes sure that
	 * the RTX work structure is properly released with a cancel sync.
	 * The fixed signal channel is only removed when disconnected and the
	 * disconnected handler is always called from the workqueue itself so
	 * canceling from there should always succeed.
	 */
	k_work_init_delayable(&ch->rtx_work, l2cap_br_rtx_timeout);
#endif
	bt_l2cap_chan_add(conn, chan, destroy);

	return true;
}

static uint8_t l2cap_br_get_ident(void)
{
	static uint8_t ident;

	ident++;
	/* handle integer overflow (0 is not valid) */
	if (!ident) {
		ident++;
	}

	return ident;
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

static void l2cap_br_chan_send_req(struct bt_l2cap_br_chan *chan,
				   struct net_buf *buf, size_t timeout)
{

	if (bt_l2cap_send(chan->chan.conn, BT_L2CAP_CID_BR_SIG, buf)) {
		net_buf_unref(buf);
		return;
	}

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
	k_work_reschedule(&chan->rtx_work, timeout);
#endif
}
#if 0
static void l2cap_br_get_info(struct bt_l2cap_br *l2cap, uint16_t info_type)
{
	struct bt_l2cap_info_req *info;
	struct net_buf *buf;
	struct bt_l2cap_sig_hdr *hdr;

	BT_DBG("info type %u", info_type);

	if (atomic_test_bit(l2cap->chan.flags, L2CAP_FLAG_SIG_INFO_PENDING)) {
		return;
	}

	switch (info_type) {
	case BT_L2CAP_INFO_FEAT_MASK:
	case BT_L2CAP_INFO_FIXED_CHAN:
		break;
	default:
		BT_WARN("Unsupported info type %u", info_type);
		return;
	}

	buf = bt_l2cap_create_pdu(&br_sig_pool, 0);

	atomic_set_bit(l2cap->chan.flags, L2CAP_FLAG_SIG_INFO_PENDING);
	l2cap->info_ident = l2cap_br_get_ident();

	hdr = (struct bt_l2cap_sig_hdr *)net_buf_add(buf, sizeof(*hdr));
	hdr->code = BT_L2CAP_INFO_REQ;
	hdr->ident = l2cap->info_ident;
	hdr->len = sys_cpu_to_le16(sizeof(*info));

	info = (struct bt_l2cap_info_req *)net_buf_add(buf, sizeof(*info));
	info->type = sys_cpu_to_le16(info_type);

	l2cap_br_chan_send_req(&l2cap->chan, buf, L2CAP_BR_INFO_TIMEOUT);
}
#endif
#if 0
static void connect_fixed_channel(struct bt_l2cap_br_chan *chan)
{
	if (atomic_test_and_set_bit(chan->flags, L2CAP_FLAG_FIXED_CONNECTED)) {
		return;
	}

	if (chan->chan.ops && chan->chan.ops->connected) {
		chan->chan.ops->connected(&chan->chan);
	}
}

static void connect_optional_fixed_channels(struct bt_l2cap_br *l2cap)
{
	/* can be change to loop if more BR/EDR fixed channels are added */
	if (l2cap->info_fixed_chan & BIT(BT_L2CAP_CID_BR_SMP)) {
		struct bt_l2cap_chan *chan;

		chan = bt_l2cap_br_lookup_rx_cid(l2cap->chan.chan.conn,
						 BT_L2CAP_CID_BR_SMP);
		if (chan) {
			connect_fixed_channel(BR_CHAN(chan));
		}
	}
}

static int l2cap_br_info_rsp(struct bt_l2cap_br *l2cap, uint8_t ident,
				 struct net_buf *buf)
{
	struct bt_l2cap_info_rsp *rsp;
	uint16_t type, result;
	int err = 0;

	if (atomic_test_bit(l2cap->chan.flags, L2CAP_FLAG_SIG_INFO_DONE)) {
		return 0;
	}

	if (atomic_test_and_clear_bit(l2cap->chan.flags,
					  L2CAP_FLAG_SIG_INFO_PENDING)) {
		/*
		 * Release RTX timer since got the response & there's pending
		 * command request.
		 */
#if 0
		k_work_cancel_delayable(&l2cap->chan.rtx_work);
#endif
	}

	if (buf->len < sizeof(*rsp)) {
		BT_ERR("Too small info rsp packet size");
		err = -EINVAL;
		goto done;
	}

	if (ident != l2cap->info_ident) {
		BT_WARN("Idents mismatch");
		err = -EINVAL;
		goto done;
	}

	rsp = (struct bt_l2cap_info_rsp *)net_buf_pull_mem(buf, sizeof(*rsp));
	result = sys_le16_to_cpu(rsp->result);
	if (result != BT_L2CAP_INFO_SUCCESS) {
		BT_WARN("Result unsuccessful");
		err = -EINVAL;
		goto done;
	}

	type = sys_le16_to_cpu(rsp->type);

	switch (type) {
	case BT_L2CAP_INFO_FEAT_MASK:
		l2cap->info_feat_mask = net_buf_pull_le32(buf);
		BT_DBG("remote info mask 0x%08x", l2cap->info_feat_mask);

		if (!(l2cap->info_feat_mask & L2CAP_FEAT_FIXED_CHAN_MASK)) {
			break;
		}

		l2cap_br_get_info(l2cap, BT_L2CAP_INFO_FIXED_CHAN);
		return 0;
	case BT_L2CAP_INFO_FIXED_CHAN:
		l2cap->info_fixed_chan = net_buf_pull_u8(buf);
		BT_DBG("remote fixed channel mask 0x%02x",
		       l2cap->info_fixed_chan);

		connect_optional_fixed_channels(l2cap);

		break;
	default:
		BT_WARN("type 0x%04x unsupported", type);
		err = -EINVAL;
		break;
	}
done:
	atomic_set_bit(l2cap->chan.flags, L2CAP_FLAG_SIG_INFO_DONE);
	l2cap->info_ident = 0U;
	return err;
}

static uint8_t get_fixed_channels_mask(void)
{
	uint8_t mask = 0U;

	/* this needs to be enhanced if AMP Test Manager support is added */
	STRUCT_SECTION_FOREACH(bt_l2cap_br_fixed_chan, fchan) {
		mask |= BIT(fchan->cid);
	}

	return mask;
}

static int l2cap_br_info_req(struct bt_l2cap_br *l2cap, uint8_t ident,
			     struct net_buf *buf)
{
	struct bt_conn *conn = l2cap->chan.chan.conn;
	struct bt_l2cap_info_req *req = (void *)buf->data;
	struct bt_l2cap_info_rsp *rsp;
	struct net_buf *rsp_buf;
	struct bt_l2cap_sig_hdr *hdr_info;
	uint16_t type;

	if (buf->len < sizeof(*req)) {
		BT_ERR("Too small info req packet size");
		return -EINVAL;
	}

	rsp_buf = bt_l2cap_create_pdu(&br_sig_pool, 0);

	type = sys_le16_to_cpu(req->type);
	BT_DBG("type 0x%04x", type);

	hdr_info = net_buf_add(rsp_buf, sizeof(*hdr_info));
	hdr_info->code = BT_L2CAP_INFO_RSP;
	hdr_info->ident = ident;

	rsp = net_buf_add(rsp_buf, sizeof(*rsp));

	switch (type) {
	case BT_L2CAP_INFO_FEAT_MASK:
		rsp->type = sys_cpu_to_le16(BT_L2CAP_INFO_FEAT_MASK);
		rsp->result = sys_cpu_to_le16(BT_L2CAP_INFO_SUCCESS);
		net_buf_add_le32(rsp_buf, L2CAP_FEAT_FIXED_CHAN_MASK);
		hdr_info->len = sys_cpu_to_le16(sizeof(*rsp) + sizeof(uint32_t));
		break;
	case BT_L2CAP_INFO_FIXED_CHAN:
		rsp->type = sys_cpu_to_le16(BT_L2CAP_INFO_FIXED_CHAN);
		rsp->result = sys_cpu_to_le16(BT_L2CAP_INFO_SUCCESS);
		/* fixed channel mask protocol data is 8 octets wide */
		(void)memset(net_buf_add(rsp_buf, 8), 0, 8);
		rsp->data[0] = get_fixed_channels_mask();

		hdr_info->len = sys_cpu_to_le16(sizeof(*rsp) + 8);
		break;
	default:
		rsp->type = req->type;
		rsp->result = sys_cpu_to_le16(BT_L2CAP_INFO_NOTSUPP);
		hdr_info->len = sys_cpu_to_le16(sizeof(*rsp));
		break;
	}

	l2cap_send(conn, BT_L2CAP_CID_BR_SIG, rsp_buf);

	return 0;
}
#endif
void bt_l2cap_br_connected(struct bt_conn *conn)
{
	struct bt_l2cap_chan *chan;

	STRUCT_SECTION_FOREACH(bt_l2cap_br_fixed_chan, fchan) {
		struct bt_l2cap_br_chan *br_chan;

		if (!fchan->accept) {
			continue;
		}

		if (fchan->accept(conn, &chan) < 0) {
			continue;
		}

		br_chan = BR_CHAN(chan);

		br_chan->rx.cid = fchan->cid;
		br_chan->tx.cid = fchan->cid;

		if (!l2cap_br_chan_add(conn, chan, NULL, fchan->cid)) {
			return;
		}

		/*
		 * other fixed channels will be connected after Information
		 * Response is received
		 */
#if 0
		if (fchan->cid == BT_L2CAP_CID_BR_SIG) {
			struct bt_l2cap_br *sig_ch;

			connect_fixed_channel(br_chan);

			sig_ch = CONTAINER_OF(br_chan, struct bt_l2cap_br, chan);
			l2cap_br_get_info(sig_ch, BT_L2CAP_INFO_FEAT_MASK);
		}
#endif
	}
}

static struct bt_l2cap_server *l2cap_br_server_lookup_psm(uint16_t psm)
{
	struct bt_l2cap_server *server;

	SYS_SLIST_FOR_EACH_CONTAINER(&br_servers, server, node, struct bt_l2cap_server) {
		if (server->psm == psm) {
			return server;
		}
	}

	return NULL;
}
#if 0
static void l2cap_br_conf_add_mtu(struct net_buf *buf, const uint16_t mtu)
{
	net_buf_add_u8(buf, BT_L2CAP_CONF_OPT_MTU);
	net_buf_add_u8(buf, sizeof(mtu));
	net_buf_add_le16(buf, mtu);
}
#endif

#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
static void bt_l2cap_create_config(struct bt_l2cap_cfg_options *cfg, L2CAP_CONFIG_OPTION *config,
	struct bt_l2cap_qos *qos,
#ifdef L2CAP_EXTENDED_FLOW_SPEC
	struct bt_l2cap_retrans_fc *retrans_fc,
	struct bt_l2cap_ext_flow_spec *ext_fs)
#else
	struct bt_l2cap_retrans_fc *retrans_fc)
#endif
{
	cfg->mtu = &config->mtu;
	cfg->flush_timeout = &config->flush_to;
	cfg->fcs_type = &config->fcs;
	/* ethermind dosn't support it yet */
#if 0
	cfg->max_window_size = &config->max_window_size;
#endif

	qos->service_type = config->flow_spec->service_type;
	qos->token_rate = config->flow_spec->token_rate;
	qos->token_bucket_size = config->flow_spec->token_bucket_size;
	qos->peak_bandwidth = config->flow_spec->peak_bandwidth;
	qos->latency = config->flow_spec->latency;
	qos->delay_variation = config->flow_spec->delay_variation;
	cfg->qos = qos;

	retrans_fc->mode = config->fec->mode;
	retrans_fc->tx_window_size = config->fec->tx_window;
	retrans_fc->max_transmit = config->fec->max_transmit;
	retrans_fc->retrans_time_out = config->fec->retx_timeout;
	retrans_fc->monitor_time_out = config->fec->monitor_timeout;
	retrans_fc->max_pdu = config->fec->mps;
	cfg->retrans_fc = retrans_fc;

#ifdef L2CAP_EXTENDED_FLOW_SPEC
	ext_fs->identifier = config->ext_flow_spec->id;
	ext_fs->service_type = config->ext_flow_spec->service_type;
	ext_fs->max_sdu = config->ext_flow_spec->max_sdu_size;
	ext_fs->sdu_inter_arrival_time = config->ext_flow_spec->sdu_ia_time;
	ext_fs->access_latency = config->ext_flow_spec->access_latency;
	ext_fs->flush_timeout = config->ext_flow_spec->flush_to;
	cfg->ext_fs = ext_fs;
#endif
}

static void bt_l2cap_create_ethermind_config(struct bt_l2cap_cfg_options *cfg,
	L2CAP_CONFIG_OPTION *config,
	FLOW_SPEC *flow_spec,
#ifdef L2CAP_EXTENDED_FLOW_SPEC
	L2CAP_FEC_OPTION *fec,
	L2CAP_EXT_FLOW_SPEC *ext_flow_spec)
#else
	L2CAP_FEC_OPTION *fec)
#endif
{
	if (cfg->mtu != NULL) {
	config->mtu = *cfg->mtu;
	}

	if (cfg->flush_timeout != NULL) {
	config->flush_to = *cfg->flush_timeout;
	}

	if (cfg->fcs_type != NULL) {
	config->fcs = *cfg->fcs_type;
	}

	/* ethermind dosn't support it yet */
#if 0
	if (cfg->max_window_size != NULL) {
	config->max_window_size = *cfg->max_window_size;
	}
#endif

	if (cfg->qos != NULL) {
	config->flow_spec = flow_spec;
	config->flow_spec->service_type = cfg->qos->service_type;
	config->flow_spec->token_rate = cfg->qos->token_rate;
	config->flow_spec->token_bucket_size = cfg->qos->token_bucket_size;
	config->flow_spec->peak_bandwidth = cfg->qos->peak_bandwidth;
	config->flow_spec->latency = cfg->qos->latency;
	config->flow_spec->delay_variation = cfg->qos->delay_variation;
	}

	if (cfg->retrans_fc != NULL) {
	config->fec = fec;
	config->fec->mode = cfg->retrans_fc->mode;
	config->fec->tx_window = cfg->retrans_fc->tx_window_size;
	config->fec->max_transmit = cfg->retrans_fc->max_transmit;
	config->fec->retx_timeout = cfg->retrans_fc->retrans_time_out;
	config->fec->monitor_timeout = cfg->retrans_fc->monitor_time_out;
	config->fec->mps = cfg->retrans_fc->max_pdu;
	}

#ifdef L2CAP_EXTENDED_FLOW_SPEC
	if (cfg->ext_fs != NULL) {
	config->ext_flow_spec = ext_flow_spec;
	config->ext_flow_spec->id = cfg->ext_fs->identifier;
	config->ext_flow_spec->service_type = cfg->ext_fs->service_type;
	config->ext_flow_spec->max_sdu_size = cfg->ext_fs->max_sdu;
	config->ext_flow_spec->sdu_ia_time = cfg->ext_fs->sdu_inter_arrival_time;
	config->ext_flow_spec->access_latency = cfg->ext_fs->access_latency;
	config->ext_flow_spec->flush_to = cfg->ext_fs->flush_timeout;
	}
#endif
}
#endif

static void l2cap_br_conf(struct bt_l2cap_chan *chan)
{
#if 0
	struct bt_l2cap_sig_hdr *hdr;
	struct bt_l2cap_conf_req *conf;
	struct net_buf *buf;

	buf = bt_l2cap_create_pdu(&br_sig_pool, 0);

	hdr = net_buf_add(buf, sizeof(*hdr));
	hdr->code = BT_L2CAP_CONF_REQ;
	hdr->ident = l2cap_br_get_ident();
	conf = net_buf_add(buf, sizeof(*conf));
	(void)memset(conf, 0, sizeof(*conf));

	conf->dcid = sys_cpu_to_le16(BR_CHAN(chan)->tx.cid);
	/*
	 * Add MTU option if app set non default BR/EDR L2CAP MTU,
	 * otherwise sent empty configuration data meaning default MTU
	 * to be used.
	 */
	if (BR_CHAN(chan)->rx.mtu != L2CAP_BR_DEFAULT_MTU) {
		l2cap_br_conf_add_mtu(buf, BR_CHAN(chan)->rx.mtu);
	}

	hdr->len = sys_cpu_to_le16(buf->len - sizeof(*hdr));

	/*
	 * TODO:
	 * might be needed to start tracking number of configuration iterations
	 * on both directions
	 */
	l2cap_br_chan_send_req(BR_CHAN(chan), buf, L2CAP_BR_CFG_TIMEOUT);
#endif
	L2CAP_CONFIG_OPTION config;
#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
	FLOW_SPEC flow_spec;
	L2CAP_FEC_OPTION fec;
#ifdef L2CAP_EXTENDED_FLOW_SPEC
	L2CAP_EXT_FLOW_SPEC ext_flow_spec;
#endif
#endif

	l2cap_init_config_option(&config);
	config.mtu = BR_CHAN(chan)->rx.mtu;

#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
	if (chan->ops->get_cfg != NULL) {
		struct bt_l2cap_cfg_options cfg;

		memset(&cfg, 0, sizeof(cfg));
		chan->ops->get_cfg(chan, &cfg);
#ifdef L2CAP_EXTENDED_FLOW_SPEC
		bt_l2cap_create_ethermind_config(&cfg, &config, &flow_spec, &fec, &ext_flow_spec);
#else
		bt_l2cap_create_ethermind_config(&cfg, &config, &flow_spec, &fec);
#endif
	}
#endif

	(void)l2ca_config_req(BR_CHAN(chan)->rx.cid, &config);
}

enum l2cap_br_conn_security_result {
	L2CAP_CONN_SECURITY_PASSED,
	L2CAP_CONN_SECURITY_REJECT,
	L2CAP_CONN_SECURITY_PENDING
};

/*
 * Security helper against channel connection.
 * Returns L2CAP_CONN_SECURITY_PASSED if:
 * - existing security on link is applicable for requested PSM in connection,
 * - legacy (non SSP) devices connecting with low security requirements,
 * Returns L2CAP_CONN_SECURITY_PENDING if:
 * - channel connection process is on hold since there were valid security
 *   conditions triggering authentication indirectly in subcall.
 * Returns L2CAP_CONN_SECURITY_REJECT if:
 * - bt_conn_set_security API returns < 0.
 */

static enum l2cap_br_conn_security_result
l2cap_br_conn_security(struct bt_l2cap_chan *chan, const uint16_t psm)
{
	int check;
	struct bt_l2cap_br_chan *br_chan = BR_CHAN(chan);

	/* For SDP PSM there's no need to change existing security on link */
	if (br_chan->required_sec_level == BT_SECURITY_L0) {
		return L2CAP_CONN_SECURITY_PASSED;
	}

	/*
	 * No link key needed for legacy devices (pre 2.1) and when low security
	 * level is required.
	 */
	if (br_chan->required_sec_level == BT_SECURITY_L1 &&
	    !BT_FEAT_HOST_SSP(chan->conn->br.features)) {
		return L2CAP_CONN_SECURITY_PASSED;
	}

	switch (br_chan->required_sec_level) {
	case BT_SECURITY_L4:
	case BT_SECURITY_L3:
	case BT_SECURITY_L2:
		break;
	default:
		/*
		 * For non SDP PSM connections GAP's Security Mode 4 requires at
		 * least unauthenticated link key and enabled encryption if
		 * remote supports SSP before any L2CAP CoC traffic. So preset
		 * local to MEDIUM security to trigger it if needed.
		 */
		if (BT_FEAT_HOST_SSP(chan->conn->br.features)) {
			br_chan->required_sec_level = BT_SECURITY_L2;
		}
		break;
	}

	check = bt_conn_set_security(chan->conn, br_chan->required_sec_level);

	/*
	 * Check case when on existing connection security level already covers
	 * channel (service) security requirements against link security and
	 * bt_conn_set_security API returns 0 what implies also there was no
	 * need to trigger authentication.
	 */
	if (check == 0 &&
	    chan->conn->sec_level >= br_chan->required_sec_level) {
		return L2CAP_CONN_SECURITY_PASSED;
	}

	/*
	 * If 'check' still holds 0, it means local host just sent HCI
	 * authentication command to start procedure to increase link security
	 * since service/profile requires that.
	 */
	if (check == 0) {
		return L2CAP_CONN_SECURITY_PENDING;
	}

	/*
	 * For any other values in 'check' it means there was internal
	 * validation condition forbidding to start authentication at this
	 * moment.
	 */
	return L2CAP_CONN_SECURITY_REJECT;
}

static void l2cap_br_send_conn_rsp(struct bt_l2cap_chan *chan, uint16_t scid,
				  uint16_t dcid, uint16_t result)
{
#if 0
	struct net_buf *buf;
	struct bt_l2cap_conn_rsp *rsp;
	struct bt_l2cap_sig_hdr *hdr;

	buf = bt_l2cap_create_pdu(&br_sig_pool, 0);

	hdr = net_buf_add(buf, sizeof(*hdr));
	hdr->code = BT_L2CAP_CONN_RSP;
	hdr->ident = ident;
	hdr->len = sys_cpu_to_le16(sizeof(*rsp));

	rsp = net_buf_add(buf, sizeof(*rsp));
	rsp->dcid = sys_cpu_to_le16(dcid);
	rsp->scid = sys_cpu_to_le16(scid);
	rsp->result = sys_cpu_to_le16(result);

	if (result == BT_L2CAP_BR_PENDING) {
		rsp->status = sys_cpu_to_le16(BT_L2CAP_CS_AUTHEN_PEND);
	} else {
		rsp->status = sys_cpu_to_le16(BT_L2CAP_CS_NO_INFO);
	}

	l2cap_send(conn, BT_L2CAP_CID_BR_SIG, buf);
#endif
	uint16_t resp_type;
	API_RESULT ret;
	uint8_t config_pref;

	/* covert result to ethermind result */
	switch (result) {
	case BT_L2CAP_BR_ERR_PSM_NOT_SUPP:
		resp_type = L2CAP_CONNECTION_REFUSED_PSM_UNSUPPORTED;
		break;
	case BT_L2CAP_BR_ERR_SEC_BLOCK:
		resp_type = L2CAP_CONNECTION_REFUSED_SECURITY_BLOCK;
		break;
	case BT_L2CAP_BR_ERR_INVALID_SCID:
		resp_type = L2CAP_CONNECTION_REFUSED_SOME_INVALID_SRC_CID;
		break;
	case BT_L2CAP_BR_ERR_SCID_IN_USE:
		resp_type = L2CAP_CONNECTION_REFUSED_SOME_SRC_CID_ALREADY_ALLOCATED;
		break;
	case BT_L2CAP_BR_ERR_NO_RESOURCES:
		resp_type = L2CAP_CONNECTION_REFUSED_NO_RESOURCE;
		break;
	case BT_L2CAP_BR_PENDING:
		resp_type = L2CAP_CONNECTION_PENDING;
		break;
	case BT_L2CAP_BR_SUCCESS:
		resp_type = L2CAP_CONNECTION_SUCCESSFUL;
		break;
	default:
		resp_type = L2CAP_CONNECTION_REFUSED_NO_RESOURCE;
		break;
	}

	config_pref = L2CAP_CONFIG_PREF_BASIC;
#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
	if ((chan != NULL) && (chan->ops->get_cfg != NULL)) {
		struct bt_l2cap_cfg_options cfg;

		memset(&cfg, 0, sizeof(cfg));
		chan->ops->get_cfg(chan, &cfg);
		if (cfg.retrans_fc != NULL) {
			config_pref = L2CAP_CONFIG_PREF_ERTM_OPTIONAL;
			if (cfg.retrans_fc->mode == BT_L2CAP_MODE_SM) {
				config_pref = L2CAP_CONFIG_PREF_SM_OPTIONAL;
			}
		}
	}
#endif
	ret = l2ca_connect_rsp_ex(dcid, resp_type, L2CAP_CONNECTION_NO_FURTHER_INFORMATION, config_pref);
	if (ret != API_SUCCESS)
	{
		l2ca_disconnect_req(dcid);
	}
}

static int l2cap_br_conn_req_reply(struct bt_l2cap_chan *chan, uint16_t result)
{
	/* Send response to connection request only when in acceptor role */
	if (!atomic_test_bit(BR_CHAN(chan)->flags, L2CAP_FLAG_CONN_ACCEPTOR)) {
		return -ESRCH;
	}

	l2cap_br_send_conn_rsp(chan, BR_CHAN(chan)->tx.cid,
			       BR_CHAN(chan)->rx.cid, result);
	BR_CHAN(chan)->ident = 0U;

	return 0;
}

static void l2cap_br_conn_req(struct bt_conn *conn,
				  uint16_t psm, uint16_t scid, uint16_t dcid)
{
	struct bt_l2cap_chan *chan = NULL;
	struct bt_l2cap_server *server;
	uint16_t result;
	struct bt_l2cap_br_chan *br_chan;

#if 0
	BT_DBG("psm 0x%02x scid 0x%04x", psm, scid);
#endif

	/* Check if there is a server registered */
	server = l2cap_br_server_lookup_psm(psm);
	if (!server) {
		result = BT_L2CAP_BR_ERR_PSM_NOT_SUPP;
		goto no_chan;
	}

	/*
	 * Report security violation for non SDP channel without encryption when
	 * remote supports SSP.
	 */
	if (server->sec_level != BT_SECURITY_L0 &&
	    BT_FEAT_HOST_SSP(conn->br.features) && !conn->encrypt) {
		result = BT_L2CAP_BR_ERR_SEC_BLOCK;
		goto no_chan;
	}
#if 0
	if (!L2CAP_BR_CID_IS_DYN(scid)) {
		result = BT_L2CAP_BR_ERR_INVALID_SCID;
		goto no_chan;
	}

	chan = bt_l2cap_br_lookup_tx_cid(conn, scid);
	if (chan) {
		/*
		 * we have a chan here but this is due to SCID being already in
		 * use so it is not channel we are suppose to pass to
		 * l2cap_br_conn_req_reply as wrong DCID would be used
		 */
		result = BT_L2CAP_BR_ERR_SCID_IN_USE;
		goto no_chan;
	}
#endif

	br_chan = BR_CHAN(chan);

	/*
	 * Request server to accept the new connection and allocate the
	 * channel. If no free channels available for PSM server reply with
	 * proper result and quit since chan pointer is uninitialized then.
	 */
	if (server->accept(conn, &chan) < 0) {
		result = BT_L2CAP_BR_ERR_NO_RESOURCES;
		goto no_chan;
	}

	br_chan->required_sec_level = server->sec_level;

	l2cap_br_chan_add(conn, chan, l2cap_br_chan_destroy, dcid);
#if 0
	BR_CHAN(chan)->tx.cid = scid;
#endif
	br_chan->ident = 0;
	bt_l2cap_chan_set_state(chan, BT_L2CAP_CONNECTING);
	atomic_set_bit(BR_CHAN(chan)->flags, L2CAP_FLAG_CONN_ACCEPTOR);

	/* Disable fragmentation of l2cap rx pdu */
	BR_CHAN(chan)->rx.mtu = MIN(BR_CHAN(chan)->rx.mtu, BT_L2CAP_RX_MTU);

	switch (l2cap_br_conn_security(chan, psm)) {
	case L2CAP_CONN_SECURITY_PENDING:
		result = BT_L2CAP_BR_PENDING;
		/* TODO: auth timeout */
		break;
	case L2CAP_CONN_SECURITY_PASSED:
		result = BT_L2CAP_BR_SUCCESS;
		break;
	case L2CAP_CONN_SECURITY_REJECT:
	default:
		result = BT_L2CAP_BR_ERR_SEC_BLOCK;
		break;
	}
	/* Reply on connection request as acceptor */
	l2cap_br_conn_req_reply(chan, result);

	if (result != BT_L2CAP_BR_SUCCESS) {
		/* Disconnect link when security rules were violated */
		if (result == BT_L2CAP_BR_ERR_SEC_BLOCK) {
			bt_conn_disconnect(conn, BT_HCI_ERR_AUTH_FAIL);
		}

		return;
	}

	bt_l2cap_chan_set_state(chan, BT_L2CAP_CONFIG);
	l2cap_br_conf(chan);
	return;

no_chan:
	l2cap_br_send_conn_rsp(chan, scid, dcid, result);
}

static void l2cap_br_conf_rsp(struct bt_conn *conn,
			  uint16_t				 scid,
			  uint16_t				 result,
			  L2CAP_CONFIG_OPTION *  config_option)
{
	struct bt_l2cap_chan *chan;
	struct bt_l2cap_br_chan *br_chan;

	chan = bt_l2cap_br_lookup_rx_cid(conn, scid);
	if (!chan) {
		BT_ERR("channel mismatch!");
		return;
	}

	br_chan = BR_CHAN(chan);

#if 0
	/* Release RTX work since got the response */
	k_work_cancel_delayable(&br_chan->rtx_work);
#endif

	/*
	 * TODO: handle other results than success and parse response data if
	 * available
	 */
	switch (result) {
	case BT_L2CAP_CONF_SUCCESS:
		atomic_set_bit(br_chan->flags, L2CAP_FLAG_CONN_LCONF_DONE);

		if (br_chan->state == BT_L2CAP_CONFIG &&
		    atomic_test_bit(br_chan->flags,
				    L2CAP_FLAG_CONN_RCONF_DONE)) {
			BT_DBG("scid 0x%04x rx MTU %u dcid 0x%04x tx MTU %u",
			       br_chan->rx.cid, br_chan->rx.mtu,
			       br_chan->tx.cid, br_chan->tx.mtu);
#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
		if (chan->ops->cfg_rsp != NULL) {
			struct bt_l2cap_cfg_options cfg;
			struct bt_l2cap_qos qos;
			struct bt_l2cap_retrans_fc retrans_fc;
#ifdef L2CAP_EXTENDED_FLOW_SPEC
			struct bt_l2cap_ext_flow_spec ext_fs;
#endif

			memset(&cfg, 0, sizeof(cfg));
#ifdef L2CAP_EXTENDED_FLOW_SPEC
			bt_l2cap_create_config(&cfg, config_option, &qos, &retrans_fc, &ext_fs);
#else
			bt_l2cap_create_config(&cfg, config_option, &qos, &retrans_fc);
#endif
			chan->ops->cfg_rsp(chan, &cfg);
		}
#endif

			bt_l2cap_chan_set_state(chan, BT_L2CAP_CONNECTED);
			if (chan->ops && chan->ops->connected) {
				chan->ops->connected(chan);
			}
		}
		break;
	default:
		/* currently disconnect channel on non success result */
		bt_l2cap_chan_disconnect(chan);
		break;
	}
}

static struct bt_conn *l2cap_get_conn;
static void l2cap_get_conn_by_addr(struct bt_conn *conn, void *data)
{
	uint8_t *addr = (uint8_t *)data;

	if (memcmp(&conn->br.dst, addr, 6u) == 0u)
	{
		l2cap_get_conn = conn;
	}
}

static void l2cap_get_conn_by_lcid(struct bt_conn *conn, void *data)
{
	uint16_t lcid = *((uint16_t *)data);
	struct bt_l2cap_chan *chan;

	chan = bt_l2cap_br_lookup_rx_cid(conn, lcid);
	if (chan) {
		l2cap_get_conn = conn;
		return;
	}
}

static API_RESULT reg_l2cap_connect_ind
				  (
					  DEVICE_HANDLE *device_handle,
					  uint16_t cid,
					  uint16_t psm
				  )
{
	struct bt_conn *conn;
	API_RESULT ret;
	uint8_t bd_addr[6u];
	uint16_t rcid;

	ret = device_queue_get_remote_addr_only (device_handle, bd_addr);
	if (API_SUCCESS != ret)
	{
		return ret;
	}

	l2cap_get_conn = NULL;
	bt_conn_foreach(BT_CONN_TYPE_BR, l2cap_get_conn_by_addr, &bd_addr[0]);
	conn = l2cap_get_conn;
	if (conn == NULL) {
		return -1;
	}

	l2cap_get_remote_cid(cid, &rcid);
	l2cap_br_conn_req(conn, psm, rcid, cid);

	return ret;
}

static void l2cap_br_conn_rsp(struct bt_conn *conn, uint16_t dcid,
	uint16_t scid, uint16_t result, uint16_t status);
API_RESULT reg_l2cap_connect_cfm
		   (
			   /* IN */  DEVICE_HANDLE *	handle,
			   /* IN */  UINT16			 lcid,
			   /* IN */  UINT16			 result,
			   /* IN */  UINT16			 status
		   )
{
	struct bt_conn *conn;
	API_RESULT ret;
	uint8_t bd_addr[6u];
	uint16_t rcid;

	ret = device_queue_get_remote_addr_only(handle, bd_addr);
	if (API_SUCCESS != ret)
	{
		return ret;
	}

	l2cap_get_conn = NULL;
	bt_conn_foreach(BT_CONN_TYPE_BR, l2cap_get_conn_by_addr, &bd_addr[0]);
	conn = l2cap_get_conn;
	if (conn == NULL) {
		return -1;
	}

	l2cap_get_remote_cid(lcid, &rcid);
	l2cap_br_conn_rsp(conn, rcid, lcid, result, status);
	return ret;
}

static void l2cap_br_conf_req(struct bt_conn *conn, uint8_t ident,
				uint16_t lcid,
				L2CAP_CONFIG_OPTION *config_option);
API_RESULT reg_l2cap_config_ind
		   (
			  /* IN */  UINT16				 lcid,
			  /* IN */  L2CAP_CONFIG_OPTION *  config_option
		   )
{
	l2cap_get_conn = NULL;
	bt_conn_foreach(BT_CONN_TYPE_BR, l2cap_get_conn_by_lcid, &lcid);
	if (l2cap_get_conn == NULL) {
		return -1;
	}

	l2cap_br_conf_req(l2cap_get_conn, 0, lcid, config_option);
	return 0;
}

API_RESULT reg_l2cap_config_cfm
		   (
			  /* IN */  UINT16				 lcid,
			  /* IN */  UINT16				 result,
			  /* IN */  L2CAP_CONFIG_OPTION   *config_option
		   )
{
	l2cap_get_conn = NULL;
	bt_conn_foreach(BT_CONN_TYPE_BR, l2cap_get_conn_by_lcid, &lcid);
	if (l2cap_get_conn == NULL) {
		return -1;
	}

	l2cap_br_conf_rsp(l2cap_get_conn, lcid, result, config_option);
	return 0;
}

static void l2cap_br_disconn_req(struct bt_conn *conn, uint16_t dcid, uint16_t scid);
API_RESULT reg_l2cap_disconnect_ind
		   (
			  /* IN */  UINT16  lcid
		   )
{
	uint16_t rcid;

	l2cap_get_conn = NULL;
	bt_conn_foreach(BT_CONN_TYPE_BR, l2cap_get_conn_by_lcid, &lcid);
	if (l2cap_get_conn == NULL) {
		return -1;
	}

	l2cap_get_remote_cid(lcid, &rcid);
	l2cap_br_disconn_req(l2cap_get_conn, lcid, rcid);
	return 0;
}

static void l2cap_br_disconn_rsp(struct bt_conn *conn, uint16_t dcid, uint16_t scid);
API_RESULT reg_l2cap_disconnect_cfm
		   (
			  /* IN */  UINT16  lcid,
			  /* IN */  UINT16  result
		   )
{
	uint16_t rcid;

	l2cap_get_conn = NULL;
	bt_conn_foreach(BT_CONN_TYPE_BR, l2cap_get_conn_by_lcid, &lcid);
	if (l2cap_get_conn == NULL) {
		return -1;
	}

	l2cap_get_remote_cid(lcid, &rcid);
	l2cap_br_disconn_rsp(l2cap_get_conn, rcid, lcid);
	return 0;
}

API_RESULT reg_l2cap_data_read
		   (
			  /* IN */  UINT16   lcid,
			  /* IN */  UCHAR *  data,
			  /* IN */  UINT16   datalen
		   )
{
	struct bt_l2cap_chan *chan;

	l2cap_get_conn = NULL;
	bt_conn_foreach(BT_CONN_TYPE_BR, l2cap_get_conn_by_lcid, &lcid);
	if (l2cap_get_conn == NULL) {
		return -1;
	}

	chan = bt_l2cap_br_lookup_rx_cid(l2cap_get_conn, lcid);
	if (!chan) {
		return -1;
	}

	if ((chan->ops != NULL) && (chan->ops->recv != NULL)) {
		struct net_buf data_buf;

		data_buf.__buf = data;
		data_buf.data  = data;
		data_buf.size  = datalen;
		data_buf.len   = datalen;
		chan->ops->recv(chan, &data_buf);
	}
	return 0;
}

int bt_l2cap_br_server_register(struct bt_l2cap_server *server)
{
	PSM_STRUCT l2cap_psm;
	API_RESULT ret;

	if (server->psm < L2CAP_BR_PSM_START || !server->accept) {
		return -EINVAL;
	}

	/* PSM must be odd and lsb of upper byte must be 0 */
	if ((server->psm & 0x0101) != 0x0001) {
		return -EINVAL;
	}

	if (server->sec_level > BT_SECURITY_L4) {
		return -EINVAL;
	} else if (server->sec_level == BT_SECURITY_L0 &&
		   server->psm != L2CAP_BR_PSM_SDP) {
		server->sec_level = BT_SECURITY_L1;
	}

	/* Check if given PSM is already in use */
	if (l2cap_br_server_lookup_psm(server->psm)) {
		BT_DBG("PSM already registered");
		return -EADDRINUSE;
	}

	BT_DBG("PSM 0x%04x", server->psm);

	l2cap_init_psm (server->psm, &l2cap_psm);
	l2cap_psm.l2ca_connect_ind_cb = reg_l2cap_connect_ind;
	l2cap_psm.l2ca_connect_cnf_cb = reg_l2cap_connect_cfm;
	l2cap_psm.l2ca_config_ind_cb = reg_l2cap_config_ind;
	l2cap_psm.l2ca_config_cnf_cb = reg_l2cap_config_cfm;
	l2cap_psm.l2ca_disconnect_ind_cb = reg_l2cap_disconnect_ind;
	l2cap_psm.l2ca_disconnect_cnf_cb = reg_l2cap_disconnect_cfm;
	l2cap_psm.l2ca_data_read_cb = reg_l2cap_data_read;

#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
	if (server->feature_mask != 0) {
		l2cap_psm.feature_mask = server->feature_mask;
	}
#endif
	ret = l2cap_register_psm(&l2cap_psm);
	if (ret != API_SUCCESS) {
		return -EIO;
	}

	sys_slist_append(&br_servers, &server->node);
	
	if (server->sec_level != SM_DEFAULT_SERVICE_SECURITY_LEVEL) {
		SM_SERVICE sm_service;
		uint8_t service_id;

		memset(&sm_service, 0, sizeof(sm_service));
		sm_service.psm = server->psm;
		sm_service.server_ch = 0;
		sm_service.authenticate = server->sec_level;
		sm_service.authorize = SM_SERVICE_AUTHORIZATION_NONE;
		sm_service.encrypt = SM_DEFAULT_SERVICE_ENCRYPTION;
		sm_service.service_sm_cb = NULL;
		if (SM_FALSE == SM_IS_BONDABLE()) {
		sm_service.no_bonding = 1;
		}
		ret = BT_sm_add_service (&sm_service, &service_id);
	}

	return 0;
}
#if 0
static void l2cap_br_send_reject(struct bt_conn *conn, uint8_t ident,
				 uint16_t reason, void *data, uint8_t data_len)
{
	struct bt_l2cap_cmd_reject *rej;
	struct bt_l2cap_sig_hdr *hdr;
	struct net_buf *buf;

	buf = bt_l2cap_create_pdu(&br_sig_pool, 0);

	hdr = net_buf_add(buf, sizeof(*hdr));
	hdr->code = BT_L2CAP_CMD_REJECT;
	hdr->ident = ident;
	hdr->len = sys_cpu_to_le16(sizeof(*rej) + data_len);

	rej = net_buf_add(buf, sizeof(*rej));
	rej->reason = sys_cpu_to_le16(reason);

	/*
	 * optional data if available must be already in little-endian format
	 * made by caller.and be compliant with Core 4.2 [Vol 3, Part A, 4.1,
	 * table 4.4]
	 */
	if (data) {
		net_buf_add_mem(buf, data, data_len);
	}

	l2cap_send(conn, BT_L2CAP_CID_BR_SIG, buf);
}

static uint16_t l2cap_br_conf_opt_mtu(struct bt_l2cap_chan *chan,
				   struct net_buf *buf, size_t len)
{
	uint16_t mtu, result = BT_L2CAP_CONF_SUCCESS;

	/* Core 4.2 [Vol 3, Part A, 5.1] MTU payload length */
	if (len != 2) {
		BT_ERR("tx MTU length %lu invalid", len);
		result = BT_L2CAP_CONF_REJECT;
		goto done;
	}

	/* pulling MTU value moves buf data to next option item */
	mtu = net_buf_pull_le16(buf);
	if (mtu < L2CAP_BR_MIN_MTU) {
		result = BT_L2CAP_CONF_UNACCEPT;
		BR_CHAN(chan)->tx.mtu = L2CAP_BR_MIN_MTU;
		BT_DBG("tx MTU %u invalid", mtu);
		goto done;
	}

	BR_CHAN(chan)->tx.mtu = mtu;
	BT_DBG("tx MTU %u", mtu);
done:
	return result;
}
#endif
static void l2cap_br_conf_req(struct bt_conn *conn, uint8_t ident,
				uint16_t lcid,
				L2CAP_CONFIG_OPTION *config_option)
{
	struct bt_l2cap_chan *chan;
	uint16_t dcid;
	L2CAP_CONFIG_OPTION config;
	API_RESULT ret;
#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
	FLOW_SPEC flow_spec;
	L2CAP_FEC_OPTION fec;
#ifdef L2CAP_EXTENDED_FLOW_SPEC
	L2CAP_EXT_FLOW_SPEC ext_flow_spec;
	struct bt_l2cap_ext_flow_spec ext_fs;
#endif
	struct bt_l2cap_cfg_options cfg;
	struct bt_l2cap_cfg_options rsp;
	struct bt_l2cap_qos qos;
	struct bt_l2cap_retrans_fc retrans_fc;
#endif

	dcid = lcid;

	BT_DBG("dcid 0x%04x", dcid);

	chan = bt_l2cap_br_lookup_rx_cid(conn, dcid);
	if (!chan) {
		BT_ERR("rx channel mismatch!");
#if 0
		struct bt_l2cap_cmd_reject_cid_data data = {.scid = dcid,
								.dcid = 0,
							   };

		l2cap_br_send_reject(conn, ident, BT_L2CAP_REJ_INVALID_CID,
					 &data, sizeof(data));
#endif
		return;
	}

	l2cap_init_config_option(&config);
	BR_CHAN(chan)->tx.mtu = ((L2CAP_MIN_MTU < config_option->mtu)? config_option->mtu: L2CAP_MIN_MTU);

#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
	if (chan->ops->cfg_req != NULL) {
		memset(&cfg, 0, sizeof(cfg));
		memset(&rsp, 0, sizeof(rsp));
#ifdef L2CAP_EXTENDED_FLOW_SPEC
		bt_l2cap_create_config(&cfg, config_option, &qos, &retrans_fc, &ext_fs);
#else
		bt_l2cap_create_config(&cfg, config_option, &qos, &retrans_fc);
#endif
		chan->ops->cfg_req(chan, &cfg, &rsp);
#ifdef L2CAP_EXTENDED_FLOW_SPEC
		bt_l2cap_create_ethermind_config(&rsp, &config, &flow_spec, &fec, &ext_flow_spec);
#else
		bt_l2cap_create_ethermind_config(&rsp, &config, &flow_spec, &fec);
#endif
		goto config_rsp_send;
	}
#endif
	config.mtu = ((L2CAP_MIN_MTU < config_option->mtu)? config_option->mtu: L2CAP_MIN_MTU);
	config.flush_to = config_option->flush_to;
	config.flow_spec = config_option->flow_spec;
	config.fec = config_option->fec;
	config.fcs = config_option->fcs;

#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
config_rsp_send:
#endif
	ret = l2ca_config_rsp (lcid, &config);
	if (ret != API_SUCCESS) {
		return;
	}

	atomic_set_bit(BR_CHAN(chan)->flags, L2CAP_FLAG_CONN_RCONF_DONE);

	if (atomic_test_bit(BR_CHAN(chan)->flags, L2CAP_FLAG_CONN_LCONF_DONE) &&
	    BR_CHAN(chan)->state == BT_L2CAP_CONFIG) {
		BT_DBG("scid 0x%04x rx MTU %u dcid 0x%04x tx MTU %u",
		       BR_CHAN(chan)->rx.cid, BR_CHAN(chan)->rx.mtu,
		       BR_CHAN(chan)->tx.cid, BR_CHAN(chan)->tx.mtu);

		bt_l2cap_chan_set_state(chan, BT_L2CAP_CONNECTED);
		if (chan->ops && chan->ops->connected) {
			chan->ops->connected(chan);
		}
	}
}

static struct bt_l2cap_br_chan *l2cap_br_remove_rx_cid(struct bt_conn *conn,
						       uint16_t cid)
{
	struct bt_l2cap_chan *chan;
	sys_snode_t *prev = NULL;

	/* Protect fixed channels against accidental removal */
	if (!L2CAP_BR_CID_IS_DYN(cid)) {
		return NULL;
	}

	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		if (BR_CHAN(chan)->rx.cid == cid) {
			sys_slist_remove(&conn->channels, prev, &chan->node);
			return BR_CHAN(chan);
		}

		prev = &chan->node;
	}

	return NULL;
}

static void l2cap_br_disconn_req(struct bt_conn *conn, uint16_t dcid, uint16_t scid)
{
	struct bt_l2cap_br_chan *chan;

	BT_DBG("scid 0x%04x dcid 0x%04x", dcid, scid);

	chan = l2cap_br_remove_rx_cid(conn, dcid);

	if (!chan) {
#if 0
		struct bt_l2cap_cmd_reject_cid_data data;

		data.scid = req->scid;
		data.dcid = req->dcid;
		l2cap_br_send_reject(conn, ident, BT_L2CAP_REJ_INVALID_CID,
					 &data, sizeof(data));
#endif
		return;
	}
#if 0
	buf = bt_l2cap_create_pdu(&br_sig_pool, 0);

	hdr = net_buf_add(buf, sizeof(*hdr));
	hdr->code = BT_L2CAP_DISCONN_RSP;
	hdr->ident = ident;
	hdr->len = sys_cpu_to_le16(sizeof(*rsp));

	rsp = net_buf_add(buf, sizeof(*rsp));
	rsp->dcid = sys_cpu_to_le16(chan->rx.cid);
	rsp->scid = sys_cpu_to_le16(chan->tx.cid);
#endif
	bt_l2cap_chan_del(&chan->chan);
#if 0
	l2cap_send(conn, BT_L2CAP_CID_BR_SIG, buf);
#endif
}

static void l2cap_br_connected(struct bt_l2cap_chan *chan)
{
	BT_DBG("ch %p cid 0x%04x", BR_CHAN(chan), BR_CHAN(chan)->rx.cid);
}

static void l2cap_br_disconnected(struct bt_l2cap_chan *chan)
{
	struct bt_l2cap_br_chan *br_chan = BR_CHAN(chan);

	BT_DBG("ch %p cid 0x%04x", br_chan, br_chan->rx.cid);

	if (atomic_test_and_clear_bit(br_chan->flags,
				      L2CAP_FLAG_SIG_INFO_PENDING)) {
		/* Cancel RTX work on signal channel */
#if 0
		(void)k_work_cancel_delayable(&br_chan->rtx_work);
#endif
	}
}

int bt_l2cap_br_chan_disconnect(struct bt_l2cap_chan *chan)
{
	struct bt_conn *conn = chan->conn;
	struct bt_l2cap_br_chan *br_chan;

	if (!conn) {
		return -ENOTCONN;
	}

	br_chan = BR_CHAN(chan);

	if (br_chan->state == BT_L2CAP_DISCONNECTING) {
		return -EALREADY;
	}

	BT_DBG("chan %p scid 0x%04x dcid 0x%04x", chan, br_chan->rx.cid,
	       br_chan->tx.cid);

#if 0
	buf = bt_l2cap_create_pdu(&br_sig_pool, 0);

	hdr = net_buf_add(buf, sizeof(*hdr));
	hdr->code = BT_L2CAP_DISCONN_REQ;
	hdr->ident = l2cap_br_get_ident();
	hdr->len = sys_cpu_to_le16(sizeof(*req));

	req = net_buf_add(buf, sizeof(*req));
	req->dcid = sys_cpu_to_le16(br_chan->tx.cid);
	req->scid = sys_cpu_to_le16(br_chan->rx.cid);

	l2cap_br_chan_send_req(br_chan, buf, L2CAP_BR_DISCONN_TIMEOUT);
#endif
	l2ca_disconnect_req(br_chan->rx.cid);
	bt_l2cap_chan_set_state(chan, BT_L2CAP_DISCONNECTING);

	return 0;
}

static void l2cap_br_disconn_rsp(struct bt_conn *conn, uint16_t dcid, uint16_t scid)
{
	struct bt_l2cap_br_chan *chan;

	BT_DBG("dcid 0x%04x scid 0x%04x", dcid, scid);

	chan = l2cap_br_remove_rx_cid(conn, scid);
	if (!chan) {
		BT_WARN("No dcid 0x%04x channel found", dcid);
		return;
	}

	bt_l2cap_chan_del(&chan->chan);
}

int bt_l2cap_br_chan_connect(struct bt_conn *conn, struct bt_l2cap_chan *chan,
				 uint16_t psm)
{
	API_RESULT ret;
	uint8_t device_handle;
	BT_DEVICE_ADDR device_addr;
	uint8_t config_pref;
	struct bt_l2cap_br_chan *br_chan = BR_CHAN(chan);

	if (!psm) {
		return -EINVAL;
	}

	if (br_chan->psm) {
		return -EEXIST;
	}

	/* PSM must be odd and lsb of upper byte must be 0 */
	if ((psm & 0x0101) != 0x0001) {
		return -EINVAL;
	}

	if (br_chan->required_sec_level > BT_SECURITY_L4) {
		return -EINVAL;
	} else if (br_chan->required_sec_level == BT_SECURITY_L0 &&
		   psm != L2CAP_BR_PSM_SDP) {
		br_chan->required_sec_level = BT_SECURITY_L1;
	}

	switch (br_chan->state) {
	case BT_L2CAP_CONNECTED:
		/* Already connected */
		return -EISCONN;
	case BT_L2CAP_DISCONNECTED:
		/* Can connect */
		break;
	case BT_L2CAP_CONFIG:
	case BT_L2CAP_DISCONNECTING:
	default:
		/* Bad context */
		return -EBUSY;
	}

	if (!l2cap_br_chan_add(conn, chan, l2cap_br_chan_destroy, 0xFFFFu)) {
		return -ENOMEM;
	}

	br_chan->psm = psm;
	bt_l2cap_chan_set_state(chan, BT_L2CAP_CONNECTING);
	atomic_set_bit(BR_CHAN(chan)->flags, L2CAP_FLAG_CONN_PENDING);
#if 0
	switch (l2cap_br_conn_security(chan, psm)) {
	case L2CAP_CONN_SECURITY_PENDING:
		/*
		 * Authentication was triggered, wait with sending request on
		 * connection security changed callback context.
		 */
		 return 0;
	case L2CAP_CONN_SECURITY_PASSED:
		break;
	case L2CAP_CONN_SECURITY_REJECT:
	default:
		l2cap_br_chan_cleanup(chan);
		return -EIO;
	}

	buf = bt_l2cap_create_pdu(&br_sig_pool, 0);

	hdr = net_buf_add(buf, sizeof(*hdr));
	hdr->code = BT_L2CAP_CONN_REQ;
	hdr->ident = l2cap_br_get_ident();
	hdr->len = sys_cpu_to_le16(sizeof(*req));

	req = net_buf_add(buf, sizeof(*req));
	req->psm = sys_cpu_to_le16(psm);
	req->scid = sys_cpu_to_le16(BR_CHAN(chan)->rx.cid);

	l2cap_br_chan_send_req(BR_CHAN(chan), buf, L2CAP_BR_CONN_TIMEOUT);
#endif
	BT_INIT_BD_ADDR(&device_addr);
	BT_COPY_BD_ADDR (BT_BD_ADDR(&device_addr), &conn->br.dst);
	ret = device_queue_search_br_edr_remote_addr (&device_handle, &device_addr);

	config_pref = L2CAP_CONFIG_PREF_BASIC;
#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
	if (chan->ops->get_cfg != NULL) {
		struct bt_l2cap_cfg_options cfg;

		memset(&cfg, 0, sizeof(cfg));
		chan->ops->get_cfg(chan, &cfg);
		if (cfg.retrans_fc != NULL) {
			config_pref = L2CAP_CONFIG_PREF_ERTM_OPTIONAL;
			if (cfg.retrans_fc->mode == BT_L2CAP_MODE_SM) {
				config_pref = L2CAP_CONFIG_PREF_SM_OPTIONAL;
			}
		}
	}
#endif
	ret = l2ca_connect_req_ex(psm, psm, &device_handle, config_pref);
	if (ret != API_SUCCESS) {
		return -EIO;
	}

	return 0;
}

static void l2cap_br_conn_rsp(struct bt_conn *conn, uint16_t dcid,
	uint16_t scid, uint16_t result, uint16_t status)
{
	struct bt_l2cap_chan *chan;
	struct bt_l2cap_br_chan *br_chan;

	(void)status;

	BT_DBG("dcid 0x%04x scid 0x%04x result %u status %u", dcid, scid,
		   result, status);



#if 0
	/* Release RTX work since got the response */
	k_work_cancel_delayable(&BR_CHAN(chan)->rtx_work);
#endif

	switch (result) {
	case BT_L2CAP_BR_SUCCESS:
        /* find the first one that is 0xFFFFu that is connecting by ethermind */
        chan = bt_l2cap_br_lookup_rx_cid(conn, 0xFFFFu);
        if (!chan) {
            BT_ERR("No scid 0x%04x channel found", scid);
            return;
        }

	br_chan = BR_CHAN(chan);

        br_chan->rx.cid = scid;
	if (br_chan->state != BT_L2CAP_CONNECTING) {
            BT_DBG("Invalid channel %p state %s", chan,
		       bt_l2cap_chan_state_str(br_chan->state));
            return;
        }

		br_chan->ident = 0U;
		br_chan->tx.cid = dcid;
		l2cap_br_conf(chan);
		bt_l2cap_chan_set_state(chan, BT_L2CAP_CONFIG);
		atomic_clear_bit(br_chan->flags, L2CAP_FLAG_CONN_PENDING);
		break;
	case BT_L2CAP_BR_PENDING:
#if 0
		k_work_schedule(&BR_CHAN(chan)->rtx_work, L2CAP_BR_CONN_TIMEOUT);
#endif
		break;
	default:
        chan = bt_l2cap_br_lookup_rx_cid(conn, scid);
        if(NULL != chan)
        {
            l2cap_br_chan_cleanup(chan);
        }
		break;
	}
}

int bt_l2cap_br_chan_send_cb(struct bt_l2cap_chan *chan, struct net_buf *buf, bt_conn_tx_cb_t cb,
			     void *user_data)
{
	struct bt_l2cap_br_chan *br_chan = BR_CHAN(chan);

	if (buf->len > br_chan->tx.mtu) {
		return -EMSGSIZE;
	}

	return bt_l2cap_send_cb(br_chan->chan.conn, br_chan->tx.cid, buf, cb, user_data);
}

int bt_l2cap_br_chan_send(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
	struct bt_l2cap_br_chan *br_chan = BR_CHAN(chan);
	uint8_t *ether_buf;
	uint16_t actual = 0U;

	if (buf->len > br_chan->tx.mtu) {
		return -EMSGSIZE;
	}

	if (buf->len + L2CAP_HDR_LEN > BT_STATIC_DATA_SIZE){
		ether_buf = OSA_MemoryAllocate(buf->len + L2CAP_HDR_LEN);
		if (ether_buf == NULL) {
			return -EMSGSIZE;
		}
		memcpy(&ether_buf[L2CAP_HDR_LEN], buf->data, buf->len);
		l2ca_data_write(br_chan->rx.cid, buf->len + L2CAP_HDR_LEN,
			ether_buf,
			&actual);
	} else {
		l2ca_data_write(br_chan->rx.cid, buf->len + L2CAP_HDR_LEN,
			buf->data - L2CAP_HDR_LEN,
			&actual);
	}

	if (actual != buf->len)
	{
		return -EIO;
	}
	net_buf_unref(buf);
	return actual;
}

static int l2cap_br_recv(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
#if 0
	struct bt_l2cap_br *l2cap = CONTAINER_OF(chan, struct bt_l2cap_br, chan);
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

	if (!hdr->ident) {
		BT_ERR("Invalid ident value in L2CAP PDU");
		return 0;
	}

	switch (hdr->code) {
	case BT_L2CAP_INFO_RSP:
		l2cap_br_info_rsp(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_INFO_REQ:
		l2cap_br_info_req(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_DISCONN_REQ:
		l2cap_br_disconn_req(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_CONN_REQ:
		l2cap_br_conn_req(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_CONF_RSP:
		l2cap_br_conf_rsp(l2cap, hdr->ident, len, buf);
		break;
	case BT_L2CAP_CONF_REQ:
		l2cap_br_conf_req(l2cap, hdr->ident, len, buf);
		break;
	case BT_L2CAP_DISCONN_RSP:
		l2cap_br_disconn_rsp(l2cap, hdr->ident, buf);
		break;
	case BT_L2CAP_CONN_RSP:
		l2cap_br_conn_rsp(l2cap, hdr->ident, buf);
		break;
	default:
		BT_WARN("Unknown/Unsupported L2CAP PDU code 0x%02x", hdr->code);
		l2cap_br_send_reject(chan->conn, hdr->ident,
				     BT_L2CAP_REJ_NOT_UNDERSTOOD, NULL, 0);
		break;
	}
#endif
	return 0;
}

static void l2cap_br_conn_pend(struct bt_l2cap_chan *chan, uint8_t status)
{
	struct net_buf *buf;
	struct bt_l2cap_sig_hdr *hdr;
	struct bt_l2cap_conn_req *req;

	if (BR_CHAN(chan)->state != BT_L2CAP_CONNECTING) {
		return;
	}

	BT_DBG("chan %p status 0x%02x encr 0x%02x", chan, status,
	       chan->conn->encrypt);

	if (status) {
		/*
		 * Security procedure status is non-zero so respond with
		 * security violation only as channel acceptor.
		 */
		l2cap_br_conn_req_reply(chan, BT_L2CAP_BR_ERR_SEC_BLOCK);

		/* Release channel allocated to outgoing connection request */
		if (atomic_test_bit(BR_CHAN(chan)->flags,
				    L2CAP_FLAG_CONN_PENDING)) {
			l2cap_br_chan_cleanup(chan);
		}

		return;
	}

	if (!chan->conn->encrypt) {
		return;
	}

	/*
	 * For incoming connection state send confirming outstanding
	 * response and initiate configuration request.
	 */
	if (l2cap_br_conn_req_reply(chan, BT_L2CAP_BR_SUCCESS) == 0) {
		bt_l2cap_chan_set_state(chan, BT_L2CAP_CONFIG);
		/*
		 * Initialize config request since remote needs to know
		 * local MTU segmentation.
		 */
		l2cap_br_conf(chan);
	} else if (atomic_test_and_clear_bit(BR_CHAN(chan)->flags,
					     L2CAP_FLAG_CONN_PENDING)) {
		buf = bt_l2cap_create_pdu(&br_sig_pool, 0);

		hdr = net_buf_add(buf, sizeof(*hdr));
		hdr->code = BT_L2CAP_CONN_REQ;
		hdr->ident = l2cap_br_get_ident();
		hdr->len = sys_cpu_to_le16(sizeof(*req));

		req = net_buf_add(buf, sizeof(*req));
		req->psm = sys_cpu_to_le16(BR_CHAN(chan)->psm);
		req->scid = sys_cpu_to_le16(BR_CHAN(chan)->rx.cid);

		l2cap_br_chan_send_req(BR_CHAN(chan), buf,
				       L2CAP_BR_CONN_TIMEOUT);
	}
}

void l2cap_br_encrypt_change(struct bt_conn *conn, uint8_t hci_status)
{
	struct bt_l2cap_chan *chan;

	SYS_SLIST_FOR_EACH_CONTAINER(&conn->channels, chan, node, struct bt_l2cap_chan) {
		l2cap_br_conn_pend(chan, hci_status);

		if (chan->ops && chan->ops->encrypt_change) {
			chan->ops->encrypt_change(chan, hci_status);
		}
	}
}
#if 0
static void check_fixed_channel(struct bt_l2cap_chan *chan)
{
	struct bt_l2cap_br_chan *br_chan = BR_CHAN(chan);

	if (br_chan->rx.cid < L2CAP_BR_CID_DYN_START) {
		connect_fixed_channel(br_chan);
	}
}
#endif
void bt_l2cap_br_recv(struct bt_conn *conn, struct net_buf *buf)
{
#if 0
	struct bt_l2cap_hdr *hdr;
	struct bt_l2cap_chan *chan;
	uint16_t cid;

	if (buf->len < sizeof(*hdr)) {
		BT_ERR("Too small L2CAP PDU received");
		net_buf_unref(buf);
		return;
	}

	hdr = net_buf_pull_mem(buf, sizeof(*hdr));
	cid = sys_le16_to_cpu(hdr->cid);

	chan = bt_l2cap_br_lookup_rx_cid(conn, cid);
	if (!chan) {
		BT_WARN("Ignoring data for unknown channel ID 0x%04x", cid);
		net_buf_unref(buf);
		return;
	}

	/*
	 * if data was received for fixed channel before Information
	 * Response we connect channel here.
	 */
	check_fixed_channel(chan);

	chan->ops->recv(chan, buf);
	net_buf_unref(buf);
#endif
}

static int l2cap_br_accept(struct bt_conn *conn, struct bt_l2cap_chan **chan)
{
	int i;
	static const struct bt_l2cap_chan_ops ops = {
		.connected = l2cap_br_connected,
		.disconnected = l2cap_br_disconnected,
		.recv = l2cap_br_recv,
	};

	BT_DBG("conn %p handle %u", conn, conn->handle);

	for (i = 0; i < ARRAY_SIZE(bt_l2cap_br_pool); i++) {
		struct bt_l2cap_br *l2cap = &bt_l2cap_br_pool[i];

		if (l2cap->chan.chan.conn) {
			continue;
		}

		l2cap->chan.chan.ops = &ops;
		*chan = &l2cap->chan.chan;
		atomic_set(l2cap->chan.flags, 0);
		return 0;
	}

	BT_ERR("No available L2CAP context for conn %p", conn);

	return -ENOMEM;
}

BT_L2CAP_BR_CHANNEL_DEFINE(br_fixed_chan, BT_L2CAP_CID_BR_SIG, l2cap_br_accept);

void bt_l2cap_br_init(void)
{
	sys_slist_init(&br_servers);

	if (IS_ENABLED(CONFIG_BT_RFCOMM)) {
		bt_rfcomm_init();
	}

	bt_sdp_init();

	if (IS_ENABLED(CONFIG_BT_A2DP)) {
		bt_a2dp_init();
	}

	if (IS_ENABLED(CONFIG_BT_AVRCP)) {
		bt_avrcp_init();
	}
}
#endif /* CONFIG_BT_BREDR */
