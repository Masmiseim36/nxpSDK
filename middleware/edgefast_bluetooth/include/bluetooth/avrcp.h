/** @file
 * @brief AVRCP Profile header.
 */

/*
 * Copyright (c) 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_BLUETOOTH_AVRCP_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_AVRCP_H_

/** AVRCP cover art PSM */
#define BT_AVRCP_COVER_ART_PSM (0x1005)

/** AVRCP SubUnit Types */
#define BT_AVRCP_SUBUNIT_TYPE_UNIT  0x1F
#define BT_AVRCP_SUBUNIT_TYPE_PANEL 0x09

/** AVRCP Command Opcode Types */
#define BT_AVRCP_OPCODE_UNIT_INFO        0x30
#define BT_AVRCP_OPCODE_SUBUNIT_INFO     0x31
#define BT_AVRCP_OPCODE_PASS_THROUGH     0x7C
#define BT_AVRCP_OPCODE_VENDOR_DEPENDENT 0x00

/** AVRCP operation ID PASS THROUGH */
#define BT_AVRCP_OP_ID_SELECT              0x00
#define BT_AVRCP_OP_ID_UP                  0x01
#define BT_AVRCP_OP_ID_DOWN                0x02
#define BT_AVRCP_OP_ID_LEFT                0x03
#define BT_AVRCP_OP_ID_RIGHT               0x04
#define BT_AVRCP_OP_ID_RIGHT_UP            0x05
#define BT_AVRCP_OP_ID_RIGHT_DOWN          0x06
#define BT_AVRCP_OP_ID_LEFT_UP             0x07
#define BT_AVRCP_OP_ID_LEFT_DOWN           0x08
#define BT_AVRCP_OP_ID_ROOT_MENU           0x09
#define BT_AVRCP_OP_ID_SETUP_MENU          0x0A
#define BT_AVRCP_OP_ID_CONTENTS_MENU       0x0B
#define BT_AVRCP_OP_ID_FAVORITE_MENU       0x0C
#define BT_AVRCP_OP_ID_EXIT                0x0D
#define BT_AVRCP_OP_ID_0                   0x20
#define BT_AVRCP_OP_ID_1                   0x21
#define BT_AVRCP_OP_ID_2                   0x22
#define BT_AVRCP_OP_ID_3                   0x23
#define BT_AVRCP_OP_ID_4                   0x24
#define BT_AVRCP_OP_ID_5                   0x25
#define BT_AVRCP_OP_ID_6                   0x26
#define BT_AVRCP_OP_ID_7                   0x27
#define BT_AVRCP_OP_ID_8                   0x28
#define BT_AVRCP_OP_ID_9                   0x29
#define BT_AVRCP_OP_ID_DOT                 0x2A
#define BT_AVRCP_OP_ID_ENTER               0x2B
#define BT_AVRCP_OP_ID_CLEAR               0x2C
#define BT_AVRCP_OP_ID_CHANNEL_UP          0x30
#define BT_AVRCP_OP_ID_CHANNEL_DOWN        0x31
#define BT_AVRCP_OP_ID_PREVIOUS_CHANNEL    0x32
#define BT_AVRCP_OP_ID_SOUND_SELECT        0x33
#define BT_AVRCP_OP_ID_INPUT_SELECT        0x34
#define BT_AVRCP_OP_ID_DISPLAY_INFORMATION 0x35
#define BT_AVRCP_OP_ID_HELP                0x36
#define BT_AVRCP_OP_ID_PAGE_UP             0x37
#define BT_AVRCP_OP_ID_PAGE_DOWN           0x38
#define BT_AVRCP_OP_ID_POWER               0x40
#define BT_AVRCP_OP_ID_VOLUME_UP           0x41
#define BT_AVRCP_OP_ID_VOLUME_DOWN         0x42
#define BT_AVRCP_OP_ID_MUTE                0x43
#define BT_AVRCP_OP_ID_PLAY                0x44
#define BT_AVRCP_OP_ID_STOP                0x45
#define BT_AVRCP_OP_ID_PAUSE               0x46
#define BT_AVRCP_OP_ID_RECORD              0x47
#define BT_AVRCP_OP_ID_REWIND              0x48
#define BT_AVRCP_OP_ID_FAST_FORWARD        0x49
#define BT_AVRCP_OP_ID_EJECT               0x4A
#define BT_AVRCP_OP_ID_FORWARD             0x4B
#define BT_AVRCP_OP_ID_BACKWARD            0x4C
#define BT_AVRCP_OP_ID_ANGLE               0x50
#define BT_AVRCP_OP_ID_SUBPICTURE          0x51
#define BT_AVRCP_OP_ID_F1                  0x71
#define BT_AVRCP_OP_ID_F2                  0x72
#define BT_AVRCP_OP_ID_F3                  0x73
#define BT_AVRCP_OP_ID_F4                  0x74
#define BT_AVRCP_OP_ID_F5                  0x75
#define BT_AVRCP_OP_ID_VENDOR_UNIQUE       0x7E

/** VENDOR DEPENDENT Transfer PDU ID */
#define BT_AVRCP_PDU_ID_GET_CAPABILITY                  0x10
#define BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR    0x11
#define BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL     0x12
#define BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL  0x13
#define BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL      0x14
#define BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT 0x15
#define BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT  0x16
#define BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET     0x17
#define BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS           0x18
#define BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE           0x20
#define BT_AVRCP_PDU_ID_GET_PLAY_STATUS                 0x30
#define BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION           0x31
#define BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE     0x40
#define BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE       0x41
#define BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME             0x50
#define BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER            0x60
#define BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER              0x70
#define BT_AVRCP_PDU_ID_GET_FOLDER_ITEMS                0x71
#define BT_AVRCP_PDU_ID_CHANGE_PATH                     0x72
#define BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES             0x73
#define BT_AVRCP_PDU_ID_PLAY_ITEMS                      0x74
#define BT_AVRCP_PDU_ID_SEARCH                          0x80
#define BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING              0x90
#define BT_AVRCP_PDU_ID_GENERAL_REJECT                  0xA0
#define BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS             0x75

/** AVRCP Capability. */
#define BT_AVRCP_CAP_ID_COMPANY_ID       0x02
#define BT_AVRCP_CAP_ID_EVENTS_SUPPORTED 0x03

/** AVRCP Message Response Types. */
#define BT_AVRCP_RESPONSE_TYPE_NOT_IMPLEMENTED 0x08
#define BT_AVRCP_RESPONSE_TYPE_ACCEPTED        0x09
#define BT_AVRCP_RESPONSE_TYPE_REJECTED        0x0A
#define BT_AVRCP_RESPONSE_TYPE_INTERIM         0x0F
#define BT_AVRCP_RESPONSE_TYPE_STABLE          0x0C
#define BT_AVRCP_RESPONSE_TYPE_CHANGED         0x0D

