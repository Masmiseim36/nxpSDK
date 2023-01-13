/* hci_core.c - HCI core Bluetooth handling */

/*
 * Copyright 2021 NXP
 * Copyright (c) 2017-2021 Nordic Semiconductor ASA
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>
#include <string.h>
#include <stdio.h>
#include <errno/errno.h>
#include <sys/atomic.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <sys/byteorder.h>


#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_vs.h>

#include "BT_common.h"
#include "BT_version.h"
#include "BT_hci_api.h"
#include "BT_smp_api.h"
#include "BT_sm_api.h"
#include "smp_pl.h"
#include "sco_audio_pl.h"

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_HCI_CORE)
#define LOG_MODULE_NAME bt_hci_core
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#include "bt_pal_rpa.h"
#include "bt_pal_keys.h"
#include "bt_pal_monitor.h"
#include "bt_pal_hci_core.h"
#include "bt_pal_hci_ecc.h"
#if ( (defined(CONFIG_BT_ECC) && ((CONFIG_BT_ECC) > 0U)) || (defined(CONFIG_BT_TINYCRYPT_ECC) && ((CONFIG_BT_TINYCRYPT_ECC) > 0U)) )
#include "bt_pal_ecc.h"
#endif

#include "bt_pal_conn_internal.h"
#include "bt_pal_l2cap_internal.h"
#include "bt_pal_gatt_internal.h"
#include "bt_pal_smp.h"
#include "bt_pal_crypto_internal.h"
#include "bt_pal_settings.h"
#include "bt_pal_id.h"
#include "bt_pal_scan.h"
#include "bt_pal_adv.h"
#if (defined(CONFIG_BT_ISO) && ((CONFIG_BT_ISO) > 0))
#include "bt_pal_iso_internal.h"
#endif

#include "bt_ble_settings.h"

#include "controller.h"
#include "bt_ble_platform.h"

#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0U))
#include "bt_pal_br.h"
#endif

#define HCI_CMD_TIMEOUT      BT_SECONDS(10)

static void hci_tx_thread(void *param);

static OSA_TASK_HANDLE_DEFINE(tx_thread_data);
static OSA_TASK_DEFINE( hci_tx_thread, CONFIG_BT_HCI_TX_PRIO, 1, CONFIG_BT_HCI_TX_STACK_SIZE, 0);

static void hci_rx_thread(void *param);

static OSA_TASK_HANDLE_DEFINE(rx_thread_data);
static OSA_TASK_DEFINE( hci_rx_thread, CONFIG_BT_RX_PRIO, 1, CONFIG_BT_RX_STACK_SIZE, 0);

static void init_work(struct k_work *work);

#if (defined(CONFIG_BT_PRIVACY) && (CONFIG_BT_PRIVACY > 0)) || \
    (defined(CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC) && (CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC > 0))
struct _bt_dev bt_dev = {
#if (defined(CONFIG_BT_PRIVACY) && (CONFIG_BT_PRIVACY > 0))
	.rpa_timeout   = CONFIG_BT_RPA_TIMEOUT,
#endif
#if (defined(CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC) && (CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC > 0))
	.appearance = CONFIG_BT_DEVICE_APPEARANCE,
#endif
};
#else
struct _bt_dev bt_dev;
#endif

static bt_ready_cb_t ready_cb;

#if (defined(CONFIG_BT_HCI_VS_EVT_USER) && ((CONFIG_BT_HCI_VS_EVT_USER) > 0U))
static bt_hci_vnd_evt_cb_t *hci_vnd_evt_cb;
#endif /* CONFIG_BT_HCI_VS_EVT_USER */

static void hci_cmd_complete_rsp_handlers(struct net_buf *buf);

static bool send_cmd(void);

struct cmd_data {
	/** HCI status of the command completion */
	uint8_t  status;

	/** The command OpCode that the buffer contains */
	uint16_t opcode;

	/** The state to update when command completes with success. */
	struct bt_hci_cmd_state_set *state;

	/** Used by bt_hci_cmd_send_sync. */
	osa_semaphore_handle_t sync;
	OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreHandle);
};

static struct cmd_data cmd_data[CONFIG_BT_BUF_CMD_TX_COUNT];

#define cmd(buf) (&cmd_data[net_buf_id(buf)])
#define acl(buf) ((struct acl_data *)net_buf_user_data(buf))

void bt_hci_cmd_state_set_init(struct net_buf *buf,
			       struct bt_hci_cmd_state_set *state,
			       atomic_t *target, int bit, bool val)
{
	state->target = target;
	state->bit = bit;
	state->val = val;
	cmd(buf)->state = state;
}

/* HCI command buffers. Derive the needed size from both Command and Event
 * buffer length since the buffer is also used for the response event i.e
 * command complete or command status.
 */
#define CMD_BUF_SIZE MAX(BT_BUF_EVT_RX_SIZE, BT_BUF_CMD_TX_SIZE)
NET_BUF_POOL_FIXED_DEFINE(hci_cmd_pool, CONFIG_BT_BUF_CMD_TX_COUNT,
			  CMD_BUF_SIZE, NULL);

struct cmd_complete_handler {
	uint16_t opcode;
	uint8_t min_len;
	void (*handler)(struct net_buf *buf);
};

#define CMD_COMPLETE_HANDLER(_opcode, _handler, _min_len) \
{ \
	.opcode = (_opcode),   \
	.min_len = (_min_len), \
	.handler = (_handler), \
}

struct event_handler {
	uint8_t event;
	uint8_t min_len;
	void (*handler)(struct net_buf *buf);
};

#define EVENT_HANDLER(_evt, _handler, _min_len) \
{ \
	.event = _evt, \
	.handler = _handler, \
	.min_len = _min_len, \
}

static int handle_event_common(uint8_t event, struct net_buf *buf,
			       const struct event_handler *handlers, size_t num_handlers)
{
	size_t i;

	for (i = 0; i < num_handlers; i++) {
		const struct event_handler *handler = &handlers[i];

		if (handler->event != event) {
			continue;
		}

		if (buf->len < handler->min_len) {
			BT_ERR("Too small (%u bytes) event 0x%02x",
			       buf->len, event);
			return -EINVAL;
		}

		handler->handler(buf);
		return 0;
	}

	return -EOPNOTSUPP;
}

static void handle_event(uint8_t event, struct net_buf *buf, const struct event_handler *handlers,
			 size_t num_handlers)
{
	int err;

	err = handle_event_common(event, buf, handlers, num_handlers);
	if (err == -EOPNOTSUPP) {
		BT_WARN("Unhandled event 0x%02x len %u: %s", event, buf->len,
			bt_hex(buf->data, buf->len));
	}

	/* Other possible errors are handled by handle_event_common function */
}

static void handle_vs_event(uint8_t event, struct net_buf *buf,
			    const struct event_handler *handlers, size_t num_handlers)
{
	int err;

	err = handle_event_common(event, buf, handlers, num_handlers);
	if (err == -EOPNOTSUPP) {
		BT_WARN("Unhandled vendor-specific event: %s", bt_hex(buf->data, buf->len));
	}

	/* Other possible errors are handled by handle_event_common function */
}

#if (defined(CONFIG_BT_HCI_ACL_FLOW_CONTROL) && ((CONFIG_BT_HCI_ACL_FLOW_CONTROL) > 0U))
void bt_hci_host_num_completed_packets(struct net_buf *buf)
{

	struct bt_hci_cp_host_num_completed_packets *cp;
	uint16_t handle = acl(buf)->handle;
	struct bt_hci_handle_count *hc;
	struct bt_conn *conn;
	uint8_t index = acl(buf)->index;

	net_buf_destroy(buf);

	/* Do nothing if controller to host flow control is not supported */
	if (!BT_CMD_TEST(bt_dev.supported_commands, 10, 5)) {
		return;
	}

	conn = bt_conn_lookup_index(index);
	if (!conn) {
		BT_WARN("Unable to look up conn with index 0x%02x", index);
		return;
	}

	if (conn->state != BT_CONN_CONNECTED &&
	    conn->state != BT_CONN_DISCONNECTING) {
		BT_WARN("Not reporting packet for non-connected conn");
		bt_conn_unref(conn);
		return;
	}

	bt_conn_unref(conn);

	BT_DBG("Reporting completed packet for handle %u", handle);

	buf = bt_hci_cmd_create(BT_HCI_OP_HOST_NUM_COMPLETED_PACKETS,
				sizeof(*cp) + sizeof(*hc));
	if (!buf) {
		BT_ERR("Unable to allocate new HCI command");
		return;
	}

	cp = net_buf_add(buf, sizeof(*cp));
	cp->num_handles = sys_cpu_to_le16(1);

	hc = net_buf_add(buf, sizeof(*hc));
	hc->handle = sys_cpu_to_le16(handle);
	hc->count  = sys_cpu_to_le16(1);

	bt_hci_cmd_send(BT_HCI_OP_HOST_NUM_COMPLETED_PACKETS, buf);
}
#endif /* CONFIG_BT_HCI_ACL_FLOW_CONTROL */

static void bt_set_send_new_command(void)
{
    osa_status_t ret;
	ret = OSA_EventSet(bt_dev.new_event, BT_DEV_SEND_COMMAND);
    assert(KOSA_StatusSuccess == ret);
    (void)ret;
}

#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))
void bt_set_send_iso(struct bt_conn *conn)
{
    osa_status_t ret;

    ret = OSA_MsgQPut(bt_dev.iso_conn, &conn);
    assert(KOSA_StatusSuccess == ret);

	ret = OSA_EventSet(bt_dev.new_event, BT_DEV_SEND_ISO);
    assert(KOSA_StatusSuccess == ret);

	(void)ret;
}
#endif /* CONFIG_BT_ISO */

void bt_set_conn_changed(struct bt_conn *conn)
{
    osa_status_t ret;

    ret = OSA_MsgQPut(bt_dev.conn_changed, &conn);
    assert(KOSA_StatusSuccess == ret);

	ret = OSA_EventSet(bt_dev.new_event, BT_DEV_CONN_CHANGED);
    assert(KOSA_StatusSuccess == ret);

    (void)ret;
}

struct net_buf *bt_hci_cmd_create(uint16_t opcode, uint8_t param_len)
{
	struct bt_hci_cmd_hdr *hdr;
	struct net_buf *buf;

	BT_DBG("opcode 0x%04x param_len %u", opcode, param_len);

	buf = net_buf_alloc(&hci_cmd_pool, osaWaitForever_c);
	__ASSERT_NO_MSG(buf);

	BT_DBG("buf %p", buf);

	net_buf_reserve(buf, BT_BUF_RESERVE);

	bt_buf_set_type(buf, BT_BUF_CMD);

	cmd(buf)->opcode = opcode;
	cmd(buf)->sync = NULL;
	cmd(buf)->state = NULL;

	hdr = (struct bt_hci_cmd_hdr *)net_buf_add(buf, sizeof(*hdr));
	hdr->opcode = sys_cpu_to_le16(opcode);
	hdr->param_len = param_len;

	return buf;
}

int bt_hci_cmd_send(uint16_t opcode, struct net_buf *buf)
{
	if (!buf) {
		buf = bt_hci_cmd_create(opcode, 0);
		if (!buf) {
			return -ENOBUFS;
		}
	}

	BT_DBG("opcode 0x%04x len %u", opcode, buf->len);

	/* Host Number of Completed Packets can ignore the ncmd value
	 * and does not generate any cmd complete/status events.
	 */
	if (opcode == BT_HCI_OP_HOST_NUM_COMPLETED_PACKETS) {
		int err;

		err = bt_send(buf);
		if (err) {
			BT_ERR("Unable to send to driver (err %d)", err);
			net_buf_unref(buf);
		}

		return err;
	}

	net_buf_put(bt_dev.cmd_tx_queue, buf);
	bt_set_send_new_command();

	return 0;
}

int bt_hci_cmd_send_sync(uint16_t opcode, struct net_buf *buf,
			 struct net_buf **rsp)
{
	osa_status_t ret;
	uint8_t status;

	if (!buf) {
		buf = bt_hci_cmd_create(opcode, 0);
		if (!buf) {
			return -ENOBUFS;
		}
	}

	BT_DBG("buf %p opcode 0x%04x len %u", buf, opcode, buf->len);

	ret = OSA_SemaphoreCreate((osa_semaphore_handle_t)(cmd(buf)->semaphoreHandle), 0);
	if (KOSA_StatusSuccess != ret)
	{
		return -ENOBUFS;
	}

	cmd(buf)->sync = (osa_semaphore_handle_t)(cmd(buf)->semaphoreHandle);

	net_buf_put(bt_dev.cmd_tx_queue, net_buf_ref(buf));

	bt_set_send_new_command();

	ret = OSA_SemaphoreWait(cmd(buf)->sync, HCI_CMD_TIMEOUT);
#if 0
	assert(KOSA_StatusSuccess == ret);
#endif
	(void)OSA_SemaphoreDestroy(cmd(buf)->sync);

	status = cmd(buf)->status;
	if (status) {
		BT_WARN("opcode 0x%04x status 0x%02x", opcode, status);
		net_buf_unref(buf);

		switch (status) {
		case BT_HCI_ERR_CONN_LIMIT_EXCEEDED:
			return -ECONNREFUSED;
		default:
			return -EIO;
		}
	}

	BT_DBG("rsp %p opcode 0x%04x len %u", buf, opcode, buf->len);

	if (rsp) {
		*rsp = buf;
	} else {
		net_buf_unref(buf);
	}

	return 0;
}

int bt_hci_le_rand(void *buffer, size_t len)
{
	struct bt_hci_rp_le_rand *rp;
	struct net_buf *rsp;
	size_t count;
	int err;

	/* Check first that HCI_LE_Rand is supported */
	if (!BT_CMD_TEST(bt_dev.supported_commands, 27, 7)) {
		return -ENOTSUP;
	}

	while (len > 0) {
		/* Number of bytes to fill on this iteration */
		count = MIN(len, sizeof(rp->rand));
		/* Request the next 8 bytes over HCI */
		err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_RAND, NULL, &rsp);
		if (err) {
			return err;
		}
		/* Copy random data into buffer */
		rp = (void *)rsp->data;
		memcpy(buffer, rp->rand, count);

		net_buf_unref(rsp);
		buffer = (uint8_t *)buffer + count;
		len -= count;
	}

	return 0;
}

static int hci_le_read_max_data_len(uint16_t *tx_octets, uint16_t *tx_time)
{
	struct bt_hci_rp_le_read_max_data_len *rp;
	struct net_buf *rsp;
	int err;

	err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_READ_MAX_DATA_LEN, NULL, &rsp);
	if (err) {
		BT_ERR("Failed to read DLE max data len");
		return err;
	}

	rp = (struct bt_hci_rp_le_read_max_data_len *)rsp->data;
	*tx_octets = sys_le16_to_cpu(rp->max_tx_octets);
	*tx_time = sys_le16_to_cpu(rp->max_tx_time);
	net_buf_unref(rsp);

	return 0;
}

uint8_t bt_get_phy(uint8_t hci_phy)
{
	switch (hci_phy) {
	case BT_HCI_LE_PHY_1M:
		return BT_GAP_LE_PHY_1M;
	case BT_HCI_LE_PHY_2M:
		return BT_GAP_LE_PHY_2M;
	case BT_HCI_LE_PHY_CODED:
		return BT_GAP_LE_PHY_CODED;
	default:
		return 0;
	}
}

#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0U))
static void hci_num_completed_packets(struct net_buf *buf)
{
	struct bt_hci_evt_num_completed_packets *evt = (void *)buf->data;
	int i;

	BT_DBG("num_handles %u", evt->num_handles);

	for (i = 0; i < evt->num_handles; i++) {
		uint16_t handle, count;
		struct bt_conn *conn;
		unsigned int key;

		handle = sys_le16_to_cpu(evt->h[i].handle);
		count = sys_le16_to_cpu(evt->h[i].count);

		BT_DBG("handle %u count %u", handle, count);

		key = DisableGlobalIRQ();

		conn = bt_conn_lookup_handle(handle);
		if (!conn) {
			EnableGlobalIRQ(key);
			BT_ERR("No connection for handle %u", handle);
			continue;
		}

		EnableGlobalIRQ(key);

		while (count--) {
			struct bt_conn_tx *tx;
			sys_snode_t *node;

			key = DisableGlobalIRQ();

			if (conn->pending_no_cb) {
				conn->pending_no_cb--;
				EnableGlobalIRQ(key);
				OSA_SemaphorePost(bt_conn_get_pkts(conn));
				continue;
			}

			node = sys_slist_get(&conn->tx_pending);
			EnableGlobalIRQ(key);

			if (!node) {
				BT_ERR("packets count mismatch");
				break;
			}

			tx = CONTAINER_OF(node, struct bt_conn_tx, node);

			key = DisableGlobalIRQ();
			conn->pending_no_cb = tx->pending_no_cb;
			tx->pending_no_cb = 0U;
			sys_slist_append(&conn->tx_complete, &tx->node);
			EnableGlobalIRQ(key);

			k_work_submit(&conn->tx_complete_work);
			OSA_SemaphorePost(bt_conn_get_pkts(conn));
		}

		bt_conn_unref(conn);
	}
}

static void hci_acl(struct net_buf *buf)
{
    struct bt_hci_acl_hdr_simulation *hdr;

    hdr = (struct bt_hci_acl_hdr_simulation *)&buf->data[0];

    if (NULL != hdr->handler)
    {
        hdr->handler(buf);
    }
    net_buf_unref(buf);
}

static void hci_data_buf_overflow(struct net_buf *buf)
{
	struct bt_hci_evt_data_buf_overflow *evt = (struct bt_hci_evt_data_buf_overflow *)buf->data;

	BT_WARN("Data buffer overflow (link type 0x%02x)", evt->link_type);
    (void)evt;
}

#if (defined(CONFIG_BT_CENTRAL) && ((CONFIG_BT_CENTRAL) > 0U))
static void set_phy_conn_param(const struct bt_conn *conn,
			       struct bt_hci_ext_conn_phy *phy)
{
	phy->conn_interval_min = sys_cpu_to_le16(conn->le.interval_min);
	phy->conn_interval_max = sys_cpu_to_le16(conn->le.interval_max);
	phy->conn_latency = sys_cpu_to_le16(conn->le.latency);
	phy->supervision_timeout = sys_cpu_to_le16(conn->le.timeout);

	phy->min_ce_len = 0;
	phy->max_ce_len = 0;
}

int bt_le_create_conn_ext(const struct bt_conn *conn)
{
	struct bt_hci_cp_le_ext_create_conn *cp;
	struct bt_hci_ext_conn_phy *phy;
	struct bt_hci_cmd_state_set state;
	bool use_filter = false;
	struct net_buf *buf;
	uint8_t own_addr_type;
	uint8_t num_phys;
	int err;

	if (IS_ENABLED(CONFIG_BT_FILTER_ACCEPT_LIST)) {
		use_filter = atomic_test_bit(conn->flags, BT_CONN_AUTO_CONNECT);
	}

	err = bt_id_set_create_conn_own_addr(use_filter, &own_addr_type);
	if (err) {
		return err;
	}

	num_phys = (!(bt_dev.create_param.options &
		      BT_CONN_LE_OPT_NO_1M) ? 1 : 0) +
		   ((bt_dev.create_param.options &
		      BT_CONN_LE_OPT_CODED) ? 1 : 0);

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_EXT_CREATE_CONN, sizeof(*cp) +
				num_phys * sizeof(*phy));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = (struct bt_hci_cp_le_ext_create_conn *)net_buf_add(buf, sizeof(*cp));
	(void)memset(cp, 0, sizeof(*cp));

	if (use_filter) {
		/* User Initiated procedure use fast scan parameters. */
		bt_addr_le_copy(&cp->peer_addr, BT_ADDR_LE_ANY);
		cp->filter_policy = BT_HCI_LE_CREATE_CONN_FP_FILTER;
	} else {
		const bt_addr_le_t *peer_addr = &conn->le.dst;

		if (bt_addr_le_cmp(&conn->le.resp_addr, BT_ADDR_LE_ANY)) {
			/* Host resolving is used, use the RPA directly. */
			peer_addr = &conn->le.resp_addr;
			BT_DBG("Using resp_addr %s", bt_addr_le_str(peer_addr));
		}

		bt_addr_le_copy(&cp->peer_addr, peer_addr);
		cp->filter_policy = BT_HCI_LE_CREATE_CONN_FP_NO_FILTER;
	}

	cp->own_addr_type = own_addr_type;
	cp->phys = 0;

	if (!(bt_dev.create_param.options & BT_CONN_LE_OPT_NO_1M)) {
		cp->phys |= BT_HCI_LE_EXT_SCAN_PHY_1M;
		phy = (struct bt_hci_ext_conn_phy *)net_buf_add(buf, sizeof(*phy));
		phy->scan_interval = sys_cpu_to_le16(
			bt_dev.create_param.interval);
		phy->scan_window = sys_cpu_to_le16(
			bt_dev.create_param.window);
		set_phy_conn_param(conn, phy);
	}

	if (bt_dev.create_param.options & BT_CONN_LE_OPT_CODED) {
		cp->phys |= BT_HCI_LE_EXT_SCAN_PHY_CODED;
		phy = (struct bt_hci_ext_conn_phy *)net_buf_add(buf, sizeof(*phy));
		phy->scan_interval = sys_cpu_to_le16(
			bt_dev.create_param.interval_coded);
		phy->scan_window = sys_cpu_to_le16(
			bt_dev.create_param.window_coded);
		set_phy_conn_param(conn, phy);
	}

	bt_hci_cmd_state_set_init(buf, &state, bt_dev.flags,
				  BT_DEV_INITIATING, true);

	return bt_hci_cmd_send_sync(BT_HCI_OP_LE_EXT_CREATE_CONN, buf, NULL);
}

