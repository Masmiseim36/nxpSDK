/* avrcp.c - Bluetooth AVRCP Tester */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if defined(CONFIG_BT_AVRCP) && (CONFIG_BT_AVRCP > 0) 

#include <porting.h>

#include "bt_pal_conn_internal.h"

#include <bluetooth/l2cap.h>
#include <bluetooth/avrcp.h>
#include <bluetooth/sdp.h>

#include "board.h"

#include "btp/btp.h"

#include "fsl_component_log_config.h"
#define LOG_MODULE_NAME bttester_avrcp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#define SDP_CLIENT_USER_BUF_LEN 512U

NET_BUF_POOL_FIXED_DEFINE(sdp_client_pool, CONFIG_BT_MAX_CONN, SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

#if defined(CONFIG_BT_AVRCP_TG) && (CONFIG_BT_AVRCP_TG > 0)
#define AVRCP_TG_SUPPORTED_FEATURES (0x01ffu)

#define DEF_DATA(len) __aligned(4) uint8_t data[len];

struct avrcp_target_notify_t {
	uint8_t event_id;
	struct bt_conn *conn;
	struct k_work_delayable tg_notify_work;
};
static struct avrcp_target_notify_t avrcp_target_notify;
static void avrcp_tg_notify_work_handler(struct k_work *work);

/* used for BT_AVRCP_EVENT_TRACK_CHANGED evnt id in RegisterNotification */
static uint8_t trackID[8];

/* feature mask FFFFFFFFFFFFFF7F1F00000000000000 defined in IXIT */
static uint8_t feature_mask[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* attribute value length: 0x3D(61)*/
static uint8_t attributValue1[] = {0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x77, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x77, 0x31, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x77, 0x32, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x77, 0x33, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x77, 0x34, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x77, 0x35, 0x2B, 0x54, 0x6F};
/* attribute value length: 0xBB(187)*/
static uint8_t attributValue2[] = {0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x31, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x32, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x33, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x34, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x35, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x36, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x37, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x38, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x39, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x31, 0x30, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x31, 0x31, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x31, 0x32, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x31, 0x33, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x31, 0x34, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x31, 0x35, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x31, 0x36, 0x2B, 0x54, 0x6F, 0x6D, 0x6F, 0x72, 0x72, 0x6F, 0x57, 0x31, 0x37, 0x0A};
/* attribute value length: 0x9A(154)*/
static uint8_t attributValue3[] = {0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x31, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x32, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x33, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x34, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x35, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x36, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x37, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x38, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x39, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x31, 0x30, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x31, 0x31, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x31, 0x32, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x31, 0x33, 0x2B, 0x73, 0x68, 0x77, 0x65, 0x74, 0x68, 0x61, 0x61, 0x31, 0x34, 0x0A};
/* attribute value length: 0x2(2)*/
static uint8_t attributValue4[] = {0x31, 0x0A};
/* attribute value length: 0x3(3)*/
static uint8_t attributValue5[] = {0x31, 0x35, 0x0A};
/* attribute value length: 0x35(53)*/
static uint8_t attributValue6[] = {0x61, 0x6E, 0x69, 0x6C, 0x2B, 0x61, 0x6E, 0x69, 0x6C, 0x31, 0x2B, 0x61, 0x6E, 0x69, 0x6C, 0x32, 0x2B, 0x61, 0x6E, 0x69, 0x6C, 0x33, 0x2B, 0x61, 0x6E, 0x69, 0x6C, 0x34, 0x2B, 0x61, 0x6E, 0x69, 0x6C, 0x35, 0x2B, 0x61, 0x6E, 0x69, 0x6C, 0x36, 0x2B, 0x61, 0x6E, 0x69, 0x6C, 0x37, 0x2B, 0x61, 0x6E, 0x69, 0x6C, 0x38, 0x0A};
/* attribute value length: 0x7(7)*/
static uint8_t attributValue7[] = {0x31, 0x35, 0x30, 0x30, 0x30, 0x30, 0x0A};
static struct bt_avrcp_element_attr playing_element_attributes[] = {
	{
		/* Title */
		.attr_id = 0x01,	/* attribut ID: 0x01*/
		.char_set = 0x6A,				/* character set ID: 0x6A(UTF-8)*/
		.string_len = sizeof(attributValue1),
		.string = attributValue1,
	},
	{
		/* Artist Name */
		.attr_id = 0x02,	/* attribut ID: 0x02*/
		.char_set = 0x6A,				/* character set ID: 0x6A(UTF-8)*/
		.string_len = sizeof(attributValue2),	
		.string = attributValue2,
	},
	{
		/* Album Name */
		.attr_id = 0x03,	/* attribut ID: 0x03*/
		.char_set = 0x6A,				/* character set ID: 0x6A(UTF-8)*/
		.string_len = sizeof(attributValue3),
		.string = attributValue3,
	},
	{
		/* Track Number */
		.attr_id = 0x04,	/* attribut ID: 0x04*/
		.char_set = 0x6A,			/* character set ID: 0x6A(UTF-8)*/
		.string_len = sizeof(attributValue4),
		.string = attributValue4,
	},
	{
		/* Total Number of Tracks */
		.attr_id = 0x05,	/* attribut ID: 0x05*/
		.char_set = 0x6A,				/* character set ID: 0x6A(UTF-8)*/
		.string_len = sizeof(attributValue5),
		.string = attributValue5,
	},
	{
		/* Genre */
		.attr_id = 0x06,	/* attribut ID: 0x06*/
		.char_set = 0x6A,				/* character set ID: 0x6A(UTF-8)*/
		.string_len = sizeof(attributValue6),
		.string = attributValue6,
	},
	{
		/* Playing Time */
		.attr_id = 0x07,	/* attribut ID: 0x07*/
		.char_set = 0x6A,				/* character set ID: 0x6A(UTF-8)*/
		.string_len = sizeof(attributValue7),
		.string = attributValue7,
	},
};
#endif /* CONFIG_BT_AVRCP_TG > 0 */

#if defined(CONFIG_BT_AVRCP_CT) && (CONFIG_BT_AVRCP_CT > 0)
#define AVRCP_CT_SUPPORTED_FEATURES (0x03Cfu)
#endif

#define MEDIA_ITEM_UID_LENGTH 8
static uint16_t UIDCounter;
static uint8_t MediaItemUID[MEDIA_ITEM_UID_LENGTH];

void avrcp_control_connected(struct bt_conn *conn, int err)
{
  	/* avrcp control connected */
    return;
}

void avrcp_control_disconnected(struct bt_conn *conn, int err)
{
  	/* avrcp control disconnected */
    return;
}

void avrcp_browsing_connected(struct bt_conn *conn, int err)
{
  	/* avrcp browsing connected */
    return;
}

void avrcp_browsing_disconnected(struct bt_conn *conn, int err)
{
  	/* avrcp browsing disconnected */
    return;
}

void avrcp_send_result(struct bt_conn *conn, int err)
{
    if (err) {
        /* send fail */
    }
    else {
	    /* send success */
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
    if (event_id <= 13) {
        registered_events[event_id - 1][0] = 0x01;
        registered_events[event_id - 1][1] = tl;
    }
}

static void avrcp_tg_notify_work_handler(struct k_work *work)
{
	struct k_work_delayable *dwork = k_work_delayable_from_work(work);
    struct avrcp_target_notify_t *avrcp_ntf = CONTAINER_OF(dwork, struct avrcp_target_notify_t, tg_notify_work);
    uint8_t event;
    struct bt_avrcp_event_rsp *rsp;
    DEF_DATA(32u);
    uint16_t rsp_len;

	event = avrcp_ntf->event_id;
    if (registered_events[event - 1][0] == 0u) {
        /* the event is not registered by CT */
        return;
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
            memcpy(rsp->identifier, trackID, 8u);
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
            rsp->setting_changed.attr_vals[0].value_id = 0;

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

    bt_avrcp_response_vendor_dependent(avrcp_ntf->conn, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION,
                                       registered_events[event - 1][1], BT_AVRCP_RESPONSE_TYPE_CHANGED,
                                       rsp, rsp_len);

    return;
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
            rsp->play_status = 0;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_TRACK_CHANGED:
		  memcpy(rsp->identifier, trackID, 8u);
		  if ((trackID[0] == 0xFF) || (trackID[0] == 0x00)) {
			/* trackID is 0XFFFFFFFFFFFFFFFF, No Selected Track */
			/* trackID is 0X0000000000000000, Playing Track */
			return;
		  } else {			
            register_player_event(event_id, msg->header.tl);
		  }
            break;

        case BT_AVRCP_EVENT_TRACK_REACHED_END:
        case BT_AVRCP_EVENT_TRACK_REACHED_START: /* Fall Through */
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
            rsp->playback_pos = 1000;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_BATT_STATUS_CHANGED:
            rsp->battery_status = 0;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED:
            rsp->system_status = 0;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:
        {
            rsp->setting_changed.num_of_attr           = 1;
            rsp->setting_changed.attr_vals[0].attr_id  = 1;
            rsp->setting_changed.attr_vals[0].value_id = 1;

            *rsp_len = sizeof(rsp) + sizeof(struct bt_avrcp_player_attr_value);
            register_player_event(event_id, msg->header.tl);
            break;
        }

#ifdef AVRCP_1_4
        case BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
            rsp->addressed_player_changed.player_id   = 1;
            rsp->addressed_player_changed.uid_counter = 1;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_UIDS_CHANGED:
            rsp->uid_counter = 1;
            register_player_event(event_id, msg->header.tl);
            break;

        case BT_AVRCP_EVENT_VOLUME_CHANGED:
            rsp->absolute_volume = 10;
            register_player_event(event_id, msg->header.tl);
            break;

#endif /* AVRCP_1_4 */
        default:
            break;
    }

    /* Check Event ID */
    if (event_id > BT_AVRCP_EVENT_VOLUME_CHANGED) {
        *response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
        data[0]        = BT_AVRCP_METADATA_ERROR_INVALID_PARAMETER;
        *rsp_len       = 1;
	} else {
	    avrcp_target_notify.event_id = event_id;
		avrcp_target_notify.conn = conn;
		k_work_init_delayable(&avrcp_target_notify.tg_notify_work, avrcp_tg_notify_work_handler);
		k_work_schedule(&avrcp_target_notify.tg_notify_work, BT_SECONDS(3));
	}

    return;
}

static void avrcp_target_handle_vendor_dependent_msg(struct bt_conn *conn, struct bt_avrcp_control_msg *msg)
{
    DEF_DATA(1024u);
    uint8_t rj_data = BT_AVRCP_METADATA_ERROR_INVALID_PARAMETER;
    void *rsp_param;
    uint16_t rsp_len;
    uint8_t response_type = BT_AVRCP_RESPONSE_TYPE_STABLE;
	uint8_t index;
    struct bt_avrcp_vendor *vendor_msg = &msg->vendor;
	int ret;

    rsp_param = NULL;
    rsp_len   = 0u;
    switch (vendor_msg->pdu_id)
    {
        case BT_AVRCP_PDU_ID_GET_CAPABILITY:
        {
            uint8_t cap_id;
            cap_id = vendor_msg->parameter;

            if (cap_id == BT_AVRCP_CAP_ID_COMPANY_ID) {
                struct bt_avrcp_capability_company_id *rsp;
                rsp = (struct bt_avrcp_capability_company_id *)&data[0];

                rsp->capability_id    = cap_id;
                rsp->capability_count = 1;
                sys_put_be24(0x1958u, (uint8_t *)&rsp->company_ids[0]);

                rsp_param = rsp;
                rsp_len   = 5u;
            }
            else if (cap_id == BT_AVRCP_CAP_ID_EVENTS_SUPPORTED) {
                struct bt_avrcp_capability_events_supported *rsp;
                rsp = (struct bt_avrcp_capability_events_supported *)&data[0];

                rsp->capability_id    = cap_id;
                rsp->capability_count = 13u;
                for (index = 0; index < 13u; index++) {
                    rsp->event_ids[index] = event_ids[index];
                }

                rsp_param = &data[0];
                rsp_len   = 15u;
            } else {
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

            /* Validate Attribute ID */
            if (attr_id != 1) {
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
                response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
                break;
            }

            if (response_type != BT_AVRCP_RESPONSE_TYPE_REJECTED) {
				rsp               = (struct bt_avrcp_player_app_setting_values *)&data[0];
				rsp->num_of_value = 2;
				rsp->value_ids[0] = 1;
				rsp->value_ids[1] = 2;
				rsp_param         = &data[0];
				rsp_len           = 3;
			}
            break;
        }
        case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
        {
            struct bt_avrcp_player_app_setting_attr_ids *cmd = &vendor_msg->player_attr_ids;
            struct bt_avrcp_player_app_attr_values *rsp;

            if (cmd->num_of_attr > 1) {
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
                response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
                break;
            }
			
			for (index = 0U; index < cmd->num_of_attr; index++) {
				if ((cmd->attr_ids[index] < 0x01U) || ((cmd->attr_ids[index] > 0x04) && (cmd->attr_ids[index] <= 0x7F))) {
					/* invalid player application setting attribute IDs */
					rsp_param     = &rj_data;
					rsp_len       = sizeof(rj_data);
					response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
					break;
				}
			}
			
			if (response_type != BT_AVRCP_RESPONSE_TYPE_REJECTED) {
				rsp                        = (struct bt_avrcp_player_app_attr_values *)&data[0];
				rsp->num_of_attr           = 1;
				rsp->attr_vals[0].attr_id  = 1;
				rsp->attr_vals[0].value_id = 1;
				rsp_param                  = &data[0];
				rsp_len                    = 3u;
			}
            break;
        }
        case BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL:
        {
            struct bt_avrcp_player_app_attr_values *cmd = &vendor_msg->player_attr_values;

			for (index = 0U; index < cmd->num_of_attr; index++) {
				switch (cmd->attr_vals[index].attr_id)
				{
					case 0x01:
						if (!((cmd->attr_vals[index].value_id >= 0x01) && (cmd->attr_vals[index].value_id <= 0x02))) {
							response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
						}
						break;

					case 0x02:
						if (!((cmd->attr_vals[index].value_id >= 0x01) && (cmd->attr_vals[index].value_id <= 0x04))) {
							response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
						}
						break;

					case 0x03:
					case 0x04:
						if (!((cmd->attr_vals[index].value_id >= 0x01) && (cmd->attr_vals[index].value_id <= 0x03))) {
							response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
						}
						break;

					default:
						response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
						break;
				}
			}

			if (response_type != BT_AVRCP_RESPONSE_TYPE_REJECTED) {
				rsp_param = NULL;
				rsp_len   = 0;
			} else {
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
			}
            break;
        }
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT:
        {
            struct bt_avrcp_player_app_setting_attr_ids *cmd = &vendor_msg->player_attr_ids;
            struct bt_avrcp_player_get_txt_rsp *rsp;

            if (cmd->num_of_attr > 1) {
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
                response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
                break;
            }
			
			for (index = 0U; index < cmd->num_of_attr; index++) {
				if ((cmd->attr_ids[index] < 0x01U) || ((cmd->attr_ids[index] > 0x04) && (cmd->attr_ids[index] <= 0x7F))) {
					/* invalid player application setting attribute IDs */
					rsp_param     = &rj_data;
					rsp_len       = sizeof(rj_data);
					response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
					break;
				}
			}

			if (response_type != BT_AVRCP_RESPONSE_TYPE_REJECTED) {
				rsp = (struct bt_avrcp_player_get_txt_rsp *)&data[0];
				rsp->num_of_id              = 1;
				rsp->texts[0].attr_value_id = 1;
				rsp->texts[0].char_set      = 0x6a;
				rsp->texts[0].string_len    = 9;
				rsp->texts[0].string        = (uint8_t *)"EQUALIZER";
				rsp_param                   = &data[0];
				rsp_len                     = 5 + sizeof(struct bt_avrcp_player_attr_value_text);
			}
            break;
        }
        case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
        {
            struct bt_avrcp_get_player_app_setting_value_text *cmd = &vendor_msg->player_value_text;
            struct bt_avrcp_player_get_txt_rsp *rsp;
		
			switch (cmd->attr_id)
			{
				case 0x01:	/* Equalizer ON/OFF status */
					/* valid value_id is 0x01/0x02 */
					for (index = 0U; index < cmd->num_of_value; index++) {
						if ((cmd->value_ids[index] != 0x01) && (cmd->value_ids[index] != 0x02)) {
							response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
							break;
					  }
					}
				
					if (response_type != BT_AVRCP_RESPONSE_TYPE_REJECTED) {
						rsp = (struct bt_avrcp_player_get_txt_rsp *)&data[0];
						rsp->num_of_id = 0x02;
						
						rsp->texts[0].attr_value_id = 0x01;
						rsp->texts[0].char_set = 0x6A;	/* UTF-8 */
						rsp->texts[0].string_len = 0x04;
						rsp->texts[0].string = (uint8_t *)"OFF.";
						
						rsp->texts[1].attr_value_id = 0x02;
						rsp->texts[1].char_set = 0x6A;	/* UTF-8 */
						rsp->texts[1].string_len = 0x03;
						rsp->texts[1].string = (uint8_t *)"ON.";
						
						rsp_param = &data[0];
						rsp_len   = 5 + rsp->num_of_id * sizeof(struct bt_avrcp_player_attr_value_text);
					} else {
						rsp_param = &rj_data;
						rsp_len   = sizeof(rj_data);
					}
					break;
			  
				case 0x02:
				case 0x03:
				case 0x04:
					/* TD */
					break;
			  
				default:
					/* invalid player application setting attribute IDs */
					rsp_param     = &rj_data;
					rsp_len       = sizeof(rj_data);
					response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
					break;
			}
        	break;
        }
        case BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:
		case BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS:
        {
            rsp_param = NULL;
            rsp_len   = 0;
            break;
        }
        case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
        {
            struct bt_avrcp_get_element_attrs *cmd = &vendor_msg->element_attrs;
			struct bt_avrcp_player_get_element_attr_rsp *rsp = (struct bt_avrcp_player_get_element_attr_rsp *)&data[0];
			
			if (cmd->num_of_attr == 0x00) {
				/* get all attributes */			
				for (index = 0U; index < 7U; index++) {
					rsp->attrs[index].attr_id = playing_element_attributes[index].attr_id;
					rsp->attrs[index].char_set = playing_element_attributes[index].char_set;
					rsp->attrs[index].string_len = playing_element_attributes[index].string_len;
					rsp->attrs[index].string = playing_element_attributes[index].string;
				}

			    rsp->num_of_attr = 7U;
			    rsp_param = rsp;
			    rsp_len = sizeof(struct bt_avrcp_player_get_element_attr_rsp) + sizeof(struct bt_avrcp_element_attr) * 7;
			} else {
				for (index = 0U; index < cmd->num_of_attr; index++) {
					switch(sys_be32_to_cpu(cmd->attr_ids[index]))
					{
						case 0x01: 
						case 0x02:
						case 0x03:
							rsp->attrs[index].attr_id = sys_be32_to_cpu(cmd->attr_ids[index]);
							rsp->attrs[index].char_set = 0x6A; /* UTF-8 */
							rsp->attrs[index].string_len = 5;
							rsp->attrs[index].string = (uint8_t *)"Test.";
							break;
															
						case 0x04:
						case 0x05:
						case 0x06:
						case 0x07:
							/* TD */
							break;

						default: 
							/* invalid attribute ID */
							rsp_param     = &rj_data;
							rsp_len       = sizeof(rj_data);
							response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
							break;
					}
				}
				
				if (response_type != BT_AVRCP_RESPONSE_TYPE_REJECTED) {
				    rsp->num_of_attr = cmd->num_of_attr;
					rsp_param = rsp;
					rsp_len = sizeof(struct bt_avrcp_player_get_element_attr_rsp) + sizeof(struct bt_avrcp_element_attr) * cmd->num_of_attr;
				}
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
            response_type = BT_AVRCP_RESPONSE_TYPE_INTERIM;
            avrcp_target_rsp_notify_cmd_interim(conn, msg, data, &rsp_len, &response_type);
            rsp_param = data;
            break;

        case BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE:
        {
            rsp_param = NULL;
            rsp_len   = 0;

            break;
        }
        case BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE:
        {
            uint8_t pdu_id = vendor_msg->parameter;
			response_type = BT_AVRCP_RESPONSE_TYPE_ACCEPTED;
            rsp_param = NULL;
            rsp_len   = 0;
            break;
        }
        case BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
        {
            data[0] = vendor_msg->parameter;
            rsp_param = &data[0];
            rsp_len   = 1;
            break;
        }
        case BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER:
        {
            uint8_t player_id  = vendor_msg->parameter;
			
			if (player_id == 0xFF) {
			    rj_data       = BT_AVRCP_BOW_ERROR_INVALID_PLAYER_ID;
                rsp_param     = &rj_data;
                rsp_len       = sizeof(rj_data);
                response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
			} else {
				data[0] = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
				
				rsp_param = &data[0];
				rsp_len   = 1;
			}
            break;
        }
        case BT_AVRCP_PDU_ID_PLAY_ITEMS:
        {
            struct bt_avrcp_play_item *play = &vendor_msg->play_item;

            data[0] = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            rsp_param = &data[0];
            rsp_len = 1;
            break;
        }
        case BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING:
        {
            struct bt_avrcp_add_to_now_playing *play = &vendor_msg->add_now_play;

            data[0] = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            rsp_param = &data[0];
            rsp_len = 1;
            break;
        }
        default:
        {
		    rj_data = BT_AVRCP_METADATA_ERROR_INVALID_COMMAND;
            rsp_param     = &rj_data;
            rsp_len       = sizeof(rj_data);
            response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
            break;
        }
    }

	if(0 != bt_avrcp_response_vendor_dependent(conn, vendor_msg->pdu_id, msg->header.tl, response_type, rsp_param, rsp_len)) {
		/* send vendor dependent response failed */
		assert(0);
	}
	
}

void avrcp_control_received(struct bt_conn *conn, struct bt_avrcp_control_msg *msg, int err)
{
    int ret;

    if (err) {
        return;
    }

    switch (msg->header.op_code)
    {
        case BT_AVRCP_OPCODE_UNIT_INFO:
            ret = bt_avrcp_response_info(conn, BT_AVRCP_OPCODE_UNIT_INFO, BT_AVRCP_SUBUNIT_TYPE_PANEL, msg->header.tl);
            break;
        case BT_AVRCP_OPCODE_SUBUNIT_INFO:
            ret = bt_avrcp_response_info(conn, BT_AVRCP_OPCODE_SUBUNIT_INFO, BT_AVRCP_SUBUNIT_TYPE_PANEL, msg->header.tl);
            break;
        case BT_AVRCP_OPCODE_PASS_THROUGH:
            ret = bt_avrcp_response_passthrough(conn, BT_AVRCP_RESPONSE_TYPE_ACCEPTED, msg);
            break;
        case BT_AVRCP_OPCODE_VENDOR_DEPENDENT:
            avrcp_target_handle_vendor_dependent_msg(conn, msg);
            break;
        default:
            break;
    }

	(void)ret;
	
	return;
}
#endif

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
void avrcp_control_rsp_received(struct bt_conn *conn, struct bt_avrcp_control_msg *msg, int err)
{
	if (err) {
		return;
	}
  
    switch (msg->header.op_code)
    {
        case BT_AVRCP_OPCODE_UNIT_INFO:
		case BT_AVRCP_OPCODE_SUBUNIT_INFO:
		case BT_AVRCP_OPCODE_PASS_THROUGH:
            break;
        case BT_AVRCP_OPCODE_VENDOR_DEPENDENT:
			if ((msg->header.ctype_response == BT_AVRCP_RESPONSE_TYPE_CHANGED) && (msg->vendor.event_rsp.event_id == BT_AVRCP_EVENT_VOLUME_CHANGED)) {
			  	/* covert volume data to the volume percentage */
			  	uint8_t absolute_volume = (msg->vendor.event_rsp.absolute_volume & BT_AVRCP_VOLUME_MAXIMUM) * 100 / BT_AVRCP_VOLUME_MAXIMUM;
				tester_event(BTP_SERVICE_ID_AVRCP, BTP_AVRCP_EVENT_VOLUME_CHANGED, &absolute_volume, 1);
			}
            break;
        default:
            break;
    }

	return;
}

void avrcp_vendor_dependent_continue_rsp(struct bt_conn *conn, struct bt_avrcp_vendor_header *header, struct net_buf *buf)
{
	/* To do */
	return;
}
#endif

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
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

    /* Validate the scope */
    if (cmd->folder_items.scope > BT_AVRCP_SCOPE_NOW_PLAYING) {
        rsp_status = BT_AVRCP_BOW_ERROR_INVALID_SCOPE;
    }

    start_item = cmd->folder_items.start_item;
    end_item   = cmd->folder_items.end_item;
    if (start_item > end_item) {
        rsp_status = BT_AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;
    }

    if (rsp_status != 0u) {
        avrcp_target_get_folder_items_send_reject(conn, cmd, rsp_status);
        return;
    }

    switch (cmd->folder_items.scope)
    {
        case BT_AVRCP_SCOPE_MEDIA_PLAYER_LIST:
        {
            DEF_DATA(sizeof(*response) + sizeof(*item));
            response = (struct bt_avrcp_get_folder_items_rsp *)&data[0];

            if (start_item >= 1) {
                rsp_status = BT_AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;
                avrcp_target_get_folder_items_send_reject(conn, cmd, rsp_status);
                return;
            }

            end_item = 0;

            response->status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            response->uid_counter  = 1u;
            response->num_of_items = 1u;
            for (uint8_t index = start_item; index < (end_item + 1); index++) {
                item = &response->items[index];

                item->item_type                  = BT_AVRCP_ITEM_TYPER_MEDIA_PLAYER;
                item->item_length                = 0;
                item->player_item.player_id      = 1;
                item->player_item.player_type    = 1;
                item->player_item.player_subtype = 0;
                item->player_item.play_status    = 0;
                item->player_item.player_type    = 1;
				/* set feature mask according to TSPX_player_feature_bitmask defined in IXIT */
				memcpy(&item->player_item.feature_mask[0], feature_mask, 16);
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
	if (err) {
		return;
	}

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

            if (player_id != 1) {
                rsp.status = BT_AVRCP_BOW_ERROR_INVALID_PLAYER_ID;
                len= 1;
            } else {
                rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
                rsp.uid_counter  = 1;
                rsp.num_of_items = 2;
                rsp.char_set     = 0x006A;
                rsp.folder_depth = 0; /* Setting to ROOT folder */
                len = sizeof(rsp);
            }

            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp, len);
            break;
        }

        case BT_AVRCP_PDU_ID_CHANGE_PATH:
        {
            struct bt_avrcp_change_path_rsp rsp;

            rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            rsp.num_of_items = 1;

            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp, sizeof(rsp));
            break;
        }

        case BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES:
        {
            struct bt_avrcp_get_item_attrs_rsp rsp;

            rsp.status      = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            rsp.num_of_attr = 0;
            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp, sizeof(rsp));
            break;
        }

        case BT_AVRCP_PDU_ID_SEARCH:
        {
            struct bt_avrcp_search_rsp rsp;

            rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            rsp.uid_counter  = 1;
            rsp.num_of_items = 1;
            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp, sizeof(rsp));
            break;
        }

        case BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS:
        {
			struct bt_avrcp_get_total_num_of_items_rsp rsp;

            rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
            rsp.uid_counter  = 1;
            rsp.num_of_items = 2;
            bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp, sizeof(rsp));
            break;
        }

        default:
        {
            uint8_t rsp_param;
			
			cmd->header.pdu_id = BT_AVRCP_PDU_ID_GENERAL_REJECT;
            rsp_param = BT_AVRCP_METADATA_ERROR_INVALID_COMMAND;
			
			bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp_param, sizeof(rsp_param));
            break;
        }
    }
}
#endif

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
void avrcp_browsing_rsp_received(struct bt_conn *conn, struct bt_avrcp_browsing_rsp *rsp, int err)
{
    if (err) {
        return;
    }

    switch (rsp->header.pdu_id)
    {
		case BT_AVRCP_PDU_ID_GET_FOLDER_ITEMS:
        {
			for (uint8_t index = 0U; index < rsp->folder_items.num_of_items; index++) {
				if (rsp->folder_items.items[index].item_type == 0x03) {
					/* Media Element Item */
					UIDCounter = rsp->folder_items.uid_counter;
					memcpy(MediaItemUID, rsp->folder_items.items[index].media_item.media_uid, MEDIA_ITEM_UID_LENGTH);
				} else if ((rsp->folder_items.items[index].item_type == 0x02) && (rsp->folder_items.items[index].folder_item.playable)) {
					/* Folder Item */
					UIDCounter = rsp->folder_items.uid_counter;
					memcpy(MediaItemUID, rsp->folder_items.items[index].folder_item.folder_uid, MEDIA_ITEM_UID_LENGTH);
				} else {
					/* misra */
				}
			}
            break;
        }

        case BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER:
        {
		    UIDCounter = rsp->folder_items.uid_counter;
            break;
        }

        case BT_AVRCP_PDU_ID_CHANGE_PATH:
        case BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES:
		case BT_AVRCP_PDU_ID_SEARCH:
		case BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS:
        {
            break;
        }

        default:
            break;
    }
}
#endif

