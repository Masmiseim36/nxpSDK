/** @file
 * @brief Bluetooth MAP shell module
 *
 * Provide some Bluetooth shell commands that can be useful to applications.
 */

/*
 * Copyright (c) 2024 NXP
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
#include <errno.h>

#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/map_mce.h>
#include <bluetooth/map_mse.h>
#include <bluetooth/sdp.h>
#include "fsl_shell.h"
#include "shell_bt.h"
#if (defined(CONFIG_BT_MAP_MSE) && ((CONFIG_BT_MAP_MSE) > 0U))
#include "ff.h"
#include "diskio.h"
#endif /* CONFIG_BT_MAP_MSE */

#if ((defined(CONFIG_BT_MAP_MCE) && ((CONFIG_BT_MAP_MCE) > 0U)) || \
     (defined(CONFIG_BT_MAP_MSE) && ((CONFIG_BT_MAP_MSE) > 0U)))
#define HELP_NONE "[none]"
#define SDP_CLIENT_USER_BUF_LEN 512U

struct map_hdr
{
    uint8_t *value;
    uint16_t length;
};

NET_BUF_POOL_FIXED_DEFINE(sdp_client_pool, CONFIG_BT_MAX_CONN, SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

static int str_to_int(char *str, uint32_t *value)
{
    char *end_ptr;

    if (str == NULL)
    {
        return -EINVAL;
    }

    if (strspn(str, "0123456789") == strlen(str))
    {
        *value = (uint32_t)strtol(str, &end_ptr, 10);
    }
    else
    {
        *value = (uint32_t)strtol(str, &end_ptr, 16);
    }
    if (*end_ptr != '\0')
    {
        return -EINVAL;
    }
    return 0;
}
#endif /* CONFIG_BT_MAP_MCE || CONFIG_BT_MAP_MSE */

#if (defined(CONFIG_BT_MAP_MCE) && ((CONFIG_BT_MAP_MCE) > 0U))

#define APP_MAX_NAME_LEN (64U)
#if (APP_MAX_NAME_LEN < BT_MAP_MSG_HANDLE_SIZE)
#error "APP_MAX_NAME_LEN should be greater than or equal to BT_MAP_MSG_HANDLE_SIZE."
#endif

#define MAP_MCE_CLASS_OF_DEVICE (0x10010CU) /* Object Transfer, Computer, Laptop */

#define MAP_MCE_MAS_TX_NET_BUF_COUNT   (1U)
#define MAP_MCE_MAS_TX_NET_BUF_SIZE    (1024U + 2U) /* L2CAP I-frame Enhanced Control Field(2-byte) */

static struct bt_sdp_attribute map_mce_attrs[] = {
    BT_SDP_NEW_SERVICE,
    /* ServiceClassIDList */
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_MAP_MCE_SVCLASS) //11 33
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 17), //35, 11
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), // 35 , 3
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) //01 00
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),// 35 05
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM), // 00 03
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
                BT_SDP_ARRAY_8(BT_RFCOMM_CHAN_MAP_MCE) //RFCOMM CHANNEL
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),// 35 03
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTP_OBEX) // 00 08
            },
            )
        },
        )
    ),
        /* BluetoothProfileDescriptorList */
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_MAP_SVCLASS) //11 34
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0104U) //01 04
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("MAP MNS-name"),
    /* GoepL2CapPsm */
    BT_SDP_ATTR_GOEP_L2CAP_PSM,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
        BT_SDP_ARRAY_16(BT_BR_PSM_MAP_MCE)
    },
    /*  SupportedFeatures */
    BT_SDP_ATTR_MAP_SUPPORTED_FEATURES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT32),
        BT_SDP_ARRAY_32(CONFIG_BT_MAP_MCE_MAS_SUPPORTED_FEATURES)
    },
};

static struct bt_sdp_record map_mce_rec = BT_SDP_RECORD(map_mce_attrs);

/* The following message is encoded by G-7bit including twenty "Bluetooth MAP Test!".
The message can be used for testing with Android phone and boards with MSE function.
The message may not delivered successfully by MSE(e.g. Android phone) because the
phone number is 0000000000000. If wanting MSE to deliver this message successfully,
users need to modify the phone number(FN, N, TEL and 000000000000f0 in the message)
to the valid and modify the length(1080) accordingly.
For example, if users want to deliver the messsage to the phone number 123456,
FN, N and TEL shall be +123456, 000000000000f0 shall be 214365 and
the length(1080) shall be 1048(1080 - (14 - 6) * 4).
For example, If users want to deliver the messsage to the phone number 1234567,
FN, N and TEL shall be +1234567, 000000000000f0 shall be 214365f7 and
the length(1080) shall be 1056(1080 - (14 - 8) * 4). */
#define MAP_MCE_MSG_G_7BIT \
"BEGIN:BMSG\r\n\
VERSION:1.0\r\n\
STATUS:READ\r\n\
TYPE:SMS_GSM\r\n\
FOLDER:\r\n\
BEGIN:BENV\r\n\
BEGIN:VCARD\r\n\
VERSION:3.0\r\n\
FN:+0000000000000\r\n\
N:+0000000000000\r\n\
TEL:+0000000000000\r\n\
END:VCARD\r\n\
BEGIN:BBODY\r\n\
ENCODING:G-7BIT\r\n\
LENGTH:1080\r\n\
BEGIN:MSG\r\n\
0041000d91000000000000f00000a0050003080401622e90905d2fd3df6f3a1ad40c4241d4f29c1e52c85c2021bb5ea6bfdf7434a8198482a8e5393da498b9404276bd4c7fbfe9685033080551cb737a4841738184ec7a99fe7ed3d1a066100aa296e7f490a2e60209d9f532fdfda6a341cd2014442dcfe92185cd0512b2eb65fafb4d47839a4128885a9ed3438a9b0b2464d7cbf4f79b8e063583\r\n\
END:MSG\r\n\
BEGIN:MSG\r\n\
0041000d91000000000000f00000a0050003080402a0206a794e0f29702e90905d2fd3df6f3a1ad40c4241d4f29c1e52e45c2021bb5ea6bfdf7434a8198482a8e5393da488c15c2021bb5ea6bfdf7434a8198482a8e5393da488c55c2021bb5ea6bfdf7434a8198482a8e5393da488c95c2021bb5ea6bfdf7434a8198482a8e5393da488cd5c2021bb5ea6bfdf7434a8198482a8e5393da488d15c\r\n\
END:MSG\r\n\
BEGIN:MSG\r\n\
0041000d91000000000000f00000a0050003080403404276bd4c7fbfe9685033080551cb737a4811abb9404276bd4c7fbfe9685033080551cb737a4811b3b9404276bd4c7fbfe9685033080551cb737a4811bbb9404276bd4c7fbfe9685033080551cb737a4811c3b9404276bd4c7fbfe9685033080551cb737a4811cbb9404276bd4c7fbfe9685033080551cb737a482183b9404276bd4c7fbfe9\r\n\
END:MSG\r\n\
BEGIN:MSG\r\n\
0041000d91000000000000f0000012050003080404d0a066100aa296e7f410\r\n\
END:MSG\r\n\
END:BBODY\r\n\
END:BENV\r\n\
END:BMSG"

/* The following message is encoded by UTF-8 including twenty "Bluetooth MAP Test!".
The message can be used for testing with iPhone and boards with MSE function.
The message may not delivered successfully by MSE(e.g. iPhone) because the
phone number is 0000000000000. If wanting MSE to deliver this message successfully,
users need to modify the phone number(FN, N, TEL).
For example, if users want to deliver the messsage to the phone number 123456,
FN, N and TEL shall be +123456. */
#define MAP_MCE_MSG_UTF_8 \
"BEGIN:BMSG\r\n\
VERSION:1.0\r\n\
STATUS:UNREAD\r\n\
TYPE:SMS_GSM\r\n\
FOLDER:\r\n\
BEGIN:VCARD\r\n\
VERSION:2.1\r\n\
N;CHARSET=UTF-8:\r\n\
TEL;CHARSET=UTF-8:\r\n\
END:VCARD\r\n\
BEGIN:BENV\r\n\
BEGIN:VCARD\r\n\
VERSION:2.1\r\n\
FN;CHARSET=UTF-8:+0000000000000\r\n\
N;CHARSET=UTF-8:+0000000000000\r\n\
TEL:+0000000000000\r\n\
END:VCARD\r\n\
BEGIN:BBODY\r\n\
CHARSET:UTF-8\r\n\
LANGUAGE:UNKNOWN\r\n\
LENGTH:492\r\n\
BEGIN:MSG\r\n\
1. Bluetooth MAP Test!\n\
2. Bluetooth MAP Test!\n\
3. Bluetooth MAP Test!\n\
4. Bluetooth MAP Test!\n\
5. Bluetooth MAP Test!\n\
6. Bluetooth MAP Test!\n\
7. Bluetooth MAP Test!\n\
8. Bluetooth MAP Test!\n\
9. Bluetooth MAP Test!\n\
10. Bluetooth MAP Test!\n\
11. Bluetooth MAP Test!\n\
12. Bluetooth MAP Test!\n\
13. Bluetooth MAP Test!\n\
14. Bluetooth MAP Test!\n\
15. Bluetooth MAP Test!\n\
16. Bluetooth MAP Test!\n\
17. Bluetooth MAP Test!\n\
18. Bluetooth MAP Test!\n\
19. Bluetooth MAP Test!\n\
20. Bluetooth MAP Test!\r\n\
END:MSG\r\n\
END:BBODY\r\n\
END:BENV\r\n\
END:BMSG"

struct app_map_mce_instance
{
    struct bt_conn *acl_conn;
    struct bt_map_mce_mas *mce_mas;
    struct bt_map_mce_mns *mce_mns;
    uint32_t supported_features;
    uint16_t max_pkt_len; /* range from opcode to the end of packet */
    uint16_t mns_max_pkt_len; /* range from opcode to the end of packet */
    uint16_t tx_cnt;
    uint16_t goep_version;
    uint16_t map_version;
    uint16_t psm;
    uint8_t scn;
    uint8_t mas_instance_id;
    uint8_t state;
    uint8_t num_srmp_wait;
};

struct map_tag
{
    uint8_t *value;
    uint8_t length;
};

static uint8_t sdp_map_mse_user(struct bt_conn *conn, struct bt_sdp_client_result *result);
static void map_mce_mns_connected(struct bt_map_mce_mns *mce_mns);
static void map_mce_mns_disconnected(struct bt_map_mce_mns *mce_mns, uint8_t result);
static void app_mce_send_event_cb(struct bt_map_mce_mns *mce_mns, struct net_buf *buf, enum bt_obex_req_flags flag);
static void map_mce_mas_connected(struct bt_map_mce_mas *mce_mas);
static void map_mce_mas_disconnected(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_abort_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_get_folder_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_set_folder_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_get_msg_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_get_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_set_msg_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_push_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, char *name);
static void app_mce_set_ntf_reg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_update_inbox_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_get_mas_inst_info_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_set_owner_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_mce_get_owner_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_get_convo_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_mce_set_ntf_filter_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);

