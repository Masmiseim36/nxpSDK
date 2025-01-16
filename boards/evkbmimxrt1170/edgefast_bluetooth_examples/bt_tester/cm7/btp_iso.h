/* btp_ias.h - Bluetooth tester headers */

/*
 * Copyright (c) 2022 Codecoup
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

/* GAP Service */
/* commands */
#define BTP_ISO_READ_SUPPORTED_COMMANDS		0x01
struct btp_iso_read_supported_commands_rp {
	uint8_t data[1];
} __packed;

#define BTP_ISO_BIG_CREATE			0x02
struct btp_iso_big_create_cmd {
	uint8_t adv_index;
	uint8_t encrypt;
	uint8_t bcode[BT_ISO_BROADCAST_CODE_SIZE];
} __packed;

#define BTP_ISO_BIG_SYNC			0x03
struct btp_iso_big_sync_cmd {
	bt_addr_le_t address;
	uint8_t sid;
	uint8_t bitfield;
	uint8_t encrypt;
	uint8_t bcode[BT_ISO_BROADCAST_CODE_SIZE];
} __packed;

#define BTP_ISO_BIG_TERM			0x04

#define BTP_ISO_BIS_SEND			0x05
struct btp_iso_big_send_cmd {
	uint8_t chan_id;
	uint16_t data_len;
	uint8_t data[];
} __packed;

/* events */
#define BTP_ISO_EV_BIS_CONNECTED		0x81
struct btp_iso_big_connected_ev {
	uint8_t chan_id;
} __packed;

#define BTP_ISO_EV_BIS_DISCONNECTED		0x82
struct btp_iso_big_disconnected_ev {
	uint8_t chan_id;
} __packed;

#define BTP_ISO_EV_BIS_DATA_RECEIVED		0x83
struct btp_iso_big_data_received_ev {
	uint8_t chan_id;
	uint16_t data_length;
	uint8_t data[];
} __packed;

#ifdef __cplusplus
}
#endif