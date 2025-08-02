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
#include <bluetooth/byteorder.h>
#include <bluetooth/conn.h>
#include <bluetooth/audio/audio.h>
#include <bluetooth/audio/bap.h>
#include <bluetooth/audio/pacs.h>
#include <bluetooth/audio/csip.h>
#include <sys/byteorder.h>

#include <bluetooth/audio/vcp.h>
#include <bluetooth/audio/mcs.h>
#include <bluetooth/audio/mcc.h>
#include <bluetooth/audio/media_proxy.h>

#include <bluetooth/audio/bap_lc3_preset.h>

#include "le_audio_common.h"
#include "le_audio_shell.h"
#include "unicast_media_receiver.h"

#if defined(LE_AUDIO_SYNC_ENABLE) && (LE_AUDIO_SYNC_ENABLE > 0)
/* Note: this include should be remove once audio api could get bt_iso_chan. */
#include "audio/bap_endpoint.h"
#include "audio/bap_iso.h"
#endif

#define AVAILABLE_SINK_CONTEXT  (BT_AUDIO_CONTEXT_TYPE_UNSPECIFIED | \
				 BT_AUDIO_CONTEXT_TYPE_CONVERSATIONAL | \
				 BT_AUDIO_CONTEXT_TYPE_MEDIA | \
				 BT_AUDIO_CONTEXT_TYPE_GAME | \
				 BT_AUDIO_CONTEXT_TYPE_INSTRUCTIONAL)

#define AVAILABLE_SOURCE_CONTEXT (BT_AUDIO_CONTEXT_TYPE_PROHIBITED)



static const struct bt_audio_codec_cap lc3_codec_cap[] = {BT_AUDIO_CODEC_CAP_LC3(
	BT_AUDIO_CODEC_CAP_FREQ_ANY, BT_AUDIO_CODEC_CAP_DURATION_10,
	BT_AUDIO_CODEC_CAP_CHAN_COUNT_SUPPORT(2), 40u, 120u, 1u,
	(BT_AUDIO_CONTEXT_TYPE_CONVERSATIONAL | BT_AUDIO_CONTEXT_TYPE_MEDIA)),};

static struct bt_conn *default_conn;
static struct bt_bap_stream sink_streams[CONFIG_BT_ASCS_ASE_SNK_COUNT];

static struct bt_bap_lc3_preset lc3_preset;

static const struct bt_audio_codec_qos_pref qos_pref =
	BT_AUDIO_CODEC_QOS_PREF(true, BT_GAP_LE_PHY_2M, 0x02, 10, 10000, 70000, 10000, 70000);

static uint8_t unicast_server_addata[] = {
	BT_UUID_16_ENCODE(BT_UUID_ASCS_VAL), /* ASCS UUID */
	BT_AUDIO_UNICAST_ANNOUNCEMENT_TARGETED, /* Target Announcement */
	BT_BYTES_LIST_LE16(AVAILABLE_SINK_CONTEXT),
	BT_BYTES_LIST_LE16(AVAILABLE_SOURCE_CONTEXT),
	0x00, /* Metadata length */
};

/* TODO: Expand with BAP data */
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_GAP_APPEARANCE, BT_UUID_16_ENCODE(CONFIG_BT_DEVICE_APPEARANCE)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_ASCS_VAL), BT_UUID_16_ENCODE(BT_UUID_PACS_VAL), BT_UUID_16_ENCODE(BT_UUID_VCS_VAL)),
	BT_DATA(BT_DATA_SVC_DATA16, unicast_server_addata, ARRAY_SIZE(unicast_server_addata)),
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME,
		sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

/* Audio Sink parameters. */
#define MAX_AUDIO_SAMPLE_RATE		48000
#define MAX_AUDIO_CHANNEL_COUNT		2
#define MAX_AUDIO_BYTES_PER_SAMPLE 	2

#define PCM_BUFF_COUNT 			10
#define PCM_AUDIO_BUFF_SIZE		(MAX_AUDIO_SAMPLE_RATE / 100 * MAX_AUDIO_CHANNEL_COUNT * MAX_AUDIO_BYTES_PER_SAMPLE)

