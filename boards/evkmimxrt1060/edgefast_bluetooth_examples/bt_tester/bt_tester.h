/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno/errno.h>
#include <porting.h>

#include <bluetooth/bluetooth.h>

#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BTP_MTU                 1024

#define BTP_INDEX_NONE          0xff /* Command or event is not related to any controller*/

#define BTP_SERVICE_ID_CORE     0
#define BTP_SERVICE_ID_GAP      1
#define BTP_SERVICE_ID_GATT     2
#define BTP_SERVICE_ID_L2CAP    3

#define BTP_STATUS_SUCCESS          0x00
#define BTP_STATUS_FAILED           0x01
#define BTP_STATUS_UNKNOWN_CMD      0x02
#define BTP_STATUS_NOT_READY        0x03
#define BTP_STATUS_INVALID_INDEX    0x04

typedef __packed struct btp_hdr_tag {
    uint8_t  service;
    uint8_t  opcode;
    uint8_t  index;
    uint16_t len;
    uint8_t  data[BTP_MTU];
} btp_hdr_t;

#define CMD_BTP_HDR_SIZE    0x05U

#define BTP_DATA_MAX_SIZE   (BTP_MTU - 0x05U)

#define BTP_STATUS          0x00

#define CMD_QUEUED          0x02

typedef __packed struct btp_status_tag {
    uint8_t code;
} btp_status_t;

/* Core Service */
#define CORE_READ_SUPPORTED_COMMANDS    0x01

typedef __packed struct core_read_supported_commands_rp_tag {
    uint8_t rsp;
} core_read_supported_commands_rp_t;

#define CORE_READ_SUPPORTED_SERVICES    0x02
typedef __packed struct core_read_supported_services_rp_tag {
    uint8_t rsp;
} core_read_supported_services_rp_t;

#define CORE_REGISTER_SERVICE           0x03
typedef __packed struct core_register_service_cmd_tag {
    uint8_t id;
} core_register_service_cmd_t;

#define CORE_UNREGISTER_SERVICE         0x04
typedef __packed struct core_unregister_service_cmd_tag {
    uint8_t id;
} core_unregister_service_cmd_t;

#define CORE_RESET_BOARD                0x06

/* events */
#define CORE_EV_IUT_READY               0x80

/* GAP Service */
/* commands */
#define GAP_READ_SUPPORTED_COMMANDS 0x01
typedef __packed struct gap_read_supported_commands_rp {
  uint8_t data;
} gap_read_supported_commands_rp_t;

#define GAP_READ_CONTROLLER_INDEX_LIST  0x02
typedef __packed struct gap_read_controller_index_list_rp {
  uint8_t num;
  uint8_t index;
} gap_read_controller_index_list_rp_t;

#define GAP_SETTINGS_POWERED            0
#define GAP_SETTINGS_CONNECTABLE        1
#define GAP_SETTINGS_FAST_CONNECTABLE   2
#define GAP_SETTINGS_DISCOVERABLE       3
#define GAP_SETTINGS_BONDABLE           4
#define GAP_SETTINGS_LINK_SEC_3         5
#define GAP_SETTINGS_SSP                6
#define GAP_SETTINGS_BREDR              7
#define GAP_SETTINGS_HS                 8
#define GAP_SETTINGS_LE                 9
#define GAP_SETTINGS_ADVERTISING        10
#define GAP_SETTINGS_SC                 11
#define GAP_SETTINGS_DEBUG_KEYS         12
#define GAP_SETTINGS_PRIVACY            13
#define GAP_SETTINGS_CONTROLLER_CONFIG  14
#define GAP_SETTINGS_STATIC_ADDRESS     15

#define GAP_READ_CONTROLLER_INFO  0x03
typedef __packed struct gap_read_controller_info_rp {
  uint8_t  address[6];
  uint32_t supported_settings;
  uint32_t current_settings;
  uint8_t  cod[3];
  uint8_t  name[249];
  uint8_t  short_name[11];
} gap_read_controller_info_rp_t;

#define GAP_RESET     0x04
typedef __packed struct gap_reset_rp {
  uint32_t current_settings;
} gap_reset_rp_t;