/** AVRCP PDU Packet Type. */
#define BT_AVRCP_PACKET_TYPE_SINGLE   0x00
#define BT_AVRCP_PACKET_TYPE_START    0x01
#define BT_AVRCP_PACKET_TYPE_CONTINUE 0x02
#define BT_AVRCP_PACKET_TYPE_END      0x03

/** AVRCP Events. */
#define BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED     0x01
#define BT_AVRCP_EVENT_TRACK_CHANGED               0x02
#define BT_AVRCP_EVENT_TRACK_REACHED_END           0x03
#define BT_AVRCP_EVENT_TRACK_REACHED_START         0x04
#define BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED        0x05
#define BT_AVRCP_EVENT_BATT_STATUS_CHANGED         0x06
#define BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED       0x07
#define BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED  0x08
#define BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED 0x09
#define BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED    0x0A
#define BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED    0x0B
#define BT_AVRCP_EVENT_UIDS_CHANGED                0x0C
#define BT_AVRCP_EVENT_VOLUME_CHANGED              0x0D

/** AVRCP Player Application Setting Attribute ID.*/
#define BT_AVRCP_PLAYER_APP_ATTR_ID_EQUALIZER 0x01
#define BT_AVRCP_PLAYER_APP_ATTR_ID_REPEAT    0x02
#define BT_AVRCP_PLAYER_APP_ATTR_ID_SHUFFLE   0x03
#define BT_AVRCP_PLAYER_APP_ATTR_ID_SCAN      0x04

/** AVRCP Metadata Attribute ID. */
#define BT_AVRCP_MEDIA_ATTR_ID_MEDIA_TITLE        0x01
#define BT_AVRCP_MEDIA_ATTR_ID_ARTIST_NAME        0x02
#define BT_AVRCP_MEDIA_ATTR_ID_ALBUM_NAME         0x03
#define BT_AVRCP_MEDIA_ATTR_ID_MEDIA_NUMBER       0x04
#define BT_AVRCP_MEDIA_ATTR_ID_TOTAL_MEDIA_NUMBER 0x05
#define BT_AVRCP_MEDIA_ATTR_ID_GENRE              0x06
#define BT_AVRCP_MEDIA_ATTR_ID_PLAYING_TIME       0x07
#define BT_AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART  0x08

/** AVRCP System Status Code. */
#define BT_AVRCP_SYSTEM_STATUS_POWER_ON  0x00
#define BT_AVRCP_SYSTEM_STATUS_POWER_OFF 0x01
#define BT_AVRCP_SYSTEM_STATUS_UNPLUGED  0x02

/** AVRCP Battery Status Code. */
#define BT_AVRCP_BATTERY_STATUS_NORMAL   0x00
#define BT_AVRCP_BATTERY_STATUS_WARNING  0x01
#define BT_AVRCP_BATTERY_STATUS_CRITICAL 0x02
#define BT_AVRCP_BATTERY_STATUS_EXTERNAL 0x03
#define BT_AVRCP_BATTERY_STATUS_FULL     0x04

/** AVRCP Error Status Code. */
#define BT_AVRCP_METADATA_ERROR_INVALID_COMMAND      0x00
#define BT_AVRCP_METADATA_ERROR_INVALID_PARAMETER    0x01
#define BT_AVRCP_METADATA_ERROR_PARAMETER_NOT_FOUND  0x02
#define BT_AVRCP_METADATA_ERROR_INTERNAL             0x03
#define BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL 0x04
#define BT_AVRCP_METADATA_ERROR_UID_CHANGED          0x05

/** AVRCP Message Command Types. */
#define BT_AVRCP_COMMAND_TYPE_CONTROL 0x00
#define BT_AVRCP_COMMAND_TYPE_STATUS  0x01
#define BT_AVRCP_COMMAND_TYPE_NOTIFY  0x03

/** AVRCP Scopes. */
#define BT_AVRCP_SCOPE_MEDIA_PLAYER_LIST  0x00
#define BT_AVRCP_SCOPE_VIRTUAL_FILESYSTEM 0x01
#define BT_AVRCP_SCOPE_SEARCH             0x02
#define BT_AVRCP_SCOPE_NOW_PLAYING        0x03

/** AVRCP Browsing Error Status Code. */
#define BT_AVRCP_BOW_ERROR_INVALID_DIRECTION       0x07
#define BT_AVRCP_BOW_ERROR_NOT_A_DIRECTORY         0x08
#define BT_AVRCP_BOW_ERROR_DOES_NOT_EXIST          0x09
#define BT_AVRCP_BOW_ERROR_INVALID_SCOPE           0x0A
#define BT_AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS     0x0B
#define BT_AVRCP_BOW_ERROR_DIRECTORY_NOT_HANDLED   0x0C
#define BT_AVRCP_BOW_ERROR_MEDIA_IN_USE            0x0D
#define BT_AVRCP_BOW_ERROR_NOW_PLAYING_LIST_FULL   0x0E
#define BT_AVRCP_BOW_ERROR_SEARCH_NOT_SUPPORTED    0x0F
#define BT_AVRCP_BOW_ERROR_SEARCH_IN_PROGRESS      0x10
#define BT_AVRCP_BOW_ERROR_INVALID_PLAYER_ID       0x11
#define BT_AVRCP_BOW_ERROR_PLAYER_NOT_BROWSABLE    0x12
#define BT_AVRCP_BOW_ERROR_PLAYER_NOT_ADDRESSED    0x13
#define BT_AVRCP_BOW_ERROR_NO_VALID_SEARCH_RESULTS 0x14
#define BT_AVRCP_BOW_ERROR_NO_AVAILABLE_PLAYERS    0x15
#define BT_AVRCP_BOW_ERROR_ADDR_PLAYER_CHANGED     0x16

/** Browsable Item types */
#define BT_AVRCP_ITEM_TYPER_MEDIA_PLAYER 0x01
#define BT_AVRCP_ITEM_TYPER_FOLDER       0x02
#define BT_AVRCP_ITEM_TYPER_MEDIA        0x03