#if defined(LE_AUDIO_SYNC_ENABLE) && (LE_AUDIO_SYNC_ENABLE > 0)
#else
static uint8_t audio_i2s_buff[PCM_AUDIO_BUFF_SIZE];
#endif
static bool audio_codec_initialized = false;
static bool audio_sync_initialized = false;

#if defined(LE_AUDIO_SYNC_ENABLE) && (LE_AUDIO_SYNC_ENABLE > 0)
#include "le_audio_sync.h"
static frame_packet_t frame;
#endif

static lc3_codec_info_t lc3_codec_info;

static OSA_SEMAPHORE_HANDLE_DEFINE(sem_disconnected);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_security_changed);
static OSA_SEMAPHORE_HANDLE_DEFINE(sem_mcs_server_discovered);


extern void umr_to_bms_vol_control(uint8_t volume, uint8_t mute);
extern int umr_to_bms_audio_codec_cfg(struct bt_audio_codec_cfg *cfg);
extern int umr_to_bms_audio_codec_qos(struct bt_audio_codec_qos *qos);
extern int umr_to_bms_audio_enable(void);
extern int umr_to_bms_audio_start(void);
extern int umr_to_bms_audio_stop(void);
extern int umr_to_bms_audio_disable(void);
extern int umr_to_bms_audio_release(void);
extern int umr_to_bms_stream_recv(enum bt_audio_location chan_allocation, const struct bt_iso_recv_info *info, struct net_buf *buf);

static int get_channel_count_from_allocation(uint32_t allocation)
{
	int count = 0;
	for (int i = 0; i < 32; i++)
	{
		if(allocation & (1U<<i))
		{
			count++;
			allocation &= ~(1U<<i);
		}
		if(!allocation)
		{
			break;
		}
	}
	return count;
}

void print_hex(const uint8_t *ptr, size_t len)
{
	while (len-- != 0) {
		printk("UMR: %02x", *ptr++);
	}
}

static bool print_cb(struct bt_data *data, void *user_data)
{
	const char *str = (const char *)user_data;

	printk("UMR: %s: type 0x%02x value_len %u\r\n", str, data->type, data->data_len);
	print_hex(data->data, data->data_len);
	printk("UMR: \r\n");

	return true;
}

static void print_codec_cfg(const struct bt_audio_codec_cfg *codec_cfg)
{
	printk("UMR: codec_cfg 0x%02x cid 0x%04x vid 0x%04x count %u\r\n", codec_cfg->id, codec_cfg->cid,
	       codec_cfg->vid, codec_cfg->data_len);

	if (codec_cfg->id == BT_HCI_CODING_FORMAT_LC3) {
		enum bt_audio_location chan_allocation;
		int ret;

		/* LC3 uses the generic LTV format - other codecs might do as well */

		bt_audio_data_parse(codec_cfg->data, codec_cfg->data_len, print_cb, "data");

		ret = bt_audio_codec_cfg_get_freq(codec_cfg);
		if (ret > 0) {
			printk("UMR:   Frequency: %d Hz\r\n", bt_audio_codec_cfg_freq_to_freq_hz((enum bt_audio_codec_cfg_freq)ret));
		}

                ret = bt_audio_codec_cfg_get_frame_dur(codec_cfg);
		printk("UMR:   Frame Duration: %d us\r\n",
		       bt_audio_codec_cfg_frame_dur_to_frame_dur_us((enum bt_audio_codec_cfg_frame_dur)ret));
		if (bt_audio_codec_cfg_get_chan_allocation(codec_cfg, &chan_allocation, false) == 0) {
			printk("UMR:   Channel allocation: 0x%x\r\n", chan_allocation);
		}

		printk("UMR:   Octets per frame: %d (negative means value not pressent)\r\n",
		       bt_audio_codec_cfg_get_octets_per_frame(codec_cfg));
		printk("UMR:   Frames per SDU: %d\r\n",
		       bt_audio_codec_cfg_get_frame_blocks_per_sdu(codec_cfg, true));
	} else {
		print_hex(codec_cfg->data, codec_cfg->data_len);
	}

	bt_audio_data_parse(codec_cfg->meta, codec_cfg->meta_len, print_cb, "meta");
}

