/** @file
 *  @brief Header for Bluetooth BAP LC3 presets.
 *
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_AUDIO_BAP_LC3_PRESET_
#define ZEPHYR_INCLUDE_BLUETOOTH_AUDIO_BAP_LC3_PRESET_

#include <bluetooth/audio/audio.h>

/** Struct to hold a BAP defined LC3 preset */
struct bt_bap_lc3_preset {
	/** The LC3 Codec */
	struct bt_audio_codec_cfg codec_cfg;
	/** The BAP spec defined QoS values */
	struct bt_audio_codec_qos qos;
};

/** Helper to declare an LC3 preset structure */
#define BT_BAP_LC3_PRESET(_codec, _qos)                                                            \
	{                                                                                          \
		.codec_cfg = _codec, .qos = _qos,                                                  \
	}

/* LC3 Unicast presets defined by table 5.2 in the BAP v1.0 specification */

/**
 *  @brief Helper to declare LC3 Unicast 8_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_8_1_1(_loc, _stream_context)                                     \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_8_1(_loc, _stream_context),                    \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(26u, 2u, 8u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 8_2_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_8_2_1(_loc, _stream_context)                                     \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_8_2(_loc, _stream_context),                    \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(30u, 2u, 10u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 16_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_16_1_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_16_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(30u, 2u, 8u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 16_2_1 codec configuration
 *
 *  Mandatory to support as both unicast client and server
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_16_2_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_16_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(40u, 2u, 10u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 24_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_24_1_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_24_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(45u, 2u, 8u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 24_2_1 codec configuration
 *
 *  Mandatory to support as unicast server
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_24_2_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_24_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(60u, 2u, 10u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 32_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_32_1_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_32_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(60u, 2u, 8u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 32_2_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_32_2_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_32_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(80u, 2u, 10u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 441_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_441_1_1(_loc, _stream_context)                                   \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_441_1(_loc, _stream_context),                  \
			  BT_AUDIO_CODEC_QOS(8163u, BT_AUDIO_CODEC_QOS_FRAMING_FRAMED,             \
					     BT_AUDIO_CODEC_QOS_2M, 97u, 5u, 24u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 441_2_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_441_2_1(_loc, _stream_context)                                   \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_441_2(_loc, _stream_context),                  \
			  BT_AUDIO_CODEC_QOS(10884u, BT_AUDIO_CODEC_QOS_FRAMING_FRAMED,            \
					     BT_AUDIO_CODEC_QOS_2M, 130u, 5u, 31u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 48_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_1_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(75u, 5u, 15u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 48_2_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_2_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(100u, 5u, 20u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 48_3_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_3_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_3(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(90u, 5u, 15u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 48_4_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_4_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_4(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(120u, 5u, 20u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 8_5_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_5_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_5(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(117u, 5u, 15u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 48_6_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_6_1(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_6(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(155u, 5u, 20u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 8_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
/* Following presets are for unicast high reliability audio data */
#define BT_BAP_LC3_UNICAST_PRESET_8_1_2(_loc, _stream_context)                                     \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_8_1(_loc, _stream_context),                    \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(26u, 13u, 75u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 8_2_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_8_2_2(_loc, _stream_context)                                     \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_8_2(_loc, _stream_context),                    \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(30u, 13u, 95u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 16_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_16_1_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_16_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(30u, 13u, 75u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 16_2_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_16_2_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_16_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(40u, 13u, 95u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 24_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_24_1_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_24_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(45u, 13u, 75u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 24_2_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_24_2_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_24_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(60u, 13u, 95u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 32_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_32_1_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_32_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(60u, 13u, 75u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 32_2_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_32_2_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_32_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(80u, 13u, 95u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 441_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_441_1_2(_loc, _stream_context)                                   \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_441_1(_loc, _stream_context),                  \
			  BT_AUDIO_CODEC_QOS(8163u, BT_AUDIO_CODEC_QOS_FRAMING_FRAMED,             \
					     BT_AUDIO_CODEC_QOS_2M, 97u, 13u, 80u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 441_2_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_441_2_2(_loc, _stream_context)                                   \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_441_2(_loc, _stream_context),                  \
			  BT_AUDIO_CODEC_QOS(10884u, BT_AUDIO_CODEC_QOS_FRAMING_FRAMED,            \
					     BT_AUDIO_CODEC_QOS_2M, 130u, 13u, 85u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 48_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_1_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(75u, 13u, 75u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 48_2_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_2_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(100u, 13u, 95u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 48_3_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_3_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_3(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(90u, 13u, 75u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 48_4_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_4_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_4(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(120u, 13u, 100u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 48_5_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_5_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_5(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(117u, 13u, 75u, 40000u))

/**
 *  @brief Helper to declare LC3 Unicast 48_6_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_UNICAST_PRESET_48_6_2(_loc, _stream_context)                                    \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_6(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(155u, 13u, 100u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 8_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
/* LC3 Broadcast presets defined by table 6.4 in the BAP v1.0 specification */
#define BT_BAP_LC3_BROADCAST_PRESET_8_1_1(_loc, _stream_context)                                   \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_8_1(_loc, _stream_context),                    \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(26u, 2u, 8u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 8_2_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_8_2_1(_loc, _stream_context)                                   \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_8_2(_loc, _stream_context),                    \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(30u, 2u, 10u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 16_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_16_1_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_16_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(30u, 2u, 8u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 16_2_1 codec configuration
 *
 *  Mandatory to support as both broadcast source and sink
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_16_2_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_16_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(40u, 2u, 10u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 24_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_24_1_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_24_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(45u, 2u, 8u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 24_2_1 codec configuration
 *
 *  Mandatory to support as broadcast sink
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_24_2_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_24_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(60u, 2u, 10u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 32_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_32_1_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_32_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(60u, 2u, 8u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 32_2_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_32_2_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_32_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(80u, 2u, 10u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 441_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_441_1_1(_loc, _stream_context)                                 \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_441_1(_loc, _stream_context),                  \
			  BT_AUDIO_CODEC_QOS(8163u, BT_AUDIO_CODEC_QOS_FRAMING_FRAMED,             \
					     BT_AUDIO_CODEC_QOS_2M, 97u, 4u, 24u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 441_2_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_441_2_1(_loc, _stream_context)                                 \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_441_2(_loc, _stream_context),                  \
			  BT_AUDIO_CODEC_QOS(10884u, BT_AUDIO_CODEC_QOS_FRAMING_FRAMED,            \
					     BT_AUDIO_CODEC_QOS_2M, 130u, 4u, 31u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_1_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_1_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(75u, 4u, 15u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_2_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_2_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(100u, 4u, 20u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_3_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_3_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_3(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(90u, 4u, 15u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_4_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_4_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_4(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(120u, 4u, 20u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_5_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_5_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_5(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(117u, 4u, 15u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_6_1 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_6_1(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_6(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(155u, 4u, 20u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 8_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
/* Following presets are for broadcast high reliability audio data */
#define BT_BAP_LC3_BROADCAST_PRESET_8_1_2(_loc, _stream_context)                                   \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_8_1(_loc, _stream_context),                    \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(26u, 4u, 45u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 8_2_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_8_2_2(_loc, _stream_context)                                   \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_8_2(_loc, _stream_context),                    \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(30u, 4u, 60u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 16_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_16_1_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_16_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(30u, 4u, 45u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 16_2_2 codec configuration
 *
 *  Mandatory to support as both broadcast source and sink
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_16_2_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_16_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(40u, 4u, 60u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 24_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_24_1_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_24_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(45u, 4u, 45u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 24_2_2 codec configuration
 *
 *  Mandatory to support as broadcast sink
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_24_2_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_24_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(60u, 4u, 60u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 32_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_32_1_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_32_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(60u, 4u, 45u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 32_2_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_32_2_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_32_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(80u, 4u, 60u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 441_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_441_1_2(_loc, _stream_context)                                 \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_441_1(_loc, _stream_context),                  \
			  BT_AUDIO_CODEC_QOS(8163u, BT_AUDIO_CODEC_QOS_FRAMING_FRAMED,             \
					     BT_AUDIO_CODEC_QOS_2M, 97u, 4u, 54u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 441_2_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_441_2_2(_loc, _stream_context)                                 \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_441_2(_loc, _stream_context),                  \
			  BT_AUDIO_CODEC_QOS(10884u, BT_AUDIO_CODEC_QOS_FRAMING_FRAMED,            \
					     BT_AUDIO_CODEC_QOS_2M, 130u, 4u, 60u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_1_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_1_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_1(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(75u, 4u, 50u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_2_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_2_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_2(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(100u, 4u, 65u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_3_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_3_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_3(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(90u, 4u, 50u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_4_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_4_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_4(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(120u, 4u, 65u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_5_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_5_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_5(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_7_5_UNFRAMED(117u, 4u, 50u, 40000u))

/**
 *  @brief Helper to declare LC3 Broadcast 48_6_2 codec configuration
 *
 *  @param _loc             Audio channel location bitfield (@ref bt_audio_location)
 *  @param _stream_context  Stream context (BT_AUDIO_CONTEXT_*)
 */
#define BT_BAP_LC3_BROADCAST_PRESET_48_6_2(_loc, _stream_context)                                  \
	BT_BAP_LC3_PRESET(BT_AUDIO_CODEC_LC3_CONFIG_48_6(_loc, _stream_context),                   \
			  BT_AUDIO_CODEC_LC3_QOS_10_UNFRAMED(155u, 4u, 65u, 40000u))

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_AUDIO_BAP_LC3_PRESET_ */
