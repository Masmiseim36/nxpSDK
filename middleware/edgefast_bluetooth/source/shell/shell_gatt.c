/** @file
 * @brief Bluetooth GATT shell functions
 *
 */

/*
 * Copyright (c) 2017 Intel Corporation
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

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>

#include "fsl_shell.h"

#include "shell_bt.h"

#define CHAR_SIZE_MAX           512

#if (defined(CONFIG_BT_GATT_CLIENT) && (CONFIG_BT_GATT_CLIENT > 0)) || \
	(defined(CONFIG_BT_GATT_DYNAMIC_DB) && (CONFIG_BT_GATT_DYNAMIC_DB > 0))
extern uint8_t selected_id;

static struct write_stats {
	uint32_t count;
	uint32_t len;
	uint32_t total;
	uint32_t rate;
} write_stats;

static void update_write_stats(uint16_t len)
{
	static uint32_t cycle_stamp;
	uint32_t delta;

	delta = (uint32_t)OSA_TimeGetMsec() - cycle_stamp;
	delta = (uint32_t)(delta * 1000);

	if (!delta) {
		delta = 1;
	}

	write_stats.count++;
	write_stats.total += len;

	/* if last data rx-ed was greater than 1 second in the past,
	 * reset the metrics.
	 */
	if (delta > 1000000000) {
		write_stats.len = 0U;
		write_stats.rate = 0U;
		cycle_stamp = (uint32_t)OSA_TimeGetMsec();
	} else {
		write_stats.len += len;
		write_stats.rate = ((uint64_t)write_stats.len << 3) *
			1000000000U / delta;
	}
}

#if (defined(CONFIG_BT_EATT) && (CONFIG_BT_EATT > 0))
#define SET_CHAN_OPT_ANY(params) (params).chan_opt = BT_ATT_CHAN_OPT_NONE
#else
#define SET_CHAN_OPT_ANY(params)
#endif /* CONFIG_BT_EATT */

static void print_write_stats(void)
{
	shell_print(ctx_shell, "Write #%u: %u bytes (%u bps)",
		    write_stats.count, write_stats.total, write_stats.rate);
}
#endif /* CONFIG_BT_GATT_CLIENT || CONFIG_BT_GATT_DYNAMIC_DB */

#if (defined(CONFIG_BT_GATT_CLIENT) && (CONFIG_BT_GATT_CLIENT > 0))
static void reset_write_stats(void)
{
	memset(&write_stats, 0, sizeof(write_stats));
}

/* This variable is write-locked when `(exchange_params.func != NULL)`.
 * Must be zero-initialized when unlocked.
 */
static struct bt_gatt_exchange_params exchange_params;

static void exchange_func(struct bt_conn *conn, uint8_t err,
			  struct bt_gatt_exchange_params *params)
{
	shell_print(ctx_shell, "Exchange %s", err == 0U ? "successful" :
		    "failed");

	/* Release global `exchange_params`. */
	__ASSERT_NO_MSG(params == &exchange_params);
	(void)memset(params, 0, sizeof(*params));
}