static int bt_le_create_conn_legacy(const struct bt_conn *conn)
{
	struct bt_hci_cp_le_create_conn *cp;
	struct bt_hci_cmd_state_set state;
	bool use_filter = false;
	struct net_buf *buf;
	uint8_t own_addr_type;
	int err;

	if (IS_ENABLED(CONFIG_BT_FILTER_ACCEPT_LIST)) {
		use_filter = atomic_test_bit(conn->flags, BT_CONN_AUTO_CONNECT);
	}

	err = bt_id_set_create_conn_own_addr(use_filter, &own_addr_type);
	if (err) {
		return err;
	}

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_CREATE_CONN, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = (struct bt_hci_cp_le_create_conn *)net_buf_add(buf, sizeof(*cp));
	memset(cp, 0, sizeof(*cp));
	cp->own_addr_type = own_addr_type;

	if (use_filter) {
		/* User Initiated procedure use fast scan parameters. */
		bt_addr_le_copy(&cp->peer_addr, BT_ADDR_LE_ANY);
		cp->filter_policy = BT_HCI_LE_CREATE_CONN_FP_FILTER;
	} else {
		const bt_addr_le_t *peer_addr = &conn->le.dst;

		if (bt_addr_le_cmp(&conn->le.resp_addr, BT_ADDR_LE_ANY)) {
			/* Host resolving is used, use the RPA directly. */
			peer_addr = &conn->le.resp_addr;
			BT_DBG("Using resp_addr %s", bt_addr_le_str(peer_addr));
		}

		bt_addr_le_copy(&cp->peer_addr, peer_addr);
		cp->filter_policy = BT_HCI_LE_CREATE_CONN_FP_NO_FILTER;
	}

	cp->scan_interval = sys_cpu_to_le16(bt_dev.create_param.interval);
	cp->scan_window = sys_cpu_to_le16(bt_dev.create_param.window);

	cp->conn_interval_min = sys_cpu_to_le16(conn->le.interval_min);
	cp->conn_interval_max = sys_cpu_to_le16(conn->le.interval_max);
	cp->conn_latency = sys_cpu_to_le16(conn->le.latency);
	cp->supervision_timeout = sys_cpu_to_le16(conn->le.timeout);

	bt_hci_cmd_state_set_init(buf, &state, bt_dev.flags,
				  BT_DEV_INITIATING, true);

	return bt_hci_cmd_send_sync(BT_HCI_OP_LE_CREATE_CONN, buf, NULL);
}

int bt_le_create_conn(const struct bt_conn *conn)
{
	if (IS_ENABLED(CONFIG_BT_EXT_ADV) &&
	    BT_DEV_FEAT_LE_EXT_ADV(bt_dev.le.features)) {
		return bt_le_create_conn_ext(conn);
	}

	return bt_le_create_conn_legacy(conn);
}

int bt_le_create_conn_cancel(void)
{
	struct net_buf *buf;
	struct bt_hci_cmd_state_set state;

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_CREATE_CONN_CANCEL, 0);

	bt_hci_cmd_state_set_init(buf, &state, bt_dev.flags,
				  BT_DEV_INITIATING, false);

	return bt_hci_cmd_send_sync(BT_HCI_OP_LE_CREATE_CONN_CANCEL, buf, NULL);
}
#endif /* CONFIG_BT_CENTRAL */

int bt_hci_disconnect(uint16_t handle, uint8_t reason)
{
	struct net_buf *buf;
	struct bt_hci_cp_disconnect *disconn;

	buf = bt_hci_cmd_create(BT_HCI_OP_DISCONNECT, sizeof(*disconn));
	if (!buf) {
		return -ENOBUFS;
	}

	disconn = (struct bt_hci_cp_disconnect *)net_buf_add(buf, sizeof(*disconn));
	disconn->handle = sys_cpu_to_le16(handle);
	disconn->reason = reason;

	return bt_hci_cmd_send_sync(BT_HCI_OP_DISCONNECT, buf, NULL);
}

static uint16_t disconnected_handles[CONFIG_BT_MAX_CONN];
static void conn_handle_disconnected(uint16_t handle)
{
	for (int i = 0; i < ARRAY_SIZE(disconnected_handles); i++) {
		if (!disconnected_handles[i]) {
			/* Use invalid connection handle bits so that connection
			 * handle 0 can be used as a valid non-zero handle.
			 */
			disconnected_handles[i] = ~BT_ACL_HANDLE_MASK | handle;
		}
	}
}

static bool conn_handle_is_disconnected(uint16_t handle)
{
	handle |= ~BT_ACL_HANDLE_MASK;

	for (int i = 0; i < ARRAY_SIZE(disconnected_handles); i++) {
		if (disconnected_handles[i] == handle) {
			disconnected_handles[i] = 0;
			return true;
		}
	}

	return false;
}

static void hci_disconn_complete_prio(struct net_buf *buf)
{
	struct bt_hci_evt_disconn_complete *evt = (struct bt_hci_evt_disconn_complete *)buf->data;
	uint16_t handle = sys_le16_to_cpu(evt->handle);
	struct bt_conn *conn;

	BT_DBG("status 0x%02x handle %u reason 0x%02x", evt->status, handle,
	       evt->reason);

	if (evt->status) {
		return;
	}

	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		/* Priority disconnect complete event received before normal
		 * connection complete event.
		 */
		conn_handle_disconnected(handle);
		return;
	}

	bt_conn_set_state(conn, BT_CONN_DISCONNECT_COMPLETE);
	bt_conn_unref(conn);
}

static void hci_disconn_complete(struct net_buf *buf)
{
	struct bt_hci_evt_disconn_complete *evt = (struct bt_hci_evt_disconn_complete *)buf->data;
	uint16_t handle = sys_le16_to_cpu(evt->handle);
	struct bt_conn *conn;

	BT_DBG("status 0x%02x handle %u reason 0x%02x", evt->status, handle,
	       evt->reason);

	if (evt->status) {
		return;
	}

	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		BT_ERR("Unable to look up conn with handle %u", handle);
		return;
	}

	conn->err = evt->reason;

	bt_conn_set_state(conn, BT_CONN_DISCONNECTED);

	if (conn->type != BT_CONN_TYPE_LE) {
#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
		if (conn->type == BT_CONN_TYPE_SCO) {
			bt_sco_cleanup(conn);
			return;
		}
		/*
		 * If only for one connection session bond was set, clear keys
		 * database row for this connection.
		 */
		if (conn->type == BT_CONN_TYPE_BR &&
		    atomic_test_and_clear_bit(conn->flags, BT_CONN_BR_NOBOND)) {
			bt_keys_link_key_clear(conn->br.link_key);
		}
#endif
		bt_conn_unref(conn);
		return;
	}

#if ((defined(CONFIG_BT_CENTRAL) && (CONFIG_BT_CENTRAL > 0U)) && !(defined(CONFIG_BT_FILTER_ACCEPT_LIST) && (CONFIG_BT_FILTER_ACCEPT_LIST > 0U)))
	if (atomic_test_bit(conn->flags, BT_CONN_AUTO_CONNECT)) {
		bt_conn_set_state(conn, BT_CONN_CONNECTING_SCAN);
		bt_le_scan_update(false);
	}
#endif /* defined(CONFIG_BT_CENTRAL) && !defined(CONFIG_BT_FILTER_ACCEPT_LIST) */

	bt_conn_unref(conn);
}

static int hci_le_read_remote_features(struct bt_conn *conn)
{
	struct bt_hci_cp_le_read_remote_features *cp;
	struct net_buf *buf;

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_READ_REMOTE_FEATURES,
				sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = (struct bt_hci_cp_le_read_remote_features *)net_buf_add(buf, sizeof(*cp));
	cp->handle = sys_cpu_to_le16(conn->handle);
	return bt_hci_cmd_send_sync(BT_HCI_OP_LE_READ_REMOTE_FEATURES, buf, NULL);
}

static int hci_read_remote_version(struct bt_conn *conn)
{
	struct bt_hci_cp_read_remote_version_info *cp;
	struct net_buf *buf;

	if (conn->state != BT_CONN_CONNECTED) {
		return -ENOTCONN;
	}

	/* Remote version cannot change. */
	if (atomic_test_bit(conn->flags, BT_CONN_AUTO_VERSION_INFO)) {
		return 0;
	}

	buf = bt_hci_cmd_create(BT_HCI_OP_READ_REMOTE_VERSION_INFO,
				sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = (struct bt_hci_cp_read_remote_version_info *)net_buf_add(buf, sizeof(*cp));
	cp->handle = sys_cpu_to_le16(conn->handle);

	return bt_hci_cmd_send_sync(BT_HCI_OP_READ_REMOTE_VERSION_INFO, buf,
				    NULL);
}

/* LE Data Length Change Event is optional so this function just ignore
 * error and stack will continue to use default values.
 */
int bt_le_set_data_len(struct bt_conn *conn, uint16_t tx_octets, uint16_t tx_time)
{
	struct bt_hci_cp_le_set_data_len *cp;
	struct net_buf *buf;

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_DATA_LEN, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = (struct bt_hci_cp_le_set_data_len *)net_buf_add(buf, sizeof(*cp));
	cp->handle = sys_cpu_to_le16(conn->handle);
	cp->tx_octets = sys_cpu_to_le16(tx_octets);
	cp->tx_time = sys_cpu_to_le16(tx_time);

	return bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_DATA_LEN, buf, NULL);
}

#if (defined(CONFIG_BT_USER_PHY_UPDATE) && ((CONFIG_BT_USER_PHY_UPDATE) > 0U))
static int hci_le_read_phy(struct bt_conn *conn)
{
	struct bt_hci_cp_le_read_phy *cp;
	struct bt_hci_rp_le_read_phy *rp;
	struct net_buf *buf, *rsp;
	int err;

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_READ_PHY, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = (struct bt_hci_cp_le_read_phy *)net_buf_add(buf, sizeof(*cp));
	cp->handle = sys_cpu_to_le16(conn->handle);

	err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_READ_PHY, buf, &rsp);
	if (err) {
		return err;
	}

	rp = (struct bt_hci_rp_le_read_phy *)rsp->data;
	conn->le.phy.tx_phy = bt_get_phy(rp->tx_phy);
	conn->le.phy.rx_phy = bt_get_phy(rp->rx_phy);
	net_buf_unref(rsp);

	return 0;
}
#endif /* defined(CONFIG_BT_USER_PHY_UPDATE) */

int bt_le_set_phy(struct bt_conn *conn, uint8_t all_phys,
		  uint8_t pref_tx_phy, uint8_t pref_rx_phy, uint8_t phy_opts)
{
	struct bt_hci_cp_le_set_phy *cp;
	struct net_buf *buf;

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_PHY, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = (struct bt_hci_cp_le_set_phy *)net_buf_add(buf, sizeof(*cp));
	cp->handle = sys_cpu_to_le16(conn->handle);
	cp->all_phys = all_phys;
	cp->tx_phys = pref_tx_phy;
	cp->rx_phys = pref_rx_phy;
	cp->phy_opts = phy_opts;

	return bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_PHY, buf, NULL);
}

static struct bt_conn *find_pending_connect(uint8_t role, bt_addr_le_t *peer_addr)
{
	struct bt_conn *conn;

	/*
	 * Make lookup to check if there's a connection object in
	 * CONNECT or CONNECT_AUTO state associated with passed peer LE address.
	 */
	if (IS_ENABLED(CONFIG_BT_CENTRAL) && role == BT_HCI_ROLE_CENTRAL) {
		conn = bt_conn_lookup_state_le(BT_ID_DEFAULT, peer_addr,
					       BT_CONN_CONNECTING);
		if (IS_ENABLED(CONFIG_BT_FILTER_ACCEPT_LIST) && !conn) {
			conn = bt_conn_lookup_state_le(BT_ID_DEFAULT,
						       BT_ADDR_LE_NONE,
						       BT_CONN_CONNECTING_AUTO);
		}

		return conn;
	}

	if (IS_ENABLED(CONFIG_BT_PERIPHERAL) && role == BT_HCI_ROLE_PERIPHERAL) {
		conn = bt_conn_lookup_state_le(bt_dev.adv_conn_id, peer_addr,
					       BT_CONN_CONNECTING_DIR_ADV);
		if (!conn) {
			conn = bt_conn_lookup_state_le(bt_dev.adv_conn_id,
						       BT_ADDR_LE_NONE,
						       BT_CONN_CONNECTING_ADV);
		}

		return conn;
	}

	return NULL;
}

/* We don't want the application to get a PHY update callback upon connection
 * establishment on 2M PHY. Therefore we must prevent issuing LE Set PHY
 * in this scenario.
 */
static bool skip_auto_phy_update_on_conn_establishment(struct bt_conn *conn)
{
#if (defined(CONFIG_BT_USER_PHY_UPDATE) && (CONFIG_BT_USER_PHY_UPDATE > 0))
	if (IS_ENABLED(CONFIG_BT_AUTO_PHY_UPDATE) &&
	    IS_ENABLED(CONFIG_BT_EXT_ADV) &&
	    BT_DEV_FEAT_LE_EXT_ADV(bt_dev.le.features)) {
		if (conn->le.phy.tx_phy == BT_HCI_LE_PHY_2M &&
		    conn->le.phy.rx_phy == BT_HCI_LE_PHY_2M) {
			return true;
		}
	}
#else
	ARG_UNUSED(conn);
#endif /* defined(CONFIG_BT_USER_PHY_UPDATE) */

	return false;
}

static void conn_auto_initiate(struct bt_conn *conn)
{
	int err;

	if (conn->state != BT_CONN_CONNECTED) {
		/* It is possible that connection was disconnected directly from
		 * connected callback so we must check state before doing
		 * connection parameters update.
		 */
		return;
	}

	if (!atomic_test_bit(conn->flags, BT_CONN_AUTO_FEATURE_EXCH) &&
	    ((conn->role == BT_HCI_ROLE_CENTRAL) ||
	     BT_FEAT_LE_PER_INIT_FEAT_XCHG(bt_dev.le.features))) {
		err = hci_le_read_remote_features(conn);
		if (err) {
			BT_ERR("Failed read remote features (%d)", err);
		}
	}

	if (IS_ENABLED(CONFIG_BT_REMOTE_VERSION) &&
	    !atomic_test_bit(conn->flags, BT_CONN_AUTO_VERSION_INFO)) {
		err = hci_read_remote_version(conn);
		if (err) {
			BT_ERR("Failed read remote version (%d)", err);
		}
	}

	if (IS_ENABLED(CONFIG_BT_AUTO_PHY_UPDATE) &&
	    BT_FEAT_LE_PHY_2M(bt_dev.le.features) &&
	    !skip_auto_phy_update_on_conn_establishment(conn)) {
		err = bt_le_set_phy(conn, 0U, BT_HCI_LE_PHY_PREFER_2M,
				    BT_HCI_LE_PHY_PREFER_2M,
				    BT_HCI_LE_PHY_CODED_ANY);
		if (err) {
			BT_ERR("Failed LE Set PHY (%d)", err);
		}
	}

	if (IS_ENABLED(CONFIG_BT_AUTO_DATA_LEN_UPDATE) &&
	    BT_FEAT_LE_DLE(bt_dev.le.features)) {
#if 0
		if (IS_BT_QUIRK_NO_AUTO_DLE(&bt_dev)) {
#endif
			uint16_t tx_octets, tx_time;

			err = hci_le_read_max_data_len(&tx_octets, &tx_time);
			if (!err) {
				err = bt_le_set_data_len(conn,
						tx_octets, tx_time);
				if (err) {
					BT_ERR("Failed to set data len (%d)", err);
				}
			}
#if 0
		} else {
			/* No need to auto-initiate DLE procedure.
			 * It is done by the controller.
			 */
		}
#endif
	}
}

static void le_conn_complete_cancel(void)
{
	struct bt_conn *conn;

	/* Handle create connection cancel.
	 *
	 * There is no need to check ID address as only one
	 * connection in central role can be in pending state.
	 */
	conn = find_pending_connect(BT_HCI_ROLE_CENTRAL, NULL);
	if (!conn) {
		BT_ERR("No pending central connection");
		return;
	}

	conn->err = BT_HCI_ERR_UNKNOWN_CONN_ID;

	/* Handle cancellation of outgoing connection attempt. */
	if (!IS_ENABLED(CONFIG_BT_FILTER_ACCEPT_LIST)) {
		/* We notify before checking autoconnect flag
		 * as application may choose to change it from
		 * callback.
		 */
		bt_conn_set_state(conn, BT_CONN_DISCONNECTED);
		/* Check if device is marked for autoconnect. */
		if (atomic_test_bit(conn->flags, BT_CONN_AUTO_CONNECT)) {
			/* Restart passive scanner for device */
			bt_conn_set_state(conn, BT_CONN_CONNECTING_SCAN);
		}
	} else {
		if (atomic_test_bit(conn->flags, BT_CONN_AUTO_CONNECT)) {
			/* Restart FAL initiator after RPA timeout. */
			bt_le_create_conn(conn);
		} else {
			/* Create connection canceled by timeout */
			bt_conn_set_state(conn, BT_CONN_DISCONNECTED);
		}
	}

	bt_conn_unref(conn);
}

static void le_conn_complete_adv_timeout(void)
{
	if (!(IS_ENABLED(CONFIG_BT_EXT_ADV) &&
	      BT_DEV_FEAT_LE_EXT_ADV(bt_dev.le.features))) {
		struct bt_le_ext_adv *adv = bt_le_adv_lookup_legacy();
		struct bt_conn *conn;

		/* Handle advertising timeout after high duty cycle directed
		 * advertising.
		 */

		atomic_clear_bit(adv->flags, BT_ADV_ENABLED);

		if (IS_ENABLED(CONFIG_BT_EXT_ADV) &&
		    !BT_DEV_FEAT_LE_EXT_ADV(bt_dev.le.features)) {
			/* No advertising set terminated event, must be a
			 * legacy advertiser set.
			 */
			bt_le_adv_delete_legacy();
		}

		/* There is no need to check ID address as only one
		 * connection in peripheral role can be in pending state.
		 */
		conn = find_pending_connect(BT_HCI_ROLE_PERIPHERAL, NULL);
		if (!conn) {
			BT_ERR("No pending peripheral connection");
			return;
		}

		conn->err = BT_HCI_ERR_ADV_TIMEOUT;
		bt_conn_set_state(conn, BT_CONN_DISCONNECTED);

		bt_conn_unref(conn);
	}
}

static void enh_conn_complete(struct bt_hci_evt_le_enh_conn_complete *evt)
{
#if (CONFIG_BT_ID_MAX > 1) && (CONFIG_BT_EXT_ADV_MAX_ADV_SET > 1)
	if (IS_ENABLED(CONFIG_BT_PERIPHERAL) &&
		evt->role == BT_HCI_ROLE_PERIPHERAL &&
		evt->status == BT_HCI_ERR_SUCCESS &&
		(IS_ENABLED(CONFIG_BT_EXT_ADV) &&
				BT_FEAT_LE_EXT_ADV(bt_dev.le.features))) {

		/* Cache the connection complete event. Process it later.
		 * See bt_dev.cached_conn_complete.
		 */
		for (int i = 0; i < ARRAY_SIZE(bt_dev.cached_conn_complete); i++) {
			if (!bt_dev.cached_conn_complete[i].valid) {
				(void)memcpy(&bt_dev.cached_conn_complete[i].evt,
					evt,
					sizeof(struct bt_hci_evt_le_enh_conn_complete));
				bt_dev.cached_conn_complete[i].valid = true;
				return;
			}
		}

		__ASSERT(false, "No more cache entries available."
				"This should not happen by design");

		return;
	}
#endif
	bt_hci_le_enh_conn_complete(evt);
}

