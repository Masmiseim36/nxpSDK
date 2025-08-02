/* map.c - Bluetooth L2CAP Tester */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if ((defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)) || \
     (defined(CONFIG_BT_MAP_MSE) && (CONFIG_BT_MAP_MSE > 0)))

#include <porting.h>

#include "bt_pal_conn_internal.h"

#include <bluetooth/l2cap.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/map_mce.h>
#include <bluetooth/map_mse.h>
#include <bluetooth/sdp.h>

#include "board.h"

#include "fsl_adapter_audio.h"
#include "fsl_codec_common.h"

#include "btp/btp.h"

#if defined(CONFIG_BT_MAP_MSE) && (CONFIG_BT_MAP_MSE > 0)
#include "ff.h"
#include "diskio.h"
#endif

#include "fsl_component_log_config.h"
#define LOG_MODULE_NAME bttester_map
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#define SDP_CLIENT_USER_BUF_LEN 512U
#define MAX_BUFFER_SIZE 2048
#define MAX_NAME_LEN (64U)

static struct bt_conn *default_conn;
NET_BUF_POOL_FIXED_DEFINE(sdp_client_pool, CONFIG_BT_MAX_CONN, SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
/*
 * map_buf - cache used by a map client (to cache data read)
 * and map server (to store response data).
 * It is not intended to be used by client and server at the same time.
 */
static struct {
	uint16_t len;
	uint8_t buf[MAX_BUFFER_SIZE];
} map_buf;
static struct {
    void *instance;
    uint8_t cmd;
    uint8_t flags;
    char name[MAX_NAME_LEN];
    struct net_buf *buf;
} map_cmd;

struct map_conn_info
{
    struct bt_conn *acl_conn;
    void *handle;
    uint16_t psm;
    uint8_t scn;
};

static void *map_buf_add(const void *data, size_t len)
{
	void *ptr = map_buf.buf + map_buf.len;

	if ((len + map_buf.len) > MAX_BUFFER_SIZE) {
		return NULL;
	}

	if (data) {
		memcpy(ptr, data, len);
	} else {
		(void)memset(ptr, 0, len);
	}

	map_buf.len += len;

	LOG_DBG("%d/%d used", map_buf.len, MAX_BUFFER_SIZE);

	return ptr;
}

static void map_buf_clear(void)
{
	(void)memset(&map_buf, 0, sizeof(map_buf));
}

static void map_ev_failure(struct bt_conn *conn, uint8_t instance_id, uint8_t event)
{
    uint8_t result = BTP_STATUS_FAILED;
    map_buf_add(bt_conn_get_dst_br(conn), sizeof(bt_addr_t));
    map_buf_add(&instance_id, sizeof(instance_id));
    map_buf_add(&result, sizeof(result));
    tester_event(BTP_SERVICE_ID_MAP, event, &result, sizeof(result));
}

static void map_ev_success(struct bt_conn *conn, uint8_t instance_id, uint8_t event, uint8_t result, struct net_buf *buf, const char *name)
{
    map_buf_add(bt_conn_get_dst_br(conn), sizeof(bt_addr_t));
    map_buf_add(&instance_id, sizeof(instance_id));
    map_buf_add(&result, sizeof(result));
    if (name != NULL)
    {
        uint8_t hi = BT_MAP_HDR_NAME;
        uint16_t hv_len = strlen(name) + 4; /* 4 = 1(hi) + 2(hv_len) + 1(null terminated) */

        hv_len = sys_cpu_to_be16(hv_len);
        map_buf_add(&hi, sizeof(hi));
        map_buf_add(&hv_len, sizeof(hv_len));
        map_buf_add(name, strlen(name) + 1);
    }
    if (buf != NULL)
    {
        map_buf_add(buf->data, buf->len);
        net_buf_unref(buf);
    }
    tester_event(BTP_SERVICE_ID_MAP, event, &map_buf.buf, map_buf.len);
    map_buf_clear();
}

#if defined(CONFIG_BT_MAP_MSE) && (CONFIG_BT_MAP_MSE > 0)
#define MAP_MSE_CLASS_OF_DEVICE (0x100214U) /* Object Transfer, Phone, Palm-size PC/PDA */

#define MAP_MSE_MAS_TX_NET_BUF_COUNT   (1U)
#define MAP_MSE_MNS_TX_NET_BUF_COUNT   (1U)
#define MAP_MSE_MAS_TX_NET_BUF_SIZE    (1790U + 2U) /* L2CAP I-frame Enhanced Control Field(2-byte) */
#define MAP_MSE_MNS_TX_NET_BUF_SIZE    (CONFIG_BT_MAP_MSE_MNS_MAX_PKT_LEN + 2U) /* L2CAP I-frame Enhanced Control Field(2-byte) */

#define MAP_MSE_DRIVE_NUMBER "0:" /* RAMDISK + '0' */
#define MAP_MSE_REPO_ROOT "0:/root0" /* RAMDISK + '0' */
#define MAP_MSE_XML_FOLDER_LISTING_NAME "folder_listing.xml"
#define MAP_MSE_XML_MSG_LISTING_NAME    "msg_listing.xml"
#define MAP_MSE_XML_EVENT_REPORT_PATH   "0:/event_report.xml"
#define MAP_MSE_MAS_SUPPORTED_FEATURES (0x000E03FFU)
#define MAP_MSE_MAS_SDP_COUNT (1U)
#define MAP_MSE_MAS_INSTANCE_INFO "SMS"
#define MAP_MSE_TIME "20180101T000000+0000"
#define MAP_MSE_MAX_PATH_LEN (45U)

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

struct map_mse_mas_instance
{
    struct map_conn_info conn_info;
    uint64_t msg_handle;
    const char *root_path;
    uint16_t max_pkt_len; /* range from opcode to the end of packet */
    uint16_t tx_cnt;
    uint8_t instance_id;
    char path[MAP_MSE_MAX_PATH_LEN];
    char msg_path[MAP_MSE_MAX_PATH_LEN];
    enum map_cmd_id cmd_id;
};

struct map_mse_mns_instance
{
    struct map_conn_info conn_info;
    uint32_t supported_features;
    uint16_t mns_max_pkt_len; /* range from opcode to the end of packet */
    uint16_t mns_tx_cnt;
    bool set_ntf_reg;
    char msg_path[sizeof(MAP_MSE_XML_EVENT_REPORT_PATH)];
};

struct map_hdr
{
    uint8_t *value;
    uint16_t length;
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

struct map_event_report
{
    const char *event_type;
    const char *handle;
    const char *folder;
    const char *msg_type;
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

struct map_mse_mas_instance mse_mas_instances[CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES];
struct map_mse_mns_instance mse_mns_instances[CONFIG_BT_MAP_MSE_MNS_NUM_INSTANCES];
NET_BUF_POOL_FIXED_DEFINE(mse_mas_tx_pool, MAP_MSE_MAS_TX_NET_BUF_COUNT, BT_L2CAP_BUF_SIZE(MAP_MSE_MAS_TX_NET_BUF_SIZE), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
NET_BUF_POOL_FIXED_DEFINE(mse_mns_tx_pool, MAP_MSE_MNS_TX_NET_BUF_COUNT, BT_L2CAP_BUF_SIZE(MAP_MSE_MNS_TX_NET_BUF_SIZE), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
static FATFS map_fatfs;
static BYTE map_fatfs_work[FF_MAX_SS];
static FIL map_fsrc;
static FIL map_fdes;
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
    // .set_owner_status = app_mse_set_owner_status_cb,
    // .get_owner_status = app_mse_get_owner_status_cb,
    // .get_convo_listing = app_mse_get_convo_listing_cb,
    .set_ntf_filter = app_mse_set_ntf_filter_cb,
};

struct bt_map_mse_mns_cb mse_mns_cb = {
    .connected = map_mse_mns_connected,
    .disconnected = map_mse_mns_disconnected,
    .send_event = app_mse_send_event_cb,
};

static struct bt_sdp_attribute map_mse_attrs[] = {
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
        BT_SDP_ARRAY_8(0x02)
    },
    /*  SupportedFeatures */
    BT_SDP_ATTR_MAP_SUPPORTED_FEATURES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT32), 
        BT_SDP_ARRAY_32(MAP_MSE_MAS_SUPPORTED_FEATURES)
    },
};

static struct bt_sdp_record map_mse_rec = BT_SDP_RECORD(map_mse_attrs);

static struct map_mse_mas_instance *map_mse_allocate_mas_instance(void)
{
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES; index++)
    {
        if (mse_mas_instances[index].conn_info.handle == NULL)
        {
            memset(&mse_mas_instances[index], 0, sizeof(struct map_mse_mas_instance));
            return &mse_mas_instances[index];
        }
    }
    return NULL;
}

static struct map_mse_mns_instance *map_mse_allocate_mns_instance(void)
{
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MNS_NUM_INSTANCES; index++)
    {
        if (mse_mns_instances[index].conn_info.handle == NULL)
        {
            memset(&mse_mns_instances[index], 0, sizeof(struct map_mse_mns_instance));
            return &mse_mns_instances[index];
        }
    }
    return NULL;
}

static struct map_mse_mas_instance *map_mse_lookup_mas_instance(void *handle)
{
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MAS_NUM_INSTANCES; index++)
    {
        if (mse_mas_instances[index].conn_info.handle == handle)
        {
            return &mse_mas_instances[index];
        }
    }
    return NULL;
}

static struct map_mse_mns_instance *map_mse_lookup_mns_instance(void *handle)
{
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MNS_NUM_INSTANCES; index++)
    {
        if (mse_mns_instances[index].conn_info.handle == handle)
        {
            return &mse_mns_instances[index];
        }
    }
    return NULL;
}

static void map_mse_free_mas_instance(void *handle)
{
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(handle);

    if (instance != NULL)
    {
        memset(instance, 0, sizeof(struct map_mse_mas_instance));
    }
}

static void map_mse_free_mns_instance(void *handle)
{
    struct map_mse_mns_instance *instance = map_mse_lookup_mns_instance(handle);

    if (instance != NULL)
    {
        memset(instance, 0, sizeof(struct map_mse_mns_instance));
    }
}

static struct map_mse_mns_instance *map_mse_lookup_mns_instance_by_conn(struct bt_conn *conn)
{
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MSE_MNS_NUM_INSTANCES; index++)
    {
        if (mse_mns_instances[index].conn_info.acl_conn == conn)
        {
            return &mse_mns_instances[index];
        }
    }
    return NULL;
}