NET_BUF_POOL_FIXED_DEFINE(mce_mas_tx_pool, MAP_MCE_MAS_TX_NET_BUF_COUNT, BT_L2CAP_BUF_SIZE(MAP_MCE_MAS_TX_NET_BUF_SIZE), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
static struct app_map_mce_instance app_mce_instance;
static const char map_msg_example[] = MAP_MCE_MSG_G_7BIT;

static struct bt_map_mce_mas_cb mce_mas_cb = {
    .connected = map_mce_mas_connected,
    .disconnected = map_mce_mas_disconnected,
    .abort = app_mce_abort_cb,
    .get_folder_listing = app_mce_get_folder_listing_cb,
    .set_folder = app_mce_set_folder_cb,
    .get_msg_listing = app_mce_get_msg_listing_cb,
    .get_msg = app_mce_get_msg_cb,
    .set_msg_status = app_mce_set_msg_status_cb,
    .push_msg = app_mce_push_msg_cb,
    .set_ntf_reg = app_mce_set_ntf_reg_cb,
    .update_inbox = app_mce_update_inbox_cb,
    .get_mas_inst_info = app_mce_get_mas_inst_info_cb,
    .set_owner_status = app_mce_set_owner_status_cb,
    .get_owner_status = app_mce_get_owner_status_cb,
    .get_convo_listing = app_mce_get_convo_listing_cb,
    .set_ntf_filter = app_mce_set_ntf_filter_cb,
};

static struct bt_map_mce_mns_cb mce_mns_cb = {
    .connected = map_mce_mns_connected,
    .disconnected = map_mce_mns_disconnected,
    .send_event = app_mce_send_event_cb,
};

static struct bt_sdp_discover_params discov_map_mse = {
    .uuid = BT_UUID_DECLARE_16(BT_SDP_MAP_MSE_SVCLASS),
    .func = sdp_map_mse_user,
    .pool = &sdp_client_pool,
};

static uint8_t sdp_map_mse_user(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
    int res;
    uint16_t scn;
    uint16_t psm = 0;
    uint32_t supported_features;
    uint16_t map_version;
    const char *service_name;
    uint8_t mas_instance_id;
    uint8_t supported_msg_type;

    if ((result) && (result->resp_buf))
    {
        shell_print(ctx_shell, "sdp success callback");
        res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_RFCOMM, &scn);
        if (res < 0)
        {
            shell_print(ctx_shell, "REFCOMM channel number is not found");
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
        shell_print(ctx_shell, "REFCOMM channel number %d", scn);
        res = bt_sdp_get_goep_l2cap_psm(result->resp_buf, &psm);
        if (res < 0)
        {
            app_mce_instance.goep_version = BT_GOEP_VERSION_1_1;
            shell_print(ctx_shell, "L2CAP PSM is not found");
        }
        else
        {
            app_mce_instance.goep_version = BT_GOEP_VERSION_2_0;
            shell_print(ctx_shell, "L2CAP PSM  0x%04X", psm);
        }
        res = bt_sdp_get_profile_version(result->resp_buf, BT_SDP_MAP_SVCLASS, &map_version);
        if (res < 0)
        {
            shell_print(ctx_shell, "MAP version is not found");
        }
        else
        {
            shell_print(ctx_shell, "MAP version 0x%04X", map_version);
            app_mce_instance.map_version = map_version;
        }
        res = bt_sdp_get_pbap_map_ctn_features(result->resp_buf, &supported_features);
        if (res < 0)
        {
            switch (app_mce_instance.map_version)
            {
                case BT_MAP_VERSION_1_1:
                    app_mce_instance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V11;
                    break;
                case BT_MAP_VERSION_1_2:
                    app_mce_instance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V12;
                    break;
                case BT_MAP_VERSION_1_3:
                    app_mce_instance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V13;
                    break;
                case BT_MAP_VERSION_1_4:
                    app_mce_instance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V14;
                    break;
                default:
                    app_mce_instance.supported_features = 0;
                    break;
            }
            shell_print(ctx_shell, "Supported features is not found."
            "Use the default supported features 0x%08X\r\n", app_mce_instance.supported_features);
        }
        else
        {
            shell_print(ctx_shell, "MAP supported features 0x%08X", supported_features);
            app_mce_instance.supported_features = supported_features;
        }
        res = bt_sdp_get_instance_id(result->resp_buf, &mas_instance_id);
        if (res < 0)
        {
            shell_print(ctx_shell, "MAS instance ID is not found");
        }
        else
        {
            shell_print(ctx_shell, "MAS instance ID %d", mas_instance_id);
            app_mce_instance.mas_instance_id = mas_instance_id;
        }
        res = bt_sdp_get_supported_msg_type(result->resp_buf, &supported_msg_type);
        if (res < 0)
        {
            shell_print(ctx_shell, "Supported message type is not found");
        }
        else
        {
            shell_print(ctx_shell, "Supported message type 0x%02X", supported_msg_type);
        }
        res = bt_sdp_get_service_name(result->resp_buf, &service_name);
        if (res < 0)
        {
            shell_print(ctx_shell, "Service name is not found");
        }
        else
        {
            shell_print(ctx_shell, "Service name %s", service_name);
        }
        if ((scn != 0U) || (psm != 0U))
        {
            shell_print(ctx_shell, "Message Access Server found. Connecting ...");
            app_mce_instance.psm = psm;
            app_mce_instance.scn = scn;
            if (app_mce_instance.goep_version >= BT_GOEP_VERSION_2_0)
            {
                res = bt_map_mce_psm_connect(conn, psm, supported_features, &app_mce_instance.mce_mas);
            }
            else
            {
                res = bt_map_mce_scn_connect(conn, (uint8_t)scn, supported_features, &app_mce_instance.mce_mas);
            }
            if (0 != res)
            {
                shell_print(ctx_shell, "fail to connect MSE (err: %d)", res);
            }
        }
        return BT_SDP_DISCOVER_UUID_STOP;
    }
    else
    {
        shell_print(ctx_shell, "sdp fail callback");
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
}

static void app_print_body(struct net_buf *buf)
{
    struct map_hdr body;
    if (bt_map_mce_get_body(buf, &body.value, &body.length) == 0)
    {
        shell_print(ctx_shell, "============== BODY ==============");
        shell_print(ctx_shell, "%.*s", body.length, body.value);
        shell_print(ctx_shell, "============ END BODY ============");
    }
    else
    {
        shell_print(ctx_shell, "BODY not Found ");
    }
}

static bool app_mce_app_param_cb(struct bt_data *data, void *user_data)
{
    switch (data->type)
    {
        case BT_MAP_TAG_ID_NEW_MESSAGE:
            shell_print(ctx_shell, "New Message - %d",  data->data[0]);
            break;

        case BT_MAP_TAG_ID_MAS_INSTANCE_ID:
            shell_print(ctx_shell, "MAS Instance ID - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_FOLDER_LISTING_SIZE:
            if (data->data_len < 2U)
            {
                return false;
            }
            shell_print(ctx_shell, "Folder Listing Size - %d", sys_get_be16(data->data));
            break;

        case BT_MAP_TAG_ID_LISTING_SIZE:
            if (data->data_len < 2U)
            {
                return false;
            }
            shell_print(ctx_shell, "Listing Size - %d", sys_get_be16(data->data));
            break;

        case BT_MAP_TAG_ID_FRACTION_DELIVER:
            shell_print(ctx_shell, "Fraction Deliver - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_MSE_TIME:
            shell_print(ctx_shell, "MSE Time - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_DATABASE_IDENTIFIER:
            PRINTF("Database Identifier - ");
            for (uint8_t index = 0; index < data->data_len; index++)
            {
                PRINTF("%02X", data->data[index]);
            }
            PRINTF("\r\n");
            break;

        case BT_MAP_TAG_ID_CONV_LIST_VER_CNTR:
            shell_print(ctx_shell, "Conversation Listing Version Counter - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_PRESENCE_AVAILABILITY:
            shell_print(ctx_shell, "Presence Availability - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_PRESENCE_TEXT:
            shell_print(ctx_shell, "============== Presence Text ==============\r\n");
            shell_print(ctx_shell, "%.*s", data->data_len, data->data);
            shell_print(ctx_shell, "============ END Presence Text ============\r\n");
            break;

        case BT_MAP_TAG_ID_LAST_ACTIVITY:
            shell_print(ctx_shell, "Last Activity - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_CHAT_STATE:
            shell_print(ctx_shell, "Chat State - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_FOLDER_VER_CNTR:
            shell_print(ctx_shell, "Folder Version Counter - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_OWNER_UCI:
            shell_print(ctx_shell, "============== Owner UCI ==============");
            shell_print(ctx_shell, "%.*s", data->data_len, data->data);
            shell_print(ctx_shell, "============ END Owner UCI ============");
            break;

        case BT_MAP_TAG_ID_MAP_SUPPORTED_FEATURES:
            if (data->data_len < 4U)
            {
                return false;
            }
            shell_print(ctx_shell, "Supported Features - %d", sys_get_be32(data->data));
            break;

        default:
            break;
    }

    return true;
}

static void map_mce_mns_connected(struct bt_map_mce_mns *mce_mns)
{
    app_mce_instance.mce_mns = mce_mns;
    shell_print(ctx_shell, "MCE MNS connection");
    if (bt_map_mce_mns_get_max_pkt_len(mce_mns, &app_mce_instance.mns_max_pkt_len) == 0)
    {
        shell_print(ctx_shell, "MAX Packet Length - %d", app_mce_instance.mns_max_pkt_len);
    }
    else
    {
        shell_print(ctx_shell, "MAX Packet Length is invalid");
    }
}

static void map_mce_mns_disconnected(struct bt_map_mce_mns *mce_mns, uint8_t result)
{
    app_mce_instance.mce_mns = NULL;
    shell_print(ctx_shell, "MCE MNS disconnection - 0x%02X", result);
}

static void map_mce_mas_connected(struct bt_map_mce_mas *mce_mas)
{
    app_mce_instance.mce_mas = mce_mas;
    shell_print(ctx_shell, "MCE MAS connection");
    if (bt_map_mce_get_max_pkt_len(mce_mas, &app_mce_instance.max_pkt_len) == 0)
    {
        shell_print(ctx_shell, "MAX Packet Length - %d", app_mce_instance.max_pkt_len);
    }
    else
    {
        shell_print(ctx_shell, "MAX Packet Length is invalid");
    }
}

static void map_mce_mas_disconnected(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    app_mce_instance.mce_mas = NULL;
    shell_print(ctx_shell, "MCE MAS disconnection - 0x%02X", result);
}

static void app_mce_abort_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    shell_print(ctx_shell, "MCE Abort - 0x%02X", result);
}

static void app_mce_send_event_cb(struct bt_map_mce_mns *mce_mns, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    uint8_t result;
    shell_print(ctx_shell, "MAP Recv Send Event");
    bt_map_mce_app_param_parse(buf, app_mce_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (flag & BT_OBEX_REQ_END)
    {
        result = BT_MAP_RSP_SUCCESS;
    }
    else
    {
        result = BT_MAP_RSP_CONTINUE;
    }
    if (bt_map_mce_send_event_response(mce_mns, result, false) < 0)
    {
        shell_print(ctx_shell, "Failed to send event response");
    }
}

static void app_mce_get_folder_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    shell_print(ctx_shell, "MAP Get Folder Listing CNF - 0x%02X", result);
    bt_map_mce_app_param_parse(buf, app_mce_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        app_mce_instance.num_srmp_wait = app_mce_instance.num_srmp_wait ? app_mce_instance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_FOLDER_LISTING(app_mce_instance.mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_folder_listing(mce_mas, buf, (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            shell_print(ctx_shell, "Failed to get folder listing");
        }
    }
}

static void app_mce_set_folder_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    shell_print(ctx_shell, "MAP Set Folder CNF - 0x%02X", result);
}

static void app_mce_get_msg_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    shell_print(ctx_shell, "MAP Get MSG Listing CNF - 0x%02X", result);
    bt_map_mce_app_param_parse(buf, app_mce_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        app_mce_instance.num_srmp_wait = app_mce_instance.num_srmp_wait ? app_mce_instance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG_LISTING(app_mce_instance.mce_mas, NULL, BT_OBEX_REQ_END));
        if (bt_map_mce_get_msg_listing(mce_mas, buf, NULL, (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            shell_print(ctx_shell, "Failed to get msg listing");
        }
    }
}

static void app_mce_get_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    shell_print(ctx_shell, "MAP Get MSG CNF - 0x%02X", result);
    bt_map_mce_app_param_parse(buf, app_mce_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        app_mce_instance.num_srmp_wait = app_mce_instance.num_srmp_wait ? app_mce_instance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG(app_mce_instance.mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_msg(mce_mas, buf, NULL, (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            shell_print(ctx_shell, "Failed to get msg");
        }
    }
}

static void app_mce_set_msg_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    shell_print(ctx_shell, "MAP Set MSG Status CNF - 0x%02X", result);
    /* If the result is BT_MAP_RSP_CONTINUE and the message is not sent completely,
        call bt_map_mce_set_msg_status to continue to send */
}

static void app_mce_push_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, char *name)
{
    struct net_buf *buf;
    uint16_t actual;
    enum bt_obex_req_flags flags = BT_OBEX_REQ_END;
    uint16_t max_body_len;

    shell_print(ctx_shell, "MAP Push MSG CNF - 0x%02X", result);
    if (name != NULL)
    {
        PRINTF ("Name - %.*s\r\n", BT_MAP_MSG_HANDLE_SIZE / 2U, name);
    }

    if (result == BT_MAP_RSP_CONTINUE)
    {
        if (app_mce_instance.tx_cnt < strlen(map_msg_example))
        {
            buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
            net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_PUSH_MSG(app_mce_instance.mce_mas, NULL, flags));
            max_body_len = app_mce_instance.max_pkt_len;
            max_body_len -= BT_MAP_MCE_RSV_LEN_PUSH_MSG(app_mce_instance.mce_mas, NULL, flags) - BT_L2CAP_BUF_SIZE(2U);
            max_body_len -= sizeof(struct bt_obex_hdr_bytes);
            actual = strlen(map_msg_example) - app_mce_instance.tx_cnt;
            if (actual > max_body_len)
            {
                actual = max_body_len;
                flags = BT_OBEX_REQ_CONTINUE;
            }
            if (flags == BT_OBEX_REQ_CONTINUE)
            {
                BT_MAP_ADD_BODY(buf, (uint8_t *)(map_msg_example + app_mce_instance.tx_cnt), actual);
            }
            else
            {
                BT_MAP_ADD_END_OF_BODY(buf, (uint8_t *)(map_msg_example + app_mce_instance.tx_cnt), actual);
            }
            if (bt_map_mce_push_msg(mce_mas, buf, NULL, flags) != 0)
            {
                net_buf_unref(buf);
                shell_print(ctx_shell, "Failed to push msg");
            }
            else
            {
                app_mce_instance.tx_cnt += actual;
            }
        }
    }
}

static void app_mce_set_ntf_reg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    shell_print(ctx_shell, "MAP Set Notification Registration CNF - 0x%02X", result);
}

static void app_mce_update_inbox_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    shell_print(ctx_shell, "MAP Update Inbox CNF - 0x%02X", result);
}

static void app_mce_get_mas_inst_info_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    shell_print(ctx_shell, "MAP Get MAS Instance Info - 0x%02X", result);
    bt_map_mce_app_param_parse(buf, app_mce_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        app_mce_instance.num_srmp_wait = app_mce_instance.num_srmp_wait ? app_mce_instance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MAS_INST_INFO(app_mce_instance.mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_mas_inst_info(mce_mas, buf, (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            shell_print(ctx_shell, "Failed to get MAS instance infomation");
        }
    }
}

static void app_mce_set_owner_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    shell_print(ctx_shell, "MAP Set Owner Status CNF - 0x%02X", result);
}

static void app_mce_get_owner_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    shell_print(ctx_shell, "MAP Get Owner Status CNF - 0x%02X", result);
    bt_map_mce_app_param_parse(buf, app_mce_app_param_cb, NULL);
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        app_mce_instance.num_srmp_wait = app_mce_instance.num_srmp_wait ? app_mce_instance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_OWNER_STATUS(app_mce_instance.mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_owner_status(mce_mas, buf, (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            shell_print(ctx_shell, "Failed to get owner status");
        }
    }
}

static void app_mce_get_convo_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    shell_print(ctx_shell, "MAP Get Conversation Listing CNF - 0x%02X", result);
    bt_map_mce_app_param_parse(buf, app_mce_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        app_mce_instance.num_srmp_wait = app_mce_instance.num_srmp_wait ? app_mce_instance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_CONVO_LISTING(app_mce_instance.mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_convo_listing(mce_mas, buf, (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            shell_print(ctx_shell, "Failed to get conversation listing");
        }
    }
}

static void app_mce_set_ntf_filter_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    shell_print(ctx_shell, "MAP Set NTF Filter CNF - 0x%02X", result);
}

static void shell_map_init(void)
{
    bt_map_mce_mas_register(&mce_mas_cb);
}

static void shell_map_deinit(void)
{
    bt_map_mce_mas_unregister();
}

static int cmd_mce_register(const struct shell *sh, size_t argc, char *argv[])
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;
    int err;

    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(MAP_MCE_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        err = -ENOBUFS;
        shell_error(sh, "write class of device failed, Reason : %d", err);

    }
    bt_sdp_register_service(&map_mce_rec);
    shell_map_init();

    return 0;
}

static int cmd_mce_unregister(const struct shell *sh, size_t argc, char *argv[])
{
    shell_map_deinit();

    return 0;
}

static int cmd_mce_mns_register(const struct shell *sh, size_t argc, char *argv[])
{
    bt_map_mce_mns_register(&mce_mns_cb);
    return 0;
}

static int cmd_mce_mns_unregister(const struct shell *sh, size_t argc, char *argv[])
{
    bt_map_mce_mns_unregister();
    return 0;
}

static int cmd_mce_connect(const struct shell *sh, size_t argc, char *argv[])
{
    int err = bt_sdp_discover(default_conn, &discov_map_mse);

    if (err)
    {
        shell_error(sh, "SDP discovery failed: %d", err);
        return -EINVAL;
    }
    else
    {
        shell_print(sh, "SDP discovery started");
    }
    return 0;
}

static int cmd_mce_disconnect(const struct shell *sh, size_t argc, char *argv[])
{
    int err = bt_map_mce_disconnect(app_mce_instance.mce_mas);

    if (err < 0)
    {
        shell_error(sh, "disconnect failed Reason %d", err);
        return -EINVAL;
    }
    return 0;
}

static int cmd_mce_mns_disconnect(const struct shell *sh, size_t argc, char *argv[])
{
    int err = bt_map_mce_mns_disconnect(app_mce_instance.mce_mns);

    if (err < 0)
    {
        shell_error(sh, "disconnect MNS failed Reason %d", err);
        return -EINVAL;
    }
    return 0;
}

static int cmd_abort(const struct shell *sh, size_t argc, char *argv[])
{
    int err = bt_map_mce_abort(app_mce_instance.mce_mas);

    if (err == -EINPROGRESS)
    {
        shell_print(sh, "abort is pending");
    }
    else if (err < 0)
    {
        shell_error(sh, "abort failed Reason %d", err);
        return -EINVAL;
    }
    return 0;
}

static int cmd_get_folder_list(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    struct net_buf *buf;
    uint32_t index;
    uint32_t tag_u32;

    if ((app_mce_instance.supported_features & (BT_MAP_BROWSING_FEATURE | BT_MAP_UPLOADING_FEATURE)) == 0)
    {
        shell_error(ctx_shell, "Broswing and Uploading feature not supported by partner");
        return -EINVAL;
    }
    app_mce_instance.num_srmp_wait = 0;
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_FOLDER_LISTING(app_mce_instance.mce_mas, BT_OBEX_REQ_UNSEG));
    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-srmp") == 0)
        {
            if (str_to_int(argv[index + 1U], &tag_u32) == 0)
            {
                app_mce_instance.num_srmp_wait = (uint8_t)tag_u32;
            }
            else
            {
                shell_error(ctx_shell, "SRMP invaild");
                break;
            }
        }
        else if (strcmp(argv[index], "-mlc") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 0xFFFFU))
            {
                BT_MAP_ADD_MAX_LIST_COUNT(buf, tag_u32);
            }
            else
            {
                shell_error(ctx_shell, "MAX_LIST_COUNT invaild");
                break;
            }
        }
        else if (strcmp(argv[index], "-lso") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 0xFFFFU))
            {
                BT_MAP_ADD_LIST_START_OFFSET(buf, tag_u32);
            }
            else
            {
                shell_error(ctx_shell, "LIST_START_OFFSET invaild");
                break;
            }
        }
        else
        {
            /* no action */
        }
    }

    if (index < argc - 1)
    {
        net_buf_unref(buf);
        return -EINVAL;
    }

    err = bt_map_mce_get_folder_listing(app_mce_instance.mce_mas, buf, (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG);
    if (err != 0)
    {
        net_buf_unref(buf);
        shell_error(sh, "get folder listing failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

static int cmd_set_folder(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    uint32_t index;
    char name[APP_MAX_NAME_LEN] = {'\0'}; /* string */

    if ((app_mce_instance.supported_features & (BT_MAP_BROWSING_FEATURE | BT_MAP_UPLOADING_FEATURE)) == 0)
    {
        shell_error(ctx_shell, "Broswing and Uploading feature not supported by partner");
        return -EINVAL;
    }
    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-name") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (strlen(argv[index + 1U]) < APP_MAX_NAME_LEN)
                {
                    strcpy(&name[0], argv[index + 1U]);
                }
                else
                {
                    shell_error(ctx_shell, "Name length greater than APP_MAX_NAME_LEN");
                    return -EINVAL;
                }
            }
            else
            {
                shell_error(ctx_shell, "Name invaild");
                return -EINVAL;
            }
        }
        else
        {
            /* no action */
        }
    }
    err = bt_map_mce_set_folder(app_mce_instance.mce_mas, &name[0]);
    if (err != 0)
    {
        shell_error(sh, "set folder failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

static int cmd_get_msg_list_param(size_t argc, char *argv[], struct net_buf *buf, uint32_t *param_len)
{
    uint32_t index;
    uint32_t tag_u32;
    *param_len = 0;

    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-srmp") == 0)
        {
            if (str_to_int(argv[index + 1U], &tag_u32) == 0)
            {
                app_mce_instance.num_srmp_wait = (uint8_t)tag_u32;
            }
            else
            {
                shell_error(ctx_shell, "SRMP invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-mlc") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 0xFFFFU))
            {
                if (param_len != NULL)
                {
                    *param_len += 2U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_MAX_LIST_COUNT(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "MAX_LIST_COUNT invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-lso") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 0xFFFFU))
            {
                if (param_len != NULL)
                {
                    *param_len += 2U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_LIST_START_OFFSET(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "LIST_START_OFFSET invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-fmt") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 0x1F))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_MESSAGE_TYPE(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_MESSAGE_TYPE invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-fpb") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_PERIOD_BEGIN(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_PERIOD_BEGIN invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-fpe") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_PERIOD_END(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_PERIOD_END invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-frs") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 2U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_READ_STATUS(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_READ_STATUS invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-fr") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_RECIPIENT(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_RECIPIENT invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-fo") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_ORIGINATOR(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_ORIGINATOR invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-fp") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 2U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_PRIORITY(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_PRIORITY invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-pm") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 0x1FFFFFU))
            {
                if (param_len != NULL)
                {
                    *param_len += 4U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_PARAMETER_MASK(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "PARAMETER_MASK invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-sl") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 >= 1U) && (tag_u32 <= 255U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_SUBJECT_LENGTH(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "SUBJECT_LENGTH invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-ci") == 0)
        {
            if ((argv[index + 1U] != NULL) && (strlen(argv[index + 1U]) < BT_MAP_CONVO_ID_SIZE) &&
                (strspn(argv[index + 1U], "0123456789ABCDEF") == strlen(argv[index + 1U])))
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_CONVERSATION_ID(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "CONVERSATION_ID invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-fmh") == 0)
        {
            if ((argv[index + 1U] != NULL) && (strspn(argv[index + 1U], "0123456789ABCDEF") == strlen(argv[index + 1U])))
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_MSG_HANDLE(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_MSG_HANDLE invaild");
                return -EINVAL;
            }
        }
        else
        {
            /* no action */
        }
    }

    if ((param_len != NULL) && (*param_len != 0))
    {
        *param_len += sizeof(struct bt_obex_hdr_bytes); /* application parameter header */
    }

    return 0;
}

static int cmd_get_msg_list(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    struct net_buf *buf;
    uint16_t max_body_len;
    uint32_t index;
    uint32_t param_len;
    char name[APP_MAX_NAME_LEN] = {'\0'}; /* string */

    if ((app_mce_instance.supported_features & BT_MAP_BROWSING_FEATURE) == 0)
    {
        shell_error(ctx_shell, "Broswing feature not supported by partner");
        return -EINVAL;
    }

    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-name") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (strlen(argv[index + 1U]) < APP_MAX_NAME_LEN)
                {
                    strcpy(&name[0], argv[index + 1U]);
                    break;
                }
                else
                {
                    shell_error(ctx_shell, "Name length greater than APP_MAX_NAME_LEN");
                    return -EINVAL;
                }
            }
            else
            {
                shell_error(ctx_shell, "Name invaild");
                return -EINVAL;
            }
        }
    }

    app_mce_instance.num_srmp_wait = 0;
    max_body_len = app_mce_instance.max_pkt_len;
    max_body_len -= BT_MAP_MCE_RSV_LEN_GET_MSG_LISTING(app_mce_instance.mce_mas, name, BT_OBEX_REQ_UNSEG) - BT_L2CAP_BUF_SIZE(2U);
    if ((cmd_get_msg_list_param(argc, argv, NULL, &param_len) != 0) || (param_len > max_body_len))
    {
        shell_error(sh, "parameters wrong or too long");
        return -EINVAL;
    }
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG_LISTING(app_mce_instance.mce_mas, name, BT_OBEX_REQ_UNSEG));
    (void)cmd_get_msg_list_param(argc, argv, buf, NULL);

    err = bt_map_mce_get_msg_listing(app_mce_instance.mce_mas, buf, &name[0], (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG);
    if (err != 0)
    {
        net_buf_unref(buf);
        shell_error(sh, "get msg list failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

static int cmd_get_msg(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    struct net_buf *buf;
    uint32_t index;
    uint32_t tag_u32;
    char name[APP_MAX_NAME_LEN] = {'\0'}; /* string */

    if ((app_mce_instance.supported_features & BT_MAP_BROWSING_FEATURE) == 0)
    {
        shell_error(ctx_shell, "Broswing feature not supported by partner");
        return -EINVAL;
    }
    app_mce_instance.num_srmp_wait = 0;
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG(app_mce_instance.mce_mas, BT_OBEX_REQ_UNSEG));
    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-srmp") == 0)
        {
            if (str_to_int(argv[index + 1U], &tag_u32) == 0)
            {
                app_mce_instance.num_srmp_wait = (uint8_t)tag_u32;
            }
            else
            {
                shell_error(ctx_shell, "SRMP invaild");
                break;
            }
        }
        else if (strcmp(argv[index], "-name") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (strlen(argv[index + 1U]) < APP_MAX_NAME_LEN)
                {
                    strcpy(&name[0], argv[index + 1U]);
                }
                else
                {
                    shell_error(ctx_shell, "Name length greater than APP_MAX_NAME_LEN");
                    break;
                }
            }
            else
            {
                shell_error(ctx_shell, "Name invaild");
                break;
            }
        }
        else if (strcmp(argv[index], "-a") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 1U))
            {
                BT_MAP_ADD_ATTACHMENT(buf, tag_u32);
            }
            else
            {
                shell_error(ctx_shell, "ATTACHMENT invaild");
                break;
            }
        }
        else if (strcmp(argv[index], "-c") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 1U))
            {
                BT_MAP_ADD_CHARSET(buf, tag_u32);
            }
            else
            {
                shell_error(ctx_shell, "CHARSET invaild");
                break;
            }
        }
        else if (strcmp(argv[index], "-fr") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 1U))
            {
                BT_MAP_ADD_FRACTION_REQUEST(buf, tag_u32);
            }
            else
            {
                shell_error(ctx_shell, "FRACTION_REQUEST invaild");
                break;
            }
        }
        else
        {
            /* no action */
        }
    }

    if (index < argc - 1)
    {
        net_buf_unref(buf);
        return -EINVAL;
    }

    err = bt_map_mce_get_msg(app_mce_instance.mce_mas, buf, &name[0], (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG);
    if (err != 0)
    {
        net_buf_unref(buf);
        shell_error(sh, "get msg failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

static int cmd_set_msg_status_param(size_t argc, char *argv[], struct net_buf *buf, uint32_t *param_len, char *name)
{
    uint32_t index;
    uint32_t tag_u32;
    *param_len = 0;

    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-name") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (strlen(argv[index + 1U]) < APP_MAX_NAME_LEN)
                {
                    if (name != NULL)
                    {
                        strcpy(&name[0], argv[index + 1U]);
                    }
                }
                else
                {
                    shell_error(ctx_shell, "Name length greater than APP_MAX_NAME_LEN");
                    return -EINVAL;
                }
            }
            else
            {
                shell_error(ctx_shell, "Name invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-si") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 2U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_STATUS_INDICATOR(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "STATUS_INDICATOR invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-sv") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 1U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_STATUS_VALUE(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "STATUS_VALUE invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-ed") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_EXTENDED_DATA(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "EXTENDED_DATA invaild");
                return -EINVAL;
            }
        }
        else
        {
            /* no action */
        }
    }

    if ((param_len != NULL) && (*param_len != 0))
    {
        *param_len += sizeof(struct bt_obex_hdr_bytes); /* application parameter header */
    }

    return 0;
}