static uint8_t avrcp_control_connect(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
	const struct btp_avrcp_control_connect_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		status = bt_avrcp_control_connect(conn);
	}
	
	return status;
}

static uint8_t avrcp_control_disconnect(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
	const struct btp_avrcp_control_disconnect_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		status = bt_avrcp_control_disconnect(conn);
	}
	
	return status;
}

static uint8_t avrcp_browsing_connect(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
	const struct btp_avrcp_browsing_connect_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		status = bt_avrcp_browsing_connect(conn);
	}
	
	return status;
}

static uint8_t avrcp_browsing_disconnect(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{
	const struct btp_avrcp_browsing_disconnect_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		status = bt_avrcp_browsing_disconnect(conn);
	}
	
	return status;
}

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
static uint8_t avrcp_send_pass_through(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{  
	const struct btp_avrcp_send_pass_through_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		status = bt_avrcp_send_passthrough(conn, cp->op_id, cp->vendor_op_id, cp->state_flag);
	}
	
	return status;
}

static uint8_t avrcp_send_vendor_dependent(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{  
	int status;
	const struct btp_avrcp_send_vendor_dependent_cmd *cp = cmd;
	struct bt_conn *conn;
	uint8_t param;
	uint16_t palyer_id;
	struct bt_avrcp_player_app_setting_attr_ids attr_id_param;
	struct bt_avrcp_get_player_app_setting_value_text value_text;
	struct bt_avrcp_player_app_attr_values attr_values;
	struct bt_avrcp_inform_displayable_char_set char_set;
	struct bt_avrcp_get_element_attrs element_attrs;
	struct bt_avrcp_register_ntfy register_ntfy;
	struct bt_avrcp_play_item play_item;
	struct bt_avrcp_add_to_now_playing add_2_now_playing;
  	  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		switch (cp->pdu_id) 
		{
			case BT_AVRCP_PDU_ID_GET_CAPABILITY:
				param = 0x02;	/* valid param is: 0x2 and 0x3 */
			case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
			case BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS:
			case BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE:
			case BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE:
			case BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
				param = 0x01;
					
				status = bt_avrcp_send_vendor_dependent(conn, cp->pdu_id, &param);
				break;
					
			case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
			case BT_AVRCP_PDU_ID_GET_PLAY_STATUS:
				status = bt_avrcp_send_vendor_dependent(conn, cp->pdu_id, NULL);
				break;

			case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
			case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT:
				attr_id_param.num_of_attr = 1;
				attr_id_param.attr_ids[0] = 0x01;	
					
				status = bt_avrcp_send_vendor_dependent(conn, cp->pdu_id, &attr_id_param);
				break;				
			  
			case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
				value_text.attr_id = 0x01;
				value_text.num_of_value = 0x01;
				value_text.value_ids[0] = 0x01;

				status = bt_avrcp_send_vendor_dependent(conn, cp->pdu_id, &value_text);
				break;
			  
			case BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL:
				attr_values.num_of_attr = 0x01;
				attr_values.attr_vals[0].attr_id = 0x01;
				attr_values.attr_vals[0].value_id = 0x01;

				status = bt_avrcp_send_vendor_dependent(conn, cp->pdu_id, &attr_values);
				break;
			  
			case BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:
				char_set.num_of_char = 0x01;
				char_set.char_sets[0] = 0x01;

				status = bt_avrcp_send_vendor_dependent(conn, cp->pdu_id, &char_set);
				break;
			  
			case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
				memset(&element_attrs.identifier[0], 0, 8);
				element_attrs.num_of_attr = 0x00;		/* return all attribute */

				status = bt_avrcp_send_vendor_dependent(conn, cp->pdu_id, &element_attrs);
				break;
			  
			case BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER:
				palyer_id = 0x01;

				status = bt_avrcp_send_vendor_dependent(conn, cp->pdu_id, &palyer_id);
				break;
				  
			case BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION:
				register_ntfy.event_id = cp->param[0];
				register_ntfy.playback_interval = 0x00;

				status = bt_avrcp_send_vendor_dependent(conn, cp->pdu_id, &register_ntfy);
				break;
			  
			case BT_AVRCP_PDU_ID_PLAY_ITEMS:
				play_item.scope = cp->param[0];
				memcpy(&play_item.uid[0], MediaItemUID, MEDIA_ITEM_UID_LENGTH);
				play_item.uid_counter = UIDCounter;

				status = bt_avrcp_send_vendor_dependent(conn, cp->pdu_id, &play_item);
				break;

			case BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING:
				add_2_now_playing.scope = cp->param[0];
				memcpy(&add_2_now_playing.uid[0], MediaItemUID, MEDIA_ITEM_UID_LENGTH);
				add_2_now_playing.uid_counter = UIDCounter;

				status = bt_avrcp_send_vendor_dependent(conn, cp->pdu_id, &add_2_now_playing);
				break;
						
			default:
				status = BTP_STATUS_FAILED;
				break;
		}
	}

	return status;
}