static uint8_t sdp_map_mce_user(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
    int res;
    uint16_t scn;
    uint16_t psm = 0;
    uint32_t supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V11;
    uint16_t map_version = BT_MAP_VERSION_1_1;
    const char *service_name;
    struct map_mse_mns_instance *mns_instance;
    struct btp_map_mce_sdp_discover_ev ev;

    if ((result) && (result->resp_buf))
    {
        res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_RFCOMM, &scn);
        if (res < 0)
        {
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
        bt_sdp_get_goep_l2cap_psm(result->resp_buf, &psm);
        bt_sdp_get_profile_version(result->resp_buf, BT_SDP_MAP_SVCLASS, &map_version);
        bt_sdp_get_pbap_map_ctn_features(result->resp_buf, &supported_features);
        bt_sdp_get_service_name(result->resp_buf, &service_name);

        memset(&ev, 0, sizeof(ev));
        bt_addr_copy(&ev.address, bt_conn_get_dst_br(conn));
        ev.scn = scn;
        ev.psm = psm;
        ev.supported_features = supported_features;
        ev.map_version = map_version;
        tester_event(BTP_SERVICE_ID_MAP, BTP_MAP_EV_SDP_DISCOVER, &ev, sizeof(ev));

        mns_instance = map_mse_lookup_mns_instance_by_conn(conn);
        if (mns_instance != NULL)
        {
            mns_instance->supported_features = supported_features;
            mns_instance->conn_info.psm = psm;
            mns_instance->conn_info.scn = scn;
            if ((mns_instance->conn_info.handle == NULL) && (mns_instance->set_ntf_reg == true))
            {
                mns_instance->set_ntf_reg = false;
                if (psm != 0)
                {
                    res = bt_map_mse_mns_psm_connect(conn, psm, supported_features, (struct bt_map_mse_mns **)&mns_instance->conn_info.handle);
                }
                else
                {
                    res = bt_map_mse_mns_scn_connect(conn, (uint8_t)scn, supported_features, (struct bt_map_mse_mns **)&mns_instance->conn_info.handle);
                }
            }
        }

        return BT_SDP_DISCOVER_UUID_STOP;
    }
    else
    {
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

static int map_fs_mount(void)
{
    MKFS_PARM formatOptions;

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

    return 0;
}

static int map_fs_init(struct map_mse_mas_instance *instance)
{
    UINT actual;
    FATFS *fs;
    DWORD fre_clust;
    WORD sector_size;

    (void)sprintf(&instance->path[0], "%s", instance->root_path);
    if (f_mkdir((const TCHAR *)&instance->path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&instance->path[0], "%s/telecom", instance->root_path);
    if (f_mkdir((const TCHAR *)&instance->path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&instance->path[0], "%s/telecom/msg", instance->root_path);
    if (f_mkdir((const TCHAR *)&instance->path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&instance->path[0], "%s/telecom/msg/inbox", instance->root_path);
    if (f_mkdir((const TCHAR *)&instance->path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&instance->path[0], "%s/telecom/msg/outbox", instance->root_path);
    if (f_mkdir((const TCHAR *)&instance->path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&instance->path[0], "%s/telecom/msg/sent", instance->root_path);
    if (f_mkdir((const TCHAR *)&instance->path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&instance->path[0], "%s/telecom/msg/deleted", instance->root_path);
    if (f_mkdir((const TCHAR *)&instance->path[0]) != FR_OK)
    {
        return -EIO;
    }
    (void)sprintf(&instance->path[0], "%s/telecom/msg/draft", instance->root_path);
    if (f_mkdir((const TCHAR *)&instance->path[0]) != FR_OK)
    {
        return -EIO;
    }

    /* write one message into inbox */
    /* Not use %016llX here to avoid uint64_t is not supported in some platforms. */
    (void)sprintf(&instance->path[0], "%s/telecom/msg/inbox/%08X%08X", instance->root_path,
    (unsigned int)(instance->msg_handle >> 32U), (unsigned int)instance->msg_handle);
    instance->msg_handle++;
    if (f_open(&map_fdes, &instance->path[0], FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
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
    (void)sprintf(&instance->path[0], "%s", instance->root_path);

    return 0;
}

static int map_search_msg(char *dir_name, const char *file_name)
{
    DIR dir;
    FILINFO fno;

    if (f_opendir(&dir, dir_name) != FR_OK)
    {
        return -ENOENT;
    }

    for (;;)
    {
        if (f_readdir(&dir, &fno) != FR_OK || fno.fname[0] == 0)
        {
            break;
        }

        if ((fno.fattrib & AM_DIR) != 0U)
        {
            /* directory */
            if (strlen(dir_name) + strlen("/") + strlen(fno.fname) >= MAP_MSE_MAX_PATH_LEN)
            {
                return -ENAMETOOLONG;
            }
            strcat(dir_name, "/");
            strcat(dir_name, fno.fname);
            if (map_search_msg(dir_name, file_name) == 0)
            {
                return 0; /* file is found */
            }
            map_dirname(dir_name);
        }
        else if (strcmp(fno.fname, file_name) == 0)
        {
            /* file is found */
            if (strlen(dir_name) + strlen("/") + strlen(fno.fname) >= MAP_MSE_MAX_PATH_LEN)
            {
                return -ENAMETOOLONG;
            }
            strcat(dir_name, "/");
            strcat(dir_name, fno.fname);

            return 0;
        }
        else
        {
            /* no action */
        }
    }

    return -ENOENT;
}

static void map_mse_mns_connected(struct bt_map_mse_mns *mse_mns)
{
    struct btp_map_mse_mns_connected_ev ev;
    struct map_mse_mns_instance *mns_instance = map_mse_lookup_mns_instance(mse_mns);

    if (mns_instance == NULL)
    {
        return;
    }
    if (bt_map_mse_mns_get_max_pkt_len(mse_mns, &mns_instance->mns_max_pkt_len) == 0)
    {
        ev.scn = mns_instance->conn_info.scn;
        ev.psm = mns_instance->conn_info.psm;
        ev.max_pkt_len = mns_instance->mns_max_pkt_len;
        bt_addr_copy(&ev.address, bt_conn_get_dst_br(mns_instance->conn_info.acl_conn));
        tester_event(BTP_SERVICE_ID_MAP, BTP_MAP_EV_MSE_MNS_CONNECTED, &ev, sizeof(ev));
    }
}

static void map_mse_mns_disconnected(struct bt_map_mse_mns *mse_mns, uint8_t result)
{
    struct btp_map_mse_mns_disconnected_ev ev;
    struct map_mse_mns_instance *mns_instance = map_mse_lookup_mns_instance(mse_mns);

    if (mns_instance == NULL)
    {
        return;
    }
    ev.result = result;
    ev.psm = mns_instance->conn_info.psm;
    ev.scn = mns_instance->conn_info.scn;
    bt_addr_copy(&ev.address, bt_conn_get_dst_br(mns_instance->conn_info.acl_conn));
    tester_event(BTP_SERVICE_ID_MAP, BTP_MAP_EV_MSE_MNS_DISCONNECTED, &ev, sizeof(ev));
    map_mse_free_mns_instance(mse_mns);
}

static void map_mse_mas_connected(struct bt_map_mse_mas *mse_mas, uint16_t psm, uint8_t scn)
{
    struct btp_map_mse_mas_connected_ev ev;
    struct map_mse_mns_instance *mns_instance;
    struct map_mse_mas_instance *instance = map_mse_allocate_mas_instance();
    const char *root_path = NULL;
    struct {
        uint16_t psm;
        uint8_t scn;
        uint8_t instance_id;
        const char *path;
    } table[MAP_MSE_MAS_SDP_COUNT] = {
        {BT_BR_PSM_MAP_MSE_1, BT_RFCOMM_CHAN_MAP_MSE, 0, MAP_MSE_REPO_ROOT},
    };
    uint8_t mas_intance_id;

    /* Find root path and MAS instance id */
    for (uint8_t index = 0; index < MAP_MSE_MAS_SDP_COUNT; index++)
    {
        if ((table[index].psm == psm) && (table[index].scn == scn))
        {
            root_path = table[index].path;
            mas_intance_id = table[index].instance_id;
            break;
        }
    }

    if ((instance == NULL) || (root_path == NULL))
    {
        return;
    }
    if (bt_map_mse_get_max_pkt_len(mse_mas, &instance->max_pkt_len) == 0)
    {
        instance->conn_info.acl_conn = default_conn;
        instance->conn_info.handle = mse_mas;
        instance->conn_info.psm = psm;
        instance->conn_info.scn = scn;
        instance->msg_handle = 0;
        instance->instance_id = mas_intance_id;
        instance->root_path = root_path;
        instance->tx_cnt = 0;
        map_fs_init(instance);

        mns_instance = map_mse_lookup_mns_instance_by_conn(instance->conn_info.acl_conn);
        if (mns_instance == NULL)
        {
            mns_instance = map_mse_allocate_mns_instance();
        }
        if (mns_instance != NULL)
        {
            mns_instance->conn_info.acl_conn = instance->conn_info.acl_conn;
            mns_instance->set_ntf_reg = false;
            bt_sdp_discover(instance->conn_info.acl_conn, &discov_map_mce);
        }

        ev.instance_id = instance->instance_id;
        ev.psm = psm;
        ev.scn = scn;
        ev.max_pkt_len = instance->max_pkt_len;
        bt_addr_copy(&ev.address, bt_conn_get_dst_br(instance->conn_info.acl_conn));
        tester_event(BTP_SERVICE_ID_MAP, BTP_MAP_EV_MSE_MAS_CONNECTED, &ev, sizeof(ev));
    }
}

static void map_mse_mas_disconnected(struct bt_map_mse_mas *mse_mas, uint8_t result)
{
    struct btp_map_mse_mas_disconnected_ev ev;
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);

    if (instance == NULL)
    {
        return;
    }
    ev.result = result;
    ev.instance_id = instance->instance_id;
    ev.psm = instance->conn_info.psm;
    ev.scn = instance->conn_info.scn;
    bt_addr_copy(&ev.address, bt_conn_get_dst_br(instance->conn_info.acl_conn));
    tester_event(BTP_SERVICE_ID_MAP, BTP_MAP_EV_MSE_MAS_DISCONNECTED, &ev, sizeof(ev));
    map_mse_free_mas_instance(mse_mas);
}

static void app_mse_abort_cb(struct bt_map_mse_mas *mse_mas)
{
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);

    if (instance == NULL)
    {
        return;
    }

    switch (instance->cmd_id)
    {
        case CMD_ID_GET_FOLDER_LISTING:
        case CMD_ID_GET_MSG_LISTING:
        case CMD_ID_GET_MAS_INST_INFO:
        case CMD_ID_GET_CONVO_LISTING:
        case CMD_ID_GET_MSG:
        case CMD_ID_PUSH_MSG:
            instance->tx_cnt = 0;
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

    instance->cmd_id = CMD_ID_NONE;
}

static void app_mse_send_event_cb(struct bt_map_mse_mns *mse_mns, uint8_t result)
{
    struct net_buf *buf;
    uint8_t *rd_buf;
    uint16_t max_pkt_len;
    UINT actual;
    enum bt_obex_req_flags flag;
    struct map_mse_mns_instance *mns_instance = map_mse_lookup_mns_instance(mse_mns);

    if (mns_instance == NULL)
    {
        return;
    }

    if (result == BT_MAP_RSP_CONTINUE)
    {
        buf = net_buf_alloc(&mse_mns_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_EVENT(mse_mns, BT_OBEX_REQ_END));
        if (mns_instance->mns_tx_cnt < f_size(&map_fdes))
        {
            max_pkt_len = mns_instance->mns_max_pkt_len;
            max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_EVENT(mse_mns, BT_OBEX_REQ_END) - BT_L2CAP_BUF_SIZE(2U);
            max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
            rd_buf = buf->data + sizeof(struct bt_obex_hdr_bytes); /* application parameter and body header */

            if ((f_open(&map_fdes, &mns_instance->msg_path[0], FA_READ) != FR_OK) ||
                (f_lseek(&map_fdes, mns_instance->mns_tx_cnt) != FR_OK) ||
                (f_read(&map_fdes, rd_buf, max_pkt_len, &actual) != FR_OK))
            {
                f_close(&map_fdes);
                mns_instance->mns_tx_cnt = 0;
                flag = BT_OBEX_REQ_END;
            }
            else
            {
                mns_instance->mns_tx_cnt += actual;
                if (mns_instance->mns_tx_cnt < f_size(&map_fdes))
                {
                    flag = BT_OBEX_REQ_CONTINUE;
                    BT_MAP_ADD_BODY(buf, rd_buf, actual);
                }
                else
                {
                    flag = BT_OBEX_REQ_END;
                    BT_MAP_ADD_END_OF_BODY(buf, rd_buf, actual);
                }
            }
            f_close(&map_fdes);
        }
        else
        {
            mns_instance->mns_tx_cnt = 0;
            flag = BT_OBEX_REQ_END;
        }
        if (bt_map_mse_send_event((struct bt_map_mse_mns *)mns_instance->conn_info.handle, buf, flag) != 0)
        {
            net_buf_unref(buf);
            map_ev_failure(mns_instance->conn_info.acl_conn, 0, BTP_MAP_EV_MSE_SEND_EVENT);
        }
    }
    else
    {
        mns_instance->mns_tx_cnt = 0;
        map_ev_success(mns_instance->conn_info.acl_conn, 0, BTP_MAP_EV_MSE_SEND_EVENT, result, NULL, NULL);
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
            break;

        case BT_MAP_TAG_ID_LIST_START_OFFSET:
            if (data->data_len < 2U)
            {
                return false;
            }
            break;

        case BT_MAP_TAG_ID_FILTER_MESSAGE_TYPE:
            break;

        case BT_MAP_TAG_ID_FILTER_PERIOD_BEGIN:
            break;

        case BT_MAP_TAG_ID_FILTER_PERIOD_END:
            break;

        case BT_MAP_TAG_ID_FILTER_READ_STATUS:
            break;

        case BT_MAP_TAG_ID_FILTER_RECIPIENT:
            break;

        case BT_MAP_TAG_ID_FILTER_ORIGINATOR:
            break;

        case BT_MAP_TAG_ID_FILTER_PRIORITY:
            break;

        case BT_MAP_TAG_ID_ATTACHMENT:
            break;

        case BT_MAP_TAG_ID_TRANSPARENT:
            break;

        case BT_MAP_TAG_ID_RETRY:
            break;

        case BT_MAP_TAG_ID_NOTIFICATION_STATUS:
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_NTF_REG))
            {
                *(uint8_t *)app_param->data = data->data[0];
            }
            break;

        case BT_MAP_TAG_ID_MAS_INSTANCE_ID:
            break;

        case BT_MAP_TAG_ID_PARAMETER_MASK:
            if (data->data_len < 4U)
            {
                return false;
            }
            break;

        case BT_MAP_TAG_ID_SUBJECT_LENGTH:
            break;

        case BT_MAP_TAG_ID_CHARSET:
            break;

        case BT_MAP_TAG_ID_FRACTION_REQUEST:
            break;

        case BT_MAP_TAG_ID_STATUS_INDICATOR:
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_MSG_STATUS))
            {
                ((struct map_msg_status *)app_param->data)->status_ind = data->data[0];
            }
            break;

        case BT_MAP_TAG_ID_STATUS_VALUE:
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_MSG_STATUS))
            {
                ((struct map_msg_status *)app_param->data)->status_val = data->data[0];
            }
            break;

        case BT_MAP_TAG_ID_PRESENCE_AVAILABILITY:
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_OWNER_STATUS))
            {
                ((struct map_owner_status *)app_param->data)->pres_avail = data->data[0];
            }
            break;

        case BT_MAP_TAG_ID_PRESENCE_TEXT:
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_OWNER_STATUS))
            {
                uint8_t length = data->data_len > (sizeof(((struct map_owner_status *)app_param->data)->pres_text) - 1U) ?
                                 (sizeof(((struct map_owner_status *)app_param->data)->pres_text) - 1U) : data->data_len;
                memcpy(((struct map_owner_status *)app_param->data)->pres_text, data->data, length);
            }
            break;

        case BT_MAP_TAG_ID_LAST_ACTIVITY:
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_OWNER_STATUS))
            {
                uint8_t length = data->data_len > (sizeof(((struct map_owner_status *)app_param->data)->last_activity) - 1U) ?
                                 (sizeof(((struct map_owner_status *)app_param->data)->last_activity) - 1U) : data->data_len;
                memcpy(((struct map_owner_status *)app_param->data)->last_activity, data->data, length);
            }
            break;

        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_BEGIN:
            break;

        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_END:
            break;

        case BT_MAP_TAG_ID_CHAT_STATE:
            if ((app_param != NULL) && (app_param->id == CMD_ID_SET_OWNER_STATUS))
            {
                ((struct map_owner_status *)app_param->data)->chat_state = data->data[0];
            }
            break;

        case BT_MAP_TAG_ID_CONVERSATION_ID:
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
            break;

        case BT_MAP_TAG_ID_NOTIFICATION_FILTER_MASK:
            if (data->data_len < 4U)
            {
                return false;
            }
            break;

        case BT_MAP_TAG_ID_CONV_PARAMETER_MASK:
            if (data->data_len < 4U)
            {
                return false;
            }
            break;

        case BT_MAP_TAG_ID_EXTENDED_DATA:
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
            break;

        case BT_MAP_TAG_ID_MESSAGE_HANDLE:
            break;

        case BT_MAP_TAG_ID_MODIFY_TEXT:
            break;

        default:
            break;
    }

    return true;
}