#define GAP_SET_POWERED     0x05
typedef __packed struct gap_set_powered_cmd {
  uint8_t powered;
} gap_set_powered_cmd_t;
typedef __packed struct gap_set_powered_rp {
  uint32_t current_settings;
} gap_set_powered_rp_t;

#define GAP_SET_CONNECTABLE   0x06
typedef __packed struct gap_set_connectable_cmd {
  uint8_t connectable;
} gap_set_connectable_cmd_t;
typedef __packed struct gap_set_connectable_rp {
  uint32_t current_settings;
} gap_set_connectable_rp_t;

#define GAP_SET_FAST_CONNECTABLE  0x07
typedef __packed struct gap_set_fast_connectable_cmd {
  uint8_t fast_connectable;
} gap_set_fast_connectable_cmd_t;
typedef __packed struct gap_set_fast_connectable_rp {
  uint32_t current_settings;
} gap_set_fast_connectable_rp_t;

#define GAP_NON_DISCOVERABLE      0x00
#define GAP_GENERAL_DISCOVERABLE  0x01
#define GAP_LIMITED_DISCOVERABLE  0x02

#define GAP_SET_DISCOVERABLE    0x08
typedef __packed struct gap_set_discoverable_cmd {
  uint8_t discoverable;
} gap_set_discoverable_cmd_t;
typedef __packed struct gap_set_discoverable_rp {
  uint32_t current_settings;
} gap_set_discoverable_rp_t;

#define GAP_SET_BONDABLE    0x09
typedef __packed struct gap_set_bondable_cmd {
  uint8_t bondable;
} gap_set_bondable_cmd_t;
typedef __packed struct gap_set_bondable_rp {
  uint32_t current_settings;
} gap_set_bondable_rp_t;

#define GAP_START_ADVERTISING 0x0a
typedef __packed struct gap_start_advertising_cmd {
  uint8_t adv_data_len;
  uint8_t scan_rsp_len;
  uint8_t adv_sr_data[];
} gap_start_advertising_cmd_t;
typedef __packed struct gap_start_advertising_rp {
  uint32_t current_settings;
} gap_start_advertising_rp_t;

#define GAP_STOP_ADVERTISING    0x0b
typedef __packed struct gap_stop_advertising_rp {
  uint32_t current_settings;
} gap_stop_advertising_rp_t;

#define GAP_DISCOVERY_FLAG_LE                0x01
#define GAP_DISCOVERY_FLAG_BREDR             0x02
#define GAP_DISCOVERY_FLAG_LIMITED           0x04
#define GAP_DISCOVERY_FLAG_LE_ACTIVE_SCAN    0x08
#define GAP_DISCOVERY_FLAG_LE_OBSERVE        0x10
#define GAP_DISCOVERY_FLAG_OWN_ID_ADDR       0x20

#define GAP_START_DISCOVERY   0x0c
typedef __packed struct gap_start_discovery_cmd {
  uint8_t flags;
} gap_start_discovery_cmd_t;

#define GAP_STOP_DISCOVERY    0x0d

#define GAP_CONNECT     0x0e
typedef __packed struct gap_connect_cmd {
  uint8_t address_type;
  uint8_t address[6];
} gap_connect_cmd_t;

#define GAP_DISCONNECT      0x0f
typedef __packed struct gap_disconnect_cmd {
  uint8_t  address_type;
  uint8_t  address[6];
} gap_disconnect_cmd_t;

#define GAP_IO_CAP_DISPLAY_ONLY     0
#define GAP_IO_CAP_DISPLAY_YESNO    1
#define GAP_IO_CAP_KEYBOARD_ONLY    2
#define GAP_IO_CAP_NO_INPUT_OUTPUT  3
#define GAP_IO_CAP_KEYBOARD_DISPLAY 4

#define GAP_SET_IO_CAP      0x10
typedef __packed struct gap_set_io_cap_cmd {
  uint8_t io_cap;
} gap_set_io_cap_cmd_t;