void bt_hci_le_enh_conn_complete(struct bt_hci_evt_le_enh_conn_complete *evt)
{
	uint16_t handle = sys_le16_to_cpu(evt->handle);
	bool is_disconnected = conn_handle_is_disconnected(handle);
	bt_addr_le_t peer_addr, id_addr;
	struct bt_conn *conn;

	BT_DBG("status 0x%02x handle %u role %u peer %s peer RPA %s",
	       evt->status, handle, evt->role, bt_addr_le_str(&evt->peer_addr),
	       bt_addr_str(&evt->peer_rpa));
	BT_DBG("local RPA %s", bt_addr_str(&evt->local_rpa));

#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
	bt_id_pending_keys_update();
#endif

	if (evt->status) {
		if (IS_ENABLED(CONFIG_BT_PERIPHERAL) &&
		    evt->status == BT_HCI_ERR_ADV_TIMEOUT) {
			le_conn_complete_adv_timeout();
			return;
		}

		if (IS_ENABLED(CONFIG_BT_CENTRAL) &&
		    evt->status == BT_HCI_ERR_UNKNOWN_CONN_ID) {
			le_conn_complete_cancel();
			bt_le_scan_update(false);
			return;
		}

		BT_WARN("Unexpected status 0x%02x", evt->status);

		return;
	}

	/* Translate "enhanced" identity address type to normal one */
	if (evt->peer_addr.type == BT_ADDR_LE_PUBLIC_ID ||
	    evt->peer_addr.type == BT_ADDR_LE_RANDOM_ID) {
		bt_addr_le_copy(&id_addr, &evt->peer_addr);
		id_addr.type -= BT_ADDR_LE_PUBLIC_ID;

		bt_addr_copy(&peer_addr.a, &evt->peer_rpa);
		peer_addr.type = BT_ADDR_LE_RANDOM;
	} else {
		uint8_t id = evt->role == BT_HCI_ROLE_PERIPHERAL ? bt_dev.adv_conn_id :
							   BT_ID_DEFAULT;

		bt_addr_le_copy(&id_addr,
				bt_lookup_id_addr(id, &evt->peer_addr));
		bt_addr_le_copy(&peer_addr, &evt->peer_addr);
	}

	conn = find_pending_connect(evt->role, &id_addr);

	if (IS_ENABLED(CONFIG_BT_PERIPHERAL) &&
	    evt->role == BT_HCI_ROLE_PERIPHERAL &&
	    !(IS_ENABLED(CONFIG_BT_EXT_ADV) &&
	      BT_DEV_FEAT_LE_EXT_ADV(bt_dev.le.features))) {
		struct bt_le_ext_adv *adv = bt_le_adv_lookup_legacy();
		/* Clear advertising even if we are not able to add connection
		 * object to keep host in sync with controller state.
		 */
		atomic_clear_bit(adv->flags, BT_ADV_ENABLED);
		(void)bt_le_lim_adv_cancel_timeout(adv);
	}

	if (IS_ENABLED(CONFIG_BT_CENTRAL) &&
	    evt->role == BT_HCI_ROLE_CENTRAL) {
		/* Clear initiating even if we are not able to add connection
		 * object to keep the host in sync with controller state.
		 */
		atomic_clear_bit(bt_dev.flags, BT_DEV_INITIATING);
	}

	if (!conn) {
		BT_ERR("No pending conn for peer %s",
		       bt_addr_le_str(&evt->peer_addr));
		bt_hci_disconnect(handle, BT_HCI_ERR_UNSPECIFIED);
		return;
	}

	conn->handle = handle;
	bt_addr_le_copy(&conn->le.dst, &id_addr);
	conn->le.interval = sys_le16_to_cpu(evt->interval);
	conn->le.latency = sys_le16_to_cpu(evt->latency);
	conn->le.timeout = sys_le16_to_cpu(evt->supv_timeout);
	conn->role = evt->role;
	conn->err = 0U;

#if (defined(CONFIG_BT_USER_DATA_LEN_UPDATE) && ((CONFIG_BT_USER_DATA_LEN_UPDATE) > 0U))
	conn->le.data_len.tx_max_len = BT_GAP_DATA_LEN_DEFAULT;
	conn->le.data_len.tx_max_time = BT_GAP_DATA_TIME_DEFAULT;
	conn->le.data_len.rx_max_len = BT_GAP_DATA_LEN_DEFAULT;
	conn->le.data_len.rx_max_time = BT_GAP_DATA_TIME_DEFAULT;
#endif

#if (defined(CONFIG_BT_USER_PHY_UPDATE) && ((CONFIG_BT_USER_PHY_UPDATE) > 0U))
	conn->le.phy.tx_phy = BT_GAP_LE_PHY_1M;
	conn->le.phy.rx_phy = BT_GAP_LE_PHY_1M;
#endif
	/*
	 * Use connection address (instead of identity address) as initiator
	 * or responder address. Only peripheral needs to be updated. For central all
	 * was set during outgoing connection creation.
	 */
	if (IS_ENABLED(CONFIG_BT_PERIPHERAL) &&
	    conn->role == BT_HCI_ROLE_PERIPHERAL) {
		bt_addr_le_copy(&conn->le.init_addr, &peer_addr);

		if (!(IS_ENABLED(CONFIG_BT_EXT_ADV) &&
		      BT_DEV_FEAT_LE_EXT_ADV(bt_dev.le.features))) {
			struct bt_le_ext_adv *adv = bt_le_adv_lookup_legacy();

			if (IS_ENABLED(CONFIG_BT_PRIVACY) &&
			    !atomic_test_bit(adv->flags, BT_ADV_USE_IDENTITY)) {
				conn->le.resp_addr.type = BT_ADDR_LE_RANDOM;
				if (bt_addr_cmp(&evt->local_rpa,
						BT_ADDR_ANY) != 0) {
					bt_addr_copy(&conn->le.resp_addr.a,
						     &evt->local_rpa);
				} else {
					bt_addr_copy(&conn->le.resp_addr.a,
						     &bt_dev.random_addr.a);
				}
			} else {
				bt_addr_le_copy(&conn->le.resp_addr,
						&bt_dev.id_addr[conn->id]);
			}
		} else {
			/* Copy the local RPA and handle this in advertising set
			 * terminated event.
			 */
			bt_addr_copy(&conn->le.resp_addr.a, &evt->local_rpa);
		}

		/* if the controller supports, lets advertise for another
		 * peripheral connection.
		 * check for connectable advertising state is sufficient as
		 * this is how this le connection complete for peripheral occurred.
		 */
		if (BT_LE_STATES_PER_CONN_ADV(bt_dev.le.states)) {
			bt_le_adv_resume();
		}

		if (IS_ENABLED(CONFIG_BT_EXT_ADV) &&
		    !BT_DEV_FEAT_LE_EXT_ADV(bt_dev.le.features)) {
			struct bt_le_ext_adv *adv = bt_le_adv_lookup_legacy();
			/* No advertising set terminated event, must be a
			 * legacy advertiser set.
			 */
			if (!atomic_test_bit(adv->flags, BT_ADV_PERSIST)) {
				bt_le_adv_delete_legacy();
			}
		}
	}

	if (IS_ENABLED(CONFIG_BT_CENTRAL) &&
	    conn->role == BT_HCI_ROLE_CENTRAL) {
		bt_addr_le_copy(&conn->le.resp_addr, &peer_addr);

		if (IS_ENABLED(CONFIG_BT_PRIVACY)) {
			conn->le.init_addr.type = BT_ADDR_LE_RANDOM;
			if (bt_addr_cmp(&evt->local_rpa, BT_ADDR_ANY) != 0) {
				bt_addr_copy(&conn->le.init_addr.a,
					     &evt->local_rpa);
			} else {
				bt_addr_copy(&conn->le.init_addr.a,
					     &bt_dev.random_addr.a);
			}
		} else {
			bt_addr_le_copy(&conn->le.init_addr,
					&bt_dev.id_addr[conn->id]);
		}
	}

#if (defined(CONFIG_BT_USER_PHY_UPDATE) && ((CONFIG_BT_USER_PHY_UPDATE) > 0U))
	if (IS_ENABLED(CONFIG_BT_EXT_ADV) &&
	    BT_DEV_FEAT_LE_EXT_ADV(bt_dev.le.features)) {
		int err;

		err = hci_le_read_phy(conn);
		if (err) {
			BT_WARN("Failed to read PHY (%d)", err);
		}
	}
#endif /* defined(CONFIG_BT_USER_PHY_UPDATE) */

	bt_conn_set_state(conn, BT_CONN_CONNECTED);

	if (is_disconnected) {
		/* Mark the connection as already disconnected before calling
		 * the connected callback, so that the application cannot
		 * start sending packets
		 */
		bt_conn_set_state(conn, BT_CONN_DISCONNECT_COMPLETE);
	}

	bt_conn_connected(conn);

	/* Start auto-initiated procedures */
	conn_auto_initiate(conn);

	bt_conn_unref(conn);

	if (IS_ENABLED(CONFIG_BT_CENTRAL) &&
	    conn->role == BT_HCI_ROLE_CENTRAL) {
		bt_le_scan_update(false);
	}
}

static void le_enh_conn_complete(struct net_buf *buf)
{
	enh_conn_complete((struct bt_hci_evt_le_enh_conn_complete *)buf->data);
}

static void le_legacy_conn_complete(struct net_buf *buf)
{
	struct bt_hci_evt_le_conn_complete *evt = (struct bt_hci_evt_le_conn_complete *)buf->data;
	struct bt_hci_evt_le_enh_conn_complete enh;

	BT_DBG("status 0x%02x role %u %s", evt->status, evt->role,
	       bt_addr_le_str(&evt->peer_addr));

	enh.status         = evt->status;
	enh.handle         = evt->handle;
	enh.role           = evt->role;
	enh.interval       = evt->interval;
	enh.latency        = evt->latency;
	enh.supv_timeout   = evt->supv_timeout;
	enh.clock_accuracy = evt->clock_accuracy;

	bt_addr_le_copy(&enh.peer_addr, &evt->peer_addr);

	if (IS_ENABLED(CONFIG_BT_PRIVACY)) {
		bt_addr_copy(&enh.local_rpa, &bt_dev.random_addr.a);
	} else {
		bt_addr_copy(&enh.local_rpa, BT_ADDR_ANY);
	}

	bt_addr_copy(&enh.peer_rpa, BT_ADDR_ANY);

	enh_conn_complete(&enh);
}

static void le_remote_feat_complete(struct net_buf *buf)
{
	struct bt_hci_evt_le_remote_feat_complete *evt = (struct bt_hci_evt_le_remote_feat_complete *)buf->data;
	uint16_t handle = sys_le16_to_cpu(evt->handle);
	struct bt_conn *conn;

	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		BT_ERR("Unable to lookup conn for handle %u", handle);
		return;
	}

	if (!evt->status) {
		memcpy(conn->le.features, evt->features,
		       sizeof(conn->le.features));
	}

	atomic_set_bit(conn->flags, BT_CONN_AUTO_FEATURE_EXCH);

	if (IS_ENABLED(CONFIG_BT_REMOTE_INFO) &&
	    !IS_ENABLED(CONFIG_BT_REMOTE_VERSION)) {
		notify_remote_info(conn);
	}

	bt_conn_unref(conn);
}

#if (defined(CONFIG_BT_DATA_LEN_UPDATE) && ((CONFIG_BT_DATA_LEN_UPDATE) > 0U))
static void le_data_len_change(struct net_buf *buf)
{
	struct bt_hci_evt_le_data_len_change *evt = (struct bt_hci_evt_le_data_len_change *)buf->data;
	uint16_t handle = sys_le16_to_cpu(evt->handle);
	struct bt_conn *conn;

	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		BT_ERR("Unable to lookup conn for handle %u", handle);
		return;
	}

#if (defined(CONFIG_BT_USER_DATA_LEN_UPDATE) && ((CONFIG_BT_USER_DATA_LEN_UPDATE) > 0U))
	uint16_t max_tx_octets = sys_le16_to_cpu(evt->max_tx_octets);
	uint16_t max_rx_octets = sys_le16_to_cpu(evt->max_rx_octets);
	uint16_t max_tx_time = sys_le16_to_cpu(evt->max_tx_time);
	uint16_t max_rx_time = sys_le16_to_cpu(evt->max_rx_time);

	BT_DBG("max. tx: %u (%uus), max. rx: %u (%uus)",
		max_tx_octets, max_tx_time, max_rx_octets, max_rx_time);

	conn->le.data_len.tx_max_len = max_tx_octets;
	conn->le.data_len.tx_max_time = max_tx_time;
	conn->le.data_len.rx_max_len = max_rx_octets;
	conn->le.data_len.rx_max_time = max_rx_time;
	notify_le_data_len_updated(conn);
#endif

	bt_conn_unref(conn);
}
#endif /* CONFIG_BT_DATA_LEN_UPDATE */

#if (defined(CONFIG_BT_PHY_UPDATE) && ((CONFIG_BT_PHY_UPDATE) > 0U))
static void le_phy_update_complete(struct net_buf *buf)
{
	struct bt_hci_evt_le_phy_update_complete *evt = (struct bt_hci_evt_le_phy_update_complete *)buf->data;
	uint16_t handle = sys_le16_to_cpu(evt->handle);
	struct bt_conn *conn;

	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		BT_ERR("Unable to lookup conn for handle %u", handle);
		return;
	}

	BT_DBG("PHY updated: status: 0x%02x, tx: %u, rx: %u",
	       evt->status, evt->tx_phy, evt->rx_phy);

#if (defined(CONFIG_BT_USER_PHY_UPDATE) && ((CONFIG_BT_USER_PHY_UPDATE) > 0U))
	conn->le.phy.tx_phy = bt_get_phy(evt->tx_phy);
	conn->le.phy.rx_phy = bt_get_phy(evt->rx_phy);
	notify_le_phy_updated(conn);
#endif

	bt_conn_unref(conn);
}
#endif /* CONFIG_BT_PHY_UPDATE */

bool bt_le_conn_params_valid(const struct bt_le_conn_param *param)
{
	/* All limits according to BT Core spec 5.0 [Vol 2, Part E, 7.8.12] */

	if (param->interval_min > param->interval_max ||
	    param->interval_min < 6 || param->interval_max > 3200) {
		return false;
	}

	if (param->latency > 499) {
		return false;
	}

	if (param->timeout < 10 || param->timeout > 3200 ||
	    ((param->timeout * 4U) <=
	     ((1U + param->latency) * param->interval_max))) {
		return false;
	}

	return true;
}

static void le_conn_param_neg_reply(uint16_t handle, uint8_t reason)
{
	struct bt_hci_cp_le_conn_param_req_neg_reply *cp;
	struct net_buf *buf;

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_CONN_PARAM_REQ_NEG_REPLY,
				sizeof(*cp));
	if (!buf) {
		BT_ERR("Unable to allocate buffer");
		return;
	}

	cp = (struct bt_hci_cp_le_conn_param_req_neg_reply *)net_buf_add(buf, sizeof(*cp));
	cp->handle = sys_cpu_to_le16(handle);
	cp->reason = sys_cpu_to_le16(reason);

	bt_hci_cmd_send(BT_HCI_OP_LE_CONN_PARAM_REQ_NEG_REPLY, buf);
}

static int le_conn_param_req_reply(uint16_t handle,
				   const struct bt_le_conn_param *param)
{
	struct bt_hci_cp_le_conn_param_req_reply *cp;
	struct net_buf *buf;

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_CONN_PARAM_REQ_REPLY, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = (struct bt_hci_cp_le_conn_param_req_reply *)net_buf_add(buf, sizeof(*cp));
	(void)memset(cp, 0, sizeof(*cp));

	cp->handle = sys_cpu_to_le16(handle);
	cp->interval_min = sys_cpu_to_le16(param->interval_min);
	cp->interval_max = sys_cpu_to_le16(param->interval_max);
	cp->latency = sys_cpu_to_le16(param->latency);
	cp->timeout = sys_cpu_to_le16(param->timeout);

	return bt_hci_cmd_send(BT_HCI_OP_LE_CONN_PARAM_REQ_REPLY, buf);
}

static void le_conn_param_req(struct net_buf *buf)
{
	struct bt_hci_evt_le_conn_param_req *evt = (struct bt_hci_evt_le_conn_param_req *)buf->data;
	struct bt_le_conn_param param;
	struct bt_conn *conn;
	uint16_t handle;

	handle = sys_le16_to_cpu(evt->handle);
	param.interval_min = sys_le16_to_cpu(evt->interval_min);
	param.interval_max = sys_le16_to_cpu(evt->interval_max);
	param.latency = sys_le16_to_cpu(evt->latency);
	param.timeout = sys_le16_to_cpu(evt->timeout);

	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		BT_ERR("Unable to lookup conn for handle %u", handle);
		le_conn_param_neg_reply(handle, BT_HCI_ERR_UNKNOWN_CONN_ID);
		return;
	}

	if (!le_param_req(conn, &param)) {
		le_conn_param_neg_reply(handle, BT_HCI_ERR_INVALID_LL_PARAM);
	} else {
		le_conn_param_req_reply(handle, &param);
	}

	bt_conn_unref(conn);
}

static void le_conn_update_complete(struct net_buf *buf)
{
	struct bt_hci_evt_le_conn_update_complete *evt = (struct bt_hci_evt_le_conn_update_complete *)buf->data;
	struct bt_conn *conn;
	uint16_t handle;

	handle = sys_le16_to_cpu(evt->handle);

	BT_DBG("status 0x%02x, handle %u", evt->status, handle);

	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		BT_ERR("Unable to lookup conn for handle %u", handle);
		return;
	}

	if (!evt->status) {
		conn->le.interval = sys_le16_to_cpu(evt->interval);
		conn->le.latency = sys_le16_to_cpu(evt->latency);
		conn->le.timeout = sys_le16_to_cpu(evt->supv_timeout);
		notify_le_param_updated(conn);
	} else if (evt->status == BT_HCI_ERR_UNSUPP_REMOTE_FEATURE &&
		   conn->role == BT_HCI_ROLE_PERIPHERAL &&
		   !atomic_test_and_set_bit(conn->flags,
					    BT_CONN_PERIPHERAL_PARAM_L2CAP)) {
		/* CPR not supported, let's try L2CAP CPUP instead */
		struct bt_le_conn_param param;

		param.interval_min = conn->le.interval_min;
		param.interval_max = conn->le.interval_max;
		param.latency = conn->le.pending_latency;
		param.timeout = conn->le.pending_timeout;

		bt_l2cap_update_conn_param(conn, &param);
	}

	bt_conn_unref(conn);
}

#if (defined(CONFIG_BT_HCI_ACL_FLOW_CONTROL) && ((CONFIG_BT_HCI_ACL_FLOW_CONTROL) > 0U))
static int set_flow_control(void)
{
	struct bt_hci_cp_host_buffer_size *hbs;
	struct net_buf *buf;
	int err;

	/* Check if host flow control is actually supported */
	if (!BT_CMD_TEST(bt_dev.supported_commands, 10, 5)) {
		BT_WARN("Controller to host flow control not supported");
		return 0;
	}

	buf = bt_hci_cmd_create(BT_HCI_OP_HOST_BUFFER_SIZE,
				sizeof(*hbs));
	if (!buf) {
		return -ENOBUFS;
	}

	hbs = net_buf_add(buf, sizeof(*hbs));
	(void)memset(hbs, 0, sizeof(*hbs));
	hbs->acl_mtu = sys_cpu_to_le16(CONFIG_BT_BUF_ACL_RX_SIZE);
	hbs->acl_pkts = sys_cpu_to_le16(CONFIG_BT_BUF_ACL_RX_COUNT);

	err = bt_hci_cmd_send_sync(BT_HCI_OP_HOST_BUFFER_SIZE, buf, NULL);
	if (err) {
		return err;
	}

	buf = bt_hci_cmd_create(BT_HCI_OP_SET_CTL_TO_HOST_FLOW, 1);
	if (!buf) {
		return -ENOBUFS;
	}

	net_buf_add_u8(buf, BT_HCI_CTL_TO_HOST_FLOW_ENABLE);
	return bt_hci_cmd_send_sync(BT_HCI_OP_SET_CTL_TO_HOST_FLOW, buf, NULL);
}
#endif /* CONFIG_BT_HCI_ACL_FLOW_CONTROL */

void bt_conn_unpair(uint8_t id, const bt_addr_le_t *addr)
{
	struct bt_keys *keys = NULL;
	struct bt_conn *conn = bt_conn_lookup_addr_le(id, addr);
	BT_DEVICE_ADDR bd_addr;
	DEVICE_HANDLE handle;
	API_RESULT retval;

	if (NULL != conn)
	{
		/* Clear the conn->le.keys pointer since we'll invalidate it,
		 * and don't want any subsequent code (like disconnected
		 * callbacks) accessing it.
		 */
		if (conn->type == BT_CONN_TYPE_LE) {
			keys = conn->le.keys;
			conn->le.keys = NULL;
		}
#if 0
		bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
#endif
		bt_conn_unref(conn);
	}

	if (IS_ENABLED(CONFIG_BT_BREDR)) {
		/* LE Public may indicate BR/EDR as well */
		if (addr->type == BT_ADDR_LE_PUBLIC) {
			bt_keys_link_key_clear_addr(&addr->a);
		}
	}

	if (IS_ENABLED(CONFIG_BT_SMP)) {
		if (!keys) {
			keys = bt_keys_find_addr(id, addr);
		}

		bd_addr.type = addr->type;
		memcpy(bd_addr.addr, addr->a.val, sizeof(bd_addr.addr));
        if (NULL == conn)
        {
            retval = BT_smp_get_bd_handle(&bd_addr, &handle);
            if (API_SUCCESS == retval)
            {
                retval = BT_smp_mark_device_untrusted_pl(&handle);
                if (API_SUCCESS == retval)
                {
                }
            }
        }
        else
        {
            retval = BT_smp_mark_device_untrusted_pl(&conn->deviceId);
            if (API_SUCCESS == retval)
            {
            }
        }
		(void)retval;

        if (keys) {
            bt_keys_clear(keys);
        }
        BT_DBG("untrust device err %d", retval);
	}

#if !(defined (CONFIG_BT_BLE_DISABLE) && (CONFIG_BT_BLE_DISABLE > 0U))
	bt_gatt_clear(id, addr);
#endif

#if ((defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U)) || (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U)))
	struct bt_conn_auth_info_cb *listener, *next;

	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&bt_auth_info_cbs, listener,
					  next, node, struct bt_conn_auth_info_cb) {
		if (listener->bond_deleted) {
			listener->bond_deleted(id, addr);
		}
	}
#endif /* defined(CONFIG_BT_SMP) || defined(CONFIG_BT_BREDR) */

    (void)keys;
}