static shell_status_t cmd_exchange_mtu(shell_handle_t shell,
			     int32_t argc, char *argv[])
{
	int err;

	if (!default_conn) {
		shell_print(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	if (exchange_params.func) {
		shell_print(shell, "Shell command busy. A previous invocation is in progress.");
		return kStatus_SHELL_Error;
	}

	exchange_params.func = exchange_func;

	err = bt_gatt_exchange_mtu(default_conn, &exchange_params);
	if (err) {
		/* Release global `exchange_params`. */
		exchange_params.func = NULL;
	}

	if (err == -EALREADY) {
		shell_print(shell, "Already exchanged");
	} else if (err) {
		shell_print(shell, "Exchange failed (err %d)", err);
	} else {
		shell_print(shell, "Exchange pending");
	}

	return (shell_status_t)err;
}

static struct bt_gatt_discover_params discover_params;
static struct bt_uuid_16 uuid = BT_UUID_INIT_16(0);

static void print_chrc_props(shell_handle_t shell, uint8_t properties)
{
	shell_print(shell, "Properties: ");

	if (properties & BT_GATT_CHRC_BROADCAST) {
		shell_print(shell, "[bcast]");
	}

	if (properties & BT_GATT_CHRC_READ) {
		shell_print(shell, "[read]");
	}

	if (properties & BT_GATT_CHRC_WRITE) {
		shell_print(shell, "[write]");
	}

	if (properties & BT_GATT_CHRC_WRITE_WITHOUT_RESP) {
		shell_print(shell, "[write w/w rsp]");
	}

	if (properties & BT_GATT_CHRC_NOTIFY) {
		shell_print(shell, "[notify]");
	}

	if (properties & BT_GATT_CHRC_INDICATE) {
		shell_print(shell, "[indicate]");
	}

	if (properties & BT_GATT_CHRC_AUTH) {
		shell_print(shell, "[auth]");
	}

	if (properties & BT_GATT_CHRC_EXT_PROP) {
		shell_print(shell, "[ext prop]");
	}

	shell_print(shell, "");
}

static uint8_t discover_func(struct bt_conn *conn,
			     const struct bt_gatt_attr *attr,
			     struct bt_gatt_discover_params *params)
{
	struct bt_gatt_service_val *gatt_service;
	struct bt_gatt_chrc *gatt_chrc;
	struct bt_gatt_include *gatt_include;
	char str[BT_UUID_STR_LEN];

	if (!attr) {
		shell_print(ctx_shell, "Discover complete");
		(void)memset(params, 0, sizeof(*params));
		return BT_GATT_ITER_STOP;
	}

	switch (params->type) {
	case BT_GATT_DISCOVER_SECONDARY:
	case BT_GATT_DISCOVER_PRIMARY:
		gatt_service = (struct bt_gatt_service_val *)attr->user_data;
		bt_uuid_to_str(gatt_service->uuid, str, sizeof(str));
		shell_print(ctx_shell, "Service %s found: start handle %x, "
			    "end_handle %x", str, attr->handle,
			    gatt_service->end_handle);
		break;
	case BT_GATT_DISCOVER_CHARACTERISTIC:
		gatt_chrc = (struct bt_gatt_chrc *)attr->user_data;
		bt_uuid_to_str(gatt_chrc->uuid, str, sizeof(str));
		shell_print(ctx_shell, "Characteristic %s found: handle %x",
			    str, attr->handle);
		print_chrc_props(ctx_shell, gatt_chrc->properties);
		break;
	case BT_GATT_DISCOVER_INCLUDE:
		gatt_include = (struct bt_gatt_include *)attr->user_data;
		bt_uuid_to_str(gatt_include->uuid, str, sizeof(str));
		shell_print(ctx_shell, "Include %s found: handle %x, start %x, "
			    "end %x", str, attr->handle,
			    gatt_include->start_handle,
			    gatt_include->end_handle);
		break;
	default:
		bt_uuid_to_str(attr->uuid, str, sizeof(str));
		shell_print(ctx_shell, "Descriptor %s found: handle %x", str,
			    attr->handle);
		break;
	}

	return BT_GATT_ITER_CONTINUE;
}

static shell_status_t cmd_discover(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	if (discover_params.func) {
		shell_print(shell, "Discover ongoing");
		return kStatus_SHELL_Error;
	}

	discover_params.func = discover_func;
	discover_params.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE;
	discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;
	SET_CHAN_OPT_ANY(discover_params);

	if (argc > 1) {
		/* Only set the UUID if the value is valid (non zero) */
		uuid.val = strtoul(argv[1], NULL, 16);
		if (uuid.val) {
			discover_params.uuid = &uuid.uuid;
		}
	}

	if (argc > 2) {
		discover_params.start_handle = strtoul(argv[2], NULL, 16);
		if (argc > 3) {
			discover_params.end_handle = strtoul(argv[3], NULL, 16);
		}
	}

	if (!strcmp(argv[0], "discover")) {
		discover_params.type = BT_GATT_DISCOVER_ATTRIBUTE;
	} else if (!strcmp(argv[0], "discover-secondary")) {
		discover_params.type = BT_GATT_DISCOVER_SECONDARY;
	} else if (!strcmp(argv[0], "discover-include")) {
		discover_params.type = BT_GATT_DISCOVER_INCLUDE;
	} else if (!strcmp(argv[0], "discover-characteristic")) {
		discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;
	} else if (!strcmp(argv[0], "discover-descriptor")) {
		discover_params.type = BT_GATT_DISCOVER_DESCRIPTOR;
	} else {
		discover_params.type = BT_GATT_DISCOVER_PRIMARY;
	}

	err = bt_gatt_discover(default_conn, &discover_params);
	if (err) {
		shell_error(shell, "Discover failed (err %d)", err);
		(void)memset(&discover_params, 0, sizeof(discover_params));
	} else {
		shell_print(shell, "Discover pending");
	}

	return (shell_status_t)err;
}

static struct bt_gatt_read_params read_params;

static uint8_t read_func(struct bt_conn *conn, uint8_t err,
			 struct bt_gatt_read_params *params,
			 const void *data, uint16_t length)
{
	shell_print(ctx_shell, "Read complete: err 0x%02x length %u", err, length);

	if (!data) {
		(void)memset(params, 0, sizeof(*params));
		return BT_GATT_ITER_STOP;
	} else {
		shell_hexdump(ctx_shell, data, length);
	}

	return BT_GATT_ITER_CONTINUE;
}

static shell_status_t cmd_read(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	if (read_params.func) {
		shell_print(shell, "Read ongoing");
		return kStatus_SHELL_Error;
	}

	read_params.func = read_func;
	read_params.handle_count = 1;
	read_params.single.handle = strtoul(argv[1], NULL, 16);
	read_params.single.offset = 0U;
	SET_CHAN_OPT_ANY(read_params);

	if (argc > 2) {
		read_params.single.offset = strtoul(argv[2], NULL, 16);
	}

	err = bt_gatt_read(default_conn, &read_params);
	if (err) {
		shell_error(shell, "Read failed (err %d)", err);
	} else {
		shell_print(shell, "Read pending");
	}

	return (shell_status_t)err;
}

static shell_status_t cmd_mread(shell_handle_t shell, int32_t argc, char *argv[])
{
	uint16_t h[8];
	size_t i;
	int err;

	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	if (read_params.func) {
		shell_print(shell, "Read ongoing");
		return kStatus_SHELL_Error;
	}

	if ((argc - 1) >  ARRAY_SIZE(h)) {
		shell_print(shell, "Enter max %lu handle items to read",
			    ARRAY_SIZE(h));
		return (shell_status_t)-EINVAL;
	}

	for (i = 0; i < argc - 1; i++) {
		h[i] = strtoul(argv[i + 1], NULL, 16);
	}

	read_params.func = read_func;
	read_params.handle_count = i;
	read_params.multiple.handles = h;
	read_params.multiple.variable = true;
	SET_CHAN_OPT_ANY(read_params);

	err = bt_gatt_read(default_conn, &read_params);
	if (err) {
		shell_error(shell, "GATT multiple read request failed (err %d)",
			    err);
	}

	return (shell_status_t)err;
}

static shell_status_t cmd_read_uuid(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	if (read_params.func) {
		shell_print(shell, "Read ongoing");
		return kStatus_SHELL_Error;
	}

	read_params.func = read_func;
	read_params.handle_count = 0;
	read_params.by_uuid.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE;
	read_params.by_uuid.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;
	SET_CHAN_OPT_ANY(read_params);

	if (argc > 1) {
		uuid.val = strtoul(argv[1], NULL, 16);
		if (uuid.val) {
			read_params.by_uuid.uuid = &uuid.uuid;
		}
	}

	if (argc > 2) {
		read_params.by_uuid.start_handle = strtoul(argv[2], NULL, 16);
		if (argc > 3) {
			read_params.by_uuid.end_handle = strtoul(argv[3],
								 NULL, 16);
		}
	}

	err = bt_gatt_read(default_conn, &read_params);
	if (err) {
		shell_error(shell, "Read failed (err %d)", err);
	} else {
		shell_print(shell, "Read pending");
	}

	return (shell_status_t)err;
}

static struct bt_gatt_write_params write_params;
static uint8_t gatt_write_buf[CHAR_SIZE_MAX];

static void write_func(struct bt_conn *conn, uint8_t err,
		       struct bt_gatt_write_params *params)
{
	shell_print(ctx_shell, "Write complete: err 0x%02x", err);

	(void)memset(&write_params, 0, sizeof(write_params));
}

static shell_status_t cmd_write(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;
	uint16_t handle, offset;

	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	if (write_params.func) {
		shell_error(shell, "Write ongoing");
		return kStatus_SHELL_Error;
	}

	handle = strtoul(argv[1], NULL, 16);
	offset = strtoul(argv[2], NULL, 16);

	write_params.length = hex2bin(argv[3], strlen(argv[3]),
				      gatt_write_buf, sizeof(gatt_write_buf));
	if (write_params.length == 0) {
		shell_error(shell, "No data set");
		return (shell_status_t)kStatus_SHELL_Error;
	}

	write_params.data = gatt_write_buf;
	write_params.handle = handle;
	write_params.offset = offset;
	write_params.func = write_func;
	SET_CHAN_OPT_ANY(write_params);

	err = bt_gatt_write(default_conn, &write_params);
	if (err) {
		write_params.func = NULL;
		shell_error(shell, "Write failed (err %d)", err);
	} else {
		shell_print(shell, "Write pending");
	}

	return (shell_status_t)err;
}

static void write_without_rsp_cb(struct bt_conn *conn, void *user_data)
{
	uint16_t len = POINTER_TO_UINT(user_data);

	update_write_stats(len);

	print_write_stats();
}

static shell_status_t cmd_write_without_rsp(shell_handle_t shell,
				 int32_t argc, char *argv[])
{
	uint16_t handle;
	uint16_t repeat;
	int err = -1;
	uint16_t len;
	bool sign;
	bt_gatt_complete_func_t func = NULL;

	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	sign = !strcmp(argv[0], "signed-write");
	if (!sign) {
		if (!strcmp(argv[0], "write-without-response-cb")) {
			func = write_without_rsp_cb;
			reset_write_stats();
		}
	}

	handle = strtoul(argv[1], NULL, 16);
	gatt_write_buf[0] = strtoul(argv[2], NULL, 16);
	len = 1U;

	if (argc > 3) {
		int i;

		len = MIN(strtoul(argv[3], NULL, 16), sizeof(gatt_write_buf));

		for (i = 1; i < len; i++) {
			gatt_write_buf[i] = gatt_write_buf[0];
		}
	}

	repeat = 0U;

	if (argc > 4) {
		repeat = strtoul(argv[4], NULL, 16);
	}

	if (!repeat) {
		repeat = 1U;
	}

	while (repeat--) {
		err = bt_gatt_write_without_response_cb(default_conn, handle,
							gatt_write_buf, len,
							sign, func,
							UINT_TO_POINTER(len));
		if (err) {
			break;
		}

		OSA_TaskYield();

	}

	shell_print(shell, "Write Complete (err %d)", err);
	return (shell_status_t)err;
}

static struct bt_gatt_subscribe_params subscribe_params;

static uint8_t notify_func(struct bt_conn *conn,
			struct bt_gatt_subscribe_params *params,
			const void *data, uint16_t length)
{
	if (!data) {
		shell_print(ctx_shell, "Unsubscribed");
		params->value_handle = 0U;
		return BT_GATT_ITER_STOP;
	}

	shell_print(ctx_shell, "Notification: data %p length %u", data, length);

	return BT_GATT_ITER_CONTINUE;
}

static shell_status_t cmd_subscribe(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	if (subscribe_params.value_handle) {
		shell_error(shell, "Cannot subscribe: subscription to %x "
			    "already exists", subscribe_params.value_handle);
		return kStatus_SHELL_Error;
	}

	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	subscribe_params.ccc_handle = strtoul(argv[1], NULL, 16);
	subscribe_params.value_handle = strtoul(argv[2], NULL, 16);
	subscribe_params.value = BT_GATT_CCC_NOTIFY;
	subscribe_params.notify = notify_func;
	SET_CHAN_OPT_ANY(subscribe_params);

#if (defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) && (CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
	if (subscribe_params.ccc_handle == 0) {
		static struct bt_gatt_discover_params disc_params;

		subscribe_params.disc_params = &disc_params;
		subscribe_params.end_handle = 0xFFFF;
	}
#endif /* CONFIG_BT_GATT_AUTO_DISCOVER_CCC */


	if (argc > 3 && !strcmp(argv[3], "ind")) {
		subscribe_params.value = BT_GATT_CCC_INDICATE;
	}

	err = bt_gatt_subscribe(default_conn, &subscribe_params);
	if (err) {
		subscribe_params.value_handle = 0U;
		shell_error(shell, "Subscribe failed (err %d)", err);
	} else {
		shell_print(shell, "Subscribed");
	}

	return (shell_status_t)err;
}

static shell_status_t cmd_resubscribe(shell_handle_t shell, int32_t argc,
				char *argv[])
{
	bt_addr_le_t addr;
	int err;

	if (subscribe_params.value_handle) {
		shell_error(shell, "Cannot resubscribe: subscription to %x"
			    " already exists", subscribe_params.value_handle);
		return kStatus_SHELL_Error;
	}

	err = bt_addr_le_from_str(argv[1], argv[2], &addr);
	if (err) {
		shell_error(shell, "Invalid peer address (err %d)", err);
		return kStatus_SHELL_Error;
	}

	subscribe_params.ccc_handle = strtoul(argv[3], NULL, 16);
	subscribe_params.value_handle = strtoul(argv[4], NULL, 16);
	subscribe_params.value = BT_GATT_CCC_NOTIFY;
	subscribe_params.notify = notify_func;
	SET_CHAN_OPT_ANY(subscribe_params);

	if (argc > 5 && !strcmp(argv[5], "ind")) {
		subscribe_params.value = BT_GATT_CCC_INDICATE;
	}

	err = bt_gatt_resubscribe(selected_id, &addr, &subscribe_params);
	if (err) {
		subscribe_params.value_handle = 0U;
		shell_error(shell, "Resubscribe failed (err %d)", err);
	} else {
		shell_print(shell, "Resubscribed");
	}

	return (shell_status_t)err;
}

static shell_status_t cmd_unsubscribe(shell_handle_t shell,
			   int32_t argc, char *argv[])
{
	int err;

	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	if (!subscribe_params.value_handle) {
		shell_error(shell, "No subscription found");
		return kStatus_SHELL_Error;
	}

	err = bt_gatt_unsubscribe(default_conn, &subscribe_params);
	if (err) {
		shell_error(shell, "Unsubscribe failed (err %d)", err);
	} else {
		shell_print(shell, "Unsubscribe success");
	}

	return (shell_status_t)err;
}
#endif /* CONFIG_BT_GATT_CLIENT */

static struct db_stats {
	uint16_t svc_count;
	uint16_t attr_count;
	uint16_t chrc_count;
	uint16_t ccc_count;
} stats;

static uint8_t print_attr(const struct bt_gatt_attr *attr, uint16_t handle,
			  void *user_data)
{
	shell_handle_t shell = (shell_handle_t)user_data;
	char str[BT_UUID_STR_LEN];

	stats.attr_count++;

	if (!bt_uuid_cmp(attr->uuid, BT_UUID_GATT_PRIMARY) ||
	    !bt_uuid_cmp(attr->uuid, BT_UUID_GATT_SECONDARY)) {
		stats.svc_count++;
	}

	if (!bt_uuid_cmp(attr->uuid, BT_UUID_GATT_CHRC)) {
		stats.chrc_count++;
	}

	if (!bt_uuid_cmp(attr->uuid, BT_UUID_GATT_CCC) &&
	    attr->write == bt_gatt_attr_write_ccc) {
		stats.ccc_count++;
	}

	bt_uuid_to_str(attr->uuid, str, sizeof(str));
	shell_print(shell, "attr %p handle 0x%04x uuid %s perm 0x%02x",
		    attr, handle, str, attr->perm);

	return BT_GATT_ITER_CONTINUE;
}

static shell_status_t cmd_show_db(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_uuid_16 uuid;
	size_t total_len;

	memset(&stats, 0, sizeof(stats));

	if (argc > 1) {
		uint16_t num_matches = 0;

		uuid.uuid.type = BT_UUID_TYPE_16;
		uuid.val = strtoul(argv[1], NULL, 16);

		if (argc > 2) {
			num_matches = strtoul(argv[2], NULL, 10);
		}

		bt_gatt_foreach_attr_type(0x0001, 0xffff, &uuid.uuid, NULL,
					  num_matches, print_attr,
					  (void *)shell);
		return kStatus_SHELL_Success;
	}

	bt_gatt_foreach_attr(0x0001, 0xffff, print_attr, (void *)shell);

	if (!stats.attr_count) {
		shell_print(shell, "No attribute found");
		return kStatus_SHELL_Success;
	}

	total_len = stats.svc_count * sizeof(struct bt_gatt_service);
	total_len += stats.chrc_count * sizeof(struct bt_gatt_chrc);
	total_len += stats.attr_count * sizeof(struct bt_gatt_attr);
	total_len += stats.ccc_count * sizeof(struct _bt_gatt_ccc);

	shell_print(shell, "=================================================");
	shell_print(shell, "Total: %u services %u attributes (%zu bytes)",
		    stats.svc_count, stats.attr_count, total_len);

	return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_GATT_DYNAMIC_DB) && (CONFIG_BT_GATT_DYNAMIC_DB > 0))
/* Custom Service Variables */

static struct bt_uuid_128 vnd_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0));

