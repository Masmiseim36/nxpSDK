/* keys_br.c - Bluetooth BR/EDR key handling */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>
#include <string.h>
#include <sys/atomic.h>
#include <sys/util.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/hci.h>

#include "BT_sm_api.h"

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_KEYS)
#define LOG_MODULE_NAME bt_keys_br
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#include "bt_pal_hci_core.h"
#include "bt_pal_settings.h"
#include "bt_pal_keys.h"

static struct bt_keys_link_key key_pool[CONFIG_BT_MAX_PAIRED];
#if (defined(CONFIG_BT_SETTINGS) && ((CONFIG_BT_SETTINGS) > 0U))
#if IS_ENABLED(CONFIG_BT_KEYS_OVERWRITE_OLDEST)
static uint32_t aging_counter_val;
static struct bt_keys_link_key *last_keys_updated;
#endif /* CONFIG_BT_KEYS_OVERWRITE_OLDEST */
#endif /* CONFIG_BT_SETTINGS */
struct bt_keys_link_key *bt_keys_find_link_key(const bt_addr_t *addr)
{
	struct bt_keys_link_key *key;
	int i;

	LOG_DBG("%s", bt_addr_str(addr));

	for (i = 0; i < ARRAY_SIZE(key_pool); i++) {
		key = &key_pool[i];

		if (bt_addr_eq(&key->addr, addr)) {
			return key;
		}
	}

	return NULL;
}

struct bt_keys_link_key *bt_keys_get_link_key(const bt_addr_t *addr)
{
	struct bt_keys_link_key *key;

	key = bt_keys_find_link_key(addr);
	if (key) {
		return key;
	}

	key = bt_keys_find_link_key(BT_ADDR_ANY);
#if IS_ENABLED(CONFIG_BT_KEYS_OVERWRITE_OLDEST)
	if (!key) {
		int i;

		key = &key_pool[0];
		for (i = 1; i < ARRAY_SIZE(key_pool); i++) {
			struct bt_keys_link_key *current = &key_pool[i];

			if (current->aging_counter < key->aging_counter) {
				key = current;
			}
		}

		if (key) {
			bt_keys_link_key_clear(key);
		}
	}
#endif

	if (key) {
		bt_addr_copy(&key->addr, addr);
#if (defined(CONFIG_BT_SETTINGS) && ((CONFIG_BT_SETTINGS) > 0U))
#if IS_ENABLED(CONFIG_BT_KEYS_OVERWRITE_OLDEST)
		key->aging_counter = ++aging_counter_val;
		last_keys_updated = key;
#endif
#endif
		LOG_DBG("created %p for %s", key, bt_addr_str(addr));
		return key;
	}

	LOG_DBG("unable to create keys for %s", bt_addr_str(addr));

	return NULL;
}

void bt_keys_link_key_clear(struct bt_keys_link_key *link_key)
{
	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_addr_le_t le_addr;

		le_addr.type = BT_ADDR_LE_PUBLIC;
		bt_addr_copy(&le_addr.a, &link_key->addr);

		bt_settings_delete_link_key(&le_addr);
	}

	LOG_DBG("%s", bt_addr_str(&link_key->addr));
	(void)memset(link_key, 0, sizeof(*link_key));
}

void bt_keys_link_key_clear_addr(const bt_addr_t *addr)
{

	API_RESULT retval;
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
	int i;
	struct bt_keys_link_key *key;


    memcpy(bd_addr, addr->val, sizeof(bd_addr));
#if IS_ENABLED(CONFIG_BT_CLASSIC)
	retval = BT_sm_delete_device (bd_addr, SM_ANY_LIST);
#endif  /* CONFIG_BT_CLASSIC */
	(void)retval;

	if (!addr) {
		for (i = 0; i < ARRAY_SIZE(key_pool); i++) {
			key = &key_pool[i];
			bt_keys_link_key_clear(key);
		}
		return;
	}

	key = bt_keys_find_link_key(addr);
	if (key) {
		bt_keys_link_key_clear(key);
	}
}

