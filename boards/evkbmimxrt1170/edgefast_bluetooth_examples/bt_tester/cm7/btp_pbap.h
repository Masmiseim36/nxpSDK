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

#define BTP_PBAP_PCE_CONNECT           0x03
#define BTP_PBAP_PCE_RFCOMM_CONN       0x01
#define BTP_PBAP_PCE_L2CAP_PSM_CONN    0x02
struct btp_pbap_pce_connect_cmd {
	bt_addr_le_t address;
	uint8_t model;
}  __packed;

#define BTP_PBAP_PCE_DISCONNECT        0x04

#define BTP_PBAP_PCE_CONNECT_AUTH      0x05

#define BTP_PBAP_PSE_CONNECT_AUTH      0x06

#define BTP_PBAP_PULL_PHONEBOOK        0x07

#define BTP_PBAP_PCE_NAME                     0x01
#define BTP_PBAP_PCE_APPL_MLC                 0x02
#define BTP_PBAP_PCE_APPL_PROPERTY_SELECTOR   0x03
#define BTP_PBAP_PCE_APPL_RESET_NEW_MISSED_CALLS 0x04
#define BTP_PBAP_PCE_APPL_VCAED_SELECTOR      0x05
#define BTP_PBAP_PCE_APPL_SEARCH_PROPERTY    0x06
#define BTP_PBAP_PCE_APPL_SEARCH_VALUE        0x07
#define BTP_PBAP_PCE_APPL_VCARD_SELELCTOP_OPERATOR  0x08

#define BTP_PBAP_PCE_SET_PATH             0x08
#define BTP_PBAP_PCE_PULL_VCARD_LISTING   0x09
#define BTP_PBAP_PCE_PULL_VCARD_ENTRY     0x10
#define BTP_PBAP_PCE_ABORT                0x11
#define BTP_PBAP_PSE_RESET_DBI            0x12
#define BTP_PBAP_PSE_MODIFY_CONTACT       0x13

#ifdef __cplusplus
}
#endif