static int cmd_set_msg_status(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    struct net_buf *buf;
    uint16_t max_body_len;
    uint32_t param_len;
    char name[APP_MAX_NAME_LEN] = {'\0'}; /* string */

    if ((app_mce_instance.supported_features & (BT_MAP_BROWSING_FEATURE | BT_MAP_DELETE_FEATURE)) == 0)
    {
        shell_error(ctx_shell, "Broswing and Delete feature not supported by partner");
        return -EINVAL;
    }
    max_body_len = app_mce_instance.max_pkt_len;
    max_body_len -= BT_MAP_MCE_RSV_LEN_SET_MSG_STATUS(app_mce_instance.mce_mas, BT_OBEX_REQ_UNSEG) - BT_L2CAP_BUF_SIZE(2U);
    max_body_len -= sizeof(struct bt_obex_hdr_bytes) + 1; /* body header + '0' */
    if ((cmd_set_msg_status_param(argc, argv, NULL, &param_len, NULL) != 0) || (param_len > max_body_len))
    {
        shell_error(sh, "parameters wrong or too long");
        return -EINVAL;
    }
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_MSG_STATUS(app_mce_instance.mce_mas, BT_OBEX_REQ_UNSEG));
    (void)cmd_set_msg_status_param(argc, argv, buf, NULL, &name[0]);

    err = bt_map_mce_set_msg_status(app_mce_instance.mce_mas, buf, &name[0], BT_OBEX_REQ_UNSEG);
    if (err != 0)
    {
        net_buf_unref(buf);
        shell_error(sh, "set msg status failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

static int cmd_push_msg_param(size_t argc, char *argv[], struct net_buf *buf, uint32_t *param_len)
{
    uint32_t index;
    uint32_t tag_u32;
    *param_len = 0;

    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-a") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 1U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_ATTACHMENT(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "ATTACHMENT invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-t") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 1U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_TRANSPARENT(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "TRANSPARENT invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-r") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 1U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_RETRY(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "RETRY invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-c") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 1U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_CHARSET(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "CHARSET invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-ci") == 0)
        {
            if ((argv[index + 1U] != NULL) && (strlen(argv[index + 1U]) < BT_MAP_CONVO_ID_SIZE) &&
                (strspn(argv[index + 1U], "0123456789ABCDEF") == strlen(argv[index + 1U])))
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_CONVERSATION_ID(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "CONVERSATION_ID invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-mh") == 0)
        {
            if ((argv[index + 1U] != NULL) && (strspn(argv[index + 1U], "0123456789ABCDEF") == strlen(argv[index + 1U])))
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_MESSAGE_HANDLE(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "MESSAGE_HANDLE invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-mt") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 1U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_MODIFY_TEXT(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "MODIFY_TEXT invaild");
                return -EINVAL;
            }
        }
        else
        {
            /* no action */
        }
    }

    if ((param_len != NULL) && (*param_len != 0))
    {
        *param_len += sizeof(struct bt_obex_hdr_bytes); /* application parameter header */
    }

    return 0;
}

static int cmd_push_msg(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    struct net_buf *buf;
    enum bt_obex_req_flags flags = BT_OBEX_REQ_UNSEG;
    uint16_t max_body_len;
    uint16_t actual;
    uint32_t index;
    uint32_t param_len;
    char name[APP_MAX_NAME_LEN] = {'\0'}; /* string */

    if ((app_mce_instance.supported_features & BT_MAP_UPLOADING_FEATURE) == 0)
    {
        shell_error(ctx_shell, "Uploading feature not supported by partner");
        return -EINVAL;
    }

    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-name") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (strlen(argv[index + 1U]) < APP_MAX_NAME_LEN)
                {
                    strcpy(&name[0], argv[index + 1U]);
                    break;
                }
                else
                {
                    shell_error(ctx_shell, "Name length greater than APP_MAX_NAME_LEN");
                    return -EINVAL;
                }
            }
            else
            {
                shell_error(ctx_shell, "Name invaild");
                return -EINVAL;
            }
        }
    }

    max_body_len = app_mce_instance.max_pkt_len;
    max_body_len -= BT_MAP_MCE_RSV_LEN_PUSH_MSG(app_mce_instance.mce_mas, name, flags) - BT_L2CAP_BUF_SIZE(2U);
    max_body_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
    if ((cmd_push_msg_param(argc, argv, NULL, &param_len) != 0) || (param_len > max_body_len))
    {
        shell_error(sh, "parameters wrong or too long");
        return -EINVAL;
    }
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_PUSH_MSG(app_mce_instance.mce_mas, name, flags));
    (void)cmd_push_msg_param(argc, argv, buf, NULL);
    max_body_len -= buf->len; /* application parameters */

    actual = strlen(map_msg_example);
    if (actual > max_body_len)
    {
        actual = max_body_len;
        flags = BT_OBEX_REQ_START;
    }
    if (flags == BT_OBEX_REQ_START)
    {
        BT_MAP_ADD_BODY(buf, (uint8_t *)map_msg_example, actual);
    }
    else
    {
        BT_MAP_ADD_END_OF_BODY(buf, (uint8_t *)map_msg_example, actual);
    }
    err = bt_map_mce_push_msg(app_mce_instance.mce_mas, buf, &name[0], flags);
    if (err != 0)
    {
        net_buf_unref(buf);
        shell_error(sh, "push msg failed Reason %d", err);
        return -EINVAL;
    }
    else
    {
        app_mce_instance.tx_cnt = actual;
    }

    return 0;
}

