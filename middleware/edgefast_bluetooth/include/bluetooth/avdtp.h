/** @file
 * @brief Audio/Video Distribution Transport Protocol header.
 */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_BLUETOOTH_AVDTP_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_AVDTP_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @brief AVDTP SEID Information */
struct bt_avdtp_seid_info {
	/** Stream End Point ID */
	uint8_t id:6;
	/** End Point usage status */
	uint8_t inuse:1;
	/** Reserved */
	uint8_t rfa0:1;
	/** Media-type of the End Point */
	uint8_t media_type:4;
	/** TSEP of the End Point */
	uint8_t tsep:1;
	/** Reserved */
	uint8_t rfa1:3;
} __packed;

/** @brief AVDTP Local SEP*/
struct bt_avdtp_seid_lsep {
	/** Stream End Point information */
	struct bt_avdtp_seid_info sep;
	/** Pointer to next local Stream End Point structure */
	struct bt_avdtp_seid_lsep *next;
};

/** @brief AVDTP Stream */
struct bt_avdtp_stream {
	struct bt_l2cap_br_chan chan; /* Transport Channel*/
	struct bt_avdtp_seid_info lsep; /* Configured Local SEP */
	struct bt_avdtp_seid_info rsep; /* Configured Remote SEP*/
	uint8_t state; /* current state of the stream */
	struct bt_avdtp_stream *next;
};

/*
 *  AVDTP Media Packet Header (data is big endian in packet)
 *
 *              7   6   5   4   3   2   1   0
 *  Byte 0:   |     CC        | X | P |  V   |
 *  Byte 1:   |             PT           | M |
 *  Byte 2:   |          Seq No              |
 *  Byte 3:   |          Seq No              |
 *  Byte 4:   |            TS                |
 *  Byte 5:   |            TS                |
 *  Byte 6:   |            TS                |
 *  Byte 7:   |            TS                |
 *  Byte 8:   |           SSRC               |
 *  Byte 9:   |           SSRC               |
 *  Byte10:   |           SSRC               |
 *  Byte11:   |           SSRC               |
 *
 *  V   : Version
 *  P   : Padding
 *  X   : Extension
 *  CC  : CSRC Count (must be 0 in this stack)
 *  M   : Marker
 *  PT  : Payload Type
 *  Seq : Sequence Number
 *  TS  : Time Stamp
 *  SSRC: Synchronization Source
 */
#define BT_AVDTP_MEDIA_HDR_SIZE (12U)

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_AVDTP_H_ */
