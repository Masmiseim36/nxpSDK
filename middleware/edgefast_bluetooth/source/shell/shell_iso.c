/** @file
 *  @brief Bluetooth Audio shell
 *
 */

/*
 * Copyright (c) 2020 Intel Corporation
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * Copyright (C) 2022-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))

#include <stdlib.h>
#include <ctype.h>
#include <porting.h>
#include "fsl_shell.h"
#include <sys/byteorder.h>
#include <sys/util.h>

#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/iso.h>

#include "shell_bt.h"

static uint32_t cis_sn_last;
static uint32_t bis_sn_last;
static int64_t cis_sn_last_updated_ticks;
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
#if 0
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
#else
	int64_t uptime_ms, delta_ms;
	uint64_t delta_us;
	uint64_t seq_num_incr;
	uint64_t next_seq_num;

	uptime_ms   = (int64_t)OSA_TimeGetMsec();
	delta_ms    = uptime_ms - *last_ticks;
	*last_ticks = uptime_ms;

	delta_us     = delta_ms * 1000;
	seq_num_incr = delta_us / interval_us;
	next_seq_num = (seq_num_incr + last_sn);

	return (uint32_t)next_seq_num;
#endif
}

static void iso_recv(struct bt_iso_chan *chan, const struct bt_iso_recv_info *info,
		struct net_buf *buf)
{
	if (info->flags & BT_ISO_FLAGS_VALID) {
	shell_print(ctx_shell, "Incoming data channel %p len %u, seq: %d, ts: %d",
		    chan, buf->len, info->seq_num, info->ts);
	}
}

static void iso_connected(struct bt_iso_chan *chan)
{
	struct bt_iso_info iso_info;
	int err;

	shell_print(ctx_shell, "ISO Channel %p connected", chan);


	err = bt_iso_chan_get_info(chan, &iso_info);
	if (err != 0) {
		PRINTF("Failed to get ISO info: %d", err);
		return;
	}

	if (iso_info.type == BT_ISO_CHAN_TYPE_CONNECTED) {
		cis_sn_last = 0U;
		#if 0
		cis_sn_last_updated_ticks = k_uptime_ticks();
		#else
		cis_sn_last_updated_ticks = (int64_t)OSA_TimeGetMsec();
		#endif
	} else {
		bis_sn_last = 0U;
		#if 0
		bis_sn_last_updated_ticks = k_uptime_ticks();
		#else
		bis_sn_last_updated_ticks = (int64_t)OSA_TimeGetMsec();
		#endif
	}
}

static void iso_disconnected(struct bt_iso_chan *chan, uint8_t reason)
{
	shell_print(ctx_shell, "ISO Channel %p disconnected with reason 0x%02x",
		    chan, reason);
}

static struct bt_iso_chan_ops iso_ops = {
	.recv		= iso_recv,
	.connected	= iso_connected,
	.disconnected	= iso_disconnected,
};

#define DEFAULT_IO_QOS \
{ \
	.sdu		= 40u, \
	.phy		= BT_GAP_LE_PHY_2M, \
	.rtn		= 2u, \
}

static struct bt_iso_chan_io_qos iso_tx_qos = DEFAULT_IO_QOS;

#if (defined(CONFIG_BT_ISO_UNICAST) && (CONFIG_BT_ISO_UNICAST > 0))
static uint32_t cis_sdu_interval_us;

static struct bt_iso_chan_io_qos iso_rx_qos = DEFAULT_IO_QOS;

static struct bt_iso_chan_qos cis_iso_qos = {
	.tx = &iso_tx_qos,
	.rx = &iso_rx_qos,
};

#define CIS_ISO_CHAN_COUNT 1

struct bt_iso_chan iso_chan = {
	.ops = &iso_ops,
	.qos = &cis_iso_qos,
};

NET_BUF_POOL_FIXED_DEFINE(tx_pool, 1, BT_ISO_SDU_BUF_SIZE(CONFIG_BT_ISO_TX_MTU), NULL);

#if (defined(CONFIG_BT_ISO_CENTRAL) && (CONFIG_BT_ISO_CENTRAL > 0))
static struct bt_iso_cig *cig;

static shell_status_t cmd_cig_create(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;
	struct bt_iso_cig_param param;
	struct bt_iso_chan *chans[CIS_ISO_CHAN_COUNT];

	if (cig != NULL) {
		shell_error(shell, "Already created");
		return kStatus_SHELL_Error;
	}

	chans[0] = &iso_chan;

	if (argc > 1) {
		if (!strcmp("tx", argv[1])) {
			chans[0]->qos->tx = &iso_tx_qos;
			chans[0]->qos->rx = NULL;
		} else if (!strcmp("rx", argv[1])) {
			chans[0]->qos->tx = NULL;
			chans[0]->qos->rx = &iso_rx_qos;
		} else if (!strcmp("txrx", argv[1])) {
			chans[0]->qos->tx = &iso_tx_qos;
			chans[0]->qos->rx = &iso_rx_qos;
		}
	}

	err = 0;
	if (argc > 2) {
		unsigned long interval;

		interval = shell_strtoul(argv[2], 0, &err);
		if (err != 0) {
			shell_error(shell, "Could not parse interval: %d", err);

			return kStatus_SHELL_Error;
		}

		if (!IN_RANGE(interval,
			      BT_ISO_SDU_INTERVAL_MIN,
			      BT_ISO_SDU_INTERVAL_MAX)) {
			shell_error(shell, "Invalid interval %lu", interval);

			return kStatus_SHELL_Error;
		}

		param.interval = interval;
	} else {
		param.interval = 10000;
	}
	cis_sdu_interval_us = param.interval;

	if (argc > 3) {
		unsigned long packing;

		packing = shell_strtoul(argv[3], 0, &err);
		if (err != 0) {
			shell_error(shell, "Could not parse packing: %d", err);

			return kStatus_SHELL_Error;
		}

		if (packing != BT_ISO_PACKING_SEQUENTIAL && packing != BT_ISO_PACKING_INTERLEAVED) {
			shell_error(shell, "Invalid packing %lu", packing);

			return kStatus_SHELL_Error;
		}

		param.packing = packing;
	} else {
		param.packing = 0;
	}

	if (argc > 4) {
		unsigned long framing;

		framing = shell_strtoul(argv[4], 0, &err);
		if (err != 0) {
			shell_error(shell, "Could not parse framing: %d", err);

			return kStatus_SHELL_Error;
		}

		if (framing != BT_ISO_FRAMING_UNFRAMED && framing != BT_ISO_FRAMING_FRAMED) {
			shell_error(shell, "Invalid framing %lu", framing);

			return kStatus_SHELL_Error;
		}

		param.framing = framing;
	} else {
		param.framing = 0;
	}

	if (argc > 5) {
		unsigned long latency;

		latency = shell_strtoul(argv[5], 0, &err);
		if (err != 0) {
			shell_error(shell, "Could not parse latency: %d", err);

			return kStatus_SHELL_Error;
		}

		if (!IN_RANGE(latency,
			      BT_ISO_LATENCY_MIN,
			      BT_ISO_LATENCY_MAX)) {
			shell_error(shell, "Invalid latency %lu", latency);

			return kStatus_SHELL_Error;
		}

		param.latency = latency;
	} else {
		param.latency = 10;
	}

	if (argc > 6) {
		unsigned long sdu;

		sdu = shell_strtoul(argv[6], 0, &err);
		if (err != 0) {
			shell_error(shell, "Could not parse sdu: %d", err);

			return kStatus_SHELL_Error;
		}

		if (sdu > BT_ISO_MAX_SDU) {
			shell_error(shell, "Invalid sdu %lu", sdu);

			return kStatus_SHELL_Error;
		}

		if (chans[0]->qos->tx) {
			chans[0]->qos->tx->sdu = sdu;
		}

		if (chans[0]->qos->rx) {
			chans[0]->qos->rx->sdu = sdu;
		}
	}

	if (argc > 7) {
		unsigned long phy;

		phy = shell_strtoul(argv[7], 0, &err);
		if (err != 0) {
			shell_error(shell, "Could not parse phy: %d", err);

			return kStatus_SHELL_Error;
		}

		if (phy != BT_GAP_LE_PHY_1M &&
		    phy != BT_GAP_LE_PHY_2M &&
		    phy != BT_GAP_LE_PHY_CODED) {
			shell_error(shell, "Invalid phy %lu", phy);

			return kStatus_SHELL_Error;
		}

		if (chans[0]->qos->tx) {
			chans[0]->qos->tx->phy = phy;
		}

		if (chans[0]->qos->rx) {
			chans[0]->qos->rx->phy = phy;
		}
	}

	if (argc > 8) {
		unsigned long rtn;

		rtn = shell_strtoul(argv[8], 0, &err);
		if (err != 0) {
			shell_error(shell, "Could not parse rtn: %d", err);

			return kStatus_SHELL_Error;
		}

		if (rtn > BT_ISO_CONNECTED_RTN_MAX) {
			shell_error(shell, "Invalid rtn %lu", rtn);

			return kStatus_SHELL_Error;
		}

		if (chans[0]->qos->tx) {
			chans[0]->qos->tx->rtn = rtn;
		}

		if (chans[0]->qos->rx) {
			chans[0]->qos->rx->rtn = rtn;
		}
	}

	param.sca = BT_GAP_SCA_UNKNOWN;
	param.cis_channels = chans;
	param.num_cis = ARRAY_SIZE(chans);

	err = bt_iso_cig_create(&param, &cig);
	if (err) {
		shell_error(shell, "Unable to create CIG (err %d)", err);
		return kStatus_SHELL_Success;
	}

	shell_print(shell, "CIG created");

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_cig_term(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	if (cig == NULL) {
		shell_error(shell, "CIG not created");
		return kStatus_SHELL_Error;
	}

	err = bt_iso_cig_terminate(cig);
	if (err) {
		shell_error(shell, "Unable to terminate CIG (err %d)", err);
		return kStatus_SHELL_Success;
	}

	shell_print(shell, "CIG terminated");
	cig = NULL;

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_connect(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_iso_connect_param connect_param = {
		.acl = default_conn,
		.iso_chan = &iso_chan
	};
	int err;

	if (iso_chan.iso == NULL) {
		shell_error(shell, "ISO channel not initialized in a CIG");
		return kStatus_SHELL_Success;
	}

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
	if (argc > 1) {
		iso_chan.required_sec_level = (bt_security_t)(*argv[1] - '0');
	}
#endif /* CONFIG_BT_SMP */

	err = bt_iso_chan_connect(&connect_param, 1);
	if (err) {
		shell_error(shell, "Unable to connect (err %d)", err);
		return kStatus_SHELL_Success;
	}

	shell_print(shell, "ISO Connect pending...");

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_ISO_CENTRAL */