static void unpair(uint8_t id, const bt_addr_le_t *addr)
{
	struct bt_conn *conn;
	bt_addr_le_t id_addr;

    if (addr->type == BT_ADDR_LE_PUBLIC_ID ||
        addr->type == BT_ADDR_LE_RANDOM_ID) {
        bt_addr_le_copy(&id_addr, addr);
        id_addr.type -= BT_ADDR_LE_PUBLIC_ID;
    } else if (addr->type == BT_HCI_PEER_ADDR_ANONYMOUS) {
        bt_addr_le_copy(&id_addr, BT_ADDR_LE_ANY);
    } else {
        bt_addr_le_copy(&id_addr,
                bt_lookup_id_addr(id, addr));
    }

    conn = bt_conn_lookup_addr_le(id, &id_addr);
    if (NULL != conn) {
        atomic_set_bit(conn->flags, BT_CONN_UNPAIRING);
        bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
        bt_conn_unref(conn);
        return;
    }
#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
    conn = bt_conn_lookup_addr_br(&id_addr.a);
    if (NULL != conn) {
        bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
        atomic_set_bit(conn->flags, BT_CONN_UNPAIRING);
        bt_conn_unref(conn);
        return;
    }
#endif
    bt_conn_unpair(id, &id_addr);
}
#if 0
static void unpair_remote(const struct bt_bond_info *info, void *data)
{
	uint8_t *id = (uint8_t *) data;

	unpair(*id, &info->addr);
}
#endif
int bt_unpair(uint8_t id, const bt_addr_le_t *addr)
{
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
	static SM_PEER_INFO br_list[SM_MAX_DEVICES];
#endif
	bt_addr_le_t peer_addr;
	SMP_BD_ADDR bdaddr;
	API_RESULT retval;
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
	uint8_t count;
#endif

	if (id >= CONFIG_BT_ID_MAX) {
		return -EINVAL;
	}

#if 0
	if (IS_ENABLED(CONFIG_BT_SMP) &&
	    (!addr || !bt_addr_le_cmp(addr, BT_ADDR_LE_ANY))) {
		bt_foreach_bond(id, unpair_remote, &id);
		return 0;
	}

	unpair(id, addr);
#else
	if (!addr || !bt_addr_le_cmp(addr, BT_ADDR_LE_ANY))
	{
		/* BR EDR */
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
		retval = BT_sm_get_device_list (br_list, SM_ANY_LIST, &count);
		if (API_SUCCESS == retval)
		{
			for (uint8_t i = 0; i < count; i++)
			{
				peer_addr.type = BT_ADDR_LE_PUBLIC;
				memcpy(peer_addr.a.val, br_list[i].bd_addr, sizeof(peer_addr.a.val));
				unpair(id, &peer_addr);
			}
		}
#endif
		/* BLE */
		for (uint8_t i = BT_MAX_DEVICE_QUEUE_SIZE; i > 0 ; i--)
		{
            uint8_t bd_handle = (uint8_t)(i - 1);
			retval = BT_smp_get_bd_addr (&bd_handle, &bdaddr);
			if (API_SUCCESS == retval)
			{
                DEVICE_LINK_TYPE    link_type;
                retval = device_queue_get_link_type
                (
                     &link_type,
                     &bd_handle
                );
                if ((API_SUCCESS == retval) && (DQ_LE_LINK == link_type))
                {
                    peer_addr.type = bdaddr.type;
                    memcpy(peer_addr.a.val, bdaddr.addr, sizeof(peer_addr.a.val));
                    unpair(id, &peer_addr);
                }
			}
		}

		return 0;
	}

	unpair(id, addr);
#endif
	return 0;
}

#endif /* CONFIG_BT_CONN */

#if ((defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U)) || (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U)))
enum bt_security_err bt_security_err_get(uint8_t hci_err)
{
	switch (hci_err) {
	case BT_HCI_ERR_SUCCESS:
		return BT_SECURITY_ERR_SUCCESS;
	case BT_HCI_ERR_AUTH_FAIL:
		return BT_SECURITY_ERR_AUTH_FAIL;
	case BT_HCI_ERR_PIN_OR_KEY_MISSING:
		return BT_SECURITY_ERR_PIN_OR_KEY_MISSING;
	case BT_HCI_ERR_PAIRING_NOT_SUPPORTED:
		return BT_SECURITY_ERR_PAIR_NOT_SUPPORTED;
	case BT_HCI_ERR_PAIRING_NOT_ALLOWED:
		return BT_SECURITY_ERR_PAIR_NOT_ALLOWED;
	case BT_HCI_ERR_INVALID_PARAM:
		return BT_SECURITY_ERR_INVALID_PARAM;
	default:
		return BT_SECURITY_ERR_UNSPECIFIED;
	}
}
#endif /* defined(CONFIG_BT_SMP) || defined(CONFIG_BT_BREDR) */

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0U))
bool update_sec_level(struct bt_conn *conn)
{
	if (conn->le.keys && (conn->le.keys->flags & BT_KEYS_AUTHENTICATED)) {
		if (conn->le.keys->flags & BT_KEYS_SC &&
		    conn->le.keys->enc_size == BT_SMP_MAX_ENC_KEY_SIZE) {
			conn->sec_level = BT_SECURITY_L4;
		} else {
			conn->sec_level = BT_SECURITY_L3;
		}
	} else {
		conn->sec_level = BT_SECURITY_L2;
	}

	return !(conn->required_sec_level > conn->sec_level);
}
#endif /* CONFIG_BT_SMP */

#if ((defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U)) || (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U)))
static void hci_encrypt_change(struct net_buf *buf)
{
	struct bt_hci_evt_encrypt_change *evt = (struct bt_hci_evt_encrypt_change *)buf->data;
	uint16_t handle = sys_le16_to_cpu(evt->handle);
	uint8_t status = evt->status;
	struct bt_conn *conn;

	BT_DBG("status 0x%02x handle %u encrypt 0x%02x", evt->status, handle,
	       evt->encrypt);

	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		BT_ERR("Unable to look up conn with handle %u", handle);
		return;
	}
    (void)status;
#if 0
	if (status) {
		bt_conn_security_changed(conn, status,
					 bt_security_err_get(status));
		bt_conn_unref(conn);
		return;
	}

	conn->encrypt = evt->encrypt;

#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
	if (conn->type == BT_CONN_TYPE_LE) {
		/*
		 * we update keys properties only on successful encryption to
		 * avoid losing valid keys if encryption was not successful.
		 *
		 * Update keys with last pairing info for proper sec level
		 * update. This is done only for LE transport, for BR/EDR keys
		 * are updated on HCI 'Link Key Notification Event'
		 */
		if (conn->encrypt) {
			bt_smp_update_keys(conn);
		}

		if (!update_sec_level(conn)) {
			status = BT_HCI_ERR_AUTH_FAIL;
		}
	}
#endif /* CONFIG_BT_SMP */
#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
	if (conn->type == BT_CONN_TYPE_BR) {
		if (!bt_br_update_sec_level(conn)) {
			bt_conn_unref(conn);
			return;
		}

		if (IS_ENABLED(CONFIG_BT_SMP)) {
			/*
			 * Start SMP over BR/EDR if we are pairing and are
			 * central on the link
			 */
			if (atomic_test_bit(conn->flags, BT_CONN_BR_PAIRING) &&
			    conn->role == BT_CONN_ROLE_CENTRAL) {
				bt_smp_br_send_pairing_req(conn);
			}
		}
	}
#endif /* CONFIG_BT_BREDR */

	bt_conn_security_changed(conn, status, bt_security_err_get(status));

	if (status) {
		BT_ERR("Failed to set required security level");
		bt_conn_disconnect(conn, status);
	}
#else

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
	if (conn->type == BT_CONN_TYPE_BR)
    {
        if (status) {
            bt_conn_security_changed(conn, status,
                         bt_security_err_get(status));
            bt_conn_unref(conn);
            return;
        }

        conn->encrypt = evt->encrypt;

        if (conn->type == BT_CONN_TYPE_BR) {
            if (!bt_br_update_sec_level(conn)) {
                bt_conn_unref(conn);
                return;
            }

            if (IS_ENABLED(CONFIG_BT_SMP)) {
                /*
                 * Start SMP over BR/EDR if we are pairing and are
                 * master on the link
                 */
                if (atomic_test_bit(conn->flags, BT_CONN_BR_PAIRING) &&
                    conn->role == BT_CONN_ROLE_CENTRAL) {
                    bt_smp_br_send_pairing_req(conn);
                }
            }
        }

        bt_conn_security_changed(conn, status, bt_security_err_get(status));

        if (status) {
            BT_ERR("Failed to set required security level");
            bt_conn_disconnect(conn, status);
        }
    }
#endif /* CONFIG_BT_BREDR */

#endif

	bt_conn_unref(conn);
}

static void hci_encrypt_key_refresh_complete(struct net_buf *buf)
{
	struct bt_hci_evt_encrypt_key_refresh_complete *evt = (struct bt_hci_evt_encrypt_key_refresh_complete *)buf->data;
	uint8_t status = evt->status;
	struct bt_conn *conn;
	uint16_t handle;

	handle = sys_le16_to_cpu(evt->handle);

	BT_DBG("status 0x%02x handle %u", evt->status, handle);

	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		BT_ERR("Unable to look up conn with handle %u", handle);
		return;
	}

	if (status) {
		bt_conn_security_changed(conn, status,
					 bt_security_err_get(status));
		bt_conn_unref(conn);
		return;
	}

	/*
	 * Update keys with last pairing info for proper sec level update.
	 * This is done only for LE transport. For BR/EDR transport keys are
	 * updated on HCI 'Link Key Notification Event', therefore update here
	 * only security level based on available keys and encryption state.
	 */
#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
	if (conn->type == BT_CONN_TYPE_LE) {
		bt_smp_update_keys(conn);

		if (!update_sec_level(conn)) {
			status = BT_HCI_ERR_AUTH_FAIL;
		}
	}
#endif /* CONFIG_BT_SMP */
#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
	if (conn->type == BT_CONN_TYPE_BR) {
		if (!bt_br_update_sec_level(conn)) {
			bt_conn_unref(conn);
			return;
		}
	}
#endif /* CONFIG_BT_BREDR */

	bt_conn_security_changed(conn, status, bt_security_err_get(status));
	if (status) {
		BT_ERR("Failed to set required security level");
		bt_conn_disconnect(conn, status);
	}

	bt_conn_unref(conn);
}
#endif /* CONFIG_BT_SMP || CONFIG_BT_BREDR */

#if (defined(CONFIG_BT_REMOTE_VERSION) && ((CONFIG_BT_REMOTE_VERSION) > 0U))
static void bt_hci_evt_read_remote_version_complete(struct net_buf *buf)
{
	struct bt_hci_evt_remote_version_info *evt;
	struct bt_conn *conn;
	uint16_t handle;

	evt = net_buf_pull_mem(buf, sizeof(*evt));
	handle = sys_le16_to_cpu(evt->handle);
	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		BT_ERR("No connection for handle %u", handle);
		return;
	}

	if (!evt->status) {
		conn->rv.version = evt->version;
		conn->rv.manufacturer = sys_le16_to_cpu(evt->manufacturer);
		conn->rv.subversion = sys_le16_to_cpu(evt->subversion);
	}

	atomic_set_bit(conn->flags, BT_CONN_AUTO_VERSION_INFO);

	if (IS_ENABLED(CONFIG_BT_REMOTE_INFO)) {
		/* Remote features is already present */
		notify_remote_info(conn);
	}

	bt_conn_unref(conn);
}
#endif /* CONFIG_BT_REMOTE_VERSION */

static void hci_hardware_error(struct net_buf *buf)
{
	struct bt_hci_evt_hardware_error *evt;

	evt = (struct bt_hci_evt_hardware_error *)net_buf_pull_mem(buf, sizeof(*evt));

	BT_ERR("Hardware error, hardware code: %d", evt->hardware_code);
    (void)evt;
}

#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
static void le_ltk_neg_reply(uint16_t handle)
{
	struct bt_hci_cp_le_ltk_req_neg_reply *cp;
	struct net_buf *buf;

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_LTK_REQ_NEG_REPLY, sizeof(*cp));
	if (!buf) {
		BT_ERR("Out of command buffers");

		return;
	}

	cp = (struct bt_hci_cp_le_ltk_req_neg_reply *)net_buf_add(buf, sizeof(*cp));
	cp->handle = sys_cpu_to_le16(handle);

	bt_hci_cmd_send(BT_HCI_OP_LE_LTK_REQ_NEG_REPLY, buf);
}

static void le_ltk_reply(uint16_t handle, uint8_t *ltk)
{
	struct bt_hci_cp_le_ltk_req_reply *cp;
	struct net_buf *buf;

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_LTK_REQ_REPLY,
				sizeof(*cp));
	if (!buf) {
		BT_ERR("Out of command buffers");
		return;
	}

	cp = (struct bt_hci_cp_le_ltk_req_reply *)net_buf_add(buf, sizeof(*cp));
	cp->handle = sys_cpu_to_le16(handle);
	memcpy(cp->ltk, ltk, sizeof(cp->ltk));

	bt_hci_cmd_send(BT_HCI_OP_LE_LTK_REQ_REPLY, buf);
}

static void le_ltk_request(struct net_buf *buf)
{
	struct bt_hci_evt_le_ltk_request *evt = (struct bt_hci_evt_le_ltk_request *)buf->data;
	struct bt_conn *conn;
	uint16_t handle;
	uint8_t ltk[16];

	handle = sys_le16_to_cpu(evt->handle);

	BT_DBG("handle %u", handle);

	conn = bt_conn_lookup_handle(handle);
	if (!conn) {
		BT_ERR("Unable to lookup conn for handle %u", handle);
		return;
	}

	if (bt_smp_request_ltk(conn, evt->rand, evt->ediv, ltk)) {
		le_ltk_reply(handle, ltk);
	} else {
		le_ltk_neg_reply(handle);
	}

	bt_conn_unref(conn);
}
#endif /* CONFIG_BT_SMP */

#if (defined(BT_TESTER) && ((BT_TESTER) > 0U))
static void hci_reset_complete(struct net_buf *buf)
{
	uint8_t status = buf->data[0];
	atomic_t flags;

	BT_DBG("status 0x%02x", status);

	if (status) {
		return;
	}

	if (IS_ENABLED(CONFIG_BT_OBSERVER)) {
		bt_scan_reset();
	}

#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0U))
	bt_br_discovery_reset();
#endif /* CONFIG_BT_BREDR */

	flags = (atomic_get(bt_dev.flags) & BT_DEV_PERSISTENT_FLAGS);
	atomic_set(bt_dev.flags, flags);
}
#endif /* BT_TESTER */
static void hci_cmd_done(uint16_t opcode, uint8_t status, struct net_buf *buf)
{
	BT_DBG("opcode 0x%04x status 0x%02x buf %p", opcode, status, buf);

	if (net_buf_pool_get(buf->pool_id) != &hci_cmd_pool) {
		BT_WARN("opcode 0x%04x pool id %u pool %p != &hci_cmd_pool %p",
			opcode, buf->pool_id, net_buf_pool_get(buf->pool_id),
			&hci_cmd_pool);
		return;
	}

	if (cmd(buf)->opcode != opcode) {
		BT_WARN("OpCode 0x%04x completed instead of expected 0x%04x",
			opcode, cmd(buf)->opcode);
		return;
	}

	if (bt_dev.sent_cmd) {
		net_buf_unref(bt_dev.sent_cmd);
		bt_dev.sent_cmd = NULL;
	}

	if (cmd(buf)->state && !status) {
		struct bt_hci_cmd_state_set *update = cmd(buf)->state;

		atomic_set_bit_to(update->target, update->bit, update->val);
	}

	/* If the command was synchronous wake up bt_hci_cmd_send_sync() */
	if (cmd(buf)->sync) {
		cmd(buf)->status = status;
		OSA_SemaphorePost(cmd(buf)->sync);
	}
}

static void hci_cmd_complete(struct net_buf *buf)
{
	struct bt_hci_evt_cmd_complete *evt;
	uint8_t status, ncmd;
	uint16_t opcode;

	hci_cmd_complete_rsp_handlers(buf);

	evt = (struct bt_hci_evt_cmd_complete *)net_buf_pull_mem(buf, sizeof(*evt));
	ncmd = evt->ncmd;
	opcode = sys_le16_to_cpu(evt->opcode);

	BT_DBG("opcode 0x%04x", opcode);

	/* All command return parameters have a 1-byte status in the
	 * beginning, so we can safely make this generalization.
	 */
	status = buf->data[0];

	hci_cmd_done(opcode, status, buf);

#if 0
	/* Allow next command to be sent */
	if (ncmd) {
		OSA_SemaphorePost(bt_dev.ncmd_sem);
	}
#endif
    (void)ncmd;
}

static void hci_cmd_status(struct net_buf *buf)
{
	struct bt_hci_evt_cmd_status *evt;
	uint16_t opcode;
	uint8_t ncmd;

	evt = (struct bt_hci_evt_cmd_status *)net_buf_pull_mem(buf, sizeof(*evt));
	opcode = sys_le16_to_cpu(evt->opcode);
	ncmd = evt->ncmd;

	BT_DBG("opcode 0x%04x", opcode);

	hci_cmd_done(opcode, evt->status, buf);
#if 0
	/* Allow next command to be sent */
	if (ncmd) {
		OSA_SemaphorePost(bt_dev.ncmd_sem);
	}
#endif
    (void)ncmd;
}

int bt_hci_get_conn_handle(const struct bt_conn *conn, uint16_t *conn_handle)
{
	if (conn->state != BT_CONN_CONNECTED) {
		return -ENOTCONN;
	}

	*conn_handle = conn->handle;
	return 0;
}

#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
int bt_hci_get_adv_handle(const struct bt_le_ext_adv *adv, uint8_t *adv_handle)
{
	if (!atomic_test_bit(adv->flags, BT_ADV_CREATED)) {
		return -EINVAL;
	}

	*adv_handle = adv->handle;
	return 0;
}
#endif /* CONFIG_BT_EXT_ADV */

#if (defined(CONFIG_BT_HCI_VS_EVT_USER) && ((CONFIG_BT_HCI_VS_EVT_USER) > 0U))
int bt_hci_register_vnd_evt_cb(bt_hci_vnd_evt_cb_t cb)
{
	hci_vnd_evt_cb = cb;
	return 0;
}
#endif /* CONFIG_BT_HCI_VS_EVT_USER */

#if (defined(CONFIG_BT_HCI_VS_EVT) && (CONFIG_BT_HCI_VS_EVT > 0))
static const struct event_handler vs_events[] = {
#if (defined(CONFIG_BT_DF_VS_CL_IQ_REPORT_16_BITS_IQ_SAMPLES) && (CONFIG_BT_DF_VS_CL_IQ_REPORT_16_BITS_IQ_SAMPLES > 0))
	EVENT_HANDLER(BT_HCI_EVT_VS_LE_CONNECTIONLESS_IQ_REPORT,
		      bt_hci_le_vs_df_connectionless_iq_report,
		      sizeof(struct bt_hci_evt_vs_le_connectionless_iq_report)),
#endif /* CONFIG_BT_DF_VS_CL_IQ_REPORT_16_BITS_IQ_SAMPLES */
#if (defined(CONFIG_BT_DF_VS_CONN_IQ_REPORT_16_BITS_IQ_SAMPLES) && (CONFIG_BT_DF_VS_CONN_IQ_REPORT_16_BITS_IQ_SAMPLES > 0))
	EVENT_HANDLER(BT_HCI_EVT_VS_LE_CONNECTION_IQ_REPORT, bt_hci_le_vs_df_connection_iq_report,
		      sizeof(struct bt_hci_evt_vs_le_connection_iq_report)),
#endif /* CONFIG_BT_DF_VS_CONN_IQ_REPORT_16_BITS_IQ_SAMPLES */
};
#endif

static void hci_vendor_event(struct net_buf *buf)
{
	bool handled = false;

#if (defined(CONFIG_BT_HCI_VS_EVT_USER) && ((CONFIG_BT_HCI_VS_EVT_USER) > 0U))
	if (hci_vnd_evt_cb) {
		struct net_buf_simple_state state;

		net_buf_simple_save(&buf->b, &state);

		handled = hci_vnd_evt_cb(&buf->b);

		net_buf_simple_restore(&buf->b, &state);
	}
#endif /* CONFIG_BT_HCI_VS_EVT_USER */

#if (defined(CONFIG_BT_HCI_VS_EVT) && (CONFIG_BT_HCI_VS_EVT > 0))
	if (IS_ENABLED(CONFIG_BT_HCI_VS_EVT) && !handled) {
		struct bt_hci_evt_vs *evt;

		evt = net_buf_pull_mem(buf, sizeof(*evt));

		BT_DBG("subevent 0x%02x", evt->subevent);

		handle_vs_event(evt->subevent, buf, vs_events, ARRAY_SIZE(vs_events));
	}
#endif /* CONFIG_BT_HCI_VS_EVT */

	(void)handled;
}

static const struct event_handler meta_events[] = {
#if (defined(CONFIG_BT_OBSERVER) && ((CONFIG_BT_OBSERVER) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_ADVERTISING_REPORT, bt_hci_le_adv_report,
		      sizeof(struct bt_hci_evt_le_advertising_report)),
#endif /* CONFIG_BT_OBSERVER */
#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_CONN_COMPLETE, le_legacy_conn_complete,
		      sizeof(struct bt_hci_evt_le_conn_complete)),
	EVENT_HANDLER(BT_HCI_EVT_LE_ENH_CONN_COMPLETE, le_enh_conn_complete,
		      sizeof(struct bt_hci_evt_le_enh_conn_complete)),
	EVENT_HANDLER(BT_HCI_EVT_LE_CONN_UPDATE_COMPLETE,
		      le_conn_update_complete,
		      sizeof(struct bt_hci_evt_le_conn_update_complete)),
	EVENT_HANDLER(BT_HCI_EVT_LE_REMOTE_FEAT_COMPLETE,
		      le_remote_feat_complete,
		      sizeof(struct bt_hci_evt_le_remote_feat_complete)),
	EVENT_HANDLER(BT_HCI_EVT_LE_CONN_PARAM_REQ, le_conn_param_req,
		      sizeof(struct bt_hci_evt_le_conn_param_req)),