/** AVRCP Cover Art OBEX Response Codes */
#define BT_AVRCP_CA_SUCCESS_RSP         0xA0
#define BT_AVRCP_CA_CONTINUE_RSP        0x90
#define BT_AVRCP_CA_BAD_REQ_RSP         0xC0
#define BT_AVRCP_CA_NOT_IMPLEMENTED_RSP 0xD1
#define BT_AVRCP_CA_UNAUTH_RSP          0xC1
#define BT_AVRCP_CA_PRECOND_FAILED_RSP  0xCC
#define BT_AVRCP_CA_NOT_FOUND_RSP       0xC4
#define BT_AVRCP_CA_NOT_ACCEPTABLE_RSP  0xC6
#define BT_AVRCP_CA_NO_SERVICE_RSP      0xD3
#define BT_AVRCP_CA_FORBIDDEN_RSP       0xC3
#define BT_AVRCP_CA_SERVER_ERROR        0xD0

/** @brief avrcp message header */
STRUCT_PACKED_PRE
struct bt_avrcp_header
{
    union
    {
        struct
        {
            /** IPID */
            uint8_t ipid : 1;
            /** command/response */
            uint8_t cr : 1;
            /** packet type */
            uint8_t packet_type : 2;
            /** transaction label */
            uint8_t tl : 4;
        };
        uint8_t tl_pt_cr_ipid;
    };
    /** profile identifier (it is omitted) */
    /* uint16_t pid; */
    /** command type or response code */
    uint8_t ctype_response : 4;
    uint8_t reserved : 4;
    /** subunit id */
    uint8_t subunit_id : 3;
    /** subunit type */
    uint8_t subunit_type : 5;
    /** operation code */
    uint8_t op_code;
} STRUCT_PACKED_POST;

/** @brief unit info */
STRUCT_PACKED_PRE
struct bt_avrcp_unit_info
{
    uint8_t unit : 3;
    uint8_t unit_type : 5;
    uint8_t comapny_id[3];
} STRUCT_PACKED_POST;

/** @brief subunit info */
STRUCT_PACKED_PRE
struct bt_avrcp_subunit_info
{
    uint8_t exten_code : 3;
    uint8_t reserved1 : 1;
    uint8_t page : 3;
    uint8_t reserved2 : 1;
    uint8_t data[0];
} STRUCT_PACKED_POST;

/** @brief pass through message */
STRUCT_PACKED_PRE
struct bt_avrcp_pass_through
{
    /** operation id */
    uint8_t op_id : 7;
    /** state flag */
    uint8_t state_flag : 1;
    /** operation data length */
    uint8_t op_data_len;
    /** operation data */
    uint8_t op_data[0];
} STRUCT_PACKED_POST;

/** @brief register notification */
STRUCT_PACKED_PRE
struct bt_avrcp_register_ntfy
{
    /** event id */
    uint8_t event_id;
    /** Playback interval */
    uint32_t playback_interval;
} STRUCT_PACKED_POST;

/** @brief player application setting attribute IDs  */
STRUCT_PACKED_PRE
struct bt_avrcp_player_app_setting_attr_ids
{
    /** number of attributes */
    uint8_t num_of_attr;
    /** attribute IDs */
    uint8_t attr_ids[0];
} STRUCT_PACKED_POST;

/** @brief attribute value  */
STRUCT_PACKED_PRE
struct bt_avrcp_player_attr_value
{
    uint8_t attr_id;
    uint8_t value_id;
} STRUCT_PACKED_POST;

/** @brief player application values  */
STRUCT_PACKED_PRE
struct bt_avrcp_player_app_attr_values
{
    /** number of attributes */
    uint8_t num_of_attr;
    /** attribute value */
    struct bt_avrcp_player_attr_value attr_vals[0];
} STRUCT_PACKED_POST;

/** @brief get player application setting values text */
STRUCT_PACKED_PRE
struct bt_avrcp_get_player_app_setting_value_text
{
    uint8_t attr_id;
    /** number of attributes */
    uint8_t num_of_value;
    /** attribute IDs */
    uint8_t value_ids[0];
} STRUCT_PACKED_POST;

/** @brief inform displayable character set */
STRUCT_PACKED_PRE
struct bt_avrcp_inform_displayable_char_set
{
    uint8_t num_of_char;
    uint16_t char_sets[0];
} STRUCT_PACKED_POST;

/** @brief get element attributes */
STRUCT_PACKED_PRE
struct bt_avrcp_get_element_attrs
{
    /** PLAYING (0x0) */
    uint8_t identifier[8];
    uint8_t num_of_attr;
    uint32_t attr_ids[0];
} STRUCT_PACKED_POST;

/** @brief AddToNowPlaying */
STRUCT_PACKED_PRE
struct bt_avrcp_add_to_now_playing
{
    uint8_t scope;
    uint8_t uid[8];
    uint16_t uid_counter;
} STRUCT_PACKED_POST;

/** @brief PlayItem */
STRUCT_PACKED_PRE
struct bt_avrcp_play_item
{
    uint8_t scope;
    uint8_t uid[8];
    uint16_t uid_counter;
} STRUCT_PACKED_POST;


/** @brief company id */
STRUCT_PACKED_PRE
struct bt_avrcp_company_id
{
    /** company id */
    uint8_t company_id0;
    uint8_t company_id1;
    uint8_t company_id2;
} STRUCT_PACKED_POST;

/** @brief capability id */
STRUCT_PACKED_PRE
struct bt_avrcp_capability_company_id
{
    /** capability id */
    uint8_t capability_id;
    /** capability count */
    uint8_t capability_count;
    /** capability IDs */
    struct bt_avrcp_company_id company_ids[0];
} STRUCT_PACKED_POST;

/** @brief capability event supports  */
STRUCT_PACKED_PRE
struct bt_avrcp_capability_events_supported
{
    /** capability id */
    uint8_t capability_id;
    /** capability count */
    uint8_t capability_count;
    /** events supported */
    uint8_t event_ids[0];
} STRUCT_PACKED_POST;

/** @brief player application setting attribute IDs  */
STRUCT_PACKED_PRE
struct bt_avrcp_player_app_setting_values
{
    /** number of attributes */
    uint8_t num_of_value;
    /** attribute IDs */
    uint8_t value_ids[0];
} STRUCT_PACKED_POST;

/** @brief attribute/value text */
STRUCT_PACKED_PRE
struct bt_avrcp_player_attr_value_text
{
    uint8_t attr_value_id;
    uint16_t char_set;
    uint8_t string_len;
    uint8_t *string;
} STRUCT_PACKED_POST;

/** @brief get text response */
STRUCT_PACKED_PRE
struct bt_avrcp_player_get_txt_rsp
{
    uint8_t num_of_id;
    struct bt_avrcp_player_attr_value_text texts[0];
} STRUCT_PACKED_POST;

/** @brief element attribute */
STRUCT_PACKED_PRE
struct bt_avrcp_element_attr
{
    uint32_t attr_id;
    uint16_t char_set;
    uint16_t string_len;
    uint8_t *string;
} STRUCT_PACKED_POST;

