/* a2dp.h - Bluetooth tester headers */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

/* A2DP Service */
#define BTP_A2DP_INIT_SEP		    0x01
struct btp_a2dp_init_sep_cmd {
	uint8_t role;
	uint16_t codec_ie;
} __packed;

#define BTP_A2DP_CONFIGURE		    0x02

#define BTP_A2DP_START				0x03

#define BTP_A2DP_CLOSE              0x04

#define BTP_A2DP_SUSPEND			0x05

#define BTP_A2DP_CONNECT            0x06
struct btp_a2dp_connect_cmd {
	bt_addr_le_t address;
} __packed;

#define BTP_A2DP_INIT_DR 			0x07

#define BTP_A2DP_SEND_DR 			0x08

#if 0
#define BTP_A2DP_RESTART		    0x09	/* to do */

/* events */
#define BTP_A2DP_EV_CONNECTED			0x80
struct btp_a2dp_connected_ev {
	uint8_t status;
} __packed;
#endif

#define BTP_A2DP_EV_DISCONNECTED		0x81
struct btp_a2dp_disconnected_ev {
	uint8_t role;
} __packed;

#ifdef __cplusplus
}
#endif