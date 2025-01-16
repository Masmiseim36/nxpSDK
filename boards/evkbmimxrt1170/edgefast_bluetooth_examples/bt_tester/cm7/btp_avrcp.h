/* avrcp.h - Bluetooth tester headers */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

/* AVRCP Service */
#define BTP_AVRCP_CONTROL_CONNECT         0x01
struct btp_avrcp_control_connect_cmd {
	bt_addr_le_t address;
} __packed;

#define BTP_AVRCP_CONTROL_DISCONNECT      0x02
struct btp_avrcp_control_disconnect_cmd {
	bt_addr_le_t address;
} __packed;

#define BTP_AVRCP_BROWSING_CONNECT        0x03
struct btp_avrcp_browsing_connect_cmd {
	bt_addr_le_t address;
} __packed;

#define BTP_AVRCP_BROWSING_DISCONNECT     0x04
struct btp_avrcp_browsing_disconnect_cmd {
	bt_addr_le_t address;
} __packed;

#define BTP_AVRCP_SEND_PASS_THROUGH       0x05
struct btp_avrcp_send_pass_through_cmd {
	bt_addr_le_t address;
	uint8_t op_id;
	uint8_t state_flag;
	uint8_t vendor_op_id;
} __packed;

#define BTP_AVRCP_SEND_VENDOR_DEPENDENT   0x06
struct btp_avrcp_send_vendor_dependent_cmd {
	bt_addr_le_t address;
	uint8_t pdu_id;
	/* For BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION pdu id, param is event_id(uint8_t).
     * For BT_AVRCP_PDU_ID_PLAY_ITEMS, param is scope(uint8_t). */
	uint8_t param[0];
} __packed;

#define BTP_AVRCP_SET_BROWSED_PLAYER      0x07
struct btp_avrcp_set_browsed_player_cmd {
	bt_addr_le_t address;
	uint16_t player_id;
} __packed;

#define BTP_AVRCP_GET_FOLDER_ITEMS        0x08
struct btp_avrcp_get_folder_items_cmd {
	bt_addr_le_t address;
	uint8_t scope;	/* 0x00(Media Player List), 0x01(Media Player Virtual Filesystem), 0x02(Search), 0x03(Now Playing) */
	uint32_t start_item;
	uint32_t end_item;
	uint8_t attr_count;
	/** Appendix E in AVRCP spec
	 *  every attribute id is 4 bytes.
	 */
	uint32_t attr_list[0];
} __packed;

#define BTP_AVRCP_CHANGE_PATH	          0x09
struct btp_avrcp_change_path_cmd {
	bt_addr_le_t address;
	uint8_t direction;
} __packed;

#define BTP_AVRCP_GET_ITEM_ATTRIBUTES	  0x010
struct btp_avrcp_get_item_attrs_cmd {
	bt_addr_le_t address;
	uint8_t scope;
	uint8_t num_of_attr;
	/** Appendix E in AVRCP spec
	 *  every attribute id is 4 bytes.
	 */
	uint32_t attr_list[0];
} __packed;

#define BTP_AVRCP_SEARCH	              0x011
struct btp_avrcp_search_cmd {
	bt_addr_le_t address;
	uint16_t length;
	uint8_t str[0];
} __packed;

#define BTP_AVRCP_GET_TOTAL_NUM_OF_ITEMS  0x012
struct btp_avrcp_get_total_num_of_items_cmd {
	bt_addr_le_t address;
	uint8_t scope;
} __packed;

#define BTP_AVRCP_SEND_UNIT_INFO	      0x013
struct btp_avrcp_send_unit_info_cmd {
	bt_addr_le_t address;
} __packed;

#define BTP_AVRCP_SEND_SUBUNIT_INFO	      0x014
struct btp_avrcp_send_subunit_info_cmd {
	bt_addr_le_t address;
} __packed;

#define BTP_AVRCP_HANDLE_REGISTER_NOTIFICATION_ID  0x015
struct btp_avrcp_handle_register_notification_cmd {
	bt_addr_le_t address;
	uint8_t event_id;
	uint8_t status;
} __packed;

#define BTP_COVER_ART_START_INITIATOR     0x016
struct btp_avrcp_cover_art_initiator_cmd {
        bt_addr_le_t address;
} __packed;

#define BTP_AVRCP_EVENT_VOLUME_CHANGED	  0x80
struct btp_avrcp_volume_changed_ev {
	uint8_t volume;
} __packed;

#ifdef __cplusplus
}
#endif