#if (defined(CONFIG_BT_DATA_LEN_UPDATE) && ((CONFIG_BT_DATA_LEN_UPDATE) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_DATA_LEN_CHANGE, le_data_len_change,
		      sizeof(struct bt_hci_evt_le_data_len_change)),
#endif /* CONFIG_BT_DATA_LEN_UPDATE */
#if (defined(CONFIG_BT_PHY_UPDATE) && ((CONFIG_BT_PHY_UPDATE) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_PHY_UPDATE_COMPLETE,
		      le_phy_update_complete,
		      sizeof(struct bt_hci_evt_le_phy_update_complete)),
#endif /* CONFIG_BT_PHY_UPDATE */
#endif /* CONFIG_BT_CONN */
#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_LTK_REQUEST, le_ltk_request,
		      sizeof(struct bt_hci_evt_le_ltk_request)),
#endif /* CONFIG_BT_SMP */
#if (defined(CONFIG_BT_ECC) && ((CONFIG_BT_ECC) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_P256_PUBLIC_KEY_COMPLETE,
		      bt_hci_evt_le_pkey_complete,
		      sizeof(struct bt_hci_evt_le_p256_public_key_complete)),
	EVENT_HANDLER(BT_HCI_EVT_LE_GENERATE_DHKEY_COMPLETE,
		      bt_hci_evt_le_dhkey_complete,
		      sizeof(struct bt_hci_evt_le_generate_dhkey_complete)),
#endif /* CONFIG_BT_SMP */
#if (defined(CONFIG_BT_EXT_ADV) && ((CONFIG_BT_EXT_ADV) > 0U))
#if (defined(CONFIG_BT_BROADCASTER) && ((CONFIG_BT_BROADCASTER) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_ADV_SET_TERMINATED, bt_hci_le_adv_set_terminated,
		      sizeof(struct bt_hci_evt_le_adv_set_terminated)),
	EVENT_HANDLER(BT_HCI_EVT_LE_SCAN_REQ_RECEIVED, bt_hci_le_scan_req_received,
		      sizeof(struct bt_hci_evt_le_scan_req_received)),
#endif
#if (defined(CONFIG_BT_OBSERVER) && ((CONFIG_BT_OBSERVER) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_SCAN_TIMEOUT, bt_hci_le_scan_timeout,
		      0),
	EVENT_HANDLER(BT_HCI_EVT_LE_EXT_ADVERTISING_REPORT, bt_hci_le_adv_ext_report,
		      sizeof(struct bt_hci_evt_le_ext_advertising_report)),
#endif /* defined(CONFIG_BT_OBSERVER) */
#if (defined(CONFIG_BT_PER_ADV_SYNC) && ((CONFIG_BT_PER_ADV_SYNC) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_PER_ADV_SYNC_ESTABLISHED,
		      bt_hci_le_per_adv_sync_established,
		      sizeof(struct bt_hci_evt_le_per_adv_sync_established)),
	EVENT_HANDLER(BT_HCI_EVT_LE_PER_ADVERTISING_REPORT, bt_hci_le_per_adv_report,
		      sizeof(struct bt_hci_evt_le_per_advertising_report)),
	EVENT_HANDLER(BT_HCI_EVT_LE_PER_ADV_SYNC_LOST, bt_hci_le_per_adv_sync_lost,
		      sizeof(struct bt_hci_evt_le_per_adv_sync_lost)),
#if (defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_PAST_RECEIVED, bt_hci_le_past_received,
		      sizeof(struct bt_hci_evt_le_past_received)),
#endif /* CONFIG_BT_CONN */
#endif /* defined(CONFIG_BT_PER_ADV_SYNC) */
#endif /* defined(CONFIG_BT_EXT_ADV) */
#if (defined(CONFIG_BT_ISO_UNICAST) && ((CONFIG_BT_ISO_UNICAST) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_CIS_ESTABLISHED, hci_le_cis_established,
		      sizeof(struct bt_hci_evt_le_cis_established)),
#if (defined(CONFIG_BT_ISO_PERIPHERAL) && ((CONFIG_BT_ISO_PERIPHERAL) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_CIS_REQ, hci_le_cis_req,
		      sizeof(struct bt_hci_evt_le_cis_req)),
#endif /* (CONFIG_BT_ISO_PERIPHERAL) */
#endif /* (CONFIG_BT_ISO_UNICAST) */
#if (defined(CONFIG_BT_ISO_BROADCASTER) && (CONFIG_BT_ISO_BROADCASTER > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_BIG_COMPLETE,
		      hci_le_big_complete,
		      sizeof(struct bt_hci_evt_le_big_complete)),
	EVENT_HANDLER(BT_HCI_EVT_LE_BIG_TERMINATE,
		      hci_le_big_terminate,
		      sizeof(struct bt_hci_evt_le_big_terminate)),
#endif /* CONFIG_BT_ISO_BROADCASTER */
#if (defined(CONFIG_BT_ISO_SYNC_RECEIVER) && ((CONFIG_BT_ISO_SYNC_RECEIVER) > 0))
	EVENT_HANDLER(BT_HCI_EVT_LE_BIG_SYNC_ESTABLISHED,
		      hci_le_big_sync_established,
		      sizeof(struct bt_hci_evt_le_big_sync_established)),
	EVENT_HANDLER(BT_HCI_EVT_LE_BIG_SYNC_LOST,
		      hci_le_big_sync_lost,
		      sizeof(struct bt_hci_evt_le_big_sync_lost)),
	EVENT_HANDLER(BT_HCI_EVT_LE_BIGINFO_ADV_REPORT,
		      bt_hci_le_biginfo_adv_report,
		      sizeof(struct bt_hci_evt_le_biginfo_adv_report)),
#endif /* CONFIG_BT_ISO_SYNC_RECEIVER */
#if (defined(CONFIG_BT_DF_CONNECTIONLESS_CTE_RX) && (CONFIG_BT_DF_CONNECTIONLESS_CTE_RX > 0U))
	EVENT_HANDLER(BT_HCI_EVT_LE_CONNECTIONLESS_IQ_REPORT, bt_hci_le_df_connectionless_iq_report,
		      sizeof(struct bt_hci_evt_le_connectionless_iq_report)),
#endif /* CONFIG_BT_DF_CONNECTIONLESS_CTE_RX */
#if (defined(CONFIG_BT_DF_CONNECTION_CTE_RX) && (CONFIG_BT_DF_CONNECTION_CTE_RX > 0))
	EVENT_HANDLER(BT_HCI_EVT_LE_CONNECTION_IQ_REPORT, bt_hci_le_df_connection_iq_report,
		      sizeof(struct bt_hci_evt_le_connection_iq_report)),
#endif /* CONFIG_BT_DF_CONNECTION_CTE_RX */
#if (defined(CONFIG_BT_DF_CONNECTION_CTE_REQ) && (CONFIG_BT_DF_CONNECTION_CTE_REQ > 0))
	EVENT_HANDLER(BT_HCI_EVT_LE_CTE_REQUEST_FAILED, bt_hci_le_df_cte_req_failed,
		      sizeof(struct bt_hci_evt_le_cte_req_failed)),
#endif /* CONFIG_BT_DF_CONNECTION_CTE_REQ */

};

static void hci_le_meta_event(struct net_buf *buf)
{
	struct bt_hci_evt_le_meta_event *evt;

	evt = (struct bt_hci_evt_le_meta_event *)net_buf_pull_mem(buf, sizeof(*evt));

	BT_DBG("subevent 0x%02x", evt->subevent);

	handle_event(evt->subevent, buf, meta_events, ARRAY_SIZE(meta_events));
}

static const struct event_handler normal_events[] = {
	EVENT_HANDLER(BT_HCI_EVT_VENDOR, hci_vendor_event,
		      sizeof(struct bt_hci_evt_vs)),
	EVENT_HANDLER(BT_HCI_EVT_LE_META_EVENT, hci_le_meta_event,
		      sizeof(struct bt_hci_evt_le_meta_event)),
#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_CONN_REQUEST, bt_hci_conn_req,
		      sizeof(struct bt_hci_evt_conn_request)),
	EVENT_HANDLER(BT_HCI_EVT_CONN_COMPLETE, bt_hci_conn_complete,
		      sizeof(struct bt_hci_evt_conn_complete)),
	EVENT_HANDLER(BT_HCI_EVT_PIN_CODE_REQ, bt_hci_pin_code_req,
		      sizeof(struct bt_hci_evt_pin_code_req)),
	EVENT_HANDLER(BT_HCI_EVT_LINK_KEY_NOTIFY, bt_hci_link_key_notify,
		      sizeof(struct bt_hci_evt_link_key_notify)),
	EVENT_HANDLER(BT_HCI_EVT_LINK_KEY_REQ, bt_hci_link_key_req,
		      sizeof(struct bt_hci_evt_link_key_req)),
	EVENT_HANDLER(BT_HCI_EVT_IO_CAPA_RESP, bt_hci_io_capa_resp,
		      sizeof(struct bt_hci_evt_io_capa_resp)),
	EVENT_HANDLER(BT_HCI_EVT_IO_CAPA_REQ, bt_hci_io_capa_req,
		      sizeof(struct bt_hci_evt_io_capa_req)),
	EVENT_HANDLER(BT_HCI_EVT_SSP_COMPLETE, bt_hci_ssp_complete,
		      sizeof(struct bt_hci_evt_ssp_complete)),
	EVENT_HANDLER(BT_HCI_EVT_USER_CONFIRM_REQ, bt_hci_user_confirm_req,
		      sizeof(struct bt_hci_evt_user_confirm_req)),
	EVENT_HANDLER(BT_HCI_EVT_USER_PASSKEY_NOTIFY,
		      bt_hci_user_passkey_notify,
		      sizeof(struct bt_hci_evt_user_passkey_notify)),
	EVENT_HANDLER(BT_HCI_EVT_USER_PASSKEY_REQ, bt_hci_user_passkey_req,
		      sizeof(struct bt_hci_evt_user_passkey_req)),
	EVENT_HANDLER(BT_HCI_EVT_INQUIRY_COMPLETE, bt_hci_inquiry_complete,
		      sizeof(struct bt_hci_evt_inquiry_complete)),
	EVENT_HANDLER(BT_HCI_EVT_INQUIRY_RESULT_WITH_RSSI,
		      bt_hci_inquiry_result_with_rssi,
		      sizeof(struct bt_hci_evt_inquiry_result_with_rssi)),
	EVENT_HANDLER(BT_HCI_EVT_EXTENDED_INQUIRY_RESULT,
		      bt_hci_extended_inquiry_result,
		      sizeof(struct bt_hci_evt_extended_inquiry_result)),
	EVENT_HANDLER(BT_HCI_EVT_REMOTE_NAME_REQ_COMPLETE,
		      bt_hci_remote_name_request_complete,
		      sizeof(struct bt_hci_evt_remote_name_req_complete)),
	EVENT_HANDLER(BT_HCI_EVT_AUTH_COMPLETE, bt_hci_auth_complete,
		      sizeof(struct bt_hci_evt_auth_complete)),
	EVENT_HANDLER(BT_HCI_EVT_REMOTE_FEATURES,
		      bt_hci_read_remote_features_complete,
		      sizeof(struct bt_hci_evt_remote_features)),
	EVENT_HANDLER(BT_HCI_EVT_REMOTE_EXT_FEATURES,
		      bt_hci_read_remote_ext_features_complete,
		      sizeof(struct bt_hci_evt_remote_ext_features)),
	EVENT_HANDLER(BT_HCI_EVT_ROLE_CHANGE, bt_hci_role_change,
		      sizeof(struct bt_hci_evt_role_change)),
	EVENT_HANDLER(BT_HCI_EVT_SYNC_CONN_COMPLETE, bt_hci_synchronous_conn_complete,
		      sizeof(struct bt_hci_evt_sync_conn_complete)),
#endif /* CONFIG_BT_BREDR */
#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_DISCONN_COMPLETE, hci_disconn_complete,
		      sizeof(struct bt_hci_evt_disconn_complete)),
#endif /* CONFIG_BT_CONN */
#if ((defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U)) || (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U)))
	EVENT_HANDLER(BT_HCI_EVT_ENCRYPT_CHANGE, hci_encrypt_change,
		      sizeof(struct bt_hci_evt_encrypt_change)),
	EVENT_HANDLER(BT_HCI_EVT_ENCRYPT_KEY_REFRESH_COMPLETE,
		      hci_encrypt_key_refresh_complete,
		      sizeof(struct bt_hci_evt_encrypt_key_refresh_complete)),
#endif /* CONFIG_BT_SMP || CONFIG_BT_BREDR */
#if (defined(CONFIG_BT_REMOTE_VERSION) && ((CONFIG_BT_REMOTE_VERSION) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_REMOTE_VERSION_INFO,
		      bt_hci_evt_read_remote_version_complete,
		      sizeof(struct bt_hci_evt_remote_version_info)),
#endif /* CONFIG_BT_REMOTE_VERSION */
	EVENT_HANDLER(BT_HCI_EVT_HARDWARE_ERROR, hci_hardware_error,
		      sizeof(struct bt_hci_evt_hardware_error)),
};

/* @brief The HCI event shall be given to bt_recv_prio */
#define BT_HCI_EVT_FLAG_RECV_PRIO BIT(0)
/* @brief  The HCI event shall be given to bt_recv. */
#define BT_HCI_EVT_FLAG_RECV      BIT(1)

static uint8_t bt_hci_evt_get_flags(uint8_t evt, uint8_t *data, uint8_t len)
{
#if 0
	struct bt_hci_evt_le_meta_event *leEvt;
#endif
	switch (evt) {
	case BT_HCI_EVT_DISCONN_COMPLETE:
		return BT_HCI_EVT_FLAG_RECV | BT_HCI_EVT_FLAG_RECV_PRIO;
		/* fallthrough */
#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0U))
	case BT_HCI_EVT_NUM_COMPLETED_PACKETS:
	case BT_HCI_EVT_DATA_BUF_OVERFLOW:
#endif /* defined(CONFIG_BT_CONN) */
	case BT_HCI_EVT_CMD_COMPLETE:
	case BT_HCI_EVT_CMD_STATUS:
		return BT_HCI_EVT_FLAG_RECV_PRIO;
#if 0
#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0U))
	case BT_HCI_EVT_LE_META_EVENT:
		leEvt = (struct bt_hci_evt_le_meta_event *)data;
		if ((NULL != leEvt) && (len > 0)
		&& ((BT_HCI_EVT_LE_CONN_COMPLETE == leEvt->subevent)
		 || (BT_HCI_EVT_LE_ENH_CONN_COMPLETE == leEvt->subevent)))
		{
			return BT_HCI_EVT_FLAG_RECV_PRIO;
		}
		else
		{
			return BT_HCI_EVT_FLAG_RECV;
		}
#endif
#endif
	default:
		return BT_HCI_EVT_FLAG_RECV;
	}
}

static void hci_event(struct net_buf *buf)
{
	struct bt_hci_evt_hdr *hdr;

	assert(buf->len >= sizeof(*hdr));

	hdr = (struct bt_hci_evt_hdr *)net_buf_pull_mem(buf, sizeof(*hdr));
	BT_DBG("event 0x%02x", hdr->evt);
	assert(bt_hci_evt_get_flags(hdr->evt, buf->data, buf->len) & BT_HCI_EVT_FLAG_RECV);

	handle_event(hdr->evt, buf, normal_events, ARRAY_SIZE(normal_events));

	net_buf_unref(buf);
}

static bool send_cmd(void)
{
	struct net_buf *buf;
	int err;

	/* Get next command */
	BT_DBG("calling net_buf_get");
	buf = net_buf_get(bt_dev.cmd_tx_queue, osaWaitNone_c);
	if (NULL == buf)
	{
		return false;
	}
#if 0
	/* Wait until ncmd > 0 */
	BT_DBG("calling sem_take_wait");
	OSA_SemaphoreWait(bt_dev.ncmd_sem, osaWaitForever_c);
#endif
	/* Clear out any existing sent command */
	if (bt_dev.sent_cmd) {
		BT_ERR("Uncleared pending sent_cmd");
		net_buf_unref(bt_dev.sent_cmd);
		bt_dev.sent_cmd = NULL;
	}

	bt_dev.sent_cmd = net_buf_ref(buf);

	BT_DBG("Sending command 0x%04x (buf %p) to driver",
	       cmd(buf)->opcode, buf);

	err = bt_send(buf);
	if (err) {
		BT_ERR("Unable to send to driver (err %d)", err);
#if 0
		OSA_SemaphorePost(bt_dev.ncmd_sem);
#endif
		hci_cmd_done(cmd(buf)->opcode, BT_HCI_ERR_UNSPECIFIED, buf);
		net_buf_unref(buf);
	}
	return true;
}

#if 0
static void process_events(struct k_poll_event *ev, int count)
{
	BT_DBG("count %d", count);

	for (; count; ev++, count--) {
		BT_DBG("ev->state %u", ev->state);

		switch (ev->state) {
		case K_POLL_STATE_SIGNALED:
			break;
		case K_POLL_STATE_FIFO_DATA_AVAILABLE:
			if (ev->tag == BT_EVENT_CMD_TX) {
				send_cmd();
			} else if (IS_ENABLED(CONFIG_BT_CONN)) {
				struct bt_conn *conn;

				if (ev->tag == BT_EVENT_CONN_TX_QUEUE) {
					conn = CONTAINER_OF(ev->fifo,
							    struct bt_conn,
							    tx_queue);
					bt_conn_process_tx(conn);
				}
			}
			break;
		case K_POLL_STATE_NOT_READY:
			break;
		default:
			BT_WARN("Unexpected k_poll event state %u", ev->state);
			break;
		}
	}
}
#endif

/* HCI Command Complete handler */

static sys_slist_t hci_cmd_le_encrypt_rp_cb_list;

static void hci_rp_le_encrypt_handler(struct net_buf *buf)
{
	struct bt_hci_cmd_le_encrypt_rp_cb *tmp = NULL;
	struct bt_hci_cmd_le_encrypt_rp_cb *cb = NULL;

	/* Scan the callback list of hci command le encrypt response */
	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&hci_cmd_le_encrypt_rp_cb_list, cb, tmp, node, struct bt_hci_cmd_le_encrypt_rp_cb) {
        if (NULL != cb->cb)
		{
			cb->cb(buf, cb->user_data);
		}
	}
}
static const struct cmd_complete_handler normal_cmd_complete_handlers[] = {
	CMD_COMPLETE_HANDLER(BT_HCI_OP_LE_ENCRYPT, hci_rp_le_encrypt_handler, sizeof(struct bt_hci_rp_le_encrypt))
};

static void hci_cmd_complete_rsp_handlers(struct net_buf *buf)
{
	uint32_t count = ARRAY_SIZE(normal_cmd_complete_handlers);
	uint32_t index;
	struct bt_hci_evt_cmd_complete *evt;

	evt = (struct bt_hci_evt_cmd_complete *)buf->data;

	for (index = 0;index < count;index ++)
	{
		const struct cmd_complete_handler *handler = &normal_cmd_complete_handlers[index];

		if (handler->opcode != evt->opcode)
		{
			continue;
		}

		if (buf->len < (handler->min_len + sizeof(*evt)))
		{
			BT_ERR("Too small (%u bytes) opcode 0x%04x",
			       buf->len, evt->opcode);
			return;
		}

		handler->handler(buf);
		return;
	}
	BT_WARN("Unhandled event 0x%04x len %u: %s", evt->opcode,
		buf->len, bt_hex(buf->data, buf->len));
}

int hci_cmd_le_encrypt_rp_cb_register(struct bt_hci_cmd_le_encrypt_rp_cb *cb)
{
	if (false == sys_slist_find(&hci_cmd_le_encrypt_rp_cb_list, &cb->node))
	{
		sys_slist_prepend(&hci_cmd_le_encrypt_rp_cb_list, &cb->node);
	}
	return 0;
}

int hci_cmd_le_encrypt_rp_cb_unregister(struct bt_hci_cmd_le_encrypt_rp_cb *cb)
{
	if (true == sys_slist_find(&hci_cmd_le_encrypt_rp_cb_list, &cb->node))
	{
		sys_slist_find_and_remove(&hci_cmd_le_encrypt_rp_cb_list, &cb->node);
	}
	return 0;
}

static void hci_tx_thread(void *param)
{
    osa_status_t ret;
    osa_event_flags_t flags;

	BT_DBG("Started");

	while (1) {

		ret = OSA_EventWait(bt_dev.new_event, 0xffffffff, 0, osaWaitForever_c,&flags);

        if (BT_DEV_SEND_COMMAND & flags)
        {
            while (true == send_cmd())
            {
            }
        }
#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))
        else if (BT_DEV_SEND_ISO & flags)
        {
            struct bt_conn *conn;
            do
            {
                ret = OSA_MsgQGet(bt_dev.iso_conn, &conn, osaWaitNone_c);
                if (KOSA_StatusSuccess == ret)
                {
                    bt_conn_process_tx(conn);
                }
            } while (KOSA_StatusSuccess == ret);
        }
#endif /* CONFIG_BT_ISO */
        else if (BT_DEV_CONN_CHANGED & flags)
        {
            struct bt_conn *conn;
            do
            {
                ret = OSA_MsgQGet(bt_dev.conn_changed, &conn, osaWaitNone_c);
                if (KOSA_StatusSuccess == ret)
                {
                    bt_conn_process_tx(conn);
                }
            } while (KOSA_StatusSuccess == ret);
        }
        else
        {
        }

		/* Make sure we don't hog the CPU if there's all the time
		 * some ready events.
		 */
		OSA_TaskYield();
	}
}


