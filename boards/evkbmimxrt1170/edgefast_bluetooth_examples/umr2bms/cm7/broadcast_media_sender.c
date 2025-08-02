/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/types.h>
#include <stdio.h>
#include <stddef.h>
#include <errno/errno.h>
#include <toolchain.h>
#include <porting.h>
#include "fsl_debug_console.h"

#include <bluetooth/bluetooth.h>
#include <bluetooth/audio/audio.h>
#include <bluetooth/audio/bap.h>
#include <bluetooth/audio/bap_lc3_preset.h>

#include "le_audio_common.h"
#include "le_audio_shell.h"
#include "broadcast_media_sender.h"

/* Note: this include should be remove once audio api could get bt_iso_chan. */
#include "audio/bap_endpoint.h"
#include "audio/bap_iso.h"

/* Audio Source parameters. */
#define MAX_AUDIO_SAMPLE_RATE		48000
#define MAX_AUDIO_CHANNEL_COUNT		2
#define MAX_AUDIO_BYTES_PER_SAMPLE 	4
#define MAX_AUDIO_BUFF_SIZE		(MAX_AUDIO_SAMPLE_RATE / 100 * MAX_AUDIO_BYTES_PER_SAMPLE)

static lc3_codec_info_t lc3_codec_info;

/* When BROADCAST_ENQUEUE_COUNT > 1 we can enqueue enough buffers to ensure that
 * the controller is never idle
 */
#define BROADCAST_ENQUEUE_COUNT 16U
#define TOTAL_BUF_NEEDED (BROADCAST_ENQUEUE_COUNT * CONFIG_BT_BAP_BROADCAST_SRC_STREAM_COUNT)

BUILD_ASSERT_MSG(CONFIG_BT_ISO_TX_BUF_COUNT >= TOTAL_BUF_NEEDED,
	     "CONFIG_BT_ISO_TX_BUF_COUNT should be at least "
	     "BROADCAST_ENQUEUE_COUNT * CONFIG_BT_BAP_BROADCAST_SRC_STREAM_COUNT");

static struct bt_bap_lc3_preset lc3_preset;
static uint8_t iso_packing = BT_ISO_PACKING_SEQUENTIAL;
static uint8_t broadcast_code[BT_AUDIO_BROADCAST_CODE_SIZE] = { 0 };
static bool broadcast_code_set = false;

struct named_lc3_preset {
	const char *name;
	struct bt_bap_lc3_preset preset;
};

#define LOCATION BT_AUDIO_LOCATION_FRONT_LEFT | BT_AUDIO_LOCATION_FRONT_RIGHT
#define CONTEXT BT_AUDIO_CONTEXT_TYPE_MEDIA