#define GAP_PAIR      0x11
typedef __packed struct gap_pair_cmd {
  uint8_t address_type;
  uint8_t address[6];
} gap_pair_cmd_t;

#define GAP_UNPAIR      0x12
typedef __packed struct gap_unpair_cmd {
  uint8_t address_type;
  uint8_t address[6];
} gap_unpair_cmd_t;

#define GAP_PASSKEY_ENTRY   0x13
typedef __packed struct gap_passkey_entry_cmd {
  uint8_t  address_type;
  uint8_t  address[6];
  uint32_t passkey;
} gap_passkey_entry_cmd_t;

#define GAP_PASSKEY_CONFIRM   0x14
typedef __packed struct gap_passkey_confirm_cmd {
  uint8_t address_type;
  uint8_t address[6];
  uint8_t match;
} gap_passkey_confirm_cmd_t;

#define GAP_START_DIRECTED_ADV_HD BIT(0)
#define GAP_START_DIRECTED_ADV_OWN_ID BIT(1)
#define GAP_START_DIRECTED_ADV_PEER_RPA BIT(2)

#define GAP_START_DIRECTED_ADV    0x15
typedef __packed struct gap_start_directed_adv_cmd {
  uint8_t address_type;
  uint8_t address[6];
  uint16_t options;
} gap_start_directed_adv_cmd_t;
typedef __packed struct gap_start_directed_adv_rp {
  uint32_t current_settings;
} gap_start_directed_adv_rp_t;

#define GAP_CONN_PARAM_UPDATE   0x16
typedef __packed struct gap_conn_param_update_cmd {
  uint8_t address_type;
  uint8_t address[6];
  uint16_t interval_min;
  uint16_t interval_max;
  uint16_t latency;
  uint16_t timeout;
} gap_conn_param_update_cmd_t;

#define GAP_PAIRING_CONSENT   0x17
typedef __packed struct gap_pairing_consent_cmd {
  uint8_t address_type;
  uint8_t address[6];
  uint8_t consent;
} gap_pairing_consent_cmd_t;

#define GAP_OOB_LEGACY_SET_DATA   0x18
typedef __packed struct gap_oob_legacy_set_data_cmd {
  uint8_t oob_data[16];
} gap_oob_legacy_set_data_cmd_t;

#define GAP_OOB_SC_GET_LOCAL_DATA 0x19
typedef __packed struct gap_oob_sc_get_local_data_rp {
  uint8_t rand[16];
  uint8_t conf[16];
} gap_oob_sc_get_local_data_rp_t;

#define GAP_OOB_SC_SET_REMOTE_DATA  0x1a
typedef __packed struct gap_oob_sc_set_remote_data_cmd {
  uint8_t rand[16];
  uint8_t conf[16];
} gap_oob_sc_set_remote_data_cmd_t;

#define GAP_SET_MITM      0x1b
typedef __packed struct gap_set_mitm {
  uint8_t mitm;
} gap_set_mitm_t;

/* events */
#define GAP_EV_NEW_SETTINGS   0x80
typedef __packed struct gap_new_settings_ev {
  uint32_t current_settings;
} gap_new_settings_ev_t;

#define GAP_DEVICE_FOUND_FLAG_RSSI  0x01
#define GAP_DEVICE_FOUND_FLAG_AD    0x02
#define GAP_DEVICE_FOUND_FLAG_SD    0x04

#define GAP_EV_DEVICE_FOUND   0x81
typedef __packed struct gap_device_found_ev {
  uint8_t  address_type;
  uint8_t  address[6];
  int8_t   rssi;
  uint8_t  flags;
  uint16_t eir_data_len;
  uint8_t  eir_data[];
} gap_device_found_ev_t;

#define GAP_EV_DEVICE_CONNECTED   0x82
typedef __packed struct gap_device_connected_ev {
  uint8_t address_type;
  uint8_t address[6];
  uint16_t interval;
  uint16_t latency;
  uint16_t timeout;
} gap_device_connected_ev_t;

#define GAP_EV_DEVICE_DISCONNECTED  0x83
typedef __packed struct gap_device_disconnected_ev {
  uint8_t address_type;
  uint8_t address[6];
} gap_device_disconnected_ev_t;