static int map_create_xml_folder_listing(struct map_mse_mas_instance *instance)
{
    int err = 0;
    DIR dir;
    FILINFO fno;
    UINT actual;
    const char *xml_name = &instance->msg_path[0];
    const char *dir_name = &instance->path[0];
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

static int map_create_event_report(struct map_mse_mns_instance *mns_instance, struct map_event_report *event_report)
{
    int err = 0;
    FRESULT result;
    UINT actual;
    const char *xml_name = &mns_instance->msg_path[0];
    const char head[] = "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\r\n<MAP-event-report version=\"";
    const char tail[] = "</MAP-event-report>";
    uint32_t supported_features = mns_instance->supported_features & MAP_MSE_MAS_SUPPORTED_FEATURES;

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
    if ((supported_features & BT_MAP_EXT_EVENT_VERSION_1_2) != 0)
    {
        (void)f_printf(&map_fdes, "1.2");
    }
    else if ((supported_features & BT_MAP_EXT_EVENT_REPORT_1_1) != 0)
    {
        (void)f_printf(&map_fdes, "1.1");
    }
    else
    {
        (void)f_printf(&map_fdes, "1.0");
    }
    (void)f_printf(&map_fdes, "\">\r\n");
    (void)f_printf(&map_fdes, "    <event type = \"%s\" ", event_report->event_type);
    (void)f_printf(&map_fdes, "handle = \"%s\" ", event_report->handle);
    (void)f_printf(&map_fdes, "folder = \"%s\" ", event_report->folder);
    (void)f_printf(&map_fdes, "msg_type = \"%s\" ", event_report->msg_type);
    (void)f_printf(&map_fdes, "/>\r\n");
    if ((f_write(&map_fdes, tail, strlen(tail), &actual) != FR_OK) || (actual < strlen(tail)))
    {
        err = -EIO;
    }
    f_close(&map_fdes);

    return err;
}

static int map_create_xml_msg_listing(struct map_mse_mas_instance *instance, struct map_xml_msg_listing *out)
{
    int err = 0;
    DIR dir;
    FILINFO fno;
    UINT actual;
    struct map_mse_mns_instance *mns_instance;
    const char *dir_name = &instance->path[0];
    const char *xml_name = &instance->msg_path[0];
    char path[MAP_MSE_MAX_PATH_LEN];
    char head[] = "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\r\n<MAP-msg-listing version=\"";
    char tail[] = "</MAP-msg-listing>";
    uint32_t supported_features = 0;
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

    mns_instance = map_mse_lookup_mns_instance_by_conn(instance->conn_info.acl_conn);
    if (mns_instance != NULL)
    {
        supported_features = mns_instance->supported_features & MAP_MSE_MAS_SUPPORTED_FEATURES;
    }

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
    if ((supported_features & BT_MAP_MSG_LISTING_FORMAT_VERSION_1_1) != 0)
    {
        (void)f_printf(&map_fdes, "1.1");
    }
    else
    {
        (void)f_printf(&map_fdes, "1.0");
    }
    (void)f_printf(&map_fdes, "\">\r\n");
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
    struct net_buf *rx_buf = buf;
    UINT actual;
    uint8_t *rd_buf;
    uint16_t max_pkt_len;
    uint8_t result;
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);

    buf = net_buf_alloc(&mse_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas));
    if (flag == BT_OBEX_REQ_UNSEG)
    {
        result = BT_MAP_RSP_SUCCESS;
        if (instance->tx_cnt == 0)
        {
            (void)snprintf(&instance->msg_path[0], MAP_MSE_MAX_PATH_LEN, "%s/%s", instance->root_path, MAP_MSE_XML_FOLDER_LISTING_NAME);
            if (map_create_xml_folder_listing(instance) != 0)
            {
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
        }

        if (result == BT_MAP_RSP_SUCCESS)
        {
            max_pkt_len = instance->max_pkt_len;
            max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas) - BT_L2CAP_BUF_SIZE(2U);
            max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
            rd_buf = buf->data + sizeof(struct bt_obex_hdr_bytes); /* body header */

            if ((f_open(&map_fdes, &instance->msg_path[0], FA_READ) != FR_OK) ||
                (f_lseek(&map_fdes, instance->tx_cnt) != FR_OK) ||
                (f_read(&map_fdes, rd_buf, max_pkt_len, &actual) != FR_OK))
            {
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
            else
            {
                instance->tx_cnt += actual;
                if (instance->tx_cnt < f_size(&map_fdes))
                {
                    instance->cmd_id = CMD_ID_GET_FOLDER_LISTING;
                    BT_MAP_ADD_BODY(buf, rd_buf, actual);
                    result = BT_MAP_RSP_CONTINUE;
                }
                else
                {
                    instance->cmd_id = CMD_ID_NONE;
                    instance->tx_cnt = 0;
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
        map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_GET_FOLDER_LISTING);
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_GET_FOLDER_LISTING, result, rx_buf, NULL);
}

static void app_mse_set_folder_cb(struct bt_map_mse_mas *mse_mas, char *name)
{
    uint8_t result;
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);

    if (instance == NULL)
    {
        return;
    }

    if (name != NULL)
    {
        result = BT_MAP_RSP_SUCCESS;
        if (strcmp(name, "/") == 0)
        {
            strcpy(&instance->path[0], instance->root_path);
        }
        else if (strstr(name, "../") != NULL)
        {
            if (map_dirname(&instance->path[0]) == 0)
            {
                if (name[3] != '\0')
                {
                    if ((strchr(&name[3], '/') != NULL) || (map_joinpath(&instance->path[0], &name[3]) != 0))
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
                if (map_joinpath(&instance->path[0], name) != 0)
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
        map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_SET_FOLDER);
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_SET_FOLDER, result, NULL, name);
}

static void app_mse_get_msg_listing_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag)
{
    struct net_buf *rx_buf = buf;
    UINT actual;
    uint8_t *rd_buf;
    uint16_t max_pkt_len;
    struct map_xml_msg_listing listing;
    uint8_t result;
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }

    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);

    buf = net_buf_alloc(&mse_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas));
    if (flag == BT_OBEX_REQ_UNSEG)
    {
        result = BT_MAP_RSP_SUCCESS;
        if (instance->tx_cnt == 0)
        {
            (void)snprintf(&instance->msg_path[0], MAP_MSE_MAX_PATH_LEN, "%s/%s", instance->root_path, MAP_MSE_XML_MSG_LISTING_NAME);
            if ((name == NULL) || (name[0] == '\0'))
            {
                if (map_create_xml_msg_listing(instance, &listing) != 0)
                {
                    result = BT_MAP_RSP_INT_SERVER_ERR;
                }
            }
            else if (strchr(name, '/') != NULL)
            {
                result = BT_MAP_RSP_PRECOND_FAILED;
            }
            else if (map_joinpath(&instance->path[0], name) == 0)
            {
                if (map_create_xml_msg_listing(instance, &listing) != 0)
                {
                    result = BT_MAP_RSP_INT_SERVER_ERR;
                }
                map_dirname(&instance->path[0]);
            }
            else
            {
                result = BT_MAP_RSP_PRECOND_FAILED;
            }
        }

        if (result == BT_MAP_RSP_SUCCESS)
        {
            max_pkt_len = instance->max_pkt_len;
            max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas) - BT_L2CAP_BUF_SIZE(2U);
            if (instance->tx_cnt == 0)
            {
                BT_MAP_ADD_NEW_MESSAGE(buf, (listing.unread_cnt > 0) ? 1 : 0);
                BT_MAP_ADD_MSE_TIME(buf, (uint8_t *)MAP_MSE_TIME, sizeof(MAP_MSE_TIME));
                BT_MAP_ADD_LISTING_SIZE(buf, listing.listing_size);
                max_pkt_len -= buf->len; /* application parameters */
            }
            max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
            rd_buf = buf->data + buf->len + sizeof(struct bt_obex_hdr_bytes); /* body header */

            if ((f_open(&map_fdes, &instance->msg_path[0], FA_READ) != FR_OK) ||
                (f_lseek(&map_fdes, instance->tx_cnt) != FR_OK) ||
                (f_read(&map_fdes, rd_buf, max_pkt_len, &actual) != FR_OK))
            {
                instance->tx_cnt = 0;
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
            else
            {
                instance->tx_cnt += actual;
                if (instance->tx_cnt < f_size(&map_fdes))
                {
                    instance->cmd_id = CMD_ID_GET_MSG_LISTING;
                    BT_MAP_ADD_BODY(buf, rd_buf, actual);
                    result = BT_MAP_RSP_CONTINUE;
                }
                else
                {
                    instance->cmd_id = CMD_ID_NONE;
                    instance->tx_cnt = 0;
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
        map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_GET_MSG_LISTING);
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_GET_MSG_LISTING, result, rx_buf, name);
}

static void app_mse_get_msg_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag)
{
    struct net_buf *rx_buf = buf;
    UINT actual;
    uint8_t *rd_buf;
    uint16_t max_pkt_len;
    uint8_t result;
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);

    buf = net_buf_alloc(&mse_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas));
    if (flag == BT_OBEX_REQ_UNSEG)
    {
        result = BT_MAP_RSP_SUCCESS;
        if (instance->tx_cnt == 0)
        {
            if (name == NULL)
            {
                result = BT_MAP_RSP_BAD_REQ;
            }
            else if ((name[0] == '\0') || (strspn(name, "0123456789abcdefABCDEF") != strlen(name)))
            {
                result = BT_MAP_RSP_PRECOND_FAILED;
            }
            else
            {
                strcpy(&instance->msg_path[0], &instance->path[0]);
                if (map_search_msg(&instance->msg_path[0], name) == 0)
                {
                    /* success */
                }
                else
                {
                    result = BT_MAP_RSP_PRECOND_FAILED;
                }
            }
        }

        if (result == BT_MAP_RSP_SUCCESS)
        {
            max_pkt_len = instance->max_pkt_len;
            max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas) - BT_L2CAP_BUF_SIZE(2U);
            max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
            rd_buf = buf->data + sizeof(struct bt_obex_hdr_bytes); /* body header */

            if ((f_open(&map_fdes, &instance->msg_path[0], FA_READ) != FR_OK) ||
                (f_lseek(&map_fdes, instance->tx_cnt) != FR_OK) ||
                (f_read(&map_fdes, rd_buf, max_pkt_len, &actual) != FR_OK))
            {
                instance->tx_cnt = 0;
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
            else
            {
                instance->tx_cnt += actual;
                if (instance->tx_cnt < f_size(&map_fdes))
                {
                    instance->cmd_id = CMD_ID_GET_MSG;
                    BT_MAP_ADD_BODY(buf, rd_buf, actual);
                    result = BT_MAP_RSP_CONTINUE;
                }
                else
                {
                    instance->cmd_id = CMD_ID_NONE;
                    instance->tx_cnt = 0;
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
        map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_GET_MSG);
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_GET_FOLDER_LISTING, result, rx_buf, name);
}

static void app_mse_set_msg_status_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag)
{
    uint8_t result;
    struct map_app_param_user_data user_data;
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }

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
        else
        {
            strcpy(&instance->msg_path[0], &instance->path[0]);
            if (map_search_msg(&instance->msg_path[0], name) == 0)
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
    }

    if (result == BT_MAP_RSP_SUCCESS)
    {
        user_data.id = CMD_ID_SET_MSG_STATUS;
        user_data.data = (void *)&map_msg_status;
        bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, &user_data);

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
                if (map_handle_set_msg_status(&instance->msg_path[0], &map_msg_status) != 0)
                {
                    result = BT_MAP_RSP_INT_SERVER_ERR;
                }
            }
        }
        else
        {
            result = BT_MAP_RSP_CONTINUE;
        }
    }

    if (bt_map_mse_set_msg_status_response(mse_mas, result) != 0)
    {
        map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_SET_MSG_STATUS);
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_SET_MSG_STATUS, result, buf, name);
}

