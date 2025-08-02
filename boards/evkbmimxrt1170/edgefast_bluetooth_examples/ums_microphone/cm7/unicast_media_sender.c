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
#include <bluetooth/conn.h>
#include <bluetooth/audio/audio.h>
#include <bluetooth/audio/bap.h>
#include <bluetooth/audio/bap_lc3_preset.h>
#include <bluetooth/audio/csip.h>
#include <sys/byteorder.h>

#include "le_audio_common.h"
#include "le_audio_shell.h"
#include "le_audio_service.h"

/* Note: this include should be remove once audio api could get bt_iso_chan. */
#include "audio/bap_endpoint.h"
#include "audio/bap_iso.h"

typedef void (*codec_rx_callback_t)(uint8_t *rx_buffer);

extern void BOARD_SyncSignal_Start(uint32_t init_offset);
extern void BOARD_SyncSignal_Stop(void);
extern uint32_t BOARD_SyncSignal_Count(void);

extern int BOARD_StartCodec(codec_rx_callback_t rx_callback, uint32_t simpleBitRate, uint32_t simpleBits, uint32_t duration_us);
extern int BOARD_StopCodec(void);
extern void BOARD_StartStream(void);

static void codec_rx_callback(uint8_t *rx_buffer);

/* Audio Source parameters. */
#define MAX_AUDIO_SAMPLE_RATE		384000
#define MAX_AUDIO_CHANNEL_COUNT		2
#define MAX_AUDIO_BYTES_PER_SAMPLE 	2
#define BITS_RATES_OF_SAMPLE            16U

/* LC3 encoder variables. */
#include "lc3_codec.h"
lc3_encoder_t encoder;

static uint8_t sdu_buff[1][LC3_FRAME_SIZE_MAX];

static lc3_codec_info_t lc3_codec_info;

/* When BROADCAST_ENQUEUE_COUNT > 1 we can enqueue enough buffers to ensure that
 * the controller is never idle
 */
#define UNICAST_ENQUEUE_COUNT 8
#define TOTAL_BUF_NEEDED (UNICAST_ENQUEUE_COUNT * CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK_COUNT)

BUILD_ASSERT_MSG(CONFIG_BT_ISO_TX_BUF_COUNT >= TOTAL_BUF_NEEDED,
	     "CONFIG_BT_ISO_TX_BUF_COUNT should be at least "
	     "BROADCAST_ENQUEUE_COUNT * CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK_COUNT");

static bt_addr_le_t devices_list[16];
static bt_addr_le_t target_devices[2];
static int devices_list_count = 0;
static uint8_t sirk[BT_CSIP_SIRK_SIZE];
static bool set_sirk_set = false;

static struct bt_bap_unicast_client_cb unicast_client_cbs;
static struct bt_conn *default_conn[CONFIG_BT_MAX_CONN];
static int default_conn_index;
static struct bt_bap_unicast_group *unicast_group;

enum {
	AUDIO_SINK_CONFIGURED,
	AUDIO_SINK_QOS_SET,
	AUDIO_SINK_ENABLED,
	AUDIO_SINK_CONNECTED,
	AUDIO_SINK_STARTED,
	AUDIO_SINK_META_UPDATED,
};

static struct audio_sink {
	struct bt_bap_stream stream;
	struct bt_bap_ep *ep;
	enum bt_audio_location loc;
	atomic_t flags;
	struct k_sem sem;
} sinks[1]; /* Only support one sink instead of CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK_COUNT. */

NET_BUF_POOL_FIXED_DEFINE(tx_pool, TOTAL_BUF_NEEDED,
			  LC3_INPUT_FRAME_SIZE_MAX * MAX_AUDIO_CHANNEL_COUNT * BITS_RATES_OF_SAMPLE / 8,
			  CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

static enum bt_audio_location audio_receiver_loc[CONFIG_BT_MAX_CONN];

/* Select a codec configuration to apply that is mandatory to support by both client and server.
 * Allows this sample application to work without logic to parse the codec capabilities of the
 * server and selection of an appropriate codec configuration.
 */
static struct bt_bap_lc3_preset lc3_preset;
static int new_rtn = -1;
static int new_pd = -1;
static int new_phy = -1;
static uint8_t iso_packing = BT_ISO_PACKING_SEQUENTIAL;

struct named_lc3_preset {
	const char *name;
	struct bt_bap_lc3_preset preset;
};

#define LOCATION BT_AUDIO_LOCATION_FRONT_LEFT | BT_AUDIO_LOCATION_FRONT_RIGHT
#define CONTEXT BT_AUDIO_CONTEXT_TYPE_MEDIA

static const struct named_lc3_preset lc3_unicast_presets[] = {
	{"8_1_1", BT_BAP_LC3_UNICAST_PRESET_8_1_1(LOCATION, CONTEXT)},
	{"8_2_1", BT_BAP_LC3_UNICAST_PRESET_8_2_1(LOCATION, CONTEXT)},
	{"16_1_1", BT_BAP_LC3_UNICAST_PRESET_16_1_1(LOCATION, CONTEXT)},
	{"16_2_1", BT_BAP_LC3_UNICAST_PRESET_16_2_1(LOCATION, CONTEXT)},
	{"24_1_1", BT_BAP_LC3_UNICAST_PRESET_24_1_1(LOCATION, CONTEXT)},
	{"24_2_1", BT_BAP_LC3_UNICAST_PRESET_24_2_1(LOCATION, CONTEXT)},
	{"32_1_1", BT_BAP_LC3_UNICAST_PRESET_32_1_1(LOCATION, CONTEXT)},
	{"32_2_1", BT_BAP_LC3_UNICAST_PRESET_32_2_1(LOCATION, CONTEXT)},
	{"441_1_1", BT_BAP_LC3_UNICAST_PRESET_441_1_1(LOCATION, CONTEXT)},
	{"441_2_1", BT_BAP_LC3_UNICAST_PRESET_441_2_1(LOCATION, CONTEXT)},
	{"48_1_1", BT_BAP_LC3_UNICAST_PRESET_48_1_1(LOCATION, CONTEXT)},
	{"48_2_1", BT_BAP_LC3_UNICAST_PRESET_48_2_1(LOCATION, CONTEXT)},
	{"48_3_1", BT_BAP_LC3_UNICAST_PRESET_48_3_1(LOCATION, CONTEXT)},
	{"48_4_1", BT_BAP_LC3_UNICAST_PRESET_48_4_1(LOCATION, CONTEXT)},
	{"48_5_1", BT_BAP_LC3_UNICAST_PRESET_48_5_1(LOCATION, CONTEXT)},
	{"48_6_1", BT_BAP_LC3_UNICAST_PRESET_48_6_1(LOCATION, CONTEXT)},
	/* High-reliability presets */
	{"8_1_2", BT_BAP_LC3_UNICAST_PRESET_8_1_2(LOCATION, CONTEXT)},
	{"8_2_2", BT_BAP_LC3_UNICAST_PRESET_8_2_2(LOCATION, CONTEXT)},
	{"16_1_2", BT_BAP_LC3_UNICAST_PRESET_16_1_2(LOCATION, CONTEXT)},
	{"16_2_2", BT_BAP_LC3_UNICAST_PRESET_16_2_2(LOCATION, CONTEXT)},
	{"24_1_2", BT_BAP_LC3_UNICAST_PRESET_24_1_2(LOCATION, CONTEXT)},
	{"24_2_2", BT_BAP_LC3_UNICAST_PRESET_24_2_2(LOCATION, CONTEXT)},
	{"32_1_2", BT_BAP_LC3_UNICAST_PRESET_32_1_2(LOCATION, CONTEXT)},
	{"32_2_2", BT_BAP_LC3_UNICAST_PRESET_32_2_2(LOCATION, CONTEXT)},
	{"441_1_2", BT_BAP_LC3_UNICAST_PRESET_441_1_2(LOCATION, CONTEXT)},
	{"441_2_2", BT_BAP_LC3_UNICAST_PRESET_441_2_2(LOCATION, CONTEXT)},
	{"48_1_2", BT_BAP_LC3_UNICAST_PRESET_48_1_2(LOCATION, CONTEXT)},
	{"48_2_2", BT_BAP_LC3_UNICAST_PRESET_48_2_2(LOCATION, CONTEXT)},
	{"48_3_2", BT_BAP_LC3_UNICAST_PRESET_48_3_2(LOCATION, CONTEXT)},
	{"48_4_2", BT_BAP_LC3_UNICAST_PRESET_48_4_2(LOCATION, CONTEXT)},
	{"48_5_2", BT_BAP_LC3_UNICAST_PRESET_48_5_2(LOCATION, CONTEXT)},
	{"48_6_2", BT_BAP_LC3_UNICAST_PRESET_48_6_2(LOCATION, CONTEXT)},
};

/* set conn interval to 30ms and timeout to 300ms. */
static struct bt_le_conn_param conn_param = {
	.interval_min = 24,
	.interval_max = 24,
	.latency = 0,
	.timeout = 30,
};

static OSA_SEMAPHORE_HANDLE_DEFINE(sem_wav_opened);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_lc3_preset);
OSA_SEMAPHORE_HANDLE_DEFINE(sem_scan);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_device_selected);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_connected);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_csip_discovered);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_member_discovered);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_disconnected);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_mtu_exchanged);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_security_updated);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_sinks_discovered);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_vcs_discovered);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_sources_discovered);

