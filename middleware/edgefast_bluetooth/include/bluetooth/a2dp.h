/** @file
 * @brief Advance Audio Distribution Profile header.
 */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_BLUETOOTH_A2DP_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_A2DP_H_

#include <bluetooth/avdtp.h>

/**
 * @brief Advance Audio Distribution Profile (A2DP)
 * @defgroup bt_a2dp Advance Audio Distribution Profile (A2DP)
 * @ingroup bluetooth
 * @{
 */
 
#ifdef __cplusplus
extern "C" {
#endif

/** @brief define the audio endpoint
 *  @param role BT_A2DP_SOURCE or BT_A2DP_SINK.
 *  @param codec value of enum bt_a2dp_codec_id.
 *  @param capability the codec capability.
 *  @param config the default config to configure the peer same codec type endpoint.
 */
#define BT_A2DP_ENDPOINT_INIT(role, codec, capability, config)\
{\
	.codec_id = codec,\
	.info = {.sep = {.media_type = BT_A2DP_AUDIO, .tsep = role}, .next = NULL},\
	.preset = (struct bt_a2dp_preset *)(config),\
	.capabilities = (struct bt_a2dp_preset *)(capability),\
}

/** @brief define the audio sink endpoint
 *  @param codec value of enum bt_a2dp_codec_id.
 *  @param capability the codec capability.
 */
#define BT_A2DP_SINK_ENDPOINT_INIT(codec, capability)\
BT_A2DP_ENDPOINT_INIT(BT_A2DP_SINK, codec, capability, NULL)

/** @brief define the audio source endpoint
 *  @param codec value of enum bt_a2dp_codec_id.
 *  @param capability the codec capability.
 *  @param config the default config to configure the peer same codec type endpoint.
 */
#define BT_A2DP_SOURCE_ENDPOINT_INIT(codec, capability, config)\
BT_A2DP_ENDPOINT_INIT(BT_A2DP_SOURCE, codec, capability, config)

/** @brief define the default SBC sink endpoint that can be used as
 * bt_a2dp_register_endpoint's parameter.
 *
 * SBC is mandatory as a2dp specification, BT_A2DP_SBC_SINK_ENDPOINT is more convenient for
 * user to register SBC endpoint.
 *
 *  @param name the endpoint variable name.
 */
#define BT_A2DP_SBC_SINK_ENDPOINT(name)\
static uint8_t bt_a2dp_endpoint_cap_buffer##name[5] =\
{4U, A2DP_SBC_SAMP_FREQ_44100 | A2DP_SBC_SAMP_FREQ_48000 | A2DP_SBC_CH_MODE_MONO | \
A2DP_SBC_CH_MODE_STREO | A2DP_SBC_CH_MODE_JOINT, A2DP_SBC_BLK_LEN_16 | A2DP_SBC_SUBBAND_8 | \
A2DP_SBC_ALLOC_MTHD_LOUDNESS, 18U, 35U};\
static struct bt_a2dp_endpoint name = BT_A2DP_SINK_ENDPOINT_INIT(BT_A2DP_SBC,\
(&bt_a2dp_endpoint_cap_buffer##name[0]))

/** @brief define the default SBC source endpoint that can be used as bt_a2dp_register_endpoint's
 * parameter.
 *
 * SBC is mandatory as a2dp specification, BT_A2DP_SBC_SOURCE_ENDPOINT is more convenient for
 * user to register SBC endpoint.
 *
 *  @param name the endpoint variable name.
 *  @param config_freq the frequency to configure the peer same codec type endpoint.
 */
#define BT_A2DP_SBC_SOURCE_ENDPOINT(name, config_freq)\
static uint8_t bt_a2dp_endpoint_cap_buffer##name[5] = {4U, A2DP_SBC_SAMP_FREQ_44100 | \
A2DP_SBC_SAMP_FREQ_48000 | A2DP_SBC_CH_MODE_MONO | A2DP_SBC_CH_MODE_STREO | \
A2DP_SBC_CH_MODE_JOINT, A2DP_SBC_BLK_LEN_16 | A2DP_SBC_SUBBAND_8 | A2DP_SBC_ALLOC_MTHD_LOUDNESS,\
18U, 35U};\
static uint8_t bt_a2dp_endpoint_preset_buffer##name[5] =\
{4U, config_freq | A2DP_SBC_CH_MODE_JOINT, A2DP_SBC_BLK_LEN_16 | A2DP_SBC_SUBBAND_8 |\
A2DP_SBC_ALLOC_MTHD_LOUDNESS, 18U, 35U};\
static struct bt_a2dp_endpoint name = BT_A2DP_SOURCE_ENDPOINT_INIT(BT_A2DP_SBC,\
&bt_a2dp_endpoint_cap_buffer##name[0], &bt_a2dp_endpoint_preset_buffer##name[0])

/** @brief Codec ID */
enum bt_a2dp_codec_id {
	/** Codec SBC */
	BT_A2DP_SBC = 0x00,
	/** Codec MPEG-1 */
	BT_A2DP_MPEG1 = 0x01,
	/** Codec MPEG-2 */
	BT_A2DP_MPEG2 = 0x02,
	/** Codec ATRAC */
	BT_A2DP_ATRAC = 0x04,
	/** Codec Non-A2DP */
	BT_A2DP_VENDOR = 0xff
};

/** @brief Preset for the endpoint */
struct bt_a2dp_preset {
	/** Length of preset */
	uint8_t len;
	/** Preset */
	uint8_t preset[0];
};

/** @brief Stream End Point */
struct bt_a2dp_endpoint {
	/** Code ID */
	uint8_t codec_id;
	/** Stream End Point Information */
	struct bt_avdtp_seid_lsep info;
	/** Pointer to preset codec chosen */
	struct bt_a2dp_preset *preset;
	/** Capabilities */
	struct bt_a2dp_preset *capabilities;
};

/** @brief Stream End Point Media Type */
enum MEDIA_TYPE {
	/** Audio Media Type */
	BT_A2DP_AUDIO = 0x00,
	/** Video Media Type */
	BT_A2DP_VIDEO = 0x01,
	/** Multimedia Media Type */
	BT_A2DP_MULTIMEDIA = 0x02
};

/** @brief Stream End Point Role */
enum ROLE_TYPE {
	/** Source Role */
	BT_A2DP_SOURCE = 0,
	/** Sink Role */
	BT_A2DP_SINK = 1
};

/** @brief A2DP structure */
struct bt_a2dp;

/** @brief The configuration result */
struct a2dp_configure_result {
	/** 0 - success; other values - fail code */
	int err;
	/** The endpoint that is configured, it is registered by bt_a2dp_register_endpoint */
	struct bt_a2dp_endpoint *endpoint;
	/** The configuration content */
	struct bt_a2dp_preset *config;
};

/** @brief The media streaming is started
 *
 *  @param a2dp a2dp connection object.
 *  @param err error code.
 */
/** @brief The start control's callback */
typedef void (*bt_a2dp_start_cb_t)(struct bt_a2dp *a2dp, int err);

/** @brief the media streaming is suspended
 *
 *  @param a2dp a2dp connection object.
 *  @param err error code.
 */
typedef void (*bt_a2dp_suspend_cb_t)(struct bt_a2dp *a2dp, int err);

/** @brief A a2dp has been configured.
 *
 *  This callback notifies the application of a2dp configuring result.
 *
 *  @param a2dp a2dp connection object.
 *  @param config configuring result.
 */
typedef void (*bt_a2dp_configure_cb_t)(struct bt_a2dp *a2dp, struct a2dp_configure_result *config);

/** @brief Revert the configuration and put the a2dp in unconfiguraion state, then can configure
 * again.
 */
typedef void (*bt_a2dp_deconfigure_cb_t)(struct bt_a2dp *a2dp, int err);

/** @brief Helper enum to be used as return value of bt_a2dp_discover_peer_endpoint_cb_t.
 *  The value informs the caller to perform further pending actions or stop them.
 */
enum {
	BT_A2DP_DISCOVER_ENDPOINT_STOP = 0,
	BT_A2DP_DISCOVER_ENDPOINT_CONTINUE,
};

/** @brief Get peer's endpoints callback */
typedef uint8_t (*bt_a2dp_discover_peer_endpoint_cb_t)(struct bt_a2dp *a2dp,
							struct bt_a2dp_endpoint *endpoint, int err);

/** @brief The callback that is controlled by peer */
struct bt_a2dp_control_cb {
	/** @brief a2dp is configured by peer, usual for sink.
	 *
	 *  @param a2dp a2dp connection object.
	 *  @param err a2dp configuration result.
	 */
	void (*configured)(struct bt_a2dp *a2dp, struct a2dp_configure_result *config);
#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
	/** @brief The media streaming is started, only for sink.*/
	bt_a2dp_start_cb_t sink_start_play;
	/** @brief the media streaming is suspended, only for sink.*/
	bt_a2dp_suspend_cb_t sink_suspend_play;
	/** @brief the media streaming data, only for sink.
	 *
	 *  @param a2dp a2dp connection object.
	 *  @param err error code.
	 */
	void (*sink_streamer_data)(struct bt_a2dp *a2dp, uint8_t *data, uint32_t length);
#endif
};

/** @brief The connecting callback */
struct bt_a2dp_connect_cb {
	/** @brief A a2dp connection has been established.
	 *
	 *  This callback notifies the application of a a2dp connection.
	 *  It means the AVDTP L2CAP connection.
	 *  In case the err parameter is non-zero it means that the
	 *  connection establishment failed.
	 *
	 *  @param a2dp a2dp connection object.
	 *  @param err error code.
	 */
	void (*connected)(struct bt_a2dp *a2dp, int err);
	/** @brief A a2dp connection has been disconnected.
	 *
	 *  This callback notifies the application that a a2dp connection
	 *  has been disconnected.
	 *
	 *  @param a2dp a2dp connection object.
	 */
	void (*disconnected)(struct bt_a2dp *a2dp);
};

/** @brief A2DP Connect.
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to establish A2DP
 *  connection between devices.
 *  This funciton only establish AVDTP L2CAP connection.
 *  After connection success, the callback that is registered by
 *  bt_a2dp_register_connect_callback is called.
 *
 *  @param conn Pointer to bt_conn structure.
 *
 *  @return pointer to struct bt_a2dp in case of success or NULL in case
 *  of error.
 */
struct bt_a2dp *bt_a2dp_connect(struct bt_conn *conn);

/** @brief Endpoint Registration.
 *
 *  This function is used for registering the stream end points. The user has
 *  to take care of allocating the memory, the preset pointer and then pass the
 *  required arguments. Also, only one sep can be registered at a time.
 *  Multiple stream end points can be registered by calling multiple times.
 *  The endpoint registered first has a higher priority than the endpoint registered later.
 *  The priority is used in bt_a2dp_configure.
 *
 *  @param endpoint Pointer to bt_a2dp_endpoint structure.
 *  @param media_type Media type that the Endpoint is.
 *  @param role Role of Endpoint.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_register_endpoint(struct bt_a2dp_endpoint *endpoint,
				  uint8_t media_type, uint8_t role);

/** @brief register connecting callback.
 *
 *  The cb is called when bt_a2dp_connect is called or it is connected by peer device.
 *
 *  @param cb The callback function.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_register_connect_callback(struct bt_a2dp_connect_cb *cb);

/** @brief register control callback.
 *
 *  The cb is called when receiving control from peer.
 *
 *  @param a2dp The a2dp instance.
 *  @param cb The callback function.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_register_control_callback(struct bt_a2dp *a2dp, struct bt_a2dp_control_cb *cb);

/** @brief configure control callback.
 *
 *  This function will get peer's all endpoints and select one endpoint
 *  based on the priority of registered endpoints,
 *  then configure the endpoint based on the preset of endpoint.
 *  Note: (1) priority is described in bt_a2dp_register_endpoint;
 *        (2) preset is the preset field of struct bt_a2dp_endpoint that is registered by
 *            bt_a2dp_register_endpoint.
 *
 *  @param a2dp The a2dp instance.
 *  @param cb The callback function.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_configure(struct bt_a2dp *a2dp, bt_a2dp_configure_cb_t cb);

/** @brief get peer's endpoints.
 *
 *  bt_a2dp_configure can be called to configure a2dp.
 *  bt_a2dp_discover_peer_endpoints and bt_a2dp_configure_endpoint can be used too.
 *  In bt_a2dp_configure, the endpoint is selected automatically based on the
 *  prioriy. If bt_a2dp_configure fails, it means the default preset of endpoint
 *  is not reasonal. bt_a2dp_discover_peer_endpoints and bt_a2dp_configure_endpoint
 *  can be used.
 *  bt_a2dp_discover_peer_endpoints is used to get peer endpoints. the peer endpoint
 *  is returned in the cb. then endpoint can be selected and configufed by
 *  bt_a2dp_configure_endpoint. If user stops to discover more peer endpoins, return
 *  BT_A2DP_DISCOVER_ENDPOINT_STOP in the cb; if user wants to discover more peer endpoints,
 *  return BT_A2DP_DISCOVER_ENDPOINT_CONTINUE in the cb.
 *
 *  @param a2dp The a2dp instance.
 *  @param cb notify the result.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_discover_peer_endpoints(struct bt_a2dp *a2dp, bt_a2dp_discover_peer_endpoint_cb_t cb);

/** @brief get peer's endpoints.
 *
 *  If the bt_a2dp_configure is failed or user want to change configured endpoint,
 *  user can call bt_a2dp_discover_peer_endpoints and this function to configure
 *  the selected endpoint.
 *
 *  @param a2dp The a2dp instance.
 *  @param endpoint The configured endpoint that is registered.
 *  @param config The config to configure the endpoint.
 *  @param cb The callback function.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_configure_endpoint(struct bt_a2dp *a2dp, struct bt_a2dp_endpoint *endpoint,
		struct bt_a2dp_preset *config, bt_a2dp_configure_cb_t cb);

/** @brief revert the configuration, then it can be configured again.
 *
 *  After bt_a2dp_deconfigure succeed, bt_a2dp_configure or
 *  bt_a2dp_configure_endpoint can be used to configure peer again.
 *
 *  @param a2dp The a2dp instance.
 *  @param cb The callback function.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_deconfigure(struct bt_a2dp *a2dp, bt_a2dp_deconfigure_cb_t cb);

#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
/** @brief start a2dp streamer, it is source only.
 *
 *  @param a2dp The a2dp instance.
 *  @param cb The callback function.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_src_start(struct bt_a2dp *a2dp, bt_a2dp_start_cb_t cb);

/** @brief suspend a2dp streamer, it is source only.
 *
 *  @param a2dp The a2dp instance.
 *  @param cb The callback function.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_src_suspend(struct bt_a2dp *a2dp, bt_a2dp_suspend_cb_t cb);
#endif

/** @brief get the peer's endpoint information that is configured.
 *
 * The peer endpoint that is configured by bt_a2dp_configure or
 * bt_a2dp_configure_endpoint.
 *
 *  @return the peer endpoint that is configured or NULL
 */
struct bt_a2dp_endpoint *bt_a2dp_get_configured_peer_endpoint(struct bt_a2dp *a2dp);

/** @brief re-configure a2dp streamer
 *
 * This function send the AVDTP_RECONFIGURE command
 *
 *  @param a2dp The a2dp instance.
 *  @param config The config to configure the endpoint.
 *  @param cb The callback function.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_reconfigure(struct bt_a2dp *a2dp, struct bt_a2dp_preset *config,
		bt_a2dp_configure_cb_t cb);

#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
/** @brief send a2dp streamer data
 *
 *  @param a2dp The a2dp instance.
 *  @param data The streamer data.
 *  @param datalen The streamer data length.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_src_media_write(struct bt_a2dp *a2dp, uint8_t *data, uint16_t datalen);
#endif

#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
/** @brief notify the streamer data is consumed.
 *
 *  The streamer data is received in sink_streamer_data callback
 *
 *  @param a2dp The a2dp instance.
 *  @param data The streamer data.
 *  @param datalen The streamer data length.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_a2dp_snk_media_sync(struct bt_a2dp *a2dp, uint8_t *data, uint16_t datalen);
#endif

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
 
#endif /* ZEPHYR_INCLUDE_BLUETOOTH_A2DP_H_ */