void bt_keys_link_key_store(struct bt_keys_link_key *link_key)
{
	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		int err;
		bt_addr_le_t le_addr;

		le_addr.type = BT_ADDR_LE_PUBLIC;
		bt_addr_copy(&le_addr.a, &link_key->addr);

		err = bt_settings_store_link_key(&le_addr, link_key->storage_start,
						 BT_KEYS_LINK_KEY_STORAGE_LEN);
		if (err) {
			LOG_ERR("Failed to save link key (err %d)", err);
		}
	}
}

void bt_foreach_bond_br(void (*func)(const struct bt_bond_info *info, void *user_data),
			void *user_data)
{
	__ASSERT_NO_MSG(func != NULL);

	for (size_t i = 0; i < ARRAY_SIZE(key_pool); i++) {
		const struct bt_keys_link_key *key = &key_pool[i];

		if (!bt_addr_eq(&key->addr, BT_ADDR_ANY)) {
			struct bt_bond_info info;

			info.addr.type = BT_ADDR_LE_PUBLIC;
			bt_addr_copy(&info.addr.a, &key->addr);
			func(&info, user_data);
		}
	}
}

#if (defined(CONFIG_BT_SETTINGS) && ((CONFIG_BT_SETTINGS) > 0U))

static int link_key_set(const char *name, size_t len_rd,
			settings_read_cb read_cb, void *cb_arg)
{
	int err;
	ssize_t len;
	bt_addr_le_t le_addr;
	struct bt_keys_link_key *link_key;
	char val[BT_KEYS_LINK_KEY_STORAGE_LEN];

	if (!name) {
		LOG_ERR("Insufficient number of arguments");
		return -EINVAL;
	}

	len = read_cb(cb_arg, val, sizeof(val));
	if (len < 0) {
		LOG_ERR("Failed to read value (err %zd)", len);
		return -EINVAL;
	}

	LOG_DBG("name %s val %s", name,
	       len ? bt_hex(val, sizeof(val)) : "(null)");

	err = bt_settings_decode_key(name, &le_addr);
	if (err) {
		LOG_ERR("Unable to decode address %s", name);
		return -EINVAL;
	}

	link_key = bt_keys_get_link_key(&le_addr.a);
	if (len != BT_KEYS_LINK_KEY_STORAGE_LEN) {
		if (link_key) {
			bt_keys_link_key_clear(link_key);
			LOG_DBG("Clear keys for %s", bt_addr_le_str(&le_addr));
		} else {
			LOG_WRN("Unable to find deleted keys for %s",
				bt_addr_le_str(&le_addr));
		}

		return 0;
	}

	memcpy(link_key->storage_start, val, len);
	LOG_DBG("Successfully restored link key for %s",
	       bt_addr_le_str(&le_addr));
#if IS_ENABLED(CONFIG_BT_KEYS_OVERWRITE_OLDEST)
	if (aging_counter_val < link_key->aging_counter) {
		aging_counter_val = link_key->aging_counter;
	}
#endif  /* CONFIG_BT_KEYS_OVERWRITE_OLDEST */

	return 0;
}

static int link_key_commit(void)
{
	return 0;
}

SETTINGS_STATIC_HANDLER_DEFINE(bt_link_key, "bt/link_key", NULL, link_key_set,
			       link_key_commit, NULL);

#if IS_ENABLED(CONFIG_BT_KEYS_OVERWRITE_OLDEST)
void bt_keys_link_key_update_usage(const bt_addr_t *addr)
{
	struct bt_keys_link_key *link_key = bt_keys_find_link_key(addr);

	if (!link_key) {
		return;
	}

	if (last_keys_updated == link_key) {
		return;
	}

	link_key->aging_counter = ++aging_counter_val;
	last_keys_updated = link_key;

	LOG_DBG("Aging counter for %s is set to %u", bt_addr_str(addr),
	       link_key->aging_counter);

	if (IS_ENABLED(CONFIG_BT_KEYS_SAVE_AGING_COUNTER_ON_PAIRING)) {
		bt_keys_link_key_store(link_key);
	}
}
#endif  /* CONFIG_BT_KEYS_OVERWRITE_OLDEST */

#endif /* defined(CONFIG_BT_SETTINGS) */