static void app_mse_push_msg_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, char *name, enum bt_obex_req_flags flag)
{
    UINT actual;
    uint8_t result;
    char path[MAP_MSE_MAX_PATH_LEN];
    char msg_handle[BT_MAP_MSG_HANDLE_SIZE / 2U];
    char *name_req = NULL;
    struct map_hdr body;
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);

    result = BT_MAP_RSP_SUCCESS;
    if ((flag & BT_OBEX_REQ_START) != 0U)
    {
        strcpy(&instance->msg_path[0], &instance->path[0]);
        if ((name == NULL) || (name[0] == '\0'))
        {
            /* success */
        }
        else if (strchr(name, '/') != NULL)
        {
            result = BT_MAP_RSP_PRECOND_FAILED;
        }
        else if (map_joinpath(&instance->msg_path[0], name) == 0)
        {
            /* success */
        }
        else
        {
            result = BT_MAP_RSP_PRECOND_FAILED;
        }

        if (result == BT_MAP_RSP_SUCCESS)
        {
            /* Not use %016llX here to avoid uint64_t is not supported in some platforms. */
            (void)sprintf(&instance->msg_path[strlen(&instance->msg_path[0])], "/%08X%08X",
            (unsigned int)(instance->msg_handle >> 32U), (unsigned int)instance->msg_handle);
            if (f_open(&map_fdes, &instance->msg_path[0], FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
            {
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
            f_close(&map_fdes);
        }
    }

    if (result == BT_MAP_RSP_SUCCESS)
    {
        if (bt_map_mse_get_body(buf, &body.value, &body.length) == 0)
        {
            if ((f_open(&map_fdes, &instance->msg_path[0], FA_WRITE) != FR_OK) ||
                (f_lseek(&map_fdes, instance->tx_cnt) != FR_OK) ||
                (f_write(&map_fdes, body.value, body.length, &actual) != FR_OK) ||
                (actual < body.length))
            {
                instance->tx_cnt = 0;
                result = BT_MAP_RSP_INT_SERVER_ERR;
            }
            else
            {
                instance->tx_cnt += actual;
            }
        }
        if (result == BT_MAP_RSP_SUCCESS)
        {
            if ((flag & BT_OBEX_REQ_END) != 0U)
            {
                (void)sprintf(&msg_handle[0], "%08X%08X",
                (unsigned int)(instance->msg_handle >> 32U), (unsigned int)instance->msg_handle);
                instance->msg_handle++;
                instance->cmd_id = CMD_ID_NONE;
                instance->tx_cnt = 0;
                name_req = msg_handle;
                f_close(&map_fdes);

                /* Move message from outbox to sent */
                strcpy(&path[0], &instance->msg_path[0]);
                map_dirname(&path[0]);
                map_dirname(&path[0]);
                if (strcmp(&path[strlen(&path[0]) + 1U], "outbox") == 0)
                {
                    map_joinpath(&path[0], "sent");
                    sprintf(&path[strlen(&path[0])], "/%s", msg_handle);
                    if (f_rename(&instance->msg_path[0], &path[0]) != FR_OK)
                    {
                        result = BT_MAP_RSP_INT_SERVER_ERR;
                    }
                }
            }
            else
            {
                instance->cmd_id = CMD_ID_PUSH_MSG;
                result = BT_MAP_RSP_CONTINUE;
            }
        }
    }

    if (bt_map_mse_push_msg_response(mse_mas, result, name_req, false) != 0)
    {
        map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_PUSH_MSG);
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_PUSH_MSG, result, buf, name);
}

static void app_mse_set_ntf_reg_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    uint8_t result;
    struct map_app_param_user_data user_data;
    uint8_t ntf_status = 0xFF;
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);
    struct map_mse_mns_instance *mns_instance;

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }
    user_data.id = CMD_ID_SET_NTF_REG;
    user_data.data = (void *)&ntf_status;
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, &user_data);

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
        map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_SET_NTF_REG);
    }
    else
    {
        if (result == BT_MAP_RSP_SUCCESS)
        {
            mns_instance = map_mse_lookup_mns_instance_by_conn(instance->conn_info.acl_conn);
            if (ntf_status == 1U)
            {
                if ((mns_instance != NULL) && (mns_instance->conn_info.handle == NULL))
                {
                    mns_instance->set_ntf_reg = true;
                    bt_sdp_discover(instance->conn_info.acl_conn, &discov_map_mce);
                }
                else
                {
                    /* already connected or instance allocation failed */
                }
            }
            else
            {
                if ((mns_instance != NULL) && (mns_instance->conn_info.handle != NULL))
                {
                    (void)bt_map_mse_mns_disconnect((struct bt_map_mse_mns *)mns_instance->conn_info.handle);
                }
                else
                {
                    /* not connected or instance allocation failed */
                }
            }
        }
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_SET_NTF_REG, result, buf, NULL);
}