static void print_qos(const struct bt_audio_codec_qos *qos)
{
	printk("UMR: QoS: interval %u framing 0x%02x phy 0x%02x sdu %u "
	       "rtn %u latency %u pd %u\r\n",
	       qos->interval, qos->framing, qos->phy, qos->sdu,
	       qos->rtn, qos->latency, qos->pd);
}

static enum bt_audio_dir stream_dir(const struct bt_bap_stream *stream)
{
	for (size_t i = 0U; i < ARRAY_SIZE(sink_streams); i++) {
		if (stream == &sink_streams[i]) {
			return BT_AUDIO_DIR_SINK;
		}
	}

	__ASSERT(false, "Invalid stream %p", stream);
	return (enum bt_audio_dir)0;
}

static struct bt_bap_stream *stream_alloc(void)
{
	for (size_t i = 0; i < ARRAY_SIZE(sink_streams); i++) {
		struct bt_bap_stream *stream = &sink_streams[i];

		if (!stream->conn) {
			return stream;
		}
	}

	return NULL;
}

static int lc3_config(struct bt_conn *conn, const struct bt_bap_ep *ep, enum bt_audio_dir dir,
		      const struct bt_audio_codec_cfg *codec_cfg, struct bt_bap_stream **stream,
		      struct bt_audio_codec_qos_pref *const pref, struct bt_bap_ascs_rsp *rsp)
{
	printk("UMR: ASE Codec Config: conn %p ep %p dir %u\r\n", conn, ep, dir);

	print_codec_cfg(codec_cfg);

	*stream = stream_alloc();
	if (*stream == NULL) {
		printk("UMR: No streams available\r\n");
		*rsp = BT_BAP_ASCS_RSP(BT_BAP_ASCS_RSP_CODE_NO_MEM, BT_BAP_ASCS_REASON_NONE);

		return -ENOMEM;
	}

	printk("UMR: ASE Codec Config stream %p\r\n", *stream);

	*pref = qos_pref;

	return 0;
}

static int lc3_reconfig(struct bt_bap_stream *stream, enum bt_audio_dir dir,
			const struct bt_audio_codec_cfg *codec_cfg,
			struct bt_audio_codec_qos_pref *const pref, struct bt_bap_ascs_rsp *rsp)
{
	printk("UMR: ASE Codec Reconfig: stream %p\r\n", stream);

	print_codec_cfg(codec_cfg);


	*rsp = BT_BAP_ASCS_RSP(BT_BAP_ASCS_RSP_CODE_CONF_UNSUPPORTED, BT_BAP_ASCS_REASON_NONE);

	/* We only support one QoS at the moment, reject changes */
	return -ENOEXEC;
}

static int lc3_qos(struct bt_bap_stream *stream, const struct bt_audio_codec_qos *qos,
		   struct bt_bap_ascs_rsp *rsp)
{
	printk("UMR: QoS: stream %p qos %p\r\n", stream, qos);

	print_qos(qos);

	return 0;
}

static int lc3_enable(struct bt_bap_stream *stream, const uint8_t meta[], size_t meta_len,
		      struct bt_bap_ascs_rsp *rsp)
{
	printk("UMR: Enable: stream %p meta_len %zu\r\n", stream, meta_len);

	umr_to_bms_audio_codec_cfg(stream->codec_cfg);
	umr_to_bms_audio_codec_qos(stream->qos);

	umr_to_bms_audio_enable();
	return 0;
}

static int lc3_start(struct bt_bap_stream *stream, struct bt_bap_ascs_rsp *rsp)
{
	printk("UMR: Start: stream %p\r\n", stream);

	return 0;
}

static bool data_func_cb(struct bt_data *data, void *user_data)
{
	struct bt_bap_ascs_rsp *rsp = (struct bt_bap_ascs_rsp *)user_data;

	if (!BT_AUDIO_METADATA_TYPE_IS_KNOWN((int)data->type)) {
		printk("UMR: Invalid metadata type %u or length %u\r\n", data->type, data->data_len);
		*rsp = BT_BAP_ASCS_RSP(BT_BAP_ASCS_RSP_CODE_METADATA_REJECTED, (enum bt_bap_ascs_reason)data->type);

		return -EINVAL;
	}

	return true;
}