#define GAP_EV_PASSKEY_DISPLAY    0x84
typedef __packed struct gap_passkey_display_ev {
  uint8_t  address_type;
  uint8_t  address[6];
  uint32_t passkey;
} gap_passkey_display_ev_t;

#define GAP_EV_PASSKEY_ENTRY_REQ  0x85
typedef __packed struct gap_passkey_entry_req_ev {
  uint8_t address_type;
  uint8_t address[6];
} gap_passkey_entry_req_ev_t;

#define GAP_EV_PASSKEY_CONFIRM_REQ  0x86
typedef __packed struct gap_passkey_confirm_req_ev {
  uint8_t  address_type;
  uint8_t  address[6];
  uint32_t passkey;
} gap_passkey_confirm_req_ev_t;

#define GAP_EV_IDENTITY_RESOLVED  0x87
typedef __packed struct gap_identity_resolved_ev {
  uint8_t address_type;
  uint8_t address[6];
  uint8_t identity_address_type;
  uint8_t identity_address[6];
} gap_identity_resolved_ev_t;

#define GAP_EV_CONN_PARAM_UPDATE  0x88
typedef __packed struct gap_conn_param_update_ev {
  uint8_t address_type;
  uint8_t address[6];
  uint16_t interval;
  uint16_t latency;
  uint16_t timeout;
} gap_conn_param_update_ev_t;

#define GAP_SEC_LEVEL_UNAUTH_ENC  0x01
#define GAP_SEC_LEVEL_AUTH_ENC    0x02
#define GAP_SEC_LEVEL_AUTH_SC     0x03

#define GAP_EV_SEC_LEVEL_CHANGED  0x89
typedef __packed struct gap_sec_level_changed_ev {
  uint8_t address_type;
  uint8_t address[6];
  uint8_t sec_level;
} gap_sec_level_changed_ev_t;

#define GAP_EV_PAIRING_CONSENT_REQ  0x8a
typedef __packed struct gap_pairing_consent_req_ev {
  uint8_t address_type;
  uint8_t address[6];
} gap_pairing_consent_req_ev_t;

#define GAP_EV_BOND_LOST  0x8b
typedef __packed struct gap_bond_lost_ev {
  uint8_t address_type;
  uint8_t address[6];
} gap_bond_lost_ev_t;

#define GAP_EV_PAIRING_FAILED   0x8c
typedef __packed struct gap_bond_pairing_failed_ev {
  uint8_t address_type;
  uint8_t address[6];
  uint8_t reason;
} gap_bond_pairing_failed_ev_t;

/* L2CAP Service */
/* commands */
#define L2CAP_READ_SUPPORTED_COMMANDS   0x01
typedef __packed struct l2cap_read_supported_commands_rp_tag {
    uint8_t data[1];
} l2cap_read_supported_commands_rp_t;

#define L2CAP_CONNECT_OPT_ECFC          0x01
#define L2CAP_CONNECT_OPT_HOLD_CREDIT   0x02

#define L2CAP_CONNECT   0x02
typedef __packed struct l2cap_connect_cmd_tag {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t psm;
    uint16_t mtu;
    uint8_t num;
    uint8_t options;
} l2cap_connect_cmd_t;

typedef __packed struct l2cap_connect_rp_tag {
    uint8_t num;
    uint8_t chan_id[];
} l2cap_connect_rp_t;

#define L2CAP_DISCONNECT    0x03
typedef __packed struct l2cap_disconnect_cmd_tag {
    uint8_t chan_id;
} l2cap_disconnect_cmd_t;

#define L2CAP_SEND_DATA     0x04
typedef __packed struct l2cap_send_data_cmd_tag {
    uint8_t chan_id;
    uint16_t data_len;
    uint8_t data[];
} l2cap_send_data_cmd_t;

#define L2CAP_TRANSPORT_BREDR                       0x00
#define L2CAP_TRANSPORT_LE                          0x01