static uint8_t avrcp_set_browsed_player(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{  
	const struct btp_avrcp_set_browsed_player_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		status = bt_avrcp_set_borwsed_player(conn, cp->player_id);
	}
	
	return status;
}

static uint8_t bt_avrcp_sdp_user(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
    int status;
    uint8_t *handle = NULL;
    struct bt_avrcp_cover_art_connect param;
    uint16_t l2cappsm     = 0;
    if ((result) && (result->resp_buf))
    {
        bt_sdp_get_addl_proto_param(result->resp_buf, BT_SDP_PROTO_L2CAP, 0x01, &l2cappsm);
        if(l2cappsm > 0)
        {
            status = bt_avrcp_cover_art_start_initiator(handle);
            if (status == 0)
            {
                param.l2cap_rpsm = l2cappsm;
                param.max_recv_size = 512;
                status = bt_avrcp_cover_art_connect(*handle, conn, &param);
            }
            return BT_SDP_DISCOVER_UUID_STOP;
        }
    }
    return BT_SDP_DISCOVER_UUID_STOP;
}

static struct bt_sdp_discover_params discov_avrcp = {
    .uuid = BT_UUID_DECLARE_16(BT_SDP_AV_REMOTE_TARGET_SVCLASS),
    .func = bt_avrcp_sdp_user,
    .pool = &sdp_client_pool,
};

