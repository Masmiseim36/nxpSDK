/** @file
 * @brief Bluetooth AVRCP shell module
 *
 * Provide some Bluetooth shell commands that can be useful to applications.
 */

/*
 * Copyright (c) 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno/errno.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/byteorder.h>
#include <porting.h>

#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/sdp.h>
#include <bluetooth/avrcp.h>
#include "fsl_shell.h"
#include "shell_bt.h"

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))

uint8_t search_uid;
uint8_t now_playing_uid;
#define FOLDER_UID    2
#define DEF_DATA(len) __aligned(4) uint8_t data[len];

#define APPL_AVRCP_MAX_PLAYER_ATTR_COUNT     4
#define APPL_AVRCP_MAX_PLAYER_ATTR_VAL_COUNT 4
#define APPL_AVRCP_MAX_ATTR_COUNT            8
#define APPL_AVRCP_MAX_PLAYER_EVENT_COUNT    13

static void avrcp_auto_test(uint8_t print);

#define AVRCP_TG_SUPPORTED_FEATURES (0x01ffu)
#define AVRCP_CT_SUPPORTED_FEATURES (0x03Cfu)

static struct bt_sdp_attribute avrcp_tg_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST( //09
        BT_SDP_ATTR_SVCLASS_ID_LIST, //00 01
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_AV_REMOTE_TARGET_SVCLASS) //11 0C
        },
        )
    ),
    BT_SDP_LIST(//09
        BT_SDP_ATTR_PROTO_DESC_LIST, //00 04
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 16),//35 10
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) // 01 00
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(BT_UUID_AVCTP_VAL) // 00 17
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_UUID_AVCTP_VAL) // 00 17
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0X0104u) //AVCTP version: 01 04
            },
            )
        },
        )
    ),
    BT_SDP_LIST(//09
        BT_SDP_ATTR_PROFILE_DESC_LIST, //00 09
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_AV_REMOTE_SVCLASS) //11 0e
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0106U) //01 06
            },
            )
        },
        )
    ),
    BT_SDP_LIST(//09
        BT_SDP_ATTR_ADD_PROTO_DESC_LIST, //00 0d
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 0x21), //35 21
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 0x10), //35 10
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
                BT_SDP_DATA_ELEM_LIST(
                {
                    BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                    BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) //01 00
                },
                {
                    BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                    BT_SDP_ARRAY_16(0x001bU) // 00 1b
                },
                )
            },
            {
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
                BT_SDP_DATA_ELEM_LIST(
                {
                    BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                    BT_SDP_ARRAY_16(BT_UUID_AVCTP_VAL) //00 17
                },
                {
                    BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                    BT_SDP_ARRAY_16(0x0104U) // 01 04
                },
                )
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 0x0d), //35 0d
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
                BT_SDP_DATA_ELEM_LIST(
                {
                    BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                    BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) //01 00
                },
                {
                    BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                    BT_SDP_ARRAY_16(BT_AVRCP_COVER_ART_PSM) // 10 05
                },
                )
            },
            {
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 03
                BT_SDP_DATA_ELEM_LIST(
                {
                    BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                    BT_SDP_ARRAY_16(BT_UUID_OBEX_VAL) //00 08
                },
                )
            },
            )
        },
        )
    ),
    BT_SDP_SUPPORTED_FEATURES(AVRCP_TG_SUPPORTED_FEATURES),
};

static struct bt_sdp_record avrcp_tg_rec = BT_SDP_RECORD(avrcp_tg_attrs);

static struct bt_sdp_attribute avrcp_ct_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST( //09
        BT_SDP_ATTR_SVCLASS_ID_LIST, //00 01
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_AV_REMOTE_CONTROLLER_SVCLASS) //11 0F
        },
        )
    ),
    BT_SDP_LIST(//09
        BT_SDP_ATTR_PROTO_DESC_LIST, //00 04
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 16),//35 10
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) // 01 00
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(BT_UUID_AVCTP_VAL) // 00 17
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_UUID_AVCTP_VAL) // 00 17
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0X0104u) //AVCTP version: 01 04
            },
            )
        },
        )
    ),
    BT_SDP_LIST(//09
        BT_SDP_ATTR_PROFILE_DESC_LIST, //00 09
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_AV_REMOTE_SVCLASS) //11 0e
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0106U) //01 06
            },
            )
        },
        )
    ),
    BT_SDP_SUPPORTED_FEATURES(AVRCP_CT_SUPPORTED_FEATURES),
};

static struct bt_sdp_record avrcp_ct_rec = BT_SDP_RECORD(avrcp_ct_attrs);

void avrcp_op_id_print(uint8_t op_id)
{
    switch (op_id)
    {
        case BT_AVRCP_OP_ID_SELECT:
            shell_print(ctx_shell, "    Operation ID = SELECT");
            break;

        case BT_AVRCP_OP_ID_UP:
            shell_print(ctx_shell, "    Operation ID = UP");
            break;

        case BT_AVRCP_OP_ID_DOWN:
            shell_print(ctx_shell, "    Operation ID = DOWN");
            break;

        case BT_AVRCP_OP_ID_LEFT:
            shell_print(ctx_shell, "    Operation ID = LEFT");
            break;

        case BT_AVRCP_OP_ID_RIGHT:
            shell_print(ctx_shell, "    Operation ID = RIGHT");
            break;

        case BT_AVRCP_OP_ID_RIGHT_UP:
            shell_print(ctx_shell, "    Operation ID = RIGHT UP");
            break;

        case BT_AVRCP_OP_ID_RIGHT_DOWN:
            shell_print(ctx_shell, "    Operation ID = RIGHT DOWN");
            break;

        case BT_AVRCP_OP_ID_LEFT_UP:
            shell_print(ctx_shell, "    Operation ID = LEFT UP");
            break;

        case BT_AVRCP_OP_ID_LEFT_DOWN:
            shell_print(ctx_shell, "    Operation ID = LEFT DOWN");
            break;

        case BT_AVRCP_OP_ID_ROOT_MENU:
            shell_print(ctx_shell, "    Operation ID = ROOT MENU");
            break;

        case BT_AVRCP_OP_ID_SETUP_MENU:
            shell_print(ctx_shell, "    Operation ID = SETUP MENU");
            break;

        case BT_AVRCP_OP_ID_CONTENTS_MENU:
            shell_print(ctx_shell, "    Operation ID = CONTENTS MENU");
            break;

        case BT_AVRCP_OP_ID_FAVORITE_MENU:
            shell_print(ctx_shell, "    Operation ID = FAVORITE MENU");
            break;

        case BT_AVRCP_OP_ID_EXIT:
            shell_print(ctx_shell, "    Operation ID = EXIT");
            break;

        case BT_AVRCP_OP_ID_0:
            shell_print(ctx_shell, "    Operation ID = 0");
            break;

        case BT_AVRCP_OP_ID_1:
            shell_print(ctx_shell, "    Operation ID = 1");
            break;

        case BT_AVRCP_OP_ID_2:
            shell_print(ctx_shell, "    Operation ID = 2");
            break;

        case BT_AVRCP_OP_ID_3:
            shell_print(ctx_shell, "    Operation ID = 3");
            break;

        case BT_AVRCP_OP_ID_4:
            shell_print(ctx_shell, "    Operation ID = 4");
            break;

        case BT_AVRCP_OP_ID_5:
            shell_print(ctx_shell, "    Operation ID = 5");
            break;

        case BT_AVRCP_OP_ID_6:
            shell_print(ctx_shell, "    Operation ID = 6");
            break;

        case BT_AVRCP_OP_ID_7:
            shell_print(ctx_shell, "    Operation ID = 7");
            break;

        case BT_AVRCP_OP_ID_8:
            shell_print(ctx_shell, "    Operation ID = 8");
            break;

        case BT_AVRCP_OP_ID_9:
            shell_print(ctx_shell, "    Operation ID = 9");
            break;

        case BT_AVRCP_OP_ID_DOT:
            shell_print(ctx_shell, "    Operation ID = DOT");
            break;

        case BT_AVRCP_OP_ID_ENTER:
            shell_print(ctx_shell, "    Operation ID = ENTER");
            break;

        case BT_AVRCP_OP_ID_CLEAR:
            shell_print(ctx_shell, "    Operation ID = CLEAR");
            break;

        case BT_AVRCP_OP_ID_CHANNEL_UP:
            shell_print(ctx_shell, "    Operation ID = CHANNEL UP");
            break;

        case BT_AVRCP_OP_ID_CHANNEL_DOWN:
            shell_print(ctx_shell, "    Operation ID = CHANNEL DOWN");
            break;

        case BT_AVRCP_OP_ID_PREVIOUS_CHANNEL:
            shell_print(ctx_shell, "    Operation ID = PREVIOUS CHANNEL");
            break;

        case BT_AVRCP_OP_ID_SOUND_SELECT:
            shell_print(ctx_shell, "    Operation ID = SOUND SELECT");
            break;

        case BT_AVRCP_OP_ID_INPUT_SELECT:
            shell_print(ctx_shell, "    Operation ID = INPUT SELECT");
            break;

        case BT_AVRCP_OP_ID_DISPLAY_INFORMATION:
            shell_print(ctx_shell, "    Operation ID = DISPLAY INFORMATION");
            break;

        case BT_AVRCP_OP_ID_HELP:
            shell_print(ctx_shell, "    Operation ID = HELP");
            break;

        case BT_AVRCP_OP_ID_PAGE_UP:
            shell_print(ctx_shell, "    Operation ID = PAGE UP");
            break;

        case BT_AVRCP_OP_ID_PAGE_DOWN:
            shell_print(ctx_shell, "    Operation ID = PAGE DOWN");
            break;

        case BT_AVRCP_OP_ID_POWER:
            shell_print(ctx_shell, "    Operation ID = POWER");
            break;

        case BT_AVRCP_OP_ID_VOLUME_UP:
            shell_print(ctx_shell, "    Operation ID = VOLUME UP");
            break;

        case BT_AVRCP_OP_ID_VOLUME_DOWN:
            shell_print(ctx_shell, "    Operation ID = VOLUME DOWN");
            break;

        case BT_AVRCP_OP_ID_MUTE:
            shell_print(ctx_shell, "    Operation ID = MUTE");
            break;

        case BT_AVRCP_OP_ID_PLAY:
            shell_print(ctx_shell, "    Operation ID = PLAY");
            break;

        case BT_AVRCP_OP_ID_STOP:
            shell_print(ctx_shell, "    Operation ID = STOP");
            break;

        case BT_AVRCP_OP_ID_PAUSE:
            shell_print(ctx_shell, "    Operation ID = PAUSE");
            break;

        case BT_AVRCP_OP_ID_RECORD:
            shell_print(ctx_shell, "    Operation ID = RECORD");
            break;

        case BT_AVRCP_OP_ID_REWIND:
            shell_print(ctx_shell, "    Operation ID = REWIND");
            break;

        case BT_AVRCP_OP_ID_FAST_FORWARD:
            shell_print(ctx_shell, "    Operation ID = FAST FORWARD");
            break;

        case BT_AVRCP_OP_ID_EJECT:
            shell_print(ctx_shell, "    Operation ID = EJECT");
            break;

        case BT_AVRCP_OP_ID_FORWARD:
            shell_print(ctx_shell, "    Operation ID = FORWARD");
            break;

        case BT_AVRCP_OP_ID_BACKWARD:
            shell_print(ctx_shell, "    Operation ID = BACKWARD");
            break;

        case BT_AVRCP_OP_ID_ANGLE:
            shell_print(ctx_shell, "    Operation ID = ANGLE");
            break;

        case BT_AVRCP_OP_ID_SUBPICTURE:
            shell_print(ctx_shell, "    Operation ID = SUBPICTURE");
            break;

        case BT_AVRCP_OP_ID_F1:
            shell_print(ctx_shell, "    Operation ID = F1");
            break;

        case BT_AVRCP_OP_ID_F2:
            shell_print(ctx_shell, "    Operation ID = F2");
            break;

        case BT_AVRCP_OP_ID_F3:
            shell_print(ctx_shell, "    Operation ID = F3");
            break;

        case BT_AVRCP_OP_ID_F4:
            shell_print(ctx_shell, "    Operation ID = F4");
            break;

        case BT_AVRCP_OP_ID_F5:
            shell_print(ctx_shell, "    Operation ID = F5");
            break;

        case BT_AVRCP_OP_ID_VENDOR_UNIQUE:
            shell_print(ctx_shell, "    Operation ID = VENDOR UNIQUE");
            break;

        default:
            shell_print(ctx_shell, "    Operation ID = ???? (0x%02X)", op_id);
            break;
    }

    return;
}

void avrcp_opcode_print(uint8_t opcode)
{
    switch (opcode)
    {
        case BT_AVRCP_OPCODE_VENDOR_DEPENDENT:
            shell_print(ctx_shell, "    Opcode = VENDOR DEPENDENT");
            break;

        case BT_AVRCP_OPCODE_UNIT_INFO:
            shell_print(ctx_shell, "    Opcode = UNIT INFO");
            break;

        case BT_AVRCP_OPCODE_SUBUNIT_INFO:
            shell_print(ctx_shell, "    Opcode = SUBUNIT INFO");
            break;

        case BT_AVRCP_OPCODE_PASS_THROUGH:
            shell_print(ctx_shell, "    Opcode = PASS THROUGH");
            break;

        default:
            shell_print(ctx_shell, "    Opcode = ???? (0x%02X)", opcode);
            break;
    }

    return;
}

void avrcp_response_type_print(uint8_t rsp_type)
{
    switch (rsp_type)
    {
        case BT_AVRCP_RESPONSE_TYPE_NOT_IMPLEMENTED:
            shell_print(ctx_shell, "    Response Type = NOT IMPLEMENTED");
            break;

        case BT_AVRCP_RESPONSE_TYPE_ACCEPTED:
            shell_print(ctx_shell, "    Response Type = ACCEPTED");
            break;

        case BT_AVRCP_RESPONSE_TYPE_REJECTED:
            shell_print(ctx_shell, "    Response Type = REJECTED");
            break;

        case BT_AVRCP_RESPONSE_TYPE_INTERIM:
            shell_print(ctx_shell, "    Response Type = INTERIM");
            break;

        case BT_AVRCP_RESPONSE_TYPE_STABLE:
            shell_print(ctx_shell, "    Response Type = STABLE");
            break;

        default:
            shell_print(ctx_shell, "    Response Type = ???? (0x%02X)", rsp_type);
            break;
    }

    return;
}

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))

uint8_t event_ids[] = {BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED,
                       BT_AVRCP_EVENT_TRACK_CHANGED,
                       BT_AVRCP_EVENT_TRACK_REACHED_END,
                       BT_AVRCP_EVENT_TRACK_REACHED_START,
                       BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED,
                       BT_AVRCP_EVENT_BATT_STATUS_CHANGED,
                       BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED,
                       BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED,
                       BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED,
                       BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED,
                       BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED,
                       BT_AVRCP_EVENT_UIDS_CHANGED,
                       BT_AVRCP_EVENT_VOLUME_CHANGED};

uint8_t registered_events[13][2];

void register_player_event(uint8_t event_id, uint8_t tl)
{
    if (event_id <= 13)
    {
        registered_events[event_id - 1][0] = 0x01;
        registered_events[event_id - 1][1] = tl;
    }
}

void avrcp_target_rsp_notify_cmd_interim(
    struct bt_conn *conn, struct bt_avrcp_control_msg *msg, uint8_t *data, uint16_t *rsp_len, uint8_t *response_type)
{
    struct bt_avrcp_event_rsp *rsp = (struct bt_avrcp_event_rsp *)data;
    struct bt_avrcp_vendor *vendor_msg       = &msg->vendor;
    uint8_t event_id                         = vendor_msg->parameter;
    *rsp_len                                 = sizeof(*rsp);

    rsp->event_id = event_id;
    switch (event_id)
    {
        case BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED<0x%x>.", event_id);
            rsp->play_status = 0;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_TRACK_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_TRACK_CHANGED<0x%x>.", event_id);
            memset(rsp->identifier, 0, 8u);
            rsp->identifier[0] = 1u;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_TRACK_REACHED_END:
        case BT_AVRCP_EVENT_TRACK_REACHED_START: /* Fall Through */
            shell_print(ctx_shell, "    Event-ID -> %s<0x%x>.",
                        event_id == BT_AVRCP_EVENT_TRACK_REACHED_END ? "BT_AVRCP_EVENT_TRACK_REACHED_END" :
                                                                       "AVRCP_EVENT_TRACK_REACHED_START",
                        event_id);
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED<0x%x>.", event_id);
            rsp->playback_pos = 1000;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_BATT_STATUS_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_BATT_STATUS_CHANGED<0x%x>.", event_id);
            rsp->battery_status = 0;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED<0x%x>.", event_id);
            rsp->system_status = 0;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:
        {
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED<0x%x>.", event_id);
            rsp->setting_changed.num_of_attr           = 1;
            rsp->setting_changed.attr_vals[0].attr_id  = 1;
            rsp->setting_changed.attr_vals[0].value_id = 1;

            *rsp_len = sizeof(rsp) + sizeof(struct bt_avrcp_player_attr_value);
            register_player_event(event_id, msg->header.tl);
            break;
        }