#define L2CAP_CONNECTION_RESPONSE_SUCCESS           0x00
#define L2CAP_CONNECTION_RESPONSE_INSUFF_AUTHEN     0x01
#define L2CAP_CONNECTION_RESPONSE_INSUFF_AUTHOR     0x02
#define L2CAP_CONNECTION_RESPONSE_INSUFF_ENC_KEY    0x03

#define L2CAP_LISTEN    0x05
typedef __packed struct l2cap_listen_cmd_tag {
    uint16_t psm;
    uint8_t transport;
    uint16_t mtu;
    uint16_t response;
} l2cap_listen_cmd_t;

#define L2CAP_ACCEPT_CONNECTION     0x06
typedef __packed struct l2cap_accept_connection_cmd_tag {
    uint8_t chan_id;
    uint16_t result;
} l2cap_accept_connection_cmd_t;

#define L2CAP_RECONFIGURE   0x07
typedef __packed struct l2cap_reconfigure_cmd_tag {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t mtu;
    uint8_t num;
    uint8_t chan_id[];
} l2cap_reconfigure_cmd_t;

#define L2CAP_CREDITS   0x08
typedef __packed struct l2cap_credits_cmd_tag {
    uint8_t chan_id;
} l2cap_credits_cmd_t;

#define L2CAP_DISCONNECT_EATT_CHANS     0x09
typedef __packed struct l2cap_disconnect_eatt_chans_cmd_tag {
    uint8_t address_type;
    uint8_t address[6];
    uint8_t count;
} l2cap_disconnect_eatt_chans_cmd_t;

/* events */
#define L2CAP_EV_CONNECTION_REQ     0x80
typedef __packed struct l2cap_connection_req_ev_tag {
    uint8_t chan_id;
    uint16_t psm;
    uint8_t address_type;
    uint8_t address[6];
} l2cap_connection_req_ev_t;

#define L2CAP_EV_CONNECTED  0x81
typedef __packed struct l2cap_connected_ev_tag {
    uint8_t chan_id;
    uint16_t psm;
    uint16_t mtu_remote;
    uint16_t mps_remote;
    uint16_t mtu_local;
    uint16_t mps_local;
    uint8_t address_type;
    uint8_t address[6];
} l2cap_connected_ev_t;

#define L2CAP_EV_DISCONNECTED   0x82
typedef __packed struct l2cap_disconnected_ev_tag {
    uint16_t result;
    uint8_t chan_id;
    uint16_t psm;
    uint8_t address_type;
    uint8_t address[6];
} l2cap_disconnected_ev_t;

#define L2CAP_EV_DATA_RECEIVED  0x83
typedef __packed struct l2cap_data_received_ev_tag {
    uint8_t chan_id;
    uint16_t data_length;
    uint8_t data[];
} l2cap_data_received_ev_t;

#define L2CAP_EV_RECONFIGURED   0x84
typedef __packed struct l2cap_reconfigured_ev_tag {
    uint8_t chan_id;
    uint16_t mtu_remote;
    uint16_t mps_remote;
    uint16_t mtu_local;
    uint16_t mps_local;
} l2cap_reconfigured_ev_t;

/* GATT Service */

/* GATT server context */
#define SERVER_MAX_SERVICES     10
#define SERVER_MAX_ATTRIBUTES   50
#define SERVER_MAX_UUID_LEN     16
#define SERVER_BUF_SIZE         2048

/* commands */
#define GATT_READ_SUPPORTED_COMMANDS    0x01
typedef __packed struct gatt_read_supported_commands_rp {
    uint8_t data;
} gatt_read_supported_commands_rp_t;

#define GATT_SERVICE_PRIMARY        0x00
#define GATT_SERVICE_SECONDARY      0x01

#define GATT_ADD_SERVICE        0x02
typedef __packed struct gatt_add_service_cmd {
    uint8_t type;
    uint8_t uuid_length;
    uint8_t uuid[];
} gatt_add_service_cmd_t;
typedef __packed struct gatt_add_service_rp {
    uint16_t svc_id;
} gatt_add_service_rp_t;