static struct bt_uuid_128 vnd_auth_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef2));

static const struct bt_uuid_128 vnd_long_uuid1 = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef3));

static const struct bt_uuid_128 vnd_long_uuid2 = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x12340, 0x5678cefaadde));

static uint8_t vnd_value[] = { 'V', 'e', 'n', 'd', 'o', 'r' };

static struct bt_uuid_128 vnd1_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x12340, 0x56789abcdef4));

static const struct bt_uuid_128 vnd1_echo_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x12340, 0x56789abcdef5));

static uint8_t echo_enabled;

static void vnd1_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	echo_enabled = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

static ssize_t write_vnd1(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			  const void *buf, uint16_t len, uint16_t offset,
			  uint8_t flags)
{
	if (echo_enabled) {
		shell_print(ctx_shell, "Echo attr len %u", len);
		bt_gatt_notify(conn, attr, buf, len);
	}

	return len;
}

static ssize_t read_vnd(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			void *buf, uint16_t len, uint16_t offset)
{
	const char *value = (const char *)attr->user_data;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 strlen(value));
}

static ssize_t write_vnd(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 const void *buf, uint16_t len, uint16_t offset,
			 uint8_t flags)
{
	uint8_t *value = (uint8_t *)(uint8_t *)attr->user_data;

	if (offset + len > sizeof(vnd_value)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	memcpy(value + offset, buf, len);
    shell_hexdump(NULL, buf, len);

	return len;
}

#define MAX_DATA 30
static uint8_t vnd_long_value1[MAX_DATA] = { 'V', 'e', 'n', 'd', 'o', 'r' };
static uint8_t vnd_long_value2[MAX_DATA] = { 'S', 't', 'r', 'i', 'n', 'g' };

static ssize_t read_long_vnd(struct bt_conn *conn,
			     const struct bt_gatt_attr *attr, void *buf,
			     uint16_t len, uint16_t offset)
{
	uint8_t *value = (uint8_t *)attr->user_data;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 sizeof(vnd_long_value1));
}