static void app_mse_update_inbox_cb(struct bt_map_mse_mas *mse_mas)
{
    char path[MAP_MSE_MAX_PATH_LEN];
    uint8_t result = BT_MAP_RSP_SUCCESS;
    UINT actual;
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);

    if (instance == NULL)
    {
        return;
    }

    /* write one message into inbox */
    /* Not use %016llX here to avoid uint64_t is not supported in some platforms. */
    (void)sprintf(&path[0], "%s/telecom/msg/inbox/%08X%08X", instance->root_path,
    (unsigned int)(instance->msg_handle >> 32U), (unsigned int)instance->msg_handle);
    instance->msg_handle++;
    if ((f_open(&map_fdes, &path[0], FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) ||
        (f_write(&map_fdes, MAP_MSE_MSG_UTF_8, strlen(MAP_MSE_MSG_UTF_8), &actual) != FR_OK) ||
        (actual != strlen(MAP_MSE_MSG_UTF_8)))
    {
        result = BT_MAP_RSP_INT_SERVER_ERR;
    }
    f_close(&map_fdes);

    if (bt_map_mse_update_inbox_response(mse_mas, result) != 0)
    {
        map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_UPDATE_INBOX);
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_UPDATE_INBOX, result, NULL, NULL);
}

static void app_mse_get_mas_inst_info_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    struct net_buf *rx_buf = buf;
    uint16_t max_pkt_len;
    uint8_t result;
    uint16_t actual;
    char info[] = MAP_MSE_MAS_INSTANCE_INFO;
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);

    buf = net_buf_alloc(&mse_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas));
    if (flag == BT_OBEX_REQ_UNSEG)
    {
        max_pkt_len = instance->max_pkt_len;
        max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_RESP(mse_mas) - BT_L2CAP_BUF_SIZE(2U);
        max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
        actual = strlen(info) - instance->tx_cnt;
        actual = (actual > max_pkt_len) ? actual : max_pkt_len;
        if ((instance->tx_cnt + actual) < strlen(info))
        {
            BT_MAP_ADD_BODY(buf, (uint8_t *)&info[instance->tx_cnt], actual);
            instance->tx_cnt += actual;
            instance->cmd_id = CMD_ID_GET_MAS_INST_INFO;
            result = BT_MAP_RSP_CONTINUE;
        }
        else
        {
            BT_MAP_ADD_END_OF_BODY(buf, (uint8_t *)&info[instance->tx_cnt], actual);
            instance->tx_cnt = 0;
            instance->cmd_id = CMD_ID_NONE;
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
        map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_GET_MAS_INST_INFO);
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_GET_MAS_INST_INFO, result, rx_buf, NULL);
}

static void app_mse_set_ntf_filter_cb(struct bt_map_mse_mas *mse_mas, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    uint8_t result;
    struct map_mse_mas_instance *instance = map_mse_lookup_mas_instance(mse_mas);

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }
    bt_map_mse_app_param_parse(buf, app_mse_app_param_cb, NULL);

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
        map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_SET_NTF_FILTER);
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MSE_SET_NTF_FILTER, result, buf, NULL);
}

static int app_mse_send_event(struct map_mse_mns_instance *mns_instance)
{
    struct net_buf *buf;
    uint8_t *rd_buf;
    uint16_t max_pkt_len;
    UINT actual;
    enum bt_obex_req_flags flag;

    buf = net_buf_alloc(&mse_mns_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MSE_RSV_LEN_SEND_EVENT(mse_mns, BT_OBEX_REQ_UNSEG));
    BT_MAP_ADD_MAS_INSTANCE_ID(buf, 0);
    max_pkt_len = mns_instance->mns_max_pkt_len;
    max_pkt_len -= BT_MAP_MSE_RSV_LEN_SEND_EVENT(mse_mns, BT_OBEX_REQ_UNSEG) - BT_L2CAP_BUF_SIZE(2U);
    max_pkt_len -= buf->len; /* application parameter */
    max_pkt_len -= sizeof(struct bt_obex_hdr_bytes); /* body header */
    rd_buf = buf->data + buf->len + sizeof(struct bt_obex_hdr_bytes); /* application parameter and body header */

    if ((f_open(&map_fdes, &mns_instance->msg_path[0], FA_READ) != FR_OK) ||
        (f_lseek(&map_fdes, mns_instance->mns_tx_cnt) != FR_OK) ||
        (f_read(&map_fdes, rd_buf, max_pkt_len, &actual) != FR_OK))
    {
        f_close(&map_fdes);
        net_buf_unref(buf);
        return BTP_STATUS_FAILED;
    }
    else
    {
        mns_instance->mns_tx_cnt += actual;
        if (mns_instance->mns_tx_cnt < f_size(&map_fdes))
        {
            flag = BT_OBEX_REQ_START;
            BT_MAP_ADD_BODY(buf, rd_buf, actual);
        }
        else
        {
            flag = BT_OBEX_REQ_UNSEG;
            BT_MAP_ADD_END_OF_BODY(buf, rd_buf, actual);
        }
    }
    f_close(&map_fdes);

    if (bt_map_mse_send_event((struct bt_map_mse_mns *)mns_instance->conn_info.handle, buf, flag) != 0)
    {
        net_buf_unref(buf);
        return BTP_STATUS_FAILED;
    }
    else
    {
        return BTP_STATUS_SUCCESS;
    }
}

#endif /* CONFIG_BT_MAP_MSE */

#if defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)
#define MAP_MCE_CLASS_OF_DEVICE (0x10010CU) /* Object Transfer, Computer, Laptop */

#define MAP_MCE_MAS_TX_NET_BUF_COUNT   (1U)
#define MAP_MCE_MAS_TX_NET_BUF_SIZE    (1024U + 2U) /* L2CAP I-frame Enhanced Control Field(2-byte) */

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

struct map_mce_mas_instance
{
    struct map_conn_info conn_info;
    uint32_t supported_features;
    uint16_t max_pkt_len; /* range from opcode to the end of packet */
    uint16_t tx_cnt;
    uint16_t map_version;
    uint8_t instance_id;
};

struct map_mce_mns_instance
{
    struct map_conn_info conn_info;
    uint16_t mns_max_pkt_len; /* range from opcode to the end of packet */
    uint16_t mns_tx_cnt;
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

static struct bt_sdp_attribute map_mce_attrs[] = 
{
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
struct map_mce_mas_instance mce_mas_instances[CONFIG_BT_MAP_MCE_MAS_NUM_INSTANCES];
struct map_mce_mns_instance mce_mns_instances[CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES];
NET_BUF_POOL_FIXED_DEFINE(mce_mas_tx_pool, MAP_MCE_MAS_TX_NET_BUF_COUNT, BT_L2CAP_BUF_SIZE(MAP_MCE_MAS_TX_NET_BUF_SIZE), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
static const char map_msg_example[] = MAP_MCE_MSG_G_7BIT;
static struct btp_map_mce_connect_cmd mce_mas_conn_info;

struct bt_map_mce_mas_cb mce_mas_cb = {
    .connected = map_mce_mas_connected,
    .disconnected = map_mce_mas_disconnected,
    .get_folder_listing = app_mce_get_folder_listing_cb,
    .set_folder = app_mce_set_folder_cb,
    .get_msg_listing = app_mce_get_msg_listing_cb,
    .get_msg = app_mce_get_msg_cb,
    .set_msg_status = app_mce_set_msg_status_cb,
    .push_msg = app_mce_push_msg_cb,
    .set_ntf_reg = app_mce_set_ntf_reg_cb,
    .update_inbox = app_mce_update_inbox_cb,
    .get_mas_inst_info = app_mce_get_mas_inst_info_cb,
    // .set_owner_status = app_mce_set_owner_status_cb,
    // .get_owner_status = app_mce_get_owner_status_cb,
    // .get_convo_listing = app_mce_get_convo_listing_cb,
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

static struct map_mce_mas_instance *map_mce_allocate_mas_instance(void)
{
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MAS_NUM_INSTANCES; index++)
    {
        if (mce_mas_instances[index].conn_info.handle == NULL)
        {
            memset(&mce_mas_instances[index], 0, sizeof(struct map_mce_mas_instance));
            return &mce_mas_instances[index];
        }
    }
    return NULL;
}

static struct map_mce_mns_instance *map_mce_allocate_mns_instance(void)
{
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES; index++)
    {
        if (mce_mns_instances[index].conn_info.handle == NULL)
        {
            memset(&mce_mns_instances[index], 0, sizeof(struct map_mce_mns_instance));
            return &mce_mns_instances[index];
        }
    }
    return NULL;
}

static struct map_mce_mas_instance *map_mce_lookup_mas_instance(void *handle)
{
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MAS_NUM_INSTANCES; index++)
    {
        if (mce_mas_instances[index].conn_info.handle == handle)
        {
            return &mce_mas_instances[index];
        }
    }
    return NULL;
}

static struct map_mce_mns_instance *map_mce_lookup_mns_instance(void *handle)
{
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MNS_NUM_INSTANCES; index++)
    {
        if (mce_mns_instances[index].conn_info.handle == handle)
        {
            return &mce_mns_instances[index];
        }
    }
    return NULL;
}