#define GATT_ADD_CHARACTERISTIC     0x03
typedef __packed struct gatt_add_characteristic_cmd {
    uint16_t svc_id;
    uint8_t properties;
    uint8_t permissions;
    uint8_t uuid_length;
    uint8_t uuid[SERVER_MAX_UUID_LEN];
} gatt_add_characteristic_cmd_t;
typedef __packed struct gatt_add_characteristic_rp {
    uint16_t char_id;
} gatt_add_characteristic_rp_t;

#define GATT_ADD_DESCRIPTOR     0x04
typedef __packed struct gatt_add_descriptor_cmd {
    uint16_t char_id;
    uint8_t permissions;
    uint8_t uuid_length;
    uint8_t uuid[];
} gatt_add_descriptor_cmd_t;
typedef __packed struct gatt_add_descriptor_rp {
    uint16_t desc_id;
} gatt_add_descriptor_rp_t;

#define GATT_ADD_INCLUDED_SERVICE   0x05
typedef __packed struct gatt_add_included_service_cmd {
    uint16_t svc_id;
} gatt_add_included_service_cmd_t;
typedef __packed struct gatt_add_included_service_rp {
    uint16_t included_service_id;
} gatt_add_included_service_rp_t;

#define GATT_SET_VALUE          0x06
typedef __packed struct gatt_set_value_cmd {
    uint16_t attr_id;
    uint16_t len;
    uint8_t value[];
} gatt_set_value_cmd_t;

#define GATT_START_SERVER       0x07
typedef __packed struct gatt_start_server_rp {
    uint16_t db_attr_off;
    uint8_t db_attr_cnt;
} gatt_start_server_rp_t;

#define GATT_RESET_SERVER       0x08

#define GATT_SET_ENC_KEY_SIZE       0x09
typedef __packed struct gatt_set_enc_key_size_cmd {
    uint16_t attr_id;
    uint8_t key_size;
} gatt_set_enc_key_size_cmd_t;

/* Gatt Client */
typedef __packed struct gatt_service {
    uint16_t start_handle;
    uint16_t end_handle;
    uint8_t uuid_length;
    uint8_t uuid[SERVER_MAX_UUID_LEN];
} gatt_service_t;

typedef __packed struct gatt_included {
    uint16_t included_handle;
    struct gatt_service service;
} gatt_included_t;

typedef __packed struct gatt_characteristic {
    uint16_t characteristic_handle;
    uint16_t value_handle;
    uint8_t properties;
    uint8_t uuid_length;
    uint8_t uuid[SERVER_MAX_UUID_LEN];
} gatt_characteristic_t;

typedef __packed struct gatt_descriptor {
    uint16_t descriptor_handle;
    uint8_t uuid_length;
    uint8_t uuid[SERVER_MAX_UUID_LEN];
} gatt_descriptor_t;

#define GATT_EXCHANGE_MTU       0x0a
typedef __packed struct gatt_exchange_mtu_cmd {
    uint8_t address_type;
    uint8_t address[6];
} gatt_exchange_mtu_cmd_t;

#define GATT_DISC_ALL_PRIM      0x0b
typedef __packed struct gatt_disc_all_prim_cmd {
    uint8_t address_type;
    uint8_t address[6];
} gatt_disc_all_prim_cmd_t;
typedef __packed struct gatt_disc_all_prim_rp {
    uint8_t services_count;
    gatt_service_t services[SERVER_MAX_SERVICES];
} gatt_disc_all_prim_rp_t;

#define GATT_DISC_PRIM_UUID     0x0c
typedef __packed struct gatt_disc_prim_uuid_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint8_t uuid_length;
    uint8_t uuid[];
} gatt_disc_prim_uuid_cmd_t;
typedef __packed struct gatt_disc_prim_rp {
    uint8_t services_count;
    gatt_service_t services[SERVER_MAX_SERVICES];
} gatt_disc_prim_rp_t;

#define GATT_FIND_INCLUDED      0x0d
typedef __packed struct gatt_find_included_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t start_handle;
    uint16_t end_handle;
} gatt_find_included_cmd_t;
typedef __packed struct gatt_find_included_rp {
    uint8_t services_count;
    gatt_included_t included[SERVER_MAX_SERVICES];
} gatt_find_included_rp_t;

