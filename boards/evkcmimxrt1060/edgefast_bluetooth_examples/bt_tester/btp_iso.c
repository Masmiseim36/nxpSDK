/* btp_ias.c - Bluetooth IAS Server Tester */

/*
 * Copyright (c) 2022 Codecoup
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))
#include "porting.h"

#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/iso.h>

#include "btp/btp.h"

#include "fsl_component_log_config.h"
#define LOG_MODULE_NAME bttester_iso
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#define DEFAULT_IO_QOS                                           \
	{                                                        \
		.sdu = 40u, .phy = BT_GAP_LE_PHY_2M, .rtn = 2u,  \
	}

#define BIS_ISO_CHAN_COUNT 1

#if (defined(CONFIG_BT_ISO_BROADCASTER) && (CONFIG_BT_ISO_BROADCASTER > 0))
NET_BUF_POOL_FIXED_DEFINE(bis_tx_pool, BIS_ISO_CHAN_COUNT,
			  BT_ISO_SDU_BUF_SIZE(CONFIG_BT_ISO_TX_MTU),
			  CONFIG_BT_CONN_TX_USER_DATA_SIZE, NULL);
#endif /* CONFIG_BT_ISO_BROADCASTER */

#if (defined(CONFIG_BT_ISO_TX) && (CONFIG_BT_ISO_TX > 0))
static uint32_t bis_sn_last;
static int64_t bis_sn_last_updated_ticks;

/**
 * @brief Get the next sequence number based on the last used values
 *
 * @param last_sn     The last sequence number sent.
 * @param last_ticks  The uptime ticks since the last sequence number increment.
 * @param interval_us The SDU interval in microseconds.
 *
 * @return The next sequence number to use
 */
static uint32_t get_next_sn(uint32_t last_sn, int64_t *last_ticks,
			    uint32_t interval_us)
{
	int64_t uptime_ticks, delta_ticks;
	uint64_t delta_us;
	uint64_t sn_incr;
	uint64_t next_sn;

	/* Note: This does not handle wrapping of ticks when they go above
	 * 2^(62-1)
	 */
	uptime_ticks = k_uptime_ticks();
	delta_ticks = uptime_ticks - *last_ticks;
	*last_ticks = uptime_ticks;

	delta_us = k_ticks_to_us_near64((uint64_t)delta_ticks);
	sn_incr = delta_us / interval_us;
	next_sn = (sn_incr + last_sn);

	return (uint32_t)next_sn;
}
#endif /* CONFIG_BT_ISO_TX */

#if (defined(CONFIG_BT_ISO_RX) && (CONFIG_BT_ISO_RX > 0))
static uint8_t recv_cb_buf[256 + sizeof(struct btp_iso_big_data_received_ev)];

static void iso_bis_recv(struct bt_iso_chan *chan, const struct bt_iso_recv_info *info,
		struct net_buf *buf)
{
	struct btp_iso_big_data_received_ev *ev;

	ev = (struct btp_iso_big_data_received_ev *)recv_cb_buf;
	ev->chan_id = 0;
	ev->data_length = buf->len;
	memcpy(ev->data, buf->data, MIN(buf->len, (sizeof(recv_cb_buf)-sizeof(struct btp_iso_big_data_received_ev))));

	tester_event(BTP_SERVICE_ID_ISO, BTP_ISO_EV_BIS_DATA_RECEIVED, ev, sizeof(*ev) + ev->data_length);
}
#endif /* CONFIG_BT_ISO_RX */

static void iso_bis_connected(struct bt_iso_chan *chan)
{
	struct bt_iso_info iso_info;
	struct btp_iso_big_connected_ev ev;
	int err;

	ev.chan_id = 0;

	err = bt_iso_chan_get_info(chan, &iso_info);
	if (err != 0) {
		return;
	}

#if (defined(CONFIG_BT_ISO_TX) && (CONFIG_BT_ISO_TX > 0))
	if (iso_info.type == BT_ISO_CHAN_TYPE_BROADCASTER) {
		bis_sn_last = 0U;
		bis_sn_last_updated_ticks = k_uptime_ticks();
	}
#endif /* CONFIG_BT_ISO_TX */

	tester_event(BTP_SERVICE_ID_ISO, BTP_ISO_EV_BIS_CONNECTED, &ev, sizeof(ev));
}

static void iso_bis_disconnected(struct bt_iso_chan *chan, uint8_t reason)
{
	struct btp_iso_big_disconnected_ev ev;

	ev.chan_id = 0;

	tester_event(BTP_SERVICE_ID_ISO, BTP_ISO_EV_BIS_DISCONNECTED, &ev, sizeof(ev));
}

static struct bt_iso_chan_ops iso_ops = {
#if (defined(CONFIG_BT_ISO_RX) && (CONFIG_BT_ISO_RX > 0))
	.recv = iso_bis_recv,
#endif /* CONFIG_BT_ISO_RX */
	.connected = iso_bis_connected,
	.disconnected = iso_bis_disconnected,
};

