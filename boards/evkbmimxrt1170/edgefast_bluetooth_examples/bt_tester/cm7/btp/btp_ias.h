/* btp_ias.h - Bluetooth tester headers */

/*
 * Copyright (c) 2022 Codecoup
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <bluetooth/services/ias.h>
#include <stdint.h>

/* events */
#define BTP_IAS_EV_OUT_ALERT_ACTION 0x80
struct btp_ias_alert_action_ev {
	uint8_t alert_lvl;
} __packed;

#ifdef __cplusplus
}
#endif