#ifdef AVRCP_1_4
        case BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED<0x%x>.", event_id);
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED<0x%x>.", event_id);
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED<0x%x>.", event_id);
            rsp->addressed_player_changed.player_id   = 1;
            rsp->addressed_player_changed.uid_counter = 1;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_UIDS_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_UIDS_CHANGED<0x%x>.", event_id);
            rsp->uid_counter = 1;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_VOLUME_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_VOLUME_CHANGED<0x%x>.", event_id);
            rsp->absolute_volume = 10;
            register_player_event(event_id, msg->header.tl);
            break;

#endif /* AVRCP_1_4 */
        default:
            shell_print(ctx_shell, "    Event-ID -> ??? ");
            break;
    }

    /* Check Event ID */
    if (event_id > BT_AVRCP_EVENT_VOLUME_CHANGED)
    {
        *response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
        data[0]        = BT_AVRCP_METADATA_ERROR_INVALID_PARAMETER;
        *rsp_len       = 1;
    }

    return;
}

static void avrcp_target_handle_vendor_dependent_msg(struct bt_conn *conn, struct bt_avrcp_control_msg *msg)
{
    DEF_DATA(128u);
    uint8_t rj_data = BT_AVRCP_METADATA_ERROR_INVALID_PARAMETER;
    void *rsp_param;
    uint16_t rsp_len;
    uint8_t response_type              = BT_AVRCP_RESPONSE_TYPE_STABLE;
    struct bt_avrcp_vendor *vendor_msg = &msg->vendor;

    /* Command Type */
    shell_print(ctx_shell, "    Command Type - ");
    switch (msg->header.ctype_response)
    {
        case BT_AVRCP_COMMAND_TYPE_CONTROL:
            shell_print(ctx_shell, "CONTROL");
            break;

        case BT_AVRCP_COMMAND_TYPE_STATUS:
            shell_print(ctx_shell, "STATUS");
            break;

        case BT_AVRCP_COMMAND_TYPE_NOTIFY:
            shell_print(ctx_shell, "NOTIFY");
            break;

        default:
            shell_print(ctx_shell, "UNKNOWN");
            break;
    }

    shell_print(ctx_shell, "    Param Length: 0x%04x", vendor_msg->parameter_len);

    shell_print(ctx_shell, "");
    rsp_param = NULL;
    rsp_len   = 0u;
    switch (vendor_msg->pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_CAPABILITY:
        {
            uint8_t cap_id;
            cap_id = vendor_msg->parameter;

            shell_print(ctx_shell, "    PDU-ID -> Get Capability");
            shell_print(ctx_shell, "     - Capability ID  = 0x%02x", cap_id);

            if (cap_id == BT_AVRCP_CAP_ID_COMPANY_ID)
            {
                struct bt_avrcp_capability_company_id *rsp;
                rsp = (struct bt_avrcp_capability_company_id *)&data[0];

                rsp->capability_id    = cap_id;
                rsp->capability_count = 1;
                sys_put_be24(0x1958u, (uint8_t *)&rsp->company_ids[0]);

                rsp_param = rsp;
                rsp_len   = 5u;
            }
            else if (cap_id == BT_AVRCP_CAP_ID_EVENTS_SUPPORTED)
            {
                struct bt_avrcp_capability_events_supported *rsp;
                rsp = (struct bt_avrcp_capability_events_supported *)&data[0];

                rsp->capability_id    = cap_id;
                rsp->capability_count = 13u;
                for (uint8_t index = 0; index < 13u; index++)
                {
                    rsp->event_ids[index] = event_ids[index];
                }

                rsp_param = &data[0];
                rsp_len   = 15u;
            }
            else
            {
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
                response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
            }
            break;
        }
        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
        {
            struct bt_avrcp_player_app_setting_attr_ids *rsp;
            rsp = (struct bt_avrcp_player_app_setting_attr_ids *)&data[0];

            shell_print(ctx_shell, "    PDU-ID -> List Player Appl. Setting Attributes<0x%x>.", vendor_msg->pdu_id);

            rsp->num_of_attr = 1;
            rsp->attr_ids[0] = 1;

            rsp_param = &data[0];
            rsp_len   = 2;
            break;
        }
        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
        {
            struct bt_avrcp_player_app_setting_values *rsp;
            uint8_t attr_id = vendor_msg->parameter;

            shell_print(ctx_shell, "    PDU-ID -> List Player Appl. Setting Values<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    Attribute ID  = 0x%02x", attr_id);

            /* Validate Attribute ID */
            if (attr_id != 1)
            {
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
                response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
                break;
            }

            rsp               = (struct bt_avrcp_player_app_setting_values *)&data[0];
            rsp->num_of_value = 2;
            rsp->value_ids[0] = 1;
            rsp->value_ids[1] = 2;
            rsp_param         = &data[0];
            rsp_len           = 3;
            break;
        }
        case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
        {
            struct bt_avrcp_player_app_setting_attr_ids *cmd =
                &vendor_msg->player_attr_ids;
            struct bt_avrcp_player_app_attr_values *rsp;

            shell_print(ctx_shell, "    PDU-ID -> Get Current Player Appl. Setting Value<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    No. of Attrbutes: %d", cmd->num_of_attr);

            if (cmd->num_of_attr > 1)
            {
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
                response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
                break;
            }

            rsp                        = (struct bt_avrcp_player_app_attr_values *)&data[0];
            rsp->num_of_attr           = 1;
            rsp->attr_vals[0].attr_id  = 1;
            rsp->attr_vals[0].value_id = 1;
            rsp_param                  = &data[0];
            rsp_len                    = 3u;
            break;
        }
        case BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL:
        {
            struct bt_avrcp_player_app_attr_values *cmd =
                &vendor_msg->player_attr_values;

            shell_print(ctx_shell, "    PDU-ID -> Set Player Appl. Setting Value<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    No. of Attrbutes: %d", cmd->num_of_attr);

            if (cmd->num_of_attr > 1)
            {
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
                response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
                break;
            }

            rsp_param = NULL;
            rsp_len   = 0;
            break;
        }
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT:
        {
            struct bt_avrcp_player_app_setting_attr_ids *cmd =
                &vendor_msg->player_attr_ids;
            struct bt_avrcp_player_get_txt_rsp *rsp;

            if (cmd->num_of_attr > 1)
            {
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
                response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
                break;
            }

            rsp = (struct bt_avrcp_player_get_txt_rsp *)&data[0];
            shell_print(ctx_shell, "    PDU-ID -> Get Player Appl. Setting Attr. Text<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    No. of Attrbutes: %d", cmd->num_of_attr);
            rsp->num_of_id              = 1;
            rsp->texts[0].attr_value_id = 1;
            rsp->texts[0].char_set      = 0x6a;
            rsp->texts[0].string_len    = 9;
            rsp->texts[0].string        = (uint8_t *)"EQUALIZER";
            rsp_param                   = &data[0];
            rsp_len                     = 5 + sizeof(struct bt_avrcp_player_attr_value_text);
            break;
        }
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
        {
            struct bt_avrcp_get_player_app_setting_value_text *cmd =
                &vendor_msg->player_value_text;
            struct bt_avrcp_player_get_txt_rsp *rsp;

            if (cmd->num_of_value > 1)
            {
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
                response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
                break;
            }

            rsp = (struct bt_avrcp_player_get_txt_rsp *)&data[0];
            shell_print(ctx_shell, "    PDU-ID -> Get Player Appl. Setting Value Text<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    Attribute ID  = 0x%02x", cmd->attr_id);
            shell_print(ctx_shell, "    No. of Attr. Values  = 0x%02x", cmd->num_of_value);
            rsp->num_of_id              = 1;
            rsp->texts[0].attr_value_id = 1;
            rsp->texts[0].char_set      = 0x6a;
            rsp->texts[0].string_len    = 3;
            rsp->texts[0].string        = (uint8_t *)"OFF";
            rsp_param                   = &data[0];
            rsp_len                     = 5 + sizeof(struct bt_avrcp_player_attr_value_text);
            break;
        }
        case BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:
        {
            struct bt_avrcp_inform_displayable_char_set *cmd =
                &vendor_msg->inform_char_set;

            shell_print(ctx_shell, "    PDU-ID -> Inform Displayable Char Set<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    No. of Char Set = 0x%02x", cmd->num_of_char);

            for (uint8_t i = 0; i < cmd->num_of_char; i++)
            {
                shell_print(ctx_shell, "        - 0x%04x", cmd->char_sets[i]);
            }
            rsp_param = NULL;
            rsp_len   = 0;
            break;
        }
        case BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS:
        {
            uint8_t battery_status = vendor_msg->parameter;

            shell_print(ctx_shell, "    PDU-ID -> Inform Battery Status<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    Battery Status = 0x%02x", battery_status);

            rsp_param = NULL;
            rsp_len   = 0;
            break;
        }
        case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
        {
            struct bt_avrcp_get_element_attrs *cmd =
                &vendor_msg->element_attrs;

            shell_print(ctx_shell, "    PDU-ID -> Get Element Attributes<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    No. of Attributes: %d", cmd->num_of_attr);
            if (cmd->num_of_attr > 3)
            {
                cmd->num_of_attr = 3;
            }

            if (cmd->num_of_attr == 0)
            {
                struct bt_avrcp_player_get_element_attr_rsp *rsp =
                    (struct bt_avrcp_player_get_element_attr_rsp *)&data[0];

                rsp->num_of_attr = 3;

                rsp->attrs[0].attr_id    = 1;
                rsp->attrs[0].char_set   = 0x6a;
                rsp->attrs[0].string_len = 6;
                rsp->attrs[0].string     = (uint8_t *)"title1";
                rsp->attrs[1].attr_id    = 2;
                rsp->attrs[1].char_set   = 0x6a;
                rsp->attrs[1].string_len = 7;
                rsp->attrs[1].string     = (uint8_t *)"artist1";
                rsp->attrs[2].attr_id    = 8;
                rsp->attrs[2].char_set   = 0x6a;
                rsp->attrs[2].string_len = 7;
                rsp->attrs[2].string     = (uint8_t *)"1000004";

                rsp_param = rsp;
                rsp_len =
                    sizeof(struct bt_avrcp_player_get_element_attr_rsp) + sizeof(struct bt_avrcp_element_attr) * 3;
            }
            break;
        }
        case BT_AVRCP_PDU_ID_GET_PLAY_STATUS:
        {
            struct bt_avrcp_play_status_rsp *rsp = (struct bt_avrcp_play_status_rsp *)&data[0];

            rsp->song_length = 2000;
            rsp->song_pos    = 0;
            rsp->play_status = 0;

            rsp_param = rsp;
            rsp_len   = sizeof(rsp);
            break;
        }

        case BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION:
            shell_print(ctx_shell, "    PDU-ID -> Register Notification<0x%x>.", vendor_msg->pdu_id);
            response_type = BT_AVRCP_RESPONSE_TYPE_INTERIM;
            avrcp_target_rsp_notify_cmd_interim(conn, msg, data, &rsp_len, &response_type);
            rsp_param = data;
            break;

        case BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE:
        {
            uint8_t pdu_id = vendor_msg->parameter;
            shell_print(ctx_shell, "    PDU-ID -> Request Continue Response<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    Continue PDU ID: 0x%02x", pdu_id);
            rsp_param = NULL;
            rsp_len   = 0;
            if (pdu_id == BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR)
            {
                struct bt_avrcp_player_app_setting_attr_ids *rsp;
                rsp = (struct bt_avrcp_player_app_setting_attr_ids *)&data[0];

                shell_print(ctx_shell, "    PDU-ID -> List Player Appl. Setting Attributes<0x%x>.", vendor_msg->pdu_id);

                vendor_msg->pdu_id = pdu_id;
                rsp->num_of_attr   = 1;
                rsp->attr_ids[0]   = 1;

                rsp_param = &data[0];
                rsp_len   = 2;
            }
            else
            {
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
                response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
            }
            break;
        }
        case BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE:
        {
            uint8_t pdu_id = vendor_msg->parameter;
            shell_print(ctx_shell, "    PDU-ID -> Abort Continue Response<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    Abort PDU ID: 0x%02x", pdu_id);
            rsp_param = NULL;
            rsp_len   = 0;
            break;
        }

        case BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
        {
            data[0] = vendor_msg->parameter;
            shell_print(ctx_shell, "    PDU-ID -> Set Absolute Volume<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    Volume: 0x%02x", vendor_msg->parameter);
            rsp_param = &data[0];
            rsp_len   = 1;
            break;
        }
        case BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER:
        {
            uint8_t player_id  = vendor_msg->parameter;

            data[0] = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            shell_print(ctx_shell, "    PDU-ID -> Set Addressed Player<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "    Player ID: 0x%04x", player_id);
            rsp_param = &data[0];
            rsp_len   = 1;
            break;
        }
        case BT_AVRCP_PDU_ID_PLAY_ITEMS:
        {
            struct bt_avrcp_play_item *play = &vendor_msg->play_item;

            data[0] = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            shell_print(ctx_shell, "    PDU-ID -> Play Item<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "     - Scope : %d", play->scope);
            shell_print(ctx_shell, "     - UID counter : 0x%04x", play->uid_counter);
            rsp_param = &data[0];
            rsp_len = 1;
            break;
        }
        case BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING:
        {
            struct bt_avrcp_add_to_now_playing *play = &vendor_msg->add_now_play;

            data[0] = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            shell_print(ctx_shell, "    PDU-ID -> Add To Now Playing<0x%x>.", vendor_msg->pdu_id);
            shell_print(ctx_shell, "     - Scope : %d", play->scope);
            shell_print(ctx_shell, "     - UID counter : 0x%04x", play->uid_counter);
            rsp_param = &data[0];
            rsp_len = 1;
            break;
        }

        default:
        {
            shell_print(ctx_shell, "Unknown PDU-ID <0x%x>", vendor_msg->pdu_id);
            rsp_param     = &rj_data;
            rsp_len       = sizeof(rj_data);
            response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
            break;
        }
    }

    bt_avrcp_response_vendor_dependent(conn, vendor_msg->pdu_id, msg->header.tl, response_type, rsp_param, rsp_len);
}

void avrcp_control_received(struct bt_conn *conn, struct bt_avrcp_control_msg *msg, int err)
{
    int ret;

    if (err)
    {
        shell_print(ctx_shell, "error msg");
        return;
    }

    switch (msg->header.op_code)
    {
        case BT_AVRCP_OPCODE_UNIT_INFO:
        {
            shell_print(ctx_shell, "BT_AVRCP_OPCODE_UNIT_INFO");

            ret = bt_avrcp_response_info(conn, BT_AVRCP_OPCODE_UNIT_INFO, BT_AVRCP_SUBUNIT_TYPE_PANEL, msg->header.tl);
            if (ret)
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_response_info");
            }
            break;
        }
        case BT_AVRCP_OPCODE_SUBUNIT_INFO:
            shell_print(ctx_shell, "BT_AVRCP_OPCODE_SUBUNIT_INFO");

            ret =
                bt_avrcp_response_info(conn, BT_AVRCP_OPCODE_SUBUNIT_INFO, BT_AVRCP_SUBUNIT_TYPE_PANEL, msg->header.tl);
            if (ret)
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_response_info");
            }
            break;
        case BT_AVRCP_OPCODE_PASS_THROUGH:
            shell_print(ctx_shell, "BT_AVRCP_OPCODE_PASS_THROUGH");

            ret = bt_avrcp_response_passthrough(conn, BT_AVRCP_RESPONSE_TYPE_ACCEPTED, msg);
            if (ret)
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_response_passthrough");
            }
            break;
        case BT_AVRCP_OPCODE_VENDOR_DEPENDENT:
            avrcp_target_handle_vendor_dependent_msg(conn, msg);
            break;
        default:
            break;
    }
}