static const struct named_lc3_preset lc3_broadcast_presets[] = {
	{"8_1_1", BT_BAP_LC3_BROADCAST_PRESET_8_1_1(LOCATION, CONTEXT)},
	{"8_2_1", BT_BAP_LC3_BROADCAST_PRESET_8_2_1(LOCATION, CONTEXT)},
	{"16_1_1", BT_BAP_LC3_BROADCAST_PRESET_16_1_1(LOCATION, CONTEXT)},
	{"16_2_1", BT_BAP_LC3_BROADCAST_PRESET_16_2_1(LOCATION, CONTEXT)},
	{"24_1_1", BT_BAP_LC3_BROADCAST_PRESET_24_1_1(LOCATION, CONTEXT)},
	{"24_2_1", BT_BAP_LC3_BROADCAST_PRESET_24_2_1(LOCATION, CONTEXT)},
	{"32_1_1", BT_BAP_LC3_BROADCAST_PRESET_32_1_1(LOCATION, CONTEXT)},
	{"32_2_1", BT_BAP_LC3_BROADCAST_PRESET_32_2_1(LOCATION, CONTEXT)},
	{"441_1_1", BT_BAP_LC3_BROADCAST_PRESET_441_1_1(LOCATION, CONTEXT)},
	{"441_2_1", BT_BAP_LC3_BROADCAST_PRESET_441_2_1(LOCATION, CONTEXT)},
	{"48_1_1", BT_BAP_LC3_BROADCAST_PRESET_48_1_1(LOCATION, CONTEXT)},
	{"48_2_1", BT_BAP_LC3_BROADCAST_PRESET_48_2_1(LOCATION, CONTEXT)},
	{"48_3_1", BT_BAP_LC3_BROADCAST_PRESET_48_3_1(LOCATION, CONTEXT)},
	{"48_4_1", BT_BAP_LC3_BROADCAST_PRESET_48_4_1(LOCATION, CONTEXT)},
	{"48_5_1", BT_BAP_LC3_BROADCAST_PRESET_48_5_1(LOCATION, CONTEXT)},
	{"48_6_1", BT_BAP_LC3_BROADCAST_PRESET_48_6_1(LOCATION, CONTEXT)},
	/* High-reliability presets */
	{"8_1_2", BT_BAP_LC3_BROADCAST_PRESET_8_1_2(LOCATION, CONTEXT)},
	{"8_2_2", BT_BAP_LC3_BROADCAST_PRESET_8_2_2(LOCATION, CONTEXT)},
	{"16_1_2", BT_BAP_LC3_BROADCAST_PRESET_16_1_2(LOCATION, CONTEXT)},
	{"16_2_2", BT_BAP_LC3_BROADCAST_PRESET_16_2_2(LOCATION, CONTEXT)},
	{"24_1_2", BT_BAP_LC3_BROADCAST_PRESET_24_1_2(LOCATION, CONTEXT)},
	{"24_2_2", BT_BAP_LC3_BROADCAST_PRESET_24_2_2(LOCATION, CONTEXT)},
	{"32_1_2", BT_BAP_LC3_BROADCAST_PRESET_32_1_2(LOCATION, CONTEXT)},
	{"32_2_2", BT_BAP_LC3_BROADCAST_PRESET_32_2_2(LOCATION, CONTEXT)},
	{"441_1_2", BT_BAP_LC3_BROADCAST_PRESET_441_1_2(LOCATION, CONTEXT)},
	{"441_2_2", BT_BAP_LC3_BROADCAST_PRESET_441_2_2(LOCATION, CONTEXT)},
	{"48_1_2", BT_BAP_LC3_BROADCAST_PRESET_48_1_2(LOCATION, CONTEXT)},
	{"48_2_2", BT_BAP_LC3_BROADCAST_PRESET_48_2_2(LOCATION, CONTEXT)},
	{"48_3_2", BT_BAP_LC3_BROADCAST_PRESET_48_3_2(LOCATION, CONTEXT)},
	{"48_4_2", BT_BAP_LC3_BROADCAST_PRESET_48_4_2(LOCATION, CONTEXT)},
	{"48_5_2", BT_BAP_LC3_BROADCAST_PRESET_48_5_2(LOCATION, CONTEXT)},
	{"48_6_2", BT_BAP_LC3_BROADCAST_PRESET_48_6_2(LOCATION, CONTEXT)},
};

static struct bt_audio_codec_cfg bis_codec_specific_config[CONFIG_BT_BAP_BROADCAST_SRC_STREAM_COUNT][1];

enum {
	BMS_START,
	/* Total number of flags - must be at the end of the enum */
	BMS_NUM_FLAGS,
};

static struct broadcast_source_stream {
	struct bt_bap_stream stream;
	enum bt_audio_location loc;
	uint16_t seq_num;
	size_t sent_cnt;
	struct k_sem sem;
	ATOMIC_DEFINE(flags, BMS_NUM_FLAGS);
} streams[CONFIG_BT_BAP_BROADCAST_SRC_STREAM_COUNT];
static struct bt_bap_broadcast_source *broadcast_source;