static void map_mce_free_mas_instance(void *handle)
{
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(handle);

    if (instance != NULL)
    {
        memset(instance, 0, sizeof(struct map_mce_mns_instance));
    }
}

static void map_mce_free_mns_instance(void *handle)
{
    struct map_mce_mns_instance *instance = map_mce_lookup_mns_instance(handle);

    if (instance != NULL)
    {
        memset(instance, 0, sizeof(struct map_mce_mns_instance));
    }
}

static struct map_mce_mas_instance *map_mce_lookup_mas_instance_by_conn_info(struct bt_conn *conn, uint8_t instance_id)
{
    for (uint8_t index = 0; index < CONFIG_BT_MAP_MCE_MAS_NUM_INSTANCES; index++)
    {
        if ((mce_mas_instances[index].conn_info.acl_conn == conn) &&
            (mce_mas_instances[index].instance_id == instance_id))
        {
            return &mce_mas_instances[index];
        }
    }
    return NULL;
}

static uint8_t sdp_map_mse_user(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
    uint16_t scn;
    uint16_t psm = 0;
    uint32_t supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V11;
    uint16_t map_version = BT_MAP_VERSION_1_1;
    uint8_t mas_instance_id = 0;
    uint8_t supported_msg_type = 0;
    const char *service_name;
    struct map_mce_mas_instance *instance;
    struct btp_map_mce_sdp_discover_ev ev;

    if ((result) && (result->resp_buf))
    {
        if (bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_RFCOMM, &scn) < 0)
        {
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
        bt_sdp_get_goep_l2cap_psm(result->resp_buf, &psm);
        bt_sdp_get_profile_version(result->resp_buf, BT_SDP_MAP_SVCLASS, &map_version);
        bt_sdp_get_pbap_map_ctn_features(result->resp_buf, &supported_features);
        bt_sdp_get_instance_id(result->resp_buf, &mas_instance_id);
        bt_sdp_get_supported_msg_type(result->resp_buf, &supported_msg_type);
        bt_sdp_get_service_name(result->resp_buf, &service_name);

        memset(&ev, 0, sizeof(ev));
        bt_addr_copy(&ev.address, bt_conn_get_dst_br(conn));
        ev.scn = scn;
        ev.psm = psm;
        ev.supported_features = supported_features;
        ev.map_version = map_version;
        ev.mas_instance_id = mas_instance_id;
        tester_event(BTP_SERVICE_ID_MAP, BTP_MAP_EV_SDP_DISCOVER, &ev, sizeof(ev));
        instance = map_mce_lookup_mas_instance_by_conn_info(conn, mas_instance_id);
        if (instance != NULL)
        {
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }

        if (mce_mas_conn_info.instance_id == mas_instance_id)
        {
            instance = map_mce_allocate_mas_instance();
            instance->conn_info.acl_conn = conn;
            instance->conn_info.psm = psm;
            instance->conn_info.scn = scn;
            instance->instance_id = mas_instance_id;
            instance->supported_features = supported_features;
            if ((psm != 0) && (mce_mas_conn_info.mode == BTP_MAP_MCE_CONNECT_L2CAP))
            {
                bt_map_mce_psm_connect(conn, psm, supported_features, (struct bt_map_mce_mas **)&instance->conn_info.handle);
            }
            else
            {
                bt_map_mce_scn_connect(conn, (uint8_t)scn, supported_features, (struct bt_map_mce_mas **)&instance->conn_info.handle);
            }
            return BT_SDP_DISCOVER_UUID_STOP;
        }
        else if (mce_mas_conn_info.instance_id == 0xFF)
        {
            instance = map_mce_allocate_mas_instance();
            instance->conn_info.acl_conn = conn;
            instance->conn_info.psm = psm;
            instance->conn_info.scn = scn;
            instance->instance_id = mas_instance_id;
            instance->supported_features = supported_features;
            if ((psm != 0) && (mce_mas_conn_info.mode == BTP_MAP_MCE_CONNECT_L2CAP))
            {
                bt_map_mce_psm_connect(conn, psm, supported_features, (struct bt_map_mce_mas **)&instance->conn_info.handle);
            }
            else
            {
                bt_map_mce_scn_connect(conn, (uint8_t)scn, supported_features, (struct bt_map_mce_mas **)&instance->conn_info.handle);
            }
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
        else
        {
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
    }
    else
    {
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
}

static uint8_t map_mce_connect(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{  
	const struct btp_map_mce_connect_cmd *cp = cmd;
    struct bt_conn *conn = bt_conn_lookup_addr_br(&cp->address);

	if (!conn) {
		return BTP_STATUS_FAILED;
	} else {
        memcpy(&mce_mas_conn_info, cp, sizeof(mce_mas_conn_info));
		bt_sdp_discover(conn, &discov_map_mse);
        return BTP_STATUS_SUCCESS;
	}
}

static uint8_t map_mce_disconnect(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
    const struct btp_map_mce_disconnect_cmd *cp = cmd;
    struct bt_conn *conn = bt_conn_lookup_addr_br(&cp->address);
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance_by_conn_info(conn, cp->instance_id);
    struct bt_map_mce_mas *mce_mas;

	if ((conn == NULL) || (instance == NULL))
    {
		return BTP_STATUS_FAILED;
	}

    mce_mas = (struct bt_map_mce_mas *)instance->conn_info.handle;
    if (bt_map_mce_disconnect(mce_mas) < 0)
    {
        return BTP_STATUS_FAILED;
    }
    return BTP_STATUS_SUCCESS;
}

uint8_t app_mce_push_msg(struct map_mce_mas_instance *instance, char *name, bool charset)
{
    struct net_buf *buf;
    enum bt_obex_req_flags flags = BT_OBEX_REQ_UNSEG;
    uint16_t max_body_len;
    uint16_t actual;
    struct bt_map_mce_mas *mce_mas = (struct bt_map_mce_mas *)instance->conn_info.handle;

    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_PUSH_MSG(mce_mas, name, flags));
    BT_MAP_ADD_CHARSET(buf, (uint8_t)charset);
    max_body_len = instance->max_pkt_len;
    max_body_len -= BT_MAP_MCE_RSV_LEN_PUSH_MSG(mce_mas, name, flags) - BT_L2CAP_BUF_SIZE(2U);
    max_body_len -= buf->len; /* application parameters */
    max_body_len -= sizeof(struct bt_obex_hdr_bytes);

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
    if (bt_map_mce_push_msg(mce_mas, buf, NULL, flags) != 0)
    {
        net_buf_unref(buf);
        return BTP_STATUS_FAILED;
    }
    else
    {
        instance->tx_cnt = actual;
        return BTP_STATUS_SUCCESS;
    }
}

static void map_mce_mns_connected(struct bt_map_mce_mns *mce_mns)
{
    struct btp_map_mce_mns_connected_ev ev;
    struct map_mce_mns_instance *mns_instance = map_mce_allocate_mns_instance();

    if (mns_instance == NULL)
    {
        return;
    }
    if (bt_map_mce_mns_get_max_pkt_len(mce_mns, &mns_instance->mns_max_pkt_len) == 0)
    {
        mns_instance->conn_info.acl_conn = default_conn;
        mns_instance->conn_info.handle = mce_mns;
        mns_instance->conn_info.psm = BT_BR_PSM_MAP_MCE;
        mns_instance->conn_info.scn = BT_RFCOMM_CHAN_MAP_MCE;
        ev.psm = BT_BR_PSM_MAP_MCE;
        ev.scn = BT_RFCOMM_CHAN_MAP_MCE;
        ev.max_pkt_len = mns_instance->mns_max_pkt_len;
        bt_addr_copy(&ev.address, bt_conn_get_dst_br(mns_instance->conn_info.acl_conn));
        tester_event(BTP_SERVICE_ID_MAP, BTP_MAP_EV_MCE_MNS_CONNECTED, &ev, sizeof(ev));
    }
}

static void map_mce_mns_disconnected(struct bt_map_mce_mns *mce_mns, uint8_t result)
{
    struct btp_map_mce_mns_disconnected_ev ev;
    struct map_mce_mns_instance *mns_instance = map_mce_lookup_mns_instance(mce_mns);

    if (mns_instance == NULL)
    {
        return;
    }
    ev.result = result;
    ev.psm = mns_instance->conn_info.psm;
    ev.scn = mns_instance->conn_info.scn;
    bt_addr_copy(&ev.address, bt_conn_get_dst_br(mns_instance->conn_info.acl_conn));
    tester_event(BTP_SERVICE_ID_MAP, BTP_MAP_EV_MCE_MNS_DISCONNECTED, &ev, sizeof(ev));
    map_mce_free_mns_instance(mce_mns);
}

static void map_mce_mas_connected(struct bt_map_mce_mas *mce_mas)
{
    struct btp_map_mce_mas_connected_ev ev;
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        return;
    }
    if (bt_map_mce_get_max_pkt_len(mce_mas, &instance->max_pkt_len) == 0)
    {
        ev.scn = instance->conn_info.scn;
        ev.psm = instance->conn_info.psm;
        ev.instance_id = instance->instance_id;
        ev.max_pkt_len = instance->max_pkt_len;
        bt_addr_copy(&ev.address, bt_conn_get_dst_br(instance->conn_info.acl_conn));
        tester_event(BTP_SERVICE_ID_MAP, BTP_MAP_EV_MCE_MAS_CONNECTED, &ev, sizeof(ev));
    }
}

static void map_mce_mas_disconnected(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    struct btp_map_mce_mas_disconnected_ev ev;
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        return;
    }
    ev.result = result;
    ev.instance_id = instance->instance_id;
    ev.scn = instance->conn_info.scn;
    ev.psm = instance->conn_info.psm;
    bt_addr_copy(&ev.address, bt_conn_get_dst_br(instance->conn_info.acl_conn));
    tester_event(BTP_SERVICE_ID_MAP, BTP_MAP_EV_MCE_MAS_DISCONNECTED, &ev, sizeof(ev));
    map_mce_free_mas_instance(mce_mas);
}

static void app_mce_set_ntf_reg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        return;
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_SET_NTF_REG, result, NULL, NULL);
}

static void app_mce_set_folder_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        return;
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_SET_FOLDER, result, NULL, NULL);
}

static void app_mce_get_folder_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_GET_FOLDER_LISTING, result, buf, NULL);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_FOLDER_LISTING(mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_folder_listing(mce_mas, buf, false, BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_GET_FOLDER_LISTING);
        }
    }
}

static void app_mce_get_msg_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_GET_MSG_LISTING, result, buf, NULL);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG_LISTING(mce_mas, NULL, BT_OBEX_REQ_END));
        if (bt_map_mce_get_msg_listing(mce_mas, buf, NULL, false, BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_GET_MSG_LISTING);
        }
    }
}

static void app_mce_get_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_GET_MSG, result, buf, NULL);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG(mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_msg(mce_mas, buf, NULL, false, BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_GET_MSG);
        }
    }
}