void avrcp_target_get_folder_items_send_reject(struct bt_conn *conn,
                                               struct bt_avrcp_browsing_cmd *cmd,
                                               uint8_t rsp_status)
{
    bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp_status, 1);
}

void avrcp_target_handle_get_folder_items_req(struct bt_conn *conn, struct bt_avrcp_browsing_cmd *cmd)
{
    uint8_t rsp_status;
    uint32_t start_item;
    uint32_t end_item;
    struct bt_avrcp_item *item;
    struct bt_avrcp_get_folder_items_rsp *response;

    rsp_status = 0u;

    shell_print(ctx_shell, "    PDU ID - Get Folder Items(0x%x).", cmd->header.pdu_id);

    /* Validate the scope */
    if (cmd->folder_items.scope > BT_AVRCP_SCOPE_NOW_PLAYING)
    {
        rsp_status = BT_AVRCP_BOW_ERROR_INVALID_SCOPE;
    }

    start_item = cmd->folder_items.start_item;
    end_item   = cmd->folder_items.end_item;
    if (start_item > end_item)
    {
        rsp_status = BT_AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;
    }

    if (rsp_status != 0u)
    {
        avrcp_target_get_folder_items_send_reject(conn, cmd, rsp_status);
        return;
    }

    shell_print(ctx_shell, "    Scope : %d", cmd->folder_items.scope);
    shell_print(ctx_shell, "    Start Item Index : %d", start_item);
    shell_print(ctx_shell, "    End Item Index : %d", end_item);

    if (0x00 == cmd->folder_items.attr_count)
    {
        shell_print(ctx_shell, "    Requested ALL Attributes");
    }
    else if (0xFF == cmd->folder_items.attr_count)
    {
        shell_print(ctx_shell, "    Requested NO Attributes");
    }
    else
    {
    }

    switch (cmd->folder_items.scope)
    {
        case BT_AVRCP_SCOPE_MEDIA_PLAYER_LIST:
        {
            // uint16_t rsp_data_len = sizeof (*response) + sizeof (*item);
            DEF_DATA(sizeof(*response) + sizeof(*item));
            response = (struct bt_avrcp_get_folder_items_rsp *)&data[0];

            if (start_item >= 1)
            {
                rsp_status = BT_AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;
                avrcp_target_get_folder_items_send_reject(conn, cmd, rsp_status);
                return;
            }

            end_item = 0;

            response->status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            response->uid_counter  = 1u;
            response->num_of_items = 1u;
            for (uint8_t index = start_item; index < (end_item + 1); index++)
            {
                item = &response->items[index];

                item->item_type                  = BT_AVRCP_ITEM_TYPER_MEDIA_PLAYER;
                item->item_length                = 0;
                item->player_item.player_id      = 1;
                item->player_item.player_type    = 1;
                item->player_item.player_subtype = 0;
                item->player_item.play_status    = 0;
                item->player_item.player_type    = 1;
                memset(&item->player_item.feature_mask[0], 0xFFu, 16u);
                item->player_item.char_set = 0x6a;
                item->player_item.name_len = 7;
                item->player_item.name     = (uint8_t *)"player1";
            }

            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, response,
                sizeof(*response) + sizeof(*item));
            break;
        }

        case BT_AVRCP_SCOPE_VIRTUAL_FILESYSTEM:
        case BT_AVRCP_SCOPE_SEARCH:
        {
            // uint16_t rsp_data_len = sizeof (*response) + sizeof (*item);
            DEF_DATA(sizeof(*response) + sizeof(*item) * 2);
            struct bt_avrcp_item *item;

            response               = (struct bt_avrcp_get_folder_items_rsp *)&data[0];
            response->status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            response->uid_counter  = 1;
            response->num_of_items = 2;

            item            = &response->items[0];
            item->item_type = BT_AVRCP_ITEM_TYPER_MEDIA;
            memset(item->media_item.media_uid, 0, 8);
            item->media_item.media_uid[0]       = 1;
            item->media_item.media_type         = 0;
            item->media_item.char_set           = 0x6a;
            item->media_item.name_len           = 6;
            item->media_item.name               = (uint8_t *)"media1";
            item->media_item.num_of_attr        = 3;
            item->media_item.attrs[0].attr_id   = 1;
            item->media_item.attrs[0].char_set  = 0x6a;
            item->media_item.attrs[0].value_len = 6;
            item->media_item.attrs[0].value_str = (uint8_t *)"title1";
            item->media_item.attrs[1].attr_id   = 2;
            item->media_item.attrs[1].char_set  = 0x6a;
            item->media_item.attrs[1].value_len = 7;
            item->media_item.attrs[1].value_str = (uint8_t *)"artist1";
            item->media_item.attrs[2].attr_id   = 8;
            item->media_item.attrs[2].char_set  = 0x6a;
            item->media_item.attrs[2].value_len = 7;
            item->media_item.attrs[2].value_str = (uint8_t *)"1000004";

            item            = &response->items[1];
            item->item_type = BT_AVRCP_ITEM_TYPER_FOLDER;
            memset(item->folder_item.folder_uid, 0, 8);
            item->folder_item.folder_uid[0] = 2;
            item->folder_item.folder_type   = 5;
            item->folder_item.playable      = 1;
            item->folder_item.char_set      = 0x6a;
            item->folder_item.name_len      = 7;
            item->folder_item.name          = (uint8_t *)"folder1";

            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, response,
                sizeof(*response) + sizeof(*item) * 2);
            break;
        }

        case BT_AVRCP_SCOPE_NOW_PLAYING:
        {
            // uint16_t rsp_data_len = sizeof (*response) + sizeof (*item);
            DEF_DATA(sizeof(*response) + sizeof(*item) * 1);
            struct bt_avrcp_item *item;

            response               = (struct bt_avrcp_get_folder_items_rsp *)&data[0];
            response->status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            response->uid_counter  = 1;
            response->num_of_items = 1;

            item            = &response->items[0];
            item->item_type = BT_AVRCP_ITEM_TYPER_MEDIA;
            memset(item->media_item.media_uid, 0, 8);
            item->media_item.media_uid[0]       = 1;
            item->media_item.media_type         = 0;
            item->media_item.char_set           = 0x6a;
            item->media_item.name_len           = 6;
            item->media_item.name               = (uint8_t *)"media1";
            item->media_item.num_of_attr        = 3;
            item->media_item.attrs[0].attr_id   = 1;
            item->media_item.attrs[0].char_set  = 0x6a;
            item->media_item.attrs[0].value_len = 6;
            item->media_item.attrs[0].value_str = (uint8_t *)"title1";
            item->media_item.attrs[1].attr_id   = 2;
            item->media_item.attrs[1].char_set  = 0x6a;
            item->media_item.attrs[1].value_len = 7;
            item->media_item.attrs[1].value_str = (uint8_t *)"artist1";
            item->media_item.attrs[2].attr_id   = 8;
            item->media_item.attrs[2].char_set  = 0x6a;
            item->media_item.attrs[2].value_len = 7;
            item->media_item.attrs[2].value_str = (uint8_t *)"1000004";

            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, response,
                sizeof(*response) + sizeof(*item) * 1);
            break;
        }
    }
}