static uint8_t avrcp_cover_art_initiator(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
    const struct btp_avrcp_cover_art_initiator_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;

	conn = bt_conn_lookup_addr_br(&cp->address.a);

	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
        status = bt_sdp_discover(conn, &discov_avrcp);
    }
    return status;
}

static uint8_t avrcp_get_folder_items(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{  
	const struct btp_avrcp_get_folder_items_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
	struct bt_avrcp_get_folder_items_cmd param;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
        param.scope = cp->scope;
        param.start_item = cp->start_item;
        param.end_item = cp->end_item;
        param.attr_count = cp->attr_count;
        param.attr_list[0] = cp->attr_list[0];

        status = bt_avrcp_get_folder_items(conn, &param);
	}

	return status;
}

static uint8_t avrcp_change_path(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{  
	const struct btp_avrcp_change_path_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
	struct bt_avrcp_change_path_cmd param;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		param.uid_counter = UIDCounter;
		param.direction = cp->direction;
		memcpy(&param.folder_uid[0], MediaItemUID, MEDIA_ITEM_UID_LENGTH);

		status = bt_avrcp_change_path(conn, &param);
	}
	
	return status;
}

static uint8_t avrcp_get_item_attributes(const void *cmd, uint16_t cmd_len, void *rsp, uint16_t *rsp_len)
{  
	const struct btp_avrcp_get_item_attrs_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
	struct bt_avrcp_get_item_attrs_cmd param;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		param.scope = cp->scope;
		memcpy(&param.uid[0], MediaItemUID, MEDIA_ITEM_UID_LENGTH);
		param.uid_counter = UIDCounter;
        param.num_of_attr = cp->num_of_attr;
        param.attr_list[0] = cp->attr_list[0];

        status = bt_avrcp_get_items_attribute(conn, &param);
	}
	
	return status;
}