static void app_mce_set_msg_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        return;
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_SET_MSG_STATUS, result, NULL, NULL);
}

static void app_mce_update_inbox_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        return;
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_UPDATE_INBOX, result, NULL, NULL);
}

static void app_mce_push_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, char *name)
{
    struct net_buf *buf;
    uint16_t actual;
    enum bt_obex_req_flags flags = BT_OBEX_REQ_END;
    uint16_t max_body_len;
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        return;
    }

    if (result == BT_MAP_RSP_CONTINUE)
    {
        if (instance->tx_cnt < strlen(map_msg_example))
        {
            buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
            net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_PUSH_MSG(mce_mas, NULL, flags));
            max_body_len = instance->max_pkt_len;
            max_body_len -= BT_MAP_MCE_RSV_LEN_PUSH_MSG(mce_mas, NULL, flags) - BT_L2CAP_BUF_SIZE(2U);
            max_body_len -= sizeof(struct bt_obex_hdr_bytes);
            actual = strlen(map_msg_example) - instance->tx_cnt;
            if (actual > max_body_len)
            {
                actual = max_body_len;
                flags = BT_OBEX_REQ_CONTINUE;
            }
            if (flags == BT_OBEX_REQ_CONTINUE)
            {
                BT_MAP_ADD_BODY(buf, (uint8_t *)(map_msg_example + instance->tx_cnt), actual);
            }
            else
            {
                BT_MAP_ADD_END_OF_BODY(buf, (uint8_t *)(map_msg_example + instance->tx_cnt), actual);
            }
            if (bt_map_mce_push_msg(mce_mas, buf, NULL, flags) != 0)
            {
                net_buf_unref(buf);
                map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_PUSH_MSG);
            }
            else
            {
                instance->tx_cnt += actual;
            }
        }
        else
        {
            map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_PUSH_MSG);
        }
    }
    else
    {
        map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_PUSH_MSG, result, NULL, NULL);
    }
}

static void app_mce_get_mas_inst_info_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_GET_MAS_INST_INFO, result, buf, NULL);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MAS_INST_INFO(mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_mas_inst_info(mce_mas, buf, false, BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            map_ev_failure(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_GET_MAS_INST_INFO);
        }
    }
}

static void app_mce_set_ntf_filter_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    struct map_mce_mas_instance *instance = map_mce_lookup_mas_instance(mce_mas);

    if (instance == NULL)
    {
        return;
    }
    map_ev_success(instance->conn_info.acl_conn, instance->instance_id, BTP_MAP_EV_MCE_SET_NTF_FILTER, result, NULL, NULL);
}

static void app_mce_send_event_cb(struct bt_map_mce_mns *mce_mns, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    uint8_t result;
    struct map_mce_mns_instance *mns_instance = map_mce_lookup_mns_instance(mce_mns);

    if (mns_instance == NULL)
    {
        net_buf_unref(buf);
        return;
    }

    if (flag & BT_OBEX_REQ_END)
    {
        result = BT_MAP_RSP_SUCCESS;
    }
    else
    {
        result = BT_MAP_RSP_CONTINUE;
    }
    map_ev_success(mns_instance->conn_info.acl_conn, 0, BTP_MAP_EV_MCE_SEND_EVENT, result, buf, NULL);

    if (bt_map_mce_send_event_response(mce_mns, result, false) < 0)
    {
        map_ev_failure(mns_instance->conn_info.acl_conn, 0, BTP_MAP_EV_MCE_SEND_EVENT);
    }
}
#endif /* CONFIG_BT_MAP_MCE */

static uint8_t map_create_cmd(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
    uint8_t err = BTP_STATUS_SUCCESS;
    const struct btp_map_create_cmd_cmd *cp = cmd;
    struct bt_conn *conn = bt_conn_lookup_addr_br(&cp->address);
    struct net_buf *buf = NULL;
    void *handle;
    void *instance;

#if defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)
    if (cp->cmd != BTP_MAP_MSE_SEND_EVENT)
    {
        instance = map_mce_lookup_mas_instance_by_conn_info(conn, cp->instance_id);
    }
#endif
#if defined(CONFIG_BT_MAP_MSE) && (CONFIG_BT_MAP_MSE > 0)
    if (cp->cmd == BTP_MAP_MSE_SEND_EVENT)
    {
        instance = map_mse_lookup_mns_instance_by_conn(conn);
    }
#endif

    if ((instance == NULL) || (((struct map_conn_info *)instance)->handle == NULL))
    {
        return BTP_STATUS_FAILED;
    }
    (void)handle;
    handle = ((struct map_conn_info *)instance)->handle;

    switch (cp->cmd)
    {
#if defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)
        case BTP_MAP_MCE_SET_NTF_REG:
            buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
            net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_NTF_REG((struct bt_map_mce_mas *)handle));
            break;
        case BTP_MAP_MCE_SET_FOLDER:
            break;
        case BTP_MAP_MCE_GET_FOLDER_LISTING:
            buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
            net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_FOLDER_LISTING((struct bt_map_mce_mas *)handle, cp->flags));
            break;
        case BTP_MAP_MCE_GET_MSG_LISTING:
            buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
            net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG_LISTING((struct bt_map_mce_mas *)handle, cp->name, cp->flags));
            break;
        case BTP_MAP_MCE_GET_MSG:
            buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
            net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG((struct bt_map_mce_mas *)handle, cp->flags));
            break;
        case BTP_MAP_MCE_SET_MSG_STATUS:
            buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
            net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_MSG_STATUS((struct bt_map_mce_mas *)handle, cp->flags));
            break;
        case BTP_MAP_MCE_UPDATE_INBOX:
            break;
        case BTP_MAP_MCE_PUSH_MSG:
            break;
        case BTP_MAP_MCE_GET_MAS_INST_INFO:
            buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
            net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MAS_INST_INFO((struct bt_map_mce_mas *)handle, cp->flags));
            break;
        case BTP_MAP_MCE_SET_NTF_FILTER:
            buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
            net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_NTF_FILTER((struct bt_map_mce_mas *)handle));
            break;
#endif /* CONFIG_BT_MAP_MCE */

#if defined(CONFIG_BT_MAP_MSE) && (CONFIG_BT_MAP_MSE > 0)
        case BTP_MAP_MSE_SEND_EVENT:
        {
            struct map_event_report event_report;

            event_report.handle = "0000000000000001";
            event_report.folder = "TELECOM/MSG/INBOX";
            event_report.msg_type = "SMS_GSM";
            event_report.event_type = cp->name; /* "NewMessage", "MessageRemoved", etc */
            (void)snprintf(&((struct map_mse_mns_instance *)instance)->msg_path[0], 
                            sizeof(((struct map_mse_mns_instance *)instance)->msg_path), "%s",
                            MAP_MSE_XML_EVENT_REPORT_PATH);
            if (map_create_event_report((struct map_mse_mns_instance *)instance, &event_report) != 0)
            {
                err = BTP_STATUS_FAILED;
            }
            break;
        }
#endif /* CONFIG_BT_MAP_MSE */

        default:
            err = BTP_STATUS_FAILED;
            break;
    }

    if (err == BTP_STATUS_SUCCESS)
    {
        map_cmd.cmd = cp->cmd;
        map_cmd.flags = cp->flags;
        map_cmd.buf = buf;
        map_cmd.instance = instance;
        strncpy(map_cmd.name, cp->name, MAX_NAME_LEN);
    }

    return err;
}

static uint8_t map_execute_cmd(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
    uint8_t err = BTP_STATUS_SUCCESS;
#if defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)
    struct map_mce_mas_instance *instance = (struct map_mce_mas_instance *)map_cmd.instance;
    struct bt_map_mce_mas *mce_mas = (struct bt_map_mce_mas *)instance->conn_info.handle;
#endif

    switch (map_cmd.cmd)
    {
#if defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)
        case BTP_MAP_MCE_SET_NTF_REG:
            if (bt_map_mce_set_ntf_reg(mce_mas, map_cmd.buf) != 0)
            {
                net_buf_unref(map_cmd.buf);
                err = BTP_STATUS_FAILED;
            }
            break;
        case BTP_MAP_MCE_SET_FOLDER:
            if (bt_map_mce_set_folder(mce_mas, map_cmd.name) != 0)
            {
                err = BTP_STATUS_FAILED;
            }
            break;
        case BTP_MAP_MCE_GET_FOLDER_LISTING:
            if (bt_map_mce_get_folder_listing(mce_mas, map_cmd.buf, false, (enum bt_obex_req_flags)map_cmd.flags) != 0)
            {
                net_buf_unref(map_cmd.buf);
                err = BTP_STATUS_FAILED;
            }
            break;
        case BTP_MAP_MCE_GET_MSG_LISTING:
            if (bt_map_mce_get_msg_listing(mce_mas, map_cmd.buf, map_cmd.name, false, (enum bt_obex_req_flags)map_cmd.flags) != 0)
            {
                net_buf_unref(map_cmd.buf);
                err = BTP_STATUS_FAILED;
            }
            break;
        case BTP_MAP_MCE_GET_MSG:
            if (bt_map_mce_get_msg(mce_mas, map_cmd.buf, map_cmd.name, false, (enum bt_obex_req_flags)map_cmd.flags) != 0)
            {
                net_buf_unref(map_cmd.buf);
                err = BTP_STATUS_FAILED;
            }
            break;
        case BTP_MAP_MCE_SET_MSG_STATUS:
            if (bt_map_mce_set_msg_status(mce_mas, map_cmd.buf, map_cmd.name, (enum bt_obex_req_flags)map_cmd.flags) != 0)
            {
                net_buf_unref(map_cmd.buf);
                err = BTP_STATUS_FAILED;
            }
            break;
        case BTP_MAP_MCE_UPDATE_INBOX:
            if (bt_map_mce_update_inbox(mce_mas) != 0)
            {
                err = BTP_STATUS_FAILED;
            }
            break;
        case BTP_MAP_MCE_PUSH_MSG:
            err = app_mce_push_msg(map_cmd.instance, NULL, 0);
            break;
        case BTP_MAP_MCE_GET_MAS_INST_INFO:
            if (bt_map_mce_get_mas_inst_info(mce_mas, map_cmd.buf, false, (enum bt_obex_req_flags)map_cmd.flags) != 0)
            {
                net_buf_unref(map_cmd.buf);
                err =  BTP_STATUS_FAILED;
            }
            break;
        case BTP_MAP_MCE_SET_NTF_FILTER:
            if (bt_map_mce_set_ntf_filter(mce_mas, map_cmd.buf) != 0)
            {
                net_buf_unref(map_cmd.buf);
                err = BTP_STATUS_FAILED;
            }
            break;
#endif /* CONFIG_BT_MAP_MCE */

#if defined(CONFIG_BT_MAP_MSE) && (CONFIG_BT_MAP_MSE > 0)
        case BTP_MAP_MSE_SEND_EVENT:
            app_mse_send_event(map_cmd.instance);
            break;
#endif /* CONFIG_BT_MAP_MSE */

        default:
            err = BTP_STATUS_FAILED;
            break;
    }

    return err;
}