static int lc3_metadata(struct bt_bap_stream *stream, const uint8_t meta[], size_t meta_len,
			struct bt_bap_ascs_rsp *rsp)
{
	printk("UMR: Metadata: stream %p meta_len %zu\r\n", stream, meta_len);

	return bt_audio_data_parse(meta, meta_len, data_func_cb, rsp);
}

static int lc3_disable(struct bt_bap_stream *stream, struct bt_bap_ascs_rsp *rsp)
{
	printk("UMR: Disable: stream %p\r\n", stream);

	return 0;
}

static int lc3_stop(struct bt_bap_stream *stream, struct bt_bap_ascs_rsp *rsp)
{
	printk("UMR: Stop: stream %p\r\n", stream);

	return 0;
}

static int lc3_release(struct bt_bap_stream *stream, struct bt_bap_ascs_rsp *rsp)
{
	printk("UMR: Release: stream %p\r\n", stream);

	return 0;
}

static const struct bt_bap_unicast_server_cb unicast_server_cb = {
	.config = lc3_config,
	.reconfig = lc3_reconfig,
	.qos = lc3_qos,
	.enable = lc3_enable,
	.start = lc3_start,
	.metadata = lc3_metadata,
	.disable = lc3_disable,
	.stop = lc3_stop,
	.release = lc3_release,
};

static void stream_recv_lc3_codec(struct bt_bap_stream *stream,
				  const struct bt_iso_recv_info *info,
				  struct net_buf *buf)
{
	enum bt_audio_location chan_allocation;

	if (bt_audio_codec_cfg_get_chan_allocation(stream->codec_cfg, &chan_allocation, false) == 0) {
		umr_to_bms_stream_recv(chan_allocation, info, buf);
	}
}

static void stream_started(struct bt_bap_stream *stream)
{
	printk("UMR: Stream %p started\r\n", stream);
	umr_to_bms_audio_start();
}

static void stream_stopped(struct bt_bap_stream *stream, uint8_t reason)
{
	printk("UMR: Audio Stream %p stopped with reason 0x%02X\r\n", stream, reason);
	umr_to_bms_audio_stop();
}

static void stream_enabled(struct bt_bap_stream *stream)
{
	printk("UMR: Stream %p enabled\r\n", stream);
	/* The unicast server is responsible for starting sink ASEs after the
	 * client has enabled them.
	 */
	if (stream_dir(stream) == BT_AUDIO_DIR_SINK) {
		const int err = bt_bap_stream_start(stream);

		if (err != 0) {
			printk("UMR: Failed to start stream %p: %d", stream, err);
		}
	}
}

static void stream_disabled(struct bt_bap_stream *stream)
{
	printk("UMR: Stream %p disabled\r\n", stream);

	umr_to_bms_audio_disable();
}

static void stream_released(struct bt_bap_stream *stream)
{
	printk("UMR: Stream %p release\r\n", stream);

	umr_to_bms_audio_release();
}

static struct bt_bap_stream_ops stream_ops = {
	.recv = stream_recv_lc3_codec,
	.started = stream_started,
	.stopped = stream_stopped,
	.enabled = stream_enabled,
	.disabled = stream_disabled,
	.released = stream_released,
};

static void connected(struct bt_conn *conn, uint8_t err)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (err != 0) {
		printk("UMR: Failed to connect to %s (%u)\r\n", addr, err);

		default_conn = NULL;
		return;
	}

	printk("UMR: Connected: %s\r\n", addr);
	default_conn = bt_conn_ref(conn);

#if CONFIG_BT_SMP
	if (bt_conn_set_security(conn, BT_SECURITY_L2))
	{
		printk("UMR: Failed to set security\r\n");
	}
#endif
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	if (conn != default_conn) {
		return;
	}

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("UMR: Disconnected: %s (reason 0x%02x)\r\n", addr, reason);

	bt_conn_unref(default_conn);
	default_conn = NULL;

	OSA_SemaphorePost(sem_disconnected);
}