static uint8_t avrcp_search(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{  
	const struct btp_avrcp_search_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
	struct bt_avrcp_search_cmd param;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
	    param.char_set = 0x6A;	/* UTF-8 */
		param.length = cp->length;
		param.str = (void *)(&cp->str[0]);

		status = bt_avrcp_search(conn, &param);
	}
	
	return status;
}

static uint8_t avrcp_get_total_num_of_items(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{  
	const struct btp_avrcp_get_total_num_of_items_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
    if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		status = bt_avrcp_get_total_num_of_items(conn, cp->scope);
	}
	
	return status;
}

static uint8_t avrcp_send_unit_info(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{  
	const struct btp_avrcp_send_unit_info_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		status = bt_avrcp_send_unit_info(conn);
	}
	
	return status;
}

static uint8_t avrcp_send_subunit_info(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
	const struct btp_avrcp_send_subunit_info_cmd *cp = cmd;
	struct bt_conn *conn;
	int status;
  
	conn = bt_conn_lookup_addr_br(&cp->address.a);
  
	if (!conn) {
		status = BTP_STATUS_FAILED;
	} else {
		status = bt_avrcp_send_subunit_info(conn);
	}
	
	return status;
}
#endif /* CONFIG_BT_AVRCP_CT > 0U */

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
static uint8_t avrcp_handle_register_notification(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
	const struct btp_avrcp_handle_register_notification_cmd *cp = cmd;
	
	switch (cp->event_id) 
	{
		case BT_AVRCP_EVENT_TRACK_CHANGED:
			if (cp->status == 0x00) {
				/* CHANGED */
				memset(trackID, 0x00, 8);
				trackID[0] = 0x01;
			} else if (cp->status == 0x01) {
				/* No Selected Track */
				memset(trackID, 0xFF, 8);
			} else {
				/* Default trackID is 0x0000000000000000, Playing Track */
			}
			break;
		  
		default:
			break;
	}
	
	return 0;
}
#endif /* CONFIG_BT_AVRCP_TG > 0 */

