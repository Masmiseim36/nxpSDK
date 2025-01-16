/* bttester.h - Bluetooth tester headers */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 * Copyright (c) 2022 Codecoup
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Core Service */
#define BTP_CORE_READ_SUPPORTED_COMMANDS	0x01
struct btp_core_read_supported_commands_rp {
	uint8_t data[1];
} __packed;

#define BTP_CORE_READ_SUPPORTED_SERVICES	0x02
struct btp_core_read_supported_services_rp {
	uint8_t data[1];
} __packed;

#define BTP_CORE_REGISTER_SERVICE		0x03
struct btp_core_register_service_cmd {
	uint8_t id;
} __packed;

#define BTP_CORE_UNREGISTER_SERVICE		0x04
struct btp_core_unregister_service_cmd {
	uint8_t id;
} __packed;

#define BTP_CORE_RESET_BOARD                0x06

/* events */
#define BTP_CORE_EV_IUT_READY			0x80

#ifdef __cplusplus
}
#endif