#define GATT_DISC_ALL_CHRC      0x0e
typedef __packed struct gatt_disc_all_chrc_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t start_handle;
    uint16_t end_handle;
} gatt_disc_all_chrc_cmd_t;
typedef __packed struct gatt_disc_chrc_rp {
    uint8_t characteristics_count;
    gatt_characteristic_t characteristics[SERVER_MAX_ATTRIBUTES];
} gatt_disc_chrc_rp_t;

#define GATT_DISC_CHRC_UUID     0x0f
typedef __packed struct gatt_disc_chrc_uuid_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t start_handle;
    uint16_t end_handle;
    uint8_t uuid_length;
    uint8_t uuid[];
} gatt_disc_chrc_uuid_cmd_t;

#define GATT_DISC_ALL_DESC      0x10
typedef __packed struct gatt_disc_all_desc_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t start_handle;
    uint16_t end_handle;
} gatt_disc_all_desc_cmd_t;
typedef __packed struct gatt_disc_all_desc_rp {
    uint8_t descriptors_count;
    gatt_descriptor_t descriptors[SERVER_MAX_ATTRIBUTES];
} gatt_disc_all_desc_rp_t;

#define GATT_READ           0x11
typedef __packed struct gatt_read_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t handle;
} gatt_read_cmd_t;
typedef __packed struct gatt_read_rp {
    uint8_t att_response;
    uint16_t data_length;
    uint8_t data[];
} gatt_read_rp_t;

#define GATT_READ_UUID          0x12
typedef __packed struct gatt_read_uuid_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t start_handle;
    uint16_t end_handle;
    uint8_t uuid_length;
    uint8_t uuid[];
} gatt_read_uuid_cmd_t;
typedef __packed struct gatt_char_value {
	uint16_t handle;
	uint8_t data_len;
	uint8_t data[SERVER_MAX_UUID_LEN];
} gatt_char_value_t;
typedef __packed struct gatt_read_uuid_rp {
    uint8_t att_response;
    uint8_t values_count;
    gatt_char_value_t values[SERVER_MAX_ATTRIBUTES];
} gatt_read_uuid_rp_t;

#define GATT_READ_LONG          0x13
typedef __packed struct gatt_read_long_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t handle;
    uint16_t offset;
} gatt_read_long_cmd_t;
typedef __packed struct gatt_read_long_rp {
    uint8_t att_response;
    uint16_t data_length;
    uint8_t data[];
} gatt_read_long_rp_t;

#define GATT_READ_MULTIPLE      0x14
typedef __packed struct gatt_read_multiple_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint8_t handles_count;
    uint16_t handles[];
} gatt_read_multiple_cmd_t;
typedef __packed struct gatt_read_multiple_rp {
    uint8_t att_response;
    uint16_t data_length;
    uint8_t data[];
} gatt_read_multiple_rp_t;

#define GATT_WRITE_WITHOUT_RSP      0x15
typedef __packed struct gatt_write_without_rsp_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t handle;
    uint16_t data_length;
    uint8_t data[];
} gatt_write_without_rsp_cmd_t;

#define GATT_SIGNED_WRITE_WITHOUT_RSP   0x16
typedef __packed struct gatt_signed_write_without_rsp_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t handle;
    uint16_t data_length;
    uint8_t data[];
} gatt_signed_write_without_rsp_cmd_t;

#define GATT_WRITE          0x17
typedef __packed struct gatt_write_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t handle;
    uint16_t data_length;
    uint8_t data[];
} gatt_write_cmd_t;
typedef __packed struct gatt_write_rp {
    uint8_t att_response;
} gatt_write_rp_t;

#define GATT_WRITE_LONG         0x18
typedef __packed struct gatt_write_long_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t handle;
    uint16_t offset;
    uint16_t data_length;
    uint8_t data[];
} gatt_write_long_cmd_t;
typedef __packed struct gatt_write_long_rp {
    uint8_t att_response;
} gatt_write_long_rp_t;