/** @brief get element attribute response */
STRUCT_PACKED_PRE
struct bt_avrcp_player_get_element_attr_rsp
{
    uint8_t num_of_attr;
    struct bt_avrcp_element_attr attrs[0];
} STRUCT_PACKED_POST;

/** @brief play status response */
STRUCT_PACKED_PRE
struct bt_avrcp_play_status_rsp
{
    uint32_t song_length;
    uint32_t song_pos;
    uint8_t play_status;
} STRUCT_PACKED_POST;


/** @brief event response data format */
STRUCT_PACKED_PRE
struct bt_avrcp_event_rsp
{
    uint8_t event_id;
    union
    {
        /** EVENT_PLAYBACK_STATUS_CHANGED */
        uint8_t play_status;
        /** EVENT_TRACK_CHANGED */
        uint8_t identifier[8];
        /** EVENT_PLAYBACK_POS_CHANGED */
        uint32_t playback_pos;
        /** EVENT_BATT_STATUS_CHANGED */
        uint8_t battery_status;
        /** EVENT_SYSTEM_STATUS_CHANGED */
        uint8_t system_status;
        /** EVENT_PLAYER_APPLICATION_SETTING_CHANGED */
        struct
        {
            uint8_t num_of_attr;
            struct bt_avrcp_player_attr_value attr_vals[0];
        } setting_changed;
        /** EVENT_ADDRESSED_PLAYER_CHANGED */
        struct
        {
            uint16_t player_id;
            uint16_t uid_counter;
        } addressed_player_changed;
        /** EVENT_UIDS_CHANGED */
        uint16_t uid_counter;
        /** EVENT_VOLUME_CHANGED */
        uint8_t absolute_volume;
    };
} STRUCT_PACKED_POST;

/** @brief vendor dependent message */
STRUCT_PACKED_PRE
struct bt_avrcp_vendor
{
    /** company id */
    uint8_t company_id0;
    uint8_t company_id1;
    uint8_t company_id2;
    /** pdu id */
    uint8_t pdu_id;
    /** packet type */
    uint8_t packet_type : 2;
    /** reserved */
    uint8_t reserved : 6;
    /** parameter length */
    uint16_t parameter_len;
    union {
        /** parameters */
        uint8_t parameter; /* the message is one byte data */
        /* command message */
        struct bt_avrcp_player_app_setting_attr_ids player_attr_ids;
        struct bt_avrcp_player_app_attr_values player_attr_values;
        struct bt_avrcp_get_player_app_setting_value_text player_value_text;
        struct bt_avrcp_inform_displayable_char_set inform_char_set;
        struct bt_avrcp_get_element_attrs element_attrs;
        struct bt_avrcp_play_item play_item;
        struct bt_avrcp_add_to_now_playing add_now_play;
        struct bt_avrcp_register_ntfy register_notify;
        /* response message */
        struct bt_avrcp_capability_company_id comapny_id_rsp;
        struct bt_avrcp_capability_events_supported events_suported_rsp;
        struct bt_avrcp_player_app_setting_attr_ids attr_ids_rsp;
        struct bt_avrcp_player_app_setting_values values_rsp;
        struct bt_avrcp_player_app_attr_values attr_values_rsp;
        struct bt_avrcp_player_get_txt_rsp get_txt_rsp;
        struct bt_avrcp_player_get_element_attr_rsp element_attr_rsp;
        struct bt_avrcp_play_status_rsp play_status_rsp;
        struct bt_avrcp_event_rsp event_rsp;
    };    
} STRUCT_PACKED_POST;

/** @brief avrcp control message */
STRUCT_PACKED_PRE
struct bt_avrcp_control_msg
{
    struct bt_avrcp_header header;
    union
    {
        struct bt_avrcp_unit_info unit_info;
        struct bt_avrcp_subunit_info subunit_info;
        struct bt_avrcp_pass_through pass_th;
        struct bt_avrcp_vendor vendor;
    };
} STRUCT_PACKED_POST;

/** @brief browsing message header */
STRUCT_PACKED_PRE
struct bt_avrcp_browsing_header
{
    union
    {
        struct
        {
            /** IPID */
            uint8_t ipid : 1;
            /** command/response */
            uint8_t cr : 1;
            /** packet type */
            uint8_t packet_type : 2;
            /** transaction label */
            uint8_t tl : 4;
        };
        uint8_t tl_pt_cr_ipid;
    };
    /** profile identifier (it is omitted) */
    /* uint16_t pid; */
    /** pdu id */
    uint8_t pdu_id;
    /** parameter length */
    uint16_t parameter_len;
} STRUCT_PACKED_POST;

/** @brief get folder items player list */
STRUCT_PACKED_PRE
struct bt_avrcp_get_folder_items_cmd
{
    uint8_t scope;
    uint32_t start_item;
    uint32_t end_item;
    uint8_t attr_count;
    /** Appendix E in AVRCP spec
     *  every attribute id is 4 bytes.
     */
    uint32_t attr_list[0];
} STRUCT_PACKED_POST;

/** @brief set browsed player */
STRUCT_PACKED_PRE
struct bt_avrcp_set_browsed_player_cmd
{
    uint16_t player_id;
} STRUCT_PACKED_POST;

/** @brief change path */
STRUCT_PACKED_PRE
struct bt_avrcp_change_path_cmd
{
    uint16_t uid_counter;
    uint8_t direction;
    uint8_t folder_uid[8];
} STRUCT_PACKED_POST;

/** @brief get item attributes */
STRUCT_PACKED_PRE
struct bt_avrcp_get_item_attrs_cmd
{
    uint8_t scope;
    uint8_t uid[8];
    uint16_t uid_counter;
    uint8_t num_of_attr;
    /** Appendix E in AVRCP spec
     *  every attribute id is 4 bytes.
     */
    uint32_t attr_list[0];
} STRUCT_PACKED_POST;

/** @brief search */
STRUCT_PACKED_PRE
struct bt_avrcp_search_cmd
{
    uint8_t char_set;
    uint16_t length;
    uint8_t *str;
} STRUCT_PACKED_POST;

/** @brief get total number of items */
STRUCT_PACKED_PRE
struct bt_avrcp_get_total_num_of_items_cmd
{
    uint8_t scope;
} STRUCT_PACKED_POST;

/** @brief avrcp browsing command */
STRUCT_PACKED_PRE
struct bt_avrcp_browsing_cmd
{
    struct bt_avrcp_browsing_header header;
    union
    {
        struct bt_avrcp_get_folder_items_cmd folder_items;
        struct bt_avrcp_set_browsed_player_cmd set_browsed_player;
        struct bt_avrcp_change_path_cmd change_path;
        struct bt_avrcp_get_item_attrs_cmd get_item_attrs;
        struct bt_avrcp_search_cmd search;
        struct bt_avrcp_get_total_num_of_items_cmd get_total_num_of_items;
    };
} STRUCT_PACKED_POST;

