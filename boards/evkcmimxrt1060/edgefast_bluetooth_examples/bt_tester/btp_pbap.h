/* btp_pbap.h - Bluetooth tester headers */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

#define BTP_PBAP_READ_SUPPORTED_COMMANDS  0x1

struct btp_pbap_read_supported_commands_rp {
	uint8_t data[1];
}  __packed;

#define BTP_PBAP_MODIFY_VERSION         0x2

struct btp_pbap_modify_version_req {
	uint8_t data[1];
}  __packed;

#define BTP_PBAP_PCE_CONNECT             0x03
#define BTP_PBAP_PCE_RFCOMM_CONN         0x01
#define BTP_PBAP_PCE_L2CAP_PSM_CONN      0x02
struct btp_pbap_pce_connect_cmd {
	bt_addr_le_t address;
	uint8_t model;
	bool is_auth;
}  __packed;

#define BTP_PBAP_PCE_DISCONNECT          0x04

#define BTP_PBAP_PCE_CREATE_CMD          0x05
/* Parameter of pbap_create_cmd cmd */
#define BTP_PBAP_PCE_PULL_PHONEBOOK      0x01
#define BTP_PBAP_PCE_PULL_VCARD_LISTING  0x02
#define BTP_PBAP_PCE_PULL_VCARD_ENTRY    0x03
/* Parameter of pbap_create_cmd flag */ 
#define BTP_PBAP_REQ_START               0x01
#define BTP_PBAP_REQ_CONTINUE            0x02
#define BTP_PBAP_REQ_END                 0x04
#define BTP_PBAP_REQ_UNSEG               0x05
struct btp_pbap_pce_create_cmd {
	uint8_t cmd;
	uint8_t flags;
	uint8_t srmp_wait;
	char name[];
} __packed;

struct btp_pbap_pce_set_appl_param{
	uint8_t tag;
	uint8_t value[];
} __packed;
#define PBAP_PCE_EXCUTE_CMD              0x06

#define BTP_PBAP_PCE_SET_PATH            0x12
#define BTP_PBAP_PCE_ABORT               0x13
#define BTP_PBAP_PSE_CONNECT_AUTH        0x16
#define BTP_PBAP_PSE_RESET_DBI           0x17
#define BTP_PBAP_PSE_MODIFY_CONTACT      0x18
#define BTP_PBAP_PSE_RESPONSE_CODE       0x19
#define BTP_PBAP_PCE_SET_APPL_PARAM      0x24

#define PBAP_EV_PCE_PULL_PHONEBOOK       0x81
#define PBAP_EV_PCE_PULL_VCARD_LISTING   0x82
#define PBAP_EV_PCE_PULL_VCARD_ENTRY     0x83

#ifdef __cplusplus
}
#endif