void avrcp_browsing_received(struct bt_conn *conn, struct bt_avrcp_browsing_cmd *cmd, int err)
{
    switch (cmd->header.pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_FOLDER_ITEMS:
            avrcp_target_handle_get_folder_items_req(conn, cmd);
            break;

        case BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER:
        {
            uint16_t len;
            struct bt_avrcp_set_browsed_player_rsp rsp;
            uint16_t player_id = cmd->set_browsed_player.player_id;
            shell_print(ctx_shell, "    PDU ID - Set Browsed Player");
            shell_print(ctx_shell, "    Player ID - 0x%04x", player_id);

            if (player_id != 1)
            {
                rsp.status = BT_AVRCP_BOW_ERROR_INVALID_PLAYER_ID;
                len= 1;
            }
            else
            {
                rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
                rsp.uid_counter  = 1;
                rsp.num_of_items = 2;
                rsp.char_set     = 0x006A;
                rsp.folder_depth = 0; /* Setting to ROOT folder */
                len = sizeof(rsp);
            }

            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp,
                len);
            break;
        }

        case BT_AVRCP_PDU_ID_CHANGE_PATH:
        {
            struct bt_avrcp_change_path_rsp rsp;

            shell_print(ctx_shell, "    PDU ID - Change Path (0x%x)");
            rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            rsp.num_of_items = 1;

            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp,
                sizeof(rsp));
            break;
        }

        case BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES:
        {
            struct bt_avrcp_get_item_attrs_rsp rsp;

            rsp.status      = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            rsp.num_of_attr = 0;
            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp,
                sizeof(rsp));
            break;
        }

        case BT_AVRCP_PDU_ID_SEARCH:
        {
            struct bt_avrcp_search_rsp rsp;

            rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            rsp.uid_counter  = 1;
            rsp.num_of_items = 1;
            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp,
                sizeof(rsp));
            break;
        }

        case BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS:
        {
            struct bt_avrcp_get_total_num_of_items_rsp rsp;

            rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            rsp.uid_counter  = 1;
            rsp.num_of_items = 2;
            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp,
                sizeof(rsp));
            break;
        }

        default:
            break;
    }
}
#endif

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))

void avrcp_print_event_nofity_rsp(uint8_t event_id, struct bt_avrcp_event_rsp *event_res)
{
    switch (event_id)
    {
        case BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED<0x%x>.", event_id);

            shell_print(ctx_shell, "     - Player Status: ");
            switch (event_res->play_status)
            {
                case 0x00:
                    shell_print(ctx_shell, "0x00(STOPPED)");
                    break;

                case 0x01:
                    shell_print(ctx_shell, "0x01(PLAYING)");
                    break;

                case 0x02:
                    shell_print(ctx_shell, "0x02(PAUSED)");
                    break;

                case 0x03:
                    shell_print(ctx_shell, "0x03(FWD_SEEK)");
                    break;

                case 0x04:
                    shell_print(ctx_shell, "0x04(REV_SEEK)");
                    break;

                case 0xFF:
                    shell_print(ctx_shell, "0xFF(ERROR)");
                    break;

                default:
                    shell_print(ctx_shell, "??");
                    break;
            }

            break;

        case BT_AVRCP_EVENT_TRACK_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_TRACK_CHANGED<0x%x>.", event_id);
            break;

        case BT_AVRCP_EVENT_TRACK_REACHED_END:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_TRACK_REACHED_END<0x%x>.", event_id);
            break;

        case BT_AVRCP_EVENT_TRACK_REACHED_START:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_TRACK_REACHED_START<0x%x>.", event_id);
            break;

        case BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED<0x%x>.", event_id);
            break;

        case BT_AVRCP_EVENT_BATT_STATUS_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_BATT_STATUS_CHANGED<0x%x>.", event_id);
            break;

        case BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED<0x%x>.", event_id);
            break;

        case BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED<0x%x>.", event_id);
            break;

#ifdef AVRCP_1_4
        case BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED<0x%x>.", event_id);
            break;

        case BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED<0x%x>.", event_id);
            break;

        case BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED<0x%x>.", event_id);
            break;

        case BT_AVRCP_EVENT_UIDS_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_UIDS_CHANGED<0x%x>.", event_id);
            break;

        case BT_AVRCP_EVENT_VOLUME_CHANGED:
            shell_print(ctx_shell, "    Event-ID ->BT_AVRCP_EVENT_VOLUME_CHANGED<0x%x>.", event_id);
            break;

#endif /* AVRCP_1_4 */
        default:
            shell_print(ctx_shell, "    Event-ID -> ??? ");
            break;
    }
    return;
}

void avrcp_print_vendor_cmd_rsp_content(struct bt_avrcp_control_msg *msg)
{
    struct bt_avrcp_vendor *vendor_rsp = &msg->vendor;
    switch (vendor_rsp->pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_CAPABILITY:
        {
            struct bt_avrcp_capability_company_id *caps =
                &vendor_rsp->comapny_id_rsp;
            struct bt_avrcp_capability_events_supported *events =
                &vendor_rsp->events_suported_rsp;

            if (BT_AVRCP_CAP_ID_COMPANY_ID == caps->capability_id)
            {
                uint32_t company_id;

                shell_print(ctx_shell, "    No. of Company ID's Supported: %d", caps->capability_count);

                for (uint8_t i = 0; i < caps->capability_count; i++)
                {
                    /* Extract Company ID's */
                    company_id = sys_get_be24((uint8_t *)&caps->company_ids[i]);
                    shell_print(ctx_shell, "     - 0x%06x", company_id);
                }
            }
            else if (BT_AVRCP_CAP_ID_EVENTS_SUPPORTED == caps->capability_id)
            {
                shell_print(ctx_shell, "    No. of Events Supported: %d", events->capability_count);

                for (uint8_t i = 0; i < events->capability_count; i++)
                {
                    shell_print(ctx_shell, "     - 0x%02x", events->event_ids[i]);
                }
            }
            else
            {
                shell_print(ctx_shell, "    Unknown Capability ID Received.");
            }
        }
        break;

        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
        {
            struct bt_avrcp_player_app_setting_attr_ids *att_ids =
                &vendor_rsp->attr_ids_rsp;

            shell_print(ctx_shell, "    No. of Player Appl. Attributes = 0x%0x", att_ids->num_of_attr);

            for (uint8_t i = 0; i < att_ids->num_of_attr; i++)
            {
                shell_print(ctx_shell, "     - 0x%0x", att_ids->attr_ids[i]);
            }
        }

        break;

        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
        {
            struct bt_avrcp_player_app_setting_values *values =
                &vendor_rsp->values_rsp;

            shell_print(ctx_shell, "    No. of Player Appl. Attr. Values = 0x%0x", values->num_of_value);

            for (uint8_t i = 0; i < values->num_of_value; i++)
            {
                shell_print(ctx_shell, "     - 0x%0x", values->value_ids[i]);
            }
        }

        break;

        case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
        {
            struct bt_avrcp_player_app_attr_values *values =
                &vendor_rsp->attr_values_rsp;

            shell_print(ctx_shell, "    No. of Player Appl. Attributes = 0x%0x", values->num_of_attr);

            for (uint8_t i = 0; i < values->num_of_attr; i++)
            {
                shell_print(ctx_shell, "     - ID: 0x%0x", values->attr_vals[i].attr_id);
                shell_print(ctx_shell, "     - Value: 0x%0x", values->attr_vals[i].value_id);
            }
        }
        break;

        case BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL:
            /* No params expected in the response */
            break;

        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT: /* Fall Through */
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
        {
            uint8_t attr_val_txt[64];
            struct bt_avrcp_player_get_txt_rsp *rsp =
                &vendor_rsp->get_txt_rsp;

            for (uint8_t i = 0; i < rsp->num_of_id; i++)
            {
                /* Attribute Value Text */
                memcpy(attr_val_txt, &rsp->texts[i].string[0], rsp->texts[i].string_len);
                attr_val_txt[rsp->texts[i].string_len] = '\0';

                if (BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT == vendor_rsp->pdu_id)
                {
                    shell_print(ctx_shell, "     - Attr. ID: 0x%0x", rsp->texts[i].attr_value_id);
                }
                else
                {
                    shell_print(ctx_shell, "     - Attr. Val ID: 0x%0x", rsp->texts[i].attr_value_id);
                }

                shell_print(ctx_shell, "     - Value Txt: %s", attr_val_txt);
            }
        }
        break;

        case BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:
            /* No params expected in the response */
            break;

        case BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS:
            /* No params expected in the response */
            break;

        case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
        {
            uint8_t attr_val[64];
            struct bt_avrcp_player_get_element_attr_rsp *rsp =
                &vendor_rsp->element_attr_rsp;

            if (BT_AVRCP_RESPONSE_TYPE_REJECTED == msg->header.ctype_response)
            {
                shell_print(ctx_shell, "    Reason: 0x%02x", vendor_rsp->parameter);
                break;
            }

            shell_print(ctx_shell, "    No. of Attributes: %d", rsp->num_of_attr);
            for (uint8_t i = 0; i < rsp->num_of_attr; i++)
            {
                /* Attribute value */
                memcpy(attr_val, rsp->attrs[i].string, rsp->attrs[i].string_len);

                attr_val[rsp->attrs[i].string_len] = '\0';
                shell_print(ctx_shell, "       - ID: 0x%04x", rsp->attrs[i].attr_id);
                shell_print(ctx_shell, "       - Value: %s", attr_val);
            }
        }
        break;

        case BT_AVRCP_PDU_ID_GET_PLAY_STATUS:
        {
            struct bt_avrcp_play_status_rsp *rsp =
                &vendor_rsp->play_status_rsp;

            shell_print(ctx_shell, "    Current Media Play Status:");
            shell_print(ctx_shell, "       - Song Total Length(in msec): %d", rsp->song_length);
            shell_print(ctx_shell, "       - Song Current postion: %d", rsp->song_pos);
            shell_print(ctx_shell, "       - Song Play Status: ");

            switch (rsp->play_status)
            {
                case 0x00:
                    shell_print(ctx_shell, "0x00(STOPPED)");
                    break;

                case 0x01:
                    shell_print(ctx_shell, "0x01(PLAYING)");
                    break;

                case 0x02:
                    shell_print(ctx_shell, "0x02(PAUSED)");
                    break;

                case 0x03:
                    shell_print(ctx_shell, "0x03(FWD_SEEK)");
                    break;

                case 0x04:
                    shell_print(ctx_shell, "0x04(REV_SEEK)");
                    break;

                case 0xFF:
                    shell_print(ctx_shell, "0xFF(ERROR)");
                    break;

                default:
                    shell_print(ctx_shell, "??");
                    break;
            }

            shell_print(ctx_shell, "");
        }
        break;

        case BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION:
        {
            struct bt_avrcp_event_rsp *rsp =
                &vendor_rsp->event_rsp;

            avrcp_print_event_nofity_rsp(rsp->event_id, rsp);
        }
        break;

        case BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE: /* Fallthrough */
        case BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE:
            break;

        case BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
        {
            shell_print(ctx_shell, "    Volume: 0x%02x", vendor_rsp->parameter);
        }
        break;

        case BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER:
        case BT_AVRCP_PDU_ID_PLAY_ITEMS:         /* Fall Through */
        case BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING: /* Fall Through */
        {
            if (BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL == vendor_rsp->parameter)
            {
                shell_print(ctx_shell, "    Status: Operation Completed Without error");
            }
        }
        break;

        default:

            break;
    }

    return;
}