static bool cis_stream_play = true;
static bool cis_stream_play_update = false;

static uint32_t seq_num = 0;
static uint64_t tx_samples = 0;
static uint32_t tx_time_stamp_start;

static int selected_sample_rate = 0;
static int selected_bits = 16;
static int selected_duration_us = 0;

static struct k_fifo tx_fifo;

static uint32_t get_cig_sync_delay(void)
{
	struct bt_iso_info iso_info;

	bt_iso_chan_get_info(&sinks[0].ep->iso->chan, &iso_info);

	return iso_info.unicast.cig_sync_delay;
}

static uint32_t get_iso_interval(void)
{
	struct bt_iso_info iso_info;
	uint32_t ISO_Interval_us;

	bt_iso_chan_get_info(&sinks[0].ep->iso->chan, &iso_info);

	ISO_Interval_us = iso_info.iso_interval * 1250;

	return ISO_Interval_us;
}

static uint32_t get_sync_signal_timestamp(void)
{
	uint32_t time_stamp;

	time_stamp = BOARD_SyncSignal_Count() * get_iso_interval() + get_cig_sync_delay();

	return time_stamp;
}

static void codec_rx_callback(uint8_t *rx_buffer)
{
	struct net_buf *buf;

	buf = net_buf_alloc(&tx_pool, 0U);
	if (NULL == buf)
	{
		return;
	}

	net_buf_add_mem(buf, rx_buffer, lc3_codec_info.bytes_per_channel_frame);

	net_buf_put(&tx_fifo, buf);
}