#define GATT_RELIABLE_WRITE     0x19
typedef __packed struct gatt_reliable_write_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t handle;
    uint16_t offset;
    uint16_t data_length;
    uint8_t data[];
} gatt_reliable_write_cmd_t;
typedef __packed struct gatt_reliable_write_rp {
    uint8_t att_response;
} gatt_reliable_write_rp_t;

#define GATT_CFG_NOTIFY         0x1a
#define GATT_CFG_INDICATE       0x1b
typedef __packed struct gatt_cfg_notify_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint8_t enable;
    uint16_t ccc_handle;
} gatt_cfg_notify_cmd_t;

#define GATT_GET_ATTRIBUTES     0x1c
typedef __packed struct gatt_get_attributes_cmd {
    uint16_t start_handle;
    uint16_t end_handle;
    uint8_t type_length;
    uint8_t type[];
} gatt_get_attributes_cmd_t;
typedef __packed struct gatt_get_attributes_rp {
    uint8_t attrs_count;
    uint8_t attrs[];
} gatt_get_attributes_rp_t;
typedef __packed struct gatt_attr {
    uint16_t handle;
    uint8_t permission;
    uint8_t type_length;
    uint8_t type[];
} gatt_attr_t;

#define GATT_GET_ATTRIBUTE_VALUE    0x1d
typedef __packed struct gatt_get_attribute_value_cmd {
    uint8_t address_type;
    uint8_t address[6];
    uint16_t handle;
} gatt_get_attribute_value_cmd_t;
typedef __packed struct gatt_get_attribute_value_rp {
    uint8_t att_response;
    uint16_t value_length;
    uint8_t value[];
} gatt_get_attribute_value_rp_t;

#define GATT_CHANGE_DB          0x1e
typedef __packed struct gatt_change_db_cmd {
    uint16_t start_handle;
    uint8_t visibility;
} gatt_change_db_cmd_t;

#define GATT_READ_MULTIPLE_VAR      0x20

/* GATT events */
#define GATT_EV_NOTIFICATION        0x80
typedef __packed struct gatt_notification_ev {
    uint8_t address_type;
    uint8_t address[6];
    uint8_t type;
    uint16_t handle;
    uint16_t data_length;
    uint8_t data[];
} gatt_notification_ev_t;

#define GATT_EV_ATTR_VALUE_CHANGED  0x81
typedef __packed struct gatt_attr_value_changed_ev {
    uint16_t handle;
    uint16_t data_length;
    uint8_t data[];
} gatt_attr_value_changed_ev_t;

/*******************************************************************************
 * API
 ******************************************************************************/
void bt_tester_task(void *pvParameters);

uint8_t tester_init_gap(void);
uint8_t tester_init_l2cap(void);
uint8_t tester_init_gatt(void);

void tester_rsp
(
    uint8_t service,
    uint8_t opcode,
    uint8_t index,
    uint8_t status
);
void tester_send
(
    uint8_t service,
    uint8_t opcode,
    uint8_t index,
    uint8_t *data,
    size_t len
);

void tester_handle_gap
(
    uint8_t opcode,
    uint8_t index,
    uint8_t *data,
    uint16_t len
);
void tester_handle_l2cap
(
    uint8_t opcode,
    uint8_t index,
    uint8_t *data,
    uint16_t len
);
void tester_handle_gatt
(
    uint8_t opcode,
    uint8_t index,
    uint8_t *data,
    uint16_t len
);

uint8_t tester_unregister_l2cap(void);
uint8_t tester_unregister_gatt(void);
uint8_t tester_unregister_gap(void);

extern BaseType_t xReturn;

/*******************************************************************************
 * Code
 ******************************************************************************/
static inline void tester_set_bit(uint8_t *addr, unsigned int bit)
{
    uint8_t *p = addr + (bit / 8U);

    *p |= BIT(bit % 8);
}

static inline uint8_t tester_test_bit(const uint8_t *addr, unsigned int bit)
{
    const uint8_t *p = addr + (bit / 8U);

    return *p & BIT(bit % 8);
}

#ifdef __cplusplus
}
#endif