static void read_local_ver_complete(struct net_buf *buf)
{
	struct bt_hci_rp_read_local_version_info *rp = (struct bt_hci_rp_read_local_version_info *)buf->data;

	BT_DBG("status 0x%02x", rp->status);

	bt_dev.hci_version = rp->hci_version;
	bt_dev.hci_revision = sys_le16_to_cpu(rp->hci_revision);
	bt_dev.lmp_version = rp->lmp_version;
	bt_dev.lmp_subversion = sys_le16_to_cpu(rp->lmp_subversion);
	bt_dev.manufacturer = sys_le16_to_cpu(rp->manufacturer);
}

static void read_le_features_complete(struct net_buf *buf)
{
	struct bt_hci_rp_le_read_local_features *rp = (struct bt_hci_rp_le_read_local_features *)buf->data;

	BT_DBG("status 0x%02x", rp->status);

#if (defined CONFIG_BT_CTLR_CONN_PARAM_REQ) && (CONFIG_BT_CTLR_CONN_PARAM_REQ == 0U)
	rp->features[(BT_LE_FEAT_BIT_CONN_PARAM_REQ) >> 3] = (rp->features[(BT_LE_FEAT_BIT_CONN_PARAM_REQ) >> 3] & (~BIT((BT_LE_FEAT_BIT_CONN_PARAM_REQ) & 7)));
#endif
	memcpy(bt_dev.le.features, rp->features, sizeof(bt_dev.le.features));
}

#if (defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0U))
#if !(defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0U))
static void read_buffer_size_complete(struct net_buf *buf)
{
	struct bt_hci_rp_read_buffer_size *rp = (void *)buf->data;
	uint16_t pkts;
	osa_status_t ret;

	BT_DBG("status 0x%02x", rp->status);

	/* If LE-side has buffers we can ignore the BR/EDR values */
	if (bt_dev.le.acl_mtu) {
		return;
	}

	bt_dev.le.acl_mtu = sys_le16_to_cpu(rp->acl_max_len);
	pkts = sys_le16_to_cpu(rp->acl_max_num);

	BT_DBG("ACL BR/EDR buffers: pkts %u mtu %u", pkts, bt_dev.le.acl_mtu);

    if (NULL == bt_dev.le.acl_pkts)
    {
        ret = OSA_SemaphoreCreate((osa_semaphore_handle_t)bt_dev.le.acl_pkts_handle, pkts);
        assert(KOSA_StatusSuccess == ret);

        if (KOSA_StatusSuccess == ret)
        {
            bt_dev.le.acl_pkts = (osa_semaphore_handle_t)bt_dev.le.acl_pkts_handle;
        }
    }
}
#endif /* !defined(CONFIG_BT_BREDR) */

static void le_read_buffer_size_complete(struct net_buf *buf)
{
	struct bt_hci_rp_le_read_buffer_size *rp = (struct bt_hci_rp_le_read_buffer_size *)buf->data;
	osa_status_t ret;

	BT_DBG("status 0x%02x", rp->status);

	bt_dev.le.acl_mtu = sys_le16_to_cpu(rp->le_max_len);
	if (!bt_dev.le.acl_mtu) {
		return;
	}

	BT_DBG("ACL LE buffers: pkts %u mtu %u", rp->le_max_num,
	       bt_dev.le.acl_mtu);

    if (NULL == bt_dev.le.acl_pkts)
    {
        ret = OSA_SemaphoreCreate((osa_semaphore_handle_t)bt_dev.le.acl_pkts_handle, sys_le16_to_cpu(rp->le_max_num));
        assert(KOSA_StatusSuccess == ret);

        if (KOSA_StatusSuccess == ret)
        {
            bt_dev.le.acl_pkts = (osa_semaphore_handle_t)bt_dev.le.acl_pkts_handle;
        }
    }
}

static void read_buffer_size_v2_complete(struct net_buf *buf)
{
#if (defined(CONFIG_BT_ISO) && ((CONFIG_BT_ISO) > 0U))
	struct bt_hci_rp_le_read_buffer_size_v2 *rp = (void *)buf->data;
	osa_status_t ret;
	uint8_t max_num;

	BT_DBG("status %u", rp->status);

#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0))
	bt_dev.le.acl_mtu = sys_le16_to_cpu(rp->acl_max_len);
	if (!bt_dev.le.acl_mtu) {
		return;
	}

	BT_DBG("ACL LE buffers: pkts %u mtu %u", rp->acl_max_num,
		bt_dev.le.acl_mtu);

	max_num = MIN(rp->acl_max_num, CONFIG_BT_CONN_TX_MAX);
	if (NULL == bt_dev.le.acl_pkts)
	{
		ret = OSA_SemaphoreCreate((osa_semaphore_handle_t)bt_dev.le.acl_pkts_handle, sys_le16_to_cpu(max_num));
		assert(KOSA_StatusSuccess == ret);

		if (KOSA_StatusSuccess == ret)
		{
			bt_dev.le.acl_pkts = (osa_semaphore_handle_t)bt_dev.le.acl_pkts_handle;
		}
	}
#endif /* CONFIG_BT_CONN */

	bt_dev.le.iso_mtu = sys_le16_to_cpu(rp->iso_max_len);
	if (!bt_dev.le.iso_mtu) {
		BT_ERR("ISO buffer size not set");
		return;
	}

	BT_DBG("ISO buffers: pkts %u mtu %u", rp->iso_max_num,
		bt_dev.le.iso_mtu);

	max_num = MIN(rp->iso_max_num, CONFIG_BT_ISO_TX_BUF_COUNT);
	if (NULL == bt_dev.le.iso_pkts)
	{
		ret = OSA_SemaphoreCreate((osa_semaphore_handle_t)bt_dev.le.iso_pkts_handle, sys_le16_to_cpu(max_num));
		assert(KOSA_StatusSuccess == ret);

		if (KOSA_StatusSuccess == ret)
		{
			bt_dev.le.iso_pkts = (osa_semaphore_handle_t)bt_dev.le.iso_pkts_handle;
		}
	}
#endif /* CONFIG_BT_ISO */
}

static int le_set_host_feature(uint8_t bit_number, uint8_t bit_value)
{
#if (defined(CONFIG_BT_ISO) && ((CONFIG_BT_ISO) > 0U))
	struct bt_hci_cp_le_set_host_feature *cp;
	struct net_buf *buf;

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_HOST_FEATURE, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = net_buf_add(buf, sizeof(*cp));
	cp->bit_number = bit_number;
	cp->bit_value = bit_value;

	return bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_HOST_FEATURE, buf, NULL);
#else
	return -ENOTSUP;
#endif /* CONFIG_BT_ISO */
}

#endif /* CONFIG_BT_CONN */

static void read_supported_commands_complete(struct net_buf *buf)
{
	struct bt_hci_rp_read_supported_commands *rp = (struct bt_hci_rp_read_supported_commands *)buf->data;

	BT_DBG("status 0x%02x", rp->status);

	memcpy(bt_dev.supported_commands, rp->commands,
	       sizeof(bt_dev.supported_commands));

	/* Report additional HCI commands used for ECDH as
	 * supported if TinyCrypt ECC is used for emulation.
	 */
	if (IS_ENABLED(CONFIG_BT_TINYCRYPT_ECC)) {
		bt_hci_ecc_supported_commands(bt_dev.supported_commands);
	}
}

static void read_local_features_complete(struct net_buf *buf)
{
	struct bt_hci_rp_read_local_features *rp = (struct bt_hci_rp_read_local_features *)buf->data;

	BT_DBG("status 0x%02x", rp->status);

	memcpy(bt_dev.features[0], rp->features, sizeof(bt_dev.features[0]));
}

static void le_read_supp_states_complete(struct net_buf *buf)
{
	struct bt_hci_rp_le_read_supp_states *rp = (struct bt_hci_rp_le_read_supp_states *)buf->data;

	BT_DBG("status 0x%02x", rp->status);

	bt_dev.le.states = sys_get_le64(rp->le_states);
}

#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
static void le_read_resolving_list_size_complete(struct net_buf *buf)
{
	struct bt_hci_rp_le_read_rl_size *rp = (struct bt_hci_rp_le_read_rl_size *)buf->data;

	BT_DBG("Resolving List size %u", rp->rl_size);

	bt_dev.le.rl_size = rp->rl_size;
}
#endif /* defined(CONFIG_BT_SMP) */

static int common_init(void)
{
	struct net_buf *rsp;
	int err;

#if 0
	if (!(bt_dev.drv->quirks & BT_QUIRK_NO_RESET)) {
#endif
#if (defined(BT_TESTER) && ((BT_TESTER) > 0U))
		/* Send HCI_RESET */
		err = bt_hci_cmd_send_sync(BT_HCI_OP_RESET, NULL, &rsp);
		if (err) {
			return err;
		}
		hci_reset_complete(rsp);
		net_buf_unref(rsp);
#endif /* BT_TESTER */
#if 0
	}
#endif
	/* Read Local Supported Features */
	err = bt_hci_cmd_send_sync(BT_HCI_OP_READ_LOCAL_FEATURES, NULL, &rsp);
	if (err) {
		return err;
	}
	read_local_features_complete(rsp);
	net_buf_unref(rsp);

	/* Read Local Version Information */
	err = bt_hci_cmd_send_sync(BT_HCI_OP_READ_LOCAL_VERSION_INFO, NULL,
				   &rsp);
	if (err) {
		return err;
	}
	read_local_ver_complete(rsp);
	net_buf_unref(rsp);

	/* Read Local Supported Commands */
	err = bt_hci_cmd_send_sync(BT_HCI_OP_READ_SUPPORTED_COMMANDS, NULL,
				   &rsp);
	if (err) {
		return err;
	}
	read_supported_commands_complete(rsp);
	net_buf_unref(rsp);

#if 0
	if (IS_ENABLED(CONFIG_BT_HOST_CRYPTO))
#endif
	{
		/* Initialize the PRNG so that it is safe to use it later
		 * on in the initialization process.
		 */
		err = prng_init();
		if (err) {
			return err;
		}
	}

#if (defined(CONFIG_BT_HCI_ACL_FLOW_CONTROL) && ((CONFIG_BT_HCI_ACL_FLOW_CONTROL) > 0U))
	err = set_flow_control();
	if (err) {
		return err;
	}
#endif /* CONFIG_BT_HCI_ACL_FLOW_CONTROL */

	return 0;
}

static int le_set_event_mask(void)
{
	struct bt_hci_cp_le_set_event_mask *cp_mask;
	struct net_buf *buf;
	uint64_t mask = 0U;

	/* Set LE event mask */
	buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_EVENT_MASK, sizeof(*cp_mask));
	if (!buf) {
		return -ENOBUFS;
	}

	cp_mask = (struct bt_hci_cp_le_set_event_mask *)net_buf_add(buf, sizeof(*cp_mask));

	mask |= BT_EVT_MASK_LE_ADVERTISING_REPORT;

	if (IS_ENABLED(CONFIG_BT_EXT_ADV) &&
	    BT_DEV_FEAT_LE_EXT_ADV(bt_dev.le.features)) {
		mask |= BT_EVT_MASK_LE_ADV_SET_TERMINATED;
		mask |= BT_EVT_MASK_LE_SCAN_REQ_RECEIVED;
		mask |= BT_EVT_MASK_LE_EXT_ADVERTISING_REPORT;
		mask |= BT_EVT_MASK_LE_SCAN_TIMEOUT;
		if (IS_ENABLED(CONFIG_BT_PER_ADV_SYNC)) {
			mask |= BT_EVT_MASK_LE_PER_ADV_SYNC_ESTABLISHED;
			mask |= BT_EVT_MASK_LE_PER_ADVERTISING_REPORT;
			mask |= BT_EVT_MASK_LE_PER_ADV_SYNC_LOST;
			mask |= BT_EVT_MASK_LE_PAST_RECEIVED;
		}
	}

	if (IS_ENABLED(CONFIG_BT_CONN)) {
		if ((IS_ENABLED(CONFIG_BT_SMP) &&
		     BT_FEAT_LE_PRIVACY(bt_dev.le.features)) ||
		    (IS_ENABLED(CONFIG_BT_EXT_ADV) &&
		     BT_DEV_FEAT_LE_EXT_ADV(bt_dev.le.features))) {
			/* C24:
			 * Mandatory if the LE Controller supports Connection
			 * State and either LE Feature (LL Privacy) or
			 * LE Feature (Extended Advertising) is supported, ...
			 */
			mask |= BT_EVT_MASK_LE_ENH_CONN_COMPLETE;
		} else {
			mask |= BT_EVT_MASK_LE_CONN_COMPLETE;
		}

		mask |= BT_EVT_MASK_LE_CONN_UPDATE_COMPLETE;
		mask |= BT_EVT_MASK_LE_REMOTE_FEAT_COMPLETE;

		if (BT_FEAT_LE_CONN_PARAM_REQ_PROC(bt_dev.le.features)) {
			mask |= BT_EVT_MASK_LE_CONN_PARAM_REQ;
		}

		if (IS_ENABLED(CONFIG_BT_DATA_LEN_UPDATE) &&
		    BT_FEAT_LE_DLE(bt_dev.le.features)) {
			mask |= BT_EVT_MASK_LE_DATA_LEN_CHANGE;
		}

		if (IS_ENABLED(CONFIG_BT_PHY_UPDATE) &&
		    (BT_FEAT_LE_PHY_2M(bt_dev.le.features) ||
		     BT_FEAT_LE_PHY_CODED(bt_dev.le.features))) {
			mask |= BT_EVT_MASK_LE_PHY_UPDATE_COMPLETE;
		}
	}

	if (IS_ENABLED(CONFIG_BT_SMP) &&
	    BT_FEAT_LE_ENCR(bt_dev.le.features)) {
		mask |= BT_EVT_MASK_LE_LTK_REQUEST;
	}

	/*
	 * If "LE Read Local P-256 Public Key" and "LE Generate DH Key" are
	 * supported we need to enable events generated by those commands.
	 */
	if (IS_ENABLED(CONFIG_BT_ECC) &&
	    (BT_CMD_TEST(bt_dev.supported_commands, 34, 1)) &&
	    (BT_CMD_TEST(bt_dev.supported_commands, 34, 2))) {
		mask |= BT_EVT_MASK_LE_P256_PUBLIC_KEY_COMPLETE;
		mask |= BT_EVT_MASK_LE_GENERATE_DHKEY_COMPLETE;
	}

	/*
	 * Enable CIS events only if ISO connections are enabled and controller
	 * support them.
	 */
	if (IS_ENABLED(CONFIG_BT_ISO) &&
	    BT_FEAT_LE_CIS(bt_dev.le.features)) {
		mask |= BT_EVT_MASK_LE_CIS_ESTABLISHED;
		if (BT_FEAT_LE_CIS_PERIPHERAL(bt_dev.le.features)) {
			mask |= BT_EVT_MASK_LE_CIS_REQ;
		}
	}

	/* Enable BIS events for broadcaster and/or receiver */
	if (IS_ENABLED(CONFIG_BT_ISO_BROADCAST) &&
	    BT_FEAT_LE_BIS(bt_dev.le.features)) {
		if (BT_FEAT_LE_ISO_BROADCASTER(bt_dev.le.features)) {
			mask |= BT_EVT_MASK_LE_BIG_COMPLETE;
			mask |= BT_EVT_MASK_LE_BIG_TERMINATED;
		}
		if (BT_FEAT_LE_SYNC_RECEIVER(bt_dev.le.features)) {
			mask |= BT_EVT_MASK_LE_BIG_SYNC_ESTABLISHED;
			mask |= BT_EVT_MASK_LE_BIG_SYNC_LOST;
			mask |= BT_EVT_MASK_LE_BIGINFO_ADV_REPORT;
		}
	}

	/* Enable IQ samples report events receiver */
	if (IS_ENABLED(CONFIG_BT_DF_CONNECTIONLESS_CTE_RX)) {
		mask |= BT_EVT_MASK_LE_CONNECTIONLESS_IQ_REPORT;
	}

	if (IS_ENABLED(CONFIG_BT_DF_CONNECTION_CTE_RX)) {
		mask |= BT_EVT_MASK_LE_CONNECTION_IQ_REPORT;
		mask |= BT_EVT_MASK_LE_CTE_REQUEST_FAILED;
	}

	sys_put_le64(mask, cp_mask->events);
	return bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_EVENT_MASK, buf, NULL);
}

static int le_init_iso(void)
{
	int err;
	struct net_buf *rsp;

	/* Set Isochronous Channels - Host support */
	err = le_set_host_feature(BT_LE_FEAT_BIT_ISO_CHANNELS, 1);
	if (err) {
		return err;
	}

	/* Octet 41, bit 5 is read buffer size V2 */
	if (BT_CMD_TEST(bt_dev.supported_commands, 41, 5)) {
		/* Read ISO Buffer Size V2 */
		err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_READ_BUFFER_SIZE_V2,
					   NULL, &rsp);
		if (err) {
			return err;
		}
		read_buffer_size_v2_complete(rsp);

		net_buf_unref(rsp);
	} else if (IS_ENABLED(CONFIG_BT_CONN)) {
		BT_WARN("Read Buffer Size V2 command is not supported."
			"No ISO buffers will be available");

		/* Read LE Buffer Size */
		err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_READ_BUFFER_SIZE,
					   NULL, &rsp);
		if (err) {
			return err;
		}
		le_read_buffer_size_complete(rsp);

		net_buf_unref(rsp);
	}

	return 0;
}

static int le_init(void)
{
	struct bt_hci_cp_write_le_host_supp *cp_le;
	struct net_buf *buf, *rsp;
	int err;

	/* For now we only support LE capable controllers */
	if (!BT_FEAT_LE(bt_dev.features)) {
		BT_ERR("Non-LE capable controller detected!");
		return -ENODEV;
	}

	/* Read Low Energy Supported Features */
	err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_READ_LOCAL_FEATURES, NULL,
				   &rsp);
	if (err) {
		return err;
	}

	read_le_features_complete(rsp);
	net_buf_unref(rsp);

#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0U))
	if (IS_ENABLED(CONFIG_BT_ISO) &&
	    BT_FEAT_LE_ISO(bt_dev.le.features)) {
		err = le_init_iso();
		if (err) {
			return err;
		}
	} else {
		/* Read LE Buffer Size */
		err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_READ_BUFFER_SIZE,
					   NULL, &rsp);
		if (err) {
			return err;
		}
		le_read_buffer_size_complete(rsp);
		net_buf_unref(rsp);
	}
#endif /* CONFIG_BT_CONN */

	if (BT_FEAT_BREDR(bt_dev.features)) {
		buf = bt_hci_cmd_create(BT_HCI_OP_LE_WRITE_LE_HOST_SUPP,
					sizeof(*cp_le));
		if (!buf) {
			return -ENOBUFS;
		}

		cp_le = (struct bt_hci_cp_write_le_host_supp *)net_buf_add(buf, sizeof(*cp_le));

		/* Explicitly enable LE for dual-mode controllers */
		cp_le->le = 0x01;
		cp_le->simul = 0x00;
		err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_WRITE_LE_HOST_SUPP, buf,
					   NULL);
		if (err) {
			return err;
		}
	}

	/* Read LE Supported States */
	if (BT_CMD_LE_STATES(bt_dev.supported_commands)) {
		err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_READ_SUPP_STATES, NULL,
					   &rsp);
		if (err) {
			return err;
		}

		le_read_supp_states_complete(rsp);
		net_buf_unref(rsp);
	}

	if (IS_ENABLED(CONFIG_BT_CONN) &&
	    IS_ENABLED(CONFIG_BT_DATA_LEN_UPDATE) &&
	    IS_ENABLED(CONFIG_BT_AUTO_DATA_LEN_UPDATE) &&
	    BT_FEAT_LE_DLE(bt_dev.le.features)) {
		struct bt_hci_cp_le_write_default_data_len *cp;
		uint16_t tx_octets, tx_time;

		err = hci_le_read_max_data_len(&tx_octets, &tx_time);
		if (err) {
			return err;
		}

		buf = bt_hci_cmd_create(BT_HCI_OP_LE_WRITE_DEFAULT_DATA_LEN,
					sizeof(*cp));
		if (!buf) {
			return -ENOBUFS;
		}

		cp = (struct bt_hci_cp_le_write_default_data_len *)net_buf_add(buf, sizeof(*cp));
		cp->max_tx_octets = sys_cpu_to_le16(tx_octets);
		cp->max_tx_time = sys_cpu_to_le16(tx_time);

		err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_WRITE_DEFAULT_DATA_LEN,
					   buf, NULL);
		if (err) {
			return err;
		}
	}

#if (defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U))
	if (BT_FEAT_LE_PRIVACY(bt_dev.le.features)) {
#if (defined(CONFIG_BT_PRIVACY) && ((CONFIG_BT_PRIVACY) > 0U))
		struct bt_hci_cp_le_set_rpa_timeout *cp;

		buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_RPA_TIMEOUT,
					sizeof(*cp));
		if (!buf) {
			return -ENOBUFS;
		}

		cp = (struct bt_hci_cp_le_set_rpa_timeout *)net_buf_add(buf, sizeof(*cp));
		cp->rpa_timeout = sys_cpu_to_le16(bt_dev.rpa_timeout);
		err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_RPA_TIMEOUT, buf,
					   NULL);
		if (err) {
			return err;
		}