static void source_send_stream_task(void *param)
{
	struct net_buf *buf;
	uint8_t * buffer = NULL;
	int err;
	uint32_t sdu_time_stamp;

	while (true)
	{
		buf = net_buf_get(&tx_fifo, K_FOREVER);
		if (buf == NULL) {
			continue;
		}

		buffer = buf->data;
		net_buf_reset(buf);
		net_buf_reserve(buf, BT_ISO_CHAN_SEND_RESERVE);

		err = lc3_encoder(&encoder, buffer, buf->data);
		if(err < 0)
		{
			PRINTF("\nlc3_encoder fail!\r\n");
			net_buf_unref(buf);
			continue;
		}

		buffer = buf->data;
		net_buf_add(buf, lc3_codec_info.octets_per_frame);
		if (lc3_codec_info.channels > 1) {
			net_buf_add_mem(buf, buffer, lc3_codec_info.octets_per_frame);
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

		err = bt_bap_stream_send_ts(&sinks[0].stream, buf, seq_num, sdu_time_stamp);
		if (err < 0) {
			/* This will end send stream. */
			PRINTF("Unable to send stream on %p: %d\r\n", &sinks[0].stream, err);
			net_buf_unref(buf);
		}
		seq_num ++;
	}
}

static void print_lc3_preset(const char *name, const struct bt_bap_lc3_preset *preset)
{
	const struct bt_audio_codec_cfg *codec_cfg = &preset->codec_cfg;
	const struct bt_audio_codec_qos *qos = &preset->qos;

	PRINTF("%s:\r\n", name);

	PRINTF("\tcodec_cfg - sample_rate: %d, duration: %d, len: %d\r\n",
			bt_audio_codec_cfg_freq_to_freq_hz((enum bt_audio_codec_cfg_freq)bt_audio_codec_cfg_get_freq(codec_cfg)),
			bt_audio_codec_cfg_frame_dur_to_frame_dur_us((enum bt_audio_codec_cfg_frame_dur)bt_audio_codec_cfg_get_frame_dur(codec_cfg)),
			bt_audio_codec_cfg_get_octets_per_frame(codec_cfg)
		);

	PRINTF("\tqos - interval: %d, framing: %d, phy: %d, sdu: %d, rtn: %d, pd: %d\r\n",
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
	PRINTF("\nlc3 preset list:\r\n");
	for(int i = 0; i < ARRAY_SIZE(lc3_unicast_presets); i++)
	{
		if(sample_rate != 0)
		{
			const struct bt_audio_codec_cfg *codec_cfg = &lc3_unicast_presets[i].preset.codec_cfg;
			int codec_sample_rate = bt_audio_codec_cfg_freq_to_freq_hz((enum bt_audio_codec_cfg_freq)bt_audio_codec_cfg_get_freq(codec_cfg));
			if(codec_sample_rate != sample_rate)
			{
				continue;
			}
		}
		print_lc3_preset(lc3_unicast_presets[i].name, &lc3_unicast_presets[i].preset);
	}
}

int get_lc3_channel_count(enum bt_audio_location loc)
{
	int channel_count = 0;

	for (int index = 0; index < (sizeof(loc) * 8); index++) {
		if (((uint32_t)loc) & BIT(index)) {
			channel_count ++;
		}
	}

	return channel_count;
}

int select_lc3_preset(char *preset_name)
{
	bool find = false;
	int err;
	enum bt_audio_location loc;
	int channel_count = 0;

	for(int i = 0; i < ARRAY_SIZE(lc3_unicast_presets); i++)
	{
		if(0 == strcmp(lc3_unicast_presets[i].name, preset_name))
		{
			find = true;
			memcpy(&lc3_preset, &lc3_unicast_presets[i].preset, sizeof(lc3_preset));
			break;
		}
	}

	if(!find)
	{
		return -EINVAL;
	}

	err = bt_audio_codec_cfg_get_chan_allocation(&lc3_preset.codec_cfg, &loc, false);
	if (err < 0) {
		return err;
	}

	channel_count = get_lc3_channel_count(loc);
	if (channel_count == 0) {
		return -EINVAL;
	}

	print_lc3_preset(preset_name, &lc3_preset);

	lc3_preset.qos.sdu = bt_audio_codec_cfg_get_octets_per_frame(&lc3_preset.codec_cfg) * channel_count;

	(void)OSA_SemaphorePost(sem_lc3_preset);

	return 0;
}

int modify_rtn(int rtn)
{
	if(!IN_RANGE(rtn, 0, 255))
	{
		return -1;
	}

	new_rtn = rtn;

	return 0;
}

int modify_pd(int pd)
{
	if(!IN_RANGE(pd, 10000, 70000))
	{
		return -1;
	}

	new_pd = pd;

	return 0;
}

int modify_phy(int phy)
{
	/* 1: 1M, 2: 2M, 4: Coded. */
	if((phy == BT_AUDIO_CODEC_QOS_1M) || (phy == BT_AUDIO_CODEC_QOS_2M) || (phy == BT_AUDIO_CODEC_QOS_CODED))
	{
		new_phy = phy;
		return 0;
	}

	return -1;
}

int modify_packing(int packing)
{
	/* 0: sequentially, 1: interleaved. */
	if((packing == BT_ISO_PACKING_SEQUENTIAL) || (packing == BT_ISO_PACKING_INTERLEAVED))
	{
		iso_packing = packing;
		return 0;
	}

	return -1;
}

void print_sync_info(void)
{
	uint32_t iso_interval = get_iso_interval();
	uint32_t sync_delay = get_cig_sync_delay();

	PRINTF("sync info - iso_interval: %u, sync_delay: %u\r\n", iso_interval, sync_delay);
}

int modify_conn_param(int interval_min, int interval_max, int latency, int timeout)
{
	if(!IN_RANGE(interval_min, 0x0006, 0x0C80))
	{
		return -1;
	}

	if(!IN_RANGE(interval_max, 0x0006, 0x0C80))
	{
		return -1;
	}

	if(!IN_RANGE(latency, 0x0000, 0x01F3))
	{
		return -1;
	}

	if(!IN_RANGE(timeout, 0x000A, 0x0C80))
	{
		return -1;
	}

	conn_param.interval_min = interval_min;
	conn_param.interval_max = interval_max;
	conn_param.latency = latency;
	conn_param.timeout = timeout;

	return 0;
}

void config_channel_location(void)
{
	int count = 0;
	enum bt_audio_location loc = BT_AUDIO_LOCATION_MONO_AUDIO;

	for (int index = 0; index < CONFIG_BT_MAX_CONN; index++)
	{
		if(NULL != default_conn[index])
		{
			loc = audio_receiver_loc[index];
			break;
		}
	}

	sinks[count].loc = loc;
#if 0
	for (int index = 0; index < sizeof(loc); index++)
	{
		if ((uint32_t)loc & BIT(index)) {
			sinks[count].loc = (enum bt_audio_location)BIT(index);
			count ++;
			if (count >= ARRAY_SIZE(sinks)) {
				break;
			}
		}
	}
#endif
}

void config_audio_parameters(int sample_rate, int bits)
{
	enum bt_audio_location loc;
	int err;

	err = bt_audio_codec_cfg_get_chan_allocation(&lc3_preset.codec_cfg, &loc, false);
	if (err < 0) {
		PRINTF("Invalid channel count, channel location %08x\r\n", loc);
	}

	__ASSERT(!(err < 0), "Invalid channel count, channel location %08x", loc);

	/* set the LC3 encoder parameters. */
	lc3_codec_info.sample_rate = sample_rate;
	lc3_codec_info.frame_duration_us = bt_audio_codec_cfg_frame_dur_to_frame_dur_us((enum bt_audio_codec_cfg_frame_dur)bt_audio_codec_cfg_get_frame_dur(&lc3_preset.codec_cfg));
	lc3_codec_info.octets_per_frame = bt_audio_codec_cfg_get_octets_per_frame(&lc3_preset.codec_cfg);
	lc3_codec_info.blocks_per_sdu = 1;
	lc3_codec_info.chan_allocation = 0; /* not used. */

	lc3_codec_info.channels = get_lc3_channel_count(loc);
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
	lc3_codec_info.bytes_per_channel_frame = lc3_codec_info.samples_per_frame * bits / 8;

	/* LC3 Encoder Init. */
	err = lc3_encoder_init(&encoder, sample_rate, lc3_codec_info.frame_duration_us, lc3_codec_info.octets_per_frame, bits);
	if(err)
	{
		PRINTF("lc3_encoder_init fail err %d!\r\n", err);
	}
	PRINTF("LC3 encoder setup done!\r\n");

	/* set codec data. */

	/* set codec qos. */
}

static void print_hex(const uint8_t *ptr, size_t len)
{
	while (len-- != 0) {
		PRINTF("%02x", *ptr++);
	}
}

static bool print_cb(struct bt_data *data, void *user_data)
{
	const char *str = (const char *)user_data;

	PRINTF("%s: type 0x%02x value_len %u\r\n", str, data->type, data->data_len);
	print_hex(data->data, data->data_len);
	PRINTF("\r\n");

	return true;
}

static void print_codec_cap(const struct bt_audio_codec_cap *codec_cap)
{
	PRINTF("codec id 0x%02x cid 0x%04x vid 0x%04x count %u\r\n", codec_cap->id, codec_cap->cid,
	       codec_cap->vid, codec_cap->data_len);

	if (codec_cap->id == BT_HCI_CODING_FORMAT_LC3) {
		bt_audio_data_parse(codec_cap->data, codec_cap->data_len, print_cb, "data");
	} else { /* If not LC3, we cannot assume it's LTV */
		PRINTF("data: ");
		print_hex(codec_cap->data, codec_cap->data_len);
		PRINTF("\r\n");
	}

	bt_audio_data_parse(codec_cap->meta, codec_cap->meta_len, print_cb, "meta");
}

static void stream_configured(struct bt_bap_stream *stream,
			      const struct bt_audio_codec_qos_pref *pref)
{
	struct audio_sink *sink = (struct audio_sink *)CONTAINER_OF(stream, struct audio_sink, stream);
	PRINTF("Audio Stream %p configured\r\n", stream);

	atomic_set_bit(&sink->flags, AUDIO_SINK_CONFIGURED);
	k_sem_give(&sink->sem);
}

static void stream_qos_set(struct bt_bap_stream *stream)
{
	struct audio_sink *sink = (struct audio_sink *)CONTAINER_OF(stream, struct audio_sink, stream);
	PRINTF("Audio Stream %p QoS set\r\n", stream);

	atomic_set_bit(&sink->flags, AUDIO_SINK_QOS_SET);
	k_sem_give(&sink->sem);
}

static void stream_enabled(struct bt_bap_stream *stream)
{
	struct audio_sink *sink = (struct audio_sink *)CONTAINER_OF(stream, struct audio_sink, stream);
	PRINTF("Audio Stream %p enabled\r\n", stream);

	if(sink == &sinks[0])
	{
		seq_num = 0;
		BOARD_SyncSignal_Start(0);
		BOARD_StartCodec(codec_rx_callback, selected_sample_rate, selected_bits, selected_duration_us);
	}

	atomic_set_bit(&sink->flags, AUDIO_SINK_ENABLED);
	k_sem_give(&sink->sem);
}

static void stream_connected(struct bt_bap_stream *stream)
{
	struct audio_sink *sink = (struct audio_sink *)CONTAINER_OF(stream, struct audio_sink, stream);
	PRINTF("Audio Stream %p connected\r\n", stream);

	atomic_set_bit(&sink->flags, AUDIO_SINK_CONNECTED);
	k_sem_give(&sink->sem);
}

static void stream_started(struct bt_bap_stream *stream)
{
	struct audio_sink *sink = (struct audio_sink *)CONTAINER_OF(stream, struct audio_sink, stream);
	PRINTF("Audio Stream %p started\r\n", stream);

	atomic_set_bit(&sink->flags, AUDIO_SINK_STARTED);
	k_sem_give(&sink->sem);
}

static void stream_metadata_updated(struct bt_bap_stream *stream)
{
	struct audio_sink *sink = (struct audio_sink *)CONTAINER_OF(stream, struct audio_sink, stream);
	PRINTF("Audio Stream %p metadata updated\r\n", stream);

	atomic_set_bit(&sink->flags, AUDIO_SINK_META_UPDATED);
	k_sem_give(&sink->sem);
}

static void stream_disabled(struct bt_bap_stream *stream)
{
	struct audio_sink *sink = (struct audio_sink *)CONTAINER_OF(stream, struct audio_sink, stream);
	PRINTF("Audio Stream %p disabled\r\n", stream);

	atomic_clear_bit(&sink->flags, AUDIO_SINK_ENABLED);
	atomic_clear_bit(&sink->flags, AUDIO_SINK_CONNECTED);
	atomic_clear_bit(&sink->flags, AUDIO_SINK_STARTED);
	k_sem_give(&sink->sem);
}

static void stream_stopped(struct bt_bap_stream *stream, uint8_t reason)
{
	struct audio_sink *sink = (struct audio_sink *)CONTAINER_OF(stream, struct audio_sink, stream);

	BOARD_SyncSignal_Stop();
	BOARD_StopCodec();
	PRINTF("Audio Stream %p stopped with reason 0x%02X\r\n", stream, reason);

	atomic_clear_bit(&sink->flags, AUDIO_SINK_STARTED);
	k_sem_give(&sink->sem);
}

static void stream_released(struct bt_bap_stream *stream)
{
	struct audio_sink *sink = (struct audio_sink *)CONTAINER_OF(stream, struct audio_sink, stream);
	PRINTF("Audio Stream %p released\r\n", stream);

	atomic_set(&sink->flags, 0);
	k_sem_give(&sink->sem);
}

static struct bt_bap_stream_ops stream_ops = {
	.configured = stream_configured,
	.qos_set = stream_qos_set,
	.enabled = stream_enabled,
	.started = stream_started,
	.metadata_updated = stream_metadata_updated,
	.disabled = stream_disabled,
	.stopped = stream_stopped,
	.released = stream_released,
	.connected = stream_connected,
};

static void add_remote_sink(struct bt_bap_ep *ep)
{
	PRINTF("Sink: ep %p\r\n", ep);

	for (int i = 0; i < ARRAY_SIZE(sinks); i++) {
		if (sinks[i].ep == NULL) {
			sinks[i].ep = ep;
			k_sem_init(&sinks[i].sem, 0, 0xff);
			return;
		}
	}

	PRINTF("Could not add sink ep\r\n");
}

static void print_remote_codec_cap(const struct bt_audio_codec_cap *codec_cap,
				   enum bt_audio_dir dir)
{
	PRINTF("codec_cap %p dir 0x%02x\r\n", codec_cap, dir);

	print_codec_cap(codec_cap);
}

static void discover_sinks_cb(struct bt_conn *conn, int err, enum bt_audio_dir dir)
{
	if (err != 0 && err != BT_ATT_ERR_ATTRIBUTE_NOT_FOUND) {
		PRINTF("Discovery failed: %d\r\n", err);
		return;
	}

	if (err == BT_ATT_ERR_ATTRIBUTE_NOT_FOUND) {
		PRINTF("Discover sinks completed without finding any sink ASEs\r\n");
	} else {
		PRINTF("Discover sinks complete: err %d\r\n", err);
	}

	OSA_SemaphorePost(sem_sinks_discovered);

}

static void connected(struct bt_conn *conn, uint8_t err)
{
	char addr[BT_ADDR_LE_STR_LEN];
	struct bt_conn_info info;

	bt_conn_get_info(conn, &info);

	if(info.type == BT_CONN_TYPE_LE)
	{

		(void)bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

		if (err != 0) {
			PRINTF("LE Failed to connect to %s (%u)\r\n", addr, err);

			bt_conn_unref(default_conn[default_conn_index]);
			default_conn[default_conn_index] = NULL;

			return;
		}

		if (conn != default_conn[default_conn_index]) {
			return;
		}

		PRINTF("LE Connected: %s\r\n", addr);

		OSA_SemaphorePost(sem_connected);
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int i;

	struct bt_conn_info info;

	bt_conn_get_info(conn, &info);

    	if(info.type == BT_CONN_TYPE_LE)
	{
		for (i = 0; i < CONFIG_BT_MAX_CONN; i++)
		{
			if(conn == default_conn[i])
			{
				break;
			}
		}

		if(i >= CONFIG_BT_MAX_CONN)
		{
			return;
		}

		if (conn != default_conn[i]) {
			return;
		}

		(void)bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

		PRINTF("LE Disconnected: %s (reason 0x%02x)\r\n", addr, reason);

		bt_conn_unref(default_conn[i]);
		default_conn[i] = NULL;

		OSA_SemaphorePost(sem_disconnected);
	}
}

static void security_changed_cb(struct bt_conn *conn, bt_security_t level,
				enum bt_security_err err)
{
	struct bt_conn_info info;

	bt_conn_get_info(conn, &info);

    	if(info.type == BT_CONN_TYPE_LE)
	{
		if (err == 0) {
			OSA_SemaphorePost(sem_security_updated);
		} else {
			PRINTF("LE Failed to set security level: %u", err);
		}
	}
}

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
    .security_changed = security_changed_cb
};

static void att_mtu_updated(struct bt_conn *conn, uint16_t tx, uint16_t rx)
{
	PRINTF("MTU exchanged: %u/%u\r\n", tx, rx);
	OSA_SemaphorePost(sem_mtu_exchanged);
}

static struct bt_gatt_cb gatt_callbacks = {
	.att_mtu_updated = att_mtu_updated,
};

static void unicast_client_location_cb(struct bt_conn *conn,
				      enum bt_audio_dir dir,
				      enum bt_audio_location loc)
{
	PRINTF("dir %u loc %X\r\n", dir, loc);

	for(int index = 0; index < CONFIG_BT_MAX_CONN; index++)
	{
		if(conn == default_conn[index])
		{
			audio_receiver_loc[index] = loc;
			break;
		}
	}
}

static void available_contexts_cb(struct bt_conn *conn,
				  enum bt_audio_context snk_ctx,
				  enum bt_audio_context src_ctx)
{
	PRINTF("snk ctx %u src ctx %u\r\n", snk_ctx, src_ctx);
}

static void pac_record_cb(struct bt_conn *conn, enum bt_audio_dir dir,
			  const struct bt_audio_codec_cap *codec_cap)
{
	print_remote_codec_cap(codec_cap, dir);
}

static void endpoint_cb(struct bt_conn *conn, enum bt_audio_dir dir, struct bt_bap_ep *ep)
{
	if (dir == BT_AUDIO_DIR_SINK) {
		add_remote_sink(ep);
	}
}

static struct bt_bap_unicast_client_cb unicast_client_cbs = {
	.location = unicast_client_location_cb,
	.available_contexts = available_contexts_cb,
	.pac_record = pac_record_cb,
	.endpoint = endpoint_cb,
};

static int init(void)
{
	(void)OSA_SemaphoreCreate(sem_wav_opened, 0);
	(void)OSA_SemaphoreCreate(sem_lc3_preset, 0);
	(void)OSA_SemaphoreCreate(sem_scan, 0);
	(void)OSA_SemaphoreCreate(sem_device_selected, 0);
	(void)OSA_SemaphoreCreate(sem_connected, 0);
	(void)OSA_SemaphoreCreate(sem_csip_discovered, 0);
	(void)OSA_SemaphoreCreate(sem_member_discovered, 0);
	(void)OSA_SemaphoreCreate(sem_disconnected, 0);
	(void)OSA_SemaphoreCreate(sem_mtu_exchanged, 0);
	(void)OSA_SemaphoreCreate(sem_security_updated, 0);
	(void)OSA_SemaphoreCreate(sem_sinks_discovered, 0);
	(void)OSA_SemaphoreCreate(sem_vcs_discovered, 0);
	(void)OSA_SemaphoreCreate(sem_sources_discovered, 0);

	int err = bt_enable(NULL);
	if (err != 0) {
		PRINTF("Bluetooth enable failed (err %d)\r\n", err);
		return err;
	}

	bt_conn_cb_register(&conn_callbacks);
	bt_gatt_cb_register(&gatt_callbacks);

	return 0;
}

static bool get_advertise_data(struct net_buf_simple *ad, uint8_t type, struct bt_data *ad_data)
{
	bool found = false;
	struct net_buf_simple_state state;

	net_buf_simple_save(ad, &state);

	while(ad->len > 1)
	{
		uint8_t len = net_buf_simple_pull_u8(ad);
		if (len == 0U) {
			/* Early termination */
			break;
		}

		if (len > ad->len) {
			/* malformed advertising data */
			break;
		}

		uint8_t ad_type = net_buf_simple_pull_u8(ad);
		if(ad_type == type)
		{
			if(ad_data)
			{
				ad_data->type = type;
				ad_data->data_len = len - 1;
				ad_data->data = ad->data;
			}
			found = true;
			break;
		}

		net_buf_simple_pull(ad, len - 1);
	}

	net_buf_simple_restore(ad, &state);

	return found;
}

static void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type, struct net_buf_simple *ad)
{
	struct bt_data ad_data;
	char addr_str[BT_ADDR_LE_STR_LEN];
	char device_name[32];

#if 0
	/* Resolvable Set Identifier */
	if(!get_advertise_data(ad, BT_DATA_CSIS_RSI, NULL))
	{
		return;
	}
#endif
	if(rssi < -70)
	{
		return;
	}

	/* skip the addr in the list. */
	for(int i = 0; i < devices_list_count; i++)
	{
		if(0 == memcmp(addr, &devices_list[i], sizeof(bt_addr_le_t)))
		{
			return;
		}
	}

	/* save the addr */
	if (devices_list_count >= ARRAY_SIZE(devices_list))
	{
		return;
	}
	memcpy(&devices_list[devices_list_count], addr, sizeof(bt_addr_le_t));
	devices_list_count += 1;

	/* print the device info */
	(void)bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	memset(device_name, 0, sizeof(device_name));
	if(get_advertise_data(ad, BT_DATA_NAME_COMPLETE, &ad_data))
	{
		memcpy(device_name, ad_data.data, (ad_data.data_len < sizeof(device_name)) ? ad_data.data_len : (sizeof(device_name) - 1));
	}
	PRINTF("[%d]: %s, rssi %d, %s\r\n", devices_list_count - 1, addr_str, rssi, device_name);
}

int device_scan(void)
{
	int err;

	/* This demo doesn't require active scan */
	err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, scan_cb);
	if(err)
	{
		if(err == -EALREADY) {
			PRINTF("Scan already started!\r\n");
		}
		else
		{
			PRINTF("Scan failed to start (err %d)\r\n", err);
			return -1;
		}
	}

	return 0;
}