static int cmd_set_ntf_reg(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    struct net_buf *buf;
    uint32_t index;
    uint32_t tag_u32;

    if ((app_mce_instance.supported_features & BT_MAP_NTF_REG_FEATURE) == 0)
    {
        shell_error(ctx_shell, "Notification Registration feature not supported by partner");
        return -EINVAL;
    }
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_NTF_REG(app_mce_instance.mce_mas));
    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-ns") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 1U))
            {
                BT_MAP_ADD_NOTIFICATION_STATUS(buf, tag_u32);
            }
            else
            {
                net_buf_unref(buf);
                shell_error(ctx_shell, "NOTIFICATION_STATUS invaild");
                return -EINVAL;
            }
        }
    }
    err = bt_map_mce_set_ntf_reg(app_mce_instance.mce_mas, buf);
    if (err != 0)
    {
        net_buf_unref(buf);
        shell_error(sh, "set notification registration failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

static int cmd_update_inbox(const struct shell *sh, size_t argc, char *argv[])
{
    int err;

    if ((app_mce_instance.supported_features & BT_MAP_BROWSING_FEATURE) == 0)
    {
        shell_error(ctx_shell, "Broswing feature not supported by partner");
        return -EINVAL;
    }
    err = bt_map_mce_update_inbox(app_mce_instance.mce_mas);
    if (err != 0)
    {
        shell_error(sh, "update inbox failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

static int cmd_get_mas_inst_info(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    struct net_buf *buf;
    uint32_t index;
    uint32_t tag_u32;

    if ((app_mce_instance.supported_features & BT_MAP_INST_INFO_FEATURE) == 0)
    {
        shell_error(ctx_shell, "Instance Information feature not supported by partner");
        return -EINVAL;
    }
    app_mce_instance.num_srmp_wait = 0;
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MAS_INST_INFO(app_mce_instance.mce_mas, BT_OBEX_REQ_UNSEG));
    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-srmp") == 0)
        {
            if (str_to_int(argv[index + 1U], &tag_u32) == 0)
            {
                app_mce_instance.num_srmp_wait = (uint8_t)tag_u32;
            }
            else
            {
                net_buf_unref(buf);
                shell_error(ctx_shell, "SRMP invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-mii") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 255U))
            {
                BT_MAP_ADD_MAS_INSTANCE_ID(buf, tag_u32);
            }
            else
            {
                net_buf_unref(buf);
                shell_error(ctx_shell, "MAS_INSTANCE_ID invaild");
                return -EINVAL;
            }
        }
        else
        {
            /* no action */
        }
    }
    err = bt_map_mce_get_mas_inst_info(app_mce_instance.mce_mas, buf, (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG);
    if (err != 0)
    {
        net_buf_unref(buf);
        shell_error(sh, "get MAS instance infomation failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

static int cmd_set_owner_status_param(size_t argc, char *argv[], struct net_buf *buf, uint32_t *param_len)
{
    uint32_t index;
    uint32_t tag_u32;
    *param_len = 0;

    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-pa") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 255U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_PRESENCE_AVAILABILITY(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "PRESENCE_AVAILABILITY invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-pt") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_PRESENCE_TEXT(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "PRESENCE_TEXT invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-la") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_LAST_ACTIVITY(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "LAST_ACTIVITY invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-cs") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 255U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_CHAT_STATE(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "CHAT_STATE invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-ci") == 0)
        {
            if ((argv[index + 1U] != NULL) && (strlen(argv[index + 1U]) < BT_MAP_CONVO_ID_SIZE) &&
                (strspn(argv[index + 1U], "0123456789ABCDEF") == strlen(argv[index + 1U])))
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_CONVERSATION_ID(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "CONVERSATION_ID invaild");
                return -EINVAL;
            }
        }
        else
        {
            /* no action */
        }
    }

    if ((param_len != NULL) && (*param_len != 0))
    {
        *param_len += sizeof(struct bt_obex_hdr_bytes); /* application parameter header */
    }

    return 0;
}

static int cmd_set_owner_status(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    struct net_buf *buf;
    uint16_t max_body_len;
    uint32_t param_len;

    if ((app_mce_instance.supported_features & BT_MAP_OWNER_STATUS) == 0)
    {
        shell_error(ctx_shell, "Owner Status not supported by partner");
        return -EINVAL;
    }
    max_body_len = app_mce_instance.max_pkt_len;
    max_body_len -= BT_MAP_MCE_RSV_LEN_SET_OWNER_STATUS(app_mce_instance.mce_mas, BT_OBEX_REQ_UNSEG) - BT_L2CAP_BUF_SIZE(2U);
    max_body_len -= sizeof(struct bt_obex_hdr_bytes) + 1; /* body header + '0' */
    if ((cmd_set_owner_status_param(argc, argv, NULL, &param_len) != 0) || (param_len > max_body_len))
    {
        shell_error(sh, "parameters wrong or too long");
        return -EINVAL;
    }
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_OWNER_STATUS(app_mce_instance.mce_mas, BT_OBEX_REQ_UNSEG));
    (void)cmd_set_owner_status_param(argc, argv, buf, NULL);

    err = bt_map_mce_set_owner_status(app_mce_instance.mce_mas, buf, BT_OBEX_REQ_UNSEG);
    if (err != 0)
    {
        net_buf_unref(buf);
        shell_error(sh, "set owner status failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

static int cmd_get_owner_status(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    struct net_buf *buf;
    uint32_t index;
    uint32_t tag_u32;

    if ((app_mce_instance.supported_features & BT_MAP_OWNER_STATUS) == 0)
    {
        shell_error(ctx_shell, "Owner Status not supported by partner");
        return -EINVAL;
    }
    app_mce_instance.num_srmp_wait = 0;
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_OWNER_STATUS(app_mce_instance.mce_mas, BT_OBEX_REQ_UNSEG));
    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-srmp") == 0)
        {
            if (str_to_int(argv[index + 1U], &tag_u32) == 0)
            {
                app_mce_instance.num_srmp_wait = (uint8_t)tag_u32;
            }
            else
            {
                net_buf_unref(buf);
                shell_error(ctx_shell, "SRMP invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-ci") == 0)
        {
            if ((argv[index + 1U] != NULL) && (strlen(argv[index + 1U]) < BT_MAP_CONVO_ID_SIZE) &&
                (strspn(argv[index + 1U], "0123456789ABCDEF") == strlen(argv[index + 1U])))
            {
                BT_MAP_ADD_CONVERSATION_ID(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
            }
            else
            {
                net_buf_unref(buf);
                shell_error(ctx_shell, "CONVERSATION_ID invaild");
                return -EINVAL;
            }
        }
        else
        {
            /* no action */
        }
    }
    err = bt_map_mce_get_owner_status(app_mce_instance.mce_mas, buf, (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG);
    if (err != 0)
    {
        net_buf_unref(buf);
        shell_error(sh, "get owner status failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

static int cmd_get_convo_list_param(size_t argc, char *argv[], struct net_buf *buf, uint32_t *param_len)
{
    uint32_t index;
    uint32_t tag_u32;
    *param_len = 0;

    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-srmp") == 0)
        {
            if (str_to_int(argv[index + 1U], &tag_u32) == 0)
            {
                app_mce_instance.num_srmp_wait = (uint8_t)tag_u32;
            }
            else
            {
                shell_error(ctx_shell, "SRMP invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-mlc") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 0xFFFFU))
            {
                if (param_len != NULL)
                {
                    *param_len += 2U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_MAX_LIST_COUNT(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "MAX_LIST_COUNT invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-lso") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 0xFFFFU))
            {
                if (param_len != NULL)
                {
                    *param_len += 2U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_LIST_START_OFFSET(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "LIST_START_OFFSET invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-flab") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_LAST_ACTIVITY_BEGIN(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_LAST_ACTIVITY_BEGIN invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-flae") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_LAST_ACTIVITY_END(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_LAST_ACTIVITY_END invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-frs") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 2U))
            {
                if (param_len != NULL)
                {
                    *param_len += 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_READ_STATUS(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_READ_STATUS invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-fr") == 0)
        {
            if (argv[index + 1U] != NULL)
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_FILTER_RECIPIENT(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "FILTER_RECIPIENT invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-ci") == 0)
        {
            if ((argv[index + 1U] != NULL) && (strlen(argv[index + 1U]) < BT_MAP_CONVO_ID_SIZE) &&
                (strspn(argv[index + 1U], "0123456789ABCDEF") == strlen(argv[index + 1U])))
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_CONVERSATION_ID(buf, (uint8_t *)argv[index + 1U], strlen(argv[index + 1U]) + 1U);
                }
            }
            else
            {
                shell_error(ctx_shell, "CONVERSATION_ID invaild");
                return -EINVAL;
            }
        }
        else if (strcmp(argv[index], "-cpm") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 0x7FFFU))
            {
                if (param_len != NULL)
                {
                    *param_len += strlen(argv[index + 1U]) + 1U;
                }
                if (buf != NULL)
                {
                    BT_MAP_ADD_CONV_PARAMETER_MASK(buf, tag_u32);
                }
            }
            else
            {
                shell_error(ctx_shell, "CONV_PARAMETER_MASK invaild");
                return -EINVAL;
            }
        }
        else
        {
            /* no action */
        }
    }

    if ((param_len != NULL) && (*param_len != 0))
    {
        *param_len += sizeof(struct bt_obex_hdr_bytes); /* application parameter header */
    }

    return 0;
}

static int cmd_get_convo_list(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    struct net_buf *buf;
    uint16_t max_body_len;
    uint32_t param_len;

    if ((app_mce_instance.supported_features & BT_MAP_CONVO_LISTING) == 0)
    {
        shell_error(ctx_shell, "Conversation listing not supported by partner");
        return -EINVAL;
    }
    app_mce_instance.num_srmp_wait = 0;
    max_body_len = app_mce_instance.max_pkt_len;
    max_body_len -= BT_MAP_MCE_RSV_LEN_GET_CONVO_LISTING(app_mce_instance.mce_mas, BT_OBEX_REQ_UNSEG) - BT_L2CAP_BUF_SIZE(2U);
    if ((cmd_get_convo_list_param(argc, argv, NULL, &param_len) != 0) || (param_len > max_body_len))
    {
        shell_error(sh, "parameters wrong or too long");
        return -EINVAL;
    }
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_CONVO_LISTING(app_mce_instance.mce_mas, BT_OBEX_REQ_UNSEG));
    (void)cmd_get_convo_list_param(argc, argv, buf, NULL);

    err = bt_map_mce_get_convo_listing(app_mce_instance.mce_mas, buf, (app_mce_instance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG);
    if (err != 0)
    {
        net_buf_unref(buf);
        shell_error(sh, "get conversation listing failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

static int cmd_set_ntf_filter(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    struct net_buf *buf;
    uint32_t index;
    uint32_t tag_u32;

    if ((app_mce_instance.supported_features & BT_MAP_NTF_FILTERING) == 0)
    {
        shell_error(ctx_shell, "Notification Filtering feature not supported by partner");
        return -EINVAL;
    }
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_NTF_FILTER(app_mce_instance.mce_mas));
    for (index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-nfm") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 0x7FFFU))
            {
                BT_MAP_ADD_NOTIFICATION_FILTER_MASK(buf, tag_u32);
            }
            else
            {
                net_buf_unref(buf);
                shell_error(ctx_shell, "NOTIFICATION_FILTER_MASK invaild");
                return -EINVAL;
            }
        }
    }
    err = bt_map_mce_set_ntf_filter(app_mce_instance.mce_mas, buf);
    if (err != 0)
    {
        net_buf_unref(buf);
        shell_error(sh, "set notification filter failed Reason %d", err);
        return -EINVAL;
    }

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(mce_cmds,
   SHELL_CMD_ARG(register, NULL, HELP_NONE, cmd_mce_register, 1, 0),
   SHELL_CMD_ARG(unregister, NULL, HELP_NONE, cmd_mce_unregister, 1, 0),
   SHELL_CMD_ARG(mns_register, NULL, HELP_NONE, cmd_mce_mns_register, 1, 0),
   SHELL_CMD_ARG(mns_unregister, NULL, HELP_NONE, cmd_mce_mns_unregister, 1, 0),
   SHELL_CMD_ARG(connect, NULL, "SDP first, then connect", cmd_mce_connect, 1, 0),
   SHELL_CMD_ARG(disconnect, NULL, HELP_NONE, cmd_mce_disconnect, 1, 0),
   SHELL_CMD_ARG(mns_disconnect, NULL, HELP_NONE, cmd_mce_mns_disconnect, 1, 0),
   SHELL_CMD_ARG(abort, NULL, HELP_NONE, cmd_abort, 1, 0),
   SHELL_CMD_ARG(get_folder_list, NULL,"\r\n\
               -srmp(optional) : [Single Response Mode Param (>=0)].\r\n\
               input application parameters(optional).\r\n\
               1: -mlc : [MaxListCount (0 - 0xFFFF)].\r\n\
               2: -lso : [ListStartOffset (0 - 0xFFFF)].", cmd_get_folder_list, 1, 6),
   SHELL_CMD_ARG(set_folder, NULL, "\r\n\
               -name(mandatory) : [name (\"/\" : root | \"../\" : parent | \"child\" : child | \"../child\" : parent then child)].", cmd_set_folder, 3, 0),
   SHELL_CMD_ARG(get_msg_list, NULL, "\r\n\
               -name(mandatory if getting child folder, or optional) : [name (string)].\r\n\
               -srmp(optional) : [Single Response Mode Param (>=0)].\r\n\
               input application parameters(optional).\r\n\
               1: -mlc : [MaxListCount (0 - 0xFFFF)].\r\n\
               2: -lso : [ListStartOffset (0 - 0xFFFF)].\r\n\
               3: -sl  : [SubjectLength (1 - 255)].\r\n\
               4: -pm  : [ParameterMask (0 - 0x1FFFFF)].\r\n\
               5: -fmt : [FilterMessageType (0 - 0x1F)].\r\n\
               6: -fpb : [FilterPeriodBegin (string of timestamp)].\r\n\
               7: -fpe : [FilterPeriodEnd (string of timestamp)].\r\n\
               8: -frs : [FilterReadStatus (0 : no-filter | 1: unread | 2 : read)].\r\n\
               9: -fr  : [FilterRecipient (string)].\r\n\
               10: -fo : [FilterOriginator (string)].\r\n\
               11: -fp : [FilterPriority (0 : no-filter | 1: high priority msg | 2 : non-high priority msg)].\r\n\
               12: -ci : [ConversationID (128-bit value in hex string format)].\r\n\
               13: -fmh : [FilterMessageHandle (64-bit value in hex string format)].", cmd_get_msg_list, 1, 30),
   SHELL_CMD_ARG(get_msg, NULL, "\r\n\
               -name(mandatory) : [MessageHandle (string)].\r\n\
               -srmp(optional)  : [Single Response Mode Param (>=0)].\r\n\
               input application parameters.\r\n\
               1: -a(mandatory) : [Attachment (0 : OFF | 1 : ON)].\r\n\
               2: -c(mandatory) : [Charset (0 : native | 1 : UTF-8)].\r\n\
               3: -fr(optional) : [FractionRequest (0 : first | 1 : next)].", cmd_get_msg, 3, 8),
   SHELL_CMD_ARG(set_msg_status, NULL, "\r\n\
               -name(mandatory) : [Message Handle (string)].\r\n\
               input application parameters.\r\n\
               1: -si(mandatory) : [StatusIndicator (0 : readStatus | 1 : deletedStatus | 2 : setExtendedData)].\r\n\
               2: -sv(mandatory) : [StatusValue (0 : no | 1 : yes)].\r\n\
               3: -ed(optional)  : [ExtendedData (string)].", cmd_set_msg_status, 3, 6),
   SHELL_CMD_ARG(push_msg, NULL, "\r\n\
               -name(mandatory if pushing child folder, or optional) : [name (string)].\r\n\
               input application parameters.\r\n\
               1: -t(optional)  : [Transparent (0 : OFF | 1 : ON)].\r\n\
               2: -r(optional)  : [Retry (0 : OFF | 1 : ON)].\r\n\
               3: -c(mandatory) : [Charset (0 : native | 1 : UTF-8)].\r\n\
               4: -ci(optional) : [ConversationID (128-bit value in hex string format)].\r\n\
               5: -mh(optional if Message Forwarding is supported or excluded) : [MessageHandle (string)].\r\n\
               6: -a(mandatory if MessageHandle present in request or excluded)  : [Attachment (0 : OFF | 1 : ON)].\r\n\
               7: -mt(mandatory if MessageHandle present in request or excluded) : [ModifyText (0 : REPLACE | 1 : PREPEND)].", cmd_push_msg, 1, 16),
   SHELL_CMD_ARG(set_ntf_reg, NULL, "\r\n\
               input application parameters(mandatory).\r\n\
               1: -ns : [NotificationStatus (0 : OFF | 1 : ON)].", cmd_set_ntf_reg, 3, 0),
   SHELL_CMD_ARG(update_inbox, NULL, HELP_NONE, cmd_update_inbox, 1, 0),
   SHELL_CMD_ARG(get_mas_inst_info, NULL, "\r\n\
               -srmp(optional) : [Single Response Mode Param (>=0)].\r\n\
               input application parameters(mandatory).\r\n\
               1: -mii : [MASInstanceID (0 - 255)].", cmd_get_mas_inst_info, 3, 2),
   SHELL_CMD_ARG(set_owner_status, NULL, "\r\n\
               input application parameters(at least one parameter present).\r\n\
               1: -pa : [PresenceAvailability (0 - 255)].\r\n\
               2: -pt : [PresenceText (string)].\r\n\
               3: -la : [LastActivity (string of timestamp)].\r\n\
               4: -cs : [ChatState (0 - 255)].\r\n\
               5: -ci : [ConversationID (128-bit value in hex string format)].", cmd_set_owner_status, 3, 8),
   SHELL_CMD_ARG(get_owner_status, NULL, "\r\n\
               -srmp(optional) : [Single Response Mode Param (>=0)].\r\n\
               input application parameters(optional).\r\n\
               1: -ci : [ConversationID (128-bit value in hex string format)].", cmd_get_owner_status, 1, 4),
   SHELL_CMD_ARG(get_convo_list, NULL, "\r\n\
               -srmp(optional) : [Single Response Mode Param (>=0)].\r\n\
               input application parameters(optional).\r\n\
               1: -mlc  : [MaxListCount (0 - 0xFFFF)].\r\n\
               2: -lso  : [ListStartOffset (0 - 0xFFFF)].\r\n\
               3: -flab : [FilterLastActivityBegin (string)].\r\n\
               4: -flae : [FilterLastActivityEnd (string)].\r\n\
               5: -frs  : [FilterReadStatus (0 : no-filter | 1: unread | 2 : read)].\r\n\
               6: -fr   : [FilterRecipient (string)].\r\n\
               7. -ci   : [ConversationID (128-bit value in hex string format)].\r\n\
               8: -cpm  : [ConvParameterMask (0 - 0x7FFF)].", cmd_get_convo_list, 1, 18),
   SHELL_CMD_ARG(set_ntf_filter, NULL, "\r\n\
               input application parameters(mandatory).\r\n\
               1: -nfm : [NotificationFilterMask (0 - 0x7FFF)].", cmd_set_ntf_filter, 3, 0),
   SHELL_SUBCMD_SET_END);
#endif /* CONFIG_BT_MAP_MCE */

#if (defined(CONFIG_BT_MAP_MSE) && ((CONFIG_BT_MAP_MSE) > 0U))
#define MAP_MSE_CLASS_OF_DEVICE (0x100214U) /* Object Transfer, Phone, Palm-size PC/PDA */

#define MAP_MSE_MAS_TX_NET_BUF_COUNT   (1U)
#define MAP_MSE_MNS_TX_NET_BUF_COUNT   (1U)
#define MAP_MSE_MAS_TX_NET_BUF_SIZE    (1790U + 2U) /* L2CAP I-frame Enhanced Control Field(2-byte) */
#define MAP_MSE_MNS_TX_NET_BUF_SIZE    (CONFIG_BT_MAP_MSE_MNS_MAX_PKT_LEN + 2U) /* L2CAP I-frame Enhanced Control Field(2-byte) */

#define MAP_MSE_DRIVE_NUMBER "0:" /* RAMDISK + '0' */
#define MAP_MSE_REPO_ROOT "0:/root" /* RAMDISK + '0' */
#define MAP_MSE_XML_FOLDER_LISTING_PATH "0:/folder_listing.xml"
#define MAP_MSE_XML_MSG_LISTING_PATH    "0:/msg_listing.xml"
#define MAP_MSE_MAS_INSTANCE_INFO "SMS/MMS"
#define MAP_MSE_TIME "20180101T000000+0000"
#define MAP_MSE_MAX_PATH_LEN (45U)

#define MAP_FLAG_STRING(index) ((char *[]){"", "START", "CONTINUE", "", "END", "UNSEG"})[index]

#define DATE_YEAR_SHIFT   (9U)
#define DATE_YEAR_MASK    (0xFE00U)
#define DATE_MONTH_SHIFT  (5U)
#define DATE_MONTH_MASK   (0x01E0U)
#define DATE_DAY_SHIFT    (0U)
#define DATE_DAY_MASK     (0x001FU)
#define TIME_HOUR_SHIFT   (11U)
#define TIME_HOUR_MASK    (0xF800U)
#define TIME_MINUTE_SHIFT (5U)
#define TIME_MINUTE_MASK  (0x07E0U)
#define TIME_SECOND_SHIFT (0U)
#define TIME_SECOND_MASK  (0x001FU)

static struct bt_sdp_attribute map_mse_instatance_1_attrs[] = {
    BT_SDP_NEW_SERVICE,
    /* ServiceClassIDList */
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_MAP_MSE_SVCLASS) //11 32
        },
        )
    ),

    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 17), //35, 11
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), // 35 , 3
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) //01 00
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),// 35 05
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM), // 00 03
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
                BT_SDP_ARRAY_8(BT_RFCOMM_CHAN_MAP_MSE) //RFCOMM CHANNEL
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),// 35 03
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTP_OBEX) // 00 08
            },
            )
        },
        )
    ),
        /* BluetoothProfileDescriptorList */
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_MAP_SVCLASS) //11 34
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0104U) //01 04
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("MAP MAS-name"),
    /* GoepL2CapPsm */
    BT_SDP_ATTR_GOEP_L2CAP_PSM,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
        BT_SDP_ARRAY_16(BT_BR_PSM_MAP_MSE_1)
    },
    /* MASInstanceID */
    BT_SDP_ATTR_MAS_INSTANCE_ID,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
        BT_SDP_ARRAY_8(0x0)
    },
    /* SupportedMessageTypes */
    BT_SDP_ATTR_SUPPORTED_MESSAGE_TYPES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
        BT_SDP_ARRAY_8(0x1F)
    },
    /*  SupportedFeatures */
    BT_SDP_ATTR_MAP_SUPPORTED_FEATURES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT32),
        BT_SDP_ARRAY_32(CONFIG_BT_MAP_MSE_MNS_SUPPORTED_FEATURES)
    },
};

static struct bt_sdp_attribute map_mse_instatance_2_attrs[] = {
    BT_SDP_NEW_SERVICE,
    /* ServiceClassIDList */
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_MAP_MSE_SVCLASS) //11 32
        },
        )
    ),

    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 17), //35, 11
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), // 35 , 3
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) //01 00
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),// 35 05
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM), // 00 03
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
                BT_SDP_ARRAY_8(BT_RFCOMM_CHAN_MAP_MSE) //RFCOMM CHANNEL
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),// 35 03
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTP_OBEX) // 00 08
            },
            )
        },
        )
    ),
        /* BluetoothProfileDescriptorList */
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_MAP_SVCLASS) //11 34
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0104U) //01 04
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("MAP MAS-name"),
    /* GoepL2CapPsm */
    BT_SDP_ATTR_GOEP_L2CAP_PSM,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
        BT_SDP_ARRAY_16(BT_BR_PSM_MAP_MSE_2)
    },
    /* MASInstanceID */
    BT_SDP_ATTR_MAS_INSTANCE_ID,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
        BT_SDP_ARRAY_8(0x0)
    },
    /* SupportedMessageTypes */
    BT_SDP_ATTR_SUPPORTED_MESSAGE_TYPES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
        BT_SDP_ARRAY_8(0x1F)
    },
    /*  SupportedFeatures */
    BT_SDP_ATTR_MAP_SUPPORTED_FEATURES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT32),
        BT_SDP_ARRAY_32(CONFIG_BT_MAP_MSE_MNS_SUPPORTED_FEATURES)
    },
};

static struct bt_sdp_record map_mse_inst_1_rec = BT_SDP_RECORD(map_mse_instatance_1_attrs);
static struct bt_sdp_record map_mse_inst_2_rec = BT_SDP_RECORD(map_mse_instatance_2_attrs);

#define MAP_MSE_MSG_UTF_8 \
"BEGIN:BMSG\r\n\
VERSION:1.0\r\n\
STATUS:UNREAD\r\n\
TYPE:SMS_GSM\r\n\
FOLDER:\r\n\
BEGIN:VCARD\r\n\
VERSION:2.1\r\n\
N;CHARSET=UTF-8:\r\n\
TEL;CHARSET=UTF-8:\r\n\
END:VCARD\r\n\
BEGIN:BENV\r\n\
BEGIN:VCARD\r\n\
VERSION:2.1\r\n\
FN;CHARSET=UTF-8:+0000000000000\r\n\
N;CHARSET=UTF-8:+0000000000000\r\n\
TEL:+0000000000000\r\n\
END:VCARD\r\n\
BEGIN:BBODY\r\n\
CHARSET:UTF-8\r\n\
LANGUAGE:UNKNOWN\r\n\
LENGTH:492\r\n\
BEGIN:MSG\r\n\
1. Bluetooth MAP Test!\n\
2. Bluetooth MAP Test!\n\
3. Bluetooth MAP Test!\n\
4. Bluetooth MAP Test!\n\
5. Bluetooth MAP Test!\n\
6. Bluetooth MAP Test!\n\
7. Bluetooth MAP Test!\n\
8. Bluetooth MAP Test!\n\
9. Bluetooth MAP Test!\n\
10. Bluetooth MAP Test!\n\
11. Bluetooth MAP Test!\n\
12. Bluetooth MAP Test!\n\
13. Bluetooth MAP Test!\n\
14. Bluetooth MAP Test!\n\
15. Bluetooth MAP Test!\n\
16. Bluetooth MAP Test!\n\
17. Bluetooth MAP Test!\n\
18. Bluetooth MAP Test!\n\
19. Bluetooth MAP Test!\n\
20. Bluetooth MAP Test!\r\n\
END:MSG\r\n\
END:BBODY\r\n\
END:BENV\r\n\
END:BMSG"