static struct bt_iso_big *big;
static struct bt_iso_chan_io_qos bis_qos_rx = DEFAULT_IO_QOS;
static struct bt_iso_chan_io_qos bis_qos_tx = DEFAULT_IO_QOS;

static struct bt_iso_chan_qos bis_iso_qos = {
	.rx = &bis_qos_rx,
	.tx = &bis_qos_tx,
};

static struct bt_iso_chan bis_iso_chan = {
	.ops = &iso_ops,
	.qos = &bis_iso_qos,
};

static struct bt_iso_chan *bis_channels[BIS_ISO_CHAN_COUNT] = { &bis_iso_chan };

static uint8_t supported_commands(const void *cmd, uint16_t cmd_len,
				  void *rsp, uint16_t *rsp_len)
{
	struct btp_iso_read_supported_commands_rp *rp = rsp;

	/* octet 0 */
	tester_set_bit(rp->data, BTP_ISO_READ_SUPPORTED_COMMANDS);
#if (defined(CONFIG_BT_ISO_BROADCASTER) && (CONFIG_BT_ISO_BROADCASTER > 0))
	tester_set_bit(rp->data, BTP_ISO_BIG_CREATE);
	tester_set_bit(rp->data, BTP_ISO_BIS_SEND);
#endif /* CONFIG_BT_ISO_BROADCASTER */
#if (defined(CONFIG_BT_ISO_SYNC_RECEIVER) && (CONFIG_BT_ISO_SYNC_RECEIVER > 0))
	tester_set_bit(rp->data, BTP_ISO_BIG_SYNC);
#endif /* CONFIG_BT_ISO_SYNC_RECEIVER */
#if (defined(CONFIG_BT_ISO_BROADCAST) && (CONFIG_BT_ISO_BROADCAST > 0))
	tester_set_bit(rp->data, BTP_ISO_BIG_TERM);
#endif /* CONFIG_BT_ISO_BROADCAST */
	*rsp_len = sizeof(*rp);
	return BTP_STATUS_SUCCESS;
}

#if (defined(CONFIG_BT_ISO_BROADCASTER) && (CONFIG_BT_ISO_BROADCASTER > 0))
static uint32_t bis_sdu_interval_us;

static void big_ext_adv_scan_cb(struct bt_le_ext_adv *adv, void *data)
{
	struct bt_le_ext_adv **adv_list = (struct bt_le_ext_adv **)data;

	adv_list[bt_le_ext_adv_get_index(adv)] = adv;
}

void bt_le_ext_adv_foreach(void (*func)(struct bt_le_ext_adv *adv, void *data),
			   void *data);

static uint8_t big_create(const void *cmd, uint16_t cmd_len,
				  void *rsp, uint16_t *rsp_len)
{
	struct btp_iso_big_create_cmd *cp = (struct btp_iso_big_create_cmd *)cmd;
	struct bt_iso_big_create_param param = {0};
	struct bt_le_ext_adv *adv_list[CONFIG_BT_EXT_ADV_MAX_ADV_SET] = {NULL};
	struct bt_le_ext_adv *adv = NULL;
	int err = -EINVAL;

	bt_le_ext_adv_foreach(big_ext_adv_scan_cb, &adv_list[0]);

	if (cp->adv_index < ARRAY_SIZE(adv_list)) {
		adv = adv_list[cp->adv_index];
	}

	if (adv != NULL) {
		param.interval = 10000; /* us */
		param.latency = 20; /* ms */
		param.bis_channels = bis_channels;
		param.num_bis = ARRAY_SIZE(bis_channels);
		param.encryption = cp->encrypt ? true : false;
		param.packing = BT_ISO_PACKING_SEQUENTIAL;
		param.framing = BT_ISO_FRAMING_UNFRAMED;
		memcpy(param.bcode, cp->bcode, sizeof(param.bcode));

		bis_sdu_interval_us = param.interval;

		err = bt_iso_big_create(adv, &param, &big);
	}

	if (err) {
		return BTP_STATUS_FAILED;
	} else {
		return BTP_STATUS_SUCCESS;
	}
}
#endif /* CONFIG_BT_ISO_SYNC_RECEIVER */

#if (defined(CONFIG_BT_ISO_SYNC_RECEIVER) && (CONFIG_BT_ISO_SYNC_RECEIVER > 0))
static uint8_t big_sync(const void *cmd, uint16_t cmd_len,
				  void *rsp, uint16_t *rsp_len)
{
	struct btp_iso_big_sync_cmd *cp = (struct btp_iso_big_sync_cmd *)cmd;
	struct bt_le_per_adv_sync *per_adv_sync;
	struct bt_iso_big_sync_param param;
	int err = -EINVAL;

	per_adv_sync = bt_le_per_adv_sync_lookup_addr(&cp->address, cp->sid);
	if (per_adv_sync != NULL) {
		param.bis_channels = bis_channels;
		param.num_bis = ARRAY_SIZE(bis_channels);
		param.encryption = cp->encrypt ? true : false;
		param.bis_bitfield = cp->bitfield;
		param.mse = 0;
		param.sync_timeout = 0xFF;
		memcpy(param.bcode, cp->bcode, sizeof(param.bcode));

		err = bt_iso_big_sync(per_adv_sync, &param, &big);
	}

	if (err) {
		return BTP_STATUS_FAILED;
	} else {
		return BTP_STATUS_SUCCESS;
	}
}
#endif /* CONFIG_BT_ISO_BROADCASTER */