int device_select(int index)
{
	if(!IN_RANGE(index, 0, devices_list_count))
	{
		PRINTF("index should in range [0, %d]\r\n", devices_list_count - 1);
		return -1;
	}

	memcpy(&target_devices[0], &devices_list[index], sizeof(bt_addr_le_t));

	int err = bt_le_scan_stop();
	if(err)
	{
		PRINTF("scan stop fail with err %d\r\n", err);
	}

	(void)OSA_SemaphorePost(sem_device_selected);

	return 0;
}

static void member_scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type, struct net_buf_simple *ad)
{
	struct bt_data ad_data;
	char addr_str[BT_ADDR_LE_STR_LEN];
	char device_name[32];

	if(rssi < -70)
	{
		return;
	}

	/* Resolvable Set Identifier */
	if(!get_advertise_data(ad, BT_DATA_CSIS_RSI, &ad_data))
	{
		return;
	}

	if(!bt_csip_set_coordinator_is_set_member(sirk, &ad_data))
	{
		return;
	}

	/* save the addr */
	memcpy(&target_devices[1], addr, sizeof(bt_addr_le_t));

	/* print the device info */
	(void)bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	memset(device_name, 0, sizeof(device_name));
	if(get_advertise_data(ad, BT_DATA_NAME_COMPLETE, &ad_data))
	{
		memcpy(device_name, ad_data.data, (ad_data.data_len < sizeof(device_name)) ? ad_data.data_len : (sizeof(device_name) - 1));
	}
	PRINTF("member: %s, rssi %d, %s\r\n", addr_str, rssi, device_name);

	int err = bt_le_scan_stop();
	if(err)
	{
		PRINTF("scan stop fail with err %d\r\n", err);
	}

	(void)OSA_SemaphorePost(sem_member_discovered);
}