#endif /* defined(CONFIG_BT_PRIVACY) */

		err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_READ_RL_SIZE, NULL,
					   &rsp);
		if (err) {
			return err;
		}
		le_read_resolving_list_size_complete(rsp);
		net_buf_unref(rsp);
	}
#endif

#if IS_ENABLED(CONFIG_BT_DF)
	if (BT_FEAT_LE_CONNECTIONLESS_CTE_TX(bt_dev.le.features) ||
	    BT_FEAT_LE_CONNECTIONLESS_CTE_RX(bt_dev.le.features) ||
	    BT_FEAT_LE_RX_CTE(bt_dev.le.features)) {
		err = le_df_init();
		if (err) {
			return err;
		}
	}
#endif /* CONFIG_BT_DF */

	return  le_set_event_mask();
}

#if !(defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0U))
static int bt_br_init(void)
{
#if (defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0U))
	struct net_buf *rsp;
	int err;

	if (bt_dev.le.acl_mtu) {
		return 0;
	}

	/* Use BR/EDR buffer size if LE reports zero buffers */
	err = bt_hci_cmd_send_sync(BT_HCI_OP_READ_BUFFER_SIZE, NULL, &rsp);
	if (err) {
		return err;
	}

	read_buffer_size_complete(rsp);
	net_buf_unref(rsp);
#endif /* CONFIG_BT_CONN */

	return 0;
}
#endif /* !defined(CONFIG_BT_BREDR) */

static int set_event_mask(void)
{
	struct bt_hci_cp_set_event_mask *ev;
	struct net_buf *buf;
	uint64_t mask = 0U;

	buf = bt_hci_cmd_create(BT_HCI_OP_SET_EVENT_MASK, sizeof(*ev));
	if (!buf) {
		return -ENOBUFS;
	}

	ev = (struct bt_hci_cp_set_event_mask *)net_buf_add(buf, sizeof(*ev));

	if (IS_ENABLED(CONFIG_BT_BREDR)) {
		/* Since we require LE support, we can count on a
		 * Bluetooth 4.0 feature set
		 */
		mask |= BT_EVT_MASK_INQUIRY_COMPLETE;
		mask |= BT_EVT_MASK_CONN_COMPLETE;
		mask |= BT_EVT_MASK_CONN_REQUEST;
		mask |= BT_EVT_MASK_AUTH_COMPLETE;
		mask |= BT_EVT_MASK_REMOTE_NAME_REQ_COMPLETE;
		mask |= BT_EVT_MASK_REMOTE_FEATURES;
		mask |= BT_EVT_MASK_ROLE_CHANGE;
		mask |= BT_EVT_MASK_PIN_CODE_REQ;
		mask |= BT_EVT_MASK_LINK_KEY_REQ;
		mask |= BT_EVT_MASK_LINK_KEY_NOTIFY;
		mask |= BT_EVT_MASK_INQUIRY_RESULT_WITH_RSSI;
		mask |= BT_EVT_MASK_REMOTE_EXT_FEATURES;
		mask |= BT_EVT_MASK_SYNC_CONN_COMPLETE;
		mask |= BT_EVT_MASK_EXTENDED_INQUIRY_RESULT;
		mask |= BT_EVT_MASK_IO_CAPA_REQ;
		mask |= BT_EVT_MASK_IO_CAPA_RESP;
		mask |= BT_EVT_MASK_USER_CONFIRM_REQ;
		mask |= BT_EVT_MASK_USER_PASSKEY_REQ;
		mask |= BT_EVT_MASK_SSP_COMPLETE;
		mask |= BT_EVT_MASK_USER_PASSKEY_NOTIFY;
	}

	mask |= BT_EVT_MASK_HARDWARE_ERROR;
	mask |= BT_EVT_MASK_DATA_BUFFER_OVERFLOW;
	mask |= BT_EVT_MASK_LE_META_EVENT;

	if (IS_ENABLED(CONFIG_BT_CONN)) {
		mask |= BT_EVT_MASK_DISCONN_COMPLETE;
		mask |= BT_EVT_MASK_REMOTE_VERSION_INFO;
	}

	if (IS_ENABLED(CONFIG_BT_SMP) &&
	    BT_FEAT_LE_ENCR(bt_dev.le.features)) {
		mask |= BT_EVT_MASK_ENCRYPT_CHANGE;
		mask |= BT_EVT_MASK_ENCRYPT_KEY_REFRESH_COMPLETE;
	}

	sys_put_le64(mask, ev->events);
	return bt_hci_cmd_send_sync(BT_HCI_OP_SET_EVENT_MASK, buf, NULL);
}

#if (defined(CONFIG_BT_DEBUG) && ((CONFIG_BT_DEBUG) > 0U))
static const char *ver_str(uint8_t ver)
{
	const char * const str[] = {
		"1.0b", "1.1", "1.2", "2.0", "2.1", "3.0", "4.0", "4.1", "4.2",
		"5.0", "5.1", "5.2", "5.3"
	};

	if (ver < ARRAY_SIZE(str)) {
		return str[ver];
	}

	return "unknown";
}

static void bt_dev_show_info(void)
{
	int i;

	BT_INFO("Identity%s: %s", bt_dev.id_count > 1 ? "[0]" : "",
		bt_addr_le_str(&bt_dev.id_addr[0]));

	if (IS_ENABLED(CONFIG_BT_LOG_SNIFFER_INFO)) {
#if (defined(CONFIG_BT_PRIVACY) && (CONFIG_BT_PRIVACY > 0U))
		uint8_t irk[16];

		sys_memcpy_swap(irk, bt_dev.irk[0], 16);
		BT_INFO("IRK%s: 0x%s", bt_dev.id_count > 1 ? "[0]" : "",
			bt_hex(irk, 16));
#endif
	}

	for (i = 1; i < bt_dev.id_count; i++) {
		BT_INFO("Identity[%d]: %s",
			i, bt_addr_le_str(&bt_dev.id_addr[i]));

		if (IS_ENABLED(CONFIG_BT_LOG_SNIFFER_INFO)) {
#if (defined(CONFIG_BT_PRIVACY) && (CONFIG_BT_PRIVACY > 0U))
			uint8_t irk[16];

			sys_memcpy_swap(irk, bt_dev.irk[i], 16);
			BT_INFO("IRK[%d]: 0x%s", i, bt_hex(irk, 16));
#endif
		}
	}

#if (defined(CONFIG_BT_LOG_SNIFFER_INFO) && (CONFIG_BT_LOG_SNIFFER_INFO > 0U))
	if (IS_ENABLED(CONFIG_BT_SMP) &&
	    IS_ENABLED(CONFIG_BT_LOG_SNIFFER_INFO)) {
		bt_keys_foreach_type(BT_KEYS_ALL, bt_keys_show_sniffer_info, NULL);
	}
#endif /* CONFIG_BT_LOG_SNIFFER_INFO */

	BT_INFO("HCI: version %s (0x%02x) revision 0x%04x, manufacturer 0x%04x",
		ver_str(bt_dev.hci_version), bt_dev.hci_version,
		bt_dev.hci_revision, bt_dev.manufacturer);
	BT_INFO("LMP: version %s (0x%02x) subver 0x%04x",
		ver_str(bt_dev.lmp_version), bt_dev.lmp_version,
		bt_dev.lmp_subversion);
}
#else
static inline void bt_dev_show_info(void)
{
}
#endif /* CONFIG_BT_DEBUG */

#if (defined(CONFIG_BT_HCI_VS_EXT) && ((CONFIG_BT_HCI_VS_EXT) > 0U))
#if (defined(CONFIG_BT_DEBUG) && ((CONFIG_BT_DEBUG) > 0U))
static const char *vs_hw_platform(uint16_t platform)
{
	static const char * const plat_str[] = {
		"reserved", "Intel Corporation", "Nordic Semiconductor",
		"NXP Semiconductors" };

	if (platform < ARRAY_SIZE(plat_str)) {
		return plat_str[platform];
	}

	return "unknown";
}

static const char *vs_hw_variant(uint16_t platform, uint16_t variant)
{
	static const char * const nordic_str[] = {
		"reserved", "nRF51x", "nRF52x", "nRF53x"
	};

	if (platform != BT_HCI_VS_HW_PLAT_NORDIC) {
		return "unknown";
	}

	if (variant < ARRAY_SIZE(nordic_str)) {
		return nordic_str[variant];
	}

	return "unknown";
}

static const char *vs_fw_variant(uint8_t variant)
{
	static const char * const var_str[] = {
		"Standard Bluetooth controller",
		"Vendor specific controller",
		"Firmware loader",
		"Rescue image",
	};

	if (variant < ARRAY_SIZE(var_str)) {
		return var_str[variant];
	}

	return "unknown";
}
#endif /* CONFIG_BT_DEBUG */

static void hci_vs_init(void)
{
	union {
		struct bt_hci_rp_vs_read_version_info *info;
		struct bt_hci_rp_vs_read_supported_commands *cmds;
		struct bt_hci_rp_vs_read_supported_features *feat;
	} rp;
	struct net_buf *rsp;
	int err;

	/* If heuristics is enabled, try to guess HCI VS support by looking
	 * at the HCI version and identity address. We haven't set any addresses
	 * at this point. So we need to read the public address.
	 */
	if (IS_ENABLED(CONFIG_BT_HCI_VS_EXT_DETECT)) {
		bt_addr_le_t addr;

		if ((bt_dev.hci_version < BT_HCI_VERSION_5_0) ||
		    bt_id_read_public_addr(&addr)) {
			BT_WARN("Controller doesn't seem to support "
				"Zephyr vendor HCI");
			return;
		}
	}

	err = bt_hci_cmd_send_sync(BT_HCI_OP_VS_READ_VERSION_INFO, NULL, &rsp);
	if (err) {
		BT_WARN("Vendor HCI extensions not available");
		return;
	}

	if (IS_ENABLED(CONFIG_BT_HCI_VS_EXT_DETECT) &&
	    rsp->len != sizeof(struct bt_hci_rp_vs_read_version_info)) {
		BT_WARN("Invalid Vendor HCI extensions");
		net_buf_unref(rsp);
		return;
	}

#if (defined(CONFIG_BT_DEBUG) && ((CONFIG_BT_DEBUG) > 0U))
	rp.info = (void *)rsp->data;
	BT_INFO("HW Platform: %s (0x%04x)",
		vs_hw_platform(sys_le16_to_cpu(rp.info->hw_platform)),
		sys_le16_to_cpu(rp.info->hw_platform));
	BT_INFO("HW Variant: %s (0x%04x)",
		vs_hw_variant(sys_le16_to_cpu(rp.info->hw_platform),
			      sys_le16_to_cpu(rp.info->hw_variant)),
		sys_le16_to_cpu(rp.info->hw_variant));
	BT_INFO("Firmware: %s (0x%02x) Version %u.%u Build %u",
		vs_fw_variant(rp.info->fw_variant), rp.info->fw_variant,
		rp.info->fw_version, sys_le16_to_cpu(rp.info->fw_revision),
		sys_le32_to_cpu(rp.info->fw_build));
#endif /* CONFIG_BT_DEBUG */

	net_buf_unref(rsp);

	err = bt_hci_cmd_send_sync(BT_HCI_OP_VS_READ_SUPPORTED_COMMANDS,
				   NULL, &rsp);
	if (err) {
		BT_WARN("Failed to read supported vendor commands");
		return;
	}

	if (IS_ENABLED(CONFIG_BT_HCI_VS_EXT_DETECT) &&
	    rsp->len != sizeof(struct bt_hci_rp_vs_read_supported_commands)) {
		BT_WARN("Invalid Vendor HCI extensions");
		net_buf_unref(rsp);
		return;
	}

	rp.cmds = (void *)rsp->data;
	memcpy(bt_dev.vs_commands, rp.cmds->commands, BT_DEV_VS_CMDS_MAX);
	net_buf_unref(rsp);

	if (BT_VS_CMD_SUP_FEAT(bt_dev.vs_commands)) {
		err = bt_hci_cmd_send_sync(BT_HCI_OP_VS_READ_SUPPORTED_FEATURES,
					   NULL, &rsp);
		if (err) {
			BT_WARN("Failed to read supported vendor features");
			return;
		}

		if (IS_ENABLED(CONFIG_BT_HCI_VS_EXT_DETECT) &&
		    rsp->len !=
		    sizeof(struct bt_hci_rp_vs_read_supported_features)) {
			BT_WARN("Invalid Vendor HCI extensions");
			net_buf_unref(rsp);
			return;
		}

		rp.feat = (void *)rsp->data;
		memcpy(bt_dev.vs_features, rp.feat->features,
		       BT_DEV_VS_FEAT_MAX);
		net_buf_unref(rsp);
	}
}
#endif /* CONFIG_BT_HCI_VS_EXT */

static int hci_init(void)
{
	int err;
#if (defined(CONFIG_BT_HCI_SETUP) && ((CONFIG_BT_HCI_SETUP) > 0))
	if (bt_dev.drv->setup) {
		err = bt_dev.drv->setup();
		if (err) {
			return err;
		}
	}
#endif /* defined(CONFIG_BT_HCI_SETUP) */

	err = common_init();
	if (err) {
		return err;
	}

	err = le_init();
	if (err) {
		return err;
	}

	if (BT_FEAT_BREDR(bt_dev.features)) {
		err = bt_br_init();
		if (err) {
			return err;
		}
	} else if (IS_ENABLED(CONFIG_BT_BREDR)) {
		BT_ERR("Non-BR/EDR controller detected");
		return -EIO;
	}
#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0))
	else if (!bt_dev.le.acl_mtu) {
		BT_ERR("ACL BR/EDR buffers not initialized");
		return -EIO;
	}
#endif

	err = set_event_mask();
	if (err) {
		return err;
	}

#if (defined(CONFIG_BT_HCI_VS_EXT) && ((CONFIG_BT_HCI_VS_EXT) > 0U))
	hci_vs_init();
#endif
	err = bt_id_init();
	if (err) {
		return err;
	}

	return 0;
}

int bt_send(struct net_buf *buf)
{
	API_RESULT ret = API_FAILURE;
	BT_DBG("buf %p len %u type %u", buf, buf->len, bt_buf_get_type(buf));

	bt_monitor_send(bt_monitor_opcode(buf), buf->data, buf->len);
#if 0
	if (IS_ENABLED(CONFIG_BT_TINYCRYPT_ECC)) {
		bt_hci_ecc_send(buf);
	}
#endif
	switch (bt_buf_get_type(buf))
    {
    case BT_BUF_CMD:
		{
			struct bt_hci_cmd_hdr *hdr = (struct bt_hci_cmd_hdr *)buf->data;

			ret = BT_hci_send_command(BT_OGF(hdr->opcode), BT_OCF(hdr->opcode), &(((uint8_t *)buf->data)[sizeof(*hdr)]), buf->len - sizeof(*hdr));
		}
        break;
#if (defined(CONFIG_BT_ISO_UNICAST) && (CONFIG_BT_ISO_UNICAST > 0)) || \
	(defined(CONFIG_BT_ISO_BROADCAST) && (CONFIG_BT_ISO_BROADCAST > 0))
	case BT_BUF_ISO_OUT:
		{
			struct bt_hci_iso_hdr *iso_hdr = (struct bt_hci_iso_hdr *)buf->data;
			ret = BT_hci_iso_write_pdu(bt_iso_handle(iso_hdr->handle), buf->data, buf->len);
		}
		break;
#endif
    case BT_BUF_ACL_OUT:
        break;
    default:
        BT_ERR("Unknown packet type %u", bt_buf_get_type(buf));
        break;
    }

    net_buf_unref(buf);

	return (API_SUCCESS == ret) ? 0 : -EIO;
}

static const struct event_handler prio_events[] = {
	EVENT_HANDLER(BT_HCI_EVT_CMD_COMPLETE, hci_cmd_complete,
		      sizeof(struct bt_hci_evt_cmd_complete)),
	EVENT_HANDLER(BT_HCI_EVT_CMD_STATUS, hci_cmd_status,
		      sizeof(struct bt_hci_evt_cmd_status)),
#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0U))
	EVENT_HANDLER(BT_HCI_EVT_DATA_BUF_OVERFLOW,
		      hci_data_buf_overflow,
		      sizeof(struct bt_hci_evt_data_buf_overflow)),
	EVENT_HANDLER(BT_HCI_EVT_DISCONN_COMPLETE, hci_disconn_complete_prio,
		      sizeof(struct bt_hci_evt_disconn_complete)),
	EVENT_HANDLER(BT_HCI_EVT_NUM_COMPLETED_PACKETS,
		      hci_num_completed_packets,
		      sizeof(struct bt_hci_evt_num_completed_packets)),

#endif /* CONFIG_BT_CONN */
};

void hci_event_prio(struct net_buf *buf)
{
	struct net_buf_simple_state state;
	struct bt_hci_evt_hdr *hdr;
	uint8_t evt_flags;

	net_buf_simple_save(&buf->b, &state);

	assert(buf->len >= sizeof(*hdr));

	hdr = (struct bt_hci_evt_hdr *)net_buf_pull_mem(buf, sizeof(*hdr));
	evt_flags = bt_hci_evt_get_flags(hdr->evt, buf->data, buf->len);
	assert(evt_flags & BT_HCI_EVT_FLAG_RECV_PRIO);

	handle_event(hdr->evt, buf, prio_events, ARRAY_SIZE(prio_events));

	if (evt_flags & BT_HCI_EVT_FLAG_RECV) {
		net_buf_simple_restore(&buf->b, &state);
	} else {
		net_buf_unref(buf);
	}
}

int bt_recv(struct net_buf *buf)
{
	bt_monitor_send(bt_monitor_opcode(buf), buf->data, buf->len);

	BT_DBG("buf %p len %u", buf, buf->len);

	switch (bt_buf_get_type(buf)) {
#if (defined(CONFIG_BT_CONN) && ((CONFIG_BT_CONN) > 0U))
	case BT_BUF_ACL_IN:
#if (defined(CONFIG_BT_RECV_IS_RX_THREAD) && (CONFIG_BT_RECV_IS_RX_THREAD > 0))
		hci_acl(buf);
#else
		net_buf_put(bt_dev.rx_queue, buf);
#endif
		return 0;
#endif /* BT_CONN */
	case BT_BUF_EVT:
	{

		struct bt_hci_evt_hdr *hdr = (struct bt_hci_evt_hdr *)buf->data;
		uint8_t evt_flags = bt_hci_evt_get_flags(hdr->evt, &buf->data[sizeof(*hdr)], buf->len - sizeof(*hdr));

		if (evt_flags & BT_HCI_EVT_FLAG_RECV_PRIO) {
			hci_event_prio(buf);
		}

		if (evt_flags & BT_HCI_EVT_FLAG_RECV) {
#if (defined(CONFIG_BT_RECV_IS_RX_THREAD) && (CONFIG_BT_RECV_IS_RX_THREAD > 0))
			hci_event(buf);
#else
			net_buf_put(bt_dev.rx_queue, buf);
#endif
		}
		return 0;
	}
#if (defined(CONFIG_BT_ISO) && ((CONFIG_BT_ISO) > 0U))
	case BT_BUF_ISO_IN:
#if (defined(CONFIG_BT_RECV_IS_RX_THREAD) && (CONFIG_BT_RECV_IS_RX_THREAD > 0))
		hci_iso(buf);
#else
		net_buf_put(bt_dev.rx_queue, buf);
#endif
		return 0;
#endif /* CONFIG_BT_ISO */
	default:
		BT_ERR("Invalid buf type %u", bt_buf_get_type(buf));
		net_buf_unref(buf);
		return -EINVAL;
	}
}

#if 0
int bt_recv_prio(struct net_buf *buf)
{
	bt_monitor_send(bt_monitor_opcode(buf), buf->data, buf->len);

	assert(bt_buf_get_type(buf) == BT_BUF_EVT);

	hci_event_prio(buf);

	return 0;
}
#endif /* defined(CONFIG_BT_RECV_IS_RX_THREAD) */

#if 0
int bt_hci_driver_register(const struct bt_hci_driver *drv)
{
	if (bt_dev.drv) {
		return -EALREADY;
	}

	if (!drv->open || !drv->send) {
		return -EINVAL;
	}

	bt_dev.drv = drv;

	BT_DBG("Registered %s", drv->name ? drv->name : "");

	bt_monitor_new_index(BT_MONITOR_TYPE_PRIMARY, drv->bus,
			     BT_ADDR_ANY, drv->name ? drv->name : "bt0");

	return 0;
}
#endif

void bt_finalize_init(void)
{
	atomic_set_bit(bt_dev.flags, BT_DEV_READY);

	if (IS_ENABLED(CONFIG_BT_OBSERVER)) {
		bt_le_scan_update(false);
	}

	bt_dev_show_info();
}

static int bt_init(void)
{
	int err;

	err = hci_init();
	if (err) {
		return err;
	}

	if (IS_ENABLED(CONFIG_BT_CONN)) {
		err = bt_conn_init();
		if (err) {
			return err;
		}
	}

	if (IS_ENABLED(CONFIG_BT_ISO)) {
		err = bt_conn_iso_init();
		if (err) {
			return err;
		}
	}

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		if (!bt_dev.id_count) {
			BT_INFO("No ID address. App must call settings_load()");
			return 0;
		}

		atomic_set_bit(bt_dev.flags, BT_DEV_PRESET_ID);
	}

	bt_finalize_init();
	return 0;
}