static uint8_t map_write_hdr(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
    uint8_t err = BTP_STATUS_SUCCESS;
    const struct btp_map_write_hdr_cmd *cp = cmd;
    struct net_buf *buf = map_cmd.buf;

    if (cmd_len <= sizeof(cp->hdr))
    {
        return BTP_STATUS_FAILED;
    }

    switch (cp->hdr)
    {
        case BT_MAP_HDR_BODY:
            BT_MAP_ADD_BODY(buf, (uint8_t *)cp->data,  cmd_len - sizeof(cp->hdr));
            break;
        case BT_MAP_HDR_END_OF_BODY:
            BT_MAP_ADD_END_OF_BODY(buf, (uint8_t *)cp->data,  cmd_len - sizeof(cp->hdr));
            break;

        default:
            err = BTP_STATUS_FAILED;
            break;
    }

    return err;
}

static uint8_t map_write_app_param(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
    uint8_t err = BTP_STATUS_SUCCESS;
    const struct btp_map_write_app_param_cmd *cp = cmd;
    struct net_buf *buf = map_cmd.buf;

    switch (cp->tag)
    {
        case BT_MAP_TAG_ID_MAX_LIST_COUNT:
        case BT_MAP_TAG_ID_LIST_START_OFFSET:
            if (cmd_len < sizeof(cp->tag) + sizeof(cp->u16))
            {
                return BTP_STATUS_FAILED;
            }
            break;

        case BT_MAP_TAG_ID_PARAMETER_MASK:
        case BT_MAP_TAG_ID_NOTIFICATION_FILTER_MASK:
        case BT_MAP_TAG_ID_CONV_PARAMETER_MASK:
        case BT_MAP_TAG_ID_MAP_SUPPORTED_FEATURES:
            if (cmd_len < sizeof(cp->tag) + sizeof(cp->u32))
            {
                return BTP_STATUS_FAILED;
            }
            break;

        default:
            if (cmd_len < sizeof(cp->tag) + sizeof(cp->u8))
            {
                return BTP_STATUS_FAILED;
            }
            break;
    }

    switch (cp->tag)
    {
        case BT_MAP_TAG_ID_MAX_LIST_COUNT:
            BT_MAP_ADD_MAX_LIST_COUNT(buf, sys_le16_to_cpu(cp->u16));
            break;
        case BT_MAP_TAG_ID_LIST_START_OFFSET:
            BT_MAP_ADD_LIST_START_OFFSET(buf, sys_le16_to_cpu(cp->u16));
            break;
        case BT_MAP_TAG_ID_FILTER_MESSAGE_TYPE:
            BT_MAP_ADD_FILTER_MESSAGE_TYPE(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_FILTER_PERIOD_BEGIN:
            BT_MAP_ADD_FILTER_PERIOD_BEGIN(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_FILTER_PERIOD_END:
            BT_MAP_ADD_FILTER_PERIOD_END(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_FILTER_READ_STATUS:
            BT_MAP_ADD_FILTER_READ_STATUS(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_FILTER_RECIPIENT:
            BT_MAP_ADD_FILTER_RECIPIENT(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_FILTER_ORIGINATOR:
            BT_MAP_ADD_FILTER_ORIGINATOR(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_FILTER_PRIORITY:
            BT_MAP_ADD_FILTER_PRIORITY(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_ATTACHMENT:
            BT_MAP_ADD_ATTACHMENT(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_TRANSPARENT:
            BT_MAP_ADD_TRANSPARENT(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_RETRY:
            BT_MAP_ADD_RETRY(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_NEW_MESSAGE:
            err = BTP_STATUS_FAILED;
            break;
        case BT_MAP_TAG_ID_NOTIFICATION_STATUS:
            BT_MAP_ADD_NOTIFICATION_STATUS(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_MAS_INSTANCE_ID:
            BT_MAP_ADD_MAS_INSTANCE_ID(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_PARAMETER_MASK:
            BT_MAP_ADD_PARAMETER_MASK(buf, sys_le32_to_cpu(cp->u32));
            break;
        case BT_MAP_TAG_ID_FOLDER_LISTING_SIZE:
        case BT_MAP_TAG_ID_LISTING_SIZE:
            err = BTP_STATUS_FAILED;
            break;
        case BT_MAP_TAG_ID_SUBJECT_LENGTH:
            BT_MAP_ADD_SUBJECT_LENGTH(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_CHARSET:
            BT_MAP_ADD_CHARSET(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_FRACTION_REQUEST:
            BT_MAP_ADD_FRACTION_REQUEST(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_FRACTION_DELIVER:
            err = BTP_STATUS_FAILED;
            break;
        case BT_MAP_TAG_ID_STATUS_INDICATOR:
            BT_MAP_ADD_STATUS_INDICATOR(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_STATUS_VALUE:
            BT_MAP_ADD_STATUS_VALUE(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_MSE_TIME:
            err = BTP_STATUS_FAILED;
            break;
        case BT_MAP_TAG_ID_DATABASE_IDENTIFIER:
        case BT_MAP_TAG_ID_CONV_LIST_VER_CNTR:
            err = BTP_STATUS_FAILED;
            break;
        case BT_MAP_TAG_ID_PRESENCE_AVAILABILITY:
            BT_MAP_ADD_PRESENCE_AVAILABILITY(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_PRESENCE_TEXT:
            BT_MAP_ADD_PRESENCE_TEXT(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_LAST_ACTIVITY:
            BT_MAP_ADD_LAST_ACTIVITY(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_BEGIN:
            BT_MAP_ADD_FILTER_LAST_ACTIVITY_BEGIN(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_FILTER_LAST_ACTIVITY_END:
            BT_MAP_ADD_FILTER_LAST_ACTIVITY_END(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_CHAT_STATE:
            BT_MAP_ADD_CHAT_STATE(buf, cp->u8);
            break;
        case BT_MAP_TAG_ID_CONVERSATION_ID:
            BT_MAP_ADD_CONVERSATION_ID(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_FOLDER_VER_CNTR:
            err = BTP_STATUS_FAILED;
            break;
        case BT_MAP_TAG_ID_FILTER_MSG_HANDLE:
            BT_MAP_ADD_FILTER_MSG_HANDLE(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_NOTIFICATION_FILTER_MASK:
            BT_MAP_ADD_NOTIFICATION_FILTER_MASK(buf, sys_le32_to_cpu(cp->u32));
            break;
        case BT_MAP_TAG_ID_CONV_PARAMETER_MASK:
            BT_MAP_ADD_CONV_PARAMETER_MASK(buf, sys_le32_to_cpu(cp->u32));
            break;
        case BT_MAP_TAG_ID_OWNER_UCI:
            err = BTP_STATUS_FAILED;
            break;
        case BT_MAP_TAG_ID_EXTENDED_DATA:
            BT_MAP_ADD_EXTENDED_DATA(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_MAP_SUPPORTED_FEATURES:
            BT_MAP_ADD_MAP_SUPPORTED_FEATURES(buf, sys_le32_to_cpu(cp->u32));
            break;
        case BT_MAP_TAG_ID_MESSAGE_HANDLE:
            BT_MAP_ADD_MESSAGE_HANDLE(buf, (uint8_t *)cp->data, cmd_len - sizeof(cp->tag));
            break;
        case BT_MAP_TAG_ID_MODIFY_TEXT:
            BT_MAP_ADD_MODIFY_TEXT(buf, cp->u8);
            break;
        default:
            err = BTP_STATUS_FAILED;
            break;
    }

    return err;
}

static const struct btp_handler handlers[] = {
#if defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)
 	{
		.opcode = BTP_MAP_MCE_DISCONNECT,
		.expect_len = sizeof(struct btp_map_mce_disconnect_cmd),
		.func = map_mce_disconnect,
	},
 	{
		.opcode = BTP_MAP_MCE_CONNECT,
		.expect_len = sizeof(struct btp_map_mce_connect_cmd),
		.func = map_mce_connect,
	},
#endif /* CONFIG_BT_MAP_MCE */
 	{
		.opcode = BTP_MAP_CREATE_CMD,
		.expect_len = BTP_HANDLER_LENGTH_VARIABLE,
		.func = map_create_cmd,
	},
 	{
		.opcode = BTP_MAP_EXECUTE_CMD,
		.expect_len = 0,
		.func = map_execute_cmd,
	},
 	{
		.opcode = BTP_MAP_WRITE_HDR,
		.expect_len = BTP_HANDLER_LENGTH_VARIABLE,
		.func = map_write_hdr,
	},
 	{
		.opcode = BTP_MAP_WRITE_APP_PARAM,
		.expect_len = BTP_HANDLER_LENGTH_VARIABLE,
		.func = map_write_app_param,
	},
};

static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err)
    {
        if (default_conn != NULL)
        {
            default_conn = NULL;
        }
    }
    else
    {
        struct bt_conn_info info;

        bt_conn_get_info(conn, &info);
        if (info.type == BT_CONN_TYPE_LE)
        {
            return;
        }
        default_conn = conn;
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    if (default_conn != conn)
    {
        return;
    }
    default_conn = NULL;
}

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};

uint8_t tester_init_map(void)
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;

#if defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0)
    /* send hci write device class cmd */
    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(MAP_MCE_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        (void)bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        return  BTP_STATUS_FAILED;
    }

    /* reigster SDP records */
    bt_sdp_register_service(&map_mce_rec);

    /* register mce callback */
    bt_map_mce_mas_register(&mce_mas_cb);
    bt_map_mce_mns_register(&mce_mns_cb);
#endif /* CONFIG_BT_MAP_MCE */

#if defined(CONFIG_BT_MAP_MSE) && (CONFIG_BT_MAP_MSE > 0)
    /* send hci write device class cmd */
    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(MAP_MSE_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        (void)bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        return  BTP_STATUS_FAILED;
    }

    /* reigster SDP records */
    bt_sdp_register_service(&map_mse_rec);

    /* register mse callback */
    bt_map_mse_mas_register(&mse_mas_cb);
    bt_map_mse_mns_register(&mse_mns_cb);
    map_fs_mount();
#endif /* CONFIG_BT_MAP_MSE */

    bt_conn_cb_register(&conn_callbacks);
    tester_register_command_handlers(BTP_SERVICE_ID_MAP, handlers,
                                    ARRAY_SIZE(handlers));

    return BTP_STATUS_SUCCESS;
}

uint8_t tester_unregister_map(void)
{
    return BTP_STATUS_SUCCESS;
}
#endif /* CONFIG_BT_MAP_MCE || CONFIG_BT_MAP_MSE > 0 */