NET_BUF_POOL_FIXED_DEFINE(tx_pool,
			  TOTAL_BUF_NEEDED,
			  BT_ISO_SDU_BUF_SIZE(CONFIG_BT_ISO_TX_MTU), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
static uint32_t seq_num;
static uint64_t tx_samples = 0;
static uint32_t tx_time_stamp_start;

extern void BOARD_SyncSignal_Start(uint32_t init_offset);
extern void BOARD_SyncSignal_Stop(void);
extern uint32_t BOARD_SyncSignal_Count(void);

static void stream_started_cb(struct bt_bap_stream *stream)
{
	struct broadcast_source_stream *source_stream =
		CONTAINER_OF(stream, struct broadcast_source_stream, stream);

	if(stream == &streams[0].stream)
	{
		seq_num = 0;
		BOARD_SyncSignal_Start(0);
	}

	source_stream->seq_num = 0U;
	source_stream->sent_cnt = 0U;
	atomic_set_bit(source_stream->flags, BMS_START);
	k_sem_give(&source_stream->sem);
}

static void stream_stopped_cb(struct bt_bap_stream *stream, uint8_t reason)
{
	struct broadcast_source_stream *source_stream =
		CONTAINER_OF(stream, struct broadcast_source_stream, stream);

	BOARD_SyncSignal_Stop();

	atomic_clear_bit(source_stream->flags, BMS_START);
	k_sem_give(&source_stream->sem);
}

struct bt_bap_stream_ops stream_ops = {
	.started = stream_started_cb,
	.stopped = stream_stopped_cb,
};

static int setup_broadcast_source(struct bt_bap_broadcast_source **source)
{
	struct bt_bap_broadcast_source_stream_param
		stream_params[CONFIG_BT_BAP_BROADCAST_SRC_STREAM_COUNT];
	struct bt_bap_broadcast_source_subgroup_param
		subgroup_param[CONFIG_BT_BAP_BROADCAST_SRC_SUBGROUP_COUNT];
	struct bt_bap_broadcast_source_param create_param;
	const size_t streams_per_subgroup = ARRAY_SIZE(stream_params) / ARRAY_SIZE(subgroup_param);
	int err;

	(void)memset(streams, 0, sizeof(streams));

	for (size_t i = 0U; i < ARRAY_SIZE(streams); i++) {
		k_sem_init(&streams[i].sem, 0, 0xFF);
	}

	for (size_t i = 0U; i < ARRAY_SIZE(subgroup_param); i++) {
		subgroup_param[i].params_count = streams_per_subgroup;
		subgroup_param[i].params = stream_params + i * streams_per_subgroup;
		subgroup_param[i].codec_cfg = &lc3_preset.codec_cfg;
	}

	for (size_t j = 0U; j < ARRAY_SIZE(stream_params); j++) {
		stream_params[j].stream = &streams[j].stream;

		stream_params[j].data = bis_codec_specific_config[j][0].data;
		stream_params[j].data_len = bis_codec_specific_config[j][0].data_len;

		bt_audio_codec_cfg_get_chan_allocation(&bis_codec_specific_config[j][0], &streams[j].loc, true);

		bt_bap_stream_cb_register(stream_params[j].stream, &stream_ops);
	}

	create_param.params_count = ARRAY_SIZE(subgroup_param);
	create_param.params = subgroup_param;
	create_param.qos = &lc3_preset.qos;
	create_param.encryption = broadcast_code_set;
	create_param.packing = iso_packing;

	if(broadcast_code_set)
	{
		memcpy(create_param.broadcast_code, broadcast_code, BT_AUDIO_BROADCAST_CODE_SIZE);
	}

	printk("BMS: Creating broadcast source with %zu subgroups with %zu streams\r\n",
	       ARRAY_SIZE(subgroup_param),
	       ARRAY_SIZE(subgroup_param) * streams_per_subgroup);

	err = bt_bap_broadcast_source_create(&create_param, source);
	if (err != 0) {
		printk("BMS: Unable to create broadcast source: %d\r\n", err);
		return err;
	}

	return 0;
}

static void print_lc3_preset(const char *name, const struct bt_bap_lc3_preset *preset)
{
	const struct bt_audio_codec_cfg *codec_cfg = &preset->codec_cfg;
	const struct bt_audio_codec_qos *qos = &preset->qos;

	printk("BMS: %s:\r\n", name);

	printk("BMS: \tcodec_cfg - sample_rate: %d, duration: %d, len: %d\r\n",
			bt_audio_codec_cfg_freq_to_freq_hz((enum bt_audio_codec_cfg_freq)bt_audio_codec_cfg_get_freq(codec_cfg)),
			bt_audio_codec_cfg_frame_dur_to_frame_dur_us((enum bt_audio_codec_cfg_frame_dur)bt_audio_codec_cfg_get_frame_dur(codec_cfg)),
			bt_audio_codec_cfg_get_octets_per_frame(codec_cfg)
		);

	printk("BMS: \tqos - interval: %d, framing: %d, phy: %d, sdu: %d, rtn: %d, pd: %d\r\n",
			qos->interval,
			qos->framing,
			qos->phy,
			qos->sdu,
			qos->rtn,
			qos->pd
		);
}

void print_all_preset(int sample_rate)
{
	PRINTF("\nlc3 preset list:\n");
	for(int i = 0; i < ARRAY_SIZE(lc3_broadcast_presets); i++)
	{
		if(sample_rate != 0)
		{
			const struct bt_audio_codec_cfg *codec_cfg = &lc3_broadcast_presets[i].preset.codec_cfg;
			int codec_sample_rate = bt_audio_codec_cfg_freq_to_freq_hz((enum bt_audio_codec_cfg_freq)bt_audio_codec_cfg_get_freq(codec_cfg));
			if(codec_sample_rate != sample_rate)
			{
				continue;
			}
		}
		print_lc3_preset(lc3_broadcast_presets[i].name, &lc3_broadcast_presets[i].preset);
	}
}

int select_lc3_preset(char *preset_name)
{
	bool find = false;

	for(int i = 0; i < ARRAY_SIZE(lc3_broadcast_presets); i++)
	{
		if(0 == strcmp(lc3_broadcast_presets[i].name, preset_name))
		{
			find = true;
			memcpy(&lc3_preset, &lc3_broadcast_presets[i].preset, sizeof(lc3_preset));
			break;
		}
	}

	if(!find)
	{
		return -1;
	}

	bt_audio_codec_cfg_unset_val(&lc3_preset.codec_cfg, BT_AUDIO_CODEC_CFG_CHAN_ALLOC);
	bt_audio_codec_cfg_set_chan_allocation(&bis_codec_specific_config[0][0], BT_AUDIO_LOCATION_FRONT_LEFT);
	bt_audio_codec_cfg_set_chan_allocation(&bis_codec_specific_config[1][0], BT_AUDIO_LOCATION_FRONT_RIGHT);
	lc3_preset.qos.latency = 95;
	lc3_preset.qos.rtn = 13;
	print_lc3_preset(preset_name, &lc3_preset);
	return 0;
}

/* Here we don't require the user input all the bytes, and the left bytes will fill with 0. */
int config_broadcast_code(uint8_t *data, int len)
{
	memset(broadcast_code, 0, BT_AUDIO_BROADCAST_CODE_SIZE);
	if(len <= BT_AUDIO_BROADCAST_CODE_SIZE)
	{
		memcpy(broadcast_code, data, len);
		broadcast_code_set = true;

		printk("BMS: broadcast_code: %s\r\n", bt_hex(broadcast_code, BT_AUDIO_BROADCAST_CODE_SIZE));
	}
	else
	{
		return -1;
	}

	return 0;
}

int bms_codec_cfg(struct bt_audio_codec_cfg *cfg)
{
	lc3_preset.codec_cfg = *cfg;
	bt_audio_codec_cfg_unset_val(&lc3_preset.codec_cfg, BT_AUDIO_CODEC_CFG_CHAN_ALLOC);
	bt_audio_codec_cfg_set_chan_allocation(&bis_codec_specific_config[0][0], BT_AUDIO_LOCATION_FRONT_LEFT);
	bt_audio_codec_cfg_set_chan_allocation(&bis_codec_specific_config[1][0], BT_AUDIO_LOCATION_FRONT_RIGHT);

	return 0;
}

int bms_qos(struct bt_audio_codec_qos *qos)
{
	lc3_preset.qos = *qos;

	lc3_preset.qos.sdu = bt_audio_codec_cfg_get_octets_per_frame(&lc3_preset.codec_cfg);

	return 0;
}

static volatile bool initiated;
static struct bt_le_ext_adv *adv;

static void config_parameter(void)
{
	/* set the LC3 encoder parameters. */
	lc3_codec_info.sample_rate = bt_audio_codec_cfg_freq_to_freq_hz((enum bt_audio_codec_cfg_freq)bt_audio_codec_cfg_get_freq(&lc3_preset.codec_cfg));
	lc3_codec_info.frame_duration_us = bt_audio_codec_cfg_frame_dur_to_frame_dur_us((enum bt_audio_codec_cfg_frame_dur)bt_audio_codec_cfg_get_frame_dur(&lc3_preset.codec_cfg));
	lc3_codec_info.octets_per_frame = bt_audio_codec_cfg_get_octets_per_frame(&lc3_preset.codec_cfg);
	lc3_codec_info.blocks_per_sdu = 1;
	lc3_codec_info.chan_allocation = BT_AUDIO_LOCATION_FRONT_LEFT | BT_AUDIO_LOCATION_FRONT_RIGHT;

	lc3_codec_info.channels = 2;
	if(lc3_codec_info.sample_rate == 44100)
	{
		if(lc3_codec_info.frame_duration_us == 7500)
		{
			lc3_codec_info.samples_per_frame = 360;
		}
		else
		{
			lc3_codec_info.samples_per_frame = 480;
		}
	}
	else
	{
		lc3_codec_info.samples_per_frame = lc3_codec_info.sample_rate * (lc3_codec_info.frame_duration_us / 100) / 10000;
	}
	lc3_codec_info.bytes_per_channel_frame = lc3_codec_info.samples_per_frame * 16 / 8;
}

int bms_init(void)
{
	int err;

	if (initiated) {
		return 0;
	}

	if ((lc3_preset.codec_cfg.data_len == 0) || (lc3_preset.qos.pd == 0)) {
		PRINTF("Invalid lc3_preset");
		return -EINVAL;
	}

	config_parameter();

	/* Broadcast Audio Streaming Endpoint advertising data */
	NET_BUF_SIMPLE_DEFINE(ad_buf,
				BT_UUID_SIZE_16 + BT_AUDIO_BROADCAST_ID_SIZE);
	NET_BUF_SIMPLE_DEFINE(base_buf, 128);
	struct bt_data ext_ad[2];
	struct bt_data per_ad;
	uint32_t broadcast_id;

	/* Create a non-connectable non-scannable advertising set */
	err = bt_le_ext_adv_create(BT_LE_EXT_ADV_NCONN, NULL, &adv);
	if (err != 0) {
		printk("BMS: Unable to create extended advertising set: %d\r\n",
			err);
		return err;
	}

	/* Set periodic advertising parameters */
	err = bt_le_per_adv_set_param(adv, BT_LE_PER_ADV_DEFAULT);
	if (err) {
		printk("BMS: Failed to set periodic advertising parameters"
		" (err %d)\r\n", err);
		return err;
	}

	printk("BMS: Creating broadcast source\r\n");
	err = setup_broadcast_source(&broadcast_source);
	if (err != 0) {
		printk("BMS: Unable to setup broadcast source: %d\r\n", err);
		return err;
	}

	err = bt_bap_broadcast_source_get_id(broadcast_source, &broadcast_id);
	if (err != 0) {
		printk("BMS: Unable to get broadcast ID: %d\r\n", err);
		return err;
	}

	/* Setup extended advertising data */
	net_buf_simple_add_le16(&ad_buf, BT_UUID_BROADCAST_AUDIO_VAL);
	net_buf_simple_add_le24(&ad_buf, broadcast_id);
	ext_ad[0].type = BT_DATA_SVC_DATA16;
	ext_ad[0].data_len = ad_buf.len;
	ext_ad[0].data = ad_buf.data;
	ext_ad[1] = (struct bt_data)BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME,
						sizeof(CONFIG_BT_DEVICE_NAME) - 1);
	err = bt_le_ext_adv_set_data(adv, ext_ad, ARRAY_SIZE(ext_ad), NULL, 0);
	if (err != 0) {
		printk("BMS: Failed to set extended advertising data: %d\r\n",
			err);
		return err;
	}

	/* Setup periodic advertising data */
	err = bt_bap_broadcast_source_get_base(broadcast_source, &base_buf);
	if (err != 0) {
		printk("BMS: Failed to get encoded BASE: %d\r\n", err);
		return err;
	}

	per_ad.type = BT_DATA_SVC_DATA16;
	per_ad.data_len = base_buf.len;
	per_ad.data = base_buf.data;
	err = bt_le_per_adv_set_data(adv, &per_ad, 1);
	if (err != 0) {
		printk("BMS: Failed to set periodic advertising data: %d\r\n",
			err);
		return err;
	}

	/* Start extended advertising */
	err = bt_le_ext_adv_start(adv, BT_LE_EXT_ADV_START_DEFAULT);
	if (err) {
		printk("BMS: Failed to start extended advertising: %d\r\n",
			err);
		return err;
	}

	/* Enable Periodic Advertising */
	err = bt_le_per_adv_start(adv);
	if (err) {
		printk("BMS: Failed to enable periodic advertising: %d\r\n",
			err);
		return err;
	}

	initiated = true;
	return 0;
}