#if (defined(CONFIG_BT_ISO_BROADCAST) && (CONFIG_BT_ISO_BROADCAST > 0))
static uint8_t big_term(const void *cmd, uint16_t cmd_len,
				  void *rsp, uint16_t *rsp_len)
{
	int err = -EINVAL;

	if (big != NULL) {
		err = bt_iso_big_terminate(big);
	}

	if (err) {
		return BTP_STATUS_FAILED;
	} else {
		return BTP_STATUS_SUCCESS;
	}
}
#endif /* CONFIG_BT_ISO_BROADCAST */

#if (defined(CONFIG_BT_ISO_BROADCASTER) && (CONFIG_BT_ISO_BROADCASTER > 0))
#define TX_BUF_TIMEOUT K_SECONDS(1)

static uint8_t big_send(const void *cmd, uint16_t cmd_len,
				  void *rsp, uint16_t *rsp_len)
{
	struct btp_iso_big_send_cmd *cp = (struct btp_iso_big_send_cmd *)cmd;
	struct net_buf *buf;
	int err = -EINVAL;
	int len;

	if (cp->chan_id >= ARRAY_SIZE(bis_channels)) {
		return BTP_STATUS_FAILED;
	}

	if (bis_channels[cp->chan_id] == NULL) {
		return BTP_STATUS_FAILED;
	}

	buf = net_buf_alloc(&bis_tx_pool, TX_BUF_TIMEOUT);
	if (buf == NULL){
		return BTP_STATUS_FAILED;
	}

	net_buf_reserve(buf, BT_ISO_CHAN_SEND_RESERVE);

	len = MIN(bis_channels[cp->chan_id]->qos->tx->sdu, CONFIG_BT_ISO_TX_MTU);
	len = MIN(len, cp->data_len);

	net_buf_add_mem(buf, cp->data, len);

	bis_sn_last = get_next_sn(bis_sn_last, &bis_sn_last_updated_ticks,
				  bis_sdu_interval_us);

	err = bt_iso_chan_send(bis_channels[cp->chan_id], buf, bis_sn_last);

	if (err) {
		return BTP_STATUS_FAILED;
	} else {
		return BTP_STATUS_SUCCESS;
	}
}
#endif /* CONFIG_BT_ISO_BROADCASTER */

static const struct btp_handler handlers[] = {
	{
		.opcode = BTP_ISO_READ_SUPPORTED_COMMANDS,
		.index = BTP_INDEX_NONE,
		.expect_len = 0,
		.func = supported_commands,
	},
#if (defined(CONFIG_BT_ISO_BROADCASTER) && (CONFIG_BT_ISO_BROADCASTER > 0))
	{
		.opcode = BTP_ISO_BIG_CREATE,
		.index = BTP_INDEX_NONE,
		.expect_len = sizeof(struct btp_iso_big_create_cmd),
		.func = big_create,
	},
#endif /* CONFIG_BT_ISO_BROADCASTER */
#if (defined(CONFIG_BT_ISO_SYNC_RECEIVER) && (CONFIG_BT_ISO_SYNC_RECEIVER > 0))
	{
		.opcode = BTP_ISO_BIG_SYNC,
		.index = BTP_INDEX_NONE,
		.expect_len = sizeof(struct btp_iso_big_sync_cmd),
		.func = big_sync,
	},
#endif /* CONFIG_BT_ISO_SYNC_RECEIVER */
#if (defined(CONFIG_BT_ISO_BROADCAST) && (CONFIG_BT_ISO_BROADCAST > 0))
	{
		.opcode = BTP_ISO_BIG_TERM,
		.index = BTP_INDEX_NONE,
		.expect_len = 0,
		.func = big_term,
	},
#endif /* CONFIG_BT_ISO_BROADCAST */
#if (defined(CONFIG_BT_ISO_BROADCASTER) && (CONFIG_BT_ISO_BROADCASTER > 0))
	{
		.opcode = BTP_ISO_BIS_SEND,
		.index = BTP_INDEX_NONE,
		.expect_len = BTP_HANDLER_LENGTH_VARIABLE,
		.func = big_send,
	},
#endif /* CONFIG_BT_ISO_BROADCASTER */
};

uint8_t tester_init_iso(void)
{
	tester_register_command_handlers(BTP_SERVICE_ID_ISO, handlers,
					 ARRAY_SIZE(handlers));

	return BTP_STATUS_SUCCESS;
}

uint8_t tester_unregister_iso(void)
{
	return BTP_STATUS_SUCCESS;
}
#endif  /* CONFIG_BT_ISO */