/** @brief player item */
STRUCT_PACKED_PRE
struct bt_avrcp_player_item
{
    uint16_t player_id;
    uint8_t player_type;
    uint32_t player_subtype;
    uint8_t play_status;
    uint8_t feature_mask[16];
    uint16_t char_set;
    uint16_t name_len;
    uint8_t *name;
} STRUCT_PACKED_POST;

/** @brief folder item */
STRUCT_PACKED_PRE
struct bt_avrcp_folder_item
{
    uint8_t folder_uid[8];
    uint8_t folder_type;
    uint8_t playable;
    uint16_t char_set;
    uint16_t name_len;
    uint8_t *name;
} STRUCT_PACKED_POST;

/** @brief media item attribute */
STRUCT_PACKED_PRE
struct bt_avrcp_attr_val_entry
{
    uint32_t attr_id;
    uint16_t char_set;
    uint16_t value_len;
    uint8_t *value_str;
} STRUCT_PACKED_POST;

/** @brief media item */
STRUCT_PACKED_PRE
struct bt_avrcp_media_item
{
    uint8_t media_uid[8];
    uint8_t media_type;
    uint16_t char_set;
    uint16_t name_len;
    uint8_t *name;
    uint8_t num_of_attr;
    /* the max is 8 in avrcp spec */
    struct bt_avrcp_attr_val_entry attrs[8];
} STRUCT_PACKED_POST;

/** @brief avrcp item */
STRUCT_PACKED_PRE
struct bt_avrcp_item
{
    uint8_t item_type;
    uint16_t item_length;
    union
    {
        struct bt_avrcp_player_item player_item;
        struct bt_avrcp_folder_item folder_item;
        struct bt_avrcp_media_item media_item;
    };
} STRUCT_PACKED_POST;

/** @brief get folder items (response) */
STRUCT_PACKED_PRE
struct bt_avrcp_get_folder_items_rsp
{
    uint8_t status;
    uint16_t uid_counter;
    uint16_t num_of_items;
    struct bt_avrcp_item items[0];
} STRUCT_PACKED_POST;

/** @brief get folder name (response) */
STRUCT_PACKED_PRE
struct bt_avrcp_folder_name
{
    uint16_t folder_name_len;
    uint8_t *folder_name;
} STRUCT_PACKED_POST;

/** @brief set browsed player (response) */
STRUCT_PACKED_PRE
struct bt_avrcp_set_browsed_player_rsp
{
    uint8_t status;
    uint16_t uid_counter;
    uint32_t num_of_items;
    uint16_t char_set;
    uint8_t folder_depth;
    struct bt_avrcp_folder_name folder_names[0];
} STRUCT_PACKED_POST;

/** @brief change path (response) */
STRUCT_PACKED_PRE
struct bt_avrcp_change_path_rsp
{
    uint8_t status;
    uint32_t num_of_items;
} STRUCT_PACKED_POST;

/** @brief get item attributes (response) */
STRUCT_PACKED_PRE
struct bt_avrcp_get_item_attrs_rsp
{
    uint8_t status;
    uint8_t num_of_attr;
    /** Appendix E in AVRCP spec
     *  every attribute id is 4 bytes.
     */
    struct bt_avrcp_attr_val_entry attrs[0];
} STRUCT_PACKED_POST;

/** @brief search (response) */
STRUCT_PACKED_PRE
struct bt_avrcp_search_rsp
{
    uint8_t status;
    uint16_t uid_counter;
    uint32_t num_of_items;
} STRUCT_PACKED_POST;

/** @brief get total number of items (response) */
STRUCT_PACKED_PRE
struct bt_avrcp_get_total_num_of_items_rsp
{
    uint8_t status;
    uint16_t uid_counter;
    uint32_t num_of_items;
} STRUCT_PACKED_POST;

/** @brief browsing message (response) */
STRUCT_PACKED_PRE
struct bt_avrcp_browsing_rsp
{
    struct bt_avrcp_browsing_header header;
    union
    {
        struct bt_avrcp_get_folder_items_rsp folder_items;
        struct bt_avrcp_set_browsed_player_rsp set_browsed_player;
        struct bt_avrcp_change_path_rsp change_path;
        struct bt_avrcp_get_item_attrs_rsp get_item_attrs;
        struct bt_avrcp_search_rsp search;
        struct bt_avrcp_get_total_num_of_items_rsp get_total_num_of_items;
    };
} STRUCT_PACKED_POST;

/** @brief The connecting callback */
struct bt_avrcp_cb
{
    /** @brief avrcp control connection has been established.
     *
     *  This callback notifies the application of avrcp control connection.
     *
     *  @param conn connection object.
     *  @param err error code.
     */
    void (*control_connected)(struct bt_conn *conn, int err);
    /** @brief avrcp control connection has been disconnected.
     *
     *  This callback notifies the application of avrcp control disconnection.
     *
     *  @param conn connection object.
     */
    void (*control_disconnected)(struct bt_conn *conn, int err);
    /** @brief avrcp browsing connection has been established.
     *
     *  This callback notifies the application of avrcp browsing connection.
     *
     *  @param conn connection object.
     *  @param err error code.
     */
    void (*browsing_connected)(struct bt_conn *conn, int err);
    /** @brief avrcp browsing connection has been disconnected.
     *
     *  This callback notifies the application of avrcp browsing disconnection.
     *
     *  @param conn connection object.
     *  @param err error code.
     */
    void (*browsing_disconnected)(struct bt_conn *conn, int err);
    /** @brief avrcp send result callback.
     *
     *  This callback notifies the application that a avrcp command send result.
     *
     *  @param conn connection object.
     *  @param err error code.
     */
    void (*send_result)(struct bt_conn *conn, int err);
#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
    /** @brief received the control command.
     *
     *  This callback notifies the application that a avrcp control command is received.
	 *  The command is transfered in the AVRCP control l2cap channel.
     *
     *  @param conn connection object.
     *  @param msg command message.
     *  @param err error code.
     */
    void (*control_received)(struct bt_conn *conn, struct bt_avrcp_control_msg *msg, int err);
#endif
#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
    /** @brief received the control response.
     *
     *  This callback notifies the application that a avrcp control response is received.
	 *  The response is transfered in the AVRCP control l2cap channel.
     *
     *  @param conn connection object.
     *  @param msg response.
     *  @param err error code.
     */
    void (*control_rsp_received)(struct bt_conn *conn, struct bt_avrcp_control_msg *msg, int err);
#endif
#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
    /** @brief received the browsing command.
     *
     *  This callback notifies the application that a avrcp browsing command is received.
	 *  The command is transfered in the AVRCP browsing l2cap channel.
     *
     *  @param conn connection object.
     *  @param cmd command message.
     *  @param err error code.
     */
    void (*browsing_received)(struct bt_conn *conn, struct bt_avrcp_browsing_cmd *cmd, int err);
#endif
#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
    /** @brief received the browsing response.
     *
     *  This callback notifies the application that a avrcp browsing response is received.
	 *  The response is transfered in the AVRCP browsing l2cap channel.
     *
     *  @param conn connection object.
     *  @param rsp response message.
     *  @param err error code.
     */
    void (*browsing_rsp_received)(struct bt_conn *conn, struct bt_avrcp_browsing_rsp *rsp, int err);
#endif
};