#define MAP_MSE_XML_CONVO_LISTING \
"<MAP-convo-listing version = \"1.0\">\r\n\
    <conversation id=\"E1E2E3E4F1F2F3F4A1A2A3A4B1B2B3B4\" name=\"Beergarden Connection\" last_activity=\"20140612T105430+0100\" read_status=\"no\" version_counter=\"A1A1B2B2C3C3D4D5E5E6F6F7A7A8B8B\">\r\n\
        <participant uci=\"4986925814@s.whateverapp.net\" display_name=\"Tien\" chat_state=\"3\" last_activity=\"20140612T105430+0100\"/>\r\n\
        <participant uci=\"4912345678@s.whateverapp.net\" display_name=\"Jonas\" chat_state=\"5\" last_activity=\"20140610T115130+0100\"/>\r\n\
        <participant uci=\"4913579864@s.whateverapp.net\" display_name=\"Max\" chat_state=\"2\" last_activity=\"20140608T225435+0100\"/>\r\n\
        <participant uci=\"4924689753@s.whateverapp.net\" display_name=\"Nils\" chat_state=\"0\" last_activity=\"20140612T092320+0100\"/>\r\n\
        <participant uci=\"4923568910@s.whateverapp.net\" display_name=\"Alex\" chat_state=\"4\" last_activity=\"20140612T104110+0100\"/>\r\n\
    </conversation>\r\n\
    <conversation id=\"C1C2C3C4D1D2D3D4E1E2E3E4F1F2F3F4\" name=\"\" last_activity=\"20140801T012900+0100\" read_status=\"yes\" version_counter=\"0A0A1B1B2C2C3D3D4E4E5F5F6A6A7B7B\">\r\n\
        <participant uci=\"malo@email.de\" display_name=\"Mari\" chat_state=\"2\" last_activity=\"20140801T012900+0100\" x_bt_uid=\" A1A2A3A4B1B2C1C2D1D2E1E2E3E4F1F2\"/>\r\n\
    </conversation>\r\n\
    <conversation id=\"F1F2F3F4E1E2E3E4D1D2D3D4C1C2C3C4\" name=\"family\" last_activity=\"20140925T133700+0100\" read_status=\"yes\" version_counter=\"1A1A2B2B3C3C4D4D5E5E6F6F7A7A8B8B\">\r\n\
        <participant uci=\"malo@email.de\" display_name=\"Mari\" chat_state=\"2\" last_activity=\"20140801T012900+0100\" x_bt_uid=\" A1A2A3A4B1B2C1C2D1D2E1E2E3E4F1F2\" name=\"Mareike Loh\" presence_availability=\"2\" presence_text=\"Wow it's hot today\" priority=\"100\"/>\r\n\
        <participant uci=\"alois.s@august.de\" display_name=\"Lil Al\" chat_state=\"1\" last_activity=\"20140801T012800+0100\" x_bt_uid=\" A1A2A3A4B1B2C1C2D1D2E1E2E3E4F1F2\" name=\"Alois S.\" presence_availability=\"0\" presence_text=\"On my way\" priority=\"100\"/>\r\n\
    </conversation>\r\n\
</MAP-convo-listing>"

#define MAP_MSE_XML_EVENT_REPORT \
"<MAP-event-report version = \"1.0\">\r\n\
    <event type = \"NewMessage\" handle = \"12345678\" folder = \"TELECOM/MSG/INBOX\" msg_type = \"SMS_CDMA\" />\r\n\
</MAP-event-report>"

enum map_cmd_id
{
    CMD_ID_NONE,
    CMD_ID_GET_FOLDER_LISTING,
    CMD_ID_SET_FOLDER,
    CMD_ID_GET_MSG_LISTING,
    CMD_ID_GET_MSG,
    CMD_ID_SET_MSG_STATUS,
    CMD_ID_PUSH_MSG,
    CMD_ID_SET_NTF_REG,
    CMD_ID_UPDATE_INBOX,
    CMD_ID_GET_MAS_INST_INFO,
    CMD_ID_SET_OWNER_STATUS,
    CMD_ID_GET_OWNER_STATUS,
    CMD_ID_GET_CONVO_LISTING,
    CMD_ID_SET_NTF_FILTER,
};

struct app_map_mse_instance
{
    struct bt_conn *acl_conn;
    struct bt_map_mse_mas *mse_mas;
    struct bt_map_mse_mns *mse_mns;
    uint64_t msg_handle;
    uint16_t max_pkt_len; /* range from opcode to the end of packet */
    uint16_t mns_max_pkt_len; /* range from opcode to the end of packet */
    uint16_t tx_cnt;
    uint16_t mns_tx_cnt;
    char path[MAP_MSE_MAX_PATH_LEN];
    enum map_cmd_id cmd_id;
};

struct map_app_param_user_data
{
    enum map_cmd_id id;
    void *data;
};

struct map_xml_msg_listing
{
    uint32_t listing_size;
    uint32_t unread_cnt;
};

struct map_msg_status
{
    uint8_t status_ind;
    uint8_t status_val;
    uint8_t ext_data[30];
};

struct map_owner_status
{
    char convo_id[BT_MAP_CONVO_ID_SIZE];
    uint8_t last_activity[sizeof("YYYYMMDDTHHMMSS+HHMM")];
    uint8_t pres_text[30];
    uint8_t pres_avail;
    uint8_t chat_state;
};

static uint8_t sdp_map_mce_user(struct bt_conn *conn, struct bt_sdp_client_result *result);
static void map_mse_mns_connected(struct bt_map_mse_mns *mse_mns);
static void map_mse_mns_disconnected(struct bt_map_mse_mns *mse_mns, uint8_t result);
static void app_mse_send_event_cb(struct bt_map_mse_mns *mse_mns, uint8_t result);
static void map_mse_mas_connected(struct bt_map_mse_mas *mse_mas, uint16_t psm, uint8_t scn);
static void map_mse_mas_disconnected(struct bt_map_mse_mas *mse_mas, uint8_t result);
static void app_mse_abort_cb(struct bt_map_mse_mas *mse_mas);
static void app_mse_get_folder_listing_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);
static void app_mse_set_folder_cb(struct bt_map_mse_mas *mse_mas, char *name);
static void app_mse_get_msg_listing_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag);
static void app_mse_get_msg_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag);
static void app_mse_set_msg_status_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag);
static void app_mse_push_msg_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag);
static void app_mse_set_ntf_reg_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);
static void app_mse_update_inbox_cb(struct bt_map_mse_mas *mse_mas);
static void app_mse_get_mas_inst_info_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);
static void app_mse_set_owner_status_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);
static void app_mse_get_owner_status_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);
static void app_mse_get_convo_listing_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);
static void app_mse_set_ntf_filter_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag);