static ssize_t write_long_vnd(struct bt_conn *conn,
			      const struct bt_gatt_attr *attr, const void *buf,
			      uint16_t len, uint16_t offset, uint8_t flags)
{
	uint8_t *value = (uint8_t *)attr->user_data;

	if (flags & BT_GATT_WRITE_FLAG_PREPARE) {
		return kStatus_SHELL_Success;
	}

	if (offset + len > sizeof(vnd_long_value1)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	/* Copy to buffer */
	memcpy(value + offset, buf, len);

    shell_hexdump(NULL, buf, len);

	return len;
}

static struct bt_gatt_attr vnd_attrs[] = {
	/* Vendor Primary Service Declaration */
	BT_GATT_PRIMARY_SERVICE(&vnd_uuid),

	BT_GATT_CHARACTERISTIC(&vnd_auth_uuid.uuid,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_READ_AUTHEN |
			       BT_GATT_PERM_WRITE_AUTHEN,
			       read_vnd, write_vnd, vnd_value),

	BT_GATT_CHARACTERISTIC(&vnd_long_uuid1.uuid, BT_GATT_CHRC_READ |
			       BT_GATT_CHRC_WRITE | BT_GATT_CHRC_EXT_PROP,
			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE |
			       BT_GATT_PERM_PREPARE_WRITE,
			       read_long_vnd, write_long_vnd,
			       &vnd_long_value1),

	BT_GATT_CHARACTERISTIC(&vnd_long_uuid2.uuid, BT_GATT_CHRC_READ |
			       BT_GATT_CHRC_WRITE | BT_GATT_CHRC_EXT_PROP,
			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE |
			       BT_GATT_PERM_PREPARE_WRITE,
			       read_long_vnd, write_long_vnd,
			       &vnd_long_value2),
};

static struct bt_gatt_service vnd_svc = BT_GATT_SERVICE(vnd_attrs);

static struct bt_gatt_attr vnd1_attrs[] = {
	/* Vendor Primary Service Declaration */
	BT_GATT_PRIMARY_SERVICE(&vnd1_uuid),

	BT_GATT_CHARACTERISTIC(&vnd1_echo_uuid.uuid,
			       BT_GATT_CHRC_WRITE_WITHOUT_RESP |
			       BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_WRITE, NULL, write_vnd1, NULL),
	BT_GATT_CCC(vnd1_ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
};

static struct bt_gatt_service vnd1_svc = BT_GATT_SERVICE(vnd1_attrs);

static shell_status_t cmd_register_test_svc(shell_handle_t shell,
				  int32_t argc, char *argv[])
{
	char str[BT_UUID_STR_LEN];
	int err;

	bt_uuid_to_str(&vnd_uuid.uuid, str, sizeof(str));
	err = bt_gatt_service_register(&vnd_svc);
	if (!err) {
		shell_print(shell, "Registered test vendor service %s", str);
	} else {
		shell_error(shell, "Failed to register test vendor service %s (%d)", str, err);
	}

	bt_uuid_to_str(&vnd1_uuid.uuid, str, sizeof(str));
	err = bt_gatt_service_register(&vnd1_svc);
	if (!err) {
		shell_print(shell, "Registered test vendor service %s", str);
	} else {
		shell_error(shell, "Failed to register test vendor service %s (%d)", str, err);
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_unregister_test_svc(shell_handle_t shell,
				    int32_t argc, char *argv[])
{
	char str[BT_UUID_STR_LEN];
	int err;

	bt_uuid_to_str(&vnd_uuid.uuid, str, sizeof(str));
	err = bt_gatt_service_unregister(&vnd_svc);
	if (!err) {
		shell_print(shell, "Unregistered test vendor service %s", str);
	} else {
		shell_error(shell, "Failed to unregister test vendor service %s (%d)", str, err);
	}

	bt_uuid_to_str(&vnd1_uuid.uuid, str, sizeof(str));
	err = bt_gatt_service_unregister(&vnd1_svc);
	if (!err) {
		shell_print(shell, "Unregistered test vendor service %s", str);
	} else {
		shell_error(shell, "Failed to unregister test vendor service %s (%d)", str, err);
	}

	return kStatus_SHELL_Success;
}

static void notify_cb(struct bt_conn *conn, void *user_data)
{
	shell_handle_t shell = user_data;

	shell_print(shell, "Nofication sent to conn %p", conn);
}

static shell_status_t cmd_notify(shell_handle_t shell, int32_t argc, char *argv[])
{
	const size_t max_cnt = CONFIG_BT_L2CAP_TX_BUF_COUNT;
	struct bt_gatt_notify_params params[CONFIG_BT_L2CAP_TX_BUF_COUNT];
	const size_t min_cnt = 1U;
	unsigned long data;
	unsigned long cnt;
	uint16_t cnt_u16;
	int err = 0;

	if (!default_conn) {
		shell_error(shell, "Not connected.");

		return kStatus_SHELL_Error;
	}

	if (!echo_enabled) {
		shell_error(shell, "No clients have enabled notifications for the vnd1_echo CCC.");
		return kStatus_SHELL_Error;
	}

	cnt = shell_strtoul(argv[1], 10, &err);
	if (err != 0) {
		shell_error(shell, "Invalid count parameter: %s", argv[1]);

		return kStatus_SHELL_Error;
	}

	if (!IN_RANGE(cnt, min_cnt, max_cnt)) {
		shell_error(shell, "Invalid count value %lu (range %zu to %zu)",
			    cnt, min_cnt, max_cnt);

		return kStatus_SHELL_Error;
	}

	cnt_u16 = (uint16_t)cnt;

	if (argc > 2) {
		data = shell_strtoul(argv[2], 16, &err);
		if (err != 0) {
			shell_error(shell, "Invalid data parameter: %s", argv[1]);

			return kStatus_SHELL_Error;
		}
	}

	(void)memset(params, 0, sizeof(params));

	for (uint16_t i = 0U; i < cnt_u16; i++) {
		params[i].uuid = 0;
		params[i].attr = vnd1_attrs;
		params[i].data = &data;
		params[i].len = sizeof(data);
		params[i].func = notify_cb;
		params[i].user_data = (void *)shell;
		SET_CHAN_OPT_ANY(params[i]);
	}

	err = bt_gatt_notify_multiple(default_conn, cnt_u16, params);
	if (err != 0) {
		shell_error(shell, "bt_gatt_notify_multiple failed: %d", err);
	} else {
		shell_print(shell, "Send %u notifications", cnt_u16);
	}

	return (shell_status_t)err;
}

#if (defined(CONFIG_BT_GATT_NOTIFY_MULTIPLE) && (CONFIG_BT_GATT_NOTIFY_MULTIPLE > 0))
static shell_status_t cmd_notify_mult(shell_handle_t shell, int32_t argc, char *argv[])
{
	const size_t max_cnt = CONFIG_BT_L2CAP_TX_BUF_COUNT;
	struct bt_gatt_notify_params params[CONFIG_BT_L2CAP_TX_BUF_COUNT];
	const size_t min_cnt = 1U;
	unsigned long data;
	unsigned long cnt;
	uint16_t cnt_u16;
	int err = 0;

	if (!default_conn) {
		shell_error(shell, "Not connected.");

		return kStatus_SHELL_Error;
	}

	if (!echo_enabled) {
		shell_error(shell, "No clients have enabled notifications for the vnd1_echo CCC.");

		return kStatus_SHELL_Error;
	}

	cnt = shell_strtoul(argv[1], 10, &err);
	if (err != 0) {
		shell_error(shell, "Invalid count parameter: %s", argv[1]);

		return kStatus_SHELL_Error;
	}

	if (!IN_RANGE(cnt, min_cnt, max_cnt)) {
		shell_error(shell, "Invalid count value %lu (range %zu to %zu)",
			    cnt, min_cnt, max_cnt);

		return kStatus_SHELL_Error;
	}

	cnt_u16 = (uint16_t)cnt;

	if (argc > 2) {
		data = shell_strtoul(argv[2], 16, &err);
		if (err != 0) {
			shell_error(shell, "Invalid data parameter: %s", argv[1]);

			return kStatus_SHELL_Error;
		}
	}

	(void)memset(params, 0, sizeof(params));

	for (uint16_t i = 0U; i < cnt_u16; i++) {
		params[i].uuid = 0;
		params[i].attr = vnd1_attrs;
		params[i].data = &data;
		params[i].len = sizeof(data);
		params[i].func = notify_cb;
		params[i].user_data = (void *)shell;
	}

	err = bt_gatt_notify_multiple(default_conn, cnt_u16, params);
	if (err != 0) {
		shell_error(shell, "bt_gatt_notify_multiple failed: %d", err);
	} else {
		shell_print(shell, "Send %u notifications", cnt_u16);
	}

	return (shell_status_t)err;
}
#endif /* CONFIG_BT_GATT_NOTIFY_MULTIPLE */

static struct bt_uuid_128 met_svc_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcde01));
static const struct bt_uuid_128 met_char_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcde02));