static volatile bool started;

int bms_start(void)
{
	int err;

	if (started) {
		return 0;
	}

	printk("BMS: Starting broadcast source\r\n");
	err = bt_bap_broadcast_source_start(broadcast_source, adv);
	if (err != 0) {
		printk("BMS: Unable to start broadcast source: %d\r\n", err);
		return err;
	}

	printk("BMS: Waiting stream start event\r\n");
	/* Wait for all to be started */
	for (size_t i = 0U; i < ARRAY_SIZE(streams); i++) {
		k_sem_take(&streams[i].sem, K_FOREVER);
		if (!atomic_test_bit(streams[i].flags, BMS_START)) {
			return -EINVAL;
		}
	}
	printk("BMS: Broadcast source started\r\n");

	started = true;
	return 0;
}

int bms_stop(void)
{
	int err;

	if (!started) {
		return -EINVAL;
	}

	printk("BMS: Stopping broadcast source\r\n");
	err = bt_bap_broadcast_source_stop(broadcast_source);
	if (err != 0) {
		printk("BMS: Unable to stop broadcast source: %d\r\n", err);
		return err;
	}

	/* Wait for all to be stopped */
	for (size_t i = 0U; i < ARRAY_SIZE(streams); i++) {
		k_sem_take(&streams[i].sem, K_FOREVER);
		if (atomic_test_bit(streams[i].flags, BMS_START)) {
			return -EINVAL;
		}
	}
	printk("BMS: Broadcast source stopped\r\n");
	started = false;

	return 0;
}