struct app_map_mse_instance app_mse_instance;
NET_BUF_POOL_FIXED_DEFINE(mse_mas_tx_pool, MAP_MSE_MAS_TX_NET_BUF_COUNT, BT_L2CAP_BUF_SIZE(MAP_MSE_MAS_TX_NET_BUF_SIZE), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
NET_BUF_POOL_FIXED_DEFINE(mse_mns_tx_pool, MAP_MSE_MNS_TX_NET_BUF_COUNT, BT_L2CAP_BUF_SIZE(MAP_MSE_MNS_TX_NET_BUF_SIZE), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
static FATFS map_fatfs;
static BYTE map_fatfs_work[FF_MAX_SS];
static FIL map_fsrc;
static FIL map_fdes;
static struct map_owner_status map_owner_status;
static struct map_msg_status map_msg_status;

static struct bt_sdp_discover_params discov_map_mce = {
    .uuid = BT_UUID_DECLARE_16(BT_SDP_MAP_MCE_SVCLASS),
    .func = sdp_map_mce_user,
    .pool = &sdp_client_pool,
};

struct bt_map_mse_mas_cb mse_mas_cb = {
    .connected = map_mse_mas_connected,
    .disconnected = map_mse_mas_disconnected,
    .abort = app_mse_abort_cb,
    .get_folder_listing = app_mse_get_folder_listing_cb,
    .set_folder = app_mse_set_folder_cb,
    .get_msg_listing = app_mse_get_msg_listing_cb,
    .get_msg = app_mse_get_msg_cb,
    .set_msg_status = app_mse_set_msg_status_cb,
    .push_msg = app_mse_push_msg_cb,
    .set_ntf_reg = app_mse_set_ntf_reg_cb,
    .update_inbox = app_mse_update_inbox_cb,
    .get_mas_inst_info = app_mse_get_mas_inst_info_cb,
    .set_owner_status = app_mse_set_owner_status_cb,
    .get_owner_status = app_mse_get_owner_status_cb,
    .get_convo_listing = app_mse_get_convo_listing_cb,
    .set_ntf_filter = app_mse_set_ntf_filter_cb,
};

struct bt_map_mse_mns_cb mse_mns_cb = {
    .connected = map_mse_mns_connected,
    .disconnected = map_mse_mns_disconnected,
    .send_event = app_mse_send_event_cb,
};

static uint8_t sdp_map_mce_user(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
    int res;
    uint16_t scn;
    uint16_t psm = 0;
    uint32_t supported_features;
    uint16_t map_version;
    const char *service_name;

    if ((app_mse_instance.acl_conn == conn) && (result) && (result->resp_buf))
    {
        shell_print(ctx_shell, "sdp success callback");
        res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_RFCOMM, &scn);
        if (res < 0)
        {
            shell_print(ctx_shell, "REFCOMM channel number is not found");
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
        shell_print(ctx_shell, "REFCOMM channel number %d", scn);
        res = bt_sdp_get_goep_l2cap_psm(result->resp_buf, &psm);
        if (res < 0)
        {
            shell_print(ctx_shell, "L2CAP PSM is not found");
        }
        else
        {
            shell_print(ctx_shell, "L2CAP PSM  0x%04X", psm);
        }
        res = bt_sdp_get_profile_version(result->resp_buf, BT_SDP_MAP_SVCLASS, &map_version);
        if (res < 0)
        {
            shell_print(ctx_shell, "MAP version is not found");
        }
        else
        {
            shell_print(ctx_shell, "MAP version 0x%04X", map_version);
        }
        res = bt_sdp_get_pbap_map_ctn_features(result->resp_buf, &supported_features);
        if (res < 0)
        {
            shell_print(ctx_shell, "Supported features is not found");
        }
        else
        {
            shell_print(ctx_shell, "MAP supported features 0x%08X", supported_features);
        }
        res = bt_sdp_get_service_name(result->resp_buf, &service_name);
        if (res < 0)
        {
            shell_print(ctx_shell, "Service name is not found");
        }
        else
        {
            shell_print(ctx_shell, "Service name %s", service_name);
        }
        if ((scn != 0U) || (psm != 0U))
        {
            shell_print(ctx_shell, "Message Notification Server found. Connecting ...");

            if (psm != 0)
            {
                res = bt_map_mse_mns_psm_connect(app_mse_instance.acl_conn, psm, supported_features, &app_mse_instance.mse_mns);
            }
            else
            {
                res = bt_map_mse_mns_scn_connect(app_mse_instance.acl_conn, (uint8_t)scn, supported_features, &app_mse_instance.mse_mns);
            }
            if (0 != res)
            {
                shell_print(ctx_shell, "fail to connect MSE (err: %d)", res);
            }
        }
        return BT_SDP_DISCOVER_UUID_STOP;
    }
    else
    {
        shell_print(ctx_shell, "sdp fail callback");
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
}

static int map_dirname(char *name)
{
    char *str;

    if (name == NULL)
    {
        return -EINVAL;
    }

    str = strrchr(name, '/');
    if (str != NULL)
    {
        str[0] = '\0';
    }
    else
    {
        return -EINVAL;
    }

    return 0;
}

static int map_joinpath(char *name1, char *name2)
{
    FRESULT fr;

    if ((name1 == NULL) || (name2 == NULL))
    {
        return -EINVAL;
    }
    if (strlen(name1) + strlen("/") + strlen(name2) >= MAP_MSE_MAX_PATH_LEN)
    {
        return -ENAMETOOLONG;
    }
    strcat(name1, "/");
    strcat(name1, name2);

    fr = f_stat(name1, NULL);
    if (fr == FR_OK)
    {
        return 0;
    }
    else if (fr == FR_NO_FILE)
    {
        map_dirname(name1);
        return -ENOENT;
    }
    else
    {
        map_dirname(name1);
        return -EIO;
    }
}

static int map_fs_init(void)
{
    MKFS_PARM formatOptions;
    UINT actual;
    FATFS *fs;
    DWORD fre_clust;
    DWORD fre_sect;
    DWORD tot_sect;
    WORD sector_size;

    if (f_mount(&map_fatfs, MAP_MSE_DRIVE_NUMBER, 0) != FR_OK)
    {
        return -EBUSY;
    }
    memset(&formatOptions, 0, sizeof(formatOptions));
    formatOptions.fmt = FM_SFD | FM_ANY;
    if (f_mkfs(MAP_MSE_DRIVE_NUMBER, &formatOptions, &map_fatfs_work, FF_MAX_SS) != FR_OK)
    {
        return -EBUSY;
    }
    (void)sprintf(&app_mse_instance.path[0], "%s", MAP_MSE_REPO_ROOT);
    if (f_mkdir((const TCHAR *)&app_mse_instance.path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&app_mse_instance.path[0], "%s/telecom", MAP_MSE_REPO_ROOT);
    if (f_mkdir((const TCHAR *)&app_mse_instance.path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&app_mse_instance.path[0], "%s/telecom/msg", MAP_MSE_REPO_ROOT);
    if (f_mkdir((const TCHAR *)&app_mse_instance.path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&app_mse_instance.path[0], "%s/telecom/msg/inbox", MAP_MSE_REPO_ROOT);
    if (f_mkdir((const TCHAR *)&app_mse_instance.path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&app_mse_instance.path[0], "%s/telecom/msg/outbox", MAP_MSE_REPO_ROOT);
    if (f_mkdir((const TCHAR *)&app_mse_instance.path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&app_mse_instance.path[0], "%s/telecom/msg/sent", MAP_MSE_REPO_ROOT);
    if (f_mkdir((const TCHAR *)&app_mse_instance.path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&app_mse_instance.path[0], "%s/telecom/msg/deleted", MAP_MSE_REPO_ROOT);
    if (f_mkdir((const TCHAR *)&app_mse_instance.path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&app_mse_instance.path[0], "%s/telecom/msg/draft", MAP_MSE_REPO_ROOT);
    if (f_mkdir((const TCHAR *)&app_mse_instance.path[0]) != FR_OK)
    {
        return -EIO;
    }

    /* write one message into inbox */
    /* Not use %016llX here to avoid uint64_t is not supported in some platforms. */
    (void)sprintf(&app_mse_instance.path[0], "%s/telecom/msg/inbox/%08X%08X", MAP_MSE_REPO_ROOT,
    (unsigned int)(app_mse_instance.msg_handle >> 32U), (unsigned int)app_mse_instance.msg_handle);
    app_mse_instance.msg_handle++;
    if (f_open(&map_fdes, &app_mse_instance.path[0], FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
    {
        return -EIO;
    }
    if ((f_write(&map_fdes, MAP_MSE_MSG_UTF_8, strlen(MAP_MSE_MSG_UTF_8), &actual) != FR_OK) || (actual != strlen(MAP_MSE_MSG_UTF_8)))
    {
        return -EIO;
    }
    f_close(&map_fdes);

    if ((f_getfree(MAP_MSE_DRIVE_NUMBER, &fre_clust, &fs) != FR_OK) || (disk_ioctl(RAMDISK, GET_SECTOR_SIZE, &sector_size) != RES_OK))
    {
        return -EIO;
    }
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;
    shell_print(ctx_shell, "File system mounted");
    shell_print(ctx_shell, "Total drive space - %lluB", (uint64_t)tot_sect * (uint64_t)sector_size);
    shell_print(ctx_shell, "Free drive space - %lluB", (uint64_t)fre_sect * (uint64_t)sector_size);

    (void)sprintf(&app_mse_instance.path[0], "%s", MAP_MSE_REPO_ROOT);

    return 0;
}

static void map_mse_mns_connected(struct bt_map_mse_mns *mse_mns)
{
    app_mse_instance.mse_mns = mse_mns;
    app_mse_instance.mns_tx_cnt = 0;
    shell_print(ctx_shell, "MSE MNS connection");
    if (bt_map_mse_mns_get_max_pkt_len(mse_mns, &app_mse_instance.mns_max_pkt_len) == 0)
    {
        shell_print(ctx_shell, "MAX Packet Length - %d", app_mse_instance.mns_max_pkt_len);
    }
    else
    {
        shell_print(ctx_shell, "MAX Packet Length is invalid");
    }
}

static void map_mse_mns_disconnected(struct bt_map_mse_mns *mse_mns, uint8_t result)
{
    app_mse_instance.mse_mns = NULL;
    shell_print(ctx_shell, "MSE MNS disconnection - 0x%02X", result);
}

static void map_mse_mas_connected(struct bt_map_mse_mas *mse_mas, uint16_t psm, uint8_t scn)
{
    app_mse_instance.msg_handle = 0;
    app_mse_instance.tx_cnt = 0;
    app_mse_instance.mse_mas = mse_mas;
    if (map_fs_init() != 0)
    {
        shell_print(ctx_shell, "File system init failed");
    }
    shell_print(ctx_shell, "MSE MAS connection");
    if (bt_map_mse_get_max_pkt_len(mse_mas, &app_mse_instance.max_pkt_len) == 0)
    {
        shell_print(ctx_shell, "MAX Packet Length - %d", app_mse_instance.max_pkt_len);
    }
    else
    {
        shell_print(ctx_shell, "MAX Packet Length is invalid");
    }
}

static void map_mse_mas_disconnected(struct bt_map_mse_mas *mse_mas, uint8_t result)
{
    app_mse_instance.mse_mas = NULL;
    shell_print(ctx_shell, "MSE MAS disconnection - 0x%02X", result);
}

static void app_mse_abort_cb(struct bt_map_mse_mas *mse_mas)
{
    shell_print(ctx_shell, "MCE Abort IND");
    switch (app_mse_instance.cmd_id)
    {
        case CMD_ID_GET_FOLDER_LISTING:
        case CMD_ID_GET_MSG_LISTING:
        case CMD_ID_GET_MAS_INST_INFO:
        case CMD_ID_GET_CONVO_LISTING:
            app_mse_instance.tx_cnt = 0;
            break;
        case CMD_ID_GET_MSG:
            app_mse_instance.tx_cnt = 0;
            map_dirname(&app_mse_instance.path[0]);
            break;
        case CMD_ID_PUSH_MSG:
            f_close(&map_fdes);
            break;
        case CMD_ID_SET_NTF_FILTER:
        case CMD_ID_SET_OWNER_STATUS:
        case CMD_ID_GET_OWNER_STATUS:
        case CMD_ID_SET_NTF_REG:
        case CMD_ID_UPDATE_INBOX:
        case CMD_ID_SET_MSG_STATUS:
        case CMD_ID_SET_FOLDER:
        default:
            break;
    }

    app_mse_instance.cmd_id = CMD_ID_NONE;
}

static void app_mse_send_event_cb(struct bt_map_mse_mns *mse_mns, uint8_t result)
{
    struct net_buf *buf;
    uint16_t max_pkt_len;
    uint16_t actual;
    char event_report[] = MAP_MSE_XML_EVENT_REPORT;
    enum bt_obex_req_flags flag;

    shell_print(ctx_shell, "MAP Recv Send Event CNF - 0x%02X", result);
    if (result == BT_MAP_RSP_CONTINUE)
    {
        buf = net_buf_alloc(&mse_mns_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_EVENT(mse_mns, BT_OBEX_REQ_END));
        if (app_mse_instance.mns_tx_cnt < strlen(event_report))
        {
            max_pkt_len = app_mse_instance.mns_max_pkt_len;
            max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_EVENT(mse_mns, BT_OBEX_REQ_END) - BT_L2CAP_BUF_SIZE(2U);
            max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
            actual = strlen(event_report) - app_mse_instance.mns_tx_cnt;
            if (actual > max_pkt_len)
            {
                BT_MAP_ADD_BODY(buf, (uint8_t *)&event_report[app_mse_instance.mns_tx_cnt], max_pkt_len);
                app_mse_instance.mns_tx_cnt += max_pkt_len;
                flag = BT_OBEX_REQ_CONTINUE;
            }
            else
            {
                BT_MAP_ADD_END_OF_BODY(buf, (uint8_t *)&event_report[app_mse_instance.mns_tx_cnt], actual);
                app_mse_instance.mns_tx_cnt = 0;
                flag = BT_OBEX_REQ_END;
            }
        }
        else
        {
            app_mse_instance.mns_tx_cnt = 0;
            flag = BT_OBEX_REQ_END;
        }
        if (bt_map_mse_send_event(app_mse_instance.mse_mns, buf, flag) != 0)
        {
            net_buf_unref(buf);
            shell_print(ctx_shell, "Failed to send event report");
        }
    }
    else
    {
        app_mse_instance.mns_tx_cnt = 0;
    }
}

static bool app_mse_app_param_cb(struct bt_data *data, void *user_data)
{
    struct map_app_param_user_data *app_param = (struct map_app_param_user_data *)user_data;

    switch (data->type)
    {
        case BT_MAP_TAG_ID_MAX_LIST_COUNT:
            if (data->data_len < 2U)
            {
                return false;
            }
            shell_print(ctx_shell, "Max List Count - %d", sys_get_be16(data->data));
            break;

        case BT_MAP_TAG_ID_LIST_START_OFFSET:
            if (data->data_len < 2U)
            {
                return false;
            }
            shell_print(ctx_shell, "List Start Offset - %d", sys_get_be16(data->data));
            break;

        case BT_MAP_TAG_ID_FILTER_MESSAGE_TYPE:
            shell_print(ctx_shell, "Filter Message Type - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_FILTER_PERIOD_BEGIN:
            shell_print(ctx_shell, "Filter Period Begin - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_FILTER_PERIOD_END:
            shell_print(ctx_shell, "Filter Period End - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_FILTER_READ_STATUS:
            shell_print(ctx_shell, "Filter Read Status - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_FILTER_RECIPIENT:
            shell_print(ctx_shell, "Filter Recipient - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_FILTER_ORIGINATOR:
            shell_print(ctx_shell, "Filter Originator - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_FILTER_PRIORITY:
            shell_print(ctx_shell, "Filter Priority - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_ATTACHMENT:
            shell_print(ctx_shell, "Attachment - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_TRANSPARENT:
            shell_print(ctx_shell, "Transparent - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_RETRY:
            shell_print(ctx_shell, "Retry - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_NOTIFICATION_STATUS:
            shell_print(ctx_shell, "Notification Status - %d", data->data[0]);
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_NTF_REG))
            {
                *(uint8_t *)app_param->data = data->data[0];
            }
            break;

        case BT_MAP_TAG_ID_MAS_INSTANCE_ID:
            shell_print(ctx_shell, "MAS Instance ID - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_PARAMETER_MASK:
            if (data->data_len < 4U)
            {
                return false;
            }
            shell_print(ctx_shell, "Parameter Mask - %08X", sys_get_be32(data->data));
            break;

        case BT_MAP_TAG_ID_SUBJECT_LENGTH:
            shell_print(ctx_shell, "Subject Length - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_CHARSET:
            shell_print(ctx_shell, "Charset - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_FRACTION_REQUEST:
            shell_print(ctx_shell, "Fraction Request - %d", data->data[0]);
            break;

        case BT_MAP_TAG_ID_STATUS_INDICATOR:
            shell_print(ctx_shell, "Status Indicator - %d", data->data[0]);
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_MSG_STATUS))
            {
                ((struct map_msg_status *)app_param->data)->status_ind = data->data[0];
            }
            break;

        case BT_MAP_TAG_ID_STATUS_VALUE:
            shell_print(ctx_shell, "Status Value - %d", data->data[0]);
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_MSG_STATUS))
            {
                ((struct map_msg_status *)app_param->data)->status_val = data->data[0];
            }
            break;

        case BT_MAP_TAG_ID_PRESENCE_AVAILABILITY:
            shell_print(ctx_shell, "Presence  - %d", data->data[0]);
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_OWNER_STATUS))
            {
                ((struct map_owner_status *)app_param->data)->pres_avail = data->data[0];
            }
            break;

        case BT_MAP_TAG_ID_PRESENCE_TEXT:
            shell_print(ctx_shell, "============== Presence Text ==============");
            shell_print(ctx_shell, "%.*s", data->data_len, data->data);
            shell_print(ctx_shell, "============ END Presence Text ============");
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_OWNER_STATUS))
            {
                uint8_t length = data->data_len > (sizeof(((struct map_owner_status *)app_param->data)->pres_text) - 1U) ?
                                 (sizeof(((struct map_owner_status *)app_param->data)->pres_text) - 1U) : data->data_len;
                memcpy(((struct map_owner_status *)app_param->data)->pres_text, data->data, length);
            }
            break;

        case BT_MAP_TAG_ID_LAST_ACTIVITY:
            shell_print(ctx_shell, "Last Activity - %.*s", data->data_len, data->data);
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_OWNER_STATUS))
            {
                uint8_t length = data->data_len > (sizeof(((struct map_owner_status *)app_param->data)->last_activity) - 1U) ?
                                 (sizeof(((struct map_owner_status *)app_param->data)->last_activity) - 1U) : data->data_len;
                memcpy(((struct map_owner_status *)app_param->data)->last_activity, data->data, length);
            }
            break;

        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_BEGIN:
            shell_print(ctx_shell, "Last Activity Begin - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_END:
            shell_print(ctx_shell, "Last Activity End - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_CHAT_STATE:
            shell_print(ctx_shell, "Chat State - %d", data->data[0]);
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_OWNER_STATUS))
            {
                ((struct map_owner_status *)app_param->data)->chat_state = data->data[0];
            }
            break;

        case BT_MAP_TAG_ID_CONVERSATION_ID:
            shell_print(ctx_shell, "Conversation ID - %.*s", data->data_len, data->data);
            if (app_param != NULL)
            {
                if (app_param->id == CMD_ID_SET_OWNER_STATUS)
                {
                    uint8_t length = data->data_len > (BT_MAP_CONVO_ID_SIZE - 1U) ?
                                     (BT_MAP_CONVO_ID_SIZE - 1U) : data->data_len;
                    memcpy(((struct map_owner_status *)app_param->data)->convo_id, data->data, length);
                }
                else if (app_param->id == CMD_ID_GET_OWNER_STATUS)
                {
                    uint8_t length = data->data_len > (BT_MAP_CONVO_ID_SIZE - 1U) ?
                                     (BT_MAP_CONVO_ID_SIZE - 1U) : data->data_len;
                    memcpy(app_param->data, data->data, length);
                }
                else
                {
                    /* no action */
                }
            }
            break;

        case BT_MAP_TAG_ID_FILTER_MSG_HANDLE:
            shell_print(ctx_shell, "Filter Message Handle - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_NOTIFICATION_FILTER_MASK:
            if (data->data_len < 4U)
            {
                return false;
            }
            shell_print(ctx_shell, "Notification Filter Mask - %08X", sys_get_be32(data->data));
            break;

        case BT_MAP_TAG_ID_CONV_PARAMETER_MASK:
            if (data->data_len < 4U)
            {
                return false;
            }
            shell_print(ctx_shell, "Conversation Parameter Mask - %08X", sys_get_be32(data->data));
            break;

        case BT_MAP_TAG_ID_EXTENDED_DATA:
            shell_print(ctx_shell, "============== Extended Data ==============");
            shell_print(ctx_shell, "Extended Data - %.*s", data->data_len, data->data);
            shell_print(ctx_shell, "============ END Extended Data ============");
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_MSG_STATUS))
            {
                uint8_t length = data->data_len > (sizeof(((struct map_msg_status *)app_param->data)->ext_data) - 1U) ?
                                 (sizeof(((struct map_msg_status *)app_param->data)->ext_data) - 1U) : data->data_len;
                memcpy(((struct map_msg_status *)app_param->data)->ext_data, data->data, length);
            }
            break;

        case BT_MAP_TAG_ID_MAP_SUPPORTED_FEATURES:
            if (data->data_len < 4U)
            {
                return false;
            }
            shell_print(ctx_shell, "Supported Features - %08X", sys_get_be32(data->data));
            break;

        case BT_MAP_TAG_ID_MESSAGE_HANDLE:
            shell_print(ctx_shell, "Message Handle - %.*s", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_MODIFY_TEXT:
            shell_print(ctx_shell, "Modify Text - %d", data->data[0]);
            break;

        default:
            break;
    }

    return true;
}

static int map_create_xml_folder_listing(char *xml_name, char *dir_name)
{
    int err = 0;
    DIR dir;
    FILINFO fno;
    UINT actual;
    char head[] = "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\r\n<folder-listing version=\"1.0\">\r\n";
    char tail[] = "</folder-listing>";

    if (f_open(&map_fdes, xml_name, FA_CREATE_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
    {
        return -EIO;
    }
    (void)f_chmod(xml_name, AM_SYS | AM_HID, AM_SYS | AM_HID);
    if ((f_write(&map_fdes, head, strlen(head), &actual) != FR_OK) || (actual < strlen(head)))
    {
        f_close(&map_fdes);
        return -EIO;
    }
    if (f_opendir(&dir, dir_name) != FR_OK)
    {
        f_close(&map_fdes);
        return -EIO;
    }
    for (;;)
    {
        if (f_readdir(&dir, &fno) != FR_OK || fno.fname[0] == 0)
        {
            break;
        }
        if (fno.fattrib & AM_DIR)
        {
            (void)f_printf(&map_fdes, "    <folder name = \"%s\"/>\r\n", fno.fname);
        }
    }
    if ((f_write(&map_fdes, tail, strlen(tail), &actual) != FR_OK) || (actual < strlen(tail)))
    {
        err = -EIO;
    }
    f_closedir(&dir);
    f_close(&map_fdes);

    return err;
}

static int map_create_xml_msg_listing(char *xml_name, char *dir_name, struct map_xml_msg_listing *out)
{
    int err = 0;
    DIR dir;
    FILINFO fno;
    UINT actual;
    char path[MAP_MSE_MAX_PATH_LEN];
    char head[] = "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\r\n<MAP-msg-listing version=\"1.0\">\r\n";
    char tail[] = "</MAP-msg-listing>";
    uint8_t rd_buf[64];
    uint8_t vcard_bgn;
    uint8_t benv_bgn;
    uint8_t msg_bgn;
    struct attr
    {
        uint8_t subject[30];
        uint8_t datetime[sizeof("YYYYMMDDTHHMMSS+HHMM")];
        uint8_t sd_name[20];
        uint8_t sd_addr[20];
        uint8_t rcp_name[20];
        uint8_t rcp_addr[20];
        uint8_t type[9]; /* EMAIL, SMS_GSM, SMS_CDMA, MMS, IM */
        uint8_t size[11]; /* 32 bit decimal string */
        uint8_t read[4]; /* "yes"/"no" */
    } attr;

    memset(out, 0, sizeof(struct map_xml_msg_listing));
    if (f_open(&map_fdes, xml_name, FA_CREATE_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
    {
        return -EIO;
    }
    (void)f_chmod(xml_name, AM_SYS | AM_HID, AM_SYS | AM_HID);
    if ((f_write(&map_fdes, head, strlen(head), &actual) != FR_OK) || (actual < strlen(head)))
    {
        f_close(&map_fdes);
        return -EIO;
    }
    if (f_opendir(&dir, dir_name) != FR_OK)
    {
        f_close(&map_fdes);
        return -EIO;
    }
    for (;;)
    {
        if ((f_readdir(&dir, &fno) != FR_OK) || (fno.fname[0] == 0))
        {
            break;
        }
        if ((fno.fattrib & AM_DIR) == 0U)
        {
            out->listing_size++;
            (void)snprintf(path, MAP_MSE_MAX_PATH_LEN, "%s/%s", dir_name, fno.fname);
            vcard_bgn = 0;
            benv_bgn = 0;
            msg_bgn = 0;
            memset(&attr, 0U, sizeof(attr));
            f_open(&map_fsrc, path, FA_READ);
            for (;;)
            {
                if (f_gets((char *)&rd_buf[0], sizeof(rd_buf), &map_fsrc) == NULL)
                {
                    break;
                }
                for (uint8_t index = 0; index < sizeof(rd_buf); index++)
                {
                    if ((rd_buf[index] == '\r') || (rd_buf[index] == '\n'))
                    {
                        rd_buf[index] = '\0';
                        break;
                    }
                }
                if (strstr((char *)&rd_buf[0], "STATUS:"))
                {
                    if (strstr((char *)&rd_buf[sizeof("STATUS:") - 1U], "UNREAD"))
                    {
                        out->unread_cnt++;
                        snprintf((char *)&attr.read[0], sizeof(attr.read), "%s", "no");
                    }
                    else
                    {
                        snprintf((char *)&attr.read[0], sizeof(attr.read), "%s", "yes");
                    }
                    continue;
                }
                if (strstr((char *)&rd_buf[0], "TYPE:"))
                {
                    snprintf((char *)&attr.type[0], sizeof(attr.type), "%s", &rd_buf[sizeof("TYPE:") - 1U]);
                    continue;
                }
                if (strstr((char *)&rd_buf[0], "BEGIN:VCARD"))
                {
                    vcard_bgn++;
                    continue;
                }
                if (strstr((char *)&rd_buf[0], "BEGIN:BENV"))
                {
                    benv_bgn++;
                    continue;
                }
                if (vcard_bgn > 0)
                {
                    if ((strstr((char *)&rd_buf[0], "VERSION:")) || (strstr((char *)&rd_buf[0], "FN:")))
                    {
                        continue;
                    }
                    else if (strstr((char *)&rd_buf[0], "N:"))
                    {
                        if (benv_bgn == 0)
                        {
                            snprintf((char *)&attr.sd_name[0], sizeof(attr.sd_name), "%s", &rd_buf[sizeof("N:") - 1U]);
                        }
                        else
                        {
                            snprintf((char *)&attr.rcp_name[0], sizeof(attr.rcp_name), "%s", &rd_buf[sizeof("N:") - 1U]);
                        }
                        continue;
                    }
                    else if (strstr((char *)&rd_buf[0], "TEL:"))
                    {
                        if (benv_bgn == 0)
                        {
                            snprintf((char *)&attr.sd_addr[0], sizeof(attr.sd_addr), "%s", &rd_buf[sizeof("TEL:") - 1U]);
                        }
                        else
                        {
                            snprintf((char *)&attr.rcp_addr[0], sizeof(attr.rcp_addr), "%s", &rd_buf[sizeof("TEL:") - 1U]);
                        }
                        continue;
                    }
                    else if (strstr((char *)&rd_buf[0], "EMAIL:"))
                    {
                        if (benv_bgn == 0)
                        {
                            snprintf((char *)&attr.sd_addr[0], sizeof(attr.sd_addr), "%s", &rd_buf[sizeof("EMAIL:") - 1U]);
                        }
                        else
                        {
                            snprintf((char *)&attr.rcp_addr[0], sizeof(attr.rcp_addr), "%s", &rd_buf[sizeof("EMAIL:") - 1U]);
                        }
                        continue;
                    }
                    else
                    {
                        /* no action */
                    }
                }
                if (strstr((char *)&rd_buf[0], "BEGIN:BENV"))
                {
                    benv_bgn--;
                    continue;
                }
                if (strstr((char *)&rd_buf[0], "END:VCARD"))
                {
                    vcard_bgn--;
                    continue;
                }
                if (strstr((char *)&rd_buf[0], "LENGTH:"))
                {
                    snprintf((char *)&attr.size[0], sizeof(attr.size), "%s", &rd_buf[sizeof("LENGTH:") - 1U]);
                    continue;
                }
                if (strstr((char *)&rd_buf[0], "BEGIN:MSG"))
                {
                    msg_bgn++;
                    continue;
                }
                if (msg_bgn > 0)
                {
                    snprintf((char *)&attr.subject[0], sizeof(attr.subject), "%s", &rd_buf[0]);
                    break;
                }
            }
            f_close(&map_fsrc);

            (void)sprintf((char *)&attr.datetime[0], "%.4d%.2d%.2dT%.2d%.2d%.2d+0000",
                ((fno.fdate & DATE_YEAR_MASK) >> DATE_YEAR_SHIFT) + 1980U, /* Year origin from 1980 */
                (fno.fdate & DATE_MONTH_MASK) >> DATE_MONTH_SHIFT,
                (fno.fdate & DATE_DAY_MASK) >> DATE_DAY_SHIFT,
                (fno.ftime & TIME_HOUR_MASK) >> TIME_HOUR_SHIFT,
                (fno.ftime & TIME_MINUTE_MASK) >> TIME_MINUTE_SHIFT,
                ((fno.ftime & TIME_SECOND_MASK) >> TIME_SECOND_SHIFT) << 1U); /* Second / 2 (0...29) */

            (void)f_printf(&map_fdes, "    <msg handle = \"%s\" ", fno.fname);
            (void)f_printf(&map_fdes, "subject = \"%s\" ", &attr.subject[0]);
            (void)f_printf(&map_fdes, "datetime = \"%s\" ", &attr.datetime[0]);
            (void)f_printf(&map_fdes, "sender_name = \"%s\" ", &attr.sd_name[0]);
            (void)f_printf(&map_fdes, "sender_addressing = \"%s\" ", &attr.sd_addr[0]);
            (void)f_printf(&map_fdes, "recipient_name = \"%s\" ", &attr.rcp_name[0]);
            (void)f_printf(&map_fdes, "recipient_addressing = \"%s\" ", &attr.rcp_addr[0]);
            (void)f_printf(&map_fdes, "type = \"%s\" ", &attr.type[0]);
            (void)f_printf(&map_fdes, "size = \"%s\" ", &attr.size[0]);
            (void)f_printf(&map_fdes, "text = \"%s\" ", "yes");
            (void)f_printf(&map_fdes, "recipient_status = \"%s\" ", "complete");
            (void)f_printf(&map_fdes, "attachment_size = \"%s\" ", "0");
            (void)f_printf(&map_fdes, "priority = \"%s\" ", "no");
            (void)f_printf(&map_fdes, "read = \"%s\" ", &attr.read[0]);
            (void)f_printf(&map_fdes, "sent = \"%s\" ", "no");
            (void)f_printf(&map_fdes, "protected = \"%s\"/>\r\n", "no");
        }
    }
    if ((f_write(&map_fdes, tail, strlen(tail), &actual) != FR_OK) || (actual < strlen(tail)))
    {
        err = -EIO;
    }
    f_close(&map_fdes);
    f_closedir(&dir);

    return err;
}

static int map_handle_set_msg_status(char *file_name, struct map_msg_status *msg_status)
{
    char path[MAP_MSE_MAX_PATH_LEN];
    char *msg_handle;
    char *found;
    uint8_t rd_buf[256];

    strcpy(&path[0], file_name);
    if (msg_status->status_ind == 1U) /* deleted */
    {
        map_dirname(&path[0]);
        msg_handle = &file_name[strlen(&path[0]) + 1U];
        map_dirname(&path[0]);
        if (msg_status->status_val == 1)
        {
            map_joinpath(&path[0], "deleted");
            sprintf(&path[strlen(&path[0])], "/%s", msg_handle);
            if (f_rename(file_name, &path[0]) != FR_OK)
            {
                return -EIO;
            }
        }
        else if (msg_status->status_val == 0)
        {
            map_joinpath(&path[0], "inbox");
            sprintf(&path[strlen(&path[0])], "/%s", msg_handle);
            if (f_rename(file_name, &path[0]) != FR_OK)
            {
                return -EIO;
            }
        }
        else
        {
            /* no action */
        }
    }
    else if (msg_status->status_ind == 0U)
    {
        map_dirname(&path[0]);
        sprintf(&path[strlen(&path[0])], "/%s", "internal.msg");
        if (f_open(&map_fsrc, file_name, FA_READ) != FR_OK)
        {
            return -EIO;
        }
        if (f_open(&map_fdes, &path[0], FA_CREATE_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
        {
            f_close(&map_fsrc);
            return -EIO;
        }
        for(;;)
        {
            if (f_gets((char *)&rd_buf[0], sizeof(rd_buf), &map_fsrc) == NULL)
            {
                break;
            }
            if ((strstr((char *)&rd_buf[0], "STATUS:") != NULL) &&
                (strstr((char *)&rd_buf[sizeof("STATUS:") - 1U], "READ") != NULL))
            {
                found = strstr((char *)&rd_buf[0], "STATUS:");
                found[0] = '\0';
                (void)f_printf(&map_fdes, "%s", &rd_buf[0]);
                found = &found[sizeof("STATUS:") - 1U];
                found = strstr(found, "READ");
                (void)f_printf(&map_fdes, "%s", "STATUS:");
                if (msg_status->status_val == 0)
                {
                    (void)f_printf(&map_fdes, "%s", "UNREAD");
                }
                else
                {
                    (void)f_printf(&map_fdes, "%s", "READ");
                }
                (void)f_printf(&map_fdes, "%s", &found[sizeof("READ") - 1U]);
                continue;
            }
            if (f_puts((char *)&rd_buf[0], &map_fdes) != strlen((char *)&rd_buf[0]))
            {
                f_close(&map_fsrc);
                f_close(&map_fdes);
                f_unlink(&path[0]);
                return -EIO;
            }
        }
        f_close(&map_fsrc);
        f_close(&map_fdes);
        f_unlink(file_name);
        f_rename(&path[0], file_name);
    }
    else
    {
        /* handle extended data */
    }

    return 0;
}

static void app_mse_get_folder_listing_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    UINT actual;
    uint8_t *rd_buf;
    uint16_t max_pkt_len;
    uint8_t result;

    shell_print(ctx_shell, "MAP Get Folder Listing IND - %s", MAP_FLAG_STRING(flag));
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);
    net_buf_unref(buf);

    buf = net_buf_alloc(&mse_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas));
    if (flag == BT_OBEX_REQ_UNSEG)
    {
        result = BT_MAP_RSP_SUCCESS;
        if (app_mse_instance.tx_cnt == 0)
        {
            if (map_create_xml_folder_listing(MAP_MSE_XML_FOLDER_LISTING_PATH, &app_mse_instance.path[0]) != 0)
            {
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
        }

        if (result == BT_MAP_RSP_SUCCESS)
        {
            max_pkt_len = app_mse_instance.max_pkt_len;
            max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas) - BT_L2CAP_BUF_SIZE(2U);
            max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
            rd_buf = buf->data + sizeof(struct bt_obex_hdr_bytes); /* body header */

            if ((f_open(&map_fdes, MAP_MSE_XML_FOLDER_LISTING_PATH, FA_READ) != FR_OK) ||
                (f_lseek(&map_fdes, app_mse_instance.tx_cnt) != FR_OK) ||
                (f_read(&map_fdes, rd_buf, max_pkt_len, &actual) != FR_OK))
            {
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
            else
            {
                app_mse_instance.tx_cnt += actual;
                if (app_mse_instance.tx_cnt < f_size(&map_fdes))
                {
                    app_mse_instance.cmd_id = CMD_ID_GET_FOLDER_LISTING;
                    BT_MAP_ADD_BODY(buf, rd_buf, actual);
                    result = BT_MAP_RSP_CONTINUE;
                }
                else
                {
                    app_mse_instance.cmd_id = CMD_ID_NONE;
                    app_mse_instance.tx_cnt = 0;
                    BT_MAP_ADD_END_OF_BODY(buf, rd_buf, actual);
                    result = BT_MAP_RSP_SUCCESS;
                }
            }
            f_close(&map_fdes);
        }
    }
    else
    {
        result = BT_MAP_RSP_NOT_IMPLEMENTED;
    }
    if (bt_map_mse_get_folder_listing_response(mse_mas, result, buf, false) != 0)
    {
        net_buf_unref(buf);
        shell_print(ctx_shell, "Failed to send response");
    }
}

static void app_mse_set_folder_cb(struct bt_map_mse_mas *mse_mas, char *name)
{
    uint8_t result;

    shell_print(ctx_shell, "MAP Set Folder IND");
    shell_print(ctx_shell, "Name - %s", (name != NULL) ? name : "NULL");

    if (name != NULL)
    {
        result = BT_MAP_RSP_SUCCESS;
        if (strcmp(name, "/") == 0)
        {
            strcpy(&app_mse_instance.path[0], MAP_MSE_REPO_ROOT);
        }
        else if (strstr(name, "../") != NULL)
        {
            if (map_dirname(&app_mse_instance.path[0]) == 0)
            {
                if (name[3] != '\0')
                {
                    if ((strchr(&name[3], '/') != NULL) || (map_joinpath(&app_mse_instance.path[0], &name[3]) != 0))
                    {
                        result = BT_MAP_RSP_PRECOND_FAILED;
                    }
                }
            }
            else
            {
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
        }
        else
        {
            if (name[0] != '\0')
            {
                if (map_joinpath(&app_mse_instance.path[0], name) != 0)
                {
                    result = BT_MAP_RSP_PRECOND_FAILED;
                }
            }
        }
    }
    else
    {
        result = BT_MAP_RSP_BAD_REQ;
    }
    if (bt_map_mse_set_folder_response(mse_mas, result) != 0)
    {
        shell_print(ctx_shell, "Failed to send response");
    }
}

static void app_mse_get_msg_listing_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag)
{
    UINT actual;
    uint8_t *rd_buf;
    uint16_t max_pkt_len;
    struct map_xml_msg_listing listing;
    uint8_t result;

    shell_print(ctx_shell, "MAP Get MSG Listing IND - %s", MAP_FLAG_STRING(flag));
    shell_print(ctx_shell, "Name - %s", (name != NULL) ? name : "NULL");
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);
    net_buf_unref(buf);

    buf = net_buf_alloc(&mse_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas));
    if (flag == BT_OBEX_REQ_UNSEG)
    {
        result = BT_MAP_RSP_SUCCESS;
        if (app_mse_instance.tx_cnt == 0)
        {
            if ((name == NULL) || (name[0] == '\0'))
            {
                if (map_create_xml_msg_listing(MAP_MSE_XML_MSG_LISTING_PATH, &app_mse_instance.path[0], &listing) != 0)
                {
                    result = BT_MAP_RSP_INT_SERVER_ERR;
                }
            }
            else if (strchr(name, '/') != NULL)
            {
                result = BT_MAP_RSP_PRECOND_FAILED;
            }
            else if (map_joinpath(&app_mse_instance.path[0], name) == 0)
            {
                if (map_create_xml_msg_listing(MAP_MSE_XML_MSG_LISTING_PATH, &app_mse_instance.path[0], &listing) != 0)
                {
                    result = BT_MAP_RSP_INT_SERVER_ERR;
                }
                map_dirname(&app_mse_instance.path[0]);
            }
            else
            {
                result = BT_MAP_RSP_PRECOND_FAILED;
            }
        }

        if (result == BT_MAP_RSP_SUCCESS)
        {
            max_pkt_len = app_mse_instance.max_pkt_len;
            max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas) - BT_L2CAP_BUF_SIZE(2U);
            if (app_mse_instance.tx_cnt == 0)
            {
                BT_MAP_ADD_NEW_MESSAGE(buf, (listing.unread_cnt > 0) ? 1 : 0);
                BT_MAP_ADD_MSE_TIME(buf, (uint8_t *)MAP_MSE_TIME, sizeof(MAP_MSE_TIME));
                BT_MAP_ADD_LISTING_SIZE(buf, listing.listing_size);
                max_pkt_len -= buf->len; /* application parameters */
            }
            max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
            rd_buf = buf->data + buf->len + sizeof(struct bt_obex_hdr_bytes); /* body header */

            if ((f_open(&map_fdes, MAP_MSE_XML_MSG_LISTING_PATH, FA_READ) != FR_OK) ||
                (f_lseek(&map_fdes, app_mse_instance.tx_cnt) != FR_OK) ||
                (f_read(&map_fdes, rd_buf, max_pkt_len, &actual) != FR_OK))
            {
                app_mse_instance.tx_cnt = 0;
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
            else
            {
                app_mse_instance.tx_cnt += actual;
                if (app_mse_instance.tx_cnt < f_size(&map_fdes))
                {
                    app_mse_instance.cmd_id = CMD_ID_GET_MSG_LISTING;
                    BT_MAP_ADD_BODY(buf, rd_buf, actual);
                    result = BT_MAP_RSP_CONTINUE;
                }
                else
                {
                    app_mse_instance.cmd_id = CMD_ID_NONE;
                    app_mse_instance.tx_cnt = 0;
                    BT_MAP_ADD_END_OF_BODY(buf, rd_buf, actual);
                    result = BT_MAP_RSP_SUCCESS;
                }
            }
            f_close(&map_fdes);
        }
    }
    else
    {
        result = BT_MAP_RSP_NOT_IMPLEMENTED;
    }
    if (bt_map_mse_get_msg_listing_response(mse_mas, result, buf, false) != 0)
    {
        net_buf_unref(buf);
        shell_print(ctx_shell, "Failed to send response");
    }
}

static void app_mse_get_msg_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag)
{
    UINT actual;
    uint8_t *rd_buf;
    uint16_t max_pkt_len;
    uint8_t result;

    shell_print(ctx_shell, "MAP Get MSG IND -%s", MAP_FLAG_STRING(flag));
    PRINTF ("Name - %s\r\n", (name != NULL) ? name : "NULL");
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);
    net_buf_unref(buf);

    buf = net_buf_alloc(&mse_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas));
    if (flag == BT_OBEX_REQ_UNSEG)
    {
        result = BT_MAP_RSP_SUCCESS;
        if (app_mse_instance.tx_cnt == 0)
        {
            if (name == NULL)
            {
                result = BT_MAP_RSP_BAD_REQ;
            }
            else if ((name[0] == '\0') || (strspn(name, "0123456789abcdefABCDEF") != strlen(name)))
            {
                result = BT_MAP_RSP_PRECOND_FAILED;
            }
            else if (map_joinpath(&app_mse_instance.path[0], name) == 0)
            {
                /* success */
            }
            else
            {
                result = BT_MAP_RSP_PRECOND_FAILED;
            }
        }

        if (result == BT_MAP_RSP_SUCCESS)
        {
            max_pkt_len = app_mse_instance.max_pkt_len;
            max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas) - BT_L2CAP_BUF_SIZE(2U);
            max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
            rd_buf = buf->data + sizeof(struct bt_obex_hdr_bytes); /* body header */

            if ((f_open(&map_fdes, &app_mse_instance.path[0], FA_READ) != FR_OK) ||
                (f_lseek(&map_fdes, app_mse_instance.tx_cnt) != FR_OK) ||
                (f_read(&map_fdes, rd_buf, max_pkt_len, &actual) != FR_OK))
            {
                app_mse_instance.tx_cnt = 0;
                map_dirname(&app_mse_instance.path[0]);
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
            else
            {
                app_mse_instance.tx_cnt += actual;
                if (app_mse_instance.tx_cnt < f_size(&map_fdes))
                {
                    app_mse_instance.cmd_id = CMD_ID_GET_MSG;
                    BT_MAP_ADD_BODY(buf, rd_buf, actual);
                    result = BT_MAP_RSP_CONTINUE;
                }
                else
                {
                    app_mse_instance.cmd_id = CMD_ID_NONE;
                    app_mse_instance.tx_cnt = 0;
                    map_dirname(&app_mse_instance.path[0]);
                    BT_MAP_ADD_END_OF_BODY(buf, rd_buf, actual);
                    result = BT_MAP_RSP_SUCCESS;
                }
            }
            f_close(&map_fdes);
        }
    }
    else
    {
        result = BT_MAP_RSP_NOT_IMPLEMENTED;
    }
    if (bt_map_mse_get_msg_response(mse_mas, result, buf, false) != 0)
    {
        net_buf_unref(buf);
        shell_print(ctx_shell, "Failed to send response");
    }
}

static void app_mse_set_msg_status_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag)
{
    uint8_t result;
    struct map_app_param_user_data user_data;

    shell_print(ctx_shell, "MAP Set MSG Status IND - %s", MAP_FLAG_STRING(flag));
    PRINTF ("Name - %s\r\n", (name != NULL) ? name : "NULL");

    result = BT_MAP_RSP_SUCCESS;
    if ((flag & BT_OBEX_REQ_START) != 0U)
    {
        if (name == NULL)
        {
            result = BT_MAP_RSP_BAD_REQ;
        }
        else if ((name[0] == '\0') || (strspn(name, "0123456789abcdefABCDEF") != strlen(name)))
        {
            result = BT_MAP_RSP_PRECOND_FAILED;
        }
        else if (map_joinpath(&app_mse_instance.path[0], name) == 0)
        {
            /* success */
            map_msg_status.status_ind = 0xFF;
            map_msg_status.status_val = 0xFF;
            memset(&map_msg_status.ext_data[0], 0, sizeof(map_msg_status.ext_data));
        }
        else
        {
            result = BT_MAP_RSP_PRECOND_FAILED;
        }
    }

    user_data.id = CMD_ID_SET_MSG_STATUS;
    user_data.data = (void *)&map_msg_status;
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, &user_data);

    if (result == BT_MAP_RSP_SUCCESS)
    {
        if ((flag & BT_OBEX_REQ_END) != 0U)
        {
            if ((map_msg_status.status_ind == 0xFF) ||
                ((map_msg_status.status_ind == 0x00) && (map_msg_status.status_val == 0xFF)) ||
                ((map_msg_status.status_ind == 0x01) && (map_msg_status.status_val == 0xFF)) ||
                ((map_msg_status.status_ind == 0x02) && (strlen((char *)map_msg_status.ext_data) == 0)))
            {
                result = BT_MAP_RSP_BAD_REQ;
            }
            else
            {
                if (map_handle_set_msg_status(&app_mse_instance.path[0], &map_msg_status) != 0)
                {
                    result = BT_MAP_RSP_INT_SERVER_ERR;
                }
            }
            map_dirname(&app_mse_instance.path[0]);
        }
        else
        {
            result = BT_MAP_RSP_CONTINUE;
        }
    }
    net_buf_unref(buf);

    if (bt_map_mse_set_msg_status_response(mse_mas, result) != 0)
    {
        shell_print(ctx_shell, "Failed to send response");
    }
}

static void app_mse_push_msg_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag)
{
    UINT actual;
    uint8_t result;
    char msg_handle[BT_MAP_MSG_HANDLE_SIZE / 2U];
    char path[MAP_MSE_MAX_PATH_LEN];
    char *name_req = NULL;
    struct map_hdr body;

    shell_print(ctx_shell, "MAP PUSH MSG IND - %s", MAP_FLAG_STRING(flag));
    PRINTF ("Name - %s\r\n", (name != NULL) ? name : "NULL");
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);

    result = BT_MAP_RSP_SUCCESS;
    if ((flag & BT_OBEX_REQ_START) != 0U)
    {
        strcpy(&path[0], &app_mse_instance.path[0]);
        if ((name == NULL) || (name[0] == '\0'))
        {
            /* success */
        }
        else if (strchr(name, '/') != NULL)
        {
            result = BT_MAP_RSP_PRECOND_FAILED;
        }
        else if (map_joinpath(&path[0], name) == 0)
        {
            /* success */
        }
        else
        {
            result = BT_MAP_RSP_INT_SERVER_ERR;
        }

        if (result == BT_MAP_RSP_SUCCESS)
        {
            /* Not use %016llX here to avoid uint64_t is not supported in some platforms. */
            (void)sprintf(&path[strlen(&path[0])], "/%08X%08X",
            (unsigned int)(app_mse_instance.msg_handle >> 32U), (unsigned int)app_mse_instance.msg_handle);
            if (f_open(&map_fdes, &path[0], FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
            {
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
        }
    }

    if (result == BT_MAP_RSP_SUCCESS)
    {
        if (bt_map_mse_get_body(buf, &body.value, &body.length) == 0)
        {
            shell_print(ctx_shell, "============== BODY ==============");
            shell_print(ctx_shell, "%.*s", body.length, body.value);
            shell_print(ctx_shell, "============ END BODY ============");
            if ((f_write(&map_fdes, body.value, body.length, &actual) != FR_OK) ||
                (actual < body.length))
            {
                f_close(&map_fdes);
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
        }
        if (result == BT_MAP_RSP_SUCCESS)
        {
            if ((flag & BT_OBEX_REQ_END) != 0U)
            {
                (void)sprintf(&msg_handle[0], "%08X%08X",
                (unsigned int)(app_mse_instance.msg_handle >> 32U), (unsigned int)app_mse_instance.msg_handle);
                app_mse_instance.msg_handle++;
                app_mse_instance.cmd_id = CMD_ID_NONE;
                name_req = msg_handle;
                f_close(&map_fdes);
            }
            else
            {
                app_mse_instance.cmd_id = CMD_ID_PUSH_MSG;
                result = BT_MAP_RSP_CONTINUE;
            }
        }
    }
    net_buf_unref(buf);

    if (bt_map_mse_push_msg_response(mse_mas, result, name_req, false) != 0)
    {
        shell_print(ctx_shell, "Failed to send response");
    }
}

static void app_mse_set_ntf_reg_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    uint8_t result;
    struct map_app_param_user_data user_data;
    uint8_t ntf_status = 0xFF;

    shell_print(ctx_shell, "MAP Set Notification Registration IND - %s", MAP_FLAG_STRING(flag));
    user_data.id = CMD_ID_SET_NTF_REG;
    user_data.data = (void *)&ntf_status;
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, &user_data);
    net_buf_unref(buf);

    if ((flag & BT_OBEX_REQ_END) != 0U)
    {
        if (ntf_status <= 1U)
        {
            result = BT_MAP_RSP_SUCCESS;
        }
        else
        {
            result = BT_MAP_RSP_BAD_REQ;
        }
    }
    else
    {
        result = BT_MAP_RSP_CONTINUE;
    }

    if (bt_map_mse_set_ntf_reg_response(mse_mas, result) != 0)
    {
        shell_print(ctx_shell, "Failed to send response");
    }
    else
    {
        if (result == BT_MAP_RSP_SUCCESS)
        {
            if (ntf_status == 1U)
            {
                if (app_mse_instance.mse_mns == NULL)
                {
                    app_mse_instance.acl_conn = default_conn;
                    if (bt_sdp_discover(app_mse_instance.acl_conn, &discov_map_mce) != 0)
                    {
                        shell_print(ctx_shell, "SDP discovery failed: result");
                    }
                    else
                    {
                        shell_print(ctx_shell, "SDP discovery started");
                    }
                }
                else
                {
                    shell_print(ctx_shell, "MSE MNS connection alreay established");
                }
            }
            else
            {
                (void)bt_map_mse_mns_disconnect(app_mse_instance.mse_mns);
            }
        }
    }
}

static void app_mse_update_inbox_cb(struct bt_map_mse_mas *mse_mas)
{
    shell_print(ctx_shell, "MAP Update Inbox IND");

    if (bt_map_mse_update_inbox_response(mse_mas, BT_MAP_RSP_SUCCESS) != 0)
    {
        shell_print(ctx_shell, "Failed to send response");
    }
}

static void app_mse_get_mas_inst_info_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    uint16_t max_pkt_len;
    uint8_t result;
    uint16_t actual;
    char info[] = MAP_MSE_MAS_INSTANCE_INFO;

    shell_print(ctx_shell, "MAP Get MAS Instance Info IND - %s", MAP_FLAG_STRING(flag));
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);
    net_buf_unref(buf);

    buf = net_buf_alloc(&mse_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas));
    if (flag == BT_OBEX_REQ_UNSEG)
    {
        max_pkt_len = app_mse_instance.max_pkt_len;
        max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas) - BT_L2CAP_BUF_SIZE(2U);
        max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
        actual = strlen(info) - app_mse_instance.tx_cnt;
        actual = (actual > max_pkt_len) ? actual : max_pkt_len;
        if ((app_mse_instance.tx_cnt + actual) < strlen(info))
        {
            BT_MAP_ADD_BODY(buf, (uint8_t *)&info[app_mse_instance.tx_cnt], actual);
            app_mse_instance.tx_cnt += actual;
            app_mse_instance.cmd_id = CMD_ID_GET_MAS_INST_INFO;
            result = BT_MAP_RSP_CONTINUE;
        }
        else
        {
            BT_MAP_ADD_END_OF_BODY(buf, (uint8_t *)&info[app_mse_instance.tx_cnt], actual);
            app_mse_instance.tx_cnt = 0;
            app_mse_instance.cmd_id = CMD_ID_NONE;
            result = BT_MAP_RSP_SUCCESS;
        }
    }
    else
    {
        result = BT_MAP_RSP_NOT_IMPLEMENTED;
    }

    if (bt_map_mse_get_mas_inst_info_response(mse_mas, result, buf, false) != 0)
    {
        net_buf_unref(buf);
        shell_print(ctx_shell, "Failed to send response");
    }
}

static void app_mse_set_owner_status_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    uint8_t result;
    struct map_app_param_user_data user_data;

    shell_print(ctx_shell, "MAP Set Owner Status IND - %s", MAP_FLAG_STRING(flag));
    memset(&map_owner_status, 0, sizeof(map_owner_status));
    user_data.id = CMD_ID_SET_OWNER_STATUS;
    user_data.data = (void *)&map_owner_status;
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, &user_data);
    net_buf_unref(buf);

    if ((flag & BT_OBEX_REQ_END) != 0U)
    {
        result = BT_MAP_RSP_SUCCESS;
    }
    else
    {
        result = BT_MAP_RSP_CONTINUE;
    }

    if (bt_map_mse_set_owner_status_response(mse_mas, result) != 0)
    {
        shell_print(ctx_shell, "Failed to send response");
    }
}

static void app_mse_get_owner_status_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    uint8_t result;
    struct map_app_param_user_data user_data;
    char convo_id[BT_MAP_CONVO_ID_SIZE];

    shell_print(ctx_shell, "MAP Get Owner Status IND - %s", MAP_FLAG_STRING(flag));
    memset(&convo_id[0], 0, BT_MAP_CONVO_ID_SIZE);
    user_data.id = CMD_ID_GET_OWNER_STATUS;
    user_data.data = (void *)&convo_id[0];
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, &user_data);
    net_buf_unref(buf);

    buf = net_buf_alloc(&mse_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas));
    if (flag == BT_OBEX_REQ_UNSEG)
    {
        if (strcmp(convo_id, map_owner_status.convo_id) == 0)
        {
            BT_MAP_ADD_PRESENCE_AVAILABILITY(buf, map_owner_status.pres_avail);
            BT_MAP_ADD_PRESENCE_TEXT(buf, &map_owner_status.pres_text[0], strlen((char *)&map_owner_status.pres_text[0]) + 1U);
            BT_MAP_ADD_LAST_ACTIVITY(buf, &map_owner_status.last_activity[0], strlen((char *)&map_owner_status.last_activity[0]) + 1U);
            BT_MAP_ADD_CHAT_STATE(buf, map_owner_status.chat_state);
            BT_MAP_ADD_END_OF_BODY(buf, NULL, 0);
            result = BT_MAP_RSP_SUCCESS;
        }
        else
        {
            result = BT_MAP_RSP_NOT_FOUND;
        }
    }
    else
    {
        result = BT_MAP_RSP_NOT_IMPLEMENTED;
    }

    if (bt_map_mse_get_owner_status_response(mse_mas, result, buf, false) != 0)
    {
        net_buf_unref(buf);
        shell_print(ctx_shell, "Failed to send response");
    }
}

static void app_mse_get_convo_listing_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    uint8_t result;
    uint16_t max_pkt_len;
    uint16_t actual;
    char convo_listing[] = MAP_MSE_XML_CONVO_LISTING;

    shell_print(ctx_shell, "MAP Get Conversation Listing IND - %s", MAP_FLAG_STRING(flag));
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);
    net_buf_unref(buf);

    buf = net_buf_alloc(&mse_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas));
    if (flag == BT_OBEX_REQ_UNSEG)
    {
        max_pkt_len = app_mse_instance.max_pkt_len;
        max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas) - BT_L2CAP_BUF_SIZE(2U);
        max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
        actual = strlen(convo_listing) - app_mse_instance.tx_cnt;
        actual = (actual > max_pkt_len) ? max_pkt_len : actual;
        if ((app_mse_instance.tx_cnt + actual) < strlen(convo_listing))
        {
            BT_MAP_ADD_BODY(buf, (uint8_t *)&convo_listing[app_mse_instance.tx_cnt], actual);
            app_mse_instance.tx_cnt += actual;
            app_mse_instance.cmd_id = CMD_ID_GET_CONVO_LISTING;
            result = BT_MAP_RSP_CONTINUE;
        }
        else
        {
            BT_MAP_ADD_END_OF_BODY(buf, (uint8_t *)&convo_listing[app_mse_instance.tx_cnt], actual);
            app_mse_instance.tx_cnt = 0;
            app_mse_instance.cmd_id = CMD_ID_NONE;
            result = BT_MAP_RSP_SUCCESS;
        }
    }
    else
    {
        result = BT_MAP_RSP_NOT_IMPLEMENTED;
    }

    if (bt_map_mse_get_convo_listing_response(mse_mas, result, buf, false) != 0)
    {
        net_buf_unref(buf);
        shell_print(ctx_shell, "Failed to send response");
    }
}