static uint8_t met_char_value[CHAR_SIZE_MAX] = {
	'M', 'e', 't', 'r', 'i', 'c', 's' };

static ssize_t read_met(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			void *buf, uint16_t len, uint16_t offset)
{
	const char *value = (const char *)attr->user_data;
	uint16_t value_len;

	value_len = MIN(strlen(value), CHAR_SIZE_MAX);

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 value_len);
}

static ssize_t write_met(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 const void *buf, uint16_t len, uint16_t offset,
			 uint8_t flags)
{
	uint8_t *value = (uint8_t *)attr->user_data;

	if (offset + len > sizeof(met_char_value)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	memcpy(value + offset, buf, len);

	update_write_stats(len);

	return len;
}

static struct bt_gatt_attr met_attrs[] = {
	BT_GATT_PRIMARY_SERVICE(&met_svc_uuid),

	BT_GATT_CHARACTERISTIC(&met_char_uuid.uuid,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
			       read_met, write_met, met_char_value),
};

static struct bt_gatt_service met_svc = BT_GATT_SERVICE(met_attrs);

static shell_status_t cmd_metrics(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err = 0;

	if (argc < 2) {
		print_write_stats();
		return kStatus_SHELL_Success;
	}

	if (!strcmp(argv[1], "on")) {
		shell_print(shell, "Registering GATT metrics test Service.");
		err = bt_gatt_service_register(&met_svc);
	} else if (!strcmp(argv[1], "off")) {
		shell_print(shell, "Unregistering GATT metrics test Service.");
		err = bt_gatt_service_unregister(&met_svc);
	} else {
		shell_error(shell, "Incorrect value: %s", argv[1]);
		shell_help(shell);
		return kStatus_SHELL_Error;
	}

	if (!err) {
		shell_print(shell, "GATT write cmd metrics %s.", argv[1]);
	}

	return (shell_status_t)err;
}
#endif /* CONFIG_BT_GATT_DYNAMIC_DB */

static uint8_t get_cb(const struct bt_gatt_attr *attr, uint16_t handle,
		      void *user_data)
{
	struct shell *shell = (struct shell *)user_data;
	uint8_t buf[256];
	ssize_t ret;
	char str[BT_UUID_STR_LEN];

	bt_uuid_to_str(attr->uuid, str, sizeof(str));
	shell_print(shell, "attr %p uuid %s perm 0x%02x", attr, str,
		    attr->perm);

	if (!attr->read) {
		return BT_GATT_ITER_CONTINUE;
	}

	ret = attr->read(NULL, attr, (void *)buf, sizeof(buf), 0);
	if (ret < 0) {
		shell_print(shell, "Failed to read: %zd", ret);
		return BT_GATT_ITER_STOP;
	}

	shell_hexdump(shell, buf, ret);

	return BT_GATT_ITER_CONTINUE;
}

static shell_status_t cmd_get(shell_handle_t shell, int32_t argc, char *argv[])
{
	uint16_t start, end;

	start = strtoul(argv[1], NULL, 16);
	end = start;

	if (argc > 2) {
		end = strtoul(argv[2], NULL, 16);
	}

	bt_gatt_foreach_attr(start, end, get_cb, (void *)shell);

	return kStatus_SHELL_Success;
}

struct set_data {
	shell_handle_t shell;
	int32_t argc;
	char **argv;
	int err;
};

static uint8_t set_cb(const struct bt_gatt_attr *attr, uint16_t handle,
		      void *user_data)
{
	struct set_data *data = (struct set_data *)user_data;
	uint8_t buf[256];
	size_t i;
	ssize_t ret;

	if (!attr->write) {
		shell_error(data->shell, "Write not supported");
		data->err = -ENOENT;
		return BT_GATT_ITER_CONTINUE;
	}

	for (i = 0; i < data->argc; i++) {
		buf[i] = strtoul(data->argv[i], NULL, 16);
	}

	ret = attr->write(NULL, attr, (void *)buf, i, 0, 0);
	if (ret < 0) {
		data->err = ret;
		shell_error(data->shell, "Failed to write: %zd", ret);
		return BT_GATT_ITER_STOP;
	}

	return BT_GATT_ITER_CONTINUE;
}

static shell_status_t cmd_set(shell_handle_t shell, int32_t argc, char *argv[])
{
	uint16_t handle;
	struct set_data data;

	handle = strtoul(argv[1], NULL, 16);

	data.shell = shell;
	data.argc = argc - 2;
	data.argv = argv + 2;
	data.err = 0;

	bt_gatt_foreach_attr(handle, handle, set_cb, &data);

	if (data.err < 0) {
		return kStatus_SHELL_Error;
	}

	bt_gatt_foreach_attr(handle, handle, get_cb, (void *)shell);

	return kStatus_SHELL_Success;
}

shell_status_t cmd_att_mtu(shell_handle_t shell, int32_t argc, char *argv[])
{
	uint16_t mtu;

	if (default_conn) {
		mtu = bt_gatt_get_mtu(default_conn);
		shell_print(shell, "MTU size: %u", mtu);
	} else {
		shell_print(shell, "No default connection");
	}

	return kStatus_SHELL_Success;
}

#define HELP_NONE "[none]"
#define HELP_ADDR_LE "<address: XX:XX:XX:XX:XX:XX> <type: (public|random)>"

SHELL_STATIC_SUBCMD_SET_CREATE(gatt_cmds,
#if (defined(CONFIG_BT_GATT_CLIENT) && (CONFIG_BT_GATT_CLIENT > 0))
	SHELL_CMD_ARG(discover, NULL,
		      "[UUID] [start handle] [end handle]", cmd_discover, 1, 3),
	SHELL_CMD_ARG(discover-characteristic, NULL,
		      "[UUID] [start handle] [end handle]", cmd_discover, 1, 3),
	SHELL_CMD_ARG(discover-descriptor, NULL,
		      "[UUID] [start handle] [end handle]", cmd_discover, 1, 3),
	SHELL_CMD_ARG(discover-include, NULL,
		      "[UUID] [start handle] [end handle]", cmd_discover, 1, 3),
	SHELL_CMD_ARG(discover-primary, NULL,
		      "[UUID] [start handle] [end handle]", cmd_discover, 1, 3),
	SHELL_CMD_ARG(discover-secondary, NULL,
		      "[UUID] [start handle] [end handle]", cmd_discover, 1, 3),
	SHELL_CMD_ARG(exchange-mtu, NULL, HELP_NONE, cmd_exchange_mtu, 1, 0),
	SHELL_CMD_ARG(read, NULL, "<handle> [offset]", cmd_read, 2, 1),
	SHELL_CMD_ARG(read-uuid, NULL, "<UUID> [start handle] [end handle]",
		      cmd_read_uuid, 2, 2),
	SHELL_CMD_ARG(read-multiple, NULL, "<handle 1> <handle 2> ...",
		      cmd_mread, 2, -1),
	SHELL_CMD_ARG(signed-write, NULL, "<handle> <data> [length] [repeat]",
		      cmd_write_without_rsp, 3, 2),
	SHELL_CMD_ARG(subscribe, NULL, "<CCC handle> <value handle> [ind]",
		      cmd_subscribe, 3, 1),
	SHELL_CMD_ARG(resubscribe, NULL, HELP_ADDR_LE" <CCC handle> "
		      "<value handle> [ind]", cmd_resubscribe, 5, 1),
	SHELL_CMD_ARG(write, NULL, "<handle> <offset> <data>", cmd_write, 4, 0),
	SHELL_CMD_ARG(write-without-response, NULL,
		      "<handle> <data> [length] [repeat]",
		      cmd_write_without_rsp, 3, 2),
	SHELL_CMD_ARG(write-without-response-cb, NULL,
		      "<handle> <data> [length] [repeat]",
		      cmd_write_without_rsp, 3, 2),
	SHELL_CMD_ARG(unsubscribe, NULL, HELP_NONE, cmd_unsubscribe, 1, 0),
#endif /* CONFIG_BT_GATT_CLIENT */
	SHELL_CMD_ARG(get, NULL, "<start handle> [end handle]", cmd_get, 2, 1),
	SHELL_CMD_ARG(set, NULL, "<handle> [data...]", cmd_set, 2, 255),
	SHELL_CMD_ARG(show-db, NULL, "[uuid] [num_matches]", cmd_show_db, 1, 2),
	SHELL_CMD_ARG(att_mtu, NULL, "Output ATT MTU size", cmd_att_mtu, 1, 0),
#if (defined(CONFIG_BT_GATT_DYNAMIC_DB) && (CONFIG_BT_GATT_DYNAMIC_DB > 0))
	SHELL_CMD_ARG(metrics, NULL, "[value: on, off]", cmd_metrics, 1, 1),
	SHELL_CMD_ARG(register, NULL,
		      "register pre-predefined test service",
		      cmd_register_test_svc, 1, 0),
	SHELL_CMD_ARG(unregister, NULL,
		      "unregister pre-predefined test service",
		      cmd_unregister_test_svc, 1, 0),
	SHELL_CMD_ARG(notify, NULL, "[data]", cmd_notify, 1, 1),
#if (defined(CONFIG_BT_GATT_NOTIFY_MULTIPLE) && (CONFIG_BT_GATT_NOTIFY_MULTIPLE > 0))
	SHELL_CMD_ARG(notify-mult, NULL, "count [data]", cmd_notify_mult, 2, 1),
#endif /* CONFIG_BT_GATT_NOTIFY_MULTIPLE */
#endif /* CONFIG_BT_GATT_DYNAMIC_DB */
	SHELL_SUBCMD_SET_END
);

static shell_status_t cmd_gatt(shell_handle_t shell, int32_t argc, char **argv)
{
	if (argc == 1) {
		shell_help(shell);
		/* shell returns 1 when help is printed */
		return kStatus_SHELL_PrintCmdHelp;
	}

	shell_error(shell, "%s unknown parameter: %s", argv[0], argv[1]);

	return kStatus_SHELL_Error;
}

SHELL_CMD_REGISTER(gatt, gatt_cmds, "Bluetooth GATT shell commands",
		       cmd_gatt, 1, 1);

void bt_ShellGattInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandgatt))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandgatt.pcCommand);
    }
}