#if CONFIG_BT_SMP
static void security_changed(struct bt_conn *conn, bt_security_t level, enum bt_security_err err)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("UMR: Security changed: %s level %u (error %d)\r\n", addr, level, err);

	(void)OSA_SemaphorePost(sem_security_changed);
}

static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("UMR: Passkey for %s: %06u\r\n", addr, passkey);
}

static void auth_cancel(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("UMR: Pairing cancelled: %s\r\n", addr);
}
#endif

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
#if CONFIG_BT_SMP
    .security_changed = security_changed,
#endif
};

#if CONFIG_BT_SMP
static struct bt_conn_auth_cb auth_cb_display = {
    .passkey_display = auth_passkey_display,
    .passkey_entry = NULL,
    .cancel = auth_cancel,
};
#endif

static struct bt_pacs_cap cap_sink = {
	.codec_cap = lc3_codec_cap,
};

static int set_location(void)
{
	int err;
	enum bt_audio_location audio_location = (enum bt_audio_location)(BT_AUDIO_LOCATION_FRONT_LEFT | BT_AUDIO_LOCATION_FRONT_RIGHT);

	if (IS_ENABLED(CONFIG_BT_PAC_SNK_LOC)) {
		err = bt_pacs_set_location(BT_AUDIO_DIR_SINK, audio_location);
		if (err != 0) {
			printk("UMR: Failed to set sink location (err %d)\r\n", err);
			return err;
		}
	}

	printk("UMR: Location successfully set\r\n");

	return 0;
}

static int set_supported_contexts(void)
{
	int err;

	if (IS_ENABLED(CONFIG_BT_PAC_SNK)) {
		err = bt_pacs_set_supported_contexts(BT_AUDIO_DIR_SINK,
						     (enum bt_audio_context)AVAILABLE_SINK_CONTEXT);
		if (err != 0) {
			printk("UMR: Failed to set sink supported contexts (err %d)\r\n",
			       err);

			return err;
		}
	}

	printk("UMR: Supported contexts successfully set\r\n");

	return 0;
}

static int set_available_contexts(void)
{
	int err;

	if (IS_ENABLED(CONFIG_BT_PAC_SNK)) {
		err = bt_pacs_set_available_contexts(BT_AUDIO_DIR_SINK,
						     (enum bt_audio_context)AVAILABLE_SINK_CONTEXT);
		if (err != 0) {
			printk("UMR: Failed to set sink available contexts (err %d)\r\n", err);
			return err;
		}
	}

	printk("UMR: Available contexts successfully set\r\n");
	return 0;
}

#if defined(CONFIG_BT_MCC) && (CONFIG_BT_MCC > 0)

static struct bt_conn *mcs_server_conn = NULL;

static void mcc_discover_mcs_cb(struct bt_conn *conn, int err)
{
	if(err)
	{
		printk("UMR: \nMedia control server is discoverd, %d\r\n", err);
		return;
	}

	OSA_SemaphorePost(sem_mcs_server_discovered);
	mcs_server_conn = conn;

	(void)mcs_server_conn;
}

static void mcc_send_cmd_cb(struct bt_conn *conn, int err, const struct mpl_cmd *cmd)
{
    if(err)
    {
        printk("UMR: \nMedia control command is sent, %d, op %d\r\n", err, (int)cmd->opcode);
    }
}

static void mcc_cmd_ntf_cb(struct bt_conn *conn, int err, const struct mpl_cmd_ntf *ntf)
{
    if(err)
    {
        printk("UMR: \nMedia control command notify, %d, req_op %d, res %d\r\n", err, (int)ntf->requested_opcode, (int)ntf->result_code);
    }
}

int le_audio_mcs_client_init(void)
{
    static struct bt_mcc_cb mcs_client_callback;

    mcs_client_callback.discover_mcs = mcc_discover_mcs_cb;
    mcs_client_callback.send_cmd     = mcc_send_cmd_cb;
    mcs_client_callback.cmd_ntf      = mcc_cmd_ntf_cb;

    return bt_mcc_init(&mcs_client_callback);
}

int le_audio_mcs_discover(struct bt_conn *conn)
{
    return bt_mcc_discover_mcs(conn, true);
}

#endif /* CONFIG_BT_MCC */


