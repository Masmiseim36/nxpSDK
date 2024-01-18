/* @file
 * @brief Internal APIs for PACS handling
 *
 * Copyright (c) 2020 Intel Corporation
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <bluetooth/audio/audio.h>

#define BT_AUDIO_LOCATION_MASK BIT_MASK(28)

STRUCT_PACKED_PRE
struct bt_pac_codec {
	uint8_t  id;			/* Codec ID */
	uint16_t cid;			/* Company ID */
	uint16_t vid;			/* Vendor specific Codec ID */
} STRUCT_PACKED_POST;


STRUCT_PACKED_PRE
struct bt_pac_ltv {
	uint8_t  len;
	uint8_t  type;
#if 0 /* walkaround "unknown-size array" compile error. */
	uint8_t  value[0];
#endif
} STRUCT_PACKED_POST;

STRUCT_PACKED_PRE
struct bt_pac_ltv_data {
	uint8_t  len;
	struct bt_pac_ltv data[0];
} STRUCT_PACKED_POST;

STRUCT_PACKED_PRE
struct bt_pacs_read_rsp {
	uint8_t  num_pac;		/* Number of PAC Records*/
} STRUCT_PACKED_POST;

STRUCT_PACKED_PRE
struct bt_pacs_context {
	uint16_t  snk;
	uint16_t  src;
} STRUCT_PACKED_POST;

bool bt_pacs_context_available(enum bt_audio_dir dir, uint16_t context);