void avrcp_print_vendor_cmd_rsp(struct bt_avrcp_control_msg *msg)
{
    struct bt_avrcp_vendor *vendor_rsp = &msg->vendor;
    shell_print(ctx_shell, "    PDU-ID -> ");

    switch (vendor_rsp->pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_CAPABILITY:
            shell_print(ctx_shell, "Get Capability <0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
            shell_print(ctx_shell, "List Player Appl. Setting Attributes<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
            shell_print(ctx_shell, "List Player Appl. Setting Values<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
            shell_print(ctx_shell, "Get Current Player Appl. Setting Value<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL:
            shell_print(ctx_shell, "Set Player Appl. Setting Value<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT:
            shell_print(ctx_shell, "Get Player Appl. Setting Attr. Text<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
            shell_print(ctx_shell, "Get Player Appl. Setting Value Text<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:
            shell_print(ctx_shell, "Inform Displayable Char Set<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS:
            shell_print(ctx_shell, "Inform Battery Status<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
            shell_print(ctx_shell, "Get Element Attributes<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_GET_PLAY_STATUS:
            shell_print(ctx_shell, "Get Play Status<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION:
            shell_print(ctx_shell, "Register Notification<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE:
            shell_print(ctx_shell, "Request Continue Response <0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE:
            shell_print(ctx_shell, "Abort Continue Response <0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
            shell_print(ctx_shell, "Set Absolute Volume<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER:
            shell_print(ctx_shell, "Set Addressed Player<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_PLAY_ITEMS:
            shell_print(ctx_shell, "Play Item<0x%x>.", vendor_rsp->pdu_id);
            break;

        case BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING:
            shell_print(ctx_shell, "Add To Now Playing<0x%x>.", vendor_rsp->pdu_id);
            break;

        default:
            shell_print(ctx_shell, "Unknown PDU-ID<0x%x>", vendor_rsp->pdu_id);

            break;
    }

    switch (vendor_rsp->packet_type)
    {
        case BT_AVRCP_PACKET_TYPE_SINGLE:
            shell_print(ctx_shell, "Single");
            break;

        case BT_AVRCP_PACKET_TYPE_START:
            shell_print(ctx_shell, "Start");
            shell_print(ctx_shell, "====== Params are Not Displaying ======= ");
            shell_print(ctx_shell, "Take Action to Send Continue/Abort Command");
            return;
            /* break; */

        case BT_AVRCP_PACKET_TYPE_CONTINUE:
            shell_print(ctx_shell, "Continue");
            shell_print(ctx_shell, "====== Params are Not Displaying ======= ");
            shell_print(ctx_shell, "Take Action to Send Continue/Abort Command");
            return;
            /* break; */

        case BT_AVRCP_PACKET_TYPE_END:
            shell_print(ctx_shell, "End");
            shell_print(ctx_shell, "====== Params are Not Displaying ======= ");

            /**
             * Since the previously received fragments are not saves, parameters will not be correct.
             * So returning from here.
             */
            return;
            /* break; */
    }

    shell_print(ctx_shell, "    Param Length: 0x%04x", vendor_rsp->parameter_len);

    avrcp_print_vendor_cmd_rsp_content(msg);

    return;
}

void avrcp_control_rsp_received(struct bt_conn *conn, struct bt_avrcp_control_msg *msg, int err)
{
    uint16_t vu_op_id;

    if ((err) || (msg == NULL))
    {
        shell_error(ctx_shell, "    respone fail");
        return;
    }

    shell_error(ctx_shell, "    avrcp control response");
    avrcp_opcode_print(msg->header.op_code);
    avrcp_response_type_print(msg->header.ctype_response);
    if (msg->header.op_code == BT_AVRCP_OPCODE_PASS_THROUGH)
    {
        struct bt_avrcp_pass_through *pass_th = &msg->pass_th;

        avrcp_op_id_print(pass_th->op_id);
        shell_print(ctx_shell, "    State = ");
        if (0x00 == pass_th->state_flag)
        {
            shell_print(ctx_shell, "Pressed");
        }
        else
        {
            shell_print(ctx_shell, "Released");
        }

        if (pass_th->op_id == BT_AVRCP_OP_ID_VENDOR_UNIQUE)
        {
            vu_op_id = sys_get_be16(&pass_th->op_data[3]);
            shell_print(ctx_shell, "    Vendor Unique Op Id: 0x%04x", vu_op_id);
        }
    }
    else if (msg->header.op_code == BT_AVRCP_OPCODE_VENDOR_DEPENDENT)
    {
        avrcp_print_vendor_cmd_rsp(msg);
    }
    else
    {
    }

    avrcp_auto_test(0);
}

void avrcp_browsing_rsp_received(struct bt_conn *conn, struct bt_avrcp_browsing_rsp *rsp, int err)
{
    if (err)
    {
        shell_print(ctx_shell, "browsing rsp error");
        return;
    }

    shell_print(ctx_shell, "browsing rsp...");
    switch (rsp->header.pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_FOLDER_ITEMS:
            shell_print(ctx_shell, "BT_AVRCP_PDU_ID_GET_FOLDER_ITEMS");
            break;

        case BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER:
        {
            shell_print(ctx_shell, "BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER");
            break;
        }

        case BT_AVRCP_PDU_ID_CHANGE_PATH:
        {
            shell_print(ctx_shell, "BT_AVRCP_PDU_ID_CHANGE_PATH");
            break;
        }

        case BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES:
        {
            shell_print(ctx_shell, "BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES");
            break;
        }

        case BT_AVRCP_PDU_ID_SEARCH:
        {
            shell_print(ctx_shell, "BT_AVRCP_PDU_ID_SEARCH");
            break;
        }

        case BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS:
        {
            shell_print(ctx_shell, "BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS");
            break;
        }

        default:
            break;
    }

    avrcp_auto_test(0);
}

#endif

void avrcp_control_connected(struct bt_conn *conn, int err)
{
    shell_print(ctx_shell, "control connected");
}

void avrcp_control_disconnected(struct bt_conn *conn, int err)
{
    shell_print(ctx_shell, "control disconnected");
}

void avrcp_browsing_connected(struct bt_conn *conn, int err)
{
    shell_print(ctx_shell, "browsing connected");
}

void avrcp_browsing_disconnected(struct bt_conn *conn, int err)
{
    shell_print(ctx_shell, "browsing disconnected");
}

void avrcp_send_result(struct bt_conn *conn, int err)
{
    if (err)
    {
        shell_print(ctx_shell, "send fail");
    }
    else
    {
        shell_print(ctx_shell, "send success");
    }
}

static uint8_t test_enable;
static uint8_t test_step;
static uint8_t test_one;
static void avrcp_auto_test(uint8_t print)
{
    uint8_t pdu_id;
    int err;
    struct bt_conn *conn = default_conn;

    if (print == 0)
    {
        if (test_enable == 0)
        {
            return;
        }

        if (test_one == 2)
        {
            test_enable = 0;
            return;
        }
    }

    switch (test_step)
    {
        case 0:
            shell_print(ctx_shell, "0.bt_avrcp_send_unit_info");
            if (print)
            {
                break;
            }
            if (bt_avrcp_send_unit_info(default_conn))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_unit_info");
            }
            break;
        case 1:
            shell_print(ctx_shell, "1.bt_avrcp_send_subunit_info");
            if (print)
            {
                break;
            }
            if (bt_avrcp_send_subunit_info(default_conn))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_subunit_info");
            }
            break;
        case 2:
            shell_print(ctx_shell, "2.cmd_send_pass_through(button pressed)");
            if (print)
            {
                break;
            }
            err = bt_avrcp_send_passthrough(conn, BT_AVRCP_OP_ID_UP, 0, 0);
            if (err)
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_send_passthrough");
            }
            break;
        case 3:
            shell_print(ctx_shell, "3.cmd_send_pass_through(button released)");
            if (print)
            {
                break;
            }
            err = bt_avrcp_send_passthrough(conn, BT_AVRCP_OP_ID_UP, 0, 1);
            if (err)
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_send_passthrough");
            }
            break;
        case 4:
            shell_print(ctx_shell, "4.cmd_send_pass_through vendor(button pressed)");
            if (print)
            {
                break;
            }
            err = bt_avrcp_send_passthrough(conn, BT_AVRCP_OP_ID_VENDOR_UNIQUE, 1, 0);
            if (err)
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_send_passthrough");
            }
            break;
        case 5:
            shell_print(ctx_shell, "5.cmd_send_pass_through vendor(button released)");
            if (print)
            {
                break;
            }
            err = bt_avrcp_send_passthrough(conn, BT_AVRCP_OP_ID_VENDOR_UNIQUE, 1, 1);
            if (err)
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_send_passthrough");
            }
            break;
        case 6:
        {
            uint8_t cap_id;

            shell_print(ctx_shell, "6.get company ids");
            if (print)
            {
                break;
            }

            pdu_id = BT_AVRCP_PDU_ID_GET_CAPABILITY;
            cap_id = BT_AVRCP_CAP_ID_COMPANY_ID;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, &cap_id))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 7:
        {
            uint8_t cap_id;

            shell_print(ctx_shell, "7.get events supported");
            if (print)
            {
                break;
            }
            pdu_id = BT_AVRCP_PDU_ID_GET_CAPABILITY;
            cap_id = BT_AVRCP_CAP_ID_EVENTS_SUPPORTED;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, &cap_id))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 8:
            shell_print(ctx_shell, "8.ListPlayerApplicationSettingAttributes");
            if (print)
            {
                break;
            }
            pdu_id = BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, NULL))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        case 9:
        {
            uint8_t attr_id = 1;
            shell_print(ctx_shell, "9.ListPlayerApplicationSettingValues");
            if (print)
            {
                break;
            }
            pdu_id = BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, &attr_id))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 10:
        {
            uint8_t req_pdu = BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR;

            shell_print(ctx_shell, "10.RequestContinuingResponse");
            if (print)
            {
                break;
            }
            if (bt_avrcp_send_vendor_dependent(default_conn, BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE, &req_pdu))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 11:
        {
            uint8_t req_pdu = BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR;
            shell_print(ctx_shell, "11.AbortContinuingResponse");
            if (print)
            {
                break;
            }
            if (bt_avrcp_send_vendor_dependent(default_conn, BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE, &req_pdu))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_request_continuing_rsp");
            }
            break;
        }
        case 12:
        {
            DEF_DATA(2);
            struct bt_avrcp_player_app_setting_attr_ids *attr = (struct bt_avrcp_player_app_setting_attr_ids *)&data[0];
            shell_print(ctx_shell, "12.GetCurrentPlayerApplicationSettingValue");
            if (print)
            {
                break;
            }

            attr->num_of_attr = 1;
            attr->attr_ids[0] = 1;
            pdu_id            = BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, attr))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 13:
        {
            DEF_DATA(3);
            struct bt_avrcp_player_app_attr_values *attr_values = (struct bt_avrcp_player_app_attr_values *)&data[0];
            shell_print(ctx_shell, "13.SetPlayerApplicationSettingValue");
            if (print)
            {
                break;
            }

            attr_values->num_of_attr           = 1;
            attr_values->attr_vals[0].attr_id  = 1;
            attr_values->attr_vals[0].value_id = 1;
            pdu_id                             = BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, attr_values))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 14:
        {
            DEF_DATA(2);
            struct bt_avrcp_player_app_setting_attr_ids *attr = (struct bt_avrcp_player_app_setting_attr_ids *)&data[0];
            shell_print(ctx_shell, "14.GetPlayerApplicationSettingAttributeText");
            if (print)
            {
                break;
            }

            attr->num_of_attr = 1;
            attr->attr_ids[0] = 1;
            pdu_id            = BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, attr))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 15:
        {
            DEF_DATA(3);
            struct bt_avrcp_get_player_app_setting_value_text *text =
                (struct bt_avrcp_get_player_app_setting_value_text *)&data[0];
            shell_print(ctx_shell, "15.GetPlayerApplicatoinSettingValueText");
            if (print)
            {
                break;
            }

            text->num_of_value = 1;
            text->attr_id      = 1;
            text->value_ids[0] = 1;
            pdu_id             = BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, text))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 16:
        {
            DEF_DATA(3);
            struct bt_avrcp_inform_displayable_char_set *set = (struct bt_avrcp_inform_displayable_char_set *)&data[0];
            shell_print(ctx_shell, "16.InformDisplayableCharacterSet");
            if (print)
            {
                break;
            }

            set->num_of_char  = 1;
            set->char_sets[0] = 0x6a;
            pdu_id            = BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, set))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 17:
        {
            uint8_t battery_status = 0;

            shell_print(ctx_shell, "17.InformBatterSatusofCT");
            if (print)
            {
                break;
            }
            pdu_id = BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, &battery_status))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 18:
        {
            DEF_DATA(13u);
            struct bt_avrcp_get_element_attrs *attrs = (struct bt_avrcp_get_element_attrs *)&data[0];

            shell_print(ctx_shell, "18.GetElementAttributes");
            if (print)
            {
                break;
            }
            memset(&attrs->identifier[0], 0, 8u);
            attrs->num_of_attr = 0u; /* get all attrs */
            pdu_id             = BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, attrs))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 19:
        {
            shell_print(ctx_shell, "19.GetPlayStatus");
            if (print)
            {
                break;
            }
            if (bt_avrcp_send_vendor_dependent(default_conn, BT_AVRCP_PDU_ID_GET_PLAY_STATUS, NULL))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 20:
        {
            struct bt_avrcp_register_ntfy reg;
            shell_print(ctx_shell, "20.RegisterNotification");
            if (print)
            {
                break;
            }

            reg.event_id = BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED;
            reg.playback_interval = 0;
            if (bt_avrcp_send_vendor_dependent(default_conn, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION, &reg))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 21:
        {
            uint8_t volume = 10;
            shell_print(ctx_shell, "21.SetAbsoluteVolume");
            if (print)
            {
                break;
            }
            if (bt_avrcp_send_vendor_dependent(default_conn, BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME, &volume))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 22:
        {
            uint16_t player_id = 1;
            shell_print(ctx_shell, "22.SetAddressedPlayer");
            if (print)
            {
                break;
            }
            if (bt_avrcp_send_vendor_dependent(default_conn, BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER, &player_id))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 23:
        {
            shell_print(ctx_shell, "23.SetBrowsedPlayer");
            if (print)
            {
                break;
            }
            if (bt_avrcp_set_borwsed_player(default_conn, 1))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_set_borwsed_player");
            }
            break;
        }
        case 24:
        {
            struct bt_avrcp_search_cmd search;
            shell_print(ctx_shell, "24.Search");
            if (print)
            {
                break;
            }

            search.char_set = 0x6a;
            search.length   = 7;
            search.str      = (uint8_t *)"folder1";

            if (bt_avrcp_search(default_conn, &search))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_search");
            }
            break;
        }
        case 25:
        {
            struct bt_avrcp_add_to_now_playing add;

            shell_print(ctx_shell, "25.AddToNowPlaying");
            if (print)
            {
                break;
            }
            add.scope = 1; /* Filesystem */
            memset(add.uid, 0, 8);
            add.uid[0]      = FOLDER_UID;
            add.uid_counter = 1;
            pdu_id          = BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, &add))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }
        case 26:
        {
            struct bt_avrcp_get_folder_items_cmd param;
            shell_print(ctx_shell, "26.GetFolderItems(MediaPlayerList)");
            if (print)
            {
                break;
            }

            param.scope      = 0; /* Media Player */
            param.start_item = 0;
            param.end_item   = 10;
            param.attr_count = 0; /* all attr */

            if (bt_avrcp_get_folder_items(default_conn, &param))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_get_folder_items");
            }
            break;
        }
        case 27:
        {
            struct bt_avrcp_get_folder_items_cmd param;
            shell_print(ctx_shell, "27.GetFolderItems(Filesystem)");
            if (print)
            {
                break;
            }

            param.scope      = 1; /* Filesystem */
            param.start_item = 0;
            param.end_item   = 10;
            param.attr_count = 0; /* all attr */

            if (bt_avrcp_get_folder_items(default_conn, &param))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_get_folder_items");
            }
            break;
        }
        case 28:
        {
            struct bt_avrcp_get_folder_items_cmd param;
            shell_print(ctx_shell, "28.GetFolderItems(SearchResultList)");
            if (print)
            {
                break;
            }

            param.scope      = 2; /* SearchResultList */
            param.start_item = 0;
            param.end_item   = 10;
            param.attr_count = 0; /* all attr */

            if (bt_avrcp_get_folder_items(default_conn, &param))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_get_folder_items");
            }
            break;
        }
        case 29:
        {
            struct bt_avrcp_get_folder_items_cmd param;
            shell_print(ctx_shell, "29.GetFolderItems(NowPlayingList)");
            if (print)
            {
                break;
            }

            param.scope      = 3; /* NowPlayingList */
            param.start_item = 0;
            param.end_item   = 10;
            param.attr_count = 0; /* all attr */

            if (bt_avrcp_get_folder_items(default_conn, &param))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_get_folder_items");
            }
            break;
        }
        case 30:
        {
            struct bt_avrcp_change_path_cmd change_path;
            shell_print(ctx_shell, "30.ChangePath");
            if (print)
            {
                break;
            }

            change_path.uid_counter = 1;
            change_path.direction   = 1; /* down */
            memset(change_path.folder_uid, 0, 8);
            change_path.folder_uid[0] = FOLDER_UID;

            if (bt_avrcp_change_path(default_conn, &change_path))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_change_path");
            }
            break;
        }

        case 31:
        {
            struct bt_avrcp_get_item_attrs_cmd get_item;
            shell_print(ctx_shell, "31.GetItemAttributes(FileSystem)");
            if (print)
            {
                break;
            }

            get_item.scope = 1; /* FileSystem */
            memset(get_item.uid, 0, 8);
            get_item.uid[0]      = FOLDER_UID;
            get_item.uid_counter = 1;
            get_item.num_of_attr = 0; /* all */

            if (bt_avrcp_get_items_attribute(default_conn, &get_item))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_get_items_attribute");
            }
            break;
        }

        case 32:
        {
            struct bt_avrcp_get_item_attrs_cmd get_item;
            shell_print(ctx_shell, "32.GetItemAttributes(search)");
            if (print)
            {
                break;
            }

            get_item.scope = 2; /* search */
            memset(get_item.uid, 0, 8);
            get_item.uid[0]      = search_uid;
            get_item.uid_counter = 1;
            get_item.num_of_attr = 0; /* all */

            if (bt_avrcp_get_items_attribute(default_conn, &get_item))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_get_items_attribute");
            }
            break;
        }

        case 33:
        {
            struct bt_avrcp_get_item_attrs_cmd get_item;
            shell_print(ctx_shell, "33.GetItemAttributes(Now Playing)");
            if (print)
            {
                break;
            }

            get_item.scope = 3; /* Now Playing */
            memset(get_item.uid, 0, 8);
            get_item.uid[0]      = now_playing_uid;
            get_item.uid_counter = 1;
            get_item.num_of_attr = 0; /* all */

            if (bt_avrcp_get_items_attribute(default_conn, &get_item))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_get_items_attribute");
            }
            break;
        }

        case 34:
        {
            shell_print(ctx_shell, "34.GetTotalNumberOfItems");
            if (print)
            {
                break;
            }
            if (bt_avrcp_get_total_num_of_items(default_conn, 0))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_get_total_num_of_items");
            }
            break;
        }

        case 35:
        {
            struct bt_avrcp_play_item play;

            shell_print(ctx_shell, "35.PlayItem(Filesystem)");
            if (print)
            {
                break;
            }
            play.scope = 1; /* Filesystem */
            memset(play.uid, 0, 8);
            play.uid[0]      = FOLDER_UID;
            play.uid_counter = 1;
            pdu_id           = BT_AVRCP_PDU_ID_PLAY_ITEMS;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, &play))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }

        case 36:
        {
            struct bt_avrcp_play_item play;

            shell_print(ctx_shell, "36.PlayItem(SearchResultList)");
            if (print)
            {
                break;
            }
            play.scope = 2; /* SearchResultList */
            memset(play.uid, 0, 8);
            play.uid[0]      = search_uid;
            play.uid_counter = 1;
            pdu_id           = BT_AVRCP_PDU_ID_PLAY_ITEMS;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, &play))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }

        case 37:
        {
            struct bt_avrcp_play_item play;

            shell_print(ctx_shell, "37.PlayItem(NowPlayingList)");
            if (print)
            {
                break;
            }
            play.scope = 3; /* NowPlayingList */
            memset(play.uid, 0, 8);
            play.uid[0]      = now_playing_uid;
            play.uid_counter = 1;
            pdu_id           = BT_AVRCP_PDU_ID_PLAY_ITEMS;
            if (bt_avrcp_send_vendor_dependent(default_conn, pdu_id, &play))
            {
                shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
            }
            break;
        }

        default:
            test_enable = 0;
            break;
    }

    if (print == 0)
    {
        if (test_one == 0)
        {
            test_step++;
        }
        else
        {
            test_one++;
        }
    }
}