static void mcs_server_discover_cb(struct bt_conn *conn)
{
	OSA_SemaphorePost(sem_mcs_server_discovered);
}

static void vcs_vol_rend_state_callback(int err, uint8_t volume, uint8_t mute)
{
	if (err) {
		printk("UMR: \nVCS state callback error: %d\r\n", err);
		return;
	}
	printk("UMR: \nVCS Volume = %d, mute state = %d\r\r\n", volume, mute);

	umr_to_bms_vol_control(volume, mute);
}

static void vcs_vol_rend_flags_callback(int err, uint8_t flags)
{
	if (err) {
		printk("UMR: \nVCS flag callback error: %d\r\n", err);
	}
}

int le_audio_vcs_server_init(void)
{
	int err;
	struct bt_vcp_vol_rend_register_param vcs_param;
	static struct bt_vcp_vol_rend_cb vcs_server_callback;

	vcs_server_callback.state = vcs_vol_rend_state_callback;
	vcs_server_callback.flags = vcs_vol_rend_flags_callback;

	vcs_param.step = 10 * 255 / 100;
	vcs_param.volume = 90 * 255 / 100;
	vcs_param.mute = BT_VCP_STATE_UNMUTED;
	vcs_param.cb = &vcs_server_callback;

	err = bt_vcp_vol_rend_register(&vcs_param);
	if (err) {
		return err;
	}

	return 0;
}

void unicast_media_receiver_task(void *param)
{
	struct bt_le_ext_adv *adv;
	int err;

	(void)OSA_SemaphoreCreate(sem_disconnected, 0);
	(void)OSA_SemaphoreCreate(sem_mcs_server_discovered, 0);
	(void)OSA_SemaphoreCreate(sem_security_changed, 0);

	bt_conn_cb_register(&conn_callbacks);
#if CONFIG_BT_SMP
	bt_conn_auth_cb_register(&auth_cb_display);
#endif

	/* VCS server init. */
	le_audio_vcs_server_init();

	le_audio_mcs_client_init();

	/* Unicast server init. */
	bt_bap_unicast_server_register_cb(&unicast_server_cb);

	bt_pacs_cap_register(BT_AUDIO_DIR_SINK, &cap_sink);

	for (size_t i = 0; i < ARRAY_SIZE(sink_streams); i++) {
		bt_bap_stream_cb_register(&sink_streams[i], &stream_ops);
	}

	err = set_location();
	if (err != 0) {
		while(1);
	}

	err = set_supported_contexts();
	if (err != 0) {
		while(1);
	}

	err = set_available_contexts();
	if (err != 0) {
		while(1);
	}

	/* Create a non-connectable non-scannable advertising set */
	err = bt_le_ext_adv_create(BT_LE_EXT_ADV_CONN, NULL, &adv);
	if (err) {
		printk("UMR: Failed to create advertising set (err %d)\r\n", err);
		while(1);
	}

	err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("UMR: Failed to set advertising data (err %d)\r\n", err);
		while(1);
	}

	while (true) {
		err = bt_le_ext_adv_start(adv, BT_LE_EXT_ADV_START_DEFAULT);
		if (err) {
			printk("UMR: Failed to start advertising set (err %d)\r\n", err);
			break;
		}

		printk("UMR: Advertising successfully started\r\n");

		err = OSA_SemaphoreWait(sem_security_changed, osaWaitForever_c);
		if (err != 0) {
			printk("UMR: failed to take sem_security_changed (err %d)\r\n", err);
			while(1);
		}

		printk("UMR: MCS server discover:\r\n");

		(void)le_audio_mcs_discover(default_conn);

		err = OSA_SemaphoreWait(sem_mcs_server_discovered, osaWaitForever_c);
		if (err != 0) {
			printk("UMR: failed to take sem_mcs_server_discovered (err %d)\r\n", err);
			while(1);
		}
		printk("UMR: MCS server discovered.\r\n");

		err = OSA_SemaphoreWait(sem_disconnected, osaWaitForever_c);
		if (err != 0) {
			printk("UMR: failed to take sem_disconnected (err %d)\r\n", err);
			break;
		}
	}

	while(1);
}
