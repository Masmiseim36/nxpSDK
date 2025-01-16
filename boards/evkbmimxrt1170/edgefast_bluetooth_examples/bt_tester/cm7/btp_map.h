/* map.h - Bluetooth tester headers */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

#define BTP_MAP_MCE_DISCONNECT      0x01

#define BTP_MAP_MCE_CONNECT         0x02
struct btp_map_mce_connect_cmd {
	bt_addr_le_t address;
} __packed;

#define BTP_MAP_MCE_SET_NTF_REG     0x03
struct btp_map_mce_set_ntf_reg_cmd {
	uint8_t ntf_status;
} __packed;

#ifdef __cplusplus
}
#endif