int bms_deinit(void)
{
	int err;

	if (!initiated) {
		return -EINVAL;
	}

	printk("BMS: Deleting broadcast source\r\n");
	err = bt_bap_broadcast_source_delete(broadcast_source);
	if (err != 0) {
		printk("BMS: Unable to delete broadcast source: %d\r\n", err);
		return err;
	}
	printk("BMS: Broadcast source deleted\r\n");
	broadcast_source = NULL;
	seq_num = 0;

	err = bt_le_per_adv_stop(adv);
	if (err) {
		printk("BMS: Failed to stop periodic advertising (err %d)\r\n",
			err);
		return err;
	}

	err = bt_le_ext_adv_stop(adv);
	if (err) {
		printk("BMS: Failed to stop extended advertising (err %d)\r\n",
			err);
		return err;
	}

	err = bt_le_ext_adv_delete(adv);
	if (err) {
		printk("BMS: Failed to delete extended advertising (err %d)\r\n",
			err);
		return err;
	}

	initiated = false;
	return 0;
}

static uint32_t get_big_sync_delay(void)
{
	struct bt_iso_info iso_info;
	uint32_t BIG_Sync_Delay_us;

	bt_iso_chan_get_info(&streams[0].stream.ep->iso->chan, &iso_info);

	BIG_Sync_Delay_us = iso_info.broadcaster.sync_delay;

	return BIG_Sync_Delay_us;
}

