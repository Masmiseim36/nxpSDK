/* btp_vcs.h - Bluetooth tester headers */

/*
 * Copyright (c) 2022 Codecoup
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#ifdef __cplusplus
extern "C" {
#endif

#include <sys/util.h>

#define BTP_VCS_READ_SUPPORTED_COMMANDS		0x01
struct btp_vcs_read_supported_commands_rp {
	uint8_t data[1];
} __packed;

#define BTP_VCS_SET_VOL				0x02
struct btp_vcs_set_vol_cmd {
	uint8_t volume;
} __packed;

#define BTP_VCS_VOL_UP				0x03
#define BTP_VCS_VOL_DOWN			0x04
#define BTP_VCS_MUTE				0x05
#define BTP_VCS_UNMUTE				0x06

#ifdef __cplusplus
}
#endif