/** @brief register callbacks to enable avrcp and monitor avrcp states.
 *
 *  @param cb The callback structure.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_register_callback(struct bt_avrcp_cb *cb);

/** @brief Create the AVRCP control l2cap connection.
 * 
 *  The sender will be notified by the registered callback (control_connected).
 *
 *  @param conn connection object.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_control_connect(struct bt_conn *conn);

/** @brief Release the AVRCP control l2cap connection.
 * 
 *  The sender will be notified by the registered callback (control_disconnected).
 *
 *  @param conn connection object.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_control_disconnect(struct bt_conn *conn);

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
/** @brief send unit info command.
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (control_received).
 *
 *  @param conn connection object.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_send_unit_info(struct bt_conn *conn);

/** @brief send subunit info command.
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (control_received).
 *
 *  @param conn connection object.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_send_subunit_info(struct bt_conn *conn);
#endif

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
/** @brief response info command.
 *
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (control_rsp_received).
 * 
 *  @param conn connection object.
 *  @param subunit sbuunit.
 *  @param subunit_type subunit type.
 *  @param conn transaction label.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_response_info(struct bt_conn *conn, uint8_t subunit, uint8_t subunit_type, uint8_t tl);
#endif

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
/** @brief send pass through command.
 *
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (control_received).
 * 
 *  @param conn connection object.
 *  @param op_id operation id.
 *  @param vendor_op_id vendor operation id.
 *  @param state_flag state flag.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_send_passthrough(struct bt_conn *conn, uint8_t op_id, uint8_t vendor_op_id, uint8_t state_flag);
#endif

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
/** @brief send pass through response.
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (control_rsp_received).
 * 
 *  @param conn connection object.
 *  @param rsp_type response type, for example: BT_AVRCP_RESPONSE_TYPE_ACCEPTED.
 *  @param cmd the received command message.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_response_passthrough(struct bt_conn *conn, uint8_t rsp_type, struct bt_avrcp_control_msg *cmd);
#endif

#if 0
int bt_avrcp_get_company_id_supported(struct bt_conn *conn);
int bt_avrcp_get_event_supported(struct bt_conn *conn);
int bt_avrcp_get_play_status(struct bt_conn *conn);
int bt_avrcp_register_notification(struct bt_conn *conn, uint8_t event, uint32_t playback_interval);
int bt_avrcp_request_continuing_rsp(struct bt_conn *conn, uint8_t pdu_id);
int bt_avrcp_abort_continuing_rsp(struct bt_conn *conn, uint8_t pdu_id);
int bt_avrcp_set_volume(struct bt_conn *conn, uint8_t volume);
int bt_avrcp_set_addressed_player(struct bt_conn *conn, uint16_t player_id);
#endif

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
/** @brief send vendor dependent command.
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (control_received).
 * 
 *  @param conn connection object.
 *  @param pdu_id pdu id.
 *  @param parameter parameter as follow:
 *            pdu_id                                    parameter
 *  BT_AVRCP_PDU_ID_GET_CAPABILITY                   cap_id(uint8_t)
 *  BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR     NULL
 *  BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL      attr_id(uint8_t)
 *  BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL   bt_avrcp_player_app_setting_attr_ids
 *  BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT  bt_avrcp_player_app_setting_attr_ids
 *  BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT   bt_avrcp_get_player_app_setting_value_text
 *  BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL       bt_avrcp_player_app_attr_values
 *  BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET      bt_avrcp_inform_displayable_char_set
 *  BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS            battery_status(uint8_t)
 *  BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE            bt_avrcp_get_element_attrs
 *  BT_AVRCP_PDU_ID_GET_PLAY_STATUS                  NULL
 *  BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION bt_avrcp_register_notification()+event_id(uint8_t) bt_avrcp_event_rsp
 *  BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE      pdu_id(uint8_t)
 *  BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE        pdu_id(uint8_t)
 *  BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME              volume(uint8_t)
 *  BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER             player id(uint16_t)
 *  BT_AVRCP_PDU_ID_PLAY_ITEMS                       struct bt_avrcp_play_item
 *  BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING               struct bt_avrcp_add_to_now_playing
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_send_vendor_dependent(struct bt_conn *conn, uint8_t pdu_id, void *parameter);
#endif

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
/** @brief send vendor dependent response.
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (control_rsp_received).
 * 
 *  @param conn connection object.
 *  @param pdu_id pdu id.
 *  @param tl transaction label.
 *  @param response_type response type, for example: BT_AVRCP_RESPONSE_TYPE_ACCEPTED.
 *  @param rsp_param parameter as follow:
 *            pdu_id                                    rsp_param
 *  BT_AVRCP_PDU_ID_GET_CAPABILITY                   bt_avrcp_capability_company_id or bt_avrcp_capability_events_supported
 *  BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR     bt_avrcp_player_app_setting_attr_ids
 *  BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL      bt_avrcp_player_app_setting_values
 *  BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL   bt_avrcp_player_app_attr_values
 *  BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT  bt_avrcp_player_get_txt_rsp
 *  BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT   bt_avrcp_player_get_txt_rsp
 *  BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL       NULL
 *  BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET      NULL
 *  BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS            NULL
 *  BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE            bt_avrcp_player_get_element_attr_rsp
 *  BT_AVRCP_PDU_ID_GET_PLAY_STATUS                  bt_avrcp_play_status_rsp
 *  BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION            bt_avrcp_event_rsp
 *  BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE      requested messgae response
 *  BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE        NULL
 *  BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME              volume(uint8_t)
 *  BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER             status(uint8_t)
 *  BT_AVRCP_PDU_ID_PLAY_ITEMS                       status(uint8_t)
 *  BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING               status(uint8_t)
 *  @param param_len rsp_param's data length.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_response_vendor_dependent(
    struct bt_conn *conn, uint8_t pdu_id, uint8_t tl,
	uint8_t response_type, void *rsp_param, uint16_t param_len);
#endif

#if (defined(CONFIG_BT_AVRCP_BROWSING) && ((CONFIG_BT_AVRCP_BROWSING) > 0U))
/** @brief Create the AVRCP browsing l2cap connection.
 * 
 *  The sender will be notified by the registered callback (browsing_connected).
 *
 *  @param conn connection object.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_browsing_connect(struct bt_conn *conn);

/** @brief Release the AVRCP browsing l2cap connection.
 * 
 *  The sender will be notified by the registered callback (browsing_disconnected).
 *
 *  @param conn connection object.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_browsing_disconnect(struct bt_conn *conn);

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
/** @brief get folder items (browsing).
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (browsing_received).
 *
 *  @param conn connection object.
 *  @param param parameter.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_get_folder_items(struct bt_conn *conn, struct bt_avrcp_get_folder_items_cmd *param);

/** @brief change path (browsing).
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (browsing_received).
 *
 *  @param conn connection object.
 *  @param param parameter.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_change_path(struct bt_conn *conn, struct bt_avrcp_change_path_cmd *param);

/** @brief change path (browsing).
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (browsing_received).
 *
 *  @param conn connection object.
 *  @param param parameter.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_set_borwsed_player(struct bt_conn *conn, uint16_t player_id);

/** @brief get items attribute (browsing).
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (browsing_received).
 *
 *  @param conn connection object.
 *  @param param parameter.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_get_items_attribute(struct bt_conn *conn, struct bt_avrcp_get_item_attrs_cmd *param);

/** @brief search (browsing).
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (browsing_received).
 *
 *  @param conn connection object.
 *  @param param parameter.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_search(struct bt_conn *conn, struct bt_avrcp_search_cmd *param);

/** @brief get total number of items (browsing).
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (browsing_received).
 *
 *  @param conn connection object.
 *  @param param parameter.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_get_total_num_of_items(struct bt_conn *conn, uint8_t scope);
#endif

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
/** @brief response browsing command (browsing).
 * 
 *  The sender will be notified by the registered callback (send_result).
 *  The receiver will be notified by the registered callback (browsing_rsp_received).
 *
 *  @param conn connection object.
 *  @param pdu_id pdu id.
 *  @param tl transaction label.
 *  @param rsp_param respone parameter.
 *           pdu_id                                rsp_param
 *  BT_AVRCP_PDU_ID_GET_FOLDER_ITEMS         bt_avrcp_get_folder_items_rsp
 *  BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER       bt_avrcp_set_browsed_player_rsp
 *  BT_AVRCP_PDU_ID_CHANGE_PATH              bt_avrcp_change_path_rsp
 *  BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES      bt_avrcp_get_item_attrs_rsp
 *  BT_AVRCP_PDU_ID_SEARCH                   bt_avrcp_search_rsp
 *  BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS      bt_avrcp_get_total_num_of_items_rsp
 *  
 *  @param param_len rsp_param data length.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_response_browsing(struct bt_conn *conn, uint8_t pdu_id, uint8_t tl, void *rsp_param, uint16_t param_len);
#endif
#endif

#if (defined(CONFIG_BT_AVRCP_COVER_ART) && ((CONFIG_BT_AVRCP_COVER_ART) > 0U))

/** @brief avrcp get image property (cover art) */
struct bt_avrcp_get_image_property
{
    uint8_t *image_handle;
    uint16_t image_handle_len;
    uint8_t wait;
};

