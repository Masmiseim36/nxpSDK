/* map.h - Bluetooth tester headers */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

#define BTP_MAP_CREATE_CMD 0x01
struct btp_map_create_cmd_cmd {
	bt_addr_t address;
	uint8_t instance_id;
	uint8_t cmd;
	uint8_t flags;
	char name[];
} __packed;

#define BTP_MAP_EXECUTE_CMD 0x02
struct btp_map_execute_cmd_cmd {
} __packed;

#define BTP_MAP_WRITE_HDR 0x03
struct btp_map_write_hdr_cmd {
	uint8_t hdr;
	uint8_t data[];
} __packed;

#define BTP_MAP_WRITE_APP_PARAM 0x04
struct btp_map_write_app_param_cmd {
	uint8_t tag;
	union{
		uint8_t u8;
		uint16_t u16;
		uint32_t u32;
		uint8_t data[0];
	};
} __packed;

#define BTP_MAP_MCE_CONNECT         0x11
#define BTP_MAP_MCE_CONNECT_RFCOMM    0x00
#define BTP_MAP_MCE_CONNECT_L2CAP    0x01
struct btp_map_mce_connect_cmd {
	bt_addr_t address;
	uint8_t instance_id;
	uint8_t mode;
} __packed;

#define BTP_MAP_MCE_DISCONNECT      0x12
struct btp_map_mce_disconnect_cmd {
	bt_addr_t address;
	uint8_t instance_id;
} __packed;

#define BTP_MAP_MCE_SET_NTF_REG     0x13
#define BTP_MAP_MCE_SET_FOLDER     0x14
#define BTP_MAP_MCE_GET_FOLDER_LISTING     0x15
#define BTP_MAP_MCE_GET_MSG_LISTING     0x16
#define BTP_MAP_MCE_GET_MSG 0x17
#define BTP_MAP_MCE_SET_MSG_STATUS 0x18
#define BTP_MAP_MCE_UPDATE_INBOX 0x19
#define BTP_MAP_MCE_PUSH_MSG 0x1A
#define BTP_MAP_MCE_GET_MAS_INST_INFO 0x1B
#define BTP_MAP_MCE_SET_NTF_FILTER     0x1C

#define BTP_MAP_MSE_SEND_EVENT   0x31

/* events */
#define BTP_MAP_EV_SDP_DISCOVER 0x80
struct btp_map_mce_sdp_discover_ev {
	bt_addr_t address;
	uint8_t scn;
	uint16_t psm;
	uint32_t supported_features;
	uint16_t map_version;
	uint8_t mas_instance_id;
} __packed;

#define BTP_MAP_EV_MCE_MAS_CONNECTED		0x81
struct btp_map_mce_mas_connected_ev {
	bt_addr_t address;
	uint16_t psm;
	uint8_t scn;
	uint8_t instance_id;
	uint16_t max_pkt_len;
} __packed;

#define BTP_MAP_EV_MCE_MAS_DISCONNECTED		0x82
struct btp_map_mce_mas_disconnected_ev {
	bt_addr_t address;
	uint16_t psm;
	uint8_t scn;
	uint8_t instance_id;
    uint8_t result;
} __packed;

#define BTP_MAP_EV_MCE_MNS_CONNECTED		0x83
struct btp_map_mce_mns_connected_ev {
	bt_addr_t address;
	uint16_t psm;
	uint8_t scn;
	uint16_t max_pkt_len;
} __packed;

#define BTP_MAP_EV_MCE_MNS_DISCONNECTED		0x84
struct btp_map_mce_mns_disconnected_ev {
	bt_addr_t address;
	uint16_t psm;
	uint8_t scn;
	uint8_t result;
} __packed;

#define BTP_MAP_EV_MCE_SET_NTF_REG		0x85
struct btp_map_mce_set_ntf_reg_ev {
	uint8_t result;
} __packed;

#define BTP_MAP_EV_MCE_SET_FOLDER		0x86
struct btp_map_mce_set_folder_ev {
	uint8_t result;
} __packed;

#define BTP_MAP_EV_MCE_GET_FOLDER_LISTING		0x87
struct btp_map_mce_get_folder_listing_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MCE_GET_MSG_LISTING		0x88
struct btp_map_mce_get_msg_listing_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MCE_GET_MSG		0x89
struct btp_map_mce_get_msg_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MCE_SET_MSG_STATUS		0x8A
struct btp_map_mce_set_msg_status_ev {
	uint8_t result;
} __packed;

#define BTP_MAP_EV_MCE_UPDATE_INBOX		0x8B
struct btp_map_mce_update_inbox_ev {
	uint8_t result;
} __packed;

#define BTP_MAP_EV_MCE_PUSH_MSG		0x8C
struct btp_map_mce_push_msg_ev {
	uint8_t result;
} __packed;

#define BTP_MAP_EV_MCE_GET_MAS_INST_INFO		0x8D
struct btp_map_mce_get_mas_instance_info_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MCE_SET_NTF_FILTER 0x8E
struct btp_map_mce_set_ntf_filter_ev {
	uint8_t result;
} __packed;

#define BTP_MAP_EV_MCE_SEND_EVENT 0x8F
struct btp_map_mce_send_event_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MSE_MAS_CONNECTED        0xA1
struct btp_map_mse_mas_connected_ev {
    bt_addr_t address;
	uint16_t psm;
	uint8_t scn;
	uint8_t instance_id;
    uint16_t max_pkt_len;
} __packed;

#define BTP_MAP_EV_MSE_MAS_DISCONNECTED     0xA2
struct btp_map_mse_mas_disconnected_ev {
    bt_addr_t address;
	uint16_t psm;
	uint8_t scn;
	uint8_t instance_id;
    uint8_t result;
} __packed;

#define BTP_MAP_EV_MSE_MNS_CONNECTED        0xA3
struct btp_map_mse_mns_connected_ev {
    bt_addr_t address;
	uint16_t psm;
	uint8_t scn;
	uint16_t max_pkt_len;
} __packed;

#define BTP_MAP_EV_MSE_MNS_DISCONNECTED     0xA4
struct btp_map_mse_mns_disconnected_ev {
	bt_addr_t address;
	uint16_t psm;
	uint8_t scn;
	uint8_t result;
} __packed;

#define BTP_MAP_EV_MSE_SET_NTF_REG      0xA5
struct btp_map_mse_set_ntf_reg_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MSE_SET_FOLDER       0xA6
struct btp_map_mse_set_folder_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MSE_GET_FOLDER_LISTING       0xA7
struct btp_map_mse_get_folder_listing_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MSE_GET_MSG_LISTING      0xA8
struct btp_map_mse_get_msg_listing_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MSE_GET_MSG      0xA9
struct btp_map_mse_get_msg_ev {
    uint8_t result;
    uint8_t data[];
} __packed;

#define BTP_MAP_EV_MSE_SET_MSG_STATUS       0xAA
struct btp_map_mse_set_msg_status_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MSE_UPDATE_INBOX	 0xAB
struct btp_map_mse_update_inbox_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MSE_PUSH_MSG	 0xAC
struct btp_map_mse_push_msg_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MSE_GET_MAS_INST_INFO		0xAD
struct btp_map_mse_get_mas_instance_info_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MSE_SET_NTF_FILTER 0xAF
struct btp_map_mse_set_ntf_filter_ev {
	uint8_t result;
	uint8_t data[];
} __packed;

#define BTP_MAP_EV_MSE_SEND_EVENT 0xB0
struct btp_map_mse_send_event_ev {
	uint8_t result;
} __packed;

#ifdef __cplusplus
}
#endif