static void app_mse_set_ntf_filter_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    uint8_t result;

    shell_print(ctx_shell, "MAP Set Notification Filter IND - %s", MAP_FLAG_STRING(flag));
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);
    net_buf_unref(buf);

    if ((flag & BT_OBEX_REQ_END) != 0U)
    {
        result = BT_MAP_RSP_SUCCESS;
    }
    else
    {
        result = BT_MAP_RSP_CONTINUE;
    }

    if (bt_map_mse_set_ntf_filter_response(mse_mas, result) != 0)
    {
        shell_print(ctx_shell, "Failed to send response");
    }
}

static int cmd_mse_register(const struct shell *sh, size_t argc, char *argv[])
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;
    int err;

    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(MAP_MSE_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        err = -ENOBUFS;
        shell_error(sh, "write class of device failed, Reason : %d", err);
    }
    bt_sdp_register_service(&map_mse_inst_1_rec);
    bt_sdp_register_service(&map_mse_inst_2_rec);

    bt_map_mse_mas_register(&mse_mas_cb);

    return 0;
}

static int cmd_mse_unregister(const struct shell *sh, size_t argc, char *argv[])
{
    bt_map_mse_mas_unregister();

    return 0;
}

static int cmd_mse_mns_register(const struct shell *sh, size_t argc, char *argv[])
{
    bt_map_mse_mns_register(&mse_mns_cb);
    return 0;
}