static const struct btp_handler handlers[] = {
 	{
		.opcode = BTP_AVRCP_CONTROL_CONNECT,
		.expect_len = sizeof(struct btp_avrcp_control_connect_cmd),
		.func = avrcp_control_connect,
	},
 	{
		.opcode = BTP_AVRCP_CONTROL_DISCONNECT,
		.expect_len = sizeof(struct btp_avrcp_control_disconnect_cmd),
		.func = avrcp_control_disconnect,
	},
 	{
		.opcode = BTP_AVRCP_BROWSING_CONNECT,
		.expect_len = sizeof(struct btp_avrcp_browsing_connect_cmd),
		.func = avrcp_browsing_connect,
	},
 	{
		.opcode = BTP_AVRCP_BROWSING_DISCONNECT,
		.expect_len = sizeof(struct btp_avrcp_browsing_disconnect_cmd),
		.func = avrcp_browsing_disconnect,
	},
#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
 	{
		.opcode = BTP_AVRCP_SEND_PASS_THROUGH,
		.expect_len = sizeof(struct btp_avrcp_send_pass_through_cmd),
		.func = avrcp_send_pass_through,
	},
 	{
        .opcode = BTP_AVRCP_SEND_VENDOR_DEPENDENT,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
        .func = avrcp_send_vendor_dependent,
	},
 	{
		.opcode = BTP_AVRCP_SET_BROWSED_PLAYER,
		.expect_len = sizeof(struct btp_avrcp_set_browsed_player_cmd),
		.func = avrcp_set_browsed_player,
	},
 	{
		.opcode = BTP_AVRCP_GET_FOLDER_ITEMS,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
		.func = avrcp_get_folder_items,
	},
 	{
		.opcode = BTP_AVRCP_CHANGE_PATH,
		.expect_len = sizeof(struct btp_avrcp_change_path_cmd),
		.func = avrcp_change_path,
	},
 	{
        .opcode = BTP_AVRCP_GET_ITEM_ATTRIBUTES,
        .expect_len = BTP_HANDLER_LENGTH_VARIABLE,
		.func = avrcp_get_item_attributes,
	},
 	{
		.opcode = BTP_AVRCP_SEARCH,
		.expect_len = BTP_HANDLER_LENGTH_VARIABLE,
		.func = avrcp_search,
	},
 	{
		.opcode = BTP_AVRCP_GET_TOTAL_NUM_OF_ITEMS,
		.expect_len = sizeof(struct btp_avrcp_get_total_num_of_items_cmd),
		.func = avrcp_get_total_num_of_items,
	},
 	{
		.opcode = BTP_AVRCP_SEND_UNIT_INFO,
		.expect_len = sizeof(struct btp_avrcp_send_unit_info_cmd),
		.func = avrcp_send_unit_info,
	},
 	{
		.opcode = BTP_AVRCP_SEND_SUBUNIT_INFO,
		.expect_len = sizeof(struct btp_avrcp_send_subunit_info_cmd),
		.func = avrcp_send_subunit_info,
	},
    {
        .opcode = BTP_COVER_ART_START_INITIATOR,
        .expect_len = sizeof(struct btp_avrcp_cover_art_initiator_cmd),
        .func = avrcp_cover_art_initiator,
    },
#endif /* CONFIG_BT_AVRCP_CT > 0 */
#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
 	{
		.opcode = BTP_AVRCP_HANDLE_REGISTER_NOTIFICATION_ID,
		.expect_len = sizeof(struct btp_avrcp_handle_register_notification_cmd),
		.func = avrcp_handle_register_notification,
	},
#endif /* CONFIG_BT_AVRCP_TG > 0 */
};