#if (defined(CONFIG_BT_ISO_PERIPHERAL) && (CONFIG_BT_ISO_PERIPHERAL > 0))

static int iso_accept(const struct bt_iso_accept_info *info,
		      struct bt_iso_chan **chan)
{
	shell_print(ctx_shell, "Incoming request from %p with CIG ID 0x%02X and CIS ID 0x%02X",
		    info->acl, info->cig_id, info->cis_id);

	if (iso_chan.iso) {
		shell_print(ctx_shell, "No channels available");
		return -ENOMEM;
	}

	*chan = &iso_chan;

	/* As the peripheral host we do not know the SDU interval, and thus we
	 * cannot find the proper interval of incrementing the packet
	 * sequence number (PSN). The only way to ensure that we correctly
	 * increment the PSN, is by incrementing once per the minimum SDU
	 * interval. This should be okay as the spec does not specify how much
	 * the PSN may be incremented, and it is thus OK for us to increment
	 * it faster than the SDU interval.
	 */
	cis_sdu_interval_us = BT_ISO_SDU_INTERVAL_MIN;

	return 0;
}

struct bt_iso_server iso_server = {
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
	.sec_level = BT_SECURITY_L1,
#endif /* CONFIG_BT_SMP */
	.accept = iso_accept,
};

static shell_status_t cmd_listen(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;
	static struct bt_iso_chan_io_qos *tx_qos, *rx_qos;

	if (!strcmp("tx", argv[1])) {
		tx_qos = &iso_tx_qos;
		rx_qos = NULL;
	} else if (!strcmp("rx", argv[1])) {
		tx_qos = NULL;
		rx_qos = &iso_rx_qos;
	} else if (!strcmp("txrx", argv[1])) {
		tx_qos = &iso_tx_qos;
		rx_qos = &iso_rx_qos;
	} else {
		shell_error(shell, "Invalid argument - use tx, rx or txrx");
		return kStatus_SHELL_Error;
	}

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
	if (argc > 2) {
		iso_server.sec_level = (bt_security_t)(*argv[2] - '0');
	}
#endif /* CONFIG_BT_SMP */

	err = bt_iso_server_register(&iso_server);
	if (err) {
		shell_error(shell, "Unable to register ISO cap (err %d)",
			    err);
		return (shell_status_t)err;
	}

	/* Setup peripheral iso data direction only if register is success */
	iso_chan.qos->tx = tx_qos;
	iso_chan.qos->rx = rx_qos;
	return (shell_status_t)err;
}
#endif /* CONFIG_BT_ISO_PERIPHERAL */