static void init_work(struct k_work *work)
{
	int err;

	err = bt_init();
	if (ready_cb) {
		ready_cb(err);
	}
    else
    {
        osa_status_t ret = OSA_SemaphorePost(bt_dev.init_done);
        assert(KOSA_StatusSuccess == ret);
        (void)ret;
    }
}

#if !(defined(CONFIG_BT_RECV_IS_RX_THREAD) && (CONFIG_BT_RECV_IS_RX_THREAD > 0))
static void hci_rx_thread(void* param)
{
	struct net_buf *buf;

	BT_DBG("started");

	while (1) {
		BT_DBG("calling fifo_get_wait");
		buf = net_buf_get(bt_dev.rx_queue, osaWaitForever_c);

		BT_DBG("buf %p type %u len %u", buf, bt_buf_get_type(buf),
		       buf->len);

		switch (bt_buf_get_type(buf)) {
#if (defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0))
		case BT_BUF_ACL_IN:
			hci_acl(buf);
			break;
#endif /* CONFIG_BT_CONN */
#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))
		case BT_BUF_ISO_IN:
			hci_iso(buf);
			break;
#endif /* CONFIG_BT_ISO */
		case BT_BUF_EVT:
			hci_event(buf);
			break;
		default:
			BT_ERR("Unknown buf type %u", bt_buf_get_type(buf));
			net_buf_unref(buf);
			break;
		}

		/* Make sure we don't hog the CPU if the rx_queue never
		 * gets empty.
		 */
		OSA_TaskYield();
	}
}
#endif /* !CONFIG_BT_RECV_IS_RX_THREAD */

uint16_t ethermind_hci_event_callback
           (uint8_t  event_type,
            uint8_t *event_data,
            uint8_t  event_datalen)
{
	struct net_buf *buf = NULL;
    struct bt_hci_evt_hdr hdr;
    uint16_t opcode = 0;
    BT_DBG("eventCode %d len %d 0x%02X 0x%02X 0x%02X\r\n", event_type, event_datalen, event_data[0], event_data[1], event_data[2]);

	if ((BT_HCI_EVT_CMD_COMPLETE == event_type) || (BT_HCI_EVT_CMD_STATUS == event_type))
	{
        if (NULL != bt_dev.sent_cmd)
        {
            struct bt_hci_evt_cmd_complete *cmdComplete = (struct bt_hci_evt_cmd_complete *)event_data;
            if (BT_HCI_EVT_CMD_STATUS == event_type)
            {
                cmdComplete = (struct bt_hci_evt_cmd_complete *)&event_data[1];
            }
            opcode = cmdComplete->opcode;
            if (cmd(bt_dev.sent_cmd)->opcode == opcode)
            {
                buf = bt_buf_get_cmd_complete(osaWaitNone_c);
            }
        }
        if ((NULL != bt_dev.sent_cmd) && (NULL == buf))
        {
            BT_WARN("OPCode %0d is not found", opcode);
        }
        if (NULL == buf)
        {
            buf = bt_buf_get_rx(BT_BUF_EVT, osaWaitForever_c);
        }
	}
	else
	{
		buf = bt_buf_get_evt(event_type, false, osaWaitForever_c);
	}

    hdr.evt = event_type;
    hdr.len = event_datalen;
    (void)net_buf_add_mem(buf, &hdr, sizeof(hdr));
    (void)net_buf_add_mem(buf, event_data, event_datalen);
	bt_recv(buf);

    return API_SUCCESS;
}

#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))
API_RESULT ethermind_iso_data_in_callback(UCHAR *header, UCHAR *data, UINT16 datalen)
{
	struct net_buf *buf = bt_buf_get_rx(BT_BUF_ISO_IN, osaWaitForever_c);

	/* Copy ISO data to buf. */
	struct bt_hci_iso_hdr *hdr = (struct bt_hci_iso_hdr *)header;
	uint16_t handle = sys_le16_to_cpu(hdr->handle);
	uint8_t flags = bt_iso_flags(handle);
	uint8_t ts = bt_iso_flags_ts(flags);
	uint16_t len = datalen + (8U + (4U * ts));
	net_buf_add_mem(buf, header, len);

	if(0 != bt_recv(buf))
	{
		return API_FAILURE;
	}

	return API_SUCCESS;
}
#endif

void bt_br_acl_link_connect_req(bt_addr_t *peer, uint32_t cod)
{
    struct bt_hci_evt_conn_request evt;

    memcpy(&evt.bdaddr, peer, sizeof(evt.bdaddr));
    memcpy(evt.dev_class, &cod, sizeof(evt.dev_class));
    evt.link_type = BT_HCI_ACL;

    ethermind_hci_event_callback(BT_HCI_EVT_CONN_REQUEST, (UCHAR *)&evt, sizeof(evt));
}

static API_RESULT ethermind_bt_on_complete ( void )
{
    /* Set default link policy to support role switch and sniff mode */
    BT_hci_write_default_link_policy_settings(0x05);
#if 0
    BT_hci_read_local_supported_features();
#ifdef BT_LE
    /* Register LE support in Host with controller */
    BT_hci_write_le_host_support(0x01, 0x00);
#endif /* BT_LE */
#endif
    k_work_submit(&bt_dev.init);
    return API_SUCCESS;
}

int bt_enable(bt_ready_cb_t cb)
{
    API_RESULT retval;
    osa_status_t ret;
    int err = 0;

	if (atomic_test_and_set_bit(bt_dev.flags, BT_DEV_ENABLE))
	{
		return -EALREADY;
	}
#if (defined(CONFIG_BT_DEVICE_NAME_DYNAMIC) && ((CONFIG_BT_DEVICE_NAME_DYNAMIC) > 0U))
	strncpy(bt_dev.name, CONFIG_BT_DEVICE_NAME, MIN(sizeof(CONFIG_BT_DEVICE_NAME), sizeof(bt_dev.name)));
	bt_dev.name[MIN(sizeof(CONFIG_BT_DEVICE_NAME), sizeof(bt_dev.name))] = '\0';
#endif
    ret = OSA_SemaphoreCreate((osa_semaphore_handle_t)bt_dev.init_done_handle, 0);
    if (KOSA_StatusSuccess == ret)
    {
        bt_dev.init_done = (osa_semaphore_handle_t)bt_dev.init_done_handle;
    }
    assert(KOSA_StatusSuccess == ret);

    ret = OSA_MsgQCreate((osa_msgq_handle_t)bt_dev.cmd_tx_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void *));
    assert(KOSA_StatusSuccess == ret);
    bt_dev.cmd_tx_queue = (osa_msgq_handle_t)bt_dev.cmd_tx_queue_handle;

#if !(defined(CONFIG_BT_RECV_IS_RX_THREAD) && (CONFIG_BT_RECV_IS_RX_THREAD > 0))
    ret = OSA_MsgQCreate((osa_msgq_handle_t)bt_dev.rx_queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void *));
    assert(KOSA_StatusSuccess == ret);
    bt_dev.rx_queue = (osa_msgq_handle_t)bt_dev.rx_queue_handle;
#endif

	if (NULL == bt_dev.new_event)
	{
		ret = OSA_EventCreate((osa_event_handle_t)bt_dev.new_event_handle, 1);
		if (KOSA_StatusSuccess == ret)
		{
			bt_dev.new_event = (osa_event_handle_t)bt_dev.new_event_handle;
		}
		assert(KOSA_StatusSuccess == ret);
	}

    ret = OSA_MsgQCreate((osa_msgq_handle_t)bt_dev.conn_changed_handle, EV_COUNT, sizeof(void *));
    assert(KOSA_StatusSuccess == ret);
    bt_dev.conn_changed = (osa_msgq_handle_t)bt_dev.conn_changed_handle;

#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))
    ret = OSA_MsgQCreate((osa_msgq_handle_t)bt_dev.iso_conn_handle, EV_COUNT, sizeof(void *));
    assert(KOSA_StatusSuccess == ret);
    bt_dev.iso_conn = (osa_msgq_handle_t)bt_dev.iso_conn_handle;
#endif

    bt_ble_porting_init();

    k_work_queue_init();

	k_work_init(&bt_dev.init, init_work);

	/* Initialize OSAL */
    bt_ble_platform_init();
    controller_init();
    EM_os_init();
    EM_debug_init();
    EM_timer_init();
    timer_em_init();

	if (IS_ENABLED(CONFIG_BT_SETTINGS))
	{
		err = bt_settings_init();
	}

	if (0 != err)
	{
		return err;
	}
	else
	{
		(void)bt_set_name(CONFIG_BT_DEVICE_NAME);
	}

	ready_cb = cb;

	/* TX thread */
	ret = OSA_TaskCreate((osa_task_handle_t)tx_thread_data, OSA_TASK(hci_tx_thread), NULL);
	assert(KOSA_StatusSuccess == ret);

#if !(defined(CONFIG_BT_RECV_IS_RX_THREAD) && (CONFIG_BT_RECV_IS_RX_THREAD > 0))
	/* RX thread */
	ret = OSA_TaskCreate((osa_task_handle_t)rx_thread_data, OSA_TASK(hci_rx_thread), NULL);
	assert(KOSA_StatusSuccess == ret);
#endif

    BT_ethermind_init();

    /* TODO: Enable Snoop Logging */
#if (defined(CONFIG_BT_SNOOP) && (CONFIG_BT_SNOOP > 0))
    BT_snoop_logging_enable();
#else
    BT_snoop_logging_disable();
#endif

    retval = BT_bluetooth_on
         (
             ethermind_hci_event_callback,
             ethermind_bt_on_complete,
             (CHAR *)CONFIG_BT_DEVICE_NAME
         );
    assert(API_SUCCESS == retval);

#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))
	BT_hci_register_iso_data_handler(ethermind_iso_data_in_callback);
#endif

    if (!cb) {
        ret = OSA_SemaphoreWait(bt_dev.init_done, BT_INIT_TIMEOUT);
        assert(KOSA_StatusSuccess == ret);
        if (KOSA_StatusSuccess != ret)
        {
            return -EHOSTDOWN;
        }
	}
    (void)retval;
    (void)err;
	return 0;
}

#if 0 /* Not verified yet. */
int bt_disable(void)
{

    int err;
    API_RESULT result;
    osa_status_t ret;

    if (atomic_test_and_set_bit(bt_dev.flags, BT_DEV_DISABLE)) {
            return -EALREADY;
    }

    /* Clear BT_DEV_READY before disabling HCI link */
    atomic_clear_bit(bt_dev.flags, BT_DEV_READY);

    result = BT_bluetooth_off();
    if(API_SUCCESS != result)
    {
            /* Re-enable BT_DEV_READY to avoid inconsistent stack state */
            atomic_set_bit(bt_dev.flags, BT_DEV_READY);

            return -EHOSTDOWN;
    }

    (void)BT_hci_register_iso_data_handler(NULL);

#if (defined(CONFIG_BT_SNOOP) && (CONFIG_BT_SNOOP > 0))
    BT_snoop_logging_disable();
#endif

#ifdef BT_HAVE_SHUTDOWN
    BT_ethermind_shutdown();
#endif

    /* TX thread */
    ret = OSA_TaskDestroy((osa_task_handle_t)tx_thread_data);
    assert(KOSA_StatusSuccess == ret);

#if !(defined(CONFIG_BT_RECV_IS_RX_THREAD) && (CONFIG_BT_RECV_IS_RX_THREAD > 0))
    /* RX thread */
    ret = OSA_TaskDestroy((osa_task_handle_t)rx_thread_data);
    assert(KOSA_StatusSuccess == ret);
#endif

    ready_cb = NULL;

	/* Initialize OSAL */
    timer_em_init();
    EM_timer_init();
    EM_debug_init();
    EM_os_init();
    controller_init();
    bt_ble_platform_init();

    ret = OSA_MsgQDestroy((osa_msgq_handle_t)bt_dev.conn_changed_handle);
    assert(KOSA_StatusSuccess == ret);
    bt_dev.conn_changed = NULL;

#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))
    ret = OSA_MsgQDestroy((osa_msgq_handle_t)bt_dev.iso_conn_handle);
    assert(KOSA_StatusSuccess == ret);
    bt_dev.iso_conn = NULL;
#endif

    if (NULL != bt_dev.new_event)
    {
        ret = OSA_EventDestroy((osa_event_handle_t)bt_dev.new_event_handle);
        assert(KOSA_StatusSuccess == ret);
        bt_dev.new_event = NULL;
    }

    /* Some functions rely on checking this bitfield */
    memset(bt_dev.supported_commands, 0x00, sizeof(bt_dev.supported_commands));

    /* If random address was set up - clear it */
    bt_addr_le_copy(&bt_dev.random_addr, BT_ADDR_LE_ANY);

    ret = OSA_SemaphoreDestroy((osa_semaphore_handle_t)bt_dev.init_done_handle);
    assert(KOSA_StatusSuccess == ret);
    bt_dev.init_done = NULL;

    ret = OSA_MsgQDestroy((osa_msgq_handle_t)bt_dev.cmd_tx_queue_handle);
    assert(KOSA_StatusSuccess == ret);
    bt_dev.cmd_tx_queue = NULL;

#if !(defined(CONFIG_BT_RECV_IS_RX_THREAD) && (CONFIG_BT_RECV_IS_RX_THREAD > 0))
    ret = OSA_MsgQDestroy((osa_msgq_handle_t)bt_dev.rx_queue_handle);
    assert(KOSA_StatusSuccess == ret);
    bt_dev.rx_queue = NULL;
#endif

	bt_monitor_send(BT_MONITOR_CLOSE_INDEX, NULL, 0);

#if (defined(CONFIG_BT_PER_ADV_SYNC) && (CONFIG_BT_PER_ADV_SYNC > 0))
	bt_periodic_sync_disable();
#endif /* CONFIG_BT_PER_ADV_SYNC */

    /* Clear BT_DEV_ENABLE here to prevent early bt_enable() calls, before disable is
     * completed.
     */
    atomic_clear_bit(bt_dev.flags, BT_DEV_ENABLE);


    return 0;
}
#endif

bool bt_is_ready(void)
{
	return atomic_test_bit(bt_dev.flags, BT_DEV_READY);
}

int bt_set_name(const char *name)
{
#if (defined(CONFIG_BT_DEVICE_NAME_DYNAMIC) && (CONFIG_BT_DEVICE_NAME_DYNAMIC > 0))
	size_t len = strlen(name);
	int err;

	if (len > CONFIG_BT_DEVICE_NAME_MAX) {
		return -ENOMEM;
	}

	if (!strcmp(bt_dev.name, name)) {
		return 0;
	}

	strncpy(bt_dev.name, name, len);
	bt_dev.name[len] = '\0';

#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		err = settings_save_one("bt/name", bt_dev.name, len);
		if (err) {
			BT_WARN("Unable to store name");
		}
	}
#endif

	return 0;
#else
	return -ENOMEM;
#endif
}

const char *bt_get_name(void)
{
#if (defined(CONFIG_BT_DEVICE_NAME_DYNAMIC) && ((CONFIG_BT_DEVICE_NAME_DYNAMIC) > 0U))
	return bt_dev.name;
#else
	return CONFIG_BT_DEVICE_NAME;
#endif
}

uint16_t bt_get_appearance(void)
{
#if (defined(CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC) && (CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC > 0))
	return bt_dev.appearance;
#else
	return CONFIG_BT_DEVICE_APPEARANCE;
#endif
}

#if (defined(CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC) && (CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC > 0))
int bt_set_appearance(uint16_t appearance)
{
	if (bt_dev.appearance != appearance) {
		if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
			int err = settings_save_one("bt/appearance", &appearance,
					sizeof(appearance));

			if (err) {
				BT_ERR("Unable to save setting 'bt/appearance' (err %d).", err);
				return err;
			}
		}

		bt_dev.appearance = appearance;
	}

	return 0;
}
#endif

bool bt_addr_le_is_bonded(uint8_t id, const bt_addr_le_t *addr)
{
	if (IS_ENABLED(CONFIG_BT_SMP)) {
		struct bt_keys *keys = bt_keys_find_addr(id, addr);

		/* if there are any keys stored then device is bonded */
		return keys && keys->keys;
	} else {
		return false;
	}
}

#if (defined(CONFIG_BT_FILTER_ACCEPT_LIST) && (CONFIG_BT_FILTER_ACCEPT_LIST > 0U))
int bt_le_filter_accept_list_add(const bt_addr_le_t *addr)
{
	struct bt_hci_cp_le_add_dev_to_fal *cp;
	struct net_buf *buf;
	int err;

	if (!atomic_test_bit(bt_dev.flags, BT_DEV_READY)) {
		return -EAGAIN;
	}

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_ADD_DEV_TO_FAL, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = (struct bt_hci_cp_le_add_dev_to_fal *)net_buf_add(buf, sizeof(*cp));
	bt_addr_le_copy(&cp->addr, addr);

	err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_ADD_DEV_TO_FAL, buf, NULL);
	if (err) {
		BT_ERR("Failed to add device to filter accept list");

		return err;
	}

	return 0;
}

int bt_le_filter_accept_list_remove(const bt_addr_le_t *addr)
{
	struct bt_hci_cp_le_rem_dev_from_fal *cp;
	struct net_buf *buf;
	int err;

	if (!atomic_test_bit(bt_dev.flags, BT_DEV_READY)) {
		return -EAGAIN;
	}

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_REM_DEV_FROM_FAL, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = (struct bt_hci_cp_le_rem_dev_from_fal *)net_buf_add(buf, sizeof(*cp));
	bt_addr_le_copy(&cp->addr, addr);

	err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_REM_DEV_FROM_FAL, buf, NULL);
	if (err) {
		BT_ERR("Failed to remove device from filter accept list");
		return err;
	}

	return 0;
}

int bt_le_filter_accept_list_clear(void)
{
	int err;

	if (!atomic_test_bit(bt_dev.flags, BT_DEV_READY)) {
		return -EAGAIN;
	}

	err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_CLEAR_FAL, NULL, NULL);
	if (err) {
		BT_ERR("Failed to clear filter accept list");
		return err;
	}

	return 0;
}
#endif /* defined(CONFIG_BT_FILTER_ACCEPT_LIST) */

int bt_le_set_chan_map(uint8_t chan_map[5])
{
	struct bt_hci_cp_le_set_host_chan_classif *cp;
	struct net_buf *buf;

	if (!IS_ENABLED(CONFIG_BT_CENTRAL)) {
		return -ENOTSUP;
	}
    else
    {
        if (!BT_CMD_TEST(bt_dev.supported_commands, 27, 3)) {
            BT_WARN("Set Host Channel Classification command is "
                "not supported");
            return -ENOTSUP;
        }

        buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_HOST_CHAN_CLASSIF,
                    sizeof(*cp));
        if (!buf) {
            return -ENOBUFS;
        }

        cp = (struct bt_hci_cp_le_set_host_chan_classif *)net_buf_add(buf, sizeof(*cp));

        memcpy(&cp->ch_map[0], &chan_map[0], 4);
        cp->ch_map[4] = chan_map[4] & BIT_MASK(5);

        return bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_HOST_CHAN_CLASSIF,
                        buf, NULL);
    }
}

#if (defined(CONFIG_BT_RPA_TIMEOUT_DYNAMIC) && (CONFIG_BT_RPA_TIMEOUT_DYNAMIC > 0))
int bt_le_set_rpa_timeout(uint16_t new_rpa_timeout)
{
	if ((new_rpa_timeout == 0) || (new_rpa_timeout > 3600)) {
		return -EINVAL;
	}

	if (new_rpa_timeout == bt_dev.rpa_timeout) {
		return 0;
	}

	bt_dev.rpa_timeout = new_rpa_timeout;
	atomic_set_bit(bt_dev.flags, BT_DEV_RPA_TIMEOUT_CHANGED);

	return 0;
}
#endif

void bt_data_parse(struct net_buf_simple *ad,
		   bool (*func)(struct bt_data *data, void *user_data),
		   void *user_data)
{
	while (ad->len > 1) {
		struct bt_data data;
		uint8_t len;

		len = net_buf_simple_pull_u8(ad);
		if (len == 0U) {
			/* Early termination */
			return;
		}

		if (len > ad->len) {
			BT_WARN("Malformed data");
			return;
		}

		data.type = net_buf_simple_pull_u8(ad);
		data.data_len = len - 1;
		data.data = ad->data;

		if (!func(&data, user_data)) {
			return;
		}

		net_buf_simple_pull(ad, len - 1);
	}
}

int bt_configure_data_path(uint8_t dir, uint8_t id, uint8_t vs_config_len,
			   const uint8_t *vs_config)
{
	struct bt_hci_rp_configure_data_path *rp;
	struct bt_hci_cp_configure_data_path *cp;
	struct net_buf *rsp;
	struct net_buf *buf;
	int err;

	buf = bt_hci_cmd_create(BT_HCI_OP_CONFIGURE_DATA_PATH, sizeof(*cp) +
				vs_config_len);
	if (!buf) {
		return -ENOBUFS;
	}

	cp = net_buf_add(buf, sizeof(*cp));
	cp->data_path_dir = dir;
	cp->data_path_id  = id;
	cp->vs_config_len = vs_config_len;
	if (vs_config_len) {
		(void)memcpy(cp->vs_config, vs_config, vs_config_len);
	}

	err = bt_hci_cmd_send_sync(BT_HCI_OP_CONFIGURE_DATA_PATH, buf, &rsp);
	if (err) {
		return err;
	}

	rp = (void *)rsp->data;
	if (rp->status) {
		err = -EIO;
	}
	net_buf_unref(rsp);

	return err;
}