#if defined(CONFIG_BT_AVRCP_CT) && (CONFIG_BT_AVRCP_CT > 0)
/* AVRCP Controller SDP service record */
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
#endif

#if defined(CONFIG_BT_AVRCP_TG) && (CONFIG_BT_AVRCP_TG > 0)
/* AVRCP Target SDP service record */
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
                BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
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
#endif

static struct bt_avrcp_cb avrcp_cbs = {avrcp_control_connected, 
							avrcp_control_disconnected,
							avrcp_browsing_connected,
							avrcp_browsing_disconnected,
							avrcp_send_result,
#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
							avrcp_control_received,
#endif
#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
							avrcp_control_rsp_received,
							avrcp_vendor_dependent_continue_rsp,
#endif
#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
							avrcp_browsing_received,
#endif
#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))
							avrcp_browsing_rsp_received
#endif
};

uint8_t tester_init_avrcp(void)
{	
	/* init avrcp controller */
#if defined(CONFIG_BT_AVRCP_CT) && (CONFIG_BT_AVRCP_CT > 0)
	/* register AVRCP Controller SDP Record */
	bt_sdp_register_service(&avrcp_ct_rec);
#endif

	/* init avrcp target */
#if defined(CONFIG_BT_AVRCP_TG) && (CONFIG_BT_AVRCP_TG > 0)
	/* register AVRCP Target SDP Record */
	bt_sdp_register_service(&avrcp_tg_rec);
#endif
	
	/* register appl callbacks */
    bt_avrcp_register_callback(&avrcp_cbs);
	
	tester_register_command_handlers(BTP_SERVICE_ID_AVRCP, handlers, ARRAY_SIZE(handlers));

	return BTP_STATUS_SUCCESS;
}

uint8_t tester_unregister_avrcp(void)
{
	return BTP_STATUS_SUCCESS;
}
#endif /* CONFIG_BT_AVRCP > 0 */