/** @brief avrcp get image property response (cover art) */
struct bt_avrcp_get_image_properity_rsp
{
    uint8_t *data;
    uint16_t length;
};

/** @brief avrcp get image (cover art) */
struct bt_avrcp_get_image
{
    uint8_t *image_handle;
    uint8_t *image_descriptor_data;
    uint16_t image_handle_len;
    uint16_t image_descriptor_data_len;
    uint8_t wait;
};

/** @brief avrcp get image response (cover art) */
struct bt_avrcp_get_image_rsp
{
    uint32_t image_file_size;
    uint8_t *data;
    uint16_t length;
};

/** @brief avrcp get linked thumbnail (cover art) */
struct bt_avrcp_get_linked_thumbnail
{
    uint8_t *image_handle;
    uint16_t image_handle_len;
    uint8_t wait;
};

/** @brief avrcp get linked thumbnail response (cover art) */
struct bt_avrcp_get_linked_thumbnail_rsp
{
    uint8_t *data;
    uint16_t length;
};

/** @brief avrcp cover art connect (cover art) */
struct bt_avrcp_cover_art_connect
{
    uint16_t max_recv_size;
    /* uint16_t l2cap_lpsm; */
    uint16_t l2cap_rpsm;
};

/** @brief avrcp cover art connect response (cover art) */
struct bt_avrcp_cover_art_connect_rsp
{
    uint16_t max_recv_size;
};

/** @brief avrcp cover art command (cover art) */
enum
{
    BT_AVRCP_COVER_ART_CONNECT,
    BT_AVRCP_COVER_ART_DISCONNECT,
    BT_AVRCP_COVER_ART_GET_PROP,
    BT_AVRCP_COVER_ART_GET_IMAGE,
    BT_AVRCP_COVER_ART_GET_THUMB,
    BT_AVRCP_COVER_ART_ABORT,
};

/** @brief avrcp cover art command message (cover art) */
struct bt_avrcp_cover_art_cmd
{
	/** command, for example: BT_AVRCP_COVER_ART_GET_PROP */
    uint8_t cmd;
    union
    {
        struct bt_avrcp_cover_art_connect connect;
        struct bt_avrcp_get_image_property get_prop;
        struct bt_avrcp_get_image get_image;
        struct bt_avrcp_get_linked_thumbnail get_thumb;
    };
};

/** @brief avrcp cover art response message (cover art) */
struct bt_avrcp_cover_art_rsp
{
	/** command, for example: BT_AVRCP_COVER_ART_GET_PROP */
    uint8_t cmd;
    uint8_t response;
    union
    {
        struct bt_avrcp_cover_art_connect_rsp connect;
        struct bt_avrcp_get_image_properity_rsp get_prop;
        struct bt_avrcp_get_image_rsp get_image;
        struct bt_avrcp_get_linked_thumbnail_rsp get_thumb;
    };
};

