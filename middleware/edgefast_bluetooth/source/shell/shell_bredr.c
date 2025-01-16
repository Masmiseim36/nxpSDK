/** @file
 * @brief Bluetooth BR/EDR shell module
 *
 * Provide some Bluetooth shell commands that can be useful to applications.
 */

/*
 * Copyright 2021 NXP
 * Copyright (c) 2018 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno/errno.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/byteorder.h>
#include <porting.h>

#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>

#if (defined(CONFIG_BT_CLASSIC) && ((CONFIG_BT_CLASSIC) > 0U))

#include "fsl_shell.h"

#include "shell_bt.h"

#if (defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0))
/* Connection context for BR/EDR legacy pairing in sec mode 3 */
static struct bt_conn *pairing_conn;
#endif /* CONFIG_BT_CONN */

#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
#define MAX_BR_L2CAP_CHANNEL    2
#else
#define MAX_BR_L2CAP_CHANNEL    1
#endif
#define DATA_BREDR_MTU		100
#define DATA_BREDR_COUNT	1

NET_BUF_POOL_FIXED_DEFINE(data_pool, DATA_BREDR_COUNT, DATA_BREDR_MTU, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

#define SDP_CLIENT_USER_BUF_LEN		512
NET_BUF_POOL_FIXED_DEFINE(sdp_client_pool, CONFIG_BT_MAX_CONN,
			  SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
static uint8_t select_l2cap_mode;
#endif

static int cmd_auth_pincode(const struct shell *sh,
			    size_t argc, char *argv[])
{
	struct bt_conn *conn;
	uint8_t max = 16U;

	if (default_conn) {
		conn = default_conn;
	} else if (pairing_conn) {
		conn = pairing_conn;
	} else {
		conn = NULL;
	}

	if (!conn) {
		shell_print(sh, "Not connected");
		return 0;
	}

	if (strlen(argv[1]) > max) {
		shell_print(sh, "PIN code value invalid - enter max %u "
			    "digits", max);
		return 0;
	}

	shell_print(sh, "PIN code \"%s\" applied", argv[1]);

	bt_conn_auth_pincode_entry(conn, argv[1]);

	return 0;
}

static int cmd_connect(const struct shell *sh, size_t argc, char *argv[])
{
	struct bt_conn *conn;
	bt_addr_t addr;
	int err;

	memset(&addr, 0, sizeof(addr));
	err = bt_addr_from_str(argv[1], &addr);
	if (err) {
		shell_print(sh, "Invalid peer address (err %d)", err);
		return -ENOEXEC;
	}

	conn = bt_conn_create_br(&addr, BT_BR_CONN_PARAM_DEFAULT);
	if (!conn) {
		shell_print(sh, "Connection failed");
	} else {

		shell_print(sh, "Connection pending");

		/* unref connection obj in advance as app user */
		bt_conn_unref(conn);
	}

	return 0;
}

static void br_device_found(const bt_addr_t *addr, int8_t rssi,
				  const uint8_t cod[3], const uint8_t eir[240])
{
	char br_addr[BT_ADDR_STR_LEN];
	char name[239];
	int len = 240;

	(void)memset(name, 0, sizeof(name));

	while (len) {
		if (len < 2) {
			break;
		}

		/* Look for early termination */
		if (!eir[0]) {
			break;
		}

		/* Check if field length is correct */
		if (eir[0] > len - 1) {
			break;
		}

		switch (eir[1]) {
		case BT_DATA_NAME_SHORTENED:
		case BT_DATA_NAME_COMPLETE:
			if (eir[0] > sizeof(name) - 1) {
				memcpy(name, &eir[2], sizeof(name) - 1);
			} else {
				memcpy(name, &eir[2], eir[0] - 1);
			}
			break;
		default:
			break;
		}

		/* Parse next AD Structure */
		len -= eir[0] + 1;
		eir += eir[0] + 1;
	}

	bt_addr_to_str(addr, br_addr, sizeof(br_addr));

	shell_print(ctx_shell, "[DEVICE]: %s, RSSI %i %s", br_addr, rssi, name);
}

static struct bt_br_discovery_result br_discovery_results[5];

static void br_discovery_complete(struct bt_br_discovery_result *results,
				  size_t count)
{
	size_t i;

	shell_print(ctx_shell, "BR/EDR discovery complete");

	for (i = 0; i < count; i++) {
		br_device_found(&results[i].addr, results[i].rssi,
				results[i].cod, results[i].eir);
	}
}

static int cmd_discovery(const struct shell *sh, size_t argc, char *argv[])
{
	const char *action;

	action = argv[1];
	if (!strcmp(action, "on")) {
		struct bt_br_discovery_param param;

		param.limited = false;
		param.length = 8U;

		if (argc > 2) {
			param.length = atoi(argv[2]);
		}

		if (argc > 3 && !strcmp(argv[3], "limited")) {
			param.limited = true;
		}

		if (bt_br_discovery_start(&param, br_discovery_results,
					  ARRAY_SIZE(br_discovery_results),
					  br_discovery_complete) < 0) {
			shell_print(sh, "Failed to start discovery");
			return 0;
		}

		shell_print(sh, "Discovery started");
	} else if (!strcmp(action, "off")) {
		if (bt_br_discovery_stop()) {
			shell_print(sh, "Failed to stop discovery");
			return 0;
		}

		shell_print(sh, "Discovery stopped");
	} else {
		shell_help(sh);
	}

	return 0;
}

static int l2cap_recv(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
	shell_print(ctx_shell, "Incoming data channel %p len %u", chan,
		    buf->len);
	if (buf->len) {
		shell_hexdump(ctx_shell, buf->data, buf->len);
	}

	return 0;
}

static void l2cap_connected(struct bt_l2cap_chan *chan)
{
	shell_print(ctx_shell, "Channel %p connected", chan);
}

struct br_l2ch {
	bool used;
	struct bt_l2cap_br_chan ch;
};

static void l2cap_channel_free(struct br_l2ch *chan);
#define BR_L2CH_CHAN(_chan) CONTAINER_OF(_chan, struct br_l2ch, ch.chan)
static void l2cap_disconnected(struct bt_l2cap_chan *chan)
{
	struct br_l2ch *l2ch = BR_L2CH_CHAN(chan);

	shell_print(ctx_shell, "Channel %p disconnected", chan);
	l2cap_channel_free(l2ch);
}

static struct net_buf *l2cap_alloc_buf(struct bt_l2cap_chan *chan)
{
	shell_print(ctx_shell, "Channel %p requires buffer", chan);

	return net_buf_alloc(&data_pool, K_FOREVER);
}

static const struct bt_l2cap_chan_ops l2cap_ops = {
	.alloc_buf	= l2cap_alloc_buf,
	.recv		= l2cap_recv,
	.connected	= l2cap_connected,
	.disconnected	= l2cap_disconnected,
};

struct br_l2ch br_l2ch_chan[MAX_BR_L2CAP_CHANNEL] = {
	{
		.used = false,
		.ch.chan.ops	= &l2cap_ops,
		/* Set for now min. MTU */
		.ch.rx.mtu		= DATA_BREDR_MTU,
	},
};

static struct br_l2ch * l2cap_channel_create_new(void)
{
	for (int i = 0;i < MAX_BR_L2CAP_CHANNEL;i++)
	{
		if (false == br_l2ch_chan[i].used)
		{
			br_l2ch_chan[i].used = true;
			br_l2ch_chan[i].ch.chan.ops = &l2cap_ops;
			br_l2ch_chan[i].ch.rx.mtu = DATA_BREDR_MTU;
			br_l2ch_chan[i].ch.rx.credits = DATA_BREDR_COUNT;
			return &br_l2ch_chan[i];
		}
	}
	return NULL;
}

static void l2cap_channel_free(struct br_l2ch *chan)
{
	if (true == chan->used)
	{
		chan->used = false;
		memset(chan, 0, sizeof(*chan));
	}
}

static struct br_l2ch * l2cap_channel_lookup_conn(struct bt_conn *conn)
{
	for (int i = 0;i < MAX_BR_L2CAP_CHANNEL;i++)
	{
		if ((true == br_l2ch_chan[i].used) && (conn == br_l2ch_chan[i].ch.chan.conn))
		{
			return &br_l2ch_chan[i];
		}
	}
	return NULL;
}

static int l2cap_accept(struct bt_conn *conn, struct bt_l2cap_server *server,
			struct bt_l2cap_chan **chan)
{
	struct br_l2ch *br_l2cap_channel;

	shell_print(ctx_shell, "Incoming BR/EDR conn %p", conn);
	br_l2cap_channel = l2cap_channel_create_new();

	if (NULL == br_l2cap_channel) {
		shell_error(ctx_shell, "No channels available");
		return -ENOMEM;
	}

	br_l2cap_channel->ch.chan.ops = &l2cap_ops;
	*chan = &br_l2cap_channel->ch.chan;

	return 0;
}

static struct bt_l2cap_server br_server = {
	.accept = l2cap_accept,
	.sec_level = BT_SECURITY_L2,
};

static int cmd_l2cap_register(const struct shell *sh,
			      size_t argc, char *argv[])
{
	if (br_server.psm) {
		shell_print(sh, "Already registered");
		return 0;
	}

	br_server.psm = strtoul(argv[1], NULL, 16);
#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
        select_l2cap_mode = 0u;
#endif

	if (bt_l2cap_br_server_register(&br_server) < 0) {
		shell_error(sh, "Unable to register psm");
		br_server.psm = 0U;
		return -ENOEXEC;
	} else {
		shell_print(sh, "L2CAP psm %u registered", br_server.psm);
	}

	return 0;
}

#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
static struct bt_l2cap_retrans_fc rfc;

void l2cap_mode_get_cfg(struct bt_l2cap_chan *chan, struct bt_l2cap_cfg_options *cfg)
{
    switch (select_l2cap_mode) {
    case 0: /* basic mode */
    {
        break;
    }
    case 1: /*Retransmission mode*/
    {
        break;
    }
    case 2: /*Flow control mode*/
        break;
    case BT_L2CAP_MODE_ERTM: /*Enhanced Retransmission mode*/
    {
        rfc.mode = BT_L2CAP_MODE_ERTM;
        rfc.max_pdu = 200u;
        rfc.max_transmit = 20u;
        rfc.tx_window_size = 10u;
        rfc.retrans_time_out = 10000u;
        rfc.monitor_time_out = 2000u;

        cfg->retrans_fc = &rfc;
        break;
    }
    case BT_L2CAP_MODE_SM: /*Streaming mode*/
    {
        rfc.mode = BT_L2CAP_MODE_SM;
        rfc.max_pdu = 100u;
        rfc.max_transmit = 5u;
        rfc.tx_window_size = 7u;
        rfc.retrans_time_out = 10000u;
        rfc.monitor_time_out = 2000u;

        cfg->retrans_fc = &rfc;
        break;
    }
    default:
        break;
    }
}

void l2cap_mode_cfg_req(struct bt_l2cap_chan *chan, struct bt_l2cap_cfg_options *cfg, struct bt_l2cap_cfg_options *rsp)
{
    shell_print(ctx_shell, "received configuration request");
    if (cfg->retrans_fc != NULL) {
        memcpy(rsp, cfg, sizeof(*rsp));
        if (cfg->retrans_fc->mode == BT_L2CAP_MODE_ERTM) {
            rfc.mode = BT_L2CAP_MODE_ERTM;
            rfc.max_pdu = 200u;
            rfc.max_transmit = 20u;
            rfc.tx_window_size = 10u;
            rfc.retrans_time_out = 10000u;
            rfc.monitor_time_out = 2000u;

            rsp->retrans_fc = &rfc;
        }

        switch (select_l2cap_mode) {
        case 0: /* basic mode */
            shell_print(ctx_shell, "  basic mode");
            break;
        case 1: /*Retransmission mode*/
            shell_print(ctx_shell, "  Retransmission mode");
            break;
        case 2: /*Flow control mode*/
            shell_print(ctx_shell, "  Flow control mode");
            break;
        case 3: /*Enhanced Retransmission mode*/
            shell_print(ctx_shell, "  Enhanced Retransmission mode");
            break;
        case 4: /*Streaming mode*/
            shell_print(ctx_shell, "  Streaming mode");
            break;
        default:
            break;
        }
    } else {
        shell_print(ctx_shell, "  basic mode");
        memcpy(rsp, cfg, sizeof(*rsp));
    }
}

void l2cap_mode_cfg_rsp(struct bt_l2cap_chan *chan, struct bt_l2cap_cfg_options *rsp)
{
    shell_print(ctx_shell, "received configuration response");
    if (rsp->retrans_fc != NULL) {
        switch (select_l2cap_mode) {
        case 0: /* basic mode */
            shell_print(ctx_shell, "  basic mode");
            break;
        case 1: /*Retransmission mode*/
            shell_print(ctx_shell, "  Retransmission mode");
            break;
        case 2: /*Flow control mode*/
            shell_print(ctx_shell, "  Flow control mode");
            break;
        case 3: /*Enhanced Retransmission mode*/
            shell_print(ctx_shell, "  Enhanced Retransmission mode");
            break;
        case 4: /*Streaming mode*/
            shell_print(ctx_shell, "  Streaming mode");
            break;
        default:
            break;
        }
    } else {
        shell_print(ctx_shell, "  basic mode");
    }
}

static const struct bt_l2cap_chan_ops l2cap_mode_ops = {
	.alloc_buf	= l2cap_alloc_buf,
	.recv		= l2cap_recv,
	.connected	= l2cap_connected,
	.disconnected	= l2cap_disconnected,
        .get_cfg        = l2cap_mode_get_cfg,
        .cfg_req        = l2cap_mode_cfg_req,
        .cfg_rsp        = l2cap_mode_cfg_rsp
};

static int l2cap_mode_accept(struct bt_conn *conn, struct bt_l2cap_server *server, struct bt_l2cap_chan **chan)
{
	struct br_l2ch *br_l2cap_channel;

	shell_print(ctx_shell, "Incoming BR/EDR conn %p", conn);
	br_l2cap_channel = l2cap_channel_create_new();

	if (NULL == br_l2cap_channel) {
		shell_error(ctx_shell, "No channels available");
		return -ENOMEM;
	}

        br_l2cap_channel->ch.chan.ops = &l2cap_mode_ops;
	*chan = &br_l2cap_channel->ch.chan;

	return 0;
}

static struct bt_l2cap_server br_mode_server = {
	.accept = l2cap_mode_accept,
	.sec_level = BT_SECURITY_L2,
	.feature_mask = BT_L2CAP_FEATURE_ERTM |
		BT_L2CAP_FEATURE_SM |
		BT_L2CAP_FEATURE_FCS,
};

static int cmd_l2cap_register_modes(const struct shell *sh,
			      size_t argc, char *argv[])
{
        uint8_t mode;

	if (br_mode_server.psm) {
		shell_print(sh, "Already registered");
		return 0;
	}

	br_mode_server.psm = strtoul(argv[1], NULL, 16);
        mode = strtoul(argv[2], NULL, 16);
        if ((mode != 3u) && (mode != 4u)) {
            shell_error(sh, "mode is not right");
            return -EINVAL;
        }
        select_l2cap_mode = mode;

	if (bt_l2cap_br_server_register(&br_mode_server) < 0) {
		shell_error(sh, "Unable to register psm");
		br_mode_server.psm = 0U;
		return -EINVAL;
	} else {
		shell_print(sh, "L2CAP psm %u registered", br_mode_server.psm);
	}

	return 0;
}
#endif

static int cmd_discoverable(const struct shell *sh,
			    size_t argc, char *argv[])
{
	int err;
	const char *action;

	action = argv[1];

	if (!strcmp(action, "on")) {
		err = bt_br_set_discoverable(true);
	} else if (!strcmp(action, "off")) {
		err = bt_br_set_discoverable(false);
	} else {
		shell_help(sh);
		return 0;
	}

	if (err) {
		shell_print(sh, "BR/EDR set/reset discoverable failed "
			    "(err %d)", err);
		return -ENOEXEC;
	} else {
		shell_print(sh, "BR/EDR set/reset discoverable done");
	}

	return 0;
}

static int cmd_connectable(const struct shell *sh,
			   size_t argc, char *argv[])
{
	int err;
	const char *action;

	action = argv[1];

	if (!strcmp(action, "on")) {
		err = bt_br_set_connectable(true);
	} else if (!strcmp(action, "off")) {
		err = bt_br_set_connectable(false);
	} else {
		shell_help(sh);
		return 0;
	}

	if (err) {
		shell_print(sh, "BR/EDR set/rest connectable failed "
			    "(err %d)", err);
		return -ENOEXEC;
	} else {
		shell_print(sh, "BR/EDR set/reset connectable done");
	}

	return 0;
}

static int cmd_oob(const struct shell *sh, size_t argc, char *argv[])
{
	char addr[BT_ADDR_STR_LEN];
	struct bt_br_oob oob;
	int err;

	err = bt_br_oob_get_local(&oob);
	if (err) {
		shell_print(sh, "BR/EDR OOB data failed");
		return -ENOEXEC;
	}

	bt_addr_to_str(&oob.addr, addr, sizeof(addr));

	shell_print(sh, "BR/EDR OOB data:");
	shell_print(sh, "  addr %s", addr);
	return 0;
}

static uint8_t sdp_hfp_ag_user(struct bt_conn *conn,
			       struct bt_sdp_client_result *result)
{
	char addr[BT_ADDR_STR_LEN];
	uint16_t param, version;
	uint16_t features;
	int res;

	conn_addr_str(conn, addr, sizeof(addr));

	if (result) {
		shell_print(ctx_shell, "SDP HFPAG data@%p (len %u) hint %u from"
			    " remote %s", result->resp_buf,
			    result->resp_buf->len, result->next_record_hint,
			    addr);

		/*
		 * Focus to get BT_SDP_ATTR_PROTO_DESC_LIST attribute item to
		 * get HFPAG Server Channel Number operating on RFCOMM protocol.
		 */
		res = bt_sdp_get_proto_param(result->resp_buf,
					     BT_SDP_PROTO_RFCOMM, &param);
		if (res < 0) {
			shell_error(ctx_shell, "Error getting Server CN, "
				    "err %d", res);
			goto done;
		}
		shell_print(ctx_shell, "HFPAG Server CN param 0x%04x", param);

		res = bt_sdp_get_profile_version(result->resp_buf,
						 BT_SDP_HANDSFREE_SVCLASS,
						 &version);
		if (res < 0) {
			shell_error(ctx_shell, "Error getting profile version, "
				    "err %d", res);
			goto done;
		}
		shell_print(ctx_shell, "HFP version param 0x%04x", version);

		/*
		 * Focus to get BT_SDP_ATTR_SUPPORTED_FEATURES attribute item to
		 * get profile Supported Features mask.
		 */
		res = bt_sdp_get_features(result->resp_buf, &features);
		if (res < 0) {
			shell_error(ctx_shell, "Error getting HFPAG Features, "
				    "err %d", res);
			goto done;
		}
		shell_print(ctx_shell, "HFPAG Supported Features param 0x%04x",
		      features);
	} else {
		shell_print(ctx_shell, "No SDP HFPAG data from remote %s",
			    addr);
	}
done:
	return BT_SDP_DISCOVER_UUID_CONTINUE;
}

static uint8_t sdp_a2src_user(struct bt_conn *conn,
			   struct bt_sdp_client_result *result)
{
	char addr[BT_ADDR_STR_LEN];
	uint16_t param, version;
	uint16_t features;
	int res;

	conn_addr_str(conn, addr, sizeof(addr));

	if (result) {
		shell_print(ctx_shell, "SDP A2SRC data@%p (len %u) hint %u from"
			    " remote %s", result->resp_buf,
			    result->resp_buf->len, result->next_record_hint,
			    addr);

		/*
		 * Focus to get BT_SDP_ATTR_PROTO_DESC_LIST attribute item to
		 * get A2SRC Server PSM Number.
		 */
		res = bt_sdp_get_proto_param(result->resp_buf,
					     BT_SDP_PROTO_L2CAP, &param);
		if (res < 0) {
			shell_error(ctx_shell, "A2SRC PSM Number not found, "
				    "err %d", res);
			goto done;
		}

		shell_print(ctx_shell, "A2SRC Server PSM Number param 0x%04x",
			    param);

		/*
		 * Focus to get BT_SDP_ATTR_PROFILE_DESC_LIST attribute item to
		 * get profile version number.
		 */
		res = bt_sdp_get_profile_version(result->resp_buf,
						 BT_SDP_ADVANCED_AUDIO_SVCLASS,
						 &version);
		if (res < 0) {
			shell_error(ctx_shell, "A2SRC version not found, "
				    "err %d", res);
			goto done;
		}
		shell_print(ctx_shell, "A2SRC version param 0x%04x", version);

		/*
		 * Focus to get BT_SDP_ATTR_SUPPORTED_FEATURES attribute item to
		 * get profile supported features mask.
		 */
		res = bt_sdp_get_features(result->resp_buf, &features);
		if (res < 0) {
			shell_error(ctx_shell, "A2SRC Features not found, "
				    "err %d", res);
			goto done;
		}
		shell_print(ctx_shell, "A2SRC Supported Features param 0x%04x",
		      features);
	} else {
		shell_print(ctx_shell, "No SDP A2SRC data from remote %s",
			    addr);
	}
done:
	return BT_SDP_DISCOVER_UUID_CONTINUE;
}

static uint8_t bt_pbap_pce_sdp_user(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
    int res;
	uint32_t peer_feature = 0;
	uint16_t rfommchannel = 0;
	uint16_t l2cappsm     = 0;
    if ((result) && (result->resp_buf))
    {
        res = bt_sdp_get_pbap_map_ctn_features(result->resp_buf, &peer_feature);
        if (res < 0)
        {
            shell_error(ctx_shell, "No peer_feature found\r\n");
        }
        else
        {
            shell_print(ctx_shell, "peer_feature : %x\r\n", peer_feature);
        }
        res = bt_sdp_get_goep_l2cap_psm(result->resp_buf, &l2cappsm);
        if (res < 0)
        {
            shell_error(ctx_shell, "No GOEP_L2CAPPSM found\r\n");
        }
        else
        {
            shell_print(ctx_shell, "GOEP_L2CAPPSM : %d\r\n", l2cappsm);
        }

        res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_RFCOMM, &rfommchannel);
        if (res < 0)
        {
            shell_error(ctx_shell, "No rfcomm found\r\n");
        }
        else
        {
            shell_print(ctx_shell, "rfcomm channel : %d\r\n", rfommchannel);
        }
    }

    return BT_SDP_DISCOVER_UUID_STOP;
}

static struct bt_sdp_discover_params discov_hfpag = {
	.uuid = BT_UUID_DECLARE_16(BT_SDP_HANDSFREE_AGW_SVCLASS),
	.func = sdp_hfp_ag_user,
	.pool = &sdp_client_pool,
};

static struct bt_sdp_discover_params discov_a2src = {
	.uuid = BT_UUID_DECLARE_16(BT_SDP_AUDIO_SOURCE_SVCLASS),
	.func = sdp_a2src_user,
	.pool = &sdp_client_pool,
};

static struct bt_sdp_discover_params discov_pbap_pce = {
    .uuid = BT_UUID_DECLARE_16(BT_SDP_PBAP_PSE_SVCLASS),
    .func = bt_pbap_pce_sdp_user,
    .pool = &sdp_client_pool,
};

static struct bt_sdp_discover_params discov;

static int cmd_sdp_find_record(const struct shell *sh,
			       size_t argc, char *argv[])
{
	int res;
	const char *action;

	if (!default_conn) {
		shell_print(sh, "Not connected");
		return 0;
	}

	action = argv[1];

	if (!strcmp(action, "HFPAG")) {
		discov = discov_hfpag;
	} else if (!strcmp(action, "A2SRC")) {
		discov = discov_a2src;
	}else if (!strcmp(action, "PBAP_PCE")){
		discov = discov_pbap_pce;
	}else {
		shell_help(sh);
		return SHELL_CMD_HELP_PRINTED;
	}

	shell_print(sh, "SDP UUID \'%s\' gets applied", action);

	res = bt_sdp_discover(default_conn, &discov);
	if (res) {
		shell_error(sh, "SDP discovery failed: result %d", res);
		return -ENOEXEC;
	} else {
		shell_print(sh, "SDP discovery started");
	}

	return 0;
}

static int cmd_l2cap_connect(const struct shell *sh, size_t argc, char *argv[])
{
	struct br_l2ch *br_l2cap_channel;
	uint16_t psm;
	int err;

	if (!default_conn) {
		shell_error(sh, "Not connected");
		return -EINVAL;
	}

	br_l2cap_channel = l2cap_channel_create_new();
	if (NULL == br_l2cap_channel) {
		shell_error(sh, "Channel already in use");
		return -EINVAL;
	}

	psm = strtoul(argv[1], NULL, 16);

	if (argc > 2) {
		int sec;

		sec = *argv[2] - '0';
#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && (CONFIG_BT_L2CAP_DYNAMIC_CHANNEL > 0))
		br_l2cap_channel->ch.required_sec_level = (bt_security_t)sec;
#endif
	}
#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
        if (select_l2cap_mode != 0u) {
            br_l2cap_channel->ch.chan.ops = &l2cap_mode_ops;
        }
#endif
	err = bt_l2cap_chan_connect(default_conn, &br_l2cap_channel->ch.chan, psm);
	if (err < 0) {
		shell_error(sh, "Unable to connect to psm %u (err %d)", psm,
			    err);
                l2cap_channel_free(br_l2cap_channel);
	} else {
		shell_print(sh, "L2CAP connection pending");
	}

	return err;
}

static int cmd_l2cap_disconnect(const struct shell *sh, size_t argc, char *argv[])
{
	struct br_l2ch *br_l2cap_channel;
	int err;

	br_l2cap_channel = l2cap_channel_lookup_conn(default_conn);
	if (NULL == br_l2cap_channel) {
		shell_error(sh, "Channel is not found");
		return -EINVAL;
	}

	err = bt_l2cap_chan_disconnect(&br_l2cap_channel->ch.chan);
	if (err) {
		shell_print(sh, "Unable to disconnect: %u", -err);
	}

	return err;
}

#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED> 0))
static int cmd_ecred_register(const struct shell *sh, size_t argc, char *argv[])
{
	const char *policy;

	if (br_server.psm) {
		shell_error(sh, "Already registered");
		return -ENOEXEC;
	}

	br_server.psm = strtoul(argv[1], NULL, 16);

	if (argc > 2) {
		br_server.sec_level = (bt_security_t)strtoul(argv[2], NULL, 10);
	}

	if (bt_l2cap_server_register(&br_server) < 0) {
		shell_error(sh, "Unable to register psm");
		br_server.psm = 0U;
		return -ENOEXEC;
	} else {
		shell_print(sh, "L2CAP psm %u sec_level %u registered",
			    br_server.psm, br_server.sec_level);
	}

	return 0;
}

static int cmd_ecred_reconfigure(const struct shell *sh, size_t argc, char *argv[])
{
	struct bt_l2cap_chan *l2cap_ecred_chans[] = { NULL, NULL };
	struct br_l2ch *br_l2cap_channel;
	uint16_t mtu;
	int err = 0;

	if (!default_conn) {
		shell_error(sh, "Not connected");
		return -ENOEXEC;
	}

	br_l2cap_channel = l2cap_channel_lookup_conn(default_conn);
	if (NULL == br_l2cap_channel) {
		shell_error(sh, "Not connected");
		return -EINVAL;
	}

	l2cap_ecred_chans[0] = &br_l2cap_channel->ch.chan;

	mtu = shell_strtoul(argv[1], 10, &err);
	if (err) {
		shell_error(sh, "Unable to parse MTU (err %d)", err);

		return -ENOEXEC;
	}

	err = bt_l2cap_ecred_chan_reconfigure(l2cap_ecred_chans, mtu, 0);
	if (err < 0) {
		shell_error(sh, "Unable to reconfigure channel (err %d)", err);
	} else {
		shell_print(sh, "L2CAP reconfiguration pending");
	}

	return err;
}

static int cmd_ecred_connect(const struct shell *sh, size_t argc, char *argv[])
{
	struct bt_l2cap_chan *l2cap_ecred_chans[] = { NULL, NULL };
	struct br_l2ch *br_l2cap_channel;
	uint16_t psm;
	int err = 0;

	if (!default_conn) {
		shell_error(sh, "Not connected");

		return -ENOEXEC;
	}

	br_l2cap_channel = l2cap_channel_create_new();
	if (NULL == br_l2cap_channel) {
		shell_error(sh, "Channel already in use");
		return -EINVAL;
	}

	l2cap_ecred_chans[0] = &br_l2cap_channel->ch.chan;

	psm = shell_strtoul(argv[1], 16, &err);
	if (err) {
		shell_error(sh, "Unable to parse PSM (err %d)", err);

		return err;
	}

	if (argc > 2) {
		int sec;

		sec = shell_strtoul(argv[2], 10, &err);
		if (err) {
			shell_error(sh, "Unable to parse security level (err %d)", err);

			return err;
		}


		br_l2cap_channel->ch.required_sec_level = (bt_security_t)sec;
	}

	err = bt_l2cap_ecred_chan_connect(default_conn, l2cap_ecred_chans, psm);
	if (err < 0) {
		shell_error(sh, "Unable to connect to psm %u (err %d)", psm,
			    err);
		l2cap_channel_free(br_l2cap_channel);
	} else {
		shell_print(sh, "L2CAP connection pending");
	}

	return err;
}
#endif /* CONFIG_BT_L2CAP_ECRED */

static int cmd_l2cap_send(const struct shell *sh, size_t argc, char *argv[])
{
	struct br_l2ch *br_l2cap_channel;
	static uint8_t buf_data[DATA_BREDR_MTU] = { [0 ... (DATA_BREDR_MTU - 1)] = 0xff };
	int ret, len, count = 1;
	struct net_buf *buf;

	if (argc > 1) {
		count = strtoul(argv[1], NULL, 10);
	}

	br_l2cap_channel = l2cap_channel_lookup_conn(default_conn);
	if (NULL == br_l2cap_channel) {
		shell_error(sh, "Channel is not found");
		return -EINVAL;
	}

	len = MIN(br_l2cap_channel->ch.tx.mtu, DATA_BREDR_MTU - BT_L2CAP_CHAN_SEND_RESERVE);

	while (count--) {
		buf = net_buf_alloc(&data_pool, osaWaitForever_c);
		net_buf_reserve(buf, BT_L2CAP_CHAN_SEND_RESERVE);

		net_buf_add_mem(buf, buf_data, len);
		ret = bt_l2cap_chan_send(&br_l2cap_channel->ch.chan, buf);
		if (ret < 0) {
			shell_print(sh, "Unable to send: %d", -ret);
			net_buf_unref(buf);
			return -EINVAL;
		}
	}

	return 0;
}

void discovery_recv_cb(const struct bt_br_discovery_result *result)
{
	char addr[BT_ADDR_LE_STR_LEN];
	const uint8_t *eir;
	size_t len = sizeof(result->eir);
	static uint8_t temp[240];

	bt_addr_to_str(&result->addr, addr, sizeof(addr));
	PRINTF("Monitor: Device: %s, rssi %d, cod 0x%X%X%X", addr, result->rssi,
		       result->cod[0], result->cod[1], result->cod[2]);

	eir = result->eir;

	while ((eir[0] > 2) && (len > eir[0])) {
		switch (eir[1]) {
		case BT_DATA_NAME_SHORTENED:
		case BT_DATA_NAME_COMPLETE:
			memcpy(temp, &eir[2], eir[0] - 1);
			temp[eir[0] - 1] = '\0'; /* Set end flag */
			PRINTF(", name %s", temp);
			break;
		}
		len = len - eir[0] - 1;
		eir = eir + eir[0] + 1;
	}
	PRINTF("\n");
}

void discovery_timeout_cb(const struct bt_br_discovery_result *results,
		size_t count)
{
	char addr[BT_ADDR_LE_STR_LEN];
	const uint8_t *eir;
	size_t len = sizeof(results->eir);
	static uint8_t temp[240];

	for (size_t i = 0; i < count; i++) {
		bt_addr_to_str(&results[i].addr, addr, sizeof(addr));
		PRINTF("Monitor: Device[%d]: %s, rssi %d, cod 0x%X%X%X", i, addr, results[i].rssi,
		       results[i].cod[0], results[i].cod[1], results[i].cod[2]);

		eir = results[i].eir;

		while ((eir[0] > 2) && (len > eir[0])) {
			switch (eir[1]) {
			case BT_DATA_NAME_SHORTENED:
			case BT_DATA_NAME_COMPLETE:
				memcpy(temp, &eir[2], eir[0] - 1);
				temp[eir[0] - 1] = '\0'; /* Set end flag */
				PRINTF(", name %s", temp);
				break;
			}
			len = len - eir[0] - 1;
			eir = eir + eir[0] + 1;
		}
		PRINTF("\n");
	}
}

static struct bt_br_discovery_cb discovery_cb = {
	.recv = discovery_recv_cb,
	.timeout = discovery_timeout_cb,
};

static int cmd_discovery_cb(const struct shell *sh, size_t argc, char *argv[])
{
	const char *action;

	action = argv[1];
	if (!strcmp(action, "on")) {
		bt_br_discovery_cb_register(&discovery_cb);

		shell_print(sh, "Discovery cb register");
	} else if (!strcmp(action, "off")) {
		bt_br_discovery_cb_unregister(&discovery_cb);

		shell_print(sh, "Discovery cb unregister");
	} else {
		shell_help(sh);
	}

	return 0;
}

#define HELP_NONE "[none]"
#define HELP_ADDR_LE "<address: XX:XX:XX:XX:XX:XX> <type: (public|random)>"

SHELL_STATIC_SUBCMD_SET_CREATE(br_cmds,
	SHELL_CMD_ARG(auth-pincode, NULL, "<pincode>", cmd_auth_pincode, 2, 0),
	SHELL_CMD_ARG(connect, NULL, "<address>", cmd_connect, 2, 0),
	SHELL_CMD_ARG(discovery, NULL,
		      "<value: on, off> [length: 1-48] [mode: limited]",
		      cmd_discovery, 2, 2),
	SHELL_CMD_ARG(iscan, NULL, "<value: on, off>", cmd_discoverable, 2, 0),
	SHELL_CMD_ARG(l2cap-register, NULL, "<psm>", cmd_l2cap_register, 2, 0),
#if (defined(CONFIG_BT_L2CAP_IFRAME_SUPPORT) && (CONFIG_BT_L2CAP_IFRAME_SUPPORT > 0U))
	SHELL_CMD_ARG(l2cap-register-mode, NULL, "<psm> <mode:\r\n\
                                    3. Enhanced Retransmission mode\r\n\
                                    4. Streaming mode>", cmd_l2cap_register_modes, 3, 0),
#endif
	SHELL_CMD_ARG(l2cap-connect, NULL, "<psm>", cmd_l2cap_connect, 2, 0),
	SHELL_CMD_ARG(l2cap-disconnect, NULL, HELP_NONE, cmd_l2cap_disconnect, 1, 0),
#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED> 0))
	SHELL_CMD_ARG(ecred-register, NULL, "<psm> [sec_level]", cmd_ecred_register, 2, 1),
	SHELL_CMD_ARG(ecred-connect, NULL, "<psm (hex)> [sec_level (dec)]",
		cmd_ecred_connect, 2, 1),
	SHELL_CMD_ARG(ecred-reconfigure, NULL, "<mtu (dec)>",
		cmd_ecred_reconfigure, 1, 1),
#endif /* CONFIG_BT_L2CAP_ECRED */
	SHELL_CMD_ARG(l2cap-send, NULL, "<number of packets>", cmd_l2cap_send, 2, 0),
	SHELL_CMD_ARG(oob, NULL, HELP_NONE, cmd_oob, 1, 0),
	SHELL_CMD_ARG(pscan, NULL, "<value: on, off>", cmd_connectable, 2, 0),
	SHELL_CMD_ARG(sdp-find, NULL, "<HFPAG/A2SRC/PBAP_PCE>", cmd_sdp_find_record, 2, 0),
	SHELL_CMD_ARG(discovery-cb-register, NULL, "<value: on, off>", cmd_discovery_cb, 2, 0),
	SHELL_SUBCMD_SET_END
);

static int cmd_br(const struct shell *sh, size_t argc, char **argv)
{
	if (argc == 1) {
		shell_help(sh);
		/* shell returns 1 when help is printed */
		return SHELL_CMD_HELP_PRINTED;
	}

	shell_error(sh, "%s unknown parameter: %s", argv[0], argv[1]);

	return -ENOEXEC;
}

SHELL_CMD_ARG_REGISTER(br, &br_cmds, "Bluetooth BR/EDR shell commands", cmd_br,
		       1, 1);

void bt_ShellBrEdrInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandbr))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandbr.pcCommand);
    }
}
#endif /* CONFIG_BT_CLASSIC */