static shell_status_t cmd_send(shell_handle_t shell, int32_t argc, char *argv[])
{
	static uint8_t buf_data[CONFIG_BT_ISO_TX_MTU] = {
		[0 ... (CONFIG_BT_ISO_TX_MTU - 1)] = 0xff
	};
	unsigned long count = 1;
	struct net_buf *buf;
	int ret = 0;
	int len;

	if (argc > 1) {
		count = shell_strtoul(argv[1], 0, &ret);
		if (ret != 0) {
			shell_error(shell, "Could not parse count: %d", ret);

			return kStatus_SHELL_Error;
		}
	}

	if (!iso_chan.iso) {
		shell_error(shell, "Not bound");
		return kStatus_SHELL_Success;
	}

	if (!iso_chan.qos->tx) {
		shell_error(shell, "Transmission QoS disabled");
		return kStatus_SHELL_Error;
	}

	len = MIN(iso_chan.qos->tx->sdu, CONFIG_BT_ISO_TX_MTU);
	cis_sn_last = get_next_sn(cis_sn_last, &cis_sn_last_updated_ticks,
				  cis_sdu_interval_us);

	while (count--) {
		buf = net_buf_alloc(&tx_pool, osaWaitForever_c);
		net_buf_reserve(buf, BT_ISO_CHAN_SEND_RESERVE);

		net_buf_add_mem(buf, buf_data, len);
		shell_info(shell, "send: %d bytes of data", len);
		ret = bt_iso_chan_send(&iso_chan, buf, cis_sn_last,
				       BT_ISO_TIMESTAMP_NONE);
		if (ret < 0) {
			shell_print(shell, "Unable to send: %d", -ret);
			net_buf_unref(buf);
			return kStatus_SHELL_Error;
		}
	}

	shell_print(shell, "ISO sending...");

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_disconnect(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	err = bt_iso_chan_disconnect(&iso_chan);
	if (err) {
		shell_error(shell, "Unable to disconnect (err %d)", err);
		return kStatus_SHELL_Success;
	}

	shell_print(shell, "ISO Disconnect pending...");

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_tx_sync_read_cis(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_iso_tx_info tx_info;
	int err;

	if (!iso_chan.iso) {
		shell_error(shell, "Not bound");
		return kStatus_SHELL_Success;
	}

	err = bt_iso_chan_get_tx_sync(&iso_chan, &tx_info);
	if (err) {
		shell_error(shell, "Unable to read sync info (err %d)", err);
		return kStatus_SHELL_Success;
	}

	shell_print(shell, "TX sync info:\n\tTimestamp=%u\n\tOffset=%u\n\tSequence number=%u",
		tx_info.ts, tx_info.offset, tx_info.seq_num);

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_ISO_UNICAST */

#if (defined(CONFIG_BT_ISO_BROADCAST) && (CONFIG_BT_ISO_BROADCAST > 0))
#define BIS_ISO_CHAN_COUNT 1
static struct bt_iso_big *big;

static struct bt_iso_chan_qos bis_iso_qos;

static struct bt_iso_chan bis_iso_chan = {
	.ops = &iso_ops,
	.qos = &bis_iso_qos,
};

static struct bt_iso_chan *bis_channels[BIS_ISO_CHAN_COUNT] = { &bis_iso_chan };

#if (defined(CONFIG_BT_ISO_BROADCASTER) && (CONFIG_BT_ISO_BROADCASTER > 0))
static uint32_t bis_sdu_interval_us;

NET_BUF_POOL_FIXED_DEFINE(bis_tx_pool, BIS_ISO_CHAN_COUNT,
			  BT_ISO_SDU_BUF_SIZE(CONFIG_BT_ISO_TX_MTU), NULL);

static shell_status_t cmd_broadcast(shell_handle_t shell, int32_t argc, char *argv[])
{
	static uint8_t buf_data[CONFIG_BT_ISO_TX_MTU] = {
		[0 ... (CONFIG_BT_ISO_TX_MTU - 1)] = 0xff
	};
	unsigned long count = 1;
	struct net_buf *buf;
	int ret = 0;
	int len;

	if (argc > 1) {
		count = shell_strtoul(argv[1], 0, &ret);
		if (ret != 0) {
			shell_error(shell, "Could not parse count: %d", ret);

			return kStatus_SHELL_Error;
		}
	}

	if (!bis_iso_chan.iso) {
		shell_error(shell, "BIG not created");
		return kStatus_SHELL_Error;
	}

	if (!bis_iso_qos.tx) {
		shell_error(shell, "BIG not setup as broadcaster");
		return kStatus_SHELL_Error;
	}

	len = MIN(bis_iso_chan.qos->tx->sdu, CONFIG_BT_ISO_TX_MTU);

	bis_sn_last = get_next_sn(bis_sn_last, &bis_sn_last_updated_ticks,
				  bis_sdu_interval_us);

	while (count--) {
		for (int i = 0; i < BIS_ISO_CHAN_COUNT; i++) {
			buf = net_buf_alloc(&bis_tx_pool, osaWaitForever_c);
			net_buf_reserve(buf, BT_ISO_CHAN_SEND_RESERVE);

			net_buf_add_mem(buf, buf_data, len);
			ret = bt_iso_chan_send(&bis_iso_chan, buf, bis_sn_last,
					       BT_ISO_TIMESTAMP_NONE);
			if (ret < 0) {
				shell_print(shell, "[%i]: Unable to broadcast: %d", i, -ret);
				net_buf_unref(buf);
				return kStatus_SHELL_Error;
			}
		}
	}

	shell_print(shell, "ISO broadcasting...");

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_big_create(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;
	struct bt_iso_big_create_param param;
	struct bt_le_ext_adv *adv = adv_sets[selected_adv];

	if (!adv) {
		shell_error(shell, "No (periodic) advertising set selected");
		return kStatus_SHELL_Error;
	}

	/* TODO: Allow setting QOS from shell */
	bis_iso_qos.tx = &iso_tx_qos;
	bis_iso_qos.tx->phy = BT_GAP_LE_PHY_2M; /* 2 MBit */
	bis_iso_qos.tx->rtn = 2;
	bis_iso_qos.tx->sdu = CONFIG_BT_ISO_TX_MTU;

	bis_sdu_interval_us = param.interval = 10000;      /* us */
	param.latency = 20;          /* ms */
	param.bis_channels = bis_channels;
	param.num_bis = BIS_ISO_CHAN_COUNT;
	param.encryption = false;
	param.packing = BT_ISO_PACKING_SEQUENTIAL;
	param.framing = BT_ISO_FRAMING_UNFRAMED;

	if (argc > 1) {
		if (!strcmp(argv[1], "enc")) {
			uint8_t bcode_len = hex2bin(argv[1], strlen(argv[1]), param.bcode,
						    sizeof(param.bcode));
			if (!bcode_len || bcode_len != sizeof(param.bcode)) {
				shell_error(shell, "Invalid Broadcast Code Length");
				return kStatus_SHELL_Error;
			}
			param.encryption = true;
		} else {
			shell_help(shell);
			return kStatus_SHELL_Error;
		}
	} else {
		memset(param.bcode, 0, sizeof(param.bcode));
	}

	err = bt_iso_big_create(adv, &param, &big);
	if (err) {
		shell_error(shell, "Unable to create BIG (err %d)", err);
		return kStatus_SHELL_Success;
	}

	shell_print(shell, "BIG created");

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_tx_sync_read_bis(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_iso_tx_info tx_info;
	int err;

	if (!bis_iso_chan.iso) {
		shell_error(shell, "BIG not created");
		return kStatus_SHELL_Error;
	}

	err = bt_iso_chan_get_tx_sync(&bis_iso_chan, &tx_info);
	if (err) {
		shell_error(shell, "Unable to read sync info (err %d)", err);
		return kStatus_SHELL_Success;
	}

	shell_print(shell, "TX sync info:\n\tTimestamp=%u\n\tOffset=%u\n\tSequence number=%u",
		tx_info.ts, tx_info.offset, tx_info.seq_num);

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_ISO_BROADCASTER */

#if (defined(CONFIG_BT_ISO_SYNC_RECEIVER) && (CONFIG_BT_ISO_SYNC_RECEIVER > 0))
static shell_status_t cmd_big_sync(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;
	/* TODO: Add support to select which PA sync to BIG sync to */
	struct bt_le_per_adv_sync *pa_sync = per_adv_syncs[0];
	struct bt_iso_big_sync_param param;
	unsigned long bis_bitfield;

	if (!pa_sync) {
		shell_error(shell, "No PA sync selected");
		return kStatus_SHELL_Error;
	}

	bis_bitfield = shell_strtoul(argv[1], 0, &err);
	if (err != 0) {
		shell_error(shell, "Could not parse bis_bitfield: %d", err);

		return kStatus_SHELL_Error;
	}

	if (bis_bitfield > BIT_MASK(BT_ISO_BIS_INDEX_MAX)) {
		shell_error(shell, "Invalid bis_bitfield: %lu", bis_bitfield);

		return kStatus_SHELL_Error;
	}

	bis_iso_qos.tx = NULL;

	param.bis_channels = bis_channels;
	param.num_bis = BIS_ISO_CHAN_COUNT;
	param.encryption = false;
	param.bis_bitfield = bis_bitfield;
	param.mse = 0;
	param.sync_timeout = 0xFF;

	for (size_t i = 2U; i < argc; i++) {
		if (!strcmp(argv[i], "mse")) {
			unsigned long mse;

			i++;
			if (i == argc) {
				shell_help(shell);
				return kStatus_SHELL_Error;
			}

			mse = shell_strtoul(argv[i], 0, &err);
			if (err != 0) {
				shell_error(shell, "Could not parse mse: %d", err);

				return kStatus_SHELL_Error;
			}

			if (!IN_RANGE(mse,
				      BT_ISO_SYNC_MSE_MIN,
				      BT_ISO_SYNC_MSE_MAX)) {
				shell_error(shell, "Invalid mse %lu", mse);

				return kStatus_SHELL_Error;
			}

			param.mse = mse;
		} else if (!strcmp(argv[i], "timeout")) {
			unsigned long sync_timeout;

			i++;
			if (i == argc) {
				shell_help(shell);
				return kStatus_SHELL_Error;
			}

			sync_timeout = shell_strtoul(argv[i], 0, &err);
			if (err != 0) {
				shell_error(shell,
					    "Could not parse sync_timeout: %d",
					    err);

				return kStatus_SHELL_Error;
			}

			if (!IN_RANGE(sync_timeout,
				      BT_ISO_SYNC_MSE_MIN,
				      BT_ISO_SYNC_MSE_MAX)) {
				shell_error(shell, "Invalid sync_timeout %lu",
					    sync_timeout);

				return kStatus_SHELL_Error;
			}

			param.sync_timeout = sync_timeout;
		} else if (!strcmp(argv[i], "enc")) {
			size_t bcode_len;

			i++;
			if (i == argc) {
				shell_help(shell);
				return kStatus_SHELL_Error;
			}

			memset(param.bcode, 0, sizeof(param.bcode));
			bcode_len = hex2bin(argv[i], strlen(argv[i]), param.bcode,
					    sizeof(param.bcode));

			if (bcode_len == 0) {
				shell_error(shell, "Invalid Broadcast Code");

				return kStatus_SHELL_Error;
			}

			param.encryption = true;
		} else {
			shell_help(sh);
			return kStatus_SHELL_Error;
		}
	}

	err = bt_iso_big_sync(pa_sync, &param, &big);
	if (err) {
		shell_error(shell, "Unable to sync to BIG (err %d)", err);
		return kStatus_SHELL_Success;
	}

	shell_print(shell, "BIG syncing");

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_ISO_SYNC_RECEIVER */

static shell_status_t cmd_big_term(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	err = bt_iso_big_terminate(big);
	if (err) {
		shell_error(shell, "Unable to terminate BIG (err %d)", err);
		return kStatus_SHELL_Success;
	}

	shell_print(shell, "BIG terminated");

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_ISO_BROADCAST*/

SHELL_STATIC_SUBCMD_SET_CREATE(iso_cmds,
#if (defined(CONFIG_BT_ISO_UNICAST) && (CONFIG_BT_ISO_UNICAST > 0))
#if (defined(CONFIG_BT_ISO_CENTRAL) && (CONFIG_BT_ISO_CENTRAL > 0))
	SHELL_CMD_ARG(cig_create, NULL, "[dir=tx,rx,txrx] [interval] [packing] [framing] "
		      "[latency] [sdu] [phy] [rtn]", cmd_cig_create, 1, 8),
	SHELL_CMD_ARG(cig_term, NULL, "Terminate the CIG", cmd_cig_term, 1, 0),
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
	SHELL_CMD_ARG(connect, NULL, "Connect ISO Channel [security level]", cmd_connect, 1, 1),
#else /* !CONFIG_BT_SMP */
	SHELL_CMD_ARG(connect, NULL, "Connect ISO Channel", cmd_connect, 1, 0),
#endif /* CONFIG_BT_SMP */
#endif /* CONFIG_BT_ISO_CENTRAL */
#if (defined(CONFIG_BT_ISO_PERIPHERAL) && (CONFIG_BT_ISO_PERIPHERAL > 0))
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
	SHELL_CMD_ARG(listen, NULL, "<dir=tx,rx,txrx> [security level]", cmd_listen, 2, 1),
#else /* !CONFIG_BT_SMP */
	SHELL_CMD_ARG(listen, NULL, "<dir=tx,rx,txrx>", cmd_listen, 2, 0),
#endif /* CONFIG_BT_SMP */
#endif /* CONFIG_BT_ISO_PERIPHERAL */
	SHELL_CMD_ARG(send, NULL, "Send to ISO Channel [count]",
		      cmd_send, 1, 1),
	SHELL_CMD_ARG(disconnect, NULL, "Disconnect ISO Channel",
		      cmd_disconnect, 1, 0),
	SHELL_CMD_ARG(tx_sync_read_cis, NULL, "Read CIS TX sync info", cmd_tx_sync_read_cis, 1, 0),
#endif /* CONFIG_BT_ISO_UNICAST */
#if (defined(CONFIG_BT_ISO_BROADCASTER) && (CONFIG_BT_ISO_BROADCASTER > 0))
	SHELL_CMD_ARG(create-big, NULL, "Create a BIG as a broadcaster [enc <broadcast code>]",
		      cmd_big_create, 1, 2),
	SHELL_CMD_ARG(broadcast, NULL, "Broadcast on ISO channels", cmd_broadcast, 1, 1),
	SHELL_CMD_ARG(tx_sync_read_bis, NULL, "Read BIS TX sync info", cmd_tx_sync_read_bis, 1, 0),
#endif /* CONFIG_BT_ISO_BROADCASTER */
#if (defined(CONFIG_BT_ISO_SYNC_RECEIVER) && (CONFIG_BT_ISO_SYNC_RECEIVER > 0))
	SHELL_CMD_ARG(sync-big, NULL, "Synchronize to a BIG as a receiver <BIS bitfield> [mse] "
		      "[timeout] [enc <broadcast code>]", cmd_big_sync, 2, 4),
#endif /* CONFIG_BT_ISO_SYNC_RECEIVER */
#if (defined(CONFIG_BT_ISO_BROADCAST) && (CONFIG_BT_ISO_BROADCAST > 0))
	SHELL_CMD_ARG(term-big, NULL, "Terminate a BIG", cmd_big_term, 1, 0),
#endif /* CONFIG_BT_ISO_BROADCAST */
	SHELL_SUBCMD_SET_END
);

static shell_status_t cmd_iso(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc > 1) {
		shell_error(shell, "%s unknown parameter: %s",
			    argv[0], argv[1]);
	} else {
		shell_error(shell, "%s Missing subcommand", argv[0]);
	}

	return kStatus_SHELL_Error;
}

SHELL_CMD_REGISTER(iso, iso_cmds, "Bluetooth ISO shell commands",
		       cmd_iso, 1, 1);

void bt_ShellIsoInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandiso))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandiso.pcCommand);
    }
}

#endif /*  */