/** @brief avrcp cover art callbacks (cover art) */
struct bt_avrcp_cover_art_cb
{
    /** @brief avrcp cover art connection has been established.
     *
     *  This callback notifies the application of avrcp cover art connection.
     *
     *  @param conn connection object.
     *  @param err error code.
     */
    void (*connected)(uint8_t handle, struct bt_conn *conn, int err);
    /** @brief avrcp cover art connection has been disconnected.
     *
     *  This callback notifies the application that of avrcp cover art disconnection.
     *  @param err error code.
     *
     *  @param conn connection object.
     */
    void (*disconnected)(uint8_t handle, int err);
#if (defined(CONFIG_BT_AVRCP_COVER_ART_RESPONDER) && ((CONFIG_BT_AVRCP_COVER_ART_RESPONDER) > 0U))
    /** @brief received the cover art command.
     *
     *  This callback notifies the application that a avrcp cover art command is received.
	 *  The command is transfered in the AVRCP cover art l2cap channel.
     *
     *  @param conn connection object.
     *  @param cmd command message.
     *  @param err error code.
     */
    void (*cmd_received)(uint8_t handle, struct bt_avrcp_cover_art_cmd *cmd, int err);
#endif
#if (defined(CONFIG_BT_AVRCP_COVER_ART_INITIATOR) && ((CONFIG_BT_AVRCP_COVER_ART_INITIATOR) > 0U))
    /** @brief received the cover art response.
     *
     *  This callback notifies the application that a avrcp cover art response is received.
	 *  The response is transfered in the AVRCP cover art l2cap channel.
     *
     *  @param conn connection object.
     *  @param rsp response.
     *  @param err error code.
     */
    void (*rsp_received)(uint8_t handle, struct bt_avrcp_cover_art_rsp *rsp, int err);
#endif
};

/** @brief register cover art callbacks.
 *
 *  @param cb callback.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_register_cover_art_cb(struct bt_avrcp_cover_art_cb *cb);

#if (defined(CONFIG_BT_AVRCP_COVER_ART_INITIATOR) && ((CONFIG_BT_AVRCP_COVER_ART_INITIATOR) > 0U))
/** @brief start one cover art initiator.
 *
 *  @param[out] handle the handle, all other cover art APIs use this handle as parameter. 
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_cover_art_start_initiator(uint8_t *handle);

/** @brief stop cover art initiator.
 *
 *  @param handle the handle.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_cover_art_stop_initiator(uint8_t handle);
#endif

#if (defined(CONFIG_BT_AVRCP_COVER_ART_RESPONDER) && ((CONFIG_BT_AVRCP_COVER_ART_RESPONDER) > 0U))
/** @brief start cover art responder.
 *
 *  @param[out] handle the handle, , all other cover art APIs use this handle as parameter. 
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_cover_art_start_responder(uint8_t *handle);

/** @brief stop cover art responder.
 *
 *  @param handle the handle.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_cover_art_stop_responder(uint8_t handle);
#endif

#if (defined(CONFIG_BT_AVRCP_COVER_ART_INITIATOR) && ((CONFIG_BT_AVRCP_COVER_ART_INITIATOR) > 0U))
/** @brief create cover art connection.
 *
 *  The sender will be notified by the registered callback (connected & rsp_received).
 *  The receiver will be notified by the registered callback (connected & cmd_received).
 * 
 *  @param handle the handle.
 *  @param conn connection object.
 *  @param cmd connection command parameter.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_cover_art_connect(uint8_t handle, struct bt_conn *conn, struct bt_avrcp_cover_art_connect *cmd);

/** @brief release cover art connection.
 *
 *  The sender will be notified by the registered callback (connected & rsp_received).
 *  The receiver will be notified by the registered callback (connected & cmd_received).
 * 
 *  @param handle the handle.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_cover_art_disconnect(uint8_t handle);

/** @brief get image property (cover art).
 *
 *  The sender will be notified by the registered callback (rsp_received).
 *  The receiver will be notified by the registered callback (cmd_received).
 * 
 *  @param handle the handle.
 *  @param param parameter.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_get_image_property(uint8_t handle, struct bt_avrcp_get_image_property *param);

/** @brief get image (cover art).
 *
 *  The sender will be notified by the registered callback (rsp_received).
 *  The receiver will be notified by the registered callback (cmd_received).
 * 
 *  @param handle the handle.
 *  @param param parameter.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_get_image(uint8_t handle, struct bt_avrcp_get_image *param);

/** @brief get linked thumbnail (cover art).
 *
 *  The sender will be notified by the registered callback (rsp_received).
 *  The receiver will be notified by the registered callback (cmd_received).
 * 
 *  @param handle the handle.
 *  @param param parameter.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_get_linked_thumbnail(uint8_t handle, struct bt_avrcp_get_linked_thumbnail *param);

/** @brief abort (cover art).
 *
 *  The sender will be notified by the registered callback (rsp_received).
 *  The receiver will be notified by the registered callback (cmd_received).
 * 
 *  @param handle the handle.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_abort(uint8_t handle);

/** @brief request peer to continue responsing data (cover art).
 *
 *  The sender will be notified by the registered callback (rsp_received).
 *  The receiver will be notified by the registered callback (cmd_received).
 * 
 *  @param handle the handle.
 *  @param wait ture: not 0, faluse: 0.
 *  @param rsp the received respone message.
 *
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_send_request(uint8_t handle, uint8_t wait, struct bt_avrcp_cover_art_rsp *rsp);
#endif

#if (defined(CONFIG_BT_AVRCP_COVER_ART_RESPONDER) && ((CONFIG_BT_AVRCP_COVER_ART_RESPONDER) > 0U))
/** @brief response cover art command (cover art).
 *
 *  The receiver will be notified by the registered callback (rsp_received).
 * 
 *  @param handle the handle.
 *  @param cmd the command, for example: BT_AVRCP_COVER_ART_GET_PROP.
 *  @param response for example: BT_AVRCP_CA_SUCCESS_RSP.
 *  @param rsp parameter.
 *         cmd                                   rsp
 *  BT_AVRCP_COVER_ART_CONNECT      NULL
 *  BT_AVRCP_COVER_ART_DISCONNECT   NULL
 *  BT_AVRCP_COVER_ART_GET_PROP     struct bt_avrcp_get_image_properity_rsp
 *  BT_AVRCP_COVER_ART_GET_IMAGE    struct bt_avrcp_get_image_rsp get_image
 *  BT_AVRCP_COVER_ART_GET_THUMB    struct bt_avrcp_get_linked_thumbnail_rsp
 *  BT_AVRCP_COVER_ART_ABORT        NULL
 *  @return 0 in case of success and error code in case of error.
 */
int bt_avrcp_response_cover_art(uint8_t handle, uint8_t cmd, uint8_t response, void *rsp);
#endif

#endif

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_AVRCP_H_ */