static int member_scan(void)
{
	int err;

	/* This demo doesn't require active scan */
	err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, member_scan_cb);
	if(err)
	{
		if(err == -EALREADY) {
			PRINTF("Scan already started!\r\n");
		}
		else
		{
			PRINTF("Scan failed to start (err %d)\r\n", err);
			return -1;
		}
	}

	return 0;
}

static int device_connect(int index)
{
	int err;

	if(index == 0)
	{
		PRINTF("Connect first device\r\n");
		default_conn_index = 0;
	}
	if(index == 1)
	{
		PRINTF("Connect second device\r\n");
		default_conn_index = 1;
	}

	err = bt_conn_le_create(&target_devices[default_conn_index], BT_CONN_LE_CREATE_CONN,
		&conn_param,
		&default_conn[default_conn_index]);
	if (err != 0) {
		PRINTF("Create conn to failed (%u)\r\n", err);
		return err;
	}

	err = OSA_SemaphoreWait(sem_connected, osaWaitForever_c);
	if (err != 0) {
		PRINTF("failed to take sem_connected (err %d)\r\n", err);
		return err;
	}

	err = OSA_SemaphoreWait(sem_mtu_exchanged, osaWaitForever_c);
	if (err != 0) {
		PRINTF("failed to take sem_mtu_exchanged (err %d)\r\n", err);
		return err;
	}

	err = bt_conn_set_security(default_conn[index], BT_SECURITY_L2);
	if (err != 0) {
		PRINTF("failed to set security (err %d)\r\n", err);
	}

	err = OSA_SemaphoreWait(sem_security_updated, osaWaitForever_c);
	if (err != 0) {
		PRINTF("failed to take sem_security_updated (err %d)\r\n", err);
		return err;
	}

	return 0;
}