static uint32_t get_iso_interval(void)
{
	struct bt_iso_info iso_info;
	uint32_t ISO_Interval_us;

	bt_iso_chan_get_info(&streams[0].stream.ep->iso->chan, &iso_info);

	ISO_Interval_us = iso_info.iso_interval * 1250;

	return ISO_Interval_us;
}

static uint32_t get_sync_signal_timestamp(void)
{
	uint32_t time_stamp;

	time_stamp = BOARD_SyncSignal_Count() * get_iso_interval() + get_big_sync_delay();

	return time_stamp;
}

int bms_stream_send(enum bt_audio_location chan_allocation, struct net_buf *buf)
{
	uint32_t sdu_time_stamp;
	struct broadcast_source_stream *stream = NULL;
	struct net_buf *tx_buf;
	int err;
	uint32_t loc = (uint32_t)chan_allocation;

	if (!started) {
		return -EINVAL;
	}

	if(seq_num == 0)
	{
		tx_samples = 0;
		tx_time_stamp_start = get_sync_signal_timestamp() + get_iso_interval();
		sdu_time_stamp = tx_time_stamp_start;
	}
	else
	{
		tx_samples += lc3_codec_info.samples_per_frame;
		sdu_time_stamp = (uint32_t)((double)tx_time_stamp_start + (double)tx_samples * 1000000.0 / (double)lc3_codec_info.sample_rate);
	}

	for (int index = 0; index < (sizeof(loc) *8); index++) {
		if (loc == 0) {
			break;
		}

		if (loc & BIT(index)) {
			loc &= ~BIT(index);
		} else {
			continue;
		}

		for (size_t i = 0U; i < ARRAY_SIZE(streams); i++) {
			if (streams[i].loc == (enum bt_audio_location)BIT(index)) {
				stream = &streams[i];
				break;
			}
		}

		if (!stream) {
			continue;
		}

		if (!atomic_test_bit(stream->flags, BMS_START)) {
			continue;
		}

		tx_buf = net_buf_alloc(&tx_pool, 0);
		if (tx_buf != NULL) {
			size_t len;

			len = lc3_codec_info.octets_per_frame;

			net_buf_reserve(tx_buf, BT_ISO_CHAN_SEND_RESERVE);
			if ((buf != NULL) && (buf->len >= len)) {
				net_buf_add_mem(tx_buf, buf->data, len);
				net_buf_pull(buf, len);
			} else {
				uint8_t *data;

				data = net_buf_add(tx_buf, len);
				memset(data, 0, len);
			}

			err = bt_bap_stream_send_ts(&stream->stream, tx_buf, (uint16_t)(seq_num & 0xffff), sdu_time_stamp);
			if (err < 0) {
				printk("BMS: Unable to broadcast data on %p: %d\r\n", stream, err);
				net_buf_unref(tx_buf);
			}
		}
	}

	seq_num ++;

	return 0;
}