static int cmd_mse_mns_unregister(const struct shell *sh, size_t argc, char *argv[])
{
    bt_map_mse_mns_unregister();
    return 0;
}

static int cmd_mse_disconnect(const struct shell *sh, size_t argc, char *argv[])
{
    int err = bt_map_mse_disconnect(app_mse_instance.mse_mas);

    if (err < 0)
    {
        shell_error(sh, "disconnect failed Reason %d", err);
        return -EINVAL;
    }
    return 0;
}

static int cmd_mse_mns_disconnect(const struct shell *sh, size_t argc, char *argv[])
{
    int err = bt_map_mse_mns_disconnect(app_mse_instance.mse_mns);

    if (err < 0)
    {
        shell_error(sh, "disconnect MNS failed Reason %d", err);
        return -EINVAL;
    }
    return 0;
}

static int cmd_send_event(const struct shell *sh, size_t argc, char *argv[])
{
    struct net_buf *buf;
    uint16_t max_pkt_len;
    uint16_t actual;
    char event_report[] = MAP_MSE_XML_EVENT_REPORT;
    enum bt_obex_req_flags flag;
    uint32_t tag_u32;

    buf = net_buf_alloc(&mse_mns_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_EVENT(mse_mns, BT_OBEX_REQ_UNSEG));
    for (uint8_t index = 1; index < argc - 1; index++)
    {
        if (strcmp(argv[index], "-mii") == 0)
        {
            if ((str_to_int(argv[index + 1U], &tag_u32) == 0) && (tag_u32 <= 255U))
            {
                BT_MAP_ADD_MAS_INSTANCE_ID(buf, tag_u32);
            }
            else
            {
                shell_error(ctx_shell, "MAS_INSTANCE_ID invaild");
                net_buf_unref(buf);
                return -EINVAL;
            }
        }
        else
        {
            /* no action */
        }
    }

    max_pkt_len = app_mse_instance.mns_max_pkt_len;
    max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_EVENT(mse_mns, BT_OBEX_REQ_UNSEG) - BT_L2CAP_BUF_SIZE(2U);
    max_pkt_len -= buf->len; /* application parameter */
    max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
    actual = strlen(event_report);
    if (actual > max_pkt_len)
    {
        flag = BT_OBEX_REQ_START;
        BT_MAP_ADD_BODY(buf, (uint8_t *)event_report, max_pkt_len);
        app_mse_instance.mns_tx_cnt += max_pkt_len;
    }
    else
    {
        flag = BT_OBEX_REQ_UNSEG;
        BT_MAP_ADD_END_OF_BODY(buf, (uint8_t *)event_report, actual);
        app_mse_instance.mns_tx_cnt = 0;
    }

    if (bt_map_mse_send_event(app_mse_instance.mse_mns, buf, flag) != 0)
    {
        net_buf_unref(buf);
        shell_print(ctx_shell, "Failed to send event report");
    }
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(mse_cmds,
   SHELL_CMD_ARG(register, NULL, HELP_NONE, cmd_mse_register, 1, 0),
   SHELL_CMD_ARG(unregister, NULL, HELP_NONE, cmd_mse_unregister, 1, 0),
   SHELL_CMD_ARG(mns_register, NULL, HELP_NONE, cmd_mse_mns_register, 1, 0),
   SHELL_CMD_ARG(mns_unregister, NULL, HELP_NONE, cmd_mse_mns_unregister, 1, 0),
   SHELL_CMD_ARG(disconnect, NULL, HELP_NONE, cmd_mse_disconnect, 1, 0),
   SHELL_CMD_ARG(mns_disconnect, NULL, HELP_NONE, cmd_mse_mns_disconnect, 1, 0),
   SHELL_CMD_ARG(send_event, NULL, "\r\n\
               input application parameters(mandatory).\r\n\
               1: -mii : [MASInstanceID (0 - 255)].", cmd_send_event, 3, 0),
   SHELL_SUBCMD_SET_END);
#endif

static int cmd_mce_mse(const struct shell *sh, size_t argc, char **argv)
{
    if (argc == 1)
    {
        shell_help(sh);
        /* shell returns 1 when help is printed */
        return SHELL_CMD_HELP_PRINTED;
    }

    shell_error(sh, "%s unknown parameter: %s", argv[0], argv[1]);

    return -EINVAL;
}

#if ((defined(CONFIG_BT_MAP_MCE) && ((CONFIG_BT_MAP_MCE) > 0U)) || \
     (defined(CONFIG_BT_MAP_MSE) && ((CONFIG_BT_MAP_MSE) > 0U)))
SHELL_STATIC_SUBCMD_SET_CREATE(map_cmds,
#if (defined(CONFIG_BT_MAP_MCE) && ((CONFIG_BT_MAP_MCE) > 0U))
    SHELL_CMD_ARG(mce, mce_cmds, HELP_NONE, cmd_mce_mse, 1, 0),
#endif
#if (defined(CONFIG_BT_MAP_MSE) && ((CONFIG_BT_MAP_MSE) > 0U))
    SHELL_CMD_ARG(mse, mse_cmds, HELP_NONE, cmd_mce_mse, 1, 0),
#endif
	SHELL_SUBCMD_SET_END
);

static int cmd_map(const struct shell *sh, size_t argc, char **argv)
{
    if (argc == 1)
    {
        shell_help(sh);
        /* shell returns 1 when help is printed */
        return SHELL_CMD_HELP_PRINTED;
    }

    shell_error(sh, "%s unknown parameter: %s", argv[0], argv[1]);

    return -EINVAL;
}

SHELL_CMD_ARG_REGISTER(map, map_cmds, "Bluetooth MAP shell commands", cmd_map, 1, 1);

void bt_ShellMapInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandmap))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandmap.pcCommand);
    }
}

#endif /* CONFIG_BT_MAP_MCE || CONFIG_BT_MAP_MSE */