static void vcs_client_discover_callback(struct bt_conn *conn, int err)
{
	if(err)
	{
		PRINTF("VCS discover failed with %d\r\n", err);
		return;
	}

	(void)OSA_SemaphorePost(sem_vcs_discovered);
}

static int discover_vcs(int index)
{
	int ret;
	ret = le_audio_vcs_discover(default_conn[index], index);
	if(ret)
	{
		PRINTF("vcs discover fail %d\r\n", ret);
		return -1;
	}

	(void)OSA_SemaphoreWait(sem_vcs_discovered, osaWaitForever_c);

        return 0;
}

static int discover_sinks(int index)
{
	int err;

	unicast_client_cbs.discover = discover_sinks_cb;

	err = bt_bap_unicast_client_discover(default_conn[index], BT_AUDIO_DIR_SINK);
	if (err != 0) {
		PRINTF("Failed to discover sinks: %d\r\n", err);
		return err;
	}

	err = OSA_SemaphoreWait(sem_sinks_discovered, osaWaitForever_c);
	if (err != 0) {
		PRINTF("failed to take sem_sinks_discovered (err %d)\r\n", err);
		return err;
	}

	return 0;
}

static int configure_stream(struct audio_sink *sink)
{
	int err;
	int i;

	/* change channel allocation. */
	bt_audio_codec_cfg_set_chan_allocation(&lc3_preset.codec_cfg, sink->loc);

	err = bt_bap_stream_config(default_conn[0], &sink->stream, sink->ep,
					&lc3_preset.codec_cfg);
	if (err != 0) {
		return err;
	}

	err = k_sem_take(&sink->sem, K_FOREVER);
	if (err != 0) {
		PRINTF("failed to take sem (err %d)\r\n", err);
		return err;
	}

	if (!atomic_test_bit(&sink->flags, AUDIO_SINK_CONFIGURED)) {
		PRINTF("FLAG AUDIO_SINK_CONFIGURED is not set");
		return -EINVAL;
	}

	return 0;
}

static int configure_streams()
{
	int err;
	struct audio_sink *sink;

	config_channel_location();

	for (int index = 0; index < ARRAY_SIZE(sinks); index++ ) {
		sink = &sinks[index];

		if (sinks[index].ep == NULL) {
			return -EINVAL;
		}

		err = configure_stream(sink);
		if (err != 0) {
			PRINTF("Could not configure sink sinks[%zu]: %d\r\n", index, err);
			return err;
		}

		PRINTF("Configured sink sinks[%zu]\r\n", index);
	}

	return 0;
}

static int create_group(void)
{
	const size_t params_count = ARRAY_SIZE(sinks);
	struct bt_bap_unicast_group_stream_pair_param pair_params[ARRAY_SIZE(sinks) + CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC_COUNT];
	struct bt_bap_unicast_group_stream_param stream_params[ARRAY_SIZE(sinks) + CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC_COUNT];
	struct bt_bap_unicast_group_param param;
	int err;

	for (size_t i = 0U; i < ARRAY_SIZE(sinks); i++) {
		stream_params[i].stream = &sinks[i].stream;
		stream_params[i].qos = &lc3_preset.qos;

		pair_params[i].tx_param = &stream_params[i];
		pair_params[i].rx_param = NULL;
	}

	param.params = pair_params;
	param.params_count = params_count;
	param.packing = iso_packing;

	err = bt_bap_unicast_group_create(&param, &unicast_group);
	if (err != 0) {
		PRINTF("Could not create unicast group (err %d)\r\n", err);
		return err;
	}

	return 0;
}

static int delete_group(void)
{
	int err;

	err = bt_bap_unicast_group_delete(unicast_group);
	if (err != 0) {
		PRINTF("Could not create unicast group (err %d)\r\n", err);
		return err;
	}

	return 0;
}

