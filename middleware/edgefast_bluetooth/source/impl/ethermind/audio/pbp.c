/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
#if defined(CONFIG_BT_PBP) && (CONFIG_BT_PBP > 0)

#include <stddef.h>
#include <stdint.h>

#include <porting.h>
#include <bluetooth/audio/audio.h>
#include <bluetooth/audio/pbp.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/gap.h>
#include <bluetooth/uuid.h>
#include <net/buf.h>
#include <sys/byteorder.h>
#include <sys/check.h>

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_PBP)
#define LOG_MODULE_NAME bt_pbp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

int bt_pbp_get_announcement(const uint8_t meta[], size_t meta_len,
			    enum bt_pbp_announcement_feature features,
			    struct net_buf_simple *pba_data_buf)
{
	CHECKIF(pba_data_buf == NULL) {
		LOG_DBG("No buffer provided for advertising data!\n");

		return -EINVAL;
	}

	CHECKIF((meta == NULL && meta_len != 0) || (meta != NULL && meta_len == 0)) {
		LOG_DBG("Invalid metadata combination: %p %zu", meta, meta_len);

		return -EINVAL;
	}

	CHECKIF(pba_data_buf->size < (meta_len + BT_PBP_MIN_PBA_SIZE)) {
		LOG_DBG("Buffer size needs to be at least %d!\n", meta_len + BT_PBP_MIN_PBA_SIZE);

		return -EINVAL;
	}

	/* Fill Announcement data */
	net_buf_simple_add_le16(pba_data_buf, BT_UUID_PBA_VAL);
	net_buf_simple_add_u8(pba_data_buf, features);
	net_buf_simple_add_u8(pba_data_buf, meta_len);
	net_buf_simple_add_mem(pba_data_buf, meta, meta_len);

	return 0;
}

int bt_pbp_parse_announcement(struct bt_data *data, enum bt_pbp_announcement_feature *features,
			      uint8_t **meta)
{
	struct bt_uuid_16 adv_uuid;
	struct net_buf_simple buf;
	uint8_t meta_len = 0;
	void *uuid;

	CHECKIF(!data || !features || !meta) {
		return -EINVAL;
	}

	if (data->type != BT_DATA_SVC_DATA16) {
		return -ENOENT;
	}

	if (data->data_len < BT_PBP_MIN_PBA_SIZE) {
		return -EMSGSIZE;
	}

	net_buf_simple_init_with_data(&buf, (void *)data->data, data->data_len);
	uuid = net_buf_simple_pull_mem(&buf, BT_UUID_SIZE_16);

	(void)bt_uuid_create(&adv_uuid.uuid, uuid, BT_UUID_SIZE_16); /* cannot fail */

	if (bt_uuid_cmp(&adv_uuid.uuid, BT_UUID_PBA)) {
		return -ENOENT;
	}

	/* Copy source features, metadata length and metadata from the Announcement */
	*features = (enum bt_pbp_announcement_feature)net_buf_simple_pull_u8(&buf);
	meta_len = net_buf_simple_pull_u8(&buf);
	if (buf.len < meta_len) {
		return -EBADMSG;
	}

	*meta = (uint8_t *)net_buf_simple_pull_mem(&buf, meta_len);

	return meta_len;
}

#endif /* CONFIG_BT_PBP */