static shell_status_t cmd_init_ct(shell_handle_t shell, int32_t argc, char *argv[])
{
    struct bt_avrcp_cb cbs = {avrcp_control_connected,    avrcp_control_disconnected,
                              avrcp_browsing_connected,   avrcp_browsing_disconnected,
                              avrcp_send_result,          NULL,
                              avrcp_control_rsp_received, NULL,
                              avrcp_browsing_rsp_received};

    bt_sdp_register_service(&avrcp_ct_rec);

    bt_avrcp_register_callback(&cbs);
    shell_print(shell, "init success");
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_init_tg(shell_handle_t shell, int32_t argc, char *argv[])
{
    struct bt_avrcp_cb cbs = {avrcp_control_connected,
                              avrcp_control_disconnected,
                              avrcp_browsing_connected,
                              avrcp_browsing_disconnected,
                              avrcp_send_result,
                              avrcp_control_received,
                              NULL,
                              avrcp_browsing_received,
                              NULL};

    bt_sdp_register_service(&avrcp_tg_rec);

    bt_avrcp_register_callback(&cbs);
    shell_print(shell, "init success");
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_control_connect(shell_handle_t shell, int32_t argc, char *argv[])
{
    if (bt_avrcp_control_connect(default_conn))
    {
        shell_print(shell, "fail to call bt_avrcp_control_connect");
    }
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_browsing_connect(shell_handle_t shell, int32_t argc, char *argv[])
{
    if (bt_avrcp_browsing_connect(default_conn))
    {
        shell_print(shell, "fail to call bt_avrcp_browsing_connect");
    }
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_ct_list_all_cases(shell_handle_t shell, int32_t argc, char *argv[])
{
    for (uint8_t index = 0; index <= 37u; index++)
    {
        test_step = index;
        avrcp_auto_test(1);
    }
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_ct_test_case(shell_handle_t shell, int32_t argc, char *argv[])
{
    if (argc == 2)
    {
        test_step = strtoul(argv[1], NULL, 10);
    }
    else
    {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    test_enable = 1;
    test_one = 1;
    avrcp_auto_test(0);
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_ct_test_all(shell_handle_t shell, int32_t argc, char *argv[])
{
    test_enable = 1;
    test_one  = 0;
    test_step = 0;
    avrcp_auto_test(0);
    return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_AVRCP_COVER_ART) && ((CONFIG_BT_AVRCP_COVER_ART) > 0U))
uint8_t default_cover_handle;
static uint16_t cover_art_xchg_size;
uint16_t sent;
static uint8_t img_properties[256u];
static uint16_t image_prop_obj_size;
static uint32_t remaining;
#define IMAGE_FILE_LEN (512u * 4)
uint8_t image_data[512U];
void avrcp_cover_art_connected(uint8_t handle, struct bt_conn *conn, int err)
{
    shell_print(ctx_shell, "connected");
}

void avrcp_cover_art_disconnected(uint8_t handle, int err)
{
    shell_print(ctx_shell, "disconnected");
}

static uint8_t cover_test_step;
static uint8_t wait_count;
static void cover_art_auto_test(uint8_t print)
{
    switch (cover_test_step)
    {
        case 0:
        {
            struct bt_avrcp_get_image_property param;

            shell_print(ctx_shell, "0.bt_avrcp_get_image_property");
            if (print)
            {
                break;
            }

            param.image_handle     = (uint8_t *)"1000004";
            param.image_handle_len = 7u;
            param.wait             = 0;
            if (bt_avrcp_get_image_property(default_cover_handle, &param))
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_get_image_property");
            }
            break;
        }
        case 1:
        {
            struct bt_avrcp_get_image param;

            shell_print(ctx_shell, "1.bt_avrcp_get_image");
            if (print)
            {
                break;
            }

            param.image_descriptor_data     = NULL;
            param.image_descriptor_data_len = 0u;
            param.image_handle              = (uint8_t *)"1000004";
            param.image_handle_len          = 7u;
            param.wait                      = 0;
            if (bt_avrcp_get_image(default_cover_handle, &param))
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_get_image");
            }
            break;
        }
        case 2:
        {
            struct bt_avrcp_get_image param;

            shell_print(ctx_shell, "2.bt_avrcp_get_image with wait");
            if (print)
            {
                break;
            }

            wait_count                      = 2;
            param.image_descriptor_data     = NULL;
            param.image_descriptor_data_len = 0u;
            param.image_handle              = (uint8_t *)"1000004";
            param.image_handle_len          = 7u;
            param.wait                      = wait_count;
            if (bt_avrcp_get_image(default_cover_handle, &param))
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_get_image");
            }
            break;
        }
        case 3:
        {
            struct bt_avrcp_get_linked_thumbnail param;

            shell_print(ctx_shell, "3.bt_avrcp_get_linked_thumbnail");
            if (print)
            {
                break;
            }

            param.image_handle     = (uint8_t *)"1000004";
            param.image_handle_len = 7u;
            param.wait             = 0;
            if (bt_avrcp_get_linked_thumbnail(default_cover_handle, &param))
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_get_linked_thumbnail");
            }
            break;
        }
        default:
            break;
    }
}

void avrcp_cover_art_cmd_received(uint8_t handle, struct bt_avrcp_cover_art_cmd *cmd, int err)
{
    struct bt_avrcp_cover_art_rsp rsp;
    uint8_t send_response = 1;

    shell_print(ctx_shell, "cover art cmd:");
    rsp.cmd = cmd->cmd;
    switch (cmd->cmd)
    {
        case BT_AVRCP_COVER_ART_CONNECT:
        {
            shell_print(ctx_shell, "  BT_AVRCP_COVER_ART_CONNECT");
            shell_print(ctx_shell, "  MAX Exchange Size: %d", cmd->connect.max_recv_size);
            cover_art_xchg_size = cmd->connect.max_recv_size;
            rsp.response        = BT_AVRCP_CA_SUCCESS_RSP;
            break;
        }
        case BT_AVRCP_COVER_ART_GET_PROP:
        {
            struct bt_avrcp_get_image_property *get = &cmd->get_prop;

            shell_print(ctx_shell, "  BT_AVRCP_COVER_ART_GET_PROP");
            if (sent == 0)
            {
                rsp.response = BT_AVRCP_CA_SUCCESS_RSP;
                if ((get->image_handle == NULL) || (get->image_handle_len == 0))
                {
                    rsp.response = BT_AVRCP_CA_BAD_REQ_RSP;
                }
                memset(&img_properties[0], 0, sizeof(img_properties));
                snprintf((char *)img_properties, sizeof(img_properties) - 1,
                        "<image-properties version=\"1.0\" handle="
                        "\"%s\"> \n"
                        "<native encoding=\"JPEG\" pixel=\"640*480\" size=\"10240\"/>\n"
                        "<variant encoding=\"GIF\" pixel=\"640*480\"/>\n"
                        "<variant encoding=\"JPEG\" pixel=\"200*200\"/>\n"
                        "<variant encoding=\"JPEG\" pixel=\"75*75\"/>\n"
                        "</image-properties>",
                        get->image_handle);

                image_prop_obj_size = (uint16_t)strlen((char *)img_properties);
                remaining           = image_prop_obj_size;
            }

            if (remaining > cover_art_xchg_size)
            {
                rsp.get_prop.length = cover_art_xchg_size;
                rsp.response        = BT_AVRCP_CA_CONTINUE_RSP;
            }
            else if (remaining != 0)
            {
                rsp.get_prop.length = (uint16_t)remaining;
                rsp.response        = BT_AVRCP_CA_SUCCESS_RSP;
            }
            else
            {
                rsp.get_prop.length = 0u;
                rsp.response        = BT_AVRCP_CA_SUCCESS_RSP;
            }

            rsp.get_prop.data = &img_properties[sent];
            sent += rsp.get_prop.length;
            remaining -= rsp.get_prop.length;
            if (remaining == 0)
            {
                image_prop_obj_size = 0;
                sent                = 0;
            }
            break;
        }

        case BT_AVRCP_COVER_ART_GET_IMAGE:
        {
            struct bt_avrcp_get_image *get = &cmd->get_image;

            shell_print(ctx_shell, "  BT_AVRCP_COVER_ART_GET_IMAGE");
            if (sent == 0)
            {
                rsp.response = BT_AVRCP_CA_SUCCESS_RSP;
                if ((get->image_handle == NULL) || (get->image_handle_len == 0))
                {
                    rsp.response = BT_AVRCP_CA_BAD_REQ_RSP;
                }

                if ((get->image_descriptor_data != NULL) && (get->image_descriptor_data_len != 0))
                {
                    // todo:
                }

                remaining = IMAGE_FILE_LEN;
            }

            if (remaining > cover_art_xchg_size)
            {
                rsp.get_image.length = cover_art_xchg_size;
                rsp.response         = BT_AVRCP_CA_CONTINUE_RSP;
            }
            else if (remaining != 0)
            {
                rsp.get_image.length = (uint16_t)remaining;
                rsp.response         = BT_AVRCP_CA_SUCCESS_RSP;
            }
            else
            {
                rsp.get_image.length = 0u;
                rsp.response        = BT_AVRCP_CA_SUCCESS_RSP;
            }

            rsp.get_image.data = &image_data[0];
            sent += rsp.get_image.length;
            remaining -= rsp.get_image.length;
            if (remaining == 0)
            {
                sent = 0;
            }

            break;
        }

        case BT_AVRCP_COVER_ART_GET_THUMB:
        {
            struct bt_avrcp_get_linked_thumbnail *get = &cmd->get_thumb;

            shell_print(ctx_shell, "  BT_AVRCP_COVER_ART_GET_THUMB");
            if (sent == 0)
            {
                rsp.response = BT_AVRCP_CA_SUCCESS_RSP;
                if ((get->image_handle == NULL) || (get->image_handle_len == 0))
                {
                    rsp.response = BT_AVRCP_CA_BAD_REQ_RSP;
                }

                remaining = IMAGE_FILE_LEN;
            }

            if (remaining > cover_art_xchg_size)
            {
                rsp.get_thumb.length = cover_art_xchg_size;
                rsp.response         = BT_AVRCP_CA_CONTINUE_RSP;
            }
            else if (remaining != 0)
            {
                rsp.get_thumb.length = (uint16_t)remaining;
                rsp.response         = BT_AVRCP_CA_SUCCESS_RSP;
            }
            else
            {
                rsp.get_thumb.length = 0u;
                rsp.response        = BT_AVRCP_CA_SUCCESS_RSP;
            }

            rsp.get_thumb.data = &image_data[0];
            sent += rsp.get_thumb.length;
            remaining -= rsp.get_thumb.length;
            if (remaining == 0)
            {
                sent = 0;
            }
            break;
        }

        case BT_AVRCP_COVER_ART_ABORT:
            shell_print(ctx_shell, "  BT_AVRCP_COVER_ART_ABORT");
            send_response = 0;
            break;

        default:
            break;
    }

    if (send_response)
    {
        bt_avrcp_response_cover_art(handle, rsp.cmd, rsp.response, &rsp.get_prop);
    }
}

void avrcp_cover_art_rsp_received(uint8_t handle, struct bt_avrcp_cover_art_rsp *rsp, int err)
{
    uint8_t next         = 0;
    uint8_t send_request = 0;
    shell_print(ctx_shell, "cover art rsp:");
    switch (rsp->cmd)
    {
        case BT_AVRCP_COVER_ART_CONNECT:
        {
            shell_print(ctx_shell, "  BT_AVRCP_COVER_ART_CONNECT");
            shell_print(ctx_shell, "MAX Exchange Size: %d", rsp->connect.max_recv_size);
            cover_art_xchg_size = rsp->connect.max_recv_size;
            break;
        }
        case BT_AVRCP_COVER_ART_GET_PROP:
        {
            uint8_t str[128u];
            shell_print(ctx_shell, "  BT_AVRCP_COVER_ART_GET_PROP");
            memcpy(str, rsp->get_prop.data, (128u > rsp->get_prop.length) ? rsp->get_prop.length : 128u);
            str[127] = 0;
            shell_print(ctx_shell, "  %s", str);
            if (rsp->response == BT_AVRCP_CA_CONTINUE_RSP)
            {
                send_request = 1;
            }
            else
            {
                next = 1;
            }
            break;
        }

        case BT_AVRCP_COVER_ART_GET_IMAGE:
        {
            shell_print(ctx_shell, "  BT_AVRCP_COVER_ART_GET_IMAGE");
            shell_print(ctx_shell, "  image file length:%d, data len:%d", rsp->get_image.image_file_size,
                        rsp->get_image.length);
            if (rsp->response == BT_AVRCP_CA_CONTINUE_RSP)
            {
                send_request = 1;
            }
            else
            {
                next = 1;
            }
            break;
        }

        case BT_AVRCP_COVER_ART_GET_THUMB:
        {
            shell_print(ctx_shell, "  BT_AVRCP_COVER_ART_GET_THUMB");
            shell_print(ctx_shell, "  data len:%d", rsp->get_thumb.length);
            if (rsp->response == BT_AVRCP_CA_CONTINUE_RSP)
            {
                send_request = 1;
            }
            else
            {
                next = 1;
            }
            break;
        }

        case BT_AVRCP_COVER_ART_ABORT:
            shell_print(ctx_shell, "  BT_AVRCP_COVER_ART_ABORT");
            break;

        default:
            break;
    }

    if (next == 1)
    {
        cover_test_step++;
        cover_art_auto_test(0);
    }
    else if (send_request == 1)
    {
        if (wait_count > 0)
        {
            wait_count--;
            bt_avrcp_send_request(handle, 1, rsp);
        }
        else
        {
            bt_avrcp_send_request(handle, 0, rsp);
        }
    }
}
#endif

static shell_status_t cmd_ca_init_responder(shell_handle_t shell, int32_t argc, char *argv[])
{
    struct bt_avrcp_cover_art_cb cb = {avrcp_cover_art_connected, avrcp_cover_art_disconnected,
                                       avrcp_cover_art_cmd_received, NULL};

    bt_avrcp_register_cover_art_cb(&cb);
    if (bt_avrcp_cover_art_start_responder(&default_cover_handle))
    {
        shell_print(shell, "fail to call bt_avrcp_cover_art_start_responder");
    }
    else
    {
        shell_print(shell, "success");
    }
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_ca_init_initiator(shell_handle_t shell, int32_t argc, char *argv[])
{
    struct bt_avrcp_cover_art_cb cb = {avrcp_cover_art_connected, avrcp_cover_art_disconnected, NULL,
                                       avrcp_cover_art_rsp_received};

    bt_avrcp_register_cover_art_cb(&cb);
    if (bt_avrcp_cover_art_start_initiator(&default_cover_handle))
    {
        shell_print(shell, "fail to call bt_avrcp_cover_art_start_responder");
    }
    else
    {
        shell_print(shell, "success");
    }
    return kStatus_SHELL_Success;
}

uint16_t remote_cover_psm;
static uint8_t app_sdp_avrcp_user(struct bt_conn *conn,
               struct bt_sdp_client_result *result)
{
    uint16_t param;
    int res;

    if ((result) && (result->resp_buf))
    {
        shell_print(ctx_shell, "sdp callback\r\n");
        res = bt_sdp_get_addl_proto_param(result->resp_buf, BT_SDP_PROTO_L2CAP, 1, &param);
        if (res < 0)
        {
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
        else
        {
            struct bt_avrcp_cover_art_connect connect;

            connect.l2cap_rpsm    = param;
            connect.max_recv_size = 512u;
            if (bt_avrcp_cover_art_connect(default_cover_handle, default_conn, &connect))
            {
                shell_print(ctx_shell, "fail to call bt_avrcp_cover_art_connect");
            }
            return BT_SDP_DISCOVER_UUID_STOP;
        }
    }
    else
    {
        shell_print(ctx_shell, "sdp fail callback\r\n");
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
}

#define SDP_CLIENT_USER_BUF_LEN        512U
NET_BUF_POOL_FIXED_DEFINE(app_sdp_client_pool, CONFIG_BT_MAX_CONN,
              SDP_CLIENT_USER_BUF_LEN, NULL);

static uint8_t app_sdp_avrcp_user(struct bt_conn *conn, struct bt_sdp_client_result *result);
static struct bt_sdp_discover_params discov_avrcp_tg =
{
    .uuid = BT_UUID_DECLARE_16(BT_SDP_AV_REMOTE_TARGET_SVCLASS),
    .func = app_sdp_avrcp_user,
    .pool = &app_sdp_client_pool,
};

static shell_status_t cmd_ca_init_connect(shell_handle_t shell, int32_t argc, char *argv[])
{
    int res = bt_sdp_discover(default_conn, &discov_avrcp_tg);
    if (res)
    {
        shell_print(ctx_shell, "SDP discovery failed\r\n");
    }
    else
    {
        shell_print(ctx_shell, "SDP discovery started\r\n");
    }

    return kStatus_SHELL_Success;
}

static shell_status_t cmd_ca_initiator_test(shell_handle_t shell, int32_t argc, char *argv[])
{
    cover_test_step = 0;
    cover_art_auto_test(0);
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_ct_reg_ntf(shell_handle_t shell, int32_t argc, char *argv[])
{
    uint8_t event;
    uint32_t playback_interval = 0;
    struct bt_avrcp_register_ntfy reg;

    if (argc == 2) {
        event = strtoul(argv[1], NULL, 16);
    } else {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    if (event > 0x0du) {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    if (event == BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED){
         playback_interval = 5;
    }

    reg.event_id = event;
    reg.playback_interval = playback_interval;
    test_enable = 0;
    if (bt_avrcp_send_vendor_dependent(default_conn, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION, &reg))
    {
        shell_error(ctx_shell, "fail to call bt_avrcp_send_vendor_dependent");
    }

    return kStatus_SHELL_Success;
}

static shell_status_t cmd_tg_notify(shell_handle_t shell, int32_t argc, char *argv[])
{
    uint8_t event;
    struct bt_avrcp_event_rsp *rsp;
    DEF_DATA(32u);
    uint16_t rsp_len;

    if (argc == 2) {
        event = strtoul(argv[1], NULL, 16);
    } else {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    if (event > 0x0du) {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    if (registered_events[event - 1][0] == 0u){
        shell_print(shell, "the event is not registered by CT");
        return kStatus_SHELL_Success;
    }

    rsp = (struct bt_avrcp_event_rsp *)&data[0];
    rsp_len = sizeof(*rsp);
    rsp->event_id = event;
    switch (event)
    {
        case BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
            rsp->play_status = 0;
            break;

        case BT_AVRCP_EVENT_TRACK_CHANGED:
            memset(rsp->identifier, 0, 8u);
            rsp->identifier[0] = 1u;
            break;

        case BT_AVRCP_EVENT_TRACK_REACHED_END:
        case BT_AVRCP_EVENT_TRACK_REACHED_START: /* Fall Through */
            break;

        case BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
            rsp->playback_pos = 1000;
            break;

        case BT_AVRCP_EVENT_BATT_STATUS_CHANGED:
            rsp->battery_status = 0;
            break;

        case BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED:
            rsp->system_status = 0;
            break;

        case BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:
        {
            rsp->setting_changed.num_of_attr           = 1;
            rsp->setting_changed.attr_vals[0].attr_id  = 1;
            rsp->setting_changed.attr_vals[0].value_id = 1;

            rsp_len = sizeof(rsp) + sizeof(struct bt_avrcp_player_attr_value);
            break;
        }

        case BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
            break;

        case BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
            break;

        case BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
            rsp->addressed_player_changed.player_id   = 1;
            rsp->addressed_player_changed.uid_counter = 1;
            break;

        case BT_AVRCP_EVENT_UIDS_CHANGED:
            rsp->uid_counter = 1;
            break;

        case BT_AVRCP_EVENT_VOLUME_CHANGED:
            rsp->absolute_volume = 10;
            break;

        default:
            break;
    }

    bt_avrcp_response_vendor_dependent(default_conn, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION,
                                       registered_events[event - 1][1], BT_AVRCP_RESPONSE_TYPE_CHANGED,
                                       rsp, rsp_len);

    return kStatus_SHELL_Success;
}

#define HELP_NONE "[none]"

SHELL_STATIC_SUBCMD_SET_CREATE(
    avrcp_cmds,
    SHELL_CMD_ARG(init_ct, NULL, HELP_NONE, cmd_init_ct, 1, 0),
    SHELL_CMD_ARG(init_tg, NULL, HELP_NONE, cmd_init_tg, 1, 0),
    SHELL_CMD_ARG(ctl_connect, NULL, "\"create control connection\"", cmd_control_connect, 1, 0),
    SHELL_CMD_ARG(brow_connect, NULL, "\"create browsing connection\"", cmd_browsing_connect, 1, 0),
    SHELL_CMD_ARG(ct_list_all_cases, NULL, "\"display all the test cases\"", cmd_ct_list_all_cases, 1, 0),
    SHELL_CMD_ARG(ct_test_case, NULL, "<select one case to test>", cmd_ct_test_case, 2, 0),
    SHELL_CMD_ARG(ct_test_all, NULL, "\"test all cases\"", cmd_ct_test_all, 1, 0),
    SHELL_CMD_ARG(ct_reg_ntf, NULL, "<Register Notification. select event:\r\n\
                                    1. EVENT_PLAYBACK_STATUS_CHANGED\r\n\
                                    2. EVENT_TRACK_CHANGED\r\n\
                                    3. EVENT_TRACK_REACHED_END\r\n\
                                    4. EVENT_TRACK_REACHED_START\r\n\
                                    5. EVENT_PLAYBACK_POS_CHANGED\r\n\
                                    6. EVENT_BATT_STATUS_CHANGED\r\n\
                                    7. EVENT_SYSTEM_STATUS_CHANGED\r\n\
                                    8. EVENT_PLAYER_APPLICATION_SETTING_CHANGED\r\n\
                                    9. EVENT_NOW_PLAYING_CONTENT_CHANGED\r\n\
                                    a. EVENT_AVAILABLE_PLAYERS_CHANGED\r\n\
                                    b. EVENT_ADDRESSED_PLAYER_CHANGED\r\n\
                                    c. EVENT_UIDS_CHANGED\r\n\
                                    d. EVENT_VOLUME_CHANGED>", cmd_ct_reg_ntf, 2, 0),
    SHELL_CMD_ARG(tg_notify, NULL, "<Notify event. select event:\r\n\
                                    1. EVENT_PLAYBACK_STATUS_CHANGED\r\n\
                                    2. EVENT_TRACK_CHANGED\r\n\
                                    3. EVENT_TRACK_REACHED_END\r\n\
                                    4. EVENT_TRACK_REACHED_START\r\n\
                                    5. EVENT_PLAYBACK_POS_CHANGED\r\n\
                                    6. EVENT_BATT_STATUS_CHANGED\r\n\
                                    7. EVENT_SYSTEM_STATUS_CHANGED\r\n\
                                    8. EVENT_PLAYER_APPLICATION_SETTING_CHANGED\r\n\
                                    9. EVENT_NOW_PLAYING_CONTENT_CHANGED\r\n\
                                    a. EVENT_AVAILABLE_PLAYERS_CHANGED\r\n\
                                    b. EVENT_ADDRESSED_PLAYER_CHANGED\r\n\
                                    c. EVENT_UIDS_CHANGED\r\n\
                                    d. EVENT_VOLUME_CHANGED>", cmd_tg_notify, 2, 0),
    SHELL_CMD_ARG(ca_init_i, NULL, "\"Init cover art initiator\"", cmd_ca_init_initiator, 1, 0),
    SHELL_CMD_ARG(ca_init_r, NULL, "\"Init cover art responder\"", cmd_ca_init_responder, 1, 0),
    SHELL_CMD_ARG(ca_connect, NULL, "\"create cover art connection\"", cmd_ca_init_connect, 1, 0),
    SHELL_CMD_ARG(ca_test, NULL, "\"cover art test all cases\"", cmd_ca_initiator_test, 1, 0),
    SHELL_SUBCMD_SET_END);

static shell_status_t cmd_avrcp(shell_handle_t shell, int32_t argc, char **argv)
{
    if (argc == 1)
    {
        shell_help(shell);
        /* shell returns 1 when help is printed */
        return kStatus_SHELL_PrintCmdHelp;
    }

    shell_error(shell, "%s unknown parameter: %s", argv[0], argv[1]);

    return kStatus_SHELL_Error;
}

SHELL_CMD_REGISTER(avrcp, avrcp_cmds, "Bluetooth AVRCP shell commands", cmd_avrcp, 1, 1);

void bt_ShellAvrcpInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandavrcp))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandavrcp.pcCommand);
    }
}

#endif /* CONFIG_BT_BREDR */