static int set_stream_qos(int index)
{
	int err;

	err = bt_bap_stream_qos(default_conn[index], unicast_group);
	if (err != 0) {
		PRINTF("Unable to setup QoS: %d\r\n", err);
		return err;
	}

	for (int index = 0; index < ARRAY_SIZE(sinks); index++ ) {
		if (sinks[index].ep == NULL) {
			continue;
		}

		PRINTF("QoS: waiting for %zu sink\r\n", index);

		err = k_sem_take(&sinks[index].sem, K_FOREVER);
		if (err != 0) {
			PRINTF("failed to take sem (err %d)\r\n", err);
			return err;
		}

		if (!atomic_test_bit(&sinks[index].flags, AUDIO_SINK_QOS_SET)) {
			PRINTF("FLAG AUDIO_SINK_QOS_SET is not set");
			return -EINVAL;
		}

		PRINTF("QOS Set sink sinks[%zu]\r\n", index);
	}

	return 0;
}

static int enable_streams()
{
	int err;

	for (int index = 0; index < ARRAY_SIZE(sinks); index++ ) {
		if (sinks[index].ep == NULL) {
			continue;
		}

		err = bt_bap_stream_enable(&sinks[index].stream,
						lc3_preset.codec_cfg.meta,
						lc3_preset.codec_cfg.meta_len);
		if (err != 0) {
			PRINTF("Unable to enable stream: %d\r\n", err);
			return err;
		}

		err = k_sem_take(&sinks[index].sem, K_FOREVER);
		if (err != 0) {
			PRINTF("failed to take sem (err %d)\r\n", err);
			return err;
		}

		if (!atomic_test_bit(&sinks[index].flags, AUDIO_SINK_ENABLED)) {
			PRINTF("FLAG AUDIO_SINK_ENABLED is not set");
			return -EINVAL;
		}

		PRINTF("Enabled sink sinks[%zu]\r\n", index);
	}

	return 0;
}

static int disable_streams()
{
	int err;

	/* MCUX-62728: workaround to disconnect 2rd CIS first for controller fw limitation. */

	for (int index = (ARRAY_SIZE(sinks) - 1); index >= 0; index-- ) {
		if (sinks[index].ep == NULL) {
			continue;
		}

		err = bt_bap_stream_disable(&sinks[index].stream);
		if (err != 0) {
			PRINTF("Unable to disable stream: %d\r\n", err);
			return err;
		}

		err = k_sem_take(&sinks[index].sem, K_FOREVER);
		if (err != 0) {
			PRINTF("failed to take sem (err %d)\r\n", err);
			return err;
		}

		if (atomic_test_bit(&sinks[index].flags, AUDIO_SINK_ENABLED)) {
			PRINTF("FLAG AUDIO_SINK_ENABLED is not cleared");
			return -EINVAL;
		}

		PRINTF("Disabled sink sinks[%zu]\r\n", index);
	}

	return 0;
}

static int connect_streams()
{
	int err;

	for (int index = 0; index < ARRAY_SIZE(sinks); index++ ) {
		if (sinks[index].ep == NULL) {
			continue;
		}

		err = bt_bap_stream_connect(&sinks[index].stream);
		if (err != 0) {
			PRINTF("Unable to start stream: %d\r\n", err);
			return err;
		}

		err = k_sem_take(&sinks[index].sem, K_FOREVER);
		if (err != 0) {
			PRINTF("failed to take sem (err %d)\r\n", err);
			return err;
		}

		if (!atomic_test_bit(&sinks[index].flags, AUDIO_SINK_CONNECTED)) {
			PRINTF("FLAG AUDIO_SINK_CONNECTED is not set");
			return -EINVAL;
		}

		PRINTF("Connect sink sinks[%zu]\r\n", index);
	}

	return 0;
}

static int start_streams()
{
	int err;

	for (int index = 0; index < ARRAY_SIZE(sinks); index++ ) {
		if (sinks[index].ep == NULL) {
			continue;
		}

		err = k_sem_take(&sinks[index].sem, K_FOREVER);
		if (err != 0) {
			PRINTF("failed to take sem (err %d)\r\n", err);
			return err;
		}

		if (!atomic_test_bit(&sinks[index].flags, AUDIO_SINK_STARTED)) {
			PRINTF("FLAG AUDIO_SINK_STARTED is not set");
			return -EINVAL;
		}
	}

	BOARD_StartStream();

	return 0;
}

static void reset_data(void)
{
	(void)OSA_SemaphoreDestroy(sem_wav_opened);
	(void)OSA_SemaphoreDestroy(sem_lc3_preset);
	(void)OSA_SemaphoreDestroy(sem_scan);
	(void)OSA_SemaphoreDestroy(sem_device_selected);
	(void)OSA_SemaphoreDestroy(sem_connected);
	(void)OSA_SemaphoreDestroy(sem_csip_discovered);
	(void)OSA_SemaphoreDestroy(sem_member_discovered);
	(void)OSA_SemaphoreDestroy(sem_disconnected);
	(void)OSA_SemaphoreDestroy(sem_mtu_exchanged);
	(void)OSA_SemaphoreDestroy(sem_security_updated);
	(void)OSA_SemaphoreDestroy(sem_sinks_discovered);
	(void)OSA_SemaphoreDestroy(sem_vcs_discovered);
	(void)OSA_SemaphoreDestroy(sem_sources_discovered);

	(void)OSA_SemaphoreCreate(sem_wav_opened, 0);
	(void)OSA_SemaphoreCreate(sem_lc3_preset, 0);
	(void)OSA_SemaphoreCreate(sem_scan, 0);
	(void)OSA_SemaphoreCreate(sem_device_selected, 0);
	(void)OSA_SemaphoreCreate(sem_connected, 0);
	(void)OSA_SemaphoreCreate(sem_csip_discovered, 0);
	(void)OSA_SemaphoreCreate(sem_member_discovered, 0);
	(void)OSA_SemaphoreCreate(sem_disconnected, 0);
	(void)OSA_SemaphoreCreate(sem_mtu_exchanged, 0);
	(void)OSA_SemaphoreCreate(sem_security_updated, 0);
	(void)OSA_SemaphoreCreate(sem_sinks_discovered, 0);
	(void)OSA_SemaphoreCreate(sem_vcs_discovered, 0);
	(void)OSA_SemaphoreCreate(sem_sources_discovered, 0);

	memset(sinks, 0, sizeof(sinks));

	for (size_t i = 0; i < ARRAY_SIZE(sinks); i++) {
		sinks[i].stream.ops = &stream_ops;
	}
}

void mcs_server_state_cb(int state)
{
	if(state == MCS_SERVER_STATE_PLAYING)
	{
		PRINTF("\nMCS server state: playing\r\n");
		if(cis_stream_play)
		{
			return;
		}

		cis_stream_play_update = true;
		cis_stream_play = true;
	}

	if(state == MCS_SERVER_STATE_PAUSED)
	{
		PRINTF("\nMCS server state: pause\r\n");
		if(!cis_stream_play)
		{
			return;
		}

		cis_stream_play_update = true;
		cis_stream_play = false;
	}
}