volatile bool streaming_simulate_start = false;

void streaming_simulate_work_handler(struct k_work *work)
{
	struct net_buf *buf;
	struct k_work_delayable *dwork = (struct k_work_delayable *)CONTAINER_OF(work, struct k_work_delayable, work);
	static uint8_t index = 0;

	if (streaming_simulate_start) {
		k_work_reschedule(dwork, K_USEC(lc3_codec_info.frame_duration_us));
	}

	buf = net_buf_alloc(&tx_pool, 0);
	if (buf != NULL) {
		memset(buf->data, index, lc3_codec_info.octets_per_frame);
		net_buf_add(buf, lc3_codec_info.octets_per_frame);
		index ++;
		memset(&buf->data[buf->len], index, lc3_codec_info.octets_per_frame);
		net_buf_add(buf, lc3_codec_info.octets_per_frame);

		bms_stream_send((enum bt_audio_location)(BT_AUDIO_LOCATION_FRONT_LEFT | BT_AUDIO_LOCATION_FRONT_RIGHT), buf);

		net_buf_unref(buf);
	}
	index++;
}

struct k_work_delayable streaming_simulate =
{
	.work = {
		.handler = streaming_simulate_work_handler,
		.flags = K_WORK_DELAYABLE,
	},
};

int bms_streaming_simulate_start(void)
{
	if (!streaming_simulate_start) {
		streaming_simulate_start = true;
		k_work_schedule(&streaming_simulate, K_USEC(lc3_codec_info.frame_duration_us));
		return 0;
	}
	PRINTF("BMS: Streaming is started");
	return -EINVAL;
}

int bms_streaming_simulate_stop(void)
{
	if (streaming_simulate_start) {
		streaming_simulate_start = false;
		k_work_cancel_delayable(&streaming_simulate);
		return 0;
	}
	PRINTF("BMS: Streaming is not started");
	return -EINVAL;
}