static void csip_set_coordinator_discover_cb(
	struct bt_conn *conn,
	const struct bt_csip_set_coordinator_set_member *member,
	int err, size_t set_count)
{
	if(err) {
		PRINTF("CSIP conn %p csip discover err %d\r\n", conn, err);
	}
	else {
		PRINTF("CSIP conn %p discovered set count %d\r\n", conn, set_count);
		for(int i = 0; i < set_count; i++)
		{
			const struct bt_csip_set_coordinator_set_info *info = &member->insts[i].info;
			PRINTF("set %d/%d info:\r\n", i+1, set_count);
			PRINTF("\tsirk: ");
			for(int j = 0; j < BT_CSIP_SIRK_SIZE; j++)
			{
				PRINTF("%02x ", info->sirk[j]);
			}
			PRINTF("\r\n");

			PRINTF("\tset_size: %d\r\n", info->set_size);

			PRINTF("\trank: %d\r\n", info->rank);

			PRINTF("\tlockable: %d\r\n", info->lockable);

			if(!set_sirk_set)
			{
				set_sirk_set = true;
				memcpy(&sirk, &info->sirk[i], BT_CSIP_SIRK_SIZE);
			}
		}
	}
	(void)OSA_SemaphorePost(sem_csip_discovered);
}

static struct bt_csip_set_coordinator_cb csip_cb = {
	.discover = csip_set_coordinator_discover_cb
};

void ums_microphone_task(void *param)
{
	int err;

	/* shell init. */
	le_audio_shell_init();

	PRINTF("Initializing\r\n");
	err = init();
	if (err != 0) {
		while(1);
	}
	PRINTF("Initialized\r\n");

	if (xTaskCreate(source_send_stream_task, "source_send_stream_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
	{
	    PRINTF("Source stream send task creation failed!\r\n");
	    return;
	}

	/* VCS */
	le_audio_vcs_client_init(vcs_client_discover_callback);

	/* MCS */
	le_audio_mcs_server_init(mcs_server_state_cb);

	/* Select LC3 preset */
	PRINTF("\nPlease select lc3 preset use \"lc3_preset <name>\" command.\r\n");
	OSA_SemaphoreWait(sem_lc3_preset, osaWaitForever_c);
	/* Config audio parameters. */
	selected_sample_rate = bt_audio_codec_cfg_freq_to_freq_hz((enum bt_audio_codec_cfg_freq)bt_audio_codec_cfg_get_freq(&lc3_preset.codec_cfg));
	config_audio_parameters(selected_sample_rate, selected_bits);

	selected_duration_us = bt_audio_codec_cfg_frame_dur_to_frame_dur_us((enum bt_audio_codec_cfg_frame_dur)bt_audio_codec_cfg_get_frame_dur(&lc3_preset.codec_cfg));

	/* overlay rtn & pd if set */
	if(new_rtn >= 0)
	{
		lc3_preset.qos.rtn = new_rtn;
	}
	if(new_pd >= 0)
	{
		lc3_preset.qos.pd = new_pd;
	}
	if(new_phy > 0)
	{
		lc3_preset.qos.phy = new_phy;
	}

	if((new_rtn >= 0) || (new_pd >= 0) || (new_phy > 0))
	{
		print_lc3_preset("new_preset", &lc3_preset);
	}

	err = bt_bap_unicast_client_register_cb(&unicast_client_cbs);
	if (err != 0) {
		PRINTF("Failed to register client callbacks: %d", err);
		while(1);
	}

	while (true) {
		reset_data();

		PRINTF("Creating unicast group\r\n");
		err = create_group();
		if (err != 0) {
			break;
		}
		PRINTF("Unicast group created\r\n");

		PRINTF("Please scan and connect the devices you want!\r\n");
		err = OSA_SemaphoreWait(sem_scan, osaWaitForever_c);
		if (err != 0) {
			PRINTF("failed to take sem_scan (err %d)\r\n", err);
			break;
		}

		err = device_scan();
		if (err != 0) {
			PRINTF("device scan err %d\r\n", err);
			break;
		}
		PRINTF("Scanning successfully started\r\n");

		err = OSA_SemaphoreWait(sem_device_selected, osaWaitForever_c);
		if (err != 0) {
			PRINTF("failed to take sem_device_selected (err %d)\r\n", err);
			break;
		}
		PRINTF("device selected!\r\n");

		PRINTF("Connecting\r\n");
		err = device_connect(0);
		if (err != 0) {
			break;
		}
		PRINTF("Connected\r\n");

		PRINTF("Discover VCS\r\n");
		err = discover_vcs(0);
		if(err)
		{
			PRINTF("discover vcs %d failed, error %d\r\n", 0, err);
		}
		PRINTF("Discover VCS complete.\r\n");

		PRINTF("Discovering sinks\r\n");
		err = discover_sinks(0);
		if (err != 0) {
			break;
		}
		PRINTF("Sinks discovered\r\n");


		PRINTF("Configuring streams\r\n");
		err = configure_streams();
		if (err != 0) {
			break;
		}
		PRINTF("Stream configured\r\n");

		PRINTF("Setting stream QoS\r\n");
		err = set_stream_qos(0);
		if (err != 0) {
			break;
		}
		PRINTF("Stream QoS Set\r\n");

		PRINTF("Enabling streams\r\n");
		err = enable_streams();
		if (err != 0) {
			break;
		}
		PRINTF("Streams enabled\r\n");

		PRINTF("Connecting streams\r\n");
		err = connect_streams();
		if (err != 0) {
			break;
		}
		PRINTF("Streams connected\r\n");

		PRINTF("Starting streams\r\n");
		err = start_streams();
		if (err != 0) {
			break;
		}
		PRINTF("Streams started\r\n");

		do{
			if (cis_stream_play)
			{
				if(cis_stream_play_update)
				{
					cis_stream_play_update = false;

					err = enable_streams();
					if (err) {
						PRINTF("\nEnable stream err %d\r\n", err);
					}

					err = connect_streams();
					if (err) {
						PRINTF("\nConnect stream err %d\r\n", err);
					}

					err = start_streams();
					if (err) {
						PRINTF("\nStart stream err %d\r\n", err);
					}
				}
			}
			else
			{
				if(cis_stream_play_update)
				{
					cis_stream_play_update = false;

					/* send 2 mute frames to sink to avoid LC3 PLC noise. */
					err = disable_streams();
					if (err) {
						PRINTF("\nDisable stream err %d\r\n", err);
					}
				}
				OSA_TimeDelay(2);
			}
		} while (0 == err);

		/* Wait for disconnect */
		err = OSA_SemaphoreWait(sem_disconnected, osaWaitForever_c);
		if (err != 0) {
			PRINTF("failed to take sem_disconnected (err %d)\r\n", err);
			break;
		}

		PRINTF("Deleting group\r\n");
		err = delete_group();
		if (err != 0) {
			break;
		}
		PRINTF("Group deleted\r\n");
	}

	while(1);
}
